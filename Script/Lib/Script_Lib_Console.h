/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Console.h
* 
* @class      SCRIPT_LIB_CONSOLE
* @brief      Script Library Console
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

#include "Script_Lib.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_CONSOLE  __L("Console")

#define SCRIPT_SET_LIB_CONSOLE(script, appconsole)  { SCRIPT_LIB_CONSOLE* lib = (SCRIPT_LIB_CONSOLE*)script->GetLibrary(SCRIPT_LIB_NAME_CONSOLE); \
                                                      if(lib)                                                                                     \
                                                        {                                                                                         \
                                                          lib->SetConsole(appconsole);                                                            \
                                                        }                                                                                         \
                                                    }



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class XCONSOLE;
class SCRIPT;

class SCRIPT_LIB_CONSOLE : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_CONSOLE      ();
    virtual              ~SCRIPT_LIB_CONSOLE      ();

    bool                  AddLibraryFunctions     (SCRIPT* script);

    XCONSOLE*             GetConsole              ();
    bool                  SetConsole              (XCONSOLE* console);

  private:

    void                  Clean                   ();

    XCONSOLE*             console;
    bool                  consoleowned;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void    Call_Console_GetChar                  (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Console_PutChar                  (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void    Call_Console_Printf                   (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);



