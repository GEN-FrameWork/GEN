/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItemValueUnitFormat.cpp
* 
* @class      DIONODEITEMVALUEUNITFORMAT
* @brief      Data Input/Output Node Item value unit format class
* @ingroup    DATAIO
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

#include "DIONodeItemValueUnitFormat.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEMVALUEUNITFORMAT::DIONODEITEMVALUEUNITFORMAT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEMVALUEUNITFORMAT::DIONODEITEMVALUEUNITFORMAT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEMVALUEUNITFORMAT::~DIONODEITEMVALUEUNITFORMAT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEMVALUEUNITFORMAT::~DIONODEITEMVALUEUNITFORMAT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIONODEITEMVALUEUNITFORMAT::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIONODEITEMVALUEUNITFORMAT::GetType()
{
  return type;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEMVALUEUNITFORMAT::SetType(XDWORD type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEMVALUEUNITFORMAT::SetType(XDWORD type)
{
  this->type = type;

  GetDefaultName();
  GetDefaultSymbol();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* DIONODEITEMVALUEUNITFORMAT::GetName()
* @brief      Get name
* @ingroup    DATAIO
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* DIONODEITEMVALUEUNITFORMAT::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* DIONODEITEMVALUEUNITFORMAT::GetSymbol()
* @brief      Get symbol
* @ingroup    DATAIO
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* DIONODEITEMVALUEUNITFORMAT::GetSymbol()
{
  return &symbol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMVALUEUNITFORMAT::Serialize()
* @brief      Serialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMVALUEUNITFORMAT::Serialize()
{  
  Primitive_Add<int>(type           , _L("type"));
  Primitive_Add<XVARIANT*>(&name    , _L("name"));
  Primitive_Add<XVARIANT*>(&symbol  , _L("symbol"));
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMVALUEUNITFORMAT::Deserialize()
* @brief      Deserialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMVALUEUNITFORMAT::Deserialize()
{
  Primitive_Extract<XDWORD>(type       , _L("type"));
  Primitive_Extract<XVARIANT>(name     , _L("name"));
  Primitive_Extract<XVARIANT>(symbol   , _L("symbol"));

  return true;
}

   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEMVALUEUNITFORMAT::GetDefaultName()
* @brief      Get default name
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEMVALUEUNITFORMAT::GetDefaultName()
{
  switch(type)
    {
      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_UNKNOWN              : 
                                                  default         : break; 

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_SIMPLE               : name = _L("simple");                   
                                                                    break; 

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_BOOLEAN              : name = _L("active/deactive (boolean)");                  
                                                                    break;  

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_RELATIVEHUMIDITY     : name = _L("relative humidity");        
                                                                    break; 

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_CELSIUSDEGREE        : name = _L("Celsius degree");           
                                                                    break;            

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_FAHRENHEITDEGREE     : name = _L("Fahrenheit degree");        
                                                                    break;    

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_KELVINDEGREE         : name = _L("Kelvin degree");            
                                                                    break;                                                               
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEMVALUEUNITFORMAT::GetDefaultSymbol()
* @brief      Get default symbol
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEMVALUEUNITFORMAT::GetDefaultSymbol()
{
  switch(type)
    {
      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_UNKNOWN              : 

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_SIMPLE               :  
      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_BOOLEAN              : break;  

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_RELATIVEHUMIDITY     : symbol = _L("%");           
                                                                    break; 

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_CELSIUSDEGREE        : { XSTRING typestr;

                                                                      typestr += (XCHAR)0x00B0;        
                                                                      typestr += _C('C');                                                                             

                                                                      symbol = typestr.Get();
                                                                    }
                                                                    break;            

      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_FAHRENHEITDEGREE     : { XSTRING typestr;

                                                                      typestr += (XCHAR)0x00B0;        
                                                                      typestr += _C('F');                                                                             

                                                                      symbol = typestr.Get();
                                                                    }
                                                                    break;  
  
      case DIONODEITEMVALUE_UNITSFORMAT_TYPE_KELVINDEGREE         : symbol = _L("K");           
                                                                    break;   

                                                        default   : break;
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEMVALUEUNITFORMAT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEMVALUEUNITFORMAT::Clean()
{
  type = DIONODEITEMVALUE_UNITSFORMAT_TYPE_UNKNOWN;
}



