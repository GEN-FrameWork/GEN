/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPLINUXScreenFrameBuffer.cpp
* 
* @class      GRPLINUXSCREENFRAMEBUFFER
* @brief      LINUX Graphics Frame Buffer Screen class
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


#include "GRPLINUXScreenFrameBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/kd.h>
#include <errno.h>

#include "XTrace.h"

#include "GRP2DCanvas.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXSCREENFRAMEBUFFER::GRPLINUXSCREENFRAMEBUFFER(): GRPSCREEN()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFER::GRPLINUXSCREENFRAMEBUFFER(): GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_LINUX_FRAMEBUFFER;

  struct fb_var_screeninfo varinfo;
  struct fb_fix_screeninfo fixinfo;

  handlefb = open(GRPLINUXSCREENFRAMEBUFFER_DEVICE, O_RDWR);
  if(handlefb !=-1)
    {
      if(ioctl(handlefb, FBIOGET_VSCREENINFO, &varinfo) != -1)
        {
          SetWidth(varinfo.yres);
          SetHeight(varinfo.xres);

          SetSize(varinfo.xres, varinfo.yres);
          SetMaxSize(varinfo.xres, varinfo.yres);

          // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Linux FrameBuffer] Screen [%d x %d]  %d bits"), varinfo.xres, varinfo.yres, varinfo.bits_per_pixel);

          switch(varinfo.bits_per_pixel)
            {
              case   8 : SetMode(GRPPROPERTYMODE_08_INDEX);     break;
              case  16 : SetMode(GRPPROPERTYMODE_16_RGB_565);   break;
              case  24 : SetMode(GRPPROPERTYMODE_24_BGR_888);   break;
              case  32 : SetMode(GRPPROPERTYMODE_32_BGRA_8888); break;
            }

          SetIsBufferInverse(true);

        }
    }

  if(handlefb!=-1)  close(handlefb);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPLINUXSCREENFRAMEBUFFER::~GRPLINUXSCREENFRAMEBUFFER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFER::~GRPLINUXSCREENFRAMEBUFFER()
{
  Delete();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::Create(bool show)
* @brief      Create resource
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Create(bool show)
{
  Clean();

  handlefb = open(GRPLINUXSCREENFRAMEBUFFER_DEVICE, O_RDWR);
  if(handlefb!=-1)
    {
      int consoletty = open("/dev/tty1", O_RDWR);
      if(ioctl(consoletty, KDSETMODE, KD_GRAPHICS) != -1)
        {
          if(!ScreenResolution(width, height))
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[LINUX FrameBuffer] Change screen mode: error [%d]"), errno);
            }
         
          int zero = 0;
          if(ioctl(handlefb, FBIO_WAITFORVSYNC, &zero) == -1) 
            {
              XSTRING errorstr;
              
              errorstr = strerror(errno);

              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[LINUX FrameBuffer] Frame Buffer VSYNC: %s"), errorstr.Get());
            }

          return GRPSCREEN::Create(show);
        }
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[LINUX FrameBuffer] Open device screen: error [%d]"), errno);

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Update(GRP2DCANVAS* canvas)
{
  if(handlefb == -1) 
    {
      return false;
    }
  
  if(ioctl(handlefb, FBIO_WAITFORVSYNC, 0)) 
    {
      //printf("VSync failed.\n");
    }

  XBYTE* fbp = (XBYTE*)mmap(0, canvas->Buffer_GetSize(), PROT_READ | PROT_WRITE, MAP_SHARED, handlefb, 0);
  if(fbp == (XBYTE*)-1)  
    {
      return false;
    }
  
  memcpy(fbp, (XBYTE*)canvas->Buffer_Get(), canvas->Buffer_GetSize());
 
  munmap((XBYTE*)fbp, canvas->Buffer_GetSize());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::Delete()
* @brief      Delete resource
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Delete()
{
  ClearScreen();

  int consoletty = open("/dev/tty1", O_RDWR);
  ioctl(consoletty, KDSETMODE, KD_TEXT);

  if(handlefb!=-1)  close(handlefb);

  return GRPSCREEN::Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::Resize(int width, int height)
* @brief      Resize
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  width : GEN_NEW width
* @param[in]  height : GEN_NEW height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Resize(int width, int height)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::Show(bool active)
* @brief      Show
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Show(bool active)
{
  isshow  = active;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::ShowCursor(bool active)
* @brief      Show cursor
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::ShowCursor(bool active)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPLINUXSCREENFRAMEBUFFER::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_LINUX
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPLINUXSCREENFRAMEBUFFER::GetHandle()
{
  void* ptr = 0;

  memcpy((XBYTE*)&ptr, (XBYTE*)&handlefb, sizeof(int));

  return ptr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPLINUXSCREENFRAMEBUFFER::GetHandleFB()
* @brief      Get handle FB
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPLINUXSCREENFRAMEBUFFER::GetHandleFB()
{
  return handlefb;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::ScreenResolution(int width, int height)
* @brief      Screen resolution
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::ScreenResolution(int width, int height)
{
  if(handlefb == -1) return false;

  struct fb_fix_screeninfo fixed_info;
  struct fb_var_screeninfo variable_info;
  int                      screen_width;
  int                      screen_height;

  if(ioctl(handlefb, FBIOGET_VSCREENINFO, &variable_info) == -1)
    {
      return false;
    }

  screen_width               = variable_info.xres;
  screen_height              = variable_info.yres;

  variable_info.xres         = width;
  variable_info.yres         = height;

  variable_info.xres_virtual = variable_info.xres;
  variable_info.yres_virtual = variable_info.yres;

  if(ioctl(handlefb, FBIOPUT_VSCREENINFO, &variable_info) == -1)
    {
      ioctl(handlefb, FBIOGET_VSCREENINFO, &variable_info);

      SetWidth(screen_width);
      SetHeight(screen_height);

      return false;
    }

  if(ioctl(handlefb, FBIOGET_FSCREENINFO, &fixed_info) == -1)
    {
      return false;
    }

  /*
  variable_info.xres = width;
  variable_info.yres = height;

  if(ioctl(handlefb, FBIOPAN_DISPLAY    , &variable_info) == -1)
    {
      XTRACE_PRINTCOLOR(4, _L("ERROR! 4"));
      return false;
    }
  */

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPLINUXSCREENFRAMEBUFFER::ClearScreen()
* @brief      Clear screen
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::ClearScreen()
{
  if(handlefb == -1) return false;
  
  if(screencanvas)
    {
      XBYTE* fbp = (XBYTE *)mmap(0, screencanvas->Buffer_GetSize(), PROT_READ | PROT_WRITE, MAP_SHARED, handlefb, 0);
      if(fbp == (XBYTE*)-1)  
        {
          return false;
        }

      memset(fbp, 0, screencanvas->Buffer_GetSize());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPLINUXSCREENFRAMEBUFFER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPLINUXSCREENFRAMEBUFFER::Clean()
{
  handlefb  = -1;
}



