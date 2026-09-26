/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCLIProtocolBus.cpp
* 
* @class      DIOCLIPROTOCOLBUS
* @brief      Data Input/Output protocol in CLI Bus class
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

#include "DIOCLIProtocolBus.h"

#include "XFactory.h"
#include "XRand.h"
#include "XTrace.h"
#include "XSleep.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCLIPROTOCOLBUS::DIOCLIPROTOCOLBUS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCLIPROTOCOLBUS::DIOCLIPROTOCOLBUS()

{
  Clean();

  nretries = DIOCLIPROTOCOLBUS_SEND_DEFAULNRETRIES;

	AddCommand(DIOCLIPROTOCOLBUS_COMMAND_VERSION			      , 0);
	AddCommand(DIOCLIPROTOCOLBUS_COMMAND_PING					      , 0);
  AddCommand(DIOCLIPROTOCOLBUS_COMMAND_ENUM 				      , 0);
  AddCommand(DIOCLIPROTOCOLBUS_COMMAND_ENUMREQUEST        , 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCLIPROTOCOLBUS::~DIOCLIPROTOCOLBUS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCLIPROTOCOLBUS::~DIOCLIPROTOCOLBUS()

{
  enum_remotedevices.DeleteContents();
  enum_remotedevices.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::Ini(DIOSTREAM* diostream, XCHAR* ID, int timeout)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* @param[in]  ID : Identifier to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::Ini(DIOSTREAM* diostream, XCHAR* ID, int timeout)
{	
	this->ID = ID;

  xmutexsendcommand=GEN_XFACTORY.Create_Mutex();
  if(!xmutexsendcommand) return false;

  enum_mutex=GEN_XFACTORY.Create_Mutex();
  if(!enum_mutex) return false;

  threadsendenumrequest	= CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CLI_BUS, __L("DIOCLIPROTOCOLBUS::Ini2"), ThreadSendEnumRequest, (void*)this);
  if(!threadsendenumrequest) return false;

	threadreceivedcommand	= CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CLI_BUS, __L("DIOCLIPROTOCOLBUS::Ini"), ThreadReceivedCommand, (void*)this);
  if(!threadreceivedcommand) return false;

  enum_timer = GEN_XFACTORY.CreateTimer();
  if(!enum_timer) return false;
  	
  if(!threadreceivedcommand->Ini()) return false;

  return DIOCLIPROTOCOL::Ini(diostream, ID, timeout);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerror)
* @brief      Get version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerror : Subversionerror value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerror)
{
  if((!version) && (!subversion) && (!subversionerror)) return false;

  version         = this->version;
  subversion      = this->subversion;
  subversionerror = this->subversionerror;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerror)
* @brief      Set version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerror : Subversionerror value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerror)
{
  this->version         = version;
  this->subversion      = subversion;
  this->subversionerror = subversionerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::EnumRemoteDevices(XVECTOR<XSTRING*>* remotedevices, XDWORD maxtime)
* @brief      Enum remote devices
* @ingroup    DATAIO
* 
* @param[in]  remotedevices : Remotedevices pointer to use.
* @param[in]  maxtime : Maxtime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::EnumRemoteDevices(XVECTOR<XSTRING*>* remotedevices, XDWORD maxtime)
{
  bool status = false;

  enum_remotedevices.DeleteContents();
  enum_remotedevices.DeleteAll();

  if(!enum_timer) return false;

  SendCommand(DIOCLIPROTOCOLBUS_COMMAND_ENUM, NULL, NULL, 0);
    
  enum_maxtimersec = maxtime;

  enum_timer->Reset();

  if(remotedevices)
    {
      remotedevices->DeleteContents();
      remotedevices->DeleteAll();

      do{ GEN_XSLEEP.Seconds(1);           

        } while(enum_timer->GetMeasureSeconds() < enum_maxtimersec); 

      status = GetEnumRemoteDevices((*remotedevices));

    } else status = true;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::GetEnumRemoteDevices(XVECTOR<XSTRING*>& remotedevices)
* @brief      Get enum remote devices
* @ingroup    DATAIO
* 
* @param[in]  remotedevices : Remotedevices pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::GetEnumRemoteDevices(XVECTOR<XSTRING*>& remotedevices)
{
  if(!enum_timer)       return false;
  if(!enum_maxtimersec) return false;

  if(enum_timer->GetMeasureSeconds() < enum_maxtimersec) return false;

  for(XDWORD c=0; c<enum_remotedevices.GetSize(); c++)
    {
      XSTRING* device = GEN_NEW XSTRING();
      if(device)
        {
          (*device) = enum_remotedevices.Get(c)->Get();
          remotedevices.Add(device);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOCLIPROTOCOLBUS::GetNRetries()
* @brief      Get N retries
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOCLIPROTOCOLBUS::GetNRetries()
{
  return nretries;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::SetNRetries(int nretries)
* @brief      Set N retries
* @ingroup    DATAIO
* 
* @param[in]  nretries : Nretries value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::SetNRetries(int nretries)
{
  this->nretries = nretries;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::SendCommand(XCHAR* command, XSTRING* target, XSTRING* answer, int timeoutanswer, ...)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command pointer to use.
* @param[in]  target : Target pointer to use.
* @param[in]  answer : Answer pointer to use.
* @param[in]  timeoutanswer : Timeoutanswer value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::SendCommand(XCHAR* command, XSTRING* target, XSTRING* answer, int timeoutanswer, ...)
{
  if(!command) return false;
  
  bool status    = false;

  if(xmutexsendcommand) xmutexsendcommand->Lock();
  
  va_list arg;
			
  va_start(arg, timeoutanswer);	
  status = DIOCLIPROTOCOL::SendCommandArg(command, target, answer, timeoutanswer, &arg);           																		
  va_end(arg);  
  
  if(xmutexsendcommand) xmutexsendcommand->UnLock();

  return status;
}    


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCLIPROTOCOLBUS::ReceivedCommand(XSTRING& originID, XSTRING& command, XVECTOR<XSTRING*>& params, XSTRING& answer)
* @brief      Received command
* @ingroup    DATAIO
* 
* @param[in]  originID : Origin ID value.
* @param[in]  command : Command value.
* @param[in]  params : Params pointer to use.
* @param[in]  answer : Answer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCLIPROTOCOLBUS::ReceivedCommand(XSTRING& originID, XSTRING& command, XVECTOR<XSTRING*>& params, XSTRING& answer)
{
  bool status = false;
  
	answer.Empty();  
  
  DIOCLIPROTOCOLCOMMAND* protocolcommand = GetCommand(command.Get());
	if(protocolcommand) 
		{
			if(protocolcommand->GetNParams() < (int)params.GetSize()) return false;			

			if(!command.Compare(DIOCLIPROTOCOLBUS_COMMAND_VERSION, true))	  		 
				{
					answer.Format(__L("%d.%d.%d"), version, subversion, subversionerror); 
					status = true;            
				}

			if(!command.Compare(DIOCLIPROTOCOLBUS_COMMAND_PING, true))	  		 
				{					
					status = true;            
				}

      if(!command.Compare(DIOCLIPROTOCOLBUS_COMMAND_ENUM, true))	  		 
				{		
          enum_mutex->Lock();

          enum_sendoriginID = originID;

          enum_mutex->UnLock();

          threadsendenumrequest->Ini();
				}
      
      if(!command.Compare(DIOCLIPROTOCOLBUS_COMMAND_ENUMREQUEST, true))
        {
          bool found = false;

          for(XDWORD c=0; c<enum_remotedevices.GetSize(); c++)
            {
              XSTRING* device = enum_remotedevices.Get(c);
              if(device)
                {
                  if(!device->Compare(originID, true)) found = true;
                }
            }

          if(!found)
            {
              XSTRING* origin = GEN_NEW XSTRING();
              if(origin)
                {
                  (*origin) = originID;    
                  enum_remotedevices.Add(origin);
                }
            }
         
          status = true;
        }
		}

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::End()
{
  exitproccess = true;

  if(threadsendenumrequest)					
    {
      threadsendenumrequest->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CLI_BUS, threadsendenumrequest);
      
      threadsendenumrequest = NULL;
    }

  if(threadreceivedcommand)					
    {
      threadreceivedcommand->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CLI_BUS, threadreceivedcommand);
      
      threadreceivedcommand = NULL;
    }

  if(enum_mutex)
    { 
      GEN_XFACTORY.Delete_Mutex(enum_mutex);
      enum_mutex = NULL;
    }

  if(xmutexsendcommand)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexsendcommand);
      xmutexsendcommand = NULL;
    }

  if(enum_timer)
    {
      GEN_XFACTORY.DeleteTimer(enum_timer);
      enum_timer = NULL;
    }

  return DIOCLIPROTOCOL::End();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::ThreadReceivedCommand(void* param)
* @brief      Thread received command
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::ThreadReceivedCommand(void* param)
{
  DIOCLIPROTOCOLBUS* sp = (DIOCLIPROTOCOLBUS*)param;
	if(!sp) return;

  if(sp->exitproccess) 
    {      
      return;
    }

  sp->ReceivedCommandManager();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::ThreadSendEnumRequest(void* param)
* @brief      Thread send enum request
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::ThreadSendEnumRequest(void* param)
{
  DIOCLIPROTOCOLBUS* sp = (DIOCLIPROTOCOLBUS*)param;
	if(!sp) return;

  if(sp->exitproccess) 
    { 
      return;
    }

  if(sp->enum_mutex) sp->enum_mutex->Lock();

  if(sp->enum_sendoriginID.IsEmpty()) 
    {
      sp->threadsendenumrequest->Run(false); 
    }
   else
    {    
      XSTRING answer;
  
      bool status = sp->SendCommand(DIOCLIPROTOCOLBUS_COMMAND_ENUMREQUEST, &sp->enum_sendoriginID, &answer, sp->enum_maxtimersec); 
      if(status)
        {
          sp->enum_sendoriginID.Empty();
        }
    } 

  if(sp->enum_mutex) sp->enum_mutex->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCLIPROTOCOLBUS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCLIPROTOCOLBUS::Clean()
{
  version                 = 0;
  subversion              = 0;
  subversionerror         = 0;

  nretries                = 0;

  exitproccess            = false;

  enum_mutex              = NULL;
  enum_sendoriginID.Empty();
  enum_timer              = NULL;
  enum_maxtimersec        = 0;

  xmutexsendcommand       = NULL;

	threadreceivedcommand   = NULL;
  threadsendenumrequest   = NULL;
}



