/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOAlertsUDPServer.cpp
* 
* @class      DIOALERTSUDPSERVER
* @brief      Data Input/Output Alerts UDP servers class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOAlertsUDPServer.h"

#include "XThreadCollected.h"

#include "HashCRC32.h"

#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDPAcknowledge.h"
#include "DIOAlerts.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOALERTSUDPSERVER::DIOALERTSUDPSERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOALERTSUDPSERVER::DIOALERTSUDPSERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOALERTSUDPSERVER::~DIOALERTSUDPSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOALERTSUDPSERVER::~DIOALERTSUDPSERVER()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::Ini(XCHAR* URL, int port)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::Ini(XCHAR* URL, int port)
{
  bool status = false;

  UDPdiostreamcfg = GEN_NEW DIOSTREAMUDPCONFIG();
  if(!UDPdiostreamcfg)  return false;

  UDPdiostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
  UDPdiostreamcfg->GetRemoteURL()->Set(URL);
  UDPdiostreamcfg->SetRemotePort(port);
  UDPdiostreamcfg->SetIsUsedDatagrams(true);

  UDPdiostream = GEN_NEW DIOSTREAMUDPACKNOWLEDGE(UDPdiostreamcfg);
  if(UDPdiostream)
    {
      status = UDPdiostream->Open();
      if(status)
        {
          xmutexalert=GEN_XFACTORY.Create_Mutex();
          if(xmutexalert)
            {
              threadread = CREATEXTHREAD(XTHREADGROUPID_DIOALERTS, _L("DIOALERTSUDPSERVER::Ini"), ThreadReadFunction, (void*)this);
              if(threadread)  status = threadread->Ini();
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::End()
{
  if(threadread)
    {
      threadread->End();

      DELETEXTHREAD(XTHREADGROUPID_DIOALERTS, threadread);
      threadread = NULL;
    }

  DeleteAllAlerts();

  if(UDPdiostream)
    {
      UDPdiostream->Close();
      GEN_DELETE UDPdiostream;
      UDPdiostream = NULL;
    }

  if(UDPdiostreamcfg)
    {
      GEN_DELETE UDPdiostreamcfg;
      UDPdiostreamcfg = NULL;
    }

  if(xmutexalert)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexalert);
      xmutexalert = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOALERTSUDPSERVER::GetNAlerts()
* @brief      Get N alerts
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOALERTSUDPSERVER::GetNAlerts()
{
  if(xmutexalert) xmutexalert->Lock();

  int nalerts = alerts.GetSize();

  if(xmutexalert) xmutexalert->UnLock();

  return nalerts;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOALERT* DIOALERTSUDPSERVER::GetAlertByIndex(int index)
* @brief      Get alert by index
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOALERT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOALERT* DIOALERTSUDPSERVER::GetAlertByIndex(int index)
{
  if(xmutexalert) xmutexalert->Lock();

  DIOALERT* alert = alerts.Get(index);

  if(xmutexalert) xmutexalert->UnLock();

  return alert;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOALERT* DIOALERTSUDPSERVER::GetAlertByID(XDWORD ID)
* @brief      Get alert by ID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     DIOALERT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOALERT* DIOALERTSUDPSERVER::GetAlertByID(XDWORD ID)
{
  DIOALERT* alert = NULL;

  if(xmutexalert) xmutexalert->Lock();

  for(int c=0; c<(int)alerts.GetSize(); c++)
    {
      alert  = alerts.Get(c);
      if(alert)
        {
          if(alert->GetID() == ID)  break;
        }
    }

  if(xmutexalert) xmutexalert->UnLock();

  return alert;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::DeleteAlertByIndex(int index)
* @brief      Delete alert by index
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAlertByIndex(int index)
{
  DIOALERT* alert = alerts.Get(index);
  if(alert)
    {
      if(xmutexalert) xmutexalert->Lock();

      alerts.Delete(alert);
      GEN_DELETE alert;

      if(xmutexalert) xmutexalert->UnLock();

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::DeleteAlertByID(XDWORD ID)
* @brief      Delete alert by ID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAlertByID(XDWORD ID)
{
  DIOALERT* alert  = NULL;
  bool      status = false;

  if(xmutexalert) xmutexalert->Lock();

  for(int c=0; c<(int)alerts.GetSize(); c++)
    {
      alert  = alerts.Get(c);
      if(alert)
        {
          if(alert->GetID() == ID)
            {
              alerts.Delete(alert);
              GEN_DELETE alert;

              status = true;
            }
        }
    }

  if(xmutexalert) xmutexalert->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::DeleteAllAlerts()
* @brief      Delete all alerts
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAllAlerts()
{
  if(alerts.IsEmpty()) return false;

  if(xmutexalert) xmutexalert->Lock();

  alerts.DeleteContents();
  alerts.DeleteAll();

  if(xmutexalert) xmutexalert->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOALERTSUDPSERVER::ReceivedEvents()
* @brief      Received events
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::ReceivedEvents()
{
  if(!UDPdiostream) return false;

  XBUFFER  data;
  XSTRING  address;
  XWORD    port;
  bool     status = false;

  if(UDPdiostream->ReadDatagram(address, port, data))
    {
      HASHCRC32   hashcrc32;
      XDWORD      CRC32;
      DIOALERT*   alert  = NULL;

      if(data.GetSize())
        {
          alert = GEN_NEW DIOALERT();
          if(alert)
            {
              XSTRING string;
              XDWORD  dataxdword;
              XDWORD  sizestring;
              XDWORD  applicationversion;
              XDWORD  applicationsubversion;
              XDWORD  applicationsubversionerr;

              data.Extract(CRC32, (data.GetSize()-sizeof(XDWORD)));

              hashcrc32.ResetResult();
              hashcrc32.Do(data);

              if(CRC32 == hashcrc32.GetResultCRC32())
                {
                  // Date time
                  data.Extract(dataxdword);   alert->GetDateTime()->SetDay(dataxdword);                                           // Day
                  data.Extract(dataxdword);   alert->GetDateTime()->SetMonth(dataxdword);                                         // Month
                  data.Extract(dataxdword);   alert->GetDateTime()->SetYear(dataxdword);                                          // Year
                  data.Extract(dataxdword);   alert->GetDateTime()->SetHours(dataxdword);                                         // Hours
                  data.Extract(dataxdword);   alert->GetDateTime()->SetMinutes(dataxdword);                                       // Minutes
                  data.Extract(dataxdword);   alert->GetDateTime()->SetSeconds(dataxdword);                                       // Seconds

                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->Application_GetID()->Set(string);       // Aplication String
                  data.Extract(applicationversion);
                  data.Extract(applicationsubversion);
                  data.Extract(applicationsubversionerr);

                  alert->Application_SetVersion(applicationversion, applicationsubversion, applicationsubversionerr);

                  data.Extract(dataxdword);                                         alert->SetID(dataxdword);                     // ID
                  data.Extract(dataxdword);                                         alert->SetType(dataxdword);                   // Type
                  data.Extract(dataxdword);                                         alert->SetLevel((DIOALERTLEVEL)dataxdword);   // Level
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->GetOrigin()->Set(string);              // Origin
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->GetTitle()->Set(string);               // Title
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->Get_Message()->Set(string);            // Message

                  status = alerts.Add(alert);
                }

              if(!status) GEN_DELETE alert;
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOALERTSUDPSERVER::ThreadReadFunction(void* param)
* @brief      Thread read function
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOALERTSUDPSERVER::ThreadReadFunction(void* param)
{
  DIOALERTSUDPSERVER* alertUDPserver = ( DIOALERTSUDPSERVER*)param;
  if(!alertUDPserver) return;

  alertUDPserver->ReceivedEvents();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOALERTSUDPSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOALERTSUDPSERVER::Clean()
{
  UDPdiostreamcfg     = NULL;
  UDPdiostream        = NULL;

  xmutexalert         = NULL;

  threadread          = NULL;
}



