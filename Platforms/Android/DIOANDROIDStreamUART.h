/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamUART.h
* 
* @class      DIOANDROIDSTREAMUART
* @brief      ANDROID Data Input/Output Stream UART class
* @ingroup    PLATFORM_ANDROID
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XThreadCollected.h"

#include "DIOStreamUART.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOANDROIDUARTFSMEVENTS
{
  DIOANDROIDUARTFSMEVENT_NONE              = 0  ,
  DIOANDROIDUARTFSMEVENT_CONNECTED              ,
  DIOANDROIDUARTFSMEVENT_WAITINGTOREAD          ,
  DIOANDROIDUARTFSMEVENT_DISCONNECTING          ,

  DIOANDROIDUART_LASTEVENT
};

enum DIOANDROIDUARTFSMSTATES
{
  DIOANDROIDUARTFSMSTATE_NONE              = 0  ,

  DIOANDROIDUARTFSMSTATE_CONNECTED              ,
  DIOANDROIDUARTFSMSTATE_WAITINGTOREAD          ,
  DIOANDROIDUARTFSMSTATE_DISCONNECTING          ,

  DIOANDROIDUART_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOANDROIDSTREAMUART : public DIOSTREAMUART , public XFSMACHINE
{
  public:
                          DIOANDROIDSTREAMUART          ();
    virtual              ~DIOANDROIDSTREAMUART          ();

    DIOSTREAMSTATUS       GetStatus              ();

    bool                  Open                          ();

    bool                  Config                        (XWORD mask = DIOSTREAMUARTMASK_ALL);

    XDWORD                ReadDirect                    (XBYTE* buffer, XDWORD size);
    XDWORD                WriteDirect                   (XBYTE* buffer, XDWORD size);

    bool                  Disconnect                    ();
    bool                  Close                         ();

    bool                  GetCTS                        ();
    bool                  GetDSR                        ();
    bool                  GetRing                       ();
    bool                  GetRLSD                       ();

    bool                  SetRTS                        (bool on = true);
    bool                  SetDTR                        (bool on = true);

    bool                  CleanBuffers                  ();

  protected:

    void                  Clean                         ();

    static void           ThreadConnection              (void* param);

    XTHREADCOLLECTED*     threadconnection;

    int                   fd;

    int                   readtimeout;
    int                   writetimeout;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif







