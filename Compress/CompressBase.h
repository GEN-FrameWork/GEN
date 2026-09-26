/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CompressBase.h
* 
* @class      COMPRESSBASE
* @brief      Compress Base class
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

#include "XBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum COMPRESSBASE_TYPE
{
  COMPRESSBASE_TYPE_NONE      = 0 ,
  COMPRESSBASE_TYPE_LZW           ,
  COMPRESSBASE_TYPE_LZRW1KH       ,
  COMPRESSBASE_TYPE_ZIP           ,
  COMPRESSBASE_TYPE_GZ            ,
  COMPRESSBASE_TYPE_DEFLATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;

class COMPRESSBASE
{
  public:
                          COMPRESSBASE          ();
                          COMPRESSBASE          (COMPRESSBASE_TYPE type);
    virtual              ~COMPRESSBASE          ();

    COMPRESSBASE_TYPE     GetType               ();
    COMPRESSBASE_TYPE     SetType               (COMPRESSBASE_TYPE type = COMPRESSBASE_TYPE_NONE);

    virtual bool          Compress              (XBYTE* origin, XDWORD size, XBUFFER* buffer);
    virtual bool          Decompress            (XBYTE* origin, XDWORD size, XBUFFER* buffer);

    virtual bool          Compress              (XBYTE* origin, XDWORD size, void* param1 = NULL);
    virtual bool          Decompress            (XBYTE* origin, XDWORD size, void* param1 = NULL);

    virtual bool          CompressResult        (bool compress, XBYTE data, void* param1 = NULL);

  protected:

    void*                 param1;

  private:

    COMPRESSBASE_TYPE     type;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





