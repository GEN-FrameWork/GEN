/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileTXT.h
* 
* @class      XFILETXT
* @brief      eXtended Utils TXT (Text) file class
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
#include "XFile.h"
#include "XString.h"
#include "XPath.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILETXTFORMATCHAR
{
  XFILETXTFORMATCHAR_UNKNOWN             = 0 ,
  XFILETXTFORMATCHAR_ASCII                   ,
  XFILETXTFORMATCHAR_UTF8                    ,
  XFILETXTFORMATCHAR_UTF16_BE                ,
  XFILETXTFORMATCHAR_UTF16_LE
};


enum XFILETXTTYPELF
{
  XFILETXTTYPELF_UNKNOWN                 = 0 ,
  XFILETXTTYPELF_0A                          ,
  XFILETXTTYPELF_0D                          ,
  XFILETXTTYPELF_0A0D                        ,
  XFILETXTTYPELF_0D0A                        ,
};

#define XFILETXT_MAXBUFFER      (5000*1024)

#define XFILETXT_TOLASTLINE     0x80000000

#ifdef LINUX
#define XFILETXTTYPELF_DEFAULT  XFILETXTTYPELF_0A
#else
#define XFILETXTTYPELF_DEFAULT  XFILETXTTYPELF_0D0A
#endif

#define XFILETXT_ALLLINES       -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSTRING;
class XBUFFER;

class XFILETXT : public XFILECONTAINER
{
  public:
                          XFILETXT                ();
    virtual              ~XFILETXT                ();

    virtual bool          Open                    (XCHAR* xpath, bool readonly = true);
    virtual bool          Open                    (XPATH& xpath, bool readonly = true);
    virtual bool          Create                  (XPATH& xpath, XFILETXTFORMATCHAR formatchar = XFILETXTFORMATCHAR_ASCII, XFILETXTTYPELF typeLF = XFILETXTTYPELF_DEFAULT);
    virtual bool          IsOpen                  ();
    virtual bool          Close                   (void);

    XFILETXTFORMATCHAR    GetFormatChar           ();
    bool                  SetFormatChar           (XFILETXTFORMATCHAR formatchar);
    XFILETXTFORMATCHAR    GetFormatCharFromFile   (XDWORD* sizeBOM = NULL);
    bool                  CreateBOMFormatChar     (XFILETXTFORMATCHAR formatchar, XBYTE* BOM, XDWORD& sizeBOM);
    int                   SizeOfCharacter         (XFILETXTFORMATCHAR formatchar);
    XFILETXTTYPELF        GetTypeLF               ();
    bool                  SetTypeLF               (XFILETXTTYPELF typeLF);
    bool                  CreateTypeLF            (XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF, XBYTE* LF, XDWORD& sizeLF);
    bool                  GetLF                   (XBUFFER& lfdata);
    XSTRING*              GetLF                   ();
    XSTRING*              GetLF                   (XFILETXTTYPELF typeLF);

    bool                  IsBinaryFile            ();

    int                   GetNLines               ();
    XSTRING*              GetLine                 (int index);
    XCHAR*                GetLineText             (int index);

    bool                  GetAllInOneLine         (XSTRING& alllines, XFILETXTTYPELF typeLF = XFILETXTTYPELF_UNKNOWN, XDWORD start = 0, XDWORD end = XFILETXT_TOLASTLINE);
    bool                  GetAllInBuffer          (XBUFFER& xbuffer, XFILETXTTYPELF typeLF = XFILETXTTYPELF_UNKNOWN, XDWORD start = 0, XDWORD end = XFILETXT_TOLASTLINE);

    bool                  ReadAllFile             ();
    bool                  WriteAllFile            ();

    bool                  ReadNLines              (int nlines = XFILETXT_ALLLINES);

    bool                  DeleteFormatFile        ();

    bool                  AddLine                 (XCHAR* line);
    bool                  AddLine                 (XSTRING& line);
    bool                  AddLine                 (XBUFFER& line, XFILETXTFORMATCHAR formatchar = XFILETXTFORMATCHAR_ASCII);

    bool                  GenerateLineFromBuffer  (XFILETXTFORMATCHAR formatchar, XBYTE* line, XDWORD sizeline, XSTRING& string);
    bool                  AddLine                 (XFILETXTFORMATCHAR type, XBYTE* line, XDWORD sizeline);

    bool                  AddLineAlready          (XCHAR* line, XDWORD* sizeline = NULL, XDWORD* sizeLF = NULL);
    bool                  AddLineAlready          (XSTRING& line, XDWORD* sizeline = NULL, XDWORD* sizeLF = NULL);

    bool                  InsertLine              (int index, XCHAR* line);
    bool                  InsertLine              (int index, XSTRING& line);

    bool                  DeleteLine              (int index);
    bool                  DeleteAllLines          ();

    bool                  AddBufferLines          (XFILETXTFORMATCHAR formatchar, XBUFFER& xbuffer);

    bool                  GetSizeOfLine           (XFILETXTFORMATCHAR formatchar, XBYTE* buffer, XFILETXTTYPELF& typeLF, XDWORD& sizeLF, XDWORD& size, XDWORD maxsize);

    XVECTOR<XSTRING*>*    GetLines                ();

    bool                  CopyFrom                (XFILETXT* filetxt);
    bool                  CopyTo                  (XFILETXT* filetxt);

    XFILETXTFORMATCHAR    formatchar;
    XFILETXTTYPELF        typeLF;
    XSTRING               LF;
    XVECTOR<XSTRING*>     lines;

  private:

    void                  Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




