/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32System.h
*
* @class      XSTM32SYSTEM
* @brief      eXtended utils STM32 system class
* @ingroup    PLATFORM_STM32
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

#include "XSystem.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XSTM32SYSTEM : public XSYSTEM
{
  public:
													  XSTM32SYSTEM                ();
    virtual 				       ~XSTM32SYSTEM                ();

    XSYSTEM_HARDWARETYPE    GetTypeHardware             (int* revision = NULL);
    
    XSYSTEM_PLATFORM        GetPlatform                 (XSTRING* namestring = NULL);

    bool                    GetMemoryInfo               (XDWORD& total, XDWORD& free);

    bool                    ShutDown                    (XSYSTEM_CHANGESTATUSTYPE type);

  private:

    void                    Clean                       ();
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


