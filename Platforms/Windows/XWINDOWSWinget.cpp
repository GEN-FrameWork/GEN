/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSWinget.cpp
* 
* @class      XWINDOWSWINGET
* @brief      WINDOWS eXtended Utils WinGet API class
* @ingroup    PLATFORM_WINDOWS
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

#include "Windows.h"

#include "XWINDOWSWinget.h"

#include "XBuffer.h"
#include "XSerializable.h"
#include "XFileJSON.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XWINDOWSWINGET* XWINDOWSWINGET::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET_ELEMENTRESULT::XWINDOWSWINGET_ELEMENTRESULT()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET_ELEMENTRESULT::XWINDOWSWINGET_ELEMENTRESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET_ELEMENTRESULT::~XWINDOWSWINGET_ELEMENTRESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET_ELEMENTRESULT::~XWINDOWSWINGET_ELEMENTRESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET_ELEMENTRESULT::Serialize()
* @brief      serialize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET_ELEMENTRESULT::Serialize()
{
  Primitive_Add<XSTRING*>(&name                 , __L("name"));
  Primitive_Add<XSTRING*>(&ID                   , __L("ID"));
  Primitive_Add<XSTRING*>(&actualversion        , __L("actualversion"));
  Primitive_Add<XSTRING*>(&availableversion     , __L("availableversion"));

  return true;
}

   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET_ELEMENTRESULT::Deserialize()
* @brief      deserialize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET_ELEMENTRESULT::Deserialize()
{
  Primitive_Extract<XSTRING&>(name              , __L("name"));
  Primitive_Extract<XSTRING&>(ID                , __L("ID"));
  Primitive_Extract<XSTRING&>(actualversion     , __L("actualversion"));
  Primitive_Extract<XSTRING&>(availableversion  , __L("availableversion"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET_ELEMENTRESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET_ELEMENTRESULT::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET_LISTRESULT::XWINDOWSWINGET_LISTRESULT()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET_LISTRESULT::XWINDOWSWINGET_LISTRESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET_LISTRESULT::~XWINDOWSWINGET_LISTRESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET_LISTRESULT::~XWINDOWSWINGET_LISTRESULT()
{
  list.DeleteContents();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET_LISTRESULT::Serialize()
* @brief      serialize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET_LISTRESULT::Serialize()
{
  XVectorClass_Add<XWINDOWSWINGET_ELEMENTRESULT>(&list, __L("list"), __L(""));
 
  return true;
}

   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET_LISTRESULT::Deserialize()
* @brief      deserialize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET_LISTRESULT::Deserialize()
{
  XVectorClass_Extract<XWINDOWSWINGET_ELEMENTRESULT>(&list, __L("list"), __L(""));
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET_LISTRESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET_LISTRESULT::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::GetIsInstanced()
* @brief      get is instanced
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET& XWINDOWSWINGET::GetInstance()
* @brief      get instance
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XWINDOWSWINGET& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET& XWINDOWSWINGET::GetInstance()
{
  if(!instance) instance = GEN_NEW XWINDOWSWINGET();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::DelInstance()
* @brief      del instance
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::DelInstance()
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
* @fn         bool XWINDOWSWINGET::InstallModule()
* @brief      install module
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::InstallModule()
{
  XBUFFER    buffer_output;
  bool       status = false;
 
  /*
  status = Exec( __L("Import-Module PackageManagement -Force"), buffer_output);
  status = Exec( __L("Import-Module PowerShellGet -Force"), buffer_output);
  status = Exec( __L("Install-PackageProvider -Name NuGet -MinimumVersion 2.8.5.201 -Force -Confirm:$false | Out-Null"), buffer_output);
  status = Exec( __L("Install-Module Microsoft.WinGet.Client -Force -Confirm:$false -AllowClobber"), buffer_output);
  */

  status = Exec( __L("Install-Module Microsoft.WinGet.Client -Force -Confirm:$false -AllowClobber"), buffer_output);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::List(XWINDOWSWINGET_LISTRESULT* listresult)
* @brief      list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  listresult : Output listresult.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::List(XWINDOWSWINGET_LISTRESULT* listresult)
{ 
  XSTRING             string;
  XVECTOR<XSTRING*>   list_name;
  XVECTOR<XSTRING*>   list_ID;
  XBUFFER             buffer_output;
  bool                status = false;

  if(!listresult)
    {
      return false;    
    }

  status = GenerateColumnList(__L("Get-WinGetPackage | Select-Object Name"), &list_name);
  if(status)
    {
      status = GenerateColumnList(__L("Get-WinGetPackage | Select-Object ID"), &list_ID);
      if(status)
        {             
          if(list_name.GetSize() == list_ID.GetSize())
            {    
              for(int c=0; c<list_name.GetSize(); c++)
                {
                  if(!list_name.Get(c)->IsEmpty() || !list_ID.Get(c)->IsEmpty())
                    {                        
                      XWINDOWSWINGET_ELEMENTRESULT* elementresult = GEN_NEW XWINDOWSWINGET_ELEMENTRESULT();
                      if(elementresult)  
                        {

                          // list_name.Get(c)->DeleteCharacter(__C('\\'), XSTRINGCONTEXT_ALLSTRING);
                          // list_ID.Get(c)->DeleteCharacter(__C('\\'), XSTRINGCONTEXT_ALLSTRING);

                          elementresult->name = list_name.Get(c)->Get();
                          elementresult->ID   = list_ID.Get(c)->Get();

                          listresult->list.Add(elementresult); 

                          status = true;
                        }
                       else
                        {
                          status = false;
                          break;
                        }         
                    }
                }                      
            } 
        }
    }

  list_name.DeleteContents();
  list_ID.DeleteContents();
    
  return status;
}              


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::ListUpdateAvailable(XWINDOWSWINGET_LISTRESULT* listresult)
* @brief      list update available
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  listresult : Output listresult.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::ListUpdateAvailable(XWINDOWSWINGET_LISTRESULT* listresult)
{ 
  XVECTOR<XSTRING*>   list_name;
  XVECTOR<XSTRING*>   list_ID;
  XVECTOR<XSTRING*>   list_actualversion;
  XVECTOR<XSTRING*>   list_availableversion;
  XBUFFER             buffer_output;
  bool                status = false;

  if(!listresult)
    {
      return false;    
    }

  status = GenerateColumnList(__L("Get-WinGetPackage | Where-Object IsUpdateAvailable | Select-Object Name"), &list_name);
  if(status)
    {
      status = GenerateColumnList(__L("Get-WinGetPackage | Where-Object IsUpdateAvailable | Select-Object ID"), &list_ID);
      if(status)
        {             
          status = GenerateColumnList(__L("Get-WinGetPackage | Where-Object IsUpdateAvailable | Select-Object InstalledVersion"), &list_actualversion);
          if(status)
            {             
              status = GenerateColumnList(__L("Get-WinGetPackage | Where-Object IsUpdateAvailable | Select-Object AvailableVersions"), &list_availableversion);
              if(status)
                {               

                  if(list_name.GetSize() == list_ID.GetSize())
                    {    
                      for(int c=0; c<list_name.GetSize(); c++)
                        {
                          if(!list_name.Get(c)->IsEmpty() || !list_ID.Get(c)->IsEmpty())
                            {                        
                              XWINDOWSWINGET_ELEMENTRESULT* elementresult = GEN_NEW XWINDOWSWINGET_ELEMENTRESULT();
                              if(elementresult)  
                                {
                                  elementresult->name             = list_name.Get(c)->Get();
                                  elementresult->ID               = list_ID.Get(c)->Get();
                                  elementresult->actualversion    = list_actualversion.Get(c)->Get();
                                  elementresult->availableversion = list_availableversion.Get(c)->Get();

                                  listresult->list.Add(elementresult); 

                                  status = true;
                                }
                               else
                                {
                                  status = false;
                                  break;
                                }         
                            }
                        }                      
                    } 
                }
            }
        }
    }

  list_name.DeleteContents();
  list_ID.DeleteContents();

  list_actualversion.DeleteContents();
  list_availableversion.DeleteContents();
    
  return status;
}              


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::Find(XCHAR* search, XWINDOWSWINGET_LISTRESULT* listresult)
* @brief      find
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  search : Search pointer to use.
* @param[in]  listresult : Output listresult.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::Find(XCHAR* search, XWINDOWSWINGET_LISTRESULT* listresult)
{ 
  XSTRING             cmdstr;
  XSTRING             mask;
  XVECTOR<XSTRING*>   list_name;
  XVECTOR<XSTRING*>   list_ID;
  XVECTOR<XSTRING*>   list_actualversion;
  XBUFFER             buffer_output;
  bool                status = false;

  if(!search)
    {
      return false;    
    }

  if(!listresult)
    {
      return false;    
    }

  mask.Format(__L("Find-WinGetPackage -Name \"%s\" | Select-Object"), search);

  cmdstr.Format(__L("%s %s"), mask.Get(), __L("Name"));
  status = GenerateColumnList(cmdstr.Get(), &list_name);
  if(status)
    {
      cmdstr.Format(__L("%s %s"), mask.Get(), __L("ID"));
      status = GenerateColumnList(cmdstr.Get(), &list_ID);
      if(status)
        {             
          cmdstr.Format(__L("%s %s"), mask.Get(), __L("Version"));
          status = GenerateColumnList(cmdstr.Get(), &list_actualversion);
          if(status)
            {                           
              if(list_name.GetSize() == list_ID.GetSize())
                {    
                  for(int c=0; c<list_name.GetSize(); c++)
                    {
                      if(!list_name.Get(c)->IsEmpty() || !list_ID.Get(c)->IsEmpty())
                        {                        
                          XWINDOWSWINGET_ELEMENTRESULT* elementresult = GEN_NEW XWINDOWSWINGET_ELEMENTRESULT();
                          if(elementresult)  
                            {
                              elementresult->name             = list_name.Get(c)->Get();
                              elementresult->ID               = list_ID.Get(c)->Get();
                              elementresult->actualversion    = list_actualversion.Get(c)->Get();
                                  
                              listresult->list.Add(elementresult); 

                              status = true;
                            }
                            else
                            {
                              status = false;
                              break;
                            }         
                        }
                    }                      
                }                 
            }
        }
    }

  list_name.DeleteContents();
  list_ID.DeleteContents();
  list_actualversion.DeleteContents();
    
  return status;
}              


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::List(bool updateavaible, XSTRING& jsonresult)
* @brief      list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  updateavaible : Updateavaible value.
* @param[in]  jsonresult : Output jsonresult.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::List(bool updateavaible, XSTRING& jsonresult)
{ 
  XSERIALIZATIONMETHOD*       serializationmethod;
  XFILEJSON                   xfileJSON;      
  XWINDOWSWINGET              winget;  
  XWINDOWSWINGET_LISTRESULT   listresult;
  bool                        status = false;
 
  status = updateavaible?winget.ListUpdateAvailable(&listresult):winget.List(&listresult);
  if(!status)
    {
      return false;
    }
     
  serializationmethod = XSERIALIZABLE::CreateInstance(xfileJSON);

  status = listresult.DoSerialize(serializationmethod);
  xfileJSON.EncodeAllLines(false);  

  xfileJSON.GetAllInOneLine(jsonresult);

  // xfileJSON.ShowTraceJSON(XTRACE_COLOR_BLUE);
 
  GEN_DELETE serializationmethod;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::Find(XCHAR* search, XSTRING& jsonresult)
* @brief      find
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  search : Search pointer to use.
* @param[in]  jsonresult : Output jsonresult.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::Find(XCHAR* search, XSTRING& jsonresult)
{ 
  if(!search)
    {
      return false;
    }

  XSERIALIZATIONMETHOD*       serializationmethod;
  XFILEJSON                   xfileJSON;      
  XWINDOWSWINGET              winget;  
  XWINDOWSWINGET_LISTRESULT   listresult;
  bool                        status = false;
 
  status = winget.Find(search, &listresult);
  if(!status)
    {
      return false;
    }
     
  serializationmethod = XSERIALIZABLE::CreateInstance(xfileJSON);

  status = listresult.DoSerialize(serializationmethod);
  xfileJSON.EncodeAllLines(false);  

  xfileJSON.GetAllInOneLine(jsonresult);

  //xfileJSON.ShowTraceJSON(XTRACE_COLOR_BLUE);
 
  GEN_DELETE serializationmethod;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::ApplicationOperation(XWINDOWSWINGET_APPLICATIONOPERATION appoper, XCHAR* ID, bool force)
* @brief      application operation
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  appoper : Appoper value.
* @param[in]  ID : Identifier to use.
* @param[in]  force : Force value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::ApplicationOperation(XWINDOWSWINGET_APPLICATIONOPERATION appoper, XCHAR* ID, bool force)
{
  XSTRING  cmdstr;
  XBUFFER  buffer_output;
  bool     status = false;

  if(!ID)
    {
      return false;  
    }

  switch(appoper)
    {
      case XWINDOWSWINGET_APPLICATIONOPERATION_UNKNOWN        : return status;
      case XWINDOWSWINGET_APPLICATIONOPERATION_INSTALL        : cmdstr.Format(__L("Install-WinGetPackage"));    break;
      case XWINDOWSWINGET_APPLICATIONOPERATION_UPDATEVERSION  : cmdstr.Format(__L("Update-WinGetPackage"));     break;
      case XWINDOWSWINGET_APPLICATIONOPERATION_UNINSTALL      : cmdstr.Format(__L("Uninstall-WinGetPackage"));  break;      
    }

  cmdstr.AddFormat(__L(" -Id \"%s\""), ID);

  if(force)
    {
      cmdstr.AddFormat(__L(" -force"));
    }


  cmdstr.AddFormat(__L(" | Select-Object Status"));

  status = Exec(cmdstr.Get(), buffer_output);
  if(status)
    {
      if(!buffer_output.GetSize())
        {
          status = true;
        }  
    }

  XVECTOR<XSTRING*> statuslist;
  XSTRING           result;

  result.ConvertFromUTF8(buffer_output);
  GenerateList(result, &statuslist);

  status = statuslist.GetSize()?true:false;

  if(status)
    {  
      XSTRING* statusstr = statuslist.Get(0);
      if(statusstr)
        {
          status = false;

          if(!statusstr->Compare(__L("NoApplicableUpgrade"), false))  
            {
              status = true;   
            }

          if(!statusstr->Compare(__L("Ok"), false))  
            {
              status = true;   
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET::XWINDOWSWINGET()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET::XWINDOWSWINGET()
{
  Clean();

  //InstallModule();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWINGET::~XWINDOWSWINGET()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWINGET::~XWINDOWSWINGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::Exec(XCHAR* params, XBUFFER& output)
* @brief      exec
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  params : Params pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::Exec(XCHAR* params, XBUFFER& output)
{
  output.Delete();

  // Construir comando PowerShell (igual que antes)
  XSTRING cmd_str;
  //cmd_str.Format(__L("powershell.exe -NonInteractive -Command \"%s\" 2>&1"), params);
  cmd_str.Format(__L("pwsh.exe -NonInteractive -Command \"%s\" 2>&1"), params);

  
  // Crear pipe para capturar salida
  SECURITY_ATTRIBUTES sa;  
  STARTUPINFO         si;  
  PROCESS_INFORMATION pi;    
  HANDLE              hread = NULL;
  HANDLE              hwrite = NULL;

  ZeroMemory(&si, sizeof(si));  
  ZeroMemory(&pi, sizeof(pi));

  sa.nLength              = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle       = TRUE;

  if(!CreatePipe(&hread, &hwrite, &sa, 0))
    {
      return false;
    }

  // El extremo de lectura NO debe heredarse
  SetHandleInformation(hread, HANDLE_FLAG_INHERIT, 0);

  si.cb           = sizeof(si);
  si.dwFlags      = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow  = SW_HIDE;
  si.hStdOutput   = hwrite;
  si.hStdError    = hwrite;
  si.hStdInput    = GetStdHandle(STD_INPUT_HANDLE);

  BOOL ok = CreateProcess(NULL,
                          (XCHAR*)cmd_str.Get(),  // buffer modificable
                          NULL,
                          NULL,
                          TRUE,                   // heredar handles
                          CREATE_NO_WINDOW,       // CLAVE: evita CMD
                          NULL,
                          NULL,
                          &si,
                          &pi);

  CloseHandle(hwrite);

  if(!ok)
  {
    CloseHandle(hread);
    return false;
  }

  BYTE    buffer[4096];
  DWORD   bytes_read = 0;

  while(ReadFile(hread, buffer, sizeof(buffer), &bytes_read, NULL) && bytes_read > 0)
    {
      output.Add(buffer, (XDWORD)bytes_read);
    }

  CloseHandle(hread);
  
  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  NormalizeUnicode(output.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSWINGET::LooksLikeUTF16(XBYTE* buffer, int size)
* @brief      looks like UTf16
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSWINGET::LooksLikeUTF16(XBYTE* buffer, int size) 
{
  return (size >= 2 && buffer[0] == 0xFF && buffer[1] == 0xFE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::LooksLikeUTF8(XBYTE* buffer, int size)
* @brief      looks like UTf8
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::LooksLikeUTF8(XBYTE* buffer, int size) 
{
  int i = 0;

  while(i < size) 
    {
      XBYTE c = buffer[i];

      if(c < 0x80) 
        { 
          i++; 
          continue; 
        }

      if((c & 0xE0) == 0xC0 && i + 1 < size && (buffer[i+1] & 0xC0) == 0x80) 
        { 
          i += 2; 
          continue; 
        }

      if((c & 0xF0) == 0xE0 && i + 2 < size && (buffer[i+1] & 0xC0) == 0x80 && (buffer[i+2] & 0xC0) == 0x80) 
        { 
          i += 3; 
          continue; 
        }

      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET::ConvertUTF16ToUTF8(XBYTE* data)
* @brief      convert UTf16 to UTf8
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET::ConvertUTF16ToUTF8(XBYTE* data) 
{
  int       len   = (int)strlen((char*)data);
  wchar_t*  w     = (wchar_t*)(data + 2);
  int       wlen  = (len - 2) / 2;
  XBYTE*    out   = (XBYTE*)malloc(wlen * 3 + 1);
  
  if(!out) 
    {
      return;
    }
  
  int r = WideCharToMultiByte(CP_UTF8, 0, w, wlen, (char*)out, wlen * 3, NULL, NULL);
  if(r < 0) 
    {
      r = 0;
    }
  
  out[r] = 0;
  
  strcpy((char*)data, (char*)out);
  
  free(out);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET::CP437ToASCII(XBYTE* dst, const XBYTE* src, int max)
* @brief      Cp437 to ASCIi
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  dst : Dst pointer to use.
* @param[in]  src : Src value.
* @param[in]  max : Max value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET::CP437ToASCII(XBYTE* dst, const XBYTE* src, int max) 
{
  int di = 0;
  
  for(int i = 0; src[i] && di < max - 1; i++) 
    {
      XBYTE c = (XBYTE)src[i];
      if(c < 128) 
        {
          dst[di++] = (char)c;
        }
       else 
        {
          dst[di++] = '?';
        }
    }

  dst[di] = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET::NormalizeUnicode(XBYTE* data)
* @brief      normalize unicode
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET::NormalizeUnicode(XBYTE* data) 
{
  if(!data)
    {
      return;
    }

  int     len    = (int)strlen((char*)data);
  XBYTE*  buffer = (XBYTE*)data;
  
  if(LooksLikeUTF16(buffer, len)) 
    {
      ConvertUTF16ToUTF8(data);
      return;
    }
  
  if(!LooksLikeUTF8(buffer, len)) 
    {
      char* tmp = GEN_NEW char[32768];
      
      if(tmp)
        {  
          memset(tmp, 0, 32768);

          CP437ToASCII((XBYTE*)tmp, data, 32768);
          strcpy((char*)data, tmp);

          GEN_DELETE_ARRAY tmp;        
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::GenerateColumnList(XCHAR* ask, XVECTOR<XSTRING*>* list)
* @brief      generate column list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ask : Ask pointer to use.
* @param[in]  list : List pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::GenerateColumnList(XCHAR* ask, XVECTOR<XSTRING*>* list) 
{
  XSTRING  result;
  XBUFFER  buffer_output;
  bool     status;

  status = Exec(ask, buffer_output);
  if(!status)
    {
      return false;
    }    

  result.ConvertFromUTF8(buffer_output);

  return GenerateList(result, list);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWINGET::GenerateList(XSTRING& result, XVECTOR<XSTRING*>* list)
* @brief      generate list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  result : Output result.
* @param[in]  list : List pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWINGET::GenerateList(XSTRING& result, XVECTOR<XSTRING*>* list) 
{  
  XSTRING literal1  = __L("\r\n\x1b[32;1m--");
  XSTRING literal2  = __L("\x1B[0m\r\n");
  int     start     =  XSTRING_NOTFOUND;
  int     end       =  XSTRING_NOTFOUND;
  
  start = result.Find(literal1.Get(), true);
  if(start == XSTRING_NOTFOUND)
    {
      return false;
    }

  start += literal1.GetSize();

  end = result.Find(literal2.Get(), true, start);
  if(end == XSTRING_NOTFOUND)
    {      
      return false;      
    }

  start = end + literal2.GetSize();;
  
  while(1)
    {
      end = result.Find(__L("\r\n"), true, start);
      if(end == XSTRING_NOTFOUND)
        {
          break;
        }
  
      XSTRING* element = GEN_NEW XSTRING();
      if(!element)
        {
          break;
        }

      result.Copy(start, end, (*element));

      element->DeleteCharacter(__C(' '), XSTRINGCONTEXT_TO_END);

      list->Add(element);

      start = end + 2;
    }

  if(!list->GetSize())
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWINGET::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWINGET::Clean()
{

}






