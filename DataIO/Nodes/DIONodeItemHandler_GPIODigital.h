/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItemHandler_GPIODigital.h
* 
* @class      DIONODEITEMHANDLER_GPIODIGITAL
* @brief      Data Input/Output Node Item Handler GPIO Digital class
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

#include "XList.h"
#include "DIONodeItemHandler.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;

class DIONODEITEMHANDLER_ENTRYGPIO
{
  public: 
                                            DIONODEITEMHANDLER_ENTRYGPIO      ();
                                            DIONODEITEMHANDLER_ENTRYGPIO      (XDWORD entryID, bool writemode, int GPIO = DIONODEITEMHANDLER_INVALIDPARAM, int pin = DIONODEITEMHANDLER_INVALIDPARAM, XCHAR* description = __L(""));
    virtual                                ~DIONODEITEMHANDLER_ENTRYGPIO      ();

    XDWORD                                  GetEntryID                        ();
    void                                    SetEntryID                        (XDWORD entryID);

    bool                                    GetWriteMode                      ();
    void                                    SetWriteMode                      (bool writemode);

    int                                     GetGPIO                           (); 
    void                                    SetGPIO                           (int GPIO = DIONODEITEMHANDLER_INVALIDPARAM); 

    int                                     GetPin                            ();
    void                                    SetPin                            (int pin = DIONODEITEMHANDLER_INVALIDPARAM);

    XSTRING*                                GetDescription                    ();

    XQWORD                                  GetTimeLastActivation             (); 
    XQWORD                                  GetTimeLastDeactivation           (); 

    bool                                    CopyFrom                          (DIONODEITEMHANDLER_ENTRYGPIO* entryGPIO);
    bool                                    CopyTo                            (DIONODEITEMHANDLER_ENTRYGPIO* entryGPIO);

    void                                    AdjustTimeInChange                (bool status);

  protected:

    XDWORD                                  entryID;
    bool                                    writemode;
    int                                     GPIO; 
    int                                     pin;
    XSTRING                                 description;

    XQWORD                                  time_last_activation;
    XQWORD                                  time_last_deactivation;

  private:
    
    void                                    Clean                             ();     

    XTIMER*                                 timerstatus;
};


class DIONODEITEMHANDLER_GPIODIGITAL : public DIONODEITEMHANDLER
{
  public:
                                            DIONODEITEMHANDLER_GPIODIGITAL    (XLIST<DIONODEITEMHANDLER_ENTRYGPIO*>* entrysGPIO);
    virtual                                ~DIONODEITEMHANDLER_GPIODIGITAL    ();

    virtual bool                            Open                              ();
    virtual bool                            Update                            ();
    bool                                    Close                             ();

    bool                                    SetNodeItem                       (DIONODEITEM* nodeitem);  
      
  private:
  
    void                                    Clean                             (); 

    XLIST<DIONODEITEMHANDLER_ENTRYGPIO*>    entrysGPIO;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







