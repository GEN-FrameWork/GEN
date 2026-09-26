/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CompressDeflate.h
*
* @class      COMPRESS_DEFLATE
* @brief      Compress Deflate class
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

#define COMPRESS_DEFLATE_CHUNK          16384
#define COMPRESS_DEFLATE_WINDOWBITS     15                                      // zlib-wrapped deflate (RFC 1950)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;

// HTTP's "deflate" Content-Encoding is, per RFC 2616/7231, a zlib-wrapped DEFLATE stream (RFC 1950). In practice a
// number of servers instead send a raw DEFLATE stream with no zlib header (RFC 1951), a long-standing interop bug
// that HTTP clients are expected to tolerate. Decompress() follows that convention: it first tries the correct
// zlib-wrapped format and, only if that fails outright, retries once as raw deflate. Compress() always produces
// the RFC-correct zlib-wrapped form.
class COMPRESS_DEFLATE : public COMPRESSBASE
{
  public:
                            COMPRESS_DEFLATE      ();
    virtual                ~COMPRESS_DEFLATE      ();

    bool                    Compress              (XBYTE* source, XDWORD size, XBUFFER* buffer);
    bool                    Decompress            (XBYTE* source, XDWORD size, XBUFFER* buffer);

  private:

    void                    Clean                 ();

    int                     ZCompress             (XBUFFER* target, XBYTE* source, XDWORD sourcesize);
    int                     ZDecompress           (XBUFFER* target, XBYTE* source, XDWORD sourcesize, int windowbits);

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


