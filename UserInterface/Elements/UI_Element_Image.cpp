/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Image.cpp
* 
* @class      UI_ELEMENT_IMAGE
* @brief      User Interface Element Image class
* @ingroup    USERINTERFACE
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

#include "UI_Element_Image.h"

#include "GRPBitmap.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_IMAGE::UI_ELEMENT_IMAGE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_IMAGE::UI_ELEMENT_IMAGE()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_IMAGE);
  GetTypeString()->Set(_L("image"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_IMAGE::~UI_ELEMENT_IMAGE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_IMAGE::~UI_ELEMENT_IMAGE()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ELEMENT_IMAGE::GetImage()
* @brief      Get image
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_IMAGE::GetImage()
{
  return image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_IMAGE::SetImage(GRPBITMAP* image)
* @brief      Set image
* @ingroup    USERINTERFACE
* 
* @param[in]  image : Image pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_IMAGE::SetImage(GRPBITMAP* image)
{
  if(!image) return false;

  this->image = image;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_IMAGE::GetResource()
* @brief      Fase 7: resource path for density rebind.
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_IMAGE::GetResource()
{
  return &resource;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_IMAGE::SetResource(XCHAR* resource)
* @brief      Fase 7: store resource path for density rebind.
* @ingroup    USERINTERFACE
* 
* @param[in]  resource : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_IMAGE::SetResource(XCHAR* resource)
{
  this->resource = resource ? resource : _L("");
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE UI_ELEMENT_IMAGE::GetAlpha()
* @brief      Get alpha
* @ingroup    USERINTERFACE
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE UI_ELEMENT_IMAGE::GetAlpha()
{
  return alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_IMAGE::SetAlpha(XBYTE alpha)
* @brief      Set alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  alpha : Alpha value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_IMAGE::SetAlpha(XBYTE alpha)
{
  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_IMAGE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_IMAGE::Clean()
{
  image =  NULL;
  resource.Empty();
  alpha =  100;
}



