/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXSimulate.h
* 
* @class      INPLINUXSIMULATE
* @brief      LINUX Input Simulate
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

#include "INPSimulate.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class INPLINUXSIMULATE : public INPSIMULATE
{
  public:
                    INPLINUXSIMULATE            ();
    virtual        ~INPLINUXSIMULATE            ();

    bool            Key_Press                   (XBYTE code);
    bool            Key_UnPress                 (XBYTE code);
    bool            Key_Click                   (XBYTE code, int pressuretime = 100);
    bool            Mouse_SetPos                (int x, int y);
    bool            Mouse_Click                 (int x, int y);

  protected:

  private:

    bool            UInput_Ini                  ();
    bool            UInput_End                  ();
    bool            UInput_Emit                 (XWORD type, XWORD code, int value);
    bool            UInput_Syn                  ();
    bool            Key_Event                   (XBYTE code, bool pressed);
    XWORD           GetLinuxKeyByVirtualKey     (XBYTE code);

    #ifdef LINUX_X11_ACTIVE
    bool            Mouse_WarpX11               (int x, int y);
    #endif

    int             uinputfd;

    void            Clean                       ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
