/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTranslation_GEN.h
* 
* @class      XTRANSLATION_GEN
* @brief      eXtension Translation of GEN class (sencences within the framework)
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



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XTRANSLATION_GEN_ID
{
  XTRANSLATION_GEN_ID_UNKWON                                                  = 0 ,

  XTRANSLATION_GEN_ID_OK                                                          ,
  XTRANSLATION_GEN_ID_ERROR                                                       ,
  XTRANSLATION_GEN_ID_YES                                                         ,
  XTRANSLATION_GEN_ID_NO                                                          ,
  XTRANSLATION_GEN_ID_TRUE                                                        ,
  XTRANSLATION_GEN_ID_FALSE                                                       ,

  XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY                                     ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY                                    ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH                                       ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL                                       ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY                                         ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE                                        ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY                                        ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST                                      ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER                                   ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER                                     ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER                                    ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER                                    ,
  XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR                                ,

  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY                                  ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY                                  ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY                                 ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY                               ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY                                ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY                                  ,
  XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY                                ,

  XTRANSLATION_GEN_ID_XTIMER_XX_YEARS                                             ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR                                             ,
  XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS                                            ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH                                            ,
  XTRANSLATION_GEN_ID_XTIMER_XX_DAYS                                              ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_DAY                                              ,
  XTRANSLATION_GEN_ID_XTIMER_XX_HOURS                                             ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR                                             ,
  XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES                                           ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE                                           ,
  XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS                                           ,
  XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND                                           ,
  XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS                                         ,

  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG                                      ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG                                      ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS                                ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS                                ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS                           ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS                           ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION                                  ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION                                   ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH                                 ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY                                 ,

  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION                 ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY                 ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE                  ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE               ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME             ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE                ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG               ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG                 ,

  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP                      ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION                   ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY                      ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP                     ,
  XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS                    ,

  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO                                 ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR                         ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE                               ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION                          ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER                                 ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN                             ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE                       ,
  XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE                       ,

  XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE                           ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE                         ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS                                  ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD                                ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL                              ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW                                ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED                           ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE                               ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE                                  ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE                              ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE                         ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES                        ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES                       ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE                                 ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE                             ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES                                ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END                                    ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE                          ,
  XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES                         ,

  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED                    ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT               ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD                             ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED                             ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST                                ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID                               ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD                      ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER                                 ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE                             ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT                              ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT                                       ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND                                    ,
  XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS                       ,

};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRANSLATION_GEN_SENTENCE
{
  public:

                                          XTRANSLATION_GEN_SENTENCE          ();
    virtual                              ~XTRANSLATION_GEN_SENTENCE          ();

    void                                  Clean                              ();

    XDWORD                                ID;
    XDWORD                                codelanguage;
    XCHAR*                                sentence;
    XDWORD                                fixed;
};

class XTRANSLATION_GEN
{
  public:

    static bool                           GetIsInstanced                      ();
    static XTRANSLATION_GEN&              GetInstance                         ();
    static bool                           DelInstance                         ();

    bool                                  IsLanguageAvailable                 (XDWORD code);

    bool                                  Sentence_Add                        (XDWORD ID, XDWORD codelanguage, XCHAR* sentence, XDWORD fixed = 0);
    XTRANSLATION_GEN_SENTENCE*            Sentence_Get                        (XDWORD ID, XDWORD codelanguage);
    XTRANSLATION_GEN_SENTENCE*            Sentence_FindByText                 (XCHAR* sentence, bool ignorecase = false);

    bool                                  Sentences_AddAll                    ();
    bool                                  Sentences_DeleteAll                 ();

    bool                                  Sentences_AddToTranslation          (XDWORD codelanguage);

  private:
                                          XTRANSLATION_GEN                    ();
                                          XTRANSLATION_GEN                    (XTRANSLATION_GEN const&);
    virtual                              ~XTRANSLATION_GEN                    ();

    void                                  operator =                          (XTRANSLATION_GEN const&);

    void                                  Clean                               ();

    static XTRANSLATION_GEN*              instance;
    XVECTOR<XDWORD>                       languageavailables;
    XVECTOR<XTRANSLATION_GEN_SENTENCE*>   sentences;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




