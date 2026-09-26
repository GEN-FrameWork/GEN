/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DVectorFileRenderAGG.h
* 
* @class      GRP2DVECTORFILERENDERAGG
* @brief      Graphic Vector File Render (AGG) class : common front end that inspects the type of a GRPVECTORFILE and
*             dispatches the rendering to the matching backend (GRP2DVECTORFILEDXFRENDERAGG or GRP2DVECTORFILESVGRENDERAGG)
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

#include "XPath.h"

#include "GRP2DColor.h"

#ifdef GRP_VECTOR_FILE_SVG_ACTIVE
#include "GRP2DVectorFileSVGRenderAGG.h"
#endif

#ifdef GRP_VECTOR_FILE_DXF_ACTIVE
#include "GRP2DVectorFileDXFRenderAGG.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;
class GRPBITMAP;
class GRPVECTORFILE;

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class GRP2DVECTORFILERENDERAGG
{
  public:
                                    GRP2DVECTORFILERENDERAGG     ();
    virtual                        ~GRP2DVECTORFILERENDERAGG     ();

    bool                            Render                       (GRPVECTORFILE* file, GRP2DCANVAS* canvas);
    bool                            Render                       (GRPVECTORFILE* file, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight);

    bool                            RenderCached                 (GRPVECTORFILE* file, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight);
    void                            InvalidateCache              ();

    bool                            GetBackgroundIsDark          ();
    void                            SetBackgroundIsDark          (bool isdark);

    bool                            GetForceColorActive          ();
    void                            SetForceColor                (bool active, GRP2DCOLOR_RGBA8 color);

    double                          GetLineWidth                 ();
    void                            SetLineWidth                 (double linewidth);

    bool                            GetDrawText                  ();
    void                            SetDrawText                  (bool drawtext);

    #ifdef GRP_VECTOR_FILE_SVG_ACTIVE
    GRP2DVECTORFILESVGRENDERAGG*    GetSVGRender                 ();
    #endif

    #ifdef GRP_VECTOR_FILE_DXF_ACTIVE
    GRP2DVECTORFILEDXFRENDERAGG*    GetDXFRender                 ();
    #endif

  private:

    bool                            BuildCacheBitmap             (GRPVECTORFILE* file, GRP2DCANVAS* referencecanvas, double width, double height);
    void                            Clean                        ();

    GRPBITMAP*                      cachebitmap;
    bool                            cachevalid;
    GRPVECTORFILE*                  cachefile;
    int                             cachefiletype;
    double                          cachewidth;
    double                          cacheheight;
    XPATH                           cachevectorfontpathfile;
    bool                            cachevectorkerning;

    #ifdef GRP_VECTOR_FILE_SVG_ACTIVE
    GRP2DVECTORFILESVGRENDERAGG     svgrender;
    #endif

    #ifdef GRP_VECTOR_FILE_DXF_ACTIVE
    GRP2DVECTORFILEDXFRENDERAGG     dxfrender;
    #endif
};
