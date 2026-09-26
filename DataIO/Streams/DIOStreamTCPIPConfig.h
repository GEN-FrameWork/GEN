/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTCPIPConfig.h
* 
* @class      DIOSTREAMTCPIPCONFIG
* @brief      Data Input/Output Stream TCP/IP Config class
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

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamConfig.h"
#include "DIOStreamConfigString.h"
#include "DIOStreamTCPIPProxy.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTCPIPCONFIG  : public DIOSTREAMCONFIG, public DIOSTREAMCONFIGSTRING
{
  public:
                            DIOSTREAMTCPIPCONFIG        ();
    virtual                ~DIOSTREAMTCPIPCONFIG        ();

    DIOIP*                  GetLocalIP                  ();
    DIOURL*                 GetRemoteURL                ();

    int                     GetRemotePort               ();
    bool                    SetRemotePort               (int port);

    DIOSTREAMTCPIPPROXYCFG* GetProxyCFG                 ();
    DIOURL*                 GetConnectionURL            ();
    int                     GetConnectionPort           ();

    int                     GetCounterMultiServer       ();
    void                    SetCounterMultiServer       (int countermultiserver);

    XMUTEX*                 GetXMutexMultiServer        ();

    int                     GetHandleMultiServer        ();
    void                    SetHandleMultiServer        (int handlemultiserver);

  protected:

    DIOIP                   localIP;
    DIOSTREAMTCPIPPROXYCFG  proxycfg;

    XMUTEX*                 xmutexmultiserver;
    int                     countermultiserver;
    int                     handlemultiserver;

  private:

    void                    Clean                       ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





