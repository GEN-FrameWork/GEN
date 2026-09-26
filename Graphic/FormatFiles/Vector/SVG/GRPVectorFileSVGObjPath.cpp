/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjPath.cpp
* 
* @class      GRPVECTORFILESVGOBJPATH
* @brief      Graphic Vector File SVG Object Path class (parses the 'd' attribute)
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

#include "GRPVectorFileSVGObjPath.h"

#include "XFileXML.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJPATH::GRPVECTORFILESVGOBJPATH()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJPATH::GRPVECTORFILESVGOBJPATH()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJPATH::~GRPVECTORFILESVGOBJPATH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJPATH::~GRPVECTORFILESVGOBJPATH()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read the common attributes and parse the 'd' geometry
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  XCHAR* valued = element->GetValueAttribute(__L("d"));
  if(valued)  ParsePathData(valued);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::BuildPath(GRP2DPATH& path)
* @brief      Build path : copy the already parsed path geometry into the output path
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::BuildPath(GRP2DPATH& path)
{
  for(XDWORD c=0; c<pathdata.GetSize(); c++)
    {
      GRP2DPATHSEGMENT* segment = pathdata.Get((int)c);
      if(!segment) continue;

      switch(segment->type)
        {
          case GRP2DPATHSEGMENTTYPE_MOVETO   :  path.MoveTo(segment->x, segment->y);
                                                break;

          case GRP2DPATHSEGMENTTYPE_LINETO   :  path.LineTo(segment->x, segment->y);
                                                break;

          case GRP2DPATHSEGMENTTYPE_CURVETO  :  path.CurveTo(segment->c1x, segment->c1y, segment->c2x, segment->c2y, segment->x, segment->y);
                                                break;

          case GRP2DPATHSEGMENTTYPE_QUADTO   :  path.QuadTo(segment->c1x, segment->c1y, segment->x, segment->y);
                                                break;

          case GRP2DPATHSEGMENTTYPE_ARCTO    :  path.ArcTo(segment->rx, segment->ry, segment->xrot, segment->largearc, segment->sweep, segment->x, segment->y);
                                                break;

          case GRP2DPATHSEGMENTTYPE_CLOSE    :  path.Close();
                                                break;

                                   default   :  break;
        }
    }

  return (!path.IsEmpty());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATH* GRPVECTORFILESVGOBJPATH::GetPathData()
* @brief      Get path data
* @ingroup    GRAPHIC
* 
* @return     GRP2DPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATH* GRPVECTORFILESVGOBJPATH::GetPathData()
{
  return &pathdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::ParsePathData(XCHAR* d)
* @brief      Parse path data : parse the SVG 'd' mini language into the path
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  d : 'd' attribute value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::ParsePathData(XCHAR* d)
{
  if(!d) return false;

  pathdata.DeleteAll();

  XSTRING source(d);
  int     len = (int)source.GetSize();
  int     pos = 0;

  double  cx   = 0.0;   double cy   = 0.0;                                      // current point
  double  sx   = 0.0;   double sy   = 0.0;                                      // current sub path start
  double  lc2x = 0.0;   double lc2y = 0.0;                                      // last cubic control (for S/s)
  double  lqx  = 0.0;   double lqy  = 0.0;                                      // last quadratic control (for T/t)

  XCHAR   command = 0;
  XCHAR   previous = 0;                                                         // previous (upper cased) command for smooth reflection

  while(pos < len)
    {
      while((pos < len) && IsSeparator(source[pos]))  pos++;
      if(pos >= len) break;

      if(IsCommand(source[pos]))
        {
          command = source[pos];
          pos++;
        }

      if(!command) break;

      bool  relative = (command >= __C('a')) && (command <= __C('z'));
      XCHAR upper    = relative ? (XCHAR)(command - __C('a') + __C('A')) : command;

      double x   = 0.0;  double y   = 0.0;
      double c1x = 0.0;  double c1y = 0.0;
      double c2x = 0.0;  double c2y = 0.0;
      double rx  = 0.0;  double ry  = 0.0;  double xrot = 0.0;
      bool   largearc = false;  bool sweep = false;

      switch(upper)
        {
          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('M') :  if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y)) { pos = len; break; }
                           if(relative) { x += cx;  y += cy; }
                           pathdata.MoveTo(x, y);
                           cx = x;  cy = y;  sx = x;  sy = y;
                           command  = relative ? __C('l') : __C('L');           // subsequent pairs are implicit line to
                           previous = __C('M');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('L') :  if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y)) { pos = len; break; }
                           if(relative) { x += cx;  y += cy; }
                           pathdata.LineTo(x, y);
                           cx = x;  cy = y;
                           previous = __C('L');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, x))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('H') :  if(!ReadNumber(source, pos, x)) { pos = len; break; }
                           if(relative) x += cx;
                           pathdata.LineTo(x, cy);
                           cx = x;
                           previous = __C('L');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('V') :  if(!ReadNumber(source, pos, y)) { pos = len; break; }
                           if(relative) y += cy;
                           pathdata.LineTo(cx, y);
                           cy = y;
                           previous = __C('L');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, c1x) || !ReadNumber(source, pos, c1y) || !ReadNumber(source, pos, c2x) || !ReadNumber(source, pos, c2y) || !ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('C') :  if(!ReadNumber(source, pos, c1x) || !ReadNumber(source, pos, c1y) ||
                              !ReadNumber(source, pos, c2x) || !ReadNumber(source, pos, c2y) ||
                              !ReadNumber(source, pos, x)   || !ReadNumber(source, pos, y))    { pos = len; break; }
                           if(relative) { c1x += cx; c1y += cy;  c2x += cx; c2y += cy;  x += cx; y += cy; }
                           pathdata.CurveTo(c1x, c1y, c2x, c2y, x, y);
                           lc2x = c2x;  lc2y = c2y;
                           cx = x;  cy = y;
                           previous = __C('C');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, c2x) || !ReadNumber(source, pos, c2y) || !ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('S') :  if(!ReadNumber(source, pos, c2x) || !ReadNumber(source, pos, c2y) ||
                              !ReadNumber(source, pos, x)   || !ReadNumber(source, pos, y))    { pos = len; break; }
                           if(relative) { c2x += cx; c2y += cy;  x += cx; y += cy; }
                           if((previous == __C('C')) || (previous == __C('S')))
                             {
                               c1x = (2.0 * cx) - lc2x;
                               c1y = (2.0 * cy) - lc2y;
                             }
                            else
                             {
                               c1x = cx;  c1y = cy;
                             }
                           pathdata.CurveTo(c1x, c1y, c2x, c2y, x, y);
                           lc2x = c2x;  lc2y = c2y;
                           cx = x;  cy = y;
                           previous = __C('S');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, c1x) || !ReadNumber(source, pos, c1y) || !ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('Q') :  if(!ReadNumber(source, pos, c1x) || !ReadNumber(source, pos, c1y) ||
                              !ReadNumber(source, pos, x)   || !ReadNumber(source, pos, y))    { pos = len; break; }
                           if(relative) { c1x += cx; c1y += cy;  x += cx; y += cy; }
                           pathdata.QuadTo(c1x, c1y, x, y);
                           lqx = c1x;  lqy = c1y;
                           cx = x;  cy = y;
                           previous = __C('Q');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('T') :  if(!ReadNumber(source, pos, x) || !ReadNumber(source, pos, y)) { pos = len; break; }
                           if(relative) { x += cx;  y += cy; }
                           if((previous == __C('Q')) || (previous == __C('T')))
                             {
                               c1x = (2.0 * cx) - lqx;
                               c1y = (2.0 * cy) - lqy;
                             }
                            else
                             {
                               c1x = cx;  c1y = cy;
                             }
                           pathdata.QuadTo(c1x, c1y, x, y);
                           lqx = c1x;  lqy = c1y;
                           cx = x;  cy = y;
                           previous = __C('T');
                           break;

          /**-------------------------------------------------------------------------------------------------------------------
          * 
          * @fn         case __C( ) : if(!ReadNumber(source, pos, rx) || !ReadNumber(source, pos, ry) || !ReadNumber(source, pos, xrot) || !ReadFlag (source, pos, largearc) || !ReadFlag(source, pos, sweep) || !ReadNumber(source, pos, x) || !ReadNumber(source, pos, y))
          * @brief      C
          * @ingroup    GRAPHIC
          * 
          * 
          * @return     case : Requested value.
          * 
          * --------------------------------------------------------------------------------------------------------------------*/
          case __C('A') :  if(!ReadNumber(source, pos, rx)   || !ReadNumber(source, pos, ry) || !ReadNumber(source, pos, xrot) ||
                              !ReadFlag  (source, pos, largearc) || !ReadFlag(source, pos, sweep) ||
                              !ReadNumber(source, pos, x)    || !ReadNumber(source, pos, y))   { pos = len; break; }
                           if(relative) { x += cx;  y += cy; }
                           pathdata.ArcTo(rx, ry, xrot, largearc, sweep, x, y);
                           cx = x;  cy = y;
                           previous = __C('A');
                           break;

          case __C('Z') :  pathdata.Close();
                           cx = sx;  cy = sy;
                           previous = __C('Z');
                           break;

                default :  pos = len;                                           // unknown command : stop
                           break;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::ReadNumber(XSTRING& source, int& pos, double& value)
* @brief      Read number : read one floating point number from the current position
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  source : source string
* @param[in]  pos : current position (in/out)
* @param[in]  value : parsed value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::ReadNumber(XSTRING& source, int& pos, double& value)
{
  int len = (int)source.GetSize();

  while((pos < len) && IsSeparator(source[pos]))  pos++;
  if(pos >= len) return false;

  int  start = pos;
  bool dot   = false;

  if((source[pos] == __C('+')) || (source[pos] == __C('-')))  pos++;

  while(pos < len)
    {
      XCHAR c = source[pos];

      if((c >= __C('0')) && (c <= __C('9')))  { pos++; continue; }
      if((c == __C('.')) && !dot)             { dot = true; pos++; continue; }

      break;
    }

  if((pos < len) && ((source[pos] == __C('e')) || (source[pos] == __C('E'))))
    {
      pos++;
      if((pos < len) && ((source[pos] == __C('+')) || (source[pos] == __C('-'))))  pos++;
      while((pos < len) && (source[pos] >= __C('0')) && (source[pos] <= __C('9')))  pos++;
    }

  if(pos == start) return false;

  XSTRING token;
  source.Copy(start, pos, token);

  value = token.ConvertToDouble(0, NULL, false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::ReadFlag(XSTRING& source, int& pos, bool& flag)
* @brief      Read flag : read a single 0/1 arc flag
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  source : source string
* @param[in]  pos : current position (in/out)
* @param[in]  flag : parsed flag
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::ReadFlag(XSTRING& source, int& pos, bool& flag)
{
  int len = (int)source.GetSize();

  while((pos < len) && IsSeparator(source[pos]))  pos++;
  if(pos >= len) return false;

  XCHAR c = source[pos];

  if(c == __C('0'))       { flag = false;  pos++;  return true; }
  if(c == __C('1'))       { flag = true;   pos++;  return true; }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::IsSeparator(XCHAR character)
* @brief      Is separator : space, comma, tab, new line
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  character : character to test
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::IsSeparator(XCHAR character)
{
  return ((character == __C(' '))  || (character == __C(',')) || (character == __C('\t')) ||
          (character == __C('\n')) || (character == __C('\r')));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJPATH::IsCommand(XCHAR character)
* @brief      Is command : an alphabetic path command letter
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  character : character to test
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJPATH::IsCommand(XCHAR character)
{
  return (((character >= __C('A')) && (character <= __C('Z'))) ||
          ((character >= __C('a')) && (character <= __C('z'))));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJPATH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJPATH::Clean()
{
  pathdata.DeleteAll();
}
