/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_DevTest.h
* 
* @class      SCRIPT_LIB_DEVTEST
* @brief      Script Library Dev Test
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

#define SCRIPT_LIB_NAME_DEVTEST  _L("Developed Test")



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class XCONSOLE;
class SCRIPT;

class SCRIPT_LIB_DEVTEST : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_DEVTEST      ();
    virtual              ~SCRIPT_LIB_DEVTEST      ();

    bool                  AddLibraryFunctions     (SCRIPT* script);
  
  private:

    void                  Clean                   ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void    Call_DevTest_Func1                    (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);



