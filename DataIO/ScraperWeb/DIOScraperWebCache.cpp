/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebCache.cpp
* 
* @class      DIOSCRAPERWEBCACHE
* @brief      Data Input/Output Scraper Web cache class
* @ingroup    DATAIO
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

#include "DIOScraperWebCache.h"

#include "XFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBCACHE_RESULT::DIOSCRAPERWEBCACHE_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBCACHE_RESULT::DIOSCRAPERWEBCACHE_RESULT()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBCACHE_RESULT::~DIOSCRAPERWEBCACHE_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBCACHE_RESULT::~DIOSCRAPERWEBCACHE_RESULT()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBCACHE_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBCACHE_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBCACHE_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBCACHE_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBCACHE::DIOSCRAPERWEBCACHE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBCACHE::DIOSCRAPERWEBCACHE()
{
  Clean();

  xmutexlist=GEN_XFACTORY.Create_Mutex();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBCACHE::~DIOSCRAPERWEBCACHE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBCACHE::~DIOSCRAPERWEBCACHE()
{
  GEN_XFACTORY.Delete_Mutex(xmutexlist);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBCACHE::Add(XSTRING& ask, DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Add value
* @ingroup    DATAIO
* 
* @param[in]  ask : Ask value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBCACHE::Add(XSTRING& ask, DIOSCRAPERWEBCACHE_RESULT* result)
{
  DIOSCRAPERWEBCACHE_RESULT* _result = Get(ask);
  if(_result)  return false;

  if(xmutexlist) xmutexlist->Lock();

  bool status = false;

  XSTRING* _ask = GEN_NEW XSTRING();
  if(_ask)
    {
      (*_ask) = ask.Get();
      if(cache.Add(_ask, result))
        {
          status = true;

        } else GEN_DELETE _ask;
    }

  if(xmutexlist) xmutexlist->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBCACHE_RESULT* DIOSCRAPERWEBCACHE::Get(XSTRING& ask)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  ask : Ask value.
* 
* @return     DIOSCRAPERWEBCACHE_RESULT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBCACHE_RESULT* DIOSCRAPERWEBCACHE::Get(XSTRING& ask)
{
  if(xmutexlist) xmutexlist->Lock();

  for(XDWORD c=0;c<cache.GetSize();c++)
    {
      XSTRING* _ask = (XSTRING*)cache.GetKey(c);
      if(_ask)
        {
          if(!_ask->Compare(ask.Get()))
            {
              DIOSCRAPERWEBCACHE_RESULT* result  = cache.GetElement(c);
              if(result)
                {
                  if(xmutexlist) xmutexlist->UnLock();
                  return result;
                }
            }
        }
    }

  if(xmutexlist) xmutexlist->UnLock();

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBCACHE::DeleteAll()
* @brief      Delete all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBCACHE::DeleteAll()
{
  if(cache.IsEmpty())  return false;

  if(xmutexlist) xmutexlist->Lock();

  cache.DeleteKeyContents();
  cache.DeleteElementContents();
  cache.DeleteAll();

  if(xmutexlist) xmutexlist->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBCACHE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBCACHE::Clean()
{
  xmutexlist = NULL;
}





