/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32File_NotUnicode.cpp
*
* @class      XSTM32FILE_NOTUNICODE
* @brief      STM32 eXtended Not Unicode File class (Use FatFs - Generic FAT file system module)
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


#include "XPath.h"
#include "XString.h"
#include "XTrace.h"

#include "Cipher.h"

#include "XSTM32File_NotUnicode.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32FILE_NOTUNICODE::XSTM32FILE_NOTUNICODE(): XFILE()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32FILE_NOTUNICODE::XSTM32FILE_NOTUNICODE(): XFILE()
{
  Clean();

  fresult = f_mount(&userFATFS, "/", 1);
  if(fresult == FR_OK) ismount = true; 

}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32FILE_NOTUNICODE::~XSTM32FILE_NOTUNICODE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32FILE_NOTUNICODE::~XSTM32FILE_NOTUNICODE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Exist(XCHAR* path)
{  
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 

  XBUFFER charstr;
  
  pathstring.ConvertToASCII(charstr);
  fresult = f_stat (charstr.GetPtrChar(), &fileinfo);			
  cachesize =0;
  if(fresult == FR_OK) cachesize = (XQWORD)fileinfo.fsize;  
  
  status = (fresult == FR_OK)?true:false;  
   
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Open(XCHAR* path, bool isreadonly)
* @brief      Open
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  isreadonly : Isreadonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Open(XCHAR* path, bool isreadonly)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 

  XBYTE mode  = (FA_OPEN_ALWAYS | FA_READ);
    
  if(!isreadonly) mode |= FA_WRITE;
 
  XBUFFER charstr;
  
  pathstring.ConvertToASCII(charstr);
 
  fresult = f_open(&file, charstr.GetPtrChar(), mode);
  cachesize = 0;   
  if(fresult == FR_OK) cachesize = (XQWORD)f_size(&file);
  
  status = (fresult == FR_OK)?true:false;  
  if(status) 
    {
      xpathnamefile = path;
      isopen        = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Create(XCHAR* path)
* @brief      Create resource
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Create(XCHAR* path)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 

  XBYTE mode  = (FA_CREATE_ALWAYS | FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
      
  XBUFFER charstr;
  
  pathstring.ConvertToASCII(charstr); 
  
  fresult = f_open(&file, charstr.GetPtrChar(), mode);  
  cachesize = 0;    
  status = (fresult == FR_OK)?true:false;  
  if(status) 
    {
      xpathnamefile = path;
      isopen        = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::SetSize(XQWORD size)
* @brief      Set size
* @ingroup    PLATFORM_STM32
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::SetSize(XQWORD size)
{
  if(!isopen)     return false;
  if(isreadonly)  return false;
  
  if(!SetPosition(size)) return true;
  
  fresult = f_truncate(&file);
  if(fresult == FR_OK) cachesize = (XQWORD)f_size(&file);
  
  return (fresult == FR_OK)?true:false;  

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::GetPosition(XQWORD& position)
* @brief      Get position
* @ingroup    PLATFORM_STM32
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::GetPosition(XQWORD& position)
{
  position = 0;
  
  if(!isopen) return false;
   
  FSIZE_t _position = f_tell(&file);
  
  position = (XQWORD)_position;   
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::SetPosition(XQWORD position)
* @brief      Set position
* @ingroup    PLATFORM_STM32
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::SetPosition(XQWORD position)
{  
  if(!isopen) return false;
  
  FSIZE_t   _position  = (FSIZE_t)position;
  bool      status     = false;

  fresult = f_lseek (&file, _position);  
  status = (fresult == FR_OK)?true:false; 
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
{
  if(!isopen) return false;

  UINT  sizetoread = (UINT)size;
  UINT  readsize   = 0;
  bool  status     = false;

  fresult = f_read(&file, (void*)buffer, sizetoread, &readsize);	   
  status = (fresult == FR_OK)?true:false;  
  if(status) status = (sizetoread == readsize);
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
* @brief      Read
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
{ 
  if(!isopen) return false;

  UINT  sizetoread = (UINT)size;
  UINT  readsize   = 0;
  bool  status     = false;

  fresult = f_read(&file, (void*)buffer, sizetoread, &readsize);	

  status = (fresult == FR_OK)?true:false;  
  if(status) (*size) = readsize;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
* @brief      Write
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  cipher : Cipher pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
{
  if(!isopen) return false;

  UINT  sizetowrite = (UINT)size;
  UINT  writesize   = 0;
  bool  status      = false;
    
  fresult = f_write(&file, (void*)buffer, sizetowrite, &writesize);	/* Write data to the file */
  
  status = (fresult == FR_OK)?true:false;  
  if(status) 
    {
      cachesize = (XQWORD)f_size(&file);
      status = (sizetowrite == writesize);
    }
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Flush()
* @brief      Flush
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Flush()
{
  if(!isopen) return false;
  
  bool status;
  
  fresult = f_sync(&file);											
  status = (fresult == FR_OK)?true:false;  
  if(status) cachesize = (XQWORD)f_size(&file);
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Close()
* @brief      Close
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Close()
{
  if(!isopen) return false;

  fresult = f_close(&file);

  xpathnamefile.Empty();
  isopen    = false;
  cachesize = 0;
  
  return  (fresult == FR_OK)?true:false;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Erase(XCHAR* path, bool overwrite)
* @brief      Erase
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  overwrite : Overwrite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Erase(XCHAR* path, bool overwrite)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 
    
  XBUFFER charstr;
  
  pathstring.ConvertToASCII(charstr);     
  fresult = f_unlink(charstr.GetPtrChar());				  
  status = (fresult == FR_OK)?true:false;  
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::Rename(XCHAR* pathold, XCHAR* pathnew)
* @brief      Rename
* @ingroup    PLATFORM_STM32
* 
* @param[in]  pathold : Pathold pointer to use.
* @param[in]  pathnew : Pathnew pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::Rename(XCHAR* pathold, XCHAR* pathnew)
{  
  if(!ismount) return false;
  
  XSTRING pathstring[2];
  bool    status;
  
  pathstring[0] = pathold;
  pathstring[1] = pathnew;
  
  XBUFFER charpathold;
  XBUFFER charpathnew;
  
  pathstring[0].ConvertToASCII(charpathold);
  pathstring[1].ConvertToASCII(charpathnew);
   
  fresult = f_rename(charpathold.GetPtrChar(), charpathnew.GetPtrChar());	   
  status = (fresult == FR_OK)?true:false;  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         FILE* XSTM32FILE_NOTUNICODE::CreateStructHandle()
* @brief      Create struct handle
* @ingroup    PLATFORM_STM32
* 
* @return     FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
FILE* XSTM32FILE_NOTUNICODE::CreateStructHandle()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::DeleteStructHandle()
* @brief      Delete struct handle
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::DeleteStructHandle()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE_NOTUNICODE::ActualizeSize()
* @brief      Actualize size
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE_NOTUNICODE::ActualizeSize()
{
  return Exist(xpathnamefile.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32FILE_NOTUNICODE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32FILE_NOTUNICODE::Clean()
{
  memset(&userFATFS, 0, sizeof(FATFS));
  
  fresult = FR_OK; 
  
  memset(&file, 0, sizeof(FIL));
  memset(&fileinfo, 0, sizeof(FILINFO));
  
  ismount = false;  
}
