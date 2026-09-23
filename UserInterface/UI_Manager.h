/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Manager.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XPath.h"
#include "XObserver.h"
#include "XSubject.h"
#include "XFileXML.h"
#include "XString.h"
#include "XFileZIP.h"

#include "GRPProperties.h"

#include "UI_XEvent.h"
#include "UI_Element.h"
#include "UI_Skin.h"
#include "UI_Layout.h"
#include "UI_Length.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_MANAGER_LAYOUT_NOTFOUND          -1
#define UI_MANAGER_LAYOUT_COMMON            __L("COMMON")
#define UI_MANAGER_LAYOUT_TEXT_INI          __L("#[")
#define UI_MANAGER_LAYOUT_TEXT_END          __L("]")
#define UI_MANAGER_LAYOUT_TEXT_TRANSLATE    __L("TRANSLATE_")



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class GRPSCREEN;
class GRPCONTEXT;
class GRP2DCANVAS;
class GRPBITMAP;
class INPCURSORMOTION;
class UI_ANIMATION;
class UI_ELEMENT_TEXT;
class UI_ELEMENT_FORM;
class UI_VIRTUALKEYBOARD;
class UI_STYLE;
class UI_STYLESHEET;
class UI_COMPUTEDSTYLE;
class UI_SKINCANVAS;


class UI_MANAGER : public XOBSERVER, public XSUBJECT
{
  public:

    static bool                     GetIsInstanced                            ();
    static UI_MANAGER&              GetInstance                               ();
    static bool                     DelInstance                               ();

    bool                            Load                                      (XPATH& pathfile, GRPSCREEN* screen, int viewportindex = 0);
    bool                            LoadLayout                                (XPATH& pathfile, GRPSCREEN* screen, int viewportindex = 0);
  
    bool                            IsZippedFile                              (); 
    XPATH*                          GetUnzipPathFile                          (); 
    XFILEUNZIP*                     GetUnzipFile                              ();
    bool                            DeleteTemporalUnZipFile                   (XPATH& pathfile);
    bool                            CloseUnZipFile                            ();
    
    bool                            Layouts_Add                               (UI_LAYOUT* layout);
    XVECTOR<UI_LAYOUT*>*            Layouts_GetAll                            ();
    UI_LAYOUT*                      Layouts_Get                               (int index);
    UI_LAYOUT*                      Layouts_Get                               (XCHAR* name);
    UI_LAYOUT*                      Layouts_Get                               (XSTRING& tname);
    bool                            Layouts_DeleteAll                         ();     
    UI_LAYOUT*                      Layouts_GetCommonLayout                   ();
           
    // -------------------------------------------------------------------------
    // SCREEN COMPOSITION LAYERS (dirty-rect, NOT full-frame Z)
    // -------------------------------------------------------------------------
    // Per GRPSCREEN / shared viewport canvas the paint order is fixed:
    //   1) Background (Layout_PutBackground)
    //   2) Content layouts (non-chrome)
    //   3) Chrome layout (CFG caption / window buttons)
    //   4) Modal layer — Draw the modal tree LAST on the live canvas (opaque AABB wipe + paint). Layout
    //      skips the modal root while modal_layer_valid so it is not painted twice. This is option B:
    //      the modal always wins the intersection without full-frame Z and without capturing a punched
    //      shared-canvas snapshot.
    //
    // GetZLevel() only orders RESTORE inside one skin's RebuildAllAreas (erase high→low).
    // It does NOT define paint order across layouts.
    // -------------------------------------------------------------------------

    // Layout_PutBackground* rewrite the canvas under widgets. Both entry points then call
    // InvalidateCompositionCachesForScreen() so EVERY canvas skin on that screen (content layouts AND the
    // custom window-chrome layout) drops persistent backdrop caches. Without the screen-wide wipe, chrome
    // kept a stale formbackdrop after the virtual keyboard PutBackground and could bake itself into its own
    // cache / leave ghosts over the top menu when auto-hide toggles.
    //
    // AUTHORING CONTRACT (compat descendente): layouts WITHOUT a <stylesheet> keep the historical XML-only
    // path (no CSS cascade, no ReapplyStyleVisual). CSS Lite is opt-in per layout. Absolute xpos/ypos layouts
    // (e.g. UI_Options) must remain pixel-identical when no stylesheet is present.
    // Margin: no sheet -> 4-value LEFT,RIGHT,UP,DOWN and margin-* longhands ignored; with sheet -> CSS TRBL
    // plus margin-top/right/bottom/left (see UI_PROPERTYREGISTRY::ResolveMarginEdges).
    //
    // UIScale (Opción A): authors work in design px (UI_LAYOUT::SetDesignSize / GetUIScale). Opt-in when the
    // layout has a stylesheet OR SetUIScaleEnabled(true). scale=1.0 is the compatible baseline. Input maps
    // screen→design via MapScreenToDesign before IsWithin (Fase 2). Paint uses design offscreen + scaled
    // present when NeedsScaledPresent (Fase 3). XML-only layouts ignore UIScale entirely.
    bool                            Layout_PutBackground                      (XCHAR* layoutname);    
    bool                            Layout_PutBackgroundColor                 (XCHAR* layoutname);    
    bool                            Layout_PutBackgroundImage                 (XCHAR* layoutname);    
    bool                            Layout_PutBackgroundSeamlessPattern       (XCHAR* layoutname);    

    bool                            Layout_PutBackground                      (bool scale = false);    
    bool                            Layout_PutBackgroundColor                 ();    
    bool                            Layout_PutBackgroundImage                 (bool scale = false);    
    bool                            Layout_PutBackgroundSeamlessPattern       ();

    // Drop composition caches for every UI_SKINCANVAS whose GetScreen() is "screen" (content + chrome).
    void                            InvalidateCompositionCachesForScreen     (GRPSCREEN* screen);
    // Mark every layout on "screen" dirty. exclude_chrome: skip the CFG chromes layout (content-only redraw).
    void                            Elements_SetToRedrawForScreen             (GRPSCREEN* screen, bool exclude_chrome = false);    

    bool                            Update                                    (UI_LAYOUT* layout);
    bool                            Update                                    (XCHAR* layoutname);
    bool                            Update                                    ();
        
    UI_ELEMENT*                     Element_Get                               (XCHAR* layoutname, XCHAR* name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);  
    UI_ELEMENT*                     Element_Get                               (XCHAR* layoutname, XSTRING& name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);       
    UI_ELEMENT*                     Element_Get                               (XCHAR* name, UI_ELEMENT_TYPE type);
    UI_ELEMENT*                     Element_Get                               (XSTRING& name, UI_ELEMENT_TYPE type);

    UI_LAYOUT*                      Element_GetLayout                         (UI_ELEMENT* element);
    UI_LAYOUT*                      Element_GetLayout                         (XCHAR* name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);    
    UI_LAYOUT*                      Element_GetLayout                         (XSTRING& name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);    

    bool                            Element_PutToLastPositionLayout           (UI_ELEMENT* element);      
    bool                            Element_PutToLastPositionLayout           (XCHAR* name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);      
    bool                            Element_PutToLastPositionLayout           (XSTRING& name, UI_ELEMENT_TYPE type = UI_ELEMENT_TYPE_UNKNOWN);    
    
    bool                            Element_SetModal                          (UI_ELEMENT* element_modal);
    UI_ELEMENT*                     Element_GetModal                          ();
    // Modal composition layer (option B): after content AND chrome, blit an opaque cache built on an
    // offscreen canvas (Draw of the modal tree only — never a snapshot of the shared screen). Layout Draw
    // of the modal root is deferred while the cache is valid (see ModalLayer_IsLayoutDrawDeferred).
    bool                            Element_DrawModalOnTop                    ();
    void                            ModalLayer_Invalidate                     ();
    bool                            ModalLayer_IsLayoutDrawDeferred           (UI_ELEMENT* element);
    bool                            ModalLayer_IsCompositing                  ();
    // While the modal cache is valid, RebuildAllAreas must not peel the modal subtree (its PreDraw
    // snapshot on the shared canvas is meaningless; the offscreen layer owns those pixels).
    bool                            ModalLayer_IsRebuildProtected             (UI_ELEMENT* element);
    // After the modal layer has been composed once (modal_layer_valid): content whose AABB intersects the
    // modal must not Keep MustReDraw / rebuild-peel — that PutBitmapNoAlpha restores parchment/ListBoxMenu/
    // Edit into the keyboard every frame (UI_Options punch-through). Modal tree itself is excluded.
    // Also suppresses content under a visible custom-chrome caption (translucent caption otherwise shows
    // ListBoxMenu/title ghosts in the title band).
    bool                            ModalLayer_SuppressesContentDraw          (UI_ELEMENT* element);
    bool                            ChromeCaption_SuppressesContentDraw       (UI_ELEMENT* element);
    bool                            Overlay_SuppressesContentDraw             (UI_ELEMENT* element);
    void                            Overlay_ClearSuppressedContentDirt        ();

    // Before RebuildAllAreas: if two canvas skins share the same GRP2DCANVAS, propagate MustReDraw across
    // overlapping rebuild areas so content/chrome/modal cannot leave ghosts in each other's bands.
    void                            PropagateRebuildOverlapDirtAcrossSharedCanvases();

    bool                            Elements_SetToRedraw                      ();
    bool                            Elements_SetToRedraw                      (UI_ELEMENT* element, bool recursive = true);

    bool                            Elements_RebuildDrawAreas                 ();
    bool                            Elements_RebuildDrawAreas                 (UI_LAYOUT* layout); 
    bool                            Elements_RebuildDrawAreas                 (UI_LAYOUT* layout, UI_ELEMENT* element);
    bool                            Elements_RebuildDrawAreas                 (XCHAR* layoutname); 
    bool                            Elements_RebuildDrawAreas                 (XCHAR* layoutname, UI_ELEMENT* element);
    
    int                             GetOutputTextChangeID                     (XSTRING* text, int start, XSTRING& value);
    UI_ANIMATION*                   GetOrAddAnimationCache                    (UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, XCHAR* name, XCHAR* resource, GRP2DCANVAS* referencecanvas = NULL, double width = 0.0, double height = 0.0);
    
    bool                            SetLevelAuto                              (UI_ELEMENT* element, UI_ELEMENT* father, XDWORD addlevel = 0);

    bool                            SetPreselectElement                       (UI_ELEMENT* element = NULL);
    bool                            ResetPreselect                            ();
      
    bool                            SendEvent                                 (int event, ...);
    bool                            ChangeAutomaticTextElementValue           (UI_ELEMENT* element, XSTRING* maskvalue, XSTRING* maskresolved);
    bool                            ChangeTextElementValue                    (UI_ELEMENT* element, XSTRING* text, XSTRING& value);
    
    bool                            ChangeTextElementValue                    (UI_LAYOUT* layout);
    bool                            ChangeTextElementValue                    (UI_LAYOUT* layout, UI_ELEMENT* element);

    
    bool                            SubscribeInputEvents                      (bool active);    
    bool                            SubscribeOutputEvents                     (bool active, XOBSERVER* observer, XSUBJECT* subject); 

    bool                            CreaterVirtualKeyboard                    (UI_LAYOUT* layout, GRPSCREEN* screen);
    bool                            DeleteVirtualKeyboard                     ();

    // Fase 3: after SetUIScale / SetDesignSize, ensure design canvas + seed background + full redraw.
    bool                            UIScale_PrepareLayout                     (UI_LAYOUT* layout);

    // Fase 4: runtime zoom — SetUIScale + PrepareLayout (redraw without reloading XML).
    bool                            Layouts_SetUIScale                        (UI_LAYOUT* layout, double scale);

    // Fase 5: scale = min(sw/dw, sh/dh) then PrepareLayout. No-op if UIScale inactive.
    bool                            Layouts_ApplyFitUIScale                   (UI_LAYOUT* layout);

    // Track L.4: re-resolve rem/vw/vh/%/em length tokens from each element's computed style bag against the
    // current design viewport + parent box, then RunLayout on top-level roots. Call after SetDesignSize or on
    // window CHANGESIZE when the design viewport (or parent geometry) may have changed. XML-only layouts = no-op.
    bool                            Layouts_ReresolveStyleLengths             (UI_LAYOUT* layout);

    // Track L.4 convenience: SetDesignSize + media viewport + ReresolveStyleLengths + UIScale_PrepareLayout.
    bool                            Layouts_SetDesignSize                     (UI_LAYOUT* layout, XDWORD width, XDWORD height);

  private:
                                    UI_MANAGER                                ();
                                    UI_MANAGER                                (UI_MANAGER const&);
    virtual                        ~UI_MANAGER                                ();

    void                            operator =                                (UI_MANAGER const&);

    UI_SKIN*                        Skin_Create                               (XSTRING& skintypename, UI_SKIN_DRAWMODE drawmode, GRPSCREEN* screen, int viewportindex = 0);

    #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE
    bool                            IsCFGChromesLayout                        (UI_LAYOUT* layout);
    #endif

    bool                            GetLayoutElementValue                     (XFILEXMLELEMENT* node, XCHAR* leyend, double& value);
    bool                            GetLayoutElementValue                     (XFILEXMLELEMENT* node, XCHAR* leyend, XSTRING& value);    

    bool                            GetLayoutElement_CalculateBoundaryLine    (UI_LAYOUT* layout, UI_ELEMENT* element, bool adjustsizemargin = false);
    bool                            GetParentSizeFont                         (XFILEXMLELEMENT* node, double& sizefont);
    bool                            ResolvePercentValue                       (XSTRING& valuestr, double basis, double& out);

    // Fase 8: rem/vw/vh/%/em via UI_LENGTH — only for layouts that own a stylesheet (UI_Options untouched).
    void                            BuildLengthContext                        (UI_LAYOUT* layout, double basis, double fontsize, UI_LENGTH_CONTEXT& out);
    bool                            ResolveStyleLength                        (XSTRING& valuestr, UI_LENGTH_CONTEXT& context, double& out);

    // Track L.4: apply xpos/ypos/width/height/gap/flex-basis/margin/padding length tokens from an existing
    // computed-style bag (no XML, no cascade rebuild). Used by Layouts_ReresolveStyleLengths.
    bool                            ApplyStyleLengthsFromBag                 (UI_ELEMENT* element, UI_LAYOUT* layout, UI_STYLE& style);
    void                            ReresolveElementStyleLengthsRecursive     (UI_ELEMENT* element, UI_LAYOUT* layout);

    bool                            GetLayoutElement_Base                     (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent = false);
    bool                            GetLayoutElement_Base                     (UI_STYLE& style, XSTRING& fathertagname, UI_LAYOUT* layout, UI_ELEMENT* element, bool adjusttoparent = false);

    // Same as the XFILEXMLELEMENT* overload above, but also hands back the fully-resolved bag (XML attributes
    // < CSS rules < inline style) it built internally, typed as UI_COMPUTEDSTYLE, so a per-widget builder can
    // read its own extra keys (e.g. "sizefont") through the SAME cascade instead of re-reading the raw XML node
    // and silently losing any CSS/inline override -- see UI_ComputedStyle.h.
    bool                            GetLayoutElement_Base                     (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* element, UI_COMPUTEDSTYLE& outstyle, bool adjusttoparent = false);
    UI_ELEMENT*                     GetLayoutElement_Text                     (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_TextBox                  (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_Image                    (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    bool                            IsVectorResource                          (XCHAR* resource);
    UI_ELEMENT*                     GetLayoutElement_Animation                (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_Option                   (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_MultiOption              (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_Button                   (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_CheckBox                 (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_EditText                 (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_Form                     (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_Menu                     (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_ListBox                  (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_ProgressBar              (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_ProgressRadial           (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_ProgressImage            (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);
    UI_ELEMENT*                     GetLayoutElement_StatisticsChart          (XFILEXMLELEMENT* node, UI_LAYOUT* layout, UI_ELEMENT* father, UI_ELEMENT* element_legacy = NULL);

    UI_ELEMENT*                     CreatePartialLayout                       (XFILEXMLELEMENT* nodeelement, UI_LAYOUT* layout, UI_ELEMENT* father);
    void                            PrepareElementStyleState                  (UI_ELEMENT* element);
    void                            RefreshFlexProgressBarTracks              (UI_ELEMENT* element, UI_SKIN* skin);
    bool                            CreateLayouts                             (XFILEXML& xml, XPATH& xmlpathfile, GRPSCREEN* screen, int viewportindex = 0);
    
    GRPBITMAP*                      LoadBackgroundBitmap                      (XSTRING& namefilebitmap, GRPPROPERTYMODE mode, GRP2DCANVAS* referencecanvas = NULL, double width = 0.0, double height = 0.0);
    GRPBITMAP*                      LoadBackgroundVectorFileToBitmap          (XSTRING& namefilevector, GRP2DCANVAS* referencecanvas, double width, double height);
    bool                            PutBitmapAsSeamlessPattern                (GRP2DCANVAS* canvas, GRPSCREEN* screen, GRPBITMAP* pattern);

    bool                            CreateCacheElements                       (XFILEXMLELEMENT* nodeelement, UI_SKIN_DRAWMODE drawmode, GRPPROPERTYMODE grppropertymode, bool recursive = true); 
    
    bool                            RegisterEvents                            (bool active); 

    UI_ELEMENT*                     PreSelectElement                          (UI_ELEMENT* element, int x, int y);
    
    bool                            SelectElement                             (UI_ELEMENT* element);
    bool                            SelectedElement                           (UI_ELEMENT* element);
    
    bool                            UnSelectedElement                         (UI_ELEMENT* element);
    bool                            UnSelectedElement                         ();

    bool                            UseMotionInElement                        (UI_ELEMENT* element, INPCURSORMOTION* cursormotion);
    bool                            UseMotion                                 (INPCURSORMOTION* cursormotion);

    // Map pointer screen px → design px for a layout (identity if UIScale inactive).
    void                            MapScreenToDesign                         (UI_LAYOUT* layout, int screen_x, int screen_y, int& design_x, int& design_y);

    // Fase 3 internals: design offscreen paint + scaled present (letterbox).
    bool                            UIScale_EnsureDesignCanvas                (UI_LAYOUT* layout);
    bool                            UIScale_BeginFrame                        (UI_LAYOUT* layout);
    bool                            UIScale_Present                           (UI_LAYOUT* layout);
    void                            UIScale_EndFrame                          (UI_LAYOUT* layout);
    // After scale/window change: drop composition caches, clear live canvas, dirty chrome.
    void                            UIScale_ResetLiveComposition              (UI_LAYOUT* layout);
    // Fase 7: post-Present sharp overlay for SVG icons + StatisticsCharts (no BoundaryLine mutation).
    void                            UIScale_PresentSharpOverlay               (UI_LAYOUT* layout, GRP2DCANVAS* live);
    void                            UIScale_PresentSharpOverlay_Element       (UI_LAYOUT* layout, UI_ELEMENT* element, GRP2DCANVAS* live, double density);
    void                            UIScale_InvalidateSharpOverlays           (UI_LAYOUT* layout);
    void                            UIScale_InvalidateSharpOverlays_Element   (UI_ELEMENT* element);

    bool                            SelectScrollBar                           (int x, int y);
    bool                            SelectScrollBarInElement                  (UI_ELEMENT* element, int x, int y);

    void                            HandleEvent_UI                            (UI_XEVENT* event);
    void                            HandleEvent                               (XEVENT* xevent);
    
    void                            Clean                                     ();   

    void                            ModalLayer_Release                        ();
    bool                            ModalLayer_RebuildOffscreen               (UI_SKINCANVAS* skin_canvas, UI_ELEMENT* modal);
    bool                            ModalLayer_Rect                           (UI_ELEMENT* modal, double& x, double& y, double& w, double& h);

    static UI_MANAGER*              instance; 

    bool                            iszippedfile;
    XPATH                           unzippathfile;
    XFILEUNZIP*                     unzipfile;
        
    
    XVECTOR<UI_LAYOUT*>             layouts;
    int                             layout_commonindex;
   
    XMUTEX*                         xmutex_modal;
    UI_ELEMENT*                     element_modal;

    // Option B modal layer cache (opaque snapshot blitted after content).
    GRPBITMAP*                      modal_layer_bitmap;
    double                          modal_layer_x;
    double                          modal_layer_y;
    double                          modal_layer_w;
    double                          modal_layer_h;
    bool                            modal_layer_valid;
    bool                            modal_layer_compositing;  // true while Composite rebuilds via Draw

    XMUTEX*                         xmutex_UIevent;
    
    int                             last_xposition;
    int                             last_yposition;

    UI_ELEMENT*                     preselect_element;

    UI_VIRTUALKEYBOARD*             virtualkeyboard;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





