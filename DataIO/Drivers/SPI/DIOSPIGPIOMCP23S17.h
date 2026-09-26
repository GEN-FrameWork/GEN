/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIGPIOMCP23S17.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"
#include "DIODeviceSPI.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSPIGPIOMCP23S17_WRITE_CMD      0
#define DIOSPIGPIOMCP23S17_READ_CMD       1

// Register addresses
#define DIOSPIGPIOMCP23S17_IODIRA         0x00  // I/O direction A
#define DIOSPIGPIOMCP23S17_IODIRB         0x01  // I/O direction B
#define DIOSPIGPIOMCP23S17_IPOLA          0x02  // I/O polarity A
#define DIOSPIGPIOMCP23S17_IPOLB          0x03  // I/O polarity B
#define DIOSPIGPIOMCP23S17_GPINTENA       0x04  // interupt enable A
#define DIOSPIGPIOMCP23S17_GPINTENB       0x05  // interupt enable B
#define DIOSPIGPIOMCP23S17_DEFVALA        0x06  // register default value A (interupts)
#define DIOSPIGPIOMCP23S17_DEFVALB        0x07  // register default value B (interupts)
#define DIOSPIGPIOMCP23S17_INTCONA        0x08  // interupt control A
#define DIOSPIGPIOMCP23S17_INTCONB        0x09  // interupt control B
#define DIOSPIGPIOMCP23S17_IOCON          0x0A  // I/O config (also 0x0B)
#define DIOSPIGPIOMCP23S17_GPPUA          0x0C  // port A pullups
#define DIOSPIGPIOMCP23S17_GPPUB          0x0D  // port B pullups
#define DIOSPIGPIOMCP23S17_INTFA          0x0E  // interupt flag A (where the interupt came from)
#define DIOSPIGPIOMCP23S17_INTFB          0x0F  // interupt flag B
#define DIOSPIGPIOMCP23S17_INTCAPA        0x10  // interupt capture A (value at interupt is saved here)
#define DIOSPIGPIOMCP23S17_INTCAPB        0x11  // interupt capture B
#define DIOSPIGPIOMCP23S17_GPIOA          0x12  // port A
#define DIOSPIGPIOMCP23S17_GPIOB          0x13  // port B
#define DIOSPIGPIOMCP23S17_OLATA          0x14  // output latch A
#define DIOSPIGPIOMCP23S17_OLATB          0x15  // output latch B

// I/O config
#define DIOSPIGPIOMCP23S17_BANK_OFF       0x00  // addressing mode
#define DIOSPIGPIOMCP23S17_BANK_ON        0x80
#define DIOSPIGPIOMCP23S17_INT_MIRROR_ON  0x40  // interupt mirror (INTa|INTb)
#define DIOSPIGPIOMCP23S17_INT_MIRROR_OFF 0x00
#define DIOSPIGPIOMCP23S17_SEQOP_OFF      0x20  // incrementing address pointer
#define DIOSPIGPIOMCP23S17_SEQOP_ON       0x00
#define DIOSPIGPIOMCP23S17_DISSLW_ON      0x10  // slew rate
#define DIOSPIGPIOMCP23S17_DISSLW_OFF     0x00
#define DIOSPIGPIOMCP23S17_HAEN_ON        0x08  // hardware addressing
#define DIOSPIGPIOMCP23S17_HAEN_OFF       0x00
#define DIOSPIGPIOMCP23S17_ODR_ON         0x04  // open drain for interupts
#define DIOSPIGPIOMCP23S17_ODR_OFF        0x00
#define DIOSPIGPIOMCP23S17_INTPOL_HIGH    0x02  // interupt polarity
#define DIOSPIGPIOMCP23S17_INTPOL_LOW     0x00

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMSPICONFIG;
class DIOSTREAMSPI;

class DIOSPIGPIOMCP23S17 : public DIODEVICESPI
{
  public:
                              DIOSPIGPIOMCP23S17            ();
    virtual                  ~DIOSPIGPIOMCP23S17            ();

    bool                      IniDevice                     ();
   
    bool                      Configure                     ();
   
    bool                      Read_Register                 (XBYTE reg, XBYTE addr, XBYTE& data);
    bool                      Write_Register                (XBYTE reg, XBYTE addr, XBYTE data);

    bool                      Read_Bit                      (XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE& data);    
    bool                      Write_Bit                     (XBYTE reg, XBYTE addr, XBYTE bitnum, XBYTE data);

    bool                      End                           ();

    DIOSTREAMSPI*             GetDIOStreamSPI               ();
    void                      SetDIOStreamSPI               (DIOSTREAMSPI* diostreamSPI);

  protected:

    int                       timeout;

    DIOSTREAMSPICONFIG*       diostreamcfg;
    DIOSTREAMSPI*             diostream;
    bool                      isdiostreamSPIexternal;

  private:

    static XBYTE              GetControlByte                (XBYTE rw_cmd, XBYTE addr);

    void                      Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





