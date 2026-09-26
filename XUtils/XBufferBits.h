/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBufferBits.h
* 
* @class      XBUFFERBITS
* @brief      eXtended Utils Reads a bit stream from a xbuffer
* @ingroup    XUTILS
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;

class XBUFFERBITS
{
  public:

                            XBUFFERBITS                     (XBUFFER* xbuffer);
    virtual                ~XBUFFERBITS                     ();

    bool                    IsByteAligned                   ();
    bool                    IsEOF                           ();
    bool                    IsOverrun                       ();

    XDWORD                  GetBytePosition                 ();
    XDWORD                  GetBitPosition                  ();
    void                    SetPosition                     (XDWORD index, XDWORD bitindex = 0);

    void                    SkipU1                          ();
    XDWORD                  PeekU1                          ();
    XDWORD                  ReadU1                          ();

    void                    SkipU                           (XDWORD n);
    XDWORD                  PeekU                           (XDWORD n);
    XDWORD                  ReadU                           (XDWORD n);

    XDWORD                  ReadUE                          ();
    int                     ReadSE                          ();

    void                    WriteU1                         (XDWORD v);
    void                    WriteU                          (XDWORD n, XDWORD v);

    void                    WriteUE                         (XDWORD v);
    void                    WriteSE                         (int v);

  protected:

    XBUFFER*                xbuffer;
    XBYTE*                  p;
    XDWORD                  bits_left;

  private:

    void                    Clean                           ();

  };



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





