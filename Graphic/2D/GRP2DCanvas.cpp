/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DCanvas.cpp
* 
* @class      GRP2DCANVAS
* @brief      Graphics Canvas class
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

#include "GRP2DCanvas.h"

#include "GRP2DRebuildAreas.h"
#include "GRPViewPort.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS_VECTORFONT_CONFIG::GRP2DCANVAS_VECTORFONT_CONFIG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS_VECTORFONT_CONFIG::GRP2DCANVAS_VECTORFONT_CONFIG()
{
  Clean();    
  color = GEN_NEW GRP2DCOLOR_RGBA8(0, 0, 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS_VECTORFONT_CONFIG::~GRP2DCANVAS_VECTORFONT_CONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS_VECTORFONT_CONFIG::~GRP2DCANVAS_VECTORFONT_CONFIG()
{
  if(color) 
    {
      GEN_DELETE color;   
      color = NULL;
    }      

  Clean();                                                                                                                                                                                              
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS_VECTORFONT_CONFIG::GetSize(double& width, double& height)
* @brief      Get size
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS_VECTORFONT_CONFIG::GetSize(double& width, double& height)
{
  width   = this->width;
  height  = this->height;    

  return true;
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS_VECTORFONT_CONFIG::SetSize(double width, double height)
* @brief      Set size
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS_VECTORFONT_CONFIG::SetSize(double width, double height)
{
  this->width   = width;
  this->height  = height; 

  return true;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS_VECTORFONT_CONFIG::GetSize()
* @brief      Get size
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS_VECTORFONT_CONFIG::GetSize()
{
  if(height > width) return height;
    else if(height < width) return width;
      else if(height == width) return width;
  
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS_VECTORFONT_CONFIG::SetSize(double size)
* @brief      Set size
* @ingroup    GRAPHIC
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS_VECTORFONT_CONFIG::SetSize(double size)
{
  this->width  = size;
  this->height = size;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS_VECTORFONT_CONFIG::GetWidth()
* @brief      Get width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS_VECTORFONT_CONFIG::GetWidth()                                 
{ 
  return width;               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS_VECTORFONT_CONFIG::SetWidth(double width)
* @brief      Set width
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS_VECTORFONT_CONFIG::SetWidth(double width)                     
{ 
  this->width = width;        
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS_VECTORFONT_CONFIG::GetHeight()
* @brief      Get height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS_VECTORFONT_CONFIG::GetHeight()                                 
{ 
  return height;              
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS_VECTORFONT_CONFIG::SetHeight(double height)
* @brief      Set height
* @ingroup    GRAPHIC
* 
* @param[in]  height : Height value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS_VECTORFONT_CONFIG::SetHeight(double height)                    
{ 
  this->height = height;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR_RGBA8* GRP2DCANVAS_VECTORFONT_CONFIG::GetColor()
* @brief      Get color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR_RGBA8* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR_RGBA8* GRP2DCANVAS_VECTORFONT_CONFIG::GetColor()                                 
{
  return color;               
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS_VECTORFONT_CONFIG::SetColor(GRP2DCOLOR_RGBA8* color)
* @brief      Set color
* @ingroup    GRAPHIC
* 
* @param[in]  color : Color value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS_VECTORFONT_CONFIG::SetColor(GRP2DCOLOR_RGBA8* color)          
{ 
  if(!color) return;

  this->color->r = color->r;
  this->color->g = color->g;
  this->color->b = color->b;
  this->color->a = color->a;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS_VECTORFONT_CONFIG::IsKerning()
* @brief      Is kerning
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS_VECTORFONT_CONFIG::IsKerning()
{
  return iskerning;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS_VECTORFONT_CONFIG::SetKerning(bool iskerning)
* @brief      Set kerning
* @ingroup    GRAPHIC
* 
* @param[in]  iskerning : Iskerning value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS_VECTORFONT_CONFIG::SetKerning(bool iskerning)
{
  this->iskerning = iskerning;
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS_VECTORFONT_CONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS_VECTORFONT_CONFIG::Clean()
{
  width       = 0.0f;
  height      = 0.0f;
  color       = NULL;
  iskerning   = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS::GRP2DCANVAS()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS::GRP2DCANVAS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS::~GRP2DCANVAS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS::~GRP2DCANVAS()
{
  RebuildAllAreas();
  DeleteAllRebuildAreas();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::Buffer_Create()
* @brief      Buffer create
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::Buffer_Create()
{
  buffersize = (width * height * GetBytesperPixel());
  buffer     = GEN_NEW XBYTE[buffersize];
  if(!buffer)
    {
      return false;
    }

  // A freshly allocated buffer holds whatever the heap left in it. Nothing in the engine guarantees that
  // every single pixel is written before the first Update(): repainting is driven by rebuild areas, so any
  // region that no layout actually covers keeps showing the raw memory as garbage. The strip a custom
  // window chrome paints over with a semi-transparent caption is exactly such a region, which is why the
  // garbage used to be visible along the top of the window on the first frames. It is worse than a cosmetic
  // one-off: the FIRST rebuild area created over an uncovered region captures that garbage as its saved
  // "background" and restores it on every later repaint. Start from a known, fully transparent surface.
  Buffer_SetToZero();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRP2DCANVAS::Buffer_Get()
* @brief      Buffer get
* @ingroup    GRAPHIC
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRP2DCANVAS::Buffer_Get()
{
  return buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRP2DCANVAS::Buffer_GetSize()
* @brief      Buffer get size
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRP2DCANVAS::Buffer_GetSize()
{
  return buffersize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::Buffer_SetToZero()
* @brief      Buffer set to zero
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::Buffer_SetToZero()
{
  if(!Buffer_Get()) 
    {
      return false;
    }

  memset(Buffer_Get(), 0, buffersize);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::Buffer_Delete()
* @brief      Buffer delete
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::Buffer_Delete()
{
  if(!buffer) 
    {
      return false;
    }

  GEN_DELETE_ARRAY buffer;
  buffer = NULL;

  buffersize = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Clear(const GRP2DCOLOR* color)
* @brief      Clear
* @ingroup    GRAPHIC
* 
* @param[in]  color : Color value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Clear(const GRP2DCOLOR* color)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::GetClipBox(double& x1, double& y1, double& x2, double& y2)
* @brief      Get clip box
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::GetClipBox(double& x1, double& y1, double& x2, double& y2)
{
  x1 = lastcliprect.x1;
  y1 = lastcliprect.y1;
  x2 = lastcliprect.x2;
  y2 = lastcliprect.y2;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::GetClipBox(XRECT& rect)
* @brief      Get clip box
* @ingroup    GRAPHIC
* 
* @param[in]  rect : Rectangle to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::GetClipBox(XRECT& rect)
{
  rect.CopyFrom(lastcliprect);   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRECT& GRP2DCANVAS::GetClipBox()
* @brief      Get clip box
* @ingroup    GRAPHIC
* 
* @return     XRECT& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRECT& GRP2DCANVAS::GetClipBox()
{
  return lastcliprect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetClipBox(double x1, double y1, double x2, double y2)
* @brief      Set clip box
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void  GRP2DCANVAS::SetClipBox(double x1, double y1, double x2, double y2)
{
  lastcliprect.x1 = x1;
  lastcliprect.y1 = y1;
  lastcliprect.x2 = x2;
  lastcliprect.y2 = y2;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetClipBox(XRECT& rect)
* @brief      Set clip box
* @ingroup    GRAPHIC
* 
* @param[in]  rect : Rectangle to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::SetClipBox(XRECT& rect)
{
  lastcliprect.CopyFrom(rect);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR* GRP2DCANVAS::GetPixel(double x, double y)
* @brief      Get pixel
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos of pixel
* @param[in]  y : y pos of pixel
* 
* @return     GRP2DCOLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR* GRP2DCANVAS::GetPixel(double x, double y)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutPixel(double x, double y, const GRP2DCOLOR* color)
* @brief      Put pixel
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos to put pixel
* @param[in]  y : y pos to put pixel
* @param[in]  color : color to put
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutPixel(double x, double y, const GRP2DCOLOR* color)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutBlendPixel(double x, double y, const GRP2DCOLOR* color, double alpha)
* @brief      Put blend pixel
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos to put pixel
* @param[in]  y : y pos to put pixel
* @param[in]  color : color to put
* @param[in]  alpha : alpha to put pixel
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutBlendPixel(double x, double y, const GRP2DCOLOR* color, double alpha)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR* GRP2DCANVAS::GetFillColor()
* @brief      Get fill color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR* GRP2DCANVAS::GetFillColor()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetFillColor(const GRP2DCOLOR* color)
* @brief      Set fill color
* @ingroup    GRAPHIC
* 
* @param[in]  color : color to fill
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::SetFillColor(const GRP2DCOLOR* color)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR* GRP2DCANVAS::GetLineColor()
* @brief      Get line color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR* GRP2DCANVAS::GetLineColor()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetLineColor(const GRP2DCOLOR* color)
* @brief      Set line color
* @ingroup    GRAPHIC
* 
* @param[in]  color : color to lines
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::SetLineColor(const GRP2DCOLOR* color)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::GetLineWidth()
* @brief      Get line width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::GetLineWidth()
{
  return linewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetLineWidth(double linewidth)
* @brief      Set line width
* @ingroup    GRAPHIC
* 
* @param[in]  linewidth : GEN_NEW line width
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::SetLineWidth(double linewidth)
{
  this->linewidth = linewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::GetDashLength()
* @brief      Get dash length
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::GetDashLength()
{
  return dashlength;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::SetDashLenght(double dashlength)
* @brief      Set dash lenght
* @ingroup    GRAPHIC
* 
* @param[in]  dashlength : GEN_NEW dash lenght
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::SetDashLenght(double dashlength)
{
  this->dashlength = dashlength;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Line(double x1, double y1, double x2, double y2)
* @brief      Line
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Line(double x1, double y1, double x2, double y2)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Rectangle(double x1, double y1, double x2, double y2, bool isfill)
* @brief      Rectangle
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  isfill : Isfill value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Rectangle(double x1, double y1, double x2, double y2, bool isfill)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Circle(double x, double y, double radius, bool isfill)
* @brief      Circle
* @ingroup    GRAPHIC
* 
* @param[in]  x : x center position
* @param[in]  y : y center position
* @param[in]  radius : radius of circle
* @param[in]  isfill : is fill
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Circle(double x, double y, double radius, bool isfill)
{
  Ellipse(x, y, radius, radius, isfill);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Ellipse(double x, double y, double rx, double ry, bool isfill)
* @brief      Ellipse
* @ingroup    GRAPHIC
* 
* @param[in]  x : x center position
* @param[in]  y : y center position
* @param[in]  rx : x radius of ellipse
* @param[in]  ry : y radius of ellipse
* @param[in]  isfill : is fill
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Ellipse(double x, double y, double rx, double ry, bool isfill)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Polygon(GRP2DVERTEXS& vertexs, bool isfill)
* @brief      Polygon
* @ingroup    GRAPHIC
* 
* @param[in]  vertexs : vector of vertex
* @param[in]  isfill : is fill
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Polygon(GRP2DVERTEXS& vertexs, bool isfill)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Path(GRP2DPATH& path, bool isfill)
* @brief      Path
* @ingroup    GRAPHIC
* 
* @param[in]  path : path with the segments
* @param[in]  isfill : is fill
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Path(GRP2DPATH& path, bool isfill)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PathGradientLinear(GRP2DPATH& path, bool evenodd, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
* @brief      Path gradient linear (base : nothing; implemented by the AGG canvas)
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* @param[in]  evenodd : Evenodd value.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  stops : Stops pointer to use.
* @param[in]  nstops : Nstops value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PathGradientLinear(GRP2DPATH& path, bool evenodd, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PathGradientRadial(GRP2DPATH& path, bool evenodd, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
* @brief      Path gradient radial (base : nothing; implemented by the AGG canvas)
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* @param[in]  evenodd : Evenodd value.
* @param[in]  cx : Cx value.
* @param[in]  cy : Cy value.
* @param[in]  r : R value.
* @param[in]  stops : Stops pointer to use.
* @param[in]  nstops : Nstops value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PathGradientRadial(GRP2DPATH& path, bool evenodd, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PathGradientLinearStroke(GRP2DPATH& path, double linewidth, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
* @brief      Path gradient linear stroke (base : nothing; implemented by the AGG canvas)
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* @param[in]  linewidth : Linewidth value.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  stops : Stops pointer to use.
* @param[in]  nstops : Nstops value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PathGradientLinearStroke(GRP2DPATH& path, double linewidth, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PathGradientRadialStroke(GRP2DPATH& path, double linewidth, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
* @brief      Path gradient radial stroke (base : nothing; implemented by the AGG canvas)
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* @param[in]  linewidth : Linewidth value.
* @param[in]  cx : Cx value.
* @param[in]  cy : Cy value.
* @param[in]  r : R value.
* @param[in]  stops : Stops pointer to use.
* @param[in]  nstops : Nstops value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PathGradientRadialStroke(GRP2DPATH& path, double linewidth, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::RoundRect(double x1, double y1, double x2, double y2, double radius, bool isfill)
* @brief      Round rect
* @ingroup    GRAPHIC
* 
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  radius : Radius value.
* @param[in]  isfill : Isfill value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::RoundRect(double x1, double y1, double x2, double y2, double radius, bool isfill)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRP2DCANVAS::GetBitmap(double x, double y, double width, double height)
* @brief      Get bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRP2DCANVAS::GetBitmap(double x, double y, double width, double height)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutBitmapNoAlpha(double x, double y, GRPBITMAP* bitmap)
* @brief      Put bitmap no alpha
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  bitmap : Bitmap pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutBitmapNoAlpha(double x, double y, GRPBITMAP* bitmap)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutBitmap(double x, double y, GRPBITMAP* bitmap)
* @brief      Put bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos of bitmap
* @param[in]  y : y pos of bitmap
* @param[in]  bitmap : bitmap class to draw
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutBitmap(double x, double y, GRPBITMAP* bitmap)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutBitmapAlpha(double x, double y, GRPBITMAP* bitmap, double alpha)
* @brief      Put bitmap alpha
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  alpha : Alpha value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutBitmapAlpha(double x, double y, GRPBITMAP* bitmap, double alpha)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::PutBitmapFrame(double x, double y, GRPBITMAPFRAME* bitmapframe, double alpha)
* @brief      Put bitmap frame
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos of bitmap
* @param[in]  y : y pos of bitmap
* @param[in]  bitmapframe : bitmap frame class to draw
* @param[in]  alpha : bitmap frame class to draw
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::PutBitmapFrame(double x, double y, GRPBITMAPFRAME* bitmapframe, double alpha)
{
  if(!bitmapframe)              return;
  if(!bitmapframe->GetBitmap()) return;

  if(alpha == 100)
         PutBitmap(x + bitmapframe->GetAjustX(), y + bitmapframe->GetAjustY(), bitmapframe->GetBitmap());
   else  PutBitmapAlpha(x + bitmapframe->GetAjustX(), y + bitmapframe->GetAjustY(), bitmapframe->GetBitmap(), alpha);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::RasterFont_Select(XCHAR* fontname)
* @brief      Raster font select
* @ingroup    GRAPHIC
* 
* @param[in]  fontname : Fontname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::RasterFont_Select(XCHAR* fontname)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::RasterFont_Select(XSTRING* fontname)
* @brief      Raster font select
* @ingroup    GRAPHIC
* 
* @param[in]  fontname : Fontname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::RasterFont_Select(XSTRING* fontname)
{
  return RasterFont_Select(fontname->Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR* GRP2DCANVAS::RasterFont_GetColor()
* @brief      Raster font get color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR* GRP2DCANVAS::RasterFont_GetColor()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::RasterFont_SetColor(const GRP2DCOLOR* c)
* @brief      Raster font set color
* @ingroup    GRAPHIC
* 
* @param[in]  c : C value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::RasterFont_SetColor(const GRP2DCOLOR* c)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::RasterFont_GetWidth(XCHAR* string)
* @brief      Raster font get width
* @ingroup    GRAPHIC
* 
* @param[in]  string : String to use.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::RasterFont_GetWidth(XCHAR* string)
{
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::RasterFont_GetHeight()
* @brief      Raster font get height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::RasterFont_GetHeight()
{
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::RasterFont_Printf(double x, double y, XCHAR* mask, ...)
* @brief      Raster font printf
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  mask : Mask pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::RasterFont_Printf(double x, double y, XCHAR* mask, ...)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::VectorFont_Load(XCHAR* fontnamefile)
* @brief      Vector font load
* @ingroup    GRAPHIC
* 
* @param[in]  fontnamefile : Fontnamefile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::VectorFont_Load(XCHAR* fontnamefile)
{
  XPATH fnamefile;

  fnamefile = fontnamefile;

  return VectorFont_Load(fnamefile);        
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::VectorFont_Load(XPATH& fontnamefile)
* @brief      Vector font load
* @ingroup    GRAPHIC
* 
* @param[in]  fontnamefile : Fontnamefile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::VectorFont_Load(XPATH& fontnamefile)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS_VECTORFONT_CONFIG* GRP2DCANVAS::Vectorfont_GetConfig()
* @brief      Vectorfont get config
* @ingroup    GRAPHIC
* 
* @return     GRP2DCANVAS_VECTORFONT_CONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS_VECTORFONT_CONFIG* GRP2DCANVAS::Vectorfont_GetConfig()
{
  return &vectorfont_config;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* GRP2DCANVAS::VectorFont_GetPathFile()
* @brief      Vector font get path file
* @ingroup    GRAPHIC
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* GRP2DCANVAS::VectorFont_GetPathFile()
{
  return &vectorfont_pathfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::VectorFont_CopyFrom(GRP2DCANVAS* canvas)
* @brief      Vector font copy from another canvas
* @ingroup    GRAPHIC
* 
* @param[in]  canvas : source canvas
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::VectorFont_CopyFrom(GRP2DCANVAS* canvas)
{
  if(!canvas) return false;

  GRP2DCANVAS_VECTORFONT_CONFIG* sourceconfig = canvas->Vectorfont_GetConfig();
  GRP2DCANVAS_VECTORFONT_CONFIG* targetconfig = Vectorfont_GetConfig();

  if(sourceconfig && targetconfig)
    {
      double width  = 0.0;
      double height = 0.0;

      sourceconfig->GetSize(width, height);

      targetconfig->SetSize(width, height);
      targetconfig->SetColor(sourceconfig->GetColor());
      targetconfig->SetKerning(sourceconfig->IsKerning());
    }

  XPATH* sourcepathfile = canvas->VectorFont_GetPathFile();
  if(!sourcepathfile) return false;
  if(sourcepathfile->IsEmpty()) return false;

  return VectorFont_Load((*sourcepathfile));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::VectorFont_GetWidth(XCHAR* string)
* @brief      Vector font get width
* @ingroup    GRAPHIC
* 
* @param[in]  string : String to use.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::VectorFont_GetWidth(XCHAR* string)
{
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::VectorFont_GetHeight(XCHAR* string)
* @brief      Vector font get height
* @ingroup    GRAPHIC
* 
* @param[in]  string : String to use.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::VectorFont_GetHeight(XCHAR* string)
{
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRP2DCANVAS::VectorFont_GetHeight()
* @brief      Vector font get height
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRP2DCANVAS::VectorFont_GetHeight()
{
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::VectorFont_Print(double x, double y, XCHAR* outstring)
* @brief      Vector font print
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  outstring : Output outstring.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::VectorFont_Print(double x, double y, XCHAR* outstring)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::VectorFont_Printf(double x, double y, XCHAR* mask, ...)
* @brief      Vector font printf
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  mask : Mask pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::VectorFont_Printf(double x, double y, XCHAR* mask, ...)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::CopyBufferRenderToScreen(GRPSCREEN* screen)
* @brief      Copy buffer render to screen
* @ingroup    GRAPHIC
* 
* @param[in]  screen : Screen object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::CopyBufferRenderToScreen(GRPSCREEN* screen)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::CopyBufferRenderToBufferScreen(XBYTE* buffer, XDWORD width, XDWORD height, bool isinverse)
* @brief      Copy buffer render to buffer screen
* @ingroup    GRAPHIC
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* @param[in]  isinverse : Isinverse value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::CopyBufferRenderToBufferScreen(XBYTE* buffer, XDWORD width, XDWORD height, bool isinverse)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::CopyBufferRenderFromViewport(GRPVIEWPORT* viewport)
* @brief      Copy buffer render from viewport
* @ingroup    GRAPHIC
* 
* @param[in]  viewport : Viewport pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::CopyBufferRenderFromViewport(GRPVIEWPORT* viewport)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::ReleaseDrawFramerate()
* @brief      Release draw framerate
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::ReleaseDrawFramerate()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DCANVAS::DrawFramerate(GRPSCREEN* screen, double x, double y)
* @brief      Draw framerate
* @ingroup    GRAPHIC
* 
* @param[in]  screen : Screen object to use.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DCANVAS::DrawFramerate(GRPSCREEN* screen, double x, double y)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DCANVAS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DCANVAS::Clean()
{
  buffer        = NULL;
  buffersize    = 0;

  linewidth     = 1.0f;
  dashlength    = 1.0f;
}










