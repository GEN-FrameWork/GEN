/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XString.h
* 
* @class      XSTRING
* @brief      eXtended Utils Unicode String class
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>

#include "XBase.h"
#include "XVector.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef wchar_t XCHAR;

#define XSTRING_NOTFOUND     NOTFOUND

enum XSTRINGCODING
{
  XSTRINGCODING_UNKWOWN              = 0 ,
  XSTRINGCODING_ASCII                    ,
  XSTRINGCODING_UTF8                     ,
  XSTRINGCODING_UTF16                    ,
  XSTRINGCODING_UTF32                    ,
};                                   
                                     
enum XSTRINGCONTEXT                  
{                                    
  XSTRINGCONTEXT_FROM_FIRST          = 0 ,
  XSTRINGCONTEXT_TO_END                  ,
  XSTRINGCONTEXT_ALLSTRING               ,
};                                   

enum XSTRINGASCIICODE
{  
  XSTRINGASCIICODE_NONE              = 0 ,
  XSTRINGASCIICODE_7BIT                  ,
  XSTRINGASCIICODE_CODEPAGE_437          ,
  XSTRINGASCIICODE_CODEPAGE_850          ,     
  XSTRINGASCIICODE_CODEPAGE_852          ,     
  XSTRINGASCIICODE_CODEPAGE_866          ,     
  XSTRINGASCIICODE_ISO_8859_1            ,
  XSTRINGASCIICODE_ISO_8859_2            ,
  XSTRINGASCIICODE_ISO_8859_3            ,
  XSTRINGASCIICODE_ISO_8859_4            ,
  XSTRINGASCIICODE_WINDOWS_1250          ,
  XSTRINGASCIICODE_WINDOWS_1251          ,
  XSTRINGASCIICODE_WINDOWS_1252          ,  
  XSTRINGASCIICODE_WINDOWS_1253          ,
  XSTRINGASCIICODE_WINDOWS_1254          ,  
};


#define XSTRINGBOOLEANMODE_COMPUTER    0x01
#define XSTRINGBOOLEANMODE_HUMAN       0x02
#define XSTRINGBOOLEANMODE_UPPERCASE   0x80
#define XSTRINGBOOLEANMODE_LOWERCASE   0x40

typedef struct 
{
  XDWORD             character;
  XBYTE              code7bit;  
  XBYTE              codepage437;  
  XBYTE              codepage850;  
  XBYTE              codepage852;  
  XBYTE              codepage866;  
  XBYTE              iso88591;
  XBYTE              iso88592;
  XBYTE              iso88593;
  XBYTE              iso88594;
  XBYTE              windows1250;
  XBYTE              windows1251;
  XBYTE              windows1252;
  XBYTE              windows1253;
  XBYTE              windows1254;

} ASCCICODEENTRY;
           

#pragma            pack(push, r1, 1)
typedef struct
{
  XBYTE size;
  char  data[255];

} SHORTSTRING;
#pragma pack(pop, r1)

#define XSTRING_EXCESSCHARS             1

#ifdef MICROCONTROLLER
  #define XSTRING_MAXTEMPOSTR           128
#else
  #define XSTRING_MAXTEMPOSTR           1024
#endif

//#define XSTRING_TABLE64BITSSIZE         64
#define XSTRING_TABLE64BITS             _L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")

#define XSTRING_VALIDCHARSINT           _L("0123456789-+");
#define XSTRING_VALIDCHARSFLOAT         _L("0123456789-+.eE");

#define NL(x)                           (XCHAR*)(L##x)
#define FL(x)                           (XCHAR*)(L##x)
#define _L(x)                           (XCHAR*)(L##x)
#define _C(x)                           (XCHAR)(L##x)


#if (defined(LINUX) || defined(ANDROID))

  #define SPRINTF(str, ...)       sprintf(str, ## __VA_ARGS__)
  #define SSCANF(str, ...)        sscanf(str, ## __VA_ARGS__)

#else

  #if (defined(WINDOWS) || defined(_WINDOWS))

    #if defined(BUILDER)

      #define SPRINTF             sprintf
      #define SSCANF              sscanf

    #else

      #define SPRINTF(str, ...)   sprintf_s(str, XSTRING_MAXTEMPOSTR-1, ## __VA_ARGS__)
      #define SSCANF(str, ...)    sscanf_s(str, ## __VA_ARGS__)

    #endif

  #endif

#endif

#if defined(MICROCONTROLLER)

  #define SPRINTF(str, ...)       sprintf(str, ## __VA_ARGS__)
  #define SSCANF(str, ...)        sscanf(str, ## __VA_ARGS__)

#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;
class XMUTEX;

class GEN_API_LIB_EXP XSTRING
{
  public:
                          XSTRING                         ();
                          XSTRING                         (XDWORD size);
                          XSTRING                         (const char* string);
                          XSTRING                         (const XCHAR* string);
                          XSTRING                         (const XCHAR* string, XDWORD size);
                          XSTRING                         (const XSTRING& string);
                          XSTRING                         (XWORD* string);

    virtual              ~XSTRING                         ();

    XCHAR*                Get                             () const;
    XDWORD                GetSize                         () const;
    static XDWORD         GetSize                         (const XCHAR* string);
    static XDWORD         GetSize                         (XWORD* string);

    XSTRINGCODING         GetTypeCoding                   ();
    
    bool                  Set                             (XDWORD size);
    bool                  Set                             (const char* string);
    bool                  Set                             (const XCHAR* string);
    bool                  Set                             (const XCHAR* string, XDWORD size);
    bool                  Set                             (const XSTRING& string);
    bool                  Set                             (XWORD* string);
    bool                  Set                             (const XCHAR character);
    static bool           Set                             (XCHAR* str1, const XCHAR* str2);
    bool                  Set                             (const XBYTE* buffer, XDWORD size);
    bool                  Set                             (XBUFFER& buffer);
    bool                  Set                             (const XDWORD* buffer, XDWORD size);

    bool                  Add                             (const char* string);
    bool                  Add                             (const XCHAR* string);
    bool                  Add                             (XSTRING& string);
    bool                  Add                             (XCHAR character);
    static bool           Add                             (XCHAR* str1, const XCHAR* str2);
    bool                  Add                             (const XBYTE* buffer, XDWORD size);
    bool                  Add                             (XBUFFER& buffer);

    void                  operator =                      (const char* string);
    void                  operator =                      (const XCHAR* string);
    void                  operator =                      (const XSTRING& string);
    void                  operator =                      (XWORD* string);
    void                  operator =                      (XBUFFER& string);

    void                  operator +=                     (const char* string);
    void                  operator +=                     (const XCHAR* string);
    void                  operator +=                     (XSTRING& string);
    void                  operator +=                     (XCHAR character);

    bool                  operator <                      (const XSTRING& string) const;
    bool                  operator >                      (XSTRING& string);
    bool                  operator <=                     (XSTRING& string);
    bool                  operator >=                     (XSTRING& string);
    bool                  operator ==                     (const XSTRING& string) const;
    bool                  operator !=                     (XSTRING& string);

    XCHAR                 operator []                     (int position);

    bool                  FillChar                        (XCHAR character = 0);
    bool                  Empty                           ();
    bool                  IsEmpty                         ();

    bool                  HaveCharacters                  ();
    bool                  HaveOnlyNumbers                 ();
    bool                  HaveNumbers                     ();

    bool                  DeleteCharacters                (XDWORD index, XDWORD ncharacteres);
    bool                  DeleteCharactersToEnd           (XDWORD index);
    bool                  DeleteCharacter                 (XCHAR character, XSTRINGCONTEXT context = XSTRINGCONTEXT_ALLSTRING);
    bool                  DeleteNoCharacters              (XSTRINGCONTEXT context);
    int                   DeleteNoCharacters              (XCHAR* needle=_L("\t\r\n"), int start=0, XSTRINGCONTEXT context= XSTRINGCONTEXT_ALLSTRING);
    bool                  DeleteLastCharacter             ();
    bool                  DeleteLastZeros                 ();

    bool                  Character_IsAlpha               (XCHAR character) const;
    bool                  Character_IsUpperCase           (XCHAR character);
    bool                  Character_IsLowerCase           (XCHAR character);
    bool                  Character_IsNumber              (XCHAR character, bool isextended = true);
    bool                  Character_IsNOT                 (XCHAR character);
    XCHAR                 Character_ToUpper               (XCHAR character) const;
    XCHAR                 Character_ToLower               (XCHAR character) const;
    XCHAR                 Character_GetFirst              ();
    XCHAR                 Character_GetLast               ();
    bool                  Character_Change                (XCHAR source, XCHAR target);

    bool                  IsNumber                        ();               
    bool                  IsDecimalNumber                 (int* ndecimals = NULL);               

    bool                  ToUpperCase                     ();
    bool                  ToLowerCase                     ();

    int                   Compare                         (const XCHAR* string, int csize, bool ignorecase = false);
    int                   Compare                         (const XCHAR* string, bool ignorecase = false);   
    int                   Compare                         (const XSTRING& string, bool ignorecase = false) const;  
    static int            Compare                         (const XCHAR* string, const XCHAR* string2, bool ignorecase = false);
    int                   Compare                         (const char* string, bool ignorecase = false);

    int                   Find                            (const XCHAR* string, bool ignorecase, int startindex = 0);
    int                   Find                            (XSTRING& string, bool ignorecase, int startindex = 0);
    int                   FindCharacter                   (XCHAR character, XDWORD startindex = 0, bool endtostart = false);
    int                   FindDiferentCharacter           (XCHAR character, int startindex = 0, bool endtostart = false);
    XDWORD                CountCharacter                  (XCHAR character, int startindex = 0, bool endtostart = false);

    int                   GetNCharacters                  (XCHAR character);

    bool                  AreValidCharacters              (XCHAR* validcharacterslist);

    int                   Copy                            (const XCHAR* startmark, const XCHAR* endmark, bool ignorecase , int addstartindex, XSTRING& string);
    int                   Copy                            (int startindex, const XCHAR* endmark, bool ignorecase, XSTRING& string);
    int                   Copy                            (const XCHAR* startmark, int endindex, bool ignorecase, int startindex, XSTRING& string);
    int                   Copy                            (int startindex, int endindex, XSTRING& string);
    int                   Copy                            (int startindex, XSTRING& string);

    int                   Replace                         (XCHAR* findWhat, XCHAR* replaceBy);
    int                   ReplaceFirst                    (XCHAR* findWhat, XCHAR* replaceBy);
    bool                  Insert                          (XCHAR* str, XDWORD position = 0);
    bool                  Insert                          (XSTRING& str, XDWORD position = 0);

    bool                  ConvertFromNULL                 (bool uppercase);
    bool                  ConvertFromBoolean              (bool boolean, XBYTE mode = XSTRINGBOOLEANMODE_COMPUTER);
    bool                  ConvertFromShort                (short value, const XCHAR* mask = NULL);
    bool                  ConvertFromWord                 (XWORD value, const XCHAR* mask = NULL);
    bool                  ConvertFromInt                  (int value, const XCHAR* mask = NULL);
    bool                  ConvertFromDWord                (XDWORD value, const XCHAR* mask = NULL);
    bool                  ConvertFromLongLong             (long long value, const XCHAR* mask = NULL);
    bool                  ConvertFromQWord                (XQWORD value, const XCHAR* mask = NULL);    
    bool                  ConvertFromFloat                (float value, const XCHAR* mask = NULL);
    bool                  ConvertFromDouble               (double value, const XCHAR* mask = NULL);
        
    bool                  ConvertHexFormatChars           ();
    bool                  ConvertToHexString              (XSTRING& string, bool uppercase = true);
    bool                  ConvertHexStringToBuffer        (XBUFFER& xbuffer);
    bool                  ConvertHexStringFromBuffer      (XBUFFER& xbuffer, bool upercase = true);
    
    bool                  ConvertToBoolean                ();
    int                   ConvertToInt                    (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
    XDWORD                ConvertToDWord                  (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
    long long             ConvertToLongLong               (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
    XQWORD                ConvertToQWord                  (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
    float                 ConvertToFloat                  (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
    double                ConvertToDouble                 (int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
        
    bool                  IsValidASCII                    ();
    bool                  ConvertCharacterUnicodeToASCII  (XCHAR character, XBYTE& asciicharacter, XSTRINGASCIICODE asccicode = XSTRINGASCIICODE_7BIT);
    bool                  ConvertASCIICharacterToUnicode  (XBYTE asciicharacter, XCHAR& character, XSTRINGASCIICODE asccicode = XSTRINGASCIICODE_7BIT);
    bool                  ConvertToASCII                  (XBUFFER& xbuffer, XSTRINGASCIICODE asccicode = XSTRINGASCIICODE_7BIT);
    bool                  ConvertFromASCII                (XBUFFER& xbuffer, XSTRINGASCIICODE asccicode = XSTRINGASCIICODE_7BIT);
    XSTRINGASCIICODE      ConsoleCodePageToConvertASCII   (XDWORD symbolused);
    
    bool                  ConvertToXBuffer                (XBUFFER& xbuffer);
    bool                  ConvertFromXBuffer              (XBUFFER& xbuffer, XSTRINGCODING buffercoding);

    bool                  ConvertToExchangeXBuffer        (XBUFFER& xbuffer, bool addzeroatend = true, bool inverse = false);

    int                   GetSizeConvertToUTF8            ();
    bool                  ConvertFromUTF8                 (XBYTE* data, XDWORD size);
    bool                  ConvertFromUTF8                 (XBUFFER& xbuffer);    
    bool                  ConvertToUTF8                   (XBYTE* data, int& size, bool addzeroatend = true);
    bool                  ConvertToUTF8                   (XBUFFER& xbuffer, bool addzeroatend = true);

    bool                  ConvertFromUTF16                (XBUFFER& xbuffer);    
    bool                  ConvertToUTF16                  (XBUFFER& xbuffer);    

    bool                  ConvertFromUTF32                (XBUFFER& xbuffer);
    bool                  ConvertToUTF32                  (XBUFFER& xbuffer);
       
    bool                  ConvertToBase64                 (XSTRING& string);
    bool                  ConvertBinaryToBase64           (XBUFFER& inbuffer);
    bool                  ConvertBase64ToBinary           (XBUFFER& outbuffer);

    bool                  ConvertToPascal                 (SHORTSTRING& pascalstring);
    bool                  ConvertFromPascal               (SHORTSTRING& pascalstring);

    bool                  ConvertFromDoubleToSpanishText  (double value, bool withintegerpart, bool withdecimalpart, double& integerpart, double& decimalpart);

    bool                  Swab                            ();

    bool                  Split                           (XCHAR separator, XVECTOR<XSTRING*>& results, bool addsubstringempty = false);

    bool                  FormatArg                       (const XCHAR* mask, va_list* arg, bool isspecialweb = false);
    bool                  Format                          (const XCHAR* mask, ...);
    bool                  AddFormat                       (const XCHAR* mask, ...);
    bool                  UnFormat                        (const XCHAR* mask, ...);

    int                   FindCharacterFromSet            (const XCHAR* set);

    bool                  AdjustSize                      (XDWORD size, bool ahead = false, XCHAR* characters = NULL, bool addstring = false);
    bool                  AdjustSize                      ();

    bool                  BinaryCompare                   (XSTRING& string);

    bool                  Explode                         (XCHAR character, XVECTOR<XSTRING*>* receive);

    bool                  GetTypeOfLineEnd                (XSTRING& lineend);
    
  protected: 
    
    bool                  ReAllocBuffer                   (XDWORD sizechar);
    bool                  FreeBuffer                      ();

    bool                  ConvertStringWithMask           (XCHAR* mask, XCHAR* string, XCHAR* result);

    void                  Base64_EncodeBlock              (XBYTE* in, XBYTE* out, int len);
    bool                  Base64_Encode                   (XBYTE* buffer, XDWORD buffersize, XSTRING& line);

    void                  Base64_DecodeBlock              (XBYTE in[4], XBYTE out[3]);
    bool                  Base64_Decode                   (const char* in_buffer, int in_buffer_size, XBUFFER& outbuffer);

    XCHAR*                text;
    XDWORD                size;
    XDWORD                sizemem;

    XMUTEX*               xmutexfreemen;

    short*                value_short;
    XWORD*                value_word;
    int*                  value_int;
    XDWORD*               value_dword;
    long long*            value_longlong;
    XQWORD*               value_qword;
    float*                value_float;
    double*               value_double;
    XCHAR*                value_xchar;

    static const char     codetablebase64[];
    static const char     decodetablebase64[];

  private:

    void                  Clean                           ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





