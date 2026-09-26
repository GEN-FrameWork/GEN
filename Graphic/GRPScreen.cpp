/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPScreen.cpp
* 
* @class      GRPSCREEN
* @brief      Graphics Screen Class
* @ingroup    GRAPHIC
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

#include "GRPScreen.h"

#include "GRPFactory.h"
#include "GRPXEvent.h"
#include "GRP2DCanvas.h"
#include "GRPViewPort.h"
#include "GRPFrameRate.h"

#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
#include "XPathsManager.h"
#include "UI_Manager.h"
#include "UI_Layout.h"
#include "UI_Element_Text.h"
#include "UI_Element_Form.h"
#include "UI_SkinCanvas.h"
#include "INPManager.h"
#include "XTimer.h"
#include "XTrace.h"
#endif


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- DEFINES -------------------------------------------------------------------------------------------------------*/


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
// Diagnostics for the custom-caption drag. Off by default: define GRPSCREEN_CHROMESDRAG_TRACE (or -D it on the
// compiler command line) to get one line per drag start/stop and per rejected press, which is what tells apart
// "the drag never started" from "the drag started but the platform did not move the window".
// Low frequency (one line per click): always on when tracing is compiled in.
#ifdef XTRACE_ACTIVE
#define GRPSCREEN_CHROMESDRAGTRACE(...)       XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __VA_ARGS__)
#else
#define GRPSCREEN_CHROMESDRAGTRACE(...)
#endif

// One line per moved frame: opt in with -DGRPSCREEN_CHROMESDRAG_TRACE, it is very noisy.
#if defined(GRPSCREEN_CHROMESDRAG_TRACE) && defined(XTRACE_ACTIVE)
#define GRPSCREEN_CHROMESDRAGTRACEMOVE(...)   XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __VA_ARGS__)
#else
#define GRPSCREEN_CHROMESDRAGTRACEMOVE(...)
#endif
#endif


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XMAP<void*, GRPSCREEN*>  GRPSCREEN::listscreens;

#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
XVECTOR<GRPSCREEN*>      GRPSCREEN::allscreens;
#endif


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREEN::GRPSCREEN()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREEN::GRPSCREEN()
{
  Clean();

  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CREATING);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CREATED);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CANVASCREATING);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_DELETING);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_DELETED);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGEPOSITION);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGESIZE);
  RegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGEFOCUS);

  framerate = GEN_NEW GRPFRAMERATE();

  desktopmanager = GEN_GRPFACTORY.CreateDesktopManager();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREEN::~GRPSCREEN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREEN::~GRPSCREEN()
{
  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  // Also done in Delete(), but a backend whose Delete() bails out early (GRPLINUXSCREENX11::Delete() returns
  // before reaching the base when its display/window are already gone) must not leave a dangling pointer in
  // the registry UpdateAllCFGChromesDrag() walks. XVECTOR::Delete() on an element that is not there is a
  // no-op.
  allscreens.Delete(this);
  #endif

  if(framerate)
    {
      GEN_DELETE framerate;
      framerate = NULL;
    }

  if(desktopmanager)
    {
      GEN_GRPFACTORY.DeleteDesktopManager(desktopmanager);
      desktopmanager = NULL;
    }

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  if(cfgchromesautohidetimer)
    {
      GEN_XFACTORY.DeleteTimer(cfgchromesautohidetimer);
      cfgchromesautohidetimer = NULL;
    }
  #endif

  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CREATING);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CANVASCREATING);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CREATED);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_DELETING);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_DELETED);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGEFOCUS);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGESIZE);
  DeRegisterEvent(GRPXEVENT_TYPE_SCREEN_CHANGEPOSITION);

  DeleteAllViewports();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENTYPE GRPSCREEN::GetType()
* @brief      Get type
* @ingroup    GRAPHIC
* 
* @return     GRPSCREENTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENTYPE GRPSCREEN::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsValid()
* @brief      Is valid
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsValid()
{
  return isvalid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsActive()
* @brief      Is active
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Style_Is(XDWORD style)
* @brief      Style is
* @ingroup    GRAPHIC
* 
* @param[in]  style : Style value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Style_Is(XDWORD style)
{
  return (styles & style)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::Style_Add(XDWORD style)
* @brief      Style add
* @ingroup    GRAPHIC
* 
* @param[in]  style : Style value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::Style_Add(XDWORD style)
{
  Styles_Set(Styles_Get() | style);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::Style_Remove(XDWORD style)
* @brief      Style remove
* @ingroup    GRAPHIC
* 
* @param[in]  style : Style value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::Style_Remove(XDWORD style)
{
  Styles_Set(Styles_Get() & ~style);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSCREEN::Styles_Get()
* @brief      Styles get
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSCREEN::Styles_Get()
{
  return styles;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::Styles_Set(XDWORD styles)
* @brief      Styles set
* @ingroup    GRAPHIC
* 
* @param[in]  styles : Styles value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::Styles_Set(XDWORD styles)
{
  this->styles = styles;

  if(Styles_IsFullScreen())
    {
      this->styles &= ~GRPSCREENSTYLE_TITLE;
      this->styles &= ~GRPSCREENSTYLE_NOWINDOWICONS;
      this->styles |=  GRPSCREENSTYLE_ONTOP;
    }  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Styles_IsFullScreen()
* @brief      Styles is full screen
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Styles_IsFullScreen()
{
  if(((this->styles & GRPSCREENSTYLE_FULLSCREEN)                   == GRPSCREENSTYLE_FULLSCREEN)                    ||
     ((this->styles & GRPSCREENSTYLE_FULLSCREEN_ADJUSTRESOLUTION)  == GRPSCREENSTYLE_FULLSCREEN_ADJUSTRESOLUTION))
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::SetPropertys(int width, int height, float DPIs, int stride, GRPPROPERTYMODE mode)
* @brief      Set propertys
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* @param[in]  DPIs : DP Is value.
* @param[in]  stride : Stride value.
* @param[in]  mode : Mode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::SetPropertys(int width, int height, float DPIs, int stride, GRPPROPERTYMODE mode)
{
  SetWidth(width);
  SetHeight(height);

  SetDPI(DPIs);

  SetStride(stride);

  SetMode(mode);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENROTATION GRPSCREEN::GetRotation()
* @brief      Get rotation
* @ingroup    GRAPHIC
* 
* @return     GRPSCREENROTATION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENROTATION GRPSCREEN::GetRotation()
{
  return rotation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::SetRotation(GRPSCREENROTATION rotation)
* @brief      Set rotation
* @ingroup    GRAPHIC
* 
* @param[in]  rotation : Rotation value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetRotation(GRPSCREENROTATION rotation)
{
  this->rotation = rotation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Rotate(int degrees)
* @brief      Configures the screen presentation rotation, expressed in degrees, CLOCKWISE.
* @ingroup    GRAPHIC
* 
* @param[in]  degrees : clockwise rotation in degrees (snapped to 0, 90, 180 or 270).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Rotate(int degrees)
{
  // Normalize to [0, 360)
  int normalized = degrees % 360;
  if(normalized < 0)
    {
      normalized += 360;
    }

  // Snap to the nearest 90 degree step: 0, 1, 2, 3  ->  0, 90, 180, 270
  int step = ((normalized + 45) / 90) % 4;

  switch(step)
    {
      case 1  : rotation = GRPSCREENROTATION_90_CLOCKWISE;     break;   //  90 clockwise
      case 2  : rotation = GRPSCREENROTATION_180;              break;   // 180
      case 3  : rotation = GRPSCREENROTATION_90_ANTICLOCKWISE; break;   // 270 clockwise == 90 anti-clockwise
      case 0  :
      default : rotation = GRPSCREENROTATION_NONE;             break;   //   0 (no rotation)
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsRotationOrthogonal()
* @brief      true when the current rotation swaps the screen axes (90 / 270 degrees)
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsRotationOrthogonal()
{
  return (rotation == GRPSCREENROTATION_90_CLOCKWISE || rotation == GRPSCREENROTATION_90_ANTICLOCKWISE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSCREEN::GetPresentationWidth()
* @brief      Width of the NATIVE WINDOW (presentation surface). On a 90/270 rotation the content is presented with its axes swapped, so the window width is the content HEIGHT. The content itself (canvas / viewport / UI, sized by GetWidth()/GetHeight()) is never changed.
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSCREEN::GetPresentationWidth()
{
  return IsRotationOrthogonal() ? GetHeight() : GetWidth();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSCREEN::GetPresentationHeight()
* @brief      Height of the NATIVE WINDOW (presentation surface). See GetPresentationWidth().
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSCREEN::GetPresentationHeight()
{
  return IsRotationOrthogonal() ? GetWidth() : GetHeight();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Create(bool show)
* @brief      Create resource
* @ingroup    GRAPHIC
* 
* @param[in]  show : Show value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Create(bool show)
{
  screencanvas = GRPFACTORY::GetInstance().CreateCanvas(this);
  if(!screencanvas)
    {
      return false;
    }

  // The screen width/height ARE the content size (the rotation only affects the native window,
  // see GRPSCREEN::Rotate / GetPresentationWidth/Height), so the canvas keeps the screen size.
  screencanvas->SetWidth(this->GetWidth());
  screencanvas->SetHeight(this->GetHeight());

  screencanvas->Buffer_Create();
      
  isactive = true;

  if(framerate) framerate->Reset();

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  if(allscreens.Find(this) == NOTFOUND)
    {
      allscreens.Add(this);
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Update(GRP2DCANVAS* canvas)
* @brief      Update
* @ingroup    GRAPHIC
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Update(GRP2DCANVAS* canvas)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
* @brief      Update transparent
* @ingroup    GRAPHIC
* 
* @param[in]  canvas : Canvas object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateTransparent(GRP2DCANVAS* canvas)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Delete()
* @brief      Delete resource
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Delete()
{
  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  allscreens.Delete(this);
  #endif

  if(screencanvas)
    {
      GEN_DELETE screencanvas;
      screencanvas = NULL;
    }

  isactive = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Get_Position(int &x, int &y)
* @brief      Get position
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Get_Position(int &x, int &y)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Set_Position(int x, int y)
* @brief      Set position
* @ingroup    GRAPHIC
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Set_Position(int x, int y)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::GetClientSize(int& width, int& height)
* @brief      Get client size: the LIVE size of the native window's client area, as opposed to
*             GetWidth()/GetHeight() which are the fixed CONTENT (canvas design) size. Each platform overrides
*             this with the real native query (GetClientRect on Windows, XGetWindowAttributes on X11); this
*             base fallback simply reports the content size, which is also correct for platforms whose window
*             can never be resized away from it.
* @ingroup    GRAPHIC
*
* @param[out] width : live client area width.
* @param[out] height : live client area height.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::GetClientSize(int& width, int& height)
{
  width  = (int)GetWidth();
  height = (int)GetHeight();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::IsClientSizeAtMaximum()
* @brief      Is client size at maximum: true when the native window's LIVE client area has already reached
*             the effective growth cap on BOTH axes, i.e. maximizing it could not make it any bigger. The cap
*             follows the exact same resolution rule every resize path in the engine uses (Windows'
*             ApplyResizeLimits / X11's ResolveViewportMax): the GRPVIEWPORT_ID_MAIN viewport's explicit
*             SetMaxSize() per axis, falling back to the viewport's own declared (design) size. With no
*             viewport or no usable cap this returns false (an OS maximize could always grow the window).
*             Used by the platform Maximize() overrides to turn a maximize request into a no-op -- keeping
*             both the SIZE and the POSITION untouched -- when the window is already as big as it is allowed
*             to get; without this, the OS would keep the capped size but still relocate the window to the
*             work area's top-left corner.
* @ingroup    GRAPHIC
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsClientSizeAtMaximum()
{
  GRPVIEWPORT* mainviewport = GetViewport(GRPVIEWPORT_ID_MAIN);
  if(!mainviewport)
    {
      return false;
    }

  int maxw = (int)mainviewport->GetMaxWidth();
  int maxh = (int)mainviewport->GetMaxHeight();

  if(maxw <= 0) maxw = (int)mainviewport->GetWidth();
  if(maxh <= 0) maxh = (int)mainviewport->GetHeight();

  if((maxw <= 0) || (maxh <= 0))
    {
      return false;
    }

  int clientwidth  = 0;
  int clientheight = 0;

  if(!GetClientSize(clientwidth, clientheight))
    {
      return false;
    }

  return ((clientwidth >= maxw) && (clientheight >= maxh));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::Resize(int width, int height)
* @brief      Resize
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Resize(int width, int height)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsShow()
* @brief      Is show
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsShow()
{
  return isshow;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Show(bool active)
* @brief      Show
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Show(bool active)
{
  isshow  = active;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::ShowCursor(bool active)
* @brief      Show cursor
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::ShowCursor(bool active)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPSCREEN::GetHandle()
* @brief      Get handle
* @ingroup    GRAPHIC
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPSCREEN::GetHandle()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::SetHandle(void* handle_window)
* @brief      Set handle
* @ingroup    GRAPHIC
* 
* @param[in]  handle_window : Handle window pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetHandle(void* handle_window)
{
  return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Set_Focus()
* @brief      Set focus
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Set_Focus()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Minimize(bool active)
* @brief      Minimize
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Minimize(bool active)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::Maximize(bool active)
* @brief      Maximize
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Maximize(bool active)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPSCREEN::CaptureContent(GRPRECTINT* rect, void* handle_window)
* @brief      Capture content
* @ingroup    GRAPHIC
* 
* @param[in]  rect : Rectangle to use.
* @param[in]  handle_window : Handle window pointer to use.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPSCREEN::CaptureContent(GRPRECTINT* rect, void* handle_window)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPSCREEN::GetDesktopHandle()
* @brief      Get desktop handle
* @ingroup    GRAPHIC
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPSCREEN::GetDesktopHandle()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* GRPSCREEN::GetShellHandle()
* @brief      Get shell handle
* @ingroup    GRAPHIC
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* GRPSCREEN::GetShellHandle()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::CanClose()
* @brief      can close
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::CanClose()
{
  return canclose;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::SetCanClose(bool canclose)
* @brief      set can close
* @ingroup    GRAPHIC
* 
* @param[in]  canclose : Canclose value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetCanClose(bool canclose)
{
  this->canclose = canclose;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPSCREEN::GetTitle()
* @brief      Get title
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPSCREEN::GetTitle()
{
  return &title;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENCFGCHROMES* GRPSCREEN::GetCFGChromes()
* @brief      Get CFG chromes
* @ingroup    GRAPHIC
* 
* @return     GRPSCREENCFGCHROMES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENCFGCHROMES* GRPSCREEN::GetCFGChromes()
{
  return &cfgchromes;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::SetCFGChromes(GRPSCREENCFGCHROMES& cfgchromes)
* @brief      Set CFG chromes
* @ingroup    GRAPHIC
* 
* @param[in]  cfgchromes : Chromes configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::SetCFGChromes(GRPSCREENCFGCHROMES& cfgchromes)
{
  if(!this->cfgchromes.CopyFrom(cfgchromes)) return false;

  cfgchromesactive = true;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  // Auto-hide enabled: start with the caption HIDDEN. Showing it from frame 0 painted the chrome over the
  // top band and ChromeCaption_SuppressesContentDraw froze ListBoxMenu / edit until the bar auto-hid
  // (UI_Options startup: native-looking chrome first, menu+edit missing). Show only when the cursor enters
  // the caption area (UpdateCFGChromesAutoHide — show is instantaneous).
  if(!cfgchromes.GetUseNativeChromes() && cfgchromes.GetCustomAutoHide() > 0)
    {
      cfgchromesautohidevisible = false;
      cfgchromesautohidedesired = false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsCFGChromesActive()
* @brief      Is CFG chromes active
* @ingroup    GRAPHIC
* 
* @return     bool : true if a chromes configuration has been assigned to the screen; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsCFGChromesActive()
{
  return cfgchromesactive;
}


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

// Forward declaration: defined later in this file, before UpdateCFGChromesAutoHide.
static void GRPSCREEN_SetElementVisibleRecursive(UI_ELEMENT* element, bool visible);

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::LoadCFGChromesLayout()
* @brief      Load CFG chromes layout
* @note       Resolves and loads the .xml (or .zip bundle) + layout named in GetCFGChromes(), and keeps a weak
*             reference to it (see GetCFGChromesLayout()). A no-op (returns false) when there is no active
*             Chromes configuration, when native chromes are requested (nothing to load, by design), when the
*             screen has no viewport/canvas yet (call this once the native window is created), or when the
*             file/layout name have not been set.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::LoadCFGChromesLayout()
{
  if(!IsCFGChromesActive())               return false;
  if(cfgchromes.GetUseNativeChromes())    return false;

  GRPVIEWPORT* viewport = GetViewport(0);
  if(!viewport)                           return false;
  if(!viewport->GetCanvas())              return false;

  XSTRING* namelayoutfile = cfgchromes.GetCustomLayoutFile();
  XSTRING* namelayout     = cfgchromes.GetCustomLayoutName();

  if(!namelayoutfile || namelayoutfile->IsEmpty())  return false;
  if(!namelayout      || namelayout->IsEmpty())     return false;

  // Already loaded (e.g. shared with other layouts/layers that loaded this same file earlier): just reuse it.
  cfgchromeslayout = GEN_USERINTERFACE.Layouts_Get(namelayout->Get());

  if(!cfgchromeslayout)
    {
      // namelayoutfile is already a full, resolved path (either the class' own default -- see
      // GRPSCREENCFGCHROMES::Clean() -- or whatever the app itself set via SetCustomLayoutFile()); no section
      // path gets prepended here. UI_MANAGER::Load() already tells a plain .xml apart from a compressed .zip
      // bundle by extension.
      XPATH pathfile;

      pathfile = namelayoutfile->Get();

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Chromes] loading custom layout: file [%s] layout [%s]"), namelayoutfile->Get(), namelayout->Get());

      if(!GEN_USERINTERFACE.Load(pathfile, this, 0))
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Chromes] ERROR: custom layout file [%s] could NOT be loaded -> NO custom chrome will be drawn (see [UI Load] trace above for the exact reason)"), namelayoutfile->Get());
          return false;
        }

      cfgchromeslayout = GEN_USERINTERFACE.Layouts_Get(namelayout->Get());
    }

  if(!cfgchromeslayout)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Chromes] ERROR: layout named [%s] not found inside [%s] -> NO custom chrome will be drawn (check the <layout name=...> attribute in the XML)"), namelayout->Get(), namelayoutfile->Get());
      return false;
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Chromes] custom layout [%s] loaded OK"), namelayout->Get());

  // The layout's own role="title" element (if any) keeps whatever static text it was authored with in the
  // .xml; once custom chromes are active, this screen's own title (GetTitle(), the same string a native
  // caption would show) is what must actually be displayed, always -- so it is pushed into that element here,
  // every time this function runs (fresh load or an already-loaded layout shared with something else, e.g. a
  // different screen with a different title of its own).
  UI_ELEMENT* titleelement = cfgchromeslayout->Elements_Get(UI_ELEMENT_CHROMEROLE_TITLE);
  if(titleelement && (titleelement->GetType() == UI_ELEMENT_TYPE_TEXT))
    {
      ((UI_ELEMENT_TEXT*)titleelement)->GetText()->Set(GetTitle()->Get());
    }

  // Sync the caption element's actual visibility to the initial desired state. When CustomAutoHide > 0,
  // SetCFGChromes() already set cfgchromesautohidevisible = false so the first frames show layout content
  // (menu / edit) without the caption band. When auto-hide is disabled, the default remains visible.
  UI_ELEMENT* captionelement = cfgchromeslayout->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
  if(captionelement)
    {
      GRPSCREEN_SetElementVisibleRecursive(captionelement, cfgchromesautohidevisible);
    }

  // Reference width for UpdateCFGChromesButtonsPosition(): the width the layout's element positions were just
  // resolved against. The min/max/close buttons keep their authored distance to the RIGHT edge of this width;
  // when the window is later resized they are shifted by (live client width - this reference) so they hug
  // the live right edge — both when shrinking and when enlarging / maximizing. The caption bar width is
  // also kept equal to the live client width in that same update.
  //
  // That reference is the CAPTION's own resolved width, not GetWidth(): a chrome layout no longer has to
  // hardcode a width at all (UI_SKINCANVAS::CalculateBoundaryLine_Form resolves a role="caption" form with no
  // authored width to the full canvas width), and the caption is by definition the box the buttons were
  // aligned inside. The two coincide in the ordinary one-viewport case; reading the caption keeps them
  // consistent when they do not -- and when a layout DOES author an explicit width, that authored value is
  // what the buttons were positioned against, so it is the right reference too. GetWidth() stays as the
  // fallback for a layout with no caption element.
  cfgchromesbuttonsrefwidth = (int)GetWidth();

  if(captionelement && (captionelement->GetBoundaryLine()->width > 0.0))
    {
      cfgchromesbuttonsrefwidth = (int)captionelement->GetBoundaryLine()->width;
    }

  cfgchromesbuttonsshift = 0;

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Chromes] caption width resolved to [%d] (buttons reference width)"), cfgchromesbuttonsrefwidth);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* GRPSCREEN::GetCFGChromesLayout()
* @brief      Get CFG chromes layout
* @ingroup    GRAPHIC
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* GRPSCREEN::GetCFGChromesLayout()
{
  return cfgchromeslayout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateCFGChromesDrag()
* @brief      Update CFG chromes drag
* @note       Reads GEN_INPMANAGER's mouse device directly (raw cursor position + raw button state) -- not the
*             UserInterface event pipeline, and not whatever input polling the running application does for its
*             own purposes; those stay entirely separate.
*
*             On the frame the button GOES DOWN (the PRESSED edge, not the held level) the layout's
*             role="caption" element is hit-tested in client space, excluding the minimize/maximize/close
*             elements that live inside it. From then on, and until the button is released, the window is moved
*             by exactly the delta the cursor has travelled in DESKTOP (screen/root) coordinates since that
*             moment:
*
*                 newpos = windowpos_at_drag_start + (cursor_desktop_now - cursor_desktop_at_drag_start)
*
*             The reference frame is the desktop, which the drag does not move. This is the whole point of the
*             rewrite: the previous version reconstructed the desktop cursor position as
*             "client_relative_cursor + GetPositionX()", which measured the input in the very frame of
*             reference the algorithm was moving. Any delay between the two readings (one full render frame,
*             since the drag tick runs after DrawFrame(); plus, on X11, the asynchronous round trip of
*             XMoveWindow() through the window manager, which leaves GetPositionX() holding a REQUESTED
*             position while the client-relative reading refers to the REAL one) turned straight into a
*             position error: the window lagged behind the pointer and, on X11, could overshoot it.
*
*             It also no longer aborts when the cursor sits outside the client area. The platform pointer is
*             captured for the duration of the drag (BeginCFGChromesDrag()), so it keeps being reported
*             wherever it goes; and even if a platform cannot capture it, GetCursorDesktopPosition() is a
*             desktop-wide reading that does not depend on the cursor being over this window. The old
*             "skip this frame" guard could not recover on its own -- a frozen window can never catch up with
*             a pointer that has already left it -- which is what left the window stopped mid-drag.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateCFGChromesDrag()
{
  if(!IsCFGChromesActive())            return false;
  if(cfgchromes.GetUseNativeChromes()) return false;
  if(!cfgchromeslayout)                return false;

  INPDEVICE* mousedevice = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(!mousedevice) return false;

  INPBUTTON* inpbutton = mousedevice->GetButton(INPBUTTON_ID_MOUSE_LEFT);
  if(!inpbutton) return false;

  // Press EDGE detected here, from this class' own bookkeeping, instead of asking the button for
  // INPBUTTON_STATE_PRESSED. The per-platform input devices age that state on their own schedule (the X11
  // device, for one, promotes PRESSED to HOLD at the top of its next Update()), so how long the edge stays
  // visible depends on where in the frame this tick happens to run -- and this tick now runs from two places.
  // A level read plus one bool is not subject to any of that.
  bool ispressed  = inpbutton->IsPressed();
  bool waspressed = cfgchromesdragbuttonwaspressed;

  cfgchromesdragbuttonwaspressed = ispressed;

  if(!ispressed)
    {
      if(cfgchromesdragging)
        {
          EndCFGChromesDrag();

          GRPSCREEN_CHROMESDRAGTRACE(__L("[Chromes drag] END: window (%d,%d)"), GetPositionX(), GetPositionY());
        }

      cfgchromesdragging   = false;
      cfgchromesdragmoving = false;

      return true;
    }

  int desktopcursorx = 0;
  int desktopcursory = 0;

  if(!cfgchromesdragging)
    {
      // A button that was ALREADY down on the previous tick never starts a drag: otherwise a press that began
      // somewhere else, or one made while the window was not focused, would grab the window as soon as the
      // pointer happened to travel over the caption.
      if(waspressed) return false;

      int uix = 0;
      int uiy = 0;

      if(!GetCFGChromesCursorPosition(uix, uiy))                  return false;

      UI_ELEMENT* captionelement = cfgchromeslayout->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
      if(!captionelement)                                         return false;
      if(!captionelement->IsActive() || !captionelement->IsVisible()) return false;

      UI_BOUNDARYLINE bline;

      bline.x      = captionelement->GetXPosition();
      bline.y      = captionelement->GetYPosition();
      bline.width  = captionelement->GetBoundaryLine()->width;
      bline.height = captionelement->GetBoundaryLine()->height;

      if(!bline.IsWithin(uix, uiy))                               return false;

      // The minimize/maximize/close elements are laid out INSIDE the caption's own area (see any
      // role="caption" layout: they are compose elements of it), so a press on any of them also passes the
      // test above. They must not start a window move.
      if(IsOverCFGChromesButton(uix, uiy))
        {
          GRPSCREEN_CHROMESDRAGTRACE(__L("[Chromes drag] press at UI (%d,%d) landed on a window button: no drag"), uix, uiy);
          return false;
        }

      if(!GetCursorDesktopPosition(desktopcursorx, desktopcursory))
        {
          GRPSCREEN_CHROMESDRAGTRACE(__L("[Chromes drag] ERROR: GetCursorDesktopPosition() not available on this backend -> the caption drag CANNOT run"));
          return false;
        }

      int anchorx = 0;
      int anchory = 0;

      if(!GetCFGChromesDragAnchor(anchorx, anchory))
        {
          anchorx = GetPositionX();
          anchory = GetPositionY();
        }

      cfgchromesdragging         = true;
      cfgchromesdragmoving       = false;
      cfgchromesdragstartscreenx = anchorx;
      cfgchromesdragstartscreeny = anchory;
      cfgchromesdragstartcursorx = desktopcursorx;
      cfgchromesdragstartcursory = desktopcursory;
      cfgchromesdraglastposx     = anchorx;
      cfgchromesdraglastposy     = anchory;

      #ifndef GRPSCREEN_CFGCHROMES_NOSYSTEMMOVE
      // Preferred path: let the window manager run the move. Everything above still had to happen (the press
      // must be identified as landing on the caption before anything is handed over), but from here on the
      // in-process loop is not used at all -- SystemMove() only returns true when the platform really took
      // over, so a false keeps the previous behaviour byte for byte.
      if(SystemMove())
        {
          cfgchromesdragging   = false;
          cfgchromesdragmoving = false;

          GRPSCREEN_CHROMESDRAGTRACE(__L("[Chromes drag] START: handed over to the window manager (SystemMove)"));

          return true;
        }
      #endif

      BeginCFGChromesDrag();

      GRPSCREEN_CHROMESDRAGTRACE(__L("[Chromes drag] START: in-process. window (%d,%d) cursor desktop (%d,%d) UI (%d,%d)"), anchorx, anchory, desktopcursorx, desktopcursory, uix, uiy);

      return true;
    }

  // A failed reading here is a genuine platform failure, not "the cursor left the window": the position is
  // read desktop-wide. Hold still for this tick; the next valid reading recomputes the ABSOLUTE target, so
  // nothing is lost and nothing accumulates.
  if(!GetCursorDesktopPosition(desktopcursorx, desktopcursory)) return true;

  int deltax = desktopcursorx - cfgchromesdragstartcursorx;
  int deltay = desktopcursory - cfgchromesdragstartcursory;

  if(!cfgchromesdragmoving)
    {
      int absdeltax = (deltax < 0)?-deltax:deltax;
      int absdeltay = (deltay < 0)?-deltay:deltay;

      if((absdeltax < GRPSCREEN_CFGCHROMES_DRAGDEADZONE) && (absdeltay < GRPSCREEN_CFGCHROMES_DRAGDEADZONE))
        {
          return true;
        }

      cfgchromesdragmoving = true;
    }

  int newpositionx = cfgchromesdragstartscreenx + deltax;
  int newpositiony = cfgchromesdragstartscreeny + deltay;

  // Compared against what WE last asked for, never against GetPositionX()/Y(). Those track the window's real
  // position, which lags the request (X11 moves are asynchronous and go through the window manager) and on
  // the X11 backend is re-read from the server once per rendered frame -- comparing against it would skip
  // moves that had not landed yet. Besides saving a system call, this is what makes it harmless for this tick
  // to run more than once per main-loop iteration: the early one from UpdateAllCFGChromesDrag() plus the
  // legacy one from UpdateViewports().
  if((newpositionx == cfgchromesdraglastposx) && (newpositiony == cfgchromesdraglastposy))
    {
      return true;
    }

  cfgchromesdraglastposx = newpositionx;
  cfgchromesdraglastposy = newpositiony;

  bool moved = Set_Position(newpositionx, newpositiony);

  (void)moved;   // only read by the trace below, which compiles away unless diagnostics are enabled

  GRPSCREEN_CHROMESDRAGTRACEMOVE(__L("[Chromes drag] MOVE to (%d,%d) delta (%d,%d) Set_Position=%s real (%d,%d)"), newpositionx, newpositiony, deltax, deltay, moved?__L("ok"):__L("FAILED"), GetPositionX(), GetPositionY());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsCFGChromesDragging()
* @brief      Is CFG chromes dragging: true while the user holds the button down on the custom caption.
* @ingroup    GRAPHIC
* 
* @return     bool : true if a caption drag is currently in progress.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsCFGChromesDragging()
{
  return cfgchromesdragging;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateAllCFGChromesDrag()
* @brief      Update all CFG chromes drag
* @note       STATIC. Runs the caption-drag tick on every screen currently registered in GetListScreens().
*             Called by the platform main loop immediately after the input devices are updated, so the window
*             follows the pointer with the freshest reading available instead of one whole rendered frame
*             later (UpdateViewports() runs after the application's DrawFrame()).
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateAllCFGChromesDrag()
{
  for(XDWORD c=0; c<allscreens.GetSize(); c++)
    {
      GRPSCREEN* screen = allscreens.Get(c);
      if(screen)
        {
          screen->UpdateCFGChromesDrag();
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::IsOverCFGChromesButton(int uix, int uiy)
* @brief      Is over CFG chromes button
* @note       INTERNAL: true when the given UI-space point falls on the layout's role="minimize"/"maximize"/
*             "close" element (or on any of its compose elements, e.g. the graphic of a multioption). Used to
*             keep a press on a window button from also starting a caption drag.
* @ingroup    GRAPHIC
* 
* @param[in]  uix : X coordinate, UI-space (top-down, client-relative).
* @param[in]  uiy : Y coordinate, UI-space (top-down, client-relative).
* 
* @return     bool : true if the point is over one of the caption's window buttons.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsOverCFGChromesButton(int uix, int uiy)
{
  if(!cfgchromeslayout) return false;

  UI_ELEMENT_CHROMEROLE roles[3] = { UI_ELEMENT_CHROMEROLE_MINIMIZE,
                                     UI_ELEMENT_CHROMEROLE_MAXIMIZE,
                                     UI_ELEMENT_CHROMEROLE_CLOSE    };

  for(int c=0; c<3; c++)
    {
      UI_ELEMENT* element = cfgchromeslayout->Elements_Get(roles[c]);
      if(!element)                              continue;
      if(!element->IsActive())                  continue;
      if(!element->IsVisible())                 continue;

      UI_BOUNDARYLINE bline;

      bline.x      = element->GetXPosition();
      bline.y      = element->GetYPosition();
      bline.width  = element->GetBoundaryLine()->width;
      bline.height = element->GetBoundaryLine()->height;

      // A degenerate boundary (a role wired to an element that never got a size) must not be able to veto
      // the drag over an arbitrary area: ignore it and let the caption keep the press.
      if((bline.width <= 0) || (bline.height <= 0)) continue;

      if(bline.IsWithin(uix, uiy)) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::GetCursorDesktopPosition(int& x, int& y)
* @brief      Get cursor desktop position
* @note       VIRTUAL. Base implementation: not available. Each platform screen that can report the pointer in
*             desktop (screen/root) coordinates overrides this -- see GRPWINDOWSSCREEN (GetCursorPos) and
*             GRPLINUXSCREENX11 (XQueryPointer against the root window). Where it is not available the custom
*             caption drag simply never starts, which is the previous behaviour on those platforms anyway.
* @ingroup    GRAPHIC
* 
* @param[out] x : X coordinate, desktop space.
* @param[out] y : Y coordinate, desktop space.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::GetCursorDesktopPosition(int& x, int& y)
{
  x = 0;
  y = 0;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::SystemMove()
* @brief      System move
* @note       VIRTUAL. Base implementation: not available, so the in-process drag runs. Overridden where the
*             platform offers a "the window manager moves the window from here" request -- X11's EWMH
*             _NET_WM_MOVERESIZE for one. MUST return false, having done nothing, whenever the request could
*             not actually be issued: the caller relies on that to fall back cleanly.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the platform took over the move; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::SystemMove()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::GetCFGChromesDragAnchor(int& x, int& y)
* @brief      Get CFG chromes drag anchor
* @note       VIRTUAL. Base implementation: not available, so UpdateCFGChromesDrag() falls back to the stored
*             GetPositionX()/GetPositionY(). That is exactly right for the Windows backend, whose WM_MOVE
*             handler keeps those members equal to the real position at all times. Backends where the stored
*             position can lag reality override this with a live query.
* @ingroup    GRAPHIC
* 
* @param[out] x : X coordinate, in the space Set_Position() consumes.
* @param[out] y : Y coordinate, in the space Set_Position() consumes.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::GetCFGChromesDragAnchor(int& x, int& y)
{
  x = 0;
  y = 0;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::BeginCFGChromesDrag()
* @brief      Begin CFG chromes drag
* @note       VIRTUAL. Base implementation: nothing to do. Platforms override it to capture/grab the pointer so
*             it keeps being reported to this window while the drag is in progress even if it travels outside
*             the window's own area.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::BeginCFGChromesDrag()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::EndCFGChromesDrag()
* @brief      End CFG chromes drag
* @note       VIRTUAL. Base implementation: nothing to do. Counterpart of BeginCFGChromesDrag(); releases the
*             pointer capture/grab. MUST be safe to call when no capture was ever taken.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::EndCFGChromesDrag()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::GetCFGChromesCursorPosition(int& uix, int& uiy)
* @brief      Get CFG chromes cursor position
* @note       INTERNAL: reads GEN_INPMANAGER's mouse device directly (raw, client-relative, bottom-up -- 0 at the
*             bottom of the client area, increasing upward, confirmed by tracing INPWINDOWSDEVICEMOUSE::Update())
*             and converts it into the same top-down UI-space coordinates UI_ELEMENT's own
*             GetXPosition()/GetYPosition() use. Returns false (uix/uiy left untouched) when the cursor is
*             currently outside this window's client area (the device reports (-1,-1) in that case) or the
*             mouse device itself is unavailable. Shared by UpdateCFGChromesDrag() and UpdateCFGChromesAutoHide().
* @ingroup    GRAPHIC
* 
* @param[out] uix : X coordinate, UI-space (top-down, client-relative).
* @param[out] uiy : Y coordinate, UI-space (top-down, client-relative).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::GetCFGChromesCursorPosition(int& uix, int& uiy)
{
  INPDEVICE* mousedevice = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(!mousedevice) return false;

  INPCURSOR* inpcursor = mousedevice->GetCursor(INPCURSOR_ID_MOUSE);
  if(!inpcursor) return false;

  int rawx = (int)inpcursor->GetX();
  int rawy = (int)inpcursor->GetY();

  if((rawx == -1) && (rawy == -1)) return false;

  uix = rawx;
  uiy = GetHeight() - rawy;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static void GRPSCREEN_SetElementVisibleRecursive(UI_ELEMENT* element, bool visible)
* @brief      INTERNAL: sets an element and every one of its compose (child) elements visible/invisible.
*             UI_ELEMENT::SetVisible() only ever affects the element itself, so hiding just the caption's own
*             "form" element would leave its icon/title/buttons drawn on their own.
* @ingroup    GRAPHIC
* 
* @param[in]  element : Pointer to element.
* @param[in]  visible : Visible value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void GRPSCREEN_SetElementVisibleRecursive(UI_ELEMENT* element, bool visible)
{
  if(!element) return;

  element->SetVisible(visible);

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      GRPSCREEN_SetElementVisibleRecursive(element->GetComposeElements()->Get(c), visible);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void GRPSCREEN_ShiftElementXRecursive(UI_ELEMENT* element, double dx)
* @brief      INTERNAL: shifts an element and every one of its compose (child) elements horizontally by dx.
*             Child element positions are stored ABSOLUTE (the skin adds the father's position at layout time,
*             see UI_SKINCANVAS::SetAroundFromSubElements()/CalculePosition()), so moving just the button's own
*             element would leave its icon/graphic drawn at the old spot.
* @ingroup    GRAPHIC
* 
* @param[in]  element : Pointer to element.
* @param[in]  dx : Dx value.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void GRPSCREEN_ShiftElementXRecursive(UI_ELEMENT* element, double dx)
{
  if(!element) return;

  element->SetXPosition(element->GetXPosition() + dx);

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      GRPSCREEN_ShiftElementXRecursive(element->GetComposeElements()->Get(c), dx);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateCFGChromesAutoHide()
* @brief      Update CFG chromes auto hide
* @note       Hides the layout's role="caption" element (and everything inside it: icon, title, min/max/close)
*             when the cursor has stayed steadily outside its own area for GetCFGChromes()->GetCustomAutoHide()
*             milliseconds -- that parameter is the HIDE delay only. Showing is INSTANTANEOUS: the moment the
*             cursor enters the caption area the bar is made visible again, with no debounce at all. The hide
*             timer resets every time the cursor moves in or out, so a quick pass near the caption's edge never
*             hides it by itself. When auto-hide is 0 (disabled, the default) the caption is simply always
*             visible.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateCFGChromesAutoHide()
{
  if(!IsCFGChromesActive())            return false;
  if(cfgchromes.GetUseNativeChromes()) return false;
  if(!cfgchromeslayout)                return false;

  UI_ELEMENT* captionelement = cfgchromeslayout->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
  if(!captionelement) return false;

  XDWORD timehidden = cfgchromes.GetCustomAutoHide();
  if(!timehidden)
    {
      // Auto-hide disabled: caption is always visible. LoadCFGChromesLayout()
      // already forced it visible once; this guard keeps cfgchromesautohidevisible
      // consistent so that if auto-hide is re-enabled at runtime the timer logic
      // starts from a known-good state.
      if(!cfgchromesautohidevisible)
        {
          cfgchromesautohidevisible = true;
          GRPSCREEN_SetElementVisibleRecursive(captionelement, true);
        }
      return true;
    }

  if(!cfgchromesautohidetimer)
    {
      cfgchromesautohidetimer = GEN_XFACTORY.CreateTimer();
      if(!cfgchromesautohidetimer) return false;

      cfgchromesautohidetimer->Reset();
    }

  int  uix             = 0;
  int  uiy             = 0;
  bool cursoravailable = GetCFGChromesCursorPosition(uix, uiy);

  UI_BOUNDARYLINE bline;

  bline.x      = captionelement->GetXPosition();
  bline.y      = captionelement->GetYPosition();
  bline.width  = captionelement->GetBoundaryLine()->width;
  bline.height = captionelement->GetBoundaryLine()->height;

  // When cursor position is NOT available (cursor outside the window, or the
  // input device hasn't been updated yet because the window doesn't have
  // keyboard focus), preserve the current desired state rather than treating
  // it as "cursor away from chrome". This prevents the caption being hidden
  // on KDE native X11 simply because the WM hasn't granted focus yet.
  bool cursorover;
  if(cursoravailable)
    {
      cursorover = bline.IsWithin(uix, uiy);
    }
  else
    {
      cursorover = cfgchromesautohidedesired;
    }

  if(cursorover != cfgchromesautohidedesired)
    {
      cfgchromesautohidedesired = cursorover;
      cfgchromesautohidetimer->Reset();
    }

  if(cfgchromesautohidedesired != cfgchromesautohidevisible)
    {
      // Asymmetric by design: SHOWING is instantaneous (the instant the cursor touches the caption area the
      // bar pops in, no debounce), while HIDING only happens after the cursor has stayed steadily away from
      // the caption for GetCustomAutoHide() milliseconds -- that parameter is a HIDE delay only.
      if(cfgchromesautohidedesired || (cfgchromesautohidetimer->GetMeasureMilliSeconds() >= timehidden))
        {
          bool becomingvisible = cfgchromesautohidedesired;

          UI_SKIN*       skin        = cfgchromeslayout ? cfgchromeslayout->GetSkin() : NULL;
          UI_SKINCANVAS* skin_canvas = NULL;
          if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
            {
              skin_canvas = (UI_SKINCANVAS*)skin;
            }

          if(!becomingvisible)
            {
              // HIDE: erase the caption band NOW while the formbackdrop cache is still valid.
              // BUG (2026-09): invalidating caches BEFORE RestoreOnHide destroyed the only pixels that could
              // cleanly remove the bar; Draw()'s later RestoreOnHide found nothing, so icon/buttons ghosts
              // stayed over the top menu (UI_Options ListBoxMenu overlaps y~0..46). Restore while still
              // "logically visible", then mark invisible, then drop caches so Draw does not blit twice.
              if(skin) skin->RestoreOnHide(captionelement);

              cfgchromesautohidevisible = false;
              GRPSCREEN_SetElementVisibleRecursive(captionelement, false);

              if(skin_canvas) skin_canvas->InvalidateCompositionCaches();

              // Content under the band must repaint (absolute options menu sits in the caption strip).
              GEN_USERINTERFACE.Elements_SetToRedrawForScreen(this, true /* exclude_chrome */);
            }
           else
            {
              // SHOW: content redraws first next frame, then chrome Draw_Form re-captures under the bar.
              cfgchromesautohidevisible = true;
              GRPSCREEN_SetElementVisibleRecursive(captionelement, true);

              if(skin_canvas) skin_canvas->InvalidateCompositionCaches();

              GEN_USERINTERFACE.Elements_SetToRedrawForScreen(this, true /* exclude_chrome */);
              if(cfgchromeslayout) cfgchromeslayout->Elements_SetToRedraw();
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::UpdateCFGChromesButtonsPosition()
* @brief      Update CFG chromes buttons position
* @note       Keeps the chrome caption + minimize/maximize/close buttons locked to the LIVE client size:
*             the caption bar width tracks the client width, and the three buttons shift by
*             (client width - reference width) so they hug the live right edge (shrink and grow).
*             Any client size change — including height-only (resize from the top/bottom edge) — also
*             dirties the chrome layout: UpdateSize recreates empty canvases, and without a redraw the
*             caption band disappears until something else happens to invalidate it.
* @ingroup    GRAPHIC
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateCFGChromesButtonsPosition()
{
  if(!IsCFGChromesActive())            return false;
  if(cfgchromes.GetUseNativeChromes()) return false;
  if(!cfgchromeslayout)                return false;
  if(!cfgchromesbuttonsrefwidth)       return false;

  // LIVE native client size (platform virtual) -- NOT GetWidth()/GetHeight() alone in all backends.
  int clientwidth  = 0;
  int clientheight = 0;

  if(!GetClientSize(clientwidth, clientheight)) return false;

  // Ignore a client size the window manager cannot have meant. During the first frames the native window may
  // still be unmapped / mid-creation and report a degenerate size (0 on X11 while the WM has not configured
  // the frame yet, or a stale default on Windows between CreateWindow and the first WM_SIZE). Acting on it
  // would yank all three buttons far to the left for a frame or two and then snap them back -- and because
  // the canvas is only repainted through rebuild areas, the pixels they left behind at the old position are
  // not erased, so those frames smear the top of the window. Waiting one frame for a real size costs nothing.
  if(clientwidth <= 0 || clientheight <= 0) return false;

  bool size_changed = ((clientwidth != cfgchromeslastclientwidth) || (clientheight != cfgchromeslastclientheight));
  cfgchromeslastclientwidth  = clientwidth;
  cfgchromeslastclientheight = clientheight;

  // Caption bar IS the window width: keep its AABB in sync with the live client so enlarge/maximize paints
  // the full band (authored XML width / load-time canvas size is only the initial reference).
  bool caption_resized = false;
  UI_ELEMENT* captionelement = cfgchromeslayout->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
  if(captionelement)
    {
      double captionwidth = captionelement->GetBoundaryLine()->width;
      if(captionwidth != (double)clientwidth)
        {
          captionelement->GetBoundaryLine()->width = (double)clientwidth;
          UI_ELEMENT_FORM* captionform = dynamic_cast<UI_ELEMENT_FORM*>(captionelement);
          if(captionform && captionform->GetVisibleRect() && (captionform->GetVisibleRect()->width > 0.0))
            {
              captionform->GetVisibleRect()->width = (double)clientwidth;
            }
          caption_resized = true;
        }
    }

  // Bidirectional: negative when narrower than ref, positive when wider (maximize / enlarge).
  int shift = clientwidth - cfgchromesbuttonsrefwidth;

  if(shift != cfgchromesbuttonsshift)
    {
      double dx = (double)(shift - cfgchromesbuttonsshift);

      UI_ELEMENT_CHROMEROLE roles[3] = { UI_ELEMENT_CHROMEROLE_MINIMIZE,
                                         UI_ELEMENT_CHROMEROLE_MAXIMIZE,
                                         UI_ELEMENT_CHROMEROLE_CLOSE    };

      for(int c=0; c<3; c++)
        {
          UI_ELEMENT* buttonelement = cfgchromeslayout->Elements_Get(roles[c]);
          if(buttonelement)
            {
              GRPSCREEN_ShiftElementXRecursive(buttonelement, dx);
            }
        }

      cfgchromesbuttonsshift = shift;
      size_changed = true;   // buttons moved → must repaint
    }

  if(!size_changed && !caption_resized) return true;

  // Height-only (or any) resize after UpdateSize wiped the live canvas: drop chrome FormBackdrops so the
  // next Draw_Form re-captures under the new size, and mark the whole chrome layout dirty.
  UI_SKIN* skin = cfgchromeslayout->GetSkin();
  if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
    {
      ((UI_SKINCANVAS*)skin)->InvalidateCompositionCaches();
    }

  cfgchromeslayout->Elements_SetToRedraw();

  return true;
}


#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateSize(int width, int height)
* @brief      Update Size
* @ingroup    GRAPHIC
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateSize(int width, int height)
{
  if(width  < 1) width  = 1;
  if(height < 1) height = 1;

  bool changed = (((XDWORD)width != GetWidth()) || ((XDWORD)height != GetHeight()));
  if(changed)
    {
      SetSize((XDWORD)width, (XDWORD)height);

      if(screencanvas)
        {
          screencanvas->Buffer_Delete();
          screencanvas->SetWidth((XDWORD)width);
          screencanvas->SetHeight((XDWORD)height);
          screencanvas->Buffer_Create();
        }

      GRPVIEWPORT* viewport = GetViewport(0);
      if(viewport)
        {
          viewport->SetSize((float)width, (float)height);
          GRP2DCANVAS* canvas = viewport->GetCanvas();
          if(canvas)
            {
              canvas->Buffer_Delete();
              canvas->SetWidth((XDWORD)width);
              canvas->SetHeight((XDWORD)height);
              canvas->Buffer_Create();
            }
        }

      #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
      // BEFORE CHANGESIZE observers run Update(): recreate wiped the live canvas. Height-only
      // resizes (drag top/bottom edge) never moved the chrome buttons, so the chrome layout used
      // to stay clean and never repainted — caption vanished until a later width change. Sync
      // caption/buttons and dirty chrome here so the imminent UI Update() redraws the bar.
      if(IsCFGChromesActive() && !cfgchromes.GetUseNativeChromes() && cfgchromeslayout)
        {
          UpdateCFGChromesButtonsPosition();
        }
      #endif
    }

  GRPXEVENT grpevent(this, GRPXEVENT_TYPE_SCREEN_CHANGESIZE);
  grpevent.SetScreen(this);

  PostEvent(&grpevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPVIEWPORT*>* GRPSCREEN::GetViewports()
* @brief      Get viewports
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<GRPVIEWPORT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVIEWPORT*>* GRPSCREEN::GetViewports()
{
  return &viewports;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIEWPORT* GRPSCREEN::GetViewport(int index)
* @brief      Get viewport
* @ingroup    GRAPHIC
* 
* @param[in]  index : Index value.
* 
* @return     GRPVIEWPORT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT* GRPSCREEN::GetViewport(int index)
{
  return (viewports.Get(index));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIEWPORT* GRPSCREEN::GetViewport(XCHAR* ID)
* @brief      Get viewport
* @ingroup    GRAPHIC
* 
* @param[in]  ID : Identifier to use.
* 
* @return     GRPVIEWPORT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT* GRPSCREEN::GetViewport(XCHAR* ID)
{
  for(XDWORD c=0; c<viewports.GetSize(); c++)
    {
      GRPVIEWPORT* viewport = viewports.Get(c);
      if(viewport) 
        {
          if(!viewport->GetID()->Compare(ID, true)) return viewport;
        }                    
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::CreateViewport(XCHAR* ID, float posx, float posy, float width, float height, XDWORD canvasposx, XDWORD canvasposy, XDWORD canvaswidth, XDWORD canvasheight)
* @brief      Create viewport
* @ingroup    GRAPHIC
* 
* @param[in]  ID : Identifier to use.
* @param[in]  posx : Posx value.
* @param[in]  posy : Posy value.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* @param[in]  canvasposx : Canvasposx value.
* @param[in]  canvasposy : Canvasposy value.
* @param[in]  canvaswidth : Canvaswidth value.
* @param[in]  canvasheight : Canvasheight value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::CreateViewport(XCHAR* ID, float posx, float posy, float width, float height, XDWORD canvasposx, XDWORD canvasposy, XDWORD canvaswidth, XDWORD canvasheight)
{
  GRPVIEWPORT* viewport = GEN_NEW GRPVIEWPORT();
  if(!viewport)  return false;

  viewport->GetID()->Set(ID);
  viewport->SetPosition(posx, posy);
  viewport->SetSize(width, height);

  GRPPROPERTIES canvasproperties; 
  float         cwidth  = (float)canvaswidth;
  float         cheight = (float)canvasheight;

  if(width  > cwidth)  cwidth  = width;
  if(height > cheight) cheight = height;

  canvasproperties.CopyPropertysFrom(this);
  canvasproperties.SetPosition(canvasposx, canvasposy); 
  canvasproperties.SetSize((XDWORD)cwidth, (XDWORD)cheight);

  if(viewport->CreateCanvas(canvasproperties))
    {
      viewport->SetCanvasPosition(posx, posy);
      viewports.Add(viewport);

      #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
      
      if(viewports.GetSize() == 1) 
        {
          LoadCFGChromesLayout();
        }
      #endif

    } else return false;

  return true;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::UpdateViewports()
* @brief      Update Viewports
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateViewports()
{
  if(!screencanvas)
    {   
      return false;
    }

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  // NOTE: the drag tick also runs earlier in the frame, straight after the input devices are updated (see
  // UpdateAllCFGChromesDrag(), called from MAINPROCWINDOWS::Update()/MAINPROCLINUX::Update()). That is the
  // one that gives the window a fresh cursor reading instead of one a whole DrawFrame() old. This call is
  // kept so that a main loop which does not go through MAINPROC still gets the drag; running twice costs
  // nothing because the tick recomputes an absolute target and skips the platform call when it has not
  // changed.
  UpdateCFGChromesDrag();

  // ROOT CAUSE (UI_Options video 2026-09-19): AutoHide used to run HERE after the app's DrawFrame() had
  // already composed content → chrome → modal. Hiding then called RestoreOnHide() on the caption and wrote
  // straight into the canvas that was about to be presented — after Element_DrawModalOnTop — so ListBoxMenu /
  // title ghosts and keyboard punches came back in the same frame. Showing dirtied content without a second
  // compose before CopyBuffer. Re-compose when visibility actually flips, then present.
  bool chrome_was_visible = cfgchromesautohidevisible;
  UpdateCFGChromesAutoHide();
  bool chrome_toggled = (chrome_was_visible != cfgchromesautohidevisible);

  UpdateCFGChromesButtonsPosition();

  if(chrome_toggled)
    {
      // Drop modal_layer_valid for this recompose so ClearIntersectingContentDirt does not freeze
      // ListBoxMenu under the caption band — content must repaint after RestoreOnHide / show, then
      // Element_DrawModalOnTop rebuilds the keyboard on top in the same Update().
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      GEN_USERINTERFACE.Elements_RebuildDrawAreas();
      GEN_USERINTERFACE.Update();
    }
  #endif

  for(XDWORD c=0; c<viewports.GetSize(); c++)
    {      
      screencanvas->CopyBufferRenderFromViewport(viewports.Get(c));                               
    } 
       
  if(Style_Is(GRPSCREENSTYLE_TRANSPARENT))
    {
      UpdateTransparent(screencanvas);      
    }
    else
    {
      Update(screencanvas);      
    }    

  if(framerate) framerate->AddNFrames();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::DeleteViewport(GRPVIEWPORT* viewport)
* @brief      Delete viewport
* @ingroup    GRAPHIC
* 
* @param[in]  viewport : Viewport pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::DeleteViewport(GRPVIEWPORT* viewport)
{
  if(!viewport) 
    {
      return false;
    }

  viewports.Delete(viewport);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::DeleteAllViewports()
* @brief      Delete all viewports
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::DeleteAllViewports()
{
  if(viewports.IsEmpty()) 
    {
      return false;
    }

  viewports.DeleteContents();
  viewports.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPFRAMERATE* GRPSCREEN::GetFrameRate()
* @brief      Get frame rate
* @ingroup    GRAPHIC
* 
* @return     GRPFRAMERATE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPFRAMERATE* GRPSCREEN::GetFrameRate()
{
  return framerate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPDESKTOPMANAGER* GRPSCREEN::GetDesktopManager()
* @brief      Get desktop manager
* @ingroup    GRAPHIC
* 
* @return     GRPDESKTOPMANAGER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPDESKTOPMANAGER* GRPSCREEN::GetDesktopManager()
{
  return desktopmanager;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENTYPE_DESKTOP GRPSCREEN::GetDesktopScreenSelected()
* @brief      Get desktop screen selected
* @ingroup    GRAPHIC
* 
* @return     GRPSCREENTYPE_DESKTOP : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENTYPE_DESKTOP GRPSCREEN::GetDesktopScreenSelected()
{
  return desktopscreenselected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::SetDesktopScreenSelected(GRPSCREENTYPE_DESKTOP desktopscreenselected)
* @brief      Set desktop screen selected
* @ingroup    GRAPHIC
* 
* @param[in]  desktopscreenselected : Desktopscreenselected value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetDesktopScreenSelected(GRPSCREENTYPE_DESKTOP desktopscreenselected)
{
  this->desktopscreenselected = desktopscreenselected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMAP<void*, GRPSCREEN*>* GRPSCREEN::GetListScreens()
* @brief      Get list screens
* @ingroup    GRAPHIC
* 
* @return     XMAP<void*, GRPSCREEN*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMAP<void*, GRPSCREEN*>* GRPSCREEN::GetListScreens()
{
  return &listscreens;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS* GRPSCREEN::GetScreenCanvas()
* @brief      Get screen canvas
* @ingroup    GRAPHIC
* 
* @return     GRP2DCANVAS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS* GRPSCREEN::GetScreenCanvas()
{
  return screencanvas;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREEN::SetScreenCanvas(GRP2DCANVAS* screencanvas)
* @brief      Set screen canvas
* @ingroup    GRAPHIC
* 
* @param[in]  screencanvas : Screencanvas pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::SetScreenCanvas(GRP2DCANVAS* screencanvas)
{
  this->screencanvas = screencanvas;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREEN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::Clean()
{
  type                    = GRPSCREENTYPE_UNKNOW;

  isvalid                 = false;
  isactive                = false;

  styles                  = GRPSCREENSTYLE_DEFAULT;

  rotation                = GRPSCREENROTATION_NONE;
     
  framerate               = NULL; 

  screencanvas            = NULL;

  desktopmanager          = NULL;
  desktopscreenselected   = GRPSCREENTYPE_DESKTOP_ALL;

  isshow                  = false;

  canclose                = true;

  cfgchromesactive        = false;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  cfgchromeslayout        = NULL;

  cfgchromesdragging             = false;
  cfgchromesdragmoving           = false;
  cfgchromesdragbuttonwaspressed = false;
  cfgchromesdraglastposx         = 0;
  cfgchromesdraglastposy         = 0;
  cfgchromesdragstartcursorx = 0;
  cfgchromesdragstartcursory = 0;
  cfgchromesdragstartscreenx = 0;
  cfgchromesdragstartscreeny = 0;

  cfgchromesautohidevisible  = true;      // default when auto-hide is off; SetCFGChromes() forces false if AutoHide > 0
  cfgchromesautohidedesired  = true;
  cfgchromesautohidetimer    = NULL;      // created lazily, on first actual use (see UpdateCFGChromesAutoHide())

  cfgchromesbuttonsrefwidth  = 0;         // captured by LoadCFGChromesLayout(); 0 = repositioning inactive
  cfgchromesbuttonsshift     = 0;
  cfgchromeslastclientwidth  = 0;
  cfgchromeslastclientheight = 0;
  #endif
}


