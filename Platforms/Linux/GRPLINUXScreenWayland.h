/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXScreenWayland.h
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

#pragma once


#ifdef LINUX_WAYLAND_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include "xdg-shell-client-protocol.h"

#ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE
#include "xdg-decoration-unstable-v1-client-protocol.h"
#endif

#include "XVector.h"

#include "GRPScreen.h"


#ifdef GRP_OPENGL_ACTIVE
class GRPLINUXBLITGLESWAYLAND;
#endif


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// xdg-shell gives no equivalent of X11's synchronous XGetWindowAttributes/XQueryPointer: the
// compositor announces the size it wants via a "configure" event and the client must ack it.
// Until the first configure arrives, GEN falls back to this size (matches GRPLINUXSCREENX11's
// default window request when no explicit size was set yet).
#define GRPLINUXSCREENWAYLAND_DEFAULT_WIDTH     640
#define GRPLINUXSCREENWAYLAND_DEFAULT_HEIGHT    480

// A wl_proxy (wl_pointer, wl_keyboard...) can only have ONE listener attached at a time --
// calling wl_*_add_listener() twice on the same object silently replaces the first listener
// instead of chaining it. Since GRPLINUXSCREENWAYLAND is the one binding wl_pointer/wl_keyboard
// from the seat (so ShowCursor()/HasFocus() have something to call directly, the same way
// GRPLINUXSCREENX11 manipulates Display*/Window directly), it must ALSO be the sole listener
// owner for both -- INPLINUXDEVICEKEYBOARDWAYLAND/INPLINUXDEVICEMOUSEWAYLAND cannot install
// their own. Instead they drain these small fixed-size FIFOs every Update(), which this class
// fills from its own listener callbacks. Capacity is generous for anything a single frame could
// plausibly produce; PushXxxEvent() drops the oldest entry rather than the newest if it is ever
// exceeded, so a slow frame loses old (already stale) input rather than the most recent state.
#define GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE   64

typedef struct
{
  XDWORD   code;
  bool     pressed;

} GRPLINUXSCREENWAYLAND_BUTTONEVENT;

typedef struct
{
  XDWORD   keysym;
  bool     pressed;

} GRPLINUXSCREENWAYLAND_KEYEVENT;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;

class GRPLINUXSCREENWAYLAND : public GRPSCREEN
{
  public:
                                          GRPLINUXSCREENWAYLAND             ();
    virtual                              ~GRPLINUXSCREENWAYLAND             ();

    bool                                  Create                            (bool show);

    bool                                  Update                            (GRP2DCANVAS* canvas);
    bool                                  UpdateTransparent                 (GRP2DCANVAS* canvas);
    bool                                  Delete                            ();

    bool                                  Resize                            (int width, int height);

    bool                                  Set_Position                      (int x, int y);

    bool                                  Show                              (bool active);
    bool                                  ShowCursor                        (bool active);

    bool                                  Minimize                          (bool active);
    bool                                  Maximize                          (bool active);

    void*                                 GetHandle                         ();

    bool                                  IsDesktop                         ();
    void                                  SetIsDesktop                      (bool isdesktop);

    bool                                  HasFocus                          ();

    bool                                  DispatchEvents                    ();

    struct wl_display*                    GetWLDisplay                      ();
    struct wl_surface*                    GetWLSurface                      ();
    struct wl_seat*                       GetWLSeat                         ();
    struct wl_pointer*                    GetWLPointer                      ();
    struct wl_keyboard*                   GetWLKeyboard                     ();
    struct xdg_toplevel*                  GetXDGToplevel                    ();

    XDWORD                                GetLastPointerSerial              ();
    XDWORD                                GetLastKeyboardSerial             ();

    bool                                   PopButtonEvent                    (XDWORD& code, bool& pressed);
    bool                                   PopKeyEvent                       (XDWORD& keysym, bool& pressed);
    bool                                   GetPointerPosition                (int& x, int& y);
    int                                    GetAndResetScrollDelta            ();

    int                                    GetWindowWidth                    ();
    int                                    GetWindowHeight                   ();

    #ifdef GRP_OPENGL_ACTIVE
    GRPLINUXBLITGLESWAYLAND*              GetBlitGLES                       ();
    #endif

  protected:

    struct wl_display*                    display;
    struct wl_registry*                   registry;
    struct wl_compositor*                 compositor;
    struct wl_shm*                        shm;
    struct xdg_wm_base*                   xdgwmbase;
    struct wl_seat*                       seat;

    struct wl_surface*                    surface;
    struct xdg_surface*                   xdgsurface;
    struct xdg_toplevel*                  xdgtoplevel;

    #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE

    struct zxdg_decoration_manager_v1*    decorationmanager;

    struct zxdg_toplevel_decoration_v1*   toplleveldecoration;
    #endif

    struct wl_pointer*                    pointer;
    struct wl_keyboard*                   keyboard;

    struct xkb_context*                   xkbcontext;
    struct xkb_keymap*                    xkbkeymap;
    struct xkb_state*                     xkbstate;

    XDWORD                                lastpointerserial;
    XDWORD                                lastkeyboardserial;

    bool                                  configured;
    bool                                  haskeyboardfocus;
    bool                                  haspointerfocus;

    int                                   windowwidth;
    int                                   windowheight;

    int                                   pointerx;
    int                                   pointery;
    int                                   scrolldelta;

    GRPLINUXSCREENWAYLAND_BUTTONEVENT     buttonqueue[GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE];
    XDWORD                                buttonqueuehead;
    XDWORD                                buttonqueuetail;

    GRPLINUXSCREENWAYLAND_KEYEVENT        keyqueue[GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE];
    XDWORD                                keyqueuehead;
    XDWORD                                keyqueuetail;

    bool                                  isdesktop;

    XVECTOR<struct wl_output*>            outputs;

    #ifdef GRP_OPENGL_ACTIVE
    GRPLINUXBLITGLESWAYLAND*              blitgles;
    #endif

  private:

    bool                                  Create_Surface                    (bool show);

    bool                                  ConnectDisplay                    ();
    bool                                  BindGlobals                       ();

    void                                  ResolveViewportMax                (float& maxw, float& maxh);

    void                                  ClampToViewportMax                (int& w, int& h);

    void                                  ApplyMaxSizeHint                  ();

  public:

    static void                           Registry_Global                   (void* data, struct wl_registry* registry, XDWORD name, const char* interface, XDWORD version);
    static void                           Registry_GlobalRemove              (void* data, struct wl_registry* registry, XDWORD name);

    static void                           XDGWMBase_Ping                    (void* data, struct xdg_wm_base* xdgwmbase, XDWORD serial);

    static void                           XDGSurface_Configure              (void* data, struct xdg_surface* xdgsurface, XDWORD serial);

    static void                           XDGToplevel_Configure             (void* data, struct xdg_toplevel* xdgtoplevel, int width, int height, struct wl_array* states);
    static void                           XDGToplevel_Close                 (void* data, struct xdg_toplevel* xdgtoplevel);

    static void                           Seat_Capabilities                 (void* data, struct wl_seat* seat, XDWORD capabilities);
    static void                           Seat_Name                        (void* data, struct wl_seat* seat, const char* name);

    static void                           Pointer_Enter                     (void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy);
    static void                           Pointer_Leave                     (void* data, struct wl_pointer* pointer, XDWORD serial, struct wl_surface* surface);
    static void                           Pointer_Motion                    (void* data, struct wl_pointer* pointer, XDWORD time, wl_fixed_t sx, wl_fixed_t sy);
    static void                           Pointer_Button                    (void* data, struct wl_pointer* pointer, XDWORD serial, XDWORD time, XDWORD button, XDWORD state);
    static void                           Pointer_Axis                      (void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis, wl_fixed_t value);
    static void                           Pointer_Frame                     (void* data, struct wl_pointer* pointer);
    static void                           Pointer_AxisSource                (void* data, struct wl_pointer* pointer, XDWORD axissource);
    static void                           Pointer_AxisStop                  (void* data, struct wl_pointer* pointer, XDWORD time, XDWORD axis);
    static void                           Pointer_AxisDiscrete               (void* data, struct wl_pointer* pointer, XDWORD axis, int discrete);

    static void                           Keyboard_Keymap                   (void* data, struct wl_keyboard* keyboard, XDWORD format, int fd, XDWORD size);
    static void                           Keyboard_Enter                    (void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface, struct wl_array* keys);
    static void                           Keyboard_Leave                    (void* data, struct wl_keyboard* keyboard, XDWORD serial, struct wl_surface* surface);
    static void                           Keyboard_Key                      (void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD time, XDWORD key, XDWORD state);
    static void                           Keyboard_Modifiers                (void* data, struct wl_keyboard* keyboard, XDWORD serial, XDWORD modsdepressed, XDWORD modslatched, XDWORD modslocked, XDWORD group);
    static void                           Keyboard_RepeatInfo                (void* data, struct wl_keyboard* keyboard, int rate, int delay);





    static void                           Buffer_Release                     (void* data, struct wl_buffer* buffer);

    #ifdef LINUX_WAYLAND_XDGDECORATION_ACTIVE





    static void                           Decoration_Configure               (void* data, struct zxdg_toplevel_decoration_v1* decoration, XDWORD mode);
    #endif

  private:

    void                                  PushButtonEvent                   (XDWORD code, bool pressed);
    void                                  PushKeyEvent                      (XDWORD keysym, bool pressed);

    bool                                  CreateSHMBuffer                   (int width, int height, XBYTE* buffer, struct wl_buffer** outbuffer, void** outshmdata);

    XBYTE*                                ScaleBufferNearestLetterbox       (XBYTE* src, int srcw, int srch, int dstw, int dsth);

    void                                  Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif

