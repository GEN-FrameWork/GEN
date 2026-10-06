/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGConfig.cpp
* 
* @class      GRPVECTORFILESVGCONFIG
* @brief      Graphic Vector File SVG Config class
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

#include "GRPVectorFileSVGConfig.h"

#include "XFileXML.h"
#include "XVector.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGCONFIG::GRPVECTORFILESVGCONFIG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGCONFIG::GRPVECTORFILESVGCONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGCONFIG::~GRPVECTORFILESVGCONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGCONFIG::~GRPVECTORFILESVGCONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCONFIG::ApplyData(XFILEXMLELEMENT* elementsvg)
* @brief      Apply data : read the root &lt;svg&gt; attributes (width, height, viewBox, preserveAspectRatio)
* @ingroup    GRAPHIC
* 
* @param[in]  elementsvg : root svg element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCONFIG::ApplyData(XFILEXMLELEMENT* elementsvg)
{
  if(!elementsvg) return false;

  XCHAR* valuewidth  = elementsvg->GetValueAttribute(_L("width"));
  XCHAR* valueheight = elementsvg->GetValueAttribute(_L("height"));

  if(valuewidth)
    {
      XSTRING str(valuewidth);
      width = str.ConvertToDouble(0, NULL, false);
    }

  if(valueheight)
    {
      XSTRING str(valueheight);
      height = str.ConvertToDouble(0, NULL, false);
    }

  XCHAR* valueviewbox = elementsvg->GetValueAttribute(_L("viewBox"));
  if(valueviewbox)
    {
      XSTRING strviewbox(valueviewbox);
      strviewbox.Replace(_L(","), _L(" "));

      XVECTOR<XSTRING*> tokens;
      strviewbox.Split(_C(' '), tokens, false);

      if(tokens.GetSize() >= 4)
        {
          viewboxminx   = tokens.Get(0)->ConvertToDouble(0, NULL, false);
          viewboxminy   = tokens.Get(1)->ConvertToDouble(0, NULL, false);
          viewboxwidth  = tokens.Get(2)->ConvertToDouble(0, NULL, false);
          viewboxheight = tokens.Get(3)->ConvertToDouble(0, NULL, false);

          hasviewbox    = true;
        }

      tokens.DeleteContents();
      tokens.DeleteAll();
    }

  XCHAR* valuepreserve = elementsvg->GetValueAttribute(_L("preserveAspectRatio"));
  if(valuepreserve)
    {
      preserveaspectratio = valuepreserve;
    }

  // If width/height are missing but viewBox exists, fall back to the viewBox size.
  if((width <= 0.0) && hasviewbox)   width  = viewboxwidth;
  if((height <= 0.0) && hasviewbox)  height = viewboxheight;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetWidth()
* @brief      Get width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetWidth()
{
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCONFIG::SetWidth(double width)
* @brief      Set width
* @ingroup    GRAPHIC
* 
* @param[in]  width : width
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCONFIG::SetWidth(double width)
{
  this->width = width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetHeight()
* @brief      Get height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetHeight()
{
  return height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCONFIG::SetHeight(double height)
* @brief      Set height
* @ingroup    GRAPHIC
* 
* @param[in]  height : height
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCONFIG::SetHeight(double height)
{
  this->height = height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCONFIG::HasViewBox()
* @brief      Has view box
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCONFIG::HasViewBox()
{
  return hasviewbox;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCONFIG::SetHasViewBox(bool hasviewbox)
* @brief      Set has view box
* @ingroup    GRAPHIC
* 
* @param[in]  hasviewbox : has view box
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCONFIG::SetHasViewBox(bool hasviewbox)
{
  this->hasviewbox = hasviewbox;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetViewBoxMinX()
* @brief      Get view box min x
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetViewBoxMinX()
{
  return viewboxminx;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetViewBoxMinY()
* @brief      Get view box min y
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetViewBoxMinY()
{
  return viewboxminy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetViewBoxWidth()
* @brief      Get view box width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetViewBoxWidth()
{
  return viewboxwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGCONFIG::GetViewBoxHeight()
* @brief      Get view box height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGCONFIG::GetViewBoxHeight()
{
  return viewboxheight;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCONFIG::SetViewBox(double minx, double miny, double width, double height)
* @brief      Set view box
* @ingroup    GRAPHIC
* 
* @param[in]  minx : view box min x
* @param[in]  miny : view box min y
* @param[in]  width : view box width
* @param[in]  height : view box height
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCONFIG::SetViewBox(double minx, double miny, double width, double height)
{
  viewboxminx   = minx;
  viewboxminy   = miny;
  viewboxwidth  = width;
  viewboxheight = height;
  hasviewbox    = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGCONFIG::GetPreserveAspectRatio()
* @brief      Get preserve aspect ratio
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGCONFIG::GetPreserveAspectRatio()
{
  return &preserveaspectratio;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCONFIG::Clean()
{
  width         = 0.0;
  height        = 0.0;

  hasviewbox    = false;
  viewboxminx   = 0.0;
  viewboxminy   = 0.0;
  viewboxwidth  = 0.0;
  viewboxheight = 0.0;
}




