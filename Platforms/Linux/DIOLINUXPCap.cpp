/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXPCap.cpp
* 
* @class      DIOLINUXPCAP
* @brief      LINUX Data Input/Output PCap Library class
* @ingroup    PLATFORM_LINUX
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



#ifdef DIO_PCAP_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXPCap.h"

#include "XFactory.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXPCAP::DIOLINUXPCAP() : DIOPCAP()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXPCAP::DIOLINUXPCAP() : DIOPCAP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXPCAP::~DIOLINUXPCAP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXPCAP::~DIOLINUXPCAP()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXPCAP::Capture_Start(DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode, int timeout)
* @brief      Capture start
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  netinterface : Netinterface pointer to use.
* @param[in]  promiscuousmode : Promiscuousmode value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXPCAP::Capture_Start(DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode, int timeout)
{
  if(!netinterface) return false;

  char errbuf[PCAP_ERRBUF_SIZE];

  XBUFFER ni;
  
  (*netinterface->GetName()).ConvertToASCII(ni);
  
  handle = pcap_open_live(ni.GetPtrChar(),        // name of the device
                          65536,                  // portion of the packet to capture
                          promiscuousmode?1:0,    // promiscuous mode
                          timeout,                // read timeout
                          errbuf);                // error buffer
  
  if(handle == NULL) return false;
  
  /*
  handle = pcap_create(ni.GetPtrChar(), errbuf);
  if(handle == NULL)
    {    
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[PCAP] error to create..."));  
      return false;
    }

  if(pcap_set_rfmon(handle,1) == 0)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[PCAP] error to rfmonitor..."));    
    }
  
  pcap_set_snaplen(handle, 2048);       // Set the snapshot length to 2048
  pcap_set_promisc(handle, promiscuousmode?1:0);          // Turn promiscuous mode off
  pcap_set_timeout(handle, timeout);    // Set the timeout to 512 milliseconds

  int status = pcap_activate(handle);
  
  
  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[PCAP] activate: %d"), status);
  */  

  threadcapture = CREATEXTHREAD(XTHREADGROUPID_DIOPCAP, __L("DIOLINUXPCAP::Capture_Start"), ThreadCapture, (void*)this);
  if(!threadcapture) return false;

  return threadcapture->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXPCAP::Capture_End()
* @brief      Capture end
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXPCAP::Capture_End()
{
  if(threadcapture)
    {
      threadcapture->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPCAP, threadcapture);
      threadcapture = NULL;
    }

  if(handle)
    {
      pcap_close(handle);
      handle = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXPCAP::CreateListNetInterfaces()
* @brief      Create list net interfaces
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXPCAP::CreateListNetInterfaces()
{
  pcap_if_t* allnetinterfaces;
  pcap_if_t* netinterface;
  char       errbuf[PCAP_ERRBUF_SIZE];

  // Retrieve the device list
  if(pcap_findalldevs(&allnetinterfaces, errbuf) == -1) return false;

  // Create list
  for(netinterface=allnetinterfaces; netinterface; netinterface=netinterface->next)
    {
      DIOPCAPNETINTERFACE* _netinterface = GEN_NEW DIOPCAPNETINTERFACE();
      if(_netinterface)
        {
          _netinterface->GetName()->Set(netinterface->name);
          _netinterface->GetDescription()->Set(netinterface->description);

          if(netinterface->flags & PCAP_IF_UP)
            {
              _netinterface->SetIsUp(true);
            }

          if(netinterface->flags & PCAP_IF_RUNNING)
            {
              _netinterface->SetIsRunning(true);
            }

          if(netinterface->flags & PCAP_IF_WIRELESS)
            {
              _netinterface->SetIsWireless(true);
            }

          if(netinterface->flags & PCAP_IF_LOOPBACK)
            {
              _netinterface->SetIsLoopBack(true);
            }

          netinterfaces.Add(_netinterface);
        }
    }

  pcap_freealldevs(allnetinterfaces);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXPCAP::ThreadCapture(void* data)
* @brief      Thread capture
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXPCAP::ThreadCapture(void* data)
{
  DIOLINUXPCAP* diopcap = (DIOLINUXPCAP*)data;
  if(!diopcap) return;

  if(diopcap->handle)
    {
      int error = pcap_loop(diopcap->handle, 1, PacketHandler, (u_char*)diopcap);
      if(error)
        {

        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXPCAP::PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* data)
* @brief      Packet handler
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  param : Param pointer to use.
* @param[in]  header : Header value.
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXPCAP::PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* data)
{
  DIOLINUXPCAP* diopcap = (DIOLINUXPCAP*)param;
  if(!diopcap) return;

  diopcap->Frames_Add((XBYTE*)data, header->len);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXPCAP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXPCAP::Clean()
{
  handle        = NULL;
  threadcapture = NULL;
}





#endif

