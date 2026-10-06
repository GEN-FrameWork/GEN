/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXScreenWayland.cpp
*
* @class      GRPLINUXSCREENWAYLAND
* @brief      LINUX Graphics Wayland Screen class
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


#ifdef LINUX_WAYLAND_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <xkbcommon/xkbcommon-keysyms.h>

#include "GRPLINUXScreenWayland.h"

#ifdef GRP_OPENGL_ACTIVE
#include "GRPLINUXBlitGLESWayland.h"
#endif

#include "GRP2DCanvas.h"
#include "GRPViewPort.h"

#include "XBuffer.h"
#include "XString.h"
#include "XTrace.h"

#include "MainProcLINUX.h"
#include "APPFlowGraphics.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// wl_registry / xdg_wm_base / xdg_surface / xdg_toplevel / wl_seat / wl_pointer / wl_keyboard
// listener vtables. Wayland delivers every event through one of these function-pointer tables
// instead of a queue you dequeue by hand (XCheckWindowEvent's model) -- registering them is the
// direct equivalent of X11's XSelectInput(), just resolved through a jump table instead of an
// event-type bitmask.

static const struct wl_registry_listener    grplinuxscreenwayland_registry_listener    = { GRPLINUXSCREENWAYLAND::Registry_Global, GRPLINUXSCREENWAYLAND::Registry_GlobalRemove };
static const struct xdg_wm_base_listener    grplinuxscreenwayland_xdgwmbase_listener   = { GRPLINUXSCREENWAYLAND::XDGWMBase_Ping };
static const struct xdg_surface_listener    grplinuxscreenwayland_xdgsurface_listener  = { GRPLINUXSCREENWAYLAND::XDGSurface_Configure };
static const struct xdg_toplevel_listener   grplinuxscreenwayland_xdgtoplevel_listener = { GRPLINUXSCREENWAYLAND::XDGToplevel_Configure, GRPLINUXSCREENWAYLAND::XDGToplevel_Close };
static const struct wl_seat_listener        grplinuxscreenwayland_seat_listener        = { GRPLINUXSCREENWAYLAND::Seat_Capabilities, GRPLINUXSCREENWAYLAND::Seat_Name };

// wl_pointer/wl_keyboard listeners ARE installed here, by this class, even though the events
// they carry (motion, button, key...) are logically INPLINUXDEVICEMOUSEWAYLAND/
// INPLINUXDEVICEKEYBOARDWAYLAND's concern. Reason: a wl_proxy accepts exactly ONE listener --
// calling wl_pointer_add_listener() a second time on the same object would silently replace
// whatever GRPLINUXSCREENWAYLAND itself needs (enter/leave, for HasFocus() and the serial
// ShowCursor() needs). Since this class already owns the wl_pointer/wl_keyboard objects (bound
// from wl_seat capabilities, in Seat_Capabilities() below), it is also the only place that CAN
// install the listener, full stop -- so it does the whole job, and the input device classes
// drain the small FIFOs it fills instead of listening themselves. See PushButtonEvent()/
// PushKeyEvent() and the GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE comment in the header for the
// full rationale.
static const struct wl_pointer_listener     grplinuxscreenwayland_pointer_listener      = { GRPLINUXSCREENWAYLAND::Pointer_Enter, GRPLINUXSCREENWAYLAND::Pointer_Leave, GRPLINUXSCREENWAYLAND::Pointer_Motion, GRPLINUXSCREENWAYLAND::Pointer_Button, GRPLINUXSCREENWAYLAND::Pointer_Axis, GRPLINUXSCREENWAYLAND::Pointer_Frame, GRPLINUXSCREENWAYLAND::Pointer_AxisSource, GRPLINUXSCREENWAYLAND::Pointer_AxisStop, GRPLINUXSCREENWAYLAND::Pointer_AxisDiscrete };
static const struct wl_keyboard_listener    grplinuxscreenwayland_keyboard_listener     = { GRPLINUXSCREENWAYLAND::Keyboard_Keymap, GRPLINUXSCREENWAYLAND::Keyboard_Enter, GRPLINUXSCREENWAYLAND::Keyboard_Leave, GRPLINUXSCREENWAYLAND::Keyboard_Key, GRPLINUXSCREENWAYLAND::Keyboard_Modifiers, GRPLINUXSCREENWAYLAND::Keyboard_RepeatInfo };

// wl_buffer_listener for the one-shot SHM buffers CreateSHMBuffer() creates (software fallback
// path only, see CreateSHMBuffer() below) -- unlike the listeners above (installed once, for the
// lifetime of the screen), a fresh one of these is installed per buffer, since each buffer is
// itself a fresh one-shot object.
static const struct wl_buffer_listener      grplinuxscreenwayland_buffer_listener       = { GRPLINUXSCREENWAYLAND::Buffer_Release };

#ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
static const struct zxdg_toplevel_decoration_v1_listener grplinuxscreenwayland_decoration_listener = { GRPLINUXSCREENWAYLAND::Decoration_Configure };
#endif


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPLINUXSCREENWAYLAND::GRPLINUXSCREENWAYLAND(): GRPSCREEN()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENWAYLAND::GRPLINUXSCREENWAYLAND(): GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_LINUX_WAYLAND;

  // Created once here, not in Clean(): Clean() also runs at the END of the destructor (after
  // xkbcontext has already been unref'd below), and if it unconditionally called
  // xkb_context_new() again there, that fresh context would be immediately orphaned -- leaked on
  // every single screen destruction, since nothing would ever unref it afterwards.
  xkbcontext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

  if(!ConnectDisplay())
    {
      return;
    }

  if(!BindGlobals())
    {
      return;
    }

  // Unlike X11's DisplayWidth()/DisplayHeight() (synchronous, answered from data the server
  // already sent at connection time), Wayland has no "current desktop size" query at all -- the
  // real size for THIS window only arrives, asynchronously, as the first xdg_toplevel::configure
  // event after the surface is mapped (handled in XDGToplevel_Configure / Create_Surface). Until
  // then we seed a reasonable default, exactly like GRPLINUXSCREENX11 seeds from the X11 default
  // screen before Create_Window() runs.
  SetSize(GRPLINUXSCREENWAYLAND_DEFAULT_WIDTH, GRPLINUXSCREENWAYLAND_DEFAULT_HEIGHT);
  SetMaxSize(GRPLINUXSCREENWAYLAND_DEFAULT_WIDTH, GRPLINUXSCREENWAYLAND_DEFAULT_HEIGHT);

  // Live window size kept in sync with the canvas design size only here, at construction time --
  // see the windowwidth/windowheight member comment in the header for why the two must never be
  // conflated again afterwards (Resize()/XDGToplevel_Configure() update windowwidth/windowheight
  // only, never SetSize()).
  windowwidth  = GRPLINUXSCREENWAYLAND_DEFAULT_WIDTH;
  windowheight = GRPLINUXSCREENWAYLAND_DEFAULT_HEIGHT;

  SetMode(GRPPROPERTYMODE_32_BGRA_8888);

  SetIsBufferInverse(true);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPLINUXSCREENWAYLAND::~GRPLINUXSCREENWAYLAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENWAYLAND::~GRPLINUXSCREENWAYLAND()
{
  if(!display)
    {
      return;
    }

  Delete();

  if(xkbstate)      { xkb_state_unref(xkbstate);        xkbstate      = NULL; }
  if(xkbkeymap)     { xkb_keymap_unref(xkbkeymap);       xkbkeymap     = NULL; }
  if(xkbcontext)    { xkb_context_unref(xkbcontext);     xkbcontext    = NULL; }

  if(pointer)       { wl_pointer_destroy(pointer);       pointer       = NULL; }
  if(keyboard)      { wl_keyboard_destroy(keyboard);     keyboard      = NULL; }
  if(seat)          { wl_seat_destroy(seat);             seat          = NULL; }
  if(shm)           { wl_shm_destroy(shm);               shm           = NULL; }
  if(xdgwmbase)     { xdg_wm_base_destroy(xdgwmbase);    xdgwmbase     = NULL; }

  #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
  if(decorationmanager) { zxdg_decoration_manager_v1_destroy(decorationmanager); decorationmanager = NULL; }
  #endif

  if(compositor)    { wl_compositor_destroy(compositor); compositor    = NULL; }
  if(registry)      { wl_registry_destroy(registry);     registry      = NULL; }

  // As with GRPLINUXSCREENX11 (which never closes 'display', see the commented-out
  // XCloseDisplay() in its destructor), the connection itself is intentionally left open here
  // too, for the same reason: parity of behaviour between backends is more valuable than
  // "correctness" the original code never actually had either. A future cleanup pass should
  // revisit both together.
  //wl_display_disconnect(display);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::ConnectDisplay()
* @brief      Open the connection to the Wayland compositor (equivalent of X11's XOpenDisplay(NULL))
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::ConnectDisplay()
{
  display = wl_display_connect(NULL);
  if(!display)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen Wayland] wl_display_connect failed (no compositor / WAYLAND_DISPLAY not set)"));
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::BindGlobals()
* @brief      Bind wl_compositor/wl_shm/xdg_wm_base/wl_seat from the registry. Equivalent of the
*             implicit globals Xlib already gives you for free after XOpenDisplay() (root window,
*             default visual, etc.) -- Wayland requires an explicit registry round-trip first.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::BindGlobals()
{
  registry = wl_display_get_registry(display);
  if(!registry)
    {
      return false;
    }

  wl_registry_add_listener(registry, &grplinuxscreenwayland_registry_listener, this);

  // Blocking round-trip: sends the get_registry request and waits until the server has replied
  // to it, which guarantees every wl_registry::global event for already-existing objects (the
  // compositor, shm, xdg_wm_base, seat, outputs...) has been dispatched before we continue. GEN
  // calls this ONCE, here, in the constructor -- every later per-frame pump goes through the
  // non-blocking DispatchEvents() instead, same "blocking only at setup" shape as XOpenDisplay().
  wl_display_roundtrip(display);

  if(!compositor || !xdgwmbase)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Screen Wayland] compositor and/or xdg_wm_base global not advertised by the compositor"));
      return false;
    }

  xdg_wm_base_add_listener(xdgwmbase, &grplinuxscreenwayland_xdgwmbase_listener, this);

  if(seat)
    {
      wl_seat_add_listener(seat, &grplinuxscreenwayland_seat_listener, this);
    }

  // A second round-trip lets wl_seat announce its capabilities (Seat_Capabilities, which binds
  // wl_pointer/wl_keyboard and installs their listeners) and any wl_output announce its
  // geometry/mode, so both are already known before Create() runs.
  wl_display_roundtrip(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::ResolveViewportMax(float& maxw, float& maxh)
* @brief      Resolves the GRPVIEWPORT_ID_MAIN viewport's max size, shared by ClampToViewportMax()
*             and ApplyMaxSizeHint() below: an EXPLICIT GRPVIEWPORT::SetMaxSize() on an axis wins;
*             otherwise that axis falls back to the viewport's own declared size
*             (GetWidth()/GetHeight() -- the design/content resolution), same fallback
*             GRPLINUXSCREENX11::ApplyWMNormalHints() and GRPWINDOWSSCREEN::ApplyResizeLimits() use,
*             so the window is capped even when the app never calls the new SetMaxSize() API
*             explicitly.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[out] maxw : Resolved max width. Left UNTOUCHED if no viewport is configured yet.
* @param[out] maxh : Resolved max height. Left UNTOUCHED if no viewport is configured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::ResolveViewportMax(float& maxw, float& maxh)
{
  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return; // no viewport configured yet: caller keeps whatever it had
    }

  maxw = mainviewport->GetMaxWidth();
  maxh = mainviewport->GetMaxHeight();

  if(maxw <= 0.0f) maxw = mainviewport->GetWidth();
  if(maxh <= 0.0f) maxh = mainviewport->GetHeight();

  // Final safety net: if one axis is still unresolved (e.g. a viewport whose width/height were
  // never both set) while the other DOES have a real value, never hand xdg_toplevel_set_max_size()/
  // a caller a literal 0 on just that one axis -- fall back to this screen's own GRPPROPERTIES max
  // (set in the constructor to the display's own resolution, see GRPLINUXSCREENWAYLAND::Clean()/
  // constructor), so both axes always end up with a real, non-zero ceiling together.
  if(maxw <= 0.0f) maxw = (float)GetMaxWidth();
  if(maxh <= 0.0f) maxh = (float)GetMaxHeight();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::ClampToViewportMax(int& w, int& h)
* @brief      In/out clamp of a requested (w,h) pair down to the GRPVIEWPORT_ID_MAIN viewport's
*             resolved max (see ResolveViewportMax() above), per axis independently -- the Wayland
*             analogue of GRPWINDOWSSCREEN::ApplyResizeLimits() clamping ptMaxTrackSize, applied
*             in-process here since xdg-shell gives clients no equivalent OS-level drag-limit hook
*             (see ApplyMaxSizeHint() below for the proactive xdg_toplevel_set_max_size() request
*             that covers the compositor side of the same cap). Called from both Resize() (locally
*             requested size) and XDGToplevel_Configure() (compositor-confirmed size).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in,out] w : Requested width in; clamped width out. Untouched if no cap applies on this axis.
* @param[in,out] h : Requested height in; clamped height out. Untouched if no cap applies on this axis.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::ClampToViewportMax(int& w, int& h)
{
  float maxw = 0.0f;
  float maxh = 0.0f;

  ResolveViewportMax(maxw, maxh);

  if(maxw > 0.0f && w > (int)maxw) w = (int)maxw;
  if(maxh > 0.0f && h > (int)maxh) h = (int)maxh;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::ApplyMaxSizeHint()
* @brief      Proactive Wayland analogue of GRPLINUXSCREENX11::ApplyWMNormalHints()/
*             GRPWINDOWSSCREEN::ApplyResizeLimits(): resolves the GRPVIEWPORT_ID_MAIN viewport's
*             max size (see ResolveViewportMax() above) and, if xdgtoplevel already exists, issues
*             the standard xdg_toplevel_set_max_size() request so the compositor's own interactive
*             resize grab AND its own maximize logic both respect it. Unlike X11's
*             XSetWMNormalHints (a property the WM re-reads whenever it needs to), this is a live
*             protocol request that must be reissued -- called once from Create_Surface() (in case
*             the app already created its viewport before Create()) and again on every
*             XDGToplevel_Configure() (in case it created it only afterwards; both call sites are
*             cheap/idempotent, so this redundancy just makes the hint self-heal regardless of
*             ordering).
* @note       Deliberately does NOT call xdg_toplevel_set_min_size() (no OS-enforced minimum, same
*             policy as X11/Windows) -- see the Update() raster-path guard and
*             GRPLINUXBLITGLESWAYLAND::ComputePresentationScale (GL path) for how content below the
*             viewport's min is hidden instead.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::ApplyMaxSizeHint()
{
  if(!xdgtoplevel)
    {
      return;
    }

  float maxw = 0.0f;
  float maxh = 0.0f;

  ResolveViewportMax(maxw, maxh);

  if(maxw <= 0.0f && maxh <= 0.0f)
    {
      return; // no viewport configured yet, or it has no usable size at all
    }

  xdg_toplevel_set_max_size(xdgtoplevel, (int)maxw, (int)maxh);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Create(bool show)
* @brief      Create
* @ingroup    PLATFORM_LINUX
*
* @param[in]  show : Show value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Create(bool show)
{
  if(!Create_Surface(show))
    {
      return false;
    }

  wl_display_flush(display);

  return GRPSCREEN::Create(show);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Create_Surface(bool show)
* @brief      Create the wl_surface + xdg_surface + xdg_toplevel chain and, if OpenGL is active,
*             the wl_egl_window and EGL context on top of it. The Wayland analogue of
*             GRPLINUXSCREENX11::Create_Window(), with one structural difference worth calling
*             out: X11 needs the EGL-compatible XVisualInfo BEFORE XCreateWindow() (see
*             GRPLINUXBlitGLESX11's sibling comment in the X11 file), because the visual is baked
*             into the X11 window at creation time. Wayland has no such constraint -- any
*             wl_surface can host any EGL config -- so blitgles->Create() simply runs AFTER the
*             surface exists, which is a simpler, more linear sequence than the X11 one.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  show : Show value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Create_Surface(bool show)
{
  if(!display || !compositor || !xdgwmbase)
    {
      return false;
    }

  surface = wl_compositor_create_surface(compositor);
  if(!surface)
    {
      return false;
    }

  xdgsurface = xdg_wm_base_get_xdg_surface(xdgwmbase, surface);
  if(!xdgsurface)
    {
      return false;
    }

  xdg_surface_add_listener(xdgsurface, &grplinuxscreenwayland_xdgsurface_listener, this);

  xdgtoplevel = xdg_surface_get_toplevel(xdgsurface);
  if(!xdgtoplevel)
    {
      return false;
    }

  xdg_toplevel_add_listener(xdgtoplevel, &grplinuxscreenwayland_xdgtoplevel_listener, this);

  // Cap this toplevel's growth (interactive resize grab and the compositor's own maximize logic)
  // at the GRPVIEWPORT_ID_MAIN viewport's max size, in case the app already created that viewport
  // before calling Create() -- ApplyMaxSizeHint() is called again on every XDGToplevel_Configure()
  // below, so it self-heals even if the viewport is only created afterwards.
  ApplyMaxSizeHint();

  if(!title.IsEmpty())
    {
      XBUFFER titleutf8;
      if(title.ConvertToUTF8(titleutf8))
        {
          xdg_toplevel_set_title(xdgtoplevel, (char*)titleutf8.Get());
        }
    }

  #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
  // Ask the compositor for native (server-side) decorations, mirroring GRPLINUXSCREENX11's
  // _MOTIF_WM_HINTS caption toggle at the granularity this protocol actually offers: a single
  // client-side/server-side switch, driven off the same GetNativeCaptionActive() flag that turns
  // the WHOLE X11 decoration set on/off. There is no Wayland equivalent of the X11 path's
  // per-piece control (minimize/maximize/close/icon each toggled independently via MWM hints,
  // see Chromes_ApplyStyle()) -- once server-side mode is granted, which buttons the compositor
  // actually draws is entirely its own theme's decision. decorationmanager is NULL whenever the
  // compositor never advertised zxdg_decoration_manager_v1 (an optional extension -- GNOME/Mutter
  // notably never implements it), in which case this whole block is skipped and the window simply
  // gets no decoration at all unless the application draws its own (GEN's "Custom Chromes" mode).
  if(decorationmanager)
    {
      toplleveldecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(decorationmanager, xdgtoplevel);
      if(toplleveldecoration)
        {
          zxdg_toplevel_decoration_v1_add_listener(toplleveldecoration, &grplinuxscreenwayland_decoration_listener, this);

          GRPSCREENCFGCHROMES* cfgchromes = GetCFGChromes();

          XDWORD requestedmode = (cfgchromes && cfgchromes->GetNativeCaptionActive())
                                    ? ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE
                                    : ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE;

          zxdg_toplevel_decoration_v1_set_mode(toplleveldecoration, requestedmode);
        }
    }
  #endif

  // No equivalent of X11's _NET_WM_STATE_SKIP_TASKBAR/_NET_WM_STATE_ABOVE property dance
  // (Chromes_ApplyStyle() in the X11 file): xdg-shell/xdg-decoration expose native caption
  // on/off (handled just above, when available) but nothing for "no taskbar icon" or
  // "borderless/always-on-top" as a client request -- those remain compositor policy. The
  // one style xdg-shell DOES support directly is fullscreen:
  if(Style_Is(GRPSCREENSTYLE_FULLSCREEN) || Style_Is(GRPSCREENSTYLE_FULLSCREEN_ADJUSTRESOLUTION))
    {
      struct wl_output* targetoutput = NULL;

      int screenselected = GetDesktopScreenSelected();
      if((screenselected != GRPSCREENTYPE_DESKTOP_ALL) && (screenselected >= 0) && (screenselected < (int)outputs.GetSize()))
        {
          targetoutput = outputs.Get(screenselected);
        }

      xdg_toplevel_set_fullscreen(xdgtoplevel, targetoutput);
    }

  // First commit with no buffer attached: this is required by xdg-shell before the compositor
  // will send the initial xdg_surface::configure. There is nothing equivalent to "commit an
  // empty surface" in X11 -- XCreateWindow()+XMapWindow() already gives you a paintable
  // (if blank) window synchronously.
  wl_surface_commit(surface);

  wl_display_roundtrip(display);   // waits for the initial configure + our ack (XDGSurface_Configure)

  #ifdef GRP_OPENGL_ACTIVE

  if(!blitgles)
    {
      blitgles = GEN_NEW GRPLINUXBLITGLESWAYLAND();
    }

  if(blitgles)
    {
      if(!blitgles->Create(this))
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen Wayland] BlitGLES create failed; falling back to Wayland SHM software path"));
          GEN_DELETE blitgles;
          blitgles = NULL;
        }
       else
        {
          // Same Mesa EGL vertical-flip correction GRPLINUXSCREENX11 applies for GLX -- the
          // underlying GL/EGL semantics are identical regardless of the windowing system.
          blitgles->SetFlipY(true);
        }
    }

  #endif

  if(show)
    {
      Show(true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::DispatchEvents()
* @brief      Non-blocking pump of the wl_display connection: flush pending outgoing requests,
*             read any new incoming events without blocking if none are available, then dispatch
*             everything already queued (which synchronously invokes every listener callback in
*             this file). The single point every X11 XCheckWindowEvent()/XCheckTypedWindowEvent()
*             call in GRPLINUXSCREENX11/INPLINUXDEVICEKEYBOARDX11/INPLINUXDEVICEMOUSEX11 collapses
*             into on this backend, since Wayland has no per-window/per-mask selective dequeue.
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::DispatchEvents()
{
  if(!display)
    {
      return false;
    }

  while(wl_display_prepare_read(display) != 0)
    {
      wl_display_dispatch_pending(display);
    }

  wl_display_flush(display);

  struct pollfd pfd;
  pfd.fd     = wl_display_get_fd(display);
  pfd.events = POLLIN;

  if(poll(&pfd, 1, 0) > 0)
    {
      wl_display_read_events(display);
    }
   else
    {
      wl_display_cancel_read(display);
    }

  wl_display_dispatch_pending(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    PLATFORM_LINUX
*
* @param[in]  canvas : Canvas object to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Update(GRP2DCANVAS* canvas)
{
  if(!display)
    {
      return false;
    }

  if(!surface)
    {
      return false;
    }

  DispatchEvents();

  // xdg_toplevel_close is the equivalent of X11's WM_DELETE_WINDOW ClientMessage -- but pushed
  // via XDGToplevel_Close() (invoked from inside DispatchEvents() above) instead of polled here,
  // so by the time we reach this point 'canclose'/exit handling driven by it has already run.
  // CanClose()/SetExitType() wiring mirrors GRPLINUXSCREENX11::Update() exactly; see
  // XDGToplevel_Close() below for the actual check.

  if(!canvas)
    {
      return false;
    }

  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      return blitgles->Update(canvas);
    }
  #endif

  if(!configured)
    {
      return true;   // nothing to paint into yet -- no configure/ack round-trip completed
    }

  // No bitmap rescaling on Wayland: the canvas is always blitted 1:1, cropped to the smaller of
  // the canvas' own (fixed design) size (width/height) and the window's current LIVE size
  // (windowwidth/windowheight, kept fresh by Resize()/XDGToplevel_Configure() -- see the
  // windowwidth/windowheight member comment in the header), on each axis independently, and
  // anchored to the window's TOP-LEFT corner -- growing the window reveals more background instead
  // of stretching the canvas to fill it, mirroring the X11 raster path in
  // GRPLINUXSCREENX11::Update(). If the window's live size is below the GRPVIEWPORT_ID_MAIN
  // viewport's declared minimum (either axis), the content is hidden -- but unlike X11 (where
  // simply skipping XPutImage leaves the OS-painted window background visible), a wl_surface shows
  // whatever was PREVIOUSLY committed if nothing new is attached this frame (a STALE frame, not a
  // blank one), so a freshly allocated all-zero buffer is attached/committed instead, actually
  // blanking the surface.
  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);

  bool belowviewportminimum = mainviewport &&
                               ((mainviewport->GetMinWidth()  > 0.0f && windowwidth  < (int)mainviewport->GetMinWidth())  ||
                                (mainviewport->GetMinHeight() > 0.0f && windowheight < (int)mainviewport->GetMinHeight()));

  if(belowviewportminimum)
    {
      if((windowwidth > 0) && (windowheight > 0))
        {
          size_t blankbytes  = (size_t)windowwidth * (size_t)windowheight * 4;
          XBYTE* blankbuffer = (XBYTE*)malloc(blankbytes);

          if(blankbuffer)
            {
              memset(blankbuffer, 0, blankbytes);

              struct wl_buffer* wlbuffer = NULL;
              void*             shmdata  = NULL;

              if(CreateSHMBuffer(windowwidth, windowheight, blankbuffer, &wlbuffer, &shmdata))
                {
                  wl_surface_attach(surface, wlbuffer, 0, 0);
                  wl_surface_damage_buffer(surface, 0, 0, windowwidth, windowheight);
                  wl_surface_commit(surface);
                }

              free(blankbuffer);
            }
        }

      wl_display_flush(display);

      return true;
    }

  XBYTE* srcbuffer = (XBYTE*)canvas->Buffer_Get();

  if(srcbuffer && (windowwidth > 0) && (windowheight > 0))
    {
      int bw = ((int)width  < windowwidth)  ? (int)width  : windowwidth;
      int bh = ((int)height < windowheight) ? (int)height : windowheight;

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
          struct wl_buffer* wlbuffer = NULL;
          void*             shmdata  = NULL;

          if(CreateSHMBuffer(bw, bh, blitbuffer, &wlbuffer, &shmdata))
            {
              wl_surface_attach(surface, wlbuffer, 0, 0);
              wl_surface_damage_buffer(surface, 0, 0, bw, bh);
              wl_surface_commit(surface);
            }
        }

      if(cropped)
        {
          free(blitbuffer);
        }
    }

  wl_display_flush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::UpdateTransparent(GRP2DCANVAS* canvas)
* @brief      Update transparent
* @ingroup    PLATFORM_LINUX
*
* @param[in]  canvas : Canvas object to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::UpdateTransparent(GRP2DCANVAS* canvas)
{
  // wl_shm/EGL buffers with a per-pixel alpha channel are transparent by construction as soon as
  // the surface has no opaque region set (GEN never sets one here) -- there is no separate
  // "transparent update" request to make on Wayland the way X11 needs _COMPTON_SHADOW plus
  // override_redirect. A plain Update() already produces the same visual result.
  return Update(canvas);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Delete()
* @brief      Delete
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Delete()
{
  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      GEN_DELETE blitgles;
      blitgles = NULL;
    }
  #endif

  #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
  if(toplleveldecoration) { zxdg_toplevel_decoration_v1_destroy(toplleveldecoration); toplleveldecoration = NULL; }
  #endif

  if(xdgtoplevel) { xdg_toplevel_destroy(xdgtoplevel); xdgtoplevel = NULL; }
  if(xdgsurface)  { xdg_surface_destroy(xdgsurface);   xdgsurface  = NULL; }
  if(surface)     { wl_surface_destroy(surface);       surface     = NULL; }

  configured = false;

  // Was missing: GRPSCREEN::Delete() is what actually frees 'screencanvas' (created by
  // GRPSCREEN::Create() -> GRPFACTORY::CreateCanvas(), see GRPLINUXSCREENX11::Delete() for the
  // exact same chain-up). Without this call the AGG canvas -- and everything it transitively
  // owns (pixel buffer, rasterizer/renderer state, a couple of XLINUXFactory sync primitives)
  // -- leaked on every screen delete, since nothing else in this class ever touches screencanvas.
  return GRPSCREEN::Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Resize(int width, int height)
* @brief      Resize
* @note       Tracks the requested LIVE window size (windowwidth/windowheight) separately from the
*             canvas' fixed design size (inherited GRPPROPERTIES::width/height via GetWidth()/
*             GetHeight()) -- SetSize() is deliberately NOT called here, matching the invariant
*             GRPLINUXSCREENX11::Resize() already preserves (by never calling SetSize() either) and
*             GRPWINDOWSSCREEN relies on: the canvas' design size is set once by the app and never
*             touched again by window-resize code. Prior to this port this method called SetSize()
*             directly, conflating live window size with canvas design size (a Wayland-only bug --
*             see the windowwidth/windowheight member comment in the header). blitgles->Resize() is
*             likewise deliberately NOT called here: it reallocates the GL TEXTURE that holds the
*             canvas' pixel CONTENT, sized to match canvas->GetWidth()/GetHeight() (see
*             GRPBLITGLES::Update(), which calls Resize() itself whenever it detects a texture-size
*             mismatch) -- calling it with the WINDOW's size here would corrupt/thrash that texture
*             size for no benefit, since it gets silently corrected back on the very next Update()
*             call anyway.
* @ingroup    PLATFORM_LINUX
*
* @param[in]  width : Requested LIVE window width.
* @param[in]  height : Requested LIVE window height.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Resize(int width, int height)
{
  if(!display || !surface)
    {
      return false;
    }

  // xdg-shell has no "request this size" call for a toplevel that is not in a resizing
  // interactive grab -- the compositor decides and tells the client via configure. GEN records
  // the request locally so content already looks right even before any compositor round-trip
  // confirms it, same spirit as GRPLINUXSCREENX11::Resize().
  ClampToViewportMax(width, height);

  windowwidth  = width;
  windowheight = height;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Set_Position(int x, int y)
* @brief      Set position
* @ingroup    PLATFORM_LINUX
*
* @param[in]  x : X value.
* @param[in]  y : Y value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Set_Position(int x, int y)
{
  // No equivalent exists. xdg-shell deliberately gives clients no way to place themselves on
  // screen (the compositor owns window placement, by design, for security/isolation reasons) --
  // unlike GRPLINUXSCREENX11::Set_Position(), which walks the X11 parent chain and issues
  // XMoveWindow() on a second connection to work around a WSLg quirk, there is no request to
  // send here at all. Custom-chrome window dragging (GRPSCREEN::UpdateCFGChromesDrag(), which
  // calls this on every drag-move tick on X11) simply cannot work the same way under Wayland;
  // the only sanctioned mechanism is xdg_toplevel_move(), which must be issued synchronously
  // from inside a pointer-button-press event handler (it needs that event's serial) and hands
  // the ENTIRE interactive move over to the compositor -- GEN cannot drive it frame-by-frame
  // from arbitrary (x,y) the way it drives X11. Documented as a known platform limitation; see
  // the study document for the xdg_toplevel_move() based follow-up design.
  SetPosition(x, y);

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Show(bool active)
* @brief      Show
* @ingroup    PLATFORM_LINUX
*
* @param[in]  active : Active value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Show(bool active)
{
  if(!display || !surface)
    {
      return false;
    }

  if(!active)
    {
      // xdg-shell has no "hide" request either (nor does it have "iconify" outside
      // xdg_toplevel_set_minimized(), handled in Minimize() below) -- attaching a NULL buffer
      // and committing is the documented way to make a surface stop being displayed without
      // destroying it.
      wl_surface_attach(surface, NULL, 0, 0);
      wl_surface_commit(surface);

      isshow = false;

      return true;
    }

  isshow = true;

  wl_display_flush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::ShowCursor(bool active)
* @brief      Show cursor
* @ingroup    PLATFORM_LINUX
*
* @param[in]  active : Active value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::ShowCursor(bool active)
{
  if(!pointer)
    {
      return false;
    }

  if(!active)
    {
      // wl_pointer_set_cursor(serial, NULL, 0, 0) is the documented way to hide the cursor --
      // notably simpler than X11's ShowCursor(false), which has to fabricate a 1x1 transparent
      // XCreateBitmapFromData()/XCreatePixmapCursor() and XDefineCursor() it. The serial MUST be
      // the one from the last wl_pointer::enter event (lastpointerserial, updated in
      // Pointer_Enter()) -- there is no X11 equivalent of tracking this.
      wl_pointer_set_cursor(pointer, lastpointerserial, NULL, 0, 0);

      return true;
    }

  // Restoring the default/system cursor image needs wl_cursor_theme_load() +
  // wl_cursor_theme_get_cursor() (libwayland-cursor) and re-attaching its wl_buffer on a small
  // dedicated cursor wl_surface; left as a documented follow-up rather than implemented here,
  // since GEN's own INPCURSOR already draws a software cursor for most applications and this
  // path is only relevant to apps that want the native system arrow back.
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Minimize(bool active)
* @brief      Minimize
* @ingroup    PLATFORM_LINUX
*
* @param[in]  active : Active value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Minimize(bool active)
{
  if(!xdgtoplevel)
    {
      return false;
    }

  if(!active)
    {
      // xdg-shell asymmetry worth flagging explicitly: xdg_toplevel_set_minimized() exists,
      // but there is no xdg_toplevel_unset_minimized() and no configure event announcing "the
      // user restored it from the taskbar" either -- unlike X11 (XMapWindow() un-iconifies
      // unconditionally and reliably), a client cannot programmatically un-minimize itself on
      // Wayland at all; only the user/compositor can. Returning false here is honest, not a
      // stub -- there is no request this could send.
      return false;
    }

  xdg_toplevel_set_minimized(xdgtoplevel);

  wl_display_flush(display);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::Maximize(bool active)
* @brief      Maximize
* @ingroup    PLATFORM_LINUX
*
* @param[in]  active : Active value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::Maximize(bool active)
{
  if(!xdgtoplevel)
    {
      return false;
    }

  if(active)
       xdg_toplevel_set_maximized(xdgtoplevel);
  else xdg_toplevel_unset_maximized(xdgtoplevel);

  wl_display_flush(display);

  // Unlike X11's Maximize() (a fire-and-forget _NET_WM_STATE ClientMessage with no
  // confirmation), the compositor answers this with an xdg_toplevel::configure carrying the
  // XDG_TOPLEVEL_STATE_MAXIMIZED state, handled asynchronously in XDGToplevel_Configure() --
  // the resulting live-size update (windowwidth/windowheight, NOT SetSize() -- see the
  // windowwidth/windowheight member comment in the header) happens there, once the compositor
  // actually agrees.
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void* GRPLINUXSCREENWAYLAND::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_LINUX
*
* @return     void* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPLINUXSCREENWAYLAND::GetHandle()
{
  // GRPLINUXSCREENX11::GetHandle() returns the XID cast to void* -- an integer disguised as a
  // pointer. wl_surface* IS already a real, dereferenceable pointer, so this is arguably more
  // honest than the X11 version, but any external code that assumed "GetHandle() on Linux is
  // always an XID" (no evidence any GEN code does, but third-party integrations might) will
  // need updating.
  return (void*)surface;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::IsDesktop()
* @brief      Is desktop
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::IsDesktop()
{
  return isdesktop;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::SetIsDesktop(bool isdesktop)
* @brief      Set is desktop
* @ingroup    PLATFORM_LINUX
*
* @param[in]  isdesktop : Isdesktop value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::SetIsDesktop(bool isdesktop)
{
  this->isdesktop = isdesktop;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::HasFocus()
* @brief      Has focus
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::HasFocus()
{
  // GRPLINUXSCREENX11::HasFocus() asks the server synchronously (XGetInputFocus()). Wayland has
  // no such query -- focus is only ever PUSHED to the client via wl_keyboard::enter/leave, so
  // this reads a flag updated by Keyboard_Enter()/Keyboard_Leave() instead of asking anything.
  // Both INPLINUXDEVICEKEYBOARDWAYLAND::Update() and INPLINUXDEVICEMOUSEWAYLAND::Update() call
  // this exactly like their X11 counterparts call grpscreenx11->HasFocus().
  return haskeyboardfocus;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct wl_display* GRPLINUXSCREENWAYLAND::GetWLDisplay()
* @brief      Get display
* @ingroup    PLATFORM_LINUX
*
* @return     struct wl_display* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct wl_display* GRPLINUXSCREENWAYLAND::GetWLDisplay()
{
  return display;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct wl_surface* GRPLINUXSCREENWAYLAND::GetWLSurface()
* @brief      Get surface
* @ingroup    PLATFORM_LINUX
*
* @return     struct wl_surface* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct wl_surface* GRPLINUXSCREENWAYLAND::GetWLSurface()
{
  return surface;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct wl_seat* GRPLINUXSCREENWAYLAND::GetWLSeat()
* @brief      Get seat
* @ingroup    PLATFORM_LINUX
*
* @return     struct wl_seat* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct wl_seat* GRPLINUXSCREENWAYLAND::GetWLSeat()
{
  return seat;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct wl_pointer* GRPLINUXSCREENWAYLAND::GetWLPointer()
* @brief      Get pointer
* @ingroup    PLATFORM_LINUX
*
* @return     struct wl_pointer* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct wl_pointer* GRPLINUXSCREENWAYLAND::GetWLPointer()
{
  return pointer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct wl_keyboard* GRPLINUXSCREENWAYLAND::GetWLKeyboard()
* @brief      Get keyboard
* @ingroup    PLATFORM_LINUX
*
* @return     struct wl_keyboard* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct wl_keyboard* GRPLINUXSCREENWAYLAND::GetWLKeyboard()
{
  return keyboard;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         struct xdg_toplevel* GRPLINUXSCREENWAYLAND::GetXDGToplevel()
* @brief      Get XDG toplevel
* @ingroup    PLATFORM_LINUX
*
* @return     struct xdg_toplevel* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
struct xdg_toplevel* GRPLINUXSCREENWAYLAND::GetXDGToplevel()
{
  return xdgtoplevel;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD GRPLINUXSCREENWAYLAND::GetLastPointerSerial()
* @brief      Get last pointer serial
* @ingroup    PLATFORM_LINUX
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPLINUXSCREENWAYLAND::GetLastPointerSerial()
{
  return lastpointerserial;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD GRPLINUXSCREENWAYLAND::GetLastKeyboardSerial()
* @brief      Get last keyboard serial
* @ingroup    PLATFORM_LINUX
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPLINUXSCREENWAYLAND::GetLastKeyboardSerial()
{
  return lastkeyboardserial;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::PopButtonEvent(XDWORD& code, bool& pressed)
* @brief      Pop button event
* @ingroup    PLATFORM_LINUX
*
* @param[out] code : Receives the Linux evdev BTN_* code.
* @param[out] pressed : Receives the pressed value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::PopButtonEvent(XDWORD& code, bool& pressed)
{
  if(buttonqueuehead == buttonqueuetail) return false;

  code    = buttonqueue[buttonqueuehead].code;
  pressed = buttonqueue[buttonqueuehead].pressed;

  buttonqueuehead = (buttonqueuehead + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::PopKeyEvent(XDWORD& keysym, bool& pressed)
* @brief      Pop key event
* @ingroup    PLATFORM_LINUX
*
* @param[out] keysym : Receives the xkb_keysym_t value.
* @param[out] pressed : Receives the pressed value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::PopKeyEvent(XDWORD& keysym, bool& pressed)
{
  if(keyqueuehead == keyqueuetail) return false;

  keysym  = keyqueue[keyqueuehead].keysym;
  pressed = keyqueue[keyqueuehead].pressed;

  keyqueuehead = (keyqueuehead + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::GetPointerPosition(int& x, int& y)
* @brief      Get pointer position
* @ingroup    PLATFORM_LINUX
*
* @param[out] x : Receives the last known surface-local X position.
* @param[out] y : Receives the last known surface-local Y position.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::GetPointerPosition(int& x, int& y)
{
  x = pointerx;
  y = pointery;

  return haspointerfocus;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int GRPLINUXSCREENWAYLAND::GetAndResetScrollDelta()
* @brief      Get and reset scroll delta
* @ingroup    PLATFORM_LINUX
*
* @return     int : Accumulated vertical scroll delta since the last call; positive = down.
*
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENWAYLAND::GetAndResetScrollDelta()
{
  int delta = scrolldelta;

  scrolldelta = 0;

  return delta;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int GRPLINUXSCREENWAYLAND::GetWindowWidth()
* @brief      Get the LIVE compositor-driven window width -- see the windowwidth member comment in
*             the header for why this is deliberately separate from GetWidth() (the canvas' fixed
*             design size).
* @ingroup    PLATFORM_LINUX
*
* @return     int : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENWAYLAND::GetWindowWidth()
{
  return windowwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int GRPLINUXSCREENWAYLAND::GetWindowHeight()
* @brief      Get the LIVE compositor-driven window height -- see the windowwidth member comment in
*             the header for why this is deliberately separate from GetHeight() (the canvas' fixed
*             design size).
* @ingroup    PLATFORM_LINUX
*
* @return     int : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENWAYLAND::GetWindowHeight()
{
  return windowheight;
}


#ifdef GRP_OPENGL_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPLINUXBLITGLESWAYLAND* GRPLINUXSCREENWAYLAND::GetBlitGLES()
* @brief      Get blit GLES
* @ingroup    PLATFORM_LINUX
*
* @return     GRPLINUXBLITGLESWAYLAND* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLESWAYLAND* GRPLINUXSCREENWAYLAND::GetBlitGLES()
{
  return blitgles;
}
#endif


// ------------------------------------------------------------------------------------------------
// Registry / xdg_wm_base / xdg_surface / xdg_toplevel / wl_seat / wl_pointer / wl_keyboard static
// listener callbacks. All receive 'data' == the GRPLINUXSCREENWAYLAND instance passed to
// *_add_listener() above.
// ------------------------------------------------------------------------------------------------


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Registry_Global(void* data, struct wl_registry* registry, XDWORD name, const char* interface, XDWORD version)
* @brief      wl_registry::global listener -- binds the globals GEN needs as the compositor
*             announces them (equivalent, conceptually, to the globals Xlib exposes for free
*             right after XOpenDisplay()).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  registry : Registry object to use.
* @param[in]  name : Global numeric name to use.
* @param[in]  interface : Interface string to use.
* @param[in]  version : Version value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Registry_Global(void* data, struct wl_registry* registry, XDWORD name, const char* interface, XDWORD version)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  if(strcmp(interface, wl_compositor_interface.name) == 0)
    {
      screen->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, (version < 4) ? version : 4);
    }
   else
    {
      if(strcmp(interface, wl_shm_interface.name) == 0)
        {
          screen->shm = (struct wl_shm*)wl_registry_bind(registry, name, &wl_shm_interface, 1);
        }
       else
        {
          if(strcmp(interface, xdg_wm_base_interface.name) == 0)
            {
              screen->xdgwmbase = (struct xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
            }
           else
            {
              if(strcmp(interface, wl_seat_interface.name) == 0)
                {
                  screen->seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, (version < 5) ? version : 5);
                }
               else
                {
                  if(strcmp(interface, wl_output_interface.name) == 0)
                    {
                      struct wl_output* output = (struct wl_output*)wl_registry_bind(registry, name, &wl_output_interface, 1);
                      if(output)
                        {
                          screen->outputs.Add(output);
                        }
                    }
                   else
                    {
                      #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
                      if(strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0)
                        {
                          screen->decorationmanager = (struct zxdg_decoration_manager_v1*)wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, 1);
                        }
                      #endif
                    }
                }
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Registry_GlobalRemove(void* data, struct wl_registry* registry, XDWORD name)
* @brief      wl_registry::global_remove listener -- a global (e.g. a monitor being unplugged)
*             went away. No X11 equivalent: Xrandr-enumerated monitors are only ever re-queried
*             on demand, never pushed as a removal event.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  registry : Registry object to use.
* @param[in]  name : Global numeric name to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Registry_GlobalRemove(void* data, struct wl_registry* registry, XDWORD name)
{
  // Deliberately not tracking wl_output <-> numeric-name association to remove the exact
  // wl_output* here (would need a small name->wl_output* map); left as a documented follow-up.
  // A monitor unplugged mid-run today just leaves a stale entry in 'outputs' until the process
  // restarts -- no worse than GRPLINUXDESKTOPMONITORS, which does not refresh at all after
  // construction either (see GRPLINUXDesktopManager.cpp).
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::XDGWMBase_Ping(void* data, struct xdg_wm_base* xdgwmbase, XDWORD serial)
* @brief      xdg_wm_base::ping listener -- the compositor's liveness check. Must be answered with
*             xdg_wm_base_pong() or the compositor will consider the client unresponsive and may
*             grey it out / offer to kill it, the Wayland equivalent of an X11 WM's "Force Quit"
*             dialog for a client stuck not pumping its event loop.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  xdgwmbase : XDG WM base object to use.
* @param[in]  serial : Serial value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::XDGWMBase_Ping(void* data, struct xdg_wm_base* xdgwmbase, XDWORD serial)
{
  xdg_wm_base_pong(xdgwmbase, serial);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::XDGSurface_Configure(void* data, struct xdg_surface* xdgsurface, XDWORD serial)
* @brief      xdg_surface::configure listener -- MUST be acked (xdg_surface_ack_configure()) for
*             every single configure the compositor sends, or the compositor will eventually give
*             up on the client. This is the closest Wayland gets to "the window now exists and is
*             ready to be drawn into", the point GEN flips 'configured' to true.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  xdgsurface : XDG surface object to use.
* @param[in]  serial : Serial value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::XDGSurface_Configure(void* data, struct xdg_surface* xdgsurface, XDWORD serial)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;

  xdg_surface_ack_configure(xdgsurface, serial);

  if(screen)
    {
      screen->configured = true;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::XDGToplevel_Configure(void* data, struct xdg_toplevel* xdgtoplevel, int width, int height, struct wl_array* states)
* @brief      xdg_toplevel::configure listener -- the compositor's answer to "what size/state
*             should this toplevel be". width/height are HINTS: 0 means "you choose" (typically
*             the very first configure, before the compositor has an opinion), matching
*             GRPLINUXSCREENWAYLAND_DEFAULT_WIDTH/HEIGHT already seeded in the constructor. This
*             is the asynchronous counterpart of the synchronous resize GRPLINUXSCREENX11 gets
*             for free from XGetWindowAttributes() inside its own Update().
* @note       Re-asserts ApplyMaxSizeHint() on every call (cheap/idempotent), mirroring how
*             GRPWINDOWSSCREEN recomputes its cap fresh on every WM_GETMINMAXINFO call rather than
*             once at creation -- covers the case where the app only creates GRPVIEWPORT_ID_MAIN
*             after Create_Surface() already ran its own one-shot call. Updates windowwidth/
*             windowheight (the LIVE window size), clamped to the viewport's max via
*             ClampToViewportMax() -- NOT SetSize() (which now represents only the canvas' fixed
*             design size, see the windowwidth/windowheight member comment in the header) and NOT
*             blitgles->Resize() (which resizes the canvas content TEXTURE, driven instead by
*             canvas->GetWidth()/GetHeight() inside GRPBLITGLES::Update() -- see the Resize() @note
*             above for the full rationale, identical here).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  xdgtoplevel : XDG toplevel object to use.
* @param[in]  width : Width value (0 = compositor has no preference).
* @param[in]  height : Height value (0 = compositor has no preference).
* @param[in]  states : Array of XDG_TOPLEVEL_STATE_* enum values currently active (maximized,
*                       fullscreen, resizing, activated...).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::XDGToplevel_Configure(void* data, struct xdg_toplevel* xdgtoplevel, int width, int height, struct wl_array* states)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->ApplyMaxSizeHint();

  if((width > 0) && (height > 0))
    {
      screen->ClampToViewportMax(width, height);

      screen->windowwidth  = width;
      screen->windowheight = height;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::XDGToplevel_Close(void* data, struct xdg_toplevel* xdgtoplevel)
* @brief      xdg_toplevel::close listener -- the user asked the compositor to close this window
*             (clicked the [x] the compositor itself drew, or an equivalent gesture). The direct
*             equivalent of X11's WM_DELETE_WINDOW ClientMessage, handled in
*             GRPLINUXSCREENX11::Update() -- ported here verbatim, just triggered by a listener
*             callback instead of a polled XCheckTypedWindowEvent().
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  xdgtoplevel : XDG toplevel object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::XDGToplevel_Close(void* data, struct xdg_toplevel* xdgtoplevel)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  if(!screen->CanClose())
    {
      return;
    }

  if(mainproclinux.GetAppMain())
    {
      APPFLOWGRAPHICS* appgraphics = dynamic_cast<APPFLOWGRAPHICS*>(mainproclinux.GetAppMain()->GetApplication());
      if(appgraphics)
        {
          if((GRPSCREEN*)screen == appgraphics->GetMainScreen())
            {
              appgraphics->SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
            }
           else
            {
              screen->Delete();
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Seat_Capabilities(void* data, struct wl_seat* seat, XDWORD capabilities)
* @brief      wl_seat::capabilities listener -- announces which input devices this seat has
*             (pointer/keyboard/touch), and is re-sent whenever that set changes (e.g. a USB
*             mouse unplugged). Binds/releases wl_pointer and wl_keyboard accordingly, and
*             installs THIS class's own listener on each (see the one-listener-per-proxy note at
*             the top of this file for why INPLINUXDEVICEMOUSEWAYLAND/INPLINUXDEVICEKEYBOARDWAYLAND
*             cannot install their own instead).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  seat : Seat object to use.
* @param[in]  capabilities : Bitmask of WL_SEAT_CAPABILITY_* values.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Seat_Capabilities(void* data, struct wl_seat* seat, XDWORD capabilities)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  bool haspointer  = (capabilities & WL_SEAT_CAPABILITY_POINTER)  ? true : false;
  bool haskeyboard = (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) ? true : false;

  if(haspointer && !screen->pointer)
    {
      screen->pointer = wl_seat_get_pointer(seat);
      if(screen->pointer)
        {
          wl_pointer_add_listener(screen->pointer, &grplinuxscreenwayland_pointer_listener, screen);
        }
    }
   else
    {
      if(!haspointer && screen->pointer)
        {
          // wl_pointer_destroy() (available since interface version 1) instead of the "cleaner"
          // wl_pointer_release() (only valid from version 3 onward) -- avoids asserting on a
          // seat bound at an old version by a minimal/older compositor.
          wl_pointer_destroy(screen->pointer);
          screen->pointer = NULL;
        }
    }

  if(haskeyboard && !screen->keyboard)
    {
      screen->keyboard = wl_seat_get_keyboard(seat);
      if(screen->keyboard)
        {
          wl_keyboard_add_listener(screen->keyboard, &grplinuxscreenwayland_keyboard_listener, screen);
        }
    }
   else
    {
      if(!haskeyboard && screen->keyboard)
        {
          wl_keyboard_destroy(screen->keyboard);
          screen->keyboard = NULL;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Seat_Name(void* data, struct wl_seat* seat, const char* name)
* @brief      wl_seat::name listener -- informational only (human-readable seat name), not used.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  seat : Seat object to use.
* @param[in]  name : Name string to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Seat_Name(void* data, struct wl_seat* seat, const char* name)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Enter(void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy)
* @brief      wl_pointer::enter listener -- pointer entered our surface. Updates haspointerfocus
*             and lastpointerserial (needed by ShowCursor()'s wl_pointer_set_cursor() call).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  serial : Serial value.
* @param[in]  surface : Surface object to use.
* @param[in]  sx : Surface-local X position, fixed-point.
* @param[in]  sy : Surface-local Y position, fixed-point.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Enter(void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->haspointerfocus   = true;
  screen->lastpointerserial = serial;
  screen->pointerx          = wl_fixed_to_int(sx);
  screen->pointery          = wl_fixed_to_int(sy);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Leave(void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface)
* @brief      wl_pointer::leave listener -- pointer left our surface.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  serial : Serial value.
* @param[in]  surface : Surface object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Leave(void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->haspointerfocus   = false;
  screen->lastpointerserial = serial;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Motion(void* data, struct wl_pointer* pointer, XDWORD time, wl_fixed_t sx, wl_fixed_t sy)
* @brief      wl_pointer::motion listener -- direct push equivalent of X11's XQueryPointer()
*             poll. Coordinates arrive already relative to our surface, unlike XQueryPointer()
*             (relative to the root window, requiring the framework to compute an offset itself).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  time : Timestamp (compositor clock, milliseconds).
* @param[in]  sx : Surface-local X position, fixed-point.
* @param[in]  sy : Surface-local Y position, fixed-point.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Motion(void* data, struct wl_pointer* pointer, XDWORD time, wl_fixed_t sx, wl_fixed_t sy)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->pointerx = wl_fixed_to_int(sx);
  screen->pointery = wl_fixed_to_int(sy);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Button(void* data, struct wl_pointer* pointer, XDWORD serial, XDWORD time, XDWORD button, XDWORD state)
* @brief      wl_pointer::button listener -- push equivalent of X11's ButtonPress/ButtonRelease
*             XCheckWindowEvent() results. 'button' is a Linux evdev BTN_* code (BTN_LEFT=0x110,
*             BTN_RIGHT=0x111, BTN_MIDDLE=0x112), NOT an X11 Button1/2/3 index -- INPBUTTON codes
*             registered by INPLINUXDEVICEMOUSEWAYLAND::CreateAllButtons() use the evdev values.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  serial : Serial value.
* @param[in]  time : Timestamp (compositor clock, milliseconds).
* @param[in]  button : Linux evdev BTN_* code.
* @param[in]  state : WL_POINTER_BUTTON_STATE_PRESSED or WL_POINTER_BUTTON_STATE_RELEASED.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Button(void* data, struct wl_pointer* pointer, XDWORD serial, XDWORD time, XDWORD button, XDWORD state)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->lastpointerserial = serial;

  screen->PushButtonEvent(button, (state == WL_POINTER_BUTTON_STATE_PRESSED));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Axis(void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis, wl_fixed_t value)
* @brief      wl_pointer::axis listener -- scroll wheel. Genuinely new functionality, not a port:
*             X11's INPLINUXDEVICEMOUSEX11::CreateAllButtons() never registered Button4/Button5
*             (the X11 scroll-wheel convention) in the first place, so there is nothing to keep
*             parity with here -- this is scroll support GEN's Linux backends did not have before.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  time : Timestamp (compositor clock, milliseconds).
* @param[in]  axis : WL_POINTER_AXIS_VERTICAL_SCROLL or WL_POINTER_AXIS_HORIZONTAL_SCROLL.
* @param[in]  value : Scroll amount, fixed-point, positive = down/right.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Axis(void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis, wl_fixed_t value)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  if(axis == WL_POINTER_AXIS_VERTICAL_SCROLL)
    {
      screen->scrolldelta += wl_fixed_to_int(value);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_Frame(void* data, struct wl_pointer* pointer)
* @brief      wl_pointer::frame listener -- marks the end of a logically-grouped batch of pointer
*             events (e.g. one motion + one axis delivered together for the same input). GEN does
*             not need batching (each event already updates independent state immediately), so
*             this is a required-but-empty listener entry -- wl_pointer_listener has no "optional"
*             callbacks, every entry must be a valid function pointer.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_Frame(void* data, struct wl_pointer* pointer)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_AxisSource(void* data, struct wl_pointer* pointer, XDWORD axissource)
* @brief      wl_pointer::axis_source listener (wheel/finger/continuous/wheel_tilt) -- not used;
*             required listener table entry, see Pointer_Frame() above for why it cannot be NULL.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  axissource : WL_POINTER_AXIS_SOURCE_* value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_AxisSource(void* data, struct wl_pointer* pointer, XDWORD axissource)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_AxisStop(void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis)
* @brief      wl_pointer::axis_stop listener -- not used; see Pointer_Frame() above.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  time : Timestamp (compositor clock, milliseconds).
* @param[in]  axis : WL_POINTER_AXIS_* value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_AxisStop(void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Pointer_AxisDiscrete(void* data, struct wl_pointer* pointer, XDWORD axis, int discrete)
* @brief      wl_pointer::axis_discrete listener (legacy "number of wheel clicks") -- not used;
*             GEN reads the continuous axis value instead (Pointer_Axis() above). See
*             Pointer_Frame() above for why the entry cannot be NULL.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  pointer : Pointer object to use.
* @param[in]  axis : WL_POINTER_AXIS_* value.
* @param[in]  discrete : Number of discrete wheel clicks.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Pointer_AxisDiscrete(void* data, struct wl_pointer* pointer, XDWORD axis, int discrete)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_Keymap(void* data, struct wl_keyboard* keyboard, XDWORD format, int fd, XDWORD size)
* @brief      wl_keyboard::keymap listener -- compositor hands us a shared-memory fd containing
*             the active XKB keymap description. Builds xkbcontext/xkbkeymap/xkbstate from it.
*             This is the Wayland replacement for Xlib's built-in keysym tables (XLookupKeysym
*             etc.): under Wayland the keymap is compositor-supplied and can vary at runtime, so
*             it must be parsed on receipt rather than assumed to be a fixed built-in table.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  format : WL_KEYBOARD_KEYMAP_FORMAT_* value.
* @param[in]  fd : Shared-memory file descriptor containing the keymap string.
* @param[in]  size : Size in bytes of the keymap string (including trailing NUL).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_Keymap(void* data, struct wl_keyboard* keyboard, XDWORD format, int fd, XDWORD size)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen)
    {
      close(fd);
      return;
    }

  if(format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1)
    {
      close(fd);
      return;
    }

  char* mapstr = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(mapstr == MAP_FAILED)
    {
      close(fd);
      return;
    }

  if(screen->xkbstate)  { xkb_state_unref(screen->xkbstate);   screen->xkbstate  = NULL; }
  if(screen->xkbkeymap) { xkb_keymap_unref(screen->xkbkeymap); screen->xkbkeymap = NULL; }

  screen->xkbkeymap = xkb_keymap_new_from_string(screen->xkbcontext, mapstr, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

  munmap(mapstr, size);
  close(fd);

  if(screen->xkbkeymap)
    {
      screen->xkbstate = xkb_state_new(screen->xkbkeymap);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_Enter(void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface, struct wl_array* keys)
* @brief      wl_keyboard::enter listener -- keyboard focus entered our surface.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  serial : Serial value.
* @param[in]  surface : Surface object to use.
* @param[in]  keys : Array of keycodes currently pressed (not consumed by GEN).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_Enter(void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface, struct wl_array* keys)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->haskeyboardfocus   = true;
  screen->lastkeyboardserial = serial;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_Leave(void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface)
* @brief      wl_keyboard::leave listener -- keyboard focus left our surface.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  serial : Serial value.
* @param[in]  surface : Surface object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_Leave(void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen) return;

  screen->haskeyboardfocus   = false;
  screen->lastkeyboardserial = serial;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_Key(void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD time, XDWORD key, XDWORD state)
* @brief      wl_keyboard::key listener -- push equivalent of X11's KeyPress/KeyRelease
*             XCheckWindowEvent() results. 'key' is a Linux evdev keycode; per the Wayland
*             protocol it must be offset by +8 to obtain the XKB keycode (historical XKB/evdev
*             offset, same +8 rule Xlib applies internally). The resulting keysym numeric value
*             is identical to X11's XK_* constants, so INPLINUXDEVICEKEYBOARDWAYLAND can reuse
*             the exact same INPBUTTON registration table as INPLINUXDEVICEKEYBOARDX11.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  serial : Serial value.
* @param[in]  time : Timestamp (compositor clock, milliseconds).
* @param[in]  key : Linux evdev keycode.
* @param[in]  state : WL_KEYBOARD_KEY_STATE_PRESSED or WL_KEYBOARD_KEY_STATE_RELEASED.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_Key(void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD time, XDWORD key, XDWORD state)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen)              return;
  if(!screen->xkbstate)    return;

  screen->lastkeyboardserial = serial;

  XDWORD    xkbkeycode = key + 8;
  xkb_keysym_t keysym  = xkb_state_key_get_one_sym(screen->xkbstate, xkbkeycode);

  if(keysym == XKB_KEY_NoSymbol) return;

  screen->PushKeyEvent((XDWORD)keysym, (state == WL_KEYBOARD_KEY_STATE_PRESSED));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_Modifiers(void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD modsdepressed, XDWORD modslatched, XDWORD modslocked, XDWORD group)
* @brief      wl_keyboard::modifiers listener -- updates xkbstate's modifier/group mask so that
*             subsequent xkb_state_key_get_one_sym() calls in Keyboard_Key() resolve keysyms with
*             the correct Shift/Ctrl/Alt/AltGr/level state applied (equivalent to what Xlib does
*             internally on every KeyPress via the core protocol's implicit state).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  serial : Serial value.
* @param[in]  modsdepressed : Depressed modifiers mask.
* @param[in]  modslatched : Latched modifiers mask.
* @param[in]  modslocked : Locked modifiers mask.
* @param[in]  group : Keyboard layout group (e.g. active layout index).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_Modifiers(void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD modsdepressed, XDWORD modslatched, XDWORD modslocked, XDWORD group)
{
  GRPLINUXSCREENWAYLAND* screen = (GRPLINUXSCREENWAYLAND*)data;
  if(!screen)           return;
  if(!screen->xkbstate) return;

  xkb_state_update_mask(screen->xkbstate, modsdepressed, modslatched, modslocked, 0, 0, group);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Keyboard_RepeatInfo(void* data, struct wl_keyboard* keyboard, int rate, int delay)
* @brief      wl_keyboard::repeat_info listener -- compositor-suggested key auto-repeat rate and
*             delay. Intentionally a no-op: GEN's existing X11 keyboard input
*             (INPLINUXDEVICEKEYBOARDX11) does not implement software auto-repeat handling either,
*             so leaving this unimplemented preserves exact behavioral parity between backends
*             rather than fixing a pre-existing gap as an unplanned side effect of this port.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  keyboard : Keyboard object to use.
* @param[in]  rate : Repeats per second (0 = disabled).
* @param[in]  delay : Delay in milliseconds before repeating starts.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Keyboard_RepeatInfo(void* data, struct wl_keyboard* keyboard, int rate, int delay)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::PushButtonEvent(XDWORD code, bool pressed)
* @brief      Pushes a button event into the fixed-size circular FIFO that
*             INPLINUXDEVICEMOUSEWAYLAND::Update() drains via PopButtonEvent(). On overflow
*             (consumer not draining fast enough) the oldest queued event is silently dropped to
*             make room, matching the bounded-memory, best-effort spirit of X11's XCheckWindowEvent()
*             polling (which itself risks losing events for the equivalent reasons if not polled
*             often enough).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  code : Linux evdev BTN_* code.
* @param[in]  pressed : true = pressed, false = released.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::PushButtonEvent(XDWORD code, bool pressed)
{
  XDWORD nexttail = (buttonqueuetail + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;

  if(nexttail == buttonqueuehead)
    {
      buttonqueuehead = (buttonqueuehead + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;
    }

  buttonqueue[buttonqueuetail].code    = code;
  buttonqueue[buttonqueuetail].pressed = pressed;
  buttonqueuetail                      = nexttail;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::PushKeyEvent(XDWORD keysym, bool pressed)
* @brief      Pushes a key event into the fixed-size circular FIFO that
*             INPLINUXDEVICEKEYBOARDWAYLAND::Update() drains via PopKeyEvent(). Same
*             overflow policy as PushButtonEvent() above.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  keysym : XKB/X11 keysym value (XK_* constant space).
* @param[in]  pressed : true = pressed, false = released.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::PushKeyEvent(XDWORD keysym, bool pressed)
{
  XDWORD nexttail = (keyqueuetail + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;

  if(nexttail == keyqueuehead)
    {
      keyqueuehead = (keyqueuehead + 1) % GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE;
    }

  keyqueue[keyqueuetail].keysym  = keysym;
  keyqueue[keyqueuetail].pressed = pressed;
  keyqueuetail                   = nexttail;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE* GRPLINUXSCREENWAYLAND::ScaleBufferNearestLetterbox(XBYTE* src, int srcw, int srch, int dstw, int dsth)
* @brief      Nearest-neighbour scale of a 32bpp BGRA buffer from (srcw x srch) to (dstw x dsth),
*             letterboxed (aspect ratio preserved, remaining border filled opaque black).
* @note       DEAD CODE, kept only for reference/possible future reuse. No longer called: both the
*             X11 raster path (GRPLINUXSCREENX11::Update()) and this class's own raster path
*             (Update() above) now crop-to-top-left-anchor instead of rescaling, matching the
*             native-1:1/top-left-anchored presentation policy this whole port establishes (see
*             GRPLINUXBLITGLESX11::ComputePresentationScale / GRPLINUXBLITGLESWAYLAND::
*             ComputePresentationScale for the GL-path equivalent). The stale claim this comment
*             used to make -- that "X11 can rely on ConfigureNotify plus the framework's own
*             Resize()/UpdateSize() sequencing to keep the canvas and window size in lockstep" --
*             was never actually true: GRPLINUXSCREENX11 has no ConfigureNotify handling anywhere,
*             and its Resize()/UpdateSize() never touch width/height at all (confirmed by direct
*             reading of that file). What both platforms actually rely on now is the SAME
*             invariant: GetWidth()/GetHeight() (GRPPROPERTIES::width/height) always reflect the
*             canvas' FIXED design size, never the live window size, on every platform -- X11 always
*             preserved this (by omission: Resize()/UpdateSize() never called SetSize()); Wayland
*             did NOT (Resize()/XDGToplevel_Configure() used to call SetSize() with the live size,
*             a Wayland-only bug fixed by this port -- see the windowwidth/windowheight member
*             comment in the header for the live-size tracking that replaces it).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  src : Source buffer, srcw*srch 32bpp BGRA pixels.
* @param[in]  srcw : Source width in pixels.
* @param[in]  srch : Source height in pixels.
* @param[in]  dstw : Destination width in pixels.
* @param[in]  dsth : Destination height in pixels.
*
* @return     XBYTE* : Newly malloc()'d dstw*dsth 32bpp BGRA buffer (caller must free()), or NULL on failure.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPLINUXSCREENWAYLAND::ScaleBufferNearestLetterbox(XBYTE* src, int srcw, int srch, int dstw, int dsth)
{
  if(!src)                       return NULL;
  if((srcw <= 0) || (srch <= 0)) return NULL;
  if((dstw <= 0) || (dsth <= 0)) return NULL;

  XBYTE* dst = (XBYTE*)malloc((size_t)dstw * (size_t)dsth * 4);
  if(!dst) return NULL;

  memset(dst, 0, (size_t)dstw * (size_t)dsth * 4);

  float scalex = (float)dstw / (float)srcw;
  float scaley = (float)dsth / (float)srch;
  float scale  = (scalex < scaley) ? scalex : scaley;

  int scaledw = (int)((float)srcw * scale);
  int scaledh = (int)((float)srch * scale);

  if(scaledw < 1) scaledw = 1;
  if(scaledh < 1) scaledh = 1;

  int offsetx = (dstw - scaledw) / 2;
  int offsety = (dsth - scaledh) / 2;

  for(int y = 0; y < scaledh; y++)
    {
      int srcy = (int)(((float)y / (float)scaledh) * (float)srch);
      if(srcy >= srch) srcy = srch - 1;

      XBYTE* dstrow = dst + (((size_t)(y + offsety) * (size_t)dstw) + (size_t)offsetx) * 4;
      XBYTE* srcrowbase = src + ((size_t)srcy * (size_t)srcw * 4);

      for(int x = 0; x < scaledw; x++)
        {
          int srcx = (int)(((float)x / (float)scaledw) * (float)srcw);
          if(srcx >= srcw) srcx = srcw - 1;

          memcpy(dstrow + ((size_t)x * 4), srcrowbase + ((size_t)srcx * 4), 4);
        }
    }

  return dst;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPLINUXSCREENWAYLAND::CreateSHMBuffer(int width, int height, XBYTE* buffer, struct wl_buffer** outbuffer, void** outshmdata)
* @brief      Creates a one-shot wl_shm_pool-backed wl_buffer sized width*height*4 (32bpp BGRA,
*             matching GEN's GRPPROPERTYMODE_32_BGRA_8888), copies 'buffer' into its shared
*             memory, and returns the wl_buffer ready for wl_surface_attach(). Equivalent role to
*             GRPLINUXSCREENX11's XPutImage() path, except Wayland requires an explicit
*             POSIX-shm-backed file descriptor (via memfd_create) instead of Xlib handling pixel
*             transfer through the existing socket connection. The underlying SHM memory (fd +
*             mmap) IS safe to close/unmap right after wl_shm_pool_create_buffer() returns -- the
*             compositor received its own independent fd over the wire and keeps the pages alive
*             on its side regardless of what this process does with its own fd/mapping. The
*             wl_buffer PROTOCOL OBJECT returned here is a different story: it is NOT destroyed by
*             this function, and must not be destroyed synchronously by the caller either, because
*             the compositor may still be reading from it (e.g. uploading to a GPU texture) well
*             after wl_surface_commit() returns. A wl_buffer_listener IS installed (Buffer_Release,
*             below) so the buffer destroys itself once the compositor signals, via the
*             wl_buffer::release event, that it is done with this buffer -- the correct point to
*             free a one-shot wl_buffer, matching how a compositor expects clients to manage them.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  width : Buffer width in pixels.
* @param[in]  height : Buffer height in pixels.
* @param[in]  buffer : Source pixel data, width*height 32bpp BGRA pixels.
* @param[out] outbuffer : Receives the created wl_buffer (ready for wl_surface_attach()).
* @param[out] outshmdata : Receives the mmap()'d shared-memory pointer (already copied from
*                           and no longer needed by the caller, kept only for symmetry/future use).
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENWAYLAND::CreateSHMBuffer(int width, int height, XBYTE* buffer, struct wl_buffer** outbuffer, void** outshmdata)
{
  if(!shm)        return false;
  if(!buffer)     return false;
  if(!outbuffer)  return false;

  int stride = width * 4;
  int size   = stride * height;

  if(size <= 0) return false;

  char shmname[64];
  snprintf(shmname, sizeof(shmname), "/gen-wayland-%d-%p", (int)getpid(), (void*)this);

  int fd = shm_open(shmname, O_RDWR | O_CREAT | O_EXCL, 0600);
  if(fd < 0) return false;

  shm_unlink(shmname);   // unlink immediately -- the fd itself keeps the memory alive until closed

  if(ftruncate(fd, size) < 0)
    {
      close(fd);
      return false;
    }

  void* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(data == MAP_FAILED)
    {
      close(fd);
      return false;
    }

  memcpy(data, buffer, (size_t)size);

  struct wl_shm_pool* pool = wl_shm_create_pool(shm, fd, size);
  close(fd);   // the pool keeps its own reference; the fd is no longer needed after this call

  if(!pool)
    {
      munmap(data, size);
      return false;
    }

  struct wl_buffer* wlbuffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);

  wl_shm_pool_destroy(pool);

  munmap(data, size);

  if(!wlbuffer) return false;

  // See Buffer_Release() below: this is what actually frees 'wlbuffer' once the compositor is
  // done with it. Without this, every software-path Update() call leaked one wl_buffer proxy.
  wl_buffer_add_listener(wlbuffer, &grplinuxscreenwayland_buffer_listener, NULL);

  *outbuffer = wlbuffer;
  if(outshmdata) *outshmdata = NULL;   // already unmapped above -- nothing left for the caller to touch

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Buffer_Release(void* data, struct wl_buffer* buffer)
* @brief      wl_buffer::release listener -- the compositor is done reading from this one-shot SHM
*             buffer (see CreateSHMBuffer()), so it is now safe to destroy the protocol object.
*             'data' is unused (NULL, see wl_buffer_add_listener() call site) since each buffer is
*             self-contained and needs no back-reference to the screen that created it.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : Unused.
* @param[in]  buffer : Buffer object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Buffer_Release(void* data, struct wl_buffer* buffer)
{
  if(buffer)
    {
      wl_buffer_destroy(buffer);
    }
}


#ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Decoration_Configure(void* data, struct zxdg_toplevel_decoration_v1* decoration, XDWORD mode)
* @brief      zxdg_toplevel_decoration_v1::configure listener -- the compositor's answer to the
*             mode Create_Surface() requested via zxdg_toplevel_decoration_v1_set_mode(). The
*             compositor is free to grant a different mode than requested (e.g. a compositor that
*             only ever does client-side decorations may still create the decoration object just
*             to immediately tell the client "you get CLIENT_SIDE regardless of what you asked
*             for"), so this is purely informational logging, not something GEN reacts to: there
*             is no finer-grained fallback to attempt here the way GRPLINUXBLITGLESWAYLAND falls
*             back from EGL to Wayland SHM, since "draw your own decorations" (GEN's Custom
*             Chromes mode) is a design-time application choice, not something to switch to live.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXSCREENWAYLAND instance (listener user data).
* @param[in]  decoration : Decoration object to use.
* @param[in]  mode : ZXDG_TOPLEVEL_DECORATION_V1_MODE_* value granted by the compositor.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Decoration_Configure(void* data, struct zxdg_toplevel_decoration_v1* decoration, XDWORD mode)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Screen Wayland] xdg-decoration: compositor granted mode %s"), (mode == ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE) ? _L("SERVER_SIDE (native)") : _L("CLIENT_SIDE (app must draw its own)"));
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXSCREENWAYLAND::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENWAYLAND::Clean()
{
  display            = NULL;
  registry            = NULL;
  compositor          = NULL;
  shm                 = NULL;
  xdgwmbase           = NULL;
  seat                = NULL;

  surface             = NULL;
  xdgsurface          = NULL;
  xdgtoplevel         = NULL;

  #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
  decorationmanager   = NULL;
  toplleveldecoration = NULL;
  #endif

  pointer             = NULL;
  keyboard            = NULL;

  xkbcontext          = NULL;
  xkbkeymap           = NULL;
  xkbstate            = NULL;

  lastpointerserial   = 0;
  lastkeyboardserial  = 0;

  configured          = false;
  haskeyboardfocus    = false;
  haspointerfocus     = false;

  windowwidth         = 0;
  windowheight        = 0;

  pointerx            = 0;
  pointery            = 0;
  scrolldelta         = 0;

  memset(buttonqueue, 0, sizeof(buttonqueue));
  buttonqueuehead     = 0;
  buttonqueuetail     = 0;

  memset(keyqueue, 0, sizeof(keyqueue));
  keyqueuehead        = 0;
  keyqueuetail        = 0;

  isdesktop           = false;

  // Each entry here is a wl_output* this class itself bound via wl_registry_bind() in
  // Registry_Global() -- like any other bound proxy (compositor, shm, seat...) that is this
  // class's own client-side object, not something libwayland or the registry destroys on our
  // behalf. wl_output_destroy() must be called explicitly for each one before the backing array
  // is freed; DeleteAll() alone only releases the XVECTOR's own storage, not what it points to.
  for(XDWORD c=0; c<outputs.GetSize(); c++)
    {
      struct wl_output* output = outputs.Get(c);
      if(output)
        {
          wl_output_destroy(output);
        }
    }

  outputs.DeleteAll();   // frees only the vector's own backing array (the wl_output* pointers
                          // themselves were just destroyed above) -- DeleteContents() must still
                          // never be used here, it would call C++ delete on a libwayland C proxy.

  #ifdef GRP_OPENGL_ACTIVE
  blitgles            = NULL;
  #endif
}


#endif
