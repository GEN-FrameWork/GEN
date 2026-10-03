/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmap.cpp
* 
* @class      GRPBITMAP
* @brief      Graphics Bitmap class
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

#include "GRPBitmap.h"

#include "GRPRect.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP::GRPBITMAP(int width, int height, GRPPROPERTYMODE mode, bool isbufferinverse)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* @param[in]  mode : Mode value.
* @param[in]  isbufferinverse : Isbufferinverse value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP::GRPBITMAP(int width, int height, GRPPROPERTYMODE mode, bool isbufferinverse)
{
  Clean();

  this->width           = width;
  this->height          = height;
  this->mode            = mode;
  this->isbufferinverse = isbufferinverse;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP::~GRPBITMAP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP::~GRPBITMAP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::IsValid()
* @brief      Is valid
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::IsValid()
{
  return isvalid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRPBITMAP::GetBuffer()
* @brief      Get buffer
* @ingroup    GRAPHIC
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPBITMAP::GetBuffer()
{
  return buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE** GRPBITMAP::GetBufferPointer()
* @brief      Get buffer pointer
* @ingroup    GRAPHIC
* 
* @return     XBYTE** : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE** GRPBITMAP::GetBufferPointer()
{
  return &buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPBITMAP::GetBufferSize()
* @brief      Get buffer size
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPBITMAP::GetBufferSize()
{
  return buffersize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAP::GetSubBitmap(GRPRECTINT& rect)
* @brief      Get sub bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  rect : rect to get sub-bitmap
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAP::GetSubBitmap(GRPRECTINT& rect)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAP::Clone()
* @brief      Clone
* @ingroup    GRAPHIC
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAP::Clone()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::CopyFrom(GRPBITMAP* bitmap)
* @brief      Copy from
* @ingroup    GRAPHIC
* 
* @param[in]  bitmap : bitmap origin to clone
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::CopyFrom(GRPBITMAP* bitmap)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAP::ConvertToMode(GRPPROPERTYMODE newmode)
* @brief      Convert to mode
* @ingroup    GRAPHIC
* 
* @param[in]  newmode : Newmode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAP::ConvertToMode(GRPPROPERTYMODE newmode)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::FlipHorizontal()
* @brief      Flip horizontal
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::FlipHorizontal()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::FlipVertical()
* @brief      Flip vertical
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::FlipVertical()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::Rotate(float degrees)
* @brief      Rotate
* @ingroup    GRAPHIC
* 
* @param[in]  degrees : degrees to rotate
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::Rotate(float degrees)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::Scale(int nwidth, int nheight)
* @brief      Scale
* @ingroup    GRAPHIC
* 
* @param[in]  nwidth : GEN_NEW width
* @param[in]  nheight : GEN_NEW height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::Scale(int nwidth, int nheight)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::Crop(GRPRECTINT& rect)
* @brief      Crop
* @ingroup    GRAPHIC
* 
* @param[in]  rect : recto to crop
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::Crop(GRPRECTINT& rect)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::Compare(GRPBITMAP* bitmap)
* @brief      Compare
* @ingroup    GRAPHIC
* 
* @param[in]  bitmap : Bitmap pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::Compare(GRPBITMAP* bitmap)
{
  if(!bitmap)
    {
      return false;
    }

  if(bitmap->GetMode() != GetMode()) 
    {
      return false;
    }

  if(bitmap->GetBufferSize() != buffersize) 
    {
      return false;
    }

  XBYTE* bufferbitmap = bitmap->GetBuffer();
  if(!bufferbitmap)
    {
      return false;
    }

  bool equal = true;

  for(XDWORD c=0; c<buffersize; c++)
    {
      if(bufferbitmap[c] != buffer[c])
        {
          equal = false;
          break;
        }
    }  

  return equal;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::FindSubBitmap(GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent, XBYTE pixelmargin)
* @brief      Find sub bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  bitmapref : Bitmapref pointer to use.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  difflimitpercent : Difflimitpercent value.
* @param[in]  pixelmargin : Pixelmargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::FindSubBitmap(GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent, XBYTE pixelmargin)
{   
  x = 0; 
  y = 0;

  if(!bitmapref || !buffer)
    {
      return false;
    }
  
  GRPBITMAP* _bitmap = bitmapref->ConvertToMode(GetMode());
  if(!_bitmap)
    {
      return false;
    }

  if(!_bitmap->GetBuffer())
    {
      GEN_DELETE _bitmap;
      return false;
    }

  XDWORD screenw = GetWidth();
  XDWORD screenh = GetHeight();
  XDWORD bmpw    = _bitmap->GetWidth();
  XDWORD bmph    = _bitmap->GetHeight();

  if(!screenw || !screenh || !bmpw || !bmph || (bmpw > screenw) || (bmph > screenh))
    {
      GEN_DELETE _bitmap;
      return false;
    }

  XDWORD* bufferscreen     = (XDWORD*)buffer;
  XDWORD* bufferbitmap     = (XDWORD*)_bitmap->GetBuffer();
  XDWORD  bufferscreensize = GetBufferSize() / sizeof(XDWORD);
  XDWORD  expectedsize     = screenw * screenh;

  if(bufferscreensize < expectedsize)
    {
      GEN_DELETE _bitmap;
      return false;
    }

  bool found = false;

  // DIB buffers are bottom-up. Only test origins where the full template fits
  // (avoids read AVs when the needle is near the right/bottom edge).
  for(XDWORD line = 0; line <= (screenh - bmph); line++)
    {
      for(XDWORD xoff = 0; xoff <= (screenw - bmpw); xoff++)
        {
          XDWORD bmpidx = 0;
          bool   rowok  = true;

          for(XDWORD brow = 0; brow < bmph; brow++)
            {
              XDWORD scrpos = ((line + brow) * screenw) + xoff;
              XDWORD ndiff  = 0;

              if((scrpos + bmpw) > bufferscreensize)
                {
                  rowok = false;
                  break;
                }

              for(XDWORD bx = 0; bx < bmpw; bx++)
                {
                  XDWORD spixel = bufferscreen[scrpos + bx];
                  XDWORD bpixel = bufferbitmap[bmpidx++];

                  if(spixel != bpixel)
                    {
                      if(!IsSimilarPixel(spixel, bpixel, pixelmargin))
                        {
                          ndiff++;
                        }
                    }
                }

              if(!DifferencesPerCent(ndiff, bmpw, difflimitpercent))
                {
                  rowok = false;
                  break;
                }
            }

          if(rowok)
            {
              x     = (int)xoff;
              y     = (int)(screenh - line - bmph);
              found = true;
              break;
            }
        }

      if(found) break;
    }

  if(!found)
    {
      x = 0;
      y = 0;
    }

  GEN_DELETE _bitmap;

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPBITMAP::GetHandle()
* @brief      Get handle
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPBITMAP::GetHandle()
{
  return handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAP::SetHandle(XDWORD handle)
* @brief      Set handle
* @ingroup    GRAPHIC
* 
* @param[in]  handle : Handle value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAP::SetHandle(XDWORD handle)
{
  this->handle = handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::DifferencesPerCent(XDWORD ndiff, XDWORD max, int limit)
* @brief      Differences per cent
* @ingroup    GRAPHIC
* 
* @param[in]  ndiff : Ndiff value.
* @param[in]  max : Max value.
* @param[in]  limit : Limit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::DifferencesPerCent(XDWORD ndiff, XDWORD max, int limit)
{
  int actualdiff = ((ndiff*100)/max);

  if(actualdiff > limit) return false;

  return true;
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAP::IsSimilarPixel(XDWORD origin, XDWORD target, XBYTE margin)
* @brief      Is similar pixel
* @ingroup    GRAPHIC
* 
* @param[in]  origin : Origin value.
* @param[in]  target : Target value.
* @param[in]  margin : Margin value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAP::IsSimilarPixel(XDWORD origin, XDWORD target, XBYTE margin)
{
  XBYTE* originRGBA = (XBYTE*)&origin;
  XBYTE* targetRGBA = (XBYTE*)&target;  
  bool   status     = false;

  int ncomponent = 0;

  for(int c=0; c<4; c++)
    {
      if(abs(originRGBA[c] - targetRGBA[c]) > margin)
        {
          ncomponent++;
        }
    }

  if(!ncomponent) 
    {
      status = true;
    }  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAP::Clean()
{
  buffer      = NULL;
  buffersize  = 0;
  isvalid     = false;
  handle      = GRPBITMAP_INVALIDHANDLE;
}






