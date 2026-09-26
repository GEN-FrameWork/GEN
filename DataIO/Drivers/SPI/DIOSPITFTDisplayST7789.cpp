/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITFTDisplayST7789.cpp
* 
* @class      DIOSPITFTDISPLAYST7789
* @brief      Data Input/Output SPI Display TFT ST7789 class
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

#include "DIOSPITFTDisplayST7789.h"

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
* @fn         DIOSPITFTDISPLAYST7789::DIOSPITFTDISPLAYST7789(XDWORD width, XDWORD height)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITFTDISPLAYST7789::DIOSPITFTDISPLAYST7789(XDWORD width, XDWORD height)
{
  Clean();

  this->width   = width;
  this->height  = height;

  sizebuffer    = (width * height)* sizeof(XWORD);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPITFTDISPLAYST7789::~DIOSPITFTDISPLAYST7789()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPITFTDISPLAYST7789::~DIOSPITFTDISPLAYST7789()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::IniDevice()
{
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC) != DIOGPIO_ID_NOTDEFINED)
    {
      GEN_DIOGPIO.SetMode(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), DIOGPIO_MODE_OUTPUT);      
      GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);
      
    }

  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT) != DIOGPIO_ID_NOTDEFINED)
    {
      GEN_DIOGPIO.SetMode(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT), DIOGPIO_MODE_OUTPUT);
      GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT) , false);
    }

  if(!DIODEVICESPI::IniDevice()) return false;

  diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_3);
  diostreamcfg->SetNBitsWord(8);
  diostreamcfg->SetSpeed(16*1000*1000);
  diostreamcfg->SetDelay(0);
  diostreamcfg->SetIsOnlyWrite(false);
 
  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {     
          if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) != DIOGPIO_ID_NOTDEFINED)
            {
              GEN_DIOGPIO.SetMode(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), DIOGPIO_MODE_OUTPUT);

              GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);    GEN_XSLEEP.MilliSeconds(50);
              GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), false);   GEN_XSLEEP.MilliSeconds(50);
              GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);    GEN_XSLEEP.MilliSeconds(50);    
            }              

          SetActiveBlackLight(true);

          if(!TFT_Init()) return false;

          return DIODEVICE::Ini();
        }
    }


  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::Clear(XWORD color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::Clear(XWORD color)
{
  int size = GetSizeBuffer();

  if(!TFT_SetWindow(0, 0, width, height)) return false;

  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_RAMWR);  // write to RAM

  XBYTE chigh = (XBYTE)(color >> 8);
  XBYTE clow  = (XBYTE)(color &  0x00FF);

  canvasbuffer.Delete();
  canvasbuffer.Resize(size);
  canvasbuffer.ResetPosition();

  int c = 0;
 
  do{ canvasbuffer.Set(chigh);  c++;
      canvasbuffer.Set(clow);   c++;

    } while(c < size);

  return SendData();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;

  if(!TFT_SetWindow(0, 0, width, height)) return false;

  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_RAMWR);  // write to RAM

  canvasbuffer.Delete();
  canvasbuffer.Add(buffer, GetSizeBuffer());

  return SendData();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::PutPixel(XWORD x, XWORD y, XWORD color)
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
bool DIOSPITFTDISPLAYST7789::PutPixel(XWORD x, XWORD y, XWORD color)
{
  if(!IsInitialized()) return false;

  if(!TFT_SetWindow(x, y, x+1, y+1)) return false;

  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_RAMWR);  // write to RAM

  return SendData(SwapWORD(color));  
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::End()
{
  SetActiveBlackLight(false);

  if(!DIODEVICESPI::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SetActiveBlackLight(bool active)
* @brief      Set active black light
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SetActiveBlackLight(bool active)
{
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT) == DIOGPIO_ID_NOTDEFINED) return false;
  
  return GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT), active);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::TFT_Init()
* @brief      TFT init
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::TFT_Init()
{
  _colstart  = 0;
  _rowstart  = 0;
  _xstart    = 0;
  _ystart    = 0;

  //TFT_Reset();

  if(!SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_SWRESET)) return false;                         // Software reset
  GEN_XSLEEP.MilliSeconds(150);

  if(!SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_SLPOUT))  return false;                         // Out of sleep mode
  GEN_XSLEEP.MilliSeconds(500);

  if(!SendCommandParams(DIOSPITFTDISPLAYST7789_CMD_ST7789_COLMOD, 1, 0x55)) return false;          // Set color mode   0x55 (16-bit color)
  GEN_XSLEEP.MilliSeconds(10);

  if(!SendCommandParams(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL  , 1, 0x00)) return false;         // Memory access ctrl (directions) 0x00 Row addr/col addr, bottom to top refresh

  if(!SendCommandParams(DIOSPITFTDISPLAYST7789_CMD_ST7789_CASET   , 4                               // Column addr set
                                                                  , 0x00                            // XSTART = 0
                                                                  , DIOSPITFTDISPLAYST7789_240x240_XSTART
                                                                  , (DIOSPITFTDISPLAYST7789_TFTWIDTH + DIOSPITFTDISPLAYST7789_240x240_XSTART) >> 8
                                                                  , (DIOSPITFTDISPLAYST7789_TFTWIDTH + DIOSPITFTDISPLAYST7789_240x240_XSTART) & 0xFF)) return false;   // XEND = 240
  
  if(!SendCommandParams(DIOSPITFTDISPLAYST7789_CMD_ST7789_RASET   , 4                               // Row addr set
                                                                  , 0x00                            // YSTART = 0
                                                                  , DIOSPITFTDISPLAYST7789_240x240_YSTART
                                                                  , (DIOSPITFTDISPLAYST7789_TFTHEIGHT + DIOSPITFTDISPLAYST7789_240x240_YSTART) >> 8
                                                                  , (DIOSPITFTDISPLAYST7789_TFTHEIGHT + DIOSPITFTDISPLAYST7789_240x240_YSTART) & 0xFF)) return false;   // YEND = 240



  if(!SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_INVON)) return false;                           // Inversion ON
  GEN_XSLEEP.MilliSeconds(10);

  if(!SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_NORON)) return false;                           // Normal display on
  GEN_XSLEEP.MilliSeconds(10);

  if(!SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_DISPON)) return false;                          // Main display turn on, no args, w/delay
  GEN_XSLEEP.MilliSeconds(20);

  TFT_SetRotation(1); 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::TFT_Reset()
* @brief      TFT reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::TFT_Reset()
{  
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) == DIOGPIO_ID_NOTDEFINED) return false;
  
  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);
  GEN_XSLEEP.MilliSeconds(50);

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), false);
  GEN_XSLEEP.MilliSeconds(50);

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET), true);
  GEN_XSLEEP.MilliSeconds(50);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::TFT_SetWindow(int xs, int ys, int xe, int ye)
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
bool DIOSPITFTDISPLAYST7789::TFT_SetWindow(int xs, int ys, int xe, int ye)
{
  XWORD x_start = xs + _xstart;
  XWORD y_start = ys + _ystart;
  XWORD x_end   = xe + _xstart;  
  XWORD y_end   = ye + _ystart;

  bool status = true;

  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_CASET);  // Column addr set
  
  
  SendData((XWORD)SwapWORD(x_start));  
  SendData((XWORD)SwapWORD(x_end));  

  /*
  canvasbuffer.Delete();
  canvasbuffer.Add((XBYTE)(x_start >> 8));
  canvasbuffer.Add((XBYTE)(x_start &  0xFF));             // X START
  canvasbuffer.Add((XBYTE)(x_end   >> 8));
  canvasbuffer.Add((XBYTE)(x_end   &  0xFF));             // X END
  SendData();
  */


  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_RASET);  // Row addr set
  
  SendData((XWORD)SwapWORD(y_start));  
  SendData((XWORD)SwapWORD(y_end));  

  /*
  canvasbuffer.Delete();
  canvasbuffer.Add((XBYTE)(y_start >> 8));
  canvasbuffer.Add((XBYTE)(y_start &  0xFF));             // Y START
  canvasbuffer.Add((XBYTE)(y_end   >> 8));
  canvasbuffer.Add((XBYTE)(y_end   &  0xFF));             // Y END
  SendData();
  */
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPITFTDISPLAYST7789::TFT_SetRotation(XBYTE mode)
* @brief      TFT set rotation
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPITFTDISPLAYST7789::TFT_SetRotation(XBYTE mode) 
{
  SendCommand(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL);
  
  rotation = (mode & 3);
  
  switch(rotation) 
    {
      case 0: SendData((XBYTE)(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MX | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MY | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_RGB));
              _xstart = _colstart;
              _ystart = _rowstart;
              break;

      case 1: SendData((XBYTE)(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MY | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MV | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_RGB));
              _ystart = _colstart;
              _xstart = _rowstart;
              break;

      case 2: SendData((XBYTE)(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_RGB));
              _xstart = 0;
              _ystart = 0;
              break;

      case 3: SendData((XBYTE)(DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MX | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MV | DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_RGB));
              _xstart = 0;
              _ystart = 0;
              break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SendCommand(XBYTE command)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SendCommand(XBYTE command)
{
  bool status;

  DIOSPITFTDISPLAYST7789_DC_CMD;

  status = diostream->Write(&command, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SendData()
* @brief      Send data
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SendData()
{
  bool status;

  DIOSPITFTDISPLAYST7789_DC_DATA;  

  status = diostream->Write(canvasbuffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SendData(XBYTE data)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SendData(XBYTE data)
{
  bool status;

  DIOSPITFTDISPLAYST7789_DC_DATA;  

  status = diostream->Write(&data, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SendData(XWORD data)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SendData(XWORD data)
{
  bool status;

  DIOSPITFTDISPLAYST7789_DC_DATA;  

  status = diostream->Write((XBYTE*)&data, 2)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPITFTDISPLAYST7789::SendCommandParams(XBYTE command, int ndata, ...)
* @brief      Send command params
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* @param[in]  ndata : Ndata value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPITFTDISPLAYST7789::SendCommandParams(XBYTE command, int ndata, ...)
{
  va_list arg;

  va_start(arg, ndata);

  SendCommand(command);

  canvasbuffer.Delete();

  for(int c=0; c<ndata; c++)
    {
      XBYTE data = (XBYTE)va_arg(arg, int);
      canvasbuffer.Add((XBYTE)data);
    }

  SendData();

  va_end(arg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPITFTDISPLAYST7789::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPITFTDISPLAYST7789::Clean()
{
  diostream = NULL;
  
  rotation  = 0;
}



