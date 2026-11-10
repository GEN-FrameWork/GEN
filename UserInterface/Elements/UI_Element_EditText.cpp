/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_EditText.cpp
* 
* @class      UI_ELEMENT_EDITTEXT
* @brief      User Interface Element Edit Text class
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

#include "UI_Element_EditText.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_EDITTEXT::UI_ELEMENT_EDITTEXT()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_EDITTEXT::UI_ELEMENT_EDITTEXT()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_EDITTEXT);
  GetTypeString()->Set(NL("edittext"));  

  cursor_xtimerblink = GEN_XFACTORY.CreateTimer();
  cursor_timeblink   = UI_ELEMENT_EDITTEXT_DEFAULTCUSORTIMEBLINK;

  // Latch selection while typing (default selectable hold is 100 ms).
  SetTimeSelected(0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_EDITTEXT::~UI_ELEMENT_EDITTEXT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_EDITTEXT::~UI_ELEMENT_EDITTEXT()    
{ 
  if(cursor_xtimerblink)
    {
      GEN_XFACTORY.DeleteTimer(cursor_xtimerblink);
      cursor_xtimerblink = NULL;
    }

  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_EDITTEXT::Cursor_IsVisible()
* @brief      Cursor is visible
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::Cursor_IsVisible()
{
  return cursor_isvisible;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_EDITTEXT::Cursor_SetVisible(bool on)
* @brief      Cursor set visible
* @ingroup    USERINTERFACE
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::Cursor_SetVisible(bool on)
{
  this->cursor_isvisible = on;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* UI_ELEMENT_EDITTEXT::Cursor_GetXTimerBlink()
* @brief      Cursor get X timer blink
* @ingroup    USERINTERFACE
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* UI_ELEMENT_EDITTEXT::Cursor_GetXTimerBlink()
{
  return cursor_xtimerblink;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_EDITTEXT::Cursor_GetTimeBlink()
* @brief      Cursor get time blink
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_EDITTEXT::Cursor_GetTimeBlink()
{
  return  cursor_timeblink;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_EDITTEXT::Cursor_SetTimeBlink(XDWORD timeblink)
* @brief      Cursor set time blink
* @ingroup    USERINTERFACE
* 
* @param[in]  timeblink : Timeblink value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::Cursor_SetTimeBlink(XDWORD timeblink)
{
  this->cursor_timeblink = timeblink;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_EDITTEXT::Cursor_GetStateBlink()
* @brief      Cursor get state blink
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::Cursor_GetStateBlink()
{
  return cursor_stateblink;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_EDITTEXT::Cursor_ChangeStateBlink()
* @brief      Cursor change state blink
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void  UI_ELEMENT_EDITTEXT::Cursor_ChangeStateBlink()
{
  cursor_stateblink =!cursor_stateblink;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_EDITTEXT_INPUTMODE UI_ELEMENT_EDITTEXT::GetInputMode()
* @brief      Get input mode
* @ingroup    USERINTERFACE
*
* @return     UI_ELEMENT_EDITTEXT_INPUTMODE : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_EDITTEXT_INPUTMODE UI_ELEMENT_EDITTEXT::GetInputMode()
{
  return inputmode;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_EDITTEXT::SetInputMode(UI_ELEMENT_EDITTEXT_INPUTMODE mode)
* @brief      Set input mode
* @ingroup    USERINTERFACE
*
* @param[in]  mode : Mode value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::SetInputMode(UI_ELEMENT_EDITTEXT_INPUTMODE mode)
{
  inputmode = mode;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::SetInputModeFromString(XCHAR* modestring)
* @brief      Set input mode from string
* @ingroup    USERINTERFACE
*
* @param[in]  modestring : Mode string (text / numeric / password).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::SetInputModeFromString(XCHAR* modestring)
{
  if(!modestring) return false;

  XSTRING mode;
  mode = modestring;

  if(!mode.Compare(NL("numeric") , true) || !mode.Compare(NL("number"), true) || !mode.Compare(NL("int"), true))
    {
      inputmode = UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC;
      return true;
    }

  if(!mode.Compare(NL("password"), true) || !mode.Compare(NL("passwd"), true))
    {
      inputmode = UI_ELEMENT_EDITTEXT_INPUTMODE_PASSWORD;
      return true;
    }

  if(!mode.Compare(NL("text"), true) || !mode.Compare(NL("string"), true) || !mode.Compare(NL("normal"), true))
    {
      inputmode = UI_ELEMENT_EDITTEXT_INPUTMODE_TEXT;
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_EDITTEXT::GetValueMin()
* @brief      Get numeric minimum
* @ingroup    USERINTERFACE
*
* @return     int : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_EDITTEXT::GetValueMin()
{
  return valuemin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_EDITTEXT::SetValueMin(int valuemin)
* @brief      Set numeric minimum
* @ingroup    USERINTERFACE
*
* @param[in]  valuemin : Valuemin value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::SetValueMin(int valuemin)
{
  this->valuemin = valuemin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_EDITTEXT::GetValueMax()
* @brief      Get numeric maximum
* @ingroup    USERINTERFACE
*
* @return     int : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_EDITTEXT::GetValueMax()
{
  return valuemax;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_EDITTEXT::SetValueMax(int valuemax)
* @brief      Set numeric maximum
* @ingroup    USERINTERFACE
*
* @param[in]  valuemax : Valuemax value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::SetValueMax(int valuemax)
{
  this->valuemax = valuemax;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_EDITTEXT::GetValueStep()
* @brief      Get numeric step
* @ingroup    USERINTERFACE
*
* @return     int : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_EDITTEXT::GetValueStep()
{
  return valuestep;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_EDITTEXT::SetValueStep(int valuestep)
* @brief      Set numeric step
* @ingroup    USERINTERFACE
*
* @param[in]  valuestep : Valuestep value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::SetValueStep(int valuestep)
{
  this->valuestep = (valuestep == 0) ? 1 : valuestep;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::IsCharAllowed(XCHAR character)
* @brief      True when the character may be inserted for the current input mode.
* @ingroup    USERINTERFACE
*
* @param[in]  character : Character to test.
*
* @return     bool : true if allowed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::IsCharAllowed(XCHAR character)
{
  switch(inputmode)
    {
      case UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC  : return ((character >= _C('0')) && (character <= _C('9')));

      case UI_ELEMENT_EDITTEXT_INPUTMODE_PASSWORD :
      case UI_ELEMENT_EDITTEXT_INPUTMODE_TEXT     :
                                       default     : return true;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::SanitizeTextToMode()
* @brief      Filters / clamps the current text to the active input mode.
* @ingroup    USERINTERFACE
*
* @return     bool : true if the text was modified; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::SanitizeTextToMode()
{
  if(!GetText()) return false;

  if(inputmode != UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC)
    {
      return false;
    }

  XSTRING filtered;
  XSTRING* src = GetText();
  for(XDWORD c=0; c<src->GetSize(); c++)
    {
      XCHAR ch = src->Get()[c];
      if((ch >= _C('0')) && (ch <= _C('9'))) filtered.Add(ch);
    }

  bool changed = filtered.Compare(src->Get()) ? true : false;
  src->Set(filtered.Get());

  if(src->IsEmpty())
    {
      SetNumericValue(valuemin);
      return true;
    }

  int value = src->ConvertToInt();
  if(value < valuemin) value = valuemin;
  if(value > valuemax) value = valuemax;

  XSTRING clamped;
  clamped.Format(NL("%d"), value);
  if(clamped.Compare(src->Get())) changed = true;
  src->Set(clamped.Get());

  Cursor_SetPosition(src->GetSize());
  return changed;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_EDITTEXT::GetNumericValue()
* @brief      Get numeric value from text
* @ingroup    USERINTERFACE
*
* @return     int : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_EDITTEXT::GetNumericValue()
{
  if(!GetText() || GetText()->IsEmpty()) return valuemin;
  return GetText()->ConvertToInt();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::SetNumericValue(int value)
* @brief      Set numeric value into text (clamped)
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to set.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::SetNumericValue(int value)
{
  if(value < valuemin) value = valuemin;
  if(value > valuemax) value = valuemax;

  if(!GetText()) return false;
  GetText()->Format(NL("%d"), value);
  Cursor_SetPosition(GetText()->GetSize());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::StepNumericValue(int direction)
* @brief      Add/subtract step from the numeric value
* @ingroup    USERINTERFACE
*
* @param[in]  direction : +1 up, -1 down (sign only matters).
*
* @return     bool : true if the value changed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::StepNumericValue(int direction)
{
  if(inputmode != UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC) return false;
  if(direction == 0) return false;

  int step = valuestep;
  if(step < 1) step = 1;

  int before = GetNumericValue();
  int after  = before + ((direction > 0) ? step : -step);
  SetNumericValue(after);
  return (GetNumericValue() != before);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_EDITTEXT::BuildDisplayText(XSTRING& display)
* @brief      Builds the string that should be painted (masked for password).
* @ingroup    USERINTERFACE
*
* @param[out] display : Display string.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_EDITTEXT::BuildDisplayText(XSTRING& display)
{
  display.Empty();
  if(!GetText()) return false;

  if(inputmode != UI_ELEMENT_EDITTEXT_INPUTMODE_PASSWORD)
    {
      display = GetText()->Get();
      return true;
    }

  XDWORD n = GetText()->GetSize();
  for(XDWORD c=0; c<n; c++)
    {
      display.Add(_C('*'));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT_EDITTEXT::GetSpinWidth()
* @brief      Width reserved for numeric spin buttons (0 when not numeric).
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_EDITTEXT::GetSpinWidth()
{
  if(inputmode != UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC) return 0.0;
  return UI_ELEMENT_EDITTEXT_SPIN_WIDTH;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_EDITTEXT::HitTestSpin(int design_x, int design_y)
* @brief      Hit-tests the numeric spin zone
* @ingroup    USERINTERFACE
*
* @param[in]  design_x : Absolute design X.
* @param[in]  design_y : Absolute design Y (screen: +Y down; stored y = EdgeBottom).
*
* @return     int : 1 up, -1 down, 0 outside spin.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_EDITTEXT::HitTestSpin(int design_x, int design_y)
{
  if(inputmode != UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC) return 0;
  if(!GetBoundaryLine()) return 0;

  double spin_w = GetSpinWidth();
  if(spin_w < 1.0) return 0;

  double left   = GetXPosition();
  double bottom = GetYPosition();   // EdgeBottom (larger y)
  double width  = GetBoundaryLine()->width;
  double right  = left + width;
  double top    = GetTopY();        // EdgeTop (smaller y)

  double spin_left = right - spin_w;
  if((design_x < (int)spin_left) || (design_x > (int)right)) return 0;
  if((design_y < (int)top) || (design_y > (int)bottom)) return 0;

  double mid = top + ((bottom - top) * 0.5);
  if(design_y <= (int)mid) return 1;   // upper half (toward EdgeTop) = +
  return -1;                            // lower half = -
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_EDITTEXT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_EDITTEXT::Clean()
{
  cursor_isvisible    = false;
  cursor_xtimerblink  = NULL;
  cursor_timeblink    = 0;
  cursor_stateblink   = false;
  inputmode           = UI_ELEMENT_EDITTEXT_INPUTMODE_TEXT;
  valuemin            = 0;
  valuemax            = 2147483647;
  valuestep           = 1;
}
