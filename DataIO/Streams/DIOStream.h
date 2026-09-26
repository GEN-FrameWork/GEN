/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStream.h
* 
* @class      DIOSTREAM
* @brief      Data Input/Output Stream class
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

#include "XSleep.h"
#include "XUUID.h"
#include "XString.h"
#include "XSubject.h"
#include "XTimer.h"

#include "DIOStreamConfig.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMSTATUS
{
  DIOSTREAMSTATUS_DISCONNECTED          = 0 ,
  DIOSTREAMSTATUS_CONNECTED                 ,
  DIOSTREAMSTATUS_GETTINGCONNECTION         ,
};

enum DIOSTREAMERROR
{
  DIOSTREAMERROR_NONE                   = 0 ,
  DIOSTREAMERROR_UNKNOWN                    ,
  DIOSTREAMERROR_URLNOTVALID                ,
  DIOSTREAMERROR_ADDRINUSE                  ,
  DIOSTREAMERROR_TLSAUTHENTICATION          ,
  DIOSTREAMERROR_TCPIPPROXY                 ,
  DIOSTREAMERROR_DNS                        ,
  DIOSTREAMERROR_TCPCONNECTION              ,
  DIOSTREAMERROR_TCPTIMEOUT                 ,
  DIOSTREAMERROR_TLSCONFIGURATION           ,
  DIOSTREAMERROR_TLSPROTOCOL                ,
};

#ifndef MICROCONTROLLER
#define DIOSTREAM_MAXBUFFER             1024*10  // 10k
#else
#define DIOSTREAM_MAXBUFFER             32
#endif

#define DIOSTREAM_SOMETHINGTOREAD       -1

#define DIOSTREAM_TIMEINWAITFUNCTIONS   10       // milliseconds

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;
class DIOSTREAMCONFIG;
class CIPHER;

class DIOSTREAM : public XSUBJECT
{
  public:
                                DIOSTREAM                       ();
    virtual                    ~DIOSTREAM                       ();

    virtual DIOSTREAMCONFIG*    GetConfig                       ()                            = 0;
    virtual bool                SetConfig                       (DIOSTREAMCONFIG* config)     = 0;

    DIOSTREAMSTATUS             GetStatus                       ();
    void                        SetStatus                       (DIOSTREAMSTATUS status);

    virtual bool                Open                            ()                            = 0;

    bool                        IsConnected                     ();
    bool                        IsDisconnected                  ();

    virtual bool                WaitToConnected                 (int timeout);
    virtual bool                WaitToDisconnected              (int timeout);

    virtual XDWORD              Read                            (XBYTE* buffer, XDWORD size);
    virtual XDWORD              Write                           (XBYTE* buffer, XDWORD size);

    virtual XDWORD              Read                            (XBUFFER& xbuffer);
    XDWORD                      Write                           (XBUFFER& xbuffer);

    virtual bool                WaitToFilledReadingBuffer       (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);

    bool                        ReadStr                         (char* str);
    bool                        ReadStr                         (XSTRING& string);
    bool                        ReadStr                         (XSTRING& string, int timeout);

    bool                        WriteStr                        (const char* str);
    bool                        WriteStr                        (XCHAR* string);
    bool                        WriteStr                        (XSTRING& string);

    bool                        IsBlockRead                     ();
    void                        SetIsBlockRead                  (bool isblockread);

    bool                        IsBlockWrite                    ();
    void                        SetIsBlockWrite                 (bool isblockwrite);

    virtual bool                Disconnect                      ();

    virtual bool                Close                           ()                          = 0;

    DIOSTREAMTYPE               GetType                         ();
    void                        SetType                         (DIOSTREAMTYPE type);

    virtual XBUFFER*            GetInXBuffer                    ();
    XBUFFER*                    GetOutXBuffer                   ();

    bool                        ResetXBuffers                   ();
    bool                        ResetInXBuffer                  ();
    bool                        ResetOutXBuffer                 ();

    bool                        IsFlushOutXBuffer               ();
    bool                        IsFlushXBuffers                 ();

    bool                        WaitToFlushOutXBuffer           (int timeout = XTIMER_INFINITE, bool mutexactive = true);
    bool                        WaitToFlushXBuffers             (int timeout = XTIMER_INFINITE, bool mutexactive = true);

    virtual XTIMER*             GetXTimerConnection             ();
    virtual XTIMER*             GetXTimerNotActivity            ();

    XDWORD                      GetNBytesRead                   ();
    void                        AddNBytesRead                   (XDWORD size);

    XDWORD                      GetNBytesWrite                  ();
    void                        AddNBytesWrite                  (XDWORD size);

    bool                        ResetConnectionStatistics       ();

    DIOSTREAMERROR              GetLastDIOError                 ();
    DIOSTREAMERROR              PeekLastDIOError                ();
    void                        SetLastDIOError                 (DIOSTREAMERROR error);

    void                        Wait                            (int milliseconds);

    XBUFFER*                    inbuffer;
    XBUFFER*                    outbuffer;

  protected:

    DIOSTREAMTYPE               type;

    DIOSTREAMSTATUS             status;
    DIOSTREAMERROR              error;

    XTIMER*                     xtimerconnection;
    XTIMER*                     xtimernotactivity;
    XTIMER*                     xtimerout;

    XMUTEX*                     xmutextimerout;

    bool                        isblockread;
    bool                        isblockwrite;

    XDWORD                      nbytesread;
    XDWORD                      nbyteswrite;

  private:

    void                        Clean                           ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



