/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTCPIPProxy.h
* 
* @class      DIOSTREAMTCPIPPROXYCFG
* 
* @class      DIOSTREAMTCPIPPROXY
* @brief      Data Input/Output proxy configuration and tunnel negotiation classes
* @ingroup    DATAIO
* 
* @copyright  EndoraSoft. All rights reserved.
* 
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
#include "DIOURL.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOSTREAMTCPIPPROXYTYPE
{
  DIOSTREAMTCPIPPROXYTYPE_NONE                  = 0 ,
  DIOSTREAMTCPIPPROXYTYPE_HTTP                      ,
};


enum DIOSTREAMTCPIPPROXYMODE
{
  DIOSTREAMTCPIPPROXYMODE_NONE                  = 0 ,
  DIOSTREAMTCPIPPROXYMODE_FORWARD                   ,
  DIOSTREAMTCPIPPROXYMODE_TUNNEL                    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAM;

class DIOSTREAMTCPIPPROXYCFG
{
  public:
                              DIOSTREAMTCPIPPROXYCFG          ();
    virtual                  ~DIOSTREAMTCPIPPROXYCFG          ();

    DIOSTREAMTCPIPPROXYTYPE  GetType                         ();
    void                      SetType                         (DIOSTREAMTCPIPPROXYTYPE type);

    DIOSTREAMTCPIPPROXYMODE  GetMode                         ();
    void                      SetMode                         (DIOSTREAMTCPIPPROXYMODE mode);

    bool                      IsActive                        ();

    DIOURL*                   GetURL                          ();

    int                       GetPort                         ();
    void                      SetPort                         (int port);

    XSTRING*                  GetLogin                        ();
    XSTRING*                  GetPassword                     ();

  private:

    void                      Clean                           ();

    DIOSTREAMTCPIPPROXYTYPE  type;
    DIOSTREAMTCPIPPROXYMODE  mode;
    DIOURL                    url;
    int                       port;
    XSTRING                   login;
    XSTRING                   password;
};

class DIOSTREAMTCPIPPROXY
{
  public:
                              DIOSTREAMTCPIPPROXY             ();
    virtual                  ~DIOSTREAMTCPIPPROXY             ();

    bool                      Connect                         (DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout);

  private:

    bool                      ConnectHTTP                     (DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout);
};

