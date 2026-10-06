/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXDir.cpp
* 
* @class      XLINUXDIR
* @brief      LINUX eXtended Utils Directory class
* @ingroup    PLATFORM_LINUX
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

#include "XLINUXDir.h"

#include <stdio.h>
#include <time.h>       
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>

#include "XPath.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "XLINUXFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXDIR::XLINUXDIR(): XDIR()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXDIR::XLINUXDIR(): XDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXDIR::~XLINUXDIR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXDIR::~XLINUXDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : path to directory
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::Exist(XCHAR* path)
{
  DIR*    dir    = NULL;
  XPATH   xpath;
  bool    status = false;

  xpath = path;
  
  XBUFFER pathchar;
  
  xpath.ConvertToASCII(pathchar);    
  dir  = opendir(pathchar.GetPtrChar());  
  if(dir) 
    {    
      status = true; 

      /*      
      if( errno  == ENOENT)
        {
        
        } 
       else 
        {
          status = true;
        }
      */

      closedir(dir);
    }
   else
    {
      XSTRING s;
      s.Set(strerror(errno));
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::Make(XCHAR* path, bool recursive)
* @brief      Make
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : Path to use.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::Make(XCHAR* path, bool recursive)
{
  if(!path)   return false;
  if(!path[0]) return false;

  if(Exist(path)) return true;

  XPATH   xpath;
  xpath = path;

  if(recursive)
    {
      xpath.Slash_Add();

      XPATH   xpathsequence;
      XSTRING pathpart;
      int     index = 0;

      xpathsequence.Empty();

      do{
          if(xpath.GetPathInSequence(index, pathpart))
            {
              xpathsequence.Slash_Add();
              xpathsequence += pathpart;
              
              //if(!Exist(xpathsequence.Get()))
                {
                  XBUFFER newpath;
                  int     status;

                  xpathsequence.ConvertToUTF8(newpath);

                  status = mkdir((char*)newpath.Get(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

                  if(status)
                    {
                      XSTRING s;
                      s.Set(strerror(errno));
                    }

                  //if(status) return false;                    
                }
        
            } else break;

          index++;

        } while(1);
    }
   else
    {
      XBUFFER newpath;
      int     status;

      xpath.ConvertToUTF8(newpath);

      status = mkdir((char*)newpath.Get(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

      if(status)
        {
          XSTRING s;
          s.Set(strerror(errno));
        }

      if(status) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::ChangeTo(XCHAR* path)
* @brief      Change to
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : path to change
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::ChangeTo(XCHAR* path)
{
  XPATH   xpath;
  XBUFFER newpath;
  int     status = 1;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  status = chdir((char*)newpath.Get());

  return (!status)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::Delete(XCHAR* path, bool all)
* @brief      Delete resource
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : directory to delete
* @param[in]  all : GEN_DELETE all elements
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::Delete(XCHAR* path, bool all)
{
  XDIRELEMENT search;
  XPATH       pathfile;
  XPATH       xpath;
  XBUFFER     newpath;

  if(all)
    {
      if(FirstSearch(path, _L("*"), &search))
        {
          do { pathfile = path;
               pathfile.Slash_Add();
               pathfile += search.GetNameFile()->Get();

               if(search.GetType()==XDIRELEMENTTYPE_DIR)
                 {
                   Delete(pathfile.Get(), all);
                 }
                else
                 {
                   xpath = pathfile;
                   xpath.ConvertToUTF8(newpath);

                   unlink((char*)newpath.Get());
                 }

             } while(NextSearch(&search));
        }
    }

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  int status = rmdir((char*)newpath.Get());

  return (!status)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::GetActual(XPATH& path)
* @brief      Get actual
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::GetActual(XPATH& path)
{
  char buffer[PATH_MAX];

  if(!getcwd(buffer, PATH_MAX)) return false;

  // ConvertFromUTF8() walks exactly the byte count given it, with no NUL-termination check of its own, so passing
  // the full PATH_MAX capacity here (instead of the actual cwd length) fed it whatever uninitialized stack bytes
  // followed the string: XSTRING's cached size ended up covering that garbage tail, so every later Add()/+=
  // (which appends at the cached size, not at the first NUL) landed past an embedded NUL byte and was invisible
  // to any consumer that reads the string as a C string (fopen(), ConvertToUTF8() for logging, etc.).
  path.ConvertFromUTF8((XBYTE*)buffer, (XDWORD)strlen(buffer));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::FirstSearch(XCHAR* path, XCHAR* patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : path to do the search
* @param[in]  patternsearch : pattern to search
* @param[in]  searchelement : search element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::FirstSearch(XCHAR* path, XCHAR* patternsearch, XDIRELEMENT* searchelement)
{
  if(!searchelement) return false;

  XPATH   xpath;
  XBUFFER newpath;
  bool    status = false;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  DIR* dir = opendir((char*)newpath.Get());
  if(dir)
    {
      searchelement->GetPathSearch()->Set(path);
      searchelement->GetPatternSearch()->Set(patternsearch);
      searchelement->SetHandle((void*)dir);

      struct dirent* entry = readdir(dir);
      if(!entry)
        {
          closedir(dir);
          status = false;
        }
       else
        {
          searchelement->SetFindFileData((void*)entry);
          status = NextSearch(searchelement);
        }

    } else status = false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXDIR::NextSearch(XDIRELEMENT* searchelement)
* @brief      Next search
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  searchelement : search element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDIR::NextSearch(XDIRELEMENT* searchelement)
{
  if(!searchelement) return false;

  DIR*           dir    = (DIR*)searchelement->GetHandle();
  struct dirent* entry  = (struct dirent*)searchelement->GetFindFileData();
  bool           status = false;
  XSTRING        name;
  XBUFFER        patternsearch;

  while(entry != NULL)
    {
      name.ConvertFromUTF8((XBYTE*)entry->d_name, strlen(entry->d_name) /*_MAXPATH*/ );

      if((name.Compare(_L("."))) && (name.Compare(_L(".."))))
        {
          searchelement->GetPatternSearch()->ConvertToUTF8(patternsearch);
          if(!fnmatch((char*)patternsearch.Get(), entry->d_name, 0))
            {
              searchelement->SetType(XDIRELEMENTTYPE_NONE);
              searchelement->GetNameFile()->Set(name.Get());

              XPATH xpath;

              xpath = searchelement->GetPathSearch()->Get();
              xpath.Slash_Add();
              xpath += searchelement->GetNameFile()->Get();

              TypeOfEntry(xpath.Get(), searchelement);

              status = true;
            }
        }

      entry = readdir(dir);
      searchelement->SetFindFileData((void*)entry);

      if(status) break;
    }

  if(!status) closedir (dir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIRELEMENTTYPE XLINUXDIR::TypeOfEntry(XCHAR* path, XDIRELEMENT* searchelement)
* @brief      Type of entry
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  path : path of element
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     XDIRELEMENTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIRELEMENTTYPE XLINUXDIR::TypeOfEntry(XCHAR* path, XDIRELEMENT* searchelement)
{
  if(!path)     return XDIRELEMENTTYPE_NONE;

  XPATH           xpath;
  XBUFFER         newpath;
  XDIRELEMENTTYPE type = XDIRELEMENTTYPE_NONE;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  struct stat datafile;

  if(lstat((char*)newpath.Get(), &datafile) != -1)
    {
      struct tm*  timefile;
      XDATETIME*  datetimefile  = NULL;

      for(int c=0; c<3; c++)
        {      
          switch(c)
            {
              case  0 : datetimefile  = searchelement->GetDateTimeFile_Created();
                        timefile      = gmtime(&(datafile.st_ctime));  
                        break;

              case  1 : datetimefile  = searchelement->GetDateTimeFile_Modificated();
                        timefile      = gmtime(&(datafile.st_mtime));  
                        break;

              case  2 : datetimefile  = searchelement->GetDateTimeFile_LastAccess();
                        timefile      = gmtime(&(datafile.st_atime));  
                        break;
            }

          if(datetimefile)
            {
              datetimefile->SetYear(timefile->tm_year + 1900);
              datetimefile->SetMonth(timefile->tm_mon);
              datetimefile->SetDay(timefile->tm_mday);
              datetimefile->SetHours(timefile->tm_hour);
              datetimefile->SetMinutes(timefile->tm_min);
              datetimefile->SetSeconds(timefile->tm_sec);
              datetimefile->SetMilliSeconds(0);
            }
        }


      /*
      struct tm* mod_time = gmtime(&(datafile.st_mtime);  
      struct tm* cre_time = gmtime(&(datafile.st_ctime);  
      struct tm* cre_time = gmtime(&(datafile.st_atime);  

      int    tm_sec   seconds [0,61]
      int    tm_min   minutes [0,59]
      int    tm_hour  hour [0,23]
      int    tm_mday  day of month [1,31]
      int    tm_mon   month of year [0,11]
      int    tm_year  years since 1900
      int    tm_wday  day of week [0,6] (Sunday = 0)
      int    tm_yday  day of year [0,365]
      int    tm_isdst daylight savings flag
      */

      //Link
      if(S_ISLNK(datafile.st_mode))
        {

        }

      // File
      if(S_ISREG(datafile.st_mode))
        {
          type = XDIRELEMENTTYPE_FILE;       
        }
       else
        {
          // Directory
          if(S_ISDIR(datafile.st_mode)) type = XDIRELEMENTTYPE_DIR;
        }
    }

  searchelement->SetType(type);

  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXDIR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXDIR::Clean()
{

}



