/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGTransform.cpp
* 
* @class      GRPVECTORFILESVGTRANSFORM
* @brief      Graphic Vector File SVG Transform class (2x3 affine matrix)
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

#include "GRPVectorFileSVGTransform.h"

#include <math.h>

#include "XVector.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GRPVECTORFILESVGTRANSFORM_DEG2RAD   (3.14159265358979323846 / 180.0)



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGTRANSFORM::GRPVECTORFILESVGTRANSFORM()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGTRANSFORM::GRPVECTORFILESVGTRANSFORM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGTRANSFORM::~GRPVECTORFILESVGTRANSFORM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGTRANSFORM::~GRPVECTORFILESVGTRANSFORM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::SetIdentity()
* @brief      Set identity
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::SetIdentity()
{
  a = 1.0;  b = 0.0;  c = 0.0;  d = 1.0;  e = 0.0;  f = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGTRANSFORM::IsIdentity()
* @brief      Is identity
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGTRANSFORM::IsIdentity()
{
  return (a==1.0 && b==0.0 && c==0.0 && d==1.0 && e==0.0 && f==0.0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::Set(double a, double b, double c, double d, double e, double f)
* @brief      Set value
* @ingroup    GRAPHIC
* 
* @param[in]  a : matrix a
* @param[in]  b : matrix b
* @param[in]  c : matrix c
* @param[in]  d : matrix d
* @param[in]  e : matrix e
* @param[in]  f : matrix f
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::Set(double a, double b, double c, double d, double e, double f)
{
  this->a = a;  this->b = b;  this->c = c;  this->d = d;  this->e = e;  this->f = f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetA()
* @brief      Get a
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetA()      { return a; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetB()
* @brief      Get b
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetB()      { return b; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetC()
* @brief      Get c
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetC()      { return c; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetD()
* @brief      Get d
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetD()      { return d; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetE()
* @brief      Get e
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetE()      { return e; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGTRANSFORM::GetF()
* @brief      Get f
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGTRANSFORM::GetF()      { return f; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::CopyFrom(GRPVECTORFILESVGTRANSFORM& transform)
* @brief      Copy from
* @ingroup    GRAPHIC
* 
* @param[in]  transform : transform to copy from
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::CopyFrom(GRPVECTORFILESVGTRANSFORM& transform)
{
  a = transform.GetA();
  b = transform.GetB();
  c = transform.GetC();
  d = transform.GetD();
  e = transform.GetE();
  f = transform.GetF();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::Multiply(GRPVECTORFILESVGTRANSFORM& transform)
* @brief      Multiply : this = this * transform (transform is applied first to a point)
* @ingroup    GRAPHIC
* 
* @param[in]  transform : right hand side matrix
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::Multiply(GRPVECTORFILESVGTRANSFORM& transform)
{
  double oa = transform.GetA();
  double ob = transform.GetB();
  double oc = transform.GetC();
  double od = transform.GetD();
  double oe = transform.GetE();
  double of = transform.GetF();

  double na = (a*oa) + (c*ob);
  double nb = (b*oa) + (d*ob);
  double nc = (a*oc) + (c*od);
  double nd = (b*oc) + (d*od);
  double ne = (a*oe) + (c*of) + e;
  double nf = (b*oe) + (d*of) + f;

  a = na;  b = nb;  c = nc;  d = nd;  e = ne;  f = nf;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::ApplyToPoint(double& x, double& y)
* @brief      Apply to point
* @ingroup    GRAPHIC
* 
* @param[in]  x : point x (in/out)
* @param[in]  y : point y (in/out)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::ApplyToPoint(double& x, double& y)
{
  double nx = (a*x) + (c*y) + e;
  double ny = (b*x) + (d*y) + f;

  x = nx;
  y = ny;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGTRANSFORM::ParseFromString(XCHAR* string)
* @brief      Parse from string : "translate(10,20) rotate(45) scale(2)" ...
* @ingroup    GRAPHIC
* 
* @param[in]  string : transform attribute value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGTRANSFORM::ParseFromString(XCHAR* string)
{
  if(!string) return false;

  SetIdentity();

  XSTRING source(string);

  int index = 0;
  int size  = (int)source.GetSize();

  while(index < size)
    {
      int open = source.Find(__L("("), false, index);
      if(open < 0) break;

      XSTRING name;
      source.Copy(index, open, name);
      name.DeleteCharacter(__C(' '));
      name.DeleteCharacter(__C(','));
      name.DeleteCharacter(__C('\t'));
      name.DeleteCharacter(__C('\n'));
      name.DeleteCharacter(__C('\r'));

      int close = source.Find(__L(")"), false, open);
      if(close < 0) break;

      XSTRING args;
      source.Copy(open+1, close, args);

      ParsePrimitive(name, args);

      index = close + 1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGTRANSFORM::ParsePrimitive(XSTRING& name, XSTRING& args)
* @brief      Parse primitive : compose one transform primitive into the matrix
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  name : primitive name (translate, scale, rotate, matrix, skewX, skewY)
* @param[in]  args : arguments inside the parenthesis
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGTRANSFORM::ParsePrimitive(XSTRING& name, XSTRING& args)
{
  double values[6] = { 0, 0, 0, 0, 0, 0 };
  int    nvalues   = 0;

  ExtractNumbers(args, values, 6, nvalues);

  GRPVECTORFILESVGTRANSFORM prim;
  prim.SetIdentity();

  if(!name.Compare(__L("matrix"), true) && (nvalues >= 6))
    {
      prim.Set(values[0], values[1], values[2], values[3], values[4], values[5]);
    }
   else if(!name.Compare(__L("translate"), true) && (nvalues >= 1))
    {
      double tx = values[0];
      double ty = (nvalues >= 2) ? values[1] : 0.0;
      prim.Set(1.0, 0.0, 0.0, 1.0, tx, ty);
    }
   else if(!name.Compare(__L("scale"), true) && (nvalues >= 1))
    {
      double sx = values[0];
      double sy = (nvalues >= 2) ? values[1] : sx;
      prim.Set(sx, 0.0, 0.0, sy, 0.0, 0.0);
    }
   else if(!name.Compare(__L("rotate"), true) && (nvalues >= 1))
    {
      double rad = values[0] * GRPVECTORFILESVGTRANSFORM_DEG2RAD;
      double co  = cos(rad);
      double si  = sin(rad);

      if(nvalues >= 3)
        {
          double cx = values[1];
          double cy = values[2];

          // T(cx,cy) * R * T(-cx,-cy)
          GRPVECTORFILESVGTRANSFORM t1;  t1.Set(1.0, 0.0, 0.0, 1.0, cx, cy);
          GRPVECTORFILESVGTRANSFORM r;   r.Set (co, si, -si, co, 0.0, 0.0);
          GRPVECTORFILESVGTRANSFORM t2;  t2.Set(1.0, 0.0, 0.0, 1.0, -cx, -cy);

          prim.CopyFrom(t1);
          prim.Multiply(r);
          prim.Multiply(t2);
        }
       else
        {
          prim.Set(co, si, -si, co, 0.0, 0.0);
        }
    }
   else if(!name.Compare(__L("skewX"), true) && (nvalues >= 1))
    {
      double t = tan(values[0] * GRPVECTORFILESVGTRANSFORM_DEG2RAD);
      prim.Set(1.0, 0.0, t, 1.0, 0.0, 0.0);
    }
   else if(!name.Compare(__L("skewY"), true) && (nvalues >= 1))
    {
      double t = tan(values[0] * GRPVECTORFILESVGTRANSFORM_DEG2RAD);
      prim.Set(1.0, t, 0.0, 1.0, 0.0, 0.0);
    }
   else
    {
      return false;
    }

  Multiply(prim);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGTRANSFORM::ExtractNumbers(XSTRING& args, double* values, int maxvalues, int& nvalues)
* @brief      Extract numbers from a comma/space separated argument list
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  args : argument string
* @param[in]  values : output array
* @param[in]  maxvalues : output array capacity
* @param[in]  nvalues : number of values extracted
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGTRANSFORM::ExtractNumbers(XSTRING& args, double* values, int maxvalues, int& nvalues)
{
  nvalues = 0;

  XSTRING source(args);
  source.Replace(__L(","), __L(" "));

  XVECTOR<XSTRING*> tokens;
  source.Split(__C(' '), tokens, false);

  for(XDWORD c=0; c<tokens.GetSize(); c++)
    {
      if(nvalues >= maxvalues) break;

      XSTRING* token = tokens.Get(c);
      if(token && !token->IsEmpty())
        {
          values[nvalues] = token->ConvertToDouble(0, NULL, false);
          nvalues++;
        }
    }

  tokens.DeleteContents();
  tokens.DeleteAll();

  return (nvalues > 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGTRANSFORM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGTRANSFORM::Clean()
{
  SetIdentity();
}




