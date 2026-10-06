/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Color.cpp
* 
* @class      UI_COLOR
* @brief      User Interface Color class
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

#include "UI_Color.h"

#include "UI_Colors.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR::UI_COLOR()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR::UI_COLOR()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR::UI_COLOR(XCHAR* string)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR::UI_COLOR(XCHAR* string)
{
  Clean();
  
  SetFromString(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR::UI_COLOR(XSTRING& string)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR::UI_COLOR(XSTRING& string)
{
  Clean();

  SetFromString(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR::~UI_COLOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR::~UI_COLOR()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::IsValid()
* @brief      Is valid
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::IsValid()
{
  return valid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::SetValid(bool valid)
* @brief      Set valid
* @ingroup    USERINTERFACE
* 
* @param[in]  valid : Valid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::SetValid(bool valid)
{
  this->valid = valid;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::SetInvalid()
* @brief      Set invalid
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::SetInvalid()
{
  valid = false;

  red   = 0;
  green = 0;
  blue  = 0;
  alpha = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::GetRed()
* @brief      Get red
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::GetRed() 
{
  return red;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::SetRed(int red)
* @brief      Set red
* @ingroup    USERINTERFACE
* 
* @param[in]  red : Red value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::SetRed(int red)  
{
  this->red = red;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::GetGreen()
* @brief      Get green
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::GetGreen()
{
  return green;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::SetGreen(int green)
* @brief      Set green
* @ingroup    USERINTERFACE
* 
* @param[in]  green : Green value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::SetGreen(int green)
{
  this->green = green;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::GetBlue()
* @brief      Get blue
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::GetBlue()
{
  return blue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::SetBlue(int blue)
* @brief      Set blue
* @ingroup    USERINTERFACE
* 
* @param[in]  blue : Blue value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::SetBlue(int blue)
{
  this->blue = blue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::GetAlpha()
* @brief      Get alpha
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::GetAlpha()
{
  return alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::SetAlpha(int alpha)
* @brief      Set alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  alpha : Alpha value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::SetAlpha(int alpha)
{
  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::SetFromString(XCHAR* string)
* @brief      Set from string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::SetFromString(XCHAR* string)
{
  SetInvalid();

  if(!string) 
    {
      return false;  
    }

  XSTRING colorstr;
  int     ncommas = 0;
  int     _alpha  = 0;
  bool    status  = false;

  colorstr = string;
  if(colorstr.IsEmpty()) 
    {
      return false;
    }

  // "#RRGGBB" / "#RRGGBBAA" (CSS-style hex notation)
  if(colorstr.Get()[0] == _C('#'))
    {
      XDWORD length = colorstr.GetSize() - 1;

      if((length == 6) || (length == 8))
        {
          red   = (HexValue(colorstr.Get()[1]) * 16) + HexValue(colorstr.Get()[2]);
          green = (HexValue(colorstr.Get()[3]) * 16) + HexValue(colorstr.Get()[4]);
          blue  = (HexValue(colorstr.Get()[5]) * 16) + HexValue(colorstr.Get()[6]);
          alpha = (length == 8) ? (HexValue(colorstr.Get()[7]) * 16) + HexValue(colorstr.Get()[8]) : 255;

          valid = true;

          return true;
        }

      return false;
    }

  for(XDWORD c=0; c<colorstr.GetSize(); c++)
    {
      XCHAR character = colorstr.Get()[c];
      if(character == _C(',')) 
        {
          ncommas++;
        }
    }
    
  switch(ncommas)
    {
      case  0 : // only name
                { XSTRING* resolved_colorstr = GEN_UI_COLORS.Get(colorstr);
                  if(resolved_colorstr) 
                    {
                      status = SetFromString(resolved_colorstr->Get()); 
                    }
                }
                break;

      case  1 : // name +  int alpha
                { XSTRING  _colorstr;                  
                  XSTRING* resolved_colorstr = NULL;

                  _colorstr.AdjustSize(_MAXSTR);
                  colorstr.UnFormat(_L("%s,%d"), _colorstr.Get(), &_alpha);
                  _colorstr.AdjustSize();

                  resolved_colorstr = GEN_UI_COLORS.Get(_colorstr);
                  if(resolved_colorstr) 
                    {
                      status = SetFromString(resolved_colorstr->Get());                    
                    }

                  alpha = GetAlphaForPercent(_alpha);
                }
                break;

      case  2 : // int colors
                status = colorstr.UnFormat(_L("%d,%d,%d"), &red, &green, &blue);
                alpha = 255;  
                break;

      case  3 : // int colors + int alpha
                status = colorstr.UnFormat(_L("%d,%d,%d,%d"), &red, &green, &blue, &_alpha);
                alpha = GetAlphaForPercent(_alpha);
                break;

      default : break;
    }

  if(!status)
    {
      SetInvalid();
    }

  valid = true;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::SetFromString(XSTRING& string)
* @brief      Set from string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::SetFromString(XSTRING& string)
{
  return SetFromString(string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::CopyFrom(UI_COLOR* color)
* @brief      Copy from
* @ingroup    USERINTERFACE
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::CopyFrom(UI_COLOR* color)
{
  if(!color) 
    {
      return false;
    }

  valid = color->IsValid();
  red   = color->GetRed();
  green = color->GetGreen();
  blue  = color->GetBlue();
  alpha = color->GetAlpha();  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_COLOR::CopyTo(UI_COLOR* color)
* @brief      Copy to
* @ingroup    USERINTERFACE
* 
* @param[in]  color : Color value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COLOR::CopyTo(UI_COLOR* color)
{
  if(!color) 
    {
      return false;
    }

  color->SetValid(valid);
  color->SetRed(red);
  color->SetGreen(green);
  color->SetBlue(blue);
  color->SetAlpha(alpha);  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::GetAlphaForPercent(int percent)
* @brief      Get alpha for percent
* @ingroup    USERINTERFACE
* 
* @param[in]  percent : Percent value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::GetAlphaForPercent(int percent)
{
  int _percent = percent;

  if(_percent < 0)   _percent = 0;
  if(_percent > 100) _percent = 100;

  return (int)((_percent * 255) / 100);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_COLOR::HexValue(XCHAR character)
* @brief      Hex value : convert one hexadecimal character to its numeric value (0..15)
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* @param[in]  character : hexadecimal character
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_COLOR::HexValue(XCHAR character)
{
  if((character >= _C('0')) && (character <= _C('9')))  return (int)(character - _C('0'));
  if((character >= _C('a')) && (character <= _C('f')))  return (int)(character - _C('a')) + 10;
  if((character >= _C('A')) && (character <= _C('F')))  return (int)(character - _C('A')) + 10;

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_COLOR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_COLOR::Clean()
{
  valid = false;
  red   = 0;
  green = 0;
  blue  = 0;
  alpha = 0;
}



