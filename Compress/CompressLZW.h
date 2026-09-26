/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CompressLZW.h
* 
* @class      COMPRESSLZW
* @brief      Compress LZW (GIF compression) class
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

#define   LZW_MAX_TABLE_SIZE    4096
#define   LZW_MAX_HASHSIZE      0x1000FF // (4096 << 8) + 0xFF

#define   LZW_MAX_OVERCMP       1024       // A?adido por si la compresion es deficiente.

typedef struct tagLZW_STRING
{
  XWORD prefix;
  XWORD suffix;

} LZW_STRING, *PLZW_STRING;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class COMPRESS_LZW : public COMPRESSBASE
{
  public :
                           COMPRESS_LZW                       ();
    virtual               ~COMPRESS_LZW                       ();

    bool                   Compress                           (XBYTE* source, XDWORD size, XBUFFER* buffer);
    bool                   Decompress                         (XBYTE* source, XDWORD size, XBUFFER* buffer);

    XDWORD                 LZW_Encode                         (XBYTE* inbuffer, XDWORD length, XBYTE* outbuffer);
    void                   LZW_Decode                         (XBYTE* inbuffer, XBYTE* outbuffer);

    XDWORD                 LZW_GIF_Encode                     (XBYTE* dibbuffer, XBYTE* outbuffer, XDWORD dibwidth, XDWORD dibheight, XWORD colorbit);
    void                   LZW_GIF_Decode                     (XBYTE* inbuffer, XBYTE* dibbuffer, XDWORD dibwidth, XDWORD dibheight, bool binterlace);

  private:

    void                   Decode_InitStringTable             ();
    XWORD                  Decode_GetNextcode                 ();
    bool                   Decode_IsInTable                   (XWORD code);
    void                   Decode_AddStringToTable            (XWORD prefix, XWORD suffix);
    XBYTE                  Decode_GetFirstChar                (XWORD code);
    void                   Decode_WriteString_to8             (XWORD code);
    void                   Decode_SwitchToFollowLine          ();
    void                   Encode_InitStringTable             ();
    XBYTE                  Encode_GetNextPixel                ();
    bool                   Encode_IsInTable                   (XWORD old, XWORD pixel);
    void                   Encode_AddStringToTable            (XWORD old, XWORD pixel);
    void                   Encode_WriteIndex                  (XDWORD windex);

    LZW_STRING*            strbegin;
    XWORD*                 hash;

    XWORD                  LZWclear;
    XWORD                  LZWend;

    XBYTE                  byinterval;
    XBYTE                  interval[7];

    XBYTE*                 origin;
    XBYTE*                 curin;
    XBYTE*                 curout;
    XBYTE                  byinbit;
    XBYTE                  byoutbit;

    XBYTE                  bymincode;
    XBYTE                  bycurrbits;
    XWORD                  currtableindex;


    XDWORD                 m_dibheight;
    XDWORD                 m_dibwidth;
    XDWORD                 currpixel;
    XDWORD                 currheight;
    XDWORD                 pitch;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





