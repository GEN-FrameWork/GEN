/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUARTConfig.h
* 
* @class      DIOSTREAMUARTCONFIG
* @brief      Data Input/Output Stream UART config class
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

#include "DIOStreamConfig.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMUARTDATABITS
{
  DIOSTREAMUARTDATABIT_5           = 5  ,
  DIOSTREAMUARTDATABIT_6                ,
  DIOSTREAMUARTDATABIT_7                ,
  DIOSTREAMUARTDATABIT_8                ,
  DIOSTREAMUARTDATABIT_9
};

enum DIOSTREAMUARTPARITY
{
  DIOSTREAMUARTPARITY_NONE          = 0 ,
  DIOSTREAMUARTPARITY_ODD               ,
  DIOSTREAMUARTPARITY_EVEN              ,
  DIOSTREAMUARTPARITY_MARK              ,
  DIOSTREAMUARTPARITY_SPACE
};

enum DIOSTREAMUARTSTOPBITS
{
  DIOSTREAMUARTSTOPBITS_ONE         = 0 ,
  DIOSTREAMUARTSTOPBITS_ONEANDAHALF     ,
  DIOSTREAMUARTSTOPBITS_TWO             ,
};

enum DIOSTREAMUARTFLOWCONTROL
{
  DIOSTREAMUARTFLOWCONTROL_NONE     = 0 ,
  DIOSTREAMUARTFLOWCONTROL_HARD         ,
  DIOSTREAMUARTFLOWCONTROL_SOFT         ,
};

#define DIOSTREAMUARTMASK_BAUDRATE            0x0001
#define DIOSTREAMUARTMASK_DATABITS            0x0002
#define DIOSTREAMUARTMASK_PARITY              0x0004
#define DIOSTREAMUARTMASK_STOPBITS            0x0008
#define DIOSTREAMUARTMASK_FLOWCONTROL         0x0010
#define DIOSTREAMUARTMASK_ALL                 (DIOSTREAMUARTMASK_BAUDRATE | DIOSTREAMUARTMASK_DATABITS | DIOSTREAMUARTMASK_PARITY |DIOSTREAMUARTMASK_STOPBITS | DIOSTREAMUARTMASK_FLOWCONTROL)

#define DIOSTREAMUART_FLOWCONTROL_STR_NONE    _L("NONE")
#define DIOSTREAMUART_FLOWCONTROL_STR_SOFT    _L("XON/XOFF")
#define DIOSTREAMUART_FLOWCONTROL_STR_HARD    _L("RTS/CTS")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMUARTCONFIG : public DIOSTREAMCONFIG
{
  public:
                                  DIOSTREAMUARTCONFIG               ();
    virtual                      ~DIOSTREAMUARTCONFIG               ();

    int                           GetPort                           ();
    void                          SetPort                           (int port);

    XSTRING*                      GetLocalDeviceName                ();

    int                           GetBaudRate                       ();
    void                          SetBaudRate                       (int baudrate);

    DIOSTREAMUARTDATABITS         GetDataBits                       ();
    void                          SetDataBits                       (DIOSTREAMUARTDATABITS databits);

    DIOSTREAMUARTPARITY           GetParity                         ();
    void                          SetParity                         (DIOSTREAMUARTPARITY parity);

    DIOSTREAMUARTSTOPBITS         GetStopBits                       ();
    void                          SetStopBits                       (DIOSTREAMUARTSTOPBITS stopbits);

    DIOSTREAMUARTFLOWCONTROL      GetFlowControl                    ();
    void                          SetFlowControl                    (DIOSTREAMUARTFLOWCONTROL flowcontrol);

    bool                          GetToString                       (XSTRING* string);
    bool                          GetToString                       (XSTRING& string);

    bool                          SetFromString                     (XCHAR* string);
    bool                          SetFromString                     (XSTRING* string);
    bool                          SetFromString                     (XSTRING& string);

  protected:

    int                           port;
    XSTRING                       devicename;
    int                           baudrate;
    DIOSTREAMUARTDATABITS         databits;
    DIOSTREAMUARTPARITY           parity;
    DIOSTREAMUARTSTOPBITS         stopbits;
    DIOSTREAMUARTFLOWCONTROL      flowcontrol;

  private:

    void                          Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




