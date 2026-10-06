/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Dir_NotUnicode.cpp
*
* @class      XSTM32DIR_NOTUNICODE
* @brief      STM32 eXtended Not Unicode Directory class
* @ingroup    PLATFORM_STM32
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XFactory.h"
#include "XFile.h"
#include "XString.h"
#include "XPath.h"
#include "XTrace.h"

#include "XSTM32Dir_NotUnicode.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32DIR_NOTUNICODE::XSTM32DIR_NOTUNICODE(): XDIR()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32DIR_NOTUNICODE::XSTM32DIR_NOTUNICODE(): XDIR()
{
  Clean();
  
  fresult = f_mount(&userFATFS, "/", 1);
  if(fresult == FR_OK) ismount = true; 
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32DIR_NOTUNICODE::~XSTM32DIR_NOTUNICODE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32DIR_NOTUNICODE::~XSTM32DIR_NOTUNICODE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::Exist(XCHAR* path)
{   
  XSTRING pathstring;
  DIR     dir;
 
  pathstring = path;
  if(pathstring.IsEmpty()) return false; 
   
  XBUFFER charstr;
  
  pathstring.ConvertToASCII(charstr);  
  fresult = f_opendir(&dir, charstr.GetPtrChar());						
  if(fresult) fresult = f_closedir(&dir);	
  
  return (fresult == FR_OK)?true:false;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::Make(XCHAR* path, bool recursive)
* @brief      Make
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::Make(XCHAR* path, bool recursive)
{
  if(!path)   return false;
  if(!path[0]) return false;

  if(Exist(path)) return true;

  XPATH   xpath;

  xpath = path;

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

              if(!Exist(xpathsequence.Get()))
                {
                  //XTRACE_PRINTCOLOR(2, _L("---> Make: %s"), xpathsequence.Get());
                      
                  xpathsequence.Slash_Delete();
                      
                  XBUFFER charstr;
                  
                  xpathsequence.ConvertToASCII(charstr);                  
                  fresult = f_mkdir(charpath.GetPtrChar());						
                  
                  if(fresult != FR_OK) return false;  
                }
                
            } else break;

          index++;

        } while(1);
    }
   else
    {      
      XBUFFER charstr;
      
      xpath.ConvertToASCII(charstr);
      fresult = f_mkdir(charstr.GetPtrChar());						      
      if(fresult != FR_OK) return false;  
    }

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::ChangeTo(XCHAR* path)
* @brief      Change to
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::ChangeTo(XCHAR* path)
{  
  if(!path)       return false;
  if(!path[0])    return false;

  if(Exist(path)) return true;

  XPATH xpath;

  xpath = path;

  xpath.Slash_Normalize(true);
  
  XBUFFER charstr;
  
  xpath.ConvertToASCII(charstr);
  fresult = f_chdir(charstr.GetPtrChar());	  
  if(fresult != FR_OK) return false;  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::Delete(XCHAR* path, bool all)
* @brief      Delete resource
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  all : All value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::Delete(XCHAR* path, bool all)
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
          do{ xpathname  = path;
              xpathname.Slash_Add();
              xpathname += search.GetNameFile()->Get();

              if(search.GetType() == XDIRELEMENTTYPE_DIR)
                {
                  Delete(xpathname.Get(), all);
                }
               else
                {
                  XFILE* xfile=GEN_XFACTORY.Create_File();
                  if(xfile) xfile->Erase(xpathname.Get());
                  GEN_XFACTORY.Delete_File(xfile);
                 }
                
            } while(NextSearch(&search));
        }
    }

  
  if(!path)       return false;
  if(!path[0])    return false; 

  XPATH xpath;

  xpath = path;

  xpath.Slash_Normalize(true);
  
  XBUFFER charstr;
  
  xpath.ConvertToASCII(charstr);
  fresult = f_rmdir(charstr.GetPtrChar());	
  if(fresult != FR_OK) return false;  
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::GetActual(XPATH& xpath)
* @brief      Get actual
* @ingroup    PLATFORM_STM32
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::GetActual(XPATH& xpath)
{ 
  char path[_MAXPATH];
  
  memset(path, 0, _MAXPATH);
  
  fresult = f_getcwd(path, _MAXPATH);							
  if(fresult != FR_OK) return false;  
  
  xpath = path;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::FirstSearch(XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    PLATFORM_STM32
* 
* @param[in]  xpath : Path to use.
* @param[in]  patternsearch : Patternsearch pointer to use.
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::FirstSearch(XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
{  
  if(!xpath)         return false;
  if(!patternsearch) return false;

  XBYTE* findfiledata = GEN_NEW XBYTE[sizeof(XSTM32DIR_NOTUNICODE_SEARCHINFO)];
  if(!findfiledata) return false;

  memset(findfiledata, 0, sizeof(XSTM32DIR_NOTUNICODE_SEARCHINFO));

  searchelement->SetFindFileData(findfiledata);

  searchelement->GetPathSearch()->Set(xpath);

  XPATH   pathsearch;
  XSTRING _patternsearch;
  
  _patternsearch = patternsearch;
  
  searchelement->GetPatternSearch()->Set(_patternsearch);

  pathsearch  = xpath;
  pathsearch.Slash_Add();
  
  searchelement->GetPathSearch()->Set(pathsearch);
    
  XSTM32DIR_NOTUNICODE_SEARCHINFO* searchinfo = (XSTM32DIR_NOTUNICODE_SEARCHINFO*)searchelement->GetFindFileData();

  XBUFFER charpathsearch;
  XBUFFER charpatternsearch;
  
  pathsearch.ConvertToASCII(charpathsearch);
  _patternsearch.ConvertToASCII(charpatternsearch);

  fresult = f_findfirst(&searchinfo->dir, &searchinfo->fileinfo, (const TCHAR*)charpathsearch.GetPtrChar(), (const TCHAR*)charpatternsearch.GetPtrChar());  
  if(fresult != FR_OK || !searchinfo->fileinfo.fname[0]) 
    {      
      searchelement->DeleteFindFileData();
      return false;
    }

  XSTRING searchfilename(searchinfo->fileinfo.fname);

  if((!searchfilename.Compare(_L(".")))||
     (!searchfilename.Compare(_L(".."))))
    {
      return NextSearch(searchelement);
    }

   ReconvertSearchData(searchelement, &searchinfo->fileinfo);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::NextSearch(XDIRELEMENT* searchelement)
* @brief      Next search
* @ingroup    PLATFORM_STM32
* 
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::NextSearch(XDIRELEMENT* searchelement)
{
  XSTM32DIR_NOTUNICODE_SEARCHINFO* searchinfo = (XSTM32DIR_NOTUNICODE_SEARCHINFO*)searchelement->GetFindFileData();
  if(!searchinfo) return false;
  
  XBUFFER charstr;
  
  (*searchelement->GetPatternSearch()).ConvertToASCII(charstr);  
  searchinfo->dir.pat = charstr.GetPtrChar(); 
  fresult = f_findnext(&searchinfo->dir, &searchinfo->fileinfo);  
  if(fresult != FR_OK || !searchinfo->fileinfo.fname[0]) 
    {
      searchelement->DeleteFindFileData();
      f_closedir(&searchinfo->dir);
      return false;
    }

   XSTRING searchfilename(searchinfo->fileinfo.fname);

   if((!searchfilename.Compare(_L(".")))||
      (!searchfilename.Compare(_L(".."))))
     {
       return NextSearch(searchelement);
     }

  ReconvertSearchData(searchelement, &searchinfo->fileinfo);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DIR_NOTUNICODE::ReconvertSearchData(XDIRELEMENT* searchelement, FILINFO* fileinfo)
* @brief      Reconvert search data
* @ingroup    PLATFORM_STM32
* 
* @param[in]  searchelement : Searchelement pointer to use.
* @param[in]  fileinfo : Fileinfo pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DIR_NOTUNICODE::ReconvertSearchData(XDIRELEMENT* searchelement, FILINFO* fileinfo)
{
  if(!searchelement) return false;

  searchelement->GetNameFile()->Set(fileinfo->fname);

  if(fileinfo->fattrib & AM_DIR)
          searchelement->SetType(XDIRELEMENTTYPE_DIR);
    else  searchelement->SetType(XDIRELEMENTTYPE_FILE);
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32DIR_NOTUNICODE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32DIR_NOTUNICODE::Clean()
{
  memset(&userFATFS, 0, sizeof(FATFS));
  
  fresult = FR_OK; 
}

