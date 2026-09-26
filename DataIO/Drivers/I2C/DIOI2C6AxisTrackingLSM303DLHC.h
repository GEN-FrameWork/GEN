/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2C6AxisTrackingLSM303DLHC.h
* 
* @class      DIOI2C6AXISTRACKINGLSM303DLHC
* @brief      Data Input/Output I2C 6 axis tracking LSM303DLHC class
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
#include <cmath>

#include "XBase.h"

#include "DIODeviceI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum REG_ADDRS
{
  // ----- Compass -------------------------------

  DIOI2CLSM303DLHC_CRA_REG_M    = 0x00,
  DIOI2CLSM303DLHC_CRB_REG_M    = 0x01,
  DIOI2CLSM303DLHC_MR_REG_M     = 0x02,
  DIOI2CLSM303DLHC_OUT_X_M      = 0x03,
  DIOI2CLSM303DLHC_OUT_Y_M      = 0x05,
  DIOI2CLSM303DLHC_OUT_Z_M      = 0x07,

  // ----- Accelerometer -------------------------

  DIOI2CLSM303DLHC_CTRL_REG1_A  = 0x20,
  DIOI2CLSM303DLHC_CTRL_REG4_A  = 0x23,
  DIOI2CLSM303DLHC_OUT_X_A      = 0x28,
  DIOI2CLSM303DLHC_OUT_Y_A      = 0x2A,
  DIOI2CLSM303DLHC_OUT_Z_A      = 0x2C,
};

#define DIOI2CLSM303DLHC_ADDR_ACCELEROMETER         0x18
#define DIOI2CLSM303DLHC_ADDR_COMPASS               0x1e

#define DIOI2CLSM303DLHC_FILTERSHIFT    6

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XTIMER;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;

class DIOI2CLSM303DLHCVECTOR
{
  public:
                                        DIOI2CLSM303DLHCVECTOR                      ();
    virtual                            ~DIOI2CLSM303DLHCVECTOR                      ();

    static void                         Cross                                       (const DIOI2CLSM303DLHCVECTOR& a, const DIOI2CLSM303DLHCVECTOR& b, DIOI2CLSM303DLHCVECTOR& out);
    static float                        Dot                                         (const DIOI2CLSM303DLHCVECTOR& a, const DIOI2CLSM303DLHCVECTOR& b);
    static void                         Normalize                                   (DIOI2CLSM303DLHCVECTOR& a);

    float                               x;
    float                               y;
    float                               z;

  private:

    void                                Clean                                       ();
};


class DIOI2CLSM303DLHC : public DIODEVICEI2C
{
  public:

                                        DIOI2CLSM303DLHC                            ();
    virtual                            ~DIOI2CLSM303DLHC                            ();

    bool                                End                                         ();

  protected:

    virtual bool                        IniDevice                                   ();


    bool                                ReadRegister                                (XBYTE reg, XBYTE& value);
    bool                                ReadRegister                                (XBYTE reg, XWORD& value);
    bool                                WriteRegister                               (XBYTE reg, XBYTE value);

    void                                Clean                                       ();

};


class DIOI2C3DCOMPASSLSM303DLHC : public DIOI2CLSM303DLHC
{
  public:

                                        DIOI2C3DCOMPASSLSM303DLHC                   ();
    virtual                            ~DIOI2C3DCOMPASSLSM303DLHC                   ();

    bool                                Configure                                   ();

    bool                                Read                                        (XWORD& c_x, XWORD& c_y, XWORD& c_z);
};

class DIOI2C3DACCELEROMETERLSM303DLHC : public DIOI2CLSM303DLHC
{
  public:

                                        DIOI2C3DACCELEROMETERLSM303DLHC             ();
    virtual                            ~DIOI2C3DACCELEROMETERLSM303DLHC             ();

    bool                                Configure                                   ();

    bool                                Read                                        (XWORD& a_x, XWORD& a_y, XWORD& a_z);
};

class DIOI2C6AXISTRACKINGLSM303DLHC
{
  public:

                                        DIOI2C6AXISTRACKINGLSM303DLHC               ();
    virtual                            ~DIOI2C6AXISTRACKINGLSM303DLHC               ();

    bool                                Ini                                         (XCHAR* localdevicename, int remotedeviceaddressaccelerometer, int remotedeviceaddresscompass, int timeout);
    bool                                Ini                                         (int port, int remotedeviceaddressaccelerometer, int remotedeviceaddresscompass, int timeout);

    void                                SetOffset                                   (float x, float y, float z);
    void                                SetScale                                    (float x, float y, float z);

    bool                                Read                                        (XWORD& a_x, XWORD& a_y, XWORD& a_z, XWORD& c_x, XWORD& c_y, XWORD& c_z);

    bool                                Read                                        (DIOI2CLSM303DLHCVECTOR& a, DIOI2CLSM303DLHCVECTOR& c);

    float                               Heading                                     ();

    float                               Heading                                     (DIOI2CLSM303DLHCVECTOR& from);
    bool                                End                                         ();

  private:

    void                                Clean                                       ();


    DIOI2C3DACCELEROMETERLSM303DLHC*    accelerometer;
    DIOI2C3DCOMPASSLSM303DLHC*          compass;

    float                               offset_x;
    float                               offset_y;
    float                               offset_z;

    float                               scale_x;
    float                               scale_y;
    float                               scale_z;

    long                                filter_ax;
    long                                filter_ay;
    long                                filter_az;

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






