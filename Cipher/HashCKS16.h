/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashCKS16.h
* 
* @class      HASHCKS16
* @brief      Hash CKS16 class
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

class HASHCKS16 : public HASH
{
  public:

                        HASHCKS16               ();
    virtual            ~HASHCKS16               ();

    using               HASH::Do;
    bool                Do                      (XBYTE* input, XQWORD size);

    bool                ResetResult             ();

    int                 GetDefaultSize          ();

    XBUFFER*            GetResult               ();
    XBYTE*              GetResult               (XDWORD& resultsize);

    XWORD               GetResultCKS16          ();

  private:
   

    void                Ini                     (void);
    XWORD               Update                  (XWORD CKS16, XWORD c); 

    void                Clean                   ();
    
    XBUFFER*            resulttmp;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





