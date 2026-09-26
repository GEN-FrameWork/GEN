/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element.h
* 
* @class      UI_ELEMENT
* @brief      User Interface Element class
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

#include "XPublisher.h"
#include "XSubject.h"

#include "UI_Color.h"
#include "UI_BoundaryLine.h"

// CSS Lite wiring (see UI_LayoutEngine.h's RunLayout()/BuildTree() SCOPE ADDENDUM): UI_ELEMENT reuses the SAME
// Flexbox enums/structs UI_LAYOUTBOX already defines (UI_FLEX_DIRECTION, UI_JUSTIFY_CONTENT, UI_FLEX_WRAP,
// UI_ALIGN_CONTENT, UI_ALIGN_ITEMS, UI_ALIGN_SELF, UI_LAYOUTBOX_INSET) instead of a parallel duplicate set, so
// BuildTree() can copy an element's Flexbox properties onto its mirrored UI_LAYOUTBOX with a plain assignment,
// no translation table. UI_LayoutBox.h has no dependency on this header, so this include is one-directional.
#include "UI_LayoutBox.h"
#include "UI_Style.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum UI_ELEMENT_TYPE
{
	UI_ELEMENT_TYPE_UNKNOWN												= 0	,	
	
	UI_ELEMENT_TYPE_TEXT															,	
	UI_ELEMENT_TYPE_TEXTBOX											  		,	
	UI_ELEMENT_TYPE_IMAGE															,	
	UI_ELEMENT_TYPE_ANIMATION													,		
	UI_ELEMENT_TYPE_OPTION														,	
	UI_ELEMENT_TYPE_MULTIOPTION												,									
	UI_ELEMENT_TYPE_BUTTON														,		
	UI_ELEMENT_TYPE_CHECKBOX													,		
	UI_ELEMENT_TYPE_EDITTEXT													,		
	UI_ELEMENT_TYPE_FORM															,
	UI_ELEMENT_TYPE_MENU															,				
	UI_ELEMENT_TYPE_LISTBOX														,			
	UI_ELEMENT_TYPE_SCROLL														,
	UI_ELEMENT_TYPE_PROGRESSBAR												,	
  UI_ELEMENT_TYPE_PROGRESSRADIAL										,	
	UI_ELEMENT_TYPE_PROGRESSIMAGE                       ,
  UI_ELEMENT_TYPE_STATISTICSCHART  
};				


enum UI_ELEMENT_TYPE_ALIGN
{
  UI_ELEMENT_TYPE_ALIGN_NOTDETERMINED	   = -1000000 ,
  UI_ELEMENT_TYPE_ALIGN_LEFT												,
  UI_ELEMENT_TYPE_ALIGN_RIGHT												,
  UI_ELEMENT_TYPE_ALIGN_UP													,
  UI_ELEMENT_TYPE_ALIGN_DOWN												,
  UI_ELEMENT_TYPE_ALIGN_CENTER											,
	UI_ELEMENT_TYPE_ALIGN_MAX													,
	UI_ELEMENT_TYPE_ALIGN_AUTO												,
};


enum UI_ELEMENT_TYPE_DIRECTION
{
	UI_ELEMENT_TYPE_DIRECTION_UNKWOWN							= 0	,
	UI_ELEMENT_TYPE_DIRECTION_HORIZONTAL							,
	UI_ELEMENT_TYPE_DIRECTION_VERTICAL								,
};


enum UI_ELEMENT_TRANSITION_STATE_SHOW	
{
	UI_ELEMENT_TRANSITION_STATE_SHOW_NONE					= 0 ,
	UI_ELEMENT_TRANSITION_STATE_SHOW_TOSHOW						,
	UI_ELEMENT_TRANSITION_STATE_SHOW_TOHIDE						,
};

// NOTE: reserved roles a layout element can take in a GEN custom Chromes (window caption) layout. Set via the
// "role" XML attribute (see UI_MANAGER::GetLayoutElement_Base); NONE means "just a regular element", the default
// for every element that is not part of a Chromes layout.
enum UI_ELEMENT_CHROMEROLE
{
	UI_ELEMENT_CHROMEROLE_NONE											= 0	,

	UI_ELEMENT_CHROMEROLE_CAPTION											,	// Draggable caption background/area
	UI_ELEMENT_CHROMEROLE_ICON												,	// Application icon
	UI_ELEMENT_CHROMEROLE_TITLE											,	// Window title text
	UI_ELEMENT_CHROMEROLE_MINIMIZE										,	// Minimize button
	UI_ELEMENT_CHROMEROLE_MAXIMIZE										,	// Maximize / restore button
	UI_ELEMENT_CHROMEROLE_CLOSE											,	// Close button
};

#define UI_ELEMENT_MARGIN_MAX										  4

// --- Step 5: per-corner border radii (CSS ordering, clockwise from top-left) ---------------------------------------
enum UI_ELEMENT_BORDER_CORNER
{
  UI_ELEMENT_BORDER_CORNER_TL      = 0 ,
  UI_ELEMENT_BORDER_CORNER_TR            ,
  UI_ELEMENT_BORDER_CORNER_BR            ,
  UI_ELEMENT_BORDER_CORNER_BL            ,
  UI_ELEMENT_BORDER_CORNER_MAX
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class UI_ELEMENT_SCROLL;
class UI_LAYOUT;

class UI_ELEMENT : public XSUBJECT 
{
  public:
																					UI_ELEMENT									();
    virtual															 ~UI_ELEMENT									();

		XSTRING*															GetName											();

		UI_ELEMENT_TYPE												GetType											();
		void																	SetType											(UI_ELEMENT_TYPE type);

		XSTRING*															GetTypeString								();

		XSTRING*															GetClassNamesRaw						();
		XVECTOR<XSTRING*>*										GetClassNames								();
		void																	SetClassNames								(XCHAR* rawlist);
		void																	SetClassNames								(XSTRING& rawlist);
		bool																	HasClass										(XCHAR* classname);

		void																	GetActivePseudos						(XVECTOR<XSTRING*>& out);

		void																	SnapshotStyleVisual					();

		virtual void													ReapplyStyleVisual					();

		void																	ReapplyStyleVisualRecursive	();

		UI_STYLE*															GetComputedStyle						();
		void																	StoreComputedStyle					(UI_STYLE& style);

		bool																	GetStyleHasStateRules				();
		void																	SetStyleHasStateRules				(bool has);

		UI_ELEMENT_CHROMEROLE									GetChromeRole								();
		void																	SetChromeRole								(UI_ELEMENT_CHROMEROLE chromerole);

		UI_ELEMENT*														GetFather										();
		void																	SetFather										(UI_ELEMENT* father);

		UI_LAYOUT*														GetLayout										();
		void																	SetLayout										(UI_LAYOUT* layout);

		bool																	IsDetached									();	
		void																	SetIsDetached								(bool isdetached);	
		
		UI_COLOR*															GetColor										();
		UI_COLOR*															GetBackgroundColor					();

		bool																	IsColorSet									();
		void																	SetColorSet									(bool value);

		bool																	IsBackgroundColorSet				();
		void																	SetBackgroundColorSet				(bool value);
		
		double																GetXPosition								();		
		bool																	SetXPosition								(double x_position);		

		double																GetYPosition								();		
		bool																	SetYPosition								(double y_position);		

		XDWORD																GetZLevel										();		
		bool																	SetZLevel										(XDWORD z_level);		
	
		UI_ELEMENT_TYPE_DIRECTION							GetDirection								();
		bool																	SetDirection								(UI_ELEMENT_TYPE_DIRECTION direction);

		UI_ELEMENT_TYPE_ALIGN									GetTextAlign								();
		bool																	SetTextAlign								(UI_ELEMENT_TYPE_ALIGN textalign);

		double																GetXPositionWithScroll			();
		bool																	SetXPositionWithScroll			(double x_positionwithscroll);		

		double																GetYPositionWithScroll			();		
		bool																	SetYPositionWithScroll			(double y_positionwithscroll);		

		UI_BOUNDARYLINE*											GetBoundaryLine							();
		void																	SetBoundaryLine							(UI_BOUNDARYLINE& boundaryline);

		double																GetLeftX										();
		double																GetRightX										();
		double																GetTopY											();
		double																GetBottomY									();
		
		double																GetMargin                   (UI_ELEMENT_TYPE_ALIGN position);
		void																	SetMargin                   (UI_ELEMENT_TYPE_ALIGN position, double value);

		double																GetPadding                  (UI_ELEMENT_TYPE_ALIGN position);
		void																	SetPadding                  (UI_ELEMENT_TYPE_ALIGN position, double value);

		double																GetBorderWidth              ();
		void																	SetBorderWidth              (double borderwidth);

		UI_COLOR*															GetBorderColor              ();
		bool																	IsBorderColorSet            ();
		void																	SetBorderColorFromString    (XCHAR* string);
		void																	SetBorderColorFromString    (XSTRING& string);

		double																GetBorderRadius             (UI_ELEMENT_BORDER_CORNER corner);
		void																	SetBorderRadius             (UI_ELEMENT_BORDER_CORNER corner, double value);
		double																GetEffectiveBorderRadius    (UI_ELEMENT_BORDER_CORNER corner);
		bool																	HasAnyPerCornerRadius       ();

		bool																	IsBoxShadowSet              ();
		void																	SetBoxShadowSet             (bool value);
		double																GetShadowOffsetX            ();
		void																	SetShadowOffsetX            (double value);
		double																GetShadowOffsetY            ();
		void																	SetShadowOffsetY            (double value);
		double																GetShadowBlur               ();
		void																	SetShadowBlur               (double value);
		UI_COLOR*															GetShadowColor              ();

		bool																	IsActive										();
		void																	SetActive										(bool isactive);

		bool																	IsPreSelect									();
		bool																	SetPreSelect								(bool ispreselect);

		bool																	IsSelected									();
		void																	SetSelected									(bool isselected);		
		
		bool																	IsVisible										();
		void																	SetVisible									(bool isvisible);		

		XDWORD																GetRoundRect								();
    void																	SetRoundRect								(XDWORD roundrect);

		XDWORD																IsBlinking									();
		bool																	SetBlink										(XDWORD timeblink);
		bool																	GetStateBlink								();
		bool																	SwitchStateBlink					  ();
		XTIMER*																GetTimerBlink								();

		XDWORD																GetTransitionDuration				();
		bool																	SetTransitionDuration				(XDWORD milliseconds);
		bool																	IsTransitioning							();
		void																	UpdateTransition						();

		UI_ELEMENT_TRANSITION_STATE_SHOW			GetTransitionStateShow			();
		void																	SetTransitionStateShow			(UI_ELEMENT_TRANSITION_STATE_SHOW transitionstateshow);

		bool																	HasScroll										();
		void																	SetHasScroll								(bool hasscroll);

		XSTRING*															GetExtra										();	

		bool																	MustReDraw									();
		void																	SetMustReDraw								(bool mustredraw);
			
		XVECTOR<UI_ELEMENT*>*									GetComposeElements					();
		bool																	DeleteAllComposeElements		();

		bool																	SetToRedraw									(bool recursive);

		bool																	IsFlexContainer							();
		void																	SetFlexContainer						(bool isflexcontainer);
		UI_FLEX_DIRECTION										GetFlexDirection						();
		void																	SetFlexDirection						(UI_FLEX_DIRECTION flexdirection);
		UI_JUSTIFY_CONTENT										GetJustifyContent						();
		void																	SetJustifyContent					(UI_JUSTIFY_CONTENT justifycontent);
		double																GetRowGap										();
		double																GetColumnGap								();
		void																	SetGap											(double rowgap, double columngap);
		UI_FLEX_WRAP													GetFlexWrap									();
		void																	SetFlexWrap									(UI_FLEX_WRAP flexwrap);
		UI_ALIGN_CONTENT											GetAlignContent							();
		void																	SetAlignContent							(UI_ALIGN_CONTENT aligncontent);
		UI_ALIGN_ITEMS												GetAlignItems								();
		void																	SetAlignItems								(UI_ALIGN_ITEMS alignitems);

		double																GetFlexGrow									();
		void																	SetFlexGrow									(double flexgrow);
		double																GetFlexShrink								();
		void																	SetFlexShrink								(double flexshrink);
		UI_LAYOUTBOX_INSET										GetFlexBasis								();
		void																	SetFlexBasisAuto						();
		void																	SetFlexBasis								(double value);
		UI_ALIGN_SELF													GetAlignSelf								();
		void																	SetAlignSelf								(UI_ALIGN_SELF alignself);

		bool																	IsGridContainer							();
		void																	SetGridContainer						(bool isgridcontainer);
		void																	ClearGridColumnTracks				();
		void																	AddGridColumnTrack					(UI_GRIDTRACK track);
		XVECTOR<UI_GRIDTRACK>&									GetGridColumnTracks					();
		void																	ClearGridRowTracks					();
		void																	AddGridRowTrack							(UI_GRIDTRACK track);
		XVECTOR<UI_GRIDTRACK>&									GetGridRowTracks						();
		XDWORD																GetGridColumnSpan						();
		void																	SetGridColumnSpan						(XDWORD span);
		XDWORD																GetGridRowSpan							();
		void																	SetGridRowSpan							(XDWORD span);

		bool																	IsPressed										();
		void																	SetPressed									(bool ispressed);

		double																	GetIntrinsicWidth							();
		void																	SetIntrinsicWidth							(double intrinsicwidth);
		double																	GetIntrinsicHeight							();
		void																	SetIntrinsicHeight							(double intrinsicheight);

  private:

		void																	Clean												();

		XSTRING																name;
		UI_ELEMENT_TYPE												type;
		XSTRING																type_string;
		XSTRING																class_names_raw;
		XVECTOR<XSTRING*>											class_names;
		UI_ELEMENT_CHROMEROLE									chromerole;
		
		UI_ELEMENT*														father;
		bool																	isdetached;

		UI_LAYOUT*														element_layout;
		
		UI_COLOR															color;
		UI_COLOR															backgroundcolor;
		bool																	color_set;
		bool																	background_color_set;

		bool																	box_shadow_set;
		double																shadow_offset_x;
		double																shadow_offset_y;
		double																shadow_blur;
		UI_COLOR															shadow_color;

		double																x_position;
		double																y_position;

		XDWORD																z_level;

		UI_ELEMENT_TYPE_DIRECTION							direction;
		UI_ELEMENT_TYPE_ALIGN									textalign;

		double																x_positionwithscroll;
		double																y_positionwithscroll;

		UI_BOUNDARYLINE												boundaryline;

		double																margin[UI_ELEMENT_MARGIN_MAX];
		double																padding[UI_ELEMENT_MARGIN_MAX];
		double																border_width;

		UI_COLOR															border_color;
		bool																	border_color_set;
		double																border_radius[UI_ELEMENT_BORDER_CORNER_MAX];

		bool																	isactive;
		bool																	ispreselect;		
		bool																	isselected;
		bool																	isvisible;

		XDWORD																roundrect;	

		XDWORD																blink_time;
		bool																	blink_state;
		XTIMER*																blink_xtimer;
		XDWORD																blink_nchanges;

		XDWORD																style_transition_duration;
		bool																	style_transition_active;
		XTIMER*																style_transition_xtimer;
		UI_COLOR															style_transition_color_from;
		UI_COLOR															style_transition_color_to;
		UI_COLOR															style_transition_backgroundcolor_from;
		UI_COLOR															style_transition_backgroundcolor_to;

		UI_ELEMENT_TRANSITION_STATE_SHOW			transitionstateshow;

		bool																	hasscroll;

		bool																	mustredraw;

		XSTRING																extra;
		
		XVECTOR<UI_ELEMENT*>									compose_elements;

		bool																	css_flexcontainer;
		UI_FLEX_DIRECTION										css_flexdirection;
		UI_JUSTIFY_CONTENT										css_justifycontent;
		double																css_rowgap;
		double																css_columngap;
		UI_FLEX_WRAP													css_flexwrap;
		UI_ALIGN_CONTENT											css_aligncontent;
		UI_ALIGN_ITEMS												css_alignitems;

		double																css_flexgrow;
		double																css_flexshrink;
		UI_LAYOUTBOX_INSET										css_flexbasis;
		UI_ALIGN_SELF													css_alignself;

		double																intrinsic_width;
		double																intrinsic_height;

		UI_COLOR															snapshot_color;
		UI_COLOR															snapshot_backgroundcolor;
		XDWORD																snapshot_roundrect;

		double																snapshot_border_width;
		UI_COLOR															snapshot_border_color;
		bool																	snapshot_border_color_set;
		double																snapshot_border_radius[UI_ELEMENT_BORDER_CORNER_MAX];
		bool																	snapshot_box_shadow_set;
		double																snapshot_shadow_offset_x;
		double																snapshot_shadow_offset_y;
		double																snapshot_shadow_blur;
		UI_COLOR															snapshot_shadow_color;

		bool																	snapshot_taken;
		bool																	style_has_state_rules;

		UI_STYLE*															computed_style;

		bool																	css_gridcontainer;
		XVECTOR<UI_GRIDTRACK>									css_gridcolumntracks;
		XVECTOR<UI_GRIDTRACK>									css_gridrowtracks;
		XDWORD																css_gridcolumnspan;
		XDWORD																css_gridrowspan;

		bool																	ispressed;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





