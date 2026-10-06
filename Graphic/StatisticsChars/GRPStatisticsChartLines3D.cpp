/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartLines3D.cpp
* 
* @class      GRPSTATISTICSCHARTLINES3D
* @brief      Graphic Chart : 3D lines chart implementation (depth lane ribbons)
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPStatisticsChartLines3D.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTLINES3D::GRPSTATISTICSCHARTLINES3D()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTLINES3D::GRPSTATISTICSCHARTLINES3D()
{
  depthratio = 0.15;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTLINES3D::~GRPSTATISTICSCHARTLINES3D()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTLINES3D::~GRPSTATISTICSCHARTLINES3D()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTLINES3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : back wall + floor, value grid, one depth lane ribbon per series and labels
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  x : plot area left
* @param[in]  y : plot area top
* @param[in]  width : plot area width
* @param[in]  height : plot area height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTLINES3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats   = data.GetNCategories();
  XDWORD nseries = data.GetNSeries();

  if((ncats < 2) || !nseries) return false;

  double rawmax  = data.GetMaxValue();
  double nicemax = 1.0;
  int    ndiv    = 5;

  ComputeNiceMax(rawmax, nicemax, ndiv);

  if(nicemax <= 0.0) return false;

  double fontsize     = config.GetFontSize();
  double axisfontsize = config.GetAxisFontSize();

  double depth   = ((width < height) ? width : height) * depthratio;

  double fx0     = x;
  double fwidth  = width - depth;
  double basey   = y + height;
  double ftopy   = y + depth;
  double fheight = basey - ftopy;

  if((fwidth <= 0.0) || (fheight <= 0.0)) return false;

  // back wall + floor (3D context)

  GRPSTATISTICSCHARTCOLOR wallcolor(245, 245, 245);
  GRPSTATISTICSCHARTCOLOR floorcolor(232, 232, 232);
  GRPSTATISTICSCHARTCOLOR contextedge(205, 205, 205);

  double wallquad[8];
  wallquad[0] = fx0 + depth;           wallquad[1] = ftopy - depth;
  wallquad[2] = fx0 + fwidth + depth;  wallquad[3] = ftopy - depth;
  wallquad[4] = fx0 + fwidth + depth;  wallquad[5] = basey - depth;
  wallquad[6] = fx0 + depth;           wallquad[7] = basey - depth;

  GRPSTATISTICSCHARTSTYLE wallstyle;
  wallstyle.SetFill(wallcolor);
  wallstyle.SetStroke(contextedge, 1.0);
  builder.DrawPolygon(wallquad, 4, wallstyle);

  double floorquad[8];
  floorquad[0] = fx0;                  floorquad[1] = basey;
  floorquad[2] = fx0 + fwidth;         floorquad[3] = basey;
  floorquad[4] = fx0 + fwidth + depth; floorquad[5] = basey - depth;
  floorquad[6] = fx0 + depth;          floorquad[7] = basey - depth;

  GRPSTATISTICSCHARTSTYLE floorstyle;
  floorstyle.SetFill(floorcolor);
  floorstyle.SetStroke(contextedge, 1.0);
  builder.DrawPolygon(floorquad, 4, floorstyle);

  // value grid (on the back wall) + value labels (Y axis)

  GRPSTATISTICSCHARTSTYLE gridstyle;
  gridstyle.SetStroke(config.GetGridColor(), 1.0);

  GRPSTATISTICSCHARTTEXTSTYLE valuestyle;
  valuestyle.color  = config.GetTextColor();
  valuestyle.size   = axisfontsize;
  valuestyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

  for(int d=0; d<=ndiv; d++)
    {
      double value = (nicemax * (double)d) / (double)ndiv;
      double wy    = (basey - depth) - ((value / nicemax) * fheight);

      if(config.GetShowGrid())
        {
          builder.DrawLine(fx0 + depth, wy, fx0 + fwidth + depth, wy, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING label;
          label.Format(_L("%g"), value);

          builder.DrawText(fx0 + depth - (axisfontsize * 0.5), wy + (axisfontsize * 0.35), label.Get(), valuestyle);
        }
    }

  // depth lane ribbons (one per series, drawn from the back lane to the front lane)

  double leftgap     = depth + (axisfontsize * 0.6);
  double colareax    = fx0 + leftgap;
  double colareaw    = fwidth - leftgap;

  if(colareaw <= 0.0) return false;

  double slotwidth   = colareaw / (double)ncats;
  double banddepth   = depth / (double)nseries;                                 // each series owns one depth lane
  double ribbonthick = fheight * 0.04;

  if(ribbonthick < 4.0) ribbonthick = 4.0;

  for(int s=(int)nseries-1; s>=0; s--)
    {
      GRPSTATISTICSCHARTSERIE* serie = data.GetSerie((XDWORD)s);
      if(!serie) continue;

      GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor((XDWORD)s);

      double zfront = (double)s * banddepth;
      double zback  = zfront + (banddepth * 0.7);                               // ribbon fills 70% of its lane

      //  both end caps first : they sit behind, and the ribbon body (drawn on top) covers the overlap,
      //  leaving only the clean side face of each cap visible (see images 2 and 3)

      double capxR = colareax + (slotwidth * ((double)(ncats - 1) + 0.5));
      double capyR = basey - ((serie->GetValue(ncats - 1) / nicemax) * fheight);
      double capxL = colareax + (slotwidth * 0.5);
      double capyL = basey - ((serie->GetValue(0) / nicemax) * fheight);

      DrawRibbonCap(builder, capxR, capyR, zfront, zback, ribbonthick, color);
      DrawRibbonCap(builder, capxL, capyL, zfront, zback, ribbonthick, color);

      for(XDWORD c=0; (c + 1)<ncats; c++)
        {
          double v0 = serie->GetValue(c);
          double v1 = serie->GetValue(c + 1);

          double x0 = colareax + (slotwidth * ((double)c + 0.5));
          double x1 = colareax + (slotwidth * ((double)(c + 1) + 0.5));
          double y0 = basey - ((v0 / nicemax) * fheight);
          double y1 = basey - ((v1 / nicemax) * fheight);

          DrawRibbonSegment(builder, x0, y0, x1, y1, zfront, zback, ribbonthick, color);
        }

      if(config.GetShowValues())
        {
          GRPSTATISTICSCHARTTEXTSTYLE vstyle;
          vstyle.color  = config.GetTextColor();
          vstyle.size   = fontsize * 0.8;
          vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

          for(XDWORD c=0; c<ncats; c++)
            {
              double value = serie->GetValue(c);
              double x0    = colareax + (slotwidth * ((double)c + 0.5));
              double y0    = basey - ((value / nicemax) * fheight);
              double fx    = x0 + zfront;
              double fy    = y0 - zfront;

              XSTRING vlabel;
              vlabel.Format(_L("%g"), value);

              builder.DrawText(fx, fy - ribbonthick - 2.0, vlabel.Get(), vstyle);
            }
        }
    }

  // category labels (X axis) under the front floor edge

  if(config.GetShowAxisLabels())
    {
      GRPSTATISTICSCHARTTEXTSTYLE catstyle;
      catstyle.color  = config.GetTextColor();
      catstyle.size   = axisfontsize;
      catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

      for(XDWORD c=0; c<ncats; c++)
        {
          XSTRING* category = data.GetCategory(c);
          if(!category) continue;

          double lx = colareax + (slotwidth * ((double)c + 0.5));

          builder.DrawText(lx, basey + (axisfontsize * 1.3), category->Get(), catstyle);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTLINES3D::DrawRibbonSegment(GRPSTATISTICSCHARTBUILDER& builder, double x0, double y0, double x1, double y1, double zfront, double zback, double thick, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Draw ribbon segment : the top band (start->end, projected through the lane depth) plus a shaded front edge
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  x0 : plane x of the start point (z = 0)
* @param[in]  y0 : plane y of the start point (z = 0)
* @param[in]  x1 : plane x of the end point (z = 0)
* @param[in]  y1 : plane y of the end point (z = 0)
* @param[in]  zfront : lane front depth offset
* @param[in]  zback : lane back depth offset
* @param[in]  thick : vertical thickness of the front edge
* @param[in]  color : ribbon color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTLINES3D::DrawRibbonSegment(GRPSTATISTICSCHARTBUILDER& builder, double x0, double y0, double x1, double y1, double zfront, double zback, double thick, GRPSTATISTICSCHARTCOLOR& color)
{
  double fax = x0 + zfront;   double fay = y0 - zfront;                          // front start (top)
  double fbx = x1 + zfront;   double fby = y1 - zfront;                          // front end   (top)
  double bax = x0 + zback;    double bay = y0 - zback;                           // back  start (top)
  double bbx = x1 + zback;    double bby = y1 - zback;                           // back  end   (top)

  GRPSTATISTICSCHARTCOLOR edgecolor = MakeShade(color, 0.65);
  GRPSTATISTICSCHARTCOLOR line(70, 70, 70);

  GRPSTATISTICSCHARTSTYLE topstyle;
  topstyle.SetFill(color);

  GRPSTATISTICSCHARTSTYLE edgestyle;
  edgestyle.SetFill(edgecolor);
  edgestyle.SetStroke(line, 0.5);

  double topface[8]  = { fax, fay, fbx, fby, bbx, bby, bax, bay };               // top band
  double edgeface[8] = { fax, fay, fbx, fby, fbx, fby + thick, fax, fay + thick };// front edge (thickness)

  builder.DrawPolygon(topface, 4, topstyle);
  builder.DrawPolygon(edgeface, 4, edgestyle);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTLINES3D::DrawRibbonCap(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double zfront, double zback, double thick, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Draw ribbon cap : the transversal face that closes a ribbon end (front..back x top..thick)
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  px : plane x of the end point (z = 0)
* @param[in]  py : plane y of the end point (z = 0)
* @param[in]  zfront : lane front depth offset
* @param[in]  zback : lane back depth offset
* @param[in]  thick : vertical thickness of the ribbon
* @param[in]  color : ribbon color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTLINES3D::DrawRibbonCap(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double zfront, double zback, double thick, GRPSTATISTICSCHARTCOLOR& color)
{
  double ftx = px + zfront;   double fty = py - zfront;                          // front top
  double btx = px + zback;    double bty = py - zback;                           // back  top

  GRPSTATISTICSCHARTCOLOR capcolor = MakeShade(color, 0.80);
  GRPSTATISTICSCHARTCOLOR line(70, 70, 70);

  GRPSTATISTICSCHARTSTYLE capstyle;
  capstyle.SetFill(capcolor);
  capstyle.SetStroke(line, 0.5);

  double capface[8] = { ftx, fty, btx, bty, btx, bty + thick, ftx, fty + thick };

  builder.DrawPolygon(capface, 4, capstyle);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTLINES3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
* @brief      Make shade : scale a color by a factor (>1 lighter, <1 darker), clamped to 0..255
* @ingroup    GRAPHIC
* 
* @param[in]  color : base color
* @param[in]  factor : brightness factor
* 
* @return     GRPSTATISTICSCHARTCOLOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTLINES3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
{
  int r = (int)((double)color.r * factor);
  int g = (int)((double)color.g * factor);
  int b = (int)((double)color.b * factor);

  if(r > 255) r = 255;   if(r < 0) r = 0;
  if(g > 255) g = 255;   if(g < 0) g = 0;
  if(b > 255) b = 255;   if(b < 0) b = 0;

  return GRPSTATISTICSCHARTCOLOR((XBYTE)r, (XBYTE)g, (XBYTE)b, color.a);
}


