/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_SkinCanvas.h
* 
* @class      UI_SKINCANVAS
* @brief      User Interface Skin Canvas class
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

#include "XRect.h"
#include "XPath.h"

#include "GRP2DRebuildAreas.h"

#include "UI_Element.h"
#include "UI_Skin.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum UI_SKINCANVAS_TYPE
{
  UI_SKINCANVAS_TYPE_UNKNOWN          =  0  ,
  UI_SKINCANVAS_TYPE_FLAT                   ,
};


#define UI_SKINCANVAS_NAME_UNKNOWN					_L("")
#define UI_SKINCANVAS_NAME_FLAT							_L("FLAT")

#define UI_SKINCANVAS_PRESELECT_MAXEDGE		4
#define UI_SKINCANVAS_EDIT_MAXEDGE				10	

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSCREEN;
class GRPVIEWPORT;
class GRP2DCANVAS;
class GRP2DPATH;
class UI_LAYOUT;
class UI_ELEMENT_TEXTBOX;
class UI_ELEMENT_PROGRESSBAR;
class UI_PROPERTY_SCROLLEABLE;

class UI_SKINCANVAS_REBUILDAREAS : public GRP2DREBUILDAREAS
{
	public:
																		  UI_SKINCANVAS_REBUILDAREAS							(GRPSCREEN* screen, int viewportindex = 0);
    virtual													 ~UI_SKINCANVAS_REBUILDAREAS							();

		bool															RebuildAllAreas													();    
		bool															RebuildAllAreas													(UI_LAYOUT* layout);
		bool															RebuildAllAreas													(UI_ELEMENT* element);

    bool                              CreateRebuildArea												(double x, double y, double width, double height, UI_ELEMENT* element);

    GRPBITMAP*												GetBitmap																(double x, double y, double width, double height);
    void															PutBitmapNoAlpha												(double x, double y, GRPBITMAP* bitmap);

    void                              SetTargetCanvas                         (GRP2DCANVAS* newcanvas);
    GRP2DCANVAS*                      GetTargetCanvas                         ();
  
		GRP2DREBUILDAREA*									GetRebuildAreaByElement									(UI_ELEMENT* element);					

	private:

		void															Clean																		();

		void															MarkOverlappingAreasDirty							(GRP2DREBUILDAREA* area, GRPBITMAP* bitmap, int excludeindex);
		void															MarkElementSubtreeDirty									(UI_ELEMENT* element);

		GRPSCREEN*												screen;
		int																viewportindex;
		GRP2DCANVAS*											canvas;
};


// Plain last-known-bounds record for one progress-bar widget's element_progressrect/element_animation, keyed
// by the progress-bar element itself -- see the ProgressBounds_Find/Remember/HasChanged declarations and the
// GHOST-FILL FIX comment in Draw_ProgressBar() (UI_SkinCanvas.cpp). Deliberately a plain struct rather than
// reusing GRP2DREBUILDAREA: there is no bitmap or restore/discard lifecycle here, just four numbers per
// sub-element to compare against on the next tick.
class UI_PROGRESSBAR_LASTBOUNDS
{
	public:
																			UI_PROGRESSBAR_LASTBOUNDS	() : element(NULL), rectx(0.0), recty(0.0), rectwidth(0.0), rectheight(0.0),
																	                                     animx(0.0), animy(0.0), animwidth(0.0), animheight(0.0) { }

		UI_ELEMENT*								element;
		double										rectx;
		double										recty;
		double										rectwidth;
		double										rectheight;
		double										animx;
		double										animy;
		double										animwidth;
		double										animheight;
};

class UI_SKINCANVAS : public UI_SKIN, public UI_SKINCANVAS_REBUILDAREAS
{
  public:
																		  UI_SKINCANVAS														(GRPSCREEN* screen, int viewportindex = 0);
    virtual													 ~UI_SKINCANVAS														();

		static void												GetScrollViewportSize										(UI_ELEMENT* element, double& width, double& height);

		GRPSCREEN*                        GetScreen																(); 
		GRP2DCANVAS*                      GetCanvas																();
    void                              SetCanvasOverride                       (GRP2DCANVAS* override_canvas);

		double														GetPaintDensity													() const;
		void															SetPaintDensity													(double density);
		
		bool															LoadFonts																();

		bool															GetFatherSize														(UI_ELEMENT* element, double& width, double& height);
		double														GetPositionWithoutDefine								(double position);
		bool															SetAroundFromSubElements								(UI_ELEMENT* element);
		bool															AddPositionSubElements									(UI_ELEMENT* element);
		bool															CalculePosition													(UI_ELEMENT* element, double fatherwidth, double fatherheight, bool adjustsizemargin = false);

    double														GetWidthString													(XCHAR* string, XDWORD sizefont = 12);  
    double														GetHeightString													(XCHAR* string, XDWORD sizefont = 12);  
				           
	  virtual bool                      CalculateBoundaryLine_Scroll						(UI_ELEMENT* element, bool adjustsizemargin = false);
	  virtual bool                      CalculateBoundaryLine_Text							(UI_ELEMENT* element, bool adjustsizemargin = false);
	  virtual bool                      CalculateBoundaryLine_TextBox						(UI_ELEMENT* element, bool adjustsizemargin = false);
	  virtual bool                      CalculateBoundaryLine_Image   					(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_Animation					(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_Option						(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_MultiOption				(UI_ELEMENT* element, bool adjustsizemargin = false);
	  virtual bool                      CalculateBoundaryLine_Button						(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_CheckBox					(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_EditText					(UI_ELEMENT* element, bool adjustsizemargin = false);	  
	  virtual bool                      CalculateBoundaryLine_Form							(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_Menu							(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_ListBox						(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_ProgressBar				(UI_ELEMENT* element, bool adjustsizemargin = false);
    virtual bool                      CalculateBoundaryLine_ProgressRadial		(UI_ELEMENT* element, bool adjustsizemargin = false);
		virtual bool                      CalculateBoundaryLine_ProgressImage			(UI_ELEMENT* element, bool adjustsizemargin = false);

		void                               ReapplyProgressBarAllocationLayout			(UI_ELEMENT_PROGRESSBAR* element_progressbar, bool adjustsizemargin = false);

		virtual bool                      SetElementPosition											(UI_ELEMENT* element, double x_position, double y_position);

		virtual bool                      RestoreOnHide														(UI_ELEMENT* element);

		void                               InvalidateCompositionCaches							();

		XDWORD                             CompositionCacheCount									();

	  virtual bool                      Draw_Scroll															(UI_ELEMENT* element);
	  virtual bool                      Draw_Text																(UI_ELEMENT* element);
		virtual bool											Draw_TextBox														(UI_ELEMENT* element);
	  virtual bool                      Draw_Image  														(UI_ELEMENT* element);
		virtual bool                      Draw_Animation													(UI_ELEMENT* element);
		virtual bool                      Draw_Option															(UI_ELEMENT* element);
		virtual bool                      Draw_MultiOption												(UI_ELEMENT* element);
	  virtual bool                      Draw_Button															(UI_ELEMENT* element);
		virtual bool											Draw_CheckBox														(UI_ELEMENT* element);
		virtual bool											Draw_EditText														(UI_ELEMENT* element);		
	  virtual bool                      Draw_Form																(UI_ELEMENT* element);
		virtual bool											Draw_Menu																(UI_ELEMENT* element);
		virtual bool											Draw_ListBox														(UI_ELEMENT* element);
		virtual bool											Draw_ProgressBar												(UI_ELEMENT* element);
    virtual bool                      Draw_ProgressRadial											(UI_ELEMENT* element);
    virtual bool                      Draw_ProgressImage											(UI_ELEMENT* element);
    virtual bool                      Draw_StatisticsChart										(UI_ELEMENT* element);

  protected: 

		bool															GetFontSize															(XCHAR* text, XDWORD& width, XDWORD& height);
		bool															SetFontSize															(XDWORD size);

		bool															DrawBackgroundColor											(UI_ELEMENT* element, GRP2DCANVAS* canvas, double x_position, double y_position);

		static void												AppendRoundRectPathPerCorner						(GRP2DPATH& path, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL);
		static void												DrawElementBoxShadow										(GRP2DCANVAS* canvas, UI_ELEMENT* element, double x_position, double y_position);

		bool															PreDrawFunction													(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double& x_position, double& y_position, XDWORD edge = 5);
		bool															PostDrawFunction												(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double x_position, double y_position);

		bool															DrawScrollBars													(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable, GRP2DCANVAS* canvas, double x_position, double y_position);
		bool															ResolveScrollPolicy											(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable);
			
		double														TextBox_SizeLine												(UI_ELEMENT_TEXTBOX* element_textbox, GRP2DCANVAS* canvas, double x_position, double y_position, int nline, XSTRING& characterstr, XDWORD index_char, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts);
		bool															TextBox_GenerateLines										(UI_ELEMENT_TEXTBOX* element, GRP2DCANVAS* canvas, double x_position, double y_position, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts); 

		#ifdef USERINTERFACE_DEBUG
		bool															Debug_Draw															(UI_ELEMENT* element, double x_position, double y_position);	
		#endif

		XDWORD														fontsize;
    GRPSCREEN*												screen;
	  int																viewportindex;
		XPATH															fontpathfile;
    GRP2DCANVAS*                      canvas_override;
		double														paint_density;

	private:

		void															Clean																		();		

		GRP2DREBUILDAREA*								ProgressBackdrop_Find										(UI_ELEMENT* element);
		bool													ProgressBackdrop_Capture								(UI_ELEMENT* element, double x, double y, double width, double height);

		XVECTOR<GRP2DREBUILDAREA*>				progressbackdrops;

		UI_PROGRESSBAR_LASTBOUNDS*				ProgressBounds_Find											(UI_ELEMENT* element);
		void													ProgressBounds_Remember								(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight);
		bool													ProgressBounds_HasChanged							(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight);

		XVECTOR<UI_PROGRESSBAR_LASTBOUNDS*>	progressbarlastbounds;

		GRP2DREBUILDAREA*								FormBackdrop_Find												(UI_ELEMENT* element);
		bool													FormBackdrop_Capture										(UI_ELEMENT* element, double x, double y, double width, double height);

		bool													FormBackdrop_MatchesArea								(GRP2DREBUILDAREA* formbackdrop, double x, double y, double width, double height);
		bool													FormBackdrop_Delete										(UI_ELEMENT* element);

		GRP2DREBUILDAREA*								FormBackdrop_FindOverlapping						(double x, double y, double width, double height);

		XVECTOR<GRP2DREBUILDAREA*>				formbackdrops;

		XVECTOR<UI_ELEMENT*>					formhiddentracked;

		GRP2DREBUILDAREA*								RadialBackdrop_Find											(UI_ELEMENT* element);
		bool													RadialBackdrop_Capture									(UI_ELEMENT* element, double x, double y, double width, double height);

		XVECTOR<GRP2DREBUILDAREA*>				radialbackdrops;

		GRP2DREBUILDAREA*								TextBackdrop_Find											(UI_ELEMENT* element);
		bool													TextBackdrop_MatchesArea								(GRP2DREBUILDAREA* textbackdrop, GRP2DREBUILDAREA* ownarea);
		bool													TextBackdrop_Delete										(UI_ELEMENT* element);
		bool													TextBackdrop_Capture										(UI_ELEMENT* element, double x, double y, double width, double height);

		bool													TextBackdrop_InvalidateOverlapping					(double x, double y, double width, double height);

		XVECTOR<GRP2DREBUILDAREA*>				textbackdrops;

		GRP2DREBUILDAREA*								OptionBackdrop_Find											(UI_ELEMENT* element);
		bool													OptionBackdrop_Delete										(UI_ELEMENT* element);
		bool													OptionBackdrop_Capture									(UI_ELEMENT* element, double x, double y, double width, double height);
		bool													OptionBackdrop_InvalidateOverlapping					(double x, double y, double width, double height, bool force = false);
		GRP2DREBUILDAREA*								OptionBackdrop_FindOverlapping							(double x, double y, double width, double height);

		XVECTOR<GRP2DREBUILDAREA*>				optionbackdrops;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




