/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Variant.h
* 
* @class      DB_SQL_VARIANT
* @brief      DataBase SQL Variant class
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


#ifdef DB_SQL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTrace.h"
#include "XVariant.h"

#include "DB_SQL_Base.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DB_SQL_VARIANT_TYPE
{
  DB_SQL_VARIANT_TYPE_NULL                        = XVARIANT_TYPE_NULL            ,
  DB_SQL_VARIANT_TYPE_INTEGER                     = XVARIANT_TYPE_INTEGER         ,
  DB_SQL_VARIANT_TYPE_DOUBLEINTEGER               = XVARIANT_TYPE_DOUBLEINTEGER   ,
  DB_SQL_VARIANT_TYPE_FLOAT                       = XVARIANT_TYPE_FLOAT           ,
  DB_SQL_VARIANT_TYPE_STRING                      = XVARIANT_TYPE_STRING          ,
  DB_SQL_VARIANT_TYPE_DATE                                                        ,
  DB_SQL_VARIANT_TYPE_TIME                                                         ,
  DB_SQL_VARIANT_TYPE_DATETIME
};

enum DB_SQL_VARIANT_FLAGS
{
  DB_SQL_VARIANT_FLAGS_NONE                       = 0     ,
  DB_SQL_VARIANT_FLAGS_NOT_NULL                   = 1<<1  ,
  DB_SQL_VARIANT_FLAGS_PRIMARY_KEY                = 1<<2  ,
  DB_SQL_VARIANT_FLAGS_UNIQUE_KEY_FLAG            = 1<<3  ,
  DB_SQL_VARIANT_FLAGS_UNIQUE_MULTIPLE_KEY_FLAG   = 1<<4  ,
  DB_SQL_VARIANT_FLAGS_UNSIGNED_FLAG              = 1<<5  ,
  DB_SQL_VARIANT_FLAGS_ZEROFILL_FLAG              = 1<<6  ,
  DB_SQL_VARIANT_FLAGS_BINARY_FLAG                = 1<<7  ,
  DB_SQL_VARIANT_FLAGS_AUTO_INCREMENT_FLAG        = 1<<8  ,
  DB_SQL_VARIANT_FLAGS_ENUM_FLAG                  = 1<<9  ,
  DB_SQL_VARIANT_FLAGS_SET_FLAG                   = 1<<10 ,
  DB_SQL_VARIANT_FLAGS_BLOB_FLAG                  = 1<<11 ,
  DB_SQL_VARIANT_FLAGS_TIMESTAMP_FLAG             = 1<<12 ,
  DB_SQL_VARIANT_FLAGS_NUM_FLAG                   = 1<<13 ,
  DB_SQL_VARIANT_FLAGS_NO_DEFAULT_VALUE_FLAG      = 1<<14
};

#define  DB_SQL_ROWHEADER       DB_SQL_VECTOR<DB_SQL_STRING*>

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4522)
#endif

class DB_SQL_VARIANT : public XVARIANT
{
  public:

    using XVARIANT::operator =;

                                    DB_SQL_VARIANT          ();
                                    DB_SQL_VARIANT          (const DB_SQL_VARIANT& variant);
    virtual                        ~DB_SQL_VARIANT          ();

    DB_SQL_VARIANT_FLAGS*           GetFlags                ();
    void                            SetFlags                (DB_SQL_VARIANT_FLAGS flags);

  protected:

    DB_SQL_VARIANT_FLAGS           flags;

  private:

    void                            Clean                   ();

};

#ifdef _MSC_VER
#pragma warning(pop)
#endif


class DB_SQL_ROW
{
  public:
                                    DB_SQL_ROW              ();
                                   ~DB_SQL_ROW              ();

    DB_SQL_VARIANT                  Get                     (int index);
    bool                            Add                     (DB_SQL_VARIANT* variant);

    void                            Clear                   ();

  protected:

    DB_SQL_VECTOR<DB_SQL_VARIANT*>  row;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



