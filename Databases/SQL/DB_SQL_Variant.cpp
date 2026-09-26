/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DB_SQL_Variant.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



#ifdef DB_SQL_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DB_SQL_Variant.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_VARIANT::DB_SQL_VARIANT(): XVARIANT()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_VARIANT::DB_SQL_VARIANT(): XVARIANT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_VARIANT::DB_SQL_VARIANT(const DB_SQL_VARIANT& variant): XVARIANT()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* @param[in]  XVARIANT : XVARIANT value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_VARIANT::DB_SQL_VARIANT(const DB_SQL_VARIANT& variant): XVARIANT()
{
  Clean();

  *this = variant;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_VARIANT::~DB_SQL_VARIANT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_VARIANT::~DB_SQL_VARIANT()
{
  if(data)  Destroy();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_VARIANT_FLAGS* DB_SQL_VARIANT::GetFlags()
* @brief      Get flags
* @ingroup    DATABASE
* 
* @return     DB_SQL_VARIANT_FLAGS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_VARIANT_FLAGS* DB_SQL_VARIANT::GetFlags()
{
  return &flags;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_VARIANT::SetFlags(DB_SQL_VARIANT_FLAGS flags)
* @brief      Set flags
* @ingroup    DATABASE
* 
* @param[in]  flags : Flags value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_VARIANT::SetFlags(DB_SQL_VARIANT_FLAGS flags)
{
  this->flags = flags;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_VARIANT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_VARIANT::Clean()
{
  flags   = DB_SQL_VARIANT_FLAGS_NONE;
  size    = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_ROW::DB_SQL_ROW()
* @brief      Constructor of class
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_ROW::DB_SQL_ROW()
{
  Clear();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_ROW::~DB_SQL_ROW()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_ROW::~DB_SQL_ROW()
{
  Clear();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DB_SQL_VARIANT DB_SQL_ROW::Get(int index)
* @brief      Get value
* @ingroup    DATABASE
* 
* @param[in]  index : Index value.
* 
* @return     DB_SQL_VARIANT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DB_SQL_VARIANT DB_SQL_ROW::Get(int index)
{
  if(row.Get(index) == NULL)
    {
      DB_SQL_VARIANT result;
      return result;
    }

  DB_SQL_VARIANT copy  = *row.FastGet(index);

  return copy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DB_SQL_ROW::Add(DB_SQL_VARIANT* variant)
* @brief      Add value
* @ingroup    DATABASE
* 
* @param[in]  variant : Variant pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DB_SQL_ROW::Add(DB_SQL_VARIANT* variant)
{
  return row.Add(variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DB_SQL_ROW::Clear()
* @brief      Clear
* @ingroup    DATABASE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DB_SQL_ROW::Clear()
{
  row.DeleteContents();
  row.DeleteAll();
}






#endif

