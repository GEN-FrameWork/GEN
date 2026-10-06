/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Form.cpp
* 
* @class      UI_ELEMENT_FORM
* @brief      User Interface Element Form class
* @ingroup    USERINTERFACE
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

#include "UI_Element_Form.h"

#include "GRPBitmap.h"
#include "GRPFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_FORM::UI_ELEMENT_FORM()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_FORM::UI_ELEMENT_FORM()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_FORM);

  GetTypeString()->Set(_L("form"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_FORM::~UI_ELEMENT_FORM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_FORM::~UI_ELEMENT_FORM()
{
  ShadowCache_Release();              // must run BEFORE Clean() -- see the note on shadowcache_bitmap in the header
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR* UI_ELEMENT_FORM::GetLineColor()
* @brief      Get line color
* @ingroup    USERINTERFACE
* 
* @return     UI_COLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_ELEMENT_FORM::GetLineColor()
{
  return &linecolor;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_BOUNDARYLINE* UI_ELEMENT_FORM::GetVisibleRect()
* @brief      Get visible rect
* @ingroup    USERINTERFACE
* 
* @return     UI_BOUNDARYLINE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_BOUNDARYLINE* UI_ELEMENT_FORM::GetVisibleRect()
{
  return &visiblerect;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_FORM::GetLegacyFillWarningEmitted()
* @brief      Latched by Draw_Form the first time it uses the legacy "color" property as fill, so the migration
*             warning is emitted once per instance rather than every frame.
* @ingroup    USERINTERFACE
*
* @return     bool : true iff the warning was already emitted for this Form.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_FORM::GetLegacyFillWarningEmitted()
{
  return legacy_fill_warning_emitted;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_FORM::SetLegacyFillWarningEmitted(bool value)
* @brief      Set the legacy-fill warning latch.
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to store.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_FORM::SetLegacyFillWarningEmitted(bool value)
{
  legacy_fill_warning_emitted = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPBITMAP* UI_ELEMENT_FORM::ShadowCache_GetIfMatches(int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color)
* @brief      Returns the cached, already-blurred box-shadow bitmap if it was built with exactly this key (the
*             only inputs that change its pixels); NULL if there is no cache yet or the key no longer matches,
*             meaning the caller must re-render (see UI_SkinCanvas_DrawSoftShadow_FormCached).
* @ingroup    USERINTERFACE
*
* @param[in]  shape_w : Un-padded shadow silhouette width (screen pixels).
* @param[in]  shape_h : Un-padded shadow silhouette height (screen pixels).
* @param[in]  rTL : Top-left corner radius.
* @param[in]  rTR : Top-right corner radius.
* @param[in]  rBR : Bottom-right corner radius.
* @param[in]  rBL : Bottom-left corner radius.
* @param[in]  blur : Blur radius in pixels.
* @param[in]  color : Shadow colour.
*
* @return     GRPBITMAP* : The cached bitmap on a match; NULL otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_FORM::ShadowCache_GetIfMatches(int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color)
{
  if(!shadowcache_bitmap) return NULL;
  if(!color)              return NULL;

  XDWORD colorrgba = ((XDWORD)color->GetRed()   << 24) |
                      ((XDWORD)color->GetGreen() << 16) |
                      ((XDWORD)color->GetBlue()  <<  8) |
                      ((XDWORD)color->GetAlpha());

  if(shadowcache_shape_w   != shape_w)     return NULL;
  if(shadowcache_shape_h   != shape_h)     return NULL;
  if(shadowcache_rTL       != rTL)         return NULL;
  if(shadowcache_rTR       != rTR)         return NULL;
  if(shadowcache_rBR       != rBR)         return NULL;
  if(shadowcache_rBL       != rBL)         return NULL;
  if(shadowcache_blur      != blur)        return NULL;
  if(shadowcache_colorrgba != colorrgba)   return NULL;

  return shadowcache_bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_FORM::ShadowCache_Set(GRPBITMAP* bitmap, int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color)
* @brief      Takes ownership of a freshly-rendered, already-blurred box-shadow bitmap and stores it as the new
*             cache entry, freeing whatever was cached before. The element now owns "bitmap" -- the caller must
*             not delete it.
* @ingroup    USERINTERFACE
*
* @param[in]  bitmap : Newly-rendered blurred shadow bitmap (ownership transfers to this element).
* @param[in]  shape_w : Un-padded shadow silhouette width (screen pixels) this bitmap was built for.
* @param[in]  shape_h : Un-padded shadow silhouette height (screen pixels) this bitmap was built for.
* @param[in]  rTL : Top-left corner radius this bitmap was built for.
* @param[in]  rTR : Top-right corner radius this bitmap was built for.
* @param[in]  rBR : Bottom-right corner radius this bitmap was built for.
* @param[in]  rBL : Bottom-left corner radius this bitmap was built for.
* @param[in]  blur : Blur radius (pixels) this bitmap was built for.
* @param[in]  color : Shadow colour this bitmap was built for.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_FORM::ShadowCache_Set(GRPBITMAP* bitmap, int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color)
{
  ShadowCache_Release();      // drop whatever was cached before taking ownership of the new bitmap

  shadowcache_bitmap  = bitmap;
  shadowcache_shape_w = shape_w;
  shadowcache_shape_h = shape_h;
  shadowcache_rTL     = rTL;
  shadowcache_rTR     = rTR;
  shadowcache_rBR     = rBR;
  shadowcache_rBL     = rBL;
  shadowcache_blur    = blur;

  if(color) shadowcache_colorrgba = ((XDWORD)color->GetRed()   << 24) |
                                     ((XDWORD)color->GetGreen() << 16) |
                                     ((XDWORD)color->GetBlue()  <<  8) |
                                     ((XDWORD)color->GetAlpha());
   else     shadowcache_colorrgba = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_FORM::ShadowCache_Release()
* @brief      Frees the cached box-shadow bitmap, if any. Safe to call whether or not a bitmap is cached. Called
*             from the destructor (before Clean() resets the key fields) and from ShadowCache_Set() before
*             taking ownership of a new bitmap.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_FORM::ShadowCache_Release()
{
  if(shadowcache_bitmap)
    {
      GRPFACTORY::GetInstance().DeleteBitmap(shadowcache_bitmap);
      shadowcache_bitmap = NULL;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_FORM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_FORM::Clean()
{
  roundvisiblerect            = 0;
  legacy_fill_warning_emitted = false;

  // Box-shadow bitmap cache: reset the KEY/pointer only -- freeing shadowcache_bitmap, if set, is the caller's
  // job (ShadowCache_Release()) since Clean() also runs from the CONSTRUCTOR, before shadowcache_bitmap holds
  // a defined value to safely inspect or delete.
  shadowcache_bitmap    = NULL;
  shadowcache_shape_w   = -1;
  shadowcache_shape_h   = -1;
  shadowcache_rTL       = -1.0;
  shadowcache_rTR       = -1.0;
  shadowcache_rBR       = -1.0;
  shadowcache_rBL       = -1.0;
  shadowcache_blur      = -1;
  shadowcache_colorrgba = 0;
}



