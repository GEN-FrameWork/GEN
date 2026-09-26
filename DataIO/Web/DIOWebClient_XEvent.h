/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebClient_XEvent.h
* 
* @class      DIOWEBCLIENT_XEVENT
* @brief      Data Input/Output Web Client Event class
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

#include "DIOURL.h"
#include "DIOWebHeader.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWEBCLIENT_XEVENT_TYPE
{
  DIOWEBCLIENT_XEVENT_TYPE_UNKNOWN          = XEVENT_TYPE_WEBCLIENT  ,
  DIOWEBCLIENT_XEVENT_TYPE_OPENWEB                                  ,
  DIOWEBCLIENT_XEVENT_TYPE_WRITEHEADER                              ,
  DIOWEBCLIENT_XEVENT_TYPE_SENDPOSTDATA                             ,
  DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR                              ,
  DIOWEBCLIENT_XEVENT_TYPE_READHEADER                               ,
  DIOWEBCLIENT_XEVENT_TYPE_READBODYBLOCK                            ,
  DIOWEBCLIENT_XEVENT_TYPE_CLOSEWEB
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWEBCLIENT_XEVENT : public XEVENT
{
  public:
                                  DIOWEBCLIENT_XEVENT               (XSUBJECT* subject, XDWORD type = DIOWEBCLIENT_XEVENT_TYPE_UNKNOWN, XDWORD family =  XEVENT_TYPE_WEBCLIENT);
    virtual                      ~DIOWEBCLIENT_XEVENT               ();

    DIOURL*                       GetURL                            ();

    DIOWEBHEADER_METHOD           GetMethod                         ();
    void                          SetMethod                         (DIOWEBHEADER_METHOD method);

    XSTRING*                      GetEtag                           ();

    XQWORD                        GetContentLenght                  ();
    void                          SetContentLenght                  (XQWORD contentlenght);

    float                         GetOperationPercent               ();
    void                          SetOperationPercent               (float operationpercent);

    XQWORD                        GetDownloadSize                   ();
    void                          SetDownloadSize                   (XQWORD downloadsize);

    XDWORD                        GetMBPSSpeed                      ();
    void                          SetMBPSSpeed                      (XDWORD MBPSspeed);


  private:

    void                          Clean                             ();

    DIOURL                        URL;
    DIOWEBHEADER_METHOD           method;
    XSTRING                       etag;
    XQWORD                        contentlenght;
    float                         operationpercent;
    XQWORD                        downloadsize;
    XDWORD                        MBPSspeed;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






