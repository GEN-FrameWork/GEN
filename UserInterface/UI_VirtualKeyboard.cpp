/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_VirtualKeyboard.cpp
* 
* @class      UI_VIRTUALKEYBOARD
* @brief      User Interface Virtual Keyboard class
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

#include "UI_VirtualKeyboard.h"

#include "GRPScreen.h"
#include "GRP2DCanvas.h"
#include "GRPBitmapFile.h"

#include "UI_Animation.h"
#include "UI_Animations.h"
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "UI_SkinCanvas.h"
#include "UI_Element_Image.h"
#include "UI_Element_Text.h"
#include "UI_Element_EditText.h"
#include "UI_Element_MultiOption.h"
#include "UI_Element_Option.h"
#include "UI_Element_Form.h"
#include "UI_Manager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_VIRTUALKEYBOARD_KEYINFO::UI_VIRTUALKEYBOARD_KEYINFO()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_VIRTUALKEYBOARD_KEYINFO::UI_VIRTUALKEYBOARD_KEYINFO()    
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_VIRTUALKEYBOARD_KEYINFO::~UI_VIRTUALKEYBOARD_KEYINFO()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_VIRTUALKEYBOARD_KEYINFO::~UI_VIRTUALKEYBOARD_KEYINFO()
{
  Clean();
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_VIRTUALKEYBOARD_KEYINFO::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_VIRTUALKEYBOARD_KEYINFO::Clean()
{
  color.Empty();
  sizefont = 0;
  margin   = 0;
  xpos     = 0;
  ypos     = 0;
  width    = 0;
  height   = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_VIRTUALKEYBOARD::UI_VIRTUALKEYBOARD()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_VIRTUALKEYBOARD::UI_VIRTUALKEYBOARD()
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_VIRTUALKEYBOARD::~UI_VIRTUALKEYBOARD()
* @brief      Destructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_VIRTUALKEYBOARD::~UI_VIRTUALKEYBOARD()
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::Ini(UI_LAYOUT* layout, GRPSCREEN* screen)
* @brief      Initialize the object
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* @param[in]  screen : Screen object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::Ini(UI_LAYOUT* layout, GRPSCREEN* screen)
{
  if(main_form) 
    {
      return false;
    }
 
  this->layout = layout;  
  this->screen = screen; 
  this->skin   = layout->GetSkin(); 

  main_form = GEN_NEW UI_ELEMENT_FORM();
  if(!main_form) return false;

  main_form->GetName()->Set(_L("Virtual Keyboard"));

  main_form->GetBoundaryLine()->x = 0;
  main_form->GetBoundaryLine()->y = 0;

  main_form->GetBoundaryLine()->width  = screen->GetWidth()-1;
  main_form->GetBoundaryLine()->height = (screen->GetHeight()-1)/2;

  main_form->GetVisibleRect()->CopyFrom((*main_form->GetBoundaryLine()));
    
  UI_COLOR* bckgcolor = main_form->GetColor();  
  UI_COLOR* linecolor = main_form->GetLineColor();
     
  if(bckgcolor) bckgcolor->SetFromString(_L("200,200,200,255"));
  if(linecolor) linecolor->SetFromString(_L("100,100,100,150"));

  main_form->SetVisible(false);
 
  layout->Elements_Add(main_form);

  CreateAllKeys(0 , screen->GetHeight());

  width  =  main_form->GetBoundaryLine()->width;
  height =  main_form->GetBoundaryLine()->height;

  actualset =  UI_VIRTUALKEYBOARD_SET_UPPERCASE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::IsShow()
* @brief      Is show
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::IsShow()
{
  return isshow;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_VIRTUALKEYBOARD::GetElementEditable()
* @brief      Get element editable
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_VIRTUALKEYBOARD::GetElementEditable()
{
  return element_editable;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_EDITTEXT* UI_VIRTUALKEYBOARD::GetElementInput()
* @brief      Live edit field hosted inside the keyboard form (above the keys).
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_EDITTEXT* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_EDITTEXT* UI_VIRTUALKEYBOARD::GetElementInput()
{
  return element_input;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::IsOwnElement(UI_ELEMENT* element)
* @brief      True when element is the keyboard form or a descendant (keys / input edit).
* @ingroup    USERINTERFACE
* 
* @param[in]  element : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::IsOwnElement(UI_ELEMENT* element)
{
  if(!element || !main_form) return false;

  UI_ELEMENT* walk = element;
  while(walk)
    {
      if(walk == main_form)      return true;
      if(walk == element_input)  return true;
      walk = walk->GetFather();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::Show(bool on, UI_ELEMENT* element_editable)
* @brief      Show
* @ingroup    USERINTERFACE
* 
* @param[in]  on : On value.
* @param[in]  element_editable : Element editable pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::Show(bool on, UI_ELEMENT* element_editable)
{
  if(!main_form)  return false;

  double x = 0;
  double y = 0;

  if(on)
    {
      if(!element_editable)  return false;

      // Selecting the in-keyboard input must not restart the session.
      if(IsOwnElement(element_editable)) return true;

      // Preserve the live input when recreating keys (CAPS / SYMBOLS toggle).
      XSTRING keep_text;
      XDWORD  keep_cursor = 0;
      bool    keep_input  = false;

      if(isshow && element_input)
        {
          keep_text.Set(element_input->GetText()->Get());
          keep_cursor = element_input->Cursor_GetPosition();
          keep_input  = true;
        }
       else
        {
          contentchanged = false;
        }

      this->element_editable = element_editable;

      // Android-style dock: full window width, bottom edge of the screen (not beside the source edit).
      x = 0;
      y = (double)screen->GetHeight();

      // Do not rebuild only the pending dirty areas here.
      // The virtual keyboard is a modal layer and it changes the global composition of the frame.  Some existing
      // controls in the layout are semi-transparent and may not have a valid saved rebuild area at this exact moment.
      // If they are redrawn over the already-composited frame, their alpha is accumulated and artifacts appear
      // outside the keyboard area (for example in the lower horizontal menu of UI_Options).
      //
      // The safe transition is: rebuild the complete frame from the layout background and then redraw every element
      // once with the keyboard already visible.
      GEN_USERINTERFACE.SetPreselectElement();

      DeleteAllKeys();
      CreateAllKeys(x, y);

      width  = main_form->GetBoundaryLine()->width;
      height = main_form->GetBoundaryLine()->height;

      main_form->SetVisible(true);
      GEN_USERINTERFACE.Element_SetModal(main_form);
      GEN_USERINTERFACE.ModalLayer_Invalidate();

      if(element_input)
        {
          if(keep_input)
            {
              element_input->GetText()->Set(keep_text.Get());
              element_input->Cursor_SetPosition(keep_cursor);
            }
           else if(element_editable->GetType() == UI_ELEMENT_TYPE_EDITTEXT)
            {
              UI_ELEMENT_EDITTEXT* src = (UI_ELEMENT_EDITTEXT*)element_editable;
              XDWORD maxsizetext = src->GetMaxSizeText();
              XDWORD sizetext    = src->GetText()->GetSize();

              if(!maxsizetext) maxsizetext = (sizetext ? sizetext : 32);

              element_input->SetMaxSizeText(maxsizetext);
              element_input->GetText()->Set(src->GetText()->Get());

              if(element_input->GetText()->GetSize() > maxsizetext)
                {
                  element_input->GetText()->AdjustSize(maxsizetext);
                }

              element_input->Cursor_SetPosition(element_input->GetText()->GetSize());
            }

          element_input->Cursor_SetVisible(true);
        }

      // Original layout edit keeps its text until RETURN commits; hide its cursor while the keyboard owns input.
      if(element_editable->GetType() == UI_ELEMENT_TYPE_EDITTEXT)
        {
          UI_ELEMENT_EDITTEXT* src = (UI_ELEMENT_EDITTEXT*)element_editable;
          if(src) src->Cursor_SetVisible(false);
        }

      isshow = true;
    }
   else
    {
      if(element_input)
        {
          element_input->Cursor_SetVisible(false);
        }

      if(this->element_editable)
        {
          if(this->element_editable->GetType() == UI_ELEMENT_TYPE_EDITTEXT)
            {
              UI_ELEMENT_EDITTEXT* element_edittext = (UI_ELEMENT_EDITTEXT*)this->element_editable;
              if(element_edittext) element_edittext->Cursor_SetVisible(false);
            }
        }

      actualset              = UI_VIRTUALKEYBOARD_SET_UPPERCASE;
      this->element_editable = NULL;

      GEN_USERINTERFACE.Element_SetModal(NULL);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      main_form->SetVisible(false);

      isshow = false;
    }

  // Force a full repaint for the layout that owns the keyboard. This clears the already-composited screen with the
  // layout background before the next Update() draws the transparent controls again. It also invalidates every element
  // so no stale dirty rectangle is reused after the modal visibility transition.
  // Layout_PutBackground() also drops the skin's persistent true-backdrop caches (InvalidateCompositionCaches) so
  // Draw_* cannot PutBitmapNoAlpha-restore parchment captured before this wipe.
  if(layout)
    {
      GEN_USERINTERFACE.Layout_PutBackground(layout->GetNameID()->Get());

      // Belt-and-suspenders: keyboard Show is the known producer of stale menu/chrome rectangles. Re-drop caches
      // on this layout's skin even if a future PutBackground helper path forgets to invalidate.
      UI_SKIN* skin = layout->GetSkin();
      if(skin && skin->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
        {
          UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)skin;
          if(skin_canvas) skin_canvas->InvalidateCompositionCaches();
        }
    }
   else
    {
      GEN_USERINTERFACE.Layout_PutBackground();
    }

  GEN_USERINTERFACE.Elements_SetToRedraw();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::SelectInput(UI_ELEMENT* key_select)
* @brief      Select input
* @ingroup    USERINTERFACE
* 
* @param[in]  key_select : Key select pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::SelectInput(UI_ELEMENT* key_select)
{
  XCHAR* literals[] =  { _L("RETURN")        ,  // 0
                          _L("CAPS")         ,  // 1
                          _L("SYMBOLS")      ,  // 2
                          _L("BACK")         ,  // 3
                          _L("ARROWLEFT")    ,  // 4
                          _L("ARROWRIGHT")   ,  // 5
                          _L("SPACE")        ,  // 6
                          _L("ESC")          ,  // 7
                        };

  if(!key_select) return false;

  XSTRING elementID;
  XSTRING keystr;

  // All typing goes to the in-keyboard input field; the layout edit is only updated on RETURN.
  UI_ELEMENT_EDITTEXT* element_edittext = element_input;
  if(!element_edittext) return false;

  elementID = UI_VIRTUALKEYBOARD_ELEMENTID;

  int index = key_select->GetName()->Find(elementID, true);
  if(index == XSTRING_NOTFOUND) return false;
 
  index += elementID.GetSize();

  key_select->GetName()->Copy(index, keystr);

  int index_literal = -1;
  for(int c=0; c<(sizeof(literals) / sizeof(XCHAR*)); c++)
    {
      if(!keystr.Compare(literals[c], true)) 
        {
          index_literal = c;
          break;
        }
    }
  
  switch(index_literal)
    {
      case  0 : //-------------------------------------------------------      
                // RETURN — commit live input into the layout edit that opened the keyboard.
                {
                  CommitInputToEditable();

                  if(contentchanged)
                    {
                      GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS, element_editable); 
                      contentchanged = false;
                    }

                  Show(false, NULL);
                }
                break;      
    

      case  1 : //-------------------------------------------------------      
                // CAPS

                { bool   _contentchanged = contentchanged;
                  XDWORD cursor_position = element_edittext->Cursor_GetPosition(); 

                  if(actualset == UI_VIRTUALKEYBOARD_SET_UPPERCASE)  
                         actualset =  UI_VIRTUALKEYBOARD_SET_LOWERCASE; 
                    else actualset =  UI_VIRTUALKEYBOARD_SET_UPPERCASE;
      
                  Show(true, element_editable);

                  contentchanged = _contentchanged;
                  if(element_input) element_input->Cursor_SetPosition(cursor_position); 
                }
                break;

      case  2 : //-------------------------------------------------------      
                //  SYMBOLS

                { bool   _contentchanged = contentchanged;
                  XDWORD cursor_position = element_edittext->Cursor_GetPosition(); 

                  if(actualset == UI_VIRTUALKEYBOARD_SET_SYMBOLS)  
                          actualset =  UI_VIRTUALKEYBOARD_SET_UPPERCASE; 
                    else  actualset =  UI_VIRTUALKEYBOARD_SET_SYMBOLS;
    
                  Show(true, element_editable);
                  
                  contentchanged = _contentchanged;
                  if(element_input) element_input->Cursor_SetPosition(cursor_position); 
                }
                break;

      case  3 : //-------------------------------------------------------      
                // BACK

                { XDWORD position = element_edittext->Cursor_GetPosition();
                  if(position) 
                    { 
                      element_edittext->GetText()->DeleteCharacters(position-1, 1); 
                      element_edittext->Cursor_SetPosition(position-1); 

                      contentchanged = true;             
                    }
                }
                break;

      case  4 : //-------------------------------------------------------      
                //  ARROW LEFT

                { XDWORD position = element_edittext->Cursor_GetPosition();
      
                  if(position)  position--;
  
                  element_edittext->Cursor_SetPosition(position);
                }
                break;

      case  5 : //-------------------------------------------------------      
                // ARROW RIGHT  

                {
                  XDWORD position = element_edittext->Cursor_GetPosition();
      
                  position++;
                  if(position > element_edittext->GetText()->GetSize())  position = element_edittext->GetText()->GetSize();
  
                  element_edittext->Cursor_SetPosition(position);
                }
                break;

      case  6 : //-------------------------------------------------------      
                // SPACE  

                {
                  keystr = _L(" ");
                } 
                // fall through

      default : //-------------------------------------------------------      
                // Other key (or SPACE)

                { 
                  XDWORD maxsizetext  = element_edittext->GetMaxSizeText();
                  XDWORD position     = element_edittext->Cursor_GetPosition();
                  XDWORD textsizetext = element_edittext->GetText()->GetSize();

                  if(!maxsizetext || (textsizetext + 1 <= maxsizetext))
                    { 
                      element_edittext->GetText()->Insert(keystr, position);   
                      element_edittext->Cursor_SetPosition(position+1);   

                      contentchanged = true;
                    }
                }                
                break;

      case  7 : //-------------------------------------------------------
                // ESC — close without modifying the layout edit that opened the keyboard.
                {
                  contentchanged = false;
                  Show(false, NULL);
                }
                break;


    }
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::CommitInputToEditable()
* @brief      Copy the in-keyboard input text into the layout edit that opened the session.
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::CommitInputToEditable()
{
  if(!element_input || !element_editable) return false;
  if(element_editable->GetType() != UI_ELEMENT_TYPE_EDITTEXT) return false;

  UI_ELEMENT_EDITTEXT* dst = (UI_ELEMENT_EDITTEXT*)element_editable;

  dst->GetText()->Set(element_input->GetText()->Get());

  XDWORD maxsizetext = dst->GetMaxSizeText();
  if(maxsizetext && dst->GetText()->GetSize() > maxsizetext)
    {
      dst->GetText()->AdjustSize(maxsizetext);
    }

  dst->Cursor_SetPosition(dst->GetText()->GetSize());
  GEN_USERINTERFACE.Elements_SetToRedraw(dst);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::End()
* @brief      End the object
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::End()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ANIMATION* UI_VIRTUALKEYBOARD::AddImageCache(XCHAR* name, XCHAR* resource)
* @brief      Add image cache
* @ingroup    USERINTERFACE
* 
* @param[in]  name : Name to use.
* @param[in]  resource : Resource pointer to use.
* 
* @return     UI_ANIMATION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ANIMATION* UI_VIRTUALKEYBOARD::AddImageCache(XCHAR* name, XCHAR* resource)
{
  UI_ANIMATION* animation = NULL;

  if(!skin) return NULL;

  animation = GEN_UI_ANIMATIONS.Get(name);
  if(animation) return animation;

  animation = GEN_UI_ANIMATIONS.Get(resource);
  if(animation) return animation;

  animation = GEN_NEW UI_ANIMATION();
  if(!animation) return NULL;
  
  XSTRING resourcename;
  bool    status = false;

  resourcename = resource;

  switch(skin->GetDrawMode())
    {
      case UI_SKIN_DRAWMODE_UNKNOWN   : break;

      case UI_SKIN_DRAWMODE_CANVAS    : { UI_SKINCANVAS* skincanvas = (UI_SKINCANVAS*)skin; 
                                          if(skincanvas) status = animation->LoadFromFile(resourcename, skincanvas->GetCanvas()->GetMode());                                                                           
                                        }
                                        break;

      case UI_SKIN_DRAWMODE_CONTEXT   : break;
    } 

  if(status)   
    {      
      animation->GetName()->Set(name);  
      animation->GetResource()->Set(resource);

      GEN_UI_ANIMATIONS.Add(animation);

      return animation;
    }
  
  GEN_DELETE animation;

  return NULL;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_VIRTUALKEYBOARD::LoadKeyImage(XCHAR* pathimage)
* @brief      Load key image
* @ingroup    USERINTERFACE
* 
* @param[in]  pathimage : Pathimage pointer to use.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_VIRTUALKEYBOARD::LoadKeyImage(XCHAR* pathimage)
{
  UI_ANIMATION* animation = NULL;
  GRPBITMAP*    bitmap    = NULL;  

  animation = AddImageCache(_L(""), pathimage);
  if(animation) bitmap = animation->GetBitmap();
    
  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::AddKeyButton(XCHAR* leyend, XCHAR* text, XCHAR* xpathbitmap, UI_VIRTUALKEYBOARD_KEYINFO& keyinfo)
* @brief      Add key button
* @ingroup    USERINTERFACE
* 
* @param[in]  leyend : Leyend pointer to use.
* @param[in]  text : Text to use.
* @param[in]  xpathbitmap : Xpathbitmap pointer to use.
* @param[in]  keyinfo : Keyinfo value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::AddKeyButton(XCHAR* leyend, XCHAR* text, XCHAR* xpathbitmap, UI_VIRTUALKEYBOARD_KEYINFO& keyinfo)
{  
  UI_ELEMENT_MULTIOPTION* element_multioption = GEN_NEW UI_ELEMENT_MULTIOPTION();
  if(!element_multioption) return false;

  for(int c=0; c<4; c++)
    { 
      UI_ELEMENT_OPTION*            element_option  = GEN_NEW UI_ELEMENT_OPTION();
      UI_PROPERTY_SELECTABLE_STATE  state           = (UI_PROPERTY_SELECTABLE_STATE)c;
      if(!element_option) return false;
  
      if(text)
        {  
          UI_ELEMENT_TEXT* element_text = GEN_NEW UI_ELEMENT_TEXT();
          if(element_text) 
            {          
              element_text->SetFather(element_option);
              element_text->GetName()->Add(UI_VIRTUALKEYBOARD_ELEMENTID);
              element_text->GetName()->Add(leyend);
              element_text->GetText()->Set(text);
      
              // Width follows the visible label. Height must NOT: glyphs with diacritics (Ñ, ñ) report a taller
              // box, and centering that box lifts the letter body above A–Z. Use a Latin reference height so
              // every key shares the same vertical slot.
              double label_w = skin->GetWidthString(text, keyinfo.sizefont);
              double label_h = skin->GetHeightString(_L("M"), keyinfo.sizefont);
              if(label_h <= 0.0) label_h = skin->GetHeightString(text, keyinfo.sizefont);

              element_text->GetBoundaryLine()->width  = label_w;
              element_text->GetBoundaryLine()->height = label_h;
              element_text->GetBoundaryLine()->x      = ((keyinfo.width  - label_w) / 2);
              element_text->GetBoundaryLine()->y      = ((keyinfo.height - label_h) / 2);         
 
              if(state == UI_PROPERTY_SELECTABLE_STATE_PRESELECT)
                {
                  element_text->GetColor()->SetFromString(_L("red,255"));
                }
               else 
                {
                  if(keyinfo.color.GetSize()) element_text->GetColor()->SetFromString(keyinfo.color);
                }

              element_text->SetSizeFont(keyinfo.sizefont);
                      
              element_option->Set_UIText(element_text);              

              GEN_USERINTERFACE.SetLevelAuto(element_text, element_option);   

              element_option->GetComposeElements()->Add(element_text);              
            }
        }

      if(xpathbitmap)
        {     
          UI_ELEMENT_ANIMATION* element_animation = GEN_NEW UI_ELEMENT_ANIMATION();
          if(element_animation) 
            {                  
              GRPBITMAP* bitmap = NULL;

              element_animation->SetFather(element_option);
              element_animation->GetName()->Add(UI_VIRTUALKEYBOARD_ELEMENTID);
              element_animation->GetName()->Add(leyend);
          
              UI_ELEMENT_IMAGE* element_image = GEN_NEW UI_ELEMENT_IMAGE();
              if(element_image) 
                {                  
                  bitmap = LoadKeyImage(xpathbitmap);
                  if(bitmap)
                    {
                      element_image->SetImage(bitmap);
                      element_animation->GetComposeElements()->Add(element_image);                                                                                  
                    }
                }

              if(bitmap) 
                {
                  element_image->SetFather(element_animation); 

                  element_image->GetBoundaryLine()->width       = bitmap->GetWidth();
                  element_image->GetBoundaryLine()->height      = bitmap->GetHeight();
                  element_image->GetBoundaryLine()->x           = 0; //keyinfo.xpos + ((keyinfo.width  - element_image->GetBoundaryLine()->width)  / 2); 
                  element_image->GetBoundaryLine()->y           = 0; //keyinfo.ypos - ((keyinfo.height - element_image->GetBoundaryLine()->height) / 2);      

                  element_animation->GetBoundaryLine()->width   = bitmap->GetWidth();
                  element_animation->GetBoundaryLine()->height  = bitmap->GetHeight();
                  element_animation->GetBoundaryLine()->x       = 0; //keyinfo.xpos + ((keyinfo.width  - element_animation->GetBoundaryLine()->width)  / 2);
                  element_animation->GetBoundaryLine()->y       = 0; //keyinfo.ypos - ((keyinfo.height - element_animation->GetBoundaryLine()->height) / 2);      
                  
                  element_option->Set_UIAnimation(element_animation);
                                    
                  GEN_USERINTERFACE.SetLevelAuto(element_animation, element_option);   

                  element_option->GetComposeElements()->Add(element_animation);               
                }
               else
                {
                  GEN_DELETE element_image;    
                  GEN_DELETE element_animation;    
                } 
            }
        }
                 
      element_option->SetFather(element_multioption);
      element_option->GetName()->Add(UI_VIRTUALKEYBOARD_ELEMENTID);
      element_option->SetSelectableState(state);
      element_option->GetName()->Add(leyend);      
      element_option->GetBoundaryLine()->x      = 0; //keyinfo.xpos;
      element_option->GetBoundaryLine()->y      = 0; //keyinfo.ypos;         
      element_option->GetBoundaryLine()->width  = keyinfo.width;
      element_option->GetBoundaryLine()->height = keyinfo.height;
      
      GEN_USERINTERFACE.SetLevelAuto(element_option, element_multioption);     

      element_multioption->GetComposeElements()->Add(element_option);     
    }

  element_multioption->SetFather(main_form);
  element_multioption->GetName()->Add(UI_VIRTUALKEYBOARD_ELEMENTID);
  element_multioption->GetName()->Add(leyend);  
  element_multioption->SetActive(true);  
  element_multioption->GetBoundaryLine()->x      = keyinfo.xpos;
  element_multioption->GetBoundaryLine()->y      = keyinfo.ypos;         
  element_multioption->GetBoundaryLine()->width  = keyinfo.width;
  element_multioption->GetBoundaryLine()->height = keyinfo.height;

  GEN_USERINTERFACE.SetLevelAuto(element_multioption, main_form);
                
  main_form->GetComposeElements()->Add(element_multioption);        

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::CreateAllKeys(double x, double y)
* @brief      Create all keys
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::CreateAllKeys(double x, double y)
{
  #define UI_VIRTUALKEYBOARD_MARGIN   40
 
  XSTRING line[3][3][10] =  { { { _L("Q"), _L("W"), _L("E"), _L("R"), _L("T"), _L("Y"), _L("U"), _L("I"), _L("O"), _L("P") },
                                { _L("A"), _L("S"), _L("D"), _L("F"), _L("G"), _L("H"), _L("J"), _L("K"), _L("L"), _L("\xD1") },
                                { _L("Z"), _L("X"), _L("C"), _L("V"), _L("B"), _L("N"), _L("M"), _L("") , _L("") , _L("")  },
                              },
                              { { _L("q"), _L("w"), _L("e"), _L("r"), _L("t"), _L("y"), _L("u"), _L("i"), _L("o"), _L("p") },
                                { _L("a"), _L("s"), _L("d"), _L("f"), _L("g"), _L("h"), _L("j"), _L("k"), _L("l"), _L("\xF1") },
                                { _L("z"), _L("x"), _L("c"), _L("v"), _L("b"), _L("n"), _L("m"), _L("") , _L("") , _L("")  },
                              },
                              { { _L("1"), _L("2"), _L("3"), _L("4"), _L("5"), _L("6"), _L("7"), _L("8"), _L("9"), _L("0") },
                                { _L("@"), _L("#"), _L("!"), _L("_"), _L("&"), _L("-"), _L("+"), _L("("), _L(")"), _L("/") },
                                { _L("*"), _L("\""), _L("'"), _L(":"), _L(";"), _L(","), _L("?"), _L("") , _L("") , _L("")  },
                              }
                            };


  UI_VIRTUALKEYBOARD_KEYINFO keyinfo;
  XPATH                      pathimage;
  XDWORD                     key_normal_width  = 0; 
  XDWORD                     key_normal_height = 0; 

  GRPBITMAP* keyimage = LoadKeyImage(_L("virtualkbd/key.png"));
  if(keyimage)
    {
      key_normal_width  = keyimage->GetWidth();
      key_normal_height = keyimage->GetHeight();  

      //GEN_DELETE keyimage;
    }

  keyinfo.color     = _L("black,255");
  keyinfo.sizefont  = 32;
  keyinfo.margin    = 5;
  keyinfo.width     = key_normal_width;
  keyinfo.height    = key_normal_height;


  // Full window width (Android-style). Key rows stay at native key.png size and are centered;
  // the extra side space holds ESC left of Q.
  double sizewidth  = (double)screen->GetWidth();
  if(sizewidth < 1.0) sizewidth = (keyinfo.width * 10) + (UI_VIRTUALKEYBOARD_MARGIN*2);

  double sizeheight = (keyinfo.height * 4)  + (UI_VIRTUALKEYBOARD_MARGIN*2) + UI_VIRTUALKEYBOARD_INPUTBAND_HEIGHT; 

  double key_block_w = keyinfo.width * 10;
  double keys_left   = (sizewidth - key_block_w) / 2.0;
  // Keep room for one ESC key to the left of Q.
  if(keys_left < (double)(key_normal_width + UI_VIRTUALKEYBOARD_MARGIN))
    {
      keys_left = (double)(key_normal_width + UI_VIRTUALKEYBOARD_MARGIN);
    }

  main_form->GetBoundaryLine()->x       = x;
  main_form->GetBoundaryLine()->y       = y;
  main_form->GetBoundaryLine()->width   = sizewidth;
  main_form->GetBoundaryLine()->height  = sizeheight;

  main_form->GetBoundaryLine()->CopyTo((*main_form->GetVisibleRect()));

  // Live input edit sits in the top band; key rows start below it.
  CreateInputField(sizewidth, sizeheight);
  
  double xpos           = keys_left;
  double ypos           = sizeheight -1 - UI_VIRTUALKEYBOARD_MARGIN - UI_VIRTUALKEYBOARD_INPUTBAND_HEIGHT - keyinfo.height;  

  keyinfo.xpos          = xpos;
  keyinfo.ypos          = ypos;

  // ESC left of Q (top row) — away from RETURN/confirm on the right.
  keyinfo.xpos      = xpos - key_normal_width;
  keyinfo.sizefont  = 22;
  AddKeyButton(_L("ESC"), _L("Esc"), _L("virtualkbd/key.png"), keyinfo);
  keyinfo.sizefont  = 32;
  keyinfo.xpos      = xpos;
  
  int col               = 0; 
  int row               = 0;

      
  do{ for(col=0; col<10; col++)
        {              
          if(line[actualset][row][col].GetSize())
            {
              AddKeyButton(line[actualset][row][col].Get(), line[actualset][row][col].Get(), _L("virtualkbd/key.png"), keyinfo);
              keyinfo.xpos += keyinfo.width;          
            }
        }

      row++;

      if(row != 2) 
        { 
          keyinfo.xpos  = xpos; 
        }
       else 
        {
          keyinfo.xpos  = xpos +  (((keyinfo.width  * 10) -  (keyinfo.width  * 7)) /2);
        }

      keyinfo.ypos -= keyinfo.height;

    } while(row < 3);

 
  GRPBITMAP* spaceimage = LoadKeyImage(_L("virtualkbd/space_key.png"));
  if(spaceimage)
    {
      keyinfo.width   = spaceimage->GetWidth();
      keyinfo.height  = spaceimage->GetHeight();
      keyinfo.xpos    = (main_form->GetBoundaryLine()->width - keyinfo.width)/2;
      AddKeyButton(_L("SPACE"), NULL, _L("virtualkbd/space_key.png"), keyinfo);
      
      //GEN_DELETE spaceimage;
    }  
 
  keyinfo.width     = key_normal_width;
  keyinfo.height    = key_normal_height;

  keyinfo.xpos      = xpos + (key_normal_width *2);  
  AddKeyButton(_L("ARROWLEFT"), NULL, _L("virtualkbd/arrowleft_key.png"), keyinfo);


  keyinfo.xpos      = xpos + (key_normal_width * 7);  
  AddKeyButton(_L("ARROWRIGHT"), NULL, _L("virtualkbd/arrowright_key.png"), keyinfo);


  keyinfo.xpos      = xpos;
  keyinfo.ypos     += key_normal_height;
  AddKeyButton(_L("CAPS"), NULL, _L("virtualkbd/caps_key.png"), keyinfo);

  keyinfo.xpos      = xpos;
  keyinfo.ypos     -= key_normal_height;
  AddKeyButton(_L("SYMBOLS"), NULL, _L("virtualkbd/symbols_key.png"), keyinfo);

  keyinfo.xpos      = xpos + (key_normal_width * 9); 
  keyinfo.ypos     += key_normal_height;
  AddKeyButton(_L("BACK"), NULL, _L("virtualkbd/back_key.png"), keyinfo);

  keyinfo.xpos      = xpos + (key_normal_width * 9); 
  keyinfo.ypos     -= key_normal_height;
  AddKeyButton(_L("RETURN"), NULL, _L("virtualkbd/return_key.png"), keyinfo);
    
  if(skin) skin->CalculeBoundaryLine_AllElements(main_form, true);

  // Keep VisibleRect in lockstep with GetXPosition/GetYPosition after CalculeBoundaryLine — Draw_Form
  // fills VisibleRect while ModalLayer/PreDraw use GetXPosition. A stale VisibleRect (copied before this
  // pass) made the opaque modal blit miss the left strip where ListBoxMenu overlaps.
  main_form->GetVisibleRect()->x      = main_form->GetXPosition();
  main_form->GetVisibleRect()->y      = main_form->GetYPosition();
  main_form->GetVisibleRect()->width  = main_form->GetBoundaryLine()->width;
  main_form->GetVisibleRect()->height = main_form->GetBoundaryLine()->height;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::CreateInputField(double sizewidth, double sizeheight)
* @brief      Create the GEN edittext hosted above the keys (cursor, blink, max size).
* @ingroup    USERINTERFACE
* 
* @param[in]  sizewidth : 
* @param[in]  sizeheight : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::CreateInputField(double sizewidth, double sizeheight)
{
  if(!main_form || !skin) return false;

  element_input = GEN_NEW UI_ELEMENT_EDITTEXT();
  if(!element_input) return false;

  element_input->SetFather(main_form);
  element_input->GetName()->Set(UI_VIRTUALKEYBOARD_INPUT_NAME);

  double input_w = sizewidth - (UI_VIRTUALKEYBOARD_MARGIN * 2);
  double input_h = (double)UI_VIRTUALKEYBOARD_INPUT_HEIGHT;
  if(input_w < 32.0) input_w = 32.0;

  // Top band of the form (same local y-up convention as the key rows).
  element_input->GetBoundaryLine()->x      = UI_VIRTUALKEYBOARD_MARGIN;
  element_input->GetBoundaryLine()->y      = sizeheight - UI_VIRTUALKEYBOARD_MARGIN - input_h;
  element_input->GetBoundaryLine()->width  = input_w;
  element_input->GetBoundaryLine()->height = input_h;

  element_input->SetSizeFont(22);
  element_input->SetRoundRect(10);
  element_input->SetMaxSizeText(32);
  element_input->GetText()->Empty();

  // Readable on the grey keyboard panel — same widget path as layout edittext (Draw_EditText).
  element_input->GetColor()->SetFromString(_L("20,20,20,255"));
  element_input->GetBackgroundColor()->SetFromString(_L("245,245,245,255"));
  element_input->SetBackgroundColorSet(true);

  element_input->SetVisible(true);
  element_input->Cursor_SetVisible(false);

  GEN_USERINTERFACE.SetLevelAuto(element_input, main_form);
  main_form->GetComposeElements()->Add(element_input);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_VIRTUALKEYBOARD::DeleteAllKeys()
* @brief      Delete all keys
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_VIRTUALKEYBOARD::DeleteAllKeys()
{
  if(!main_form) return false;
  if(main_form->GetComposeElements()->IsEmpty()) return false;

  // Compose delete frees the input edit; drop the dangling pointer first.
  element_input = NULL;

  main_form->GetComposeElements()->DeleteContents();
  main_form->GetComposeElements()->DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_VIRTUALKEYBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_VIRTUALKEYBOARD::Clean()
{
  screen            = NULL;
  layout            = NULL;
  skin              = NULL;
  isshow            = false;
  main_form         = NULL;
  element_editable  = NULL;
  element_input     = NULL;
  width             = 0;
  height            = 0;
  actualset         = UI_VIRTUALKEYBOARD_SET_UPPERCASE;
  contentchanged    = false;
}





