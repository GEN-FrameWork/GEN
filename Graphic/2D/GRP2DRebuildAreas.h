/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DRebuildAreas.h
* 
* @class      GRP2DREBUILDAREAS
* @brief      Graphics 2D Rebuild Areas class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRP2DREBUILDAREAS_DEFAULT_EXCESSEDGE   10

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVAS;
class GRPBITMAP;

class GRP2DREBUILDAREA
{
  public:
                                      GRP2DREBUILDAREA        ();
    virtual                          ~GRP2DREBUILDAREA        ();

    double                            GetXPos                 ();
    double                            GetYPos                 ();

    void                              SetXPos                 (double xpos);
    void                              SetYPos                 (double ypos);

    GRPBITMAP*                        GetBitmap               ();
    void                              SetBitmap               (GRPBITMAP* bitmap);

    void*                             GetExtraData            ();
    void                              SetExtraData            (void* extra_data);

  protected:

    double                            xpos;
    double                            ypos;

    GRPBITMAP*                        bitmap;

    void*                             extra_data;

  private:

    void                              Clean                   ();
};

class GRP2DREBUILDAREAS
{
  public:
                                      GRP2DREBUILDAREAS       ();
    virtual                          ~GRP2DREBUILDAREAS       ();

    XDWORD                            GetExcessEdge           ();
    bool                              SetExcessEdge           (XDWORD excessedge);

    XVECTOR<GRP2DREBUILDAREA*>*       GetRebuildAreas         ();
    virtual bool                      RebuildAllAreas         ();
    bool                              DeleteAllRebuildAreas   ();

    bool                              CreateRebuildArea       (double x, double y, double width, double height, void* extra_data = NULL);

    virtual GRPBITMAP*                GetBitmap               (double x, double y, double width, double height);
    virtual void                      PutBitmapNoAlpha        (double x, double y, GRPBITMAP* bitmap);

  protected: 

    XDWORD                            excessedge;
    XVECTOR<GRP2DREBUILDAREA*>        areas;

  private:

    void                              Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





