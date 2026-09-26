/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFileTGA.h
* 
* @class      GRPBITMAPFILETGA
* @brief      Graphic bitmap file TGA / TARGA format class (TGA  : Truevision Graphics Adapter)
* @brief                                                   (TARGA: Truevision Advanced Raster Graphics Adapter)
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XMap.h"

#include "GRPProperties.h"
#include "GRP2DColor.h"
#include "GRPBitmapFile.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum GRPBITMAPFILETGATYPE
{
  GRPBITMAPFILETGATYPE_NOIMAGE                                =  0 ,
  GRPBITMAPFILETGATYPE_UNCOMPRESS_INDEXCOLOR                  =  1 ,
  GRPBITMAPFILETGATYPE_UNCOMPRESS_RGB                         =  2 ,
  GRPBITMAPFILETGATYPE_UNCOMPRESS_BLACKWHITE                  =  3 ,
  GRPBITMAPFILETGATYPE_RLECOMPRESS_INDEXCOLOR                 =  9 ,
  GRPBITMAPFILETGATYPE_RLECOMPRESS_RGB                        = 10 ,
  GRPBITMAPFILETGATYPE_COMPRESS_BLACKWHITE                    = 11 ,
  GRPBITMAPFILETGATYPE_HUFFMANCOMPRESS_TICKET_INDEXCOLOR        = 32 ,
  GRPBITMAPFILETGATYPE_HUFFMANCOMPRESS4PASS_TICKET_INDEXCOLOR   = 33
};

enum
{
  GRPBITMAPFILETGA_SET              ,
  GRPBITMAPFILETGA_CUR              ,
  GRPBITMAPFILETGA_END
};

enum
{
  GRPBITMAPFILETGA_ITUNDEFINED      ,
  GRPBITMAPFILETGA_ITRGB            ,
  GRPBITMAPFILETGA_ITRGBA           ,
  GRPBITMAPFILETGA_ITGREYSCALE
};

enum IMAGE_ORIGIN
{
  GRPBITMAPFILETGA_BOTTOM_LEFT  = 0 ,
  GRPBITMAPFILETGA_BOTTOM_RIGHT     ,
  GRPBITMAPFILETGA_TOP_LEFT         ,
  GRPBITMAPFILETGA_TOP_RIGHT
};

#pragma pack(push, r1, 1)
typedef struct
{
  XBYTE             IDlength;
  XBYTE             colourmaptype;
  XBYTE             datatypecode;
  XWORD             colourmaporigin;
  XWORD             colourmaplength;
  XBYTE             colourmapdepth;
  XWORD             xorigin;
  XWORD             yorigin;
  XWORD             width;
  XWORD             height;
  XBYTE             bitsperpixel;
  XBYTE             imagedescriptor;

} GRPBITMAPFILETGAHEADERINFO;
#pragma pack(pop, r1)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;

class GRPBITMAPFILETGAHEADER
{
  public:
                                        GRPBITMAPFILETGAHEADER    ();
    virtual                            ~GRPBITMAPFILETGAHEADER    ();

    bool                                Load                      (XFILE* file);
    bool                                Load                      (XBYTE* buffer, XDWORD size, XDWORD& indexbuffer);

    GRPBITMAPFILETGAHEADERINFO*         GetHeaderInfo             ();

    int                                 GetWidth                  ();
    int                                 GetHeight                 ();
    int                                 GetBitsPerPixel           ();
    int                                 GetOffsetData             ();

    GRPPROPERTYMODE                     GetMode                   ();
    GRPBITMAPFILETGATYPE                GetType                   ();

  private:

    void                                Clean                     ();

    GRPPROPERTYMODE                     mode;
    GRPBITMAPFILETGAHEADERINFO          headerinfo;
    XWORD                               offsetdata;
};


class GRPBITMAPFILETGA : public GRPBITMAPFILEBASE
{
  public:
                                        GRPBITMAPFILETGA          ();
    virtual                            ~GRPBITMAPFILETGA          ();

    GRPBITMAP*                          CreateBitmapFromFile      (XPATH& xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    GRPBITMAP*                          CreateBitmapFromBuffer    (XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);

    bool                                CreateFileFromBitmap      (XPATH& xpath, GRPBITMAP* bitmap, int quality = 100);

  private:    

    bool                                ReadHeaderFile            (GRPBITMAPFILETGAHEADER* header);
    bool                                ReadHeaderBuffer          (XBYTE* buffer, XDWORD size, GRPBITMAPFILETGAHEADER* header);
    bool                                CreateColor               (GRPPROPERTYMODE mode, XBYTE* buffer, GRP2DCOLOR_RGBA8& color);

    void                                Clean                     ();

    XDWORD                              indexbuffer;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





