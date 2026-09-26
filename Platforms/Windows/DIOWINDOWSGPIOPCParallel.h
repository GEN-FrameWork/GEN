/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSGPIOPCParallel.h
* 
* @class      DIOWINDOWSGPIOPCPARALLEL
* @brief      WINDOWS Data Input/Output GPIO (General Purpose Input/Output) PC Parallel
* @ingroup    PLATFORM_WINDOWS
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

#include <stdio.h>
#include <Windows.h>

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOWINDOWSGPIOPCPARALLEL_PORT1                0x378
#define DIOWINDOWSGPIOPCPARALLEL_PORT2                0x278

typedef void  (__stdcall* DIOWGPIOOUT)               (short, short);
typedef short (__stdcall* DIOWGPIOINP)               (short);
typedef BOOL  (__stdcall* DIOWGPIOISDRIVEROPEN)      (void);
typedef BOOL  (__stdcall* DIOWGPIOISDRIVER64BITS)    (void);

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATH;

class DIOWINDOWSGPIOPCPARALLEL : public DIOGPIO
{
  public:
                                DIOWINDOWSGPIOPCPARALLEL      ();
    virtual                    ~DIOWINDOWSGPIOPCPARALLEL      ();

    XPATH*                      GetPath                       ();
    void                        SetPath                       (XPATH* xpath);

    XWORD                       GetParallelPort               ();
    void                        SetParallelPort               (XWORD port = DIOWINDOWSGPIOPCPARALLEL_PORT1);
    
    bool                        Ini                           ();   
    
    XWORD                       GetMode                       (DIOGPIO_ENTRY* entry);  
    bool                        SetMode                       (DIOGPIO_ENTRY* entry, XWORD mode);  

    bool                        GetValue                      (DIOGPIO_ENTRY* entry);
    bool                        SetValue                      (DIOGPIO_ENTRY* entry, bool value);

    bool                        End                           ();

  private:

    
    bool                        PC_Get                        (XDWORD nport, XBYTE& data);
    bool                        PC_Set                        (XDWORD nport, XBYTE data);
    
    void                        Clean                         ();
    
    XPATH*                      xpath;
    XWORD                       port;    
    HINSTANCE                   handleDLL;
    DIOWGPIOOUT                 outport;
    DIOWGPIOINP                 inpport;
    DIOWGPIOISDRIVEROPEN        isdriveropen;
    DIOWGPIOISDRIVER64BITS      isdriver64bit;
    XBYTE                       dataport;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




