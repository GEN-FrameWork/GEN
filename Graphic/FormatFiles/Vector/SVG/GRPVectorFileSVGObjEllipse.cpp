/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjEllipse.cpp
* 
* @class      GRPVECTORFILESVGOBJELLIPSE
* @brief      Graphic Vector File SVG Object Ellipse class
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

#include "GRPVectorFileSVGObjEllipse.h"

#include "XFileXML.h"
#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPVECTORFILESVGOBJELLIPSE_KAPPA   0.5522847498307936



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJELLIPSE::GRPVECTORFILESVGOBJELLIPSE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJELLIPSE::GRPVECTORFILESVGOBJELLIPSE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJELLIPSE::~GRPVECTORFILESVGOBJELLIPSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJELLIPSE::~GRPVECTORFILESVGOBJELLIPSE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJELLIPSE::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and the ellipse geometry (cx, cy, rx, ry)
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJELLIPSE::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  cx = element->GetFloatValueAttribute(_L("cx"));
  cy = element->GetFloatValueAttribute(_L("cy"));
  rx = element->GetFloatValueAttribute(_L("rx"));
  ry = element->GetFloatValueAttribute(_L("ry"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJELLIPSE::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the ellipse as 4 cubic bezier curves
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJELLIPSE::BuildPath(GRP2DPATH& path)
{
  if((rx <= 0.0) || (ry <= 0.0)) return false;

  double ox = rx * GRPVECTORFILESVGOBJELLIPSE_KAPPA;
  double oy = ry * GRPVECTORFILESVGOBJELLIPSE_KAPPA;

  path.MoveTo (cx + rx, cy);
  path.CurveTo(cx + rx, cy + oy, cx + ox, cy + ry, cx, cy + ry);
  path.CurveTo(cx - ox, cy + ry, cx - rx, cy + oy, cx - rx, cy);
  path.CurveTo(cx - rx, cy - oy, cx - ox, cy - ry, cx, cy - ry);
  path.CurveTo(cx + ox, cy - ry, cx + rx, cy - oy, cx + rx, cy);
  path.Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJELLIPSE::GetCX()
* @brief      Get cx
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJELLIPSE::GetCX()   { return cx; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJELLIPSE::GetCY()
* @brief      Get cy
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJELLIPSE::GetCY()   { return cy; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJELLIPSE::GetRX()
* @brief      Get rx
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJELLIPSE::GetRX()   { return rx; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJELLIPSE::GetRY()
* @brief      Get ry
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJELLIPSE::GetRY()   { return ry; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJELLIPSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJELLIPSE::Clean()
{
  cx = 0.0;
  cy = 0.0;
  rx = 0.0;
  ry = 0.0;
}
