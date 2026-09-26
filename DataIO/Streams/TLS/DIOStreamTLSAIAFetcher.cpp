/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSAIAFetcher.cpp
*
* @class      DIOSTREAMTLSAIAFETCHER
* @brief      Data Input/Output Stream TLS Authority Information Access (AIA) Fetcher class
* @ingroup    DATAIO
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

#include "DIOStreamTLSAIAFetcher.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"

#include "DIOFactory.h"
#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSAIAFETCHER::DIOSTREAMTLSAIAFETCHER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSAIAFETCHER::DIOSTREAMTLSAIAFETCHER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSAIAFETCHER::~DIOSTREAMTLSAIAFETCHER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSAIAFETCHER::~DIOSTREAMTLSAIAFETCHER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSAIAFETCHER::Fetch(XSTRING& url, XBUFFER& tobuffer, int timeout)
* @brief      Fetch the DER document published at a certificate's CA Issuers (Authority Information Access) URL
* @ingroup    DATAIO
*
* @param[in]  url : CA Issuers access location, as decoded from a certificate's AuthorityInfoAccess extension.
* @param[out] tobuffer : Fetched document body (a DER certificate, or a PKCS#7 "degenerate" certs-only bag), on
*             success. Left empty on failure.
* @param[in]  timeout : Connect / idle timeout, in seconds.
* @param[in]  maximumheadersize : Maximumheadersize value.
* @param[in]  maximumbodysize : Maximumbodysize value.
*
* @note       Deliberately narrow in scope: only plain "http://" is fetched (see DIOSTREAMTLSAIAFETCHER_DEFAULTPORT
*             above), only a single non-redirected 200 OK response is accepted, and the response is capped to
*             DIOSTREAMTLSAIAFETCHER_MAXBODYSIZE. This is a best-effort convenience that lets the client complete a
*             chain a server forgot to send an intermediate for -- it is not a general purpose HTTP client, and a
*             server that requires more than this to be reached is treated the same as a server that cannot be
*             reached at all: the caller falls back to the untrusted-root failure it already had.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSAIAFETCHER::Fetch(XSTRING& url, XBUFFER& tobuffer, int timeout, XDWORD maximumheadersize, XDWORD maximumbodysize)
{
  tobuffer.Delete();

  if(url.IsEmpty() || !maximumheadersize || !maximumbodysize) return false;
  this->maximumheadersize = maximumheadersize;
  this->maximumbodysize   = maximumbodysize;

  DIOURL requesturl;
  requesturl = url.Get();

  if(requesturl.Find(DIOURL_WEBURLID, true) != 0) return false;

  DIOSTREAMTCPIPCONFIG streamcfg;

  streamcfg.SetRemotePort(0);
  if(!streamcfg.SetFromString(requesturl.Get())) return false;

  DIOURL  hostpath;
  XSTRING server;
  XSTRING login;
  XSTRING password;
  DIOURL  resource;
  int     remoteport;

  hostpath   = streamcfg.GetRemoteURL()->Get();
  remoteport = streamcfg.GetRemotePort()?streamcfg.GetRemotePort():DIOSTREAMTLSAIAFETCHER_DEFAULTPORT;

  if(!hostpath.GetHTTPServer(server, login, password) || server.IsEmpty()) return false;
  if(!hostpath.GetHTTPResource(resource)) resource.Slash_Add();

  // AIA is deliberately restricted to the standard HTTP port.  Besides keeping this fetcher narrowly scoped,
  // this prevents an untrusted certificate from turning chain completion into an arbitrary TCP port probe.
  if(remoteport != DIOSTREAMTLSAIAFETCHER_DEFAULTPORT) return false;

  // Resolve once, validate that exact address, and connect to the validated IP instead of resolving the hostname
  // again inside the stream.  This avoids DNS rebinding between the security check and the actual connection.
  DIOURL resolvedurl;
  DIOIP  resolvedIP;

  resolvedurl = server.Get();
  if(!resolvedurl.ResolveURL(resolvedIP)) return false;
  if(!Destination_IsPublic(resolvedIP))   return false;

  XSTRING resolvedIPstring;
  if(!resolvedIP.GetXString(resolvedIPstring) || resolvedIPstring.IsEmpty()) return false;

  streamcfg.SetMode(DIOSTREAMMODE_CLIENT);
  streamcfg.GetRemoteURL()->Set(resolvedIPstring.Get());
  streamcfg.SetRemotePort(remoteport);

  DIOSTREAMTCPIP* diostream = (DIOSTREAMTCPIP*)GEN_DIOFACTORY.CreateStreamIO(&streamcfg);
  if(!diostream) return false;

  XBUFFER raw;
  bool    status = Exchange(diostream, server, resource, timeout, raw);

  GEN_DIOFACTORY.DeleteStreamIO(diostream);

  if(!status) return false;

  return ResponseBody_Extract(raw, tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSAIAFETCHER::Destination_IsPublic(DIOIP& IP)
* @brief      Check whether an AIA destination is a globally routable IPv4 address
* @note       INTERNAL.  The GEN DNS resolver used by this fetcher currently resolves A records, so IPv6 literals
*             and AAAA-only names are rejected before reaching this function rather than bypassing the policy.
* @ingroup    DATAIO
*
* @param[in]  IP : Resolved destination address.
*
* @return     bool : true only for a public unicast IPv4 destination; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSAIAFETCHER::Destination_IsPublic(DIOIP& IP)
{
  if(IP.IsEmpty() || IP.IsLocal()) return false;

  XBYTE* address = IP.Get();
  if(!address) return false;

  // Unspecified/current-network, shared CGNAT, protocol-assignment/documentation, benchmarking and non-unicast
  // ranges are not appropriate destinations for certificate-chain completion.  Rejecting them also closes the
  // obvious SSRF paths to local infrastructure and metadata endpoints.
  if(address[0] == 0)   return false;
  if(address[0] == 127) return false;                                         // complete loopback block 127.0.0.0/8

  if((address[0] == 100) && ((address[1] & 0xC0) == 0x40)) return false;       // 100.64.0.0/10

  if((address[0] == 192) && (address[1] == 0) && (address[2] == 0)) return false;
  if((address[0] == 192) && (address[1] == 0) && (address[2] == 2)) return false;
  if((address[0] == 192) && (address[1] == 88) && (address[2] == 99)) return false;

  if((address[0] == 198) && ((address[1] == 18) || (address[1] == 19))) return false;
  if((address[0] == 198) && (address[1] == 51) && (address[2] == 100)) return false;
  if((address[0] == 203) && (address[1] == 0) && (address[2] == 113)) return false;

  if(address[0] >= 224) return false;                                         // multicast + reserved/broadcast

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSAIAFETCHER::Exchange(DIOSTREAMTCPIP* diostream, XSTRING& server, XSTRING& resource, int timeout, XBUFFER& raw)
* @brief      Connect, issue a single GET, and collect the complete raw response
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  diostream : Unopened TCP/IP transport, owned by the caller.
* @param[in]  server : Value to send as the Host header.
* @param[in]  resource : Request target (the path, including a leading "/").
* @param[in]  timeout : Total operation deadline and idle timeout, in seconds.  Incoming data resets only the idle
*             timer; the total deadline is never extended.
* @param[out] raw : Complete response (status line, headers and body), on success.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSAIAFETCHER::Exchange(DIOSTREAMTCPIP* diostream, XSTRING& server, XSTRING& resource, int timeout, XBUFFER& raw)
{
  raw.Delete();

  if(!diostream) return false;

  XTIMER* timertotal = GEN_XFACTORY.CreateTimer();
  XTIMER* timeridle  = GEN_XFACTORY.CreateTimer();

  if(!timertotal || !timeridle)
    {
      if(timertotal) GEN_XFACTORY.DeleteTimer(timertotal);
      if(timeridle)  GEN_XFACTORY.DeleteTimer(timeridle);

      diostream->Close();
      return false;
    }

  timertotal->Reset();
  timeridle->Reset();

  bool status    = true;
  bool completed = false;

  if(!diostream->Open()) status = false;

  if(status)
    {
      if(!diostream->WaitToConnected(timeout)) status = false;
      if(timeout && (timertotal->GetMeasureSeconds() >= (XDWORD)timeout)) status = false;
    }

  if(status)
    {
      XSTRING request;
      request.Format(__L("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nUser-Agent: Mozilla 5.0\r\nAccept: */*\r\n\r\n"), resource.Get(), server.Get());

      if(!diostream->WriteStr(request)) status = false;

      if(status)
        {
          int remainingtimeout = timeout;

          if(timeout)
            {
              XDWORD elapsed = timertotal->GetMeasureSeconds();

              if(elapsed >= (XDWORD)timeout)
                {
                  status = false;
                }
               else
                {
                  remainingtimeout = timeout - (int)elapsed;
                }
            }

          if(status && !diostream->WaitToFlushOutXBuffer(remainingtimeout)) status = false;
          if(timeout && (timertotal->GetMeasureSeconds() >= (XDWORD)timeout)) status = false;
        }
    }

  XBYTE readbuffer[DIOSTREAMTLSAIAFETCHER_READBLOCKSIZE];

  timeridle->Reset();

  while(status && !completed)
    {
      if(timeout && (timertotal->GetMeasureSeconds() >= (XDWORD)timeout))
        {
          status = false;
          break;
        }

      XDWORD sizeread = diostream->Read(readbuffer, sizeof(readbuffer));
      if(sizeread)
        {
          timeridle->Reset();

          if(((XQWORD)raw.GetSize() + sizeread) > ((XQWORD)maximumheadersize + maximumbodysize + 4))
            {
              status = false;
              break;
            }

          if(!raw.Add(readbuffer, sizeread))
            {
              status = false;
              break;
            }
        }
       else
        {
          if(timeout && (timeridle->GetMeasureSeconds() >= (XDWORD)timeout))
            {
              status = false;
              break;
            }

          if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED)
            {
              completed = true;                          // the server closing the connection is how "Connection: close" ends
              break;
            }

          GEN_XSLEEP.MilliSeconds(1);
        }
    }

  GEN_XFACTORY.DeleteTimer(timeridle);
  GEN_XFACTORY.DeleteTimer(timertotal);

  diostream->Close();

  return status && completed && !raw.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSAIAFETCHER::ResponseBody_Extract(XBUFFER& raw, XBUFFER& tobuffer)
* @brief      Parse a complete HTTP/1.x response and extract its body
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  raw : Complete response, as collected by Exchange().
* @param[out] tobuffer : Decoded response body, on success.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSAIAFETCHER::ResponseBody_Extract(XBUFFER& raw, XBUFFER& tobuffer)
{
  tobuffer.Delete();

  if(raw.IsEmpty()) return false;

  int headerend = XBUFFER_INVALIDPOSITION;

  for(XDWORD c=0; (c+3)<raw.GetSize(); c++)
    {
      if((raw.GetByte(c)==0x0D) && (raw.GetByte(c+1)==0x0A) && (raw.GetByte(c+2)==0x0D) && (raw.GetByte(c+3)==0x0A))
        {
          headerend = c;
          break;
        }
    }

  if((headerend == XBUFFER_INVALIDPOSITION) || (headerend > maximumheadersize)) return false;

  XSTRING headertext;
  for(int c=0; c<headerend; c++)
    {
      if(!headertext.Add((XCHAR)raw.GetByte(c))) return false;
    }

  // Only a plain 200 OK is accepted -- redirects are not followed here: an AIA fetch is a best-effort convenience,
  // not a general purpose HTTP client, and following a redirect would reopen the same "which server do we now
  // trust" question this fetch exists to sidestep.
  if((headertext.Find(__L("HTTP/1.0 200"), true) != 0) && (headertext.Find(__L("HTTP/1.1 200"), true) != 0)) return false;

  bool   ischunked        = (headertext.Find(__L("Transfer-Encoding:"), true) != XSTRING_NOTFOUND) &&
                             (headertext.Find(__L("chunked"), true) != XSTRING_NOTFOUND);
  bool   hascontentlength = false;
  XQWORD contentlength    = 0;

  int lengthindex = headertext.Find(__L("Content-Length:"), true);
  if(lengthindex != XSTRING_NOTFOUND)
    {
      XDWORD position = lengthindex + XSTRING(__L("Content-Length:")).GetSize();
      bool   hasdigit  = false;

      while((position < headertext.GetSize()) &&
            ((headertext.Get()[position]==__C(' ')) || (headertext.Get()[position]==__C('\t'))))
        {
          position++;
        }

      while(position < headertext.GetSize())
        {
          XCHAR character = headertext.Get()[position];

          if((character < __C('0')) || (character > __C('9'))) break;

          XQWORD digit   = character - __C('0');
          XQWORD maximum = (XQWORD)-1;

          if(contentlength > ((maximum - digit) / 10)) return false;

          contentlength = (contentlength * 10) + digit;
          hasdigit      = true;
          position++;
        }

      hascontentlength = hasdigit;
    }

  XBUFFER body;
  if(!body.Add(raw.Get() + headerend + 4, raw.GetSize() - (headerend + 4))) return false;

  if(ischunked)
    {
      XBUFFER decoded;
      XBUFFER pending;

      if(!pending.Add(body)) return false;

      while(!pending.IsEmpty())
        {
          XQWORD chunksize = 0;

          DIOSTREAMTLSAIAFETCHER_CHUNKRESULT result = ChunkSize_Get(pending, chunksize);
          if(result == DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR)      return false;
          if(result == DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_INCOMPLETE) break;

          if(!chunksize) break;                          // final (zero-size) chunk: the body is complete

          if(((XQWORD)decoded.GetSize() + chunksize) > maximumbodysize) return false;
          if((pending.GetSize() < chunksize) || ((pending.GetSize() - chunksize) < 2)) return false;

          if(!decoded.Add(pending.Get(), (XDWORD)chunksize)) return false;

          pending.Extract(NULL, 0, (XDWORD)(chunksize + 2));
        }

      if(!tobuffer.Add(decoded)) return false;
    }
   else if(hascontentlength)
    {
      if(contentlength > maximumbodysize)   return false;
      if((XQWORD)body.GetSize() < contentlength)               return false;

      if(!tobuffer.Add(body.Get(), (XDWORD)contentlength)) return false;
    }
   else
    {
      if(body.GetSize() > maximumbodysize) return false;
      if(!tobuffer.Add(body)) return false;
    }

  return !tobuffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSAIAFETCHER_CHUNKRESULT DIOSTREAMTLSAIAFETCHER::ChunkSize_Get(XBUFFER& input, XQWORD& chunksize)
* @brief      Extract and validate one HTTP chunk-size line
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in,out] input : Pending transfer-coded bytes.
* @param[out] chunksize : Parsed chunk size.
*
* @return     DIOSTREAMTLSAIAFETCHER_CHUNKRESULT : Complete, incomplete or error.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSAIAFETCHER_CHUNKRESULT DIOSTREAMTLSAIAFETCHER::ChunkSize_Get(XBUFFER& input, XQWORD& chunksize)
{
  int lineend = XBUFFER_INVALIDPOSITION;

  chunksize = 0;

  for(XDWORD c=0; (c+1)<input.GetSize(); c++)
    {
      if((input.GetByte(c) == 0x0D) && (input.GetByte(c+1) == 0x0A))
        {
          lineend = c;
          break;
        }
    }

  if(lineend == XBUFFER_INVALIDPOSITION)
    {
      if(input.GetSize() > maximumheadersize) return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR;

      return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_INCOMPLETE;
    }

  bool   hasdigit    = false;
  bool   afterdigits = false;
  XQWORD maximum     = (XQWORD)-1;

  for(int c=0; c<lineend; c++)
    {
      XBYTE character = input.GetByte(c);

      if(character == __C(';')) break;

      if((character == __C(' ')) || (character == __C('\t')))
        {
          if(hasdigit) afterdigits = true;
          continue;
        }

      if(afterdigits) return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR;

      XBYTE digit;

      if((character >= __C('0')) && (character <= __C('9')))      digit = character - __C('0');
       else if((character >= __C('a')) && (character <= __C('f'))) digit = character - __C('a') + 10;
       else if((character >= __C('A')) && (character <= __C('F'))) digit = character - __C('A') + 10;
       else return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR;

      if(chunksize > ((maximum - digit) >> 4)) return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR;

      chunksize = (chunksize << 4) + digit;
      hasdigit  = true;
    }

  if(!hasdigit) return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR;

  input.Extract(NULL, 0, lineend+2);

  return DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_COMPLETE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSAIAFETCHER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSAIAFETCHER::Clean()
{
  maximumheadersize = DIOSTREAMTLSAIAFETCHER_MAXHEADERSIZE;
  maximumbodysize   = DIOSTREAMTLSAIAFETCHER_MAXBODYSIZE;
}
