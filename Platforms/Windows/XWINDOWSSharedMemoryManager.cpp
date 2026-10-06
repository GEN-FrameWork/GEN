/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSSharedMemoryManager.cpp
* 
* @class      XWINDOWSSHAREDMEMORYMANAGER
* @brief      WINDOWS eXtended Utils Shared Memory Manager (Exchange between applications)
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

#include "XWINDOWSSharedMemoryManager.h"

#include <windows.h>
#include <sddl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSHAREDMEMORYMANAGER::XWINDOWSSHAREDMEMORYMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSHAREDMEMORYMANAGER::XWINDOWSSHAREDMEMORYMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSHAREDMEMORYMANAGER::~XWINDOWSSHAREDMEMORYMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSHAREDMEMORYMANAGER::~XWINDOWSSHAREDMEMORYMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* XWINDOWSSHAREDMEMORYMANAGER::Create(XCHAR* ID, XDWORD size, bool ispublic)
* @brief      Create resource
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  size : Size value.
* @param[in]  ispublic : Ispublic value.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* XWINDOWSSHAREDMEMORYMANAGER::Create(XCHAR* ID, XDWORD size, bool ispublic)
{
  // Allocate shared memory from the system by creating a shared memory
  // pool basing it out of the system paging (swapper) file.

  this->ID   = ID;
  this->size = (size + sizeof(XDWORD));

  if(ispublic)
    {
      SECURITY_ATTRIBUTES  attributes;
      wchar_t              sdd[71] = L"D:"
                                     L"(D;OICI;GA;;;BG)"      // Deny guests
                                     L"(D;OICI;GA;;;AN)"      // Deny anonymous
                                     L"(A;OICI;GRGWGX;;;AU)"  // Allow read, write and execute for Users
                                     L"(A;OICI;GA;;;BA)";     // Allow all for Administrators


      ZeroMemory(&attributes, sizeof(attributes));
      attributes.nLength = sizeof(attributes);

      if(ConvertStringSecurityDescriptorToSecurityDescriptor(sdd, SDDL_REVISION_1, &attributes.lpSecurityDescriptor, NULL) != TRUE) return NULL;

      handlefile = CreateFileMapping((HANDLE)NULL, &attributes, PAGE_READWRITE, 0, this->size, ID);

      LocalFree(attributes.lpSecurityDescriptor);
    }
   else
    {
      handlefile = CreateFileMapping((HANDLE)NULL, NULL, PAGE_READWRITE, 0, this->size, ID);
    }

  if(!handlefile) return NULL;

  // Map the file to this process' address space, starting at an address
  // that should also be available in child processe(s)

  base = (XBYTE*)MapViewOfFileEx(handlefile, FILE_MAP_WRITE, 0, 0, 0, 0);
  if(base)
    {
      memcpy((XBYTE*)base, (XBYTE*)&this->size, sizeof(XDWORD));
      
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("[WINDOWS Shared Mem]  Create Size [%d]"), this->size);

      pointer  = base;
      pointer += sizeof(XDWORD);
    }

  isserver = true;

  return pointer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* XWINDOWSSHAREDMEMORYMANAGER::Open(XCHAR* ID, XDWORD& size)
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  size : Size value.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* XWINDOWSSHAREDMEMORYMANAGER::Open(XCHAR* ID, XDWORD& size)
{
  // Open the shared memory file by name.  The file is based on the
  // system paging (swapper) file.

  this->ID   = ID;
  this->size = 0;

  size = 0;

  handlefile  = OpenFileMapping(FILE_MAP_WRITE, FALSE, ID);
  if(!handlefile)
    {
      //DWORD error = GetLastError();
      return NULL;
    }

  // Figure out where to map this file by looking at the address in the
  // shared memory where the memory was mapped in the parent process.

  base = (XBYTE*)MapViewOfFile(handlefile, FILE_MAP_WRITE, 0, 0, sizeof(XDWORD));
  if(base)
    {

       memcpy((XBYTE*)&this->size, base, sizeof(XDWORD));
    
       //XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("[WINDOWS Shared Mem]  Open Size [%d]"), this->size);

       UnmapViewOfFile(base);

       base = (XBYTE*)MapViewOfFile(handlefile, FILE_MAP_WRITE, 0, 0, this->size);
       if(base)
         {
           this->size -= sizeof(XDWORD);
           size = this->size;

           //XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("[WINDOWS Shared Mem]  Open Size [%d]"), this->size);
           
           pointer  = base;
           pointer += sizeof(XDWORD);
         }
    }

  isserver = false;

  return pointer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSHAREDMEMORYMANAGER::Close()
* @brief      Close Shared Memory for this instance
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSHAREDMEMORYMANAGER::Close()
{
  if(!base)    return false;
  if(!pointer) return false;

  if(!isserver) UnmapViewOfFile(base);
  
  if(handlefile) 
    {
      CloseHandle(handlefile);
      handlefile = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSSHAREDMEMORYMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSHAREDMEMORYMANAGER::Clean()
{
  handlefile  = NULL;
}



