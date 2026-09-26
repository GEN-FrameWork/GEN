/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashMD5.h
* 
* @class      HASHMD5
* @brief      Hash MD5 class
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

#define HASHMD5_BLOCKSIZE  64

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHMD5 : public HASH
{
  public:
                      HASHMD5               ();
    virtual          ~HASHMD5               ();

    using             HASH::Do;
    bool              Do                    (XBYTE* input, XQWORD size);

    int               GetDefaultSize        ();

  private:

    bool              finalized;
    XBYTE             buffer[HASHMD5_BLOCKSIZE];
    XDWORD            count[2];
    XDWORD            state[4];
    XBYTE             digest[16];

    void              Clean                 ();

    void              Ini                   ();

    void              Update                (XBYTE* input, XQWORD size);
    void              Transform             (XBYTE block[HASHMD5_BLOCKSIZE]);

    void              End                   ();

    void              Decode                (XDWORD* output, XBYTE* input, XDWORD size);
    void              Encode                (XBYTE* output, XDWORD* input, XDWORD size);

    inline XDWORD     F                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     G                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     H                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     I                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     RotateLeft            (XDWORD x, int n);
    inline void       FF                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       GG                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       HH                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       II                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





