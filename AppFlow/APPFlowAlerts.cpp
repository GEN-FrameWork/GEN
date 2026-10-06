/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowAlerts.cpp
* 
* @class      APPFLOWALERTS
* @brief      Application Flow Alerts class
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



#ifdef DIO_ALERTS_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "APPFlowAlerts.h"

#include "XLog.h"
#include "XSystem.h"

#include "DIOStream.h"
#include "DIOAlerts.h"

#include "APPFlowBase.h"
#include "APPFlowCFG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPFLOWALERTS* APPFLOWALERTS::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWALERTS::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWALERTS::GetIsInstanced()
{
  return instance != NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWALERTS& APPFLOWALERTS::GetInstance()
* @brief      Get instance
* @ingroup    APPFLOW
* 
* @return     APPFLOWALERTS& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWALERTS& APPFLOWALERTS::GetInstance()
{
  if(!instance) instance = GEN_NEW APPFLOWALERTS();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWALERTS::DelInstance()
* @brief      Del instance
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWALERTS::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWALERTS::Ini(APPFLOWCFG* cfg, XCHAR* applicationname, int appversion, int appsubversion, int appsubversionerror, int status[APPFLOW_ALERT_TYPE_MAX], DIOSTREAM* streamSMS)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* @param[in]  applicationname : Application name.
* @param[in]  appversion : Application major version.
* @param[in]  appsubversion : Application minor version.
* @param[in]  appsubversionerror : Application error subversion.
* @param[in]  status : Status value to read or update.
* @param[in]  streamSMS : SMS stream to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWALERTS::Ini(APPFLOWCFG* cfg, XCHAR* applicationname, int appversion, int appsubversion, int appsubversionerror, int status[APPFLOW_ALERT_TYPE_MAX], DIOSTREAM* streamSMS)
{
  if(!cfg) return false;

  this->cfg = cfg;

  int  nrecipients[APPFLOW_ALERT_TYPE_MAX];

  for(int c=0; c<APPFLOW_ALERT_TYPE_MAX; c++)
    {
      status[c]      = false;
      nrecipients[c] = 0;
    }

  #ifdef DIO_ALERTS_ACTIVE

  if(!cfg->Alerts_IsActive()) return false;
  if(!GEN_DIOALERTS.Ini())    return false;

  GEN_DIOALERTS.Application_GetID()->Set(applicationname);
  GEN_DIOALERTS.Application_SetVersion(appversion, appsubversion, appsubversionerror);

  #define ALERTS_MAX_RECIPIENTS  100

  XCHAR* recipients[ALERTS_MAX_RECIPIENTS];

  //-----------------------------------------------------------------------------------------
  // SMTP
  if(cfg->Alerts_IsActiveSMTP())
    {
      memset(recipients, 0, sizeof(XCHAR*) * 100);

      for(int c=0; c<cfg->Alerts_GetSMTPRecipients()->GetSize(); c++)
        {
          if(cfg->Alerts_GetSMTPRecipient(c))
            {
              if(cfg->Alerts_GetSMTPRecipient(c)->GetSize())
                {
                  recipients[nrecipients[APPFLOW_ALERT_TYPE_SMTP]] = cfg->Alerts_GetSMTPRecipient(c)->Get();
                  nrecipients[APPFLOW_ALERT_TYPE_SMTP]++;
                }
            }
        }

      if(nrecipients[APPFLOW_ALERT_TYPE_SMTP])
        {
          bool isconfig = GEN_DIOALERTS.Sender_SMTPConfig(cfg->Alerts_GetSMTPURL()->Get() , cfg->Alerts_GetSMTPPort()
                                                                                          , cfg->Alerts_GetSMTPLogin()->Get()
                                                                                          , cfg->Alerts_GetSMTPPassword()->Get()
                                                                                          , cfg->Alerts_GetSMTPSender()->Get()
                                                                                          , nrecipients[APPFLOW_ALERT_TYPE_SMTP]
                                                                                          , recipients[0], recipients[1], recipients[2], recipients[3], recipients[4]
                                                                                          , recipients[5], recipients[6], recipients[7], recipients[8], recipients[9]) ? 1 : 0;

          if(isconfig)  
                 status[APPFLOW_ALERT_TYPE_SMTP] = APPFLOW_ALERT_STATUS_ACTIVE;
            else status[APPFLOW_ALERT_TYPE_SMTP] = APPFLOW_ALERT_STATUS_NOTCONFIG;

        } else status[APPFLOW_ALERT_TYPE_SMTP] =  APPFLOW_ALERT_STATUS_NOTRECIPIENTS;  

    } else status[APPFLOW_ALERT_TYPE_SMTP] = APPFLOW_ALERT_STATUS_NOTACTIVATED; 
  //-----------------------------------------------------------------------------------------


  //-----------------------------------------------------------------------------------------
  // SMS
  if(cfg->Alerts_IsActiveSMS())
    {
      memset(recipients, 0, sizeof(XCHAR*) * ALERTS_MAX_RECIPIENTS);

      for(int c=0; c<cfg->Alerts_GetSMSRecipients()->GetSize(); c++)
        {
          if(cfg->Alerts_GetSMSRecipient(c))
            {
              if(cfg->Alerts_GetSMSRecipient(c)->GetSize())
                {
                  recipients[nrecipients[APPFLOW_ALERT_TYPE_SMS]] = cfg->Alerts_GetSMSRecipient(c)->Get();
                  nrecipients[APPFLOW_ALERT_TYPE_SMS]++;
                }
            }
        }

      if(nrecipients[APPFLOW_ALERT_TYPE_SMS])
        {
          bool isconfig = GEN_DIOALERTS.Sender_SMSConfig(streamSMS , nrecipients[APPFLOW_ALERT_TYPE_SMS]
                                                                   , recipients[0], recipients[1], recipients[2], recipients[3], recipients[4]
                                                                   , recipients[5], recipients[6], recipients[7], recipients[8], recipients[9]) ? 1 : 0;

          if(isconfig)  
                 status[APPFLOW_ALERT_TYPE_SMS] = APPFLOW_ALERT_STATUS_ACTIVE;
            else status[APPFLOW_ALERT_TYPE_SMS] = APPFLOW_ALERT_STATUS_NOTCONFIG;

        } else status[APPFLOW_ALERT_TYPE_SMS] =  APPFLOW_ALERT_STATUS_NOTRECIPIENTS; 

    } else status[APPFLOW_ALERT_TYPE_SMS] = APPFLOW_ALERT_STATUS_NOTACTIVATED; 

  //-----------------------------------------------------------------------------------------


  //-----------------------------------------------------------------------------------------
  // WEB
  if(cfg->Alerts_IsActiveWEB())
    {
      memset(recipients, 0, sizeof(XCHAR*) * ALERTS_MAX_RECIPIENTS);

      for(int c=0; c<cfg->Alerts_GetWEBRecipients()->GetSize(); c++)
        {
          if(cfg->Alerts_GetWEBRecipient(c))
            {
              if(cfg->Alerts_GetWEBRecipient(c)->GetSize())
                {
                  recipients[nrecipients[APPFLOW_ALERT_TYPE_WEB]] = cfg->Alerts_GetWEBRecipient(c)->Get();
                  nrecipients[APPFLOW_ALERT_TYPE_WEB]++;
                }
            }
        }

      if(nrecipients[APPFLOW_ALERT_TYPE_WEB])
        {
          bool isconfig = GEN_DIOALERTS.Sender_WEBConfig(APPFLOW_ALERTS_WEBALERTCMD  , cfg->Alerts_GetWEBIsUseGet()
                                                                                 , nrecipients[APPFLOW_ALERT_TYPE_WEB]
                                                                                 , recipients[0], recipients[1], recipients[2], recipients[3], recipients[4]
                                                                                 , recipients[5], recipients[6], recipients[7], recipients[8], recipients[9]) ? 1 : 0;

          if(isconfig)  
                 status[APPFLOW_ALERT_TYPE_WEB] = APPFLOW_ALERT_STATUS_ACTIVE;
            else status[APPFLOW_ALERT_TYPE_WEB] = APPFLOW_ALERT_STATUS_NOTCONFIG;

        } else status[APPFLOW_ALERT_TYPE_WEB] =  APPFLOW_ALERT_STATUS_NOTRECIPIENTS; 

    } else status[APPFLOW_ALERT_TYPE_WEB] = APPFLOW_ALERT_STATUS_NOTACTIVATED; 

  //-----------------------------------------------------------------------------------------


  //-----------------------------------------------------------------------------------------
  // UDP
  if(cfg->Alerts_IsActiveUDP())
    {
      memset(recipients, 0, sizeof(XCHAR*) * ALERTS_MAX_RECIPIENTS);

      for(int c=0; c<cfg->Alerts_GetUDPRecipients()->GetSize(); c++)
        {
          if(cfg->Alerts_GetUDPRecipient(c))
            {
              if(cfg->Alerts_GetUDPRecipient(c)->GetSize())
                {
                  recipients[nrecipients[APPFLOW_ALERT_TYPE_UDP]] = cfg->Alerts_GetUDPRecipient(c)->Get();
                  nrecipients[APPFLOW_ALERT_TYPE_UDP]++;
                }
            }
        }

      if(nrecipients[APPFLOW_ALERT_TYPE_UDP])
        {
          bool isconfig = GEN_DIOALERTS.Sender_UDPConfig(cfg->Alerts_GetUDPPort() , nrecipients[APPFLOW_ALERT_TYPE_UDP]
                                                                                  , recipients[0], recipients[1], recipients[2], recipients[3], recipients[4]
                                                                                  , recipients[5], recipients[6], recipients[7], recipients[8], recipients[9]) ? 1 : 0;

          if(isconfig)  
                 status[APPFLOW_ALERT_TYPE_UDP] = APPFLOW_ALERT_STATUS_ACTIVE;
            else status[APPFLOW_ALERT_TYPE_UDP] = APPFLOW_ALERT_STATUS_NOTCONFIG;

        } else status[APPFLOW_ALERT_TYPE_UDP] =  APPFLOW_ALERT_STATUS_NOTRECIPIENTS; 

    } else status[APPFLOW_ALERT_TYPE_UDP] = APPFLOW_ALERT_STATUS_NOTACTIVATED; 

  //-----------------------------------------------------------------------------------------

  /*
      stringresult.Empty();

      if(nsmtprecipients) stringresult += (status2)?_L("Email Ok"):_L("Email ERROR!");
      if(nwebrecipients)
        {
          stringresult += _L(", ");
          stringresult += (status3)?_L("WEB Ok"):_L("WEB ERROR!");
        }
      if(nudprecipients)
        {
          stringresult += _L(", ");
          stringresult += (status4)?_L("UDP Ok")   :_L("UDP ERROR!");
        }

      stringresult += _L(".");

    }
*/

 for(XDWORD c=0; c<cfg->Alerts_GetConditions()->GetSize(); c++)
    {
      XDWORD conditionID          = 0;
      int    timelimitforrepeat   = 0; 
      int    everynumberoftimes   = 0;

      if(cfg->Alerts_GetCondition(c, conditionID, timelimitforrepeat, everynumberoftimes))
        {
          GEN_DIOALERTS.AddCondition(conditionID, timelimitforrepeat, everynumberoftimes);
        }   
    } 

  #endif

  return ((status[APPFLOW_ALERT_TYPE_SMTP] ==  APPFLOW_ALERT_STATUS_ACTIVE) || 
          (status[APPFLOW_ALERT_TYPE_SMS]  ==  APPFLOW_ALERT_STATUS_ACTIVE) || 
          (status[APPFLOW_ALERT_TYPE_WEB]  ==  APPFLOW_ALERT_STATUS_ACTIVE) || 
          (status[APPFLOW_ALERT_TYPE_UDP]  ==  APPFLOW_ALERT_STATUS_ACTIVE))?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWALERTS::Send(XDWORD senders, XDWORD condition, DIOALERTLEVEL level, XCHAR* title, XCHAR* message)
* @brief      Send
* @ingroup    APPFLOW
* 
* @param[in]  senders : Sender mask to use.
* @param[in]  condition : Condition mask to evaluate.
* @param[in]  level : Level value.
* @param[in]  title : Title text.
* @param[in]  message : Message text.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWALERTS::Send(XDWORD senders, XDWORD condition, DIOALERTLEVEL level, XCHAR* title, XCHAR* message)
{
  DIOALERT* alert  = GEN_DIOALERTS.CreateAlert(level, title, message);
  int       status = 0;

  if(!alert) return status;

  status = GEN_DIOALERTS.Send((DIOALERTSENDER)senders, condition, alert);

  GEN_DELETE alert;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWALERTS::End()
* @brief      End the object
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWALERTS::End()
{
  GEN_DIOALERTS.End();

  GEN_DIOALERTS.DelInstance();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWALERTS::APPFLOWALERTS()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWALERTS::APPFLOWALERTS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWALERTS::~APPFLOWALERTS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWALERTS::~APPFLOWALERTS()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWALERTS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWALERTS::Clean()
{
  cfg = NULL;
}




#endif
