/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOIP.h
* 
* @class      DIOIP
* @brief      Data Input/Output IP class
* @ingroup    DATAIO
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

#include "XBase.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define  DIOIP_MAXIPADDRESS                4

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOURL;
class DIOIP;

class DIOIPADDRESS
{
  public:

                                  DIOIPADDRESS                        ();
    virtual                      ~DIOIPADDRESS                        ();

    XBYTE*                        Get                                 ();
    bool                          GetXString                          (XSTRING& addressstring);
    bool                          GetDIOURL                           (DIOURL* url);

    bool                          IsEmpty                             ();
    bool                          IsLocal                             ();
    bool                          IsAPIPA                             ();

    bool                          Set                                 (XBYTE* address);
    bool                          Set                                 (XBYTE byte1, XBYTE byte2, XBYTE byte3, XBYTE byte4);
    bool                          Set                                 (XCHAR* addresschar);
    bool                          Set                                 (XSTRING& addressstring);
    bool                          Set                                 (char* addresschar);

    bool                          Compare                             (DIOIP& address);

  protected:

    XBYTE                         address[DIOIP_MAXIPADDRESS];

  private:

    void                          Clean                               ();
};



class DIOIP : public DIOIPADDRESS
{
  public:

                                  DIOIP                               ();
    virtual                      ~DIOIP                               ();

    DIOIPADDRESS*                 GetMask                             ();

  protected:

    DIOIPADDRESS                  mask;

  private:

    void                          Clean                               ();
};





/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





