/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MySQL_Connection.cpp
* 
* @class      MYSQL_CONNECTION
* @brief      DataBase SQL MySQL Connection class
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

#include "MySQL_Connection.h"

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_CONNECTION::MYSQL_CONNECTION(DB_SQL_DATABASE* database): DB_SQL_CONNECTION(database)
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  database : Database pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_CONNECTION::MYSQL_CONNECTION(DB_SQL_DATABASE* database): DB_SQL_CONNECTION(database)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MYSQL_CONNECTION::~MYSQL_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
MYSQL_CONNECTION::~MYSQL_CONNECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_CONNECTION::Connect()
* @brief      Connect
* @ingroup    DATABASE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_CONNECTION::Connect()
{
  connection = mysql_init(NULL);
  if(!connection)
    {
      XSTRING error;
      error.Set(mysql_error(connection));

      database->Error(error.Get());

      return false;
    }

  DB_SQL_STRING databaseurl;
  DB_SQL_STRING databaseuser;
  DB_SQL_STRING databasepassword;
  DB_SQL_STRING databasename;
  DB_SQL_STRING databaseport;
  DB_SQL_STRING databasetimeout;
  DB_SQL_STRING reconnect;

  if(!FindOption(_L("URL"), &databaseurl))
    {
      database->Error(_L("DATABASE URL argument not supplied"));
      return false;
    }

  if(!FindOption(_L("USER"), &databaseuser))
    {
      database->Error(_L("DATABASE User Name argument not supplied"));
      return false;
    }

  if(!FindOption(_L("PASSWORD"), &databasepassword))
    {
      database->Error(_L("DATABASE User Name argument not supplied"));
      return false;
    }

  FindOption(_L("DATABASE"), &databasename);
  FindOption(_L("PORT"), &databaseport);
  
  XBUFFER url;
  XBUFFER user;
  XBUFFER pswd;
  XBUFFER name;
  
  databaseurl.ConvertToASCII(url);
  databaseuser.ConvertToASCII(user);
  databasepassword.ConvertToASCII(pswd);
  databasename.ConvertToASCII(name);

  unsigned int port = databaseport.ConvertToDWord();

  bool success=true;

  if (FindOption(_L("RECONNECT"), &reconnect))
  {
    int reconnect_opt = reconnect.ConvertToInt();
    mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect_opt);
  }
  else
  {
    int reconnect_opt = 1;
    mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect_opt);
  }

  if (FindOption(_L("TIMEOUT"), &databasetimeout))
  {
      int timeoutseconds=databasetimeout.ConvertToInt();
      mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeoutseconds);
  }

  if(mysql_real_connect(connection, url.GetPtrChar(), user.GetPtrChar(), pswd.GetPtrChar(), name.GetPtrChar(), port, NULL, 0) == NULL)
    {
      DB_SQL_STRING error;

      error.Set(mysql_error(connection));
      database->Error(error.Get());
      mysql_close(connection);

      success = false;
    }
  
  return success;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MYSQL_CONNECTION::Disconnect()
* @brief      Disconnect
* @ingroup    DATABASE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MYSQL_CONNECTION::Disconnect()
{
  mysql_close(connection);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MYSQL_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MYSQL_CONNECTION::Clean()
{

}




#endif

