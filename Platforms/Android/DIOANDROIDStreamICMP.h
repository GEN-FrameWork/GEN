/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamICMP.h
* 
* @class      DIOANDROIDSTREAMICMP
* @brief      ANDROID Data Input/Output Stream ICMP class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMICMP_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XFSMachine.h"

#include "DIOStreamICMP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOANDROIDICMPFSMEVENTS
{
  DIOANDROIDICMPFSMEVENT_NONE               = 0 ,

  DIOANDROIDICMPFSMEVENT_GETTINGCONNECTION      ,
  DIOANDROIDICMPFSMEVENT_CONNECTED              ,
  DIOANDROIDICMPFSMEVENT_WAITINGTOREAD          ,
  DIOANDROIDICMPFSMEVENT_SENDINGDATA            ,
  DIOANDROIDICMPFSMEVENT_DISCONNECTING          ,

  DIOANDROIDICMP_LASTEVENT
};

enum DIOANDROIDICMPFSMSTATES
{
  DIOANDROIDICMPFSMSTATE_NONE               = 0 ,

  DIOANDROIDICMPFSMSTATE_GETTINGCONNECTION      ,
  DIOANDROIDICMPFSMSTATE_CONNECTED              ,
  DIOANDROIDICMPFSMSTATE_WAITINGTOREAD          ,
  DIOANDROIDICMPFSMSTATE_SENDINGDATA            ,
  DIOANDROIDICMPFSMSTATE_DISCONNECTING          ,

  DIOANDROIDICMP_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREADCOLLECTED;


class DIOANDROIDSTREAMICMP : public DIOSTREAMICMP , public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMICMP                  ();
    virtual                  ~DIOANDROIDSTREAMICMP                  ();

    bool                      Open                                  ();
    bool                      Disconnect                            ();
    bool                      Close                                 ();

  protected:

    int                       IsReadyConnect                        (int sock);

  private:
   
    static void               ThreadRunFunction                     (void* thread); 
    void                      Clean                                 ();

    XTHREADCOLLECTED*         threadconnection;
    int                       handle;
    XSTRING                   remoteaddress;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



