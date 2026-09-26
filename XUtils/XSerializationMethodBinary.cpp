/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSerializationMethodBinary.cpp
* 
* @class      XSERIALIZATIONMETHODBINARY
* @brief      eXtended Utils Serialization binary class
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

#include "XSerializationMethodBinary.h"

#include <stdio.h>
#include <string.h>

#include "XBuffer.h"
#include "XVariant.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool XSERIALIZATIONMETHODBINARY_ReadType(XBUFFER* buffer, XDWORD& position, XSERIALIZATIONMETHODBINARY_TYPEELEMENT expected)
* @brief      Read type
* @ingroup    XUTILS
* 
* @param[in]  buffer : Pointer to buffer.
* @param[in]  position : Position value.
* @param[in]  expected : Expected value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool XSERIALIZATIONMETHODBINARY_ReadType(XBUFFER* buffer, XDWORD& position, XSERIALIZATIONMETHODBINARY_TYPEELEMENT expected)
{
  XBYTE type = 0;
  if(!buffer || !buffer->Get(type, position) || type != (XBYTE)expected) return false;
  position++;
  return true;
}


template <class T>
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool XSERIALIZATIONMETHODBINARY_ReadValue(XBUFFER* buffer, XDWORD& position, T& value)
* @brief      Read value
* @ingroup    XUTILS
* 
* @param[in]  buffer : Pointer to buffer.
* @param[in]  position : Position value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool XSERIALIZATIONMETHODBINARY_ReadValue(XBUFFER* buffer, XDWORD& position, T& value)
{
  if(!buffer || position > buffer->GetSize() || (buffer->GetSize()-position) < sizeof(T) ||
     !buffer->Get(value, position)) return false;
  position += sizeof(T);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool XSERIALIZATIONMETHODBINARY_AddData(XBUFFER* target, XSERIALIZATIONMETHODBINARY_TYPEELEMENT type, XBUFFER& data)
* @brief      Add data
* @ingroup    XUTILS
* 
* @param[out] target : Pointer to target.
* @param[in]  type : Type value.
* @param[in]  data : Data value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool XSERIALIZATIONMETHODBINARY_AddData(XBUFFER* target, XSERIALIZATIONMETHODBINARY_TYPEELEMENT type, XBUFFER& data)
{
  if(!target) return false;
  return target->Add((XBYTE)type) && target->Add((XDWORD)data.GetSize()) &&
         (data.IsEmpty() || target->Add(data));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool XSERIALIZATIONMETHODBINARY_ReadData(XBUFFER* source, XDWORD& position, XSERIALIZATIONMETHODBINARY_TYPEELEMENT type, XBUFFER& data)
* @brief      Read data
* @ingroup    XUTILS
* 
* @param[in]  source : Pointer to source.
* @param[in]  position : Position value.
* @param[in]  type : Type value.
* @param[in]  data : Data value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool XSERIALIZATIONMETHODBINARY_ReadData(XBUFFER* source, XDWORD& position, XSERIALIZATIONMETHODBINARY_TYPEELEMENT type, XBUFFER& data)
{
  XDWORD size = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(source, position, type) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(source, position, size) ||
     position > source->GetSize() || (source->GetSize()-position) < size) return false;
  data.Delete();
  if(size && !data.Add(source->Get()+position, size)) return false;
  position += size;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSERIALIZATIONMETHODBINARY::XSERIALIZATIONMETHODBINARY()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSERIALIZATIONMETHODBINARY::XSERIALIZATIONMETHODBINARY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSERIALIZATIONMETHODBINARY::~XSERIALIZATIONMETHODBINARY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSERIALIZATIONMETHODBINARY::~XSERIALIZATIONMETHODBINARY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(bool var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(bool var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }
 
  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_BOOLEAN) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(char var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(char var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_CHAR) && bufferdata->Add((XBYTE)var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(int var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(int var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }
  
  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_INTEGER) && bufferdata->Add((XDWORD)var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(float var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(float var, XCHAR* name)  
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_FLOAT) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(double var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(double var, XCHAR* name) 
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_DOUBLE) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(long var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(long var, XCHAR* name) 
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_LONG) && bufferdata->Add((XQWORD)(long long)var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(long long var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(long long var, XCHAR* name) 
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_LONGLONG) && bufferdata->Add((XQWORD)var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XBYTE var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XBYTE var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }
 
  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XBYTE) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XWORD var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XWORD var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XWORD) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XDWORD var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XDWORD var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }
    
  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XDWORD) && bufferdata->Add(var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XQWORD var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XQWORD var, XCHAR* name)
{
  if(!bufferdata) 
    {
      return false;
    }

  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XQWORD) && bufferdata->Add(var);
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XSTRING* var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XSTRING* var, XCHAR* name)
{
  if(!bufferdata || !var) 
    {
      return false;
    }

  XBUFFER buffer;
  if(!var->IsEmpty() && !var->ConvertToUTF8(buffer, false)) return false;
  return XSERIALIZATIONMETHODBINARY_AddData(bufferdata, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XSTRING, buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XBUFFER* var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XBUFFER* var, XCHAR* name)
{
  if(!bufferdata || !var) return false;
  return XSERIALIZATIONMETHODBINARY_AddData(bufferdata, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XBUFFER, (*var));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Add(XVARIANT* var, XCHAR* name)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Add(XVARIANT* var, XCHAR* name)
{ 
  if(!bufferdata || !var) return false;
  XSTRING text;
  XBUFFER encoded;
  if(!var->ToString(text) || (!text.IsEmpty() && !text.ConvertToUTF8(encoded, false))) return false;
  return bufferdata->Add((XBYTE)XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XVARIANT) &&
         bufferdata->Add((XDWORD)var->GetType()) && bufferdata->Add((XDWORD)encoded.GetSize()) &&
         (encoded.IsEmpty() || bufferdata->Add(encoded));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::AddStruct(XCHAR* name, bool open)
* @brief      Add struct
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  open : Open value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::AddStruct(XCHAR* name, bool open)
{
  if(!bufferdata) 
    {
      return false;
    }
  
  return true;
}
   

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::AddArray(XDWORD nelements, XCHAR* name, bool open)
* @brief      Add array
* @ingroup    XUTILS
* 
* @param[in]  nelements : Nelements value.
* @param[in]  name : Name to use.
* @param[in]  open : Open value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::AddArray(XDWORD nelements, XCHAR* name, bool open)
{
  if(!bufferdata) 
    {
      return false;
    }
  
  if(!open) return true;
  return bufferdata->Add((XWORD)XSERIALIZATIONMETHOD_ARRAY_ID) && bufferdata->Add(nelements);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(bool var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(bool& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_BOOLEAN) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(char var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(char& var, XCHAR* name)
{
  XBYTE value = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_CHAR) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, value)) return false;
  var = (char)value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(int var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(int& var, XCHAR* name)
{
  XDWORD value = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_INTEGER) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, value)) return false;
  var = (int)value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(float var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(float& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_FLOAT) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(double var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(double& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_DOUBLE) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(long var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(long& var, XCHAR* name)
{
  XQWORD value = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_LONG) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, value)) return false;
  var = (long)(long long)value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(long long var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(long long& var, XCHAR* name)
{
  XQWORD value = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_LONGLONG) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, value)) return false;
  var = (long long)value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XBYTE var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XBYTE& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XBYTE) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XWORD var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XWORD& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XWORD) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XDWORD var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XDWORD& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XDWORD) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XQWORD var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XQWORD& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XQWORD) &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, var);
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XSTRING* var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XSTRING& var, XCHAR* name)
{
  XBUFFER data;
  if(!XSERIALIZATIONMETHODBINARY_ReadData(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XSTRING, data)) return false;
  var.Empty();
  return data.IsEmpty() || var.ConvertFromUTF8(data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XBUFFER* var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XBUFFER& var, XCHAR* name)
{
  return XSERIALIZATIONMETHODBINARY_ReadData(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XBUFFER, var);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::Extract(XVARIANT* var, XCHAR* name)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  var : Var pointer to use.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::Extract(XVARIANT& var, XCHAR* name)
{
  XDWORD type = 0;
  XDWORD size = 0;
  if(!XSERIALIZATIONMETHODBINARY_ReadType(bufferdata, readposition, XSERIALIZATIONMETHODBINARY_TYPEELEMENT_XVARIANT) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, type) ||
     !XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, size) ||
     type >= XVARIANT_TYPE_UNKNOWN || readposition > bufferdata->GetSize() ||
     (bufferdata->GetSize()-readposition) < size) return false;
  XSTRING text;
  if(size && !text.ConvertFromUTF8(bufferdata->Get()+readposition, size)) return false;
  readposition += size;
  if(type == XVARIANT_TYPE_NULL) return var.Set(XVARIANT_TYPE_NULL);
  return var.FromString(text, (XVARIANT_TYPE)type);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::ExtractStruct(XCHAR* name)
* @brief      Extract struct
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::ExtractStruct(XCHAR* name)
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::ExtractArray(XDWORD nelements, XCHAR* name)
* @brief      Extract array
* @ingroup    XUTILS
* 
* @param[in]  nelements : Nelements value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::ExtractArray(XDWORD nelements, XCHAR* name)
{
  XWORD marker = 0;
  XDWORD stored = 0;
  return XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, marker) &&
         marker == XSERIALIZATIONMETHOD_ARRAY_ID &&
         XSERIALIZATIONMETHODBINARY_ReadValue(bufferdata, readposition, stored) && stored == nelements;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSERIALIZATIONMETHODBINARY::ExtractArrayElement(XDWORD index, XCHAR* name, bool open)
* @brief      Extract array element
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  name : Pointer to name.
* @param[in]  open : Open value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSERIALIZATIONMETHODBINARY::ExtractArrayElement(XDWORD index, XCHAR* name, bool open)
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XSERIALIZATIONMETHODBINARY::GetBufferData()
* @brief      Get buffer data
* @ingroup    XUTILS
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XSERIALIZATIONMETHODBINARY::GetBufferData()
{
  return bufferdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSERIALIZATIONMETHODBINARY::SetBufferData(XBUFFER* bufferdata)
* @brief      Set buffer data
* @ingroup    XUTILS
* 
* @param[in]  bufferdata : Bufferdata pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSERIALIZATIONMETHODBINARY::SetBufferData(XBUFFER* bufferdata)
{
  this->bufferdata = bufferdata;
  readposition = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSERIALIZATIONMETHODBINARY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSERIALIZATIONMETHODBINARY::Clean()
{
  bufferdata = NULL; 
  readposition = 0;
}

