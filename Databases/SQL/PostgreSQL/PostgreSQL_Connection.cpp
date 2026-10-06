/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       PostgreSQL_Connection.cpp
* 
* @class      POSTGRESQL_CONNECTION
* @brief      DataBase SQL PostgreSQL Connection class
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

#include "PostgreSQL_Connection.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         POSTGRESQL_CONNECTION::POSTGRESQL_CONNECTION(DB_SQL_DATABASE* database) : DB_SQL_CONNECTION(database)
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  database : Database pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
POSTGRESQL_CONNECTION::POSTGRESQL_CONNECTION(DB_SQL_DATABASE* database) : DB_SQL_CONNECTION(database)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         POSTGRESQL_CONNECTION::~POSTGRESQL_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
POSTGRESQL_CONNECTION::~POSTGRESQL_CONNECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_CONNECTION::Connect()
* @brief      Connect
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_CONNECTION::Connect()
{
  DB_SQL_STRING connectionstring;
  DB_SQL_STRING databaseUrl;
  DB_SQL_STRING databaseUser;
  DB_SQL_STRING databasePassword;
  DB_SQL_STRING databaseName;
  DB_SQL_STRING databasePort;
  DB_SQL_STRING databaseTimeOut;

  if(!FindOption(_L("URL"), &databaseUrl))
    {
      database->Error(_L("DATABASE URL argument not supplied"));
      return false;
    }

  if(!FindOption(_L("USER"), &databaseUser))
    {
      database->Error(_L("DATABASE User Name argument not supplied"));
      return false;
    }

  if(!FindOption(_L("PASSWORD"), &databasePassword))
    {
      database->Error(_L("DATABASE User Password argument not supplied"));
      return false;
    }

  if(!FindOption(_L("DATABASE"), &databaseName))
    {
      database->Error(_L("DATABASE Name argument not supplied"));
      return false;
    }

  if(!FindOption(_L("PORT"), &databasePort))
    {
      database->Error(_L("DATABASE Port argument not supplied"));
      return false;
    }

  FindOption(_L("TIMEOUT"), &databaseTimeOut);

  if(!databaseUrl.IsEmpty())
    {
      connectionstring.Add(_L("host = "));
      connectionstring.Add(databaseUrl);
      connectionstring.Add(_L(" "));
    }

  if(!databaseUser.IsEmpty())
    {
      connectionstring.Add(_L("user = "));
      connectionstring.Add(databaseUser);
      connectionstring.Add(_L(" "));
    }

  if(!databasePassword.IsEmpty())
    {
      connectionstring.Add(_L("password = "));
      connectionstring.Add(databasePassword);
      connectionstring.Add(_L(" "));
    }

  if(!databaseName.IsEmpty())
    {
      connectionstring.Add(_L("dbname = "));
      connectionstring.Add(databaseName);
      connectionstring.Add(_L(" "));
    }

  if(!databasePort.IsEmpty())
    {
      connectionstring.Add(_L("port = "));
      connectionstring.Add(databasePort);
      connectionstring.Add(_L(" "));
    }

  if(!databaseTimeOut.IsEmpty())
    {
      connectionstring.Add(_L("connect_timeout = "));
      connectionstring.Add(databaseTimeOut);
      connectionstring.Add(_L(" "));
    }

  XBUFFER conninfo;
  
  connectionstring.ConvertToASCII(conninfo);  
  connection = PQconnectdb(conninfo.GetPtrChar());

  bool status = true;

  if(PQstatus(connection) != CONNECTION_OK)
    {
      XSTRING error;

      error.Set(PQerrorMessage(connection));
      database->Error(error.Get());

      status = false;
    }
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool POSTGRESQL_CONNECTION::Disconnect()
* @brief      Disconnect
* @ingroup    DATABASE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool POSTGRESQL_CONNECTION::Disconnect()
{
  PQfinish(connection);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void POSTGRESQL_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void POSTGRESQL_CONNECTION::Clean()
{

}




#endif

