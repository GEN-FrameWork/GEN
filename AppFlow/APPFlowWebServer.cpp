/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowWebServer.cpp
* 
* @class      APPFLOWWEBSERVER
* @brief      Application Flow Web Server class
* @ingroup    APPFLOW
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

#include "APPFlowWebServer.h"

#include "XFile.h"
#include "XPath.h"
#include "XLog.h"
#include "XFactory.h"
#include "XBuffer.h"

#include "HashMD5.h"

#include "DIOStreamTCPIP.h"
#include "DIOStreamTLSConfig.h"
#include "DIOWebServer_Plugin_PHP.h"
#include "DIOWebServer_XEvent.h"

#include "APPFlowCFG.h"
#include "APPFlowLog.h"

#include "XTranslation_GEN.h"
#include "XTranslation.h"

#ifdef DIO_STREAMTLS_ACTIVE
#include "CipherCredentialsLoader.h"
#include "CipherCredentialsProvider.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWWEBSERVER::APPFLOWWEBSERVER()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWWEBSERVER::APPFLOWWEBSERVER()
{
  Clean();

  webserver = GEN_NEW DIOWEBSERVER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWWEBSERVER::~APPFLOWWEBSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWWEBSERVER::~APPFLOWWEBSERVER()
{
  End();

  #ifdef DIO_STREAMTLS_ACTIVE
  if(tlscredentialsprovider && tlscredentialsproviderowner)
    {
      GEN_DELETE tlscredentialsprovider;
    }

  tlscredentialsprovider      = NULL;
  tlscredentialsproviderowner = false;

  if(tlssecretprovider && tlssecretproviderowner)
    {
      GEN_DELETE tlssecretprovider;
    }

  tlssecretprovider      = NULL;
  tlssecretproviderowner = false;
  #endif

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::Ini(APPFLOWCFG* cfg, bool doinitialconnectitivitytest, bool isapirestonly, bool chekuseragentid)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* @param[in]  doinitialconnectitivitytest : Doinitialconnectitivitytest value.
* @param[in]  isapirestonly : Isapirestonly value.
* @param[in]  chekuseragentid : Chekuseragentid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini(APPFLOWCFG* cfg, bool doinitialconnectitivitytest, bool isapirestonly, bool chekuseragentid)
{
  if(!cfg) return false;

  this->cfg = cfg;

  if(!webserver)  return false;

  //-------------------------------------------------------------------------------------------------------------------------------
  // The configuration of the object was never transferred to the members nor to the DIOWEBSERVER, so the authenticated
  // access of the .INI file was simply ignored.

  this->isapirestonly         = isapirestonly;
  this->isauthenticatedaccess = cfg->WebServer_IsAuthenticatedAccess();

  if(!Ini_Authentication(cfg)) return false;

  //-------------------------------------------------------------------------------------------------------------------------------

  /*
  if(chekuseragentid)
    {
      useragentID = GEN_NEW DIOSCRAPERWEBUSERAGENTID();
      if(!useragentID) return false;
    }
  */


  if(!cfg->WebServer_PathPHP()->IsEmpty())
    {
      pluginPHP = GEN_NEW DIOWEBSERVER_PLUGIN_PHP();
      if(pluginPHP)
        {
          pluginPHP->Config(cfg->WebServer_PathPHP());

          if(!webserver->GetPluginsManager()->Plugins_Add(pluginPHP)) return false;
        }
    }

  //-------------------------------------------------------------------------------------------------------------------------------
  // TLS (HTTPS): there is no separate "istls" setting. TLS is enabled when a credentials provider has been
  // injected or, for the default file-backed provider, when path_privatekey/path_certificate are configured.
  // The port, local address and timeout are shared with the plain-HTTP path below -- this is the same single
  // listener, just wrapped in TLS instead of speaking plain HTTP, not a second independent one.

  #ifdef DIO_STREAMTLS_ACTIVE
  if(tlscredentialsprovider || cfg->WebServer_IsTLS())
    {
      DIOSTREAMTLSCONFIG* tlsconfig = Ini_BuildTLSConfig(cfg);
      if(!tlsconfig)
        {
          APPFLOW_LOG_ENTRY(XLOGLEVEL_ERROR, DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED));

          return false;
        }

      // Ini(DIOSTREAMTLSCONFIG*, ...) below takes ownership of tlsconfig on both success and failure from this point on.
      return Ini(tlsconfig, cfg->WebServer_GetPort(), cfg->WebServer_GetTimeoutToServerPage(), cfg->WebServer_GetLocalAddress());
    }
  #endif

  return Ini(cfg->WebServer_GetPort(), doinitialconnectitivitytest, cfg->WebServer_GetTimeoutToServerPage(), cfg->WebServer_GetLocalAddress());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPFLOWWEBSERVER::Ini(APPFLOWCFG* cfg, XDWORD port, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Initialize a secondary listener (e.g. WebSocket) on its own port, following the same
*             TLS provider / cfg->WebServer_IsTLS() decision and credentials as the main Ini(APPFLOWCFG*, ...) listener.
* @ingroup    APPFLOW
*
* @param[in]  cfg : Configuration object to use (WebServer_IsTLS() / credential paths are consulted when no
*             TLS credentials provider is injected; WebServer_GetLocalAddress() is also used -- port and
*             timeoutserverpage are taken from the parameters below, not from cfg, since this listener runs on
*             a different port than the one configured in cfg->WebServer_GetPort()).
* @param[in]  port : Port number to use.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini(APPFLOWCFG* cfg, XDWORD port, int timeoutserverpage, XSTRING* addrlocal)
{
  if(!cfg) return false;

  this->cfg = cfg;

  if(!webserver)  return false;

  #ifdef DIO_STREAMTLS_ACTIVE
  if(tlscredentialsprovider || cfg->WebServer_IsTLS())
    {
      DIOSTREAMTLSCONFIG* tlsconfig = Ini_BuildTLSConfig(cfg);
      if(!tlsconfig)
        {
          APPFLOW_LOG_ENTRY(XLOGLEVEL_ERROR, DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT), (int)port);

          return false;
        }

      // Ini(DIOSTREAMTLSCONFIG*, ...) below takes ownership of tlsconfig on both success and failure from this point on.
      return Ini(tlsconfig, port, timeoutserverpage, addrlocal);
    }
  #endif

  return Ini(port, true, timeoutserverpage, addrlocal);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::Ini_Authentication(APPFLOWCFG* cfg)
* @brief      Transfer the authentication configuration (APPFLOWCFG) to the DIOWEBSERVER.
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini_Authentication(APPFLOWCFG* cfg)
{
  if(!cfg)        return false;
  if(!webserver)  return false;

  webserver->DelAllAuthentication();

  if(!cfg->WebServer_IsAuthenticatedAccess()) return true;

  XSTRING* login    = cfg->WebServer_GetLogin();
  XSTRING* password = cfg->WebServer_GetPassword();

  if(!login)    return false;
  if(!password) return false;

  if(password->IsEmpty())
    {
      // Authenticated access is active but there is no password in the configuration:
      // the authentication is resolved by events (DOAUTHENTICATE / CHECKAUTHENTICATE).
      APPFLOW_LOG_ENTRY(XLOGLEVEL_WARNING, DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD));

      return true;
    }

  XSTRING guest;

  // The wildcard guest is used to accept the credentials for any Host of the request.
  guest = DIOWEBSERVER_AUTHENTICATION_ALLGUESTS;

  if(!webserver->AddAuthentication(guest, (*login), (*password))) return false;

  // If a local address is configured, the same credentials are also registered for that specific guest.
  XSTRING* localaddress = cfg->WebServer_GetLocalAddress();
  if(localaddress)
    {
      if(!localaddress->IsEmpty())
        {
          if(!webserver->AddAuthentication((*localaddress), (*login), (*password))) return false;
        }
    }

  APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED), login->IsEmpty()?_L("<any>"):login->Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::Ini(XDWORD port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  port : Port number to use.
* @param[in]  doinitialconnectitivitytest : Doinitialconnectitivitytest value.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini(XDWORD port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
{
  if(!webserver)  return false;

  bool status = webserver->Ini(port, doinitialconnectitivitytest, timeoutserverpage, addrlocal);
  if(status) Ini_RegisterEvents();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPFLOWWEBSERVER::Ini(DIOSTREAMTLSCONFIG* tlsconfig, XDWORD port, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Initialize the object as an HTTPS listener
* @note       See DIOWEBSERVER::Ini(DIOSTREAMTLSCONFIG*, ...) for the ownership and credential requirements on
*             tlsconfig (ownership transfers to the internal DIOWEBSERVER instance).
* @ingroup    APPFLOW
*
* @param[in]  tlsconfig : TLS server configuration, already carrying local certificate/private key and offered
*             cipher suites / groups / signature schemes / ALPN protocols.
* @param[in]  port : Port number to use.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini(DIOSTREAMTLSCONFIG* tlsconfig, XDWORD port, int timeoutserverpage, XSTRING* addrlocal)
{
  if(!webserver)  return false;

  bool status = webserver->Ini(tlsconfig, (int)port, timeoutserverpage, addrlocal);
  if(status) Ini_RegisterEvents();

  return status;
}


#ifdef DIO_STREAMTLS_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPFLOWWEBSERVER::Ini_ResolveCertificatePath(XPATH* cfgpath, XPATH& resolvedpath)
* @brief      Resolve a WebServer_PathPrivateKey()/WebServer_PathCertificate() configuration field to the actual
*             file to open (see the declaration in APPFlowWebServer.h for the full explanation).
* @note       INTERNAL
* @ingroup    APPFLOW
*
* @param[in]  cfgpath : Configuration field to resolve (as returned by WebServer_PathPrivateKey() / WebServer_PathCertificate()).
* @param[out] resolvedpath : Output resolved path.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini_ResolveCertificatePath(XPATH* cfgpath, XPATH& resolvedpath)
{
  if(!cfgpath || cfgpath->IsEmpty()) return false;

  XSTRING namefileext;
  if(!cfgpath->GetNamefileExt(namefileext) || namefileext.IsEmpty()) return false;

  XSTRING directorypart;
  if(cfgpath->GetPath(directorypart) && !directorypart.IsEmpty())
    {
      // cfgpath already carries a directory part of its own (relative or absolute): used as-is, same
      // behaviour as before this change.
      resolvedpath = cfgpath->Get();
    }
   else
    {
      // cfgpath is just a file name: resolve it against the certificates section of the XPATHSMANAGER.
      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_CERTIFICATES, resolvedpath);
      resolvedpath.Slash_Add();
      resolvedpath += namefileext;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPFLOWWEBSERVER::Ini_ReadFile(XPATH& path, XBUFFER& filedata)
* @brief      Read the whole file at path into filedata (raw bytes) -- used to sniff the format
*             (CIPHERPEMCODEC::IsPEM()) before deciding how to parse a credential file.
* @note       INTERNAL
* @ingroup    APPFLOW
*
* @param[in]  path : Path of the file to read.
* @param[out] filedata : Output raw file bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini_ReadFile(XPATH& path, XBUFFER& filedata)
{
  bool status = false;

  XFILE* file = GEN_XFACTORY.Create_File();
  if(file)
    {
      if(file->Open(path, true))
        {
          XDWORD size = (XDWORD)file->GetSize();

          if(size)
            {
              if(filedata.Resize(size))
                {
                  status = file->Read(filedata.Get(), size);
                }
            }

          file->Close();
        }

      GEN_XFACTORY.Delete_File(file);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPFLOWWEBSERVER::Ini_LoadTLSCredentials(APPFLOWCFG* cfg, DIOSTREAMTLSCONFIG* tlsconfig)
* @brief      Load TLS credentials through the configured provider or the default file-backed provider.
* @ingroup    APPFLOW
*
* @param[in]  cfg : Configuration object to use.
* @param[in]  tlsconfig : TLS configuration that receives the local credentials.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::Ini_LoadTLSCredentials(APPFLOWCFG* cfg, DIOSTREAMTLSCONFIG* tlsconfig)
{
  if(!cfg || !tlsconfig) return false;

  CIPHERCREDENTIALSPROVIDER* provider = tlscredentialsprovider;

  XPATH pathkey;
  XPATH pathcert;
  XBUFFER keyfile;
  XBUFFER certificatefile;
  CIPHERCREDENTIALSPROVIDERBUFFER bufferprovider;
  CIPHERSECRETPROVIDERSTRING secretprovider;

  if(!provider)
    {
      if(!Ini_ResolveCertificatePath(cfg->WebServer_PathPrivateKey(), pathkey) ||
         !Ini_ResolveCertificatePath(cfg->WebServer_PathCertificate(), pathcert) ||
         !Ini_ReadFile(pathkey, keyfile) || !Ini_ReadFile(pathcert, certificatefile))
        {
          keyfile.SecureDelete();
          certificatefile.Delete();
          return false;
        }

      CIPHERSECRETPROVIDER* selectedsecretprovider = tlssecretprovider;

      if(!selectedsecretprovider)
        {
          secretprovider.SetSource(cfg->WebServer_GetPrivateKeyPassword());
          selectedsecretprovider = &secretprovider;
        }

      bufferprovider.SetCertificateData(&certificatefile);
      bufferprovider.SetPrivateKeyData(&keyfile);
      bufferprovider.SetSecretProvider(selectedsecretprovider);

      provider = &bufferprovider;
    }

  XVECTOR<XBUFFER*> certificatechain;
  CIPHERKEY* privatekey = NULL;
  bool status = provider->Credentials_Load(certificatechain, privatekey);

  keyfile.SecureDelete();
  certificatefile.Delete();

  if(status) status = tlsconfig->SetLocalPrivateKey(privatekey);

  for(XDWORD c=0; status && c<certificatechain.GetSize(); c++)
    {
      XBUFFER* certificate = certificatechain.Get(c);
      status = certificate && tlsconfig->LocalCertificate_Add((*certificate));
    }

  CIPHERCREDENTIALSLOADER::Certificates_Delete(certificatechain);
  CIPHERCREDENTIALSLOADER::PrivateKey_Delete(privatekey);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSCONFIG* APPFLOWWEBSERVER::Ini_BuildTLSConfig(APPFLOWCFG* cfg)
* @brief      Build a DIOSTREAMTLSCONFIG (cipher suite / group / signature scheme / ALPN + credentials) from cfg.
* @note       INTERNAL. Ownership of the returned object belongs to the caller. Returns NULL if cfg is NULL or
*             the private key / certificate could not be loaded (see Ini_LoadTLSCredentials()).
* @ingroup    APPFLOW
*
* @param[in]  cfg : Configuration object to use.
*
* @return     DIOSTREAMTLSCONFIG* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSCONFIG* APPFLOWWEBSERVER::Ini_BuildTLSConfig(APPFLOWCFG* cfg)
{
  if(!cfg) return NULL;

  DIOSTREAMTLSCONFIG* tlsconfig = GEN_NEW DIOSTREAMTLSCONFIG();
  if(!tlsconfig) return NULL;

  tlsconfig->SetMode(DIOSTREAMMODE_SERVER);

  // Register every RFC 8446 AEAD suite (AES-128-GCM, ChaCha20-Poly1305, AES-256-GCM) rather than a single one,
  // so interoperability is not needlessly limited to peers that happen to prefer the first suite offered.
  tlsconfig->CipherSuites_AddRecommendedProfile();

  tlsconfig->SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768);
  tlsconfig->SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_X25519);

  // All signature schemes actually implemented are offered unconditionally: SignatureScheme_Select() (see
  // DIOStreamTLS13HandshakeServer.cpp) already filters them through DIOSTREAMTLSSIGNATURE::IsSupported(),
  // which safely narrows the offer down to whichever one matches the local certificate's actual key type.
  tlsconfig->SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256);
  tlsconfig->SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519);
  tlsconfig->SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256);
  tlsconfig->SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384);
  tlsconfig->SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512);

  tlsconfig->ApplicationProtocol_Add(DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1);

  if(!Ini_LoadTLSCredentials(cfg, tlsconfig))
    {
      GEN_DELETE tlsconfig;
      return NULL;
    }

  return tlsconfig;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void APPFLOWWEBSERVER::Ini_RegisterEvents()
* @brief      Register/subscribe to the DIOWEBSERVER events common to every Ini() entry point.
* @note       INTERNAL
* @ingroup    APPFLOW
*
* @return     void : Does not return a value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWWEBSERVER::Ini_RegisterEvents()
{
  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST, (XSUBJECT *)webserver);

  if(isauthenticatedaccess)
    {
      RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_DOAUTHENTICATE);
      RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_CHECKAUTHENTICATE);
    }

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT, (XSUBJECT *)webserver);

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED, (XSUBJECT *)webserver);

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA, (XSUBJECT *)webserver);

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG, (XSUBJECT *)webserver);

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED);
  SubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED , (XSUBJECT *)webserver);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOWEBSERVER* APPFLOWWEBSERVER::GetWebServer()
* @brief      Get web server
* @ingroup    APPFLOW
* 
* @return     DIOWEBSERVER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER* APPFLOWWEBSERVER::GetWebServer()
{
  return webserver;
}


#ifdef DIO_STREAMTLS_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::SetTLSCredentialsProvider(CIPHERCREDENTIALSPROVIDER* provider, bool owner)
* @brief      Set tls credentials provider
* @ingroup    APPFLOW
* 
* @param[in]  provider : Pointer to provider.
* @param[in]  owner : Owner value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::SetTLSCredentialsProvider(CIPHERCREDENTIALSPROVIDER* provider, bool owner)
{
  if(provider == tlscredentialsprovider)
    {
      tlscredentialsproviderowner = owner;
      return true;
    }

  if(tlscredentialsprovider && tlscredentialsproviderowner)
    {
      GEN_DELETE tlscredentialsprovider;
    }

  tlscredentialsprovider      = provider;
  tlscredentialsproviderowner = owner;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCREDENTIALSPROVIDER* APPFLOWWEBSERVER::GetTLSCredentialsProvider()
* @brief      Get tls credentials provider
* @ingroup    APPFLOW
* 
* @return     CIPHERCREDENTIALSPROVIDER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCREDENTIALSPROVIDER* APPFLOWWEBSERVER::GetTLSCredentialsProvider()
{
  return tlscredentialsprovider;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::SetTLSSecretProvider(CIPHERSECRETPROVIDER* provider, bool owner)
* @brief      Set tls secret provider
* @ingroup    APPFLOW
* 
* @param[in]  provider : Pointer to provider.
* @param[in]  owner : Owner value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::SetTLSSecretProvider(CIPHERSECRETPROVIDER* provider, bool owner)
{
  if(provider == tlssecretprovider)
    {
      tlssecretproviderowner = owner;
      return true;
    }

  if(tlssecretprovider && tlssecretproviderowner)
    {
      GEN_DELETE tlssecretprovider;
    }

  tlssecretprovider      = provider;
  tlssecretproviderowner = owner;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECRETPROVIDER* APPFLOWWEBSERVER::GetTLSSecretProvider()
* @brief      Get tls secret provider
* @ingroup    APPFLOW
* 
* @return     CIPHERSECRETPROVIDER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECRETPROVIDER* APPFLOWWEBSERVER::GetTLSSecretProvider()
{
  return tlssecretprovider;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GetIsAuthenticatedAccess()
* @brief      Get is authenticated access
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GetIsAuthenticatedAccess()
{
  return isauthenticatedaccess;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GetIsApiRestOnly()
* @brief      Get is api rest only
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GetIsApiRestOnly()
{
  return isapirestonly;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XBUFFER* data, int timeout, XSTRING* addhead)
* @brief      Send request
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  headerresult : Output headerresult.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  addhead : Addhead pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XBUFFER* data, int timeout, XSTRING* addhead)
{
  DIOWEBSERVER_HEADER webserverheader;
  XSTRING             entitytag;
  HASHMD5             hashmd5;
  bool                iscached = false;
  bool                status   = false;

  if(!connection)                                return false;
  if(!connection->GetDIOStream())                return false;
  if(!connection->GetDIOStream()->IsConnected()) return false;

  if(data)
    {
      hashmd5.Do((*data));
      hashmd5.GetResultString(entitytag);
      iscached = connection->GetRequest()->GetIfNoneMatch()->Compare(entitytag)?false:true;
    }

  if(iscached)
    {
      webserverheader.Create(connection->GetRequest()->GetResource(), 1, 1, DIOWEBHEADER_RESULT_NOTMODIFIED, connection->GetRequest()->GetSize());
      status = webserverheader.Write(connection->GetDIOStream(), 5);
    }
   else
    {
      XPATH resourceext;
      int   size = (data ?(int)data->GetSize():-1);

      resourceext = connection->GetRequest()->GetResource()->Get();
      resourceext.SetOnlyExt();

      webserverheader.Create(connection->GetRequest()->GetResource(), 1, 1, headerresult, size, &entitytag, connection->GetServer()->IsPlayablePage(resourceext));
      if(addhead) webserverheader.AddLine(addhead->Get());
      status = webserverheader.Write(connection->GetDIOStream(), 5);
      if(status)
        {
          if(size > 0) status = connection->Send((*data), timeout);
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XSTRING& data, int timeout, XSTRING* addhead)
* @brief      Send request
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  headerresult : Output headerresult.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  addhead : Addhead pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XSTRING& data, int timeout, XSTRING* addhead)
{
  XDWORD size = data.GetSize();
  if(!size) return false;

  XBUFFER xbuffer(size);
  bool    status = false;

  
  XBUFFER charstr;
  data.ConvertToASCII(charstr);
  memcpy(xbuffer.Get(), charstr.Get(), size);
  status = SendRequest(connection, headerresult, &xbuffer, timeout, addhead);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XPATH& xpathfile, int timeout, XSTRING* addhead)
* @brief      Send request
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  headerresult : Output headerresult.
* @param[in]  xpathfile : Xpathfile value.
* @param[in]  timeout : Timeout value.
* @param[in]  addhead : Addhead pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::SendRequest(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT headerresult, XPATH& xpathfile, int timeout, XSTRING* addhead)
{
  XBUFFER xbuffer;
  bool    status = false;

  XFILE* xfile=GEN_XFACTORY.Create_File();
  if(!xfile) return false;

  if(xfile->Open(xpathfile))
    {
      XQWORD size = xfile->GetSize();
      if(size)
        {
          xbuffer.Resize((XDWORD)size);
          status = xfile->Read(xbuffer.Get(), (XDWORD)size);
        }

      xfile->Close();
    }

  GEN_XFACTORY.Delete_File(xfile);

  if(status)
    {
      status = SendRequest(connection, headerresult, &xbuffer, timeout, addhead);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::ResolveRequest(DIOWEBSERVER* server, DIOWEBSERVER_CONNECTION* connection, DIOWEBSERVER_REQUEST* request)
* @brief      Resolve request
* @ingroup    APPFLOW
* 
* @param[in]  server : Server pointer to use.
* @param[in]  connection : Connection pointer to use.
* @param[in]  request : Request pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::ResolveRequest(DIOWEBSERVER* server, DIOWEBSERVER_CONNECTION* connection, DIOWEBSERVER_REQUEST* request)
{
  if(!server)     return false;
  if(!connection) return false;
  if(!request)    return false;

  XSTRING* resource = request->GetResource();
  if(!resource) return false;

  // NOTE: cfg may legitimately be NULL here. APPFLOWWEBSERVER::Ini(DIOSTREAMTLSCONFIG*, XDWORD, int, XSTRING*) --
  // the entry point used to initialize an HTTPS-only instance (no APPFLOWCFG involved) -- never assigns this->cfg,
  // unlike APPFLOWWEBSERVER::Ini(APPFLOWCFG*, ...) used for plain HTTP. Bailing out here unconditionally used to
  // make every HTTPS request silently return with no response at all (the handshake completed, but nothing was
  // ever written back). cfg is only actually dereferenced below to resolve the resources root path, and that
  // already has a fallback (GEN_XPATHSMANAGER) for when it is unavailable, so do not early-out on it: let that
  // fallback run instead of assuming cfg is always present.

  DIOWEBSERVER_HEADER         webserverheader;
  DIOURL                      resourceconv;
  int                         nparams               = 0;
  DIOWEBSERVER_QUERYSTRINGS   querystring;
  XSTRING                     extension;
  bool                        ispagepluging         = false;
  bool                        status                = false;

  resourceconv.Set(resource->Get());
  resourceconv.DecodeUnsafeChars();

  if(isauthenticatedaccess)
    {
      if(webserver->HaveAuthentications())
        {
          XSTRING login;
          XSTRING password;

          if(!request->GetLoginPassword(login, password))
            {
              // There are no credentials in the request: ask for them (401 + WWW-Authenticate).
              GenerateResponse_Unauthorized(connection);

              return true;
            }
           else
            {
              bool  ispasswordvalid = webserver->CheckAuthentication((*request->GetGuest()), login, password);

              page_login    = login;
              page_password = password;

              XSTRING IPstring;
              connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);

              APPFLOW_LOG_ENTRY((ispasswordvalid?XLOGLEVEL_INFO:XLOGLEVEL_WARNING), DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST), IPstring.Get(), login.Get(), (ispasswordvalid)? XT_L(XTRANSLATION_GEN_ID_OK): XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID) );

              if(!ispasswordvalid)
                {
                  page_login.Empty();
                  page_password.Empty();

                  // Answer again with 401 so the client can retry with valid credentials.
                  GenerateResponse_Unauthorized(connection);

                  return true;
                }
            }
        }
       else
        {
          bool invalidpassword = false;

          if(page_login.IsEmpty() || page_password.IsEmpty())
            {
              DIOWEBSERVER_XEVENT xevent(this, DIOWEBSERVER_XEVENT_TYPE_DOAUTHENTICATE);

              xevent.SetServer(server);
              xevent.SetConnection(connection);
              xevent.SetRequest(request);

              PostEvent(&xevent);

              if(xevent.IsRequestAuthenticate())
                {
                  page_login    = _L("usr");
                  page_password = _L("pwd");

                } else invalidpassword = true;
            }
           else
            {
              DIOWEBSERVER_XEVENT xevent(this, DIOWEBSERVER_XEVENT_TYPE_CHECKAUTHENTICATE);

              xevent.SetServer(server);
              xevent.SetConnection(connection);
              xevent.SetRequest(request);

              PostEvent(&xevent);

              if(!xevent.IsRequestAuthenticate()) invalidpassword = true;
            }

          if(invalidpassword)
            {
              XSTRING leyend;

              leyend = XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD);
              GenerateResponse_Error(connection, DIOWEBHEADER_RESULT_NOTFOUND, leyend);

              APPFLOW_LOG_ENTRY(XLOGLEVEL_ERROR, DIOWEBSERVER_LOGSECTIONID, false, leyend.Get());

              return true;
            }
        }
    }

  //-------------------------------------------------------------------------------------------------------------------------------
  /*
  if(useragentID)
    {
      XSTRING browser;
      XSTRING SO;

      if(useragentID->Get(request->GetUserAgent()->Get(), browser, SO, 5))
        {
          APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER), browser.Get(), SO.Get());
        }
    }
  */  

  //-------------------------------------------------------------------------------------------------------------------------------

  status  = false;
  nparams = querystring.GetParamsFromURL(resourceconv.Get())?true:false;

  querystring.DeleteParamsFromURL(resourceconv);

  if(!resourceconv.Compare(_L("/"), true)) resourceconv.Set(_L("/index.html"));


  //-------------------------------------------------------------------------------------------------------------------------------


  XPATH xpathfile;

  if(!cfg || cfg->WebServer_PathResources()->IsEmpty())
         GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_WEB, xpathfile);
    else xpathfile = cfg->WebServer_PathResources()->Get();

  xpathfile.Slash_Delete();
  xpathfile += resourceconv.Get();

  //-------------------------------------------------------------------------------------------------------------------------------

  if(resourceconv.GetExtension(extension))
    {
      int nplugins = webserver->GetPluginsManager()->Plugins_GetNPlugins();
      for(int c=0; c<nplugins; c++)
        {
          DIOWEBSERVER_PLUGIN* plugin = webserver->GetPluginsManager()->Plugins_Get(c);
          if(plugin)
            {
              if(plugin->PageExtension_IsContent(extension))
                {
                  DIOWEBHEADER_RESULT    headerresult = DIOWEBHEADER_RESULT_OK;
                  XSTRING                result;

                  bool istls = (server && server->GetDIOStreamCFG())?server->GetDIOStreamCFG()->IsTLS():false;

                  status = plugin->PageExtension(xpathfile, request, &querystring, headerresult, result, istls);
                  if(status)
                    {
                      XSTRING head;
                      XSTRING data;

                      plugin->SeparedResult(result, head, data);

                      status = SendRequest(connection, headerresult, data, 5, &head);
                    }
                   else
                    {
                      XSTRING leyend;

                      leyend.Format(XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE), plugin->GetName()->Get());

                      GenerateResponse_Error(connection, DIOWEBHEADER_RESULT_NOTFOUND, leyend);
                      APPFLOW_LOG_ENTRY(XLOGLEVEL_ERROR, DIOWEBSERVER_LOGSECTIONID, false, leyend.Get());
                    }

                  XSTRING IPstring;
                  connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);           

                  if(!status)
                    {
                      APPFLOW_LOG_ENTRY((status?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT), IPstring.Get(), resourceconv.Get(), status?XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT):XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND));
                    }

                  ispagepluging = true;
                }
            }
        }
    }

  if(!ispagepluging)
    {
      if(!nparams && !isapirestonly)
        {
          status = SendRequest(connection, DIOWEBHEADER_RESULT_OK, xpathfile, 5);
          if(!status)
            {
              // Brower close the conextion in thread
              if(!connection->GetDIOStream()->IsConnected()) status = true;
            }
        }

      if(!status)
        {
          if(!nparams && !isapirestonly)
            {
              DIOWEBSERVER_XEVENT xevent(this, DIOWEBSERVER_XEVENT_TYPE_REQUEST);

              xevent.SetServer(server);
              xevent.SetConnection(connection);
              xevent.SetRequest(request);

              PostEvent(&xevent);

              status = xevent.GetStatus();
            }
        }

      //-------------------------------------------------------------------------------------------------------------------------------

      if(!status)
        {
          DIOWEBPAGEHTMLCREATOR* webHTMLpage = GEN_NEW DIOWEBPAGEHTMLCREATOR();
          if(webHTMLpage)
            {
              DIOWEBSERVER_XEVENT xevent(this, DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT);

              xevent.SetServer(server);
              xevent.SetConnection(connection);
              xevent.SetRequest(request);
              xevent.SetQueryString(&querystring);

              PostEvent(&xevent);

              //DIOWEBHEADER_RESULT result = xevent.GetResult();

              status = xevent.GetStatus();
              if(status)
                {
                  if(!xevent.GetOutputString()->IsEmpty())
                    {
                      webHTMLpage->Set(xevent.GetOutputString()->Get());
                      status = SendRequest(connection, DIOWEBHEADER_RESULT_OK, (*webHTMLpage), 5);
                    }
                   else
                    {
                      status = SendRequest(connection, DIOWEBHEADER_RESULT_OK, xevent.GetOutputBuffer(), 5);
                    }
                }
               else
                {
                  XSTRING leyend;

                  leyend = XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS);
                  GenerateMessagePage(leyend, (*webHTMLpage));

                  GenerateResponse_Error(connection, DIOWEBHEADER_RESULT_NOTFOUND, leyend);

                  APPFLOW_LOG_ENTRY(XLOGLEVEL_ERROR, DIOWEBSERVER_LOGSECTIONID, false, leyend.Get());
                }

              XSTRING IPstring;
              connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);   

              if(!status)
                {
                  APPFLOW_LOG_ENTRY((status?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), DIOWEBSERVER_LOGSECTIONID, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT), IPstring.Get(), resourceconv.Get(), status?XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT):XT_L(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND));
                }

              GEN_DELETE webHTMLpage;
            }
        }
       else 
        {
          XSTRING IPstring;
          connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);  

          // APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID, false, _L("Request from [%s] to the web server of file \"%s\" sent."), IPstring.Get(), xpathfile.Get());
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GenerateMessagePage(XCHAR* leyend, DIOWEBPAGEHTMLCREATOR& webHTMLpage)
* @brief      Generate message page
* @ingroup    APPFLOW
* 
* @param[in]  leyend : Leyend pointer to use.
* @param[in]  webHTMLpage : Web HTM Lpage value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GenerateMessagePage(XCHAR* leyend, DIOWEBPAGEHTMLCREATOR& webHTMLpage)
{
  webHTMLpage.Printf(_L("<font color=\"red\" face=\"Courier New\" size=\"2\">"));
  webHTMLpage.Printf(_L("<b>%s.</b>\n"), leyend);
  webHTMLpage.Printf(_L("</font>"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GenerateMessagePage(XSTRING& leyend, DIOWEBPAGEHTMLCREATOR& webHTMLpage)
* @brief      Generate message page
* @ingroup    APPFLOW
* 
* @param[in]  leyend : Leyend value.
* @param[in]  webHTMLpage : Web HTM Lpage value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GenerateMessagePage(XSTRING& leyend, DIOWEBPAGEHTMLCREATOR& webHTMLpage)
{
  return GenerateMessagePage(leyend.Get(), webHTMLpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GenerateResponse_Error(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT result, XCHAR* leyend)
* @brief      Generate response error
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  result : Output result.
* @param[in]  leyend : Leyend pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GenerateResponse_Error(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT result, XCHAR* leyend)
{
  DIOWEBPAGEHTMLCREATOR* webHTMLpage = GEN_NEW DIOWEBPAGEHTMLCREATOR();
  if(!webHTMLpage) return false;

  GenerateMessagePage(leyend, (*webHTMLpage));

  connection->GetRequest()->GetResource()->Set("error.html");

  bool status = SendRequest(connection, result, (*webHTMLpage), 5);

  GEN_DELETE webHTMLpage;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GenerateResponse_Error(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT result, XSTRING& leyend)
* @brief      Generate response error
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  result : Output result.
* @param[in]  leyend : Leyend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GenerateResponse_Error(DIOWEBSERVER_CONNECTION* connection, DIOWEBHEADER_RESULT result, XSTRING& leyend)
{
  return GenerateResponse_Error(connection, result, leyend.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::GenerateResponse_Unauthorized(DIOWEBSERVER_CONNECTION* connection)
* @brief      Generate a 401 response including the header WWW-Authenticate, so the client asks for the credentials.
* @ingroup    APPFLOW
* 
* @param[in]  connection : Connection pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::GenerateResponse_Unauthorized(DIOWEBSERVER_CONNECTION* connection)
{
  DIOWEBSERVER_HEADER webserverheader;

  if(!connection)                                return false;
  if(!connection->GetDIOStream())                return false;
  if(!connection->GetDIOStream()->IsConnected()) return false;
  if(!connection->GetRequest())                  return false;

  webserverheader.Create(connection->GetRequest()->GetResource(), 1, 1, DIOWEBHEADER_RESULT_UNAUTHORIZED, 0, NULL, false, true);

  return webserverheader.Write(connection->GetDIOStream(), 5);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWWEBSERVER::End()
* @brief      End the object
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWWEBSERVER::End()
{
  if(!webserver) return false;

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST, (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST);

  if(isauthenticatedaccess)
    {
      DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_DOAUTHENTICATE);
      DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_CHECKAUTHENTICATE);
    }

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT, (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT);

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED, (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED);

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA, (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA);

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG, (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG);

  UnSubscribeEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED , (XSUBJECT *)webserver);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED);

  if(webserver)
    {
      GEN_DELETE webserver;
      webserver = NULL;
    }

  if(pluginPHP)
    {
      GEN_DELETE pluginPHP;
      pluginPHP = NULL;
    }

  /*
  if(useragentID)
    {
      GEN_DELETE useragentID;
      useragentID = NULL;
    }
  */

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWWEBSERVER::HandleEvent_WebServer(DIOWEBSERVER_XEVENT* event)
* @brief      Handle event web server
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWWEBSERVER::HandleEvent_WebServer(DIOWEBSERVER_XEVENT* event)
{
  switch(event->GetEventType())
    {
      case DIOWEBSERVER_XEVENT_TYPE_REQUEST                : ResolveRequest(event->GetServer(), event->GetConnection(), event->GetRequest());
                                                             break;

      case DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED    : if(!PostEvent(event))
                                                              {
                                                                // Accept connection with any version, protocol o resources.
                                                                event->SetStatus(true);
                                                                break;
                                                              }
                                                             break;

      case DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA     : PostEvent(event);
                                                             break;

      case DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED : PostEvent(event);
                                                             break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWWEBSERVER::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWWEBSERVER::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_WEBSERVER       : { DIOWEBSERVER_XEVENT* event = (DIOWEBSERVER_XEVENT*)xevent;
                                          if(!event) return;

                                          HandleEvent_WebServer(event);
                                        }
                                        break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWWEBSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWWEBSERVER::Clean()
{
  cfg                        = NULL;

  webserver                  = NULL;

  isauthenticatedaccess      = false;
  isapirestonly              = false;

  #ifdef DIO_STREAMTLS_ACTIVE
  tlscredentialsprovider      = NULL;
  tlscredentialsproviderowner = false;
  tlssecretprovider           = NULL;
  tlssecretproviderowner      = false;
  #endif

//useragentID                = NULL;

  pluginPHP                  = NULL;
}
