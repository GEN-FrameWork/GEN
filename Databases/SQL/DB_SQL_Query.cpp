/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Query.cpp
* 
* @class      DB_SQL_QUERY
* @brief      DataBase SQL Query Abstract class
* @ingroup    DATABASE
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



#ifdef DB_SQL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DB_SQL_Query.h"

#include "XBuffer.h"

#include "DB_SQL_Result.h"
#include "DB_SQL_Database.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_QUERY::DB_SQL_QUERY(DB_SQL_DATABASE* database)
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  database : Database pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_QUERY::DB_SQL_QUERY(DB_SQL_DATABASE* database)
{
  Clean();

  this->database = database;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_QUERY::~DB_SQL_QUERY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_QUERY::~DB_SQL_QUERY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::ClearCursor()
* @brief      Clear cursor
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::ClearCursor()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::IsWellConstructedQuery()
* @brief      Is well constructed query
* @ingroup    DATABASE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::IsWellConstructedQuery()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_RESULT* DB_SQL_QUERY::GetResult()
* @brief      Get result
* @ingroup    DATABASE
* 
* @return     DB_SQL_RESULT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_RESULT* DB_SQL_QUERY::GetResult()
{
  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_STRING* DB_SQL_QUERY::GetValue()
* @brief      Get value
* @ingroup    DATABASE
* 
* @return     DB_SQL_STRING* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_STRING* DB_SQL_QUERY::GetValue()
{
  return &value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Set(XCHAR* query)
* @brief      Set value
* @ingroup    DATABASE
* 
* @param[in]  query : Query pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Set(XCHAR* query)
{
  UnbindAll();

  value.Set(query);

  return IsWellConstructedQuery();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::UnbindAll()
* @brief      Unbind all
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::UnbindAll()
{
  return (statementbindings.DeleteElementContents() && statementbindings.DeleteAll());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD ID, DB_SQL_VARIANT* variant)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  variant : Variant pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD ID, DB_SQL_VARIANT* variant)
{
  return statementbindings.Set(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD ID, int integer)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  integer : Integer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD ID, int integer)
{
  DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
  if(!variant) return false;

  (*variant) = integer;

  return statementbindings.Add(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD ID, XCHAR* string)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD ID, XCHAR* string)
{
  DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
  if(!variant) return false;

  (*variant) = string;

  return statementbindings.Add(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD ID, float decimal)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  decimal : Decimal value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD ID, float decimal)
{
  DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
  if(!variant) return false;

  (*variant) = decimal;

  return statementbindings.Add(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD ID, char* string)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD ID, char* string)
{
  DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
  if(!variant) return false;

  (*variant) = string;

  return statementbindings.Add(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::Bind(XDWORD id, XDATETIME& datetime)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  id : Identifier to use.
* @param[in]  datetime : Datetime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::Bind(XDWORD id, XDATETIME& datetime)
{
  DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
  if(!variant) return false;

  (*variant) = datetime;

  return statementbindings.Add(id, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_QUERY::ClearBuffers()
* @brief      Clear buffers
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_QUERY::ClearBuffers()
{
  for(XDWORD e=0; e<buffers.GetSize(); e++)
    {
      XBUFFER * buffer = buffers.Get(e);
      GEN_DELETE buffer;
    }

  buffers.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_QUERY::UseCursor(XCHAR* cursorname)
* @brief      Use cursor
* @ingroup    DATABASE
* 
* @param[in]  cursorname : Cursorname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_QUERY::UseCursor(XCHAR* cursorname)
{
  this->cursorname.Set(cursorname);

  hascursor=true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_QUERY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_QUERY::Clean()
{
  database    = NULL;
  result      = NULL;

  statementbindings.DeleteElementContents();
  statementbindings.DeleteAll();

  value.Empty();
}




#endif

