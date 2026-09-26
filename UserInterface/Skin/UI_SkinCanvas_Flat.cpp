/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_SkinCanvas_Flat.cpp
* 
* @class      UI_SKINCANVAS_FLAT
* @brief      User Interface Skin Canvas class
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

#include "UI_SkinCanvas_Flat.h"

#include "XRect.h"

#include "GRP2DCanvas.h"
#include "GRPViewPort.h"
#include "GRPScreen.h"

#include "UI_Element.h"
#include "UI_Element_Scroll.h"
#include "UI_Element_Text.h"
#include "UI_Element_Image.h"
#include "UI_Element_Animation.h"
#include "UI_Element_Option.h"
#include "UI_Element_Button.h"
#include "UI_Element_CheckBox.h"
#include "UI_Element_EditText.h"
#include "UI_Element_Form.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS_FLAT::UI_SKINCANVAS_FLAT(GRPSCREEN* screen, int viewportindex) : UI_SKINCANVAS(screen, viewportindex)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS_FLAT::UI_SKINCANVAS_FLAT(GRPSCREEN* screen, int viewportindex) : UI_SKINCANVAS(screen, viewportindex)
{ 
  Clean();  

  this->name    = UI_SKINCANVAS_NAME_FLAT;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS_FLAT::~UI_SKINCANVAS_FLAT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS_FLAT::~UI_SKINCANVAS_FLAT()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS_FLAT::Draw_Form(UI_ELEMENT* element)
* @brief      Draw form
* @note       Box-model parity with UI_SKINCANVAS::Draw_Form (box-shadow, background-color/border-color/
*             border-width, uniform and per-corner border-radius), reusing the same shared helpers
*             (DrawElementBoxShadow / AppendRoundRectPathPerCorner) so a stylesheet renders identically
*             whichever skin ("" or "FLAT") is selected in the layout's \<skin\> node. The Flat skin still draws
*             the plain-rectangle fallback when none of these CSS properties were authored, so pre-existing
*             Flat layouts are pixel-identical to before this change.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS_FLAT::Draw_Form(UI_ELEMENT* element)
{
  if(!screen)  return false;
  if(!element) return false;

  UI_ELEMENT_FORM*    element_form = (UI_ELEMENT_FORM*)element;
  double              x_position   = 0.0f;
  double              y_position   = 0.0f;
  GRP2DCANVAS*          canvas       = GetCanvas();
  XRECT               clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      // Box-shadow, drawn before the fill so the form paints on top of it. No-op when box-shadow was not
      // authored, so this is zero behaviour change for every pre-existing Flat layout.
      DrawElementBoxShadow(canvas, element, x_position, y_position);

      // Fill: prefer background-color/bckgrdcolor when authored, else fall back to the historical "color"
      // property -- same precedence as UI_SKINCANVAS::Draw_Form, so a stylesheet does not have to know which
      // skin is active. No legacy-fill trace here: Flat already drew straight from "color" before this change,
      // so warning about it would flag every untouched Flat layout instead of only genuinely CSS-authored ones.
      UI_COLOR* fillsrc = element_form->IsBackgroundColorSet() ? element_form->GetBackgroundColor()
                                                               : element_form->GetColor();

      GRP2DCOLOR_RGBA8    color(fillsrc->GetRed(), fillsrc->GetGreen(), fillsrc->GetBlue(), fillsrc->GetAlpha());

      // Border: prefer base-level border-color when authored, else fall back to the historical per-type
      // linecolor member -- same precedence as UI_SKINCANVAS::Draw_Form.
      UI_COLOR* bcsrc = element_form->IsBorderColorSet() ? element_form->GetBorderColor()
                                                         : element_form->GetLineColor();

      GRP2DCOLOR_RGBA8    linecolor(bcsrc->GetRed(), bcsrc->GetGreen(), bcsrc->GetBlue(), bcsrc->GetAlpha());

      canvas->SetLineColor(&linecolor);
      canvas->SetFillColor(&color);

      // border-width: -1 = default (historical 0.5f stroke, kept as Flat's own default), 0 = no stroke at all
      // (transparent line color, since Rectangle/RoundRect always draw a stroke alongside the fill), >0 =
      // author-specified thickness.
      double bw = element_form->GetBorderWidth();

      if(bw == 0.0)
        {
          GRP2DCOLOR_RGBA8  linecolor_none(0, 0, 0, 0);
          canvas->SetLineColor(&linecolor_none);
          canvas->SetLineWidth(0.5f);
        }
       else
        {
          canvas->SetLineWidth((bw < 0.0) ? 0.5f : bw);
        }

      // NOTE on Y order: GetVisibleRect()->y is the BOTTOM edge (larger Y, y-down screen coords) and GetTop()
      // is the TOP edge (smaller Y) -- same two values UI_SKINCANVAS::Draw_Form reads off the same rect.
      // AppendRoundRectPathPerCorner requires them pre-normalized (miny <= maxy); RoundRect/Rectangle are
      // called with the historical (x, y=bottom, x+width, top) order instead, matching every other call site.
      double vr_minx = element_form->GetVisibleRect()->x;
      double vr_maxx = element_form->GetVisibleRect()->x + element_form->GetVisibleRect()->width;
      double vr_bottomy = element_form->GetVisibleRect()->y;
      double vr_topy    = element_form->GetVisibleRect()->GetTop();

      // border-radius: per-corner path when any corner was authored, uniform RoundRect when only the legacy
      // "roundrect" attribute is set, plain Rectangle otherwise -- same precedence as UI_SKINCANVAS::Draw_Form.
      if(element_form->HasAnyPerCornerRadius())
        {
          double rTL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TL);
          double rTR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TR);
          double rBR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BR);
          double rBL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BL);

          GRP2DPATH path;
          AppendRoundRectPathPerCorner(path, vr_minx, vr_topy, vr_maxx, vr_bottomy, rTL, rTR, rBR, rBL);

          canvas->Path(path, true);       // fill
          canvas->Path(path, false);      // stroke (transparent line colour when border-width == 0)
        }
       else if(element_form->GetRoundRect())
        {
          canvas->RoundRect(vr_minx, vr_bottomy, vr_maxx, vr_topy, element_form->GetRoundRect(), true);
        }
       else
        {
          canvas->Rectangle(vr_minx, vr_bottomy, vr_maxx, vr_topy, true);
        }
     }

  for(XDWORD c=0; c<element_form->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
      if(subelement)
        {
          Draw(subelement);
        }
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS_FLAT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_FLAT::Clean()
{
  
}



