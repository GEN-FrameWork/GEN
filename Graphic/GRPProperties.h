/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPProperties.h
* 
* @class      GRPPROPERTIES
* @brief      Graphics Properties class
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

#include "XBase.h"

#include "GRPRect.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPPROPERTYMODE
{
  GRPPROPERTYMODE_XX_UNKNOWN          = 0 ,
  GRPPROPERTYMODE_08_INDEX              ,
  GRPPROPERTYMODE_16_RGBA_4444          ,
  GRPPROPERTYMODE_16_RGBA_5551          ,
  GRPPROPERTYMODE_16_RGB_565            ,
  GRPPROPERTYMODE_16_SRGB_565           ,
  GRPPROPERTYMODE_24_RGB_888            ,
  GRPPROPERTYMODE_24_BGR_888            ,
  GRPPROPERTYMODE_32_RGBA_8888          ,
  GRPPROPERTYMODE_32_BGRA_8888          ,
};

#define GRPPROPERTYMODE_SCREEN_CENTER    100000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPPROPERTIES
{
  public:

                            GRPPROPERTIES               ();
    virtual                ~GRPPROPERTIES               ();

    GRPPROPERTYMODE         GetMode                     ();
    void                    SetMode                     (GRPPROPERTYMODE mode);

    int                     GetPositionX                ();
    int                     GetPositionY                ();
    void                    SetPosition                 (int x, int y);

    XDWORD                  GetWidth                    ();
    void                    SetWidth                    (XDWORD width);

    XDWORD                  GetHeight                   ();
    void                    SetHeight                   (XDWORD height);

    void                    SetSize                     (XDWORD width, XDWORD height);

    XDWORD                  GetMaxWidth                 ();
    void                    SetMaxWidth                 (XDWORD width);

    XDWORD                  GetMaxHeight                ();
    void                    SetMaxHeight                (XDWORD height);

    void                    SetMaxSize                  (XDWORD maxwidth, XDWORD maxheight);

    GRPRECTINT*             GetWorkRegion               ();

    float                   GetDPI                      ();
    void                    SetDPI                      (float DPI);

    XDWORD                  GetStride                   ();
    void                    SetStride                   (XDWORD stride);

    bool                    IsBufferInverse             ();
    void                    SetIsBufferInverse          (bool isbufferinverse);

    XBYTE                   GetBitsperPixel             ();
    XBYTE                   GetBytesperPixel            ();

    void                    CopyPropertysFrom           (GRPPROPERTIES* properties);
    void                    CopyPropertysTo             (GRPPROPERTIES* properties);

    int                     IsEqualSizeTo               (GRPPROPERTIES* properties);

    void                    Default                     ();

  protected:

    GRPPROPERTYMODE         mode;
    int                     positionx;
    int                     positiony;
    XDWORD                  width;
    XDWORD                  height;
    XDWORD                  maxwidth;
    XDWORD                  maxheight;
    GRPRECTINT              workregion;
    float                   DPIs;
    XDWORD                  stride;
    bool                    isbufferinverse;

  private:

    void                    Clean                       ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





