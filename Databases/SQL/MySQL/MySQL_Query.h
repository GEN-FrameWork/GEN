/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Query.h
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

#pragma once


#if defined(DB_SQL_ACTIVE) && defined(DB_MYSQL_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <mysql.h>

#include "DB_SQL_Query.h"
#include "DB_SQL_Database.h"
#include "DB_SQL_Result.h"

#include "MySQL_Result.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class MYSQL_RESULT;

class MYSQL_QUERY : public DB_SQL_QUERY
{
  public:

    friend class                MYSQL_RESULT;

                                MYSQL_QUERY                 (DB_SQL_DATABASE* database);
    virtual                    ~MYSQL_QUERY                 ();

    virtual bool                Execute                     ();
    virtual bool                UnbindAll                   ();

    virtual bool                Bind                        (XDWORD ID, DB_SQL_DATETIME& datetime);

  protected:

    virtual bool                Exec                        (MYSQL* db, char* sql);

    virtual DB_SQL_RESULT*      ConstructResult             ();
    virtual bool                BindParametersToQuery       ();
    virtual bool                BindParametersToResult      ();

  private:

    void                        Clean                       ();

    MYSQL_STMT*                 stmt;
    MYSQL*                      mysqldb;
    MYSQL_BIND*                 param;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif




