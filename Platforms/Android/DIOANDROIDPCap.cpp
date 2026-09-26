/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDPCap.cpp
* 
* @class      DIOANDROIDPCAP
* @brief      ANDROID Data Input/Output PCap Library class
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



#ifdef DIO_PCAP_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDPCap.h"

#include "XFactory.h"
#include "XTrace.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDPCAP::DIOANDROIDPCAP() : DIOPCAP()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDPCAP::DIOANDROIDPCAP() : DIOPCAP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDPCAP::~DIOANDROIDPCAP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDPCAP::~DIOANDROIDPCAP()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDPCAP::Capture_Start(DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode, int timeout)
* @brief      Capture start
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  netinterface : Netinterface pointer to use.
* @param[in]  promiscuousmode : Promiscuousmode value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDPCAP::Capture_Start(DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode, int timeout)
{
  if(!netinterface) return false;
  if(!xfactory)     return false;

  char errbuf[PCAP_ERRBUF_SIZE];

  XBUFFER charstr;
  
  (*netinterface->GetName()).ConvertToASCII(charstr);
    
  handle = pcap_open_live(charstr.GetPtrChar(),   // name of the device
                          65536,                  // portion of the packet to capture
                          promiscuousmode?1:0,    // promiscuous mode
                          timeout,                // read timeout
                          errbuf);                // error buffer  
  if(handle == NULL) return false;


  threadcapture = CREATEXTHREAD(__L("DIOANDROIDPCAP::Capture_Start"), ThreadCapture, (void*)this);
  if(!threadcapture) return false;

  return threadcapture->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDPCAP::Capture_End()
* @brief      Capture end
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDPCAP::Capture_End()
{
  if(threadcapture)
    {
      threadcapture->End();
      DELETEXTHREAD(threadcapture);
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
* @fn         bool DIOANDROIDPCAP::CreateListNetInterfaces()
* @brief      Create list net interfaces
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDPCAP::CreateListNetInterfaces()
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
          XSTRING string;

          string = netinterface->name;          _netinterface->SetName(string);
          string = netinterface->description;   _netinterface->SetDescription(string);

          netinterfaces.Add(_netinterface);
        }
    }

  pcap_freealldevs(allnetinterfaces);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDPCAP::ThreadCapture(void* data)
* @brief      Thread capture
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDPCAP::ThreadCapture(void* data)
{
  DIOANDROIDPCAP* diopcap = (DIOANDROIDPCAP*)data;
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
* @fn         void DIOANDROIDPCAP::PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* data)
* @brief      Packet handler
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  param : Param pointer to use.
* @param[in]  header : Header value.
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDPCAP::PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* data)
{
  DIOANDROIDPCAP* diopcap = (DIOANDROIDPCAP*)param;
  if(!diopcap) return;

  diopcap->Frames_Add((XBYTE*)data, header->len);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDPCAP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDPCAP::Clean()
{
  handle        = NULL;
  threadcapture = NULL;
}




#endif
