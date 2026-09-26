/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Variant.h
* 
* @class      MYSQL_VARIANT
* @brief      DataBase SQL MySQL Variant class
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

#include "DB_SQL_Variant.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class MYSQL_VARIANT : public DB_SQL_VARIANT
{
  public:
                                  MYSQL_VARIANT       ();
    virtual                      ~MYSQL_VARIANT       ();

    virtual DB_SQL_VARIANT&       operator =          (DB_SQL_DATETIME& datetime);
    virtual operator              DB_SQL_DATETIME     ();

    virtual XDWORD                GetSize             ();
    virtual void*                 GetData             ();

  private:

    void                          Clean               ();

    MYSQL_TIME                    mysqltime;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



