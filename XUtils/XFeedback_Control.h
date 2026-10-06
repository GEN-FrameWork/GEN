/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFeedback_Control.h
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

#pragma once

#ifdef XFEEDBACK_CONTROL_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef BUILDER
#include <cstdarg>
#endif

#include "XVector.h"
#include "XMap.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFEEDBACK_CODE
{
  XFEEDBACK_CODE_UNKNOWN                     = 0 ,  
  XFEEDBACK_CODE_INVALIDPARAMFUNCTION            , 
  XFEEDBACK_CODE_INVALIDMEMORYALLOCATION         ,
  XFEEDBACK_CODE_ERRORINFUNCTION                 ,

  XFEEDBACK_CODE_MAXNCODES
};


#define XFEEDBACK_MAXNAMEMODULESIZE               32
#define XFEEDBACK_CONTROL_MAXBLOCKASSIGN          256

#define XFEEDBACK_CONTROL_ACTIVATED               if(XFeedback_Control) XFeedback_Control->Activate(true);
#define XFEEDBACK_CONTROL_DEACTIVATED             if(XFeedback_Control) XFeedback_Control->Activate(false);

#define XFEEDBACK_ADD(code, ...)                  if(XFeedback_Control) XFeedback_Control->AddFeedbak(GEN_MODULE_EXEC, code, GEN_LINE_EXEC, ## __VA_ARGS__);

#define XFEEDBACK_CONTROL_DISPLAYFEEDBACK         if(XFeedback_Control) XFeedback_Control->DisplayAll();

#define XFEEDBACK_CONTROL_DELETERESOURCES         if(XFeedback_Control) XFeedback_Control->DeleteAllResources();

#define XFEEDBACK_CONTROL_CREATE                  XFeedback_Control = GEN_NEW XFEEDBACK_CONTROL();          
#define XFEEDBACK_CONTROL_DELETE                  if(XFeedback_Control)                                 \
                                                    {                                                   \
                                                      GEN_DELETE XFeedback_Control;                         \
                                                      XFeedback_Control = NULL;                         \
                                                    }
  
#define XFEEDBACK_CODE_REFTEXT                    feedback_code_text_temporal                                 
#define XFEEDBACK_CODE_TEXT                       feedback_code_text_temporal.Get()                                 
#define XFEEDBACK_CODE_CREATETEXT(text, ...)      XSTRING XFEEDBACK_CODE_REFTEXT;                          \
                                                  XFEEDBACK_CODE_REFTEXT.Format(text, ## __VA_ARGS__);   

#define XFEEDBACK_CONTROL_LOG_SECTIONID           _L("Feedback")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XFEEDBACK
{
public:
                                      XFEEDBACK                   ();
    virtual                          ~XFEEDBACK                   ();

    XFEEDBACK_CODE                    GetCode                     ();
    void                              SetCode                     (XFEEDBACK_CODE code);

    XSTRING*                          GetModuleName               ();

    int                               GetModuleLine               ();
    void                              SetModuleLine               (int moduleline);

    XSTRING*                          GetText                     ();
    
  private:

    void                              Clean                       ();
  
    XFEEDBACK_CODE                    code;
    XSTRING*                          modulename;
    int                               moduleline;
    XSTRING*                          text;
};

class XFEEDBACK_CONTROL
{
  public:
                                      XFEEDBACK_CONTROL           ();
    virtual                          ~XFEEDBACK_CONTROL           ();

    bool                              IsActive                    ();
    bool                              Activate                    (bool isactive);

    bool                              AddFeedbak                  (char const* namefile, XFEEDBACK_CODE code, int line, ...); 
  
    bool                              DisplayAll                  ();

    bool                              DeleteAllResources          ();

  private:

    bool                              AddFeedbakText              (XFEEDBACK_CODE code, XCHAR* text);
    bool                              GetFeedbakText              (XFEEDBACK_CODE code, XSTRING& text);

    void                              Clean                       ();

    bool                              isactive;    
    XVECTOR<XFEEDBACK*>               feedbacks;   
    XMAP<XFEEDBACK_CODE, XSTRING*>    feedbacks_text;   
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

extern XFEEDBACK_CONTROL* XFeedback_Control;


#else

#ifdef BUILDER

  #define XFEEDBACK_CONTROL_ACTIVATED               
  #define XFEEDBACK_CONTROL_DEACTIVATED            
  #define XFEEDBACK_ADD  
  #define XFEEDBACK_CONTROL_DISPLAYFEEDBACK
  #define XFEEDBACK_CONTROL_DELETERESOURCES
  #define XFEEDBACK_CONTROL_CREATE
  #define XFEEDBACK_CONTROL_DELETE
  #define XFEEDBACK_CODE_REFTEXT  
  #define XFEEDBACK_CODE_TEXT                
  #define XFEEDBACK_CODE_CREATETEXT
                                                    
#else

  #define XFEEDBACK_CONTROL_ACTIVATED               
  #define XFEEDBACK_CONTROL_DEACTIVATED            
  #define XFEEDBACK_ADD(code, ...)                  
  #define XFEEDBACK_CONTROL_DISPLAYFEEDBACK    
  #define XFEEDBACK_CONTROL_DELETERESOURCES
  #define XFEEDBACK_CONTROL_CREATE
  #define XFEEDBACK_CONTROL_DELETE
  #define XFEEDBACK_CODE_REFTEXT                
  #define XFEEDBACK_CODE_TEXT                
  #define XFEEDBACK_CODE_CREATETEXT(text, ...)

#endif

#endif



