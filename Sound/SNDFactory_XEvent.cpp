/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFactory_XEvent.cpp
* 
* @class      SNDFACTORY_XEVENT
* @brief      Sound Factory eXtended event class
* @ingroup    SOUND
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

#include "SNDFactory_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFACTORY_XEVENT::SNDFACTORY_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
* @brief      Constructor of class
* @ingroup    SOUND
* 
* @param[in]  subject : Subject pointer to use.
* @param[in]  type : Type value.
* @param[in]  family : Family value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFACTORY_XEVENT::SNDFACTORY_XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFACTORY_XEVENT::~SNDFACTORY_XEVENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFACTORY_XEVENT::~SNDFACTORY_XEVENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM* SNDFACTORY_XEVENT::GetItem()
* @brief      Get item
* @ingroup    SOUND
* 
* @return     SNDITEM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM* SNDFACTORY_XEVENT::GetItem()
{
  return item;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDFACTORY_XEVENT::SetItem(SNDITEM* item)
* @brief      Set item
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDFACTORY_XEVENT::SetItem(SNDITEM* item)
{
  this->item = item;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDFACTORY_XEVENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDFACTORY_XEVENT::Clean()
{
  item   = NULL;
}



