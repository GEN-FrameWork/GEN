/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPViewPort.cpp
* 
* @class      GRPVIEWPORT
* @brief      Graphics Viewport class
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

#include "GRPViewPort.h"

#include "GRPXEvent.h"
#include "GRPFactory.h"
#include "GRPScreen.h"
#include "GRP2DCanvasAGG.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIEWPORT::GRPVIEWPORT()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT::GRPVIEWPORT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIEWPORT::~GRPVIEWPORT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT::~GRPVIEWPORT()
{
  if(canvas)
    {
      GRPFACTORY::GetInstance().DeleteCanvas(canvas);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVIEWPORT::GetID()
* @brief      Get ID
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVIEWPORT::GetID()
{
  return &ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIEWPORT::IsActive()
* @brief      Is active
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIEWPORT::IsActive()
{
  return active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIEWPORT::SetIsActive(bool active)
* @brief      Set is active
* @ingroup    GRAPHIC
* 
* @param[in]  active : true to active
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetIsActive(bool active)
{
  this->active = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIEWPORT_PROJECTIONTYPE GRPVIEWPORT::GetProjectionType()
* @brief      Get projection type
* @ingroup    GRAPHIC
* 
* @return     GRPVIEWPORT_PROJECTIONTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT_PROJECTIONTYPE GRPVIEWPORT::GetProjectionType()
{
  return projectiontype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIEWPORT::SetProjectionType(GRPVIEWPORT_PROJECTIONTYPE projectiontype)
* @brief      Set projection type
* @ingroup    GRAPHIC
* 
* @param[in]  projectiontype : GEN_NEW projection type
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetProjectionType(GRPVIEWPORT_PROJECTIONTYPE projectiontype)
{
  this->projectiontype = projectiontype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIEWPORT::CreateCanvas(GRPPROPERTIES& canvasproperties)
* @brief      Create canvas
* @ingroup    GRAPHIC
* 
* @param[in]  canvasproperties : Canvasproperties value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIEWPORT::CreateCanvas(GRPPROPERTIES& canvasproperties)
{
  canvas = GRPFACTORY::GetInstance().CreateCanvas(&canvasproperties);
  if(!canvas)
    {
      return false;
    }
  
  canvas->SetWidth(canvasproperties.GetWidth());
  canvas->SetHeight(canvasproperties.GetHeight());

  canvas->Buffer_Create();
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPVIEWPORT::GetPositionX()
* @brief      Get position x
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetPositionX()
{
  return x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPVIEWPORT::GetPositionY()
* @brief      Get position y
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float  GRPVIEWPORT::GetPositionY()
{
  return y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIEWPORT::SetPosition(float x, float y)
* @brief      Set position
* @ingroup    GRAPHIC
* 
* @param[in]  x : Set X position
* @param[in]  y : Set Y position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetPosition(float x, float y)
{
  this->x = x;
  this->y = y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPVIEWPORT::GetWidth()
* @brief      Get width
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetWidth()
{
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPVIEWPORT::GetHeight()
* @brief      Get height
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetHeight()
{
  return height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIEWPORT::SetSize(float width, float height)
* @brief      Set size
* @ingroup    GRAPHIC
* 
* @param[in]  width : Set Width of View Port
* @param[in]  height : Set Height of View Port
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetSize(float width, float height)
{
  this->width  = width;
  this->height = height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPVIEWPORT::GetMinWidth()
* @brief      Get minimum width (0 = not set / no limit)
* @ingroup    GRAPHIC
*
* @return     float : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetMinWidth()
{
  return minwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPVIEWPORT::GetMinHeight()
* @brief      Get minimum height (0 = not set / no limit)
* @ingroup    GRAPHIC
*
* @return     float : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetMinHeight()
{
  return minheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPVIEWPORT::SetMinSize(float minwidth, float minheight)
* @brief      Set minimum size. Platforms that honour it (Windows WM_GETMINMAXINFO / X11 PMinSize /
*             custom-chromes resize grip) refuse to shrink the native window below this client size.
*             Below-min content hiding remains a secondary safety net. Pass 0 to leave an axis unrestricted.
* @ingroup    GRAPHIC
*
* @param[in]  minwidth : Minimum width value.
* @param[in]  minheight : Minimum height value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetMinSize(float minwidth, float minheight)
{
  this->minwidth  = minwidth;
  this->minheight = minheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPVIEWPORT::GetMaxWidth()
* @brief      Get maximum width (0 = not set / no limit)
* @ingroup    GRAPHIC
*
* @return     float : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetMaxWidth()
{
  return maxwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPVIEWPORT::GetMaxHeight()
* @brief      Get maximum height (0 = not set / no limit)
* @ingroup    GRAPHIC
*
* @return     float : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetMaxHeight()
{
  return maxheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPVIEWPORT::SetMaxSize(float maxwidth, float maxheight)
* @brief      Set maximum size. Beyond this size (either axis, independently) the native window
*             hosting this viewport is expected to stop growing (native resize border and
*             Maximize are both capped at it) instead of stretching its rendered content to fill
*             a bigger window. Pass 0 to leave an axis unrestricted.
* @ingroup    GRAPHIC
*
* @param[in]  maxwidth : Maximum width value.
* @param[in]  maxheight : Maximum height value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::SetMaxSize(float maxwidth, float maxheight)
{
  this->maxwidth  = maxwidth;
  this->maxheight = maxheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPVIEWPORT::GetCanvasPositionX()
* @brief      Get canvas position x
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetCanvasPositionX()
{
  return canvas_x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPVIEWPORT::GetCanvasPositionY()
* @brief      Get canvas position y
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPVIEWPORT::GetCanvasPositionY()
{
  return canvas_y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIEWPORT::SetCanvasPosition(float x, float y)
* @brief      Set canvas position
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIEWPORT::SetCanvasPosition(float x, float y)
{
  bool status = true;

  canvas_x = x;
  canvas_y = y;

  if(canvas_x < 0)  
    {
      canvas_x  = 0;
      status    = false;
    }

  if(canvas_y < 0)
    {
      canvas_y  = 0;
      status    = false;
    }

  if(canvas)
    {
      if((canvas_x + width) > canvas->GetWidth())
        {
          canvas_x  = canvas->GetWidth() - width;
          status    = false;
        }

      if((canvas_y + height) > canvas->GetHeight())
        {
          canvas_y  = canvas->GetHeight() - height;
          status    = false;
        }
    }
   else
    {
      status = false;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS* GRPVIEWPORT::GetCanvas()
* @brief      Get canvas
* @ingroup    GRAPHIC
* 
* @return     GRP2DCANVAS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS*  GRPVIEWPORT::GetCanvas()
{
  return canvas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIEWPORT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIEWPORT::Clean()
{
  active          = true;

  projectiontype  = GRPVIEWPORT_PROJECTIONTYPE_UNKNOWN;

  x               = 0.0f;
  y               = 0.0f;

  width           = 0.0f;
  height          = 0.0f;

  minwidth        = 0.0f;
  minheight       = 0.0f;
  maxwidth        = 0.0f;
  maxheight       = 0.0f;

  canvas_x        = 0.0f;
  canvas_y        = 0.0f;
  canvas          = NULL;
}



