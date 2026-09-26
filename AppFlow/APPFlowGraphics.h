/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowGraphics.h
* 
* @class      APPFLOWGRAPHICS
* @brief      Application Flow Graphics class
* @ingroup    GRAPHIC
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

#ifdef GRP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

#include "APPFlowBase.h"
#include "APPFlowConsole.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum APPFLOWGRAPHICS_INIOPTIONS
{
  APPFLOWGRAPHICS_INIOPTION_NONE                = 0       ,
  APPFLOWGRAPHICS_INIOPTION_CREATEMAINSCREEN    = 1 << 1  ,
  APPFLOWGRAPHICS_INIOPTION_SHOWMAINSCREEN      = 1 << 2  ,
  APPFLOWGRAPHICS_INIOPTION_INPUT               = 1 << 3
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSCREEN;
class GRP2DCANVAS;
class INPDEVICE;

class APPFLOWGRAPHICS : public APPFLOWCONSOLE, public XOBSERVER
{
  public:
                              APPFLOWGRAPHICS                     ();
    virtual                  ~APPFLOWGRAPHICS                     ();

    bool                      Ini                                 (XVECTOR<XSTRING*>* execparams);
    bool                      End                                 ();

    XDWORD                    GetInitOptions                      ();
    void                      SetInitOptions                      (XDWORD initoptions);

    bool                      CreateMainScreenProcess             (bool show);
    bool                      DeleteMainScreenProcess             ();

    GRPSCREEN*                GetMainScreen                       ();
    void                      SetMainScreen                       (GRPSCREEN* mainscreen);

    GRP2DCANVAS*                GetScreenCanvas                     (GRPSCREEN* screen, int viewportindex);

    #ifdef INP_ACTIVE
    INPDEVICE*                GetInputKeyboard                    ();
    INPDEVICE*                GetInputMouse                       ();
    #endif

    virtual bool              AppProc_BeforeCreateMainScreen      (GRPSCREEN* mainscreen);
    virtual bool              AppProc_BeforeCreateMainCanvas      (GRP2DCANVAS* maincanvas);

  private:

    void                      Clean                               ();

    XDWORD                    initoptions;
    GRPSCREEN*                mainscreen;

    #ifdef INP_ACTIVE
    INPDEVICE*                keyboard;
    INPDEVICE*                mouse;
    INPDEVICE*                touchscreen;
    #endif
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif


