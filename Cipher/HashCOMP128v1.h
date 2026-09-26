/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashCOMP128v1.h
* 
* @class      HASHCOMP128V1
* @brief      Hash COMP128 (version 1) class
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

#define HASHCOMP128V1_RAND_SIZE     16
#define HASHCOMP128V1_KI_SIZE       16

#define HASHCOMP128V1_SRES_SIZE     4
#define HASHCOMP128V1_KC_SIZE       8
#define HASHCOMP128V1_RESULT_SIZE   (HASHCOMP128V1_SRES_SIZE + HASHCOMP128V1_KC_SIZE)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHCOMP128V1 : public HASH
{
  public:
                          HASHCOMP128V1             ();
    virtual              ~HASHCOMP128V1             ();

    bool                  SetInput                  (XBYTE* rand, XBYTE* ki, XBUFFER& xbuffer);

    using                 HASH::Do;
    bool                  Do                        (XBYTE* input, XQWORD size);

    int                   GetDefaultSize            ();

    bool                  GetSRES                   (XBUFFER& SRES);
    bool                  GetKc                     (XBUFFER& Kc);

  private:


    void                  Clean                     ();

    void                  Make                      (XBYTE* input);

    static const XBYTE    table0[512];
    static const XBYTE    table1[256];
    static const XBYTE    table2[128];
    static const XBYTE    table3[64];
    static const XBYTE    table4[32];
    static const XBYTE*   table[5];

    XBYTE                 digest[HASHCOMP128V1_RESULT_SIZE];
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




