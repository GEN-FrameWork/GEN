/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITFTDisplayILI9341.cpp
* 
* @class      DIOSPITFTDISPLAYILI9341
* @brief      Data Input/Output SPI Display TFT ILI9341 (Ilitek) class
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

#include "DIOSPITFTDisplayILI9341.h"

#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamSPI.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPITFTDISPLAYILI9341::DIOSPITFTDISPLAYILI9341()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITFTDISPLAYILI9341::DIOSPITFTDISPLAYILI9341()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPITFTDISPLAYILI9341::~DIOSPITFTDISPLAYILI9341()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITFTDISPLAYILI9341::~DIOSPITFTDISPLAYILI9341()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::IniDevice()
{
  if(!DIODEVICESPI::IniDevice()) return false;

  diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_3);
  diostreamcfg->SetNBitsWord(8);
  diostreamcfg->SetSpeed(80*1000*1000);
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
          if(!TFT_Init()) return false;

          width      = 320;
          height     = 240;

          sizebuffer = (width * height * sizeof(XWORD));

          Clear();

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::Clear(XWORD color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::Clear(XWORD color)
{
  int size = (GetSizeBuffer() / sizeof(XWORD));

  XWORD* buffer = GEN_NEW XWORD[size];
  if(!buffer) return false;

  for(int c=0; c<size; c++)
    {
      buffer[c] = color;
    }

  bool status = Update((XBYTE*)buffer);

  GEN_DELETE_ARRAY buffer;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;

  if(!TFT_SetWindow(0, 0, 319, 239)) return false;
  if(!SendCommand(0x2c)) return false;

  databuffer.Delete();
  databuffer.Add(buffer, GetSizeBuffer());

  return SendData();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::PutPixel(XWORD x, XWORD y, XWORD color)
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
bool DIOSPITFTDISPLAYILI9341::PutPixel(XWORD x, XWORD y, XWORD color)
{
  if(!IsInitialized()) return false;

  if(!TFT_SetWindow(x, y, x, y)) return false;
  if(!SendCommand(0x2c)) return false;

  databuffer.Delete();
  databuffer.Add((XWORD)color);

  return SendData();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::End()
{
  Clear();

  if(!DIODEVICESPI::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::TFT_Init()
* @brief      TFT init
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::TFT_Init()
{
  DIOSPITFTDISPLAYILI9341_DCHIGH;

  TFT_Reset();

  if(!SendCommand(0x01)) return false;
  GEN_XSLEEP.MilliSeconds(5);

  SendCommand(0x28);

  //----------------------------------------------------------------------------
  // 1
  /*
  WriteRegister(0xCF,  3, 0x00, 0x83, 0x30);
  WriteRegister(0xED,  4, 0x64, 0x03, 0x12, 0x81);
  WriteRegister(0xE8,  3, 0x85, 0x01, 0x79);
  WriteRegister(0xCB,  5, 0x39, 0x2C, 0x00, 0x34, 0x02);
  WriteRegister(0xF7,  1, 0x20);
  WriteRegister(0xEA,  2, 0x00, 0x00);
  WriteRegister(0xC0,  1, 0x26);
  WriteRegister(0xC1,  1, 0x11);
  WriteRegister(0xC5,  2, 0x35, 0x3E);
  WriteRegister(0xC7,  1, 0xBE);
  WriteRegister(0x3A,  1, 0x55);
  WriteRegister(0xB1,  2, 0x00, 0x1B);
  WriteRegister(0x26,  1, 0x01);
  WriteRegister(0xB7,  1, 0x07);
  WriteRegister(0xB6,  4, 0x0A, 0x82, 0x27, 0x00);
  */
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // 2
  /*
  WriteRegister(0xCB,  5, 0x39, 0x2C, 0x00, 0x34, 0x02);
  WriteRegister(0xCF,  3, 0x00, 0xC1, 0x30);
  WriteRegister(0xE8,  3, 0x85, 0x00, 0x78);
  WriteRegister(0xEA,  2, 0x00, 0x00);
  WriteRegister(0xED,  4, 0x64, 0x03, 0x12, 0x81);
  WriteRegister(0xF7,  1, 0x20);
  WriteRegister(0xC0,  1, 0x23);
  WriteRegister(0xC1,  1, 0x10);
  WriteRegister(0xC5,  2, 0x3e, 0x28);
  WriteRegister(0xC7,  1, 0x86);
  WriteRegister(0x36,  1, 0x48);
  WriteRegister(0x3A,  1, 0x55);
  WriteRegister(0xB1,  2, 0x00, 0x18);
  WriteRegister(0xB6,  3, 0x08, 0x82, 0x27);
  WriteRegister(0xF2,  1, 0x00);
  WriteRegister(0x26,  1, 0x01);
  WriteRegister(0xE0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);
  WriteRegister(0XE1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);
  */
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // 3

  WriteRegister(0xCD, 5, 0x39, 0x2C, 0x00, 0x34, 0x02);        // Power Control A
  WriteRegister(0xCF, 3, 0x00, 0xAA, 0xB0);                    // Power Control B
  WriteRegister(0xF7, 1, 0x30);                                // CMD_PUMP_RATIO_CONTROL
  WriteRegister(0xC0, 1, 0x25);                                // POWER_CONTROL_1
  WriteRegister(0xC1, 1, 0x11);                                // POWER_CONTROL_2
  WriteRegister(0xC5, 2, 0x5C, 0x4C);                          // VCOM_CONTROL_1
  WriteRegister(0xC7, 1, 0x94);                                // VCOM_CONTROL_2
  WriteRegister(0xE8, 3, 0x85, 0x01, 0x78);                    // DRIVER_TIMING_CONTROL_A
  WriteRegister(0xEA, 2, 0x00, 0x00);                          // DRIVER_TIMING_CONTROL_B
  WriteRegister(0x3A, 1, 0x05);                                // COLMOD_PIXEL_FORMAT_SET
  WriteRegister(0x36, 1, 0x48);                                // MEMORY_ACCESS_CONTROL

  //----------------------------------------------------------------------------

  if(!TFT_RotateRGB(90, 2))          return false;

  SendCommand(0x11);                // Sleep out
  GEN_XSLEEP.MilliSeconds(100);

  SendCommand(0x29);                // display on
  GEN_XSLEEP.MilliSeconds(20);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::TFT_Reset()
* @brief      TFT reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::TFT_Reset()
{
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) == DIOGPIO_ID_NOTDEFINED) return false;
  
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), false);
  GEN_XSLEEP.MilliSeconds(100);

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);
  GEN_XSLEEP.MilliSeconds(100);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::TFT_RotateRGB(int rotate, XBYTE bgr)
* @brief      TFT rotate RGB
* @ingroup    DATAIO
* 
* @param[in]  rotate : Rotate value.
* @param[in]  bgr : Bgr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::TFT_RotateRGB(int rotate, XBYTE bgr)
{
  #define MEM_Y   (7) /* MY row address order */
  #define MEM_X   (6) /* MX column address order */
  #define MEM_V   (5) /* MV row / column exchange */
  #define MEM_L   (4) /* ML vertical refresh order */
  #define MEM_H   (2) /* MH horizontal refresh order */
  #define MEM_BGR (3) /* RGB-BGR Order */

  bool status = false;

  switch(rotate)
    {
      case    0 : status = WriteRegister(0x36, 1, (1 << MEM_X) | (bgr << MEM_BGR));                               break;
      case   90 : status = WriteRegister(0x36, 1, (1 << MEM_Y) | (1 << MEM_X) |(1 << MEM_V) | (bgr << MEM_BGR));  break;
      case  180 : status = WriteRegister(0x36, 1, (1 << MEM_Y) | (bgr << MEM_BGR));                               break;
      case  270 : status = WriteRegister(0x36, 1, (1 << MEM_V) | (1 << MEM_L) | (bgr << MEM_BGR));                break;
      default   : break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::TFT_SetWindow(int xs, int ys, int xe, int ye)
* @brief      TFT set window
* @ingroup    DATAIO
* 
* @param[in]  xs : Xs value.
* @param[in]  ys : Ys value.
* @param[in]  xe : Xe value.
* @param[in]  ye : Ye value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::TFT_SetWindow(int xs, int ys, int xe, int ye)
{
  bool status;

  /* Column address set */
  status = WriteRegister(0x2A, 4, (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF);

  /* Row adress set */
  if(status) status = WriteRegister(0x2B, 4, (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::SendCommand(XBYTE command)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::SendCommand(XBYTE command)
{
  bool status;

  DIOSPITFTDISPLAYILI9341_DCLOW;

  status = diostream->Write(&command, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::SendData()
* @brief      Send data
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::SendData()
{
  bool status;

  DIOSPITFTDISPLAYILI9341_DCHIGH;

  status = diostream->Write(databuffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  DIOSPITFTDISPLAYILI9341_DCLOW;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYILI9341::WriteRegister(XBYTE reg, int ndata, ...)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* @param[in]  ndata : Ndata value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYILI9341::WriteRegister(XBYTE reg, int ndata, ...)
{
  va_list arg;

  va_start(arg, ndata);

  SendCommand(reg);

  databuffer.Delete();

  for(int c=0; c<ndata; c++)
    {
      int data = (int)va_arg(arg, int);
      databuffer.Add((XBYTE)data);
    }

  SendData();

  va_end(arg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPITFTDISPLAYILI9341::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPITFTDISPLAYILI9341::Clean()
{
 
}



