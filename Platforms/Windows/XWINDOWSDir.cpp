/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSDir.cpp
* 
* @class      XWINDOWSDIR
* @brief      WINDOWS eXtended Utils Directory class
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

#include "XWINDOWSDir.h"

#include <stdio.h>

#include "XString.h"
#include "XPath.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSDIR::XWINDOWSDIR(): XDIR()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDIR::XWINDOWSDIR(): XDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSDIR::~XWINDOWSDIR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDIR::~XWINDOWSDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::Exist(XCHAR* path)
{
  XDIRELEMENT search;
  XPATH       xpath;
  XSTRING     patternsearch;
  bool        status = false;

  xpath = path;
  xpath.Slash_Delete();
  xpath.GetNamefileExt(patternsearch);
  xpath.SetOnlyDriveAndPath();

  if(xpath.IsEmpty())
    {
      XDWORD size = _MAXPATH;
      xpath.AdjustSize(size, false, _L(" "));

      GetCurrentDirectory(size, xpath.Get());
    }

  if(FirstSearch(xpath.Get(), patternsearch.Get(), &search))
    {
      status = true;

      while(NextSearch(&search))
        {

        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::Make(XCHAR* path, bool recursive)
* @brief      Make
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::Make(XCHAR* path, bool recursive)
{
  if(!path)     
    {
      return false;
    }

  if(!path[0]) 
    {
      return false;
    }

  if(Exist(path)) 
    {
      return true;
    }

  XPATH   xpath;

  xpath = path;

  xpath.Slash_Add();
  xpath.Slash_Normalize(true);
  
  if(recursive)
    {      
      XPATH   xpathsequence;
      XSTRING pathpart;
      int     index = 0;

      xpathsequence.Empty();
    
      do{
          if(xpath.GetPathInSequence(index, pathpart))
            {
              xpathsequence += pathpart;
              xpathsequence += _L("\\");

              if(pathpart.Character_GetLast()!=_C(':'))
                {
                  if(!Exist(xpathsequence.Get()))
                    {
                      //XTRACE_PRINTCOLOR(2, _L("---> Make: %s"), xpathsequence.Get());

                      int result = CreateDirectory(xpathsequence.Get(), NULL);
                      if(!result) 
                        {
                          return false;
                        }
                    }
                }

            } else break;

          index++;

        } while(1);
    }
   else
    {
      int result = CreateDirectory(xpath.Get(), NULL);
      if(!result) 
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::ChangeTo(XCHAR* path)
* @brief      Change to
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::ChangeTo(XCHAR* path)
{
  if(!path)    
    {
      return false;
    }

  if(!path[0]) 
    {
      return false;
    }

  int result = SetCurrentDirectory(path);

  return result?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::Delete(XCHAR* path, bool all)
* @brief      Delete resource
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* @param[in]  all : All value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::Delete(XCHAR* path, bool all)
{
  XDIRELEMENT search;
  XPATH       xpathname;
  XSTRING     patternsearch;

  if(all)
    {
      xpathname     = path;
      patternsearch = _L("*");

      if(FirstSearch(xpathname.Get(), patternsearch.Get(), &search))
        {
          do { xpathname  = path;
               xpathname.Slash_Add();
               xpathname += search.GetNameFile()->Get();

               if(search.GetType() == XDIRELEMENTTYPE_DIR)
                     Delete(xpathname.Get(), all);
                else DeleteFile(xpathname.Get());

             } while(NextSearch(&search));
        }
    }

  BOOL result = RemoveDirectory(path);

  return result?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::GetActual(XPATH& xpath)
* @brief      Get actual
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::GetActual(XPATH& xpath)
{
  xpath.Empty();

  xpath.AdjustSize(_MAXPATH);

  if(!GetCurrentDirectory(xpath.GetSize(), xpath.Get())) 
    {
      return false;
    }

  xpath.AdjustSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::FirstSearch(XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  xpath : Path to use.
* @param[in]  patternsearch : Patternsearch pointer to use.
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::FirstSearch(XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
{
  if(!xpath)         
    {
      return false;
    }

  if(!patternsearch) 
    {
      return false;
    }

  XBYTE* findfiledata = GEN_NEW XBYTE[sizeof(WIN32_FIND_DATA)];
  if(!findfiledata) 
    {
      return false;
    }

  memset(findfiledata, 0, sizeof(WIN32_FIND_DATA));

  searchelement->SetFindFileData(findfiledata);

  searchelement->GetPathSearch()->Set(xpath);

  XPATH pathsearch;

  pathsearch  = xpath;
  pathsearch.Slash_Add();
  pathsearch += patternsearch;

  WIN32_FIND_DATA* search = (WIN32_FIND_DATA*)searchelement->GetFindFileData();

  searchelement->SetHandle((void*)FindFirstFile(pathsearch.Get() , search));
  if((HANDLE)(searchelement->GetHandle()) == INVALID_HANDLE_VALUE)
    {
      searchelement->SetHandle((void*)-1);
      searchelement->DeleteFindFileData();

      return false;
    }
  
  XSTRING searchfilename(search->cFileName);
  
  if((!searchfilename.Compare(_L(".")))||
     (!searchfilename.Compare(_L(".."))))
    {
      return NextSearch(searchelement);
    }

  ReconvertSearchData(searchelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::NextSearch(XDIRELEMENT* searchelement)
* @brief      Next search
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::NextSearch(XDIRELEMENT* searchelement)
{
  WIN32_FIND_DATA* search = (WIN32_FIND_DATA*)searchelement->GetFindFileData();
  HANDLE           handle = (HANDLE)searchelement->GetHandle();

  if(!FindNextFile(handle, search))
    {
      FindClose(handle);

      searchelement->SetHandle((void*)-1);
      searchelement->DeleteFindFileData();

      return false;
    }

   XSTRING searchfilename(search->cFileName);
   
   if((!searchfilename.Compare(_L(".")))||
      (!searchfilename.Compare(_L(".."))))
     {
       return NextSearch(searchelement);
     }

  ReconvertSearchData(searchelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::ReconvertSearchData(XDIRELEMENT* searchelement)
* @brief      Reconvert search data
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::ReconvertSearchData(XDIRELEMENT* searchelement)
{
  if(!searchelement) 
    {
      return false;
    }

  WIN32_FIND_DATA* search =(WIN32_FIND_DATA*)searchelement->GetFindFileData();

  searchelement->GetNameFile()->Set(search->cFileName);
  
  if(search->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
          searchelement->SetType(XDIRELEMENTTYPE_DIR);
    else  searchelement->SetType(XDIRELEMENTTYPE_FILE);

  SetFileDate(searchelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDIR::SetFileDate(XDIRELEMENT* searchelement)
* @brief      Set file date
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDIR::SetFileDate(XDIRELEMENT* searchelement)
{
  WIN32_FIND_DATA*  search        = (WIN32_FIND_DATA*)searchelement->GetFindFileData();
  XDATETIME*        datetimefile  = NULL;
  SYSTEMTIME        st;

  for(int c=0; c<3; c++)
    {      
      switch(c)
        {
          case  0 : datetimefile  = searchelement->GetDateTimeFile_Created();
                    FileTimeToSystemTime(&search->ftCreationTime, &st);  
                    break;

          case  1 : datetimefile  = searchelement->GetDateTimeFile_Modificated();
                    FileTimeToSystemTime(&search->ftLastWriteTime, &st);  
                    break;

          case  2 : datetimefile  = searchelement->GetDateTimeFile_LastAccess();
                    FileTimeToSystemTime(&search->ftLastAccessTime, &st);  
                    break;
        }

      if(datetimefile)
        {
          datetimefile->SetYear(st.wYear);
          datetimefile->SetMonth(st.wMonth);
          datetimefile->SetDay(st.wDay);
          datetimefile->SetHours(st.wHour);
          datetimefile->SetMinutes(st.wMinute);
          datetimefile->SetSeconds(st.wSecond);
          datetimefile->SetMilliSeconds(st.wMilliseconds);
        }
    }
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSDIR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDIR::Clean()
{

}




