/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODNSProtocol_Client.h
* 
* @class      DIODNSPROTOCOL_CLIENT
* @brief      Data Input/Output Domain Network System (DNS) protocol Client class
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

#include "XVector.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIODNSProtocol.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;

class DIODNSPROTOCOLCLIENT
{
  public:
                                            DIODNSPROTOCOLCLIENT            ();
    virtual                                ~DIODNSPROTOCOLCLIENT            ();

    bool                                    SetServer                       (DIOIP& serverIP, XWORD serverport = DIODNSPROTOCOL_DEFAULTPORT);
    bool                                    SetServer                       (XCHAR* serverIP, XWORD serverport = DIODNSPROTOCOL_DEFAULTPORT);
    bool                                    SetServer                       (XSTRING& IPserver, XWORD serverport = DIODNSPROTOCOL_DEFAULTPORT);

    bool                                    ResolveURL                      (XCHAR* URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);
    bool                                    ResolveURL                      (DIOURL& URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);
    bool                                    ResolveURL                      (XSTRING& URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);

    static bool                             EncodeDNSFormat                 (XSTRING& URL, XBUFFER& EncodedURL);
    static bool                             DecodeDNSFormat                 (XBUFFER& EncodedURL, XSTRING& URL);
    static bool                             DecodeDNSFormat                 (XBYTE* EncodedURL, XSTRING& URL); 

    static XBYTE*                           GetBufferName                   (XBYTE* reader, XBYTE* buffer, int* count);

  private:

    void                                    Clean                           ();

    DIOSTREAMUDPCONFIG*                     diostreamudpcfg;
    DIOSTREAMUDP*                           diostreamudp;

    DIOIP                                   serverIP;
    XWORD                                   serverport;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




