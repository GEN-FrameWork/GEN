/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSMessagesHandShakeServerFlight.cpp
*
* @class      DIOSTREAMTLSMESSAGESHANDSHAKESERVERFLIGHT
* @brief      Data Input/Output Stream TLS 1.3 Handshake Server Flight Messages classes
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

#include "DIOStreamTLSMessagesHandShakeServerFlight.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions, DIOSTREAMTLS_MSG_EXTENSION* extension)
* @brief      Add an owned extension while rejecting duplicated types
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  extensions : Extension list.
* @param[in]  extension : Extension whose ownership is transferred to the list.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions, DIOSTREAMTLS_MSG_EXTENSION* extension)
{
  if(!extension)
    {
      return false;
    }

  for(XDWORD c=0; c<extensions.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* current = extensions.Get(c);

      if(current && (current->GetType() == extension->GetType()))
        {
          return false;
        }
    }

  return extensions.Add(extension);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions)
* @brief      Delete every owned extension
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  extensions : Extension list.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions)
{
  extensions.DeleteContents();
  extensions.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_SetToBuffer(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions, XBUFFER& buffer, bool showdebug)
* @brief      Encode an extension list
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  extensions : Extension list.
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_SetToBuffer(XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions, XBUFFER& buffer, bool showdebug)
{
  for(XDWORD c=0; c<extensions.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = extensions.Get(c);

      if(!extension || !extension->SetToBuffer(buffer, showdebug))
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_GetFromBuffer(XBUFFER& buffer, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context, XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions)
* @brief      Decode an exact extension vector for its handshake context
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  context : Handshake context used to select the extension codec.
* @param[out] extensions : Decoded extension list.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_GetFromBuffer(XBUFFER& buffer, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context, XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>& extensions)
{
  DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);

  while(!buffer.IsEmpty())
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = NULL;

      if(!DIOSTREAMTLS_MSG_EXTENSION_Extract(buffer, context, extension))
        {
          DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);
          return false;
        }

      if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(extensions, extension))
        {
          GEN_DELETE extension;
          DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::~DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::~DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_GetLength()
* @brief      Get the encoded extension vector length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_GetLength()
{
  return extensionslength;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_GetAll()
* @brief      Get every decoded extension
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* : Pointer to the extension list.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_GetAll()
{
  return &extensions;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
* @brief      Add an extension
* @ingroup    DATAIO
*
* @param[in]  extension : Extension whose ownership is transferred to the message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
{
  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(extensions, extension);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_DeleteAll()
* @brief      Delete every extension
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Extensions_DeleteAll()
{
  extensionslength = 0;

  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode the EncryptedExtensions body
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER extensionbuffer;

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_SetToBuffer(extensions, extensionbuffer, showdebug))
    {
      return false;
    }

  if(extensionbuffer.GetSize() > 0xFFFF)
    {
      return false;
    }

  extensionslength = (XWORD)extensionbuffer.GetSize();

  if(!buffer.Add(extensionslength)) return false;

  return extensionslength?buffer.Add(extensionbuffer):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode an exact EncryptedExtensions body
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBUFFER extensionbuffer;
  XWORD   newextensionslength = 0;
  XDWORD  sizeconsumed        = buffer.GetSize();

  workbuffer.Add(buffer);

  if(!workbuffer.Extract(newextensionslength)) return false;
  if(workbuffer.GetSize() != newextensionslength) return false;

  extensionbuffer.Add(workbuffer);

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_GetFromBuffer(extensionbuffer,
                                                              DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_ENCRYPTEDEXTENSIONS,
                                                              extensions))
    {
      return false;
    }

  extensionslength = newextensionslength;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS::Clean()
{
  Extensions_DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_CERTIFICATEENTRY::DIOSTREAMTLS_MSG_CERTIFICATEENTRY()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_CERTIFICATEENTRY::DIOSTREAMTLS_MSG_CERTIFICATEENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_CERTIFICATEENTRY::~DIOSTREAMTLS_MSG_CERTIFICATEENTRY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_CERTIFICATEENTRY::~DIOSTREAMTLS_MSG_CERTIFICATEENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_CERTIFICATEENTRY::GetCertificateData()
* @brief      Get the DER encoded certificate
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_CERTIFICATEENTRY::GetCertificateData()
{
  return &certificatedata;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_GetLength()
* @brief      Get the encoded certificate extension vector length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_GetLength()
{
  return extensionslength;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_GetAll()
* @brief      Get every certificate extension
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* : Pointer to the extension list.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_GetAll()
{
  return &extensions;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
* @brief      Add a certificate extension
* @ingroup    DATAIO
*
* @param[in]  extension : Extension whose ownership is transferred to the entry.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
{
  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(extensions, extension);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_DeleteAll()
* @brief      Delete every certificate extension
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Extensions_DeleteAll()
{
  extensionslength = 0;

  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode a CertificateEntry
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(certificatedata.IsEmpty() || (certificatedata.GetSize() > DIOSTREAMTLS_MSG_MAXLENGTH24))
    {
      return false;
    }

  XBUFFER extensionbuffer;

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_SetToBuffer(extensions, extensionbuffer, showdebug))
    {
      return false;
    }

  if(extensionbuffer.GetSize() > 0xFFFF)
    {
      return false;
    }

  extensionslength = (XWORD)extensionbuffer.GetSize();

  if(!DIOSTREAMTLS_MSG_AddLength24(buffer, certificatedata.GetSize())) return false;
  if(!buffer.Add(certificatedata))                                   return false;
  if(!buffer.Add(extensionslength))                                  return false;

  return extensionslength?buffer.Add(extensionbuffer):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode one CertificateEntry from the start of a certificate list
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_CERTIFICATEENTRY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBUFFER newcertificatedata;
  XBUFFER extensionbuffer;
  XDWORD  certificatelength   = 0;
  XWORD   newextensionslength = 0;
  XDWORD  sizeconsumed        = 0;

  workbuffer.Add(buffer);

  if(!DIOSTREAMTLS_MSG_ExtractLength24(workbuffer, certificatelength)) return false;
  if(!certificatelength || (workbuffer.GetSize() < (certificatelength + sizeof(XWORD)))) return false;

  if(!newcertificatedata.Resize(certificatelength)) return false;
  if(workbuffer.Extract(newcertificatedata.Get(), 0, certificatelength) != certificatelength) return false;

  if(!workbuffer.Extract(newextensionslength)) return false;
  if(workbuffer.GetSize() < newextensionslength) return false;

  if(newextensionslength && !extensionbuffer.Resize(newextensionslength)) return false;
  if(newextensionslength &&
     (workbuffer.Extract(extensionbuffer.Get(), 0, newextensionslength) != newextensionslength)) return false;

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_GetFromBuffer(extensionbuffer,
                                                              DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CERTIFICATEENTRY,
                                                              extensions))
    {
      return false;
    }

  certificatedata.Delete();
  if(!certificatedata.Add(newcertificatedata)) return false;
  extensionslength = newextensionslength;
  sizeconsumed      = DIOSTREAMTLS_MSG_LENGTH24_SIZE + certificatelength + sizeof(XWORD) + newextensionslength;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_CERTIFICATEENTRY::Clean()
{
  certificatedata.Delete();
  Extensions_DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::GetRequestContext()
* @brief      Get the certificate request context
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::GetRequestContext()
{
  return &requestcontext;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::SetRequestContext(XBYTE* context, XBYTE size)
* @brief      Set the certificate request context
* @ingroup    DATAIO
*
* @param[in]  context : Context data.
* @param[in]  size : Context size in bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::SetRequestContext(XBYTE* context, XBYTE size)
{
  if(!context && size)
    {
      return false;
    }

  requestcontext.Delete();

  return size?requestcontext.Add(context, size):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_GetLength()
* @brief      Get the encoded certificate list length
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_GetLength()
{
  return certificatelistlength;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_MSG_CERTIFICATEENTRY*>* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_GetAll()
* @brief      Get every certificate entry
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_MSG_CERTIFICATEENTRY*>* : Pointer to the certificate list.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_CERTIFICATEENTRY*>* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_GetAll()
{
  return &certificatelist;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_Add(DIOSTREAMTLS_MSG_CERTIFICATEENTRY* certificate)
* @brief      Add a certificate entry
* @ingroup    DATAIO
*
* @param[in]  certificate : Entry whose ownership is transferred to the message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_Add(DIOSTREAMTLS_MSG_CERTIFICATEENTRY* certificate)
{
  if(!certificate)
    {
      return false;
    }

  return certificatelist.Add(certificate);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_DeleteAll()
* @brief      Delete every certificate entry
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::CertificateList_DeleteAll()
{
  certificatelist.DeleteContents();
  certificatelist.DeleteAll();
  certificatelistlength = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode the Certificate body
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(requestcontext.GetSize() > 0xFF)
    {
      return false;
    }

  XBUFFER certificatelistbuffer;

  for(XDWORD c=0; c<certificatelist.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_CERTIFICATEENTRY* certificate = certificatelist.Get(c);

      if(!certificate || !certificate->SetToBuffer(certificatelistbuffer, showdebug))
        {
          return false;
        }
    }

  if(certificatelistbuffer.GetSize() > DIOSTREAMTLS_MSG_MAXLENGTH24)
    {
      return false;
    }

  certificatelistlength = certificatelistbuffer.GetSize();

  if(!buffer.Add((XBYTE)requestcontext.GetSize()))                    return false;
  if(!requestcontext.IsEmpty() && !buffer.Add(requestcontext))        return false;
  if(!DIOSTREAMTLS_MSG_AddLength24(buffer, certificatelistlength))    return false;

  return certificatelistlength?buffer.Add(certificatelistbuffer):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode an exact Certificate body
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBUFFER newrequestcontext;
  XBUFFER certificatelistbuffer;
  XBYTE   requestcontextlength   = 0;
  XDWORD  newcertificatelistlength = 0;
  XDWORD  sizeconsumed           = buffer.GetSize();

  workbuffer.Add(buffer);

  if(!workbuffer.Extract(requestcontextlength)) return false;
  if(workbuffer.GetSize() < requestcontextlength) return false;

  if(requestcontextlength && !newrequestcontext.Resize(requestcontextlength)) return false;
  if(requestcontextlength &&
     (workbuffer.Extract(newrequestcontext.Get(), 0, requestcontextlength) != requestcontextlength)) return false;

  if(!DIOSTREAMTLS_MSG_ExtractLength24(workbuffer, newcertificatelistlength)) return false;
  if(workbuffer.GetSize() != newcertificatelistlength) return false;

  certificatelistbuffer.Add(workbuffer);
  CertificateList_DeleteAll();

  while(!certificatelistbuffer.IsEmpty())
    {
      DIOSTREAMTLS_MSG_CERTIFICATEENTRY* certificate = GEN_NEW DIOSTREAMTLS_MSG_CERTIFICATEENTRY();
      if(!certificate)
        {
          CertificateList_DeleteAll();
          return false;
        }

      if(!certificate->GetFromBuffer(certificatelistbuffer, showdebug) || !CertificateList_Add(certificate))
        {
          GEN_DELETE certificate;
          CertificateList_DeleteAll();
          return false;
        }
    }

  requestcontext.Delete();
  if(requestcontextlength && !requestcontext.Add(newrequestcontext)) return false;
  certificatelistlength = newcertificatelistlength;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE::Clean()
{
  requestcontext.Delete();
  CertificateList_DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::GetRequestContext()
* @brief      Get the certificate request context
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::GetRequestContext()
{
  return &requestcontext;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::SetRequestContext(XBYTE* context, XBYTE size)
* @brief      Set the certificate request context
* @ingroup    DATAIO
*
* @param[in]  context : Context data.
* @param[in]  size : Context size in bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::SetRequestContext(XBYTE* context, XBYTE size)
{
  if(!context && size)
    {
      return false;
    }

  requestcontext.Delete();

  return size?requestcontext.Add(context, size):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_GetLength()
* @brief      Get the encoded extension vector length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_GetLength()
{
  return extensionslength;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_GetAll()
* @brief      Get every certificate request extension
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* : Pointer to the extension list.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_GetAll()
{
  return &extensions;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
* @brief      Add a certificate request extension
* @ingroup    DATAIO
*
* @param[in]  extension : Extension whose ownership is transferred to the message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_Add(DIOSTREAMTLS_MSG_EXTENSION* extension)
{
  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_Add(extensions, extension);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_DeleteAll()
* @brief      Delete every certificate request extension
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Extensions_DeleteAll()
{
  extensionslength = 0;

  return DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_DeleteAll(extensions);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode the CertificateRequest body
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(requestcontext.GetSize() > 0xFF)
    {
      return false;
    }

  XBUFFER extensionbuffer;

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_SetToBuffer(extensions, extensionbuffer, showdebug))
    {
      return false;
    }

  if(extensionbuffer.IsEmpty() || (extensionbuffer.GetSize() > 0xFFFF))
    {
      return false;
    }

  extensionslength = (XWORD)extensionbuffer.GetSize();

  if(!buffer.Add((XBYTE)requestcontext.GetSize()))                    return false;
  if(!requestcontext.IsEmpty() && !buffer.Add(requestcontext))        return false;
  if(!buffer.Add(extensionslength))                                  return false;

  return buffer.Add(extensionbuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode an exact CertificateRequest body
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBUFFER newrequestcontext;
  XBUFFER extensionbuffer;
  XBYTE   requestcontextlength   = 0;
  XWORD   newextensionslength    = 0;
  XDWORD  sizeconsumed           = buffer.GetSize();

  workbuffer.Add(buffer);

  if(!workbuffer.Extract(requestcontextlength)) return false;
  if(workbuffer.GetSize() < requestcontextlength) return false;

  if(requestcontextlength && !newrequestcontext.Resize(requestcontextlength)) return false;
  if(requestcontextlength &&
     (workbuffer.Extract(newrequestcontext.Get(), 0, requestcontextlength) != requestcontextlength)) return false;

  if(!workbuffer.Extract(newextensionslength)) return false;
  if(!newextensionslength || (workbuffer.GetSize() != newextensionslength)) return false;

  extensionbuffer.Add(workbuffer);

  if(!DIOSTREAMTLS_MSG_SERVERFLIGHT_Extensions_GetFromBuffer(extensionbuffer,
                                                              DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CERTIFICATEREQUEST,
                                                              extensions))
    {
      return false;
    }

  requestcontext.Delete();
  if(requestcontextlength && !requestcontext.Add(newrequestcontext)) return false;
  extensionslength = newextensionslength;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST::Clean()
{
  requestcontext.Delete();
  Extensions_DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetAlgorithm()
* @brief      Get the signature algorithm
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetAlgorithm()
{
  return algorithm;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::SetAlgorithm(XWORD algorithm)
* @brief      Set the signature algorithm
* @ingroup    DATAIO
*
* @param[in]  algorithm : TLS SignatureScheme value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::SetAlgorithm(XWORD algorithm)
{
  this->algorithm = algorithm;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetSignature()
* @brief      Get the encoded signature
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetSignature()
{
  return &signature;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode the CertificateVerify body
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(signature.IsEmpty() || (signature.GetSize() > 0xFFFF))
    {
      return false;
    }

  if(!buffer.Add(algorithm))                        return false;
  if(!buffer.Add((XWORD)signature.GetSize()))       return false;

  return buffer.Add(signature);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode an exact CertificateVerify body
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER workbuffer;
  XBUFFER newsignature;
  XWORD   newalgorithm = 0;
  XWORD   signaturesize = 0;
  XDWORD  sizeconsumed = buffer.GetSize();

  workbuffer.Add(buffer);

  if(!workbuffer.Extract(newalgorithm)) return false;
  if(!workbuffer.Extract(signaturesize)) return false;
  if(!signaturesize || (workbuffer.GetSize() != signaturesize)) return false;

  if(!newsignature.Resize(signaturesize)) return false;
  if(workbuffer.Extract(newsignature.Get(), 0, signaturesize) != signaturesize) return false;

  algorithm = newalgorithm;
  signature.Delete();
  if(!signature.Add(newsignature)) return false;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY::Clean()
{
  algorithm = 0;
  signature.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::~DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::~DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::GetVerifyData()
* @brief      Get the Finished verify data
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::GetVerifyData()
{
  return &verifydata;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Encode the Finished body
* @ingroup    DATAIO
*
* @param[in]  buffer : Destination buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(verifydata.IsEmpty() || (verifydata.GetSize() > DIOSTREAMTLS_MSG_FINISHED_MAXSIZE))
    {
      return false;
    }

  return buffer.Add(verifydata);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Decode an exact Finished body
* @ingroup    DATAIO
*
* @param[in]  buffer : Source buffer.
* @param[in]  showdebug : Show debug information.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  XDWORD sizeconsumed = buffer.GetSize();

  if(!sizeconsumed || (sizeconsumed > DIOSTREAMTLS_MSG_FINISHED_MAXSIZE))
    {
      return false;
    }

  verifydata.Delete();
  if(!verifydata.Add(buffer)) return false;

  return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED::Clean()
{
  verifydata.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::~DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::~DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketLifetime()
* @brief      Get ticket lifetime
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketLifetime()
{
  return ticketlifetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetTicketLifetime(XDWORD lifetime)
* @brief      Set ticket lifetime
* @ingroup    DATAIO
* 
* @param[in]  lifetime : Lifetime value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetTicketLifetime(XDWORD lifetime)
{
  ticketlifetime = lifetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketAgeAdd()
* @brief      Get ticket age add
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketAgeAdd()
{
  return ticketageadd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetTicketAgeAdd(XDWORD ageadd)
* @brief      Set ticket age add
* @ingroup    DATAIO
* 
* @param[in]  ageadd : Ageadd value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetTicketAgeAdd(XDWORD ageadd)
{
  ticketageadd = ageadd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketNonce()
* @brief      Get ticket nonce
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicketNonce()
{
  return &ticketnonce;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicket()
* @brief      Get ticket
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetTicket()
{
  return &ticket;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetMaximumEarlyDataSize()
* @brief      Get maximum early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetMaximumEarlyDataSize()
{
  return maximumearlydatasize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetMaximumEarlyDataSize(XDWORD size)
* @brief      Set maximum early data size
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetMaximumEarlyDataSize(XDWORD size)
{
  maximumearlydatasize = size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  (void)showdebug;

  if((ticketlifetime > 604800) || ticketnonce.GetSize() > 255 || ticket.IsEmpty() || ticket.GetSize() > 0xFFFF)
    {
      return false;
    }

  XWORD extensionslength = maximumearlydatasize ? 8 : 0;
  if(!buffer.Add(ticketlifetime) || !buffer.Add(ticketageadd) ||
     !buffer.Add((XBYTE)ticketnonce.GetSize()) || !buffer.Add(ticketnonce) ||
     !buffer.Add((XWORD)ticket.GetSize()) || !buffer.Add(ticket) ||
     !buffer.Add(extensionslength))
    {
      return false;
    }

  if(maximumearlydatasize)
    {
      if(!buffer.Add((XWORD)DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA) ||
         !buffer.Add((XWORD)4) || !buffer.Add(maximumearlydatasize)) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  (void)showdebug;

  XBYTE nonce_length = 0;
  XWORD ticket_length = 0;
  XWORD extensions_length = 0;

  ticketnonce.Delete();
  ticket.Delete();
  maximumearlydatasize = 0;

  if(!buffer.Extract(ticketlifetime) || !buffer.Extract(ticketageadd) ||
     (ticketlifetime > 604800) || !buffer.Extract(nonce_length) ||
     (buffer.GetSize() < ((XDWORD)nonce_length + sizeof(XWORD))))
    {
      return false;
    }

  if(nonce_length)
    {
      if(!ticketnonce.Resize(nonce_length) || buffer.Extract(ticketnonce.Get(), 0, nonce_length) != nonce_length) return false;
    }

  if(!buffer.Extract(ticket_length) || !ticket_length ||
     (buffer.GetSize() < ((XDWORD)ticket_length + sizeof(XWORD))))
    {
      return false;
    }

  if(!ticket.Resize(ticket_length) || buffer.Extract(ticket.Get(), 0, ticket_length) != ticket_length) return false;
  if(!buffer.Extract(extensions_length) || buffer.GetSize() != extensions_length) return false;

  maximumearlydatasize = 0;
  while(buffer.GetSize())
    {
      XWORD extensiontype = 0;
      XWORD extensionlength = 0;
      if(buffer.GetSize() < 4 || !buffer.Extract(extensiontype) || !buffer.Extract(extensionlength) ||
         buffer.GetSize() < extensionlength) return false;

      if(extensiontype == DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA)
        {
          if(maximumearlydatasize || extensionlength != 4 || !buffer.Extract(maximumearlydatasize)) return false;
        }
       else
        {
          if(buffer.Extract(NULL, 0, extensionlength) != extensionlength) return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::Clean()
* @brief      Clean
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET::Clean()
{
  ticketlifetime = 0;
  ticketageadd    = 0;
  ticketnonce.Delete();
  ticket.Delete();
  maximumearlydatasize = 0;
}
