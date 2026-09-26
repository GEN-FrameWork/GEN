/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Database.h
* 
* @class      DB_SQL_DATABASE
* @brief      DataBase SQL "database" class
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

#include "XBase.h"
#include "XString.h"
#include "XPath.h"

#include "DB_SQL_Base.h"
#include "DB_SQL_Connection.h"
#include "DB_SQL_Error.h"
#include "DB_SQL_Query.h"
#include "DB_SQL_Variant.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DB_SQL_DATABASE_FLAGS
{
  DB_SQL_DATABASE_FLAGS_NONE            = 0     ,
  DB_SQL_DATABASE_FLAGS_SQL             = 1<<0  ,
  DB_SQL_DATABASE_FLAGS_TRANSACTIONAL   = 1<<1  ,
  DB_SQL_DATABASE_FLAGS_SERVERBASED     = 1<<2
};

enum DB_SQL_DATABASE_STATUS
{
  DB_SQL_DATABASE_FLAGS_UNKNOWN         = 0     ,
  DB_SQL_DATABASE_FLAGS_CONNECTED               ,
  DB_SQL_DATABASE_FLAGS_DISCONNECTED
};

enum DB_SQL_DATABASE_ENCODING
{
  DB_SQL_DATABASE_ENCODING_UNKNOWN      = -1    ,
  DB_SQL_DATABASE_ENCODING_SQL_ASCII            , //  unspecified (see text)              any
  DB_SQL_DATABASE_ENCODING_EUC_JP               , //  Extended UNIX Code-JP               Japanese
  DB_SQL_DATABASE_ENCODING_EUC_CN               , //  Extended UNIX Code-CN               Simplified  Chinese
  DB_SQL_DATABASE_ENCODING_EUC_KR               , //  Extended UNIX Code-KR               Korean
  DB_SQL_DATABASE_ENCODING_EUC_TW               , //  Extended UNIX Code-TW               Traditional Chinese, Taiwanese
  DB_SQL_DATABASE_ENCODING_EUC_JIS_2004         , //  Extended UNIX Code-JP, JIS X 0213   Japanese
  DB_SQL_DATABASE_ENCODING_UTF8                 , //  Unicode, 8-bit                      all
  DB_SQL_DATABASE_ENCODING_MULE_INTERNAL        , //  Mule internal code                  Multilingual
  DB_SQL_DATABASE_ENCODING_LATIN1               , //  ISO 8859-1, ECMA 94                 Western European
  DB_SQL_DATABASE_ENCODING_LATIN2               , //  ISO 8859-2, ECMA 94                 Central European
  DB_SQL_DATABASE_ENCODING_LATIN3               , //  ISO 8859-3, ECMA 94                 South European
  DB_SQL_DATABASE_ENCODING_LATIN4               , //  ISO 8859-4, ECMA 94                 North European
  DB_SQL_DATABASE_ENCODING_LATIN5               , //  ISO 8859-9, ECMA 128                Turkish
  DB_SQL_DATABASE_ENCODING_LATIN6               , //  ISO 8859-10,ECMA 144                Nordic
  DB_SQL_DATABASE_ENCODING_LATIN7               , //  ISO 8859-13                         Baltic
  DB_SQL_DATABASE_ENCODING_LATIN8               , //  ISO 8859-14                         Celtic
  DB_SQL_DATABASE_ENCODING_LATIN9               , //  ISO 8859-15                         LATIN1 with Euro and accents
  DB_SQL_DATABASE_ENCODING_LATIN10              , //  ISO 8859-16,ASRO SR 14111           Romanian
  DB_SQL_DATABASE_ENCODING_WIN1256              , //  Windows CP1256                      Arabic
  DB_SQL_DATABASE_ENCODING_WIN1258              , //  Windows CP1258                      Vietnamese
  DB_SQL_DATABASE_ENCODING_WIN866               , //  Windows CP866                       Cyrillic
  DB_SQL_DATABASE_ENCODING_WIN874               , //  Windows CP874                       Thai
  DB_SQL_DATABASE_ENCODING_KOI8R                , //  KOI8-R                              Cyrillic (Russian)
  DB_SQL_DATABASE_ENCODING_WIN1251              , //  Windows CP1251                      Cyrillic
  DB_SQL_DATABASE_ENCODING_WIN1252              , //  Windows CP1252                      Western European
  DB_SQL_DATABASE_ENCODING_ISO_8859_5           , //  ISO 8859-5, ECMA 113                Latin/Cyrillic  Yes 1
  DB_SQL_DATABASE_ENCODING_ISO_8859_6           , //  ISO 8859-6, ECMA 114                Latin/Arabic  Yes 1
  DB_SQL_DATABASE_ENCODING_ISO_8859_7           , //  ISO 8859-7, ECMA 118                Latin/Greek Yes 1
  DB_SQL_DATABASE_ENCODING_ISO_8859_8           , //  ISO 8859-8, ECMA 121                Latin/Hebrew  Yes 1
  DB_SQL_DATABASE_ENCODING_WIN1250              , //  Windows CP1250                      Central European
  DB_SQL_DATABASE_ENCODING_WIN1253              , //  Windows CP1253                      Greek
  DB_SQL_DATABASE_ENCODING_WIN1254              , //  Windows CP1254                      Turkish
  DB_SQL_DATABASE_ENCODING_WIN1255              , //  Windows CP1255                      Hebrew
  DB_SQL_DATABASE_ENCODING_WIN1257              , //  Windows CP1257                      Baltic
  DB_SQL_DATABASE_ENCODING_KOI8U                , //  KOI8-U                              Cyrillic (Ukrainian)
  DB_SQL_DATABASE_ENCODING_SJIS                 , //  Shift JIS                           Japanese
  DB_SQL_DATABASE_ENCODING_BIG5                 , //  Big Five                            Traditional Chinese
  DB_SQL_DATABASE_ENCODING_GBK                  , //  Extended National                   Standard  Simplified Chinese
  DB_SQL_DATABASE_ENCODING_UHC                  , //  Unified Hangul Code                 Korean
  DB_SQL_DATABASE_ENCODING_GB18030              , //  National Standard                   Chinese
  DB_SQL_DATABASE_ENCODING_JOHAB                , //  JOHAB                               Korean (Hangul)
  DB_SQL_DATABASE_ENCODING_SHIFT_JIS_2004       , //  Shift JIS, JIS X 0213               Japanese

  DB_SQL_DATABASE_ENCODING__LAST_ENCODING       ,
  DB_SQL_DATABASE_ENCODING_BE_LAST              = DB_SQL_DATABASE_ENCODING_KOI8U
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;
class DB_SQL_CONNECTION;

class DB_SQL_DATABASE
{
  public:

    friend class                              DB_SQL_VARIANT;
    friend class                              DB_SQL_QUERY;

                                              DB_SQL_DATABASE             ();
    virtual                                  ~DB_SQL_DATABASE             ();

    virtual bool                              Transaction                 ()                                        = 0;
    virtual bool                              Commit                      ()                                        = 0;
    virtual bool                              Rollback                    (XCHAR* savepointname = NULL)             = 0;
    virtual bool                              Savepoint                   (XCHAR* savepoint)                        = 0;
    virtual bool                              ReleaseSavepoint            (XCHAR* savepoint)                        = 0;

    virtual DB_SQL_QUERY*                     CreateQuery                 ()                                        = 0;
    virtual DB_SQL_CONNECTION*                CreateConnection            ()                                        = 0;
    virtual bool                              GetTables                   ()                                        = 0;

    virtual DB_SQL_DATABASE_ENCODING          GetDatabaseEncoding         ()                                        = 0;
    virtual bool                              SetDatabaseEncoding         (DB_SQL_DATABASE_ENCODING encodingtouse)  = 0;

    virtual DB_SQL_DATABASE_ENCODING          GetCurrentEncoding          ();
    virtual DB_SQL_DATABASE_TYPE              GetType                     ();
    virtual XCHAR*                            GetTypeName                 ();
    XDWORD                                    GetDefaultPort              ();
    XPATH*                                    GetDefaultPath              ();

    virtual bool                              IsOpen                      ();
    virtual bool                              SetConnection               (DB_SQL_CONNECTION* connection);

    virtual bool                              IsTransactionStarted        ();

    virtual DB_SQL_VECTOR<DB_SQL_STRING*>*    GetTableList                ();
    virtual DB_SQL_VECTOR<DB_SQL_ERROR*>*     GetErrorList                ();
    virtual DB_SQL_VARIANT*                   CreateVariant               ();

    virtual bool                              OnConnection                ();
    virtual bool                              OnDisconnection             ();

    virtual bool                              Open                        ();
    virtual bool                              Close                       ();

    virtual XCHAR*                            GetLastError                ();

    virtual bool                              Execute                     (DB_SQL_QUERY* constructedquery);

    virtual void                              Error                       (XCHAR* errorstring);
    virtual void                              Error                       (char* errorstring);
    virtual void                              ClearPreviousErrors         ();

    virtual bool                              Table_IsThere               (XCHAR* nametable, XCHAR* constfield, bool& isexist);
    virtual bool                              Table_IsThere               (XSTRING& nametable, XCHAR* constfield, bool& isexist);
    virtual bool                              Table_Create                (XCHAR* nametable, XCHAR* fields[], int nfields);
    virtual bool                              Table_Delete                (XCHAR* nametable);
    virtual bool                              Table_GetNRecords           (XCHAR* nametable, XQWORD& nrecords);

    virtual bool                              IsThreadSafe                ();

    virtual bool                              Lock                        ();
    virtual bool                              Unlock                      ();

    XMUTEX*                                   mutex;
    bool                                      isthreadsafe;

protected:

    virtual   bool                            ExecuteQuery                (DB_SQL_QUERY* wellconstructedquery);

    bool                                      transactionstarted;
    DB_SQL_CONNECTION*                        connection;
    XDWORD                                    type;
    XDWORD                                    defaultport;
    XPATH                                     defaultpath;
    DB_SQL_DATABASE_STATUS                    status;
    DB_SQL_VECTOR<DB_SQL_STRING*>             tables;
    DB_SQL_VECTOR<DB_SQL_ERROR*>              errorstack;
    DB_SQL_DATABASE_ENCODING                  encodingtype;

  private:

    void                                      Clean                       ();

};





/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



