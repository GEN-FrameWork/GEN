/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Hash.h
* 
* @class      HASH
* @brief      Hash interface class
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

#include "XFactory.h"
#include "XDateTime.h"
#include "XPath.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define HASHMAXFILESIZEBUFFER (1024*512)
#define HASHALLFILESIZE       0x80000000

enum HASHTYPE
{
  HASHTYPE_NONE     = 0 ,
  HASHTYPE_CKS16        ,
  HASHTYPE_CRC16        ,
  HASHTYPE_CRC32        ,
  HASHTYPE_MD2          ,
  HASHTYPE_MD4          ,
  HASHTYPE_MD5          ,
  HASHTYPE_SHA1         ,
  HASHTYPE_SHA224       ,
  HASHTYPE_SHA256       ,
  HASHTYPE_SHA384       ,
  HASHTYPE_SHA512       ,
  HASHTYPE_RIPEMD160    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XFILE;

class HASH
{
  public:
                                  HASH                          ();
    virtual                      ~HASH                          ();

    HASHTYPE                      GetType                       ();
    XCHAR*                        GetTypeStr                    ();

    XSTRING*                      GetOUINoSign                  ();

    virtual bool                  Do                            (XBYTE* input, XQWORD size);
    bool                          Do                            (XBUFFER& input);

    bool                          Do                            (XPATH& xpath, XQWORD size = HASHALLFILESIZE, XQWORD pos = 0);
    bool                          Do                            (XFILE* xfile, XQWORD size = HASHALLFILESIZE, XQWORD pos = 0);

    virtual bool                  ResetResult                   ();

    virtual int                   GetDefaultSize                ();

    virtual XBUFFER*              GetResult                     ();
    virtual XBYTE*                GetResult                     (XDWORD& resultsize);

    bool                          GetResultString               (XSTRING& stringhex);

  protected:

    HASHTYPE                      type;
    XSTRING                       OUInosignstring;
    XBUFFER*                      result;

  private:

    void                          Clean                         ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





