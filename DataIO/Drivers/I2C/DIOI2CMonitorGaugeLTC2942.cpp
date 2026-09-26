/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CMonitorGaugeLTC2942.cpp
* 
* @class      DIOI2CMONITORGAUGELTC2942
* @brief      Data Input/Output I2C Monitor Gauge LTC2942 class
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

#include "DIOI2CMonitorGaugeLTC2942.h"

#include <stdio.h>
#include <cmath>

#include "XFactory.h"
#include "XSleep.h"
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
* @fn         DIOI2CMONITORGAUGELTC2942::DIOI2CMONITORGAUGELTC2942() : DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CMONITORGAUGELTC2942::DIOI2CMONITORGAUGELTC2942() : DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CMONITORGAUGELTC2942::~DIOI2CMONITORGAUGELTC2942()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CMONITORGAUGELTC2942::~DIOI2CMONITORGAUGELTC2942()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIOI2CMONITORGAUGELTC2942::GetVoltage (void)
* @brief      Get voltage
* @ingroup    DATAIO
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIOI2CMONITORGAUGELTC2942::GetVoltage (void)
{
  if(!diostream) return false;  
  
  float voltage_return  = 0;   
  XBYTE buffer[2]       = { 0, 0 };	    
  
  if(!ReadRegister(DIOI2CMONITORGAUGELTC2942_REG_VOL_H, buffer, 2)) return 0;
  
  XDWORD read_value = buffer[1] | (buffer[0] << 8);

  if(read_value == DIOI2CMONITORGAUGELTC2942_VOLTAGE_READ_ERROR)
    {
      return 0;
    }                       

  voltage_return = (float)read_value;	
  
  return ((voltage_return* 6000) / 65535)/1000;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::SetAC(XWORD ac)
* @brief      Set AC
* @ingroup    DATAIO
* 
* @param[in]  ac : Ac value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::SetAC(XWORD ac)
{  		  
  XBYTE ctrl;
  
  // Backup control register 
  if(!ReadRegister(DIOI2CMONITORGAUGELTC2942_REG_CONTROL, ctrl))           return false;
  
  // Shutdown analog section before programming GEN_NEW AC value 
  if(!WriteRegister(DIOI2CMONITORGAUGELTC2942_REG_CONTROL, ctrl | DIOI2CMONITORGAUGELTC2942_CTL_SHUTDOWN))   return false;
  
  // Program GEN_NEW AC value 
  if(!WriteRegister(DIOI2CMONITORGAUGELTC2942_REG_AC_H, ac >> 8)!=0)      return false;
  
  if(!WriteRegister(DIOI2CMONITORGAUGELTC2942_REG_AC_L, ac & 0xFF)!=0)    return false;
  
  // Restore control register 
  if(!WriteRegister(DIOI2CMONITORGAUGELTC2942_REG_CONTROL, ctrl)!=0)      return false;
  
  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOI2CMONITORGAUGELTC2942::GetAC(void)
* @brief      Get AC
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOI2CMONITORGAUGELTC2942::GetAC(void)
{
  XBYTE   buffer[2] = { 0, 0 };
  XDWORD  AC_value = 0;
  
  if(!ReadRegister(DIOI2CMONITORGAUGELTC2942_REG_AC_H, buffer, 2)) return 0;  
  
  AC_value = buffer[1] | (buffer[0] << 8);

  return AC_value;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOI2CMONITORGAUGELTC2942::GetMaxAC(void)
* @brief      Get max AC
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOI2CMONITORGAUGELTC2942::GetMaxAC(void)
{
  return DIOI2CMONITORGAUGELTC2942_AC_MAXVALUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::End()
{
  if(!DIODEVICEI2C::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::IniDevice()
{  
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {             
          if(!Initialize()) return false;
          
          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::Initialize()
* @brief      Initialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::Initialize()
{  
  XBYTE ctrl; 
	
  if(!ReadRegister(DIOI2CMONITORGAUGELTC2942_REG_CONTROL, ctrl)) return false;
      
	// Enable auto measurement of battery voltage and temperature 
	ctrl &= DIOI2CMONITORGAUGELTC2942_CTL_ADC_MSK;  // clear ADC mode bits
	ctrl |= DIOI2CMONITORGAUGELTC2942_ADC_AUTO;     // configure mode

	// Enable analog section of the chip 
	ctrl &= ~DIOI2CMONITORGAUGELTC2942_CTL_SHUTDOWN;

	// Set prescaler M value 
  // 
  //                  0.085mAh  50momh  M
  //    qLSB =       ----------------------- = 
  //	                  	RSENSE  128

  //                  0.085mAh  50momh  16
  //    qLSB =       ----------------------- = 0,0133mAh ->  MaxValue: 870,38mAh (not valid)
  //	                  	40momh  128  
    
  //                  0.085mAh  50momh  32
  //    qLSB =       ----------------------- = 0,0265625mAh ->  MaxValue: 1.740,7mAh valid
  //	                  	40momh  128    
    
  // M=32 for 1050mA battery AC max value 39.530    
        
	ctrl &= DIOI2CMONITORGAUGELTC2942_CTL_PSCM_MSK;      // clear prescaler M bits
	ctrl |= DIOI2CMONITORGAUGELTC2942_PSCM_32;          // configure GEN_NEW value

	// Disable AL/CC pin 
	ctrl &= DIOI2CMONITORGAUGELTC2942_CTL_ALCC_MSK;     // clear AL/CC bits
	ctrl |= DIOI2CMONITORGAUGELTC2942_ALCC_DISABLED;    // configure GEN_NEW value

	// Write control register 
	if(!WriteRegister(DIOI2CMONITORGAUGELTC2942_REG_CONTROL, ctrl))
    {
      return false;
    }
    
  // Checks if there was power off and sets de acumulator in middle value
  XWORD currentAC = GetAC();    
  if(currentAC >= 0x7FF0)  SetAC(DIOI2CMONITORGAUGELTC2942_AC_MAXVALUE/2);
    	
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::ReadRegister(XBYTE offset, XBYTE& data)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::ReadRegister(XBYTE offset, XBYTE& data)
{
  return ReadRegister(offset, &data, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
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
bool DIOI2CMONITORGAUGELTC2942::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
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
* @fn         bool DIOI2CMONITORGAUGELTC2942::WriteRegister(XBYTE offset, XBYTE data)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CMONITORGAUGELTC2942::WriteRegister(XBYTE offset, XBYTE data)
{
  return WriteRegister(offset, &data, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CMONITORGAUGELTC2942::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
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
bool DIOI2CMONITORGAUGELTC2942::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
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
* @fn         void DIOI2CMONITORGAUGELTC2942::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CMONITORGAUGELTC2942::Clean()
{
  referenceresistance   = 0.0f;  
}



