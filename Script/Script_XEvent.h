/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_XEvent.h
* 
* @class      SCRIPT_XEVENT
* @brief      Script XEvent
* @ingroup    SCRIPT
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
#include "XEvent.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum SCRIPT_XEVENT_TYPE
{
  SCRIPT_XEVENT_TYPE_UNKNOWN      = XEVENT_TYPE_SCRIPT  ,
  SCRIPT_XEVENT_TYPE_ERROR                              ,
  SCRIPT_XEVENT_TYPE_BREAK                              ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SCRIPT;

class SCRIPT_XEVENT : public XEVENT
{
  public:
                                        SCRIPT_XEVENT           (XSUBJECT* subject, XDWORD type = SCRIPT_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_SCRIPT);
    virtual                            ~SCRIPT_XEVENT           ();

    SCRIPT*                             GetScript               ();
    void                                SetScript               (SCRIPT* script);

    XSTRING*                            GetNameScript           ();

    int                                 GetError                ();
    void                                SetError                (int error);

    XSTRING*                            GetErrorText            ();

    int                                 GetNLine                ();
    void                                SetNLine                (int nline);

    XSTRING*                            GetCurrentToken         ();

  private:

    void                                Clean                   ();

    SCRIPT*                             script;
    XSTRING                             namescript;
    int                                 error;
    XSTRING                             errortext;
    XSTRING                             currenttoken;
    int                                 nline;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




