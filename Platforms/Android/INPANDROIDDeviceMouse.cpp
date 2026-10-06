/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPANDROIDDeviceMouse.cpp
* 
* @class      INPANDROIDDEVICEMOUSE
* @brief      ANDROID input device mouse class
* @ingroup    PLATFORM_ANDROID
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

#include "INPANDROIDDeviceMouse.h"

#include "GRPANDROIDScreen.h"

#include "INPButton.h"
#include "INPCursor.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPANDROIDDEVICEMOUSE::INPANDROIDDEVICEMOUSE(GRPANDROIDSCREEN* grpscreen)
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  grpscreen : Grpscreen pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPANDROIDDEVICEMOUSE::INPANDROIDDEVICEMOUSE(GRPANDROIDSCREEN* grpscreen)
{
  Clean();

  SetType(INPDEVICE_TYPE_MOUSE);

  created = true;

  SetScreen(grpscreen);

  if(CreateAllCursors() &&  CreateAllButtons())  SetEnabled(true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPANDROIDDEVICEMOUSE::~INPANDROIDDEVICEMOUSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPANDROIDDEVICEMOUSE::~INPANDROIDDEVICEMOUSE()
{
  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEMOUSE::Update()
* @brief      Update
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::Update()
{
  if(!IsEnabled()) return false;

  bool anypressed         = false;
  bool releasedthisframe  = false;

  for(XDWORD e=0; e<this->buttons.GetSize(); e++)
    {
      INPBUTTON* button = buttons.Get(e);
      if(!button) continue;

      if(button->IsPressed()) anypressed = true;

      // A button RELEASED by OnTouchEvent (the finger-up that happened during this frame's event
      // drain) must survive one full UI frame, so it ages to UP here and we flag the frame. The
      // pending "click" (IsPressedWithRelease) is consumed by the UI right after this Update().
      if(button->GetState() == INPBUTTON_STATE_RELEASED)
        {
          releasedthisframe = true;
          button->SetState(INPBUTTON_STATE_UP);
        }
    }

  // Touch input has no persistent hover: unlike a desktop mouse the pointer simply disappears when
  // the finger is lifted. Android delivers the touch through this mouse device, so without help the
  // cursor would stay frozen at the lift point and the UI would keep that element pre-selected
  // (highlighted) forever. The dedicated touchscreen device (INPLINUXDEVICETOUCHSCREENDIRECT) avoids
  // this by resetting its position to (-1,-1) every frame a finger is absent; X11 does the same when
  // the mouse leaves the window. We replicate that here: once no button is held AND the release
  // frame's click has already been handed to the UI, park the cursor off-canvas so the UI drops the
  // hover/preselect highlight (a CURSOR_MOVE to -1 triggers ResetPreselect, exactly like on desktop).
  if(!anypressed && !releasedthisframe)
    {
      for(XDWORD e=0; e<this->cursors.GetSize(); e++)
        {
          INPCURSOR* cursor = cursors.Get(e);
          if(!cursor) continue;

          if((cursor->GetX() != -1) || (cursor->GetY() != -1))
            {
              cursor->SetIsChanged(false);
              cursor->Set(-1, -1);
              cursor->SetIsChanged(true);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEMOUSE::SetScreen(void* screenpointer)
* @brief      Set screen
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  screenpointer : Screenpointer pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::SetScreen(void* screenpointer)
{
  grpscreen = static_cast<GRPANDROIDSCREEN*>(screenpointer);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEMOUSE::AddPosition(int index, float x, float y, bool ispressed)
* @brief      Add position
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  index : Index value.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  ispressed : Ispressed value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::AddPosition(int index, float x, float y, bool ispressed)
{
  if(!grpscreen) return false;

  INPCURSOR* cursor = GetCursor(index);
  if(!cursor) return false;

  GRPANDROIDSCREEN* _grpscreen = (GRPANDROIDSCREEN*)grpscreen;

  // The touch arrives in NATIVE-WINDOW pixels. The canvas/UI works in design-resolution pixels and the
  // blitter letterboxes the canvas inside the (usually larger, different-aspect) window, so the raw window
  // coordinate does not match the canvas. Convert it here; if the mapping is unavailable (no native window
  // yet) keep the raw value so behaviour never gets worse than before.
  float canvasx = x;
  float canvasy = y;
  _grpscreen->MapWindowToCanvas(x, y, canvasx, canvasy);

  // to place the origin coordinates top left instead of bottom left
  canvasy = (float)_grpscreen->GetHeight() - canvasy;

  cursor->SetIsChanged(false);

  if((cursor->GetX() != canvasx) || (cursor->GetY() != canvasy))
    {
      cursor->Set(canvasx, canvasy);
      cursor->SetIsChanged(true);
    }

  cursor->AddPointToMotion(ispressed);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPANDROIDDEVICEMOUSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPANDROIDDEVICEMOUSE::Clean()
{
  grpscreen = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEMOUSE::CreateAllButtons()
* @brief      Create all buttons
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, 0, INPBUTTON_ID_MOUSE_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, 1, INPBUTTON_ID_MOUSE_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, 2, INPBUTTON_ID_MOUSE_MIDDLE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, 3, INPBUTTON_ID_MOUSE_MIDDLE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, 4, INPBUTTON_ID_MOUSE_MIDDLE, _C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEMOUSE::CreateAllCursors()
* @brief      Create all cursors
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::CreateAllCursors()
{
  INPCURSOR* cursor;

  for(int c=0;c<5;c++)
    {
      cursor = GEN_NEW INPCURSOR();
      if(!cursor) return false;

      cursor->SetID(INPCURSOR_ID(INPCURSOR_ID_MOUSE+c));
      cursor->SetHavePreSelect(false);
      cursors.Add(cursor);
    }

  return true;
}



