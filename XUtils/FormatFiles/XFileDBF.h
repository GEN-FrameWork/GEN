/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileDBF.h
* 
* @class      XFILEDBF
* @brief      eXtended Utils DBF file class
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
#include "XVector.h"
#include "XFile.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEDBF_TYPE
{
  XFILEDBF_TYPE_UNKNOWN         = 0,
  XFILEDBF_TYPE_DBASEIII           ,
  XFILEDBF_TYPE_DBASEIV            ,
  XFILEDBF_TYPE_DBASEV             ,
  XFILEDBF_TYPE_FOXPRO             ,
  XFILEDBF_TYPE_CLIPPER            ,
};


enum XFILEDBF_FIELDTYPE
{
  XFILEDBF_FIELDTYPE_UNKNOWN      = 0,
  XFILEDBF_FIELDTYPE_CHARACTER     ,
  XFILEDBF_FIELDTYPE_DATE          ,
  XFILEDBF_FIELDTYPE_FLOATING      ,
  XFILEDBF_FIELDTYPE_BINARYHEX     ,
  XFILEDBF_FIELDTYPE_LOGICAL       ,
  XFILEDBF_FIELDTYPE_MEMO          ,
  XFILEDBF_FIELDTYPE_VARIABLE      ,
  XFILEDBF_FIELDTYPE_PICTURE       ,
  XFILEDBF_FIELDTYPE_BINARY        ,
  XFILEDBF_FIELDTYPE_GENERAL       ,
  XFILEDBF_FIELDTYPE_SHORTINT      ,
  XFILEDBF_FIELDTYPE_LONGINT       ,
  XFILEDBF_FIELDTYPE_DOUBLE        ,
};


#define XFILEDBF_HEADRESERVED   19
#define XFILEDBF_FIELDSTART     32
#define XFILEDBF_FIELDSIZE      32

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEDBF_FIELD
{
  public:
                                    XFILEDBF_FIELD              ();
    virtual                        ~XFILEDBF_FIELD              ();

    void                            Clean                       ();

    XSTRING                         name;
    XBYTE                           type;
    XBYTE                           size;
};

class XFILEDBF_RECORD
{
  public:
                                    XFILEDBF_RECORD             (XVECTOR<XFILEDBF_FIELD*>* fields);
    virtual                        ~XFILEDBF_RECORD             ();

    bool                            IsDelete                    ();
    bool                            IsFieldEmpty                (int indexfield);

    XBYTE*                          GetData                     (int indexfield);
    bool                            SetData                     (int indexfield, XBYTE* data, int limitsize);

    bool                            GetDataString               (int indexfield, XSTRING& string);
    bool                            SetDataString               (int indexfield, XSTRING& string);

    bool                            SetCharacterType            (int indexfield, XCHAR* data);
    bool                            SetData                     (XBYTE* data, XDWORD size);
    int                             GetSize                     ();
    int                             GetSizeField                (int indexfield);

  private:

    void                            Clean                       ();
    int                             GetOffset                   (int indexfield);

    XVECTOR<XFILEDBF_FIELD*>*       fields;
    int                             size;
    XBYTE*                          datarec;
};

class XFILEDBF_HEADER
{
  public:
                                    XFILEDBF_HEADER             ();
                                    XFILEDBF_HEADER             ( XFILE* fileb);
    virtual                        ~XFILEDBF_HEADER             ();

    bool                            Load                        (XFILE* fileb);
    bool                            Create                      (XFILE* fileb);
    bool                            Update                      (XFILE* fileb);

    XFILEDBF_TYPE                   GetType                     ();
    XWORD                           GetHeaderSize               ();
    XBYTE*                          GetHeaderData               ();
    XDWORD                          GetReservedData             ();
    bool                            SetReservedData             (XDWORD data);
    int                             GetNFields                  ();
    XDWORD                          GetNRecords                 ();
    bool                            SetNRecords                 (XDWORD nrecords);
    int                             GetSizeRecord               ();

    XVECTOR<XFILEDBF_FIELD*>*       GetFields                   ();
    XFILEDBF_FIELD*                 GetField                    (int indexfield);
    bool                            CreateField                 (XFILEDBF_FIELDTYPE type, XCHAR* name, XBYTE size = 0);

    bool                            CopyFrom                    (XFILEDBF_HEADER* header);

  private:

    bool                            DeleteAllFields             ();
    void                            Clean                       ();

    XBYTE*                          headerdata;
    XWORD                           headersize;
    XVECTOR<XFILEDBF_FIELD*>        fields;
};

class XFILEDBF : public XFILECONTAINER
{
  public:
                                    XFILEDBF                    ();
                                    XFILEDBF                    (XPATH& xpath);
                                    XFILEDBF                    (XFILE* filebasic);
    virtual                        ~XFILEDBF                    ();

    bool                            Open                        (XPATH& xpath, bool readonly=true);
    bool                            Create                      (XPATH& xpath);
    bool                            IsOpen                      ();
    bool                            Close                       (void);

    XPATH*                          GetXPath                    ();

    XFILEDBF_HEADER*                GetHeader                   ();

    XFILEDBF_RECORD*                ReadRecord                  (XDWORD nrecord);
    bool                            WriteRecord                 (XDWORD nrecord, XFILEDBF_RECORD* record);
    bool                            InsertRecord                (XDWORD nrecord, XFILEDBF_RECORD* record);
    bool                            AddRecord                   (XFILEDBF_RECORD* record);
    bool                            DeleteRecord                (XDWORD nrecord);

    bool                            Pack                        ();

    bool                            Reduction                   (int maxrecords, int reductionpercent);

  private:

    bool                            CreateHeader                ();
    void                            Clean                       ();

    XPATH                           xpath;
    XFILEDBF_HEADER*                header;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




