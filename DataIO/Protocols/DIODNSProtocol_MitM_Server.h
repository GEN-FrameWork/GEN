/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIODNSProtocol_MitM_Server.h
*
* @class      DIODNSPROTOCOL_MITM_SERVER
* @brief      Data Input/Output Domain Network System (DNS) protocol Man-in-the-Middle (MitM) Server class
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

#include "XSubject.h"

#include "DIOStreamUDPConfig.h"
#include "DIODNSResolver.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREAD;
class XMUTEX;  
class DIOSTREAMUDP;

class DIODNSPROTOCOL_MITM_SERVER : public XSUBJECT
{
  public:
                                        DIODNSPROTOCOL_MITM_SERVER      ();
    virtual                            ~DIODNSPROTOCOL_MITM_SERVER      ();

    bool                                Ini                             ();
    bool                                Activate                        (bool active);   
    bool                                Update                          ();
    bool                                End                             ();

    bool                                AddDNSServer                    (XCHAR* server, XWORD port = DIODNSPROTOCOL_DEFAULTPORT);
    bool                                AddDNSServer                    (XSTRING& server, XWORD port = DIODNSPROTOCOL_DEFAULTPORT);
    bool                                AddDNSServer                    (XBYTE* serverIP, XWORD port = DIODNSPROTOCOL_DEFAULTPORT);
    bool                                AddDNSServer                    (DIOIP& serverIP, XWORD port = DIODNSPROTOCOL_DEFAULTPORT);

    bool                                DeleteAllDNSServers             ();

    XMUTEX*                             GetServerMutex                  ();  
 
  private:

    bool                                Detour                          (DIODNSRESOLVER_DNSSERVER* DNSserver, XBUFFER& sendbuffer, XBUFFER& receivedbuffer);

    static void                         ThreadServer                    (void* param);

    void                                Clean                           ();

    XTHREAD*                            serverthread;
    XMUTEX*                             servermutex;  

    DIOSTREAMUDPCONFIG                  diostreamudpcfg;
		DIOSTREAMUDP*				                diostreamudp;

    XVECTOR<DIODNSRESOLVER_DNSSERVER*>  dnsservers;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


