/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFile.h
* 
* @class      XFILE
* @brief      eXtended Utils File class
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

#include "XVector.h"
#include "XPath.h"
#include "XBuffer.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum
{
  XFILE_SEEKINI   =  0,
  XFILE_SEEKEND   = -1,
};


#define XFILE_MAXBUFFER     8192

#ifdef XTRACE_ACTIVE
  #define XFILE_DISPLAYNOTCLOSEFILES    XFileOpenList.DisplayAll();
#else
  #define XFILE_DISPLAYNOTCLOSEFILES
#endif

#define XFILE_GETFILESIZE(xpathfile, size)    { size = -1;                                        \
                                                XFILE* xfile=GEN_XFACTORY.Create_File();  \
                                                if(xfile)                                         \
                                                  {                                               \
                                                    if(xfile->Open(xpathfile))                    \
                                                      {                                           \
                                                        size = xfile->GetSize();                   \
                                                        xfile->Close();                           \
                                                      }                                           \
                                                    GEN_XFACTORY.Delete_File(xfile);              \
                                                  }                                               \
                                              }

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHER;

class XFILE 
{
  public:
                            XFILE                   ();
    virtual                ~XFILE                   ();

    virtual bool            Exist                   (XCHAR* xpath)                                              = 0;

    virtual bool            Open                    (XCHAR* xpath, bool isreadonly = true)                      = 0;
    virtual bool            Create                  (XCHAR* xpath)                                              = 0;

    XQWORD                  GetSize                 ();
    virtual bool            SetSize                 (XQWORD size)                                               = 0;

    virtual bool            GetPosition             (XQWORD& position)                                          = 0;
    virtual bool            SetPosition             (XQWORD position)                                           = 0;

    virtual bool            Read                    (XBYTE* buffer, XDWORD size, CIPHER* cipher = NULL)       = 0;
    virtual bool            Read                    (XBYTE* buffer, XDWORD* size , CIPHER* cipher = NULL)       = 0;

    virtual bool            Write                   (XBYTE* buffer, XDWORD size , CIPHER* cipher = NULL)        = 0;
    bool                    Write                   (XBUFFER &xbuffer);

    virtual bool            Flush                   ()                                                          = 0;

    virtual bool            Close                   ();

    virtual bool            Erase                   (XCHAR* xpath, bool overwrite = false)                      = 0;
    virtual bool            Rename                  (XCHAR* xpathold, XCHAR* xpathnew)                          = 0;

    virtual FILE*           CreateStructHandle      ()                                                          = 0;
    virtual bool            DeleteStructHandle      ()                                                          = 0;

    bool                    Exist                   (XPATH& xpath);

    bool                    Open                    (XPATH& xpath, bool readonly = true);
    bool                    Create                  (XPATH& xpath);

    bool                    Erase                   (XPATH& xpath, bool overwrite = false);
    bool                    Rename                  (XPATH& xpathold, XPATH& xpathnew);
    bool                    Rename                  (XCHAR* xpathold, XPATH& xpathnew);
    bool                    Rename                  (XPATH& xpathold, XCHAR* xpathnew);

    bool                    IsOpen                  ();
    bool                    IsReadOnly              ();
    XCHAR*                  GetPathNameFile         ();

    bool                    EraseBlock              (XQWORD size);
    bool                    InsertBlock             (XBYTE* buffer, XDWORD size);

    virtual void            ChangeAttributes        (XCHAR* attributes);

  protected:

    virtual bool            ActualizeSize           ()                                                          = 0;
    bool                    OverwriteContent        (XCHAR* xpath);

    bool                    isopen;
    bool                    isreadonly;
    XPATH                   xpathnamefile;
    XQWORD                  cachesize;

  private:

    void                    Clean                   ();

};


class XFILECONTAINER
{
  public:
                            XFILECONTAINER          ();
    virtual                ~XFILECONTAINER          ();

    bool                    CreatePrimaryFile       ();
    XFILE*                  GetPrimaryFile          ();
    bool                    DeletePrimaryFile       ();

  protected:

    XFILE*                  file;
    bool                    eraseprimaryfile;

  private:

    void                    Clean                   ();
};


#ifdef XTRACE_ACTIVE

class XFILEOPENLIST
{
  public:

    virtual                ~XFILEOPENLIST         ();

    bool                    Add                   (XFILE* xfile);
    bool                    Del                   (XFILE* xfile);

    bool                    CloseAll              ();
    bool                    DisplayAll            ();

  private:

    XVECTOR<XFILE*>         filelist;
};

#endif




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
extern XFILEOPENLIST  XFileOpenList;
#endif





