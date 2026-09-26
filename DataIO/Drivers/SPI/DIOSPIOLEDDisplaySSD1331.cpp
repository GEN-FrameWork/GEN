/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIOLEDDisplaySSD1331.cpp
* 
* @class      DIOSPIOLEDDISPLAYSSD1331
* @brief      Data Input/Output SPI Display OLED SSD 1331 class
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

#include "DIOSPIOLEDDisplaySSD1331.h"

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
* @fn         DIOSPIOLEDDISPLAYSSD1331::DIOSPIOLEDDISPLAYSSD1331(): DIODEVICESPI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIOLEDDISPLAYSSD1331::DIOSPIOLEDDISPLAYSSD1331(): DIODEVICESPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPIOLEDDISPLAYSSD1331::~DIOSPIOLEDDISPLAYSSD1331()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPIOLEDDISPLAYSSD1331::~DIOSPIOLEDDISPLAYSSD1331()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::IniDevice()
{
  if(!DIODEVICESPI::IniDevice()) return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_MASTER);
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
          width            = 96;
          height           = 64;

          sizebuffer       = ((width*height)*(sizeof(XWORD)));

          if(!Initializate()) return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::Clear(XWORD color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::Clear(XWORD color)
{
  if(!IsInitialized()) return false;

  if(!EnableFillMode(true)) return false;

  if(!FillRect(0, 0, width-1, height-1, color, color)) return false;

  return EnableFillMode(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;
  if(!buffer)          return false;

  if(!MaxWindow()) return false;

  return SendData(buffer, sizebuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::PutPixel(XWORD x, XWORD y, XWORD color)
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
bool DIOSPIOLEDDISPLAYSSD1331::PutPixel(XWORD x, XWORD y, XWORD color)
{
  if(!IsInitialized()) return false;

  XBYTE cmd[]= { DIOSPIOLEDDISPLAYSSD1331_CMD_SETCOLUMNADDRESS, (XBYTE)x, (XBYTE)x, DIOSPIOLEDDISPLAYSSD1331_CMD_SETROWADDRESS, (XBYTE)y, (XBYTE)y };

  if(!SendCommand(cmd, sizeof(cmd))) return false;

  XBYTE datacolor[2];

  datacolor[0] = (color >> 8);
  datacolor[1] = (color &  0xFF);

  return SendData(datacolor, sizeof(datacolor));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::End()
{  
  ClearDisplay();

  Active(false);

  if(!DIODEVICESPI::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::Initializate()
* @brief      Initializate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::Initializate()
{
  Reset();

  // --------------------------------------------------------------
  // Initialize sequence
  //

  XBYTE cmd[] = {  DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYOFF             ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETREMAP               , 0x72 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETDISPLAYSTARTLINE    , 0x00 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETDISPLAYOFFSET       , 0x00 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_NORMALDISPLAY          ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETMULTIPLEXRATIO      , 0x3F ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETMASTERCONFIGURE     , 0x8E ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_POWERSAVEMODE          , 0x0B ,  // Disabled.
                   DIOSPIOLEDDISPLAYSSD1331_CMD_PHASE12PERIOD          , 0x74 ,  // Default value.
                   DIOSPIOLEDDISPLAYSSD1331_CMD_CLOCKDIVIDER           , 0xD0 ,  // Default value.
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDA     , 0x80 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDB     , 0x80 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDC     , 0x80 ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGEVOLTAGE    , 0x3E ,  // Default value.
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETVVOLTAGE            , 0x3E ,  // Default value.
                   DIOSPIOLEDDISPLAYSSD1331_CMD_MASTERCURRENTCONTROL   , 0x0F ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTA           , 0xFF ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTB           , 0xFF ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTC           , 0xFF ,
                   DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYON
                };

  if(!SendCommand(cmd, sizeof(cmd))) return false;

  return ClearDisplay();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::Reset()
{
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET) == DIOGPIO_ID_NOTDEFINED)  return false;
  if(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC)    == DIOGPIO_ID_NOTDEFINED)  return false;
            
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
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::Active(bool on)
* @brief      Active
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::Active(bool on)
{
  return SendCommand((on?DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYON:DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYOFF));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::EnableFillMode(bool on)
* @brief      Enable fill mode
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::EnableFillMode(bool on)
{
  XBYTE cmd[]=  {  (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECT                                                              ,
                   on?(XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECTENABLE:(XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECTDISABLE
                };

  return SendCommand(cmd, sizeof(cmd));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::EnableLockMode(bool on)
* @brief      Enable lock mode
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::EnableLockMode(bool on)
{
  XBYTE cmd[] = { (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODE                                                                 ,
                  on? (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODEENABLE: (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODEDISABLE
                };

  return SendCommand(cmd, sizeof(cmd));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::EnableScrollMode(bool on)
* @brief      Enable scroll mode
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::EnableScrollMode(bool on)
{
  return SendCommand((on?DIOSPIOLEDDISPLAYSSD1331_CMD_ACTIVESCROLLING:DIOSPIOLEDDISPLAYSSD1331_CMD_DEACTIVESCROLLING));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::SetScrollMode(XWORD horizontal, XWORD vertical)
* @brief      Set scroll mode
* @ingroup    DATAIO
* 
* @param[in]  horizontal : Horizontal value.
* @param[in]  vertical : Vertical value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::SetScrollMode(XWORD horizontal, XWORD vertical)
{
  EnableScrollMode(false);

  XBYTE cmd[]= { DIOSPIOLEDDISPLAYSSD1331_CMD_CONTINUOUSSCROLLINGSETUP, (XBYTE)horizontal, 0x00, 0x3F, (XBYTE)vertical, 0x00 };

  return SendCommand(cmd, sizeof(cmd));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::ClearDisplay()
* @brief      Clear display
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::ClearDisplay()
{
  XBYTE cmd[]=  { (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_CLEARWINDOW  ,
                  (XBYTE)0                                        ,
                  (XBYTE)0                                        ,
                  (XBYTE)(width-1)                                ,
                  (XBYTE)(height-1)
                };

  return SendCommand(cmd, sizeof(cmd));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::MaxWindow()
* @brief      Max window
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::MaxWindow()
{
  XBYTE cmd[]= {  DIOSPIOLEDDISPLAYSSD1331_CMD_SETCOLUMNADDRESS, 0x00, (XBYTE)(width-1),  DIOSPIOLEDDISPLAYSSD1331_CMD_SETROWADDRESS, 0x00, (XBYTE)(height-1) };

  return SendCommand(cmd, sizeof(cmd));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::FillRect(XBYTE x0, XBYTE y0, XBYTE x1, XBYTE y1, XWORD color, XWORD background)
* @brief      Fill rect
* @ingroup    DATAIO
* 
* @param[in]  x0 : X0 value.
* @param[in]  y0 : Y0 value.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  color : Color value.
* @param[in]  background : Background value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::FillRect(XBYTE x0, XBYTE y0, XBYTE x1, XBYTE y1, XWORD color, XWORD background)
{
  //---------------------------------------------------------------

  XBYTE cmd[]     = { (XBYTE)DIOSPIOLEDDISPLAYSSD1331_CMD_DRAWRECT ,
                      (XBYTE)(x0 & 0xFF)                          ,
                      (XBYTE)(y0 & 0xFF)                          ,
                      (XBYTE)(x1 & 0xFF)                          ,
                      (XBYTE)(y1 & 0xFF)
                    };

  if(!SendCommand(cmd, sizeof(cmd)))   return false;

  //---------------------------------------------------------------

  XBYTE cmd1[]    = { (XBYTE)((color >> 11) << 1)                 ,
                      (XBYTE)((color >> 5) & 0x3F)                ,
                      (XBYTE)((color << 1) & 0x3F)
                    };

  if(!SendCommand(cmd1, sizeof(cmd1))) return false;

  //---------------------------------------------------------------

  XBYTE cmd2[]    = { (XBYTE)((background >> 11) << 1)            ,
                      (XBYTE)((background >> 5) & 0x3F)           ,
                      (XBYTE)((background << 1) & 0x3F)
                    };

  if(!SendCommand(cmd2, sizeof(cmd2))) return false;

  //---------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::SendCommand(XBYTE command, int timeout)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::SendCommand(XBYTE command, int timeout)
{
  bool status = false;

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);
  
  if(diostream) status = diostream->Write(&command, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::SendCommand(XBYTE* command, XDWORD size, int timeout)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command pointer to use.
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::SendCommand(XBYTE* command, XDWORD size, int timeout)
{
  bool status = false;

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);
    
  if(diostream) status = diostream->Write(command, size)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::SendData(XBYTE data, int timeout)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::SendData(XBYTE data, int timeout)
{
  bool status = false;

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true);
  
  if(diostream) status = diostream->Write(&data, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPIOLEDDISPLAYSSD1331::SendData(XBYTE* data, XDWORD size, int timeout)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPIOLEDDISPLAYSSD1331::SendData(XBYTE* data, XDWORD size, int timeout)
{
  bool status = false;

  GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true);
  
  if(diostream) status = diostream->Write(data, size)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPIOLEDDISPLAYSSD1331::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPIOLEDDISPLAYSSD1331::Clean()
{
  isdirectconnection      = false;
}























