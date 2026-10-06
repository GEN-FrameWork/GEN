/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPViewPort.h
* 
* @class      GRPVIEWPORT
* @brief      Graphics Viewport class
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

#include "GRPProperties.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPVIEWPORT_PROJECTIONTYPE
{
  GRPVIEWPORT_PROJECTIONTYPE_UNKNOWN   = 0 ,
  GRPVIEWPORT_PROJECTIONTYPE_ORTHO         ,
  GRPVIEWPORT_PROJECTIONTYPE_FRUSTUM       ,
  GRPVIEWPORT_PROJECTIONTYPE_PIXELS        ,
};

#define GRPVIEWPORT_ID_MAIN   _L("mainviewport")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;

class GRPVIEWPORT
{
  public:
                                  GRPVIEWPORT                         ();
    virtual                      ~GRPVIEWPORT                         ();

    XSTRING*                      GetID                               ();

    bool                          IsActive                            ();
    void                          SetIsActive                         (bool active);

    GRPVIEWPORT_PROJECTIONTYPE    GetProjectionType                   ();
    void                          SetProjectionType                   (GRPVIEWPORT_PROJECTIONTYPE projectiontype);

    bool                          CreateCanvas                        (GRPPROPERTIES& canvasproperties);

    float                         GetPositionX                        ();
    float                         GetPositionY                        ();
    void                          SetPosition                         (float x, float y);

    float                         GetWidth                            ();
    float                         GetHeight                           ();

    void                          SetSize                             (float width, float height);

    float                         GetMinWidth                         ();
    float                         GetMinHeight                        ();
    void                          SetMinSize                          (float minwidth, float minheight);

    float                         GetMaxWidth                         ();
    float                         GetMaxHeight                        ();
    void                          SetMaxSize                          (float maxwidth, float maxheight);

    float                         GetCanvasPositionX                  ();
    float                         GetCanvasPositionY                  ();
    bool                          SetCanvasPosition                   (float x, float y);

    GRP2DCANVAS*                  GetCanvas                           ();

  protected:

    XSTRING                       ID;
    bool                          active;
    GRPVIEWPORT_PROJECTIONTYPE    projectiontype;

    float                         x;
    float                         y;
    float                         width;
    float                         height;






    float                         minwidth;
    float                         minheight;
    float                         maxwidth;
    float                         maxheight;


    float                         canvas_x;
    float                         canvas_y;
    GRP2DCANVAS*                  canvas;
    
  private:

    void                          Clean                               ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





