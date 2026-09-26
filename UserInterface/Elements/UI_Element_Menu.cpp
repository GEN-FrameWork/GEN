/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Menu.cpp
* 
* @class      UI_ELEMENT_MENU
* @brief      User Interface Element Menu class
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

#include "UI_Element_Menu.h"

#include "UI_Element.h"
#include "UI_Element_Text.h"
#include "UI_Element_Option.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/





/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_MENU_OPTION_CFG::UI_ELEMENT_MENU_OPTION_CFG()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU_OPTION_CFG::UI_ELEMENT_MENU_OPTION_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_MENU_OPTION_CFG::~UI_ELEMENT_MENU_OPTION_CFG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU_OPTION_CFG::~UI_ELEMENT_MENU_OPTION_CFG()
{
  Clean();
}
   		

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_MENU_OPTION_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_MENU_OPTION_CFG::Clean()
{
  index         = 0; 

  nameoption.Empty();
  leyend.Empty(); 

  colorstr.Empty();

  sizefont      = 0; 
  marginwidth   = 0.0f;  
  marginheight  = 0.0f;  
  width         = 0.0f;
  height        = 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_MENU::UI_ELEMENT_MENU()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU::UI_ELEMENT_MENU()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_MENU);

  GetTypeString()->Set(__L("menu"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_MENU::~UI_ELEMENT_MENU()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU::~UI_ELEMENT_MENU()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_OPTION* UI_ELEMENT_MENU::Option_AddText(UI_ELEMENT_MENU_OPTION_CFG& option_cfg)
* @brief      Option add text
* @ingroup    USERINTERFACE
* 
* @param[in]  option_cfg : Option configuration value.
* 
* @return     UI_ELEMENT_OPTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_OPTION* UI_ELEMENT_MENU::Option_AddText(UI_ELEMENT_MENU_OPTION_CFG& option_cfg)
{  
  UI_ELEMENT_OPTION* element_option = GEN_NEW UI_ELEMENT_OPTION();
  if(element_option)
    {
      UI_ELEMENT_TEXT* element_text = GEN_NEW UI_ELEMENT_TEXT();
      if(element_text) 
        {          
          element_text->SetFather(element_option);
          element_text->GetName()->Set(option_cfg.nameoption);
          element_text->GetText()->Set(option_cfg.leyend);
      
          element_text->GetBoundaryLine()->x        = 0;
          element_text->GetBoundaryLine()->y        = UI_ELEMENT_TYPE_ALIGN_CENTER;
          element_text->GetBoundaryLine()->width    = UI_ELEMENT_TYPE_ALIGN_AUTO;
          element_text->GetBoundaryLine()->height   = UI_ELEMENT_TYPE_ALIGN_AUTO;

          element_text->SetMustReDraw(true);
 
          if(!option_cfg.colorstr.IsEmpty()) element_text->GetColor()->SetFromString(option_cfg.colorstr);
          element_text->SetSizeFont(option_cfg.sizefont);          
       
          element_option->SetFather(this);
          element_option->GetName()->Set(option_cfg.nameoption);   
          element_option->GetBoundaryLine()->x      = option_cfg.marginwidth;          
          element_option->GetBoundaryLine()->y      = GetBoundaryLine()->height -  ((option_cfg.height + option_cfg.marginheight) * option_cfg.index);
          element_option->GetBoundaryLine()->width  = option_cfg.width - (option_cfg.marginwidth*2);
          element_option->GetBoundaryLine()->height = option_cfg.height;
          
          element_option->Set_UIText(element_text);
          element_option->GetComposeElements()->Add(element_text);

          GetComposeElements()->Add(element_option);

          element_option->SetTimeSelected(100);

          element_option->SetMustReDraw(true);

          return element_option;
        }

      GEN_DELETE element_option;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_MENU::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_MENU::Clean()
{
  
}




