/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartStackedColumns3D.cpp
* 
* @class      GRPSTATISTICSCHARTSTACKEDCOLUMNS3D
* @brief      Graphic Chart : 3D stacked columns chart implementation
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

#include "GRPStatisticsChartStackedColumns3D.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::GRPSTATISTICSCHARTSTACKEDCOLUMNS3D()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::GRPSTATISTICSCHARTSTACKEDCOLUMNS3D()
{
  depthratio = 0.15;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::~GRPSTATISTICSCHARTSTACKEDCOLUMNS3D()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::~GRPSTATISTICSCHARTSTACKEDCOLUMNS3D()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : back wall + floor, value grid, one stacked 3D column (series prisms) per category and labels
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
bool GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats   = data.GetNCategories();
  XDWORD nseries = data.GetNSeries();

  if(!ncats || !nseries) return false;

  // stacked maximum : the largest per category total (sum of every serie)

  double rawmax = 0.0;

  for(XDWORD c=0; c<ncats; c++)
    {
      double total = 0.0;

      for(XDWORD s=0; s<nseries; s++)
        {
          GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
          if(serie) total += serie->GetValue(c);
        }

      if(total > rawmax) rawmax = total;
    }

  double nicemax = 1.0;
  int    ndiv    = 5;

  ComputeNiceMax(rawmax, nicemax, ndiv);

  if(nicemax <= 0.0) return false;

  double fontsize     = config.GetFontSize();
  double axisfontsize = config.GetAxisFontSize();

  double depth   = ((width < height) ? width : height) * depthratio;
  double depthx  = depth;
  double depthy  = depth;

  double fx0     = x;
  double fwidth  = width - depthx;
  double basey   = y + height;
  double ftopy   = y + depthy;
  double fheight = basey - ftopy;

  if((fwidth <= 0.0) || (fheight <= 0.0)) return false;

  // back wall + floor (3D context)

  GRPSTATISTICSCHARTCOLOR wallcolor(245, 245, 245);
  GRPSTATISTICSCHARTCOLOR floorcolor(232, 232, 232);
  GRPSTATISTICSCHARTCOLOR contextedge(205, 205, 205);

  double wallquad[8];
  wallquad[0] = fx0 + depthx;          wallquad[1] = ftopy - depthy;
  wallquad[2] = fx0 + fwidth + depthx; wallquad[3] = ftopy - depthy;
  wallquad[4] = fx0 + fwidth + depthx; wallquad[5] = basey - depthy;
  wallquad[6] = fx0 + depthx;          wallquad[7] = basey - depthy;

  GRPSTATISTICSCHARTSTYLE wallstyle;
  wallstyle.SetFill(wallcolor);
  wallstyle.SetStroke(contextedge, 1.0);
  builder.DrawPolygon(wallquad, 4, wallstyle);

  double floorquad[8];
  floorquad[0] = fx0;                   floorquad[1] = basey;
  floorquad[2] = fx0 + fwidth;          floorquad[3] = basey;
  floorquad[4] = fx0 + fwidth + depthx; floorquad[5] = basey - depthy;
  floorquad[6] = fx0 + depthx;          floorquad[7] = basey - depthy;

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
      double wy    = (basey - depthy) - ((value / nicemax) * fheight);

      if(config.GetShowGrid())
        {
          builder.DrawLine(fx0 + depthx, wy, fx0 + fwidth + depthx, wy, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING label;
          label.Format(_L("%g"), value);

          builder.DrawText(fx0 + depthx - (axisfontsize * 0.5), wy + (axisfontsize * 0.35), label.Get(), valuestyle);
        }
    }

  // stacked 3D columns (one column per category, series prisms stacked from the base up)

  double leftgap    = depthx + (axisfontsize * 0.6);
  double colareax   = fx0 + leftgap;
  double colareaw   = fwidth - leftgap;

  if(colareaw <= 0.0) return false;

  double slotwidth  = colareaw / (double)ncats;
  double colpad     = slotwidth * 0.20;
  double colw       = slotwidth - (colpad * 2.0);

  GRPSTATISTICSCHARTCOLOR valuecolor(255, 255, 255);

  GRPSTATISTICSCHARTTEXTSTYLE catstyle;
  catstyle.color  = config.GetTextColor();
  catstyle.size   = axisfontsize;
  catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

  for(XDWORD c=0; c<ncats; c++)
    {
      double slotx = colareax + (slotwidth * (double)c);
      double colx  = slotx + colpad;
      double cumy  = basey;                                                     // running top of the stack (starts at the base)

      for(XDWORD s=0; s<nseries; s++)                                           // base to top : the upper segment covers the lower's top
        {
          GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
          if(!serie) continue;

          double value = serie->GetValue(c);
          double segh  = (value / nicemax) * fheight;
          double segy  = cumy - segh;

          GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor(s);

          DrawPrism(builder, colx, segy, colw, segh, depthx, depthy, color);

          if(config.GetShowValues() && (segh > fontsize))
            {
              XSTRING vlabel;
              vlabel.Format(_L("%g"), value);

              GRPSTATISTICSCHARTTEXTSTYLE vstyle;
              vstyle.color  = valuecolor;
              vstyle.size   = fontsize * 0.85;
              vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

              builder.DrawText(colx + (colw * 0.5), segy + (segh * 0.5) + (fontsize * 0.3), vlabel.Get(), vstyle);
            }

          cumy = segy;
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING* category = data.GetCategory(c);
          if(category)
            {
              builder.DrawText(slotx + (slotwidth * 0.5), basey + (axisfontsize * 1.3), category->Get(), catstyle);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::DrawPrism(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double pw, double ph, double depthx, double depthy, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Draw prism : a 3D box with three visible faces (top lit, right shaded, front base color)
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  px : front left of the box
* @param[in]  py : front top of the box
* @param[in]  pw : box width
* @param[in]  ph : box height
* @param[in]  depthx : depth offset on X
* @param[in]  depthy : depth offset on Y (applied upwards)
* @param[in]  color : base (front) color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::DrawPrism(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double pw, double ph, double depthx, double depthy, GRPSTATISTICSCHARTCOLOR& color)
{
  double fltx = px;                     double flty = py;                       // front left  top
  double frtx = px + pw;                double frty = py;                       // front right top
  double frbx = px + pw;                double frby = py + ph;                  // front right bottom
  double flbx = px;                     double flby = py + ph;                  // front left  bottom

  double bltx = px + depthx;            double blty = py - depthy;              // back  left  top
  double brtx = px + pw + depthx;       double brty = py - depthy;              // back  right top
  double brbx = px + pw + depthx;       double brby = py + ph - depthy;         // back  right bottom

  GRPSTATISTICSCHARTCOLOR topcolor  = MakeShade(color, 1.22);
  GRPSTATISTICSCHARTCOLOR sidecolor = MakeShade(color, 0.72);
  GRPSTATISTICSCHARTCOLOR edge(70, 70, 70);

  GRPSTATISTICSCHARTSTYLE topstyle;
  topstyle.SetFill(topcolor);
  topstyle.SetStroke(edge, 0.75);

  GRPSTATISTICSCHARTSTYLE sidestyle;
  sidestyle.SetFill(sidecolor);
  sidestyle.SetStroke(edge, 0.75);

  GRPSTATISTICSCHARTSTYLE frontstyle;
  frontstyle.SetFill(color);
  frontstyle.SetStroke(edge, 0.75);

  double topface[8]   = { fltx, flty, frtx, frty, brtx, brty, bltx, blty };     // top   face
  double sideface[8]  = { frtx, frty, frbx, frby, brbx, brby, brtx, brty };     // right face
  double frontface[8] = { flbx, flby, frbx, frby, frtx, frty, fltx, flty };     // front face

  builder.DrawPolygon(topface, 4, topstyle);
  builder.DrawPolygon(sideface, 4, sidestyle);
  builder.DrawPolygon(frontface, 4, frontstyle);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
* @brief      Make shade : scale a color by a factor (>1 lighter, <1 darker), clamped to 0..255
* @ingroup    GRAPHIC
* 
* @param[in]  color : base color
* @param[in]  factor : brightness factor
* 
* @return     GRPSTATISTICSCHARTCOLOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTSTACKEDCOLUMNS3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
{
  int r = (int)((double)color.r * factor);
  int g = (int)((double)color.g * factor);
  int b = (int)((double)color.b * factor);

  if(r > 255) r = 255;   if(r < 0) r = 0;
  if(g > 255) g = 255;   if(g < 0) g = 0;
  if(b > 255) b = 255;   if(b < 0) b = 0;

  return GRPSTATISTICSCHARTCOLOR((XBYTE)r, (XBYTE)g, (XBYTE)b, color.a);
}


