/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPCursorMotion.cpp
* 
* @class      INPCURSORMOTION
* @brief      Input Cursor Motion
* @ingroup    INPUT
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

#include "INPCursorMotion.h"

#include <stdlib.h>
#include <cmath>
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTIONPOINT::INPCURSORMOTIONPOINT()
* @brief      Constructor of class
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTIONPOINT::INPCURSORMOTIONPOINT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTIONPOINT::~INPCURSORMOTIONPOINT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTIONPOINT::~INPCURSORMOTIONPOINT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float INPCURSORMOTIONPOINT::GetX()
* @brief      Get x
* @ingroup    INPUT
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float INPCURSORMOTIONPOINT::GetX()
{
  return x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float INPCURSORMOTIONPOINT::GetY()
* @brief      Get y
* @ingroup    INPUT
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float INPCURSORMOTIONPOINT::GetY()
{
  return y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float INPCURSORMOTIONPOINT::GetZ()
* @brief      Get z
* @ingroup    INPUT
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float INPCURSORMOTIONPOINT::GetZ()
{
  return z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTIONPOINT::Set(float x, float y, float z)
* @brief      Set value
* @ingroup    INPUT
* 
* @param[in]  x : GEN_NEW x position
* @param[in]  y : GEN_NEW y position
* @param[in]  z : GEN_NEW z position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTIONPOINT::Set(float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTIONPOINT::SetX(float x)
* @brief      Set x
* @ingroup    INPUT
* 
* @param[in]  x : GEN_NEW x position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTIONPOINT::SetX(float x)
{
  this->x = x;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTIONPOINT::SetY(float y)
* @brief      Set y
* @ingroup    INPUT
* 
* @param[in]  y : GEN_NEW y position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTIONPOINT::SetY(float y)
{
  this->y = y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTIONPOINT::SetZ(float z)
* @brief      Set z
* @ingroup    INPUT
* 
* @param[in]  z : GEN_NEW z position
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTIONPOINT::SetZ(float z)
{
  this->z = z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTIONPOINT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTIONPOINT::Clean()
{
  x = 0;
  y = 0;
  z = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTION::INPCURSORMOTION()
* @brief      Constructor of class
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTION::INPCURSORMOTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTION::~INPCURSORMOTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTION::~INPCURSORMOTION()
{
  DeleteAllPoints();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<INPCURSORMOTIONPOINT*>* INPCURSORMOTION::Get()
* @brief      Get value
* @ingroup    INPUT
* 
* @return     XVECTOR<INPCURSORMOTIONPOINT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<INPCURSORMOTIONPOINT*>* INPCURSORMOTION::Get()
{
  return &points;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPCURSORMOTION::GetNPoints()
* @brief      Get N points
* @ingroup    INPUT
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPCURSORMOTION::GetNPoints()
{
  return points.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTIONPOINT* INPCURSORMOTION::GetFirstPoint()
* @brief      Get first point
* @ingroup    INPUT
* 
* @return     INPCURSORMOTIONPOINT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTIONPOINT* INPCURSORMOTION::GetFirstPoint()
{
  if(!points.GetSize()) return NULL;

  INPCURSORMOTIONPOINT* point = (INPCURSORMOTIONPOINT*)points.Get(0);

  return point;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTIONPOINT* INPCURSORMOTION::GetLastPoint()
* @brief      Get last point
* @ingroup    INPUT
* 
* @return     INPCURSORMOTIONPOINT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTIONPOINT* INPCURSORMOTION::GetLastPoint()
{
  if(points.GetSize() < 1) return NULL;

  INPCURSORMOTIONPOINT* point = (INPCURSORMOTIONPOINT*)points.Get((int)points.GetSize()-1);

  return point;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::AddPoint(float x, float y, float z)
* @brief      Add point
* @ingroup    INPUT
* 
* @param[in]  x : GEN_NEW x point
* @param[in]  y : GEN_NEW y point
* @param[in]  z : GEN_NEW z point
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::AddPoint(float x, float y, float z)
{
  INPCURSORMOTIONPOINT* point = GEN_NEW INPCURSORMOTIONPOINT();
  if(!point) return false;

  point->Set(x, y, z);

  points.Add(point);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::DeleteAllPoints()
* @brief      Delete all points
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::DeleteAllPoints()
{
  if(points.IsEmpty())  return false;

  points.DeleteContents();
  points.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::AddFromLine(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x1, int y1, int x2, int y2)
* @brief      Add from line
* @ingroup    INPUT
* 
* @param[in]  reducedmode : reduced mode
* @param[in]  reducedfactor : factor of reduction
* @param[in]  x1 : x origin of rect
* @param[in]  y1 : y origin of rect
* @param[in]  x2 : x target of rect
* @param[in]  y2 : x target of rect
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::AddFromLine(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x1, int y1, int x2, int y2)
{
  int x, xinc1, xinc2;
  int y, yinc1, yinc2;
  int i, deltax, deltay, npixels;
  int d, dinc1, dinc2;

  // Calculate deltax and deltay for initialisation
  deltax = abs(x2 - x1);
  deltay = abs(y2 - y1);

  // Initialize all vars based on which is the independent variable
  if(deltax>=deltay)
    {
      // x is independent variable }
      npixels   = deltax + 1;
      d         = (2 * deltay) - deltax;
      dinc1     = deltay >> 1;
      dinc2     = (deltay - deltax) >> 1;
      xinc1     = 1;
      xinc2     = 1;
      yinc1     = 0;
      yinc2     = 1;
    }
   else
    {
      // y is independent variable }
      npixels   = deltay + 1;
      d         = (2 * deltax) - deltay;
      dinc1     = deltax >> 1;
      dinc2     = (deltax - deltay) >> 1;
      xinc1     = 0;
      xinc2     = 1;
      yinc1     = 1;
      yinc2     = 1;
    }

  // Make sure x and y move in the right directions
  if(x1>x2)
    {
      xinc1 = -xinc1;
      xinc2 = -xinc2;
    }

  if(y1>y2)
    {
      yinc1 = -yinc1;
      yinc2 = -yinc2;
    }

  // Start drawing at
  x = x1;
  y = y1;


  int  steps    = CalculateReducedModeSteps(reducedmode, reducedfactor, npixels);
  int  incstep  = steps;
  bool addpoint = false;

  // Draw the pixels }
  for(i=0;i<npixels;i++)
    {
      if(incstep == steps)
        {
          addpoint = true;
          incstep  = 0;
        }

      if(addpoint)
            AddPoint((float)x, (float)y, 0);
       else incstep++;

      addpoint = false;

      if(d<0)
        {
          d = d + dinc1;
          x = x + xinc1;
          y = y + yinc1;
        }
       else
        {
          d = d + dinc2;
          x = x + xinc2;
          y = y + yinc2;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::AddFromCircle(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int xc, int yc, int radio)
* @brief      Add from circle
* @ingroup    INPUT
* 
* @param[in]  reducedmode : reduced mode
* @param[in]  reducedfactor : factor of reduction
* @param[in]  xc : x center of circle
* @param[in]  yc : y center of circle
* @param[in]  radio : radio of circle
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::AddFromCircle(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int xc, int yc, int radio)
{
  float deg     = 0;
  int   npixels = 0;

  do{ deg+=0.005f;

      npixels++;

     } while (deg <= 6.4);

  deg = 0;

  int  steps    = CalculateReducedModeSteps(reducedmode, reducedfactor, npixels);
  int  incstep  = steps;
  bool addpoint = false;

  do { int x = Round(radio * cos(deg));
       int y = Round(radio * sin(deg));

       if(incstep == steps)
         {
           addpoint = true;
           incstep  = 0;
         }

       if(addpoint)
              AddPoint((float)xc+x, (float)yc+y, true);
         else incstep++;

       addpoint = false;

       deg += 0.005f;

     } while (deg <= 6.4);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::AddFromElipse(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x, int y, int rx, int ry)
* @brief      Add from elipse
* @ingroup    INPUT
* 
* @param[in]  reducedmode : reduced mode
* @param[in]  reducedfactor : factor of reduction
* @param[in]  x : x center of elipse
* @param[in]  y : y center of elipse
* @param[in]  rx : radio x of elipse
* @param[in]  ry : radio y of elipse
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::AddFromElipse(INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x, int y, int rx, int ry)
{
  int ix, iy;
  int h, i, j, k;
  int oh, oi, oj, ok;

  if(rx < 1)  rx = 1;
  if(ry < 1)  ry = 1;

  h = i = j = k = 0xFFFF;

  if(rx > ry)
    {
      ix = 0;
      iy = rx * 64;

      do{   oh = h;
            oi = i;
            oj = j;
            ok = k;

            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

          if(((h != oh) || (k != ok)) && (h < oi))
            {

              AddPoint((float)x+h, (float)y+k, true);

              if(h) AddPoint((float)x-h, (float)y+k, true);

              if(k)
                {
                  AddPoint((float)x+h, (float)y-k, true);

                  if(h) AddPoint((float)x-h, (float)y-k, true);
                }

            }

          if(((i!=oi)||(j!=oj)) && (h < i))
            {

              AddPoint((float)x+i, (float)y+j, true);

              if(i) AddPoint((float)x-i, (float)y+j, true);

              if(j)
                {
                  AddPoint((float)x+i, (float)y-j, true);

                  if(i) AddPoint((float)x-i, (float)y-j, true);
                }

            }

          ix = ix + iy / rx;
          iy = iy - ix / rx;

        } while (i > h);

    }
   else
    {
      ix = 0;
      iy = ry * 64;

      do{   oh = h;
            oi = i;
            oj = j;
            ok = k;

            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if(((j != oj) || (i != oi)) && (h < i))
              {

                AddPoint((float)x+j, (float)y+i, true);

                if(j) AddPoint((float)x-j, (float)y+i, true);

                if(i)
                  {
                    AddPoint((float)x+j, (float)y-i, true);

                    if(j) AddPoint((float)x-j, (float)y-i, true);

                  }
              }

           if(((k != ok) || (h != oh)) && (h < oi))
             {

               AddPoint((float)x+k, (float)y+h, true);

                if(k) AddPoint((float)x-k, (float)y+h, true);

                if(h)
                  {
                    AddPoint((float)x+k, (float)y-h, true);

                    if(k) AddPoint((float)x-k, (float)y-h, true);
                  }

             }

           ix = ix + iy / ry;
           iy = iy - ix / ry;

        } while(i > h);
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float INPCURSORMOTION::GetAngle()
* @brief      Get angle
* @ingroup    INPUT
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float INPCURSORMOTION::GetAngle()
{
  if(points.GetSize() < 2) return 0;

  INPCURSORMOTIONPOINT* point[2];

  point[0] = GetFirstPoint();
  point[1] = GetLastPoint();

  double radians = atan2(point[1]->GetY()-point[0]->GetY() , point[1]->GetX()-point[0]->GetX());

  float angle = (float)((180*radians)/PI);

  if(angle<0) angle = 180 + (180+angle);

  return angle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPCURSORMOTION::GetModule()
* @brief      Get module
* @ingroup    INPUT
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPCURSORMOTION::GetModule()
{
  if(points.GetSize() < 2) return 0;

  INPCURSORMOTIONPOINT* point[2];

  point[0] = GetFirstPoint();
  point[1] = GetLastPoint();

  float difx = point[1]->GetX() - point[0]->GetX();
  float dify = point[1]->GetY() - point[0]->GetY();

  int module = (int) (((int)difx^2) + ((int)dify^2)) ^ (1/2);

  return module;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::IsInRect(int x, int y, int width, int height)
* @brief      Is in rect
* @ingroup    INPUT
* 
* @param[in]  x : x position of rect
* @param[in]  y : y position of rect
* @param[in]  width : width of rect
* @param[in]  height : height of rect
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::IsInRect(int x, int y, int width, int height)
{
  if(points.IsEmpty()) return false;

  int npoints = 0;

  for(int c=0;c<(int)points.GetSize();c++)
    {
      INPCURSORMOTIONPOINT* point = (INPCURSORMOTIONPOINT*)points.Get(c);
      if(point)
        {
          if((point->GetX() >= x) &&  (point->GetX() <= (x+width)) &&
             (point->GetY() >= y) &&  (point->GetY() <= (y+height))) npoints++;
        }
    }

  if(npoints == points.GetSize()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::IsInCurse()
* @brief      Is in curse
* @ingroup    INPUT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::IsInCurse()
{
  return isincurse;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::SetIsInCurse(bool isincurse)
* @brief      Set is in curse
* @ingroup    INPUT
* 
* @param[in]  isincurse : Isincurse value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::SetIsInCurse(bool isincurse)
{
  this->isincurse = isincurse;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::IsReadyToTest(XDWORD npoints_min)
* @brief      Is ready to test
* @ingroup    INPUT
* 
* @param[in]  npoints_min : Npoints min value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::IsReadyToTest(XDWORD npoints_min)
{
  if(!isincurse) 
    {
      if(points.GetSize() >= npoints_min) 
        {
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::InvertYAxis(XDWORD screen_height)
* @brief      Invert Y axis
* @ingroup    INPUT
* 
* @param[in]  screen_height : Screen height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::InvertYAxis(XDWORD screen_height)
{
  if(!screen_height) return false;

  if(points.IsEmpty()) return false;

  for(XDWORD c=0; c<points.GetSize(); c++)
    {
      INPCURSORMOTIONPOINT* point = (INPCURSORMOTIONPOINT*)points.Get(c);
      if(point) point->SetY(screen_height - point->GetY());      
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTION_DIR INPCURSORMOTION::GetDirectionByAngle(bool indetail)
* @brief      Get direction by angle
* @ingroup    INPUT
* 
* @param[in]  indetail : Indetail value.
* 
* @return     INPCURSORMOTION_DIR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTION_DIR INPCURSORMOTION::GetDirectionByAngle(bool indetail)
{  
  if(points.GetSize() < 2) return INPCURSORMOTION_DIR_UNKNOWN;

  float angle = GetAngle();

  if(indetail)
    {
      if((angle > 337.5f) || (angle <=  22.5f))  return INPCURSORMOTION_DIR_RIGHT;
      if((angle >  22.5f) && (angle <=  67.5f))  return INPCURSORMOTION_DIR_RIGHTUP;
      if((angle >  67.5f) && (angle <= 112.5f))  return INPCURSORMOTION_DIR_UP;
      if((angle > 112.5f) && (angle <= 157.5f))  return INPCURSORMOTION_DIR_LEFTUP;
      if((angle > 157.5f) && (angle <= 202.5f))  return INPCURSORMOTION_DIR_LEFT;
      if((angle > 202.5f) && (angle <= 247.5f))  return INPCURSORMOTION_DIR_LEFTDOWN;
      if((angle > 247.5f) && (angle <= 292.5f))  return INPCURSORMOTION_DIR_DOWN;
      if((angle > 292.5f) && (angle <= 337.5f))  return INPCURSORMOTION_DIR_RIGHTDOWN;
    }
   else
    {
      if((angle > 270) || (angle <=  45))        return INPCURSORMOTION_DIR_RIGHT;
      if((angle >  45) && (angle <= 135))        return INPCURSORMOTION_DIR_UP;
      if((angle > 135) && (angle <= 180))        return INPCURSORMOTION_DIR_LEFT;
      if((angle > 180) && (angle <= 270))        return INPCURSORMOTION_DIR_DOWN;
    }

  return INPCURSORMOTION_DIR_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPCURSORMOTION_DIR INPCURSORMOTION::GetDirectionByDifferential(XDWORD* differential)
* @brief      Get direction by differential
* @ingroup    INPUT
* 
* @param[in]  differential : Differential pointer to use.
* 
* @return     INPCURSORMOTION_DIR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPCURSORMOTION_DIR INPCURSORMOTION::GetDirectionByDifferential(XDWORD* differential)
{
  INPCURSORMOTION_DIR dir = INPCURSORMOTION_DIR_UNKNOWN;

  if(differential) (*differential) = 0;

  if(points.GetSize() < 2) return dir;

  int max_up    = 0;
  int max_down  = 0;
  int max_right = 0;
  int max_left  = 0;

  int x_origin  = (int)points.Get(0)->GetX();
  int y_origin  = (int)points.Get(0)->GetY();

  for(XDWORD c=1; c<points.GetSize(); c++)
    {
      max_right = __MAX(((int)points.Get(c)->GetX() - x_origin), max_right);
      max_left  = __MAX((x_origin - (int)points.Get(c)->GetX()), max_left);

      max_up    = __MAX(((int)points.Get(c)->GetY() - y_origin), max_up);
      max_down  = __MAX((y_origin - (int)points.Get(c)->GetY()), max_down);     
    }
  
  int x   = __MAX(max_right, max_left);
  int y   = __MAX(max_up, max_down);

  if(x > y)
    {
      if(differential) (*differential) = x;

      dir = INPCURSORMOTION_DIR_LEFT;
      if(max_right > max_left) dir = INPCURSORMOTION_DIR_RIGHT;
    }
   else
    {
      if(differential) (*differential) = y;

      dir = INPCURSORMOTION_DIR_DOWN;
      if(max_up > max_down) dir = INPCURSORMOTION_DIR_UP;
    }
 
  return dir;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD INPCURSORMOTION::GetTimeElapsed()
* @brief      Get time elapsed
* @ingroup    INPUT
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD INPCURSORMOTION::GetTimeElapsed()
{
  return timeelapsed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::SetTimeElapsed(XQWORD timeelapsed)
* @brief      Set time elapsed
* @ingroup    INPUT
* 
* @param[in]  timeelapsed : GEN_NEW time elapsed
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::SetTimeElapsed(XQWORD timeelapsed)
{
  this->timeelapsed = timeelapsed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPCURSORMOTION::Reset()
* @brief      Reset
* @ingroup    INPUT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPCURSORMOTION::Reset()
{
  isincurse   = false;
  timeelapsed = 0;

  DeleteAllPoints();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPCURSORMOTION::CalculateReducedModeSteps(INPCURSORMOTION_REDUCEDMODE mode, int factor, int npixels)
* @brief      Calculate reduced mode steps
* @ingroup    INPUT
* 
* @param[in]  mode : mode of reduction
* @param[in]  factor : factor reduction
* @param[in]  npixels : n pixels to reduction
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPCURSORMOTION::CalculateReducedModeSteps(INPCURSORMOTION_REDUCEDMODE mode, int factor, int npixels)
{
  int steps = 0;

  switch(mode)
    {
      case INPCURSORMOTION_REDUCEDMODE_NONE            :                                 break;
      case INPCURSORMOTION_REDUCEDMODE_BYSTEPS         : steps = factor;                 break;
      case INPCURSORMOTION_REDUCEDMODE_NMAX            : steps = (npixels/factor);       break;
      case INPCURSORMOTION_REDUCEDMODE_NMAXPERCENT     : steps = (npixels*factor)/100;   break;
    }

  return steps;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPCURSORMOTION::Round(double a)
* @brief      Round
* @ingroup    INPUT
* 
* @param[in]  a : value to round
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPCURSORMOTION::Round(double a)
{
  return int(a + 0.5);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPCURSORMOTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPCURSORMOTION::Clean()
{
  isincurse   = false;
  timeelapsed = 0;
}








