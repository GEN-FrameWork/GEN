/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_TraceServer.h
* 
* @class      SCRIPT_LIB_TRACESERVER
* @brief      Script Library XTrace Server (open / get / clear / close)
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

#include "Script_Lib.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_TRACESERVER   __L("TraceServer")



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class SCRIPT;
class XTRACESERVER;
class XTRACESERVER_MSG;

class SCRIPT_LIB_TRACESERVER : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_TRACESERVER    ();
    virtual              ~SCRIPT_LIB_TRACESERVER    ();

    bool                  AddLibraryFunctions       (SCRIPT* script);

    XTRACESERVER*         GetTraceServer            ();

    bool                  FormatMessage             (XTRACESERVER_MSG& msg, XSTRING& out);
    bool                  MessageMatches            (XTRACESERVER_MSG& msg, int levelfilter, XSTRING& textfilter);

  private:

    void                  Clean                     ();

    XTRACESERVER*         traceserver;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void    Call_TraceServer_Ini              (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_IniUART          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_End              (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_IsOpen           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_Clear            (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_GetCount         (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_GetDroppedCount  (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_SetMaxMessages   (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_Pop              (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_WaitPop          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_Peek             (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_TraceServer_Get              (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
