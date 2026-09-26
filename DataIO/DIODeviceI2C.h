/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODeviceI2C.h
* 
* @class      DIODEVICEI2C
* @brief      Data Input/Output device I2C class
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

#include "DIODevice.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;

class DIODEVICEI2C : public DIODEVICE
{
  public:
  
    using DIODEVICE::Ini;

                              DIODEVICEI2C          ();                            
    virtual                  ~DIODEVICEI2C          ();    
    
    bool                      Ini                   (XCHAR* localdevicename, int remotedeviceaddress, int timeout);
    bool                      Ini                   (int port, int remotedeviceaddress, int timeout);                          

    DIOSTREAMI2CCONFIG*       CreateConfig          (XCHAR* localdevicename, int remotedeviceaddress);    
    DIOSTREAMI2CCONFIG*       CreateConfig          (int port, int remotedeviceaddress);

    virtual bool              IniDevice             ();

    bool                      End                   ();

  protected:

    int                       timeout;
    XTIMER*                   xtimerout;
    DIOSTREAMI2CCONFIG*       diostreamcfg;
    DIOSTREAMI2C*             diostream;

  private:

    void                      Clean                 ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





