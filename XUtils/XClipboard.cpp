/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XClipboard.cpp
* 
* @class      XCLIPBOARD
* @brief      eXtended Utils Clipboard class (portable text clipboard)
* @ingroup    XUTILS
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

#include "XClipboard.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XCLIPBOARD* XCLIPBOARD::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCLIPBOARD::XCLIPBOARD()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCLIPBOARD::XCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCLIPBOARD::~XCLIPBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCLIPBOARD::~XCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::GetIsInstanced()
* @brief      Get is instanced
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCLIPBOARD& XCLIPBOARD::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XCLIPBOARD& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCLIPBOARD& XCLIPBOARD::GetInstance()
{
  if(!instance) instance = GEN_NEW XCLIPBOARD();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::SetInstance(XCLIPBOARD* _instance)
* @brief      Set instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @param[in]  _instance : GEN_NEW instance
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::SetInstance(XCLIPBOARD* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::SetText(XSTRING& text)
* @brief      Set text into the system clipboard
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  text : Text to place on the clipboard.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::SetText(XSTRING& text)
{
  (void)text;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::GetText(XSTRING& text)
* @brief      Get text from the system clipboard
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[out] text : Clipboard text (cleared on failure).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::GetText(XSTRING& text)
{
  text.Empty();
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::Clear()
* @brief      Clear the system clipboard
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::Clear()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XCLIPBOARD::HasText()
* @brief      Has text available on the system clipboard
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @return     bool : true if clipboard holds text; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XCLIPBOARD::HasText()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XCLIPBOARD::Update()
* @brief      Platform clipboard pump (no-op in the portable base)
* @note       VIRTUAL
* @ingroup    XUTILS
*
*---------------------------------------------------------------------------------------------------------------------*/
void XCLIPBOARD::Update()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XCLIPBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XCLIPBOARD::Clean()
{

}



