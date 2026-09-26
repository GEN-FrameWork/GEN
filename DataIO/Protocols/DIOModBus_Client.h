/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOModBus_Client.h
* 
* @class      DIOMODBUS_CLIENT
* @brief      Data Input/Output Mod Bus client protocol class
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

#include "XBuffer.h"
#include "XMap.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOMODBUS_CLIENTMODE
{
  DIOMODBUS_CLIENTMODE_UNKNOWN   = 0 ,

  DIOMODBUS_CLIENTMODE_RTU           ,
  DIOMODBUS_CLIENTMODE_ASCII         ,
  DIOMODBUS_CLIENTMODE_TCPIP
};

enum DIOMODBUS_CLIENT_FUNCTION
{
  DIOMODBUS_CLIENT_FUNCTION_READ_COILS                        = 0x01 ,
  DIOMODBUS_CLIENT_FUNCTION_READ_DISCRETE_INPUTS              = 0x02 ,
  DIOMODBUS_CLIENT_FUNCTION_READ_HOLDING_REGISTERS            = 0x03 ,
  DIOMODBUS_CLIENT_FUNCTION_READ_INPUT_REGISTERS              = 0x04 ,
  DIOMODBUS_CLIENT_FUNCTION_WRITE_SINGLE_COIL                 = 0x05 ,
  DIOMODBUS_CLIENT_FUNCTION_WRITE_SINGLE_REGISTER             = 0x06 ,
  DIOMODBUS_CLIENT_FUNCTION_WRITE_MULTIPLE_COILS              = 0x0F ,
  DIOMODBUS_CLIENT_FUNCTION_WRITE_MULTIPLE_REGISTERS          = 0x10 ,
  DIOMODBUS_CLIENT_FUNCTION_READ_FILE_RECORD                  = 0x14 ,
  DIOMODBUS_CLIENT_FUNCTION_WRITE_FILE_RECORD                 = 0x15 ,
  DIOMODBUS_CLIENT_FUNCTION_MASK_WRITE_REGISTER               = 0x16 ,
  DIOMODBUS_CLIENT_FUNCTION_READWRITE_MULTIPLE_REGISTERS      = 0x17 ,
  DIOMODBUS_CLIENT_FUNCTION_READ_FIFO_QUEUE                   = 0x18 ,
  DIOMODBUS_CLIENT_FUNCTION_ENCAPSULATED_INTERFACE_TRANSPORT  = 0x2B
};

#define DIOMODBUS_CLIENTMODE_STANDARDPORT                      502
#define DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT                    5

#define DIOMODBUS_CLIENTMODE_SIZEANSWERFIX                     sizeanswer =    2                                     /* Size Head (Address +  Function Code  */             \
                                                                             + 2                                     /* Addres                               */             \
                                                                             + 2                                     /* Value                                */             \
                                                                             + 2                                     /* Size CRC                             */

#define DIOMODBUS_CLIENTMODE_SIZEANSWERVAR                     sizeanswer =    3                                     /* Size Head (Address +  Function Code + Size data */  \
                                                                             + diostream->GetInXBuffer()->GetByte(2) /* Size data */                                        \
                                                                             + 2                                     /* Size CRC */

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAM;

class DIOMODBUS_CLIENT
{
  public:
                              DIOMODBUS_CLIENT        (DIOSTREAM* diostream, DIOMODBUS_CLIENTMODE mode = DIOMODBUS_CLIENTMODE_RTU);
    virtual                  ~DIOMODBUS_CLIENT        ();

    DIOSTREAM*                GetDIOStream            ();

    DIOMODBUS_CLIENTMODE      GetMode                 ();
    bool                      SetMode                 (DIOMODBUS_CLIENTMODE mode);

    bool                      Connect                 (XBYTE unit, int timeout = DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT);

    XBYTE                     GetUnit                 ();
    void                      SetUnit                 (XBYTE unit);

    bool                      ReadHoldingRegisters    (XWORD address, XWORD nregisters, XBUFFER& answer, int timeout = DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT);

    bool                      ReadInputRegisters      (XWORD address, XWORD nregisters, XMAP<XWORD, XWORD>& answer, int timeout = DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT);
    bool                      WriteSingleRegister     (XWORD address, XWORD value, int timeout = DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT);

    bool                      Disconnect              ();

  private:

    bool                      MakeCommand             (DIOMODBUS_CLIENT_FUNCTION function, XBUFFER& ask, XBUFFER& answer, int timeout = DIOMODBUS_CLIENTMODE_DEFAULTTIMEOUT);
    XWORD                     CRC16                   (XBUFFER& data);

    void                      Clean                   ();

    DIOSTREAM*                diostream;

    DIOMODBUS_CLIENTMODE      mode;
    XBYTE                     unit;

    XTIMER*                   xtimer;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




