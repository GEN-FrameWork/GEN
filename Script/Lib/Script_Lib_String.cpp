/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_String.cpp
* 
* @class      SCRIPT_LIB_STRING
* @brief      Script Library String
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

#include "Script_Lib_String.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_STRING::SCRIPT_LIB_STRING()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_STRING::SCRIPT_LIB_STRING() : SCRIPT_LIB(SCRIPT_LIB_NAME_STRING)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_STRING::~SCRIPT_LIB_STRING()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_STRING::~SCRIPT_LIB_STRING()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_STRING::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_STRING::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) 
    {
      return false;
    }

  this->script = script;

  script->AddLibraryFunction(this, __L("AddString")       , Call_AddString);
  script->AddLibraryFunction(this, __L("FindString")      , Call_FindString);
  script->AddLibraryFunction(this, __L("CompareString")   , Call_CompareString);
  script->AddLibraryFunction(this, __L("ReplaceString")   , Call_ReplaceString);
  script->AddLibraryFunction(this, __L("ReplaceAllString"), Call_ReplaceAllString);
  script->AddLibraryFunction(this, __L("SPrintf")         , Call_SPrintf);
  script->AddLibraryFunction(this, __L("GetStringSize")   , Call_GetStringSize);
  script->AddLibraryFunction(this, __L("IsEmptyString")   , Call_IsEmptyString);
  script->AddLibraryFunction(this, __L("SubString")       , Call_SubString);
  script->AddLibraryFunction(this, __L("SubStringFrom")   , Call_SubStringFrom);
  script->AddLibraryFunction(this, __L("ExtractBetween")  , Call_ExtractBetween);
  script->AddLibraryFunction(this, __L("TrimString")      , Call_TrimString);
  script->AddLibraryFunction(this, __L("ToUpperString")   , Call_ToUpperString);
  script->AddLibraryFunction(this, __L("ToLowerString")   , Call_ToLowerString);
  script->AddLibraryFunction(this, __L("GetCharString")   , Call_GetCharString);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_STRING::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_STRING::Clean()
{

}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_AddString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_AddString
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_AddString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = (XSTRING*)params->Get(0)->GetData();
  XSTRING* string2 = (XSTRING*)params->Get(1)->GetData();

  if(string1 && string2) (*string1)+=(*string2);

  (*returnvalue) = (*string1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FindString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_FindString
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* @note       FindString(haystack, needle, ignorecase [, startindex=0]) → index or -1
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FindString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING haystack;
  XSTRING needle;
  bool    ignorecase = false;
  int     startindex = 0;

  if(!library->GetParamConverted(params->Get(0), haystack))
    {
      (*returnvalue) = -1;
      return;
    }

  if(!library->GetParamConverted(params->Get(1), needle))
    {
      (*returnvalue) = -1;
      return;
    }

  library->GetParamConverted(params->Get(2), ignorecase);

  if(params->GetSize() >= 4)
    {
      library->GetParamConverted(params->Get(3), startindex);
      if(startindex < 0) startindex = 0;
    }

  (*returnvalue) = haystack.Find(needle.Get(), ignorecase, startindex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_CompareString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_CompareString
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_CompareString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* string1    = (XSTRING*)params->Get(0)->GetData();
  XSTRING* string2    = (XSTRING*)params->Get(1)->GetData();
  bool     ignorecase = (bool)params->Get(2)->GetData(); 

  if(string1 && string2) 
    {
      (*returnvalue) = (bool)(string1->Compare(string2->Get(), ignorecase)==0?true:false);
    }
   else 
    { 
      (*returnvalue) = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ReplaceString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ReplaceString
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ReplaceString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* string     = (XSTRING*)params->Get(0)->GetData();
  XSTRING* tofind     = (XSTRING*)params->Get(1)->GetData();
  XSTRING* toreplace  = (XSTRING*)params->Get(2)->GetData();

  if(string && tofind && toreplace) 
    {
      if(string->ReplaceFirst(tofind->Get(), toreplace->Get()) != XSTRING_NOTFOUND)
        {
          (*returnvalue) = (*string);
        }
    }   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ReplaceAllString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ReplaceAllString
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* @note       ReplaceAllString(text, find, replace) → new string with all occurrences replaced (input not required to be mutable)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ReplaceAllString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;
  XSTRING tofind;
  XSTRING toreplace;

  if(!library->GetParamConverted(params->Get(0), text))   return;
  if(!library->GetParamConverted(params->Get(1), tofind)) return;
  if(!library->GetParamConverted(params->Get(2), toreplace)) return;

  text.Replace(tofind.Get(), toreplace.Get());
  (*returnvalue) = text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_SPrintf(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_SPrintf
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_SPrintf(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XVARIANT  variantout = (*params->Get(0));
  XCHAR*    out = variantout;
  XSTRING  _out = out;

  XVARIANT  variantmask = (*params->Get(1));
  XCHAR*    mask = variantmask;

  if(!mask)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING outstring;
  XSTRING string;

  int paramindex = 2;
  int c          = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case '%' : {
                        #define MAXTEMPOSTR 32

                        XCHAR param[MAXTEMPOSTR];

                        int  nparam = 1;
                        bool end    = false;

                        memset(param, 0, MAXTEMPOSTR*sizeof(XCHAR));
                        param[0] = '%';

                        c++;

                        do{ string.Empty();

                            param[nparam] = mask[c];
                            nparam++;

                            switch(mask[c])
                              {
                                case __C('c')   :
                                case __C('C')   :
                                case __C('d')   :
                                case __C('i')   :
                                case __C('o')   :
                                case __C('u')   :
                                case __C('x')   :
                                case __C('X')   : { int value = 0;
                                                    library->GetParamConverted(params->Get(paramindex), value);
                                                    string.Format(param, value);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('f')   : { float value = 0;
                                                    library->GetParamConverted(params->Get(paramindex), value);
                                                    string.Format(param, value);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    paramindex++;
                                                    // Pass data as a string value — do not re-parse '%' inside it.
                                                    string = (XCHAR*)variantparam;
                                                    end = true;
                                                  }
                                                  break;

                                case __C('%')   : string = __L("%");
                                                  end = true;
                                                  break;

                                case __C('\0')  : end = true;
                                                  break;

                                      default   : break;
                              }

                            c++;

                          } while(!end);
                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
      string.Empty();
    }

  _out = outstring;
  (*returnvalue) = outstring;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_GetStringSize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_GetStringSize
* @ingroup    SCRIPT
* 
* @note       GetStringSize(text) → length in characters
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_GetStringSize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = 0;
      return;
    }

  (*returnvalue) = (int)text.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_IsEmptyString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_IsEmptyString
* @ingroup    SCRIPT
* 
* @note       IsEmptyString(text) → bool
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_IsEmptyString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = true;
      return;
    }

  (*returnvalue) = text.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_SubString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_SubString
* @ingroup    SCRIPT
* 
* @note       SubString(text, start, end) → characters in [start, end). Clamps to string bounds; empty if invalid range.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_SubString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;
  int     start = 0;
  int     end   = 0;
  XSTRING result;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = result;
      return;
    }

  library->GetParamConverted(params->Get(1), start);
  library->GetParamConverted(params->Get(2), end);

  int size = (int)text.GetSize();

  if(start < 0) start = 0;
  if(end   < 0) end   = 0;
  if(start > size) start = size;
  if(end   > size) end   = size;

  if(start < end)
    {
      text.Copy(start, end, result);
    }

  (*returnvalue) = result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_SubStringFrom(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_SubStringFrom
* @ingroup    SCRIPT
* 
* @note       SubStringFrom(text, start) → characters from start to end of string
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_SubStringFrom(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;
  int     start = 0;
  XSTRING result;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = result;
      return;
    }

  library->GetParamConverted(params->Get(1), start);

  int size = (int)text.GetSize();

  if(start < 0) start = 0;
  if(start > size) start = size;

  text.Copy(start, result);
  (*returnvalue) = result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ExtractBetween(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ExtractBetween
* @ingroup    SCRIPT
* 
* @note       ExtractBetween(text, startmark, endmark [, ignorecase=false [, from=0]]) → text between marks (exclusive).
*             Empty string if either mark is missing. Marks themselves are not included.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ExtractBetween(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;
  XSTRING startmark;
  XSTRING endmark;
  bool    ignorecase = false;
  int     from       = 0;
  XSTRING result;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = result;
      return;
    }

  if(!library->GetParamConverted(params->Get(1), startmark))
    {
      (*returnvalue) = result;
      return;
    }

  if(!library->GetParamConverted(params->Get(2), endmark))
    {
      (*returnvalue) = result;
      return;
    }

  if(params->GetSize() >= 4)
    {
      library->GetParamConverted(params->Get(3), ignorecase);
    }

  if(params->GetSize() >= 5)
    {
      library->GetParamConverted(params->Get(4), from);
      if(from < 0) from = 0;
    }

  int start = text.Find(startmark.Get(), ignorecase, from);
  if(start == XSTRING_NOTFOUND)
    {
      (*returnvalue) = result;
      return;
    }

  start += (int)startmark.GetSize();

  int end = text.Find(endmark.Get(), ignorecase, start);
  if(end == XSTRING_NOTFOUND)
    {
      (*returnvalue) = result;
      return;
    }

  if(start < end)
    {
      text.Copy(start, end, result);
    }

  (*returnvalue) = result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TrimString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_TrimString
* @ingroup    SCRIPT
* 
* @note       TrimString(text) → copy with leading/trailing whitespace (space, tab, CR, LF) removed
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TrimString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = text;
      return;
    }

  text.DeleteNoCharacters(__L(" \t\r\n"), 0, XSTRINGCONTEXT_FROM_FIRST);
  text.DeleteNoCharacters(__L(" \t\r\n"), 0, XSTRINGCONTEXT_TO_END);

  (*returnvalue) = text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ToUpperString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ToUpperString
* @ingroup    SCRIPT
* 
* @note       ToUpperString(text) → uppercase copy
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ToUpperString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = text;
      return;
    }

  text.ToUpperCase();
  (*returnvalue) = text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ToLowerString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ToLowerString
* @ingroup    SCRIPT
* 
* @note       ToLowerString(text) → lowercase copy
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ToLowerString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = text;
      return;
    }

  text.ToLowerCase();
  (*returnvalue) = text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_GetCharString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_GetCharString
* @ingroup    SCRIPT
* 
* @note       GetCharString(text, index) → one-character string, or empty if out of range
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_GetCharString(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING text;
  int     index = 0;
  XSTRING result;

  if(!library->GetParamConverted(params->Get(0), text))
    {
      (*returnvalue) = result;
      return;
    }

  library->GetParamConverted(params->Get(1), index);

  if((index >= 0) && (index < (int)text.GetSize()) && text.Get())
    {
      result.Set(text.Get()[index]);
    }

  (*returnvalue) = result;
}
