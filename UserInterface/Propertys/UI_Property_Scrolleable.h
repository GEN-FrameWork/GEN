/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Scrolleable.h
* 
* @class      UI_PROPERTY_SCROLLEABLE
* @brief      User Interface Element Scrolleable class
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

#include "UI_BoundaryLine.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class UI_COLOR;

enum UI_PROPERTY_SCROLLEABLE_TYPE
{
  UI_PROPERTY_SCROLLEABLE_TYPE_UNKWOWN          = 0 ,
  UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL           ,
  UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL             ,
  UI_PROPERTY_SCROLLEABLE_TYPE_ALL                  ,
};


enum UI_OVERFLOW_MODE
{
  UI_OVERFLOW_VISIBLE                           = 0 ,   // no clip, no scrollbar
  UI_OVERFLOW_HIDDEN                                ,   // clip,    no scrollbar
  UI_OVERFLOW_SCROLL                               ,   // clip,    scrollbar always
  UI_OVERFLOW_AUTO                                 ,   // clip,    scrollbar only if content exceeds viewport
};

enum UI_PROPERTY_SCROLLEABLE_ACTIVATE
{
  UI_PROPERTY_SCROLLEABLE_ACTIVATE_AUTO         = 0 ,   // activation follows the overflow policy (default)
  UI_PROPERTY_SCROLLEABLE_ACTIVATE_ON               ,   // user forces the axis active   (scroll mechanics enabled)
  UI_PROPERTY_SCROLLEABLE_ACTIVATE_OFF              ,   // user forces the axis inactive (no scroll, no bar)
};

#define UI_PROPERTY_SCROLLEABLE_BARWIDTH        6        // overlay scrollbar thickness (px). Single source for draw + hit-test.

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class UI_PROPERTY_SCROLLEABLE_STATUS
{
  public:
																		          UI_PROPERTY_SCROLLEABLE_STATUS	        ();
    virtual											         	   ~UI_PROPERTY_SCROLLEABLE_STATUS          ();

    bool                                      active;
    bool                                      visible;
    UI_PROPERTY_SCROLLEABLE_ACTIVATE          activateoverride;
    double                                    limit;        
    double                                    displacement;    
    double                                    step;                     
    UI_OVERFLOW_MODE                          overflow;
    double                                    barwidth;
    bool                                      showbar;
    int                                       thumbcolor[4];
    int                                       trackcolor[4];
    double                                    sepedge;
    double                                    sepalong;

  private:

    void															        Clean										                ();              
};

class UI_PROPERTY_SCROLLEABLE 
{
  public:
																		          UI_PROPERTY_SCROLLEABLE		              ();
    virtual											         	   ~UI_PROPERTY_SCROLLEABLE	  	            ();
        
    bool                                      Scroll_GetActive                        (UI_PROPERTY_SCROLLEABLE_TYPE type);
    bool                                      Scroll_SetActivate                      (UI_PROPERTY_SCROLLEABLE_TYPE type, bool on);
    bool                                      Scroll_SetActivateAuto                  (UI_PROPERTY_SCROLLEABLE_TYPE type);
    UI_PROPERTY_SCROLLEABLE_ACTIVATE          Scroll_GetActivateMode                  (UI_PROPERTY_SCROLLEABLE_TYPE type);

    bool                                      Scroll_GetVisible                       (UI_PROPERTY_SCROLLEABLE_TYPE type);
    bool                                      Scroll_SetVisible                       (UI_PROPERTY_SCROLLEABLE_TYPE type, bool on);

    double                                    Scroll_GetLimit                         (UI_PROPERTY_SCROLLEABLE_TYPE type);
    void                                      Scroll_SetLimit                         (UI_PROPERTY_SCROLLEABLE_TYPE type, double limit);

    double                                    Scroll_GetDisplacement                  (UI_PROPERTY_SCROLLEABLE_TYPE type);

    double                                    Scroll_GetStep                          (UI_PROPERTY_SCROLLEABLE_TYPE type);
    void                                      Scroll_SetStep                          (UI_PROPERTY_SCROLLEABLE_TYPE type, double h_step);

    void                                      Scroll_SetFraction                      (UI_PROPERTY_SCROLLEABLE_TYPE type, double fraction);

    void                                      Scroll_SetBarWidth                      (UI_PROPERTY_SCROLLEABLE_TYPE type, double width);
    double                                    Scroll_GetBarWidth                      (UI_PROPERTY_SCROLLEABLE_TYPE type);

    void                                      Scroll_SetBarShown                      (UI_PROPERTY_SCROLLEABLE_TYPE type, bool shown);
    bool                                      Scroll_GetBarShown                      (UI_PROPERTY_SCROLLEABLE_TYPE type);

    void                                      Scroll_SetBarColors                     (UI_PROPERTY_SCROLLEABLE_TYPE type, UI_COLOR& thumbcolor, UI_COLOR& trackcolor);
    void                                      Scroll_GetBarThumbColor                 (UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a);
    void                                      Scroll_GetBarTrackColor                 (UI_PROPERTY_SCROLLEABLE_TYPE type, int& r, int& g, int& b, int& a);

    void                                      Scroll_SetBarSeparation                 (UI_PROPERTY_SCROLLEABLE_TYPE type, double separationedge, double separationalong);
    void                                      Scroll_GetBarSeparation                 (UI_PROPERTY_SCROLLEABLE_TYPE type, double& separationedge, double& separationalong);

    bool                                      Scroll_GetTrackRect                     (UI_PROPERTY_SCROLLEABLE_TYPE type, double box_left, double box_top, double box_right, double box_bottom, double radius, double& track_left, double& track_top, double& track_right, double& track_bottom);

    void                                      Scroll_ResetStep                        (UI_PROPERTY_SCROLLEABLE_TYPE type = UI_PROPERTY_SCROLLEABLE_TYPE_ALL);


    UI_OVERFLOW_MODE                          Scroll_GetOverflow                      (UI_PROPERTY_SCROLLEABLE_TYPE type);
    void                                      Scroll_SetOverflow                      (UI_PROPERTY_SCROLLEABLE_TYPE type, UI_OVERFLOW_MODE mode);

    bool                                      Scroll_NeedClip                         ();



    bool                                      Scroll_ResolvePolicy                    (UI_PROPERTY_SCROLLEABLE_TYPE type, double viewport, double contentextent);



    double                                    Scroll_GetContentViewport               (UI_PROPERTY_SCROLLEABLE_TYPE type, double fullviewport);
      	
  private:

		void															        Clean										                ();  


    UI_PROPERTY_SCROLLEABLE_STATUS            horizontal;  
    UI_PROPERTY_SCROLLEABLE_STATUS            vertical;  
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




