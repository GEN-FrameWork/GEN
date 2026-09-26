/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGTransform.h
* 
* @class      GRPVECTORFILESVGTRANSFORM
* @brief      Graphic Vector File SVG Transform class (2x3 affine matrix)
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

//
//  SVG affine matrix layout:    [ a  c  e ]
//                               [ b  d  f ]
//                               [ 0  0  1 ]
//
//  x' = a*x + c*y + e
//  y' = b*x + d*y + f
//

class GRPVECTORFILESVGTRANSFORM
{
  public:
                                    GRPVECTORFILESVGTRANSFORM  ();
    virtual                        ~GRPVECTORFILESVGTRANSFORM  ();

    void                            SetIdentity                ();
    bool                            IsIdentity                 ();

    void                            Set                        (double a, double b, double c, double d, double e, double f);

    double                          GetA                       ();
    double                          GetB                       ();
    double                          GetC                       ();
    double                          GetD                       ();
    double                          GetE                       ();
    double                          GetF                       ();

    void                            CopyFrom                   (GRPVECTORFILESVGTRANSFORM& transform);

    void                            Multiply                   (GRPVECTORFILESVGTRANSFORM& transform);

    void                            ApplyToPoint               (double& x, double& y);

    bool                            ParseFromString            (XCHAR* string);

  private:

    bool                            ParsePrimitive             (XSTRING& name, XSTRING& args);
    bool                            ExtractNumbers             (XSTRING& args, double* values, int maxvalues, int& nvalues);

    void                            Clean                      ();

    double                          a;
    double                          b;
    double                          c;
    double                          d;
    double                          e;
    double                          f;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




