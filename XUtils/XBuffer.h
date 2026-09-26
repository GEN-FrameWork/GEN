/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBuffer.h
* 
* @class      XBUFFER
* @brief      eXtended Utils Buffer functions class
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
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XBUFFER_PADDINGTYPE
{
  XBUFFER_PADDINGTYPE_NONE                 = 0 ,
  XBUFFER_PADDINGTYPE_ZEROS                    ,
  XBUFFER_PADDINGTYPE_SPACES                   ,
  XBUFFER_PADDINGTYPE_ANSIX923                 ,
  XBUFFER_PADDINGTYPE_ISO10126                 ,
  XBUFFER_PADDINGTYPE_PKCS7                    ,
  XBUFFER_PADDINGTYPE_PKCS5                    ,
  XBUFFER_PADDINGTYPE_ISOIEC7816_4             ,
};

#define XBUFFER_INVALIDPOSITION             -1
#define XBUFFER_ISTHREADSAFE                true

#define XBUFFER_MASTER_NBITS                -1

#ifdef MICROCONTROLLER
#define XBUFFER_ADDBLOCKMEMSIZE_DEFAULT      8
#else
#define XBUFFER_ADDBLOCKMEMSIZE_DEFAULT    128
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;

class GEN_API_LIB_EXP XBUFFER
{
  public:
                             XBUFFER                              (bool threadsafe = XBUFFER_ISTHREADSAFE);
                             XBUFFER                              (XDWORD size, bool threadsafe = XBUFFER_ISTHREADSAFE);
                             XBUFFER                              (const XBUFFER& xbuffer);

    virtual                 ~XBUFFER                              ();

    XDWORD                   GetSize                              ();
    bool                     SetSize                              (XDWORD size);

    bool                     SetAddBlockMemSize                   (XDWORD addblockMemSize);

    bool                     IsEmpty                              ();

    void                     ResetPosition                        ();
    XDWORD                   GetPosition                          ();
    bool                     SetPosition                          (XDWORD position = 0);

    bool                     IsBlocked                            ();
    bool                     SetBlocked                           (bool blocked);

    bool                     Add                                  (XBYTE* pbuffer, XDWORD psize);
    bool                     Add                                  (XBUFFER* buffer);
    bool                     Add                                  (XBUFFER& buffer);
    bool                     Add                                  (XBYTE data);
    bool                     Add                                  (bool data);
    bool                     Add                                  (XWORD data);
    bool                     Add                                  (XDWORD data);
    bool                     Add                                  (XQWORD data);
    bool                     Add                                  (float data);
    bool                     Add                                  (double data);
    bool                     Add                                  (XSTRING& string, bool normalize = false);
    bool                     AddWithMaskArg                       (XCHAR* mask, va_list& arg);
    bool                     AddWithMaskArg                       (XSTRING* mask, va_list& arg);
    bool                     AddWithMask                          (XCHAR* mask, ...);
    bool                     AddWithMask                          (XSTRING* mask, ...);

    bool                     Insert                               (XBYTE* pbuffer, XDWORD psize, int frompos = 0);
    bool                     Insert                               (XBUFFER* buffer, int frompos = 0);
    bool                     Insert                               (XBUFFER& buffer, int frompos = 0);
    bool                     Insert                               (XBYTE data, int frompos = 0);
    bool                     Insert                               (bool data, int frompos = 0);
    bool                     Insert                               (XWORD data, int frompos = 0);
    bool                     Insert                               (XDWORD data, int frompos = 0);
    bool                     Insert                               (XQWORD data, int frompos = 0);
    bool                     Insert                               (float data, int frompos = 0);
    bool                     Insert                               (double data, int frompos = 0);
    bool                     Insert                               (XSTRING& string, int frompos = 0);
    bool                     InsertWithMaskArg                    (XCHAR* mask, int frompos, va_list& arg);
    bool                     InsertWithMaskArg                    (XSTRING* mask, int frompos, va_list& arg);
    bool                     InsertWithMask                       (XCHAR* mask, int frompos, ...);
    bool                     InsertWithMask                       (XSTRING* mask, int frompos, ...);

    XDWORD                   Extract                              (XBYTE* pbuffer, XDWORD ppos, XDWORD psize);
    bool                     Extract                              (XBYTE& data, XDWORD ppos = 0);
    bool                     Extract                              (bool& data, XDWORD ppos = 0);
    bool                     Extract                              (XWORD& data, XDWORD ppos = 0);
    bool                     Extract                              (XDWORD& data, XDWORD ppos = 0);
    bool                     Extract                              (XQWORD& data, XDWORD ppos = 0);
    bool                     Extract                              (float& data, XDWORD ppos = 0);
    bool                     Extract                              (double& data, XDWORD ppos = 0);
    bool                     Extract                              (XSTRING& string, XDWORD ppos, XDWORD psize);
    bool                     ExtractWithMaskArg                   (XCHAR* mask, int frompos, va_list& arg);
    bool                     ExtractWithMaskArg                   (XSTRING* mask, int frompos, va_list& arg);
    bool                     ExtractWithMask                      (XCHAR* mask, int frompos, ...);
    bool                     ExtractWithMask                      (XSTRING* mask, int frompos, ...);

    XBYTE*                   Get                                  ();
    XBYTE                    GetByte                              (XDWORD index);
    XBYTE                    GetLastByte                          ();

    char*                    GetPtrChar                           ();
    XWORD*                   GetPtrWord                           ();

    bool                     Get                                  (XBYTE* pbuffer, int psize, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (XBYTE& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (bool& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (XWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (XDWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (XQWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (double& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (float& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                                  (XSTRING& data, int psize, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     GetWithMaskArg                       (XCHAR* mask, int frompos, va_list& arg);
    bool                     GetWithMaskArg                       (XSTRING* mask, int frompos, va_list& arg);
    bool                     GetWithMask                          (XCHAR* mask, int frompos, ...);
    bool                     GetWithMask                          (XSTRING* mask, int frompos, ...);

    bool                     Set                                  (XBYTE* pbuffer, int psize, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (XBYTE data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (bool data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (XWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (XDWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (XQWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (float data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (double data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                                  (XSTRING& data, int topos = XBUFFER_INVALIDPOSITION);
    XDWORD                   SetWithMaskArg                       (XCHAR* mask, int topos, va_list& arg);
    XDWORD                   SetWithMaskArg                       (XSTRING* mask, int topos, va_list& arg);
    XDWORD                   SetWithMask                          (XCHAR* mask, int topos, ...);
    XDWORD                   SetWithMask                          (XSTRING* mask, int topos, ...);

    bool                     Resize                               (XDWORD newsize, bool setblocked = true);
    bool                     Delete                               (bool setblocked = true);
    bool                     SecureDelete                         (bool setblocked = true);
    bool                     DeleteByte                           (XBYTE data, bool setblocked = true); 
    bool                     FillBuffer                           (XBYTE fillchar = 0);
    bool                     Empty                                ();
    bool                     Swap                                 ();
    
    bool                     Compare                              (XBYTE* pbuffer, XDWORD psize);
    bool                     Compare                              (XBUFFER* buffer);
    bool                     Compare                              (XBUFFER& buffer);

    int                      Find                                 (XBUFFER* buffer, int startindex = 0);
    int                      Find                                 (XSTRING& string, bool normalize = false, int startindex = 0);

    void                     operator  =                          (const XBUFFER& string);
    bool                     operator  ==                         (XBUFFER buffer);
    bool                     operator  !=                         (XBUFFER buffer);

    bool                     CopyFrom                             (XBUFFER& buffer);
    bool                     CopyTo                               (XBUFFER& buffer);   

    bool                     Padding_Add                          (XBYTE bitsadjust, XBUFFER_PADDINGTYPE type = XBUFFER_PADDINGTYPE_PKCS5);
    bool                     Padding_Has                          ();
    XBUFFER_PADDINGTYPE      Padding_GetType                      ();
    XBYTE                    Padding_GetSize                      ();
    bool                     Padding_Delete                       ();

    XBYTE                    Bit_GetNBits                         ();
    bool                     Bit_SetNBits                         (int nbits);
    bool                     Bit_AddData                          (XDWORD data, int nbits = XBUFFER_MASTER_NBITS);
    XDWORD                   Bit_GetData                          (int indexbit, int nbits = XBUFFER_MASTER_NBITS);
    bool                     Bit_SetData                          (XDWORD data, int indexbit, int nbits = XBUFFER_MASTER_NBITS);
    XBYTE                    Bit_GetBitsFree                      ();
    bool                     Bit_SetBitsFree                      (XBYTE nbitsfree);

    bool                     ConvertFromBase64                    (XSTRING& in_stringbase64);
    bool                     ConvertToBase64                      (XSTRING& out_stringbase64);

    XDWORD                   DecodeBCD                            (XDWORD ppos, XDWORD psize);
    XQWORD                   DecodeBCDLong                        (XDWORD ppos, XDWORD psize);

    bool                     GetGlobalHardwareUseLittleEndian     ();
    static void              SetGlobalHardwareUseLittleEndian     (bool globalhardwareuselittleendian);

    bool                     GetLocalHardwareUseLittleEndian      ();
    void                     SetLocalHardwareUseLittleEndian      (bool globalhardwareuselittleendian);
    
  private:

    void                     Clean                                ();

    bool                     AddXBufferWithMask                   (XBUFFER& xbuffer, XCHAR* mask, va_list& arg);
    bool                     AddXBufferWithMask                   (XBUFFER& xbuffer, XSTRING* mask, va_list& arg);

    XDWORD                   CalculeExtractSizeArgWithMask        (XBUFFER& xbuffer, int frompos, XCHAR* mask, va_list& arg);

    bool                     ExtractXBufferWithMask               (XBUFFER& xbuffer, XCHAR* mask, va_list& arg);
    bool                     ExtractXBufferWithMask               (XBUFFER& xbuffer, XSTRING* mask, va_list& arg);
    
    XBYTE*                   buffer;
    XDWORD                   size;
    XDWORD                   sizeassign;
    XDWORD                   position;

    XDWORD                   addblockmemsize;

    XBYTE                    nbits;
    XBYTE                    nbitsfree;

    bool                     paddinghas;
    XBUFFER_PADDINGTYPE      paddingtype;
    XBYTE                    paddingbitadjust;
    XBYTE                    paddingsize;

    XMUTEX*                  xmutex;

    static bool              globalhardwareuselittleendian;
    bool                     localhardwareuselittleendian;
};


class GEN_API_LIB_EXP XSECUREBUFFER : public XBUFFER
{
  public:
                             XSECUREBUFFER                        (bool threadsafe = XBUFFER_ISTHREADSAFE) : XBUFFER(threadsafe) {}
                             XSECUREBUFFER                        (XDWORD size, bool threadsafe = XBUFFER_ISTHREADSAFE) : XBUFFER(size, threadsafe) {}
                             XSECUREBUFFER                        (const XBUFFER& xbuffer) : XBUFFER(xbuffer) {}
    virtual                 ~XSECUREBUFFER                        () { SecureDelete(); }
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




