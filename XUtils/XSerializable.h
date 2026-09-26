/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSerializable.h
* 
* @class      XSERIALIZABLE
* @brief      eXtended Utils Serializable class
* @ingroup    XUTILS
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

#include "XBase.h"
#include "XString.h"
#include "XBuffer.h"
#include "XVariant.h"
#include "XVector.h"

#ifdef XSERIALIZABLE_JSON_ACTIVE
#include "XFileJSON.h"
#endif

#include "XSerializationMethod.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSERIALIZABLE
{
  public:

                                   XSERIALIZABLE        ();
    virtual                       ~XSERIALIZABLE        ();

    #ifdef XSERIALIZABLE_BINARY_ACTIVE
    static XSERIALIZATIONMETHOD*   CreateInstance       (XBUFFER& databinary); 
    #endif

    #ifdef XSERIALIZABLE_JSON_ACTIVE
    static XSERIALIZATIONMETHOD*   CreateInstance       (XFILEJSON& fileJSON);    
    #endif

    template<class T>
    bool                           Primitive_Add        (T var, XCHAR* name)
                                   {                               
                                     return serializationmethod->Add(var, name);                                 
                                   }

    template<class T>
    bool                           Primitive_Extract    (T& var, XCHAR* name)
                                   {
                                     return serializationmethod->Extract(var, name);                               
                                   }

    template<class T>
    bool                           Class_Add            (T* var, XCHAR* name)
                                   {                                        
                                     if(!var || !dynamic_cast<XSERIALIZABLE*>(var))
                                       {
                                         return false;
                                       }
                                
                                     if(!serializationmethod || !serializationmethod->AddStruct(name, true)) return false;

                                     var->SetSerializationMethod(serializationmethod);
                                     bool status = var->Serialize();            
                                     
                                     if(!serializationmethod->AddStruct(name, false)) status = false;

                                     return status;
                                   }

    template<class T>
    bool                           Class_Extract        (T* var, XCHAR* name)
                                   {
                                     if(!var)
                                       {
                                         return false;
                                       }

                                     if(!dynamic_cast<XSERIALIZABLE*>(var))
                                       {
                                         return false;
                                       }
                                                                 
                                     if(!serializationmethod || !serializationmethod->ExtractStruct(name)) return false;
                                     var->SetSerializationMethod(serializationmethod);
                                     bool status = var->Deserialize();
                                     if(!serializationmethod->ExtractStruct(NULL)) status = false;
                                                             
                                     return status;
                                   }                              
  
    template<class T>
    bool                           XVector_Add          (XVECTOR<T*>* var, XCHAR* name)
                                   { 
                                     if(!var || (var->GetSize() && !dynamic_cast<XSERIALIZABLE*>(var->Get(0))))  
                                       {
                                         return false;
                                       }
                                       
                                     if(!serializationmethod || !serializationmethod->AddArray(var->GetSize(), name, true)) return false;
                                     
                                     for(XDWORD c=0; c<var->GetSize(); c++)
                                       {
                                         T* element = var->Get(c);
                                         if(!element || !serializationmethod->AddStruct(NULL, true)) return false;
                                         element->SetSerializationMethod(serializationmethod);
                                         bool status = element->Serialize();
                                         if(!serializationmethod->AddStruct(NULL, false) || !status) return false;
                                       }

                                     return serializationmethod->AddArray(var->GetSize(), name, false);
                                   }

    template<class T>
    bool                           XVector_Extract      (XVECTOR<T*>* var, XCHAR* name)
                                   {
                                     if(!var || (var->GetSize() && !dynamic_cast<XSERIALIZABLE*>(var->Get(0))))  
                                       {
                                         return false;
                                       }
                                     if(!serializationmethod || !serializationmethod->ExtractArray(var->GetSize(), name)) return false;
                                     
                                     for(XDWORD c=0; c<var->GetSize(); c++)
                                       {
                                         T* element = var->Get(c);
                                         if(element)
                                           {
                                             element->SetSerializationMethod(serializationmethod);

                                             if(!serializationmethod->ExtractArrayElement(c, name, true)) return false;
                                             bool status = element->Deserialize();
                                             if(!serializationmethod->ExtractArrayElement(c, name, false) || !status) return false;
                                           }
                                          else return false;
                                       }

                                     return true;
                                   }

    template<class T>
    bool                           XVectorClass_Add          (XVECTOR<T*>* var, XCHAR* name, XCHAR* nameclass)
                                   { 
                                     if(!var || (var->GetSize() && !dynamic_cast<XSERIALIZABLE*>(var->Get(0))))  
                                       {
                                         return false;
                                       }
                                       
                                     if(!serializationmethod || !serializationmethod->AddArray(var->GetSize(), name, true)) return false;
                                     
                                     for(XDWORD c=0; c<var->GetSize(); c++)
                                       {
                                         T* element = var->Get(c);
                                         if(!element || !Class_Add<T>(element, nameclass)) return false;
                                       }

                                     return serializationmethod->AddArray(var->GetSize(), name, false);
                                   }

    template<class T>
    bool                           XVectorClass_Extract      (XVECTOR<T*>* var, XCHAR* name, XCHAR* nameclass)
                                   {
                                     if(!var || (var->GetSize() && !dynamic_cast<XSERIALIZABLE*>(var->Get(0))))  
                                       {
                                         return false;
                                       }
                                     if(!serializationmethod || !serializationmethod->ExtractArray(var->GetSize(), name)) return false;
                                     
                                     for(XDWORD c=0; c<var->GetSize(); c++)
                                       {
                                         T* element = var->Get(c);
                                         if(element)
                                           {
                                             element->SetSerializationMethod(serializationmethod);

                                             if(!serializationmethod->ExtractArrayElement(c, name, true)) return false;
                                             bool status = element->Deserialize();
                                             if(!serializationmethod->ExtractArrayElement(c, name, false) || !status) return false;
                                           }
                                          else return false;
                                       }

                                     return true;
                                   }

    XSERIALIZATIONMETHOD*          GetSerializationMethod     ();
    void                           SetSerializationMethod     (XSERIALIZATIONMETHOD* serializationmethod);

    bool                           DoSerialize                ();    
    bool                           DoDeserialize              ();   

    bool                           DoSerialize                (XSERIALIZATIONMETHOD* serializationmethod);    
    bool                           DoDeserialize              (XSERIALIZATIONMETHOD* serializationmethod);   

    bool                           HasBeenChanged             ();
    void                           SetHasBeenChanged          (bool hasbeenchanged);
                                     
    virtual bool                   Serialize                  ();    
    virtual bool                   Deserialize                ();   
    
  private:

    void                           Clean                      ();   

    bool                           hasbeenchanged;
    XSERIALIZATIONMETHOD*          serializationmethod; 

    XBUFFER                        cache;
       
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



