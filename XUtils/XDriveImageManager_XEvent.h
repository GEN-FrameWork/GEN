/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDriveImageManager_XEvent.h
* 
* @class      XDRIVEIMAGEMANAGER_XEVENT
* @brief      eXtended Utils Drive Image Mamanger Event class
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
#include "XSubject.h"
#include "XDriveImageManager.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XDRIVEIMAGEMANAGER_XEVENT_TYPE
{
  XDRIVEIMAGEMANAGER_XEVENT_TYPE_UNKNOWN          = XEVENT_TYPE_DRIVEIMAGEMANAGER ,

  XDRIVEIMAGEMANAGER_XEVENT_TYPE_READSECTORS                                      ,
  XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITESECTORS                                     ,
  XDRIVEIMAGEMANAGER_XEVENT_TYPE_READTOIMGFILE                                    ,
  XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITEFROMIMGFILE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XPUBLISHER;

class XDRIVEIMAGEMANAGER_XEVENT : public XEVENT
{
  public:
                                XDRIVEIMAGEMANAGER_XEVENT       (XSUBJECT* subject, XDWORD type = XDRIVEIMAGEMANAGER_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_DRIVEIMAGEMANAGER);
    virtual                    ~XDRIVEIMAGEMANAGER_XEVENT       ();

    XDRIVEIMAGEMANAGER_DRIVE*   GetDrive                        ();
    void                        SetDrive                        (XDRIVEIMAGEMANAGER_DRIVE* drive);

    XQWORD                      GetIndexSector                  ();
    void                        SetIndexSector                  (XQWORD indexsector);

    XQWORD                      GetNumberSectorsInOperation     ();
    void                        SetNumberSectorsInOperation     (XQWORD numbersectorsinoperation);

    float                       GetOperationPercent             ();
    void                        SetOperationPercent             (float operationpercent);

    XDRIVEIMAGEMANAGER_ERROR    GetError                        ();
    void                        SetError                        (XDRIVEIMAGEMANAGER_ERROR error);

  private:

    void                        Clean                           ();

    XDRIVEIMAGEMANAGER_DRIVE*   drive;
    XQWORD                      indexsector;
    XQWORD                      numbersectorsinoperation;
    float                       operationpercent;
    XDRIVEIMAGEMANAGER_ERROR    error;

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





