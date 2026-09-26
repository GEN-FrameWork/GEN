/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamDeviceBluetoothLE.cpp
* 
* @class      DIOSTREAMDEVICEBLUETOOTHLE
* @brief      Data Input/Output Stream Device Bluetooth LE class
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

#include "DIOStreamDeviceBluetoothLE.h"

#include <string.h>

#include "XFactory.h"
#include "XString.h"
#include "XTimer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICEBLUETOOTHLE::DIOSTREAMDEVICEBLUETOOTHLE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICEBLUETOOTHLE::DIOSTREAMDEVICEBLUETOOTHLE()
{
  Clean();
  
  lastscantimer=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICEBLUETOOTHLE::~DIOSTREAMDEVICEBLUETOOTHLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICEBLUETOOTHLE::~DIOSTREAMDEVICEBLUETOOTHLE()
{
  if(lastscantimer) 
    {
      GEN_XFACTORY.DeleteTimer(lastscantimer);
      lastscantimer = NULL;
    } 

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMDEVICEBLUETOOTHLE::GetAdvertisingData()
* @brief      Get advertising data
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMDEVICEBLUETOOTHLE::GetAdvertisingData()
{
  return &advertisingdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOSTREAMDEVICEBLUETOOTHLE::GetLastScanTimer()
* @brief      Get last scan timer
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOSTREAMDEVICEBLUETOOTHLE::GetLastScanTimer()
{
  return lastscantimer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICEBLUETOOTHLE::IsReadLastScan()
* @brief      Is read last scan
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTHLE::IsReadLastScan()
{
  return isreadlastscan;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICEBLUETOOTHLE::SetIsReadLastScan(bool isreadlastscan)
* @brief      Set is read last scan
* @ingroup    DATAIO
* 
* @param[in]  isreadlastscan : Isreadlastscan value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICEBLUETOOTHLE::SetIsReadLastScan(bool isreadlastscan)
{
  this->isreadlastscan = isreadlastscan;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICEBLUETOOTHLE::DebugPrintInfo()
* @brief      Debug print info
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTHLE::DebugPrintInfo()
{
  DIOSTREAMDEVICE::DebugPrintInfo();

  XSTRING line;
  XSTRING data;

  GetMAC()->GetXString(data);
  line.Format(__L("MAC : %s"), data.Get());                          
  XTRACE_PRINTCOLOR(2, line.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICEBLUETOOTHLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICEBLUETOOTHLE::Clean()
{
  type            = DIOSTREAMDEVICE_TYPE_BLUETOOTHLE;

  rssi            = 0;
  lastscantimer   = NULL;
  isreadlastscan  = false;
  isvisible       = false;
}














