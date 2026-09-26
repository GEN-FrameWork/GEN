/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOIEC60870_5.cpp
* 
* @class      DIOIEC60870_5
* @brief      Data Input/Output IEC 60870_5 (electric meters)
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOStream.h"

#include "DIOIEC60870_5.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIEC60870_5_XEVENT::DIOIEC60870_5_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  subject : Subject pointer to use.
* @param[in]  type : Type value.
* @param[in]  family : Family value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIEC60870_5_XEVENT::DIOIEC60870_5_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIEC60870_5_XEVENT::~DIOIEC60870_5_XEVENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIEC60870_5_XEVENT::~DIOIEC60870_5_XEVENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOIEC60870_5_XEVENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOIEC60870_5_XEVENT::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIO_C_CI_NU_2_RESULT::DIO_C_CI_NU_2_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIO_C_CI_NU_2_RESULT::DIO_C_CI_NU_2_RESULT()
{
  Clean();

  xdatetime=GEN_XFACTORY.CreateDateTime();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIO_C_CI_NU_2_RESULT::~DIO_C_CI_NU_2_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIO_C_CI_NU_2_RESULT::~DIO_C_CI_NU_2_RESULT()
{
  GEN_XFACTORY.DeleteDateTime(xdatetime);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* DIO_C_CI_NU_2_RESULT::GetDateTime()
* @brief      Get date time
* @ingroup    DATAIO
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* DIO_C_CI_NU_2_RESULT::GetDateTime()
{
  return xdatetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetIndexValues()
* @brief      Get index values
* @ingroup    DATAIO
* 
* @return     XVECTOR<XDWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetIndexValues()
{
  return &indexvalues;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetValues()
* @brief      Get values
* @ingroup    DATAIO
* 
* @return     XVECTOR<XDWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetValues()
{
  return &values;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIO_C_CI_NU_2_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIO_C_CI_NU_2_RESULT::Clean()
{
  xdatetime   = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIO_C_TR_AA_RESULT::DIO_C_TR_AA_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIO_C_TR_AA_RESULT::DIO_C_TR_AA_RESULT()
{
  Clean();

  xdatetime=GEN_XFACTORY.CreateDateTime();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIO_C_TR_AA_RESULT::~DIO_C_TR_AA_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIO_C_TR_AA_RESULT::~DIO_C_TR_AA_RESULT()
{
  GEN_XFACTORY.DeleteDateTime(xdatetime);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* DIO_C_TR_AA_RESULT::GetDateTime()
* @brief      Get date time
* @ingroup    DATAIO
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* DIO_C_TR_AA_RESULT::GetDateTime()
{
  return xdatetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetActiveImport()
* @brief      Get active import
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActiveImport()
{
  return activeimport;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetActiveImport(int activeimport)
* @brief      Set active import
* @ingroup    DATAIO
* 
* @param[in]  activeimport : Activeimport value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActiveImport(int activeimport)
{
  this->activeimport = activeimport;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetActiveExport()
* @brief      Get active export
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActiveExport()
{
  return activeexport;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetActiveExport(int activeexport)
* @brief      Set active export
* @ingroup    DATAIO
* 
* @param[in]  activeexport : Activeexport value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActiveExport(int activeexport)
{
  this->activeexport = activeexport;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetReactiveQ(int index)
* @brief      Get reactive q
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetReactiveQ(int index)
{
  if((index<1)||(index>4))  return -1;

  return reactiveQ[index-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetReactiveQ(int index, int reactiveQ)
* @brief      Set reactive q
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* @param[in]  reactiveQ : Reactive Q value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetReactiveQ(int index, int reactiveQ)
{
  if((index<1)||(index>4))  return false;
  this->reactiveQ[index-1] = reactiveQ;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetTotalActivePower()
* @brief      Get total active power
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetTotalActivePower()
{
  return totalactivepower;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetTotalActivePower(int totalactivepower)
* @brief      Set total active power
* @ingroup    DATAIO
* 
* @param[in]  totalactivepower : Totalactivepower value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalActivePower(int totalactivepower)
{
  this->totalactivepower = totalactivepower;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetTotalReactivePower()
* @brief      Get total reactive power
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetTotalReactivePower()
{
  return totalreactivepower;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetTotalReactivePower(int totalreactivepower)
* @brief      Set total reactive power
* @ingroup    DATAIO
* 
* @param[in]  totalreactivepower : Totalreactivepower value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalReactivePower(int totalreactivepower)
{
  this->totalreactivepower = totalreactivepower;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::GetTotalPowerFactor()
* @brief      Get total power factor
* @ingroup    DATAIO
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetTotalPowerFactor()
{
  return totalpowerfactor;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetTotalPowerFactor(float totalpowerfactor)
* @brief      Set total power factor
* @ingroup    DATAIO
* 
* @param[in]  totalpowerfactor : Totalpowerfactor value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalPowerFactor(float totalpowerfactor)
{
  this->totalpowerfactor = totalpowerfactor;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetActivePowerPhase(int phase)
* @brief      Get active power phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActivePowerPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return activepowerphase[phase-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetActivePowerPhase(int phase, int activepowerphase)
* @brief      Set active power phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* @param[in]  activepowerphase : Activepowerphase value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActivePowerPhase(int phase, int activepowerphase)
{
  if((phase<1)||(phase>3)) return false;
  this->activepowerphase[phase-1] = activepowerphase;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIO_C_TR_AA_RESULT::GetReactivePowerPhase(int phase)
* @brief      Get reactive power phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetReactivePowerPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return reactivepowerphase[phase-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetReactivePowerPhase(int phase, int reactivepowerphase)
* @brief      Set reactive power phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* @param[in]  reactivepowerphase : Reactivepowerphase value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetReactivePowerPhase(int phase, int reactivepowerphase)
{
  if((phase<1)||(phase>3)) return false;
  this->reactivepowerphase[phase-1] = reactivepowerphase;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::GetPowerFactorPhase(int phase)
* @brief      Get power factor phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetPowerFactorPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return powerfactorphase[phase-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIO_C_TR_AA_RESULT::SetPowerFactorPhase(int phase, float powerfactorphase)
* @brief      Set power factor phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* @param[in]  powerfactorphase : Powerfactorphase value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetPowerFactorPhase(int phase, float powerfactorphase)
{
  if((phase<1)||(phase>3)) return false;
  this->powerfactorphase[phase-1] = powerfactorphase;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::GetVoltagePhase(int phase)
* @brief      Get voltage phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetVoltagePhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return voltagephase[phase-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::SetVoltagePhase(int phase, float voltagephase)
* @brief      Set voltage phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* @param[in]  voltagephase : Voltagephase value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::SetVoltagePhase(int phase, float voltagephase)
{
  if((phase<1)||(phase>3)) return false;
  this->voltagephase[phase-1] = voltagephase;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::GetCurrentPhase(int phase)
* @brief      Get current phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetCurrentPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return currentphase[phase-1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIO_C_TR_AA_RESULT::SetCurrentPhase(int phase, float currentphase)
* @brief      Set current phase
* @ingroup    DATAIO
* 
* @param[in]  phase : Phase value.
* @param[in]  currentphase : Currentphase value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::SetCurrentPhase(int phase, float currentphase)
{
  if((phase<1)||(phase>3)) return false;
  this->currentphase[phase-1] = currentphase;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIO_C_TR_AA_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIO_C_TR_AA_RESULT::Clean()
{
  xdatetime    = NULL;

  activeimport = 0;
  activeexport = 0;

  int c;

  for(c=0;c<4;c++)
    {
      reactiveQ[c] = 0;
    }

  totalactivepower    = 0;
  totalreactivepower  = 0;
  totalpowerfactor    = 0.0f;

  for(c=0;c<3;c++)
    {
      activepowerphase[c]   = 0;
      reactivepowerphase[c] = 0;
      powerfactorphase[c]   = 0.0f;

      voltagephase[c]       = 0;
      currentphase[c]       = 0;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIEC60870_5::DIOIEC60870_5(DIOSTREAM* diostream)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIEC60870_5::DIOIEC60870_5(DIOSTREAM* diostream)
{
  Clean();

  this->diostream   = diostream;

  RegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_AC_NA_2_INISESSION);
  RegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_TR_AA_READVALUES);
  RegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_CI_NU_2_READVALUE);
  RegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_FS_NA_2_ENDSESSION);

  xtimer=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIEC60870_5::~DIOIEC60870_5()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIEC60870_5::~DIOIEC60870_5()
{
  GEN_XFACTORY.DeleteTimer(xtimer);

  Disconnect();

  DeRegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_AC_NA_2_INISESSION);
  DeRegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_TR_AA_READVALUES);
  DeRegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_CI_NU_2_READVALUE);
  DeRegisterEvent(DIOIEC60870_5_XEVENT_TYPE_C_FS_NA_2_ENDSESSION);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::Connect(XWORD addressfield, XBYTE addressPM, XDWORD keyPM, bool inlittleendian, bool havelongaddress, int timeout)
* @brief      Connect
* @ingroup    DATAIO
* 
* @param[in]  addressfield : Addressfield value.
* @param[in]  addressPM : Address PM value.
* @param[in]  keyPM : Key PM value.
* @param[in]  inlittleendian : Inlittleendian value.
* @param[in]  havelongaddress : Havelongaddress value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::Connect(XWORD addressfield, XBYTE addressPM, XDWORD keyPM, bool inlittleendian, bool havelongaddress, int timeout)
{
  if(!diostream)                                                  return false;
  //if(diostream->GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED) return false;

  #ifdef XTRACE_ACTIVE

  switch(diostream->GetConfig()->GetType())
    {
      case DIOSTREAMTYPE_UART  : XTRACE_PRINTCOLOR(1, __L("IEC60870_5 Connection to UART: "));
                                  break;

      case DIOSTREAMTYPE_TCPIP : XTRACE_PRINTCOLOR(1, __L("IEC60870_5 Connection to TCP/IP: "));
                                  break;

                       default :  break;
    }

  #endif

  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  XTRACE_PRINTCOLOR(1, __L("IEC60870_5 Connected physical layer."));

  this->addressfield      = addressfield;
  this->addressPM         = addressPM;
  this->keyPM             = keyPM;
  this->inlittleendian    = inlittleendian;
  this->havelongaddress   = havelongaddress;

  XTRACE_PRINTCOLOR(1, __L("IEC60870_5 Meter Address Field: %d, Address PM: %d, Key PM: %d"), addressfield, addressPM, keyPM);

  canceloperations = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_ResetRemoteLink(int timeout)
* @brief      CMD reset remote link
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_ResetRemoteLink(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCRESETREMOTELINK))    return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))      return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)    return true;
  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_RequestStatusLink(int timeout)
* @brief      CMD request status link
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestStatusLink(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTSTATUSLINK))    return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))        return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDSTATUSLINK) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_RequestUserDataClass1(XBUFFER* asdu, int timeout)
* @brief      CMD request user data class1
* @ingroup    DATAIO
* 
* @param[in]  asdu : Asdu pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestUserDataClass1(XBUFFER* asdu, int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS1))  return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, asdu, timeout))          return false;

  if(retisfixmsg)
    {
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDDATAUSER)     return true;
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDNOTDATAUSER)  return false;

    } else return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_RequestUserDataClass2(XBUFFER* asdu, int timeout)
* @brief      CMD request user data class2
* @ingroup    DATAIO
* 
* @param[in]  asdu : Asdu pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestUserDataClass2(XBUFFER* asdu, int timeout)
{
  if(!asdu) return false;

  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS2))  return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, asdu, timeout))          return false;

  if(retisfixmsg)
    {
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDDATAUSER)     return true;
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDNOTDATAUSER)  return false;

    }
   else
    {
      if(asdu->GetByte(2) == DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return true;
      if(asdu->GetByte(2) == DIOIEC60870_5_ASDUCAUSETRANS_REQUESTED)         return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_AC_NA_2_IniSession(int timeout)
* @brief      CMDCACNA2 ini session
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_AC_NA_2_IniSession(int timeout)
{
  XBYTE retfunctioncode;
  bool  retisfixmsg;

  if(!CMD_RequestStatusLink(timeout)) return false;
  if(!CMD_ResetRemoteLink(timeout))   return false;
  if(!CMD_RequestStatusLink(timeout)) return false;

  XBUFFER asdu;

  CMD_RequestUserDataClass1(&asdu, timeout);

  asdu.Delete();

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_AC_NA_2_SESSIONINI); // Identificator
  asdu.Add((XBYTE)1);                                         // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);   // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                 // Meter Point Address
  asdu.Add((XBYTE)00);                                        // Register Address  = 0
  asdu.Add((XBYTE)00);                                        // Register Address  = 0

  XDWORD _keyPM = keyPM;

  if(inlittleendian) SWAPDWORD(_keyPM);
  asdu.Add((XDWORD)_keyPM);                                   // Meter Point Key

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA, &asdu))   return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;

      if(CMD_RequestUserDataClass2(&retasdu, timeout))
        {
          XBYTE* buffer = retasdu.Get();
          if(!buffer) return false;

          if(buffer[0]!=DIOIEC60870_5_ASDUID_C_AC_NA_2_SESSIONINI)      return false;
          if(buffer[2]!=DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return false;

          DIOIEC60870_5_XEVENT xevent(this, DIOIEC60870_5_XEVENT_TYPE_C_AC_NA_2_INISESSION);
          PostEvent(&xevent);

          return true;
        }

      return false;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_FS_NA_2_EndSession(int timeout)
* @brief      CMDCFSNA2 end session
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_FS_NA_2_EndSession(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  XBUFFER asdu;

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_FS_NA_2_SESSIONEND);  // Identificator
  asdu.Add((XBYTE)0);                                          // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);    // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                  // Meter Point Address
  asdu.Add((XWORD)0x0000);                                     // Register Address  = 0

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA, &asdu))   return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;

      if(CMD_RequestUserDataClass2(&retasdu, timeout))
        {
          XBYTE* buffer = retasdu.Get();
          if(!buffer) return false;

          if(buffer[0]!=DIOIEC60870_5_ASDUID_C_FS_NA_2_SESSIONEND)      return false;
          if(buffer[2]!=DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return false;

          DIOIEC60870_5_XEVENT xevent(this, DIOIEC60870_5_XEVENT_TYPE_C_FS_NA_2_ENDSESSION);
          PostEvent(&xevent);

          return true;
        }

      return false;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_CI_NU_2_Init(int TM, XDATETIME& timestart, XDATETIME& timeend, int timeout)
* @brief      CMDCCINU2 init
* @ingroup    DATAIO
* 
* @param[in]  TM : TM value.
* @param[in]  timestart : Timestart value.
* @param[in]  timeend : Timeend value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_Init(int TM, XDATETIME& timestart, XDATETIME& timeend, int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  XBUFFER asdu;

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_CI_NU_2);              // Identificator
  asdu.Add((XBYTE)01);                                          // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);     // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                   // Meter Point Address
  asdu.Add((XBYTE)0x00);                                        // Register Address
  asdu.Add((XBYTE)((TM==1)?0x0B:0x0C));                         // Register Address
  asdu.Add((XBYTE)0x01);                                        // Address of Total First
  asdu.Add((XBYTE)0x08);                                        // Address of Total Last

  XBYTE resulttime[DIOIEC60870_5_SIZEMAXTIMELABELTYPEA];

  SetTimeLabelTypeA(timestart, false, false, false, resulttime);    asdu.Add(resulttime, DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);
  SetTimeLabelTypeA(timeend, false, false, false, resulttime);    asdu.Add(resulttime, DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA, &asdu))   return false;
  if(!ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;
      if(!CMD_RequestUserDataClass2(&retasdu, timeout)) return false;

      return true;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_CI_NU_2_Read(DIO_C_CI_NU_2_RESULT* result, int timeout)
* @brief      CMDCCINU2 read
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_Read(DIO_C_CI_NU_2_RESULT* result, int timeout)
{
  if(!result) return false;

  XBUFFER retasdu;

  if(!CMD_RequestUserDataClass2(&retasdu, timeout)) return false;

  DIOIEC60870_5_XEVENT xevent(this, DIOIEC60870_5_XEVENT_TYPE_C_CI_NU_2_READVALUE);
  PostEvent(&xevent);

  XBYTE* buffer = retasdu.Get();
  if(!buffer) return false;

  bool rate   = false;
  bool VI     = false;
  bool summer = false;

  GetTimeLabelTypeA(&buffer[54], rate, VI, summer, (*result->GetDateTime()));

  for(int c=6;c<(int)retasdu.GetSize()-8;c+=6)
    {
      XDWORD value = 0;

      for(int d=0;d<4;d++)
        {
          value |= (XDWORD)buffer[c+1+(3-d)];
          if(d<3) value<<=8;
        }

      result->GetIndexValues()->Add(buffer[c]);
      result->GetValues()->Add(value);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_CI_NU_2_ReadAllValues(int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout)
* @brief      CMDCCINU2 read all values
* @ingroup    DATAIO
* 
* @param[in]  TM : TM value.
* @param[in]  timestart : Timestart pointer to use.
* @param[in]  timeend : Timeend pointer to use.
* @param[in]  results : Output results.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_ReadAllValues(int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout)
{
  if(!timestart) return false;
  if(!timeend)   return false;
  if(!results)   return false;

  bool status = true;

  if(CMD_C_AC_NA_2_IniSession(timeout))
    {
      if(CMD_C_CI_NU_2_Init(TM, (*timestart), (*timeend), timeout))
        {
          DIO_C_CI_NU_2_RESULT* result;
          bool                  status2 = true;

          do{ result = GEN_NEW DIO_C_CI_NU_2_RESULT();
              if(result)
                {
                  status2 = CMD_C_CI_NU_2_Read(result, timeout);
                  if(status2)
                    {
                      results->Add(result);
                    }
                   else GEN_DELETE result;

                } else status2 = false;

            } while(status2);

        } else status = false;

      if(!CMD_C_FS_NA_2_EndSession(timeout)) status = false;

    } else status = false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_CI_NU_2_DeleteResults(XVECTOR<DIO_C_CI_NU_2_RESULT*>* results)
* @brief      CMDCCINU2 GEN_DELETE results
* @ingroup    DATAIO
* 
* @param[in]  results : Output results.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_DeleteResults(XVECTOR<DIO_C_CI_NU_2_RESULT*>* results)
{
  if(!results) return false;

  if(results->IsEmpty()) return false;

  results->DeleteContents();
  results->DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CMD_C_TR_AA_ReadValues(DIO_C_TR_AA_RESULT* result, int timeout)
* @brief      CMDCTRAA read values
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_TR_AA_ReadValues(DIO_C_TR_AA_RESULT* result, int timeout)
{
  if(!result) return false;

  bool status = true;

  if(CMD_C_AC_NA_2_IniSession(timeout))
    {
      XBYTE retfunctioncode;
      bool retisfixmsg;

      XBUFFER asdu;

      asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_TR_AA);                // Identificator
      asdu.Add((XBYTE)0x03);                                        // Object Number
      asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_REQUESTED);      // Transmision Cause
      asdu.Add((XBYTE)addressPM);                                   // Meter Point Address
      asdu.Add((XBYTE)0x00);                                        // Register Address
      asdu.Add((XBYTE)0x00);                                        // Register Address
      asdu.Add((XBYTE)0xC0);                                        // ????
      asdu.Add((XBYTE)0xC1);                                        // ????
      asdu.Add((XBYTE)0xC2);                                        // ????

      if(SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA, &asdu))
        {
          if(ReadMsg(retfunctioncode, retisfixmsg, NULL, timeout))
            {
              if(retfunctioncode!=DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)
                {
                  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
                    {
                      float data        = 0.0f;
                      int   index       = 7;
                      XBYTE  datasigned = 0;
                      int   c;

                      XBUFFER retasdu;
                      if(CMD_RequestUserDataClass2(&retasdu, timeout))
                        {
                          XBYTE* buffer = retasdu.Get();
                          if(buffer)
                            {
                              result->SetActiveImport(GetIntFromBuffer(&buffer[index], 4));  index+=4;
                              result->SetActiveExport(GetIntFromBuffer(&buffer[index], 4));  index+=4;

                              result->SetReactiveQ(1, GetIntFromBuffer(&buffer[index], 4));   index+=4;
                              result->SetReactiveQ(2, GetIntFromBuffer(&buffer[index], 4));   index+=4;
                              result->SetReactiveQ(3, GetIntFromBuffer(&buffer[index], 4));   index+=4;
                              result->SetReactiveQ(4, GetIntFromBuffer(&buffer[index], 4));   index+=4;

                              index+=6;

                              result->SetTotalActivePower  (GetIntFromBuffer(&buffer[index], 3));      index+=3;
                              result->SetTotalReactivePower(GetIntFromBuffer(&buffer[index], 3));      index+=3;

                              data  = (float)(GetIntFromBuffer(&buffer[index], 2)&0x03FF);
                              data /= 1000.0f;
                              result->SetTotalPowerFactor(data);

                              datasigned = buffer[index+1];
                              if(datasigned&0x04) result->SetTotalActivePower  (-result->GetTotalActivePower());
                              if(datasigned&0x08) result->SetTotalReactivePower(-result->GetTotalReactivePower());

                              index+=2;

                              for(c=0;c<3;c++)
                                {
                                  result->SetActivePowerPhase(c+1, GetIntFromBuffer(&buffer[index], 3));      index+=3;
                                  result->SetReactivePowerPhase(c+1, GetIntFromBuffer(&buffer[index], 3));      index+=3;

                                  data  = (float)(GetIntFromBuffer(&buffer[index], 2)&0x03FF);
                                  data /= 1000.0f;
                                  result->SetPowerFactorPhase(c+1, data);

                                  datasigned = buffer[index+1];
                                  if(datasigned&0x04) result->SetActivePowerPhase  (c+1, -result->GetActivePowerPhase(c+1));
                                  if(datasigned&0x08) result->SetReactivePowerPhase(c+1, -result->GetReactivePowerPhase(c+1));

                                  index+=2;
                                }

                              index+=6;

                              for(c=0;c<3;c++)
                                {
                                  data = 0.0f;

                                  data = (float)GetIntFromBuffer(&buffer[index], 3);
                                  data/=10.0f; index+=3;

                                  result->SetCurrentPhase(c+1 , data);

                                  data = (float)GetIntFromBuffer(&buffer[index], 4);
                                  data/=10.0f; index+=4;

                                  result->SetVoltagePhase(c+1 , data);
                                }

                              bool rate   = false;
                              bool VI     = false;
                              bool summer = false;

                              GetTimeLabelTypeA(&buffer[index], rate, VI, summer, (*result->GetDateTime()));

                            } else status = false;

                        } else status = false;

                    } else status = false;

                  DIOIEC60870_5_XEVENT xevent(this, DIOIEC60870_5_XEVENT_TYPE_C_TR_AA_READVALUES);
                  PostEvent(&xevent);

                  if(!CMD_C_FS_NA_2_EndSession(timeout)) status = false;

                } else status = false;

            } else status = false;

        } else status = false;

    } else status = false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::SetTimeLabelTypeA(XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result)
* @brief      Set time label type a
* @ingroup    DATAIO
* 
* @param[in]  time : Time value.
* @param[in]  rate : Rate value.
* @param[in]  VI : VI value.
* @param[in]  summer : Summer value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::SetTimeLabelTypeA(XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result)
{
  memset(result, 0, DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);

  result[0] = time.GetMinutes() & 0x00FF;       // add valid flag and genuine flag
  result[1] = time.GetHours()   & 0x00FF;       // add summer flag
  result[2] = ((time.GetDayOfWeek()%7)&0x03)<<5 | (time.GetDay() & 0x1F);
  result[3] = time.GetMonth()   & 0x0F;
  result[4] = time.GetYear() - 2000;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::SetTimeLabelTypeB(XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result)
* @brief      Set time label type b
* @ingroup    DATAIO
* 
* @param[in]  time : Time value.
* @param[in]  rate : Rate value.
* @param[in]  VI : VI value.
* @param[in]  summer : Summer value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::SetTimeLabelTypeB(XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result)
{
  memset(result, 0, DIOIEC60870_5_SIZEMAXTIMELABELTYPEB);

  XDWORD m = time.GetMilliSeconds() + 1000 * time.GetSeconds();

  result[0] = (m & 0xFF);
  result[1] = (m & 0xFF00)>>8;
  result[2] = time.GetMinutes() & 0x00FF;       // add valid flag and genuine flag
  result[3] = time.GetHours()   & 0x00FF;       // add summer flag
  result[4] = ((time.GetDayOfWeek()%7)&0x03)<<5 | (time.GetDay() & 0x1F);
  result[5] = time.GetMonth()   & 0x0F;
  result[6] = time.GetYear() - 2000;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::GetTimeLabelTypeA(XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time)
* @brief      Get time label type a
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* @param[in]  rate : Rate value.
* @param[in]  VI : VI value.
* @param[in]  summer : Summer value.
* @param[in]  time : Time value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::GetTimeLabelTypeA(XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time)
{
  time.SetToZero();

  rate = (result[0] & 0x40)?true:false;
  VI   = (result[0] & 0x80)?true:false;

  time.SetMinutes(result[0] & 0x3F);
  time.SetHours(result[1] & 0x1F);

  summer = (result[1] & 0x80)?true:false;

  time.SetDay(result[2] & 0x1F);
  time.SetMonth(result[3] & 0x0F);
  time.SetYear(2000 + (result[4] & 0x7F));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::GetTimeLabelTypeB(XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time)
* @brief      Get time label type b
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* @param[in]  rate : Rate value.
* @param[in]  VI : VI value.
* @param[in]  summer : Summer value.
* @param[in]  time : Time value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::GetTimeLabelTypeB(XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time)
{
  time.SetToZero();

  XDWORD m = result[0] | result[1]<<8;

  time.SetSeconds(m/1000);
  time.SetMilliSeconds(m - (time.GetSeconds()*1000));

  rate = (result[2] & 0x40)?true:false;
  VI   = (result[2] & 0x80)?true:false;

  time.SetMinutes(result[2] & 0x3F);
  time.SetHours(result[3] & 0x1F);

  summer = (result[3] & 0x80)?true:false;

  time.SetDay(result[4] & 0x1F);

  //XBYTE dayofweek = (result[4] & 0xE0 ) >> 5;
  //if(dayofweek) dayofweek = (dayofweek + 1)%7;
  //time.SetDayOfWeek((XDATETIME_DAYWEEK)dayofweek);

  time.SetMonth(result[5] & 0x0F);

  time.SetYear(2000 + (result[6] & 0x7F));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::CancelOperations()
* @brief      Cancel operations
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::CancelOperations()
{
  canceloperations = true;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOIEC60870_5::Disconnect()
* @brief      Disconnect
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOIEC60870_5::Disconnect()
{
  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();

      diostream = NULL;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::IsValidFrameCount(XBYTE functioncode)
* @brief      Is valid frame count
* @ingroup    DATAIO
* 
* @param[in]  functioncode : Functioncode value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::IsValidFrameCount(XBYTE functioncode)
{
  bool isFCVon = false;

  switch(functioncode)
    {
      case DIOIEC60870_5_MASTERFUNCRESETREMOTELINK        : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCRESETUSERPROCESS      : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCUSERDATA               : isFCVon = true;   break;
      case DIOIEC60870_5_MASTERFUNCREQUESTSTATUSLINK      : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS1  : isFCVon = true;   break;
      case DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS2  : isFCVon = true;   break;
    }

  return isFCVon;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::SendMsgVar(XBYTE functioncode, XBUFFER* dataASDU)
* @brief      Send msg var
* @ingroup    DATAIO
* 
* @param[in]  functioncode : Functioncode value.
* @param[in]  dataASDU : Data ASDU pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::SendMsgVar(XBYTE functioncode, XBUFFER* dataASDU)
{
  if(!diostream) return false;
  if(diostream->GetStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  if(!dataASDU)  return false;

  XBUFFER xbuffer;
  XBYTE   controlfield = 0;
  
  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTVAR);
  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTVAR);

  controlfield  = 0x00;
  controlfield |= DIOIEC60870_5_CONTROLDIRPRIMARY;

  if(IsValidFrameCount(functioncode))
    {
      controlfield |= FCBstate?DIOIEC60870_5_CONTROLFCB:0x00;
      controlfield |= DIOIEC60870_5_CONTROLFCV;

      FCBstate = !FCBstate;
    }

  controlfield |= (functioncode&0x0F);
  xbuffer.Add(controlfield);

  if(!havelongaddress)
    {
      xbuffer.Add((XBYTE)addressfield);
    }
   else
    {
      XWORD _addressfield = addressfield;

      if(inlittleendian) SWAPWORD(_addressfield);
      xbuffer.Add((XWORD)_addressfield);
    }

  xbuffer.Add(dataASDU);

  xbuffer.Set((XBYTE)(dataASDU->GetSize()+3), 1);
  xbuffer.Set((XBYTE)(dataASDU->GetSize()+3), 2);

  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKEND);

  xbuffer.Set(CalculateCheckSum(xbuffer), (xbuffer.GetSize()-2));

  if(!diostream->Write(xbuffer.Get(), xbuffer.GetSize())) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::SendMsgFix(XBYTE functioncode)
* @brief      Send msg fix
* @ingroup    DATAIO
* 
* @param[in]  functioncode : Functioncode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::SendMsgFix(XBYTE functioncode)
{
  if(!diostream) return false;
  if(diostream->GetStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  XBUFFER  xbuffer;
  XBYTE    controlfield = 0;

  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTFIX);

  controlfield  = 0x00;
  controlfield |= DIOIEC60870_5_CONTROLDIRPRIMARY;

  if(IsValidFrameCount(functioncode))
    {
      controlfield |= FCBstate?DIOIEC60870_5_CONTROLFCB:0x00;
      controlfield |= DIOIEC60870_5_CONTROLFCV;

      FCBstate = !FCBstate;
    }

  controlfield |= (functioncode&0x0F);
  xbuffer.Add(controlfield);

  if(!havelongaddress)
    {
      xbuffer.Add((XBYTE)addressfield);
    }
   else
    {
      XWORD _addressfield = addressfield;

      if(inlittleendian) SWAPWORD(_addressfield);
      xbuffer.Add((XWORD)_addressfield);
    }

  xbuffer.Add((XBYTE)0); // Temporal CheckSUM

  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKEND);

  xbuffer.Get()[4] = CalculateCheckSum(xbuffer);

  if(!diostream->Write(xbuffer.Get(), xbuffer.GetSize())) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::WaitToReadMsg(XDWORD size, int timeout)
* @brief      Wait to read msg
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::WaitToReadMsg(XDWORD size, int timeout)
{
  XDWORD actualsize = 0;

  if(!xtimer) return false;
  xtimer->Reset();

  do{
      if(canceloperations)                                         return false;

      if(!diostream)                                               return false;
      if(diostream->GetStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;

      actualsize = diostream->GetInXBuffer()->GetSize();
      if((int)xtimer->GetMeasureSeconds() >= timeout) return false;

      GEN_XSLEEP.MilliSeconds(10);

      if(actualsize >= size) return true;

    } while(!actualsize);

  if(!actualsize) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOIEC60870_5::ReadMsg(XBYTE& retfunctioncode, bool& retisfixmsg, XBUFFER* retASDU, int timeout)
* @brief      Read msg
* @ingroup    DATAIO
* 
* @param[in]  retfunctioncode : Retfunctioncode value.
* @param[in]  retisfixmsg : Retisfixmsg value.
* @param[in]  retASDU : Ret ASDU pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOIEC60870_5::ReadMsg(XBYTE& retfunctioncode, bool& retisfixmsg, XBUFFER* retASDU, int timeout)
{
  if(!diostream) return false;
  if(diostream->GetStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  bool status  = false;

  XBUFFER xbuffer((XDWORD)DIOIEC60870_5_MAXSIZE, true);

  if(!WaitToReadMsg(DIOIEC60870_5_MAXSIZE, timeout))
    {
      return false;
    }

  int indexbufferread = 0;

  xtimer->Reset();

  while(1)
    {
      if(canceloperations) return false;

      if((int)xtimer->GetMeasureSeconds()>=timeout) return false;

      if((indexbufferread+DIOIEC60870_5_SIZEMSGFIX)>(int)diostream->GetInXBuffer()->GetSize()) break;

      XBYTE* buffer = &(diostream->GetInXBuffer()->Get()[indexbufferread]);

      switch(buffer[0])
        {
          case DIOIEC60870_5_MARKSTARTFIX : { XBYTE checksum = CalculateCheckSum(buffer, DIOIEC60870_5_SIZEMSGFIX);
                                              if(checksum!=buffer[4])  break;

                                              XWORD _addressfield;

                                              _addressfield   = buffer[2];
                                              _addressfield <<= 8;
                                              _addressfield  |= buffer[3];

                                              if(inlittleendian) SWAPWORD(_addressfield);

                                              if(_addressfield != addressfield)
                                                {
                                                  indexbufferread += DIOIEC60870_5_SIZEMSGFIX;
                                                  continue;
                                                }

                                              XBYTE controlfield = buffer[1];
                                              if(controlfield&DIOIEC60870_5_CONTROLDIRPRIMARY)  break;

                                              retfunctioncode   = (controlfield&0x0f);
                                              retisfixmsg       = true;
                                              diostream->GetInXBuffer()->Extract(NULL, indexbufferread, DIOIEC60870_5_SIZEMSGFIX);
                                            }

                                            status = true;
                                            break;

          case DIOIEC60870_5_MARKSTARTVAR : { XBYTE sizeasdu  = buffer[1];
                                              XBYTE checksum = CalculateCheckSum(buffer, sizeasdu+4+2);
                                              if(checksum!=buffer[sizeasdu+4])  break;

                                              XWORD _addressfield;

                                              _addressfield   = buffer[5];
                                              _addressfield <<= 8;
                                              _addressfield  |= buffer[6];

                                              if(inlittleendian) SWAPWORD(_addressfield);

                                              if(_addressfield != addressfield)
                                                {
                                                  indexbufferread += DIOIEC60870_5_SIZEMSGFIX;
                                                  continue;
                                                }

                                              XBYTE controlfield = buffer[4];
                                              if(controlfield&DIOIEC60870_5_CONTROLDIRPRIMARY) break;

                                              retfunctioncode   = (controlfield&0x0f);
                                              retisfixmsg       = false;

                                              if(retASDU) retASDU->Add(&buffer[7], sizeasdu-3);

                                              diostream->GetInXBuffer()->Extract(NULL, indexbufferread, sizeasdu+4+2);
                                            }

                                            status = true;
                                            break;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOIEC60870_5::CalculateCheckSum(XBYTE* buffer, XDWORD sizebuffer)
* @brief      Calculate check sum
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  sizebuffer : Sizebuffer value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOIEC60870_5::CalculateCheckSum(XBYTE* buffer, XDWORD sizebuffer)
{
  if(!buffer)       return 0;

  XBYTE csum    = 0;
  int  start  = 0;
  int  size   = 0;

  switch(buffer[0])
    {
      case DIOIEC60870_5_MARKSTARTFIX : if(buffer[4]==DIOIEC60870_5_MARKEND)
                                          {
                                            start = 1;
                                            size  = 2;
                                          }

                                        if(buffer[5]==DIOIEC60870_5_MARKEND)
                                          {
                                            start = 1;
                                            size  = 3;
                                          }
                                        break;

      case DIOIEC60870_5_MARKSTARTVAR : if(buffer[3]==DIOIEC60870_5_MARKSTARTVAR)
                                          {
                                            start = 4;
                                            size = sizebuffer-start-2;
                                          }
                                        break;
    }


  for(int c=0;c<size;c++)
    {
      csum += buffer[start+c];
    }

  return csum;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOIEC60870_5::CalculateCheckSum(XBUFFER& xbuffer)
* @brief      Calculate check sum
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOIEC60870_5::CalculateCheckSum(XBUFFER& xbuffer)
{
  return CalculateCheckSum(xbuffer.Get(), xbuffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOIEC60870_5::GetIntFromBuffer(XBYTE* buffer, XDWORD size)
* @brief      Get int from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOIEC60870_5::GetIntFromBuffer(XBYTE* buffer, XDWORD size)
{
  XDWORD value = 0;

  for(XDWORD d=0; d<size; d++)
    {
      value |= (XDWORD)buffer[((size-1)-d)];
      if(d<(size-1)) value<<=8;
    }

  return (int)value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOIEC60870_5::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOIEC60870_5::Clean()
{
  diostream         = NULL;
  xtimer            = NULL;

  addressfield      = 0;
  addressPM         = 0;
  keyPM             = 0;

  FCBstate          = true;

  inlittleendian    = true;
  havelongaddress   = true;

  canceloperations  = false;
}





