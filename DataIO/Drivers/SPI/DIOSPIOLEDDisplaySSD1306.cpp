/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIOLEDDisplaySSD1306.cpp
* 
* @class      DIOSPIOLEDDISPLAYSSD1306
* @brief      Data Input/Output SPI Display OLED SSD 1306 class
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

#include "DIOSPIOLEDDisplaySSD1306.h"

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
* @fn         DIOSPIOLEDDISPLAYSSD1306::DIOSPIOLEDDISPLAYSSD1306(XDWORD width, XDWORD height)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIOLEDDISPLAYSSD1306::DIOSPIOLEDDISPLAYSSD1306(XDWORD width, XDWORD height)
{
  Clean();

  this->width   = width;
  this->height  = height;

  sizebuffer    = ((width/8) * height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPIOLEDDISPLAYSSD1306::~DIOSPIOLEDDISPLAYSSD1306()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIOLEDDISPLAYSSD1306::~DIOSPIOLEDDISPLAYSSD1306()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::IniDevice()
{
  if(!DIODEVICESPI::IniDevice()) return false;

  diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_3);
  diostreamcfg->SetNBitsWord(8);
  diostreamcfg->SetSpeed(16*1000*1000);
  diostreamcfg->SetDelay(0);
  diostreamcfg->SetIsOnlyWrite(true);
 
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) != DIOGPIO_ID_NOTDEFINED)
    {
      GEN_DIOGPIO.SetMode(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), DIOGPIO_MODE_OUTPUT);
      GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) , true);
    }

  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC) != DIOGPIO_ID_NOTDEFINED)
    {
      GEN_DIOGPIO.SetMode(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), DIOGPIO_MODE_OUTPUT);
      GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true);
    }

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          Reset();

          if(!Initializate())  return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Clear(XBYTE color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Clear(XBYTE color)
{
  XBYTE nblocks;

  nblocks = (width >> 4) * (height >> 3);

  // Set columns
  SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_COLUMN_ADDRESS_SET);     // 0x21
  SendCommand(0x00);                                                    // Start
  SendCommand(width - 1);                                               // End

  //Set rows
  SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESS_SET);       // 0x22
  SendCommand(0x00);                                                    // Start
  SendCommand(height - 1);                                              // End

  XBYTE buffer[16];

  memset(buffer, color, 16);

  //Send buffer
  for(XBYTE c=0; c<nblocks; c++)
    {
      SendData(buffer, 16);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Update(XBYTE* buffer)
{  
  XBYTE nblocks;

  nblocks = (width >> 4) * (height >> 3);

  // Set columns
  SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_COLUMN_ADDRESS_SET);      // 0x21
  SendCommand(0x00);                                                    // Start
  SendCommand(width - 1);                                               // End

  //Set rows
  SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESS_SET);        // 0x22
  SendCommand(0x00);                                                    // Start
  SendCommand(height - 1);                                              // End

  for(XBYTE c=0; c<nblocks; c++)
    {
      SendData(buffer+(c*16), 16);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::PutPixel(XWORD x, XWORD y, bool color)
* @brief      Put pixel
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::PutPixel(XWORD x, XWORD y, bool color)
{
  XBYTE* buffer = NULL;

  // Goto to point
  SetXY((XBYTE)x, (XBYTE)y);

  XBYTE data = 0;

  // Read data
  data = buffer[DIOSPIOLEDDISPLAYSSD1306_POINTER(x, y)];

  //Set pixel
  if(color)
       DIOSPIOLEDDISPLAYSSD1306_BITSET(data, y % 8);
  else DIOSPIOLEDDISPLAYSSD1306_BITCLEAR(data, y % 8);

  //Sent data
  buffer[DIOSPIOLEDDISPLAYSSD1306_POINTER(x, y)] = data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::End()
{
  Clear(0x00);

  if(!DIODEVICESPI::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Initializate()
* @brief      Initializate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Initializate()
{
  Reset();

  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFF))                          return false;     // 0xAE
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_CLOCK_DIV_RATIO_SET))          return false;     // 0xD5
  if(!SendCommand(0xF0))                                                                  return false;     // Suggest ratio
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_MULTIPLEX_RADIO_SET))                  return false;     // 0xA8
  if(!SendCommand((64 - 1)))                                                              return false;     // Display_Height - 1

  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFFSET_SET))                   return false;     // 0xD3 + No offset
  if(!SendCommand(00))                                                                    return false;     // No offset
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_CHARGE_PUMP_SET))                      return false;     // 0x8D
  if(!SendCommand(0x14))                                                                  return false;     // Enable charge pump
  if(!SendCommand((DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_START_LINE_SET | 0x00)))      return false;     // 0x40 | Start line
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_MEMORY_ADDRESSING_SET))                return false;     // 0x20
  if(!SendCommand(0x00))                                                                  return false;     // Horizontal Addressing - Operate like KS0108
  if(!SendCommand((DIOSPIOLEDDISPLAYSSD1306_COMMAND_SEGMENT_REMAP_SET | 0x01)))           return false;     // 0xA0 - Left towards Right
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_COM_OUTPUT_SCAN_DEC))                  return false;     // 0xC8 - Up towards Down
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_COM_PINS_SET))                         return false;     // 0xDA
//if(!SendCommand(0x12))                                                                  return false;     // Sequential COM pin configuration

  if((width == 128) && (height == 64)) 
    {
      if(!SendCommand(0x12))                             return false;     // Sequential COM pin configuration
    }
  if((width == 128) && (height == 32)) 
    {
      if(!SendCommand(0x02))                             return false;     // Sequential COM pin configuration
    }
  if((width ==  96) && (height == 16)) 
    {
      if(!SendCommand(0x02))                             return false;     // Sequential COM pin configuration
    }

  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_CONSTRAST_SET))                        return false;     // 0x81
  if(!SendCommand(0xFF))                                                                  return false;     // 0xFF
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_PRECHARGE_PERIOD_SET))                 return false;     // 0xD9
  if(!SendCommand(0xF1))                                                                  return false;     // 0xF1

  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_VCOMH_DESELECT_LEVEL_SET))             return false;     // 0xDB
  if(!SendCommand(0x20))                                                                  return false;     // 0x20

  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ALL_ON_RESUME))                return false;     // 0xA4
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_NORMAL))                       return false;     // 0xA6
  if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_DEACTIVATE))                    return false;     // 0x2E

//if(!SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ON))                           return false;     // 0xAF

//Activate(false);

  Clear(0x00);

  Activate(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Reset()
{
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) == DIOGPIO_ID_NOTDEFINED)   return false;
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC)    == DIOGPIO_ID_NOTDEFINED)   return false;
            
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);
  GEN_XSLEEP.MilliSeconds(10); 
  
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), false);
  GEN_XSLEEP.MilliSeconds(10);

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);
  GEN_XSLEEP.MilliSeconds(10);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Activate(bool active)
* @brief      Activate
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Activate(bool active)
{
  return SendCommand(active ? DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ON:DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPIOLEDDISPLAYSSD1306::SetContrast(XBYTE contrast)
* @brief      Set contrast
* @ingroup    DATAIO
* 
* @param[in]  contrast : Contrast value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPIOLEDDISPLAYSSD1306::SetContrast(XBYTE contrast)
{
  SendCommand(DIOSPIOLEDDISPLAYSSD1306_COMMAND_CONSTRAST_SET);
  SendCommand(contrast);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSPIOLEDDISPLAYSSD1306::GetX()
* @brief      Get x
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSPIOLEDDISPLAYSSD1306::GetX()
{
  return x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSPIOLEDDISPLAYSSD1306::GetY()
* @brief      Get y
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSPIOLEDDISPLAYSSD1306::GetY()
{
  return y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::SetX(XBYTE x)
* @brief      Set x
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::SetX(XBYTE x)
{
  if(x >= width) return false;

  this->x = x;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::SetY(XBYTE y)
* @brief      Set y
* @ingroup    DATAIO
* 
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::SetY(XBYTE y)
{
  if(y >= height) return false;

  this->y = y;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::SetXY(XBYTE x, XBYTE y)
* @brief      Set XY
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::SetXY(XBYTE x, XBYTE y)
{
  if(!SetX(x)) return false;
  if(!SetY(y)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::SendCommand(XBYTE command)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::SendCommand(XBYTE command)
{
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false); 

  return Send(&command, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::SendData(XBYTE* data, int size)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::SendData(XBYTE* data, int size)
{
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true); 

  return Send(data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1306::Send(XBYTE* data, int size)
* @brief      Send
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1306::Send(XBYTE* data, int size)
{  
  if(!diostream->Write(data, size))              return false;
  if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPIOLEDDISPLAYSSD1306::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPIOLEDDISPLAYSSD1306::Clean()
{ 
  x             = 0;
  y             = 0;  
}



