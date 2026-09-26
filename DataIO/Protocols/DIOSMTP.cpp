/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSMTP.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOSMTP.h"

#include "XBuffer.h"
#include "XFileTXT.h"
#include "XTrace.h"

#include "VersionFrameWork.h"

#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPRECIPIENT::DIOSMTPRECIPIENT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPRECIPIENT::DIOSMTPRECIPIENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPRECIPIENT::~DIOSMTPRECIPIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPRECIPIENT::~DIOSMTPRECIPIENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPRECIPIENTTYPE DIOSMTPRECIPIENT::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     DIOSMTPRECIPIENTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPRECIPIENTTYPE DIOSMTPRECIPIENT::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTPRECIPIENT::SetType(DIOSMTPRECIPIENTTYPE type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTPRECIPIENT::SetType(DIOSMTPRECIPIENTTYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTPRECIPIENT::GetName()
* @brief      Get name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTPRECIPIENT::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOEMAILADDRESS* DIOSMTPRECIPIENT::GetEmail()
* @brief      Get email
* @ingroup    DATAIO
* 
* @return     DIOEMAILADDRESS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOEMAILADDRESS* DIOSMTPRECIPIENT::GetEmail()
{
  return &email;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTPRECIPIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTPRECIPIENT::Clean()
{
  type = DIOSMTPRECIPIENTTYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPATTACHMENT::DIOSMTPATTACHMENT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPATTACHMENT::DIOSMTPATTACHMENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPATTACHMENT::~DIOSMTPATTACHMENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPATTACHMENT::~DIOSMTPATTACHMENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOSMTPATTACHMENT::GetXPath()
* @brief      Get X path
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOSMTPATTACHMENT::GetXPath()
{
  return &xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTPATTACHMENT::FileExists(XQWORD* size)
* @brief      File exists
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTPATTACHMENT::FileExists(XQWORD* size)
{  
  bool   status = false;
  
  XFILE* xfile=GEN_XFACTORY.Create_File();
  if(xfile)
    {
      if(xfile->Open(xpath))
        {
          if(size) (*size) = xfile->GetSize(); else this->size = xfile->GetSize();
          xfile->Close();
          status = true;
        }
      GEN_XFACTORY.Delete_File(xfile);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOSMTPATTACHMENT::GetSize()
* @brief      Get size
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOSMTPATTACHMENT::GetSize()
{
  FileExists();
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOSMTPATTACHMENT::GetSizeLimit()
* @brief      Get size limit
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOSMTPATTACHMENT::GetSizeLimit()
{
  return sizelimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTPATTACHMENT::SetSizeLimit(XQWORD sizelimit)
* @brief      Set size limit
* @ingroup    DATAIO
* 
* @param[in]  sizelimit : Sizelimit value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTPATTACHMENT::SetSizeLimit(XQWORD sizelimit)
{
  this->sizelimit = sizelimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTPATTACHMENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTPATTACHMENT::Clean()
{
  size        = 0;
  sizelimit   = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTP::DIOSMTP(DIOSTREAM* diostream)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTP::DIOSMTP(DIOSTREAM* diostream)
{
  Clean();

  this->diostream   = diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTP::~DIOSMTP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTP::~DIOSMTP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::Ini()
{
  if(!diostream)                return false;
  if(!diostream->GetConfig())   return false;

  message = GEN_NEW XFILETXT();
  if(!message) return false;

  serverconnectiontimeout  = DIOSMTP_DEFAULTTIMEOUT;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOSMTP::Server_GetURL()
* @brief      Server get URL
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOSMTP::Server_GetURL()
{
  return &serverURL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSMTP::Server_GetPort()
* @brief      Server get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSMTP::Server_GetPort()
{
  return serverport;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTP::Server_SetPort(int port)
* @brief      Server set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTP::Server_SetPort(int port)
{
  serverport = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::Server_GetLogin()
* @brief      Server get login
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::Server_GetLogin()
{
  return &serverlogin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::Server_GetPassword()
* @brief      Server get password
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::Server_GetPassword()
{
  return &serverpassword;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSMTP::Server_GetConnectionTimeout()
* @brief      Server get connection timeout
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSMTP::Server_GetConnectionTimeout()
{
  return serverconnectiontimeout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTP::Server_SetConnectionTimeout(int timeout)
* @brief      Server set connection timeout
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTP::Server_SetConnectionTimeout(int timeout)
{
  serverconnectiontimeout = timeout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::Server_GetConnectionLocalIP()
* @brief      Server get connection local IP
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::Server_GetConnectionLocalIP()
{
  return serverconnectionlocalIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::Server_IsAvailable()
* @brief      Server is available
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::Server_IsAvailable()
{
  int  attempts   = DIOSMTP_MAXNATTEMPTSCONNECT;
  bool status     = false;

  UpdateConnectionConfig();

  while(attempts)
    {
      if(diostream->Open())
        {
          status = diostream->WaitToConnected(serverconnectiontimeout);
          diostream->Close();
        }

      attempts--;
      if(status) break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSSMPTCONTENTTYPE DIOSMTP::GetContentType()
* @brief      Get content type
* @ingroup    DATAIO
* 
* @return     DIOSSMPTCONTENTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSSMPTCONTENTTYPE DIOSMTP::GetContentType()
{
  return contenttype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTP::SetContentType(DIOSSMPTCONTENTTYPE contenttype)
* @brief      Set content type
* @ingroup    DATAIO
* 
* @param[in]  contenttype : Contenttype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTP::SetContentType(DIOSSMPTCONTENTTYPE contenttype)
{
  this->contenttype = contenttype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::GetSenderName()
* @brief      Get sender name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::GetSenderName()
{
  return &sendername;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOEMAILADDRESS* DIOSMTP::GetSenderEmail()
* @brief      Get sender email
* @ingroup    DATAIO
* 
* @return     DIOEMAILADDRESS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOEMAILADDRESS* DIOSMTP::GetSenderEmail()
{
  return &senderemail;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOEMAILADDRESS* DIOSMTP::GetReplytoEmail()
* @brief      Get replyto email
* @ingroup    DATAIO
* 
* @return     DIOEMAILADDRESS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOEMAILADDRESS* DIOSMTP::GetReplytoEmail()
{
  return &replytoemail;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOSMTPRECIPIENT*>* DIOSMTP::GetRecipients()
* @brief      Get recipients
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOSMTPRECIPIENT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSMTPRECIPIENT*>* DIOSMTP::GetRecipients()
{
  return &recipients;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, XCHAR* email, bool check)
* @brief      Add recipient
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  name : Name to use.
* @param[in]  email : Email pointer to use.
* @param[in]  check : Check value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, XCHAR* email, bool check)
{
  DIOEMAILADDRESS _email;

  _email = email;

  if(check)
    {
      if(!_email.IsValid()) return false;
    }

  DIOSMTPRECIPIENT* recipient = GEN_NEW DIOSMTPRECIPIENT();
  if(!recipient) return false;

  recipient->SetType(type);
  if(name) recipient->GetName()->Set(name);
  recipient->GetEmail()->Set(_email);

  return recipients.Add(recipient);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, XSTRING& email, bool check)
* @brief      Add recipient
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  name : Name to use.
* @param[in]  email : Email value.
* @param[in]  check : Check value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, XSTRING& email, bool check)
{
  return AddRecipient(type, name.Get(), email.Get(), check);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, DIOEMAILADDRESS& email, bool check)
* @brief      Add recipient
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  name : Name to use.
* @param[in]  email : Email value.
* @param[in]  check : Check value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, DIOEMAILADDRESS& email, bool check)
{
  return AddRecipient(type, name, email.Get(), check);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, DIOEMAILADDRESS& email, bool check)
* @brief      Add recipient
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  name : Name to use.
* @param[in]  email : Email value.
* @param[in]  check : Check value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, DIOEMAILADDRESS& email, bool check)
{
  return AddRecipient(type, name.Get(), email.Get(), check);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::DelRecipient(DIOEMAILADDRESS& email)
* @brief      Del recipient
* @ingroup    DATAIO
* 
* @param[in]  email : Email value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::DelRecipient(DIOEMAILADDRESS& email)
{
  if(recipients.IsEmpty()) return false;

  for(XDWORD c=0; c<recipients.GetSize(); c++)
    {
      DIOSMTPRECIPIENT* recipient = recipients.Get(c);
      if(recipient)
        {
          if((*recipient->GetEmail()) == email)
            {
              recipients.Delete(recipient);
              GEN_DELETE recipient;

              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::DelAllRecipients()
* @brief      Del all recipients
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::DelAllRecipients()
{
  if(recipients.IsEmpty()) return false;

  recipients.DeleteContents();
  recipients.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::GetXMailer()
* @brief      Get X mailer
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::GetXMailer()
{
  return &xmailer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSMTP::GetSubject()
* @brief      Get subject
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSMTP::GetSubject()
{
  return &subject;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILETXT* DIOSMTP::GetMessage()
* @brief      Get message
* @ingroup    DATAIO
* 
* @return     XFILETXT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILETXT* DIOSMTP::GetMessage()
{
  return message;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSMTPXPRIORITY DIOSMTP::GetXPriority()
* @brief      Get X priority
* @ingroup    DATAIO
* 
* @return     DIOSMTPXPRIORITY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSMTPXPRIORITY DIOSMTP::GetXPriority()
{
  return xpriority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTP::SetXPriority(DIOSMTPXPRIORITY xpriority)
* @brief      Set X priority
* @ingroup    DATAIO
* 
* @param[in]  xpriority : Xpriority value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTP::SetXPriority(DIOSMTPXPRIORITY xpriority)
{
  this->xpriority = xpriority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddAttachment(XCHAR* path, bool check, XDWORD sizelimit)
* @brief      Add attachment
* @ingroup    DATAIO
* 
* @param[in]  path : Path to use.
* @param[in]  check : Check value.
* @param[in]  sizelimit : Sizelimit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddAttachment(XCHAR* path, bool check, XDWORD sizelimit)
{
  if(!path) return false;

  DIOSMTPATTACHMENT* attachment = GEN_NEW DIOSMTPATTACHMENT();
  if(!attachment) return false;

  attachment->GetXPath()->Set(path);

  if(check)
    {
      if(!attachment->FileExists())
        {
          GEN_DELETE attachment;
          return false;
        }
    }

  if(attachment->GetSize()>sizelimit)
    {
      GEN_DELETE attachment;
      return false;
    }

  attachments.Add(attachment);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::AddAttachment(XPATH& xpath, bool check, XDWORD sizelimit)
* @brief      Add attachment
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* @param[in]  check : Check value.
* @param[in]  sizelimit : Sizelimit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::AddAttachment(XPATH& xpath, bool check, XDWORD sizelimit)
{
  return AddAttachment(xpath.Get(), check, sizelimit);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::DelAttachment(XCHAR* path)
* @brief      Del attachment
* @ingroup    DATAIO
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::DelAttachment(XCHAR* path)
{
  if(attachments.IsEmpty()) return false;

  for(XDWORD c=0; c<attachments.GetSize(); c++)
    {
      DIOSMTPATTACHMENT* attachment = attachments.Get(c);
      if(attachment)
        {
          if(!attachment->GetXPath()->Compare(path))
            {
              attachments.Delete(attachment);
              GEN_DELETE attachment;

              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::DelAttachment(XPATH& xpath)
* @brief      Del attachment
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::DelAttachment(XPATH& xpath)
{
  return DelAttachment(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::DelAllAttachments()
* @brief      Del all attachments
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::DelAllAttachments()
{
  attachments.DeleteContents();
  attachments.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::Send()
* @brief      Send
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::Send()
{
  if(!diostream)                return false;

  if(!diostream->GetConfig())   return false;

  if(recipients.IsEmpty())      return false;

  if(!UpdateConnectionConfig())  return false;

  int  attempts = DIOSMTP_MAXNATTEMPTSCONNECT;
  bool status   = false;

  while(attempts)
    {
      if(diostream->Open()) status = diostream->WaitToConnected(serverconnectiontimeout);

      attempts--;
      if(status) break;
    }

  if(!status)
    {
      diostream->Close();
      return false;
    }

  XSTRING response;

  if(!diostream->ReadStr(response, serverconnectiontimeout))
    {
      diostream->Close();
      return false;
    }

  int code = GetCodeResult(response);
  if(code!=220)
    {
      diostream->Close();
      return false;
    }

  XSTRING domain;
  senderemail.GetDomain(domain);

  response.Format(__L("EHLO %s\r\n"), domain.Get());
  if(!SendResponse(response, 250))
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("AUTH LOGIN\r\n"));
  if(!SendResponseAndWait(response, 334, 250))
    {
      diostream->Close();
      return false;
    }

  XSTRING encoded64login;
  XSTRING encoded64password;

  serverlogin.ConvertToBase64(encoded64login);
  serverpassword.ConvertToBase64(encoded64password);

  response.Format(__L("%s\r\n"), encoded64login.Get());
  if(!SendResponse(response, 334))
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("%s\r\n"), encoded64password.Get());
  if(!SendResponseAndWait(response, 235, 334))
    {
      diostream->Close();
      return false;
    }

  if(sendername.GetSize())
    {
      response.Format(__L("MAIL FROM:<%s>\r\n"), sendername.Get());
      if(!SendResponse(response, 250))
        {
          diostream->Close();
          return false;
        }
    }

  bool atleastonereceiver =false;

  for(XDWORD c=0; c<recipients.GetSize(); c++)
    {
      DIOSMTPRECIPIENT* recipient = recipients.Get(c);
      if(recipient)
        {
          response.Format(__L("RCPT TO:<%s>\r\n"), recipient->GetEmail()->Get());
          if(!SendResponse(response, 250))
            {

            } else atleastonereceiver = true;
        }
    }

  if(!atleastonereceiver)
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("DATA\r\n"), encoded64password.Get());
  if(!SendResponseAndWait(response, 354, 250))
    {
      diostream->Close();
      return false;
    }

  if(!CreateHeader(response))
    {
      diostream->Close();
      return false;
    }

  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
    {
      diostream->Close();
      return false;
    }

  if(GetMessage()->GetNLines())
    {
      switch(contenttype)
        {
          case DIOSSMPTCONTENTTYPE_PLAINTTEXT : response.Empty();
                                                for(int c=0; c<GetMessage()->GetNLines(); c++)
                                                  {
                                                    XSTRING line;
                                                    line.Format(__L("%s\r\n"), GetMessage()->GetLine(c)->Get());
                                                    response  += line.Get();
                                                  }

                                                if(response.IsEmpty()) response  += __L(" \r\n");

                                                diostream->WriteStr(response);
                                                if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
                                                  {
                                                    diostream->Close();
                                                    return false;
                                                  }
                                                break;

            case DIOSSMPTCONTENTTYPE_UTF8     : for(int c=0; c<GetMessage()->GetNLines(); c++)
                                                  {
                                                    XBUFFER xbufferline;
                                                    XSTRING line;

                                                    line  = GetMessage()->GetLine(c)->Get();
                                                    line += __L("\r\n");

                                                    line.ConvertToUTF8(xbufferline);

                                                    diostream->Write(xbufferline);
                                                    if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
                                                      {
                                                        diostream->Close();
                                                        return false;
                                                      }
                                                  }
                                                break;
          }
    }
   else
    {
      response  += __L(" \r\n");

      diostream->WriteStr(response);
      if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
        {
          diostream->Close();
          return false;
        }
    }

  if(!attachments.IsEmpty())
    {
      for(XDWORD c=0; c<attachments.GetSize() ;c++)
        {
          DIOSMTPATTACHMENT* attachment = attachments.Get(c);
          if(attachment)
            {
              XBUFFER datafile;
              XSTRING datafileenconded64;
              XQWORD  filesize;

              if(attachment->FileExists(&filesize))
                {
                  XSTRING filenameext;

                  attachment->GetXPath()->GetNamefileExt(filenameext);

                  response.Format(__L("--%s\r\n"), DIOSMTP_BOUNDARYTEXT);
                  response += __L("Content-Type: application/octect-stream\r\n");
                  response += __L("Content-Transfer-Encoding: base64\r\n");
                  response += __L("Content-Disposition: attachment;\r\n filename=\"");
                  response += filenameext.Get();
                  response += __L("\"\r\n");
                  response += __L("\r\n");

                  diostream->WriteStr(response);
                  if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
                    {
                      diostream->Close();
                      return false;
                    }

                  XFILE* xfile=GEN_XFACTORY.Create_File();
                  if(xfile)
                    {
                      XQWORD br  = filesize;
                      
                      if(xfile->Open((*attachment->GetXPath())))
                        {
                          datafile.Resize((XDWORD)filesize);

                          if(xfile->Read(datafile.Get(), (XDWORD*)&br)) datafile.ConvertToBase64(datafileenconded64);
                    
                          xfile->Close();
                        }

                      #define MAXSIZEPARTENCODED64 64

                      XSTRING partstring;
                      int     index    = 0;
                      int     sizepart;

                      response.Empty();

                      do{ sizepart = MAXSIZEPARTENCODED64;
                          if((index+MAXSIZEPARTENCODED64)>=(int)datafileenconded64.GetSize()) sizepart = datafileenconded64.GetSize()-index;

                          datafileenconded64.Copy(index, index+sizepart, partstring);

                          response += partstring;
                          response += __L("\r\n");

                          index += sizepart;

                        } while(sizepart == MAXSIZEPARTENCODED64);

                      diostream->WriteStr(response);
                      if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
                        {
                          diostream->Close();
                          return false;
                        }
                    }
                }
            }
        }

      response.Format(__L("--%s--\r\n"), DIOSMTP_BOUNDARYTEXT);
      diostream->WriteStr(response);
      if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout))
        {
          diostream->Close();
          return false;
        }
    }

  response = __L("\r\n.\r\n");
  if(!SendResponse(response, 250))
    {
      diostream->Close();
      return false;
    }

  response = __L("QUIT\r\n");
  if(!SendResponse(response, 221))
    {
      diostream->Close();
      return false;
    }

  diostream->Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::End()
{
  DelAllRecipients();

  DelAllAttachments();

  if(message)
    {
      GEN_DELETE message;
      message = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::UpdateConnectionConfig()
* @brief      Update connection config
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::UpdateConnectionConfig()
{
  DIOSTREAMTCPIPCONFIG* diostreamcfg = (DIOSTREAMTCPIPCONFIG*)diostream->GetConfig();

  if(!diostreamcfg) return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(serverconnectionlocalIP)  diostreamcfg->GetLocalIP()->Set(serverconnectionlocalIP->Get());

  diostreamcfg->GetRemoteURL()->Set(serverURL.Get());
  diostreamcfg->SetRemotePort(serverport);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSMTP::GetCodeResult(XSTRING& answer)
* @brief      Get code result
* @ingroup    DATAIO
* 
* @param[in]  answer : Answer value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSMTP::GetCodeResult(XSTRING& answer)
{
  XSTRING  codestring;
  int      code = 0;

  answer.Copy(0, 3, codestring);

  code = codestring.ConvertToInt();

  return code;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::SendResponse(XSTRING& response, int codevalid)
* @brief      Send response
* @ingroup    DATAIO
* 
* @param[in]  response : Response value.
* @param[in]  codevalid : Codevalid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::SendResponse(XSTRING& response, int codevalid)
{
  int code = 0;

  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout)) return false;

  if(!diostream->ReadStr(response, serverconnectiontimeout)) return false;

  code = GetCodeResult(response);
  if(code!=codevalid) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::SendResponseAndWait(XSTRING& response, int codevalid, int codecontinue)
* @brief      Send response and wait
* @ingroup    DATAIO
* 
* @param[in]  response : Response value.
* @param[in]  codevalid : Codevalid value.
* @param[in]  codecontinue : Codecontinue value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::SendResponseAndWait(XSTRING& response, int codevalid, int codecontinue)
{
  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnectiontimeout)) return false;

  int  code     = 0;
  bool accepted = false;
  bool status   = false;

  do{ status = diostream->ReadStr(response, serverconnectiontimeout);
      if(!status) break;

      code = GetCodeResult(response);
      if(code == codevalid)
        {
          accepted = true;
        }
       else
        {
          if(code != codecontinue)  status = false;
        }

      if(!status) break;

    } while(!accepted);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSMTP::CreateHeader(XSTRING& header)
* @brief      Create header
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSMTP::CreateHeader(XSTRING& header)
{
//XCHAR*  month[] = { __L("Jan") , __L("Feb"), __L("Mar"), __L("Apr"), __L("May"), __L("Jun"), __L("Jul") , __L("Aug"), __L("Sep"), __L("Oct"), __L("Nov"), __L("Dec") };
  XSTRING to_line;
  XSTRING cc_line;
  XSTRING bcc_line;
  XSTRING string;

  header.Empty();

  if(!recipients.IsEmpty())
    {
      for(XDWORD c=0; c<recipients.GetSize(); c++)
        {
          DIOSMTPRECIPIENT* recipient = recipients.Get(c);
          if(recipient)
            {
              string.Format(__L("%s<%s>"), recipient->GetName()->Get(), recipient->GetEmail()->Get());

              switch(recipient->GetType())
                {
                  case DIOSMTPRECIPIENTTYPE_UNKNOWN : break;

                  case DIOSMTPRECIPIENTTYPE_TO      : if(!to_line.IsEmpty()) to_line+= __L(",");
                                                      to_line += string.Get();
                                                      break;

                  case DIOSMTPRECIPIENTTYPE_CC      : if(!cc_line.IsEmpty()) cc_line+= __L(",");
                                                      cc_line += string.Get();
                                                      break;

                  case DIOSMTPRECIPIENTTYPE_BCC     : if(!bcc_line.IsEmpty()) bcc_line+= __L(",");
                                                      bcc_line += string.Get();
                                                      break;
                }
            }
        }
    }

  string.Empty();

  /*
  XDATETIME* GEN_XFACTORY_CREATE(datetime, CreateTime())
  if(datetime)
    {
      datetime->Read();

      string.Format(__L("Date: %d %s %d %d:%d:%d\r\n"), datetime->GetDay()
                                                      , month[datetime->GetMonth()-1]
                                                      , datetime->GetYear()
                                                      , datetime->GetHours()
                                                      , datetime->GetMinutes()
                                                      , datetime->GetSeconds());


      GEN_XFACTORY.DeleteTime(datetime);
    }

  if(string.IsEmpty()) return false;
  header+= string;
  */

  string.Format(__L("From: %s <%s>\r\n"), GetSenderName()->Get(), GetSenderEmail()->Get());
  header+= string;


  if(!GetReplytoEmail()->IsEmpty())
    {
      string.Format(__L("Reply-To: %s\r\n"), GetReplytoEmail()->Get());
      header+= string;
    }

  XSTRING defaultxmailer;

  defaultxmailer  = GEN_VERSION.GetName()->Get();
  defaultxmailer += __L(" ");
  defaultxmailer += GEN_VERSION.GetVersion()->Get();

  string.Format(__L("X-Mailer: %s\r\n"), GetXMailer()->IsEmpty()? defaultxmailer.Get() : GetXMailer()->Get());
  header+= string;

  switch(xpriority)
    {
      case DIOSMTPXPRIORITY_HIGH    : string = __L("X-Priority: 2 (High)\r\n");     break;
      case DIOSMTPXPRIORITY_UNKNOWN :
      case DIOSMTPXPRIORITY_NORMAL  : string = __L("X-Priority: 3 (Normal)\r\n");   break;
      case DIOSMTPXPRIORITY_LOW     : string = __L("X-Priority: 4 (Low)\r\n");      break;
    }

  if(string.IsEmpty()) return false;
  header+= string;

  if(!to_line.IsEmpty())
    {
      string.Format(__L("To: %s\r\n"), to_line.Get());
      header+= string;
    }

  if(!cc_line.IsEmpty())
    {
      string.Format(__L("Cc: %s\r\n"), cc_line.Get());
      header+= string;
    }

  if(!bcc_line.IsEmpty())
    {
      string.Format(__L("Bcc: %s\r\n"), bcc_line.Get());
      header+= string;
    }


  string.Empty();
  switch(contenttype)
    {
      case DIOSSMPTCONTENTTYPE_PLAINTTEXT : string.Format(__L("Subject: %s\r\n"), GetSubject()->IsEmpty()?__L(" "):GetSubject()->Get());
                                            header+= string;
                                            break;

      case DIOSSMPTCONTENTTYPE_UTF8       : { XBUFFER subjectUTF8;
                                              XSTRING subjectbase64;

                                              GetSubject()->ConvertToUTF8(subjectUTF8);

                                              subjectUTF8.ConvertToBase64(subjectbase64);

                                              string = __L("Subject: =?UTF-8?B?");
                                              string += subjectbase64;
                                              string += __L("?=\r\n");
                                            }
                                            break;
    }

  header += string;


  string = __L("MIME-Version: 1.0\r\n");

  if(attachments.GetSize())
    {
      XSTRING string2;
      string2.Format(__L("Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n--%s\n\r"), DIOSMTP_BOUNDARYTEXT, DIOSMTP_BOUNDARYTEXT);
      string += string2;
    }

  switch(contenttype)
    {
      case DIOSSMPTCONTENTTYPE_PLAINTTEXT : string += __L("Content-type: text/plain; charset=utf-8\r\n"); break;
      case DIOSSMPTCONTENTTYPE_UTF8       : string += __L("Content-type: text/plain; charset=utf-8\r\n"); break;
    }

//string += __L("Content-Transfer-Encoding: 7bit\r\n");
  string += __L("\r\n");

  header += string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSMTP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSMTP::Clean()
{

  diostream                 = NULL;

  serverport                = 0;

  serverconnectiontimeout    = DIOSMTP_DEFAULTTIMEOUT;
  serverconnectionlocalIP    = NULL;

  contenttype               = DIOSSMPTCONTENTTYPE_PLAINTTEXT;

  message                   = NULL;

  xpriority                 = DIOSMTPXPRIORITY_UNKNOWN;
}





