/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCheckTCPIPConnections.cpp
* 
* @class      DIOCHECKTCPIPCONNECTIONS
* @brief      Data Input/Output Check connections TPC/IP class
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

#include "DIOCheckTCPIPConnections.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XDateTime.h"
#include "XBuffer.h"
#include "XTrace.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOPing.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION_CUT::DIOCHECKTCPIPCONNECTION_CUT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION_CUT::DIOCHECKTCPIPCONNECTION_CUT()
{
  Clean();

  startdatetime=GEN_XFACTORY.CreateDateTime();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION_CUT::~DIOCHECKTCPIPCONNECTION_CUT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION_CUT::~DIOCHECKTCPIPCONNECTION_CUT()
{
  if(startdatetime) GEN_XFACTORY.DeleteDateTime(startdatetime);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* DIOCHECKTCPIPCONNECTION_CUT::GetStartDateTime()
* @brief      Get start date time
* @ingroup    DATAIO
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* DIOCHECKTCPIPCONNECTION_CUT::GetStartDateTime()
{
  return startdatetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOCHECKTCPIPCONNECTION_CUT::GetMeasureNSeconds()
* @brief      Get measure N seconds
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOCHECKTCPIPCONNECTION_CUT::GetMeasureNSeconds()
{
  return nseconds;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION_CUT::SetMeasureNSeconds(XDWORD nseconds)
* @brief      Set measure N seconds
* @ingroup    DATAIO
* 
* @param[in]  nseconds : Nseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION_CUT::SetMeasureNSeconds(XDWORD nseconds)
{
  this->nseconds = nseconds;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION_CUT::GetMeasureTimeString(XSTRING& measuretime, bool large)
* @brief      Get measure time string
* @ingroup    DATAIO
* 
* @param[in]  measuretime : Measuretime value.
* @param[in]  large : Large value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION_CUT::GetMeasureTimeString(XSTRING& measuretime, bool large)
{
  XTIMER xtimer;

  xtimer.Reset();
  xtimer.AddSeconds(GetMeasureNSeconds());

  xtimer.GetMeasureString(measuretime, large);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION_CUT::CopyFrom(DIOCHECKTCPIPCONNECTION_CUT* connectioncut)
* @brief      Copy from
* @ingroup    DATAIO
* 
* @param[in]  connectioncut : Connectioncut pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION_CUT::CopyFrom(DIOCHECKTCPIPCONNECTION_CUT* connectioncut)
{
  if(!connectioncut)   return false;

  return connectioncut->CopyTo(this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION_CUT::CopyTo(DIOCHECKTCPIPCONNECTION_CUT* connectioncut)
* @brief      Copy to
* @ingroup    DATAIO
* 
* @param[in]  connectioncut : Connectioncut pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION_CUT::CopyTo(DIOCHECKTCPIPCONNECTION_CUT* connectioncut)
{
  if(!connectioncut)   return false;

  connectioncut->GetStartDateTime()->CopyFrom(startdatetime);
  connectioncut->SetMeasureNSeconds(nseconds);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION_CUT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION_CUT::Clean()
{
  startdatetime   = NULL;
  nseconds        = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION::DIOCHECKTCPIPCONNECTION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION::DIOCHECKTCPIPCONNECTION()
{
  Clean();

  url = GEN_NEW DIOURL();
  
  timerconnexion=GEN_XFACTORY.CreateTimer(); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION::~DIOCHECKTCPIPCONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION::~DIOCHECKTCPIPCONNECTION()
{
  if(url) GEN_DELETE url;

  if(timerconnexion) GEN_XFACTORY.DeleteTimer(timerconnexion);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTION::GetID()
* @brief      Get ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTION::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOCHECKTCPIPCONNECTION::GetURL()
* @brief      Get URL
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOCHECKTCPIPCONNECTION::GetURL()
{
  return url;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION::IsConnected()
* @brief      Is connected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION::IsConnected()
{
  return isconnected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION::SetIsConnected(bool isconnected)
* @brief      Set is connected
* @ingroup    DATAIO
* 
* @param[in]  isconnected : Isconnected value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION::SetIsConnected(bool isconnected)
{
  this->isconnected = isconnected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTION::GetNChecks()
* @brief      Get N checks
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTION::GetNChecks()
{
  return nchecks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION::SetNChecks(XDWORD nchecks)
* @brief      Set N checks
* @ingroup    DATAIO
* 
* @param[in]  nchecks : Nchecks value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION::SetNChecks(XDWORD nchecks)
{
  this->nchecks = nchecks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTION::IncNChecks()
* @brief      Inc N checks
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTION::IncNChecks()
{
  return ++nchecks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION::Set(XCHAR* url)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION::Set(XCHAR* url)
{
  if(!this->url)  return 0;

  (*this->url) = url;

  this->url->Slash_Delete();

  ID = CreateID();

  return ID?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTION::GetElapsedTime()
* @brief      Get elapsed time
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTION::GetElapsedTime()
{
  return elapsedtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION::SetElapsedTime(XDWORD elapsedtime)
* @brief      Set elapsed time
* @ingroup    DATAIO
* 
* @param[in]  elapsedtime : Elapsedtime value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION::SetElapsedTime(XDWORD elapsedtime)
{
  this->elapsedtime = elapsedtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTION::ResetTimeConnexionStatus()
* @brief      Reset time connexion status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTION::ResetTimeConnexionStatus()
{
  if(!timerconnexion) return false;
  timerconnexion->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOCHECKTCPIPCONNECTION::GetTimeConnexionStatus()
* @brief      Get time connexion status
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOCHECKTCPIPCONNECTION::GetTimeConnexionStatus()
{
  if(!timerconnexion) return 0;
  return timerconnexion->GetMeasureSeconds();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTION::CreateID()
* @brief      Create ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTION::CreateID()
{
  HASHCRC32 hashcrc32;
  XDWORD    _ID = 0;

  XBUFFER charstr;
  
  url->ConvertToASCII(charstr);
  if(hashcrc32.Do((XBYTE*)charstr.Get(), url->GetSize())) _ID += hashcrc32.GetResultCRC32();
  
  return _ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTION::Clean()
{
  ID                    = 0;
  url                   = NULL;
  isconnected           = false;
  nchecks               = 0;
  elapsedtime           = 0;
  timerconnexion        = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTIONS::DIOCHECKTCPIPCONNECTIONS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTIONS::DIOCHECKTCPIPCONNECTIONS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTIONS::~DIOCHECKTCPIPCONNECTIONS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTIONS::~DIOCHECKTCPIPCONNECTIONS()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Ini(int timeconnectionchecks, bool validsomeisconnected, bool dispersionmode)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  timeconnectionchecks : Timeconnectionchecks value.
* @param[in]  validsomeisconnected : Validsomeisconnected value.
* @param[in]  dispersionmode : Dispersionmode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Ini(int timeconnectionchecks, bool validsomeisconnected, bool dispersionmode)
{
  Setup(timeconnectionchecks, validsomeisconnected, dispersionmode);

  xtimerfortimeconnectionchecks=GEN_XFACTORY.CreateTimer();
  if(!xtimerfortimeconnectionchecks) return false;
      
  xmutexconnections=GEN_XFACTORY.Create_Mutex();
  if(!xmutexconnections) return false;

  threadcheckconnections = CREATEXTHREAD(XTHREADGROUPID_DIOCHECKTCPIPCONNECTIONS, _L("DIOCHECKTCPIPCONNECTIONS::Ini"), ThreadCheckConnections, (void*)this);
  if(!threadcheckconnections)  return false;

  SetIsCheckTimeConnections(true);

  return threadcheckconnections->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Run(bool activate)
* @brief      Run operation
* @ingroup    DATAIO
* 
* @param[in]  activate : Activate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Run(bool activate)
{
  if(!threadcheckconnections) return false;
  return threadcheckconnections->Run(activate);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOCHECKTCPIPCONNECTIONS::GetTimeConnectionChecks()
* @brief      Get time connection checks
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOCHECKTCPIPCONNECTIONS::GetTimeConnectionChecks()
{
  return timeconnectionchecks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::IsCheckTimeConnections()
* @brief      Is check time connections
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::IsCheckTimeConnections()
{
  return ischecktimeconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTIONS::SetIsCheckTimeConnections(bool ischecktimeconnections)
* @brief      Set is check time connections
* @ingroup    DATAIO
* 
* @param[in]  ischecktimeconnections : Ischecktimeconnections value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTIONS::SetIsCheckTimeConnections(bool ischecktimeconnections)
{
  this->ischecktimeconnections = ischecktimeconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTIONS::Setup(int timeconnectionchecks, bool validsomeisconnected, bool dispersionmode)
* @brief      Setup
* @ingroup    DATAIO
* 
* @param[in]  timeconnectionchecks : Timeconnectionchecks value.
* @param[in]  validsomeisconnected : Validsomeisconnected value.
* @param[in]  dispersionmode : Dispersionmode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTIONS::Setup(int timeconnectionchecks, bool validsomeisconnected, bool dispersionmode)
{
  this->timeconnectionchecks  = timeconnectionchecks;
  this->validsomeisconnected  = validsomeisconnected;
  this->dispersionmode        = dispersionmode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(XCHAR* url, XDWORD& ID)
* @brief      Connection add
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(XCHAR* url, XDWORD& ID)
{
  DIOCHECKTCPIPCONNECTION* connection = GEN_NEW DIOCHECKTCPIPCONNECTION();
  if(!connection) return false;

  if(connection->Set(url))
    {
      ID = connection->GetID();

      if(ID)
        {
          if(xmutexconnections) xmutexconnections->Lock();

          connections.Add(connection);

          if(xmutexconnections) xmutexconnections->UnLock();

          return true;
        }
    }

  GEN_DELETE connection;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(XSTRING& url, XDWORD& ID)
* @brief      Connection add
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(XSTRING& url, XDWORD& ID)
{
  return Connection_Add(url.Get(), ID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(DIOURL& url, XDWORD& ID)
* @brief      Connection add
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connection_Add(DIOURL& url, XDWORD& ID)
{
  return Connection_Add(url.Get(), ID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION* DIOCHECKTCPIPCONNECTIONS::Connection_GetByID(XDWORD ID)
* @brief      Connection_GetByID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     DIOCHECKTCPIPCONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION* DIOCHECKTCPIPCONNECTIONS::Connection_GetByID(XDWORD ID)
{
  if(xmutexconnections) 
    {
      xmutexconnections->Lock();
    }

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          if(cc->GetID() == ID)
            {
              if(xmutexconnections) xmutexconnections->UnLock();
              return cc;
            }
        }
    }

  if(xmutexconnections) 
    {
      xmutexconnections->UnLock();
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connection_IsActive(XDWORD ID, bool& isactive)
* @brief      Connection is active
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* @param[in]  isactive : Isactive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connection_IsActive(XDWORD ID, bool& isactive)
{
  isactive = false;

  if(xmutexconnections) xmutexconnections->Lock();

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          if(cc->GetID() == ID)
            {
              isactive = cc->IsConnected();
              if(xmutexconnections) xmutexconnections->UnLock();

              return true;
            }
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connection_DeleteByID(XDWORD ID)
* @brief      Connection GEN_DELETE by ID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connection_DeleteByID(XDWORD ID)
{
  DIOCHECKTCPIPCONNECTION* connection = Connection_GetByID(ID);
  if(connection)
    {
      if(xmutexconnections) xmutexconnections->Lock();

      connections.Delete(connection);
      GEN_DELETE connection;

      if(xmutexconnections) xmutexconnections->UnLock();

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOCHECKTCPIPCONNECTIONS::Connections_GetMutex()
* @brief      Connections get mutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOCHECKTCPIPCONNECTIONS::Connections_GetMutex()
{
  return xmutexconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_IsAllChecked()
* @brief      Connections is all checked
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_IsAllChecked()
{
  bool ischeckallconnections = true;

  if(xmutexconnections) xmutexconnections->Lock();

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          if(!cc->GetNChecks()) ischeckallconnections = false;
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return ischeckallconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_IsSomeChecked()
* @brief      Connections is some checked
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_IsSomeChecked()
{
  bool ischecksomeconnections = false;

  if(xmutexconnections) xmutexconnections->Lock();

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          if(cc->GetNChecks())
            {
              ischecksomeconnections = true;
              break;
            }
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return ischecksomeconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_WaitToAllChecked(int timeout)
* @brief      Connections wait to all checked
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_WaitToAllChecked(int timeout)
{
  if(!connections.GetSize()) return false;

  XTIMER* xtimer=GEN_XFACTORY.CreateTimer();
  if(!xtimer) return false;

  bool status = true;

  xtimer->Reset();

  SetIsCheckTimeConnections(false);

  while(1)
    {
      if(timeout != XTIMER_INFINITE)
        {
          if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      if(Connections_IsAllChecked()) break;

      GEN_XSLEEP.MilliSeconds(100);
    }

  GEN_XFACTORY.DeleteTimer(xtimer);

  SetIsCheckTimeConnections(true);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_WaitToSomeIsChecked(int timeout)
* @brief      Connections wait to some is checked
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_WaitToSomeIsChecked(int timeout)
{
  if(!connections.GetSize()) return false;

  XTIMER* xtimer=GEN_XFACTORY.CreateTimer();
  if(!xtimer) return false;
    
  bool status = true;

  xtimer->Reset();

  SetIsCheckTimeConnections(false);

  while(1)
    {
      if(timeout != XTIMER_INFINITE)
        {
          if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      if(Connections_IsSomeChecked()) break;

      GEN_XSLEEP.MilliSeconds(100);
    }

  GEN_XFACTORY.DeleteTimer(xtimer);

  SetIsCheckTimeConnections(true);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_SomeIsConnected()
* @brief      Connections some is connected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_SomeIsConnected()
{
  bool someisactive = false;

  if(xmutexconnections) xmutexconnections->Lock();

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          someisactive = cc->IsConnected();
          if(someisactive) break;
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return someisactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKTCPIPCONNECTIONS::Connections_GetLatency()
* @brief      Connections get latency
* @ingroup    DATAIO
* 
* @return     XDWORD : Elapsed time (in milliseconds) of the last connection found connected; 0 if none is connected.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKTCPIPCONNECTIONS::Connections_GetLatency()
{
  XDWORD latency = 0;

  if(xmutexconnections) xmutexconnections->Lock();

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCHECKTCPIPCONNECTION* cc = connections.Get(c);
      if(cc)
        {
          if(cc->IsConnected())
            {
              latency = cc->GetElapsedTime();
              break;
            }
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::Connections_DeleteAll()
* @brief      Connections GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::Connections_DeleteAll()
{
  if(xmutexconnections) xmutexconnections->Lock();

  connections.DeleteContents();
  connections.DeleteAll();

  if(xmutexconnections) xmutexconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::IsActivedExit()
* @brief      Is actived exit
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::IsActivedExit()
{
  return activedexit;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::SetActivedExit(bool actived)
* @brief      Set actived exit
* @ingroup    DATAIO
* 
* @param[in]  actived : Actived value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::SetActivedExit(bool actived)
{
  activedexit = actived;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKTCPIPCONNECTIONS::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKTCPIPCONNECTIONS::End()
{
  SetActivedExit(true); 

  if(threadcheckconnections)
    {
      threadcheckconnections->End();

      DELETEXTHREAD(XTHREADGROUPID_DIOCHECKTCPIPCONNECTIONS, threadcheckconnections);
      threadcheckconnections = NULL;
    }

  Connections_DeleteAll();

  if(xmutexconnections)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexconnections);
      xmutexconnections = NULL;
    }
 
  if(xtimerfortimeconnectionchecks)
    {
      GEN_XFACTORY.DeleteTimer(xtimerfortimeconnectionchecks);
      xtimerfortimeconnectionchecks = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTIONS::ThreadCheckConnections(void* param)
* @brief      Thread check connections
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTIONS::ThreadCheckConnections(void* param)
{
  DIOCHECKTCPIPCONNECTIONS* checkconnections = (DIOCHECKTCPIPCONNECTIONS*)param;
  if(!checkconnections) return;

  if(!checkconnections->xmutexconnections)                return;
  if(!checkconnections->xtimerfortimeconnectionchecks)    return;

  static int index = 0;

  int nconnections = checkconnections->connections.GetSize();
  if(!nconnections) return;

  if(checkconnections->activedexit)
    {
      return;
    }

  if(checkconnections->dispersionmode)
    {
      DIOCHECKTCPIPCONNECTION* checkconnection = checkconnections->connections.Get(index);
      if(!checkconnection)
        {
          index = 0;
          checkconnection = checkconnections->connections.Get(index);
        }

      if(checkconnection)
        {
          checkconnections->xmutexconnections->Lock();

          GEN_DIOPING.Set(checkconnection->GetURL()->Get());

          bool connexionstatus = GEN_DIOPING.Do(DIOCHECKTCPIPCONNECTIONS_DEFAULTNCHECKSFOREVERYCONNECTION, DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMERCONNECTIONCHECK, checkconnections->validsomeisconnected);
          if(connexionstatus != checkconnection->IsConnected()) checkconnection->ResetTimeConnexionStatus();

          checkconnection->SetIsConnected(connexionstatus);
          checkconnection->IncNChecks();

          if(checkconnection->IsConnected())
                 checkconnection->SetElapsedTime(GEN_DIOPING.CalculateMeanTime());
            else checkconnection->SetElapsedTime(0);

          index++;

          checkconnections->xmutexconnections->UnLock();       
        }
    }
   else
    {
      checkconnections->xmutexconnections->Lock();

      for(int c=0; c<nconnections; c++)
        { 
          if(checkconnections->activedexit)
            {
              break; 
            }

          if(!checkconnections->threadcheckconnections->IsRunning()) 
            {
              break;             
            }

          DIOCHECKTCPIPCONNECTION* checkconnection = checkconnections->connections.Get(c);
          if(checkconnection)
            {
              GEN_DIOPING.Set(checkconnection->GetURL()->Get());

              bool connexionstatus = GEN_DIOPING.Do(DIOCHECKTCPIPCONNECTIONS_DEFAULTNCHECKSFOREVERYCONNECTION, DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMERCONNECTIONCHECK, checkconnections->validsomeisconnected);
              if(connexionstatus != checkconnection->IsConnected()) 
                {
                  checkconnection->ResetTimeConnexionStatus();
                }

              checkconnection->SetIsConnected(connexionstatus);
              checkconnection->IncNChecks();

              if(checkconnection->IsConnected())
                      checkconnection->SetElapsedTime(GEN_DIOPING.CalculateMeanTime());
                else checkconnection->SetElapsedTime(0);

              if((checkconnections->validsomeisconnected) && (checkconnection->IsConnected())) 
                {
                  break;
                }              
            }
        }

      checkconnections->xmutexconnections->UnLock();      
    }

  if(checkconnections->IsCheckTimeConnections())
    {
      for(int e=0; e<checkconnections->timeconnectionchecks; e++)
        {
          if(checkconnections->activedexit)
            {
              break; 
            }

          if(!checkconnections->threadcheckconnections->IsRunning()) 
            {
              break;
            }

          // Wait One second
          for(int d=0; d<10; d++)
            {
              if(checkconnections->activedexit)
                {
                  break; 
                }

              if(!checkconnections->threadcheckconnections->IsRunning()) 
                {
                  break;
                }

              GEN_XSLEEP.MilliSeconds(100);
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKTCPIPCONNECTIONS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKTCPIPCONNECTIONS::Clean()
{
  timeconnectionchecks                = DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMECONNECTIONCHECKS;
  validsomeisconnected                = false;
  dispersionmode                      = false;

  xtimerfortimeconnectionchecks       = NULL;
  ischecktimeconnections              = false;

  xmutexconnections                   = NULL;
  threadcheckconnections              = NULL;

  activedexit                         = false;
}








