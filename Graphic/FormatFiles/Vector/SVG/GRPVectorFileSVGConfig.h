/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGConfig.h
* 
* @class      GRPVECTORFILESVGCONFIG
* @brief      Graphic Vector File SVG Config class
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

#include "GRPVectorFileConfig.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLELEMENT;

class GRPVECTORFILESVGCONFIG : public GRPVECTORFILECONFIG
{
  public:
                                    GRPVECTORFILESVGCONFIG     ();
    virtual                        ~GRPVECTORFILESVGCONFIG     ();

    bool                            ApplyData                  (XFILEXMLELEMENT* elementsvg);

    double                          GetWidth                   ();
    void                            SetWidth                   (double width);

    double                          GetHeight                  ();
    void                            SetHeight                  (double height);

    bool                            HasViewBox                 ();
    void                            SetHasViewBox              (bool hasviewbox);

    double                          GetViewBoxMinX             ();
    double                          GetViewBoxMinY             ();
    double                          GetViewBoxWidth            ();
    double                          GetViewBoxHeight           ();
    void                            SetViewBox                 (double minx, double miny, double width, double height);

    XSTRING*                        GetPreserveAspectRatio     ();

  private:

    void                            Clean                      ();

    double                          width;
    double                          height;

    bool                            hasviewbox;
    double                          viewboxminx;
    double                          viewboxminy;
    double                          viewboxwidth;
    double                          viewboxheight;

    XSTRING                         preserveaspectratio;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




