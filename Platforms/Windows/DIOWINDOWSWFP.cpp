/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSWFP.cpp
* 
* @class      DIOWINDOWSWFP
* @brief      WINDOWS Data Input/Output Windows Filtering Platform class
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSWFP.h"

#ifdef DIO_WINDOWSWFP_ACTIVE

#include <fwpmu.h>
#include <fwptypes.h>
#include <rpc.h>

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



#pragma comment(lib, "fwpuclnt.lib")



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

static const GUID DIOWINDOWSWFP_PROVIDER_GUID         = { 0x09e6cedd, 0xa796, 0x41eb, { 0xa6, 0x82, 0x42, 0xc6, 0x44, 0xfd, 0x22, 0xad } };
static const GUID DIOWINDOWSWFP_SUBLAYER_GUID         = { 0x4af724ad, 0x12e3, 0x42b6, { 0x94, 0x04, 0x88, 0x58, 0xcb, 0xcf, 0x3e, 0x76 } };



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSWFP::DIOWINDOWSWFP()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSWFP::DIOWINDOWSWFP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSWFP::~DIOWINDOWSWFP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSWFP::~DIOWINDOWSWFP()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::Ini(bool dynamicsession)
* @brief      Initialize the object
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  dynamicsession : if true, WFP objects are deleted automatically when the engine session is closed
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::Ini(bool dynamicsession)
{
  if(initialized)
    {
      return true;
    }

  FWPM_SESSION0 session;

  memset(&session, 0, sizeof(FWPM_SESSION0));

  session.displayData.name        = _L("GEN DIOWINDOWSWFP session");
  session.displayData.description = _L("GEN Windows Filtering Platform dynamic session");

  if(dynamicsession)
    {
      session.flags = FWPM_SESSION_FLAG_DYNAMIC;
    }

  lasterror = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, &session, &enginehandle);
  if(lasterror != ERROR_SUCCESS)
    {
      enginehandle = NULL;
      return false;
    }

  this->dynamicsession = dynamicsession;

  if(!Provider_Add())
    {
      End();
      return false;
    }

  if(!SubLayer_Add())
    {
      End();
      return false;
    }

  initialized = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::End()
* @brief      End the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::End()
{
  DeleteFilters();

  if(enginehandle)
    {
      if(!dynamicsession)
        {
          FwpmSubLayerDeleteByKey0(enginehandle, &DIOWINDOWSWFP_SUBLAYER_GUID);
          FwpmProviderDeleteByKey0(enginehandle, &DIOWINDOWSWFP_PROVIDER_GUID);
        }

      FwpmEngineClose0(enginehandle);
    }

  Clean();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::IsInitialized()
* @brief      Is initialized
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::IsInitialized()
{
  return initialized;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWINDOWSWFP::GetLastError()
* @brief      Get last error
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSWFP::GetLastError()
{
  return lasterror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemoteIPv4(XCHAR* remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
* @brief      Add block remote IPv4
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  remoteIP : Remote IP pointer to use.
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : optional. 0 means any protocol. Use IPPROTO_TCP/IPPROTO_UDP
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemoteIPv4(XCHAR* remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
{
  if(!remoteIP)
    {
      return false;
    }

  XSTRING _remoteIP;

  _remoteIP = remoteIP;

  return AddBlockRemoteIPv4(_remoteIP, remoteport, protocol, filtername);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemoteIPv4(XSTRING& remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
* @brief      Add block remote IPv4
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  remoteIP : Remote IP value.
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : optional. 0 means any protocol. Use IPPROTO_TCP/IPPROTO_UDP
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemoteIPv4(XSTRING& remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
{
  DIOIP IP;

  if(!IP.Set(remoteIP))
    {
      return false;
    }

  return AddBlockRemoteIPv4(IP, remoteport, protocol, filtername);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemoteIPv4(DIOIP& remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
* @brief      Add block remote IPv4
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  remoteIP : Remote IP value.
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : optional. 0 means any protocol. Use IPPROTO_TCP/IPPROTO_UDP
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemoteIPv4(DIOIP& remoteIP, XWORD remoteport, XBYTE protocol, XCHAR* filtername)
{
  if(!initialized)
    {
      return false;
    }

  if(remoteIP.IsEmpty())
    {
      return false;
    }

  FWPM_FILTER_CONDITION0 conditions[3];
  XDWORD                 nconditions = 0;

  memset(conditions, 0, sizeof(conditions));

  conditions[nconditions].fieldKey               = FWPM_CONDITION_IP_REMOTE_ADDRESS;
  conditions[nconditions].matchType              = FWP_MATCH_EQUAL;
  conditions[nconditions].conditionValue.type    = FWP_UINT32;
  conditions[nconditions].conditionValue.uint32  = IPv4ToUINT32(remoteIP);
  nconditions++;

  if(remoteport)
    {
      conditions[nconditions].fieldKey               = FWPM_CONDITION_IP_REMOTE_PORT;
      conditions[nconditions].matchType              = FWP_MATCH_EQUAL;
      conditions[nconditions].conditionValue.type    = FWP_UINT16;
      conditions[nconditions].conditionValue.uint16  = remoteport;
      nconditions++;
    }

  if(protocol)
    {
      conditions[nconditions].fieldKey               = FWPM_CONDITION_IP_PROTOCOL;
      conditions[nconditions].matchType              = FWP_MATCH_EQUAL;
      conditions[nconditions].conditionValue.type    = FWP_UINT8;
      conditions[nconditions].conditionValue.uint8   = protocol;
      nconditions++;
    }

  UINT64 filterID = 0;

  return AddFilter(FWPM_LAYER_ALE_AUTH_CONNECT_V4, conditions, nconditions, filterID, filtername?filtername:_L("GEN WFP block remote IPv4"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemotePortIPv4(XWORD remoteport, XBYTE protocol, XCHAR* filtername)
* @brief      Add block remote port IPv4
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : Protocol value.
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemotePortIPv4(XWORD remoteport, XBYTE protocol, XCHAR* filtername)
{
  return AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V4, remoteport, protocol, NULL, filtername);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemotePortIPv6(XWORD remoteport, XBYTE protocol, XCHAR* filtername)
* @brief      Add block remote port IPv6
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : Protocol value.
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemotePortIPv6(XWORD remoteport, XBYTE protocol, XCHAR* filtername)
{
  return AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V6, remoteport, protocol, NULL, filtername);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockDNSExternal(bool excludeownprocess)
* @brief      Add block external DNS. It blocks TCP/UDP port 53, optionally excluding current process path.
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  excludeownprocess : Excludeownprocess value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockDNSExternal(bool excludeownprocess)
{
  XSTRING applicationpath;

  if(excludeownprocess)
    {
      GetCurrentProcessPath(applicationpath);
    }

  return AddBlockDNSExternal(applicationpath);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockDNSExternal(XCHAR* applicationpathexception)
* @brief      Add block external DNS. It blocks TCP/UDP port 53, excluding an application path if specified.
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applicationpathexception : Applicationpathexception pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockDNSExternal(XCHAR* applicationpathexception)
{
  XSTRING _applicationpathexception;

  if(applicationpathexception)
    {
      _applicationpathexception = applicationpathexception;
    }

  return AddBlockDNSExternal(_applicationpathexception);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockDNSExternal(XSTRING& applicationpathexception)
* @brief      Add block external DNS. It blocks TCP/UDP port 53, excluding an application path if specified.
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applicationpathexception : Applicationpathexception value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockDNSExternal(XSTRING& applicationpathexception)
{
  bool status = true;

  status &= AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V4, DIOWINDOWSWFP_DNS_PORT, IPPROTO_TCP, applicationpathexception.IsEmpty()?NULL:applicationpathexception.Get(), _L("GEN WFP block external DNS TCP IPv4"));
  status &= AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V6, DIOWINDOWSWFP_DNS_PORT, IPPROTO_TCP, applicationpathexception.IsEmpty()?NULL:applicationpathexception.Get(), _L("GEN WFP block external DNS TCP IPv6"));
  status &= AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V4, DIOWINDOWSWFP_DNS_PORT, IPPROTO_UDP, applicationpathexception.IsEmpty()?NULL:applicationpathexception.Get(), _L("GEN WFP block external DNS UDP IPv4"));
  status &= AddBlockRemotePort(FWPM_LAYER_ALE_AUTH_CONNECT_V6, DIOWINDOWSWFP_DNS_PORT, IPPROTO_UDP, applicationpathexception.IsEmpty()?NULL:applicationpathexception.Get(), _L("GEN WFP block external DNS UDP IPv6"));

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::DeleteFilters()
* @brief      Delete filters created by this object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::DeleteFilters()
{
  if(!enginehandle)
    {
      filtersID.DeleteAll();
      return true;
    }

  for(int c=(int)filtersID.GetSize()-1; c>=0; c--)
    {
      UINT64 filterID = filtersID.Get(c);

      FwpmFilterDeleteById0(enginehandle, filterID);
    }

  filtersID.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::Provider_Add()
* @brief      Provider add
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::Provider_Add()
{
  FWPM_PROVIDER0 provider;

  memset(&provider, 0, sizeof(FWPM_PROVIDER0));

  provider.providerKey             = DIOWINDOWSWFP_PROVIDER_GUID;
  provider.displayData.name        = _L("GEN WFP Provider");
  provider.displayData.description = _L("GEN Windows Filtering Platform provider");

  lasterror = FwpmProviderAdd0(enginehandle, &provider, NULL);
  if((lasterror != ERROR_SUCCESS) && (lasterror != FWP_E_ALREADY_EXISTS))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::SubLayer_Add()
* @brief      Sub layer add
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::SubLayer_Add()
{
  FWPM_SUBLAYER0 sublayer;

  memset(&sublayer, 0, sizeof(FWPM_SUBLAYER0));

  sublayer.subLayerKey             = DIOWINDOWSWFP_SUBLAYER_GUID;
  sublayer.displayData.name        = _L("GEN WFP SubLayer");
  sublayer.displayData.description = _L("GEN Windows Filtering Platform sublayer");
  sublayer.providerKey             = (GUID*)&DIOWINDOWSWFP_PROVIDER_GUID;
  sublayer.weight                  = 0x100;

  lasterror = FwpmSubLayerAdd0(enginehandle, &sublayer, NULL);
  if((lasterror != ERROR_SUCCESS) && (lasterror != FWP_E_ALREADY_EXISTS))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddBlockRemotePort(const GUID& layerkey, XWORD remoteport, XBYTE protocol, XCHAR* applicationpathexception, XCHAR* filtername)
* @brief      Add block remote port
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  layerkey : Layerkey value.
* @param[in]  remoteport : Remoteport value.
* @param[in]  protocol : Use IPPROTO_TCP/IPPROTO_UDP
* @param[in]  applicationpathexception : full path of application not blocked by this filter
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddBlockRemotePort(const GUID& layerkey, XWORD remoteport, XBYTE protocol, XCHAR* applicationpathexception, XCHAR* filtername)
{
  if(!initialized)
    {
      return false;
    }

  if(!remoteport)
    {
      return false;
    }

  if(!protocol)
    {
      return false;
    }

  FWPM_FILTER_CONDITION0 conditions[3];
  XDWORD                 nconditions = 0;
  void*                  appID       = NULL;

  memset(conditions, 0, sizeof(conditions));

  conditions[nconditions].fieldKey               = FWPM_CONDITION_IP_REMOTE_PORT;
  conditions[nconditions].matchType              = FWP_MATCH_EQUAL;
  conditions[nconditions].conditionValue.type    = FWP_UINT16;
  conditions[nconditions].conditionValue.uint16  = remoteport;
  nconditions++;

  conditions[nconditions].fieldKey               = FWPM_CONDITION_IP_PROTOCOL;
  conditions[nconditions].matchType              = FWP_MATCH_EQUAL;
  conditions[nconditions].conditionValue.type    = FWP_UINT8;
  conditions[nconditions].conditionValue.uint8   = protocol;
  nconditions++;

  if(applicationpathexception)
    {
      if(GetAppIDFromPath(applicationpathexception, &appID))
        {
          conditions[nconditions].fieldKey                   = FWPM_CONDITION_ALE_APP_ID;
          conditions[nconditions].matchType                  = FWP_MATCH_NOT_EQUAL;
          conditions[nconditions].conditionValue.type        = FWP_BYTE_BLOB_TYPE;
          conditions[nconditions].conditionValue.byteBlob    = (FWP_BYTE_BLOB*)appID;
          nconditions++;
        }
    }

  UINT64 filterID = 0;
  bool   status   = AddFilter(layerkey, conditions, nconditions, filterID, filtername?filtername:_L("GEN WFP block remote port"));

  FreeAppID(&appID);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::AddFilter(const GUID& layerkey, void* filterconditions, XDWORD nfilterconditions, UINT64& filterID, XCHAR* filtername)
* @brief      Add filter
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  layerkey : Layerkey value.
* @param[in]  filterconditions : Filterconditions pointer to use.
* @param[in]  nfilterconditions : Nfilterconditions value.
* @param[in]  filterID : Filter ID value.
* @param[in]  filtername : Filtername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::AddFilter(const GUID& layerkey, void* filterconditions, XDWORD nfilterconditions, UINT64& filterID, XCHAR* filtername)
{
  if(!enginehandle)
    {
      return false;
    }

  if(!filterconditions)
    {
      return false;
    }

  if(!nfilterconditions)
    {
      return false;
    }

  FWPM_FILTER0 filter;

  memset(&filter, 0, sizeof(FWPM_FILTER0));

  filter.displayData.name        = filtername?filtername:_L("GEN WFP block filter");
  filter.displayData.description = _L("GEN Windows Filtering Platform filter");
  filter.providerKey             = (GUID*)&DIOWINDOWSWFP_PROVIDER_GUID;
  filter.layerKey                = layerkey;
  filter.subLayerKey             = DIOWINDOWSWFP_SUBLAYER_GUID;
  filter.weight.type             = FWP_UINT8;
  filter.weight.uint8            = 0x0F;
  filter.action.type             = FWP_ACTION_BLOCK;
  filter.numFilterConditions     = nfilterconditions;
  filter.filterCondition         = (FWPM_FILTER_CONDITION0*)filterconditions;

  lasterror = FwpmFilterAdd0(enginehandle, &filter, NULL, &filterID);
  if(lasterror != ERROR_SUCCESS)
    {
      return false;
    }

  filtersID.Add(filterID);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::GetCurrentProcessPath(XSTRING& path)
* @brief      Get current process path
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::GetCurrentProcessPath(XSTRING& path)
{
  XCHAR buffer[MAX_PATH];

  memset(buffer, 0, sizeof(buffer));

  if(!GetModuleFileNameW(NULL, buffer, MAX_PATH))
    {
      return false;
    }

  path = buffer;

  return !path.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::GetAppIDFromPath(XCHAR* applicationpath, void** appID)
* @brief      Get WFP application ID from path
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  appID : Application ID pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::GetAppIDFromPath(XCHAR* applicationpath, void** appID)
{
  if(!applicationpath)
    {
      return false;
    }

  if(!appID)
    {
      return false;
    }

  *appID = NULL;

  lasterror = FwpmGetAppIdFromFileName0(applicationpath, (FWP_BYTE_BLOB**)appID);
  if(lasterror != ERROR_SUCCESS)
    {
      *appID = NULL;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWFP::FreeAppID(void** appID)
* @brief      Free app ID
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  appID : Application ID pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWFP::FreeAppID(void** appID)
{
  if(!appID)
    {
      return false;
    }

  if(*appID)
    {
      FwpmFreeMemory0((void**)appID);
    }

  *appID = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWINDOWSWFP::IPv4ToUINT32(DIOIP& IP)
* @brief      IPv4 to UINT32 in WFP IPv4 address format
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  IP : IP address to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSWFP::IPv4ToUINT32(DIOIP& IP)
{
  XBYTE* address = IP.Get();

  return (((XDWORD)address[0]) << 24) | (((XDWORD)address[1]) << 16) | (((XDWORD)address[2]) << 8) | ((XDWORD)address[3]);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSWFP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSWFP::Clean()
{
  enginehandle   = NULL;
  initialized    = false;
  lasterror      = ERROR_SUCCESS;
  dynamicsession = true;

  filtersID.DeleteAll();
}


#endif

