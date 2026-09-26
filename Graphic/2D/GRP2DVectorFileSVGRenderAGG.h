/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DVectorFileSVGRenderAGG.h
* 
* @class      GRP2DVECTORFILESVGRENDERAGG
* @brief      Graphic Vector File SVG Render (AGG) class : walks the SVG object tree and paints it on a canvas
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

#include "GRPVectorFileSVGStyle.h"
#include "GRPVectorFileSVGTransform.h"
#include "GRPVectorFileSVGObjText.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;
class GRP2DPATH;
class GRPVECTORFILESVG;
class GRPVECTORFILESVGOBJ;

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class GRP2DVECTORFILESVGRENDERAGG
{
  public:
                                    GRP2DVECTORFILESVGRENDERAGG   ();
    virtual                        ~GRP2DVECTORFILESVGRENDERAGG   ();

    bool                            Render                     (GRPVECTORFILESVG* svg, GRP2DCANVAS* canvas);
    bool                            Render                     (GRPVECTORFILESVG* svg, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight);

  private:

    bool                            CalculateViewBoxTransform  (GRPVECTORFILESVG* svg, double targetx, double targety, double targetwidth, double targetheight, GRPVECTORFILESVGTRANSFORM& transform);

    bool                            RenderNode                 (GRPVECTORFILESVGOBJ* obj, GRPVECTORFILESVGTRANSFORM& parenttransform, GRPVECTORFILESVGSTYLE& parentstyle, GRP2DCANVAS* canvas);
    bool                            RenderNodeContent          (GRPVECTORFILESVGOBJ* obj, GRPVECTORFILESVGTRANSFORM& nodetransform, GRPVECTORFILESVGSTYLE& nodestyle, GRP2DCANVAS* canvas);
    bool                            ComputeClipDeviceBox       (GRPVECTORFILESVGOBJ* clipobj, GRPVECTORFILESVGTRANSFORM& transform, double& x1, double& y1, double& x2, double& y2);
    void                            AccumulateClipShape        (GRPVECTORFILESVGOBJ* shape, GRPVECTORFILESVGTRANSFORM& transform, int depth, bool& found, double& x1, double& y1, double& x2, double& y2);
    bool                            RenderShape                (GRPVECTORFILESVGOBJ* obj, GRPVECTORFILESVGTRANSFORM& transform, GRPVECTORFILESVGSTYLE& style, GRP2DCANVAS* canvas);
    bool                            RenderText                 (GRPVECTORFILESVGOBJ* obj, GRPVECTORFILESVGTRANSFORM& transform, GRPVECTORFILESVGSTYLE& style, GRP2DCANVAS* canvas);
    double                          DrawTextRun                (XSTRING* text, GRPVECTORFILESVGSTYLE& style, double sizeuser, double penx, double peny, GRPVECTORFILESVGTEXTANCHOR anchor, GRPVECTORFILESVGTRANSFORM& transform, double scale, GRP2DCANVAS* canvas);

    bool                            RenderGradient             (GRP2DPATH& devicepath, XSTRING* paintid, double globalalpha, bool isstroke, double linewidth, bool evenodd, GRPVECTORFILESVGTRANSFORM& transform, double bbx, double bby, double bbw, double bbh, GRP2DCANVAS* canvas);
    void                            ComputePathBBox            (GRP2DPATH& path, double& minx, double& miny, double& width, double& height);

    void                            TransformPath              (GRP2DPATH& path, GRPVECTORFILESVGTRANSFORM& transform);
    double                          GetScaleFactor             (GRPVECTORFILESVGTRANSFORM& transform);

    void                            Clean                      ();

    GRPVECTORFILESVG*               contextsvg;
    int                             usedepth;
};




