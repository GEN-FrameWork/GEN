/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XRand.cpp
* 
* @class      XRAND
* @brief      eXtended Utils Random class
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

#include "XRand.h"

#include <time.h>

#include "XBase.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XRAND* XRAND::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRAND::XRAND()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRAND::XRAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRAND::~XRAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRAND::~XRAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XRAND::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRAND& XRAND::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XRAND& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRAND& XRAND::GetInstance()
{
  if(!instance) instance = GEN_NEW XRAND();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XRAND::SetInstance(XRAND* _instance)
* @brief      Set instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @param[in]  _instance : GEN_NEW instance
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::SetInstance(XRAND* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XRAND::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::DelInstance()
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
* @fn         bool XRAND::Ini()
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::Ini()
{
  const int* prime1    = GEN_NEW int();
  const int* prime2    = GEN_NEW int();

  XQWORD initialvaluelong = (XQWORD)time(NULL);

  initialvaluelong += (XQWORD)prime1;
  initialvaluelong += (XQWORD)prime2;

  GEN_DELETE prime1;
  GEN_DELETE prime2;

  srand((int)(initialvaluelong));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XRAND::Generate(XBYTE* buffer, XDWORD size)
* @brief      Generate random bytes
* @ingroup    XUTILS
* 
* @param[out] buffer : Buffer where the random bytes are stored.
* @param[in]  size : Size of the buffer in bytes.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::Generate(XBYTE* buffer, XDWORD size)
{
  if(!size)
    {
      return true;
    }

  if(!buffer)
    {
      return false;
    }

  for(XDWORD c=0; c<size; c++)
    {
      buffer[c] = (XBYTE)GETRANDOM(0, 255);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XRAND::IsCryptographicallySecure()
* @brief      Report whether Generate() is backed by a cryptographically secure random source
* @ingroup    XUTILS
*
* @return     bool : true only for platform implementations backed by a CSPRNG.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::IsCryptographicallySecure()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XRAND::MaxElements(int max)
* @brief      Max elements
* @ingroup    XUTILS
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XRAND::MaxElements(int max)
{
  if(max <= 0)
    {
      return 0;
    }

  return Between(0, max-1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XRAND::Max(int max)
* @brief      Max
* @ingroup    XUTILS
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XRAND::Max(int max)
{
  if(max <= 0)
    {
      return 0;
    }

  return Between(0, max);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XRAND::Between(int min, int max)
* @brief      Between
* @ingroup    XUTILS
* 
* @param[in]  min : Min value.
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XRAND::Between(int min, int max)
{
  if(min >= max)
    {
      return min;
    }

  XQWORD range  = (XQWORD)((XQWORDSIG)max - (XQWORDSIG)min) + 1;
  XQWORD total  = ((XQWORD)0xFFFFFFFF) + 1;
  XQWORD limit  = total - (total % range);
  XDWORD value  = 0;

  do
    {
      if(!Generate((XBYTE*)&value, sizeof(value)))
        {
          return min;
        }
    }
  while((XQWORD)value >= limit);

  return (int)((XQWORDSIG)min + (XQWORDSIG)((XQWORD)value % range));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XRAND::Between(float min, float max)
* @brief      Between
* @ingroup    XUTILS
* 
* @param[in]  min : Min value.
* @param[in]  max : Max value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XRAND::Between(float min, float max)
{
  if(min >= max)
    {
      return min;
    }

  XDWORD value = 0;

  if(!Generate((XBYTE*)&value, sizeof(value)))
    {
      return min;
    }

  value >>= 8;

  float delta = ((float)value) / 16777215.0f;

  return ((max-min)*delta)+min;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XRAND::Percent(int percent)
* @brief      Percent
* @ingroup    XUTILS
* 
* @param[in]  percent : Percent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XRAND::Percent(int percent)
{
  if(percent <= 0)   return false;
  if(percent >= 100) return true;

  int random = Between(1, 100);
  if(random <= percent) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XRAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XRAND::Clean()
{

}
