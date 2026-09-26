/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DCanvas.h
* 
* @class      GRP2DCANVAS
* @brief      Graphics Canvas class
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

#include "XString.h"
#include "XPath.h"
#include "XRect.h"
#include "XVector.h"

#include "GRP2DColor.h"
#include "GRP2DVertexs.h"
#include "GRP2DPath.h"
#include "GRP2DGradientStop.h"
#include "GRP2DRebuildAreas.h"
#include "GRPProperties.h"
#include "GRPRect.h"
#include "GRPBitmapSequence.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRP2DCANVASTYPE
{
  GRP2DCANVASTYPE_UNKNOW    =0  ,
  GRP2DCANVASTYPE_AGGLIB        ,
};

#define GRP2DCANVASTYPE_DRAWFRAMERATE_ADJUSTSCREEN  -1.0f

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSCREEN;
class GRPVIEWPORT;
class GRPBITMAP;
class GRPBITMAPFRAME;
class GRP2DREBUILDAREAS;

class GRP2DCANVAS_VECTORFONT_CONFIG
{
  public:
                                  GRP2DCANVAS_VECTORFONT_CONFIG         ();    
    virtual                      ~GRP2DCANVAS_VECTORFONT_CONFIG         ();
    
    bool                          GetSize                             (double& width, double& height);
    bool                          SetSize                             (double width, double height);
    
    double                        GetSize                             ();
    bool                          SetSize                             (double size);
    
    double                        GetWidth                            ();
    void                          SetWidth                            (double width);
    
    double                        GetHeight                           ();
    void                          SetHeight                           (double height);

    GRP2DCOLOR_RGBA8*             GetColor                            ();
    void                          SetColor                            (GRP2DCOLOR_RGBA8* color);

    bool                          IsKerning                           ();
    void                          SetKerning                          (bool iskerning);
    
  private:

    void                          Clean                               ();

    double                        width;    
    double                        height;    
    GRP2DCOLOR_RGBA8*             color;
    bool                          iskerning;
};

class GRP2DCANVAS : public GRPPROPERTIES, public GRP2DREBUILDAREAS
{
  public:                            
                                      GRP2DCANVAS                         ();
    virtual                          ~GRP2DCANVAS                         ();
                                     
    virtual bool                      Buffer_Create                       ();   
    XBYTE*                            Buffer_Get                          ();
    XDWORD                            Buffer_GetSize                      ();
    virtual bool                      Buffer_SetToZero                    (); 
    virtual bool                      Buffer_Delete                       ();
  
    virtual void                      Clear                               (const GRP2DCOLOR* color);

    virtual void                      GetClipBox                          (double& x1, double& y1, double& x2, double& y2);
    virtual void                      GetClipBox                          (XRECT& rect);
    virtual XRECT&                    GetClipBox                          ();
    virtual void                      SetClipBox                          (double x1, double y1, double x2, double y2);
    virtual void                      SetClipBox                          (XRECT& rect);

    virtual GRP2DCOLOR*               GetPixel                            (double x, double y);
    virtual void                      PutPixel                            (double x, double y, const GRP2DCOLOR* color);
    virtual void                      PutBlendPixel                       (double x, double y, const GRP2DCOLOR* color, double alpha);

    virtual GRP2DCOLOR*               GetFillColor                        ();
    virtual void                      SetFillColor                        (const GRP2DCOLOR* color);

    virtual GRP2DCOLOR*               GetLineColor                        ();
    virtual void                      SetLineColor                        (const GRP2DCOLOR* color);

    virtual double                    GetLineWidth                        ();
    virtual void                      SetLineWidth                        (double linewidth);

    virtual double                    GetDashLength                       ();
    virtual void                      SetDashLenght                       (double dashlength);

    virtual void                      Line                                (double x1, double y1, double x2, double y2);

    virtual void                      Rectangle                           (double x1, double y1, double x2, double y2, bool isfill = false);

    void                              Circle                              (double x, double y, double radius, bool isfill = false);
    virtual void                      Ellipse                             (double x, double y, double rx, double ry, bool isfill = false);

    virtual void                      Polygon                             (GRP2DVERTEXS& vertexs, bool isfill);

    virtual void                      Path                                (GRP2DPATH& path, bool isfill = false);
    virtual void                      PathGradientLinear                  (GRP2DPATH& path, bool evenodd, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops);
    virtual void                      PathGradientRadial                  (GRP2DPATH& path, bool evenodd, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops);
    virtual void                      PathGradientLinearStroke            (GRP2DPATH& path, double linewidth, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops);
    virtual void                      PathGradientRadialStroke            (GRP2DPATH& path, double linewidth, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops);

    virtual void                      RoundRect                           (double x1, double y1, double x2, double y2, double radius, bool isfill = false);

    virtual GRPBITMAP*                GetBitmap                           (double x, double y, double width, double height);

    virtual void                      PutBitmapNoAlpha                    (double x, double y, GRPBITMAP* bitmap);
    virtual void                      PutBitmap                           (double x, double y, GRPBITMAP* bitmap);
    virtual void                      PutBitmapAlpha                      (double x, double y, GRPBITMAP* bitmap, double alpha = 100.0f);
    void                              PutBitmapFrame                      (double x, double y, GRPBITMAPFRAME* bitmapframe, double alpha = 100.0f);

    virtual bool                      RasterFont_Select                   (XCHAR* fontname);
    bool                              RasterFont_Select                   (XSTRING* fontname);
    virtual GRP2DCOLOR*               RasterFont_GetColor                 ();
    virtual void                      RasterFont_SetColor                 (const GRP2DCOLOR* c);
    virtual double                    RasterFont_GetWidth                 (XCHAR* string);
    virtual double                    RasterFont_GetHeight                ();
    virtual bool                      RasterFont_Printf                   (double x, double y, XCHAR* mask, ...);

    bool                              VectorFont_Load                     (XCHAR* fontnamefile);
    virtual bool                      VectorFont_Load                     (XPATH& fontnamefile);
    GRP2DCANVAS_VECTORFONT_CONFIG*    Vectorfont_GetConfig                ();
    XPATH*                            VectorFont_GetPathFile              ();
    bool                              VectorFont_CopyFrom                 (GRP2DCANVAS* canvas);
    virtual double                    VectorFont_GetWidth                 (XCHAR* string);
    virtual double                    VectorFont_GetHeight                (XCHAR* string);
    virtual double                    VectorFont_GetHeight                ();
    
    virtual bool                      VectorFont_Print                    (double x, double y, XCHAR* outstring);
    virtual bool                      VectorFont_PrintAngle               (double x, double y, double angle, XCHAR* outstring)                                                                      
                                      { 
                                        return VectorFont_Print(x, y, outstring); 
                                      }

    virtual bool                      VectorFont_Printf                   (double x, double y, XCHAR* mask, ...);

    virtual bool                      CopyBufferRenderToScreen            (GRPSCREEN* screen);
    virtual bool                      CopyBufferRenderToBufferScreen      (XBYTE* buffer, XDWORD width, XDWORD height, bool isinverse);
    virtual bool                      CopyBufferRenderFromViewport        (GRPVIEWPORT* viewport);

    virtual bool                      ReleaseDrawFramerate                ();
    virtual bool                      DrawFramerate                       (GRPSCREEN* screen, double x, double y);

  protected:

    XBYTE*                            buffer;
    XDWORD                            buffersize;

    XRECT                             lastcliprect;

    double                            linewidth;
    double                            dashlength;

    GRP2DCANVAS_VECTORFONT_CONFIG     vectorfont_config;
    XPATH                             vectorfont_pathfile;

  private:

    void                              Clean                               ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




