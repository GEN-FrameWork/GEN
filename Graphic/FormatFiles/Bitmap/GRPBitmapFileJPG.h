/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFileJPG.h
* 
* @class      GRPBITMAPFILEJPG
* @brief      Graphic bitmap file JPEG format (JPEG: Joint Photographic Experts Group) class
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

#include <setjmp.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

#include "GRPBitmapFile.h"

extern "C"
{
 #include "jpeglib.h"
}

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

struct GRPBITMAPFILEJPG_ERROR
{
  struct jpeg_error_mgr             pub;
  jmp_buf                           setjmp_buffer;
};

typedef struct GRPBITMAPFILEJPG_ERROR*  GRPBITMAPFILEJPG_ERROR_PTR;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;

class GRPBITMAPFILEJPGHEADER
{
  public:

    XDWORD                          width;
    XDWORD                          height;
    XDWORD                          size;
};

class GRPBITMAPFILEJPG : public GRPBITMAPFILEBASE
{
  public:
                                    GRPBITMAPFILEJPG              ();
    virtual                        ~GRPBITMAPFILEJPG              ();

    GRPBITMAP*                      CreateBitmapFromFile          (XPATH& xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    GRPBITMAP*                      CreateBitmapFromBuffer        (XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);

    bool                            CreateFileFromBitmap          (XPATH& xpath, GRPBITMAP* bitmap, int quality = 100);

  private:
  
    bool                            ReadHeaderFile                (GRPBITMAPFILEJPGHEADER* header);
    bool                            ReadHeaderBuffer              (XBYTE* buffer, XDWORD size, GRPBITMAPFILEJPGHEADER* header);

    bool                            DecodeToBufferFromFile        (XBYTE* tobuffer);
    bool                            DecodeToBufferFromRawBuffer   (XBYTE* buffer, XDWORD size, XBYTE* tobuffer);

    bool                            EncodeToFile                  (int width, int height, XBYTE* buffer, int quality, bool baseline);

    void                            Clean                         ();

    struct jpeg_compress_struct     cinfo;
    struct jpeg_decompress_struct   dinfo;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





