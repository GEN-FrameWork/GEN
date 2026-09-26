/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXConsole.h
* 
* @class      XLINUXCONSOLE
* @brief      LINUX eXtended Utils Console class
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

#include "XConsole.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XLINUXCONSOLE : public XCONSOLE
{
  public:
                          XLINUXCONSOLE         ();
    virtual              ~XLINUXCONSOLE         ();

    bool                  GetSizeText           (int& columns, int& rows);

    XCONSOLE_SYMBOLSUSED  GetSymbolsUsed        ();

    bool                  Maximize              ();
    bool                  Minimize              ();
    bool                  Hide                  ();
    bool                  IsHide                ();
    bool                  Print                 (XCHAR* string);
    bool                  Clear                 (bool fill = true);

    bool                  KBHit                 (void);
    int                   GetChar               ();

  private:

    void                  RawModeEnable         ();
    void                  RawModeDisable        ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





