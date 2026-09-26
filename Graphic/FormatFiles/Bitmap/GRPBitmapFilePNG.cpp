/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFilePNG.cpp
* 
* @class      GRPBITMAPFILEPNG
* @brief      Graphic bitmap file PNG format (PNG: Portable Network Graphics) class
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

#include "GRPBitmapFilePNG.h"

#include "GRPFactory.h"
#include "GRPBitmap.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEPNG::GRPBITMAPFILEPNG(): GRPBITMAPFILEBASE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEPNG::GRPBITMAPFILEPNG(): GRPBITMAPFILEBASE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEPNG::~GRPBITMAPFILEPNG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEPNG::~GRPBITMAPFILEPNG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEPNG::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
* @brief      Create bitmap from file
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEPNG::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
{
  if(!Open(xpath))  return NULL;

  GRPBITMAP*    bitmap      = NULL;
  png_bytep*    databuffer  = NULL;
  XBYTE         header[8];
  png_structp   png;
  png_infop     info;
  png_uint_32   width       = 0;
  png_uint_32   height      = 0;
  png_byte      colortype   = 0;
  int           bytesppixel = 0;
  int           rowbytes    = 0;
//int           npasses     = 0;

  if(file->Read(header, GRPBITMAPFILEPNG_HEADERSIZE))
    {
      if(!png_sig_cmp(header, 0, 8))
        {
          png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
          if(png)
            {
              info = png_create_info_struct(png);
              if(info)
                {
                  if(!setjmp(png_jmpbuf(png)))
                    {
                      png_set_read_fn(png , (void *)this, GRPBITMAPFILEPNG::ReadFileData);

                      png_set_sig_bytes(png, 8);

                      png_read_info(png, info);

                      width        = png_get_image_width(png, info);
                      height       = png_get_image_height(png, info);
                      colortype    = png_get_color_type(png, info);
                      bytesppixel  = png_get_channels(png, info);
                      rowbytes     = (int)png_get_rowbytes(png, info);
                      //npasses      = png_set_interlace_handling(png);

                      png_read_update_info(png, info);

                      if(!setjmp(png_jmpbuf(png)))
                        {
                          databuffer = (png_bytep*)GEN_NEW XBYTE[sizeof(png_bytep) * height];
                          for(int y=0; y<(int)height; y++)
                            {
                              databuffer[y] = (png_byte*)GEN_NEW XBYTE[rowbytes];
                            }

                          png_read_image(png, databuffer);
                          png_destroy_read_struct(&png, &info, NULL);

                          GRPPROPERTYMODE mode = GRPPROPERTYMODE_XX_UNKNOWN;

                          switch(colortype)
                            {
                              case PNG_COLOR_TYPE_RGB         : mode = GRPPROPERTYMODE_24_RGB_888;
                              case PNG_COLOR_TYPE_RGB_ALPHA   : mode = GRPPROPERTYMODE_32_RGBA_8888;
                            }

                          bitmap = GRPFACTORY::GetInstance().CreateBitmap(width, height, mode);
                          if(bitmap)
                            {
                              int index = 0;

                              for(int y=0; y<(int)height; y++)
                                {
                                  index = 0;

                                  for(int x=0; x<(int)width; x++)
                                    {
                                      GRP2DCOLOR_RGBA8 color;

                                      color.r = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 0));
                                      color.g = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 1));
                                      color.b = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 2));

                                      if(colortype == PNG_COLOR_TYPE_RGB)
                                        {
                                          color.a = 255;
                                        }
                                      else
                                        {
                                          color.a = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 3));
                                        }

                                      bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);

                                      index  += bytesppixel;
                                    }
                                }

                            }

                          for(int y=0; y<(int)height; y++)
                            {
                              GEN_DELETE_ARRAY databuffer[y];
                            }

                          GEN_DELETE_ARRAY databuffer;
                        }
                    }
                }
            }
        }
    }

  Close();

  return ForceModeOfBitmap(forcemode, bitmap);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEPNG::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
* @brief      Create bitmap from buffer
* @ingroup    GRAPHIC
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEPNG::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
{
  if(!buffer)                            return NULL;
  if(size<GRPBITMAPFILEPNG_HEADERSIZE)   return NULL;

  GRPBITMAP*    bitmap      = NULL;
  png_bytep*    databuffer  = NULL;
  XBYTE         header[8];
  png_structp   png;
  png_infop     info;
  png_uint_32   width       = 0;
  png_uint_32   height      = 0;
  png_byte      colortype   = 0;
  int           bytesppixel = 0;
  int           rowbytes    = 0;
  int           npasses     = 0;

  bufferdata      = buffer;
  bufferdatasize  = size;
  bufferdataindex = GRPBITMAPFILEPNG_HEADERSIZE;

  memcpy(header, buffer, GRPBITMAPFILEPNG_HEADERSIZE);

  bufferdataindex = GRPBITMAPFILEPNG_HEADERSIZE;

  if(!png_sig_cmp(header, 0, 8))
    {
      png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
      if(png)
        {
          info = png_create_info_struct(png);
          if(info)
            {
              if(!setjmp(png_jmpbuf(png)))
                {
                  png_set_read_fn(png , (void *)this, GRPBITMAPFILEPNG::ReadBufferData);

                  png_set_sig_bytes(png, 8);

                  png_read_info(png, info);

                  width        = png_get_image_width(png, info);
                  height       = png_get_image_height(png, info);
                  colortype    = png_get_color_type(png, info);
                  bytesppixel  = png_get_channels(png, info);
                  rowbytes     = (int)png_get_rowbytes(png, info);
                  npasses      = png_set_interlace_handling(png);

                  png_read_update_info(png, info);

                  if(!setjmp(png_jmpbuf(png)))
                    {
                      databuffer = (png_bytep*)GEN_NEW XBYTE[sizeof(png_bytep) * height];
                      for(int y=0; y<(int)height; y++)
                        {
                          databuffer[y] = (png_byte*)GEN_NEW XBYTE[rowbytes];
                        }

                      png_read_image(png, databuffer);
                      png_destroy_read_struct(&png, &info, NULL);

                      GRPPROPERTYMODE mode = GRPPROPERTYMODE_XX_UNKNOWN;

                      switch(colortype)
                        {
                          case PNG_COLOR_TYPE_RGB         : mode = GRPPROPERTYMODE_24_RGB_888;
                          case PNG_COLOR_TYPE_RGB_ALPHA   : mode = GRPPROPERTYMODE_32_RGBA_8888;
                        }

                      bitmap = GRPFACTORY::GetInstance().CreateBitmap(width, height, mode);
                      if(bitmap)
                        {
                          int index = 0;

                          for(int y=0; y<(int)height; y++)
                            {
                              index = 0;

                              for(int x=0; x<(int)width; x++)
                                {
                                  GRP2DCOLOR_RGBA8 color;

                                  color.r = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 0));
                                  color.g = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 1));
                                  color.b = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 2));

                                  if(colortype == PNG_COLOR_TYPE_RGB)
                                    {
                                      color.a = 255;
                                    }
                                  else
                                    {
                                      color.a = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 3));
                                    }

                                  bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);

                                  index  += bytesppixel;
                                }
                            }

                        }

                      for(int y=0; y<(int)height; y++)
                        {
                          GEN_DELETE_ARRAY databuffer[y];
                        }

                      GEN_DELETE_ARRAY databuffer;
                    }
                }
            }
        }
    }

  return ForceModeOfBitmap(forcemode, bitmap);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEPNG::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
* @brief      Create file from bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  quality : Quality value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEPNG::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
{
  if(!bitmap)     return false;

  if(!Create(xpath)) return false;

  png_structp   png;
  png_infop     info;
  png_bytep*    databuffer  = NULL;
  png_uint_32   width       = bitmap->GetWidth();
  png_uint_32   height      = bitmap->GetHeight();
  png_byte      bitdepth    = 8;
  int           bytesppixel = 4;
  int           rowbytes    = 0;
  bool          status      = true;

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(png)
    {
      info = png_create_info_struct(png);
      if(info)
        {
          if(!setjmp(png_jmpbuf(png)))
            {
              png_set_write_fn(png, (void *)this, GRPBITMAPFILEPNG::WriteFileData, NULL);

              if(!setjmp(png_jmpbuf(png)))
                {
                  rowbytes = width * 4;

                  databuffer = (png_bytep*)GEN_NEW XBYTE[sizeof(png_bytep) * height];
                  for(int y=0; y<(int)height; y++)
                    {
                      databuffer[y] = (png_byte*)GEN_NEW XBYTE[rowbytes];
                    }

                  png_set_IHDR(png, info, width                     ,
                                          height                    ,
                                          bitdepth                  ,
                                          PNG_COLOR_TYPE_RGB_ALPHA  ,
                                          PNG_INTERLACE_NONE        ,
                                          PNG_COMPRESSION_TYPE_BASE ,
                                          PNG_FILTER_TYPE_BASE);

                  png_write_info(png, info);

                  if(!setjmp(png_jmpbuf(png)))
                    {
                      int index = 0;

                      for(int y=0; y<(int)height; y++)
                        {
                          index = 0;

                          for(int x=0; x<(int)width; x++)
                            {
                              GRP2DCOLOR_RGBA8* color = (GRP2DCOLOR_RGBA8*)bitmap->GetPixel(x, y);
                              if(color)
                                {
                                  XBYTE* ptr = (XBYTE*)(databuffer[y] + index);

                                  ptr[0] = color->r;
                                  ptr[1] = color->g;
                                  ptr[2] = color->b;
                                  ptr[3] = color->a;                                 
                                }

                              index  += bytesppixel;
                            }
                        }

                      png_write_image(png, databuffer);

                      if(!setjmp(png_jmpbuf(png)))
                        {
                          png_write_end(png, NULL);
                        }
                    }

                  for(int y=0; y<(int)height; y++)
                    {
                      GEN_DELETE_ARRAY databuffer[y];
                    }

                  GEN_DELETE_ARRAY databuffer;

                }
            }
        }
    }

  Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRPBITMAPFILEPNG::BufferData_Get()
* @brief      Buffer data get
* @ingroup    GRAPHIC
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPBITMAPFILEPNG::BufferData_Get()
{
  return bufferdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPBITMAPFILEPNG::BufferData_GetIndex()
* @brief      Buffer data get index
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPBITMAPFILEPNG::BufferData_GetIndex()
{
  return bufferdataindex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEPNG::BufferData_SetIndex(XDWORD index)
* @brief      Buffer data set index
* @ingroup    GRAPHIC
* 
* @param[in]  index : Index value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEPNG::BufferData_SetIndex(XDWORD index)
{
  this->bufferdataindex = index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPBITMAPFILEPNG::BufferData_GetSize()
* @brief      Buffer data get size
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPBITMAPFILEPNG::BufferData_GetSize()
{
  return bufferdatasize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEPNG::ReadFileData(png_structp png, png_bytep data, png_size_t length)
* @brief      Read file data
* @ingroup    GRAPHIC
* 
* @param[in]  png : struct png
* @param[in]  data : data of PNG
* @param[in]  length : size of data
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEPNG::ReadFileData(png_structp png, png_bytep data, png_size_t length)
{
  GRPBITMAPFILEPNG* xfilepng = (GRPBITMAPFILEPNG*)png_get_io_ptr(png);
  if(!xfilepng) return;

  xfilepng->GetPrimaryFile()->Read((XBYTE*)data, (int)length);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEPNG::ReadBufferData(png_structp png, png_bytep data, png_size_t length)
* @brief      Read buffer data
* @ingroup    GRAPHIC
* 
* @param[in]  png : Png value.
* @param[in]  data : Data buffer to use.
* @param[in]  length : Length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEPNG::ReadBufferData(png_structp png, png_bytep data, png_size_t length)
{
  GRPBITMAPFILEPNG* xfilepng = (GRPBITMAPFILEPNG*)png_get_io_ptr(png);
  if(!xfilepng) return;

  XBYTE* buffer = xfilepng->BufferData_Get();
  int    index  = xfilepng->BufferData_GetIndex();

  memcpy((XBYTE*)data, &buffer[index], (int)length);

  index+= (int)length;

  xfilepng->BufferData_SetIndex(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEPNG::WriteFileData(png_structp png, png_bytep data, png_size_t length)
* @brief      Write file data
* @ingroup    GRAPHIC
* 
* @param[in]  png : png struct
* @param[in]  data : data of PNG
* @param[in]  length : size of data
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEPNG::WriteFileData(png_structp png, png_bytep data, png_size_t length)
{
  GRPBITMAPFILEPNG* xfilepng = (GRPBITMAPFILEPNG*)png_get_io_ptr(png);
  if(!xfilepng) return;

  xfilepng->GetPrimaryFile()->Write((XBYTE*)data, (int)length);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEPNG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEPNG::Clean()
{
  bufferdata        = NULL;
  bufferdataindex   = 0;
  bufferdatasize    = 0;
}


