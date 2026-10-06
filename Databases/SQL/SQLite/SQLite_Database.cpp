/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SQLite_Database.cpp
* 
* @class      SQLITE_DATABASE
* @brief      DataBase SQL SQLite Database class
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

#include "SQLite_Database.h"

#include "XTrace.h"

#include "SQLite_Connection.h"
#include "SQLite_Query.h"

#include "DB_SQL_Query.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SQLITE_DATABASE::SQLITE_DATABASE() : DB_SQL_DATABASE()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
SQLITE_DATABASE::SQLITE_DATABASE() : DB_SQL_DATABASE()
{
  Clean();

  type  = DB_SQL_DATABASE_FLAGS_SQL | DB_SQL_DATABASE_FLAGS_TRANSACTIONAL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SQLITE_DATABASE::~SQLITE_DATABASE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
SQLITE_DATABASE::~SQLITE_DATABASE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_DATABASE_TYPE SQLITE_DATABASE::GetType()
* @brief      Get type
* @ingroup    DATABASE
* 
* @return     DB_SQL_DATABASE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_DATABASE_TYPE SQLITE_DATABASE::GetType()
{
  return DB_SQL_DATABASE_TYPE_SQLITE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* SQLITE_DATABASE::GetTypeName()
* @brief      Get type name
* @ingroup    DATABASE
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* SQLITE_DATABASE::GetTypeName()
{
  return _L("SQLite");
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::Transaction()
* @brief      Transaction
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::Transaction()
{
  transactionstarted = true;

  int rc=sqlite3_exec(sqlite3database, "BEGIN IMMEDIATE", 0, 0, 0);

  return (rc==SQLITE_OK || rc==SQLITE_BUSY);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::Commit()
* @brief      Commit
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::Commit()
{
  if(IsTransactionStarted())
    {
      int rc=sqlite3_exec(sqlite3database, "COMMIT", 0, 0, 0);
      transactionstarted = false;

      if(rc == SQLITE_OK)   return true;
      if(rc == SQLITE_BUSY) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::Rollback(XCHAR* savepoint)
* @brief      Rollback
* @ingroup    DATABASE
* 
* @param[in]  savepoint : Savepoint pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::Rollback(XCHAR* savepoint)
{
  if(IsTransactionStarted())
    {
      if(savepoint == NULL)
        {
          int rc = sqlite3_exec(sqlite3database, "ROLLBACK", 0, 0, 0);
          transactionstarted = false;

          return (rc==SQLITE_OK);
        }
       else
        {
          DB_SQL_STRING text;
          text.Format(_L("ROLLBACK TO SAVEPOINT %s"), savepoint);

          XBUFFER rollbacktext;
          
          text.ConvertToASCII(rollbacktext);
          int rc = sqlite3_exec(sqlite3database, rollbacktext.GetPtrChar(), 0, 0, 0);
          transactionstarted = false;
          
          return (rc==SQLITE_OK);
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::Savepoint(XCHAR* savepoint)
* @brief      Savepoint
* @ingroup    DATABASE
* 
* @param[in]  savepoint : Savepoint pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::Savepoint(XCHAR* savepoint)
{
  DB_SQL_STRING text;
  text.Format(_L("SAVEPOINT %s"), savepoint);

  XBUFFER rollbacktext;
  
  text.ConvertToASCII(rollbacktext);         
  int rc=sqlite3_exec(sqlite3database, rollbacktext.GetPtrChar(), 0, 0, 0);
  
  return (rc==SQLITE_OK);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::ReleaseSavepoint(XCHAR* savepoint)
* @brief      Release savepoint
* @ingroup    DATABASE
* 
* @param[in]  savepoint : Savepoint pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::ReleaseSavepoint(XCHAR* savepoint)
{
  DB_SQL_STRING text;
  text.Format(_L("RELEASE %s"), savepoint);

  XBUFFER rollbacktext;
  
  text.ConvertToASCII(rollbacktext);  
  int rc=sqlite3_exec(sqlite3database, rollbacktext.GetPtrChar(), 0, 0, 0);
  
  return (rc==SQLITE_OK);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_QUERY* SQLITE_DATABASE::CreateQuery()
* @brief      Create query
* @ingroup    DATABASE
* 
* @return     DB_SQL_QUERY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_QUERY* SQLITE_DATABASE::CreateQuery()
{
  DB_SQL_QUERY* query = GEN_NEW SQLITE_QUERY(this);
  if(!query)
    {      
      return NULL;
    }

  return query;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_CONNECTION* SQLITE_DATABASE::CreateConnection()
* @brief      Create connection
* @ingroup    DATABASE
* 
* @return     DB_SQL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_CONNECTION* SQLITE_DATABASE::CreateConnection()
{
  DB_SQL_CONNECTION* conn = GEN_NEW SQLITE_CONNECTION(this);
  return conn;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::GetTables()
* @brief      Get tables
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::GetTables()
{
  DB_SQL_QUERY* query = CreateQuery();
  if(!query) return false;

  bool status = true;

  tables.DeleteContents();
  tables.DeleteAll();

  query->Set(_L("SELECT name FROM sqlite_master WHERE type='table'"));
  if(query->Execute())
    {
      DB_SQL_RESULT* result=query->GetResult();
      if(result!=NULL) //hay un result
        {
          result->GetNumRows();
          while(result->HasNext())
            {
              result->ProcessRow();

              DB_SQL_ROW* row = result->GetRow();
              for(XDWORD e=0; e<result->GetNumColumns(); e++)
                {
                  if(row->Get(e).GetType()==(XVARIANT_TYPE)DB_SQL_VARIANT_TYPE_STRING)
                    {
                      DB_SQL_STRING* tablename = GEN_NEW DB_SQL_STRING();
                      if(tablename)
                        {
                          tablename->Set((XCHAR*)row->Get(0));
                          tables.Add(tablename);
                        }
                    }
                }

              result->Next();
            }
        }
    }
   else
    {
      Error(_L("Unable to obtain Tables"));
      status = false;
    }

  delete(query);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_DATABASE_ENCODING SQLITE_DATABASE::GetDatabaseEncoding()
* @brief      Get database encoding
* @ingroup    DATABASE
* 
* @return     DB_SQL_DATABASE_ENCODING : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_DATABASE_ENCODING SQLITE_DATABASE::GetDatabaseEncoding()
{
  return DB_SQL_DATABASE_ENCODING_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_DATABASE::SetDatabaseEncoding(DB_SQL_DATABASE_ENCODING encodingtouse)
* @brief      Set database encoding
* @ingroup    DATABASE
* 
* @param[in]  encodingtouse : Encodingtouse value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_DATABASE::SetDatabaseEncoding(DB_SQL_DATABASE_ENCODING encodingtouse)
{
  switch(encodingtouse)
    {
      case DB_SQL_DATABASE_ENCODING_BIG5              :
      case DB_SQL_DATABASE_ENCODING_EUC_CN            :
      case DB_SQL_DATABASE_ENCODING_EUC_JP            :
      case DB_SQL_DATABASE_ENCODING_EUC_JIS_2004      :
      case DB_SQL_DATABASE_ENCODING_EUC_KR            :
      case DB_SQL_DATABASE_ENCODING_EUC_TW            :
      case DB_SQL_DATABASE_ENCODING_GB18030           :
      case DB_SQL_DATABASE_ENCODING_GBK               :
      case DB_SQL_DATABASE_ENCODING_ISO_8859_5        :
      case DB_SQL_DATABASE_ENCODING_ISO_8859_6        :
      case DB_SQL_DATABASE_ENCODING_ISO_8859_7        :
      case DB_SQL_DATABASE_ENCODING_ISO_8859_8        :
      case DB_SQL_DATABASE_ENCODING_JOHAB             :
      case DB_SQL_DATABASE_ENCODING_KOI8R             :
      case DB_SQL_DATABASE_ENCODING_KOI8U             :
      case DB_SQL_DATABASE_ENCODING_LATIN1            :
      case DB_SQL_DATABASE_ENCODING_LATIN2            :
      case DB_SQL_DATABASE_ENCODING_LATIN3            :
      case DB_SQL_DATABASE_ENCODING_LATIN4            :
      case DB_SQL_DATABASE_ENCODING_LATIN5            :
      case DB_SQL_DATABASE_ENCODING_LATIN6            :
      case DB_SQL_DATABASE_ENCODING_LATIN7            :
      case DB_SQL_DATABASE_ENCODING_LATIN8            :
      case DB_SQL_DATABASE_ENCODING_LATIN9            :
      case DB_SQL_DATABASE_ENCODING_LATIN10           :
      case DB_SQL_DATABASE_ENCODING_MULE_INTERNAL     :
      case DB_SQL_DATABASE_ENCODING_SJIS              :
      case DB_SQL_DATABASE_ENCODING_SHIFT_JIS_2004    :
      case DB_SQL_DATABASE_ENCODING_SQL_ASCII         :
      case DB_SQL_DATABASE_ENCODING_UHC               : break;

      case DB_SQL_DATABASE_ENCODING_UTF8              : { DB_SQL_QUERY* query = CreateQuery();
                                                        if(!query) break;

                                                        query->Set(_L("PRAGMA encoding=\"UTF-8\""));
                                                        int rc = query->Execute();

                                                        GEN_DELETE query;

                                                        return rc==0;
                                                      }
                                                      break;
      case DB_SQL_DATABASE_ENCODING_WIN866            :
      case DB_SQL_DATABASE_ENCODING_WIN874            :
      case DB_SQL_DATABASE_ENCODING_WIN1250           :
      case DB_SQL_DATABASE_ENCODING_WIN1251           :
      case DB_SQL_DATABASE_ENCODING_WIN1252           :
      case DB_SQL_DATABASE_ENCODING_WIN1253           :
      case DB_SQL_DATABASE_ENCODING_WIN1254           :
      case DB_SQL_DATABASE_ENCODING_WIN1255           :
      case DB_SQL_DATABASE_ENCODING_WIN1256           :
      case DB_SQL_DATABASE_ENCODING_WIN1257           :
      case DB_SQL_DATABASE_ENCODING_WIN1258           : break;

                                      default         : break;

    }

  Error (_L("Unable to set the charset encoding"));

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SQLITE_DATABASE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SQLITE_DATABASE::Clean()
{
  type            = DB_SQL_DATABASE_FLAGS_NONE;
  connection      = NULL;
  sqlite3database = NULL;
}




#endif

