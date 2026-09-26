/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDir.cpp
* 
* @class      XDIR
* @brief      eXtended Utils dir class
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

#include "XDir.h"

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XFile.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIRELEMENT::XDIRELEMENT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIRELEMENT::XDIRELEMENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIRELEMENT::~XDIRELEMENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIRELEMENT::~XDIRELEMENT()
{
  DeleteFindFileData();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIRELEMENTTYPE XDIRELEMENT::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XDIRELEMENTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIRELEMENTTYPE XDIRELEMENT::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDIRELEMENT::SetType(XDIRELEMENTTYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDIRELEMENT::SetType(XDIRELEMENTTYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XDIRELEMENT::GetPathSearch()
* @brief      Get path search
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XDIRELEMENT::GetPathSearch()
{
  return &pathsearch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XDIRELEMENT::GetPatternSearch()
* @brief      Get pattern search
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XDIRELEMENT::GetPatternSearch()
{
  return &patternsearch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XDIRELEMENT::GetNameFile()
* @brief      Get name file
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XDIRELEMENT::GetNameFile()
{
  return &namefile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XDIRELEMENT::GetDateTimeFile_Created()
* @brief      Get date time file created
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XDIRELEMENT::GetDateTimeFile_Created()
{
  return &datetime_created;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XDIRELEMENT::GetDateTimeFile_Modificated()
* @brief      Get date time file modificated
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XDIRELEMENT::GetDateTimeFile_Modificated()
{
  return &datetime_modificated;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XDIRELEMENT::GetDateTimeFile_LastAccess()
* @brief      Get date time file last access
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XDIRELEMENT::GetDateTimeFile_LastAccess()
{
  return &datetime_lastaccess;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XDIRELEMENT::GetHandle()
* @brief      Get handle
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XDIRELEMENT::GetHandle()
{
  return handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDIRELEMENT::SetHandle(void* handle)
* @brief      Set handle
* @ingroup    XUTILS
* 
* @param[in]  handle : Handle pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDIRELEMENT::SetHandle(void* handle)
{
  this->handle = handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XDIRELEMENT::GetFindFileData()
* @brief      Get find file data
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XDIRELEMENT::GetFindFileData()
{
  return findfiledata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDIRELEMENT::SetFindFileData(void* findfiledata)
* @brief      Set find file data
* @ingroup    XUTILS
* 
* @param[in]  findfiledata : Findfiledata pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDIRELEMENT::SetFindFileData(void* findfiledata)
{
  this->findfiledata = findfiledata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIRELEMENT::DeleteFindFileData()
* @brief      Delete find file data
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIRELEMENT::DeleteFindFileData()
{
  if(!findfiledata) return false;

  GEN_DELETE_ARRAY (XBYTE*)findfiledata;

  findfiledata = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDIRELEMENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDIRELEMENT::Clean()
{
  type = XDIRELEMENTTYPE_NONE;

  handle       = (void*)-1;
  findfiledata = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIR::XDIR()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIR::XDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIR::~XDIR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIR::~XDIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Exist(XPATH& xpath)
* @brief      Exist
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Exist(XPATH& xpath)
{
  return Exist(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Make(XPATH& xpath, bool recursive)
* @brief      Make
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Make(XPATH& xpath, bool recursive)
{
  return Make(xpath.Get(), recursive);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::ChangeTo(XPATH& xpath)
* @brief      Change to
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::ChangeTo(XPATH& xpath)
{
  return ChangeTo(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Delete(XPATH& xpath, bool all)
* @brief      Delete resource
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  all : All value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Delete(XPATH& xpath, bool all)
{
  return Delete(xpath.Get(), all);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::FirstSearch(XPATH& xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  patternsearch : Patternsearch value.
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::FirstSearch(XPATH& xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath.Get(), patternsearch.Get(), searchelement);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::FirstSearch(XPATH& xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  patternsearch : Patternsearch pointer to use.
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::FirstSearch(XPATH& xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath.Get(), patternsearch, searchelement);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::FirstSearch(XCHAR* xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement)
* @brief      First search
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  patternsearch : Patternsearch value.
* @param[in]  searchelement : Searchelement pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::FirstSearch(XCHAR* xpath, XSTRING& patternsearch, XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath, patternsearch.Get(), searchelement);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Copy(XPATH& xpathsource, XPATH& xpathtarget)
* @brief      Copy
* @ingroup    XUTILS
* 
* @param[in]  xpathsource : Xpathsource value.
* @param[in]  xpathtarget : Xpathtarget value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Copy(XPATH& xpathsource, XPATH& xpathtarget)
{
  #define COPYFILE_MAXBUFFER 2048

  bool status = true;

  XFILE* fsource=GEN_XFACTORY.Create_File();
  XFILE* ftarget=GEN_XFACTORY.Create_File();

  if(fsource && ftarget)
    {
      if(fsource->Open(xpathsource))
        {
          if(ftarget->Create(xpathtarget))
            {
              XBYTE* buffer = GEN_NEW XBYTE[COPYFILE_MAXBUFFER];
              if(buffer)
                {
                  XDWORD rsize  = 0;

                  do { rsize = COPYFILE_MAXBUFFER;

                       memset(buffer, 0, rsize);

                       fsource->Read(buffer, &rsize);

                       if(rsize) ftarget->Write(buffer, rsize);

                     } while(rsize == COPYFILE_MAXBUFFER);

                  GEN_DELETE_ARRAY buffer;
                }

              ftarget->Close();

            } else status = false;

          fsource->Close();

        } else status = false;

    } else status = false;

  GEN_XFACTORY.Delete_File(fsource);
  GEN_XFACTORY.Delete_File(ftarget);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Copy(XCHAR* xpathsource, XPATH& xpathtarget)
* @brief      Copy
* @ingroup    XUTILS
* 
* @param[in]  xpathsource : Xpathsource pointer to use.
* @param[in]  xpathtarget : Xpathtarget value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Copy(XCHAR* xpathsource, XPATH& xpathtarget)
{
  XPATH _xpathsource;

  _xpathsource = xpathsource;

  return Copy(_xpathsource, xpathtarget);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Copy(XPATH& xpathsource, XCHAR* xpathtarget)
* @brief      Copy
* @ingroup    XUTILS
* 
* @param[in]  xpathsource : Xpathsource value.
* @param[in]  xpathtarget : Xpathtarget pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Copy(XPATH& xpathsource, XCHAR* xpathtarget)
{
  XPATH _xpathtarget;

  _xpathtarget = xpathtarget;

  return Copy(xpathsource, _xpathtarget);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDIR::Copy(XCHAR* xpathsource, XCHAR* xpathtarget)
* @brief      Copy
* @ingroup    XUTILS
* 
* @param[in]  xpathsource : Xpathsource pointer to use.
* @param[in]  xpathtarget : Xpathtarget pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDIR::Copy(XCHAR* xpathsource, XCHAR* xpathtarget)
{
  XPATH _xpathsource;
  XPATH _xpathtarget;

  _xpathsource = xpathsource;
  _xpathtarget = xpathtarget;

  return Copy(_xpathsource, _xpathtarget);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDIR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDIR::Clean()
{

}





