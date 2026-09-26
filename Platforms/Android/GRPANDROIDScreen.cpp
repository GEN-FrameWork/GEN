/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPANDROIDScreen.cpp
* 
* @class      GRPANDROIDSCREEN
* @brief      ANDROID Graphics Screen class
* @ingroup    PLATFORM_ANDROID
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

#include "GRPANDROIDScreen.h"

#include "XTrace.h"

#include "GRP2DCanvas.h"



#ifdef GRP_OPENGL_ACTIVE
#include "GRPANDROIDBlitGLES.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

#include <android/log.h>
#define GENBLITLOG(...) __android_log_print(ANDROID_LOG_INFO, "GEN_BLIT", __VA_ARGS__)




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPANDROIDSCREEN::GRPANDROIDSCREEN(): GRPSCREEN()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPANDROIDSCREEN::GRPANDROIDSCREEN(): GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_ANDROID;

  SetMode(GRPPROPERTYMODE_32_RGBA_8888);

  SetIsBufferInverse(true);

  Styles_Set(GRPSCREENSTYLE_FULLSCREEN);

  isvalid = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPANDROIDSCREEN::~GRPANDROIDSCREEN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPANDROIDSCREEN::~GRPANDROIDSCREEN()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::Create(bool show)
* @brief      Create resource
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Create(bool show)
{
  return GRPSCREEN::Create(show);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::Update()
* @brief      Update
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Update()
{
  GENBLITLOG("PATH Update() no-arg (does nothing)");
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Update(GRP2DCANVAS* canvas)
{
  GENBLITLOG("PATH Update(canvas) ENTER canvas=%p handle=%p", (void*)canvas, (void*)anativehandle);
  if(!canvas)        { GENBLITLOG("Update(canvas): canvas NULL -> false"); return false; }
  if(!anativehandle) { GENBLITLOG("Update(canvas): anativehandle NULL -> false"); return false; }

  #ifndef GRP_OPENGL_ACTIVE

  ANativeWindow_Buffer abuffer;

  if(IsEqualSizeTo(canvas) == ISEQUAL)
    {
      if(ANativeWindow_lock(anativehandle, &abuffer, NULL) >= 0)
        {
          memcpy(abuffer.bits, canvas->Buffer_Get(), height * width * GetBytesperPixel());
          ANativeWindow_unlockAndPost(anativehandle);
        }
    }
   else
    {
      canvas->CopyBufferRenderToScreen(this);

      if(ANativeWindow_lock(anativehandle, &abuffer, NULL) >= 0)
        {
          memcpy((XBYTE*)abuffer.bits, canvas->Buffer_Get(), height * width * GetBytesperPixel());

          ANativeWindow_unlockAndPost(anativehandle);
        }

      /*
      if(ANativeWindow_lock(anativehandle, &abuffer, NULL) >= 0)
        {
          canvas->CopyBufferRenderToBufferScreen((XBYTE*)abuffer.bits, width, height, isbufferinverse);

          ANativeWindow_unlockAndPost(anativehandle);
        }
      */
    }

  return true;

  #else

  // On Android the native window handle arrives via SetAndroidHandle() AFTER
  // the activity is in the foreground; so blitter creation is deferred to the
  // first Update() call.
  if(!blitgles)
    {
      blitgles = GEN_NEW GRPANDROIDBLITGLES();
      if(!blitgles) return false;

      // FIX: propagate the screen's TRANSPARENT style to the blitter BEFORE
      // Create() is called.  Create() drives EGL config selection (ChooseConfig)
      // which requests EGL_ALPHA_SIZE only when usealpha==true.  If we skip
      // this call the EGL surface is created without an alpha channel and the
      // Android compositor treats the window as fully opaque (black background).
      // This mirrors what the Linux X11 blitter does via ChooseVisualID().
      blitgles->SetUseAlpha(Style_Is(GRPSCREENSTYLE_TRANSPARENT));
      GENBLITLOG("Update(canvas): usealpha=%d (screen style TRANSPARENT=%d)", (int)Style_Is(GRPSCREENSTYLE_TRANSPARENT), (int)Style_Is(GRPSCREENSTYLE_TRANSPARENT));

      GENBLITLOG("Update(canvas): creating blitgles...");
      if(!blitgles->Create(this))
        {
          GENBLITLOG("Update(canvas): blitgles->Create FAILED");
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Screen Android] BlitGLES create failed"));
          GEN_DELETE blitgles;
          blitgles = NULL;
          return false;
        }
    }

  GENBLITLOG("Update(canvas): GL branch, blitgles=%p, calling blit", (void*)blitgles);
  {
    bool br = blitgles->Update(canvas);
    GENBLITLOG("Update(canvas): GL blit result=%d", (int)br);
    return br;
  }

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
* @brief      Present a transparent-style screen.
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  canvas : canvas to present.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
{
  GENBLITLOG("PATH UpdateTransparent ENTER canvas=%p handle=%p style_transp=%d w=%d h=%d", (void*)canvas, (void*)anativehandle, (int)Style_Is(GRPSCREENSTYLE_TRANSPARENT), (int)width, (int)height);
  // Sample canvas pixels BEFORE blitting to see if the UI system drew the dialog.
  // If ALL pixels are 0x00000000 (transparent black), the canvas is empty and the
  // problem is NOT the blitter — it is GEN_USERINTERFACE.Update() not rendering.
  if(canvas && canvas->Buffer_Get())
    {
      auto* buf  = (unsigned int*)canvas->Buffer_Get();
      int   cx   = (int)width / 2;
      int   cy   = (int)height / 2;
      unsigned int p0     = buf[0];
      unsigned int pcenter= buf[cx + cy * (int)width];
      unsigned int p10_30 = ((int)width > 10 && (int)height > 30) ? buf[10 + 30 * (int)width] : 0;
      GENBLITLOG("CANVAS PIXELS top-left=0x%08X center(%d,%d)=0x%08X pos(10,30)=0x%08X", p0, cx, cy, pcenter, p10_30);
      GENBLITLOG("CANVAS EMPTY=%s  (0x00000000 = transparent/nothing drawn)", (p0==0 && pcenter==0 && p10_30==0) ? "YES -> UI not drawing" : "NO -> blitter issue");
    }
  bool r = Update(canvas);
  GENBLITLOG("PATH UpdateTransparent EXIT result=%d", (int)r);
  return r;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::Delete()
* @brief      Delete resource
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Delete()
{
  #ifdef GRP_OPENGL_ACTIVE
  if(blitgles)
    {
      blitgles->Destroy();
      GEN_DELETE blitgles;
      blitgles = NULL;
    }
  #endif

  isactive = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::Resize(int width, int height)
* @brief      Resize
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Resize(int width, int height)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPANDROIDSCREEN::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_ANDROID
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPANDROIDSCREEN::GetHandle()
{
  return (void*)anativehandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ANativeWindow* GRPANDROIDSCREEN::GetAndroidHandle()
* @brief      Get android handle
* @ingroup    PLATFORM_ANDROID
* 
* @return     ANativeWindow* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
ANativeWindow* GRPANDROIDSCREEN::GetAndroidHandle()
{
  return anativehandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPANDROIDSCREEN::SetAndroidHandle(ANativeWindow* anativehandle)
* @brief      Set android handle
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  anativehandle : Anativehandle pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPANDROIDSCREEN::SetAndroidHandle(ANativeWindow* anativehandle)
{
  this->anativehandle = anativehandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDSCREEN::MapWindowToCanvas(float windowx, float windowy, float& canvasx, float& canvasy)
* @brief      Convert a touch/pointer position expressed in NATIVE-WINDOW pixels (the space Android reports in AMotionEvent_getX/getY) into CANVAS pixels (the design-resolution space the UI works in).
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  windowx : pointer X in native-window pixels.
* @param[in]  windowy : pointer Y in native-window pixels.
* @param[in]  canvasx : resulting X in canvas pixels (top-left origin).
* @param[in]  canvasy : resulting Y in canvas pixels (top-left origin).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDSCREEN::MapWindowToCanvas(float windowx, float windowy, float& canvasx, float& canvasy)
{
  if(!anativehandle) return false;

  float surfacew = (float)ANativeWindow_getWidth(anativehandle);
  float surfaceh = (float)ANativeWindow_getHeight(anativehandle);
  if(surfacew <= 0.0f || surfaceh <= 0.0f) return false;

  float canvasw  = (float)GetWidth();
  float canvash  = (float)GetHeight();
  if(canvasw <= 0.0f || canvash <= 0.0f) return false;

  // Presentation (on-screen) logical size: for a 90/270 rotation the axes are swapped. This is exactly the
  // aspect the blitter letterboxes against (canvas_aspect in GRPBLITGLES::Update).
  float presentw = (float)GetPresentationWidth();
  float presenth = (float)GetPresentationHeight();
  if(presentw <= 0.0f || presenth <= 0.0f) return false;

  // --- Reproduce the blitter letterbox (same math as GRPBLITGLES::Update) ----------------------------------
  float canvas_aspect  = presentw / presenth;
  float surface_aspect = surfacew / surfaceh;
  float ratio          = canvas_aspect / surface_aspect;

  float lboxsx;
  float lboxsy;
  if(ratio >= 1.0f) { lboxsx = 1.0f;  lboxsy = 1.0f / ratio; }   // bars top/bottom
  else              { lboxsx = ratio; lboxsy = 1.0f;         }   // bars left/right

  float draww = surfacew * lboxsx;
  float drawh = surfaceh * lboxsy;
  if(draww <= 0.0f || drawh <= 0.0f) return false;

  float offx  = (surfacew - draww) * 0.5f;
  float offy  = (surfaceh - drawh) * 0.5f;

  // Normalized position inside the presented (on-screen) content, top-left origin.
  float nx = (windowx - offx) / draww;
  float ny = (windowy - offy) / drawh;

  // A touch on the black bars is clamped to the nearest content edge.
  if(nx < 0.0f) nx = 0.0f; else if(nx > 1.0f) nx = 1.0f;
  if(ny < 0.0f) ny = 0.0f; else if(ny > 1.0f) ny = 1.0f;

  // Un-rotate from the presented orientation back into canvas space (top-left origin). For the common
  // non-rotated case this is a direct scale; the 90/270 branches undo the presentation axis swap.
  switch(GetRotation())
    {
      case GRPSCREENROTATION_90_CLOCKWISE      : canvasx = ny * canvasw;
                                                 canvasy = (1.0f - nx) * canvash;
                                                 break;

      case GRPSCREENROTATION_90_ANTICLOCKWISE  : canvasx = (1.0f - ny) * canvasw;
                                                 canvasy = nx * canvash;
                                                 break;

      case GRPSCREENROTATION_180               : canvasx = (1.0f - nx) * canvasw;
                                                 canvasy = (1.0f - ny) * canvash;
                                                 break;

      case GRPSCREENROTATION_NONE              :
      default                                  : canvasx = nx * canvasw;
                                                 canvasy = ny * canvash;
                                                 break;
    }

  return true;
}


#ifdef GRP_OPENGL_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPANDROIDBLITGLES* GRPANDROIDSCREEN::GetBlitGLES()
* @brief      Get OpenGL ES blitter (only present when GRP_OPENGL_ACTIVE is defined)
* @ingroup    PLATFORM_ANDROID
* 
* @return     GRPANDROIDBLITGLES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPANDROIDBLITGLES* GRPANDROIDSCREEN::GetBlitGLES()
{
  return blitgles;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPANDROIDSCREEN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPANDROIDSCREEN::Clean()
{
  anativehandle  = NULL;

  #ifdef GRP_OPENGL_ACTIVE
  blitgles       = NULL;
  #endif
}



