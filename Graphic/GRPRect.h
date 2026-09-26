/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPRect.h
* 
* @class      GRPRECT
* @brief      Graphics Rect template
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

#include <cmath>

#include "XBase.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template<class T>
class GRPRECTBASE
{
  public:

    typedef GRPRECTBASE<T>    SELFTYPE;

                              GRPRECTBASE                       ()
                              {
                                Clean();
                              }

                              GRPRECTBASE                       (T x1, T y1, T x2, T y2)
                              {
                                Clean();

                                Set(x1, y1, x2, y2);
                              }

    virtual                  ~GRPRECTBASE                       ()
                              {
                                Clean();
                              }

    void                      Set                               (T x1, T y1, T x2, T y2)
                              {
                                this->x1  = x1;
                                this->y1  = y1;
                                this->x2  = x2;
                                this->y2  = y2;
                              }

    T                         GetWidth                          ()
                              {
                                T width  = abs(this->x2 - this->x1);

                                return width;
                              }

    T                         GetHeight                         ()
                              {
                                T height = abs(this->y2 - this->y1);

                                return height;
                              }

    const SELFTYPE&           Normalize                         ()
                              {
                                T t;
                                if(x1 > x2) { t = x1; x1 = x2; x2 = t; }
                                if(y1 > y2) { t = y1; y1 = y2; y2 = t; }

                                return *this;
                              }

    bool                      Clip                              (const SELFTYPE& r)
                              {
                                if(x2 > r.x2) x2 = r.x2;
                                if(y2 > r.y2) y2 = r.y2;
                                if(x1 < r.x1) x1 = r.x1;
                                if(y1 < r.y1) y1 = r.y1;

                                return x1 <= x2 && y1 <= y2;
                              }

    bool                      IsValid                           () const
                              {
                                return x1 <= x2 && y1 <= y2;
                              }

    bool                      IsEmpty                           () const
                              {
                                if(x1 || y1 || x2 || y2) 
                                  {
                                    return false;
                                  }

                                return true;
                              }

    bool                      IsHit                             (T x, T y) const
                              {
                                return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
                              }

    int                       IsEqual                           (GRPRECTBASE* rect)
                              {
                                if(!rect) return ISLESS;

                                int size[2];

                                size[0]  = (x2 - x1) * (y2 - y1);
                                size[1]  = (rect->x2 - rect->x1) * (rect->y2 - rect->y1);

                                if(size[0] > size[1])   return ISGREATER;
                                if(size[0] < size[1])   return ISLESS;

                                return ISEQUAL;
                              }

    bool                      CopyFrom                          (GRPRECTBASE* rect)
                              {
                                x1 = rect->x1;
                                y1 = rect->y1;
                                x2 = rect->x2;
                                y2 = rect->y2;

                                return IsValid();                                  
                              }

    bool                      CopyTo                            (GRPRECTBASE* rect)
                              {
                                rect->x1 = x1;
                                rect->y1 = y1;
                                rect->x2 = x2;
                                rect->y2 = y2;

                                return IsValid();
                              }


    T                         x1;
    T                         y1;
    T                         x2;
    T                         y2;


  private:

    void                      Clean                             ()
                              {
                                x1 = 0;
                                y1 = 0;
                                x2 = 0;
                                y2 = 0;
                              }
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


template<class R>
inline R IntersectRectangles(const R& r1, const R& r2)
{
  R r = r1;

  if(r.x2 > r2.x2) r.x2 = r2.x2;
  if(r.y2 > r2.y2) r.y2 = r2.y2;
  if(r.x1 < r2.x1) r.x1 = r2.x1;
  if(r.y1 < r2.y1) r.y1 = r2.y1;

  return r;
}

template<class R>
inline R UniteRectangles(const R& r1, const R& r2)
{
  R r = r1;

  if(r.x2 < r2.x2) r.x2 = r2.x2;
  if(r.y2 < r2.y2) r.y2 = r2.y2;
  if(r.x1 > r2.x1) r.x1 = r2.x1;
  if(r.y1 > r2.y1) r.y1 = r2.y1;

  return r;
}




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef GRPRECTBASE<int>    GRPRECTINT;
typedef GRPRECTBASE<float>  GRPRECTFLOAT;
typedef GRPRECTBASE<double> GRPRECTDOUBLE;





