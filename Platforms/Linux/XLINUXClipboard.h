/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXClipboard.h
* 
* @class      XLINUXCLIPBOARD
* @brief      LINUX eXtended Utils Clipboard class (X11 CLIPBOARD / UTF8_STRING)
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

#ifdef LINUX_X11_ACTIVE
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

#include "XBuffer.h"
#include "XClipboard.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XLINUXCLIPBOARD_GETTEXT_TIMEOUTMS   500

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XLINUXCLIPBOARD : public XCLIPBOARD
{
  public:

                                XLINUXCLIPBOARD                   ();
    virtual                    ~XLINUXCLIPBOARD                   ();

    bool                        SetText                           (XSTRING& text);
    bool                        GetText                           (XSTRING& text);
    bool                        Clear                             ();
    bool                        HasText                           ();

    virtual void                Update                            ();

  private:

    bool                        EnsureDisplay                     ();
    void                        CloseDisplay                      ();
    void                        ProcessPendingEvents              ();
    bool                        WaitSelectionNotify               (Atom property, XBUFFER& utf8, int timeoutms);

    void                        Clean                             ();

#ifdef LINUX_X11_ACTIVE
    Display*                    display;
    Window                      window;
    Atom                        atomclipboard;
    Atom                        atomtargets;
    Atom                        atomutf8;
    Atom                        atomstring;
    Atom                        atomproperty;
#endif

    XBUFFER                     ownedutf8;
    bool                        ownclipboard;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




