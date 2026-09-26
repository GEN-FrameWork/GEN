/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DPath.h
* 
* @class      GRP2DPATH
* @brief      Graphics 2D Path class
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

#include "XVector.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum GRP2DPATHSEGMENTTYPE
{
  GRP2DPATHSEGMENTTYPE_UNKNOWN     = 0 ,
  GRP2DPATHSEGMENTTYPE_MOVETO          ,                                       // Move to (x , y).
  GRP2DPATHSEGMENTTYPE_LINETO          ,                                       // Line to (x , y).
  GRP2DPATHSEGMENTTYPE_CURVETO         ,                                       // Cubic   bezier (c1 , c2 , end).
  GRP2DPATHSEGMENTTYPE_QUADTO          ,                                       // Quadratic bezier (c1 , end).
  GRP2DPATHSEGMENTTYPE_ARCTO           ,                                       // Elliptical arc (rx , ry , xrot , largearc , sweep , end).
  GRP2DPATHSEGMENTTYPE_CLOSE           ,                                       // Close current sub path.
};

enum GRP2DPATHFILLRULE
{
  GRP2DPATHFILLRULE_NONZERO        = 0 ,                                       // Non zero  fill rule (default).
  GRP2DPATHFILLRULE_EVENODD            ,                                       // Even odd  fill rule.
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DPATHSEGMENT
{
  public:
                                 GRP2DPATHSEGMENT     ();
    virtual                     ~GRP2DPATHSEGMENT     ();

    GRP2DPATHSEGMENTTYPE         type;

    double                       x;
    double                       y;

    double                       c1x;
    double                       c1y;
    double                       c2x;
    double                       c2y;

    double                       rx;
    double                       ry;
    double                       xrot;
    bool                         largearc;
    bool                         sweep;

  private:

    void                         Clean                ();
};

class GRP2DPATH
{
  public:
                                 GRP2DPATH            ();
    virtual                     ~GRP2DPATH            ();

    bool                         MoveTo               (double x, double y);
    bool                         LineTo               (double x, double y);
    bool                         CurveTo              (double c1x, double c1y, double c2x, double c2y, double x, double y);
    bool                         QuadTo               (double c1x, double c1y, double x, double y);
    bool                         ArcTo                (double rx, double ry, double xrot, bool largearc, bool sweep, double x, double y);
    bool                         Close                ();

    XDWORD                       GetSize              ();
    GRP2DPATHSEGMENT*            Get                  (int index);
    bool                         IsEmpty              ();
    bool                         DeleteAll            ();

    GRP2DPATHFILLRULE            GetFillRule          ();
    void                         SetFillRule          (GRP2DPATHFILLRULE fillrule);

  private:

    bool                         AddSegment           (GRP2DPATHSEGMENT* segment);

    void                         Clean                ();

    XVECTOR<GRP2DPATHSEGMENT*>   segments;
    GRP2DPATHFILLRULE            fillrule;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




