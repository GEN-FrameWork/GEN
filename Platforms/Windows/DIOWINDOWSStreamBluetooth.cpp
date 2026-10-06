/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamBluetooth.cpp
* 
* @class      DIOWINDOWSSTREAMBLUETOOTH
* @brief      WINDOWS Data Input/Output Bluetooth class
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



#if defined(DIO_ACTIVE) && (defined(DIO_STREAMBLUETOOTH_ACTIVE) || defined(DIO_STREAMBLUETOOTHLE_ACTIVE))


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <winsock2.h>
#include <windows.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "DIOWINDOWSStreamBluetooth.h"

#include "XFactory.h"
#include "XTrace.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStreamBluetoothConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH() : DIOSTREAMBLUETOOTH() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH() : DIOSTREAMBLUETOOTH() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSBTFSMSTATE_NONE               ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_CONNECTED            ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_WAITINGTOREAD        ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_DISCONNECTING        ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, _L("DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMBLUETOOTH::~DIOWINDOWSSTREAMBLUETOOTH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTH::~DIOWINDOWSSTREAMBLUETOOTH()
{
  if(threadconnection)
    {
      threadconnection->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTH::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTH::Open()
{
  if(!threadconnection) return false;

  SetEvent(DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION);
  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTH::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTH::Disconnect()
{
  if(!IsConnected()) return false;

  if((GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION) ||
     (GetStatus()==DIOSTREAMSTATUS_CONNECTED))
     {
       SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);

       while(GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          Sleep(10);
        }
     }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTH::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTH::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(config->IsServer())
    {
      if(addr)
        {
          SDP_RegisterService(false, (char*)config->GetServerPropertys(0), (char*)config->GetServerPropertys(1), (char*)config->GetServerPropertys(2), (void*)addr);

          GEN_DELETE (SOCKADDR_BTH*)addr;
          addr = NULL;
        }

      if(handleserver!=INVALID_SOCKET)
        {
          shutdown(handleserver, SD_BOTH);
          closesocket(handleserver);
          handleserver  = INVALID_SOCKET;
        }
    }


  if(handlesocket!=INVALID_SOCKET)
    {
      DWORD nonblock = 0;
      if(ioctlsocket(handlesocket, FIONBIO, &nonblock) != SOCKET_ERROR)
        {
          shutdown(handlesocket, SD_BOTH);
          closesocket(handlesocket);

          handlesocket  = INVALID_SOCKET;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWINDOWSSTREAMBLUETOOTH::IsReadyConnect(SOCKET socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWINDOWSSTREAMBLUETOOTH::IsReadyConnect(SOCKET socket)
{
  struct timeval  tv;
  SOCKET          rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket==INVALID_SOCKET) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
  FD_SET((unsigned int)socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)socket+1, &fdr, &fdw, &fds, &tv);
  if(rc==SOCKET_ERROR) return -1;
  //if(rc>1)             return -1;

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  if(status3) return -1;
  if(((status1) || (status2)) && (rc==1))  return  1;

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMBLUETOOTH::SDP_RegisterService(bool reg, char* service_name, char* service_dsc, char* service_prov, void* addr)
* @brief      SDP register service
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  reg : Reg value.
* @param[in]  service_name : Service name pointer to use.
* @param[in]  service_dsc : Service dsc pointer to use.
* @param[in]  service_prov : Service prov pointer to use.
* @param[in]  addr : Addr pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTH::SDP_RegisterService(bool reg, char* service_name, char* service_dsc, char* service_prov, void* addr)
{
  CSADDR_INFO csaddr;
  memset(&csaddr, 0, sizeof(csaddr));

  csaddr.LocalAddr.iSockaddrLength  = sizeof(SOCKADDR_BTH);
  csaddr.LocalAddr.lpSockaddr       = (sockaddr*)addr;
  csaddr.iSocketType                = SOCK_STREAM;
  csaddr.iProtocol                  = BTHPROTO_RFCOMM;

  WSAQUERYSET   service;
  GUID          serviceID = { 0x00001101, 0x0000, 0x1000, { 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB } }; //SerialPortServiceClass_UUID;

  memset(&service, 0, sizeof(service));

  XSTRING stringservicename(service_name);
  XSTRING stringservicedsc(service_dsc);

  service.dwSize                    = sizeof(service);

  #ifdef UNICODE
  service.lpszServiceInstanceName   = stringservicename.Get();
  service.lpszComment               = stringservicedsc.Get();
  #else
  service.lpszServiceInstanceName   = service_name;
  service.lpszComment               = service_dsc;
  #endif

  service.lpServiceClassId          = &serviceID;
  service.dwNameSpace               = NS_BTH;
  service.dwNumberOfCsAddrs         = 1;
  service.lpcsaBuffer               = &csaddr;

  if(WSASetService(&service, reg?RNRSERVICE_REGISTER:RNRSERVICE_DELETE, 0) == SOCKET_ERROR) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMBLUETOOTH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTH::Clean()
{
  threadconnection    = NULL;
  status              = DIOSTREAMSTATUS_DISCONNECTED;

  addr                = NULL;
  handlesocket        = INVALID_SOCKET;
  handleserver        = INVALID_SOCKET;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMBLUETOOTH::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTH::ThreadConnection(void* param)
{
  DIOWINDOWSSTREAMBLUETOOTH* diostream = (DIOWINDOWSSTREAMBLUETOOTH*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSBTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSBTFSMSTATE_NONE              : break;

          case DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION  : switch(diostream->IsReadyConnect(diostream->handlesocket))
                                                          {
                                                            case -1:  { int sockerr    = 0;
                                                                        int sockerrlen = sizeof(sockerr);

                                                                        if(!getsockopt(diostream->handlesocket, SOL_SOCKET, SO_ERROR, (char *)&sockerr, &sockerrlen))
                                                                          {
                                                                            if(sockerr==WSAEHOSTDOWN) diostream->SetIsRefusedConnection(true);
                                                                          }

                                                                        diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      }
                                                                      break;

                                                            case  1:  diostream->SetEvent(DIOWINDOWSBTFSMEVENT_CONNECTED);
                                                                      diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                      break;

                                                            default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                      break;
                                                          }
                                                        break;

          case DIOWINDOWSBTFSMSTATE_CONNECTED         : break;

          case DIOWINDOWSBTFSMSTATE_WAITINGTOREAD     : { struct timeval  waitd;
                                                          fd_set          read_flags;
                                                          fd_set          write_flags;

                                                          waitd.tv_sec  = 0;
                                                          waitd.tv_usec = 100;

                                                          FD_ZERO(&read_flags);
                                                          FD_ZERO(&write_flags);

                                                          if(diostream->handlesocket == INVALID_SOCKET)
                                                            {
                                                              diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          if(!diostream->IsBlockRead()) FD_SET((unsigned int)diostream->handlesocket, &read_flags);
                                                          if(!diostream->IsBlockWrite())
                                                            {
                                                              if(diostream->outbuffer->GetSize()) FD_SET((unsigned int)diostream->handlesocket, &write_flags);
                                                            }

                                                          int error = select((int)(diostream->handlesocket)+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
                                                          if(error==SOCKET_ERROR)
                                                            {
                                                              diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
                                                            {
                                                              XBYTE buffer[DIOSTREAM_MAXBUFFER];


                                                              XDWORD size = recv(diostream->handlesocket, (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                              if(size<=0)
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }
                                                               else diostream->inbuffer->Add(buffer, size);

                                                              FD_CLR((unsigned int)diostream->handlesocket, &read_flags);
                                                            }

                                                          if(FD_ISSET(diostream->handlesocket, &write_flags))  //Socket ready for writing
                                                            {
                                                              int esize = diostream->outbuffer->GetSize();
                                                              if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                              if(esize)
                                                                {
                                                                  diostream->outbuffer->SetBlocked(true);
                                                                  XDWORD size = send(diostream->handlesocket, (char*)diostream->outbuffer->Get(), esize, 0);
                                                                  diostream->outbuffer->SetBlocked(false);

                                                                  if(size<=0)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;

                                                                    }
                                                                  else diostream->outbuffer->Extract(NULL, 0, size);

                                                                  FD_CLR((unsigned int)diostream->handlesocket, &write_flags);
                                                                }
                                                            }
                                                        }
                                                        break;

          case DIOWINDOWSBTFSMSTATE_SENDINGDATA       : break;

          case DIOWINDOWSBTFSMSTATE_DISCONNECTING     : break;
        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSBT_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSBTFSMSTATE_NONE              : break;

              case DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION  : { if(diostream->config->IsServer())
                                                                {
                                                                  SOCKADDR_BTH  loc_addr;
                                                                  SOCKADDR_BTH  rem_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_BTH));
                                                                  memset(&rem_addr, 0, sizeof(SOCKADDR_BTH));

                                                                  diostream->handleserver = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                  if(diostream->handleserver== INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  loc_addr.addressFamily  = AF_BTH;
                                                                  loc_addr.btAddr         = 0;
                                                                  loc_addr.serviceClassId = GUID_NULL;
                                                                  loc_addr.port           = (ULONG) -1; // config->port & 0xFF;

                                                                  bind(diostream->handleserver, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_BTH));

                                                                  int saddr = sizeof(SOCKADDR_BTH);
                                                                  if(getsockname(diostream->handleserver, (sockaddr *)&loc_addr, &saddr)==SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }


                                                                  diostream->SDP_RegisterService(true, (char*)diostream->config->GetServerPropertys(0), (char*)diostream->config->GetServerPropertys(1), (char*)diostream->config->GetServerPropertys(2), (void* )diostream->addr);

                                                                  listen(diostream->handleserver, 5);

                                                                  int sizeaddr = sizeof(SOCKADDR_BTH);
                                                                  diostream->handlesocket = accept(diostream->handleserver, (LPSOCKADDR)&rem_addr, &sizeaddr);

                                                                  DWORD nonblock = 1;
                                                                  if(ioctlsocket(diostream->handlesocket, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                    }
                                                                }
                                                               else
                                                                {
                                                                  if(!diostream->config->GetPIN()->IsEmpty())
                                                                    {
                                                                      WCHAR pass[BLUETOOTH_MAX_PASSKEY_SIZE];

                                                                      memset(pass, 0 , (BLUETOOTH_MAX_PASSKEY_SIZE*sizeof(WCHAR)));
                                                                      
                                                                      XBUFFER xbufferexchange;

                                                                      diostream->config->GetPIN()->ConvertToExchangeXBuffer(xbufferexchange);
                                                                      wcsncpy_s(pass, (WCHAR*)xbufferexchange.Get(), BLUETOOTH_MAX_PASSKEY_SIZE-sizeof(WCHAR));
                                                                      
                                                                      BLUETOOTH_DEVICE_INFO btdi;

                                                                      ZeroMemory(&btdi, sizeof(BLUETOOTH_DEVICE_INFO));

                                                                      btdi.dwSize           = sizeof(BLUETOOTH_DEVICE_INFO);
                                                                      btdi.Address.ullLong  = diostream->config->GetRemoteMAC()->GetLongNumber();

                                                                      DWORD error = BluetoothAuthenticateDevice(NULL, NULL, &btdi, pass, 4);
                                                                      if(error != ERROR_SUCCESS)
                                                                        {
                                                                          diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                          break;
                                                                        }
                                                                    }

                                                                  SOCKADDR_BTH  loc_addr;
                                                                  SOCKADDR_BTH  rem_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_BTH));
                                                                  memset(&rem_addr, 0, sizeof(SOCKADDR_BTH));

                                                                  diostream->handlesocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                  if(diostream->handlesocket == INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  loc_addr.addressFamily    = AF_BTH;
                                                                  loc_addr.serviceClassId   = GUID_NULL;
                                                                  loc_addr.btAddr           = diostream->config->GetLocalMAC()->GetLongNumber();
                                                                  loc_addr.port             = 0;

                                                                  if(bind(diostream->handlesocket, (SOCKADDR *)&loc_addr, sizeof(SOCKADDR_BTH)))
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  rem_addr.addressFamily    = AF_BTH;
                                                                  rem_addr.serviceClassId   = GUID_NULL;
                                                                  rem_addr.btAddr           = diostream->config->GetRemoteMAC()->GetLongNumber();
                                                                  rem_addr.port             = (diostream->config->GetRemoteChannel() & 0xFF);

                                                                  diostream->SetIsRefusedConnection(false);

                                                                  DWORD nonblock = 1;
                                                                  if(ioctlsocket(diostream->handlesocket, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  connect(diostream->handlesocket, (SOCKADDR *)&rem_addr, sizeof(SOCKADDR_BTH));
                                                                }
                                                            }
                                                            break;

              case DIOWINDOWSBTFSMSTATE_CONNECTED         : diostream->SetEvent(DIOWINDOWSBTFSMEVENT_WAITINGTOREAD);
                                                            break;

              case DIOWINDOWSBTFSMSTATE_WAITINGTOREAD     : break;

              case DIOWINDOWSBTFSMSTATE_SENDINGDATA       : break;

              case DIOWINDOWSBTFSMSTATE_DISCONNECTING     : diostream->threadconnection->Run(false);
                                                            diostream->status = DIOSTREAMSTATUS_DISCONNECTED;

                                                            if(!diostream->config->GetPIN()->IsEmpty())
                                                              {
                                                                BLUETOOTH_DEVICE_INFO btdi;

                                                                ZeroMemory(&btdi, sizeof(BLUETOOTH_DEVICE_INFO));
                                                                btdi.dwSize           = sizeof(BLUETOOTH_DEVICE_INFO);
                                                                btdi.Address.ullLong  = diostream->config->GetRemoteMAC()->GetLongNumber();

                                                                BluetoothRemoveDevice(&btdi.Address);
                                                              }

                                                            break;
            }
        }
    }
}




#endif

