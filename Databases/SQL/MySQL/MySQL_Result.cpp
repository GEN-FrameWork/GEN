/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Result.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



#if defined(DB_SQL_ACTIVE) && defined(DB_MYSQL_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "MySQL_Result.h"

#include <mysql.h>

#include "MySQL_Query.h"
#include "MySQL_Result.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_RESULT::~MYSQL_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_RESULT::~MYSQL_RESULT()
{
  XQWORD ncols = this->GetNumColumns();

  if(bindresults)
    {
      int e=0;
      while(e < bind_results_size)
        {
          if(bindresults[e].buffer)
            {
              switch(bindresults[e].buffer_type)
                {                             
                  case MYSQL_TYPE_FLOAT       : delete((float*)bindresults[e].buffer);
                                                break;

                  case MYSQL_TYPE_LONG        :
                  case MYSQL_TYPE_SHORT       : delete((int*)bindresults[e].buffer);
                                                break;

                  case MYSQL_TYPE_LONGLONG    : delete((long long*)bindresults[e].buffer);
                                                break;

                  case MYSQL_TYPE_VAR_STRING  :
                  case MYSQL_TYPE_STRING      :
                  case MYSQL_TYPE_VARCHAR     :
                                                delete((char*)bindresults[e].buffer);
                                                break;

                  case MYSQL_TYPE_DATETIME    : delete((MYSQL_TIME*)bindresults[e].buffer);
                                                break; 

                              default         : break;
                }
            }

          bindresults[e].buffer = NULL;
          e++;

        }

      GEN_DELETE_ARRAY bindresults;
      bindresults = NULL;

    }

  row.Clear();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD MYSQL_RESULT::GetNumRows()
* @brief      Get num rows
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD MYSQL_RESULT::GetNumRows()
{
  return mysql_stmt_num_rows(query->stmt);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD MYSQL_RESULT::GetNumColumns()
* @brief      Get num columns
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD MYSQL_RESULT::GetNumColumns()
{
  return mysql_stmt_field_count(query->stmt);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::Seek(int index)
* @brief      Seek
* @ingroup    DATABASE
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::Seek(int index)
{
  mysql_stmt_data_seek(query->stmt, index);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::First()
* @brief      First
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::First()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::Last()
* @brief      Last
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::Last()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::Next()
* @brief      Next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::Next()
{
  int RC = mysql_stmt_fetch(query->stmt);

  currentrow++;
  hasnext = RC;

  return (RC==DB_SQL_MYSQL_OK);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::ProcessRow()
* @brief      Process row
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::ProcessRow()
{
  row.Clear();

  if(!GetNumRows()) return true;

  MYSQL_RES* ares = mysql_stmt_result_metadata(query->stmt);
  XQWORD     ncol = GetNumColumns();

  for(XQWORD e=0; e<ncol; e++)
    {
      DB_SQL_STRING* columnname = GEN_NEW DB_SQL_STRING();
      if(!columnname) return false;

      MYSQL_FIELD* field = &ares->fields[e];
      columnname->Set(field->name);

      DB_SQL_VARIANT* variant = GEN_NEW DB_SQL_VARIANT();
      if(!variant)
        {
          delete(columnname);
          return false;
        }

      DB_SQL_STRING columnvalue;

      if(!bindresults[e].buffer) continue;

      switch(bindresults[e].buffer_type)
        {                    
          case MYSQL_TYPE_SHORT       : { int value  = *(int*)bindresults[e].buffer;
                                          (*variant) = value;
                                        }
                                        break;

          case MYSQL_TYPE_LONG        : { long value = *(long*)bindresults[e].buffer;
                                          (*variant) = (int)value;
                                        }
                                        break;

          case MYSQL_TYPE_LONGLONG    : { XQWORD value = *(XQWORD*)bindresults[e].buffer;
                                          (*variant) = (XQWORD)value;
                                        }
                                        break;

          case MYSQL_TYPE_DOUBLE      :
          case MYSQL_TYPE_FLOAT       : { float value = *(float*)bindresults[e].buffer;
                                          (*variant) = value;
                                        }
                                        break;

          case MYSQL_TYPE_TIME        :
          case MYSQL_TYPE_DATE        : break;

          case MYSQL_TYPE_DATETIME    : { MYSQL_TIME* mytime=(MYSQL_TIME*)bindresults[e].buffer;

                                          DB_SQL_DATETIME time;

                                          time.SetDay(mytime->day);
                                          time.SetMonth(mytime->month);
                                          time.SetYear(mytime->year);

                                          time.SetHours(mytime->hour);
                                          time.SetMinutes(mytime->minute);
                                          time.SetSeconds(mytime->second);

                                          (*variant) = time;
                                        }
                                        break;

          case MYSQL_TYPE_VAR_STRING  :
          case MYSQL_TYPE_VARCHAR     : { XWORD* value = (XWORD*)bindresults[e].buffer;
                                          int    size  = bindresults[e].buffer_length;

                                          XSTRING received;
                                          received.ConvertFromUTF8((XBYTE*)value, size);
                                          (*variant) = received;
                                        }
                                        break; 

                      default         : break;


        }

      header.Add(columnname);
      row.Add(variant);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::Abort()
* @brief      Abort
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::Abort()
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_RESULT::HasNext()
* @brief      Has next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_RESULT::HasNext()
{
  return (hasnext != MYSQL_NO_DATA);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_RESULT::MYSQL_RESULT() : DB_SQL_RESULT()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_RESULT::MYSQL_RESULT() : DB_SQL_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MYSQL_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MYSQL_RESULT::Clean()
{
  query             = NULL;
  hasnext           = 0;
  bind_results_size = 0;
  bindresults       = NULL;
  currentrow        = 0;
}




#endif

