/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Factory.cpp
* 
* @class      DB_SQL_FACTORY
* @brief      DataBase SQL Connector Factory class
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

#include "DB_SQL_Factory.h"

#include "XTrace.h"

#ifdef DB_MYSQL_ACTIVE
#include "MySQL_Database.h"
#endif
#ifdef DB_POSTGRESQL_ACTIVE
#include "PostgreSQL_Database.h"
#endif
#ifdef DB_SQLITE_ACTIVE
#include "SQLite_Database.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_DATABASE* DB_SQL_FACTORY::Create(DB_SQL_DATABASE_TYPE databasetype)
* @brief      Create resource
* @ingroup    DATABASE
* 
* @param[in]  databasetype : Databasetype value.
* 
* @return     DB_SQL_DATABASE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_DATABASE* DB_SQL_FACTORY::Create( DB_SQL_DATABASE_TYPE databasetype)
{
  DB_SQL_DATABASE* db = NULL;

  switch(databasetype)
    {
      case DB_SQL_DATABASE_TYPE_MYSQL       :
                                              #ifdef DB_MYSQL_ACTIVE
                                              db = GEN_NEW MYSQL_DATABASE();
                                              return db;
                                              #endif
                                              break;

      case DB_SQL_DATABASE_TYPE_POSTGRESQL  :
                                              #ifdef DB_POSTGRESQL_ACTIVE
                                              db = GEN_NEW POSTGRESQL_DATABASE();
                                              return db;
                                              #endif
                                              break;

      case DB_SQL_DATABASE_TYPE_SQLITE      :
                                              #ifdef DB_SQLITE_ACTIVE
                                              db = GEN_NEW SQLITE_DATABASE();
                                              return db;
                                              #endif
                                              break;

                              default       : break;
    }

  return NULL;
}




#endif

