/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFLOWCFG.h
* 
* @class      APPFLOWCFG
* @brief      Application Flow Config class
* @ingroup    APPFLOW
*
* @note       if use defines to active for the blocks of the config.
*
*             APPFLOW_CFG_REMOTEFILE_ACTIVE        : Active the remote file CFG
*
*             Active the block of CFG
*
*             APPFLOW_CFG_GENERAL_ACTIVE           : Default general (debug trace, memory check, etc..
*             APPFLOW_CFG_INTERNETSERVICES_ACTIVE  : Inter
*             APPFLOW_CFG_DIOLOCATION_ACTIVE       : Location info (street, postal code, etc..)
*             APPFLOW_CFG_APPUPDATE_ACTIVE         : Application update
*             APPFLOW_CFG_ALERTS_ACTIVE            : Alerts
*             APPFLOW_CFG_LOG_ACTIVE               : Log
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTrace.h"
#include "XFileCFG.h"

#ifdef APPFLOW_CFG_REMOTEFILE_ACTIVE
#include "DIORemoteFileCFG.h"
#endif



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define APPFLOW_CFG_DEFAULT_REMARK_COLUMN                                             98


#ifdef APPFLOW_CFG_GENERAL_ACTIVE

#define APPFLOW_CFG_SECTION_GENERAL                                                   _L("general")
#define APPFLOW_CFG_SCRAPERWEBSCRIPTURLDOWNLOAD                                       _L("scraperwebscripturldownload")
#define APPFLOW_CFG_SHOWDETAILINFO                                                    _L("showdetailinfo")
#ifdef  XTRACE_ACTIVE
#define APPFLOW_CFG_TRACETARGET                                                       _L("trace_target")
#endif

#endif


#ifdef APPFLOW_CFG_CHECKRESOURCESHARDWARE_ACTIVE

#define APPFLOW_CFG_SECTION_CHECKRESOURCESHARDWARE                                    _L("check resources hardware")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_MEMSTATUSCHECKCADENCE                      _L("memstatuscheckcadenceseconds")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_MEMSTATUSLIMITPERCENT                      _L("memstatuslimitpercent")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_TOTALCPUUSAGECHECKCADENCE                  _L("totalcpuusagecheckcadenceseconds")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_TOTALCPUUSAGELIMITPERCENT                  _L("totalcpuusagelimitpercent")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_APPCPUUSAGEPROCESSNAME                     _L("appcpuusageprocessname")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_APPCPUUSAGECHECKCADENCE                    _L("appcpuusagecheckcadenceseconds")
#define APPFLOW_CFG_CHECKRESOURCESHARDWARE_APPCPUUSAGELIMITPERCENT                    _L("appcpuusagelimitpercent")

#endif


#ifdef APPFLOW_CFG_INTERNETSERVICES_ACTIVE

#define APPFLOW_CFG_SECTION_INTERNETSERVICES                                          _L("internet services")
#define APPFLOW_CFG_INTERNETSERVICES_CHECKINTERNETSTATUSCADENCE                       _L("checkinternetstatuscadenceseconds")
#define APPFLOW_CFG_INTERNETSERVICES_DONOTLETINTERNETCONNECTIONMATTER                 _L("donotletinternetconnectionmatter")  
#define APPFLOW_CFG_INTERNETSERVICES_CHECKIPSCHANGECADENCE                            _L("checkipschangecadenceseconds")
#define APPFLOW_CFG_INTERNETSERVICES_UPDATETIMEBYNTPCADENCE                           _L("updatetimebyntpcadencehours")
#define APPFLOW_CFG_INTERNETSERVICES_UPDATETIMENTPSERVER                              _L("updatetimentpserver")
#define APPFLOW_CFG_INTERNETSERVICES_UPDATETIMENTPMERIDIANDIFFERENCE                  _L("updatetimentpmeridiandifference")
#define APPFLOW_CFG_INTERNETSERVICES_UPDATETIMENTPUSEDAYLIGHTSAVING                   _L("updatetimentpusedaylightsaving")

#define APPFLOW_CFG_INTERNETSERVICES_UPDATETIMENTPMERIDIANDIFFERENCE_AUTO             25


#ifdef APPFLOW_CFG_DNSRESOLVER_ACTIVE

#define APPFLOW_CFG_SECTION_DNSRESOLVER                                               _L("dns resolved")
#define APPFLOW_CFG_DNSRESOLVER_HOSTRESOLVED                                          _L("host_resolved")
#define APPFLOW_CFG_DNSRESOLVER_DNSSERVER                                             _L("dns_server")

#endif


#ifdef APPFLOW_CFG_DYNDNSMANAGER_ACTIVE

#define APPFLOW_CFG_SECTION_DYNDNSMANAGER                                             _L("dyndns manager")
#define APPFLOW_CFG_DYNDNSMANAGER_URL                                                 _L("url")

#endif

#endif


#ifdef APPFLOW_CFG_DIOLOCATION_ACTIVE

#define APPFLOW_CFG_SECTION_LOCATION                                                  _L("location")
#define APPFLOW_CFG_CENTERNAME                                                        _L("name")
#define APPFLOW_CFG_LOCATION_STREET                                                   _L("street")
#define APPFLOW_CFG_LOCATION_TOWN                                                     _L("city")
#define APPFLOW_CFG_LOCATION_STATE                                                    _L("state")
#define APPFLOW_CFG_LOCATION_COUNTRY                                                  _L("country")
#define APPFLOW_CFG_LOCATION_POSTALCODE                                               _L("postalcode")

#endif


#ifdef APPFLOW_CFG_APPUPDATE_ACTIVE

#define APPFLOW_CFG_SECTION_APPLICATIONUPDATE                                         _L("applicationupdate")
#define APPFLOW_CFG_APPLICATIONUPDATE_ISACTIVE                                        _L("isactive")
#define APPFLOW_CFG_APPLICATIONUPDATE_URL                                             _L("url")
#define APPFLOW_CFG_APPLICATIONUPDATE_PORT                                            _L("port")
#define APPFLOW_CFG_APPLICATIONUPDATE_CHECKCADENCE                                    _L("checkcadenceminutes")
#define APPFLOW_CFG_APPLICATIONUPDATE_CHECKTIME                                       _L("checktime")
#define APPFLOW_CFG_APPLICATIONUPDATE_MAXRESTORATIONS                                 _L("maxrestorations")

#endif


#ifdef APPFLOW_CFG_WEBSERVER_ACTIVE

#define APPFLOW_CFG_SECTION_WEBSERVER                                                 _L("webserver")
#define APPFLOW_CFG_WEBSERVER_LOCALADDR                                               _L("localaddr")
#define APPFLOW_CFG_WEBSERVER_PORT                                                    _L("port")
#define APPFLOW_CFG_WEBSERVER_TIMEOUTTOSERVERPAGE                                     _L("timeouttoserverpage")
#define APPFLOW_CFG_WEBSERVER_AUTHENTICATEDACCESS                                     _L("isauthenticatedaccess")
#define APPFLOW_CFG_WEBSERVER_LOGIN                                                   _L("login")
#define APPFLOW_CFG_WEBSERVER_PASSWORD                                                _L("password")
#define APPFLOW_CFG_WEBSERVER_PATH_RESOURCES                                          _L("path_resources")
#define APPFLOW_CFG_WEBSERVER_PATH_PHP                                                _L("path_php")

#ifdef DIO_STREAMTLS_ACTIVE

#define APPFLOW_CFG_WEBSERVER_PATH_PRIVATEKEY                                         _L("path_privatekey")
#define APPFLOW_CFG_WEBSERVER_PATH_CERTIFICATE                                        _L("path_certificate")
#define APPFLOW_CFG_WEBSERVER_PRIVATEKEY_PASSWORD                                      _L("privatekey_password")
#endif

#endif


#ifdef APPFLOW_CFG_ALERTS_ACTIVE

#define APPFLOW_CFG_SECTION_ALERTS                                                    _L("alerts")
#define APPFLOW_CFG_ALERTS_ISACTIVE                                                   _L("isactive")
#define APPFLOW_CFG_ALERTS_CONDITION                                                  _L("condition")
#define APPFLOW_CFG_ALERTS_SMTP_ISACTIVE                                              _L("smtp_isactive")
#define APPFLOW_CFG_ALERTS_SMTP_URL                                                   _L("smtp_url")
#define APPFLOW_CFG_ALERTS_SMTP_PORT                                                  _L("smtp_port")
#define APPFLOW_CFG_ALERTS_SMTP_LOGIN                                                 _L("smtp_login")
#define APPFLOW_CFG_ALERTS_SMTP_PASSWORD                                              _L("smtp_password")
#define APPFLOW_CFG_ALERTS_SMTP_SENDER                                                _L("smtp_sender")
#define APPFLOW_CFG_ALERTS_SMTP_RECIPIENT                                             _L("smtp_recipient")

#define APPFLOW_CFG_ALERTS_SMS_ISACTIVE                                               _L("sms_isactive")
#define APPFLOW_CFG_ALERTS_SMS_RECIPIENT                                              _L("sms_recipient")

#define APPFLOW_CFG_ALERTS_WEB_ISACTIVE                                               _L("web_isactive")
#define APPFLOW_CFG_ALERTS_WEB_UISUSEGET                                              _L("web_isuseget")
#define APPFLOW_CFG_ALERTS_WEB_RECIPIENT                                              _L("web_recipient")

#define APPFLOW_CFG_ALERTS_UDP_ISACTIVE                                               _L("udp_isactive")
#define APPFLOW_CFG_ALERTS_UDP_PORT                                                   _L("udp_port")
#define APPFLOW_CFG_ALERTS_UDP_RECIPIENT                                              _L("udp_recipient")

#endif


#ifdef APPFLOW_CFG_SCRIPTS_ACTIVE

#define APPFLOW_CFG_SECTION_SCRIPTS                                                   _L("scriptslist")
#define APPFLOW_CFG_SCRIPTS_SCRIPT                                                    _L("scripts")  

#endif


#ifdef APPFLOW_CFG_LOG_ACTIVE

#define APPFLOW_CFG_SECTION_LOG                                                       _L("log")
#define APPFLOW_CFG_LOG_ISACTIVE                                                      _L("isactive")
#define APPFLOW_CFG_LOG_BACKUPISACTIVE                                                _L("backupisactive")
#define APPFLOW_CFG_LOG_BACKUPMAXFILES                                                _L("backupmaxfiles")
#define APPFLOW_CFG_LOG_BACKUPISCOMPRESS                                              _L("backupiscompress")
#define APPFLOW_CFG_LOG_ACTIVESECTIONSID                                              _L("activesectionsID")
#define APPFLOW_CFG_LOG_LEVELMASK                                                     _L("levelmask")
#define APPFLOW_CFG_LOG_MAXSIZE                                                       _L("maxsize")
#define APPFLOW_CFG_LOG_REDUCTIONPERCENT                                              _L("reductionpercent")

// ID Sections "generic" of LOG
#define APPFLOW_CFG_LOG_SECTIONID_INITIATION                                          _L("Ini")
#define APPFLOW_CFG_LOG_SECTIONID_GENERIC                                             _L("General")
#define APPFLOW_CFG_LOG_SECTIONID_CONNEXIONS                                          _L("Connexions")
#define APPFLOW_CFG_LOG_SECTIONID_STATUSAPP                                           _L("Status")
#define APPFLOW_CFG_LOG_SECTIONID_ENDING                                              _L("End")

#endif

#ifdef XTRACE_ACTIVE
  #define   APPFLOW_CFG_SETAUTOMATICTRACETARGETS                                      APPFLOW_CFG.SetAutomaticTraceTargets();
#else
  #define   APPFLOW_CFG_SETAUTOMATICTRACETARGETS             
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPATHS;
class XSYSTEM;
class XLOG;

class APPFLOWCFG
#ifdef APPFLOW_CFG_REMOTEFILE_ACTIVE
  : public DIOREMOTEFILECFG
#else
  : public XFILECFG
#endif
{
  public:
                         APPFLOWCFG                                               (XCHAR* namefile);
    virtual             ~APPFLOWCFG                                               ();

    virtual bool         DoVariableMapping                                        (); 
    virtual bool         DoDefault                                                ();

    virtual bool         End                                                      ();  

    #ifdef APPFLOW_CFG_GENERAL_ACTIVE
    XSTRING*             GetScraperWebScriptURLDownload                           ();
    XWORD                GetShowDetailInfo                                        ();
    void                 SetShowDetailInfo                                        (XWORD detail);
    #ifdef XTRACE_ACTIVE
    XSTRING*             GetTraceTarget                                           (int index);
    bool                 SetTraceTarget                                           (int index, XSTRING& target);
    bool                 SetAutomaticTraceTargets                                 ();
    bool                 SetDefaultTraceTargets                                   ();
    #endif
    #endif

    #ifdef APPFLOW_CFG_CHECKRESOURCESHARDWARE_ACTIVE
    int                  CheckResourcesHardware_GetMemStatusCheckCadence          ();
    int                  CheckResourcesHardware_GetMemStatusLimitPercent          ();

    int                  CheckResourcesHardware_GetTotalCPUUsageCheckCadence      ();
    int                  CheckResourcesHardware_GetTotalCPUUsageLimitPercent      ();
    XSTRING*             CheckResourcesHardware_GetAppCPUUsageProcessName         ();
    int                  CheckResourcesHardware_GetAppCPUUsageCheckCadence        ();
    int                  CheckResourcesHardware_GetAppCPUUsageLimitPercent        ();
    #endif

    #ifdef APPFLOW_CFG_INTERNETSERVICES_ACTIVE
    int                  InternetServices_GetCheckInternetStatusCadence           ();
    bool                 InternetServices_DoNotLetInternetConnectionMatter        ();
    int                  InternetServices_GetCheckIPsChangeCadence                ();
    int                  InternetServices_GetUpdateTimeByNTPCadence               ();
    XVECTOR<XSTRING*>*   InternetServices_GetUpdateTimeNTPServers                 (); 
    XSTRING*             InternetServices_GetUpdateTimeNTPServer                  (int index);
    int                  InternetServices_GetUpdateTimeNTPMeridianDifference      ();
    bool                 InternetServices_GetUpdateTimeNTPUseDayLightSaving       ();
    #ifdef APPFLOW_CFG_DNSRESOLVER_ACTIVE
    bool                 DNSResolver_GetHost                                      (int index, XSTRING& host, XSTRING& IPresolved);
    XSTRING*             DNSResolver_GetDNSserver                                 (int index);
    bool                 SetAutomaticDNSResolver                                  ();
    #endif
    #ifdef APPFLOW_CFG_DYNDNSMANAGER_ACTIVE
    XVECTOR<XSTRING*>*   DNSManager_GetURLs                                       ();      
    XSTRING*             DNSManager_GetURL                                        (int index);
    #endif
    #endif

    #ifdef APPFLOW_CFG_DIOLOCATION_ACTIVE
    XSTRING*             Location_GetStreet                                       ();
    XSTRING*             Location_GetCity                                         ();
    XSTRING*             Location_GetState                                        ();
    XSTRING*             Location_GetCountry                                      ();
    int                  Location_GetPostalCode                                   ();
    #endif

    #ifdef APPFLOW_CFG_APPUPDATE_ACTIVE
    bool                 ApplicationUpdate_IsActive                               ();
    int                  ApplicationUpdate_GetPort                                ();
    XSTRING*             ApplicationUpdate_GetURL                                 ();
    int                  ApplicationUpdate_GetCheckCadence                        ();
    XSTRING*             ApplicationUpdate_GetCheckTime                           ();
    int                  ApplicationUpdate_GetMaxRestorations                     ();
    #endif

    #ifdef APPFLOW_CFG_ALERTS_ACTIVE
    bool                 Alerts_IsActive                                          ();
    XVECTOR<XSTRING*>*   Alerts_GetConditions                                     ();
    XSTRING*             Alerts_GetCondition                                      (int index);
    bool                 Alerts_GetCondition                                      (int index, XDWORD& conditionID, int& timelimitforrepeat, int& numberoftimesrepeat);
    bool                 Alerts_IsActiveSMTP                                      ();
    XSTRING*             Alerts_GetSMTPURL                                        ();
    int                  Alerts_GetSMTPPort                                       ();
    XSTRING*             Alerts_GetSMTPLogin                                      ();
    XSTRING*             Alerts_GetSMTPPassword                                   ();
    XSTRING*             Alerts_GetSMTPSender                                     ();
    XVECTOR<XSTRING*>*   Alerts_GetSMTPRecipients                                 ();
    XSTRING*             Alerts_GetSMTPRecipient                                  (int index);
    bool                 Alerts_IsActiveSMS                                       ();
    XVECTOR<XSTRING*>*   Alerts_GetSMSRecipients                                  ();
    XSTRING*             Alerts_GetSMSRecipient                                   (int index);
    bool                 Alerts_IsActiveWEB                                       ();
    bool                 Alerts_GetWEBIsUseGet                                    ();
    XVECTOR<XSTRING*>*   Alerts_GetWEBRecipients                                  ();
    XSTRING*             Alerts_GetWEBRecipient                                   (int index);
    bool                 Alerts_IsActiveUDP                                       ();
    int                  Alerts_GetUDPPort                                        ();
    XVECTOR<XSTRING*>*   Alerts_GetUDPRecipients                                  ();
    XSTRING*             Alerts_GetUDPRecipient                                   (int index);
    #endif

    #ifdef APPFLOW_CFG_WEBSERVER_ACTIVE
    XSTRING*             WebServer_GetLocalAddress                                ();
    int                  WebServer_GetPort                                        ();
    int                  WebServer_GetTimeoutToServerPage                         ();
    bool                 WebServer_IsAuthenticatedAccess                          ();
    XSTRING*             WebServer_GetLogin                                       ();
    XSTRING*             WebServer_GetPassword                                    ();
    XPATH*               WebServer_PathResources                                  ();
    XPATH*               WebServer_PathPHP                                        ();
    #ifdef DIO_STREAMTLS_ACTIVE
    
    bool                 WebServer_IsTLS                                          ();
    XPATH*               WebServer_PathPrivateKey                                 ();
    XPATH*               WebServer_PathCertificate                                ();
    XSTRING*             WebServer_GetPrivateKeyPassword                          ();
    #endif
    #endif

    #ifdef APPFLOW_CFG_SCRIPTS_ACTIVE
    XVECTOR<XSTRING*>*   Scripts_GetAll                                           ();
    XSTRING*             Scripts_GetScript                                        (int index);
    #endif

    #ifdef APPFLOW_CFG_LOG_ACTIVE
    bool                 Log_IsActive                                             ();
    bool                 Log_Backup_IsActive                                      ();
    int                  Log_Backup_GetMaxFiles                                   ();
    bool                 Log_Backup_IsCompress                                    ();
    XSTRING*             Log_ActiveSectionsID                                     ();
    XBYTE                Log_LevelMask                                            ();
    int                  Log_MaxSize                                              ();
    int                  Log_ReductionPercent                                     ();
    #endif

  protected:

    #ifdef APPFLOW_CFG_GENERAL_ACTIVE
    XSTRING              scraperwebscripturldownload;
    XSTRING              showdetailinfo;
    #ifdef XTRACE_ACTIVE
    int                  ntracetargets;
    XVECTOR<XSTRING*>    tracetargets;
    #endif
    #endif

    #ifdef APPFLOW_CFG_CHECKRESOURCESHARDWARE_ACTIVE
    int                  checkresourceshardware_memstatuscheckcadence;
    int                  checkresourceshardware_memstatuslimitpercent;
    int                  checkresourceshardware_totalcpuusagecheckcadence;
    int                  checkresourceshardware_totalcpuusagelimitpercent;
    XSTRING              checkresourceshardware_appcpuusageprocessname; 
    int                  checkresourceshardware_appcpuusagecheckcadence;
    int                  checkresourceshardware_appcpuusagelimitpercent;
    #endif

    #ifdef APPFLOW_CFG_INTERNETSERVICES_ACTIVE
    int                  internetservices_checkinternetstatuscadence;
    bool                 internetservices_donotletinternetconnectionmatter;
    int                  internetservices_checkipschangecadence;
    int                  internetservices_updatetimebyntpcadence;
    int                  internetservices_nupdatetimentpservers;
    XVECTOR<XSTRING*>    internetservices_updatetimentpservers;    
    int                  internetservices_updatetimentpmeridiandifference;
    bool                 internetservices_updatetimentpusedaylightsaving;
    #ifdef APPFLOW_CFG_DNSRESOLVER_ACTIVE
    int                  nhostsresolved; 
    XVECTOR<XSTRING*>    hostsresolved;
    int                  nDNSservers;
    XVECTOR<XSTRING*>    DNSservers;
    #endif
    #ifdef APPFLOW_CFG_DYNDNSMANAGER_ACTIVE
    int                  dnsmanager_nurls;
    XVECTOR<XSTRING*>    dnsmanager_urls;      
    #endif
    #endif

    #ifdef APPFLOW_CFG_DIOLOCATION_ACTIVE
    XSTRING              location_street;
    XSTRING              location_city;
    XSTRING              location_state;
    XSTRING              location_country;
    int                  location_postalcode;
    #endif

    #ifdef APPFLOW_CFG_APPUPDATE_ACTIVE
    bool                 applicationupdate_isactive;
    XSTRING              applicationupdate_URL;
    int                  applicationupdate_port;
    int                  applicationupdate_checkcadence;
    XSTRING              applicationupdate_checktime;
    int                  applicationupdate_maxrestorations;
    #endif

    #ifdef APPFLOW_CFG_WEBSERVER_ACTIVE
    XSTRING              webserver_localaddr;
    int                  webserver_port;
    int                  webserver_timeouttoserverpage;
    bool                 webserver_isauthenticatedaccess;
    XSTRING              webserver_login;
    XSTRING              webserver_password;
    XPATH                webserver_path_resources;
    XPATH                webserver_path_PHP;
    #ifdef DIO_STREAMTLS_ACTIVE
    XPATH                webserver_path_privatekey;
    XPATH                webserver_path_certificate;
    XSTRING              webserver_privatekey_password;
    #endif
    #endif

    #ifdef APPFLOW_CFG_ALERTS_ACTIVE
    bool                 alerts_isactive;
    int                  alerts_nconditions;
    XVECTOR<XSTRING*>    alerts_conditions;
    bool                 alerts_SMTP_isactive;
    XSTRING              alerts_SMTP_URL;
    int                  alerts_SMTP_port;
    XSTRING              alerts_SMTP_login;
    XSTRING              alerts_SMTP_password;
    XSTRING              alerts_SMTP_sender;
    int                  alerts_SMTP_nrecipients;
    XVECTOR<XSTRING*>    alerts_SMTP_recipients;
    bool                 alerts_SMS_isactive;
    int                  alerts_SMS_nrecipients;
    XVECTOR<XSTRING*>    alerts_SMS_recipients;
    bool                 alerts_WEB_isactive;
    bool                 alerts_WEB_isuseget;
    int                  alerts_WEB_nrecipients;
    XVECTOR<XSTRING*>    alerts_WEB_recipients;
    bool                 alerts_UDP_isactive;
    int                  alerts_UDP_port;
    int                  alerts_UDP_nrecipients;
    XVECTOR<XSTRING*>    alerts_UDP_recipients;
    #endif


    #ifdef APPFLOW_CFG_SCRIPTS_ACTIVE
    int                  scripts_nscripts;
    XVECTOR<XSTRING*>    scripts_list;
    #endif


    #ifdef APPFLOW_CFG_LOG_ACTIVE
    bool                 log_isactive;
    bool                 log_backupisactive;
    int                  log_backupmaxfiles;
    bool                 log_backupiscompress;
    XSTRING              log_activesectionsID;
    int                  log_levelmask;
    int                  log_maxsize;
    int                  log_reductionpercent;
    #endif

  private:

    void                 Clean                                                    ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





