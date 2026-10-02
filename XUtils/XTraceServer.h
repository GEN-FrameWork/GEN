/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTraceServer.h
* 
* @class      XTRACESERVER
* @brief      XTRACE receive server (UDP/UART) with bounded thread-safe queue
* @ingroup    XUTILS
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

#include "XBase.h"
#include "XString.h"
#include "XDateTime.h"
#include "XVector.h"
#include "XThreadCollected.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))

#define XTRACESERVER_DEFAULT_MAXMESSAGES      4096
#define XTRACESERVER_DEFAULT_PORT             10001

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;
class XBUFFER;

#if defined(DIO_STREAMUDP_ACTIVE)
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;
#endif

#if defined(DIO_STREAMUART_ACTIVE)
class DIOSTREAMUARTCONFIG;
class DIOSTREAMUART;
#endif


class XTRACESERVER_MSG
{
  public:

                            XTRACESERVER_MSG                ();
    virtual                ~XTRACESERVER_MSG                ();

    bool                    CopyFrom                        (XTRACESERVER_MSG* origin);
    bool                    CopyFrom                        (XTRACESERVER_MSG& origin);

    XDWORD                  publicIP;
    XDWORD                  localIP;
    XBYTE                   level;
    XDWORD                  sequence;
    XDATETIME               time;
    XSTRING                 text;

  private:

    void                    Clean                           ();
};


class XTRACESERVER
{
  public:

                            XTRACESERVER                    ();
    virtual                ~XTRACESERVER                    ();

  #if defined(DIO_STREAMUDP_ACTIVE)
    bool                    Ini                             (XWORD port = XTRACESERVER_DEFAULT_PORT);
    bool                    Ini                             (XCHAR* config);
    bool                    Ini                             (XSTRING& config);
    bool                    Ini                             (XSTRING* config);
  #endif

  #if defined(DIO_STREAMUART_ACTIVE)
    bool                    IniUART                         (XCHAR* config);
    bool                    IniUART                         (XSTRING& config);
    bool                    IniUART                         (XSTRING* config);
  #endif

    bool                    End                             ();

    bool                    IsOpen                          ();
  #if defined(DIO_STREAMUDP_ACTIVE)
    bool                    IsOpenUDP                       ();
  #endif
  #if defined(DIO_STREAMUART_ACTIVE)
    bool                    IsOpenUART                      ();
  #endif

    XDWORD                  GetMaxMessages                  ();
    void                    SetMaxMessages                  (XDWORD maxmessages);

    XDWORD                  GetCount                        ();
    XDWORD                  GetDroppedCount                 ();

    bool                    Clear                           ();

    bool                    Pop                             (XTRACESERVER_MSG& msg);
    bool                    Peek                            (XDWORD index, XTRACESERVER_MSG& msg);

    bool                    WaitPop                         (XTRACESERVER_MSG& msg, int timeout_ms = -1);

  private:

  #if defined(DIO_STREAMUDP_ACTIVE)
    bool                    OpenUDP                         (XSTRING* config);
    bool                    CloseUDP                        ();
    static void             ThreadReadUDPFunction           (void* param);
  #endif

  #if defined(DIO_STREAMUART_ACTIVE)
    bool                    OpenUART                        (XSTRING* config);
    bool                    CloseUART                       ();
    static void             ThreadReadUARTFunction          (void* param);
  #endif

    bool                    PushParsedPacket                (XBUFFER& packet, XDWORD publicIP, XDWORD localIP);
    bool                    PushMessage                     (XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& text);

    void                    Clean                           ();

    XDWORD                  maxmessages;
    XDWORD                  droppedcount;

    XMUTEX*                 xmutexmessages;
    XVECTOR<XTRACESERVER_MSG*>  messages;

  #if defined(DIO_STREAMUDP_ACTIVE)
    bool                    isopenudp;
    DIOSTREAMUDPCONFIG*     diostreamudpcfg;
    DIOSTREAMUDP*           diostreamudp;
    XTHREADCOLLECTED*       xthreadreadUDP;
  #endif

  #if defined(DIO_STREAMUART_ACTIVE)
    bool                    isopenuart;
    DIOSTREAMUARTCONFIG*    diostreamuartcfg;
    DIOSTREAMUART*          diostreamuart;
    XTHREADCOLLECTED*       xthreadreadUART;
  #endif
};


#endif

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
