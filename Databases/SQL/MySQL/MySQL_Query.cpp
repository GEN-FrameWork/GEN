/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Query.cpp
* 
* @class      MYSQL_QUERY
* @brief      DataBase SQL MySQL Query class
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



#if defined(DB_SQL_ACTIVE) && defined(DB_MYSQL_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "MySQL_Query.h"

#include "XBuffer.h"
#include "XMemory_Control.h"

#include "MySQL_Connection.h"
#include "MySQL_Database.h"
#include "MySQL_Result.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_QUERY::MYSQL_QUERY(DB_SQL_DATABASE* database): DB_SQL_QUERY(database)
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  database : Database pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_QUERY::MYSQL_QUERY(DB_SQL_DATABASE* database): DB_SQL_QUERY(database)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_QUERY::~MYSQL_QUERY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_QUERY::~MYSQL_QUERY()
{
  UnbindAll();

  ClearBuffers();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::Execute()
* @brief      Execute
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::Execute()
{
  MYSQL_DATABASE* db    = static_cast<MYSQL_DATABASE*>  (database);
  MYSQL*          msql  = static_cast<MYSQL_CONNECTION*>(db->connection)->connection;

  if(value.FindCharacter(__C('*'))!= NOTFOUND && value.Find(__L("COUNT(*)"), true)==XSTRING_NOTFOUND)
    {
      DB_SQL_STRING error;

      database->ClearPreviousErrors();
      error.Set(__L("Wildcard * is discouraged and therefore disabled in this implementation"));

      database->Error(error.Get());

      return false;
    }

  DB_SQL_STRING query;
  query.Set(this->value);

  XDWORD size = query.GetSize();
  if(!size)
    {
      DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      database->ClearPreviousErrors();

      error->description.Set(__L("void statement"));
      database->GetErrorList()->Add(error);

      if(database->IsTransactionStarted())  database->Rollback();

      return false;
   }

  XBUFFER* querystringbuffer = GEN_NEW XBUFFER();
  query.ConvertToUTF8   (*querystringbuffer );
  buffers.Add           (querystringbuffer);

  int RC = Exec(msql, (char*)querystringbuffer->Get());
  if(RC) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::UnbindAll()
* @brief      Unbind all
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::UnbindAll()
{
  if(result)
    {
      GEN_DELETE result;
      result = NULL;
    }

  if(param != NULL)
    {
      for(XDWORD e=0; e<statementbindings.GetSize(); e++)
        {
          if(param[e].buffer)
            {
              param[e].buffer = NULL;
            }
        }

      GEN_DELETE_ARRAY param;

      param = NULL;
    }

  return DB_SQL_QUERY::UnbindAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::Bind(XDWORD ID, DB_SQL_DATETIME& datetime)
* @brief      Bind
* @ingroup    DATABASE
* 
* @param[in]  ID : Identifier to use.
* @param[in]  datetime : Datetime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::Bind(XDWORD ID, DB_SQL_DATETIME& datetime)
{
  MYSQL_VARIANT* variant=GEN_NEW MYSQL_VARIANT();
  if(!variant)  return true;

  (*variant) = datetime;

  return statementbindings.Add(ID, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::Exec(MYSQL* db, char* sql)
* @brief      Exec
* @ingroup    DATABASE
* 
* @param[in]  db : Db pointer to use.
* @param[in]  sql : Sql pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::Exec(MYSQL* db, char* sql)
{
  if(!db)
    {
      database->Error(__L("database not initialized"));
      return true;
    }

  this->mysqldb=db;

  // Check statement

  stmt = mysql_stmt_init(db);

  // Create statement

  if(!stmt)
    {
      DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(mysql_error(db));
      database->GetErrorList()->Add(error);
      if(database->IsTransactionStarted())  database->Rollback();

      return true;
    }
   else
    {
      // Set up statement
      if(mysql_stmt_prepare(stmt, sql, (unsigned long)strlen(sql)) != 0)
        {
          DB_SQL_ERROR* error=GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if(database->IsTransactionStarted())  database->Rollback();

          return true;
       }

      // Statement ready, bind parameters

      BindParametersToQuery();

      // Clear old result sets

      if(result)
        {
          GEN_DELETE result;
          result = NULL;
        }

      // Clear old result sets

      int RC = mysql_stmt_execute(stmt);

      if(RC != DB_SQL_MYSQL_OK)
        {
          DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if (database->IsTransactionStarted()) database->Rollback();

          return true;
        }

      // Prepare result

      if(!result)   // prepare resultset
        {
          result = ConstructResult();

          if(!result)
            {
              DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_MEMORY_ERROR);
              if(!error) return false;

              error->description.Set(__L("not enought memory for result"));
              database->GetErrorList()->Add(error);
              mysql_stmt_free_result(stmt);
              mysql_stmt_close(stmt);

              if(database->IsTransactionStarted())  database->Rollback();

              ClearBuffers();

              return true;
            }
        }

      // Bind results

      BindParametersToResult();

      RC = mysql_stmt_store_result(stmt);
      if(RC!= DB_SQL_MYSQL_OK)
        {
          DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if(database->IsTransactionStarted())      database->Rollback();

          return true;
        }

      // Bind result and store

      mysql_stmt_fetch(stmt);
      if(mysql_stmt_num_rows(stmt) == 0)
        {
          if (mysql_stmt_free_result(stmt)==DB_SQL_MYSQL_OK)
              mysql_stmt_close(stmt);

          if(result)
            {
              delete(result);
              result = NULL;
            }
        }

      ClearBuffers();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_RESULT* MYSQL_QUERY::ConstructResult()
* @brief      Construct result
* @ingroup    DATABASE
* 
* @return     DB_SQL_RESULT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_RESULT* MYSQL_QUERY::ConstructResult()
{
  MYSQL_RESULT* result = GEN_NEW MYSQL_RESULT();
  if(!result) return NULL;

  result->query = this;

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::BindParametersToQuery()
* @brief      Bind parameters to query
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::BindParametersToQuery()
{
  if(!statementbindings.GetSize())  return true;

  // Prepare memory

  if(param)
    {
      GEN_DELETE_ARRAY param;
      param=NULL;
    }

  param = GEN_NEW MYSQL_BIND[statementbindings.GetSize()];
  if(!param)
    {
      database->Error(__L("Not enought memory"));
      return false;
    }

  memset(param, 0, statementbindings.GetSize()*sizeof(MYSQL_BIND));

  static_cast<MYSQL_DATABASE*>(this->database)->isnull=false;

  // Iterate

  for(XDWORD e=0; e<statementbindings.GetSize(); e++)
    {
      XDWORD        statementindex = statementbindings.GetKey(e);
      DB_SQL_VARIANT* variant        = statementbindings.GetElement(e);

      if(variant)
        {
          param[e].is_unsigned      = 0;
          param[e].is_null          = &static_cast<MYSQL_DATABASE*>(this->database)->isnull;
          param[e].length           = 0;

          switch((DB_SQL_VARIANT_TYPE)variant->GetType())
            {
              case DB_SQL_VARIANT_TYPE_INTEGER        : param[e].buffer_type      = MYSQL_TYPE_SHORT;
                                                        param[e].buffer           = (unsigned char*)variant->GetData();
                                                        param[e].buffer_length    = variant->GetSize();
                                                        break;

              case DB_SQL_VARIANT_TYPE_DOUBLEINTEGER  : param[e].buffer_type      = MYSQL_TYPE_LONG;
                                                        param[e].buffer           = (unsigned char*)variant->GetData();
                                                        param[e].buffer_length    = variant->GetSize();
                                                        break;


              case DB_SQL_VARIANT_TYPE_DATETIME       : param[e].buffer_type      = MYSQL_TYPE_DATETIME;
                                                        param[e].buffer           = (unsigned char*)variant->GetData();
                                                        param[e].is_null          = 0;
                                                        param[e].buffer_length    = 0;
                                                        break;

              case DB_SQL_VARIANT_TYPE_STRING         : { DB_SQL_STRING variantdata;
                                                          variantdata.Set((XCHAR*)*variant);

                                                          XBUFFER*  variantdatabuffer = GEN_NEW XBUFFER();

                                                          variantdata.ConvertToUTF8(*variantdatabuffer);
                                                          buffers.Add(variantdatabuffer);

                                                          XDWORD size = variantdatabuffer->GetSize();

                                                          param[e].buffer_type      = MYSQL_TYPE_VARCHAR;
                                                          param[e].buffer           = variantdatabuffer->Get();
                                                          param[e].buffer_length    = size;

                                                        }
                                                        break;

               case DB_SQL_VARIANT_TYPE_FLOAT         : { float f = (*variant);
                                                          param[e].buffer_type      = MYSQL_TYPE_FLOAT;
                                                          param[e].buffer           = variant->GetData();
                                                        }
                                                        break;

                                      default         : return false;
            }
        }

      // Bind param block

      if(mysql_stmt_bind_param(stmt, param) != 0)
        {
          DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(mysqldb));
          database->GetErrorList()->Add(error);
          if (database->IsTransactionStarted()) database->Rollback();

          return true;
        }
   }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_QUERY::BindParametersToResult()
* @brief      Bind parameters to result
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_QUERY::BindParametersToResult()
{
  MYSQL_RES* mysqlresultfromstmt = mysql_stmt_result_metadata(stmt);
  if(!mysqlresultfromstmt)  return true;   // This statement gives no result set

  // Prepare memory
  int ncols=mysql_num_fields(mysqlresultfromstmt);
  if(!ncols) return true;

  MYSQL_RESULT* result = static_cast<MYSQL_RESULT*>(this->result);
  result->bind_results_size = ncols;

  result->bindresults = NULL;
  result->bindresults = GEN_NEW MYSQL_BIND[ncols];
  if(!result->bindresults)
    {
      this->database->Error(__L("Not enought memory"));
      return false;
    }

  memset(result->bindresults, 0, ncols*sizeof(MYSQL_BIND));

  // Iterate

  for(int e=0; e<ncols; e++)
    {
      MYSQL_FIELD* field = mysql_fetch_field(mysqlresultfromstmt);

      result->bindresults[e].buffer_type      = field->type;
      switch(field->type)
        {
          case MYSQL_TYPE_DATETIME  : result->bindresults[e].buffer = GEN_NEW MYSQL_TIME;                   break;
                            default : result->bindresults[e].buffer = GEN_NEW unsigned char[field->length]; break;
        }

      if(!result->bindresults[e].buffer)
        {
          GEN_DELETE (result->bindresults);
          return false;
        }

      result->bindresults[e].buffer_length    = field->length;
      result->bindresults[e].is_unsigned      = 0;
      result->bindresults[e].is_null          = 0;
      result->bindresults[e].length           = 0;
    }

  //ind param block

  int RC = mysql_stmt_bind_result(stmt, result->bindresults);
  if(RC)
    {
      DB_SQL_ERROR* error=GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(mysql_stmt_error(stmt));
      database->GetErrorList()->Add(error);
      if (database->IsTransactionStarted()) database->Rollback();

      return true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MYSQL_QUERY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MYSQL_QUERY::Clean()
{
  stmt    = NULL;
  mysqldb = NULL;
  param   = NULL;
  result  = NULL;
}




#endif

