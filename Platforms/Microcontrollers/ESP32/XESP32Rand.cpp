/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XESP32Rand.cpp
* 
* @class      XESP32RAND
* @brief      eXtended utils ESP32 rand class
* @ingroup    PLATFORM_ESP32
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

#include "XESP32Rand.h"

#include "esp_random.h"

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XESP32RAND::XESP32RAND()
* @brief      Constructor of class
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XESP32RAND::XESP32RAND()
{
  Clean();

  Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XESP32RAND::~XESP32RAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XESP32RAND::~XESP32RAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XESP32RAND::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XESP32RAND::Ini()
{
  XBYTE data = 0;
  return Generate(&data, sizeof(data));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XESP32RAND::IsCryptographicallySecure()
* @brief      Report whether the ESP32 provider is cryptographically secure
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XESP32RAND::IsCryptographicallySecure()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XESP32RAND::Generate(XBYTE* buffer, XDWORD size)
* @brief      Generate random bytes using the ESP-IDF hardware-backed random provider
* @ingroup    PLATFORM_ESP32
* 
* @param[out] buffer : Destination buffer.
* @param[in]  size : Number of bytes to generate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XESP32RAND::Generate(XBYTE* buffer, XDWORD size)
{
  if(!size) return true;
  if(!buffer) return false;

  esp_fill_random(buffer, size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XESP32RAND::MaxElements(int max)
* @brief      Max elements
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XESP32RAND::MaxElements(int max)
{
  return GETRANDOM(0, max-1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XESP32RAND::Max(int max)
* @brief      Max
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XESP32RAND::Max(int max)
{
  return GETRANDOM(0, max);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XESP32RAND::Between(int min, int max)
* @brief      Between
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  min : Min value.
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XESP32RAND::Between(int min, int max)
{
  return GETRANDOM(min, max);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XESP32RAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XESP32RAND::Clean()
{
  initialvalue = 0;
}



