/**-------------------------------------------------------------------------------------------------------------------
*
* @file       GRPANDROIDBlitGLES.cpp
*
* @class      GRPANDROIDBLITGLES
* @brief      Android specialisation. Provides ANativeWindow* to EGL and reconfigures the
*             buffer geometry to the EGL-chosen native visual id (this is mandatory on Android).
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

#ifdef GRP_OPENGL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPANDROIDBlitGLES.h"

#include "GRPANDROIDScreen.h"
#include "GRPEGLContext.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPANDROIDBLITGLES::GRPANDROIDBLITGLES() : GRPBLITGLES()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPANDROIDBLITGLES::GRPANDROIDBLITGLES()  : GRPBLITGLES() { }
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPANDROIDBLITGLES::~GRPANDROIDBLITGLES()
* @brief      Destructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPANDROIDBLITGLES::~GRPANDROIDBLITGLES()                 { }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeDisplayType GRPANDROIDBLITGLES::GetNativeDisplay()
* @brief      Get native display
* @ingroup    PLATFORM_ANDROID
* 
* @return     EGLNativeDisplayType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeDisplayType GRPANDROIDBLITGLES::GetNativeDisplay()
{
  return (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         EGLNativeWindowType GRPANDROIDBLITGLES::GetNativeWindow()
* @brief      Get native window
* @ingroup    PLATFORM_ANDROID
* 
* @return     EGLNativeWindowType : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
EGLNativeWindowType GRPANDROIDBLITGLES::GetNativeWindow()
{
  GRPANDROIDSCREEN* as = (GRPANDROIDSCREEN*)screen;
  if(!as) return (EGLNativeWindowType)NULL;
  return (EGLNativeWindowType)as->GetAndroidHandle();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDBLITGLES::GetNativeWindowSize(int& width, int& height)
* @brief      Report the CURRENT ANativeWindow size. Used by the base blitter to detect a rotation/resize and recreate the EGL surface accordingly.
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDBLITGLES::GetNativeWindowSize(int& width, int& height)
{
  GRPANDROIDSCREEN* as = (GRPANDROIDSCREEN*)screen;
  if(!as) return false;

  ANativeWindow* awin = (ANativeWindow*)as->GetAndroidHandle();
  if(!awin) return false;

  width  = ANativeWindow_getWidth(awin);
  height = ANativeWindow_getHeight(awin);
  return (width > 0 && height > 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPANDROIDBLITGLES::PostCreateHook(EGLint native_visual_id)
* @brief      On Android the ANativeWindow buffer geometry MUST be reconfigured to the native visual id selected by EGL before the surface is used. Otherwise the driver may either fail or silently mismatch the format.
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  native_visual_id : Native visual identifier.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPANDROIDBLITGLES::PostCreateHook(EGLint native_visual_id)
{
  if(native_visual_id == 0 && eglctx)
    {
      EGLDisplay d = eglctx->GetEGLDisplay();
      EGLConfig  c = eglctx->GetEGLConfig();
      eglGetConfigAttrib(d, c, EGL_NATIVE_VISUAL_ID, &native_visual_id);
    }

  ANativeWindow* awin = (ANativeWindow*)GetNativeWindow();
  if(!awin) return false;

  ANativeWindow_setBuffersGeometry(awin, 0, 0, native_visual_id);

  // On Android the canvas reaches the texture with the opposite ROW order
  // compared to the screen (bottom-up vs top-down), so only the vertical axis
  // needs flipping. The horizontal axis is already correct.
  SetFlipX(false);
  SetFlipY(true);

  return true;
}


#endif // GRP_OPENGL_ACTIVE
