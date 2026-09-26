/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Scrolleable.cpp
* 
* @class      UI_PROPERTY_SCROLLEABLE
* @brief      User Interface Property Scrolleable class
* @ingroup    USERINTERFACE
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

#include "UI_Property_Scrolleable.h"
#include "UI_Color.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SCROLLEABLE_STATUS::UI_PROPERTY_SCROLLEABLE_STATUS()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SCROLLEABLE_STATUS::UI_PROPERTY_SCROLLEABLE_STATUS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SCROLLEABLE_STATUS::~UI_PROPERTY_SCROLLEABLE_STATUS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SCROLLEABLE_STATUS::~UI_PROPERTY_SCROLLEABLE_STATUS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE_STATUS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE_STATUS::Clean()
{
  active        = false;
  visible       = false;
  activateoverride = UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO;
  limit         = 0.0f;        
  displacement  = 0.0f;    
  step          = 0.0f;      
  overflow      = UI_OVERFLOW_AUTO;

  barwidth      = (double)UI_PROPERTY_SCROLLEABLE_BARWIDTH;
  showbar       = true;

  thumbcolor[0] = 80;   thumbcolor[1] = 80;   thumbcolor[2] = 80;   thumbcolor[3] = 160;
  trackcolor[0] = 0;    trackcolor[1] = 0;    trackcolor[2] = 0;    trackcolor[3] = 30;

  sepedge       = 0.0f;
  sepalong      = 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SCROLLEABLE::UI_PROPERTY_SCROLLEABLE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SCROLLEABLE::UI_PROPERTY_SCROLLEABLE()    
{ 
  Clean(); 

  horizontal.limit    = -1;
  vertical.limit      = -1;
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SCROLLEABLE::~UI_PROPERTY_SCROLLEABLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SCROLLEABLE::~UI_PROPERTY_SCROLLEABLE()    
{   
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_GetActive(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll get active
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_GetActive(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  bool status = false;  

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :  
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :       
                                          default     : return false; 
                                                        break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : status = horizontal.active;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : status = vertical.active;     break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE type, bool on)
* @brief      Scroll set activate
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE type, bool on)
{
  UI_PROPERTY_SCROLLEABLE_ACTIVATE mode = on ? UI_PROPERTY_SCROLLEABLE_ACTIVATE_ON : UI_PROPERTY_SCROLLEABLE_ACTIVATE_OFF;

  switch(type)
    {                                     default     : 
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      : return false; 
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.activateoverride = mode;      
                                                        vertical.activateoverride   = mode;
                                                        break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.activateoverride = mode;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.activateoverride   = mode;   break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_SetActivateAuto(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Clears the user activation override: activation goes back to following the overflow policy.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_SetActivateAuto(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  switch(type)
    {                                     default     : 
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      : return false; 
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.activateoverride = UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO;      
                                                        vertical.activateoverride   = UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO;
                                                        break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.activateoverride = UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.activateoverride   = UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO;   break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SCROLLEABLE_ACTIVATE UI_PROPERTY_SCROLLEABLE::Scroll_GetActivateMode(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Returns the user activation override (AUTO / ON / OFF) for an axis. This is the user intent, not the resolved state (use Scroll_GetActive for the resolved state).
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     UI_PROPERTY_SCROLLEABLE_ACTIVATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SCROLLEABLE_ACTIVATE UI_PROPERTY_SCROLLEABLE::Scroll_GetActivateMode(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  if(type == UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) return horizontal.activateoverride;

  return vertical.activateoverride;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_GetVisible(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll get visible
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_GetVisible(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  bool status = false;  

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :  
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :       
                                          default     : return false; 
                                                        break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : status = horizontal.visible;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : status = vertical.visible;     break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE type, bool on)
* @brief      Scroll set visible
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE type, bool on)
{
  switch(type)
    {                                     default     : 
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      : return false; 
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : if(horizontal.active) horizontal.visible = on;      
                                                        if(vertical.active)   vertical.visible   = on;
                                                        break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) horizontal.visible = on;   
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active) vertical.visible   = on;     
                                                        break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_PROPERTY_SCROLLEABLE::Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll get limit
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_PROPERTY_SCROLLEABLE::Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  double limit = 0.0f;

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :  
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :       
                                          default     : break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) limit = horizontal.limit;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   limit = vertical.limit;     break;
    }

  return limit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetLimit(UI_PROPERTY_SCROLLEABLE_TYPE type, double limit)
* @brief      Scroll set limit
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  limit : Limit value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetLimit(UI_PROPERTY_SCROLLEABLE_TYPE type, double limit)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :                              
                                        default       : break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) horizontal.limit = limit;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   vertical.limit = limit;    break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_PROPERTY_SCROLLEABLE::Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll get displacement
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_PROPERTY_SCROLLEABLE::Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  double displacement = 0.0f;

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :  
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :       
                                        default       : break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) displacement = horizontal.displacement;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   displacement = vertical.displacement;     break;
    }

  return displacement;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_PROPERTY_SCROLLEABLE::Scroll_GetStep(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll get step
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_PROPERTY_SCROLLEABLE::Scroll_GetStep(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  double step = 0.0f;

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :  
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :       
                                        default       : break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) step = horizontal.step;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   step = vertical.step;     break;
    }

  return step;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetStep(UI_PROPERTY_SCROLLEABLE_TYPE type, double step)
* @brief      Scroll set step
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  step : Step value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetStep(UI_PROPERTY_SCROLLEABLE_TYPE type, double step)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :       
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :              
                                        default       : break; 

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(!horizontal.active) break;

                                                        if((horizontal.displacement + step) > 0) 
                                                          { 
                                                            horizontal.displacement = 0;
                                                            break;
                                                          }
  
                                                        if(horizontal.limit != -1)
                                                          {
                                                            if((horizontal.displacement + step) < horizontal.limit) 
                                                              {
                                                                horizontal.displacement = horizontal.limit;
                                                                break;
                                                              }
                                                          }

                                                        horizontal.step          = step;
                                                        horizontal.displacement += step;
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL    : if(!vertical.active) break;

                                                        if((vertical.displacement + step) > 0) 
                                                          { 
                                                            vertical.displacement = 0;
                                                            break;
                                                          }
  
                                                        if(vertical.limit != -1)
                                                          {
                                                            if((vertical.displacement + step) < vertical.limit) 
                                                              {
                                                                vertical.displacement = vertical.limit;
                                                                break;
                                                              }
                                                          }

                                                        vertical.step           = step;
                                                        vertical.displacement  += step;   
                                                        break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_ResetStep(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Scroll reset step
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_ResetStep(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN      :        
                                        default       : break;
 
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          :

                                                        if(horizontal.active) horizontal.step = 0.0f;
                                                        if(vertical.active)   vertical.step   = 0.0f;
                                                        break;
         
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) horizontal.step = 0.0f;      
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   vertical.step   = 0.0f;     
                                                        break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_OVERFLOW_MODE UI_PROPERTY_SCROLLEABLE::Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Get overflow mode for an axis
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     UI_OVERFLOW_MODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_OVERFLOW_MODE UI_PROPERTY_SCROLLEABLE::Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : return horizontal.overflow;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : return vertical.overflow;
                                        default       : break;
    }

  return UI_OVERFLOW_VISIBLE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE type, UI_OVERFLOW_MODE mode)
* @brief      Set overflow mode for an axis (or both with ALL)
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE type, UI_OVERFLOW_MODE mode)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.overflow = mode;
                                                        vertical.overflow   = mode;
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.overflow = mode;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.overflow   = mode;  break;
                                        default       : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_NeedClip()
* @brief      Whether the container must clip its content (any axis overflow != visible)
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_NeedClip()
{
  if(horizontal.overflow != UI_OVERFLOW_VISIBLE) return true;
  if(vertical.overflow   != UI_OVERFLOW_VISIBLE) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_PROPERTY_SCROLLEABLE::Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE type, double fullviewport)
* @brief      Single overlay-vs-classic seam. Overlay (default): the scrollbar floats, content keeps the full viewport.
* @note       Classic mode (future) will subtract the perpendicular scrollbar width here; nothing else changes.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  fullviewport : Fullviewport value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_PROPERTY_SCROLLEABLE::Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE type, double fullviewport)
{
  (void)type;

  // Overlay mode: scrollbars do not steal layout. Content viewport == full viewport.
  return fullviewport;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE type, double viewport, double contentextent)
* @brief      Resolve active/visible/limit for an axis from its overflow mode and content vs viewport.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  viewport : Viewport value.
* @param[in]  contentextent : Contentextent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE type, double viewport, double contentextent)
{
  if((type != UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) && (type != UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL)) return false;

  UI_PROPERTY_SCROLLEABLE_STATUS* status = (type == UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) ? &horizontal : &vertical;

  UI_OVERFLOW_MODE  mode    = status->overflow;
  bool              exceeds = (contentextent > viewport);
  bool              active  = false;

  switch(mode)                                                       // base activation from the overflow policy
    {
      case  UI_OVERFLOW_VISIBLE   :
      case  UI_OVERFLOW_HIDDEN    : active = false;     break;
      case  UI_OVERFLOW_SCROLL    : active = true;      break;
      case  UI_OVERFLOW_AUTO      : active = exceeds;   break;
                        default   : break;
    }

  if(status->activateoverride == UI_PROPERTY_SCROLLEABLE_ACTIVATE_ON)  active = true;    // user override on top of the policy
  if(status->activateoverride == UI_PROPERTY_SCROLLEABLE_ACTIVATE_OFF) active = false;

  status->active = active;                                           // resolved state (set directly; SetActivate is now the override)

  if(!active)
    {
      status->visible = false;
      status->limit   = 0.0f;
    }
   else
    {
      status->limit   = __MIN(0.0, viewport - contentextent);        // <= 0 : range the content can be displaced
      status->visible = (mode == UI_OVERFLOW_SCROLL) ? true : (status->limit < 0.0);   // SCROLL: always; else only if there is range
    }

  return (mode != UI_OVERFLOW_VISIBLE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE type, double fraction)
* @brief      Sets the scroll position from a 0..1 fraction (0 = start, 1 = end). Back-channel for interactive bars.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  fraction : Fraction value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE type, double fraction)
{
  if(fraction < 0.0f) fraction = 0.0f;
  if(fraction > 1.0f) fraction = 1.0f;

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : if(horizontal.active) horizontal.displacement = fraction * horizontal.limit;   break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : if(vertical.active)   vertical.displacement   = fraction * vertical.limit;     break;
                                        default       : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarWidth(UI_PROPERTY_SCROLLEABLE_TYPE type, double width)
* @brief      Sets the scrollbar thickness (vertical: width, horizontal: height) for an axis (ALL = both).
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  width : Width value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarWidth(UI_PROPERTY_SCROLLEABLE_TYPE type, double width)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.barwidth = width;
                                                        vertical.barwidth   = width;
                                                        break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.barwidth = width;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.barwidth   = width;  break;
                                        default       : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_PROPERTY_SCROLLEABLE::Scroll_GetBarWidth(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Gets the scrollbar thickness for an axis.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_PROPERTY_SCROLLEABLE::Scroll_GetBarWidth(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : return horizontal.barwidth;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : return vertical.barwidth;
                                        default       : break;
    }

  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE type, bool shown)
* @brief      If false the bar is not drawn nor interactive, but scrolling keeps working (swipe / programmatic).
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  shown : Shown value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE type, bool shown)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.showbar = shown;
                                                        vertical.showbar   = shown;
                                                        break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.showbar = shown;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.showbar   = shown;  break;
                                        default       : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_GetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE type)
* @brief      Whether the bar is configured to be drawn/interactive for an axis.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_GetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE type)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : return horizontal.showbar;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : return vertical.showbar;
                                        default       : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarColors(UI_PROPERTY_SCROLLEABLE_TYPE type, UI_COLOR& thumbcolor, UI_COLOR& trackcolor)
* @brief      Sets thumb and track RGBA colors (0..255) for an axis (ALL = both).
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  thumbcolor : Thumbcolor value.
* @param[in]  trackcolor : Trackcolor value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarColors(UI_PROPERTY_SCROLLEABLE_TYPE type, UI_COLOR& thumbcolor, UI_COLOR& trackcolor)
{
  UI_PROPERTY_SCROLLEABLE_STATUS* targets[2] = { NULL, NULL };

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : targets[0] = &horizontal;  targets[1] = &vertical;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : targets[0] = &horizontal;                           break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : targets[0] = &vertical;                             break;
                                        default       : break;
    }

  for(int i=0; i<2; i++)
    {
      if(!targets[i]) continue;

      targets[i]->thumbcolor[0] = thumbcolor.GetRed();    targets[i]->thumbcolor[1] = thumbcolor.GetGreen();
      targets[i]->thumbcolor[2] = thumbcolor.GetBlue();   targets[i]->thumbcolor[3] = thumbcolor.GetAlpha();

      targets[i]->trackcolor[0] = trackcolor.GetRed();    targets[i]->trackcolor[1] = trackcolor.GetGreen();
      targets[i]->trackcolor[2] = trackcolor.GetBlue();   targets[i]->trackcolor[3] = trackcolor.GetAlpha();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarThumbColor(UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a)
* @brief      Gets the thumb RGBA color for an axis.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  r : R value.
* @param[in]  g : G value.
* @param[in]  b : B value.
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarThumbColor(UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a)
{
  UI_PROPERTY_SCROLLEABLE_STATUS* status = (type == UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) ? &horizontal : &vertical;

  r = status->thumbcolor[0];  g = status->thumbcolor[1];  b = status->thumbcolor[2];  a = status->thumbcolor[3];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarTrackColor(UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a)
* @brief      Gets the track RGBA color for an axis.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  r : R value.
* @param[in]  g : G value.
* @param[in]  b : B value.
* @param[in]  a : A value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarTrackColor(UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a)
{
  UI_PROPERTY_SCROLLEABLE_STATUS* status = (type == UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) ? &horizontal : &vertical;

  r = status->trackcolor[0];  g = status->trackcolor[1];  b = status->trackcolor[2];  a = status->trackcolor[3];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE type, double separationedge, double separationalong)
* @brief      Sets the bar separation (gap) from the frame, horizontal and vertical, for an axis (ALL = both).
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  separationedge : Separationedge value.
* @param[in]  separationalong : Separationalong value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_SetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE type, double separationedge, double separationalong)
{
  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_ALL          : horizontal.sepedge = separationedge;  horizontal.sepalong = separationalong;
                                                        vertical.sepedge   = separationedge;  vertical.sepalong   = separationalong;
                                                        break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : horizontal.sepedge = separationedge;  horizontal.sepalong = separationalong;  break;
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : vertical.sepedge   = separationedge;  vertical.sepalong   = separationalong;  break;
                                        default       : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE type, double& separationedge, double& separationalong)
* @brief      Gets the bar separation (gap) from the frame for an axis.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  separationedge : Separationedge value.
* @param[in]  separationalong : Separationalong value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Scroll_GetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE type, double& separationedge, double& separationalong)
{
  UI_PROPERTY_SCROLLEABLE_STATUS* status = (type == UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) ? &horizontal : &vertical;

  separationedge  = status->sepedge;
  separationalong = status->sepalong;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SCROLLEABLE::Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE type, double box_left, double box_top, double box_right, double box_bottom, double radius, double& track_left, double& track_top, double& track_right, double& track_bottom)
* @brief      Inset track (gutter) rectangle for an axis, shortened by the container corner radius so the bar clears rounded corners. Single geometry source for draw and hit-test. Returns false if no bar must be shown.
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* @param[in]  box_left : Box left value.
* @param[in]  box_top : Box top value.
* @param[in]  box_right : Box right value.
* @param[in]  box_bottom : Box bottom value.
* @param[in]  radius : Radius value.
* @param[in]  track_left : Track left value.
* @param[in]  track_top : Track top value.
* @param[in]  track_right : Track right value.
* @param[in]  track_bottom : Track bottom value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SCROLLEABLE::Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE type, double box_left, double box_top, double box_right, double box_bottom, double radius, double& track_left, double& track_top, double& track_right, double& track_bottom)
{
  if(!Scroll_GetVisible(type))  return false;
  if(!Scroll_GetBarShown(type)) return false;

  double width = Scroll_GetBarWidth(type);
  if(width <= 0.0f) return false;

  double sepedge  = 0.0f;   // gap from the frame edge the bar docks to
  double sepalong = 0.0f;   // inset at the two ends of the bar
  Scroll_GetBarSeparation(type, sepedge, sepalong);

  // When BOTH bars are drawn they share the bottom-right corner. Reserve it: shorten this bar at the end nearest the
  // other bar by the other bar's thickness (+ its edge gap), so they never overlap.
  UI_PROPERTY_SCROLLEABLE_TYPE other = (type == UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL) ? UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL
                                                                                       : UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL;
  double corner = 0.0f;
  if(Scroll_GetVisible(other) && Scroll_GetBarShown(other) && (Scroll_GetBarWidth(other) > 0.0f))
    {
      double other_sepedge  = 0.0f;
      double other_sepalong = 0.0f;
      Scroll_GetBarSeparation(other, other_sepedge, other_sepalong);
      corner = Scroll_GetBarWidth(other) + other_sepedge;
    }

  switch(type)
    {
      case  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL     : track_right  = box_right  - sepedge;                      // gap from the right edge
                                                        track_left   = track_right - width;
                                                        track_top    = box_top    + radius + sepalong;           // end inset (+ rounded corner)
                                                        track_bottom = box_bottom - radius - sepalong - corner;  // leave room for the horizontal bar
                                                        break;

      case  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL   : track_bottom = box_bottom - sepedge;                     // gap from the bottom edge
                                                        track_top    = track_bottom - width;
                                                        track_left   = box_left   + radius + sepalong;           // end inset (+ rounded corner)
                                                        track_right  = box_right  - radius - sepalong - corner;  // leave room for the vertical bar
                                                        break;

                                        default       : return false;
    }

  if((track_bottom - track_top) <= 0.0f) return false;
  if((track_right  - track_left) <= 0.0f) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SCROLLEABLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SCROLLEABLE::Clean()
{  
  
}





