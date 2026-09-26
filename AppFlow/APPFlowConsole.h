/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowConsole.h
* 
* @class      APPFLOWCONSOLE
* @brief      Application Flow Console class
* @ingroup    APPFLOW
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

#include "XObserver.h"

#include "APPFlowBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define APPFLOWCONSOLE_DEFAULT_MESSAGEMASK                   __L("%-36s ... ")
#define APPFLOWCONSOLE_DEFAULT_MESSAGEMASKWITHOUTPOINTS      __L("%-36s ")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XCONSOLE;

class APPFLOWCONSOLE : public APPFLOWBASE
{
  public:

                                  APPFLOWCONSOLE              ();
    virtual                      ~APPFLOWCONSOLE              ();

    bool                          Ini                         (XVECTOR<XSTRING*>* execparams);
  
    bool                          Show_Line                   (XSTRING& string, XSTRING& string2, int tab = 3, bool linefeed = true);
    bool                          Show_BlankLine              ();
    bool                          Show_LineDirect             (XSTRING& string, bool lf);
    bool                          Show_Header                 (bool separator);

    bool                          End                         ();

    XCONSOLE*                     GetConsole                  ();

    bool                          PrintExitMessage_Active     (bool isprintexitmessageactive);
    virtual bool                  PrintExitMessage            (XSTRING& message);

  protected:

    XCONSOLE*                     console;

  private:

    void                          Clean                       ();

   bool                           isprintexitmessageactive;

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




