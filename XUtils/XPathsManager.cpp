/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XPathsManager.cpp
* 
* @class      XPATHSMANAGER
* @brief      eXtended Utils Paths manager class
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

#include "XPathsManager.h"

#include "XFactory.h"
#include "XDir.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XPATHSMANAGER*  XPATHSMANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGERSECTION::XPATHSMANAGERSECTION()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGERSECTION::XPATHSMANAGERSECTION()
{
  Clean();

  xpath = GEN_NEW XPATH();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGERSECTION::~XPATHSMANAGERSECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGERSECTION::~XPATHSMANAGERSECTION()
{
  if(xpath) GEN_DELETE xpath;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATHSMANAGERSECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATHSMANAGERSECTION::Clean()
{
  type = XPATHSMANAGERSECTIONTYPE_UNKNOWN;
  xpath = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGER& XPATHSMANAGER::GetInstance()
* @brief      Get instance
* @ingroup    XUTILS
* 
* @return     XPATHSMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGER& XPATHSMANAGER::GetInstance()
{
  if(!instance) instance = GEN_NEW XPATHSMANAGER();
  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::DelInstance()
* @brief      Del instance
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::DelInstance()
{
  if(!instance) return false;

  GEN_DELETE instance;
  instance = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::GetPathOfSection(XPATHSMANAGERSECTIONTYPE sectiontype, XPATH& xpath, bool addroot)
* @brief      Get path of section
* @ingroup    XUTILS
* 
* @param[in]  sectiontype : Sectiontype value.
* @param[in]  xpath : Path to use.
* @param[in]  addroot : Addroot value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::GetPathOfSection(XPATHSMANAGERSECTIONTYPE sectiontype, XPATH& xpath, bool addroot)
{
  bool found = false;

  xpath.Empty();

  if(addroot && (sectiontype!=XPATHSMANAGERSECTIONTYPE_ROOT))
    {
      for(XDWORD c=0; c<pathsections.GetSize(); c++)
        {
          XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
          if(pathsection)
            {
              if(pathsection->type == XPATHSMANAGERSECTIONTYPE_ROOT)
                {
                  xpath = pathsection->xpath->Get();
                  xpath.Slash_Add();
                  break;
                }
            }
        }
    }

  for(XDWORD c=0; c<pathsections.GetSize(); c++)
    {
      XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
      if(pathsection)
        {
          if(pathsection->type == sectiontype)
            {
              xpath += pathsection->xpath->Get();

              found = true;
              break;
            }
        }
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XPATHSMANAGER::GetAppExecPath()
* @brief      Get app exec path
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XPATHSMANAGER::GetAppExecPath()
{
  return appexecpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::GetAppExecPathWithoutAsset(XCHAR* assetlevel, XPATH& xpath)
* @brief      Get app exec path without asset
* @ingroup    XUTILS
* 
* @param[in]  assetlevel : Assetlevel pointer to use.
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::GetAppExecPathWithoutAsset(XCHAR* assetlevel, XPATH& xpath)
{
  XSTRING assetname;

  xpath.Empty();

  assetname = assetlevel;

  GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);     

  int index = xpath.Find(assetlevel, false);
  if(index == XSTRING_NOTFOUND) return true;

  xpath.DeleteCharacters(index, assetname.GetSize());  

  xpath.Slash_Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGERSECTION* XPATHSMANAGER::GetPathSection(XPATHSMANAGERSECTIONTYPE sectiontype)
* @brief      Get path section
* @ingroup    XUTILS
* 
* @param[in]  sectiontype : Sectiontype value.
* 
* @return     XPATHSMANAGERSECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGERSECTION* XPATHSMANAGER::GetPathSection(XPATHSMANAGERSECTIONTYPE sectiontype)
{
  if(pathsections.IsEmpty()) return NULL;

  for(XDWORD c=0; c<pathsections.GetSize(); c++)
    {
      XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
      if(pathsection)
        {
          if(pathsection->type == sectiontype)
            {
              return pathsection;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype, XSTRING& xpath)
* @brief      Add path section
* @ingroup    XUTILS
* 
* @param[in]  sectiontype : Sectiontype value.
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype, XSTRING& xpath)
{
  XPATHSMANAGERSECTION* xpathsection = GetPathSection(sectiontype);
  if(!xpathsection)
    {
      xpathsection = GEN_NEW XPATHSMANAGERSECTION();
      if(!xpathsection) return false;

      xpathsection->type  = sectiontype;
      (*xpathsection->xpath) = xpath.Get();

      if(!xpathsection->xpath->IsEmpty())
        {
          xpathsection->xpath->Slash_Add();
          xpathsection->xpath->Slash_Normalize();
        }

      pathsections.Add(xpathsection);
    }
   else
    {
      xpathsection->xpath->Set(xpath.Get());
      xpathsection->xpath->Slash_Add();
      xpathsection->xpath->Slash_Normalize();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype, const XCHAR* string)
* @brief      Add path section
* @ingroup    XUTILS
* 
* @param[in]  sectiontype : Sectiontype value.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype, const XCHAR* string)
{
  XSTRING path(string);

  return AddPathSection(sectiontype, path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::DeleteAllPathSections()
* @brief      Delete all path sections
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::DeleteAllPathSections()
{
  if(pathsections.IsEmpty()) return false;

  pathsections.DeleteContents();

  pathsections.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::CreateAllPathSectionOnDisk()
* @brief      Create all path section on disk
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::CreateAllPathSectionOnDisk()
{
  XDIR* xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  bool status = true;

  for(XDWORD c=0; c<pathsections.GetSize(); c++)
    {
      XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
      if(pathsection)
        {
          XPATH xpath;

          if(GetPathOfSection(pathsection->type, xpath, true))
            {
              if(!xdir->Exist(xpath.Get()))
                {
                  if(!xdir->Make(xpath.Get()))
                    {
                      status = false;
                    }

                  //XTRACE_PRINTCOLOR((status?2:4), __L("Dir Create: %s "), xpath->Get());
                }
               else
                {
                  //XTRACE_PRINTCOLOR(2, __L("Dir Exist: %s "), xpath->Get());
                }
            }
        }
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATHSMANAGER::AdjustRootPathDefault(XCHAR* assetsdirname)
* @brief      Adjust root path default
* @ingroup    XUTILS
* 
* @param[in]  assetsdirname : Assetsdirname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATHSMANAGER::AdjustRootPathDefault(XCHAR* assetsdirname)
{ 
  XDIR* xdir;
  XPATH xpathroot;
  XPATH xpathtest;
  bool  founddir    = false;
  int   indexfound  = 0;
  bool  status      = false;

  xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(!GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot))
    {
      // No ROOT section has been registered yet (typical on the very first call, before any AddPathSection):
      // fall back to the application executable path as the search base instead of operating on an empty
      // XPATH, which Slash_Add() below would otherwise turn into the real filesystem root ("/").
      if(!appexecpath || appexecpath->IsEmpty())
        {
          GEN_XFACTORY.Delete_Dir(xdir);
          return false;
        }

      xpathroot = appexecpath->Get();
    }

  xpathroot.Slash_Add();

  do{ indexfound = xpathroot.Find(__L("/"), false, indexfound);
      if(indexfound == XSTRING_NOTFOUND)   indexfound = xpathroot.Find(__L("\\"), false, indexfound);

      if(indexfound != XSTRING_NOTFOUND)
        {
          xpathroot.Copy(0, indexfound, xpathtest);

          xpathtest.Slash_Add();
          xpathtest.Add(assetsdirname);

          founddir = xdir->Exist(xpathtest);
          if(founddir)
            {
              AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathtest);
              status  = true;
              break;    
            }

          indexfound++;
        } 

    } while(indexfound != XSTRING_NOTFOUND);

  if(!founddir)
    {
      xpathtest = xpathroot;
      xpathtest.Slash_Add();
      xpathtest.Add(assetsdirname);

      if(xdir->Make(xpathtest, false))
        {
          AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathtest);
          status  = true;      
        }
    }
  
  GEN_XFACTORY.Delete_Dir(xdir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGER::XPATHSMANAGER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGER::XPATHSMANAGER()
{
  Clean();

  appexecpath = GEN_NEW XPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATHSMANAGER::~XPATHSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATHSMANAGER::~XPATHSMANAGER()
{
  DeleteAllPathSections();

  if(appexecpath)
    {
      GEN_DELETE appexecpath;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATHSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATHSMANAGER::Clean()
{
  appexecpath = NULL;
}





