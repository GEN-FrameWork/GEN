/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSFileBorland.cpp
* 
* @class      XWINDOWSFILEBORLAND
* @brief      WINDOWS eXtended Utils Borland file class
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

#include "XWINDOWSFileBorland.h"

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <share.h>

#include "Cipher.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSFILEBORLAND::XWINDOWSFILEBORLAND(): XFILE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSFILEBORLAND::XWINDOWSFILEBORLAND(): XFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSFILEBORLAND::~XWINDOWSFILEBORLAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSFILEBORLAND::~XWINDOWSFILEBORLAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Exist(XCHAR* path)
{
  bool status = Open(path, true);
  if(status) Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Open(XCHAR* path, bool isreadonly)
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* @param[in]  isreadonly : Isreadonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Open(XCHAR* path, bool isreadonly)
{
  return ExtendedOpen(path, (isreadonly)? __L("rb") : __L("r+b"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Create(XCHAR* path)
* @brief      Create resource
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Create(XCHAR* path)
{
  return ExtendedOpen(path, __L("w+b"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::SetSize(XQWORD size)
* @brief      Set size
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::SetSize(XQWORD size)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  #ifdef _MSC_VER
  if(_chsize(_fileno(filehandle), size)) return false;
  #else
  if(chsize(_fileno(filehandle), size)) return false;
  #endif

  ActualizeSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::GetPosition(XQWORD& position)
* @brief      Get position
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::GetPosition(XQWORD& position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  position = ftell(filehandle);

  if(position == -1) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::SetPosition(XQWORD position)
* @brief      Set position
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::SetPosition(XQWORD position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  int  _position    = position;
  bool status;

  if(_position == XFILE_SEEKEND) _position = cachesize;

  if(_position > (int)cachesize) return false;

  status=fseek(filehandle, _position, SEEK_SET)?false:true;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  XDWORD _size;

  _size=(int)fread((void *)buffer, 1, (size_t)size, filehandle);
  if(_size!=size)  return false;

  if(cipher) cipher->Uncipher(buffer, _size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  XDWORD  _size;
  bool    status = true;

  _size=(int)fread((void *)buffer, (size_t)1, (size_t)(*size), filehandle);

  if(_size!=(*size))  status=false;

  (*size)=_size;

  if(cipher)
    {
      if(cipher->Uncipher(buffer, _size))
        {
          memcpy(buffer, cipher->GetResult()->Get(), _size);

        } else status = false;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  if(cipher)
    {
      if(!cipher->Cipher(buffer, size)) return false;
      if(fwrite((void *)cipher->GetResult()->Get(), 1, size, filehandle)!=(unsigned)size)  return false;
    }
   else
    {
      if(fwrite((void *)buffer, 1, size, filehandle)!=(unsigned)size)  return false;
    }

  ActualizeSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Flush()
* @brief      Flush
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Flush()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  fflush(filehandle);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Close()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  Flush();

  if(filehandle)
    {
      fclose(filehandle);

      #ifdef XTRACE_ACTIVE
      XFileOpenList.Del(this);
      #endif

      filehandle = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Erase(XCHAR* path, bool overwrite)
* @brief      Erase
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* @param[in]  overwrite : Overwrite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Erase(XCHAR* path, bool overwrite)
{
  bool status=true;

  if(Exist(path)!=true)  return false;

  if(overwrite) status = OverwriteContent(path);

  if(status) status = DeleteFile(path)?true:false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::Rename(XCHAR* xpathold, XCHAR* xpathnew)
* @brief      Rename
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  xpathold : Xpathold pointer to use.
* @param[in]  xpathnew : Xpathnew pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::Rename(XCHAR* xpathold, XCHAR* xpathnew)
{
  if(!_wrename(xpathold, xpathnew)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         FILE* XWINDOWSFILEBORLAND::GetFileStructHandle()
* @brief      Get file struct handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
FILE* XWINDOWSFILEBORLAND::GetFileStructHandle()                                            
{ 
  return filehandle;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         FILE* XWINDOWSFILEBORLAND::CreateStructHandle()
* @brief      Create struct handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
FILE* XWINDOWSFILEBORLAND::CreateStructHandle()
{
  return filehandle;    
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::DeleteStructHandle()
* @brief      Delete struct handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::DeleteStructHandle()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::ActualizeSize()
* @brief      Actualize size
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::ActualizeSize()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  Flush();

  int position = ftell(filehandle);

  fseek(filehandle, 0, SEEK_END);
  cachesize = ftell(filehandle);
  fseek(filehandle, position, SEEK_SET);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSFILEBORLAND::ExtendedOpen(XCHAR* path, XCHAR* mode)
* @brief      Extended open
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  path : Path to use.
* @param[in]  mode : Mode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSFILEBORLAND::ExtendedOpen(XCHAR* path, XCHAR* mode)
{
  if(isopen) Close();

  this->xpathnamefile = path;

  int error = 0;

  #ifdef BUILDER
  filehandle = _wfopen(path, mode);
  if(!filehandle) return false;
  #else
  error = _wfopen_s(&filehandle, path, mode);
  #endif

  if(error)        return false;
  if(!filehandle)  return false;

  isopen = true;

  ActualizeSize();

  #ifdef XTRACE_ACTIVE
  XFileOpenList.Add(this);
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSFILEBORLAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSFILEBORLAND::Clean()
{
  filehandle  = NULL;
}



