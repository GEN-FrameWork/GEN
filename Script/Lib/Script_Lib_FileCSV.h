/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_FileCSV.h
* 
* @class      SCRIPT_LIB_FILECSV
* @brief      Script Library File CSV (read / write)
* @ingroup    SCRIPT
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

#include "XPath.h"
#include "XString.h"
#include "Script_Lib.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_FILECSV   __L("FileCSV")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class XFILECSV;
class SCRIPT;

class SCRIPT_LIB_FILECSV : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_FILECSV      ();
    virtual              ~SCRIPT_LIB_FILECSV      ();

    bool                  AddLibraryFunctions     (SCRIPT* script);

    bool                  Create                  (XSTRING& path);
    bool                  Open                    (XSTRING& path, bool readonly);
    bool                  Close                   ();
    bool                  Save                    ();

    bool                  SetSeparator            (XSTRING& separator);
    bool                  SetHeader               (XVECTOR<XSTRING*>* elements);
    bool                  AddRecord               (XVECTOR<XSTRING*>* elements);

    int                   GetNRecords             ();
    int                   GetNHeaderElements      ();
    bool                  GetHeaderElement        (int index, XSTRING& element);
    bool                  GetElement              (int recordindex, int elementindex, XSTRING& element);

    XCHAR*                GetFilePath             ();
    bool                  IsOpen                  ();

    static bool           GetShortDateTime        (XSTRING& stamp);

  private:

    bool                  EnsureInstance          ();
    bool                  ApplyHeader             (XVECTOR<XSTRING*>* elements);
    bool                  FlushToDisk             ();

    void                  Clean                   ();

    XFILECSV*             filecsv;
    XPATH                 filepath;
    bool                  haveheaderdeclared;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void    Call_FileCSV_Create               (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_Open                 (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_Close                (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_Save                 (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_SetSeparator         (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_SetHeader            (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_AddRecord            (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetNRecords          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetNHeaderElements   (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetHeaderElement     (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetElement           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetPath              (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_IsOpen               (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_FileCSV_GetShortDateTime     (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);

