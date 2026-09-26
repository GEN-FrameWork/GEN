/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Language_Javascript.h
* 
* @class      SCRIPT_LANGUAGE_JAVASCRIPT
* @brief      Script Language Javascript interpreter class
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

#include "duk_config.h"
#include "duktape.h"

#include "Script.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum SCRIPT_LNG_JAVASCRIPT_ERRORCODE
{
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_ERROR             = SCRIPT_ERRORCODE_OWN  ,  // Error
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_EVAL_ERROR                                ,  // EvalError
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_RANGE_ERROR                               ,  // RangeError
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_REFERENCE_ERROR                           ,  // ReferenceError
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_SYNTAX_ERROR                              ,  // SyntaxError
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_TYPE_ERROR                                ,  // TypeError
  SCRIPT_LNG_JAVASCRIPT_ERRORCODE_URI_ERROR                                 ,  // URIError
};

#define SCRIPT_LNG_JAVASCRIPT_MAINFUNCTIONNAME   "main"


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SCRIPT_LNG_JAVASCRIPT : public SCRIPT
{
  public:
                                        SCRIPT_LNG_JAVASCRIPT       ();
    virtual                            ~SCRIPT_LNG_JAVASCRIPT       ();

    int                                 Run                         (int* returnval = NULL);

    bool                                AddLibraryFunction          (SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY ptrfunction);

    bool                                HaveError                   (int errorcode);

  private:

    bool                                HaveMainFunction            ();

    static duk_ret_t                    LibraryCallBack             (duk_context* context);
    static void                         FatalErrorHandler           (void* udata, const char* msg);

    void                                Clean                       ();

    duk_context*                        context;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





