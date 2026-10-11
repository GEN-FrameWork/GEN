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

  if(!FindOption(NL("URL"), &databaseUrl))
    {
      database->Error(NL("DATABASE URL argument not supplied"));
      return false;
    }

  if(!FindOption(NL("USER"), &databaseUser))
    {
      database->Error(NL("DATABASE User Name argument not supplied"));
      return false;
    }

  if(!FindOption(NL("PASSWORD"), &databasePassword))
    {
      database->Error(NL("DATABASE User Password argument not supplied"));
      return false;
    }

  if(!FindOption(NL("DATABASE"), &databaseName))
    {
      database->Error(NL("DATABASE Name argument not supplied"));
      return false;
    }

  if(!FindOption(NL("PORT"), &databasePort))
    {
      database->Error(NL("DATABASE Port argument not supplied"));
      return false;
    }

  FindOption(NL("TIMEOUT"), &databaseTimeOut);

  if(!databaseUrl.IsEmpty())
    {
      connectionstring.Add(NL("host = "));
      connectionstring.Add(databaseUrl);
      connectionstring.Add(NL(" "));
    }

  if(!databaseUser.IsEmpty())
    {
      connectionstring.Add(NL("user = "));
      connectionstring.Add(databaseUser);
      connectionstring.Add(NL(" "));
    }

  if(!databasePassword.IsEmpty())
    {
      connectionstring.Add(NL("password = "));
      connectionstring.Add(databasePassword);
      connectionstring.Add(NL(" "));
    }

  if(!databaseName.IsEmpty())
    {
      connectionstring.Add(NL("dbname = "));
      connectionstring.Add(databaseName);
      connectionstring.Add(NL(" "));
    }

  if(!databasePort.IsEmpty())
    {
      connectionstring.Add(NL("port = "));
      connectionstring.Add(databasePort);
      connectionstring.Add(NL(" "));
    }

  if(!databaseTimeOut.IsEmpty())
    {
      connectionstring.Add(NL("connect_timeout = "));
      connectionstring.Add(databaseTimeOut);
      connectionstring.Add(NL(" "));
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

