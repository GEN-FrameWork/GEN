/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartPie3D.cpp
* 
* @class      GRPSTATISTICSCHARTPIE3D
* @brief      Graphic Chart : 3D pie chart implementation (tilted ellipse with thickness)
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

#include <math.h>

#include "GRPStatisticsChartPie3D.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPSTATISTICSCHARTPIE3D_PI        3.14159265358979323846
#define GRPSTATISTICSCHARTPIE3D_DEG2RAD   (GRPSTATISTICSCHARTPIE3D_PI / 180.0)


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTPIE3D::GRPSTATISTICSCHARTPIE3D()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTPIE3D::GRPSTATISTICSCHARTPIE3D()
{
  tiltratio      = 0.55;
  thicknessratio = 0.16;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTPIE3D::~GRPSTATISTICSCHARTPIE3D()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTPIE3D::~GRPSTATISTICSCHARTPIE3D()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTPIE3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : a tilted pie (top faces) with a front wall thickness, one sector per category (first serie)
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
bool GRPSTATISTICSCHARTPIE3D::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats = data.GetNCategories();

  if(!ncats) return false;

  GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(0);

  if(!serie) return false;

  double total = 0.0;

  for(XDWORD c=0; c<ncats; c++)
    {
      double value = serie->GetValue(c);
      if(value > 0.0) total += value;
    }

  if(total <= 0.0) return false;

  double fontsize = config.GetFontSize();

  double rx    = (width * 0.5) * 0.9;
  double ry    = rx * tiltratio;
  double thick = rx * thicknessratio;

  double totalh = (2.0 * ry) + thick;                                          // fit the tilted pie + thickness vertically

  if(totalh > (height * 0.9))
    {
      double scale = (height * 0.9) / totalh;

      rx    *= scale;
      ry    *= scale;
      thick *= scale;
    }

  double cx = x + (width * 0.5);
  double cy = y + (((height - ((2.0 * ry) + thick)) * 0.5) + ry);              // center of the top ellipse

  //  pass 1 : top faces (all sectors)

  double startdeg = -90.0;

  for(XDWORD c=0; c<ncats; c++)
    {
      double value = serie->GetValue(c);
      if(value <= 0.0) continue;

      double sweep  = (value / total) * 360.0;
      double enddeg = startdeg + sweep;

      GRPSTATISTICSCHARTCOLOR color = config.GetPaletteColor(c);

      DrawSectorTop(builder, cx, cy, rx, ry, startdeg, enddeg, color);

      startdeg = enddeg;
    }

  //  pass 2 : front walls (thickness), only the front half of the rim (sin > 0)

  startdeg = -90.0;

  for(XDWORD c=0; c<ncats; c++)
    {
      double value = serie->GetValue(c);
      if(value <= 0.0) continue;

      double sweep  = (value / total) * 360.0;
      double enddeg = startdeg + sweep;

      GRPSTATISTICSCHARTCOLOR color = config.GetPaletteColor(c);

      DrawSectorWall(builder, cx, cy, rx, ry, thick, startdeg, enddeg, color);

      startdeg = enddeg;
    }

  //  value labels on the top faces

  if(config.GetShowValues())
    {
      startdeg = -90.0;

      GRPSTATISTICSCHARTTEXTSTYLE vstyle;
      vstyle.color  = config.GetTextColor();
      vstyle.size   = fontsize * 0.9;
      vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

      for(XDWORD c=0; c<ncats; c++)
        {
          double value = serie->GetValue(c);
          if(value <= 0.0) continue;

          double sweep  = (value / total) * 360.0;
          double enddeg = startdeg + sweep;
          double middeg = (startdeg + enddeg) * 0.5;
          double midrad = middeg * GRPSTATISTICSCHARTPIE3D_DEG2RAD;

          double lx = cx + (rx * 0.6 * cos(midrad));
          double ly = cy + (ry * 0.6 * sin(midrad));

          XSTRING vlabel;
          vlabel.Format(_L("%g"), value);

          builder.DrawText(lx, ly + (fontsize * 0.3), vlabel.Get(), vstyle);

          startdeg = enddeg;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSTATISTICSCHARTPIE3D::GetLegendCount()
* @brief      Get legend count : one legend entry per category (a pie legend describes the sectors)
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSTATISTICSCHARTPIE3D::GetLegendCount()
{
  return data.GetNCategories();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTPIE3D::GetLegendEntry(XDWORD index, XSTRING** name, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Get legend entry : category name + sector color
* @ingroup    GRAPHIC
* 
* @param[in]  index : entry index
* @param[in]  name : category name (not owned)
* @param[in]  color : sector color
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTPIE3D::GetLegendEntry(XDWORD index, XSTRING** name, GRPSTATISTICSCHARTCOLOR& color)
{
  XSTRING* category = data.GetCategory(index);

  if(!category) return false;

  *name = category;
  color = config.GetPaletteColor(index);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTPIE3D::DrawSectorTop(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Draw sector top : the (elliptical) top face of one sector (center + arc)
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  cx : ellipse center x
* @param[in]  cy : ellipse center y
* @param[in]  rx : horizontal radius
* @param[in]  ry : vertical radius (tilted)
* @param[in]  startdeg : start angle in degrees
* @param[in]  enddeg : end angle in degrees
* @param[in]  color : sector color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTPIE3D::DrawSectorTop(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color)
{
  double sweep = enddeg - startdeg;
  int    steps = (int)(sweep / 5.0);

  if(steps < 2) steps = 2;

  GRPSTATISTICSCHARTCOLOR sep(255, 255, 255);

  GRPSTATISTICSCHARTSTYLE style;
  style.SetFill(color);
  style.SetStroke(sep, 1.0);

  XVECTOR<double> points;

  points.Add(cx);
  points.Add(cy);

  for(int i=0; i<=steps; i++)
    {
      double a = (startdeg + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE3D_DEG2RAD;

      points.Add(cx + (rx * cos(a)));
      points.Add(cy + (ry * sin(a)));
    }

  builder.DrawPolygon(points.GetPointer(0), points.GetSize() / 2, style);

  points.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTPIE3D::DrawSectorWall(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double thick, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Draw sector wall : the outer side wall (thickness) for the front half of a sector rim (sin > 0)
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  cx : ellipse center x
* @param[in]  cy : ellipse center y
* @param[in]  rx : horizontal radius
* @param[in]  ry : vertical radius (tilted)
* @param[in]  thick : wall height (pie thickness)
* @param[in]  startdeg : start angle in degrees
* @param[in]  enddeg : end angle in degrees
* @param[in]  color : sector color
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTPIE3D::DrawSectorWall(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double thick, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color)
{
  double fa0 = (startdeg > 0.0)   ? startdeg : 0.0;                             // clip the sector to the front half [0, 180]
  double fa1 = (enddeg   < 180.0) ? enddeg   : 180.0;

  if(fa0 >= fa1) return;                                                        // nothing on the front rim

  double sweep = fa1 - fa0;
  int    steps = (int)(sweep / 4.0);

  if(steps < 1) steps = 1;

  GRPSTATISTICSCHARTCOLOR wallcolor = MakeShade(color, 0.60);

  GRPSTATISTICSCHARTSTYLE style;
  style.SetFill(wallcolor);

  //  one polygon per sector wall (top arc left->right, then bottom arc right->left) : no internal seams

  XVECTOR<double> points;

  for(int i=0; i<=steps; i++)
    {
      double a = (fa0 + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE3D_DEG2RAD;

      points.Add(cx + (rx * cos(a)));
      points.Add(cy + (ry * sin(a)));
    }

  for(int i=steps; i>=0; i--)
    {
      double a = (fa0 + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE3D_DEG2RAD;

      points.Add(cx + (rx * cos(a)));
      points.Add(cy + (ry * sin(a)) + thick);
    }

  builder.DrawPolygon(points.GetPointer(0), points.GetSize() / 2, style);

  points.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTPIE3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
* @brief      Make shade : scale a color by a factor (>1 lighter, <1 darker), clamped to 0..255
* @ingroup    GRAPHIC
* 
* @param[in]  color : base color
* @param[in]  factor : brightness factor
* 
* @return     GRPSTATISTICSCHARTCOLOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTCOLOR GRPSTATISTICSCHARTPIE3D::MakeShade(GRPSTATISTICSCHARTCOLOR& color, double factor)
{
  int r = (int)((double)color.r * factor);
  int g = (int)((double)color.g * factor);
  int b = (int)((double)color.b * factor);

  if(r > 255) r = 255;   if(r < 0) r = 0;
  if(g > 255) g = 255;   if(g < 0) g = 0;
  if(b > 255) b = 255;   if(b < 0) b = 0;

  return GRPSTATISTICSCHARTCOLOR((XBYTE)r, (XBYTE)g, (XBYTE)b, color.a);
}


