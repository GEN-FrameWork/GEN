/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjText.h
* 
* @class      GRPVECTORFILESVGOBJTEXT
* @brief      Graphic Vector File SVG Object Text class
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

#include "GRPVectorFileSVGObj.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPVECTORFILESVGTEXTANCHOR
{
  GRPVECTORFILESVGTEXTANCHOR_START    = 0 ,
  GRPVECTORFILESVGTEXTANCHOR_MIDDLE       ,
  GRPVECTORFILESVGTEXTANCHOR_END          ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLELEMENT;

class GRPVECTORFILESVGOBJTEXT : public GRPVECTORFILESVGOBJ
{
  public:
                                    GRPVECTORFILESVGOBJTEXT     ();
    virtual                        ~GRPVECTORFILESVGOBJTEXT     ();

    bool                            ApplyData                  (XFILEXMLELEMENT* element);

    XSTRING*                        GetText                    ();

    double                          GetX                       ();
    double                          GetY                       ();
    bool                            HasX                       ();
    bool                            HasY                       ();
    double                          GetDX                      ();
    double                          GetDY                      ();
    double                          GetFontSize                ();
    bool                            HasFontSize                ();
    GRPVECTORFILESVGTEXTANCHOR      GetTextAnchor              ();

  private:

    static bool                     GetProperty                (XFILEXMLELEMENT* element, XCHAR* name, XSTRING& outvalue);

    void                            Clean                      ();

    XSTRING                         text;
    double                          x;
    double                          y;
    bool                            hasx;
    bool                            hasy;
    double                          dx;
    double                          dy;
    double                          fontsize;
    bool                            hasfontsize;
    GRPVECTORFILESVGTEXTANCHOR      textanchor;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




