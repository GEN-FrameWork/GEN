/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileZIP_XEvent.h
* 
* @class      XFILEZIP_XEVENT
* @brief      eXtended Utils ZIP File eXtended Event class
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

#include "XEvent.h"
#include "XPath.h"
#include "XSubject.h"

#include "XFileZIP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEZIP_XEVENT_TYPE
{
  XFILEZIP_XEVENT_TYPE_UNKNOWN                = XEVENT_TYPE_FILEZIP ,

  XFILEZIP_XEVENT_TYPE_ZIP_OPEN                                     ,
  XFILEZIP_XEVENT_TYPE_ZIP_ADDFILE                                  ,
  XFILEZIP_XEVENT_TYPE_ZIP_CLOSE                                    ,

  XFILEZIP_XEVENT_TYPE_UNZIP_OPEN                                   ,
  XFILEZIP_XEVENT_TYPE_UNZIP_DELFILE                                ,
  XFILEZIP_XEVENT_TYPE_UNZIP_EXTRACTFILE                            ,
  XFILEZIP_XEVENT_TYPE_UNZIP_CLOSE                                  ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XPUBLISHER;

class XFILEZIP_XEVENT : public XEVENT
{
  public:
                                XFILEZIP_XEVENT                 (XSUBJECT* subject, XDWORD type = XFILEZIP_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_FILEZIP);
    virtual                    ~XFILEZIP_XEVENT                 ();

    XPATH*                      GetFileName                     ();

    XFILECMPZIP*                GetFileCompress                 ();
    void                        SetFileCompress                 (XFILECMPZIP* filecmp);

    float                       GetOperationPercent             ();
    void                        SetOperationPercent             (float operationpercent);

    XFILEZIP_ERROR              GetError                        ();
    void                        SetError                        (XFILEZIP_ERROR error);


  private:

    void                        Clean                           ();

    XPATH                       filename;
    XFILECMPZIP*                filecmp;
    float                       operationpercent;
    XFILEZIP_ERROR              error;

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





