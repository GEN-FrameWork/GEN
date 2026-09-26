/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFileBMP.cpp
* 
* @class      GRPBITMAPFILEBMP
* @brief      Graphic bitmap file BMP (Windows bitmap Format) class
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

#include "GRPBitmapFileBMP.h"

#include "GRPFactory.h"
#include "GRPBitmap.h"
#include "GRPBitmapFileBMP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPCOLORPALETE::GRPBITMAPFILEBMPCOLORPALETE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPCOLORPALETE::GRPBITMAPFILEBMPCOLORPALETE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPCOLORPALETE::GRPBITMAPFILEBMPCOLORPALETE(XBYTE red, XBYTE green, XBYTE blue, XBYTE alpha)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  red : initial red component
* @param[in]  green : initial green component
* @param[in]  blue : initial blue component
* @param[in]  alpha : initial alpha component
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPCOLORPALETE::GRPBITMAPFILEBMPCOLORPALETE(XBYTE red, XBYTE green, XBYTE blue, XBYTE alpha)
{
  Clean();

  this->red   = red;
  this->green = green;
  this->blue  = blue;
  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPCOLORPALETE::~GRPBITMAPFILEBMPCOLORPALETE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPCOLORPALETE::~GRPBITMAPFILEBMPCOLORPALETE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEBMPCOLORPALETE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEBMPCOLORPALETE::Clean()
{
  red   = 0;
  green = 0;
  blue  = 0;
  alpha = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPHEADER::GRPBITMAPFILEBMPHEADER()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPHEADER::GRPBITMAPFILEBMPHEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPHEADER::~GRPBITMAPFILEBMPHEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPHEADER::~GRPBITMAPFILEBMPHEADER()
{
  Clean();

  DeleteAllPalette();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMPHEADER::Load(XFILE* file)
* @brief      Load
* @ingroup    GRAPHIC
* 
* @param[in]  file : File object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMPHEADER::Load(XFILE* file)
{
  if(!file->Read((XBYTE*)&headerinfo.type             , sizeof(XWORD)))     return false;
  if(!file->Read((XBYTE*)&headerinfo.size             , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.reserved1        , sizeof(XWORD)))     return false;
  if(!file->Read((XBYTE*)&headerinfo.reserved2        , sizeof(XWORD)))     return false;
  if(!file->Read((XBYTE*)&headerinfo.offsetdata       , sizeof(XDWORD)))    return false;

  if(!file->Read((XBYTE*)&headerinfo.headersize       , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.width            , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.height           , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.planes           , sizeof(XWORD)))     return false;
  if(!file->Read((XBYTE*)&headerinfo.bitsperpixel     , sizeof(XWORD)))     return false;
  if(!file->Read((XBYTE*)&headerinfo.compression      , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.sizebmp          , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.xpixelspermeter  , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.ypixelspermeter  , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.colorused        , sizeof(XDWORD)))    return false;
  if(!file->Read((XBYTE*)&headerinfo.colorimportant   , sizeof(XDWORD)))    return false;


  // Check: "BM" signature in the file for WINDOWS NO OS/2 valid format.
  if(headerinfo.type!=0x4D42) return false;

  switch(headerinfo.bitsperpixel)
    {
      case  8 : mode = GRPPROPERTYMODE_08_INDEX;
                break;

      case 16 : { XDWORD maskcolor[4];
                  if(!file->Read((XBYTE*)maskcolor, 16)) return false;

                  if((maskcolor[0] == 0x00007c00) && (maskcolor[1] == 0x000003E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00008000)) mode = GRPPROPERTYMODE_16_RGBA_5551;
                  if((maskcolor[0] == 0x0000F800) && (maskcolor[1] == 0x000007E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00000000)) mode = GRPPROPERTYMODE_16_RGB_565;
                }
                break;


      case 24 : mode = GRPPROPERTYMODE_24_RGB_888;
                break;

      case 32 : mode = GRPPROPERTYMODE_32_RGBA_8888;
                break;
    }


  DeleteAllPalette();

  int ncpalette = headerinfo.colorused;

  if(ncpalette)
    {
      for(int c=0;c<ncpalette;c++)
        {
          XBYTE red;
          XBYTE green;
          XBYTE blue;
          XBYTE alpha;

          if(!file->Read(&blue, 1)) return false;
          if(!file->Read(&green, 1)) return false;
          if(!file->Read(&red, 1)) return false;
          if(!file->Read(&alpha, 1)) return false;

          GRPBITMAPFILEBMPCOLORPALETE* colorpalette = GEN_NEW GRPBITMAPFILEBMPCOLORPALETE(red, green, blue, alpha);
          if(colorpalette) palette.Add((XBYTE)c, colorpalette);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMPHEADER::Load(XBYTE* buffer, XDWORD size, XDWORD& indexbuffer)
* @brief      Load
* @ingroup    GRAPHIC
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  indexbuffer : Indexbuffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMPHEADER::Load(XBYTE* buffer, XDWORD size, XDWORD& indexbuffer)
{
  memcpy((XBYTE*)&headerinfo.type, &buffer[indexbuffer], sizeof(XWORD));     indexbuffer += sizeof(XWORD);
  memcpy((XBYTE*)&headerinfo.size, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.reserved1, &buffer[indexbuffer], sizeof(XWORD));     indexbuffer += sizeof(XWORD);
  memcpy((XBYTE*)&headerinfo.reserved2, &buffer[indexbuffer], sizeof(XWORD));     indexbuffer += sizeof(XWORD);
  memcpy((XBYTE*)&headerinfo.offsetdata, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);

  memcpy((XBYTE*)&headerinfo.headersize, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.width, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.height, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.planes, &buffer[indexbuffer], sizeof(XWORD));     indexbuffer += sizeof(XWORD);
  memcpy((XBYTE*)&headerinfo.bitsperpixel, &buffer[indexbuffer], sizeof(XWORD));     indexbuffer += sizeof(XWORD);
  memcpy((XBYTE*)&headerinfo.compression, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.sizebmp, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.xpixelspermeter, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.ypixelspermeter, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.colorused, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);
  memcpy((XBYTE*)&headerinfo.colorimportant, &buffer[indexbuffer], sizeof(XDWORD));    indexbuffer += sizeof(XDWORD);

  // Check: "BM" signature in the file for WINDOWS NO OS/2 valid format.
  if(headerinfo.type!=0x4D42) return false;

  switch(headerinfo.bitsperpixel)
    {
      case  8 : mode = GRPPROPERTYMODE_08_INDEX;
                break;

      case 16 : { XDWORD maskcolor[4];
                  memcpy((XBYTE*)maskcolor, &buffer[indexbuffer], 16); indexbuffer += 16;

                  if((maskcolor[0] == 0x00007c00) && (maskcolor[1] == 0x000003E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00008000)) mode = GRPPROPERTYMODE_16_RGBA_5551;
                  if((maskcolor[0] == 0x0000F800) && (maskcolor[1] == 0x000007E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00000000)) mode = GRPPROPERTYMODE_16_RGB_565;
                }
                break;


      case 24 : mode = GRPPROPERTYMODE_24_RGB_888;
                break;

      case 32 : mode = GRPPROPERTYMODE_32_RGBA_8888;
                break;
    }


  DeleteAllPalette();

  int ncpalette = headerinfo.colorused;

  if(ncpalette)
    {
      for(int c=0;c<ncpalette;c++)
        {
          XBYTE red;
          XBYTE green;
          XBYTE blue;
          XBYTE alpha;

          memcpy((XBYTE*)&blue, &buffer[indexbuffer], 1);   indexbuffer++;
          memcpy((XBYTE*)&green , &buffer[indexbuffer], 1);   indexbuffer++;
          memcpy((XBYTE*)&red, &buffer[indexbuffer], 1);   indexbuffer++;
          memcpy((XBYTE*)&alpha , &buffer[indexbuffer], 1);   indexbuffer++;

          GRPBITMAPFILEBMPCOLORPALETE* colorpalette = GEN_NEW GRPBITMAPFILEBMPCOLORPALETE(red, green, blue, alpha);
          if(colorpalette) palette.Add((XBYTE)c, colorpalette);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMPHEADERINFO* GRPBITMAPFILEBMPHEADER::GetHeaderInfo()
* @brief      Get header info
* @ingroup    GRAPHIC
* 
* @return     GRPBITMAPFILEBMPHEADERINFO* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMPHEADERINFO* GRPBITMAPFILEBMPHEADER::GetHeaderInfo()
{
  return &headerinfo;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPBITMAPFILEBMPHEADER::GetWidth()
* @brief      Get width
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPBITMAPFILEBMPHEADER::GetWidth()
{
  return headerinfo.width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPBITMAPFILEBMPHEADER::GetHeight()
* @brief      Get height
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPBITMAPFILEBMPHEADER::GetHeight()
{
  return headerinfo.height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPBITMAPFILEBMPHEADER::GetBitsPerPixel()
* @brief      Get bits per pixel
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPBITMAPFILEBMPHEADER::GetBitsPerPixel()
{
  return headerinfo.bitsperpixel;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPBITMAPFILEBMPHEADER::GetOffsetData()
* @brief      Get offset data
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPBITMAPFILEBMPHEADER::GetOffsetData()
{
  return headerinfo.offsetdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPPROPERTYMODE GRPBITMAPFILEBMPHEADER::GetMode()
* @brief      Get mode
* @ingroup    GRAPHIC
* 
* @return     GRPPROPERTYMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPPROPERTYMODE GRPBITMAPFILEBMPHEADER::GetMode()
{
  return mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMPHEADER::GetColorPalette(XBYTE index, GRP2DCOLOR_RGBA8& color)
* @brief      Get color palette
* @ingroup    GRAPHIC
* 
* @param[in]  index : index into palette
* @param[in]  color : color of palette
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMPHEADER::GetColorPalette(XBYTE index, GRP2DCOLOR_RGBA8& color)
{
  GRPBITMAPFILEBMPCOLORPALETE* colorpalette = (GRPBITMAPFILEBMPCOLORPALETE*)palette.GetElement(index);
  if(!colorpalette) return false;

  color.r = colorpalette->red;
  color.g = colorpalette->green;
  color.b = colorpalette->blue;
  color.a = colorpalette->alpha;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMPHEADER::DeleteAllPalette()
* @brief      Delete all palette
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMPHEADER::DeleteAllPalette()
{
  if(palette.IsEmpty()) return true;

  palette.DeleteElementContents();

  palette.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEBMPHEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEBMPHEADER::Clean()
{
  mode = GRPPROPERTYMODE_XX_UNKNOWN;
  memset((XBYTE*)&headerinfo, 0, sizeof(GRPBITMAPFILEBMPHEADERINFO));
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMP::GRPBITMAPFILEBMP(): GRPBITMAPFILEBASE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMP::GRPBITMAPFILEBMP(): GRPBITMAPFILEBASE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBMP::~GRPBITMAPFILEBMP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBMP::~GRPBITMAPFILEBMP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEBMP::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
* @brief      Create bitmap from file
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEBMP::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
{
  if(!Open(xpath))  return NULL;

  GRPBITMAPFILEBMPHEADER  header;
  GRPBITMAP*              bitmap = NULL;
//XBYTE*                  buffer = NULL;
  bool                    status = true;

  if(!ReadHeaderFile(&header))
    {
      Close();
      return NULL;
    }

  int  width        = header.GetWidth();
  int  height       = header.GetHeight();
  int  bytesppixel  = (header.GetBitsPerPixel()/8);

  bitmap = GRPFACTORY::GetInstance().CreateBitmap(width, height, header.GetMode());
  if(bitmap)
    {
      int     sizebmp       = (((header.GetWidth()*bytesppixel)+3)&(~3))*header.GetHeight();
      XBYTE*  databuffer    = NULL;
      int     index         = 0;

      databuffer = GEN_NEW XBYTE[sizebmp];
      if(databuffer)
        {
          file->SetPosition(header.GetOffsetData());

          if(!file->Read(databuffer, sizebmp)) return NULL;

          for(int y=height-1; y>-1; y--)
            {
              for(int x=0; x<width; x++)
                {
                  GRP2DCOLOR_RGBA8 color;

                  switch(header.GetMode())
                    {
                      case GRPPROPERTYMODE_08_INDEX     : header.GetColorPalette(databuffer[index], color);
                                                          color.a = 255;
                                                          bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          break;

                      case GRPPROPERTYMODE_16_RGBA_5551 : { XWORD rgb = databuffer[index+1];
                                                            rgb <<= 8;
                                                            rgb |= databuffer[index+0];

                                                            color.r = (rgb >> 7) & 0xF8;
                                                            color.g = (rgb >> 2) & 0xF8;
                                                            color.b = (rgb << 3) & 0xF8;
                                                            color.a = (rgb & 0x8000)?255:0;

                                                            bitmap->PutBlendPixel(x, y, (GRP2DCOLOR*)&color, (int)color.a);
                                                          }
                                                          break;

                      case GRPPROPERTYMODE_16_RGB_565   : { XWORD rgb = databuffer[index+1];
                                                            rgb <<= 8;
                                                            rgb |= databuffer[index+0];

                                                            color.r = (rgb >> 8) & 0xF8;
                                                            color.g = (rgb >> 3) & 0xFC;
                                                            color.b = (rgb << 3) & 0xF8;
                                                            color.a = 255;

                                                            bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          }
                                                          break;

                      case GRPPROPERTYMODE_24_RGB_888   : color.r = databuffer[index+2];
                                                          color.g = databuffer[index+1];
                                                          color.b = databuffer[index+0];
                                                          color.a = 255;

                                                          bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          break;

                      case GRPPROPERTYMODE_32_RGBA_8888 : color.r = databuffer[index+2];
                                                          color.g = databuffer[index+1];
                                                          color.b = databuffer[index+0];
                                                          color.a = databuffer[index+3];

                                                          bitmap->PutBlendPixel(x, y, (GRP2DCOLOR*)&color, (int)color.a);
                                                          break;
                      default: break;
                    }

                  index  += bytesppixel;
                }
            }

        } else status = false;

      GEN_DELETE_ARRAY databuffer;

    } else status = false;


  if(!status)
    {
      if(bitmap)
        {
          GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
          bitmap = NULL;
        }
    }

  Close();

  return ForceModeOfBitmap(forcemode, bitmap);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEBMP::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
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
GRPBITMAP* GRPBITMAPFILEBMP::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
{
  GRPBITMAPFILEBMPHEADER  header;
  GRPBITMAP*              bitmap  = NULL;
  bool                    status  = true;

  indexbuffer = 0;

  if(!ReadHeaderBuffer(buffer, size, &header))
    {
      return NULL;
    }

  int  width        = header.GetWidth();
  int  height       = header.GetHeight();
  int  bytesppixel  = (header.GetBitsPerPixel()/8);

  bitmap = GRPFACTORY::GetInstance().CreateBitmap(width, height, header.GetMode());
  if(bitmap)
    {
      int     sizebmp       = (((header.GetWidth()*bytesppixel)+3)&(~3))*header.GetHeight();
      XBYTE*  databuffer    = NULL;
      int     index         = 0;

      databuffer = GEN_NEW XBYTE[sizebmp];
      if(databuffer)
        {
          memcpy(databuffer, &buffer[indexbuffer], sizebmp);

          indexbuffer += sizebmp;

          for(int y=height-1; y>-1; y--)
            {
              for(int x=0; x<width; x++)
                {
                  GRP2DCOLOR_RGBA8 color;

                  switch(header.GetMode())
                    {
                      case GRPPROPERTYMODE_08_INDEX     : header.GetColorPalette(databuffer[index], color);
                                                          color.a = 255;
                                                          bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          break;

                      case GRPPROPERTYMODE_16_RGBA_5551 : { XWORD rgb = databuffer[index+1];
                                                            rgb <<= 8;
                                                            rgb |= databuffer[index+0];

                                                            color.r = (rgb >> 7) & 0xF8;
                                                            color.g = (rgb >> 2) & 0xF8;
                                                            color.b = (rgb << 3) & 0xF8;
                                                            color.a = (rgb & 0x8000)?255:0;

                                                            bitmap->PutBlendPixel(x, y, (GRP2DCOLOR*)&color, (int)color.a);
                                                          }
                                                          break;

                      case GRPPROPERTYMODE_16_RGB_565   : { XWORD rgb = databuffer[index+1];
                                                            rgb <<= 8;
                                                            rgb |= databuffer[index+0];

                                                            color.r = (rgb >> 8) & 0xF8;
                                                            color.g = (rgb >> 3) & 0xFC;
                                                            color.b = (rgb << 3) & 0xF8;
                                                            color.a = 255;

                                                            bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          }
                                                          break;

                      case GRPPROPERTYMODE_24_RGB_888   : color.r = databuffer[index+2];
                                                          color.g = databuffer[index+1];
                                                          color.b = databuffer[index+0];
                                                          color.a = 255;

                                                          bitmap->PutPixel(x, y, (GRP2DCOLOR*)&color);
                                                          break;

                      case GRPPROPERTYMODE_32_RGBA_8888 : color.r = databuffer[index+2];
                                                          color.g = databuffer[index+1];
                                                          color.b = databuffer[index+0];
                                                          color.a = databuffer[index+3];

                                                          bitmap->PutBlendPixel(x, y, (GRP2DCOLOR*)&color, (int)color.a);
                                                          break;
                      default: break;
                    }

                  index  += bytesppixel;
                }
            }

        } else status = false;

      GEN_DELETE_ARRAY databuffer;

    } else status = false;


  if(!status)
    {
      if(bitmap)
        {
          GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
          bitmap = NULL;
        }
    }

  return ForceModeOfBitmap(forcemode, bitmap);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMP::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
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
bool GRPBITMAPFILEBMP::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
{
  if(!bitmap)     return false;

  if(!Create(xpath)) return false;

  GRPBITMAPFILEBMPHEADER header;

  header.GetHeaderInfo()->type            = 0x4D42;  // BM Windows
  header.GetHeaderInfo()->size            = 0;
  header.GetHeaderInfo()->reserved1       = 0;
  header.GetHeaderInfo()->reserved2       = 0;
  header.GetHeaderInfo()->offsetdata      = sizeof(GRPBITMAPFILEBMPHEADERINFO);

  header.GetHeaderInfo()->headersize      = sizeof(GRPBITMAPFILEBMPHEADERINFO) - 14;
  header.GetHeaderInfo()->width           = bitmap->GetWidth();
  header.GetHeaderInfo()->height          = bitmap->GetHeight();
  header.GetHeaderInfo()->planes          = 1;
  header.GetHeaderInfo()->bitsperpixel    = 32;
  header.GetHeaderInfo()->compression     = GRPBITMAPFILEBMPCOMPRESSION_NOT;
  header.GetHeaderInfo()->sizebmp         = 0;
  header.GetHeaderInfo()->xpixelspermeter = 2835;
  header.GetHeaderInfo()->ypixelspermeter = 2835;
  header.GetHeaderInfo()->colorused       = 0;
  header.GetHeaderInfo()->colorimportant  = 0;

  int  width        = bitmap->GetWidth();
  int  height       = bitmap->GetHeight();
  int  bytesppixel  = (header.GetBitsPerPixel()/8);
  int  sizebmp      = (((header.GetWidth()*bytesppixel)+3)&(~3))*header.GetHeight();
  int  index        = 0;
  bool status       = true;

  header.GetHeaderInfo()->sizebmp = sizebmp;
  header.GetHeaderInfo()->size      = sizebmp + sizeof(GRPBITMAPFILEBMPHEADERINFO);

  if(file->Write((XBYTE*)header.GetHeaderInfo(), sizeof(GRPBITMAPFILEBMPHEADERINFO)))
    {
      XBYTE* databuffer = GEN_NEW XBYTE[sizebmp];
      if(databuffer)
        {
          for(int y=height-1; y>-1; y--)
            {
              for(int x=0; x<width; x++)
                {
                  GRP2DCOLOR_RGBA8* color = (GRP2DCOLOR_RGBA8*)bitmap->GetPixel(x, y);
                  if(color)
                    {
                      databuffer[index+2] = color->r;
                      databuffer[index+1] = color->g;
                      databuffer[index+0] = color->b;
                      databuffer[index+3] = color->a;
                    }

                  index  += bytesppixel;
                }
            }

          if(!file->Write((XBYTE*)databuffer, sizebmp)) status = false;

          GEN_DELETE_ARRAY databuffer;

        } else status = false;

    } else status = false;

  Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMP::ReadHeaderFile(GRPBITMAPFILEBMPHEADER* header)
* @brief      Read header file
* @ingroup    GRAPHIC
* 
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMP::ReadHeaderFile(GRPBITMAPFILEBMPHEADER* header)
{
  if(!header)           return false;
  if(!GetPrimaryFile()) return false;

  if(!header->Load(GetPrimaryFile())) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBMP::ReadHeaderBuffer(XBYTE* buffer, XDWORD size, GRPBITMAPFILEBMPHEADER* header)
* @brief      Read header buffer
* @ingroup    GRAPHIC
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBMP::ReadHeaderBuffer(XBYTE* buffer, XDWORD size, GRPBITMAPFILEBMPHEADER* header)
{
  if(!header->Load(buffer, size, indexbuffer)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEBMP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEBMP::Clean()
{
  indexbuffer = 0;
}







