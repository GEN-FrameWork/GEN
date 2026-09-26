/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_BoundaryLine.cpp
*
* @class      UI_BOUNDARYLINE
* @brief      User Interface Boundary Line class
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

#include "UI_BoundaryLine.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_BOUNDARYLINE::UI_BOUNDARYLINE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_BOUNDARYLINE::UI_BOUNDARYLINE()
{
  Clean();

  x       = 0;
  y       = 0;
  width   = 0;
  height  = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_BOUNDARYLINE::~UI_BOUNDARYLINE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_BOUNDARYLINE::~UI_BOUNDARYLINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_BOUNDARYLINE::CopyFrom(UI_BOUNDARYLINE& boundaryline)
* @brief      Copy from
* @ingroup    USERINTERFACE
* 
* @param[in]  boundaryline : Boundaryline value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_BOUNDARYLINE::CopyFrom(UI_BOUNDARYLINE& boundaryline)
{
  x       = boundaryline.x;
  y       = boundaryline.y;
  width   = boundaryline.width;
  height  = boundaryline.height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_BOUNDARYLINE::CopyTo(UI_BOUNDARYLINE& boundaryline)
* @brief      Copy to
* @ingroup    USERINTERFACE
* 
* @param[in]  boundaryline : Boundaryline value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_BOUNDARYLINE::CopyTo(UI_BOUNDARYLINE& boundaryline)
{
  boundaryline.x       = x;
  boundaryline.y       = y;
  boundaryline.width   = width;
  boundaryline.height  = height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_BOUNDARYLINE::SetAround(UI_BOUNDARYLINE& boundaryline)
* @brief      Set around
* @note       2b TOUCH POINT: the width/height reconstruction below is written in the current "y = bottom edge,
* @ingroup    USERINTERFACE
* 
* @param[in]  boundaryline : Boundaryline value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_BOUNDARYLINE::SetAround(UI_BOUNDARYLINE& boundaryline)
{
  if(x > boundaryline.x)    x = boundaryline.x;
  if(boundaryline.y > y)    y = boundaryline.y;

  if((boundaryline.x + boundaryline.width)  > (x + width))    width  = boundaryline.width  + (boundaryline.x - x);
  if((boundaryline.y - boundaryline.height) < (y - height))   height = boundaryline.height - (boundaryline.y - y);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_BOUNDARYLINE::IsWithin(XDWORD xpos, XDWORD ypos)
* @brief      Is within. Hit-test routed through the single coordinate conversion point (GetLeft/GetRight/Top/Bottom).
* @ingroup    USERINTERFACE
* 
* @param[in]  xpos : Xpos value.
* @param[in]  ypos : Ypos value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_BOUNDARYLINE::IsWithin(XDWORD xpos, XDWORD ypos)
{
  if( ((xpos >= GetLeft())   && (xpos <  GetRight()))  &&
      ((ypos <= GetBottom()) && (ypos >  GetTop())) )
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_BOUNDARYLINE::ExpandCenteredToMin(double min_width, double min_height)
* @brief      Fase 6: grow this box to at least min_width x min_height, centered (hit-test only).
* @ingroup    USERINTERFACE
* 
* @param[in]  min_width : 
* @param[in]  min_height : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_BOUNDARYLINE::ExpandCenteredToMin(double min_width, double min_height)
{
  if((min_width > 0.0) && (width < min_width))
    {
      double pad = (min_width - width) * 0.5;
      x     -= pad;
      width  = min_width;
    }

  if((min_height > 0.0) && (height < min_height))
    {
      double mid = y - (height * 0.5);
      height = min_height;
      y      = mid + (height * 0.5);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_BOUNDARYLINE::IsEmpty()
* @brief      Is empty
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_BOUNDARYLINE::IsEmpty()
{
  if(!x && !y && !width && !height)
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_BOUNDARYLINE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_BOUNDARYLINE::Clean()
{
  x       = 0;
  y       = 0;
  width   = 0;
  height  = 0;
}
