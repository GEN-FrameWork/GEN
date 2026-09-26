/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashWhirlpool.h
* 
* @class      HASHWHIRLPOOL
* @brief      Hash Whirlpool class
* @ingroup    CIPHER
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

#include "Hash.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define HASHWHIRLPOOL_DIGESTBYTES             64
#define HASHWHIRLPOOL_DIGESTBITS              (8*HASHWHIRLPOOL_DIGESTBYTES) /* 512 */

#define HASHWHIRLPOOL_WBLOCKBYTES             64
#define HASHWHIRLPOOL_WBLOCKBITS              (8*HASHWHIRLPOOL_WBLOCKBYTES) /* 512 */

#define HASHWHIRLPOOL_LENGTHBYTES             32
#define HASHWHIRLPOOL_LENGTHBITS              (8*HASHWHIRLPOOL_LENGTHBYTES) /* 256 */

#define HASHWHIRLPOOL_ROUNDS                  10

typedef struct
{
  XBYTE   bitlength[HASHWHIRLPOOL_LENGTHBYTES];
  XBYTE   buffer[HASHWHIRLPOOL_WBLOCKBYTES];
  int     bufferbits;
  int     bufferpos;
  XQWORD  hash[HASHWHIRLPOOL_DIGESTBYTES/8];

} WHIRLPOOL;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHWHIRLPOOL : public HASH
{
  public:
                        HASHWHIRLPOOL         ();
    virtual            ~HASHWHIRLPOOL         ();

    using               HASH::Do;
    bool                Do                    (XBYTE* input, XQWORD size);

    int                 GetDefaultSize        ();

  private:

    static XQWORD       C0[256];
    static XQWORD       C1[256];
    static XQWORD       C2[256];
    static XQWORD       C3[256];
    static XQWORD       C4[256];
    static XQWORD       C5[256];
    static XQWORD       C6[256];
    static XQWORD       C7[256];
    static XQWORD       rc[HASHWHIRLPOOL_ROUNDS + 1];
  
    void                ProcessBuffer         (WHIRLPOOL* structpointer);

    void                Ini                   (WHIRLPOOL* structpointer);
    void                Add                   (XBYTE* source, XDWORD sourceBits, WHIRLPOOL* structpointer);
    void                End                   (WHIRLPOOL* structpointer, XBYTE* result);

    void                Clean                 ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





