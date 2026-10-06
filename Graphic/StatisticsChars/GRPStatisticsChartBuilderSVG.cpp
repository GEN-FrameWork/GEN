/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartBuilderSVG.cpp
* 
* @class      GRPSTATISTICSCHARTBUILDERSVG
* @brief      Graphic Chart : SVG output builder implementation
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

#include "GRPStatisticsChartBuilderSVG.h"

#include "XString.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBUILDERSVG::GRPSTATISTICSCHARTBUILDERSVG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBUILDERSVG::GRPSTATISTICSCHARTBUILDERSVG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBUILDERSVG::~GRPSTATISTICSCHARTBUILDERSVG()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBUILDERSVG::~GRPSTATISTICSCHARTBUILDERSVG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTOUTPUTFORMAT GRPSTATISTICSCHARTBUILDERSVG::GetFormat()
* @brief      Get format
* @ingroup    GRAPHIC
* 
* @return     GRPSTATISTICSCHARTOUTPUTFORMAT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTOUTPUTFORMAT GRPSTATISTICSCHARTBUILDERSVG::GetFormat()
{
  return GRPSTATISTICSCHARTOUTPUTFORMAT_SVG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILETYPE GRPSTATISTICSCHARTBUILDERSVG::GetVectorFileType()
* @brief      Get vector file type to render the result
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILETYPE GRPSTATISTICSCHARTBUILDERSVG::GetVectorFileType()
{
  return GRPVECTORFILETYPE_SVG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::BeginDocument(double width, double height)
* @brief      Begin document : open the SVG root element
* @ingroup    GRAPHIC
* 
* @param[in]  width : document width
* @param[in]  height : document height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::BeginDocument(double width, double height)
{
  content.Empty();

  content.Add(_L("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));

  content.Add(_L("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""));
  AppendNumber(content, width);
  content.Add(_L("\" height=\""));
  AppendNumber(content, height);
  content.Add(_L("\" viewBox=\"0 0 "));
  AppendNumber(content, width);
  content.Add(_C(' '));
  AppendNumber(content, height);
  content.Add(_L("\">\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::EndDocument()
* @brief      End document : close the SVG root element
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::EndDocument()
{
  content.Add(_L("</svg>\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::DrawRect(double x, double y, double width, double height, GRPSTATISTICSCHARTSTYLE& style)
* @brief      Draw rectangle
* @ingroup    GRAPHIC
* 
* @param[in]  x : left
* @param[in]  y : top
* @param[in]  width : width
* @param[in]  height : height
* @param[in]  style : fill / stroke style
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::DrawRect(double x, double y, double width, double height, GRPSTATISTICSCHARTSTYLE& style)
{
  content.Add(_L(" <rect x=\""));
  AppendNumber(content, x);
  content.Add(_L("\" y=\""));
  AppendNumber(content, y);
  content.Add(_L("\" width=\""));
  AppendNumber(content, width);
  content.Add(_L("\" height=\""));
  AppendNumber(content, height);
  content.Add(_C('"'));

  AppendFillAndStroke(content, style);

  content.Add(_L("/>\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::DrawLine(double x1, double y1, double x2, double y2, GRPSTATISTICSCHARTSTYLE& style)
* @brief      Draw line
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : start x
* @param[in]  y1 : start y
* @param[in]  x2 : end x
* @param[in]  y2 : end y
* @param[in]  style : stroke style
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::DrawLine(double x1, double y1, double x2, double y2, GRPSTATISTICSCHARTSTYLE& style)
{
  content.Add(_L(" <line x1=\""));
  AppendNumber(content, x1);
  content.Add(_L("\" y1=\""));
  AppendNumber(content, y1);
  content.Add(_L("\" x2=\""));
  AppendNumber(content, x2);
  content.Add(_L("\" y2=\""));
  AppendNumber(content, y2);
  content.Add(_L("\" stroke=\""));
  AppendColor(content, style.strokecolor);
  content.Add(_L("\" stroke-width=\""));
  AppendNumber(content, style.strokewidth);
  content.Add(_C('"'));

  AppendOpacity(content, _L("stroke-opacity"), style.strokecolor);

  content.Add(_L("/>\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::DrawText(double x, double y, XCHAR* text, GRPSTATISTICSCHARTTEXTSTYLE& style)
* @brief      Draw text
* @ingroup    GRAPHIC
* 
* @param[in]  x : anchor x
* @param[in]  y : anchor y (text baseline)
* @param[in]  text : text to draw
* @param[in]  style : text style
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::DrawText(double x, double y, XCHAR* text, GRPSTATISTICSCHARTTEXTSTYLE& style)
{
  if(!text) return false;

  content.Add(_L(" <text x=\""));
  AppendNumber(content, x);
  content.Add(_L("\" y=\""));
  AppendNumber(content, y);
  content.Add(_L("\" font-size=\""));
  AppendNumber(content, style.size);
  content.Add(_L("\" fill=\""));
  AppendColor(content, style.color);
  content.Add(_L("\" text-anchor=\""));
  content.Add(AnchorToString(style.anchor));
  content.Add(_C('"'));

  if(style.bold)
    {
      content.Add(_L(" font-weight=\"bold\""));
    }

  if(style.rotation != 0.0)
    {
      content.Add(_L(" transform=\"rotate("));
      AppendNumber(content, style.rotation);
      content.Add(_C(' '));
      AppendNumber(content, x);
      content.Add(_C(' '));
      AppendNumber(content, y);
      content.Add(_L(")\""));
    }

  content.Add(_C('>'));

  XSTRING escaped;
  EscapeXML(text, escaped);
  content.Add(escaped);

  content.Add(_L("</text>\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::DrawPolygon(const double* xy, XDWORD npoints, GRPSTATISTICSCHARTSTYLE& style)
* @brief      Draw polygon : a closed filled / stroked shape from an interleaved point array
* @ingroup    GRAPHIC
* 
* @param[in]  xy : interleaved point coordinates [x0, y0, x1, y1, ...] (2 * npoints values)
* @param[in]  npoints : number of points
* @param[in]  style : fill / stroke style
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::DrawPolygon(const double* xy, XDWORD npoints, GRPSTATISTICSCHARTSTYLE& style)
{
  if(!xy || (npoints < 2)) return false;

  content.Add(_L(" <polygon points=\""));

  for(XDWORD p=0; p<npoints; p++)
    {
      if(p) content.Add(_C(' '));

      AppendNumber(content, xy[(p * 2) + 0]);
      content.Add(_C(','));
      AppendNumber(content, xy[(p * 2) + 1]);
    }

  content.Add(_C('"'));

  AppendFillAndStroke(content, style);

  content.Add(_L("/>\n"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBUILDERSVG::GetResult(XSTRING& result)
* @brief      Get result : the serialized SVG XML
* @ingroup    GRAPHIC
* 
* @param[in]  result : SVG XML string
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTBUILDERSVG::GetResult(XSTRING& result)
{
  result.Set(content);

  return !result.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::Clean()
* @brief      Clean internal state
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::Clean()
{
  content.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::AppendNumber(XSTRING& out, double value)
* @brief      Append number : format a number with a locale independent decimal point
* @ingroup    GRAPHIC
* 
* @param[in]  out : output string
* @param[in]  value : number to append
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::AppendNumber(XSTRING& out, double value)
{
  XSTRING number;

  number.Format(_L("%g"), value);
  number.Replace(_L(","), _L("."));

  out.Add(number);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::AppendFillAndStroke(XSTRING& out, GRPSTATISTICSCHARTSTYLE& style)
* @brief      Append fill and stroke attributes from a style
* @ingroup    GRAPHIC
* 
* @param[in]  out : output string
* @param[in]  style : style to serialize
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::AppendFillAndStroke(XSTRING& out, GRPSTATISTICSCHARTSTYLE& style)
{
  if(style.hasfill)
    {
      out.Add(_L(" fill=\""));
      AppendColor(out, style.fillcolor);
      out.Add(_C('"'));

      AppendOpacity(out, _L("fill-opacity"), style.fillcolor);
    }
   else
    {
      out.Add(_L(" fill=\"none\""));
    }

  if(style.hasstroke)
    {
      out.Add(_L(" stroke=\""));
      AppendColor(out, style.strokecolor);
      out.Add(_L("\" stroke-width=\""));
      AppendNumber(out, style.strokewidth);
      out.Add(_C('"'));

      AppendOpacity(out, _L("stroke-opacity"), style.strokecolor);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::AppendColor(XSTRING& out, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Append color as a SVG rgb() function
* @ingroup    GRAPHIC
* 
* @param[in]  out : output string
* @param[in]  color : color to serialize
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::AppendColor(XSTRING& out, GRPSTATISTICSCHARTCOLOR& color)
{
  XSTRING rgb;

  rgb.Format(_L("rgb(%d,%d,%d)"), (int)color.r, (int)color.g, (int)color.b);

  out.Add(rgb);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::AppendOpacity(XSTRING& out, XCHAR* attribute, GRPSTATISTICSCHARTCOLOR& color)
* @brief      Append an opacity attribute only when the color is not fully opaque
* @ingroup    GRAPHIC
* 
* @param[in]  out : output string
* @param[in]  attribute : opacity attribute name (fill-opacity / stroke-opacity)
* @param[in]  color : color carrying the alpha
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::AppendOpacity(XSTRING& out, XCHAR* attribute, GRPSTATISTICSCHARTCOLOR& color)
{
  if(color.a >= 255) return;

  out.Add(_C(' '));
  out.Add(attribute);
  out.Add(_L("=\""));
  AppendNumber(out, (double)color.a / 255.0);
  out.Add(_C('"'));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRPSTATISTICSCHARTBUILDERSVG::AnchorToString(GRPSTATISTICSCHARTTEXTANCHOR anchor)
* @brief      Anchor to string : SVG text-anchor value
* @ingroup    GRAPHIC
* 
* @param[in]  anchor : text anchor
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRPSTATISTICSCHARTBUILDERSVG::AnchorToString(GRPSTATISTICSCHARTTEXTANCHOR anchor)
{
  switch(anchor)
    {
      case GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE  : return _L("middle");
      case GRPSTATISTICSCHARTTEXTANCHOR_END     : return _L("end");
      case GRPSTATISTICSCHARTTEXTANCHOR_START   :
                            default   : return _L("start");
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSTATISTICSCHARTBUILDERSVG::EscapeXML(XCHAR* text, XSTRING& out)
* @brief      Escape XML : replace the XML reserved characters
* @ingroup    GRAPHIC
* 
* @param[in]  text : raw text
* @param[in]  out : escaped text
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSTATISTICSCHARTBUILDERSVG::EscapeXML(XCHAR* text, XSTRING& out)
{
  out.Empty();

  if(!text) return;

  out.Set(text);

  out.Replace(_L("&"), _L("&amp;"));
  out.Replace(_L("<"), _L("&lt;"));
  out.Replace(_L(">"), _L("&gt;"));
  out.Replace(_L("\""), _L("&quot;"));
  out.Replace(_L("'"), _L("&apos;"));
}


