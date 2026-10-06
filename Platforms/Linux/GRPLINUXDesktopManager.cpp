/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPLINUXDesktopManager.cpp
* 
* @class      GRPLINUXDESKTOPMANAGER
* @brief      LINUX Graphics Desktop Manager class
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPLINUXDesktopManager.h"

#include <stdio.h>
#include <string.h>

#ifdef LINUX_X11_ACTIVE
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif

#ifdef LINUX_WAYLAND_ACTIVE
#include <wayland-client.h>
#endif

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


#ifdef LINUX_WAYLAND_ACTIVE

// One-shot wl_output enumeration used only by GRPLINUXDESKTOPMONITORS::MonitorEnum() below.
// Unlike GRPLINUXSCREENWAYLAND (which keeps a long-lived wl_display connection and its own
// wl_output list for the lifetime of the window, since it needs to pick a target output for
// fullscreen requests), this desktop manager opens a throwaway connection, harvests geometry
// once via two blocking round-trips, and disconnects -- mirroring how MonitorEnum()'s existing
// X11 path already does its own XOpenDisplay()/XCloseDisplay() independent of any
// GRPLINUXSCREENX11 instance, rather than reusing one. There is no live-refresh here, same as
// the X11 path: a monitor hot-plugged after this runs is simply not picked up, matching the
// pre-existing (X11) behaviour rather than fixing an unrelated gap as a side effect of this port.

#define GRPLINUXDESKTOPMONITORS_WAYLAND_MAXOUTPUTS   16

typedef struct
{
  struct wl_output*    output;
  int                  x;
  int                  y;
  int                  width;
  int                  height;

} GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO;


typedef struct
{
  GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO   outputs[GRPLINUXDESKTOPMONITORS_WAYLAND_MAXOUTPUTS];
  int                                     noutputs;

} GRPLINUXDESKTOPMONITORS_WLENUMCTX;


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wloutput_geometry(void* data, struct wl_output* output, int x, int y, int physicalwidth, int physicalheight, int subpixel, const char* make, const char* model, int transform)
* @brief      wl_output::geometry listener -- gives this output's position in the compositor's
*             global coordinate space, the equivalent of XRRCrtcInfo::x/y in the X11 path.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO instance (listener user data).
* @param[in]  output : Output object to use.
* @param[in]  x : Output position X, in the compositor's global coordinate space.
* @param[in]  y : Output position Y, in the compositor's global coordinate space.
* @param[in]  physicalwidth : Physical output width, in millimetres (not used).
* @param[in]  physicalheight : Physical output height, in millimetres (not used).
* @param[in]  subpixel : Subpixel orientation (not used).
* @param[in]  make : Output manufacturer name (not used).
* @param[in]  model : Output model name (not used).
* @param[in]  transform : Output transform (not used).
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wloutput_geometry(void* data, struct wl_output* output, int x, int y, int physicalwidth, int physicalheight, int subpixel, const char* make, const char* model, int transform)
{
  GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO* info = (GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO*)data;
  if(!info) return;

  info->x = x;
  info->y = y;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wloutput_mode(void* data, struct wl_output* output, XDWORD flags, int width, int height, int refresh)
* @brief      wl_output::mode listener -- an output can advertise several supported modes; only
*             the one flagged WL_OUTPUT_MODE_CURRENT is the mode actually in use, the equivalent
*             of the single width/height XRRCrtcInfo already gives per CRTC in the X11 path.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO instance (listener user data).
* @param[in]  output : Output object to use.
* @param[in]  flags : WL_OUTPUT_MODE_* bitmask.
* @param[in]  width : Mode width, in pixels.
* @param[in]  height : Mode height, in pixels.
* @param[in]  refresh : Vertical refresh rate, in mHz (not used).
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wloutput_mode(void* data, struct wl_output* output, XDWORD flags, int width, int height, int refresh)
{
  GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO* info = (GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO*)data;
  if(!info) return;

  if(flags & WL_OUTPUT_MODE_CURRENT)
    {
      info->width  = width;
      info->height = height;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wloutput_done(void* data, struct wl_output* output)
* @brief      wl_output::done listener -- marks the end of the initial geometry/mode/scale burst
*             for this output. Not used: MonitorEnum() below reads the accumulated fields after a
*             fixed pair of round-trips instead of waiting on this event, matching the "blocking
*             only at setup" shape already used elsewhere in the Wayland port.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO instance (listener user data).
* @param[in]  output : Output object to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wloutput_done(void* data, struct wl_output* output)
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wloutput_scale(void* data, struct wl_output* output, int factor)
* @brief      wl_output::scale listener (HiDPI integer scale factor) -- not used; GEN has no
*             existing per-monitor DPI-scale consumer on the X11 path either (GRPLINUXSCREENX11
*             does not read Xrandr's DPI hints), so this preserves parity rather than adding new
*             scope. Required listener table entry regardless (a wl_proxy accepts no NULL entries).
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO instance (listener user data).
* @param[in]  output : Output object to use.
* @param[in]  factor : Integer scale factor.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wloutput_scale(void* data, struct wl_output* output, int factor)
{
}


static const struct wl_output_listener grplinuxdesktopmonitors_wloutput_listener = { grplinuxdesktopmonitors_wloutput_geometry, grplinuxdesktopmonitors_wloutput_mode, grplinuxdesktopmonitors_wloutput_done, grplinuxdesktopmonitors_wloutput_scale };


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wlregistry_global(void* data, struct wl_registry* registry, XDWORD name, const char* interface, XDWORD version)
* @brief      wl_registry::global listener -- binds every advertised wl_output and installs its
*             listener. Equivalent role to XRRGetScreenResources()'s output array in the X11 path,
*             just populated by push events instead of returned synchronously from one call.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLENUMCTX instance (listener user data).
* @param[in]  registry : Registry object to use.
* @param[in]  name : Global object numeric name.
* @param[in]  interface : Global object interface name (e.g. "wl_output").
* @param[in]  version : Highest protocol version this compositor supports for this interface.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wlregistry_global(void* data, struct wl_registry* registry, XDWORD name, const char* interface, XDWORD version)
{
  GRPLINUXDESKTOPMONITORS_WLENUMCTX* ctx = (GRPLINUXDESKTOPMONITORS_WLENUMCTX*)data;
  if(!ctx) return;

  if(!strcmp(interface, "wl_output"))
    {
      if(ctx->noutputs >= GRPLINUXDESKTOPMONITORS_WAYLAND_MAXOUTPUTS) return;

      GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO* info = &ctx->outputs[ctx->noutputs];

      XDWORD bindversion = (version < 2) ? version : 2;

      info->output = (struct wl_output*)wl_registry_bind(registry, name, &wl_output_interface, bindversion);
      info->x      = 0;
      info->y      = 0;
      info->width  = 0;
      info->height = 0;

      wl_output_add_listener(info->output, &grplinuxdesktopmonitors_wloutput_listener, info);

      ctx->noutputs++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void grplinuxdesktopmonitors_wlregistry_globalremove(void* data, struct wl_registry* registry, XDWORD name)
* @brief      wl_registry::global_remove listener -- not used, see the "no live-refresh" note
*             above the GRPLINUXDESKTOPMONITORS_WLENUMCTX typedef.
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @param[in]  data : GRPLINUXDESKTOPMONITORS_WLENUMCTX instance (listener user data).
* @param[in]  registry : Registry object to use.
* @param[in]  name : Global object numeric name.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void grplinuxdesktopmonitors_wlregistry_globalremove(void* data, struct wl_registry* registry, XDWORD name)
{
}


static const struct wl_registry_listener grplinuxdesktopmonitors_wlregistry_listener = { grplinuxdesktopmonitors_wlregistry_global, grplinuxdesktopmonitors_wlregistry_globalremove };

#endif



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXDESKTOPMONITORS::GRPLINUXDESKTOPMONITORS() : GRPDESKTOPMONITORS()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXDESKTOPMONITORS::GRPLINUXDESKTOPMONITORS() : GRPDESKTOPMONITORS()
{
  Clean();

  MonitorEnum();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXDESKTOPMONITORS::~GRPLINUXDESKTOPMONITORS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXDESKTOPMONITORS::~GRPLINUXDESKTOPMONITORS()
{
  monitorsrects.DeleteContents();
  monitorsrects.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXDESKTOPMONITORS::MonitorEnum()
* @brief      Monitor enum
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXDESKTOPMONITORS::MonitorEnum()
{
  bool status = false;

  #ifdef LINUX_X11_ACTIVE

  Display*              display;
  Window                root;
  XRRScreenResources*   screen_resources;
  XRROutputInfo*        output_info;
  XRRCrtcInfo*          crtc_info;
  int                   screen;
  
  //display = XOpenDisplay(":0.0");
  display = XOpenDisplay(NULL);
  if(display) 
    {   
      screen  = DefaultScreen(display);  
      root    = RootWindow(display, screen);

      screen_resources = XRRGetScreenResources(display, root);
      if (screen_resources == NULL) 
        {
          XCloseDisplay(display);
          return false;
        }
      status = true;

      GRPRECTINT* allmonitor = GetCombinedRect();
      if(!allmonitor)
        {
          return false;
        }

      for(int c=0; c<screen_resources->noutput; c++) 
        {
          output_info = XRRGetOutputInfo(display, screen_resources, screen_resources->outputs[c]);
          if(output_info == NULL || output_info->connection == RR_Disconnected) 
            {
              if(output_info)
                {
                  XRRFreeOutputInfo(output_info);
                }

              continue;
            }
      
          crtc_info = XRRGetCrtcInfo(display, screen_resources, output_info->crtc);
          if(crtc_info == NULL) 
            {
              XRRFreeOutputInfo(output_info);
              continue;
            }

          // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("%d) screen: X:%d,Y:%d  %dx%d"), c, crtc_info->x, crtc_info->y, crtc_info->width, crtc_info->height);

          GRPRECTINT* newmonitor = GEN_NEW GRPRECTINT();
          if(newmonitor)
            {
              newmonitor->x1  = crtc_info->x;
              newmonitor->x2  = crtc_info->x + crtc_info->width;
              newmonitor->y1  = crtc_info->y;
              newmonitor->y2  = crtc_info->y + crtc_info->height;

              GetMonitorsRects()->Add(newmonitor);

              GRPRECTINT* allmonitor = GetCombinedRect();
              if(!allmonitor)
                {
                  return false;
                }

              GRPRECTINT allmonitortempo =  UniteRectangles((*allmonitor), (*newmonitor));

              allmonitor->CopyFrom(&allmonitortempo);
            }

          XRRFreeCrtcInfo(crtc_info);
          XRRFreeOutputInfo(output_info);     
        }

      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Full Screen: X1:%d,Y1:%d  X2:%d,Y2:%d"), allmonitor->x1, allmonitor->y1, allmonitor->x2, allmonitor->y2);

      XRRFreeScreenResources(screen_resources);
      XCloseDisplay(display);

    }
  #elif defined(LINUX_WAYLAND_ACTIVE)

  struct wl_display* wldisplay = wl_display_connect(NULL);
  if(wldisplay)
    {
      GRPLINUXDESKTOPMONITORS_WLENUMCTX ctx;
      memset(&ctx, 0, sizeof(ctx));

      struct wl_registry* wlregistry = wl_display_get_registry(wldisplay);
      if(wlregistry)
        {
          wl_registry_add_listener(wlregistry, &grplinuxdesktopmonitors_wlregistry_listener, &ctx);

          // First round-trip: the registry announces every wl_output global that already exists
          // (grplinuxdesktopmonitors_wlregistry_global() binds each one and installs its listener).
          wl_display_roundtrip(wldisplay);

          // Second round-trip: lets each newly-bound wl_output actually deliver its
          // geometry/mode/done burst, which was only queued as a result of wl_output_add_listener()
          // above -- not yet dispatched after just the first round-trip.
          wl_display_roundtrip(wldisplay);

          GRPRECTINT* allmonitor = GetCombinedRect();
          if(!allmonitor)
            {
              wl_registry_destroy(wlregistry);
              wl_display_disconnect(wldisplay);
              return false;
            }

          for(int c=0; c<ctx.noutputs; c++)
            {
              GRPLINUXDESKTOPMONITORS_WLOUTPUTINFO* info = &ctx.outputs[c];

              if((info->width > 0) && (info->height > 0))
                {
                  GRPRECTINT* newmonitor = GEN_NEW GRPRECTINT();
                  if(newmonitor)
                    {
                      newmonitor->x1  = info->x;
                      newmonitor->x2  = info->x + info->width;
                      newmonitor->y1  = info->y;
                      newmonitor->y2  = info->y + info->height;

                      GetMonitorsRects()->Add(newmonitor);

                      GRPRECTINT allmonitortempo = UniteRectangles((*allmonitor), (*newmonitor));

                      allmonitor->CopyFrom(&allmonitortempo);

                      status = true;
                    }
                }

              if(info->output)
                {
                  wl_output_destroy(info->output);
                }
            }

          wl_registry_destroy(wlregistry);
        }

      wl_display_disconnect(wldisplay);
    }
  #endif

  /*
  if(!GetMonitorsRects()->GetSize())
    {    
      GRPRECTINT* newmonitor = GEN_NEW GRPRECTINT();
      if(newmonitor)
        {
          newmonitor->x1  = 0;
          newmonitor->x2  = 640;
          newmonitor->y1  = 0;
          newmonitor->y2  = 480;

          GetMonitorsRects()->Add(newmonitor);

          GRPRECTINT* allmonitor = GetCombinedRect();
          if(!allmonitor)
            {
              return false;
            }

          GRPRECTINT allmonitortempo =  UniteRectangles((*allmonitor), (*newmonitor));

          allmonitor->CopyFrom(&allmonitortempo);

          status = true;
        }
     }
  */

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXDESKTOPMONITORS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXDESKTOPMONITORS::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXDESKTOPMANAGER::GRPLINUXDESKTOPMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXDESKTOPMANAGER::GRPLINUXDESKTOPMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXDESKTOPMANAGER::~GRPLINUXDESKTOPMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXDESKTOPMANAGER::~GRPLINUXDESKTOPMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPDESKTOPMONITORS* GRPLINUXDESKTOPMANAGER::GetDesktopMonitors()
* @brief      Get desktop monitors
* @ingroup    PLATFORM_LINUX
* 
* @return     GRPDESKTOPMONITORS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPDESKTOPMONITORS* GRPLINUXDESKTOPMANAGER::GetDesktopMonitors()
{
  return (GRPDESKTOPMONITORS*)&desktopmonitors;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXDESKTOPMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXDESKTOPMANAGER::Clean()
{

}

