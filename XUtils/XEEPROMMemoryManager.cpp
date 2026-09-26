/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XEEPROMMemoryManager.cpp
* 
* @class      XEEPROMMEMORYMANAGER
* @brief      eXtended Utils EEPROM Memory Manager class
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

#include "XEEPROMMemoryManager.h"

#include <stdio.h>
#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XEEPROMMEMORYMANAGER* XEEPROMMEMORYMANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XEEPROMMEMORYMANAGER::XEEPROMMEMORYMANAGER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XEEPROMMEMORYMANAGER::XEEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XEEPROMMEMORYMANAGER::~XEEPROMMEMORYMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XEEPROMMEMORYMANAGER::~XEEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::GetIsInstanced()
* @brief      Get is instanced
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XEEPROMMEMORYMANAGER& XEEPROMMEMORYMANAGER::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XEEPROMMEMORYMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XEEPROMMEMORYMANAGER& XEEPROMMEMORYMANAGER::GetInstance()
{
  if(!instance) instance = GEN_NEW XEEPROMMEMORYMANAGER();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::SetInstance(XEEPROMMEMORYMANAGER* _instance)
* @brief      Set instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @param[in]  _instance : GEN_NEW instance
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::SetInstance(XEEPROMMEMORYMANAGER* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Ini()
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Ini()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::End()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE& data)
* @brief      Read
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XWORD& data)
* @brief      Read
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XDWORD& data)
* @brief      Read
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XDWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Read
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE data)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XWORD data)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XWORD data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XDWORD data)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XDWORD data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XEEPROMMEMORYMANAGER::EraseAll()
* @brief      Erase all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XEEPROMMEMORYMANAGER::EraseAll()
{
  return false;
}



