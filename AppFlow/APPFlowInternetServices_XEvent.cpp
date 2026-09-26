/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowInternetServices_XEvent.cpp
* 
* @class      APPFLOWINTERNETSERVICES_XEVENT
* @brief      Application Flow Internet Services eXtended Event class
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

#include "APPFlowInternetServices_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWINTERNETSERVICES_XEVENT::APPFLOWINTERNETSERVICES_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* @param[in]  subject : Subject pointer to use.
* @param[in]  type : Type value.
* @param[in]  family : Family value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWINTERNETSERVICES_XEVENT::APPFLOWINTERNETSERVICES_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWINTERNETSERVICES_XEVENT::~APPFLOWINTERNETSERVICES_XEVENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWINTERNETSERVICES_XEVENT::~APPFLOWINTERNETSERVICES_XEVENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE APPFLOWINTERNETSERVICES_XEVENT::GetInternetConnexionState()
* @brief      Get internet connexion state
* @ingroup    APPFLOW
* 
* @return     APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE APPFLOWINTERNETSERVICES_XEVENT::GetInternetConnexionState()
{
  return internetconnexionstate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetInternetConnexionState(APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE internetconnexionstate)
* @brief      Set internet connexion state
* @ingroup    APPFLOW
* 
* @param[in]  internetconnexionstate : Internetconnexionstate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetInternetConnexionState(APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE internetconnexionstate)
{
  this->internetconnexionstate = internetconnexionstate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCHECKTCPIPCONNECTION_CUT* APPFLOWINTERNETSERVICES_XEVENT::GetInternetConnextionCut()
* @brief      Get internet connextion cut
* @ingroup    APPFLOW
* 
* @return     DIOCHECKTCPIPCONNECTION_CUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCHECKTCPIPCONNECTION_CUT* APPFLOWINTERNETSERVICES_XEVENT::GetInternetConnextionCut()
{
  return internetconnectioncut;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetInternetConnextionCut(DIOCHECKTCPIPCONNECTION_CUT* internetconnectioncut)
* @brief      Set internet connextion cut
* @ingroup    APPFLOW
* 
* @param[in]  internetconnectioncut : Internetconnectioncut pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetInternetConnextionCut(DIOCHECKTCPIPCONNECTION_CUT* internetconnectioncut)
{
  this->internetconnectioncut  = internetconnectioncut;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetLatency()
* @brief      Get latency
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetLatency()
{
  return latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetLatency(XDWORD latency)
* @brief      Set latency
* @ingroup    APPFLOW
* 
* @param[in]  latency : Latency value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetLatency(XDWORD latency)
{
  this->latency = latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWINTERNETSERVICES_XEVENT::IsChangePublicIP()
* @brief      Is change public IP
* @ingroup    APPFLOW
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWINTERNETSERVICES_XEVENT::IsChangePublicIP()
{
  return ischangepublicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetIsChangePublicIP(bool ischangepublicIP)
* @brief      Set is change public IP
* @ingroup    APPFLOW
* 
* @param[in]  ischangepublicIP : Ischangepublic IP value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetIsChangePublicIP(bool ischangepublicIP)
{
  this->ischangepublicIP = ischangepublicIP;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWINTERNETSERVICES_XEVENT::IsChangeLocalIP()
* @brief      Is change local IP
* @ingroup    APPFLOW
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWINTERNETSERVICES_XEVENT::IsChangeLocalIP()
{
  return ischangelocalIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetIsChangeLocalIP(bool ischangelocalIP)
* @brief      Set is change local IP
* @ingroup    APPFLOW
* 
* @param[in]  ischangelocalIP : Ischangelocal IP value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetIsChangeLocalIP(bool ischangelocalIP)
{
  this->ischangelocalIP = ischangelocalIP;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWINTERNETSERVICES_XEVENT::GetChangePublicIP()
* @brief      Get change public IP
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWINTERNETSERVICES_XEVENT::GetChangePublicIP()
{
  return &changepublicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWINTERNETSERVICES_XEVENT::GetChangeLocalIP()
* @brief      Get change local IP
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWINTERNETSERVICES_XEVENT::GetChangeLocalIP()
{
  return &changelocalIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesIP()
* @brief      Get N changes IP
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesIP()
{
  return nchangesIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesIP(XDWORD nchangesIP)
* @brief      Set N changes IP
* @ingroup    APPFLOW
* 
* @param[in]  nchangesIP : Nchanges IP value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesIP(XDWORD nchangesIP)
{
  this->nchangesIP = nchangesIP; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesLocalIP()
* @brief      Get N changes local IP
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesLocalIP()
{
  return nchangeslocalIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesLocalIP(XDWORD nchangeslocalIP)
* @brief      Set N changes local IP
* @ingroup    APPFLOW
* 
* @param[in]  nchangeslocalIP : Nchangeslocal IP value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesLocalIP(XDWORD nchangeslocalIP)
{
  this->nchangeslocalIP = nchangeslocalIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesPublicIP()
* @brief      Get N changes public IP
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWINTERNETSERVICES_XEVENT::GetNChangesPublicIP()
{
  return nchangespublicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesPublicIP(XDWORD nchangespublicIP)
* @brief      Set N changes public IP
* @ingroup    APPFLOW
* 
* @param[in]  nchangespublicIP : Nchangespublic IP value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::SetNChangesPublicIP(XDWORD nchangespublicIP)
{
  this->nchangespublicIP = nchangespublicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWINTERNETSERVICES_XEVENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWINTERNETSERVICES_XEVENT::Clean()
{
  internetconnexionstate  = APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_NONE;
  internetconnectioncut   = NULL;

  latency                 = 0;

  ischangepublicIP        = false;
  ischangelocalIP         = false;
  changepublicIP.Empty();
  changelocalIP.Empty();

  nchangesIP              = 0;
  nchangeslocalIP         = 0;
  nchangespublicIP        = 0;   
}


