/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CLightSensorTSL2561.cpp
* 
* @class      DIOI2CLIGHTSENSORTSL2561
* @brief      Data Input/Output I2C Sensor Light TSL2561 (Luminosity Sensor) class
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

#include "DIOI2CLightSensorTSL2561.h"

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
* @fn         DIOI2CLIGHTSENSORTSL2561::DIOI2CLIGHTSENSORTSL2561(): DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CLIGHTSENSORTSL2561::DIOI2CLIGHTSENSORTSL2561(): DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2CLIGHTSENSORTSL2561::~DIOI2CLIGHTSENSORTSL2561()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2CLIGHTSENSORTSL2561::~DIOI2CLIGHTSENSORTSL2561()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::SetIntegrationTime(DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME integrationtime)
* @brief      Set integration time
* @ingroup    DATAIO
* 
* @param[in]  integrationtime : Integrationtime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::SetIntegrationTime(DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME integrationtime)
{
  if(!diostream)  return false;

  if(!Activate()) return false;

  bool status;

  status = WriteReg(DIOI2CLIGHTSENSORTSL2561REGISTER_TIMING, ((XBYTE)integrationtime | (XBYTE)gain), timeout);
  if(status) this->integrationtime = integrationtime;

  if(status) status = Deactivate();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::SetGain(DIOI2CLIGHTSENSORTSL2561GAIN gain)
* @brief      Set gain
* @ingroup    DATAIO
* 
* @param[in]  gain : Gain value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::SetGain(DIOI2CLIGHTSENSORTSL2561GAIN gain)
{
  if(!diostream)  return false;

  if(!Activate()) return false;

  bool status;

  status = WriteReg(DIOI2CLIGHTSENSORTSL2561REGISTER_TIMING, ((XBYTE)integrationtime | (XBYTE)gain), timeout);
  if(status) this->gain = gain;

  if(status) status = Deactivate();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::Get(XWORD& fullspectrum, XWORD& infrared, XDWORD& lux, bool autogain)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  fullspectrum : Fullspectrum value.
* @param[in]  infrared : Infrared value.
* @param[in]  lux : Lux value.
* @param[in]  autogain : Autogain value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::Get(XWORD& fullspectrum, XWORD& infrared, XDWORD& lux, bool autogain)
{
  fullspectrum = 0;
  infrared     = 0;
  lux          = 0;

  if(!diostream) return false;

  XWORD fs, ir, hi, lo;
  bool  status = true;

  if(!autogain)
    {
      // autogain not requested. Executing a single sensor read

      status = GetData(fullspectrum, infrared);
      lux = CalculateLux(fullspectrum, infrared);

      return status;
    }

  // autogain requested
  switch(integrationtime)
    {
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_13MS   : hi = DIOI2CLIGHTSENSORTSL2561_AGC_THI_13MS;
                                                            lo = DIOI2CLIGHTSENSORTSL2561_AGC_TLO_13MS;
                                                            break;

      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS  : hi = DIOI2CLIGHTSENSORTSL2561_AGC_THI_101MS;
                                                            lo = DIOI2CLIGHTSENSORTSL2561_AGC_TLO_101MS;
                                                            break;

      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS  : hi = DIOI2CLIGHTSENSORTSL2561_AGC_THI_402MS;
                                                            lo = DIOI2CLIGHTSENSORTSL2561_AGC_TLO_402MS;
                                                            break;

                                              default     : hi = DIOI2CLIGHTSENSORTSL2561_AGC_THI_402MS;
                                                            lo = DIOI2CLIGHTSENSORTSL2561_AGC_TLO_402MS;
                                                            break;
    }

  // try to adjust the gain
  status = GetData(fs, ir);
  if(!status) return false;

  if((fs < lo) && (gain == DIOI2CLIGHTSENSORTSL2561GAIN_1X))
    {
      // light too low with this gain
      // raise the gain and redo the reading
      SetGain(DIOI2CLIGHTSENSORTSL2561GAIN_16X);

      status = GetData(fs, ir);
      if(!status) return false;

      // now consider the reading valid after being adjusted
      fullspectrum = fs;
      infrared     = ir;

      lux = CalculateLux(fullspectrum, infrared);

      return true;
    }


  if((fs > hi) && (gain == DIOI2CLIGHTSENSORTSL2561GAIN_16X))
    {
      // light too high with this gain
      // lower the gain and redo the reading
      SetGain(DIOI2CLIGHTSENSORTSL2561GAIN_1X);

      status = GetData(fs, ir);
      if(!status) return false;

      // now consider the reading valid after being adjusted
      fullspectrum = fs;
      infrared     = ir;
      lux = CalculateLux(fullspectrum, infrared);

      return true;
    }


  // the reading was valid without gain adjustment (or chip limits encountered!)
  fullspectrum = fs;
  infrared     = ir;

  lux = CalculateLux(fullspectrum, infrared);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::End()
{
  if(!DIODEVICEI2C::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::IniDevice()
{
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          bool status;
                    
          status = SetIntegrationTime(DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS);
          if(!status) return false;
          
          status = SetGain(DIOI2CLIGHTSENSORTSL2561GAIN_16X);
          if(!status) return false;
                    
          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::WriteReg(XBYTE reg, XBYTE value, int timeout)
* @brief      Write reg
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  value : Value value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::WriteReg(XBYTE reg, XBYTE value, int timeout)
{
  XBUFFER xbuffer;

  xbuffer.Add((XBYTE)(DIOI2CLIGHTSENSORTSL2561_COMMAND_BIT | reg));
  xbuffer.Add((XBYTE)value);

  if(!diostream->Write(xbuffer))                  return false;
  if(!diostream->WaitToFlushOutXBuffer(timeout))  return false;

  //XTRACE_PRINTDATABLOCKCOLOR(XDEBUG_COLORRED, xbuffer);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::ReadData16(XBYTE reg, XWORD& value, int timeout)
* @brief      Read data16
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  value : Value value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::ReadData16(XBYTE reg, XWORD& value, int timeout)
{
  XBUFFER xbuffer;

  value = 0;

  xbuffer.Add((XBYTE)reg);
  if(!diostream->Write(xbuffer))                              return false;
  if(!diostream->WaitToFlushOutXBuffer(timeout))              return false;

  xbuffer.Resize(2);
  if(!diostream->WaitToFilledReadingBuffer(2, timeout))       return false;
  if(!diostream->Read(xbuffer))                               return false;
  if(xbuffer.GetSize() < 2)                                   return false;

  value   = xbuffer.GetByte(1);
  value <<= 8;
  value  |= xbuffer.GetByte(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::GetData(XWORD& fullspectrum, XWORD& infrared)
* @brief      Get data
* @ingroup    DATAIO
* 
* @param[in]  fullspectrum : Fullspectrum value.
* @param[in]  infrared : Infrared value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::GetData(XWORD& fullspectrum, XWORD& infrared)
{
  fullspectrum = 0;
  infrared     = 0;

  if(!Activate()) return false;

  bool status;

  // wait for the internal ADC to complete conversion
  switch(integrationtime)
    {
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_13MS  : GEN_XSLEEP.MilliSeconds(20);  break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS : GEN_XSLEEP.MilliSeconds(150); break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS : GEN_XSLEEP.MilliSeconds(450); break;
    }

  status = ReadData16((DIOI2CLIGHTSENSORTSL2561_COMMAND_BIT | DIOI2CLIGHTSENSORTSL2561_WORD_BIT | DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN0_LOW), fullspectrum, timeout);
  if(status) status = ReadData16((DIOI2CLIGHTSENSORTSL2561_COMMAND_BIT | DIOI2CLIGHTSENSORTSL2561_WORD_BIT | DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN1_LOW), infrared, timeout);

  if(status) status = Deactivate();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::Activate()
* @brief      Activate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::Activate()
{
  return WriteReg(DIOI2CLIGHTSENSORTSL2561REGISTER_CONTROL, DIOI2CLIGHTSENSORTSL2561_CONTROL_POWERON, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2CLIGHTSENSORTSL2561::Deactivate()
* @brief      Deactivate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2CLIGHTSENSORTSL2561::Deactivate()
{
  return WriteReg(DIOI2CLIGHTSENSORTSL2561REGISTER_CONTROL, DIOI2CLIGHTSENSORTSL2561_CONTROL_POWEROFF, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOI2CLIGHTSENSORTSL2561::CalculateLux(XWORD broadband, XWORD inflared)
* @brief      Calculate lux
* @ingroup    DATAIO
* 
* @param[in]  broadband : Broadband value.
* @param[in]  inflared : Inflared value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOI2CLIGHTSENSORTSL2561::CalculateLux(XWORD broadband, XWORD inflared)
{
  XDWORD chscale;
  XDWORD channel1;
  XDWORD channel0;
  XDWORD clipthreshold;
  XDWORD ratio1 = 0;
  int    ratio;
  XDWORD b, m;
  int    temp;
  XDWORD lux;

  // Make sure the sensor isn't saturated!
  switch(integrationtime)
    {
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_13MS   : clipthreshold = DIOI2CLIGHTSENSORTSL2561_CLIPPING_13MS;   break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS  : clipthreshold = DIOI2CLIGHTSENSORTSL2561_CLIPPING_101MS;  break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS  : clipthreshold = DIOI2CLIGHTSENSORTSL2561_CLIPPING_402MS;  break;
                                              default     : clipthreshold = DIOI2CLIGHTSENSORTSL2561_CLIPPING_402MS;  break;
    }

  // return 0 lux if the sensor is saturated
  if((broadband > clipthreshold) || (inflared > clipthreshold)) return 0;


  // get the correct scale depending on the intergration time
  switch(integrationtime)
    {
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_13MS   : chscale = DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE_TINT0;   break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS  : chscale = DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE_TINT1;   break;
      case DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS  : chscale = (1 << DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE);  break;
                                                  default : chscale = (1 << DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE);  break;   // No scaling ... integration time = 402ms
  }

  // scale for gain (1x or 16x)
  if (!gain) chscale = chscale << 4;

  // scale the channel values
  channel0 = (broadband * chscale) >> DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE;
  channel1 = (inflared  * chscale) >> DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE;

  // find the ratio of the channel values (Channel1/Channel0)
  if (channel0 != 0) ratio1 = (channel1 << (DIOI2CLIGHTSENSORTSL2561_LUX_RATIOSCALE+1)) / channel0;

  // round the ratio value
  ratio = (ratio1 + 1) >> 1;

  #ifdef TSL2561_PACKAGE_CS
  if ((ratio >= 0) && (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K1C))
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B1C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M1C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K2C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B2C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M2C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K3C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B3C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M3C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K4C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B4C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M4C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K5C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B5C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M5C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K6C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B6C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M6C;  }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K7C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B7C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M7C;  }
   else if (ratio > DIOI2CLIGHTSENSORTSL2561_LUX_K8C)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B8C;  m = DIOI2CLIGHTSENSORTSL2561_LUX_M8C;  }
  #else
  if ((ratio >= 0) && (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K1T))
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B1T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M1T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K2T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B2T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M2T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K3T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B3T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M3T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K4T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B4T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M4T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K5T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B5T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M5T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K6T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B6T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M6T;   }
   else if (ratio <= DIOI2CLIGHTSENSORTSL2561_LUX_K7T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B7T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M7T;   }
   else if (ratio > DIOI2CLIGHTSENSORTSL2561_LUX_K8T)
    { b = DIOI2CLIGHTSENSORTSL2561_LUX_B8T; m = DIOI2CLIGHTSENSORTSL2561_LUX_M8T;   }
  #endif

  temp = ((channel0 * b) - (channel1 * m));

  // do not allow negative lux value
  if (temp < 0) temp = 0;

  // round lsb (2^(LUX_SCALE-1))
  temp += (1 << (DIOI2CLIGHTSENSORTSL2561_LUX_LUXSCALE-1));

  // strip off fractional portion
  lux = (temp >> DIOI2CLIGHTSENSORTSL2561_LUX_LUXSCALE);

  // Signal I2C had no errors
  return lux;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2CLIGHTSENSORTSL2561::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2CLIGHTSENSORTSL2561::Clean()
{
  integrationtime   = DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS;
  gain              = DIOI2CLIGHTSENSORTSL2561GAIN_16X;
}



