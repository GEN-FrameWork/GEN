/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebPageHTMLCreator_XEvent.cpp
* 
* @class      DIOWEBPAGEHTMLCREATOR_XEVENT
* @brief      Data Input/Output Web Page HTML Create eXtended Event class
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

#include "DIOWebPageHTMLCreator_XEvent.h"

#include "XString.h"
#include "XPath.h"
#include "XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATOR_XEVENT::DIOWEBPAGEHTMLCREATOR_XEVENT(XSUBJECT* subject, XDWORD type) : XEVENT(subject,type)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  subject : Subject pointer to use.
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR_XEVENT::DIOWEBPAGEHTMLCREATOR_XEVENT(XSUBJECT* subject, XDWORD type)  : XEVENT(subject, type)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATOR_XEVENT::~DIOWEBPAGEHTMLCREATOR_XEVENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR_XEVENT::~DIOWEBPAGEHTMLCREATOR_XEVENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATOR* DIOWEBPAGEHTMLCREATOR_XEVENT::GetPageHTMLCreator()
* @brief      Get page HTML creator
* @ingroup    DATAIO
* 
* @return     DIOWEBPAGEHTMLCREATOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR* DIOWEBPAGEHTMLCREATOR_XEVENT::GetPageHTMLCreator()
{
  return pageHTMLcreator;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR_XEVENT::SetPageHTMLCreator(DIOWEBPAGEHTMLCREATOR* pageHTMLcreator)
* @brief      Set page HTML creator
* @ingroup    DATAIO
* 
* @param[in]  pageHTMLcreator : Page HTM Lcreator pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR_XEVENT::SetPageHTMLCreator(DIOWEBPAGEHTMLCREATOR* pageHTMLcreator)
{
  this->pageHTMLcreator = pageHTMLcreator;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBPAGEHTMLCREATOR_XEVENT::GetXTAG()
* @brief      Get XTAG
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBPAGEHTMLCREATOR_XEVENT::GetXTAG()
{
  return &XTAG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR_XEVENT::SetXTAG(XSTRING* XTAG)
* @brief      Set XTAG
* @ingroup    DATAIO
* 
* @param[in]  XTAG : XTAG pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR_XEVENT::SetXTAG(XSTRING* XTAG)
{
  this->XTAG = XTAG->Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR_XEVENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR_XEVENT::Clean()
{
  pageHTMLcreator = NULL;
}



