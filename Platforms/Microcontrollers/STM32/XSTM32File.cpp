/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32File.cpp
*
* @class      XSTM32FILE
* @brief      eXtended STM32 File class (Use FatFs - Generic FAT file system module)
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

#include "XSTM32File.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32FILE::XSTM32FILE(): XFILE()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32FILE::XSTM32FILE(): XFILE()
{
  Clean();
  
  XSTRING root;
  
  root = _L("/");
    
  XBUFFER xbufferexchange;
  root.ConvertToBufferExchange(xbufferexchange);
  
  fresult = f_mount(&userFATFS, xbufferexchange.Get(), 1);
    
  if(fresult == FR_OK) ismount = true; 
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32FILE::~XSTM32FILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32FILE::~XSTM32FILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::Exist(XCHAR* path)
* @brief      Exist
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Exist(XCHAR* path)
{  
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 
  
  XBUFFER xbufferexchange;
  pathstring.ConvertToBufferExchange(xbufferexchange);

  fresult = f_stat(xbufferexchange.Get(), &fileinfo);			
  
  cachesize =0;
  if(fresult == FR_OK) cachesize = (XQWORD)fileinfo.fsize;  
  
  status = (fresult == FR_OK)?true:false;  
   
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::Open(XCHAR* path, bool isreadonly)
* @brief      Open
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  isreadonly : Isreadonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Open(XCHAR* path, bool isreadonly)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 

  XBYTE mode  = (FA_OPEN_ALWAYS | FA_READ);
    
  if(!isreadonly) mode |= FA_WRITE;
   
  XBUFFER xbufferexchange;
  pathstring.ConvertToBufferExchange(xbufferexchange);
  fresult = f_open(&file, xbufferexchange.Get() , mode);
    
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
* @fn         bool XSTM32FILE::Create(XCHAR* path)
* @brief      Create resource
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Create(XCHAR* path)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 

  XBYTE mode  = (FA_CREATE_ALWAYS | FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
  
  XBUFFER xbufferexchange;
  pathstring.ConvertToBufferExchange(xbufferexchange);
  fresult = f_open(&file, xbufferexchange.Get() , mode);
    
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
* @fn         bool XSTM32FILE::SetSize(XQWORD size)
* @brief      Set size
* @ingroup    PLATFORM_STM32
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::SetSize(XQWORD size)
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
* @fn         bool XSTM32FILE::GetPosition(XQWORD& position)
* @brief      Get position
* @ingroup    PLATFORM_STM32
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::GetPosition(XQWORD& position)
{
  position = 0;
  
  if(!isopen) return false;
   
  FSIZE_t _position = f_tell(&file);
  
  position = (XQWORD)_position;   
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::SetPosition(XQWORD position)
* @brief      Set position
* @ingroup    PLATFORM_STM32
* 
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::SetPosition(XQWORD position)
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
* @fn         bool XSTM32FILE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
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
bool XSTM32FILE::Read(XBYTE* buffer, XDWORD size, CIPHER* cipher)
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
* @fn         bool XSTM32FILE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
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
bool XSTM32FILE::Read(XBYTE* buffer, XDWORD* size, CIPHER* cipher)
{ 
  if(!isopen) return false;

  UINT  sizetoread = (UINT)(*size);
  UINT  readsize   = 0;
  bool  status     = false;
  
  (*size) = 0;

  fresult = f_read(&file, (void*)buffer, sizetoread, &readsize);	

  status = (fresult == FR_OK)?true:false;  
  if(status) (*size) = readsize;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
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
bool XSTM32FILE::Write(XBYTE* buffer, XDWORD size, CIPHER* cipher)
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
* @fn         bool XSTM32FILE::Flush()
* @brief      Flush
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Flush()
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
* @fn         bool XSTM32FILE::Close()
* @brief      Close
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Close()
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
* @fn         bool XSTM32FILE::Erase(XCHAR* path, bool overwrite)
* @brief      Erase
* @ingroup    PLATFORM_STM32
* 
* @param[in]  path : Path to use.
* @param[in]  overwrite : Overwrite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Erase(XCHAR* path, bool overwrite)
{
  if(!ismount) return false;
  
  XSTRING pathstring;
  bool    status;

  pathstring = path;
  if(pathstring.IsEmpty()) return false; 
  
  XBUFFER xbufferexchange;
  pathstring.ConvertToBufferExchange(xbufferexchange);
  fresult = f_unlink(xbufferexchange.Get());
    
  status = (fresult == FR_OK)?true:false;  
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::Rename(XCHAR* pathold, XCHAR* pathnew)
* @brief      Rename
* @ingroup    PLATFORM_STM32
* 
* @param[in]  pathold : Pathold pointer to use.
* @param[in]  pathnew : Pathnew pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::Rename(XCHAR* pathold, XCHAR* pathnew)
{  
  if(!ismount) return false;
  
  XSTRING   pathold_str;
  XSTRING   pathnew_str;
  bool      status;
  
  pathold_str = pathold;
  pathnew_str = pathnew;
    
  XBUFFER xbufferexchange_pathold;
  XBUFFER xbufferexchange_pathnew;
  
  pathold_str.ConvertToBufferExchange(xbufferexchange_pathold);
  pathnew_str.ConvertToBufferExchange(xbufferexchange_pathnew);
     
  fresult = f_rename(xbufferexchange_pathold.Get(), xbufferexchange_pathnew.Get());	
    
  status = (fresult == FR_OK)?true:false;  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         FILE* XSTM32FILE::CreateStructHandle()
* @brief      Create struct handle
* @ingroup    PLATFORM_STM32
* 
* @return     FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
FILE* XSTM32FILE::CreateStructHandle()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::DeleteStructHandle()
* @brief      Delete struct handle
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::DeleteStructHandle()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FILE::ActualizeSize()
* @brief      Actualize size
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FILE::ActualizeSize()
{
  return Exist(xpathnamefile.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32FILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32FILE::Clean()
{
  memset(&userFATFS, 0, sizeof(FATFS));
  
  fresult = FR_OK; 
  
  memset(&file, 0, sizeof(FIL));
  memset(&fileinfo, 0, sizeof(FILINFO));
  
  ismount = false;  
}
