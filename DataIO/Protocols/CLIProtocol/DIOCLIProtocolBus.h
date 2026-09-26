/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCLIProtocolBus.h
* 
* @class      DIOCLIPROTOCOLBUS
* @brief      Data Input/Output protocol in CLI Bus class
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
#include "XVector.h"
#include "XThreadCollected.h"

#include "DIOCLIProtocol.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOCLIPROTOCOLBUS_ENUM_DEFAULTMAXTIME     30
#define DIOCLIPROTOCOLBUS_SEND_DEFAULNRETRIES     3

#define DIOCLIPROTOCOLBUS_COMMAND_VERSION			    __L("version")
#define DIOCLIPROTOCOLBUS_COMMAND_PING   			    __L("ping")
#define DIOCLIPROTOCOLBUS_COMMAND_ENUM  			    __L("enum")
#define DIOCLIPROTOCOLBUS_COMMAND_ENUMREQUEST     __L("enumrequest")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOCLIPROTOCOLBUS : public DIOCLIPROTOCOL
{
  public:
                            DIOCLIPROTOCOLBUS             ();
    virtual                ~DIOCLIPROTOCOLBUS             ();
    
    bool                    Ini                           (DIOSTREAM* diostream, XCHAR* ID, int timeout = DIOCLIPROTOCOL_TIMEOUT);

    bool                    GetVersion                    (XDWORD& version, XDWORD& subversion, XDWORD& subversionerror);
    void                    SetVersion                    (XDWORD version, XDWORD subversion, XDWORD subversionerror);    
   
    bool                    EnumRemoteDevices             (XVECTOR<XSTRING*>* remotedevices = NULL, XDWORD maxtime = DIOCLIPROTOCOLBUS_ENUM_DEFAULTMAXTIME);
    bool                    GetEnumRemoteDevices          (XVECTOR<XSTRING*>& remotedevices);
    
    int                     GetNRetries                   ();
    void                    SetNRetries                   (int nretries = DIOCLIPROTOCOLBUS_SEND_DEFAULNRETRIES);

    bool                    SendCommand                   (XCHAR* command, XSTRING* target, XSTRING* answer, int timeoutanswer, ...);

    virtual bool            ReceivedCommand               (XSTRING& originID, XSTRING& command, XVECTOR<XSTRING*>& params, XSTRING& answer);
    
    void                    End                           ();
   
  private:

	  static void						  ThreadReceivedCommand         (void* param);
    static void             ThreadSendEnumRequest         (void* param);
    
    void                    Clean                         ();

    XDWORD                  version;
    XDWORD                  subversion;
    XDWORD                  subversionerror;

    XDWORD                  nretries;

    bool                    exitproccess;  

    XMUTEX*                 enum_mutex;
    XVECTOR<XSTRING*>       enum_remotedevices;
    XSTRING                 enum_sendoriginID;
    XTIMER*                 enum_timer;
    XDWORD                  enum_maxtimersec;

    XMUTEX*                 xmutexsendcommand;    

    XTHREADCOLLECTED*				threadreceivedcommand;
    XTHREADCOLLECTED*				threadsendenumrequest;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




