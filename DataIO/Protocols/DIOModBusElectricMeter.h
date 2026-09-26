/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOModBusElectricMeter.h
* 
* @class      DIOMODBUSELECTRICMETER
* @brief      Data Input/Output ModBus Electric Meter class
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

#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XEvent.h"
#include "XSubject.h"

#include "DIOIEC60870_5.h"
#include "DIOModBus_Client.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOMODBUSELECTRICMETERXEVENT_TYPE
{
  DIOMODBUSELECTRICMETERXEVENT_TYPE_UNKNOW                        = XEVENT_TYPE_DIOMODBUSELECTRICMETER + 1 ,
  DIOMODBUSELECTRICMETERXEVENT_TYPE_C_CI_NU_2_READVALUE                                                    ,
  DIOMODBUSELECTRICMETERXEVENT_TYPE_C_TR_AA_READVALUES                                                     ,
};

enum DIOMODBUSELECTRICMETER_REGISTERTYPE
{
  DIOMODBUSELECTRICMETER_REGISTERTYPE_UNKNOWN                     = 0 ,

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEIMPORT                    ,   // Activa Importacion
  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEEXPORT                    ,   // Activa Exportacion

  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ1                      ,   // Reactiva Q1
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ2                      ,   // Reactiva Q2
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ3                      ,   // Reactiva Q3
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ4                      ,   // Reactiva Q4

  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALACTIVEPOWER                ,   // Potencia Activa Total
  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALREACTIVEPOWER              ,   // Potencia Reactiva Total
  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALPOWERFACTOR                ,   // Factor de Potencia Total

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEI               ,   // Potencia Activa Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEI             ,   // Potencia Reactiva Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORI               ,   // Factor de Potencia Fase I

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEII              ,   // Potencia Activa Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEII            ,   // Potencia Reactiva Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORII              ,   // Factor de Potencia Fase II

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEIII             ,   // Potencia Activa Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEIII           ,   // Potencia Reactiva Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORIII             ,   // Factor de Potencia Fase III

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEI                   ,   // Tension Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEI                   ,   // Intensidad Fase I

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEII                  ,   // Tension Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEII                  ,   // Intensidad Fase II

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEIII                 ,   // Tension Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEIII                 ,   // Intensidad Fase III
};

#define DIOMODBUSELECTRICMETER_CONNECTTIMEOUT            5       // Seconds
#define DIOMODBUSELECTRICMETER_READTIMEOUT              15       // Seconds

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOSTREAM;

class DIOMODBUSELECTRICMETERXEVENT : public XEVENT
{
  public:
                              DIOMODBUSELECTRICMETERXEVENT        (XSUBJECT* subject, XDWORD type = DIOMODBUSELECTRICMETERXEVENT_TYPE_UNKNOW);
    virtual                  ~DIOMODBUSELECTRICMETERXEVENT        ();

  private:

    void                      Clean                               ();
};

class DIOMODBUSELECTRICMETER  : public XSUBJECT
{
  public:
                              DIOMODBUSELECTRICMETER              (DIOSTREAM* diostream, DIOMODBUS_CLIENTMODE mode = DIOMODBUS_CLIENTMODE_RTU);
    virtual                  ~DIOMODBUSELECTRICMETER              ();

    DIOMODBUS_CLIENT*         GetModBusProtocol                   ();

    bool                      Connect                             (XBYTE unit, bool inlittleendian, int timeout = DIOMODBUSELECTRICMETER_CONNECTTIMEOUT);

    bool                      CMD_C_CI_NU_2_ReadAllValues         (int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout = DIOMODBUSELECTRICMETER_READTIMEOUT);
    bool                      CMD_C_CI_NU_2_DeleteResults         (XVECTOR<DIO_C_CI_NU_2_RESULT*>* results);

    bool                      CMD_C_TR_AA_ReadValues              (DIO_C_TR_AA_RESULT* result, int timeout = DIOMODBUSELECTRICMETER_READTIMEOUT);

    bool                      CancelOperations                    ();

    void                      Disconnect                          ();

  protected:

    XMAP<XDWORD,XDWORD>       registermap;

  private:
    
    bool                      ReadRegister                        (int registerID, int nwords, int divisor, int timeout, float& result);
    bool                      ReadRegister                        (int registerID, int nwords, int timeout, int& result);
    void                      Clean                               ();

    XTIMER*                   xtimer;

    XBYTE                     unit;
    bool                      inlittleendian;

    int                       lastactiveenergysum;
    int                       lastreactiveenergysum;

    DIOMODBUS_CLIENT*         modbusprotocol;

    bool                      canceloperations;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






