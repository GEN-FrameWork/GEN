/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODynDNS.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIODynDNS.h"

#include "DIOFactory.h"
#include "DIOWebClient.h"
#ifdef DIO_PUBLICINTERNETIP_ACTIVE
#include "DIOPublicInternetIP.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODYNDNS::DIODYNDNS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODYNDNS::DIODYNDNS()
{
  Clean();

  webclient = GEN_NEW DIOWEBCLIENT;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODYNDNS::~DIODYNDNS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODYNDNS::~DIODYNDNS()
{
  GEN_DELETE webclient;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIODYNDNS::GetLogin()
* @brief      Get login
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIODYNDNS::GetLogin()
{
  return &login;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIODYNDNS::GetPassword()
* @brief      Get password
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIODYNDNS::GetPassword()
{
  return &password;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODYNDNS::GetPublicIP(DIOIP& publicIP, int timeout, XSTRING* IPlocal)
* @brief      Get public IP
* @ingroup    DATAIO
* 
* @param[in]  publicIP : Public IP value.
* @param[in]  timeout : Timeout value.
* @param[in]  IPlocal : I Plocal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODYNDNS::GetPublicIP(DIOIP& publicIP, int timeout, XSTRING* IPlocal)
{
  XBUFFER   xbuffer;
  bool      status;

  status = webclient->Get(__L("checkip.dyndns.com"), xbuffer, DIOWEBCLIENT_DEFAULTUSERAGENT, timeout, IPlocal);
  if(status)
    {
      XSTRING startstring;
      XSTRING result;

      result.Set(xbuffer.Get(), xbuffer.GetSize());

      startstring = __L("Current IP Address: ");

      int start = result.Find(startstring, true);
      if(start != XSTRING_NOTFOUND)
        {
          start += startstring.GetSize();

          int end  = result.Find(__L("</body>"), true, start);
          if(end != XSTRING_NOTFOUND)
            {
              XSTRING IPstring;

              result.Copy(start, end, IPstring);
              if(!IPstring.IsEmpty())
                {
                  return publicIP.Set(IPstring);
                }
            }
        }
    }
   else
    {        
      #ifdef DIO_PUBLICINTERNETIP_ACTIVE
      GEN_DIOPUBLICINTERNETIP.Get(publicIP);
      #endif
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODYNDNS::IsChangedPublicIP(DIOURL& url, bool& ischanged, DIOIP* newpublicip, int timeout, XSTRING* IPlocal)
* @brief      Is changed public IP
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  ischanged : Ischanged value.
* @param[in]  newpublicip : Newpublicip pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  IPlocal : I Plocal pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODYNDNS::IsChangedPublicIP(DIOURL& url, bool& ischanged, DIOIP* newpublicip, int timeout, XSTRING* IPlocal)
{
  DIOIP   publicIP[2];
  bool    status  = false;
  DIOURL* _url    = NULL; 

  ischanged = false;

  _url    = GEN_NEW DIOURL();
  if(!_url) 
    {
      return false;
    }

  (*_url) = url;

  if(_url->ResolveURL(publicIP[0]))
    {
      if(GetPublicIP(publicIP[1], timeout, IPlocal))
        {
          ischanged = !publicIP[0].Compare(publicIP[1]);

          if(ischanged)
            {
              if(newpublicip) newpublicip->Set(publicIP[1].Get());
            }

          status = true;
        }
    }

  GEN_DELETE _url;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODYNDNS_STATUSCODE DIODYNDNS::Update(DIOURL& url, DIOIP& newpublicip, int timeout, XSTRING* IPlocal)
* @brief      Update
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  newpublicip : Newpublicip value.
* @param[in]  timeout : Timeout value.
* @param[in]  IPlocal : I Plocal pointer to use.
* 
* @return     DIODYNDNS_STATUSCODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODYNDNS_STATUSCODE DIODYNDNS::Update(DIOURL& url, DIOIP& newpublicip, int timeout, XSTRING* IPlocal)
{
  if(!webclient) return DIODYNDNS_STATUSCODE_UNKNOWN;

  DIOURL    askurl;
  XSTRING   IPstring;
  XBUFFER   xbuffer;
  XSTRING   result;
  bool      status;

  newpublicip.GetXString(IPstring);

  askurl.Format(__L("%s:%s@members.dyndns.org/nic/update?hostname=%s&myip=%s&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG"), login.Get(), password.Get(), url.Get(), IPstring.Get());

  status = webclient->Get(askurl.Get(), xbuffer, DIOWEBCLIENT_DEFAULTUSERAGENT, timeout, IPlocal);
  if(!status)  return DIODYNDNS_STATUSCODE_NOTCONNECT;

  result.Set(xbuffer.Get(), xbuffer.GetSize());

  return GetError(result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODYNDNS_STATUSCODE DIODYNDNS::GetError(XSTRING& result)
* @brief      Get error
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* 
* @return     DIODYNDNS_STATUSCODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODYNDNS_STATUSCODE DIODYNDNS::GetError(XSTRING& result)
{
  // Other          Unknown result from dyndns service.
  // "good"         The update was successful, and the hostname is now updated.
  // "nochg"        The update changed no settings, and is considered abusive. (Additional nochg updates will cause the hostname to become blocked).
  // "badsys"       The system parameter given is not valid. Valid system parameters are dyndns, statdns and custom.
  // "badagent"     The user agent that was sent has been blocked for not following these specifications or no user agent was specified.
  // "badauth"      The username or password specified are incorrect.
  // "!donator"     An option available only to credited users (suchas offline URL) was specified, but the user is not a credited user.
  // "notfqdn"      The hostname specified is not a fully-qualified domain name (not in the form hostname.dyndns.org or domain.com).
  // "nohost"       The hostname specified does not exist (or is not in the service specified in the system parameter).
  // "!yours"       The hostname specified exists, but not under the username specified.
  // "abuse"        The hostname specified is blocked for update abuse.
  // "numhost"      Too many or too few hosts found.
  // "dnserr"       DNS error encountered.
  // "911"          There is a serious problem on our side, such as a database or DNS server failure. The client should stop updating until notified via the status page that the service is back up.


  DIODYNDNS_STATUSCODE code              =  DIODYNDNS_STATUSCODE_UNKNOWN;
  XCHAR*               stringcodes[]     =  {  __L("good")      ,
                                               __L("nochg")     ,
                                               __L("badsys")    ,
                                               __L("badagent")  ,
                                               __L("badauth")   ,
                                               __L("!donator")  ,
                                               __L("notfqdn")   ,
                                               __L("nohost")    ,
                                               __L("!yours")    ,
                                               __L("abuse")     ,
                                               __L("numhost")   ,
                                               __L("dnserr")    ,
                                               __L("911")
                                            };


  for(int c=0; c<(sizeof(stringcodes)/sizeof(XCHAR*)); c++)
    {
      if(result.Find(stringcodes[c], true) != XSTRING_NOTFOUND)
        {
          code = (DIODYNDNS_STATUSCODE)(c);
          break;
        }
    }

  return code;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODYNDNS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODYNDNS::Clean()
{
  webclient   = NULL;
}



