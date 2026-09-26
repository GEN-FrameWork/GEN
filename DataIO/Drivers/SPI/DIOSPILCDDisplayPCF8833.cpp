/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPILCDDisplayPCF8833.cpp
* 
* @class      DIOSPILCDDISPLAYPCF8833
* @brief      Data Input/Output SPI Display LCD Philips PCF8833 class
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

#include "DIOSPILCDDisplayPCF8833.h"

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
* @fn         DIOSPILCDDISPLAYPCF8833::DIOSPILCDDISPLAYPCF8833(DIOSPILCDDISPLAYPCF8833_TYPE type) : DIODEVICESPI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* 
* @param[in]  type : PCF8833 display type.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPILCDDISPLAYPCF8833::DIOSPILCDDISPLAYPCF8833(DIOSPILCDDISPLAYPCF8833_TYPE type) : DIODEVICESPI()
{
  Clean();

  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSPILCDDISPLAYPCF8833::~DIOSPILCDDISPLAYPCF8833()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSPILCDDISPLAYPCF8833::~DIOSPILCDDISPLAYPCF8833()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::IniDevice()
{
  if(!DIODEVICESPI::IniDevice()) return false;

  diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);
  diostreamcfg->SetNBitsWord(8);
  diostreamcfg->SetSpeed(40*1000*1000);
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
          width            = 132;
          height           = 132;

          sizebuffer       = ((width*height)*(sizeof(XWORD)));
          sizebuffer      += ((width*height)*(sizeof(XWORD))/8);

          if(!Initializate()) return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Clear(XWORD color)
* @brief      Clear
* @ingroup    DATAIO
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Clear(XWORD color)
{
  if(!IsInitialized()) return false;

  for(XDWORD i = 0; i <height; i++)
    {
      Draw_HLine(0, i, width, color);
    }

  return true;


  /*
  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_CASET);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 131);

  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_PASET);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 131);

  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_RAMWR);

  XBYTE bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  for(int c=0; c<(132 * 132); c++)
    {
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, bcolor[0]);
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, bcolor[1]);
    }

  return Buffer_Send(databuffer);
  */
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Update(XBYTE* buffer)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;
  if(!buffer)          return false;

  SetWindow(0, 0, (width-1), (height-1));

  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  int d = 0;
  for(XDWORD c=0; c<(width * height); c++)
    {
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, buffer[d+1]);
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, buffer[d]);
      d+=2;
    }

  return Buffer_Send(databuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::PutPixel(XWORD x, XWORD y, XWORD color)
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
bool DIOSPILCDDISPLAYPCF8833::PutPixel(XWORD x, XWORD y, XWORD color)
{
  if(!IsInitialized()) return false;

  SetWindow(x, y, x, y);

  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, ((color >> 0x04) &  0xFF));
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, ((color &  0x0F) << 0x04));

  return Buffer_Send(databuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::End()
{  
  Clear();

  if(!DIODEVICESPI::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Initializate()
* @brief      Initializate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Initializate()
{
  Reset();

  switch(type)
    {
      case DIOSPILCDDISPLAYPCF8833_EPSON     : { XBUFFER databuffer;

                                                Buffer_Delete(databuffer);
                                                // Display control (0xCA)
                                                // 12 = 1100 - CL dividing ratio [don't divide] switching period 8H (default)
                                                // nlines/4 - 1 = 132/4 - 1 = 32 duty
                                                // No inversely highlighted lines
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_DISCTL);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x0C);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x20);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x00);

                                                // common scanning direction (0xBB)
                                                // 1->68, 132<-69 scan direction
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_COMSCN);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x01);

                                                // internal oscialltor ON (0xD1)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_OSCON);

                                                // sleep out (0x94)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_SLPOUT);

                                                // power ctrl (0x20)
                                                // everything on, no external reference resistors
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_PWRCTR);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x0F);

                                                // invert display mode (0xA7)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_DISINV);

                                                // data control (0xBC)
                                                // Inverse page address, reverse rotation column address, column scan-direction !!! try 0x01
                                                // normal RGB arrangement
                                                // 16-bit Grayscale Type A (12-bit color)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_DATCTL);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x03);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x00);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x02);

                                                // electronic volume, this is the contrast/brightness (0x81)
                                                // volume (contrast) setting - fine tuning, original (0-63)
                                                // internal resistor ratio - coarse adjustment (0-7)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_VOLCTR);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 32);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 3);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_NOP);

                                                Buffer_Send(databuffer);

                                                GEN_XSLEEP.MilliSeconds(100);

                                                Buffer_Delete(databuffer);

                                                // display on (0xAF)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_EPSON_CMD_DISON);


                                                Buffer_Send(databuffer);

                                              }
                                              break;

      case DIOSPILCDDISPLAYPCF8833_PHILLIPS  : { XBUFFER databuffer;

                                                Buffer_Delete(databuffer);

                                                /*
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_SLEEPOUT);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_INVOFF);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_COLMOD);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x05);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_MADCTL);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x08);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_SETCON);
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x3F);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_TEOFF);

                                                Buffer_Send(databuffer);

                                                GEN_XSLEEP.MilliSeconds(100);

                                                Buffer_Delete(databuffer);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_DISPON);

                                                Buffer_Send(databuffer);
                                                */



                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_SLEEPOUT);    // Sleep Out (0x11)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_BSTRON);      // Booster voltage on (0x03)
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_DISPON);      // Display on (0x29)

                                                //DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_INVON);    // Inversion on (0x20)

                                                // 12-bit color pixel format:
                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_COLMOD);      // Color interface format (0x3A)
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x03);                                               // 0b011 is 12-bit/pixel mode

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_MADCTL);      // Memory Access Control(PHILLIPS)
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0xC0);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_SETCON);      // Set Contrast(PHILLIPS)
                                                DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, 0x3E);

                                                DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_NOP);         // nop

                                                Buffer_Send(databuffer);

                                              }

                                              break;

                                   default  : return false;
      }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Reset()
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
* @fn         bool DIOSPILCDDISPLAYPCF8833::SetWindow(XWORD x0, XWORD y0, XWORD x1, XWORD y1)
* @brief      Set window
* @ingroup    DATAIO
* 
* @param[in]  x0 : X0 value.
* @param[in]  y0 : Y0 value.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::SetWindow(XWORD x0, XWORD y0, XWORD x1, XWORD y1)
{
  XBYTE   paset = 0;
  XBYTE   caset = 0;
  XBYTE   ramwr = 0;

  switch(type)
    {
      case DIOSPILCDDISPLAYPCF8833_EPSON     :  paset = DIOSPILCDDISPLAYPCF8833_EPSON_CMD_PASET;
                                                caset = DIOSPILCDDISPLAYPCF8833_EPSON_CMD_CASET;
                                                ramwr = DIOSPILCDDISPLAYPCF8833_EPSON_CMD_RAMWR;
                                                break;

      case DIOSPILCDDISPLAYPCF8833_PHILLIPS  :  paset = DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_PASET;
                                                caset = DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_CASET;
                                                ramwr = DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_RAMWR;
                                                break;

                                   default   :  return false;
    }

  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  // columns
  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, caset);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, x0);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, x1);

  // rows
  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, paset);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, y0);
  DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, y1);

  // start write
  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, ramwr);

  return Buffer_Send(databuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Draw_HLine(XBYTE x, XBYTE y, XBYTE width, XWORD color)
* @brief      Draw H line
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  width : Width value.
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Draw_HLine(XBYTE x, XBYTE y, XBYTE width, XWORD color)
{
  if((width < 1) || (width > DIOSPILCDDISPLAYPCF8833_ROW_LENGTH)) return false;

  SetWindow(x, y, (x + width - 1), y);

  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  for(XBYTE i=0; i<width; )
    {
      i += 2;

      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, ((color & 0xFF0) >> 0x04));                                // R1G1
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, ((color & 0x00F) << 0x04) | ((color & 0xF00) >> 0x08));    // B1R2
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, (color & 0x0FF));                                         // G2B2
    }

  return Buffer_Send(databuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Draw_HLine(XBYTE x, XBYTE y, XBYTE width, XWORD* buffer)
* @brief      Draw H line
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  width : Width value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Draw_HLine(XBYTE x, XBYTE y, XBYTE width, XWORD* buffer)
{
  if((width < 1) || (width > DIOSPILCDDISPLAYPCF8833_ROW_LENGTH)) return false;

  SetWindow(x, y, (x + width - 1), y);

  XBUFFER databuffer;

  Buffer_Delete(databuffer);

  for(XBYTE i=0; i<width; )
    {
      XWORD c1 = *buffer++;
      XWORD c2 = *buffer++;

      i += 2;

      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, (c1 & 0xFF0) >> 0x04);                              // R1G1
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, ((c1 & 0x00F) << 0x04) | ((c2 & 0xF00) >> 0x08));    // B1R2
      DIOSPILCDDISPLAYPCF8833_ADDDATA(databuffer, (c2 & 0x0FF));                                      // G2B2
    }

  return Buffer_Send(databuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Buffer_AjustWithNOP(XBUFFER& databuffer)
* @brief      Buffer ajust with NOP
* @ingroup    DATAIO
* 
* @param[in]  databuffer : Databuffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Buffer_AjustWithNOP(XBUFFER& databuffer)
{
  XBYTE nbitsfree = databuffer.Bit_GetBitsFree();

  DIOSPILCDDISPLAYPCF8833_ADDCOMMAND(databuffer, DIOSPILCDDISPLAYPCF8833_PHILLIPS_CMD_NOP);

  return nbitsfree?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Buffer_Send(XBUFFER& databuffer)
* @brief      Buffer send
* @ingroup    DATAIO
* 
* @param[in]  databuffer : Databuffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Buffer_Send(XBUFFER& databuffer)
{
  bool status = false;

  //Buffer_AjustWithNOP(databuffer);

  if(diostream) status = diostream->Write(databuffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSPILCDDISPLAYPCF8833::Buffer_Delete(XBUFFER& databuffer)
* @brief      Buffer delete
* @ingroup    DATAIO
* 
* @param[in]  databuffer : Databuffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSPILCDDISPLAYPCF8833::Buffer_Delete(XBUFFER& databuffer)
{
  databuffer.Delete();
  databuffer.Bit_SetNBits(9);
  databuffer.Bit_SetBitsFree(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSPILCDDISPLAYPCF8833::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSPILCDDISPLAYPCF8833::Clean()
{
  type        = DIOSPILCDDISPLAYPCF8833_UNKNOWN;
}



