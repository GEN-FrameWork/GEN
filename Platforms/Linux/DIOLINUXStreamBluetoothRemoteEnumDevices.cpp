/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamBluetoothRemoteEnumDevices.cpp
* 
* @class      DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES
* @brief      LINUX Data Input/Output Stream Bluetooth Remote Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTH_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamBluetoothRemoteEnumDevices.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <netinet/in.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XVector.h"
#include "XString.h"

#include "DIOStream.h"
#include "DIOStreamDeviceBluetooth.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*  SDP_lastservice = NULL;
static char                          UUID_str[MAX_LEN_UUID_STR];



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


extern "C"
{
  static inline int   sdp_is_local_nonblock     (const bdaddr_t* device);
  sdp_session_t*      sdp_connect_nonblock      (const bdaddr_t* src, const bdaddr_t* dst, uint32_t flags);

  void                print_service_class       (void* value, void* userData);
  void                print_service_desc        (void* value, void* user);
  void                print_access_protos       (void* value, void* userData);
  void                print_service_attr        (sdp_record_t* rec);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXBTENUMFSMSTATE_NONE               ,
            DIOLINUXBTENUMFSMEVENT_SEARCHMAC          , DIOLINUXBTENUMFSMSTATE_SEARCHMAC        ,
            DIOLINUXBTENUMFSMEVENT_SEARCHEND          , DIOLINUXBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXBTENUMFSMSTATE_SEARCHMAC          ,
            DIOLINUXBTENUMFSMEVENT_SEARCHSERVICES     , DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES   ,
            DIOLINUXBTENUMFSMEVENT_SEARCHEND          , DIOLINUXBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES     ,
            DIOLINUXBTENUMFSMEVENT_SEARCHEND          , DIOLINUXBTENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXBTENUMFSMSTATE_SEARCHEND          ,
            DIOLINUXBTENUMFSMEVENT_SEARCHMAC          , DIOLINUXBTENUMFSMSTATE_SEARCHMAC        ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, _L("DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES()
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
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOLINUXBTENUMFSMEVENT_SEARCHMAC);

  threadenumdevices->Run(true);

  if(xtimerout)
    {
      xtimerout->Reset();
      while(GetCurrentState() == DIOLINUXBTENUMFSMSTATE_NONE)
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
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
* @brief      Stop search
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  waitend : Waitend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(!IsSearching()) return false;

  SetEvent(DIOLINUXBTENUMFSMEVENT_SEARCHEND);

  if(waitend)
    {
      while(GetCurrentState()!=DIOLINUXBTENUMFSMEVENT_SEARCHEND)
        {
          GEN_XSLEEP.MilliSeconds(10);
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  sleep(1000);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
* @brief      Is searching
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices) return false;

  if(!threadenumdevices->IsRunning()) return false;

  if((GetCurrentState() == DIOLINUXBTENUMFSMSTATE_SEARCHMAC) || (GetCurrentState() == DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::GetAddrFromLocalDevice(int localdeviceindex, char* btaddr)
* @brief      Get addr from local device
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  localdeviceindex : Localdeviceindex value.
* @param[in]  btaddr : Btaddr pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::GetAddrFromLocalDevice(int localdeviceindex, char* btaddr)
{
  struct hci_dev_info* di;

  di = GEN_NEW hci_dev_info();
  if(!di) return false;

  int error = hci_devinfo(localdeviceindex, di);
  if(error>=0) ba2str(&di->bdaddr, btaddr);

  GEN_DELETE di;

  if(error<0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::IsReadyConnect(int socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::IsReadyConnect(int socket)
{
  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket==-1) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
  FD_SET((unsigned int)socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select(socket+1, &fdr, &fdw, &fds, &tv);
  if(rc==-1) return -1;
  if(rc== 2) return -1;

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  if(status3) return -1;
  if(((status1) || (status2)) && (rc==1))  return  1;

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevices(XVECTOR<DIOSTREAMDEVICE*>* devices)
* @brief      Scan devices
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  devices : Devices pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevices(XVECTOR<DIOSTREAMDEVICE*>* devices)
{
  inquiry_info* info = NULL;
  int           maxanswers     = 255;  // Max answers
  int           timeout        = 6;    // 8;    // El tiempo de inquiry por dispositivo es de 1.28x8=10.24 secs/dispositivo
  int           nanswers;
  bool          status         = false;

  info = (inquiry_info*)GEN_NEW XBYTE[maxanswers * sizeof(inquiry_info)];
  if(!info) return false;

  int flags = IREQ_CACHE_FLUSH;
  int retry = 30;

  nanswers = hci_inquiry(0, timeout, maxanswers, NULL, &info, flags);

  for(int c=0;c<nanswers;c++)
    {
      inquiry_info*  infodev = (info+c);
      char           addr[32];

      DIOSTREAMDEVICEBLUETOOTH* device = GEN_NEW DIOSTREAMDEVICEBLUETOOTH();
      if(device)
        {
          uint32_t devclass;

          ba2str(&infodev->bdaddr, addr);
          device->GetMAC()->Set(addr);

          devclass = (uint32_t)(infodev->dev_class[2]) << 16 |
                     (uint32_t)(infodev->dev_class[1]) <<  8 |
                     (uint32_t)(infodev->dev_class[0]);

          device->GetServiceClass()->SetData1((XDWORD)devclass);

          ScanDevicesName(device);

          devices->Add(device);
        }

      status = true;
    }

  GEN_DELETE_ARRAY info;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesName(DIOSTREAMDEVICEBLUETOOTH* device)
* @brief      Scan devices name
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  device : Device pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesName(DIOSTREAMDEVICEBLUETOOTH* device)
{
  if(!device) return false;

  bdaddr_t bdaddr;
  XSTRING  MACstring;
  bool     status = false;

  char* name = GEN_NEW char[_MAXSTR];
  if(!name)  return false;

  memset(name, 0, _MAXSTR);
  memset(&bdaddr, 0, sizeof(bdaddr_t));

  device->GetMAC()->GetXString(MACstring);

  XBUFFER charstr;
  
  MACstring.ConvertToASCII(charstr);  
  str2ba(charstr.GetPtrChar(), &bdaddr);
  
  int handlesocket = hci_open_dev(0);
  if(handlesocket>=0)
    {
      if(hci_read_remote_name(handlesocket, &bdaddr, _MAXSTR, name, 0)>=0)
        {
          device->GetName()->ConvertFromUTF8((XBYTE*)name, strlen(name));
          status = true;
        }

      close(handlesocket);
    }

  GEN_DELETE_ARRAY name;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesServices(XVECTOR<DIOSTREAMDEVICE*>* devices)
* @brief      Scan devices services
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  devices : Devices pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesServices(XVECTOR<DIOSTREAMDEVICE*>* devices)
{
  DIOSTREAMDEVICEBLUETOOTH* device;
  int                       index = 0;

  do{ device = (DIOSTREAMDEVICEBLUETOOTH*)(devices->Get(index));
      if(!device) break;

      struct search_context context;

      uint8_t     protocol[16] = { 0x00, 0x00, 0x10 ,0x02, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };  // Public
    //uint8_t     protocol[16] = { 0x00, 0x00, 0x01 ,0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };  // L2CAP_PROTOCOL_UUID


      memset(&context, '\0', sizeof(struct search_context));

      //

      //sdp_uuid16_create(&protocol, PUBLIC_BROWSE_GROUP);

      sdp_uuid128_create(&(context.group), &protocol);


      if(!ScanDeviceServices(device, &context))
        {

        }

      index++;

    } while(device);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDeviceServices(DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context)
* @brief      Scan device services
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  device : Device pointer to use.
* @param[in]  context : Context object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDeviceServices(DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context)
{
  if(!device) return false;

  bdaddr_t        origin;
  bdaddr_t        target;
  sdp_list_t*     attrid;
  sdp_list_t*     search;
  sdp_list_t*     seq;
  sdp_list_t*     next;
  uint32_t        range = 0x0000ffff;
  sdp_session_t*  session;
  char            addrlocaldevice[DIOMAC_MAXSIZESTR];
  int             timeout   = 5*100; //seconds


  if(!GetAddrFromLocalDevice(0, addrlocaldevice)) return false;

  XSTRING MACstring;
  XBUFFER charstr;

  device->GetMAC()->GetXString(MACstring);  
  str2ba(addrlocaldevice, &origin);  
  
  MACstring.ConvertToASCII(charstr); 
  str2ba(charstr.GetPtrChar(), &target);
  
  // ----------------------------------------------------------------------
  // Special version Blocking
  session = sdp_connect(&origin, &target, SDP_RETRY_IF_BUSY);
  if(!session) return false;

  attrid = sdp_list_append(0, &range);
  search = sdp_list_append(0, &context->group);

  if(sdp_service_search_attr_req(session, search, SDP_ATTR_REQ_RANGE, attrid, &seq))
    {
      sdp_close(session);
      return false;
    }

  sdp_list_free(attrid, 0);
  sdp_list_free(search, 0);

  for(; seq; seq = next)
    {
      struct search_context   sub_context;
      size_t                  _rec = (size_t)seq->data;
      sdp_record_t*           rec  = (sdp_record_t *)_rec;

      if(_rec && ((int)_rec!=-1))
        {
          DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = GEN_NEW DIOSTREAMDEVICEBLUETOOTHSDPSERVICE();
          if(service)
            {
              sdp_data_t* data     = 0;
              sdp_list_t* list     = 0;
              sdp_list_t* proto    = 0;
              int         sizedata = 0;
              char        str[128];

              service->SetRecHandle(rec->handle);

              memset(str, 0, 128);    sdp_get_service_name (rec, str , 128-1);      service->GetName()->Set(str);
              memset(str, 0, 128);    sdp_get_service_desc (rec, str , 128-1);      service->GetDescription()->Set(str);
              memset(str, 0, 128);    sdp_get_provider_name(rec, str , 128-1);      service->GetProvider()->Set(str);

              SDP_lastservice = service;

              if(sdp_get_service_classes(rec, &list) == 0)
                {
                  sdp_list_foreach(list, print_service_class, 0);
                  sdp_list_free(list, free);
                }

              if(sdp_get_access_protos(rec, &proto) == 0)
                {
                  sdp_list_foreach(proto, print_access_protos, 0);
                  sdp_list_foreach(proto, (sdp_list_func_t)sdp_list_free, 0);
                  sdp_list_free(proto, 0);
                }

              SDP_lastservice = NULL;
              if(service->GetID())
                    device->Service_Add(service);
                else GEN_DELETE service;
            }

          if(sdp_get_group_id(rec, &sub_context.group) != -1)
            {
              memcpy(&sub_context, context, sizeof(struct search_context));
              if(sub_context.group.value.uuid16!= context->group.value.uuid16) ScanDeviceServices(device, &sub_context);
            }
        }

      next = seq->next;
      free(seq);

      sdp_record_free(rec);
    }

  sdp_close(session);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
* @brief      Thread enum devices
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{
  DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES* enumeration = (DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES*)param;
  if(!enumeration) return;

  if(enumeration->GetEvent()==DIOLINUXBTENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(enumeration->GetCurrentState())
        {
          case DIOLINUXBTENUMFSMSTATE_NONE            : break;

          case DIOLINUXBTENUMFSMSTATE_SEARCHMAC       : enumeration->SetEvent(DIOLINUXBTENUMFSMEVENT_SEARCHSERVICES);
                                                        break;

          case DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->SetEvent(DIOLINUXBTENUMFSMEVENT_SEARCHEND);
                                                        break;

          case DIOLINUXBTENUMFSMSTATE_SEARCHEND       : break;


        }
    }
   else
    {
      if(enumeration->GetEvent()<DIOLINUXBTENUM_LASTEVENT)
        {
          enumeration->CheckTransition();

          switch(enumeration->GetCurrentState())
            {
              case DIOLINUXBTENUMFSMSTATE_NONE            : break;

              case DIOLINUXBTENUMFSMSTATE_SEARCHMAC       : enumeration->ScanDevices(&enumeration->devices);
                                                            break;

              case DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->ScanDevicesServices(&enumeration->devices);
                                                            break;

              case DIOLINUXBTENUMFSMSTATE_SEARCHEND       : break;
           }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;
}


extern "C"
{


  /**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void print_service_class(void* value, void* userData)
* @brief      rint_service_class
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  value : Value pointer to use.
* @param[in]  userData : User Data pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
  void print_service_class(void* value, void* userData)
  {
    if(!SDP_lastservice) return;

    //XTRACE_PRINTCOLOR(4, _L("print_service_class"));

    uuid_t* uuid = (uuid_t*)value;
    sdp_uuid2strn(uuid, UUID_str, MAX_LEN_UUID_STR);

    XDWORD ID = 0;

    sscanf(UUID_str, "%x", &ID);
    SDP_lastservice->SetID(ID);
  }


  /**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void print_service_desc(void* value, void* user)
* @brief      rint_service_desc
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  value : Value pointer to use.
* @param[in]  user : User name.
* 
* --------------------------------------------------------------------------------------------------------------------*/
  void print_service_desc(void* value, void* user)
  {
    if(!SDP_lastservice) return;

    char        str[MAX_LEN_PROTOCOL_UUID_STR];
    sdp_data_t* p = (sdp_data_t*)value;
    int proto     = 0;
    int i         = 0;

    DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = SDP_lastservice->GetProtocol(SDP_lastservice->GetNProtocols());
    if(!protocol) return;

    for(;p; p = p->next, i++)
      {
        switch(p->dtd)
          {
            case SDP_UUID16   :
            case SDP_UUID32   :
            case SDP_UUID128  : proto = sdp_uuid_to_proto(&p->val.uuid);
                                protocol->SetType(proto);
                                break;

            case SDP_UINT8    : if(proto == RFCOMM_UUID) protocol->SetChannel(p->val.uint8);
                                break;

                      default : //XTRACE_PRINTCOLOR(1, _L("[type unknown]"));
                                break;

          }
       }

    //XTRACE_PRINTCOLOR(4, _L("Protocol: %08X %08X %d"), SDP_lastservice->GetID(), protocol->GetType(), protocol->GetChannel());

    SDP_lastservice->SetNProtocols(SDP_lastservice->GetNProtocols()+1);
  }


  /**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void print_access_protos(void* value, void* userData)
* @brief      oid print_access_protos
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  value : Value pointer to use.
* @param[in]  userData : User Data pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
  void print_access_protos(void* value, void* userData)
  {
    //XTRACE_PRINTCOLOR(4, _L("print_access_protos"));

    sdp_list_t* protDescSeq = (sdp_list_t*)value;
    sdp_list_foreach(protDescSeq, print_service_desc, 0);
  }


}




#endif

