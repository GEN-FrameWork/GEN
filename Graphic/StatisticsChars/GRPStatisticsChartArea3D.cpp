/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartArea3D.cpp
* 
* @class      GRPSTATISTICSCHARTAREA3D
* @brief      Graphic Chart : 3D area chart implementation (depth lane slabs)
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

#include "GRPStatisticsChartArea3D.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTAREA3D::GRPSTATISTICSCHARTAREA3D()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTAREA3D::GRPSTATISTICSCHARTAREA3D()
{
  depthratio = 0.15;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTAREA3D::~GRPSTATISTICSCHARTAREA3D()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTAREA3D::~GRPSTATISTICSCHARTAREA3D()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTAREA3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : back wall + floor, value grid, one extruded area slab per series (in its depth lane) and labels
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
bool GRPSTATISTICSCHARTAREA3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
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

  // depth lane area slabs (one per series, drawn from the back lane to the front lane)

  double leftgap    = depth + (axisfontsize * 0.6);
  double colareax   = fx0 + leftgap;
  double colareaw   = fwidth - leftgap;

  if(colareaw <= 0.0) return false;

  double slotwidth  = colareaw / (double)ncats;
  double banddepth  = depth / (double)nseries;

  for(int s=(int)nseries-1; s>=0; s--)
    {
      GRPSTATISTICSCHARTSERIE* serie = data.GetSerie((XDWORD)s);
      if(!serie) continue;

      GRPSTATISTICSCHARTCOLOR color     = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor((XDWORD)s);
      GRPSTATISTICSCHARTCOLOR topcolor  = MakeShade(color, 1.15);
      GRPSTATISTICSCHARTCOLOR sidecolor = MakeShade(color, 0.72);
      GRPSTATISTICSCHARTCOLOR edge(70, 70, 70);

      double zfront = (double)s * banddepth;
      double zback  = zfront + (banddepth * 0.7);

      //  top face (band that follows the values through the lane depth)

      GRPSTATISTICSCHARTSTYLE topstyle;
      topstyle.SetFill(topcolor);
      topstyle.SetStroke(edge, 0.5);

      for(XDWORD c=0; (c + 1)<ncats; c++)
        {
          double v0 = serie->GetValue(c);
          double v1 = serie->GetValue(c + 1);

          double x0 = colareax + (slotwidth * ((double)c + 0.5));
          double x1 = colareax + (slotwidth * ((double)(c + 1) + 0.5));
          double y0 = basey - ((v0 / nicemax) * fheight);
          double y1 = basey - ((v1 / nicemax) * fheight);

          double topface[8] = { x0 + zfront, y0 - zfront, x1 + zfront, y1 - zfront,
                                x1 + zback,  y1 - zback,  x0 + zback,  y0 - zback };

          builder.DrawPolygon(topface, 4, topstyle);
        }

      //  right side cap (closes the slab at the last category, drawn behind the front face)

      double vlast = serie->GetValue(ncats - 1);
      double xlast = colareax + (slotwidth * ((double)(ncats - 1) + 0.5));
      double ylast = basey - ((vlast / nicemax) * fheight);

      GRPSTATISTICSCHARTSTYLE sidestyle;
      sidestyle.SetFill(sidecolor);
      sidestyle.SetStroke(edge, 0.5);

      double sideface[8] = { xlast + zfront, ylast - zfront, xlast + zback, ylast - zback,
                             xlast + zback,  basey - zback,  xlast + zfront, basey - zfront };

      builder.DrawPolygon(sideface, 4, sidestyle);

      //  front face (the area profile at the lane front)

      XVECTOR<double> points;

      points.Add(colareax + (slotwidth * 0.5) + zfront);
      points.Add(basey - zfront);

      for(XDWORD c=0; c<ncats; c++)
        {
          double value = serie->GetValue(c);
          double px    = colareax + (slotwidth * ((double)c + 0.5));
          double py    = basey - ((value / nicemax) * fheight);

          points.Add(px + zfront);
          points.Add(py - zfront);
        }

      points.Add(xlast + zfront);
      points.Add(basey - zfront);

      GRPSTATISTICSCHARTSTYLE frontstyle;
      frontstyle.SetFill(color);
      frontstyle.SetStroke(edge, 0.5);

      builder.DrawPolygon(points.GetPointer(0), points.GetSize() / 2, frontstyle);

      points.DeleteAll();

      //  value labels (optional) on the front profile

      if(config.GetShowValues())
        {
          GRPSTATISTICSCHARTTEXTSTYLE vstyle;
          vstyle.color  = config.GetTextColor();
          vstyle.size   = fontsize * 0.8;
          vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

          for(XDWORD c=0; c<ncats; c++)
            {
              double value = serie->GetValue(c);
              double px    = colareax + (slotwidth * ((double)c + 0.5)) + zfront;
              double py    = (basey - ((value / nicemax) * fheight)) - zfront;

              XSTRING vlabel;
              vlabel.Format(_L("%g"), value);

              builder.DrawText(px, py - 3.0, vlabel.Get(), vstyle);
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
* @fn         GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTAREA3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
* @brief      Make shade : scale a color by a factor (>1 lighter, <1 darker), clamped to 0..255
* @ingroup    GRAPHIC
* 
* @param[in]  color : base color
* @param[in]  factor : brightness factor
* 
* @return     GRPSTATISTICSCHARTCOLOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTAREA3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
{
  int r = (int)((double)color.r * factor);
  int g = (int)((double)color.g * factor);
  int b = (int)((double)color.b * factor);

  if(r > 255) r = 255;   if(r < 0) r = 0;
  if(g > 255) g = 255;   if(g < 0) g = 0;
  if(b > 255) b = 255;   if(b < 0) b = 0;

  return GRPSTATISTICSCHARTCOLOR((XBYTE)r, (XBYTE)g, (XBYTE)b, color.a);
}


