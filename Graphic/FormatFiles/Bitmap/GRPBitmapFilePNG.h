/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFilePNG.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "png.h"

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XMap.h"

#include "GRPProperties.h"
#include "GRPBitmapFile.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPBITMAPFILEPNG_HEADERSIZE 8

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;

class GRPBITMAPFILEPNG : public GRPBITMAPFILEBASE
{
  public:
                                GRPBITMAPFILEPNG          ();
    virtual                    ~GRPBITMAPFILEPNG          ();

    GRPBITMAP*                  CreateBitmapFromFile      (XPATH& xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    GRPBITMAP*                  CreateBitmapFromBuffer    (XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);

    bool                        CreateFileFromBitmap      (XPATH& xpath, GRPBITMAP* bitmap, int quality = 100);

    XBYTE*                      BufferData_Get            ();
    XDWORD                      BufferData_GetIndex       ();
    void                        BufferData_SetIndex       (XDWORD index);
    XDWORD                      BufferData_GetSize        ();

  private:

    static void                 ReadFileData              (png_structp png, png_bytep data, png_size_t length);
    static void                 ReadBufferData            (png_structp png, png_bytep data, png_size_t length);

    static void                 WriteFileData             (png_structp png, png_bytep data, png_size_t length);

    void                        Clean                     ();

    XBYTE*                      bufferdata;
    XDWORD                      bufferdataindex;
    XDWORD                      bufferdatasize;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





