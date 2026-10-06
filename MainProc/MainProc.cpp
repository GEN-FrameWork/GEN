/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProc.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "MainProc.h"

#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

MAIN_FUNCTION_TYPE  Main_Proc_PlatformIni   = MAIN_FUNCTION_PLATFORMINI;
MAIN_FUNCTION_TYPE  Main_Proc_Ini           = MAIN_FUNCTION_INI;
MAIN_FUNCTION_TYPE  Main_Proc_Update        = MAIN_FUNCTION_UPDATE;
MAIN_FUNCTION_TYPE  Main_Proc_End           = MAIN_FUNCTION_END;
MAIN_FUNCTION_TYPE  Main_Proc_PlatformEnd   = MAIN_FUNCTION_PLATFORMEND;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROC::MAINPROC()
* @brief      Constructor of class
* @ingroup    MAIN_PROCEDURE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROC::MAINPROC()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROC::~MAINPROC()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    MAIN_PROCEDURE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROC::~MAINPROC()

{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::Ini(APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
* @brief      Initialize the object
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  appmain : Appmain pointer to use.
* @param[in]  applicationmode : Applicationmode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::Ini(APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::Update()
* @brief      Update
* @ingroup    MAIN_PROCEDURE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::Update()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::End()
* @brief      End the object
* @ingroup    MAIN_PROCEDURE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::End()
{
	#ifndef MICROCONTROLLER
  DeleteAllExecParams();
	#endif

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* MAINPROC::GetHandle()
* @brief      Get handle
* @ingroup    MAIN_PROCEDURE
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* MAINPROC::GetHandle()
{
  return handle;
}


#ifndef MICROCONTROLLER


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::CreateParams(int nparams, char* params[])
* @brief      Create params
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  nparams : Nparams value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::CreateParams(int nparams, char* params[])
{
  for(int c=1; c<nparams; c++)
    {
      XSTRING* param = GEN_NEW XSTRING();
      if(param)
        {
          (*param) = params[c];
          GetExecParams()->Add(param);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::CreateParams(int nparams, XCHAR* params[])
* @brief      Create params
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  nparams : Nparams value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::CreateParams(int nparams, XCHAR* params[])
{
  for(int c=1; c<nparams; c++)
    {
      XSTRING* param = GEN_NEW XSTRING();
      if(param)
        {
          (*param) = params[c];
          GetExecParams()->Add(param);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::CreateParams(int nparams, XCHAR* params)
* @brief      Create params
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  nparams : Nparams value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::CreateParams(int nparams, XCHAR* params)
{
  for(int c=1; c<nparams; c++)
    {
      XSTRING* param = GEN_NEW XSTRING();
      if(param)
        {
          (*param) = params[c];
          GetExecParams()->Add(param);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::CreateParams(XCHAR* commandline)
* @brief      Create params
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  commandline : Commandline pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::CreateParams(XCHAR* commandline)
{
  XSTRING _commandline = commandline;

  if(_commandline.IsEmpty()) return false;

  int    start  = 0;
  XDWORD c      = 0;

  do{ if((_commandline.Get()[c] == _C(' ')) || (_commandline.GetSize() == c))
        {
          XSTRING* param = GEN_NEW XSTRING();
          if(param)
            {
              _commandline.Copy(start, c, (*param));

              GetExecParams()->Add(param);
              start = c+1;
            }
        }

      c++;

    } while(c <= _commandline.GetSize());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROC::SetHandle(void* handle)
* @brief      Set handle
* @ingroup    MAIN_PROCEDURE
* 
* @param[in]  handle : Handle pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROC::SetHandle(void* handle)
{
  this->handle = handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* MAINPROC::GetXPathExec()
* @brief      Get X path exec
* @ingroup    MAIN_PROCEDURE
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* MAINPROC::GetXPathExec()
{
  return &xpathexec;
}


#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::Factorys_Ini()
* @brief      Factorys ini
* @ingroup    MAIN_PROCEDURE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::Factorys_Ini()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::Factorys_End()
* @brief      Factorys end
* @ingroup    MAIN_PROCEDURE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::Factorys_End()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWMAIN* MAINPROC::GetAppMain()
* @brief      Get app main
* @ingroup    MAIN_PROCEDURE
* 
* @return     APPFLOWMAIN* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWMAIN* MAINPROC::GetAppMain()
{
  return appmain;
}


#ifndef MICROCONTROLLER


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XSTRING*>* MAINPROC::GetExecParams()
* @brief      Get exec params
* @ingroup    MAIN_PROCEDURE
* 
* @return     XVECTOR<XSTRING*>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>* MAINPROC::GetExecParams()
{
  return &execparams;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROC::DeleteAllExecParams()
* @brief      Delete all exec params
* @ingroup    MAIN_PROCEDURE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROC::DeleteAllExecParams()
{
  if(execparams.IsEmpty()) return false;

  execparams.DeleteContents();

  execparams.DeleteAll();

  return true;
}


#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROC::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    MAIN_PROCEDURE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROC::Clean()
{  
  handle            = NULL;  
  appmain           = NULL;    
}



