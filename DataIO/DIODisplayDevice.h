/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODisplayDevice.h
* 
* @class      DIODISPLAYDEVICE
* @brief      Data Input/Output Display Device (generic display device class)
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

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIODISPLAYDEVICE_INDEX_GPIOENTRYID
{
  DIODISPLAYDEVICE_INDEX_GPIOENTRYID_RESET            = 0 ,
  DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC                   ,
  DIODISPLAYDEVICE_INDEX_GPIOENTRYID_BACKLIGHT            ,

  DIODISPLAYDEVICE_INDEX_GPIOENTRYID_MAX  
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIODISPLAYDEVICE_CONFIG
{
  public:
                                  DIODISPLAYDEVICE_CONFIG         ();
    virtual                      ~DIODISPLAYDEVICE_CONFIG         ();

    int                           GetWidth                        ();
    int                           GetHeight                       ();
    XDWORD                        GetNPixels                      ();
    XDWORD                        GetSizeBuffer                   ();
   
  protected:

    XDWORD                        width;
    XDWORD                        height;
    XDWORD                        sizebuffer;
    bool                          isdirectconnection;

  private:

    void                          Clean                           ();
};


class DIODISPLAYDEVICE : public DIODISPLAYDEVICE_CONFIG
{
  public:
                                  DIODISPLAYDEVICE                ();
    virtual                      ~DIODISPLAYDEVICE                ();

    XDWORD                        GetGPIOEntryID                  (DIODISPLAYDEVICE_INDEX_GPIOENTRYID index);
    bool                          SetGPIOEntryID                  (DIODISPLAYDEVICE_INDEX_GPIOENTRYID index, XDWORD GPIOentryID);

    virtual bool                  SetActiveBlackLight             (bool active);

  protected:

    XBUFFER                       canvasbuffer;

  private:

    void                          Clean                           ();    

    XDWORD                        GPIOentrysID[DIODISPLAYDEVICE_INDEX_GPIOENTRYID_MAX];            
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





