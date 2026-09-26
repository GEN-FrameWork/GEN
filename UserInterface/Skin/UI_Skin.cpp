/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Skin.cpp
* 
* @class      UI_SKIN
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Skin.h"

#include "XTimer.h"

#include "GRPScreen.h"
#include "GRPBitmap.h"
#include "GRPBitmapFile.h"

#include "UI_Property_Scrolleable.h"
#include "UI_Animation.h"
#include "UI_Manager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- DIAGNOSTIC COUNTERS ------------------------------------------------------------------------------------------
*  Counts every UI_SKIN::Draw() call THIS FRAME -- top-level AND every recursive descendant call made from
*  Draw_Form()/Draw_Menu() etc, since those re-enter this exact same Draw() dispatcher for each child. Read and
*  reset once per frame from UI_SYSTEM::DrawFrame() (extern-declared there, no header change needed) -- kept here
*  (rather than removed outright) because that external reader still exists.
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD diagskin_visits  = 0;      // every Draw() call, including invisible elements that bail out immediately
XDWORD diagskin_visible = 0;      // calls where IsVisible() was true (i.e. did real administrative work)
XDWORD diagskin_dirty   = 0;      // calls where MustReDraw() was true (i.e. did real painting work)




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN_TEXTBOX_PART::UI_SKIN_TEXTBOX_PART()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN_TEXTBOX_PART::UI_SKIN_TEXTBOX_PART()
{
  Clean();
}

		
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN_TEXTBOX_PART::~UI_SKIN_TEXTBOX_PART()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN_TEXTBOX_PART::~UI_SKIN_TEXTBOX_PART()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_SKIN_TEXTBOX_PART::GetLineNumber()
* @brief      Get line number
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKIN_TEXTBOX_PART::GetLineNumber()
{
  return linenumber;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetLineNumber(XDWORD linenumber)
* @brief      Set line number
* @ingroup    USERINTERFACE
* 
* @param[in]  linenumber : Linenumber value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetLineNumber(XDWORD linenumber)
{
  this->linenumber = linenumber;
}

		
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_SKIN_TEXTBOX_PART::GetXPos()
* @brief      Get X pos
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKIN_TEXTBOX_PART::GetXPos()
{
  return xpos;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetXPos(XDWORD xpos)
* @brief      Set X pos
* @ingroup    USERINTERFACE
* 
* @param[in]  xpos : Xpos value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetXPos(XDWORD xpos)
{
  this->xpos = xpos;
}

		
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_SKIN_TEXTBOX_PART::GetYPos()
* @brief      Get Y pos
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKIN_TEXTBOX_PART::GetYPos()
{
  return ypos;
}	

	
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetYPos(XDWORD ypos)
* @brief      Set Y pos
* @ingroup    USERINTERFACE
* 
* @param[in]  ypos : Ypos value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetYPos(XDWORD ypos)
{
  this->ypos = ypos;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR* UI_SKIN_TEXTBOX_PART::GetColor()
* @brief      Get color
* @ingroup    USERINTERFACE
* 
* @return     UI_COLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_SKIN_TEXTBOX_PART::GetColor()
{
  return &color;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_SKIN_TEXTBOX_PART::GetWidth()
* @brief      Get width
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKIN_TEXTBOX_PART::GetWidth()
{
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetWidth(XDWORD width)
* @brief      Set width
* @ingroup    USERINTERFACE
* 
* @param[in]  width : Width value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetWidth(XDWORD width)
{
  this->width = width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_SKIN_TEXTBOX_PART::GetHeight()
* @brief      Get height
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKIN_TEXTBOX_PART::GetHeight()
{
  return height;
}
	

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetHeight(XDWORD height)
* @brief      Set height
* @ingroup    USERINTERFACE
* 
* @param[in]  height : Height value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetHeight(XDWORD height)
{
  this->height = height;
}

		
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_SKIN_TEXTBOX_PART::GetText()
* @brief      Get text
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_SKIN_TEXTBOX_PART::GetText()
{
  return &text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_SKIN_TEXTBOX_PART::GetImage()
* @brief      Get inline image (non-NULL means this part is an image, not text)
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_SKIN_TEXTBOX_PART::GetImage()
{
  return image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::SetImage(GRPBITMAP* image)
* @brief      Set inline image. The bitmap is owned by the manager's animation cache, so it is NOT freed by this part.
* @ingroup    USERINTERFACE
* 
* @param[in]  image : Image pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::SetImage(GRPBITMAP* image)
{
  this->image = image;
}

	
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN_TEXTBOX_PART::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN_TEXTBOX_PART::Clean()
{  
  linenumber  = 0;

  xpos        = 0;
  ypos        = 0;

  width       = 0;
  height      = 0;    

  image       = NULL;     // cache-owned bitmap; never freed here
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN::UI_SKIN()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN::UI_SKIN()    
{ 
  Clean();     
  
  this->name = UI_SKIN_NAME_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN::~UI_SKIN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN::~UI_SKIN()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_SKIN::GetName()
* @brief      Get name
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_SKIN::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN_TYPE UI_SKIN::GetType(XCHAR* name)
* @brief      Get type
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* 
* @return     UI_SKIN_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN_TYPE UI_SKIN::GetType(XCHAR* name)
{
  XSTRING _name;

  if(name) 
         _name = name;  
    else _name = this->name;

  if(_name.IsEmpty()) return UI_SKIN_TYPE_UNKNOWN;

  if(!_name.Compare(__L("flat"), true))       return UI_SKIN_TYPE_FLAT;

  return UI_SKIN_TYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKIN_DRAWMODE UI_SKIN::GetDrawMode()
* @brief      Get draw mode
* @ingroup    USERINTERFACE
* 
* @return     UI_SKIN_DRAWMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKIN_DRAWMODE UI_SKIN::GetDrawMode()
{
  return drawmode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::SetDrawMode(UI_SKIN_DRAWMODE drawmode)
* @brief      Set draw mode
* @ingroup    USERINTERFACE
* 
* @param[in]  drawmode : Drawmode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::SetDrawMode(UI_SKIN_DRAWMODE drawmode)
{
  this->drawmode  = drawmode;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_SKIN::GetRasterFont()
* @brief      Get raster font
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_SKIN::GetRasterFont()
{
  return &rasterfontname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_SKIN::GetVectorFont()
* @brief      Get vector font
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_SKIN::GetVectorFont()
{
  return &vectorfontname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::LoadFonts()
* @brief      Load fonts
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::LoadFonts()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKIN::GetWidthString(XCHAR* string, XDWORD sizefont)
* @brief      Get with string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKIN::GetWidthString(XCHAR* string, XDWORD sizefont)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKIN::GetWidthString(XSTRING& string, XDWORD sizefont)
* @brief      Get with string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKIN::GetWidthString(XSTRING& string, XDWORD sizefont)
{
  return GetWidthString(string.Get(), sizefont);
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKIN::GetHeightString(XCHAR* string, XDWORD sizefont)
* @brief      Get height string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKIN::GetHeightString(XCHAR* string, XDWORD sizefont)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKIN::GetHeightString(XSTRING& string, XDWORD sizefont)
* @brief      Get height string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKIN::GetHeightString(XSTRING& string, XDWORD sizefont)
{
  return GetHeightString(string.Get(), sizefont);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine(UI_ELEMENT* element, bool adjustsizemargin)
{
  if(!element) return false;

  bool status = false;

  switch(element->GetType())
    {
      case UI_ELEMENT_TYPE_UNKNOWN				  : return false;                                                               break;
	    case UI_ELEMENT_TYPE_SCROLL				    : status = CalculateBoundaryLine_Scroll(element, adjustsizemargin);             break;
	    case UI_ELEMENT_TYPE_TEXT					    : status = CalculateBoundaryLine_Text(element, adjustsizemargin);               break;
      case UI_ELEMENT_TYPE_TEXTBOX			    : status = CalculateBoundaryLine_TextBox(element, adjustsizemargin);            break;
	    case UI_ELEMENT_TYPE_IMAGE     			  : status = CalculateBoundaryLine_Image(element, adjustsizemargin);	            break;
      case UI_ELEMENT_TYPE_ANIMATION 			  : status = CalculateBoundaryLine_Animation(element, adjustsizemargin);	        break;
      case UI_ELEMENT_TYPE_OPTION					  : status = CalculateBoundaryLine_Option(element, adjustsizemargin);             break;
      case UI_ELEMENT_TYPE_MULTIOPTION		  : status = CalculateBoundaryLine_MultiOption(element, adjustsizemargin);        break;
	    case UI_ELEMENT_TYPE_BUTTON					  : status = CalculateBoundaryLine_Button(element, adjustsizemargin);             break;
      case UI_ELEMENT_TYPE_CHECKBOX 			  : status = CalculateBoundaryLine_CheckBox(element, adjustsizemargin);           break;
      case UI_ELEMENT_TYPE_EDITTEXT   		  : status = CalculateBoundaryLine_EditText(element, adjustsizemargin);           break;
      case UI_ELEMENT_TYPE_FORM						  : status = CalculateBoundaryLine_Form(element, adjustsizemargin);               break;
      case UI_ELEMENT_TYPE_MENU             : status = CalculateBoundaryLine_Menu(element, adjustsizemargin);               break;
      case UI_ELEMENT_TYPE_LISTBOX          : status = CalculateBoundaryLine_ListBox(element, adjustsizemargin);            break;
      case UI_ELEMENT_TYPE_PROGRESSBAR	    : status = CalculateBoundaryLine_ProgressBar(element, adjustsizemargin);        break;
      case UI_ELEMENT_TYPE_PROGRESSRADIAL    : status = CalculateBoundaryLine_ProgressRadial(element, adjustsizemargin);     break;
      case UI_ELEMENT_TYPE_PROGRESSIMAGE     : status = CalculateBoundaryLine_ProgressImage(element, adjustsizemargin);      break;
      case UI_ELEMENT_TYPE_STATISTICSCHART   : status = CalculateBoundaryLine_StatisticsChart(element, adjustsizemargin);    break;
    }

  // Phase 4 ("migración del ejemplo" -- footer icon/text gap regression fix). SCOPE ADDENDUM: this is the ONE
  // place every CalculateBoundaryLine_* builder funnels back through (element creation via UI_MANAGER::
  // GetLayoutElement_CalculateBoundaryLine(), AND every live re-measurement via UI_SKIN::
  // CalculeBoundaryLine_AllElements() -- see UI_MANAGER::ChangeTextElementValue()), so it is also the one place
  // that can safely snapshot "the size this element's own content/authoring actually wants" into
  // GetIntrinsicWidth()/Height(), independent of whatever UI_LAYOUTENGINE::WriteBackTree() may have written into
  // GetBoundaryLine() on a PREVIOUS flex pass.
  //
  // Root cause this fixes: UI_LAYOUTENGINE::BuildTree() used to read a flex item's flex-basis:auto "content
  // width/height" straight from GetBoundaryLine() -- fine the FIRST time RunLayout() ever ran, but WriteBackTree()
  // then overwrites that same BoundaryLine with the RESOLVED (grow/shrink-adjusted) size. UI_MANAGER::
  // ChangeTextElementValue() re-runs RunLayout() on every live text refresh (see its own Phase 4 comment), so a
  // flex row that is even slightly overflowing (freespace < 0, e.g. dashboard.xml's footer -- see its own
  // comment) re-shrinks EVERY item a little further on every single refresh, using the ALREADY-shrunk width as
  // the new "natural" size instead of the item's true one. Over the hundreds of refreshes a running app performs,
  // this geometrically converges every flex child (including fixed-width icons whose XML "width" never changed)
  // down to a fraction of its authored size -- confirmed by instrumenting a real windowed build: footer_equipo_
  // icon's BoundaryLine width decayed from 36 to ~12.7 over ~20s/~940 RunLayout() calls, dragging its sibling
  // caption's margin-left-computed x_position down with it into the near-zero visual gap reported against the
  // real app (unit tests never caught this because they call RunLayout() once, not repeatedly).
  //
  // Fix: UI_LAYOUTENGINE::BuildTree() (see its own SCOPE ADDENDUM) now prefers GetIntrinsicWidth()/Height() over
  // the live BoundaryLine for a flex item's content size when set, so RunLayout() is idempotent no matter how
  // many times it re-runs -- every pass starts, again, from this element's true authored/measured size.
  if(status)
    {
      element->SetIntrinsicWidth(element->GetBoundaryLine()->width);
      element->SetIntrinsicHeight(element->GetBoundaryLine()->height);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Scroll(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Scroll(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Text(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Text(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_TextBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line text box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_TextBox(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Image(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Image(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Animation(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line animation
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Animation(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Option(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Option(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_MultiOption(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line multi option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_MultiOption(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Button(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line button
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Button(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_CheckBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line check box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_CheckBox(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_EditText(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line edit text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_EditText(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Form(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line form
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Form(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_Menu(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line menu
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_Menu(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_ListBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line list box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_ListBox(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_ProgressBar(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line progress bar
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_ProgressBar(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_ProgressRadial(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line radial progress
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_ProgressRadial(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_ProgressImage(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      calculate boundary line  progress image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_ProgressImage(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculeBoundaryLine_AllElements(UI_ELEMENT* element, bool recursive)
* @brief      Calcule boundary line all elements
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculeBoundaryLine_AllElements(UI_ELEMENT* element, bool recursive)
{
  CalculateBoundaryLine(element);

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement) CalculeBoundaryLine_AllElements(subelement, recursive);                   
    } 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::GetAddPositionScrollSteps(UI_ELEMENT* element, double& x_positionwithscroll, double& y_positionwithscroll)
* @brief      Get add position scroll steps
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x_positionwithscroll : X positionwithscroll value.
* @param[in]  y_positionwithscroll : Y positionwithscroll value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::GetAddPositionScrollSteps(UI_ELEMENT* element, double& x_positionwithscroll, double& y_positionwithscroll)
{  
  if(!element) return false;

  UI_PROPERTY_SCROLLEABLE* property_scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
  if(property_scrolleable)
    {
      x_positionwithscroll += property_scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);   
      y_positionwithscroll += property_scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);   
    }
    
  if(element->GetFather()) GetAddPositionScrollSteps(element->GetFather(), x_positionwithscroll, y_positionwithscroll);      
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::SetElementPosition(UI_ELEMENT* element, double x_position, double y_position)
* @brief      Set element position
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::SetElementPosition(UI_ELEMENT* element, double x_position, double y_position)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
* @brief      Elements set to redraw
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  recursive : Recursive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Elements_SetToRedraw(UI_ELEMENT* element, bool recursive)
{
  if(!element) return false;

  bool status = true;

  element->SetMustReDraw(true);

  if(recursive)
    {
      for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
          if(subelement) 
            { 
              status = Elements_SetToRedraw(subelement, recursive);          
              
            } else break;
        }           
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw(UI_ELEMENT* element)
* @brief      Draw
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw(UI_ELEMENT* element)
{
  diagskin_visits++;

  if(!element) return false;

  // Modal layer (option B): while the opaque offscreen cache is valid, skip layout Draw of the modal root —
  // Element_DrawModalOnTop blits (and rebuilds offscreen when dirty) after content and chrome.
  if(GEN_USERINTERFACE.ModalLayer_IsLayoutDrawDeferred(element))
    {
      return true;
    }

  // Content under composed modal or visible custom caption must not paint (punch / title ghosts).
  if(GEN_USERINTERFACE.Overlay_SuppressesContentDraw(element))
    {
      element->SetMustReDraw(false);
      return true;
    }

  if(!element->IsVisible())
    {
      // HIDE-RESTORE FIX (2026-09): an element that just turned invisible (SetVisible(false), e.g.
      // UI_SYSTEM::UserInterface_SelectSection() hiding the PREVIOUS sidebar entry's selection band) is marked
      // dirty (MustReDraw()) by SetVisible() itself, but this early "invisible -> skip" return meant NEITHER
      // Draw_Form() (or any other Draw_XXX()) NOR its PostDrawFunction() ever ran again for it -- so its own
      // MustReDraw() flag was never cleared (confirmed live: logged dirty=true on EVERY single frame, forever,
      // after being hidden -- a permanent, harmless-but-wasteful poll) and, more importantly, its own generic
      // per-tick rebuild area (UI_SKINCANVAS_REBUILDAREAS) is never restored either, because that area is only
      // ever restored/deleted by code paths reached FROM this same Draw() call (PreDrawFunction()/
      // PostDrawFunction() inside Draw_Form() etc.), none of which run once we bail out here.
      //
      // That would still self-heal on its own IF the element's generic per-tick area happened to still exist
      // at the moment it was hidden -- but a nav-band that has been sitting selected and otherwise idle for a
      // while (the normal case: the user does not switch sections every frame) has long since had that area
      // ORPHAN-DISCARDED by RebuildAllAreas() (see its own orphan-discard comment: correct for an element that
      // stays VISIBLE, since its on-screen content remains correct -- but not accounted for the element then
      // being told to disappear entirely). Confirmed live on UI_System's sidebar: clicking "CPU" correctly
      // hides "nav-resumen-hl" (SetVisible(false) runs, MustReDraw() becomes true) and correctly shows the new
      // selection, but the OLD blue selection band stays painted on screen indefinitely -- nothing ever restores
      // the true panel background underneath it, because nothing ever runs the code that would.
      //
      // Fix: give the concrete skin (UI_SKINCANVAS below) one chance, right here, to restore its OWN persistent
      // "true backdrop" cache for this element -- the SAME cache (formbackdrops, for a "form") that Draw_Form()
      // itself already restores from on every ordinary VISIBLE redraw (see its own ALPHA-DARKENING FIX) -- and
      // do it only once, clearing MustReDraw() immediately after so this element stops polling every frame.
      // RestoreOnHide() defaults to a no-op (returns false) for any skin/element combination with no such cache,
      // so nothing changes for those -- this only activates the exact case that was silently broken.
      if(element->MustReDraw())
        {
          RestoreOnHide(element);
          element->SetMustReDraw(false);
        }

      return false;
    }

  diagskin_visible++;
  if(element->MustReDraw()) diagskin_dirty++;

  if(element->IsBlinking())
    {
      if(element->GetTimerBlink()->GetMeasureMilliSeconds() > element->IsBlinking()) 
        {
          element->SwitchStateBlink();                  
          Elements_SetToRedraw(element);
        }
       
      if(!element->GetStateBlink()) return false;
    }

  // Step 7 ("transiciones"): same polling idiom as the blink check just above. While a color/background-color
  // tween started by ReapplyStyleVisual() is still in flight, advance it by the elapsed time and keep asking
  // for a redraw every frame until it reaches its target.
  if(element->IsTransitioning())
    {
      element->UpdateTransition();
      Elements_SetToRedraw(element);
    }

  bool status = false;

  switch(element->GetType())
    {
      case UI_ELEMENT_TYPE_UNKNOWN				  : break;  
                             default        : break;
      case UI_ELEMENT_TYPE_SCROLL				    : status = Draw_Scroll(element);           break;
	    case UI_ELEMENT_TYPE_TEXT					    : status = Draw_Text(element);             break; 
      case UI_ELEMENT_TYPE_TEXTBOX  			  : status = Draw_TextBox(element);          break;	    
	    case UI_ELEMENT_TYPE_IMAGE     			  : status = Draw_Image(element);            break;
      case UI_ELEMENT_TYPE_ANIMATION 			  : status = Draw_Animation(element);	       break;
      case UI_ELEMENT_TYPE_OPTION					  : status = Draw_Option(element);           break;
      case UI_ELEMENT_TYPE_MULTIOPTION		  : status = Draw_MultiOption(element);      break;
	    case UI_ELEMENT_TYPE_BUTTON					  : status = Draw_Button(element);           break;	    	    
      case UI_ELEMENT_TYPE_CHECKBOX 			  : status = Draw_CheckBox(element);         break;	    	    
      case UI_ELEMENT_TYPE_EDITTEXT         : status = Draw_EditText(element);         break;	      
      case UI_ELEMENT_TYPE_FORM						  : status = Draw_Form(element);             break;
      case UI_ELEMENT_TYPE_MENU             : status = Draw_Menu(element);             break;             
      case UI_ELEMENT_TYPE_LISTBOX          : status = Draw_ListBox(element);          break;             
      case UI_ELEMENT_TYPE_PROGRESSBAR      : status = Draw_ProgressBar(element);      break;
      case UI_ELEMENT_TYPE_PROGRESSRADIAL    : status = Draw_ProgressRadial(element);   break;
      case UI_ELEMENT_TYPE_PROGRESSIMAGE     : status = Draw_ProgressImage(element);    break;
      case UI_ELEMENT_TYPE_STATISTICSCHART   : status = Draw_StatisticsChart(element);  break;

    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKIN::RestoreOnHide(UI_ELEMENT* element)
* @brief      HIDE-RESTORE FIX (2026-09): default, no-op hook -- see the call site in Draw() just above and the
*             UI_SKINCANVAS override in UI_SkinCanvas.cpp for the full rationale (restoring an element's own
*             persistent "true backdrop" cache, if it has one, the moment it turns invisible). A base UI_SKIN
*             (and any DRAWMODE_CONTEXT-style skin that does not maintain such a cache) has nothing to restore,
*             so this simply reports "nothing done" and Draw()'s caller proceeds exactly as it always did.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element that just turned invisible.
*
* @return     bool : true if a persistent backdrop was found and restored; false otherwise (never an error).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::RestoreOnHide(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKIN::Draw_Scroll(UI_ELEMENT* element)
* @brief      Draw scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Scroll(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Text(UI_ELEMENT* element)
* @brief      Draw text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Text(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_TextBox(UI_ELEMENT* element)
* @brief      Draw text box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_TextBox(UI_ELEMENT* element)  
{
  return false;
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Image(UI_ELEMENT* element)
* @brief      Draw image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Image(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Animation(UI_ELEMENT* element)
* @brief      Draw animation
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Animation(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Option(UI_ELEMENT* element)
* @brief      Draw option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Option(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_MultiOption(UI_ELEMENT* element)
* @brief      Draw multi option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_MultiOption(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Button(UI_ELEMENT* element)
* @brief      Draw button
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Button(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_CheckBox(UI_ELEMENT* element)
* @brief      Draw check box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_CheckBox(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_EditText(UI_ELEMENT* element)
* @brief      Draw edit text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_EditText(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Form(UI_ELEMENT* element)
* @brief      Draw form
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Form(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_Menu(UI_ELEMENT* element)
* @brief      Draw menu
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_Menu(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_ListBox(UI_ELEMENT* element)
* @brief      Draw list box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_ListBox(UI_ELEMENT* element)  
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_ProgressBar(UI_ELEMENT* element)
* @brief      Draw progress bar
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_ProgressBar(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_ProgressRadial(UI_ELEMENT* element)
* @brief      Draw radial progress
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_ProgressRadial(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_ProgressImage(UI_ELEMENT* element)
* @brief      draw  progress image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_ProgressImage(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::CalculateBoundaryLine_StatisticsChart(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line statistics chart
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::CalculateBoundaryLine_StatisticsChart(UI_ELEMENT* element, bool adjustsizemargin)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKIN::Draw_StatisticsChart(UI_ELEMENT* element)
* @brief      Draw statistics chart
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKIN::Draw_StatisticsChart(UI_ELEMENT* element)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKIN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKIN::Clean()
{
  drawmode           = UI_SKIN_DRAWMODE_UNKNOWN;  
}





