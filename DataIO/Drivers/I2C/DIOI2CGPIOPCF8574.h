/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CGPIOPCF8574.h
* 
* @class      DIOI2CGPIOPCF8574
* @brief      Data Input/Output I2C PCF8574 (GPIO 8 Pins) class
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
#include "XString.h"

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOI2CGPIOPCF8574_ADDR          0x20



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOI2CGPIOPCF8574 : public DIODEVICEI2C
{
  public:

                              DIOI2CGPIOPCF8574           ();
    virtual                  ~DIOI2CGPIOPCF8574           ();

    bool                      Read                        (XBYTE& value);
    bool                      Write                       (XBYTE value);

    bool                      End                         ();

  private:

    bool                      IniDevice                   ();

    void                      Clean                       ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





