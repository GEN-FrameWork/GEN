/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSMessages.cpp
* 
* @class      DIOSTREAMTLSMESSAGES
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Messages class
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

#include "DIOStreamTLSMessages.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_INTERFACE::DIOSTREAMTLS_MSG_INTERFACE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_INTERFACE::DIOSTREAMTLS_MSG_INTERFACE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_INTERFACE::~DIOSTREAMTLS_MSG_INTERFACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_INTERFACE::~DIOSTREAMTLS_MSG_INTERFACE()
{ 
  Clean();
}
       
    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS_MSG_INTERFACE::GetLengthBuffer()
* @brief      get length buffer
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_INTERFACE::GetLengthBuffer()
{ 
  XBUFFER data;
                                                  
  if(!SetToBuffer(data, false))
    {
      return 0;
    }

  return data.GetSize();
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_INTERFACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_INTERFACE::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_AddLength24(XBUFFER& buffer, XDWORD length)
* @brief      Add a TLS unsigned 24-bit length to a buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer where the length is appended.
* @param[in]  length : Length value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_AddLength24(XBUFFER& buffer, XDWORD length)
{
  if(length > DIOSTREAMTLS_MSG_MAXLENGTH24)
    {
      return false;
    }

  if(!buffer.Add((XBYTE)((length >> 16) & 0xFF))) return false;
  if(!buffer.Add((XBYTE)((length >>  8) & 0xFF))) return false;
  if(!buffer.Add((XBYTE)( length        & 0xFF))) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_ExtractLength24(XBUFFER& buffer, XDWORD& length)
* @brief      Extract a TLS unsigned 24-bit length from the beginning of a buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer that contains the length.
* @param[out] length : Extracted length.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_ExtractLength24(XBUFFER& buffer, XDWORD& length)
{
  if(buffer.GetSize() < 3)
    {
      return false;
    }

  XBYTE data[3] = { 0, 0, 0 };

  if(buffer.Extract(data, 0, sizeof(data)) != sizeof(data))
    {
      return false;
    }

  length = ((XDWORD)data[0] << 16) | ((XDWORD)data[1] << 8) | (XDWORD)data[2];

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE::DIOSTREAMTLS_MSG_HANDSHAKE()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE::DIOSTREAMTLS_MSG_HANDSHAKE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE::~DIOSTREAMTLS_MSG_HANDSHAKE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE::~DIOSTREAMTLS_MSG_HANDSHAKE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE DIOSTREAMTLS_MSG_HANDSHAKE::GetMsgType()
* @brief      Get the handshake message type
* @ingroup    DATAIO
*
* @return     XBYTE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSTREAMTLS_MSG_HANDSHAKE::GetMsgType()
{
  return msgtype;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE::SetMsgType(XBYTE msgtype)
* @brief      Set the handshake message type
* @ingroup    DATAIO
*
* @param[in]  msgtype : Handshake message type.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE::SetMsgType(XBYTE msgtype)
{
  this->msgtype = msgtype;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS_MSG_HANDSHAKE::GetLength()
* @brief      Get the handshake body length
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_HANDSHAKE::GetLength()
{
  return length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE::SetLength(XDWORD length)
* @brief      Set the handshake body length
* @ingroup    DATAIO
*
* @param[in]  length : Length value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE::SetLength(XDWORD length)
{
  this->length = length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE::GetBody()
* @brief      Get the raw handshake body
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE::GetBody()
{
  return &body;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Add the complete handshake message to a buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if((body.GetSize() > DIOSTREAMTLS_MSG_MAXLENGTH24) ||
     (body.GetSize() > (DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE - DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE)))
    {
      return false;
    }

  length = body.GetSize();

  if(!buffer.Add(msgtype))                               return false;
  if(!DIOSTREAMTLS_MSG_AddLength24(buffer, length))      return false;
  if(length && !buffer.Add(body))                        return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Extract one complete handshake message from a buffer
* @note       An incomplete message consumes no bytes.
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER message;

  if(!Message_Extract(buffer, message))
    {
      return false;
    }

  if(!message.Extract(msgtype))
    {
      return false;
    }

  if(!DIOSTREAMTLS_MSG_ExtractLength24(message, length))
    {
      return false;
    }

  body.Delete();

  if(length)
    {
      body.Resize(length);

      if(message.Extract(body.Get(), 0, length) != length)
        {
          body.Delete();
          return false;
        }
    }

  return message.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE::Message_Extract(XBUFFER& input, XBUFFER& message)
* @brief      Extract the first complete handshake message while retaining incomplete input
* @ingroup    DATAIO
*
* @param[in]  input : Accumulator with zero or more handshake messages.
* @param[out] message : Extracted message including its four-byte header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE::Message_Extract(XBUFFER& input, XBUFFER& message)
{
  if(input.GetSize() < DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE)
    {
      return false;
    }

  XBYTE* data = input.Get();
  if(!data)
    {
      return false;
    }

  XDWORD length      = ((XDWORD)data[1] << 16) | ((XDWORD)data[2] << 8) | (XDWORD)data[3];
  XDWORD messagelength = DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE + length;

  if(messagelength > DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE)
    {
      return false;
    }

  if(input.GetSize() < messagelength)
    {
      return false;
    }

  message.Delete();
  message.Resize(messagelength);

  if(input.Extract(message.Get(), 0, messagelength) != messagelength)
    {
      message.Delete();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE::Clean()
{
  msgtype = 0;
  length  = 0;

  body.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_ALERT::DIOSTREAMTLS_MSG_ALERT()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_ALERT::DIOSTREAMTLS_MSG_ALERT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_ALERT::~DIOSTREAMTLS_MSG_ALERT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_ALERT::~DIOSTREAMTLS_MSG_ALERT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_LEVEL DIOSTREAMTLS_MSG_ALERT::GetLevel()
* @brief      Get the alert level
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_LEVEL : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_LEVEL DIOSTREAMTLS_MSG_ALERT::GetLevel()
{
  return (DIOSTREAMTLS_ALERT_LEVEL)level;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_ALERT::SetLevel(DIOSTREAMTLS_ALERT_LEVEL level)
* @brief      Set the alert level
* @ingroup    DATAIO
*
* @param[in]  level : Alert level.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_ALERT::SetLevel(DIOSTREAMTLS_ALERT_LEVEL level)
{
  this->level = (XBYTE)level;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS_MSG_ALERT::GetDescription()
* @brief      Get the alert description
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS_MSG_ALERT::GetDescription()
{
  return (DIOSTREAMTLS_ALERT_DESCRIPTION)description;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* DIOSTREAMTLS_MSG_ALERT::GetDescriptionString(DIOSTREAMTLS_ALERT_DESCRIPTION description)
* @brief      Get the RFC 8446 name of an alert description, for traces and logs
* @note       STATIC
* @ingroup    DATAIO
*
* @param[in]  description : Alert description value.
*
* @return     XCHAR* : Name of the alert; "unknown" when it is not a description defined by RFC 8446.
*
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSTREAMTLS_MSG_ALERT::GetDescriptionString(DIOSTREAMTLS_ALERT_DESCRIPTION description)
{
  switch(description)
    {
      case DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY                    : return _L("close_notify");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE              : return _L("unexpected_message");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_RECORD_MAC                  : return _L("bad_record_mac");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_RECORD_OVERFLOW                 : return _L("record_overflow");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE               : return _L("handshake_failure (nothing offered is acceptable)");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE                 : return _L("bad_certificate");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_CERTIFICATE         : return _L("unsupported_certificate");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED             : return _L("certificate_revoked");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_EXPIRED             : return _L("certificate_expired");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_UNKNOWN             : return _L("certificate_unknown");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER               : return _L("illegal_parameter");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNKNOWN_CA                      : return _L("unknown_ca");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_ACCESS_DENIED                   : return _L("access_denied");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR                    : return _L("decode_error");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR                   : return _L("decrypt_error");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_PROTOCOL_VERSION                : return _L("protocol_version (the server does NOT support TLS 1.3)");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_INSUFFICIENT_SECURITY           : return _L("insufficient_security");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR                  : return _L("internal_error");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_INAPPROPRIATE_FALLBACK          : return _L("inappropriate_fallback");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_USER_CANCELED                   : return _L("user_canceled");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_MISSING_EXTENSION               : return _L("missing_extension");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_EXTENSION           : return _L("unsupported_extension");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNRECOGNIZED_NAME               : return _L("unrecognized_name (SNI not served here)");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE : return _L("bad_certificate_status_response");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_UNKNOWN_PSK_IDENTITY            : return _L("unknown_psk_identity");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REQUIRED            : return _L("certificate_required");
      case DIOSTREAMTLS_ALERT_DESCRIPTION_NO_APPLICATION_PROTOCOL         : return _L("no_application_protocol (ALPN not accepted)");
    }

  return _L("unknown");
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_ALERT::SetDescription(DIOSTREAMTLS_ALERT_DESCRIPTION description)
* @brief      Set the alert description
* @ingroup    DATAIO
*
* @param[in]  description : Alert description.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_ALERT::SetDescription(DIOSTREAMTLS_ALERT_DESCRIPTION description)
{
  this->description = (XBYTE)description;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_ALERT::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Add the alert to a buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_ALERT::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!buffer.Add(level))        return false;
  if(!buffer.Add(description))  return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_ALERT::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Extract one alert from a buffer
* @note       An incomplete alert consumes no bytes.
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_ALERT::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBYTE   newlevel       = 0;
  XBYTE   newdescription = 0;

  if(buffer.GetSize() < 2)
    {
      return false;
    }

  workbuffer.Add(buffer);

  if(!workbuffer.Extract(newlevel))        return false;
  if(!workbuffer.Extract(newdescription))  return false;

  level       = newlevel;
  description = newdescription;

  return (buffer.Extract(NULL, 0, 2) == 2);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_ALERT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_ALERT::Clean()
{
  level       = 0;
  description = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_RECORDHEADER::DIOSTREAMTLS_MSG_RECORDHEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_RECORDHEADER::DIOSTREAMTLS_MSG_RECORDHEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_RECORDHEADER::~DIOSTREAMTLS_MSG_RECORDHEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_RECORDHEADER::~DIOSTREAMTLS_MSG_RECORDHEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_CONTENTTYPE DIOSTREAMTLS_MSG_RECORDHEADER::GetContenType()
* @brief      Get content type
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_CONTENTTYPE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_CONTENTTYPE DIOSTREAMTLS_MSG_RECORDHEADER::GetContenType()
{
  return contenttype;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_RECORDHEADER::SetContenType(DIOSTREAMTLS_CONTENTTYPE contenttype)
* @brief      Set content type
* @ingroup    DATAIO
*
* @param[in]  contenttype : Contenttype value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_RECORDHEADER::SetContenType(DIOSTREAMTLS_CONTENTTYPE contenttype)
{
  this->contenttype = contenttype;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_RECORDHEADER::GetProtocolVersion()
* @brief      Get protocol version
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_RECORDHEADER::GetProtocolVersion()
{
  return protocolversion;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_RECORDHEADER::SetProtocolVersion(XWORD protocolversion)
* @brief      Set protocol version
* @ingroup    DATAIO
*
* @param[in]  protocolversion : Protocolversion value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_RECORDHEADER::SetProtocolVersion(XWORD protocolversion)
{
  this->protocolversion = protocolversion;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_RECORDHEADER::GetLength()
* @brief      Get length of the payload that follows the header
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_RECORDHEADER::GetLength()
{
  return length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_RECORDHEADER::SetLength(XWORD length)
* @brief      Set length of the payload that follows the header
* @ingroup    DATAIO
*
* @param[in]  length : Length value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_RECORDHEADER::SetLength(XWORD length)
{
  this->length = length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_RECORDHEADER::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Add the five bytes of the header to the end of a buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_RECORDHEADER::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!buffer.Add((XBYTE)contenttype))  return false;
  if(!buffer.Add(protocolversion))     return false;
  if(!buffer.Add(length))              return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_RECORDHEADER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Take the five bytes of the header out of the beginning of a buffer, consuming them
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_RECORDHEADER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBYTE _contenttype = 0;

  if(buffer.GetSize() < DIOSTREAMTLS_MSG_RECORDHEADER_SIZE)
    {
      return false;
    }

  buffer.Extract(_contenttype);
  buffer.Extract(protocolversion);
  buffer.Extract(length);

  contenttype = (DIOSTREAMTLS_CONTENTTYPE)_contenttype;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_RECORDHEADER::Peek(XBUFFER& buffer)
* @brief      Read the header at the beginning of a buffer without consuming it and without moving its position
* @note       The record layer needs to know how long a record is before it can tell whether the whole of it has
*             arrived, so it has to look at the header without disturbing the buffer.
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer whose beginning is a record header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_RECORDHEADER::Peek(XBUFFER& buffer)
{
  if(buffer.GetSize() < DIOSTREAMTLS_MSG_RECORDHEADER_SIZE)
    {
      return false;
    }

  XBYTE* data = buffer.Get();

  if(!data)
    {
      return false;
    }

  contenttype     = (DIOSTREAMTLS_CONTENTTYPE)data[0];
  protocolversion = (XWORD)((data[1] << 8) | data[2]);
  length          = (XWORD)((data[3] << 8) | data[4]);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_RECORDHEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_RECORDHEADER::Clean()
{
  contenttype     = (DIOSTREAMTLS_CONTENTTYPE)0;
  protocolversion = 0;
  length          = 0;
}
