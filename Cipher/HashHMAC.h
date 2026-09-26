/**-------------------------------------------------------------------------------------------------------------------
*
* @file       HashHMAC.h
*
* @class      HASHHMAC
* @brief      Hash HMAC (Keyed-Hashing for Message Authentication, RFC 2104) class
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

#include "XBuffer.h"

#include "Hash.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define HASHHMAC_IPAD                       0x36
#define HASHHMAC_OPAD                       0x5C

#define HASHHMAC_BLOCKSIZE_16               16
#define HASHHMAC_BLOCKSIZE_64               64
#define HASHHMAC_BLOCKSIZE_128              128

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHHMAC : public HASH
{
  public:
                                    HASHHMAC                              (HASH* hash = NULL);
    virtual                        ~HASHHMAC                              ();

    HASH*                           GetHash                               ();
    bool                            SetHash                               (HASH* hash);

    XDWORD                          GetBlockSize                          ();

    XBUFFER*                        GetKey                                ();
    bool                            SetKey                                (XBYTE* key, XDWORD size);
    bool                            SetKey                                (XBUFFER& key);

    bool                            Do                                    (XBYTE* input, XQWORD size);
    bool                            Do                                    (XBUFFER& input);

    int                             GetDefaultSize                        ();

  private:

    bool                            NormalizeKey                          (XBUFFER& normalizedkey);
    XDWORD                          CalculateBlockSize                    (HASH* hash);

    void                            Clean                                 ();

    HASH*                           hash;
    XDWORD                          blocksize;
    XSECUREBUFFER                   key;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


