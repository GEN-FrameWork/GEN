/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProc.h
* 
* @class      MAINPROC
* @brief      Main Procedure class
* @ingroup    MAIN_PROCEDURE
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

#include <stdio.h>

#include "XVector.h"
#include "XPath.h"

#include "MainLibrary.h"

#include "APPFlowBase.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class MAINPROC;
typedef bool (*MAIN_FUNCTION_TYPE)(MAINPROC* main);


#ifndef MAIN_FUNCTION_PLATFORMINI
#define MAIN_FUNCTION_PLATFORMINI   NULL
#endif

#ifndef MAIN_FUNCTION_INI
#define MAIN_FUNCTION_INI           NULL
#endif

#ifndef MAIN_FUNCTION_UPDATE
#define MAIN_FUNCTION_UPDATE        NULL
#endif

#ifndef MAIN_FUNCTION_END
#define MAIN_FUNCTION_END           NULL
#endif

#ifndef MAIN_FUNCTION_PLATFORMEND
#define MAIN_FUNCTION_PLATFORMEND   NULL
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSTRING;
class APPFLOWMAIN;

class MAINPROC
{
  public:
                                  MAINPROC                ();
    virtual                      ~MAINPROC                ();

    virtual bool                  Ini                     (APPFLOWMAIN* appmain = NULL, APPFLOWBASE_MODE_TYPE applicationmode = APPFLOWBASE_MODE_TYPE_UNKNOWN);
    virtual bool                  Update                  ();
    virtual bool                  End                     ();

    void*                         GetHandle               ();
    void                          SetHandle               (void* handle);
	
		#ifndef MICROCONTROLLER
    bool                          CreateParams            (int nparams, char* params[]);
    bool                          CreateParams            (int nparams, XCHAR* params[]);
    bool                          CreateParams            (int nparams, XCHAR* params);
    bool                          CreateParams            (XCHAR* commandline);

    XVECTOR<XSTRING*>*            GetExecParams           ();
    bool                          DeleteAllExecParams     ();
		
    XPATH*                        GetXPathExec            ();
		#endif
		
    virtual bool                  Factorys_Ini            ();
    virtual bool                  Factorys_End            ();

    APPFLOWMAIN*                  GetAppMain              ();

  protected:

		#ifndef MICROCONTROLLER
    XVECTOR<XSTRING*>             execparams;
    XPATH                         xpathexec;
		#endif
	
    APPFLOWMAIN*                  appmain;

  private:

    void                          Clean                   ();


    void*                         handle;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

extern    MAIN_FUNCTION_TYPE    Main_Proc_PlatformIni;
extern    MAIN_FUNCTION_TYPE    Main_Proc_Ini;
extern    MAIN_FUNCTION_TYPE    Main_Proc_Update;
extern    MAIN_FUNCTION_TYPE    Main_Proc_End;
extern    MAIN_FUNCTION_TYPE    Main_Proc_PlatformEnd;




