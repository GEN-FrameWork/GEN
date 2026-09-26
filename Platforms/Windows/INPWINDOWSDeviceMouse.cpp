/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPWINDOWSDeviceMouse.cpp
* 
* @class      INPWINDOWSDEVICEMOUSE
* @brief      WINDOWS Input Device Mouse class
* @ingroup    PLATFORM_WINDOWS
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

#include "INPWINDOWSDeviceMouse.h"

#include <windows.h>

#include "GRPWINDOWSScreen.h"

#include "INPButton.h"
#include "INPCursor.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSDEVICEMOUSE::INPWINDOWSDEVICEMOUSE(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSDEVICEMOUSE::INPWINDOWSDEVICEMOUSE(): INPDEVICE()
{
  Clean();

  SetType(INPDEVICE_TYPE_MOUSE);

  created = true;

  if(CreateAllButtons())
    {
      if(CreateAllCursors())
        {
          SetEnabled(true);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSDEVICEMOUSE::~INPWINDOWSDEVICEMOUSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSDEVICEMOUSE::~INPWINDOWSDEVICEMOUSE()
{
  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEMOUSE::Update()
* @brief      Update
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::Update()
{
  if(!IsEnabled()) return false;

  if(grpscreen)
    {
      if(grpscreen->GetHandle()!=GetForegroundWindow())
        {
          if(!grpscreen->Style_Is(GRPSCREENSTYLE_ONTOP))
            {
              #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
              // While a custom-caption drag is in progress this device MUST keep running. Bailing out here
              // freezes the whole device -- including the left button's state -- so the release that is
              // supposed to end the drag would never be seen and the window would keep following the cursor
              // after the user let go. The window is normally the foreground one during a drag anyway; this
              // only covers the window that momentarily loses foreground while the pointer is captured.
              if(!grpscreen->IsCFGChromesDragging())
              #endif
                {
                  return false;
                }
            }
        }
    }

  float x=0.0f;
  float y=0.0f;

  POINT point;
  if(GetCursorPos(&point))
    {
      int screenx;
      int screeny;
      int screenw;
      int screenh;

      if(GetWindowPosition(screenx, screeny, screenw, screenh))
        {
          x = (float)(point.x - screenx);
          y = (float)(screeny - point.y);

          if((x<0)||(y<0)||(x>screenw)||(y>screenh))
            {
              x = -1;
              y = -1;
            }
        }
    }
  
  for(XDWORD c=0; c<buttons.GetSize(); c++)
    {
      INPBUTTON* button = buttons.Get(c);
      if(button)
        {
          INPCURSOR* cursor = cursors.Get(c);
          if(cursor)
            {
              if(x!=cursor->GetX() || y!=cursor->GetY())
                {
                  cursor->SetIsChanged(true);
                  cursor->SetX(x);
                  cursor->SetY(y);
                }
               else cursor->SetIsChanged(false);
            }


          SHORT state = GetAsyncKeyState(button->GetKeyCode());
          if(state & 0x8000) //key down
            {
              if(state & 0x01) //changed
                {
                  button->SetPressed(true);

                  if(button->GetState()!=INPBUTTON_STATE_HOLD)  button->SetState(INPBUTTON_STATE_PRESSED);

                  if(cursor) cursor->AddPointToMotion(true);
                }
               else
                {
                  button->SetPressed(true);

                  button->SetState(INPBUTTON_STATE_HOLD);

                  if(cursor) cursor->AddPointToMotion(true);
                }
            }
           else
            {
              button->SetPressed(false);

              if (button->GetState() == INPBUTTON_STATE_HOLD || button->GetState() == INPBUTTON_STATE_PRESSED) //changed
                     button->SetState(INPBUTTON_STATE_RELEASED);
                else button->SetState(INPBUTTON_STATE_UP);

              if(cursor) cursor->AddPointToMotion(false);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPWINDOWSDEVICEMOUSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPWINDOWSDEVICEMOUSE::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEMOUSE::GetWindowPosition(int& screenx, int& screeny, int& screenw, int& screenh)
* @brief      Get window position
* @note       INTERNAL. screenx/screeny are the reference point Update() subtracts the raw cursor
*             screen position from (screeny using the bottom-up convention the canvas is stored in --
*             see GRPBLITGLES::Clean()'s flipy comment); screenw/screenh are the valid coordinate
*             range used there to decide whether the cursor is over the content at all.
*             IMPORTANT: these must be the CANVAS' own (fixed) design position/size, NOT the native
*             window's current (possibly resized) client rect. The canvas is anchored to the window's
*             TOP-LEFT corner and never rescaled (see GRPWINDOWSSCREEN/GRPWINDOWSBLITGLES): its LEFT
*             and TOP always coincide with the window's, but its RIGHT and BOTTOM only coincide with
*             the window's when the window happens to be exactly the canvas' size. Using the window's
*             own (resized) client rect here -- as this used to do -- puts the Y reference at the
*             window's bottom edge instead of the canvas', and the valid range at the window's current
*             size instead of the canvas' fixed one: correct only while the window is untouched, but
*             wrong (misaligned clicks on every menu/button/custom-chrome hit-test) as soon as it is
*             resized to anything other than the canvas' own size.
* @ingroup    PLATFORM_WINDOWS
*
* @param[in]  screenx : x position of screen
* @param[in]  screeny : y position of screen
* @param[in]  screenw : width of screen
* @param[in]  screenh : height of screen
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::GetWindowPosition(int& screenx, int& screeny, int& screenw, int& screenh)
{
  if(!grpscreen) return false;

  WINDOWINFO info;

  memset(&info, 0, sizeof(WINDOWINFO));
  info.cbSize = sizeof(WINDOWINFO);

  if(!GetWindowInfo((HWND)grpscreen->GetHandle(), &info))
    {
      return false;
    }

  screenw = (int)grpscreen->GetWidth();
  screenh = (int)grpscreen->GetHeight();

  screenx = info.rcClient.left;
  screeny = info.rcClient.top + screenh;   // bottom edge of the CANVAS (top-left anchored), not of the window

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEMOUSE::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::CreateAllButtons()
{ 
  INPBUTTON::CreateButton(&buttons, VK_LBUTTON, INPBUTTON_ID_MOUSE_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_RBUTTON, INPBUTTON_ID_MOUSE_RIGHT, __C('\x0')); 
  INPBUTTON::CreateButton(&buttons, VK_MBUTTON, INPBUTTON_ID_MOUSE_MIDDLE, __C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEMOUSE::CreateAllCursors()
* @brief      Create all cursors
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::CreateAllCursors()
{
  INPCURSOR* cursor;

  cursor = GEN_NEW INPCURSOR();
  if(!cursor) return false;

  cursor->SetID(INPCURSOR_ID_MOUSE);
  cursor->SetHavePreSelect(true);

  cursors.Add(cursor);

  return true;
}



