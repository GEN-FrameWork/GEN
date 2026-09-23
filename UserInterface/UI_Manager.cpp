/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Manager.cpp
* 
* @class      UI_MANAGER
* @brief      User Interface Manager class
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

#include "UI_Manager.h"
#include "UI_Property_Scrolleable.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "XFactory.h"
#include "XThread.h"
#include "XTimer.h"
#include "XPath.h"
#include "XFile.h"
#include "XTrace.h"
#include "XTranslation.h"
#include "XSleep.h"

#include "INPCursorMotion.h"

#include "GRPProperties.h"
#include "GRPScreen.h"
#include "GRPViewPort.h"
#include "GRP2DCanvas.h"
#include "GRP2DColor.h"
#include "GRPBitmapFile.h"
#include "GRPBitmap.h"
#include "GRPFactory.h"
#include "GRPVectorFile.h"
#include "GRP2DVectorFileRenderAGG.h"

#include "APPFlowBase.h"

#include "UI_XEvent.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "UI_PropertyRegistry.h"
#include "UI_ComputedStyle.h"
#include "UI_CSSParser.h"
#include "UI_Length.h"
#include "UI_Color.h"
#include "UI_Colors.h"
#include "UI_Text.h"
#include "UI_Texts.h"
#include "UI_Animation.h"
#include "UI_Animations.h"
#include "UI_Element.h"
#include "UI_LayoutEngine.h"
#include "UI_Element_Text.h"
#include "UI_Element_TextBox.h"
#include "UI_Element_Image.h"
#include "UI_Element_Animation.h"
#include "UI_Element_Option.h"
#include "UI_Element_MultiOption.h"
#include "UI_Element_Button.h"
#include "UI_Element_CheckBox.h"
#include "UI_Element_EditText.h"
#include "UI_Element_Form.h"
#include "UI_Element_Menu.h"
#include "UI_Element_ListBox.h"
#include "UI_Element_ProgressBar.h"
#include "UI_Element_Scroll.h"
#include "UI_Element_ProgressRadial.h"
#include "UI_Element_ProgressImage.h"
#include "UI_Element_StatisticsChart.h"

#include "UI_VirtualKeyboard.h"

#include "UI_Skin.h"
#include "UI_SkinCanvas.h"
#include "UI_SkinCanvas_Flat.h"
#include "UI_BoundaryLine.h"
#include "GRP2DRebuildAreas.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

UI_MANAGER* UI_MANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_MANAGER& UI_MANAGER::GetInstance()
* @brief      Get instance
* @ingroup    USERINTERFACE
* 
* @return     UI_MANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_MANAGER& UI_MANAGER::GetInstance()
{
  if(!instance) 
    {
      instance = GEN_NEW UI_MANAGER();
    }

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::DelInstance()
* @brief      Del instance
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;
    }  

  GEN_UI_COLORS.DelInstance();

  GEN_UI_TEXTS.DelInstance();

  GEN_UI_ANIMATIONS.DelInstance();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Load(XPATH& pathfile, GRPSCREEN* screen, int viewportindex)
* @brief      Load
* @ingroup    USERINTERFACE
* 
* @param[in]  pathfile : Pathfile value.
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Load(XPATH& pathfile, GRPSCREEN* screen, int viewportindex)
{
  bool status = false;  

  iszippedfile = false;

  if(pathfile.Find(__L(".zip"), true) != XSTRING_NOTFOUND) 
    {
      iszippedfile = true;
    }

  if(!iszippedfile)
    {
      status = LoadLayout(pathfile, screen, viewportindex);

      if(!status)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[UI Load] ERROR: cannot open layout XML [%s] (file missing? name case mismatch on a case-sensitive filesystem?)"), pathfile.Get());
        }
    }
   else
    {
      unzipfile = GEN_NEW XFILEUNZIP();
      if(!unzipfile)
        {
          return false;
        }

      bool openresult = unzipfile->Open(pathfile);
      if(openresult)
        {
          XSTRING origin_drive;
          XPATH   origin_path;
          XSTRING origin_namefile;
          XSTRING origin_ext;

          pathfile.GetDrive(origin_drive);
          pathfile.GetPath(origin_path);
          pathfile.GetNamefile(origin_namefile);
          pathfile.GetExt(origin_ext);

          unzippathfile  = origin_drive;
          unzippathfile += origin_path;

          XSTRING namefile;

          namefile    = origin_namefile;
          namefile   += __L(".xml");


          status = unzipfile->DecompressFile(namefile, unzippathfile, namefile.Get());

          #ifdef LINUX
          if(!status)
            {
              // The bundle's own directory is not writable (typical of a read-only install dir on native
              // Linux: /usr/share, /opt, or simply a directory owned by another user; note WSL runs from
              // /mnt/c/... where everything is always writable, which masks this). Extract to /tmp instead.
              // unzippathfile is redirected too, so every LATER extraction from this same bundle (vector
              // fonts, bitmaps, animation frames -- they all resolve their target directory through
              // GetUnzipPathFile()) automatically follows to the writable location.
              XPATH tmppath;

              tmppath = __L("/tmp/");

              status = unzipfile->DecompressFile(namefile, tmppath, namefile.Get());
              if(status)
                {
                  unzippathfile = tmppath;
                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[UI Load] bundle dir not writable; extracting [%s] to /tmp instead"), namefile.Get());
                }
            }
          #endif

          if(status)
            {
              XPATH unzippathfile_tmp;

              unzippathfile_tmp  = unzippathfile;
              unzippathfile_tmp += namefile;

              status = LoadLayout(unzippathfile_tmp, screen, viewportindex);

              DeleteTemporalUnZipFile(unzippathfile_tmp);
            }
           else
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[UI Load] ERROR: cannot extract [%s] from bundle [%s] (entry missing in zip, or target dir not writable)"), namefile.Get(), pathfile.Get());
            }
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[UI Load] ERROR: cannot open layout bundle [%s] (file missing? name case mismatch on a case-sensitive filesystem?)"), pathfile.Get());
          CloseUnZipFile();
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::LoadLayout(XPATH& pathfile, GRPSCREEN* screen, int viewportindex)
* @brief      Load layout
* @ingroup    USERINTERFACE
* 
* @param[in]  pathfile : Pathfile value.
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::LoadLayout(XPATH& pathfile, GRPSCREEN* screen, int viewportindex)
{
  XFILEXML xml;
  bool     status = false;  

  if(xml.Open(pathfile, true))
    {
      CreateLayouts(xml, pathfile, screen, viewportindex);

      xml.Close();

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::IsZippedFile()
* @brief      Is zipped file
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::IsZippedFile()
{
  if(iszippedfile && unzipfile)
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* UI_MANAGER::GetUnzipPathFile()
* @brief      Get unzip path file
* @ingroup    USERINTERFACE
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* UI_MANAGER::GetUnzipPathFile()
{
  return &unzippathfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEUNZIP* UI_MANAGER::GetUnzipFile()
* @brief      Get unzip file
* @ingroup    USERINTERFACE
* 
* @return     XFILEUNZIP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEUNZIP* UI_MANAGER::GetUnzipFile()
{
  return unzipfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::DeleteTemporalUnZipFile(XPATH& pathfile)
* @brief      Delete temporal un zip file
* @ingroup    USERINTERFACE
* 
* @param[in]  pathfile : Pathfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::DeleteTemporalUnZipFile(XPATH& pathfile)
{
  XFILE* xfile = GEN_XFACTORY.Create_File();
  if(!xfile)
    {
      return false;
    }
                
  xfile->Erase(pathfile);

  GEN_XFACTORY.Delete_File(xfile);

  return true;
}      


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::CloseUnZipFile()
* @brief      Close un zip file
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::CloseUnZipFile()
{
  if(!unzipfile) 
    {
      return false;
    }

  unzipfile->Close();
  GEN_DELETE unzipfile;

  unzipfile = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layouts_Add(UI_LAYOUT* layout)
* @brief      Layouts add
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_Add(UI_LAYOUT* layout)
{
  if(!layout) 
    {
      return false;
    }

  layouts.Add(layout);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<UI_LAYOUT*>* UI_MANAGER::Layouts_GetAll()
* @brief      Layouts get all
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<UI_LAYOUT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_LAYOUT*>* UI_MANAGER::Layouts_GetAll()
{
  return &layouts;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Layouts_Get(int index)
* @brief      Layouts get
* @ingroup    USERINTERFACE
* 
* @param[in]  index : Index value.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Layouts_Get(int index)
{
  if(layouts.IsEmpty())                     
    {
      return NULL;
    }

  if((XDWORD)index >= layouts.GetSize())    
    {
      return NULL;
    }
  
  return layouts.Get(index);    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Layouts_Get(XCHAR* name)
* @brief      Layouts get
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Layouts_Get(XCHAR* name)
{
  if(layouts.IsEmpty()) 
    {
      return NULL;
    }

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout) 
        {
          if(!layout->GetNameID()->Compare(name, true)) 
            {
              return layout;          
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Layouts_Get(XSTRING& name)
* @brief      Layouts get
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Layouts_Get(XSTRING& name)
{
  return Layouts_Get(name.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layouts_DeleteAll()
* @brief      Layouts GEN_DELETE all
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_DeleteAll()
{
  if(layouts.IsEmpty())  return false;

  layouts.DeleteContents();
  layouts.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Layouts_GetCommonLayout()
* @brief      Layouts get common layout
* @ingroup    USERINTERFACE
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Layouts_GetCommonLayout()
{
  if(layout_commonindex == UI_MANAGER_LAYOUT_NOTFOUND) 
    {
      return NULL;  
    }

  if((XDWORD)layout_commonindex >= layouts.GetSize())     
    {
      return NULL;
    }

  return layouts.Get(layout_commonindex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackground(XCHAR* layoutname)
* @brief      Layout put background
* @note       Whether the layout's background is tiled (seamless pattern) or drawn once covering the whole layout
*             is decided by the "backgroundseamlesspattern" skin/layout attribute, resolved once at
*             CreateLayouts() time: it is stored as UI_BACKGROUND::GetPatternBitmap() instead of GetBitmap() when
*             the attribute is "yes"/"true"/"1", so at most one of the two is ever set for a given layout, and
*             this function simply routes to whichever one is present.
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackground(XCHAR* layoutname)
{
  bool        status = false;
  UI_LAYOUT*  layout = Layouts_Get(layoutname);

  if(layout && layout->GetBackground()->GetPatternBitmap())
    {
      status = Layout_PutBackgroundSeamlessPattern(layoutname);
    }
   else
    {
      status = Layout_PutBackgroundImage(layoutname);
    }

  if(!status) 
    {
      status = Layout_PutBackgroundColor(layoutname);
    }

  // COMPOSITION-RESET (2026-09): wipe every canvas skin on this screen (content + custom chrome). Chrome
  // shares the same GRP2DCANVAS; invalidating only the content layout left chrome's formbackdrop stale so
  // auto-hide / keyboard transitions could ghost the caption over the top menu (UI_Options).
  if(layout)
    {
      GRPSCREEN* screen = NULL;
      UI_SKIN* skin = layout->GetSkin();
      if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
        {
          screen = ((UI_SKINCANVAS*)skin)->GetScreen();
        }
      InvalidateCompositionCachesForScreen(screen);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundColor(XCHAR* layoutname)
* @brief      Layout put background color
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundColor(XCHAR* layoutname)
{
  UI_LAYOUT*  layout = NULL;
  UI_SKIN*    skin   = NULL;
  bool        status = false;

  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }

  skin = layout->GetSkin();
  if(!skin)
    {
      return false;
    }
 
  switch(skin->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

      case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                          if(layout->GetBackground()->GetColor()->IsValid()) 
                                            { 
                                              GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    
                                              if(canvas) 
                                                {
                                                  UI_COLOR color;

                                                  color.CopyFrom(layout->GetBackground()->GetColor()); 

                                                  GRP2DCOLOR_RGBA8 color_canvas(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());  
                                                                                                                    
                                                  canvas->Clear(&color_canvas);

                                                  status = true;                                                        
                                                }
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT  :  break;
    }
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundImage(XCHAR* layoutname)
* @brief      Layout put background bitmap
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundImage(XCHAR* layoutname)
{
  UI_LAYOUT*  layout = NULL;
  UI_SKIN*    skin   = NULL;
  bool        status = false;

  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }

  skin = layout->GetSkin();
  if(!skin)
    {
      return false;
    }
 
  switch(skin->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

      case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                          if(layout->GetBackground()->GetBitmap()) 
                                            { 
                                              GRPSCREEN* screen = skin_canvas->GetScreen();      
                                              GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    
                                              if(canvas && screen) 
                                                {
                                                  // Scale background to the paint target (design canvas when UIScale
                                                  // override is active; otherwise the live viewport).
                                                  layout->GetBackground()->GetBitmap()->Scale((int)canvas->GetWidth(), (int)canvas->GetHeight());          
                                                  canvas->PutBitmapNoAlpha(0, 0, layout->GetBackground()->GetBitmap());                                              

                                                  status = true;
                                                }
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT  :  break;
    }
   
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundSeamlessPattern(XCHAR* layoutname)
* @brief      Layout put background seamless pattern
* @note       Tiles layout->GetBackground()->GetPatternBitmap() (the "backgroundimg" resource - bitmap or a
*             rasterized vector file - loaded by CreateLayouts()/LoadBackgroundBitmap() as a pattern instead of a
*             stretched background because "backgroundseamlesspattern" resolved to true) across the whole layout
*             area, repeating it as a mosaic instead of stretching a single copy like Layout_PutBackgroundImage()
*             does. Unlike Layout_PutBackgroundImage(), it has no "scale" parameter: the tile is always drawn at
*             its own loaded size and simply repeated, since scaling the tile up or down is what would break the
*             "seamless" look.
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundSeamlessPattern(XCHAR* layoutname)
{
  UI_LAYOUT*  layout = NULL;
  UI_SKIN*    skin   = NULL;
  bool        status = false;

  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }

  skin = layout->GetSkin();
  if(!skin)
    {
      return false;
    }
 
  switch(skin->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

      case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                          if(layout->GetBackground()->GetPatternBitmap()) 
                                            { 
                                              GRPSCREEN*   screen = skin_canvas->GetScreen();      
                                              GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    

                                              status = PutBitmapAsSeamlessPattern(canvas, screen, layout->GetBackground()->GetPatternBitmap());
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT  :  break;
    }
   
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackground(bool scale)
* @brief      Layout put background
* @note       Layout_PutBackgroundImage(scale) only draws layouts whose UI_BACKGROUND has GetBitmap() set, and
*             Layout_PutBackgroundSeamlessPattern() only draws layouts whose GetPatternBitmap() is set instead
*             (see CreateLayouts()/Layout_PutBackground(XCHAR*) note) - the two are mutually exclusive per layout,
*             so both are attempted here to cover every layout regardless of its "backgroundseamlesspattern"
*             setting. This is purely additive: layouts without a pattern bitmap behave exactly as before.
* @ingroup    USERINTERFACE
* 
* @param[in]  scale : Scale value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackground(bool scale)
{
  bool status = false;

  status = Layout_PutBackgroundImage(scale);

  if(Layout_PutBackgroundSeamlessPattern())
    {
      status = true;
    }

  if(!status) 
    {
      status = Layout_PutBackgroundColor();
    }

  // COMPOSITION-RESET (2026-09): every layout on every screen that was wiped — including custom chrome skins
  // that share the same canvas as content layouts.
  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    {
      UI_LAYOUT* layout = Layouts_Get(c);
      if(!layout) continue;

      UI_SKIN* skin = layout->GetSkin();
      if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
        {
          UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)skin;
          if(skin_canvas) skin_canvas->InvalidateCompositionCaches();
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::InvalidateCompositionCachesForScreen(GRPSCREEN* screen)
* @brief      Drop persistent true-backdrop caches for every canvas skin drawn on "screen".
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::InvalidateCompositionCachesForScreen(GRPSCREEN* screen)
{
  if(!screen) return;

  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    {
      UI_LAYOUT* layout = Layouts_Get(c);
      if(!layout) continue;

      UI_SKIN* skin = layout->GetSkin();
      if(!skin || skin->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;

      UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)skin;
      if(!skin_canvas || skin_canvas->GetScreen() != screen) continue;

      skin_canvas->InvalidateCompositionCaches();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::Elements_SetToRedrawForScreen(GRPSCREEN* screen, bool exclude_chrome)
* @brief      Mark layouts on "screen" dirty so overlapping content under the caption bar redraws after chrome
*             auto-hide show/hide (UI_Options menu top sits under the chrome band).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::Elements_SetToRedrawForScreen(GRPSCREEN* screen, bool exclude_chrome)
{
  if(!screen) return;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  UI_LAYOUT* chromelayout = screen->GetCFGChromesLayout();
  #else
  UI_LAYOUT* chromelayout = NULL;
  #endif

  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    {
      UI_LAYOUT* layout = Layouts_Get(c);
      if(!layout) continue;

      UI_SKIN* skin = layout->GetSkin();
      if(!skin || skin->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;

      UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)skin;
      if(!skin_canvas || skin_canvas->GetScreen() != screen) continue;

      if(exclude_chrome && chromelayout && layout == chromelayout) continue;

      layout->Elements_SetToRedraw();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundColor()
* @brief      Layout put background color
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundColor()
{
  bool status = false;
  
  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    { 
      UI_LAYOUT*  layout = NULL;
      UI_SKIN*    skin   = NULL;

      layout = Layouts_Get(c);
      if(layout)
        {       
          skin = layout->GetSkin();
          if(!skin)
            {
              return false;
            }
 
          switch(skin->GetDrawMode())
            {
              case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

              case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                                  if(layout->GetBackground()->GetColor()->IsValid()) 
                                                    { 
                                                      GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    
                                                      if(canvas) 
                                                        {
                                                          UI_COLOR color;

                                                          color.CopyFrom(layout->GetBackground()->GetColor()); 

                                                          GRP2DCOLOR_RGBA8 color_canvas(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());  
                                                                                                                    
                                                          canvas->Clear(&color_canvas);

                                                          status = true;                                                        
                                                        }
                                                    }
                                                }
                                                break;

              case UI_SKIN_DRAWMODE_CONTEXT  :  break;
            }
        }
    }
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundImage(bool scale)
* @brief      Layout put background bitmap
* @ingroup    USERINTERFACE
* 
* @param[in]  scale : Scale value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundImage(bool scale)
{  
  bool status = false;

  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    { 
      UI_LAYOUT*  layout = NULL;
      UI_SKIN*    skin   = NULL;

      layout = Layouts_Get(c);
      if(layout)
        {    
          skin = layout->GetSkin();
          if(!skin)
            {
              return false;
            }
 
          switch(skin->GetDrawMode())
            {
              case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

              case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                                  if(layout->GetBackground()->GetBitmap()) 
                                                    { 
                                                      GRPSCREEN* screen = skin_canvas->GetScreen();      
                                                      GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    
                                                      if(canvas && screen) 
                                                        {
                                                          if(scale)
                                                            {  
                                                              layout->GetBackground()->GetBitmap()->Scale(screen->GetWidth(), screen->GetHeight());          
                                                            }

                                                          canvas->PutBitmapNoAlpha(0, 0, layout->GetBackground()->GetBitmap());                                              

                                                          status = true;
                                                        }
                                                    }
                                                }
                                                break;

              case UI_SKIN_DRAWMODE_CONTEXT  :  break;
            }
        }
    }   
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Layout_PutBackgroundSeamlessPattern()
* @brief      Layout put background seamless pattern
* @note       Tiles layout->GetBackground()->GetPatternBitmap() across every loaded layout, same tiling behavior
*             as the by-name overload; see that overload's note for details. Has no "scale" parameter, matching
*             the requested UI_MANAGER::Layout_PutBackgroundSeamlessPattern() signature: a tiled pattern is always
*             drawn at its own loaded size.
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layout_PutBackgroundSeamlessPattern()
{  
  bool status = false;

  for(XDWORD c=0; c<Layouts_GetAll()->GetSize(); c++)
    { 
      UI_LAYOUT*  layout = NULL;
      UI_SKIN*    skin   = NULL;

      layout = Layouts_Get(c);
      if(layout)
        {    
          skin = layout->GetSkin();
          if(!skin)
            {
              return false;
            }
 
          switch(skin->GetDrawMode())
            {
              case UI_SKIN_DRAWMODE_UNKNOWN  :  break;

              case UI_SKIN_DRAWMODE_CANVAS   :  { UI_SKINCANVAS* skin_canvas  = (UI_SKINCANVAS*)skin;
                                                  if(layout->GetBackground()->GetPatternBitmap()) 
                                                    { 
                                                      GRPSCREEN*   screen = skin_canvas->GetScreen();      
                                                      GRP2DCANVAS* canvas = skin_canvas->GetCanvas();    

                                                      if(PutBitmapAsSeamlessPattern(canvas, screen, layout->GetBackground()->GetPatternBitmap()))
                                                        {
                                                          status = true;
                                                        }
                                                    }
                                                }
                                                break;

              case UI_SKIN_DRAWMODE_CONTEXT  :  break;
            }
        }
    }   
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::PutBitmapAsSeamlessPattern(GRP2DCANVAS* canvas, GRPSCREEN* screen, GRPBITMAP* pattern)
* @brief      Put bitmap as seamless pattern
* @note       Shared tiling core used by both Layout_PutBackgroundSeamlessPattern() overloads. Repeats "pattern"
*             at its own natural size, left-to-right/top-to-bottom, starting at (0, 0), until the whole
*             (screen width, screen height) area is covered. The last column/row of tiles is naturally cropped
*             by GRP2DCANVAS::PutBitmapNoAlpha()'s own clipping against the canvas render area, so no partial-tile
*             bookkeeping is needed here.
* @ingroup    USERINTERFACE
* 
* @param[in]  canvas : Canvas to tile the pattern onto.
* @param[in]  screen : Screen used to know the area, in pixels, that must be covered.
* @param[in]  pattern : Bitmap : the seamless pattern tile to repeat; must have a non-zero size.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::PutBitmapAsSeamlessPattern(GRP2DCANVAS* canvas, GRPSCREEN* screen, GRPBITMAP* pattern)
{
  if(!canvas)  return false;
  if(!screen)  return false;
  if(!pattern) return false;

  XDWORD patternwidth  = pattern->GetWidth();
  XDWORD patternheight = pattern->GetHeight();

  if(!patternwidth)  return false;
  if(!patternheight) return false;

  XDWORD screenwidth  = screen->GetWidth();
  XDWORD screenheight = screen->GetHeight();

  for(XDWORD ypos=0; ypos<screenheight; ypos+=patternheight)
    {
      for(XDWORD xpos=0; xpos<screenwidth; xpos+=patternwidth)
        {
          canvas->PutBitmapNoAlpha((double)xpos, (double)ypos, pattern);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Update(UI_LAYOUT* layout)
* @brief      Update
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Update(UI_LAYOUT* layout)
{
  bool status = false;
  
  if(!layout)
    {
      return false;
    }

  ChangeTextElementValue(layout);

  // Keep the modal last in this layout's root walk so its in-pass draw already sits above content siblings.
  // Screen-level topmost (after ALL content layouts, before chrome) is Element_DrawModalOnTop in Update().
  if(element_modal && element_modal->IsVisible())
    {
      UI_LAYOUT* modal_layout = Element_GetLayout(element_modal);
      if(modal_layout == layout) Element_PutToLastPositionLayout(element_modal);
    }

  // Fase 3: paint into design canvas when scale≠1 (Begin may already be active from RebuildDrawAreas).
  bool scaled = UIScale_BeginFrame(layout);

  status = layout->Update();

  if(scaled)
    {
      UIScale_Present(layout);
      UIScale_EndFrame(layout);
    }

  if(status)
    {         
      if(layout_commonindex != UI_MANAGER_LAYOUT_NOTFOUND)
        {         
          layout = Layouts_Get(layout_commonindex);
          if(!layout) 
            {
              return false;          
            }                   
        }
    }

  // Do NOT mark the editable dirty every frame while the keyboard is open. That forced a perpetual
  // Rebuild/Draw of the edit into the keyboard AABB (UI_Options punch). Text changes still dirty via
  // SelectInput; while the modal layer is valid, ModalLayer_ClearIntersectingContentDirt freezes that dirt.

  ChangeTextElementValue(layout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Update(XCHAR* layoutname)
* @brief      Update
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Update(XCHAR* layoutname)
{
  UI_LAYOUT*  layout = NULL;
  bool        status = false;

  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }

  return Update(layout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Update()
* @brief      Update
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Update()
{
  bool status = false;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

  // Screen layers: content → chrome → modal (opaque offscreen blit last). See UI_Manager.h.

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(!layout)                    continue;
      if(IsCFGChromesLayout(layout)) continue;

      status = Update(layout);
      if(!status) return status;
    }

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(!layout)                     continue;
      if(!IsCFGChromesLayout(layout)) continue;

      status = Update(layout);
      if(!status) return status;
      // Do NOT force Elements_SetToRedraw() every frame: that re-captured chrome over content ghosts and
      // burned FPS. Chrome is dirtied on auto-hide show/hide, button shift, and cross-skin overlap propagate.
    }

  // Modal layer (option B well-done): after content AND chrome — blit opaque offscreen cache so neither
  // ListBoxMenu formbackdrop nor chrome caption can remain in the modal AABB.
  if(element_modal && element_modal->IsVisible())
    {
      Element_DrawModalOnTop();
    }

  #else

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    { 
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout)
        {
          status = Update(layout);
          if(!status)
            {
              break;
            }
        }
    }

  if(status && element_modal && element_modal->IsVisible())
    {
      Element_DrawModalOnTop();
    }

  #endif

  return status;
}


#ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::IsCFGChromesLayout(UI_LAYOUT* layout)
* @brief      Is CFG chromes layout
* @note       true when this layout IS the custom chrome layout of the screen it is drawn on -- identity, not
*             position: walks layout -> its own UI_SKINCANVAS -> the GRPSCREEN it belongs to, and compares
*             against that screen's own GRPSCREEN::GetCFGChromesLayout(). A layout that is not on a canvas skin,
*             not tied to any screen, or belongs to a screen without custom chromes active, is never one.
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::IsCFGChromesLayout(UI_LAYOUT* layout)
{
  if(!layout)                                                      return false;
  if(!layout->GetSkin())                                           return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS)   return false;

  GRPSCREEN* screen = ((UI_SKINCANVAS*)layout->GetSkin())->GetScreen();
  if(!screen)                                 return false;
  if(!screen->IsCFGChromesActive())           return false;
  if(!screen->GetCFGChromes())                return false;
  if(screen->GetCFGChromes()->GetUseNativeChromes()) return false;

  return (screen->GetCFGChromesLayout() == layout);
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* layoutname, XCHAR* name, UI_ELEMENT_TYPE type)
* @brief      Element get
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* layoutname, XCHAR* name, UI_ELEMENT_TYPE type)
{  
  UI_LAYOUT*  layout = NULL; 
  UI_ELEMENT* element = NULL;

  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return NULL;
    }

  element = layout->Elements_Get(name, type);
  if(!element)
    {
      if(layout_commonindex != UI_MANAGER_LAYOUT_NOTFOUND)
        {
          layout = Layouts_Get(layout_commonindex);
          if(!layout) 
            {
              return NULL;
            }

          element = layout->Elements_Get(name, type);
        }
    }

  return element;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* layoutname, XSTRING& name, UI_ELEMENT_TYPE type)
* @brief      Element get
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* layoutname, XSTRING& name, UI_ELEMENT_TYPE type)
{
  return Element_Get(layoutname, name.Get(), type);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* name, UI_ELEMENT_TYPE type)
* @brief      Element get
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::Element_Get(XCHAR* name, UI_ELEMENT_TYPE type)
{ 
  if(layouts.IsEmpty()) 
    {
      return NULL;
    }

  UI_ELEMENT* element = NULL; 

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    { 
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout) 
        {          
          element = layout->Elements_Get(name, type);
          if(element) 
            {
              return element;              
            }
        }
    }

  return element;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::Element_Get(XSTRING& name, UI_ELEMENT_TYPE type)
* @brief      Element get
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::Element_Get(XSTRING& name, UI_ELEMENT_TYPE type)
{ 
  return Element_Get(name.Get(), type);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Element_GetLayout(UI_ELEMENT* element)
* @brief      Element get layout
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Element_GetLayout(UI_ELEMENT* element)
{
  if(!element) 
    {
      return NULL;
    }

  if(layouts.IsEmpty()) 
    {
      return NULL;
    }

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    { 
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout) 
        { 
          XVECTOR<UI_ELEMENT*>* layout_elements = layout->Elements_Get();
          if(layout_elements) 
            {
              bool    found = false;
              XDWORD  index = 0;
 
              do{ UI_ELEMENT* _element = layout_elements->Get(index);
                  if(_element) 
                    {
                      if(_element == element) 
                        {
                          found = true;
                          break;
                        }
                    } 

                  index++;

                } while(index < layout_elements->GetSize());    
                               
              if(found) 
                {
                  return layout;              
                }
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Element_GetLayout(XCHAR* name, UI_ELEMENT_TYPE type)
* @brief      Element get layout
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Element_GetLayout(XCHAR* name, UI_ELEMENT_TYPE type)
{
  if(layouts.IsEmpty()) 
    {
      return NULL;
    }

  UI_ELEMENT* element = NULL; 

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    { 
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout) 
        {          
          element = layout->Elements_Get(name, type);
          if(element) 
            {
              return layout;              
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_LAYOUT* UI_MANAGER::Element_GetLayout(XSTRING& name, UI_ELEMENT_TYPE type)
* @brief      Element get layout
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_MANAGER::Element_GetLayout(XSTRING& name, UI_ELEMENT_TYPE type)
{
  return Element_GetLayout(name.Get(), type);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Element_PutToLastPositionLayout(UI_ELEMENT* element)
* @brief      Element put to last position layout
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Element_PutToLastPositionLayout(UI_ELEMENT* element)
{
  if(!element) 
    {
      return false;
    }

  UI_LAYOUT* layout = Element_GetLayout(element);
  if(!layout) 
    {
      return false;
    }

  XVECTOR<UI_ELEMENT*>* layout_elements = layout->Elements_Get();
  if(!layout_elements) 
    {
      return false;
    }

  if(layout_elements->IsEmpty()) return false;

  // Already last: nothing to do. (Avoid a no-op delete/re-add.)
  if(layout_elements->Get(layout_elements->GetSize()-1) == element) return true;

  // Move to end without swapping another root into the modal's old slot. Swap left an unrelated sibling in the
  // middle of the list; delete+add keeps relative order of everyone else and guarantees the modal is drawn last
  // in UI_LAYOUT::Update()'s linear walk.
  if(!layout_elements->Delete(element)) return false;
  layout_elements->Add(element);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Element_PutToLastPositionLayout(XCHAR* name, UI_ELEMENT_TYPE type)
* @brief      Element put to last position layout
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Element_PutToLastPositionLayout(XCHAR* name, UI_ELEMENT_TYPE type)
{
  UI_LAYOUT* layout = Element_GetLayout(name, type);
  if(!layout) 
    {
      return false;
    }

  UI_ELEMENT* element = layout->Elements_Get(name, type);
  if(!element) 
    {
      return false;
    }

  XVECTOR<UI_ELEMENT*>* layout_elements = layout->Elements_Get();
  if(!layout_elements) 
    {
      return false;
    }
  
  bool    found = false;
  XDWORD  index = 0;

  do{ UI_ELEMENT* _element = layout_elements->Get(index);
      if(_element) 
        {
          if(_element == element) 
            {
              found = true;
              break;
            }
        } 

       index++;

    } while(index < layout_elements->GetSize());

  if(!found) 
    {
      return false;  
    }

  UI_ELEMENT* sustitute_element =  layout_elements->Get(layout_elements->GetSize()-1);
  if(!sustitute_element) 
    {
      return false;
    }

  layout_elements->Set(layout_elements->GetSize()-1, layout_elements->Get(index));
  layout_elements->Set(index, sustitute_element);         

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Element_PutToLastPositionLayout(XSTRING& name, UI_ELEMENT_TYPE type)
* @brief      Element put to last position layout
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Element_PutToLastPositionLayout(XSTRING& name, UI_ELEMENT_TYPE type)
{
  return Element_PutToLastPositionLayout(name.Get(), type);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Element_SetModal(UI_ELEMENT* element_modal)
* @brief      Element set modal
* @ingroup    USERINTERFACE
* 
* @param[in]  element_modal : Element modal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Element_SetModal(UI_ELEMENT* element_modal)
{
  if(xmutex_modal) 
    {
      xmutex_modal->Lock();
    }

  if(this->element_modal != element_modal)
    {
      ModalLayer_Invalidate();
    }

  if(element_modal) 
    {
      Element_PutToLastPositionLayout(element_modal);
    }

  this->element_modal = element_modal;

  if(xmutex_modal) 
    {
      xmutex_modal->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT* UI_MANAGER::Element_GetModal()
* @brief      Element get modal
* @ingroup    USERINTERFACE
*
* @return     UI_ELEMENT* : Current modal element, or NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::Element_GetModal()
{
  return element_modal;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::ModalLayer_Invalidate()
* @brief      Drop the cached modal composition bitmap (next composite pass will Draw+capture again).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::ModalLayer_Invalidate()
{
  ModalLayer_Release();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_IsLayoutDrawDeferred(UI_ELEMENT* element)
* @brief      When the modal layer cache is valid, skip drawing the modal root during layout Update — the
*             composite pass will blit the opaque cache after all content. While rebuilding the cache,
*             modal_layer_compositing is set so Draw is allowed.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_IsLayoutDrawDeferred(UI_ELEMENT* element)
{
  if(!element) return false;
  if(!element_modal || element != element_modal) return false;
  if(!element_modal->IsVisible()) return false;
  if(!modal_layer_valid) return false;
  if(modal_layer_compositing) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_IsCompositing()
* @brief      True while the modal tree is being drawn onto the offscreen composition canvas.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_IsCompositing()
{
  return modal_layer_compositing;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_IsRebuildProtected(UI_ELEMENT* element)
* @brief      While the modal offscreen cache is valid, do not peel the modal subtree on the shared canvas.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_IsRebuildProtected(UI_ELEMENT* element)
{
  if(!element) return false;
  if(!element_modal || !element_modal->IsVisible()) return false;
  if(!modal_layer_valid) return false;
  if(modal_layer_compositing) return false;

  UI_ELEMENT* walk = element;
  while(walk)
    {
      if(walk == element_modal) return true;
      walk = walk->GetFather();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_SuppressesContentDraw(UI_ELEMENT* element)
* @brief      True when a non-modal element must not paint: it intersects the composed modal AABB.
* @note       ROOT CAUSE of ListBoxMenu / Edit punch-through (UI_Options video 2026-09-19): after the modal
*             was painted last, the next frame's Rebuild restored those elements' areas with PutBitmapNoAlpha
*             (parchment / menu / edit) straight into the keyboard, then Draw painted them again. Option B's
*             final DrawModalOnTop could not win against a continuous dirty cycle. Once modal_layer_valid, skip
*             that content until the modal is dismissed or invalidated.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_SuppressesContentDraw(UI_ELEMENT* element)
{
  if(!element) return false;
  if(!element_modal || !element_modal->IsVisible()) return false;
  if(!modal_layer_valid) return false;
  if(modal_layer_compositing) return false;

  UI_ELEMENT* walk = element;
  while(walk)
    {
      if(walk == element_modal) return false;
      walk = walk->GetFather();
    }

  if(!element->GetBoundaryLine()) return false;

  double mx = 0.0;
  double my = 0.0;
  double mw = 0.0;
  double mh = 0.0;
  if(!ModalLayer_Rect(element_modal, mx, my, mw, mh)) return false;

  double ew = element->GetBoundaryLine()->width;
  double eh = element->GetBoundaryLine()->height;
  if(ew <= 0.0 || eh <= 0.0) return false;

  double ex = UI_BOUNDARYLINE_EdgeLeft(element->GetXPosition(), ew);
  double ey = UI_BOUNDARYLINE_EdgeTop (element->GetYPosition(), eh);

  return (ex < mx + mw) && (ex + ew > mx) && (ey < my + mh) && (ey + eh > my);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ChromeCaption_SuppressesContentDraw(UI_ELEMENT* element)
* @brief      True when content intersects the visible custom-chrome caption band.
* @note       Content paints before chrome. A translucent caption (black,25) then shows ListBoxMenu / edit
*             through the title bar — the "chrome ghost" in UI_Options when the bar is visible. While the
*             caption is visible, content must not ink that band; chrome owns those pixels.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ChromeCaption_SuppressesContentDraw(UI_ELEMENT* element)
{
  #ifndef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  (void)element;
  return false;
  #else
  if(!element || !element->GetBoundaryLine()) return false;

  UI_LAYOUT* layout = Element_GetLayout(element);
  if(!layout) return false;
  if(IsCFGChromesLayout(layout)) return false;

  UI_SKIN* skin = layout->GetSkin();
  if(!skin || skin->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  GRPSCREEN* screen = ((UI_SKINCANVAS*)skin)->GetScreen();
  if(!screen || !screen->IsCFGChromesActive()) return false;
  if(screen->GetCFGChromes() && screen->GetCFGChromes()->GetUseNativeChromes()) return false;

  UI_LAYOUT* chromelayout = screen->GetCFGChromesLayout();
  if(!chromelayout) return false;

  UI_ELEMENT* caption = chromelayout->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
  if(!caption || !caption->IsVisible() || !caption->GetBoundaryLine()) return false;

  double cw = caption->GetBoundaryLine()->width;
  double ch = caption->GetBoundaryLine()->height;
  if(cw <= 0.0 || ch <= 0.0) return false;

  double cx = UI_BOUNDARYLINE_EdgeLeft(caption->GetXPosition(), cw);
  double cy = UI_BOUNDARYLINE_EdgeTop (caption->GetYPosition(), ch);

  double ew = element->GetBoundaryLine()->width;
  double eh = element->GetBoundaryLine()->height;
  if(ew <= 0.0 || eh <= 0.0) return false;

  double ex = UI_BOUNDARYLINE_EdgeLeft(element->GetXPosition(), ew);
  double ey = UI_BOUNDARYLINE_EdgeTop (element->GetYPosition(), eh);

  return (ex < cx + cw) && (ex + ew > cx) && (ey < cy + ch) && (ey + eh > cy);
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Overlay_SuppressesContentDraw(UI_ELEMENT* element)
* @brief      Content must not paint under the composed modal or the visible custom caption.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Overlay_SuppressesContentDraw(UI_ELEMENT* element)
{
  return ModalLayer_SuppressesContentDraw(element) || ChromeCaption_SuppressesContentDraw(element);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::Overlay_ClearSuppressedContentDirt()
* @brief      Drop MustReDraw on content that would peel/punch overlay bands before RebuildAllAreas.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::Overlay_ClearSuppressedContentDirt()
{
  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(!layout) continue;

      #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
      if(IsCFGChromesLayout(layout)) continue;
      #endif

      XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
      if(!roots) continue;

      XVECTOR<UI_ELEMENT*> stack;
      for(XDWORD i=0; i<roots->GetSize(); i++)
        {
          UI_ELEMENT* root = roots->Get(i);
          if(root) stack.Add(root);
        }

      while(!stack.IsEmpty())
        {
          UI_ELEMENT* el = stack.Get(stack.GetSize() - 1);
          stack.DeleteLast();
          if(!el) continue;

          XVECTOR<UI_ELEMENT*>* kids = el->GetComposeElements();
          if(kids)
            {
              for(XDWORD k=0; k<kids->GetSize(); k++)
                {
                  UI_ELEMENT* kid = kids->Get(k);
                  if(kid) stack.Add(kid);
                }
            }

          if(!Overlay_SuppressesContentDraw(el)) continue;

          el->SetMustReDraw(false);

          UI_SKIN* skin = layout->GetSkin();
          if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
            {
              UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)skin;
              GRP2DREBUILDAREA* area = skin_canvas->GetRebuildAreaByElement(el);
              if(area)
                {
                  skin_canvas->GetRebuildAreas()->Delete(area);
                  GEN_DELETE area;
                }
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::ModalLayer_Release()
* @brief      Free modal layer bitmap and clear validity.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::ModalLayer_Release()
{
  if(modal_layer_bitmap)
    {
      GEN_DELETE modal_layer_bitmap;
      modal_layer_bitmap = NULL;
    }

  modal_layer_valid = false;
  modal_layer_x     = 0.0;
  modal_layer_y     = 0.0;
  modal_layer_w     = 0.0;
  modal_layer_h     = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_Rect(UI_ELEMENT* modal, double& x, double& y, double& w, double& h)
* @brief      Screen AABB of the modal in the same (left, top, w, h) convention as rebuild areas / GetBitmap.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_Rect(UI_ELEMENT* modal, double& x, double& y, double& w, double& h)
{
  if(!modal || !modal->GetBoundaryLine()) return false;

  // Prefer VisibleRect when it matches resolved layout (Draw_Form fills it). VirtualKeyboard syncs
  // VisibleRect after CalculeBoundaryLine. Fallback: GetXPosition + BoundaryLine (PreDraw convention).
  if(modal->GetType() == UI_ELEMENT_TYPE_FORM || modal->GetType() == UI_ELEMENT_TYPE_MENU)
    {
      UI_ELEMENT_FORM* form = (UI_ELEMENT_FORM*)modal;
      UI_BOUNDARYLINE* vis  = form->GetVisibleRect();
      if(vis && vis->width > 0.0 && vis->height > 0.0)
        {
          w = vis->width;
          h = vis->height;
          x = UI_BOUNDARYLINE_EdgeLeft(vis->x, w);
          y = UI_BOUNDARYLINE_EdgeTop (vis->y, h);
          return true;
        }
    }

  w = modal->GetBoundaryLine()->width;
  h = modal->GetBoundaryLine()->height;
  if(w <= 0.0 || h <= 0.0) return false;

  x = UI_BOUNDARYLINE_EdgeLeft(modal->GetXPosition(), w);
  y = UI_BOUNDARYLINE_EdgeTop (modal->GetYPosition(), h);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ModalLayer_RebuildOffscreen(UI_SKINCANVAS* skin_canvas, UI_ELEMENT* modal)
* @brief      Draw the modal tree onto a dedicated offscreen canvas and store that AABB as modal_layer_bitmap.
* @note       Never snapshots the shared screen canvas — that was the failure mode of the first option-B pass
*             (ListBoxMenu formbackdrop already punched into the keyboard rect before capture).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ModalLayer_RebuildOffscreen(UI_SKINCANVAS* skin_canvas, UI_ELEMENT* modal)
{
  if(!skin_canvas || !modal) return false;

  GRPSCREEN*   screen = skin_canvas->GetScreen();
  GRP2DCANVAS* live   = skin_canvas->GetCanvas();
  if(!screen || !live) return false;

  double x = 0.0;
  double y = 0.0;
  double w = 0.0;
  double h = 0.0;
  if(!ModalLayer_Rect(modal, x, y, w, h)) return false;

  XDWORD sw = screen->GetWidth();
  XDWORD sh = screen->GetHeight();
  if(!sw || !sh) return false;

  GRPPROPERTIES properties;
  properties.CopyPropertysFrom(live);
  properties.SetPosition(0, 0);
  properties.SetSize(sw, sh);

  GRP2DCANVAS* offscreen = GEN_GRPFACTORY.CreateCanvas(&properties);
  if(!offscreen) return false;

  offscreen->SetWidth((double)sw);
  offscreen->SetHeight((double)sh);

  bool ok = false;

  if(offscreen->Buffer_Create())
    {
      offscreen->VectorFont_CopyFrom(live);

      // Opaque clear: holes in the modal AABB must not stay "transparent" when blitted with PutBitmapNoAlpha.
      GRP2DCOLOR_RGBA8 clearcolor(0, 0, 0, 255);
      offscreen->Clear(&clearcolor);

      GRP2DCANVAS* saved_rebuild_canvas = skin_canvas->GetTargetCanvas();

      modal_layer_compositing = true;
      skin_canvas->SetCanvasOverride(offscreen);
      skin_canvas->SetTargetCanvas(offscreen);

      Elements_SetToRedraw(modal);
      skin_canvas->Draw(modal);

      skin_canvas->SetCanvasOverride(NULL);
      if(saved_rebuild_canvas) skin_canvas->SetTargetCanvas(saved_rebuild_canvas);
      modal_layer_compositing = false;

      GRPBITMAP* snapshot = offscreen->GetBitmap(x, y, w, h);
      if(snapshot)
        {
          if(modal_layer_bitmap)
            {
              GEN_DELETE modal_layer_bitmap;
              modal_layer_bitmap = NULL;
            }

          modal_layer_bitmap = snapshot;
          modal_layer_x      = x;
          modal_layer_y      = y;
          modal_layer_w      = w;
          modal_layer_h      = h;
          modal_layer_valid  = true;
          ok = true;
        }
    }

  GEN_GRPFACTORY.DeleteCanvas(offscreen);
  return ok;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Element_DrawModalOnTop()
* @brief      Modal composition layer (option B): paint the modal tree LAST on the live shared canvas.
* @note       Content and chrome may freely punch the modal AABB earlier in the frame. This pass forces a full
*             Draw of the modal on top (formbackdrop/optionbackdrop skipped via ModalLayer_IsCompositing) so the
*             modal always wins without depending on an offscreen GetBitmap cache.
* @ingroup    USERINTERFACE
*
* @return     bool : true if the modal was painted on top.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Element_DrawModalOnTop()
{
  if(!element_modal) return false;
  if(!element_modal->IsVisible())
    {
      ModalLayer_Release();
      return false;
    }

  UI_LAYOUT* layout = Element_GetLayout(element_modal);
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  if(!skin_canvas) return false;

  GRP2DCANVAS* live = skin_canvas->GetCanvas();
  if(!live) return false;

  if(xmutex_modal) xmutex_modal->Lock();

  double x = 0.0;
  double y = 0.0;
  double w = 0.0;
  double h = 0.0;
  if(ModalLayer_Rect(element_modal, x, y, w, h))
    {
      modal_layer_x = x;
      modal_layer_y = y;
      modal_layer_w = w;
      modal_layer_h = h;

      // Opaque wipe of the modal AABB on the LIVE canvas before drawing keys/fill. Guarantees no sibling
      // formbackdrop/chrome pixels remain in the intersection even if the form fill uses round-rect / alpha.
      UI_COLOR* fillsrc = NULL;
      if(element_modal->GetType() == UI_ELEMENT_TYPE_FORM || element_modal->GetType() == UI_ELEMENT_TYPE_MENU)
        {
          UI_ELEMENT_FORM* form = (UI_ELEMENT_FORM*)element_modal;
          fillsrc = form->IsBackgroundColorSet() ? form->GetBackgroundColor() : form->GetColor();
        }
      if(!fillsrc) fillsrc = element_modal->GetColor();

      XBYTE fr = fillsrc ? fillsrc->GetRed()   : 200;
      XBYTE fg = fillsrc ? fillsrc->GetGreen() : 200;
      XBYTE fb = fillsrc ? fillsrc->GetBlue()  : 200;
      GRP2DCOLOR_RGBA8 solid(fr, fg, fb, 255);
      GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);
      live->SetFillColor(&solid);
      live->SetLineColor(&noline);
      live->SetLineWidth(1.0f);
      // GEN Rectangle: (x1,y1 bottom-left) (x2,y2 top-right) — same as Draw_Form.
      live->Rectangle(x, y + h, x + w, y, true);
    }

  // Paint modal last on the live canvas. Compositing flag skips formbackdrop/optionbackdrop restores that
  // would re-introduce ListBoxMenu pixels under the keys.
  modal_layer_compositing = true;
  Elements_SetToRedraw(element_modal);
  skin_canvas->Draw(element_modal);
  modal_layer_compositing = false;

  // Mark cache "valid" only so layout Update defers the modal root (avoids drawing it twice per frame).
  // The authoritative pixels are always those just painted on the live canvas above — no offscreen bitmap.
  modal_layer_valid = true;
  if(modal_layer_bitmap)
    {
      GEN_DELETE modal_layer_bitmap;
      modal_layer_bitmap = NULL;
    }

  if(xmutex_modal) xmutex_modal->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_SetToRedraw()
* @brief      Elements set to redraw
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_SetToRedraw()
{
  bool status = false;

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout)
        {
          status = layout->Elements_SetToRedraw();
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
* @brief      Elements set to redraw
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
{
  bool status = false;

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout)
        {     
          status = layout->Elements_SetToRedraw(element, recursive);         
        }
    }

  // A scrolleable ancestor paints its scrollbar as an overlay and (now) owns the single rebuild area covering its whole
  // content. If a descendant repaints, that overlay/content must be repainted by the area owner. The owner is the
  // OUTERMOST clipping scrolleable ancestor (an inner one — e.g. a MultiOption — owns no area, since it is itself inside
  // the outer container). So walk the whole chain, keep the outermost, and mark it (recursively) once.
  if(element)
    {
      UI_ELEMENT* father = element->GetFather();
      UI_ELEMENT* owner  = NULL;
      while(father)
        {
          UI_PROPERTY_SCROLLEABLE* sc = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(father);
          if(sc && sc->Scroll_NeedClip()) owner = father;      // keep the outermost clipping scrolleable
          father = father->GetFather();
        }

      if(owner)
        {
          for(XDWORD c=0; c<layouts.GetSize(); c++)
            {
              UI_LAYOUT* layout = layouts.Get(c);
              if(layout) layout->Elements_SetToRedraw(owner, true);   // mark container + content as one unit (no ghosts:
            }                                                         // descendants own no areas, see PreDrawFunction)
        }
    }
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::PropagateRebuildOverlapDirtAcrossSharedCanvases()
* @brief      Cross-skin MarkOverlapping for every pair of UI_SKINCANVAS that share the same GRP2DCANVAS.
* @note       Content and chrome keep separate rebuild-area lists but paint into one viewport. Without this,
*             restoring/redrawing ListBoxMenu could leave Option text in the chrome title band (and the reverse).
*             Fixed-point: newly dirtied neighbours may overlap further areas. Also dirties the modal when any
*             shared-canvas dirty rect intersects its bounds so the screen modal layer repaints the hole.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::PropagateRebuildOverlapDirtAcrossSharedCanvases()
{
  XVECTOR<UI_SKINCANVAS*> skins;

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(!layout || !layout->GetSkin()) continue;
      if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;

      UI_SKINCANVAS* skin = (UI_SKINCANVAS*)layout->GetSkin();
      if(!skin || !skin->GetCanvas()) continue;
      skins.Add(skin);
    }

  if(skins.GetSize() < 2 && !(element_modal && element_modal->IsVisible()))
    {
      return;
    }

  bool changed = true;
  int  guard   = 0;

  while(changed && guard < 8)
    {
      changed = false;
      guard++;

      for(XDWORD a=0; a<skins.GetSize(); a++)
        {
          UI_SKINCANVAS* skin_a = skins.Get(a);
          if(!skin_a) continue;

          XVECTOR<GRP2DREBUILDAREA*>* areas_a = skin_a->GetRebuildAreas();
          if(!areas_a) continue;

          for(XDWORD ia=0; ia<areas_a->GetSize(); ia++)
            {
              GRP2DREBUILDAREA* area_a = areas_a->Get(ia);
              if(!area_a || !area_a->GetBitmap()) continue;

              UI_ELEMENT* elem_a = (UI_ELEMENT*)area_a->GetExtraData();
              if(!elem_a) continue;
              if((!elem_a->MustReDraw()) && elem_a->IsVisible()) continue;

              double ax1 = area_a->GetXPos();
              double ay1 = area_a->GetYPos();
              double ax2 = ax1 + (double)area_a->GetBitmap()->GetWidth();
              double ay2 = ay1 + (double)area_a->GetBitmap()->GetHeight();

              // Modal layer (option B): do NOT mark the modal MustReDraw here. Overlapping content dirty
              // (ListBoxMenu formbackdrop) every frame would force perpetual Draw+recapture and bake the punch
              // into the modal layer cache. The composite pass blits the opaque cache after content instead.

              for(XDWORD b=0; b<skins.GetSize(); b++)
                {
                  if(a == b) continue;

                  UI_SKINCANVAS* skin_b = skins.Get(b);
                  if(!skin_b || skin_b->GetCanvas() != skin_a->GetCanvas()) continue;

                  XVECTOR<GRP2DREBUILDAREA*>* areas_b = skin_b->GetRebuildAreas();
                  if(!areas_b) continue;

                  for(XDWORD ib=0; ib<areas_b->GetSize(); ib++)
                    {
                      GRP2DREBUILDAREA* area_b = areas_b->Get(ib);
                      if(!area_b || !area_b->GetBitmap()) continue;

                      UI_ELEMENT* elem_b = (UI_ELEMENT*)area_b->GetExtraData();
                      if(!elem_b || elem_b->MustReDraw()) continue;

                      double bx1 = area_b->GetXPos();
                      double by1 = area_b->GetYPos();
                      double bx2 = bx1 + (double)area_b->GetBitmap()->GetWidth();
                      double by2 = by1 + (double)area_b->GetBitmap()->GetHeight();

                      if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)
                        {
                          Elements_SetToRedraw(elem_b);
                          changed = true;
                        }
                    }
                }
            }
        }

      // Content dirty without a rebuild area yet can still overlap chrome bounds (e.g. scrolled option text
      // near y=0). Walk chrome layout roots and dirty them when any dirty content element on the same canvas
      // intersects their boundary.
      #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
      for(XDWORD c=0; c<layouts.GetSize(); c++)
        {
          UI_LAYOUT* content = layouts.Get(c);
          if(!content || IsCFGChromesLayout(content)) continue;
          if(!content->GetSkin() || content->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;

          UI_SKINCANVAS* content_skin = (UI_SKINCANVAS*)content->GetSkin();
          if(!content_skin || !content_skin->GetCanvas()) continue;

          for(XDWORD d=0; d<layouts.GetSize(); d++)
            {
              UI_LAYOUT* chrome = layouts.Get(d);
              if(!chrome || !IsCFGChromesLayout(chrome)) continue;
              if(!chrome->GetSkin() || chrome->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;

              UI_SKINCANVAS* chrome_skin = (UI_SKINCANVAS*)chrome->GetSkin();
              if(!chrome_skin || chrome_skin->GetCanvas() != content_skin->GetCanvas()) continue;

              XVECTOR<UI_ELEMENT*>* chrome_roots = chrome->Elements_Get();
              if(!chrome_roots) continue;

              // Prefer dirty rebuild areas on content; also scan dirty root elements' bounds.
              XVECTOR<GRP2DREBUILDAREA*>* careas = content_skin->GetRebuildAreas();
              for(XDWORD ir=0; ir<chrome_roots->GetSize(); ir++)
                {
                  UI_ELEMENT* chrome_el = chrome_roots->Get(ir);
                  if(!chrome_el || chrome_el->MustReDraw() || !chrome_el->IsVisible()) continue;
                  if(!chrome_el->GetBoundaryLine()) continue;

                  double cw = chrome_el->GetBoundaryLine()->width;
                  double ch = chrome_el->GetBoundaryLine()->height;
                  double cx1 = UI_BOUNDARYLINE_EdgeLeft(chrome_el->GetXPosition(), cw);
                  double cy1 = UI_BOUNDARYLINE_EdgeTop (chrome_el->GetYPosition(), ch);
                  double cx2 = cx1 + cw;
                  double cy2 = cy1 + ch;

                  bool hit = false;

                  if(careas)
                    {
                      for(XDWORD ia=0; ia<careas->GetSize() && !hit; ia++)
                        {
                          GRP2DREBUILDAREA* area = careas->Get(ia);
                          if(!area || !area->GetBitmap()) continue;
                          UI_ELEMENT* ea = (UI_ELEMENT*)area->GetExtraData();
                          if(!ea || ((!ea->MustReDraw()) && ea->IsVisible())) continue;

                          double ax1 = area->GetXPos();
                          double ay1 = area->GetYPos();
                          double ax2 = ax1 + (double)area->GetBitmap()->GetWidth();
                          double ay2 = ay1 + (double)area->GetBitmap()->GetHeight();
                          if(ax1 < cx2 && ax2 > cx1 && ay1 < cy2 && ay2 > cy1) hit = true;
                        }
                    }

                  if(!hit)
                    {
                      XVECTOR<UI_ELEMENT*>* croots = content->Elements_Get();
                      if(croots)
                        {
                          for(XDWORD ie=0; ie<croots->GetSize() && !hit; ie++)
                            {
                              UI_ELEMENT* el = croots->Get(ie);
                              if(!el || !el->MustReDraw() || !el->IsVisible() || !el->GetBoundaryLine()) continue;
                              double ew = el->GetBoundaryLine()->width;
                              double eh = el->GetBoundaryLine()->height;
                              double ax1 = UI_BOUNDARYLINE_EdgeLeft(el->GetXPosition(), ew);
                              double ay1 = UI_BOUNDARYLINE_EdgeTop (el->GetYPosition(), eh);
                              double ax2 = ax1 + ew;
                              double ay2 = ay1 + eh;
                              if(ax1 < cx2 && ax2 > cx1 && ay1 < cy2 && ay2 > cy1) hit = true;
                            }
                        }
                    }

                  if(hit)
                    {
                      Elements_SetToRedraw(chrome_el);
                      changed = true;
                    }
                }
            }
        }
      #endif
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_RebuildDrawAreas()
* @brief      Elements rebuild draw areas
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_RebuildDrawAreas()
{  
  bool status = false; 

  // Cross-skin dirty closure BEFORE any PutBitmapNoAlpha restore, so chrome/content/modal overlaps are
  // peeled and redrawn in the same frame (screen composition layers).
  PropagateRebuildOverlapDirtAcrossSharedCanvases();

  // After the modal has been composed once / while caption is visible: freeze content that would peel
  // those overlay bands (keyboard punch-through, title-bar ghosts). See Overlay_SuppressesContentDraw.
  Overlay_ClearSuppressedContentDirt();

  for(XDWORD c=0; c<layouts.GetSize(); c++)
    {
      UI_LAYOUT* layout = layouts.Get(c);
      if(layout)
        {    
          if(layout->GetSkin())
            {
              UnSelectedElement();  

              switch(layout->GetSkin()->GetDrawMode())
                {
                  case UI_SKIN_DRAWMODE_UNKNOWN   : break;

                  case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin();
                                                      if(skincanvas) 
                                                        {
                                                          // Fase 3: peel dirty rects on the design canvas when scaled.
                                                          UIScale_BeginFrame(layout);
                                                          status = skincanvas->RebuildAllAreas();
                                                          UIScale_EndFrame(layout);
                                                        }
                                                    }
                                                    break;

                  case UI_SKIN_DRAWMODE_CONTEXT   : break;
                }
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_RebuildDrawAreas(UI_LAYOUT* layout)
* @brief      Elements rebuild draw areas
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_RebuildDrawAreas(UI_LAYOUT* layout)
{  
  bool status = false; 

  if(!layout)
    {
      return false;
    }

  if(!layout->GetSkin())
    {
      return false;
    }

  UnSelectedElement();  

  // Same cross-skin closure as the global rebuild: a single-layout rebuild can still wipe pixels owned by
  // another skin on the shared canvas (content vs chrome).
  PropagateRebuildOverlapDirtAcrossSharedCanvases();
  Overlay_ClearSuppressedContentDirt();

  switch(layout->GetSkin()->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin();
                                          if(skincanvas) 
                                            {
                                              status = skincanvas->RebuildAllAreas();                                                                                
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT   : break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_RebuildDrawAreas(UI_LAYOUT* layout, UI_ELEMENT* element)
* @brief      Elements rebuild draw areas
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_RebuildDrawAreas(UI_LAYOUT* layout, UI_ELEMENT* element)
{
  bool status = false; 
  
  if(!layout)
    {
      return false;
    }

  if(!layout->GetSkin())
    {
      return false;
    }

  UnSelectedElement();  

  switch(layout->GetSkin()->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin();
                                          if(skincanvas) 
                                            {
                                              status = skincanvas->RebuildAllAreas(element);                                                                                
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT   : break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_RebuildDrawAreas(XCHAR* layoutname)
* @brief      Elements rebuild draw areas
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_RebuildDrawAreas(XCHAR* layoutname)
{ 
  UI_LAYOUT* layout = NULL;
  
  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }
  
  return Elements_RebuildDrawAreas(layout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::Elements_RebuildDrawAreas(XCHAR* layoutname, UI_ELEMENT* element)
* @brief      Elements rebuild draw areas
* @ingroup    USERINTERFACE
* 
* @param[in]  layoutname : Layoutname pointer to use.
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Elements_RebuildDrawAreas(XCHAR* layoutname, UI_ELEMENT* element)
{
  UI_LAYOUT* layout = NULL;
  
  layout = Layouts_Get(layoutname);
  if(!layout)
    {
      return false;
    }
  
  return Elements_RebuildDrawAreas(layout, element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_MANAGER::GetOutputTextChangeID(XSTRING* text, int start, XSTRING& value)
* @brief      Get output text change ID
* @ingroup    USERINTERFACE
* 
* @param[in]  text : Text to use.
* @param[in]  start : Start value.
* @param[in]  value : Value value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_MANAGER::GetOutputTextChangeID(XSTRING* text, int start, XSTRING& value)
{
  if(!text) 
    {
      return false;
    }
  
  int ini_mask;
  int end_mask;

  ini_mask = text->Find(UI_MANAGER_LAYOUT_TEXT_INI, true, start);
  if(ini_mask != XSTRING_NOTFOUND)
    {
      end_mask = text->Find(UI_MANAGER_LAYOUT_TEXT_END, true, ini_mask);
      if(end_mask != XSTRING_NOTFOUND)
        {
          text->Copy(ini_mask+2, end_mask, value);

          return ini_mask;
        }       
    }                             

  return XSTRING_NOTFOUND;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ANIMATION* UI_MANAGER::GetOrAddAnimationCache(UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, XCHAR* name, XCHAR* resource, GRP2DCANVAS* referencecanvas, double width, double height)
* @brief      Get or add animation cache
* @note       When resource is a vector file (SVG, DXF...) AND referencecanvas/width/height are supplied, the
*             resource is rasterized at that exact size and the cache key folds in the size (e.g. "icon.svg@32x32"),
*             so the same vector file requested at two different sizes never collides in the cache and each size
*             gets its own cached bitmap. Bitmap-file callers (referencecanvas = NULL, the default) are completely
*             unaffected: same lookup, same key, same LoadFromFile() path as before.
* @ingroup    USERINTERFACE
* 
* @param[in]  drawmode : Drawmode value.
* @param[in]  grppropertymode : Grppropertymode value.
* @param[in]  name : Name to use.
* @param[in]  resource : Resource pointer to use.
* @param[in]  referencecanvas : Canvas to rasterize vector resources with (NULL = bitmap-only, legacy behavior).
* @param[in]  width : Target width, in pixels, to rasterize a vector resource at (ignored for bitmap files).
* @param[in]  height : Target height, in pixels, to rasterize a vector resource at (ignored for bitmap files).
* 
* @return     UI_ANIMATION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ANIMATION* UI_MANAGER::GetOrAddAnimationCache(UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, XCHAR* name, XCHAR* resource, GRP2DCANVAS* referencecanvas, double width, double height)
{
  UI_ANIMATION* animation = NULL;
  bool          status    = false; 

  bool isvectorsized = (referencecanvas && (width > 0.0) && (height > 0.0) && IsVectorResource(resource));

  XSTRING cachekey;

  cachekey = resource;

  if(isvectorsized)
    {
      XSTRING sizepart;

      sizepart.Format(__L("@%dx%d"), (int)width, (int)height);
      cachekey.Add(sizepart);
    }

  animation = GEN_UI_ANIMATIONS.Get(name);
  if(animation) 
    {
      return animation;
    }

  animation = GEN_UI_ANIMATIONS.Get(cachekey);
  if(animation) 
    {
      return animation;
    }

  animation = GEN_NEW UI_ANIMATION();
  if(!animation) 
    {
      return NULL;
    }
  
  XSTRING resourcename;
  
  resourcename = resource;

  if(isvectorsized)
    {
      status = animation->LoadFromFileVector(resourcename, referencecanvas, width, height);
    }
   else
    {
      switch(drawmode)
        {
          case UI_SKIN_DRAWMODE_UNKNOWN   : break;

          case UI_SKIN_DRAWMODE_CANVAS    : status = animation->LoadFromFile(resourcename, grppropertymode);                                                                           
                                            break;

          case UI_SKIN_DRAWMODE_CONTEXT   : break;
        } 
    }

  if(status)   
    {      
      animation->GetName()->Set(name);  
      animation->GetResource()->Set(cachekey.Get());

      GEN_UI_ANIMATIONS.Add(animation);

      return animation;
    }
  
  GEN_DELETE animation;

  return NULL;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::IsVectorResource(XCHAR* resource)
* @brief      Is vector resource
* @note       Detects a vector resource (SVG, DXF...) by its file extension, the same way GRPBITMAPFILE::GetTypeFromExtenxion
*             detects a bitmap file type, so GetOrAddAnimationCache() knows whether to rasterize it (mandatory
*             target size) instead of decoding it as a regular bitmap file.
* @ingroup    USERINTERFACE
* 
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::IsVectorResource(XCHAR* resource)
{
  if(!resource) return false;

  XPATH   pathresource;
  XSTRING ext;

  pathresource = resource;

  if(!pathresource.GetExt(ext)) return false;

  if(!ext.Compare(__L(".svg"), true)) return true;
  if(!ext.Compare(__L(".dxf"), true)) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SetLevelAuto(UI_ELEMENT* element, UI_ELEMENT* father, XDWORD addlevel)
* @brief      Set level auto
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  father : Father pointer to use.
* @param[in]  addlevel : Addlevel value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SetLevelAuto(UI_ELEMENT* element, UI_ELEMENT* father, XDWORD addlevel)
{
  if(father)
    {
      XDWORD zlevel_father = father->GetZLevel();    
      if(element) 
        {
          element->SetZLevel(element->GetZLevel() + zlevel_father + addlevel);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SetPreselectElement(UI_ELEMENT* element)
* @brief      Set preselect element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SetPreselectElement(UI_ELEMENT* element)
{
  preselect_element = element;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::ResetPreselect()
* @brief      Reset preselect
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ResetPreselect()
{
  if(!preselect_element) 
    {
      return false;
    }

  preselect_element->SetPreSelect(false); 
   
  Elements_SetToRedraw(preselect_element);                                                                

  preselect_element = NULL; 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SendEvent(int event, ...)
* @brief      Send event
* @ingroup    USERINTERFACE
* 
* @param[in]  event : Event to process.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SendEvent(int event, ...)
{
  UI_XEVENT xevent(this, event);
  va_list   arg;
  bool      postevent = false;
  bool      status    = false;

  if(xmutex_UIevent) 
    {
      xmutex_UIevent->Lock();
    }

  va_start(arg, event);
  
  switch(event)
    {
      case UI_XEVENT_TYPE_INPUT_CURSOR_MOVE         : 
      case UI_XEVENT_TYPE_INPUT_SELECCTION          : { int x = (int)va_arg(arg, int);
                                                        int y = (int)va_arg(arg, int);

                                                        xevent.SetXPos((XDWORD)x);
                                                        xevent.SetYPos((XDWORD)y);

                                                        postevent = true;
                                                      }
                                                      break;

      case UI_XEVENT_TYPE_INPUT_CURSOR_MOTION       : { void* ptr = (void*)va_arg(arg, void*);
                                                        if(ptr)
                                                          {
                                                            INPCURSORMOTION* cursormotion = (INPCURSORMOTION*)ptr;
                                                            if(cursormotion)
                                                              {                  
                                                                xevent.SetCursorMotion(cursormotion);
                                                                postevent = true;
                                                              }
                                                          }  
                                                      }
                                                      break;  

      case UI_XEVENT_TYPE_OUTPUT_SELECTED           : { void* ptr = (void*)va_arg(arg, void*);
                                                        if(ptr)
                                                          {   
                                                            UI_ELEMENT* element = (UI_ELEMENT*)ptr;
                                                            if(element) 
                                                              {
                                                                UI_ELEMENT* element_father = element->GetFather();
                                                        
                                                                if(element_father) 
                                                                  {
                                                                    if(!element_father->IsVisible())
                                                                      {
                                                                        break;
                                                                      }
                                                                  }

                                                                if(element->IsVisible()) 
                                                                  { 
                                                                    //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("UI Element [%s]: Selected. "), element->GetName()->Get());

                                                                    xevent.SetElement(element);          
                                                                    postevent = true;
                                                                  }
                                                              }
                                                          }
                                                      }
                                                      break;

      case UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE       : { void* ptr = (void*)va_arg(arg, void*);
                                                        if(ptr)
                                                          {   
                                                            UI_ELEMENT* element = (UI_ELEMENT*)ptr;
                                                            if(element) 
                                                              {
                                                                XSTRING* maskvalue = (XSTRING*)va_arg(arg, void*);
                                                                if(maskvalue)  xevent.GetMaskTextValue()->Set(maskvalue->Get());

                                                                /*
                                                                ChangeAutomaticLiteralText((UI_ELEMENT_TEXT*)element, maskvalue, xevent.GetMaskText());
                                                                
                                                                if(!xevent.GetMaskText()->IsEmpty())
                                                                  {
                                                                    XSTRING* maskresolve = (XSTRING*)va_arg(arg, void*);
                                                                    if(maskresolve) maskresolve->Set(xevent.GetMaskText()->Get()); 
                                                                    postevent = false;
                                                                  }
                                                                 else   
                                                                 */
                                                                  {
                                                                    xevent.SetElement(element);  
                                                                    postevent = true;    
                                                                  }
                                                              }
                                                          }
                                                      }  
                                                      break;

       case UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS    : { void* ptr = (void*)va_arg(arg, void*);
                                                        if(ptr)
                                                          {   
                                                            UI_ELEMENT* element = (UI_ELEMENT*)ptr;
                                                            if(element) 
                                                              {
                                                                xevent.SetElement(element);                                                                        
                                                                postevent = true;
                                                              }  
                                                          }
                                                      }
                                                      break;

                                      default       : break;



    }
  
  if(postevent) 
    {          
      status = PostEvent(&xevent);         
      if(status)
        {  
          switch(event)
            {
              case UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE   : { XSTRING* maskresolve = (XSTRING*)va_arg(arg, void*);
                                                            if(maskresolve) maskresolve->Set(xevent.GetMaskTextResolved()->Get()); 
                                                          }  
                                                          break;

                                          default       : break;
            }
        }  
    }

  va_end(arg);

  if(xmutex_UIevent) xmutex_UIevent->UnLock();

  return status;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::ChangeAutomaticTextElementValue(UI_ELEMENT* element, XSTRING* maskvalue, XSTRING* maskresolved)
* @brief      Change automatic text element value
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  maskvalue : Maskvalue pointer to use.
* @param[in]  maskresolved : Maskresolved pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ChangeAutomaticTextElementValue(UI_ELEMENT* element, XSTRING* maskvalue, XSTRING* maskresolved)
{
  if(!element)        return false;
  if(!maskvalue)      return false;
  if(!maskresolved)   return false;

  UI_ELEMENT* father = element->GetFather();
  if(father)
    {
      if(father->GetType() == UI_ELEMENT_TYPE_PROGRESSBAR     || 
         father->GetType() == UI_ELEMENT_TYPE_PROGRESSRADIAL  ||
         father->GetType() == UI_ELEMENT_TYPE_PROGRESSIMAGE)
        {
          if(!maskvalue->Compare(__L("PROGRESSBAR_PERCENT"), true))
            {
              float level = 0.0f;

              switch(father->GetType())                                                          // read the level from the ACTUAL father type: each progress class owns its
                {                                                                                // own non-virtual GetLevel()/level member at a different offset, so a blind
                  case UI_ELEMENT_TYPE_PROGRESSBAR    : level = ((UI_ELEMENT_PROGRESSBAR*)father)->GetLevel();    break;   // (PROGRESSBAR*) cast read the wrong memory for IMAGE -> always 0%.
                  case UI_ELEMENT_TYPE_PROGRESSRADIAL : level = ((UI_ELEMENT_PROGRESS_RADIAL*)father)->GetLevel(); break;
                  case UI_ELEMENT_TYPE_PROGRESSIMAGE  : level = ((UI_ELEMENT_PROGRESS_IMAGE*)father)->GetLevel();  break;
                  default                             : break;
                }

              maskresolved->Format(__L("%d"), (int)level);
              maskresolved->Add(__L("\x25"));

              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::ChangeTextElementValue(UI_ELEMENT* element, XSTRING* text, XSTRING& value)
* @brief      Change text element value
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  text : Text to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ChangeTextElementValue(UI_ELEMENT* element, XSTRING* text, XSTRING& value)
{
  XSTRING  maskvalue;
  XSTRING  maskresolve;  
  int      start      = 0;
  int      new_start  = 0;
  bool     change     = false;
  
  do{ new_start = GEN_USERINTERFACE.GetOutputTextChangeID(text, start, maskvalue);

      if(new_start != XSTRING_NOTFOUND)
        {
          XSTRING subtext;
       
          text->Copy(start, new_start, subtext);
          if(!subtext.IsEmpty()) value.Add(subtext);
           
          maskresolve.Empty();

          if(ChangeAutomaticTextElementValue(element, &maskvalue, &maskresolve))
            {                
              value.Add(maskresolve);                 
              change = true;                    
            }
           else 
            {
              SendEvent(UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE, element, &maskvalue, &maskresolve);
              if(!maskresolve.IsEmpty()) 
                {
                  value.Add(maskresolve);                 
                  change = true;              
                }  
            }

          start = new_start + maskvalue.GetSize() + 3;               

        }
       else 
        {
          XSTRING subtext;

          text->Copy(start, subtext);
          value.Add(subtext);                                  
        }

    } while(new_start != XSTRING_NOTFOUND);

  return change;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::ChangeTextElementValue(UI_LAYOUT* layout, UI_ELEMENT* element)
* @brief      Change text element value
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ChangeTextElementValue(UI_LAYOUT* layout, UI_ELEMENT* element)
{
  if(!element) 
    {
      return false;
    }

  if(!layout)
    {
      return false;
    }

  if(!layout->GetSkin())
    {
      return false;
    }

  bool change = false;

  switch(element->GetType())
    {
      case  UI_ELEMENT_TYPE_TEXT      : { UI_ELEMENT_TEXT*  element_text = (UI_ELEMENT_TEXT*)element;
                                          XSTRING           resolve;

                                          if(element_text->GetMaskText()->IsEmpty())    return false;
                                                                                                                         
                                          change = ChangeTextElementValue(element_text, element_text->GetMaskText(), resolve);   
                                                                                                            
                                          if(change)
                                            {                                                                                                                                                                                                                                                                                                                                                                                                                    
                                              switch(layout->GetSkin()->GetDrawMode())
                                                {
                                                  case UI_SKIN_DRAWMODE_UNKNOWN   : break;

                                                  case UI_SKIN_DRAWMODE_CANVAS    : { double width  = 0.0f;
                                                                                      double height = 0.0f;
                                                                                      
                                                                                      UI_SKINCANVAS* ui_skincanvas = (UI_SKINCANVAS*)layout->GetSkin(); 

                                                                                      UI_ELEMENT* father =(UI_ELEMENT_TEXT*)element->GetFather();
                                                                                      if(father)
                                                                                        {
                                                                                          width  = father->GetBoundaryLine()->width;
                                                                                          height = father->GetBoundaryLine()->height;
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                          width  =  ui_skincanvas->GetCanvas()->GetWidth();
                                                                                          height =  ui_skincanvas->GetCanvas()->GetHeight();
                                                                                        }

                                                                                      // NOTE: Elements_SetToRedraw(father) used to fire right here, unconditionally, every single
                                                                                      // tick this function runs for ANY text element carrying a live "#[MASK]" -- i.e. every frame,
                                                                                      // whether the resolved value actually differs from what is already on screen or not (the
                                                                                      // "change" flag above only means the mask was successfully RESOLVED this tick, not that its
                                                                                      // value is new). On a layout with several live masks (temperature, CPU%, RAM%, clock...) that
                                                                                      // marked each one's father -- e.g. the CARD itself -- dirty tens of times per second instead of
                                                                                      // ~once per second (whenever the underlying value truly ticks over). Every one of those
                                                                                      // spurious redraws re-blends that card's (and, through UI_SkinCanvas's rebuild-area overlap,
                                                                                      // its neighbors') box-shadow on top of whatever was already there, which is what let the
                                                                                      // shadow at overlapping rounded corners darken far faster than intended, saturating to near
                                                                                      // black within the first second or two of runtime instead of staying stable. Moved below, into
                                                                                      // the same "value actually changed" branch that already gates the boundary-line rebuild and
                                                                                      // Elements_SetToRedraw(element), so the father redraws exactly as often as the child does --
                                                                                      // once per real value change, not once per frame.

                                                                                      // NOTE: the boundary line must be zeroed (to force CalculeBoundaryLine_AllElements to
                                                                                      // re-measure an AUTO-sized box, e.g. a centered "37%" caption) ONLY when the resolved
                                                                                      // value actually changed. This used to run unconditionally on every single tick this
                                                                                      // function was called (i.e. every frame there is a live mask on the element, changed
                                                                                      // or not): on every tick where the value happened to stay the SAME, the width/height
                                                                                      // were zeroed but never rebuilt (the rebuild below only runs on an actual change), so
                                                                                      // Draw_Text's text-align:center offset -- (boundarywidth - textwidth)/2 -- was computed
                                                                                      // against a bogus width of 0 for as long as the value stayed put, permanently shifting
                                                                                      // the text left by half its own width. The ONE frame where the value DID change was the
                                                                                      // only frame with a correctly rebuilt (and therefore correctly centered) box, which is
                                                                                      // exactly why a live value looked both permanently off-center AND "flashed" wider/
                                                                                      // narrower every time it changed, most visibly on the CPU/RAM usage ring captions.
                                                                                      if(element_text->GetText()->Compare(resolve.Get(), true))
                                                                                        {
                                                                                          element_text->GetText()->Set(resolve);

                                                                                          element->GetBoundaryLine()->height  = 0;
                                                                                          element->GetBoundaryLine()->width   = 0;

                                                                                          ui_skincanvas->CalculeBoundaryLine_AllElements(element, false);
                                                                                          ui_skincanvas->CalculePosition(element, width, height);

                                                                                          if(father && ((father->GetType() == UI_ELEMENT_TYPE_PROGRESSBAR) || (father->GetType() == UI_ELEMENT_TYPE_PROGRESSRADIAL) || (father->GetType() == UI_ELEMENT_TYPE_PROGRESSIMAGE)))
                                                                                            {
                                                                                              ui_skincanvas->CalculateBoundaryLine(father, false);   // re-apply progress allocationtext/boundary with the new text size (don't leave the text mis-placed by the plain CalculePosition above)
                                                                                            }

                                                                                          // Phase 4 ("migración del ejemplo", second sub-step, live-update follow-up): CalculePosition() just above is
                                                                                          // the LEGACY (pre-Flexbox) positioning pass -- it knows nothing about "display: flex" and simply drops the
                                                                                          // element back near its father's origin using xpos/margin alone, ignoring every sibling. That is harmless for
                                                                                          // a non-flex father (its only caller before this sub-step), but for a flex father it silently undid
                                                                                          // UI_LAYOUTENGINE::RunLayout()'s one-time, load-time positioning the moment this text's own auto-sized box
                                                                                          // changed width/height (e.g. dashboard.xml's "#[FOOTER_SO]"/"#[FOOTER_UPTIME]" ticking every second) --
                                                                                          // collapsing every live text in the row onto the same spot instead of leaving it where the flex engine put
                                                                                          // it. Re-running the father's own flex layout repositions ALL of its children (not just this one) with
                                                                                          // their current (possibly just-changed) sizes, so a still-static sibling is not left stale either.
                                                                                          if(father && father->IsFlexContainer())
                                                                                            {
                                                                                              UI_LAYOUTENGINE::RunLayout(father, UI_LAYOUTSTRATEGY_CSS);
                                                                                            }

                                                                                          if(father) Elements_SetToRedraw(father);

                                                                                          Elements_SetToRedraw(element);
                                                                                        }
                                                                                    }
                                                                                    break;

                                                  case UI_SKIN_DRAWMODE_CONTEXT   : break;
                                                }

                                            }
                                        }
                                        break;

      case UI_ELEMENT_TYPE_TEXTBOX    : { UI_ELEMENT_TEXTBOX* element_textbox = (UI_ELEMENT_TEXTBOX*)element;
                                          XSTRING             resolve;

                                          if(element_textbox->GetMaskText()->IsEmpty()) return false;

                                          change = ChangeTextElementValue(element_textbox, element_textbox->GetMaskText(), resolve);                                      
                                          if(change) 
                                            {                                             
                                              if(element_textbox->GetText()->Compare(resolve.Get(), true)) 
                                                {
                                                  element_textbox->GetText()->Set(resolve);
                                                  Elements_SetToRedraw(element);    
                                                }
                                            }
                                           else
                                            {
                                              if(element_textbox->GetText()->IsEmpty())
                                                {
                                                  if(!resolve.IsEmpty())  
                                                    {
                                                      element_textbox->GetText()->Set(resolve);
                                                    }
                                                }
                                            }


                                        }  
                                        break;

                        default       : break;
    }


  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement) 
        {
          ChangeTextElementValue(layout, subelement);
        }
    }
  
  return true;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::ChangeTextElementValue(UI_LAYOUT* layout)
* @brief      Change text element value
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
 bool UI_MANAGER::ChangeTextElementValue(UI_LAYOUT* layout)
{
  if(!layout) 
    {
      return false;
    }
 
  for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
    {
      UI_ELEMENT* element = layout->Elements_Get()->Get(c);
      if(element) 
        {
          ChangeTextElementValue(layout, element);       
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SubscribeInputEvents(bool active)
* @brief      Subscribe input events
* @ingroup    USERINTERFACE
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SubscribeInputEvents(bool active)
{
  UI_XEVENT_TYPE eventtype[] = {  UI_XEVENT_TYPE_INPUT_CURSOR_MOVE            ,
                                  UI_XEVENT_TYPE_INPUT_CURSOR_MOTION          ,
                                  UI_XEVENT_TYPE_INPUT_SELECCTION             ,                                           
                               };

  for(int c=0; c<(sizeof(eventtype) / sizeof(UI_XEVENT_TYPE)); c++)
    {
      if(active)        
            SubscribeEvent(eventtype[c]   , this);              
      else  UnSubscribeEvent(eventtype[c] , this);        
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SubscribeOutputEvents(bool active, XOBSERVER* observer, XSUBJECT* subject)
* @brief      Subscribe output events
* @ingroup    USERINTERFACE
* 
* @param[in]  active : Active value.
* @param[in]  observer : Observer pointer to use.
* @param[in]  subject : Subject pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SubscribeOutputEvents(bool active, XOBSERVER* observer, XSUBJECT* subject)
{
  UI_XEVENT_TYPE eventtype[] = {  UI_XEVENT_TYPE_OUTPUT_SELECTED          ,                              
                                  UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE      ,
                                  UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS                                     
                               };

  for(int c=0; c<(sizeof(eventtype) / sizeof(UI_XEVENT_TYPE)); c++)
    {
      if(active)        
            observer->SubscribeEvent(eventtype[c]   , subject);              
      else  observer->UnSubscribeEvent(eventtype[c] , subject);        
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::CreaterVirtualKeyboard(UI_LAYOUT* layout, GRPSCREEN* screen)
* @brief      Creater virtual keyboard
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* @param[in]  screen : Screen object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::CreaterVirtualKeyboard(UI_LAYOUT* layout, GRPSCREEN* screen)
{
  if(virtualkeyboard) 
    {
      return false;   
    }

  if(!layout)         
    {
      return false;
    }
  
  if(!screen)         
    {
      return false;
    }

  virtualkeyboard = GEN_NEW UI_VIRTUALKEYBOARD();
  if(!virtualkeyboard) 
    {
      return false;
    }

  virtualkeyboard->Ini(layout, screen);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::DeleteVirtualKeyboard()
* @brief      Delete virtual keyboard
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::DeleteVirtualKeyboard()
{
  if(!virtualkeyboard)
    {
      return false;
    }

  bool status = false;

  status = virtualkeyboard->End();

  GEN_DELETE virtualkeyboard;
  virtualkeyboard = NULL;

  return status;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_MANAGER::UI_MANAGER()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_MANAGER::UI_MANAGER()    
{ 
  Clean();   

  xmutex_modal=GEN_XFACTORY.Create_Mutex();
  xmutex_UIevent=GEN_XFACTORY.Create_Mutex();

  RegisterEvents(true);                        
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_MANAGER::~UI_MANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_MANAGER::~UI_MANAGER()    
{ 
  RegisterEvents(false); 

  DeleteVirtualKeyboard();

  ModalLayer_Release();

  Layouts_DeleteAll(); 
  
  if(unzipfile)
    {
      GEN_DELETE unzipfile;
      unzipfile = NULL;
    }  

  if(xmutex_modal)
    {
      GEN_XFACTORY.Delete_Mutex(xmutex_modal);
      xmutex_modal = NULL;
    }

  if(xmutex_UIevent)
    {
      GEN_XFACTORY.Delete_Mutex(xmutex_UIevent);
      xmutex_UIevent = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN* UI_MANAGER::Skin_Create(XSTRING& skintypename, UI_SKIN_DRAWMODE drawmode, GRPSCREEN* screen, int viewportindex)
* @brief      Skin create
* @ingroup    USERINTERFACE
* 
* @param[in]  skintypename : Skintypename value.
* @param[in]  drawmode : Drawmode value.
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* 
* @return     UI_SKIN* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN* UI_MANAGER::Skin_Create(XSTRING& skintypename, UI_SKIN_DRAWMODE drawmode, GRPSCREEN* screen, int viewportindex)
{  
  if(!screen) 
    {
      return NULL;
    }

  UI_SKIN*  ui_skin;

  switch(drawmode)
    {
      case UI_SKIN_DRAWMODE_UNKNOWN     : return NULL;
                                         
      case UI_SKIN_DRAWMODE_CANVAS      : { UI_SKINCANVAS* skincanvas = NULL;

                                            if(!skintypename.Compare(UI_SKIN_NAME_DEFAULT, true) || !skintypename.Compare(UI_SKIN_NAME_UNKNOWN, true))
                                              {
                                                ui_skin = (UI_SKIN*)GEN_NEW UI_SKINCANVAS(screen, viewportindex); 
                                              }
                                             else
                                              {
                                                if(!skintypename.Compare(UI_SKIN_NAME_FLAT, true))
                                                  {
                                                    ui_skin = (UI_SKIN*)GEN_NEW UI_SKINCANVAS_FLAT(screen, viewportindex); 
                                                  }
                                              }
                                          }
                                          break;

      case UI_SKIN_DRAWMODE_CONTEXT     : break;

    }
  
  return ui_skin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetLayoutElementValue(XFILEXMLELEMENT* node, XCHAR* leyend, double& value)
* @brief      Get layout element value
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  leyend : Leyend pointer to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElementValue(XFILEXMLELEMENT* node, XCHAR* leyend, double& value)
{
  XCHAR*    xcharstr;
  XSTRING   valuestr;

  valuestr.Empty();

  xcharstr = node->GetValueAttribute(leyend);
  if(!xcharstr) return false;

  valuestr = xcharstr;

  value = valuestr.ConvertToDouble();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetLayoutElementValue(XFILEXMLELEMENT* node, XCHAR* leyend, XSTRING& value)
* @brief      Get layout element value
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  leyend : Leyend pointer to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElementValue(XFILEXMLELEMENT* node, XCHAR* leyend, XSTRING& value)
{
  XCHAR* xcharstr;

  value.Empty();

  xcharstr = node->GetValueAttribute(leyend);
  if(!xcharstr) return false;

  value = xcharstr;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetLayoutElement_CalculateBoundaryLine(UI_LAYOUT* layout, UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Get layout element calculate boundary line
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElement_CalculateBoundaryLine(UI_LAYOUT* layout, UI_ELEMENT* element, bool adjustsizemargin)
{
  bool status = false;

 if(!element) 
    {
      return false;
    }

  if(!layout)
    {
      return false;
    }

  if(!layout->GetSkin())
    {
      return false;
    }

  switch(layout->GetSkin()->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin(); 
                                          if(skincanvas) 
                                            {
                                              status = skincanvas->CalculateBoundaryLine(element, adjustsizemargin);                                                                                    
                                            }
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT   : break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetParentSizeFont(XFILEXMLELEMENT* node, double& sizefont)
* @brief      Get parent size font
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetParentSizeFont(XFILEXMLELEMENT* node, double& sizefont)
{
  if(!node) return false;

  sizefont = 0;

  if(!GetLayoutElementValue(node, __L("sizefont"), sizefont))
    {
      return GetParentSizeFont(node->GetFather(), sizefont);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ResolvePercentValue(XSTRING& valuestr, double basis, double& out)
* @brief      Step 4 (relative units): resolve a "N%" token against `basis` (the father element's own width or
*             height, whichever axis `valuestr` was authored for). Returns false -- leaving `out` untouched --
*             for anything that is not a trailing '%': callers keep using their existing raw-pixel path in that
*             case, so this never changes behaviour for values that were never a percentage to begin with.
* @note       Only "%" is added here; "em" is deliberately left out. Unlike a container's width/height (already
*             resolved on `element->GetFather()` by the time GetLayoutElement_Base() runs its children), an
*             element's own font size is not known at this point in the pipeline -- "sizefont" is read later,
*             per widget type, by builders such as GetLayoutElement_Text() -- so "em" cannot be resolved here
*             without reordering that pipeline. Flagged instead of worked around; see GEN_FrameWork chat log.
* @ingroup    USERINTERFACE
*
* @param[in]  valuestr : Raw attribute/declaration value, e.g. "50%" or "120".
* @param[in]  basis : The dimension (in pixels) that 100% maps to. Callers pass 0.0 when there is no father to
*             measure (a top-level layout element): the percentage then resolves to 0, same as any other
*             not-yet-supported case, rather than reading garbage.
* @param[out] out : Resolved pixel value. Untouched when `valuestr` is not a percentage.
*
* @return     bool : true if `valuestr` was a percentage and `out` was set; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ResolvePercentValue(XSTRING& valuestr, double basis, double& out)
{
  if(valuestr.IsEmpty()) return false;

  // Phase 4: "Nem" resolves against `basis` as the font-size (em). Callers that want em must pass sizefont.
  if(valuestr.Find(__L("em"), true) != XSTRING_NOTFOUND && valuestr.Find(__L("%"), true) == XSTRING_NOTFOUND)
    {
      XSTRING number = valuestr;
      number.DeleteCharacter(__C('e'));
      number.DeleteCharacter(__C('E'));
      number.DeleteCharacter(__C('m'));
      number.DeleteCharacter(__C('M'));
      number.DeleteNoCharacters(__L(" \t\r\n"));
      out = basis * number.ConvertToDouble();
      return true;
    }

  if(valuestr.Find(__L("%"), true) == XSTRING_NOTFOUND) return false;

  XSTRING number = valuestr;
  number.DeleteNoCharacters(__L("% \t\r\n"));   // strip the '%' plus any stray whitespace, e.g. "50 %"

  out = basis * (number.ConvertToDouble() / 100.0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::BuildLengthContext(UI_LAYOUT* layout, double basis, double fontsize, UI_LENGTH_CONTEXT& out)
* @brief      Fill a UI_LENGTH_CONTEXT from the layout's design viewport and optional --root-font-size theme var.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::BuildLengthContext(UI_LAYOUT* layout, double basis, double fontsize, UI_LENGTH_CONTEXT& out)
{
  out.basis         = basis;
  out.fontsize      = fontsize;
  out.rootfontsize  = 16.0;
  out.viewportwidth = layout ? (double)layout->GetDesignWidth()  : 0.0;
  out.viewportheight= layout ? (double)layout->GetDesignHeight() : 0.0;

  if(layout && layout->GetStyleSheet())
    {
      XSTRING rootfs;
      if(layout->GetStyleSheet()->Variables_Get(__L("--root-font-size"), rootfs) && !rootfs.IsEmpty())
        {
          double v = rootfs.ConvertToDouble();
          if(v > 0.0) out.rootfontsize = v;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ResolveStyleLength(XSTRING& valuestr, UI_LENGTH_CONTEXT& context, double& out)
* @brief      Resolve a style length token (px / % / em / rem / vw / vh / calc) for stylesheet layouts.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ResolveStyleLength(XSTRING& valuestr, UI_LENGTH_CONTEXT& context, double& out)
{
  return UI_PROPERTYREGISTRY::ResolveLengthToken(valuestr, context, out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::ApplyStyleLengthsFromBag(UI_ELEMENT* element, UI_LAYOUT* layout, UI_STYLE& style)
* @brief      Track L.4: re-apply length tokens (box + gap/flex-basis/margin/padding) from a computed style bag.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::ApplyStyleLengthsFromBag(UI_ELEMENT* element, UI_LAYOUT* layout, UI_STYLE& style)
{
  if(!element || !layout || !layout->GetStyleSheet()) return false;

  double fatherwidth  = 0.0;
  double fatherheight = 0.0;
  double fatherem     = 16.0;

  if(element->GetFather() && element->GetFather()->GetBoundaryLine())
    {
      fatherwidth  = element->GetFather()->GetBoundaryLine()->width;
      fatherheight = element->GetFather()->GetBoundaryLine()->height;
      if(element->GetFather()->GetComputedStyle())
        {
          double sf = 0.0;
          if(element->GetFather()->GetComputedStyle()->Get(__L("sizefont"), sf) && sf > 0.0) fatherem = sf;
        }
    }

  double xpos   = element->GetBoundaryLine() ? element->GetBoundaryLine()->x      : 0.0;
  double ypos   = element->GetBoundaryLine() ? element->GetBoundaryLine()->y      : 0.0;
  double width  = element->GetBoundaryLine() ? element->GetBoundaryLine()->width  : 0.0;
  double height = element->GetBoundaryLine() ? element->GetBoundaryLine()->height : 0.0;

  // Flex/grid items get x/y from RunLayout. Re-applying CSS xpos/ypos (especially %) against the post-flex
  // father box corrupts scale labels and progress widgets — only refresh size/gap/padding/margin for them.
  bool father_is_flow = false;
  if(element->GetFather())
    {
      father_is_flow = element->GetFather()->IsFlexContainer() || element->GetFather()->IsGridContainer();
    }

  XSTRING position;
  if(!father_is_flow && style.Get(__L("xpos"), position))
    {
      if(!position.Compare(__L("left"), true))        xpos = UI_ELEMENT_TYPE_ALIGN_LEFT;
      else if(!position.Compare(__L("right"), true))  xpos = UI_ELEMENT_TYPE_ALIGN_RIGHT;
      else if(!position.Compare(__L("center"), true)) xpos = UI_ELEMENT_TYPE_ALIGN_CENTER;
      else
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherwidth, fatherem, ctx);
          ResolveStyleLength(position, ctx, xpos);
        }
    }

  if(!father_is_flow && style.Get(__L("ypos"), position))
    {
      if(!position.Compare(__L("up"), true))          ypos = UI_ELEMENT_TYPE_ALIGN_UP;
      else if(!position.Compare(__L("down"), true))   ypos = UI_ELEMENT_TYPE_ALIGN_DOWN;
      else if(!position.Compare(__L("center"), true)) ypos = UI_ELEMENT_TYPE_ALIGN_CENTER;
      else
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherheight, fatherem, ctx);
          ResolveStyleLength(position, ctx, ypos);
        }
    }

  XSTRING size;
  if(style.Get(__L("width"), size))
    {
      if(!size.Compare(__L("max"), true))        width = UI_ELEMENT_TYPE_ALIGN_MAX;
      else if(!size.Compare(__L("auto"), true))  width = UI_ELEMENT_TYPE_ALIGN_AUTO;
      else
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherwidth, fatherem, ctx);
          ResolveStyleLength(size, ctx, width);
        }
    }

  if(style.Get(__L("height"), size))
    {
      if(!size.Compare(__L("max"), true))        height = UI_ELEMENT_TYPE_ALIGN_MAX;
      else if(!size.Compare(__L("auto"), true))  height = UI_ELEMENT_TYPE_ALIGN_AUTO;
      else
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherheight, fatherem, ctx);
          ResolveStyleLength(size, ctx, height);
        }
    }

  if(element->GetBoundaryLine())
    {
      element->GetBoundaryLine()->x      = xpos;
      element->GetBoundaryLine()->y      = ypos;
      element->GetBoundaryLine()->width  = width;
      element->GetBoundaryLine()->height = height;
    }

  // Prefer intrinsic snapshot for flex idempotency (same as load-time post-union fix).
  // Alignment tokens (LEFT/MAX/AUTO/…) are large negatives — only snapshot real positive sizes.
  if(width  > 0.0) element->SetIntrinsicWidth(width);
  if(height > 0.0) element->SetIntrinsicHeight(height);

  {
    UI_LENGTH_CONTEXT ctx;
    BuildLengthContext(layout, fatherwidth, fatherem, ctx);
    XSTRING gapstr;
    double  gapvalue = 0.0;
    double  rowgap   = element->GetRowGap();
    double  colgap   = element->GetColumnGap();
    if(style.Get(__L("gap")       , gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) { rowgap = gapvalue; colgap = gapvalue; }
    if(style.Get(__L("row-gap")   , gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) rowgap = gapvalue;
    if(style.Get(__L("column-gap"), gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) colgap = gapvalue;
    element->SetGap(rowgap, colgap);
  }

  XSTRING flexbasisstr;
  if(style.Get(__L("flex-basis"), flexbasisstr))
    {
      if(!flexbasisstr.Compare(__L("auto"), true)) element->SetFlexBasisAuto();
      else
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherwidth, fatherem, ctx);
          double bv = 0.0;
          if(ResolveStyleLength(flexbasisstr, ctx, bv)) element->SetFlexBasis(bv);
          else element->SetFlexBasis(flexbasisstr.ConvertToDouble());
        }
    }

  {
    double edges[4] = { 0.0, 0.0, 0.0, 0.0 };
    UI_LENGTH_CONTEXT lengthctx;
    BuildLengthContext(layout, fatherwidth, fatherem, lengthctx);
    if(UI_PROPERTYREGISTRY::ResolveMarginEdges(style, true, true, edges, &lengthctx))
      {
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_LEFT , edges[0]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_RIGHT, edges[1]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_UP   , edges[2]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN , edges[3]);
      }
  }

  XSTRING paddingstr;
  if(style.Get(__L("padding"), paddingstr))
    {
      double out[4] = { 0.0, 0.0, 0.0, 0.0 };
      UI_LENGTH_CONTEXT ctx;
      BuildLengthContext(layout, fatherwidth, fatherem, ctx);
      if(!UI_PROPERTYREGISTRY::ExpandCSSShorthand4Lengths(paddingstr, ctx, out))
        {
          UI_PROPERTYREGISTRY::ExpandCSSShorthand4(paddingstr, out);
        }
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , out[3]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, out[1]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , out[0]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , out[2]);
    }

  {
    UI_LENGTH_CONTEXT ctx;
    BuildLengthContext(layout, fatherwidth, fatherem, ctx);
    XSTRING pstr;
    double  pv = 0.0;
    if(style.Get(__L("padding-left")  , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , pv);
    if(style.Get(__L("padding-right") , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, pv);
    if(style.Get(__L("padding-top")   , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , pv);
    if(style.Get(__L("padding-bottom"), pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , pv);
  }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::ReresolveElementStyleLengthsRecursive(UI_ELEMENT* element, UI_LAYOUT* layout)
* @brief      Track L.4: depth-first re-resolve (parents before children so % basis is current).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::ReresolveElementStyleLengthsRecursive(UI_ELEMENT* element, UI_LAYOUT* layout)
{
  if(!element || !layout) return;

  UI_STYLE* bag = element->GetComputedStyle();
  if(bag) ApplyStyleLengthsFromBag(element, layout, *bag);

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(children)
    {
      for(XDWORD c=0; c<children->GetSize(); c++)
        {
          ReresolveElementStyleLengthsRecursive(children->Get(c), layout);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Layouts_ReresolveStyleLengths(UI_LAYOUT* layout)
* @brief      Track L.4: re-resolve style lengths for every element, then RunLayout on each top-level root.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_ReresolveStyleLengths(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetStyleSheet()) return false;

  // Keep @media viewport aligned with design (load-time media still; lengths follow design live).
  layout->GetStyleSheet()->SetMediaViewport((int)layout->GetDesignWidth(), (int)layout->GetDesignHeight());

  XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
  if(!roots) return false;

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      UI_ELEMENT* root = roots->Get(c);
      if(!root) continue;
      ReresolveElementStyleLengthsRecursive(root, layout);
      UI_LAYOUTENGINE::RunLayout(root, UI_LAYOUTSTRATEGY_CSS);
      // Same post-flex ProgressBar track fix CreateLayouts runs after the first RunLayout.
      RefreshFlexProgressBarTracks(root, layout->GetSkin());
    }

  layout->Elements_SetToRedraw(true);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Layouts_SetDesignSize(UI_LAYOUT* layout, XDWORD width, XDWORD height)
* @brief      Track L.4: change design canvas and refresh length-dependent layout.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_SetDesignSize(UI_LAYOUT* layout, XDWORD width, XDWORD height)
{
  if(!layout || !width || !height) return false;

  layout->SetDesignSize(width, height);
  if(layout->GetStyleSheet())
    {
      layout->GetStyleSheet()->SetMediaViewport((int)width, (int)height);
      Layouts_ReresolveStyleLengths(layout);
    }

  if(layout->IsUIScaleActive()) UIScale_PrepareLayout(layout);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::GetLayoutElement_Base(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent)
* @brief      Get layout element base
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* @param[in]  adjusttoparent : Adjusttoparent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElement_Base(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent)
{
  UI_COMPUTEDSTYLE style;

  return GetLayoutElement_Base(node, layout, element, style, adjusttoparent);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::GetLayoutElement_Base(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, UI_COMPUTEDSTYLE& outstyle, bool adjusttoparent)
* @brief      Get layout element base
* @ingroup    USERINTERFACE
*
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* @param[out] outstyle : Receives the fully-resolved bag (XML attributes < CSS rules < inline style) this call
*                         built, so the caller's own per-widget reads can go through the same cascade.
* @param[in]  adjusttoparent : Adjusttoparent value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElement_Base(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, UI_COMPUTEDSTYLE& outstyle, bool adjusttoparent)
{
  outstyle.FillFromXMLElement(node);

  // Set the element's identity BEFORE the CSS cascade runs, so #id and .class selectors can match. The XML
  // attributes we just harvested carry the authoritative identity; CSS may still overwrite visual keys later,
  // but never the identity itself.
  XSTRING xml_name;   outstyle.Get(__L("name"),  xml_name);
  XSTRING xml_class;  outstyle.Get(__L("class"), xml_class);

  if(element)
    {
      if(!xml_name.IsEmpty())  element->GetName()->Set(xml_name);
      element->SetClassNames(xml_class);         // empty string clears the list; retro-compat safe
    }

  // Layer stylesheet declarations on top (CSS-wins semantics). The stylesheet is owned by THIS element's own
  // UI_LAYOUT (Phase 1 ownership step -- see UI_LAYOUT::GetStyleSheet(), set from CreateLayouts()), not by
  // UI_MANAGER, so two layouts loaded from different XML files can never silently share (or clobber) one
  // another's rules. No layout / no stylesheet loaded -> no-op, exactly as before.
  if(layout && element)
    {
      UI_STYLESHEET* sheet = layout->GetStyleSheet();
      if(sheet) outstyle.FillFromCSSDeclarations(sheet, element);
    }

  // Step 6 ("sin overrides puntuales por elemento"): a "style=" XML attribute is the highest-priority layer,
  // exactly like HTML's own inline style -- it wins over both the plain XML attributes and any stylesheet rule
  // (class, id, or :root) applied above, without needing a one-off class in the .css for a single element's
  // tweak. FillFromXMLElement() already copied "style" into the bag like any other attribute; read it back out
  // and, if present, layer its declarations on top. Absent/empty "style" (the overwhelming majority of
  // elements, and every layout authored before this existed) costs one bag lookup and nothing else.
  XSTRING inlinestyle;
  if(outstyle.Get(__L("style"), inlinestyle) && !inlinestyle.IsEmpty())
    {
      UI_STYLESHEET* sheet = layout ? layout->GetStyleSheet() : NULL;
      outstyle.FillFromInlineStyle(inlinestyle, sheet);
    }

  XSTRING fathertagname;
  if(node && node->GetFather()) fathertagname = node->GetFather()->GetName();

  return GetLayoutElement_Base(outstyle, fathertagname, layout, element, adjusttoparent);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::GetLayoutElement_Base(UI_STYLE& style, XSTRING& fathertagname, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent)
* @brief      Get layout element base from a neutral style bag (source-format independent).
* @ingroup    USERINTERFACE
* 
* @param[in]  style : Style value.
* @param[in]  fathertagname : Fathertagname value.
* @param[in]  layout : Layout pointer to use.
* @param[in]  element : Element to process.
* @param[in]  adjusttoparent : Adjusttoparent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::GetLayoutElement_Base(UI_STYLE& style, XSTRING& fathertagname, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent)
{
  double xpos   = 0.0f;
  double ypos   = 0.0f;
  double width  = 0.0f;
  double height = 0.0f;

  if(!element) return false;

  // Phase 1 ownership step: tag the element with the UI_LAYOUT it belongs to. EVERY widget builder (top-level
  // AND nested/child elements, via CreatePartialLayout()) routes through this one function, so this is the
  // single point that guarantees UI_ELEMENT::ReapplyStyleVisual() can always resolve back to the correct
  // per-layout stylesheet later, regardless of nesting depth -- see UI_ELEMENT::GetLayout()/SetLayout().
  element->SetLayout(layout);

  element->SetIsDetached(false);

  if(!fathertagname.Compare(__L("layout"), true)) element->SetIsDetached(true);

  XSTRING name;
  style.Get(__L("name"), name);
  element->GetName()->Set(name);

  // Step 4 (relative units): "%" on xpos/ypos/width/height resolves against the father's own already-resolved
  // box -- safe because GEN builds a layout top-down (a father's UI_ELEMENT is always fully resolved before
  // GetLayoutElement_Base() ever runs for one of its children). A top-level element (no father) has no defined
  // basis, so its percentage resolves to 0 exactly like today's "missing attribute" case, rather than guessing
  // against the canvas -- see ResolvePercentValue()'s note on why "em" is not attempted here at all.
  double fatherwidth  = element->GetFather() ? element->GetFather()->GetBoundaryLine()->width  : 0.0;
  double fatherheight = element->GetFather() ? element->GetFather()->GetBoundaryLine()->height : 0.0;

  // Phase 4 em basis: father's sizefont when father is text; else try computed sizefont on father; else 16.
  double fatherem = 16.0;
  if(element->GetFather())
    {
      UI_ELEMENT_TEXT* ftext = dynamic_cast<UI_ELEMENT_TEXT*>(element->GetFather());
      if(ftext && ftext->GetSizeFont() > 0) fatherem = (double)ftext->GetSizeFont();
      else if(element->GetFather()->GetComputedStyle())
        {
          double sf = 0.0;
          if(element->GetFather()->GetComputedStyle()->Get(__L("sizefont"), sf) && sf > 0.0) fatherem = sf;
        }
    }

  // Fase 8: rem/vw/vh/%/em via UI_LENGTH only when this layout owns a stylesheet (UI_Options stays on
  // ResolvePercentValue / ConvertToDouble).
  const bool use_style_lengths = (layout && layout->GetStyleSheet());

  XSTRING position;
  if(style.Get(__L("xpos"), position))
    {
      if(!position.Compare(__L("left"), true))  xpos = UI_ELEMENT_TYPE_ALIGN_LEFT;
        else if(!position.Compare(__L("right"), true))  xpos = UI_ELEMENT_TYPE_ALIGN_RIGHT;
          else if(!position.Compare(__L("center"), true)) xpos = UI_ELEMENT_TYPE_ALIGN_CENTER;
            else if(use_style_lengths)
              {
                UI_LENGTH_CONTEXT ctx;
                BuildLengthContext(layout, fatherwidth, fatherem, ctx);
                if(!ResolveStyleLength(position, ctx, xpos)) style.Get(__L("xpos"), xpos);
              }
            else
              {
                double basis = (position.Find(__L("em"), true) != XSTRING_NOTFOUND) ? fatherem : fatherwidth;
                if(!ResolvePercentValue(position, basis, xpos)) style.Get(__L("xpos"), xpos);
              }
    }

  if(style.Get(__L("ypos"), position))
    {
      if(!position.Compare(__L("up"), true))  ypos = UI_ELEMENT_TYPE_ALIGN_UP;
        else if(!position.Compare(__L("down"), true))  ypos = UI_ELEMENT_TYPE_ALIGN_DOWN;
          else if(!position.Compare(__L("center"), true)) ypos = UI_ELEMENT_TYPE_ALIGN_CENTER;
            else if(use_style_lengths)
              {
                UI_LENGTH_CONTEXT ctx;
                BuildLengthContext(layout, fatherheight, fatherem, ctx);
                if(!ResolveStyleLength(position, ctx, ypos)) style.Get(__L("ypos"), ypos);
              }
            else
              {
                double basis = (position.Find(__L("em"), true) != XSTRING_NOTFOUND) ? fatherem : fatherheight;
                if(!ResolvePercentValue(position, basis, ypos)) style.Get(__L("ypos"), ypos);
              }
    }


  XSTRING size;
  if(style.Get(__L("width"), size))
    {
      if(!size.Compare(__L("max"), true))  width = UI_ELEMENT_TYPE_ALIGN_MAX;
        else if(!size.Compare(__L("auto"), true))  width = UI_ELEMENT_TYPE_ALIGN_AUTO;
          else if(use_style_lengths)
            {
              UI_LENGTH_CONTEXT ctx;
              BuildLengthContext(layout, fatherwidth, fatherem, ctx);
              if(!ResolveStyleLength(size, ctx, width)) style.Get(__L("width"), width);
            }
          else
            {
              double basis = (size.Find(__L("em"), true) != XSTRING_NOTFOUND) ? fatherem : fatherwidth;
              if(!ResolvePercentValue(size, basis, width)) style.Get(__L("width"), width);
            }
    }
   else
    {
      if(element->GetFather() && adjusttoparent)
        {
          width = fatherwidth;
        }
    }

  if(style.Get(__L("height"), size))
    {
      if(!size.Compare(__L("max"), true))  height = UI_ELEMENT_TYPE_ALIGN_MAX;
        else if(!size.Compare(__L("auto"), true))  height = UI_ELEMENT_TYPE_ALIGN_AUTO;
          else if(use_style_lengths)
            {
              UI_LENGTH_CONTEXT ctx;
              BuildLengthContext(layout, fatherheight, fatherem, ctx);
              if(!ResolveStyleLength(size, ctx, height)) style.Get(__L("height"), height);
            }
          else
            {
              double basis = (size.Find(__L("em"), true) != XSTRING_NOTFOUND) ? fatherem : fatherheight;
              if(!ResolvePercentValue(size, basis, height)) style.Get(__L("height"), height);
            }
    }
   else
    {
      if(element->GetFather() && adjusttoparent)
        {
          height = fatherheight;
        }
    }

  element->GetBoundaryLine()->x       = xpos;
  element->GetBoundaryLine()->y       = ypos;
  element->GetBoundaryLine()->width   = width;
  element->GetBoundaryLine()->height  = height;

  XSTRING directionstr;
  if(style.Get(__L("direction"), directionstr))
    {
      if(!directionstr.Compare(__L("horizontal"), true))  element->SetDirection(UI_ELEMENT_TYPE_DIRECTION_HORIZONTAL);
        else if(!directionstr.Compare(__L("vertical"), true))  element->SetDirection(UI_ELEMENT_TYPE_DIRECTION_VERTICAL);
    }

  // Flexbox / Grid: CSS Lite wiring. "display: flex" | "display: grid". Absence leaves defaults (false).
  XSTRING displaystr;
  if(style.Get(__L("display"), displaystr))
    {
      bool isflex = !displaystr.Compare(__L("flex"), true);
      bool isgrid = !displaystr.Compare(__L("grid"), true);
      element->SetFlexContainer(isflex);
      element->SetGridContainer(isgrid);
    }

  XSTRING flexdirectionstr;
  if(style.Get(__L("flex-direction"), flexdirectionstr))
    {
      if(!flexdirectionstr.Compare(__L("row"), true))  element->SetFlexDirection(UI_FLEX_DIRECTION_ROW);
        else if(!flexdirectionstr.Compare(__L("row-reverse"), true))  element->SetFlexDirection(UI_FLEX_DIRECTION_ROW_REVERSE);
          else if(!flexdirectionstr.Compare(__L("column"), true))  element->SetFlexDirection(UI_FLEX_DIRECTION_COLUMN);
            else if(!flexdirectionstr.Compare(__L("column-reverse"), true))  element->SetFlexDirection(UI_FLEX_DIRECTION_COLUMN_REVERSE);
    }

  XSTRING justifycontentstr;
  if(style.Get(__L("justify-content"), justifycontentstr))
    {
      if(!justifycontentstr.Compare(__L("flex-start"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_FLEX_START);
        else if(!justifycontentstr.Compare(__L("flex-end"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_FLEX_END);
          else if(!justifycontentstr.Compare(__L("center"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_CENTER);
            else if(!justifycontentstr.Compare(__L("space-between"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_SPACE_BETWEEN);
              else if(!justifycontentstr.Compare(__L("space-around"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_SPACE_AROUND);
                else if(!justifycontentstr.Compare(__L("space-evenly"), true))  element->SetJustifyContent(UI_JUSTIFY_CONTENT_SPACE_EVENLY);
    }

  // "gap" is the shorthand for both axes; "row-gap"/"column-gap" (read afterwards, so they win if present,
  // exactly like the padding/margin longhands elsewhere in this function) override just their own axis.
  // Track L.2: with a stylesheet, rem/vw/vh/% resolve via UI_LENGTH (same gate as padding/margin).
  double rowgap    = element->GetRowGap();
  double columngap = element->GetColumnGap();
  if(use_style_lengths)
    {
      UI_LENGTH_CONTEXT ctx;
      BuildLengthContext(layout, fatherwidth, fatherem, ctx);
      XSTRING gapstr;
      double  gapvalue = 0.0;
      if(style.Get(__L("gap")       , gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) { rowgap = gapvalue; columngap = gapvalue; }
      if(style.Get(__L("row-gap")   , gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) rowgap    = gapvalue;
      if(style.Get(__L("column-gap"), gapstr) && ResolveStyleLength(gapstr, ctx, gapvalue)) columngap = gapvalue;
    }
   else
    {
      double gapvalue = 0.0;
      if(style.Get(__L("gap"), gapvalue)) { rowgap = gapvalue; columngap = gapvalue; }
      if(style.Get(__L("row-gap"), gapvalue))    rowgap    = gapvalue;
      if(style.Get(__L("column-gap"), gapvalue)) columngap = gapvalue;
    }
  element->SetGap(rowgap, columngap);

  XSTRING flexwrapstr;
  if(style.Get(__L("flex-wrap"), flexwrapstr))
    {
      if(!flexwrapstr.Compare(__L("nowrap"), true))  element->SetFlexWrap(UI_FLEX_WRAP_NOWRAP);
        else if(!flexwrapstr.Compare(__L("wrap"), true))  element->SetFlexWrap(UI_FLEX_WRAP_WRAP);
          else if(!flexwrapstr.Compare(__L("wrap-reverse"), true))  element->SetFlexWrap(UI_FLEX_WRAP_WRAP_REVERSE);
    }

  XSTRING aligncontentstr;
  if(style.Get(__L("align-content"), aligncontentstr))
    {
      if(!aligncontentstr.Compare(__L("flex-start"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_FLEX_START);
        else if(!aligncontentstr.Compare(__L("flex-end"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_FLEX_END);
          else if(!aligncontentstr.Compare(__L("center"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_CENTER);
            else if(!aligncontentstr.Compare(__L("space-between"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_SPACE_BETWEEN);
              else if(!aligncontentstr.Compare(__L("space-around"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_SPACE_AROUND);
                else if(!aligncontentstr.Compare(__L("space-evenly"), true))  element->SetAlignContent(UI_ALIGN_CONTENT_SPACE_EVENLY);
    }

  XSTRING alignitemsstr;
  if(style.Get(__L("align-items"), alignitemsstr))
    {
      if(!alignitemsstr.Compare(__L("flex-start"), true))  element->SetAlignItems(UI_ALIGN_ITEMS_FLEX_START);
        else if(!alignitemsstr.Compare(__L("flex-end"), true))  element->SetAlignItems(UI_ALIGN_ITEMS_FLEX_END);
          else if(!alignitemsstr.Compare(__L("center"), true))  element->SetAlignItems(UI_ALIGN_ITEMS_CENTER);
            else if(!alignitemsstr.Compare(__L("stretch"), true))  element->SetAlignItems(UI_ALIGN_ITEMS_STRETCH);
    }

  double flexgrowvalue = 0.0;
  if(style.Get(__L("flex-grow"), flexgrowvalue)) element->SetFlexGrow(flexgrowvalue);

  double flexshrinkvalue = 0.0;
  if(style.Get(__L("flex-shrink"), flexshrinkvalue)) element->SetFlexShrink(flexshrinkvalue);

  XSTRING flexbasisstr;
  if(style.Get(__L("flex-basis"), flexbasisstr))
    {
      if(!flexbasisstr.Compare(__L("auto"), true))  element->SetFlexBasisAuto();
        else if(use_style_lengths)
          {
            UI_LENGTH_CONTEXT ctx;
            BuildLengthContext(layout, fatherwidth, fatherem, ctx);
            double bv = 0.0;
            if(ResolveStyleLength(flexbasisstr, ctx, bv)) element->SetFlexBasis(bv);
              else element->SetFlexBasis(flexbasisstr.ConvertToDouble());
          }
        else element->SetFlexBasis(flexbasisstr.ConvertToDouble());
    }

  XSTRING alignselfstr;
  if(style.Get(__L("align-self"), alignselfstr))
    {
      if(!alignselfstr.Compare(__L("auto"), true))  element->SetAlignSelf(UI_ALIGN_SELF_AUTO);
        else if(!alignselfstr.Compare(__L("flex-start"), true))  element->SetAlignSelf(UI_ALIGN_SELF_FLEX_START);
          else if(!alignselfstr.Compare(__L("flex-end"), true))  element->SetAlignSelf(UI_ALIGN_SELF_FLEX_END);
            else if(!alignselfstr.Compare(__L("center"), true))  element->SetAlignSelf(UI_ALIGN_SELF_CENTER);
              else if(!alignselfstr.Compare(__L("stretch"), true))  element->SetAlignSelf(UI_ALIGN_SELF_STRETCH);
    }

  // "text-align" (Step 10): "textalignment" is the historical GEN attribute name (already used, XML-only, by
  // UI_ELEMENT_TEXTBOX's own builder); "text-align" is accepted as the CSS-natural alias, first-hit-wins same
  // as "bckgrdcolor"/"background-color" above -- both now the SAME shared lookup, UI_PROPERTYREGISTRY::
  // GetAliased(), so this precedence and ReapplyStyleVisual()'s cannot silently drift apart. Absent/unrecognized
  // leaves the element's constructor default (LEFT) untouched, so this is a no-op for every layout authored
  // before it existed.
  XSTRING textalignstr;
  UI_PROPERTYREGISTRY::GetAliased(style, __L("textalignment"), __L("text-align"), textalignstr);
  if(!textalignstr.IsEmpty())
    {
      if(!textalignstr.Compare(__L("left"), true))  element->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_LEFT);
        else if(!textalignstr.Compare(__L("right"), true))  element->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_RIGHT);
          else if(!textalignstr.Compare(__L("center"), true)) element->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_CENTER);
    }

  // NOTE: "role" is reserved for GEN custom Chromes (window caption) layouts: it lets GRPSCREEN find "the close
  // button", "the title", etc. without depending on element names. Any element type (image, button, text, a plain
  // rectangle...) can carry it; unrecognized/absent values leave chromerole at UI_ELEMENT_CHROMEROLE_NONE.
  XSTRING rolestr;
  if(style.Get(__L("role"), rolestr))
    {
      if(!rolestr.Compare(__L("caption")  , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_CAPTION);
        else if(!rolestr.Compare(__L("icon")     , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_ICON);
          else if(!rolestr.Compare(__L("title")    , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_TITLE);
            else if(!rolestr.Compare(__L("minimize") , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_MINIMIZE);
              else if(!rolestr.Compare(__L("maximize") , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_MAXIMIZE);
                else if(!rolestr.Compare(__L("close")    , true))  element->SetChromeRole(UI_ELEMENT_CHROMEROLE_CLOSE);
    }

  // Step 6: track authored-vs-default for "color" and "bckgrdcolor", and honour the CSS-natural aliases and
  // the "inherit" value on color.
  //
  //   "color"            : if the string is exactly "inherit" (case-insensitive), walk up the parent chain and
  //                        copy the first ancestor's color that itself was authored; otherwise parse the string
  //                        into element->color. Either way, mark color_set = true on success.
  //   "bckgrdcolor" or   : first-hit lookup order accepts the CSS-natural "background-color" as an alias, so
  //   "background-color"   authors can write either. Any authored value marks background_color_set = true.
  XSTRING color;
  style.Get(__L("color"), color);
  if(!color.IsEmpty())
    {
      if(!color.Compare(__L("inherit"), true))
        {
          UI_ELEMENT* ancestor = element->GetFather();
          while(ancestor)
            {
              if(ancestor->IsColorSet())
                {
                  ancestor->GetColor()->CopyTo(element->GetColor());
                  element->SetColorSet(true);
                  break;
                }
              ancestor = ancestor->GetFather();
            }
        }
       else
        {
          element->GetColor()->SetFromString(color);
          element->SetColorSet(true);
        }
    }

  XSTRING bckgrdcolor;
  UI_PROPERTYREGISTRY::GetAliased(style, __L("bckgrdcolor"), __L("background-color"), bckgrdcolor);
  if(!bckgrdcolor.IsEmpty())
    {
      element->GetBackgroundColor()->SetFromString(bckgrdcolor);
      element->SetBackgroundColorSet(true);
    }

  XSTRING visible;
  if(style.Get(__L("visible"), visible))
    {
      element->SetVisible(visible.ConvertToBoolean());
    }

  XSTRING hasscroll;
  if(style.Get(__L("scroll"), hasscroll))
    {
      element->SetHasScroll(hasscroll.ConvertToBoolean());
    }

  double roundrect = 0.0f;
  style.Get(__L("roundrect"), roundrect);
  element->SetRoundRect((XDWORD)roundrect);

  double blinktime;
  if(style.Get(__L("blink"), blinktime)) element->SetBlink((XDWORD)blinktime);

  // Step 7 ("transiciones"): a plain "transition: <milliseconds>" duration -- GEN's minimal CSS subset stays
  // minimal here too (no property list, no easing keyword). Read once at load time exactly like "blink" above;
  // UI_ELEMENT::ReapplyStyleVisual() consults it whenever a pseudo-class state change re-resolves color/
  // bckgrdcolor, tweening between the old and new value over that many milliseconds instead of jumping
  // instantly. 0 (the default -- every layout authored before this existed) preserves the original instant
  // jump exactly.
  double transitionms;
  if(style.Get(__L("transition"), transitionms)) element->SetTransitionDuration((XDWORD)transitionms);

  XSTRING extra;
  style.Get(__L("extra"), extra);
  element->GetExtra()->Set(extra);

  // "margin": resolved via UI_PROPERTYREGISTRY::ResolveMarginEdges so load-time and unit tests share one
  // implementation. 4-value shorthand: layouts WITH a stylesheet use CSS TRBL; WITHOUT keep LEFT,RIGHT,UP,DOWN.
  // Longhands (margin-top/...) apply only when a stylesheet is present -- ignored on XML-only layouts.
  // Fase 8: with a stylesheet, rem/vw/vh/% resolve through UI_LENGTH_CONTEXT.
  {
    bool   use_css = (layout && layout->GetStyleSheet());
    double edges[4] = { 0.0, 0.0, 0.0, 0.0 };   // LEFT, RIGHT, UP, DOWN

    UI_LENGTH_CONTEXT  lengthctx;
    UI_LENGTH_CONTEXT* lengthctx_ptr = NULL;
    if(use_css)
      {
        BuildLengthContext(layout, fatherwidth, fatherem, lengthctx);
        lengthctx_ptr = &lengthctx;
      }

    if(UI_PROPERTYREGISTRY::ResolveMarginEdges(style, use_css, use_css, edges, lengthctx_ptr))
      {
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_LEFT , edges[0]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_RIGHT, edges[1]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_UP   , edges[2]);
        element->SetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN , edges[3]);
      }
  }

  // --- Step 4: box-model additions --------------------------------------------------------------------------------
  // Padding shorthand accepts 1..4 numbers separated by comma or whitespace, using CSS ordering:
  //     1 value:  all four sides
  //     2 values: TOP-BOTTOM, LEFT-RIGHT
  //     3 values: TOP, LEFT-RIGHT, BOTTOM
  //     4 values: TOP, RIGHT, BOTTOM, LEFT
  // Longhand keys ("padding-left" / "-right" / "-top" / "-bottom") override the shorthand and are applied last.
  // Fase 8: with a stylesheet, each token may be rem/vw/vh/%/em/calc via ExpandCSSShorthand4Lengths.
  XSTRING paddingstr;
  if(style.Get(__L("padding"), paddingstr))
    {
      double out[4] = { 0.0, 0.0, 0.0, 0.0 };
      bool   ok     = false;

      if(use_style_lengths)
        {
          UI_LENGTH_CONTEXT ctx;
          BuildLengthContext(layout, fatherwidth, fatherem, ctx);
          ok = UI_PROPERTYREGISTRY::ExpandCSSShorthand4Lengths(paddingstr, ctx, out);
        }

      if(!ok) UI_PROPERTYREGISTRY::ExpandCSSShorthand4(paddingstr, out);   // out = TOP, RIGHT, BOTTOM, LEFT

      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , out[3]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, out[1]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , out[0]);
      element->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , out[2]);
    }

  if(use_style_lengths)
    {
      UI_LENGTH_CONTEXT ctx;
      BuildLengthContext(layout, fatherwidth, fatherem, ctx);
      XSTRING pstr;
      double  pv = 0.0;
      if(style.Get(__L("padding-left")  , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , pv);
      if(style.Get(__L("padding-right") , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, pv);
      if(style.Get(__L("padding-top")   , pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , pv);
      if(style.Get(__L("padding-bottom"), pstr) && ResolveStyleLength(pstr, ctx, pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , pv);
    }
   else
    {
      double pv;
      if(style.Get(__L("padding-left")  , pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , pv);
      if(style.Get(__L("padding-right") , pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, pv);
      if(style.Get(__L("padding-top")   , pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , pv);
      if(style.Get(__L("padding-bottom"), pv)) element->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , pv);
    }

  // border-width in pixels. 0 = "no stroke at all" (honoured explicitly); a missing key leaves the element's
  // -1 default so the skin keeps drawing its historical 1-px border for containers.
  double bw;
  if(style.Get(__L("border-width"), bw)) element->SetBorderWidth(bw);

  // Step 5: border-color at base level. Consumers (Draw_Form et al.) prefer this when set, otherwise fall back
  // to the per-type "linecolor" so pre-step-5 layouts render unchanged.
  XSTRING bcstr;
  if(style.Get(__L("border-color"), bcstr) && !bcstr.IsEmpty()) element->SetBorderColorFromString(bcstr);

  // Step 5: border-radius shorthand + per-corner longhands.
  // Shorthand accepts 1..4 numbers (comma or whitespace separated), CSS ordering: TL, TR, BR, BL.
  //     1 value:  all four corners
  //     2 values: TL-BR, TR-BL (diagonals)
  //     3 values: TL, TR-BL, BR
  //     4 values: TL, TR, BR, BL
  XSTRING brstr;
  if(style.Get(__L("border-radius"), brstr))
    {
      double out[4] = { 0.0, 0.0, 0.0, 0.0 };
      UI_PROPERTYREGISTRY::ExpandCSSShorthand4(brstr, out);              // out = TL, TR, BR, BL

      element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_TL, out[0]);
      element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_TR, out[1]);
      element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_BR, out[2]);
      element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_BL, out[3]);

      // Keep uniform roundrect in sync so Draw_Form / soft-shadow can take the AGG rounded_rect fast path
      // when all four corners match (avoids Path+miter corner spikes on cards).
      if((out[0] == out[1]) && (out[1] == out[2]) && (out[2] == out[3]) && (out[0] > 0.0))
        {
          element->SetRoundRect((XDWORD)out[0]);
        }
    }

  double rv;
  if(style.Get(__L("border-top-left-radius")     , rv)) element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_TL, rv);
  if(style.Get(__L("border-top-right-radius")    , rv)) element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_TR, rv);
  if(style.Get(__L("border-bottom-right-radius") , rv)) element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_BR, rv);
  if(style.Get(__L("border-bottom-left-radius")  , rv)) element->SetBorderRadius(UI_ELEMENT_BORDER_CORNER_BL, rv);

  // Step 7: box-shadow. Accepted grammar (CSS subset):
  //     box-shadow: <offset-x> <offset-y> <color>
  //     box-shadow: <offset-x> <offset-y> <blur> <color>
  //   No spread, no inset, no multi-shadow list. Blur is parsed and stored but currently ignored by the
  //   renderer (hard-shadow only in this rebanada); a follow-up will add stack-blur.
  //   The tokenizer splits on whitespace; the last non-numeric token is the colour string. GEN colours never
  //   contain spaces (either a name, "#RRGGBBAA", or a "R,G,B[,A]" tuple without spaces) so this is unambiguous.
  XSTRING boxshadow;
  if(style.Get(__L("box-shadow"), boxshadow) && !boxshadow.IsEmpty())
    {
      double  sh_x    = 0.0;
      double  sh_y    = 0.0;
      double  sh_blur = 0.0;
      XSTRING sh_color;

      if(UI_PROPERTYREGISTRY::ParseBoxShadow(boxshadow, sh_x, sh_y, sh_blur, sh_color))
        {
          element->SetShadowOffsetX(sh_x);
          element->SetShadowOffsetY(sh_y);
          element->SetShadowBlur(sh_blur);
          element->GetShadowColor()->SetFromString(sh_color);
          element->SetBoxShadowSet(true);
        }
    }

  // CSS Grid templates (Phase 3): simple whitespace-separated track list (px / % / fr).
  {
    XSTRING gridcols;
    if(style.Get(__L("grid-template-columns"), gridcols) && !gridcols.IsEmpty())
      {
        element->ClearGridColumnTracks();
        XDWORD len = gridcols.GetSize();
        XDWORD p   = 0;
        while(p < len)
          {
            while(p < len && (gridcols[(int)p] == __C(' ') || gridcols[(int)p] == __C('\t') || gridcols[(int)p] == __C(','))) p++;
            if(p >= len) break;
            XDWORD start = p;
            while(p < len && gridcols[(int)p] != __C(' ') && gridcols[(int)p] != __C('\t') && gridcols[(int)p] != __C(',')) p++;
            XSTRING tok;
            gridcols.Copy((int)start, (int)p, tok);
            if(tok.IsEmpty()) continue;
            UI_GRIDTRACK track;
            track.isminmax = false;
            if(tok.Find(__L("fr"), true) != XSTRING_NOTFOUND)
              {
                track.unit = UI_GRID_TRACK_UNIT_FR;
                tok.DeleteCharacter(__C('f'));
                tok.DeleteCharacter(__C('r'));
                tok.DeleteCharacter(__C('F'));
                tok.DeleteCharacter(__C('R'));
                track.value = tok.ConvertToDouble();
              }
            else if(tok.Find(__L("%"), true) != XSTRING_NOTFOUND)
              {
                track.unit = UI_GRID_TRACK_UNIT_PERCENT;
                tok.DeleteCharacter(__C('%'));
                track.value = tok.ConvertToDouble();
              }
             else
              {
                track.unit  = UI_GRID_TRACK_UNIT_PX;
                track.value = tok.ConvertToDouble();
              }
            element->AddGridColumnTrack(track);
          }
      }

    XSTRING gridrows;
    if(style.Get(__L("grid-template-rows"), gridrows) && !gridrows.IsEmpty())
      {
        element->ClearGridRowTracks();
        XDWORD len = gridrows.GetSize();
        XDWORD p   = 0;
        while(p < len)
          {
            while(p < len && (gridrows[(int)p] == __C(' ') || gridrows[(int)p] == __C('\t') || gridrows[(int)p] == __C(','))) p++;
            if(p >= len) break;
            XDWORD start = p;
            while(p < len && gridrows[(int)p] != __C(' ') && gridrows[(int)p] != __C('\t') && gridrows[(int)p] != __C(',')) p++;
            XSTRING tok;
            gridrows.Copy((int)start, (int)p, tok);
            if(tok.IsEmpty()) continue;
            UI_GRIDTRACK track;
            track.isminmax = false;
            if(tok.Find(__L("fr"), true) != XSTRING_NOTFOUND)
              {
                track.unit = UI_GRID_TRACK_UNIT_FR;
                tok.DeleteCharacter(__C('f'));
                tok.DeleteCharacter(__C('r'));
                tok.DeleteCharacter(__C('F'));
                tok.DeleteCharacter(__C('R'));
                track.value = tok.ConvertToDouble();
              }
            else if(tok.Find(__L("%"), true) != XSTRING_NOTFOUND)
              {
                track.unit = UI_GRID_TRACK_UNIT_PERCENT;
                tok.DeleteCharacter(__C('%'));
                track.value = tok.ConvertToDouble();
              }
             else
              {
                track.unit  = UI_GRID_TRACK_UNIT_PX;
                track.value = tok.ConvertToDouble();
              }
            element->AddGridRowTrack(track);
          }
      }

    double gspan = 0.0;
    if(style.Get(__L("grid-column-span"), gspan) && gspan >= 1.0) element->SetGridColumnSpan((XDWORD)gspan);
    if(style.Get(__L("grid-row-span"), gspan) && gspan >= 1.0)    element->SetGridRowSpan((XDWORD)gspan);
  }

  // Phase 1: persist cascaded bag for typed Reapply / re-layout.
  element->StoreComputedStyle(style);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Text(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element text
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Text(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  XSTRING           text;
  double            sizefont      = 0;
  bool              havemask      = false;
  UI_ELEMENT_TEXT*  element_text  = NULL;
  
  if(element_legacy)
    {
      element_text = (UI_ELEMENT_TEXT*)element_legacy;
    }
   else
    {
      element_text = GEN_NEW UI_ELEMENT_TEXT();
      if(!element_text) return NULL;
    }

  element_text->SetFather(father);

  // Phase 1 (first builder migrated off raw-XML-only reads, per the UI/CSS analysis report's builder table):
  // "outstyle" is the SAME fully-resolved bag (XML attributes < CSS rules < inline style) GetLayoutElement_Base()
  // already used for "color"/"width"/etc. above -- "sizefont" and "maxsizetext" below now read through it too,
  // instead of re-reading the raw XFILEXMLELEMENT* and silently losing any CSS/inline override. GetParentSizeFont()
  // below is left reading raw XML on purpose: it walks ANCESTOR nodes for an inherited size, a distinct fallback
  // mechanism from this element's own resolved style, and is not one of the report's 50 direct-read call sites.
  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_text, outstyle))
    {
      GEN_DELETE element_text;
      return NULL;
    }

  SetLevelAuto(element_text, father);

  // Track D.2: sizefont (GEN) or font-size (CSS-natural alias), first-hit-wins via GetAliased.
  {
    XSTRING sizefontstr;
    if(UI_PROPERTYREGISTRY::GetAliased(outstyle, __L("sizefont"), __L("font-size"), sizefontstr) && !sizefontstr.IsEmpty())
      {
        sizefont = sizefontstr.ConvertToDouble();
      }
     else if(!GetParentSizeFont(node->GetFather(), sizefont))
      {
        GEN_DELETE element_text;
        return NULL;
      }
  }

  element_text->SetSizeFont((XDWORD)sizefont);

  text = node->GetValue().Get();

  XSTRING* textcache = GEN_UI_TEXTS.Get(text);
  if(textcache)  text = textcache->Get();

  if(text.Find(UI_MANAGER_LAYOUT_TEXT_INI, true) != XSTRING_NOTFOUND) 
    {
      if(text.Find(UI_MANAGER_LAYOUT_TEXT_END, true) != XSTRING_NOTFOUND)  havemask = true;
    }
  
  if(havemask)
    {
      element_text->GetMaskText()->Set(text.Get());
      element_text->GetText()->Empty();
      
    } 
   else
    { 
      element_text->GetMaskText()->Empty();
      element_text->GetText()->Set(text.Get());
    }

  double maxsizetext = 0.0f;
  if(outstyle.Get(__L("maxsizetext"), maxsizetext))
    {
      element_text->SetMaxSizeText((XDWORD)maxsizetext);
    }

  if(!element_text->GetMaskText()->IsEmpty()) 
    {
      XSTRING  maskvalue;
      XSTRING  maskresolved;
      XSTRING  _text;
      int      start      = 0;
      int      new_start  = XSTRING_NOTFOUND;
      bool     change     = false;
  
      do{ int new_start = GEN_USERINTERFACE.GetOutputTextChangeID(element_text->GetMaskText(), start, maskvalue);
          if(new_start != XSTRING_NOTFOUND)
            {                                    
              if(maskvalue.Find(UI_MANAGER_LAYOUT_TEXT_TRANSLATE, true) != XSTRING_NOTFOUND)
                {
                  XCHAR*    translate_str  = NULL;
                  XSTRING   maskID;
                  int       ID             = 0;

                  maskID  = UI_MANAGER_LAYOUT_TEXT_TRANSLATE;
                  maskID += __L("%d");

                  maskvalue.UnFormat(maskID.Get(), &ID);

                  translate_str = GEN_XTRANSLATION.Translate_GetSentence(ID);
                  if(translate_str)  maskresolved.Set(translate_str);
                } 
  
              if(!maskresolved.IsEmpty()) 
                {
                  element_text->GetMaskText()->DeleteCharacters(new_start, maskvalue.GetSize() +3 ); 
                  element_text->GetMaskText()->Insert(maskresolved, new_start);  
                  change = true;                
                }
            }

        } while(new_start != XSTRING_NOTFOUND);    


      if(change)
        {
          havemask = false;

          text = element_text->GetMaskText()->Get();
          if(text.Find(UI_MANAGER_LAYOUT_TEXT_INI, true) != XSTRING_NOTFOUND) 
            {
              if(text.Find(UI_MANAGER_LAYOUT_TEXT_END, true) != XSTRING_NOTFOUND)  havemask = true;
            }
  
          if(havemask)
            {
              element_text->GetMaskText()->Set(text.Get());
              element_text->GetText()->Empty();
      
            } 
           else
            { 
              element_text->GetMaskText()->Empty();
              element_text->GetText()->Set(text.Get());
            } 
         }
  
    }
  
  GetLayoutElement_CalculateBoundaryLine(layout, element_text);
   
  return element_text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_TextBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element text box
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_TextBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{  
  UI_ELEMENT_TEXTBOX* element_textbox   = NULL;
  XSTRING             text;

  if(element_legacy)
    {
      element_textbox = (UI_ELEMENT_TEXTBOX*)element_legacy;
    }
   else
    {
      element_textbox = GEN_NEW UI_ELEMENT_TEXTBOX();
      if(!element_textbox) return NULL;
    }

  element_textbox->SetFather(father);

  // Phase 1: same migration pattern as GetLayoutElement_Text() -- "outstyle" is the fully-resolved bag (XML <
  // CSS < inline) GetLayoutElement_Base() already built for "width"/"height"/etc. above; the four keys below
  // (this builder's entire XML-only footprint per the UI/CSS analysis report) now read through it too.
  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_textbox, outstyle, true))
    {
      GEN_DELETE element_textbox;
      return NULL;
    }

  SetLevelAuto(element_textbox, father);

  if(!element_textbox->GetBoundaryLine()->width)  return NULL;
  if(!element_textbox->GetBoundaryLine()->height) return NULL;

  double sizefont = 0;
  {
    XSTRING sizefontstr;
    if(UI_PROPERTYREGISTRY::GetAliased(outstyle, __L("sizefont"), __L("font-size"), sizefontstr) && !sizefontstr.IsEmpty())
      {
        sizefont = sizefontstr.ConvertToDouble();
      }
     else if(!GetParentSizeFont(node->GetFather(), sizefont))
      {
        GEN_DELETE element_textbox;
        return NULL;
      }
  }

  element_textbox->SetSizeFont((XDWORD)sizefont);

  double linespacing = UI_ELEMENT_TEXTBOX_DEFAULTLINESPACING;
  outstyle.Get(__L("linespacing"), linespacing);
  element_textbox->SetLineSpacing((XDWORD)linespacing);

  // "textalignment"/"text-align": same alias pair GetLayoutElement_Base() already resolves for the base
  // UI_ELEMENT::SetTextAlign() above -- UI_ELEMENT_TEXTBOX::SetTextAlignment() is a distinct, textbox-specific
  // setter (out of scope to unify here), but the property NAME and its CSS-natural alias are the same, so this
  // read goes through the same UI_PROPERTYREGISTRY lookup rather than a third hand-rolled copy of it.
  XSTRING                textalignmentstr;
  UI_ELEMENT_TYPE_ALIGN  textalignment;
  if(outstyle.GetAliased(__L("textalignment"), __L("text-align"), textalignmentstr))
    {
      if(!textalignmentstr.Compare(__L("left"), true))  textalignment = UI_ELEMENT_TYPE_ALIGN_LEFT;
        else if(!textalignmentstr.Compare(__L("right"), true))  textalignment = UI_ELEMENT_TYPE_ALIGN_RIGHT;
          else if(!textalignmentstr.Compare(__L("center"), true)) textalignment = UI_ELEMENT_TYPE_ALIGN_CENTER;
    }
  element_textbox->SetTextAlignment(textalignment);


  XSTRING wordclippingstr;
  element_textbox->SetIsWordClipping(false);
  if(outstyle.Get(__L("wordclipping"), wordclippingstr))
    {
      element_textbox->SetIsWordClipping(wordclippingstr.ConvertToBoolean());
    }

  text.Empty();

  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* subnode = node->GetElement(c);
      if(subnode)
        {
          if(!subnode->GetName().Compare(__L("text"), true))
            {                                                                  
              text.Add(subnode->GetValue().Get());
              text.Add(__L("\n"));
            }    
        }
    }
  
  if(!text.IsEmpty()) 
    {
      XSTRING  value;
      XSTRING  resolved;      
      int      start      = 0;
      int      new_start  = XSTRING_NOTFOUND;
      bool     change     = false;
  
      do{ int new_start = GEN_USERINTERFACE.GetOutputTextChangeID(&text, start, value);
          if(new_start != XSTRING_NOTFOUND)
            {                                    
              if(value.Find(UI_MANAGER_LAYOUT_TEXT_TRANSLATE, true) != XSTRING_NOTFOUND)
                {
                  XCHAR*    translate_str  = NULL;
                  XSTRING   IDstr;
                  int       ID             = 0;

                  IDstr  = UI_MANAGER_LAYOUT_TEXT_TRANSLATE;
                  IDstr += __L("%d");

                  value.UnFormat(IDstr.Get(), &ID);

                  translate_str = GEN_XTRANSLATION.Translate_GetSentence(ID);
                  if(translate_str) resolved.Set(translate_str);
                } 
  
              if(!resolved.IsEmpty()) 
                {
                  text.DeleteCharacters(new_start, value.GetSize() +3 ); 
                  text.Insert(resolved, new_start);  
                  change = true;                
                }
            }

        } while(new_start != XSTRING_NOTFOUND);      
    }

  element_textbox->GetMaskText()->Set(text);

  GetLayoutElement_CalculateBoundaryLine(layout, element_textbox, true);

  return element_textbox;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Image(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element image
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Image(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  XPATH             xpathimg;
  XSTRING           namefileimg;
  UI_ELEMENT_IMAGE* element_image = NULL;

  if(element_legacy)
    {
      element_image = (UI_ELEMENT_IMAGE*)element_legacy;
    }
   else
    {
      element_image = GEN_NEW UI_ELEMENT_IMAGE();
      if(!element_image) return NULL;
    }

  element_image->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_image, outstyle))
    {
      GEN_DELETE element_image;
      return NULL;
    }

  SetLevelAuto(element_image, father);

  double alpha = 0;
  if(outstyle.Get(__L("alpha"), alpha))
    {
      element_image->SetAlpha((XBYTE)alpha);
    }

  namefileimg = node->GetValue().Get();

 
  if(!namefileimg.IsEmpty()) 
    {
      GRPPROPERTYMODE   grppropertymode = GRPPROPERTYMODE_XX_UNKNOWN;
      UI_SKIN_DRAWMODE  drawmode        = UI_SKIN_DRAWMODE_UNKNOWN;
      GRP2DCANVAS*      referencecanvas = NULL;

      if(layout->GetSkin())
        {
          drawmode = layout->GetSkin()->GetDrawMode();

          switch(drawmode)
            {
              case UI_SKIN_DRAWMODE_UNKNOWN   : break;

              case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin(); 
                                                  if(skincanvas)
                                                    {
                                                      referencecanvas = skincanvas->GetCanvas();
                                                      grppropertymode = referencecanvas->GetMode();
                                                    }
                                                }
                                                break;
   
              case UI_SKIN_DRAWMODE_CONTEXT   : break;
            }  
        }

      // NOTE: a vector resource (SVG, DXF...) has no pixel size of its own, so it is always rasterized at the
      // element's own configured width/height (already resolved by GetLayoutElement_Base() above). Bitmap files
      // ignore these two values entirely (see GetOrAddAnimationCache()).
      double width  = element_image->GetBoundaryLine()->width;
      double height = element_image->GetBoundaryLine()->height;

      element_image->SetResource(namefileimg.Get());

      UI_ANIMATION* animation = GetOrAddAnimationCache(drawmode, grppropertymode, __L(""), namefileimg.Get(), referencecanvas, width, height);
      if(animation) 
        { 
          GRPBITMAP* bitmap = NULL; 
          bitmap = animation->GetBitmap(); 
          if(bitmap) element_image->SetImage(bitmap);             
        }
    }
     
  GetLayoutElement_CalculateBoundaryLine(layout, element_image);
  
  return element_image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Animation(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element animation
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Animation(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  XSTRING               text;
  XSTRING               namefileimg;
  XPATH                 xpathimg;
  int                   sizefont          = 0;
  UI_ELEMENT_ANIMATION* element_animation = NULL;

  if(element_legacy)
    {
      element_animation = (UI_ELEMENT_ANIMATION*)element_legacy;
    }
   else
    {
      element_animation = GEN_NEW UI_ELEMENT_ANIMATION();
      if(!element_animation) return NULL;
    }

  element_animation->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_animation, outstyle))
    {
      GEN_DELETE element_animation;
      return NULL;
    }

  SetLevelAuto(element_animation, father);

  XSTRING statestring;
  if(outstyle.Get(__L("state"), statestring))
    {
      if(!statestring.Compare(__L("play"), true))                                                                     
        {
          element_animation->Play();
        }
        else
        {
          if(!statestring.Compare(__L("stop"), true))                                                                     
            {
              element_animation->Stop();
            }   
            else
            {       
              if(!statestring.Compare(__L("pause"), true))                                                                     
                {
                  element_animation->Pause();    
                }
            }
        }
    }

  double timedelay = 0;
  if(outstyle.Get(__L("timedelay"), timedelay))
    {
      element_animation->SetMilliSecondsDelay((XDWORD)timedelay);
    }

  double alpha = 0;
  if(outstyle.Get(__L("alpha"), alpha))
    {
      element_animation->SetAlpha((XBYTE)alpha);
    }

  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement =  node->GetElement(c);
      if(nodeelement)
        {  
          if(!nodeelement->GetName().Compare(__L("image")))
            {              
              namefileimg = nodeelement->GetValue(); 

              GRPPROPERTYMODE   grppropertymode = GRPPROPERTYMODE_XX_UNKNOWN;
              UI_SKIN_DRAWMODE  drawmode        = UI_SKIN_DRAWMODE_UNKNOWN;
              GRP2DCANVAS*      referencecanvas = NULL;

              if(layout->GetSkin())
                {
                  drawmode = layout->GetSkin()->GetDrawMode();

                  switch(drawmode)
                    {
                      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

                      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin(); 
                                                          if(skincanvas)
                                                            {
                                                              referencecanvas = skincanvas->GetCanvas();
                                                              grppropertymode = referencecanvas ? referencecanvas->GetMode() : GRPPROPERTYMODE_XX_UNKNOWN;
                                                            }
                                                        }
                                                        break;
   
                      case UI_SKIN_DRAWMODE_CONTEXT   : break;
                    }  
                }      

              // NOTE: a vector resource (SVG, DXF...) has no pixel size of its own, so it is always rasterized at
              // the <image> tag's own width/height (same size-aware cache as GetLayoutElement_Image). Bitmap
              // frames (the usual case for sprite animations) ignore these two values entirely.
              double framewidth  = 0.0;
              double frameheight = 0.0;

              GetLayoutElementValue(nodeelement, __L("width") , framewidth);
              GetLayoutElementValue(nodeelement, __L("height"), frameheight);

              UI_ANIMATION* animation = GetOrAddAnimationCache(drawmode, grppropertymode, __L(""), namefileimg.Get(), referencecanvas, framewidth, frameheight);
              if(animation)
                {
                  for(XDWORD d=0; d<animation->GetBitmaps()->GetSize(); d++)
                    {
                      UI_ELEMENT_IMAGE* element_img = GEN_NEW UI_ELEMENT_IMAGE();
                      if(element_img) 
                        {
                          GRPBITMAP* bitmap = NULL; 

                          element_img->SetFather(element_animation);       
                          element_img->SetResource(namefileimg.Get());

                          GetLayoutElement_Base(nodeelement, layout, element_img); 

                          SetLevelAuto(element_img, element_animation);

                          bitmap = animation->GetBitmaps()->Get(d);
                          if(bitmap) 
                            { 
                              element_img->SetImage(bitmap);
                              
                              if(layout->GetSkin())
                                {
                                  switch(layout->GetSkin()->GetDrawMode())
                                    {
                                      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

                                      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin(); 
                                                                          if(skincanvas) skincanvas->CalculateBoundaryLine(element_img); 
                                                                        }
                                                                        break;
   
                                      case UI_SKIN_DRAWMODE_CONTEXT   : break;

                                    }
                                 }

                              element_img->SetAlpha(element_animation->GetAlpha());
                                                                  
                              element_animation->GetComposeElements()->Add(element_img);                                                                                                
                            } 
                        }
                    }      
                }
            }
        }
    }

  GetLayoutElement_CalculateBoundaryLine(layout, element_animation);

  return element_animation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Option(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element option
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Option(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  XSTRING             text;
  XSTRING             namefileimg;
  XPATH               xpathimg;
  double              sizefont  = 0;
  UI_ELEMENT_OPTION*  element_option = NULL;

  if(element_legacy)
    {
      element_option =  (UI_ELEMENT_OPTION*)element_legacy;
    }
   else
    {
      element_option = GEN_NEW UI_ELEMENT_OPTION();
      if(!element_option) return NULL;
    }

  element_option->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_option, outstyle))
    {
      GEN_DELETE element_option;
      return NULL;
    }

  SetLevelAuto(element_option, father);

  element_option->SetActive(true);

  outstyle.Get(__L("sizefont"), sizefont);   // read but not applied -- pre-existing, unrelated to this migration


  XSTRING selectablestr;
  outstyle.Get(__L("selectablestatus"), selectablestr);
  element_option->SetSelectableStateFromString(selectablestr);

  XSTRING allocationtextstr;
  if(outstyle.Get(__L("allocationtext"), allocationtextstr))
    {
      if(!allocationtextstr.Compare(__L("none")     , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE);  
      if(!allocationtextstr.Compare(__L("up")       , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP);
      if(!allocationtextstr.Compare(__L("down")     , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN);  
      if(!allocationtextstr.Compare(__L("right")    , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT);  
	    if(!allocationtextstr.Compare(__L("left")     , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT); 
      if(!allocationtextstr.Compare(__L("center")   , true)) element_option->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER); 
    }
  
  element_option->SetVisibleLimitType(UI_ELEMENT_OPTION_VISIBLE_LIMIT_NONE);

  XSTRING visiblelimittypestr;
  if(outstyle.Get(__L("visiblelimit"), visiblelimittypestr))
    {
      if(visiblelimittypestr.Find(__L("active"), true) != XSTRING_NOTFOUND) 
        {
          element_option->SetVisibleLimitType(element_option->GetVisibleLimitType() | UI_ELEMENT_OPTION_VISIBLE_LIMIT_ACTIVE);
        }

      if(visiblelimittypestr.Find(__L("deactive"), true) != XSTRING_NOTFOUND) 
        {
          element_option->SetVisibleLimitType(element_option->GetVisibleLimitType() | UI_ELEMENT_OPTION_VISIBLE_LIMIT_DEACTIVE);
        }

      if(visiblelimittypestr.Find(__L("preselect"), true) != XSTRING_NOTFOUND) 
        {
          element_option->SetVisibleLimitType(element_option->GetVisibleLimitType() | UI_ELEMENT_OPTION_VISIBLE_LIMIT_PRESELECT);
        }

      if(visiblelimittypestr.Find(__L("select"), true) != XSTRING_NOTFOUND) 
        {
          element_option->SetVisibleLimitType(element_option->GetVisibleLimitType() | UI_ELEMENT_OPTION_VISIBLE_LIMIT_SELECT);
        }
    }
  

  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement =  node->GetElement(c);
      if(nodeelement)
        {
          XSTRING type;
          if(GetLayoutElementValue(nodeelement, __L("type"), type))
            {        
              UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_option);
              if(element) 
                {                   
                  switch(element->GetType())
                    {
                      case UI_ELEMENT_TYPE_ANIMATION  : element_option->Set_UIAnimation((UI_ELEMENT_ANIMATION*)element);                    
                                                        element_option->GetComposeElements()->Add(element);
                                                        break;

                      case UI_ELEMENT_TYPE_TEXT       : element_option->Set_UIText((UI_ELEMENT_TEXT*)element);                    
                                                        element_option->GetComposeElements()->Add(element);
                                                        break; 

                                        default       : break;                    
                    }                     
                }              
            }
        }  
    }
  
  GetLayoutElement_CalculateBoundaryLine(layout, element_option);

  return element_option;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_MultiOption(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element multi option
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_MultiOption(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  UI_ELEMENT_MULTIOPTION* element_multioption = NULL;
  double                  sizefont            = 0;

  if(element_legacy)
    {
      element_multioption =  (UI_ELEMENT_MULTIOPTION*)element_legacy;
    }
   else
    {
      element_multioption = GEN_NEW UI_ELEMENT_MULTIOPTION();
      if(!element_multioption) return NULL;     
    }

  element_multioption->SetFather(father);
 
  if(!GetLayoutElement_Base(node, layout, element_multioption))
    {
      GEN_DELETE element_multioption;
      return NULL;
    }

  SetLevelAuto(element_multioption, father);
    
  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement =  node->GetElement(c);
      if(nodeelement)
        {
          UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_multioption);
          if(element) 
            {  
              element->SetFather(element_multioption);                
              element_multioption->GetComposeElements()->Add(element);                        
            }       
        }  
    }
  
  GetLayoutElement_CalculateBoundaryLine(layout, element_multioption);

  for(XDWORD c=0; c<element_multioption->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* element = element_multioption->GetComposeElements()->Get(c);
      if(element) GetLayoutElement_CalculateBoundaryLine(layout, element);
    }
 
  return element_multioption;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Button(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element button
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Button(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{  
  UI_ELEMENT_BUTTON* element_button = GEN_NEW UI_ELEMENT_BUTTON();
  if(!element_button) return NULL;

  return GetLayoutElement_Option(node, layout, father, element_button);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_CheckBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element check box
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_CheckBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout,  UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{  
  UI_ELEMENT_CHECKBOX* element_checkbox = GEN_NEW UI_ELEMENT_CHECKBOX();
  if(!element_checkbox) return NULL;

  return GetLayoutElement_Option(node, layout,  father, element_checkbox);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_EditText(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element edit text
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_EditText(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{  
  UI_ELEMENT_EDITTEXT* element_edittext = GEN_NEW UI_ELEMENT_EDITTEXT();
  if(!element_edittext) return NULL;

  return GetLayoutElement_Text(node, layout, father, element_edittext);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Form(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element form
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Form(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  UI_ELEMENT_FORM* element_form  = NULL;
  double           sizefont      = 0;

  if(element_legacy)
    {
      element_form =  (UI_ELEMENT_FORM*)element_legacy;
    }
   else
    {
      element_form = GEN_NEW UI_ELEMENT_FORM();
      if(!element_form) return NULL;      
    }

  element_form->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_form, outstyle))
    {
      GEN_DELETE element_form;
      return NULL;
    }

  SetLevelAuto(element_form, father);

  outstyle.GetColor(__L("linecolor"), *element_form->GetLineColor());

  GetLayoutElement_CalculateBoundaryLine(layout, element_form);

  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement =  node->GetElement(c);
      if(nodeelement)
        {
          UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_form);
          if(element) 
            {  
              element->SetFather(element_form);                
              element_form->GetComposeElements()->Add(element);                        
            }       
        }  
    }

  GetLayoutElement_CalculateBoundaryLine(layout, element_form);

  for(XDWORD c=0; c<element_form->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element_form->GetComposeElements()->Get(c);
      if(subelement) GetLayoutElement_CalculateBoundaryLine(layout, subelement);
    }

  XSTRING visibleformstr;
  if(outstyle.Get(__L("visiblerect"), visibleformstr))
    {
      if(!visibleformstr.IsEmpty())
        {                     
          int value[4] = { 0, 0, 0, 0 }; 

          visibleformstr.UnFormat(__L("%d,%d,%d,%d")  , &value[0], &value[1], &value[2], &value[3]); 

          element_form->GetVisibleRect()->x = element_form->GetXPosition() + value[0];
          element_form->GetVisibleRect()->y = element_form->GetYPosition() + value[1];      

          if(value[2]) 
            {       
              element_form->GetVisibleRect()->width = value[2]; 
            }
           else
            {
              element_form->GetVisibleRect()->width = element_form->GetBoundaryLine()->width; 
            }

          if(value[3])
            {
              element_form->GetVisibleRect()->height  = value[3];
            }
           else
            {
              element_form->GetVisibleRect()->height  = element_form->GetBoundaryLine()->height;
            }

          element_form->GetBoundaryLine()->SetAround((*element_form->GetVisibleRect()));

          // FORM VISIBLERECT/INTRINSIC-SIZE FIX (2026-09): the SetAround() call just above can WIDEN
          // GetBoundaryLine() beyond the natural, content-only size that UI_SKIN::CalculateBoundaryLine()
          // (invoked a few lines above via GetLayoutElement_CalculateBoundaryLine() -> CalculateBoundaryLine_Form())
          // already snapshotted into GetIntrinsicWidth()/Height() -- see that function's own "Phase 4" comment,
          // which added the snapshot to make UI_LAYOUTENGINE::RunLayout() idempotent across repeated flex passes.
          // That snapshot is taken the moment CalculateBoundaryLine_Form() returns, i.e. BEFORE this visiblerect
          // union runs, so for a form whose authored visiblerect is wider/taller than its own auto-fit content
          // (exactly this branch: "value[2]"/"value[3]" authored, or the width/height fallback above) the
          // snapshot is left stale -- narrower than the boundary this union just produced.
          //
          // UI_LAYOUTENGINE::RunLayout() then runs UNCONDITIONALLY on every top-level layout element right after
          // construction (see UI_MANAGER::CreateLayouts()), regardless of whether the layout authors any CSS/
          // flex at all. Its BuildTree() prefers a set GetIntrinsicWidth()/Height() over the live boundary (by
          // design, for the idempotency fix above), so it reads back the STALE, pre-union value and its
          // WriteBackTree() writes it straight into GetBoundaryLine() -- silently UNDOING the widening this
          // union just performed.
          //
          // Confirmed live on "menu_horz" (UI_Options' example.xml, xpos=60 visiblerect="0,0,550,70" over a
          // narrower ~520px-wide row of buttons): GetBoundaryLine() read 550 wide right here, but only 520 wide
          // by the time Draw_Form() ran. Draw_Form()'s own fill/stroke paints the FULL visiblerect every frame
          // (GetVisibleRect(), a separate property RunLayout() never touches) but the "ALPHA-DARKENING FIX"'s
          // true-backdrop capture/restore area is sized from GetBoundaryLine() (see
          // UI_SKINCANVAS::PreDrawFunction()) -- so the ~30px trailing strip the shrunk boundary no longer
          // covered was painted every redraw WITHOUT ever being restored first, and its translucent fill
          // alpha-compounded toward solid, opaque colour within the first few of the many redraws a running
          // animation drives per second (a spinning child icon forces continuous full-row redraws): visually a
          // dark rectangle appearing right next to the row's last button, present from moments after startup.
          //
          // Fix: re-snapshot the intrinsic size from the NOW-current (post-union) boundary, so RunLayout() sees
          // the same value CalculateBoundaryLine()'s own snapshot would have produced had it run after this
          // union instead of before it. Mirrors that function's own snapshot call exactly (same two setters,
          // same source), just re-run at the point where the boundary is actually final for this element.
          element_form->SetIntrinsicWidth(element_form->GetBoundaryLine()->width);
          element_form->SetIntrinsicHeight(element_form->GetBoundaryLine()->height);
        }
    }
   else
    {
      element_form->GetVisibleRect()->x       = element_form->GetXPosition();
      element_form->GetVisibleRect()->y       = element_form->GetYPosition();
      element_form->GetVisibleRect()->width   = element_form->GetBoundaryLine()->width;
      element_form->GetVisibleRect()->height  = element_form->GetBoundaryLine()->height;
    }

  return element_form;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_Menu(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element menu
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_Menu(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{   
  UI_ELEMENT_MENU* element_menu = GEN_NEW UI_ELEMENT_MENU();
  if(!element_menu) return NULL;

  return GetLayoutElement_Form(node, layout, father, element_menu);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_ListBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element list box
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_ListBox(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  UI_ELEMENT_LISTBOX* element_listbox = GEN_NEW UI_ELEMENT_LISTBOX();
  if(!element_listbox) return NULL;

  UI_ELEMENT* element_menu = NULL;
  XSTRING     menustr;
  GetLayoutElementValue(node, __L("menu"), menustr);    
  if(!menustr.IsEmpty()) 
    {
      element_menu = Element_Get(menustr, UI_ELEMENT_TYPE_MENU);  
      element_listbox->Set_UIMenu((UI_ELEMENT_MENU*)element_menu);              
    }

  double defaultoption = 0.0f;
  GetLayoutElementValue(node, __L("defaultoption"), defaultoption);    
  if(element_menu)
    {
      UI_ELEMENT_OPTION* element_option = (UI_ELEMENT_OPTION*)element_menu->GetComposeElements()->Get((XDWORD)defaultoption);
      if(element_option) 
        {
          XSTRING text;

          if(element_option->Get_UIText()) text = element_option->Get_UIText()->GetText()->Get();
          //element_listbox->GetMaskText()->Set(text);
          node->GetValue().Set(text);
        }
    }

  return GetLayoutElement_Text(node, layout, father, element_listbox);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressBar(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element progress bar
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressBar(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  XSTRING                   text;
  XSTRING                   namefileimg;
  XPATH                     xpathimg;
  double                    sizefont  = 0;
  UI_ELEMENT_PROGRESSBAR*   element_progressbar = NULL;

  if(element_legacy)
    {
      element_progressbar =  (UI_ELEMENT_PROGRESSBAR*)element_legacy;
    }
   else
    {
      element_progressbar = GEN_NEW UI_ELEMENT_PROGRESSBAR();
      if(!element_progressbar) return NULL;
    }

  element_progressbar->SetFather(father);

  // Phase 1: same pattern as the other migrated builders -- "outstyle" is the fully-resolved bag (XML < CSS <
  // inline) GetLayoutElement_Base() already built above. The "type" check on child nodes further below is left
  // reading raw XML on purpose, same rationale as GetLayoutElement_Option(): it selects which sub-element
  // handler to invoke, not a stylable value.
  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_progressbar, outstyle))
    {
      GEN_DELETE element_progressbar;
      return NULL;
    }

  SetLevelAuto(element_progressbar, father);

  element_progressbar->SetActive(true);

  outstyle.Get(__L("sizefont"), sizefont);   // read but not applied -- pre-existing, unrelated to this migration


  XSTRING allocationtextstr;
  if(outstyle.Get(__L("allocationtext"), allocationtextstr))
    {
      if(!allocationtextstr.Compare(__L("none")     , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE);
      if(!allocationtextstr.Compare(__L("up")       , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP);
      if(!allocationtextstr.Compare(__L("down")     , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN);
      if(!allocationtextstr.Compare(__L("right")    , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT);
	    if(!allocationtextstr.Compare(__L("left")     , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT);
      if(!allocationtextstr.Compare(__L("center")   , true)) element_progressbar->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER);
    }


  outstyle.GetColor(__L("linecolor")    , *element_progressbar->GetLineColor());
  outstyle.GetColor(__L("gradientcolor"), *element_progressbar->GetGradientColor());

  XSTRING gradientmode;
  if(outstyle.Get(__L("gradientmode"), gradientmode))
    {
      if(!gradientmode.Compare(__L("track"), true)) element_progressbar->SetGradientMode(UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK);
      if(!gradientmode.Compare(__L("fill") , true)) element_progressbar->SetGradientMode(UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL);
    }

  double levelvalue = 0.0f;
  if(outstyle.Get(__L("level"), levelvalue)) element_progressbar->SetLevel((float)levelvalue);

  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement =  node->GetElement(c);
      if(nodeelement)
        {
          XSTRING type;
          if(GetLayoutElementValue(nodeelement, __L("type"), type))
            {        
              UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_progressbar);
              if(element) 
                {            
                  element->SetFather(element_progressbar);  
       
                  switch(element->GetType())
                    {
                      case UI_ELEMENT_TYPE_ANIMATION  : element_progressbar->Set_UIAnimation((UI_ELEMENT_ANIMATION*)element);                    
                                                        element_progressbar->GetComposeElements()->Add(element);
                                                        break;

                      case UI_ELEMENT_TYPE_TEXT       : element_progressbar->Set_UIText((UI_ELEMENT_TEXT*)element);                    
                                                        element_progressbar->GetComposeElements()->Add(element);
                                                        break;  

                                        default       : break;                   
                    }                     
                }              
            }
        }  
    }

  XSTRING roundcapstr;
  if(outstyle.Get(__L("roundcap"), roundcapstr))
    {
      if(!roundcapstr.Compare(__L("yes"),  true) ||
         !roundcapstr.Compare(__L("true"), true) ||
         !roundcapstr.Compare(__L("1"),    true))   element_progressbar->SetRoundCap(true);
    }

  XSTRING continuouscyclestr;
  if(outstyle.Get(__L("continuouscycle"), continuouscyclestr))
    {
      if(!continuouscyclestr.IsEmpty())
        {
          int value[3] = { 0, 0, 0 }; 

          continuouscyclestr.UnFormat(__L("%d,%d,%d")  , &value[0], &value[1], &value[2]); 

          if(!value[0]) value[0] = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSIZESEGMENT;
          if(!value[1]) value[1] = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSTEPSEGMENT;
          if(!value[1]) value[1] = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTTIMESTEP;

          element_progressbar->ContinuousCycle_Set(true,  value[0], value[1], value[2]);             
        }
    }

  XSTRING progressrectstr;
  if(outstyle.Get(__L("progressrect"), progressrectstr))
    {
      if(!progressrectstr.IsEmpty())
        {
          UI_ELEMENT* element_progressrect = NULL;

          element_progressrect = GEN_NEW UI_ELEMENT();
          if(element_progressrect)
            {
              int value[4] = { 0, 0, 0, 0 }; 

              progressrectstr.UnFormat(__L("%d,%d,%d,%d")  , &value[0], &value[1], &value[2], &value[3]); 

              element_progressrect->GetBoundaryLine()->x = element_progressbar->GetXPosition() + value[0];
              element_progressrect->GetBoundaryLine()->y = element_progressbar->GetYPosition() + value[1];      

              if(value[2]) 
                {       
                  element_progressrect->GetBoundaryLine()->width = value[2]; 
                }
               else
                {
                  element_progressrect->GetBoundaryLine()->width = element_progressbar->GetBoundaryLine()->width; 
                }

              if(value[3])
                {
                  element_progressrect->GetBoundaryLine()->height  = value[3];
                }
               else
                {
                  element_progressrect->GetBoundaryLine()->height  = element_progressbar->GetBoundaryLine()->height;
                }

              //element_progressbar->GetBoundaryLine()->SetAround((*element_progressrect->GetBoundaryLine()));
              element_progressbar->SetProgressRect(element_progressrect);
              element_progressrect->SetFather(element_progressbar);

              element_progressbar->GetComposeElements()->Add(element_progressrect);
            }
        }
    }
   else 
    {
      element_progressbar->SetProgressRect(NULL);
    }


  ChangeTextElementValue(layout, element_progressbar);                                // FIX1: resolve the percent text to its real value BEFORE measuring the boundary

  GetLayoutElement_CalculateBoundaryLine(layout, element_progressbar);                // calculate the boundary / rebuild area. This call was accidentally overwritten by FIX1; for bars WITH text the boundary
                                                                                      // was re-derived inside ChangeTextElementValue (FIX2), masking the loss, but a textless continuous bar was never measured -> not drawn.

  return element_progressbar;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressRadial(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element radial progress
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressRadial(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  double                    value         = 0.0f;
  UI_ELEMENT_PROGRESS_RADIAL*  element_progress = NULL;

  if(element_legacy)
    {
      element_progress = (UI_ELEMENT_PROGRESS_RADIAL*)element_legacy;
    }
   else
    {
      element_progress = GEN_NEW UI_ELEMENT_PROGRESS_RADIAL();
      if(!element_progress) return NULL;
    }

  element_progress->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_progress, outstyle))
    {
      GEN_DELETE element_progress;
      return NULL;
    }

  SetLevelAuto(element_progress, father);

  element_progress->SetActive(true);

  // Value arc gradient END color (gradient START is the base "color"; track ring is "bckgrdcolor").
  outstyle.GetColor(__L("linecolor")    , *element_progress->GetLineColor());
  outstyle.GetColor(__L("gradientcolor"), *element_progress->GetGradientColor());

  XSTRING gradientmode;
  if(outstyle.Get(__L("gradientmode"), gradientmode))
    {
      if(!gradientmode.Compare(__L("track"), true)) element_progress->SetGradientMode(UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK);
      if(!gradientmode.Compare(__L("fill") , true)) element_progress->SetGradientMode(UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL);
    }

  // Level [0..100].
  value = 0.0f;
  if(outstyle.Get(__L("level"), value))      element_progress->SetLevel((float)value);

  // Geometry.
  value = 0.0f;
  if(outstyle.Get(__L("startangle"), value)) element_progress->SetStartAngle(value);

  value = 0.0f;
  if(outstyle.Get(__L("sweepangle"), value)) element_progress->SetSweepAngle(value);

  value = 0.0f;
  if(outstyle.Get(__L("thickness"), value))  element_progress->SetThickness(value);

  XSTRING roundcapstr;
  if(outstyle.Get(__L("roundcap"), roundcapstr))
    {
      if(!roundcapstr.Compare(__L("yes"),  true) ||
         !roundcapstr.Compare(__L("true"), true) ||
         !roundcapstr.Compare(__L("1"),    true))   element_progress->SetRoundCap(true);
    }

  // Child <text> => centered caption.
  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement = node->GetElement(c);
      if(nodeelement)
        {
          XSTRING type;
          if(GetLayoutElementValue(nodeelement, __L("type"), type))
            {
              UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_progress);
              if(element)
                {
                  element->SetFather(element_progress);

                  switch(element->GetType())
                    {
                      case UI_ELEMENT_TYPE_TEXT : element_progress->Set_UIText((UI_ELEMENT_TEXT*)element);
                                                  element_progress->GetComposeElements()->Add(element);
                                                  break;

                                      default   : break;
                    }
                }
            }
        }
    }

  GetLayoutElement_CalculateBoundaryLine(layout, element_progress);

  return element_progress;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressImage(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element progress image
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_ProgressImage(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  double                      value                 = 0.0f;
  UI_ELEMENT_PROGRESS_IMAGE*  element_progressimage  = NULL;

  if(element_legacy)
    {
      element_progressimage = (UI_ELEMENT_PROGRESS_IMAGE*)element_legacy;
    }
   else
    {
      element_progressimage = GEN_NEW UI_ELEMENT_PROGRESS_IMAGE();
      if(!element_progressimage) return NULL;
    }

  element_progressimage->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_progressimage, outstyle))
    {
      GEN_DELETE element_progressimage;
      return NULL;
    }

  SetLevelAuto(element_progressimage, father);

  element_progressimage->SetActive(true);

  XSTRING allocationtextstr;
  if(outstyle.Get(__L("allocationtext"), allocationtextstr))
    {
      if(!allocationtextstr.Compare(__L("none")     , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE);
      if(!allocationtextstr.Compare(__L("up")       , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP);
      if(!allocationtextstr.Compare(__L("down")     , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN);
      if(!allocationtextstr.Compare(__L("right")    , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT);
      if(!allocationtextstr.Compare(__L("left")     , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT);
      if(!allocationtextstr.Compare(__L("center")   , true)) element_progressimage->SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER);
    }

  // alpha [0..100]
  value = 0.0f;
  if(outstyle.Get(__L("alpha"), value)) element_progressimage->SetAlpha((XBYTE)value);

  // level [0..100]
  value = 0.0f;
  if(outstyle.Get(__L("level"), value)) element_progressimage->SetLevel((float)value);

  value = 0.0f;
  if(outstyle.Get(__L("offsetstart"), value)) element_progressimage->SetOffsetStart(value);

  value = 0.0f;
  if(outstyle.Get(__L("offsetend"),   value)) element_progressimage->SetOffsetEnd(value);

  // resolve the draw mode once (same as GetLayoutElement_Image)
  GRPPROPERTYMODE   grppropertymode = GRPPROPERTYMODE_XX_UNKNOWN;
  UI_SKIN_DRAWMODE  drawmode        = UI_SKIN_DRAWMODE_UNKNOWN;

  if(layout->GetSkin())
    {
      drawmode = layout->GetSkin()->GetDrawMode();

      switch(drawmode)
        {
          case UI_SKIN_DRAWMODE_UNKNOWN   : break;

          case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)layout->GetSkin();
                                              if(skincanvas) grppropertymode = skincanvas->GetCanvas()->GetMode();
                                            }
                                            break;

          case UI_SKIN_DRAWMODE_CONTEXT   : break;
        }
    }

  // empty (0%) graphic
  XSTRING fileempty;
  outstyle.Get(__L("imageempty"), fileempty);
  if(!fileempty.IsEmpty())
    {
      UI_ANIMATION* animation = GetOrAddAnimationCache(drawmode, grppropertymode, __L(""), fileempty.Get());
      if(animation && animation->GetBitmap()) element_progressimage->SetImageEmpty(animation->GetBitmap());
    }

  // full (100%) graphic
  XSTRING filefull;
  outstyle.Get(__L("imagefull"), filefull);
  if(!filefull.IsEmpty())
    {
      UI_ANIMATION* animation = GetOrAddAnimationCache(drawmode, grppropertymode, __L(""), filefull.Get());
      if(animation && animation->GetBitmap()) element_progressimage->SetImageFull(animation->GetBitmap());
    }

  // child <text> => centered caption
  for(int c=0; c<node->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeelement = node->GetElement(c);
      if(nodeelement)
        {
          XSTRING type;
          if(GetLayoutElementValue(nodeelement, __L("type"), type))
            {
              UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, element_progressimage);
              if(element)
                {
                  element->SetFather(element_progressimage);

                  switch(element->GetType())
                    {
                      case UI_ELEMENT_TYPE_TEXT : element_progressimage->Set_UIText((UI_ELEMENT_TEXT*)element);
                                                  element_progressimage->GetComposeElements()->Add(element);
                                                  break;

                                      default   : break;
                    }
                }
            }
        }
    }

  ChangeTextElementValue(layout, element_progressimage);                            
  GetLayoutElement_CalculateBoundaryLine(layout, element_progressimage);

  return element_progressimage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::GetLayoutElement_StatisticsChart(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
* @brief      Get layout element statistics chart
* @ingroup    USERINTERFACE
* 
* @param[in]  node : Node pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* @param[in]  element_legacy : Element legacy pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::GetLayoutElement_StatisticsChart(XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy)
{
  UI_ELEMENT_STATISTICSCHART* element_chart = NULL;

  if(element_legacy)
    {
      element_chart = (UI_ELEMENT_STATISTICSCHART*)element_legacy;
    }
   else
    {
      element_chart = GEN_NEW UI_ELEMENT_STATISTICSCHART();
      if(!element_chart) return NULL;
    }

  element_chart->SetFather(father);

  UI_COMPUTEDSTYLE outstyle;
  if(!GetLayoutElement_Base(node, layout, element_chart, outstyle))
    {
      GEN_DELETE element_chart;
      return NULL;
    }

  SetLevelAuto(element_chart, father);

  double alpha = 0;
  if(outstyle.Get(__L("alpha"), alpha))
    {
      element_chart->SetAlpha((XBYTE)alpha);
    }

  XSTRING charttypestr;
  if(outstyle.Get(__L("chart"), charttypestr))
    {
      if(!charttypestr.Compare(__L("lines")  , true)) element_chart->SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE_LINES);
      if(!charttypestr.Compare(__L("columns"), true)) element_chart->SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE_COLUMNS);
      if(!charttypestr.Compare(__L("area")   , true)) element_chart->SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE_AREA);
      if(!charttypestr.Compare(__L("bars")   , true)) element_chart->SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE_BARS);
      if(!charttypestr.Compare(__L("pie")    , true)) element_chart->SetChartType(UI_ELEMENT_STATISTICSCHART_TYPE_PIE);
    }

  XSTRING titlestr;
  if(outstyle.Get(__L("title"), titlestr))
    {
      element_chart->SetTitle(titlestr.Get());
    }
   else if(!node->GetValue().IsEmpty())
    {
      element_chart->SetTitle(node->GetValue().Get());
    }

  element_chart->SetNeedsRebuild(true);

  GetLayoutElement_CalculateBoundaryLine(layout, element_chart);

  return element_chart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::CreatePartialLayout(XFILEXMLELEMENT* nodeelement, UI_LAYOUT* layout, UI_ELEMENT* father)
* @brief      Create partial layout
* @ingroup    USERINTERFACE
* 
* @param[in]  nodeelement : Nodeelement pointer to use.
* @param[in]  layout : Layout pointer to use.
* @param[in]  father : Father pointer to use.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::CreatePartialLayout(XFILEXMLELEMENT* nodeelement, UI_LAYOUT* layout, UI_ELEMENT* father)
{
  XSTRING       value;
  UI_ELEMENT*   element  = NULL;

  if(GetLayoutElementValue(nodeelement, __L("type"), value))
    {                              
      if(!value.Compare(__L("text")           , true))  
        {
          element = GetLayoutElement_Text(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("textbox")        , true))  
        {
          element = GetLayoutElement_TextBox(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("image")          , true))  
        {
          element = GetLayoutElement_Image(nodeelement, layout, father);
        }

      if(!value.Compare(__L("animation")      , true))  
        {
          element = GetLayoutElement_Animation(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("option")         , true))  
        {
          element = GetLayoutElement_Option(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("multioption")    , true))  
        {
          element = GetLayoutElement_MultiOption(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("button")         , true))  
        { 
          element = GetLayoutElement_Button(nodeelement, layout, father); 
        }

      if(!value.Compare(__L("checkbox")       , true))  
        {
          element = GetLayoutElement_CheckBox(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("edittext")       , true))  
        {
          element = GetLayoutElement_EditText(nodeelement, layout, father);          
        }

      if(!value.Compare(__L("form")           , true))  
        {
          element = GetLayoutElement_Form(nodeelement, layout, father);
        }

      if(!value.Compare(__L("menu")           , true))  
        {
          element = GetLayoutElement_Menu(nodeelement, layout, father);      
        }

      if(!value.Compare(__L("listbox")        , true))  
        {
          element = GetLayoutElement_ListBox(nodeelement, layout, father);      
        }

      if(!value.Compare(__L("progressbar")    , true))  
        {
          element = GetLayoutElement_ProgressBar(nodeelement, layout, father);      
        }
        
      if(!value.Compare(__L("progressradial") , true))
        {
          element = GetLayoutElement_ProgressRadial(nodeelement, layout, father);
        }
  
      if(!value.Compare(__L("progressimage")     , true))
        {
          element = GetLayoutElement_ProgressImage(nodeelement, layout, father);
        }

      if(!value.Compare(__L("statisticschart")   , true))
        {
          element = GetLayoutElement_StatisticsChart(nodeelement, layout, father);
        }

    }

  return element;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::CreateLayouts(XFILEXML& xml, XPATH& xmlpathfile, GRPSCREEN* screen, int viewportindex)
* @brief      Create layouts
* @ingroup    USERINTERFACE
* 
* @param[in]  xml : XML value.
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* @param[in]  xmlpathfile : Xmlpathfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::CreateLayouts(XFILEXML& xml, XPATH& xmlpathfile, GRPSCREEN* screen, int viewportindex)
{
  if(!xml.ReadAndDecodeAllLines())
    {
      return false;
    }

  XFILEXMLELEMENT*  root          = xml.GetRoot();
  bool              skinowned     = false;   // P0.3 ownership guard: true once some UI_LAYOUT below has claimed
                                              // "ui_skin" (created once for the whole root, just below) as its
                                              // own -- see UI_LAYOUT::SetOwnsSkin(). Every <layout> node under
                                              // THIS root shares that one UI_SKIN*; only the first one actually
                                              // constructed may delete it.
  XSTRING           nametypeskin;
  UI_SKIN_DRAWMODE  drawmode      = UI_SKIN_DRAWMODE_UNKNOWN;
  XSTRING           raster_fontname;
  XSTRING           vector_fontname;
  XSTRING           background_color[2];
  XSTRING           background_namefile[2];
  XSTRING           background_seamlesspattern[2];
  XSTRING           background_patternwidth[2];
  XSTRING           background_patternheight[2];

  if(!root)
    {
      return false;
    }

  // --- Optional <stylesheet>file.css</stylesheet> declaration ---------------------------------------------------
  // Discover a single (first-hit) stylesheet node under the XML root and resolve its filename against the XML's
  // own directory. Phase 1 ownership step: the stylesheet itself is now owned per-UI_LAYOUT (UI_LAYOUT::
  // SetStyleSheet(), see below) rather than living as a single UI_MANAGER-wide pointer -- so only the PATH is
  // resolved here; the actual parse (one fresh UI_STYLESHEET instance per <layout> this XML root defines, the
  // overwhelming common case being exactly one) happens per-layout further down, guaranteeing each UI_LAYOUT
  // owns and deletes its own instance with no shared pointer that a second, unrelated layout's load could ever
  // silently repoint or free out from under a still-visible screen.
  XPATH stylesheet_csspath;
  bool  has_stylesheet_path = false;

  for(int c=0; c<root->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodess = root->GetElement(c);
      if(nodess && !nodess->GetName().Compare(__L("stylesheet"), true))
        {
          XSTRING cssname = nodess->GetValue();
          cssname.DeleteNoCharacters(__L(" \t\r\n"));

          if(!cssname.IsEmpty())
            {
              XSTRING drive;
              XPATH   dir;

              xmlpathfile.GetDrive(drive);
              xmlpathfile.GetPath (dir);

              stylesheet_csspath  = drive;
              stylesheet_csspath += dir;
              stylesheet_csspath += cssname;

              has_stylesheet_path = true;
            }

          break;   // first-hit wins; ignore any additional <stylesheet> nodes at the root
        }
    }

  for(int c=0; c<root->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodeskin = root->GetElement(c);
      if(nodeskin)
        {
          if(!nodeskin->GetName().Compare(__L("skin"), true))
            {                 
              XCHAR*    value;                             
              XSTRING   drawmodestr;
                 
              value = nodeskin->GetValueAttribute(__L("name"));
              if(value) nametypeskin = value;

              drawmodestr = nodeskin->GetValueAttribute(__L("drawmode"));  
              if(!drawmodestr.Compare(__L("canvas"), true))   drawmode = UI_SKIN_DRAWMODE_CANVAS;
              if(!drawmodestr.Compare(__L("context"), true))  drawmode = UI_SKIN_DRAWMODE_CONTEXT;                  

              value = nodeskin->GetValueAttribute(__L("rasterfont"));                          
              if(value) raster_fontname = value;
                                          
              value = nodeskin->GetValueAttribute(__L("vectorfont"));                          
              if(value) vector_fontname = value;    

              value = nodeskin->GetValueAttribute(__L("backgroundcolor"));                          
              if(value) background_color[1] = value;                                       

              value = nodeskin->GetValueAttribute(__L("backgroundimg"));                          
              if(value) background_namefile[1] = value;                                       

              value = nodeskin->GetValueAttribute(__L("backgroundseamlesspattern"));                          
              if(value) background_seamlesspattern[1] = value;                                       

              value = nodeskin->GetValueAttribute(__L("backgroundpatternwidth"));                          
              if(value) background_patternwidth[1] = value;                                       

              value = nodeskin->GetValueAttribute(__L("backgroundpatternheight"));                          
              if(value) background_patternheight[1] = value;                                       
            }                  
        }
    }

  UI_SKIN* ui_skin = Skin_Create(nametypeskin, drawmode, screen, viewportindex);
  if(!ui_skin) 
    {
      return false;
    }
       
  ui_skin->GetRasterFont()->Set(raster_fontname);
  ui_skin->GetVectorFont()->Set(vector_fontname);

  if(!ui_skin->LoadFonts()) 
    {
      return false;
    }

  GRPPROPERTYMODE grppropertymode      = GRPPROPERTYMODE_XX_UNKNOWN;
  GRP2DCANVAS*    backgroundcanvas     = NULL;
  double          backgroundwidth      = 0.0;
  double          backgroundheight     = 0.0;

  for(int c=0; c<root->GetNElements(); c++)
    {
      XFILEXMLELEMENT*  nodecacheelement = root->GetElement(c);
      if(nodecacheelement)
        {          
          if(ui_skin)
            {
              switch(drawmode)
                {
                  case UI_SKIN_DRAWMODE_UNKNOWN   : break;

                  case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)ui_skin;
                                                      if(skincanvas)
                                                        {
                                                          grppropertymode = skincanvas->GetCanvas()->GetMode();
                                                        }
                                                    }
                                                    break;
   
                  case UI_SKIN_DRAWMODE_CONTEXT   : break;
                }  
            }

          CreateCacheElements(nodecacheelement, drawmode, grppropertymode);
        }
    }

  // Vector (SVG, DXF...) backgrounds need a canvas to rasterize into and a target size to rasterize at, since,
  // unlike a raster bitmap file, they have no pixel size of their own. Backgrounds are always rasterized to fit
  // the destination screen, so the reference canvas/size are resolved once here, before the layouts loop below.
  if(ui_skin && screen)
    {
      switch(drawmode)
        {
          case UI_SKIN_DRAWMODE_UNKNOWN   : break;

          case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)ui_skin;
                                              if(skincanvas)
                                                {
                                                  backgroundcanvas = skincanvas->GetCanvas();
                                                  backgroundwidth  = (double)screen->GetWidth();
                                                  backgroundheight = (double)screen->GetHeight();
                                                }
                                            }
                                            break;

          case UI_SKIN_DRAWMODE_CONTEXT   : break;
        }
    }

  for(int c=0; c<root->GetNElements(); c++)
    {
      XFILEXMLELEMENT* nodelayout = root->GetElement(c);
      if(nodelayout)
        {
          if(!nodelayout->GetName().Compare(__L("layout"), true))
            {
              XSTRING   namelayout;                
              XCHAR*    value;                             

              value = nodelayout->GetValueAttribute(__L("name"));
              if(value) namelayout = value;         
              if(!namelayout.IsEmpty())
                {                      
                  value = nodelayout->GetValueAttribute(__L("backgroundcolor"));                          
                  if(value) background_color[0] = value;                                       

                  value = nodelayout->GetValueAttribute(__L("backgroundimg"));                          
                  if(value) background_namefile[0] = value;                  

                  value = nodelayout->GetValueAttribute(__L("backgroundseamlesspattern"));                          
                  if(value) background_seamlesspattern[0] = value;                  

                  value = nodelayout->GetValueAttribute(__L("backgroundpatternwidth"));                          
                  if(value) background_patternwidth[0] = value;                  

                  value = nodelayout->GetValueAttribute(__L("backgroundpatternheight"));                          
                  if(value) background_patternheight[0] = value;                  

                  UI_LAYOUT* layout = GEN_NEW UI_LAYOUT(ui_skin);
                  if(layout)
                    {
                      // P0.3 ownership guard (see the "skinowned" declaration above): the first UI_LAYOUT built
                      // for this root keeps the default (owning) behaviour set by its constructor; every
                      // subsequent one sharing the same "ui_skin" must NOT also delete it.
                      if(skinowned) layout->SetOwnsSkin(false);
                      else          skinowned = true;

                      XSTRING  bckcolor;
                      XSTRING  bcknamefile;

                      layout->GetNameID()->Set(namelayout);
                      layout->GetBackground()->GetColor()->SetFromString(background_color[1]);
                      layout->GetBackground()->GetBitmapFileName()->Set(background_namefile[1]);

                      // Track B / UIScale: optional authoring canvas from XML so @media and rem/vw resolve
                      // against design px before elements are built (UI_System used to set this only after Load).
                      XDWORD xml_design_w = 0;
                      XDWORD xml_design_h = 0;
                      {
                        XCHAR* dw = nodelayout->GetValueAttribute(__L("designwidth"));
                        XCHAR* dh = nodelayout->GetValueAttribute(__L("designheight"));
                        if(dw) { XSTRING s(dw); xml_design_w = (XDWORD)s.ConvertToInt(); }
                        if(dh) { XSTRING s(dh); xml_design_h = (XDWORD)s.ConvertToInt(); }
                      }
                      if(xml_design_w && xml_design_h)
                        {
                          layout->SetDesignSize(xml_design_w, xml_design_h);
                        }
                       else if(screen)
                        {
                          layout->SetDesignSize(screen->GetWidth(), screen->GetHeight());
                        }

                      // Phase 1 ownership step: parse a FRESH UI_STYLESHEET instance for THIS layout (instead of
                      // one UI_MANAGER-wide instance shared by every <layout> this XML root defines, which is
                      // the overwhelming common case anyway -- one root, one layout). Any I/O or parse issue is
                      // logged and skipped: an absent or malformed stylesheet must never abort layout
                      // construction, exactly like before.
                      if(has_stylesheet_path)
                        {
                          UI_STYLESHEET* sheet = GEN_NEW UI_STYLESHEET();
                          if(sheet)
                            {
                              UI_CSSPARSER parser;
                              if(parser.ParseFile(stylesheet_csspath, *sheet) && sheet->Rules_Count() > 0)
                                {
                                  layout->SetStyleSheet(sheet);
                                  // UIScale opt-in via stylesheet: seed design canvas if XML did not already.
                                  if(!xml_design_w || !xml_design_h)
                                    {
                                      if(screen)
                                        {
                                          layout->SetDesignSize(screen->GetWidth(), screen->GetHeight());
                                          layout->SetUIScale(UI_LAYOUT_UISCALE_DEFAULT);
                                        }
                                    }
                                   else
                                    {
                                      layout->SetUIScale(UI_LAYOUT_UISCALE_DEFAULT);
                                    }

                                  // Track B: gate @media against design viewport (load-time; see L.3).
                                  sheet->SetMediaViewport((int)layout->GetDesignWidth(), (int)layout->GetDesignHeight());

                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[UI Load] stylesheet [%s] loaded (%d rules) for layout [%s] media=%dx%d"), stylesheet_csspath.Get(), sheet->Rules_Count(), layout->GetNameID()->Get(), layout->GetDesignWidth(), layout->GetDesignHeight());
                                }
                               else
                                {
                                  GEN_DELETE sheet;
                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, __L("[UI Load] stylesheet [%s] not applied (missing or empty)"), stylesheet_csspath.Get());
                                }
                            }
                        }
                      
                      if(!background_color[0].IsEmpty())
                        {
                          bckcolor = background_color[0];   
                        }
                       else
                        {
                          if(!background_color[1].IsEmpty())
                            {
                              bckcolor = background_color[1];   
                            }
                        }
                        
                      if(!background_namefile[0].IsEmpty())
                        {
                          bcknamefile = background_namefile[0];   
                        }
                       else
                        {
                          if(!background_namefile[1].IsEmpty())
                            {
                              bcknamefile = background_namefile[1];   
                            }
                        }  

                      XSTRING  bckseamlesspatternstr;
                      XSTRING  bckpatternwidthstr;
                      XSTRING  bckpatternheightstr;

                      if(!background_seamlesspattern[0].IsEmpty())
                        {
                          bckseamlesspatternstr = background_seamlesspattern[0];
                        }
                       else
                        {
                          if(!background_seamlesspattern[1].IsEmpty())
                            {
                              bckseamlesspatternstr = background_seamlesspattern[1];
                            }
                        }

                      if(!background_patternwidth[0].IsEmpty())
                        {
                          bckpatternwidthstr = background_patternwidth[0];
                        }
                       else
                        {
                          if(!background_patternwidth[1].IsEmpty())
                            {
                              bckpatternwidthstr = background_patternwidth[1];
                            }
                        }

                      if(!background_patternheight[0].IsEmpty())
                        {
                          bckpatternheightstr = background_patternheight[0];
                        }
                       else
                        {
                          if(!background_patternheight[1].IsEmpty())
                            {
                              bckpatternheightstr = background_patternheight[1];
                            }
                        }

                      bool isseamlesspattern = false;

                      if(!bckseamlesspatternstr.Compare(__L("yes"),  true) ||
                         !bckseamlesspatternstr.Compare(__L("true"), true) ||
                         !bckseamlesspatternstr.Compare(__L("1"),    true))   isseamlesspattern = true;

                      // "backgroundimg" is always the same single resource, whether it ends up being drawn once,
                      // stretched to the layout (GetBitmap(), the default) or repeated as a tile (GetPatternBitmap(),
                      // when "backgroundseamlesspattern" resolves to true) - see UI_MANAGER::Layout_PutBackground().
                      // A layout never has both set: only one of the two branches below runs.
                      if(isseamlesspattern)
                        {
                          // Raster pattern files (bmp, jpg, png...) are loaded at their own natural pixel size -
                          // width/height are only used by LoadBackgroundBitmap() for the vector (SVG, DXF...) case,
                          // via "backgroundpatternwidth"/"backgroundpatternheight", to know the tile size to
                          // rasterize at, since a vector file has no pixel size of its own.
                          GRPBITMAP* pattern = LoadBackgroundBitmap(bcknamefile, grppropertymode, backgroundcanvas, bckpatternwidthstr.ConvertToDouble(), bckpatternheightstr.ConvertToDouble());
                          if(pattern)
                            {
                              layout->GetBackground()->SetPatternBitmap(pattern);
                            }
                        }
                       else
                        {
                          GRPBITMAP* background = LoadBackgroundBitmap(bcknamefile, grppropertymode, backgroundcanvas, backgroundwidth, backgroundheight);
                          if(background)
                            {
                              layout->GetBackground()->SetBitmap(background);          
                            }
                        }

                      if(!layout->GetNameID()->Compare(UI_MANAGER_LAYOUT_COMMON, true))
                        {
                          layout_commonindex = layouts.GetSize();
                        }

                      Layouts_Add(layout); 
                    
                      for(int d=0 ; d<nodelayout->GetNElements(); d++)
                        {
                          XFILEXMLELEMENT* nodeelement =  nodelayout->GetElement(d);
                          if(nodeelement)
                            {
                              UI_ELEMENT* element = CreatePartialLayout(nodeelement, layout, NULL);
                              if(element)
                                {
                                  layout->Elements_Add(element);

                                  // Post-load hook for step 3 (state-based CSS): snapshot the load-time visual
                                  // baseline of every element in the freshly-built subtree and mark those that
                                  // are targeted by pseudo-carrying rules so their state setters know they must
                                  // trigger a re-resolve. Cheap walk, runs once per layout.
                                  PrepareElementStyleState(element);

                                  // Post-load hook, Phase 4 ("migración del ejemplo", first sub-step -- see
                                  // UI_LayoutEngine.h's RunLayout()/BuildTree() SCOPE ADDENDUM): "element"'s own
                                  // subtree (every nested <element> child, built recursively by
                                  // CreatePartialLayout() above before this point) is already fully resolved by
                                  // the existing legacy XML pipeline, so it is safe to hand the whole thing to
                                  // the new CSS engine here. Deliberately unconditional -- see the SCOPE ADDENDUM
                                  // for why this is behaviour-preserving for every layout that does not actually
                                  // use "display: flex".
                                  UI_LAYOUTENGINE::RunLayout(element, UI_LAYOUTSTRATEGY_CSS);

                                  // Post-load hook, Phase 4 ("migración del ejemplo", fifth sub-step follow-up):
                                  // re-sync any ProgressBar's "track" sub-element to its now flex-resolved
                                  // position -- see RefreshFlexProgressBarTracks()'s own banner for the bug this
                                  // fixes ("ram_linear_bar" drawing nothing after becoming a flex item).
                                  RefreshFlexProgressBarTracks(element, layout->GetSkin());
                                }
                            }
                        }
                    }  
                }
            }                  
        }
    }
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_MANAGER::LoadBackgroundBitmap(XSTRING& namefilebitmap, GRPPROPERTYMODE mode, GRP2DCANVAS* referencecanvas, double width, double height)
* @brief      Load background bitmap
* @note       Accepts both raster (bmp, jpg, png...) and vector (SVG, DXF...) background files, detected by their
*             file extension via IsVectorResource(). Raster files follow the original GRPBITMAPFILE path, unchanged.
*             Vector files are rasterized once, at load time, into a bitmap that already fits (width, height) -
*             typically the destination screen size - through LoadBackgroundVectorFileToBitmap(), so the rest of the
*             background pipeline (UI_BACKGROUND::SetBitmap()/GetBitmap(), Layout_PutBackgroundImage()) keeps
*             working exactly as before, unaware of whether the original resource was raster or vector.
*             referencecanvas/width/height are only required for vector resources; raster loading ignores them,
*             so existing callers that do not pass them keep their original behavior.
* @ingroup    USERINTERFACE
* 
* @param[in]  namefilebitmap : Namefilebitmap value.
* @param[in]  mode : Mode value.
* @param[in]  referencecanvas : Canvas to rasterize a vector resource with (NULL = vector resources are skipped).
* @param[in]  width : Target width, in pixels, to rasterize a vector resource at (ignored for raster files).
* @param[in]  height : Target height, in pixels, to rasterize a vector resource at (ignored for raster files).
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_MANAGER::LoadBackgroundBitmap(XSTRING& namefilebitmap, GRPPROPERTYMODE mode, GRP2DCANVAS* referencecanvas, double width, double height)
{  
  if(IsVectorResource(namefilebitmap.Get()))
    {
      return LoadBackgroundVectorFileToBitmap(namefilebitmap, referencecanvas, width, height);
    }

  GRPBITMAPFILE*  bitmapfile;
  GRPBITMAP*      bitmap  = NULL;  
  bool            status  = false; 

  bitmapfile = GEN_NEW GRPBITMAPFILE();
  if(!bitmapfile) 
    {
      return NULL;
    }               

   if(iszippedfile && unzipfile)
    {
      if(unzipfile)
        {                
          XPATH pathnamefilecmp;

          pathnamefilecmp = APPFLOW_DEFAULT_DIRECTORY_GRAPHICS;
          pathnamefilecmp.Slash_Add();
          pathnamefilecmp += namefilebitmap;
 
          status = unzipfile->DecompressFile(pathnamefilecmp, unzippathfile, namefilebitmap.Get());   
          if(status)
            {  
              XPATH unzippathfile_tmp;

              unzippathfile_tmp  = unzippathfile.Get();
              unzippathfile_tmp += namefilebitmap;          

              bitmap = bitmapfile->Load(unzippathfile_tmp, mode);
    
              DeleteTemporalUnZipFile(unzippathfile_tmp);  
            }
        }
    }
   else
    { 
      XPATH xpath;

      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpath);
      xpath.Slash_Add();
      xpath.Add(namefilebitmap.Get());                                             

      bitmap = bitmapfile->Load(xpath, mode);    
    }

  GEN_DELETE bitmapfile;

  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_MANAGER::LoadBackgroundVectorFileToBitmap(XSTRING& namefilevector, GRP2DCANVAS* referencecanvas, double width, double height)
* @brief      Load background vector bitmap
* @note       Rasterizes a vector background file (SVG, DXF...) into a single opaque-sized bitmap that already
*             fits (width, height), following the same offscreen-canvas render approach as
*             UI_ANIMATION::LoadFromFileVector(). Unlike a raster bitmap, a vector file has no pixel size of its
*             own, and a real GRPBITMAP::Scale() resample is not currently implemented; asking the vector renderer
*             to draw straight at the final target size avoids that limitation entirely, which is the reason
*             vector backgrounds are always rasterized already scaled to (width, height) at load time, rather
*             than loaded at some "native" size and rescaled later on each Layout_PutBackgroundImage() call.
* @ingroup    USERINTERFACE
* 
* @param[in]  namefilevector : Namefilevector value.
* @param[in]  referencecanvas : Canvas to copy the pixel format and the loaded vector font from.
* @param[in]  width : Target width, in pixels, to rasterize at.
* @param[in]  height : Target height, in pixels, to rasterize at.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_MANAGER::LoadBackgroundVectorFileToBitmap(XSTRING& namefilevector, GRP2DCANVAS* referencecanvas, double width, double height)
{
  if(!referencecanvas)                  return NULL;
  if((width <= 0.0) || (height <= 0.0)) return NULL;

  XPATH pathvector;
  bool  usedunzip = false;

  if(iszippedfile && unzipfile)
    {
      XPATH pathnamefilecmp;

      pathnamefilecmp = APPFLOW_DEFAULT_DIRECTORY_GRAPHICS;
      pathnamefilecmp.Slash_Add();
      pathnamefilecmp += namefilevector;

      if(!unzipfile->DecompressFile(pathnamefilecmp, unzippathfile, namefilevector.Get()))
        {
          return NULL;
        }

      pathvector  = unzippathfile.Get();
      pathvector += namefilevector;

      usedunzip = true;
    }
   else
    {
      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, pathvector);
      pathvector.Slash_Add();
      pathvector.Add(namefilevector.Get());
    }

  bool           status     = false;
  GRPBITMAP*     bitmap     = NULL;
  GRPVECTORFILE* vectorfile = GRPVECTORFILE::CreateInstance(pathvector);

  if(vectorfile)
    {
      if(vectorfile->Load() == GRPVECTORFILERESULT_OK)
        {
          GRPPROPERTIES properties;

          properties.CopyPropertysFrom(referencecanvas);
          properties.SetPosition(0, 0);
          properties.SetSize((XDWORD)width, (XDWORD)height);

          GRP2DCANVAS* offscreen = GEN_GRPFACTORY.CreateCanvas(&properties);
          if(offscreen)
            {
              offscreen->SetWidth(width);
              offscreen->SetHeight(height);

              if(offscreen->Buffer_Create())
                {
                  offscreen->VectorFont_CopyFrom(referencecanvas);          // SVG text needs the same loaded vector font as the real canvas

                  GRP2DCOLOR_RGBA8 opaque(0, 0, 0, 255);
                  offscreen->Clear(&opaque);                                // a background has no transparent holes to show through

                  GRP2DVECTORFILERENDERAGG vectorrender;

                  status = vectorrender.Render(vectorfile, offscreen, 0.0, 0.0, width, height);
                  if(status)
                    {
                      bitmap = offscreen->GetBitmap(0.0, 0.0, width, height);
                    }
                }

              GEN_GRPFACTORY.DeleteCanvas(offscreen);
            }
        }

      GEN_DELETE vectorfile;
    }

  if(usedunzip)
    {
      DeleteTemporalUnZipFile(pathvector);
    }

  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::CreateCacheElements(XFILEXMLELEMENT* nodeelement, UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, bool recursive)
* @brief      Create cache elements
* @ingroup    USERINTERFACE
* 
* @param[in]  nodeelement : Nodeelement pointer to use.
* @param[in]  drawmode : Drawmode value.
* @param[in]  grppropertymode : Grppropertymode value.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::CreateCacheElements(XFILEXMLELEMENT* nodeelement, UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, bool recursive)
{
  XSTRING name;          
  XCHAR*  value;  
  bool    status = false;

  if(!nodeelement->GetName().Compare(__L("color"), true))
    {                                                                  
      value = nodeelement->GetValueAttribute(__L("name"));
      if(value) 
        {
          name = value;
        }
              
      value = nodeelement->GetValue().Get();
      if(value) 
        {
          status = GEN_UI_COLORS.Add(name.Get(), value);                                                                       
        }
    }                  
    
  if(!nodeelement->GetName().Compare(__L("text"), true))
    {                                      
      value = nodeelement->GetValueAttribute(__L("name"));
      if(value) 
        {
          name = value;
        }
              
      value = nodeelement->GetValue().Get();
      if(value) 
        {
          status = GEN_UI_TEXTS.Add(name.Get(), value);                                                                       
        }
    }                      
  
  if(!nodeelement->GetName().Compare(__L("animation"), true))
    {                     
      value = nodeelement->GetValueAttribute(__L("name"));
      if(value)        
        { 
          name = value;
        }
              
      value = nodeelement->GetValue().Get();
      if(value) 
        {
          if(GetOrAddAnimationCache(drawmode, grppropertymode, name.Get(), value)) 
            {
              status = true;
            }
        }
    }

  if(recursive)
    {
      for(int c=0; c<nodeelement->GetNElements(); c++)
        {
          XFILEXMLELEMENT* subnodeelement = nodeelement->GetElement(c);
          if(subnodeelement) 
            {
              CreateCacheElements(subnodeelement, drawmode, grppropertymode, recursive);
            }
        }
    }
                  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::RegisterEvents(bool active)
* @brief      Register events
* @ingroup    USERINTERFACE
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::RegisterEvents(bool active)
{
  UI_XEVENT_TYPE eventtype[] = {  UI_XEVENT_TYPE_INPUT_CURSOR_MOVE            ,
                                  UI_XEVENT_TYPE_INPUT_CURSOR_MOTION          , 
                                  UI_XEVENT_TYPE_INPUT_SELECCTION             , 
                                  UI_XEVENT_TYPE_OUTPUT_SELECTED              ,
                                  UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE          ,
                                  UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS      
                               };

  for(int c=0; c<(sizeof(eventtype) / sizeof(UI_XEVENT_TYPE)); c++)
    {
      if(active)        
             RegisterEvent(eventtype[c]);              
       else  DeRegisterEvent(eventtype[c]);              
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_MANAGER::PreSelectElement(UI_ELEMENT* element, int x, int y)
* @brief      Pre select element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_MANAGER::PreSelectElement(UI_ELEMENT* element, int x, int y)
{
  UI_BOUNDARYLINE bline;
  bool            preselect         = false;
  bool            intofather        = false;
  UI_ELEMENT*     preselect_element = NULL;
  
  if(dynamic_cast<UI_PROPERTY_SELECTABLE*>(element))                                                                                                                                                                                                                                          
    {    
      if(element->IsActive() && element->IsVisible())
        {                                                                              
          bline.x       = element->GetXPositionWithScroll();          
          bline.y       = element->GetYPositionWithScroll();          
          bline.width   = element->GetBoundaryLine()->width;
          bline.height  = element->GetBoundaryLine()->height;

          // Fase 6: expand a copy for IsWithin only. Paint AABB (bline) stays for scroll-clip checks.
          UI_BOUNDARYLINE hit = bline;
          UI_LAYOUT* hitlayout = element->GetLayout();
          if(hitlayout && hitlayout->IsUIScaleActive())
            {
              double m = hitlayout->GetMinHitSize();
              if(m > 0.0) hit.ExpandCenteredToMin(m, m);
            }
          
          preselect = hit.IsWithin(x, y);
          if(preselect)
            {
              // Phase 2: allow :selected:hover when a stylesheet drives state visuals. Without a stylesheet
              // (UI_Options XML-only), keep historical behaviour: selected items are not preselected.
              bool allow_selected_hover = false;
              if(element->GetLayout() && element->GetLayout()->GetStyleSheet() && element->GetStyleHasStateRules())
                {
                  allow_selected_hover = true;
                }

              if(element->IsSelected() && !allow_selected_hover)
                {
                  preselect = false;
                }
            }

          // A partially-hidden option (clipped by a scrollable ancestor's viewport) must NOT be selectable. Require the
          // option box to be fully inside the viewport on every axis the ancestor clips (a VISIBLE axis is never clipped,
          // so it is not restricted there).
          if(preselect)
            {
              UI_ELEMENT* anc = element->GetFather();
              while(anc)
                {
                  UI_PROPERTY_SCROLLEABLE* asc = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(anc);
                  if(asc && asc->Scroll_NeedClip())
                    {
                      double vp_w = 0.0f;
                      double vp_h = 0.0f;
                      UI_ELEMENT_FORM* af = dynamic_cast<UI_ELEMENT_FORM*>(anc);
                      if(af && af->GetVisibleRect()->width > 0 && af->GetVisibleRect()->height > 0)
                        { vp_w = af->GetVisibleRect()->width;     vp_h = af->GetVisibleRect()->height; }
                       else
                        { vp_w = anc->GetBoundaryLine()->width;   vp_h = anc->GetBoundaryLine()->height; }

                      double vp_x      = anc->GetXPositionWithScroll();
                      double vp_y      = anc->GetYPositionWithScroll();
                      double vp_left   = vp_x;
                      double vp_right  = vp_x + vp_w;
                      double vp_top    = vp_y - vp_h;
                      double vp_bottom = vp_y;

                      double o_left   = bline.x;
                      double o_right  = bline.x + bline.width;
                      double o_top    = bline.y - bline.height;
                      double o_bottom = bline.y;
                      double eps      = 1.0f;

                      bool clip_h = (asc->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) != UI_OVERFLOW_VISIBLE);
                      bool clip_v = (asc->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL)   != UI_OVERFLOW_VISIBLE);

                      double o_w = o_right  - o_left;
                      double o_h = o_bottom - o_top;

                      // Reject only when the option COULD fit the viewport on that axis but is cut by the scroll edge
                      // (a partially scrolled-out item). If the option is larger than the viewport on that axis, the
                      // "cut" is just normal content overflow (e.g. text wider than the menu) and must stay selectable.
                      if(clip_h && (o_w <= (vp_w + eps)) && ((o_left < vp_left - eps) || (o_right  > vp_right  + eps))) preselect = false;
                      if(clip_v && (o_h <= (vp_h + eps)) && ((o_top  < vp_top  - eps) || (o_bottom > vp_bottom + eps))) preselect = false;

                      break;   // only the nearest clipping scrollable ancestor defines the visible viewport
                    }
                  anc = anc->GetFather();
                }
            }

          bool preselectchanged = element->SetPreSelect(preselect);
          if(preselect)
            {
              // P1.2 fix: only invalidate on the false->true transition. While the pointer keeps moving inside
              // an already-preselected element (the common case on every mouse-move tick spent hovering one
              // button), re-marking it -- and, per Elements_SetToRedraw()'s own scrolleable-ancestor handling,
              // potentially a whole scrollable container -- dirty on EVERY tick produced a needless repaint
              // storm, especially costly with box-shadow/alpha cards, and was flagged as amplifying the
              // section-change blank-flash bug under investigation (see Informe_tecnico_GEN_UI_CSS_video.md).
              if(preselectchanged) Elements_SetToRedraw(element);

              last_xposition = x;
              last_yposition = y;

              preselect_element = element;
            }
        }
    } 

  if(!preselect)
    {
      bline.x       = element->GetXPosition();
      bline.y       = element->GetYPosition();
      bline.width   = element->GetBoundaryLine()->width;
      bline.height  = element->GetBoundaryLine()->height;
          
      intofather = bline.IsWithin(x, y);     
    }

  if(intofather)
    {
      if(element->GetComposeElements()->GetSize())
        {
          // P1.8 fix: this used to "if(found) break;" as soon as the hovered sibling turned up, which meant
          // every OTHER selectable sibling declared AFTER it in this same container was skipped entirely for
          // the tick -- including the SetPreSelect(false) that Update()/ReapplyStyleVisual() rely on to reverse
          // an earlier hover (and its CSS "transition:" tween, see dashboard.css). A sibling hovered on a
          // previous tick and left behind by a non-monotonic mouse path (e.g. sweeping down past it, then back
          // up to a row above it) would never be told the pointer left, so it stayed lit -- exactly the "stuck
          // highlighted nav row" symptom reported after the video capture. Visiting every sibling every tick is
          // cheap (a boundary-box compare; SetPreSelect() itself only touches style on an actual change), so
          // there is no reason to short-circuit here at all.
          for(XDWORD d=0; d<element->GetComposeElements()->GetSize(); d++)
            {
              UI_ELEMENT* subelement = element->GetComposeElements()->Get(d);
              if(subelement)
                {
                  UI_ELEMENT* found = PreSelectElement(subelement, x, y);
                  if(found) preselect_element = found;
                }
            }
        }
    }

  return preselect_element;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SelectElement(UI_ELEMENT* element)
* @brief      Select element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SelectElement(UI_ELEMENT* element)
{
  if(dynamic_cast<UI_PROPERTY_SELECTABLE*>(element))                                                                                                                                                                                                                                          
    {    
      if(SelectedElement(element)) 
        {
          return true;      
        }
    } 
                                                                                                                                     
  if(element->GetComposeElements()->GetSize())
    {                                                                      
      for(XDWORD d=0; d<element->GetComposeElements()->GetSize(); d++)   
        {
          UI_ELEMENT* subelement = element->GetComposeElements()->Get(d);
          if(subelement) 
            { 
              if(dynamic_cast<UI_PROPERTY_SELECTABLE*>(subelement))                                                                                                                                                                                                                                          
                {                  
                  if(SelectedElement(subelement)) 
                    {
                      return true;                  
                    }
                }
            }
        }
    }                             

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SelectedElement(UI_ELEMENT* element)
* @brief      Selected element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SelectedElement(UI_ELEMENT* element)
{
  if(!element) return false;

  if(!element->IsPreSelect()) return false;

  UI_PROPERTY_SELECTABLE* element_selectable = dynamic_cast<UI_PROPERTY_SELECTABLE*>(element);
  if(!element_selectable) return false;

  // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("UI Element [%s]: Selected. "), element->GetName()->Get());

  if(dynamic_cast<UI_PROPERTY_EDITABLE*>(element))        
    {
      if(virtualkeyboard)
        {
          // Do not reopen the keyboard when the user taps the in-keyboard input edit itself.
          if(!(virtualkeyboard->IsShow() && virtualkeyboard->IsOwnElement(element)))
            {
              virtualkeyboard->Show(true, element);
            }
        }
    }

  if(element->GetType() == UI_ELEMENT_TYPE_CHECKBOX)
    {
      UI_ELEMENT_CHECKBOX* element_checkbox = (UI_ELEMENT_CHECKBOX*)element;
      if(element_checkbox) 
        {
          bool status = element_checkbox->GetStatus();

          // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Checkbox select] name %s status: %s"), element_checkbox->GetName()->Get(), (status?__L("true"):__L("false"))); 
          
          status=!status;

          element_checkbox->SetStatus(status);
          
          // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Checkbox select] name %s status: %s"), element_checkbox->GetName()->Get(), (status?__L("true"):__L("false"))); 

          Elements_SetToRedraw(element);  
      }
    }
   else
    {
      if(virtualkeyboard)
        {
          if(virtualkeyboard->SelectInput(element)) 
            {
              return true;
            }
        }

      if(element->GetType() == UI_ELEMENT_TYPE_LISTBOX)
        {
          UI_ELEMENT_LISTBOX* element_listbox = (UI_ELEMENT_LISTBOX*)element;
          if(element_listbox)
            { 
              UI_ELEMENT_MENU* element_menu = element_listbox->Get_UIMenu();     
              if(element_menu)
                {           
                  Element_SetModal(element_menu);  
       
                  element_menu->SetVisible(!element_menu->IsVisible());     
                }
            }
        }
      
      element->SetPreSelect(false);     
      element->SetSelected(true);

      element_selectable->GetXTimerSelected()->Reset();
                      
      SendEvent(UI_XEVENT_TYPE_OUTPUT_SELECTED, element); 

      //element->SetSelected(false);
      Elements_SetToRedraw(element);                   
    }

  return true;
}                       


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::UnSelectedElement(UI_ELEMENT* element)
* @brief      Un selected element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UnSelectedElement(UI_ELEMENT* element)
{
  if(element->IsSelected()) 
    {
      UI_PROPERTY_SELECTABLE* element_selectable = dynamic_cast<UI_PROPERTY_SELECTABLE*>(element);
      if(!element_selectable) 
        {
          return false;
        }
                        
      if(element_selectable->GetXTimerSelected())
        {
          if(element_selectable->GetXTimerSelected()->GetMeasureMilliSeconds() > element_selectable->GetTimeSelected()) 
            {             
              element->SetSelected(false);
              Elements_SetToRedraw(element);     

              GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOVE, last_xposition, last_yposition);   
            }
        }                 
    }
                                                                                                                                     
  if(element->GetComposeElements()->GetSize())
    {                                                                      
      for(XDWORD d=0; d<element->GetComposeElements()->GetSize(); d++)   
        {
          UI_ELEMENT* subelement = element->GetComposeElements()->Get(d);
          if(subelement) 
            { 
              if(dynamic_cast<UI_PROPERTY_SELECTABLE*>(subelement))                                                                                                                                                                                                                                          
                {                  
                  if(UnSelectedElement(subelement)) 
                    {
                      return true;                  
                    }
                }
            }
        }
    }                             

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::UnSelectedElement()
* @brief      Un selected element
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UnSelectedElement()
{
  for(XDWORD d=0; d<layouts.GetSize(); d++)
    {    
      UI_LAYOUT* layout = layouts.Get(d);                                                          
      if(layout)
        {
          for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
            {
              UI_ELEMENT* element = layout->Elements_Get()->Get(c);
              if(element) 
                {
                  UnSelectedElement(element);                        
                }
            }        
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::MapScreenToDesign(UI_LAYOUT* layout, int screen_x, int screen_y, int& design_x, int& design_y)
* @brief      Map pointer coordinates from screen px to the layout's design px before hit-test.
* @note       XML-only layouts (UIScale inactive) keep identity. AABB / IsWithin stay in design space.
* @ingroup    USERINTERFACE
*
* @param[in]  layout   : Layout that owns the elements being tested (may be NULL).
* @param[in]  screen_x : Pointer X in framebuffer px.
* @param[in]  screen_y : Pointer Y in framebuffer px.
* @param[out] design_x : X in design px for IsWithin.
* @param[out] design_y : Y in design px for IsWithin.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::MapScreenToDesign(UI_LAYOUT* layout, int screen_x, int screen_y, int& design_x, int& design_y)
{
  if(layout && layout->IsUIScaleActive())
    {
      double dx = 0.0;
      double dy = 0.0;
      layout->ScreenToDesign((double)screen_x, (double)screen_y, dx, dy);

      design_x = (int)((dx >= 0.0) ? (dx + 0.5) : (dx - 0.5));
      design_y = (int)((dy >= 0.0) ? (dy + 0.5) : (dy - 0.5));
      return;
    }

  design_x = screen_x;
  design_y = screen_y;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::UIScale_EnsureDesignCanvas(UI_LAYOUT* layout)
* @brief      Create/resize the layout's design offscreen canvas to designWidth x designHeight.
* @ingroup    USERINTERFACE
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UIScale_EnsureDesignCanvas(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();
  GRP2DCANVAS*   live        = NULL;

  if(!screen || !screen->GetViewport(0)) return false;
  live = screen->GetViewport(0)->GetCanvas();
  if(!live) return false;

  XDWORD dw = layout->GetDesignWidth();
  XDWORD dh = layout->GetDesignHeight();
  if(!dw) dw = screen->GetWidth();
  if(!dh) dh = screen->GetHeight();
  if(!dw || !dh) return false;

  GRP2DCANVAS* existing = layout->GetDesignCanvas();
  if(existing && existing->GetWidth() == (double)dw && existing->GetHeight() == (double)dh)
    {
      return true;
    }

  GRPPROPERTIES properties;
  properties.CopyPropertysFrom(live);
  properties.SetPosition(0, 0);
  properties.SetSize(dw, dh);

  GRP2DCANVAS* design = GEN_GRPFACTORY.CreateCanvas(&properties);
  if(!design) return false;

  design->SetWidth((double)dw);
  design->SetHeight((double)dh);

  if(!design->Buffer_Create())
    {
      GEN_GRPFACTORY.DeleteCanvas(design);
      return false;
    }

  design->VectorFont_CopyFrom(live);
  layout->SetDesignCanvas(design);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::UIScale_BeginFrame(UI_LAYOUT* layout)
* @brief      Redirect skin paint/rebuild to the design canvas when scaled present is required.
* @ingroup    USERINTERFACE
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UIScale_BeginFrame(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();
  if(!screen) return false;

  if(!layout->NeedsScaledPresent(screen->GetWidth(), screen->GetHeight()))
    {
      layout->ComputePresentTransform(screen->GetWidth(), screen->GetHeight()); // offsets ~0 at scale 1
      return false;
    }

  if(!UIScale_EnsureDesignCanvas(layout)) return false;

  layout->ComputePresentTransform(screen->GetWidth(), screen->GetHeight());

  GRP2DCANVAS* design = layout->GetDesignCanvas();
  if(!design) return false;

  skin_canvas->SetCanvasOverride(design);
  skin_canvas->SetTargetCanvas(design);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::UIScale_Present(UI_LAYOUT* layout)
* @brief      Blit design canvas → viewport with uiScale + letterbox offsets; fill letterbox bars.
* @ingroup    USERINTERFACE
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UIScale_Present(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();
  GRP2DCANVAS*   design      = layout->GetDesignCanvas();
  if(!screen || !design) return false;

  if(!layout->NeedsScaledPresent(screen->GetWidth(), screen->GetHeight())) return false;

  // Live viewport canvas (clear override temporarily for GetCanvas / blit target).
  skin_canvas->SetCanvasOverride(NULL);
  GRP2DCANVAS* live = skin_canvas->GetCanvas();
  if(!live)
    {
      skin_canvas->SetCanvasOverride(design);
      return false;
    }

  XDWORD dw = layout->GetDesignWidth();
  XDWORD dh = layout->GetDesignHeight();
  if(!dw) dw = (XDWORD)design->GetWidth();
  if(!dh) dh = (XDWORD)design->GetHeight();

  XDWORD sw = screen->GetWidth();
  XDWORD sh = screen->GetHeight();

  // Always refresh letterbox offsets from the live framebuffer size (avoids stale offsets after
  // maximize/resize transitions that left asymmetric black gutters in captures).
  layout->ComputePresentTransform(sw, sh);

  double s = layout->GetUIScale();
  if(s < UI_LAYOUT_UISCALE_MIN) s = UI_LAYOUT_UISCALE_MIN;

  // Only sample the design region that will be visible after scale (avoids huge bitmaps when zoom>1).
  double src_w = (double)dw;
  double src_h = (double)dh;
  if((dw * s) > (double)sw) src_w = ((double)sw) / s;
  if((dh * s) > (double)sh) src_h = ((double)sh) / s;
  if(src_w < 1.0) src_w = 1.0;
  if(src_h < 1.0) src_h = 1.0;
  if(src_w > (double)dw) src_w = (double)dw;
  if(src_h > (double)dh) src_h = (double)dh;

  GRPBITMAP* bmp = design->GetBitmap(0, 0, src_w, src_h);
  if(!bmp)
    {
      skin_canvas->SetCanvasOverride(design);
      return false;
    }

  int pw = (int)(src_w * s + 0.5);
  int ph = (int)(src_h * s + 0.5);
  if(pw < 1) pw = 1;
  if(ph < 1) ph = 1;
  if(pw > (int)sw) pw = (int)sw;
  if(ph > (int)sh) ph = (int)sh;

  bmp->Scale(pw, ph);

  // Clear live whenever the blit will not cover every pixel (letterbox/pillarbox OR partial zoom
  // region). Skipping Clear left ghosts of the previous scale/size after resize (Fase 5).
  // Chrome is painted AFTER content Present in Update(), so a Clear here is safe for that frame
  // only if chrome is dirtied — UIScale_ResetLiveComposition does that on transform changes.
  // Steady-state: Clear only when we leave uncovered margins.
  bool leaves_margins = (layout->GetUIScaleOffsetX() > 0.5) ||
                        (layout->GetUIScaleOffsetY() > 0.5) ||
                        (pw < (int)sw) ||
                        (ph < (int)sh);
  if(leaves_margins)
    {
      // Match layout page fill (not pure black) so letterbox matches the dashboard chrome.
      GRP2DCOLOR_RGBA8 letterbox(0, 0, 0, 255);
      if(layout->GetBackground() && layout->GetBackground()->GetColor() && layout->GetBackground()->GetColor()->IsValid())
        {
          UI_COLOR* bg = layout->GetBackground()->GetColor();
          letterbox = GRP2DCOLOR_RGBA8((XBYTE)bg->GetRed(), (XBYTE)bg->GetGreen(), (XBYTE)bg->GetBlue(), 255);
        }
      live->Clear(&letterbox);
    }

  live->PutBitmapNoAlpha(layout->GetUIScaleOffsetX(), layout->GetUIScaleOffsetY(), bmp);

  GEN_DELETE bmp;

  // Fase 7: sharp SVG/chart overlay on the live canvas (design paint stays untouched; no BoundaryLine mutation).
  UIScale_PresentSharpOverlay(layout, live);

  // Scaled present writes the full (or letterboxed) live canvas and wipes anything previously
  // composited there — including the custom-chrome caption painted AFTER content on the previous
  // frame. Chrome only redraws when dirty; without an explicit dirty here the bar vanishes on the
  // next tick after a resize (especially height-only / drag-from-top) and never returns.
  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  if(screen)
    {
      for(XDWORD c=0; c<layouts.GetSize(); c++)
        {
          UI_LAYOUT* other = layouts.Get(c);
          if(!other || !IsCFGChromesLayout(other)) continue;
          if(!other->GetSkin() || other->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;
          UI_SKINCANVAS* chrome_skin = (UI_SKINCANVAS*)other->GetSkin();
          if(chrome_skin->GetScreen() != screen) continue;
          other->Elements_SetToRedraw(true);
        }
    }
  #endif

  // Keep painting redirected to design for any remaining work this layout frame; caller EndFrame clears.
  skin_canvas->SetCanvasOverride(design);
  skin_canvas->SetTargetCanvas(design);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_EndFrame(UI_LAYOUT* layout)
* @brief      Restore skin canvas to the live viewport after scaled present.
* @ingroup    USERINTERFACE
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_EndFrame(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();

  skin_canvas->SetCanvasOverride(NULL);

  if(screen && screen->GetViewport(0) && screen->GetViewport(0)->GetCanvas())
    {
      skin_canvas->SetTargetCanvas(screen->GetViewport(0)->GetCanvas());
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::UIScale_PrepareLayout(UI_LAYOUT* layout)
* @brief      After changing uiScale at runtime: ensure design canvas, seed background, force full redraw.
* @ingroup    USERINTERFACE
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UIScale_PrepareLayout(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();
  if(!screen) return false;

  // Scale / window change: drop stale form/option/rebuild bitmaps and wipe live so the next
  // Present/identity paint cannot show a previous scale's ghost (resize reduce/enlarge).
  UIScale_ResetLiveComposition(layout);

  XCHAR* layoutname = (layout->GetNameID() && !layout->GetNameID()->IsEmpty()) ? layout->GetNameID()->Get() : NULL;

  if(!layout->NeedsScaledPresent(screen->GetWidth(), screen->GetHeight()))
    {
      layout->SetDesignCanvas(NULL);
      layout->ComputePresentTransform(screen->GetWidth(), screen->GetHeight());
      UIScale_EndFrame(layout);

      // Identity path paints on live — must re-seed the real background color/image after the
      // black Clear in ResetLiveComposition (otherwise sidebar/footer FormBackdrops capture black).
      if(layoutname) Layout_PutBackground(layoutname);

      UIScale_InvalidateSharpOverlays(layout);

      layout->Elements_SetToRedraw(true);
      return true;
    }

  if(!UIScale_EnsureDesignCanvas(layout)) return false;
  layout->ComputePresentTransform(screen->GetWidth(), screen->GetHeight());

  UIScale_BeginFrame(layout);

  if(layoutname) Layout_PutBackground(layoutname);

  UIScale_InvalidateSharpOverlays(layout);

  layout->Elements_SetToRedraw(true);
  UIScale_EndFrame(layout);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_PresentSharpOverlay(UI_LAYOUT* layout, GRP2DCANVAS* live)
* @brief      Fase 7: blit denser SVG icons + StatisticsCharts onto live after scaled Present.
* @note       Design paint / BoundaryLine / hit-test stay in design px. Overlay only when density ≠ 1.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_PresentSharpOverlay(UI_LAYOUT* layout, GRP2DCANVAS* live)
{
  if(!layout || !live) return;
  if(!layout->IsUIScaleActive()) return;

  double density = layout->GetAssetRasterScale();
  if(density > (1.0 - UI_LAYOUT_UISCALE_EPSILON) && density < (1.0 + UI_LAYOUT_UISCALE_EPSILON)) return;

  XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
  if(!roots) return;

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      UIScale_PresentSharpOverlay_Element(layout, roots->Get(c), live, density);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_PresentSharpOverlay_Element(UI_LAYOUT* layout, UI_ELEMENT* element, GRP2DCANVAS* live, double density)
* @brief      Fase 7: recursive sharp overlay for one element tree.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_PresentSharpOverlay_Element(UI_LAYOUT* layout, UI_ELEMENT* element, GRP2DCANVAS* live, double density)
{
  if(!layout || !element || !live) return;
  if(!element->IsVisible()) return;

  UI_SKIN* skin = layout->GetSkin();

  switch(element->GetType())
    {
      case UI_ELEMENT_TYPE_IMAGE :
        {
          UI_ELEMENT_IMAGE* image = (UI_ELEMENT_IMAGE*)element;
          if(image->GetResource() && !image->GetResource()->IsEmpty() && IsVectorResource(image->GetResource()->Get()))
            {
              double design_w = element->GetBoundaryLine()->width;
              double design_h = element->GetBoundaryLine()->height;
              if(design_w > 0.0 && design_h > 0.0)
                {
                  GRPPROPERTYMODE  grppropertymode = live->GetMode();
                  UI_SKIN_DRAWMODE drawmode        = UI_SKIN_DRAWMODE_CANVAS;
                  double           rw              = design_w * density;
                  double           rh              = design_h * density;

                  UI_ANIMATION* animation = GetOrAddAnimationCache(drawmode, grppropertymode, __L(""), image->GetResource()->Get(), live, rw, rh);
                  GRPBITMAP*    sharp     = animation ? animation->GetBitmap() : NULL;
                  if(sharp)
                    {
                      double x = element->GetXPosition();
                      double y = element->GetYPosition();
                      if(skin && element->GetFather()) skin->GetAddPositionScrollSteps(element->GetFather(), x, y);

                      double sx = 0.0;
                      double sy = 0.0;
                      // PutBitmap uses bottom-left of the bitmap in canvas Y-up-from-bottom convention:
                      // design put is (x, y - height). Map that top edge through DesignToScreen.
                      layout->DesignToScreen(x, y - design_h, sx, sy);
                      live->PutBitmapAlpha(sx, sy, sharp, image->GetAlpha());
                    }
                }
            }
        }
        break;

      case UI_ELEMENT_TYPE_STATISTICSCHART :
        {
          UI_ELEMENT_STATISTICSCHART* chart = (UI_ELEMENT_STATISTICSCHART*)element;
          if(chart->HasData())
            {
              if(chart->EnsureSharpBitmap(live, density) && chart->GetSharpBitmap())
                {
                  double design_w = element->GetBoundaryLine()->width;
                  double design_h = element->GetBoundaryLine()->height;
                  double x = element->GetXPosition();
                  double y = element->GetYPosition();
                  if(skin && element->GetFather()) skin->GetAddPositionScrollSteps(element->GetFather(), x, y);

                  double sx = 0.0;
                  double sy = 0.0;
                  layout->DesignToScreen(x, y - design_h, sx, sy);
                  live->PutBitmapAlpha(sx, sy, chart->GetSharpBitmap(), chart->GetAlpha());
                }
            }
        }
        break;

      default: break;
    }

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(children)
    {
      for(XDWORD c=0; c<children->GetSize(); c++)
        {
          UIScale_PresentSharpOverlay_Element(layout, children->Get(c), live, density);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_InvalidateSharpOverlays(UI_LAYOUT* layout)
* @brief      Fase 7: drop denser chart overlay caches after scale/window change.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_InvalidateSharpOverlays(UI_LAYOUT* layout)
{
  if(!layout) return;

  XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
  if(!roots) return;

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      UIScale_InvalidateSharpOverlays_Element(roots->Get(c));
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_InvalidateSharpOverlays_Element(UI_ELEMENT* element)
* @brief      Fase 7: recursive sharp-overlay invalidation.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_InvalidateSharpOverlays_Element(UI_ELEMENT* element)
{
  if(!element) return;

  if(element->GetType() == UI_ELEMENT_TYPE_STATISTICSCHART)
    {
      ((UI_ELEMENT_STATISTICSCHART*)element)->InvalidateSharpBitmap();
    }

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(children)
    {
      for(XDWORD c=0; c<children->GetSize(); c++)
        {
          UIScale_InvalidateSharpOverlays_Element(children->Get(c));
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::UIScale_ResetLiveComposition(UI_LAYOUT* layout)
* @brief      Invalidate composition caches, clear the live viewport canvas, dirty chrome on the same screen.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::UIScale_ResetLiveComposition(UI_LAYOUT* layout)
{
  if(!layout || !layout->GetSkin()) return;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();

  skin_canvas->InvalidateCompositionCaches();

  // Prefer the layout background color over pure black so a failed/late PutBackground does not
  // leave navy→black (sidebar, footer, page fill) after resize.
  GRP2DCOLOR_RGBA8 clearcolor(0, 0, 0, 255);
  if(layout->GetBackground() && layout->GetBackground()->GetColor() && layout->GetBackground()->GetColor()->IsValid())
    {
      UI_COLOR* bg = layout->GetBackground()->GetColor();
      clearcolor = GRP2DCOLOR_RGBA8((XBYTE)bg->GetRed(), (XBYTE)bg->GetGreen(), (XBYTE)bg->GetBlue(), (XBYTE)bg->GetAlpha());
    }

  // GetCanvas respects override — sample the live viewport canvas.
  skin_canvas->SetCanvasOverride(NULL);
  GRP2DCANVAS* live = skin_canvas->GetCanvas();
  if(live)
    {
      live->Clear(&clearcolor);
    }

  if(layout->GetDesignCanvas())
    {
      layout->GetDesignCanvas()->Clear(&clearcolor);
      skin_canvas->SetCanvasOverride(layout->GetDesignCanvas());
      skin_canvas->SetTargetCanvas(layout->GetDesignCanvas());
    }

  ModalLayer_Invalidate();

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
  if(screen)
    {
      for(XDWORD c=0; c<layouts.GetSize(); c++)
        {
          UI_LAYOUT* other = layouts.Get(c);
          if(!other || !IsCFGChromesLayout(other)) continue;
          if(!other->GetSkin() || other->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;
          UI_SKINCANVAS* chrome_skin = (UI_SKINCANVAS*)other->GetSkin();
          if(chrome_skin->GetScreen() != screen) continue;
          chrome_skin->InvalidateCompositionCaches();
          other->Elements_SetToRedraw(true);
        }
    }
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Layouts_SetUIScale(UI_LAYOUT* layout, double scale)
* @brief      Runtime zoom: clamp+set uiScale and reclamar paint (Fase 4). No XML reload.
* @ingroup    USERINTERFACE
*
* @param[in]  layout : Layout to zoom.
* @param[in]  scale  : Desired scale (clamped by UI_LAYOUT::SetUIScale).
*
* @return     bool : true if prepared successfully.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_SetUIScale(UI_LAYOUT* layout, double scale)
{
  if(!layout) return false;

  layout->SetUIScaleAutofit(false);
  layout->SetUIScale(scale);
  return UIScale_PrepareLayout(layout);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_MANAGER::Layouts_ApplyFitUIScale(UI_LAYOUT* layout)
* @brief      Fase 5: set scale = min(sw/dw, sh/dh) from the layout's screen and reclamar paint.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::Layouts_ApplyFitUIScale(UI_LAYOUT* layout)
{
  if(!layout || !layout->IsUIScaleActive()) return false;
  if(!layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRPSCREEN*     screen      = skin_canvas->GetScreen();
  if(!screen) return false;

  XDWORD sw = screen->GetWidth();
  XDWORD sh = screen->GetHeight();
  if(!sw || !sh) return false;

  double fit = layout->ComputeFitUIScale(sw, sh);
  layout->SetUIScaleAutofit(true);
  layout->SetUIScaleForFit(fit);
  return UIScale_PrepareLayout(layout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::UseMotionInElement(UI_ELEMENT* element, INPCURSORMOTION* cursormotion)
* @brief      Use motion in element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  cursormotion : Cursormotion pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UseMotionInElement(UI_ELEMENT* element, INPCURSORMOTION* cursormotion)
{
  if(!element)      
    {
      return false;
    }

  if(!cursormotion) 
    {
      return false;
    }

  if(!element->IsVisible()) return false;

  UI_PROPERTY_SCROLLEABLE* property_scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
  if(!property_scrolleable) 
    {
      return false;
    }

  // Motion points are in screen px; element AABB is in design px. Expand the rect to screen when UIScale is active
  // so IsInRect stays aligned (Fase 2 input path; paint scale arrives in Fase 3).
  double rect_x = element->GetXPosition();
  double rect_y = element->GetYPosition() - element->GetBoundaryLine()->height;
  double rect_w = element->GetBoundaryLine()->width;
  double rect_h = element->GetBoundaryLine()->height;
  double scale  = 1.0;

  UI_LAYOUT* layout = element->GetLayout();
  if(layout && layout->IsUIScaleActive())
    {
      scale = layout->GetUIScale();
      if(scale < UI_LAYOUT_UISCALE_MIN) scale = UI_LAYOUT_UISCALE_MIN;
      rect_x = rect_x * scale + layout->GetUIScaleOffsetX();
      rect_y = rect_y * scale + layout->GetUIScaleOffsetY();
      rect_w *= scale;
      rect_h *= scale;
    }

  bool isinrect = cursormotion->IsInRect((int)rect_x, (int)rect_y, (int)rect_w, (int)rect_h);
  if(isinrect)
    {
      XDWORD differential = 0;
      INPCURSORMOTION_DIR motiondir = cursormotion->GetDirectionByDifferential(&differential);
      if(motiondir != INPCURSORMOTION_DIR_UNKNOWN)
        {
          switch(motiondir)
            {
              case INPCURSORMOTION_DIR_UP         :
              case INPCURSORMOTION_DIR_DOWN       : { double shift = (differential/3.0) / scale;
                                                      if(motiondir == INPCURSORMOTION_DIR_DOWN) shift *= -1;
                                                      
                                                      property_scrolleable->Scroll_SetStep(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, shift);   
                                                      GEN_USERINTERFACE.Elements_SetToRedraw(element);                                                                                                                 
                                                                                                                                                                                                                                                                                          
                                                      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Input Motion]  motion %s points: %d"), (motiondir == INPCURSORMOTION_DIR_UP)?__L("up"):__L("down"), cursormotion->GetNPoints());

                                                      return true;  
                                                    }
                                                    break;              

              case INPCURSORMOTION_DIR_RIGHT      : 
              case INPCURSORMOTION_DIR_LEFT       : break;                  

                                      default     : break;
            }             
        } 
    }
                                                                                                                                             
  if(element->GetComposeElements()->GetSize())
    {                                                                      
      for(XDWORD d=0; d<element->GetComposeElements()->GetSize(); d++)   
        {
          UI_ELEMENT* subelement = element->GetComposeElements()->Get(d);
          if(subelement) 
            { 
              if(dynamic_cast<UI_PROPERTY_SELECTABLE*>(subelement))                                                                                                                                                                                                                                          
                {                  
                  if(UseMotionInElement(subelement, cursormotion)) 
                    {
                      return true;                  
                    }
                }
            }
        }
    }                             

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::UseMotion(INPCURSORMOTION* cursormotion)
* @brief      Use motion
* @ingroup    USERINTERFACE
* 
* @param[in]  cursormotion : Cursormotion pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::UseMotion(INPCURSORMOTION* cursormotion)
{
  if(!cursormotion) 
    {
      return false;
    }

  for(int d=0; d<layouts.GetSize(); d++)
    {    
      UI_LAYOUT* layout = layouts.Get(d);                                                          
      if(layout) 
        {
          for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
            {
              UI_ELEMENT* element = layout->Elements_Get()->Get(c);
              if(element) 
                {
                  if(UseMotionInElement(element, cursormotion)) return true;
                }
            }        
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SelectScrollBarInElement(UI_ELEMENT* element, int x, int y)
* @brief      Hit-tests the overlay scrollbar(s) of a scrollable container (and its children) against a pointer.
* @note       Interactive scrollbar: a SELECCTION/touch on the bar track sets the scroll position proportionally to
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SelectScrollBarInElement(UI_ELEMENT* element, int x, int y)
{
  if(!element) return false;

  UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
  if(scrolleable && element->IsActive() && element->IsVisible())
    {
      double box_w      = 0.0f;
      double box_h      = 0.0f;
      UI_SKINCANVAS::GetScrollViewportSize(element, box_w, box_h);
      double box_left   = element->GetXPositionWithScroll();
      double box_bottom = element->GetYPositionWithScroll();
      double box_right  = box_left   + box_w;
      double box_top    = box_bottom - box_h;
      double radius     = (double)element->GetRoundRect();

      double tl;
      double tt;
      double tr;
      double tb;

      // vertical bar : right gutter
      if(scrolleable->Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, box_left, box_top, box_right, box_bottom, radius, tl, tt, tr, tb))
        {
          UI_BOUNDARYLINE track;
          track.x      = tl;
          track.width  = tr - tl;
          track.y      = tb;
          track.height = tb - tt;

          if(track.IsWithin(x, y))
            {
              double tracklen = tb - tt;
              double fraction = (tracklen != 0.0f) ? (((double)y - tt) / tracklen) : 0.0f;

              scrolleable->Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, fraction);
              Elements_SetToRedraw(element);

              return true;
            }
        }

      // horizontal bar : bottom gutter
      if(scrolleable->Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, box_left, box_top, box_right, box_bottom, radius, tl, tt, tr, tb))
        {
          UI_BOUNDARYLINE track;
          track.x      = tl;
          track.width  = tr - tl;
          track.y      = tb;
          track.height = tb - tt;

          if(track.IsWithin(x, y))
            {
              double tracklen = tr - tl;
              double fraction = (tracklen != 0.0f) ? (((double)x - tl) / tracklen) : 0.0f;

              scrolleable->Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, fraction);
              Elements_SetToRedraw(element);

              return true;
            }
        }
    }

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement && SelectScrollBarInElement(subelement, x, y)) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_MANAGER::SelectScrollBar(int x, int y)
* @brief      Hit-tests all layouts' scrollbars against a pointer; applies proportional scroll if hit.
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_MANAGER::SelectScrollBar(int x, int y)
{
  // x,y are screen (framebuffer) px from the input event.
  for(int d=0; d<layouts.GetSize(); d++)
    {
      UI_LAYOUT* layout = layouts.Get(d);
      if(layout)
        {
          int design_x = x;
          int design_y = y;
          MapScreenToDesign(layout, x, y, design_x, design_y);

          for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
            {
              UI_ELEMENT* element = layout->Elements_Get()->Get(c);
              if(element && SelectScrollBarInElement(element, design_x, design_y)) return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_MANAGER::HandleEvent_UI(UI_XEVENT* event)
* @brief      Handle event UI
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::HandleEvent_UI(UI_XEVENT* event)
{
  switch(event->GetEventType())
    {
      case UI_XEVENT_TYPE_INPUT_CURSOR_MOVE         : { UI_ELEMENT*  _preselect_element = NULL;
                                                        int          screen_x           = event->GetXPos();
                                                        int          screen_y           = event->GetYPos();

                                                        //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("x: %d, y: %d"), screen_x, screen_y);

                                                        // Remember where the pointer is right now (not only where it last
                                                        // landed ON an element). UnSelectedElement() re-issues a CURSOR_MOVE at
                                                        // last_xposition/last_yposition once a selection's highlight expires, to
                                                        // restore the hover (preselect) under the pointer. On a mouse that point
                                                        // is wherever the cursor sits; on touch the finger is gone after release
                                                        // and the device parks the cursor off-canvas (-1,-1), so keeping these in
                                                        // sync here means the lifted finger leaves NO stuck preselect, while a
                                                        // real mouse still correctly re-preselects whatever it is hovering.
                                                        // Stored in SCREEN px; MapScreenToDesign converts per layout on use.
                                                        last_xposition = screen_x;
                                                        last_yposition = screen_y;
                                                  
                                                        if(element_modal)
                                                          {
                                                            int design_x = screen_x;
                                                            int design_y = screen_y;
                                                            MapScreenToDesign(element_modal->GetLayout(), screen_x, screen_y, design_x, design_y);
                                                            _preselect_element = PreSelectElement(element_modal, design_x, design_y);
                                                          }
                                                         else
                                                          {
                                                            // P1.8 fix: this used to "break" out of both loops as soon as one
                                                            // top-level element/layout produced a hit, leaving every element
                                                            // declared AFTER it (e.g. "nav-procesos-btn".."nav-configuracion-btn",
                                                            // which sit after "nav-disco-btn" in dashboard.xml) unvisited for the
                                                            // tick -- so a row hovered on an earlier tick and then skipped over by
                                                            // a non-monotonic pointer path never received its SetPreSelect(false),
                                                            // and stayed visually stuck (see PreSelectElement()'s own note on the
                                                            // same issue one level down, in the recursive child loop). Visiting
                                                            // every top-level element/layout every tick is cheap and guarantees a
                                                            // single, correct hover state regardless of the path the pointer took.
                                                            //
                                                            // Fase 2: convert screen→design per layout before IsWithin so AABB
                                                            // stay in design px while the pointer arrives in framebuffer px.
                                                            for(int d=0; d<layouts.GetSize(); d++)
                                                              {
                                                                UI_LAYOUT* layout = layouts.Get(d);
                                                                if(layout)
                                                                  {
                                                                    int design_x = screen_x;
                                                                    int design_y = screen_y;
                                                                    MapScreenToDesign(layout, screen_x, screen_y, design_x, design_y);

                                                                    for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
                                                                      {
                                                                        UI_ELEMENT* element = layout->Elements_Get()->Get(c);
                                                                        if(element)
                                                                          {
                                                                            UI_ELEMENT* found = PreSelectElement(element, design_x, design_y);
                                                                            if(found) _preselect_element = found;
                                                                          }
                                                                      }
                                                                  }
                                                              }
                                                          }

                                                        if(!_preselect_element)  
                                                          {
                                                            if(preselect_element)
                                                              {
                                                                ResetPreselect();                                                                                                 
                                                              }
                                                          } 
                                                         else
                                                          {
                                                            if(_preselect_element != preselect_element)
                                                              {
                                                                if(preselect_element) 
                                                                  { 
                                                                    ResetPreselect();                                     
                                                                  }

                                                                preselect_element = _preselect_element;                                                                      
                                                              }                                                           
                                                          }                                                                                                                                                  
                                                      }
                                                      break;

      case UI_XEVENT_TYPE_INPUT_CURSOR_MOTION       : { 
                                                        if(UseMotion(event->GetCursorMotion()))
                                                          {
                                                            UnSelectedElement();
                                                          }
                                                      }
                                                      break;

      case UI_XEVENT_TYPE_INPUT_SELECCTION          : { int scrollbar_x = event->GetXPos();
                                                        int scrollbar_y = event->GetYPos();
                                                        if(SelectScrollBar(scrollbar_x, scrollbar_y)) break;   // consumed by an interactive scrollbar

                                                        // Phase 4: :pressed mirrors pointer-down on the hovered selectable.
                                                        if(preselect_element) preselect_element->SetPressed(true);

                                                        if(element_modal)
                                                          { 
                                                            if(xmutex_modal) 
                                                              {
                                                                xmutex_modal->Lock(); 
                                                              }

                                                            SelectElement(element_modal);                                                                                   

                                                            if(xmutex_modal) 
                                                              {
                                                                xmutex_modal->UnLock(); 
                                                              }
                                                          }
                                                         else
                                                          {
                                                            for(int d=0; d<layouts.GetSize(); d++)
                                                              {    
                                                                UI_LAYOUT* layout = layouts.Get(d);                                                          
                                                                if(layout) 
                                                                  {
                                                                    for(XDWORD c=0; c<layout->Elements_Get()->GetSize(); c++)
                                                                      {
                                                                        UI_ELEMENT* element = layout->Elements_Get()->Get(c);
                                                                        if(element) 
                                                                          {
                                                                            if(SelectElement(element)) 
                                                                              {
                                                                                break;
                                                                              }                                                                                                                                                                                                                                                                                                                                      
                                                                          }
                                                                      }
                                                                  }
                                                              }
                                                          }

                                                        if(preselect_element) preselect_element->SetPressed(false);
                                                      }
                                                      break;      
    }
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_MANAGER::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_USERINTERFACE    : { UI_XEVENT* event = (UI_XEVENT*)xevent;
                                            if(!event) return;

                                            HandleEvent_UI(event);
                                          }
                                          break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @class      UI_MANAGER_ELEMENTANCESTORPROVIDER
* @brief      Phase 2 ("combinadores descendiente/hijo"): concrete UI_CSSANCESTORPROVIDER for a real element
*             tree, walking UI_ELEMENT::GetFather(). File-local counterpart of UI_STYLE_ELEMENTANCESTORPROVIDER
*             (UI_Style.cpp) -- see that class's doc comment for why this stays a small file-local duplicate
*             rather than a shared header type.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
class UI_MANAGER_ELEMENTANCESTORPROVIDER : public UI_CSSANCESTORPROVIDER
{
  public:
    UI_MANAGER_ELEMENTANCESTORPROVIDER(UI_ELEMENT* _startelement) { startelement = _startelement; }
    virtual ~UI_MANAGER_ELEMENTANCESTORPROVIDER() {}

    virtual bool GetAncestor(int depth, XSTRING** outtype, XSTRING** outid, XVECTOR<XSTRING*>** outclasses)
    {
      if(!startelement) return false;

      UI_ELEMENT* current = startelement->GetFather();

      for(int d=0; d<depth; d++)
        {
          if(!current) return false;
          current = current->GetFather();
        }

      if(!current) return false;

      if(outtype)    *outtype    = current->GetTypeString();
      if(outid)      *outid      = current->GetName();
      if(outclasses) *outclasses = current->GetClassNames();

      return true;
    }

    virtual bool FillAncestorPseudos(int depth, XVECTOR<XSTRING*>& outpseudos)
    {
      if(!startelement) return false;

      UI_ELEMENT* current = startelement->GetFather();

      for(int d=0; d<depth; d++)
        {
          if(!current) return false;
          current = current->GetFather();
        }

      if(!current) return false;

      current->GetActivePseudos(outpseudos);
      return true;
    }

  private:

    UI_ELEMENT* startelement;
};


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::PrepareElementStyleState(UI_ELEMENT* element)
* @brief      Walk a freshly-built subtree, snapshot every element's visual baseline and mark those the active
*             stylesheet can restyle via pseudo-class rules. Called once per top-level element right after
*             CreatePartialLayout returns.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  element : Root of the subtree.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::PrepareElementStyleState(UI_ELEMENT* element)
{
  if(!element) return;

  element->SnapshotStyleVisual();

  // Phase 1 ownership step: the stylesheet to check comes from THIS element's own UI_LAYOUT (set by
  // GetLayoutElement_Base() before this function ever runs), not a single UI_MANAGER-wide pointer -- see
  // UI_ELEMENT::GetLayout()/UI_LAYOUT::GetStyleSheet().
  UI_LAYOUT*     element_layout = element->GetLayout();
  UI_STYLESHEET* sheet          = element_layout ? element_layout->GetStyleSheet() : NULL;

  if(sheet)
    {
      XSTRING*           type_string = element->GetTypeString();
      XSTRING*           name        = element->GetName();
      XSTRING            emptystr;
      XVECTOR<XSTRING*>  emptyclasses;

      XSTRING&           elem_type    = type_string ? *type_string  : emptystr;
      XSTRING&           elem_id      = name        ? *name         : emptystr;
      XVECTOR<XSTRING*>& elem_classes = element->GetClassNames() ? *element->GetClassNames() : emptyclasses;

      // Phase 2 ("combinadores descendiente/hijo"): see UI_MANAGER_ELEMENTANCESTORPROVIDER above -- without it,
      // a rule using a descendant/child combinator could never be found by this probe.
      UI_MANAGER_ELEMENTANCESTORPROVIDER ancestorprovider(element);

      if(sheet->HasPseudoRulesFor(elem_type, elem_id, elem_classes, &ancestorprovider))
        {
          element->SetStyleHasStateRules(true);
        }
    }

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(children)
    {
      for(XDWORD c=0; c<children->GetSize(); c++)
        {
          PrepareElementStyleState(children->Get(c));
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::RefreshFlexProgressBarTracks(UI_ELEMENT* element, UI_SKIN* skin)
* @brief      Phase 4 ("migración del ejemplo", fifth sub-step: "card bodies") follow-up fix. A ProgressBar's
*             "track" (the sub-element auto-created from its "progressrect" attribute) is positioned by
*             UI_SKINCANVAS::CalculePosition() using its FATHER's (the ProgressBar's own) GetXPosition()/
*             GetYPosition() as the anchor -- see CalculateBoundaryLine_ProgressBar(). That call happens once,
*             at CreatePartialLayout() time, i.e. BEFORE UI_LAYOUTENGINE::RunLayout() ever runs (RunLayout() is
*             a deliberate POST-load hook -- see its own SCOPE ADDENDUM in UI_LayoutEngine.h). For a ProgressBar
*             that is itself a flex ITEM (e.g. dashboard.xml's "ram_linear_bar", nested inside "right-col"), the
*             track was therefore positioned against the ProgressBar's PRE-flex position (whatever
*             GetXPosition()/GetYPosition() held before RunLayout() moved it -- typically 0,0, since a flex item
*             carries no xpos/ypos of its own), leaving the track's own drawn geometry stuck near the origin
*             instead of tracking its father's real, flex-resolved position: the bar's fill/track rectangle
*             silently failed to render anywhere near the visible ProgressBar box (confirmed against a real
*             windowed build: "ram_linear_bar" drew nothing at all in its card).
*
*             This is the SAME class of bug UI_MANAGER::ChangeTextElementValue() already works around for a
*             live-updating flex CHILD (see its own "second sub-step, live-update follow-up" comment): a
*             one-time legacy position calculation does not automatically track a later flex reflow. The fix
*             there is to re-run the affected calculation once the flex-resolved position is known; this is
*             that same fix applied once, at LOAD time, for every ProgressBar in the freshly-built subtree
*             (whether or not it turns out to be a flex item -- re-running CalculateBoundaryLine_ProgressBar()
*             against an UNCHANGED position is a harmless no-op, so this does not need to special-case which
*             ProgressBars actually moved). ProgressRadial/ProgressImage do not need this: their own children
*             (e.g. "cpu_usage_value"/"ram_usage_value") are centered via UI_ELEMENT_TYPE_ALIGN_CENTER, resolved
*             by CalculePosition() against the FATHER's already-correct GetBoundaryLine() width/height at DRAW
*             time (see UI_SKINCANVAS::Draw_ProgressRadial()), not cached from a one-time load-time snapshot.
*
*             Deliberately narrower than "just call CalculateBoundaryLine_ProgressBar() again": that function's
*             OWN first step re-runs CalculePosition() on the ProgressBar element itself, using its raw
*             GetBoundaryLine()->x/y (a legacy alignment token/offset, e.g. UI_ELEMENT_TYPE_ALIGN_LEFT for a flex
*             item that carries no "xpos" attribute at all) against its FATHER's position -- which would silently
*             throw away the ProgressBar's own correct, flex-resolved GetXPosition()/GetYPosition() (see
*             UI_CSSBox_Set()) and re-pin it back to its father's raw corner, undoing RunLayout() instead of
*             completing it. This helper therefore repositions the track using the ProgressBar's OWN
*             already-correct position as the anchor -- exactly the sub-step CalculateBoundaryLine_ProgressBar()
*             would have run, minus the one that must NOT be repeated.
* @note       ROOT-CAUSE FIX (2026-09): this used to reposition ONLY element_progressrect, via a bare
*             CalculePosition() call -- the sub-element's NATURAL, unshifted position. That is a safe no-op for
*             allocationtext="none"/"center" (CalculateBoundaryLine_ProgressBar() never moves the rect for
*             those), but for "down"/"up"/"left" it silently UNDID the shift CalculateBoundaryLine_ProgressBar()
*             had already applied moments earlier (from CreatePartialLayout(), just before RunLayout()/this hook
*             run): that shift moves element_progressrect (and element_animation) to free up room for the
*             caption, and resetting it back to "natural" leaves the rect overlapping the caption's own zone for
*             the entire first frame -- confirmed live on progressbar3 in UI_Options' example.xml
*             (allocationtext="down"): the rect sat unshifted until the first REAL value change re-ran
*             CalculateBoundaryLine_ProgressBar() from scratch, at which point it jumped to its correct position
*             in one visible step. That one-time load-vs-first-change geometry jump is what every downstream
*             caption redraw/restore mechanism (see the GHOST-FILL FIX in Draw_ProgressBar(), UI_SkinCanvas.cpp)
*             had to treat as a real, unexpected move. Fix: call
*             UI_SKINCANVAS::ReapplyProgressBarAllocationLayout() instead of a bare CalculePosition() -- the EXACT
*             same sub-element positioning + allocationtext shift CalculateBoundaryLine_ProgressBar() itself runs
*             (extracted there into that one shared method for this reason), so the track (and, where relevant,
*             the animation/caption) end up at their final, correctly-shifted position from this very first call,
*             and never need to jump later.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Root of the subtree (a freshly-built top-level element, or any descendant during recursion).
* @param[in]  skin : Skin to resolve positions against; NULL (or a non-canvas skin) is a safe no-op.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::RefreshFlexProgressBarTracks(UI_ELEMENT* element, UI_SKIN* skin)
{
  if(!element) return;

  UI_SKINCANVAS* skincanvas = dynamic_cast<UI_SKINCANVAS*>(skin);

  if(skincanvas && (element->GetType() == UI_ELEMENT_TYPE_PROGRESSBAR))
    {
      UI_ELEMENT_PROGRESSBAR* element_progressbar = (UI_ELEMENT_PROGRESSBAR*)element;

      skincanvas->ReapplyProgressBarAllocationLayout(element_progressbar, false);
    }

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(children)
    {
      for(XDWORD c=0; c<children->GetSize(); c++)
        {
          RefreshFlexProgressBarTracks(children->Get(c), skin);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_MANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_MANAGER::Clean()
{
  iszippedfile        = false;
  unzipfile           = NULL;  
  
  layout_commonindex  = UI_MANAGER_LAYOUT_NOTFOUND;

  xmutex_modal        = NULL;
  element_modal       = NULL;

  modal_layer_bitmap       = NULL;
  modal_layer_x            = 0.0;
  modal_layer_y            = 0.0;
  modal_layer_w            = 0.0;
  modal_layer_h            = 0.0;
  modal_layer_valid        = false;
  modal_layer_compositing  = false;

  xmutex_UIevent      = NULL;

  last_xposition      = 0;
  last_yposition      = 0;

  preselect_element   = NULL;

  virtualkeyboard     = NULL;
}



