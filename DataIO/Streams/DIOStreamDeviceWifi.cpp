/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamDeviceWifi.cpp
* 
* @class      DIOSTREAMDEVICEWIFI
* @brief      Data Input/Output Stream Device Wifi class
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

#include "DIOStreamDeviceWifi.h"

#include "XConsole.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICEWIFI::DIOSTREAMDEVICEWIFI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICEWIFI::DIOSTREAMDEVICEWIFI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICEWIFI::~DIOSTREAMDEVICEWIFI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICEWIFI::~DIOSTREAMDEVICEWIFI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICEWIFI::HasSecurity()
* @brief      Has security
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICEWIFI::HasSecurity()
{
  return hassecurity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICEWIFI::SetHasSecurity(bool hassecurity)
* @brief      Set has security
* @ingroup    DATAIO
* 
* @param[in]  hassecurity : Hassecurity value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICEWIFI::SetHasSecurity(bool hassecurity)
{
  this->hassecurity = hassecurity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMDEVICEWIFI::GetTransmisionPower()
* @brief      Get transmision power
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMDEVICEWIFI::GetTransmisionPower()
{
  return transmisionpower;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICEWIFI::SetTransmisionPower(int transmisionpower)
* @brief      Set transmision power
* @ingroup    DATAIO
* 
* @param[in]  transmisionpower : Transmisionpower value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICEWIFI::SetTransmisionPower(int transmisionpower)
{
  this->transmisionpower = transmisionpower;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICEWIFI::DebugPrintInfo()
* @brief      Debug print info
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICEWIFI::DebugPrintInfo()
{
  DIOSTREAMDEVICEIP::DebugPrintInfo();

  XSTRING string;
  string.ConvertFromBoolean(hassecurity, XSTRINGBOOLEANMODE_HUMAN);
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Has Security : %s "), string.Get());
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("RSSI : %d dB"), transmisionpower);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICEWIFI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICEWIFI::Clean()
{
  hassecurity       = false;
  transmisionpower  = 0;
}




