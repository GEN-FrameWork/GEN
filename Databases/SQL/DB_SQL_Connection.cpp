/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Connection.cpp
* 
* @class      DB_SQL_CONNECTION
* @brief      DataBase SQL connection class
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



#ifdef DB_SQL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DB_SQL_Connection.h"

#include "XThread.h"
#include "XFactory.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_CONNECTION::DB_SQL_CONNECTION(DB_SQL_DATABASE* database)
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  database : Database pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_CONNECTION::DB_SQL_CONNECTION(DB_SQL_DATABASE* database)
{
  Clean();

  this->database = database;

  database->SetConnection(this);

  options.SetIsMulti(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_CONNECTION::~DB_SQL_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_CONNECTION::~DB_SQL_CONNECTION()
{
  options.DeleteKeyContents();
  options.DeleteElementContents();

  options.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_CONNECTION::SetOption(XCHAR* key, XCHAR* value)
* @brief      Set option
* @ingroup    DATABASE
* 
* @param[in]  key : Key pointer to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_CONNECTION::SetOption(XCHAR* key, XCHAR* value)
{
  DB_SQL_STRING* newkey = GEN_NEW DB_SQL_STRING();
  if(newkey)
    {
      DB_SQL_STRING* newvalue = GEN_NEW DB_SQL_STRING();
      if(newvalue)
        {
          newkey->Set   (key);
          newvalue->Set (value);

          return options.Add(newkey, newvalue);
        }

      GEN_DELETE newkey;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_CONNECTION::FindOption(XCHAR* key, DB_SQL_STRING* result)
* @brief      Find option
* @ingroup    DATABASE
* 
* @param[in]  key : Key pointer to use.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_CONNECTION::FindOption(XCHAR* key, DB_SQL_STRING* result)
{
  for(XDWORD e=0; e<this->options.GetSize(); e++)
    {
      if(!options.GetKey(e)->Compare(key))
        {
          result->Set(options.GetElement(e)->Get());

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_CONNECTION::DB_SQL_CONNECTION()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_CONNECTION::DB_SQL_CONNECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_CONNECTION::Clean()
{
  database = NULL;
}




#endif


