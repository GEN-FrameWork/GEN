/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFeedback_Control.cpp
* 
* @class      XFEEDBACK_CONTROL
* @brief      eXtended Utils Feedback Control class
* @ingroup    XUTILS
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



#ifdef XFEEDBACK_CONTROL_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFeedback_Control.h"

#include <stdlib.h>
#include <string.h>

#include "XFactory.h"
#include "XString.h"
#include "XLog.h"
#include "XTrace.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XFEEDBACK_CONTROL* XFeedback_Control = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFEEDBACK::XFEEDBACK()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFEEDBACK::XFEEDBACK()
{
  Clean();

  modulename = GEN_NEW XSTRING();  
  text       = GEN_NEW XSTRING();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFEEDBACK::~XFEEDBACK()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFEEDBACK::~XFEEDBACK()
{
  if(modulename)
    {
      GEN_DELETE modulename;  
    }

  if(text)
    {
      GEN_DELETE text;
    }       

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFEEDBACK_CODE XFEEDBACK::GetCode()
* @brief      get code
* @ingroup    XUTILS
* 
* @return     XFEEDBACK_CODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFEEDBACK_CODE XFEEDBACK::GetCode()
{
  return code;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFEEDBACK::SetCode(XFEEDBACK_CODE code)
* @brief      set code
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFEEDBACK::SetCode(XFEEDBACK_CODE code)
{
  this->code = code;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFEEDBACK::GetModuleName()
* @brief      get module name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFEEDBACK::GetModuleName()
{
  return modulename;
}

               
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFEEDBACK::GetModuleLine()
* @brief      get module line
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFEEDBACK::GetModuleLine()
{
  return moduleline;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFEEDBACK::SetModuleLine(int moduleline)
* @brief      set module line
* @ingroup    XUTILS
* 
* @param[in]  moduleline : Moduleline value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFEEDBACK::SetModuleLine(int moduleline)
{
  this->moduleline = moduleline;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFEEDBACK::GetText()
* @brief      get text
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFEEDBACK::GetText()
{
  return text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFEEDBACK::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFEEDBACK::Clean()
{
  code        = XFEEDBACK_CODE_UNKNOWN;
  modulename  = NULL;
  moduleline  = 0;
  text        = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFEEDBACK_CONTROL::XFEEDBACK_CONTROL()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFEEDBACK_CONTROL::XFEEDBACK_CONTROL()
{
  Clean();

  isactive = true;

  AddFeedbakText(XFEEDBACK_CODE_UNKNOWN, __L("Unknown"));
  AddFeedbakText(XFEEDBACK_CODE_INVALIDPARAMFUNCTION, __L("The %d parameter [%s] function [%s] call was invalid."));
  AddFeedbakText(XFEEDBACK_CODE_INVALIDMEMORYALLOCATION, __L("Invalid memory allocation in variable [%s] of function [%s]."));
  AddFeedbakText(XFEEDBACK_CODE_ERRORINFUNCTION, __L("Error in function [%s]: %s"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFEEDBACK_CONTROL::~XFEEDBACK_CONTROL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFEEDBACK_CONTROL::~XFEEDBACK_CONTROL()
{
  DeleteAllResources();

  isactive = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::IsActive()
* @brief      Is active
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::Activate(bool isactive)
* @brief      Activate
* @ingroup    XUTILS
* 
* @param[in]  isactive : Isactive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::Activate(bool isactive)
{
  if(this->isactive == isactive) return false;

  this->isactive = isactive;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::AddFeedbak(char const* namefile, XFEEDBACK_CODE code, int line, ...)
* @brief      add feedbak
* @ingroup    XUTILS
* 
* @param[in]  namefile : Namefile value.
* @param[in]  code : Code value.
* @param[in]  line : Line value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::AddFeedbak(char const* namefile, XFEEDBACK_CODE code, int line, ...)
{
  if(!isactive)
    {
      return false;
    }

  XFEEDBACK* feedback = GEN_NEW XFEEDBACK();
  if(feedback)
    {
      feedback->SetCode(code);
      feedback->GetModuleName()->Set(namefile);
      feedback->SetModuleLine(line);

      XSTRING text;
      
      if(GetFeedbakText(code, text))
        {
          if(!text.IsEmpty())
            {              
              va_list arg;

              va_start(arg, line);
           
              switch(code)
                {
                  case XFEEDBACK_CODE_UNKNOWN                     : feedback->GetText()->Set(text);  
                                                                    break;

                  case XFEEDBACK_CODE_INVALIDPARAMFUNCTION        : { int indexparam      = va_arg(arg, int);
                                                                      XCHAR* nameparam    = va_arg(arg, XCHAR*);
                                                                      XCHAR* namefunction = va_arg(arg, XCHAR*);

                                                                      if(nameparam && namefunction)
                                                                        {
                                                                          feedback->GetText()->Format(text.Get(), indexparam, nameparam, namefunction);
                                                                        }
                                                                    }
                                                                    break;  

                  case XFEEDBACK_CODE_INVALIDMEMORYALLOCATION     : { XCHAR* namevariable = va_arg(arg, XCHAR*);
                                                                      XCHAR* namefunction = va_arg(arg, XCHAR*);

                                                                      if(namevariable && namefunction)
                                                                        {
                                                                          feedback->GetText()->Format(text.Get(), namevariable, namefunction);
                                                                        }
                                                                    }
                                                                    break;

                  case XFEEDBACK_CODE_ERRORINFUNCTION             : { XCHAR* namefunction = va_arg(arg, XCHAR*);
                                                                      XCHAR* errortext    = va_arg(arg, XCHAR*);

                                                                      if(namefunction && errortext)
                                                                        {
                                                                          feedback->GetText()->Format(text.Get(), namefunction, errortext);
                                                                        }
                                                                    }
                                                                    break;

                                                      default     : feedback->GetText()->Set(text);  
                                                                    break;   
                }

              va_end(arg);

  

             GEN_XLOG.AddEntry(XLOGLEVEL_WARNING, XFEEDBACK_CONTROL_LOG_SECTIONID, false, __L("%05d line %d, module %s -> %s"), feedbacks.GetSize() + 1, feedback->GetModuleLine(), feedback->GetModuleName()->Get(), feedback->GetText()->Get());       


              
              return feedbacks.Add(feedback);
            }
        }
    }

  GEN_DELETE feedback;
  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::DisplayAll()
* @brief      display all
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::DisplayAll()
{     
  #ifdef XTRACE_ACTIVE
  
  XDWORD  nfeedback = feedbacks.GetSize();
  
/*
  XBYTE   level     = (!nfeedback?XTRACE_COLOR_BLUE:XTRACE_COLOR_RED);

  XTRACE_PRINT(__L(" "));
  XTRACE_PRINTHEADER((level | XTRACE_LEVEL_WITHCOLOR), (!nfeedback)?__L("ALL FEEDBACK ENTRYS"):__L("NOT FEEDBACK ENTRYS"));
  XTRACE_PRINT(__L(" "));
  */

  if(nfeedback)
    {
      XSTRING title;

      XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, __L(" "));
      XTRACE_PRINTHEADER((XTRACE_COLOR_PURPLE | XTRACE_LEVEL_WITHCOLOR), __L("ALL FEEDBACK ENTRYS"));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, __L(" "));
           
      title.Format(__L("%-8s %-5s %-72s %s"), __L("Index"), __L("Line"), __L("Module"), __L("Description"));  

      XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, __L("Number feedbacks  : %d"), nfeedback);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, __L(" "));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, title.Get());
      
      for(XDWORD c=0; c<nfeedback; c++)
        {
          XFEEDBACK* feedback = feedbacks.Get(c);

          if(feedback)
            {              
              XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, __L("%08d %05d %-72s %s"), c+1, feedback->GetModuleLine(), feedback->GetModuleName()->Get(), feedback->GetText()->Get());                                                                               
            }
        }

      XTRACE_PRINTHEADER((XTRACE_COLOR_PURPLE | XTRACE_LEVEL_WITHCOLOR), NULL);
    }

  #endif

  return true;
  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::DeleteAllResources()
* @brief      GEN_DELETE all resources
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::DeleteAllResources()
{
  feedbacks_text.DeleteElementContents();
  feedbacks_text.DeleteAll();

  feedbacks.DeleteContents();
  feedbacks.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::AddFeedbakText(XFEEDBACK_CODE code, XCHAR* text)
* @brief      add feedbak text
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* @param[in]  text : Text to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::AddFeedbakText(XFEEDBACK_CODE code, XCHAR* text)
{
  XSTRING* textcontainer;

  textcontainer = GEN_NEW XSTRING();
  if(textcontainer)
    {
      textcontainer->Set(text);
      return feedbacks_text.Add(code, textcontainer);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFEEDBACK_CONTROL::GetFeedbakText(XFEEDBACK_CODE code, XSTRING& text)
* @brief      get feedbak text
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* @param[in]  text : Text to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFEEDBACK_CONTROL::GetFeedbakText(XFEEDBACK_CODE code, XSTRING& text)
{
  if(feedbacks_text.IsEmpty())
    {
      return false;  
    }

  XSTRING* textcontainer = feedbacks_text.Get(code); 
  if(!textcontainer)
    {
      return false;
    }

  text = (*textcontainer);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFEEDBACK_CONTROL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFEEDBACK_CONTROL::Clean()
{
  isactive      = false;  
}






#endif



