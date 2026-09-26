/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSMessagesExtension.h
* 
* @class      DIOSTREAMTLSMESSAGE
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOStreamTLSMessages.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SNI                   0x0000 // 1.0+  SNI 
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST         0x0005 // 1.2+  OCSP stapling
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS	      0x000a // 1.2+  Curve
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_ECPOINTFORMATS	      0x000b // 1.2	  ECP Point Formats
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS	  0x000d // 1.2+
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN	                0x0010 // 1.2+  Protocol app
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_EMS                   0x0017 // 1.2	  Extended Mster Secret 
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SESSIONTICKET	        0x0023 // 1.2	  Ticket
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS	    0x002b // 1.3	  TLS Versions
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_COOKIE                 0x002c // 1.3   HelloRetryRequest cookie
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA              0x002a // 1.3   0-RTT early data
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_PSKKEYEXCHANGEMODES	  0x002d // 1.3	  Mode PSK
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_CERTIFICATEAUTHORITIES   0x002f // 1.3   Acceptable CA distinguished names
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMSCERT 0x0032 // 1.2+  X.509 certificate signatures
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE	            0x0033 // 1.3	  Public Key
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY         0x0029 // 1.3	  Session resumption (PSK)
#define DIOSTREAMTLS_MSG_EXTENSION_TYPE_RENEGOTIATIONINFO     0xFF01 // 1.2   Secure renegotiation indication

enum DIOSTREAMTLS_MSG_EXTENSION_CONTEXT
{
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_UNKNOWN                    = 0 ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CLIENTHELLO                    ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_SERVERHELLO                    ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_HELLORETRYREQUEST              ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_ENCRYPTEDEXTENSIONS             ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CERTIFICATEREQUEST              ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_CERTIFICATEENTRY                ,
  DIOSTREAMTLS_MSG_EXTENSION_CONTEXT_NEWSESSIONTICKET                ,
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOSTREAMTLS_MSG_EXTENSION : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION                        ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION                        ();
                                                          
    XWORD                                                 GetType                                           ();                                            
    void                                                  SetType                                           (XWORD type);
                                                          
    XWORD                                                 GetLength                                         ();                                          
    void                                                  SetLength                                         (XWORD length);
    
    virtual bool                                          SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    virtual bool                                          GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);
     
  private:

    void                                                  Clean                                             ();
    
    XWORD                                                 type;
    XWORD                                                 length;    
};   

template<typename T, typename S>
class DIOSTREAMTLS_MSG_EXTENSION_LIST : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_LIST                   ()
                                                          {
                                                            Clean();
                                                          }

    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_LIST                   ()
                                                          {
                                                            Clean();
                                                          }

    S                                                     List_GetLength                                    () 
                                                          {
                                                            return list_length;
                                                          }
                                                         
    void                                                  List_SetLength                                    (S list_length)
                                                          {
                                                            this->list_length = list_length;
                                                          }
    
    XVECTOR<T>*                                           List_Get                                          ()  
                                                          {
                                                            return &list;
                                                          }
                                                       
    bool                                                  List_Add                                          (T element)
                                                          {
                                                            XDWORD size = (list.GetSize() + 1) * sizeof(T);

                                                            if(((XDWORD)(S)size != size) || (size > (0xFFFF - sizeof(S))))
                                                              {
                                                                return false;
                                                              }

                                                            if(!list.Add(element))
                                                              {
                                                                return false;
                                                              }

                                                            List_SetLength((S)(list.GetSize() * sizeof(T)));

                                                            SetLength(List_GetLength() + sizeof(S));  

                                                            return true;
                                                          } 

    bool                                                  List_DeleteAll                                    ()
                                                          {
                                                            list.DeleteAll();

                                                            List_SetLength(0);
                                                            SetLength(0);

                                                            return true;
                                                          }
    
    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_LIST* extension)
                                                          {
                                                            if(!extension)
                                                              {
                                                                return false;
                                                              }

                                                            extension->List_DeleteAll();

                                                            extension->SetType(GetType());  

                                                            for(XDWORD c=0; c<list.GetSize(); c++)
                                                              {
                                                                T element = list.Get(c);      
                                                                if(!extension->List_Add(element))
                                                                  {
                                                                    return false;
                                                                  }
                                                              }

                                                            return true;
                                                          }                                                          

    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_LIST* extension)
                                                          {
                                                            if(!extension)
                                                              {
                                                                return false;
                                                              }

                                                            List_DeleteAll();

                                                            SetType(extension->GetType());  

                                                            for(XDWORD c=0; c<extension->List_Get()->GetSize(); c++)
                                                              {
                                                                T element = extension->List_Get()->Get(c);      
                                                                if(!List_Add(element))
                                                                  {
                                                                    return false;
                                                                  }
                                                              }

                                                            return true;
                                                          }
                                                                                                                
    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug)
                                                          {
                                                            XDWORD size = list.GetSize() * sizeof(T);

                                                            if(((XDWORD)(S)size != size) || (size > (0xFFFF - sizeof(S))))
                                                              {
                                                                return false;
                                                              }

                                                            List_SetLength((S)size);
                                                            SetLength((XWORD)(sizeof(S) + List_GetLength()));

                                                            if(!DIOSTREAMTLS_MSG_EXTENSION::SetToBuffer(buffer, showdebug))
                                                              {
                                                                return false;
                                                              }

                                                            if(!buffer.Add((S)list_length))
                                                              {
                                                                return false;
                                                              }

                                                            for(XDWORD c=0; c<list.GetSize(); c++)
                                                              {
                                                                T element = List_Get()->Get(c);      
                                                                if(!buffer.Add(element))
                                                                  {
                                                                    return false;
                                                                  }
                                                              }

                                                            return true;
                                                          }                                                                                                     

    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug)
                                                          {
                                                            if(!DIOSTREAMTLS_MSG_EXTENSION::GetFromBuffer(buffer, showdebug))
                                                              {
                                                                return false;
                                                              }

                                                            if((GetLength() < sizeof(S)) || (buffer.GetSize() != GetLength()))
                                                              {
                                                                return false;
                                                              }

                                                            if(!buffer.Extract(list_length))
                                                              {
                                                                return false;
                                                              }

                                                            if((List_GetLength() != buffer.GetSize()) || (List_GetLength() % sizeof(T)))
                                                              {
                                                                return false;
                                                              }

                                                            List_Get()->DeleteAll();

                                                            XDWORD nsize = List_GetLength() / sizeof(T);

                                                            for(XDWORD c=0; c<nsize; c++)
                                                              {
                                                                T element = 0;

                                                                if(!buffer.Extract(element))
                                                                  {
                                                                    List_Get()->DeleteAll();
                                                                    return false;
                                                                  }

                                                                if(!List_Get()->Add(element))
                                                                  {
                                                                    List_Get()->DeleteAll();
                                                                    return false;
                                                                  }
                                                              }

                                                            return buffer.IsEmpty();
                                                          }                                                        
                                               
  private:

    void                                                  Clean                                             ()
                                                          {
                                                            list.DeleteAll();
                                                            list_length = 0;
                                                          }
                                                           
    S                                                     list_length;
    XVECTOR<T>                                            list;
};   

class DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME         ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME         ();

    XWORD                                                 Name_GetType                                      ();                                            
    void                                                  Name_SetType                                      (XWORD name_type);
                                                          
    XWORD                                                 Name_GetLength                                    ();                                          
    void                                                  Name_SetLength                                    (XWORD name_length);

    XSTRING*                                              Name_GetHost                                      (); 

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername);  

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();   

    XBYTE                                                 name_type;                                  
    XWORD                                                 name_length;     
    XSTRING                                               hostname;                                
};

class DIOSTREAMTLS_MSG_EXTENSION_SNI : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SNI                    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SNI                    ();

    XWORD                                                 List_GetLength                                    ();                                          
    void                                                  List_SetLength                                    (XWORD list_length);

    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME*>*  List_Get                                          ();
    bool                                                  List_Add                                          (DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* servername);
    bool                                                  List_DeleteAll                                    ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_SNI* extension);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_SNI* extension);  
                                                       
    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);
                                               
  private:

    void                                                  Clean                                             ();   

    XWORD                                                 list_length;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME*>   list;
};


class DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XWORD, XWORD>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS        ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS        ();
                                                
};


class DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XBYTE, XBYTE>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS         ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS         ();
                                                
};

class DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XWORD, XWORD>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS    ();
                                                
};

class DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XWORD, XWORD>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT();

};

class DIOSTREAMTLS_MSG_EXTENSION_ALPN : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_ALPN                   ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_ALPN                   ();

    XWORD                                                 List_GetLength                                    ();                                          
    void                                                  List_SetLength                                    (XWORD list_length);

    XBUFFER*                                              List_GetBuffer                                    (); 
    XDWORD                                                List_GetNProtocols                                ();
    bool                                                  List_Get                                          (XDWORD index, XBUFFER& protocol);
    bool                                                  List_Get                                          (XDWORD index, DIOSTREAMTLS_ALPN_TYPE& alpn_type);
    bool                                                  List_Is                                           (XBUFFER& protocol);
    bool                                                  List_Is                                           (DIOSTREAMTLS_ALPN_TYPE alpn_type);
    bool                                                  List_Add                                          (XBUFFER& protocol);
    bool                                                  List_Add                                          (DIOSTREAMTLS_ALPN_TYPE alpn_type);
    bool                                                  List_DeleteAll                                    ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_ALPN* extension);  
                                                       
    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);
                                               
  private:

    void                                                  Clean                                             ();   
    
    XWORD                                                 list_length;
    XBUFFER                                               list_buffer;                                                
};


class DIOSTREAMTLS_MSG_EXTENSION_EMS : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_EMS                    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_EMS                    ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_EMS* extension);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_EMS* extension);  
                                                       
    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);
                                              
  private:

    void                                                  Clean                                             ();   
};

class DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XWORD, XBYTE>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS      ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS      ();
                                                
};

class DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER ();

    XWORD                                                 GetVersion                                        ();
    void                                                  SetVersion                                        (XWORD version);

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XWORD                                                 version;
};

class DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES : public DIOSTREAMTLS_MSG_EXTENSION_LIST<XBYTE, XBYTE>
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES    ();
                                                
};

class DIOSTREAMTLS_MSG_EXTENSION_KEY
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_KEY                    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_KEY                    ();

    

    XWORD                                                 GetKeyType                                        ();
    void                                                  SetKeyType                                        (XWORD keytype);

    XWORD                                                 GetLengthKeyData                                  ();                                          
    void                                                  SetLengthKeyData                                  (XWORD lengthkeydata);

    XBUFFER*                                              GetKeyData                                        ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_KEY* key);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_KEY* key);  

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();   

    XWORD                                                 keytype;
    XWORD                                                 lengthkeydata;    
    XBUFFER                                               keydata;
};

class DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE               ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE               ();

    XWORD                                                 List_GetLength                                    ();                                          
    void                                                  List_SetLength                                    (XWORD list_length);

    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_KEY*>*             List_Get                                          ();
    bool                                                  List_Add                                          (DIOSTREAMTLS_MSG_EXTENSION_KEY* key);
    bool                                                  List_DeleteAll                                    ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension);  
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* extension);  
                                                       
    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);                                           
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);
                                               
  private:

    void                                                  Clean                                             ();   

    XWORD                                                 list_length;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_KEY*>              list;
};

class DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER        ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER        ();

    DIOSTREAMTLS_MSG_EXTENSION_KEY*                       GetKey                                            ();

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    DIOSTREAMTLS_MSG_EXTENSION_KEY                        key;
};

class DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST ();

    XWORD                                                 GetSelectedGroup                                  ();
    void                                                  SetSelectedGroup                                  (XWORD selectedgroup);

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XWORD                                                 selectedgroup;
};

class DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY            ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY            ();

    XWORD                                                 GetLengthIdentity                                 ();
    void                                                  SetLengthIdentity                                 (XWORD lengthidentity);

    XBUFFER*                                              GetIdentity                                       ();

    XDWORD                                                GetObfuscatedTicketAge                            ();
    void                                                  SetObfuscatedTicketAge                            (XDWORD obfuscatedticketage);

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity);
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity);

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XWORD                                                 lengthidentity;
    XBUFFER                                               identity;
    XDWORD                                                obfuscatedticketage;
};

// ClientHello shape (RFC 8446 4.2.11): a list of PskIdentity entries plus a list of PskBinderEntry entries, each
// its own independently length-prefixed vector. The server only ever sends the single-value form below.
class DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY           ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY           ();

    XWORD                                                 Identities_GetLength                              ();
    void                                                  Identities_SetLength                              (XWORD identities_length);
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY*>*     Identities_GetAll                                 ();
    bool                                                  Identities_Add                                    (DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity);
    bool                                                  Identities_DeleteAll                              ();

    XWORD                                                 Binders_GetLength                                 ();
    void                                                  Binders_SetLength                                 (XWORD binders_length);
    XVECTOR<XBUFFER*>*                                    Binders_GetAll                                    ();
    bool                                                  Binders_Add                                       (XBUFFER* binder);
    bool                                                  Binders_DeleteAll                                 ();

    bool                                                  CopyTo                                            (DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension);
    bool                                                  CopyFrom                                          (DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* extension);

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XWORD                                                 identities_length;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY*>      identities;

    XWORD                                                 binders_length;
    XVECTOR<XBUFFER*>                                     binders;
};

class DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER    ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER    ();

    XWORD                                                 GetSelectedIdentity                               ();
    void                                                  SetSelectedIdentity                               (XWORD selectedidentity);

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XWORD                                                 selectedidentity;
};


class DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN : public DIOSTREAMTLS_MSG_EXTENSION
{
  public:
                                                          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN                ();
    virtual                                              ~DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN                ();

    XBUFFER*                                              GetData                                           ();

    bool                                                  SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                                  GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                                  Clean                                             ();

    XBUFFER                                               data;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


DIOSTREAMTLS_MSG_EXTENSION*                              DIOSTREAMTLS_MSG_EXTENSION_Create                 (XWORD type, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context);
bool                                                     DIOSTREAMTLS_MSG_EXTENSION_Extract                (XBUFFER& buffer, DIOSTREAMTLS_MSG_EXTENSION_CONTEXT context, DIOSTREAMTLS_MSG_EXTENSION*& extension);
