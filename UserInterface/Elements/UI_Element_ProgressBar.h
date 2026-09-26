/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_ProgressBar.h
* 
* @class      UI_ELEMENT_PROGRESSBAR
* @brief      User Interface Element Progress Bar class
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

#include "UI_Color.h"
#include "UI_BoundaryLine.h"
#include "UI_Element.h"
#include "UI_Element_Option.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSIZESEGMENT          33        // Percent of size of segment in continuous cycle
#define UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSTEPSEGMENT          10        // Step of segment in continuous cycle
#define UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTTIMESTEP             10        // Time (milliseconds) of step segment (redraw) in continuous cycle

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

#ifndef UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL
#define UI_ELEMENT_PROGRESS_GRADIENTMODE_FILL    0                                   // gradient maps to the current fill (leading edge = end color)
#define UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK   1                                   // gradient maps to the full bar; the fill reveals part of it
#endif

class UI_ELEMENT_PROGRESSBAR : public UI_ELEMENT_OPTION
{
  public:
					                  UI_ELEMENT_PROGRESSBAR				    ();
    virtual							 	 ~UI_ELEMENT_PROGRESSBAR			      ();

    UI_COLOR*						    GetLineColor								      ();

    UI_COLOR*               GetGradientColor                  ();
    int                     GetGradientMode                   ();
    void                    SetGradientMode                   (int gradientmode);

    bool                    GetRoundCap                       ();
    void                    SetRoundCap                       (bool roundcap);

    UI_ELEMENT*             GetProgressRect                   ();
    void                    SetProgressRect                   (UI_ELEMENT* progressrect);

    bool                    ContinuousCycle_Is                ();
    void                    ContinuousCycle_Set               (bool iscontinuouscycle, XDWORD sizesegment = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSIZESEGMENT, XDWORD stepsegment = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTSTEPSEGMENT, XDWORD timestep = UI_ELEMENT_PROGRESSBAR_CCYCLE_DEFAULTTIMESTEP);    
    XDWORD                  ContinuousCycle_GetSizeSegment    ();
    XDWORD                  ContinuousCycle_GetStepSegment    ();    
    XDWORD                  ContinuousCycle_GetNSteps         ();
    void                    ContinuousCycle_SetNSteps         (XDWORD nstep);
    XTIMER*                 ContinuousCycle_GetXTimer         ();
    XDWORD                  ContinuousCycle_GetTimeStep       ();
           	
    float                   GetLevel                          ();
    void                    SetLevel                          (float level);
   
  private:

		void								    Clean												      ();

    float                   level;

    UI_COLOR                linecolor;
    UI_COLOR                gradientcolor;
    int                     gradientmode;
    bool                    roundcap;

    UI_ELEMENT*             progressrect; 

    bool                    continuouscycle_is;
    XDWORD                  continuouscycle_sizesegment;
    XDWORD                  continuouscycle_stepsegment;
    XDWORD                  continuouscycle_nsteps;
    XTIMER*                 continuouscycle_xtimer;                  
    XDWORD                  continuouscycle_timestep;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





