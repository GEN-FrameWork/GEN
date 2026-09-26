/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPEGLContext.h
*
* @class      GRPEGLCONTEXT
* @brief      EGL context manager. Cross-platform OpenGL ES 3.0 context creation/management.
* @ingroup    GRAPHIC
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

#ifdef GRP_OPENGL_ACTIVE

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "XBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPEGLCONTEXT_DEFAULT_RED_SIZE        8
#define GRPEGLCONTEXT_DEFAULT_GREEN_SIZE      8
#define GRPEGLCONTEXT_DEFAULT_BLUE_SIZE       8
#define GRPEGLCONTEXT_DEFAULT_ALPHA_SIZE      8
#define GRPEGLCONTEXT_DEFAULT_DEPTH_SIZE      0
#define GRPEGLCONTEXT_DEFAULT_STENCIL_SIZE    0

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPEGLCONTEXT
{
  public:
                                          GRPEGLCONTEXT                         ();
    virtual                              ~GRPEGLCONTEXT                         ();

    bool                                  ChooseConfig                          (EGLNativeDisplayType native_display, bool with_alpha, EGLint& out_native_visual_id);

    bool                                  Create                                (EGLNativeDisplayType native_display, EGLNativeWindowType native_window, bool with_alpha = true);

    bool                                  MakeCurrent                           ();
    bool                                  RecreateSurface                       (EGLNativeWindowType native_window);
    bool                                  Release                               ();
    bool                                  SwapBuffers                           ();
    bool                                  SetSwapInterval                       (int interval);
    bool                                  Destroy                               ();

    bool                                  IsValid                               ();
    EGLint                                GetClientVersion                      ();
    bool                                  HasExtension                          (const char* name);

    EGLDisplay                            GetEGLDisplay                         ();
    EGLContext                            GetEGLContext                         ();
    EGLSurface                            GetEGLSurface                         ();
    EGLConfig                             GetEGLConfig                          ();

  private:

    EGLDisplay                            display;
    EGLContext                            context;
    EGLSurface                            surface;
    EGLConfig                             config;
    EGLint                                client_version;
    bool                                  isvalid;

    void                                  Clean                                 ();
};


#endif // GRP_OPENGL_ACTIVE
