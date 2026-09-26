/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPCursor.cpp
* 
* @class      INPCURSOR
* @brief      Input Cursor Class
* @ingroup    INPUT
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

#include "INPCursor.h"

#include <string.h>

#include "XFactory.h"
#include "XTimer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR::INPCURSOR(): INPCURSORMOTIONPOINT()
* @brief      Constructor of class
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR::INPCURSOR(): INPCURSORMOTIONPOINT()
{
  Clean();

  timer=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR::~INPCURSOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR::~INPCURSOR()
{
  if(timer)
    {
      GEN_XFACTORY.DeleteTimer(timer);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSOR_ID INPCURSOR::GetID()
* @brief      Get ID
* @ingroup    INPUT
* 
* @return     INPCURSOR_ID : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSOR_ID INPCURSOR::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::SetID(INPCURSOR_ID ID)
* @brief      Set ID
* @ingroup    INPUT
* 
* @param[in]  ID : GEN_NEW ID of cursor
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::SetID(INPCURSOR_ID ID)
{
  this->ID = ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* INPCURSOR::GetTimer()
* @brief      Get timer
* @ingroup    INPUT
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* INPCURSOR::GetTimer()
{
  return timer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::HavePreSelect()
* @brief      Have pre select
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::HavePreSelect()
{
  return havepreselect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::SetHavePreSelect(bool havepreselect)
* @brief      Set have pre select
* @ingroup    INPUT
* 
* @param[in]  havepreselect : true active have pre-select
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::SetHavePreSelect(bool havepreselect)
{
  this->havepreselect = havepreselect;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::IsChanged()
* @brief      Is changed
* @ingroup    INPUT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::IsChanged()
{
  return ischanged;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::SetIsChanged(bool ischanged)
* @brief      Set is changed
* @ingroup    INPUT
* 
* @param[in]  ischanged : true is changed
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::SetIsChanged(bool ischanged)
{
  this->ischanged = ischanged;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::IsPositionInRect(int x, int y, int width, int height)
* @brief      Is position in rect
* @ingroup    INPUT
* 
* @param[in]  x : x position of rect
* @param[in]  y : y position of rect
* @param[in]  width : width of rect
* @param[in]  height : height of rect
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::IsPositionInRect(int x, int y, int width, int height)
{
  if((GetX() >= x) && (GetX() <= (x+width)) &&
     (GetY() >= y) && (GetY() <= (y+height))) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTION* INPCURSOR::GetMotion()
* @brief      Get motion
* @ingroup    INPUT
* 
* @return     INPCURSORMOTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTION* INPCURSOR::GetMotion()
{
  return &motion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSOR::AddPointToMotion(bool ispressed)
* @brief      Add point to motion
* @ingroup    INPUT
* 
* @param[in]  ispressed : is pressed
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSOR::AddPointToMotion(bool ispressed)
{
  if(ispressed)
    {
      if(!motion.IsInCurse())
        {
          motion.Reset();
          motion.SetIsInCurse(true);
        }

      motion.AddPoint(GetX(), GetY(), GetZ());
    }
   else
    {
      if(motion.IsInCurse())
        {
          motion.AddPoint(GetX(), GetY(), GetZ());

          motion.SetTimeElapsed(GetTimer()->GetMeasureMilliSeconds());
          motion.SetIsInCurse(false);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::Set(float x, float y, float z)
* @brief      Set value
* @ingroup    INPUT
* 
* @param[in]  x : GEN_NEW x position
* @param[in]  y : GEN_NEW y position
* @param[in]  z : GEN_NEW x position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::Set(float x , float y, float z)
{
  double xx=(x-this->source.x1)/(this->source.x2-this->source.x1);

  xx = xx*(this->destination.x2-this->destination.x1)+this->destination.x1;

  double yy=(y-this->source.y1)/(this->source.y2-this->source.y1);

  yy = yy*(this->destination.y2-this->destination.y1)+this->destination.y1;

  this->x = (float)xx;
  this->y = (float)yy;
  this->z = (float)z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::SetX(float x)
* @brief      Set x
* @ingroup    INPUT
* 
* @param[in]  x : GEN_NEW x position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::SetX(float x)
{
  double xx = (x - source.x1) / (source.x2 - source.x1);

  xx = xx * (destination.x2 - destination.x1) + destination.x1;

  this->x = (float)xx;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::SetY(float y)
* @brief      Set y
* @ingroup    INPUT
* 
* @param[in]  y : GEN_NEW y position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::SetY(float y)
{
  double yy = (y- source.y1) / (source.y2 - source.y1);
        
  yy = yy * (destination.y2 - destination.y1) + destination.y1;

  this->y = (float)yy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::SetZ(float z)
* @brief      Set z
* @ingroup    INPUT
* 
* @param[in]  z : GEN_NEW z position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::SetZ(float z)
{
  this->z = z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSOR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSOR::Clean()
{
  timer         = NULL;

  ID            = INPCURSOR_ID_NONE;

  ischanged     = false;
  havepreselect = false;
};


