/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamBluetoothRemoteEnumDevices.cpp
* 
* @class      DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream Bluetooth Remote Enum Devices class
* @ingroup    PLATFORM_WINDOWS
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTH_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <winsock2.h>
#include <windows.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#include <tchar.h>

#include "DIOWINDOWSStreamBluetoothRemoteEnumDevices.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamDeviceBluetooth.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSBTENUMFSMSTATE_NONE               ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC          , DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC        ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC          ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES     , DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES   ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES     ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHEND          ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC          , DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC        ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, _L("DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES()
{
  StopSearch(true);

  if(threadenumdevices)
    {
      threadenumdevices->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadenumdevices);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC);

  threadenumdevices->Run(true);

  if(xtimerout)
    {
      xtimerout->Reset();
      while(GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_NONE)
        {
          GEN_XSLEEP.MilliSeconds(10);

          if(xtimerout->GetMeasureSeconds() >= 5)
            {
              return false;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
* @brief      Stop search
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  waitend : Waitend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(!IsSearching()) return false;

  SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);

  if(waitend)
    {
      while(GetCurrentState()!=DIOWINDOWSBTENUMFSMEVENT_SEARCHEND)
        {
          GEN_XSLEEP.MilliSeconds(10);
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  Sleep(10);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
* @brief      Is searching
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices) return false;

  if(!threadenumdevices->IsRunning()) return false;

  if((GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC) || (GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* data)
* @brief      Thread enum devices
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* data)
{
  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES* enumeration = (DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES*)data;
  if(!enumeration) return;

  LPWSAQUERYSET pwsaresults   = (LPWSAQUERYSET)enumeration->buffer;
  DWORD         dwsize        = sizeof(enumeration->buffer);
  XDWORD        flagsinquiry1 =  LUP_CONTAINERS | LUP_FLUSHCACHE;
  XDWORD        flagsinquiry2 =  LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_TYPE | LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB;
  static int    index = 0;

  if(enumeration->GetEvent()==DIOWINDOWSBTENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(enumeration->GetCurrentState())
        {
          case DIOWINDOWSBTENUMFSMSTATE_NONE            : break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC       : { ZeroMemory(pwsaresults, sizeof(WSAQUERYSET));
                                                            pwsaresults->dwSize       = sizeof(WSAQUERYSET);
                                                            pwsaresults->dwNameSpace  = NS_BTH;
                                                            pwsaresults->lpBlob       = NULL;

                                                            if(WSALookupServiceNext(enumeration->hlookup, flagsinquiry2 , &dwsize, pwsaresults)!=ERROR_SUCCESS)
                                                              {
                                                                WSALookupServiceEnd(enumeration->hlookup);
                                                                enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES);
                                                                break;
                                                              }

                                                            DIOSTREAMDEVICEBLUETOOTH* device = GEN_NEW DIOSTREAMDEVICEBLUETOOTH();
                                                            if(!device) break;

                                                            XSTRING MAC;

                                                            device->SetIndex(index++);

                                                            MAC.Format(_L("%02X:%02X:%02X:%02X:%02X:%02X"), (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[5], (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[4], (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[3], (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[2], (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[1], (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[0]);
                                                            device->GetMAC()->Set(MAC);

                                                            device->GetName()->Set(pwsaresults->lpszServiceInstanceName);

                                                            device->GetServiceClass()->Set(pwsaresults->lpServiceClassId->Data1, pwsaresults->lpServiceClassId->Data2, pwsaresults->lpServiceClassId->Data3, pwsaresults->lpServiceClassId->Data4[0], pwsaresults->lpServiceClassId->Data4[1], &pwsaresults->lpServiceClassId->Data4[2]);
                                                            enumeration->devices.Add(device);
                                                          }
                                                          break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                          break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHEND       : return;
        }
    }
   else
    {
      if(enumeration->GetEvent() < DIOWINDOWSBTENUM_LASTEVENT)
        {
          enumeration->CheckTransition();

          switch(enumeration->GetCurrentState())
            {
              case DIOWINDOWSBTENUMFSMSTATE_NONE            : break;

              case DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC       : { enumeration->DelAllDevices();

                                                                index = 0;

                                                                SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                if(sock == INVALID_SOCKET)
                                                                  {
                                                                    enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                                    break;
                                                                  }

                                                                WSAPROTOCOL_INFO protocolinfo;
                                                                int protocolinfosize = sizeof(protocolinfo);

                                                                if(getsockopt(sock , SOL_SOCKET
                                                                                   , SO_PROTOCOL_INFO
                                                                                   , (char*)&protocolinfo
                                                                                   , &protocolinfosize)!=0)
                                                                  {
                                                                    enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                                    break;
                                                                  }

                                                                ZeroMemory(&enumeration->wsaq, sizeof(enumeration->wsaq));

                                                                enumeration->wsaq.dwSize       = sizeof(enumeration->wsaq);
                                                                enumeration->wsaq.dwNameSpace = NS_BTH;
                                                                enumeration->wsaq.lpcsaBuffer = NULL;

                                                                int error = WSALookupServiceBegin(&enumeration->wsaq, flagsinquiry1, &enumeration->hlookup);
                                                                if(error!=ERROR_SUCCESS) break;
                                                              }
                                                              break;


              case DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->SearchServices();
                                                              break;

              case DIOWINDOWSBTENUMFSMSTATE_SEARCHEND       : enumeration->threadenumdevices->Run(false);
                                                              break;
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::SearchServices()
* @brief      Search services
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::SearchServices()
{
  for(int c=0;c<(int)devices.GetSize();c++)
    {
      DIOSTREAMDEVICEBLUETOOTH* device  = (DIOSTREAMDEVICEBLUETOOTH*)devices.Get(c);
      if(!device) continue;

      HANDLE       hlookup;
      WSAQUERYSET* qs;
      DWORD        qssize;
      GUID         protocol = L2CAP_PROTOCOL_UUID;
      DWORD        flags    = LUP_FLUSHCACHE | LUP_RETURN_ALL;
      XSTRING      MACstring;
      bool         done;

      qssize = sizeof(WSAQUERYSET);
      qs = (WSAQUERYSET*)GEN_NEW XBYTE[qssize];

      ZeroMemory( qs, qssize );

      qs->dwSize            = sizeof(WSAQUERYSET);

      device->GetMAC()->GetXString(MACstring);

      qs->lpServiceClassId  = (LPGUID)&protocol;
    //qs->lpServiceClassId  = (LPGUID)&PublicBrowseGroupServiceClass_UUID;
      qs->dwNameSpace       = NS_BTH;
      qs->dwNumberOfCsAddrs = 0;
      qs->lpszContext       = (LPWSTR)MACstring.Get();

      if(WSALookupServiceBegin( qs, flags, &hlookup) == SOCKET_ERROR)
        {
          //int error = WSAGetLastError();
          //XTRACE_PRINT(_L("Search SDP Services Error %d"), error);

          GEN_DELETE_ARRAY qs;

          continue;
        }

      done = false;
      while(!done)
        {
          if(SOCKET_ERROR ==  WSALookupServiceNext(hlookup, flags, &qssize, qs))
            {
              int error = WSAGetLastError();
              if( error == WSAEFAULT )
                {
                  GEN_DELETE_ARRAY qs;
                  qs = (WSAQUERYSET*)GEN_NEW XBYTE[qssize];
                }
               else
                {
                  if(error == WSA_E_NO_MORE )
                    {
                      done = true;
                    }
                   else
                    {
                      break;
                    }
                }
            }
           else
            {
              DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = GEN_NEW DIOSTREAMDEVICEBLUETOOTHSDPSERVICE();
              if(service)
                {
                  service->GetName()->Set(qs->lpszServiceInstanceName);
                  service->GetDescription()->Set(qs->lpszComment);

                  const BLOB* pblob = (BLOB*)qs->lpBlob;

                  if(pblob) BluetoothSdpEnumAttributes(pblob->pBlobData, pblob->cbSize, SDP_ServiceCallback, service);

                  device->Service_Add(service);
                }
            }
        }

      GEN_DELETE_ARRAY qs;

      WSALookupServiceEnd(hlookup);

    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL __stdcall SDP_ServiceCallback(ULONG attribID, LPBYTE valuestream, ULONG cbstreamsize, LPVOID vparam)
* @brief      _stdcall SDP_ServiceCallback
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  attribID : Attrib ID value.
* @param[in]  valuestream : Valuestream value.
* @param[in]  cbstreamsize : Cbstreamsize value.
* @param[in]  vparam : Vparam value.
* 
* @return     BOOL __stdcall : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL __stdcall SDP_ServiceCallback(ULONG attribID, LPBYTE valuestream, ULONG cbstreamsize, LPVOID vparam)
{
  DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = (DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*)vparam;
  if(!service) return false;

  switch(attribID)
    {
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDHANDLE                 : { SDP_ELEMENT_DATA  data;
                                                                                    int               result;

                                                                                    result = BluetoothSdpGetElementData(valuestream, cbstreamsize, &data);
                                                                                    if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                     else
                                                                                      {
                                                                                        if(result != ERROR_SUCCESS) break;
                                                                                      }

                                                                                     service->SetRecHandle(data.data.uint32);
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICECLASSIDLIST                  : { SDP_ELEMENT_DATA              data;
                                                                                    HBLUETOOTH_CONTAINER_ELEMENT  element = NULL;
                                                                                    int                           result;

                                                                                    while(TRUE)
                                                                                      {
                                                                                        result = BluetoothSdpGetContainerElementData(valuestream, cbstreamsize, (HBLUETOOTH_CONTAINER_ELEMENT*)&element, &data);
                                                                                        if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                          else
                                                                                           {
                                                                                             if(result != ERROR_SUCCESS) break;
                                                                                           }

                                                                                        service->SetID((DWORD)data.data.uint32);
                                                                                      }
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDSTATE                  : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEID                           : break;


      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROTOCOLDESCRIPTORLIST              : { SDP_ELEMENT_DATA              data;
                                                                                    HBLUETOOTH_CONTAINER_ELEMENT  element = NULL;

                                                                                    while(TRUE)
                                                                                      {
                                                                                        int result = BluetoothSdpGetContainerElementData(valuestream, cbstreamsize, (HBLUETOOTH_CONTAINER_ELEMENT*)&element, &data);

                                                                                        if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                          else
                                                                                            {
                                                                                              if(result != ERROR_SUCCESS) break;
                                                                                            }


                                                                                        if(data.type == SDP_TYPE_SEQUENCE)
                                                                                          {

                                                                                            //---------------------------------------------------
                                                                                            /*
                                                                                            XBUFFER xbuffer(BLUETOOTH_xfactory);

                                                                                            for(ULONG c=0; c<data.data.sequence.length; c++)
                                                                                              xbuffer.Add((XBYTE)data.data.sequence.value[c]);

                                                                                             XTRACE_PRINTCOLOR(1, _L("Sequence: "));
                                                                                             XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, xbuffer);
                                                                                            */
                                                                                            //---------------------------------------------------

                                                                                            BYTE* buffer  = (BYTE*)element;
                                                                                            
                                                                                            if(buffer[0] == 0x35)
                                                                                              {
                                                                                                DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = service->GetProtocol(service->GetNProtocols());
                                                                                                if(protocol)
                                                                                                  {
                                                                                                    XDWORD type    = (XDWORD)((buffer[3]<<8)|(buffer[4]));
                                                                                                    XBYTE  size    = (XBYTE)buffer[1];
                                                                                                    XWORD  channel = 0;

                                                                                                    if(size>3) channel = (WORD)buffer[7];

                                                                                                    protocol->SetType(type);
                                                                                                    protocol->SetChannel(channel);

                                                                                                    service->SetNProtocols(service->GetNProtocols()+1);
                                                                                                  }
                                                                                              }
                                                                                          }
                                                                                      }


                                                                                  }
                                                                                  break;


      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BROWSEGROUPLIST                     : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_LANGUAGEBASEATTRIBUTEIDLIST         : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEINFOTIMETOLIVE               : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEAVAILABILITY                 : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BLUETOOTHPROFILEDESCRIPTORLIST      : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DOCUMENTATIONURL                    : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_CLIENTEXECUTABLEURL                 : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ICONURL                             : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ADDITIONALPROTOCOLDESCRIPTORLISTS   : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NAMELABELSERVICE                    :
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DESCRIPTIONSERVICE                  :
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROVIDERNAMESERVICE                 : { SDP_ELEMENT_DATA data;
                                                                                    int              result;

                                                                                    result = BluetoothSdpGetElementData(valuestream, cbstreamsize, &data);

                                                                                    if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                     else
                                                                                      {
                                                                                        if(result != ERROR_SUCCESS) break;
                                                                                      }

                                                                                    XSTRING name((char*)data.data.string.value);
                                                                                    name.AdjustSize(data.data.string.length, false, _L(" "), false);

                                                                                    switch(attribID)
                                                                                      {
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NAMELABELSERVICE    : service->GetName()->Set(name);         break;
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DESCRIPTIONSERVICE  : service->GetDescription()->Set(name);  break;
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROVIDERNAMESERVICE : service->GetProvider()->Set(name);     break;
                                                                                      }
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_GROUPID                             : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEVERSION                      : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_EXTERNALNETWORK                     : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_REMOTEAUDIOVOLUMECONTROL            : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFORMATSLIST                : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_AUDIOFEEDBACKSUPPORT                : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETWORKADDRESS                      : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPGATEWAY                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_HOMEPAGEURL                         : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPSTACKTYPE                        : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SECURITYDESCRIPTION                 : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETACCESSTYPE                       : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_MAXNETACCESSRATE                    : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV4SUBNET                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV6SUBNET                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDCAPABALITIES               : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFEATURES                   : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFUNCTIONS                  : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_TOTALIMAGINGDATACAPACITY            : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;
}




#endif


