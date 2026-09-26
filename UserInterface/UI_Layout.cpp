/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Layout.cpp
* 
* @class      UI_LAYOUT
* @brief      User Interface Layout class
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

#include "UI_Layout.h"

#include "UI_Skin.h"
#include "UI_StyleSheet.h"

#include "GRPFactory.h"
#include "GRP2DCanvas.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT::UI_LAYOUT(UI_SKIN* ui_skin)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  ui_skin : Ui skin pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT::UI_LAYOUT(UI_SKIN* ui_skin)
{
  Clean();

  this->ui_skin = ui_skin;
  this->ownskin = true;                 // default: this layout exclusively owns "ui_skin" -- see SetOwnsSkin()
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT::~UI_LAYOUT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT::~UI_LAYOUT()
{
  if(ui_skin && ownskin)          // see SetOwnsSkin(): only the owning layout frees a UI_SKIN shared by several
    {                             // UI_LAYOUT instances (one skin per XML root, not per <layout> node)
      GEN_DELETE ui_skin;
    }

  Elements_DeleteAll();

  if(stylesheet)
    {
      GEN_DELETE stylesheet;
      stylesheet = NULL;
    }

  if(designcanvas)
    {
      GEN_GRPFACTORY.DeleteCanvas(designcanvas);
      designcanvas = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_LAYOUT::GetNameID()
* @brief      Get name ID
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_LAYOUT::GetNameID()
{
  return &nameID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN* UI_LAYOUT::GetSkin()
* @brief      Get skin
* @ingroup    USERINTERFACE
* 
* @return     UI_SKIN* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN* UI_LAYOUT::GetSkin()
{
  return ui_skin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::SetSkin(UI_SKIN* ui_skin)
* @brief      Set skin
* @ingroup    USERINTERFACE
* 
* @param[in]  ui_skin : Ui skin pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetSkin(UI_SKIN* ui_skin)
{
  this->ui_skin = ui_skin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::SetOwnsSkin(bool ownskin)
* @brief      Sets whether this layout is responsible for deleting its "ui_skin" in the destructor.
* @note       See the declaration in UI_Layout.h for why this exists: UI_MANAGER::CreateLayouts() constructs
*             every UI_LAYOUT for a given XML root with the SAME UI_SKIN*, so all but the first must pass
*             "false" here to avoid deleting it more than once.
* @ingroup    USERINTERFACE
*
* @param[in]  ownskin : false if this layout must NOT delete "ui_skin" in its destructor.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetOwnsSkin(bool ownskin)
{
  this->ownskin = ownskin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_BACKGROUND* UI_LAYOUT::GetBackground()
* @brief      Get background
* @ingroup    USERINTERFACE
* 
* @return     UI_BACKGROUND* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_BACKGROUND* UI_LAYOUT::GetBackground()
{
  return &background;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_STYLESHEET* UI_LAYOUT::GetStyleSheet()
* @brief      Currently active CSS stylesheet for this layout, or NULL when none was declared in its XML.
* @ingroup    USERINTERFACE
*
* @return     UI_STYLESHEET* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_STYLESHEET* UI_LAYOUT::GetStyleSheet()
{
  return stylesheet;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::SetStyleSheet(UI_STYLESHEET* sheet)
* @brief      Set style sheet
* @note       Takes ownership of "sheet" -- deletes any previously owned instance first, then stores the new
*             pointer (which may be NULL, e.g. this layout's XML declared no \<stylesheet\>).
* @ingroup    USERINTERFACE
*
* @param[in]  sheet : Sheet pointer to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetStyleSheet(UI_STYLESHEET* sheet)
{
  if(stylesheet && (stylesheet != sheet))
    {
      GEN_DELETE stylesheet;
    }

  stylesheet = sheet;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_LAYOUT::GetDesignWidth() const
* @brief      Logical design canvas width in authoring pixels (0 = unset).
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Design width.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_LAYOUT::GetDesignWidth() const
{
  return designwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_LAYOUT::GetDesignHeight() const
* @brief      Logical design canvas height in authoring pixels (0 = unset).
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Design height.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_LAYOUT::GetDesignHeight() const
{
  return designheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::SetDesignSize(XDWORD width, XDWORD height)
* @brief      Set the logical design canvas size authors target in XML/CSS.
* @ingroup    USERINTERFACE
*
* @param[in]  width  : Design width in px (0 clears / unset).
* @param[in]  height : Design height in px (0 clears / unset).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetDesignSize(XDWORD width, XDWORD height)
{
  designwidth  = width;
  designheight = height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUT::GetUIScale() const
* @brief      Current UI scale factor (1.0 = design px == framebuffer px).
* @ingroup    USERINTERFACE
*
* @return     double : Scale (> 0).
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::GetUIScale() const
{
  return uiscale;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::SetUIScale(double scale)
* @brief      Set UI scale. Clamped to [UI_LAYOUT_UISCALE_MIN .. UI_LAYOUT_UISCALE_MAX] (product zoom limits).
* @ingroup    USERINTERFACE
*
* @param[in]  scale : Desired scale factor.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetUIScale(double scale)
{
  if(scale < UI_LAYOUT_UISCALE_MIN) scale = UI_LAYOUT_UISCALE_MIN;
  if(scale > UI_LAYOUT_UISCALE_MAX) scale = UI_LAYOUT_UISCALE_MAX;

  uiscale = scale;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::SetUIScaleForFit(double scale)
* @brief      Fase 5 autofit: clamp to [FIT_MIN .. MAX]. Does not clear the autofit flag.
* @ingroup    USERINTERFACE
* 
* @param[in]  scale : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetUIScaleForFit(double scale)
{
  if(scale < UI_LAYOUT_UISCALE_FIT_MIN) scale = UI_LAYOUT_UISCALE_FIT_MIN;
  if(scale > UI_LAYOUT_UISCALE_MAX)     scale = UI_LAYOUT_UISCALE_MAX;

  uiscale = scale;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::SetUIScaleEnabled(bool enabled)
* @brief      Explicit opt-in for the design-canvas / UIScale path (even without a stylesheet).
* @ingroup    USERINTERFACE
*
* @param[in]  enabled : true to force UIScale active.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetUIScaleEnabled(bool enabled)
{
  uiscale_enabled = enabled;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LAYOUT::GetUIScaleEnabled() const
* @brief      Explicit UIScale flag (independent of stylesheet presence).
* @ingroup    USERINTERFACE
*
* @return     bool : true if SetUIScaleEnabled(true) was called.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::GetUIScaleEnabled() const
{
  return uiscale_enabled;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::SetUIScaleAutofit(bool autofit)
* @brief      Fase 5: when true, scale follows window size via ComputeFitUIScale.
* @ingroup    USERINTERFACE
* 
* @param[in]  autofit : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetUIScaleAutofit(bool autofit)
{
  uiscale_autofit = autofit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::GetUIScaleAutofit() const
* @brief      Whether fit-to-window scale is active.
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::GetUIScaleAutofit() const
{
  return uiscale_autofit;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LAYOUT::IsUIScaleActive() const
* @brief      Whether this layout uses the design-canvas contract.
* @note       Opt-in: stylesheet present OR explicit SetUIScaleEnabled(true). XML-only layouts stay legacy.
* @ingroup    USERINTERFACE
*
* @return     bool : true when UIScale path applies.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::IsUIScaleActive() const
{
  if(uiscale_enabled) return true;
  if(stylesheet)      return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_LAYOUT::GetAssetRasterScale() const
* @brief      Fase 7: pixel density for design canvas + vector/chart raster (1.0 when UIScale inactive).
* @ingroup    USERINTERFACE
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::GetAssetRasterScale() const
{
  if(!IsUIScaleActive()) return UI_LAYOUT_UISCALE_DEFAULT;

  double s = uiscale;
  if(s < UI_LAYOUT_UISCALE_MIN) s = UI_LAYOUT_UISCALE_MIN;
  if(s > UI_LAYOUT_UISCALE_MAX) s = UI_LAYOUT_UISCALE_MAX;
  return s;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_LAYOUT::ComputeFitUIScale(XDWORD screen_w, XDWORD screen_h) const
* @brief      Fase 5: scale = min(sw/dw, sh/dh), clamped to [FIT_MIN .. MAX]. Does not change layout state.
* @ingroup    USERINTERFACE
* 
* @param[in]  screen_w : 
* @param[in]  screen_h : 
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::ComputeFitUIScale(XDWORD screen_w, XDWORD screen_h) const
{
  double dw = designwidth  ? (double)designwidth  : (double)screen_w;
  double dh = designheight ? (double)designheight : (double)screen_h;
  if(dw < 1.0) dw = 1.0;
  if(dh < 1.0) dh = 1.0;
  if(screen_w < 1) screen_w = 1;
  if(screen_h < 1) screen_h = 1;

  double sx = ((double)screen_w) / dw;
  double sy = ((double)screen_h) / dh;
  double s  = (sx < sy) ? sx : sy;

  if(s < UI_LAYOUT_UISCALE_FIT_MIN) s = UI_LAYOUT_UISCALE_FIT_MIN;
  if(s > UI_LAYOUT_UISCALE_MAX)     s = UI_LAYOUT_UISCALE_MAX;
  return s;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_LAYOUT::GetMinHitSize() const
* @brief      Fase 6: minimum hit AABB edge in design px (0 disables expansion).
* @ingroup    USERINTERFACE
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::GetMinHitSize() const
{
  return minhitsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::SetMinHitSize(double size)
* @brief      Fase 6: set minimum hit AABB (design px). Negative values clamp to 0 (disabled).
* @ingroup    USERINTERFACE
* 
* @param[in]  size : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetMinHitSize(double size)
{
  if(size < 0.0) size = 0.0;
  minhitsize = size;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::ScreenToDesign(double screen_x, double screen_y, double& design_x, double& design_y) const
* @brief      Convert framebuffer (screen) coordinates to design / authoring pixels.
* @ingroup    USERINTERFACE
*
* @param[in]  screen_x : X in screen px.
* @param[in]  screen_y : Y in screen px.
* @param[out] design_x : X in design px.
* @param[out] design_y : Y in design px.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::ScreenToDesign(double screen_x, double screen_y, double& design_x, double& design_y) const
{
  double s = uiscale;
  if(s < UI_LAYOUT_UISCALE_MIN) s = UI_LAYOUT_UISCALE_MIN;

  design_x = (screen_x - uiscale_offset_x) / s;
  design_y = (screen_y - uiscale_offset_y) / s;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUT::DesignToScreen(double design_x, double design_y, double& screen_x, double& screen_y) const
* @brief      Convert design / authoring pixels to framebuffer (screen) coordinates.
* @ingroup    USERINTERFACE
*
* @param[in]  design_x : X in design px.
* @param[in]  design_y : Y in design px.
* @param[out] screen_x : X in screen px.
* @param[out] screen_y : Y in screen px.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::DesignToScreen(double design_x, double design_y, double& screen_x, double& screen_y) const
{
  double s = uiscale;
  if(s < UI_LAYOUT_UISCALE_MIN) s = UI_LAYOUT_UISCALE_MIN;

  screen_x = design_x * s + uiscale_offset_x;
  screen_y = design_y * s + uiscale_offset_y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_LAYOUT::GetUIScaleOffsetX() const
* @brief      Present letterbox/crop offset X (screen px).
* @ingroup    USERINTERFACE
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::GetUIScaleOffsetX() const
{
  return uiscale_offset_x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_LAYOUT::GetUIScaleOffsetY() const
* @brief      Present letterbox/crop offset Y (screen px).
* @ingroup    USERINTERFACE
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUT::GetUIScaleOffsetY() const
{
  return uiscale_offset_y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::ComputePresentTransform(XDWORD screen_w, XDWORD screen_h)
* @brief      Center design*scale in the screen (letterbox when smaller; crop when larger/zoom).
* @ingroup    USERINTERFACE
* 
* @param[in]  screen_w : 
* @param[in]  screen_h : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::ComputePresentTransform(XDWORD screen_w, XDWORD screen_h)
{
  double dw = designwidth  ? (double)designwidth  : (double)screen_w;
  double dh = designheight ? (double)designheight : (double)screen_h;
  double s  = uiscale;
  if(s < UI_LAYOUT_UISCALE_MIN) s = UI_LAYOUT_UISCALE_MIN;

  double present_w = dw * s;
  double present_h = dh * s;

  // Letterbox/pillarbox when the scaled design fits: center with black bars.
  // Zoom larger than the screen: top-left align so origin-side controls (sidebar) stay hittable.
  if(present_w <= (double)screen_w && present_h <= (double)screen_h)
    {
      uiscale_offset_x = (((double)screen_w) - present_w) * 0.5;
      uiscale_offset_y = (((double)screen_h) - present_h) * 0.5;
    }
   else
    {
      uiscale_offset_x = 0.0;
      uiscale_offset_y = 0.0;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::NeedsScaledPresent(XDWORD screen_w, XDWORD screen_h) const
* @brief      Whether paint must use design offscreen + scaled blit.
* @ingroup    USERINTERFACE
* 
* @param[in]  screen_w : 
* @param[in]  screen_h : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::NeedsScaledPresent(XDWORD screen_w, XDWORD screen_h) const
{
  if(!IsUIScaleActive()) return false;

  if(uiscale < (UI_LAYOUT_UISCALE_DEFAULT - UI_LAYOUT_UISCALE_EPSILON) ||
     uiscale > (UI_LAYOUT_UISCALE_DEFAULT + UI_LAYOUT_UISCALE_EPSILON))
    {
      return true;
    }

  if(designwidth && designheight)
    {
      if(designwidth != screen_w || designheight != screen_h) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS* UI_LAYOUT::GetDesignCanvas() const
* @brief      Owned design framebuffer for scaled present, or NULL.
* @ingroup    USERINTERFACE
* 
* @return     GRP2DCANVAS* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS* UI_LAYOUT::GetDesignCanvas() const
{
  return designcanvas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::SetDesignCanvas(GRP2DCANVAS* canvas)
* @brief      Take ownership of a design canvas (deletes any previous).
* @ingroup    USERINTERFACE
* 
* @param[in]  canvas : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::SetDesignCanvas(GRP2DCANVAS* canvas)
{
  if(designcanvas && designcanvas != canvas)
    {
      GEN_GRPFACTORY.DeleteCanvas(designcanvas);
    }

  designcanvas = canvas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::Elements_Add(UI_ELEMENT* element)
* @brief      Elements add
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::Elements_Add(UI_ELEMENT* element)
{
  if(!element) return false;

  elements.Add(element);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<UI_ELEMENT*>* UI_LAYOUT::Elements_Get()
* @brief      Elements get
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<UI_ELEMENT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_ELEMENT*>* UI_LAYOUT::Elements_Get()
{
  return &elements;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_LAYOUT::Elements_Get(XCHAR* nameelement, UI_ELEMENT_TYPE type)
* @brief      Elements get
* @ingroup    USERINTERFACE
* 
* @param[in]  nameelement : Nameelement pointer to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_LAYOUT::Elements_Get(XCHAR* nameelement, UI_ELEMENT_TYPE type)
    {
  if(elements.IsEmpty()) return NULL;

  for(XDWORD c=0; c<elements.GetSize(); c++)
    {
      UI_ELEMENT* element = elements.Get(c);
      if(element)
        {
          if(!element->GetName()->Compare(nameelement, true)) 
            {
              if(type != UI_ELEMENT_TYPE_UNKNOWN) 
                {
                  if(element->GetType() == type) return element;
                }
               else return element;
            }

          if(element->GetComposeElements()->GetSize())
            {
              UI_ELEMENT* subelement = Elements_Get(element, nameelement, type);
              if(subelement) return subelement;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT_CHROMEROLE chromerole)
* @brief      Elements get
* @note       Finds the element carrying a given Chromes role ("the close button", "the title"...), so a custom
*             Chromes (window caption) can be wired up without depending on element names.
* @ingroup    USERINTERFACE
* 
* @param[in]  chromerole : Chromerole value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT_CHROMEROLE chromerole)
{
  if(chromerole == UI_ELEMENT_CHROMEROLE_NONE) return NULL;

  if(elements.IsEmpty()) return NULL;

  for(XDWORD c=0; c<elements.GetSize(); c++)
    {
      UI_ELEMENT* element = elements.Get(c);
      if(element)
        {
          if(element->GetChromeRole() == chromerole) return element;

          if(element->GetComposeElements()->GetSize())
            {
              UI_ELEMENT* subelement = Elements_Get(element, chromerole);
              if(subelement) return subelement;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* @fn         bool UI_LAYOUT::Elements_DeleteAll()
* @brief      Elements GEN_DELETE all
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::Elements_DeleteAll()
{
  if(elements.IsEmpty())  return false;

  elements.DeleteContents();
  elements.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::Elements_SetToRedraw(bool recursive)
* @brief      Elements set to redraw
* @ingroup    USERINTERFACE
* 
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::Elements_SetToRedraw(bool recursive)
{
  bool status = false;

  for(XDWORD c=0; c<elements.GetSize(); c++) 
    {
      UI_ELEMENT* element = (UI_ELEMENT*)elements.Get(c);
      if(element) 
        {
          status = Elements_SetToRedraw(element, recursive);                
        }
    }

  return status;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
* @brief      Elements set to redraw
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
{
  if(!element) return false;

  bool status = true;

  element->SetMustReDraw(true);

  if(recursive)
    {
      for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
          if(subelement) 
            { 
              status = Elements_SetToRedraw(subelement, recursive);                        
            } 
        }           
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUT::Update()
* @brief      Update
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUT::Update()
{
  for(XDWORD c=0; c<elements.GetSize(); c++)
    {
      if(ui_skin) 
        {
          ui_skin->Draw(elements.Get(c));
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT* element, XCHAR* nameelement, UI_ELEMENT_TYPE type)
* @brief      Elements get
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  nameelement : Nameelement pointer to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT* element, XCHAR* nameelement, UI_ELEMENT_TYPE type)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement)
        {
          if(!subelement->GetName()->Compare(nameelement, true)) 
            {
              if(type != UI_ELEMENT_TYPE_UNKNOWN) 
                {
                  if(subelement->GetType() == type) return subelement;
                }
               else return subelement;
            }

          if(subelement->GetComposeElements()->GetSize()) 
            {
              UI_ELEMENT* _subelement = Elements_Get(subelement, nameelement, type);
              if(_subelement) return _subelement;
            }
        }          
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT* element, UI_ELEMENT_CHROMEROLE chromerole)
* @brief      Elements get
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  chromerole : Chromerole value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_LAYOUT::Elements_Get(UI_ELEMENT* element, UI_ELEMENT_CHROMEROLE chromerole)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement)
        {
          if(subelement->GetChromeRole() == chromerole) return subelement;

          if(subelement->GetComposeElements()->GetSize()) 
            {
              UI_ELEMENT* _subelement = Elements_Get(subelement, chromerole);
              if(_subelement) return _subelement;
            }
        }          
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUT::Clean()
{
  ui_skin           = NULL;
  ownskin           = true;
  stylesheet        = NULL;
  designwidth       = 0;
  designheight      = 0;
  uiscale           = UI_LAYOUT_UISCALE_DEFAULT;
  uiscale_enabled   = false;
  uiscale_autofit   = false;
  minhitsize        = UI_LAYOUT_MINHITSIZE_DEFAULT;
  uiscale_offset_x  = 0.0;
  uiscale_offset_y  = 0.0;
  designcanvas      = NULL;
}



