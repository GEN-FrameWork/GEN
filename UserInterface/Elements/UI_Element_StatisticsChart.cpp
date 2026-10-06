/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_StatisticsChart.cpp
* 
* @class      UI_ELEMENT_STATISTICSCHART
* @brief      User Interface Element Statistics Chart class
* @ingroup    USERINTERFACE
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

#include "UI_Element_StatisticsChart.h"

#include "UI_Color.h"
#include "UI_Layout.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "UI_PropertyRegistry.h"

#include "GRPBitmap.h"
#include "GRPFactory.h"
#include "GRP2DCanvas.h"
#include "GRP2DColor.h"
#include "GRPProperties.h"
#include "GRPVectorFile.h"
#include "GRP2DVectorFileRenderAGG.h"

#ifdef GRP_STATISTICSCHARS_ACTIVE
#include "GRPStatisticsChart.h"
#include "GRPStatisticsChartLines.h"
#include "GRPStatisticsChartColumns.h"
#include "GRPStatisticsChartArea.h"
#include "GRPStatisticsChartBars.h"
#include "GRPStatisticsChartPie.h"
#include "GRPStatisticsChartBuilderSVG.h"
#include "GRPStatisticsChartConfig.h"
#include "GRPStatisticsChartData.h"
#include "GRPStatisticsChartStyle.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_STATISTICSCHART::UI_ELEMENT_STATISTICSCHART()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_STATISTICSCHART::UI_ELEMENT_STATISTICSCHART()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_STATISTICSCHART);
  GetTypeString()->Set(_L("statisticschart"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_STATISTICSCHART::~UI_ELEMENT_STATISTICSCHART()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_STATISTICSCHART::~UI_ELEMENT_STATISTICSCHART()
{
  ClearData();
  DeleteBitmap();
  DeleteSharpBitmap();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_STATISTICSCHART_TYPE UI_ELEMENT_STATISTICSCHART::GetChartType()
* @brief      Get chart type
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_STATISTICSCHART_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_STATISTICSCHART_TYPE UI_ELEMENT_STATISTICSCHART::GetChartType()
{
  return charttype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE charttype)
* @brief      Set chart type
* @ingroup    USERINTERFACE
* 
* @param[in]  charttype : Charttype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE charttype)
{
  this->charttype = charttype;
  needsrebuild    = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_STATISTICSCHART::GetTitle()
* @brief      Get title
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_STATISTICSCHART::GetTitle()
{
  return &title;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::SetTitle(XCHAR* title)
* @brief      Set title
* @ingroup    USERINTERFACE
* 
* @param[in]  title : Title pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::SetTitle(XCHAR* title)
{
  this->title.Set(title);
  needsrebuild = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::ClearData()
* @brief      Clear data
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::ClearData()
{
  categories.DeleteContents();
  categories.DeleteAll();

  series.DeleteContents();
  series.DeleteAll();

  DeleteBitmap();
  needsrebuild = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::AddCategory(XCHAR* label)
* @brief      Add category
* @ingroup    USERINTERFACE
* 
* @param[in]  label : Label pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::AddCategory(XCHAR* label)
{
  XSTRING* category = GEN_NEW XSTRING(label ? label : _L(""));
  if(!category) return false;

  if(!categories.Add(category))
    {
      GEN_DELETE category;
      return false;
    }

  needsrebuild = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::AddSerie(XCHAR* name, XBYTE r, XBYTE g, XBYTE b)
* @brief      Add serie
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name pointer to use.
* @param[in]  r : Red value.
* @param[in]  g : Green value.
* @param[in]  b : Blue value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::AddSerie(XCHAR* name, XBYTE r, XBYTE g, XBYTE b)
{
  SERIE* serie = GEN_NEW SERIE();
  if(!serie) return false;

  if(name) serie->name.Set(name);
  serie->r = r;
  serie->g = g;
  serie->b = b;

  if(!series.Add(serie))
    {
      GEN_DELETE serie;
      return false;
    }

  needsrebuild = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::AddValue(XDWORD serieindex, double value)
* @brief      Add value
* @ingroup    USERINTERFACE
* 
* @param[in]  serieindex : Serieindex value.
* @param[in]  value : Value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::AddValue(XDWORD serieindex, double value)
{
  if(serieindex >= series.GetSize()) return false;

  SERIE* serie = series.Get(serieindex);
  if(!serie) return false;

  if(!serie->values.Add(value)) return false;

  needsrebuild = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::SetSerieFromSamples(XCHAR* seriename, const float* samples, int count, XBYTE r, XBYTE g, XBYTE b)
* @brief      Replace categories/series with one line serie built from a sample array
* @ingroup    USERINTERFACE
* 
* @param[in]  seriename : Seriename pointer to use.
* @param[in]  samples : Samples pointer to use.
* @param[in]  count : Count value.
* @param[in]  r : Red value.
* @param[in]  g : Green value.
* @param[in]  b : Blue value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::SetSerieFromSamples(XCHAR* seriename, const float* samples, int count, XBYTE r, XBYTE g, XBYTE b)
{
  if(!samples) return false;
  if(count < 2) return false;

  ClearData();

  if(charttype == UI_ELEMENT_STATISTICSCHART_TYPE_UNKNOWN)
    {
      charttype = UI_ELEMENT_STATISTICSCHART_TYPE_LINES;
    }

  for(int c=0; c<count; c++)
    {
      XSTRING label;

      if((c == 0) || (c == (count - 1)) || ((c % 5) == 0))
        {
          label.Format(_L("%d"), c + 1);
        }
       else
        {
          label.Set(_L(""));
        }

      if(!AddCategory(label.Get())) return false;
    }

  if(!AddSerie(seriename, r, g, b)) return false;

  for(int c=0; c<count; c++)
    {
      if(!AddValue(0, (double)samples[c])) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::SetColumnsFromValues(XCHAR* seriename, XCHAR** labels, const float* values, int count, XBYTE r, XBYTE g, XBYTE b)
* @brief      Replace categories/series with one columns serie (one bar per label)
* @ingroup    USERINTERFACE
* 
* @param[in]  seriename : Seriename pointer to use.
* @param[in]  labels : Labels pointer to use.
* @param[in]  values : Values pointer to use.
* @param[in]  count : Count value.
* @param[in]  r : Red value.
* @param[in]  g : Green value.
* @param[in]  b : Blue value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::SetColumnsFromValues(XCHAR* seriename, XCHAR** labels, const float* values, int count, XBYTE r, XBYTE g, XBYTE b)
{
  if(!labels) return false;
  if(!values) return false;
  if(count < 1) return false;

  ClearData();

  charttype = UI_ELEMENT_STATISTICSCHART_TYPE_COLUMNS;

  for(int c=0; c<count; c++)
    {
      if(!AddCategory(labels[c] ? labels[c] : _L(""))) return false;
    }

  if(!AddSerie(seriename, r, g, b)) return false;

  for(int c=0; c<count; c++)
    {
      if(!AddValue(0, (double)values[c])) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::HasData()
* @brief      True when at least one category and one serie are present
* @ingroup    USERINTERFACE
* 
* @return     bool : true if data is present; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::HasData()
{
  if(categories.IsEmpty()) return false;
  if(series.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::GetNeedsRebuild()
* @brief      Get needs rebuild
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::GetNeedsRebuild()
{
  return needsrebuild;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::SetNeedsRebuild(bool needsrebuild)
* @brief      Set needs rebuild
* @ingroup    USERINTERFACE
* 
* @param[in]  needsrebuild : Needsrebuild value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::SetNeedsRebuild(bool needsrebuild)
{
  this->needsrebuild = needsrebuild;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ELEMENT_STATISTICSCHART::GetBitmap()
* @brief      Get bitmap
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_STATISTICSCHART::GetBitmap()
{
  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::RebuildBitmap(GRP2DCANVAS* referencecanvas)
* @brief      Rebuild bitmap from StatisticsChars (SVG generate + offscreen raster)
* @ingroup    USERINTERFACE
* 
* @param[in]  referencecanvas : Referencecanvas pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::RebuildBitmap(GRP2DCANVAS* referencecanvas)
{
  #ifndef GRP_STATISTICSCHARS_ACTIVE
  return false;
  #else

  GRPBITMAP* built = NULL;
  if(!RasterizeChartBitmap(referencecanvas, 1.0, built)) return false;

  DeleteBitmap();
  bitmap       = built;
  needsrebuild = false;
  // Design rebuild implies sharp overlay is stale until EnsureSharpBitmap runs again.
  InvalidateSharpBitmap();

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ELEMENT_STATISTICSCHART::GetSharpBitmap()
* @brief      Fase 7: denser chart bitmap for live overlay (may be NULL).
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_STATISTICSCHART::GetSharpBitmap()
{
  return sharpbitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_STATISTICSCHART::InvalidateSharpBitmap()
* @brief      Fase 7: drop denser overlay cache (scale/data change).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::InvalidateSharpBitmap()
{
  DeleteSharpBitmap();
  sharp_density = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::EnsureSharpBitmap(GRP2DCANVAS* referencecanvas, double density)
* @brief      Fase 7: build/cache chart raster at design size × density for post-Present overlay.
* @ingroup    USERINTERFACE
* 
* @param[in]  referencecanvas : 
* @param[in]  density : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::EnsureSharpBitmap(GRP2DCANVAS* referencecanvas, double density)
{
  #ifndef GRP_STATISTICSCHARS_ACTIVE
  return false;
  #else

  if(density < UI_LAYOUT_UISCALE_MIN) density = UI_LAYOUT_UISCALE_MIN;

  if(sharpbitmap &&
     (!needsrebuild) &&
     (sharp_density > (density - UI_LAYOUT_UISCALE_EPSILON)) &&
     (sharp_density < (density + UI_LAYOUT_UISCALE_EPSILON)))
    {
      return true;
    }

  GRPBITMAP* built = NULL;
  if(!RasterizeChartBitmap(referencecanvas, density, built)) return false;

  DeleteSharpBitmap();
  sharpbitmap   = built;
  sharp_density = density;
  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::RasterizeChartBitmap(GRP2DCANVAS* referencecanvas, double density, GRPBITMAP*& out_bitmap)
* @brief      Generate SVG chart and rasterize at BoundaryLine size × density.
* @note       INTERNAL. Does not touch NeedsRebuild / design bitmap ownership.
* @ingroup    USERINTERFACE
* 
* @param[in]  referencecanvas : 
* @param[in]  density : 
* @param[in]  out_bitmap : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::RasterizeChartBitmap(GRP2DCANVAS* referencecanvas, double density, GRPBITMAP*& out_bitmap)
{
  #ifndef GRP_STATISTICSCHARS_ACTIVE
  out_bitmap = NULL;
  return false;
  #else

  out_bitmap = NULL;
  if(!referencecanvas) return false;
  if(!HasData()) return false;

  bool needspair = ((charttype == UI_ELEMENT_STATISTICSCHART_TYPE_LINES) ||
                    (charttype == UI_ELEMENT_STATISTICSCHART_TYPE_AREA)  ||
                    (charttype == UI_ELEMENT_STATISTICSCHART_TYPE_UNKNOWN));

  if(needspair && (categories.GetSize() < 2)) return false;
  if((!needspair) && categories.IsEmpty()) return false;
  if(series.IsEmpty()) return false;

  double chartwidth  = GetBoundaryLine()->width;
  double chartheight = GetBoundaryLine()->height;

  if(chartwidth  <= 0.0) chartwidth  = 1.0;
  if(chartheight <= 0.0) chartheight = 1.0;
  if(density < UI_LAYOUT_UISCALE_MIN) density = UI_LAYOUT_UISCALE_MIN;

  chartwidth  *= density;
  chartheight *= density;

  GRPSTATISTICSCHART* chart = NULL;

  switch(charttype)
    {
      case UI_ELEMENT_STATISTICSCHART_TYPE_COLUMNS : chart = GEN_NEW GRPSTATISTICSCHARTCOLUMNS(); break;
      case UI_ELEMENT_STATISTICSCHART_TYPE_AREA    : chart = GEN_NEW GRPSTATISTICSCHARTAREA();    break;
      case UI_ELEMENT_STATISTICSCHART_TYPE_BARS    : chart = GEN_NEW GRPSTATISTICSCHARTBARS();    break;
      case UI_ELEMENT_STATISTICSCHART_TYPE_PIE     : chart = GEN_NEW GRPSTATISTICSCHARTPIE();     break;

      case UI_ELEMENT_STATISTICSCHART_TYPE_UNKNOWN :
      case UI_ELEMENT_STATISTICSCHART_TYPE_LINES   :
                            default                : chart = GEN_NEW GRPSTATISTICSCHARTLINES();   break;
    }

  if(!chart) return false;

  GRPSTATISTICSCHARTCONFIG* config = chart->GetConfig();
  if(config)
    {
      if(!title.IsEmpty()) config->SetTitle(title.Get());

      config->SetTitleFontSize(14.0 * density);
      config->SetFontSize(11.0 * density);
      config->SetAxisFontSize(10.0 * density);
      config->SetShowValues(false);
      config->SetShowLegend(false);
      config->SetShowGrid(true);
      config->SetShowAxisLabels(true);
      config->SetMargin(12.0 * density);

      bool usedcssbg = false;
      if(IsBackgroundColorSet())
        {
          UI_COLOR* bgcolorui = GetBackgroundColor();
          if(bgcolorui && (bgcolorui->GetAlpha() > 0))
            {
              GRPSTATISTICSCHARTCOLOR bgcolor((XBYTE)bgcolorui->GetRed(), (XBYTE)bgcolorui->GetGreen(), (XBYTE)bgcolorui->GetBlue(), (XBYTE)bgcolorui->GetAlpha());
              config->SetBackgroundColor(bgcolor);
              usedcssbg = true;
            }
        }

      if(!usedcssbg)
        {
          GRPSTATISTICSCHARTCOLOR bgcolor(22, 27, 34, 255);
          config->SetBackgroundColor(bgcolor);
        }

      config->GetAxisColor().Set(110, 118, 129);
      config->GetGridColor().Set(48, 54, 61);
      config->GetTextColor().Set(201, 209, 217);

      UI_COLOR* accent = GetColor();
      if(accent && accent->IsValid())
        {
          config->GetPaletteColor(0).Set((XBYTE)accent->GetRed(), (XBYTE)accent->GetGreen(), (XBYTE)accent->GetBlue());
        }
    }

  GRPSTATISTICSCHARTDATA* data = chart->GetData();
  if(!data)
    {
      GEN_DELETE chart;
      return false;
    }

  for(XDWORD c=0; c<categories.GetSize(); c++)
    {
      XSTRING* category = categories.Get(c);
      data->AddCategory(category ? category->Get() : _L(""));
    }

  for(XDWORD s=0; s<series.GetSize(); s++)
    {
      SERIE* src = series.Get(s);
      if(!src) continue;

      GRPSTATISTICSCHARTSERIE* dst = data->AddSerie(src->name.Get());
      if(!dst) continue;

      GRPSTATISTICSCHARTCOLOR color(src->r, src->g, src->b);
      dst->SetColor(color);

      for(XDWORD v=0; v<src->values.GetSize(); v++)
        {
          dst->AddValue(src->values.Get(v));
        }
    }

  GRPSTATISTICSCHARTBUILDERSVG builder;

  if(chart->Generate(builder, chartwidth, chartheight) != GRPVECTORFILERESULT_OK)
    {
      GEN_DELETE chart;
      return false;
    }

  XSTRING svgcontent;
  if(!builder.GetResult(svgcontent))
    {
      GEN_DELETE chart;
      return false;
    }

  GEN_DELETE chart;

  GRPVECTORFILE* vectorfile = GRPVECTORFILE::CreateInstance(GRPVECTORFILETYPE_SVG, svgcontent);
  if(!vectorfile) return false;

  GRPPROPERTIES properties;
  properties.CopyPropertysFrom(referencecanvas);
  properties.SetPosition(0, 0);
  properties.SetSize((XDWORD)(chartwidth + 0.5), (XDWORD)(chartheight + 0.5));

  GRP2DCANVAS* offscreen = GEN_GRPFACTORY.CreateCanvas(&properties);
  if(!offscreen)
    {
      GEN_DELETE vectorfile;
      return false;
    }

  offscreen->SetWidth((XDWORD)(chartwidth + 0.5));
  offscreen->SetHeight((XDWORD)(chartheight + 0.5));

  if(!offscreen->Buffer_Create())
    {
      GEN_GRPFACTORY.DeleteCanvas(offscreen);
      GEN_DELETE vectorfile;
      return false;
    }

  offscreen->VectorFont_CopyFrom(referencecanvas);

  GRP2DCOLOR_RGBA8 transparent(0, 0, 0, 0);
  offscreen->Clear(&transparent);

  GRP2DVECTORFILERENDERAGG render;
  bool status = render.Render(vectorfile, offscreen, 0.0, 0.0, chartwidth, chartheight);

  GRPBITMAP* newbitmap = NULL;
  if(status)
    {
      newbitmap = offscreen->GetBitmap(0.0, 0.0, chartwidth, chartheight);
    }

  GEN_GRPFACTORY.DeleteCanvas(offscreen);
  GEN_DELETE vectorfile;

  if(!newbitmap) return false;

  out_bitmap = newbitmap;
  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE UI_ELEMENT_STATISTICSCHART::GetAlpha()
* @brief      Get alpha
* @ingroup    USERINTERFACE
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE UI_ELEMENT_STATISTICSCHART::GetAlpha()
{
  return alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::SetAlpha(XBYTE alpha)
* @brief      Set alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  alpha : Alpha value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::SetAlpha(XBYTE alpha)
{
  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::DeleteBitmap()
* @brief      Delete bitmap
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::DeleteBitmap()
{
  if(!bitmap) return false;

  GEN_GRPFACTORY.DeleteBitmap(bitmap);
  bitmap = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_STATISTICSCHART::DeleteSharpBitmap()
* @brief      Fase 7: free denser overlay cache.
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_STATISTICSCHART::DeleteSharpBitmap()
{
  if(!sharpbitmap) return false;

  GEN_GRPFACTORY.DeleteBitmap(sharpbitmap);
  sharpbitmap = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::Clean()
{
  charttype     = UI_ELEMENT_STATISTICSCHART_TYPE_LINES;
  needsrebuild  = true;
  bitmap        = NULL;
  sharpbitmap   = NULL;
  sharp_density = 0.0;
  alpha         = 100;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_STATISTICSCHART::ReapplyStyleVisual()
* @brief      Re-read color/background-color from the stylesheet and mark the bitmap dirty
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_STATISTICSCHART::ReapplyStyleVisual()
{
  UI_ELEMENT::ReapplyStyleVisual();

  UI_LAYOUT*     layout = GetLayout();
  UI_STYLESHEET* sheet  = layout ? layout->GetStyleSheet() : NULL;
  if(!sheet) return;

  UI_STYLE bag;
  bag.FillFromCSSDeclarations(sheet, this);

  XSTRING v;
  if(UI_PROPERTYREGISTRY::GetAliased(bag, _L("bckgrdcolor"), _L("background-color"), v))
    {
      if(GetBackgroundColor()->SetFromString(v))
        {
          SetBackgroundColorSet(true);
          needsrebuild = true;
        }
    }

  if(bag.Get(_L("color"), v))
    {
      if(GetColor()->SetFromString(v))
        {
          needsrebuild = true;
        }
    }
}
