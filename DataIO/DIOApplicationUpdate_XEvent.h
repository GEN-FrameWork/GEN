/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOApplicationUpdate_XEvent.h
* 
* @class      DIOAPPLICATIONUPDATE_XEVENT
* @brief      Data Input/Output Application Update eXtended Event class
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

#include "XEvent.h"
#include "XString.h"
#include "XVector.h"

#include "DIOApplicationUpdate.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOAPPLICATIONUPDATE_XEVENT_TYPE
{
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNKNOWN                 = XEVENT_TYPE_APPFLOWUPDATE    ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE                                    ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE                                  ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE                                           ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE                                              ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILES                                          ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE                                     ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES                                    ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES                                   ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE                                             ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEEXECFILE                                         ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILES                                            ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END                                                ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE                                      ,
  DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILES                                     ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSUBJECT;
class XSTRING;
class DIOAPPLICATIONUPDATE_VERSIONDATA;
class DIOAPPLICATIONUPDATE_FILE;

class DIOAPPLICATIONUPDATE_XEVENT : public XEVENT
{
  public:
                                            DIOAPPLICATIONUPDATE_XEVENT       (XSUBJECT* subject, XDWORD family = XEVENT_TYPE_APPFLOWUPDATE, XDWORD type = DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNKNOWN);
    virtual                                ~DIOAPPLICATIONUPDATE_XEVENT       ();

    DIOAPPLICATIONUPDATE_VERSIONDATA*       GetVersionData                    ();
    void                                    SetVersionData                    (DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata);

    int                                     GetVersionComparation             ();
    void                                    SetVersionComparation             (int versioncomparation);

    XVECTOR<DIOAPPLICATIONUPDATE_FILE*>*    GetFilesToUpdate                  ();
    void                                    SetFilesToUpdate                  (XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* filestoupdate);

    XDWORD                                  GetIndexOfOperation               ();
    void                                    SetIndexOfOperation               (XDWORD indexoperation);

    DIOAPPLICATIONUPDATE_ERROR              GetError                          ();
    void                                    SetError                          (DIOAPPLICATIONUPDATE_ERROR error);

    DIOAPPLICATIONUPDATE_FILE*              GetActualFileToUpdate             ();
    XSTRING*                                GetActualUnzipFileName            ();

  private:

    void                                    Clean                             ();

    int                                     versioncomparation;
    DIOAPPLICATIONUPDATE_VERSIONDATA*       versiondata;

    XVECTOR<DIOAPPLICATIONUPDATE_FILE*>*    filestoupdate;

    XDWORD                                  indexoperation;
    DIOAPPLICATIONUPDATE_ERROR              error;

    XSTRING                                 actualunzipfilename;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





