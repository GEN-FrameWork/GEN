/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSTM32_FATSD_SPI.cpp
* 
* @class      XSTM32_FATSD_SPI
* @brief      eXtended STM32 FAT SD cards by SPI class
* @ingroup    PLATFORM_STM32
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
* ---------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSTM32_FATSD_SPI.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32_FATSD_SPI::XSTM32_FATSD_SPI()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32_FATSD_SPI::XSTM32_FATSD_SPI()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32_FATSD_SPI::~XSTM32_FATSD_SPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32_FATSD_SPI::~XSTM32_FATSD_SPI()    
{ 
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::Ini(SPI_HandleTypeDef* handleSPI, XDWORD GPIOentryID_CS, XDWORD GPIOentryID_switch, XDWORD timeout)
* @brief      Initialize the object
* @ingroup    PLATFORM_STM32
* 
* @param[in]  handleSPI : Handle SPI pointer to use.
* @param[in]  GPIOentryID_CS : GPI Oentry ID CS value.
* @param[in]  GPIOentryID_switch : GPI Oentry ID switch value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::Ini(SPI_HandleTypeDef* handleSPI, XDWORD GPIOentryID_CS, XDWORD GPIOentryID_switch, XDWORD timeout)
{
  if(!handleSPI) return false;

  this->GPIOentryID_CS      = GPIOentryID_CS; 
  this->GPIOentryID_switch  = GPIOentryID_switch;

  if(GPIOentryID_CS     != DIOGPIO_ID_NOTDEFINED)   GEN_DIOGPIO.SetMode(GPIOentryID_CS, DIOGPIO_MODE_OUTPUT);  
  if(GPIOentryID_switch != DIOGPIO_ID_NOTDEFINED)   GEN_DIOGPIO.SetMode(GPIOentryID_switch , DIOGPIO_MODE_INPUT);

  this->handleSPI = handleSPI;
  this->timeout   = timeout;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XSTM32_FATSD_SPI::GetGPIOEntryID_CS()
* @brief      Get GPIO entry IDCS
* @ingroup    PLATFORM_STM32
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XSTM32_FATSD_SPI::GetGPIOEntryID_CS()
{
  return GPIOentryID_CS;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SetGPIOEntryID_CS(XDWORD GPIOentryID)
* @brief      Set GPIO entry IDCS
* @ingroup    PLATFORM_STM32
* 
* @param[in]  GPIOentryID : GPI Oentry ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SetGPIOEntryID_CS(XDWORD GPIOentryID)
{
  GPIOentryID_CS = GPIOentryID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XSTM32_FATSD_SPI::GetGPIOEntryID_Switch()
* @brief      Get GPIO entry ID switch
* @ingroup    PLATFORM_STM32
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XSTM32_FATSD_SPI::GetGPIOEntryID_Switch()
{
  return GPIOentryID_switch; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SetGPIOEntryID_Switch(XDWORD GPIOentryID)
* @brief      Set GPIO entry ID switch
* @ingroup    PLATFORM_STM32
* 
* @param[in]  GPIOentryID : GPI Oentry ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SetGPIOEntryID_Switch(XDWORD GPIOentryID)
{
  GPIOentryID_switch = GPIOentryID; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SPI_HandleTypeDef* XSTM32_FATSD_SPI::GetHandleSPI()
* @brief      Get handle SPI
* @ingroup    PLATFORM_STM32
* 
* @return     SPI_HandleTypeDef* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SPI_HandleTypeDef* XSTM32_FATSD_SPI::GetHandleSPI()
{
  return handleSPI;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SetHandleSPI(SPI_HandleTypeDef* handleSPI)
* @brief      Set handle SPI
* @ingroup    PLATFORM_STM32
* 
* @param[in]  handleSPI : Handle SPI pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SetHandleSPI(SPI_HandleTypeDef* handleSPI)
{
  this->handleSPI = handleSPI;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::Disk_IsInserted()
* @brief      Disk is inserted
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::Disk_IsInserted()
{
  if(GPIOentryID_switch == DIOGPIO_ID_NOTDEFINED) return true;

  return GEN_DIOGPIO.GetValue(GPIOentryID_switch);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DSTATUS XSTM32_FATSD_SPI::Disk_Initialize(XBYTE drive)
* @brief      Disk initialize
* @ingroup    PLATFORM_STM32
* 
* @param[in]  drive : Drive value.
* 
* @return     DSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DSTATUS XSTM32_FATSD_SPI::Disk_Initialize(XBYTE drive)
{
  XBYTE n; 
  XBYTE type;
  XBYTE ocr[4];

  if(drive)               return STA_NOINIT;
  if(status & STA_NODISK) return status;

  // SD Power On 
  SD_PowerOn();

  // SPI Chip Select 
  CS_Select();
  
  type = 0;

  // Idle 
  if(SD_SendCmd(XSTM32_FATSD_SPI_CMD0, 0) == 1)
    {    
      timer1 = 100;
    
      if(SD_SendCmd(XSTM32_FATSD_SPI_CMD8, 0x1AA) == 1)
        {
          // SDC Ver2+ 
          for(n=0; n<4; n++)
            {
              ocr[n] = SPI_ReadByte();
            }

          if(ocr[2] == 0x01 && ocr[3] == 0xAA)
            {
              // 2.7-3.6V 
              do{ if(SD_SendCmd(XSTM32_FATSD_SPI_CMD55, 0) <= 1 && SD_SendCmd(XSTM32_FATSD_SPI_CMD41, 1UL << 30) == 0)  break; // ACMD41 with HCS bit 

                } while (timer1);

              if(timer1 && SD_SendCmd(XSTM32_FATSD_SPI_CMD58, 0) == 0)
                {
                  // Check CCS bit 
                  for(n=0; n<4; n++)
                    {
                      ocr[n] = SPI_ReadByte();
                    }

                  type = (ocr[0] & 0x40) ? 6 : 2;
                }
            }
        }
       else
        {
          // SDC Ver1 or MMC 
          type = (SD_SendCmd(XSTM32_FATSD_SPI_CMD55, 0) <= 1 && SD_SendCmd(XSTM32_FATSD_SPI_CMD41, 0) <= 1) ? 2 : 1; // SDC : MMC 

          do{ if(type == 2)
                {
                  if(SD_SendCmd(XSTM32_FATSD_SPI_CMD55, 0) <= 1 && SD_SendCmd(XSTM32_FATSD_SPI_CMD41, 0) == 0)  break; // ACMD41 
                }
               else
                {
                  if(SD_SendCmd(XSTM32_FATSD_SPI_CMD1, 0) == 0)  break; // CMD1 
                }

            } while (timer1);

          if(!timer1 || SD_SendCmd(XSTM32_FATSD_SPI_CMD16, 512) != 0)
            {        
              type = 0;
            }
       }
    }

  cardtype = type;

  CS_Deselect();

  SPI_ReadByte(); // Idle (Release DO) 

  if(type)
    {
      // Clear STA_NOINIT 
      status &= ~STA_NOINIT;
    }
   else
    {
      // Initialization failed 
      SD_PowerOff();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DSTATUS XSTM32_FATSD_SPI::Disk_Status(XBYTE drive)
* @brief      Disk status
* @ingroup    PLATFORM_STM32
* 
* @param[in]  drive : Drive value.
* 
* @return     DSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DSTATUS XSTM32_FATSD_SPI::Disk_Status(XBYTE drive)
{
  if(drive) return STA_NOINIT;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DRESULT XSTM32_FATSD_SPI::Disk_Read(XBYTE pdrive, XBYTE* buffer, XDWORD sector, XDWORD count)
* @brief      Disk read
* @ingroup    PLATFORM_STM32
* 
* @param[in]  pdrive : Pdrive value.
* @param[in]  buffer : Buffer to use.
* @param[in]  sector : Sector value.
* @param[in]  count : Count value.
* 
* @return     DRESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DRESULT XSTM32_FATSD_SPI::Disk_Read(XBYTE pdrive, XBYTE* buffer, XDWORD sector, XDWORD count)
{
  if(pdrive || !count)     return RES_PARERR;
  if(status & STA_NOINIT)  return RES_NOTRDY;

  if (!(cardtype & 4)) sector *= 512;      // sector Byte addresultsing 

  CS_Select();

  if(count == 1)
    {
      if ((SD_SendCmd(XSTM32_FATSD_SPI_CMD17, sector) == 0) && SD_ReadDataBlock(buffer, 512))  count = 0;
    }
   else
    {
      if(SD_SendCmd(XSTM32_FATSD_SPI_CMD18, sector) == 0)
        {
          do{ if(!SD_ReadDataBlock(buffer, 512))  break;

              buffer += 512;

            } while (--count);

          // STOP_TRANSMISSION 
          SD_SendCmd(XSTM32_FATSD_SPI_CMD12, 0);
        }
    }

  CS_Deselect();

  SPI_ReadByte(); // Idle Release DO

  return count?RES_ERROR:RES_OK;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DRESULT XSTM32_FATSD_SPI::Disk_Write(XBYTE pdrive, const XBYTE* buffer, XDWORD sector, XDWORD count)
* @brief      Disk write
* @ingroup    PLATFORM_STM32
* 
* @param[in]  pdrive : Pdrive value.
* @param[in]  buffer : Buffer to use.
* @param[in]  sector : Sector value.
* @param[in]  count : Count value.
* 
* @return     DRESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DRESULT XSTM32_FATSD_SPI::Disk_Write(XBYTE pdrive, const XBYTE* buffer, XDWORD sector, XDWORD count)
{
  if(pdrive || !count)      return RES_PARERR;
  if(status & STA_NOINIT)   return RES_NOTRDY;

  if(status & STA_PROTECT)  return RES_WRPRT;

  if(!(cardtype & 4))     sector *= 512; /* 지정 sector를 Byte addresultsing 단위로 변경 */

  CS_Select();

  if(count == 1)
    {    
      if((SD_SendCmd(XSTM32_FATSD_SPI_CMD24, sector) == 0) && SD_WriteDataBlock(buffer, 0xFE)) count = 0;
    }
   else
    {    
      if(cardtype & 2)
        {
          SD_SendCmd(XSTM32_FATSD_SPI_CMD55, 0);
          SD_SendCmd(XSTM32_FATSD_SPI_CMD23, count); /* ACMD23 */
        }

      if(SD_SendCmd(XSTM32_FATSD_SPI_CMD25, sector) == 0)
        {
          do{ if(!SD_WriteDataBlock(buffer, 0xFC)) break;

              buffer += 512;

            } while (--count);

          if(!SD_WriteDataBlock(0, 0xFD))
            {
              count = 1;
            }
        }
     }

  CS_Deselect();

  SPI_ReadByte();

  return (count?RES_ERROR:RES_OK);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DRESULT XSTM32_FATSD_SPI::Disk_IOTCL(XBYTE drive, XBYTE ctrl, void* buffer)
* @brief      Disk IOTCL
* @ingroup    PLATFORM_STM32
* 
* @param[in]  drive : Drive value.
* @param[in]  ctrl : Ctrl value.
* @param[in]  buffer : Buffer to use.
* 
* @return     DRESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DRESULT XSTM32_FATSD_SPI::Disk_IOTCL(XBYTE drive, XBYTE ctrl, void* buffer)
{
  DRESULT result;
  XBYTE   n;
  XBYTE   csd[16];
  XBYTE*  ptr = (XBYTE*)buffer;
  XWORD   csize;

  if(drive) return RES_PARERR;

  result = RES_ERROR;

  if(ctrl == CTRL_POWER)
    {
      switch(*ptr)
        {
          case  0 : if(SD_CheckPower())  SD_PowerOff();          // Power Off 
                    result = RES_OK;
                    break;

          case  1 : SD_PowerOn();                                // Power On 
                    result = RES_OK;
                    break;

          case  2 : *(ptr + 1) = (XBYTE) SD_CheckPower();
                    result = RES_OK;                             // Power Check 
                    break;

          default : result = RES_PARERR;
        }
    }
   else
    {
      if(status & STA_NOINIT) return RES_NOTRDY;

      CS_Select();

      switch(ctrl)
        {
          case GET_SECTOR_COUNT   : // SD Sector (DWORD) 
                                    if((SD_SendCmd(XSTM32_FATSD_SPI_CMD9, 0) == 0) && SD_ReadDataBlock(csd, 16))
                                      {
                                        if((csd[0] >> 6) == 1)
                                          {
                                            // SDC ver 2.00 
                                            csize = csd[9] + ((WORD) csd[8] << 8) + 1;
                                            *(DWORD*) buffer = (DWORD) csize << 10;
                                          }
                                          else
                                          {
                                            // MMC or SDC ver 1.XX 
                                            n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                                            csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
                                            *(DWORD*) buffer = (DWORD) csize << (n - 9);
                                          }

                                        result = RES_OK;
                                      }
                                    break;

          case GET_SECTOR_SIZE    : // (WORD) 
                                    *(WORD*) buffer = 512;
                                    result = RES_OK;
                                    break;

          case CTRL_SYNC          : if(SD_ReadyWait() == 0xFF)
                                    result = RES_OK;
                                    break;

          case MMC_GET_CSD        : // CSD (16 bytes)
                                    if(SD_SendCmd(XSTM32_FATSD_SPI_CMD9, 0) == 0 && SD_ReadDataBlock(ptr, 16)) result = RES_OK;
                                    break;

          case MMC_GET_CID        : // CID  (16 bytes) 
                                    if(SD_SendCmd(XSTM32_FATSD_SPI_CMD10, 0) == 0 && SD_ReadDataBlock(ptr, 16))  result = RES_OK;
                                    break;

          case MMC_GET_OCR        : // OCR (4 bytes) 
                                    if(SD_SendCmd(XSTM32_FATSD_SPI_CMD58, 0) == 0)
                                      {
                                        for (n = 0; n < 4; n++)
                                          {
                                            *ptr++ = SPI_ReadByte();
                                          }

                                        result = RES_OK;
                                      }

                        default   :  result = RES_PARERR;
        }

    CS_Deselect();

    SPI_ReadByte();
  }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::End()
* @brief      End the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::End()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::CS_Select()
* @brief      CS select
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::CS_Select()
{
  if(GPIOentryID_CS == DIOGPIO_ID_NOTDEFINED) return;

  GEN_DIOGPIO.SetValue(GPIOentryID_CS, false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::CS_Deselect()
* @brief      CS deselect
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::CS_Deselect()
{
  if(GPIOentryID_CS == DIOGPIO_ID_NOTDEFINED) return;

  GEN_DIOGPIO.SetValue(GPIOentryID_CS, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XSTM32_FATSD_SPI::SPI_ReadByte()
* @brief      SPI read byte
* @ingroup    PLATFORM_STM32
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XSTM32_FATSD_SPI::SPI_ReadByte()
{
  XBYTE dummy;
  XBYTE data;

  dummy = 0xFF;
  data  = 0;

  while ((HAL_SPI_GetState(handleSPI) != HAL_SPI_STATE_READY));

  HAL_SPI_TransmitReceive(handleSPI, &dummy, &data, 1, timeout);

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::SPI_WriteByte(XBYTE data)
* @brief      SPI write byte
* @ingroup    PLATFORM_STM32
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::SPI_WriteByte(XBYTE data)
{
  if(!handleSPI) return false;
  
  while(HAL_SPI_GetState(handleSPI) != HAL_SPI_STATE_READY);

  HAL_SPI_Transmit(handleSPI, &data, 1, timeout);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SPI_ReadBytePtr(XBYTE* buffer)
* @brief      SPI read byte ptr
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SPI_ReadBytePtr(XBYTE* buffer)
{
  (*buffer) = SPI_ReadByte();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XSTM32_FATSD_SPI::SD_ReadyWait()
* @brief      SD ready wait
* @ingroup    PLATFORM_STM32
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XSTM32_FATSD_SPI::SD_ReadyWait()
{
  XBYTE resultult;

  // 500ms 
  timer2 = 50;

  SPI_ReadByte();

  do{ // 0xFF SPI 
      resultult = SPI_ReadByte();

    } while ((resultult != 0xFF) && timer2);

  return resultult;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SD_PowerOn()
* @brief      SD power on
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SD_PowerOn()
{
  XBYTE     cmd_arg[6];
  XDWORD    count = 0x1FFF;
  
  CS_Deselect();

  for(int c=0; c<10; c++)
    {
      SPI_WriteByte(0xFF);
    }

  // SPI Chips Select 
  CS_Select();

  // GO_IDLE_STATE
  cmd_arg[0] = (XSTM32_FATSD_SPI_CMD0 | 0x40);
  cmd_arg[1] = 0;
  cmd_arg[2] = 0;
  cmd_arg[3] = 0;
  cmd_arg[4] = 0;
  cmd_arg[5] = 0x95;

  
  for(int c=0; c<6; c++)
    {
      SPI_WriteByte(cmd_arg[c]);
    }
  
  while((SPI_ReadByte() != 0x01) && count)
    {
      count--;
    }

  CS_Deselect();
  
  SPI_WriteByte(0XFF);

  powerflag = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::SD_PowerOff()
* @brief      SD power off
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::SD_PowerOff()
{
  powerflag = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::SD_CheckPower()
* @brief      SD check power
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::SD_CheckPower()
{
  return powerflag;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::SD_ReadDataBlock(XBYTE* buffer, XDWORD btr)
* @brief      SD read data block
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  btr : Btr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::SD_ReadDataBlock(XBYTE* buffer, XDWORD btr)
{
  XBYTE token;

  // 100ms 
  timer1 = 10;
  
  do{ token = SPI_ReadByte();

    } while((token == 0xFF) && timer1);

  // 0xFE Token
  if(token != 0xFE) return FALSE;

  do{ SPI_ReadBytePtr(buffer++);
      SPI_ReadBytePtr(buffer++);
  
    } while(btr -= 2);

  SPI_ReadByte(); // CRC
  SPI_ReadByte();

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32_FATSD_SPI::SD_WriteDataBlock(const XBYTE* buffer, XBYTE token)
* @brief      SD write data block
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  token : Token value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32_FATSD_SPI::SD_WriteDataBlock(const XBYTE* buffer, XBYTE token)
{
  XBYTE resultponse;
  XBYTE wc;
  XBYTE i = 0;

  // SD 
  if (SD_ReadyWait() != 0xFF) return FALSE;
  
  SPI_WriteByte(token);

  if(token != 0xFD)
    {
      wc = 0;

      // 512  
      do{ SPI_WriteByte(*buffer++);
          SPI_WriteByte(*buffer++);

        } while (--wc);

      SPI_ReadByte();       // CRC 
      SPI_ReadByte();
    
      while(i <= 64)
        {
          resultponse = SPI_ReadByte();
    
          if((resultponse & 0x1F) == 0x05) break;

          i++;
        }

      // SPI Clear 
      while (SPI_ReadByte() == 0);
    }

  if((resultponse & 0x1F) == 0x05) return TRUE;  
  
  return FALSE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XSTM32_FATSD_SPI::SD_SendCmd(XBYTE cmd, XDWORD arg)
* @brief      SD send cmd
* @ingroup    PLATFORM_STM32
* 
* @param[in]  cmd : Cmd value.
* @param[in]  arg : Arg value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XSTM32_FATSD_SPI::SD_SendCmd(XBYTE cmd, XDWORD arg)
{
  XBYTE crc;
  XBYTE resultult;

  // SD 
  if(SD_ReadyWait() != 0xFF) return 0xFF;
  
  SPI_WriteByte(cmd); 			            // Command 
  SPI_WriteByte((XBYTE) (arg >> 24)); 	// Argument[31..24] 
  SPI_WriteByte((XBYTE) (arg >> 16)); 	// Argument[23..16] 
  SPI_WriteByte((XBYTE) (arg >> 8)); 	  // Argument[15..8] 
  SPI_WriteByte((XBYTE) arg); 		      // Argument[7..0] 

  // CRC 
  crc = 0;
  if(cmd == XSTM32_FATSD_SPI_CMD0)   crc = 0x95; // CRC for CMD0(0) 
  if(cmd == XSTM32_FATSD_SPI_CMD8)   crc = 0x87; // CRC for CMD8(0x1AA) 

  // CRC 
  SPI_WriteByte(crc);

  // CMD12 Stop Reading 
  if (cmd == XSTM32_FATSD_SPI_CMD12) SPI_ReadByte();
  
  XBYTE n = 10;
  do{
      resultult = SPI_ReadByte();

    } while ((resultult & 0x80) && --n);

  return resultult;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32_FATSD_SPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32_FATSD_SPI::Clean()
{
  GPIOentryID_CS        = DIOGPIO_ID_NOTDEFINED;
  GPIOentryID_switch    = DIOGPIO_ID_NOTDEFINED;
  handleSPI             = NULL;

  timeout               = 0;
  timer1                = 0;
  timer2                = 0;
  status                = STA_NOINIT;     
  powerflag             = false;
  cardtype              = 0;
}

