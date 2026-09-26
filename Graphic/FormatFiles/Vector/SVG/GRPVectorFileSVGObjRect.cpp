/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjRect.cpp
* 
* @class      GRPVECTORFILESVGOBJRECT
* @brief      Graphic Vector File SVG Object Rect class
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

#include "GRPVectorFileSVGObjRect.h"

#include "XFileXML.h"
#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPVECTORFILESVGOBJRECT_KAPPA   0.5522847498307936



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJRECT::GRPVECTORFILESVGOBJRECT()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJRECT::GRPVECTORFILESVGOBJRECT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJRECT::~GRPVECTORFILESVGOBJRECT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJRECT::~GRPVECTORFILESVGOBJRECT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJRECT::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and the rect geometry (x, y, width, height, rx, ry)
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJRECT::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  x      = element->GetFloatValueAttribute(__L("x"));
  y      = element->GetFloatValueAttribute(__L("y"));
  width  = element->GetFloatValueAttribute(__L("width"));
  height = element->GetFloatValueAttribute(__L("height"));

  XCHAR* valuerx = element->GetValueAttribute(__L("rx"));
  XCHAR* valuery = element->GetValueAttribute(__L("ry"));

  bool hasrx = (valuerx != NULL);
  bool hasry = (valuery != NULL);

  rx = hasrx ? element->GetFloatValueAttribute(__L("rx")) : 0.0;
  ry = hasry ? element->GetFloatValueAttribute(__L("ry")) : 0.0;

  if(hasrx && !hasry)  ry = rx;
  if(hasry && !hasrx)  rx = ry;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJRECT::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the rectangle (with optional rounded corners) into the path
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJRECT::BuildPath(GRP2DPATH& path)
{
  if((width <= 0.0) || (height <= 0.0)) return false;

  double lrx = rx;
  double lry = ry;

  if(lrx > (width  / 2.0))  lrx = width  / 2.0;
  if(lry > (height / 2.0))  lry = height / 2.0;

  if((lrx <= 0.0) || (lry <= 0.0))
    {
      path.MoveTo(x, y);
      path.LineTo(x + width, y);
      path.LineTo(x + width, y + height);
      path.LineTo(x, y + height);
      path.Close();
    }
   else
    {
      double kx = lrx * GRPVECTORFILESVGOBJRECT_KAPPA;
      double ky = lry * GRPVECTORFILESVGOBJRECT_KAPPA;

      path.MoveTo (x + lrx, y);
      path.LineTo (x + width - lrx, y);
      path.CurveTo(x + width - lrx + kx, y, x + width, y + lry - ky, x + width, y + lry);
      path.LineTo (x + width, y + height - lry);
      path.CurveTo(x + width, y + height - lry + ky, x + width - lrx + kx, y + height, x + width - lrx, y + height);
      path.LineTo (x + lrx, y + height);
      path.CurveTo(x + lrx - kx, y + height, x, y + height - lry + ky, x, y + height - lry);
      path.LineTo (x, y + lry);
      path.CurveTo(x, y + lry - ky, x + lrx - kx, y, x + lrx, y);
      path.Close();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetX()
* @brief      Get x
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetX()       { return x;      }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetY()
* @brief      Get y
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetY()       { return y;      }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetWidth()
* @brief      Get width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetWidth()   { return width;  }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetHeight()
* @brief      Get height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetHeight()  { return height; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetRX()
* @brief      Get rx
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetRX()      { return rx;     }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJRECT::GetRY()
* @brief      Get ry
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJRECT::GetRY()      { return ry;     }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJRECT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJRECT::Clean()
{
  x      = 0.0;
  y      = 0.0;
  width  = 0.0;
  height = 0.0;
  rx     = 0.0;
  ry     = 0.0;
}
