/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_BoundaryLine.h
*
* @class      UI_BOUNDARYLINE
* @brief      User Interface Boundary Line class
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

#include "XBase.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------*/
/*  SINGLE COORDINATE CONVERSION POINT  (Y-axis edge resolution)                                                     */
/*                                                                                                                   */
/*  These two functions are THE ONLY place in the User Interface module that knows how a stored vertical anchor      */
/*  (a boundaryline 'y' together with its 'height') maps to the top and bottom screen edges. Every draw, hit-test,   */
/*  clip and rebuild-area call site goes through the GetTop()/GetBottom() accessors below (or the matching element   */
/*  accessors), which in turn call these functions. No other code performs 'y - height' by hand anymore.             */
/*                                                                                                                   */
/*  CURRENT CONVENTION (unchanged by the 2a "seam" commit, behaviour-preserving):                                    */
/*    Screen pixels, origin top-left, +X right, +Y down.                                                             */
/*    The stored 'y' is the element BOTTOM edge (its baseline); the element grows UPWARDS, occupying [y-height, y].  */
/*                                                                                                                   */
/*  THE 2b "flip" commit will change ONLY the body of these two functions (to a top-left convention: top = y,        */
/*  bottom = y + height), plus CalculePosition's output and the vector-font baseline. Nothing else needs editing.    */
/*-------------------------------------------------------------------------------------------------------------------*/

inline double UI_BOUNDARYLINE_EdgeTop    (double y, double height)    { return y - height; }
inline double UI_BOUNDARYLINE_EdgeBottom (double y, double height)    { (void)height; return y; }

inline double UI_BOUNDARYLINE_EdgeLeft   (double x, double width)     { (void)width; return x; }
inline double UI_BOUNDARYLINE_EdgeRight  (double x, double width)     { return x + width; }

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_BOUNDARYLINE
{
  public:
                     UI_BOUNDARYLINE    ();
    virtual         ~UI_BOUNDARYLINE    ();

    void             CopyFrom           (UI_BOUNDARYLINE& boundaryline);
    void             CopyTo             (UI_BOUNDARYLINE& boundaryline);

    bool             SetAround          (UI_BOUNDARYLINE& boundaryline);
    bool             IsWithin           (XDWORD xpos, XDWORD ypos);

    void             ExpandCenteredToMin(double min_width, double min_height);

    bool             IsEmpty            ();


    double           GetLeft            ()    { return UI_BOUNDARYLINE_EdgeLeft  (x, width);  }
    double           GetRight           ()    { return UI_BOUNDARYLINE_EdgeRight (x, width);  }
    double           GetTop             ()    { return UI_BOUNDARYLINE_EdgeTop   (y, height); }
    double           GetBottom          ()    { return UI_BOUNDARYLINE_EdgeBottom(y, height); }

    double           x;
    double           y;
    double           width;
    double           height;

  private:

    void             Clean              ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
