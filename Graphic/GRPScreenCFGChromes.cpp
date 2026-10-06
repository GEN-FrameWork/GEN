/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPScreenCFGChromes.cpp
* 
* @class      GRPSCREENCFGCHROMES
* @brief      Graphics Screen Chromes Configuration class
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

#include "GRPScreenCFGChromes.h"

#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
#include "XPath.h"
#include "XPathsManager.h"
#endif


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENCFGCHROMES::GRPSCREENCFGCHROMES()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENCFGCHROMES::GRPSCREENCFGCHROMES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREENCFGCHROMES::~GRPSCREENCFGCHROMES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREENCFGCHROMES::~GRPSCREENCFGCHROMES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetUseNativeChromes()
* @brief      Get use native chromes
* @ingroup    GRAPHIC
* 
* @return     bool : true if native chromes (OS managed) are used; otherwise false (custom GEN chromes).
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetUseNativeChromes()
{
  return usenativechromes;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeCaptionActive()
* @brief      Get native caption active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the caption's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeCaptionActive()
{
  return captionactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeCaptionActive(bool active)
* @brief      Set native caption active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the caption's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeCaptionActive(bool active)
{
  captionactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeTitleActive()
* @brief      Get native title active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the title's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeTitleActive()
{
  return titleactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeTitleActive(bool active)
* @brief      Set native title active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the title's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeTitleActive(bool active)
{
  titleactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeIconActive()
* @brief      Get native icon active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the icon's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeIconActive()
{
  return iconactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeIconActive(bool active)
* @brief      Set native icon active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the icon's own presence and
*             look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeIconActive(bool active)
{
  iconactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeMaximizeActive()
* @brief      Get native maximize active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the maximize button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeMaximizeActive()
{
  return maximizeactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeMaximizeActive(bool active)
* @brief      Set native maximize active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the maximize button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeMaximizeActive(bool active)
{
  maximizeactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeMinimizeActive()
* @brief      Get native minimize active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the minimize button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeMinimizeActive()
{
  return minimizeactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeMinimizeActive(bool active)
* @brief      Set native minimize active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the minimize button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeMinimizeActive(bool active)
{
  minimizeactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetNativeCloseActive()
* @brief      Get native close active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the close button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetNativeCloseActive()
{
  return closeactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetNativeCloseActive(bool active)
* @brief      Set native close active
* @note       Native chromes only (see GetUseNativeChromes()); in custom chromes the close button's own
*             presence and look are decided entirely by the layout.
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetNativeCloseActive(bool active)
{
  closeactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::GetResizeActive()
* @brief      Get resize active
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::GetResizeActive()
{
  return resizeactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetResizeActive(bool active)
* @brief      Set resize active
* @ingroup    GRAPHIC
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetResizeActive(bool active)
{
  resizeactive = active;
}


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPSCREENCFGCHROMES::GetCustomLayoutFile()
* @brief      Get custom layout file
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPSCREENCFGCHROMES::GetCustomLayoutFile()
{
  return &customlayoutfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetCustomLayoutFile(XCHAR* namelayoutfile)
* @brief      Set custom layout file
* @ingroup    GRAPHIC
* 
* @param[in]  namelayoutfile : Namelayoutfile value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetCustomLayoutFile(XCHAR* namelayoutfile)
{
  customlayoutfile.Set(namelayoutfile);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPSCREENCFGCHROMES::GetCustomLayoutName()
* @brief      Get custom layout name
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPSCREENCFGCHROMES::GetCustomLayoutName()
{
  return &customlayoutname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetCustomLayoutName(XCHAR* namelayout)
* @brief      Set custom layout name
* @ingroup    GRAPHIC
* 
* @param[in]  namelayout : Namelayout value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetCustomLayoutName(XCHAR* namelayout)
{
  customlayoutname.Set(namelayout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPSCREENCFGCHROMES::GetCustomAutoHide()
* @brief      Get custom auto hide
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPSCREENCFGCHROMES::GetCustomAutoHide()
{
  return customautohide;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::SetCustomAutoHide(XDWORD timehidden)
* @brief      Set custom auto hide
* @ingroup    GRAPHIC
* 
* @param[in]  timehidden : delay, in milliseconds, the cursor must stay steadily outside/inside the caption's
*             own area before it actually hides/shows. 0 disables auto-hide (the caption stays always visible).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::SetCustomAutoHide(XDWORD timehidden)
{
  customautohide = timehidden;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::CopyTo(GRPSCREENCFGCHROMES& cfgchromes)
* @brief      Copy to
* @ingroup    GRAPHIC
* 
* @param[in]  cfgchromes : Chromes configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::CopyTo(GRPSCREENCFGCHROMES& cfgchromes)
{
  cfgchromes.usenativechromes = usenativechromes;
  cfgchromes.captionactive    = captionactive;
  cfgchromes.titleactive      = titleactive;
  cfgchromes.iconactive       = iconactive;
  cfgchromes.maximizeactive   = maximizeactive;
  cfgchromes.minimizeactive   = minimizeactive;
  cfgchromes.closeactive      = closeactive;
  cfgchromes.resizeactive     = resizeactive;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  cfgchromes.customlayoutfile = customlayoutfile;
  cfgchromes.customlayoutname = customlayoutname;
  cfgchromes.customautohide   = customautohide;
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSCREENCFGCHROMES::CopyFrom(GRPSCREENCFGCHROMES& cfgchromes)
* @brief      Copy from
* @ingroup    GRAPHIC
* 
* @param[in]  cfgchromes : Chromes configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREENCFGCHROMES::CopyFrom(GRPSCREENCFGCHROMES& cfgchromes)
{
  usenativechromes = cfgchromes.usenativechromes;
  captionactive    = cfgchromes.captionactive;
  titleactive      = cfgchromes.titleactive;
  iconactive       = cfgchromes.iconactive;
  maximizeactive   = cfgchromes.maximizeactive;
  minimizeactive   = cfgchromes.minimizeactive;
  closeactive      = cfgchromes.closeactive;
  resizeactive     = cfgchromes.resizeactive;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  customlayoutfile = cfgchromes.customlayoutfile;
  customlayoutname = cfgchromes.customlayoutname;
  customautohide   = cfgchromes.customautohide;
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPSCREENCFGCHROMES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPSCREENCFGCHROMES::Clean()
{
  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  usenativechromes  = false;               // custom chromes feature compiled in: custom is the default
  #else
  usenativechromes  = true;                // custom chromes feature not compiled in at all: native is the only option
  #endif

  captionactive     = true;

  titleactive       = true;

  iconactive        = true;

  maximizeactive    = true;
  minimizeactive    = true;
  closeactive       = true;

  resizeactive      = true;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

  // Sensible defaults, used unless the app calls SetCustomLayoutFile()/SetCustomLayoutName() itself: a bundled
  // .zip (layout + its own graphics/fonts, self-contained) named "defaultwindowschromes.zip", resolved the same
  // way any other UI layout path is, and a "chrome" layout name within it.
  XPATH defaultlayoutfile;

  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_UI_LAYOUTS, defaultlayoutfile);
  defaultlayoutfile.Slash_Add();
  defaultlayoutfile.Add(_L("defaultwindowschromes.zip"));

  customlayoutfile = defaultlayoutfile;
  customlayoutname = _L("chrome");
  customautohide   = 0;

  #endif
}

