/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPEGLContext.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

#ifdef GRP_OPENGL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPEGLContext.h"

#include <string.h>
#include <stdint.h>

#include "XTrace.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- LOCAL HELPERS -------------------------------------------------------------------------------------------------*/


// Fallback definitions for ANGLE-specific EGL platform tokens.
// These come from the EGL_ANGLE_platform_angle extension. If the system EGL
// headers don't declare them (older Mesa headers, vanilla Khronos headers),
// we provide the canonical values from the ANGLE source.
#ifndef EGL_PLATFORM_ANGLE_ANGLE
#define EGL_PLATFORM_ANGLE_ANGLE                   0x3202
#endif
#ifndef EGL_PLATFORM_ANGLE_TYPE_ANGLE
#define EGL_PLATFORM_ANGLE_TYPE_ANGLE              0x3203
#endif
#ifndef EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE
#define EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE      0x3206
#endif
#ifndef EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE
#define EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE       0x320D
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static const XCHAR* EGLErrorToString(EGLint err)
* @brief      Translates an EGL error code to a human-readable string for logging.
* @ingroup    GRAPHIC
* 
* @param[in]  err : Err value.
* 
* @return     static const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static const XCHAR* EGLErrorToString(EGLint err)
{
  switch(err)
    {
      case EGL_SUCCESS              : return __L("SUCCESS");
      case EGL_NOT_INITIALIZED      : return __L("NOT_INITIALIZED");
      case EGL_BAD_ACCESS           : return __L("BAD_ACCESS");
      case EGL_BAD_ALLOC            : return __L("BAD_ALLOC");
      case EGL_BAD_ATTRIBUTE        : return __L("BAD_ATTRIBUTE");
      case EGL_BAD_CONFIG           : return __L("BAD_CONFIG");
      case EGL_BAD_CONTEXT          : return __L("BAD_CONTEXT");
      case EGL_BAD_CURRENT_SURFACE  : return __L("BAD_CURRENT_SURFACE");
      case EGL_BAD_DISPLAY          : return __L("BAD_DISPLAY");
      case EGL_BAD_MATCH            : return __L("BAD_MATCH");
      case EGL_BAD_NATIVE_PIXMAP    : return __L("BAD_NATIVE_PIXMAP");
      case EGL_BAD_NATIVE_WINDOW    : return __L("BAD_NATIVE_WINDOW");
      case EGL_BAD_PARAMETER        : return __L("BAD_PARAMETER");
      case EGL_BAD_SURFACE          : return __L("BAD_SURFACE");
      case EGL_CONTEXT_LOST         : return __L("CONTEXT_LOST");
      default                       : return __L("UNKNOWN");
    }
}


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPEGLCONTEXT::GRPEGLCONTEXT()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPEGLCONTEXT::GRPEGLCONTEXT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPEGLCONTEXT::~GRPEGLCONTEXT()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPEGLCONTEXT::~GRPEGLCONTEXT()
{
  Destroy();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::ChooseConfig(EGLNativeDisplayType native_display, bool with_alpha, EGLint& out_native_visual_id)
* @brief      Open EGL display, pick a config and return the associated native visual id. Used by Linux X11 to create the window with the matching visual BEFORE creating the surface.
* @ingroup    GRAPHIC
* 
* @param[in]  native_display : platform native display handle
* @param[in]  with_alpha : request 8-bit alpha channel
* @param[in]  out_native_visual_id : EGL_NATIVE_VISUAL_ID of the chosen config
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::ChooseConfig(EGLNativeDisplayType native_display, bool with_alpha, EGLint& out_native_visual_id)
{
  display = EGL_NO_DISPLAY;

  // ------------------------------------------------------------------------
  // Step 0: log EGL client-side extensions. This tells us which native
  // platforms (X11, Wayland, surfaceless, gbm, ...) the runtime can handle.
  // ------------------------------------------------------------------------
  {
    const char* client_exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    if(client_exts && *client_exts)
      {
        XSTRING xs; xs = client_exts;
        XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] Client extensions: %s"), xs.Get());
      }
     else
      {
        XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] EGL_EXT_client_extensions not advertised (legacy EGL)"));
      }
  }

  // ------------------------------------------------------------------------
  // Step 1: try each display-open method in cascade. Reset EGL error state
  // before every attempt and log the per-attempt result so the failing
  // method is clearly identifiable.
  // ------------------------------------------------------------------------

  EGLint err = EGL_SUCCESS;

#if defined(__ANDROID__)

  // ------------------------------------------------------------------------
  // Android: the ONLY valid display is EGL_DEFAULT_DISPLAY. There is no X11
  // or ANGLE platform here — calling eglGetPlatformDisplay(X11) returns
  // EGL_BAD_PARAMETER and leaves the display invalid (then eglInitialize
  // fails with EGL_BAD_DISPLAY). So we go straight to legacy eglGetDisplay.
  // ------------------------------------------------------------------------
  (void)native_display;
  eglGetError();
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  err     = eglGetError();
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (Android) eglGetDisplay(EGL_DEFAULT_DISPLAY) -> %s (err=%s)"), (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));

#else

  // Attempt 1 - eglGetPlatformDisplay (EGL 1.5 core) with X11 platform. Gated behind
  // LINUX_X11_ACTIVE (not just the EGL header capability macros): EGL_PLATFORM_X11_KHR is
  // declared by any stock Mesa/EGL headers regardless of which GEN platform features this
  // build actually has active, so without this guard a Wayland-only build (LINUX_X11_ACTIVE
  // NOT defined) would still reach here and hand a wl_display* to eglGetPlatformDisplay while
  // claiming it is an X11 Display* -- Mesa's X11 platform backend dereferences it as such and
  // crashes inside its own xcb call chain instead of returning EGL_BAD_PARAMETER.
  #if defined(LINUX_X11_ACTIVE) && defined(EGL_VERSION_1_5) && defined(EGL_PLATFORM_X11_KHR)
  if(display == EGL_NO_DISPLAY)
    {
      typedef EGLDisplay (EGLAPIENTRYP PFN_GETPLATDISP)(EGLenum, void*, const EGLAttrib*);
      PFN_GETPLATDISP fn = (PFN_GETPLATDISP)eglGetProcAddress("eglGetPlatformDisplay");
      if(fn)
        {
          eglGetError();  // clear any previous error
          display = fn(EGL_PLATFORM_X11_KHR, (void*)native_display, NULL);
          err     = eglGetError();
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (1) eglGetPlatformDisplay(X11_KHR, native=0x%lx) -> %s (err=%s)"), (long long)(uintptr_t)native_display, (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (1) eglGetPlatformDisplay symbol not exported"));
        }
    }
  #endif

  // Attempt 1b - eglGetPlatformDisplay (EGL 1.5 core) with Wayland platform. Tightly guarded
  // behind LINUX_WAYLAND_ACTIVE (in addition to the EGL headers actually declaring the
  // Wayland platform tokens): this whole block does not exist in the compiled binary at all
  // unless Wayland support was actually built in, so it carries zero risk for X11/Windows/
  // Android builds. Symmetric with Attempt 1's LINUX_X11_ACTIVE guard above -- in a build with
  // both LINUX_X11_ACTIVE and LINUX_WAYLAND_ACTIVE defined, GRPLINUXFACTORY::CreateScreen()'s
  // compile-time priority (X11 first) means only GRPLINUXBLITGLESX11 ever calls ChooseConfig(),
  // so this attempt is simply skipped at runtime (display already resolved by Attempt 1); in a
  // Wayland-only build (LINUX_X11_ACTIVE not defined) Attempt 1 above is compiled out entirely,
  // so this is the first attempt made, and gets a real wl_display* as expected.
  #if defined(LINUX_WAYLAND_ACTIVE) && defined(EGL_VERSION_1_5) && defined(EGL_PLATFORM_WAYLAND_KHR)
  if(display == EGL_NO_DISPLAY)
    {
      typedef EGLDisplay (EGLAPIENTRYP PFN_GETPLATDISP)(EGLenum, void*, const EGLAttrib*);
      PFN_GETPLATDISP fn = (PFN_GETPLATDISP)eglGetProcAddress("eglGetPlatformDisplay");
      if(fn)
        {
          eglGetError();  // clear any previous error
          display = fn(EGL_PLATFORM_WAYLAND_KHR, (void*)native_display, NULL);
          err     = eglGetError();
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (1b) eglGetPlatformDisplay(WAYLAND_KHR, native=0x%lx) -> %s (err=%s)"), (long long)(uintptr_t)native_display, (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (1b) eglGetPlatformDisplay symbol not exported"));
        }
    }
  #endif

  // Attempt 2 - eglGetPlatformDisplayEXT with X11 platform. Same LINUX_X11_ACTIVE guard
  // rationale as Attempt 1 above -- EGL_EXT_platform_base/EGL_PLATFORM_X11_EXT are also just
  // header capability macros, not a signal that this build actually wants an X11 display.
  #if defined(LINUX_X11_ACTIVE) && defined(EGL_EXT_platform_base) && defined(EGL_PLATFORM_X11_EXT)
  if(display == EGL_NO_DISPLAY)
    {
      PFNEGLGETPLATFORMDISPLAYEXTPROC fn =
          (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
      if(fn)
        {
          eglGetError();
          display = fn(EGL_PLATFORM_X11_EXT, (void*)native_display, NULL);
          err     = eglGetError();
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (2) eglGetPlatformDisplayEXT(X11_EXT, native=0x%lx) -> %s (err=%s)"), (long long)(uintptr_t)native_display, (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (2) eglGetPlatformDisplayEXT symbol not exported"));
        }
    }
  #endif

  // Attempt 2b - eglGetPlatformDisplayEXT with Wayland platform. Same rationale/guard as
  // Attempt 1b above -- only compiled into a Wayland-enabled build, tried only if the EGL 1.5
  // core entry point from Attempt 1b was unavailable (older EGL without EGL_VERSION_1_5, using
  // the EXT_platform_base extension path instead, same fallback shape as Attempts 1/2 for X11).
  #if defined(LINUX_WAYLAND_ACTIVE) && defined(EGL_EXT_platform_base) && defined(EGL_PLATFORM_WAYLAND_EXT)
  if(display == EGL_NO_DISPLAY)
    {
      PFNEGLGETPLATFORMDISPLAYEXTPROC fn =
          (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
      if(fn)
        {
          eglGetError();
          display = fn(EGL_PLATFORM_WAYLAND_EXT, (void*)native_display, NULL);
          err     = eglGetError();
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (2c) eglGetPlatformDisplayEXT(WAYLAND_EXT, native=0x%lx) -> %s (err=%s)"), (long long)(uintptr_t)native_display, (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (2c) eglGetPlatformDisplayEXT symbol not exported"));
        }
    }
  #endif

  // Attempt 3 - legacy eglGetDisplay with the native display pointer (X11 Display* or
  // wl_display*, whichever platform is actually active -- eglGetDisplay's legacy signature is
  // platform-agnostic, it just wraps whatever native handle it is given)
  if(display == EGL_NO_DISPLAY)
    {
      eglGetError();
      display = eglGetDisplay(native_display);
      err     = eglGetError();
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (3) eglGetDisplay(native=0x%lx) -> %s (err=%s)"), (long long)(uintptr_t)native_display, (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
    }

  // Attempt 4 - last resort, EGL_DEFAULT_DISPLAY
  if(display == EGL_NO_DISPLAY)
    {
      eglGetError();
      display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
      err     = eglGetError();
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (4) eglGetDisplay(EGL_DEFAULT_DISPLAY) -> %s (err=%s)"), (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
    }

  // Attempt 5 - ANGLE platform via EGL_EXT_platform_base.
  // Activated when the runtime EGL is actually ANGLE (detected by client
  // extensions or simply tried as last fallback). ANGLE on Linux ignores
  // EGL_PLATFORM_X11_* and requires its own platform enum.
  #if defined(EGL_EXT_platform_base)
  if(display == EGL_NO_DISPLAY)
    {
      PFNEGLGETPLATFORMDISPLAYEXTPROC fn =
          (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
      if(fn)
        {
          // Ask ANGLE to pick the best backend (OpenGL on Linux, D3D11 on Windows).
          const EGLint angle_attribs[] = {
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE,
            EGL_NONE
          };
          eglGetError();
          display = fn((EGLenum)EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, angle_attribs);
          err     = eglGetError();
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (5) eglGetPlatformDisplayEXT(ANGLE_ANGLE, type=DEFAULT) -> %s (err=%s)"), (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));

          // If DEFAULT didn't work, ask explicitly for the OpenGL backend.
          if(display == EGL_NO_DISPLAY)
            {
              const EGLint angle_attribs_gl[] = {
                EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE,
                EGL_NONE
              };
              eglGetError();
              display = fn((EGLenum)EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, angle_attribs_gl);
              err     = eglGetError();
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] (5b) eglGetPlatformDisplayEXT(ANGLE_ANGLE, type=OPENGL) -> %s (err=%s)"), (display != EGL_NO_DISPLAY) ? __L("OK") : __L("NO_DISPLAY"), EGLErrorToString(err));
            }
        }
    }
  #endif

#endif // __ANDROID__

  if(display == EGL_NO_DISPLAY)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] All display-open methods failed."));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]   Check the client-extensions log above:"));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]   - If 'EGL_KHR_platform_x11' is NOT listed, EGL has no X11 backend."));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]   - If 'EGL_ANGLE_platform_angle' IS listed, your Linux build is using"));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]     ANGLE instead of Mesa. Recommended: link Linux against system Mesa"));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]     (apt install libegl1 libgles2 libegl-mesa0 libgl1-mesa-dri)."));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]   - On real Linux desktop / Raspberry Pi this normally works out of the box."));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL]   - Workaround on WSL: build without GRP_OPENGL_FEATURE (the GDI/X11 path keeps working)."));
      return false;
    }

  EGLint major = 0, minor = 0;
  if(eglInitialize(display, &major, &minor) != EGL_TRUE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglInitialize failed: %s"), EGLErrorToString(eglGetError()));
      display = EGL_NO_DISPLAY;
      return false;
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] Initialized version %d.%d"), major, minor);

  const EGLint config_attribs[] = {
    EGL_RED_SIZE,         GRPEGLCONTEXT_DEFAULT_RED_SIZE,
    EGL_GREEN_SIZE,       GRPEGLCONTEXT_DEFAULT_GREEN_SIZE,
    EGL_BLUE_SIZE,        GRPEGLCONTEXT_DEFAULT_BLUE_SIZE,
    EGL_ALPHA_SIZE,       with_alpha ? GRPEGLCONTEXT_DEFAULT_ALPHA_SIZE : 0,
    EGL_DEPTH_SIZE,       GRPEGLCONTEXT_DEFAULT_DEPTH_SIZE,
    EGL_STENCIL_SIZE,     GRPEGLCONTEXT_DEFAULT_STENCIL_SIZE,
    EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES3_BIT,
    EGL_SURFACE_TYPE,     EGL_WINDOW_BIT,
    EGL_NONE
  };

  EGLint nconfig = 0;
  if(eglChooseConfig(display, config_attribs, &config, 1, &nconfig) != EGL_TRUE || nconfig <= 0)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglChooseConfig (ES3) failed: %s"), EGLErrorToString(eglGetError()));
      return false;
    }

  if(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &out_native_visual_id) != EGL_TRUE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglGetConfigAttrib EGL_NATIVE_VISUAL_ID failed: %s"), EGLErrorToString(eglGetError()));
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::Create(EGLNativeDisplayType native_display, EGLNativeWindowType native_window, bool with_alpha)
* @brief      Create surface + context. If ChooseConfig has not been called yet, it is called here.
* @ingroup    GRAPHIC
* 
* @param[in]  native_display : Native display handle.
* @param[in]  native_window : Native window value.
* @param[in]  with_alpha : With alpha value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::Create(EGLNativeDisplayType native_display, EGLNativeWindowType native_window, bool with_alpha)
{
  if(display == EGL_NO_DISPLAY)
    {
      EGLint dummy_visid = 0;
      if(!ChooseConfig(native_display, with_alpha, dummy_visid)) return false;
    }

  if(eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglBindAPI(EGL_OPENGL_ES_API) failed: %s"), EGLErrorToString(eglGetError()));
      return false;
    }

  surface = eglCreateWindowSurface(display, config, native_window, NULL);
  if(surface == EGL_NO_SURFACE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglCreateWindowSurface failed: %s"), EGLErrorToString(eglGetError()));
      return false;
    }

  const EGLint context_attribs_es3[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs_es3);
  if(context == EGL_NO_CONTEXT)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] eglCreateContext(ES3) failed (%s); trying ES2 fallback"), EGLErrorToString(eglGetError()));

      // Fallback to ES2 (should rarely happen on the target platforms but keep it as a safety net)
      const EGLint context_attribs_es2[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
      context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs_es2);
      if(context == EGL_NO_CONTEXT)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] eglCreateContext(ES2) fallback failed: %s"), EGLErrorToString(eglGetError()));
          eglDestroySurface(display, surface);
          surface = EGL_NO_SURFACE;
          return false;
        }
      client_version = 2;
    }
   else
    {
      client_version = 3;
    }

  if(!MakeCurrent()) return false;

  isvalid = true;
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[EGL] Context created (ES %d.0)"), client_version);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::RecreateSurface(EGLNativeWindowType native_window)
* @brief      Destroy the current window surface and create a new one on the (possibly resized) native window, then make it current again. Used on Android when the device rotates and the ANativeWindow changes size: the old surface keeps its creation-time size, so it must be recreated to match the new window dimensions.
* @ingroup    GRAPHIC
* 
* @param[in]  native_window : Native window value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::RecreateSurface(EGLNativeWindowType native_window)
{
  if(display == EGL_NO_DISPLAY || context == EGL_NO_CONTEXT) return false;

  // Unbind before destroying the old surface.
  eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

  if(surface != EGL_NO_SURFACE)
    {
      eglDestroySurface(display, surface);
      surface = EGL_NO_SURFACE;
    }

  surface = eglCreateWindowSurface(display, config, native_window, NULL);
  if(surface == EGL_NO_SURFACE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] RecreateSurface: eglCreateWindowSurface failed: %s"), EGLErrorToString(eglGetError()));
      isvalid = false;
      return false;
    }

  if(eglMakeCurrent(display, surface, surface, context) != EGL_TRUE)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[EGL] RecreateSurface: eglMakeCurrent failed: %s"), EGLErrorToString(eglGetError()));
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::MakeCurrent()
* @brief      Make Current
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::MakeCurrent()
{
  if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE || context == EGL_NO_CONTEXT) return false;
  if(eglGetCurrentContext() == context) return true;
  return (eglMakeCurrent(display, surface, surface, context) == EGL_TRUE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::Release()
* @brief      Release
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::Release()
{
  if(display == EGL_NO_DISPLAY) return false;
  return (eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::SwapBuffers()
* @brief      Swap Buffers
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::SwapBuffers()
{
  if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) return false;
  return (eglSwapBuffers(display, surface) == EGL_TRUE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::SetSwapInterval(int interval)
* @brief      Set swap interval
* @ingroup    GRAPHIC
* 
* @param[in]  interval : 0=no vsync, 1=vsync, n=every n refreshes
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::SetSwapInterval(int interval)
{
  if(display == EGL_NO_DISPLAY) return false;
  return (eglSwapInterval(display, (EGLint)interval) == EGL_TRUE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::Destroy()
* @brief      Destroy resource
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::Destroy()
{
  if(display == EGL_NO_DISPLAY) return false;

  eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

  if(context != EGL_NO_CONTEXT) { eglDestroyContext(display, context); context = EGL_NO_CONTEXT; }
  if(surface != EGL_NO_SURFACE) { eglDestroySurface(display, surface); surface = EGL_NO_SURFACE; }

  eglTerminate(display);
  display = EGL_NO_DISPLAY;

  isvalid = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::IsValid()
* @brief      Check if valid
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::IsValid()
{
  return isvalid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLint GRPEGLCONTEXT::GetClientVersion()
* @brief      Get client version
* @ingroup    GRAPHIC
* 
* @return     EGLint : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLint GRPEGLCONTEXT::GetClientVersion()
{
  return client_version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPEGLCONTEXT::HasExtension(const char* name)
* @brief      Check if an EGL extension is supported. Note: for GL extensions use glGetString(GL_EXTENSIONS) from the blitter.
* @ingroup    GRAPHIC
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPEGLCONTEXT::HasExtension(const char* name)
{
  if(display == EGL_NO_DISPLAY || !name) return false;
  const char* exts = eglQueryString(display, EGL_EXTENSIONS);
  if(!exts) return false;
  return (strstr(exts, name) != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLDisplay GRPEGLCONTEXT::GetEGLDisplay()
* @brief      Get egl display
* @ingroup    GRAPHIC
* 
* @return     EGLDisplay : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLDisplay GRPEGLCONTEXT::GetEGLDisplay() { return display; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLContext GRPEGLCONTEXT::GetEGLContext()
* @brief      Get egl context
* @ingroup    GRAPHIC
* 
* @return     EGLContext : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLContext GRPEGLCONTEXT::GetEGLContext() { return context; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLSurface GRPEGLCONTEXT::GetEGLSurface()
* @brief      Get egl surface
* @ingroup    GRAPHIC
* 
* @return     EGLSurface : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLSurface GRPEGLCONTEXT::GetEGLSurface() { return surface; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLConfig GRPEGLCONTEXT::GetEGLConfig()
* @brief      Get egl config
* @ingroup    GRAPHIC
* 
* @return     EGLConfig : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLConfig  GRPEGLCONTEXT::GetEGLConfig()  { return config;  }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPEGLCONTEXT::Clean()
* @brief      Clean internal state
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPEGLCONTEXT::Clean()
{
  display         = EGL_NO_DISPLAY;
  context         = EGL_NO_CONTEXT;
  surface         = EGL_NO_SURFACE;
  config          = (EGLConfig)0;
  client_version  = 0;
  isvalid         = false;
}


#endif // GRP_OPENGL_ACTIVE
