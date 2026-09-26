/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDir.h
* 
* @class      XDIR
* @brief      eXtended Utils dir class
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
#include "XPath.h"
#include "XDateTime.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XDIRELEMENTTYPE
{
  XDIRELEMENTTYPE_NONE  = 0 ,
  XDIRELEMENTTYPE_FILE      ,
  XDIRELEMENTTYPE_DIR       ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XDIRELEMENT
{
  public:
                            XDIRELEMENT                   ();
    virtual                ~XDIRELEMENT                   ();

    XDIRELEMENTTYPE         GetType                       ();
    void                    SetType                       (XDIRELEMENTTYPE type);

    XPATH*                  GetPathSearch                 ();
    XSTRING*                GetPatternSearch              ();
    XPATH*                  GetNameFile                   ();
        
    XDATETIME*              GetDateTimeFile_Created       ();
    XDATETIME*              GetDateTimeFile_Modificated   ();
    XDATETIME*              GetDateTimeFile_LastAccess    ();

    void*                   GetHandle                     ();
    void                    SetHandle                     (void* handle);

    void*                   GetFindFileData               ();
    void                    SetFindFileData               (void* findfiledata);

    bool                    DeleteFindFileData            ();

  private:

    void                    Clean                         ();

    XDIRELEMENTTYPE         type;

    XPATH                   pathsearch;
    XSTRING                 patternsearch;
    XPATH                   namefile;
    XDATETIME               datetime_created;
    XDATETIME               datetime_modificated;
    XDATETIME               datetime_lastaccess;

    void*                   handle;
    void*                   findfiledata;
};

class XDIR 
{
  public:
                            XDIR                    ();
    virtual                ~XDIR                    ();

    bool                    Exist                   (XPATH& xpath);
    bool                    Make                    (XPATH& xpath, bool recursive = true);
    bool                    ChangeTo                (XPATH& xpath);
    bool                    Delete                  (XPATH& xpath, bool all);

    virtual bool            Exist                   (XCHAR* path)                                                     = 0;
    virtual bool            Make                    (XCHAR* path, bool recursive = true)                              = 0;
    virtual bool            ChangeTo                (XCHAR* path)                                                     = 0;
    virtual bool            Delete                  (XCHAR* path, bool all)                                           = 0;

    virtual bool            GetActual               (XPATH& path)                                                     = 0;

    bool                    FirstSearch             (XPATH& xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement);
    bool                    FirstSearch             (XPATH& xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement);
    bool                    FirstSearch             (XCHAR* xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement);

    virtual bool            FirstSearch             (XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)  = 0;
    virtual bool            NextSearch              (XDIRELEMENT* searchelement)                                      = 0;

    bool                    Copy                    (XPATH& xpathsource, XPATH& xpathtarget);
    bool                    Copy                    (XCHAR* xpathsource, XPATH& xpathtarget);
    bool                    Copy                    (XPATH& xpathsource, XCHAR* xpathtarget);
    bool                    Copy                    (XCHAR* xpathsource, XCHAR* xpathtarget);

  private:

    void                    Clean                   ();

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





