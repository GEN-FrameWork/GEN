/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartPie.cpp
* 
* @class      GRPSTATISTICSCHARTPIE
* @brief      Graphic Chart : pie / donut chart implementation
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

#include "GRPStatisticsChartPie.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPSTATISTICSCHARTPIE_PI          3.14159265358979323846
#define GRPSTATISTICSCHARTPIE_DEG2RAD     (GRPSTATISTICSCHARTPIE_PI / 180.0)


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTPIE::GRPSTATISTICSCHARTPIE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTPIE::GRPSTATISTICSCHARTPIE()
{
  innerradiusratio = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTPIE::~GRPSTATISTICSCHARTPIE()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTPIE::~GRPSTATISTICSCHARTPIE()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTPIE::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : one sector per category (values taken from the first serie), with optional donut hole
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
bool GRPSTATISTICSCHARTPIE::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
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

  double cx     = x + (width  * 0.5);
  double cy     = y + (height * 0.5);
  double router = ((width < height) ? width : height) * 0.5 * 0.9;
  double rinner = router * innerradiusratio;

  double fontsize = config.GetFontSize();
  double startdeg = -90.0;                                                      // first sector starts at the top

  for(XDWORD c=0; c<ncats; c++)
    {
      double value = serie->GetValue(c);
      if(value <= 0.0) continue;

      double sweep  = (value / total) * 360.0;
      double enddeg = startdeg + sweep;

      GRPSTATISTICSCHARTCOLOR color = config.GetPaletteColor(c);
      GRPSTATISTICSCHARTCOLOR sep(255, 255, 255);

      GRPSTATISTICSCHARTSTYLE style;
      style.SetFill(color);
      style.SetStroke(sep, 1.5);                                                // thin separator between sectors

      DrawSector(builder, cx, cy, router, rinner, startdeg, enddeg, style);

      if(config.GetShowValues())
        {
          double middeg = (startdeg + enddeg) * 0.5;
          double midrad = middeg * GRPSTATISTICSCHARTPIE_DEG2RAD;
          double lr     = rinner + ((router - rinner) * 0.6);
          double lx     = cx + (lr * cos(midrad));
          double ly     = cy + (lr * sin(midrad));

          XSTRING vlabel;
          vlabel.Format(_L("%g"), value);

          GRPSTATISTICSCHARTTEXTSTYLE vstyle;
          vstyle.color  = config.GetTextColor();
          vstyle.size   = fontsize * 0.9;
          vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

          builder.DrawText(lx, ly + (fontsize * 0.3), vlabel.Get(), vstyle);
        }

      startdeg = enddeg;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSTATISTICSCHARTPIE::GetLegendCount()
* @brief      Get legend count : one legend entry per category (a pie legend describes the sectors)
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSTATISTICSCHARTPIE::GetLegendCount()
{
  return data.GetNCategories();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTPIE::GetLegendEntry(XDWORD index, XSTRING** name, GRPSTATISTICSCHARTCOLOR& color)
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
bool GRPSTATISTICSCHARTPIE::GetLegendEntry(XDWORD index, XSTRING** name, GRPSTATISTICSCHARTCOLOR& color)
{
  XSTRING* category = data.GetCategory(index);

  if(!category) return false;

  *name = category;
  color = config.GetPaletteColor(index);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTPIE::DrawSector(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double router, double rinner, double startdeg, double enddeg, GRPSTATISTICSCHARTSTYLE& style)
* @brief      Draw sector : a filled pie slice (rinner = 0) or donut ring segment, arc approximated by segments
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  cx : center x
* @param[in]  cy : center y
* @param[in]  router : outer radius
* @param[in]  rinner : inner radius (0 for a full slice)
* @param[in]  startdeg : start angle in degrees
* @param[in]  enddeg : end angle in degrees
* @param[in]  style : fill / stroke style
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTPIE::DrawSector(GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double router, double rinner, double startdeg, double enddeg, GRPSTATISTICSCHARTSTYLE& style)
{
  double sweep = enddeg - startdeg;
  int    steps = (int)(sweep / 5.0);                                            // about one segment every 5 degrees

  if(steps < 2) steps = 2;

  XVECTOR<double> points;

  if(rinner <= 0.0)
    {
      points.Add(cx);                                                           // center vertex
      points.Add(cy);

      for(int i=0; i<=steps; i++)
        {
          double a = (startdeg + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE_DEG2RAD;

          points.Add(cx + (router * cos(a)));
          points.Add(cy + (router * sin(a)));
        }
    }
   else
    {
      for(int i=0; i<=steps; i++)                                               // outer arc : start -> end
        {
          double a = (startdeg + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE_DEG2RAD;

          points.Add(cx + (router * cos(a)));
          points.Add(cy + (router * sin(a)));
        }

      for(int i=steps; i>=0; i--)                                               // inner arc : end -> start
        {
          double a = (startdeg + ((sweep * (double)i) / (double)steps)) * GRPSTATISTICSCHARTPIE_DEG2RAD;

          points.Add(cx + (rinner * cos(a)));
          points.Add(cy + (rinner * sin(a)));
        }
    }

  builder.DrawPolygon(points.GetPointer(0), points.GetSize() / 2, style);

  points.DeleteAll();
}


