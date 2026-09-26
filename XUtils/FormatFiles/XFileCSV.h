/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileCSV.h
* 
* @class      XFILECSV
* @brief      eXtended Utils CSV file class (Comma Separate Value file)
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

#include "XString.h"
#include "XFileTXT.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILECSV_RECORD
{
  public:

                                  XFILECSV_RECORD         ();
    virtual                      ~XFILECSV_RECORD         ();

    int                           GetNElements            ();
    bool                          GetElement              (int index, XSTRING& element);
    XCHAR*                        GetElement              (int index);

    bool                          AddElement              (XSTRING& element);
    bool                          AddElement              (XCHAR* element);

    bool                          DeleteElement           (int index);
    bool                          DeleteAllElements       ();

    bool                          Copy                    (XFILECSV_RECORD* header);

  private:

    void                          Clean                   ();

    XVECTOR<XSTRING*>             elements;
};

class XFILECSV : public XFILETXT
{
  public:
                                  XFILECSV                ();
                                  XFILECSV                (XPATH& xpath);
                                  XFILECSV                (XFILE* file);
    virtual                      ~XFILECSV                ();

    virtual bool                  Open                    (XPATH& xpath, bool readonly = true);
    virtual bool                  Close                   (void);

    XCHAR                         GetSeparator            ();
    bool                          SetSeparator            (XCHAR separator);

    bool                          HaveHeader              ();
    XFILECSV_RECORD*              GetHeader               ();
    bool                          SetHeader               (XFILECSV_RECORD* header);

    int                           GetNRecords             ();

    XFILECSV_RECORD*              ReadRecord              (XDWORD nrecord);
    bool                          WriteRecord             (XDWORD nrecord, XFILECSV_RECORD* record);

    bool                          AddRecord               (XFILECSV_RECORD* record);

    bool                          DeleteRecord            (XDWORD nrecord);
    bool                          DeleteAllRecords        ();

  private:

    bool                          Serialization           (bool read, bool haveheader = false);

    bool                          CreateRecordOfLine      (XSTRING& line, XFILECSV_RECORD* record);
    bool                          CreateLineOfRecord      (XFILECSV_RECORD* record, XSTRING& line);

    void                          Clean                   ();

    XCHAR                         separator;
    XFILECSV_RECORD               header;
    XVECTOR<XFILECSV_RECORD*>     records;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





