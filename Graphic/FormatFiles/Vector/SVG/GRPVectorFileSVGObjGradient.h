/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjGradient.h
* 
* @class      GRPVECTORFILESVGOBJGRADIENT
* @brief      Graphic Vector File SVG Object Gradient class
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

#include "XString.h"

#include "GRP2DGradientStop.h"

#include "GRPVectorFileSVGObj.h"
#include "GRPVectorFileSVGTransform.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPVECTORFILESVGGRADIENTUNITS
{
  GRPVECTORFILESVGGRADIENTUNITS_OBJECTBOUNDINGBOX   = 0 ,                       // default : coordinates are fractions (0..1) of the shape bounding box
  GRPVECTORFILESVGGRADIENTUNITS_USERSPACEONUSE          ,                       // coordinates are in user space
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLELEMENT;

class GRPVECTORFILESVGOBJGRADIENT : public GRPVECTORFILESVGOBJ
{
  public:
                                    GRPVECTORFILESVGOBJGRADIENT ();
    virtual                        ~GRPVECTORFILESVGOBJGRADIENT ();

    bool                            ApplyData                  (XFILEXMLELEMENT* element);

    bool                            IsLinear                   ();
    bool                            IsRadial                   ();

    GRPVECTORFILESVGGRADIENTUNITS   GetUnits                   ();
    GRPVECTORFILESVGTRANSFORM*      GetGradientTransform       ();

    double                          GetX1                      ();
    double                          GetY1                      ();
    double                          GetX2                      ();
    double                          GetY2                      ();

    double                          GetCX                      ();
    double                          GetCY                      ();
    double                          GetR                       ();
    double                          GetFX                      ();
    double                          GetFY                      ();

    GRP2DGRADIENTSTOP*              GetStops                   ();
    int                             GetNStops                  ();

    XSTRING*                        GetHRef                    ();

  private:

    bool                            ParseStops                 (XFILEXMLELEMENT* element);
    double                          ParseCoordinate            (XCHAR* value, double defaultvalue);

    static bool                     GetStopProperty            (XFILEXMLELEMENT* element, XCHAR* name, XSTRING& outvalue);

    void                            Clean                      ();

    GRPVECTORFILESVGGRADIENTUNITS   units;
    GRPVECTORFILESVGTRANSFORM       gradienttransform;

    double                          x1;
    double                          y1;
    double                          x2;
    double                          y2;

    double                          cx;
    double                          cy;
    double                          r;
    double                          fx;
    double                          fy;

    GRP2DGRADIENTSTOP               stops[GRP2DGRADIENT_MAXSTOPS];
    int                             nstops;

    XSTRING                         href;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




