/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXBlitGLESWayland.h
*
* @class      GRPLINUXBLITGLESWAYLAND
* @brief      Linux Wayland specialisation of GRPBLITGLES (uses Mesa EGL via wl_egl_window).
*             Works on PC Linux and Raspberry Pi 4/5 with Bullseye+ (KMS V3D), same as
*             GRPLINUXBLITGLESX11's X11 path -- only the native display/window handles differ.
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

#if defined(GRP_OPENGL_ACTIVE) && defined(LINUX_WAYLAND_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <wayland-client.h>
#include <wayland-egl.h>

#include "GRPBlitGLES.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPLINUXBLITGLESWAYLAND : public GRPBLITGLES
{
  public:
                                          GRPLINUXBLITGLESWAYLAND           ();
    virtual                              ~GRPLINUXBLITGLESWAYLAND           ();

    EGLNativeDisplayType                  GetNativeDisplay                  ();
    EGLNativeWindowType                   GetNativeWindow                   ();

    bool                                   GetNativeWindowSize                (int& width, int& height);









    void                                   ComputePresentationScale          (GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible);

  private:









    struct wl_egl_window*                 eglwindow;
};


#endif // GRP_OPENGL_ACTIVE && LINUX_WAYLAND_ACTIVE
