/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileHash.h
* 
* @class      XFILEHASH
* @brief      eXtended Utils HASH file class (file with HASH included)
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

#include "Hash.h"
#include "XFile.h"
#include "XFileID.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XHASH32;

class XFILEHASH : public XFILECONTAINER
{
  public:
                      XFILEHASH         (HASH* hash);
    virtual          ~XFILEHASH         ();

    bool              Open              (XPATH& pathname, bool readonly = true, bool checkHASH = true, bool checkversion = true);
    bool              Create            (XPATH& pathname, bool checkhash = true);
    bool              Close             ();

    XWORD             GetID             ();
    XWORD             GetType           ();
    XWORD             GetVersion        ();
    XSTRING*          GetIDString       ();

    bool              Set               (XWORD ID, XWORD type, XWORD version, XCHAR* IDstring = NULL);

    bool              SetID             (XWORD ID);
    bool              SetType           (XWORD type);
    bool              SetVersion        (XWORD version);
    bool              SetIDString       (XCHAR* IDstring);
    bool              SetIDString       (XSTRING& IDstring);

    XQWORD            GetDataPosition   ();

    bool              UpdateHash        ();

  private:

    void              Clean             ();

    HASH*             hash;

    XWORD             ID;
    XWORD             type;
    XWORD             version;
    XSTRING           IDstring;

    bool              hashisupdate;

    XQWORD            hashposition;
    XQWORD            dataposition;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





