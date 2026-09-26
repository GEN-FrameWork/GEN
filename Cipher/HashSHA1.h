/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashSHA1.h
* 
* @class      HASHSHA1
* @brief      Hash SHA1 class
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

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHSHA1 : public HASH
{
  public:
                      HASHSHA1                  ();
    virtual          ~HASHSHA1                  ();

    using             HASH::Do;
    bool              Do                        (XBYTE* input, XQWORD size);

    int               GetDefaultSize            ();

  private:
   
    void              Reset                     ();

    void              Input                     (XBYTE* input, XQWORD length);

    void              ProcessMessageBlock       ();
    void              PadMessage                ();
    XDWORD            CircularShift             (int bits, XDWORD word);

    bool              Result                    (XDWORD* msgdigest); 

    void              Clean                     (); 

     XDWORD           H[5];

     XDWORD           lengthlow;
     XDWORD           lengthhigh;

     XBYTE            msgblock[64];
     int              msgblockindex;

     bool             computed;
     bool             corrupted;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




