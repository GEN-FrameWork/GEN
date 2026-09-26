/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjCircle.cpp
* 
* @class      GRPVECTORFILESVGOBJCIRCLE
* @brief      Graphic Vector File SVG Object Circle class
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

#include "GRPVectorFileSVGObjCircle.h"

#include "XFileXML.h"
#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPVECTORFILESVGOBJCIRCLE_KAPPA   0.5522847498307936



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJCIRCLE::GRPVECTORFILESVGOBJCIRCLE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJCIRCLE::GRPVECTORFILESVGOBJCIRCLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJCIRCLE::~GRPVECTORFILESVGOBJCIRCLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJCIRCLE::~GRPVECTORFILESVGOBJCIRCLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJCIRCLE::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and the circle geometry (cx, cy, r)
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJCIRCLE::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  cx = element->GetFloatValueAttribute(__L("cx"));
  cy = element->GetFloatValueAttribute(__L("cy"));
  r  = element->GetFloatValueAttribute(__L("r"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJCIRCLE::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the circle as 4 cubic bezier curves
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJCIRCLE::BuildPath(GRP2DPATH& path)
{
  if(r <= 0.0) return false;

  double o = r * GRPVECTORFILESVGOBJCIRCLE_KAPPA;

  path.MoveTo (cx + r, cy);
  path.CurveTo(cx + r, cy + o, cx + o, cy + r, cx, cy + r);
  path.CurveTo(cx - o, cy + r, cx - r, cy + o, cx - r, cy);
  path.CurveTo(cx - r, cy - o, cx - o, cy - r, cx, cy - r);
  path.CurveTo(cx + o, cy - r, cx + r, cy - o, cx + r, cy);
  path.Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJCIRCLE::GetCX()
* @brief      Get cx
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJCIRCLE::GetCX()   { return cx; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJCIRCLE::GetCY()
* @brief      Get cy
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJCIRCLE::GetCY()   { return cy; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJCIRCLE::GetR()
* @brief      Get r
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJCIRCLE::GetR()    { return r;  }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJCIRCLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJCIRCLE::Clean()
{
  cx = 0.0;
  cy = 0.0;
  r  = 0.0;
}
