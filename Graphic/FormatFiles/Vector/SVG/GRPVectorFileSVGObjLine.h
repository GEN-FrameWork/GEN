/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjLine.h
* 
* @class      GRPVECTORFILESVGOBJLINE
* @brief      Graphic Vector File SVG Object Line class
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

#include "GRPVectorFileSVGObj.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLELEMENT;
class GRP2DPATH;


class GRPVECTORFILESVGOBJLINE : public GRPVECTORFILESVGOBJ
{
  public:
                                    GRPVECTORFILESVGOBJLINE     ();
    virtual                        ~GRPVECTORFILESVGOBJLINE     ();

    bool                            ApplyData                  (XFILEXMLELEMENT* element);
    bool                            BuildPath                  (GRP2DPATH& path);

    double                          GetX1                      ();
    double                          GetY1                      ();
    double                          GetX2                      ();
    double                          GetY2                      ();

  private:

    void                            Clean                      ();

    double                          x1;
    double                          y1;
    double                          x2;
    double                          y2;
};


