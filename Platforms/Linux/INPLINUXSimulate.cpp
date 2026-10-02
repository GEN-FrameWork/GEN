/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXSimulate.cpp
* 
* @class      INPLINUXSIMULATE
* @brief      LINUX Input Simulate
* @ingroup    PLATFORM_LINUX
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

#include "INPLINUXSimulate.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/input.h>
#include <linux/uinput.h>

#ifdef LINUX_X11_ACTIVE
#include <X11/Xlib.h>
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXSIMULATE::INPLINUXSIMULATE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXSIMULATE::INPLINUXSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXSIMULATE::~INPLINUXSIMULATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXSIMULATE::~INPLINUXSIMULATE()
{
  UInput_End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Key_Press(XBYTE code)
* @brief      Key press
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Windows-style virtual-key code used by INPSIMULATE tables.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Key_Press(XBYTE code)
{
  return Key_Event(code, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Key_UnPress(XBYTE code)
* @brief      Key un press
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Windows-style virtual-key code used by INPSIMULATE tables.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Key_UnPress(XBYTE code)
{
  return Key_Event(code, false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Key_Click(XBYTE code, int pressuretime)
* @brief      Key click
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Windows-style virtual-key code used by INPSIMULATE tables.
* @param[in]  pressuretime : Pressuretime value in milliseconds.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Key_Click(XBYTE code, int pressuretime)
{
  if(!Key_Press(code))
    {
      return false;
    }

  if(pressuretime < 1)
    {
      pressuretime = 1;
    }

  usleep((useconds_t)pressuretime * 1000);

  return Key_UnPress(code);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Mouse_SetPos(int x, int y)
* @brief      Mouse set pos
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Mouse_SetPos(int x, int y)
{
  #ifdef LINUX_X11_ACTIVE
  return Mouse_WarpX11(x, y);
  #else
  // Absolute cursor warp without X11 needs compositor-specific APIs or calibrated uinput ABS.
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Mouse_Click(int x, int y)
* @brief      Mouse click
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Mouse_Click(int x, int y)
{
  // Best-effort warp; still click at current pointer if warp is unavailable (e.g. pure Wayland).
  Mouse_SetPos(x, y);

  if(!UInput_Ini())
    {
      return false;
    }

  usleep(10000);

  if(!UInput_Emit(EV_KEY, BTN_LEFT, 1))
    {
      return false;
    }

  if(!UInput_Syn())
    {
      return false;
    }

  if(!UInput_Emit(EV_KEY, BTN_LEFT, 0))
    {
      return false;
    }

  return UInput_Syn();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::UInput_Ini()
* @brief      Create the virtual uinput device used to inject keys and mouse buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::UInput_Ini()
{
  struct uinput_setup usetup;
  int                 key;

  if(uinputfd >= 0)
    {
      return true;
    }

  uinputfd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if(uinputfd < 0)
    {
      uinputfd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
    }

  if(uinputfd < 0)
    {
      return false;
    }

  if(ioctl(uinputfd, UI_SET_EVBIT, EV_KEY) < 0)
    {
      UInput_End();
      return false;
    }

  if(ioctl(uinputfd, UI_SET_EVBIT, EV_SYN) < 0)
    {
      UInput_End();
      return false;
    }

  for(key = 0; key < KEY_CNT; key++)
    {
      ioctl(uinputfd, UI_SET_KEYBIT, key);
    }

  ioctl(uinputfd, UI_SET_KEYBIT, BTN_LEFT);
  ioctl(uinputfd, UI_SET_KEYBIT, BTN_RIGHT);
  ioctl(uinputfd, UI_SET_KEYBIT, BTN_MIDDLE);

  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor  = 0x1A2B;
  usetup.id.product = 0x3C4D;
  strncpy(usetup.name, "GEN Input Simulate", UINPUT_MAX_NAME_SIZE - 1);

  if(ioctl(uinputfd, UI_DEV_SETUP, &usetup) < 0)
    {
      UInput_End();
      return false;
    }

  if(ioctl(uinputfd, UI_DEV_CREATE) < 0)
    {
      UInput_End();
      return false;
    }

  // Userspace input nodes need a short settle time after UI_DEV_CREATE.
  usleep(100000);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::UInput_End()
* @brief      Destroy the virtual uinput device
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::UInput_End()
{
  if(uinputfd < 0)
    {
      return false;
    }

  ioctl(uinputfd, UI_DEV_DESTROY);
  close(uinputfd);
  uinputfd = -1;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::UInput_Emit(XWORD type, XWORD code, int value)
* @brief      Emit one uinput event
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  type : Event type (EV_KEY, EV_SYN, ...).
* @param[in]  code : Event code.
* @param[in]  value : Event value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::UInput_Emit(XWORD type, XWORD code, int value)
{
  struct input_event event;

  if(uinputfd < 0)
    {
      return false;
    }

  memset(&event, 0, sizeof(event));
  event.type  = type;
  event.code  = code;
  event.value = value;

  if(write(uinputfd, &event, sizeof(event)) != (ssize_t)sizeof(event))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::UInput_Syn()
* @brief      Emit EV_SYN / SYN_REPORT
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::UInput_Syn()
{
  return UInput_Emit(EV_SYN, SYN_REPORT, 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Key_Event(XBYTE code, bool pressed)
* @brief      Inject a key press or release through uinput
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Windows-style virtual-key code used by INPSIMULATE tables.
* @param[in]  pressed : true = key down, false = key up.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Key_Event(XBYTE code, bool pressed)
{
  XWORD linuxkey = GetLinuxKeyByVirtualKey(code);

  if(!linuxkey)
    {
      return false;
    }

  if(!UInput_Ini())
    {
      return false;
    }

  if(!UInput_Emit(EV_KEY, linuxkey, pressed ? 1 : 0))
    {
      return false;
    }

  return UInput_Syn();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD INPLINUXSIMULATE::GetLinuxKeyByVirtualKey(XBYTE code)
* @brief      Map a Windows virtual-key code (GEN INPSIMULATE tables) to a Linux KEY_* code
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Windows-style virtual-key code.
* 
* @return     XWORD : Linux KEY_* code, or 0 if unsupported.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD INPLINUXSIMULATE::GetLinuxKeyByVirtualKey(XBYTE code)
{
  switch(code)
    {
      case 0x08 : return KEY_BACKSPACE;
      case 0x09 : return KEY_TAB;
      case 0x0D : return KEY_ENTER;
      case 0x10 : return KEY_LEFTSHIFT;
      case 0x11 : return KEY_LEFTCTRL;
      case 0x12 : return KEY_LEFTALT;
      case 0x13 : return KEY_PAUSE;
      case 0x14 : return KEY_CAPSLOCK;
      case 0x1B : return KEY_ESC;
      case 0x20 : return KEY_SPACE;
      case 0x21 : return KEY_PAGEUP;
      case 0x22 : return KEY_PAGEDOWN;
      case 0x23 : return KEY_END;
      case 0x24 : return KEY_HOME;
      case 0x25 : return KEY_LEFT;
      case 0x26 : return KEY_UP;
      case 0x27 : return KEY_RIGHT;
      case 0x28 : return KEY_DOWN;
      case 0x2C : return KEY_SYSRQ;
      case 0x2D : return KEY_INSERT;
      case 0x2E : return KEY_DELETE;

      case 0x30 : return KEY_0;
      case 0x31 : return KEY_1;
      case 0x32 : return KEY_2;
      case 0x33 : return KEY_3;
      case 0x34 : return KEY_4;
      case 0x35 : return KEY_5;
      case 0x36 : return KEY_6;
      case 0x37 : return KEY_7;
      case 0x38 : return KEY_8;
      case 0x39 : return KEY_9;

      case 0x41 : return KEY_A;
      case 0x42 : return KEY_B;
      case 0x43 : return KEY_C;
      case 0x44 : return KEY_D;
      case 0x45 : return KEY_E;
      case 0x46 : return KEY_F;
      case 0x47 : return KEY_G;
      case 0x48 : return KEY_H;
      case 0x49 : return KEY_I;
      case 0x4A : return KEY_J;
      case 0x4B : return KEY_K;
      case 0x4C : return KEY_L;
      case 0x4D : return KEY_M;
      case 0x4E : return KEY_N;
      case 0x4F : return KEY_O;
      case 0x50 : return KEY_P;
      case 0x51 : return KEY_Q;
      case 0x52 : return KEY_R;
      case 0x53 : return KEY_S;
      case 0x54 : return KEY_T;
      case 0x55 : return KEY_U;
      case 0x56 : return KEY_V;
      case 0x57 : return KEY_W;
      case 0x58 : return KEY_X;
      case 0x59 : return KEY_Y;
      case 0x5A : return KEY_Z;

      case 0x5B : return KEY_LEFTMETA;
      case 0x5C : return KEY_RIGHTMETA;
      case 0x5D : return KEY_COMPOSE;

      case 0x60 : return KEY_KP0;
      case 0x61 : return KEY_KP1;
      case 0x62 : return KEY_KP2;
      case 0x63 : return KEY_KP3;
      case 0x64 : return KEY_KP4;
      case 0x65 : return KEY_KP5;
      case 0x66 : return KEY_KP6;
      case 0x67 : return KEY_KP7;
      case 0x68 : return KEY_KP8;
      case 0x69 : return KEY_KP9;
      case 0x6A : return KEY_KPASTERISK;
      case 0x6B : return KEY_KPPLUS;
      case 0x6D : return KEY_KPMINUS;
      case 0x6E : return KEY_KPDOT;
      case 0x6F : return KEY_KPSLASH;

      case 0x70 : return KEY_F1;
      case 0x71 : return KEY_F2;
      case 0x72 : return KEY_F3;
      case 0x73 : return KEY_F4;
      case 0x74 : return KEY_F5;
      case 0x75 : return KEY_F6;
      case 0x76 : return KEY_F7;
      case 0x77 : return KEY_F8;
      case 0x78 : return KEY_F9;
      case 0x79 : return KEY_F10;
      case 0x7A : return KEY_F11;
      case 0x7B : return KEY_F12;

      case 0x90 : return KEY_NUMLOCK;
      case 0x91 : return KEY_SCROLLLOCK;

      case 0xA0 : return KEY_LEFTSHIFT;
      case 0xA1 : return KEY_RIGHTSHIFT;
      case 0xA2 : return KEY_LEFTCTRL;
      case 0xA3 : return KEY_RIGHTCTRL;
      case 0xA4 : return KEY_LEFTALT;
      case 0xA5 : return KEY_RIGHTALT;

      case 0xBA : return KEY_SEMICOLON;     // OEM_1
      case 0xBB : return KEY_EQUAL;         // OEM_PLUS
      case 0xBC : return KEY_COMMA;         // OEM_COMMA
      case 0xBD : return KEY_MINUS;         // OEM_MINUS
      case 0xBE : return KEY_DOT;           // OEM_PERIOD
      case 0xBF : return KEY_SLASH;         // OEM_2
      case 0xC0 : return KEY_GRAVE;         // OEM_3  (ES: enie key often elsewhere)
      case 0xDB : return KEY_LEFTBRACE;     // OEM_4
      case 0xDC : return KEY_BACKSLASH;     // OEM_5
      case 0xDD : return KEY_RIGHTBRACE;    // OEM_6
      case 0xDE : return KEY_APOSTROPHE;    // OEM_7
      case 0xE2 : return KEY_102ND;         // OEM_102 (< > on ISO)

      default   : return 0;
    }
}


#ifdef LINUX_X11_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXSIMULATE::Mouse_WarpX11(int x, int y)
* @brief      Warp the X11 pointer to an absolute root-window position
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXSIMULATE::Mouse_WarpX11(int x, int y)
{
  Display* display = XOpenDisplay(NULL);
  Window   root;

  if(!display)
    {
      return false;
    }

  root = DefaultRootWindow(display);

  XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
  XFlush(display);
  XCloseDisplay(display);

  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPLINUXSIMULATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXSIMULATE::Clean()
{
  uinputfd = -1;
}
