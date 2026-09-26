/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XThreadCollected.cpp
* 
* @class      XTHREADCOLLECTED
* @brief      eXtended Utils Thread Collected class
* @ingroup    XUTILS
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



#ifdef XTHREADCOLLECTED_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XThreadCollected.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XTHREADSCOLLECTEDMANAGER* XTHREADSCOLLECTEDMANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED::XTHREADCOLLECTED()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED::XTHREADCOLLECTED()
{
  Clean();

  xtimerout=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED::XTHREADCOLLECTED(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED::XTHREADCOLLECTED(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  Clean();

  directxthread=GEN_XFACTORY.CreateThread(groupID, ID, function, param);
  if(directxthread)
    {
      this->ID       = ID;
      this->function = function;
      this->param    = param;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED::~XTHREADCOLLECTED()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED::~XTHREADCOLLECTED()
{
  if(xtimerout)     GEN_XFACTORY.DeleteTimer(xtimerout);
  if(directxthread) GEN_XFACTORY.DeleteThread(groupID, directxthread);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::Ini(bool run)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  run : Run value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::Ini(bool run)
{
  if(directxthread) 
    {
      return directxthread->Ini(run);
    }

  if(run)
    {
      if(!Run(true)) 
        {
          return false;
        }
    }
   else
    {
      status = XTHREADSTATUS_STOP;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::Wait(int milliseconds)
* @brief      Wait
* @ingroup    XUTILS
* 
* @param[in]  milliseconds : Milliseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::Wait(int milliseconds)
{
  if(directxthread)
    {
      directxthread->Wait(milliseconds);
      return;
    }

  GEN_XSLEEP.MilliSeconds(milliseconds);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::End()
{
  if(directxthread) 
    {
      return directxthread->End();
    }

  if(status == XTHREADSTATUS_END) 
    {
      return false;
    }
  
 
  status = XTHREADSTATUS_EXIT;

  WaitToEnd();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::IsRunning()
* @brief      Is running
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::IsRunning()
{
  if(directxthread) return directxthread->IsRunning();

  if(status == XTHREADSTATUS_RUN) return true;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::Run(bool activate)
* @brief      Run operation
* @ingroup    XUTILS
* 
* @param[in]  activate : Activate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::Run(bool activate)
{
  if(directxthread) return directxthread->Run(activate);

  if(activate)
    {
      if(IsRunning()) return false;
      status = XTHREADSTATUS_RUN;
    }
    else
    {
      if(!IsRunning()) return false;
      status = XTHREADSTATUS_STOP;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::Exit()
* @brief      Exit
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::Exit()
{
  if(directxthread) return directxthread->Exit();

  status = XTHREADSTATUS_EXIT;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADGROUPID XTHREADCOLLECTED::GetGroupID()
* @brief      Get group ID
* @ingroup    XUTILS
* 
* @return     XTHREADGROUPID : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADGROUPID XTHREADCOLLECTED::GetGroupID()
{
  return groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetGroupID(XTHREADGROUPID groupID)
* @brief      Set group ID
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetGroupID(XTHREADGROUPID groupID)
{
  this->groupID = groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XTHREADCOLLECTED::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTHREADCOLLECTED::GetID()
{
  if(directxthread)
    {
      return directxthread->GetID();
    }
    else
    {
      return &ID;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADFUNCTION XTHREADCOLLECTED::GetFunction()
* @brief      Get function
* @ingroup    XUTILS
* 
* @return     XTHREADFUNCTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADFUNCTION  XTHREADCOLLECTED::GetFunction()
{
  if(directxthread)
    {
      return directxthread->GetFunction();
    }
    else
    {
      return function;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetFunction(XTHREADFUNCTION function)
* @brief      Set function
* @ingroup    XUTILS
* 
* @param[in]  function : Function value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetFunction(XTHREADFUNCTION function)
{
  this->function = function;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XTHREADCOLLECTED::GetParam()
* @brief      Get param
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XTHREADCOLLECTED::GetParam()
{
  if(directxthread) return directxthread->GetParam();
  return param;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetParam(void* param)
* @brief      Set param
* @ingroup    XUTILS
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetParam(void* param)
{
  this->param = param;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTHREADCOLLECTED::GetWaitYield()
* @brief      Get wait yield
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTHREADCOLLECTED::GetWaitYield()
{
  if(directxthread) return directxthread->GetWaitYield();
  return waityield;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetWaitYield(XDWORD waityield)
* @brief      Set wait yield
* @ingroup    XUTILS
* 
* @param[in]  waityield : Waityield value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetWaitYield(XDWORD waityield)
{
  if(directxthread) return directxthread->SetWaitYield(waityield);
  this->waityield = waityield;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADPRIORITY XTHREADCOLLECTED::GetPriority()
* @brief      Get priority
* @ingroup    XUTILS
* 
* @return     XTHREADPRIORITY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADPRIORITY XTHREADCOLLECTED::GetPriority()
{
  if(directxthread) return directxthread->GetPriority();
  return priority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetPriority(XTHREADPRIORITY priority)
* @brief      Set priority
* @ingroup    XUTILS
* 
* @param[in]  priority : Priority value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetPriority(XTHREADPRIORITY priority)
{
  if(directxthread) return directxthread->SetPriority(priority);
  this->priority = priority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTHREADCOLLECTED::GetStackSize()
* @brief      Get stack size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTHREADCOLLECTED::GetStackSize()
{
  return stacksize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetStackSize(XDWORD stacksize)
* @brief      Set stack size
* @ingroup    XUTILS
* 
* @param[in]  stacksize : Stacksize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetStackSize(XDWORD stacksize)
{
  this->stacksize = stacksize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::IsInFunction()
* @brief      Is in function
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::IsInFunction()
{
  if(directxthread) return directxthread->IsInFunction();
  return isinfunction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetIsInFunction(bool isinfunction)
* @brief      Set is in function
* @ingroup    XUTILS
* 
* @param[in]  isinfunction : Isinfunction value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetIsInFunction(bool isinfunction)
{
  this->isinfunction = isinfunction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSTATUS XTHREADCOLLECTED::GetStatus()
* @brief      Get status
* @ingroup    XUTILS
* 
* @return     XTHREADSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSTATUS XTHREADCOLLECTED::GetStatus()
{
  if(directxthread) return directxthread->GetStatusFunc();
  //if(status == XTHREADSTATUS_EXIT) return XTHREADSTATUS_UNKNOWN; // #Imanol

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::SetStatus(XTHREADSTATUS status)
* @brief      Set status
* @ingroup    XUTILS
* 
* @param[in]  status : Status value to read or update.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::SetStatus(XTHREADSTATUS status)
{
  this->status = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADCOLLECTED::WaitToEnd()
* @brief      Wait to end
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADCOLLECTED::WaitToEnd()
{
  while(GetStatus() != XTHREADSTATUS_END)
    {
      Wait();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADCOLLECTED::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADCOLLECTED::Clean()
{
  groupID         = XTHREADGROUPID_UNKNOWN;
  ID.Empty();
  function        = NULL;
  param           = NULL;

  waityield       = XTHREAD_DEFAULTWAITYIELD;

  priority        = XTHREADPRIORITY_UNKNOWN;
  stacksize       = 0;

  isinfunction    = false;
  status          = XTHREADSTATUS_NONE;

  xtimerout       = NULL;

  directxthread   = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTED::XTHREADSCOLLECTED(XTHREADGROUPID groupID)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTED::XTHREADSCOLLECTED(XTHREADGROUPID groupID)
{
  Clean();

  this->groupID = groupID;

  xthreadsvectormutex=GEN_XFACTORY.Create_Mutex();

  XSTRING stringID;

  GetGroupThreadIDString(stringID);

  if(xthreadsvectormutex)
    {
      xthread=GEN_XFACTORY.CreateThread(groupID, stringID.Get(), ThreadRunFunction, (void*)this);
      if(xthread) xthread->Ini();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTED::XTHREADSCOLLECTED()
* @brief      XTHREADSCOLLECTED::XTHREADSCOLLECTED
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTED::XTHREADSCOLLECTED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTED::~XTHREADSCOLLECTED()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTED::~XTHREADSCOLLECTED()
{
  ExitAll();

  if(xthread)
    {
      xthread->End();
      GEN_XFACTORY.DeleteThread(groupID, xthread);
    }

  DeleteAll();

  if(xthreadsvectormutex) GEN_XFACTORY.Delete_Mutex(xthreadsvectormutex);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADGROUPID XTHREADSCOLLECTED::GetGroupID()
* @brief      Get group ID
* @ingroup    XUTILS
* 
* @return     XTHREADGROUPID : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADGROUPID XTHREADSCOLLECTED::GetGroupID()
{
  return groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADSCOLLECTED::SetGroupID(XTHREADGROUPID groupID)
* @brief      Set group ID
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADSCOLLECTED::SetGroupID(XTHREADGROUPID groupID)
{
  this->groupID = groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED* XTHREADSCOLLECTED::Create(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Create resource
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  param : Param pointer to use.
* 
* @return     XTHREADCOLLECTED* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED* XTHREADSCOLLECTED::Create(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  XTHREADCOLLECTED* xthreadcollected = NULL;

  if(xthread)
    {
      if(!xthreadsvectormutex) return NULL;

      xthreadsvectormutex->Lock();

      xthreadcollected = GEN_NEW XTHREADCOLLECTED();
      if(xthreadcollected)
        {
          xthreadcollected->SetGroupID(groupID);
          xthreadcollected->GetID()->Set(ID);
          xthreadcollected->SetFunction(function);
          xthreadcollected->SetParam(param);

          xthreadsvector.Add(xthreadcollected);
        }

      /*
      #ifdef XTRACE_ACTIVE
      XSTRING groupID;
      GetGroupThreadIDString(groupID);
      XTRACE_PRINTCOLOR(XDEBUG_COLORINFO, __L("Create THREAD COLLECTED (%02d) grupo %s: %s"), xthreadsvector.GetSize(), groupID.Get(), ID);
      #endif
      */

      xthreadsvectormutex->UnLock();
    }
    else
    {
      xthreadcollected = GEN_NEW XTHREADCOLLECTED(groupID, ID, function, param);
      if(xthreadcollected)
        {

        }
    }

  return xthreadcollected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTED::Delete(XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
* @brief      Delete resource
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  xthreadcollected : Xthreadcollected pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTED::Delete(XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
{
  bool status = false;

  if(xthread)
    {
      if(!xthreadsvectormutex) return false;

      xthreadsvectormutex->Lock();

      XDWORD index = 0;

      do{ XTHREADCOLLECTED* _xthreadcollected = xthreadsvector.Get(index);
          if(xthreadcollected == _xthreadcollected)
            {
              /*
              #ifdef XTRACE_ACTIVE
              XSTRING groupID;
              GetGroupThreadIDString(groupID);
              XTRACE_PRINTCOLOR(XDEBUG_COLORINFO, __L("Delete THREAD COLLECTED (%02d) grupo %s: %s"), xthreadsvector.GetSize(), groupID.Get(), xthreadcollected->GetID()->Get());
              #endif
              */

              xthreadcollected->Exit();
              xthreadsvector.Delete(xthreadcollected);
              GEN_DELETE xthreadcollected;

              status = true;

              break;
            }

          index++;

        } while(index < xthreadsvector.GetSize());

      xthreadsvectormutex->UnLock();
    }
    else
    {
      GEN_DELETE xthreadcollected;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTED::ExitAll()
* @brief      Exit all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTED::ExitAll()
{
  if(!xthreadsvectormutex) return false;

  xthreadsvectormutex->Lock();

  for(XDWORD c=0; c<xthreadsvector.GetSize(); c++)
    {
      XTHREADCOLLECTED* xthreadcollected  = xthreadsvector.Get(c);
      if(xthreadcollected) xthreadcollected->Exit();
    }

  xthreadsvectormutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTED::DeleteAll()
* @brief      Delete all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTED::DeleteAll()
{
  if(!xthreadsvectormutex) return false;

  xthreadsvectormutex->Lock();

  xthreadsvector.DeleteContents();
  xthreadsvector.DeleteAll();

  xthreadsvectormutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADSCOLLECTED::ThreadRunFunction(void* param)
* @brief      Thread run function
* @ingroup    XUTILS
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADSCOLLECTED::ThreadRunFunction(void* param)
{
  XTHREADSCOLLECTED* xthreadscollected = (XTHREADSCOLLECTED*)param;
  if(!xthreadscollected) return;

  if(!xthreadscollected->xthreadsvectormutex) return;

  xthreadscollected->xthreadsvectormutex->Lock();

  for(XDWORD c=0; c<xthreadscollected->xthreadsvector.GetSize(); c++)
    {
      XTHREADCOLLECTED* xthreadcollected  = xthreadscollected->xthreadsvector.Get(c);
      if(xthreadcollected)
        {
          switch(xthreadcollected->GetStatus())
            {
              case XTHREADSTATUS_RUN    : xthreadcollected->SetIsInFunction(true);
                                          xthreadcollected->GetFunction()(xthreadcollected->GetParam());
                                          xthreadcollected->SetIsInFunction(false);

                                          xthreadscollected->xthread->Wait();
                                          break;

              case XTHREADSTATUS_EXIT   : xthreadcollected->SetStatus(XTHREADSTATUS_END);
                                          break;

                           default      : break; 
            }
        }
    }

  xthreadscollected->xthreadsvectormutex->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTED::GetGroupThreadIDString(XSTRING& stringID)
* @brief      Get group thread ID string
* @ingroup    XUTILS
* 
* @param[in]  stringID : String ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTED::GetGroupThreadIDString(XSTRING& stringID)
{
  stringID = __L("TGRP_");

  switch(groupID)
    {
      case XTHREADGROUPID_UNKNOWN                       : stringID.Add(__L("UNKNOWN"));                         break;
      case XTHREADGROUPID_SCHEDULER                     : stringID.Add(__L("SCHEDULE"));                        break;
      case XTHREADGROUPID_SCRIPT                        : stringID.Add(__L("SCRIPT"));                          break;
      case XTHREADGROUPID_DIOSTREAM                     : stringID.Add(__L("DIOSTREAM"));                       break;
      case XTHREADGROUPID_DIOSTREAMUART                 : stringID.Add(__L("DIOSTREAMUART"));                   break;
      case XTHREADGROUPID_DIOSTREAMUDP                  : stringID.Add(__L("DIOSTREAMUDP"));                    break;
      case XTHREADGROUPID_DIOSTREAMTCPIP                : stringID.Add(__L("DIOSTREAMTCPIP"));                  break;
      case XTHREADGROUPID_DIOSTREAMWIFI                 : stringID.Add(__L("DIOSTREAMWIFI"));                   break;
      case XTHREADGROUPID_DIOSTREAMBLUETOOTH            : stringID.Add(__L("DIOSTREAMBLUETOOTH"));              break;
      case XTHREADGROUPID_DIOSTREAMUSB                  : stringID.Add(__L("DIOSTREAMUSB"));                    break;
      case XTHREADGROUPID_DIOSTREAMICMP                 : stringID.Add(__L("DIOSTREAMICMP"));                   break;
      case XTHREADGROUPID_DIOSTREAMSPI                  : stringID.Add(__L("DIOSTREAMSPI"));                    break;
      case XTHREADGROUPID_DIOSTREAMCIPHER               : stringID.Add(__L("DIOSTREAMCIPHER"));                 break;
      case XTHREADGROUPID_DIOPROTOCOL                   : stringID.Add(__L("DIOPROTOCOL"));                     break;
      case XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER : stringID.Add(__L("DIOPROTOCOL_CONNECTIONMANAGER"));   break;
      case XTHREADGROUPID_DIOWEBSERVER                  : stringID.Add(__L("DIOWEBSERVER"));                    break;
      case XTHREADGROUPID_DIOATCMS                      : stringID.Add(__L("DIOATCMS"));                        break;
      case XTHREADGROUPID_DIOALERTS                     : stringID.Add(__L("DIOALERTS"));                       break;
      case XTHREADGROUPID_DIOPCAP                       : stringID.Add(__L("DIOPCAP"));                         break;
      case XTHREADGROUPID_VIDEO                         : stringID.Add(__L("VIDEO"));                           break;
      case XTHREADGROUPID_APPOWNER                      :
                              default                   : { XSTRING IDlocal;
                                                            IDlocal.Format(__L("APPOWNER[%d]"), groupID);

                                                            stringID.Add(IDlocal);
                                                          }
                                                          break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADSCOLLECTED::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADSCOLLECTED::Clean()
{
  groupID             = XTHREADGROUPID_UNKNOWN;
  xthreadsvectormutex = NULL;
  xthread             = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTEDMANAGER& XTHREADSCOLLECTEDMANAGER::GetInstance()
* @brief      Get instance
* @ingroup    XUTILS
* 
* @return     XTHREADSCOLLECTEDMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTEDMANAGER& XTHREADSCOLLECTEDMANAGER::GetInstance()
{
  if(!instance) instance = GEN_NEW XTHREADSCOLLECTEDMANAGER();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::DelInstance()
* @brief      Del instance
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::DelInstance()
{
  if(instance)
    {
      instance->DeleteAll();

      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::ActivateGroup(XTHREADGROUPID groupID)
* @brief      Activate group
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::ActivateGroup(XTHREADGROUPID groupID)
{
  groupthreadsactivate.Add(groupID);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::DeactivateGroup(XTHREADGROUPID groupID)
* @brief      Deactivate group
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::DeactivateGroup(XTHREADGROUPID groupID)
{
  groupthreadsactivate.Delete(groupID);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::IsActivateGroup(XTHREADGROUPID groupID)
* @brief      Is activate group
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::IsActivateGroup(XTHREADGROUPID groupID)
{
  for(XDWORD c=0; c<groupthreadsactivate.GetSize(); c++)
    {
      XTHREADGROUPID _groupID = groupthreadsactivate.Get(c);
      if(groupID == _groupID) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTED* XTHREADSCOLLECTEDMANAGER::GetThreadsCollectedByGroupID(XTHREADGROUPID groupID)
* @brief      Get threads collected by group ID
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* @return     XTHREADSCOLLECTED* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTED* XTHREADSCOLLECTEDMANAGER::GetThreadsCollectedByGroupID(XTHREADGROUPID groupID)
{
  XTHREADSCOLLECTED* xthreadscollected = NULL;

  for(XDWORD c=0; c<groupthreadsvector.GetSize(); c++)
    {
      XTHREADSCOLLECTED* _xthreadscollected = groupthreadsvector.Get(c);
      if(_xthreadscollected)
        {
          if(_xthreadscollected->GetGroupID() == groupID)
            {
              xthreadscollected = _xthreadscollected;
              break;
            }
        }
    }

  return xthreadscollected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED* XTHREADSCOLLECTEDMANAGER::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Create thread
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  param : Param pointer to use.
* 
* @return     XTHREADCOLLECTED* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED* XTHREADSCOLLECTEDMANAGER::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  XTHREADCOLLECTED*  xthreadcollected = NULL;
  bool               isactivategroup  = IsActivateGroup(groupID);

  if(isactivategroup)
    {
      if(!groupthreadsmutex) return NULL;

      groupthreadsmutex->Lock();

      XTHREADSCOLLECTED* xthreadscollected = GetThreadsCollectedByGroupID(groupID);
      if(!xthreadscollected)
        {
          xthreadscollected = GEN_NEW XTHREADSCOLLECTED(groupID);
          if(xthreadscollected) groupthreadsvector.Add(xthreadscollected);
        }

      if(xthreadscollected) xthreadcollected = xthreadscollected->Create(groupID, ID, function, param);

      groupthreadsmutex->UnLock();
    }
    else
    {
      XTHREADSCOLLECTED* xthreadscollected = GEN_NEW XTHREADSCOLLECTED();
      if(xthreadscollected)
        {
          xthreadcollected = xthreadscollected->Create(groupID, ID, function, param);
          GEN_DELETE xthreadscollected;
        }
    }

  return xthreadcollected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::DeleteThread(XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
* @brief      Delete thread
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  xthreadcollected : Xthreadcollected pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::DeleteThread(XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
{
  bool isactivategroup = IsActivateGroup(groupID);
  bool status          = false;

  if(isactivategroup)
    {
      if(!groupthreadsmutex) return false;

      groupthreadsmutex->Lock();

      XTHREADSCOLLECTED* xthreadscollected = GetThreadsCollectedByGroupID(isactivategroup?groupID:XTHREADGROUPID_UNGROUP);
      if(xthreadscollected)
        {
          status = xthreadscollected->Delete(groupID, xthreadcollected);
        }

      groupthreadsmutex->UnLock();
    }
    else
    {
      XTHREADSCOLLECTED* xthreadscollected = GEN_NEW XTHREADSCOLLECTED();
      if(xthreadscollected)
        {
          status = xthreadscollected->Delete(groupID, xthreadcollected);
          GEN_DELETE xthreadscollected;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREADSCOLLECTEDMANAGER::DeleteAll()
* @brief      Delete all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREADSCOLLECTEDMANAGER::DeleteAll()
{
  if(!groupthreadsmutex) return false;

  groupthreadsmutex->Lock();

  groupthreadsvector.DeleteContents();
  groupthreadsvector.DeleteAll();

  groupthreadsmutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTEDMANAGER::XTHREADSCOLLECTEDMANAGER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTEDMANAGER::XTHREADSCOLLECTEDMANAGER()
{
  Clean();

  groupthreadsmutex=GEN_XFACTORY.Create_Mutex();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSCOLLECTEDMANAGER::~XTHREADSCOLLECTEDMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSCOLLECTEDMANAGER::~XTHREADSCOLLECTEDMANAGER()
{
  DeleteAll();

  groupthreadsactivate.DeleteAll();

  if(groupthreadsmutex) GEN_XFACTORY.Delete_Mutex(groupthreadsmutex);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREADSCOLLECTEDMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREADSCOLLECTEDMANAGER::Clean()
{
  nogroupthread     = NULL;
  groupthreadsmutex = NULL;
}




#endif



