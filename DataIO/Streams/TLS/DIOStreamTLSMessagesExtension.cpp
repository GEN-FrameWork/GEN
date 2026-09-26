/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSMessagesExtension.cpp
* 
* @class      DIOSTREAMTLSMESSAGES
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Messages Extension class
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

#include "DIOStreamTLSMessagesExtension.h"

#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION::DIOSTREAMTLS_MSG_EXTENSION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION::DIOSTREAMTLS_MSG_EXTENSION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION::~DIOSTREAMTLS_MSG_EXTENSION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION::~DIOSTREAMTLS_MSG_EXTENSION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION::GetType()
* @brief      get type
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION::SetType(XWORD type)
* @brief      set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION::SetType(XWORD type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION::GetLength()
* @brief      get length
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION::GetLength()
{
  return length;    
} 
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION::SetLength(XWORD length)
* @brief      set length
* @ingroup    DATAIO
* 
* @param[in]  length : Length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION::SetLength(XWORD length)
{
  this->length = length; 
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!buffer.Add((XWORD)type))   return false;
  if(!buffer.Add((XWORD)length)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(buffer.GetSize() < (sizeof(XWORD) + sizeof(XWORD)))
    {
      return false;
    }

  if(!buffer.Extract(type))   return false;
  if(!buffer.Extract(length)) return false;

  if(buffer.GetSize() != length)
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION::Clean()
{
  type    = 0;
  length  = 0;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::~DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::~DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetType()
* @brief      name  get type
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetType()
{
  return name_type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_SetType(XWORD name_type)
* @brief      name  set type
* @ingroup    DATAIO
* 
* @param[in]  name_type : Name type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_SetType(XWORD name_type)
{
  this->name_type = name_type;
}

            
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetLength()
* @brief      name  get length
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetLength()
{
  return name_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_SetLength(XWORD name_length)
* @brief      name  set length
* @ingroup    DATAIO
* 
* @param[in]  name_length : Name length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_SetLength(XWORD name_length)
{
  this->name_length = name_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetHost()
* @brief      name  get host
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Name_GetHost()
{
  return &hostname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  servername : Servername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
{
  if(!servername)
    {
      return false;
    }

  servername->Name_SetType(Name_GetType());
  servername->Name_SetLength(Name_GetLength());
  servername->Name_GetHost()->Set(Name_GetHost()->Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  servername : Servername pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
{
  if(!servername)
    {
      return false;
    }

  Name_SetType(servername->Name_GetType());
  Name_SetLength(servername->Name_GetLength());
  Name_GetHost()->Set(servername->Name_GetHost()->Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::SetToBuffer(XBUFFER& buffer, bool showdebug)
{ 
  XBUFFER bufferstring;
  
  if(!hostname.ConvertToASCII(bufferstring))
    {
      return false;
    }

  if(bufferstring.GetSize())
    {
      bufferstring.Resize(bufferstring.GetSize()-1);
    }

  if(bufferstring.IsEmpty() || (bufferstring.GetSize() > 0xFFFF))
    {
      return false;
    }

  name_length = bufferstring.GetSize();

  if(!buffer.Add((XBYTE)name_type))     return false;
  if(!buffer.Add((XWORD)name_length))  return false;

  return buffer.Add(bufferstring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(buffer.GetSize() < (sizeof(XBYTE) + sizeof(XWORD)))
    {
      return false;
    }

  if(!buffer.Extract(name_type))    return false;
  if(!buffer.Extract(name_length))  return false;

  if(!name_length || (buffer.GetSize() != name_length))
    {
      return false;
    }

  XBUFFER hostnamebuffer;

  hostnamebuffer.Resize(name_length);

  if(buffer.Extract(hostnamebuffer.Get(), 0, name_length) != name_length)
    {
      return false;
    }

  hostname.Empty();

  if(!hostname.ConvertFromASCII(hostnamebuffer))
    {
      return false;
    }

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME::Clean()
{
  name_type   = 0;
  name_length = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SNI::DIOSTREAMTLS_MSG_EXTENSION_SNI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SNI::DIOSTREAMTLS_MSG_EXTENSION_SNI()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SNI);   
}

                        
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SNI::~DIOSTREAMTLS_MSG_EXTENSION_SNI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SNI::~DIOSTREAMTLS_MSG_EXTENSION_SNI()
{
  List_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI::List_GetLength()
* @brief      list  get length
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_SNI::List_GetLength()
{
  return list_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SNI::List_SetLength(XWORD list_length)
* @brief      list  set length
* @ingroup    DATAIO
* 
* @param[in]  list_length : List length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SNI::List_SetLength(XWORD list_length)
{
  this->list_length = list_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME*>* DIOSTREAMTLS_MSG_EXTENSION_SNI::List_Get()
* @brief      list  get
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME*>* DIOSTREAMTLS_MSG_EXTENSION_SNI::List_Get()
{
  return &list;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::List_Add(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
* @brief      list  add
* @ingroup    DATAIO
* 
* @param[in]  servername : Servername pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::List_Add(DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername)
{
  if(!servername)
    {
      return false;
    }

  XDWORD entrysize = servername->GetLengthBuffer();

  if(!entrysize || ((XDWORD)List_GetLength() + entrysize > (0xFFFF - sizeof(XWORD))))
    {
      return false;
    }

  DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* _servername =  GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME();
  if(!_servername)
    {
      return false;
    }
        
  if(!_servername->CopyFrom(servername))
    {
      GEN_DELETE _servername;
      return false;
    }

  if(!list.Add(_servername))
    {
      GEN_DELETE _servername;
      return false;
    }

  List_SetLength(0);

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername = list.Get(c);
      if(servername)
        {
          List_SetLength((XWORD)(List_GetLength() + servername->GetLengthBuffer()));
        }
    }

  SetLength(List_GetLength() + sizeof(XWORD));

  return true;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::List_DeleteAll()
* @brief      list  GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::List_DeleteAll()
{
  list.DeleteContents();
  list.DeleteAll();

  List_SetLength(0);
  SetLength(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_SNI* extension)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_SNI* extension)
{
  if(!extension)
    {
      return false;
    }

  extension->List_DeleteAll();
  extension->SetType(GetType());  

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername = list.Get(c);
      if(servername)
        {
          if(!extension->List_Add(servername))
            { 
              extension->List_DeleteAll();
              return false;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_SNI* extension)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_SNI* extension)
{
  if(!extension)
    {
      return false;
    }

  List_DeleteAll();
  SetType(extension->GetType());  

  for(XDWORD c=0; c<extension->List_Get()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername = extension->List_Get()->Get(c);
      if(servername)
        {
          if(!List_Add(servername))
            { 
              List_DeleteAll();
              return false;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER listbuffer;

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername = list.Get(c);
      if(!servername || !servername->SetToBuffer(listbuffer, showdebug))
        {
          return false;
        }
    }

  if(listbuffer.IsEmpty() || (listbuffer.GetSize() > (0xFFFF - sizeof(XWORD))))
    {
      return false;
    }

  List_SetLength((XWORD)listbuffer.GetSize());
  SetLength((XWORD)(sizeof(XWORD) + listbuffer.GetSize()));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug))
    {
      return false;
    }

  if(!buffer.Add((XWORD)list_length))
    {
      return false;
    }

  return buffer.Add(listbuffer);
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SNI::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SNI::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
    {
      return false;
    }

  if((GetLength() < sizeof(XWORD)) || (buffer.GetSize() != GetLength()))
    {
      return false;
    }

  if(!buffer.Extract(list_length))
    {
      return false;
    }

  if(!list_length || (buffer.GetSize() != list_length))
    {
      return false;
    }

  List_DeleteAll();

  while(!buffer.IsEmpty())
    {
      if(buffer.GetSize() < (sizeof(XBYTE) + sizeof(XWORD)))
        {
          List_DeleteAll();
          return false;
        }

      XWORD entrylength = (XWORD)((buffer.GetByte(1) << 8) | buffer.GetByte(2));
      XDWORD entrysize  = sizeof(XBYTE) + sizeof(XWORD) + entrylength;

      if(!entrylength || (buffer.GetSize() < entrysize))
        {
          List_DeleteAll();
          return false;
        }

      XBUFFER entrybuffer;
      entrybuffer.Resize(entrysize);

      if(buffer.Extract(entrybuffer.Get(), 0, entrysize) != entrysize)
        {
          List_DeleteAll();
          return false;
        }

      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME servername;

      if(!servername.GetFromBuffer(entrybuffer, showdebug) || !entrybuffer.IsEmpty() || !List_Add(&servername))
        {
          List_DeleteAll();
          return false;
        }
    }

  return !list.IsEmpty();
}

                                               
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SNI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SNI::Clean()
{
  list_length = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS()
{
 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS::DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS::DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_ECPOINTFORMATS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS::~DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS::~DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS()
{
 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS::DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS::DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS::~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS::~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS()
{
 
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT::DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT::DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMSCERT);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT::~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT::~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_ALPN::DIOSTREAMTLS_MSG_EXTENSION_ALPN()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_ALPN::DIOSTREAMTLS_MSG_EXTENSION_ALPN()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_ALPN::~DIOSTREAMTLS_MSG_EXTENSION_ALPN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_ALPN::~DIOSTREAMTLS_MSG_EXTENSION_ALPN()
{
  List_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetLength()
* @brief      list  get length
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetLength()
{
  return list_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_SetLength(XWORD list_length)
* @brief      list  set length
* @ingroup    DATAIO
* 
* @param[in]  list_length : List length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_SetLength(XWORD list_length)
{
  this->list_length = list_length;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetBuffer()
* @brief      list  get buffer
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetBuffer()
{
  return &list_buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetNProtocols()
* @brief      Get the number of encoded ALPN protocol names
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_GetNProtocols()
{
  XDWORD index = 0;
  XDWORD count = 0;

  while(index < list_buffer.GetSize())
    {
      XBYTE protocolsize = list_buffer.GetByte(index);

      if(!protocolsize || ((index + sizeof(XBYTE) + protocolsize) > list_buffer.GetSize())) return 0;

      index += sizeof(XBYTE) + protocolsize;
      count++;
    }

  return count;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Get(XDWORD index, DIOSTREAMTLS_ALPN_TYPE& alpn_type)
* @brief      Get one supported ALPN protocol name
* @ingroup    DATAIO
*
* @param[in]  index : Protocol index.
* @param[out] alpn_type : Decoded application protocol value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Get(XDWORD index, DIOSTREAMTLS_ALPN_TYPE& alpn_type)
{
  static XBYTE HTTP11[] = { 0x68, 0x74, 0x74, 0x70, 0x2F, 0x31, 0x2E, 0x31 };
  static XBYTE HTTP2[]  = { 0x68, 0x32 };
  static XBYTE HTTP3[]  = { 0x68, 0x33 };

  XDWORD position = 0;
  XDWORD count    = 0;

  alpn_type = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;

  while(position < list_buffer.GetSize())
    {
      XBYTE protocolsize = list_buffer.GetByte(position);

      if(!protocolsize || ((position + sizeof(XBYTE) + protocolsize) > list_buffer.GetSize())) return false;

      if(count == index)
        {
          XBYTE* protocol = &list_buffer.Get()[position + sizeof(XBYTE)];

          if((protocolsize == sizeof(HTTP11)) && !memcmp(protocol, HTTP11, sizeof(HTTP11)))
            {
              alpn_type = DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1;
              return true;
            }

          if((protocolsize == sizeof(HTTP2)) && !memcmp(protocol, HTTP2, sizeof(HTTP2)))
            {
              alpn_type = DIOSTREAMTLS_ALPN_TYPE_HTTP_2;
              return true;
            }

          if((protocolsize == sizeof(HTTP3)) && !memcmp(protocol, HTTP3, sizeof(HTTP3)))
            {
              alpn_type = DIOSTREAMTLS_ALPN_TYPE_HTTP_3;
              return true;
            }

          // ALPN protocol names are opaque byte strings.  Unknown names are valid and must not
          // make the extension decoder fail; callers that only understand the legacy enum API
          // receive UNKNOWN and can still continue parsing the remaining offered protocols.
          return true;
        }

      position += sizeof(XBYTE) + protocolsize;
      count++;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Get(XDWORD index, XBUFFER& protocol)
* @brief      List get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* @param[in]  protocol : Protocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Get(XDWORD index, XBUFFER& protocol)
{
  XDWORD position = 0;
  XDWORD count = 0;

  protocol.Delete();
  while(position < list_buffer.GetSize())
    {
      XBYTE protocolsize = list_buffer.GetByte(position);
      if(!protocolsize || ((position + 1 + protocolsize) > list_buffer.GetSize())) return false;
      if(count == index) return protocol.Add(&list_buffer.Get()[position + 1], protocolsize);
      position += 1 + protocolsize;
      count++;
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Is(XBUFFER& protocol)
* @brief      List is
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Is(XBUFFER& protocol)
{
  for(XDWORD c=0; c<List_GetNProtocols(); c++)
    {
      XBUFFER current;
      if(List_Get(c, current) && current.Compare(protocol)) return true;
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Is(DIOSTREAMTLS_ALPN_TYPE alpn_type)
* @brief      Check whether an ALPN protocol name is present
* @ingroup    DATAIO
*
* @param[in]  alpn_type : Application protocol value.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Is(DIOSTREAMTLS_ALPN_TYPE alpn_type)
{
  for(XDWORD c=0; c<List_GetNProtocols(); c++)
    {
      DIOSTREAMTLS_ALPN_TYPE protocol;

      if(List_Get(c, protocol) && (protocol == alpn_type)) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Add(DIOSTREAMTLS_ALPN_TYPE alpn_type)
* @brief      list  add
* @ingroup    DATAIO
* 
* @param[in]  alpn_type : Alpn type value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Add(DIOSTREAMTLS_ALPN_TYPE alpn_type)
{
  XBYTE* data = NULL;
  XDWORD size = 0;

  if(List_Is(alpn_type)) return false;

  switch(alpn_type)
    {  
      case DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1 :  { static XBYTE value[] = { 0x08, 0x68, 0x74, 0x74, 0x70, 0x2F, 0x31, 0x2E, 0x31 };
                                                data = value;
                                                size = sizeof(value);
                                              }
                                              break;

      case DIOSTREAMTLS_ALPN_TYPE_HTTP_2   :  { static XBYTE value[] = { 0x02, 0x68, 0x32 };
                                                data = value;
                                                size = sizeof(value);
                                              }
                                              break;

      case DIOSTREAMTLS_ALPN_TYPE_HTTP_3   :  { static XBYTE value[] = { 0x02, 0x68, 0x33 };
                                                data = value;
                                                size = sizeof(value);
                                              }
                                              break;

                                     default : return false;
    }

  if(!data || !size || ((list_buffer.GetSize() + size) > (0xFFFF - sizeof(XWORD))))
    {
      return false;
    }

  if(!list_buffer.Add(data, size))
    {
      return false;
    }

  list_length = list_buffer.GetSize();

  SetLength(list_length  + sizeof(XWORD));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Add(XBUFFER& protocol)
* @brief      List add
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_Add(XBUFFER& protocol)
{
  if(protocol.IsEmpty() || protocol.GetSize() > 255 || List_Is(protocol) ||
     ((list_buffer.GetSize() + 1 + protocol.GetSize()) > (0xFFFF - sizeof(XWORD)))) return false;

  if(!list_buffer.Add((XBYTE)protocol.GetSize()) || !list_buffer.Add(protocol)) return false;
  list_length = list_buffer.GetSize();
  SetLength(list_length + sizeof(XWORD));
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_DeleteAll()
* @brief      list  GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::List_DeleteAll()
{
  list_buffer.Delete();

  list_length = 0;
  SetLength(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension)
{
  if(!extension)
    {
      return false;
    }

  extension->SetType(GetType());  
  extension->SetLength(GetLength());

  extension->List_SetLength(List_GetLength());

  if(!extension->List_GetBuffer()->CopyFrom(list_buffer))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension)
{
  if(!extension)
    {
      return false;
    }

  SetType(extension->GetType());  
  SetLength(extension->GetLength());

  List_SetLength(extension->List_GetLength());
  
  if(!list_buffer.CopyFrom((*extension->List_GetBuffer())))
    {
      return false;
    }

  return true;
}    


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(list_buffer.IsEmpty() || (list_buffer.GetSize() > (0xFFFF - sizeof(XWORD))))
    {
      return false;
    }

  list_length = (XWORD)list_buffer.GetSize();
  SetLength((XWORD)(sizeof(XWORD) + list_length));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug))
    {
      return false;
    }

  if(!buffer.Add((XWORD)list_length))
    {
      return false;
    }

  return buffer.Add(list_buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_ALPN::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
    {
      return false;
    }

  if((GetLength() < sizeof(XWORD)) || (buffer.GetSize() != GetLength()))
    {
      return false;
    }

  if(!buffer.Extract(list_length))
    {
      return false;
    }

  if(!list_length || (buffer.GetSize() != list_length))
    {
      return false;
    }

  XDWORD index = 0;

  while(index < list_length)
    {
      XBYTE protocolsize = buffer.GetByte(index);

      if(!protocolsize || ((index + sizeof(XBYTE) + protocolsize) > list_length))
        {
          return false;
        }

      index += sizeof(XBYTE) + protocolsize;
    }

  list_buffer.Delete();
  list_buffer.Resize(list_length);

  if(buffer.Extract(list_buffer.Get(), 0, list_length) != list_length)
    {
      list_buffer.Delete();
      return false;
    }

  return buffer.IsEmpty();
}
     

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_ALPN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_ALPN::Clean()
{
  list_length = 0;

  list_buffer.Empty();   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_EMS::DIOSTREAMTLS_MSG_EXTENSION_EMS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_EMS::DIOSTREAMTLS_MSG_EXTENSION_EMS()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_EMS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_EMS::~DIOSTREAMTLS_MSG_EXTENSION_EMS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_EMS::~DIOSTREAMTLS_MSG_EXTENSION_EMS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_EMS::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_EMS* extension)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_EMS::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_EMS* extension)
{
  if(!extension)
    {
      return false;
    }

  extension->SetType(GetType());  
  extension->SetLength(GetLength());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_EMS::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_EMS* extension)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_EMS::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_EMS* extension)
{
  if(!extension)
    {
      return false;
    }

  SetType(extension->GetType());  
  SetLength(extension->GetLength());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_EMS::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_EMS::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  SetLength(0);

  return DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_EMS::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_EMS::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
    {
      return false;
    }

  return (!GetLength() && buffer.IsEmpty());
}
     

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_EMS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_EMS::Clean()
{
  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS()
{
 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES::DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES::DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES()
{
  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_PSKKEYEXCHANGEMODES); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES::~DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES::~DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES()
{
 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEY::DIOSTREAMTLS_MSG_EXTENSION_KEY()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEY::DIOSTREAMTLS_MSG_EXTENSION_KEY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEY::~DIOSTREAMTLS_MSG_EXTENSION_KEY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEY::~DIOSTREAMTLS_MSG_EXTENSION_KEY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_KEY::GetKeyType()
* @brief      get key type
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_KEY::GetKeyType()
{
  return keytype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEY::SetKeyType(XWORD keytype)
* @brief      set key type
* @ingroup    DATAIO
* 
* @param[in]  keytype : Keytype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEY::SetKeyType(XWORD keytype)
{
  this->keytype = keytype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_KEY::GetLengthKeyData()
* @brief      get length key data
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_KEY::GetLengthKeyData()
{
  return lengthkeydata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEY::SetLengthKeyData(XWORD lengthkeydata)
* @brief      set length key data
* @ingroup    DATAIO
* 
* @param[in]  lengthkeydata : Lengthkeydata value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEY::SetLengthKeyData(XWORD lengthkeydata)
{
  this->lengthkeydata = lengthkeydata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_KEY::GetKeyData()
* @brief      get key data
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_KEY::GetKeyData()
{
  return &keydata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  key : Key pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
{ 
  if(!key)
    {
      return false;
    }

  key->SetKeyType(keytype);
  
  key->SetLengthKeyData(lengthkeydata);

  key->GetKeyData()->Empty();
  if(!key->GetKeyData()->Add(keydata))
    {
      return false;
    }

  return true;
}
  
  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  key : Key pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
{ 
  if(!key)
    {
      return false;
    }

  SetKeyType(key->GetKeyType());
  
  SetLengthKeyData(key->GetLengthKeyData());
  
  GetKeyData()->Empty();
  if(!GetKeyData()->Add(key->GetKeyData()))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEY::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEY::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(keydata.IsEmpty() || (keydata.GetSize() > 0xFFFF))
    {
      return false;
    }

  lengthkeydata = (XWORD)keydata.GetSize();

  if(!buffer.Add(keytype))       return false;
  if(!buffer.Add(lengthkeydata)) return false;
  if(!buffer.Add(keydata))       return false;

  return true;
}                                           


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(buffer.GetSize() < (sizeof(XWORD) + sizeof(XWORD)))
    {
      return false;
    }

  if(!buffer.Extract(keytype))       return false;
  if(!buffer.Extract(lengthkeydata)) return false;

  if(!lengthkeydata || (buffer.GetSize() != lengthkeydata))
    {
      return false;
    }

  keydata.Delete();
  if(!keydata.Resize(lengthkeydata))
    {
      return false;
    }

  if(buffer.Extract(keydata.Get(), 0, lengthkeydata) != lengthkeydata)
    {
      keydata.Delete();
      return false;
    }

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEY::Clean()
{
  keytype = 0;
  lengthkeydata  = 0;
  keydata.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE()
{
  List_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_GetLength()
* @brief      list  get length
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_GetLength()
{
  return list_length;
}                                          


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_SetLength(XWORD list_length)
* @brief      list  set length
* @ingroup    DATAIO
* 
* @param[in]  list_length : List length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_SetLength(XWORD list_length)
{
  this->list_length = list_length;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_KEY*>* DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_Get()
* @brief      list  get
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_KEY*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_KEY*>* DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_Get()
{
  return &list;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_Add(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
* @brief      list  add
* @ingroup    DATAIO
* 
* @param[in]  key : Key pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_Add(DIOSTREAMTLS_MSG_EXTENSION_KEY* key)
{
  if(!key || key->GetKeyData()->IsEmpty() || (key->GetKeyData()->GetSize() > 0xFFFF))
    {
      return false;
    }

  XDWORD entrysize = sizeof(XWORD) + sizeof(XWORD) + key->GetKeyData()->GetSize();

  if(((XDWORD)List_GetLength() + entrysize) > (0xFFFF - sizeof(XWORD)))
    {
      return false;
    }

  DIOSTREAMTLS_MSG_EXTENSION_KEY* _key =  GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEY();
  if(!_key)
    {
      return false;
    }
        
  if(!_key->CopyFrom(key))
    {
      GEN_DELETE _key;
      return false;
    }

  if(!list.Add(_key))
    {
      GEN_DELETE _key;
      return false;
    }

  List_SetLength(0);

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY* key = list.Get(c);
      if(key)
        {
          List_SetLength((XWORD)(List_GetLength() + sizeof(XWORD) + sizeof(XWORD) + key->GetKeyData()->GetSize()));
        }
    }

  SetLength(List_GetLength() + sizeof(XWORD));

  return true;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_DeleteAll()
* @brief      list  GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::List_DeleteAll()
{
  list.DeleteContents();
  list.DeleteAll();

  List_SetLength(0);
  SetLength(0);

  return true;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension)
* @brief      copy to
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension)
{
  if(!extension)
    {
      return false;
    }

  extension->List_DeleteAll();
  extension->SetType(GetType());  

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY* key = list.Get(c);
      if(key)
        {
          if(!extension->List_Add(key))
            { 
              extension->List_DeleteAll();
              return false;
            }
        }
    }

  return true;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension)
* @brief      copy from
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension)
{
  if(!extension)
    {
      return false;
    }

  List_DeleteAll();
  SetType(extension->GetType());  

  for(XDWORD c=0; c<extension->List_Get()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY* key = extension->List_Get()->Get(c);
      if(key)
        {
          if(!List_Add(key))
            { 
              List_DeleteAll();
              return false;
            }
        }
    }
  
  return true;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      set to buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER listbuffer;

  for(XDWORD c=0; c<list.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY* key = list.Get(c);
      if(!key || !key->SetToBuffer(listbuffer, showdebug))
        {
          return false;
        }
    }

  if(listbuffer.IsEmpty() || (listbuffer.GetSize() > (0xFFFF - sizeof(XWORD))))
    {
      return false;
    }

  List_SetLength((XWORD)listbuffer.GetSize());
  SetLength((XWORD)(sizeof(XWORD) + listbuffer.GetSize()));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug))
    {
      return false;
    }

  if(!buffer.Add((XWORD)list_length))
    {
      return false;
    }

  return buffer.Add(listbuffer);
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      get from buffer
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
    {
      return false;
    }

  if((GetLength() < sizeof(XWORD)) || (buffer.GetSize() != GetLength()))
    {
      return false;
    }

  if(!buffer.Extract(list_length))
    {
      return false;
    }

  // RFC 8446 permits an empty client_shares vector in ClientHello. This is the valid input that lets a
  // server select a mutually supported group through HelloRetryRequest.
  if(buffer.GetSize() != list_length)
    {
      return false;
    }

  List_DeleteAll();

  while(!buffer.IsEmpty())
    {
      if(buffer.GetSize() < (sizeof(XWORD) + sizeof(XWORD)))
        {
          List_DeleteAll();
          return false;
        }

      XWORD keylength = (XWORD)((buffer.GetByte(2) << 8) | buffer.GetByte(3));
      XDWORD keysize  = sizeof(XWORD) + sizeof(XWORD) + keylength;

      if(!keylength || (buffer.GetSize() < keysize))
        {
          List_DeleteAll();
          return false;
        }

      XBUFFER keybuffer;
      keybuffer.Resize(keysize);

      if(buffer.Extract(keybuffer.Get(), 0, keysize) != keysize)
        {
          List_DeleteAll();
          return false;
        }

      DIOSTREAMTLS_MSG_EXTENSION_KEY key;

      if(!key.GetFromBuffer(keybuffer, showdebug) || !keybuffer.IsEmpty() || !List_Add(&key))
        {
          List_DeleteAll();
          return false;
        }
    }

  return true;
}
                                             
      
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE::Clean()
{
  list_length = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::GetVersion()
* @brief      Get the selected protocol version
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::GetVersion()
{
  return version;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::SetVersion(XWORD version)
* @brief      Set the selected protocol version
* @ingroup    DATAIO
*
* @param[in]  version : Protocol version.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::SetVersion(XWORD version)
{
  this->version = version;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  SetLength(sizeof(XWORD));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;

  return buffer.Add(version);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug)) return false;
  if((GetLength() != sizeof(XWORD)) || (buffer.GetSize() != sizeof(XWORD))) return false;
  if(!buffer.Extract(version)) return false;

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER::Clean()
{
  version = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEY* DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::GetKey()
* @brief      Get the server key share
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_MSG_EXTENSION_KEY* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEY* DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::GetKey()
{
  return &key;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER keybuffer;

  if(!key.SetToBuffer(keybuffer, showdebug)) return false;
  if(keybuffer.GetSize() > 0xFFFF)            return false;

  SetLength((XWORD)keybuffer.GetSize());

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;

  return buffer.Add(keybuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug)) return false;
  if(!GetLength() || (buffer.GetSize() != GetLength()))             return false;

  return key.GetFromBuffer(buffer, showdebug) && buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::GetSelectedGroup()
* @brief      Get the selected group
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::GetSelectedGroup()
{
  return selectedgroup;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::SetSelectedGroup(XWORD selectedgroup)
* @brief      Set the selected group
* @ingroup    DATAIO
*
* @param[in]  selectedgroup : Selected group.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::SetSelectedGroup(XWORD selectedgroup)
{
  this->selectedgroup = selectedgroup;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  SetLength(sizeof(XWORD));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;

  return buffer.Add(selectedgroup);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug)) return false;
  if((GetLength() != sizeof(XWORD)) || (buffer.GetSize() != sizeof(XWORD))) return false;
  if(!buffer.Extract(selectedgroup)) return false;

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST::Clean()
{
  selectedgroup = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::~DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::~DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetLengthIdentity()
* @brief      Get the identity opaque length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetLengthIdentity()
{
  return lengthidentity;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetLengthIdentity(XWORD lengthidentity)
* @brief      Set the identity opaque length
* @ingroup    DATAIO
*
* @param[in]  lengthidentity : Lengthidentity value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetLengthIdentity(XWORD lengthidentity)
{
  this->lengthidentity = lengthidentity;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetIdentity()
* @brief      Get the identity opaque
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetIdentity()
{
  return &identity;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetObfuscatedTicketAge()
* @brief      Get the obfuscated ticket age
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetObfuscatedTicketAge()
{
  return obfuscatedticketage;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetObfuscatedTicketAge(XDWORD obfuscatedticketage)
* @brief      Set the obfuscated ticket age
* @ingroup    DATAIO
*
* @param[in]  obfuscatedticketage : Obfuscatedticketage value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetObfuscatedTicketAge(XDWORD obfuscatedticketage)
{
  this->obfuscatedticketage = obfuscatedticketage;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
* @brief      copy to
* @ingroup    DATAIO
*
* @param[in]  identity : Identity pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
{
  if(!identity)
    {
      return false;
    }

  identity->SetLengthIdentity(lengthidentity);
  identity->GetIdentity()->Empty();
  identity->GetIdentity()->Add(this->identity);
  identity->SetObfuscatedTicketAge(obfuscatedticketage);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
* @brief      copy from
* @ingroup    DATAIO
*
* @param[in]  identity : Identity pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
{
  if(!identity)
    {
      return false;
    }

  SetLengthIdentity(identity->GetLengthIdentity());
  this->identity.Empty();
  this->identity.Add((*identity->GetIdentity()));
  SetObfuscatedTicketAge(identity->GetObfuscatedTicketAge());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(identity.IsEmpty() || (identity.GetSize() > 0xFFFF))
    {
      return false;
    }

  lengthidentity = (XWORD)identity.GetSize();

  if(!buffer.Add(lengthidentity))       return false;
  if(!buffer.Add(identity))             return false;
  if(!buffer.Add(obfuscatedticketage))  return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(buffer.GetSize() < (sizeof(XWORD) + sizeof(XDWORD)))
    {
      return false;
    }

  if(!buffer.Extract(lengthidentity)) return false;

  if(!lengthidentity || (buffer.GetSize() < ((XDWORD)lengthidentity + sizeof(XDWORD))))
    {
      return false;
    }

  identity.Delete();
  identity.Resize(lengthidentity);

  if(buffer.Extract(identity.Get(), 0, lengthidentity) != lengthidentity)
    {
      identity.Delete();
      return false;
    }

  return buffer.Extract(obfuscatedticketage);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY::Clean()
{
  lengthidentity      = 0;
  obfuscatedticketage = 0;

  identity.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY()
{
  Identities_DeleteAll();
  Binders_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_GetLength()
* @brief      identities  get length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_GetLength()
{
  return identities_length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_SetLength(XWORD identities_length)
* @brief      identities  set length
* @ingroup    DATAIO
*
* @param[in]  identities_length : Identities length value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_SetLength(XWORD identities_length)
{
  this->identities_length = identities_length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY*>* DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_GetAll()
* @brief      identities  get all
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY*>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY*>* DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_GetAll()
{
  return &identities;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_Add(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
* @brief      identities  add
* @ingroup    DATAIO
*
* @param[in]  identity : Identity pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_Add(DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity)
{
  if(!identity || identity->GetIdentity()->IsEmpty() || (identity->GetIdentity()->GetSize() > 0xFFFF))
    {
      return false;
    }

  XDWORD entrysize = sizeof(XWORD) + identity->GetIdentity()->GetSize() + sizeof(XDWORD);

  if(((XDWORD)Identities_GetLength() + entrysize) > 0xFFFF)
    {
      return false;
    }

  DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* _identity = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY();
  if(!_identity)
    {
      return false;
    }

  if(!_identity->CopyFrom(identity))
    {
      GEN_DELETE _identity;
      return false;
    }

  if(!identities.Add(_identity))
    {
      GEN_DELETE _identity;
      return false;
    }

  Identities_SetLength((XWORD)(Identities_GetLength() + entrysize));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_DeleteAll()
* @brief      identities  GEN_DELETE all
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Identities_DeleteAll()
{
  identities.DeleteContents();
  identities.DeleteAll();

  Identities_SetLength(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_GetLength()
* @brief      binders  get length
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_GetLength()
{
  return binders_length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_SetLength(XWORD binders_length)
* @brief      binders  set length
* @ingroup    DATAIO
*
* @param[in]  binders_length : Binders length value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_SetLength(XWORD binders_length)
{
  this->binders_length = binders_length;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_GetAll()
* @brief      binders  get all
* @ingroup    DATAIO
*
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_GetAll()
{
  return &binders;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_Add(XBUFFER* binder)
* @brief      binders  add
* @ingroup    DATAIO
*
* @param[in]  binder : Binder pointer to use. A PskBinderEntry, 32..255 opaque bytes (RFC 8446 4.2.11).
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_Add(XBUFFER* binder)
{
  if(!binder || (binder->GetSize() < 32) || (binder->GetSize() > 255))
    {
      return false;
    }

  XDWORD entrysize = sizeof(XBYTE) + binder->GetSize();

  if(((XDWORD)Binders_GetLength() + entrysize) > 0xFFFF)
    {
      return false;
    }

  XBUFFER* _binder = GEN_NEW XBUFFER();
  if(!_binder)
    {
      return false;
    }

  _binder->Add((*binder));

  if(!binders.Add(_binder))
    {
      GEN_DELETE _binder;
      return false;
    }

  Binders_SetLength((XWORD)(Binders_GetLength() + entrysize));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_DeleteAll()
* @brief      binders  GEN_DELETE all
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Binders_DeleteAll()
{
  binders.DeleteContents();
  binders.DeleteAll();

  Binders_SetLength(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension)
* @brief      copy to
* @ingroup    DATAIO
*
* @param[in]  extension : Extension pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::CopyTo(DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension)
{
  if(!extension)
    {
      return false;
    }

  extension->SetType(GetType());
  extension->SetLength(GetLength());

  extension->Identities_DeleteAll();
  extension->Binders_DeleteAll();

  for(XDWORD c=0; c<identities.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity = identities.Get(c);
      if(identity)
        {
          extension->Identities_Add(identity);
        }
    }

  for(XDWORD c=0; c<binders.GetSize(); c++)
    {
      XBUFFER* binder = binders.Get(c);
      if(binder)
        {
          extension->Binders_Add(binder);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension)
* @brief      copy from
* @ingroup    DATAIO
*
* @param[in]  extension : Extension pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::CopyFrom(DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension)
{
  if(!extension)
    {
      return false;
    }

  return extension->CopyTo(this);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  XBUFFER identitiesbuffer;
  XBUFFER bindersbuffer;

  if(identities.IsEmpty() || binders.IsEmpty())
    {
      return false;
    }

  for(XDWORD c=0; c<identities.GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity = identities.Get(c);
      if(!identity || !identity->SetToBuffer(identitiesbuffer, showdebug))
        {
          return false;
        }
    }

  for(XDWORD c=0; c<binders.GetSize(); c++)
    {
      XBUFFER* binder = binders.Get(c);
      if(!binder || (binder->GetSize() < 32) || (binder->GetSize() > 255))
        {
          return false;
        }

      if(!bindersbuffer.Add((XBYTE)binder->GetSize()) || !bindersbuffer.Add((*binder)))
        {
          return false;
        }
    }

  if(identitiesbuffer.IsEmpty() || (identitiesbuffer.GetSize() > 0xFFFF) ||
     bindersbuffer.IsEmpty()    || (bindersbuffer.GetSize()    > 0xFFFF))
    {
      return false;
    }

  Identities_SetLength((XWORD)identitiesbuffer.GetSize());
  Binders_SetLength((XWORD)bindersbuffer.GetSize());

  XDWORD totalsize = sizeof(XWORD) + identitiesbuffer.GetSize() + sizeof(XWORD) + bindersbuffer.GetSize();

  if(totalsize > 0xFFFF)
    {
      return false;
    }

  SetLength((XWORD)totalsize);

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;

  if(!buffer.Add((XWORD)Identities_GetLength())) return false;
  if(!buffer.Add(identitiesbuffer))               return false;
  if(!buffer.Add((XWORD)Binders_GetLength()))     return false;

  return buffer.Add(bindersbuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
    {
      return false;
    }

  if((GetLength() < (sizeof(XWORD) + sizeof(XWORD))) || (buffer.GetSize() != GetLength()))
    {
      return false;
    }

  Identities_DeleteAll();
  Binders_DeleteAll();

  if(!buffer.Extract(identities_length) || !identities_length || (buffer.GetSize() < identities_length))
    {
      return false;
    }

  XBUFFER identitiesbuffer;
  identitiesbuffer.Resize(identities_length);

  if(buffer.Extract(identitiesbuffer.Get(), 0, identities_length) != identities_length)
    {
      return false;
    }

  while(!identitiesbuffer.IsEmpty())
    {
      if(identitiesbuffer.GetSize() < sizeof(XWORD))
        {
          Identities_DeleteAll();
          return false;
        }

      XWORD entrylength = (XWORD)((identitiesbuffer.GetByte(0) << 8) | identitiesbuffer.GetByte(1));
      XDWORD entrysize  = sizeof(XWORD) + entrylength + sizeof(XDWORD);

      if(!entrylength || (identitiesbuffer.GetSize() < entrysize))
        {
          Identities_DeleteAll();
          return false;
        }

      XBUFFER entrybuffer;
      entrybuffer.Resize(entrysize);

      if(identitiesbuffer.Extract(entrybuffer.Get(), 0, entrysize) != entrysize)
        {
          Identities_DeleteAll();
          return false;
        }

      DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY identity;

      if(!identity.GetFromBuffer(entrybuffer, showdebug) || !entrybuffer.IsEmpty() || !Identities_Add(&identity))
        {
          Identities_DeleteAll();
          return false;
        }
    }

  if(identities.IsEmpty() || (buffer.GetSize() < sizeof(XWORD)))
    {
      Identities_DeleteAll();
      return false;
    }

  if(!buffer.Extract(binders_length) || !binders_length || (buffer.GetSize() != binders_length))
    {
      Identities_DeleteAll();
      return false;
    }

  while(!buffer.IsEmpty())
    {
      if(buffer.GetSize() < sizeof(XBYTE))
        {
          Identities_DeleteAll();
          Binders_DeleteAll();
          return false;
        }

      XBYTE binderlength = buffer.GetByte(0);

      if((binderlength < 32) || (buffer.GetSize() < ((XDWORD)sizeof(XBYTE) + binderlength)))
        {
          Identities_DeleteAll();
          Binders_DeleteAll();
          return false;
        }

      XBYTE dummy;

      if(!buffer.Extract(dummy))
        {
          Identities_DeleteAll();
          Binders_DeleteAll();
          return false;
        }

      XBUFFER binder;
      binder.Resize(binderlength);

      if(buffer.Extract(binder.Get(), 0, binderlength) != binderlength)
        {
          Identities_DeleteAll();
          Binders_DeleteAll();
          return false;
        }

      if(!Binders_Add(&binder))
        {
          Identities_DeleteAll();
          Binders_DeleteAll();
          return false;
        }
    }

  return !identities.IsEmpty() && !binders.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY::Clean()
{
  identities_length = 0;
  binders_length    = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER()
{
  Clean();

  SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::GetSelectedIdentity()
* @brief      Get the selected identity index
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::GetSelectedIdentity()
{
  return selectedidentity;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::SetSelectedIdentity(XWORD selectedidentity)
* @brief      Set the selected identity index
* @ingroup    DATAIO
*
* @param[in]  selectedidentity : Selectedidentity value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::SetSelectedIdentity(XWORD selectedidentity)
{
  this->selectedidentity = selectedidentity;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  SetLength(sizeof(XWORD));

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;

  return buffer.Add(selectedidentity);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug)) return false;
  if((GetLength() != sizeof(XWORD)) || (buffer.GetSize() != sizeof(XWORD))) return false;
  if(!buffer.Extract(selectedidentity)) return false;

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER::Clean()
{
  selectedidentity = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::~DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::~DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::GetData()
* @brief      Get the opaque extension data
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::GetData()
{
  return &data;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::SetToBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Set to buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::SetToBuffer(XBUFFER& buffer, bool showdebug)
{
  if(data.GetSize() > 0xFFFF)
    {
      return false;
    }

  SetLength((XWORD)data.GetSize());

  if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug)) return false;
  if(!data.IsEmpty() && !buffer.Add(data))                        return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::GetFromBuffer(XBUFFER& buffer, bool showdebug)
* @brief      Get from buffer
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer to use.
* @param[in]  showdebug : Showdebug value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::GetFromBuffer(XBUFFER& buffer, bool showdebug)
{
  if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug)) return false;
  if(buffer.GetSize() != GetLength())                               return false;

  data.Delete();

  if(GetLength())
    {
      data.Resize(GetLength());

      if(buffer.Extract(data.Get(), 0, GetLength()) != GetLength())
        {
          data.Delete();
          return false;
        }
    }

  return buffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN::Clean()
{
  data.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_EXTENSION* DIOSTREAMTLS_MSG_EXTENSION_Create(XWORD type, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context)
* @brief      Create an extension codec for a type and its message context
* @ingroup    DATAIO
*
* @param[in]  type : Extension type.
* @param[in]  context : Message context that defines the extension wire format.
*
* @return     DIOSTREAMTLS_MSG_EXTENSION* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_EXTENSION* DIOSTREAMTLS_MSG_EXTENSION_Create(XWORD type, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context)
{
  DIOSTREAMTLS_MSG_EXTENSION* extension = NULL;

  switch(type)
    {
      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SNI                   : if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO)
                                                                    {
                                                                      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SNI();
                                                                    }
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS       : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS();
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_ECPOINTFORMATS        : if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO)
                                                                    {
                                                                      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS();
                                                                    }
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS   : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS();
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMSCERT : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT();
                                                                    break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN                  : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_ALPN();
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_EMS                   : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_EMS();
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS     : if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO)
                                                                    {
                                                                      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS();
                                                                    }
                                                                   else
                                                                    {
                                                                      if((context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_SERVERHELLO) ||
                                                                         (context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_HELLORETRYREQUEST))
                                                                        {
                                                                          extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER();
                                                                        }
                                                                    }
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_PSKKEYEXCHANGEMODES   : if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO)
                                                                    {
                                                                      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES();
                                                                    }
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE              : switch(context)
                                                                    {
                                                                      case DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO       : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE();
                                                                                                                               break;

                                                                      case DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_SERVERHELLO       : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER();
                                                                                                                               break;

                                                                      case DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_HELLORETRYREQUEST : extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST();
                                                                                                                               break;

                                                                                                                        default : break;
                                                                    }
                                                                  break;

      case DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY          : if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO)
                                                                    {
                                                                      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY();
                                                                    }
                                                                   else
                                                                    {
                                                                      if(context == DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_SERVERHELLO)
                                                                        {
                                                                          extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER();
                                                                        }
                                                                    }
                                                                  break;

                                                        default   : break;
    }

  if(!extension)
    {
      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
      if(extension)
        {
          extension->SetType(type);
        }
    }

  return extension;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS_MSG_EXTENSION_Extract(XBUFFER& buffer, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context, DIOSTREAMTLS_MSG_EXTENSION*& extension)
* @brief      Extract one complete extension while retaining incomplete input
* @ingroup    DATAIO
*
* @param[in]  buffer : Buffer containing one or more extensions.
* @param[in]  context : Message context that defines the extension wire format.
* @param[out] extension : Created and decoded extension. Ownership is transferred to the caller.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS_MSG_EXTENSION_Extract(XBUFFER& buffer, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context, DIOSTREAMTLS_MSG_EXTENSION*& extension)
{
  extension = NULL;

  if(buffer.GetSize() < (sizeof(XWORD) + sizeof(XWORD)))
    {
      return false;
    }

  XWORD type   = (XWORD)((buffer.GetByte(0) << 8) | buffer.GetByte(1));
  XWORD length = (XWORD)((buffer.GetByte(2) << 8) | buffer.GetByte(3));
  XDWORD size  = sizeof(XWORD) + sizeof(XWORD) + length;

  if(buffer.GetSize() < size)
    {
      return false;
    }

  XBUFFER extensionbuffer;
  extensionbuffer.Resize(size);

  if(!buffer.Get(extensionbuffer.Get(), size, 0))
    {
      return false;
    }

  extension = DIOSTREAMTLS_MSG_EXTENSION_Create(type, context);
  if(!extension)
    {
      return false;
    }

  if(!extension->GetFromBuffer(extensionbuffer, false) || !extensionbuffer.IsEmpty())
    {
      GEN_DELETE extension;
      extension = NULL;
      return false;
    }

  if(buffer.Extract(NULL, 0, size) != size)
    {
      GEN_DELETE extension;
      extension = NULL;
      return false;
    }

  return true;
}
    
