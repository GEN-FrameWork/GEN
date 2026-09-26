/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileZIP.h
* 
* @class      XFILEZIP
* @brief      eXtended Utils ZIP file class
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

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEZIP_ERROR
{
  XFILEZIP_ERROR_NONE                      = 0 ,

  XFILEZIP_ERROR_ZIP_OPEN                      ,
  XFILEZIP_ERROR_ZIP_ADDFILE                   ,
  XFILEZIP_ERROR_ZIP_CLOSE                     ,

  XFILEZIP_ERROR_UNZIP_OPEN                    ,
  XFILEZIP_ERROR_UNZIP_EXTRACTFILEREAD         ,
  XFILEZIP_ERROR_UNZIP_EXTRACTFILEWRITE        ,
  XFILEZIP_ERROR_UNZIP_DELETEFILECMP           ,
  XFILEZIP_ERROR_UNZIP_CLOSE                   ,

};

#define XFILEZIP_DEFAULT_MAXBLOCKFILE  1024*100

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XDATETIME;

class XFILECMPZIP
{
  public:
                              XFILECMPZIP               (bool compress, void* filehdl);
    virtual                  ~XFILECMPZIP               ();

    bool                      Open                      (XSTRING& password);
    bool                      Create                    (XDWORD crcfile, zip_fileinfo* zfinfo, XCHAR* password = NULL);

    bool                      IsActive                  ();

    XCHAR*                    GetName                   ();
    void                      SetName                   (XSTRING& name);

    bool                      IsDirectory               ();

    XQWORD                    Size                      ();
    XQWORD                    UnCompressSize            ();

    XDWORD                    GetCRC32                  ();

    bool                      GetSeek                   (int *position);

    bool                      Read                      (XBYTE* buffer, XDWORD size);
    bool                      Read                      (XBYTE* buffer, int* size);
    bool                      Write                     (XBYTE* buffer, XDWORD size);

    bool                      IsEOF                     ();

    bool                      Close                     ();

  private:

    bool                      SelectCurrenFile          ();
    void                      Clean                     ();

    void*                     filehdl;
    bool                      on;
    bool                      zipoper;
    XSTRING                   name;
    XSTRING                   comment;
    XDWORD                    method;
    XQWORD                    uncompresssize;
    XQWORD                    compresssize;
    XDWORD                    CRC32;
    bool                      open;
};

class XFILEZIP : public XSUBJECT
{
  public:
                              XFILEZIP                  (XDWORD maxsizebuffer = XFILEZIP_DEFAULT_MAXBLOCKFILE);
    virtual                  ~XFILEZIP                  ();

    XDWORD                    GetMaxSizeBuffer          ();
    void                      SetMaxSizeBuffer          (XDWORD maxsizebuffer);

    bool                      Open                      (XPATH& xpath);

    XFILECMPZIP*              CreateFileCmp             (XPATH& xpath);
    XDWORD                    GetFileCRC                (XFILE* xfile);

    bool                      AddFile                   (XPATH& xpath, XPATH& xpathnew, XCHAR* password = NULL);
    bool                      Close                     ();

    float                     GetLastOperationPercent   ();
    XFILEZIP_ERROR            GetLastError              ();

  private:

    void                      Clean                     ();

    XDATETIME*                xdatetime;
    zipFile                   filehdl;
    XPATH                     xpathzip;
    XDWORD                    maxsizebuffer;

    float                     lastoperationpercent;
    XFILEZIP_ERROR            lasterror;
};

class XFILEUNZIP : public XSUBJECT
{
  public:
                              XFILEUNZIP                (XDWORD maxsizebuffer = XFILEZIP_DEFAULT_MAXBLOCKFILE);
    virtual                  ~XFILEUNZIP                ();

    XDWORD                    GetMaxSizeBuffer          ();
    void                      SetMaxSizeBuffer          (XDWORD maxsizebuffer);

    bool                      Open                      (XPATH& xpath);

    int                       GetNFiles                 ();

    XFILECMPZIP*              GotoFile                  (bool first=true);
    XFILECMPZIP*              GotoFile                  (XPATH& xpath);
    XFILECMPZIP*              GotoFile                  (int index);

    bool                      DelFile                   (int index, XCHAR* password = NULL);

    bool                      DecompressFile            (int sourceindex, XPATH& xpathtarget, XCHAR* nametarget = NULL, XCHAR* password = NULL);
    bool                      DecompressFile            (XSTRING& namesource, XPATH& xpathtarget, XCHAR* nametarget = NULL, XCHAR* password = NULL);
    bool                      DecompressFile            (XFILECMPZIP* filecmp, XPATH& xpathtarget, XCHAR* nametarget = NULL, XCHAR* password = NULL);

    bool                      Close                     ();

    float                     GetLastOperationPercent   ();
    XFILEZIP_ERROR            GetLastError              ();

  private:

    XFILECMPZIP*              CreateCurrentFile         ();
    void                      Clean                     ();

    unzFile                   filehdl;
    XPATH                     xpathunzip;
    XDWORD                    maxsizebuffer;

    float                     lastoperationpercent;
    XFILEZIP_ERROR            lasterror;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





