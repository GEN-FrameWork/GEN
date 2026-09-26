/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartStyle.h
* 
* @class      GRPSTATISTICSCHARTCOLOR
* @brief      Graphic Statistics Chart : format independent style value types (color, fill / stroke style and text style)
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPSTATISTICSCHARTTEXTANCHOR
{
  GRPSTATISTICSCHARTTEXTANCHOR_START               = 0 ,                     // anchor at the left of the text
  GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE              ,                         // anchor at the center of the text
  GRPSTATISTICSCHARTTEXTANCHOR_END                 ,                         // anchor at the right of the text
};


enum GRPSTATISTICSCHARTLEGENDPOSITION
{
  GRPSTATISTICSCHARTLEGENDPOSITION_TOP             = 0 ,                     // legend band above the plot
  GRPSTATISTICSCHARTLEGENDPOSITION_BOTTOM         ,                          // legend band below the plot
  GRPSTATISTICSCHARTLEGENDPOSITION_LEFT           ,                          // legend column at the left of the plot
  GRPSTATISTICSCHARTLEGENDPOSITION_RIGHT          ,                          // legend column at the right of the plot
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct GRPSTATISTICSCHARTCOLOR
{
    XBYTE                           r;
    XBYTE                           g;
    XBYTE                           b;
    XBYTE                           a;

                                    GRPSTATISTICSCHARTCOLOR               ()                                              { r = 0;   g = 0;   b = 0;   a = 255;                   }
                                    GRPSTATISTICSCHARTCOLOR               (XBYTE r, XBYTE g, XBYTE b)                     { this->r = r; this->g = g; this->b = b; a = 255;       }
                                    GRPSTATISTICSCHARTCOLOR               (XBYTE r, XBYTE g, XBYTE b, XBYTE a)            { this->r = r; this->g = g; this->b = b; this->a = a;   }

    void                            Set                                   (XBYTE r, XBYTE g, XBYTE b, XBYTE a = 255)      { this->r = r; this->g = g; this->b = b; this->a = a;   }
};

struct GRPSTATISTICSCHARTSTYLE
{
    bool                            hasfill;
    GRPSTATISTICSCHARTCOLOR         fillcolor;

    bool                            hasstroke;
    GRPSTATISTICSCHARTCOLOR         strokecolor;
    double                          strokewidth;

                                    GRPSTATISTICSCHARTSTYLE               ()                                              { hasfill = false; hasstroke = false; strokewidth = 1.0;  }

    void                            SetFill                               (GRPSTATISTICSCHARTCOLOR& color)                { fillcolor   = color; hasfill   = true;                }
    void                            SetStroke                             (GRPSTATISTICSCHARTCOLOR& color, double width = 1.0) { strokecolor = color; strokewidth = width; hasstroke = true;  }
};



struct GRPSTATISTICSCHARTTEXTSTYLE
{
    GRPSTATISTICSCHARTCOLOR         color;
    double                          size;
    GRPSTATISTICSCHARTTEXTANCHOR    anchor;
    double                          rotation;
    bool                            bold;

                                    GRPSTATISTICSCHARTTEXTSTYLE           ()                                              { size = 12.0; anchor = GRPSTATISTICSCHARTTEXTANCHOR_START; rotation = 0.0; bold = false;  }
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




