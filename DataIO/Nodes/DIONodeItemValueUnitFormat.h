/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItemValueUnitFormat.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"
#include "XVariant.h"
#include "XSerializable.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIONODEITEMVALUE_UNITSFORMAT_TYPE
{
  DIONODEITEMVALUE_UNITSFORMAT_TYPE_UNKNOWN              = 0 ,

  DIONODEITEMVALUE_UNITSFORMAT_TYPE_SIMPLE                   ,
  DIONODEITEMVALUE_UNITSFORMAT_TYPE_BOOLEAN                  ,
  
  DIONODEITEMVALUE_UNITSFORMAT_TYPE_RELATIVEHUMIDITY         ,

  DIONODEITEMVALUE_UNITSFORMAT_TYPE_CELSIUSDEGREE            ,            
  DIONODEITEMVALUE_UNITSFORMAT_TYPE_FAHRENHEITDEGREE         ,   
  DIONODEITEMVALUE_UNITSFORMAT_TYPE_KELVINDEGREE             ,

  DIONODEITEMVALUE_UNITSFORMAT_TYPE_OWNER                   

};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIONODEITEMVALUEUNITFORMAT : public XSERIALIZABLE
{
  public:
                               DIONODEITEMVALUEUNITFORMAT    ();
    virtual                   ~DIONODEITEMVALUEUNITFORMAT    ();

    XDWORD                     GetType                       ();
    void                       SetType                       (XDWORD type);

    XVARIANT*                  GetName                       ();
    XVARIANT*                  GetSymbol                     ();      
    
    virtual bool               Serialize                     ();                                          
    virtual bool               Deserialize                   (); 
  
  private:

    void                       GetDefaultName                ();
    void                       GetDefaultSymbol              ();

    void                       Clean                         ();

    XDWORD                     type; 
    XVARIANT                   name;
    XVARIANT                   symbol;      
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





