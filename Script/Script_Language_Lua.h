/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Language_Lua.h
* 
* @class      SCRIPT_LANGUAGE_LUA
* @brief      Script Language LUA interpreter class
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

#include <lua.hpp>

#include "Script.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum SCRIPT_LNG_LUA_ERRORCODE
{
  SCRIPT_LNG_LUA_ERRORCODE_SYNTAX         = SCRIPT_ERRORCODE_OWN  ,   // "Syntax error"
  SCRIPT_LNG_LUA_ERRORCODE_YIELD                                  ,
  SCRIPT_LNG_LUA_ERRORCODE_RUN                                    ,
  SCRIPT_LNG_LUA_ERRORCODE_MEM                                    ,
  SCRIPT_LNG_LUA_ERRORCODE_GCMM                                   ,
  SCRIPT_LNG_LUA_ERRORCODE_ERRERR                                 ,
};

#define SCRIPT_LNG_LUA_MAINFUNCTIONNAME   "main"



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SCRIPT_LNG_LUA : public SCRIPT
{
  public:
                                        SCRIPT_LNG_LUA          ();
    virtual                            ~SCRIPT_LNG_LUA          ();
    
    int                                 Run                     (int* returnval = NULL);

    bool                                AddLibraryFunction      (SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY ptrfunction);

    bool                                HaveError               (int errorcode);
    bool                                HaveError               (XSTRING& currenttoken, int errorcode);

  private:

    bool                                HaveMainFunction        ();
    void                                Clean                   ();

    lua_State*                          state;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


int     LUA_LibraryCallBack       (lua_State* state);



