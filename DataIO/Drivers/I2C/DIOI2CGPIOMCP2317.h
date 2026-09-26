/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CGPIOMCP2317.h
* 
* @class      DIOI2CGPIOMCP2317
* @brief      Data Input/Output I2C MCP23S17 (Microchip) (8+8 Digital I/O) class
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

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOI2CGPIOMCP2317PORT
{
  DIOI2CGPIOMCP2317PORT_A     =  0 ,    //  PORT A
  DIOI2CGPIOMCP2317PORT_B          ,    //  PORT B
};

#define DIOI2CGPIOMCP2317_ADDR            0x23

#define DIOI2CGPIOMCP2317_IODIR           0x00    // I/O direction
#define DIOI2CGPIOMCP2317_GPIO            0x12    // Register for inputs
#define DIOI2CGPIOMCP2317_OLAT            0x14    // Register for outputs

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOI2CGPIOMCP2317 :  public DIODEVICEI2C
{
  public:

                              DIOI2CGPIOMCP2317           ();
    virtual                  ~DIOI2CGPIOMCP2317           ();

    bool                      Configure                   (XBYTE port_A = 0xFF, XBYTE port_B = 0x00);

    bool                      ReadInputs                  (DIOI2CGPIOMCP2317PORT port, XBYTE& value);
    bool                      WriteOutputs                (DIOI2CGPIOMCP2317PORT port, XBYTE value);

    bool                      SendCommand                 (XBYTE cmd, XBYTE value);

    bool                      End                         ();

    
  private:

    bool                      IniDevice                   ();

    void                      Clean                       ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





