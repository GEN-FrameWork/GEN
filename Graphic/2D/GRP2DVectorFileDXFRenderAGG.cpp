/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DVectorFileDXFRenderAGG.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRP2DVectorFileDXFRenderAGG.h"

#include <math.h>

#include "XVariant.h"
#include "XVector.h"

#include "GRP2DCanvas.h"
#include "GRP2DColor.h"
#include "GRP2DPath.h"

#include "GRPVectorFileDXF.h"
#include "GRPVectorFileDXFConfig.h"
#include "GRPVectorFileDXFTextSection.h"
#include "GRPVectorFileDXFTextSectionEntities.h"
#include "GRPVectorFileDXFTextSectionHeader.h"
#include "GRPVectorFileDXFTextSectionBlocks.h"
#include "GRPVectorFileDXFTextBlock.h"
#include "GRPVectorFileDXFEntity.h"
#include "GRPVectorFileDXFValue.h"
#include "XFileTXT.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRP2DVECTORFILEDXFRENDERAGG_PI          3.14159265358979323846
#define GRP2DVECTORFILEDXFRENDERAGG_2PI         6.28318530717958647692
#define GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD     0.01745329251994329577
#define GRP2DVECTORFILEDXFRENDERAGG_MTEXT_LINEFACTOR  1.66666666666666666667   // MTEXT single line spacing ~ 5/3 of the text height (tunable)



/*---- FORWARD DECLARATIONS ------------------------------------------------------------------------------------------*/


static void MTextDecode(XCHAR* raw, XVECTOR<XSTRING*>& lines);                 // defined below; declared here so ComputeExtents (above its definition) can size MTEXT



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVECTORFILEDXFRENDERAGG::GRP2DVECTORFILEDXFRENDERAGG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVECTORFILEDXFRENDERAGG::GRP2DVECTORFILEDXFRENDERAGG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVECTORFILEDXFRENDERAGG::~GRP2DVECTORFILEDXFRENDERAGG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVECTORFILEDXFRENDERAGG::~GRP2DVECTORFILEDXFRENDERAGG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::Render(GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas)
* @brief      Render : paint the DXF fitted into the whole canvas
* @ingroup    GRAPHIC
* 
* @param[in]  dxf : loaded DXF file
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::Render(GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas)
{
  if(!canvas) return false;

  return Render(dxf, canvas, 0.0, 0.0, canvas->GetWidth(), canvas->GetHeight());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::Render(GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight)
* @brief      Render : paint the DXF fitted into the target rectangle (uniform scale, centered, Y axis flipped)
* @ingroup    GRAPHIC
* 
* @param[in]  dxf : loaded DXF file
* @param[in]  canvas : target canvas
* @param[in]  targetx : target rectangle x
* @param[in]  targety : target rectangle y
* @param[in]  targetwidth : target rectangle width
* @param[in]  targetheight : target rectangle height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::Render(GRPVECTORFILEDXF* dxf, GRP2DCANVAS* canvas, double targetx, double targety, double targetwidth, double targetheight)
{
  if(!dxf || !canvas) return false;

  GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities = (GRPVECTORFILEDXFTEXTSECTIONENTITIES*)dxf->GetSection(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES);
  if(!entities) return false;

  XVECTOR<GRPVECTORFILEDXFENTITY*>* raws = entities->GetEntities();
  if(!raws || raws->IsEmpty()) return false;

  if(!CalculateFitTransform(dxf, entities, targetx, targety, targetwidth, targetheight)) return false;

  double           savedlinewidth = canvas->GetLineWidth();                    // save canvas state
  GRP2DCOLOR_RGBA8 savedlinecolor = GRP2DCOLOR_RGBA8(0, 0, 0, 255);

  contextcanvas = canvas;
  contextdxf    = dxf;
  insertdepth   = 0;

  blocka = 1.0;   blockb = 0.0;   blockc = 0.0;   blockd = 1.0;   blocke = 0.0;   blockf = 0.0;   // identity block transform

  BuildLayerTable(dxf);                                                        // resolve ByLayer colors (parsed from the TABLES/LAYER records)

  RenderEntityList(entities);

  contextcanvas = NULL;
  contextdxf    = NULL;

  canvas->SetLineWidth(savedlinewidth);                                        // restore canvas state
  (void)savedlinecolor;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::GetBackgroundIsDark()
* @brief      Get background is dark
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::GetBackgroundIsDark()
{
  return backgroundisdark;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::SetBackgroundIsDark(bool isdark)
* @brief      Set background is dark : selects how ACI 7 (black/white), ByLayer and ByBlock map to a foreground color
* @ingroup    GRAPHIC
* 
* @param[in]  isdark : true -> foreground is white ; false -> foreground is black.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::SetBackgroundIsDark(bool isdark)
{
  backgroundisdark = isdark;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::GetForceColorActive()
* @brief      Get force color active
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::GetForceColorActive()
{
  return forcecoloractive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::SetForceColor(bool active, GRP2DCOLOR_RGBA8 color)
* @brief      Set force color : ignore the per entity ACI and paint everything with one color
* @ingroup    GRAPHIC
* 
* @param[in]  active : true to enable the override
* @param[in]  color : override color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::SetForceColor(bool active, GRP2DCOLOR_RGBA8 color)
{
  forcecoloractive = active;
  forcecolor       = color;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DVECTORFILEDXFRENDERAGG::GetLineWidth()
* @brief      Get line width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DVECTORFILEDXFRENDERAGG::GetLineWidth()
{
  return linewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::SetLineWidth(double linewidth)
* @brief      Set line width : device stroke width used for every entity (DXF lineweights are not applied)
* @ingroup    GRAPHIC
* 
* @param[in]  linewidth : device stroke width (pixels)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::SetLineWidth(double linewidth)
{
  this->linewidth = (linewidth > 0.0) ? linewidth : GRP2DVECTORFILEDXFRENDERAGG_DEFAULTLINEWIDTH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::GetDrawText()
* @brief      Get draw text
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::GetDrawText()
{
  return drawtext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::SetDrawText(bool drawtext)
* @brief      Set draw text : enable TEXT / MTEXT painting (a vector font must be loaded on the canvas)
* @ingroup    GRAPHIC
* 
* @param[in]  drawtext : true to paint text entities
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::SetDrawText(bool drawtext)
{
  this->drawtext = drawtext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::ComputeExtents(GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, GRPVECTORFILEDXF* dxf, double& minx, double& miny, double& maxx, double& maxy, int depth)
* @brief      Compute extents : world space bounding box of all renderable geometry (the "zoom extents" of the drawing)
* @note       INTERNAL. Circles / arcs / ellipses contribute center +/- radius (a safe over estimate so nothing is
* @ingroup    GRAPHIC
* 
* @param[in]  entities : entities section
* @param[in]  dxf : loaded DXF file (for the header fallback)
* @param[in]  minx : bounding box min x
* @param[in]  miny : bounding box min y
* @param[in]  maxx : bounding box max x
* @param[in]  maxy : bounding box max y
* @param[in]  depth : Depth value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::ComputeExtents(GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, GRPVECTORFILEDXF* dxf, double& minx, double& miny, double& maxx, double& maxy, int depth)
{
  bool   found = false;
  double lminx = 0.0;  double lminy = 0.0;  double lmaxx = 0.0;  double lmaxy = 0.0;

  XVECTOR<GRPVECTORFILEDXFENTITY*>* raws = entities->GetEntities();

  for(XDWORD c=0; c<raws->GetSize(); c++)
    {
      GRPVECTORFILEDXFENTITY* entity = raws->Get(c);
      if(!entity) continue;

      XSTRING* name = entity->GetName();

      double px[4];   double py[4];   int npoints = 0;
      double cx = 0.0;   double cy = 0.0;   double rr = 0.0;   bool isradial = false;

      if(!name->Compare(__L("LINE"), true))
        {
          double x0; double y0; double x1; double y1;
          if(GetValueDouble(entity, 10, x0) && GetValueDouble(entity, 20, y0)) { px[npoints]=x0; py[npoints]=y0; npoints++; }
          if(GetValueDouble(entity, 11, x1) && GetValueDouble(entity, 21, y1)) { px[npoints]=x1; py[npoints]=y1; npoints++; }
        }
       else if(!name->Compare(__L("CIRCLE"), true) || !name->Compare(__L("ARC"), true))
        {
          if(GetValueDouble(entity, 10, cx) && GetValueDouble(entity, 20, cy) && GetValueDouble(entity, 40, rr))  isradial = true;
        }
       else if(!name->Compare(__L("ELLIPSE"), true))
        {
          double mx; double my;
          if(GetValueDouble(entity, 10, cx) && GetValueDouble(entity, 20, cy) && GetValueDouble(entity, 11, mx) && GetValueDouble(entity, 21, my))
            {
              rr       = sqrt((mx * mx) + (my * my));                          // major axis length (safe over estimate)
              isradial = true;
            }
        }
       else if(!name->Compare(__L("POINT"), true) || !name->Compare(__L("VERTEX"), true))
        {
          double x0; double y0;
          if(GetValueDouble(entity, 10, x0) && GetValueDouble(entity, 20, y0)) { px[npoints]=x0; py[npoints]=y0; npoints++; }
        }
       else if(!name->Compare(__L("TEXT"), true) || !name->Compare(__L("MTEXT"), true))
        {
          // TEXT / MTEXT contribute more than their insertion point : the glyph block extends to the right (and
          // downward) from the insertion and may be rotated (code 50). Ignoring that width made text anchored at the
          // drawing edge fall outside the computed extents and get clipped by the fit transform. Here we estimate the
          // un-rotated block rectangle (relative to the insertion, placed per the attachment 71), rotate its 4 corners
          // by the entity rotation and feed them into the bounding box so the fit always leaves room for the glyphs.
          double ix; double iy;
          if(GetValueDouble(entity, 10, ix) && GetValueDouble(entity, 20, iy))
            {
              double th = 1.0;   if(!GetValueDouble(entity, 40, th) || (th <= 0.0)) th = 1.0;   // text height (code 40)

              double textw = 0.0;                                                   // block width  (world units)
              double texth = th;                                                    // block height (world units)
              int    nlines = 1;
              int    attach = 1;

              if(!name->Compare(__L("MTEXT"), true))
                {
                  XSTRING raw;                                                       // assemble the full string (continuation 3* + final 1) exactly like RenderMText
                  XVECTOR<GRPVECTORFILEDXFVALUE*>* mvalues = entity->GetValues();
                  for(XDWORD v=0; (mvalues) && (v < mvalues->GetSize()); v++)
                    {
                      GRPVECTORFILEDXFVALUE* mv = mvalues->Get(v);
                      if(mv && (mv->GetType() == 3)) raw.Add((XCHAR*)(*mv->GetData()));
                    }
                  XCHAR* m1 = GetValueString(entity, 1);
                  if(m1) raw.Add(m1);

                  XVECTOR<XSTRING*> mlines;
                  if(!raw.IsEmpty()) MTextDecode(raw.Get(), mlines);

                  nlines = (int)mlines.GetSize();   if(nlines < 1) nlines = 1;

                  XDWORD longest = 0;                                                // longest decoded line (chars) for the width estimate
                  for(XDWORD l=0; l<mlines.GetSize(); l++)
                    {
                      XSTRING* s = mlines.Get(l);
                      if(s && (s->GetSize() > longest)) longest = s->GetSize();
                    }
                  for(XDWORD l=0; l<mlines.GetSize(); l++)                           // free the decoded lines
                    {
                      XSTRING* s = mlines.Get(l);
                      if(s) GEN_DELETE s;
                    }

                  double linefactor = 1.0;   double dls = 0.0;
                  if(GetValueDouble(entity, 44, dls) && (dls > 0.0)) linefactor = dls;

                  double refw = 0.0;   GetValueDouble(entity, 41, refw);             // MTEXT reference (wrap) width, when present

                  textw = (double)longest * 0.6 * th;                               // ~0.6*height per glyph advance (generous, avoids clipping)
                  if(refw > textw) textw = refw;
                  texth = (double)nlines * th * GRP2DVECTORFILEDXFRENDERAGG_MTEXT_LINEFACTOR * linefactor;

                  double dattach = 1.0;   GetValueDouble(entity, 71, dattach);   attach = (int)dattach;
                  if((attach < 1) || (attach > 9)) attach = 1;
                }
               else                                                                 // TEXT : single line, code 1 string, left/baseline anchored
                {
                  XCHAR* t1 = GetValueString(entity, 1);
                  XDWORD len = t1 ? XSTRING::GetSize(t1) : 0;
                  textw = (double)len * 0.6 * th;
                  texth = th;
                  attach = 1;                                                        // top-left equivalent box (block grows right and up from the baseline)
                }

              int vert  = (attach <= 3) ? 0 : ((attach <= 6) ? 1 : 2);              // 0 top, 1 middle, 2 bottom
              int horiz = (attach - 1) % 3;                                         // 0 left, 1 centre, 2 right

              double rx0; double rx1; double ry0; double ry1;                       // un-rotated block rectangle relative to the insertion point
              if     (horiz == 0) { rx0 = 0.0;          rx1 = textw;       }         // left   : grows to the right
              else if(horiz == 1) { rx0 = -textw / 2.0; rx1 = textw / 2.0; }         // centre
              else                { rx0 = -textw;       rx1 = 0.0;         }         // right  : grows to the left
              if     (vert == 0)  { ry0 = -texth;       ry1 = th;          }         // top    : grows downward (+ one line of ascent margin)
              else if(vert == 1)  { ry0 = -texth / 2.0; ry1 = texth / 2.0; }         // middle
              else                { ry0 = 0.0;          ry1 = texth;       }         // bottom : grows upward

              double rot = 0.0;   GetValueDouble(entity, 50, rot);                   // rotation (code 50, degrees CCW in world space)
              double a   = rot * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
              double ca  = cos(a);
              double sa  = sin(a);

              double cornerx[4] = { rx0, rx1, rx1, rx0 };
              double cornery[4] = { ry1, ry1, ry0, ry0 };

              for(int k=0; k<4; k++)
                {
                  double wx = ix + ((cornerx[k] * ca) - (cornery[k] * sa));         // rotate about the insertion, translate to world
                  double wy = iy + ((cornerx[k] * sa) + (cornery[k] * ca));

                  if(!found) { lminx = lmaxx = wx;  lminy = lmaxy = wy;  found = true; }
                   else
                    {
                      if(wx < lminx) lminx = wx;   if(wx > lmaxx) lmaxx = wx;
                      if(wy < lminy) lminy = wy;   if(wy > lmaxy) lmaxy = wy;
                    }
                }
            }
        }
       else if(!name->Compare(__L("LWPOLYLINE"), true) || !name->Compare(__L("SPLINE"), true))
        {
          XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();       // every code 10 / 20 pair is a point (control point for SPLINE)
          double vx = 0.0;   bool havex = false;

          for(XDWORD v=0; v<values->GetSize(); v++)
            {
              GRPVECTORFILEDXFVALUE* value = values->Get(v);
              if(!value) continue;

              if(value->GetType() == 10) { vx = (double)(*value->GetData()); havex = true; }
               else if((value->GetType() == 20) && havex)
                {
                  double vy = (double)(*value->GetData());

                  if(!found) { lminx = lmaxx = vx;  lminy = lmaxy = vy;  found = true; }
                   else
                    {
                      if(vx < lminx) lminx = vx;   if(vx > lmaxx) lmaxx = vx;
                      if(vy < lminy) lminy = vy;   if(vy > lmaxy) lmaxy = vy;
                    }

                  havex = false;
                }
            }
        }
       else if(!name->Compare(__L("INSERT"), true))
        {
          double bx; double by;
          GRPVECTORFILEDXFTEXTSECTIONENTITIES* blockentities = GetBlockEntities(entity, bx, by);

          double cminx; double cminy; double cmaxx; double cmaxy;

          if(blockentities && (depth < GRP2DVECTORFILEDXFRENDERAGG_MAXINSERTDEPTH) && ComputeExtents(blockentities, dxf, cminx, cminy, cmaxx, cmaxy, depth + 1))
            {
              double insx  = 0.0;   GetValueDouble(entity, 10, insx);
              double insy  = 0.0;   GetValueDouble(entity, 20, insy);
              double sx    = 1.0;   GetValueDouble(entity, 41, sx);
              double sy    = 1.0;   GetValueDouble(entity, 42, sy);
              double rot   = 0.0;   GetValueDouble(entity, 50, rot);
              int    ncol  = 1;     GetValueInt(entity, 70, ncol);
              int    nrow  = 1;     GetValueInt(entity, 71, nrow);
              double colsp = 0.0;   GetValueDouble(entity, 44, colsp);
              double rowsp = 0.0;   GetValueDouble(entity, 45, rowsp);

              if(ncol < 1) ncol = 1;
              if(nrow < 1) nrow = 1;

              double rad  = rot * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
              double cosr = cos(rad);
              double sinr = sin(rad);

              double cornerx[4] = { cminx, cmaxx, cmaxx, cminx };               // child bbox corners (block local space)
              double cornery[4] = { cminy, cminy, cmaxy, cmaxy };

              int cellcol[2]; int cellrow[2];
              cellcol[0] = 0;        cellrow[0] = 0;                            // the two extreme array cells bound the whole array
              cellcol[1] = ncol - 1; cellrow[1] = nrow - 1;

              for(int s=0; s<2; s++)
                {
                  double ox    = (double)cellcol[s] * colsp;
                  double oy    = (double)cellrow[s] * rowsp;
                  double cellx = insx + ((cosr * ox) - (sinr * oy));
                  double celly = insy + ((sinr * ox) + (cosr * oy));

                  double a; double b; double c; double d; double e; double f;
                  BuildInsertCellTransform(cellx, celly, sx, sy, cosr, sinr, bx, by, a, b, c, d, e, f);

                  for(int k=0; k<4; k++)
                    {
                      double wx = (a * cornerx[k]) + (c * cornery[k]) + e;
                      double wy = (b * cornerx[k]) + (d * cornery[k]) + f;

                      if(!found) { lminx = lmaxx = wx;  lminy = lmaxy = wy;  found = true; }
                       else
                        {
                          if(wx < lminx) lminx = wx;   if(wx > lmaxx) lmaxx = wx;
                          if(wy < lminy) lminy = wy;   if(wy > lmaxy) lmaxy = wy;
                        }
                    }
                }
            }
        }
       else if(!name->Compare(__L("SOLID"), true) || !name->Compare(__L("TRACE"), true) || !name->Compare(__L("3DFACE"), true))
        {
          int cornercode[4] = { 10, 11, 12, 13 };                              // SOLID / TRACE / 3DFACE corners

          for(int k=0; k<4; k++)
            {
              double vx; double vy;

              if(GetValueDouble(entity, cornercode[k], vx) && GetValueDouble(entity, cornercode[k] + 10, vy))
                {
                  if(!found) { lminx = lmaxx = vx;  lminy = lmaxy = vy;  found = true; }
                   else
                    {
                      if(vx < lminx) lminx = vx;   if(vx > lmaxx) lmaxx = vx;
                      if(vy < lminy) lminy = vy;   if(vy > lmaxy) lmaxy = vy;
                    }
                }
            }
        }
       else if(!name->Compare(__L("DIMENSION"), true))
        {
          double bx; double by;
          GRPVECTORFILEDXFTEXTSECTIONENTITIES* dimentities = GetBlockEntities(entity, bx, by);

          double cminx; double cminy; double cmaxx; double cmaxy;

          if(dimentities && (depth < GRP2DVECTORFILEDXFRENDERAGG_MAXINSERTDEPTH) && ComputeExtents(dimentities, dxf, cminx, cminy, cmaxx, cmaxy, depth + 1))
            {
              if(!found) { lminx = cminx;  lmaxx = cmaxx;  lminy = cminy;  lmaxy = cmaxy;  found = true; }   // dimension block is already in WCS
               else
                {
                  if(cminx < lminx) lminx = cminx;   if(cmaxx > lmaxx) lmaxx = cmaxx;
                  if(cminy < lminy) lminy = cminy;   if(cmaxy > lmaxy) lmaxy = cmaxy;
                }
            }
        }
       else if(!name->Compare(__L("HATCH"), true))
        {
          XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();       // approximate bbox from the boundary points (10/20 and 11/21 pairs)

          for(XDWORD v=0; (values) && (v + 1 < values->GetSize()); v++)
            {
              GRPVECTORFILEDXFVALUE* va = values->Get(v);
              GRPVECTORFILEDXFVALUE* vb = values->Get(v + 1);
              if(!va || !vb) continue;

              int ta = va->GetType();
              int tb = vb->GetType();

              if(((ta == 10) && (tb == 20)) || ((ta == 11) && (tb == 21)))
                {
                  double vx = (double)(*va->GetData());
                  double vy = (double)(*vb->GetData());

                  if(!found) { lminx = lmaxx = vx;  lminy = lmaxy = vy;  found = true; }
                   else
                    {
                      if(vx < lminx) lminx = vx;   if(vx > lmaxx) lmaxx = vx;
                      if(vy < lminy) lminy = vy;   if(vy > lmaxy) lmaxy = vy;
                    }
                }
            }
        }

      for(int i=0; i<npoints; i++)
        {
          if(!found) { lminx = lmaxx = px[i];  lminy = lmaxy = py[i];  found = true; }
           else
            {
              if(px[i] < lminx) lminx = px[i];   if(px[i] > lmaxx) lmaxx = px[i];
              if(py[i] < lminy) lminy = py[i];   if(py[i] > lmaxy) lmaxy = py[i];
            }
        }

      if(isradial)
        {
          double rx0 = cx - rr;   double rx1 = cx + rr;
          double ry0 = cy - rr;   double ry1 = cy + rr;

          if(!found) { lminx = rx0;  lmaxx = rx1;  lminy = ry0;  lmaxy = ry1;  found = true; }
           else
            {
              if(rx0 < lminx) lminx = rx0;   if(rx1 > lmaxx) lmaxx = rx1;
              if(ry0 < lminy) lminy = ry0;   if(ry1 > lmaxy) lmaxy = ry1;
            }
        }
    }

  if(!found)                                                                   // fallback : header drawing extents
    {
      GRPVECTORFILEDXFCONFIG* config = (GRPVECTORFILEDXFCONFIG*)dxf->GetConfig();
      if(config && config->GetHeader())
        {
          GRPVECTORFILEDXFTEXTSECTIONHEADER* header = config->GetHeader();

          XVARIANT* vminx = header->GetVariable(__L("$EXTMIN_10"));            // header stores points as "$EXTMIN_10_X" / "$EXTMIN_20_Y"
          XVARIANT* vminy = header->GetVariable(__L("$EXTMIN_20"));
          XVARIANT* vmaxx = header->GetVariable(__L("$EXTMAX_10"));
          XVARIANT* vmaxy = header->GetVariable(__L("$EXTMAX_20"));

          if(vminx && vminy && vmaxx && vmaxy)
            {
              lminx = (double)(*vminx);   lminy = (double)(*vminy);
              lmaxx = (double)(*vmaxx);   lmaxy = (double)(*vmaxy);

              if((lmaxx > lminx) || (lmaxy > lminy)) found = true;
            }
        }
    }

  minx = lminx;   miny = lminy;   maxx = lmaxx;   maxy = lmaxy;

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::CalculateFitTransform(GRPVECTORFILEDXF* dxf, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, double targetx, double targety, double targetwidth, double targetheight)
* @brief      Calculate fit transform : map the drawing extents into the target rectangle (uniform scale, centered, flipping the Y axis : DXF world Y grows upwards, the canvas Y grows downwards)
* @note       INTERNAL. Sets scale / offsetx / offsety used by MapPoint().
* @ingroup    GRAPHIC
* 
* @param[in]  dxf : loaded DXF file
* @param[in]  entities : entities section
* @param[in]  targetx : target rectangle x
* @param[in]  targety : target rectangle y
* @param[in]  targetwidth : target rectangle width
* @param[in]  targetheight : target rectangle height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::CalculateFitTransform(GRPVECTORFILEDXF* dxf, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, double targetx, double targety, double targetwidth, double targetheight)
{
  double minx; double miny; double maxx; double maxy;

  contextdxf = dxf;                                                            // needed so the INSERT extents branch can reach the BLOCKS section

  if(!ComputeExtents(entities, dxf, minx, miny, maxx, maxy, 0)) return false;

  double worldw = maxx - minx;
  double worldh = maxy - miny;

  if((worldw <= 0.0) && (worldh <= 0.0)) return false;                         // nothing with area : cannot fit

  double scalex = (worldw > 0.0) ? (targetwidth  / worldw) : 0.0;
  double scaley = (worldh > 0.0) ? (targetheight / worldh) : 0.0;

  double s;
  if     ((scalex > 0.0) && (scaley > 0.0))  s = (scalex < scaley) ? scalex : scaley;
  else if (scalex > 0.0)                     s = scalex;
  else                                       s = scaley;

  if(s <= 0.0) return false;

  double leftmargin = (targetwidth  - (worldw * s)) / 2.0;
  double topmargin  = (targetheight - (worldh * s)) / 2.0;

  scale   = s;
  offsetx = targetx + leftmargin - (minx * s);
  offsety = targety + topmargin  + (maxy * s);                                 // Y flip : world max y -> device top

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::MapPoint(double ux, double uy, double& dx, double& dy)
* @brief      Map point : world (Y up) -> device (Y down) using the fit transform
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  ux : world x
* @param[in]  uy : world y
* @param[in]  dx : device x
* @param[in]  dy : device y
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::MapPoint(double ux, double uy, double& dx, double& dy)
{
  double wx = (blocka * ux) + (blockc * uy) + blocke;                          // block local -> world (INSERT affine; identity at top level)
  double wy = (blockb * ux) + (blockd * uy) + blockf;

  dx = offsetx + (wx * scale);                                                 // world -> device
  dy = offsety - (wy * scale);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderEntityList(GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities)
* @brief      Render entity list : walk an entities list (the model space, or a block when expanded by INSERT) and paint each entity. Some handlers advance the index over the entities they consume (POLYLINE -> VERTEX ... SEQEND).
* @note       INTERNAL. The current block transform (blocka..blockf) is applied through MapPoint while this runs.
* @ingroup    GRAPHIC
* 
* @param[in]  entities : entities list to render
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderEntityList(GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities)
{
  if(!entities) return false;

  XVECTOR<GRPVECTORFILEDXFENTITY*>* raws = entities->GetEntities();
  if(!raws) return false;

  for(XDWORD c=0; c<raws->GetSize(); )
    {
      GRPVECTORFILEDXFENTITY* entity = raws->Get(c);

      if(entity)  RenderEntity(entity, entities, c);                           // RenderEntity may advance c (POLYLINE -> VERTEX ... SEQEND)

      c++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderEntity(GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index)
* @brief      Render entity : dispatch one raw entity to its handler
* @note       INTERNAL. For the old style POLYLINE the index is advanced over the following VERTEX ... SEQEND entities.
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* @param[in]  entities : entities section (for the POLYLINE vertex scan)
* @param[in]  index : current position in the entities stream (advanced for POLYLINE)
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderEntity(GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index)
{
  if(!entity || !contextcanvas) return false;

  XSTRING*         name  = entity->GetName();
  GRP2DCOLOR_RGBA8 color = ResolveColor(entity);

  if     (!name->Compare(__L("LINE")      , true))  return RenderLine      (entity, color, contextcanvas);
  else if(!name->Compare(__L("CIRCLE")    , true))  return RenderCircle    (entity, color, contextcanvas);
  else if(!name->Compare(__L("ARC")       , true))  return RenderArc       (entity, color, contextcanvas);
  else if(!name->Compare(__L("ELLIPSE")   , true))  return RenderEllipse   (entity, color, contextcanvas);
  else if(!name->Compare(__L("LWPOLYLINE"), true))  return RenderLWPolyLine(entity, color, contextcanvas);
  else if(!name->Compare(__L("POLYLINE")  , true))  return RenderPolyLine  (entity, entities, index, color, contextcanvas);
  else if(!name->Compare(__L("POINT")     , true))  return RenderPoint     (entity, color, contextcanvas);
  else if(!name->Compare(__L("TEXT")      , true))  return drawtext ? RenderText(entity, color, contextcanvas) : true;
  else if(!name->Compare(__L("MTEXT")     , true))  return drawtext ? RenderMText(entity, color, contextcanvas) : true;
  else if(!name->Compare(__L("3DFACE")    , true))  return Render3DFace    (entity, color, contextcanvas);
  else if(!name->Compare(__L("SPLINE")    , true))  return RenderSpline    (entity, color, contextcanvas);
  else if(!name->Compare(__L("INSERT")    , true))  return RenderInsert    (entity);
  else if(!name->Compare(__L("SOLID")     , true))  return RenderSolid     (entity, color, contextcanvas);
  else if(!name->Compare(__L("TRACE")     , true))  return RenderSolid     (entity, color, contextcanvas);
  else if(!name->Compare(__L("HATCH")     , true))  return RenderHatch     (entity, color, contextcanvas);
  else if(!name->Compare(__L("DIMENSION") , true))  return RenderDimension (entity);

  return true;                                                                 // unsupported entity : silently skipped
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderLine(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render line : LINE entity (codes 10/20 ini, 11/21 end)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw LINE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderLine(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double x0; double y0; double x1; double y1;

  if(!GetValueDouble(entity, 10, x0) || !GetValueDouble(entity, 20, y0)) return false;
  if(!GetValueDouble(entity, 11, x1) || !GetValueDouble(entity, 21, y1)) return false;

  double dx0; double dy0; double dx1; double dy1;
  MapPoint(x0, y0, dx0, dy0);
  MapPoint(x1, y1, dx1, dy1);

  GRP2DPATH path;
  path.MoveTo(dx0, dy0);
  path.LineTo(dx1, dy1);

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderCircle(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render circle : CIRCLE entity (codes 10/20 center, 40 radius) tessellated as a closed polyline
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw CIRCLE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderCircle(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double cx; double cy; double r;

  if(!GetValueDouble(entity, 10, cx) || !GetValueDouble(entity, 20, cy) || !GetValueDouble(entity, 40, r)) return false;
  if(r <= 0.0) return false;

  GRP2DPATH path;
  EmitArcWorld(path, cx, cy, r, 0.0, GRP2DVECTORFILEDXFRENDERAGG_2PI, true);
  path.Close();

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderArc(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render arc : ARC entity (codes 10/20 center, 40 radius, 50 start angle, 51 end angle, degrees, CCW)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw ARC entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderArc(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double cx; double cy; double r; double a0deg; double a1deg;

  if(!GetValueDouble(entity, 10, cx)    || !GetValueDouble(entity, 20, cy) || !GetValueDouble(entity, 40, r))     return false;
  if(!GetValueDouble(entity, 50, a0deg) || !GetValueDouble(entity, 51, a1deg))                                    return false;
  if(r <= 0.0) return false;

  double a0 = a0deg * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
  double a1 = a1deg * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;

  while(a1 < a0)  a1 += GRP2DVECTORFILEDXFRENDERAGG_2PI;                          // CCW sweep

  GRP2DPATH path;
  EmitArcWorld(path, cx, cy, r, a0, a1, true);

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderEllipse(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render ellipse : ELLIPSE entity (10/20 center, 11/21 major axis endpoint relative to center, 40 ratio minor/major, 41 start parameter, 42 end parameter, radians)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw ELLIPSE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderEllipse(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double cx; double cy; double mx; double my; double ratio; double p0; double p1;

  if(!GetValueDouble(entity, 10, cx) || !GetValueDouble(entity, 20, cy)) return false;
  if(!GetValueDouble(entity, 11, mx) || !GetValueDouble(entity, 21, my)) return false;
  if(!GetValueDouble(entity, 40, ratio)) return false;

  if(!GetValueDouble(entity, 41, p0))  p0 = 0.0;
  if(!GetValueDouble(entity, 42, p1))  p1 = GRP2DVECTORFILEDXFRENDERAGG_2PI;

  double majorlen = sqrt((mx * mx) + (my * my));
  if(majorlen <= 0.0) return false;

  double minorlen = majorlen * ratio;
  double rot      = atan2(my, mx);
  double cosr     = cos(rot);
  double sinr     = sin(rot);

  while(p1 < p0)  p1 += GRP2DVECTORFILEDXFRENDERAGG_2PI;

  double sweep = p1 - p0;
  int    n     = SegmentsForArc(majorlen, sweep);
  if(n < 1) n = 1;

  GRP2DPATH path;

  for(int i=0; i<=n; i++)
    {
      double p  = p0 + ((sweep * (double)i) / (double)n);
      double ex = majorlen * cos(p);
      double ey = minorlen * sin(p);

      double wx = cx + (ex * cosr) - (ey * sinr);
      double wy = cy + (ex * sinr) + (ey * cosr);

      double dx; double dy;
      MapPoint(wx, wy, dx, dy);

      if(i == 0)  path.MoveTo(dx, dy);
       else       path.LineTo(dx, dy);
    }

  if((sweep >= (GRP2DVECTORFILEDXFRENDERAGG_2PI - 0.000001)))  path.Close();     // full ellipse

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderLWPolyLine(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render LW polyline : LWPOLYLINE entity (90 vertex count, 70 flag (bit 1 = closed), repeated 10/20 vertex coordinates, optional 42 bulge per vertex segment)
* @note       INTERNAL. Vertices are read in stream order (the typed object does not keep them).
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw LWPOLYLINE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderLWPolyLine(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  int flag = 0;   GetValueInt(entity, 70, flag);
  bool closed = ((flag & 1) != 0);

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  XVECTOR<double> vx;
  XVECTOR<double> vy;
  XVECTOR<double> vb;                                                          // bulge per vertex (segment starting at the vertex)

  double curx = 0.0;   double cury = 0.0;   double curbulge = 0.0;   bool havevertex = false;

  for(XDWORD v=0; v<values->GetSize(); v++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(v);
      if(!value) continue;

      int code = value->GetType();

      if(code == 10)
        {
          if(havevertex) { vx.Add(curx); vy.Add(cury); vb.Add(curbulge); }     // flush previous vertex

          curx      = (double)(*value->GetData());
          curbulge  = 0.0;
          havevertex= true;
        }
       else if(code == 20)
        {
          cury = (double)(*value->GetData());
        }
       else if(code == 42)
        {
          curbulge = (double)(*value->GetData());
        }
    }

  if(havevertex) { vx.Add(curx); vy.Add(cury); vb.Add(curbulge); }             // flush last vertex

  XDWORD nv = vx.GetSize();
  if(nv < 2) { vx.DeleteAll(); vy.DeleteAll(); vb.DeleteAll(); return false; }

  GRP2DPATH path;

  double dx; double dy;
  MapPoint(vx.Get(0), vy.Get(0), dx, dy);
  path.MoveTo(dx, dy);

  for(XDWORD i=1; i<nv; i++)
    {
      EmitBulgeWorld(path, vx.Get(i - 1), vy.Get(i - 1), vx.Get(i), vy.Get(i), vb.Get(i - 1));
    }

  if(closed)
    {
      EmitBulgeWorld(path, vx.Get(nv - 1), vy.Get(nv - 1), vx.Get(0), vy.Get(0), vb.Get(nv - 1));
      path.Close();
    }

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  vx.DeleteAll();
  vy.DeleteAll();
  vb.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderPolyLine(GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render polyline : old style POLYLINE (70 flag), whose vertices are stored in the following VERTEX entities and terminated by a SEQEND entity. The index is advanced over the consumed entities.
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw POLYLINE entity
* @param[in]  entities : entities section
* @param[in]  index : position of the POLYLINE in the stream (advanced to the SEQEND on return)
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderPolyLine(GRPVECTORFILEDXFENTITY* entity, GRPVECTORFILEDXFTEXTSECTIONENTITIES* entities, XDWORD& index, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  int flag = 0;   GetValueInt(entity, 70, flag);
  bool closed = ((flag & 1) != 0);
  bool ismesh = ((flag & 16) != 0) || ((flag & 64) != 0);                      // 16 = 3D polygon mesh, 64 = polyface mesh : not a path

  XVECTOR<GRPVECTORFILEDXFENTITY*>* raws = entities->GetEntities();

  XVECTOR<double> vx;
  XVECTOR<double> vy;
  XVECTOR<double> vb;

  XDWORD c     = index + 1;
  int    guard = 0;

  while((c < raws->GetSize()) && (guard < (int)raws->GetSize()))
    {
      GRPVECTORFILEDXFENTITY* next = raws->Get(c);
      guard++;

      if(!next) { c++; continue; }

      XSTRING* nextname = next->GetName();

      if(!nextname->Compare(__L("VERTEX"), true))
        {
          double x; double y;
          if(GetValueDouble(next, 10, x) && GetValueDouble(next, 20, y))
            {
              double bulge = 0.0;   GetValueDouble(next, 42, bulge);
              vx.Add(x);   vy.Add(y);   vb.Add(bulge);
            }

          c++;
        }
       else if(!nextname->Compare(__L("SEQEND"), true))
        {
          break;                                                               // end of the vertex list
        }
       else
        {
          break;                                                               // malformed : stop the scan
        }
    }

  index = (c < raws->GetSize()) ? c : (raws->GetSize() - 1);                   // advance over consumed entities (outer loop adds +1)

  XDWORD nv = vx.GetSize();
  if(!ismesh && (nv >= 2))
    {
      GRP2DPATH path;

      double dx; double dy;
      MapPoint(vx.Get(0), vy.Get(0), dx, dy);
      path.MoveTo(dx, dy);

      for(XDWORD i=1; i<nv; i++)
        {
          EmitBulgeWorld(path, vx.Get(i - 1), vy.Get(i - 1), vx.Get(i), vy.Get(i), vb.Get(i - 1));
        }

      if(closed)
        {
          EmitBulgeWorld(path, vx.Get(nv - 1), vy.Get(nv - 1), vx.Get(0), vy.Get(0), vb.Get(nv - 1));
          path.Close();
        }

      canvas->SetLineColor(&color);
      canvas->SetLineWidth(linewidth);
      canvas->Path(path, false);
    }

  vx.DeleteAll();
  vy.DeleteAll();
  vb.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderPoint(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render point : POINT entity (codes 10/20) drawn as a small device space cross
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw POINT entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderPoint(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double x; double y;
  if(!GetValueDouble(entity, 10, x) || !GetValueDouble(entity, 20, y)) return false;

  double dx; double dy;
  MapPoint(x, y, dx, dy);

  double h = 2.0;                                                              // device half size of the marker

  GRP2DPATH path;
  path.MoveTo(dx - h, dy);   path.LineTo(dx + h, dy);
  path.MoveTo(dx, dy - h);   path.LineTo(dx, dy + h);

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderText(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render text : TEXT / MTEXT entity (1 string, 10/20 insertion point, 40 height) using the canvas vector font. The text is drawn upright (rotation and MTEXT inline formatting are not applied).
* @note       INTERNAL. A vector font must be loaded on the canvas.
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw TEXT / MTEXT entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderText(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  GRP2DCANVAS_VECTORFONT_CONFIG* config = canvas->Vectorfont_GetConfig();
  if(!config) return false;

  XCHAR* text = GetValueString(entity, 1);
  if(!text) return false;

  double x; double y; double height;
  if(!GetValueDouble(entity, 10, x) || !GetValueDouble(entity, 20, y)) return false;
  if(!GetValueDouble(entity, 40, height) || (height <= 0.0))  height = 1.0;

  double deviceheight = height * scale;
  if(deviceheight < 1.0)  deviceheight = 1.0;

  double dx; double dy;
  MapPoint(x, y, dx, dy);                                                      // DXF baseline (lower-left)

  config->SetSize(deviceheight);
  config->SetColor(&color);

  canvas->VectorFont_Print(dx, dy, text);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MTextIsArgCode(XCHAR d)
* @brief      MTEXT inline format code that carries an argument terminated by ';' (font, height, colour, width, ...).
* @ingroup    GRAPHIC
* 
* @param[in]  d : D value.
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool MTextIsArgCode(XCHAR d)
{
  return (d == __C('f')) || (d == __C('F')) || (d == __C('c')) || (d == __C('C')) || (d == __C('H')) ||
         (d == __C('W')) || (d == __C('Q')) || (d == __C('A')) || (d == __C('T')) || (d == __C('p'));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MTextIsToggle(XCHAR d)
* @brief      MTEXT inline format toggle with no argument (underline / overline / strikethrough on-off).
* @ingroup    GRAPHIC
* 
* @param[in]  d : D value.
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool MTextIsToggle(XCHAR d)
{
  return (d == __C('L')) || (d == __C('l')) || (d == __C('O')) || (d == __C('o')) || (d == __C('K')) || (d == __C('k'));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MTextDecode(XCHAR* raw, XVECTOR<XSTRING*>& lines)
* @brief      MTEXT decode : turn the MTEXT inline mini-language into plain text lines. Handles paragraph breaks (\P), escapes (\~ \\ \{ \}), grouping braces, formatting codes with an argument (\f \H \C \W \Q \A \T \p), on-off toggles (\L \O \K), and stacked text (\S num ^|/|# den ;) flattened to "num/den". The actual font / height / colour changes are not applied (single style per entity); the codes are stripped so the text reads cleanly. The caller owns the XSTRING* in the vector.
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  raw : assembled MTEXT string (codes 3* + 1)
* @param[in]  lines : decoded lines (one XSTRING per line)
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void MTextDecode(XCHAR* raw, XVECTOR<XSTRING*>& lines)
{
  if(!raw) return;

  XSTRING* cur = GEN_NEW XSTRING();
  if(!cur) return;

  XDWORD i = 0;
  XDWORD n = XSTRING::GetSize(raw);

  while(i < n)
    {
      XCHAR c = raw[i];

      if((c == __C('\\')) && ((i + 1) < n))
        {
          XCHAR d = raw[i + 1];

          if     (d == __C('P')) { lines.Add(cur);  cur = GEN_NEW XSTRING();  if(!cur) return;  i += 2; }   // paragraph break -> new line
          else if(d == __C('~')) { cur->Add(__C(' '));   i += 2; }                                          // non-breaking space
          else if(d == __C('\\')){ cur->Add(__C('\\'));  i += 2; }                                          // literal backslash
          else if(d == __C('{')) { cur->Add(__C('{'));   i += 2; }                                          // literal brace
          else if(d == __C('}')) { cur->Add(__C('}'));   i += 2; }
          else if(MTextIsArgCode(d))                                                                        // \f \H \C ... ; -> drop up to ';'
            {
              i += 2;
              while((i < n) && (raw[i] != __C(';'))) i++;
              if(i < n) i++;
            }
          else if(MTextIsToggle(d)) { i += 2; }                                                             // \L \O \K -> drop
          else if(d == __C('S'))                                                                            // \S num (^|/|#) den ; -> "num/den"
            {
              i += 2;
              while((i < n) && (raw[i] != __C(';')))
                {
                  XCHAR s = raw[i];
                  cur->Add(((s == __C('^')) || (s == __C('/')) || (s == __C('#'))) ? __C('/') : s);
                  i++;
                }
              if(i < n) i++;
            }
          else { cur->Add(d);  i += 2; }                                                                    // unknown control -> drop the backslash, keep the char
        }
       else if(c == __C('{')) { i++; }                                                                      // grouping brace -> strip
       else if(c == __C('}')) { i++; }
       else                   { cur->Add(c);  i++; }
    }

  lines.Add(cur);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderMText(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render mtext : multi-line MTEXT entity. Assembles the text (code 3 fragments + code 1), decodes the inline mini-language (see MTextDecode), and lays the lines out from the insertion point (10/20) using the text height (40), the line-spacing factor (44) and the attachment point (71) for vertical (top/middle/bottom) and horizontal (left/centre/right) anchoring (line widths measured with VectorFont_GetWidth).
* @note       INTERNAL. A vector font must be loaded on the canvas. Rotation (code 50) is applied via
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw MTEXT entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderMText(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  GRP2DCANVAS_VECTORFONT_CONFIG* config = canvas->Vectorfont_GetConfig();
  if(!config) return false;

  XSTRING raw;                                                                  // assemble the text : code 3 fragments (in order) then the final code 1

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();
  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(value && (value->GetType() == 3)) raw.Add((XCHAR*)(*value->GetData()));
    }

  XCHAR* t1 = GetValueString(entity, 1);
  if(t1) raw.Add(t1);

  if(raw.IsEmpty()) return false;

  XVECTOR<XSTRING*> lines;                                                       // decode the inline mini-language into plain lines
  MTextDecode(raw.Get(), lines);
  if(lines.IsEmpty()) return false;

  double x; double y; double height;
  if(GetValueDouble(entity, 10, x) && GetValueDouble(entity, 20, y))             // insertion point + height + attachment + line spacing
    {
      if(!GetValueDouble(entity, 40, height) || (height <= 0.0)) height = 1.0;

      double dattach = 1.0;   GetValueDouble(entity, 71, dattach);   int attach = (int)dattach;
      if((attach < 1) || (attach > 9)) attach = 1;

      double linefactor = 1.0;   double dls = 0.0;
      if(GetValueDouble(entity, 44, dls) && (dls > 0.0)) linefactor = dls;

      double worldline = height * GRP2DVECTORFILEDXFRENDERAGG_MTEXT_LINEFACTOR * linefactor;

      double deviceheight = height * scale;
      if(deviceheight < 1.0) deviceheight = 1.0;

      config->SetSize(deviceheight);
      config->SetColor(&color);

      int    nlines = (int)lines.GetSize();
      int    vert   = (attach <= 3) ? 0 : ((attach <= 6) ? 1 : 2);              // 0 top, 1 middle, 2 bottom
      int    horiz  = (attach - 1) % 3;                                         // 0 left, 1 centre, 2 right

      double firstbaseline;                                                     // world Y of the first line's baseline (lines flow downward)
      if     (vert == 0) firstbaseline = y - height;                                                       // top    : top edge of the block at the insertion point
      else if(vert == 1) firstbaseline = y - (height / 2.0) + (((double)(nlines - 1) * worldline) / 2.0);  // middle : block centred on the insertion point
      else               firstbaseline = y + ((double)(nlines - 1) * worldline);                           // bottom : bottom edge of the block at the insertion point

      double worldangle = 0.0;   double drot = 0.0;                             // rotation (code 50, CCW degrees in world space)
      if(GetValueDouble(entity, 50, drot)) worldangle = drot * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;

      double deviceangle = -worldangle;                                         // device Y is flipped versus world Y, so the screen rotation is the opposite sign
      double dcos = cos(deviceangle);
      double dsin = sin(deviceangle);
      double wcos = cos(worldangle);
      double wsin = sin(worldangle);

      for(int l=0; l<nlines; l++)
        {
          XSTRING* line = lines.Get(l);
          XCHAR*   text = line ? line->Get() : NULL;
          if(!text) continue;

          double worldx = x;                                                    // left-aligned line start (world); rotation pivots on the insertion point
          double worldy = firstbaseline - ((double)l * worldline);

          if(worldangle != 0.0)                                                 // rotate the line start about the insertion point (world, CCW)
            {
              double ox = worldx - x;
              double oy = worldy - y;
              worldx = x + ((ox * wcos) - (oy * wsin));
              worldy = y + ((ox * wsin) + (oy * wcos));
            }

          double dx; double dy;
          MapPoint(worldx, worldy, dx, dy);

          if(horiz != 0)                                                        // horizontal anchoring : shift along the (rotated) baseline direction
            {
              double w = canvas->VectorFont_GetWidth(text);
              double f = (horiz == 1) ? 0.5 : 1.0;
              dx -= (w * f * dcos);
              dy -= (w * f * dsin);
            }

          canvas->VectorFont_PrintAngle(dx, dy, deviceangle, text);            // angle == 0 delegates to VectorFont_Print (identical to the previous behaviour)
        }
    }

  for(XDWORD l=0; l<lines.GetSize(); l++)                                        // free the decoded lines
    {
      XSTRING* line = lines.Get(l);
      if(line) GEN_DELETE line;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::Render3DFace(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render 3D face : 3DFACE entity (corners 10/20, 11/21, 12/22, 13/23) painted as a closed wire quad projected onto the XY plane
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw 3DFACE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::Render3DFace(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double x[4];   double y[4];   int n = 0;

  int codex[4] = { 10, 11, 12, 13 };
  int codey[4] = { 20, 21, 22, 23 };

  for(int i=0; i<4; i++)
    {
      double px; double py;
      if(GetValueDouble(entity, codex[i], px) && GetValueDouble(entity, codey[i], py)) { x[n] = px;  y[n] = py;  n++; }
    }

  if(n < 2) return false;

  GRP2DPATH path;

  double dx; double dy;
  MapPoint(x[0], y[0], dx, dy);
  path.MoveTo(dx, dy);

  for(int i=1; i<n; i++)
    {
      MapPoint(x[i], y[i], dx, dy);
      path.LineTo(dx, dy);
    }

  path.Close();

  canvas->SetLineColor(&color);
  canvas->SetLineWidth(linewidth);
  canvas->Path(path, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderSpline(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render spline : SPLINE entity (71 degree, 72 knot count, 73 control point count, 74 fit point count, repeated 40 knots, repeated 10/20 control points, optional repeated 11/21 fit points). The B-spline is evaluated with de Boor's algorithm and tessellated to line segments. Non rational (no weights) curves are assumed; if no usable control points / knots are present it falls back to the fit points, and then to the control polygon.
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw SPLINE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderSpline(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  int  flag   = 0;   GetValueInt(entity, 70, flag);
  int  degree = 0;   GetValueInt(entity, 71, degree);
  bool closed = ((flag & 1) != 0);

  XVECTOR<double> knots;
  XVECTOR<double> cpx;
  XVECTOR<double> cpy;
  XVECTOR<double> fpx;
  XVECTOR<double> fpy;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  double cx = 0.0;   bool havecx = false;
  double fx = 0.0;   bool havefx = false;

  for(XDWORD v=0; v<values->GetSize(); v++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(v);
      if(!value) continue;

      int code = value->GetType();

      if     (code == 40)                  { knots.Add((double)(*value->GetData()));                            }
       else if(code == 10)                 { cx = (double)(*value->GetData());   havecx = true;                 }
       else if((code == 20) && havecx)     { cpx.Add(cx);   cpy.Add((double)(*value->GetData()));   havecx = false; }
       else if(code == 11)                 { fx = (double)(*value->GetData());   havefx = true;                 }
       else if((code == 21) && havefx)     { fpx.Add(fx);   fpy.Add((double)(*value->GetData()));   havefx = false; }
    }

  GRP2DPATH path;
  bool      painted = false;

  XDWORD ncp = cpx.GetSize();
  XDWORD nk  = knots.GetSize();

  if((degree >= 1) && (degree <= GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE) && (ncp >= (XDWORD)(degree + 1)) && (nk >= (ncp + (XDWORD)degree + 1)))
    {
      int    n  = (int)ncp - 1;                                                // last control point index
      double u0 = knots.Get((XDWORD)degree);                                   // valid parameter domain : [U[p], U[n+1]]
      double u1 = knots.Get((XDWORD)(n + 1));

      if(u1 > u0)
        {
          int nsamples = (int)ncp * 4;                                         // tessellation density (one knot span needs a few samples)
          if(nsamples < GRP2DVECTORFILEDXFRENDERAGG_MINSEGMENTS)       nsamples = GRP2DVECTORFILEDXFRENDERAGG_MINSEGMENTS;
          if(nsamples > GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINESEGMENTS) nsamples = GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINESEGMENTS;

          for(int i=0; i<=nsamples; i++)
            {
              double u = u0 + (((u1 - u0) * (double)i) / (double)nsamples);
              if(u > u1) u = u1;

              int    span = FindBSplineSpan(knots, (int)ncp, degree, u);
              double wx;   double wy;
              EvalBSplinePoint(knots, cpx, cpy, degree, span, u, wx, wy);

              double dx;   double dy;
              MapPoint(wx, wy, dx, dy);

              if(i == 0)  path.MoveTo(dx, dy);
               else       path.LineTo(dx, dy);
            }

          painted = true;
        }
    }

  if(!painted && (fpx.GetSize() >= 2))                                         // fallback : polyline through the fit points
    {
      XDWORD nf = fpx.GetSize();
      double dx; double dy;

      MapPoint(fpx.Get(0), fpy.Get(0), dx, dy);
      path.MoveTo(dx, dy);

      for(XDWORD i=1; i<nf; i++)
        {
          MapPoint(fpx.Get(i), fpy.Get(i), dx, dy);
          path.LineTo(dx, dy);
        }

      painted = true;
    }

  if(!painted && (ncp >= 2))                                                   // fallback : control polygon
    {
      double dx; double dy;

      MapPoint(cpx.Get(0), cpy.Get(0), dx, dy);
      path.MoveTo(dx, dy);

      for(XDWORD i=1; i<ncp; i++)
        {
          MapPoint(cpx.Get(i), cpy.Get(i), dx, dy);
          path.LineTo(dx, dy);
        }

      painted = true;
    }

  if(painted)
    {
      if(closed)  path.Close();

      canvas->SetLineColor(&color);
      canvas->SetLineWidth(linewidth);
      canvas->Path(path, false);
    }

  knots.DeleteAll();
  cpx.DeleteAll();   cpy.DeleteAll();
  fpx.DeleteAll();   fpy.DeleteAll();

  return painted;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONENTITIES* GRP2DVECTORFILEDXFRENDERAGG::GetBlockEntities(GRPVECTORFILEDXFENTITY* entity, double& basex, double& basey)
* @brief      Get block entities : resolve the block referenced by an INSERT entity (code 2) from the BLOCKS section and return its entities list, also reading the block base point (codes 10 / 20 in the block header)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw INSERT entity
* @param[in]  basex : block base point x (0 if absent)
* @param[in]  basey : block base point y (0 if absent)
* 
* @return     GRPVECTORFILEDXFTEXTSECTIONENTITIES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONENTITIES* GRP2DVECTORFILEDXFRENDERAGG::GetBlockEntities(GRPVECTORFILEDXFENTITY* entity, double& basex, double& basey)
{
  basex = 0.0;
  basey = 0.0;

  if(!contextdxf) return NULL;

  XCHAR* blockname = GetValueString(entity, 2);
  if(!blockname) return NULL;

  GRPVECTORFILEDXFTEXTSECTIONBLOCKS* blocks = (GRPVECTORFILEDXFTEXTSECTIONBLOCKS*)contextdxf->GetSection(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_BLOCKS);
  if(!blocks) return NULL;

  GRPVECTORFILEDXFTEXTBLOCK* block = blocks->GetBlock(blockname, 0);
  if(!block) return NULL;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = block->GetValues();                // block header : base point at codes 10 / 20
  if(values)
    {
      bool hx = false;   bool hy = false;

      for(XDWORD c=0; c<values->GetSize(); c++)
        {
          GRPVECTORFILEDXFVALUE* value = values->Get(c);
          if(!value) continue;

          if     (!hx && (value->GetType() == 10)) { basex = (double)(*value->GetData());   hx = true; }
           else if(!hy && (value->GetType() == 20)) { basey = (double)(*value->GetData());   hy = true; }

          if(hx && hy) break;
        }
    }

  return block->GetSectionEntities();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::BuildInsertCellTransform(double cellx, double celly, double sx, double sy, double cosr, double sinr, double basex, double basey, double& a, double& b, double& c, double& d, double& e, double& f)
* @brief      Build insert cell transform : the affine that maps a block local point to the parent space for one INSERT cell : P = cellinsertion + Rot * Scale * (P_local - base). Returned as [a c e ; b d f].
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  cellx : cell insertion point x (array offset already applied)
* @param[in]  celly : cell insertion point y
* @param[in]  sx : x scale
* @param[in]  sy : y scale
* @param[in]  cosr : cosine of the rotation angle
* @param[in]  sinr : sine of the rotation angle
* @param[in]  basex : block base point x
* @param[in]  basey : block base point y
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  e : E value.
* @param[in]  f : F value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::BuildInsertCellTransform(double cellx, double celly, double sx, double sy, double cosr, double sinr, double basex, double basey, double& a, double& b, double& c, double& d, double& e, double& f)
{
  a = sx * cosr;                                                              // linear part : Rot * Scale
  b = sx * sinr;
  c = -sy * sinr;
  d = sy * cosr;

  e = cellx - ((a * basex) + (c * basey));                                    // translation : base point maps to the insertion point
  f = celly - ((b * basex) + (d * basey));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderInsert(GRPVECTORFILEDXFENTITY* entity)
* @brief      Render insert : block reference. Reads the insertion point (10/20), scale (41/42), rotation (50) and the optional MINSERT array (70/71 counts, 44/45 spacing), composes the insertion transform with the current block transform and renders the referenced block entities once per array cell.
* @note       INTERNAL. Nested INSERT (block within block) is supported up to MAXINSERTDEPTH. The OCS extrusion
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw INSERT entity
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderInsert(GRPVECTORFILEDXFENTITY* entity)
{
  if(insertdepth >= GRP2DVECTORFILEDXFRENDERAGG_MAXINSERTDEPTH) return true;    // cyclic / too deep : stop expanding

  double basex; double basey;

  GRPVECTORFILEDXFTEXTSECTIONENTITIES* blockentities = GetBlockEntities(entity, basex, basey);
  if(!blockentities) return true;

  double insx  = 0.0;   GetValueDouble(entity, 10, insx);
  double insy  = 0.0;   GetValueDouble(entity, 20, insy);
  double sx    = 1.0;   GetValueDouble(entity, 41, sx);
  double sy    = 1.0;   GetValueDouble(entity, 42, sy);
  double rot   = 0.0;   GetValueDouble(entity, 50, rot);                       // degrees
  int    ncol  = 1;     GetValueInt(entity, 70, ncol);
  int    nrow  = 1;     GetValueInt(entity, 71, nrow);
  double colsp = 0.0;   GetValueDouble(entity, 44, colsp);
  double rowsp = 0.0;   GetValueDouble(entity, 45, rowsp);

  if(ncol < 1) ncol = 1;
  if(nrow < 1) nrow = 1;

  double rad  = rot * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
  double cosr = cos(rad);
  double sinr = sin(rad);

  double sa = blocka;   double sb = blockb;   double sc = blockc;              // save the current block transform
  double sd = blockd;   double se = blocke;   double sf = blockf;

  for(int row=0; row<nrow; row++)
    {
      for(int col=0; col<ncol; col++)
        {
          double ox    = (double)col * colsp;                                  // array offset, rotated into world by the insert angle
          double oy    = (double)row * rowsp;
          double cellx = insx + ((cosr * ox) - (sinr * oy));
          double celly = insy + ((sinr * ox) + (cosr * oy));

          double ia; double ib; double ic; double id; double ie; double iff;
          BuildInsertCellTransform(cellx, celly, sx, sy, cosr, sinr, basex, basey, ia, ib, ic, id, ie, iff);

          blocka = (sa * ia) + (sc * ib);                                      // compose : new = current o insertion
          blockb = (sb * ia) + (sd * ib);
          blockc = (sa * ic) + (sc * id);
          blockd = (sb * ic) + (sd * id);
          blocke = (sa * ie) + (sc * iff) + se;
          blockf = (sb * ie) + (sd * iff) + sf;

          insertdepth++;
          RenderEntityList(blockentities);
          insertdepth--;
        }
    }

  blocka = sa;   blockb = sb;   blockc = sc;                                    // restore the current block transform
  blockd = sd;   blocke = se;   blockf = sf;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderSolid(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render solid : SOLID / TRACE filled triangle or quadrilateral (corners at 10/20, 11/21, 12/22, 13/23). The DXF corner order is a "bowtie" (the third and fourth corners are swapped), so the filled outline is corner1 -> corner2 -> corner4 -> corner3. When the fourth corner is absent it falls back to the third (triangle).
* @note       INTERNAL. SOLID / TRACE are area fills, so this is one of the few handlers that fills instead of strokes.
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw SOLID / TRACE entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderSolid(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  double x0; double y0; double x1; double y1; double x2; double y2; double x3; double y3;

  if(!GetValueDouble(entity, 10, x0) || !GetValueDouble(entity, 20, y0)) return false;
  if(!GetValueDouble(entity, 11, x1) || !GetValueDouble(entity, 21, y1)) return false;
  if(!GetValueDouble(entity, 12, x2) || !GetValueDouble(entity, 22, y2)) return false;

  if(!GetValueDouble(entity, 13, x3)) x3 = x2;                                  // fourth corner optional : triangle
  if(!GetValueDouble(entity, 23, y3)) y3 = y2;

  double dx0; double dy0; double dx1; double dy1; double dx2; double dy2; double dx3; double dy3;
  MapPoint(x0, y0, dx0, dy0);
  MapPoint(x1, y1, dx1, dy1);
  MapPoint(x2, y2, dx2, dy2);
  MapPoint(x3, y3, dx3, dy3);

  GRP2DPATH path;
  path.MoveTo(dx0, dy0);                                                        // corner1 -> corner2 -> corner4 -> corner3 (bowtie corrected)
  path.LineTo(dx1, dy1);
  path.LineTo(dx3, dy3);
  path.LineTo(dx2, dy2);
  path.Close();

  canvas->SetFillColor(&color);
  canvas->Path(path, true);

  return true;
}


/*---- HATCH boundary stream helpers (file local) --------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HatchPeekType(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD i, XDWORD n, int& outtype)
* @brief      Peek the group code of the value at position i without advancing.
* @ingroup    GRAPHIC
* 
* @param[in]  values : Values pointer to use.
* @param[in]  i : I value.
* @param[in]  n : N value.
* @param[in]  outtype : Output outtype.
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool HatchPeekType(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD i, XDWORD n, int& outtype)
{
  if(i >= n) return false;

  GRPVECTORFILEDXFVALUE* value = values->Get(i);
  if(!value) return false;

  outtype = value->GetType();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HatchReadValue(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD& i, XDWORD n, int code, double& out)
* @brief      If the value at i has the expected group code, read it and advance i.
* @ingroup    GRAPHIC
* 
* @param[in]  values : Values pointer to use.
* @param[in]  i : I value.
* @param[in]  n : N value.
* @param[in]  code : Code value.
* @param[in]  out : Output out.
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool HatchReadValue(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD& i, XDWORD n, int code, double& out)
{
  if(i >= n) return false;

  GRPVECTORFILEDXFVALUE* value = values->Get(i);
  if(!value || (value->GetType() != code)) return false;

  XVARIANT* data = value->GetData();                                           // the XVARIANT cast operators reinterpret the raw bytes (they do not convert),
                                                                               // so read it according to its stored type : flags/counts (70..79, 90..99) are
  switch(data->GetType())                                                      // stored as INTEGER and must NOT be read as a double.
    {
      case XVARIANT_TYPE_DOUBLE  : out = (double)(*data);              break;
      case XVARIANT_TYPE_FLOAT   : out = (double)(float)(*data);       break;
                       default    : out = (double)(int)(*data);        break;   // integer family (boolean/short/word/integer/dword)
    }

  i++;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HatchReadPoint(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD& i, XDWORD n, int codex, int codey, double& x, double& y)
* @brief      Read a (codex, codey) coordinate pair and advance i past both. Atomic : i is left untouched on failure.
* @ingroup    GRAPHIC
* 
* @param[in]  values : Values pointer to use.
* @param[in]  i : I value.
* @param[in]  n : N value.
* @param[in]  codex : Codex value.
* @param[in]  codey : Codey value.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     static bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool HatchReadPoint(XVECTOR<GRPVECTORFILEDXFVALUE*>* values, XDWORD& i, XDWORD n, int codex, int codey, double& x, double& y)
{
  XDWORD save = i;

  if(!HatchReadValue(values, i, n, codex, x)) { i = save; return false; }
  if(!HatchReadValue(values, i, n, codey, y)) { i = save; return false; }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderHatch(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
* @brief      Render hatch : HATCH boundary loops filled as a single even-odd region (so nested loops cut holes). The boundary data is a structured value stream that has to be walked sequentially (the group codes repeat), so this parses it by position rather than by group-code lookup. Each loop is either a polyline (with optional bulges) or a list of edges (line / arc / elliptic arc / spline).
* @note       INTERNAL. The fill is solid with the resolved color : SOLID hatches are exact, pattern hatches are
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw HATCH entity
* @param[in]  color : resolved color
* @param[in]  canvas : target canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderHatch(GRPVECTORFILEDXFENTITY* entity, GRP2DCOLOR_RGBA8 color, GRP2DCANVAS* canvas)
{
  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();
  if(!values) return false;

  XDWORD n = values->GetSize();
  XDWORD i = 0;

  int npaths = 0;

  while(i < n)                                                                  // locate the boundary data : the path count (code 91)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(i);
      i++;
      if(value && (value->GetType() == 91)) { npaths = (int)(*value->GetData()); break; }
    }

  if(npaths <= 0) return false;

  GRP2DPATH path;
  path.SetFillRule(GRP2DPATHFILLRULE_EVENODD);

  bool any = false;

  for(int p=0; p<npaths; p++)
    {
      double dpathtype = 0.0;
      if(!HatchReadValue(values, i, n, 92, dpathtype)) break;                   // malformed : abort
      int pathtype = (int)dpathtype;

      bool ispolyline = ((pathtype & 2) != 0);

      if(ispolyline)
        {
          double dtmp     = 0.0;
          int    hasbulge = 0;
          int    nverts   = 0;

          while(i < n)                                                          // header of the polyline loop : 72 (has bulge), 73 (closed), 93 (vertices)
            {
              int t = 0;
              if(!HatchPeekType(values, i, n, t)) break;

              if     (t == 72) { HatchReadValue(values, i, n, 72, dtmp);  hasbulge = (int)dtmp; }
               else if(t == 73) { HatchReadValue(values, i, n, 73, dtmp); }
               else if(t == 93) { HatchReadValue(values, i, n, 93, dtmp);  nverts = (int)dtmp;  break; }
               else break;
            }

          double firstx = 0.0;   double firsty = 0.0;
          double prevx  = 0.0;   double prevy  = 0.0;
          double prevbulge = 0.0;
          bool   havefirst = false;

          for(int vtx=0; vtx<nverts; vtx++)
            {
              double x; double y;
              if(!HatchReadPoint(values, i, n, 10, 20, x, y)) break;

              double bulge = 0.0;
              if(hasbulge) HatchReadValue(values, i, n, 42, bulge);             // optional per-vertex bulge

              if(!havefirst)
                {
                  double dx; double dy;   MapPoint(x, y, dx, dy);
                  path.MoveTo(dx, dy);
                  firstx = x;   firsty = y;   havefirst = true;
                }
               else
                {
                  if(prevbulge != 0.0)  EmitBulgeWorld(path, prevx, prevy, x, y, prevbulge);
                   else                { double dx; double dy;   MapPoint(x, y, dx, dy);   path.LineTo(dx, dy); }
                }

              prevx = x;   prevy = y;   prevbulge = bulge;
            }

          if(havefirst)                                                         // close the loop (hatch polyline boundaries are implicitly closed)
            {
              if(prevbulge != 0.0)  EmitBulgeWorld(path, prevx, prevy, firstx, firsty, prevbulge);
              path.Close();
              any = true;
            }
        }
       else
        {
          double dnedges = 0.0;
          HatchReadValue(values, i, n, 93, dnedges);                           // number of edges
          int nedges = (int)dnedges;

          bool firstedge = true;

          for(int e=0; e<nedges; e++)
            {
              double dedgetype = 0.0;
              if(!HatchReadValue(values, i, n, 72, dedgetype)) break;
              int edgetype = (int)dedgetype;

              if(edgetype == 1)                                                 // LINE : 10/20 start, 11/21 end
                {
                  double x0; double y0; double x1; double y1;
                  if(!HatchReadPoint(values, i, n, 10, 20, x0, y0)) break;
                  if(!HatchReadPoint(values, i, n, 11, 21, x1, y1)) break;

                  double dx; double dy;
                  if(firstedge) { MapPoint(x0, y0, dx, dy);  path.MoveTo(dx, dy);  firstedge = false; }
                  MapPoint(x1, y1, dx, dy);   path.LineTo(dx, dy);
                }
               else if(edgetype == 2)                                          // CIRCULAR ARC : 10/20 center, 40 radius, 50/51 angles (deg), 73 ccw
                {
                  double cx; double cy; double r = 0.0; double a0d = 0.0; double a1d = 0.0; double ccw = 1.0;
                  if(!HatchReadPoint(values, i, n, 10, 20, cx, cy)) break;
                  HatchReadValue(values, i, n, 40, r);
                  HatchReadValue(values, i, n, 50, a0d);
                  HatchReadValue(values, i, n, 51, a1d);
                  HatchReadValue(values, i, n, 73, ccw);

                  double a0 = a0d * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
                  double a1 = a1d * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;

                  if(ccw != 0.0) { while(a1 <  a0) a1 += GRP2DVECTORFILEDXFRENDERAGG_2PI; }
                   else          { while(a1 >  a0) a1 -= GRP2DVECTORFILEDXFRENDERAGG_2PI; }

                  EmitArcWorld(path, cx, cy, r, a0, a1, firstedge);
                  firstedge = false;
                }
               else if(edgetype == 3)                                          // ELLIPTIC ARC : 10/20 center, 11/21 major endpoint (rel), 40 ratio, 50/51 angles (deg), 73 ccw
                {
                  double cx; double cy; double mx = 0.0; double my = 0.0; double ratio = 1.0; double a0d = 0.0; double a1d = 0.0; double ccw = 1.0;
                  if(!HatchReadPoint(values, i, n, 10, 20, cx, cy)) break;
                  if(!HatchReadPoint(values, i, n, 11, 21, mx, my)) break;
                  HatchReadValue(values, i, n, 40, ratio);
                  HatchReadValue(values, i, n, 50, a0d);
                  HatchReadValue(values, i, n, 51, a1d);
                  HatchReadValue(values, i, n, 73, ccw);

                  double a0 = a0d * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;
                  double a1 = a1d * GRP2DVECTORFILEDXFRENDERAGG_DEG2RAD;

                  if(ccw != 0.0) { while(a1 <  a0) a1 += GRP2DVECTORFILEDXFRENDERAGG_2PI; }
                   else          { while(a1 >  a0) a1 -= GRP2DVECTORFILEDXFRENDERAGG_2PI; }

                  double majorlen = sqrt((mx * mx) + (my * my));
                  double minorlen = majorlen * ratio;
                  double axisang  = atan2(my, mx);
                  double cosx     = cos(axisang);
                  double sinx     = sin(axisang);

                  double sweep = a1 - a0;
                  int    steps = SegmentsForArc(majorlen, sweep);
                  if(steps < 1) steps = 1;

                  for(int s=0; s<=steps; s++)
                    {
                      double t  = a0 + ((sweep * (double)s) / (double)steps);
                      double ex = majorlen * cos(t);                            // ellipse-local
                      double ey = minorlen * sin(t);
                      double wx = cx + ((ex * cosx) - (ey * sinx));             // rotate by the major-axis angle
                      double wy = cy + ((ex * sinx) + (ey * cosx));

                      double dx; double dy;   MapPoint(wx, wy, dx, dy);
                      if((s == 0) && firstedge) path.MoveTo(dx, dy);
                       else                     path.LineTo(dx, dy);
                    }

                  firstedge = false;
                }
               else if(edgetype == 4)                                          // SPLINE : approximated by its control polygon (data fully consumed to stay in sync)
                {
                  double ddeg = 0.0; double drat = 0.0; double dper = 0.0; double dnk = 0.0; double dnc = 0.0;
                  HatchReadValue(values, i, n, 94, ddeg);
                  HatchReadValue(values, i, n, 73, drat);
                  HatchReadValue(values, i, n, 74, dper);
                  HatchReadValue(values, i, n, 95, dnk);
                  HatchReadValue(values, i, n, 96, dnc);

                  int nknots = (int)dnk;
                  int ncp    = (int)dnc;
                  int israt  = (int)drat;

                  double knot = 0.0;
                  for(int k=0; k<nknots; k++) HatchReadValue(values, i, n, 40, knot);   // knots : consumed

                  for(int c=0; c<ncp; c++)                                      // control points : connected as a polygon
                    {
                      double x; double y;
                      if(!HatchReadPoint(values, i, n, 10, 20, x, y)) break;

                      double w = 0.0;
                      if(israt) HatchReadValue(values, i, n, 42, w);            // weight : consumed

                      double dx; double dy;   MapPoint(x, y, dx, dy);
                      if(firstedge) { path.MoveTo(dx, dy);  firstedge = false; }
                       else           path.LineTo(dx, dy);
                    }

                  double dnfit = 0.0;                                           // optional fit points : consumed
                  if(HatchReadValue(values, i, n, 97, dnfit))
                    {
                      int nfit = (int)dnfit;
                      for(int fp=0; fp<nfit; fp++)
                        {
                          double fx; double fy;
                          if(!HatchReadPoint(values, i, n, 11, 21, fx, fy)) break;
                        }
                    }
                }
               else
                {
                  break;                                                        // unknown edge type : cannot keep the stream in sync, abort this loop
                }
            }

          if(!firstedge) { path.Close(); any = true; }
        }

      double dnsource = 0.0;                                                    // skip the source boundary objects : 97 (count) then count * 330
      if(HatchReadValue(values, i, n, 97, dnsource))
        {
          int nsource = (int)dnsource;
          int skipped = 0;
          double h = 0.0;
          while((skipped < nsource) && HatchReadValue(values, i, n, 330, h)) skipped++;
        }
    }

  if(!any) return false;

  canvas->SetFillColor(&color);
  canvas->Path(path, true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::RenderDimension(GRPVECTORFILEDXFENTITY* entity)
* @brief      Render dimension : a DIMENSION carries its drawn geometry (lines, arrowheads, text) in an anonymous block referenced by code 2, already laid out in the dimension coordinate space. This renders that block under the current block transform, without any extra insertion transform.
* @note       INTERNAL. Shares the INSERT recursion guard.
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw DIMENSION entity
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::RenderDimension(GRPVECTORFILEDXFENTITY* entity)
{
  if(insertdepth >= GRP2DVECTORFILEDXFRENDERAGG_MAXINSERTDEPTH) return true;    // cyclic / too deep : stop expanding

  double basex; double basey;

  GRPVECTORFILEDXFTEXTSECTIONENTITIES* blockentities = GetBlockEntities(entity, basex, basey);
  if(!blockentities) return true;

  insertdepth++;
  RenderEntityList(blockentities);                                             // the dimension block geometry is already in place
  insertdepth--;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRP2DVECTORFILEDXFRENDERAGG::FindBSplineSpan(XVECTOR<double>& knots, int ncontrolpoints, int degree, double u)
* @brief      Find B-spline span : knot span index k such that knots[k] <= u < knots[k+1] (clamped to the valid range), using the binary search of The NURBS Book (algorithm A2.1)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  knots : knot vector
* @param[in]  ncontrolpoints : number of control points
* @param[in]  degree : curve degree
* @param[in]  u : parameter
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRP2DVECTORFILEDXFRENDERAGG::FindBSplineSpan(XVECTOR<double>& knots, int ncontrolpoints, int degree, double u)
{
  int n = ncontrolpoints - 1;                                                  // last control point index

  if(u >= knots.Get((XDWORD)(n + 1)))  return n;
  if(u <= knots.Get((XDWORD)degree))   return degree;

  int low  = degree;
  int high = n + 1;
  int mid  = (low + high) / 2;

  while((u < knots.Get((XDWORD)mid)) || (u >= knots.Get((XDWORD)(mid + 1))))
    {
      if(u < knots.Get((XDWORD)mid))  high = mid;
       else                           low  = mid;

      mid = (low + high) / 2;

      if((mid <= degree) || (mid >= (n + 1))) break;                           // safety bound
    }

  return mid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::EvalBSplinePoint(XVECTOR<double>& knots, XVECTOR<double>& cpx, XVECTOR<double>& cpy, int degree, int span, double u, double& outx, double& outy)
* @brief      Eval B-spline point : evaluate a non rational B-spline curve at parameter u using de Boor's algorithm
* @note       INTERNAL. Repeated knots (zero denominator) are handled with alpha = 0.
* @ingroup    GRAPHIC
* 
* @param[in]  knots : knot vector
* @param[in]  cpx : control point x coordinates
* @param[in]  cpy : control point y coordinates
* @param[in]  degree : curve degree
* @param[in]  span : knot span containing u (from FindBSplineSpan)
* @param[in]  u : parameter
* @param[in]  outx : evaluated x
* @param[in]  outy : evaluated y
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::EvalBSplinePoint(XVECTOR<double>& knots, XVECTOR<double>& cpx, XVECTOR<double>& cpy, int degree, int span, double u, double& outx, double& outy)
{
  double dx[GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE + 1];
  double dy[GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE + 1];

  if(degree > GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE) degree = GRP2DVECTORFILEDXFRENDERAGG_MAXSPLINEDEGREE;

  for(int j=0; j<=degree; j++)                                                 // initial control points of the span
    {
      int cpindex = span - degree + j;
      dx[j] = cpx.Get((XDWORD)cpindex);
      dy[j] = cpy.Get((XDWORD)cpindex);
    }

  for(int r=1; r<=degree; r++)                                                 // de Boor recurrence
    {
      for(int j=degree; j>=r; j--)
        {
          int    i      = span - degree + j;
          double left   = knots.Get((XDWORD)i);
          double right  = knots.Get((XDWORD)(i + degree - r + 1));
          double denom  = right - left;
          double alpha  = (denom > 0.0000000001) ? ((u - left) / denom) : 0.0;

          dx[j] = ((1.0 - alpha) * dx[j - 1]) + (alpha * dx[j]);
          dy[j] = ((1.0 - alpha) * dy[j - 1]) + (alpha * dy[j]);
        }
    }

  outx = dx[degree];
  outy = dy[degree];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::EmitArcWorld(GRP2DPATH& devicepath, double cx, double cy, double radius, double startangle, double endangle, bool moveto)
* @brief      Emit arc world : tessellate a circular arc (world space, radians, CCW) into the (device space) path
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  devicepath : output path (device space)
* @param[in]  cx : world center x
* @param[in]  cy : world center y
* @param[in]  radius : world radius
* @param[in]  startangle : start angle (radians)
* @param[in]  endangle : end angle (radians, >= start angle)
* @param[in]  moveto : true to emit a MoveTo for the first point, false to LineTo (continue the current sub path)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::EmitArcWorld(GRP2DPATH& devicepath, double cx, double cy, double radius, double startangle, double endangle, bool moveto)
{
  double sweep = endangle - startangle;
  int    n     = SegmentsForArc(radius, sweep);
  if(n < 1) n = 1;

  for(int i=0; i<=n; i++)
    {
      double a  = startangle + ((sweep * (double)i) / (double)n);
      double wx = cx + (radius * cos(a));
      double wy = cy + (radius * sin(a));

      double dx; double dy;
      MapPoint(wx, wy, dx, dy);

      if((i == 0) && moveto)  devicepath.MoveTo(dx, dy);
       else                   devicepath.LineTo(dx, dy);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::EmitBulgeWorld(GRP2DPATH& devicepath, double x0, double y0, double x1, double y1, double bulge)
* @brief      Emit bulge world : add the segment p0 -> p1 to the (device space) path. A non zero bulge defines a circular arc (bulge = tan(includedangle / 4)); the previous point must already be the path pen.
* @note       INTERNAL. Degenerate cases fall back to a straight line.
* @ingroup    GRAPHIC
* 
* @param[in]  devicepath : output path (device space)
* @param[in]  x0 : world segment start x
* @param[in]  y0 : world segment start y
* @param[in]  x1 : world segment end x
* @param[in]  y1 : world segment end y
* @param[in]  bulge : arc bulge (0 = straight)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::EmitBulgeWorld(GRP2DPATH& devicepath, double x0, double y0, double x1, double y1, double bulge)
{
  double dx; double dy;

  if((bulge > -0.000001) && (bulge < 0.000001))                               // straight segment
    {
      MapPoint(x1, y1, dx, dy);
      devicepath.LineTo(dx, dy);
      return;
    }

  double cdx   = x1 - x0;
  double cdy   = y1 - y0;
  double chord = sqrt((cdx * cdx) + (cdy * cdy));

  if(chord < 0.000001)
    {
      MapPoint(x1, y1, dx, dy);
      devicepath.LineTo(dx, dy);
      return;
    }

  double included = 4.0 * atan(bulge);                                         // signed included angle
  double sinhalf  = sin(included / 2.0);

  if((sinhalf > -0.000001) && (sinhalf < 0.000001))
    {
      MapPoint(x1, y1, dx, dy);
      devicepath.LineTo(dx, dy);
      return;
    }

  double r  = (chord / 2.0) / sinhalf;                                         // signed radius
  double ux = cdx / chord;
  double uy = cdy / chord;
  double h  = r * cos(included / 2.0);                                         // signed distance midpoint -> center

  double mx = (x0 + x1) / 2.0;
  double my = (y0 + y1) / 2.0;

  double ccx = mx - (uy * h);                                                  // center (perpendicular to chord)
  double ccy = my + (ux * h);

  double absr   = (r < 0.0) ? -r : r;
  double a0     = atan2(y0 - ccy, x0 - ccx);

  int    n      = SegmentsForArc(absr, (included < 0.0) ? -included : included);
  if(n < 1) n = 1;

  for(int i=1; i<=n; i++)
    {
      double a  = a0 + ((included * (double)i) / (double)n);
      double wx = ccx + (absr * cos(a));
      double wy = ccy + (absr * sin(a));

      MapPoint(wx, wy, dx, dy);
      devicepath.LineTo(dx, dy);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRP2DVECTORFILEDXFRENDERAGG::SegmentsForArc(double radius, double sweepradians)
* @brief      Segments for arc : number of line segments to tessellate an arc so the chord error stays below the configured device tolerance (clamped to sane bounds)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  radius : world radius
* @param[in]  sweepradians : arc sweep (radians)
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRP2DVECTORFILEDXFRENDERAGG::SegmentsForArc(double radius, double sweepradians)
{
  double sweep = (sweepradians < 0.0) ? -sweepradians : sweepradians;
  if(sweep <= 0.0) return 1;

  double devradius = radius * scale;
  if(devradius < 0.0) devradius = -devradius;

  double tol = GRP2DVECTORFILEDXFRENDERAGG_ARCTOLERANCE;
  double dtheta;

  if(devradius <= tol)
    {
      dtheta = sweep;                                                          // tiny radius : a couple of segments
    }
   else
    {
      double cosv = 1.0 - (tol / devradius);
      if(cosv < -1.0) cosv = -1.0;
      if(cosv >  1.0) cosv =  1.0;

      dtheta = 2.0 * acos(cosv);
      if(dtheta <= 0.0) dtheta = sweep;
    }

  int n = (int)ceil(sweep / dtheta);

  double frac = sweep / GRP2DVECTORFILEDXFRENDERAGG_2PI;
  int    minn = (int)ceil(frac * (double)GRP2DVECTORFILEDXFRENDERAGG_MINSEGMENTS);
  int    maxn = (int)ceil(frac * (double)GRP2DVECTORFILEDXFRENDERAGG_MAXSEGMENTS);
  if(minn < 1) minn = 1;
  if(maxn < 1) maxn = 1;

  if(n < minn) n = minn;
  if(n > maxn) n = maxn;

  return n;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::GetValueDouble(GRPVECTORFILEDXFENTITY* entity, int groupcode, double& out)
* @brief      Get value double : first entity value matching the DXF group code, as a double
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* @param[in]  groupcode : DXF group code
* @param[in]  out : value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::GetValueDouble(GRPVECTORFILEDXFENTITY* entity, int groupcode, double& out)
{
  if(!entity) return false;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(value && (value->GetType() == groupcode))
        {
          XVARIANT* data = value->GetData();                                   // the XVARIANT cast operators reinterpret the raw bytes, so convert by the stored type
          switch(data->GetType())
            {
              case XVARIANT_TYPE_DOUBLE : out = (double)(*data);              break;
              case XVARIANT_TYPE_FLOAT  : out = (double)(float)(*data);       break;
                               default   : out = (double)(int)(*data);        break;   // integer family (flags / counts such as code 71)
            }
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVECTORFILEDXFRENDERAGG::GetValueInt(GRPVECTORFILEDXFENTITY* entity, int groupcode, int& out)
* @brief      Get value int : first entity value matching the DXF group code, as an int
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* @param[in]  groupcode : DXF group code
* @param[in]  out : value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVECTORFILEDXFRENDERAGG::GetValueInt(GRPVECTORFILEDXFENTITY* entity, int groupcode, int& out)
{
  if(!entity) return false;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(value && (value->GetType() == groupcode))
        {
          out = (int)(*value->GetData());
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRP2DVECTORFILEDXFRENDERAGG::GetValueString(GRPVECTORFILEDXFENTITY* entity, int groupcode)
* @brief      Get value string : first entity value matching the DXF group code, as a string
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* @param[in]  groupcode : DXF group code
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRP2DVECTORFILEDXFRENDERAGG::GetValueString(GRPVECTORFILEDXFENTITY* entity, int groupcode)
{
  if(!entity) return NULL;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(value && (value->GetType() == groupcode))
        {
          return (XCHAR*)(*value->GetData());
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR_RGBA8 GRP2DVECTORFILEDXFRENDERAGG::ResolveColor(GRPVECTORFILEDXFENTITY* entity)
* @brief      Resolve color : color for an entity (forced color, or the ACI code 62 / G_COLOR_NUMBER, or foreground)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* 
* @return     GRP2DCOLOR_RGBA8 : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR_RGBA8 GRP2DVECTORFILEDXFRENDERAGG::ResolveColor(GRPVECTORFILEDXFENTITY* entity)
{
  if(forcecoloractive) return forcecolor;

  int  aci          = 256;                                                     // default : ByLayer -> resolve via the layer table
  bool haveexplicit = false;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(!value) continue;

      if(value->GetType() == 62)  { aci = (int)(*value->GetData());  haveexplicit = true;  break; }

      if(!value->GetName()->Compare(__L("G_COLOR_NUMBER"), true))  { aci = (int)(*value->GetData());  haveexplicit = true;  break; }
    }

  if(!haveexplicit || (aci == 256) || (aci == 0))                              // no explicit color, or ByLayer (256) / ByBlock (0) -> use the entity's layer color
    {
      int layeraci = LayerColorACI(GetLayerName(entity));
      if((layeraci > 0) && (layeraci < 256)) aci = layeraci;
    }

  return ColorFromACI(aci);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::BuildLayerTable(GRPVECTORFILEDXF* dxf)
* @brief      Build layer table : parse the LAYER records of the TABLES section directly from the file (the interpreter does not parse TABLES), so entities coloured ByLayer can resolve their colour. Each LAYER record carries its name (code 2) and its ACI colour (code 62; negative means the layer is off, the absolute value is the colour). The table is cached per file and only rebuilt when the file changes.
* @note       INTERNAL. "0 / LAYER" only appears in the LAYER table, so scanning the whole file is safe.
* @ingroup    GRAPHIC
* 
* @param[in]  dxf : loaded DXF file
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::BuildLayerTable(GRPVECTORFILEDXF* dxf)
{
  if(dxf == layertabledxf) return;                                             // already built for this file

  CleanLayerTable();
  layertabledxf = dxf;

  if(!dxf) return;

  XPATH* path = dxf->GetPathFile();
  if(!path) return;

  XFILETXT* file = GEN_NEW XFILETXT();
  if(!file) return;

  if(file->Open(*path))
    {
      file->ReadAllFile();

      int     nlines   = file->GetNLines();

      bool    inlayer  = false;                                                // inside a "0 / LAYER" record
      bool    havename = false;
      int     aci      = 7;
      XSTRING name;

      for(int l=0; (l + 1) < nlines; l += 2)                                   // DXF is a strict (group code, value) pair stream
        {
          XSTRING* codeline = file->GetLine(l);
          XSTRING* valline  = file->GetLine(l + 1);
          if(!codeline || !valline) continue;

          GRPVECTORFILEDXF::ParserTextFilePrepareLine(codeline);               // trim exactly as the interpreter does (so layer names match the entities')
          GRPVECTORFILEDXF::ParserTextFilePrepareLine(valline);

          int code = codeline->ConvertToInt();

          if(code == 0)
            {
              if(inlayer && havename)                                          // flush the record that just ended
                {
                  XSTRING* nm = GEN_NEW XSTRING();
                  if(nm)
                    {
                      nm->Set(name);
                      layernames.Add(nm);
                      layeracis.Add(aci);
                    }
                }

              inlayer  = (valline->Compare(__L("LAYER"), true) == 0);
              havename = false;
              aci      = 7;
              name.Empty();
            }
           else if(inlayer)
            {
              if(code == 2)        { name.Set(*valline);  havename = true; }
               else if(code == 62) { int n = valline->ConvertToInt();  aci = (n < 0) ? -n : n; }
            }
        }

      if(inlayer && havename)                                                  // flush the last record if the file ends inside it
        {
          XSTRING* nm = GEN_NEW XSTRING();
          if(nm)
            {
              nm->Set(name);
              layernames.Add(nm);
              layeracis.Add(aci);
            }
        }
    }

  GEN_DELETE file;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRP2DVECTORFILEDXFRENDERAGG::LayerColorACI(XCHAR* layername)
* @brief      Layer color ACI : ACI colour of a layer by name (case-insensitive)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  layername : layer name
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRP2DVECTORFILEDXFRENDERAGG::LayerColorACI(XCHAR* layername)
{
  if(!layername) return 256;

  for(XDWORD c=0; c<layernames.GetSize(); c++)
    {
      XSTRING* nm = layernames.Get(c);
      if(nm && (nm->Compare(layername, true) == 0)) return layeracis.Get(c);
    }

  return 256;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRP2DVECTORFILEDXFRENDERAGG::GetLayerName(GRPVECTORFILEDXFENTITY* entity)
* @brief      Get layer name : the entity's layer (group code 8 / G_LAYER_NAME)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  entity : raw entity
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRP2DVECTORFILEDXFRENDERAGG::GetLayerName(GRPVECTORFILEDXFENTITY* entity)
{
  if(!entity) return NULL;

  XVECTOR<GRPVECTORFILEDXFVALUE*>* values = entity->GetValues();

  for(XDWORD c=0; c<values->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = values->Get(c);
      if(!value) continue;

      if(value->GetType() == 8)  return (XCHAR*)(*value->GetData());

      if(value->GetName() && !value->GetName()->Compare(__L("G_LAYER_NAME"), true))  return (XCHAR*)(*value->GetData());
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::CleanLayerTable()
* @brief      Clean layer table : free the cached LAYER table
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::CleanLayerTable()
{
  for(XDWORD c=0; c<layernames.GetSize(); c++)
    {
      XSTRING* nm = layernames.Get(c);
      if(nm) GEN_DELETE nm;
    }

  layernames.DeleteAll();
  layeracis.DeleteAll();

  layertabledxf = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR_RGBA8 GRP2DVECTORFILEDXFRENDERAGG::ColorFromACI(int aci)
* @brief      Color from ACI : map an AutoCAD Color Index (1..255) to its standard RGB color. ByBlock (0), ByLayer (256) and the auto color (7) follow the foreground (white on a dark background, black on a light one), so they stay visible whatever the destination background is.
* @note       INTERNAL. The full 255 entry standard AutoCAD palette is used. Note that the gray ramp (250..255) is
* @ingroup    GRAPHIC
* 
* @param[in]  aci : AutoCAD Color Index
* 
* @return     GRP2DCOLOR_RGBA8 : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR_RGBA8 GRP2DVECTORFILEDXFRENDERAGG::ColorFromACI(int aci)
{
  static const XBYTE acipalette[255][3] =
    {
      {255,  0,  0}, {255,255,  0}, {  0,255,  0}, {  0,255,255}, {  0,  0,255},
      {255,  0,255}, {255,255,255}, {128,128,128}, {192,192,192}, {255,  0,  0},
      {255,127,127}, {165,  0,  0}, {165, 82, 82}, {127,  0,  0}, {127, 63, 63},
      { 76,  0,  0}, { 76, 38, 38}, { 38,  0,  0}, { 38, 19, 19}, {255, 63,  0},
      {255,159,127}, {165, 41,  0}, {165,103, 82}, {127, 31,  0}, {127, 79, 63},
      { 76, 19,  0}, { 76, 47, 38}, { 38,  9,  0}, { 38, 23, 19}, {255,127,  0},
      {255,191,127}, {165, 82,  0}, {165,124, 82}, {127, 63,  0}, {127, 95, 63},
      { 76, 38,  0}, { 76, 57, 38}, { 38, 19,  0}, { 38, 28, 19}, {255,191,  0},
      {255,223,127}, {165,124,  0}, {165,145, 82}, {127, 95,  0}, {127,111, 63},
      { 76, 57,  0}, { 76, 66, 38}, { 38, 28,  0}, { 38, 33, 19}, {255,255,  0},
      {255,255,127}, {165,165,  0}, {165,165, 82}, {127,127,  0}, {127,127, 63},
      { 76, 76,  0}, { 76, 76, 38}, { 38, 38,  0}, { 38, 38, 19}, {191,255,  0},
      {223,255,127}, {124,165,  0}, {145,165, 82}, { 95,127,  0}, {111,127, 63},
      { 57, 76,  0}, { 66, 76, 38}, { 28, 38,  0}, { 33, 38, 19}, {127,255,  0},
      {191,255,127}, { 82,165,  0}, {124,165, 82}, { 63,127,  0}, { 95,127, 63},
      { 38, 76,  0}, { 57, 76, 38}, { 19, 38,  0}, { 28, 38, 19}, { 63,255,  0},
      {159,255,127}, { 41,165,  0}, {103,165, 82}, { 31,127,  0}, { 79,127, 63},
      { 19, 76,  0}, { 47, 76, 38}, {  9, 38,  0}, { 23, 38, 19}, {  0,255,  0},
      {127,255,127}, {  0,165,  0}, { 82,165, 82}, {  0,127,  0}, { 63,127, 63},
      {  0, 76,  0}, { 38, 76, 38}, {  0, 38,  0}, { 19, 38, 19}, {  0,255, 63},
      {127,255,159}, {  0,165, 41}, { 82,165,103}, {  0,127, 31}, { 63,127, 79},
      {  0, 76, 19}, { 38, 76, 47}, {  0, 38,  9}, { 19, 38, 23}, {  0,255,127},
      {127,255,191}, {  0,165, 82}, { 82,165,124}, {  0,127, 63}, { 63,127, 95},
      {  0, 76, 38}, { 38, 76, 57}, {  0, 38, 19}, { 19, 38, 28}, {  0,255,191},
      {127,255,223}, {  0,165,124}, { 82,165,145}, {  0,127, 95}, { 63,127,111},
      {  0, 76, 57}, { 38, 76, 66}, {  0, 38, 28}, { 19, 38, 33}, {  0,255,255},
      {127,255,255}, {  0,165,165}, { 82,165,165}, {  0,127,127}, { 63,127,127},
      {  0, 76, 76}, { 38, 76, 76}, {  0, 38, 38}, { 19, 38, 38}, {  0,191,255},
      {127,223,255}, {  0,124,165}, { 82,145,165}, {  0, 95,127}, { 63,111,127},
      {  0, 57, 76}, { 38, 66, 76}, {  0, 28, 38}, { 19, 33, 38}, {  0,127,255},
      {127,191,255}, {  0, 82,165}, { 82,124,165}, {  0, 63,127}, { 63, 95,127},
      {  0, 38, 76}, { 38, 57, 76}, {  0, 19, 38}, { 19, 28, 38}, {  0, 63,255},
      {127,159,255}, {  0, 41,165}, { 82,103,165}, {  0, 31,127}, { 63, 79,127},
      {  0, 19, 76}, { 38, 47, 76}, {  0,  9, 38}, { 19, 23, 38}, {  0,  0,255},
      {127,127,255}, {  0,  0,165}, { 82, 82,165}, {  0,  0,127}, { 63, 63,127},
      {  0,  0, 76}, { 38, 38, 76}, {  0,  0, 38}, { 19, 19, 38}, { 63,  0,255},
      {159,127,255}, { 41,  0,165}, {103, 82,165}, { 31,  0,127}, { 79, 63,127},
      { 19,  0, 76}, { 47, 38, 76}, {  9,  0, 38}, { 23, 19, 38}, {127,  0,255},
      {191,127,255}, { 82,  0,165}, {124, 82,165}, { 63,  0,127}, { 95, 63,127},
      { 38,  0, 76}, { 57, 38, 76}, { 19,  0, 38}, { 28, 19, 38}, {191,  0,255},
      {223,127,255}, {124,  0,165}, {145, 82,165}, { 95,  0,127}, {111, 63,127},
      { 57,  0, 76}, { 66, 38, 76}, { 28,  0, 38}, { 33, 19, 38}, {255,  0,255},
      {255,127,255}, {165,  0,165}, {165, 82,165}, {127,  0,127}, {127, 63,127},
      { 76,  0, 76}, { 76, 38, 76}, { 38,  0, 38}, { 38, 19, 38}, {255,  0,191},
      {255,127,223}, {165,  0,124}, {165, 82,145}, {127,  0, 95}, {127, 63,111},
      { 76,  0, 57}, { 76, 38, 66}, { 38,  0, 28}, { 38, 19, 33}, {255,  0,127},
      {255,127,191}, {165,  0, 82}, {165, 82,124}, {127,  0, 63}, {127, 63, 95},
      { 76,  0, 38}, { 76, 38, 57}, { 38,  0, 19}, { 38, 19, 28}, {255,  0, 63},
      {255,127,159}, {165,  0, 41}, {165, 82,103}, {127,  0, 31}, {127, 63, 79},
      { 76,  0, 19}, { 76, 38, 47}, { 38,  0,  9}, { 38, 19, 23}, { 51, 51, 51},
      { 91, 91, 91}, {132,132,132}, {173,173,173}, {214,214,214}, {255,255,255}
    };

  GRP2DCOLOR_RGBA8 foreground = backgroundisdark ? GRP2DCOLOR_RGBA8(255, 255, 255, 255) : GRP2DCOLOR_RGBA8(0, 0, 0, 255);

  if((aci == 0) || (aci == 7) || (aci == 256)) return foreground;              // ByBlock / auto / ByLayer -> foreground

  if((aci >= 1) && (aci <= 255))
    {
      return GRP2DCOLOR_RGBA8(acipalette[aci - 1][0], acipalette[aci - 1][1], acipalette[aci - 1][2], 255);
    }

  return foreground;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVECTORFILEDXFRENDERAGG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVECTORFILEDXFRENDERAGG::Clean()
{
  contextcanvas    = NULL;
  contextdxf       = NULL;
  insertdepth      = 0;

  scale            = 1.0;
  offsetx          = 0.0;
  offsety          = 0.0;

  blocka           = 1.0;
  blockb           = 0.0;
  blockc           = 0.0;
  blockd           = 1.0;
  blocke           = 0.0;
  blockf           = 0.0;

  linewidth        = GRP2DVECTORFILEDXFRENDERAGG_DEFAULTLINEWIDTH;
  backgroundisdark = false;
  drawtext         = true;

  forcecoloractive = false;
  forcecolor       = GRP2DCOLOR_RGBA8(0, 0, 0, 255);

  CleanLayerTable();                                                           // frees the cached LAYER table and resets layertabledxf
}
