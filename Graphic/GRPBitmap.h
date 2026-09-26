/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmap.h
* 
* @class      GRPBITMAP
* @brief      Graphics Bitmap class
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

#include "agg_basics.h"
#include "agg_renderer_primitives.h"

#include "GRPFactory.h"
#include "GRP2DColor.h"
#include "GRPRect.h"
#include "GRPProperties.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPBITMAP_INVALIDHANDLE   0xFFFFFFFFUL

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP : public GRPPROPERTIES
{
  public:
                                                          GRPBITMAP                       (int width, int height, GRPPROPERTYMODE mode, bool isbufferinverse = false);
    virtual                                              ~GRPBITMAP                       ();

    bool                                                  IsValid                         ();
    XBYTE*                                                GetBuffer                       ();
    XBYTE**                                               GetBufferPointer                ();
    int                                                   GetBufferSize                   ();

    virtual GRP2DCOLOR*                                   GetPixel                        (int x, int y)                                      = 0;
    virtual void                                          PutPixel                        (int x, int y, const GRP2DCOLOR* color)             = 0;
    virtual void                                          PutBlendPixel                   (int x, int y, const GRP2DCOLOR* color, int alpha)  = 0;

    virtual GRPBITMAP*                                    GetSubBitmap                    (GRPRECTINT& rect);

    virtual GRPBITMAP*                                    Clone                           ();

    virtual bool                                          CopyFrom                        (GRPBITMAP* bitmap);

    virtual GRPBITMAP*                                    ConvertToMode                   (GRPPROPERTYMODE newmode);

    virtual bool                                          FlipHorizontal                  ();
    virtual bool                                          FlipVertical                    ();
    virtual bool                                          Rotate                          (float degrees);
    virtual bool                                          Scale                           (int nwidth, int nheight);
    virtual bool                                          Crop                            (GRPRECTINT& rect);

    bool                                                  Compare                         (GRPBITMAP* bitmap);
    bool                                                  FindSubBitmap                   (GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent = 2, XBYTE pixelmargin = 10);
    
    XDWORD                                                GetHandle                       ();
    void                                                  SetHandle                       (XDWORD handle);

  protected:

    bool                                                  DifferencesPerCent              (XDWORD ndiff, XDWORD max, int limit);
    bool                                                  IsSimilarPixel                  (XDWORD origin, XDWORD target, XBYTE margin);

    XBYTE*                                                buffer;
    int                                                   buffersize;
    bool                                                  isvalid;
    XDWORD                                                handle;

  private:

    void                                                  Clean                           ();
};

template<class PIXELFORMATBUFFER, class COLORTYPE>
class GRPBITMAPPIXELFORMATBUFFER : public GRPBITMAP
{
  public:

                                                          GRPBITMAPPIXELFORMATBUFFER      (int width, int height, GRPPROPERTYMODE mode, bool isbufferinverse = false) : GRPBITMAP(width, height, mode, isbufferinverse)
                                                          {
                                                            Clean();
                                                            if(CreateBuffers())  isvalid = true;
                                                          }

    virtual                                              ~GRPBITMAPPIXELFORMATBUFFER      ()
                                                          {
                                                            DeleteBuffers();
                                                            Clean();
                                                          }

    agg::rendering_buffer*                                GetRenderBuffer                 ()
                                                          {
                                                            return &rbuffer;
                                                          }

    GRP2DCOLOR*                                           GetPixel                        (int x, int y)
                                                          {                                                            
                                                            color = renderer_base->pixel(x, y);

                                                            return (GRP2DCOLOR*)&color;                                                            
                                                          }

    void                                                  PutPixel                        (int x, int y, const GRP2DCOLOR* color)
                                                          {
                                                            renderer_base->copy_pixel(x, y, (*(COLORTYPE*)color));
                                                          }

    void                                                  PutBlendPixel                   (int x, int y, const GRP2DCOLOR* color, int alpha)
                                                          {
                                                             renderer_base->blend_pixel(x, y, (*(COLORTYPE*)color), alpha);
                                                          }

    GRPBITMAP*                                            GetSubBitmap                    (GRPRECTINT& rect)
                                                          {
                                                            int w = (rect.x2 - rect.x1);
                                                            int h = (rect.y2 - rect.y1);

                                                            if((w <= 0) || (h <=0)) return NULL;

                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmap = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(w, h, mode);
                                                            if(!bitmap) return NULL;

                                                            COLORTYPE* color;

                                                            for(XDWORD y=0; y<(XDWORD)h; y++)
                                                              {
                                                                for(XDWORD x=0; x<(XDWORD)w; x++)
                                                                  {
                                                                    color = (COLORTYPE*)GetPixel(x+rect.x1, y+rect.y1);
                                                                    bitmap->PutBlendPixel(x, y, color, color->a);                                                                    
                                                                  }
                                                              }

                                                            return bitmap;
                                                          }

    GRPBITMAP*                                            Clone                           ()
                                                          {
                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bmp = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(width, height, mode);
                                                            if(!bmp) return NULL;

                                                            if(!bmp->CopyBuffer(buffer))
                                                              {
                                                                GEN_DELETE bmp;
                                                                return NULL;
                                                              }

                                                            return bmp;
                                                          }

    bool                                                  CopyFrom                        (GRPBITMAP* bitmap)
                                                          {
                                                            CopyPropertysFrom(bitmap);

                                                            DeleteBuffers();

                                                            if(CreateBuffers())
                                                                   isvalid = true;
                                                              else return false;

                                                            return CopyBuffer(bitmap->GetBuffer());
                                                          }

    GRPBITMAP*                                            ConvertToMode                   (GRPPROPERTYMODE newmode)
                                                          {
                                                            GRPBITMAP* bitmap = GRPFACTORY::GetInstance().CreateBitmap(width, height, newmode, isbufferinverse);
                                                            if(!bitmap) return NULL; 

                                                            COLORTYPE* color;

                                                            for(XDWORD y=0; y<height; y++)
                                                              {
                                                                for(XDWORD x=0; x<width; x++)
                                                                  {
                                                                    color = (COLORTYPE*)GetPixel(x, y);
                                                                    bitmap->PutBlendPixel(x, y, color, color->a);                                                                    
                                                                  }
                                                              }

                                                            return bitmap;
                                                          }

    bool                                                  FlipHorizontal                  ()
                                                          {

                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmap = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(width, height, mode);
                                                            if(!bitmap) return false;

                                                            COLORTYPE* color;

                                                            for(XDWORD y=0; y<height; y++)
                                                              {
                                                                for(XDWORD x=0; x<width; x++)
                                                                  {
                                                                    color = (COLORTYPE*)GetPixel(x, y);
                                                                    bitmap->PutBlendPixel(width-x-1, y, color, color->a);                                                                        
                                                                  }
                                                              }

                                                            if(!CopyFrom(bitmap)) return false;

                                                            GEN_DELETE bitmap;

                                                            return true;
                                                          }

    bool                                                  FlipVertical                    ()
                                                          {
                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmap = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(width, height, mode);
                                                            if(!bitmap) return false;

                                                            COLORTYPE* color;

                                                            for(XDWORD y=0; y<height; y++)
                                                              {
                                                                for(XDWORD x=0; x<width; x++)
                                                                  {
                                                                    color = (COLORTYPE*)GetPixel(x, y);
                                                                    bitmap->PutBlendPixel(x, height-y-1, color, color->a);
                                                                  }
                                                              }

                                                            if(!CopyFrom(bitmap)) return false;

                                                            GEN_DELETE bitmap;

                                                            return true;
                                                          }

    bool                                                  Rotate                          (float degrees)
                                                          {
                                                            float radians = DEGREE2RADIAN(degrees);
                                                            float oneminusradians = DEGREE2RADIAN((180-degrees));

                                                            float cosine  = (float)cos(radians);
                                                            float sine    = (float)sin(radians);

                                                            float oneminussine    = (float)sin(oneminusradians);

                                                            int w = (int)(height*fabs(oneminussine) + width*fabs(cosine));
                                                            int h = (int)(height*fabs(cosine) + width*fabs(oneminussine));

                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmap = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(w, h, mode);
                                                            if(!bitmap) return false;

                                                            int w2 = (w/2);
                                                            int h2 = (h/2);

                                                            COLORTYPE* color;

                                                            for(int y=-h2;y<h2;y++)
                                                              {
                                                                for(int x=-w2;x<w2;x++)
                                                                  {
                                                                    int sx = (int)((x*cosine) + (y*sine)) + (width/2);
                                                                    int sy = (int)((y*cosine) - (x*sine)) + (height/2);

                                                                    if( (sx>=0) && (sx<(int)width) && (sy>=0) && (sy<(int)height) )
                                                                      {
                                                                        color = (COLORTYPE*)GetPixel(sx, sy);
                                                                        bitmap->PutBlendPixel(x+w2, y+h2, color, color->a);
                                                                      }
                                                                  }
                                                              }

                                                            if(!CopyFrom(bitmap)) return false;

                                                            GEN_DELETE bitmap;

                                                            return true;
                                                          }

  bool                                                    Scale                           (int nwidth, int nheight)
                                                          {
                                                            float  xstep;
                                                            float  ystep;
                                                            float  xratio;
                                                            float  yratio;
                                                            XWORD  xcount;
                                                            XWORD  ycount;

                                                            xratio = (float)width  / nwidth;
                                                            yratio = (float)height / nheight;

                                                            GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>* bitmap = GEN_NEW GRPBITMAPPIXELFORMATBUFFER<PIXELFORMATBUFFER, COLORTYPE>(nwidth, nheight, mode);
                                                            if(!bitmap) return false; 

                                                            COLORTYPE* color;

                                                            for(ystep=0,ycount=0; ycount<nheight; ystep+=yratio,ycount++)
                                                              {
                                                                for(xstep=0,xcount=0;xcount<nwidth; xstep+=xratio,xcount++)
                                                                  {
                                                                    color = (COLORTYPE*)GetPixel((int)xstep, (int)ystep);
                                                                    bitmap->PutBlendPixel(xcount, ycount, color, color->a);
                                                                  }
                                                              }

                                                            if(!CopyFrom(bitmap)) return false;

                                                            GEN_DELETE bitmap;

                                                            return true;
                                                          }

    bool                                                  Crop(GRPRECTINT& rect)
                                                          {
                                                            GRPBITMAP* bitmap = GetSubBitmap(rect);
                                                            if(!bitmap) return false;

                                                            if(!CopyFrom(bitmap)) return false;

                                                            GEN_DELETE bitmap;

                                                            return true;
                                                          }

  private:

    void                                                  Clean                           ()
                                                          {
                                                            pixelformatbuffer = NULL;
                                                            renderer_base     = NULL;
                                                          }

    bool                                                  CreateBuffers                   ()
                                                          {
                                                            buffersize = width * height * GetBytesperPixel();

                                                            buffer = GEN_NEW XBYTE[buffersize];
                                                            if(!buffer) return false;

                                                            memset(buffer, 0, buffersize);

                                                            rbuffer.attach(buffer, width , height, (IsBufferInverse()?1:-1)*((int)width * GetBytesperPixel()));

                                                            pixelformatbuffer = GEN_NEW PIXELFORMATBUFFER(rbuffer);
                                                            if(!pixelformatbuffer) return false;

                                                            renderer_base =  GEN_NEW agg::renderer_base<PIXELFORMATBUFFER>(*pixelformatbuffer);
                                                            if(!renderer_base)  return false;

                                                            return true;
                                                          }

    bool                                                  CopyBuffer                      (XBYTE* buffer)
                                                          {
                                                            if(!buffersize) return false;
                                                            memcpy(this->buffer, buffer, buffersize);

                                                            return true;
                                                          }

    bool                                                  DeleteBuffers                   ()
                                                          {
                                                            if(buffer)
                                                              {
                                                                GEN_DELETE_ARRAY buffer;
                                                                buffer = NULL;
                                                              }

                                                            if(pixelformatbuffer)
                                                              {
                                                                GEN_DELETE pixelformatbuffer;
                                                                pixelformatbuffer = NULL;
                                                              }

                                                            if(renderer_base)
                                                              {
                                                                GEN_DELETE renderer_base;
                                                                renderer_base = NULL;
                                                              }

                                                            return true;
                                                          }


    agg::rendering_buffer                                 rbuffer;
    PIXELFORMATBUFFER*                                    pixelformatbuffer;
    agg::renderer_base<PIXELFORMATBUFFER>*                renderer_base;
    COLORTYPE                                             color;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





