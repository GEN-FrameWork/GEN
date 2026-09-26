/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Result.h
* 
* @class      MYSQL_RESULT
* @brief      DataBase SQL MySQL Result class
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

#include "DB_SQL_Base.h"
#include "DB_SQL_Result.h"

#include "MySQL_Query.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class MYSQL_QUERY;

class MYSQL_RESULT : public DB_SQL_RESULT
{
  public:

    friend class            MYSQL_QUERY;

    virtual                ~MYSQL_RESULT                  ();

    virtual XQWORD          GetNumRows                    ();
    virtual XQWORD          GetNumColumns                 ();
    virtual bool            Seek                          (int index);
    virtual bool            First                         ();
    virtual bool            Last                          ();
    virtual bool            Next                          ();
    virtual bool            ProcessRow                    ();
    virtual bool            Abort                         ();
    virtual bool            HasNext                       ();

  protected:
                            MYSQL_RESULT                  ();

    MYSQL_QUERY*            query;
    int                     hasnext;

    MYSQL_BIND*             bindresults;
    int                     bind_results_size;

  private:

    void                    Clean                         ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



