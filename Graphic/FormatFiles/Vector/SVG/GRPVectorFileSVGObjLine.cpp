/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjLine.cpp
* 
* @class      GRPVECTORFILESVGOBJLINE
* @brief      Graphic Vector File SVG Object Line class
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

#include "GRPVectorFileSVGObjLine.h"

#include "XFileXML.h"
#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJLINE::GRPVECTORFILESVGOBJLINE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJLINE::GRPVECTORFILESVGOBJLINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJLINE::~GRPVECTORFILESVGOBJLINE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJLINE::~GRPVECTORFILESVGOBJLINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJLINE::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and the line geometry (x1, y1, x2, y2)
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJLINE::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  x1 = element->GetFloatValueAttribute(_L("x1"));
  y1 = element->GetFloatValueAttribute(_L("y1"));
  x2 = element->GetFloatValueAttribute(_L("x2"));
  y2 = element->GetFloatValueAttribute(_L("y2"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJLINE::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the line (open sub path, only stroked)
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJLINE::BuildPath(GRP2DPATH& path)
{
  path.MoveTo(x1, y1);
  path.LineTo(x2, y2);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJLINE::GetX1()
* @brief      Get x1
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJLINE::GetX1()   { return x1; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJLINE::GetY1()
* @brief      Get y1
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJLINE::GetY1()   { return y1; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJLINE::GetX2()
* @brief      Get x2
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJLINE::GetX2()   { return x2; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJLINE::GetY2()
* @brief      Get y2
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJLINE::GetY2()   { return y2; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJLINE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJLINE::Clean()
{
  x1 = 0.0;
  y1 = 0.0;
  x2 = 0.0;
  y2 = 0.0;
}
