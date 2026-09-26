/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSEEPROMMemoryManager.cpp
* 
* @class      XWINDOWSEEPROMMEMORYMANAGER
* @brief      WINDOWS eXtended Utils EEPROM Memory Manager class
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

#include "XWINDOWSEEPROMMemoryManager.h"

#include <stdio.h>
#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSEEPROMMEMORYMANAGER::XWINDOWSEEPROMMEMORYMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSEEPROMMEMORYMANAGER::XWINDOWSEEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSEEPROMMEMORYMANAGER::~XWINDOWSEEPROMMEMORYMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSEEPROMMEMORYMANAGER::~XWINDOWSEEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Ini()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::End()
* @brief      End the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::End()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE& data)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XWORD& data)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XDWORD& data)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XDWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE data)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XWORD data)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XWORD data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XDWORD data)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XDWORD data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSEEPROMMEMORYMANAGER::EraseAll()
* @brief      Erase all
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSEEPROMMEMORYMANAGER::EraseAll()
{
  return false;
}




