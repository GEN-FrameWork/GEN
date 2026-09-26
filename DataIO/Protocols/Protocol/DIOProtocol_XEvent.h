/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol_XEvent.h
* 
* @class      DIOPROTOCOL_XEVENT
* @brief      Data Input/Output eXtended event Generic Binary Protocol eXtended Event class
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

#include "DIOAlerts.h"
#include "DIOProtocol.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOPROTOCOL_XEVENT : public XEVENT
{
  public:
                                              DIOPROTOCOL_XEVENT              (XSUBJECT* subject, XDWORD type = DIOPROTOCOL_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_PROTOCOL);
    virtual                                  ~DIOPROTOCOL_XEVENT              ();

    DIOPROTOCOL*                              GetProtocol                     ();
    bool                                      SetProtocol                     (DIOPROTOCOL* protocol);

    DIOPROTOCOL_DIRECTION                     GetDirection                    ();
    void                                      SetDirection                    (DIOPROTOCOL_DIRECTION direction);

    XDWORD                                    GetType                         ();
    void                                      SetType                         (XDWORD type);

    bool                                      GetResult                       ();
    void                                      SetResult                       (bool result);

    DIOPROTOCOL_FILESTATUS                    GetFileStatus                   ();
    void                                      SetFileStatus                   (DIOPROTOCOL_FILESTATUS filestatus);

    bool                                      FileIsSend                      ();
    void                                      SetFileIsSend                   (bool fileissend);

    XPATH*                                    GetFileSource                   ();
    bool                                      SetFileSource                   (XCHAR* filesource);

    XPATH*                                    GetFileTarget                   ();
    bool                                      SetFileTarget                   (XCHAR* filetarget);

    int                                       GetFilePercent                  ();
    void                                      SetFilePercent                  (int filepercent);

    void*                                     GetParam1                       ();
    void                                      SetParam1                       (void* param1);

    XDWORD                                    GetParam2                       ();
    void                                      SetParam2                       (XDWORD param2);

    DIOALERT*                                 GetAlert                        ();
    void                                      SetAlert                        (DIOALERT* alert);

  private:

    void                                      Clean                           ();

    DIOPROTOCOL*                              protocol;
    DIOPROTOCOL_DIRECTION                     direction;
    XDWORD                                    type;
    bool                                      result;

    DIOPROTOCOL_FILESTATUS                    filestatus;
    bool                                      fileissend;
    XPATH                                     filesource;
    XPATH                                     filetarget;
    int                                       filepercent;

    void*                                     param1;
    XDWORD                                    param2;

    DIOALERT*                                 alert;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





