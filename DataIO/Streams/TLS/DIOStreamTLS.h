/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS.h
*
* @class      DIOSTREAMTLS
* @brief      Data Input/Output Stream TLS (Transport Layer Security) class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XFactory.h"
#include "XBuffer.h"
#include "XTimer.h"

#include "DIOStream.h"
#include "DIOStreamTLSConfig.h"
#include "DIOStreamTLS13Session.h"
#include "DIOStreamTLS13HandshakeClient.h"
#include "DIOStreamTLS13HandshakeServer.h"
#include "DIOStreamTLSMessagesHandShakeClientHello.h"
#include "DIOStreamTLSMessagesHandShakeServerHello.h"

// TLS 1.2 is optional.  Keep the dispatcher source-compatible when a target
// deliberately disables the TLS 1.2 implementation: the inline null object
// below is never selected by Open(), but avoids making TLS 1.3 builds depend
// on TLS 1.2 translation units at link time.
#ifdef DIO_STREAMTLS12_ACTIVE
#include "DIOStreamTLS12Session.h"
#include "DIOStreamTLS12HandshakeClient.h"
#else
enum DIOSTREAMTLS12SESSION_RESULT { DIOSTREAMTLS12SESSION_RESULT_NONE = 0 };
enum DIOSTREAMTLS12HANDSHAKECLIENT_STATE { DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR = 0, DIOSTREAMTLS12HANDSHAKECLIENT_STATE_READY_CLIENTFLIGHT = 1 };
enum DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR { DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE = 0 };
class DIOSTREAMTLS12SESSION
{
  public:
    template<typename... A> bool Alert_Create(A&&...) { return false; }
    template<typename... A> bool CloseNotify_Create(A&&...) { return false; }
    template<typename... A> XDWORD ApplicationData_Read(A&&...) { return 0; }
    template<typename... A> bool ApplicationData_CanProtect(A&&...) { return false; }
    template<typename... A> bool ApplicationData_Protect(A&&...) { return false; }
    template<typename... A> DIOSTREAMTLS12SESSION_RESULT ApplicationData_Process(A&&...) { return DIOSTREAMTLS12SESSION_RESULT_NONE; }
    template<typename... A> bool RecordInput_Add(A&&...) { return false; }
    bool IsIni() const { return false; }
    bool IsTransportClosedWithoutNotify() const { return false; }
    bool IsCloseNotifyReceived() const { return false; }
    bool IsCloseNotifySent() const { return false; }
    bool IsError() const { return true; }
    bool TransportClosed() const { return false; }
    XBUFFER* GetApplicationInput() { return NULL; }
    DIOSTREAMTLS_ALERT_DESCRIPTION GetLastRecordAlertDescription() const { return static_cast<DIOSTREAMTLS_ALERT_DESCRIPTION>(0); }
};
class DIOSTREAMTLS12HANDSHAKECLIENT
{
  public:
    void End() {}
    template<typename... A> bool Ini(A&&...) { return false; }
    template<typename... A> bool Capabilities_Set(A&&...) { return false; }
    template<typename... A> bool Authentication_Set(A&&...) { return false; }
    template<typename... A> bool ClientHello_Create(A&&...) { return false; }
    template<typename... A> bool ClientFlight_Create(A&&...) { return false; }
    template<typename... A> bool RecordInput_Add(A&&...) { return false; }
    template<typename... A> bool Process(A&&...) { return false; }
    bool IsAlgorithmRejected() const { return false; }
    bool IsHandshakeCompleted() const { return false; }
    DIOSTREAMTLS12HANDSHAKECLIENT_STATE GetState() const { return DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR; }
    DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR GetAuthenticationError() const { return DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE; }
    DIOSTREAMTLS12SESSION* GetSession() { static DIOSTREAMTLS12SESSION session; return &session; }
};
#endif



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLS_TIMEOUT                DIOSTREAMTLS_DEFAULT_CONNECTION_TIMEOUT


enum DIOSTREAMTLS_ERROR
{
  DIOSTREAMTLS_ERROR_NONE              = 0 ,
  DIOSTREAMTLS_ERROR_CONFIGURATION         ,
  DIOSTREAMTLS_ERROR_TRANSPORT             ,
  DIOSTREAMTLS_ERROR_HANDSHAKE             ,
  DIOSTREAMTLS_ERROR_RECORD                ,
  DIOSTREAMTLS_ERROR_TRUNCATED             ,
  DIOSTREAMTLS_ERROR_KEY_USAGE_LIMIT       ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

enum DIOSTREAMTLS_HANDSHAKERETRYCAUSE
{
  DIOSTREAMTLS_HANDSHAKERETRYCAUSE_NONE = 0,
  DIOSTREAMTLS_HANDSHAKERETRYCAUSE_VERSION,
  DIOSTREAMTLS_HANDSHAKERETRYCAUSE_ALGORITHM,
  DIOSTREAMTLS_HANDSHAKERETRYCAUSE_OTHER,
};

template<typename T>
class DIOSTREAMTLS : public T
{
  public:
                                            DIOSTREAMTLS                            ()
                                            {
                                              Clean();

                                              timeout = DIOSTREAMTLS_TIMEOUT;
                                            }

    virtual                                ~DIOSTREAMTLS                            ()
                                            {
                                              handshakeclient.End();
                                              handshakeserver.End();
                                              session.End();
                                              handshakeclient12.End();

                                              Clean();
                                            }

    bool                                    Open                                    ()
                                            {
                                              DIOSTREAMTLSCONFIG* config;
                                              XCHAR*                servername;
                                              XCHAR*                sniservername;
                                              XSTRING               canonicalservername;
                                              DIOURL_HOSTTYPE       servernametype;
                                              XWORD                 minversion;
                                              XWORD                 maxversion;
                                              bool                  starttls12;

                                              if(!isclosed)
                                                {
                                                  if(!Close()) return false;
                                                }

                                              tlserror  = DIOSTREAMTLS_ERROR_NONE;
                                              T::SetLastDIOError(DIOSTREAMERROR_NONE);
                                              isclosing = false;

                                              config = dynamic_cast<DIOSTREAMTLSCONFIG*>(T::GetConfig());
                                               if(!config)
                                                 {
                                                   TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                   return false;
                                                 }

                                               if(!config->Freeze())
                                                 {
                                                   TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                   return false;
                                                 }

                                               connectiontimeout = config->GetConnectionTimeout();
                                              handshaketimeout  = config->GetHandshakeTimeout();
                                              timeout           = connectiontimeout;

                                              minversion = config->GetMinVersion();
                                              maxversion = config->GetMaxVersion();

                                              if(((minversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) && (minversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_3)) ||
                                                 ((maxversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) && (maxversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_3)) ||
                                                 (minversion > maxversion))
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                  return false;
                                                }

#ifndef DIO_STREAMTLS12_ACTIVE
                                              if(minversion == DIOSTREAMTLS_MSG_VERSION_TLS_1_2)
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                  return false;
                                                }
#endif

                                              if(config->IsServer())
                                                {
                                                  if((minversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_3) ||
                                                     (maxversion != DIOSTREAMTLS_MSG_VERSION_TLS_1_3))
                                                    {
                                                      TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                      return false;
                                                    }

                                                  if(Handshake_ServerAttempt(config))
                                                    {
                                                      T::SetStatus(DIOSTREAMSTATUS_CONNECTED);
                                                      return true;
                                                    }

                                                  return false;
                                                }

                                              servername = config->GetServerName()->GetSize()?config->GetServerName()->Get():config->GetRemoteURL()->Get();
                                              if(!servername || !servername[0])
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                  return false;
                                                }

                                              if(!DIOURL::Host_Canonicalize(servername, canonicalservername, servernametype))
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                  return false;
                                                }

                                              servername    = canonicalservername.Get();
                                              sniservername = (servernametype == DIOURL_HOSTTYPE_DNS)?servername:NULL;

#ifdef DIO_STREAMTLS12_ACTIVE
                                              dualversionmode = (minversion == DIOSTREAMTLS_MSG_VERSION_TLS_1_2) && (maxversion == DIOSTREAMTLS_MSG_VERSION_TLS_1_3);
                                              starttls12       = (maxversion == DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
#else
                                              dualversionmode = false;
                                              starttls12       = false;
#endif

                                              if(Handshake_Attempt(config, servername, sniservername, starttls12))
                                                {
                                                  T::SetStatus(DIOSTREAMSTATUS_CONNECTED);
                                                  return true;
                                                }

                                              if(dualversionmode &&
                                                 !usingtls12 &&
                                                 (tlserror == DIOSTREAMTLS_ERROR_HANDSHAKE) &&
                                                 (retrycause == DIOSTREAMTLS_HANDSHAKERETRYCAUSE_VERSION))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_NONE;
                                                  T::SetLastDIOError(DIOSTREAMERROR_NONE);

                                                  if(Handshake_Attempt(config, servername, sniservername, true))
                                                    {
                                                      T::SetStatus(DIOSTREAMSTATUS_CONNECTED);
                                                      return true;
                                                    }

                                                }

                                              return false;
                                            }

    bool                                    Handshake_Attempt                       (DIOSTREAMTLSCONFIG* config, XCHAR* servername, XCHAR* sniservername, bool astls12)
                                            {
                                              handshakeclient.End();
                                              session.End();
                                              handshakeclient12.End();

                                              usingtls12       = astls12;
                                              retrycause = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_NONE;

                                              if(usingtls12)
                                                {
                                                  if(!handshakeclient12.Ini(dualversionmode) ||
                                                     !handshakeclient12.Capabilities_Set(config) ||
                                                     !handshakeclient12.Authentication_Set(servername, config->GetTrustedRoots()))
                                                    {
                                                      TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                      return false;
                                                    }
                                                }
                                               else
                                                {
                                                  if(!session.Ini(config->GetCipherSuite(), DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
                                                     !session.MemoryPolicy_Set((*config->GetMemoryPolicy())) ||
                                                     !handshakeclient.Ini(&session) ||
                                                     !handshakeclient.Capabilities_Set(config) ||
                                                     !handshakeclient.Authentication_Set(servername, config->GetTrustedRoots()))
                                                    {
                                                      TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                      return false;
                                                    }
                                                }

                                              T::ResetOutXBuffer();
                                              T::ResetInXBuffer();

                                              if(!T::Open())
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  if(T::PeekLastDIOError() == DIOSTREAMERROR_NONE)
                                                    {
                                                      T::SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
                                                    }
                                                  return false;
                                                }

                                              isclosed = false;

                                              if(!T::WaitToConnected(connectiontimeout))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  if(T::PeekLastDIOError() == DIOSTREAMERROR_NONE)
                                                    {
                                                      T::SetLastDIOError(DIOSTREAMERROR_TCPTIMEOUT);
                                                    }
                                                  Close_OnError();
                                                  return false;
                                                }

                                              if(!T::Proxy_Connect(connectiontimeout))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  Close_OnError();
                                                  return false;
                                                }

                                              T::SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);

                                              if(!Handshake_Client(sniservername))
                                                {
                                                  if(tlserror == DIOSTREAMTLS_ERROR_NONE) TLSError_Set(DIOSTREAMTLS_ERROR_HANDSHAKE);

                                                  if(IsAuthenticationSecurityFailure())
                                                    {
                                                      T::SetLastDIOError(DIOSTREAMERROR_TLSAUTHENTICATION);
                                                    }
                                                   else T::SetLastDIOError(DIOSTREAMERROR_TLSPROTOCOL);

                                                  if(usingtls12 && handshakeclient12.IsAlgorithmRejected())
                                                    {
                                                      retrycause = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_ALGORITHM;
                                                    }

                                                  if((!usingtls12 && (session.GetEpoch(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL) != DIOSTREAMTLS13SESSION_EPOCH_CLEAR)) ||
                                                     (usingtls12 && handshakeclient12.GetSession()->IsIni()))
                                                    {
                                                      Alert_Send(DIOSTREAMTLS_ALERT_LEVEL_FATAL, DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE);
                                                    }

                                                  Close_OnError();
                                                  return false;
                                                }

                                              return true;
                                            }

    bool                                    Handshake_ServerAttempt                 (DIOSTREAMTLSCONFIG* config)
                                            {
                                              handshakeclient.End();
                                              handshakeserver.End();
                                              session.End();
                                              handshakeclient12.End();

                                              usingtls12  = false;
                                              isserverrole = true;

                                              if(!session.Ini(config->GetCipherSuite(), DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER) ||
                                                 !session.MemoryPolicy_Set((*config->GetMemoryPolicy())) ||
                                                 !handshakeserver.Ini(&session, config))
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_CONFIGURATION);
                                                  return false;
                                                }

                                              T::ResetOutXBuffer();
                                              T::ResetInXBuffer();

                                              if(!T::Open())
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  if(T::PeekLastDIOError() == DIOSTREAMERROR_NONE)
                                                    {
                                                      T::SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
                                                    }
                                                  return false;
                                                }

                                              isclosed = false;

                                              if(!T::WaitToConnected(connectiontimeout))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  if(T::PeekLastDIOError() == DIOSTREAMERROR_NONE)
                                                    {
                                                      T::SetLastDIOError(DIOSTREAMERROR_TCPTIMEOUT);
                                                    }
                                                  Close_OnError();
                                                  return false;
                                                }

                                              T::SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);

                                              if(!Handshake_Server())
                                                {
                                                  if(tlserror == DIOSTREAMTLS_ERROR_NONE) TLSError_Set(DIOSTREAMTLS_ERROR_HANDSHAKE);

                                                  DIOSTREAMTLS_ALERT_DESCRIPTION alertdescription = handshakeserver.GetErrorAlertDescription();

                                                  if(handshakeserver.ShouldSendErrorAlert())
                                                    {
                                                      Alert_Send(DIOSTREAMTLS_ALERT_LEVEL_FATAL, alertdescription);
                                                    }

                                                  Close_OnError();
                                                  return false;
                                                }

                                              return true;
                                            }

    XDWORD                                  Read                                    (XBYTE* buffer, XDWORD size)
                                            {
                                              if(!buffer || !size) return 0;

                                              XDWORD sizeread = usingtls12?handshakeclient12.GetSession()->ApplicationData_Read(buffer, size):session.ApplicationData_Read(buffer, size);
                                              if(!sizeread)
                                                {
                                                  if(!ApplicationInput_Process()) return 0;

                                                  sizeread = usingtls12?handshakeclient12.GetSession()->ApplicationData_Read(buffer, size):session.ApplicationData_Read(buffer, size);
                                                }

                                              if(sizeread)
                                                {
                                                  T::AddNBytesRead(sizeread);
                                                  if(this->xtimernotactivity) this->xtimernotactivity->Reset();
                                                }

                                              return sizeread;
                                            }

    XDWORD                                  Read                                    (XBUFFER& buffer)
                                            {
                                              if(!buffer.GetSize()) return 0;

                                              XBUFFER* applicationinput = usingtls12?handshakeclient12.GetSession()->GetApplicationInput():session.GetApplicationInput();

                                              if(applicationinput->IsEmpty())
                                                {
                                                  if(!ApplicationInput_Process() || applicationinput->IsEmpty()) return 0;
                                                }

                                              XDWORD sizeread = Read(buffer.Get(), buffer.GetSize());
                                              if(sizeread)
                                                {
                                                  if(sizeread != buffer.GetSize()) buffer.Resize(sizeread);
                                                }
                                               else
                                                {
                                                  buffer.Delete();
                                                }

                                              return buffer.GetSize();
                                            }

    XDWORD                                  Write                                   (XBYTE* buffer, XDWORD size)
                                            {
                                              XBUFFER records;

                                              if(!buffer || !size || isclosing || isclosed ||
                                                 (T::GetStatus() != DIOSTREAMSTATUS_CONNECTED) ||
                                                 !IsHandshakeCompleted())
                                                {
                                                  return 0;
                                                }

                                              if(usingtls12 && !handshakeclient12.GetSession()->ApplicationData_CanProtect(size))
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_KEY_USAGE_LIMIT);
                                                  Close_OnError();
                                                  return 0;
                                                }

                                              bool protectstatus = usingtls12?handshakeclient12.GetSession()->ApplicationData_Protect(buffer, size, records):
                                                                              session.ApplicationData_Protect(buffer, size, records);

                                              if(!protectstatus)
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_RECORD);
                                                  Close_OnError();
                                                  return 0;
                                                }

                                              if(!Transport_Write(records))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  Close_OnError();
                                                  return 0;
                                                }

                                              T::AddNBytesWrite(size);
                                              if(this->xtimernotactivity) this->xtimernotactivity->Reset();

                                              return size;
                                            }

    bool                                    KeyUpdate                               (bool requestpeer = false)
                                            {
                                              XBUFFER records;

                                              if(usingtls12) return false;

                                              if(isclosing || isclosed ||
                                                 (T::GetStatus() != DIOSTREAMSTATUS_CONNECTED) ||
                                                 !IsHandshakeCompleted() ||
                                                 !session.KeyUpdate_Create(requestpeer, records))
                                                {
                                                  return false;
                                                }

                                              if(!Transport_Write(records))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  Close_OnError();
                                                  return false;
                                                }

                                              return true;
                                            }

    bool                                    IsSessionResumed                        ()
                                            {
                                              if(usingtls12) return false;
                                              return isserverrole?handshakeserver.IsSessionResumed():handshakeclient.IsSessionResumed();
                                            }

    bool                                    IsClientAuthenticated                   ()
                                            {
                                              if(usingtls12 || !isserverrole) return false;
                                              return handshakeserver.IsClientAuthenticated();
                                            }

    CIPHERCERTIFICATEX509*                  GetClientCertificate                    ()
                                            {
                                              if(usingtls12 || !isserverrole) return NULL;
                                              return handshakeserver.GetClientCertificate();
                                            }

    bool                                    WaitToFilledReadingBuffer               (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE)
                                            {
                                              XTIMER* xtimer;
                                              bool    status = false;

                                              xtimer = GEN_XFACTORY.CreateTimer();
                                              if(!xtimer) return false;

                                              xtimer->Reset();

                                              while(true)
                                                {
                                                  XBUFFER* applicationinput = usingtls12?handshakeclient12.GetSession()->GetApplicationInput():session.GetApplicationInput();
                                                  XDWORD   size             = applicationinput->GetSize();

                                                  if(((filledto == DIOSTREAM_SOMETHINGTOREAD) && size) ||
                                                     ((filledto != DIOSTREAM_SOMETHINGTOREAD) && ((int)size >= filledto)))
                                                    {
                                                      status = true;
                                                      break;
                                                    }

                                                  if(!ApplicationInput_Process()) break;

                                                  size = applicationinput->GetSize();

                                                  if(((filledto == DIOSTREAM_SOMETHINGTOREAD) && size) ||
                                                     ((filledto != DIOSTREAM_SOMETHINGTOREAD) && ((int)size >= filledto)))
                                                    {
                                                      status = true;
                                                      break;
                                                    }

                                                  if((usingtls12?handshakeclient12.GetSession()->IsCloseNotifyReceived():session.IsCloseNotifyReceived()) ||
                                                     (usingtls12?handshakeclient12.GetSession()->IsError():session.IsError()) ||
                                                     (T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED))
                                                    {
                                                      break;
                                                    }

                                                  if((timeout != XTIMER_INFINITE) && (xtimer->GetMeasureSeconds() >= (XDWORD)timeout)) break;

                                                  DIOSTREAMCONFIG* streamconfig = T::GetConfig();
                                                  T::Wait(streamconfig?streamconfig->GetPollInterval():DIOSTREAM_TIMEINWAITFUNCTIONS);
                                                }

                                              GEN_XFACTORY.DeleteTimer(xtimer);

                                              if(!usingtls12 && earlydataattempted) earlydataaccepted = handshakeclient.IsEarlyDataAccepted();
                                              earlydata.Delete();
                                              return status;
                                            }

    XBUFFER*                                GetInXBuffer                            ()
                                            {
                                              ApplicationInput_Process();

                                              return usingtls12?handshakeclient12.GetSession()->GetApplicationInput():session.GetApplicationInput();
                                            }

    bool                                    Disconnect                              ()
                                            {
                                              if(isclosed) return true;

                                              bool status = true;

                                              if(!isclosing && IsHandshakeCompleted() &&
                                                 (T::GetStatus() != DIOSTREAMSTATUS_DISCONNECTED))
                                                {
                                                  status = CloseNotify_Send();
                                                }

                                              isclosing = true;

                                              if(T::GetStatus() != DIOSTREAMSTATUS_DISCONNECTED)
                                                {
                                                  status = T::Disconnect() && status;
                                                }

                                              T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);

                                              return status;
                                            }

    bool                                    Close                                   ()
                                            {
                                              if(isclosed) return true;

                                              bool status = true;

                                              if(!isclosing && IsHandshakeCompleted() &&
                                                 (T::GetStatus() != DIOSTREAMSTATUS_DISCONNECTED))
                                                {
                                                  status = CloseNotify_Send();
                                                }

                                              isclosing = true;

                                              status = T::Close() && status;

                                              handshakeclient.End();
                                              handshakeserver.End();
                                              session.End();
                                              handshakeclient12.End();

                                              T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);

                                              isclosed  = true;
                                              isclosing = false;

                                              return status;
                                            }

    int                                     GetTimeout                              ()
                                            {
                                              return timeout;
                                            }

    void                                    SetTimeout                              (int timeout = DIOSTREAMTLS_TIMEOUT)
                                            {
                                              this->timeout = timeout;
                                            }

    DIOSTREAMTLS_ERROR                      GetLastTLSError                         ()
                                            {
                                              return tlserror;
                                            }

    void                                    TLSError_Set                            (DIOSTREAMTLS_ERROR error)
                                            {
                                              tlserror = error;

                                              switch(error)
                                                {
                                                  case DIOSTREAMTLS_ERROR_CONFIGURATION   : T::SetLastDIOError(DIOSTREAMERROR_TLSCONFIGURATION);
                                                                                          break;

                                                  case DIOSTREAMTLS_ERROR_HANDSHAKE       :
                                                  case DIOSTREAMTLS_ERROR_RECORD          :
                                                  case DIOSTREAMTLS_ERROR_TRUNCATED       :
                                                  case DIOSTREAMTLS_ERROR_KEY_USAGE_LIMIT : T::SetLastDIOError(DIOSTREAMERROR_TLSPROTOCOL);
                                                                                          break;

                                                  case DIOSTREAMTLS_ERROR_NONE            : T::SetLastDIOError(DIOSTREAMERROR_NONE);
                                                                                          break;

                                                                                  default : break;
                                                }
                                            }

    DIOSTREAMTLS13SESSION*                    GetTLSSession                           ()
                                            {
                                              return &session;
                                            }

    DIOSTREAMTLS13HANDSHAKECLIENT*            GetTLSHandshakeClient                   ()
                                            {
                                              return &handshakeclient;
                                            }

    bool                                    EarlyData_Set                            (XBUFFER& data)
                                            {
                                              earlydata.Delete();
                                              earlydataattempted = false;
                                              earlydataaccepted = false;
                                              return data.IsEmpty()?true:earlydata.Add(data);
                                            }

    void                                    EarlyData_Delete                         ()
                                            {
                                              earlydata.Delete();
                                              earlydataattempted = false;
                                              earlydataaccepted = false;
                                            }

    bool                                    WasEarlyDataAttempted                    () { return earlydataattempted; }
    bool                                    IsEarlyDataAccepted                      () { return earlydataaccepted; }

    bool                                    IsUsingTLS12                            ()
                                            {
                                              return usingtls12;
                                            }

    DIOSTREAMTLS12SESSION*                  GetTLSSession12                         ()
                                            {
                                              
                                              return handshakeclient12.GetSession();
                                            }

    DIOSTREAMTLS12HANDSHAKECLIENT*          GetTLSHandshakeClient12                 ()
                                            {
                                              return &handshakeclient12;
                                            }

    bool                                    IsHandshakeCompleted                    ()
                                            {
                                              if(usingtls12)  return handshakeclient12.IsHandshakeCompleted();
                                              if(isserverrole) return handshakeserver.IsHandshakeCompleted();

                                              return handshakeclient.IsHandshakeCompleted();
                                            }

    DIOSTREAMTLS13HANDSHAKESERVER*            GetTLSHandshakeServer                   ()
                                            {
                                              return &handshakeserver;
                                            }

  private:

    bool                                    Handshake_Client                        (XCHAR* servername)
                                            {
                                              XBUFFER clienthello;
                                              XBUFFER records;
                                              XTIMER* xtimer;
                                              bool    serverhelloprocessed = false;
                                              bool    status               = false;

                                              if(usingtls12)
                                                {
                                                  
                                                  if(!handshakeclient12.ClientHello_Create(servername, clienthello, records) ||
                                                     !Transport_Write(records))
                                                    {
                                                      return false;
                                                    }
                                                }
                                               else
                                                {
                                                  if(!earlydata.IsEmpty() && !handshakeclient.EarlyData_Prepare(earlydata.GetSize()))
                                                    {
                                                      return false;
                                                    }
                                                  if(!handshakeclient.ClientHello_Create(servername, clienthello, records) ||
                                                     !Transport_Write(records))
                                                    {
                                                      return false;
                                                    }

                                                  if(!earlydata.IsEmpty() && handshakeclient.IsEarlyDataOffered())
                                                    {
                                                      XBUFFER earlyrecords;
                                                      if(earlydata.GetSize() > handshakeclient.GetMaximumEarlyDataSize() ||
                                                         !handshakeclient.EarlyData_Protect(earlydata, earlyrecords) || !Transport_Write(earlyrecords))
                                                        {
                                                          return false;
                                                        }
                                                      earlydataattempted = true;
                                                    }
                                                }

                                              xtimer = GEN_XFACTORY.CreateTimer();
                                              if(!xtimer) return false;

                                              xtimer->Reset();

                                              while((handshaketimeout == XTIMER_INFINITE) ||
                                                    (xtimer->GetMeasureSeconds() < (XDWORD)handshaketimeout))
                                                {
                                                  XBUFFER input;

                                                  if(!Transport_Read(input))
                                                    {
                                                      if(T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) break;

                                                      DIOSTREAMCONFIG* streamconfig = T::GetConfig();
                                                      T::Wait(streamconfig?streamconfig->GetPollInterval():DIOSTREAM_TIMEINWAITFUNCTIONS);
                                                      continue;
                                                    }

                                                  if(usingtls12)
                                                    {
                                                      if(!handshakeclient12.RecordInput_Add(input) || !handshakeclient12.Process()) break;

                                                      if(handshakeclient12.GetState() == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_READY_CLIENTFLIGHT)
                                                        {
                                                          XBUFFER flightrecords;

                                                          if(!handshakeclient12.ClientFlight_Create(flightrecords) || !Transport_Write(flightrecords)) break;

                                                          continue;
                                                        }

                                                      if(handshakeclient12.IsHandshakeCompleted())
                                                        {
                                                          status = true;
                                                          break;
                                                        }

                                                      if(handshakeclient12.GetState() == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR) break;

                                                      continue;
                                                    }

                                                  if(!handshakeclient.RecordInput_Add(input)) break;

                                                  if(!serverhelloprocessed &&
                                                     !ServerHello_Process(serverhelloprocessed, servername)) break;

                                                  if(serverhelloprocessed && !handshakeclient.Process()) break;

                                                  if(handshakeclient.IsServerFinishedVerified())
                                                    {
                                                      XBUFFER clientfinished;
                                                      XBUFFER clientfinishedrecords;

                                                      if(handshakeclient.ClientFinished_Create(clientfinished, clientfinishedrecords) &&
                                                         Transport_Write(clientfinishedrecords))
                                                        {
                                                          status = true;
                                                        }

                                                      break;
                                                    }
                                                }

                                              GEN_XFACTORY.DeleteTimer(xtimer);

                                              return status;
                                            }

    bool                                    ServerHello_Process                     (bool& serverhelloprocessed, XCHAR* servername = NULL)
                                            {
                                              while(!serverhelloprocessed)
                                                {
                                                  DIOSTREAMTLS_CONTENTTYPE   contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
                                                  DIOSTREAMTLS13SESSION_RESULT result;
                                                  XBUFFER                    plain;

                                                  result = session.Record_Extract(contenttype, plain);

                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE) return true;
                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
                                                    {
                                                      Alert_Send(DIOSTREAMTLS_ALERT_LEVEL_FATAL, session.GetLastRecordAlertDescription());
                                                      return false;
                                                    }

                                                  if(contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC)
                                                    {
                                                      if((plain.GetSize() != 1) || (plain.GetByte(0) != 1)) return false;
                                                      continue;
                                                    }

                                                  if(contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT)
                                                    {
                                                      DIOSTREAMTLS_MSG_ALERT alert;

                                                      if(alert.GetFromBuffer(plain, false))
                                                        {

                                                          if(alert.GetLevel() == DIOSTREAMTLS_ALERT_LEVEL_FATAL)
                                                            {
                                                              switch(alert.GetDescription())
                                                                {
                                                                  case DIOSTREAMTLS_ALERT_DESCRIPTION_PROTOCOL_VERSION      : retrycause = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_VERSION;
                                                                                                                              break;

                                                                  case DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE     :
                                                                  case DIOSTREAMTLS_ALERT_DESCRIPTION_INSUFFICIENT_SECURITY : retrycause = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_ALGORITHM;
                                                                                                                              break;

                                                                                                                       default : retrycause = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_OTHER;
                                                                                                                              break;
                                                                }
                                                            }
                                                        }
                                                       else
                                                        {
                                                        }

                                                      return false;
                                                    }

                                                  if(contenttype != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE ||
                                                     !session.HandshakeInput_Add(plain))
                                                    {
                                                      return false;
                                                    }

                                                  XBUFFER serverhello;
                                                  result = session.Handshake_Extract(serverhello);

                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE) continue;
                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)      return false;

                                                  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> serverhellomessage;
                                                  XBUFFER                                                          workbuffer;

                                                  workbuffer.Add(serverhello);
                                                  if(!serverhellomessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty()) return false;

                                                  if(serverhellomessage.GetBody()->IsHelloRetryRequest())
                                                    {
                                                      XBUFFER secondclienthello;
                                                      XBUFFER retryrecords;

                                                      if(!session.GetHandshakeInput()->IsEmpty() ||
                                                         !handshakeclient.HelloRetryRequest_Process(serverhello, secondclienthello, retryrecords) ||
                                                         !Transport_Write(retryrecords))
                                                        {
                                                          return false;
                                                        }

                                                      continue;
                                                    }

                                                  if(!handshakeclient.ServerHello_Process(serverhello)) return false;
                                                  if(!session.GetHandshakeInput()->IsEmpty())            return false;

                                                  serverhelloprocessed = true;
                                                }

                                              return true;
                                            }

    bool                                    Handshake_Server                        ()
                                            {
                                              XTIMER* xtimer;
                                              bool    clienthelloprocessed = false;
                                              bool    status               = false;

                                              xtimer = GEN_XFACTORY.CreateTimer();
                                              if(!xtimer) return false;

                                              xtimer->Reset();

                                              while((handshaketimeout == XTIMER_INFINITE) ||
                                                    (xtimer->GetMeasureSeconds() < (XDWORD)handshaketimeout))
                                                {
                                                  XBUFFER input;

                                                  if(!Transport_Read(input))
                                                    {
                                                      if(T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) break;

                                                      DIOSTREAMCONFIG* streamconfig = T::GetConfig();
                                                      T::Wait(streamconfig?streamconfig->GetPollInterval():DIOSTREAM_TIMEINWAITFUNCTIONS);
                                                      continue;
                                                    }

                                                  if(!handshakeserver.RecordInput_Add(input)) break;

                                                  if(!clienthelloprocessed &&
                                                     !ClientHello_Process(clienthelloprocessed)) break;

                                                  if(clienthelloprocessed && !handshakeserver.Process()) break;

                                                  if(handshakeserver.IsHandshakeCompleted())
                                                    {
                                                      status = true;
                                                      break;
                                                    }
                                                }

                                              GEN_XFACTORY.DeleteTimer(xtimer);

                                              return status;
                                            }

    bool                                    ClientHello_Process                     (bool& clienthelloprocessed)
                                            {
                                              while(!clienthelloprocessed)
                                                {
                                                  DIOSTREAMTLS_CONTENTTYPE   contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
                                                  DIOSTREAMTLS13SESSION_RESULT result;
                                                  XBUFFER                    plain;

                                                  result = session.Record_Extract(contenttype, plain);

                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE) return true;
                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)      return false;

                                                  if(contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT) return false;

                                                  if(contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC)
                                                    {
                                                      if(!handshakeserver.IsWaitingClientHelloRetry() ||
                                                         (plain.GetSize() != 1) || (plain.GetByte(0) != 1))
                                                        {
                                                          return false;
                                                        }

                                                      continue;
                                                    }

                                                  if(contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA &&
                                                     handshakeserver.IsWaitingClientHelloRetry() && handshakeserver.IsEarlyDataOffered())
                                                    {
                                                      continue;
                                                    }

                                                  if(contenttype != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE ||
                                                     !session.HandshakeInput_Add(plain))
                                                    {
                                                      return false;
                                                    }

                                                  XBUFFER clienthello;
                                                  result = session.Handshake_Extract(clienthello);

                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE) continue;
                                                  if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)      return false;

                                                  XBUFFER serverflightrecords;

                                                  if(!handshakeserver.ClientHello_Process(clienthello, serverflightrecords) ||
                                                     !Transport_Write(serverflightrecords))
                                                    {
                                                      return false;
                                                    }

                                                  clienthelloprocessed = !handshakeserver.IsWaitingClientHelloRetry();
                                                }

                                              return true;
                                            }

    bool                                    ApplicationInput_Process                ()
                                            {
                                              XBUFFER input;

                                              if(usingtls12)
                                                {
                                                  if(!handshakeclient12.GetSession()->IsIni()) return false;
                                                  if(handshakeclient12.GetSession()->IsTransportClosedWithoutNotify()) return false;

                                                  if(Transport_Read(input) && !handshakeclient12.GetSession()->RecordInput_Add(input))
                                                    {
                                                      TLSError_Set(DIOSTREAMTLS_ERROR_RECORD);
                                                      return false;
                                                    }

                                                  DIOSTREAMTLS12SESSION_RESULT result = handshakeclient12.GetSession()->ApplicationData_Process();

                                                  if(result == DIOSTREAMTLS12SESSION_RESULT_ERROR)
                                                    {
                                                      TLSError_Set(DIOSTREAMTLS_ERROR_RECORD);
                                                      Alert_Send(DIOSTREAMTLS_ALERT_LEVEL_FATAL, handshakeclient12.GetSession()->GetLastRecordAlertDescription());
                                                      T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                      return false;
                                                    }

                                                  if(handshakeclient12.GetSession()->IsCloseNotifyReceived())
                                                    {
                                                      if(!handshakeclient12.GetSession()->IsCloseNotifySent()) CloseNotify_Send();

                                                      T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                    }
                                                   else
                                                    {
                                                      if(!isclosed && (T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED))
                                                        {
                                                          if(!handshakeclient12.GetSession()->TransportClosed())
                                                            {
                                                              TLSError_Set(DIOSTREAMTLS_ERROR_TRUNCATED);
                                                              return !handshakeclient12.GetSession()->GetApplicationInput()->IsEmpty();
                                                            }
                                                        }
                                                    }

                                                  return true;
                                                }

                                              if(!session.IsIni()) return false;
                                              if(session.IsTransportClosedWithoutNotify()) return false;

                                              if(Transport_Read(input) && !session.RecordInput_Add(input))
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_RECORD);
                                                  return false;
                                                }

                                              DIOSTREAMTLS13SESSION_RESULT result = session.ApplicationData_Process();

                                              if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
                                                {
                                                  TLSError_Set(DIOSTREAMTLS_ERROR_RECORD);

                                                  if(session.GetReceivedAlertLevel() != DIOSTREAMTLS_ALERT_LEVEL_FATAL)
                                                    {
                                                      Alert_Send(DIOSTREAMTLS_ALERT_LEVEL_FATAL, session.GetLastRecordAlertDescription());
                                                    }

                                                  T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                  return false;
                                                }

                                              if(!isserverrole)
                                                {
                                                  while(true)
                                                    {
                                                      XBUFFER newsessionticket;
                                                      if(!session.NewSessionTicket_Extract(newsessionticket))
                                                        {
                                                          TLSError_Set(DIOSTREAMTLS_ERROR_HANDSHAKE);
                                                          T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                          return false;
                                                        }

                                                      if(newsessionticket.IsEmpty()) break;
                                                      if(!handshakeclient.NewSessionTicket_Process(newsessionticket))
                                                        {
                                                          TLSError_Set(DIOSTREAMTLS_ERROR_HANDSHAKE);
                                                          T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                          return false;
                                                        }
                                                    }
                                                }

                                              XBUFFER posthandshakeoutput;

                                              if(!session.PostHandshakeOutput_Extract(posthandshakeoutput) ||
                                                 (!posthandshakeoutput.IsEmpty() && !Transport_Write(posthandshakeoutput)))
                                                {
                                                  tlserror = DIOSTREAMTLS_ERROR_TRANSPORT;
                                                  T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                  return false;
                                                }

                                              if(session.IsCloseNotifyReceived())
                                                {
                                                  if(!session.IsCloseNotifySent()) CloseNotify_Send();

                                                  T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                }
                                               else
                                                {
                                                  if(!isclosed && (T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED))
                                                    {
                                                      if(!session.TransportClosed())
                                                        {
                                                          TLSError_Set(DIOSTREAMTLS_ERROR_TRUNCATED);
                                                          return !session.GetApplicationInput()->IsEmpty();
                                                        }
                                                    }
                                                }

                                              return true;
                                            }

    bool                                    Transport_Read                          (XBUFFER& input)
                                            {
                                              XBUFFER* transportinput = T::GetInXBuffer();
                                              XDWORD   size;

                                              input.Delete();

                                              if(!transportinput) return false;

                                              size = transportinput->GetSize();
                                              if(!size) return false;

                                              if(!input.Resize(size)) return false;

                                              if(transportinput->Extract(input.Get(), 0, size) != size)
                                                {
                                                  input.Delete();
                                                  return false;
                                                }

                                              return true;
                                            }

    bool                                    Transport_Write                         (XBUFFER& output)
                                            {
                                              XBUFFER* transportoutput;
                                              XTIMER*  xtimer;
                                              bool     status = true;

                                              if(output.IsEmpty()) return true;

                                              transportoutput = T::GetOutXBuffer();
                                              if(!transportoutput || !transportoutput->Add(output)) return false;

                                              xtimer = GEN_XFACTORY.CreateTimer();
                                              if(!xtimer) return false;

                                              xtimer->Reset();

                                              while(!T::IsFlushOutXBuffer())
                                                {
                                                  if(T::GetStatus() == DIOSTREAMSTATUS_DISCONNECTED)
                                                    {
                                                      T::ResetOutXBuffer();
                                                      status = false;
                                                      break;
                                                    }

                                                  if(timeout != XTIMER_INFINITE)
                                                    {
                                                      if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout)
                                                        {
                                                          T::ResetOutXBuffer();
                                                          status = false;
                                                          break;
                                                        }
                                                    }

                                                  DIOSTREAMCONFIG* streamconfig = T::GetConfig();
                                                  T::Wait(streamconfig?streamconfig->GetPollInterval():DIOSTREAM_TIMEINWAITFUNCTIONS);
                                                }

                                              GEN_XFACTORY.DeleteTimer(xtimer);

                                              return status;
                                            }

    bool                                    Alert_Send                              (DIOSTREAMTLS_ALERT_LEVEL level, DIOSTREAMTLS_ALERT_DESCRIPTION description)
                                            {
                                              XBUFFER records;

                                              bool status = usingtls12?handshakeclient12.GetSession()->Alert_Create(level, description, records):
                                                                        session.Alert_Create(level, description, records);

                                              if(!status) return false;

                                              return Transport_Write(records);
                                            }

    bool                                    CloseNotify_Send                        ()
                                            {
                                              XBUFFER records;

                                              bool status = usingtls12?handshakeclient12.GetSession()->CloseNotify_Create(records):
                                                                        session.CloseNotify_Create(records);

                                              if(!status)          return false;
                                              if(records.IsEmpty()) return true;

                                              return Transport_Write(records);
                                            }

    bool                                    IsAuthenticationSecurityFailure          ()
                                            {
                                              if(usingtls12)
                                                {
                                                  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR error = handshakeclient12.GetAuthenticationError();

                                                  return ((error == DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE)       ||
                                                          (error == DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE) ||
                                                          (error == DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_DOWNGRADEDETECTED));
                                                }

                                              DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR error = handshakeclient.GetAuthenticationError();

                                              return ((error == DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE)       ||
                                                      (error == DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY));
                                            }

    void                                    Close_OnError                           ()
                                            {
                                              isclosing = true;

                                              T::Close();
                                              T::SetStatus(DIOSTREAMSTATUS_DISCONNECTED);

                                              isclosed  = true;
                                              isclosing = false;
                                            }

    void                                    Clean                                   ()
                                            {
                                              timeout         = 0;
                                              connectiontimeout = DIOSTREAMTLS_DEFAULT_CONNECTION_TIMEOUT;
                                              handshaketimeout  = DIOSTREAMTLS_DEFAULT_HANDSHAKE_TIMEOUT;
                                              tlserror        = DIOSTREAMTLS_ERROR_NONE;
                                              isclosed        = true;
                                              isclosing       = false;
                                              usingtls12      = false;
                                              isserverrole    = false;
                                              dualversionmode = false;
                                              retrycause      = DIOSTREAMTLS_HANDSHAKERETRYCAUSE_NONE;
                                              earlydata.Delete();
                                              earlydataattempted = false;
                                              earlydataaccepted = false;
                                            }


    int                                     timeout;
    int                                     connectiontimeout;
    int                                     handshaketimeout;
    DIOSTREAMTLS_ERROR                      tlserror;
    bool                                    isclosed;
    bool                                    isclosing;

    DIOSTREAMTLS_HANDSHAKERETRYCAUSE        retrycause;

    DIOSTREAMTLS13SESSION                   session;
    DIOSTREAMTLS13HANDSHAKECLIENT           handshakeclient;
    DIOSTREAMTLS13HANDSHAKESERVER           handshakeserver;
    bool                                    isserverrole;

    DIOSTREAMTLS12HANDSHAKECLIENT           handshakeclient12;
    bool                                    usingtls12;
    bool                                    dualversionmode;
    XBUFFER                                 earlydata;
    bool                                    earlydataattempted;
    bool                                    earlydataaccepted;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
