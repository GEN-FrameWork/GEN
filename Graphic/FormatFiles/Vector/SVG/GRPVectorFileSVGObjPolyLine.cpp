/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjPolyLine.cpp
* 
* @class      GRPVECTORFILESVGOBJPOLYLINE
* @brief      Graphic Vector File SVG Object PolyLine class
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

#include "GRPVectorFileSVGObjPolyLine.h"

#include "XFileXML.h"
#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJPOLYLINE::GRPVECTORFILESVGOBJPOLYLINE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJPOLYLINE::GRPVECTORFILESVGOBJPOLYLINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJPOLYLINE::~GRPVECTORFILESVGOBJPOLYLINE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJPOLYLINE::~GRPVECTORFILESVGOBJPOLYLINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPOLYLINE::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and the points list
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPOLYLINE::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  ParsePoints(element);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPOLYLINE::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the poly line (closed for polygon)
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPOLYLINE::BuildPath(GRP2DPATH& path)
{
  XDWORD npoints = GetNPoints();
  if(npoints < 2) return false;

  path.MoveTo(points.Get(0), points.Get(1));

  for(XDWORD c=1; c<npoints; c++)
    {
      path.LineTo(points.Get(c*2), points.Get((c*2)+1));
    }

  if(IsClosed())  path.Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<double>* GRPVECTORFILESVGOBJPOLYLINE::GetPoints()
* @brief      Get points (interleaved x0,y0,x1,y1,...)
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<double>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<double>* GRPVECTORFILESVGOBJPOLYLINE::GetPoints()
{
  return &points;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPVECTORFILESVGOBJPOLYLINE::GetNPoints()
* @brief      Get number of (x,y) points
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPVECTORFILESVGOBJPOLYLINE::GetNPoints()
{
  return (points.GetSize() / 2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPOLYLINE::ParsePoints(XFILEXMLELEMENT* element)
* @brief      Parse points : parse the 'points' attribute ("x1,y1 x2,y2 ..." or "x1 y1 x2 y2 ...")
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPOLYLINE::ParsePoints(XFILEXMLELEMENT* element)
{
  points.DeleteAll();

  XCHAR* valuepoints = element->GetValueAttribute(_L("points"));
  if(!valuepoints) return false;

  XSTRING source(valuepoints);
  source.Replace(_L(","), _L(" "));

  XVECTOR<XSTRING*> tokens;
  source.Split(_C(' '), tokens, false);

  for(XDWORD c=0; c<tokens.GetSize(); c++)
    {
      XSTRING* token = tokens.Get(c);
      if(token && !token->IsEmpty())
        {
          points.Add(token->ConvertToDouble(0, NULL, false));
        }
    }

  tokens.DeleteContents();
  tokens.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPOLYLINE::IsClosed()
* @brief      Is closed : poly line is open (polygon overrides to true)
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPOLYLINE::IsClosed()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJPOLYLINE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJPOLYLINE::Clean()
{
  points.DeleteAll();
}
