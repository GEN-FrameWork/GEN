/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       PostgreSQL_Database.h
* 
* @class      POSTGRESQL_DATABASE
* @brief      DataBase SQL PostgreSQL Database class
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


#if defined(DB_SQL_ACTIVE) && defined(DB_POSTGRESQL_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTrace.h"

#include "DB_SQL_Base.h"
#include "DB_SQL_Database.h"

#include "PostgreSQL_Query.h"
#include "PostgreSQL_Connection.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class POSTGRESQL_DATABASE : public DB_SQL_DATABASE
{
  public:

    friend class                      POSTGRESQL_CONNECTION;
    friend class                      POSTGRESQL_QUERY;

                                      POSTGRESQL_DATABASE         ();
    virtual                          ~POSTGRESQL_DATABASE         ();

    virtual DB_SQL_DATABASE_TYPE      GetType                     ();
    virtual XCHAR*                    GetTypeName                 ();

    virtual bool                      Transaction                 ();
    virtual bool                      Commit                      ();
    virtual bool                      Rollback                    (XCHAR* savepointname=NULL);
    virtual bool                      Savepoint                   (XCHAR* savepoint);
    virtual bool                      ReleaseSavepoint            (XCHAR* savepoint);
    virtual DB_SQL_QUERY*             CreateQuery                 ();
    virtual DB_SQL_CONNECTION*        CreateConnection            ();
    virtual bool                      GetTables                   ();
    virtual DB_SQL_DATABASE_ENCODING  GetDatabaseEncoding         ();
    virtual bool                      SetDatabaseEncoding         (DB_SQL_DATABASE_ENCODING encodingtouse);

    virtual bool                      OnConnection                ();
    virtual bool                      OnDisconnection             ();

    virtual bool                      IsThreadSafe                ();

  private:

    void                              Clean                       ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



