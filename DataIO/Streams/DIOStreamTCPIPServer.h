/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTCPIPServer.h
* 
* @class      DIOSTREAMTCPIPSERVER
* @brief      Data Input/Output Stream TCP/IP Server class (Multi-Socket)
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

#include "XThreadCollected.h"

#include "DIOIP.h"
#include "DIOStream.h"
#include "DIOStreamTCPIP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMTCPIPSERVER_MAXLISTEN                        1024
#define DIOSTREAMTCPIPSERVER_TIMEOUTENUMSERVERSIGNAL          3

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTCPIPCONFIG;
class DIOSTREAMENUMSERVERS;

class DIOSTREAMTCPIPSERVER : public DIOSTREAM
{
  public:
                                            DIOSTREAMTCPIPSERVER                  ();
    virtual                                ~DIOSTREAMTCPIPSERVER                  ();

    virtual DIOSTREAMCONFIG*                GetConfig                             ();
    virtual bool                            SetConfig                             (DIOSTREAMCONFIG* config);

    virtual bool                            Open                                  ()                                = 0;
    virtual bool                            Disconnect                            ()                                = 0;
    virtual bool                            Close                                 ()                                = 0;

    bool                                    WaitToConnected                       (int timeout);

    DIOIP*                                  GetClientIP                           ();

    DIOSTREAMENUMSERVERS*                   GetEnumServers                        ();
    bool                                    SetEnumServers                        (DIOSTREAMENUMSERVERS* localenumservers);

    XVECTOR<DIOSTREAMTCPIP*>*               GetMultiSocketStreams                 (); 

    XDWORD                                  GetNumMultiSocketStreams              (); 
    XDWORD                                  GetNumConnectedMultiSocketStreams     (); 

  protected:

    DIOSTREAMTCPIPCONFIG*                   config;
    DIOIP                                   clientIP;

    DIOSTREAMENUMSERVERS*                   enumservers;

    XVECTOR<DIOSTREAMTCPIP*>                multisocketstreams;    

  private:

    void                                    Clean                     ();       
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





