/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXClipboard.cpp
* 
* @class      XLINUXCLIPBOARD
* @brief      LINUX eXtended Utils Clipboard class (X11 CLIPBOARD / UTF8_STRING)
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

#include "XLINUXClipboard.h"

#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXCLIPBOARD::XLINUXCLIPBOARD()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXCLIPBOARD::XLINUXCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXCLIPBOARD::~XLINUXCLIPBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXCLIPBOARD::~XLINUXCLIPBOARD()
{
  CloseDisplay();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::SetText(XSTRING& text)
* @brief      Set text into the X11 CLIPBOARD selection (UTF8_STRING)
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  text : Text to place on the clipboard.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::SetText(XSTRING& text)
{
#ifndef LINUX_X11_ACTIVE
  (void)text;
  return false;
#else
  if(!EnsureDisplay()) return false;

  ownedutf8.Delete();
  if(!text.IsEmpty())
    {
      if(!text.ConvertToUTF8(ownedutf8, true))
        {
          ownedutf8.Delete();
          return false;
        }
    }

  // Legacy cut buffer helps some older clients; CLIPBOARD is the modern path.
  if(ownedutf8.GetSize() > 0)
    {
      XChangeProperty(display, DefaultRootWindow(display), XA_CUT_BUFFER0, XA_STRING, 8, PropModeReplace, ownedutf8.Get(), (int)ownedutf8.GetSize());
    }
  else
    {
      XChangeProperty(display, DefaultRootWindow(display), XA_CUT_BUFFER0, XA_STRING, 8, PropModeReplace, (unsigned char*)"", 0);
    }

  XSetSelectionOwner(display, atomclipboard, window, CurrentTime);
  XFlush(display);

  if(XGetSelectionOwner(display, atomclipboard) != window)
    {
      ownclipboard = false;
      return false;
    }

  ownclipboard = true;
  ProcessPendingEvents();
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::GetText(XSTRING& text)
* @brief      Get text from the X11 CLIPBOARD selection (UTF8_STRING / STRING)
* @ingroup    PLATFORM_LINUX
* 
* @param[out] text : Clipboard text (cleared on failure).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::GetText(XSTRING& text)
{
  text.Empty();

#ifndef LINUX_X11_ACTIVE
  return false;
#else
  if(!EnsureDisplay()) return false;

  ProcessPendingEvents();

  if(ownclipboard && XGetSelectionOwner(display, atomclipboard) == window)
    {
      if(ownedutf8.GetSize() == 0) return false;
      return text.ConvertFromUTF8(ownedutf8);
    }

  XBUFFER utf8;
  XConvertSelection(display, atomclipboard, atomutf8, atomproperty, window, CurrentTime);
  XFlush(display);

  if(WaitSelectionNotify(atomproperty, utf8, XLINUXCLIPBOARD_GETTEXT_TIMEOUTMS))
    {
      if(utf8.GetSize() == 0) return false;
      return text.ConvertFromUTF8(utf8);
    }

  // Fallback: STRING (Latin-1 / locale bytes) then CUT_BUFFER0.
  utf8.Delete();
  XConvertSelection(display, atomclipboard, atomstring, atomproperty, window, CurrentTime);
  XFlush(display);

  if(WaitSelectionNotify(atomproperty, utf8, XLINUXCLIPBOARD_GETTEXT_TIMEOUTMS))
    {
      if(utf8.GetSize() == 0) return false;
      return text.ConvertFromUTF8(utf8);
    }

  Atom           actualtype   = None;
  int            actualformat = 0;
  unsigned long  nitems       = 0;
  unsigned long  bytesafter   = 0;
  unsigned char* data         = NULL;

  if(XGetWindowProperty(display, DefaultRootWindow(display), XA_CUT_BUFFER0, 0, 1024*1024, False, AnyPropertyType, &actualtype, &actualformat, &nitems, &bytesafter, &data) == Success && data && nitems > 0)
    {
      utf8.Delete();
      utf8.Add(data, (XDWORD)nitems);
      XFree(data);
      return text.ConvertFromUTF8(utf8);
    }

  if(data) XFree(data);
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::Clear()
* @brief      Clear the X11 CLIPBOARD selection owned by this process
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::Clear()
{
#ifndef LINUX_X11_ACTIVE
  return false;
#else
  if(!EnsureDisplay()) return false;

  ownedutf8.Delete();
  ownclipboard = false;

  XSetSelectionOwner(display, atomclipboard, None, CurrentTime);
  XChangeProperty(display, DefaultRootWindow(display), XA_CUT_BUFFER0, XA_STRING, 8, PropModeReplace, (unsigned char*)"", 0);
  XFlush(display);

  ProcessPendingEvents();
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::HasText()
* @brief      Has text available on the X11 CLIPBOARD selection
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if clipboard holds text; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::HasText()
{
#ifndef LINUX_X11_ACTIVE
  return false;
#else
  if(!EnsureDisplay()) return false;

  ProcessPendingEvents();

  if(ownclipboard && XGetSelectionOwner(display, atomclipboard) == window)
    {
      return (ownedutf8.GetSize() > 0);
    }

  Window owner = XGetSelectionOwner(display, atomclipboard);
  if(owner != None) return true;

  Atom           actualtype   = None;
  int            actualformat = 0;
  unsigned long  nitems       = 0;
  unsigned long  bytesafter   = 0;
  unsigned char* data         = NULL;

  bool hastxt = false;
  if(XGetWindowProperty(display, DefaultRootWindow(display), XA_CUT_BUFFER0, 0, 1, False, AnyPropertyType, &actualtype, &actualformat, &nitems, &bytesafter, &data) == Success)
    {
      hastxt = (nitems > 0 || bytesafter > 0);
    }

  if(data) XFree(data);
  return hastxt;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCLIPBOARD::Update()
* @brief      Pump X11 events so SelectionRequest from other clients can be served
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCLIPBOARD::Update()
{
#ifndef LINUX_X11_ACTIVE
  return;
#else
  ProcessPendingEvents();
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::EnsureDisplay()
* @brief      Ensure X11 display and helper window are open
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::EnsureDisplay()
{
#ifndef LINUX_X11_ACTIVE
  return false;
#else
  if(display && window) return true;

  display = XOpenDisplay(NULL);
  if(!display) return false;

  window = XCreateSimpleWindow(display, DefaultRootWindow(display), -10, -10, 1, 1, 0, 0, 0);
  if(!window)
    {
      CloseDisplay();
      return false;
    }

  atomclipboard = XInternAtom(display, "CLIPBOARD", False);
  atomtargets   = XInternAtom(display, "TARGETS", False);
  atomutf8      = XInternAtom(display, "UTF8_STRING", False);
  atomstring    = XA_STRING;
  atomproperty  = XInternAtom(display, "GEN_CLIPBOARD", False);

  XSelectInput(display, window, StructureNotifyMask);
  XFlush(display);
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCLIPBOARD::CloseDisplay()
* @brief      Close X11 display and helper window
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCLIPBOARD::CloseDisplay()
{
#ifdef LINUX_X11_ACTIVE
  if(display)
    {
      if(ownclipboard && window)
        {
          XSetSelectionOwner(display, atomclipboard, None, CurrentTime);
        }

      if(window)
        {
          XDestroyWindow(display, window);
          window = 0;
        }

      XCloseDisplay(display);
      display = NULL;
    }

  ownclipboard = false;
  ownedutf8.Delete();
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCLIPBOARD::ProcessPendingEvents()
* @brief      Process pending X11 events (SelectionRequest / SelectionClear)
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCLIPBOARD::ProcessPendingEvents()
{
#ifdef LINUX_X11_ACTIVE
  if(!display) return;

  while(XPending(display) > 0)
    {
      XEvent event;
      XNextEvent(display, &event);

      switch(event.type)
        {
          case SelectionClear :
            if(event.xselectionclear.selection == atomclipboard)
              {
                ownclipboard = false;
              }
            break;

          case SelectionRequest :
            {
              XSelectionRequestEvent* request = &event.xselectionrequest;
              XEvent                  response;

              memset(&response, 0, sizeof(response));
              response.xselection.type      = SelectionNotify;
              response.xselection.display   = request->display;
              response.xselection.requestor = request->requestor;
              response.xselection.selection = request->selection;
              response.xselection.target    = request->target;
              response.xselection.property  = None;
              response.xselection.time      = request->time;

              if(request->selection == atomclipboard && ownclipboard)
                {
                  if(request->target == atomtargets)
                    {
                      Atom targets[2];
                      targets[0] = atomutf8;
                      targets[1] = atomstring;
                      XChangeProperty(display, request->requestor, request->property, XA_ATOM, 32, PropModeReplace, (unsigned char*)targets, 2);
                      response.xselection.property = request->property;
                    }
                  else if(request->target == atomutf8 || request->target == atomstring)
                    {
                      unsigned char* data = ownedutf8.GetSize()? ownedutf8.Get() : (unsigned char*)"";
                      XChangeProperty(display, request->requestor, request->property, request->target, 8, PropModeReplace, data, (int)ownedutf8.GetSize());
                      response.xselection.property = request->property;
                    }
                }

              XSendEvent(display, request->requestor, False, 0, &response);
              XFlush(display);
            }
            break;

          default:
            break;
        }
    }
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCLIPBOARD::WaitSelectionNotify(Atom property, XBUFFER& utf8, int timeoutms)
* @brief      Wait for SelectionNotify and read the resulting property
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  property : Property atom used for the conversion.
* @param[out] utf8 : UTF-8 bytes read from the property.
* @param[in]  timeoutms : Maximum wait time in milliseconds.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCLIPBOARD::WaitSelectionNotify(Atom property, XBUFFER& utf8, int timeoutms)
{
#ifndef LINUX_X11_ACTIVE
  (void)property;
  (void)utf8;
  (void)timeoutms;
  return false;
#else
  if(!display) return false;

  utf8.Delete();

  struct timeval start;
  gettimeofday(&start, NULL);

  for(;;)
    {
      while(XPending(display) > 0)
        {
          XEvent event;
          XNextEvent(display, &event);

          if(event.type == SelectionClear)
            {
              if(event.xselectionclear.selection == atomclipboard)
                {
                  ownclipboard = false;
                }
              continue;
            }

          if(event.type == SelectionRequest)
            {
              XSelectionRequestEvent* request = &event.xselectionrequest;
              XEvent                  response;

              memset(&response, 0, sizeof(response));
              response.xselection.type      = SelectionNotify;
              response.xselection.display   = request->display;
              response.xselection.requestor = request->requestor;
              response.xselection.selection = request->selection;
              response.xselection.target    = request->target;
              response.xselection.property  = None;
              response.xselection.time      = request->time;

              if(request->selection == atomclipboard && ownclipboard)
                {
                  if(request->target == atomtargets)
                    {
                      Atom targets[2];
                      targets[0] = atomutf8;
                      targets[1] = atomstring;
                      XChangeProperty(display, request->requestor, request->property, XA_ATOM, 32, PropModeReplace, (unsigned char*)targets, 2);
                      response.xselection.property = request->property;
                    }
                  else if(request->target == atomutf8 || request->target == atomstring)
                    {
                      unsigned char* data = ownedutf8.GetSize()? ownedutf8.Get() : (unsigned char*)"";
                      XChangeProperty(display, request->requestor, request->property, request->target, 8, PropModeReplace, data, (int)ownedutf8.GetSize());
                      response.xselection.property = request->property;
                    }
                }

              XSendEvent(display, request->requestor, False, 0, &response);
              XFlush(display);
              continue;
            }

          if(event.type == SelectionNotify)
            {
              if(event.xselection.property == None) return false;

              Atom           actualtype   = None;
              int            actualformat = 0;
              unsigned long  nitems       = 0;
              unsigned long  bytesafter   = 0;
              unsigned char* data         = NULL;

              if(XGetWindowProperty(display, window, property, 0, 1024*1024, True, AnyPropertyType, &actualtype, &actualformat, &nitems, &bytesafter, &data) != Success)
                {
                  if(data) XFree(data);
                  return false;
                }

              if(data && nitems > 0 && actualformat == 8)
                {
                  XDWORD size = (XDWORD)nitems;
                  if(size > 0 && data[size-1] == 0) size--;
                  utf8.Add(data, size);
                }

              if(data) XFree(data);
              return (utf8.GetSize() > 0);
            }
        }

      struct timeval now;
      gettimeofday(&now, NULL);
      long elapsedms = (long)((now.tv_sec - start.tv_sec) * 1000L + (now.tv_usec - start.tv_usec) / 1000L);
      if(elapsedms >= timeoutms) return false;

      int    xfd     = ConnectionNumber(display);
      fd_set readset;
      FD_ZERO(&readset);
      FD_SET(xfd, &readset);

      struct timeval wait;
      long remaining = timeoutms - elapsedms;
      wait.tv_sec  = remaining / 1000;
      wait.tv_usec = (remaining % 1000) * 1000;

      select(xfd + 1, &readset, NULL, NULL, &wait);
    }
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCLIPBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCLIPBOARD::Clean()
{
#ifdef LINUX_X11_ACTIVE
  display       = NULL;
  window        = 0;
  atomclipboard = None;
  atomtargets   = None;
  atomutf8      = None;
  atomstring    = None;
  atomproperty  = None;
#endif

  ownedutf8.Delete();
  ownclipboard = false;
}



