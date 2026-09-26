/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOGPIO.h
* 
* @class      DIOGPIO
* @brief      Data Input/Output GPIO (General Purpose Input/Output)
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

#include "XBase.h"
#include "XVector.h"
#include "XSystem.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOGPIO_PINSGROUP
{
  DIOGPIO_PINSGROUP_NONE  = 0 ,
  DIOGPIO_PINSGROUP_A         ,
  DIOGPIO_PINSGROUP_B         ,
  DIOGPIO_PINSGROUP_C         ,
  DIOGPIO_PINSGROUP_D         ,
  DIOGPIO_PINSGROUP_E         ,
  DIOGPIO_PINSGROUP_F         ,
};

enum DIOGPIO_PULLMODE
{
   DIOGPIO_PULLMODE_OFF   = 0 ,
   DIOGPIO_PULLMODE_UP        ,
   DIOGPIO_PULLMODE_DOWN      ,
};

#define DIOGPIO_ID_NOTDEFINED       0
#define DIOGPIO_INVALID             0x8000000000000000
#define DIOGPIO_PINNOTUSE           0x80000000

#define DIOGPIO_MODE_NONE           0x0000
#define DIOGPIO_MODE_INPUT          0x0001
#define DIOGPIO_MODE_OUTPUT         0x0002
#define DIOGPIO_MODE_PWM            0x0004
#define DIOGPIO_MODE_INTERRUPT      0x0008

class DIOGPIO_ENTRY;

typedef void (*DIOGPIO_INT_FUNCPTR)(void* param);

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOGPIO_ENTRY
{
  public:
                                        DIOGPIO_ENTRY                   ();
    virtual                            ~DIOGPIO_ENTRY                   ();

    XDWORD                              GetID                           ();   
    void                                SetID                           (XDWORD ID);   

    XQWORD                              GetGPIO                         ();   
    void                                SetGPIO                         (XQWORD GPIO);   

    DIOGPIO_PINSGROUP                   GetGroup                        ();    
    void                                SetGroup                        (DIOGPIO_PINSGROUP group);    

    XWORD                               GetPin                          ();   
    void                                SetPin                          (XWORD pin);   

    XWORD                               GetMode                         ();    
    void                                SetMode                         (XWORD mode);    
 
    DIOGPIO_PULLMODE                    GetPullMode                     ();    
    void                                SetPullMode                     (DIOGPIO_PULLMODE pullmode);  

    DIOGPIO_INT_FUNCPTR                 GetIntFunctionPointer           ();
    void                                SetIntFunctionPointer           (DIOGPIO_INT_FUNCPTR int_funcptr);
    
    void*                               GetIntParamPointer              ();
    void                                SetIntParamPointer              (void* int_paramptr);

    XDWORD                              GetIntIRQ                       ();
    void                                SetIntIRQ                       (XDWORD int_IRQ);

    XDWORD                              GetIntPriority                  ();
    void                                SetIntPriority                  (XDWORD int_priority);

    void                                DebugTrace                      ();
 
  private:

    XDWORD                              ID;
    XQWORD                              GPIO;   
    DIOGPIO_PINSGROUP                   group; 
    XWORD                               pin;
    XWORD                               mode;  
    DIOGPIO_PULLMODE                    pullmode;

    DIOGPIO_INT_FUNCPTR                 int_funcptr;
    void*                               int_paramptr;
    XDWORD                              int_IRQ;
    XDWORD                              int_priority;        
                                                                   
    void                                Clean                           ();
};

class DIOGPIO
{
  public:
                                        DIOGPIO                         ();
    virtual                            ~DIOGPIO                         ();

    static bool                         GetIsInstanced                  ();
    static DIOGPIO&                     GetInstance                     ();
    static bool                         SetInstance                     (DIOGPIO* instance);
    static bool                         DelInstance                     ();

    XVECTOR<DIOGPIO_ENTRY*>*            GPIOEntry_GetAll                ();
    DIOGPIO_ENTRY*                      GPIOEntry_CreateByPin           (XDWORD ID, XWORD pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_NONE);  
    DIOGPIO_ENTRY*                      GPIOEntry_CreateByGPIO          (XDWORD ID, XQWORD GPIO);  
    DIOGPIO_ENTRY*                      GPIOEntry_Create                (XDWORD ID, XWORD pin, XQWORD GPIO);  
    
    DIOGPIO_ENTRY*                      GPIOEntry_GetByID               (XDWORD ID);
    DIOGPIO_ENTRY*                      GPIOEntry_GetByPin              (XWORD pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_NONE);    
    DIOGPIO_ENTRY*                      GPIOEntry_GetInterruptByPin     (XWORD pin);    
    DIOGPIO_ENTRY*                      GPIOEntry_GetByGPIO             (XQWORD GPIO);    
    bool                                GPIOEntry_DeleteAll             ();  

    bool                                GPIOEntry_SetIDByPin            (XDWORD ID, XWORD pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_NONE);
    bool                                GPIOEntry_SetIDByGPIO           (XDWORD ID, XQWORD GPIO);

    virtual bool                        Ini                             ();

    XWORD                               GetMode                         (XDWORD ID);  
    bool                                SetMode                         (XDWORD ID, XWORD mode);  

    bool                                GetValue                        (XDWORD ID);
    bool                                SetValue                        (XDWORD ID, bool value);

    virtual XWORD                       GetMode                         (DIOGPIO_ENTRY* entry);  
    virtual bool                        SetMode                         (DIOGPIO_ENTRY* entry, XWORD mode);  

    virtual bool                        GetValue                        (DIOGPIO_ENTRY* entry);
    virtual bool                        SetValue                        (DIOGPIO_ENTRY* entry, bool value);

    virtual bool                        End                             ();


  private:

    void                                Clean                       ();

    static DIOGPIO*                     instance;

    XVECTOR<DIOGPIO_ENTRY*>             GPIOentrys;
 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





