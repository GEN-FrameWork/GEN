/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SQLite_Result.h
* 
* @class      SQLITE_RESULT
* @brief      DataBase SQL SQLite Result class
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


#ifndef SQLITE_CORE
#define SQLITE_CORE
#endif

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DB_SQL_Base.h"
#include "DB_SQL_Result.h"

#include "SQLite_Query.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SQLITE_QUERY;

class SQLITE_RESULT : public DB_SQL_RESULT
{
  public:

    friend class            SQLITE_QUERY;

    virtual                ~SQLITE_RESULT           ();

    virtual XQWORD          GetNumRows              ();
    virtual XQWORD          GetNumColumns           ();
    virtual bool            Seek                    (int index);
    virtual bool            First                   ();
    virtual bool            Last                    ();
    virtual bool            Next                    ();
    virtual bool            ProcessRow              ();
    virtual bool            Abort                   ();
    virtual bool            HasNext                 ();

  protected:
                            SQLITE_RESULT           ();

    SQLITE_QUERY*           query;
    int                     lastresult;

  private:

    void                    Clean                   ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





