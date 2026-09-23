/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Language_Javascript.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "Script_Language_Javascript.h"

#include <cmath>

#include "XString.h"

#include "Script_XEvent.h"
#include "Script_Lib.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LNG_JAVASCRIPT::SCRIPT_LNG_JAVASCRIPT() : SCRIPT()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LNG_JAVASCRIPT::SCRIPT_LNG_JAVASCRIPT() : SCRIPT()
{
  Clean();

  type = SCRIPT_TYPE_JAVASCRIPT;

  //context = duk_create_heap(NULL, NULL, NULL, (void*)this, SCRIPT_LNG_JAVASCRIPT::FatalErrorHandler);
  context = duk_create_heap(NULL, NULL, NULL, (void*)this, NULL);

  AddInternalLibraries();

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LNG_JAVASCRIPT::~SCRIPT_LNG_JAVASCRIPT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LNG_JAVASCRIPT::~SCRIPT_LNG_JAVASCRIPT()
{
  if(context)
    {
      //duk_pop(context);
      duk_destroy_heap(context);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SCRIPT_LNG_JAVASCRIPT::Run(int* returnval)
* @brief      Run operation
* @ingroup    SCRIPT
* 
* @param[in]  returnval : Returnval pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SCRIPT_LNG_JAVASCRIPT::Run(int* returnval)
{
  errorcode    = SCRIPT_ERRORCODE_NONE;
  iscancelexec = false;

  XSTRING _script = script;

  if(HaveMainFunction())
    {
      XSTRING mainfunctionname = SCRIPT_LNG_JAVASCRIPT_MAINFUNCTIONNAME;
      _script.AddFormat(__L("\n%s()\n"), mainfunctionname.Get());
    }

  XBUFFER charstr;
  
  _script.ConvertToASCII(charstr);
   
  duk_int_t error =  duk_peval_string(context, charstr.GetPtrChar());
  if(error) 
    {
      HaveError(DUK_ERR_ERROR);  
    }
  
  if(returnval) 
    {
      // peval leaves the result at stack top (-1). Non-numeric results are not a hard
      // peval failure; reject them explicitly when the caller asked for an int.
      if(!duk_is_number(context, -1))
        {
          HaveError(DUK_ERR_TYPE_ERROR);
          (*returnval) = 0;
        }
       else
        {
          (*returnval) = (int)duk_get_number(context, -1);
        }
    }

  duk_pop(context);

  return errorcode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LNG_JAVASCRIPT::AddLibraryFunction(SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY ptrfunction)
* @brief      Add library function
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  name : Name to use.
* @param[in]  ptrfunction : Ptrfunction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LNG_JAVASCRIPT::AddLibraryFunction(SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY ptrfunction)
{
  XSTRING namefunction;
  XSTRING ptrfunctionstr;

  namefunction = name;

  XBUFFER charstr;
  
  namefunction.ConvertToASCII(charstr);
  
  duk_push_c_function(context, SCRIPT_LNG_JAVASCRIPT::LibraryCallBack, DUK_VARARGS);

  duk_push_pointer(context, (void *)ptrfunction);
  duk_put_prop_string(context, -2, "\xFF""_ptr");
  duk_put_global_string(context, charstr.GetPtrChar());

  return SCRIPT::AddLibraryFunction(library, name, ptrfunction);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LNG_JAVASCRIPT::HaveError(int _errorcode)
* @brief      Have error
* @ingroup    SCRIPT
* 
* @param[in]  _errorcode : Errorcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LNG_JAVASCRIPT::HaveError(int _errorcode)
{
  XSTRING   currenttoken;
  XPATH     namefile;
  int       errorcode    = _errorcode;
  XSTRING   errorstring;
  XSTRING   errorline;
  int       nline        = 0;

  currenttoken = duk_safe_to_string(context, -1);

  if(currenttoken.Find(__L("EvalError")      , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_EVAL_ERROR;
  if(currenttoken.Find(__L("RangeError")     , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_RANGE_ERROR;
  if(currenttoken.Find(__L("ReferenceError") , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_REFERENCE_ERROR;
  if(currenttoken.Find(__L("SyntaxError")    , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_SYNTAX_ERROR;
  if(currenttoken.Find(__L("TypeError")      , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_TYPE_ERROR;
  if(currenttoken.Find(__L("URIError")       , false) != XSTRING_NOTFOUND) errorcode = SCRIPT_LNG_JAVASCRIPT_ERRORCODE_URI_ERROR;

  namefile.Set(namescript);
  namefile.SetOnlyNamefileExt();

  SCRIPT_XEVENT xevent(this, SCRIPT_XEVENT_TYPE_ERROR);

  xevent.SetError(errorcode);
  xevent.GetErrorText()->Set(currenttoken);
  xevent.GetCurrentToken()->Set(currenttoken);
  xevent.SetNLine(nline);

  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Script [%s] ERROR %d:\"%s\"") , namefile.Get(), errorcode, currenttoken.Get());

  PostEvent(&xevent);

  this->errorcode  = errorcode;
  iscancelexec     = true;


  return (errorcode == SCRIPT_ERRORCODE_NONE)?false:true;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LNG_JAVASCRIPT::HaveMainFunction()
* @brief      Have main function
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LNG_JAVASCRIPT::HaveMainFunction()
{
  XSTRING mainfunctionname;
  XSTRING script;
  int     start = 0;
  int     end   = 0;
  XSTRING line;

  script = GetScript()->Get();
  if(script.IsEmpty()) return false;

  mainfunctionname = SCRIPT_LNG_JAVASCRIPT_MAINFUNCTIONNAME;

  int index = GetScript()->Find(mainfunctionname, false);
  if(index == XSTRING_NOTFOUND) return false;

  for(start=index; start>0; start--)
    {
      if((script.Get()[start] == __C('\r')) || (script.Get()[start] == __C('\n')))
        {
          break;
        }
    }

  for(end=index; end<(int)script.GetSize() ; end++)
    {
      if(script.Get()[end] == __C(')'))
        {
          break;
        }
    }

  script.Copy(start, end+1, line);

  line.DeleteCharacter(__C('\n'));
  line.DeleteCharacter(__C('\r'));
  line.DeleteCharacter(__C(' '));

  XSTRING searchmask;

  searchmask.Format(__L("function%s()"), mainfunctionname.Get());

  if(!line.Compare(searchmask)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         duk_ret_t SCRIPT_LNG_JAVASCRIPT::LibraryCallBack(duk_context* context)
* @brief      Library call back
* @ingroup    SCRIPT
* 
* @param[in]  context : Context object to use.
* 
* @return     duk_ret_t : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
duk_ret_t SCRIPT_LNG_JAVASCRIPT::LibraryCallBack(duk_context* context)
{
  XVECTOR<XVARIANT*>    params;
  XVARIANT              returnvalue;
  int                   nreturnvalues = 0;
  SCRIPT_LNG_JAVASCRIPT*    script        = NULL;
  duk_memory_functions  memoryfuncs;
  void*                 funcptr       = NULL;

  duk_get_memory_functions(context, &memoryfuncs);

  script = (SCRIPT_LNG_JAVASCRIPT*)memoryfuncs.udata;
  if(!script) return 0;

  duk_push_current_function(context);
  duk_get_prop_string(context, -1, "\xFF""_ptr");
  funcptr = (void*)duk_get_pointer(context, -1);
  if(!funcptr) 
    {
      return 0;
    }

  SCRIPT_LIB_FUNCTION* libfunction = script->GetLibraryFunction(funcptr);

  if(!libfunction) 
    {
      return 0;
    }

  if(!libfunction->GetFunctionLibrary()) 
    {
      return 0;
    }

  duk_pop_2(context);

  duk_idx_t          nargs;
  duk_idx_t          c;

  nargs = duk_get_top(context);

  for(c = 0; c < nargs; c++)
    {
      XVARIANT* variant = GEN_NEW XVARIANT();
      if(variant)
        {
          switch((int)duk_get_type(context, c))
            {
              case DUK_TYPE_NONE           :  // no value, e.g. invalid index
                                              break;

              case DUK_TYPE_UNDEFINED      :  // ECMAScript undefined
                                              break;

              case DUK_TYPE_NULL           :  // ECMAScript null
                                              //(*variant) = NULL;
                                              break;

              case DUK_TYPE_BOOLEAN        :  // ECMAScript boolean: 0 or 1
                                              (*variant) = (duk_require_boolean(context, c)?true:false);
                                              break;

              case DUK_TYPE_NUMBER         :  // ECMAScript number: double
                                              (*variant) = (double)duk_require_number(context, c);
                                              break;

              case DUK_TYPE_STRING         :  // ECMAScript string: CESU-8 / extended UTF-8 encoded
                                              (*variant) = (char*)duk_require_string(context, c);
                                              break;

              case DUK_TYPE_OBJECT         :  // ECMAScript object: includes objects, arrays, functions, threads
                                              break;

              case DUK_TYPE_BUFFER         :  // fixed or dynamic, garbage collected byte buffer
                                              break;

              case DUK_TYPE_POINTER        :  // raw void pointer
                                              break;

              case DUK_TYPE_LIGHTFUNC      :  // lightweight function pointer
                                              break;
            }

           params.Add(variant);
        }
    }

  libfunction->GetFunctionLibrary()(libfunction->GetLibrary(), script, &params, &returnvalue);

  params.DeleteContents();
  params.DeleteAll();


  switch(returnvalue.GetType())
    {
      case XVARIANT_TYPE_NULL          :  duk_push_null(context); 
                                          break;

      case XVARIANT_TYPE_BOOLEAN       :  if((bool)returnvalue)
                                            {
                                              duk_push_true(context);
                                            }
                                           else
                                            { 
                                              duk_push_false(context);
                                            }

                                          nreturnvalues++;
                                          break;
      
      case XVARIANT_TYPE_INTEGER       :  duk_push_int(context, (int)(returnvalue));     nreturnvalues++;   break;
      case XVARIANT_TYPE_CHAR          :  duk_push_int(context, (int)(returnvalue));     nreturnvalues++;   break;
      case XVARIANT_TYPE_XCHAR         :                                                                    break;
      case XVARIANT_TYPE_FLOAT         :  duk_push_number(context, (double)(float)returnvalue);  nreturnvalues++;  break;
      case XVARIANT_TYPE_DOUBLE        :  duk_push_number(context, (double)returnvalue);         nreturnvalues++;  break;

      case XVARIANT_TYPE_STRING        : { XSTRING stringreturnvalue;

                                           stringreturnvalue = (const XSTRING&)returnvalue;

                                           XBUFFER charstr;
 
                                           stringreturnvalue.ConvertToASCII(charstr); 
                                           duk_push_string(context, charstr.GetPtrChar());
                                           
                                           nreturnvalues++;
                                         }
                                         break;

      case XVARIANT_TYPE_DATE          : break;
      case XVARIANT_TYPE_TIME          : break;
      case XVARIANT_TYPE_DATETIME      : break;
                    default            : break;
    }


  // Let Lua know how many return values we've passed
  return nreturnvalues;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LNG_JAVASCRIPT::FatalErrorHandler(void* udata, const char* msg)
* @brief      Fatal error handler
* @ingroup    SCRIPT
* 
* @param[in]  udata : Udata pointer to use.
* @param[in]  msg : Msg value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LNG_JAVASCRIPT::FatalErrorHandler(void* udata, const char* msg)
{
  SCRIPT_LNG_JAVASCRIPT* script  = (SCRIPT_LNG_JAVASCRIPT*)udata;
  if(!script) return;

  XSTRING message;

  message = msg;

  abort();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LNG_JAVASCRIPT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LNG_JAVASCRIPT::Clean()
{
  context = NULL;
}


#if !defined(COMPILER_CLANG_CL) && !defined(COMPILER_CLANG) && !defined(COMPILER_GCC)
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double __builtin_inf(void)
* @brief      Builtin inf
* @ingroup    SCRIPT
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double __builtin_inf(void)
{
  return (double)0xFFFFFFFFFFFFFFFF;  
}
#endif
