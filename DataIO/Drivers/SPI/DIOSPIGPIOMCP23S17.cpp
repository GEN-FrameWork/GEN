/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIGPIOMCP23S17.cpp
* 
* @class      DIOSPIGPIOMCP23S17
* @brief      Data Input/Output SPI GPIO Extender MCP23S17 (Microchip) class
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

#include "DIOSPIGPIOMCP23S17.h"

#include "XFactory.h"
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
* @fn         DIOSPIGPIOMCP23S17::DIOSPIGPIOMCP23S17() : DIODEVICESPI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIGPIOMCP23S17::DIOSPIGPIOMCP23S17() : DIODEVICESPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPIGPIOMCP23S17::~DIOSPIGPIOMCP23S17()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIGPIOMCP23S17::~DIOSPIGPIOMCP23S17()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::IniDevice()
{
  if(!isdiostreamSPIexternal)
    {
      diostreamcfg = GEN_NEW DIOSTREAMSPICONFIG();
      if(!diostreamcfg)  return false;

      diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);
      diostreamcfg->SetNBitsWord(8);
      diostreamcfg->SetSpeed(10*1000*1000);
      diostreamcfg->SetDelay(0);
      
      
      int chipselect = 0;
      diostreamcfg->GetLocalDeviceName()->Format(_L("/dev/spidev0.%d"), chipselect);

      diostream = (DIOSTREAMSPI*)GEN_DIOFACTORY.CreateStreamIO(diostreamcfg);
      if(!diostream) return false;
    }
   else
    {
      if(!diostream) return false;

      diostreamcfg = (DIOSTREAMSPICONFIG *)diostream->GetConfig();
      if(!diostreamcfg) return false;
    }

  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  if(!DIODEVICE::Ini()) return false;

  //return Configure();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::Configure()
* @brief      Configure
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Configure()
{
  XBYTE ioconfig =  DIOSPIGPIOMCP23S17_BANK_OFF       |         
                    DIOSPIGPIOMCP23S17_INT_MIRROR_OFF |         
                    DIOSPIGPIOMCP23S17_SEQOP_OFF      | 
                    DIOSPIGPIOMCP23S17_DISSLW_OFF     | 
                    DIOSPIGPIOMCP23S17_HAEN_ON        |
                    DIOSPIGPIOMCP23S17_ODR_OFF        | 
                    DIOSPIGPIOMCP23S17_INTPOL_LOW;

  XBYTE addr  = 0;

  if(!Write_Register(DIOSPIGPIOMCP23S17_IOCON, addr, ioconfig)) return false;

  // I/O direction
  if(!Write_Register(DIOSPIGPIOMCP23S17_IODIRA , addr, 0x00)) return false;
  if(!Write_Register(DIOSPIGPIOMCP23S17_IODIRB , addr, 0xFF)) return false;

  // GPIOB pull ups
  if(!Write_Register(DIOSPIGPIOMCP23S17_GPPUB  , addr, 0xFF)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::Read_Register(XBYTE reg, XBYTE addr, XBYTE& data)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  addr : Addr value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Read_Register(XBYTE reg, XBYTE addr, XBYTE& data)
{
  XBYTE control_byte                  = GetControlByte(DIOSPIGPIOMCP23S17_READ_CMD, addr);  
  XBYTE tx_buffer[3]                  = { control_byte, reg, 0 };
  XBYTE rx_buffer[sizeof(tx_buffer)];

  data = 0;

  bool status = diostream->TransferBuffer(rx_buffer, tx_buffer, sizeof(tx_buffer));

  if(status) data = rx_buffer[2];
    
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::Write_Register(XBYTE reg, XBYTE addr, XBYTE data)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  addr : Addr value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Write_Register(XBYTE reg, XBYTE addr, XBYTE data)
{
  XBYTE control_byte                   = GetControlByte(DIOSPIGPIOMCP23S17_WRITE_CMD, addr);  
  XBYTE tx_buffer[3]                   = { control_byte, reg, data };
  XBYTE rx_buffer[sizeof(tx_buffer)];

  return diostream->TransferBuffer(rx_buffer, tx_buffer, sizeof(tx_buffer));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::Read_Bit(XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE& data)
* @brief      Read bit
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  addr : Addr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Read_Bit(XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE& data)
{
  XBYTE _data    = 0;
  bool  status  =  Read_Register(reg, addr, _data);

  data = ((_data >> bitnum) & 0x01);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::Write_Bit(XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE data)
* @brief      Write bit
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  addr : Addr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Write_Bit(XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE data)
{
  XBYTE reg_data = 0;
  
  if(!Read_Register(reg, addr, reg_data)) return false;

  if(data) 
    {
      reg_data |= (1 << bitnum);          // set
    } 
   else 
    {
      reg_data &= 0xff ^ (1 << bitnum);   // clear
    }

  return Write_Register(reg, addr, reg_data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIGPIOMCP23S17::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream) diostream->Close();

  if(!isdiostreamSPIexternal)
    {
      if(diostream)
        {
          GEN_DIOFACTORY.DeleteStreamIO(diostream);
          diostream = NULL;
        }

      if(diostreamcfg)
        {
          GEN_DELETE diostreamcfg;
          diostreamcfg = NULL;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPI* DIOSPIGPIOMCP23S17::GetDIOStreamSPI()
* @brief      Get DIO stream SPI
* @ingroup    DATAIO
* 
* @return     DIOSTREAMSPI* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPI* DIOSPIGPIOMCP23S17::GetDIOStreamSPI()
{
  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPIGPIOMCP23S17::SetDIOStreamSPI(DIOSTREAMSPI* diostream)
* @brief      Set DIO stream SPI
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPIGPIOMCP23S17::SetDIOStreamSPI(DIOSTREAMSPI* diostream)
{
  this->diostream         = diostream;
  isdiostreamSPIexternal  = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSPIGPIOMCP23S17::GetControlByte(XBYTE rw_cmd, XBYTE addr)
* @brief      Get control byte
* @ingroup    DATAIO
* 
* @param[in]  rw_cmd : Rw cmd value.
* @param[in]  addr : Addr value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSPIGPIOMCP23S17::GetControlByte(XBYTE rw_cmd, XBYTE addr)
{
  addr     = (addr << 1) & 0xE;
  rw_cmd  &= 1; // just 1 bit long
  return 0x40 | addr | rw_cmd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPIGPIOMCP23S17::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPIGPIOMCP23S17::Clean()
{
  timeout                 = 0;

  diostreamcfg            = NULL;
  diostream               = NULL;

  isdiostreamSPIexternal  = false;
}



