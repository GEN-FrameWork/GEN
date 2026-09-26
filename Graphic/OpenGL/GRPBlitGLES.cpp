/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPBlitGLES.cpp
*
* @class      GRPBLITGLES
* @brief      OpenGL ES 3.0 buffer blitter. Cross-platform abstraction.
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

#include "GRPBlitGLES.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "XTrace.h"

#include "GRPEGLContext.h"
#include "GRPScreen.h"
#include "GRP2DCanvas.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- SHADER SOURCES (GLSL ES 3.0, layout-qualified) ----------------------------------------------------------------*/

static const char* k_vshader_src =
  "#version 300 es\n"
  "layout(location=0) in vec2 a_pos;\n"
  "layout(location=1) in vec2 a_uv;\n"
  "uniform mat4 u_modelview;\n"
  "out vec2 v_uv;\n"
  "void main(){\n"
  "  v_uv = a_uv;\n"
  "  gl_Position = u_modelview * vec4(a_pos, 0.0, 1.0);\n"
  "}\n";

static const char* k_fshader_bgra_src =
  "#version 300 es\n"
  "precision mediump float;\n"
  "in  vec2 v_uv;\n"
  "out vec4 outColor;\n"
  "uniform sampler2D u_tex;\n"
  "void main(){ outColor = texture(u_tex, v_uv); }\n";

static const char* k_fshader_swizzle_src =
  "#version 300 es\n"
  "precision mediump float;\n"
  "in  vec2 v_uv;\n"
  "out vec4 outColor;\n"
  "uniform sampler2D u_tex;\n"
  "void main(){ outColor = texture(u_tex, v_uv).bgra; }\n";


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBLITGLES::GRPBLITGLES()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBLITGLES::GRPBLITGLES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBLITGLES::~GRPBLITGLES()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBLITGLES::~GRPBLITGLES()
{
  Destroy();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::PreCreateHook()
* @brief      pre create hook
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::PreCreateHook()
{ 
  return true; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::PostCreateHook(EGLint visid)
* @brief      post create hook
* @ingroup    GRAPHIC
* 
* @param[in]  visid : Native visual identifier.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::PostCreateHook(EGLint visid)
{ 
  return true; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::ChooseVisualID(EGLNativeDisplayType native_display, EGLint& out_native_visual_id)
* @brief      choose visual Id
* @ingroup    GRAPHIC
* 
* @param[in]  native_display : Native display handle.
* @param[in]  out_native_visual_id : Output native visual identifier.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::ChooseVisualID(EGLNativeDisplayType native_display, EGLint& out_native_visual_id)
{
  if(!eglctx) 
    {
      eglctx = GEN_NEW GRPEGLCONTEXT();
    }

  if(!eglctx) 
    {
      return false;
    }

  return eglctx->ChooseConfig(native_display, usealpha, out_native_visual_id);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::Create(GRPSCREEN* screen)
* @brief      Create the EGL context + GL resources.
* @ingroup    GRAPHIC
* 
* @param[in]  screen : the screen this blitter is attached to (host of native handles).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::Create(GRPSCREEN* screen)
{
  if(!screen) 
    {
      return false;
    }

  this->screen = screen;

  if(!PreCreateHook()) 
    {
      return false;
    }

  if(!eglctx) 
    {
      eglctx = GEN_NEW GRPEGLCONTEXT();
    }

  if(!eglctx) 
    {
      return false;
    }

  EGLNativeDisplayType ndisp = GetNativeDisplay();
  EGLNativeWindowType  nwin  = GetNativeWindow();

  // Choose the EGL config BEFORE creating the surface. Android requires
  // ANativeWindow_setBuffersGeometry() (done in PostCreateHook) to run with the
  // EGL native visual id BEFORE the window surface is created — otherwise the
  // surface format may mismatch and present nothing (black). On Linux the config
  // was already chosen via ChooseVisualID (display != EGL_NO_DISPLAY) so we skip it.
  EGLint native_visid = 0;
  if(eglctx->GetEGLDisplay() == EGL_NO_DISPLAY)
    {
      if(!eglctx->ChooseConfig(ndisp, usealpha, native_visid))
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[BlitGLES] EGL ChooseConfig failed"));
          return false;
        }
    }

  // Platform hook BEFORE surface creation (Android sets the native window geometry here).
  if(!PostCreateHook(native_visid))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[BlitGLES] PostCreateHook failed"));
      return false;
    }

  // Create surface + context. ChooseConfig was already done above, so Create() skips it.
  if(!eglctx->Create(ndisp, nwin, usealpha))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[BlitGLES] EGL context creation failed"));
      return false;
    }

  if(usevsync) 
    {
      eglctx->SetSwapInterval(1);
    }
   else 
    {
      eglctx->SetSwapInterval(0);
    }

  // Cache GL caps (informational only — we always RGBA-store + shader-swizzle for portability)
  DetectBGRAExtension();
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] BGRA ext detected: %s; using RGBA storage + shader swizzle (portable)"), hasbgraext ? __L("YES") : __L("NO"));

  // Log the actual GL driver (hardware vs software). Crucial on WSLg to know
  // whether we're on the d3d12 GPU driver or the llvmpipe software rasterizer.
  {
    const char* gl_vendor   = (const char*)glGetString(GL_VENDOR);
    const char* gl_renderer = (const char*)glGetString(GL_RENDERER);
    const char* gl_version  = (const char*)glGetString(GL_VERSION);
    XSTRING xv, xr, xver;
    xv   = gl_vendor   ? gl_vendor   : "(null)";
    xr   = gl_renderer ? gl_renderer : "(null)";
    xver = gl_version  ? gl_version  : "(null)";
    XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] GL_VENDOR   : %s"), xv.Get());
    XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] GL_RENDERER : %s"), xr.Get());
    XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] GL_VERSION  : %s"), xver.Get());
  }

  // GL resources
  if(!CompileShaders()) 
    {
      return false;
    }

  if(!BuildQuad())      
    {
      return false;
    }

  // Texture
  if(screen->GetWidth() > 0 && screen->GetHeight() > 0)
    {
      if(!AllocTexture((int)screen->GetWidth(), (int)screen->GetHeight())) 
        {
          return false;
        }

      if(usepbo && IsES3())
        {
          if(!AllocPBOs((int)screen->GetWidth(), (int)screen->GetHeight())) 
            {
              return false;
            }
        }
    }

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  if(usealpha)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }

  glClearColor(0.0f, 0.0f, 0.0f, usealpha ? 0.0f : 1.0f);

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] Created (ES %d.0, PBO=%s, VSync=%s)"), (int)IsES3()?3:2, (usepbo && IsES3())?__L("ON"):__L("OFF"), usevsync?__L("ON"):__L("OFF"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPBLITGLES::PresentBlankFrame()
* @brief      Clears the surface to the configured clear colour (see Create()'s glClearColor -- opaque black
*             unless usealpha) and presents it, WITHOUT drawing the canvas texture/quad.
* @note       Create() sets up the EGL surface and the GL clear colour but never itself clears or swaps: the
*             surface's initial content is therefore whatever the driver happens to leave in a freshly
*             allocated buffer -- undefined, and on several drivers visibly so (a scatter of stray coloured
*             pixels, not a clean colour). Nothing is wrong with that BY ITSELF, since the normal Update(canvas)
*             path always clears before it draws -- except the platform Create_Window() that owns this blitter
*             maps the native window to the desktop BEFORE this object exists at all, and the very first
*             Update(canvas) call only happens once the application's main loop reaches its first draw tick.
*             Everything the OS/compositor paints in that gap -- which can span several frames-worth of wall
*             clock time -- shows this undefined content, which is exactly the stray-pixel "garbage strip" a
*             custom window chrome's caption (the one thing drawn at the very top, right where a resize/DPI
*             related discrepancy would be most visible) makes so obvious.
*             Calling this once, right after Create() succeeds and BEFORE the platform code that owns this
*             blitter makes the window visible, guarantees the surface holds a clean, known colour for every
*             frame the OS could possibly display before GEN's own first real Update(canvas). Called twice by
*             the caller (once per back buffer of the common double-buffered case) so neither buffer of the
*             swap chain is left holding undefined content.
* @ingroup    GRAPHIC
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::PresentBlankFrame()
{
  if(!eglctx) return false;

  if(!eglctx->MakeCurrent()) return false;

  int vpw = (screen && (screen->GetWidth()  > 0)) ? (int)screen->GetWidth()  : texw;
  int vph = (screen && (screen->GetHeight() > 0)) ? (int)screen->GetHeight() : texh;

  if(vpw <= 0) vpw = 1;
  if(vph <= 0) vph = 1;

  glViewport(0, 0, vpw, vph);
  glClear(GL_COLOR_BUFFER_BIT);

  return eglctx->SwapBuffers();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPBLITGLES::Resize(int width, int height)
* @brief      resize
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::Resize(int width, int height)
{
  if(width <= 0 || height <= 0) 
    {
      return false;
    }

  if(width == texw && height == texh) 
    {
      return true;
    }

  if(!eglctx || !eglctx->MakeCurrent()) 
    {
      return false;
    }

  if(!AllocTexture(width, height)) 
    {
      return false;
    }

  if(usepbo && IsES3())
    {
      if(!AllocPBOs(width, height)) 
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::Update(GRP2DCANVAS* canvas)
* @brief      update
* @ingroup    GRAPHIC
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::Update(GRP2DCANVAS* canvas)
{
  if(!canvas || !screen || !eglctx) 
    {
      return false;
    }

  // The screen is the source of truth for the presentation rotation (set via
  // GRPSCREEN::Rotate()/SetRotation(), possibly before this blitter even existed).
  // Pull it every frame: cheap, and robust to Rotate() being called at any time.
  rotation = screen->GetRotation();

  if(!eglctx->MakeCurrent())        
    {
      return false;
    }

  const int w = (int)canvas->GetWidth();
  const int h = (int)canvas->GetHeight();
  if(w <= 0 || h <= 0) 
    {
      return false;
    }

  if(w != texw || h != texh)
    {
      if(!Resize(w, h)) 
        {
          return false;
        }
    }

  // Diagnostic: emit every size that matters (canvas / screen / EGL surface /
  // texture). We log DIRECTLY through the Android logging API (not the framework
  // XTRACE, which does not reach logcat on Android), and we re-log whenever any
  // size changes so the post-rotation values are captured too.
  // Filter with:  adb logcat -s GEN_BLIT
  {
    EGLint sfw = 0, sfh = 0;
    if(eglctx && eglctx->GetEGLDisplay() != EGL_NO_DISPLAY && eglctx->GetEGLSurface() != EGL_NO_SURFACE)
      {
        eglQuerySurface(eglctx->GetEGLDisplay(), eglctx->GetEGLSurface(), EGL_WIDTH, &sfw);
        eglQuerySurface(eglctx->GetEGLDisplay(), eglctx->GetEGLSurface(), EGL_HEIGHT, &sfh);
      }

    const int scrw = (int)screen->GetWidth();
    const int scrh = (int)screen->GetHeight();

    static int last_cw = -1, last_ch = -1, last_sw = -1, last_sh = -1, last_fw = -1, last_fh = -1;
    if(w != last_cw || h != last_ch || scrw != last_sw || scrh != last_sh ||(int)sfw != last_fw || (int)sfh != last_fh)
      {
        last_cw = w; last_ch = h; last_sw = scrw; last_sh = scrh; last_fw = (int)sfw; last_fh = (int)sfh;
        
        #if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_INFO, "GEN_BLIT", "SIZES canvas=%dx%d screen=%dx%d surface=%dx%d texture=%dx%d", w, h, scrw, scrh, (int)sfw, (int)sfh, texw, texh);
        #endif

        XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] SIZES canvas=%dx%d screen=%dx%d surface=%dx%d texture=%dx%d"), w, h, scrw, scrh, (int)sfw, (int)sfh, texw, texh);
      }
  }

  // Canvas is BGRA in memory. We upload the raw bytes into an RGBA texture (no format
  // reinterpretation) and the shader swizzles .bgra at sample time. Using GL_BGRA_EXT
  // as the upload format here would require a BGRA-capable texture, which is not portable
  // on ES 3.0 (see AllocTexture). GL_RGBA upload of BGRA bytes + shader swizzle is universal.
  const GLenum upload_fmt = GL_RGBA;
  XBYTE*       src        = canvas->Buffer_Get();
  if(!src) 
    {
      return false;
    }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);

  if(usepbo && IsES3() && pbo[0] && pbo[1])
    {
      const int prev = pbo_index;
      const int next = pbo_index ^ 1;

      // Upload from the previous PBO into the texture (async DMA on most drivers)
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[prev]);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, upload_fmt, GL_UNSIGNED_BYTE, NULL);

      // Map the next PBO and write the new frame
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[next]);
      glBufferData(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr)pbo_size, NULL, GL_STREAM_DRAW); // orphan

      void* dst = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, (GLsizeiptr)pbo_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
      if(dst)
        {
          memcpy(dst, src, pbo_size);
          glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        }

      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
      pbo_index = next;
    }
   else
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, upload_fmt, GL_UNSIGNED_BYTE, src);
    }

  // --- Resolve the real drawable size ----------------------------------------------------------
  // On Android the device can rotate AFTER the EGL surface was created; the surface keeps its
  // creation-time size, so we must recreate it when the native window size changes. On Windows/
  // Linux GetNativeWindowSize() returns false and we simply trust the EGL surface size.
  GLsizei vpw = (GLsizei)screen->GetWidth();
  GLsizei vph = (GLsizei)screen->GetHeight();
  if(eglctx)
    {
      EGLDisplay edisp = eglctx->GetEGLDisplay();

      int nw = 0, nh = 0;
      if(GetNativeWindowSize(nw, nh) && nw > 0 && nh > 0)
        {
          EGLint sw0 = 0, sh0 = 0;
          EGLSurface es0 = eglctx->GetEGLSurface();
          if(edisp != EGL_NO_DISPLAY && es0 != EGL_NO_SURFACE)
            {
              eglQuerySurface(edisp, es0, EGL_WIDTH, &sw0);
              eglQuerySurface(edisp, es0, EGL_HEIGHT, &sh0);
            }
          if(nw != (int)sw0 || nh != (int)sh0)
            {
              // Window resized/rotated: rebuild the surface to match.
              eglctx->RecreateSurface(GetNativeWindow());
            }
        }

      EGLint sw = 0, sh = 0;
      EGLSurface esurf = eglctx->GetEGLSurface();
      if(edisp != EGL_NO_DISPLAY && esurf != EGL_NO_SURFACE)
        {
          if(eglQuerySurface(edisp, esurf, EGL_WIDTH, &sw) == EGL_TRUE &&
             eglQuerySurface(edisp, esurf, EGL_HEIGHT, &sh) == EGL_TRUE &&
             sw > 0 && sh > 0)
            {
              vpw = (GLsizei)sw;
              vph = (GLsizei)sh;
            }
        }
    }

  // --- Presentation scale: how the canvas texture maps onto the drawable surface ----------------
  // Default (shared) policy is letterbox/pillarbox (see ComputePresentationScale below); platform
  // specialisations may replace it (e.g. GRPWINDOWSBLITGLES presents the canvas at its native pixel
  // size, with no rescaling, and can also report the canvas as not-visible -- see that override).
  bool presentationvisible = true;

  ComputePresentationScale(vpw, vph, lboxsx, lboxsy, lboxtx, lboxty, presentationvisible);

  glViewport(0, 0, vpw, vph);
  glClear(GL_COLOR_BUFFER_BIT);

  if(presentationvisible)
    {
      glUseProgram(program);

      float mv[16];
      BuildModelMatrix(mv);
      glUniformMatrix4fv(u_modelview, 1, GL_FALSE, mv);
      glUniform1i(u_tex, 0);

      if(IsES3() && vao)
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
          glBindVertexArray(0);
        }
       else
        {
          glBindBuffer(GL_ARRAY_BUFFER, vbo);
          glEnableVertexAttribArray((GLuint)a_pos);
          glEnableVertexAttribArray((GLuint)a_uv);
          glVertexAttribPointer((GLuint)a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
          glVertexAttribPointer((GLuint)a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
          glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

  return eglctx->SwapBuffers();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPBLITGLES::ComputePresentationScale(GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible)
* @brief      Computes how the canvas texture maps onto the drawable surface (the fullscreen quad's NDC scale
*             AND position), and whether it should be drawn at all.
* @note       Default (shared) policy: letterbox/pillarbox, CENTERED. The canvas (texture) is a fixed design
*             size (e.g. 1024x768). The surface may have any aspect (e.g. 2712x1220). Scale the fullscreen
*             quad down on one axis so the canvas keeps its aspect, centered (translatex/translatey left at
*             0), with black bars on the remaining space (glClear paints them). This matches the previous
*             Windows/Linux look where the 1024x768 screen is shown 1:1 / scaled to the window -- i.e. this
*             DOES rescale the presented bitmap as the surface is resized. Platform specialisations can
*             override this method to implement a different presentation policy (scale AND/OR anchor)
*             without touching the shared Update() flow above.
* @ingroup    GRAPHIC
*
* @param[in]  surfacewidth : Width, in pixels, of the drawable surface (EGL surface / native window).
* @param[in]  surfaceheight : Height, in pixels, of the drawable surface.
* @param[out] scalex : Resulting NDC X scale to apply to the fullscreen quad.
* @param[out] scaley : Resulting NDC Y scale to apply to the fullscreen quad.
* @param[out] translatex : Resulting NDC X translation to apply to the fullscreen quad (0 = centered).
* @param[out] translatey : Resulting NDC Y translation to apply to the fullscreen quad (0 = centered).
* @param[out] visible : true if the canvas should be drawn at all; false to leave the surface cleared (blank).
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::ComputePresentationScale(GLsizei surfacewidth, GLsizei surfaceheight, float& scalex, float& scaley, float& translatex, float& translatey, bool& visible)
{
  scalex     = 1.0f;
  scaley     = 1.0f;
  translatex = 0.0f;
  translatey = 0.0f;
  visible    = true;

  if(surfacewidth <= 0 || surfaceheight <= 0 || texw <= 0 || texh <= 0)
    {
      return;
    }

  float surface_aspect = (float)surfacewidth / (float)surfaceheight;

  // The canvas is texw x texh, but at 90/270 degrees it is PRESENTED rotated, so its
  // effective (on-screen) aspect ratio is the inverse (texh/texw). Account for that here,
  // otherwise a rotated canvas would be letterboxed against the wrong axis and look squashed.
  float canvas_aspect;
  if(rotation == GRPSCREENROTATION_90_CLOCKWISE || rotation == GRPSCREENROTATION_90_ANTICLOCKWISE)
    {
      canvas_aspect = (float)texh / (float)texw;   // width/height swapped by the 90/270 rotation
    }
   else
    {
      canvas_aspect = (float)texw / (float)texh;   // 0 / 180 degrees: aspect unchanged
    }

  float ratio = canvas_aspect / surface_aspect;
  if(ratio >= 1.0f)
    {
      scalex = 1.0f;
      scaley = 1.0f / ratio;
    } // canvas wider  -> bars top/bottom
   else
    {
      scalex = ratio;
      scaley = 1.0f;
    } // canvas taller -> bars left/right
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::Destroy()
* @brief      destroy
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::Destroy()
{
  if(eglctx)
    {
      eglctx->MakeCurrent();

      if(texid)    
        { 
          glDeleteTextures(1, &texid);    
          texid = 0; 
        }

      if(vbo)      
        { 
          glDeleteBuffers(1, &vbo);       
          vbo   = 0; 
        }

      if(IsES3() && vao) 
        { 
          glDeleteVertexArrays(1, &vao); 
          vao = 0; 
        }

      if(pbo[0])   
        { 
          glDeleteBuffers(2, pbo);        
          pbo[0] = pbo[1] = 0; 
        }

      if(program)  
        {
          glDeleteProgram(program);       
          program = 0; 
        }

      eglctx->Destroy();
      GEN_DELETE eglctx;
      eglctx = NULL;
    }

  texw = texh = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::SwapBuffers()
* @brief      Swap front/back buffers of the underlying EGL surface. Wrapper kept here so callers (e.g. screens) need not know about GRPEGLCONTEXT.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::SwapBuffers()
{
  if(!eglctx) 
    {
      return false;
    }

  return eglctx->SwapBuffers();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetUseVSync(bool a)
* @brief      set use Vsync
* @ingroup    GRAPHIC
* 
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetUseVSync(bool a)
{ 
  usevsync = a; 
  if(eglctx) eglctx->SetSwapInterval(a ? 1 : 0); 

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetUseVSync()
* @brief      get use Vsync
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetUseVSync()
{ 
  return usevsync;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetNativeWindowSize(int& width, int& height)
* @brief      get native window size (Default: no native window-size reporting (Windows/Linux have no rotation/resize concern here)
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetNativeWindowSize(int& width, int& height) 
{ 
  width = 0; 
  height = 0; 

  return false; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetUsePBO(bool a)
* @brief      set use PBo
* @ingroup    GRAPHIC
* 
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetUsePBO(bool a)
{ 
  usepbo   = a;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetUsePBO()
* @brief      get use PBo
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetUsePBO()
{ 
  return usepbo;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetFlipY(bool a)
* @brief      set flip y
* @ingroup    GRAPHIC
* 
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetFlipY(bool a)
{ 
  flipy    = a;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetFlipY()
* @brief      get flip y
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetFlipY()
{ 
  return flipy;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetFlipX(bool a)
* @brief      set flip x
* @ingroup    GRAPHIC
* 
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetFlipX(bool a)
{ 
  flipx    = a;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetFlipX()
* @brief      get flip x
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetFlipX()
{
  return flipx;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetRotation(GRPSCREENROTATION r)
* @brief      set rotation
* @ingroup    GRAPHIC
* 
* @param[in]  r : New screen rotation.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetRotation(GRPSCREENROTATION r) 
{ 
  rotation = r;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENROTATION GRPBLITGLES::GetRotation()
* @brief      get rotation
* @ingroup    GRAPHIC
* 
* @return     GRPSCREENROTATION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENROTATION GRPBLITGLES::GetRotation()       
{
  return rotation;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::SetUseAlpha(bool a)
* @brief      set use alpha
* @ingroup    GRAPHIC
* 
* @param[in]  a : true to enable alpha usage; false otherwise.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::SetUseAlpha(bool a)              
{ 
  usealpha = a;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::GetUseAlpha()
* @brief      get use alpha
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::GetUseAlpha()                    
{ 
  return usealpha;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::IsES3()
* @brief      is Es3
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::IsES3()                
{ 
  return eglctx && (eglctx->GetClientVersion() >= 3); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::HasBGRAExtension()
* @brief      has BGRAextension
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::HasBGRAExtension()     
{ 
  return hasbgraext; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPEGLCONTEXT* GRPBLITGLES::GetEGLContext()
* @brief      get EGLcontext
* @ingroup    GRAPHIC
* 
* @return     GRPEGLCONTEXT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPEGLCONTEXT* GRPBLITGLES::GetEGLContext()        
{ 
  return eglctx; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::DetectBGRAExtension()
* @brief      detect BGRAextension
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::DetectBGRAExtension()
{
  hasbgraext = false;

  const GLubyte* exts = glGetString(GL_EXTENSIONS);

  if(!exts) 
    {
      return false;
    }

  if(strstr((const char*)exts, "GL_EXT_texture_format_BGRA8888"))    
    {
      hasbgraext = true;
    }

  if(strstr((const char*)exts, "GL_APPLE_texture_format_BGRA8888"))  
    {
      hasbgraext = true;
    }

  if(strstr((const char*)exts, "GL_IMG_texture_format_BGRA8888"))    
    {
      hasbgraext = true;
    }

  return hasbgraext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GLuint GRPBLITGLES::CompileShader(GLenum stage, const char* source)
* @brief      compile shader
* @ingroup    GRAPHIC
* 
* @param[in]  stage : Stage value.
* @param[in]  source : Source value.
* 
* @return     GLuint : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GLuint GRPBLITGLES::CompileShader(GLenum stage, const char* source)
{
  GLuint sh = glCreateShader(stage);
  if(!sh) 
    {
      return 0;
    }

  glShaderSource(sh, 1, &source, NULL);
  glCompileShader(sh);

  GLint ok = 0;
  glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
  if(!ok)
    {
      char log[1024]; GLsizei n = 0;
      glGetShaderInfoLog(sh, sizeof(log), &n, log);

      XSTRING xlog; xlog = log;
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[BlitGLES] Shader compile error: %s"), xlog.Get());

      glDeleteShader(sh);

      return 0;
    }

  return sh;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::CompileShaders()
* @brief      compile shaders
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::CompileShaders()
{
  // Channel order differs per platform:
  //  - Windows / Linux: the canvas is BGRA. We upload its bytes verbatim into an
  //    RGBA texture, so the shader must swizzle .bgra to restore correct colors.
  //  - Android: the canvas is RGBA (the legacy software path memcpy'd it verbatim
  //    into a WINDOW_FORMAT_RGBA_8888 buffer). No swizzle, or R and B get swapped.
  // If colors still look swapped on a given device, flip this single choice.
#if defined(__ANDROID__)
  const char* fsrc = k_fshader_bgra_src;     // plain sample, no swizzle (RGBA canvas)
#else
  const char* fsrc = k_fshader_swizzle_src;  // .bgra swizzle (BGRA canvas)
#endif

  GLuint vs = CompileShader(GL_VERTEX_SHADER, k_vshader_src);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsrc);
  if(!vs || !fs)
    {
      if(vs) glDeleteShader(vs);
      if(fs) glDeleteShader(fs);
      return false;
    }

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  // Explicit attribute locations (also given in shader source via layout qualifier)
  glBindAttribLocation(program, 0, "a_pos");
  glBindAttribLocation(program, 1, "a_uv");
  glLinkProgram(program);

  GLint ok = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &ok);

  glDetachShader(program, vs);  glDeleteShader(vs);
  glDetachShader(program, fs);  glDeleteShader(fs);

  if(!ok)
    {
      char log[1024]; GLsizei n = 0;
      glGetProgramInfoLog(program, sizeof(log), &n, log);
      XSTRING xlog; xlog = log;
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[BlitGLES] Program link error: %s"), xlog.Get());
      glDeleteProgram(program); program = 0;
      return false;
    }

  a_pos       = 0;
  a_uv        = 1;
  u_modelview = glGetUniformLocation(program, "u_modelview");
  u_tex       = glGetUniformLocation(program, "u_tex");

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::BuildQuad()
* @brief      build quad
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::BuildQuad()
{
  // Canonical UV mapping: OpenGL convention with V=0 at bottom, V=1 at top.
  // Any flip (horizontal/vertical) is applied via the model matrix, NOT via UVs.
  // TRIANGLE_STRIP: bl, br, tl, tr
  const float verts[] = {
                          //  x      y      u     v
                          -1.0f, -1.0f,    0.0f, 0.0f,
                           1.0f, -1.0f,    1.0f, 0.0f,
                          -1.0f,  1.0f,    0.0f, 1.0f,
                           1.0f,  1.0f,    1.0f, 1.0f,
                        };

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  if(IsES3())
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
      glBindVertexArray(0);
    }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::AllocTexture(int width, int height)
* @brief      alloc texture
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::AllocTexture(int width, int height)
{
  if(texid) 
    { 
      glDeleteTextures(1, &texid); texid = 0; 
    }

  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // ES 3.0 portability: GL_BGRA_EXT as an INTERNAL format is unreliable. It works on
  // ANGLE (Windows) but is invalid on strict Mesa ES 3.0 (Linux/Pi), where it silently
  // produces GL_INVALID_OPERATION and leaves the texture undefined (black quad).
  // We always store as RGBA8 and let the fragment shader swizzle the BGRA canvas data
  // (.bgra) at sample time. This costs nothing on the GPU and works on every driver.
  // Zero-initialise the texture storage. If for any reason the uploaded canvas
  // does not cover the whole texture (size mismatch), the uncovered texels then
  // sample as black instead of uninitialised GPU memory (the streaky garbage).
  {
    XBYTE* zeros = (XBYTE*)malloc((size_t)width * (size_t)height * 4);
    if(zeros)
      {
        memset(zeros, 0, (size_t)width * (size_t)height * 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zeros);
        free(zeros);
      }
     else
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      }
  }

  texw = width;
  texh = height;

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[BlitGLES] AllocTexture %d x %d (RGBA8)"), width, height);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBLITGLES::AllocPBOs(int width, int height)
* @brief      alloc PBos
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBLITGLES::AllocPBOs(int width, int height)
{
  if(pbo[0]) 
    { 
      glDeleteBuffers(2, pbo); 
      pbo[0] = pbo[1] = 0; 
    }

  glGenBuffers(2, pbo);

  pbo_size = (XDWORD)(width * height * 4);

  for(int i=0; i<2; i++)
    {
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[i]);
      glBufferData(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr)pbo_size, NULL, GL_STREAM_DRAW);
    }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  pbo_index = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::BuildModelMatrix(float* m)
* @brief      build model matrix
* @ingroup    GRAPHIC
* 
* @param[in]  m : M pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::BuildModelMatrix(float* m)
{
  // Identity
  for(int i=0; i<16; i++) 
    {
      m[i] = 0.0f;
    }

  m[0] = m[5] = m[10] = m[15] = 1.0f;

  float sx = flipx ? -1.0f : 1.0f;
  float sy = flipy ? -1.0f : 1.0f;

  float c = 1.0f, s = 0.0f;
  switch(rotation)
    {
      case GRPSCREENROTATION_NONE              : c =  1.0f; s =  0.0f; break;
      case GRPSCREENROTATION_90_CLOCKWISE      : c =  0.0f; s = -1.0f; break;
      case GRPSCREENROTATION_180               : c = -1.0f; s =  0.0f; break;
      case GRPSCREENROTATION_90_ANTICLOCKWISE  : c =  0.0f; s =  1.0f; break;
      default                                  : break;
    }

  // (Sx, Sy) scale then R rotation. Column-major:
  m[0] =  c * sx;   m[4] = -s * sy;   m[ 8] = 0.0f; m[12] = 0.0f;
  m[1] =  s * sx;   m[5] =  c * sy;   m[ 9] = 0.0f; m[13] = 0.0f;
  m[2] =  0.0f;     m[6] =  0.0f;     m[10] = 1.0f; m[14] = 0.0f;
  m[3] =  0.0f;     m[7] =  0.0f;     m[11] = 0.0f; m[15] = 1.0f;

  // Letterbox/pillarbox: scale the output (screen-space) X by lboxsx and Y by lboxsy so the
  // canvas keeps its aspect ratio inside a differently-shaped surface. This is a left-multiply
  // by diag(lboxsx, lboxsy, 1, 1), i.e. scale matrix row 0 (X out) and row 1 (Y out).
  m[0] *= lboxsx;  m[4] *= lboxsx;  m[ 8] *= lboxsx;  m[12] *= lboxsx;
  m[1] *= lboxsy;  m[5] *= lboxsy;  m[ 9] *= lboxsy;  m[13] *= lboxsy;

  // Screen-space anchor: shifts the already-scaled quad. 0,0 (default) leaves it centered, exactly
  // as before. A platform presentation policy that anchors to a corner instead of centering (see
  // GRPWINDOWSBLITGLES::ComputePresentationScale) sets lboxtx/lboxty accordingly; this is a pure
  // post-scale/post-rotation additive shift in final NDC space, so it pins whichever screen corner
  // it targets regardless of any rotation/flip baked into the matrix above.
  m[12] += lboxtx;
  m[13] += lboxty;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBLITGLES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBLITGLES::Clean()
{
  screen      = NULL;
  eglctx      = NULL;
  texid       = 0;
  vbo         = 0;
  vao         = 0;
  program     = 0;
  u_modelview = -1;
  u_tex       = -1;
  a_pos       = -1;
  a_uv        = -1;
  pbo[0]      = 0;
  pbo[1]      = 0;
  pbo_index   = 0;
  pbo_size    = 0;
  texw        = 0;
  texh        = 0;
  usevsync    = true;
  usepbo      = true;
  // Defaults: NO flip. The framework's canvas is stored bottom-up on Windows
  // (matching SetDIBitsToDevice positive biHeight = bottom-up DIB). With canonical
  // OpenGL UVs (V=0 at bottom, V=1 at top), this maps 1:1 to canvas BOTTOM and TOP.
  //
  // If a future platform reports the canvas as top-down (e.g. some Linux/Android
  // configurations), call SetFlipY(true) AFTER blitter creation.
  flipy       = false;
  flipx       = false;
  usealpha    = false;
  hasbgraext  = false;
  rotation    = GRPSCREENROTATION_NONE;
  lboxsx      = 1.0f;
  lboxsy      = 1.0f;
  lboxtx      = 0.0f;
  lboxty      = 0.0f;
}


#endif // GRP_OPENGL_ACTIVE
