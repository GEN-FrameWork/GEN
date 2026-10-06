/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLog.h
* 
* @class      XLOG
* @brief      eXtended Utils Log class
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
#include "XBuffer.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XLOGLEVEL
{
  XLOGLEVEL_INFO          = 0x01  ,
  XLOGLEVEL_WARNING       = 0x02  ,
  XLOGLEVEL_ERROR         = 0x04  ,

  XLOGLEVEL_ALL           = 0x0F
};


enum XLOGTYPELIMIT
{
  XLOGTYPELIMIT_NONE      = 0 ,
  XLOGTYPELIMIT_NENTRYS       ,
  XLOGTYPELIMIT_SIZE
};


#define XLOG_DEFAULTMAXFILESBACKUP      10       // Percent to reduce default
#define XLOG_DEFAULTMAXNENTRYS          3000     // Max default number of entries
#define XLOG_DEFAULTMAXSIZE             50000    // Max default size
#define XLOG_DEFAULTREDUCTIONPERCENT    50       // Percent to reduce default

#define XLOG_MAXLINESTOFLUSH            1000     // Number lines to Flush

#define XLOG_PREFIX                     _L("[LOG]_")
#define XLOG_CREATEMASKBACKUPFILE       _L("[LOG]_%04d_%02d_%02d-%02d_%02d_%02d_")
#define XLOG_SEARCHMASKBACKUPFILE       _L("[LOG]_*-*")

#define XLOG_NAMEFILEEXT                _L(".log")

#define XLOG_MAXDATETIMESIZE            20
#define XLOG_MAXLEVELSIZE                8
#define XLOG_MAXSECTIONIDSIZE           10

#ifndef GEN_XLOG
#define GEN_XLOG                        XLOG::GetInstance()
#endif


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;
class XDATETIME;
class XTIMER;
class XFILETXT;
class XTHREADCOLLECTED;

class XLOGENTRY
{
  public:
                            XLOGENTRY                         ();
    virtual                ~XLOGENTRY                         ();

    void                    Clean                             ();

    XQWORD                  position;
    XQWORD                  size;
};

class GEN_API_LIB_EXP XLOGBASE
{
  public:
                            XLOGBASE                          ();
    virtual                ~XLOGBASE                          ();
  
    bool                    Ini                               (XCHAR* pathfile, XCHAR* nameapplication = NULL, bool isdumpintrace = true);
    bool                    Ini                               (XPATH& xpathfile, XCHAR* nameapplication = NULL, bool isdumpintrace = true);
    bool                    Ini                               (XPATH& xpathfile, XSTRING& nameapplication, bool isdumpintrace = true);

    bool                    SetLimit                          (XLOGTYPELIMIT typelimit, int limit = XLOG_DEFAULTMAXNENTRYS, int reductionpercentnentrys = XLOG_DEFAULTREDUCTIONPERCENT);
    bool                    SetBackup                         (bool isactive, int maxfiles = XLOG_DEFAULTMAXFILESBACKUP, bool iscompress = false);
    bool                    SetFilters                        (XCHAR* sectionsID, XBYTE level = XLOGLEVEL_ALL);

    XCHAR*                  GetPathFile                       ();
    XSTRING*                GetNameApplication                ();
    XSTRING*                GetPrevLabel                      ();

    bool                    IsActive                          ();

    void                    SetIsActive                       (bool isactive);
    XDWORD                  GetNEntrys                        ();
    XDWORD                  GetNLines                         ();
    XQWORD                  GetSize                           ();
    bool                    GetLevelString                    (XLOGLEVEL level, XSTRING& strlevel, XDWORD size);
    XSTRING*                GetSectionsIDFilter               ();
    bool                    PassFilters                       (XLOGLEVEL level, XCHAR* sectionID);

    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XCHAR* mask, ...);
    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBYTE* data, XDWORD size, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);
    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBUFFER& data, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);

    bool                    Flush                             ();

    bool                    End                               (bool doflush = true);

  protected:

    bool                    CalculateInitialStatus            ();
    bool                    FlushMemoryEntrys                 ();

    int                     Backup_GetNFiles                  ();
    bool                    Backup_GetNameFileMoreOld         (XPATH& xpath);
    bool                    Backup_CreateNameZIP              (XPATH& xpathzipfile);
    bool                    Backup_AdjustNFilesInCompressed   ();
    bool                    Backup_AdjustNFiles               (XCHAR* pathnamelog, bool iscompress);
    bool                    Backup_ControlLimits              ();

    XDATETIME*              xdatetime;
    XTIMER*                 xtimer;
    XMUTEX*                 mutex;
    XFILETXT*               filelog;

    XPATH                   xpathfile;
    XSTRING                 nameapplication;
    XSTRING                 prevlabel;
    bool                    isdumpintrace;

    bool                    isactive;
    XLOGTYPELIMIT           typelimit;

    bool                    backupisactive;
    int                     backupmaxfiles;
    bool                    backupiscompress;
    XPATH                   backupxpathnamelog;

    XDWORD                  limit;
    XDWORD                  reductionpercent;

    XSTRING                 lastsectionID;

    XSTRING                 filtersectionsID;
    XBYTE                   filterlevel;

    XDWORD                  nentrys;
    XDWORD                  nlines;
    XQWORD                  size;
    XVECTOR<XLOGENTRY*>     entrys;
    XLOGENTRY*              lastentry;
    
    void                    Clean                             ();    
};


class GEN_API_LIB_EXP XLOG : public XLOGBASE
{
  public:

    static bool             GetIsInstanced                    ();
    static XLOG&            GetInstance                       ();
    static bool             DelInstance                       ();
  
  private:
                            XLOG                              ();
                            XLOG                              (XLOG const&);        
    virtual                ~XLOG                              ();

    void                    operator =                        (XLOG const&);        

    static XLOG*            instance;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




