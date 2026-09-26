/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileHEX.h
* 
* @class      XFILEHEX
* @brief      eXtended Utils HEX file class (hex ASCII format)
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
#include "XPath.h"
#include "XFileTXT.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEHEX_ENTRYTYPE
{
  XFILEHEX_ENTRYTYPE_DATA                     = 0x00 ,
  XFILEHEX_ENTRYTYPE_EOF                      = 0x01 ,
  XFILEHEX_ENTRYTYPE_EXTENDADDRESS            = 0x02 ,
  XFILEHEX_ENTRYTYPE_STARTSEGMENT             = 0x03 ,
  XFILEHEX_ENTRYTYPE_EXTENDEDLINEALADDRESS    = 0x04 ,
  XFILEHEX_ENTRYTYPE_STARTLINEALADDRESS       = 0x05 ,

  XFILEHEX_ENTRYTYPE_NODEFINED                = 0xFF
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEHEX_ENTRY
{
  public:
                                XFILEHEX_ENTRY          ();
    virtual                    ~XFILEHEX_ENTRY          ();

    XBYTE                       GetSizeData             ();
    void                        SetSizeData             (XBYTE sizedata);

    XWORD                       GetAddress              ();
    void                        SetAddress              (XWORD address);

    XFILEHEX_ENTRYTYPE          GetEntryType            ();
    void                        SetEntryType            (XFILEHEX_ENTRYTYPE entrytype);

    XBUFFER*                    GetData                 ();

    XBYTE                       CalculeCRC              (XBUFFER& data);

  private:

    void                        Clean                   ();

    XBYTE                       sizedata;
    XWORD                       address;
    XFILEHEX_ENTRYTYPE          entrytype;
    XBUFFER                     data;
};

class XFILEHEX : public XFILETXT
{
  public:
                                XFILEHEX                ();
    virtual                    ~XFILEHEX                ();

    bool                        DecodeAllLines          ();
    bool                        EncodeAllLines          ();

    bool                        DecodeLine              (XSTRING* line, XFILEHEX_ENTRY& entry);
    bool                        EncodeLine              (XFILEHEX_ENTRY* entry, XSTRING& line);

    XDWORD                      GetNEntrys              ();

    bool                        Entry_Add               (XSTRING* line);
    XFILEHEX_ENTRY*             Entry_Get               (XWORD address, XFILEHEX_ENTRYTYPE type = XFILEHEX_ENTRYTYPE_DATA);
    XFILEHEX_ENTRY*             Entry_Get               (int index);
    bool                        Entry_Delete            (XWORD address, XFILEHEX_ENTRYTYPE type = XFILEHEX_ENTRYTYPE_DATA);
    bool                        Entry_Delete            (int index);
    bool                        Entry_DeleteAll         ();

  private:

    void                        Clean                   ();

    XVECTOR<XFILEHEX_ENTRY*>    entrys;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





