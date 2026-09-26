/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamSPI.h
* 
* @class      DIOSTREAMSPI
* @brief      Data Input/Output Stream SPI class
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

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamSPIConfig.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum  DIOSTREAMSPI_FSMEVENTS
{
  DIOSTREAMSPI_FSMEVENT_NONE                 = 0  ,
  DIOSTREAMSPI_FSMEVENT_CONNECTED                 ,
  DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD             ,
  DIOSTREAMSPI_FSMEVENT_SENDINGDATA               ,
  DIOSTREAMSPI_FSMEVENT_DISCONNECTING             ,

  DIOSTREAMSPI_LASTEVENT
};

enum  DIOSTREAMSPI_FSMSTATES
{
  DIOSTREAMSPI_FSMSTATE_NONE                = 0  ,

  DIOSTREAMSPI_FSMSTATE_CONNECTED                ,
  DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD            ,
  DIOSTREAMSPI_FSMSTATE_SENDINGDATA              ,
  DIOSTREAMSPI_FSMSTATE_DISCONNECTING            ,

  DIOSTREAMSPI_LASTSTATE
};

#define DIOSTREAMSPI_MAXSIZEBUFFER        4*1024
#define DIOSTREAMSPI_SLEEP                Sleep(timedelay)
#define DIOSTREAMSPI_SLEEPDOUBLE          Sleep(timedelaydouble)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;
class DIOGPIO;
class DIOSTREAMSPICONFIG;

class DIOSTREAMSPI : public DIOSTREAM , public XFSMACHINE
{
  public:
                                  DIOSTREAMSPI                              ();
    virtual                      ~DIOSTREAMSPI                              ();

    DIOSTREAMCONFIG*              GetConfig                                 ();
    bool                          SetConfig                                 (DIOSTREAMCONFIG* config);

    virtual bool                  Open                                      ();
    virtual bool                  Close                                     ();
    virtual bool                  TransferBuffer                            (XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size);

    virtual void                  Sleep                                     (int count);

 protected:

    DIOSTREAMSPICONFIG*           config;
    XTIMER*                       xtimer;
    XBYTE*                        bufferread;

  private:

    void                          Clean                                     ();

    bool                          TransferOneData                           (XWORD writedata, XWORD& readdata);

    static void                   ThreadConnection                          (void* data);

    XTHREADCOLLECTED*             threadconnection;

    DIOGPIO_ENTRY*                GPIO_Entry[DIOSTREAMSPI_GPIO_MAX];
    
    XBYTE                         mode;
    XBYTE                         nbitsword;
    int                           timedelay;
    int                           timedelaydouble;
    bool                          CSline;
    bool                          lbsfirst;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





