/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XProcessManager.cpp
* 
* @class      XPROCESSMANAGER
* @brief      eXtended Utils process manager class
* @note       Cant be construct Factory + singelton without depends of system. ITS NOT A SINGLETON.
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XProcessManager.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XConsole.h"
#include "XTrace.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XPROCESSMANAGER* XPROCESSMANAGER::instance = NULL;


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESS::XPROCESS()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESS::XPROCESS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESS::~XPROCESS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESS::~XPROCESS()
{
  processIDs.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XPROCESS::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XPROCESS::GetID()
{
  return ID;
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESS::SetID(XDWORD ID)
* @brief      Set ID
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESS::SetID(XDWORD ID)
{
  this->ID = ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XDWORD>* XPROCESS::GetProcessIDs()
* @brief      Get process I ds
* @ingroup    XUTILS
* 
* @return     XVECTOR<XDWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XDWORD>* XPROCESS::GetProcessIDs()
{
  return &processIDs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XPROCESS::GetPath()
* @brief      Get path
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XPROCESS::GetPath()
{
  return &path;
}  
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XPROCESS::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XPROCESS::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XPROCESS::GetWindowHandle()
* @brief      Get window handle
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XPROCESS::GetWindowHandle()
{
  return windowhandle;

}
 
  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESS::SetWindowHandle(void* windowhandle)
* @brief      Set window handle
* @ingroup    XUTILS
* 
* @param[in]  windowhandle : Windowhandle pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESS::SetWindowHandle(void* windowhandle)
{
  this->windowhandle = windowhandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XPROCESS::GetWindowTitle()
* @brief      Get window title
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XPROCESS::GetWindowTitle()
{
  return &windowtitle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPRECTINT* XPROCESS::GetWindowRect()
* @brief      Get window rect
* @ingroup    XUTILS
* 
* @return     GRPRECTINT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPRECTINT* XPROCESS::GetWindowRect()
{
  return &windowrect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XPROCESS::GetWindowTitleHeight()
* @brief      Get window title height
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XPROCESS::GetWindowTitleHeight()
{
  return windowtitleheight;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESS::SetWindowTitleHeight(int windowtitleheight)
* @brief      Set window title height
* @ingroup    XUTILS
* 
* @param[in]  windowtitleheight : Windowtitleheight value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESS::SetWindowTitleHeight(int windowtitleheight)
{
  this->windowtitleheight = windowtitleheight;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XPROCESS::GetWindowBorderWidth()
* @brief      Get window border width
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XPROCESS::GetWindowBorderWidth()
{
  return windowborderwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESS::SetWindowBorderWidth(int windowborderwidth)
* @brief      Set window border width
* @ingroup    XUTILS
* 
* @param[in]  windowborderwidth : Windowborderwidth value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESS::SetWindowBorderWidth(int windowborderwidth)
{
  this->windowborderwidth = windowborderwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESS::CopyTo(XPROCESS& xprocess)
* @brief      Copy to
* @ingroup    XUTILS
* 
* @param[in]  xprocess : Xprocess value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESS::CopyTo(XPROCESS& xprocess)
{
  xprocess.SetID(ID);
  xprocess.GetPath()->Set(path);  
  xprocess.GetName()->Set(name);    
  xprocess.GetWindowTitle()->Set(windowtitle); 

  xprocess.GetWindowRect()->CopyFrom(&windowrect);

  xprocess.SetWindowTitleHeight(windowtitleheight);
  xprocess.SetWindowBorderWidth(windowborderwidth);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESS::CopyFrom(XPROCESS& xprocess)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  xprocess : Xprocess value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESS::CopyFrom(XPROCESS& xprocess)
{
  ID                = xprocess.GetID();
  path              = xprocess.GetPath()->Get();  
  name              = xprocess.GetName()->Get();  
  windowtitle       = xprocess.GetWindowTitle()->Get(); 

  GetWindowRect()->CopyFrom(xprocess.GetWindowRect());

  windowtitleheight = xprocess.GetWindowTitleHeight();
  windowborderwidth = xprocess.GetWindowBorderWidth();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESS::Compare(XPROCESS& xprocess, bool onlyfixed)
* @brief      Compare
* @ingroup    XUTILS
* 
* @param[in]  xprocess : Xprocess value.
* @param[in]  onlyfixed : Onlyfixed value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESS::Compare(XPROCESS& xprocess, bool onlyfixed)
{
  if(ID != xprocess.GetID())
    {
      return false;
    }

  if(path.Compare(xprocess.GetPath()->Get(), true) != 0)
    {
      return false;   
    }

  if(name.Compare(xprocess.GetName()->Get(), true) != 0)
    {
      return false;   
    }

  if(windowtitle.Compare(xprocess.GetWindowTitle()->Get(), false) != 0)
    {
      return false;   
    }


  if(!onlyfixed)
    {
      if(GetWindowRect()->IsEqual(xprocess.GetWindowRect()) != ISEQUAL)
        {
          return false;
        }

      if(windowtitleheight != xprocess.GetWindowTitleHeight())
        {
          return false;
        }

      if(windowborderwidth != xprocess.GetWindowBorderWidth())
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESS::Clean()
{
  ID                  = 0xFFFFFFFF;

  windowhandle        = NULL;
  windowtitleheight   = 0;    
  windowborderwidth   = 0;    

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESSMANAGER::XPROCESSMANAGER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESSMANAGER::XPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESSMANAGER::~XPROCESSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESSMANAGER::~XPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::GetIsInstanced()
* @brief      Get is instanced
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESSMANAGER& XPROCESSMANAGER::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XPROCESSMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESSMANAGER& XPROCESSMANAGER::GetInstance()
{
  if(!instance) instance = GEN_NEW XPROCESSMANAGER();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::SetInstance(XPROCESSMANAGER* _instance)
* @brief      Set instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @param[in]  _instance : GEN_NEW instance
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::SetInstance(XPROCESSMANAGER* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
* @brief      Make system command
* @ingroup    XUTILS
* 
* @param[in]  command : Command pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
* @brief      Make command
* @ingroup    XUTILS
* 
* @param[in]  command : Command pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::OpenURL(XCHAR* url)
* @brief      Open URL
* @ingroup    XUTILS
* 
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::OpenURL(XCHAR* url)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      Application execute
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{
  return Application_Execute(applicationpath, params, in, out, returncode);   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, int* returncode)
* @brief      application  execute
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, int* returncode)
{  
  XSTRING* in  = NULL;
  XSTRING* out = NULL;

  return Application_Execute(applicationpath, params, in, out, returncode); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode)
* @brief      Application execute
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode)
{
  XBUFFER _in;
  XBUFFER _out;

  if(in)
    {
      if(!in->IsEmpty())
        {
          AdjustStringToConsolaSymbolsUsed((*in), _in);
        }
    }

  bool status = Application_Execute(applicationpath, params, _in.GetSize()?&_in:NULL, out?&_out:NULL, returncode);

  if(out)
    {      
      if(!_out.IsEmpty())
        {          
          AdjustConsolaSymbolsUsedToString(_out, (*out));      
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      application  execute elevated
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{
   return Application_ExecuteElevated(applicationpath, params, in, out, returncode); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, int* returncode)
* @brief      application  execute elevated
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, int* returncode)
{  
  XSTRING* in = NULL;

  return Application_ExecuteElevated(applicationpath, params, in, NULL, returncode); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode)
* @brief      application  execute elevated
* @ingroup    XUTILS
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode)
{
  XBUFFER _in;
  XBUFFER _out;

  if(in)
    {
      if(!in->IsEmpty())
        {
          AdjustStringToConsolaSymbolsUsed((*in), _in);
        }
    }

  bool status = Application_ExecuteElevated(applicationpath, params, _in.GetSize()?&_in:NULL, out?&_out:NULL, returncode);

  if(out)
    {
      if(!_out.IsEmpty())
        {
          AdjustConsolaSymbolsUsedToString(_out, (*out));
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::AdjustStringToConsolaSymbolsUsed(XSTRING& string, XBUFFER& target_buffer)
* @brief      adjust string to consola symbols used
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* @param[in]  target_buffer : Target buffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::AdjustStringToConsolaSymbolsUsed(XSTRING& string, XBUFFER& target_buffer)
{
  XCONSOLE*            console          = GEN_XFACTORY.CreateConsole();

  if(!console)
    {
      return false;
    }

  XCONSOLE_SYMBOLSUSED symbolused       = console->GetSymbolsUsed();
  XSTRINGASCIICODE     stringasccicode  = string.ConsoleCodePageToConvertASCII(symbolused);

  target_buffer.Empty();

  if(string.IsEmpty())
    {
      GEN_XFACTORY.DeleteConsole(console);  
      return false;  
    }

  if(symbolused == XCONSOLE_SYMBOLSUSED_UNICODE_UTF8)
    {
      string.ConvertToUTF8(target_buffer);    
    }
   else
    {
      if(stringasccicode != XSTRINGASCIICODE_NONE)
        {
          string.ConvertToASCII(target_buffer, stringasccicode);            
        }      
    }

  bool status = true;

  if(target_buffer.IsEmpty())
    {
      status = false;
    }

  GEN_XFACTORY.DeleteConsole(console);  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::AdjustConsolaSymbolsUsedToString(XBUFFER& origin_buffer, XSTRING& string)
* @brief      adjust consola symbols used to string
* @ingroup    XUTILS
* 
* @param[in]  origin_buffer : Origin buffer value.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::AdjustConsolaSymbolsUsedToString(XBUFFER& origin_buffer, XSTRING& string)
{
  XCONSOLE*            console          = GEN_XFACTORY.CreateConsole();

  if(!console)
    {
      return false;
    }

  XCONSOLE_SYMBOLSUSED symbolused       = console->GetSymbolsUsed();
  XSTRINGASCIICODE     stringasccicode  = string.ConsoleCodePageToConvertASCII(symbolused);

  string.Empty();

  if(origin_buffer.IsEmpty())
    {
      GEN_XFACTORY.DeleteConsole(console);
      return false;  
    }

  if(symbolused == XCONSOLE_SYMBOLSUSED_UNICODE_UTF8)
    {
      string.ConvertFromUTF8(origin_buffer);    
    }
   else
    {
      //if(stringasccicode != XSTRINGASCIICODE_NONE)
        {
          string.ConvertFromASCII(origin_buffer, stringasccicode);            
        }      
    }

  bool status = true;

  if(string.IsEmpty())
    {
      status = false;
    }

  GEN_XFACTORY.DeleteConsole(console);  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Application is running
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  applicationname : name of application
* @param[in]  ID : ID of this application of SO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
* @brief      Application get running list
* @ingroup    XUTILS
* 
* @param[in]  applist : Applist pointer to use.
* @param[in]  onlywithvalidwindow : Onlywithvalidwindow value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    XUTILS
* 
* @param[in]  processID : Process ID value.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Terminate(XPROCESS& process, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    XUTILS
* 
* @param[in]  process : Process value.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Terminate(XPROCESS& process, XDWORD exitcode)
{
  return Application_Terminate(process.GetID(), exitcode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Terminate(XCHAR* processname, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    XUTILS
* 
* @param[in]  processname : Processname pointer to use.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Terminate(XCHAR* processname, XDWORD exitcode)
{
  XVECTOR<XPROCESS*> apprunning; 
  bool               status = false;           

  if(!GEN_XPROCESSMANAGER.Application_GetRunningList(apprunning)) return false;

  for(XDWORD c=0; c<apprunning.GetSize(); c++)
    {
      XPROCESS* xprocess = apprunning.Get(c);
      if(xprocess)
        {
          if(!xprocess->GetName()->Compare(processname, true))
            {
              status = Application_Terminate((*xprocess), exitcode);
              break;
            }
        }
    }

  apprunning.DeleteContents();
  apprunning.DeleteAll();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPROCESSMANAGER::Application_Terminate(XSTRING& processname, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    XUTILS
* 
* @param[in]  processname : Processname value.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPROCESSMANAGER::Application_Terminate(XSTRING& processname, XDWORD exitcode)
{
  return Application_Terminate(processname.Get(), exitcode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPROCESS* XPROCESSMANAGER::Application_GetProcessByID(XDWORD processID, XVECTOR<XPROCESS*>& applist)
* @brief      Application get process by ID
* @ingroup    XUTILS
* 
* @param[in]  processID : Process ID value.
* @param[in]  applist : Applist pointer to use.
* 
* @return     XPROCESS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPROCESS* XPROCESSMANAGER::Application_GetProcessByID(XDWORD processID, XVECTOR<XPROCESS*>& applist)
{
  for(XDWORD c=0; c<applist.GetSize(); c++)
    {
      XPROCESS* process =  applist.Get(c);
      if(process)
        {
          if(process->GetID() == processID)
            {
              return process;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPROCESSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPROCESSMANAGER::Clean()
{
  
}



