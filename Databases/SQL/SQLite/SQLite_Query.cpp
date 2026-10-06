/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SQLite_Query.cpp
* 
* @class      SQLITE_QUERY
* @brief      DataBase SQL SQLite Query class
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



#if defined(DB_SQL_ACTIVE) && defined(DB_SQLITE_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "SQLite_Query.h"

#include "XFactory.h"
#include "XBuffer.h"

#include "SQLite_Database.h"
#include "SQLite_Connection.h"
#include "SQLite_Result.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SQLITE_QUERY::~SQLITE_QUERY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
SQLITE_QUERY::~SQLITE_QUERY()
{
  if(result)
    {
      delete(result);
      result = NULL;
    }

  UnbindAll();

  ClearBuffers();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::Execute()
* @brief      Execute
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::Execute()
{
  sqlite3* db = static_cast<SQLITE_DATABASE*>(database)->sqlite3database;
  if(!db)
    {
      database->Error(_L("database not initialized"));
      return false;
    }

  XBUFFER* buffer = GEN_NEW XBUFFER();
  if(!buffer) return false;

  GetValue()->ConvertToUTF8(*buffer);

  buffers.Add(buffer);

  //----------------------- check statement
  XDWORD size = buffer->GetSize();
  if(!size)
    {
      DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(_L("void statement"));

      database->ClearPreviousErrors();

      database->GetErrorList()->Add(error);

      if (database->IsTransactionStarted()) database->Rollback();

      return false;
    }

  int rc = Exec((char*)buffer->Get(), size);

  busyflag = (rc==2)?true:false;

  if(rc!=0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::ClearCursor()
* @brief      Clear cursor
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::ClearCursor()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::Bind(XDWORD ID, int integer)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  integer : Integer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::Bind(XDWORD ID, int integer)
{
  return DB_SQL_QUERY::Bind(ID+1, integer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::Bind(XDWORD ID, XCHAR* string)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::Bind(XDWORD ID, XCHAR* string)
{
  return DB_SQL_QUERY::Bind(ID+1, string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::Bind(XDWORD ID, float decimal)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  decimal : Decimal value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::Bind(XDWORD ID, float decimal)
{
  return DB_SQL_QUERY::Bind(ID+1, decimal );
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::Bind(XDWORD ID, XDATETIME& datetime)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  datetime : Datetime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::Bind(XDWORD ID, XDATETIME& datetime)
{
  return DB_SQL_QUERY::Bind(ID+1, datetime);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::IsWellConstructedQuery()
* @brief      Is well constructed query
* @ingroup    DATABASE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::IsWellConstructedQuery()
{
  //pending checks
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SQLITE_QUERY::Exec(const char* sql, XDWORD size)
* @brief      Exec
* @ingroup    DATABASE
* 
* @param[in]  sql : Sql value.
* @param[in]  size : Size value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SQLITE_QUERY::Exec(const char* sql, XDWORD size)
{
  sqlite3* db = static_cast<SQLITE_DATABASE*>(database)->sqlite3database;
  if(!db) return 1;

  ppstmt = NULL;

  // prepare current statement
  int preparereturncode = sqlite3_prepare_v2(db, sql, size, &ppstmt, 0);
  if(preparereturncode)
    {
      DB_SQL_ERROR* error=GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
      if(!error) return 1;

      error->description.Set(sqlite3_errmsg(db));

      database->ClearPreviousErrors();

      database->GetErrorList()->Add(error);

      if(database->IsTransactionStarted())
        {
          database->Rollback();
        }

      return 1;
    }
   else
    {
      // clear old result sets
      if(result)
        {
          UnbindAll();
          GEN_DELETE result;
          result = NULL;
        }

      // statement ready, bind parameters
      BindParametersToQuery();

      int rc = sqlite3_step(ppstmt);
      switch(rc)
        {
          case SQLITE_LOCKED  : { DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_CONNECTION_ERROR);
                                  if(!error) return 1;

                                  error->description.Set(sqlite3_errmsg(db));
                                  database->GetErrorList()->Add(error);

                                  sqlite3_finalize(ppstmt);
                                  return 2;
                                }
                                break;

          case SQLITE_PERM    : { DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_CONNECTION_ERROR);
                                  if(!error) return 1;

                                  error->description.Set(sqlite3_errmsg(db));

                                  database->GetErrorList()->Add(error);

                                  sqlite3_finalize(ppstmt);

                                  return 2;
                                }
                                break;

          case SQLITE_BUSY    : { sqlite3_finalize(ppstmt);
                                  return 2;
                                }
                                break;

          case SQLITE_ERROR   : // statment failed
                                { DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
                                  if (!error) return 1;

                                  error->description.Set(sqlite3_errmsg(db));

                                  database->GetErrorList()->Add(error);
                                  sqlite3_finalize(ppstmt);

                                  if (database->IsTransactionStarted()) database->Rollback();

                                  ClearBuffers();

                                  return 1;
                                }
                                break;

          case SQLITE_DONE    : { sqlite3_finalize(ppstmt);
                                  return 0;
                                }
                                break;


          case SQLITE_ROW     : // multirow statement (select)
                                { int ncount = sqlite3_column_count(ppstmt);
                                  if(!result)    // prepare resultset
                                    {
                                      result = ConstructResult();
                                      if(!result)
                                        {
                                          DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_MEMORY_ERROR);
                                          if(!error) return 1;

                                          error->description.Set(_L("not enought memory for result"));

                                          database->ClearPreviousErrors();

                                          database->GetErrorList()->Add(error);
                                          sqlite3_finalize(ppstmt);

                                          if (database->IsTransactionStarted())     database->Rollback();

                                          ClearBuffers();

                                          return 1;
                                        }
                                    }

                                  return 0;
                                }
                                break;
        }

      return 1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_QUERY::BindParametersToQuery()
* @brief      Bind parameters to query
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_QUERY::BindParametersToQuery()
{
  for(XDWORD e=0; e<statementbindings.GetSize(); e++)
    {
      XDWORD          statementindex  = statementbindings.GetKey(e);
      DB_SQL_VARIANT* variant         = statementbindings.GetElement(e);

      switch((DB_SQL_VARIANT_TYPE)variant->GetType())
        {
          case DB_SQL_VARIANT_TYPE_INTEGER    : { int rc = sqlite3_bind_int(ppstmt, statementindex, (int)*variant);
                                                  switch(rc)
                                                    {
                                                      case SQLITE_OK      : break;
                                                      case SQLITE_TOOBIG  : return false;
                                                      case SQLITE_NOMEM   : return false;
                                                              default     : break;
                                                    }
                                                }
                                                break;

          case DB_SQL_VARIANT_TYPE_DATETIME   : { XSTRING   string;
                                                  XDATETIME datetime    = (*variant);

                                                  datetime.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, string);

                                                  XBUFFER* variantdatabuffer = GEN_NEW XBUFFER();
                                                  if(!variantdatabuffer) break;

                                                  string.ConvertToUTF8(*variantdatabuffer);
                                                  buffers.Add(variantdatabuffer);

                                                  XDWORD size = variantdatabuffer->GetSize();
                                                  int rc = sqlite3_bind_text(ppstmt, statementindex, (char*)variantdatabuffer->Get(), -1, 0);
                                                  switch(rc)
                                                    {
                                                      case SQLITE_OK      : break;
                                                      case SQLITE_TOOBIG  : return false;
                                                      case SQLITE_NOMEM   : return false;
                                                              default     : break;
                                                    }

                                                }
                                                break;

          case DB_SQL_VARIANT_TYPE_STRING     : { XBUFFER* variantdatabuffer = GEN_NEW XBUFFER();
                                                  if(!variantdatabuffer) break;

                                                  XSTRING string;

                                                  string.Set((XCHAR*)*variant);
                                                  string.ConvertToUTF8(*variantdatabuffer);

                                                  buffers.Add(variantdatabuffer);
                                                  XDWORD size = variantdatabuffer->GetSize();

                                                  int rc = sqlite3_bind_text(ppstmt, statementindex, (char*)variantdatabuffer->Get(), -1, 0);
                                                  switch(rc)
                                                    {
                                                      case SQLITE_OK      : break;
                                                      case SQLITE_TOOBIG  : return false;
                                                      case SQLITE_NOMEM   : return false;
                                                              default     : break;
                                                    }

                                                }
                                                break;

          case DB_SQL_VARIANT_TYPE_FLOAT      : { float data = (*variant);

                                                  int rc = sqlite3_bind_double(ppstmt, statementindex, (double)data);
                                                  switch(rc)
                                                    {
                                                      case SQLITE_OK      : break;
                                                      case SQLITE_TOOBIG  : return false;
                                                      case SQLITE_NOMEM   : return false;
                                                              default     : break;
                                                    }
                                                }
                                                break;

                                    default   : break;

        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_RESULT* SQLITE_QUERY::ConstructResult()
* @brief      Construct result
* @ingroup    DATABASE
* 
* @return     DB_SQL_RESULT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_RESULT* SQLITE_QUERY::ConstructResult()
{
  SQLITE_RESULT* set = GEN_NEW SQLITE_RESULT();
  if(set)
    {
      set->query      = this;
      set->lastresult = SQLITE_ROW;
    }

  return set;
}




#endif

