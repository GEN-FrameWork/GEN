/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXGPIO.h
* 
* @class      DIOLINUXGPIO
* @brief      LINUX Data Input/Output GPIO (General Purpose Input/Output) 
* @ingroup    PLATFORM_LINUX
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

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOLINUXGPIO_PATH                     __L("/sys/class/gpio")

#define DIOLINUXGPIO_BANK_BIT(BANK, BIT)      ((BANK - 1) * 32 + BIT)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOLINUXGPIO : public DIOGPIO
{
  public:
                                DIOLINUXGPIO                ();
    virtual                    ~DIOLINUXGPIO                ();

    virtual bool                Ini                         ();
    
    virtual bool                SetMode                     (DIOGPIO_ENTRY* entry, XWORD mode);  

    virtual bool                GetValue                    (DIOGPIO_ENTRY* entry);
    virtual bool                SetValue                    (DIOGPIO_ENTRY* entry, bool value);

    virtual bool                End                         ();

   
  private:

    bool                        GPIOExport_Add              (XDWORD GPIO);
    bool                        GPIOExport_IsExport         (XDWORD GPIO);
    XVECTOR<XDWORD>*            GPIOExport_Get              ();           
    bool                        GPIOExport_DeleteAll        ();    
    
    bool                        GPIO_Export                 (XDWORD GPIO, bool isexport = false);
    bool                        GPIO_SetDirection           (XDWORD GPIO, bool isinput = true);
    bool                        GPIO_GetData                (XDWORD GPIO);
    bool                        GPIO_SetData                (XDWORD GPIO, bool on);
   
    void                        Clean                       (); 

    XVECTOR<XDWORD>             GPIOexports;  
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





