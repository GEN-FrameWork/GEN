/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFEntityDefStringValue.cpp
* 
* @class      GRPVECTORFILEDXFENTITYDEFSTRINGVALUE
* @brief      Graphic Vector File DXF Entity DefStringValue class
* @ingroup    GRAPHIC
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPVectorFileDXFEntityDefStringValue.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE()
{
  Clean();   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE(int value, XCHAR* string)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  value : Value value.
* @param[in]  string : String to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE(int value, XCHAR* string)
{
  Clean(); 
}

      
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  stringvalue : Stringvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GRPVECTORFILEDXFENTITYDEFSTRINGVALUE(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
{
  Clean();

  CopyFrom (stringvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::~GRPVECTORFILEDXFENTITYDEFSTRINGVALUE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::~GRPVECTORFILEDXFENTITYDEFSTRINGVALUE()
{ 
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::operator = (GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* point)
* @brief      operator =
* @ingroup    GRAPHIC
* 
* @param[in]  point : Point pointer to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::operator =  (GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* point)
{
  CopyFrom(point);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::operator = (GRPVECTORFILEDXFENTITYDEFSTRINGVALUE point)
* @brief      operator =
* @ingroup    GRAPHIC
* 
* @param[in]  point : Point value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::operator =  (GRPVECTORFILEDXFENTITYDEFSTRINGVALUE point)
{
  CopyFrom(&point);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GetValue()
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GetValue() 
{ 
  return value; 
}
     
     
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::SetValue(int value)
* @brief      Set value
* @ingroup    GRAPHIC
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::SetValue(int value) 
{ 
  this->value = value; 
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GetString()
* @brief      Get string
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::GetString() 
{ 
   return &string; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::CopyTo(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
* @brief      Copy to
* @ingroup    GRAPHIC
* 
* @param[in]  stringvalue : Stringvalue pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::CopyTo(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
{
  if(!stringvalue) 
    {
      return false;
    }

  stringvalue->SetValue(GetValue());
  stringvalue->GetString()->Set(GetString()->Get());
   
  return true;
}
      

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::CopyFrom(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
* @brief      Copy from
* @ingroup    GRAPHIC
* 
* @param[in]  stringvalue : Stringvalue pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::CopyFrom(GRPVECTORFILEDXFENTITYDEFSTRINGVALUE* stringvalue)
{
  if(!stringvalue)
    {
      return false;
    }

  value  = stringvalue->GetValue();
  string = stringvalue->GetString()->Get();   

  return true;
}
   

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYDEFSTRINGVALUE::Clean()
{
  value = 0;
  string.Empty();   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGLIST::GRPVECTORFILEDXFENTITYDEFSTRINGLIST()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGLIST::GRPVECTORFILEDXFENTITYDEFSTRINGLIST()
{
  Clean();   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYDEFSTRINGLIST::~GRPVECTORFILEDXFENTITYDEFSTRINGLIST()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYDEFSTRINGLIST::~GRPVECTORFILEDXFENTITYDEFSTRINGLIST()
{ 
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYDEFSTRINGLIST::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYDEFSTRINGLIST::Clean()
{
  stringsvalue.DeleteContents();
  stringsvalue.DeleteAll();
}


