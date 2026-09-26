/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_ProgressRadial.h
* 
* @class      UI_ELEMENT_PROGRESS_RADIAL
* @brief      User Interface Element Radial Progress class (circular / ring progress indicator)
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

#include "UI_Color.h"
#include "UI_BoundaryLine.h"
#include "UI_Element.h"
#include "UI_Element_Option.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_PROGRESS_RADIAL_DEFAULT_STARTANGLE      (-90.0)     // 12 o'clock (top of the ring)
#define UI_ELEMENT_PROGRESS_RADIAL_DEFAULT_SWEEPANGLE      (360.0)     // full ring; use e.g. 270 for an open ring
#define UI_ELEMENT_PROGRESS_RADIAL_DEFAULT_THICKNESS       (0.0)       // 0 => auto (a fraction of the radius)
#define UI_ELEMENT_PROGRESS_RADIAL_AUTOTHICKNESS_FACTOR    (0.16)      // ring width when thickness is auto (fraction of radius)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/**
* @class   UI_ELEMENT_PROGRESS_RADIAL
* @brief   Circular ring progress indicator (donut ring).
*
* COLOR MAPPING (reuses the base element color slots so the XML stays uniform):
*   - track ring (full sweep)          : GetBackgroundColor()  (XML attribute "bckgrdcolor")
*   - value arc gradient START color   : GetColor()            (XML attribute "color")
*   - value arc gradient END   color   : GetLineColor()        (XML attribute "linecolor")
*       When the line color alpha is 0 (unset), the value arc is stroked flat with GetColor().
*   - centered caption (e.g. "37%")    : the composed UI_ELEMENT_TEXT (Get_UIText(), set from a child &lt;text&gt;)
*
* GEOMETRY:
*   The progress ring occupies its boundary box. The ring is centered in that box; its radius is derived from the
*   smaller side. The arc starts at GetStartAngle() and sweeps GetSweepAngle() degrees clockwise; the value
*   arc covers that sweep scaled by GetLevel()/100.
*/
#ifndef UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL
#define UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL    0                                   // gradient maps to the current value arc (leading edge = end color)
#define UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK   1                                   // gradient maps to the full sweep; the value arc reveals part of it
#endif

class UI_ELEMENT_PROGRESS_RADIAL : public UI_ELEMENT_OPTION
{
  public:
                            UI_ELEMENT_PROGRESS_RADIAL  ();
    virtual                ~UI_ELEMENT_PROGRESS_RADIAL  ();

    float                   GetLevel                    ();
    void                    SetLevel                    (float level);

    double                  GetStartAngle               ();
    void                    SetStartAngle               (double startangle);

    double                  GetSweepAngle               ();
    void                    SetSweepAngle               (double sweepangle);

    double                  GetThickness                ();
    void                    SetThickness                (double thickness);

    bool                    GetRoundCap                 ();
    void                    SetRoundCap                 (bool roundcap);

    UI_COLOR*               GetLineColor                ();
    UI_COLOR*               GetGradientColor            ();
    int                     GetGradientMode             ();
    void                    SetGradientMode             (int gradientmode);

    virtual void            ReapplyStyleVisual          ();

  private:

    void                    Clean                       ();

    float                   level;
    double                  startangle;
    double                  sweepangle;
    double                  thickness;
    bool                    roundcap;
    UI_COLOR                linecolor;
    UI_COLOR                gradientcolor;
    int                     gradientmode;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
