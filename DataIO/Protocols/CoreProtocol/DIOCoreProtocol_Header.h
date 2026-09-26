/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_Header.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XUUID.h"
#include "XFileJSON.h"
#include "XSerializable.h"
#include "XSerializationMethod.h"

#include "DIOStream.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOCOREPROTOCOL_HEADER_MESSAGETYPE
{
  DIOCOREPROTOCOL_HEADER_MESSAGETYPE_UNKNOWN            = 0 ,
  DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST                ,
  DIOCOREPROTOCOL_HEADER_MESSAGETYPE_RESPONSE              
};
enum DIOCOREPROTOCOL_HEADER_OPERATION
{
  DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN              = 0 ,
  DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE              ,
  DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE             ,  
  DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA             ,  
  DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYSERVER            ,  
  DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYCLIENT            ,  
  DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND                  ,
  DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS              ,    
  DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS             
};

enum DIOCOREPROTOCOL_HEADER_CONTENTTYPE
{
  DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE               = 0 ,
  DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY                 ,
  DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT                   ,
  DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON                   ,  
};


#define DIOCOREPROTOCOL_HEADER_MAGIC_ID                                   0x47454E31  // GEN1
#define DIOCOREPROTOCOL_HEADER_SIZE_ID                                    (sizeof(XDWORD)+(sizeof(XWORD)*2)+sizeof(XDWORD))


#ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE

  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_UNKNOWN               __L("unknown")
  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_REQUEST               __L("request")
  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_RESPONSE              __L("response")

  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UNKNOWN                 __L("unknown")  
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_KEYEXCHANGE             __L("keyexchange")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_AUTHENTICATE            __L("authenticate")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_REGISTERDATA            __L("registerdata")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYSERVER           __L("toreadyserver")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYCLIENT           __L("toreadyclient")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_COMMAND                 __L("command")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UPDATECLASS             __L("updateclass")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_ASKUPDATECLASS          __L("askupdateclass")
  
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_NONE                  __L("none")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_BINARY                __L("binary")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_TEXT                  __L("text")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_JSON                  __L("json")
  
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_UNKNOWN          __L("unknown")
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_COMMAND          __L("command")
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_UPDATE           __L("update")
  
  #define DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE                           __L("ID_message")
  #define DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE                         __L("message_type")  
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION                            __L("operation")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM                      __L("operation_param")
  #define DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND                        __L("datetime_send")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE                         __L("content_type")  
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX                          __L("block_index")
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT                         __L("block_amount")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE                         __L("content_size")         
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE                 __L("content_compresssize")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32                        __L("content_CRC32")
 
#else     

  #define DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE                           __L("idm")
  #define DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE                         __L("mt")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION                            __L("o")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM                      __L("op")
  #define DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND                        __L("dts")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE                         __L("ct")  
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX                          __L("bi")
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT                         __L("ba")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE                         __L("cs")         
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE                 __L("ccs")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32                        __L("c32")

#endif

/*---- CLASS ---------------------------------------------------------c------------------------------------------------*/

class COMPRESSMANAGER;
class COMPRESSBASE;    	

class DIOCOREPROTOCOL_HEADER : public XSERIALIZABLE
{
  public:
                                                  DIOCOREPROTOCOL_HEADER        ();
    virtual                                      ~DIOCOREPROTOCOL_HEADER        ();

    XUUID*                                        GetIDMessage                  ();
   
    DIOCOREPROTOCOL_HEADER_MESSAGETYPE            GetMessageType                (); 
    void                                          SetMessageType                (DIOCOREPROTOCOL_HEADER_MESSAGETYPE message_type); 
    #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    bool                                          GetMessageTypeToString        (XSTRING& message_typestr); 
    bool                                          GetMessageTypeFromString      (XSTRING* message_typestr, DIOCOREPROTOCOL_HEADER_MESSAGETYPE& message_type);
    #endif

    DIOCOREPROTOCOL_HEADER_OPERATION              GetOperation                  ();
    void                                          SetOperation                  (DIOCOREPROTOCOL_HEADER_OPERATION operation);
    #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    bool                                          GetOperationToString          (XSTRING& operationstr); 
    bool                                          GetOperationFromString        (XSTRING* operationstr, DIOCOREPROTOCOL_HEADER_OPERATION& operation);
    #endif
    
    XSTRING*                                      GetOperationParam             (); 
    
    XDATETIME*                                    GetDateTimeSend               ();

    DIOCOREPROTOCOL_HEADER_CONTENTTYPE            GetContentType                (); 
    void                                          SetContentType                (DIOCOREPROTOCOL_HEADER_CONTENTTYPE content_type); 
    #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
    bool                                          GetContentTypeToString        (XSTRING& content_typestr); 
    bool                                          GetContentTypeFromString      (XSTRING* content_typestr, DIOCOREPROTOCOL_HEADER_CONTENTTYPE& content_type);   
    #endif

    XDWORD                                        GetBlockIndex                 ();
    void                                          SetBlockIndex                 (XDWORD block_index);

    XDWORD                                        GetBlockAmount                ();
    void                                          SetBlockAmount                (XDWORD block_amount);

    XDWORD                                        GetContentSize                ();        
    void                                          SetContentSize                (XDWORD content_size);        

    XDWORD                                        GetContentCompressSize        ();        
    void                                          SetContentCompressSize        (XDWORD content_compresssize);        

    XDWORD                                        GetContentCRC32               ();        
    void                                          SetContentCRC32               (XDWORD content_CRC32);        

    XFILEJSON*                                    GetSerializationXFileJSON     ();      
    XSERIALIZATIONMETHOD*                         GetSerializationMethod        ();

    bool                                          CopyFrom                      (DIOCOREPROTOCOL_HEADER* header);    
    bool                                          CopyTo                        (DIOCOREPROTOCOL_HEADER* header);    
    bool                                          Compare                       (DIOCOREPROTOCOL_HEADER* header);   

    bool                                          Serialize                     ();    
    bool                                          Deserialize                   ();  
   
  private:

    void                                          Clean                         ();
        
    XUUID                                         ID_message;
    DIOCOREPROTOCOL_HEADER_MESSAGETYPE            message_type;  
    DIOCOREPROTOCOL_HEADER_OPERATION              operation;
    XSTRING                                       operation_param;
    XDATETIME*                                    datetime_send;
    DIOCOREPROTOCOL_HEADER_CONTENTTYPE            content_type;   
    XDWORD                                        block_index;
    XDWORD                                        block_amount;
    XDWORD                                        content_size;
    XDWORD                                        content_compresssize;          
    XDWORD                                        content_CRC32;          

    XFILEJSON                                     xfileJSON;      
    XSERIALIZATIONMETHOD*                         serializationmethod;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






