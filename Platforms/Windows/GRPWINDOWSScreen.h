/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPWINDOWSScreen.h
* 
* @class      GRPWINDOWSSCREEN
* @brief      WINDOWS Graphics Screen class
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

#include <Windows.h>
#include <winuser.h>

#include "XString.h"

#include "GRPScreen.h"

#ifdef GRP_OPENGL_ACTIVE
class GRPWINDOWSBLITGLES;
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPWINDOWSSCREEN_NAMECLASS  __L("GEN_Windows_Screen")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPWINDOWSSCREEN : public GRPSCREEN
{
  public:
                                          GRPWINDOWSSCREEN                    ();
    virtual                              ~GRPWINDOWSSCREEN                    ();

    bool                                  Create                              (bool show);

    bool                                  Update                              ();
    bool                                  Update                              (GRP2DCANVAS* canvas);
    bool                                  UpdateTransparent                   (GRP2DCANVAS* canvas);

    bool                                  Get_Position                        (int &x, int &y);
    bool                                  Set_Position                        (int x, int y);

    bool                                  GetClientSize                       (int& width, int& height);

    #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
    bool                                  GetCursorDesktopPosition            (int& x, int& y);

    bool                                  BeginCFGChromesDrag                 ();
    bool                                  EndCFGChromesDrag                   ();
    #endif

    bool                                  Delete                              ();

    bool                                  Resize                              (int width, int height);
    
    bool                                  Show                                (bool active);
    bool                                  ShowCursor                          (bool active);
    
    void*                                 GetHandle                           ();
    void                                  SetHandle                           (void* handle_window);

    bool                                  Set_Focus                           ();

    bool                                  Minimize                            (bool active);
    bool                                  Maximize                            (bool active);

    GRPBITMAP*                            CaptureContent                      (GRPRECTINT* rect = NULL, void* handle_window = NULL);
    
    void*                                 GetDesktopHandle                    ();
    void*                                 GetShellHandle                      ();
    
    void*                                 GetHDC                              ();
    void                                  SetHDC                              (HDC hdc);

    BITMAPINFO*                           GetHInfo                            ();

    #ifdef GRP_OPENGL_ACTIVE
    GRPWINDOWSBLITGLES*                   GetBlitGLES                         ();
    #endif
    
  private:

    int                                   GetTaskbarHeight                    ();

    bool                                  Create_Window                       (bool show);

    void                                  Chromes_ApplyStyle                  (DWORD& style, DWORD& exstyle);
    void                                  Chromes_ApplyPostCreate             ();

    void                                  ApplyResizeLimits                   (MINMAXINFO* minmaxinfo);
    bool                                  IsAboveViewportMinimumSize          ();
    void                                  ClientSizeToWindowSize              (int clientwidth, int clientheight, DWORD style, int& windowwidth, int& windowheight);

    void                                  Uncloak                             ();

    static  LRESULT CALLBACK              BaseWndProc                         (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void                                  Clean                               ();

    WNDCLASSEX                            wndclass;
    XSTRING                               classname;
    XSTRING                               title;

    HINSTANCE                             hinstance;
    HWND                                  hwnd;
    HDC                                   hdc;







    bool                                   windowcloaked;
    bool                                   maximize_restore_valid;
    int                                    maximize_restore_x;
    int                                    maximize_restore_y;
    int                                    maximize_restore_w;
    int                                    maximize_restore_h;

    DEVMODE                               devmode;

    BITMAPINFO                            hinfo;

    #ifdef GRP_OPENGL_ACTIVE
    GRPWINDOWSBLITGLES*                   blitgles;
    #endif
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




