/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Layout.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

#include "UI_Element.h"
#include "UI_Background.h"
#include "UI_Skin.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// UIScale (Opción A): authors write XML/CSS in design pixels. scale = screen/design (or zoom).
// Fase 1: API + contract — scale 1.0 identical to baseline.
// Fase 2: ScreenToDesign / DesignToScreen; input maps screen→design before IsWithin (AABB stay design px).
// Fase 3: paint via design offscreen canvas + scaled present (letterbox offsets in ScreenToDesign).
// Fase 4: runtime SetUIScale + product clamp [MIN..MAX]; demo zoom +/- in UI_System.
// Fase 5: autofit scale = min(sw/dw, sh/dh), recalc on window resize (FIT_MIN..MAX).
// Fase 6: min hit-target in design px (expand IsWithin only; paint AABB unchanged).
// Layouts WITHOUT a stylesheet (and without SetUIScaleEnabled(true)) keep the legacy path (UI_Options).
#define UI_LAYOUT_UISCALE_DEFAULT           1.0
#define UI_LAYOUT_UISCALE_MIN               0.75
#define UI_LAYOUT_UISCALE_MAX               2.0
#define UI_LAYOUT_UISCALE_FIT_MIN           0.25
#define UI_LAYOUT_UISCALE_EPSILON           0.001
#define UI_LAYOUT_UISCALE_STEP              0.25
#define UI_LAYOUT_MINHITSIZE_DEFAULT        44.0
#define UI_LAYOUT_MINHITSIZE_DISABLED       0.0

class UI_STYLESHEET;
class GRP2DCANVAS;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_LAYOUT
{
  public:
                                    UI_LAYOUT                     (UI_SKIN* ui_skin);
    virtual                        ~UI_LAYOUT                     ();

    XSTRING*                        GetNameID                     ();

    UI_SKIN*                        GetSkin                       ();
    void                            SetSkin                       (UI_SKIN* ui_skin);

    void                            SetOwnsSkin                   (bool ownskin);

    UI_BACKGROUND*                  GetBackground                 ();

    UI_STYLESHEET*                  GetStyleSheet                 ();
    void                            SetStyleSheet                 (UI_STYLESHEET* sheet);

    XDWORD                          GetDesignWidth                () const;
    XDWORD                          GetDesignHeight               () const;
    void                            SetDesignSize                 (XDWORD width, XDWORD height);

    double                          GetUIScale                    () const;
    void                            SetUIScale                    (double scale);
    void                            SetUIScaleForFit              (double scale);

    void                            SetUIScaleEnabled             (bool enabled);
    bool                            GetUIScaleEnabled             () const;

    void                            SetUIScaleAutofit             (bool autofit);
    bool                            GetUIScaleAutofit             () const;

    bool                            IsUIScaleActive               () const;

    double                          GetAssetRasterScale           () const;

    double                          ComputeFitUIScale             (XDWORD screen_w, XDWORD screen_h) const;

    double                          GetMinHitSize                 () const;
    void                            SetMinHitSize                 (double size);

    void                            ScreenToDesign                (double screen_x, double screen_y, double& design_x, double& design_y) const;
    void                            DesignToScreen                (double design_x, double design_y, double& screen_x, double& screen_y) const;

    double                          GetUIScaleOffsetX             () const;
    double                          GetUIScaleOffsetY             () const;

    void                            ComputePresentTransform       (XDWORD screen_w, XDWORD screen_h);

    bool                            NeedsScaledPresent            (XDWORD screen_w, XDWORD screen_h) const;

    GRP2DCANVAS*                    GetDesignCanvas               () const;
    void                            SetDesignCanvas               (GRP2DCANVAS* canvas);

    bool                            Elements_Add                  (UI_ELEMENT* element);
    XVECTOR<UI_ELEMENT*>*           Elements_Get                  ();
    UI_ELEMENT*                     Elements_Get                  (XCHAR* nameelement, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);    
    UI_ELEMENT*                     Elements_Get                  (UI_ELEMENT_CHROMEROLE chromerole);
    bool                            Elements_DeleteAll            ();

    bool                            Elements_SetToRedraw          (bool recursive = true);
    bool                            Elements_SetToRedraw          (UI_ELEMENT* element, bool recursive = true);

    virtual bool                    Update                        ();

  private:

    UI_ELEMENT*                     Elements_Get                  (UI_ELEMENT* element, XCHAR* nameelement, UI_ELEMENT_TYPE type);
    UI_ELEMENT*                     Elements_Get                  (UI_ELEMENT* element, UI_ELEMENT_CHROMEROLE chromerole);

    void                            Clean                         ();    

    UI_SKIN*                        ui_skin;
    bool                            ownskin;
    XSTRING                         nameID;

    UI_BACKGROUND                   background;

    XVECTOR<UI_ELEMENT*>            elements;

    UI_STYLESHEET*                  stylesheet;

    XDWORD                          designwidth;
    XDWORD                          designheight;
    double                          uiscale;
    bool                            uiscale_enabled;
    bool                            uiscale_autofit;
    double                          minhitsize;
    double                          uiscale_offset_x;
    double                          uiscale_offset_y;
    GRP2DCANVAS*                    designcanvas;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




