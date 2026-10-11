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

  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_UNKNOWN               NL("unknown")
  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_REQUEST               NL("request")
  #define DIOCOREPROTOCOL_HEADER_MESSAGETYPE_STRING_RESPONSE              NL("response")

  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UNKNOWN                 NL("unknown")  
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_KEYEXCHANGE             NL("keyexchange")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_AUTHENTICATE            NL("authenticate")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_REGISTERDATA            NL("registerdata")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYSERVER           NL("toreadyserver")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_TOREADYCLIENT           NL("toreadyclient")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_COMMAND                 NL("command")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_UPDATECLASS             NL("updateclass")
  #define DIOCOREPROTOCOL_HEADER_OPERATION_STRING_ASKUPDATECLASS          NL("askupdateclass")
  
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_NONE                  NL("none")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_BINARY                NL("binary")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_TEXT                  NL("text")
  #define DIOCOREPROTOCOL_HEADER_CONTENTTYPE_STRING_JSON                  NL("json")
  
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_UNKNOWN          NL("unknown")
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_COMMAND          NL("command")
  #define DIOCOREPROTOCOL_HEADER_CONTENTOPERATION_STRING_UPDATE           NL("update")
  
  #define DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE                           NL("ID_message")
  #define DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE                         NL("message_type")  
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION                            NL("operation")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM                      NL("operation_param")
  #define DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND                        NL("datetime_send")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE                         NL("content_type")  
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX                          NL("block_index")
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT                         NL("block_amount")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE                         NL("content_size")         
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE                 NL("content_compresssize")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32                        NL("content_CRC32")
 
#else     

  #define DIOCOREPROTOCOL_HEADER_VAR_ID_MESSAGE                           NL("idm")
  #define DIOCOREPROTOCOL_HEADER_VAR_MESSAGE_TYPE                         NL("mt")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION                            NL("o")
  #define DIOCOREPROTOCOL_HEADER_VAR_OPERATION_PARAM                      NL("op")
  #define DIOCOREPROTOCOL_HEADER_VAR_DATETIME_SEND                        NL("dts")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_TYPE                         NL("ct")  
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_INDEX                          NL("bi")
  #define DIOCOREPROTOCOL_HEADER_VAR_BLOCK_AMOUNT                         NL("ba")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_SIZE                         NL("cs")         
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_COMPRESSSIZE                 NL("ccs")
  #define DIOCOREPROTOCOL_HEADER_VAR_CONTENT_CRC32                        NL("c32")

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






