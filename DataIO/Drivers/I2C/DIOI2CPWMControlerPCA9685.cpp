/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CPWMControlerPCA9685.cpp
* 
* @class      DIOI2CPWMCONTROLERPCA9685
* @brief      Data Input/Output I2C PCA9685 (16-channel, 12-bit PWM controller) class
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

#include "DIOI2CPWMControlerPCA9685.h"

#include <cmath>

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CPWMCONTROLERPCA9685::DIOI2CPWMCONTROLERPCA9685(): DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CPWMCONTROLERPCA9685::DIOI2CPWMCONTROLERPCA9685(): DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CPWMCONTROLERPCA9685::~DIOI2CPWMCONTROLERPCA9685()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CPWMCONTROLERPCA9685::~DIOI2CPWMCONTROLERPCA9685()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::Reset()
{
  return SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, 0x00);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::SetPWMFrequency(float frequency)
* @brief      Set PWM frequency
* @ingroup    DATAIO
* 
* @param[in]  frequency : Frequency value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SetPWMFrequency(float frequency)
{
  if(!IsInitialized()) return false;

  float prescaleval;

  prescaleval  = 25000000;        //  25MHz
  prescaleval /= 4096;            //  12 bits
  prescaleval /= frequency;
  prescaleval -= 1;

  XBYTE prescale     = (XBYTE)floor(prescaleval + 0.5f);
  XBYTE oldmode;
  bool  status;

  status = ReadCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, oldmode);
  if(!status) return false;

  XBYTE newmode = (oldmode&0x7F) | 0x10; // sleep

  status = SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, newmode);       // go to sleep
  if(!status) return false;

  status = SendCommand(DIOI2CPWMCONTROLERPCA9685_PRESCALE , prescale);      // set the prescaler
  if(!status) return false;

  status = SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, oldmode);
  if(!status) return false;

  xtimerout->SetMilliSeconds(50);

  return SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, (oldmode | 0x80));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::SetPWM(XBYTE channel, XWORD on, XWORD off, bool wait)
* @brief      Set PWM
* @ingroup    DATAIO
* 
* @param[in]  channel : Channel value.
* @param[in]  on : On value.
* @param[in]  off : Off value.
* @param[in]  wait : Wait value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SetPWM(XBYTE channel, XWORD on, XWORD off, bool wait)
{
  if(!IsInitialized())  return false;

  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_ON_L  + (4*channel), (on  &  0xFF), wait)) return false;
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_ON_H  + (4*channel), (on  >> 0x08), wait)) return false;
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_OFF_L + (4*channel), (off &  0xFF), wait)) return false;
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_OFF_H + (4*channel), (off >> 0x08), wait)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::End()
{
  if(!DIODEVICEI2C::End()) return false;

  isinitialized = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::IniDevice()
{  
  if(!DIODEVICEI2C::IniDevice()) return false;
  
  if(diostream->Open())  
    {
      if(!diostream->WaitToConnected(timeout)) return false;
     
      if(!DIODEVICE::Ini()) return false;
      
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::ReadCommand(XBYTE cmd, XBYTE& value)
* @brief      Read command
* @ingroup    DATAIO
* 
* @param[in]  cmd : Cmd value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::ReadCommand(XBYTE cmd, XBYTE& value)
{
  if(!diostream) return false;

  bool  status;

  status = diostream->Write(&cmd, 1)?true:false;

  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);
  if(!status) return false;

  status = diostream->WaitToFilledReadingBuffer(1, timeout);
  if(status) status = diostream->Read(&value, 1)?true:false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CPWMCONTROLERPCA9685::SendCommand(XBYTE cmd, XBYTE value, bool wait)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  cmd : Cmd value.
* @param[in]  value : Value value.
* @param[in]  wait : Wait value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SendCommand(XBYTE cmd, XBYTE value, bool wait)
{
  if(!diostream)  return false;

  XBUFFER xbuffer;

  xbuffer.Add((XBYTE)cmd);
  xbuffer.Add((XBYTE)value);

  XDWORD size_written = diostream->Write(xbuffer);
  bool status = (size_written>0)?true:false;

  if(wait)
    {
      if(status) status = diostream->WaitToFlushOutXBuffer(timeout);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2CPWMCONTROLERPCA9685::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CPWMCONTROLERPCA9685::Clean()
{
  
}



