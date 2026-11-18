/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDPing.cpp
* 
* @class      DIOANDROIDPING
* @brief      ANDROID Data Input/Output Ping class
* @ingroup    PLATFORM_ANDROID
* 
* @copyright  EndoraSoft. All rights reserved.
* 
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
* 
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDPing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "HashCRC32.h"

#include "XFactory.h"
#include "XRand.h"
#include "XSleep.h"
#include "XTrace.h"
#include "XBuffer.h"
#include "XTimer.h"

#include "DIOIP.h"
#include "DIOPing_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDPING::DIOANDROIDPING()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDPING::DIOANDROIDPING()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDPING::~DIOANDROIDPING()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDPING::~DIOANDROIDPING()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
* @brief      Execute the operation
* @ingroup    PLATFORM_ANDROID
* 
* Unprivileged Android apps cannot open SOCK_RAW (needs CAP_NET_RAW). Prefer
* SOCK_DGRAM + IPPROTO_ICMP ("ping sockets"); fall back to SOCK_RAW, then to a
* short TCP connect probe so internet-status still works when ICMP is blocked.
* 
* @param[in]  nretries : Nretries value.
* @param[in]  timebetweenchecks : Timebetweenchecks value.
* @param[in]  exitfirstgoodreply : Exitfirstgoodreply value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
{
  #define DIOPING_DEFAULTTIMEOUT 3

  if(!urltarget)         return false;
  if(!xtimer)            return false;

  XSTRING   targetIP;
  int       handle      = -1;
  bool      usedgram    = false;

  DeleteAllReplys();

  urltarget->ResolveURL(targetIP);
  if(targetIP.IsEmpty()) return false;

  // Prefer unprivileged ICMP ping sockets (API 21+ / modern kernels).
  handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
  if(handle >= 0)
    {
      usedgram = true;
    }
   else
    {
      handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
      usedgram = false;
    }

  if(handle < 0)
    {
      // Last resort: TCP connect probe (port 443 then 53). Marks connectivity without RTT/TTL.
      static const int ports[] = { 443, 53, 80 };
      for(unsigned int p = 0; p < (sizeof(ports)/sizeof(ports[0])); p++)
        {
          int tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
          if(tcp < 0) continue;

          int flags = fcntl(tcp, F_GETFL, 0);
          fcntl(tcp, F_SETFL, flags | O_NONBLOCK);

          struct sockaddr_in addr;
          memset(&addr, 0, sizeof(addr));
          addr.sin_family = AF_INET;
          addr.sin_port   = htons((uint16_t)ports[p]);

          XBUFFER charstr;
          targetIP.ConvertToASCII(charstr);
          addr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());

          int rc = connect(tcp, (struct sockaddr*)&addr, sizeof(addr));
          bool ok = false;

          if(rc == 0)
            {
              ok = true;
            }
           else if(errno == EINPROGRESS)
            {
              fd_set wfds;
              FD_ZERO(&wfds);
              FD_SET(tcp, &wfds);
              struct timeval tv;
              tv.tv_sec  = DIOPING_DEFAULTTIMEOUT;
              tv.tv_usec = 0;
              if(select(tcp + 1, NULL, &wfds, NULL, &tv) > 0)
                {
                  int soerr = 0;
                  socklen_t solen = sizeof(soerr);
                  if(getsockopt(tcp, SOL_SOCKET, SO_ERROR, &soerr, &solen) == 0 && soerr == 0)
                    {
                      ok = true;
                    }
                }
            }

          close(tcp);

          if(ok)
            {
              AddReply(1, targetIP.Get(), 0, 0, 0);
              return WasConnected();
            }
        }

      return false;
    }

  struct sockaddr_in targetaddr;
  memset(&targetaddr, 0, sizeof(targetaddr));
  targetaddr.sin_family = AF_INET;

  {
    XBUFFER charstr;
    targetIP.ConvertToASCII(charstr);
    targetaddr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());
  }

  fcntl(handle, F_SETFL, fcntl(handle, F_GETFL, 0) | O_NONBLOCK);

  DIOPING_ECHOREQUEST echorequest;
  int                 ID    = 0xCAFE;
  int                 nseq  = 2;
  int                 nloop = (int)nretries;

  while(nloop)
    {
      HASHCRC32 crc32;
      XDWORD    crc32result;

      XRAND* xrand = GEN_XFACTORY.CreateRand();
      if(!xrand) break;

      memset((XBYTE*)&echorequest, 0, sizeof(DIOPING_ECHOREQUEST));

      echorequest.icmphdr.type      = DIOPING_ICMP_ECHOREQ;
      echorequest.icmphdr.code      = 0;
      echorequest.icmphdr.ID        = (XWORD)ID;
      echorequest.icmphdr.seq       = (XWORD)nseq++;

      for(int nret = 0; nret < DIOPING_REQ_TICKETSIZE; nret++)
        {
          echorequest.cdata[nret] = (char)xrand->Max(255);
        }

      GEN_XFACTORY.DeleteRand(xrand);

      crc32.ResetResult();
      crc32.Do((XBYTE*)echorequest.cdata, DIOPING_REQ_TICKETSIZE);
      crc32result = crc32.GetResultCRC32();

      echorequest.applicationID     = applicationID;
      echorequest.time              = (XDWORD)xtimer->GetMicroSecondsTickCounter();
      echorequest.icmphdr.checksum  = CalculeCheckSum((XWORD*)&echorequest, sizeof(DIOPING_ECHOREQUEST));

      XDWORD size = (XDWORD)sendto(handle, (char*)&echorequest, sizeof(DIOPING_ECHOREQUEST), 0, (sockaddr*)&targetaddr, sizeof(struct sockaddr_in));
      if(size != sizeof(DIOPING_ECHOREQUEST))
        {
          break;
        }

      DIOPING_ECHOREQUEST echoicmp;
      DIOPING_ECHOREPLY   echoreply;
      struct sockaddr_in  originaddr;
      socklen_t           sizeaddr      = sizeof(struct sockaddr_in);
      XSTRING             fromIP;
      bool                isreplypacket = false;
      XBYTE               ttl           = 0;

      memset(&echoicmp, 0, sizeof(echoicmp));
      memset(&echoreply, 0, sizeof(echoreply));
      memset(&originaddr, 0, sizeaddr);

      xtimer->Reset();

      do{
           char recvbuf[sizeof(DIOPING_ECHOREPLY) + 64];
           int  recvsize = (int)recvfrom(handle, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&originaddr, &sizeaddr);

           if(recvsize > 0)
             {
               if(usedgram)
                 {
                   // Ping socket: kernel delivers ICMP only (no IP header).
                   if(recvsize >= (int)sizeof(DIOPING_ECHOREQUEST))
                     {
                       memcpy(&echoicmp, recvbuf, sizeof(DIOPING_ECHOREQUEST));
                       if(echoicmp.icmphdr.type == DIOPING_ICMP_ECHOREPLY)
                         {
                           fromIP = inet_ntoa(originaddr.sin_addr);
                           if(fromIP.IsEmpty()) fromIP = targetIP;
                           // Accept reply from target, or any echo reply that matches our payload
                           // (carrier NAT may rewrite source address).
                           isreplypacket = true;
                           ttl = 0;
                         }
                     }
                 }
                else
                 {
                   // SOCK_RAW: IP + ICMP echo reply.
                   if(recvsize >= (int)sizeof(DIOPING_ECHOREPLY))
                     {
                       memcpy(&echoreply, recvbuf, sizeof(DIOPING_ECHOREPLY));
                       if(echoreply.echorequest.icmphdr.type == DIOPING_ICMP_ECHOREPLY)
                         {
                           DIOIP ip;
                           ip.Set(echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b1,
                                  echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b2,
                                  echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b3,
                                  echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b4);
                           ip.GetXString(fromIP);
                           if(!fromIP.Compare(targetIP))
                             {
                               isreplypacket = true;
                               ttl = echoreply.iphdr.TTL;
                             }
                         }
                     }
                 }
             }

           if(xtimer->GetMeasureSeconds() >= DIOPING_DEFAULTTIMEOUT) break;

        } while(!isreplypacket);

      if(isreplypacket)
        {
          DIOPING_ECHOREQUEST* replyreq = usedgram ? &echoicmp : &echoreply.echorequest;

          crc32.ResetResult();
          crc32.Do((XBYTE*)replyreq->cdata, DIOPING_REQ_TICKETSIZE);

          if(crc32result == crc32.GetResultCRC32())
            {
              XWORD checksum = CalculeCheckSum((XWORD*)replyreq, sizeof(DIOPING_ECHOREQUEST));
              if(!checksum)
                {
                  if(replyreq->applicationID == applicationID)
                    {
                      XDWORD elapsed = (XDWORD)(xtimer->GetMicroSecondsTickCounter() - replyreq->time);
                      if(fromIP.IsEmpty()) fromIP = targetIP;

                      AddReply((XWORD)(nloop + 1), fromIP.Get(), DIOPING_REQ_TICKETSIZE, (elapsed / 1000), ttl);

                      DIOPING_XEVENT xevent(this, DIOPING_XEVENT_TYPE_DOPING);
                      xevent.SetPingReply((DIOPINGREPLY*)replys.Get(replys.GetSize() - 1));
                      PostEvent(&xevent);

                      if(!exitfirstgoodreply) GEN_XSLEEP.MilliSeconds(timebetweenchecks);
                    }
                }
            }
        }

      if(exitfirstgoodreply) nloop = 0; else nloop--;
    }

  close(handle);

  bool status = WasConnected();
  if(!status)
    {
      // ICMP may be filtered on the path; try TCP so internet-status / public-IP gate still works.
      static const int ports[] = { 443, 53, 80 };
      for(unsigned int p = 0; p < (sizeof(ports)/sizeof(ports[0])); p++)
        {
          int tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
          if(tcp < 0) continue;

          int flags = fcntl(tcp, F_GETFL, 0);
          fcntl(tcp, F_SETFL, flags | O_NONBLOCK);

          struct sockaddr_in addr;
          memset(&addr, 0, sizeof(addr));
          addr.sin_family = AF_INET;
          addr.sin_port   = htons((uint16_t)ports[p]);

          XBUFFER charstr;
          targetIP.ConvertToASCII(charstr);
          addr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());

          int rc = connect(tcp, (struct sockaddr*)&addr, sizeof(addr));
          bool ok = false;

          if(rc == 0)
            {
              ok = true;
            }
           else if(errno == EINPROGRESS)
            {
              fd_set wfds;
              FD_ZERO(&wfds);
              FD_SET(tcp, &wfds);
              struct timeval tv;
              tv.tv_sec  = 2;
              tv.tv_usec = 0;
              if(select(tcp + 1, NULL, &wfds, NULL, &tv) > 0)
                {
                  int soerr = 0;
                  socklen_t solen = sizeof(soerr);
                  if(getsockopt(tcp, SOL_SOCKET, SO_ERROR, &soerr, &solen) == 0 && soerr == 0)
                    {
                      ok = true;
                    }
                }
            }

          close(tcp);

          if(ok)
            {
              AddReply(1, targetIP.Get(), 0, 0, 0);
              status = WasConnected();
              break;
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDPING::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDPING::Clean()
{

}
