/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CTouchSensorAT42QT1060.cpp
* 
* @class      DIOI2CTOUCHSENSORAT42QT1060
* @brief      Data Input/Output I2C Touch Sensor AT42QT1060 class
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

#include "DIOI2CTouchSensorAT42QT1060.h"

#include <stdio.h>
#include <cmath>

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
#include "DIOGPIO.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CTOUCHSENSORAT42QT1060::DIOI2CTOUCHSENSORAT42QT1060() : DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CTOUCHSENSORAT42QT1060::DIOI2CTOUCHSENSORAT42QT1060() : DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CTOUCHSENSORAT42QT1060::~DIOI2CTOUCHSENSORAT42QT1060()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CTOUCHSENSORAT42QT1060::~DIOI2CTOUCHSENSORAT42QT1060()

{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOI2CTOUCHSENSORAT42QT1060::GetGPIOInterruptEntryID()
* @brief      Get GPIO interrupt entry ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOI2CTOUCHSENSORAT42QT1060::GetGPIOInterruptEntryID()
{
  return GPIOinterruptentryID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::SetGPIOInterruptEntryID(XDWORD GPIOentryID)
* @brief      Set GPIO interrupt entry ID
* @ingroup    DATAIO
* 
* @param[in]  GPIOentryID : GPI Oentry ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::SetGPIOInterruptEntryID(XDWORD GPIOentryID)
{
  GPIOinterruptentryID = GPIOentryID;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::GetHW_ID(XBYTE& ID)
* @brief      Get HWID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::GetHW_ID(XBYTE& ID)
{
  if(!diostream) return false;
  
  ID = 0;

  XBYTE valread;

  if(!ReadRegister(DIOI2CTOUCHSENSORAT42QT1060_CHIP_ID_REG, valread)) return false;

  ID = valread;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::IsNecessaryUpdate()
* @brief      Is necessary update
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::IsNecessaryUpdate()
{
  return isnecessaryupdate;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2CTOUCHSENSORAT42QT1060::SetIsNecessaryUpdate(bool isnecessaryupdate)
* @brief      Set is necessary update
* @ingroup    DATAIO
* 
* @param[in]  isnecessaryupdate : Isnecessaryupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CTOUCHSENSORAT42QT1060::SetIsNecessaryUpdate(bool isnecessaryupdate)
{
  this->isnecessaryupdate = isnecessaryupdate;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2CTOUCHSENSORAT42QT1060::GetStatus()
* @brief      Get status
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2CTOUCHSENSORAT42QT1060::GetStatus()
{
  XBYTE regvalue = 0xFF;
  
  if(IsNecessaryUpdate())
    {      
      prevtouch = regvalue;
    
      ReadRegister(DIOI2CTOUCHSENSORAT42QT1060_INPUT_PORT_STATUS_REG, regvalue);
      
      SetIsNecessaryUpdate(false);
    }
        
  return regvalue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::End()
{
  if(!DIODEVICE::End()) return false;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::IniDevice()
{
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {                               
          if(!Initializate()) return false;
          
          return DIODEVICE::Ini();
        }
    }

  return false;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::Initializate()
* @brief      Initializate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::Initializate()
{
  XBYTE hardwareID = 0;
           
  if(!GetHW_ID(hardwareID)) return false;
  if(hardwareID == DIOI2CTOUCHSENSORAT42QT1060_HW_ID) return false;
  
	//  Key Mask Enable CHG interrupt for KEYs 0-3
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_KEY_MASK_REG, 0x0F)) return false;
  GEN_XSLEEP.MilliSeconds(10);

  // AKS Mask Enable KEYs 0-3 in the AKS groups
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_AKS_MASK_REG, 0x0F)) return false;
  GEN_XSLEEP.MilliSeconds(10);
  
  // NTHR Key Threshold value KEY0. Count of Xms pooling  
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_0, 30)) return false; 
  GEN_XSLEEP.MilliSeconds(10);
  
  // NTHR Key Threshold value KEY1. Count of Xms pooling  
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_1, 30)) return false;
  GEN_XSLEEP.MilliSeconds(10);

  // NTHR Key Threshold value KEY2. Count of Xms pooling  
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_2, 30)) return false;
  GEN_XSLEEP.MilliSeconds(10);

  //NTHR Key Threshold value KEY3. Count of Xms pooling  
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_3, 30)) return false;
  GEN_XSLEEP.MilliSeconds(10);  
  
	//LP Mode	16ms pooling  
  if(!WriteRegister(DIOI2CTOUCHSENSORAT42QT1060_LPM_MODE, 0x01)) return false;
  GEN_XSLEEP.MilliSeconds(10);


  // Charger GPIO Interrupts
  DIOGPIO_ENTRY* interrupt = GEN_DIOGPIO.GPIOEntry_GetByID(GPIOinterruptentryID);
  if(interrupt)
    {
      interrupt->SetIntFunctionPointer(DIOI2CTOUCHSENSORAT42QT1060::Handler_Interrupt);  
      interrupt->SetIntParamPointer((void*)this);      
      GEN_DIOGPIO.SetMode(GPIOinterruptentryID, DIOGPIO_MODE_INTERRUPT); 
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::ReadRegister(XBYTE offset, XBYTE& data)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::ReadRegister(XBYTE offset, XBYTE& data)
{
  return ReadRegister(offset, &data, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizedata : Sizedata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
{
  if(!diostream) return false;

  bool status;

  status = diostream->Write(&offset, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  if(!status) return false;

  status = diostream->WaitToFilledReadingBuffer(sizedata, timeout);
  if(status) status = (diostream->Read(data, sizedata) == sizedata)?true:false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::WriteRegister(XBYTE offset, XBYTE data)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::WriteRegister(XBYTE offset, XBYTE data)
{
  return WriteRegister(offset, &data, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CTOUCHSENSORAT42QT1060::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizedata : Sizedata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CTOUCHSENSORAT42QT1060::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
{
  if(!diostream) return false;

  XBUFFER buffer;
  bool    status;

  buffer.Add((XBYTE)offset);
  if(data && sizedata) buffer.Add(data, sizedata);

  status = diostream->Write(buffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2CTOUCHSENSORAT42QT1060::Handler_Interrupt(void* param)
* @brief      Handler interrupt
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CTOUCHSENSORAT42QT1060::Handler_Interrupt(void* param)
{
  DIOI2CTOUCHSENSORAT42QT1060* touchsensor = (DIOI2CTOUCHSENSORAT42QT1060*)param;
  if(touchsensor) touchsensor->SetIsNecessaryUpdate(true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2CTOUCHSENSORAT42QT1060::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CTOUCHSENSORAT42QT1060::Clean()
{
  GPIOinterruptentryID  = DIOGPIO_ID_NOTDEFINED;
  isnecessaryupdate     = false;  
  prevtouch             = 0;    
}



