/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended_InternetStatus.cpp
* 
* @class      APPFLOWEXTENDED_INTERNETSTATUS
* @brief      Application Flow Extended Internet Status class
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

#include "APPFlowExtended_InternetStatus.h"

#include "APPFlowInternetServices.h"
#include "APPFlowCFG.h"
#include "APPFlowConsole.h"
#include "APPFlowExtended.h"

#include "XTranslation_GEN.h"
#include "XTranslation.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_INTERNETSTATUS::APPFLOWEXTENDED_INTERNETSTATUS(APPFLOWCFG* cfg) : DIOWEBSERVER_RESOLVEENDPOINT()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* 
* @param[in]  cfg : Application flow configuration.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_INTERNETSTATUS::APPFLOWEXTENDED_INTERNETSTATUS(APPFLOWCFG* cfg) : DIOWEBSERVER_RESOLVEENDPOINT()
{
  Clean();

  this->cfg = cfg;

  internetservices = GEN_NEW APPFLOWINTERNETSERVICES();
  if(internetservices) 
    {
      internetservices->Ini(cfg);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_INTERNETSTATUS::~APPFLOWEXTENDED_INTERNETSTATUS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_INTERNETSTATUS::~APPFLOWEXTENDED_INTERNETSTATUS()
{ 
  if(internetservices)
    {
      internetservices->End();
      GEN_DELETE internetservices;      
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWINTERNETSERVICES* APPFLOWEXTENDED_INTERNETSTATUS::GetInternetServices()
* @brief      Get internet services
* @ingroup    APPFLOW
* 
* @return     APPFLOWINTERNETSERVICES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWINTERNETSERVICES* APPFLOWEXTENDED_INTERNETSTATUS::GetInternetServices()
{
  return internetservices;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_INTERNETSTATUS::GetLocalIP()
* @brief      Get local IP
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_INTERNETSTATUS::GetLocalIP()
{
  return &localIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_INTERNETSTATUS::GetHaveInternetConnection()
* @brief      Get have internet connection
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_INTERNETSTATUS::GetHaveInternetConnection()
{
  return haveinternetconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_INTERNETSTATUS::GetPublicIP()
* @brief      Get public IP
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_INTERNETSTATUS::GetPublicIP()
{
  return &publicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWEXTENDED_INTERNETSTATUS::GetLatency()
* @brief      Get latency
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWEXTENDED_INTERNETSTATUS::GetLatency()
{
  return latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_INTERNETSTATUS::SetLatency(XDWORD latency)
* @brief      Set latency
* @ingroup    APPFLOW
* 
* @param[in]  latency : Latency value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_INTERNETSTATUS::SetLatency(XDWORD latency)
{
  this->latency = latency;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_INTERNETSTATUS::Update()
* @brief      Update
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_INTERNETSTATUS::Update()
{  
  if(!internetservices)
    {
      return false;
    }

  localIP = internetservices->GetAutomaticLocalIP()->Get();

  haveinternetconnection = internetservices->HaveInternetConnection();
  if(haveinternetconnection)  
    {
      publicIP = internetservices->GetPublicIP()->Get();
    }
   else
    {
      publicIP = _L("");
    }

  latency = internetservices->GetInternetLatency();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_INTERNETSTATUS::Show(XCONSOLE* console)
* @brief      Show
* @ingroup    APPFLOW
* 
* @param[in]  console : Console pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_INTERNETSTATUS::Show(XCONSOLE* console)
{
  XSTRING   string;
  XSTRING   string2;

  if(!APPFLOW_EXTENDED.GetConsole())
    { 
      return false;
    }

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP);
  string2.Format(_L("[%s]"), localIP.Get());
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION);
  string2.ConvertFromBoolean(haveinternetconnection, XSTRINGBOOLEANMODE_HUMAN);   
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  if(haveinternetconnection)
    {
      string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY);
      string2.Format(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS), latency);
      APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);
    }

  if(haveinternetconnection && !publicIP.IsEmpty())
    {
      string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP);
      string2.Format(_L("[%s]"), publicIP.Get());
      APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);
    }
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_INTERNETSTATUS::Serialize()
* @brief      Serialize
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_INTERNETSTATUS::Serialize()
{
  Primitive_Add<XSTRING*>(&localIP            , _L("localIP"));
  
  Primitive_Add<bool>(haveinternetconnection  , _L("internetconnection"));
  Primitive_Add<XSTRING*>(&publicIP           , _L("publicIP")); 
  Primitive_Add<int>(latency                  , _L("latencyms"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_INTERNETSTATUS::Deserialize()
* @brief      Deserialize
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_INTERNETSTATUS::Deserialize()
{ 
  Primitive_Extract<XSTRING>(localIP             , _L("localIP"));
  
  Primitive_Extract<bool>(haveinternetconnection , _L("internetconnection"));
  Primitive_Extract<XSTRING>(publicIP            , _L("publicIP"));
  Primitive_Extract<XDWORD>(latency              , _L("latencyms"));
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_INTERNETSTATUS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_INTERNETSTATUS::Clean()
{
  cfg                     = NULL;
  internetservices        = NULL;

  localIP.Empty();

  haveinternetconnection  = false;
  publicIP.Empty();    

  latency                 = 0;
}


