/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DPath.cpp
* 
* @class      GRP2DPATH
* @brief      Graphics 2D Path class
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

#include "GRP2DPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATHSEGMENT::GRP2DPATHSEGMENT()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATHSEGMENT::GRP2DPATHSEGMENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATHSEGMENT::~GRP2DPATHSEGMENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATHSEGMENT::~GRP2DPATHSEGMENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DPATHSEGMENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DPATHSEGMENT::Clean()
{
  type      = GRP2DPATHSEGMENTTYPE_UNKNOWN;

  x         = 0.0f;
  y         = 0.0f;

  c1x       = 0.0f;
  c1y       = 0.0f;
  c2x       = 0.0f;
  c2y       = 0.0f;

  rx        = 0.0f;
  ry        = 0.0f;
  xrot      = 0.0f;
  largearc  = false;
  sweep     = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATH::GRP2DPATH()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATH::GRP2DPATH()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATH::~GRP2DPATH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATH::~GRP2DPATH()
{
  DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::MoveTo(double x, double y)
* @brief      Move to
* @ingroup    GRAPHIC
* 
* @param[in]  x : end point x
* @param[in]  y : end point y
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::MoveTo(double x, double y)
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type = GRP2DPATHSEGMENTTYPE_MOVETO;
  segment->x    = x;
  segment->y    = y;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::LineTo(double x, double y)
* @brief      Line to
* @ingroup    GRAPHIC
* 
* @param[in]  x : end point x
* @param[in]  y : end point y
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::LineTo(double x, double y)
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type = GRP2DPATHSEGMENTTYPE_LINETO;
  segment->x    = x;
  segment->y    = y;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::CurveTo(double c1x, double c1y, double c2x, double c2y, double x, double y)
* @brief      Curve to (cubic bezier)
* @ingroup    GRAPHIC
* 
* @param[in]  c1x : control point 1 x
* @param[in]  c1y : control point 1 y
* @param[in]  c2x : control point 2 x
* @param[in]  c2y : control point 2 y
* @param[in]  x : end point x
* @param[in]  y : end point y
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::CurveTo(double c1x, double c1y, double c2x, double c2y, double x, double y)
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type = GRP2DPATHSEGMENTTYPE_CURVETO;
  segment->c1x  = c1x;
  segment->c1y  = c1y;
  segment->c2x  = c2x;
  segment->c2y  = c2y;
  segment->x    = x;
  segment->y    = y;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::QuadTo(double c1x, double c1y, double x, double y)
* @brief      Quad to (quadratic bezier)
* @ingroup    GRAPHIC
* 
* @param[in]  c1x : control point x
* @param[in]  c1y : control point y
* @param[in]  x : end point x
* @param[in]  y : end point y
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::QuadTo(double c1x, double c1y, double x, double y)
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type = GRP2DPATHSEGMENTTYPE_QUADTO;
  segment->c1x  = c1x;
  segment->c1y  = c1y;
  segment->x    = x;
  segment->y    = y;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::ArcTo(double rx, double ry, double xrot, bool largearc, bool sweep, double x, double y)
* @brief      Arc to (elliptical arc)
* @ingroup    GRAPHIC
* 
* @param[in]  rx : arc radius x
* @param[in]  ry : arc radius y
* @param[in]  xrot : arc x axis rotation (degrees)
* @param[in]  largearc : arc large flag
* @param[in]  sweep : arc sweep flag
* @param[in]  x : end point x
* @param[in]  y : end point y
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::ArcTo(double rx, double ry, double xrot, bool largearc, bool sweep, double x, double y)
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type     = GRP2DPATHSEGMENTTYPE_ARCTO;
  segment->rx       = rx;
  segment->ry       = ry;
  segment->xrot     = xrot;
  segment->largearc = largearc;
  segment->sweep    = sweep;
  segment->x        = x;
  segment->y        = y;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::Close()
* @brief      Close current sub path
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::Close()
{
  GRP2DPATHSEGMENT* segment = GEN_NEW GRP2DPATHSEGMENT();
  if(!segment) return false;

  segment->type = GRP2DPATHSEGMENTTYPE_CLOSE;

  return AddSegment(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRP2DPATH::GetSize()
* @brief      Get size
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRP2DPATH::GetSize()
{
  return segments.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATHSEGMENT* GRP2DPATH::Get(int index)
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @param[in]  index : index of segment
* 
* @return     GRP2DPATHSEGMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATHSEGMENT* GRP2DPATH::Get(int index)
{
  return segments.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::IsEmpty()
* @brief      Is empty
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::IsEmpty()
{
  return segments.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::DeleteAll()
* @brief      Delete all
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::DeleteAll()
{
  if(segments.IsEmpty()) return true;

  segments.DeleteContents();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATHFILLRULE GRP2DPATH::GetFillRule()
* @brief      Get fill rule
* @ingroup    GRAPHIC
* 
* @return     GRP2DPATHFILLRULE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATHFILLRULE GRP2DPATH::GetFillRule()
{
  return fillrule;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DPATH::SetFillRule(GRP2DPATHFILLRULE fillrule)
* @brief      Set fill rule
* @ingroup    GRAPHIC
* 
* @param[in]  fillrule : fill rule
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DPATH::SetFillRule(GRP2DPATHFILLRULE fillrule)
{
  this->fillrule = fillrule;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DPATH::AddSegment(GRP2DPATHSEGMENT* segment)
* @brief      Add segment
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  segment : segment class
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DPATH::AddSegment(GRP2DPATHSEGMENT* segment)
{
  if(!segment) return false;

  return segments.Add(segment);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DPATH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DPATH::Clean()
{
  fillrule = GRP2DPATHFILLRULE_NONZERO;
}




