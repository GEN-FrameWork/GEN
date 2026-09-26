/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODynDNS.h
* 
* @class      DIODYNDNS
* @brief      Data Input/Output DynDNS API protocol class
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

#include <stdio.h>

#include "XString.h"

#include "DIOURL.h"
#include "DIOIP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIODYNDNS_STATUSCODE
{
  DIODYNDNS_STATUSCODE_UNKNOWN       = -2 ,   // Unknown result from dyndns service.
  DIODYNDNS_STATUSCODE_NOTCONNECT         ,   // Not connected.
  DIODYNDNS_STATUSCODE_GOOD               ,   // "good"         The update was successful, and the hostname is now updated.
  DIODYNDNS_STATUSCODE_NOCHG              ,   // "nochg"        The update changed no settings, and is considered abusive. (Additional nochg updates will cause the hostname to become blocked).
  DIODYNDNS_STATUSCODE_BADSYS             ,   // "badsys"       The system parameter given is not valid. Valid system parameters are dyndns, statdns and custom.
  DIODYNDNS_STATUSCODE_BASAGENT           ,   // "badagent"     The user agent that was sent has been blocked for not following these specifications or no user agent was specified.
  DIODYNDNS_STATUSCODE_BADAUTH            ,   // "badauth"      The username or password specified are incorrect.
  DIODYNDNS_STATUSCODE_DONATOR            ,   // "!donator"     An option available only to credited users (suchas offline URL) was specified, but the user is not a credited user.
  DIODYNDNS_STATUSCODE_NOTFQDN            ,   // "notfqdn"      The hostname specified is not a fully-qualified domain name (not in the form hostname.dyndns.org or domain.com).
  DIODYNDNS_STATUSCODE_NOHOST             ,   // "nohost"       The hostname specified does not exist (or is not in the service specified in the system parameter).
  DIODYNDNS_STATUSCODE_YOURS              ,   // "!yours"       The hostname specified exists, but not under the username specified.
  DIODYNDNS_STATUSCODE_ABUSE              ,   // "abuse"        The hostname specified is blocked for update abuse.
  DIODYNDNS_STATUSCODE_NUMHOST            ,   // "numhost"      Too many or too few hosts found.
  DIODYNDNS_STATUSCODE_DNSERR             ,   // "dnserr"       DNS error encountered.
  DIODYNDNS_STATUSCODE_911                ,   // "911"          There is a serious problem on our side, such as a database or DNS server failure. The client should stop updating until notified via the status page that the service is back up.
};

#define DIODYNDNS_DEFAULTTIMEOUT  10

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWEBCLIENT;


class DIODYNDNS
{
  public:

                                DIODYNDNS               ();
    virtual                    ~DIODYNDNS               ();

    XSTRING*                    GetLogin                ();
    XSTRING*                    GetPassword             ();

    bool                        GetPublicIP             (DIOIP& publicIP, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL) ;
    bool                        IsChangedPublicIP       (DIOURL& url, bool& ischanged, DIOIP* newpublicip = NULL, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL);

    DIODYNDNS_STATUSCODE        Update                  (DIOURL& url, DIOIP& newpublicip, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL);

  private:

    DIODYNDNS_STATUSCODE        GetError                (XSTRING& result);
    void                        Clean                   ();

    DIOWEBCLIENT*               webclient;

    XSTRING                     login;
    XSTRING                     password;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





