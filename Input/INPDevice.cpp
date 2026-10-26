/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPDevice.cpp
* 
* @class      INPDEVICE
* @brief      Input Device Class
* @ingroup    INPUT
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

#include "INPDevice.h"

#include <stdio.h>

#include "XVector.h"

#include "INPButton.h"
#include "INPCursor.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPDEVICE::INPDEVICE()
* @brief      Constructor of class
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPDEVICE::INPDEVICE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPDEVICE::~INPDEVICE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPDEVICE::~INPDEVICE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::IsCreated()
* @brief      Is created
* @ingroup    INPUT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::IsCreated()
{
  return created;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::IsEnabled()
* @brief      Is enabled
* @ingroup    INPUT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::IsEnabled()
{
  return enabled;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPDEVICE::SetEnabled(bool enabled)
* @brief      Set enabled
* @ingroup    INPUT
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPDEVICE::SetEnabled(bool enabled)
{
  this->enabled  = enabled;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPDEVICE_TYPE INPDEVICE::GetType()
* @brief      Get type
* @ingroup    INPUT
* 
* @return     INPDEVICE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPDEVICE_TYPE INPDEVICE::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPDEVICE::SetType(INPDEVICE_TYPE type)
* @brief      Set type
* @ingroup    INPUT
* 
* @param[in]  type : type of device
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPDEVICE::SetType(INPDEVICE_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPDEVICE::GetNButtons()
* @brief      Get N buttons
* @ingroup    INPUT
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPDEVICE::GetNButtons()
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();
  if(!buttons) return 0;

  return buttons->GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<INPBUTTON*>* INPDEVICE::GetButtons()
* @brief      Get buttons
* @ingroup    INPUT
* 
* @return     XVECTOR<INPBUTTON*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<INPBUTTON*>* INPDEVICE::GetButtons()
{
  return &buttons;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<INPCURSOR*>* INPDEVICE::GetCursors()
* @brief      Get cursors
* @ingroup    INPUT
* 
* @return     XVECTOR<INPCURSOR*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<INPCURSOR*>* INPDEVICE::GetCursors()
{
  return &cursors;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::DeleteAllButtons()
* @brief      Delete all buttons
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::DeleteAllCursors()
* @brief      Delete all cursors
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::DeleteAllCursors()
{
  if(cursors.IsEmpty())  return false;

  cursors.DeleteContentsInstanced();
  cursors.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPBUTTON* INPDEVICE::GetButton(INPBUTTON_ID ID)
* @brief      Get button
* @ingroup    INPUT
* 
* @param[in]  ID : ID of button
* 
* @return     INPBUTTON* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPBUTTON* INPDEVICE::GetButton(INPBUTTON_ID ID)
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();
  if(!buttons) return NULL;

  XDWORD size=buttons->GetSize();
  for(XDWORD c=0;c<size;c++)
    {
      INPBUTTON*  button = (INPBUTTON*)buttons->FastGet(c);
      if(button)
          if(button->GetID() == ID)
              return button;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPBUTTON* INPDEVICE::GetButton(int index)
* @brief      Get button
* @ingroup    INPUT
* 
* @param[in]  index : index of button
* 
* @return     INPBUTTON* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPBUTTON* INPDEVICE::GetButton(int index)
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();
  if(!buttons) return NULL;

  if(buttons->IsEmpty())          return NULL;
  if(index>=(int)buttons->GetSize()) return NULL;
  if(index<0)                     return NULL;

  return (INPBUTTON*)buttons->Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPBUTTON* INPDEVICE::GetButtonByCode(XWORD code)
* @brief      Get button by code
* @ingroup    INPUT
* 
* @param[in]  code : code of button
* 
* @return     INPBUTTON* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPBUTTON* INPDEVICE::GetButtonByCode(XWORD code)
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();
  if(!buttons) return NULL;

  if(buttons->IsEmpty()) return NULL;

  for(XDWORD c=0;c<buttons->GetSize();c++)
    {
      INPBUTTON*  button = (INPBUTTON*)buttons->Get(c);
      if(button)
        {
          if(button->GetKeyCode() == code) return button;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPBUTTON_STATE INPDEVICE::GetButtonState(INPBUTTON_ID ID)
* @brief      Get button state
* @ingroup    INPUT
* 
* @param[in]  ID : ID of button
* 
* @return     INPBUTTON_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPBUTTON_STATE INPDEVICE::GetButtonState(INPBUTTON_ID ID)
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();

  if(!buttons) return INPBUTTON_STATE_UNKNOWN;

  XDWORD size = buttons->GetSize();
  for(XDWORD c=0; c<size; c++)
    {
      INPBUTTON*  button = (INPBUTTON*)buttons->FastGet(c);
      if(button)
        {
          if (button->GetID() == ID) return button->GetState();
        }
    }

  return INPBUTTON_STATE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPBUTTON* INPDEVICE::IsPressButton()
* @brief      Is press button
* @ingroup    INPUT
* 
* @return     INPBUTTON* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPBUTTON* INPDEVICE::IsPressButton()
{
  XVECTOR<INPBUTTON*>* buttons = GetButtons();
  if(!buttons) return NULL;

  for(int c=0;c<(int)buttons->GetSize();c++)
    {
      INPBUTTON* button = GetButton(c);
      if(button)
        {
          if(button->IsPressed()) return button;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::ReleaseAllButtons()
* @brief      Release all buttons
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::ReleaseAllButtons()
{
  for(XDWORD c=0;c<buttons.GetSize();c++)
    {
      INPBUTTON* button = (INPBUTTON*)buttons.Get(c);

      if(button)
        {
          button->SetPressed(false);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPDEVICE::GetNCursors()
* @brief      Get N cursors
* @ingroup    INPUT
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPDEVICE::GetNCursors()
{
  XVECTOR<INPCURSOR*>* cursors = GetCursors();
  if(!cursors) return 0;

  return cursors->GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR* INPDEVICE::GetCursor(INPCURSOR_ID ID)
* @brief      Get cursor
* @ingroup    INPUT
* 
* @param[in]  ID : id of cursor
* 
* @return     INPCURSOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR* INPDEVICE::GetCursor(INPCURSOR_ID ID)
{
  XVECTOR<INPCURSOR*>* cursors = GetCursors();
  if(!cursors) return NULL;

  if(cursors->IsEmpty()) return NULL;

  for(XDWORD c=0;c<cursors->GetSize();c++)
    {
      INPCURSOR* cursor = (INPCURSOR*)cursors->Get(c);
      if(cursor)
        {
          if(cursor->GetID() == ID) return cursor;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR* INPDEVICE::GetCursor(int index)
* @brief      Get cursor
* @ingroup    INPUT
* 
* @param[in]  index : index of cursor
* 
* @return     INPCURSOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR* INPDEVICE::GetCursor(int index)
{
  XVECTOR<INPCURSOR*>* cursors = GetCursors();
  if(!cursors) return NULL;

  if(cursors->IsEmpty())          return NULL;
  if(index>=(int)cursors->GetSize()) return NULL;
  if(index<0)                     return NULL;

  return (INPCURSOR*)cursors->Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR* INPDEVICE::IsChangeCursor()
* @brief      Is change cursor
* @ingroup    INPUT
* 
* @return     INPCURSOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR* INPDEVICE::IsChangeCursor()
{
  XVECTOR<INPCURSOR*>* cursors = GetCursors();
  if(!cursors) return NULL;

  for(int c=0;c<(int)cursors->GetSize();c++)
    {
      INPCURSOR* cursor = GetCursor(c);
      if(cursor)
        {
          if(cursor->IsChanged()) return cursor;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::SetScreen(void* param)
* @brief      Set screen
* @ingroup    INPUT
* 
* @param[in]  param : screen handle (void*)
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::SetScreen(void* param)
{
  if(param) grpscreen = static_cast<GRPSCREEN*>(param);

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::Release()
* @brief      Release
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::Release()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::Update()
* @brief      Update
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::Update()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::TranslateKeyCodeToUnicode(XWORD keycode, XSTRING& outtext)
* @brief      Translate key code to unicode (layout-aware); base stub.
* @ingroup    INPUT
* 
* @param[in]  keycode : Platform key code.
* @param[out] outtext : Produced Unicode text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::TranslateKeyCodeToUnicode(XWORD keycode, XSTRING& outtext)
{
  outtext.Empty();
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPDEVICE::TranslateButtonToUnicode(INPBUTTON* button, XSTRING& outtext)
* @brief      Translate button to unicode via its key code; base uses TranslateKeyCodeToUnicode.
* @ingroup    INPUT
* 
* @param[in]  button : Button pointer.
* @param[out] outtext : Produced Unicode text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPDEVICE::TranslateButtonToUnicode(INPBUTTON* button, XSTRING& outtext)
{
  outtext.Empty();
  if(!button) return false;

  return TranslateKeyCodeToUnicode(button->GetKeyCode(), outtext);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPDEVICE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPDEVICE::Clean()
{
  created   = false;
  enabled   = false;

  type      = INPDEVICE_TYPE_NONE;

  grpscreen = NULL;
}


