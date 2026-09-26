/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStream_XEvent.h
* 
* @class      DIOSTREAM_XEVENT
* @brief      Data Input/Output eXtended Event class
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


#include "XEvent.h"

#include "DIOStreamDevice.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOSTREAM_XEVENT_TYPE
{
  DIOSTREAM_XEVENT_TYPE_UNKNOWN             = XEVENT_TYPE_DIOSTREAM  ,
  DIOSTREAM_XEVENT_TYPE_GETTINGCONNECTION                            ,
  DIOSTREAM_XEVENT_TYPE_CONNECTED                                   ,
  DIOSTREAM_XEVENT_TYPE_DISCONNECTED                                ,
  DIOSTREAM_XEVENT_TYPE_EXCEPTIONDEVICE                             ,
  DIOSTREAM_XEVENT_TYPE_COOPERATIONDEVICE                           ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOSTREAM;


class DIOSTREAM_XEVENT : public XEVENT
{
  public:
                                DIOSTREAM_XEVENT      (XSUBJECT* subject, XDWORD type = DIOSTREAM_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_DIOSTREAM);
                               
    virtual                    ~DIOSTREAM_XEVENT      ();

    DIOSTREAM*                  GetDIOStream          ();
    void                        SetDIOStream          (DIOSTREAM* diostream);

    DIOSTREAMDEVICE*            GetDevice             ();
    void                        SetDevice             (DIOSTREAMDEVICE& device);

  private:

    void                        Clean                 ();
                                
    DIOSTREAM*                  diostream;
    DIOSTREAMDEVICE             device;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





