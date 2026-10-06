/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITouchScreenSTMPE610.cpp
* 
* @class      DIOSPITOUCHSCREENSTMPE610
* @brief      Data Input/Output SPI STMPE610 (Touch Screen) class
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

#include "DIOSPITouchScreenSTMPE610.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPITOUCHSCREENSTMPE610::DIOSPITOUCHSCREENSTMPE610(): DIODEVICE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITOUCHSCREENSTMPE610::DIOSPITOUCHSCREENSTMPE610(): DIODEVICE()
{
  Clean();

  xtimer=GEN_XFACTORY.CreateTimer();
  
  databuffer = GEN_NEW XBUFFER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPITOUCHSCREENSTMPE610::~DIOSPITOUCHSCREENSTMPE610()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITOUCHSCREENSTMPE610::~DIOSPITOUCHSCREENSTMPE610()
{
  End();

  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
      xtimer = NULL;
    }

  if(databuffer)
    {
      GEN_DELETE databuffer;
      databuffer = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::IniDevice(int chipselect, int timeout)
* @brief      Ini device
* @ingroup    DATAIO
* 
* @param[in]  chipselect : Chipselect value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::IniDevice(int chipselect, int timeout)
{
  if(!isdiostreamSPIexternal)
    {
      diostreamSPIcfg = GEN_NEW DIOSTREAMSPICONFIG();
      if(!diostreamSPIcfg)  return false;

      diostreamSPIcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);
      diostreamSPIcfg->SetNBitsWord(8);
      diostreamSPIcfg->SetSpeed(1*1000*1000);
      diostreamSPIcfg->SetDelay(0);
      diostreamSPIcfg->SetIsOnlyWrite(false);
      diostreamSPIcfg->GetLocalDeviceName()->Format(_L("/dev/spidev0.%d"), chipselect);

      diostreamSPI = (DIOSTREAMSPI*)GEN_DIOFACTORY.CreateStreamIO(diostreamSPIcfg);
      if(!diostreamSPI) return false;
    }
   else
    {
      if(!diostreamSPI) return false;

      diostreamSPIcfg = (DIOSTREAMSPICONFIG *)diostreamSPI->GetConfig();
      if(!diostreamSPIcfg) return false;
    }

  this->timeout = timeout;

  if(!diostreamSPI->Open())  return false;

  if(!diostreamSPI->WaitToConnected(timeout)) return false;

  if(!TouchScreen_Ini()) return false;

  return DIODEVICE::Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSPITOUCHSCREENSTMPE610::GetVersion()
* @brief      Get version
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSPITOUCHSCREENSTMPE610::GetVersion()
{
  if(!IsInitialized()) return false;

  XWORD version = 0;
  XBYTE data[2] = { 0, 0 };

  if(!ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID1, data[0])) return 0;
  if(!ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID2, data[1])) return 0;

  version = data[0];
  version <<= 8;
  version |= data[1];

  return version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::IsTouched()
* @brief      Is touched
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::IsTouched()
{
  if(!IsInitialized()) return false;

  XBYTE data = 0;

  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CTRL, data);
  if(!status) return false;

  if(data & 0x80) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::IsBufferEmpty()
* @brief      Is buffer empty
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::IsBufferEmpty()
{
  if(!IsInitialized()) return false;

  XBYTE data = 0;

  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA, data);
  if(!status) return false;

  if(data & DIOSPITOUCHSCREENSTMPE610_FIFO_STA_EMPTY) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSPITOUCHSCREENSTMPE610::GetBufferSize()
* @brief      Get buffer size
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSPITOUCHSCREENSTMPE610::GetBufferSize()
{
  if(!IsInitialized()) return false;

  XBYTE data;
  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_SIZE, data);
  if(!status) return 0;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::ReadPosition(XWORD& x, XWORD& y, XBYTE& z)
* @brief      Read position
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::ReadPosition(XWORD& x, XWORD& y, XBYTE& z)
{
  if(!IsInitialized()) return false;

  /*
  XBYTE data[4];

  for(XBYTE c=0; c<4; c++)
    {
      if(!ReadRegister(0xD7, data[c]))  return false;
    }

  XDWORD xyz = (XDWORD) ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0));

  x = (XWORD)((xyz >> 20) & 0x00000FFF);
  y = (XWORD)((xyz >> 8)  & 0x00000FFF);
  z = (XBYTE)((xyz >> 0)  & 0x0000000F);

  if(IsBufferEmpty()) WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA, 0xFF);
  */

  XBYTE data[4];

  for(XBYTE c=0; c<4; c++)
    {
      if(!ReadRegister(0xD7, data[c]))  return false;
    }

  x   = data[0];
  x <<= 4;
  x  |= (data[1] >> 4);

  y   = data[1] & 0x0F;
  y <<= 8;
  y  |= data[2];

  z   = data[3];

  if(IsBufferEmpty()) WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA, 0xFF);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostreamSPI) diostreamSPI->Close();

  if(!isdiostreamSPIexternal)
    {
      if(diostreamSPI)
        {
          GEN_DIOFACTORY.DeleteStreamIO(diostreamSPI);
          diostreamSPI = NULL;
        }

      if(diostreamSPIcfg)
        {
          GEN_DELETE diostreamSPIcfg;
          diostreamSPIcfg = NULL;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPI* DIOSPITOUCHSCREENSTMPE610::GetDIOStreamSPI()
* @brief      Get DIO stream SPI
* @ingroup    DATAIO
* 
* @return     DIOSTREAMSPI* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPI* DIOSPITOUCHSCREENSTMPE610::GetDIOStreamSPI()
{
  return diostreamSPI;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPITOUCHSCREENSTMPE610::SetDIOStreamSPI(DIOSTREAMSPI* diostreamSPI)
* @brief      Set DIO stream SPI
* @ingroup    DATAIO
* 
* @param[in]  diostreamSPI : Diostream SPI pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPITOUCHSCREENSTMPE610::SetDIOStreamSPI(DIOSTREAMSPI* diostreamSPI)
{
  this->diostreamSPI     = diostreamSPI;
  isdiostreamSPIexternal = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::TouchScreen_Ini()
* @brief      Touch screen ini
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::TouchScreen_Ini()
{
  if(!WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL1, DIOSPITOUCHSCREENSTMPE610_SYS_CTRL1_RESET)) return false;

  GEN_XSLEEP.MilliSeconds(250);

  for(XBYTE c=0; c<65; c++)
    {
      XBYTE data;
      ReadRegister(c, data);
    }

  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL2       , 0x0);                                                                             // turn on clocks!
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CTRL, DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_XYZ | DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_EN);  // XYZ and enable!
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_EN, DIOSPITOUCHSCREENSTMPE610_INT_EN_TOUCHDET);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL1, DIOSPITOUCHSCREENSTMPE610_ADC_CTRL1_10BIT | (0x6 << 4));                          // 96 clocks per conversion
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL2, DIOSPITOUCHSCREENSTMPE610_ADC_CTRL2_6_5MHZ);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CFG, DIOSPITOUCHSCREENSTMPE610_TSC_CFG_4SAMPLE | DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_1MS | DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_5MS);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_FRACT_XYZ   , 0x6);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_TH         , 1);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA        , DIOSPITOUCHSCREENSTMPE610_FIFO_STA_RESET);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA        , 0);                                                                               // unreset
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_I_DRIVE, DIOSPITOUCHSCREENSTMPE610_TSC_I_DRIVE_50MA);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA         , 0xFF);                                                                            // reset all ints
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_CTRL, DIOSPITOUCHSCREENSTMPE610_INT_CTRL_POL_HIGH | DIOSPITOUCHSCREENSTMPE610_INT_CTRL_ENABLE);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::ReadRegister(XBYTE reg, XBYTE& data)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::ReadRegister(XBYTE reg, XBYTE& data)
{
  //diostreamSPI->GetInXBuffer()->Delete();

  XBYTE regdata[2] = { (XBYTE)(reg | 0x80), 0 };
  XBYTE _data[2]   = { 0, 0 };

  bool status = diostreamSPI->Write(regdata, 2)?true:false;
  //if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);

  if(status) status = diostreamSPI->WaitToFilledReadingBuffer(2, timeout);
  if(status) status = diostreamSPI->Read(_data, 2)?true:false;

  data = _data[1];

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITOUCHSCREENSTMPE610::WriteRegister(XBYTE reg, XBYTE data)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::WriteRegister(XBYTE reg, XBYTE data)
{
  if(!databuffer) return false;

  bool status = diostreamSPI->Write(&reg, 1)?true:false;
  if(status) status = diostreamSPI->Write(&data, 1)?true:false;
  if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);
  if(status) status = diostreamSPI->WaitToFilledReadingBuffer(2, timeout);

  diostreamSPI->GetInXBuffer()->Delete();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPITOUCHSCREENSTMPE610::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPITOUCHSCREENSTMPE610::Clean()
{
  timeout                 = 0;

  xtimer                  = NULL;
  databuffer              = NULL;

  diostreamSPIcfg         = NULL;
  diostreamSPI            = NULL;
  isdiostreamSPIexternal  = false;
}



