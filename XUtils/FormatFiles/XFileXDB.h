/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileXDB.h
* 
* @class      XFILEXDB
* @brief      eXtended Utils Dialog Sentence file class
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

#include "XMap.h"
#include "XFileHash.h"
#include "XFileID.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XFILEXDB_TYPE         (XFILE_TYPE_GENERIC | XFILE_FEATURES_HASH)
#define XFILEXDB_VERSION      0x0100
#define XFILEXDB_IDSTRING     __L("[XDB File]")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXDB : public XFILEHASH
{
  public:
                              XFILEXDB                (HASH* hash);
                              XFILEXDB                (HASH* hash, XPATH& xpath);
    virtual                  ~XFILEXDB                ();

    bool                      SetPath                 (XPATH& xpath);

    bool                      Ini                     ();
    bool                      End                     ();

    XMAP<XDWORD, XDWORD>*     GetIndexMap             ();

    bool                      OpenFile                ();

    int                       GetNumberRecords        ();
    XBUFFER*                  GetRecord               (XDWORD ID);
    XBUFFER*                  GetRecordIndex          (XDWORD index);
    XDWORD                    GetPosition             (XDWORD ID);

    bool                      CloseFile               ();

  protected:

    HASH*                     hash;

    XPATH                     xpath;
    bool                      isopen;

    int                       nrecords;
    XMAP<XDWORD, XDWORD>      indexmap;

  private:

    void                      Clean                   ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





