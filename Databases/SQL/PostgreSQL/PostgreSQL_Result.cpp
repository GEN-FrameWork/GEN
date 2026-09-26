/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       PostgreSQL_Result.cpp
* 
* @class      POSTGRESQL_RESULT
* @brief      DataBase SQL PostgreSQL Result class
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



#if defined(DB_SQL_ACTIVE) && defined(DB_POSTGRESQL_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "PostgreSQL_Result.h"

#include <stdlib.h>
#include <time.h>

#include "XTrace.h"
#include "XSystem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         POSTGRESQL_RESULT::~POSTGRESQL_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
POSTGRESQL_RESULT::~POSTGRESQL_RESULT()
{
  row.Clear();

  header.DeleteContents();
  header.DeleteAll();

  PQclear(this->resultset);
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD POSTGRESQL_RESULT::GetNumRows()
* @brief      Get num rows
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD POSTGRESQL_RESULT::GetNumRows()
{
  int res=PQntuples(this->resultset);
  return res;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD POSTGRESQL_RESULT::GetNumColumns()
* @brief      Get num columns
* @ingroup    DATABASE
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD POSTGRESQL_RESULT::GetNumColumns()
{
  return PQnfields(resultset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::Seek(int index)
* @brief      Seek
* @ingroup    DATABASE
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::Seek(int index)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::First()
* @brief      First
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::First()
{
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
* @fn         bool POSTGRESQL_RESULT::Last()
* @brief      Last
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::Last()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::Next()
* @brief      Next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::Next()
{
  currentrow++;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::ProcessRow()
* @brief      Process row
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::ProcessRow()
{
  row.Clear();
  header.DeleteContents();
  header.DeleteAll();

  // Headers
  int ncol = (int)GetNumColumns();

  char * testcmdstatus=PQcmdStatus(resultset);
  if (GetNumRows()==0)
    return true;

  for(int e=0; e<ncol; e++)
    {
      DB_SQL_STRING* head=GEN_NEW DB_SQL_STRING();
      if(!head)
        {
          DB_SQL_ERROR* error=GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_MEMORY_ERROR);
          if(!error) return false;

          error->description.Set(__L("not enought memory for result"));
          query->database->GetErrorList()->Add(error);

          return true;
        }

      char* name = PQfname(resultset, e);
      head->Set(name);
      header.Add(head);
   }

  // Row
  for(int e=0; e<ncol; e++)
    {
      DB_SQL_VARIANT* variant=GEN_NEW DB_SQL_VARIANT();
      if(!variant)
        {
          DB_SQL_ERROR* error = GEN_NEW DB_SQL_ERROR(DB_SQL_ERROR_TYPE_MEMORY_ERROR);
          if(!error) return false;

          error->description.Set(__L("not enought memory for result"));
          query->database->GetErrorList()->Add(error);

          return true;
        }

      Oid type     = PQftype    (this->resultset, e);
      int isBinary = PQfformat  (this->resultset, e);
      int isNull   = PQgetisnull(this->resultset, (int)currentrow, e);

      if(isNull != 1)
        {
          switch(type)
            {
              case BOOLOID              :
              case BYTEAOID             :
              case CHAROID              : break;

              case NAMEOID              : { char* data = PQgetvalue(resultset, (int)currentrow, e);
                                            (*variant) = data;
                                            XCHAR* value= ((XSTRING*)(variant->GetData()))->Get();
                                            (*variant) = data;
                                          }
                                          break;

              case INT8OID              : {
                                            XQWORD number  = 0;

                                            if (!isNull)
                                            {
                                              XBYTE* data    = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
                                              memcpy(&number, (void*)data, sizeof(XQWORD));
                                              if(is_le) SWAPQWORD(number);
                                            }

                                            (*variant) = number;
                                          }
                                          break;

              case INT2VECTOROID        : break;

              case INT2OID              : {
                                            short integer = 0;

                                            if (!isNull)
                                            {
                                              XBYTE* data   = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
                                              memcpy(&integer, (void*)data, sizeof(integer));
                                              if(is_le) SWAPWORD(integer);
                                            }

                                            (*variant) = (int)integer;
                                          }

              case INT4OID              : {
                                            int   integer = 0;

                                            if (!isNull)
                                            {
                                              XBYTE* data   = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
                                              memcpy(&integer, (void*)data, sizeof(integer));
                                              if(is_le) SWAPDWORD(integer);
                                            }

                                            (*variant)=integer;
                                          }
                                          break;

              case REGPROCOID           : break;

              case TEXTOID              : {
                                            if (!isNull)
                                            {
                                            char* data = PQgetvalue(resultset, (int)currentrow, e);
                                            (*variant)  = data;
                                            }
                                            else
                                              (*variant)  = __L("");
                                          }
                                          break;

              case OIDOID               :
              case TIDOID               :
              case XIDOID               :
              case CIDOID               :
              case OIDVECTOROID         : break;

              case JSONOID              : {
                                            if (!isNull)
                                            {
                                            char* data = PQgetvalue(resultset, (int)currentrow, e);
                                            (*variant)  = data;
                                            }
                                            else
                                              (*variant)  = __L("");
                                          }
                                          break;

              case XMLOID               :
              case PGNODETREEOID        :
              case PGDDLCOMMANDOID      :
              case POINTOID             :
              case LSEGOID              :
              case PATHOID              :
              case BOXOID               :
              case POLYGONOID           :
              case LINEOID              : break;

              case FLOAT4OID            : {
                                            double  dat=0.0;
                                            if (!isNull)
                                            {
                                            char*   data = PQgetvalue(resultset, (int)currentrow, e);
                                            if(data) memcpy(&dat, (void*)data, sizeof(dat));
                                            XDWORD dword;
                                            if(data) memcpy(&dword, (void*)data, sizeof(dword));
                                            if(is_le) SWAPDWORD(dword);
                                            memcpy(&dat, (void*)&dword, sizeof(float));
                                            }

                                            (*variant) = (float)dat;
                                          }
                                          break;

              case FLOAT8OID            : {
                                            double dat = 0.0;
                                            if (!isNull)
                                            {
                                            char* data = PQgetvalue(resultset, (int)currentrow, e);
                                            memcpy(&dat, (void*)data, sizeof(dat));
                                            XQWORD qword;
                                            memcpy(&qword, (void*)data, sizeof(qword));
                                            if(is_le) SWAPQWORD(qword);
                                            memcpy(&dat, (void*)&qword, sizeof(double));
                                            }

                                            (*variant) = (float)dat;
                                          }
                                          break;

              case ABSTIMEOID           :
              case RELTIMEOID           :
              case TINTERVALOID         :
              case UNKNOWNOID           :
              case CIRCLEOID            :
              case CASHOID              :
              case MACADDROID           :
              case INETOID              :
              case CIDROID              :
              case INT2ARRAYOID         :
              case INT4ARRAYOID         :
              case TEXTARRAYOID         :
              case OIDARRAYOID          :
              case FLOAT4ARRAYOID       :
              case ACLITEMOID           : break;

              case CSTRINGARRAYOID      : { char* data = PQgetvalue(resultset, (int)currentrow, e);
                                            (*variant) = data;
                                          }
                                          break;

              case BPCHAROID            : break;

              case VARCHAROID           : { char* data        = PQgetvalue  (this->resultset, (int)this->currentrow, e);
                                            int   length      = PQgetlength(this->resultset, (int)this->currentrow, e);

                                            XSTRING received;
                                            received.ConvertFromUTF8((XBYTE*)data, length);
                                            (*variant) = received;
                                          }
                                          break;

              case TIMEOID              : {
                                            XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);

                                            DB_SQL_DATETIME datetime;

                                            if (!isNull)
                                            {
                                            XQWORD qword;

                                            memcpy(&qword, (void*)data, sizeof(qword));
                                            if(is_le)
                                              SWAPQWORD(qword);

                                            qword/=1000000;

                                                      datetime.SetDay   (1);
                                                      datetime.SetMonth (1);
                                                      datetime.SetYear    (2000);

                                                      datetime.AddSeconds(qword);

                                                      datetime.SetDay   (0);
                                                      datetime.SetMonth (0);
                                                      datetime.SetYear    (0);
                                            }
                                            (*variant) = datetime;
                                            variant->SetType(XVARIANT_TYPE_TIME);
                                          }
                                          break;

              case DATEOID              : {
                                            XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);

                                            DB_SQL_DATETIME datetime;

                                            if (!isNull)
                                            {
                                            XDWORD dword;

                                            memcpy(&dword, (void*)data, sizeof(dword));
                                            if(is_le)
                                              SWAPDWORD(dword);

                                                      datetime.SetDay   (1);
                                                      datetime.SetMonth (1);
                                                      datetime.SetYear    (2000);
                                                      datetime.AddDays    (dword);
                                            }
                                            (*variant) = datetime;
                                            variant->SetType(XVARIANT_TYPE_DATE);
                                          }
                                          break;

              case DATETIME             :
              case TIMESTAMPOID         : { XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);

                                            DB_SQL_DATETIME datetime;

                                            if (!isNull)
                                            {
                                            XQWORD qword;
                                            memcpy(&qword, (void*)data, sizeof(qword));
                                            if(is_le) SWAPQWORD(qword);

                                            qword/=1000000;

                                            XDATETIME start;
                                                      start.SetDay    (1);
                                                      start.SetMonth  (1);
                                                      start.SetYear   (2000);

                                            XQWORD secstoposix=start.GetSeconsFromDate(false);
                                            datetime.SetDateFromSeconds(secstoposix+qword, false);
                                            }
                                            (*variant) = datetime;
                                          }
                                          break;

              case INTERVALOID          :
              case TIMETZOID            :
              case BITOID               :
              case VARBITOID            :
              case NUMERICOID           :
              case REFCURSOROID         :
              case REGPROCEDUREOID      :
              case REGOPEROID           :
              case REGOPERATOROID       :
              case REGCLASSOID          :
              case REGTYPEOID           :
              case REGROLEOID           :
              case REGNAMESPACEOID      :
              case REGTYPEARRAYOID      :
              case UUIDOID              :
              case LSNOID               :
              case TSVECTOROID          :
              case GTSVECTOROID         :
              case TSQUERYOID           :
              case REGCONFIGOID         :
              case REGDICTIONARYOID     :
              case JSONBOID             :
              case INT4RANGEOID         :
              case RECORDOID            :
              case RECORDARRAYOID       :
              case CSTRINGOID           :
              case ANYOID               :
              case ANYARRAYOID          :
              case VOIDOID              :
              case TRIGGEROID           :
              case EVTTRIGGEROID        :
              case LANGUAGE_HANDLEROID  :
              case INTERNALOID          :
              case OPAQUEOID            :
              case ANYELEMENTOID        :
              case ANYNONARRAYOID       :
              case ANYENUMOID           :
              case FDW_HANDLEROID       :
              case TSM_HANDLEROID       :
              case ANYRANGEOID          : break;
            }
        }

      row.Add(variant);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::Abort()
* @brief      Abort
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::Abort()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_RESULT::HasNext()
* @brief      Has next
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_RESULT::HasNext()
{
  return (((int)this->currentrow)<PQntuples(this->resultset));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         POSTGRESQL_RESULT::POSTGRESQL_RESULT() : DB_SQL_RESULT()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
POSTGRESQL_RESULT::POSTGRESQL_RESULT() : DB_SQL_RESULT()
{
  Clean();

  is_le  = GEN_XSYSTEM.HardwareUseLittleEndian();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void POSTGRESQL_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void POSTGRESQL_RESULT::Clean()
{
  query     = NULL;
  resultset = NULL;
}




#endif

