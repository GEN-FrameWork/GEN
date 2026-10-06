/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSVirtualDesktopManager.cpp
* 
* @class      XWINDOWSVIRTUALDESKTOPMANAGER
* @brief      WINDOWS eXtended Utils Virtual Desktop Manager class
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

#include "XWINDOWSVirtualDesktopManager.h"

#include "XPath.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XWINDOWSVIRTUALDESKTOPMANAGER*     XWINDOWSVIRTUALDESKTOPMANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSVIRTUALDESKTOP::XWINDOWSVIRTUALDESKTOP()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSVIRTUALDESKTOP::XWINDOWSVIRTUALDESKTOP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSVIRTUALDESKTOP::~XWINDOWSVIRTUALDESKTOP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSVIRTUALDESKTOP::~XWINDOWSVIRTUALDESKTOP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XWINDOWSVIRTUALDESKTOP::GetName()
* @brief      Get name
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XWINDOWSVIRTUALDESKTOP::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSVIRTUALDESKTOP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSVIRTUALDESKTOP::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSVIRTUALDESKTOPMANAGER& XWINDOWSVIRTUALDESKTOPMANAGER::GetInstance()
* @brief      Get instance
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XWINDOWSVIRTUALDESKTOPMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSVIRTUALDESKTOPMANAGER& XWINDOWSVIRTUALDESKTOPMANAGER::GetInstance()
{
  if(!instance) instance = GEN_NEW XWINDOWSVIRTUALDESKTOPMANAGER();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::DelInstance()
* @brief      Del instance
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::DelInstance()
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
* @fn         XVECTOR<XWINDOWSVIRTUALDESKTOP*>* XWINDOWSVIRTUALDESKTOPMANAGER::EnumAllDesktops()
* @brief      Enum all desktops
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XVECTOR<XWINDOWSVIRTUALDESKTOP*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWINDOWSVIRTUALDESKTOP*>* XWINDOWSVIRTUALDESKTOPMANAGER::EnumAllDesktops()
{
  listdesktops.DeleteContents();
  listdesktops.DeleteAll();

  HWINSTA handle = GetProcessWindowStation();

  BOOL status = EnumDesktops(handle, (DESKTOPENUMPROC)EnumDesktopProc, (LPARAM)(this));

  return &listdesktops;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::AddDesktopToList(XSTRING& name)
* @brief      Add desktop to list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::AddDesktopToList(XSTRING& name)
{
  XWINDOWSVIRTUALDESKTOP* desktop = GEN_NEW XWINDOWSVIRTUALDESKTOP();
  if(!desktop) return false;

  desktop->GetName()->Set(name);

  listdesktops.Add(desktop);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::CreateNewDesktop(XCHAR* name, bool addexplorer)
* @brief      Create GEN_NEW desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  addexplorer : Addexplorer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::CreateNewDesktop(XCHAR* name, bool addexplorer)
{
  if(!name) return false;

  if(IsDesktopCreated(name))
    {
      //XTRACE_PRINTCOLOR(1, _L("CreateNewDesktop [%s] -> is desktop created."), name);
      return false;
    }

  SECURITY_ATTRIBUTES sattribute  = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  HDESK               hnewdesktop = NULL;
  bool                status      = false;

  hnewdesktop = ::CreateDesktop(name, NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, &sattribute);
  if(hnewdesktop)
    {
      if(addexplorer)
        {
          XPATH pathexplorer;

          pathexplorer.AdjustSize(_MAXSTR);
          GetWindowsDirectory(pathexplorer.Get(), _MAXSTR-1);
          pathexplorer.AdjustSize();
          pathexplorer += _L("\\Explorer.Exe");

          LaunchApplication(pathexplorer.Get(), name);
        }

      CloseDesktop(hnewdesktop);

      EnumAllDesktops();

      status = true;
    }

  //XTRACE_PRINTCOLOR((status?1:4), _L("CreateNewDesktop [%s] -> %s"), name, (status?_L("Ok"):_L("Error")));

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::CreateNewDesktop(XSTRING& name, bool addexplorer)
* @brief      Create GEN_NEW desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  addexplorer : Addexplorer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::CreateNewDesktop(XSTRING& name, bool addexplorer)
{
  return CreateNewDesktop(name.Get(), addexplorer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchDesktop(XCHAR* desktopname, bool inherit)
* @brief      Switch desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname pointer to use.
* @param[in]  inherit : Inherit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchDesktop(XCHAR* desktopname, bool inherit)
{
  if(!desktopname) return false;

  bool status = false;

  //HDESK hdesktoptoswitch = OpenDesktop(desktopname, DF_ALLOWOTHERACCOUNTHOOK, (inherit?TRUE:FALSE), GENERIC_ALL);
  HDESK hdesk = OpenDesktop(desktopname, 0, FALSE, MAXIMUM_ALLOWED);          
  //XTRACE_PRINTCOLOR((hdesk?1:4), _L("OpenDesktop [%s] -> %s"), desktopname, (hdesk?_L("Ok"):_L("Error")));
  if(hdesk)
    {
      status = (::SwitchDesktop(hdesk) == TRUE)?true:false;
      CloseDesktop(hdesk);
    }

  //XTRACE_PRINTCOLOR((status?1:4), _L("SwitchDesktop [%s] -> %s"), desktopname, (status?_L("Ok"):_L("Error")));

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchDesktop(XSTRING& desktopname, bool inherit)
* @brief      Switch desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname value.
* @param[in]  inherit : Inherit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchDesktop(XSTRING& desktopname, bool inherit)
{
  return SwitchDesktop(desktopname.Get(), inherit);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::IsCurrentDesktop(XCHAR* desktopname)
* @brief      Is current desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::IsCurrentDesktop(XCHAR* desktopname)
{
  HDESK    hcurrentdesktop    = GetThreadDesktop(GetCurrentThreadId());
  XSTRING  currentdesktopname;
  DWORD    sizedesktopname    = 0;

  currentdesktopname.AdjustSize(_MAXSTR);
  GetUserObjectInformation(hcurrentdesktop, UOI_NAME, currentdesktopname.Get(), _MAXSTR - 1, &sizedesktopname);
  currentdesktopname.AdjustSize();

  return (!currentdesktopname.Compare(desktopname))?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::IsCurrentDesktop(XSTRING& desktopname)
* @brief      Is current desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::IsCurrentDesktop(XSTRING& desktopname)
{
  return IsCurrentDesktop(desktopname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::IsDesktopCreated(XCHAR* desktopname)
* @brief      Is desktop created
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::IsDesktopCreated(XCHAR* desktopname)
{
  bool found = false;
  for(XDWORD c=0; c<listdesktops.GetSize(); c++)
    {
      if(!listdesktops.Get(c)->GetName()->Compare(desktopname, true))
        {
          found = true;
          break;
        }
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::IsDesktopCreated(XSTRING& desktopname)
* @brief      Is desktop created
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::IsDesktopCreated(XSTRING& desktopname)
{
  return IsDesktopCreated(desktopname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchProcessSpecifiedDesktop(XCHAR* desktopname)
* @brief      Switch process specified desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchProcessSpecifiedDesktop(XCHAR* desktopname)
{
  BOOL status = FALSE;

  HWINSTA hwinsta0 = OpenWindowStation(TEXT("WinSta0"), FALSE, MAXIMUM_ALLOWED);
  //XTRACE_PRINTCOLOR((hwinsta0?1:4), _L("SwitchProcessSpecifiedDesktop [%s] -> OpenWindowStation %08X"), desktopname, hwinsta0);
  if(hwinsta0)
    {
      status = SetProcessWindowStation(hwinsta0);
      //XTRACE_PRINTCOLOR((status?1:4), _L("SwitchProcessSpecifiedDesktop [%s]  SetProcessWindowStation -> %s"), desktopname, (status?_L("Ok"):_L("Error")));
      if(status)
        {                
          HDESK hdesk = OpenDesktop(desktopname, 0, FALSE, MAXIMUM_ALLOWED);          
          //XTRACE_PRINTCOLOR//((hdesk?1:4), _L("SwitchProcessSpecifiedDesktop [%s]  OpenDesktop -> %08X"), desktopname, hdesk);
          if(hdesk)
            {
              status = SetThreadDesktop(hdesk);
              //XTRACE_PRINTCOLOR((status?1:4), _L("SwitchProcessSpecifiedDesktop [%s] -> SetThreadDesktop %s error [%d]"), desktopname, (status?_L("Ok"):_L("Error")), GetLastError());
              CloseDesktop(hdesk);
            }                  
        }

      CloseWindowStation(hwinsta0);
    }

  return (status?true:false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchProcessSpecifiedDesktop(XSTRING& desktopname)
* @brief      Switch process specified desktop
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  desktopname : Desktopname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::SwitchProcessSpecifiedDesktop(XSTRING& desktopname)
{
  return SwitchProcessSpecifiedDesktop(desktopname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::IsUserInteractive()
* @brief      Is user interactive
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::IsUserInteractive()
{
  BOOL isuserinteractive = FALSE;

  HWINSTA hwinstation = GetProcessWindowStation();
  if(hwinstation != NULL)
    {
      USEROBJECTFLAGS uof = { 0 };
      if(GetUserObjectInformation(hwinstation, UOI_FLAGS, &uof, sizeof(USEROBJECTFLAGS), NULL) && ((uof.dwFlags & WSF_VISIBLE) == 0))
             isuserinteractive = FALSE;
        else isuserinteractive = TRUE;
    }

  return isuserinteractive?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSVIRTUALDESKTOPMANAGER::XWINDOWSVIRTUALDESKTOPMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSVIRTUALDESKTOPMANAGER::XWINDOWSVIRTUALDESKTOPMANAGER()
{
  Clean();

  enumdesktopfunc = (DESKTOPENUMPROC)XWINDOWSVIRTUALDESKTOPMANAGER::EnumDesktopProc;

  EnumAllDesktops();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSVIRTUALDESKTOPMANAGER::~XWINDOWSVIRTUALDESKTOPMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSVIRTUALDESKTOPMANAGER::~XWINDOWSVIRTUALDESKTOPMANAGER()
{
  listdesktops.DeleteContents();
  listdesktops.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSVIRTUALDESKTOPMANAGER::LaunchApplication(XCHAR* applicationfilepath, XCHAR* desktopname)
* @brief      Launch application
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applicationfilepath : Applicationfilepath pointer to use.
* @param[in]  desktopname : Desktopname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSVIRTUALDESKTOPMANAGER::LaunchApplication(XCHAR* applicationfilepath, XCHAR* desktopname)
{
  if(!applicationfilepath) return false;
  if(!desktopname)         return false;

  if(!XSTRING::GetSize(applicationfilepath)) return false;
  if(!XSTRING::GetSize(desktopname))         return false;

  XPATH directoryapp;
  bool  status        = false;

  directoryapp = applicationfilepath;

  directoryapp.SetOnlyDriveAndPath();

  STARTUPINFO         sinfo = { 0 };
  PROCESS_INFORMATION pinfo = { 0 };

  sinfo.cb        = sizeof(sinfo);
  sinfo.lpDesktop = desktopname;

  //Lanuching a application into dekstop
  BOOL processreturn = CreateProcess(applicationfilepath, NULL, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, directoryapp.Get(), &sinfo, &pinfo);
  if(processreturn) status = true;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL __stdcall XWINDOWSVIRTUALDESKTOPMANAGER::EnumDesktopProc(LPTSTR namedesktop, LPARAM param)
* @brief      Enum desktop proc
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  namedesktop : Namedesktop value.
* @param[in]  param : Param value.
* 
* @return     BOOL __stdcall : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL __stdcall XWINDOWSVIRTUALDESKTOPMANAGER::EnumDesktopProc(LPTSTR namedesktop, LPARAM param)
{
  XWINDOWSVIRTUALDESKTOPMANAGER* vdm = (XWINDOWSVIRTUALDESKTOPMANAGER*)param;
  if(!vdm) return false;

  XSTRING name;

  name = namedesktop;

  vdm->AddDesktopToList(name);

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSVIRTUALDESKTOPMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSVIRTUALDESKTOPMANAGER::Clean()
{
  enumdesktopfunc     = NULL;
}









