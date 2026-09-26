/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPScreenCFGChromes.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"

#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
#include "XString.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSCREENCFGCHROMES
{
  public:
                                          GRPSCREENCFGCHROMES           ();
    virtual                               ~GRPSCREENCFGCHROMES          ();

    bool                                  GetUseNativeChromes           ();

    bool                                  GetNativeCaptionActive        ();
    void                                  SetNativeCaptionActive        (bool active);

    bool                                  GetNativeTitleActive          ();
    void                                  SetNativeTitleActive          (bool active);

    bool                                  GetNativeIconActive           ();
    void                                  SetNativeIconActive           (bool active);

    bool                                  GetNativeMaximizeActive       ();
    void                                  SetNativeMaximizeActive       (bool active);

    bool                                  GetNativeMinimizeActive       ();
    void                                  SetNativeMinimizeActive       (bool active);

    bool                                  GetNativeCloseActive          ();
    void                                  SetNativeCloseActive          (bool active);

    bool                                  GetResizeActive               ();
    void                                  SetResizeActive               (bool active);

    #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

    XSTRING*                              GetCustomLayoutFile           ();
    void                                  SetCustomLayoutFile           (XCHAR* namelayoutfile);

    XSTRING*                              GetCustomLayoutName           ();
    void                                  SetCustomLayoutName           (XCHAR* namelayout);

    XDWORD                                GetCustomAutoHide             ();
    void                                  SetCustomAutoHide             (XDWORD timehidden);

    #endif

    bool                                  CopyTo                        (GRPSCREENCFGCHROMES& cfgchromes);
    bool                                  CopyFrom                      (GRPSCREENCFGCHROMES& cfgchromes);

  private:

    void                                  Clean                         ();

    bool                                  usenativechromes;

    bool                                  captionactive;

    bool                                  titleactive;

    bool                                  iconactive;

    bool                                  maximizeactive;
    bool                                  minimizeactive;
    bool                                  closeactive;

    bool                                  resizeactive;

    #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
    XSTRING                               customlayoutfile;
    XSTRING                               customlayoutname;
    XDWORD                                customautohide;
    #endif
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


