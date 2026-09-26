/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXBlitGLESX11.cpp
*
* @class      GRPLINUXBLITGLESX11
* @brief      Linux X11 specialisation. Provides Display* and Window to EGL.
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

#if defined(GRP_OPENGL_ACTIVE) && defined(LINUX_X11_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPLINUXBlitGLESX11.h"

#include "GRPLINUXScreenX11.h"
#include "GRPViewPort.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXBLITGLESX11::GRPLINUXBLITGLESX11() : GRPBLITGLES()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLESX11::GRPLINUXBLITGLESX11()  : GRPBLITGLES() { }
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXBLITGLESX11::~GRPLINUXBLITGLESX11()
* @brief      Destructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLESX11::~GRPLINUXBLITGLESX11()                 { }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeDisplayType GRPLINUXBLITGLESX11::GetNativeDisplay()
* @brief      Get native display
* @ingroup    PLATFORM_LINUX
* 
* @return     EGLNativeDisplayType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeDisplayType GRPLINUXBLITGLESX11::GetNativeDisplay()
{
  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs) return (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
  Display* dpy = xs->GetDisplay();
  return dpy ? (EGLNativeDisplayType)dpy : (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeWindowType GRPLINUXBLITGLESX11::GetNativeWindow()
* @brief      Get native window
* @ingroup    PLATFORM_LINUX
* 
* @return     EGLNativeWindowType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeWindowType GRPLINUXBLITGLESX11::GetNativeWindow()
{
  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs)            return (EGLNativeWindowType)0;
  Window* w = xs->GetWindow();
  return w ? (EGLNativeWindowType)(*w) : (EGLNativeWindowType)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXBLITGLESX11::GetNativeWindowSize(int& width, int& height)
* @brief      Report the CURRENT X11 window size. Used by the base blitter to detect a
*             resize (e.g. the user maximizing the window) and recreate the EGL surface
*             to match, the same way GRPANDROIDBLITGLES does for a rotation. Without this,
*             the base blitter falls back to trusting whatever eglQuerySurface() reports,
*             which on some Mesa/ANGLE-on-X11 configurations (WSLg included) keeps
*             returning the size the surface had at creation time instead of the live
*             window size.
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXBLITGLESX11::GetNativeWindowSize(int& width, int& height)
{
  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs) return false;

  Display* dpy = xs->GetDisplay();
  Window*  w   = xs->GetWindow();
  if(!dpy || !w) return false;

  XWindowAttributes attr;
  if(!XGetWindowAttributes(dpy, *w, &attr)) return false;

  width  = attr.width;
  height = attr.height;

  return (width > 0 && height > 0);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPLINUXBLITGLESX11::ComputePresentationScale(GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible)
* @brief      X11 presentation policy: NO bitmap rescaling. The canvas is drawn at its native pixel size
*             (1 texel = 1 pixel) regardless of the drawable surface size, so growing the window shows more
*             background around the (unscaled) canvas, and shrinking it crops the canvas -- it is never
*             stretched. Width and height are resolved independently, matching the same independence at the
*             window-resize level (see GRPLINUXSCREENX11::ApplyWMNormalHints).
* @note       Anchored to the window's TOP-LEFT corner, not centered: the canvas' own top-left corner always
*             stays pinned to the window's top-left corner. Growing the window only reveals more background
*             at the bottom/right; shrinking it only crops the canvas' bottom/right edges -- regardless of
*             which edge or corner the user actually dragged to resize.
*             The window's growth past the GRPVIEWPORT_ID_MAIN viewport's max size is capped elsewhere in
*             GRPLINUXSCREENX11: ApplyWMNormalHints() sets XSetWMNormalHints/PMaxSize for native-chromes
*             windows (honoured by compliant EWMH window managers for interactive border-drag/Maximize),
*             backed up by a WM-independent reactive snap-back in Update()/clamp in Resize() that does not
*             rely on the window manager honouring that hint -- so this method can still be handed a
*             surfacewidth/surfaceheight briefly larger than the max (e.g. mid-drag, one frame before the
*             snap-back applies) and must not assume otherwise. This method only needs to cover the "hide
*             below min" half: whenever the surface (i.e. the window's
*             client area) is smaller than that viewport's declared minimum, on either axis, the canvas is
*             not drawn at all (the surface is left cleared/blank) instead of being shrunk to fit.
* @ingroup    PLATFORM_LINUX
*
* @param[in]  surfacewidth : Width, in pixels, of the drawable surface (matches the native window's client area).
* @param[in]  surfaceheight : Height, in pixels, of the drawable surface.
* @param[out] scalex : Resulting NDC X scale to apply to the fullscreen quad (native size, never a stretch).
* @param[out] scaley : Resulting NDC Y scale to apply to the fullscreen quad (native size, never a stretch).
* @param[out] translatex : Resulting NDC X shift so the LEFT edge of the quad stays pinned to the window's left edge.
* @param[out] translatey : Resulting NDC Y shift so the TOP edge of the quad stays pinned to the window's top edge.
* @param[out] visible : true if the canvas should be drawn at all; false to leave the surface cleared (blank).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXBLITGLESX11::ComputePresentationScale(GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible)
{
  scalex     = 1.0f;
  scaley     = 1.0f;
  translatex = 0.0f;
  translatey = 0.0f;
  visible    = true;

  if(surfacewidth <= 0 || surfaceheight <= 0 || texw <= 0 || texh <= 0)
    {
      visible = false;
      return;
    }

  // Native 1:1 mapping: the quad's NDC half-extent (1.0) must cover exactly texw/texh pixels out of
  // a surfacewidth/surfaceheight-sized viewport. texw/texh are swapped for a 90/270 rotated
  // presentation, same convention as the base (shared) letterbox implementation.
  int effectivetexw = texw;
  int effectivetexh = texh;

  if(rotation == GRPSCREENROTATION_90_CLOCKWISE || rotation == GRPSCREENROTATION_90_ANTICLOCKWISE)
    {
      effectivetexw = texh;
      effectivetexh = texw;
    }

  scalex = (float)effectivetexw / (float)surfacewidth;
  scaley = (float)effectivetexh / (float)surfaceheight;

  // Top-left anchor: in NDC, x=-1 is the window's LEFT edge and y=+1 is its TOP edge (canonical
  // convention used throughout this pipeline -- see BuildQuad's UV mapping). After the scale above,
  // the quad spans [-scalex,+scalex] x [-scaley,+scaley], CENTERED at the origin. Shifting it by
  // (scalex-1, 1-scaley) moves its left edge to -1 and its top edge to +1 -- pinning that corner --
  // while the OPPOSITE (bottom-right) edge/corner is the one that now grows or gets clipped away as
  // the window is resized. This is a pure screen-space shift (see BuildModelMatrix), so it holds
  // regardless of any rotation/flip also baked into the matrix.
  translatex = scalex - 1.0f;
  translatey = 1.0f  - scaley;

  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs)
    {
      return;
    }

  GRPVIEWPORT* mainviewport = xs->GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return; // no viewport configured yet: no min/max restriction to apply
    }

  float minwidth  = mainviewport->GetMinWidth();
  float minheight = mainviewport->GetMinHeight();

  if((minwidth  > 0.0f && surfacewidth  < (GLsizei)minwidth) ||
     (minheight > 0.0f && surfaceheight < (GLsizei)minheight))
    {
      visible = false;
    }
}


#endif // GRP_OPENGL_ACTIVE && LINUX_X11_ACTIVE
