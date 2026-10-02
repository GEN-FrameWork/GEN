/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPWINDOWSScreen.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include "GRPWINDOWSScreen.h"

#include "GRPFactory.h"
#include "GRPDesktopManager.h"

#include "XTrace.h"
#include "XSystem.h"

#include "MainProcWINDOWS.h"
#include "APPFlowGraphics.h"

#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
#include "UI_Manager.h"
#endif

#include "GRPXEvent.h"
#include "GRP2DCanvas.h"
#include "GRPBitmap.h"
#include "GRPViewPort.h"


#ifdef GRP_OPENGL_ACTIVE
#include "GRPWINDOWSBlitGLES.h"
#endif

// Used only to cloak/uncloak Custom-Chromes windows during creation (see the DWMWA_CLOAK NOTE in
// Create_Window() and in Show()). dwmapi.dll ships with Windows Vista and later, which this engine
// already requires for its GLES3/EGL rendering path (see GRPBlitGLES.h), so this adds no new minimum
// OS requirement. #pragma comment links dwmapi.lib automatically, so no project file changes are needed.
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef DWMWA_CLOAK
#define DWMWA_CLOAK 13   // Not declared by older Windows SDKs; value is stable ABI (Windows 8+).
#endif


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPWINDOWSSCREEN::GRPWINDOWSSCREEN(): GRPSCREEN()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPWINDOWSSCREEN::GRPWINDOWSSCREEN(): GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_WINDOWS;

  DEVMODE devmode;
  DWORD   currentsettings = ENUM_CURRENT_SETTINGS;
  BOOL    result          = TRUE;

  memset(&devmode, 0, sizeof(DEVMODE));
  devmode.dmSize  = sizeof(DEVMODE);

  result = EnumDisplaySettings(NULL, currentsettings, &devmode);
  if(!result) return;

  SetSize(devmode.dmPelsWidth, devmode.dmPelsHeight);
  SetMaxSize(devmode.dmPelsWidth, devmode.dmPelsHeight);


  GRPRECTINT* rectworkarea = GetWorkRegion();
  if(rectworkarea)
    {
      RECT workarea;

      SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

      rectworkarea->x1 = workarea.left;
      rectworkarea->y1 = workarea.top;

      rectworkarea->x2 = workarea.right;
      rectworkarea->y2 = workarea.bottom;
    }

  SetMode(GRPPROPERTYMODE_32_BGRA_8888);

  SetIsBufferInverse(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPWINDOWSSCREEN::~GRPWINDOWSSCREEN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPWINDOWSSCREEN::~GRPWINDOWSSCREEN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Create(bool show)
* @brief      Create resource
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Create(bool show)
{
  if(!Create_Window(show)) 
    {
      return false;
    }

  GRPSCREEN::GetListScreens()->Add((void*)hwnd, this);

  return GRPSCREEN::Create(show);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Update()
* @brief      Update
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Update()
{
  #ifndef GRP_OPENGL_ACTIVE

  if(!hdc) 
    {
      return false;
    }

  SwapBuffers(hdc);

  #else

  if(!blitgles) return false;
  return blitgles->SwapBuffers();

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  canvas : handle of canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Update(GRP2DCANVAS* canvas)
{
  #ifndef GRP_OPENGL_ACTIVE

  if(!hdc)
    {
      return false;
    }

  if(!canvas)
    {
      return false;
    }

  // No bitmap rescaling on Windows: the canvas is always blitted 1:1 (width/height below are the
  // canvas' own size, not the window's). If the window's client area is currently smaller than the
  // GRPVIEWPORT_ID_MAIN viewport's declared minimum size (either axis), the content is hidden
  // (nothing is blitted, so the window's own background shows through) instead of shrinking it to fit.
  if(!IsAboveViewportMinimumSize())
    {
      return true;
    }

  SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, canvas->Buffer_Get(), &hinfo, DIB_RGB_COLORS);

  Uncloak();

  return true;

  #else

  if(!canvas)   return false;
  if(!blitgles) return false;

  bool status = blitgles->Update(canvas);

  if(status) Uncloak();

  return status;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
* @brief      Update transparent
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
{
  if(!hdc) 
    {
      return false;
    }

  if(!canvas) 
    {
      return false;
    }

  HDC     hdcscreen = GetDC(NULL);
  HDC     hdcmem    = CreateCompatibleDC(hdcscreen);
  HBITMAP hbmmem    = CreateCompatibleBitmap(hdcscreen, (LONG)width, (LONG)height);
  HBITMAP hbmold    = (HBITMAP)SelectObject(hdcmem, hbmmem);

  RECT    rect      = { 0, 0, (LONG)width, (LONG)height };
  HBRUSH  hbrush    = CreateSolidBrush(RGB(0, 0, 0));

  FillRect(hdcmem, &rect, hbrush);
  DeleteObject(hbrush);

  SetBkMode(hdcmem, TRANSPARENT);
 
  // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Screen Windows] Update Transparent (Equal)  (%04d,%04d)  Bitxpixel (%d)"), width, height, GetBitsperPixel());
     
  SetDIBitsToDevice(hdcmem, 0, 0, width, height, 0, 0, 0, height, canvas->Buffer_Get(), &hinfo, DIB_RGB_COLORS);     
 
  POINT         ptsrc   = { 0, 0 };
  SIZE          sizewnd = { (LONG)width, (LONG)height };
  BLENDFUNCTION blend   = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  POINT         ptdst   = { (LONG)positionx, (LONG)positiony };

  UpdateLayeredWindow(hwnd, hdcscreen, &ptdst, &sizewnd, hdcmem, &ptsrc, 0, &blend, ULW_ALPHA);
 
  SelectObject(hdcmem, hbmold);
  DeleteObject(hbmmem);
  DeleteDC(hdcmem);
  ReleaseDC(NULL, hdcscreen);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Delete()
* @brief      Delete resource
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Delete()
{
  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      blitgles->Destroy();
      GEN_DELETE blitgles;
      blitgles = NULL;
    }
  #endif

  if(Style_Is(GRPSCREENSTYLE_FULLSCREEN)) 
    {
      ChangeDisplaySettings(NULL, 0);
    }  

  if(hdc)
    {
      ReleaseDC(hwnd, hdc);
      hdc  = NULL;
    }

  if(hwnd) 
    {       
      SendMessage(hwnd, WM_CLOSE, 0, 0);
      //CloseWindow(hwnd);

      DestroyWindow(hwnd);

      hwnd = NULL;
    }

  UnregisterClassA((LPCSTR)GRPWINDOWSSCREEN_NAMECLASS, hinstance);

  return GRPSCREEN::Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Get_Position(int& x, int& y)
* @brief      Get position
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Get_Position(int& x, int& y)
{
  RECT    rect;

  GetWindowRect(hwnd, &rect);

  x = rect.left;  
  y = rect.top;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Set_Position(int x, int y)
* @brief      Set position
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Set_Position(int x, int y)
{
  SetPosition(x, y);

  // NOTE: SWP_NOSIZE, so the size arguments are ignored by Windows -- they are passed as 0. What used to be
  // here was a GetClientRect() + AdjustWindowRect() computation whose result went into those two ignored
  // arguments and nowhere else (and which, incidentally, filled the RECT with left/right swapped:
  // "rect.right = x; rect.left = x + width;"). Removing it changes no behaviour whatsoever and takes two
  // pointless round trips per call out of the caption-drag path, which now runs on every input tick.
  SetWindowPos(hwnd, NULL, positionx, positiony, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

  return true;
}


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::GetCursorDesktopPosition(int& x, int& y)
* @brief      Get cursor desktop position
* @note       Plain GetCursorPos(): the pointer in DESKTOP coordinates, with no reference at all to this
*             window's own position or size. GRPSCREEN::UpdateCFGChromesDrag() builds the drag delta on this
*             (and only this) so that moving the window cannot feed back into the next reading, and so that
*             the reading stays valid while the pointer is outside the window.
*             NOTE: this is the very value INPWINDOWSDEVICEMOUSE::Update() already asks Windows for and then
*             throws away after converting it to client-relative coordinates.
* @ingroup    PLATFORM_WINDOWS
*
* @param[out] x : X coordinate, desktop space.
* @param[out] y : Y coordinate, desktop space.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::GetCursorDesktopPosition(int& x, int& y)
{
  POINT point;

  if(!GetCursorPos(&point))
    {
      return false;
    }

  x = (int)point.x;
  y = (int)point.y;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::BeginCFGChromesDrag()
* @brief      Begin CFG chromes drag
* @note       Takes the mouse capture for the whole caption drag. Without it Windows stops routing the pointer
*             to this window the moment it leaves the client area, which is exactly what used to leave the
*             window stopped half way through a fast drag.
* @ingroup    PLATFORM_WINDOWS
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::BeginCFGChromesDrag()
{
  if(!hwnd)
    {
      return false;
    }

  SetCapture(hwnd);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::EndCFGChromesDrag()
* @brief      End CFG chromes drag
* @note       Releases the capture taken by BeginCFGChromesDrag(). Only releases it if this window is the one
*             actually holding it, so an unmatched call can never steal the capture from somebody else.
* @ingroup    PLATFORM_WINDOWS
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::EndCFGChromesDrag()
{
  if(GetCapture() != (HWND)hwnd)
    {
      return false;
    }

  ReleaseCapture();

  return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::GetClientSize(int& width, int& height)
* @brief      Get client size: LIVE size of the native window's client area (Windows override of the base's
*             content-size fallback). Queried fresh via GetClientRect each call, so it tracks interactive
*             resizes with no dependence on the width/height members (which, engine-wide, hold the fixed
*             CONTENT size -- see the SetDIBitsToDevice blit in Update()).
* @ingroup    PLATFORM_WINDOWS
*
* @param[out] width : live client area width.
* @param[out] height : live client area height.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::GetClientSize(int& width, int& height)
{
  if(!hwnd)
    {
      return GRPSCREEN::GetClientSize(width, height);
    }

  RECT rect;

  if(!GetClientRect(hwnd, &rect))
    {
      return GRPSCREEN::GetClientSize(width, height);
    }

  width  = (int)(rect.right  - rect.left);
  height = (int)(rect.bottom - rect.top);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::Resize(int width, int height)
* @brief      Resize
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  width : GEN_NEW width of the screen
* @param[in]  height : GEN_NEW height of the screen
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Resize(int width, int height)
{
  DWORD style = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
  int   windowwidth  = 0;
  int   windowheight = 0;

  // NOTE: uses ClientSizeToWindowSize(), NOT a plain AdjustWindowRect(), so a borderless-but-resizable
  // window (WS_THICKFRAME without WS_CAPTION -- custom Chromes) ends up with a client area that is
  // EXACTLY width x height, matching what BaseWndProc's WM_NCCALCSIZE override actually gives it,
  // instead of being inflated by a non-client border that style never really has.
  ClientSizeToWindowSize(width, height, style, windowwidth, windowheight);

  SetWindowPos(hwnd, NULL, positionx, positiony, windowwidth, windowheight, SWP_NOMOVE | SWP_NOZORDER);

  return UpdateSize(width, height);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPWINDOWSSCREEN::ClientSizeToWindowSize(int clientwidth, int clientheight, DWORD style, int& windowwidth, int& windowheight)
* @brief      Converts a desired CLIENT area size into the WINDOW (outer) size that must be passed to
*             SetWindowPos/CreateWindowEx to obtain it, for the given window style.
* @note       INTERNAL. This is NOT a plain AdjustWindowRect() call: BaseWndProc's WM_NCCALCSIZE handler
*             forces the non-client area to ZERO (client area = the whole window) whenever the style is
*             WS_THICKFRAME without WS_CAPTION (the borderless-but-resizable case used by this engine's
*             custom/non-native Chromes with resize enabled -- see Chromes_ApplyStyle/Create_Window).
*             AdjustWindowRect() knows nothing about that override: for that same style it still assumes
*             the STANDARD (non-zero) sizing-frame border and pads the window accordingly, so blindly
*             using it here would make the actual window (and hence the actual, unscaled-since-there-is-
*             no-more-stretching client area) end up bigger than requested by that padding -- visible as a
*             black band around the canvas. This mirrors the WM_NCCALCSIZE condition exactly, so the two
*             stay in sync; any window/client size computation in this file must go through here (or
*             through that same style check) instead of calling AdjustWindowRect() directly.
* @ingroup    PLATFORM_WINDOWS
*
* @param[in]  clientwidth : Desired client area width.
* @param[in]  clientheight : Desired client area height.
* @param[in]  style : Window style (WS_*) the window has/will have.
* @param[out] windowwidth : Resulting outer window width.
* @param[out] windowheight : Resulting outer window height.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::ClientSizeToWindowSize(int clientwidth, int clientheight, DWORD style, int& windowwidth, int& windowheight)
{
  if((style & WS_THICKFRAME) && !(style & WS_CAPTION))
    {
      // Same condition as the WM_NCCALCSIZE override in BaseWndProc: no non-client area at all,
      // so the window IS the client area.
      windowwidth  = clientwidth;
      windowheight = clientheight;

      return;
    }

  RECT rect = { 0, 0, clientwidth, clientheight };

  AdjustWindowRect(&rect, style, FALSE);

  windowwidth  = rect.right  - rect.left;
  windowheight = rect.bottom - rect.top;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::IsAboveViewportMinimumSize()
* @brief      Checks the CURRENT native client area against the GRPVIEWPORT_ID_MAIN viewport's declared
*             minimum size (width and height checked independently). Used to hide (rather than shrink) the
*             rendered content when the window becomes smaller than that minimum.
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @return     bool : true if the client area is at or above the configured minimum (or no minimum / no
*                    viewport is configured, i.e. unrestricted); false if it is below the minimum on
*                    either axis.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::IsAboveViewportMinimumSize()
{
  if(!hwnd)
    {
      return true;
    }

  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return true; // no viewport configured yet: no restriction to apply
    }

  float minwidth  = mainviewport->GetMinWidth();
  float minheight = mainviewport->GetMinHeight();

  if(minwidth <= 0.0f && minheight <= 0.0f)
    {
      return true; // no minimum configured on either axis
    }

  RECT rect;
  GetClientRect(hwnd, &rect);

  int clientwidth  = rect.right  - rect.left;
  int clientheight = rect.bottom - rect.top;

  if(minwidth  > 0.0f && clientwidth  < (int)minwidth)  return false;
  if(minheight > 0.0f && clientheight < (int)minheight) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPWINDOWSSCREEN::ApplyResizeLimits(MINMAXINFO* minmaxinfo)
* @brief      Handles WM_GETMINMAXINFO: caps how big this window can become at the GRPVIEWPORT_ID_MAIN
*             viewport's max size (width and height independently), both while the user drags the
*             native resize border (ptMaxTrackSize) and when it is maximized, either via the native Maximize
*             button/double-click on the caption or programmatically through Maximize() /
*             ShowWindow(SW_SHOWMAXIMIZED) (ptMaxSize / ptMaxPosition -- Windows consults the very same
*             WM_GETMINMAXINFO message in that case, so no extra code is needed in Maximize() itself).
*             If an axis has no EXPLICIT max configured via GRPVIEWPORT::SetMaxSize(), it defaults to the
*             viewport's OWN declared size (GetWidth()/GetHeight() -- the design/content resolution, e.g.
*             the 1024x768 an app sets via CreateViewport): growing past that shows no more real canvas
*             content anyway (just background), so this is the sane default without requiring every app to
*             call SetMaxSize() explicitly. Apps that DO want a different (larger or smaller) cap can still
*             override it per axis via SetMaxSize().
*             If the resulting max is bigger than the monitor the window is on, the monitor's own work
*             area size is used instead (a window can never usefully be bigger than the screen it is on).
*             No minimum is enforced here: the window is left free to shrink as usual; see
*             IsAboveViewportMinimumSize() / GRPWINDOWSBLITGLES::ComputePresentationScale for how the
*             rendered content is hidden (not the window itself) below the viewport's minimum.
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @param[in]  minmaxinfo : MINMAXINFO pointer received with WM_GETMINMAXINFO (lParam), to fill in.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::ApplyResizeLimits(MINMAXINFO* minmaxinfo)
{
  if(!minmaxinfo || !hwnd)
    {
      return;
    }

  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return; // no viewport configured yet: keep the OS defaults untouched
    }

  int maxviewportwidth  = (int)mainviewport->GetMaxWidth();
  int maxviewportheight = (int)mainviewport->GetMaxHeight();

  // No EXPLICIT SetMaxSize() on this axis: fall back to the viewport's own declared size, so the
  // window is capped even when the app never calls the new SetMaxSize() API (see @brief above).
  if(maxviewportwidth  <= 0) maxviewportwidth  = (int)mainviewport->GetWidth();
  if(maxviewportheight <= 0) maxviewportheight = (int)mainviewport->GetHeight();

  if(maxviewportwidth <= 0 && maxviewportheight <= 0)
    {
      return; // viewport has no usable size at all yet: keep the OS defaults untouched
    }

  // Monitor this window is (mostly) on, so multi-monitor Maximize lands on the right screen and we
  // never ask for more than that monitor's real work area.
  RECT        workarea = { 0, 0, 0, 0 };
  HMONITOR    hmonitor  = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO monitorinfo;

  memset(&monitorinfo, 0, sizeof(MONITORINFO));
  monitorinfo.cbSize = sizeof(MONITORINFO);

  if(hmonitor && GetMonitorInfo(hmonitor, &monitorinfo))
    {
      workarea = monitorinfo.rcWork;
    }
   else
    {
      SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
    }

  int monitorwidth  = workarea.right  - workarea.left;
  int monitorheight = workarea.bottom - workarea.top;

  // Per-axis target CLIENT size: the viewport max, but never larger than the monitor it is on.
  int targetclientwidth  = (maxviewportwidth  > 0 && maxviewportwidth  < monitorwidth ) ? maxviewportwidth  : monitorwidth;
  int targetclientheight = (maxviewportheight > 0 && maxviewportheight < monitorheight) ? maxviewportheight : monitorheight;

  // Convert that CLIENT size into a WINDOW size (adds back the caption/borders for the current style,
  // if it actually has any -- see ClientSizeToWindowSize()).
  DWORD style = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
  int   targetwindowwidth  = 0;
  int   targetwindowheight = 0;

  ClientSizeToWindowSize(targetclientwidth, targetclientheight, style, targetwindowwidth, targetwindowheight);

  // Drag limit (only meaningful with WS_THICKFRAME, harmless otherwise): never let the user drag the
  // window past the viewport max, per axis.
  if(maxviewportwidth  > 0) minmaxinfo->ptMaxTrackSize.x = targetwindowwidth;
  if(maxviewportheight > 0) minmaxinfo->ptMaxTrackSize.y = targetwindowheight;

  // Native Maximize (button / double-click caption / SW_SHOWMAXIMIZED): lands at the viewport max,
  // clamped to the monitor, anchored at the monitor's work area origin (in virtual-screen coordinates,
  // as required by WM_GETMINMAXINFO for correct multi-monitor behaviour).
  minmaxinfo->ptMaxSize.x     = targetwindowwidth;
  minmaxinfo->ptMaxSize.y     = targetwindowheight;
  minmaxinfo->ptMaxPosition.x = workarea.left;
  minmaxinfo->ptMaxPosition.y = workarea.top;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPWINDOWSSCREEN::Show(bool active)
* @brief      Show
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Show(bool active)
{
  if(!hwnd) return false;

  // NOTE: does NOT touch DWMWA_CLOAK (see the NOTE in Create_Window() where it is set, and in
  // Update(GRP2DCANVAS*) where it is finally cleared). Uncloaking here, on the WS_VISIBLE/ShowWindow
  // transition, was the first version of this fix and it was NOT enough: this call happens right after
  // Create_Window() presents its first blank frame, but BEFORE the rest of Ini_Graphics()'s synchronous
  // work (dashboard XML/CSS parsing, ~30 SVG icon decodes) -- which can easily take another second --
  // has run. For that whole stretch the window sat mapped, uncloaked and idle with NOTHING pumping its
  // message queue (the main loop has not started yet), which is long enough for Windows/DWM to treat it
  // as unresponsive and paint it from a stale/ghosted surface instead of GEN's own last presented frame
  // -- indistinguishable, visually, from the undefined-content garbage the earlier fixes targeted, but
  // caused by a completely different mechanism. See Update(GRP2DCANVAS*) for where uncloaking actually
  // happens now.
  ::ShowWindow(hwnd, (active?SW_SHOW:SW_HIDE));

  isshow  = active;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPWINDOWSSCREEN::Uncloak()
* @brief      Uncloak
* @note       Clears DWMWA_CLOAK (see the NOTE in Create_Window()) the first time it runs after Create_Window()
*             cloaked a Custom-Chromes window. Called from Update(GRP2DCANVAS*) right after a real frame has
*             actually been presented -- by construction that only happens once the main loop is ticking (it is
*             what calls Update() every frame), so the message queue is being pumped regularly again and the
*             window is no longer at risk of being treated as unresponsive. A no-op on every call after the
*             first (windowcloaked already false), and for any window that was never cloaked in the first place
*             (native Chromes, no Chromes, fullscreen, GRPSCREENSTYLE_TRANSPARENT).
* @ingroup    PLATFORM_WINDOWS
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::Uncloak()
{
  if(!windowcloaked) return;
  if(!hwnd)          return;

  BOOL cloak = FALSE;

  DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak));
  windowcloaked = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::ShowCursor(bool active)
* @brief      Show cursor
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::ShowCursor(bool active)
{
  if(!hwnd) return false;

  return ::ShowCursor(active?TRUE:FALSE)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPWINDOWSSCREEN::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPWINDOWSSCREEN::GetHandle()
{
  return (void*)hwnd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPWINDOWSSCREEN::SetHandle(void* handle_window)
* @brief      Set handle
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handle_window : Handle window pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::SetHandle(void* handle_window)
{
  this->hwnd = (HWND)handle_window;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Set_Focus()
* @brief      Set focus
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Set_Focus()
{
  if(!hwnd)
    {
      return false;
    }

  HWND  hwndtarget      = GetAncestor(hwnd, GA_ROOT);
  HWND  hwndforeground  = GetForegroundWindow();
  DWORD tidcurrent      = GetCurrentThreadId();
  DWORD tidtarget       = 0;
  DWORD tidforeground   = 0;
  bool  attachedtarget  = false;
  bool  attachedfore    = false;

  if(!hwndtarget)
    {
      hwndtarget = hwnd;
    }

  tidtarget = GetWindowThreadProcessId(hwndtarget, NULL);

  if(hwndforeground)
    {
      tidforeground = GetWindowThreadProcessId(hwndforeground, NULL);
    }

  {
    DWORD pidtarget = 0;
    GetWindowThreadProcessId(hwndtarget, &pidtarget);
    if(pidtarget)
      {
        AllowSetForegroundWindow(pidtarget);
      }
  }

  // Attach before SetForegroundWindow: Windows blocks focus steals unless input queues are joined.
  if(tidforeground && (tidforeground != tidcurrent))
    {
      attachedfore = AttachThreadInput(tidcurrent, tidforeground, TRUE)?true:false;
    }

  if(tidtarget && (tidtarget != tidcurrent) && (tidtarget != tidforeground))
    {
      attachedtarget = AttachThreadInput(tidcurrent, tidtarget, TRUE)?true:false;
    }

  ShowWindow(hwndtarget, SW_RESTORE);
  BringWindowToTop(hwndtarget);
  SetForegroundWindow(hwndtarget);
  SetActiveWindow(hwndtarget);
  SetFocus(hwnd);

  if(attachedtarget)
    {
      AttachThreadInput(tidcurrent, tidtarget, FALSE);
    }

  if(attachedfore)
    {
      AttachThreadInput(tidcurrent, tidforeground, FALSE);
    }

  HWND hwndnow = GetForegroundWindow();
  if(!hwndnow)
    {
      return false;
    }

  if((hwndnow == hwndtarget) || (hwndnow == hwnd))
    {
      return true;
    }

  if(GetAncestor(hwndnow, GA_ROOT) == hwndtarget)
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Minimize(bool active)
* @brief      Minimize
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Minimize(bool active)
{
  if(!hwnd)
    {
      return false;
    }

  return ShowWindow(hwnd, active?SW_SHOWMINIMIZED:SW_NORMAL)?true:false; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Maximize(bool active)
* @brief      Maximize
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Maximize(bool active)
{
  if(!hwnd)
    {
      return false;
    }

  if(!active)
    {
      // Restore the pre-maximize window rect when we grew via SetWindowPos (not SW_SHOWMAXIMIZED).
      // ShowWindow(SW_NORMAL) alone does not shrink a window that never entered the OS maximized state.
      if(maximize_restore_valid)
        {
          int x = maximize_restore_x;
          int y = maximize_restore_y;
          int w = maximize_restore_w;
          int h = maximize_restore_h;
          maximize_restore_valid = false;

          if(w < 1) w = 1;
          if(h < 1) h = 1;

          if(!SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER))
            {
              return false;
            }
          return true;
        }

      return ShowWindow(hwnd, SW_RESTORE)?true:false;
    }

  // Already as big as the viewport cap allows: caller (chrome button) toggles via Maximize(false).
  if(IsClientSizeAtMaximum())
    {
      return true;
    }

  GRPVIEWPORT* mainviewport      = GetViewport(GRPVIEWPORT_ID_MAIN);
  int          maxviewportwidth  = 0;
  int          maxviewportheight = 0;

  if(mainviewport)
    {
      maxviewportwidth  = (int)mainviewport->GetMaxWidth();
      maxviewportheight = (int)mainviewport->GetMaxHeight();

      // Same fallback chain as ApplyResizeLimits() / GRPSCREEN::IsClientSizeAtMaximum(): no explicit
      // SetMaxSize() on an axis -> the viewport's own declared (design) size.
      if(maxviewportwidth  <= 0) maxviewportwidth  = (int)mainviewport->GetWidth();
      if(maxviewportheight <= 0) maxviewportheight = (int)mainviewport->GetHeight();
    }

  if((maxviewportwidth <= 0) || (maxviewportheight <= 0))
    {
      // No usable viewport cap configured: a true OS maximize is the only sensible meaning left.
      return ShowWindow(hwnd, SW_SHOWMAXIMIZED)?true:false;
    }

  // With a viewport growth cap, NEVER go through SW_SHOWMAXIMIZED: a window in Windows' "maximized" state
  // whose WM_GETMINMAXINFO-capped size is smaller than the work area is a degenerate state -- Windows
  // relocates it to ptMaxPosition (the work area's top-left corner), the taskbar/DWM previews and the
  // restore geometry misbehave around it, and a later restore can land the window partially off-screen.
  // Instead the window is grown IN PLACE to the very same capped size every other resize path resolves
  // (viewport cap, clamped to the monitor's work area), KEEPING its current position -- which is exactly
  // what the X11 backend does under KWin (PMaxSize + snap-back, no WM 'maximized' state kept). The window
  // stays a NORMAL window at its capped size, so there is no special state to restore from later.
  RECT        workarea = { 0, 0, 0, 0 };
  HMONITOR    hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO monitorinfo;

  memset(&monitorinfo, 0, sizeof(MONITORINFO));
  monitorinfo.cbSize = sizeof(MONITORINFO);

  if(hmonitor && GetMonitorInfo(hmonitor, &monitorinfo))
    {
      workarea = monitorinfo.rcWork;
    }
   else
    {
      SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
    }

  int monitorwidth  = workarea.right  - workarea.left;
  int monitorheight = workarea.bottom - workarea.top;

  int targetclientwidth  = (maxviewportwidth  < monitorwidth ) ? maxviewportwidth  : monitorwidth;
  int targetclientheight = (maxviewportheight < monitorheight) ? maxviewportheight : monitorheight;

  DWORD style              = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
  int   targetwindowwidth  = 0;
  int   targetwindowheight = 0;

  ClientSizeToWindowSize(targetclientwidth, targetclientheight, style, targetwindowwidth, targetwindowheight);

  RECT windowrect;

  if(!GetWindowRect(hwnd, &windowrect))
    {
      return false;
    }

  // Remember current geometry so Maximize(false) / chrome toggle can shrink back.
  maximize_restore_x     = windowrect.left;
  maximize_restore_y     = windowrect.top;
  maximize_restore_w     = windowrect.right  - windowrect.left;
  maximize_restore_h     = windowrect.bottom - windowrect.top;
  maximize_restore_valid = true;

  // Keep the current position; only nudge it back if the grown window would overflow the work area
  // (so growing never leaves part of the window off-screen or under the taskbar).
  int newx = windowrect.left;
  int newy = windowrect.top;

  if((newx + targetwindowwidth ) > workarea.right ) newx = workarea.right  - targetwindowwidth;
  if((newy + targetwindowheight) > workarea.bottom) newy = workarea.bottom - targetwindowheight;
  if(newx < workarea.left) newx = workarea.left;
  if(newy < workarea.top)  newy = workarea.top;

  if(!SetWindowPos(hwnd, NULL, newx, newy, targetwindowwidth, targetwindowheight, SWP_NOZORDER))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPWINDOWSSCREEN::CaptureContent(GRPRECTINT* rect, void* handle_window)
* @brief      Capture content
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  rect : Rectangle to use.
* @param[in]  handle_window : Handle window pointer to use.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPWINDOWSSCREEN::CaptureContent(GRPRECTINT* rect, void* handle_window)
{
  XBUFFER xbuffer;
  HDC     hdcsource = GetDC(handle_window?(HWND)handle_window:hwnd);
  HDC     hdcmemory = CreateCompatibleDC(hdcsource);
  int     x         = 0;
  int     y         = 0;
  int     cx        = GetWidth();
  int     cy        = GetHeight();  
  bool    status    = false;

  if(!hdcsource)
    {
      return NULL;  
    }

  if(!hdcmemory)
    {
      DeleteDC(hdcsource);
      return NULL;        
    }

  if(rect)
    {
      x         = rect->x1;
      y         = rect->y1;
      cx        = (rect->x2 - rect->x1);
      cy        = (rect->y2 - rect->y1);
    }  

  HBITMAP hbitmap     = CreateCompatibleBitmap(hdcsource, cx, cy);
  HBITMAP hbitmapold  = (HBITMAP)SelectObject(hdcmemory, hbitmap);
  BITMAP  bitmap;

  BitBlt(hdcmemory, 0, 0, cx, cy, hdcsource, x, y, SRCCOPY);
  hbitmap = (HBITMAP)SelectObject(hdcmemory, hbitmapold);

  DeleteDC(hdcsource);
  DeleteDC(hdcmemory);

  GetObject(hbitmap, sizeof(bitmap), (LPSTR)&bitmap);

  HDC dcbitmap = CreateCompatibleDC(NULL);
  SelectObject(dcbitmap, hbitmap);

  BITMAPINFO bmpinfo;

  bmpinfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  bmpinfo.bmiHeader.biWidth        = bitmap.bmWidth;
  bmpinfo.bmiHeader.biHeight       = bitmap.bmHeight;
  bmpinfo.bmiHeader.biPlanes       = bitmap.bmPlanes;
  bmpinfo.bmiHeader.biBitCount     = bitmap.bmBitsPixel;
  bmpinfo.bmiHeader.biCompression  = BI_RGB;
  bmpinfo.bmiHeader.biSizeImage    = 0;
  bmpinfo.bmiHeader.biClrImportant = 0;
  bmpinfo.bmiHeader.biClrUsed      = 0;

  GRPBITMAP* grpbitmap = GRPFACTORY::GetInstance().CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, GRPPROPERTYMODE_32_BGRA_8888);
  if(grpbitmap)
    {
      status = GetDIBits(dcbitmap, hbitmap, 0, bitmap.bmHeight, grpbitmap->GetBuffer(), &bmpinfo, DIB_RGB_COLORS)?true:false;

      XDWORD* buffer = (XDWORD*)grpbitmap->GetBuffer();
      for(XDWORD c=0; c<(grpbitmap->GetBufferSize()/sizeof(XDWORD)); c++)
        {
          (*buffer) |= 0xFF000000;
          buffer++;
        }
    }

  if(!status)
    {
      GEN_DELETE grpbitmap;
      grpbitmap = NULL;
    }

  return grpbitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPWINDOWSSCREEN::GetDesktopHandle()
* @brief      Get desktop handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPWINDOWSSCREEN::GetDesktopHandle()
{
  HANDLE hwnd = GetDesktopWindow();

  return (void*)hwnd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPWINDOWSSCREEN::GetShellHandle()
* @brief      Get shell handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPWINDOWSSCREEN::GetShellHandle()
{
  HANDLE hwnd = GetShellWindow();

  return (void*)hwnd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPWINDOWSSCREEN::GetHDC()
* @brief      Get HDC
* @ingroup    PLATFORM_WINDOWS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPWINDOWSSCREEN::GetHDC()
{
  return (void*)hdc;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPWINDOWSSCREEN::SetHDC(HDC hdc)
* @brief      Set HDC
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hdc : Hdc value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::SetHDC(HDC hdc)
{
  this->hdc = hdc;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BITMAPINFO* GRPWINDOWSSCREEN::GetHInfo()
* @brief      Get H info
* @ingroup    PLATFORM_WINDOWS
* 
* @return     BITMAPINFO* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BITMAPINFO* GRPWINDOWSSCREEN::GetHInfo()
{
  return &hinfo;
}


#ifdef GRP_OPENGL_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPWINDOWSBLITGLES* GRPWINDOWSSCREEN::GetBlitGLES()
* @brief      Get OpenGL ES blitter (only present when GRP_OPENGL_ACTIVE is defined)
* @ingroup    PLATFORM_WINDOWS
* 
* @return     GRPWINDOWSBLITGLES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPWINDOWSBLITGLES* GRPWINDOWSSCREEN::GetBlitGLES()
{
  return blitgles;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPWINDOWSSCREEN::GetTaskbarHeight()
* @brief      Get taskbar height
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPWINDOWSSCREEN::GetTaskbarHeight()
{
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  RECT workArea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

  int taskbarHeight = screenHeight - (workArea.bottom - workArea.top);

  return taskbarHeight;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPWINDOWSSCREEN::Chromes_ApplyStyle(DWORD& style, DWORD& exstyle)
* @brief      Chromes apply style
* @note       INTERNAL: resolves the current GRPSCREENCFGCHROMES configuration (native Windows chromes only, for now)
*             into the WS_* / WS_EX_* bits used when creating the window. Only called when a Chromes
*             configuration is active, native chromes are requested, and the screen is not transparent
*             (Fullscreen and Transparent styles are prioritary over the Chromes configuration).
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  style : Window style (WS_*) bits to complete.
* @param[in]  exstyle : Window extended style (WS_EX_*) bits to complete.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::Chromes_ApplyStyle(DWORD& style, DWORD& exstyle)
{
  GRPSCREENCFGCHROMES* cfgchromes = GetCFGChromes();
  if(!cfgchromes) return;

  if(cfgchromes->GetNativeCaptionActive())
    {
      style |= WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED;

      if(cfgchromes->GetNativeMinimizeActive()) style |= WS_MINIMIZEBOX;
      if(cfgchromes->GetNativeMaximizeActive()) style |= WS_MAXIMIZEBOX;
    }
   else
    {
      style |= WS_POPUP;
    }

  if(cfgchromes->GetResizeActive())
    {
      style |= WS_THICKFRAME;
    }

  (void)exstyle; // Reserved for future (custom GEN-drawn chromes) use.
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPWINDOWSSCREEN::Chromes_ApplyPostCreate()
* @brief      Chromes apply post create
* @note       INTERNAL: applies the parts of the GRPSCREENCFGCHROMES configuration that require the window
*             handle to already exist (application icon and close button), for native Windows chromes.
*             The close configuration also reuses the existing CanClose() mechanism, so WM_CLOSE keeps
*             being blocked consistently regardless of how the window is asked to close.
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::Chromes_ApplyPostCreate()
{
  if(!hwnd) return;

  GRPSCREENCFGCHROMES* cfgchromes = GetCFGChromes();
  if(!cfgchromes) return;

  if(cfgchromes->GetNativeCaptionActive() && !cfgchromes->GetNativeIconActive())
    {
      SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)NULL);
      SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)NULL);

      // NOTE: WM_SETICON alone only affects the taskbar/Alt+Tab icon; the caption itself still reserves
      // and paints an icon slot. WS_EX_DLGMODALFRAME is the standard Win32 technique to make the caption
      // stop reserving that icon slot; SWP_FRAMECHANGED forces the non-client area to be recalculated and
      // repainted so the change is actually visible (it has a minor side effect: the window border is
      // drawn with the thin dialog-style double line instead of the usual raised border).
      LONG_PTR exstylebits = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
      SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstylebits | WS_EX_DLGMODALFRAME);

      SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }

  SetCanClose(cfgchromes->GetNativeCloseActive());

  if(!cfgchromes->GetNativeCloseActive())
    {
      HMENU hsysmenu = GetSystemMenu(hwnd, FALSE);
      if(hsysmenu)
        {
          EnableMenuItem(hsysmenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPWINDOWSSCREEN::Create_Window(bool show)
* @brief      Create window
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Create_Window(bool show)
{
  //-----------------------------------------------------------------------------------------------------

  if(hwnd) 
    {
      return false;
    }

  hinstance = (HINSTANCE)mainprocwindows.GetHandle();

  //-----------------------------------------------------------------------------------------------------

  classname               = GRPWINDOWSSCREEN_NAMECLASS;
  title                   = __L(" ");

  wndclass.cbSize         = sizeof(wndclass);
  wndclass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndclass.lpfnWndProc    = BaseWndProc;
  wndclass.cbClsExtra     = 0;
  wndclass.cbWndExtra     = 0;
  wndclass.hInstance      = NULL;
  wndclass.hIcon          = NULL;
  wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wndclass.lpszMenuName   = NULL;
  wndclass.hIconSm        = NULL;

  wndclass.lpszClassName  = classname.Get();
  wndclass.hInstance      = hinstance;

  RegisterClassEx(&wndclass);

  //-----------------------------------------------------------------------------------------------------

  GRPRECTINT* alldesktoprect  = NULL;
  int         nscreen         = GetDesktopScreenSelected(); 
  DWORD       _exstyle        = 0;
  DWORD       _style          = 0;
  int         posx            = 0;
  int         posy            = 0;  

  if(nscreen == GRPSCREENTYPE_DESKTOP_ALL)
    {
      alldesktoprect = GetDesktopManager()->GetDesktopMonitors()->GetCombinedRect(); 
    }
   else 
    {
      alldesktoprect = GetDesktopManager()->GetDesktopMonitors()->GetMonitorsRects()->Get(nscreen);
    }
  
  if(!alldesktoprect)
    {
      return false;          
    }            

  if(Styles_IsFullScreen())          
    {
      if(Style_Is(GRPSCREENSTYLE_FULLSCREEN_ADJUSTRESOLUTION))
        {
          DEVMODE devmode;

          memset(&devmode, 0, sizeof(DEVMODE));

          devmode.dmSize       = sizeof(DEVMODE);
          devmode.dmPelsWidth  = width;
          devmode.dmPelsHeight = height;
          devmode.dmBitsPerPel = GetBitsperPixel();
          devmode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

          LONG status = ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

          if(status != DISP_CHANGE_SUCCESSFUL) 
            {
              return NULL;
            }
        }
       else 
        {                         
          _exstyle  = WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
          _style    = WS_POPUP;

          width  = alldesktoprect->GetWidth();    
          height = alldesktoprect->GetHeight(); 
   
          posx   = alldesktoprect->x1;
          posy   = alldesktoprect->y1;   
        }

      if(Style_Is(GRPSCREENSTYLE_HEIGHTWITHOUTTASKBAR))
        {
          height -= GetTaskbarHeight();
        }  

      if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
        {
          _exstyle |= WS_EX_LAYERED;
        }

      if(Style_Is(GRPSCREENSTYLE_NOICONTASKBAR))
        {
          _exstyle |= WS_EX_TOOLWINDOW;
        }

      hwnd = CreateWindowEx(_exstyle, classname.Get(), NULL, _style, posx, posy, width, height, NULL, NULL, hinstance, (void*)this);

      if(!hwnd) 
        {
          return false;
        }

      SetPosition(posx, posy);  
      SetSize(width, height);
      SetMaxSize(width, height);              
    }
   else
    {          
      int winw = (int)GetPresentationWidth();
      int winh = (int)GetPresentationHeight();

      if(positionx == GRPPROPERTYMODE_SCREEN_CENTER) 
        {
          posx = (alldesktoprect->GetWidth() - winw)/2;
        }
       else 
        {
          posx = positionx; 
        }

      if(positiony == GRPPROPERTYMODE_SCREEN_CENTER) 
        {
          posy = (alldesktoprect->GetHeight() - winh)/2;
        }
       else 
        {
          posy = positiony; 
        }
   
      // NOTE: Fullscreen and Transparent styles are prioritary over any Chromes configuration (see below).
      bool usecfgchromesnative = (IsCFGChromesActive()                       &&
                                   GetCFGChromes()->GetUseNativeChromes()    &&
                                   !Style_Is(GRPSCREENSTYLE_TRANSPARENT));

      // Custom (non native) Chromes: GEN's own UI layout draws the whole caption (icon, title, min/max/close),
      // so the native window must show NO decoration of its own at all -- no caption, no system menu, no
      // native icon/buttons. WS_THICKFRAME (native resize, if requested) is the only native bit kept; the
      // leftover sizing-frame bar that combination would otherwise show is already stripped generically by the
      // WM_NCCALCSIZE/WM_NCHITTEST handling in BaseWndProc (it keys off the style bits themselves, not off
      // which Chromes mode set them), so nothing else is needed here for that.
      bool usecfgchromescustom = (IsCFGChromesActive()                       &&
                                   !GetCFGChromes()->GetUseNativeChromes()   &&
                                   !Style_Is(GRPSCREENSTYLE_TRANSPARENT));

      if(usecfgchromesnative)
        {
          Chromes_ApplyStyle(_style, _exstyle);
        }
       else if(usecfgchromescustom)
        {
          _style |= WS_POPUP;

          if(GetCFGChromes()->GetResizeActive()) _style |= WS_THICKFRAME;
        }
       else
        {
          if(Style_Is(GRPSCREENSTYLE_NOWINDOWICONS))
            {
              _style |= WS_OVERLAPPED;  // _exstyle |= WS_EX_TOOLWINDOW;
            }
           else _style |= WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        }


      if(Style_Is(GRPSCREENSTYLE_ONTOP))
        {
          _exstyle |= WS_EX_TOPMOST;
        }

      if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
        {
          _exstyle |= WS_EX_LAYERED;             
          _style   |= WS_POPUP;
        }
       else
        {
          if(!usecfgchromesnative && !usecfgchromescustom)
            {
              if(Style_Is(GRPSCREENSTYLE_TITLE))
                {
                  _style |= WS_OVERLAPPED;
                }
               else
                {
                  _style |= WS_POPUP;
                }
            }
        }

      if(Style_Is(GRPSCREENSTYLE_NOICONTASKBAR))
        {
          _exstyle |= WS_EX_TOOLWINDOW;
        }

      hwnd = CreateWindowEx(_exstyle, classname.Get(), (usecfgchromesnative && !GetCFGChromes()->GetNativeTitleActive()) ? __L("") : GetTitle()->Get(), _style, posx, posy, winw, winh, NULL, NULL, hinstance, (void*)this);

      if(!hwnd)
        {
          return false;
        }

      // NOTE (garbage/desktop content flashing at the top of Custom-Chromes windows on startup): a
      // usecfgchromescustom window has NO native non-client area at all -- its entire rectangle, row 0
      // included, is client area owned by GEN's own canvas (see usecfgchromescustom above). From this
      // CreateWindowEx() until the app's main loop is actually ticking (pumping messages AND presenting a
      // real frame every iteration -- see Uncloak(), called from Update(GRP2DCANVAS*)), the window already
      // exists and CAN legally be composited/repainted by the desktop (DWM) at any point, including while
      // GEN itself is busy with synchronous startup work (parsing the dashboard's XML/CSS, decoding SVG
      // icons...) and not pumping its own message queue. On affected driver/DWM combinations, whatever DWM
      // paints during that gap is not simply blank/black: either the compositor's own surface memory
      // (which, having just been used to composite the real desktop, can look like actual desktop/taskbar
      // content), or -- for a window Windows has started treating as unresponsive because its message
      // queue has gone unpumped for a while -- a stale/ghosted repaint that is visually indistinguishable
      // from that same garbage. DWMWA_CLOAK removes the window from composition entirely (it exists, but
      // the desktop never draws it) until Uncloak() clears it, which by construction only happens once the
      // main loop -- and with it, regular message pumping -- has actually started. A usecfgchromesnative
      // window never needs any of this: its top rows are native non-client area, painted synchronously by
      // Windows itself independently of whether the app's own message loop is keeping up, so there is no
      // gap for DWM to fill with anything else.
      if(usecfgchromescustom)
        {
          BOOL cloak = TRUE;

          if(SUCCEEDED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
            {
              windowcloaked = true;
            }
        }

      if(usecfgchromesnative)
        {
          Chromes_ApplyPostCreate();
        }

      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Screen Windows] Part 1 Ini: x=%04d, y=%04d (%04d,%04d)  Bitxpixel (%d)"), posx, posy, width, height, GetBitsperPixel());

      // NOTE: uses ClientSizeToWindowSize(), NOT a plain AdjustWindowRect(), so a borderless-but-resizable
      // window (WS_THICKFRAME without WS_CAPTION -- custom Chromes) ends up with a client area that is
      // EXACTLY winw x winh, matching what BaseWndProc's WM_NCCALCSIZE override actually gives it, instead
      // of being inflated by a non-client border that style never really has (which, now that the canvas is
      // no longer stretched to fill whatever size results, would otherwise show as a black band around it).
      int windowwidth  = 0;
      int windowheight = 0;

      ClientSizeToWindowSize(winw, winh, (DWORD)GetWindowLong(hwnd, GWL_STYLE), windowwidth, windowheight);

      SetWindowPos(hwnd, NULL, posx, posy, windowwidth, windowheight, SWP_NOMOVE | SWP_NOZORDER);

      POINT point = { 0, 0}; 

      ClientToScreen(hwnd, (LPPOINT)&point);  

      if(Style_Is(GRPSCREENSTYLE_HEIGHTWITHOUTTASKBAR))
        {
          point.y -= GetTaskbarHeight();
        }  

      point.x  += alldesktoprect->x1;
      point.y  += alldesktoprect->y1;   
    
      SetPosition(point.x, point.y);
      Set_Position(point.x, point.y);

      // NOTE: the screen size members stay at the CONTENT size (width/height), not the window size.
      SetSize(width, height);
      SetMaxSize(width, height);
    }

  // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Screen Windows] Part 2 Ini: x=%04d, y=%04d (%04d,%04d)  Bitxpixel (%d)"), posx, posy, width, height, GetBitsperPixel());

  // NOTE: Show(show)/SetFocus(hwnd) used to happen HERE, before hdc/blitgles even existed -- see the
  // GRP_OPENGL_ACTIVE block below, right before the final "return true;", for why they were moved past it.

  hdc = GetDC(hwnd);
  if(!hdc)
    {
      return false;
    }


  #ifndef OPENGL_INFO
  memset(&hinfo, 0, sizeof(BITMAPINFO));

  hinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  hinfo.bmiHeader.biWidth         = width;
  hinfo.bmiHeader.biHeight        = height;
  hinfo.bmiHeader.biPlanes        = 1;
  hinfo.bmiHeader.biBitCount      = GetBitsperPixel();
  hinfo.bmiHeader.biCompression   = BI_RGB;
  hinfo.bmiHeader.biSizeImage     = (XDWORD)(hinfo.bmiHeader.biWidth*hinfo.bmiHeader.biHeight*hinfo.bmiHeader.biBitCount)/8;
  #endif

  #ifdef GRP_OPENGL_ACTIVE  
  if(!Style_Is(GRPSCREENSTYLE_TRANSPARENT))
    {
      if(!blitgles)
        {
          blitgles = GEN_NEW GRPWINDOWSBLITGLES();
          if(!blitgles)
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Screen Windows] Could not allocate GRPWINDOWSBLITGLES"));
              return false;
            }

          if(!blitgles->Create(this))
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Screen Windows] BlitGLES create failed"));
              GEN_DELETE blitgles;
              blitgles = NULL;
              return false;
            }

          // The EGL surface Create() just built has never been cleared or presented -- its initial content
          // is whatever the driver leaves in a freshly allocated buffer, which is undefined and on several
          // drivers visibly so (stray coloured pixels, not a clean colour). Show(show) below is what first
          // makes this window visible to the desktop; clearing and presenting a blank frame BEFORE that
          // call, rather than after, means the compositor never has a chance to grab that undefined content
          // -- twice, so neither buffer of the common double-buffered swap chain is left holding it.
          blitgles->PresentBlankFrame();
          blitgles->PresentBlankFrame();
        }
    }
  #endif

  // Moved past hdc/blitgles setup above (see the NOTE further up): on a GRP_OPENGL_ACTIVE build the
  // window must not become visible to the desktop until the EGL surface behind it has at least one
  // clean, known-colour frame presented (see PresentBlankFrame() just above) -- otherwise whatever the
  // OS paints before GEN's own first real Update(canvas) call shows the surface's undefined initial
  // content instead. GetDC()/SetFocus() do not require the window to already be visible, so moving them
  // alongside Show() here changes nothing about their own behaviour.
  if(show)
    {
      if(!Show(show)) return false;
    }

  if(!SetFocus(hwnd))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         LRESULT CALLBACK GRPWINDOWSSCREEN::BaseWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
* @brief      Base wnd proc
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hwnd : Hwnd value.
* @param[in]  msg : Msg value.
* @param[in]  wparam : Wparam value.
* @param[in]  lparam : Lparam value.
* 
* @return     LRESULT CALLBACK : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK GRPWINDOWSSCREEN::BaseWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch(msg)
    {
      case WM_SYSCOMMAND            :  switch(wparam)
                                        {
                                          case SC_CLOSE         : break;
                                          case SC_SCREENSAVE    :
                                          case SC_MONITORPOWER  : return 0;
                                        }
                                      break;

      case WM_ENDSESSION            : break;

      // NOTE: only taken for windows created with Chromes caption OFF and resize ON (WS_THICKFRAME without
      // WS_CAPTION). In that combination Windows still reserves/paints the standard sizing-frame non-client
      // area (visible as a thin bar/border), even though there is no caption; these two messages remove it
      // while keeping the edges/corners resizable by hand-testing the cursor position ourselves.
      case WM_NCCALCSIZE            : { LONG stylebits = GetWindowLong(hwnd, GWL_STYLE);

                                        if(wparam && (stylebits & WS_THICKFRAME) && !(stylebits & WS_CAPTION))
                                          {
                                            return 0; // Client area = whole window (no non-client border).
                                          }
                                      }
                                      break;

      case WM_NCHITTEST              : { LONG stylebits = GetWindowLong(hwnd, GWL_STYLE);

                                        if((stylebits & WS_THICKFRAME) && !(stylebits & WS_CAPTION))
                                          {
                                            RECT rect;
                                            GetWindowRect(hwnd, &rect);

                                            int x       = (int)(short)LOWORD(lparam);
                                            int y       = (int)(short)HIWORD(lparam);
                                            int border  = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

                                            bool onleft   = (x >= rect.left)   && (x <  rect.left   + border);
                                            bool onright  = (x <  rect.right)  && (x >= rect.right  - border);
                                            bool ontop    = (y >= rect.top)    && (y <  rect.top    + border);
                                            bool onbottom = (y <  rect.bottom) && (y >= rect.bottom - border);

                                            if(ontop    && onleft)  return HTTOPLEFT;
                                            if(ontop    && onright) return HTTOPRIGHT;
                                            if(onbottom && onleft)  return HTBOTTOMLEFT;
                                            if(onbottom && onright) return HTBOTTOMRIGHT;
                                            if(onleft)              return HTLEFT;
                                            if(onright)             return HTRIGHT;
                                            if(ontop)               return HTTOP;
                                            if(onbottom)            return HTBOTTOM;

                                            return HTCLIENT;
                                          }
                                      }
                                      break;

      // No bitmap rescaling: caps how big this window can become (drag border + native Maximize) at
      // the GRPVIEWPORT_ID_MAIN viewport's declared max size. See GRPWINDOWSSCREEN::ApplyResizeLimits.
      // Screen not yet registered (e.g. this fires during the synchronous CreateWindowEx() call
      // itself, before GRPWINDOWSSCREEN::Create() adds it to GetListScreens()) is a no-op, same as
      // the existing WM_MOVE/WM_CLOSE/WM_DESTROY handling below.
      case WM_GETMINMAXINFO         : { GRPWINDOWSSCREEN* screen = (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);
                                        if(screen)
                                          {
                                            screen->ApplyResizeLimits((MINMAXINFO*)lparam);
                                            return 0;
                                          }
                                      }
                                      break;

      // While the user interactively drags the resize border, Windows runs its OWN modal message
      // loop inside DispatchMessage() (see MAINPROCWINDOWS::MainLoop's PeekMessage/DispatchMessage
      // pump), which blocks the app's normal per-frame Update/UpdateViewports call until the mouse
      // button is released -- WM_SIZE is the only thing that still gets dispatched, repeatedly, as
      // the border moves. Without re-presenting here, nothing paints the window for the whole drag,
      // so its own background brush (BLACK_BRUSH, see wndclass.hbrBackground) shows through as a
      // solid black window. Re-presenting the CURRENT screen canvas (the last frame the normal main
      // loop rendered; no UI re-render happens here, only presentation) through the same
      // UpdateViewports() the main loop already calls every frame keeps the window showing that
      // frame -- cropped/anchored to each intermediate size exactly like any other resize -- instead
      // of going black, for the whole duration of the drag.
      //
      // NOTE (Custom Chromes startup garbage): WM_SIZE is sent SYNCHRONOUSLY by Windows itself --
      // not queued through the app's own message pump -- as a direct side effect of CreateWindowEx,
      // ShowWindow, SetWindowPos, DPI changes, etc. That means it can and does fire during the still-
      // ongoing startup sequence (Ini_Graphics()/dashboard load in APPFLOWGRAPHICS::CreateMainScreenProcess,
      // well before MAINPROCWINDOWS::MainLoop ever starts pumping real frames), at a point where this
      // screen is already registered (see GRPWINDOWSSCREEN::Create()) but screencanvas may not yet hold
      // one single fully drawn frame. Calling UpdateViewports()->Update(screencanvas) at that moment
      // used to call Uncloak() (see Create_Window()/Show()/Update(GRP2DCANVAS*)) on whatever partial/stale
      // content happened to be in screencanvas right then -- lifting DWMWA_CLOAK, and hence exposing the
      // window to DWM composition, well before the real first frame was ready. Because WHETHER and WHEN
      // such a WM_SIZE fires during that startup window is entirely up to Windows/DPI/monitor timing (not
      // the app), this made the resulting garbage intermittent from run to run even with Custom Chromes
      // always active -- and it never affected native chromes because windowcloaked is never set true
      // there (see Create_Window()), so this same call was always a harmless no-op for that style. Skipping
      // the call entirely while still cloaked is safe: a cloaked window is not composed/visible at all, so
      // there is nothing on screen for this resize-drag repaint to usefully update yet; the ONLY call that
      // is allowed to clear the cloak remains the real Update(GRP2DCANVAS*) driven by the main loop's own
      // per-frame DrawFrame()/UpdateViewports() cycle, once startup has actually produced a real frame.
      case WM_SIZE                  : { GRPWINDOWSSCREEN* screen = (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);
                                        if(screen)
                                          {
                                            int clientwidth  = 0;
                                            int clientheight = 0;
                                            if(screen->GetClientSize(clientwidth, clientheight) && (clientwidth > 0) && (clientheight > 0))
                                              {
                                                if(((XDWORD)clientwidth != screen->GetWidth()) || ((XDWORD)clientheight != screen->GetHeight()))
                                                  {
                                                    screen->UpdateSize(clientwidth, clientheight);
                                                    // CHANGESIZE → Update() already ran inside UpdateSize. A second pass
                                                    // catches chrome dirtied by UIScale_Present in that first Update when
                                                    // the modal resize loop will only UpdateViewports() next (no main-loop
                                                    // tick until mouse-up) — without it the live canvas can stay blank
                                                    // for the rest of the drag.
                                                    #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
                                                    if(screen->IsCFGChromesActive())
                                                      {
                                                        GEN_USERINTERFACE.Update();
                                                      }
                                                    #endif
                                                  }
                                              }

                                            if(!screen->windowcloaked)
                                              {
                                                screen->UpdateViewports();
                                              }
                                          }
                                      }
                                      break;

      case WM_MOVE                  : { GRPWINDOWSSCREEN* screen =  (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);
                                        if(screen)
                                          {
                                            screen->SetPosition((int)(short) LOWORD(lparam), (int)(short) HIWORD(lparam));

                                            //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Windows] move: x=%04d, y=%04d (%04d,%04d)"), screen->GetPositionX(), screen->GetPositionY(), screen->GetWidth(), screen->GetHeight());
                                          }
                                      }
                                      break;

      case WM_CLOSE                 : {  GRPWINDOWSSCREEN* screen = (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);
                                         if(screen)
                                           {
                                            if(!screen->CanClose())
                                              {
                                                return 0;
                                              }
                                           }                                                                                                                             
                                      }  
                                      break;    

      case WM_DESTROY               : { GRPWINDOWSSCREEN* screen = (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);

                                        if(screen && mainprocwindows.GetAppMain())
                                          {
                                            APPFLOWGRAPHICS* appgraphics = dynamic_cast<APPFLOWGRAPHICS*>(mainprocwindows.GetAppMain()->GetApplication());
                                            if(appgraphics)
                                              {
                                                if(screen == appgraphics->GetMainScreen())
                                                  {
                                                    PostQuitMessage(0);
                                                  }
                                              }
                                          }                                        
                                      }
                                      break;

      case WM_WINDOWPOSCHANGING     : { GRPWINDOWSSCREEN* screen = (GRPWINDOWSSCREEN*)GRPSCREEN::GetListScreens()->Get((void*)hwnd);                                       
                                        if(screen)
                                          {   
                                            WINDOWPOS* pwp= (WINDOWPOS*)lparam;
                                            if(!pwp) 
                                              {
                                                break;
                                              }
  
                                            if(screen->Style_Is(GRPSCREENSTYLE_ZORDER))
                                              {
                                                pwp->flags |= SWP_NOZORDER;                                        
                                              }
                                          }
                                      }
                                      break;  

      case WM_PAINT                 : break;

      case WM_POWERBROADCAST        : //Computer is suspending
                                      if(wparam == PBT_APMSUSPEND)
                                        {
                                          if(mainprocwindows.GetAppMain())
                                            {
                                              if(mainprocwindows.GetAppMain()->GetApplication()) mainprocwindows.GetAppMain()->GetApplication()->SetSystemChangeStatus(XSYSTEM_CHANGESTATUSTYPE_SUSPEND);
                                            }                                          
                                        }

                                      break;

      case WM_QUERYENDSESSION       : //Computer is shutting down
                                      if(lparam == 0)
                                        {
                                          if(mainprocwindows.GetAppMain())
                                            {
                                              if(mainprocwindows.GetAppMain()->GetApplication()) mainprocwindows.GetAppMain()->GetApplication()->SetSystemChangeStatus(XSYSTEM_CHANGESTATUSTYPE_REBOOT);
                                            }                                          
                                        }

                                      //User is logging off
                                      if((lparam & ENDSESSION_LOGOFF) == ENDSESSION_LOGOFF)
                                        {                                          
                                          if(mainprocwindows.GetAppMain())
                                            {
                                              if(mainprocwindows.GetAppMain()->GetApplication()) mainprocwindows.GetAppMain()->GetApplication()->SetSystemChangeStatus(XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGOFF);
                                            }                                         
                                        }

                                      break;
    }
 
  return DefWindowProc(hwnd, msg, wparam, lparam);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPWINDOWSSCREEN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPWINDOWSSCREEN::Clean()
{
  hinstance               = NULL;
  hwnd                    = NULL;
  hdc                     = NULL;
  windowcloaked           = false;
  maximize_restore_valid  = false;
  maximize_restore_x      = 0;
  maximize_restore_y      = 0;
  maximize_restore_w      = 0;
  maximize_restore_h      = 0;

  #ifdef GRP_OPENGL_ACTIVE
  blitgles        = NULL;
  #endif
}


