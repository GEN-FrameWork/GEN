/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartBars3D.cpp
* 
* @class      GRPSTATISTICSCHARTBARS3D
* @brief      Graphic Chart : 3D bars chart implementation (horizontal grouped prisms)
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

#include "GRPStatisticsChartBars3D.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBARS3D::GRPSTATISTICSCHARTBARS3D()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBARS3D::GRPSTATISTICSCHARTBARS3D()
{
  depthratio = 0.15;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBARS3D::~GRPSTATISTICSCHARTBARS3D()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBARS3D::~GRPSTATISTICSCHARTBARS3D()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBARS3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : back wall + floor, vertical value grid, grouped horizontal 3D bar prisms and labels
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
bool GRPSTATISTICSCHARTBARS3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats   = data.GetNCategories();
  XDWORD nseries = data.GetNSeries();

  if(!ncats || !nseries) return false;

  double rawmax  = data.GetMaxValue();
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

  // value grid (vertical, on the back wall) + value labels (X axis)

  GRPSTATISTICSCHARTSTYLE gridstyle;
  gridstyle.SetStroke(config.GetGridColor(), 1.0);

  GRPSTATISTICSCHARTTEXTSTYLE valuestyle;
  valuestyle.color  = config.GetTextColor();
  valuestyle.size   = axisfontsize;
  valuestyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

  for(int d=0; d<=ndiv; d++)
    {
      double value = (nicemax * (double)d) / (double)ndiv;
      double wx    = (fx0 + depthx) + ((value / nicemax) * fwidth);

      if(config.GetShowGrid())
        {
          builder.DrawLine(wx, ftopy - depthy, wx, basey - depthy, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          double lx = fx0 + ((value / nicemax) * fwidth);

          XSTRING label;
          label.Format(__L("%g"), value);

          builder.DrawText(lx, basey + (axisfontsize * 1.3), label.Get(), valuestyle);
        }
    }

  // grouped horizontal 3D bars (drawn from the top category = back to the bottom category = front)

  double slotheight  = fheight / (double)ncats;
  double grouppad    = slotheight * 0.12;
  double colgap      = slotheight * 0.04;
  double groupheight = slotheight - (grouppad * 2.0);
  double barh        = (groupheight - (colgap * (double)(nseries - 1))) / (double)nseries;

  GRPSTATISTICSCHARTTEXTSTYLE catstyle;
  catstyle.color  = config.GetTextColor();
  catstyle.size   = axisfontsize;
  catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

  for(int c=(int)ncats-1; c>=0; c--)
    {
      double sloty = ftopy + (slotheight * (double)c);

      for(int s=(int)nseries-1; s>=0; s--)
        {
          GRPSTATISTICSCHARTSERIE* serie = data.GetSerie((XDWORD)s);
          if(!serie) continue;

          double value = serie->GetValue((XDWORD)c);
          double barw  = (value / nicemax) * fwidth;
          double barx  = fx0;
          double bary  = sloty + grouppad + ((barh + colgap) * (double)s);

          GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor((XDWORD)s);

          DrawPrism(builder, barx, bary, barw, barh, depthx, depthy, color);

          if(config.GetShowValues())
            {
              XSTRING vlabel;
              vlabel.Format(__L("%g"), value);

              GRPSTATISTICSCHARTCOLOR valuecolor(255, 255, 255);

              GRPSTATISTICSCHARTTEXTSTYLE vstyle;
              vstyle.color  = valuecolor;                                        // white, over the front face
              vstyle.size   = fontsize * 0.85;
              vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

              builder.DrawText(barx + barw - 5.0, bary + (barh * 0.5) + (fontsize * 0.3), vlabel.Get(), vstyle);
            }
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING* category = data.GetCategory((XDWORD)c);
          if(category)
            {
              builder.DrawText(fx0 - (axisfontsize * 0.5), sloty + (slotheight * 0.5) + (axisfontsize * 0.35), category->Get(), catstyle);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBARS3D::DrawPrism(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double pw, double ph, double depthx, double depthy, GRPSTATISTICSCHARTCOLOR& color)
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
void GRPSTATISTICSCHARTBARS3D::DrawPrism(GRPSTATISTICSCHARTBUILDER& builder, double px, double py, double pw, double ph, double depthx, double depthy, GRPSTATISTICSCHARTCOLOR& color)
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
  double sideface[8]  = { frtx, frty, frbx, frby, brbx, brby, brtx, brty };     // right face (bar end)
  double frontface[8] = { flbx, flby, frbx, frby, frtx, frty, fltx, flty };     // front face

  builder.DrawPolygon(topface, 4, topstyle);
  builder.DrawPolygon(sideface, 4, sidestyle);
  builder.DrawPolygon(frontface, 4, frontstyle);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTBARS3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
* @brief      Make shade : scale a color by a factor (>1 lighter, <1 darker), clamped to 0..255
* @ingroup    GRAPHIC
* 
* @param[in]  color : base color
* @param[in]  factor : brightness factor
* 
* @return     GRPSTATISTICSCHARTCOLOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTBARS3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
{
  int r = (int)((double)color.r * factor);
  int g = (int)((double)color.g * factor);
  int b = (int)((double)color.b * factor);

  if(r > 255) r = 255;   if(r < 0) r = 0;
  if(g > 255) g = 255;   if(g < 0) g = 0;
  if(b > 255) b = 255;   if(b < 0) b = 0;

  return GRPSTATISTICSCHARTCOLOR((XBYTE)r, (XBYTE)g, (XBYTE)b, color.a);
}


