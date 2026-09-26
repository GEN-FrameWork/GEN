/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODNSResolver.h
* 
* @class      DIODNSRESOLVER
* @brief      Data Input/Output Domain Network System (DNS) resolved class
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

#include "XFactory.h"
#include "XVector.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIODNSProtocol.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIODNSRESOLVER_HOSTRESOLVED
{
  public:
                                            DIODNSRESOLVER_HOSTRESOLVED     ();
    virtual                                ~DIODNSRESOLVER_HOSTRESOLVED     ();

    XSTRING*                                GetHost                         ();
    XSTRING*                                GetIP                           ();

  private:

    void                                    Clean                           ();

    XSTRING                                 host;
    XSTRING                                 IPresolved;
};

class DIODNSRESOLVER_DNSSERVER
{
  public:
                                            DIODNSRESOLVER_DNSSERVER        ();
    virtual                                ~DIODNSRESOLVER_DNSSERVER        ();

    DIOIP*                                  GetIP                           ();

    XWORD                                   GetPort                         ();
    void                                    SetPort                         (XWORD port);

  private:

    void                                    Clean                           ();

    DIOIP                                   IP;
    XWORD                                   port;
};

class DIODNSRESOLVER
{
  public:

    static bool                             GetIsInstanced                  ();
    static DIODNSRESOLVER&                  GetInstance                     ();
    static bool                             DelInstance                     ();

    bool                                    ResolveURL                      (XCHAR* URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);
    bool                                    ResolveURL                      (DIOURL& URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);
    bool                                    ResolveURL                      (XSTRING& URL, DIOIP& IPresolved, int querytype = DIODNSPROTOCOL_TYPEQUERY_A, XDWORD timeout = DIODNSPROTOCOL_DEFAULTTIMEOUT);

    bool                                    DNSServer_AddDNSServer          (XCHAR* server);
    bool                                    DNSServer_AddDNSServer          (XSTRING& server);
    bool                                    DNSServer_AddDNSServer          (XBYTE* serverIP, XWORD port);
    bool                                    DNSServer_AddDNSServer          (DIOIP& serverIP, XWORD port);
    bool                                    DNSServer_IsIPonList            (DIOIP& serverIP);
    XVECTOR<DIODNSRESOLVER_DNSSERVER*>*     DNSServer_GetList               ();
    bool                                    DNSServer_DeleteAllList         ();

    bool                                    Host_Add                        (XCHAR* host, XCHAR* IPresolved);
    bool                                    Host_Add                        (XSTRING& host, XSTRING& IPresolved);
    XVECTOR<DIODNSRESOLVER_HOSTRESOLVED*>*  Host_GetList                    ();
    bool                                    Host_FindIP                     (XCHAR* host, DIOIP& IPresolved);
    bool                                    Host_FindIP                     (XSTRING& host, DIOIP& IPresolved);
    bool                                    Host_DeleteAllList              ();

  private:
                                            DIODNSRESOLVER                  ();
                                            DIODNSRESOLVER                  (DIODNSRESOLVER const&);
    virtual                                ~DIODNSRESOLVER                  ();

    void                                    operator =                      (DIODNSRESOLVER const&);

    void                                    Clean                           ();

    static DIODNSRESOLVER*                  instance;

    XMUTEX*                                 xmutexresolved;    

    XVECTOR<DIODNSRESOLVER_DNSSERVER*>      listDNSservers;
    XVECTOR<DIODNSRESOLVER_HOSTRESOLVED*>   listhostresolved;    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





