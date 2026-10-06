/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileJSON.cpp
* 
* @class      XFILEJSON
* @brief      eXtended Utils JSON (JavaScript Object Notation) file class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFileJSON.h"

#include <stdio.h>
#include <string.h>

#include "XTrace.h"
#include "XVariant.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XLanguage_ISO_639_3.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE::XFILEJSONVALUE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE::XFILEJSONVALUE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE::~XFILEJSONVALUE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE::~XFILEJSONVALUE()
{
  Delete();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUETYPE XFILEJSONVALUE::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILEJSONVALUETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUETYPE XFILEJSONVALUE::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONVALUE::SetType(XFILEJSONVALUETYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONVALUE::SetType(XFILEJSONVALUETYPE type)
{
  this->type = type;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILEJSONVALUE::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEJSONVALUE::GetName()
{
  return name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::SetName(XSTRING& name)
* @brief      Set name
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::SetName(XSTRING& name)
{
  XSTRING* _name = GEN_NEW XSTRING(name);
  if(!_name) return false;

  this->name = _name;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::SetName(XCHAR* name)
* @brief      Set name
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::SetName(XCHAR* name)
{
  XSTRING* _name = GEN_NEW XSTRING(name);
  if(!_name) return false;

  this->name = _name;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* XFILEJSONVALUE::GetValue()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* XFILEJSONVALUE::GetValue()
{
  return &value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::GetValueBoolean()
* @brief      Get value boolean
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::GetValueBoolean()
{
  return (bool)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         short XFILEJSONVALUE::GetValueShort()
* @brief      Get value short
* @ingroup    XUTILS
* 
* @return     short : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
short XFILEJSONVALUE::GetValueShort()
{
  return (short)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD XFILEJSONVALUE::GetValueWord()
* @brief      Get value word
* @ingroup    XUTILS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD XFILEJSONVALUE::GetValueWord()
{
  return (XWORD)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILEJSONVALUE::GetValueInteger()
* @brief      Get value integer
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILEJSONVALUE::GetValueInteger()
{
  return (int)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILEJSONVALUE::GetValueDWord()
* @brief      Get value double integer
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILEJSONVALUE::GetValueDWord()
{
  return (XDWORD)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         long long XFILEJSONVALUE::GetValueDoubleInteger()
* @brief      Get value double integer
* @ingroup    XUTILS
* 
* @return     long long : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
long long XFILEJSONVALUE::GetValueDoubleInteger()
{
  return (long long)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XFILEJSONVALUE::GetValueQWord()
* @brief      Get value Q word
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XFILEJSONVALUE::GetValueQWord()
{
  return (XQWORD)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XFILEJSONVALUE::GetValueFloating()
* @brief      Get value floating
* @ingroup    XUTILS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XFILEJSONVALUE::GetValueFloating()
{
  return (float)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double XFILEJSONVALUE::GetValueDoubleFloat()
* @brief      Get value double float
* @ingroup    XUTILS
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double XFILEJSONVALUE::GetValueDoubleFloat()
{
  return (double)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING XFILEJSONVALUE::GetValueString()
* @brief      Get value string
* @ingroup    XUTILS
* 
* @return     XSTRING : Requested string.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING XFILEJSONVALUE::GetValueString()
{  
  XSTRING string = (const XSTRING&)value;
  
  return string;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSONVALUE::GetValueObject()
* @brief      Get value object
* @ingroup    XUTILS
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSONVALUE::GetValueObject()
{
  return (XFILEJSONOBJECT*)GetValuePointer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONARRAY* XFILEJSONVALUE::GetValueArray()
* @brief      Get value array
* @ingroup    XUTILS
* 
* @return     XFILEJSONARRAY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONARRAY* XFILEJSONVALUE::GetValueArray()
{
  return (XFILEJSONARRAY*)GetValuePointer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XFILEJSONVALUE::GetValuePointer()
* @brief      Get value pointer
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XFILEJSONVALUE::GetValuePointer()
{
  return (void*)(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(void)
* @brief      Set value
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(void)
{
  type = XFILEJSONVALUETYPE_NULL;
  value.Set(XVARIANT_TYPE_NULL);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(bool boolean)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  boolean : Boolean value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(bool boolean)
{
  type = XFILEJSONVALUETYPE_BOOLEAN;
  value = (bool)boolean;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(short number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(short number)
{
  type  =  XFILEJSONVALUETYPE_SHORT;
  value = (short)number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XWORD number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XWORD number)
{
  type  =  XFILEJSONVALUETYPE_WORD;
  value = (XWORD)number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(int number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(int number)
{
  type  =  XFILEJSONVALUETYPE_INTEGER;
  value = (int)number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XDWORD number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XDWORD number)
{
  type  = XFILEJSONVALUETYPE_DWORD;
  value = (XDWORD)number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(long long number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(long long number)
{
  type =  XFILEJSONVALUETYPE_DOUBLEINTEGER;
  value = number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XQWORD number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XQWORD number)
{
  type =  XFILEJSONVALUETYPE_QWORD;
  value = number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(float number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(float number)
{
  type  =  XFILEJSONVALUETYPE_FLOAT;
  value = number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(double number)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(double number)
{  
  type =  XFILEJSONVALUETYPE_DOUBLEFLOAT;
  value = number;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XSTRING& string)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XSTRING& string)
{
  return Set(string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XCHAR* string)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XCHAR* string)
{  
  type =  XFILEJSONVALUETYPE_STRING;
  value = (XCHAR*)(string);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XFILEJSONOBJECT* object)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  object : Object pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XFILEJSONOBJECT* object)
{
  if(!object) return false;

  type = XFILEJSONVALUETYPE_OBJECT; 
  value.Set(XVARIANT_TYPE_POINTER, (void*)object, sizeof(void*));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XFILEJSONARRAY* array)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  array : Array pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XFILEJSONARRAY* array)
{
  if(!array) return false;

  type = XFILEJSONVALUETYPE_ARRAY;
  value.Set(XVARIANT_TYPE_POINTER, (void*)array, sizeof(void*));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Set(XVARIANT* variant)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  variant : Variant pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XVARIANT* variant)
{
  if(!variant) return false;

  XSTRING tempostr;
  
  switch(variant->GetType())
    {
      case XVARIANT_TYPE_NULL           : type = XFILEJSONVALUETYPE_NULL;   
                                          value = (*variant);        
                                          break;

      case XVARIANT_TYPE_BOOLEAN        : type = XFILEJSONVALUETYPE_BOOLEAN;        
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_SHORT          : type = XFILEJSONVALUETYPE_SHORT;          
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_WORD           : type = XFILEJSONVALUETYPE_WORD;           
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_INTEGER        : type = XFILEJSONVALUETYPE_INTEGER;        
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_DWORD          : type = XFILEJSONVALUETYPE_DWORD;          
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_DOUBLEINTEGER  : type = XFILEJSONVALUETYPE_DOUBLEINTEGER;  
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_QWORD          : type = XFILEJSONVALUETYPE_QWORD;          
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_FLOAT          : type = XFILEJSONVALUETYPE_FLOAT;          
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_DOUBLE         : type = XFILEJSONVALUETYPE_DOUBLEFLOAT;    
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_CHAR           : type = XFILEJSONVALUETYPE_STRING;                                                    
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;

      case XVARIANT_TYPE_XCHAR          : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;  

      case XVARIANT_TYPE_STRING         : type = XFILEJSONVALUETYPE_STRING;         
                                          value = (*variant);
                                          break;

      case XVARIANT_TYPE_DATE           : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;

      case XVARIANT_TYPE_TIME           : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;

      case XVARIANT_TYPE_DATETIME       : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;

      case XVARIANT_TYPE_BUFFER         : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break;

      case XVARIANT_TYPE_POINTER        : type = XFILEJSONVALUETYPE_STRING;         
                                          variant->ToString(tempostr);
                                          value = tempostr;
                                          break; 

                           default      : break;                     
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONVALUE::Delete()
* @brief      Delete resource
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONVALUE::Delete()
{
  if(name)
    {
      GEN_DELETE name;
      name = NULL;
    }

  switch(type)
    {      
      case XFILEJSONVALUETYPE_NULL            :
      case XFILEJSONVALUETYPE_BOOLEAN         :
      case XFILEJSONVALUETYPE_SHORT           :
      case XFILEJSONVALUETYPE_WORD            :
      case XFILEJSONVALUETYPE_INTEGER         :
      case XFILEJSONVALUETYPE_DWORD           :
      case XFILEJSONVALUETYPE_DOUBLEINTEGER   :
      case XFILEJSONVALUETYPE_QWORD           :
      case XFILEJSONVALUETYPE_FLOAT           :
      case XFILEJSONVALUETYPE_DOUBLEFLOAT     : value.Destroy();
                                                break;

      case XFILEJSONVALUETYPE_STRING          : { //void* pointer = value;
                                                  //XSTRING* string = (XSTRING*)pointer;
                                                  //GEN_DELETE string;
                                                  value.Destroy();
                                                }                                                
                                                break;

      case XFILEJSONVALUETYPE_OBJECT          : { void* pointer = value;
                                                  XFILEJSONOBJECT* object = (XFILEJSONOBJECT*)pointer;
                                                  GEN_DELETE object;
                                                  value.Destroy();
                                                }
                                                break;

      case XFILEJSONVALUETYPE_ARRAY           : { void* pointer = value;
                                                  XFILEJSONARRAY* array = (XFILEJSONARRAY*)pointer;
                                                  GEN_DELETE array;
                                                  value.Destroy();
                                                }
                                                break;

                                 default      : break; 

    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE* XFILEJSONVALUE::Clone()
* @brief      Clone
* @ingroup    XUTILS
* 
* @return     XFILEJSONVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSONVALUE::Clone()
{
  XFILEJSONVALUE* newvalue = GEN_NEW XFILEJSONVALUE();
  if(!newvalue) return NULL;  

  newvalue->SetType(type);
  newvalue->SetName(name->Get());

  (*newvalue->GetValue()) = value;

  return newvalue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONVALUE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONVALUE::Clean()
{
  type  = XFILEJSONVALUETYPE_NULL;
  name  = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT::XFILEJSONOBJECT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT::XFILEJSONOBJECT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT::~XFILEJSONOBJECT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT::~XFILEJSONOBJECT()
{
  DeleteAllValues();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::IsArray()
* @brief      Check if array
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::IsArray()
{
  return isarray;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONOBJECT::SetIsArray(bool isarray)
* @brief      Set is array
* @ingroup    XUTILS
* 
* @param[in]  isarray : Isarray value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONOBJECT::SetIsArray(bool isarray)
{
  this->isarray = isarray;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::Add(XFILEJSONVALUE* value)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XFILEJSONVALUE* value)
{
  if(!value) return false;

  values.Add(value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::Add(XCHAR* name, XFILEJSONARRAY* array)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  array : Array pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XCHAR* name, XFILEJSONARRAY* array)
{
  if(!array) return false;

  array->SetFather(this);

  XFILEJSONVALUE* value = GEN_NEW XFILEJSONVALUE();
  if(!value) return false;

  value->SetName(name);
  value->Set(array);

  values.Add(value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::Add(XSTRING& name, XFILEJSONARRAY* array)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  array : Array pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XSTRING& name, XFILEJSONARRAY* array)
{
  return Add(name.Get(), array);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::Add(XCHAR* name, XFILEJSONOBJECT* object)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  object : Object pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XCHAR* name, XFILEJSONOBJECT* object)
{
  if(!object) return false;

  object->SetFather(this);

  XFILEJSONVALUE* value = GEN_NEW XFILEJSONVALUE();
  if(!value) return false;

  value->SetName(name);
  value->Set(object);

  values.Add(value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::Add(XSTRING& name, XFILEJSONOBJECT* object)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  object : Object pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XSTRING& name, XFILEJSONOBJECT* object)
{
  return Add(name.Get(), object);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSONOBJECT::GetFather()
* @brief      Get father
* @ingroup    XUTILS
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSONOBJECT::GetFather()
{
  return father;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONOBJECT::SetFather(XFILEJSONOBJECT* father)
* @brief      Set father
* @ingroup    XUTILS
* 
* @param[in]  father : Father pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONOBJECT::SetFather(XFILEJSONOBJECT* father)
{
  this->father = father;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XFILEJSONVALUE*>* XFILEJSONOBJECT::GetValues()
* @brief      Get values
* @ingroup    XUTILS
* 
* @return     XVECTOR<XFILEJSONVALUE*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XFILEJSONVALUE*>* XFILEJSONOBJECT::GetValues()
{
  return &values;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONOBJECT::DeleteAllValues()
* @brief      Delete all values
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONOBJECT::DeleteAllValues()
{
  if(values.IsEmpty())  return false;

  values.DeleteContents();
  values.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSONOBJECT::Clone()
* @brief      Clone
* @ingroup    XUTILS
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSONOBJECT::Clone()
{
  XFILEJSONOBJECT* cloneobj = GEN_NEW XFILEJSONOBJECT();
  if(!cloneobj) return NULL;

  cloneobj->SetFather(father);
  cloneobj->SetIsArray(isarray);

  for(XDWORD c=0; c<GetValues()->GetSize(); c++)
    {      
      XFILEJSONVALUE* value    = GetValues()->Get(c); 
      XFILEJSONVALUE* newvalue = NULL;  
     
      if(value)
        {
          newvalue =  value->Clone();
          if(newvalue) cloneobj->Add(newvalue);
        }
    }

  return cloneobj;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONOBJECT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONOBJECT::Clean()
{
  father  = NULL;
  isarray = false;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONARRAY::XFILEJSONARRAY() : XFILEJSONOBJECT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONARRAY::XFILEJSONARRAY() : XFILEJSONOBJECT()
{
  Clean();

  isarray = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONARRAY::~XFILEJSONARRAY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONARRAY::~XFILEJSONARRAY()
{
  DeleteAllValues();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILEJSONARRAY::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEJSONARRAY::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONARRAY::SetName(XSTRING& name)
* @brief      Set name
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONARRAY::SetName(XSTRING& name)
{
  this->name = name.Get();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSONARRAY::SetName(XCHAR* name)
* @brief      Set name
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSONARRAY::SetName(XCHAR* name)
{
  this->name = name;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONARRAY* XFILEJSONARRAY::Clone()
* @brief      Clone
* @ingroup    XUTILS
* 
* @return     XFILEJSONARRAY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONARRAY* XFILEJSONARRAY::Clone()
{
  XFILEJSONARRAY* clonearray = GEN_NEW XFILEJSONARRAY();
  if(!clonearray) return NULL;

  clonearray->SetName(name.Get());

  return clonearray;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSONARRAY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSONARRAY::Clean()
{

};






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSON::XFILEJSON(): XFILETXT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSON::XFILEJSON(): XFILETXT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSON::~XFILEJSON()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSON::~XFILEJSON()
{
  DeleteAllLines();

  DeleteAllObjects();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSON::GetRoot()
* @brief      Get root
* @ingroup    XUTILS
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSON::GetRoot()
{
  return root;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::SetRoot(XFILEJSONOBJECT* root)
* @brief      Set root
* @ingroup    XUTILS
* 
* @param[in]  root : Root pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::SetRoot(XFILEJSONOBJECT* root)
{
  DeleteAllObjects();

  this->root = root;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::DecodeAllLines()
* @brief      Decode all lines
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::DecodeAllLines()
{
  if(root) 
    {
      GEN_DELETE root;
      root = NULL;
    }

  if(!GetAllInOneLine(all)) return false;

  DeleteAllLines();

  //--------------------------------------------------------
  // Search Object Root

  int positionbracket = 0;
  int positionbrace   = 0;
  int position        = 0;

  if(!SearchControlCharacter(XFILEJSONCONTROLCHAR_OPENBRACKET, positionbracket)) positionbracket = 100000000;
  if(!SearchControlCharacter(XFILEJSONCONTROLCHAR_OPENBRACE  , positionbrace))   positionbrace   = 100000000;

  if(positionbracket > positionbrace)  position = positionbrace + 1;  else position =  positionbracket;

  root = GEN_NEW XFILEJSONOBJECT();
  if(!root) return false;

  bool status = DecodeObject(position, true, root);

  all.Empty();

  if(!status)
    {
      GEN_DELETE root;
      root = NULL;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::EncodeAllLines(bool istabulatedline)
* @brief      Encode all lines
* @ingroup    XUTILS
* 
* @param[in]  istabulatedline : Istabulatedline value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::EncodeAllLines(bool istabulatedline)
{
  if(!root) return false;

  encodelevel  = 0;

  return EncodeObject(true, root, istabulatedline);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::ReadAndDecodeAllLines()
* @brief      Read and decode all lines
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::ReadAndDecodeAllLines()
{
  if(!IsOpen())      return false;

  if(!ReadAllFile()) return false;

  return DecodeAllLines();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline, XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF)
* @brief      Write and encode all lines
* @ingroup    XUTILS
* 
* @param[in]  istabulatedline : Istabulatedline value.
* @param[in]  formatchar : Formatchar value.
* @param[in]  typeLF : Type LF value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline, XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF)
{
  SetFormatChar(formatchar);
  SetTypeLF(typeLF);

  return WriteAndEncodeAllLines(istabulatedline);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline)
* @brief      Write and encode all lines
* @ingroup    XUTILS
* 
* @param[in]  istabulatedline : Istabulatedline value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline)
{
  if(!IsOpen()) return false;

  file->SetPosition(0);

  EncodeAllLines(istabulatedline);

  bool status = WriteAllFile();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::DeleteAllObjects()
* @brief      Delete all objects
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::DeleteAllObjects()
{
  if(!root) return false;

  GEN_DELETE root;
  root = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSON::GetObj(XCHAR* name, XFILEJSONOBJECT* startobject)
* @brief      Get obj
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  startobject : Startobject pointer to use.
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSON::GetObj(XCHAR* name, XFILEJSONOBJECT* startobject)
{  
  XFILEJSONOBJECT* _startobject  = startobject?startobject:GetRoot();
  if(!_startobject)  return NULL;

  if(_startobject->IsArray())
    {
      XFILEJSONARRAY* array = (XFILEJSONARRAY*)(_startobject);
      if(!array) return NULL;

      if(!array->GetValues()) return NULL;

      for(int c=0; c<(int)array->GetValues()->GetSize();c++)
        {
          XFILEJSONVALUE* value = (XFILEJSONVALUE*)array->GetValues()->Get(c);
          if(value)
            {
              switch(value->GetType())
                {
                  case XFILEJSONVALUETYPE_OBJECT  :
                  case XFILEJSONVALUETYPE_ARRAY   : { XFILEJSONOBJECT* subobject = GetObjSubValue(name, value);
                                                      if(subobject) return subobject;
                                                    }
                                                    break;
                                        default   : break;
                }
            }
        }
    }
   else
    {
      XFILEJSONOBJECT* object = _startobject;
      if(!object) return NULL;

      if(!object->GetValues()) return NULL;

      for(int c=0; c<(int)object->GetValues()->GetSize();c++)
        {
          XFILEJSONVALUE* value = (XFILEJSONVALUE*)object->GetValues()->Get(c);
          if(value)
            {
              switch(value->GetType())
                {
                  case XFILEJSONVALUETYPE_OBJECT  :
                  case XFILEJSONVALUETYPE_ARRAY   : { XFILEJSONOBJECT* subobject = GetObjSubValue(name, value);
                                                      if(subobject) return subobject;
                                                    }
                                                    break;
                                        default   : break;
                }
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSON::GetObj(XSTRING& name, XFILEJSONOBJECT* startobject)
* @brief      Get obj
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  startobject : Startobject pointer to use.
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSON::GetObj(XSTRING& name, XFILEJSONOBJECT* startobject)
{
  return GetObj(name.Get(), startobject);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE* XFILEJSON::GetValue(XCHAR* name, XFILEJSONOBJECT* startobject)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  startobject : Startobject pointer to use.
* 
* @return     XFILEJSONVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSON::GetValue(XCHAR* name, XFILEJSONOBJECT* startobject)
{
  XFILEJSONOBJECT* object = startobject?startobject:GetRoot();
  if(!object) return NULL;

  if(!object->GetValues()) return NULL;

  for(int c=0; c<(int)object->GetValues()->GetSize();c++)
    {
      XFILEJSONVALUE* value = (XFILEJSONVALUE*)object->GetValues()->Get(c);
      if(value)
        {
          XFILEJSONVALUETYPE type = value->GetType();
          if((type == XFILEJSONVALUETYPE_OBJECT) || (type ==  XFILEJSONVALUETYPE_ARRAY))
            {
              value = GetValue(name, (XFILEJSONOBJECT*)value->GetValuePointer());
              if(value) return value;
            }
           else
            {
              if(value->GetName())
                {
                  if(!value->GetName()->Compare(name)) return value;
                }
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE* XFILEJSON::GetValue(XSTRING& name, XFILEJSONOBJECT* startobject)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  startobject : Startobject pointer to use.
* 
* @return     XFILEJSONVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSON::GetValue(XSTRING& name, XFILEJSONOBJECT* startobject)
{
  return GetValue(name.Get(), startobject);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONVALUE* XFILEJSON::GetValue(int index, XFILEJSONOBJECT* startobject)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  startobject : Startobject pointer to use.
* 
* @return     XFILEJSONVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSON::GetValue(int index, XFILEJSONOBJECT* startobject)
{
  XFILEJSONOBJECT* object = startobject?startobject:GetRoot();
  if(!object) return NULL;

  if(!object->GetValues()) return NULL;

  return (XFILEJSONVALUE*)object->GetValues()->Get(index);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::ShowTraceJSON(XBYTE color, bool istabulatedline)
* @brief      Show trace JSON
* @ingroup    XUTILS
* 
* @param[in]  color : Color value.
* @param[in]  istabulatedline : Istabulatedline value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::ShowTraceJSON(XBYTE color, bool istabulatedline)
{
  DeleteAllLines();

  EncodeAllLines(istabulatedline);
  
  for(int c=0; c<GetNLines(); c++)
    {      
      XSTRING* line = GetLine(c);
      if(line) 
        {
          XTRACE_PRINTCOLOR(color, line->Get(), NULL);          
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONOBJECT* XFILEJSON::GetObjSubValue(XCHAR* name, XFILEJSONVALUE* value)
* @brief      Get obj sub value
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     XFILEJSONOBJECT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSON::GetObjSubValue(XCHAR* name, XFILEJSONVALUE* value)
{
  XFILEJSONVALUETYPE type = value->GetType();
  if(value->GetName())
    {
      if(!value->GetName()->Compare(name))
        {
          switch(type)
            {
              case XFILEJSONVALUETYPE_OBJECT  :
              case XFILEJSONVALUETYPE_ARRAY   : return (XFILEJSONOBJECT*)value->GetValuePointer();

                                    default   : return NULL;
          }
        }
    }

  switch(type)
    {
      case XFILEJSONVALUETYPE_OBJECT  : { XFILEJSONOBJECT* subobject = GetObj(name, (XFILEJSONOBJECT*)value->GetValuePointer());
                                          if(subobject) return subobject;
                                        }
                                        break;

      case XFILEJSONVALUETYPE_ARRAY   : { XFILEJSONARRAY* subarray = (XFILEJSONARRAY*)value->GetValuePointer();
                                          if(subarray)
                                            {
                                              for(XDWORD d=0; d<subarray->GetValues()->GetSize(); d++)
                                                {
                                                  XFILEJSONVALUE* subvalue = (XFILEJSONVALUE*)subarray->GetValues()->Get(d);
                                                  if(subvalue)
                                                    {
                                                      XFILEJSONOBJECT* subobject = GetObjSubValue(name, subvalue);
                                                      if(subobject) return subobject;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                            default   : return NULL;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::DecodeObject(int& position, bool isobject, XFILEJSONOBJECT* object)
* @brief      Decode object
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* @param[in]  isobject : Isobject value.
* @param[in]  object : Object pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::DecodeObject(int& position, bool isobject, XFILEJSONOBJECT* object)
{
  if(!object) return false;

  while(1)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      XFILEJSONVALUE* value = GEN_NEW XFILEJSONVALUE();
      if(!value) return false;

      if(isobject)
        {
          XSTRING name;
          int     checkposition = position;

          ctrlchar = SearchNextControlCharacter(checkposition);
          if(ctrlchar == XFILEJSONCONTROLCHAR_QUOTE)         
            {
              if(!GetString(checkposition, name))
                {
                  GEN_DELETE value;
                  return false;
                }

              position = checkposition;
            }

          value->SetName(name);

          if(ctrlchar == XFILEJSONCONTROLCHAR_QUOTE)
            {
              ctrlchar = SearchNextControlCharacter(position);
              if(ctrlchar != XFILEJSONCONTROLCHAR_COLON)
                {
                  GEN_DELETE value;
                  return false;
                }

              position++;
            }
        }

      ctrlchar = SearchNextControlCharacter(position);
      switch(ctrlchar)
        {
          case XFILEJSONCONTROLCHAR_OPENBRACE     : { XFILEJSONOBJECT* newobject = GEN_NEW XFILEJSONOBJECT();
                                                      if(!object) return false;

                                                      value->Set(newobject);

                                                      position++;

                                                      if(!DecodeObject(position, true, newobject))
                                                        {
                                                          GEN_DELETE value;
                                                          return false;
                                                        }

                                                      object->GetValues()->Add(value);
                                                    }
                                                    break;

          case XFILEJSONCONTROLCHAR_OPENBRACKET   : { XFILEJSONARRAY* newarray = GEN_NEW XFILEJSONARRAY();
                                                      if(!object) return false;

                                                      value->Set(newarray);

                                                      position++;

                                                      if(!DecodeObject(position, false, (XFILEJSONOBJECT*)newarray))
                                                        {
                                                          GEN_DELETE value;
                                                          return false;
                                                        }

                                                      object->GetValues()->Add(value);
                                                    }
                                                    break;

          case XFILEJSONCONTROLCHAR_NUMBER        : { XSTRING valuestring;
                                                      bool    special = false;

                                                      GetNumber(position, valuestring, special);

                                                      if(!special)
                                                        {
                                                          long long integer = ConvertStringToLongLong(valuestring);

                                                          if((integer >= -2147483647LL - 1LL) && (integer <= 2147483647LL))
                                                            {
                                                              value->Set((int)integer);
                                                            }
                                                           else
                                                            {
                                                              value->Set(integer);
                                                            }
                                                        }
                                                       else
                                                        {
                                                          double floating = valuestring.ConvertToDouble(0, NULL, false);

                                                          value->Set(floating);
                                                        }

                                                      object->GetValues()->Add(value);
                                                    }
                                                    break;

          case XFILEJSONCONTROLCHAR_TEXT          : { XSTRING valuestring;

                                                      GetText(position, valuestring);

                                                      valuestring.ToUpperCase();
                                                      
                                                      value->Set();

                                                      if(!valuestring.Compare(_L("true") , true))  
                                                        {
                                                          value->Set(true);
                                                        }
                                                       else
                                                        {   
                                                          if(!valuestring.Compare(_L("false"), true)) 
                                                            {
                                                              value->Set(false);                                                      
                                                            }
                                                           else
                                                            {     
                                                              if(!valuestring.Compare(_L("null") , false)) 
                                                                {
                                                                  value->Set();
                                                                }                                                                  
                                                            }
                                                        }

                                                      object->GetValues()->Add(value);
                                                    }
                                                    break;

          case XFILEJSONCONTROLCHAR_QUOTE         : { XSTRING valuestring;

                                                      GetString(position, valuestring);

                                                      value->Set(valuestring);

                                                      object->GetValues()->Add(value);
                                                    }
                                                    break;

          case XFILEJSONCONTROLCHAR_CLOSEBRACE    :
          case XFILEJSONCONTROLCHAR_CLOSEBRACKET  : GEN_DELETE value;
                                                    break;

          case XFILEJSONCONTROLCHAR_COLON         : break;          

          case XFILEJSONCONTROLCHAR_NOTCONTROL    :
                                        default   : GEN_DELETE value;
                                                    return false;

        }


      ctrlchar = SearchNextControlCharacter(position);
      if(position != -1)
        {
          if(isobject)
            {
              if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACE)
                {
                  position++;
                  break;
                }
               else
                {
                  if(ctrlchar == XFILEJSONCONTROLCHAR_NOTCONTROL)
                    {
                      return false;
                    }
                }
            }
           else
            {
              if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACKET)
                {
                  position++;
                  break;
                }
               else
                {
                  if(ctrlchar == XFILEJSONCONTROLCHAR_NOTCONTROL)
                    {
                      return false;
                    }
                }
            }

          ctrlchar = SearchNextControlCharacter(position);
          if(ctrlchar!=XFILEJSONCONTROLCHAR_COMMA)
            {
              return false;
            }
           else
            {
              //-----------------------------------
              // COMMA is ADD to end (BAD JSON format)

              position++;

              ctrlchar = SearchNextControlCharacter(position);
              if(isobject)
                {
                  if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACE)
                    {
                      position++;
                      break;
                    }
                   else
                    {
                      if(ctrlchar == XFILEJSONCONTROLCHAR_NOTCONTROL)
                        {
                          return false;
                        }
                    }
                }
               else
                {
                  if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACKET)
                    {
                      position++;
                      break;
                    }
                   else
                    {
                      if(ctrlchar == XFILEJSONCONTROLCHAR_NOTCONTROL)
                        {
                          return false;
                        }
                    }
                }
            }
        }
       else
        {
          break;
        }
    }

  //XTRACE_PRINTCOLOR(2, _L("%d"), position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONCONTROLCHAR XFILEJSON::CheckControlCharacter(XCHAR character)
* @brief      Check control character
* @ingroup    XUTILS
* 
* @param[in]  character : Character value.
* 
* @return     XFILEJSONCONTROLCHAR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONCONTROLCHAR XFILEJSON::CheckControlCharacter(XCHAR character)
{
  XFILEJSONCONTROLCHAR ctrlchar;

  switch(character)
    {
      case _C('{')  : ctrlchar = XFILEJSONCONTROLCHAR_OPENBRACE;     break;
      case _C('}')  : ctrlchar = XFILEJSONCONTROLCHAR_CLOSEBRACE;    break;
      case _C('[')  : ctrlchar = XFILEJSONCONTROLCHAR_OPENBRACKET;   break;
      case _C(']')  : ctrlchar = XFILEJSONCONTROLCHAR_CLOSEBRACKET;  break;
      case _C('\"') : ctrlchar = XFILEJSONCONTROLCHAR_QUOTE;         break;
      case _C(',')  : ctrlchar = XFILEJSONCONTROLCHAR_COMMA;         break;
      case _C(':')  : ctrlchar = XFILEJSONCONTROLCHAR_COLON;         break;
      case _C('\\') : ctrlchar = XFILEJSONCONTROLCHAR_BLACKSLASH;    break;

      case _C('0')  :
      case _C('1')  :
      case _C('2')  :
      case _C('3')  :
      case _C('4')  :
      case _C('5')  :
      case _C('6')  :
      case _C('7')  :
      case _C('8')  :
      case _C('9')  :
      case _C('-')  : ctrlchar = XFILEJSONCONTROLCHAR_NUMBER;        break;

      case _C('.')  :
      case _C('+')  :
      case _C('e')  :
      case _C('E')  : ctrlchar = XFILEJSONCONTROLCHAR_NUMBERSPECIAL; break;

      case _C('\t') :
      case _C('\r') :
      case _C('\n') :
      case _C(' ')  : ctrlchar = XFILEJSONCONTROLCHAR_NOTCONTROL;    break;

           default   : ctrlchar = XFILEJSONCONTROLCHAR_TEXT;          break;
    }

  return ctrlchar;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR XFILEJSON::GetControlCharacter(XFILEJSONCONTROLCHAR ctrlchar)
* @brief      Get control character
* @ingroup    XUTILS
* 
* @param[in]  ctrlchar : Ctrlchar value.
* 
* @return     XCHAR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR XFILEJSON::GetControlCharacter(XFILEJSONCONTROLCHAR ctrlchar)
{
  switch(ctrlchar)
    {
      case XFILEJSONCONTROLCHAR_OPENBRACE     : return _C('{');
      case XFILEJSONCONTROLCHAR_CLOSEBRACE    : return _C('}');
      case XFILEJSONCONTROLCHAR_OPENBRACKET   : return _C('[');
      case XFILEJSONCONTROLCHAR_CLOSEBRACKET  : return _C(']');
      case XFILEJSONCONTROLCHAR_QUOTE         : return _C('\"');
      case XFILEJSONCONTROLCHAR_COMMA         : return _C(',');
      case XFILEJSONCONTROLCHAR_COLON         : return _C(':');
      case XFILEJSONCONTROLCHAR_BLACKSLASH    : return _C('\\');
                                     default  : break;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::AddControlCharacter(XFILEJSONCONTROLCHAR ctrlchar, XSTRING& line)
* @brief      Add control character
* @ingroup    XUTILS
* 
* @param[in]  ctrlchar : Ctrlchar value.
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::AddControlCharacter(XFILEJSONCONTROLCHAR ctrlchar, XSTRING& line)
{
  XCHAR character = GetControlCharacter(ctrlchar);
  if(character)
    {
      line += character;
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEJSONCONTROLCHAR XFILEJSON::SearchNextControlCharacter(int& position)
* @brief      Search next control character
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* 
* @return     XFILEJSONCONTROLCHAR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEJSONCONTROLCHAR XFILEJSON::SearchNextControlCharacter(int& position)
{
  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if(ctrlchar != XFILEJSONCONTROLCHAR_NOTCONTROL)
        {
          position = c;
          return ctrlchar;
        }
    }

  position = XSTRING_NOTFOUND;

  return XFILEJSONCONTROLCHAR_NOTCONTROL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::SearchControlCharacter(XFILEJSONCONTROLCHAR ctrlchar, int& position)
* @brief      Search control character
* @ingroup    XUTILS
* 
* @param[in]  ctrlchar : Ctrlchar value.
* @param[in]  position : Position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::SearchControlCharacter(XFILEJSONCONTROLCHAR ctrlchar, int& position)
{
  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR _ctrlchar;

      _ctrlchar = CheckControlCharacter(all.Get()[c]);
      if(_ctrlchar == ctrlchar)
        {
          position = c;
          return true;
        }
    }

  position = XSTRING_NOTFOUND;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         long long XFILEJSON::ConvertStringToLongLong(XSTRING& string)
* @brief      convert string to long long
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     long long : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
long long XFILEJSON::ConvertStringToLongLong(XSTRING& string)
{
  const XCHAR* text = string.Get();
  if(!text) return 0;

  long long value    = 0;
  bool      negative = false;
  int       c        = 0;

  if(text[c] == _C('-'))
    {
      negative = true;
      c++;
    }
   else
    {
      if(text[c] == _C('+')) c++;
    }

  for(; text[c]; c++)
    {
      if((text[c] < _C('0')) || (text[c] > _C('9'))) break;

      value *= 10;
      value += (long long)(text[c] - _C('0'));
    }

  return negative? -value : value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::GetNumber(int& position, XSTRING& string, bool& special)
* @brief      Get number
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* @param[in]  string : String to use.
* @param[in]  special : Special value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::GetNumber(int& position, XSTRING& string, bool& special)
{
  string.Empty();

  special = false;

  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if((ctrlchar == XFILEJSONCONTROLCHAR_NUMBER)||(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL))
        {
          string += all.Get()[c];
          if(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL) special = true;

        }
       else
        {
          position = c;
          break;
        }
    }

  return string.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::GetText(int& position, XSTRING& string)
* @brief      Get text
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::GetText(int& position, XSTRING& string)
{
  string.Empty();

  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if((ctrlchar == XFILEJSONCONTROLCHAR_TEXT) ||(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL))
        {
          string += all.Get()[c];
        }
       else
        {
          position = c;
          break;
        }
    }

  return string.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::GetString(int& position, XSTRING& string)
* @brief      Get string
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::GetString(int& position, XSTRING& string)
{
  string.Empty();

  if(position < 0) return false;
  if(position >= (int)all.GetSize()) return false;

  XFILEJSONCONTROLCHAR ctrlchar;

  //----------------------------------
  // First Quote
  ctrlchar = CheckControlCharacter(all.Get()[position]);
  if(ctrlchar != XFILEJSONCONTROLCHAR_QUOTE) return false;

  position++;

  bool slashcontrol = false;

  for(int c=position;c<(int)all.GetSize();c++)
    {
      ctrlchar = CheckControlCharacter(all.Get()[c]);

      if((ctrlchar == XFILEJSONCONTROLCHAR_QUOTE) && !slashcontrol)
        {
          position = (c + 1);
          return true;
        }

      string += all.Get()[c];

      if((ctrlchar == XFILEJSONCONTROLCHAR_BLACKSLASH) && !slashcontrol)
        {
          slashcontrol = true;
        }
       else
        {
          slashcontrol = false;
        }
    }

  position = XSTRING_NOTFOUND;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::AddTabs(int level, int spacetabs, XSTRING& string)
* @brief      Add tabs
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  spacetabs : Spacetabs value.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::AddTabs(int level, int spacetabs, XSTRING& string)
{
  if(spacetabs<=0) return false;

  for(int c=0;c<level;c++)
    {
      for(int d=0;d<spacetabs;d++)
        {
          string += _L(" ");
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEJSON::EncodeObject(bool isobject, XFILEJSONOBJECT* object, bool istabulatedline)
* @brief      Encode object
* @ingroup    XUTILS
* 
* @param[in]  isobject : Isobject value.
* @param[in]  object : Object pointer to use.
* @param[in]  istabulatedline : Istabulatedline value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEJSON::EncodeObject(bool isobject, XFILEJSONOBJECT* object, bool istabulatedline)
{
  if(!object) return false;

  XSTRING line;

  if(istabulatedline)
    {
      AddTabs(encodelevel, XFILEJSON_SPACETABS, line);
      encodelevel++;
    }

  AddControlCharacter(isobject?XFILEJSONCONTROLCHAR_OPENBRACE:XFILEJSONCONTROLCHAR_OPENBRACKET, line);
  AddLine(line);
  //XTRACE_PRINTCOLOR(0,line.Get());

  for(int c=0;c<(int)object->GetValues()->GetSize();c++)
    {
      XFILEJSONVALUE* value = (XFILEJSONVALUE*)(object->GetValues()->Get(c));
      if(value)
        {
          line.Empty();
          AddTabs(encodelevel, XFILEJSON_SPACETABS, line);

          XSTRING* name = value->GetName();
          if(isobject && name)
            {
              XSTRING line2;

              line2.Format(_L("%c%s%c : "), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE), name->Get() , GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE));
              line += line2;
            }
    
          switch(value->GetType())
            {              
              case XFILEJSONVALUETYPE_NULL            : line += _L("null");
                                                        break;



              case XFILEJSONVALUETYPE_STRING          : { XSTRING  ptrvaluestring = value->GetValueString();
                                                          XSTRING  valuestring;
                                                         
                                                          valuestring.Format(_L("%c%s%c"), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE), ptrvaluestring.Get(), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE));
                                                          line += valuestring.Get();
                                                        }
                                                        break;              

              case XFILEJSONVALUETYPE_OBJECT          : { XFILEJSONOBJECT* newobject = (XFILEJSONOBJECT*)(value->GetValuePointer());
                                                          if(newobject)
                                                            {
                                                              if((!line.IsEmpty()) && line.HaveCharacters())
                                                                {
                                                                  AddLine(line);
                                                                  //XTRACE_PRINTCOLOR(0,line.Get());
                                                                }

                                                              if(!EncodeObject(true, newobject, istabulatedline)) return false;
                                                              line.Empty();
                                                            }
                                                        }
                                                        break;

              case XFILEJSONVALUETYPE_ARRAY           : { XFILEJSONARRAY* newarray = (XFILEJSONARRAY*)(value->GetValuePointer());
                                                          if(newarray)
                                                            {
                                                              if((!line.IsEmpty()) && line.HaveCharacters())
                                                                {
                                                                  AddLine(line);
                                                                  //XTRACE_PRINTCOLOR(0,line.Get());
                                                                }

                                                              if(!EncodeObject(false, (XFILEJSONOBJECT*)newarray, istabulatedline)) return false;
                                                              line.Empty();
                                                            }
                                                        }
                                                        break;  

                                    default           : { XSTRING valuestring;

                                                          value->GetValue()->ToString(valuestring);

                                                          line += valuestring.Get();   
                                                        }
                                                        break;  
        
            }
        }

      if(c!=(int)(object->GetValues()->GetSize()-1))
        {
          if(line.IsEmpty()) AddTabs(encodelevel, XFILEJSON_SPACETABS, line);

          AddControlCharacter(XFILEJSONCONTROLCHAR_COMMA, line);
        }

      if(!line.IsEmpty())
        {
          AddLine(line);
          //XTRACE_PRINTCOLOR(0,line.Get());
        }

      line.Empty();
    }

  line.Empty();

  if(istabulatedline)
    {
      encodelevel--;
      AddTabs(encodelevel, XFILEJSON_SPACETABS, line);
    }

  AddControlCharacter(isobject?XFILEJSONCONTROLCHAR_CLOSEBRACE:XFILEJSONCONTROLCHAR_CLOSEBRACKET, line);
  AddLine(line);

  //XTRACE_PRINTCOLOR(0,line.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEJSON::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEJSON::Clean()
{
   root         = NULL;
   encodelevel  = 0;
}







