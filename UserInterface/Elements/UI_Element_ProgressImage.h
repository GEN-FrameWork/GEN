/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_ProgressImage.h
* 
* @class      UI_ELEMENT_PROGRESS_IMAGE
* @brief      User Interface Element Progress Image class (two-graphic clip-reveal progress indicator)
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

class GRPBITMAP;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/**
* @class   UI_ELEMENT_PROGRESS_IMAGE
* @brief   Two-graphic clip-reveal progress indicator.
*
* Draws an "empty" (0%) graphic in full, then draws a "full" (100%) graphic clipped to the current level. The clip
* direction follows the element direction: HORIZONTAL reveals from the left by width; VERTICAL reveals from the
* bottom by height. Both graphics are expected to share the same dimensions. They are loaded through the same
* animation/bitmap cache as UI_ELEMENT_IMAGE, so any format that cache supports (bitmap now, vector once rasterized)
* works here. The bitmap pointers are owned by the cache, not by this element.
*
* XML:
*   imageempty="<file>"  imagefull="<file>"  level="0..100"  direction="horizontal|vertical"  alpha="0..100"
*   optional child &lt;text&gt; => centered caption (Get_UIText()).
*/
class UI_ELEMENT_PROGRESS_IMAGE : public UI_ELEMENT_OPTION
{
  public:
                            UI_ELEMENT_PROGRESS_IMAGE   ();
    virtual                ~UI_ELEMENT_PROGRESS_IMAGE   ();

    GRPBITMAP*              GetImageEmpty               ();
    void                    SetImageEmpty               (GRPBITMAP* image);

    GRPBITMAP*              GetImageFull                ();
    void                    SetImageFull                (GRPBITMAP* image);

    double                  GetOffsetStart              ();
    void                    SetOffsetStart              (double offsetstart);

    double                  GetOffsetEnd                ();
    void                    SetOffsetEnd                (double offsetend);

    float                   GetLevel                    ();
    void                    SetLevel                    (float level);

    XBYTE                   GetAlpha                    ();
    void                    SetAlpha                    (XBYTE alpha);

  private:

    void                    Clean                       ();

    GRPBITMAP*              imageempty;
    GRPBITMAP*              imagefull;
    double                  offsetstart;
    double                  offsetend;
    float                   level;
    XBYTE                   alpha;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
