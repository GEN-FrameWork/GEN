/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XVariant.cpp
* 
* @class      XVARIANT
* @brief      eXtended Utils Variant class
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

#include "XVariant.h"

#include "XFactory.h"
#include "XTrace.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XLanguage_ISO_639_3.h"




/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- INTERNAL FUNCTIONS --------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         template<class T> static T XVARIANT_GetNumericValue(XVARIANT_TYPE type, const void* data)
* @brief      Get the stored numeric value converted to T, taking into account the real stored type.
*             (FIX: the old cast operators did a blind *(T*)data reinterpret, reading out of bounds when the
*             stored type was smaller than T -> e.g. XQWORD extracted from a JSON number stored as int).
* @ingroup    XUTILS
*
* @param[in]  type : Real type stored in the variant.
* @param[in]  data : Pointer to the stored data.
*
* @return     T : Requested value (0 if data is NULL or the type is not numeric).
*
* --------------------------------------------------------------------------------------------------------------------*/
template<class T>
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static T XVARIANT_GetNumericValue(XVARIANT_TYPE type, const void* data)
* @brief      Get numeric value
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* @param[in]  data : Pointer to data.
* 
* @return     T : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static T XVARIANT_GetNumericValue(XVARIANT_TYPE type, const void* data)
{
  if(!data)
    {
      return (T)0;
    }

  switch(type)
    {
      case XVARIANT_TYPE_BOOLEAN        : return (T)(*(bool*)data);
      case XVARIANT_TYPE_SHORT          : return (T)(*(short*)data);
      case XVARIANT_TYPE_WORD           : return (T)(*(XWORD*)data);
      case XVARIANT_TYPE_INTEGER        : return (T)(*(int*)data);
      case XVARIANT_TYPE_DWORD          : return (T)(*(XDWORD*)data);
      case XVARIANT_TYPE_DOUBLEINTEGER  : return (T)(*(long long*)data);
      case XVARIANT_TYPE_QWORD          : return (T)(*(XQWORD*)data);
      case XVARIANT_TYPE_FLOAT          : return (T)(*(float*)data);
      case XVARIANT_TYPE_DOUBLE         : return (T)(*(double*)data);
      case XVARIANT_TYPE_CHAR           : return (T)(*(char*)data);
      case XVARIANT_TYPE_XCHAR          : return (T)(*(XCHAR*)data);
                              default   : break;
    }

  return (T)0;
}



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const bool value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const bool value)
{
  Clean();

  (*this) = value;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const short value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const short value) 
{
  Clean();
  
  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XWORD value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XWORD value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const int value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const int value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XDWORD value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XDWORD value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const long long value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const long long value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XQWORD value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XQWORD value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const float value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const float value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const double value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const double value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const char value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const char value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const char* value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const char* value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XCHAR value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XCHAR value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XCHAR* value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XCHAR* value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XSTRING& value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XSTRING& value)
{
  Clean();

  (*this) = value; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XDATETIME& value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XDATETIME& value)
{
  Clean();

  (*this) = value;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XBUFFER& value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XBUFFER& value)

{
  Clean();   
    
  (*this) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const void* value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const void* value)
{
  Clean();
  
  (*this) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::XVARIANT(const XVARIANT& value)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::XVARIANT(const XVARIANT& value)

{
  Clean();

  (*this) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::~XVARIANT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::~XVARIANT()
{
  if(data) 
    {
      Destroy();
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const bool value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const bool value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_BOOLEAN, bool, value)

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const short value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const short value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_SHORT, short, value)

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XWORD value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XWORD value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_WORD, XWORD, value)

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const int value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const int value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_INTEGER, int, value)

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XDWORD value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XDWORD value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_DWORD, XDWORD, value)

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const long long value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const long long value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_DOUBLEINTEGER, long long, value)  
  
  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XQWORD value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XQWORD value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_QWORD, XQWORD, value)  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const float value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const float value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_FLOAT, float, value)  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const double value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const double value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_DOUBLE, double, value)  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const char value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const char value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_CHAR, char, value)  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const char* value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const char* value)
{
  GetDataFromString((char*)value); 

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XCHAR value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XCHAR value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_XCHAR, XCHAR, value)  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XCHAR* value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XCHAR* value)
{
  GetDataFromString((XCHAR*)value); 

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XSTRING& value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XSTRING& value)
{
  GetDataFromString(value.Get()); 

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XDATETIME& value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XDATETIME& value)
{  
  if(data) 
    {
      Destroy(); 
    }

  type  = XVARIANT_TYPE_DATETIME; 
  size  = sizeof(XDATETIME); 

  data = (void*)GEN_NEW XDATETIME;
  if(data) 
    {
      static_cast<XDATETIME*>(data)->CopyFrom((XDATETIME&)value);
    }

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XBUFFER& value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XBUFFER& value)
{  
  if(data) 
    { 
      Destroy();
    }
  
  type = XVARIANT_TYPE_BUFFER;
  size = sizeof(XBUFFER);

  data = GEN_NEW XBUFFER();  
  if(data)
    {
      static_cast<XBUFFER*>(data)->CopyFrom((XBUFFER&)value);
    }  

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const void* value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const void* value)
{
  XVARIANT_CREATE(XVARIANT_TYPE_POINTER, const void*, value)  
  
  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XVARIANT& XVARIANT::operator = (const XVARIANT& value)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     const XVARIANT& XVARIANT:: : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XVARIANT& value)
{
  GetDataVariant((XVARIANT&)value);

  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator bool() const
* @brief      operator bool
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator bool() const
{
  return XVARIANT_GetNumericValue<bool>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator short() const
* @brief      operator short
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator short() const
{
  return XVARIANT_GetNumericValue<short>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XWORD() const
* @brief      operator XWORD
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XWORD() const
{
  return XVARIANT_GetNumericValue<XWORD>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator int() const
* @brief      operator int
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator int() const
{
  return XVARIANT_GetNumericValue<int>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XDWORD() const
* @brief      operator XDWORD
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XDWORD() const
{
  return XVARIANT_GetNumericValue<XDWORD>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator long long() const
* @brief      operator long long
* @ingroup    XUTILS
* 
* @return     XVARIANT::operator long : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator long long() const
{
  return XVARIANT_GetNumericValue<long long>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XQWORD() const
* @brief      operator XQWORD
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XQWORD() const
{
  return XVARIANT_GetNumericValue<XQWORD>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator float() const
* @brief      operator float
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator float() const
{
  return XVARIANT_GetNumericValue<float>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator double() const
* @brief      operator double
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator double() const
{
  return XVARIANT_GetNumericValue<double>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator char() const
* @brief      operator char
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator char() const
{
  return XVARIANT_GetNumericValue<char>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator char*() const
* @brief      operator char*
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator char*() const 
{
  return (char*)data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XCHAR() const
* @brief      operator XCHAR
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XCHAR() const
{
  return XVARIANT_GetNumericValue<XCHAR>(type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XCHAR*() const
* @brief      operator XCHAR*
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XCHAR*() const 
{
  if(static_cast<XSTRING*>(data)) 
    {
      return static_cast<XSTRING*>(data)->Get();
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XSTRING() const
* @brief      operator XSTRING
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XSTRING() const 
{
  XSTRING string;
  
  if(static_cast<XSTRING*>(data))
    {
      string = static_cast<XSTRING*>(data)->Get();
    }

  return string;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XDATETIME() const
* @brief      operator XDATETIME
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XDATETIME() const 
{   
  XDATETIME datetime;
  
  if(static_cast<XDATETIME*>(data))
    {
      datetime.CopyFrom(*(static_cast<XDATETIME*>(data)));
    }

  return datetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator XBUFFER() const
* @brief      operator XBUFFER
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator XBUFFER() const 
{
  XBUFFER buffer;
  
  if(static_cast<XBUFFER*>(data))
    {
      buffer.CopyFrom(*(static_cast<XBUFFER*>(data)));
    }

  return buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT::operator void*() const
* @brief      operator void*
* @ingroup    XUTILS
* 
* @return     XVARIANT:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT::operator void*() const 
{
  if(!data) 
    {
      return NULL;
    }

  void* value = 0;

  memcpy((void*)&value, (XBYTE*)data, size); 

  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT_TYPE XVARIANT::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XVARIANT_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT_TYPE XVARIANT::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::GetType(XSTRING& typestr)
* @brief      Get type
* @ingroup    XUTILS
* 
* @param[in]  typestr : Typestr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::GetType(XSTRING& typestr)
{
  switch(type)
    {
      case XVARIANT_TYPE_NULL             : typestr = __L("null");            break;      
      case XVARIANT_TYPE_BOOLEAN          : typestr = __L("boolean");         break;    
      case XVARIANT_TYPE_SHORT            : typestr = __L("short");           break;
      case XVARIANT_TYPE_WORD             : typestr = __L("word");            break;
      case XVARIANT_TYPE_INTEGER          : typestr = __L("integer");         break;
      case XVARIANT_TYPE_DWORD            : typestr = __L("double word");     break;
      case XVARIANT_TYPE_DOUBLEINTEGER    : typestr = __L("double integer");  break;      
      case XVARIANT_TYPE_QWORD            : typestr = __L("quad word");       break;      
      case XVARIANT_TYPE_CHAR             : typestr = __L("char");            break;
      case XVARIANT_TYPE_FLOAT            : typestr = __L("float");           break;
      case XVARIANT_TYPE_DOUBLE           : typestr = __L("double");          break;
      case XVARIANT_TYPE_STRING           : typestr = __L("string");          break;
      case XVARIANT_TYPE_TIME             : typestr = __L("time");            break;
      case XVARIANT_TYPE_DATE             : typestr = __L("date");            break;
      case XVARIANT_TYPE_DATETIME         : typestr = __L("date/time");       break;
      case XVARIANT_TYPE_BUFFER           : typestr = __L("buffer");          break;      
                        default           : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XVARIANT::SetType(XVARIANT_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XVARIANT::SetType(XVARIANT_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XVARIANT::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XVARIANT::GetSize()
{
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XVARIANT::GetData()
* @brief      Get data
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XVARIANT::GetData()
{
  return (void*)data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::Set(XVARIANT_TYPE type, void* data, XDWORD size)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::Set(XVARIANT_TYPE type, void* data, XDWORD size)
{
  if(this->data)  Destroy();

  if(type == XVARIANT_TYPE_NULL)
    { 
      return true;
    }

  if(size)
    {
      this->data = GEN_NEW XBYTE[size];
      if(!this->data)
        {
          return false;
        }

      this->type    = type;
    }

  this->size = size;

  if(data && size)
    {
      memcpy(this->data, &data, size);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::GetDataFromString(char* string)
* @brief      Get data from string
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::GetDataFromString(char* string)
{
  if(data) 
    {
      Destroy();
    }

  data = GEN_NEW XSTRING();
  if(!data) return false;

  type = XVARIANT_TYPE_STRING;
  size = sizeof(XSTRING);

  if(static_cast<XSTRING*>(data))
    {
      return static_cast<XSTRING*>(data)->Set(string);
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::GetDataFromString(XCHAR* string)
* @brief      Get data from string
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::GetDataFromString(XCHAR* string)
{
  if(data) 
    { 
      Destroy();
    }

  data = GEN_NEW XSTRING();
  if(!data) return false;

  type = XVARIANT_TYPE_STRING;
  size = sizeof(XSTRING);

  if(static_cast<XSTRING*>(data))
    {
      return static_cast<XSTRING*>(data)->Set(string);
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::GetDataVariant(XVARIANT& value)
* @brief      Get data variant
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::GetDataVariant(XVARIANT& value)
{
  if(&value == this) 
    {
      return false;
    }

  if(value.data == this->data) 
    {
      return false;
    }

  if(data) Destroy();

  size    = value.size;
  type    = value.type;

  switch(type)
    {
      case XVARIANT_TYPE_NULL           : NULL;                                    break;                                     
      case XVARIANT_TYPE_BOOLEAN        : (*this) = (bool)(XVARIANT&)value;        break;

      case XVARIANT_TYPE_SHORT          : (*this) = (int)(XVARIANT&)value;         break;
      case XVARIANT_TYPE_WORD           : (*this) = (XWORD)(XVARIANT&)value;       break;
      case XVARIANT_TYPE_INTEGER        : (*this) = (int)(XVARIANT&)value;         break;
      case XVARIANT_TYPE_DWORD          : (*this) = (XDWORD)(XVARIANT&)value;      break;
      case XVARIANT_TYPE_DOUBLEINTEGER  : (*this) = (long long)(XVARIANT&)value;   break;      
      case XVARIANT_TYPE_QWORD          : (*this) = (XQWORD)(XVARIANT&)value;      break;      
      
      case XVARIANT_TYPE_FLOAT          : (*this) = (float)(XVARIANT&)value;       break;
      case XVARIANT_TYPE_DOUBLE         : (*this) = (double)(XVARIANT&)value;      break;

      case XVARIANT_TYPE_CHAR           : (*this) = (char)(XVARIANT&)value;        break;
      case XVARIANT_TYPE_XCHAR          : (*this) = (XCHAR)(XVARIANT&)value;       break;
      case XVARIANT_TYPE_STRING         : (*this) = (XCHAR*)(XVARIANT&)value;      break;

      case XVARIANT_TYPE_TIME           :     
      case XVARIANT_TYPE_DATE           :  
      case XVARIANT_TYPE_DATETIME       : (*this) = (const XDATETIME*)&value;
                                          type    = value.type;                                          
                                          break;

      case XVARIANT_TYPE_BUFFER         : (*this) = (const XBUFFER*)&value;
                                          type    = value.type;                                          
                                          break;        

                              default   : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::ToString(XSTRING& to)
* @brief      To string
* @ingroup    XUTILS
* 
* @param[in]  to : To value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::ToString(XSTRING& to)
{
  switch(type)
    {
      case XVARIANT_TYPE_NULL             : to = __L("NULL");                                     break;      
      case XVARIANT_TYPE_BOOLEAN          : to.ConvertFromBoolean((*(bool*)this->data), (XSTRINGBOOLEANMODE_COMPUTER | XSTRINGBOOLEANMODE_LOWERCASE));          
                                            break;

      case XVARIANT_TYPE_SHORT            : to.ConvertFromShort(*(short*)this->data);             break;
      case XVARIANT_TYPE_WORD             : to.ConvertFromWord(*(XWORD*)this->data);              break;
      case XVARIANT_TYPE_INTEGER          : to.ConvertFromInt(*(int*)this->data);                 break;
      case XVARIANT_TYPE_DWORD            : to.ConvertFromDWord(*(XDWORD*)this->data);            break;
      case XVARIANT_TYPE_DOUBLEINTEGER    : to.ConvertFromLongLong(*(long long*)this->data);      break;      
      case XVARIANT_TYPE_QWORD            : to.ConvertFromQWord(*(XQWORD*)this->data);            break;      
      
      case XVARIANT_TYPE_FLOAT            : to.ConvertFromFloat(*(float*)this->data);             break;
      case XVARIANT_TYPE_DOUBLE           : to.ConvertFromDouble(*(double*)this->data);           break;      

      case XVARIANT_TYPE_CHAR             : { char value = *(char*)this->data; 
                                              XCHAR value2 = value;  

                                              to.Format(__L("%c"), value2);               
                                            }
                                            break;

      case XVARIANT_TYPE_XCHAR            : { XCHAR value = *(XCHAR*)this->data; 

                                              to.Format(__L("%c"), value);               
                                            }
                                            break;

      case XVARIANT_TYPE_STRING           : to =  ((XSTRING*)this->data)->Get();                                            
                                            break;

      case XVARIANT_TYPE_TIME             : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                              if(datetime) 
                                                {
                                                  datetime->CopyFrom(((XDATETIME*)this->data));      

                                                  datetime->GetDateTimeToStringISO8601(XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS | XDATETIME_FORMAT_TIMEWITHMILLISECONDS | XDATETIME_FORMAT_ISO8601_ADDHOUROFFSET, to);      

                                                  GEN_XFACTORY.DeleteDateTime(datetime);
                                                }
                                            }
                                            break;

      case XVARIANT_TYPE_DATE             : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                              if(datetime) 
                                                {
                                                  datetime->CopyFrom(((XDATETIME*)this->data));      

                                                  datetime->GetDateTimeToStringISO8601(0, to);  

                                                  GEN_XFACTORY.DeleteDateTime(datetime);
                                                }
                                            }
                                            break;
    
      case XVARIANT_TYPE_DATETIME         : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                              if(datetime) 
                                                {
                                                  datetime->CopyFrom(((XDATETIME*)this->data));      

                                                  datetime->GetDateTimeToStringISO8601(XDATETIME_FORMAT_ISO8601_STANDARD | XDATETIME_FORMAT_TIMEWITHSECONDS | XDATETIME_FORMAT_TIMEWITHMILLISECONDS | XDATETIME_FORMAT_ISO8601_ADDHOUROFFSET, to);      

                                                  GEN_XFACTORY.DeleteDateTime(datetime);
                                                }
                                            }
                                            break;

      case XVARIANT_TYPE_BUFFER           : { XBUFFER* buffer = ((XBUFFER*)this->data);
                                              if(buffer)
                                                {
                                                  buffer->ConvertToBase64(to);                                                 
                                                }                                                                                                                         
                                            }
                                            break;
                                                                
      case XVARIANT_TYPE_POINTER          : to.Format(__L("[%08X]"), *(XBYTE*)this->data);                                                                        
                                            break;      

                        default           : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::FromString(XSTRING& from, XVARIANT_TYPE from_type)
* @brief      From string
* @ingroup    XUTILS
* 
* @param[in]  from : From value.
* @param[in]  from_type : From type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::FromString(XSTRING& from, XVARIANT_TYPE from_type)
{
  XVARIANT_TYPE _type = from_type;

  if(_type == XVARIANT_TYPE_UNKNOWN)
    {
      if(!from.Compare(__L("null"), true))
        {
          _type = XVARIANT_TYPE_NULL;
        }
       else
        {
          if((!from.Compare(XT_L(XTRANSLATION_GEN_ID_TRUE)  , true)) || 
             (!from.Compare(XT_L(XTRANSLATION_GEN_ID_FALSE) , true)) || 
             (!from.Compare(XT_L(XTRANSLATION_GEN_ID_YES)   , true)) || 
             (!from.Compare(XT_L(XTRANSLATION_GEN_ID_NO)    , true))) 
            {
              _type = XVARIANT_TYPE_BOOLEAN;
            } 
           else
            {
              int ndecimals = 0;

              if(from.IsDecimalNumber(&ndecimals))
                {
                  if(ndecimals <= 6)
                    {
                      _type = XVARIANT_TYPE_FLOAT;
                    }
                   else
                    { 
                      _type = XVARIANT_TYPE_DOUBLE;
                    }
                }
               else
                { 
                  if(from.IsNumber())
                    {
                      _type = XVARIANT_TYPE_DOUBLEINTEGER;
                    }
                }
            } 
        }

      if(_type == XVARIANT_TYPE_UNKNOWN)
        {
          _type = XVARIANT_TYPE_STRING;
        }
    }                                         

  if(!from.IsEmpty())
    {
      type = _type;

      switch(_type)
        {
          case XVARIANT_TYPE_NULL             : break;      
      
          case XVARIANT_TYPE_BOOLEAN          : (*this) = from.ConvertToBoolean();                                                                      
                                                size = sizeof(bool);    
                                                break;
      
          case XVARIANT_TYPE_SHORT            : (*this) = (short)from.ConvertToInt();
                                                size = sizeof(short);    
                                                break;

          case XVARIANT_TYPE_WORD             : (*this) = (XWORD)from.ConvertToInt();                   
                                                size = sizeof(XWORD);    
                                                break;

          case XVARIANT_TYPE_INTEGER          : (*this) = from.ConvertToInt();                          
                                                size = sizeof(int);    
                                                break;
                                                
          case XVARIANT_TYPE_DWORD            : (*this) = from.ConvertToDWord();                        
                                                size = sizeof(XDWORD);    
                                                break;
                                                
          case XVARIANT_TYPE_DOUBLEINTEGER    : (*this) = from.ConvertToLongLong();                     
                                                size = sizeof(long long);    
                                                break;
                                                
          case XVARIANT_TYPE_QWORD            : (*this) = from.ConvertToQWord();                                                                        
                                                size = sizeof(XQWORD);                                                    
                                                break;      
          

          case XVARIANT_TYPE_FLOAT            : (*this) = from.ConvertToFloat();                                                                        
                                                size = sizeof(float);                                                   
                                                break;      

          case XVARIANT_TYPE_DOUBLE           : (*this) = from.ConvertToDouble();                       
                                                type = _type;
                                                size = sizeof(double);    
                                                break;


          case XVARIANT_TYPE_CHAR             : (*this) = (char)from.Get()[0];                         
                                                
                                                size = sizeof(char);                                                    
                                                break;

          case XVARIANT_TYPE_XCHAR            : (*this) = (XCHAR)from.Get()[0];                        
                                                size = sizeof(XCHAR);
                                                break;

          case XVARIANT_TYPE_STRING           : { XSTRING* string = GEN_NEW XSTRING();
                                                  if(string)
                                                    {    
                                                      string->Set(from.Get());
                                                      
                                                      if(data)
                                                        {
                                                          Destroy();
                                                        }    

                                                      size = sizeof(XSTRING);
                                                      data = (void*)(string);
                                                    }
                                                }
                                                break;

          case XVARIANT_TYPE_TIME             : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                                  if(datetime) 
                                                    {
                                                      datetime->GetDateTimeFromStringISO8601(from, XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS | XDATETIME_FORMAT_TIMEWITHMILLISECONDS | XDATETIME_FORMAT_ISO8601_ADDHOUROFFSET);                                                                                             
                                                      if(data)
                                                        {
                                                          Destroy();
                                                        }    

                                                      size = sizeof(XDATETIME);
                                                      data = (void*)(datetime);       
                                                    }
                                                }                                            
                                                break;

          case XVARIANT_TYPE_DATE             : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                                  if(datetime) 
                                                    {
                                                      datetime->CopyFrom(((XDATETIME*)this->data));      

                                                      datetime->GetDateTimeFromStringISO8601(from, 0);  

                                                      if(data)
                                                        {
                                                          Destroy();
                                                        }    

                                                      size = sizeof(XDATETIME);
                                                      data = (void*)(datetime);       
                                                    }
                                                }
                                                break;
    
          case XVARIANT_TYPE_DATETIME         : { XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
                                                  if(datetime) 
                                                    {                                                      
                                                      datetime->GetDateTimeFromStringISO8601(from, XDATETIME_FORMAT_ISO8601_STANDARD | XDATETIME_FORMAT_TIMEWITHSECONDS | XDATETIME_FORMAT_TIMEWITHMILLISECONDS | XDATETIME_FORMAT_ISO8601_ADDHOUROFFSET);                                                                                         
                                                      if(data)
                                                        {
                                                          Destroy();
                                                        }    

                                                      size = sizeof(XDATETIME);
                                                      data = (void*)(datetime);       
                                                    }
                                                }
                                                break;

          case XVARIANT_TYPE_BUFFER           : { XBUFFER* buffer = GEN_NEW XBUFFER();
                                                  if(buffer)
                                                    {                                                      
                                                      buffer->ConvertFromBase64(from);

                                                      if(data)
                                                        {
                                                          Destroy();
                                                        }    

                                                      size = sizeof(XBUFFER);
                                                      data = (void*)(buffer);                                                       
                                                    }                                                                                                                                                        
                                                }                                                                                                                                                                     
                                                break;
                                                                
          case XVARIANT_TYPE_POINTER          : from.UnFormat(__L("[%08X]"), this->data);                                                                                                                    
                                                size = sizeof(void*);
                                                break;      

                            default           : return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::IsNull()
* @brief      Is null
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::IsNull()
{
  return (data == NULL)? true : false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XVARIANT::Destroy()
* @brief      Destroy
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XVARIANT::Destroy()
{
  if(data)
    {
      switch(this->type)
        { 
          case XVARIANT_TYPE_NULL           : break;

          case XVARIANT_TYPE_BOOLEAN        : GEN_DELETE (bool*)data;           break;

          case XVARIANT_TYPE_SHORT          : GEN_DELETE (short*)data;          break;
          case XVARIANT_TYPE_WORD           : GEN_DELETE (XWORD*)data;          break;          
          case XVARIANT_TYPE_INTEGER        : GEN_DELETE (int*)data;            break;
          case XVARIANT_TYPE_DWORD          : GEN_DELETE (XDWORD*)data;         break;          
          case XVARIANT_TYPE_DOUBLEINTEGER  : GEN_DELETE (long long*)data;      break;
          case XVARIANT_TYPE_QWORD          : GEN_DELETE (XQWORD*)data;         break;

          case XVARIANT_TYPE_FLOAT          : GEN_DELETE (float*)(data);        break;
          case XVARIANT_TYPE_DOUBLE         : GEN_DELETE (double*)(data);       break;

          case XVARIANT_TYPE_CHAR           : GEN_DELETE (char*)(data);         break;
          case XVARIANT_TYPE_XCHAR          : GEN_DELETE (XCHAR*)(data);        break;          

          case XVARIANT_TYPE_STRING         : { XSTRING* string = (XSTRING*)data;
                                                GEN_DELETE string;      
                                              }
                                              break;      

          case XVARIANT_TYPE_DATE           :
          case XVARIANT_TYPE_TIME           :
          case XVARIANT_TYPE_DATETIME       : { XDATETIME* datetime = (XDATETIME*)data;    
                                                GEN_DELETE datetime;  
                                              }
                                              break;

          case XVARIANT_TYPE_BUFFER         : { XBUFFER* buffer = (XBUFFER*)data;
                                                GEN_DELETE buffer;      
                                              }
                                              break;

          case XVARIANT_TYPE_POINTER        : GEN_DELETE (XBYTE*)(data);         break;           

                                  default   : break;
        }
    }

  type    = XVARIANT_TYPE_NULL;
  size    = 0;
  data    = NULL;

  return true;
}


#ifdef XTRACE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XVARIANT::PrintDebug()
* @brief      Print debug
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XVARIANT::PrintDebug()
{
  XSTRING string;

  ToString(string);
  
  if(!string.IsEmpty())
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_INFO, __L("%s"), string.Get());  
    }
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XVARIANT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XVARIANT::Clean()
{
  type      = XVARIANT_TYPE_NULL;
  size      = 0;
  data      = NULL;
}



