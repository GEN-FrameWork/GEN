/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjUse.cpp
* 
* @class      GRPVECTORFILESVGOBJUSE
* @brief      Graphic Vector File SVG Object Use class (references another element by id)
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

#include "GRPVectorFileSVGObjUse.h"

#include "XFileXML.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJUSE::GRPVECTORFILESVGOBJUSE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJUSE::GRPVECTORFILESVGOBJUSE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJUSE::~GRPVECTORFILESVGOBJUSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJUSE::~GRPVECTORFILESVGOBJUSE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJUSE::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes plus href / xlink:href and the x, y offset
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJUSE::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  XCHAR* valuehref = element->GetValueAttribute(_L("xlink:href"));
  if(!valuehref)  valuehref = element->GetValueAttribute(_L("href"));

  if(valuehref)
    {
      href = valuehref;

      if(!href.IsEmpty() && (href[0] == _C('#')))
        {
          XSTRING stripped;
          href.Copy(1, (int)href.GetSize(), stripped);
          href = stripped;
        }
    }

  x = element->GetFloatValueAttribute(_L("x"));
  y = element->GetFloatValueAttribute(_L("y"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGOBJUSE::GetHRef()
* @brief      Get href (referenced id, without the leading '#')
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGOBJUSE::GetHRef()
{
  return &href;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJUSE::GetX()
* @brief      Get x
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJUSE::GetX()   { return x; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJUSE::GetY()
* @brief      Get y
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGOBJUSE::GetY()   { return y; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJUSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJUSE::Clean()
{
  x = 0.0;
  y = 0.0;
}
