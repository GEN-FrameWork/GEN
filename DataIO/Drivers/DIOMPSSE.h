/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOMPSSE.h
* 
* @class      DIOMPSSE
* @brief      Data Input/Output Multi-Protocol Synchronous Serial Engine (MPSSE) class
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

#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOMPSSE_CHIP_TYPE
{
  DIOMPSSE_CHIP_TYPE_AM       =   0 ,
  DIOMPSSE_CHIP_TYPE_BM       =   1 ,
  DIOMPSSE_CHIP_TYPE_2232C    =   2 ,
  DIOMPSSE_CHIP_TYPE_R        =   3 ,
  DIOMPSSE_CHIP_TYPE_2232H    =   4 ,
  DIOMPSSE_CHIP_TYPE_4232H    =   5 ,
  DIOMPSSE_CHIP_TYPE_232H     =   6 ,

  DIOMPSSE_CHIP_TYPE_UNKNOWN  =  -1
};


// Definitions for flow control
#define DIOMPSSE_SIO_RESET                        0   // Reset the port
#define DIOMPSSE_SIO_MODEM_CTRL                   1   // Set the modem control register
#define DIOMPSSE_SIO_SET_FLOW_CTRL                2   // Set flow control register
#define DIOMPSSE_SIO_SET_BAUD_RATE                3   // Set baud rate
#define DIOMPSSE_SIO_SET_DATA                     4   // Set the data characteristics of the port


// Requests
#define DIOMPSSE_SIO_RESET_REQUEST                DIOMPSSE_SIO_RESET
#define DIOMPSSE_SIO_SET_BAUDRATE_REQUEST         DIOMPSSE_SIO_SET_BAUD_RATE
#define DIOMPSSE_SIO_SET_DATA_REQUEST             DIOMPSSE_SIO_SET_DATA
#define DIOMPSSE_SIO_SET_FLOW_CTRL_REQUEST        DIOMPSSE_SIO_SET_FLOW_CTRL
#define DIOMPSSE_SIO_SET_MODEM_CTRL_REQUEST       DIOMPSSE_SIO_MODEM_CTRL
#define DIOMPSSE_SIO_POLL_MODEM_STATUS_REQUEST    0x05
#define DIOMPSSE_SIO_SET_EVENT_CHAR_REQUEST       0x06
#define DIOMPSSE_SIO_SET_ERROR_CHAR_REQUEST       0x07
#define DIOMPSSE_SIO_SET_LATENCY_TIMER_REQUEST    0x09
#define DIOMPSSE_SIO_GET_LATENCY_TIMER_REQUEST    0x0A
#define DIOMPSSE_SIO_SET_BITMODE_REQUEST          0x0B
#define DIOMPSSE_SIO_READ_PINS_REQUEST            0x0C
#define DIOMPSSE_SIO_READ_EEPROM_REQUEST          0x90
#define DIOMPSSE_SIO_WRITE_EEPROM_REQUEST         0x91
#define DIOMPSSE_SIO_ERASE_EEPROM_REQUEST         0x92

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMUSBLOCALENUMDEVICES;
class DIOSTREAMUSBCONFIG;
class DIOSTREAMDEVICEUSB;
class DIOSTREAM;

class DIOMPSSE
{
  public:
                                      DIOMPSSE              ();
    virtual                          ~DIOMPSSE              ();

    DIOMPSSE_CHIP_TYPE                GetChipType           ();
    XCHAR*                            GetChipTypeName       ();

    bool                              Open                  (int index = 0);

    bool                              IsOpen                ();

    bool                              Reset                 ();

    bool                              Close                 ();

  private:

    bool                              EnumDevices           ();

    bool                              SendCommand           (XBYTE command, XBUFFER& buffer);
    bool                              ReadResponse          (XBUFFER& buffer, int timeout);

    void                              Clean                 ();

    DIOMPSSE_CHIP_TYPE                chiptype;
    bool                              isopen;

    DIOSTREAMUSBLOCALENUMDEVICES*     dioenumUSBdevices;
    DIOSTREAMUSBCONFIG*               diostreamUSBcfg;
    DIOSTREAMDEVICEUSB*               USBdevice;
    DIOSTREAM*                        diostreambase;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





