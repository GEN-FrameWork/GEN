/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOBusPirate.h
* 
* @class      DIOBUSPIRATE
* @brief      Data Input/Output interface Bus Pirate 4.0
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

#include "XBase.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOBUSPIRATETYPEPROCOTOL
{
  DIOBUSPIRATETYPEPROCOTOL_UNKNOWN      = 0 ,
  DIOBUSPIRATETYPEPROCOTOL_HIZ              ,
  DIOBUSPIRATETYPEPROCOTOL_1WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_UART             ,
  DIOBUSPIRATETYPEPROCOTOL_I2C              ,
  DIOBUSPIRATETYPEPROCOTOL_SPI              ,
  DIOBUSPIRATETYPEPROCOTOL_2WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_3WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_KEYB             ,
  DIOBUSPIRATETYPEPROCOTOL_LCD              ,
  DIOBUSPIRATETYPEPROCOTOL_PIC              ,
  DIOBUSPIRATETYPEPROCOTOL_DIO              ,
};

enum DIOBUSPIRATESPEEDSNIFFER
{
  DIOBUSPIRATESPEEDSNIFFER_UNKNOWN      = 0 ,
  DIOBUSPIRATESPEEDSNIFFER_5KHZ             ,
  DIOBUSPIRATESPEEDSNIFFER_50KHZ            ,
  DIOBUSPIRATESPEEDSNIFFER_100KHZ           ,
  DIOBUSPIRATESPEEDSNIFFER_400KHZ           ,
};

enum DIOBUSPIRATESPEEDNOSNIFFER
{
  DIOBUSPIRATESPEEDNOSNIFFER_UNKNOWN    = 0 ,
  DIOBUSPIRATESPEEDNOSNIFFER_100KHZ         ,
  DIOBUSPIRATESPEEDNOSNIFFER_400KHZ         ,
  DIOBUSPIRATESPEEDNOSNIFFER_1MHZ           ,
};

#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_HIZ    _L("HiZ")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_1WIRE  _L("1-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_UART   _L("UART")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_I2C    _L("I2C")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_SPI    _L("SPI")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_2WIRE  _L("2-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_3WIRE  _L("3-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_KEYB   _L("3-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_LCD    _L("LCD")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_PIC    _L("PIC")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_DIO    _L("DIO")

#define DIOBUSPIRATE_DEFAULTTIMEOUT  5

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAM;

class DIOBUSPIRATE
{
  public:
                                DIOBUSPIRATE            (DIOSTREAM* diostream);
    virtual                    ~DIOBUSPIRATE            ();

    bool                        Ini                     (int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    DIOBUSPIRATETYPEPROCOTOL    GetActualProtocol       (int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);
    bool                        SetMode                 (DIOBUSPIRATETYPEPROCOTOL protocol, bool issoftware, int speed, bool issniffer, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    bool                        ReadI2CSnifferCommand   (XBUFFER& in, XBUFFER& out);

    bool                        End                     ();

  private:

    bool                        ReadPromptStatus        (XSTRING& status, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    bool                        WriteCommand            (XCHAR* command, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);
    bool                        WriteCommand            (XSTRING& command, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT)    { return WriteCommand(command.Get(), timeout);           }

    void                        Clean                   ();

    DIOSTREAM*                  diostream;
    XTIMER*                     xtimerout;     
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





