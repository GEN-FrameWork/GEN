/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebPageHTMLCreator_XEvent.h
* 
* @class      DIOWEBPAGEHTMLCREATOR_XEVENT
* @brief      Data Input/Output Web Page HTML Create eXtended Event class
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

#include "XString.h"
#include "XPath.h"
#include "XEvent.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE
{
  DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_UNKNOW                = XEVENT_TYPE_WEBSERVER  ,
  DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_XTAGFOUND
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOWEBPAGEHTMLCREATOR;


class DIOWEBPAGEHTMLCREATOR_XEVENT : public XEVENT
{
  public:
                                            DIOWEBPAGEHTMLCREATOR_XEVENT    (XSUBJECT* subject, XDWORD type = DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_UNKNOW);
    virtual                                ~DIOWEBPAGEHTMLCREATOR_XEVENT    ();

    DIOWEBPAGEHTMLCREATOR*                  GetPageHTMLCreator              ();
    void                                    SetPageHTMLCreator              (DIOWEBPAGEHTMLCREATOR* pageHTMLcreator);

    XSTRING*                                GetXTAG                         ();
    void                                    SetXTAG                         (XSTRING* XTAG);

  private:

    void                                    Clean                           ();

    DIOWEBPAGEHTMLCREATOR*                  pageHTMLcreator;
    XSTRING                                 XTAG;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





