/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SQLite_Result.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



#if defined(DB_SQL_ACTIVE) && defined(DB_SQLITE_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "SQLite_Result.h"

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SQLITE_RESULT::~SQLITE_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
SQLITE_RESULT::~SQLITE_RESULT()
{
  Clean();

  row.Clear();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD SQLITE_RESULT::GetNumRows()
* @brief      Get num rows
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD SQLITE_RESULT::GetNumRows()
{
  return 0x8000000000000000;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD SQLITE_RESULT::GetNumColumns()
* @brief      Get num columns
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD SQLITE_RESULT::GetNumColumns()
{
  return sqlite3_column_count(query->ppstmt);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::Seek(int index)
* @brief      Seek
* @ingroup    DATABASE
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::Seek(int index)
{
  // SQLITE Can't seek a result set
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::First()
* @brief      First
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::First()
{
  // SQLITE Can't seek a result set but, if not started, we can get first step as first row
  if(!initialized)
    {
      Next();
      GetRow();

      initialized = false;
      currentrow  = 0;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::Last()
* @brief      Last
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::Last()
{
  // SQLITE Can't seek a result set
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::Next()
* @brief      Next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::Next()
{
  int RC = sqlite3_step(query->ppstmt);

  lastresult = RC;

  if(RC!=SQLITE_ROW && RC!=SQLITE_DONE) return false;

  currentrow++;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::ProcessRow()
* @brief      Process row
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::ProcessRow()
{
  row.Clear();
  header.DeleteContents();
  header.DeleteAll();

  int ncol = (int)this->GetNumColumns();

  for(int e=0; e<ncol; e++)
    {
      DB_SQL_VARIANT* variant=GEN_NEW DB_SQL_VARIANT();
      if(!variant)    return false;

      DB_SQL_STRING columnvalue;
      char* text=(char*)sqlite3_column_text(query->ppstmt, (int)e);
      int   len = sqlite3_column_bytes(query->ppstmt, (int)e);
      columnvalue.Set(text);

      DB_SQL_STRING* columnname=GEN_NEW DB_SQL_STRING();
      if(!columnname)
        {
          delete(variant);
          return false;
        }

      columnname->Set(sqlite3_column_name(query->ppstmt, (int)e));

      int sqtype=sqlite3_column_type(query->ppstmt, (int)e);
      switch(sqtype)
        {
          case SQLITE_INTEGER   : { int i = columnvalue.ConvertToInt();
                                    (*variant) = i;
                                  }
                                  break;

          case SQLITE_FLOAT     : { float i = columnvalue.ConvertToFloat();
                                    (*variant) = i;
                                  }
                                  break;

          case SQLITE_BLOB      : { // unsupported
                                    // XTRACE_PRINTCOLOR(0,__L("Unsupported BLOB type"));
                                  }
                                  break;

          case SQLITE_NULL      : break;

          case SQLITE_TEXT      : { XSTRING received;

                                    received.ConvertFromUTF8((XBYTE*)text, len);
                                    (*variant) = received;
                                  }
                                  break;
        }

      header.Add(columnname);

      row.Add(variant);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::Abort()
* @brief      Abort
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::Abort()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SQLITE_RESULT::HasNext()
* @brief      Has next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SQLITE_RESULT::HasNext()
{
  return (lastresult == SQLITE_ROW);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SQLITE_RESULT::SQLITE_RESULT(): DB_SQL_RESULT()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
SQLITE_RESULT::SQLITE_RESULT(): DB_SQL_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SQLITE_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SQLITE_RESULT::Clean()
{
  lastresult  = SQLITE_ROW;
  query       = NULL;
}




#endif

