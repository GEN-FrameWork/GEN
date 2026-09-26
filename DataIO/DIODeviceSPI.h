/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODeviceSPI.h
* 
* @class      DIODEVICESPI
* @brief      Data Input/Output device SPI class
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

#include "DIODevice.h"
#include "DIOStreamSPIConfig.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMSPICONFIG;
class DIOSTREAMSPI;

class DIODEVICESPI : public DIODEVICE
{
  public:    
  
    using DIODEVICE::Ini;

                              DIODEVICESPI          ();   
    virtual                  ~DIODEVICESPI          ();                              

    bool                      Ini                   (XCHAR* localdevicename, int timeout);
    bool                      Ini                   (int port, int chipselect, int timeout);
                              
    DIOSTREAMSPICONFIG*       CreateConfig          (XCHAR* localdevicename);                              
    DIOSTREAMSPICONFIG*       CreateConfig          (int port, int chipselect);
    
    virtual bool              IniDevice             ();

    bool                      End                   ();

  protected:

    int                       timeout;
    XTIMER*                   xtimerout;
    DIOSTREAMSPICONFIG*       diostreamcfg;
    DIOSTREAMSPI*             diostream;

  private:

    void                      Clean                 ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





