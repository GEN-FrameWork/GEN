/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamI2CConfig.h
* 
* @class      DIOSTREAMI2CCONFIG
* @brief      Data Input/Output Stream I2C Config class
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

enum DIOSTREAMI2CCONFIG_ADDRESSSIZE
{
  DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT    = 0 ,
  DIOSTREAMI2CCONFIG_ADDRESSSIZE_7BITS          ,
  DIOSTREAMI2CCONFIG_ADDRESSSIZE_10BITS         ,
};

#define DIOSTREAMI2CCONFIG_ADDRESSINVALID     -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMI2CCONFIG : public DIOSTREAMCONFIG
{
  public:
                                      DIOSTREAMI2CCONFIG                ();
    virtual                          ~DIOSTREAMI2CCONFIG                ();

    int                               GetPort                           ();
    void                              SetPort                           (int port);

    XSTRING*                          GetLocalDeviceName                ();
    bool                              SetLocalDeviceNameByPort          ();

    int                               GetLocalAddress                   ();
    void                              SetLocalAddress                   (int localaddress);

    int                               GetRemoteAddress                  ();
    void                              SetRemoteAddress                  (int remoteaddress);

    DIOSTREAMI2CCONFIG_ADDRESSSIZE    GetAddressSize                    ();
    void                              SetAddressSize                    (DIOSTREAMI2CCONFIG_ADDRESSSIZE addresssize);
                                    
    bool                              IsStretchActive                    ();
    void                              SetIsStretchActive                 (bool isstretchactive);

  protected:

    int                               port;
    XSTRING                           localdevicename;
    int                               localaddress;
    int                               remoteaddress;
    DIOSTREAMI2CCONFIG_ADDRESSSIZE    addresssize;
    bool                              isstretchactive;

  private:

    void                              Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





