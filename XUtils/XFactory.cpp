/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFactory.cpp
* 
* @class      XFACTORY
* @brief      eXtended Utils factory
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

#include "XFactory.h"

#include "XTimer.h"
#include "XDateTime.h"
#include "XRand.h"
#include "XFile.h"
#include "XDir.h"
#include "XSystem.h"
#ifdef XCONSOLE_ACTIVE
#include "XConsole.h"
#endif
#include "XThread.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XFACTORY* XFACTORY::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFACTORY::XFACTORY()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFACTORY::XFACTORY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFACTORY::~XFACTORY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFACTORY::~XFACTORY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::GetIsInstanced()
* @brief      Get is instanced
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFACTORY& XFACTORY::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XFACTORY& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFACTORY& XFACTORY::GetInstance()
{
  if(!instance)
    {
      instance = GEN_NEW XFACTORY();
    }

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::SetInstance(XFACTORY* _instance)
* @brief      Set instance
* @ingroup    XUTILS
* 
* @param[in]  _instance : Instance pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::SetInstance(XFACTORY* _instance)
{
  if(!_instance) return false;

  DelInstance();

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DelInstance()
{
  if(!instance) return false;

  GEN_DELETE instance;
  instance = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* XFACTORY::CreateTimer()
* @brief      Create timer
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* XFACTORY::CreateTimer()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DeleteTimer(XTIMER* timer)
* @brief      Delete timer
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  timer : timer to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DeleteTimer(XTIMER* timer)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XFACTORY::CreateDateTime()
* @brief      Create date time
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XFACTORY::CreateDateTime()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DeleteDateTime(XDATETIME* datetime)
* @brief      Delete date time
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  datetime : datetime to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DeleteDateTime(XDATETIME* datetime)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRAND* XFACTORY::CreateRand()
* @brief      Create rand
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XRAND* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRAND* XFACTORY::CreateRand()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DeleteRand(XRAND* rand)
* @brief      Delete rand
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  rand : rand to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DeleteRand(XRAND* rand)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILE* XFACTORY::Create_File()
* @brief      Create file
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILE* XFACTORY::Create_File()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::Delete_File(XFILE* file)
* @brief      Delete file
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  file : file to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::Delete_File(XFILE* file)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIR* XFACTORY::Create_Dir()
* @brief      Create dir
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XDIR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIR* XFACTORY::Create_Dir()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::Delete_Dir(XDIR* dir)
* @brief      Delete dir
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  dir : dir to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::Delete_Dir(XDIR* dir)
{
  return false;
}


#ifdef XCONSOLE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCONSOLE* XFACTORY::CreateConsole()
* @brief      Create console
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XCONSOLE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCONSOLE* XFACTORY::CreateConsole()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DeleteConsole(XCONSOLE* xconsole)
* @brief      Delete console
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  xconsole : xconsole to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DeleteConsole(XCONSOLE* xconsole)
{
  return false;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* XFACTORY::Create_Mutex()
* @brief      Create mutex
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* XFACTORY::Create_Mutex()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::Delete_Mutex(XMUTEX* mutex)
* @brief      Delete mutex
* @ingroup    XUTILS
* 
* @param[in]  mutex : Mutex pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::Delete_Mutex(XMUTEX* mutex)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD* XFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Create thread
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  param : Param pointer to use.
* 
* @return     XTHREAD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD* XFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
* @brief      Delete thread
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  groupID : group ID of threads
* @param[in]  xthread : thread to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFACTORY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFACTORY::Clean()
{

}



