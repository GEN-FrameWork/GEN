/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamSPIConfig.h
* 
* @class      DIOSTREAMSPICONFIG
* @brief      Data Input/Output Stream SPI Config class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOStream.h"
#include "DIOGPIO.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOSTREAMSPI_GPIO
{
  DIOSTREAMSPI_GPIO_CLK                     = 0  ,
  DIOSTREAMSPI_GPIO_MISO                         ,   
  DIOSTREAMSPI_GPIO_MOSI                         ,
  DIOSTREAMSPI_GPIO_CS                           ,
  
  DIOSTREAMSPI_GPIO_MAX                          
};

#define DIOSTREAMSPI_CPHA           0x01
#define DIOSTREAMSPI_CPOL           0x02

#define DIOSTREAMSPI_MODE_0         (0|0)
#define DIOSTREAMSPI_MODE_1         (0|DIOSTREAMSPI_CPHA)
#define DIOSTREAMSPI_MODE_2         (DIOSTREAMSPI_CPOL|0)
#define DIOSTREAMSPI_MODE_3         (DIOSTREAMSPI_CPOL|DIOSTREAMSPI_CPHA)

#define DIOSTREAMSPI_CS_HIGH        0x04
#define DIOSTREAMSPI_LSB_FIRST      0x08
#define DIOSTREAMSPI_3WIRE          0x10
#define DIOSTREAMSPI_LOOP           0x20
#define DIOSTREAMSPI_NO_CS          0x40
#define DIOSTREAMSPI_READY          0x80

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMSPICONFIG : public DIOSTREAMCONFIG
{
  public:
                                  DIOSTREAMSPICONFIG                ();
    virtual                      ~DIOSTREAMSPICONFIG                ();

    int                           GetPort                           ();
    void                          SetPort                           (int port);

    XSTRING*                      GetLocalDeviceName                ();
    bool                          SetLocalDeviceNameByPort          ();

    int                           GetChipSelect                     ();
    void                          SetChipSelect                     (int chipselect);

    bool                          IsDirectAccess                    ();

    XBYTE                         GetSPIMode                        ();
    XBYTE                         GetNBitsWord                      ();
    XDWORD                        GetSpeed                          ();
    XWORD                         GetDelay                          ();
    bool                          IsOnlyWrite                       ();
  
    XDWORD*                       GetPinsDirectAccess               ();
    bool                          SetPinsDirectAccess               (XDWORD GPIOID_CLK = DIOGPIO_ID_NOTDEFINED, XDWORD GPIOID_MISO = DIOGPIO_ID_NOTDEFINED, XDWORD GPIOID_MOSI = DIOGPIO_ID_NOTDEFINED, XDWORD GPIOID_CS = DIOGPIO_ID_NOTDEFINED);

    void                          SetSPIMode                        (XBYTE SPImode);
    void                          SetNBitsWord                      (XBYTE nbitsword);
    void                          SetSpeed                          (XDWORD speed);
    void                          SetDelay                          (XWORD delay);
    void                          SetIsOnlyWrite                    (bool isonlywrite);

  protected:

    int                           port;
    XSTRING                       localdevicename;
    int                           chipselect;

    bool                          isdirectaccess;
    XDWORD                        GPIO_ID[DIOSTREAMSPI_GPIO_MAX];

    XBYTE                         SPImode;
    XBYTE                         nbitsword;
    XDWORD                        speed;
    XWORD                         delay;
    bool                          isonlywrite;

  private:

    void                          Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





