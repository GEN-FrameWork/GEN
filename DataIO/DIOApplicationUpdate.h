/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOApplicationUpdate.h
* 
* @class      DIOAPPLICATIONUPDATE
* @brief      Data Input/Output Application Update class
* @ingroup    DATAIO
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

#include "DIOURL.h"
#include "XPath.h"

#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOAPPLICATIONUPDATEFUNC_STATUS
{
  DIOAPPLICATIONUPDATEFUNC_STATUS_UNKNOW                = 0 ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_MAKEDIR                   ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEUNKNOW             ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEREQUEST            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEOPEN               ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILECONNECT            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEGETINFO            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEGETFILE            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEWAIT               ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEEND                ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEERROR              ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_CHANGEFILEUPLOAD          ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_ERASEFILEUPLOAD           ,
};


enum DIOAPPLICATIONUPDATE_ERROR
{
  DIOAPPLICATIONUPDATE_NOT_ERROR                        = 0 ,
  DIOAPPLICATIONUPDATE_ERROR_NOTVERSIONTOUPDATE             ,
  DIOAPPLICATIONUPDATE_ERROR_NOTFILES                       ,
  DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION               ,
  DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER                   ,
  DIOAPPLICATIONUPDATE_ERROR_MAKEDIR                        ,
  DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE                   ,
  DIOAPPLICATIONUPDATE_ERROR_RENAMEORIGINALFILE             ,
  DIOAPPLICATIONUPDATE_ERROR_COPYORIGINALFILE               ,
  DIOAPPLICATIONUPDATE_ERROR_SPECIALORIGINALFILE            ,
};


#define DIOAPPLICATIONUPDATE_INIFILE                                __L("update.ini")
#define DIOAPPLICATIONUPDATE_BACKUPPREFIX                           __L("GEN#BAK_")

#define DIOAPPLICATIONUPDATE_PORTDEFAULT                            80
#define DIOAPPLICATIONUPDATE_TIMEOUT                                60

#define DIOAPPLICATIONUPDATE_MAXINDEXFILES                          10000

#define DIOAPPLICATIONUPDATE_GENERALSECTION                         __L("general")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION                 __L("version")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION              __L("subversion")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL            __L("errorcontrol")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT        __L("systemmustbeinit")

#define DIOAPPLICATIONUPDATE_DIRSECTION                             __L("directorys")
#define DIOAPPLICATIONUPDATE_FILESSECTION                           __L("files")
#define DIOAPPLICATIONUPDATE_FILESSECTION_FILES                     __L("file")

#define DIOAPPLICATIONUPDATE_RESTOREDIR_ID                          __L("update_")
#define DIOAPPLICATIONUPDATE_RESTOREDIR_MASK                        __L("[%d.%d.%d]")

#define DIOAPPLICATIONUPDATE_LOG_SECTIONID_APPFLOWUPDATE            __L("AppUpdate")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XDIR;
class DIOFACTORY;
class DIOWEBCLIENT;

class DIOAPPLICATIONUPDATE_VERSIONDATA
{
  public:
                                          DIOAPPLICATIONUPDATE_VERSIONDATA      ();
    virtual                              ~DIOAPPLICATIONUPDATE_VERSIONDATA      ();

    XDWORD                                GetVersion                            ();
    void                                  SetVersion                            (XDWORD version);

    XDWORD                                GetSubVersion                         ();
    void                                  SetSubVersion                         (XDWORD subversion);

    XDWORD                                GetSubVersionError                    ();
    void                                  SetSubVersionError                    (XDWORD subversionerror);

    bool                                  SystemMustBeInit                      ();
    void                                  SetSystemMustBeInit                   (bool applicationmustbeinit);

    bool                                  CopyTo                                (DIOAPPLICATIONUPDATE_VERSIONDATA& versiondata);
    bool                                  CopyFrom                              (DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata);

    int                                   Compare                               (XDWORD version, XDWORD subversion, XDWORD subversionerror);

    void                                  Clean                                 ();

  private:

    XDWORD                                version;
    XDWORD                                subversion;
    XDWORD                                subversionerror;
    bool                                  systemmustbeinit;
};


class DIOAPPLICATIONUPDATE_FILE
{
  public:
                                          DIOAPPLICATIONUPDATE_FILE             (XPATH& xpathfile, XDWORD CRC32, XQWORD size);
    virtual                              ~DIOAPPLICATIONUPDATE_FILE             ();

    XPATH*                                GetXPathFile                          ();
    XDWORD                                GetCRC32                              ();
    XQWORD                                GetSize                               ();
    bool                                  HasSpecialControlUpdate               ();
    void                                  SetHasSpecialControlUpdate            (bool specialcontrolupdate);

  private:

    void                                  Clean                                 ();

    XPATH                                 xpathfile;
    XDWORD                                CRC32;
    XQWORD                                size;
    bool                                  specialcontrolupdate;
};

class DIOAPPLICATIONUPDATE : public XSUBJECT
{
  public:
                                          DIOAPPLICATIONUPDATE                  (XDWORD applicationversion, XDWORD applicationsubversion, XDWORD applicationerrorcontrol, XCHAR* applicationname, XPATH& xpathrootapp);
    virtual                              ~DIOAPPLICATIONUPDATE                  ();

    bool                                  GetVersionAvailable                   (DIOURL& url, int port, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  Download                              (DIOURL& url, int port, XCHAR* addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  Download                              (DIOURL& url, int port, XPATH& addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  CopyOriginalFilesToBackup             (XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  CopyUpdateFilesToOriginal             (XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error, bool restore);

    bool                                  DeleteFilesRenamed                    (XPATH& xpathtocheck);

    bool                                  ChangeFilesFromDownload               (DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, XPATH& xpathbackup, XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  RestartApplication                    (DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  GetListOfVersionsToRestore            (XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversiontorestore);
    bool                                  GetPreviousVersionToRestore           (XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversionstorestore, DIOAPPLICATIONUPDATE_VERSIONDATA& versiontorestore);

    bool                                  RestorePrevious                       (DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  Restore                               (DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  Restore                               (XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  EliminateOldRestorations              (int maxrestorations);

    bool                                  IsNewVersion                          (DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata);

    XDWORD                                Application_GetVersion                 ();
    XDWORD                                GetApplicationSubversion              ();
    XDWORD                                GetApplicationSubVersionError         ();

  private:

    bool                                  FilesToUpdate_CreateList              (XPATH& xpathupdate);
    bool                                  FilesToUpdate_EliminateFixedDir       (XPATH& xpath);
    bool                                  FilesToUpdate_CheckListChangedFiles   (XPATH& xpathupdate);
    bool                                  FilesToUpdate_DeleteList              ();

    bool                                  CopyFile                              (XPATH& origin, XPATH& target);

    void                                  Clean                                 ();


    XDWORD                                applicationversion;
    XDWORD                                applicationsubversion;
    XDWORD                                applicationsubversionerror;
    XSTRING                               applicationname;
    XPATH                                 xpathrootapp;

    XDIR*                                 xdir;
    DIOWEBCLIENT*                         webclient;


    XVECTOR<DIOAPPLICATIONUPDATE_FILE*>   filestoupdate;

    int                                   progressfiletoupdate;
    bool                                  stopprocess;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





