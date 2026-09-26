/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPLINUXBlitGLESX11.h
*
* @class      GRPLINUXBLITGLESX11
* @brief      Linux X11 specialisation of GRPBLITGLES (uses Mesa EGL).
*             Works on PC Linux and Raspberry Pi 4/5 with Bullseye+ (KMS V3D).
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

#if defined(GRP_OPENGL_ACTIVE) && defined(LINUX_X11_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <X11/X.h>
#include <X11/Xlib.h>

#include "GRPBlitGLES.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class GRPLINUXBLITGLESX11 : public GRPBLITGLES
{
  public:
                                          GRPLINUXBLITGLESX11               ();
    virtual                              ~GRPLINUXBLITGLESX11               ();

    EGLNativeDisplayType                  GetNativeDisplay                  ();
    EGLNativeWindowType                   GetNativeWindow                   ();

    bool                                   GetNativeWindowSize                (int& width, int& height);












    void                                   ComputePresentationScale          (GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible);
};


#endif // GRP_OPENGL_ACTIVE && LINUX_X11_ACTIVE
