/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_Header.cpp
* 
* @class      DIOCOREPROTOCOL_HEADER
* @brief      Data Input/Output Core Protocol Header class
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

#include "DIOCoreProtocol_Header.h"

#include "XFactory.h"
#include "XString.h"
#include "XDateTime.h"

#include "HashCRC32.h"

#include "CompressManager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER::DIOCOREPROTOCOL_HEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER::DIOCOREPROTOCOL_HEADER()
{
  Clean();

  serializationmethod = XSERIALIZABLE::CreateInstance(xfileJSON);
  if(serializationmethod)
    {
      SetSerializationMethod(serializationmethod);
    }

  datetime_send = GEN_XFACTORY.CreateDateTime();
  if(datetime_send)
    {
      datetime_send->Read();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER::~DIOCOREPROTOCOL_HEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER::~DIOCOREPROTOCOL_HEADER()
{
  if(serializationmethod)
    {
      GEN_DELETE serializationmethod;
    }

  if(datetime_send)
    {
      GEN_XFACTORY.DeleteDateTime(datetime_send);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID* DIOCOREPROTOCOL_HEADER::GetIDMessage()
* @brief      Get ID message
* @ingroup    DATAIO
* 
* @return     XUUID* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID* DIOCOREPROTOCOL_HEADER::GetIDMessage()
{
  return &ID_message;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER_MESSAGETYPE DIOCOREPROTOCOL_HEADER::GetMessageType()
* @brief      Get message type
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_HEADER_MESSAGETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER_MESSAGETYPE DIOCOREPROTOCOL_HEADER::GetMessageType()
{
  return message_type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetMessageType(DIOCOREPROTOCOL_HEADER_MESSAGETYPE message_type)
* @brief      Set message type
* @ingroup    DATAIO
* 
* @param[in]  message_type : Message type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetMessageType(DIOCOREPROTOCOL_HEADER_MESSAGETYPE message_type)
{
  this->message_type = message_type;
}


#ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetMessageTypeToString(XSTRING& message_typestr)
* @brief      Get message type to string
* @ingroup    DATAIO
* 
* @param[in]  message_typestr : Message typestr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetMessageTypeToString(XSTRING& message_typestr)
{
  message_typestr.Empty();

  switch(message_type)
    {
      case DIOCOREPROTOCOL_HEADER_MESSAGETYPE_UNKNOWN  : message_typestr = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_UNKNOWN;  break;
      case DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST  : message_typestr = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_REQUEST;  break;
      case DIOCOREPROTOCOL_HEADER_MESSAGETYPE_RESPONSE : message_typestr = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_RESPONSE; break;      
    }

  return message_typestr.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetMessageTypeFromString(XSTRING* message_typestr, DIOCOREPROTOCOL_HEADER_MESSAGETYPE& message_type)
* @brief      Get message type from string
* @ingroup    DATAIO
* 
* @param[in]  message_typestr : Message typestr pointer to use.
* @param[in]  message_type : Message type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetMessageTypeFromString(XSTRING* message_typestr, DIOCOREPROTOCOL_HEADER_MESSAGETYPE& message_type)
{
  if(!message_typestr)
    {
      return false;
    }

  message_type = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_UNKNOWN;

  if(!message_typestr->Compare(DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_UNKNOWN))
    {
      message_type = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_UNKNOWN;
    }

  if(!message_typestr->Compare(DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_REQUEST))
    {
      message_type = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST;
    }

  if(!message_typestr->Compare(DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_RESPONSE))
    {
      message_type = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_RESPONSE;
    }

  return message_type;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER_OPERATION DIOCOREPROTOCOL_HEADER::GetOperation()
* @brief      Get operation
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_HEADER_OPERATION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER_OPERATION DIOCOREPROTOCOL_HEADER::GetOperation()
{
  return operation;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetOperation(DIOCOREPROTOCOL_HEADER_OPERATION operation)
* @brief      Set operation
* @ingroup    DATAIO
* 
* @param[in]  operation : Operation value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetOperation(DIOCOREPROTOCOL_HEADER_OPERATION operation)
{
  this->operation = operation;
}


#ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetOperationToString(XSTRING& operationstr)
* @brief      Get operation to string
* @ingroup    DATAIO
* 
* @param[in]  operationstr : Operationstr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetOperationToString(XSTRING& operationstr)
{
  operationstr.Empty();

  switch(operation)
    {
      case DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN         : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UNKNOWN;                     break;
      case DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE     : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_KEYEXCHANGE;                 break;       
      case DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE    : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_AUTHENTICATE;                break;       
      case DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA    : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_REGISTERDATA;                break;       
      case DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYSERVER   : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYSERVER;               break;       
      case DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYCLIENT   : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYCLIENT;               break;       
      case DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND         : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_COMMAND;                     break;
      case DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS     : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UPDATECLASS;                 break;      
      case DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS  : operationstr = DIOCOREPROTOCOL_HEADER_OPERATION_STRING_ASKUPDATECLASS;              break;      
    }

  return operationstr.IsEmpty()?false:true;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetOperationFromString(XSTRING* operationstr, DIOCOREPROTOCOL_HEADER_OPERATION& operation)
* @brief      Get operation from string
* @ingroup    DATAIO
* 
* @param[in]  operationstr : Operationstr pointer to use.
* @param[in]  operation : Operation value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetOperationFromString(XSTRING* operationstr, DIOCOREPROTOCOL_HEADER_OPERATION& operation)
{
  if(!operationstr)
    {
      return false;
    }

  operation = DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN;

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UNKNOWN))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_KEYEXCHANGE))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_AUTHENTICATE))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_REGISTERDATA))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYSERVER))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYSERVER;
    }

 if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYCLIENT))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYCLIENT;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_COMMAND))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UPDATECLASS))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS;
    }

  if(!operationstr->Compare(DIOCOREPROTOCOL_HEADER_OPERATION_STRING_ASKUPDATECLASS))
    {
      operation = DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS;
    }

  return operation;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_HEADER::GetOperationParam()
* @brief      Get operation param
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_HEADER::GetOperationParam()
{
  return &operation_param;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* DIOCOREPROTOCOL_HEADER::GetDateTimeSend()
* @brief      Get date time send
* @ingroup    DATAIO
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* DIOCOREPROTOCOL_HEADER::GetDateTimeSend()
{
  return datetime_send;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER_CONTENTTYPE DIOCOREPROTOCOL_HEADER::GetContentType()
* @brief      Get content type
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_HEADER_CONTENTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER_CONTENTTYPE DIOCOREPROTOCOL_HEADER::GetContentType()
{
  return content_type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE content_type)
* @brief      Set content type
* @ingroup    DATAIO
* 
* @param[in]  content_type : Content type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE content_type)
{
  this->content_type = content_type;
}


#ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetContentTypeToString(XSTRING& content_typestr)
* @brief      Get content type to string
* @ingroup    DATAIO
* 
* @param[in]  content_typestr : Content typestr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetContentTypeToString(XSTRING& content_typestr)
{
  content_typestr.Empty();

  switch(content_type)
    {
      case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE     : content_typestr = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_NONE;     break;
      case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY   : content_typestr = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_BINARY;   break;
      case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT     : content_typestr = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_TEXT;     break;
      case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON     : content_typestr = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_JSON;     break;
    }

  return content_typestr.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::GetContentTypeFromString(XSTRING* content_typestr, DIOCOREPROTOCOL_HEADER_CONTENTTYPE& content_type)
* @brief      Get content type from string
* @ingroup    DATAIO
* 
* @param[in]  content_typestr : Content typestr pointer to use.
* @param[in]  content_type : Content type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::GetContentTypeFromString(XSTRING* content_typestr, DIOCOREPROTOCOL_HEADER_CONTENTTYPE& content_type)
{
  if(!content_typestr)
    {
      return false;
    }

  content_type = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE;

  if(!content_typestr->Compare(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_NONE))
    {
      content_type = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE;
    }

  if(!content_typestr->Compare(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_BINARY))
    {
      content_type = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY;
    }

  if(!content_typestr->Compare(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_TEXT))
    {
      content_type = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT;
    }

  if(!content_typestr->Compare(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_JSON))
    {
      content_type = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON;
    }

  return content_type;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_HEADER::GetBlockIndex()
* @brief      Get block index
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_HEADER::GetBlockIndex()
{
  return block_index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetBlockIndex(XDWORD block_index)
* @brief      Set block index
* @ingroup    DATAIO
* 
* @param[in]  block_index : Block index value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetBlockIndex(XDWORD block_index)
{
  this->block_index  = block_index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_HEADER::GetBlockAmount()
* @brief      Get block amount
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_HEADER::GetBlockAmount()
{
  return block_amount;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetBlockAmount(XDWORD block_amount)
* @brief      Set block amount
* @ingroup    DATAIO
* 
* @param[in]  block_amount : Block amount value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetBlockAmount(XDWORD block_amount)
{
  this->block_amount = block_amount;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_HEADER::GetContentSize()
* @brief      Get content size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_HEADER::GetContentSize()
{
  return content_size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetContentSize(XDWORD content_size)
* @brief      Set content size
* @ingroup    DATAIO
* 
* @param[in]  content_size : Content size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetContentSize(XDWORD content_size)
{
  this->content_size = content_size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_HEADER::GetContentCompressSize()
* @brief      Get content compress size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_HEADER::GetContentCompressSize()
{
  return content_compresssize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetContentCompressSize(XDWORD content_compresssize)
* @brief      Set content compress size
* @ingroup    DATAIO
* 
* @param[in]  content_compresssize : Content compresssize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetContentCompressSize(XDWORD content_compresssize)
{
  this->content_compresssize = content_compresssize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_HEADER::GetContentCRC32()
* @brief      Get content CRC32
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_HEADER::GetContentCRC32()
{
  return content_CRC32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::SetContentCRC32(XDWORD content_CRC32)
* @brief      Set content CRC32
* @ingroup    DATAIO
* 
* @param[in]  content_CRC32 : Content CRC32 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::SetContentCRC32(XDWORD content_CRC32)
{
  this->content_CRC32 = content_CRC32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSON* DIOCOREPROTOCOL_HEADER::GetSerializationXFileJSON()
* @brief      Get serialization X file JSON
* @ingroup    DATAIO
* 
* @return     XFILEJSON* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSON* DIOCOREPROTOCOL_HEADER::GetSerializationXFileJSON()
{
  return &xfileJSON;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSERIALIZATIONMETHOD* DIOCOREPROTOCOL_HEADER::GetSerializationMethod()
* @brief      Get serialization method
* @ingroup    DATAIO
* 
* @return     XSERIALIZATIONMETHOD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSERIALIZATIONMETHOD* DIOCOREPROTOCOL_HEADER::GetSerializationMethod()
{
  return serializationmethod;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::CopyFrom(DIOCOREPROTOCOL_HEADER* header)
* @brief      Copy from
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::CopyFrom(DIOCOREPROTOCOL_HEADER* header)
{
  if(!header)
    {
      return false;
    }
  
  ID_message.CopyFrom((*header->GetIDMessage()));
  
  message_type          = header->GetMessageType();

  operation             = header->GetOperation();
  operation_param       = header->GetOperationParam()->Get();  

  datetime_send->CopyFrom(header->GetDateTimeSend());
    
  content_type          = header->GetContentType(); 
 
  block_index           = header->GetBlockIndex();
  block_amount          = header->GetBlockAmount();
  content_size          = header->GetContentSize();
  content_compresssize  = header->GetContentCompressSize();          
  content_CRC32         = header->GetContentCRC32();          

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::CopyTo(DIOCOREPROTOCOL_HEADER* header)
* @brief      Copy to
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::CopyTo(DIOCOREPROTOCOL_HEADER* header)
{
  if(!header)
    {
      return false;
    }
  
  ID_message.CopyTo((*header->GetIDMessage()));  
  
  header->SetMessageType(message_type);
  
  header->SetOperation(operation);
  header->GetOperationParam()->Set(operation_param);  

  datetime_send->CopyTo(header->GetDateTimeSend());
    
  header->SetContentType(content_type); 

  header->SetBlockIndex(block_index);
  header->SetBlockAmount(block_amount);
  header->SetContentSize(content_size);
  header->SetContentCompressSize(content_compresssize);          
  header->SetContentCRC32(content_CRC32);            

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::Compare(DIOCOREPROTOCOL_HEADER* header)
* @brief      Compare
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::Compare(DIOCOREPROTOCOL_HEADER* header)
{
  if(!header)
    {
      return false;
    }

  if(!ID_message.Compare((*header->GetIDMessage())))
    {
      return false;
    }
 
  if(message_type != header->GetMessageType())
    {
      return false;
    } 

  if(operation != header->GetOperation())
    {
      return false;
    }

  if(operation_param.Compare(header->GetOperationParam()->Get()))
    {
      return false;
    }

  if(datetime_send->Compare((*header->GetDateTimeSend())))
    {
      return false;
    }
    
  if(content_type != header->GetContentType())
    {
      return false;
    }
 
  if(block_index != header->GetBlockIndex())
    {
      return false;
    }

  if(block_amount != header->GetBlockAmount())
    {
      return false;
    }

  if(content_size != header->GetContentSize())
    {
      return false;
    }

  if(content_compresssize != header->GetContentCompressSize())
    {
      return false;
    }

  if(content_CRC32 != header->GetContentCRC32())   
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::Serialize()
* @brief      Serialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::Serialize()
{
  XSTRING   string; 
  XVARIANT  data;

  ID_message.GetToString(string);
  Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE);

  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE           
    GetMessageTypeToString(string);  
    Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE);    
  #else    
    Primitive_Add<int>(message_type, DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE);
  #endif
  
  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE           
    GetOperationToString(string);  
    Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_OPERATION);    
  #else    
    Primitive_Add<int>(operation, DIOCOREPROTOCOL_HEADER_VAR_OPERATION);
  #endif
  
  Primitive_Add<XSTRING*>(&operation_param, DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM);

  datetime_send->GetDateTimeToStringISO8601(XDATETIME_FORMAT_ISO8601_STANDARD | XDATETIME_FORMAT_TIMEWITHMILLISECONDS, string);
  Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND);

  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE                 
    GetContentTypeToString(string);  
    Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE);   
  #else
    Primitive_Add<int>(content_type, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE);
  #endif  
  
  Primitive_Add<XDWORD>(block_index, DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX);

  Primitive_Add<XDWORD>(block_amount, DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT);
      
  Primitive_Add<XDWORD>(content_size, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE);

  Primitive_Add<XDWORD>(content_compresssize, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE);
   
  string.Format(__L("%08X"), content_CRC32);
  Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_HEADER::Deserialize()
* @brief      Deserialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_HEADER::Deserialize()
{
  XSTRING string;
  
  string.Empty();
  Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE);
  ID_message.SetFromString(string);

  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE);
    GetMessageTypeFromString(&string, message_type);       
  #else
    Primitive_Extract<int>((int&)message_type, DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE);
  #endif
  
  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_OPERATION);
    GetOperationFromString(&string, operation);    
  #else
    Primitive_Extract<int>((int&)operation, DIOCOREPROTOCOL_HEADER_VAR_OPERATION);
  #endif

  Primitive_Extract<XSTRING>(operation_param, DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM);

  string.Empty();
  Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND);
  datetime_send->GetDateTimeFromStringISO8601(string, XDATETIME_FORMAT_ISO8601_STANDARD | XDATETIME_FORMAT_TIMEWITHMILLISECONDS);
  
  #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE);
    GetContentTypeFromString(&string, content_type); 
  #else
    Primitive_Extract<int>((int&)content_type, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE);
  #endif

  Primitive_Extract<XDWORD>(block_index, DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX);

  Primitive_Extract<XDWORD>(block_amount, DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT);

  Primitive_Extract<XDWORD>(content_size, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE);
         
  Primitive_Extract<XDWORD>(content_compresssize, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE);
  
  string.Empty();
  Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32);
  string.UnFormat(__L("%08X"), &content_CRC32);
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_HEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_HEADER::Clean()
{ 
  message_type          = DIOCOREPROTOCOL_HEADER_MESSAGETYPE_UNKNOWN; 
  operation             = DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN;  
  datetime_send         = NULL;
  content_type          = DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE;   
  block_index           = 1;
  block_amount          = 1;
  content_size          = 0;
  content_compresssize  = 0;          
  content_CRC32         = 0;       
}

    


