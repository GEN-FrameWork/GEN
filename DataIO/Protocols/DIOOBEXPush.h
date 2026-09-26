/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOOBEXPush.h
* 
* @class      DIOOBEXPUSH
* @brief      Data Input/Output Obex Push protocol class
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

#include "XBase.h"
#include "XBuffer.h"
#include "XString.h"
#include "XPath.h"
#include "XFSMachine.h"
#include "XThreadCollected.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


enum DIOOBEXPUSHERROR
{
  DIOOBEXPUSHERROR_NONE                 = 0 ,
  DIOOBEXPUSHERROR_NOTCONNECTION             ,
  DIOOBEXPUSHERROR_REFUSEDCONNECTION         ,
  DIOOBEXPUSHERROR_SENDING                  ,
};

enum DIOOBEXPUSHXFSMEVENTS
{
  DIOOBEXPUSH_XFSMEVENT_NONE            = 0 ,
  DIOOBEXPUSH_XFSMEVENT_CONNECT             ,
  DIOOBEXPUSH_XFSMEVENT_INIPUSH             ,
  DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH        ,
  DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH        ,
  DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH       ,
  DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH       ,
  DIOOBEXPUSH_XFSMEVENT_ENDPUSH             ,
  DIOOBEXPUSH_XFSMEVENT_DISCONNECT          ,

  DIOOBEXPUSH_LASTEVENT
};

enum DIOOBEXPUSHXFSMSTATES
{
  DIOOBEXPUSH_XFSMSTATE_NONE            = 0 ,
  DIOOBEXPUSH_XFSMSTATE_CONNECT             ,
  DIOOBEXPUSH_XFSMSTATE_INIPUSH             ,
  DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH        ,
  DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH        ,
  DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH       ,
  DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH       ,
  DIOOBEXPUSH_XFSMSTATE_ENDPUSH             ,
  DIOOBEXPUSH_XFSMSTATE_DISCONNECT          ,

  DIOOBEXPUSH_LASTSTATE
};

#define DIOOBEXPUSH_MAXBUFFER       65534   // Minimo evitar problemas en linux 38400
#define DIOOBEXPUSH_MAXTIMEOUTASK   30

#define DIOOBEXPUSH_MAXANSWER       32

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAM;

class DIOOBEXPUSHFILE
{
  public:
                                    DIOOBEXPUSHFILE                   ();
    virtual                        ~DIOOBEXPUSHFILE                   ();

    XDWORD                          GetID                             ();
    XPATH*                          GetXPath                          ();
    XSTRING*                        GetNameSend                       ();
    bool                            IsSend                            ();
    XBYTE                           GetSendPercent                    ();
    XQWORD                          GetSendTimeElapsed                ();
    DIOOBEXPUSHERROR                GetError                          ();

    bool                            SetID                             (XDWORD ID);
    bool                            SetIsSend                         (bool issend);
    bool                            SetSendPercent                    (XBYTE percent);
    bool                            SetSendTimeElapsed                (XQWORD sendtimeelapsed);
    bool                            SetError                          (DIOOBEXPUSHERROR error);

  private:

    void                            Clean                             ();

    XDWORD                          ID;
    XPATH                           xpath;
    XSTRING                         namesend;
    bool                            sendis;
    XBYTE                           sendpercent;
    XQWORD                          sendtimeelapsed;
    DIOOBEXPUSHERROR                error;
};

class DIOOBEXPUSH : public XFSMACHINE
{
  public:
                                    DIOOBEXPUSH                       (DIOSTREAM* diostream, bool inlittleendian);
    virtual                        ~DIOOBEXPUSH                       ();

    bool                            Ini                               (int timeout);

    bool                            IsSending                         ();
    bool                            SetIsSending                      (bool issending);

    bool                            SendFile                          (XPATH& xpath, XSTRING& namesend, XDWORD ID = 0, int asktimeout = DIOOBEXPUSH_MAXTIMEOUTASK);
    bool                            SendFiles                         (int asktimeout = DIOOBEXPUSH_MAXTIMEOUTASK);

    XTIMER*                         GetXTimer                         ();
    XQWORD                          GetAskTimeElapsed                 ();

    bool                            SendFileList_AddFile              (XPATH& xpath, XSTRING& namesend, XDWORD ID = 0);
    DIOOBEXPUSHFILE*                SendFileList_GetFile              (int index);
    DIOOBEXPUSHFILE*                SendFileList_GetActualFile        ();
    bool                            SendFileList_DeleteAll            ();
    int                             SendFileList_GetIndex             ();
    int                             SendFileList_GetSize              ();

    bool                            End                               ();

    DIOSTREAM*                      GetDIOStream                      ();

    XPATH*                          GetFileNameSend                   ();

    XDWORD                          GenerateFileID                    (XPATH& xpath);

  private:

    bool                            CheckSending                      ();
    bool                            ReadAnswer                        (XBUFFER& xbuffer);
    static void                     ThreadSend                        (void* data);

    void                            Clean                             ();

    DIOSTREAM*                      diostream;
    bool                            inlittleendian;

    XTIMER*                         xtimer;
    int                             asktimeout;
    XQWORD                          asktimeelapsed;

    XVECTOR<DIOOBEXPUSHFILE*>       sendfilelist;
    int                             indexfilelist;

    XPATH                           xpath;
    XFILE*                          xfile;

    XBUFFER*                        sendbuffer;

    bool                            issending;

    int                             sendblocksize;
    bool                            sendisstart;

    int                             filesize;
    int                             readfilesize;
    int                             readfilesizeblock;

    bool                            haveerror;

    XTHREADCOLLECTED*               xthreadobex;
};





