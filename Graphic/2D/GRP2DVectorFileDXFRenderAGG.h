/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DVectorFileDXFRenderAGG.h
* 
* @class      GRP2DVECTORFILEDXFRENDERAGG
* @brief      Graphic Vector File DXF Render (AGG) class : walks the DXF entities stream and paints it on a canvas
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
#include "XVector.h"

#include "GRP2DColor.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRP2DVECTORFILEDXFRENDERAGG_DEFAULTLINEWIDTH    1.0                       // device stroke width (pixels)
#define GRP2DVECTORFILEDXFRENDERAGG_ARCTOLERANCE        0.35                      // max chord error (device pixels) for arc/circle tessellation
#define GRP2DVECTORFILEDXFRENDERAGG_MINSEGMENTS         8                         // minimum segments per full revolution
#define GRP2DVECTORFILEDXFRENDERAGG_MAXSEGMENTS         512                       // maximum segments per full revolution
#define GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINESEGMENTS   4096                      // maximum line segments to tessellate a SPLINE
#define GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE     32                        // de Boor scratch bound (DXF splines are degree 2-3)
#define GRP2DVECTORFILEDXFRENDERAGG_USERECURSIONGUARD   64                        // POLYLINE -> VERTEX scan safety bound
#define GRP2DVECTORFILEDXFRENDERAGG_MAXINSERTDEPTH      16                        // nested INSERT (block in block) recursion bound

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;
class GRP2DPATH;
class GRPVECTORFILEDXF;
class GRPVECTORFILEDXFENTITY;
class GRPVECTORFILEDXFTEXTSECTIONENTITIES;

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class GRP2DVECTORFILEDXFRENDERAGG
{
  public:
                                    GRP2DVECTORFILEDXFRENDERAGG  ();
    virtual                        ~GRP2DVECTORFILEDXFRENDERAGG  ();

    bool                            Render                     (GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas);
    bool                            Render                     (GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight);

    bool                            GetBackgroundIsDark        ();
    void                            SetBackgroundIsDark        (bool isdark);

    bool                            GetForceColorActive        ();
    void                            SetForceColor              (bool active, GRP2DCOLOR_RGBA8 color);

    double                          GetLineWidth               ();
    void                            SetLineWidth               (double linewidth);

    bool                            GetDrawText                ();
    void                            SetDrawText                (bool drawtext);

  private:

    bool                            ComputeExtents             (GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, GRPVECTORFILEDXF* dxf, double& minx, double& miny, double& maxx, double& maxy, int depth);
    bool                            CalculateFitTransform      (GRPVECTORFILEDXF* dxf, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, double targetx, double targety, double targetwidth, double targetheight);

    void                            MapPoint                   (double ux, double uy, double& dx, double& dy);

    bool                            RenderEntityList           (GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities);
    bool                            RenderEntity               (GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index);

    bool                            RenderLine                 (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderCircle               (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderArc                  (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderEllipse              (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderLWPolyLine           (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderPolyLine             (GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderPoint                (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderText                 (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderMText                (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            Render3DFace               (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderSpline               (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderSolid                (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderHatch                (GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas);
    bool                            RenderInsert               (GRPVECTORFILEDXFENTITY* entity);
    bool                            RenderDimension            (GRPVECTORFILEDXFENTITY* entity);

    GRPVECTORFILEDXFTEXTSECTIONENTITIES* GetBlockEntities      (GRPVECTORFILEDXFENTITY* entity, double& basex, double& basey);
    void                            BuildInsertCellTransform   (double cellx, double celly, double sx, double sy, double cosr, double sinr, double basex, double basey, double& a, double& b, double& c, double& d, double& e, double& f);

    int                             FindBSplineSpan            (XVECTOR<double>& knots, int ncontrolpoints, int degree, double u);
    void                            EvalBSplinePoint           (XVECTOR<double>& knots, XVECTOR<double>& cpx, XVECTOR<double>& cpy, int degree, int span, double u, double& outx, double& outy);

    void                            EmitArcWorld               (GRP2DPATH& devicepath, double cx, double cy, double radius, double startangle, double endangle, bool moveto);
    void                            EmitBulgeWorld             (GRP2DPATH& devicepath, double x0, double y0, double x1, double y1, double bulge);
    int                             SegmentsForArc             (double radius, double sweepradians);

    bool                            GetValueDouble             (GRPVECTORFILEDXFENTITY* entity, int groupcode, double& out);
    bool                            GetValueInt                (GRPVECTORFILEDXFENTITY* entity, int groupcode, int& out);
    XCHAR*                          GetValueString             (GRPVECTORFILEDXFENTITY* entity, int groupcode);

    GRP2DCOLOR_RGBA8                ResolveColor               (GRPVECTORFILEDXFENTITY* entity);

    void                            BuildLayerTable            (GRPVECTORFILEDXF* dxf);
    int                             LayerColorACI              (XCHAR* layername);
    XCHAR*                          GetLayerName               (GRPVECTORFILEDXFENTITY* entity);
    void                            CleanLayerTable            ();
    GRP2DCOLOR_RGBA8                ColorFromACI               (int aci);

    void                            Clean                      ();

    GRP2DCANVAS*                    contextcanvas;
    GRPVECTORFILEDXF*               contextdxf;
    int                             insertdepth;

    double                          scale;
    double                          offsetx;
    double                          offsety;

    double                          blocka;
    double                          blockb;
    double                          blockc;
    double                          blockd;
    double                          blocke;
    double                          blockf;

    double                          linewidth;
    bool                            backgroundisdark;
    bool                            drawtext;

    bool                            forcecoloractive;
    GRP2DCOLOR_RGBA8                forcecolor;

    GRPVECTORFILEDXF*               layertabledxf;
    XVECTOR<XSTRING*>               layernames;
    XVECTOR<int>                    layeracis;
};
