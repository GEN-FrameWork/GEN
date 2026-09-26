/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script.cpp
* 
* @class      SCRIPT
* @brief      Script base class
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

#include "Script.h"

#include <stdio.h>
#include <ctype.h>

#if defined(WINDOWS) || defined(_WINDOWS)
#include <windows.h>
#endif

#include "XPath.h"
#include "XFactory.h"
#include "XFileTXT.h"
#include "XTrace.h"

#ifdef SCRIPT_G_ACTIVE
#include "Script_Language_G.h"
#endif

#ifdef SCRIPT_LUA_ACTIVE
#include "Script_Language_Lua.h"
#endif

#ifdef SCRIPT_JAVASCRIPT_ACTIVE
#include "Script_Language_Javascript.h"
#endif

#ifdef SCRIPT_CACHE_ACTIVE
#include "Script_Cache.h"
#endif

#include "Script_XEvent.h"

#include "Script_Lib_Math.h"
#include "Script_Lib_Rand.h"
#include "Script_Lib_Timer.h"
#include "Script_Lib_String.h"
#include "Script_Lib_Path.h"
#include "Script_Lib_Dir.h"
#include "Script_Lib_Trace.h"

#include "Script_Lib_System.h"
#include "Script_Lib_Process.h"
#include "Script_Lib_Log.h"
#include "Script_Lib_Console.h"
#include "Script_Lib_CFG.h"
#include "Script_Lib_Window.h"
#include "Script_Lib_InputSimulate.h"
#include "Script_Lib_DevTest.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- GENERAL FUNCTIONS ---------------------------------------------------------------------------------------------*/


#if defined(WINDOWS) || defined(_WINDOWS)

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool SCRIPT_GetCanonicalExistingPath(XPATH& path, XPATH& canonicalpath)
* @brief      Resolve an existing filesystem object to its physical canonical path.
* @ingroup    SCRIPT
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool SCRIPT_GetCanonicalExistingPath(XPATH& path, XPATH& canonicalpath)
{
  canonicalpath.Empty();

  if(path.IsEmpty()) return false;

  HANDLE handle = CreateFile(path.Get(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if(handle == INVALID_HANDLE_VALUE) return false;

  XCHAR buffer[32768];

  DWORD size = GetFinalPathNameByHandle(handle, buffer, (DWORD)((sizeof(buffer) / sizeof(buffer[0])) - 1), FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
  CloseHandle(handle);

  if(!size || (size >= ((sizeof(buffer) / sizeof(buffer[0])) - 1))) return false;

  buffer[size] = 0;

  if((buffer[0] == __C('\\')) && (buffer[1] == __C('\\')) && (buffer[2] == __C('?')) && (buffer[3] == __C('\\')))
    {
      if(((buffer[4] == __C('U')) || (buffer[4] == __C('u'))) &&
         ((buffer[5] == __C('N')) || (buffer[5] == __C('n'))) &&
         ((buffer[6] == __C('C')) || (buffer[6] == __C('c'))) &&
          (buffer[7] == __C('\\')))
        {
          canonicalpath = __L("\\\\");
          canonicalpath += &buffer[8];
        }
       else
        {
          canonicalpath = &buffer[4];
        }
    }
   else
    {
      canonicalpath = buffer;
    }

  canonicalpath.Slash_Normalize(false);
  canonicalpath.Slash_Delete();

  return !canonicalpath.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool SCRIPT_PathIsInsideRoot(XPATH& path, XPATH& root)
* @brief      Compare two normalized paths by complete path segments.
* @ingroup    SCRIPT
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool SCRIPT_PathIsInsideRoot(XPATH& path, XPATH& root)
{
  if(path.IsEmpty() || root.IsEmpty()) return false;

  XPATH pathnormalized = path;
  XPATH rootnormalized = root;

  pathnormalized.Slash_Normalize(false);
  pathnormalized.Slash_Delete();
  pathnormalized.ToLowerCase();

  rootnormalized.Slash_Normalize(false);
  rootnormalized.Slash_Delete();
  rootnormalized.ToLowerCase();

  if(!pathnormalized.Compare(rootnormalized.Get())) return true;

  rootnormalized.Slash_Add();

  if(pathnormalized.GetSize() < rootnormalized.GetSize()) return false;

  for(XDWORD c=0; c<rootnormalized.GetSize(); c++)
    {
      if(pathnormalized.Get()[c] != rootnormalized.Get()[c]) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool SCRIPT_GetCanonicalScriptsRoot(XPATH& rootcanonical)
* @brief      Resolve configured scripts root to its physical canonical path.
* @ingroup    SCRIPT
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool SCRIPT_GetCanonicalScriptsRoot(XPATH& rootcanonical)
{
  XPATH rootscripts;

  rootcanonical.Empty();

  if(!GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS, rootscripts)) return false;

  rootscripts.Slash_Normalize(false);
  rootscripts.Slash_Delete();

  return SCRIPT_GetCanonicalExistingPath(rootscripts, rootcanonical);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool SCRIPT_IsExistingPathPhysicallyInScriptsRoot(XPATH& path)
* @brief      Validate an existing target against the physical scripts root.
* @ingroup    SCRIPT
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool SCRIPT_IsExistingPathPhysicallyInScriptsRoot(XPATH& path)
{
  XPATH rootcanonical;
  XPATH pathcanonical;

  if(!SCRIPT_GetCanonicalScriptsRoot(rootcanonical)) return false;
  if(!SCRIPT_GetCanonicalExistingPath(path, pathcanonical)) return false;

  return SCRIPT_PathIsInsideRoot(pathcanonical, rootcanonical);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool SCRIPT_IsWritablePathPhysicallyInScriptsRoot(XPATH& path)
* @brief      Validate an existing target, or an existing parent for new files, against the physical scripts root.
* @ingroup    SCRIPT
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool SCRIPT_IsWritablePathPhysicallyInScriptsRoot(XPATH& path)
{
  if(SCRIPT_IsExistingPathPhysicallyInScriptsRoot(path)) return true;

  XPATH parentpath = path;

  parentpath.Slash_Normalize(false);
  parentpath.SetOnlyPath();
  parentpath.Slash_Delete();

  if(parentpath.IsEmpty()) return false;

  return SCRIPT_IsExistingPathPhysicallyInScriptsRoot(parentpath);
}

#else

static bool SCRIPT_IsExistingPathPhysicallyInScriptsRoot(XPATH& path)
{
  return !path.IsEmpty();
}


static bool SCRIPT_IsWritablePathPhysicallyInScriptsRoot(XPATH& path)
{
  return !path.IsEmpty();
}

#endif



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT::SCRIPT()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT::SCRIPT()
{
  Clean();

  xtimer=GEN_XFACTORY.CreateTimer();
  if(!xtimer) return;

  xfiletxt = GEN_NEW XFILETXT();
  if(!xfiletxt) return;

  RegisterEvent(SCRIPT_XEVENT_TYPE_ERROR);
  RegisterEvent(SCRIPT_XEVENT_TYPE_BREAK);

  thread = CREATEXTHREAD(XTHREADGROUPID_SCRIPT, __L("SCRIPT::SCRIPT"), ThreadFunction, (void*)this);

  AddInternalLibraries();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT::~SCRIPT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT::~SCRIPT()
{
  DeRegisterEvent(SCRIPT_XEVENT_TYPE_ERROR);
  DeRegisterEvent(SCRIPT_XEVENT_TYPE_BREAK);

  if(xtimer) GEN_XFACTORY.DeleteTimer(xtimer);

  if(thread)
    {
      thread->End();
      DELETEXTHREAD(XTHREADGROUPID_SCRIPT, thread);
    }

  if(xfiletxt)
    {
      GEN_DELETE xfiletxt;
    }

  DeleteLibraryFuncions();

  DeleteAllLibrarys();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_TYPE SCRIPT::GetType()
* @brief      Get type
* @ingroup    SCRIPT
* 
* @return     SCRIPT_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_TYPE SCRIPT::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_TYPE SCRIPT::GetTypeByExtension(XCHAR* namefilescript)
* @brief      Get type by extension
* @ingroup    SCRIPT
* 
* @param[in]  namefilescript : Namefilescript pointer to use.
* 
* @return     SCRIPT_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_TYPE SCRIPT::GetTypeByExtension(XCHAR* namefilescript)
{
  XSTRING       ext;
  SCRIPT_TYPE   type            = SCRIPT_TYPE_UNKNOWN;
  XPATH         _namefilescript;

  _namefilescript = namefilescript;
                                                                                        
  if(!_namefilescript.IsEmpty())
    {
      _namefilescript.GetExt(ext);

      #ifdef SCRIPT_G_ACTIVE
      if(!ext.Compare(__L(".g")   , true))  
        {
          type = SCRIPT_TYPE_G;
        }
      #endif

      #ifdef SCRIPT_LUA_ACTIVE
      if(!ext.Compare(__L(".lua") , true))
        {
          type = SCRIPT_TYPE_LUA;
        }
      #endif

      #ifdef SCRIPT_JAVASCRIPT_ACTIVE
      if(!ext.Compare(__L(".js")  , true)) 
        {
          type = SCRIPT_TYPE_JAVASCRIPT;
        }
      #endif
    }

  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT* SCRIPT::Create(XCHAR* namefilescript)
* @brief      Create resource
* @ingroup    SCRIPT
* 
* @param[in]  namefilescript : Namefilescript pointer to use.
* 
* @return     SCRIPT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT* SCRIPT::Create(XCHAR* namefilescript)
{
  return Create(GetTypeByExtension(namefilescript));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT* SCRIPT::Create(SCRIPT_TYPE type)
* @brief      Create resource
* @ingroup    SCRIPT
* 
* @param[in]  type : Type value.
* 
* @return     SCRIPT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT* SCRIPT::Create(SCRIPT_TYPE type)
{
  SCRIPT* script = NULL;

  switch(type)
    {
      case SCRIPT_TYPE_UNKNOWN      : break;

      case SCRIPT_TYPE_G            : 
                                      #ifdef SCRIPT_G_ACTIVE
                                      script = GEN_NEW SCRIPT_LNG_G();
                                      #endif
                                      break;  

      case SCRIPT_TYPE_LUA          : 
                                      #ifdef SCRIPT_LUA_ACTIVE
                                      script = GEN_NEW SCRIPT_LNG_LUA();
                                      #endif
                                      break;

      case SCRIPT_TYPE_JAVASCRIPT   :  
                                      #ifdef SCRIPT_JAVASCRIPT_ACTIVE
                                      script = GEN_NEW SCRIPT_LNG_JAVASCRIPT();
                                      #endif
                                      break;     
    }


  return script;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::Load(XPATH& xpath)
* @brief      Load
* @ingroup    SCRIPT
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::Load(XPATH& xpath)
{
  xpath.Slash_Normalize(false);

  if(!SCRIPT_IsExistingPathPhysicallyInScriptsRoot(xpath)) return false;

  #ifdef SCRIPT_CACHE_ACTIVE

  XDWORD ID = GEN_SCRIPT_CACHE.GenerateID(xpath);

  XSTRING* _script = GEN_SCRIPT_CACHE.Cache_Get(ID, xpath);
  if(_script)
    {
      script.Empty();
      script += _script->Get();  

      GetNameScript()->Format(__L("ID%08X"), ID);

      return true;
    }

  #endif

  if(!xfiletxt) return false;

  bool status = false;

  this->xpath = xpath;

  xpath.GetNamefileExt(namescript);

  if(xfiletxt->Open(xpath))
    {
      if(xfiletxt->ReadAllFile()) status = true;

      script.Empty();

      for(int c=0; c<xfiletxt->GetNLines(); c++)
        {
          script += xfiletxt->GetLine(c)->Get();
      
          AddReturnByType();
        }

      xfiletxt->Close();
    }

  #ifdef SCRIPT_CACHE_ACTIVE
  if(status)
    {
      ID = GEN_SCRIPT_CACHE.GenerateID(xpath);      
      GEN_SCRIPT_CACHE.Cache_Add(ID, &script, xpath);
    }
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::LoadAdd(XPATH& xpath)
* @brief      Load add
* @ingroup    SCRIPT
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::LoadAdd(XPATH& xpath)
{  
  if(!xfiletxt) return false;

  xpath.Slash_Normalize(false);

  if(!SCRIPT_IsExistingPathPhysicallyInScriptsRoot(xpath)) return false;

  bool status = false;

  AddReturnByType();
  AddReturnByType();
  
  this->xpath = xpath;

  xpath.GetNamefileExt(namescript);

  if(xfiletxt->Open(xpath))
    {
      if(xfiletxt->ReadAllFile()) status = true;

      for(int c=0; c<xfiletxt->GetNLines(); c++)
        {
          script += xfiletxt->GetLine(c)->Get();
      
          AddReturnByType();
        }

      xfiletxt->Close();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::Save(XPATH& xpath)
* @brief      Save
* @ingroup    SCRIPT
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::Save(XPATH& xpath)
{
  if(!xfiletxt) return false;

  xpath.Slash_Normalize(false);

  if(!SCRIPT_IsWritablePathPhysicallyInScriptsRoot(xpath)) return false;

  bool status = false;

  this->xpath = xpath;

  if(xfiletxt->Create(xpath))
    {
      if(xfiletxt->WriteAllFile()) status = true;
      xfiletxt->Close();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::ResolvePathInScriptsRoot(XCHAR* namescript, XPATH& resolvedpath)
* @brief      Resolve a relative script name inside the configured scripts root
* @ingroup    SCRIPT
*
* @param[in]  namescript : Relative script name.
* @param[out] resolvedpath : Normalized path inside the scripts root.
*
* @return     bool : true if the path is confined to the scripts root; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::ResolvePathInScriptsRoot(XCHAR* namescript, XPATH& resolvedpath)
{
  resolvedpath.Empty();

  if(!namescript || !namescript[0]) return false;

  XPATH relativepath;

  relativepath = namescript;
  EliminateExtraChars(&relativepath);
  relativepath.Slash_Normalize(false);

  if(relativepath.IsEmpty()) return false;
  if(relativepath.Get()[0] == __C('/')) return false;

  int segmentstart = 0;

  for(int index=0; index<=(int)relativepath.GetSize(); index++)
    {
      XCHAR character = relativepath.Get()[index];

      if((character < __C(' ')) && character) return false;
      if(character == __C(':') || character == __C('*') || character == __C('?') ||
         character == __C('"') || character == __C('<') || character == __C('>') ||
         character == __C('|')) return false;

      if((character == __C('/')) || !character)
        {
          int segmentsize = index - segmentstart;

          if(segmentsize <= 0) return false;
          if((segmentsize == 1) && (relativepath.Get()[segmentstart] == __C('.'))) return false;
          if((segmentsize == 2) && (relativepath.Get()[segmentstart]     == __C('.')) &&
                                    (relativepath.Get()[segmentstart + 1] == __C('.'))) return false;

          segmentstart = index + 1;
        }
    }

  if(!GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS, resolvedpath))
    {
      resolvedpath.Empty();
      return false;
    }

  resolvedpath.Slash_Normalize(false);
  resolvedpath.Slash_Delete();
  resolvedpath.Slash_Add();
  resolvedpath += relativepath.Get();
  resolvedpath.Slash_Normalize(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::LoadScriptAndRun(XVECTOR<XSTRING*>* listscripts, SCRFUNCADJUSTLIBRARYS adjustlibrarys)
* @brief      Load script and run
* @ingroup    SCRIPT
* 
* @param[in]  listscripts : Listscripts pointer to use.
* @param[in]  adjustlibrarys : Adjustlibrarys value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::LoadScriptAndRun(XVECTOR<XSTRING*>* listscripts, SCRFUNCADJUSTLIBRARYS adjustlibrarys)
{
  if(!listscripts) 
    {
      return false;
    }

  bool status = false;
                                                                                                     
  for(XDWORD c=0; c<listscripts->GetSize(); c++)
    {  
      XSTRING* linescripts = listscripts->Get(c);    
      if(linescripts)
        {
          if(!linescripts->IsEmpty())
            {
              XVECTOR<XSTRING*> namescripts;

              linescripts->Split(__C(','), namescripts);

              bool pathsvalid = namescripts.GetSize()?true:false;

              for(XDWORD d=0; d<namescripts.GetSize() && pathsvalid; d++)
                {
                  XSTRING* nametovalidate = namescripts.Get(d);
                  XPATH    validatedpath;

                  if(!nametovalidate || !SCRIPT::EliminateExtraChars(nametovalidate) ||
                     !SCRIPT::ResolvePathInScriptsRoot(nametovalidate->Get(), validatedpath))
                    {
                      pathsvalid = false;
                    }
                }

              if(!pathsvalid) status = false;

              XSTRING* namescript = pathsvalid?namescripts.Get(0):NULL;
              if(namescript)
                {
                  SCRIPT* script = SCRIPT::Create(namescript->Get());
                  if(script) 
                    {
                      XDWORD      ID = 0;
                      bool        incache = false;

                      if(adjustlibrarys)
                        {
                          adjustlibrarys(script);
                        }
                 
                      #ifdef SCRIPT_CACHE_ACTIVE

                      XSTRING cachekey;
                      bool    cachekeyvalid = GEN_SCRIPT_CACHE.GenerateListKey(&namescripts, cachekey);

                      if(cachekeyvalid)
                        {                         
                          ID = GEN_SCRIPT_CACHE.GenerateID(cachekey);

                          XSTRING* _script = GEN_SCRIPT_CACHE.Cache_Get(ID, cachekey);
                          if(_script)
                            {
                              (*script->GetScript()) += _script->Get();

                              incache = true;
                              status  = !_script->IsEmpty();

                              script->GetNameScript()->Format(__L("ID%08X"), ID);
                            }
                        }

                      #endif

                      if(!incache)
                        {
                          for(XDWORD d=0; d<namescripts.GetSize(); d++)
                            {  
                              namescript = namescripts.Get(d);
                              if(namescript)
                                {                          
                                  XPATH xpath;   

                                  if(!SCRIPT::ResolvePathInScriptsRoot(namescript->Get(), xpath))
                                    {
                                      status = false;
                                      break;
                                    }

                                  status = script->LoadAdd(xpath);
                                  if(!status)  
                                    {
                                      break;
                                    }  
                                }
                            }  
                        }
          
                      #ifdef SCRIPT_CACHE_ACTIVE
                      if(status && !incache && cachekeyvalid)
                        {                          
                          GEN_SCRIPT_CACHE.Cache_Add(ID, script->GetScript(), cachekey);
                        }
                      #endif                    

                      if(status)
                        {                               
                          script->Run();                                       
                        }
                      
                      GEN_DELETE script;
                      script = NULL;

                    }                    
                }

              namescripts.DeleteContents();
              namescripts.DeleteAll();              
            
            } 
        } 
    }
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::AddReturnByType()
* @brief      Add return by type
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::AddReturnByType()
{
  switch(type)
    {
      case SCRIPT_TYPE_UNKNOWN       : 
                           default   : script += __L("\r\n"); break;
      case SCRIPT_TYPE_G             : script += __L("\r\n"); break;
      case SCRIPT_TYPE_LUA           : script += __L("\r");   break;
      case SCRIPT_TYPE_JAVASCRIPT    : script += __L("\r");   break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::IsScript(XPATH& xpath, XCHAR* extension)
* @brief      Is script
* @ingroup    SCRIPT
* 
* @param[in]  xpath : Path to use.
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::IsScript(XPATH& xpath, XCHAR* extension)
{
  if(xpath.IsEmpty()) return false;

  if(!xpath.Find(extension, true)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* SCRIPT::GetNameScript()
* @brief      Get name script
* @ingroup    SCRIPT
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* SCRIPT::GetNameScript()
{
  return &namescript;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* SCRIPT::GetScript()
* @brief      Get script
* @ingroup    SCRIPT
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* SCRIPT::GetScript()
{
  return &script;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* SCRIPT::GetPath()
* @brief      Get path
* @ingroup    SCRIPT
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* SCRIPT::GetPath()
{
  return &xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SCRIPT::Run(int* returnval)
* @brief      Run operation
* @ingroup    SCRIPT
* 
* @param[in]  returnval : Returnval pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SCRIPT::Run(int* returnval)
{
  if(script.IsEmpty()) 
    {
      return SCRIPT_ERRORCODE_INTERNALERROR;
    }

  isrunwiththread = false;
  if(thread) isrunwiththread = thread->IsRunning();

  errorcode  = 0;
  iscancelexec = false;
 
  return errorcode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::RunWithThread()
* @brief      Run with thread
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::RunWithThread()
{
  if(!thread) return false;

  return thread->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::IsRunWithThread()
* @brief      Is run with thread
* @ingroup    SCRIPT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::IsRunWithThread()
{
  return isrunwiththread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::IsRunThread(int* error, int* returnvalue)
* @brief      Is run thread
* @ingroup    SCRIPT
* 
* @param[in]  error : Error pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::IsRunThread(int* error, int* returnvalue)
{
  if(!thread) return false;

  bool isrun = thread->IsRunning();

  if(!isrun)
    {
      if(error) (*error) = errorcode;

      if(!errorcode)
        {
          if(returnvalue) (*returnvalue) = returnvaluescript;
        }

       thread->End();
    }

  return isrun;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::CancelExecution()
* @brief      Cancel execution
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::CancelExecution()
{
  if(script.IsEmpty()) return false;

  iscancelexec = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* SCRIPT::GetTimer()
* @brief      Get timer
* @ingroup    SCRIPT
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* SCRIPT::GetTimer()
{
  return xtimer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADCOLLECTED* SCRIPT::GetThread()
* @brief      Get thread
* @ingroup    SCRIPT
* 
* @return     XTHREADCOLLECTED* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADCOLLECTED* SCRIPT::GetThread()
{
  return thread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB* SCRIPT::GetLibrary(XCHAR* ID)
* @brief      Get library
* @ingroup    SCRIPT
* 
* @param[in]  ID : Identifier to use.
* 
* @return     SCRIPT_LIB* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB* SCRIPT::GetLibrary(XCHAR* ID)
{
  if(librarys.IsEmpty()) return NULL;

  for(int c=0;c<(int)librarys.GetSize();c++)
    {
      SCRIPT_LIB* scriptlib = (SCRIPT_LIB*)librarys.Get(c);
      if(scriptlib)
        {
          XSTRING* IDlib = scriptlib->GetID();
          if(IDlib)
            {
              if(!IDlib->Compare(ID)) 
                {
                  return scriptlib;
                }
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::AddLibrary(SCRIPT_LIB* scriptlib)
* @brief      Add library
* @ingroup    SCRIPT
* 
* @param[in]  scriptlib : Scriptlib pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::AddLibrary(SCRIPT_LIB* scriptlib)
{
  if(!scriptlib) return false;

  librarys.Add(scriptlib);

  return scriptlib->AddLibraryFunctions(this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::DeleteAllLibrarys()
* @brief      Delete all librarys
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::DeleteAllLibrarys()
{
  if(librarys.IsEmpty()) return false;

  librarys.DeleteContents();
  librarys.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_FUNCTION* SCRIPT::GetLibraryFunction(XCHAR* name)
* @brief      Get library function
* @ingroup    SCRIPT
* 
* @param[in]  name : Name to use.
* 
* @return     SCRIPT_LIB_FUNCTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_FUNCTION* SCRIPT::GetLibraryFunction(XCHAR* name)
{
  if(!name) return NULL;

  for(XDWORD c=0;c<libraryfunctions.GetSize(); c++)
    {
      SCRIPT_LIB_FUNCTION* function = (SCRIPT_LIB_FUNCTION*)libraryfunctions.Get(c);
      if(function)
        {
          if(!function->GetName()->Compare(name))
            {
              if(IsLibraryFunctionAllowed(function->GetLibrary(), function->GetName()->Get())) return function;
              return NULL;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_FUNCTION* SCRIPT::GetLibraryFunction(void* ptrfunction)
* @brief      Get library function
* @ingroup    SCRIPT
* 
* @param[in]  ptrfunction : Ptrfunction pointer to use.
* 
* @return     SCRIPT_LIB_FUNCTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_FUNCTION* SCRIPT::GetLibraryFunction(void* ptrfunction)
{
  if(!ptrfunction) return NULL;

  for(XDWORD c=0;c<libraryfunctions.GetSize(); c++)
    {
      SCRIPT_LIB_FUNCTION* function = (SCRIPT_LIB_FUNCTION*)libraryfunctions.Get(c);
      if(function)
        {
          if((void*)(function->GetFunctionLibrary()) == ptrfunction)
            {
              if(IsLibraryFunctionAllowed(function->GetLibrary(), function->GetName()->Get())) return function;
              return NULL;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::AddLibraryFunction(SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY libfunction)
* @brief      Add library function
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  name : Name to use.
* @param[in]  libfunction : Libfunction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::AddLibraryFunction(SCRIPT_LIB* library, XCHAR* name, SCRFUNCIONLIBRARY libfunction)
{
  SCRIPT_LIB_FUNCTION* function = GEN_NEW SCRIPT_LIB_FUNCTION(library, name, libfunction);
  if(!function) return false;

  libraryfunctions.Add(function);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::DeleteLibraryFuncions()
* @brief      Delete library funcions
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::DeleteLibraryFuncions()
{
  if(libraryfunctions.IsEmpty()) return false;

  libraryfunctions.DeleteContents();
  libraryfunctions.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::AddInternalLibraries()
* @brief      Add internal libraries
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::AddInternalLibraries()
{  
  SCRIPT_LIB* lib;

  for(int c=0; c<15; c++)
    {
      lib = NULL;

      switch(c)
        {
          // Standard 
          case  0 : lib = GEN_NEW SCRIPT_LIB_MATH();        break;
          case  1 : lib = GEN_NEW SCRIPT_LIB_RAND();        break;
          case  2 : lib = GEN_NEW SCRIPT_LIB_TIMER();       break;
          case  3 : lib = GEN_NEW SCRIPT_LIB_STRING();      break;
          case  4 : lib = GEN_NEW SCRIPT_LIB_PATH();        break;
          case  5 : lib = GEN_NEW SCRIPT_LIB_DIR();         break;
          case  6 : lib = GEN_NEW SCRIPT_LIB_TRACE();       break;

          // Optionals
          case  7 :
                    #ifdef SCRIPT_LIB_SYSTEM_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_SYSTEM();        
                    #endif
                    break;
          case  8 :
                    #ifdef SCRIPT_LIB_PROCESS_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_PROCESS();        
                    #endif
                    break;
          case  9 :
                    #ifdef SCRIPT_LIB_LOG_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_LOG();        
                    #endif
                    break;
          case 10 :
                    #ifdef SCRIPT_LIB_CONSOLE_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_CONSOLE();        
                    #endif
                    break;
          case 11 :
                    #ifdef SCRIPT_LIB_CFG_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_CFG();                                
                    #endif
                    break;
          case 12 :
                    #ifdef SCRIPT_LIB_WINDOW_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_WINDOW();        
                    #endif
                    break;
          case 13 :
                    #ifdef SCRIPT_LIB_INPUTSIMULATE_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_INPUTSIMULATE();        
                    #endif
                    break;
          case 14 :
                    #ifdef SCRIPT_LIB_DEVTEST_ACTIVE
                    lib = GEN_NEW SCRIPT_LIB_DEVTEST();        
                    #endif
                    break;
        } 

      if(lib)
        {
          AddLibrary(lib);
        }           
    }      

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD SCRIPT::GetCapabilities()
* @brief      Get the explicitly enabled unsafe script capabilities
* @ingroup    SCRIPT
*
* @return     XDWORD : Enabled SCRIPT_CAPABILITY mask.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SCRIPT::GetCapabilities()
{
  return capabilities;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::SetCapabilities(XDWORD capabilities)
* @brief      Replace the unsafe script capability allowlist
* @ingroup    SCRIPT
*
* @param[in]  capabilities : SCRIPT_CAPABILITY mask.
*
* @return     bool : true if the mask is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::SetCapabilities(XDWORD capabilities)
{
  if(capabilities & ~((XDWORD)SCRIPT_CAPABILITY_ALL_UNSAFE)) return false;

  this->capabilities = capabilities;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::EnableCapabilities(XDWORD capabilities)
* @brief      Add capabilities to the script allowlist
* @ingroup    SCRIPT
*
* @param[in]  capabilities : SCRIPT_CAPABILITY mask.
*
* @return     bool : true if the mask is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::EnableCapabilities(XDWORD capabilities)
{
  if(capabilities & ~((XDWORD)SCRIPT_CAPABILITY_ALL_UNSAFE)) return false;

  this->capabilities |= capabilities;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::DisableCapabilities(XDWORD capabilities)
* @brief      Remove capabilities from the script allowlist
* @ingroup    SCRIPT
*
* @param[in]  capabilities : SCRIPT_CAPABILITY mask.
*
* @return     bool : true if the mask is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::DisableCapabilities(XDWORD capabilities)
{
  if(capabilities & ~((XDWORD)SCRIPT_CAPABILITY_ALL_UNSAFE)) return false;

  this->capabilities &= ~capabilities;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::IsCapabilityEnabled(SCRIPT_CAPABILITY capability)
* @brief      Check whether an unsafe script capability is enabled
* @ingroup    SCRIPT
*
* @param[in]  capability : Capability to check.
*
* @return     bool : true if enabled; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::IsCapabilityEnabled(SCRIPT_CAPABILITY capability)
{
  if(capability == SCRIPT_CAPABILITY_NONE) return true;

  return (capabilities & (XDWORD)capability) == (XDWORD)capability;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool SCRIPT::IsLibraryFunctionAllowed(SCRIPT_LIB* library, XCHAR* name)
* @brief      Apply the per-script capability allowlist to a native function
* @ingroup    SCRIPT
*
* @param[in]  library : Function library.
* @param[in]  name : Function name.
*
* @return     bool : true if the native function may be called; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::IsLibraryFunctionAllowed(SCRIPT_LIB* library, XCHAR* name)
{
  if(!library || !name || !library->GetID()) return false;

  XSTRING* libraryID = library->GetID();

  if(!libraryID->Compare(__L("Process")))
    {
      return IsCapabilityEnabled(SCRIPT_CAPABILITY_PROCESS);
    }

  if(!libraryID->Compare(__L("System")))
    {
      return IsCapabilityEnabled(SCRIPT_CAPABILITY_SYSTEM);
    }

  if(!libraryID->Compare(__L("InpSimulate")))
    {
      return IsCapabilityEnabled(SCRIPT_CAPABILITY_INPUT_SIMULATE);
    }

  if(!libraryID->Compare(__L("Window")))
    {
      return IsCapabilityEnabled(SCRIPT_CAPABILITY_WINDOW);
    }

  if(!libraryID->Compare(__L("Dir")))
    {
      XSTRING functionname(name);

      if(!functionname.Compare(__L("ChangeDir")) ||
         !functionname.Compare(__L("RemoveDir")) ||
         !functionname.Compare(__L("MakeDir")))
        {
          return IsCapabilityEnabled(SCRIPT_CAPABILITY_FILESYSTEM_WRITE);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SCRIPT::GetErrorScript()
* @brief      Get error script
* @ingroup    SCRIPT
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SCRIPT::GetErrorScript()
{
  return errorcode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::SetErrorScript(int errorcode)
* @brief      Set error script
* @ingroup    SCRIPT
* 
* @param[in]  errorcode : Errorcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::SetErrorScript(int errorcode)
{
  this->errorcode = errorcode;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::HaveError(int errorcode)
* @brief      Have error
* @ingroup    SCRIPT
* 
* @param[in]  errorcode : Errorcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::HaveError(int errorcode)
{
  return (!errorcode)?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT::EliminateExtraChars(XSTRING* namescript)
* @brief      Eliminate extra chars
* @ingroup    SCRIPT
* 
* @param[in]  namescript : Namescript pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT::EliminateExtraChars(XSTRING* namescript)
{
  if(!namescript)
    {
      return false;
    }

  if(namescript->IsEmpty())
    {
      return false;
    }

  namescript->DeleteCharacter(__C(' '), XSTRINGCONTEXT_FROM_FIRST);
  namescript->DeleteCharacter(__C(' '), XSTRINGCONTEXT_TO_END);

  namescript->DeleteCharacter(__C('\t'), XSTRINGCONTEXT_FROM_FIRST);
  namescript->DeleteCharacter(__C('\t'), XSTRINGCONTEXT_TO_END);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT::ThreadFunction(void* data)
* @brief      Thread function
* @ingroup    SCRIPT
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT::ThreadFunction(void* data)
{
  SCRIPT* script = (SCRIPT*)data;
  if(!script) return;

  int returnvaluescript;

  script->SetErrorScript(script->Run(&returnvaluescript));

  script->GetThread()->Run(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT::Clean()
{
  type                = SCRIPT_TYPE_UNKNOWN;

  xpath.Empty();

  xfiletxt            = NULL;
  namescript          = __L("");
  script.Empty();

  xtimer              = NULL;
  errorcode           = 0;

  thread              = NULL;
  isrunwiththread     = false;

  returnvaluescript   = 0;

  iscancelexec        = false;

  #ifdef SCRIPT_ALLOW_UNSAFE_LIBRARIES_BY_DEFAULT
  capabilities        = SCRIPT_CAPABILITY_ALL_UNSAFE;
  #else
  capabilities        = SCRIPT_CAPABILITY_NONE;
  #endif
}
