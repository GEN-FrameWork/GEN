/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Log.h
* 
* @class      SCRIPT_LIB_LOG
* @brief      Script Library Log
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

#include "XLog.h"
#include "Script_Lib.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_LOG  _L("Log")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class XCONSOLE;    
class XLOG;
class SCRIPT;

class SCRIPT_LIB_LOG : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_LOG          ();
    virtual              ~SCRIPT_LIB_LOG          ();

    bool                  AddLibraryFunctions     (SCRIPT* script);

    XLOGBASE*             GetLog                  ();
    void                  SetLog                  (XLOGBASE* log);
    
  private:

    void                  Clean                   ();

    XLOGBASE*             log;


};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void    Call_Log_Ini                  (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Log_CFG_SetLimit         (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Log_CFG_SetFilters       (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Log_CFG_SetBackup        (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Log_AddEntry             (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);



