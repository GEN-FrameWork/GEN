/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Form.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Property_Scrolleable.h"
#include "UI_Element.h"

class GRPBITMAP;    // forward declaration only -- keeps this header decoupled from the graphics backend, same
                     // pattern already used by UI_ELEMENT_IMAGE (UI_Element_Image.h) for its own bitmap member.

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_ELEMENT_FORM : public UI_ELEMENT, public UI_PROPERTY_SCROLLEABLE
{
  public:
																		  UI_ELEMENT_FORM					    ();
    virtual													 ~UI_ELEMENT_FORM					    ();

	  UI_COLOR*													GetLineColor								();
    UI_BOUNDARYLINE*                  GetVisibleRect              ();

    bool                              GetLegacyFillWarningEmitted ();
    void                              SetLegacyFillWarningEmitted (bool value);

    GRPBITMAP*                        ShadowCache_GetIfMatches    (int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color);
    void                              ShadowCache_Set             (GRPBITMAP* bitmap, int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, int blur, UI_COLOR* color);

  private:

		void															Clean												();
    void                              ShadowCache_Release         ();

    UI_COLOR                          linecolor;
    UI_BOUNDARYLINE                   visiblerect;
    XDWORD                            roundvisiblerect;
    bool                              legacy_fill_warning_emitted;

    GRPBITMAP*                        shadowcache_bitmap;
    int                               shadowcache_shape_w;
    int                               shadowcache_shape_h;
    double                            shadowcache_rTL;
    double                            shadowcache_rTR;
    double                            shadowcache_rBR;
    double                            shadowcache_rBL;
    int                               shadowcache_blur;
    XDWORD                            shadowcache_colorrgba;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





