/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XANDROIDSystem.cpp
* 
* @class      XANDROIDSYSTEM
* @brief      ANDROID eXtended System class
* @ingroup    PLATFORM_ANDROID
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

#include "XANDROIDSystem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDSYSTEM::XANDROIDSYSTEM(): XLINUXSYSTEM()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDSYSTEM::XANDROIDSYSTEM(): XLINUXSYSTEM()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDSYSTEM::~XANDROIDSYSTEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDSYSTEM::~XANDROIDSYSTEM()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_HARDWARETYPE XANDROIDSYSTEM::GetTypeHardware(int* revision)
* @brief      Get type hardware
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  revision : revision if is possible
* 
* @return     XSYSTEM_HARDWARETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XANDROIDSYSTEM::GetTypeHardware(int* revision)
{
  return XSYSTEM_HARDWARETYPE_ARMGENERIC;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_PLATFORM XANDROIDSYSTEM::GetPlatform(XSTRING* namestring)
* @brief      Get platform
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  namestring : Namestring pointer to use.
* 
* @return     XSYSTEM_PLATFORM : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_PLATFORM XANDROIDSYSTEM::GetPlatform(XSTRING* namestring)
{
  if(namestring)  namestring->Set(_L("Android"));

  return XSYSTEM_PLATFORM_ANDROID;  
}



