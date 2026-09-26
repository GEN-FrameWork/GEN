/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXBlitGLES.cpp
*
* @class      GRPLINUXBLITGLES
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

#include "GRPLINUXBlitGLES.h"

#include "GRPLINUXScreenX11.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXBLITGLES::GRPLINUXBLITGLES() : GRPBLITGLES()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLES::GRPLINUXBLITGLES()  : GRPBLITGLES() { }
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXBLITGLES::~GRPLINUXBLITGLES()
* @brief      Destructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXBLITGLES::~GRPLINUXBLITGLES()                 { }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeDisplayType GRPLINUXBLITGLES::GetNativeDisplay()
* @brief      Get native display
* @ingroup    PLATFORM_LINUX
* 
* @return     EGLNativeDisplayType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeDisplayType GRPLINUXBLITGLES::GetNativeDisplay()
{
  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs) return (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
  Display* dpy = xs->GetDisplay();
  return dpy ? (EGLNativeDisplayType)dpy : (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeWindowType GRPLINUXBLITGLES::GetNativeWindow()
* @brief      Get native window
* @ingroup    PLATFORM_LINUX
* 
* @return     EGLNativeWindowType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeWindowType GRPLINUXBLITGLES::GetNativeWindow()
{
  GRPLINUXSCREENX11* xs = (GRPLINUXSCREENX11*)screen;
  if(!xs)            return (EGLNativeWindowType)0;
  Window* w = xs->GetWindow();
  return w ? (EGLNativeWindowType)(*w) : (EGLNativeWindowType)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXBLITGLES::GetNativeWindowSize(int& width, int& height)
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
bool GRPLINUXBLITGLES::GetNativeWindowSize(int& width, int& height)
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


#endif // GRP_OPENGL_ACTIVE && LINUX_X11_ACTIVE
