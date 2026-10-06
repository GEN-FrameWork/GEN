/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DCanvasAGG.h
* 
* @class      GRP2DCANVASAGG
* @brief      Graphics Canvas AGG Library class
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


/*---- AGG ANTI-ALIASING / QUALITY TUNING ----------------------------------------------------------------------------*/

// Ajustes globales de calidad para AGG.
//
// Valores recomendados para empezar:
//   Balanced:  curve=8.0,  gamma=1.25, min_width=0.50, smoother_width=0.50
//   AA_MAX:    curve=32.0, gamma=1.50, min_width=1.00, smoother_width=1.00
//
// Nota: GRP2DCANVASAGG_AA_GAMMA_POWER = 1.0 es el comportamiento neutro/lineal.
// No usar 0.0 como valor "neutro" porque gamma_power(0.0) elimina visualmente parte del AA.

#ifndef GRP2DCANVASAGG_AA_CURVE_APPROXIMATION_SCALE
#define GRP2DCANVASAGG_AA_CURVE_APPROXIMATION_SCALE             8.0
#endif

#ifndef GRP2DCANVASAGG_AA_GAMMA_POWER
#define GRP2DCANVASAGG_AA_GAMMA_POWER                           1.25
#endif

#ifndef GRP2DCANVASAGG_AA_OUTLINE_MIN_WIDTH
#define GRP2DCANVASAGG_AA_OUTLINE_MIN_WIDTH                     0.50
#endif

#ifndef GRP2DCANVASAGG_AA_OUTLINE_SMOOTHER_WIDTH
#define GRP2DCANVASAGG_AA_OUTLINE_SMOOTHER_WIDTH                0.50
#endif

#define GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve)              curve.approximation_scale(GRP2DCANVASAGG_AA_CURVE_APPROXIMATION_SCALE);
#define GRP2DCANVASAGG_AA_SET_RASTERIZER_GAMMA(ras)             ras.gamma(agg::gamma_power(GRP2DCANVASAGG_AA_GAMMA_POWER));


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <cmath>

#include "agg_basics.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_raster_text.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_curve.h"
#include "agg_path_storage.h"
#include "agg_conv_contour.h"
#include "agg_gamma_lut.h"
#include "agg_ellipse.h"
#include "agg_rounded_rect.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_glyph_raster_bin.h"
#include "agg_pixfmt_rgb.h"
#include "agg_font_freetype.h"

#include "agg_span_gradient.h"
#include "agg_gradient_lut.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_allocator.h"
#include "agg_trans_affine.h"

#include "GRP2DGradientStop.h"

#include "XBase.h"
#include "XString.h"
#include "XPath.h"
#include "XBuffer.h"
#include "XMap.h"

#include "GRPFactory.h"
#include "GRP2DColor.h"
#include "GRPRect.h"
#include "GRPProperties.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"
#include "GRP2DCanvas.h"
#include "GRPViewPort.h"
#include "GRPFrameRate.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define AGG_OUTLINE_INI                                       agg::line_profile_aa                                                                             profile;                         \
                                                              agg::renderer_outline_aa<agg::renderer_base<PIXELFORMATBUFFER> >                                 ren(*renderer_base, profile);    \
                                                              agg::rasterizer_outline_aa<agg::renderer_outline_aa<agg::renderer_base<PIXELFORMATBUFFER> > >    ras(ren);                        \
                                                              agg::line_profile_aa(width, agg::gamma_none());                                                                                   \
                                                              profile.gamma(agg::gamma_power(GRP2DCANVASAGG_AA_GAMMA_POWER));                                                                                               \
                                                              profile.min_width(GRP2DCANVASAGG_AA_OUTLINE_MIN_WIDTH);                                                                                                           \
                                                              profile.smoother_width(GRP2DCANVASAGG_AA_OUTLINE_SMOOTHER_WIDTH);                                                                                                      \
                                                              profile.width(linewidth);

#define AGG_OUTLINE_END                                       ras.render(true);

#define AGG_SOLIDFILL_INI                                     agg::rasterizer_scanline_aa<>                                                                    ras;                             \
                                                              agg::scanline_p8                                                                                 sl;                              \
                                                              agg::renderer_scanline_aa_solid<agg::renderer_base<PIXELFORMATBUFFER> >                          ren(*renderer_base);             \
                                                              agg::renderer_scanline_bin_solid<agg::renderer_base<PIXELFORMATBUFFER> >                         ren_bin(*renderer_base);

#define AGG_SOLIDFILL_END                                     agg::render_scanlines(ras, sl, ren);

typedef agg::glyph_raster_bin<agg::rgba8>                     AGG_GLYPH_GEN;

typedef agg::font_engine_freetype_int32                       AGG_FONT_ENGINE;
typedef agg::font_cache_manager<AGG_FONT_ENGINE>              AGG_FONT_MANAGER;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE
{
  public:
                                                                                GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE   ()
                                                                                {
                                                                                  m_num_vertices  = 0;
                                                                                  m_count         = 0;
                                                                                  m_x[0]          = 0.0f;
                                                                                  m_y[0]          = 0.0f;

                                                                                  m_cmd[0]        = agg::path_cmd_stop;
                                                                                }

                                                                                GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE  (double x1, double y1, double x2, double y2)
                                                                                {
                                                                                  init(x1, y1, x2, y2);
                                                                                }

                                                                                GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE  (double x1, double y1, double x2, double y2, double x3, double y3)
                                                                                {
                                                                                  init(x1, y1, x2, y2, x3, y3);
                                                                                }

    void                                                                        init                          (double x1, double y1, double x2, double y2)
                                                                                {
                                                                                  m_num_vertices = 2;
                                                                                  m_count        = 0;

                                                                                  m_x[0] = x1;
                                                                                  m_y[0] = y1;
                                                                                  m_x[1] = x2;
                                                                                  m_y[1] = y2;

                                                                                  m_cmd[0] = agg::path_cmd_move_to;
                                                                                  m_cmd[1] = agg::path_cmd_line_to;
                                                                                  m_cmd[2] = agg::path_cmd_stop;
                                                                                }

    void                                                                        init                          (double x1, double y1, double x2, double y2, double x3, double y3)
                                                                                {
                                                                                  m_num_vertices = 3;
                                                                                  m_count        = 0;

                                                                                  m_x[0] = x1;
                                                                                  m_y[0] = y1;
                                                                                  m_x[1] = x2;
                                                                                  m_y[1] = y2;
                                                                                  m_x[2] = x3;
                                                                                  m_y[2] = y3;

                                                                                  m_x[3] = m_y[3] = m_x[4] = m_y[4] = 0.0;

                                                                                  m_cmd[0] = agg::path_cmd_move_to;
                                                                                  m_cmd[1] = agg::path_cmd_line_to;
                                                                                  m_cmd[2] = agg::path_cmd_line_to;
                                                                                  m_cmd[3] = agg::path_cmd_end_poly | agg::path_flags_close;
                                                                                  m_cmd[4] = agg::path_cmd_stop;
                                                                                }


    void                                                                        rewind                        (unsigned)
                                                                                {
                                                                                    m_count = 0;
                                                                                }

    unsigned                                                                    vertex                        (double* x, double* y)
                                                                                {
                                                                                  *x = m_x[m_count];
                                                                                  *y = m_y[m_count];

                                                                                  return m_cmd[m_count++];
                                                                                }

  private:

    unsigned                                                                    m_num_vertices;
    unsigned                                                                    m_count;
    double                                                                      m_x[8];
    double                                                                      m_y[8];
    unsigned                                                                    m_cmd[8];
};

template<class RASTERIZERTYPE, class RENDERER, class SCANLINETYPE>
class GRP2DCANVASAGG_DASHED_LINE
{
  public:
                                                                                GRP2DCANVASAGG_DASHED_LINE      (RASTERIZERTYPE& rasterizertype, RENDERER& renderer, SCANLINETYPE& scanlinetype) :
                                                                                                                m_ras(rasterizertype)   ,
                                                                                                                m_ren(renderer)         ,
                                                                                                                m_sl(scanlinetype)      ,
                                                                                                                m_src()                 ,
                                                                                                                m_dash(m_src)           ,
                                                                                                                m_stroke(m_src)         ,
                                                                                                                m_dash_stroke(m_dash)
                                                                                {

                                                                                }

    void                                                                        Draw                          (double x1, double y1, double x2, double y2, double line_width, double dash_length)
                                                                                {

                                                                                  m_src.init(x1, y1, x2, y2);
                                                                                  m_ras.reset();

                                                                                  if(dash_length > 0.0)
                                                                                    {
                                                                                      m_dash.remove_all_dashes();
                                                                                      m_dash.add_dash(dash_length, dash_length);

                                                                                      m_dash_stroke.width(line_width);
                                                                                      m_dash_stroke.line_cap(agg::square_cap);
                                                                                      m_ras.add_path(m_dash_stroke);
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      m_stroke.width(line_width);
                                                                                      m_stroke.line_cap(agg::square_cap);
                                                                                      m_ras.add_path(m_stroke);
                                                                                    }

                                                                                  agg::render_scanlines(m_ras, m_sl, m_ren);
                                                                                }
  private:

    RASTERIZERTYPE&                                                             m_ras;
    RENDERER&                                                                   m_ren;
    SCANLINETYPE&                                                               m_sl;

    GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE                                           m_src;
    agg::conv_dash<GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE>                           m_dash;
    agg::conv_stroke<GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE>                         m_stroke;
    agg::conv_stroke<agg::conv_dash<GRP2DCANVASAGG_SIMPLE_VERTEX_SOURCE> >        m_dash_stroke;
};

template<class PIXELFORMATBUFFER, class COLORTYPE>
class GRP2DCANVASAGG: public GRP2DCANVAS
{
  public:
                                                                                GRP2DCANVASAGG                      ()  : GRP2DCANVAS()
                                                                                {
                                                                                  Clean();

                                                                                  vectorfont_manager = GEN_NEW AGG_FONT_MANAGER(vectorfont_engine); 
                                                                                }

    virtual                                                                    ~GRP2DCANVASAGG                      ()
                                                                                {                                                                                  
                                                                                  if(vectorfont_manager) 
                                                                                    {
                                                                                      GEN_DELETE vectorfont_manager;
                                                                                      vectorfont_manager = NULL;       
                                                                                    }

                                                                                  if(framerate_bitmap)
                                                                                    {                                                                                  
                                                                                      GEN_DELETE framerate_bitmap;
                                                                                      framerate_bitmap = NULL;
                                                                                    }

                                                                                  spriteregresionmap.DeleteElementContents();
                                                                                  spriteregresionmap.DeleteAll();

                                                                                  Buffer_Delete();
                                                                                  Clean();
                                                                                }

    bool                                                                        Buffer_Create                     ()
                                                                                {
                                                                                  buffersize = (width * height * GetBytesperPixel());
                                                                                  if(!buffersize) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  buffer = GEN_NEW XBYTE[buffersize];
                                                                                  if(!buffer)
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  memset(buffer, 0, buffersize);

                                                                                  rbuffer.attach(buffer, width , height, (IsBufferInverse()?1:-1)*((int)width * GetBytesperPixel()));

                                                                                  pixelformatbuffer = GEN_NEW PIXELFORMATBUFFER(rbuffer);
                                                                                  if(!pixelformatbuffer) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  renderer_base =  GEN_NEW agg::renderer_base<PIXELFORMATBUFFER>(*pixelformatbuffer);
                                                                                  if(!renderer_base)  
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  renderer_scanline =  GEN_NEW agg::renderer_scanline_aa_solid<agg::renderer_base<PIXELFORMATBUFFER> >(*renderer_base);
                                                                                  if(!renderer_scanline) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  renderer_primitives = GEN_NEW agg::renderer_primitives<agg::renderer_base<PIXELFORMATBUFFER> >(*renderer_base);
                                                                                  if(!renderer_primitives) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  dashline = GEN_NEW GRP2DCANVASAGG_DASHED_LINE<agg::rasterizer_scanline_aa<>,
                                                                                                                          agg::renderer_scanline_aa_solid<agg::renderer_base<PIXELFORMATBUFFER> >,
                                                                                                                          agg::scanline_u8 > (rasterizer_type, *renderer_scanline, scanline_type);

                                                                                  rasterglyph =  GEN_NEW AGG_GLYPH_GEN(0);
                                                                                  if(!rasterglyph) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  renderer_rastertext = GEN_NEW agg::renderer_raster_htext_solid<agg::renderer_base<PIXELFORMATBUFFER>, AGG_GLYPH_GEN> (*renderer_base, *rasterglyph);
                                                                                  if(!renderer_rastertext) 
                                                                                    {
                                                                                      return false;
                                                                                    }
                                                                                  
                                                                                  SetClipBox((double)0.0f, (double)0.0f, (double)(width-1), (double)(height-1));

                                                                                  return true;
                                                                                }

  bool                                                                          Buffer_SetToZero                  ()
                                                                                {
                                                                                  if(!buffer) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  memset(buffer, 0, width * height * GetBytesperPixel());

                                                                                  return true;
                                                                                }

    bool                                                                        Buffer_Delete                      ()
                                                                                {

                                                                                  if(renderer_rastertext)
                                                                                    {
                                                                                      GEN_DELETE renderer_rastertext;
                                                                                      renderer_rastertext = NULL;
                                                                                    }

                                                                                  if(rasterglyph)
                                                                                    {
                                                                                      GEN_DELETE rasterglyph;
                                                                                      rasterglyph = NULL;
                                                                                    }

                                                                                  if(dashline)
                                                                                    {
                                                                                      GEN_DELETE dashline;
                                                                                      dashline = NULL;
                                                                                    }

                                                                                  if(renderer_scanline)
                                                                                    {
                                                                                      GEN_DELETE renderer_scanline;
                                                                                      renderer_scanline = NULL;
                                                                                    }

                                                                                  if(renderer_primitives)
                                                                                    {
                                                                                      GEN_DELETE renderer_primitives;
                                                                                      renderer_primitives = NULL;
                                                                                    }

                                                                                  if(renderer_base)
                                                                                    {
                                                                                      GEN_DELETE renderer_base;
                                                                                      renderer_base = NULL;
                                                                                    }

                                                                                  if(pixelformatbuffer)
                                                                                    {
                                                                                      GEN_DELETE pixelformatbuffer;
                                                                                      pixelformatbuffer = NULL;
                                                                                    }

                                                                                  if(buffer)
                                                                                    {
                                                                                      GEN_DELETE_ARRAY buffer;
                                                                                      buffer = NULL;
                                                                                    }

                                                                                  return true;
                                                                                }

    void                                                                        Clear                             (const GRP2DCOLOR* c)                     { renderer_base->clear((*(COLORTYPE*)c));                       }

    void                                                                        SetClipBox                        (double x1, double y1, double x2, double y2)
                                                                                {
                                                                                  GRP2DCANVAS::SetClipBox(x1, y1, x2, y2);                                                                                                                                                                        
                                                                                  renderer_base->clip_box((int)x1, (int)y1, (int)x2, (int)y2);
                                                                                }

    void                                                                        SetClipBox                        (XRECT& rect)
                                                                                {
                                                                                  GRP2DCANVAS::SetClipBox(rect); 
                                                                                  renderer_base->clip_box((int)rect.x1, (int)rect.y1, (int)rect.x2, (int)rect.y2);
                                                                                }

    GRP2DCOLOR*                                                                 GetPixel                          (double x, double y)
                                                                                {
                                                                                  color = renderer_base->pixel((int)x, (int)y);
                                                                                  return (GRP2DCOLOR*)&color;
                                                                                }

    void                                                                        PutPixel                          (double x, double y, const GRP2DCOLOR* color)   { renderer_base->copy_pixel((int)x, (int)y, (*(COLORTYPE*)color));        }

    void                                                                        PutBlendPixel                     (double x, double y, const GRP2DCOLOR* color, double alpha)
                                                                                {
                                                                                  renderer_base->blend_pixel((int)x, (int)y, (*(COLORTYPE*)color), (int)alpha);
                                                                                }

    GRP2DCOLOR*                                                                 GetFillColor                      ()                                        { return (GRP2DCOLOR*)(&renderer_primitives->fill_color());     }
    void                                                                        SetFillColor                      (const GRP2DCOLOR* c)                     { renderer_primitives->fill_color((*(COLORTYPE*)c));            }

    GRP2DCOLOR*                                                                 GetLineColor                      ()                                        { return (GRP2DCOLOR*)&renderer_primitives->line_color();       }
    void                                                                        SetLineColor                      (const GRP2DCOLOR* c)
                                                                                {
                                                                                  renderer_primitives->line_color((*(COLORTYPE*)c));
                                                                                  renderer_scanline->color((*(COLORTYPE*)c));
                                                                                }

    void                                                                        Line                              (double x1, double y1, double x2, double y2)
                                                                                {
                                                                                  if(!dashline) return;
                                                                                  dashline->Draw(x1, y1, x2, y2, linewidth, dashlength);
                                                                                }

    void                                                                        Rectangle                         (double x1, double y1, double x2, double y2, bool isfill)
                                                                                {
                                                                                  if(isfill)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->fill_color());

                                                                                      GRP2DCANVASAGG_AA_SET_RASTERIZER_GAMMA(ras);

                                                                                      ras.move_to_d(x1, y1);
                                                                                      ras.line_to_d(x1, y2);
                                                                                      ras.line_to_d(x2, y2);
                                                                                      ras.line_to_d(x2, y1);
                                                                                      ras.line_to_d(x1, y1);

                                                                                      AGG_SOLIDFILL_END
                                                                                    }
                                                                                    
                                                                                  if(linewidth <= 1.0)
                                                                                    {

                                                                                      AGG_SOLIDFILL_INI
                                                                                      
                                                                                      renderer_base->blend_hline((int)x1, (int)y1, (int)x2, renderer_primitives->line_color(), 255);
                                                                                      renderer_base->blend_vline((int)x2, (int)y1, (int)y2, renderer_primitives->line_color(), 255);
                                                                                      renderer_base->blend_hline((int)x1, (int)y2, (int)x2, renderer_primitives->line_color(), 255);
                                                                                      renderer_base->blend_vline((int)x1, (int)y1, (int)y2, renderer_primitives->line_color(), 255); 

                                                                                      AGG_SOLIDFILL_END
                                                                                    }
                                                                                   else                                                                                  
                                                                                    {
                                                                                      AGG_OUTLINE_INI

                                                                                      ren.color(renderer_primitives->line_color());

                                                                                      ras.move_to_d(x1, y1);
                                                                                      ras.line_to_d(x1, y2);
                                                                                      ras.line_to_d(x2, y2);
                                                                                      ras.line_to_d(x2, y1);

                                                                                      AGG_OUTLINE_END
                                                                                    }
                                                                                }

    void                                                                        Ellipse                           (double x, double y, double rx, double ry, bool isfill)
                                                                                {
                                                                                   if(isfill)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->fill_color());

                                                                                      GRP2DCANVASAGG_AA_SET_RASTERIZER_GAMMA(ras);

                                                                                      agg::ellipse ell(x, y, rx, ry, 100);
                                                                                      ras.add_path(ell);

                                                                                      AGG_SOLIDFILL_END
                                                                                    }

                                                                                  if((linewidth <= 1.0f) && (linewidth))
                                                                                    {
                                                                                      renderer_primitives->ellipse((int)x, (int)y, (int)rx, (int)ry);
                                                                                    }
                                                                                   
                                                                                  if(linewidth > 1.0f)
                                                                                    {
                                                                                      AGG_OUTLINE_INI

                                                                                      ren.color(renderer_primitives->line_color());

                                                                                      agg::ellipse ell(x, y, rx, ry, 100);
                                                                                      ras.add_path(ell);

                                                                                      AGG_OUTLINE_END
                                                                                    }
                                                                                }

    void                                                                        Polygon                           (GRP2DVERTEXS& vertexs, bool isfill)
                                                                                {
                                                                                   if(vertexs.GetSize()<3) return;

                                                                                   if(isfill)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->fill_color());

                                                                                      GRP2DCANVASAGG_AA_SET_RASTERIZER_GAMMA(ras);

                                                                                      GRP2DVERTEX* vertex = vertexs.Get(0);
                                                                                      if(!vertex) return;

                                                                                      ras.move_to_d(vertex->x, vertex->y);

                                                                                      for(XDWORD c=1; c<vertexs.GetSize(); c++)
                                                                                        {
                                                                                          vertex = vertexs.Get(c);
                                                                                          if(!vertex) break;

                                                                                          ras.line_to_d(vertex->x, vertex->y);
                                                                                        }

                                                                                      AGG_SOLIDFILL_END
                                                                                    }

                                                                                   AGG_OUTLINE_INI

                                                                                   ren.color(renderer_primitives->line_color());

                                                                                   GRP2DVERTEX* vertex = vertexs.Get(0);
                                                                                   if(!vertex) return;

                                                                                   ras.move_to_d(vertex->x, vertex->y);

                                                                                   for(XDWORD c=1; c<vertexs.GetSize(); c++)
                                                                                     {
                                                                                       vertex = vertexs.Get(c);
                                                                                       if(!vertex) break;

                                                                                       ras.line_to_d(vertex->x, vertex->y);
                                                                                     }

                                                                                   AGG_OUTLINE_END
                                                                                }

    void                                                                        Path                              (GRP2DPATH& path, bool isfill)
                                                                                {
                                                                                  if(path.IsEmpty()) return;

                                                                                  agg::path_storage ps;

                                                                                  for(XDWORD c=0; c<path.GetSize(); c++)
                                                                                    {
                                                                                      GRP2DPATHSEGMENT* segment = path.Get(c);
                                                                                      if(!segment) continue;

                                                                                      switch(segment->type)
                                                                                        {
                                                                                          case GRP2DPATHSEGMENTTYPE_UNKNOWN  :
                                                                                                                  default      : break;

                                                                                          case GRP2DPATHSEGMENTTYPE_MOVETO   : ps.move_to(segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_LINETO   : ps.line_to(segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_CURVETO  : ps.curve4(segment->c1x, segment->c1y, segment->c2x, segment->c2y, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_QUADTO   : ps.curve3(segment->c1x, segment->c1y, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_ARCTO    : ps.arc_to(segment->rx, segment->ry, agg::deg2rad(segment->xrot), segment->largearc, segment->sweep, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_CLOSE    : ps.close_polygon();
                                                                                                                               break;
                                                                                        }
                                                                                    }

                                                                                  if(isfill)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->fill_color());

                                                                                      ras.filling_rule((path.GetFillRule() == GRP2DPATHFILLRULE_EVENODD)?agg::fill_even_odd:agg::fill_non_zero);

                                                                                      agg::conv_curve<agg::path_storage> curve(ps);
                                                                                      GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);

                                                                                      ras.add_path(curve);

                                                                                      AGG_SOLIDFILL_END
                                                                                    }

                                                                                  else if(linewidth > 0.0)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->line_color());

                                                                                      agg::conv_curve<agg::path_storage>                       curve(ps);
                                                                                      GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);
                                                                                      agg::conv_stroke<agg::conv_curve<agg::path_storage> >   stroke(curve);

                                                                                      stroke.width(linewidth);
                                                                                      stroke.line_cap(agg::butt_cap);
                                                                                      stroke.line_join(agg::round_join);
                                                                                  stroke.miter_limit(4.0);

                                                                                      ras.add_path(stroke);

                                                                                      AGG_SOLIDFILL_END
                                                                                    }
                                                                                }
                                                                                  
    void                                                                        PathGradientBuildStorage          (GRP2DPATH& path, agg::path_storage& ps)
                                                                                {
                                                                                  for(XDWORD c=0; c<path.GetSize(); c++)
                                                                                    {
                                                                                      GRP2DPATHSEGMENT* segment = path.Get(c);
                                                                                      if(!segment) continue;

                                                                                      switch(segment->type)
                                                                                        {
                                                                                          case GRP2DPATHSEGMENTTYPE_UNKNOWN  :
                                                                                                                  default      : break;

                                                                                          case GRP2DPATHSEGMENTTYPE_MOVETO   : ps.move_to(segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_LINETO   : ps.line_to(segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_CURVETO  : ps.curve4(segment->c1x, segment->c1y, segment->c2x, segment->c2y, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_QUADTO   : ps.curve3(segment->c1x, segment->c1y, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_ARCTO    : ps.arc_to(segment->rx, segment->ry, agg::deg2rad(segment->xrot), segment->largearc, segment->sweep, segment->x, segment->y);
                                                                                                                               break;

                                                                                          case GRP2DPATHSEGMENTTYPE_CLOSE    : ps.close_polygon();
                                                                                                                               break;
                                                                                        }
                                                                                    }
                                                                                }

    void                                                                        PathGradientLinear                (GRP2DPATH& path, bool evenodd, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
                                                                                {
                                                                                  if(!renderer_base)  return;
                                                                                  if(path.IsEmpty())  return;
                                                                                  if(nstops < 1)      return;

                                                                                  agg::path_storage ps;
                                                                                  PathGradientBuildStorage(path, ps);

                                                                                  agg::conv_curve<agg::path_storage> curve(ps);
                                                                                  GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);

                                                                                  agg::rasterizer_scanline_aa<> ras;
                                                                                  agg::scanline_u8              sl;

                                                                                  ras.filling_rule(evenodd?agg::fill_even_odd:agg::fill_non_zero);
                                                                                  ras.add_path(curve);

                                                                                  typedef agg::gradient_lut<agg::color_interpolator<COLORTYPE>, 256>  gradient_lut_type;

                                                                                  gradient_lut_type colorlut;
                                                                                  colorlut.remove_all();

                                                                                  if(nstops == 1)
                                                                                    {
                                                                                      colorlut.add_color(0.0, stops[0].color);
                                                                                      colorlut.add_color(1.0, stops[0].color);
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      for(int i=0; i<nstops; i++)  colorlut.add_color(stops[i].offset, stops[i].color);
                                                                                    }

                                                                                  colorlut.build_lut();

                                                                                  double dx     = (x2 - x1);
                                                                                  double dy     = (y2 - y1);
                                                                                  double length = std::sqrt((dx * dx) + (dy * dy));
                                                                                  if(length < 0.000001)  length = 0.000001;

                                                                                  double angle  = std::atan2(dy, dx);

                                                                                  agg::trans_affine gmtx;
                                                                                  gmtx.reset();
                                                                                  gmtx *= agg::trans_affine_rotation(angle);
                                                                                  gmtx *= agg::trans_affine_translation(x1, y1);
                                                                                  gmtx.invert();

                                                                                  typedef agg::span_interpolator_linear<agg::trans_affine>                                          interpolator_type;
                                                                                  typedef agg::span_gradient<COLORTYPE, interpolator_type, agg::gradient_x, gradient_lut_type>      span_gradient_type;
                                                                                  typedef agg::span_allocator<COLORTYPE>                                                            span_allocator_type;

                                                                                  interpolator_type   interpolator(gmtx);
                                                                                  agg::gradient_x     gradientfunc;
                                                                                  span_gradient_type  spangradient(interpolator, gradientfunc, colorlut, 0.0, length);
                                                                                  span_allocator_type spanallocator;

                                                                                  agg::render_scanlines_aa(ras, sl, *renderer_base, spanallocator, spangradient);
                                                                                }

    void                                                                        PathGradientRadial                (GRP2DPATH& path, bool evenodd, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
                                                                                {
                                                                                  if(!renderer_base)  return;
                                                                                  if(path.IsEmpty())  return;
                                                                                  if(nstops < 1)      return;
                                                                                  if(r < 0.000001)    r = 0.000001;

                                                                                  agg::path_storage ps;
                                                                                  PathGradientBuildStorage(path, ps);

                                                                                  agg::conv_curve<agg::path_storage> curve(ps);
                                                                                  GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);

                                                                                  agg::rasterizer_scanline_aa<> ras;
                                                                                  agg::scanline_u8              sl;

                                                                                  ras.filling_rule(evenodd?agg::fill_even_odd:agg::fill_non_zero);
                                                                                  ras.add_path(curve);

                                                                                  typedef agg::gradient_lut<agg::color_interpolator<COLORTYPE>, 256>  gradient_lut_type;

                                                                                  gradient_lut_type colorlut;
                                                                                  colorlut.remove_all();

                                                                                  if(nstops == 1)
                                                                                    {
                                                                                      colorlut.add_color(0.0, stops[0].color);
                                                                                      colorlut.add_color(1.0, stops[0].color);
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      for(int i=0; i<nstops; i++)  colorlut.add_color(stops[i].offset, stops[i].color);
                                                                                    }

                                                                                  colorlut.build_lut();

                                                                                  agg::trans_affine gmtx;
                                                                                  gmtx.reset();
                                                                                  gmtx *= agg::trans_affine_translation(cx, cy);
                                                                                  gmtx.invert();

                                                                                  typedef agg::span_interpolator_linear<agg::trans_affine>                                             interpolator_type;
                                                                                  typedef agg::span_gradient<COLORTYPE, interpolator_type, agg::gradient_radial, gradient_lut_type>    span_gradient_type;
                                                                                  typedef agg::span_allocator<COLORTYPE>                                                               span_allocator_type;

                                                                                  interpolator_type    interpolator(gmtx);
                                                                                  agg::gradient_radial gradientfunc;
                                                                                  span_gradient_type   spangradient(interpolator, gradientfunc, colorlut, 0.0, r);
                                                                                  span_allocator_type  spanallocator;

                                                                                  agg::render_scanlines_aa(ras, sl, *renderer_base, spanallocator, spangradient);
                                                                                }

    void                                                                        PathGradientLinearStroke          (GRP2DPATH& path, double linewidth, double x1, double y1, double x2, double y2, GRP2DGRADIENTSTOP* stops, int nstops)
                                                                                {
                                                                                  if(!renderer_base)  return;
                                                                                  if(path.IsEmpty())  return;
                                                                                  if(nstops < 1)      return;
                                                                                  if(linewidth <= 0.0)  linewidth = 1.0;

                                                                                  agg::path_storage ps;
                                                                                  PathGradientBuildStorage(path, ps);

                                                                                  agg::conv_curve<agg::path_storage>                  curve(ps);
                                                                                  GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);
                                                                                  agg::conv_stroke<agg::conv_curve<agg::path_storage> > stroke(curve);
                                                                                  stroke.width(linewidth);
                                                                                  stroke.line_cap(agg::butt_cap);
                                                                                  stroke.line_join(agg::miter_join);
                                                                                  stroke.miter_limit(4.0);

                                                                                  agg::rasterizer_scanline_aa<> ras;
                                                                                  agg::scanline_u8              sl;

                                                                                  ras.filling_rule(agg::fill_non_zero);
                                                                                  ras.add_path(stroke);

                                                                                  typedef agg::gradient_lut<agg::color_interpolator<COLORTYPE>, 256>  gradient_lut_type;

                                                                                  gradient_lut_type colorlut;
                                                                                  colorlut.remove_all();

                                                                                  if(nstops == 1)
                                                                                    {
                                                                                      colorlut.add_color(0.0, stops[0].color);
                                                                                      colorlut.add_color(1.0, stops[0].color);
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      for(int i=0; i<nstops; i++)  colorlut.add_color(stops[i].offset, stops[i].color);
                                                                                    }

                                                                                  colorlut.build_lut();

                                                                                  double dx     = (x2 - x1);
                                                                                  double dy     = (y2 - y1);
                                                                                  double length = std::sqrt((dx * dx) + (dy * dy));
                                                                                  if(length < 0.000001)  length = 0.000001;

                                                                                  double angle  = std::atan2(dy, dx);

                                                                                  agg::trans_affine gmtx;
                                                                                  gmtx.reset();
                                                                                  gmtx *= agg::trans_affine_rotation(angle);
                                                                                  gmtx *= agg::trans_affine_translation(x1, y1);
                                                                                  gmtx.invert();

                                                                                  typedef agg::span_interpolator_linear<agg::trans_affine>                                          interpolator_type;
                                                                                  typedef agg::span_gradient<COLORTYPE, interpolator_type, agg::gradient_x, gradient_lut_type>      span_gradient_type;
                                                                                  typedef agg::span_allocator<COLORTYPE>                                                            span_allocator_type;

                                                                                  interpolator_type   interpolator(gmtx);
                                                                                  agg::gradient_x     gradientfunc;
                                                                                  span_gradient_type  spangradient(interpolator, gradientfunc, colorlut, 0.0, length);
                                                                                  span_allocator_type spanallocator;

                                                                                  agg::render_scanlines_aa(ras, sl, *renderer_base, spanallocator, spangradient);
                                                                                }

    void                                                                        PathGradientRadialStroke          (GRP2DPATH& path, double linewidth, double cx, double cy, double r, GRP2DGRADIENTSTOP* stops, int nstops)
                                                                                {
                                                                                  if(!renderer_base)  return;
                                                                                  if(path.IsEmpty())  return;
                                                                                  if(nstops < 1)      return;
                                                                                  if(linewidth <= 0.0)  linewidth = 1.0;
                                                                                  if(r < 0.000001)    r = 0.000001;

                                                                                  agg::path_storage ps;
                                                                                  PathGradientBuildStorage(path, ps);

                                                                                  agg::conv_curve<agg::path_storage>                  curve(ps);
                                                                                  GRP2DCANVASAGG_AA_SET_CURVE_QUALITY(curve);
                                                                                  agg::conv_stroke<agg::conv_curve<agg::path_storage> > stroke(curve);
                                                                                  stroke.width(linewidth);
                                                                                  stroke.line_cap(agg::butt_cap);
                                                                                  stroke.line_join(agg::miter_join);
                                                                                  stroke.miter_limit(4.0);

                                                                                  agg::rasterizer_scanline_aa<> ras;
                                                                                  agg::scanline_u8              sl;

                                                                                  ras.filling_rule(agg::fill_non_zero);
                                                                                  ras.add_path(stroke);

                                                                                  typedef agg::gradient_lut<agg::color_interpolator<COLORTYPE>, 256>  gradient_lut_type;

                                                                                  gradient_lut_type colorlut;
                                                                                  colorlut.remove_all();

                                                                                  if(nstops == 1)
                                                                                    {
                                                                                      colorlut.add_color(0.0, stops[0].color);
                                                                                      colorlut.add_color(1.0, stops[0].color);
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      for(int i=0; i<nstops; i++)  colorlut.add_color(stops[i].offset, stops[i].color);
                                                                                    }

                                                                                  colorlut.build_lut();

                                                                                  agg::trans_affine gmtx;
                                                                                  gmtx.reset();
                                                                                  gmtx *= agg::trans_affine_translation(cx, cy);
                                                                                  gmtx.invert();

                                                                                  typedef agg::span_interpolator_linear<agg::trans_affine>                                             interpolator_type;
                                                                                  typedef agg::span_gradient<COLORTYPE, interpolator_type, agg::gradient_radial, gradient_lut_type>    span_gradient_type;
                                                                                  typedef agg::span_allocator<COLORTYPE>                                                               span_allocator_type;

                                                                                  interpolator_type    interpolator(gmtx);
                                                                                  agg::gradient_radial gradientfunc;
                                                                                  span_gradient_type   spangradient(interpolator, gradientfunc, colorlut, 0.0, r);
                                                                                  span_allocator_type  spanallocator;

                                                                                  agg::render_scanlines_aa(ras, sl, *renderer_base, spanallocator, spangradient);
                                                                                }

    void                                                                        RoundRect                         (double x1, double y1, double x2, double y2, double radius, bool isfill = false)
                                                                                {
                                                                                  if(isfill)
                                                                                    {
                                                                                      AGG_SOLIDFILL_INI

                                                                                      ren.color(renderer_primitives->fill_color());

                                                                                      GRP2DCANVASAGG_AA_SET_RASTERIZER_GAMMA(ras);

                                                                                      agg::rounded_rect roundrect(x1, y1, x2, y2, radius);
                                                                                      ras.add_path(roundrect);

                                                                                      AGG_SOLIDFILL_END
                                                                                    }

                                                                                  if(linewidth >= 1.0)
                                                                                    {
                                                                                      AGG_OUTLINE_INI

                                                                                      ren.color(renderer_primitives->line_color());

                                                                                      agg::rounded_rect roundrect (x1, y1, x2, y2, radius);

                                                                                      ras.add_path(roundrect);

                                                                                      AGG_OUTLINE_END
                                                                                    }
                                                                                }

    GRPBITMAP*                                                                  GetBitmap                         (double x, double y, double width, double height)
                                                                                {

                                                                                  int clipwidth    = (renderer_base->xmax() - renderer_base->xmin());
                                                                                  int clipheight   = (renderer_base->ymax() - renderer_base->ymin());
                                                                                  int bitmapwidth  = (int)width;
                                                                                  int bitmapheight = (int)height;
                                                                                  int xpos         = (int)(x * GetBytesperPixel());
                                                                                  int c            = 0;

                                                                                  if(x < 0)
                                                                                    {
                                                                                      xpos         = 0;
                                                                                      bitmapwidth  = (int)(bitmapwidth  + x);
                                                                                    }

                                                                                  if(y < 0)
                                                                                    {
                                                                                      c = abs((int)y);
                                                                                    }

                                                                                  if((x + width)  > (renderer_base->xmin() + clipwidth))
                                                                                    {
                                                                                      bitmapwidth  = (int)(((clipwidth + renderer_base->xmin()) - x) + 1);
                                                                                    }

                                                                                  if((y + height) > (renderer_base->ymin() + clipheight))
                                                                                    {
                                                                                      bitmapheight = ((clipheight + renderer_base->ymin()) - (int)y) + 1;
                                                                                    }

                                                                                  if(bitmapwidth  <= 0) return NULL;
                                                                                  if(bitmapheight <= 0) return NULL;

                                                                                  GRPBITMAP* bitmap = GRPFACTORY::GetInstance().CreateBitmap(bitmapwidth, bitmapheight, mode, isbufferinverse);
                                                                                  if(!bitmap) return NULL;

                                                                                  GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmapfb = (GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>*)bitmap;
                                                                                  agg::rendering_buffer* bitmapbuffer = bitmapfb->GetRenderBuffer();

                                                                                  int sizeline = bitmapwidth *  GetBytesperPixel();

                                                                                  while(c < bitmapheight)
                                                                                    {
                                                                                      memcpy(bitmapbuffer->row_ptr(c), (rbuffer.row_ptr(c + (int)y) + xpos), sizeline);
                                                                                      c++;
                                                                                    }

                                                                                  return bitmap;
                                                                                }

    void                                                                        PutBitmapNoAlpha                  (double x, double y, GRPBITMAP* bitmap)
                                                                                {
                                                                                  if(!bitmap) return;

                                                                                  if(bitmap->GetMode() != GetMode())
                                                                                    {
                                                                                      COLORTYPE* color;

                                                                                      for(XDWORD yb = 0; yb<bitmap->GetHeight(); yb++)
                                                                                        {
                                                                                          for(XDWORD xb = 0; xb<bitmap->GetWidth(); xb++)
                                                                                            {
                                                                                              color = (COLORTYPE*)bitmap->GetPixel(xb, yb);
                                                                                              PutPixel(x+xb, y+yb, color);
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                   else
                                                                                    {
                                                                                      int xslide       = 0;

                                                                                      int clipwidth    = (renderer_base->xmax() - renderer_base->xmin());
                                                                                      int clipheight   = (renderer_base->ymax() - renderer_base->ymin());
                                                                                      int bitmapwidth  = bitmap->GetWidth();
                                                                                      int bitmapheight = bitmap->GetHeight();
                                                                                      int xpos         = (int)(abs((int)x) * GetBytesperPixel());
                                                                                      int c            = 0;

                                                                                      if(x < 0)
                                                                                        {
                                                                                          xpos    = (int)0;
                                                                                          xslide  = (int)(abs((int)x) * GetBytesperPixel());
                                                                                        }

                                                                                      if(y < 0)
                                                                                        {
                                                                                          c = (int)abs((int)y);
                                                                                        }

                                                                                      if((x + (int)bitmap->GetWidth())  > (renderer_base->xmin() + clipwidth))
                                                                                        {
                                                                                          bitmapwidth  = (int)((clipwidth +renderer_base->xmin()) - x) + 1;
                                                                                        }

                                                                                      if((y + (int)bitmap->GetHeight()) > (renderer_base->ymin() + clipheight))
                                                                                        {
                                                                                          bitmapheight = (int)((clipheight + renderer_base->ymin()) - y) + 1;
                                                                                        }

                                                                                      if(bitmapwidth  <= 0) return;
                                                                                      if(bitmapheight <= 0) return;

                                                                                      GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmapfb = (GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>*)bitmap;
                                                                                      agg::rendering_buffer* bitmapbuffer = bitmapfb->GetRenderBuffer();

                                                                                      int nlines    = bitmapheight;
                                                                                      int sizeline  = bitmapwidth * GetBytesperPixel();

                                                                                      while(c < nlines)
                                                                                        {
                                                                                          memcpy(rbuffer.row_ptr((int)(c + y)) + xpos, bitmapbuffer->row_ptr(c) + xslide, sizeline);
                                                                                          c++;
                                                                                        }
                                                                                    }

                                                                                  #ifdef DEBUG_ACTIVE 

                                                                                  #endif  
                                                                                }

    void                                                                        PutBitmap                         (double x, double y, GRPBITMAP* bitmap)
                                                                                {
                                                                                  if(!bitmap) return;

                                                                                  COLORTYPE* color;

                                                                                  for(XDWORD yb = 0; yb<bitmap->GetHeight(); yb++)
                                                                                    {
                                                                                      for(XDWORD xb = 0; xb<bitmap->GetWidth(); xb++)
                                                                                        {
                                                                                          color = (COLORTYPE*)bitmap->GetPixel(xb, yb);
                                                                                          PutBlendPixel(x+xb, y+yb, color, color->a);
                                                                                        }
                                                                                    }

                                                                                  #ifdef DEBUG_ACTIVE 

                                                                                  #endif  
                                                                                }

    void                                                                        PutBitmapAlpha                    (double x, double y, GRPBITMAP* bitmap, double alpha = 100)
                                                                                {
                                                                                  if(!bitmap) return;

                                                                                  COLORTYPE* color;

                                                                                  for(XDWORD yb = 0; yb<bitmap->GetHeight(); yb++)
                                                                                    {
                                                                                      for(XDWORD xb = 0; xb<bitmap->GetWidth(); xb++)
                                                                                        {
                                                                                          color = (COLORTYPE*)bitmap->GetPixel(xb, yb);
                                                                                          PutBlendPixel(x+xb, y+yb, color, ((alpha * color->a)/100));
                                                                                        }
                                                                                    }

                                                                                  #ifdef DEBUG_ACTIVE 

                                                                                  #endif  
                                                                                }

    bool                                                                        RasterFont_Select                 (XCHAR* fontname)
                                                                                {
                                                                                  typedef struct
                                                                                  {
                                                                                    const agg::int8u* font;
                                                                                    const XCHAR*      name;

                                                                                  } FONT_TYPE;

                                                                                  FONT_TYPE fonts[] =  {  { agg::gse4x6                 ,  _L("gse4x6")               },
                                                                                                          { agg::gse4x8                 ,  _L("gse4x8")               },
                                                                                                          { agg::gse5x7                 ,  _L("gse5x7")               },
                                                                                                          { agg::gse5x9                 ,  _L("gse5x9")               },
                                                                                                          { agg::gse6x9                 ,  _L("gse6x9")               },
                                                                                                          { agg::gse6x12                ,  _L("gse6x12")              },
                                                                                                          { agg::gse7x11                ,  _L("gse7x11")              },
                                                                                                          { agg::gse7x11_bold           ,  _L("gse7x11_bold")         },
                                                                                                          { agg::gse7x15                ,  _L("gse7x15")              },
                                                                                                          { agg::gse7x15_bold           ,  _L("gse7x15_bold")         },
                                                                                                          { agg::gse8x16                ,  _L("gse8x16")              },
                                                                                                          { agg::gse8x16_bold           ,  _L("gse8x16_bold")         },
                                                                                                          { agg::mcs11_prop             ,  _L("mcs11_prop")           },
                                                                                                          { agg::mcs11_prop_condensed   ,  _L("mcs11_prop_condensed") },
                                                                                                          { agg::mcs12_prop             ,  _L("mcs12_prop")           },
                                                                                                          { agg::mcs13_prop             ,  _L("mcs13_prop")           },
                                                                                                          { agg::mcs5x10_mono           ,  _L("mcs5x10_mono")         },
                                                                                                          { agg::mcs5x11_mono           ,  _L("mcs5x11_mono")         },
                                                                                                          { agg::mcs6x10_mono           ,  _L("mcs6x10_mono")         },
                                                                                                          { agg::mcs6x11_mono           ,  _L("mcs6x11_mono")         },
                                                                                                          { agg::mcs7x12_mono_high      ,  _L("mcs7x12_mono_high")    },
                                                                                                          { agg::mcs7x12_mono_low       ,  _L("mcs7x12_mono_low")     },
                                                                                                          { agg::verdana12              ,  _L("verdana12")            },
                                                                                                          { agg::verdana12_bold         ,  _L("verdana12_bold")       },
                                                                                                          { agg::verdana13              ,  _L("verdana13")            },
                                                                                                          { agg::verdana13_bold         ,  _L("verdana13_bold")       },
                                                                                                          { agg::verdana14              ,  _L("verdana14")            },
                                                                                                          { agg::verdana14_bold         ,  _L("verdana14_bold")       },
                                                                                                          { agg::verdana16              ,  _L("verdana16")            },
                                                                                                          { agg::verdana16_bold         ,  _L("verdana16_bold")       },
                                                                                                          { agg::verdana17              ,  _L("verdana17")            },
                                                                                                          { agg::verdana17_bold         ,  _L("verdana17_bold")       },
                                                                                                          { agg::verdana18              ,  _L("verdana18")            },
                                                                                                          { agg::verdana18_bold         ,  _L("verdana18_bold")       }
                                                                                                       };

                                                                                  XSTRING strfontname;

                                                                                  if(!rasterglyph) return false;

                                                                                  strfontname = fontname;

                                                                                  for(int c=0; c<sizeof(fonts)/sizeof(FONT_TYPE); c++)
                                                                                    {
                                                                                      XSTRING _strfontname;

                                                                                      _strfontname = fonts[c].name;

                                                                                      if(!strfontname.Compare(_strfontname))
                                                                                        {
                                                                                          rasterglyph->font(fonts[c].font);
                                                                                          break;
                                                                                        }
                                                                                    }

                                                                                  return true;
                                                                                }

    GRP2DCOLOR*                                                                 RasterFont_GetColor               ()                                        
                                                                                { 
                                                                                  return (GRP2DCOLOR*)(&renderer_rastertext->color());        
                                                                                }

    void                                                                        RasterFont_SetColor               (const GRP2DCOLOR* c)                     
                                                                                { 
                                                                                  renderer_rastertext->color((*(COLORTYPE*)c));               
                                                                                }

    double                                                                      RasterFont_GetWidth               (XCHAR* string)
                                                                                {
                                                                                   if(!rasterglyph) return 0.0f;

                                                                                   return rasterglyph->width(string);
                                                                                }

    double                                                                      RasterFont_GetHeight              ()
                                                                                {
                                                                                   if(!rasterglyph) return 0;

                                                                                   return rasterglyph->height();
                                                                                }

    bool                                                                        RasterFont_Printf                 (double x, double y, XCHAR* mask, ...)
                                                                                {
                                                                                  XSTRING outstring;
                                                                                  va_list arg;

                                                                                  if(!renderer_rastertext) return false;

                                                                                  va_start(arg, mask);

                                                                                  outstring.FormatArg(mask, &arg, false);

                                                                                  va_end(arg);

                                                                                  renderer_rastertext->render_text(x, y, outstring.Get(), true);

                                                                                  return true;
                                                                                }

    bool                                                                        VectorFont_Load                   (XPATH& fontnamefile)
                                                                                {                                      
                                                                                  agg::glyph_rendering gren     = agg::glyph_ren_native_mono;
                                                                                  bool                 status   = false;
                                                                                  int                  mode     = 1;   

                                                                                  switch(mode)                                                                                          
                                                                                    {
                                                                                      default :        
                                                                                      case  0 : gren = agg::glyph_ren_native_mono;  break;
                                                                                      case  1 : gren = agg::glyph_ren_native_gray8; break;
                                                                                      case  2 : gren = agg::glyph_ren_outline;      break;
                                                                                      case  3 : gren = agg::glyph_ren_agg_mono;     break;
                                                                                      case  4 : gren = agg::glyph_ren_agg_gray8;    break;
                                                                                    }

                                                                                  XBUFFER charstr;
                                                                                  
                                                                                  fontnamefile.ConvertToASCII(charstr);       
                                                                                  
                                                                                  vectorfont_namefile = charstr;     

                                                                                  status = vectorfont_engine.load_font(charstr.GetPtrChar(), 0, gren);                                                                          
                                                                                  status = vectorfont_engine.load_font(charstr.GetPtrChar(), 0, gren);

                                                                                  if(status) vectorfont_pathfile = fontnamefile;
                                                                                  
                                                                                  return status;    
                                                                                }        
                                              
    double                                                                      VectorFont_GetWidth               (XCHAR* string)
                                                                                {
                                                                                  double x        = 0;
                                                                                  double y        = 0;
                                                                                  double _width   = 0;   

                                                                                  XSTRING _string;

                                                                                  _string = string;

                                                                                  AGG_SOLIDFILL_INI  

                                                                                  vectorfont_engine.hinting(true);
                                                                                  vectorfont_engine.height(vectorfont_config.GetHeight());
                                                                                  vectorfont_engine.width(vectorfont_config.GetWidth());
                                                                                  vectorfont_engine.flip_y(true);
                                                                                                                                                                 
                                                                                  XBUFFER charstr;
                                                                                  
                                                                                  _string.ConvertToASCII(charstr);
                                                                                                                                                                    
                                                                                  const  char* p = charstr.GetPtrChar();

                                                                                  for(XDWORD c=0; c<_string.GetSize(); c++)

                                                                                    {
                                                                                      const agg::glyph_cache* glyph = vectorfont_manager->glyph(*p);

                                                                                      if(glyph)
                                                                                        {
                                                                                          if(vectorfont_config.IsKerning()) 
                                                                                            {
                                                                                              vectorfont_manager->add_kerning(&x, &y);
                                                                                            }
                                                                                                                                                                              
                                                                                          vectorfont_manager->init_embedded_adaptors(glyph, x, y);   
                                                                                          _width += glyph->advance_x;   
                                                                                        }
                                                                                                                                                                                                                                                            
                                                                                      ++p;
                                                                                    }

                                                                                  AGG_SOLIDFILL_END
                                                                                  
                                                                                  return _width;                      
                                                                                }

 double                                                                         VectorFont_GetHeight              (XCHAR* string)
                                                                                {
                                                                                  double x        = 0;
                                                                                  double y        = 0;
                                                                                  double _height  = 0;   

                                                                                  XSTRING _string;

                                                                                  _string = string;

                                                                                  AGG_SOLIDFILL_INI  

                                                                                  vectorfont_engine.hinting(true);
                                                                                  vectorfont_engine.height(vectorfont_config.GetHeight());
                                                                                  vectorfont_engine.width(vectorfont_config.GetWidth());
                                                                                  vectorfont_engine.flip_y(true);
                                                                                
                                                                                  XBUFFER charstr;
                                                                                  
                                                                                  _string.ConvertToASCII(charstr);
                                                                                                                                                                  
                                                                                  const char* p = charstr.GetPtrChar();

                                                                                  for(XDWORD c=0; c<_string.GetSize(); c++)

                                                                                    {
                                                                                      const agg::glyph_cache* glyph = vectorfont_manager->glyph(*p);

                                                                                      if(glyph)
                                                                                        {
                                                                                          if(vectorfont_config.IsKerning()) 
                                                                                            {
                                                                                              vectorfont_manager->add_kerning(&x, &y);
                                                                                            }
                                                                                                                                                                              
                                                                                          vectorfont_manager->init_embedded_adaptors(glyph, x, y);   

                                                                                          if((*p) != ' ')
                                                                                            {
                                                                                              int glyph_height = abs(glyph->bounds.y2-glyph->bounds.y1);  

                                                                                              if( glyph_height > _height) _height = glyph_height;   
                                                                                            }
                                                                                        }
                                                                                                                                                                                                                                                            
                                                                                      ++p;
                                                                                    }

                                                                                  AGG_SOLIDFILL_END

                                                                                  return _height;                      
                                                                                }

    double                                                                      VectorFont_GetHeight              ()
                                                                                {
                                                                                  return vectorfont_config.GetHeight();                                                                                   
                                                                                }

    bool                                                                        VectorFont_Print                  (double _x, double _y, XCHAR* _outstring)
                                                                                {
                                                                                  XSTRING outstring;
                                                                                  
                                                                                  outstring = _outstring;
                                                                                  
                                                                                  XDWORD  num_glyphs = 0;
                                                                                  
                                                                                  AGG_SOLIDFILL_INI  

                                                                                  vectorfont_engine.hinting(true);
                                                                                  vectorfont_engine.height(vectorfont_config.GetHeight());
                                                                                  vectorfont_engine.width(vectorfont_config.GetWidth());
                                                                                  vectorfont_engine.flip_y(true); 
                                                                                  
                                                                                  double x      = _x;
                                                                                  double y      = _y+1;
                                                                                  
                                                                                  for(XDWORD c=0; c<outstring.GetSize(); c++)
                                                                                    {
                                                                                      XCHAR character = outstring.Get()[c];

                                                                                      const agg::glyph_cache* glyph = vectorfont_manager->glyph(character);

                                                                                      if(glyph)
                                                                                        {
                                                                                          if(vectorfont_config.IsKerning()) 
                                                                                            {
                                                                                              vectorfont_manager->add_kerning(&x, &y);
                                                                                            }
                                                                                                                                                                              
                                                                                          vectorfont_manager->init_embedded_adaptors(glyph, x, y);

                                                                                          switch(glyph->data_type)
                                                                                            {
                                                                                                                  default     : break;

                                                                                              case agg::glyph_data_mono       : ren_bin.color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(vectorfont_manager->mono_adaptor(), vectorfont_manager->mono_scanline(), ren_bin);
                                                                                                                                break;

                                                                                              case agg::glyph_data_gray8      : ren.color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(vectorfont_manager->gray8_adaptor(), vectorfont_manager->gray8_scanline(), ren);
                                                                                                                                break;

                                                                                              case agg::glyph_data_outline    : ras.reset();

                                                                                                                                renderer_scanline->color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(ras, sl, ren);                                                                                  
                                                                                                                                break;
                                                                                              }

                                                                                            x += glyph->advance_x;
                                                                                            y += glyph->advance_y;  

                                                                                            ++num_glyphs;
                                                                                          }                                                                                        
                                                                                    }

                                                                                  AGG_SOLIDFILL_END                                                                                  
                                                                              
                                                                                  return true;
                                                                                }

    bool                                                                        VectorFont_PrintAngle             (double _x, double _y, double _angle, XCHAR* _outstring)
                                                                                {
                                                                                  if(_angle == 0.0)                       return VectorFont_Print(_x, _y, _outstring);
                                                                                  if(vectorfont_namefile.GetSize() == 0)  return VectorFont_Print(_x, _y, _outstring);

                                                                                  vectorfont_engine.load_font(vectorfont_namefile.GetPtrChar(), 0, agg::glyph_ren_agg_gray8);

                                                                                  XSTRING outstring;

                                                                                  outstring = _outstring;

                                                                                  XDWORD  num_glyphs = 0;

                                                                                  AGG_SOLIDFILL_INI

                                                                                  vectorfont_engine.hinting(true);
                                                                                  vectorfont_engine.height(vectorfont_config.GetHeight());
                                                                                  vectorfont_engine.width(vectorfont_config.GetWidth());
                                                                                  vectorfont_engine.flip_y(true);
                                                                                  vectorfont_engine.transform(agg::trans_affine_rotation(_angle));

                                                                                  double x      = _x;
                                                                                  double y      = _y+1;

                                                                                  for(XDWORD c=0; c<outstring.GetSize(); c++)
                                                                                    {
                                                                                      XCHAR character = outstring.Get()[c];

                                                                                      const agg::glyph_cache* glyph = vectorfont_manager->glyph(character);

                                                                                      if(glyph)
                                                                                        {
                                                                                          if(vectorfont_config.IsKerning())
                                                                                            {
                                                                                              vectorfont_manager->add_kerning(&x, &y);
                                                                                            }

                                                                                          vectorfont_manager->init_embedded_adaptors(glyph, x, y);

                                                                                          switch(glyph->data_type)
                                                                                            {
                                                                                                                  default     : break;

                                                                                              case agg::glyph_data_mono       : ren_bin.color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(vectorfont_manager->mono_adaptor(), vectorfont_manager->mono_scanline(), ren_bin);
                                                                                                                                break;

                                                                                              case agg::glyph_data_gray8      : ren.color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(vectorfont_manager->gray8_adaptor(), vectorfont_manager->gray8_scanline(), ren);
                                                                                                                                break;

                                                                                              case agg::glyph_data_outline    : ras.reset();
                                                                                                                                renderer_scanline->color((*vectorfont_config.GetColor()));
                                                                                                                                agg::render_scanlines(ras, sl, ren);
                                                                                                                                break;
                                                                                              }

                                                                                            x += glyph->advance_x;
                                                                                            y += glyph->advance_y;

                                                                                            ++num_glyphs;
                                                                                          }
                                                                                    }

                                                                                  vectorfont_engine.transform(agg::trans_affine());

                                                                                  AGG_SOLIDFILL_END

                                                                                  vectorfont_engine.load_font(vectorfont_namefile.GetPtrChar(), 0, agg::glyph_ren_native_gray8);

                                                                                  return true;
                                                                                }

    bool                                                                        VectorFont_Printf                 (double _x, double _y, XCHAR* mask, ...)
                                                                                {
                                                                                  XSTRING outstring;
                                                                                  va_list arg;
                                                                                  
                                                                                  va_start(arg, mask);

                                                                                  outstring.FormatArg(mask, &arg, false);

                                                                                  va_end(arg);

                                                                                  return VectorFont_Print(_x, _y, outstring.Get());                                                                                                                                                                 
                                                                                }

    bool                                                                        CopyBufferRenderFromViewport      (GRPVIEWPORT* viewport)
                                                                                {
                                                                                  GRP2DCANVAS*              viewportcanvas        = NULL;
                                                                                  agg::rendering_buffer   viewportcanvasbuffer;

                                                                                  if(!viewport) 
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  viewportcanvas = viewport->GetCanvas();
                                                                                  if(!viewportcanvas) 
                                                                                    {
                                                                                      return false;
                                                                                    }
                                                                         
                                                                                  viewportcanvasbuffer.attach(viewportcanvas->Buffer_Get(), viewportcanvas->GetWidth(), viewportcanvas->GetHeight(), (viewportcanvas->IsBufferInverse()?1:-1) * ((int)viewportcanvas->GetWidth() * viewportcanvas->GetBytesperPixel()));

                                                                                  int sizeline    = (int)(viewport->GetWidth() * GetBytesperPixel());
                                                                                  int nlines      = (int)(viewport->GetHeight()); 

                                                                                  int originposx  = (int)(viewport->GetPositionX() * GetBytesperPixel());
                                                                                  int originposy  = (int)(viewport->GetPositionY());
 
                                                                                  int targetposx  = (int)(viewport->GetCanvasPositionX() * GetBytesperPixel());
                                                                                  int targetposy  = (int)(viewport->GetCanvasPositionY());

                                                                                  for(int c=0; c<nlines; c++)
                                                                                    {
                                                                                      memcpy( rbuffer.row_ptr(c + originposy) + originposx, viewportcanvasbuffer.row_ptr(c + targetposy) +  targetposx, sizeline);
                                                                                    }

                                                                                  return true;
                                                                                }

    bool                                                                        ReleaseDrawFramerate              ()  
                                                                                {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
                                                                                  if(framerate_bitmap)
                                                                                    {              
                                                                                      int height = (int)RasterFont_GetHeight();                                                                        
                                                                                      PutBitmap(framerate_x - 4 , framerate_y - height - 4, framerate_bitmap);
                                                                                      
                                                                                      GEN_DELETE framerate_bitmap;
                                                                                      framerate_bitmap = NULL;
                                                                                    }
                                                                                  
                                                                                  return true;
                                                                                }

    bool                                                                        DrawFramerate                     (GRPSCREEN* screen, double x, double y)
                                                                                {
                                                                                  if(!screen)  
                                                                                    {
                                                                                      return false;
                                                                                    }

                                                                                  XSTRING           text;
                                                                                  GRP2DCOLOR_RGBA8  colorback(255, 255, 255);
                                                                                  GRP2DCOLOR_RGBA8  color(0, 0, 0, 150);

                                                                                  text.Format(_L("%3.2f fps"), screen->GetFrameRate()->Get()); 

                                                                                  int width  = (int)RasterFont_GetWidth(text.Get()) + 8;
                                                                                  int height = (int)RasterFont_GetHeight();

                                                                                  if(x == GRP2DCANVASTYPE_DRAWFRAMERATE_ADJUSTSCREEN)
                                                                                    {
                                                                                      framerate_x = screen->GetWidth() - width - 10;          
                                                                                    }
                                                                                   else 
                                                                                    {
                                                                                      if(x < 10)
                                                                                        {         
                                                                                          framerate_x = 10;
                                                                                        }
                                                                                       else 
                                                                                        {
                                                                                          framerate_x = x;   
                                                                                        }
                                                                                    }

                                                                                  if(y == GRP2DCANVASTYPE_DRAWFRAMERATE_ADJUSTSCREEN)
                                                                                    {
                                                                                      framerate_y = screen->GetHeight() - 4;          
                                                                                    }
                                                                                   else
                                                                                    { 
                                                                                      if(y < height)
                                                                                        {
                                                                                          framerate_y = height;
                                                                                        }
                                                                                       else
                                                                                        {           
                                                                                          framerate_y = y;
                                                                                        }
                                                                                    }
                                                                                           
                                                                                  framerate_bitmap = GetBitmap(framerate_x - 4 , framerate_y - height - 4, (width + 8), (height + 8));
                                                                                                                                                                       
                                                                                  SetFillColor(&colorback);
                                                                                  SetLineColor(&color);   
                                                                                  SetLineWidth(1.0f);
                                                                                  Rectangle(framerate_x, framerate_y + 2, framerate_x + width, framerate_y - height - 2, true);
                                                                                                                                                                                                                                                   
                                                                                  RasterFont_SetColor(&color);
                                                                                  RasterFont_Printf(framerate_x + 4, framerate_y - 4, text.Get());

                                                                                  return true;
                                                                                }

  private:   

    #ifdef DEBUG_ACTIVE 
    bool                                                                        Debug_Draw                        (double originx, double originy, double width, double height)
                                                                                {
                                                                                  GRP2DCOLOR_RGBA8  colorred(255, 0, 0);
                                                                                  GRP2DCOLOR_RGBA8  colorblue(0, 0, 255);
                                                                                  GRP2DCOLOR_RGBA8  colortrasparent(0, 0, 0, 255);
                                                                                  
                                                                                  SetLineWidth(1.0f);
                                                                                  SetLineColor(&colorblue);
                                                                                  Circle(originx, originy, 5);               

                                                                                  SetLineColor(&colorred);
                                                                                  SetFillColor(&colortrasparent);
                                                                                  Rectangle(originx, originy, originx + width, originy + height, false);

                                                                                  return true;
                                                                                }
    #endif

    void                                                                        Clean                             ()
                                                                                {
                                                                                  pixelformatbuffer   = NULL;
                                                                                  renderer_base       = NULL;
                                                                                  renderer_primitives = NULL;
                                                                                  rasterglyph         = NULL;
                                                                                  renderer_rastertext = NULL;

                                                                                  framerate_x         = 0;
                                                                                  framerate_y         = 0;
                                                                                  framerate_bitmap    = 0;
                                                                                }

    agg::rendering_buffer                                                       rbuffer;
    PIXELFORMATBUFFER*                                                          pixelformatbuffer;
    agg::renderer_base<PIXELFORMATBUFFER>*                                      renderer_base;
    agg::renderer_primitives<agg::renderer_base<PIXELFORMATBUFFER> >*           renderer_primitives;
    COLORTYPE                                                                   color;

    GRP2DCANVASAGG_DASHED_LINE<agg::rasterizer_scanline_aa<>,
                             agg::renderer_scanline_aa_solid<agg::renderer_base<PIXELFORMATBUFFER> >,
                             agg::scanline_u8  >*                               dashline;

    agg::rasterizer_scanline_aa<>                                               rasterizer_type;
    agg::renderer_scanline_aa_solid<agg::renderer_base<PIXELFORMATBUFFER> >*    renderer_scanline;    
    agg::scanline_u8                                                            scanline_type;

    XMAP<XDWORD, GRPBITMAP*>                                                    spriteregresionmap;

    AGG_GLYPH_GEN*                                                              rasterglyph;
    agg::renderer_raster_htext_solid<agg::renderer_base<PIXELFORMATBUFFER>, 
                                     AGG_GLYPH_GEN>*
                                                                                renderer_rastertext;

    AGG_FONT_ENGINE                                                             vectorfont_engine;
    AGG_FONT_MANAGER*                                                           vectorfont_manager; 
    XBUFFER                                                                     vectorfont_namefile;

    double                                                                      framerate_x;
    double                                                                      framerate_y;
    GRPBITMAP*                                                                  framerate_bitmap;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






