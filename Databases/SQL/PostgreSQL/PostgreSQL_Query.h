/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       PostgreSQL_Query.h
* 
* @class      POSTGRESQL_QUERY
* @brief      DataBase SQL PostgreSQL Query class
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

#include <libpq-fe.h>

#include "DB_SQL_Query.h"
#include "DB_SQL_Database.h"
#include "DB_SQL_Result.h"

#include "PostgreSQL_Result.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum POSTGRESQL_DATA_TYPE
{
  BOOLOID                   =   16  ,
  BYTEAOID                  =   17  ,
  CHAROID                   =   18  ,
  NAMEOID                   =   19  ,
  INT8OID                   =   20  ,
  INT2OID                   =   21  ,
  INT2VECTOROID             =   22  ,
  INT4OID                   =   23  ,
  REGPROCOID                =   24  ,
  TEXTOID                   =   25  ,
  OIDOID                    =   26  ,
  TIDOID                    =   27  ,
  XIDOID                    =   28  ,
  CIDOID                    =   29  ,
  OIDVECTOROID              =   30  ,
  JSONOID                   =  114  ,
  XMLOID                    =  142  ,
  PGNODETREEOID             =  194  ,
  PGDDLCOMMANDOID           =   32  ,
  POINTOID                  =  600  ,
  LSEGOID                   =  601  ,
  PATHOID                   =  602  ,
  BOXOID                    =  603  ,
  POLYGONOID                =  604  ,
  LINEOID                   =  628  ,
  FLOAT4OID                 =  700  ,
  FLOAT8OID                 =  701  ,
  ABSTIMEOID                =  702  ,
  RELTIMEOID                =  703  ,
  TINTERVALOID              =  704  ,
  UNKNOWNOID                =  705  ,
  CIRCLEOID                 =  718  ,
  CASHOID                   =  790  ,
  MACADDROID                =  829  ,
  INETOID                   =  869  ,
  CIDROID                   =  650  ,
  INT2ARRAYOID              = 1005  ,
  INT4ARRAYOID              = 1007  ,
  TEXTARRAYOID              = 1009  ,
  OIDARRAYOID               = 1028  ,
  FLOAT4ARRAYOID            = 1021  ,
  ACLITEMOID                = 1033  ,
  CSTRINGARRAYOID           = 1263  ,
  BPCHAROID                 = 1042  ,
  VARCHAROID                = 1043  ,
  DATETIME                  = 1080  ,
  DATEOID                   = 1082  ,
  TIMEOID                   = 1083  ,
  TIMESTAMPOID              = 1114  ,
  TIMESTAMPTZOID            = 1184  ,
  INTERVALOID               = 1186  ,
  TIMETZOID                 = 1266  ,
  BITOID                    = 1560  ,
  VARBITOID                 = 1562  ,
  NUMERICOID                = 1700  ,
  REFCURSOROID              = 1790  ,
  REGPROCEDUREOID           = 2202  ,
  REGOPEROID                = 2203  ,
  REGOPERATOROID            = 2204  ,
  REGCLASSOID               = 2205  ,
  REGTYPEOID                = 2206  ,
  REGROLEOID                = 4096  ,
  REGNAMESPACEOID           = 4089  ,
  REGTYPEARRAYOID           = 2211  ,
  UUIDOID                   = 2950  ,
  LSNOID                    = 3220  ,
  TSVECTOROID               = 3614  ,
  GTSVECTOROID              = 3642  ,
  TSQUERYOID                = 3615  ,
  REGCONFIGOID              = 3734  ,
  REGDICTIONARYOID          = 3769  ,
  JSONBOID                  = 3802  ,
  INT4RANGEOID              = 3904  ,
  RECORDOID                 = 2249  ,
  RECORDARRAYOID            = 2287  ,
  CSTRINGOID                = 2275  ,
  ANYOID                    = 2276  ,
  ANYARRAYOID               = 2277  ,
  VOIDOID                   = 2278  ,
  TRIGGEROID                = 2279  ,
  EVTTRIGGEROID             = 3838  ,
  LANGUAGE_HANDLEROID       = 2280  ,
  INTERNALOID               = 2281  ,
  OPAQUEOID                 = 2282  ,
  ANYELEMENTOID             = 2283  ,
  ANYNONARRAYOID            = 2776  ,
  ANYENUMOID                = 3500  ,
  FDW_HANDLEROID            = 3115  ,
  TSM_HANDLEROID            = 3310  ,
  ANYRANGEOID               = 3831
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class POSTGRESQL_RESULT;

class POSTGRESQL_QUERY : public DB_SQL_QUERY
{
  public:

    friend  class             POSTGRESQL_RESULT;

                              POSTGRESQL_QUERY        (DB_SQL_DATABASE* owner);
    virtual                  ~POSTGRESQL_QUERY        ();
    virtual bool              Execute                 ();
    virtual bool              ClearCursor             ();

  protected:

    virtual DB_SQL_RESULT*    ConstructResult         ();
    virtual bool              BindParametersToQuery   ();
    bool                      BindParametersToResult  ();

    PGconn*                   conn;

  private:

    char**                    paramvalues;
    int*                      paramlengths;
    int*                      paramformats;

    void                      Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif


