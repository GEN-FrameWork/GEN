/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSTM32_FATSD_SPI.h
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
* * --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "diskio.h"

#include "XSTM32_HAL.h"

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XSTM32_FATSD_SPI_DEFAULT_TIMEOUT  1000

// Definitions for MMC/SDC command 
#define XSTM32_FATSD_SPI_CMD0     (0x40 +  0)    // GO_IDLE_STATE 
#define XSTM32_FATSD_SPI_CMD1     (0x40 +  1)    // SEND_OP_COND 
#define XSTM32_FATSD_SPI_CMD8     (0x40 +  8)    // SEND_IF_COND 
#define XSTM32_FATSD_SPI_CMD9     (0x40 +  9)    // SEND_CSD 
#define XSTM32_FATSD_SPI_CMD10    (0x40 + 10)    // SEND_CID 
#define XSTM32_FATSD_SPI_CMD12    (0x40 + 12)    // STOP_TRANSMISSION 
#define XSTM32_FATSD_SPI_CMD16    (0x40 + 16)    // SET_BLOCKLEN 
#define XSTM32_FATSD_SPI_CMD17    (0x40 + 17)    // READ_SINGLE_BLOCK 
#define XSTM32_FATSD_SPI_CMD18    (0x40 + 18)    // READ_MULTIPLE_BLOCK 
#define XSTM32_FATSD_SPI_CMD23    (0x40 + 23)    // SET_BLOCK_COUNT 
#define XSTM32_FATSD_SPI_CMD24    (0x40 + 24)    // WRITE_BLOCK 
#define XSTM32_FATSD_SPI_CMD25    (0x40 + 25)    // WRITE_MULTIPLE_BLOCK 
#define XSTM32_FATSD_SPI_CMD41    (0x40 + 41)    // SEND_OP_COND (ACMD) 
#define XSTM32_FATSD_SPI_CMD55    (0x40 + 55)    // APP_CMD 
#define XSTM32_FATSD_SPI_CMD58    (0x40 + 58)    // READ_OCR 

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSTM32_FATSD_SPI
{
  public:
                            XSTM32_FATSD_SPI        ();
    virtual                ~XSTM32_FATSD_SPI        ();

    bool                    Ini                     (SPI_HandleTypeDef* handleSPI, XDWORD GPIOentryID_CS, XDWORD GPIOentryID_switch = DIOGPIO_ID_NOTDEFINED, XDWORD timeout = XSTM32_FATSD_SPI_DEFAULT_TIMEOUT);

    XDWORD                  GetGPIOEntryID_CS       ();
    void                    SetGPIOEntryID_CS       (XDWORD GPIOentryID);

    XDWORD                  GetGPIOEntryID_Switch   ();
    void                    SetGPIOEntryID_Switch   (XDWORD GPIOentryID);

    SPI_HandleTypeDef*      GetHandleSPI            ();   
    void                    SetHandleSPI            (SPI_HandleTypeDef* handleSPI);
    
    bool                    Disk_IsInserted         ();

    DSTATUS                 Disk_Initialize         (XBYTE drive);
    DSTATUS                 Disk_Status             (XBYTE drive);
    DRESULT                 Disk_Read               (XBYTE pdrive, XBYTE* buffer, XDWORD sector, XDWORD count);
    DRESULT                 Disk_Write              (XBYTE pdrive, const XBYTE* buffer, XDWORD sector, XDWORD count);
    DRESULT                 Disk_IOTCL              (XBYTE drive, XBYTE ctrl, void* buffer);

    bool                    End                     ();  

  private:

    void                    CS_Select               ();
    void                    CS_Deselect             ();

    XBYTE                   SPI_ReadByte            ();
    bool                    SPI_WriteByte           (XBYTE data);
    void                    SPI_ReadBytePtr         (XBYTE* buffer);

    XBYTE                   SD_ReadyWait            ();
    void                    SD_PowerOn              ();
    void                    SD_PowerOff             ();
    bool                    SD_CheckPower           ();
    bool                    SD_ReadDataBlock        (XBYTE* buffer, XDWORD btr);
    bool                    SD_WriteDataBlock       (const XBYTE* buffer, XBYTE token);
    XBYTE                   SD_SendCmd              (XBYTE cmd, XDWORD arg);


    void                    Clean                   ();

    XDWORD                  GPIOentryID_CS;
    XDWORD                  GPIOentryID_switch;
    SPI_HandleTypeDef*      handleSPI;  

    XDWORD                  timeout;
    XBYTE                   timer1;
    XBYTE                   timer2;

    DSTATUS                 status;
    bool                    powerflag;
    XBYTE                   cardtype; 
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


