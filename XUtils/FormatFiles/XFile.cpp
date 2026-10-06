/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFile.cpp
* 
* @class      XFILE
* @brief      eXtended Utils File class
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

#include "XFile.h"

#include <string.h>

#include "XFactory.h"
#include "XTrace.h"

#include "Cipher.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
XFILEOPENLIST XFileOpenList;
#endif



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILE::XFILE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILE::XFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILE::~XFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILE::~XFILE()

{
  if(isopen) this->Close();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XFILE::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XFILE::GetSize()
{
  return cachesize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Write(XBUFFER &xbuffer)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Write(XBUFFER &xbuffer)
{
  return Write(xbuffer.Get(), xbuffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Close()
* @brief      Close
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Close()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Exist(XPATH& xpath)
* @brief      Exist
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Exist(XPATH& xpath)
{
  return Exist(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Open(XPATH& xpath, bool readonly)
* @brief      Open
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  readonly : Readonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Open(XPATH& xpath, bool readonly)
{
  return Open(xpath.Get(), readonly);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Create(XPATH& xpath)
* @brief      Create resource
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Create(XPATH& xpath)
{
  return Create(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Erase(XPATH& xpath, bool overwrite)
* @brief      Erase
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  overwrite : Overwrite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Erase(XPATH& xpath, bool overwrite)
{
  return Erase(xpath.Get(), overwrite);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Rename(XPATH& xpathold, XPATH& xpathnew)
* @brief      Rename
* @ingroup    XUTILS
* 
* @param[in]  xpathold : Xpathold value.
* @param[in]  xpathnew : Xpathnew value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Rename(XPATH& xpathold, XPATH& xpathnew)
{
  return Rename(xpathold.Get(), xpathnew.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Rename(XCHAR* xpathold, XPATH& xpathnew)
* @brief      Rename
* @ingroup    XUTILS
* 
* @param[in]  xpathold : Xpathold pointer to use.
* @param[in]  xpathnew : Xpathnew value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Rename(XCHAR* xpathold, XPATH& xpathnew)
{
  return Rename(xpathold, xpathnew.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::Rename(XPATH& xpathold, XCHAR* xpathnew)
* @brief      Rename
* @ingroup    XUTILS
* 
* @param[in]  xpathold : Xpathold value.
* @param[in]  xpathnew : Xpathnew pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::Rename(XPATH& xpathold, XCHAR* xpathnew)
{
  return Rename(xpathold.Get(), xpathnew);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::IsOpen()
* @brief      Is open
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::IsOpen()
{
  return isopen;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::IsReadOnly()
* @brief      Is read only
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::IsReadOnly()
{
  return isreadonly;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XFILE::GetPathNameFile()
* @brief      Get path name file
* @ingroup    XUTILS
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XFILE::GetPathNameFile()
{
  return xpathnamefile.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::EraseBlock(XQWORD size)
* @brief      Erase block
* @ingroup    XUTILS
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::EraseBlock(XQWORD size)
{
  XBYTE* tbuffer;
  XQWORD position;
  XQWORD sizetomove;
  XDWORD nblocks;
  XDWORD rest;
  bool   status = true;

  if(!isopen) return false;

  if(!GetPosition(position)) return false;

  sizetomove = GetSize() - position - size;

  tbuffer= GEN_NEW XBYTE [XFILE_MAXBUFFER];
  if(!tbuffer) return false;

  nblocks = (XDWORD)(sizetomove / XFILE_MAXBUFFER);
  rest    = (XDWORD)(sizetomove % XFILE_MAXBUFFER);

  for(XDWORD c=0; c<nblocks; c++)
    {
      SetPosition(position + size + ((XQWORD)c * XFILE_MAXBUFFER));
      if(Read(tbuffer, XFILE_MAXBUFFER))
        {
          SetPosition(position + ((XQWORD)c * XFILE_MAXBUFFER));
          Write(tbuffer, XFILE_MAXBUFFER);
        }
    }

  SetPosition(position + size +((XQWORD)nblocks * XFILE_MAXBUFFER));
  if(Read(tbuffer, rest))
    {
      SetPosition(position + ((XQWORD)nblocks * XFILE_MAXBUFFER));
      Write(tbuffer, rest);
    }

  SetSize(GetSize() - size);

  GEN_DELETE_ARRAY tbuffer;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::InsertBlock(XBYTE* buffer, XDWORD size)
* @brief      Insert block
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::InsertBlock(XBYTE* buffer, XDWORD size)
{
  XBYTE*  movebuffer;
  XQWORD  origin;
  XQWORD  position;

  bool    status = true;

  if(!isopen) return false;

  if(GetPosition(position)!=true) return false;

  origin = position;

  movebuffer= GEN_NEW XBYTE [XFILE_MAXBUFFER];
  if(!movebuffer) return false;

  ActualizeSize();

  XQWORD sizefile      = GetSize();
  XQWORD sizetomove    = sizefile-position;
  XDWORD nblockstomove = (XDWORD)(sizetomove / XFILE_MAXBUFFER);
  XDWORD resttomove    = (XDWORD)(sizetomove % XFILE_MAXBUFFER);
  XDWORD c;

  position = GetSize() - XFILE_MAXBUFFER;

  for(c=0; c<nblockstomove+1; c++)
    {
      XDWORD rb;

      if(c>=nblockstomove)
        {
          rb = resttomove;
          position =  origin;
        }
       else
        {
          rb = XFILE_MAXBUFFER;
        }

      SetPosition(position);
      Read(movebuffer, &rb);

      SetPosition(position+size);
      Write(movebuffer, rb);

      position -= XFILE_MAXBUFFER;
    }

  SetPosition(origin);
  Write(buffer, size);

  if(status==true) ActualizeSize();

  GEN_DELETE_ARRAY movebuffer;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILE::ChangeAttributes(XCHAR* attributes)
* @brief      Change attributes
* @ingroup    XUTILS
* 
* @param[in]  attributes : Attributes pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILE::ChangeAttributes(XCHAR* attributes)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILE::OverwriteContent(XCHAR* xpath)
* @brief      Overwrite content
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILE::OverwriteContent(XCHAR* xpath)
{
  bool status=true;

  if(!Open(xpath, false)) return false;

  GetSize();

  XBYTE*      buffer;
  const int   sizeblock = 1024;
  int         block     = (XDWORD)(cachesize/sizeblock);
  int         mod       = (XDWORD)(cachesize%sizeblock);

  buffer = GEN_NEW XBYTE [sizeblock];
  if(!buffer) return false;

  memset(buffer, 0, sizeblock);

  for(int c=0;c<block;c++)
    {
      if(Write(buffer, sizeblock)!=true)
        {
          status=false;
          break;
        }
    }

  if(status==true)
    {
      if(Write(buffer, mod)!=true)  status=false;
    }

  GEN_DELETE_ARRAY buffer;

  Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILE::Clean()
{
  isopen        = false;
  isreadonly    = false;
  cachesize     = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECONTAINER::XFILECONTAINER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECONTAINER::XFILECONTAINER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECONTAINER::~XFILECONTAINER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECONTAINER::~XFILECONTAINER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECONTAINER::CreatePrimaryFile()
* @brief      Create primary file
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECONTAINER::CreatePrimaryFile()
{
  file=GEN_XFACTORY.Create_File();
  if(!file) return false;

  eraseprimaryfile = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILE* XFILECONTAINER::GetPrimaryFile()
* @brief      Get primary file
* @ingroup    XUTILS
* 
* @return     XFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILE* XFILECONTAINER::GetPrimaryFile()
{
  return file;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECONTAINER::DeletePrimaryFile()
* @brief      Delete primary file
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECONTAINER::DeletePrimaryFile()
{
  if(!file)  return false;

  if(!eraseprimaryfile) return false;

  GEN_XFACTORY.Delete_File(file);

  file = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECONTAINER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECONTAINER::Clean()
{
  file              = NULL;
  eraseprimaryfile  = false;
}






#ifdef XTRACE_ACTIVE


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEOPENLIST::~XFILEOPENLIST()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEOPENLIST::~XFILEOPENLIST()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEOPENLIST::Add(XFILE* xfile)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  xfile : Xfile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEOPENLIST::Add(XFILE* xfile)
{
  filelist.Add(xfile);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEOPENLIST::Del(XFILE* xfile)
* @brief      Delete resource
* @ingroup    XUTILS
* 
* @param[in]  xfile : Xfile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEOPENLIST::Del(XFILE* xfile)
{
  if(filelist.IsEmpty()) return false;

  filelist.Delete(xfile);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEOPENLIST::CloseAll()
* @brief      Close all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEOPENLIST::CloseAll()
{
  if(filelist.IsEmpty()) return false;

  for(XDWORD c=0;c<filelist.GetSize();c++)
    {
      XFILE* xfile = (XFILE*)filelist.Get(c);

      if(xfile) xfile->Close();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEOPENLIST::DisplayAll()
* @brief      Display all
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEOPENLIST::DisplayAll()
{
  XDWORD nfilesopen = filelist.GetSize();

  if(nfilesopen)
    {
      XTRACE_PRINTHEADER((XTRACE_COLOR_RED|XTRACE_LEVEL_WITHCOLOR), _L("NOT ALL FILES ARE CLOSED"));
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" "), NULL);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("Number files not closed: %d"), nfilesopen);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("Path + File Name"), NULL);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" "), NULL);

      for(XDWORD c=0; c<nfilesopen; c++)
        {
          XFILE* xfile = (XFILE*)filelist.Get(c);
          if(xfile) 
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("%s"), xfile->GetPathNameFile());
            }
        }

      XTRACE_PRINTHEADER((XTRACE_COLOR_RED|XTRACE_LEVEL_WITHCOLOR), NULL);
    }

  CloseAll();

  filelist.DeleteContents();
  filelist.DeleteAll();

  return true;
}






#endif

