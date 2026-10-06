/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPLINUXScreenX11.cpp
* 
* @class      GRPLINUXSCREENX11
* @brief      LINUX Graphics X11 Screen class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


#ifdef LINUX_X11_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include "GRPLINUXScreenX11.h"


#include <stdio.h>
#include <string.h>

#include "XTrace.h"
#include "XBuffer.h"

#include "GRP2DCanvas.h"
#include "GRPDesktopManager.h"
#include "GRPViewPort.h"

#include "MainProcLINUX.h"
#include "APPFlowGraphics.h"

#include <unistd.h>


#ifdef GRP_OPENGL_ACTIVE
#include "GRPLINUXBlitGLESX11.h"
#endif


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool IsRunningOnWSL()
* @brief      Detect WSL/WSLg at runtime by reading /proc/version.
* @ingroup    PLATFORM_LINUX
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool IsRunningOnWSL()
{
  FILE* f = fopen("/proc/version", "r");
  if(!f) return false;

  char buf[512];
  size_t n = fread(buf, 1, sizeof(buf)-1, f);

  fclose(f);

  buf[n] = '\0';

  return (strstr(buf, "microsoft") != NULL || strstr(buf, "Microsoft") != NULL || strstr(buf, "WSL") != NULL);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXSCREENX11::GRPLINUXSCREENX11(): GRPSCREEN()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENX11::GRPLINUXSCREENX11(): GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_LINUX_X11;

  //display = XOpenDisplay(":0.0");  
  display = XOpenDisplay(NULL);  
  if(!display)
    {
      return;
    }

  XSetErrorHandler(ErrorHandler);
    
  int width  = DisplayWidth  (display, DefaultScreen (display));
  int height = DisplayHeight (display, DefaultScreen (display));
  int depth  = DefaultDepth  (display, DefaultScreen (display));

  SetSize(width, height);
  SetMaxSize(width, height);

  /*
  switch(depth)
    {
      case   8 : SetMode(GRPPROPERTYMODE_08_INDEX);     break;
      case  16 : SetMode(GRPPROPERTYMODE_16_RGB_565);   break;
      case  24 : SetMode(GRPPROPERTYMODE_24_BGR_888);   break;
      case  32 : SetMode(GRPPROPERTYMODE_32_BGRA_8888); break;
    }
  */

  SetMode(GRPPROPERTYMODE_32_BGRA_8888);

  SetIsBufferInverse(true);
    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXSCREENX11::~GRPLINUXSCREENX11()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENX11::~GRPLINUXSCREENX11()
{
  if(!display)
    {
      return;
    }
      
  //ScreenResolution(display, root, originalwidth, originalheight, 60, false);

  Delete();

  if(movedisplay)
    {
      XCloseDisplay(movedisplay);
      movedisplay = NULL;
    }

  //XCloseDisplay(display);
 
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Create(bool show)
* @brief      Create resource
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Create(bool show)
{
  if(!Create_Window(show)) 
    {
      return false;
    }

  XFlush(display);

  return GRPSCREEN::Create(show);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  canvas : canvas handle
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Update(GRP2DCANVAS* canvas)
{
  if(!display)
    {
      return false;
    }

  if(!window)
    {
      return false;
    }

  if(!IsRunningOnWSL())
    {
      // Keep the stored window position (GetPositionX()/GetPositionY()) in sync with the window's REAL
      // position on the desktop. There is no ConfigureNotify handling on this backend, so without this the
      // stored position is whatever the app requested at creation -- and a native WM (KWin's smart placement,
      // for one) is free to put the window somewhere entirely different, or the user parks it elsewhere.
      // GRPSCREEN::UpdateCFGChromesDrag() builds all its desktop-space cursor math on GetPositionX()/Y(), so a
      // stale value teleported the window to the stale coordinates on the first caption click. Skipped on
      // WSLg, whose compositor-side phantom frame offsets make the translated origin unreliable there (and
      // whose windows never move anyway -- see the ancestor-walk workaround in Set_Position()).
      Window  translatechild = None;
      int     realx          = 0;
      int     realy          = 0;

      if(XTranslateCoordinates(display, window, root, 0, 0, &realx, &realy, &translatechild))
        {
          if((realx != GetPositionX()) || (realy != GetPositionY()))
            {
              SetPosition(realx, realy);
            }
        }

      // NOTE: the stored SIZE (GetWidth()/GetHeight()) is deliberately NOT synced here: engine-wide, those
      // members are the CONTENT (canvas design) size, constant for the life of the screen -- exactly how the
      // Windows backend treats them (its raster blit and mouse mapping are built on that assumption). Code
      // that needs the LIVE native client size queries GetClientSize() (virtual, overridden per platform)
      // instead.
    }

  if(wmdeletewindow != None)
    {
      XEvent event;

      if(XCheckTypedWindowEvent(display, window, ClientMessage, &event))
        {
          if((Atom)event.xclient.data.l[0] == wmdeletewindow)
            {
              if(CanClose())
                {
                  if(mainproclinux.GetAppMain())
                    {
                      APPFLOWGRAPHICS* appgraphics = dynamic_cast<APPFLOWGRAPHICS*>(mainproclinux.GetAppMain()->GetApplication());
                      if(appgraphics)
                        {
                          if((GRPSCREEN*)this == appgraphics->GetMainScreen())
                            {
                              appgraphics->SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                            }
                           else
                            {
                              Delete();
                            }
                        }
                    }                  
                }
            }
        }
    }


  if(!canvas)
    {
      return false;
    }

  // GEN's own resize grip for custom-chromes windows (no-op in every other mode). Runs BEFORE the
  // snap-back below so that, within the same frame, a grip-driven resize is still subject to the
  // very same viewport-max clamp as a WM-driven one.
  UpdateCustomChromesResize();

  // Reactive growth-cap snap-back: WM_NORMAL_HINTS' PMaxSize (see ApplyWMNormalHints()) is only a
  // hint some window managers do not honour for interactive resize -- unlike Win32's
  // WM_GETMINMAXINFO (which Windows itself enforces, synchronously, before the resize is ever
  // applied), X11 gives clients no veto over a WM-driven interactive resize or a WM-driven
  // maximize. This runs every frame, regardless of render path (GL or raster, hence placed before
  // the GL early-return below) and regardless of chromes mode, and is the actual WM-independent
  // guarantee that the window never PERSISTS beyond the viewport's max on either axis -- a brief
  // one-frame overshoot during an active drag is possible on non-compliant WMs, but it is
  // immediately snapped back, never left growing unbounded. Only fullscreen is excluded
  // (unrelated sizing, must not be clamped by the viewport max).
  if(!Styles_IsFullScreen())
    {
      // Lazy PMaxSize hint: ApplyWMNormalHints() is called from Create_Window() but the
      // GRPVIEWPORT_ID_MAIN viewport may not be configured yet at that point, causing it to
      // skip setting PMaxSize. Re-apply once from Update() the first time the viewport is
      // available, so KWin's maximize request is capped proactively (before the snap-back
      // even needs to trigger) on the very first frame after the viewport is set up.
      if(!normalhintsapplied && GetViewport(GRPVIEWPORT_ID_MAIN))
        {
          ApplyWMNormalHints();
          normalhintsapplied = true;
        }

      float maxw = 0.0f;
      float maxh = 0.0f;

      ResolveViewportMax(maxw, maxh);

      if((maxw > 0.0f) || (maxh > 0.0f))
        {
          XWindowAttributes wattr;
          if(XGetWindowAttributes(display, window, &wattr))
            {
              int cappedw = wattr.width;
              int cappedh = wattr.height;

              if((maxw > 0.0f) && (wattr.width  > (int)maxw)) cappedw = (int)maxw;
              if((maxh > 0.0f) && (wattr.height > (int)maxh)) cappedh = (int)maxh;

              if((cappedw != wattr.width) || (cappedh != wattr.height))
                {
                  // On EWMH-compliant WMs (KDE/KWin, GNOME/Mutter) a window in
                  // _NET_WM_STATE_MAXIMIZED is geometry-managed by the WM: any XResizeWindow
                  // from the client is intercepted (SubstructureRedirectMask on root) and
                  // overridden to maintain the maximized geometry. We must first ask the WM to
                  // leave the maximized state via a _NET_WM_STATE ClientMessage. Both this
                  // message and the XResizeWindow below are sent to the X server on the same
                  // connection, so they arrive in KWin's event queue in FIFO order -- KWin
                  // processes the de-maximize first, then the ConfigureRequest from XResizeWindow
                  // (which it now honours since the window is no longer maximized). On Weston/
                  // WSLg, XResizeWindow always works even while maximized, so this message is a
                  // no-op there. On WMs that ignore _NET_WM_STATE entirely, both are no-ops and
                  // the next frame will re-detect and retry.
                  Atom netstate = XInternAtom(display, "_NET_WM_STATE", False);
                  Atom maxhorz  = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
                  Atom maxvert  = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

                  if(netstate != None && maxhorz != None && maxvert != None)
                    {
                      XEvent ev;
                      memset(&ev, 0, sizeof(ev));
                      ev.type                 = ClientMessage;
                      ev.xclient.window       = window;
                      ev.xclient.message_type = netstate;
                      ev.xclient.format       = 32;
                      ev.xclient.data.l[0]    = 0;            // _NET_WM_STATE_REMOVE
                      ev.xclient.data.l[1]    = (long)maxhorz;
                      ev.xclient.data.l[2]    = (long)maxvert;
                      ev.xclient.data.l[3]    = 1;            // source: application (not pager)
                      ev.xclient.data.l[4]    = 0;
                      XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &ev);
                    }

                  XResizeWindow(display, window, cappedw, cappedh);
                  XFlush(display);
                }
            }
        }
    }

  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      return blitgles->Update(canvas);
    }
  #endif


  GC gc = XCreateGC(display, window, 0, NULL);
  if(gc)
    {
      XSync(display, false);

      int neww = (int)width;
      int newh = (int)height;

      XWindowAttributes wattr;
      if(XGetWindowAttributes(display, window, &wattr))
        {
          neww = wattr.width;
          newh = wattr.height;
        }

      // No bitmap rescaling on X11: the canvas is always blitted 1:1, cropped to the smaller of
      // the canvas' own (fixed design) size (width/height) and the window's current live size
      // (neww/newh), on each axis independently, and anchored to the window's TOP-LEFT corner --
      // growing the window reveals more of the existing background (background_pixel, set by
      // Create_Window()'s XCreateWindow() call) instead of stretching the canvas to fill it. If
      // the window's live size is below the GRPVIEWPORT_ID_MAIN viewport's declared minimum
      // (either axis), the content is hidden entirely this frame (XPutImage is not called, so the
      // window's own background shows through) instead of shrinking it to fit -- the X11 raster-
      // path equivalent of GRPWINDOWSSCREEN::Update()'s IsAboveViewportMinimumSize() guard.
      GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);

      bool belowviewportminimum = mainviewport &&
                                   ((mainviewport->GetMinWidth()  > 0.0f && neww < (int)mainviewport->GetMinWidth())  ||
                                    (mainviewport->GetMinHeight() > 0.0f && newh < (int)mainviewport->GetMinHeight()));

      XBYTE* srcbuffer = (XBYTE*)canvas->Buffer_Get();

      if(!belowviewportminimum && srcbuffer && (neww > 0) && (newh > 0))
        {
          int bw = ((int)width  < neww) ? (int)width  : neww;
          int bh = ((int)height < newh) ? (int)height : newh;

          XBYTE* blitbuffer = srcbuffer;
          bool   cropped    = false;
          bool   canblit    = true;

          if((bw != (int)width) || (bh != (int)height))
            {
              // Window smaller than the canvas on at least one axis: crop to the top-left bw x bh
              // region instead of blitting the whole (width x height) canvas.
              XBYTE* croppedbuffer = (XBYTE*)malloc((size_t)bw * (size_t)bh * 4);
              if(croppedbuffer)
                {
                  for(int y=0; y<bh; y++)
                    {
                      memcpy(croppedbuffer + (size_t)y * (size_t)bw * 4, srcbuffer + (size_t)y * (size_t)width * 4, (size_t)bw * 4);
                    }

                  blitbuffer = croppedbuffer;
                  cropped    = true;
                }
               else
                {
                  canblit = false;
                }
            }

          if(canblit)
            {
              XImage* image = CreateXImageFromBuffer(display, DefaultScreen(display), blitbuffer, bw, bh);
              if(image)
                {
                  XPutImage(display, window, gc, image, 0, 0, 0, 0, bw, bh);
                  XDestroyImage(image);
                }
            }

          if(cropped) free(blitbuffer);
        }

      XFreeGC(display, gc);
    }

  XFlush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::UpdateTransparent(GRP2DCANVAS* canvas)
* @brief      Update transparent
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::UpdateTransparent(GRP2DCANVAS* canvas)
{
  return Update(canvas);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Delete()
* @brief      Delete resource
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Delete()
{
  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      blitgles->Destroy();
      GEN_DELETE blitgles;
      blitgles = NULL;
    }
  #endif

  if(!display) return false;
  if(!window)  return false;

  XDestroyWindow(display, window);

  return GRPSCREEN::Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Resize(int width, int height)
* @brief      Resize
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Resize(int width, int height)
{
  // Clamp to the GRPVIEWPORT_ID_MAIN viewport's max size here too (not just via
  // ApplyWMNormalHints()/Update()'s reactive snap-back), so an API/script-driven Resize() call
  // (e.g. Script_Lib_Screen.cpp) requesting a size beyond the max settles at the cap immediately
  // instead of visibly growing for one frame and then being snapped back by Update() -- mirrors
  // Win32's WM_GETMINMAXINFO, which Windows sends (and enforces) for programmatic resizes too, not
  // just interactive drag. Not applied while fullscreen (unrelated sizing).
  if(!Styles_IsFullScreen())
    {
      float maxw = 0.0f;
      float maxh = 0.0f;

      ResolveViewportMax(maxw, maxh);

      if((maxw > 0.0f) && (width  > (int)maxw)) width  = (int)maxw;
      if((maxh > 0.0f) && (height > (int)maxh)) height = (int)maxh;
    }

  XResizeWindow(display, window, width, height);

  return UpdateSize(width, height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Get_Position(int& x, int& y)
* @brief      Get position
* @note       The window's REAL origin in root (desktop) coordinates, asked to the server. Base
*             GRPSCREEN::Get_Position() is a stub returning false; this is the X11 override.
*
*             XTranslateCoordinates and not XGetWindowAttributes: under a reparenting window manager the
*             attributes' x/y are relative to the WM's frame window, not to the root. This is the same call
*             Update() already uses once per rendered frame to refresh the stored position -- exposed here so
*             that code needing an up-to-the-moment position (the custom-caption drag takes its anchor from
*             it) does not depend on how long ago that frame happened to run.
* @ingroup    PLATFORM_LINUX
* 
* @param[out] x : X coordinate, root/desktop space.
* @param[out] y : Y coordinate, root/desktop space.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Get_Position(int& x, int& y)
{
  if(!display || !window)
    {
      return false;
    }

  // On WSLg the translated origin is unreliable (compositor-side phantom frame offsets, the very reason
  // Set_Position() needs its ancestor-walk workaround there), so keep using the stored value.
  if(IsRunningOnWSL())
    {
      return false;
    }

  Window  translatechild = None;
  int     realx          = 0;
  int     realy          = 0;

  if(!XTranslateCoordinates(display, window, root, 0, 0, &realx, &realy, &translatechild))
    {
      return false;
    }

  x = realx;
  y = realy;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Set_Position(int x, int y)
* @brief      Set position
* @note       Base GRPSCREEN::Set_Position() is a stub (returns false); this is the X11 override actually
*             moving the window, needed for e.g. custom-chrome window dragging (see
*             GRPSCREEN::UpdateCFGChromesDrag(), entirely platform-independent, calls this virtual).
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Set_Position(int x, int y)
{
  if(!display) return false;
  if(!window)  return false;

  SetPosition(x, y);

  if(!IsRunningOnWSL())
    {
      // Native, ICCCM-compliant WM (KWin, Mutter, Xfwm...): a managed top-level window moves itself by issuing
      // the request on ITS OWN window; the WM intercepts it (SubstructureRedirect on the frame) and repositions
      // the whole frame accordingly. The ancestor-walk below (the WSLg workaround) must NOT run here: under a
      // reparenting WM it climbs into the WM's OWN frame window, and XMoveWindow on a window that belongs to
      // the window manager is ignored/reverted by KWin -- which is exactly why caption-dragging moved nothing
      // on native KDE while working fine on WSLg.
      XMoveWindow(display, window, x, y);
      XFlush(display);

      return true;
    }

  Window towindow      = window;
  Window current       = window;
  int    totaloffsetx  = 0;
  int    totaloffsety  = 0;

  while(true)
    {
      Window       rootcheck = 0;
      Window       parent    = 0;
      Window*      children  = NULL;
      unsigned int nchildren = 0;

      if(!XQueryTree(display, current, &rootcheck, &parent, &children, &nchildren)) break;
      if(children) XFree(children);

      if((parent == None) || (parent == root)) break;    // current already hangs directly off the root

      XWindowAttributes currentattr;

      memset(&currentattr, 0, sizeof(currentattr));

      if(XGetWindowAttributes(display, current, &currentattr))
        {
          totaloffsetx += currentattr.x;
          totaloffsety += currentattr.y;
        }
      
      current  = parent;
      towindow = current;
    }

  #ifdef XTRACE_ACTIVE
  {
    // Checking the PID-ownership theory: if WSLg is deliberately ignoring geometry requests coming from the
    // very process it recorded as this window's owner (_NET_WM_PID), that property should read back as our
    // own getpid() here.
    Atom          netwmpidatom = XInternAtom(display, "_NET_WM_PID", True);
    Atom          actualtype   = None;
    int           actualformat = 0;
    unsigned long nitems       = 0;
    unsigned long bytesafter   = 0;
    unsigned char* proplinux   = NULL;

    long ourpid    = (long)getpid();
    long windowpid = -1;
    long towinpid  = -1;

    if((netwmpidatom != None) &&
       (XGetWindowProperty(display, window, netwmpidatom, 0, 1, False, XA_CARDINAL, &actualtype, &actualformat, &nitems, &bytesafter, &proplinux) == Success) &&
       proplinux && (nitems > 0))
      {
        windowpid = (long)(*(unsigned long*)proplinux);
        XFree(proplinux);
      }

    proplinux = NULL;

    if((netwmpidatom != None) && (towindow != window) &&
       (XGetWindowProperty(display, towindow, netwmpidatom, 0, 1, False, XA_CARDINAL, &actualtype, &actualformat, &nitems, &bytesafter, &proplinux) == Success) &&
       proplinux && (nitems > 0))
      {
        towinpid = (long)(*(unsigned long*)proplinux);
        XFree(proplinux);
      }
   
  }
  #endif

  if(!movedisplay)
    {
      movedisplay = XOpenDisplay(NULL);
    }

  if(movedisplay)
    {
      XMoveWindow(movedisplay, towindow, x - totaloffsetx, y - totaloffsety);
      XSync(movedisplay, False);
    }
   else
    {
      // Extremely unlikely (the main connection to the very same server already succeeded), but keep a
      // fallback so a move is at least attempted rather than silently doing nothing.
      XMoveWindow(display, towindow, x - totaloffsetx, y - totaloffsety);
      XSync(display, False);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::GetClientSize(int& width, int& height)
* @brief      Get client size: LIVE size of the native window (X11 override of the base's content-size
*             fallback). Queried fresh from the server each call, so it tracks interactive resizes (the
*             custom-chromes grip, a WM-driven resize, maximize...) with no dependence on any cached member.
* @ingroup    PLATFORM_LINUX
*
* @param[out] width : live client area width.
* @param[out] height : live client area height.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::GetClientSize(int& width, int& height)
{
  if(!display || !window)
    {
      return GRPSCREEN::GetClientSize(width, height);
    }

  XWindowAttributes wattr;

  if(!XGetWindowAttributes(display, window, &wattr))
    {
      return GRPSCREEN::GetClientSize(width, height);
    }

  width  = wattr.width;
  height = wattr.height;

  return true;
}


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::GetCursorDesktopPosition(int& x, int& y)
* @brief      Get cursor desktop position
* @note       XQueryPointer against the ROOT window: root_x/root_y is the pointer in desktop coordinates, with
*             no reference at all to this window's own position. That matters twice over on X11:
*
*               - XMoveWindow() is asynchronous (a request the window manager honours whenever it gets to it,
*                 and on a reparenting WM like KWin it travels through the frame window -- see Set_Position()).
*                 GetPositionX() therefore holds a REQUESTED position that can be several milliseconds ahead of
*                 the real one. The old drag math added that requested position to a client-relative pointer
*                 reading taken against the REAL one, and the difference went straight into the movement: the
*                 window could travel further than the mouse did.
*               - The client-relative reading is only valid while the pointer is over the window; the root one
*                 is always valid.
*
*             INPLINUXDEVICEMOUSEX11::Update() already asks the server for exactly these two values on every
*             tick and discards them; this is the same round trip, kept here so GRPSCREEN does not have to
*             reach into a platform input device.
* @ingroup    PLATFORM_LINUX
*
* @param[out] x : X coordinate, desktop (root) space.
* @param[out] y : Y coordinate, desktop (root) space.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::GetCursorDesktopPosition(int& x, int& y)
{
  if(!display)
    {
      return false;
    }

  Window        root      = 0;
  Window        child     = 0;
  int           rootx     = 0;
  int           rooty     = 0;
  int           winx      = 0;
  int           winy      = 0;
  unsigned int  mask      = 0;

  if(!XQueryPointer(display, DefaultRootWindow(display), &root, &child, &rootx, &rooty, &winx, &winy, &mask))
    {
      // False means the pointer is on another screen of the same display; nothing usable this tick.
      return false;
    }

  x = rootx;
  y = rooty;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::GetCFGChromesDragAnchor(int& x, int& y)
* @brief      Get CFG chromes drag anchor
* @note       The live position from Get_Position(). The stored GetPositionX()/GetPositionY() is only
*             refreshed once per rendered frame (Update(), through the same XTranslateCoordinates call), so a
*             window that the window manager or the user moved since then would anchor the drag to a position
*             the window no longer has, and the very first tick would teleport it there. Returns false on
*             WSLg -- where Get_Position() is not trustworthy -- and the caller then falls back to the stored
*             value, which is what that backend has always used.
* @ingroup    PLATFORM_LINUX
*
* @param[out] x : X coordinate, root/desktop space.
* @param[out] y : Y coordinate, root/desktop space.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::GetCFGChromesDragAnchor(int& x, int& y)
{
  return Get_Position(x, y);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::SystemMove()
* @brief      System move
* @note       EWMH _NET_WM_MOVERESIZE with _NET_WM_MOVERESIZE_MOVE: asks the window manager to run the
*             interactive move itself, from the pointer's current position, exactly as if the user had grabbed
*             a native title bar. Every compliant WM implements it (KWin, Mutter -- including the XWayland
*             case -- Xfwm, Openbox), and it is what Chrome, Electron and Qt's startSystemMove() use for their
*             own custom title bars.
*
*             This matters beyond smoothness: several window managers, Mutter in particular, are free to
*             ignore or constrain a plain XMoveWindow() coming from the client (which is all
*             GRPLINUXSCREENX11::Set_Position() can do), and then the in-process drag computes correct
*             positions that the window simply never takes. _NET_WM_MOVERESIZE is the sanctioned request and
*             is honoured.
*
*             Returns false -- having done nothing -- when the WM does not advertise the atom in
*             _NET_SUPPORTED, so the in-process drag stays in charge on non-compliant setups.
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the window manager took over the move; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::SystemMove()
{
  if(!display || !window)
    {
      return false;
    }

  // WSLg: Set_Position()'s ancestor-walk workaround exists precisely because that compositor does not behave
  // like a normal WM here. Do not add a second, differently-broken path on top of it.
  if(IsRunningOnWSL())
    {
      return false;
    }

  Atom moveresizeatom = XInternAtom(display, "_NET_WM_MOVERESIZE", True);
  if(moveresizeatom == None)
    {
      return false;
    }

  if(!IsWMSupported(moveresizeatom))
    {
      return false;
    }

  // The WM starts its move loop from where the pointer is NOW, so it needs the current root position.
  Window        queryroot  = None;
  Window        querychild = None;
  int           rootx      = 0;
  int           rooty      = 0;
  int           winx       = 0;
  int           winy       = 0;
  unsigned int  mask       = 0;

  if(!XQueryPointer(display, window, &queryroot, &querychild, &rootx, &rooty, &winx, &winy, &mask))
    {
      return false;
    }

  // The pointer must be released first: the WM cannot run its own grab while this client holds one, and the
  // mouse device installs a passive XGrabButton on this window that the press has already activated.
  XUngrabPointer(display, CurrentTime);
  XFlush(display);

  XEvent event;

  memset(&event, 0, sizeof(event));

  event.xclient.type         = ClientMessage;
  event.xclient.window       = window;
  event.xclient.message_type = moveresizeatom;
  event.xclient.format       = 32;
  event.xclient.data.l[0]    = rootx;
  event.xclient.data.l[1]    = rooty;
  event.xclient.data.l[2]    = 8;             // _NET_WM_MOVERESIZE_MOVE
  event.xclient.data.l[3]    = Button1;
  event.xclient.data.l[4]    = 1;             // source indication: normal application

  if(!XSendEvent(display, root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event))
    {
      return false;
    }

  XFlush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::IsWMSupported(Atom atom)
* @brief      Is WM supported
* @note       INTERNAL: true when the running window manager lists the given atom in the root window's
*             _NET_SUPPORTED property, i.e. when it actually implements that part of EWMH.
* @ingroup    PLATFORM_LINUX
*
* @param[in]  atom : the EWMH atom to look for.
*
* @return     bool : true if the window manager advertises support for it.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::IsWMSupported(Atom atom)
{
  if(!display || (atom == None))
    {
      return false;
    }

  Atom supportedatom = XInternAtom(display, "_NET_SUPPORTED", True);
  if(supportedatom == None)
    {
      return false;
    }

  Atom            actualtype   = None;
  int             actualformat = 0;
  unsigned long   nitems       = 0;
  unsigned long   bytesafter   = 0;
  unsigned char*  prop         = NULL;
  bool            found        = false;

  if(XGetWindowProperty(display, root, supportedatom, 0, 1024, False, XA_ATOM,
                        &actualtype, &actualformat, &nitems, &bytesafter, &prop) != Success)
    {
      return false;
    }

  if(prop)
    {
      if((actualtype == XA_ATOM) && (actualformat == 32))
        {
          Atom* atoms = (Atom*)prop;

          for(unsigned long c=0; c<nitems; c++)
            {
              if(atoms[c] == atom)
                {
                  found = true;
                  break;
                }
            }
        }

      XFree(prop);
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::BeginCFGChromesDrag()
* @brief      Begin CFG chromes drag
* @note       Grabs the pointer for the whole caption drag so motion and, above all, the ButtonRelease that
*             ends it keep being delivered to this window even while the pointer is outside it. Without the
*             grab a fast drag that let the pointer slip off the window left it stopped, unable to catch up.
*             GrabModeAsync on both pointer and keyboard: the grab must never freeze event processing.
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::BeginCFGChromesDrag()
{
  if(!display || !window)
    {
      return false;
    }

  // owner_events = False, deliberately: it is what INPLINUXDEVICEMOUSEX11::SetScreen()'s passive XGrabButton
  // on this same window already uses, and the window itself only ever selects KeyPress/KeyRelease through
  // XSelectInput -- so button events do NOT reach us "normally", they reach us because a grab reports them
  // with respect to the grab window. Passing True here would leave that routing depending on a selection the
  // window does not have. With False every ButtonPress/ButtonRelease/MotionNotify is reported against this
  // window with this mask, which is exactly what INPLINUXDEVICEMOUSEX11::Update()'s XCheckWindowEvent() looks
  // for -- including, crucially, the release that ends the drag when the pointer is outside the window.
  if(XGrabPointer(display, window, False,
                  ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                  GrabModeAsync, GrabModeAsync, None, None, CurrentTime) != GrabSuccess)
    {
      // Somebody else owns the pointer (a WM menu, another grab...). NOT fatal, and the caller ignores this
      // result on purpose: the drag still tracks correctly through the root-relative reading in
      // GetCursorDesktopPosition(), it just is not exclusive.
      return false;
    }

  XFlush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::EndCFGChromesDrag()
* @brief      End CFG chromes drag
* @note       Counterpart of BeginCFGChromesDrag(). XUngrabPointer on a connection that holds no grab is a
*             no-op for the server, so this is safe to call unmatched.
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::EndCFGChromesDrag()
{
  if(!display)
    {
      return false;
    }

  XUngrabPointer(display, CurrentTime);
  XFlush(display);

  return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::Show(bool active)
* @brief      Show
* @ingroup    PLATFORM_LINUX
*
* @param[in]  active : Active value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Show(bool active)
{
  isshow  = active;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::ShowCursor(bool active)
* @brief      Show cursor
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::ShowCursor(bool active)
{
  if(!active)
    {
      // vars to make blank cursor
      Pixmap  blank;
      XColor  dummy;
      char    data[1] = { 0 };
      Cursor  cursor;

      // make a blank cursor
      blank = XCreateBitmapFromData(display, window, data, 1, 1);
      if(blank != None)
        {
          cursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
          XFreePixmap(display, blank);
          XDefineCursor(display, window, cursor);

        } else return false;
    }
   else
    {
      XUndefineCursor(display, window);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Minimize(bool active)
* @brief      Minimize
* @note       Base GRPSCREEN::Minimize() is a stub (returns false); this is the X11 override. active=true
*             iconifies the window (ICCCM XIconifyWindow()); active=false maps it back, which is the standard
*             way of asking an EWMH-compliant window manager to de-iconify a window it manages.
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Minimize(bool active)
{
  if(!display) return false;
  if(!window)  return false;

  if(active)
    {
      return (XIconifyWindow(display, window, DefaultScreen(display)) != 0);
    }

  XMapWindow(display, window);
  XFlush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::Maximize(bool active)
* @brief      Maximize
* @note       Base GRPSCREEN::Maximize() is a stub (returns false); this is the X11 override. There is no
*             single "maximize" call in X11 the way Win32 has ShowWindow(SW_SHOWMAXIMIZED): the standard EWMH
*             way is asking the window manager, via a _NET_WM_STATE client message sent to the root window, to
*             add (active=true) or remove (active=false) both _NET_WM_STATE_MAXIMIZED_HORZ and
*             _NET_WM_STATE_MAXIMIZED_VERT -- unlike the window-creation-time state hints elsewhere in this
*             file (set directly via XChangeProperty before the window is ever mapped), a state change at
*             runtime like this one must go through the WM using this message, per the EWMH spec.
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Maximize(bool active)
{
  if(!display) return false;
  if(!window)  return false;

  // Already as big as the viewport cap allows: maximizing could not grow the window, and the WM (KWin,
  // Mutter...) would still relocate it to the work area's top-left corner while keeping the capped size
  // (PMaxSize). Per design the window must stay EXACTLY as it is (same size, same position), so this is a
  // no-op. Same guard as GRPWINDOWSSCREEN::Maximize(), common rule in GRPSCREEN::IsClientSizeAtMaximum().
  if(active && IsClientSizeAtMaximum())
    {
      return true;
    }

  Atom wm_state      = XInternAtom(display, "_NET_WM_STATE", False);
  Atom wm_state_maxh = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  Atom wm_state_maxv = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

  if((wm_state == None) || (wm_state_maxh == None) || (wm_state_maxv == None)) return false;

  XEvent xevent;

  memset(&xevent, 0, sizeof(xevent));

  xevent.type                 = ClientMessage;
  xevent.xclient.window       = window;
  xevent.xclient.message_type = wm_state;
  xevent.xclient.format       = 32;
  xevent.xclient.data.l[0]    = active ? 1 : 0;    // 1 = _NET_WM_STATE_ADD, 0 = _NET_WM_STATE_REMOVE
  xevent.xclient.data.l[1]    = (long)wm_state_maxh;
  xevent.xclient.data.l[2]    = (long)wm_state_maxv;
  xevent.xclient.data.l[3]    = 1;                 // source indication: normal application
  xevent.xclient.data.l[4]    = 0;

  XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &xevent);

  XFlush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPLINUXSCREENX11::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_LINUX
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPLINUXSCREENX11::GetHandle()
{
  return (void*)window;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::IsDesktop()
* @brief      Is desktop
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool  GRPLINUXSCREENX11::IsDesktop()
{
  return isdesktop;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXSCREENX11::SetIsDesktop(bool isdesktop)
* @brief      Set is desktop
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  isdesktop : set is run in desktop
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::SetIsDesktop(bool isdesktop)
{
  this->isdesktop = isdesktop;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::HasFocus()
* @brief      Has focus
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::HasFocus()
{
  Window focus;
  int    status = 0;

  XGetInputFocus(display, &focus, &status);

  if(focus ==  window) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         Display* GRPLINUXSCREENX11::GetDisplay()
* @brief      Get display
* @ingroup    PLATFORM_LINUX
* 
* @return     Display* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
Display* GRPLINUXSCREENX11::GetDisplay()
{
  return display;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         Window* GRPLINUXSCREENX11::GetWindow()
* @brief      Get window
* @ingroup    PLATFORM_LINUX
* 
* @return     Window* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
Window* GRPLINUXSCREENX11::GetWindow()
{
  return &window;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         Window* GRPLINUXSCREENX11::GetWindowRoot()
* @brief      Get window root
* @ingroup    PLATFORM_LINUX
* 
* @return     Window* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
Window* GRPLINUXSCREENX11::GetWindowRoot()
{
  return &root;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVisualInfo* GRPLINUXSCREENX11::GetVisualInfo()
* @brief      Get visual info
* @ingroup    PLATFORM_LINUX
* 
* @return     XVisualInfo* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVisualInfo* GRPLINUXSCREENX11::GetVisualInfo()
{
  return &vinfo;
}


#ifdef GRP_OPENGL_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXBLITGLESX11* GRPLINUXSCREENX11::GetBlitGLES()
* @brief      Get OpenGL ES blitter (only present when GRP_OPENGL_ACTIVE is defined)
* @ingroup    PLATFORM_LINUX
* 
* @return     GRPLINUXBLITGLESX11* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLESX11* GRPLINUXSCREENX11::GetBlitGLES()
{
  return blitgles;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXSCREENX11::Chromes_ApplyStyle()
* @brief      Chromes apply style
* @note       INTERNAL: resolves the current GRPSCREENCFGCHROMES configuration (native X11 chromes only, for now)
*             into a _MOTIF_WM_HINTS property on the already-created window -- the X11 counterpart of
*             GRPWINDOWSSCREEN::Chromes_ApplyStyle()'s WS_ / WS_EX_ bits: caption on/off drives the whole set of
*             border/title/menu decorations at once, minimize/maximize/close/resize are each added independently
*             on top of that. Needs the window to already exist (unlike Windows' style bits, _MOTIF_WM_HINTS is a
*             window property, not a creation-time flag), so this always runs after XCreateWindow(). Only called
*             when a Chromes configuration is active, native chromes are requested, and the screen is not
*             transparent (Fullscreen and Transparent styles are prioritary over the Chromes configuration).
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::Chromes_ApplyStyle()
{
  GRPSCREENCFGCHROMES* cfgchromes = GetCFGChromes();
  if(!cfgchromes) return;

  // only_if_exists = False: creates the atom if absent, exactly like the custom-chromes block in
  // Create_Window(). With True, on sessions where no client has interned this atom yet (possible on
  // KDE Plasma 6 / KWin 6 at early startup), the call returns None and the whole function exits
  // silently -- no hints are set, KWin sees an undecorated window with no Motif hints and applies
  // its FULL default chrome (all buttons: minimize, maximize, close). That is exactly why
  // SetNativeMinimizeActive(false) / SetNativeMaximizeActive(false) had no visible effect.
  Atom wmhintsatom = XInternAtom(display, "_MOTIF_WM_HINTS", False);
  if(wmhintsatom == None)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom _MOTIF_WM_HINTS (native chromes)"));
      return;
    }

  HITNS hints;

  memset(&hints, 0, sizeof(hints));

  hints.flags = GRPLINUXSCREENX11_MWM_HINTS_DECORATIONS | GRPLINUXSCREENX11_MWM_HINTS_FUNCTIONS;

  if(cfgchromes->GetNativeCaptionActive())
    {
      hints.decorations = GRPLINUXSCREENX11_MWM_DECOR_BORDER | GRPLINUXSCREENX11_MWM_DECOR_TITLE | GRPLINUXSCREENX11_MWM_DECOR_MENU;
      hints.functions   = GRPLINUXSCREENX11_MWM_FUNC_MOVE;

      if(cfgchromes->GetNativeMinimizeActive())
        {
          hints.decorations |= GRPLINUXSCREENX11_MWM_DECOR_MINIMIZE;
          hints.functions   |= GRPLINUXSCREENX11_MWM_FUNC_MINIMIZE;
        }

      if(cfgchromes->GetNativeMaximizeActive())
        {
          hints.decorations |= GRPLINUXSCREENX11_MWM_DECOR_MAXIMIZE;
          hints.functions   |= GRPLINUXSCREENX11_MWM_FUNC_MAXIMIZE;
        }

      if(cfgchromes->GetNativeCloseActive())
        {
          hints.functions |= GRPLINUXSCREENX11_MWM_FUNC_CLOSE;
        }
    }
   else
    {
      hints.decorations = 0;
      hints.functions    = GRPLINUXSCREENX11_MWM_FUNC_MOVE;
    }

  if(cfgchromes->GetResizeActive())
    {
      hints.decorations |= GRPLINUXSCREENX11_MWM_DECOR_RESIZEH;
      hints.functions   |= GRPLINUXSCREENX11_MWM_FUNC_RESIZE;
    }

  XChangeProperty(display, window, wmhintsatom, wmhintsatom, 32, PropModeReplace, (unsigned char*)&hints, 5);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXSCREENX11::Chromes_ApplyPostCreate()
* @brief      Chromes apply post create
* @note       INTERNAL: applies the parts of the GRPSCREENCFGCHROMES configuration that only make sense once the
*             window already exists (title text, application icon, close), for native X11 chromes. The close
*             configuration reuses the existing CanClose() mechanism (GRPSCREEN base class), already checked by
*             Update() on every WM_DELETE_WINDOW, so it keeps being honoured exactly like on every other platform.
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::Chromes_ApplyPostCreate()
{
  if(!window) return;

  GRPSCREENCFGCHROMES* cfgchromes = GetCFGChromes();
  if(!cfgchromes) return;

  if(cfgchromes->GetNativeTitleActive() && !title.IsEmpty())
    {
      XBUFFER xbuffer;

      title.ConvertToASCII(xbuffer);

      XStoreName(display, window, (char*)xbuffer.Get());
    }
   else
    {
      XStoreName(display, window, "");
    }

  if(cfgchromes->GetNativeCaptionActive() && !cfgchromes->GetNativeIconActive())
    {
      // NOTE: best-effort only -- whether/how a WM shows an app icon in the titlebar (or anywhere else) at
      // all is entirely up to its own theme; clearing both the modern (_NET_WM_ICON) and legacy (WM_HINTS)
      // icon sources is the standard way to ask for none, mirroring what WM_SETICON(NULL) does on Windows.
      Atom netwmicon = XInternAtom(display, "_NET_WM_ICON", True);
      if(netwmicon != None)
        {
          XDeleteProperty(display, window, netwmicon);
        }

      XWMHints wmhints;

      memset(&wmhints, 0, sizeof(wmhints));

      wmhints.flags       = IconPixmapHint;
      wmhints.icon_pixmap = None;

      XSetWMHints(display, window, &wmhints);
    }

  SetCanClose(cfgchromes->GetNativeCloseActive());

  // ---- _NET_WM_WINDOW_TYPE (button suppression for KWin 6+) -----------------------------------------
  // KWin 6 does NOT use _MOTIF_WM_HINTS.functions to control button VISIBILITY for NORMAL windows:
  //   • It always considers NORMAL windows minimizable (isMinimizable() == true unconditionally),
  //     so MWM_FUNC_MINIMIZE absent from functions has NO effect on the minimize button.
  //   • It derives "can maximize?" from isResizable() (i.e. whether MWM_FUNC_RESIZE is present),
  //     not from MWM_FUNC_MAXIMIZE -- confirmed by user testing: SetResizeActive(false) makes the
  //     maximize button disappear; SetResizeActive(true) always shows it regardless of
  //     SetNativeMaximizeActive(false).
  //   • _NET_WM_ALLOWED_ACTIONS set by the client before mapping IS overwritten by KWin's own
  //     updateAllowedActions() when the window is first managed, so that approach does not work.
  //
  // The only reliable EWMH mechanism to suppress both buttons across KWin 6 is _NET_WM_WINDOW_TYPE:
  // when SetNativeMinimizeActive(false) && SetNativeMaximizeActive(false), declare the window as
  // _NET_WM_WINDOW_TYPE_UTILITY ("floating tool window / palette"). KWin treats UTILITY windows as
  // non-minimizable and non-maximizable, so no minimize/maximize buttons appear in the decoration.
  // The window is still resizable (UTILITY windows have a resize border) and still has a close
  // button. The Motif hints from Chromes_ApplyStyle() still carry the correct bits for other WMs
  // that DO honour them (Openbox, XFWM, Metacity, etc.).
  //
  // When only ONE of the two buttons is suppressed (e.g. no-maximize but keep minimize), KWin 6
  // has no per-button standard mechanism -- UTILITY hides both, NORMAL shows both (ignoring hints).
  // Motif hints still handle those intermediate cases on WMs that respect them.
  // ----------------------------------------------------------------------------------------------------
  if(cfgchromes->GetNativeCaptionActive()   &&
     !cfgchromes->GetNativeMinimizeActive() &&
     !cfgchromes->GetNativeMaximizeActive())
    {
      Atom netwmwindowtype  = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
      Atom netwmtypeutility = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", False);

      if(netwmwindowtype != None && netwmtypeutility != None)
        {
          XChangeProperty(display, window, netwmwindowtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&netwmtypeutility, 1);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::IsUsingCustomChromes()
* @brief      Single source of truth for "is this screen using GEN's own custom-drawn chrome instead of
*             native/WM decoration" -- see the header declaration for the full rationale. Selects the
*             no-native-decoration-at-all path in Create_Window() (_MOTIF_WM_HINTS decorations = 0) and
*             enables GEN's own resize grip (UpdateCustomChromesResize()) in its place.
* @note       Does NOT gate the growth cap: ApplyWMNormalHints() and Update()'s reactive snap-back apply
*             to custom-chromes windows exactly like to any other.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if custom (non-native) chromes are active for this screen.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::IsUsingCustomChromes()
{
  return (IsCFGChromesActive()                     &&
          !GetCFGChromes()->GetUseNativeChromes()  &&
          !Style_Is(GRPSCREENSTYLE_TRANSPARENT));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENX11::ResolveViewportMax(float& maxw, float& maxh)
* @brief      Resolves the GRPVIEWPORT_ID_MAIN viewport's max size, shared by ApplyWMNormalHints() and
*             Update()'s reactive snap-back. See the header declaration for the full fallback chain
*             (explicit viewport max -> viewport's own declared size -> this screen's own GRPPROPERTIES max).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[out] maxw : Resolved max width. Left UNTOUCHED if no viewport is configured yet.
* @param[out] maxh : Resolved max height. Left UNTOUCHED if no viewport is configured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::ResolveViewportMax(float& maxw, float& maxh)
{
  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return; // no viewport configured yet: caller keeps whatever it had
    }

  maxw = mainviewport->GetMaxWidth();
  maxh = mainviewport->GetMaxHeight();

  // No EXPLICIT SetMaxSize() on this axis: fall back to the viewport's own declared size, so the
  // window is capped even when the app never calls the new SetMaxSize() API.
  if(maxw <= 0.0f) maxw = mainviewport->GetWidth();
  if(maxh <= 0.0f) maxh = mainviewport->GetHeight();

  // Final safety net: if one axis is still unresolved while the other DOES have a real value,
  // never hand a literal 0 on just that one axis to a caller (some WMs could read a 0 max_width/
  // max_height as "zero-size allowed" rather than "unconstrained") -- fall back to this screen's
  // own GRPPROPERTIES max (set in the constructor to the X11 display's own resolution), so both
  // axes always end up with a real, non-zero ceiling together.
  if(maxw <= 0.0f) maxw = (float)GetMaxWidth();
  if(maxh <= 0.0f) maxh = (float)GetMaxHeight();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENX11::ApplyWMNormalHints()
* @brief      X11 equivalent of GRPWINDOWSSCREEN::ApplyResizeLimits's WM_GETMINMAXINFO handling: caps how
*             big this window can become at the GRPVIEWPORT_ID_MAIN viewport's max size (width and height
*             independently), via a PMaxSize XSizeHints property (XSetWMNormalHints). Compliant EWMH window
*             managers honour WM_NORMAL_HINTS' PMaxSize both for interactive border-drag resize AND for
*             _NET_WM_STATE_MAXIMIZED_HORZ/VERT maximize requests -- Maximize() below is deliberately left
*             untouched, it already sends the correct EWMH ClientMessage; setting this hint is what makes
*             that existing maximize request respect the viewport's max instead of filling the whole monitor.
* @note       No minimum is enforced here (no PMinSize set): the window is left free to shrink as usual; see
*             GRPLINUXBLITGLESX11::ComputePresentationScale (GL path) and Update() (raster path) for how the
*             rendered content is hidden (not the window itself) below the viewport's minimum.
* @note       Called from Create_Window() for EVERY non-fullscreen window, custom-chromes ones included: a
*             size hint only tells the window manager which sizes are ALLOWED, it never asks it to DRAW
*             anything, so it cannot bring back native decoration (only _MOTIF_WM_HINTS' decorations field
*             does that -- see Create_Window()'s custom-chromes block). Fullscreen is the only exclusion
*             (unrelated sizing, must not be clamped by the viewport max). Since a hint is only ever a
*             REQUEST, Update()'s reactive snap-back is what actually GUARANTEES the cap; this just lets a
*             cooperating WM get it right the first time, without the one-frame overshoot.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::ApplyWMNormalHints()
{
  if(!display || !window)
    {
      return;
    }

  float maxw = 0.0f;
  float maxh = 0.0f;

  ResolveViewportMax(maxw, maxh);

  if(maxw <= 0.0f && maxh <= 0.0f)
    {
      return; // no viewport configured yet / viewport has no usable size at all yet
    }

  XSizeHints hints;

  memset(&hints, 0, sizeof(hints));

  hints.flags      = PMaxSize;
  hints.max_width  = (int)maxw;
  hints.max_height = (int)maxh;

  XSetWMNormalHints(display, window, &hints);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENX11::UpdateCustomChromesResize()
* @brief      GEN's own resize grip, for custom-chromes windows only: a band of
*             GRPLINUXSCREENX11_CUSTOMRESIZE_BORDER pixels along the INSIDE of the window's right and bottom
*             edges which, when dragged with the left mouse button, resizes the window in-process via
*             XResizeWindow. This exists because a custom-chromes window is created with _MOTIF_WM_HINTS
*             decorations = 0 -- completely undecorated, the only value that reliably suppresses the native
*             titlebar across window managers (see Create_Window()) -- which necessarily also gives up the
*             WM-drawn resize border. Exactly the approach a client-side-decorated GTK/Qt application takes.
* @note       Right, bottom and bottom-right corner only, never left or top. Two reasons: the canvas is
*             anchored to the window's TOP-LEFT corner and never rescaled (see
*             GRPLINUXBLITGLESX11::ComputePresentationScale), so these are the edges whose movement actually
*             changes what is visible; and keeping the top edge out of it means this can never fight
*             GRPSCREEN::UpdateCFGChromesDrag()'s caption drag, which lives up there.
* @note       A resize only ever STARTS on a fresh press that lands inside the band (a press that began
*             anywhere else and merely moved over it does not count), and every subsequent frame recomputes
*             the size from the size/pointer position captured at that moment -- never incrementally from the
*             previous frame -- so a pointer that outruns the window, or leaves it entirely, cannot make the
*             size drift. The result is clamped to the GRPVIEWPORT_ID_MAIN viewport's max exactly like every
*             other resize path in this class, so the grip can never be used to exceed it.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::UpdateCustomChromesResize()
{
  if(!display || !window) return;

  if(!IsUsingCustomChromes() || Styles_IsFullScreen() || !GetCFGChromes()->GetResizeActive())
    {
      customresizing          = false;
      customresizeprevpressed = false;

      return;
    }

  Window        rootreturn  = 0;
  Window        childreturn = 0;
  int           rootx       = 0;
  int           rooty       = 0;
  int           winx        = 0;
  int           winy        = 0;
  unsigned int  mask        = 0;

  if(!XQueryPointer(display, window, &rootreturn, &childreturn, &rootx, &rooty, &winx, &winy, &mask))
    {
      customresizing          = false;
      customresizeprevpressed = false;

      return;
    }

  bool pressed = ((mask & Button1Mask) ? true : false);

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  // The caption drag owns the pointer while it is running: never try to resize at the same time.
  if(cfgchromesdragging)
    {
      customresizing          = false;
      customresizeprevpressed = pressed;

      return;
    }
  #endif

  if(!customresizing)
    {
      if(pressed && !customresizeprevpressed)
        {
          XWindowAttributes wattr;

          if(XGetWindowAttributes(display, window, &wattr))
            {
              int edge = 0;

              if((winx >= 0) && (winx <= wattr.width) && (winy >= 0) && (winy <= wattr.height))
                {
                  if(winx >= (wattr.width  - GRPLINUXSCREENX11_CUSTOMRESIZE_BORDER)) edge |= GRPLINUXSCREENX11_CUSTOMRESIZE_EDGE_RIGHT;
                  if(winy >= (wattr.height - GRPLINUXSCREENX11_CUSTOMRESIZE_BORDER)) edge |= GRPLINUXSCREENX11_CUSTOMRESIZE_EDGE_BOTTOM;
                }

              if(edge)
                {
                  customresizing          = true;
                  customresizeedge        = edge;
                  customresizestartrootx  = rootx;
                  customresizestartrooty  = rooty;
                  customresizestartwidth  = wattr.width;
                  customresizestartheight = wattr.height;
                }
            }
        }
    }
   else
    {
      if(!pressed)
        {
          customresizing = false;
        }
       else
        {
          int neww = customresizestartwidth;
          int newh = customresizestartheight;

          if(customresizeedge & GRPLINUXSCREENX11_CUSTOMRESIZE_EDGE_RIGHT)  neww = customresizestartwidth  + (rootx - customresizestartrootx);
          if(customresizeedge & GRPLINUXSCREENX11_CUSTOMRESIZE_EDGE_BOTTOM) newh = customresizestartheight + (rooty - customresizestartrooty);

          float maxw = 0.0f;
          float maxh = 0.0f;

          ResolveViewportMax(maxw, maxh);

          if((maxw > 0.0f) && (neww > (int)maxw)) neww = (int)maxw;
          if((maxh > 0.0f) && (newh > (int)maxh)) newh = (int)maxh;

          if(neww < GRPLINUXSCREENX11_CUSTOMRESIZE_MINSIZE) neww = GRPLINUXSCREENX11_CUSTOMRESIZE_MINSIZE;
          if(newh < GRPLINUXSCREENX11_CUSTOMRESIZE_MINSIZE) newh = GRPLINUXSCREENX11_CUSTOMRESIZE_MINSIZE;

          XWindowAttributes wattr;

          if(XGetWindowAttributes(display, window, &wattr))
            {
              if((neww != wattr.width) || (newh != wattr.height))
                {
                  XResizeWindow(display, window, neww, newh);
                  XFlush(display);
                }
            }
        }
    }

  customresizeprevpressed = pressed;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENX11::Create_Window(bool show)
* @brief      Create window
* @ingroup    PLATFORM_LINUX
*
* @param[in]  show : Show value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Create_Window(bool show)
{
  typedef struct  
  {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long          inputMode;
    unsigned long status;

  } MOTIFWMHINTS;

  if(!display) 
    {
      return false;
    }

  GRPRECTINT*           alldesktoprect  = NULL;
  int                   screenselected  = GetDesktopScreenSelected();   
  XSetWindowAttributes  attr;  
  int                   posx            = 0;
  int                   posy            = 0;
  int                   status          = false;   


  root = DefaultRootWindow(display);    

  if(screenselected == GRPSCREENTYPE_DESKTOP_ALL)
    {
      alldesktoprect = GetDesktopManager()->GetDesktopMonitors()->GetCombinedRect(); 
    }
   else 
    {
      alldesktoprect = GetDesktopManager()->GetDesktopMonitors()->GetMonitorsRects()->Get(screenselected);       
    }
  
  if(!alldesktoprect)
    {
      return false;          
    }            
   
   int winw = (int)GetPresentationWidth();
   int winh = (int)GetPresentationHeight();

   if(Styles_IsFullScreen())          
    {
      posx   = alldesktoprect->x1;
      posy   = alldesktoprect->y1; 

      if(!alldesktoprect->x1)
        {
          posx++;              
        }

      if(!alldesktoprect->y1)
        {
          posy++;              
        }

      if(Style_Is(GRPSCREENSTYLE_FULLSCREEN_ADJUSTRESOLUTION))
        {          
          ChangeScreenResolution(width, height);          
        }
       else 
        {
          width  = alldesktoprect->GetWidth();    
          height = alldesktoprect->GetHeight(); 

          if(Style_Is(GRPSCREENSTYLE_HEIGHTWITHOUTTASKBAR))
            {
              height -= GetTaskBarHeight();
            }
        }                      

      winw = (int)width;
      winh = (int)height;
    }
   else
    {         
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

      posx  += alldesktoprect->x1;
      posy  += alldesktoprect->y1;       
    } 
  
  
  #ifndef GRP_OPENGL_ACTIVE

  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

  #else

  bool opengl_visual_ok = false;

  if(IsRunningOnWSL())
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] WSL detected; using X11 software path (no GPU / EGL->X11 SHM unsupported)"));
    }
   else
    {
      if(!blitgles)
        {
          blitgles = GEN_NEW GRPLINUXBLITGLESX11();
        }

      if(blitgles)
        {
          EGLint visid = 0;
          if(blitgles->ChooseVisualID((EGLNativeDisplayType)display, visid))
            {
              XVisualInfo vtempl;
              memset(&vtempl, 0, sizeof(vtempl));
              vtempl.visualid = (VisualID)visid;
              int nvis = 0;
              XVisualInfo* vis = XGetVisualInfo(display, VisualIDMask, &vtempl, &nvis);
              if(vis && nvis > 0)
                {
                  vinfo = vis[0];
                  XFree(vis);
                  opengl_visual_ok = true;
                }
            }

          if(!opengl_visual_ok)
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] EGL visual failed; falling back to X11 software path"));
              GEN_DELETE blitgles;
              blitgles = NULL;
            }
        }
    }

  if(!opengl_visual_ok)
    {
      XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
    }

  #endif

  attr.colormap         = XCreateColormap(display, root, vinfo.visual, AllocNone);
  attr.border_pixel     = 0;

  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      attr.background_pixmap = None;
      window = XCreateWindow(display, root, posx, posy, winw, winh, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixmap, &attr);
    }
   else
  #endif
    {
      attr.background_pixel = Style_Is(GRPSCREENSTYLE_TRANSPARENT)?0x00000000:0xFFFFFFFF;
      window = XCreateWindow(display, root, posx, posy, winw, winh, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);        
    }
 
  if(!window)
    {
      return false;
    }

  if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
    {
      Atom comptonshadow = XInternAtom(display, "_COMPTON_SHADOW", False);  // create if absent so we can set it
      if(comptonshadow != None)
        {
          long shadowoff = 0;
          XChangeProperty(display, window, comptonshadow, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&shadowoff, 1);
          
          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] _COMPTON_SHADOW=0 set (no compositor shadow on transparent window)"));
        }
     
        XSetWindowAttributes swa;
        swa.override_redirect = True;
        XChangeWindowAttributes(display, window, CWOverrideRedirect, &swa);
        
        //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] override_redirect set on transparent window (no WM/WSLg frame shadow)"));
      
    }

  /*
  if(Style_Is(GRPSCREENSTYLE_FULLSCREEN))
    {
      Atom wm_state            = XInternAtom (display, "_NET_WM_STATE", true );
      Atom wm_state_fullscreen = XInternAtom (display, "_NET_WM_STATE_FULLSCREEN", true);  

      if(wm_state == None || wm_state_fullscreen == None)        
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom FULLSCREEN  (fullscreen)"));
        }
       else
        {           
          XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_fullscreen, 1);          
        }
    }
  */
 

  // NOTE: Fullscreen and Transparent styles are prioritary over any Chromes configuration (see below).
  bool usecfgchromesnative = (IsCFGChromesActive()                       &&
                               GetCFGChromes()->GetUseNativeChromes()    &&
                               !Style_Is(GRPSCREENSTYLE_TRANSPARENT));

  // Custom (non native) Chromes: GEN's own UI layout draws the whole caption (icon, title, min/max/close), so
  // the native window must show as little of its own decoration as possible -- no border/title/menu/min/max at
  // all; a resize grip is the only thing kept (and only if requested), the same way GRPWINDOWSSCREEN keeps
  // WS_THICKFRAME without WS_CAPTION for its own custom mode.
  bool usecfgchromescustom = IsUsingCustomChromes();

  // Cap this window's growth (interactive resize and native Maximize) at the GRPVIEWPORT_ID_MAIN
  // viewport's max size, before the window manager ever starts managing this window (i.e. before
  // Chromes_ApplyStyle()/the custom-chromes hints block/Show() below), so the hint is already in
  // place the first time it matters. Applies to custom chromes too: a size hint tells the WM what
  // sizes are ALLOWED, it never asks it to DRAW anything, so it cannot bring back native
  // decoration (the decorations field alone does that -- see the custom-chromes block below).
  // Only fullscreen is excluded (unrelated sizing, must not be clamped by the viewport max).
  if(!Styles_IsFullScreen())
    {
      ApplyWMNormalHints();
    }

  if(usecfgchromesnative)
    {
      Chromes_ApplyStyle();
      Chromes_ApplyPostCreate();
    }
   else if(usecfgchromescustom)
    {
      // only_if_exists = False ("create if absent so we can set it", exactly like the
      // _COMPTON_SHADOW lookup above): with True, this whole block -- the ONLY thing suppressing
      // the native titlebar in custom-chromes mode -- would be silently skipped on any session
      // where no client had interned this atom yet, which is indistinguishable, from the outside,
      // from the fix not working at all.
      Atom wmhintsatom = XInternAtom(display, "_MOTIF_WM_HINTS", False);

      if(wmhintsatom == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom _MOTIF_WM_HINTS (custom chromes)"));
        }
       else
        {
          HITNS hints;

          memset(&hints, 0, sizeof(hints));

          // decorations MUST be exactly 0 here -- NOT "0 plus the resize handles". This is the whole
          // reason the native titlebar used to appear stacked on top of GEN's custom-drawn caption:
          // _MOTIF_WM_HINTS' decorations field is not a reliable "give me only these pieces" request
          // across window managers. WSLg's compositor (and several others) implement it as a plain
          // boolean -- decorations == 0 means "undecorated", ANY non-zero mask means "decorate this
          // window", at which point they draw their OWN COMPLETE frame (border AND titlebar AND
          // buttons), ignoring which individual bits were actually asked for. So the previous
          // "MWM_DECOR_RESIZEH when GetResizeActive()" (a non-zero mask, and GetResizeActive()
          // defaults to true -- see GRPSCREENCFGCHROMES::Clean() -- so this was the normal case, not
          // an edge case) was read as "decorate me", producing exactly the doubled caption reported.
          // Giving up MWM_DECOR_RESIZEH means giving up the WM-drawn resize border too: GEN draws
          // and drives its own instead, see UpdateCustomChromesResize(). MWM_HINTS_FUNCTIONS is set
          // alongside MWM_HINTS_DECORATIONS (with functions populated to match) the same way
          // Chromes_ApplyStyle() does it -- functions describes what the WM should ALLOW, not what
          // it should DRAW, so unlike decorations it can safely keep RESIZE.
          hints.flags       = GRPLINUXSCREENX11_MWM_HINTS_DECORATIONS | GRPLINUXSCREENX11_MWM_HINTS_FUNCTIONS;
          hints.decorations = 0;
          hints.functions   = GRPLINUXSCREENX11_MWM_FUNC_MOVE;

          if(GetCFGChromes()->GetResizeActive())
            {
              hints.functions |= GRPLINUXSCREENX11_MWM_FUNC_RESIZE;
            }

          XChangeProperty(display, window, wmhintsatom, wmhintsatom, 32, PropModeReplace, (unsigned char*)&hints, 5);
        }

      // The window title property itself is still set even though no titlebar is drawn to show it -- Alt+Tab
      // switchers, taskbars and window-list docks read it regardless of whether any decoration is visible.
      if(!title.IsEmpty())
        {
          XBUFFER xbuffer;

          title.ConvertToASCII(xbuffer);

          XStoreName(display, window, (char*)xbuffer.Get());
        }
    }
   else if(!Style_Is(GRPSCREENSTYLE_TITLE))
    {
      Atom          wmhintsatom = XInternAtom(display , "_MOTIF_WM_HINTS", True);
      MOTIFWMHINTS  hints; 

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom HINTS Decorations (no title) active"));

      if(wmhintsatom  == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom HINTS Decorations (no title)"));
        }
       else
        {
          memset(&hints, 0, sizeof(hints));

          hints.flags        = GRPLINUXSCREENX11_MWM_HINTS_DECORATIONS;   
          hints.decorations  = 0;  

          XChangeProperty(display, window, wmhintsatom, wmhintsatom, 32, PropModeReplace, (unsigned char *)&hints, 5); 
        }
    }
   else
    {
      if(!title.IsEmpty())
        {
          XBUFFER xbuffer;

          title.ConvertToASCII(xbuffer);

          XStoreName(display, window, (char*)xbuffer.Get());
        }

      if(Style_Is(GRPSCREENSTYLE_NOWINDOWICONS))
        {
          Atom          wmhintsatom = XInternAtom(display , "_MOTIF_WM_HINTS", True);
          MOTIFWMHINTS  hints;

          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom HINTS Decorations (no icons) active"));

          if(wmhintsatom  == None)
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom HINTS Decorations (no icons)"));
            }
           else
            { 
              memset(&hints, 0, sizeof(hints));

              hints.flags       = GRPLINUXSCREENX11_MWM_HINTS_DECORATIONS;
              hints.decorations = GRPLINUXSCREENX11_MWM_FUNC_CLOSE; 

              XChangeProperty(display, window, wmhintsatom, wmhintsatom, 32, PropModeReplace, (unsigned char*)&hints, 5);
            }
        }
    } 

  if(Style_Is(GRPSCREENSTYLE_NOICONTASKBAR))
    {
      Atom wm_state             = XInternAtom(display, "_NET_WM_STATE", True);
      Atom wm_state_skiptaskbar = XInternAtom(display , "_NET_WM_STATE_SKIP_TASKBAR", True);

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom SKIP_TASKBAR active"));
        
      if(wm_state == None || wm_state_skiptaskbar == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom SKIP_TASKBAR (No Icon Taskbar)"));
        }
       else
        {
          XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_skiptaskbar, 1);          
        }
    }

  if(Style_Is(GRPSCREENSTYLE_ONTOP))
    {
      Atom wm_state       = XInternAtom(display, "_NET_WM_STATE", True);
      Atom wm_state_above = XInternAtom(display , "_NET_WM_STATE_ABOVE" , True);

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom ABOVE active"));

      if(wm_state == None || wm_state_above == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom ABOVE (On top)"));
        }
       else
        {
          XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_above, 1);              
        }
    }


  if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
    {       
      /*
      Atom wm_opacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", True);
      unsigned long opacity = (unsigned long)(0x01010101);

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom OPACITY active"));

      if(wm_opacity == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom OPACITY (Transparent)"));
        }
       else
        {
          XChangeProperty(display, window, wm_opacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&opacity, 1L);
        }
      */

      /*
      Atom           wm_opacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", True);      
      int            alpha = 0;
          
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] Atom OPACITY active"));

      if(wm_opacity == None)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom OPACITY (Transparent)"));
        }
       else
        {
          XChangeProperty (display, window, wm_opacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&alpha, 1);          
          XFlush(display);
        }
      */
    }
  
  
  wmdeletewindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
  if(wmdeletewindow != None)
    {
      XSetWMProtocols(display, window, &wmdeletewindow, 1);
    }

  // NOTE: XMapWindow()/XMoveWindow() used to happen HERE, before blitgles even existed -- see the
  // GRP_OPENGL_ACTIVE block below, right before the final "return true;", for why they were moved past it.

  #ifdef GRP_OPENGL_ACTIVE

  if(blitgles && !blitgles->Create(this))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] BlitGLES create failed; falling back to X11 software path"));
      GEN_DELETE blitgles;
      blitgles = NULL;
    }

  if(blitgles)
    {
      blitgles->SetFlipY(true);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] BlitGLES SetFlipY(true) — correcting Mesa EGL-to-X11 vertical flip"));

      // The EGL surface Create() just built has never been cleared or presented -- its initial content is
      // whatever the driver leaves in a freshly allocated buffer, which is undefined and on several drivers
      // visibly so (stray coloured pixels, not a clean colour). XMapWindow() below is what first makes this
      // window visible on the desktop; clearing and presenting a blank frame BEFORE that call, rather than
      // after, means the X server/compositor never has a chance to grab that undefined content -- twice, so
      // neither buffer of the common double-buffered swap chain is left holding it.
      blitgles->PresentBlankFrame();
      blitgles->PresentBlankFrame();
    }

  #endif

  // Moved past blitgles setup above (see the NOTE further up): on a GRP_OPENGL_ACTIVE build the window
  // must not become visible on the desktop until the EGL surface behind it has at least one clean,
  // known-colour frame presented (see PresentBlankFrame() just above) -- otherwise whatever the X
  // server/compositor paints before GEN's own first real Update(canvas) call shows the surface's undefined
  // initial content instead.
  XMapWindow(display , window);
  XMoveWindow(display, window, posx, posy);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::ChangeScreenResolution(int width, int height)
* @brief      Change screen resolution
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::ChangeScreenResolution(int width, int height)
{   
  int                     num_sizes;
  XRRScreenSize*          xrrs;
  XRRScreenConfiguration* conf;
  short                   original_rate;
  Rotation                original_rotation;
  SizeID                  original_size_id;
 
  root  = RootWindow(display, 0);
  xrrs  = XRRSizes(display, 0, &num_sizes);
 
  for(int c=0; c<num_sizes; c++) 
    {
      if((xrrs[c].width  == width) && (xrrs[c].height == height))
        {
          short*  rates;
          int     num_rates;

          rates = XRRRates(display, 0, c, &num_rates);
                      
          conf             = XRRGetScreenInfo(display, root);
          original_rate    = XRRConfigCurrentRate(conf);
          original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
              
          int status = XRRSetScreenConfigAndRate(display, conf, root, c, original_rotation, rates[0], CurrentTime);

          return true;        
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XImage* GRPLINUXSCREENX11::CreateXImageFromBuffer(Display* display, int screen, XBYTE* buffer, int width, int height)
* @brief      Create X image from buffer
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  display : Display pointer to use.
* @param[in]  screen : Screen object to use.
* @param[in]  buffer : Buffer to use.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     XImage* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XImage* GRPLINUXSCREENX11::CreateXImageFromBuffer(Display* display, int screen, XBYTE* buffer, int width, int height)
{
  XImage* image = NULL; 
  double  rratio;
  double  gratio;
  double  bratio;
  int     outindex       = 0;
  int     numbufferbytes = (4 * (width * height));

  rratio = vinfo.visual->red_mask   / 255.0;
  gratio = vinfo.visual->green_mask / 255.0;
  bratio = vinfo.visual->blue_mask  / 255.0;

  if(vinfo.depth >= 24)
    {
      int    numnewbufferbytes = (4 * (width * height));
      XDWORD* newbuffer        = (XDWORD*)malloc(numnewbufferbytes);

      for(int index=0;index<numbufferbytes;)
        {
          XDWORD b = (buffer[index++] * bratio);
          XDWORD g = (buffer[index++] * gratio);
          XDWORD r = (buffer[index++] * rratio);         
          XDWORD a;

          if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
            {
              a = buffer[index++] << 24;         
            }
           else
            {
              a = 0xFF000000;
              index++;
            } 

          r &= vinfo.visual->red_mask;
          g &= vinfo.visual->green_mask;
          b &= vinfo.visual->blue_mask;            

          newbuffer[outindex] = r | g | b | a;

          ++outindex;
        }

      image = XCreateImage (display, vinfo.visual, vinfo.depth, ZPixmap, 0, (char *)newbuffer, width, height, 32, 0);
    }
    else
    {
      if(vinfo.depth >= 15)
        {
          int   numnewbufferbytes = (2 * (width * height));
          XWORD* newbuffer          = (XWORD*)malloc(numnewbufferbytes);

          for(int index=0;index<numbufferbytes;)
            {
              XDWORD b = (buffer[index++] * bratio);
              XDWORD g = (buffer[index++] * gratio);
              XDWORD r = (buffer[index++] * rratio);

              r &= vinfo.visual->red_mask;
              g &= vinfo.visual->green_mask;
              b &= vinfo.visual->blue_mask;

              newbuffer[outindex] = r | g | b;

              ++outindex;
            }

          image = XCreateImage(display, vinfo.visual, vinfo.depth, ZPixmap, 0, (char*)newbuffer, width, height, 16, 0);

        }  
       else 
        {
          return NULL;
        }
    }

  XInitImage(image);

  if(LSBFirst == GetByteOrder())
         image->byte_order = LSBFirst;
    else image->byte_order = MSBFirst;

  image->bitmap_bit_order = MSBFirst;

  return image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRPLINUXSCREENX11::ScaleBufferNearestLetterbox(XBYTE* src, int srcw, int srch, int dstw, int dsth)
* @brief      Nearest-neighbor scale of a 4-bytes-per-pixel buffer to a new size, letterboxed
*             (black bars) to preserve the source aspect ratio. Used by the X11 software blit
*             path (no GPU) so a resize/maximize scales the image the same way the GL path does,
*             instead of pasting it 1:1 in a corner.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  src : Source buffer, srcw x srch, 4 bytes per pixel. Channel order does not matter,
*                   pixels are copied whole.
* @param[in]  srcw : Source width.
* @param[in]  srch : Source height.
* @param[in]  dstw : Destination width.
* @param[in]  dsth : Destination height.
* 
* @return     XBYTE* : Newly malloc'd dstw x dsth x 4 buffer (caller must free()); NULL on error.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPLINUXSCREENX11::ScaleBufferNearestLetterbox(XBYTE* src, int srcw, int srch, int dstw, int dsth)
{
  if(!src || (srcw <= 0) || (srch <= 0) || (dstw <= 0) || (dsth <= 0))
    {
      return NULL;
    }

  size_t dstbytes = (size_t)dstw * (size_t)dsth * 4;

  XBYTE* dst = (XBYTE*)malloc(dstbytes);
  if(!dst)
    {
      return NULL;
    }

  memset(dst, 0, dstbytes);   // black letterbox bars

  float scalex = (float)dstw / (float)srcw;
  float scaley = (float)dsth / (float)srch;
  float scale  = (scalex < scaley) ? scalex : scaley;

  int draww = (int)((float)srcw * scale);
  int drawh = (int)((float)srch * scale);
  if(draww < 1) draww = 1;
  if(drawh < 1) drawh = 1;

  int offsetx = (dstw - draww) / 2;
  int offsety = (dsth - drawh) / 2;

  for(int y=0; y<drawh; y++)
    {
      int srcy = (int)((float)y / scale);
      if(srcy >= srch) srcy = srch - 1;

      XBYTE* dstrow    = dst + ((size_t)(y + offsety) * (size_t)dstw + (size_t)offsetx) * 4;
      XBYTE* srcrowbase = src + (size_t)srcy * (size_t)srcw * 4;

      for(int x=0; x<draww; x++)
        {
          int srcx = (int)((float)x / scale);
          if(srcx >= srcw) srcx = srcw - 1;

          memcpy(dstrow + (size_t)x * 4, srcrowbase + (size_t)srcx * 4, 4);
        }
    }

  return dst;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPLINUXSCREENX11::GetTaskBarHeight()
* @brief      Get task bar height
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENX11::GetTaskBarHeight()
{
  int screen              = DefaultScreen(display);    
  int screen_width        = DisplayWidth(display, screen);
  int screen_height       = DisplayHeight(display, screen);

    
  Atom atomworkarea = XInternAtom(display, "_NET_WORKAREA", True);
  if(atomworkarea == None) 
    {
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error Atom WORKAREA (Get Taskbar height)"));      
      return 0;
    }

  Atom           actual_type;
  int            actual_format;
  unsigned long  nitems;
  unsigned long  bytes_after;
  unsigned char* data     = nullptr;

  int status = XGetWindowProperty(display, root, atomworkarea, 0, 4 * sizeof(long), False, XA_CARDINAL, &actual_type, &actual_format, &nitems, &bytes_after, &data);
  if(status != Success || !data) 
    {
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error XGetWindowProperty (Get Taskbar height)"));     
      return 0;
    }
    
  long*   workarea        = reinterpret_cast<long*>(data);
  int     work_width      = workarea[2];
  int     work_height     = workarea[3];
  int     taskbar_height  = screen_height - work_height;

  XFree(data);  

  return taskbar_height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENX11::ShowDebugNetSupportedPropertys()
* @brief      Show debug net supported propertys
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENX11::ShowDebugNetSupportedPropertys()
{
  Atom netsupported = XInternAtom(display, "_NET_SUPPORTED", True);
  if(netsupported == None) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Error NET_SUPPORTED"));          
      return false;
    }

  Atom            type;
  int             format;
  unsigned long   nitems;
  unsigned long   bytesafter;
  Atom*           atoms = nullptr;

  int status = XGetWindowProperty(display, root, netsupported, 0, 1024, False, XA_ATOM, &type, &format, &nitems, &bytesafter, reinterpret_cast<unsigned char **>(&atoms));
  if(status != Success || type != XA_ATOM || format != 32) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen X11] Failed to retrieve _NET_SUPPORTED property"));   

      if(atoms) 
        {
          XFree(atoms);
        }
        
      return 1;
    }


  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen X11] _NET_SUPPORTED Atoms [%d]: "), nitems);

  for(unsigned long c=0; c<nitems; ++c) 
    {
      char* atomname = XGetAtomName(display, atoms[c]);
      if(atomname) 
        {
          XSTRING _atomname;

          _atomname = atomname;

          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("%s"), _atomname.Get());
          
          XFree(atomname);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPLINUXSCREENX11::GetByteOrder()
* @brief      Get byte order
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENX11::GetByteOrder()
{
  union
  {
    char c[sizeof(short)];
    short s;

  } order;

  order.s = 1;
  if((1 == order.c[0]))
    {
      return LSBFirst;
    }
   else
    {
      return MSBFirst;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPLINUXSCREENX11::ErrorHandler(Display* display, XErrorEvent* errorevent)
* @brief      Error handler
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  display : Display pointer to use.
* @param[in]  errorevent : Errorevent pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENX11::ErrorHandler(Display* display, XErrorEvent* errorevent)
{
  int a=0;
  a++;

  return 0; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXSCREENX11::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENX11::Clean()
{
  display        = NULL;
  movedisplay    = NULL;
  window         = 0;

  wmdeletewindow = None;

  isdesktop      = false;

  normalhintsapplied      = false;

  customresizing          = false;
  customresizeedge        = 0;
  customresizeprevpressed = false;
  customresizestartrootx  = 0;
  customresizestartrooty  = 0;
  customresizestartwidth  = 0;
  customresizestartheight = 0;

  #ifdef GRP_OPENGL_ACTIVE
  blitgles  = NULL;
  #endif
}



#endif

