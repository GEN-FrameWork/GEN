/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSMTP.h
* 
* @class      DIOSMTP
* @brief      Data Input/Output SMTP (Simple Mail Transfer Protocol) class
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
#include "XFile.h"

#include "DIOURL.h"
#include "DIOEmailAddress.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSSMPTCONTENTTYPE
{
  DIOSSMPTCONTENTTYPE_PLAINTTEXT          = 0 ,
  DIOSSMPTCONTENTTYPE_UTF8                    ,
};


enum DIOSMTPRECIPIENTTYPE
{
  DIOSMTPRECIPIENTTYPE_UNKNOWN            = 0 ,
  DIOSMTPRECIPIENTTYPE_TO                     ,  //  TO: header with this recipient's address
  DIOSMTPRECIPIENTTYPE_CC                     ,  //  CC: header with this recipient's address
  DIOSMTPRECIPIENTTYPE_BCC                       //  BCC: This recipient isn't mentioned in the headers at all, but is still a recipient.
};


enum DIOSMTPXPRIORITY
{
  DIOSMTPXPRIORITY_UNKNOWN                = 0 ,
  DIOSMTPXPRIORITY_HIGH                   = 2 ,
  DIOSMTPXPRIORITY_NORMAL                 = 3 ,
  DIOSMTPXPRIORITY_LOW                    = 4
};

#define DIOSMTP_MAXNATTEMPTSCONNECT         3
#define DIOSMTP_DEFAULTTIMEOUT              3
#define DIOSMTP_DEFAULSIZELIMITATTACHMENT   (3*1024*1024)
#define DIOSMTP_ANYSIZELIMITATTACHMENT      -1
#define DIOSMTP_BOUNDARYTEXT                _L("__MESSAGE__ID__54yg6f6h6y456345")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XFILETXT;
class DIOSTREAM;

class DIOSMTPRECIPIENT
{
  public:
                                  DIOSMTPRECIPIENT                ();
    virtual                      ~DIOSMTPRECIPIENT                ();

    DIOSMTPRECIPIENTTYPE          GetType                         ();
    void                          SetType                         (DIOSMTPRECIPIENTTYPE type);

    XSTRING*                      GetName                         ();
    DIOEMAILADDRESS*              GetEmail                        ();

  private:

    void                          Clean                           ();

    DIOSMTPRECIPIENTTYPE          type;
    XSTRING                       name;
    DIOEMAILADDRESS               email;
};


class DIOSMTPATTACHMENT
{
  public:
                                  DIOSMTPATTACHMENT               ();
    virtual                      ~DIOSMTPATTACHMENT               ();

    XPATH*                        GetXPath                        ();

    bool                          FileExists                      (XQWORD* size = NULL);
    XQWORD                        GetSize                         ();

    XQWORD                        GetSizeLimit                    ();
    void                          SetSizeLimit                    (XQWORD sizelimit = DIOSMTP_ANYSIZELIMITATTACHMENT);

  private:

    void                          Clean                           ();

    XPATH                         xpath;
    XQWORD                        size;
    XQWORD                        sizelimit;
};

class DIOSMTP
{
  public:

                                  DIOSMTP                         (DIOSTREAM* diostream);
    virtual                      ~DIOSMTP                         ();

    bool                          Ini                             ();

    DIOURL*                       Server_GetURL                   ();

    int                           Server_GetPort                  ();
    void                          Server_SetPort                  (int port);

    XSTRING*                      Server_GetLogin                 ();
    XSTRING*                      Server_GetPassword              ();

    int                           Server_GetConnectionTimeout     ();
    void                          Server_SetConnectionTimeout     (int timeout = DIOSMTP_DEFAULTTIMEOUT);

    XSTRING*                      Server_GetConnectionLocalIP     ();

    bool                          Server_IsAvailable              ();

    DIOSSMPTCONTENTTYPE           GetContentType                  ();
    void                          SetContentType                  (DIOSSMPTCONTENTTYPE contenttype);

    XSTRING*                      GetSenderName                   ();
    DIOEMAILADDRESS*              GetSenderEmail                  ();

    DIOEMAILADDRESS*              GetReplytoEmail                 ();

    XVECTOR<DIOSMTPRECIPIENT*>*   GetRecipients                   ();
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XCHAR* name, XCHAR* email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XSTRING& name, XSTRING& email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XCHAR* name, DIOEMAILADDRESS& email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XSTRING& name, DIOEMAILADDRESS& email, bool check = true);
    bool                          DelRecipient                    (DIOEMAILADDRESS& email);
    bool                          DelAllRecipients                ();

    XSTRING*                      GetXMailer                      ();

    XSTRING*                      GetSubject                      ();
    XFILETXT*                     GetMessage                      ();

    DIOSMTPXPRIORITY              GetXPriority                    ();
    void                          SetXPriority                    (DIOSMTPXPRIORITY xpriority);

    bool                          AddAttachment                   (XCHAR* path, bool check = true, XDWORD sizelimit = DIOSMTP_DEFAULSIZELIMITATTACHMENT);
    bool                          AddAttachment                   (XPATH& xpath, bool check = true, XDWORD sizelimit = DIOSMTP_DEFAULSIZELIMITATTACHMENT);
    bool                          DelAttachment                   (XCHAR* path);
    bool                          DelAttachment                   (XPATH& xpath);
    bool                          DelAllAttachments               ();

    bool                          Send                            ();

    bool                          End                             ();

  private:

    bool                          UpdateConnectionConfig           ();

    int                           GetCodeResult                   (XSTRING& answer);

    bool                          SendResponse                    (XSTRING& response, int codevalid);
    bool                          SendResponseAndWait             (XSTRING& response, int codevalid, int codecontinue);

    bool                          CreateHeader                    (XSTRING& header);

    void                          Clean                           ();

    DIOSTREAM*                    diostream;

    DIOURL                        serverURL;
    int                           serverport;

    XSTRING                       serverlogin;
    XSTRING                       serverpassword;

    int                           serverconnectiontimeout;
    XSTRING*                      serverconnectionlocalIP;

    DIOSSMPTCONTENTTYPE           contenttype;

    XSTRING                       sendername;
    DIOEMAILADDRESS               senderemail;
    DIOEMAILADDRESS               replytoemail;
    XVECTOR<DIOSMTPRECIPIENT*>    recipients;
    XSTRING                       xmailer;
    XSTRING                       subject;
    XFILETXT*                     message;
    DIOSMTPXPRIORITY              xpriority;
    XVECTOR<DIOSMTPATTACHMENT*>   attachments;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






