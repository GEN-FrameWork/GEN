/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CompressLZRW1KH.h
* 
* @class      COMPRESSLZRW1KH
* @brief      Compress LZRW1/KH class
* @ingroup    COMPRESS
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

#include "CompressBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CMPFLAG_COPIED                    0x80
#define CMPFLAG_COMPRESS                  0x40

#define CMPTARGET(operation,index,data)  target?(target[index]=(XBYTE)data):CompressResult((bool)operation,(XBYTE)data,(void*)param1)


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;

class COMPRESS_LZRW1KH : public COMPRESSBASE
{
  public:

                            COMPRESS_LZRW1KH      ();
    virtual                ~COMPRESS_LZRW1KH      ();

    bool                    Compress              (XBYTE* origin, XDWORD size, XBUFFER* buffer);
    bool                    Decompress            (XBYTE* origin, XDWORD size, XBUFFER* buffer);

    bool                    Compress              (XBYTE* origin, XDWORD size, void* param1=NULL);
    bool                    Decompress            (XBYTE* origin, XDWORD size, void* param1=NULL);

  private:

    XBYTE                   GetMatch              (XBYTE* source, XDWORD x, XDWORD sourcesize, XDWORD* hash, XDWORD* size, XDWORD* pos);
    XDWORD                  CompressionBuffer     (XBYTE* source, XBYTE* target, XDWORD sourcesize);
    XDWORD                  DecompressionBuffer   (XBYTE *source, XBYTE *target, XDWORD sourcesize);

    XDWORD                  hash[4096];
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





