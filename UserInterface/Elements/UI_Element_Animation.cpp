/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Animation.cpp
* 
* @class      UI_ELEMENT_ANIMATION
* @brief      User Interface Element Animation class
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

#include "UI_Element_Animation.h"

#include "XFactory.h"
#include "XTimer.h"

#include "GRPBitmap.h"

#include "UI_Element_Image.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_ANIMATION::UI_ELEMENT_ANIMATION()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_ANIMATION::UI_ELEMENT_ANIMATION()    
{ 
  Clean();   

  xtimerplay=GEN_XFACTORY.CreateTimer();
  
  SetMilliSecondsDelay();

  SetType(UI_ELEMENT_TYPE_ANIMATION);
  GetTypeString()->Set(_L("animation"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_ANIMATION::~UI_ELEMENT_ANIMATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_ANIMATION::~UI_ELEMENT_ANIMATION()    
{  
  if(xtimerplay) 
    {
      GEN_XFACTORY.DeleteTimer(xtimerplay);
      xtimerplay = NULL;
    }

  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::Play()
* @brief      Play
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::Play()
{
  state = UI_ELEMENT_ANIMATION_STATE_PLAY;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::Play(int millisecdelay)
* @brief      Play
* @ingroup    USERINTERFACE
* 
* @param[in]  millisecdelay : Millisecdelay value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::Play(int millisecdelay)
{
  SetMilliSecondsDelay(millisecdelay);

  return Play();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::Stop()
* @brief      Stop
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::Stop()
{
  state = UI_ELEMENT_ANIMATION_STATE_STOP;

  return true;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::Pause()
* @brief      Pause
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::Pause()
{
  state = UI_ELEMENT_ANIMATION_STATE_PAUSE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_ANIMATION_STATE UI_ELEMENT_ANIMATION::GetState()
* @brief      Get state
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_ANIMATION_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_ANIMATION_STATE UI_ELEMENT_ANIMATION::GetState()
{
  return state;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* UI_ELEMENT_ANIMATION::GetXTimerPlay()
* @brief      Get X timer play
* @ingroup    USERINTERFACE
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* UI_ELEMENT_ANIMATION::GetXTimerPlay()
{
  return xtimerplay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_ELEMENT_ANIMATION::GetMilliSecondsDelay()
* @brief      Get milli seconds delay
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_ANIMATION::GetMilliSecondsDelay()
{
  return millisecdelay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_ANIMATION::SetMilliSecondsDelay(int millisecdelay)
* @brief      Set milli seconds delay
* @ingroup    USERINTERFACE
* 
* @param[in]  millisecdelay : Millisecdelay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_ANIMATION::SetMilliSecondsDelay(int millisecdelay)
{
  this->millisecdelay = millisecdelay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int UI_ELEMENT_ANIMATION::IndexImage_Get()
* @brief      Index image get
* @ingroup    USERINTERFACE
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_ANIMATION::IndexImage_Get()
{
  return indeximage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::IndexImage_Set(int indeximage)
* @brief      Index image set
* @ingroup    USERINTERFACE
* 
* @param[in]  indeximage : Indeximage value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::IndexImage_Set(int indeximage)
{
  if(indeximage <0)                                        return false;
  if(indeximage >= (int)GetComposeElements()->GetSize())   return false;

  if(this->indeximage != indeximage) SetMustReDraw(true);

  this->indeximage = indeximage;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::IndexImage_Add(int inc)
* @brief      Index image add
* @ingroup    USERINTERFACE
* 
* @param[in]  inc : Inc value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::IndexImage_Add(int inc)
{
  indeximage += inc;

  SetMustReDraw(true);

  if(indeximage >= GetComposeElements()->GetSize()) indeximage = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_ANIMATION::IndexImage_Reset()
* @brief      Index image reset
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_ANIMATION::IndexImage_Reset()
{
  indeximage = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE UI_ELEMENT_ANIMATION::GetAlpha()
* @brief      Get alpha
* @ingroup    USERINTERFACE
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE UI_ELEMENT_ANIMATION::GetAlpha()
{
  return alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_ANIMATION::SetAlpha(XBYTE alpha)
* @brief      Set alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  alpha : Alpha value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_ANIMATION::SetAlpha(XBYTE alpha)
{
  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_ANIMATION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_ANIMATION::Clean()
{
  xtimerplay      = NULL;
  millisecdelay   = 0;
  state           = UI_ELEMENT_ANIMATION_STATE_UNKNOWN;    
  indeximage      = 0;
  alpha           = 100;
}



