/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartStackedColumns.cpp
* 
* @class      GRPSTATISTICSCHARTSTACKEDCOLUMNS
* @brief      Graphic Chart : stacked columns chart implementation
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

#include "GRPStatisticsChartStackedColumns.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTSTACKEDCOLUMNS::GRPSTATISTICSCHARTSTACKEDCOLUMNS()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTSTACKEDCOLUMNS::GRPSTATISTICSCHARTSTACKEDCOLUMNS()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTSTACKEDCOLUMNS::~GRPSTATISTICSCHARTSTACKEDCOLUMNS()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTSTACKEDCOLUMNS::~GRPSTATISTICSCHARTSTACKEDCOLUMNS()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTSTACKEDCOLUMNS::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : grid, axes, one stacked column per category (series segments) and labels
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
bool GRPSTATISTICSCHARTSTACKEDCOLUMNS::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
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

  // value grid + value labels (Y axis)

  GRPSTATISTICSCHARTSTYLE gridstyle;
  gridstyle.SetStroke(config.GetGridColor(), 1.0);

  GRPSTATISTICSCHARTTEXTSTYLE valuestyle;
  valuestyle.color  = config.GetTextColor();
  valuestyle.size   = axisfontsize;
  valuestyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

  for(int d=0; d<=ndiv; d++)
    {
      double value = (nicemax * (double)d) / (double)ndiv;
      double yy    = y + height - ((value / nicemax) * height);

      if(config.GetShowGrid())
        {
          builder.DrawLine(x, yy, x + width, yy, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING label;
          label.Format(__L("%g"), value);

          builder.DrawText(x - (axisfontsize * 0.5), yy + (axisfontsize * 0.35), label.Get(), valuestyle);
        }
    }

  // axes

  GRPSTATISTICSCHARTSTYLE axisstyle;
  axisstyle.SetStroke(config.GetAxisColor(), 1.5);

  builder.DrawLine(x, y + height, x + width, y + height, axisstyle);            // X axis
  builder.DrawLine(x, y, x, y + height, axisstyle);            // Y axis

  // stacked columns (one column per category, series stacked from the baseline up)

  double slotwidth = width / (double)ncats;
  double colpad    = slotwidth * 0.2;
  double colwidth  = slotwidth - (colpad * 2.0);

  GRPSTATISTICSCHARTTEXTSTYLE catstyle;
  catstyle.color  = config.GetTextColor();
  catstyle.size   = axisfontsize;
  catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

  for(XDWORD c=0; c<ncats; c++)
    {
      double slotx = x + (slotwidth * (double)c);
      double colx  = slotx + colpad;
      double cumy  = y + height;                                                // running top of the stack (starts at baseline)

      for(XDWORD s=0; s<nseries; s++)
        {
          GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
          if(!serie) continue;

          double value = serie->GetValue(c);
          double segh  = (value / nicemax) * height;
          double segy  = cumy - segh;

          GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor(s);

          GRPSTATISTICSCHARTSTYLE segstyle;
          segstyle.SetFill(color);

          builder.DrawRect(colx, segy, colwidth, segh, segstyle);

          if(config.GetShowValues() && (segh > fontsize))                       // only when the segment is tall enough
            {
              XSTRING vlabel;
              vlabel.Format(__L("%g"), value);

              GRPSTATISTICSCHARTTEXTSTYLE vstyle;
              vstyle.color  = config.GetTextColor();
              vstyle.size   = fontsize * 0.9;
              vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

              builder.DrawText(colx + (colwidth * 0.5), segy + (segh * 0.5) + (fontsize * 0.3), vlabel.Get(), vstyle);
            }

          cumy = segy;
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING* category = data.GetCategory(c);
          if(category)
            {
              builder.DrawText(slotx + (slotwidth * 0.5), y + height + (axisfontsize * 1.3), category->Get(), catstyle);
            }
        }
    }

  return true;
}


