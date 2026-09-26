/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2COLEDDisplaySSD1306.cpp
* 
* @class      DIOI2COLEDDISPLAYSSD1306
* @brief      Data Input/Output I2C OLED Display SSD1306 class 
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

#include "DIOI2COLEDDisplaySSD1306.h"

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
* @fn         DIOI2COLEDDISPLAYSSD1306::DIOI2COLEDDISPLAYSSD1306(XDWORD width, XDWORD height): DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  width : Width value.
* 
* @param[in]  height : Display height in pixels.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2COLEDDISPLAYSSD1306::DIOI2COLEDDISPLAYSSD1306( XDWORD width, XDWORD height): DIODEVICEI2C()
{
  Clean();

  this->width   = width;
  this->height  = height;

  sizebuffer    = ((width/8) * height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2COLEDDISPLAYSSD1306::~DIOI2COLEDDISPLAYSSD1306()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2COLEDDISPLAYSSD1306::~DIOI2COLEDDISPLAYSSD1306()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SetBuffer(XBYTE* buffer)
* @brief      Set buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SetBuffer(XBYTE* buffer)
{
  if(!buffer) return false;

  this->buffer = buffer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::IniDevice()
{
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFF);                            // 0xAE
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_CLOCK_DIV_RATIO_SET);            // 0xD5
          SendCommand(0xF0);                                                                    // Suggest ratio
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_MULTIPLEX_RADIO_SET);                    // 0xA8
          SendCommand((64 - 1));                                                                // Display_Height - 1

          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFFSET_SET);                     // 0xD3 + No offset
          SendCommand(00);                                                                      // No offset
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_CHARGE_PUMP_SET);                        // 0x8D
          SendCommand(0x14);                                                                    // Enable charge pump
          SendCommand((DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_START_LINE_SET | 0x00));        // 0x40 | Start line
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_MEMORY_ADDRESSING_SET);                  // 0x20
          SendCommand(0x00);                                                                    // Horizontal Addressing - Operate like KS0108
          SendCommand((DIOI2COLEDDISPLAYSSD1306_COMMAND_SEGMENT_REMAP_SET | 0x01));             // 0xA0 - Left towards Right
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_COM_OUTPUT_SCAN_DEC);                    // 0xC8 - Up towards Down
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_COM_PINS_SET);                           // 0xDA
        //SendCommand(0x12);                                                                    // Sequential COM pin configuration

          if((width == 128) && (height == 64)) SendCommand(0x12);                               // Sequential COM pin configuration
          if((width == 128) && (height == 32)) SendCommand(0x02);                               // Sequential COM pin configuration
          if((width ==  96) && (height == 16)) SendCommand(0x02);                               // Sequential COM pin configuration

          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_CONSTRAST_SET);                          // 0x81
          SendCommand(0xFF);                                                                    // 0xFF
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_PRECHARGE_PERIOD_SET);                   // 0xD9
          SendCommand(0xF1);                                                                    // 0xF1

          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_VCOMH_DESELECT_LEVEL_SET);               // 0xDB
          SendCommand(0x20);                                                                    // 0x20

          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_ALL_ON_RESUME);                  // 0xA4
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_NORMAL);                         // 0xA6
          SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_SCROLL_DEACTIVATE);                      // 0x2E

        //SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_ON);                             // 0xAF

        //Activate(false);

          Clear(0x00);

          Activate(true);

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::Clear(XBYTE color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::Clear(XBYTE color)
{
  XBYTE nblocks;

  nblocks = (width >> 4) * (height >> 3);

  // Set columns
  SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_COLUMN_ADDRESS_SET);     // 0x21
  SendCommand(0x00);                                                    // Start
  SendCommand(width - 1);                                               // End

  //Set rows
  SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESS_SET);       // 0x22
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
* @fn         bool DIOI2COLEDDISPLAYSSD1306::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::Update(XBYTE* buffer)
{
  this->buffer = buffer;

  XBYTE nblocks;

  nblocks = (width >> 4) * (height >> 3);

  // Set columns
  SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_COLUMN_ADDRESS_SET);     // 0x21
  SendCommand(0x00);                                                    // Start
  SendCommand(width - 1);                                               // End

  //Set rows
  SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESS_SET);       // 0x22
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
* @fn         bool DIOI2COLEDDISPLAYSSD1306::PutPixel(XWORD x, XWORD y, bool color)
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
bool DIOI2COLEDDISPLAYSSD1306::PutPixel(XWORD x, XWORD y, bool color)
{
  if(!buffer) return false;

  // Goto to point
  SetXY((XBYTE)x, (XBYTE)y);

  XBYTE data = 0;

  // Read data
  data = buffer[DIOI2COLEDDISPLAYSSD1306_POINTER(x, y)];

  //Set pixel
  if(color)
       DIOI2COLEDDISPLAYSSD1306_BITSET(data, y % 8);
  else DIOI2COLEDDISPLAYSSD1306_BITCLEAR(data, y % 8);

  //Sent data
  buffer[DIOI2COLEDDISPLAYSSD1306_POINTER(x, y)] = data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::End()
{
  Activate(false);

  if(!DIODEVICEI2C::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::Activate(bool active)
* @brief      Activate
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::Activate(bool active)
{
  return SendCommand(active ? DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_ON:DIOI2COLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2COLEDDISPLAYSSD1306::SetContrast(XBYTE contrast)
* @brief      Set contrast
* @ingroup    DATAIO
* 
* @param[in]  contrast : Contrast value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2COLEDDISPLAYSSD1306::SetContrast(XBYTE contrast)
{
  SendCommand(DIOI2COLEDDISPLAYSSD1306_COMMAND_CONSTRAST_SET);
  SendCommand(contrast);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2COLEDDISPLAYSSD1306::GetX()
* @brief      Get x
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2COLEDDISPLAYSSD1306::GetX()
{
  return x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2COLEDDISPLAYSSD1306::GetY()
* @brief      Get y
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2COLEDDISPLAYSSD1306::GetY()
{
  return y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SetX(XBYTE x)
* @brief      Set x
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SetX(XBYTE x)
{
  if(x >= width) return false;

  this->x = x;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SetY(XBYTE y)
* @brief      Set y
* @ingroup    DATAIO
* 
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SetY(XBYTE y)
{
  if(y >= height) return false;

  this->y = y;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SetXY(XBYTE x, XBYTE y)
* @brief      Set XY
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SetXY(XBYTE x, XBYTE y)
{
  if(!SetX(x)) return false;
  if(!SetY(y)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SendCommand(XBYTE command)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SendCommand(XBYTE command)
{
  return Send((0<<DIOI2COLEDDISPLAYSSD1306_DC), &command, 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::SendData(XBYTE* data, int size)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::SendData(XBYTE* data, int size)
{
  return Send((1<<DIOI2COLEDDISPLAYSSD1306_DC), data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2COLEDDISPLAYSSD1306::Send(XBYTE control, XBYTE* data, int sizedata)
* @brief      Send
* @ingroup    DATAIO
* 
* @param[in]  control : Control value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizedata : Sizedata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2COLEDDISPLAYSSD1306::Send(XBYTE control, XBYTE* data, int sizedata)
{
  XBUFFER xbuffer;

  if(!diostream) return false;
  
  xbuffer.Add(control);
  xbuffer.Add(data, sizedata);

  if(!diostream->Write(xbuffer))                 return false;
  if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2COLEDDISPLAYSSD1306::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2COLEDDISPLAYSSD1306::Clean()
{
  x             = 0;
  y             = 0;

  buffer        = NULL;
}



