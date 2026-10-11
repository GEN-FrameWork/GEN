/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCheckInternetConnection.cpp
* 
* @class      DIOCHECKINTERNETCONNECTION
* @brief      Data Input/Output Check Connection Internet class
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

#include "DIOCheckInternetConnection.h"

#include "XFactory.h"
#include "XDateTime.h"

#include "DIOCheckTCPIPConnections.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKINTERNETCONNECTION::DIOCHECKINTERNETCONNECTION(int timeconnectionchecks, bool waitforcheck)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  timeconnectionchecks : Timeconnectionchecks value.
* @param[in]  waitforcheck : If true, block until at least one DNS probe finishes (or timeout).
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKINTERNETCONNECTION::DIOCHECKINTERNETCONNECTION(int timeconnectionchecks, bool waitforcheck)
{
  typedef struct { XCHAR* ipaddr;
                   XCHAR* name;

                 } MAINDNS;


  MAINDNS  maindns[] = {  { NL("8.8.8.8")          , NL("Google DNS 1")                                 },
                          { NL("8.8.4.4")          , NL("Google DNS 2")                                 },
                          { NL("198.41.0.4")       , NL("VeriSign, Inc.")                               },
                          
                          /*               
                          { NL("192.228.79.201")   , NL("University of Southern California (ISI)")      },
                          { NL("192.33.4.12")      , NL("Cogent Communications")                        },
                          { NL("199.7.91.13")      , NL("University of Maryland")                       },
                          { NL("192.203.230.10")   , NL("NASA (Ames Research Center)")                  },
                          { NL("192.5.5.241")      , NL("Internet Systems Consortium, Inc.")            },
                          { NL("198.97.190.53")    , NL("US Army (Research Lab)")                       },
                          { NL("192.36.148.17")    , NL("Netnod")                                       },
                          { NL("192.58.128.30")    , NL("VeriSign, Inc.")                               },
                          { NL("193.0.14.129")     , NL("RIPE NCC")                                     },
                          { NL("199.7.83.42")      , NL("ICANN")                                        },
                          { NL("202.12.27.33")     , NL("WIDE Project")                                 }
                          */
                       };
  Clean();

  checkconnections = GEN_NEW DIOCHECKTCPIPCONNECTIONS();
  if(!checkconnections) return;

  xmutexconnectioncuts=GEN_XFACTORY.Create_Mutex();

  checkconnections->Ini(timeconnectionchecks, true, true);

  for(int c=0; c<(sizeof(maindns)/sizeof(MAINDNS)); c++)
    {
      checkconnections->Connection_Add(maindns[c].ipaddr, connectionID[c]);
    }

  // Optional: NativeActivity / UI apps should pass waitforcheck=false so AppProc_Ini does not
  // block the start path on DNS (HardwareInfo / Check() can poll asynchronously instead).
  if(waitforcheck)
    {
      checkconnections->Connections_WaitToSomeIsChecked(checkconnections->GetTimeConnectionChecks());
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKINTERNETCONNECTION::~DIOCHECKINTERNETCONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKINTERNETCONNECTION::~DIOCHECKINTERNETCONNECTION()
{
  DeleteAllConnectionCuts();

  if(checkconnections)
    {
      for(int c=0; c<DIOCHECKINTERNETCONNECTION_MAXCONNECTIONS; c++)
        {
          checkconnections->Connection_DeleteByID(connectionID[c]);
        }
    }

  if(checkconnections)
    {
      GEN_DELETE checkconnections;
      checkconnections = NULL;
    }

  GEN_XFACTORY.Delete_Mutex(xmutexconnectioncuts);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTIONS* DIOCHECKINTERNETCONNECTION::GetCheckConnections()
* @brief      Get check connections
* @ingroup    DATAIO
* 
* @return     DIOCHECKTCPIPCONNECTIONS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTIONS* DIOCHECKINTERNETCONNECTION::GetCheckConnections()
{
  return checkconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKINTERNETCONNECTION::Check()
* @brief      Check
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKINTERNETCONNECTION::Check()
{
  if(!checkconnections) return false;

  bool        isinternetactive    = false;
  static bool isinternetactiveold = false;

  isinternetactive = checkconnections->Connections_SomeIsConnected(); 

  latency = checkconnections->Connections_GetLatency();
 
  if(isinternetactiveold != isinternetactive)
    {
      if(!isinternetactive)
        {
          DIOCHECKTCPIPCONNECTION_CUT connectioncut;

          connectioncut.GetStartDateTime()->Read();
          connectioncut.SetMeasureNSeconds((XDWORD)DIOCHECKTCPIPCONNECTIONS_INFINITECUT);

          AddConnectionCut(connectioncut);
        }
       else
        {
          if(GetConnectionCuts()->GetSize())
            {
              DIOCHECKTCPIPCONNECTION_CUT* connectioncut = GetConnectionCuts()->Get(GetConnectionCuts()->GetSize()-1);
              if(connectioncut)
                {
                  if(connectioncut->GetMeasureNSeconds() == DIOCHECKTCPIPCONNECTIONS_INFINITECUT)
                    {
                      XDATETIME* xdatetime=GEN_XFACTORY.CreateDateTime();
                      if(xdatetime)
                        {
                          xdatetime->Read();

                          connectioncut->SetMeasureNSeconds((int)(xdatetime->GetSeconsFromDate() - connectioncut->GetStartDateTime()->GetSeconsFromDate()));

                          GEN_XFACTORY.DeleteDateTime(xdatetime);
                        }
                    }
                }
            }
        }

      isinternetactiveold = isinternetactive;

      ischangeconnectionstatus = true;

    } else ischangeconnectionstatus = false;


  return isinternetactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKINTERNETCONNECTION::AddConnectionCut(DIOCHECKTCPIPCONNECTION_CUT& connectioncut)
* @brief      Add connection cut
* @ingroup    DATAIO
* 
* @param[in]  connectioncut : Connectioncut value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKINTERNETCONNECTION::AddConnectionCut(DIOCHECKTCPIPCONNECTION_CUT& connectioncut)
{
  DIOCHECKTCPIPCONNECTION_CUT* chekconnectioncut = GEN_NEW DIOCHECKTCPIPCONNECTION_CUT();
  if(!chekconnectioncut) return false;

  chekconnectioncut->CopyFrom(&connectioncut);

  if(connectioncuts.GetSize() >= DIOCHECKTCPIPCONNECTIONS_MAXNCONNECTIONCUTS) connectioncuts.Delete(connectioncuts.Get(0));

  connectioncuts.Add(chekconnectioncut);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKINTERNETCONNECTION::DeleteAllConnectionCuts()
* @brief      Delete all connection cuts
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKINTERNETCONNECTION::DeleteAllConnectionCuts()
{
  if(connectioncuts.IsEmpty()) return false;

  if(!xmutexconnectioncuts) return false;

  xmutexconnectioncuts->Lock();

  connectioncuts.DeleteContents();
  connectioncuts.DeleteAll();

  xmutexconnectioncuts->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOCHECKTCPIPCONNECTION_CUT*>* DIOCHECKINTERNETCONNECTION::GetConnectionCuts()
* @brief      Get connection cuts
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOCHECKTCPIPCONNECTION_CUT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOCHECKTCPIPCONNECTION_CUT*>*   DIOCHECKINTERNETCONNECTION::GetConnectionCuts()
{
  return &connectioncuts;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION_CUT* DIOCHECKINTERNETCONNECTION::GetLastConnectionsCut()
* @brief      Get last connections cut
* @ingroup    DATAIO
* 
* @return     DIOCHECKTCPIPCONNECTION_CUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION_CUT* DIOCHECKINTERNETCONNECTION::GetLastConnectionsCut()
{
  return GetConnectionCuts()->Get(GetConnectionCuts()->GetSize()-1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCHECKINTERNETCONNECTION::IsChangeConnectionStatus()
* @brief      Is change connection status
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCHECKINTERNETCONNECTION::IsChangeConnectionStatus()
{
  return ischangeconnectionstatus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCHECKINTERNETCONNECTION::GetLatency()
* @brief      Get latency
* @ingroup    DATAIO
* 
* @return     XDWORD : Latency (in milliseconds) measured on the last Check(); 0 if there is no internet connection.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCHECKINTERNETCONNECTION::GetLatency()
{
  return latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCHECKINTERNETCONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCHECKINTERNETCONNECTION::Clean()
{
  checkconnections         = NULL;

  for(int c=0; c<DIOCHECKINTERNETCONNECTION_MAXCONNECTIONS; c++)
    {
      connectionID[c] = 0;
    }

  ischangeconnectionstatus = false;
  xmutexconnectioncuts     = NULL;
  latency                  = 0;
}



