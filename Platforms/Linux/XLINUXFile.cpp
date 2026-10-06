/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXFile.cpp
* 
* @class      XLINUXFILE
* @brief      LINUX eXtended Utils File class
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

#include "XLINUXFile.h"

#include <unistd.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "XPath.h"
#include "XString.h"

#include "XTrace.h"

#include "Cipher.h"

#include "XLINUXFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXFILE::XLINUXFILE(): XFILE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXFILE::XLINUXFILE(): XFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXFILE::~XLINUXFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXFILE::~XLINUXFILE()
{
  if(isopen) this->Close();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Exist(XCHAR* xpath)
* @brief      Exist
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpath : path to file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Exist(XCHAR* xpath)
{
  bool status = Open(xpath, true);
  if(status) Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Open(XCHAR* xpath, bool isreadonly)
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpath : path to file
* @param[in]  isreadonly : is open with read only
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Open(XCHAR* xpath, bool isreadonly)
{
  this->isreadonly = isreadonly;

  return ExtendedOpen(xpath, (isreadonly)? _L("rb") : _L("r+b"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Create(XCHAR* xpath)
* @brief      Create resource
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpath : path to file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Create(XCHAR* xpath)
{
  return ExtendedOpen(xpath, _L("w+b"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::SetSize(XQWORD size)
* @brief      Set size
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  size : GEN_NEW size
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::SetSize(XQWORD size)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  if(ftruncate(fileno(filehandle) , size)) return false;

  ActualizeSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::GetPosition(XQWORD& position)
* @brief      Get position
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  position : position of the file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::GetPosition(XQWORD& position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  position = ftell(filehandle);
  if(position == -1) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::SetPosition(XQWORD position)
* @brief      Set position
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  position : GEN_NEW position of file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::SetPosition(XQWORD position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  XDWORD _position = position;
  bool   status;

  if(_position == XFILE_SEEKEND) _position = cachesize;
  if(_position > cachesize) return false;

  status = fseek(filehandle, _position, SEEK_SET)?false:true;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  buffer : buffer where to read
* @param[in]  size : size to read
* @param[in]  cipher : cipher class uncipher buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  int _size;

  _size=(int)fread((void *)buffer, 1, (size_t)size, filehandle);
  if(_size!=size)  return false;

  if(cipher) cipher->Uncipher(buffer, _size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  buffer : buffer where to read
* @param[in]  size : size to read
* @param[in]  cipher : cipher class uncipher buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  int  _size;
  bool status = true;

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
* @fn         bool XLINUXFILE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Write
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  buffer : buffer where to read
* @param[in]  size : size to read
* @param[in]  cipher : cipher class to cipher buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
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
* @fn         bool XLINUXFILE::Flush()
* @brief      Flush
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Flush()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  fflush(filehandle);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Close()
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
      isopen     = false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Erase(XCHAR* xpath, bool overwrite)
* @brief      Erase
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpath : path of file
* @param[in]  overwrite : overwrite file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Erase(XCHAR* xpath, bool overwrite)
{
  if(Exist(xpath)!=true)  return false;

  bool status = true;

  if(overwrite) status = OverwriteContent(xpath);

  XBUFFER _xpathbuffer;
  XPATH   _xpath;

  _xpath = xpath;

  _xpath.ConvertToUTF8(_xpathbuffer);

  if(unlink((char*)_xpathbuffer.Get())==-1) status=false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::Rename(XCHAR* xpathold, XCHAR* xpathnew)
* @brief      Rename
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpathold : xpath origin
* @param[in]  xpathnew : GEN_NEW xpath
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::Rename(XCHAR* xpathold, XCHAR* xpathnew)
{
  XBUFFER _xpathbufferold;
  XBUFFER _xpathbuffernew;
  XPATH   _xpathold;
  XPATH   _xpathnew;

  _xpathold = xpathold;
  _xpathnew = xpathnew;

  _xpathold.ConvertToUTF8(_xpathbufferold);
  _xpathnew.ConvertToUTF8(_xpathbuffernew);

  // need to manually reset errno as no call is going to do it
  errno = 0;
  int status  = rename((char*)_xpathbufferold.Get(), (char*)_xpathbuffernew.Get());
  if (status != 0)
    {
      XSTRING err;
      err.Set(strerror(errno));
     XTRACE_PRINTCOLOR(4, _L("Error renaming file %s -> %s : %s"), xpathold, xpathnew, err.Get());
    }

  return (!status)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         FILE* XLINUXFILE::CreateStructHandle()
* @brief      Create struct handle
* @ingroup    PLATFORM_LINUX
* 
* @return     FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
FILE* XLINUXFILE::CreateStructHandle()
{ 
  return filehandle;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::DeleteStructHandle()
* @brief      Delete struct handle
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::DeleteStructHandle()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXFILE::ChangeAttributes(XCHAR* attributes)
* @brief      Change attributes
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  attributes : GEN_NEW atributes
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXFILE::ChangeAttributes(XCHAR* attributes)
{
  XSTRING mode;
  
  mode.Set(attributes);
  
  XBUFFER charstr0;
  
  mode.ConvertToASCII(charstr0);
  
  int i = strtol(charstr0.GetPtrChar(), 0, 8);
  
  XSTRING str(xpathnamefile.Get());
  
  XBUFFER charstr1;
  
  str.ConvertToASCII(charstr1);  
  chmod(charstr1.GetPtrChar(), i);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::ActualizeSize()
* @brief      Actualize size
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::ActualizeSize()
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
* @fn         void XLINUXFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXFILE::Clean()
{
  filehandle  = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXFILE::ExtendedOpen(XCHAR* xpath, XCHAR* mode)
* @brief      Extended open
* @note       INTERN
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  xpath : xpath of file
* @param[in]  mode : mode attributes to open
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXFILE::ExtendedOpen(XCHAR* xpath, XCHAR* mode)
{
  if(isopen) Close();

  this->xpathnamefile = xpath;

  XPATH   _xpath;
  XSTRING _mode;
  XBUFFER _xpathbuffer;
  XBUFFER _modebuffer;
  bool    status = false;

  _xpath  = xpath;
  _mode   = mode;

  _xpath.ConvertToUTF8(_xpathbuffer); _xpathbuffer.Add((XDWORD)0);
  _mode.ConvertToUTF8(_modebuffer);   _modebuffer.Add((XDWORD)0);

  filehandle = fopen((char*)_xpathbuffer.Get(), (char*)_modebuffer.Get());
  if(filehandle)
    {
      isopen = true;

      ActualizeSize();

      status = true;

      #ifdef XTRACE_ACTIVE
      XFileOpenList.Add(this);
      #endif
    }

  return status;
}



