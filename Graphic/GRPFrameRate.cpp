/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPFrameRate.cpp
* 
* @class      GRPFRAMERATE
* @brief      Graphic Frame Rate class
* @ingroup    GRAPHIC
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

#include "GRPFrameRate.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XThreadCollected.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPFRAMERATE::GRPFRAMERATE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPFRAMERATE::GRPFRAMERATE()    
{ 
  Clean(); 

  xtimertotal=GEN_XFACTORY.CreateTimer();
    
  thread_framerate = CREATEXTHREAD((XTHREADGROUPID)(XTHREADGROUPID_GRPFRAMERATE), _L("GRPFRAMERATE::GRPFRAMERATE"), ThreadRunFunction_Framerate, this);    
  if(!thread_framerate)  return;

  if(!thread_framerate->Ini()) return;                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPFRAMERATE::~GRPFRAMERATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPFRAMERATE::~GRPFRAMERATE()    
{ 
  if(thread_framerate)
    {
      thread_framerate->End();

      DELETEXTHREAD((XTHREADGROUPID)(XTHREADGROUPID_GRPFRAMERATE), thread_framerate);
      thread_framerate = NULL;  
    }

  if(xtimertotal) GEN_XFACTORY.DeleteTimer(xtimertotal);

  Clean();                       
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* GRPFRAMERATE::GetXTimer()
* @brief      Get X timer
* @ingroup    GRAPHIC
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* GRPFRAMERATE::GetXTimer()
{
  return xtimertotal;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPFRAMERATE::AddNFrames()
* @brief      Add N frames
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPFRAMERATE::AddNFrames()
{
  nframes++;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPFRAMERATE::GetNFrames()
* @brief      Get N frames
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPFRAMERATE::GetNFrames()
{
  return nframes;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPFRAMERATE::Reset()
* @brief      Reset
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPFRAMERATE::Reset()
{  
  if(xtimertotal)  xtimertotal->Reset();

  nframes           = 0; 
  ngets             = 0;
  
  actual_framerate  = 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float GRPFRAMERATE::Get()
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float GRPFRAMERATE::Get()
{  
  ngets++;

  return actual_framerate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPFRAMERATE::ThreadRunFunction_Framerate(void* param)
* @brief      Thread run function framerate
* @ingroup    GRAPHIC
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPFRAMERATE::ThreadRunFunction_Framerate(void* param)
{
  GRPFRAMERATE* grpframerate = (GRPFRAMERATE*)param;
  if(!grpframerate) return;

  XQWORD msec = grpframerate->xtimertotal->GetMeasureMilliSeconds();
  if(msec < 1000)  return;

  grpframerate->actual_framerate = (float)grpframerate->nframes;
      
  grpframerate->xtimertotal->Reset();
  grpframerate->nframes = 0;
  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPFRAMERATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPFRAMERATE::Clean()
{
  xtimertotal       = NULL;
  nframes           = 0;

  ngets             = 0;
  
  actual_framerate  = 0.0f;  
}



