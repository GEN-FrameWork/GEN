/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSectionEntities.cpp
* 
* @class      GRPVECTORFILEDXFTEXTSECTIONENTITIES
* @brief      Graphic Vector File DXF Entity Text Section Entities class
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

#include "GRPVectorFileDXFTextSectionEntities.h"

#include "XMap.h"
#include "XVariant.h"
#include "XFileTXT.h"

#include "GRPVectorFile_XEvent.h"
#include "GRPVectorFileDXF.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


GRPVECTORFILEDXFTEXTSECTIONENTITYDEF GRPVECTORFILEDXFTEXTSECTIONENTITIES::defentity[GRPVECTORFILEDXFENTITIES_MAXNDEFENTITIES] = 
{    
   { NL("3DFACE")            , 14 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbFace)") },
                                       {  10 , NL("CORNER1_X")                     , NL("First corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , NL("CORNER1_Y")                     , NL("DXF: Y value of first corner (in WCS)") },
                                       {  30 , NL("CORNER1_Z")                     , NL("DXF: Z value of first corner (in WCS)") },
                                       {  11 , NL("CORNER2_X")                     , NL("Second corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , NL("CORNER2_Y")                     , NL("DXF: Y value of second corner (in WCS)") },
                                       {  31 , NL("CORNER2_Z")                     , NL("DXF: Z value of second corner (in WCS)") },
                                       {  12 , NL("CORNER3_X")                     , NL("Third corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  22 , NL("CORNER3_Y")                     , NL("DXF: Y value of third corner (in WCS)") },
                                       {  32 , NL("CORNER3_Z")                     , NL("DXF: Z value of third corner (in WCS)") },
                                       {  13 , NL("CORNER4_X")                     , NL("Fourth corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  23 , NL("CORNER4_Y")                     , NL("DXF: Y value of fourth corner (in WCS)") },
                                       {  33 , NL("CORNER4_Z")                     , NL("DXF: Z value of fourth corner (in WCS)") },
                                       {  70 , NL("INVISIBLE_EDGE_FLAGS")          , NL("Invisible edge flags (optional; default = 0)") } } },
   { NL("3DSOLID")           ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("ACAD_PROXY_ENTITY") ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("ARC")               , 11 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbCircle)") },
                                       { 100 , VFDXF_SUBCLASS                      , NL("Subclass marker (AcDbArc)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_CENTER_POINT_X                , NL("Center point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                , NL("DXF: Y value of center point (in OCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                , NL("DXF: Z value of center point (in OCS)") },
                                       {  40 , VFDXF_RADIOUS                       , NL("Radius") },
                                       {  50 , VFDXF_INI_ANGLE                     , NL("Ini angle") },
                                       {  51 , VFDXF_END_ANGLE                     , NL("End angle") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("ATTDEF")            ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("ATTRIB")            ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("BODY")              ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("CIRCLE")            ,  9 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbCircle)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_CENTER_POINT_X                , NL("Center point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                , NL("DXF: Y value of center point (in OCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                , NL("DXF: Z value of center point (in OCS)") },
                                       {  40 , VFDXF_RADIOUS                       , NL("Radius") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("DIMENSION")         ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("ELLIPSE")           , 13 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbEllipse)") },
                                       {  10 , VFDXF_CENTER_POINT_X                , NL("Center point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                , NL("DXF: Y value of center point (in WCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                , NL("DXF: Z value of center point (in WCS)") },
                                       {  11 , VFDXF_END_POINT_X                   , NL("Endpoint of major axis, relative to the center (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , VFDXF_END_POINT_Y                   , NL("DXF: Y value of endpoint of major axis, relative to the center (in WCS)") },
                                       {  31 , VFDXF_END_POINT_Z                   , NL("DXF: Z value of endpoint of major axis, relative to the center (in WCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") },
                                       {  40 , NL("RATIO_MINOR_MAJOR_AXIS")        , NL("Ratio of minor axis to major axis") },
                                       {  41 , VFDXF_INI_PARAMETER                 , NL("Start parameter (this value is 0.0 for a full ellipse)") },
                                       {  42 , VFDXF_END_PARAMETER                 , NL("End parameter (this value is 2pi for a full ellipse)") } } },

   { NL("HATCH")            , 34 , { { 100 , VFDXF_SUBCLASS                        , NL("Subclass marker (AcDbHatch)") },
                                       {   2 , NL("HATCH_PATTERN_NAME")            , NL("Hatch pattern name") },
                                       {  70 , NL("HATCH_SOLID_FILL_FLAG")         , NL("Solid fill flag (0 = pattern fill; 1 = solid fill)") },
                                       {  71 , NL("HATCH_ASSOCIATIVITY_FLAG")      , NL("Associativity flag (0 = non-associative; 1 = associative)") },
                                       {  91 , NL("HATCH_NUMBER_PATHS")            , NL("Number of boundary paths (loops)") },
                                       {  92 , NL("HATCH_PATH_TYPE_FLAG")          , NL("Boundary path type flag (bit coded: 0 = default; 2 = polyline; 16 = outermost)") },
                                       {  93 , NL("HATCH_NUMBER_EDGES")            , NL("Number of edges in this boundary path (or vertices if polyline)") },
                                       {  72 , NL("HATCH_EDGE_TYPE")               , NL("Edge type (1 = line; 2 = circular arc; 3 = elliptic arc; 4 = spline) / polyline has-bulge flag") },
                                       {  73 , NL("HATCH_IS_CLOSED")               , NL("Is closed flag (polyline) / counterclockwise flag (arc) / rational (spline)") },
                                       {  74 , NL("HATCH_PERIODIC")                , NL("Periodic flag (spline edge)") },
                                       {  94 , NL("HATCH_DEGREE")                  , NL("Degree of the spline edge") },
                                       {  95 , NL("HATCH_NUMBER_KNOTS")            , NL("Number of knots (spline edge)") },
                                       {  96 , NL("HATCH_NUMBER_CONTROL_POINTS")   , NL("Number of control points (spline edge)") },
                                       {  97 , NL("HATCH_NUMBER_SOURCE")           , NL("Number of source boundary objects / fit points") },
                                       {  98 , NL("HATCH_NUMBER_SEED")             , NL("Number of seed points") },
                                       {  10 , VFDXF_POINT_X                       , NL("DXF: X value of a point (vertex / center / start / control point)") },
                                       {  20 , VFDXF_POINT_Y                       , NL("DXF: Y value of a point") },
                                       {  30 , VFDXF_POINT_Z                       , NL("DXF: Z value (elevation)") },
                                       {  11 , NL("HATCH_POINT2_X")                , NL("DXF: X value of a second point (line end / ellipse major axis endpoint)") },
                                       {  21 , NL("HATCH_POINT2_Y")                , NL("DXF: Y value of a second point") },
                                       {  40 , NL("HATCH_RADIUS")                  , NL("Radius (arc) / minor-major ratio (ellipse) / knot value (spline) / pattern scale") },
                                       {  41 , NL("HATCH_PATTERN_SCALE")           , NL("Pattern scale or spacing") },
                                       {  42 , NL("HATCH_BULGE")                   , NL("Bulge (polyline vertex) / weight (rational spline)") },
                                       {  50 , NL("HATCH_START_ANGLE")             , NL("Start angle (arc / ellipse edge)") },
                                       {  51 , NL("HATCH_END_ANGLE")               , NL("End angle (arc / ellipse edge)") },
                                       {  52 , NL("HATCH_PATTERN_ANGLE")           , NL("Hatch pattern angle") },
                                       {  75 , NL("HATCH_STYLE")                   , NL("Hatch style (0 = normal/odd parity; 1 = outer; 2 = ignore)") },
                                       {  76 , NL("HATCH_PATTERN_TYPE")            , NL("Hatch pattern type") },
                                       {  78 , NL("HATCH_NUMBER_PATTERN_LINES")    , NL("Number of pattern definition lines") },
                                       {  47 , NL("HATCH_PIXEL_SIZE")              , NL("Pixel size") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction") },
                                       { 330 , NL("HATCH_SOURCE_HANDLE")           , NL("Source boundary object handle (soft pointer)") } } },
   { NL("HELIX")             ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("IMAGE")             ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("INSERT")            , 17 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbBlockReference)") },
                                       {  66 , NL("VARIABLE_ATTRIBUTES_FLAG")      , NL("Variable attributes-follow flag (optional; default = 0); if the value of attributes-follow flag is 1, a series of attribute entities is expected to follow the insert, terminated by a seqend entity") },
                                       {   2 , NL("BLOCK_NAME")                    , NL("Block name") },
                                       {  10 , VFDXF_INSERTION_POINT_X             , NL("Insertion point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INSERTION_POINT_Y             , NL("DXF: Y value of insertion point (in OCS)") },
                                       {  30 , VFDXF_INSERTION_POINT_Z             , NL("DXF: Z value of insertion point (in OCS)") },
                                       {  41 , NL("SCALE_FACTOR_X")                , NL("X scale factor (optional; default = 1)") },
                                       {  42 , NL("SCALE_FACTOR_Y")                , NL("Y scale factor (optional; default = 1)") },
                                       {  43 , NL("SCALE_FACTOR_Z")                , NL("Z scale factor (optional; default = 1)") },
                                       {  50 , NL("ROTATION_ANGLE")                , NL("Rotation angle (optional; default = 0)") },
                                       {  70 , NL("COLUMN_COUNT")                  , NL("Column count (optional; default = 1)") },
                                       {  71 , NL("ROW_COUNT")                     , NL("Row count (optional; default = 1)") },
                                       {  44 , NL("COLUMN_SPACING")                , NL("Column spacing (optional; default = 0)") },
                                       {  45 , NL("ROW_SPACING")                   , NL("Row spacing (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },


   { NL("LEADER")            ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("LIGHT")             ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("LINE")              , 11 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbLine)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_INI_POINT_X                   , NL("Start point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INI_POINT_Y                   , NL("DXF: Y value of start point (in WCS)") },
                                       {  30 , VFDXF_INI_POINT_Z                   , NL("DXF: Z value of start point (in WCS)") },
                                       {  11 , VFDXF_END_POINT_X                   , NL("Endpoint (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , VFDXF_END_POINT_Y                   , NL("DXF: Y value of endpoint (in WCS)") },
                                       {  31 , VFDXF_END_POINT_Z                   , NL("DXF: Z value of endpoint (in WCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("LWPOLYLINE")        , 15 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbPolyline)") },
                                       {  90 , NL("NVERTICES")                     , NL("Number of vertices") },
                                       {  70 , NL("POLYLINE_FLAG")                 , NL("Polyline flag (bit-coded); default is 0: 1 = Closed; 128 = Plinegen") },
                                       {  43 , NL("CONSTANT_WIDTH")                , NL("Constant width (optional; default = 0). Not used if variable width (codes 40 and/or 41) is set") },
                                       {  38 , NL("ELEVATION")                     , NL("Elevation (optional; default = 0)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , NL("VERTEX_X")                      , NL("Vertex coordinates (in OCS), multiple entries; one entry for each vertex DXF: X value; APP: 2D point") },
                                       {  20 , NL("VERTEX_Y")                      , NL("DXF: Y value of vertex coordinates (in OCS), multiple entries; one entry for each vertex") },
                                       {  91 , NL("VERTEX_ID")                     , NL("Vertex identifier") },
                                       {  40 , NL("INI_WIDTH")                     , NL("Starting width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set") },
                                       {  41 , NL("END_WIDTH")                     , NL("End width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set") },
                                       {  42 , NL("BULGLE")                        , NL("Bulge (multiple entries; one entry for each vertex) (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("MESH")              ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("MLEADER")           ,  1 , { {   0 , NL(""), NL("") } } },   
   { NL("MLEADERSTYLE")      ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("MLINE")             ,  1 , { {   0 , NL(""), NL("") } } },
   
   { NL("MTEXT")             , 51 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbMText)") },
                                       {  10 , VFDXF_INSERTION_POINT_X             , NL("Insertion point DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INSERTION_POINT_Y             , NL("DXF: Y and Z values of insertion point") },
                                       {  30 , VFDXF_INSERTION_POINT_Z             , NL("DXF: Y and Z values of insertion point") },
                                       {  40 , NL("NOMINAL_TEXT_HEIGHT")           , NL("Nominal (initial) text height") },
                                       {  41 , NL("RECTANGLE_WIDTH")               , NL("Reference rectangle width") },
                                       {  71 , NL("ATTACHMENT_POINT")              , NL("Attachment point: 1 = Top left, 2 = Top center, 3 = Top right, 4 = Middle left, 5 = Middle center, 6 = Middle right, 7 = Bottom left, 8 = Bottom center, 9 = Bottom right.") },
                                       {  72 , NL("DRAWING_DIRECTION")             , NL("Drawing direction: 1 = Left to right, 3 = Top to bottom, 5 = By style (the flow direction is inherited from the associated text style).") },
                                       {   1 , NL("TEXT_STRING")                   , NL("Text string. If the text string is less than 250 characters, all characters appear in group 1. If the text string is greater than 250 characters, the string is divided into 250-character chunks, which appear in one or more group 3 codes. If group 3 codes are used, the last group is a group 1 and has fewer than 250 characters") },
                                       {   3 , NL("ADDITIONAL_TEXT_STRING")        , NL("Additional text (always in 250-character chunks) (optional)") },
                                       {   7 , NL("TEXT_STYLE_NAME")               , NL("Text style name (STANDARD if not provided) (optional)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") },
                                       {  11 , NL("DIRECTION_VECTOR_X")            , NL("X-axis direction vector (in WCS) DXF: X value; APP: 3D vector A group code 50 (rotation angle in radians) passed as DXF input is converted to the equivalent direction vector (if both a code 50 and codes 11, 21, 31 are passed, the last one wins). This is provided as a convenience for conversions from text objects") },
                                       {  21 , NL("DIRECTION_VECTOR_Y")            , NL("DXF: Y value of X-axis direction vector (in WCS)") },  
                                       {  31 , NL("DIRECTION_VECTOR_Z")            , NL("DXF: Z value of X-axis direction vector (in WCS)") },
                                       {  42 , NL("HORIZONTAL_WIDTH")              , NL("Horizontal width of the characters that make up the mtext entity. This value will always be equal to or less than the value of group code 41 (read-only, ignored if supplied)") },
                                       {  43 , NL("VERTICAL_HEIGHT")               , NL("Vertical height of the mtext entity (read-only, ignored if supplied)") },
                                       {  50 , NL("ROTATION_ANGLE")                , NL("Rotation angle in radians") },
                                       {  73 , NL("LINE_SPACING_STYLE")            , NL("Mtext line spacing style (optional): 1 = At least (taller characters will override), 2 = Exact (taller characters will not override).") },
                                       {  44 , NL("LINE_SPACING_FACTOR")           , NL("Mtext line spacing factor (optional): Percentage of default (3-on-5) line spacing to be applied. Valid values range from 0.25 to 4.00") },
                                       {  90 , NL("BACKGROUND_FILL_SETTING")       , NL("Background fill setting: 0 = Background fill off, 1 = Use background fill color, 2 = Use drawing window color as background fill color.") },
                                       { 420 , NL("BACKGROUND_COLOR_00")           , NL("Background color (if RGB color)") },
                                       { 421 , NL("BACKGROUND_COLOR_01")           , NL("Background color (if RGB color)") },
                                       { 422 , NL("BACKGROUND_COLOR_02")           , NL("Background color (if RGB color)") },  
                                       { 423 , NL("BACKGROUND_COLOR_03")           , NL("Background color (if RGB color)") },
                                       { 424 , NL("BACKGROUND_COLOR_04")           , NL("Background color (if RGB color)") },
                                       { 425 , NL("BACKGROUND_COLOR_05")           , NL("Background color (if RGB color)") },
                                       { 426 , NL("BACKGROUND_COLOR_06")           , NL("Background color (if RGB color)") },
                                       { 427 , NL("BACKGROUND_COLOR_07")           , NL("Background color (if RGB color)") },
                                       { 428 , NL("BACKGROUND_COLOR_08")           , NL("Background color (if RGB color)") },
                                       { 429 , NL("BACKGROUND_COLOR_09")           , NL("Background color (if RGB color)") },
                                       { 430 , NL("BACKGROUND_COLOR_10")           , NL("Background color (if RGB color)") }, 
                                       { 431 , NL("BACKGROUND_COLOR_11")           , NL("Background color (if RGB color)") },
                                       { 432 , NL("BACKGROUND_COLOR_12")           , NL("Background color (if RGB color)") },  
                                       { 433 , NL("BACKGROUND_COLOR_13")           , NL("Background color (if RGB color)") },
                                       { 434 , NL("BACKGROUND_COLOR_14")           , NL("Background color (if RGB color)") },
                                       { 435 , NL("BACKGROUND_COLOR_15")           , NL("Background color (if RGB color)") },
                                       { 436 , NL("BACKGROUND_COLOR_16")           , NL("Background color (if RGB color)") },
                                       { 437 , NL("BACKGROUND_COLOR_17")           , NL("Background color (if RGB color)") },
                                       { 438 , NL("BACKGROUND_COLOR_18")           , NL("Background color (if RGB color)") },
                                       { 439 , NL("BACKGROUND_COLOR_19")           , NL("Background color (if RGB color)") },
                                       {  45 , NL("FILL_BOX_SCALE")                , NL("Fill box scale (optional): Determines how much border there is around the text.") },
                                       {  63 , NL("BACKGROUND_FILL_COLOR")         , NL("Background fill color (optional): Color to use for background fill when group code 90 is 1.") },
                                       { 441 , NL("TRANSPARENCY_BACKGROUND_COLOR") , NL("Transparency of background fill color (not implemented)") },
                                       {  75 , NL("COLUMN_TYPE")                   , NL("Column type") },
                                       {  76 , NL("COLUMN_COUNT")                  , NL("Column count") },
                                       {  78 , NL("COLUMN_FLOW REVERSED")          , NL("Column Flow Reversed") },
                                       {  79 , NL("COLUMN_AUTOHEIGHT")             , NL("Column Autoheight") },
                                       {  48 , NL("COLUMN_WIDTH")                  , NL("Column width") },
                                       {  49 , NL("COLUMN_GUTTER")                 , NL("Column gutter") },
                                       {  50 , NL("COLUMN_HEIGHTS")                , NL("Column heights; this code is followed by a column count (Int16), and then the number of column heights.") } } },

   { NL("MULTILEADER")       ,  1 , { {   0 , NL(""), NL("") } } },                                          
   { NL("OLEFRAME")          ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("OLE2FRAME")         ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("POINT")             ,  9 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbPoint)") },
                                       {  10 , VFDXF_POINT_X                       , NL("Point location (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_POINT_Y                       , NL("DXF: Y value of point location (in WCS)") },
                                       {  30 , VFDXF_POINT_Z                       , NL("DXF: Z value of point location (in WCS)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") },
                                       {  50 , VFDXF_ANGLE_X                       , NL("Angle of the X axis for the UCS in effect when the point was drawn (optional, default = 0); used when PDMODE is nonzero") } } },

   { NL("POLYLINE")          , 17 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker AcDb2dPolyline") },
                                       { 100 , VFDXF_SUBCLASS                      , NL("Subclass marker AcDb3dPolyline") },
                                       {  66 , NL("OBSOLETE")                      , NL("Obsolete; formerly an 'entities follow flag' (optional; ignore if present)") },
                                       {  10 , NL("ALWAYS_0_APP")                  , NL("DXF: always 0 APP: a 'dummy' point; the X and Y values are always 0, and the Z value is the polyline's elevation (in OCS when 2D, WCS when 3D)") },
                                       {  20 , NL("ALWAYS_0")                      , NL("DXF: always 0") },
                                       {  30 , NL("POLYLINE_ELEVATION")            , NL("DXF: polyline's elevation (in OCS when 2D; WCS when 3D)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  70 , NL("POLYLINE_FLAG")                 , NL("Polyline flag (bit-coded; default = 0): 1 = This is a closed polyline (or a polygon mesh closed in the M direction), 2 = Curve-fit vertices have been added, 4 = Spline-fit vertices have been added, 8 = This is a 3D polyline, 16 = This is a 3D polygon mesh, 32 = The polygon mesh is closed in the N direction, 64 = The polyline is a polyface mesh. 128 = The linetype pattern is generated continuously around the vertices of this polyline.") },
                                       {  40 , NL("DEFAULT_START WIDTH")           , NL("Default start width (optional; default = 0)") },
                                       {  41 , NL("DEFAULT_END_WIDTH")             , NL("Default end width (optional; default = 0)") },
                                       {  71 , NL("POLYGON_MESH_M_VERTEX_COUNT")   , NL("Polygon mesh M vertex count (optional; default = 0)") },
                                       {  73 , NL("SMOOTH_SURFACE_M_DENSITY")      , NL("Smooth surface M density (optional; default = 0)") },
                                       {  74 , NL("SMOOTH_SURFACE_N_DENSITY")      , NL("Smooth surface N density (optional; default = 0)") },
                                       {  75 , NL("CURVES_SMOOTH_SURFACE_TYPE")    , NL("Curves and smooth surface type (optional; default = 0); integer codes, not bit-coded: 0 = No smooth surface fitted, 5 = Quadratic B-spline surface, 6 = Cubic B-spline surface, 8 = Bezier surface.") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("RAY")               ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("REGION")            ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("SECTION")           ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("SEQEND")            ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("SHAPE")             ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("SOLID")             , 17 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbTrace)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , NL("CORNER1_X")                     , NL("First corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , NL("CORNER1_Y")                     , NL("DXF: Y value of first corner (in OCS)") },
                                       {  30 , NL("CORNER1_Z")                     , NL("DXF: Z value of first corner (in OCS)") },
                                       {  11 , NL("CORNER2_X")                     , NL("Second corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  21 , NL("CORNER2_Y")                     , NL("DXF: Y value of second corner (in OCS)") },
                                       {  31 , NL("CORNER2_Z")                     , NL("DXF: Z value of second corner (in OCS)") },
                                       {  12 , NL("CORNER3_X")                     , NL("Third corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  22 , NL("CORNER3_Y")                     , NL("DXF: Y value of third corner (in OCS)") },
                                       {  32 , NL("CORNER3_Z")                     , NL("DXF: Z value of third corner (in OCS)") },
                                       {  13 , NL("CORNER4_X")                     , NL("Fourth corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  23 , NL("CORNER4_Y")                     , NL("DXF: Y value of fourth corner (in OCS)") },
                                       {  33 , NL("CORNER4_Z")                     , NL("DXF: Z value of fourth corner (in OCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },

   { NL("SPLINE")            , 26 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbSpline)") },
                                       { 210 , NL("VECTOR_X")                      , NL("Normal vector (omitted if the spline is nonplanar) DXF: X value; APP: 3D vector") },
                                       { 220 , NL("VECTOR_Y")                      , NL("DXF: Y value of normal vector (optional)") },
                                       { 230 , NL("VECTOR_Z")                      , NL("DXF: Z value of normal vector (optional)") },
                                       {  70 , NL("SPLINE_FLAG")                   , NL("Spline flag (bit coded): 1 = Closed spline, 2 = Periodic spline, 4 = Rational spline, 8 = Planar, 16 = Linear (planar bit is also set)") },
                                       {  71 , NL("DEGREE")                        , NL("Degree of the spline curve") },
                                       {  72 , NL("NKNOTS")                        , NL("Number of knots") },
                                       {  73 , NL("NCTRL_POINTS")                  , NL("Number of control points") },
                                       {  74 , NL("NFIT_POINTS")                   , NL("Number of fit points (if any)") },
                                       {  42 , NL("KNOT_TOLERANCE")                , NL("Knot tolerance (default = 0.0000001)") },
                                       {  43 , NL("CTRLPOINT_TOLERANCE")           , NL("Control-point tolerance (default = 0.0000001)") },
                                       {  44 , NL("FIT_TOLERANCE")                 , NL("Fit tolerance (default = 0.0000000001)") },
                                       {  12 , NL("INI_TANGENT_X")                 , NL("Start tangent-may be omitted (in WCS) DXF: X value; APP: 3D point") },
                                       {  22 , NL("INI_TANGENT_Y")                 , NL("DXF: Y value of start tangent-may be omitted (in WCS)") },
                                       {  32 , NL("INI_TANGENT_Z")                 , NL("DXF: Z value of start tangent-may be omitted (in WCS)") },
                                       {  13 , NL("END_TANGENT_X")                 , NL("End tangent-may be omitted (in WCS) DXF: X value; APP: 3D point") },
                                       {  23 , NL("END_TANGENT_Y")                 , NL("DXF: Y value of end tangent-may be omitted (in WCS)") },
                                       {  33 , NL("END_TANGENT_Z")                 , NL("DXF: Z value of end tangent-may be omitted (in WCS)") },
                                       {  40 , NL("KNOT_VALUE")                    , NL("Knot value (one entry per knot)") },
                                       {  41 , NL("WEIGHT")                        , NL("Weight (if not 1); with multiple group pairs, they are present if all are not 1") },
                                       {  10 , NL("CTRL_POINT_X")                  , NL("Control points (in WCS); one entry per control point DXF: X value; APP: 3D point") },
                                       {  20 , NL("CTRL_POINT_Y")                  , NL("DXF: Y value of control points (in WCS); one entry per control point") },
                                       {  30 , NL("CTRL_POINT_Z")                  , NL("DXF: Z value of control points (in WCS); one entry per control point") },
                                       {  11 , NL("FIT_POINT_X")                   , NL("Fit points (in WCS); one entry per fit point DXF: X value; APP: 3D point") },
                                       {  21 , NL("FIT_POINT_Y")                   , NL("DXF: Y value of fit points (in WCS); one entry per fit point") },
                                       {  31 , NL("FIT_POINT_Z")                   , NL("DXF: Z value of fit points (in WCS); one entry per fit point") } } },

   { NL("SUN")               ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("SURFACE")           ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("TABLE")             ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("TEXT")              , 21 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbText)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , NL("ALIGNMENT_POINT_X")             , NL("First alignment point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , NL("ALIGNMENT_POINT_Y")             , NL("DXF: Y value of first alignment point (in OCS)") },
                                       {  30 , NL("ALIGNMENT_POINT_Z")             , NL("DXF: Z value of first alignment point (in OCS)") },
                                       {  40 , NL("TEXT_HEIGHT")                   , NL("Text height") },
                                       {   1 , NL("DEFAULT_VALUE")                 , NL("Default value (the string itself)") },
                                       {  50 , NL("TEXT_ROTATION")                 , NL("Text rotation (optional; default = 0)") },
                                       {  41 , NL("RELATIVE_X_SCALE_WIDTH")        , NL("Relative X scale factor-width (optional; default = 1) This value is also adjusted when fit-type text is used") },
                                       {  51 , NL("OBLIQUE_ANGLE")                 , NL("Oblique angle (optional; default = 0)") },
                                       {   7 , NL("TEXT_STYLE_NAME")               , NL("Text style name (optional, default = STANDARD)") },
                                       {  71 , NL("TEXT GENERATION FLAGS")         , NL("Text generation flags (optional, default = 0): 2 = Text is backward (mirrored in X), 4 = Text is upside down (mirrored in Y)") },
                                       {  72 , NL("HORIZONTAL_JUSTIFICATION_TYPE") , NL("Horizontal text justification type (optional, default = 0) integer codes (not bit-coded): 0 = Left, 1= Center, 2 = Right, 3 = Aligned (if vertical alignment = 0), 4 = Middle (if vertical alignment = 0), 5 = Fit (if vertical alignment = 0), See the Group 72 and 73 integer codes table for clarification.") },
                                       {  11 , NL("SECOND_ALIGNMENT_POINT_X")      , NL("Second alignment point (in OCS) (optional) DXF: X value; APP: 3D point This value is meaningful only if the value of a 72 or 73 group is nonzero (if the justification is anything other than baseline/left)") },
                                       {  21 , NL("SECOND_ALIGNMENT_POINT_Y")      , NL("DXF: Y value of second alignment point (in OCS) (optional)") },
                                       {  31 , NL("SECOND_ALIGNMENT_POINT_Z")      , NL("DXF: Z value of second alignment point (in OCS) (optional)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") },
                                       {  73 , NL("VERTICAL_JUSTIFICATION_TYPE")   , NL("Vertical text justification type (optional, default = 0): integer codes (not bit-coded): 0 = Baseline, 1 = Bottom, 2 = Middle, 3 = Top. See the Group 72 and 73 integer codes table for clarification.") } } },

   { NL("TOLERANCE")         ,  1 , { {   0 , NL(""), NL("") } } },
   { NL("TRACE")             , 17 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbTrace)") },
                                       {  39 , VFDXF_THICKNESS                     , NL("Thickness (optional; default = 0)") },
                                       {  10 , NL("CORNER1_X")                     , NL("First corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , NL("CORNER1_Y")                     , NL("DXF: Y value of first corner (in OCS)") },
                                       {  30 , NL("CORNER1_Z")                     , NL("DXF: Z value of first corner (in OCS)") },
                                       {  11 , NL("CORNER2_X")                     , NL("Second corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  21 , NL("CORNER2_Y")                     , NL("DXF: Y value of second corner (in OCS)") },
                                       {  31 , NL("CORNER2_Z")                     , NL("DXF: Z value of second corner (in OCS)") },
                                       {  12 , NL("CORNER3_X")                     , NL("Third corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  22 , NL("CORNER3_Y")                     , NL("DXF: Y value of third corner (in OCS)") },
                                       {  32 , NL("CORNER3_Z")                     , NL("DXF: Z value of third corner (in OCS)") },
                                       {  13 , NL("CORNER4_X")                     , NL("Fourth corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  23 , NL("CORNER4_Y")                     , NL("DXF: Y value of fourth corner (in OCS)") },
                                       {  33 , NL("CORNER4_Z")                     , NL("DXF: Z value of fourth corner (in OCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X         , NL("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y         , NL("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z         , NL("DXF: Z value of extrusion direction (optional)") } } },
   { NL("UNDERLAY")          ,  1 , { {   0 , NL(""), NL("") } } },

   { NL("VERTEX")            , 15 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbVertex)") },
                                       { 100 , VFDXF_SUBCLASS                      , NL("Subclass marker (AcDb2dVertex or AcDb3dPolylineVertex)") },
                                       {  10 , VFDXF_LOCATION_POINT_X              , NL("Location point (in OCS when 2D, and WCS when 3D) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_LOCATION_POINT_Y              , NL("DXF: Y and Z values of location point (in OCS when 2D, and WCS when 3D)") },
                                       {  30 , VFDXF_LOCATION_POINT_Z              , NL("DXF: Y and Z values of location point (in OCS when 2D, and WCS when 3D)") },
                                       {  40 , NL("STARTING_WIDTH")                , NL("Starting width (optional; default is 0)") },
                                       {  41 , NL("ENDING_WIDTH")                  , NL("Ending width (optional; default is 0)") },
                                       {  42 , NL("BULGE")                         , NL("Bulge (optional; default is 0). The bulge is the tangent of one fourth the included angle for an arc segment, made negative if the arc goes clockwise from the start point to the endpoint. A bulge of 0 indicates a straight segment, and a bulge of 1 is a semicircle") },
                                       {  70 , NL("VERTEX_FLAGS")                  , NL("Vertex flags: 1 = Extra vertex created by curve-fitting, 2 = Curve-fit tangent defined for this vertex. A curve-fit tangent direction of 0 may be omitted from DXF output but is significant if this bit is set, 4 = Not used, 8 = Spline vertex created by spline-fitting, 16 = Spline frame control point, 32 = 3D polyline vertex, 64 = 3D polygon mesh, 128 = Polyface mesh vertex.") },
                                       {  50 , NL("CURVE_FIT_TANGENT_DIRECTION")   , NL("Curve fit tangent direction") },
                                       {  71 , NL("POLYFACE_MESH_VERTEX_INDEX_1")  , NL("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  72 , NL("POLYFACE_MESH_VERTEX_INDEX_2")  , NL("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  73 , NL("POLYFACE_MESH_VERTEX_INDEX_3")  , NL("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  74 , NL("POLYFACE_MESH_VERTEX_INDEX_4")  , NL("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  91 , NL("VERTEX_IDENTIFIER")             , NL("Vertex identifier") } } },

   { NL("VIEWPORT")          , 85 , { { 100 , VFDXF_SUBCLASS                       , NL("Subclass marker (AcDbViewport)") },                                                
                                       {  10 , VFDXF_CENTER_POINT_X                , NL("Center point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                , NL("DXF: Y value of center point (in WCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                , NL("DXF: Z value of center point (in WCS)") },
                                       {  40 , NL("WIDTH_PAPER")                   , NL("Width in paper space units") },
                                       {  41 , NL("HEIGHT_PAPER")                  , NL("Height in paper space units") },
                                       {  68 , NL("VIEWPORT_STATUS_FIELD")         , NL("Viewport status field: -1 = On, but is fully off screen, or is one of the viewports that is not active because the $MAXACTVP count is currently being exceeded. 0 = Off <positive value > = On and active. The value indicates the order of stacking for the viewports, where 1 is the active viewport, 2 is the next, and so forth") },
                                       {  69 , NL("VIEWPORT_ID")                   , NL("Viewport ID") },
                                       {  12 , NL("VIEW_CENTER_POINT_X")           , NL("View center point (in DCS) DXF: X value; APP: 2D point") },
                                       {  22 , NL("VIEW_CENTER_POINT_Y")           , NL("DXF: View center point Y value (in DCS)") },
                                       {  13 , NL("SNAP_BASE_POINT_X")             , NL("Snap base point DXF: X value; APP: 2D point") },
                                       {  23 , NL("SNAP_BASE_POINT_Y")             , NL("DXF: Snap base point Y value") },
                                       {  14 , NL("SNAP_SPACING_X")                , NL("Snap spacing DXF: X value; APP: 2D point") },
                                       {  24 , NL("SNAP_SPACING_Y")                , NL("DXF: Snap spacing Y value") },
                                       {  15 , NL("GRID_SPACING_X")                , NL("Grid spacing DXF: X value; APP: 2D point") },
                                       {  25 , NL("GRID_SPACING_Y")                , NL("DXF: Grid spacing Y value") },
                                       {  16 , NL("VIEW_DIRECTION_VECTOR_X")       , NL("View direction vector (in WCS) DXF: X value; APP: 3D vector") },
                                       {  26 , NL("VIEW_DIRECTION_VECTOR_Y")       , NL("DXF: Y value of view direction vector (in WCS)") },
                                       {  36 , NL("VIEW_DIRECTION_VECTOR_Z")       , NL("DXF: Z value of view direction vector (in WCS)") },
                                       {  17 , NL("VIEW_TARGET_POINT_X")           , NL("View target point (in WCS) DXF: X value; APP: 3D vector") },
                                       {  27 , NL("VIEW_TARGET_POINT_Y")           , NL("DXF: Y value of view target point (in WCS)") },
                                       {  37 , NL("VIEW_TARGET_POINT_Z")           , NL("DXF: Z value of view target point (in WCS)") },
                                       {  42 , NL("PERSPECTIVE_LENS_LENGTH")       , NL("Perspective lens length") },
                                       {  43 , NL("FRONT_CLIP_PLANE_Z")            , NL("Front clip plane Z value") },
                                       {  44 , NL("BACK_CLIP_PLANE_Z")             , NL("Back clip plane Z value") },
                                       {  45 , NL("VIEW_HEIGHT")                   , NL("View height (in model space units)") },
                                       {  50 , NL("SNAP_ANGLE")                    , NL("Snap angle") },
                                       {  51 , NL("VIEW_TWIST_ANGLE")              , NL("View twist angle") },
                                       {  72 , NL("CIRCLE_ZOOM_PERCENT")           , NL("Circle zoom percent") },
                                       { 331 , NL("FROZEN_LAYER_ID-HDL")           , NL("Frozen layer object ID-handle (multiple entries may exist) (optional)") },
                                       {  90 , NL("VIEWPORT_STATUS")               , NL("Viewport status bit-coded flags: 1 (0x1) = Enables perspective mode,"
                                                                                                                           "2 (0x2) = Enables front clipping,"
                                                                                                                           "4 (0x4) = Enables back clipping,"
                                                                                                                           "8 (0x8) = Enables UCS follow,"
                                                                                                                           "16 (0x10) = Enables front clip not at eye,"
                                                                                                                           "32 (0x20) = Enables UCS icon visibility,"
                                                                                                                           "64 (0x40) = Enables UCS icon at origin,"
                                                                                                                           "128 (0x80) = Enables fast zoom,"
                                                                                                                           "256 (0x100) = Enables snap mode,"
                                                                                                                           "512 (0x200) = Enables grid mode,"
                                                                                                                           "1024 (0x400) = Enables isometric snap style,"
                                                                                                                           "2048 (0x800) = Enables hide plot mode,"
                                                                                                                           "4096 (0x1000) = kIsoPairTop. If set and kIsoPairRight is not set, then isopair top is enabled. If both kIsoPairTop and kIsoPairRight are set, then isopair left is enabled, "
                                                                                                                           "8192 (0x2000) = kIsoPairRight. If set and kIsoPairTop is not set, then isopair right is enabled,"
                                                                                                                           "16384 (0x4000) = Enables viewport zoom locking, 32768 (0x8000) = Currently always enabled,"
                                                                                                                           "65536 (0x10000) = Enables non-rectangular clipping,"
                                                                                                                           "131072 (0x20000) = Turns the viewport off,"
                                                                                                                           "262144 (0x40000) = Enables the display of the grid beyond the drawing limits,"
                                                                                                                           "524288 (0x80000) = Enable adaptive grid display,"
                                                                                                                           "1048576 (0x100000) = Enables subdivision of the grid below the set grid spacing when the grid display is adaptive,"
                                                                                                                           "2097152 (0x200000) = Enables grid follows workplane switching.") },                                        
                                       { 340 , NL("HARD-POINTER_ID-HDL")           , NL("Hard-pointer ID-handle to entity that serves as the viewport's clipping boundary (only present if viewport is non-rectangular)") },
                                       {   1 , NL("PLOT_STYLE_SHEET_NAME")         , NL("Plot style sheet name assigned to this viewport") },
                                       { 281 , NL("RENDER_MODE")                   , NL("Render mode: 0 = 2D Optimized (classic 2D), 1 = Wireframe, 2 = Hidden line, 3 = Flat shaded, 4 = Gouraud shaded, 5 = Flat shaded with wireframe, 6 = Gouraud shaded with wireframe, All rendering modes other than 2D Optimized engage the GEN_NEW 3D graphics pipeline. These values directly correspond to the SHADEMODE command and the AcDbAbstractViewTableRecord::RenderMode enum.") },
                                       {  71 , NL("UCS_VIEWPORT_FLAG")             , NL("UCS per viewport flag: 0 = The UCS will not change when this viewport becomes active. 1 = This viewport stores its own UCS which will become the current UCS whenever the viewport is activated") },
                                       {  74 , NL("DISPLAY_UCS_ICON")              , NL("Display UCS icon at UCS origin flag: Controls whether UCS icon represents viewport UCS or current UCS (these will be different if UCSVP is 1 and viewport is not active). However, this field is currently being ignored and the icon always represents the viewport UCS") },
                                       { 110 , NL("UCS_ORIGIN_X")                  , NL("UCS origin DXF: X value; APP: 3D point") },
                                       { 120 , NL("UCS_ORIGIN_Y")                  , NL("DXF: Y value of UCS origin") },
                                       { 130 , NL("UCS_ORIGIN_Z")                  , NL("DXF: Z value of UCS origin") },
                                       { 111 , NL("UCS_X-AXIS_X")                  , NL("UCS X-axis DXF: X value; APP: 3D vector") },
                                       { 121 , NL("UCS_X-AXIS_Y")                  , NL("DXF: Y value of UCS X-axis") },
                                       { 131 , NL("UCS_X-AXIS_Z")                  , NL("DXF: Z value of UCS X-axis") },
                                       { 112 , NL("UCS_Y-AXIS_X")                  , NL("UCS Y-axis DXF: X value; APP: 3D vector") },
                                       { 122 , NL("UCS_Y-AXIS_Y")                  , NL("DXF: Y value of UCS Y-axis") },
                                       { 132 , NL("UCS_Y-AXIS_Z")                  , NL("DXF: Z value of UCS Y-axis") },
                                       { 345 , NL("ID_HDL_ACDBUCSTABLE_UCS")       , NL("ID/handle of AcDbUCSTableRecord if UCS is a named UCS. If not present, then UCS is unnamed") },
                                       { 346 , NL("ID_HDL_ACDBUCSTABLE_BASE_UCS")  , NL("ID/handle of AcDbUCSTableRecord of base UCS if UCS is orthographic (79 code is non-zero). If not present and 79 code is non-zero, then base UCS is taken to be WORLD") },
                                       {  79 , NL("ORTHOGRAPHIC_TYPE")             , NL("Orthographic type of UCS: 0 = UCS is not orthographic, 1 = Top; 2 = Bottom, 3 = Front; 4 = Back, 5 = Left; 6 = Right.") },
                                       { 146 , NL("ELEVATION")                     , NL("Elevation") },
                                       { 170 , NL("SHADEPLOT_MODE")                , NL("ShadePlot mode: 0 = As Displayed, 1 = Wireframe, 2 = Hidden, 3 = Rendered.") },
                                       {  61 , NL("FREQUENCY_GRID_LINES")          , NL("Frequency of major grid lines compared to minor grid lines") },
                                       { 332 , NL("BACKGROUND_ID-HDL")             , NL("Background ID/Handle (optional)") },
                                       { 333 , NL("SHADE_PLOT_ID-HDL")             , NL("Shade plot ID/Handle (optional)") },
                                       { 348 , NL("VISUAL_STYLE_ID-HDL")           , NL("Visual style ID/Handle (optional)") },
                                       { 292 , NL("DEFAULT_LIGHTING_FLAG")         , NL("Default lighting flag. On when no user lights are specified.") },
                                       { 282 , NL("DEFAULT_LIGHTING_TYPE")         , NL("Default lighting type: 0 = One distant light, 1 = Two distant lights.") },
                                       { 141 , NL("VIEW_BRIGHTNESS")               , NL("View brightness") },
                                       { 142 , NL("VIEW_CONTRAST")                 , NL("View contrast") },
                                       {  63 , NL("AMBIENT_LIGHT_COLOR_63")        , NL("Ambient light color. Write only if not black color.") },
                                       { 421 , NL("AMBIENT_LIGHT_COLOR_421")       , NL("Ambient light color. Write only if not black color.") },
                                       { 431 , NL("AMBIENT_LIGHT_COLOR_431")       , NL("Ambient light color. Write only if not black color.") },
                                       { 361 , NL("SUN_ID-HDL")                    , NL("Sun ID/Handle (optional)") },
                                       { 335 , NL("SOFT_PTR_REF_335")              , NL("Soft pointer reference to viewport object (for layer VP property override)") },
                                       { 343 , NL("SOFT_PTR_REF_343")              , NL("Soft pointer reference to viewport object (for layer VP property override)") },
                                       { 344 , NL("SOFT_PTR_REF_344")              , NL("Soft pointer reference to viewport object (for layer VP property override)") },
                                       {  91 , NL("SOFT_PTR_REF_VIEWPORT")         , NL("Soft pointer reference to viewport object (for layer VP property override)") } } },

   { NL("WIPEOUT")           ,  1 , { { 0 , NL(""), NL("") } } },
   { NL("XLINE")             ,  1 , { { 0 , NL(""), NL("") } } },
}; 


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONENTITIES::GRPVECTORFILEDXFTEXTSECTIONENTITIES()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONENTITIES::GRPVECTORFILEDXFTEXTSECTIONENTITIES()
{
  Clean();

  type = GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONENTITIES::~GRPVECTORFILEDXFTEXTSECTIONENTITIES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONENTITIES::~GRPVECTORFILEDXFTEXTSECTIONENTITIES()
{
  DeleteAllEntities();
  DeleteAllEntitiesObj();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::IsKnownEntity(XSTRING& namevar)
* @brief      Is known entity
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::IsKnownEntity(XSTRING& namevar)
{  
  for(XDWORD c=0; c<GRPVECTORFILEDXFENTITIES_MAXNDEFENTITIES; c++)
    {
      GRPVECTORFILEDXFTEXTSECTIONENTITYDEF* entityDef  = &GRPVECTORFILEDXFTEXTSECTIONENTITIES::defentity[c];
      if(entityDef)
        {
          if(!namevar.Compare(entityDef->name, true)) 
            {
              return true;
            }    
        }
    }
  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE* GRPVECTORFILEDXFTEXTSECTIONENTITIES::IsKnownTypeValue(XSTRING& namevar, int type)
* @brief      Is known type value
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar value.
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE* GRPVECTORFILEDXFTEXTSECTIONENTITIES::IsKnownTypeValue(XSTRING& namevar, int type)
{  
  for(int c=0; c<GRPVECTORFILEDXFENTITIES_MAXNDEFENTITIES; c++)
    {
      GRPVECTORFILEDXFTEXTSECTIONENTITYDEF* entity  = &GRPVECTORFILEDXFTEXTSECTIONENTITIES::defentity[c];
      if(entity)
        {
          if(!namevar.Compare(entity->name, true)) 
            {
              for(int d=0; d<entity->ntypes; d++)
                {
                  GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE* typeDef = &entity->type[d]; 
                  if(typeDef)
                    {
                      if(typeDef->type == type) 
                        {
                          return typeDef;                  
                        }
                    }            
                }
            }    
        }
    }
  
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::AddEntity(GRPVECTORFILEDXFENTITY* entity)
* @brief      Add entity
* @ingroup    GRAPHIC
* 
* @param[in]  entity : Entity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::AddEntity(GRPVECTORFILEDXFENTITY* entity)
{ 
  if(!entity) 
    {
      return false;
    }

  if(entity->GetName()->IsEmpty()) 
    {
      return false;
    }
   
  if(entities.Add(entity))
    {
      AddEntityEnum(entity->GetName()->Get());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPVECTORFILEDXFENTITY*>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntities()
* @brief      Get entities
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<GRPVECTORFILEDXFENTITY*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVECTORFILEDXFENTITY*>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntities()
{
  return &entities;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITY* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntity (XCHAR* nameentity, XDWORD index)
* @brief      Get entity
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* @param[in]  index : Index value.
* 
* @return     GRPVECTORFILEDXFENTITY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITY* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntity (XCHAR* nameentity, XDWORD index)
{
  if(entities.IsEmpty()) 
    {
      return NULL;
    }

  int iindex = 0;

  for(XDWORD c=0; c<entities.GetSize(); c++)
    {
      GRPVECTORFILEDXFENTITY* entity = entities.Get(c);
      if(entity)
        {
          if(entity->GetName()->Find(nameentity, false) != XSTRING_NOTFOUND)
            {
              if(iindex == index)
                {              
                  return entity;
                }

              iindex ++;
            }
        }    
    }
  
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteEntity(XCHAR* nameentity, XDWORD index)
* @brief      Delete entity
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteEntity(XCHAR* nameentity, XDWORD index)
{
  if(entities.IsEmpty()) 
    {
      return false;
    }

  GRPVECTORFILEDXFENTITY* entity = GetEntity(nameentity, index);
  if(entity)
    { 
      entities.Delete(entity);
      GEN_DELETE entity;

      SubtractEntityEnum(entity->GetName()->Get());

      return true;
    }

  return false;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntities(XCHAR* nameentity)
* @brief      Delete all entities
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntities(XCHAR* nameentity)
{
  if(entities.IsEmpty()) 
    {
      return false;
    }

  GRPVECTORFILEDXFENTITY* entity;
  int index = 0;

  do{ entity = GetEntity (nameentity, index);
      if(entity)
        {
          DeleteEntity(nameentity, index);
        }
       else 
        {
          index++;
        }
   
    } while(entity);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntities()
* @brief      Delete all entities
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntities()
{
  if(entities.IsEmpty()) 
    {
      return false;
    }

  entities.DeleteContents();
  entities.DeleteAll();

  enumentities.DeleteKeyContents();
  enumentities.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMAP<XSTRING*, int>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEnumEntitys()
* @brief      Get enum entitys
* @ingroup    GRAPHIC
* 
* @return     XMAP<XSTRING*, int>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMAP<XSTRING*, int>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEnumEntitys()
{
  return &enumentities;
}
    
    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetNEntitys(XCHAR* nameentity)
* @brief      Get N entitys
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetNEntitys(XCHAR* nameentity)
{
  int index     = GetEntityEnumIndex(nameentity);
  int nentities = 0;

  if(index != GRPVECTORFILEDXFEntities_NotEnumEntity)    
    {
      nentities = enumentities.GetElement(index);
    }

  return nentities;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPVECTORFILEDXFENTITYOBJ*>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntitiesObj()
* @brief      Get entities obj
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<GRPVECTORFILEDXFENTITYOBJ*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVECTORFILEDXFENTITYOBJ*>* GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntitiesObj()
{
  return &entitiesObj;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntitiesObj()
* @brief      Delete all entities obj
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::DeleteAllEntitiesObj()
{
  if(entitiesObj.IsEmpty()) 
    {
      return false;
    }

  entitiesObj.DeleteContents();
  entitiesObj.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTIONENTITIES::ParserTextSection(XFILETXT* fileTXT)
* @brief      Parser text section
* @ingroup    GRAPHIC
* 
* @param[in]  fileTXT : File TXT pointer to use.
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTIONENTITIES::ParserTextSection(XFILETXT* fileTXT)
{
  XVECTOR<GRPVECTORFILEDXFTEXTPART*>  parts;
  GRPVECTORFILEDXFTEXTPART*           part      = NULL;  
  GRPVECTORFILEDXFXDATACTRL*          xdatactrl = NULL;
  int                                 indexline = iniline;
  XSTRING*                            line;

  enumentities.DeleteKeyContents();
  enumentities.DeleteAll();

  part = GEN_NEW GRPVECTORFILEDXFTEXTPART();

  do{ line = fileTXT->GetLine(indexline);                                       // code line of the (group code, value) DXF pair
      if(line) 
        {                       
          GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

          if(!line->Compare(NL("0"), true))                                     // an entity starts at group code 0 : tested only at a code position (we step by pairs), so a *value* of "0" can never be mistaken for a delimiter
            {
              line = fileTXT->GetLine(indexline + 1);                           // value line of the same pair : the entity type name
              if(line)
                {
                  GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

                  if(!line->IsNumber())
                    {
                      if(IsKnownEntity(*line))
                        {
                          if(part && (part->iniline != -1))
                            {               
                              part->endline = indexline-1;                      // previous entity ends just before this "0"
                                    
                              parts.Add(part); 
                              part = NULL;
                      
                              part = GEN_NEW GRPVECTORFILEDXFTEXTPART ();
                            }

                          if(part && (part->iniline == -1))
                            {                     
                              part->name    = line->Get();
                              part->iniline = indexline + 2;                    // entity body starts after the "0" / type pair
                            }                             
                        }    
                       else
                        {
                          XSTRING message;

                          message.Format(NL("entity %s Unknown"), line->Get());
                                   
                          GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

                          vfevent.SetType(GRPVECTORFILETYPE_DXF);
                          vfevent.GetPath()->Set(fileTXT->GetPrimaryFile()->GetPathNameFile());
                          vfevent.GetMsg()->Set(message);

                          PostEvent(&vfevent, GetGrpVectorFile());                   
                        }
                    }
                }
            } 
        }
                           
      indexline += 2;                                                           // advance a full (group code, value) pair : value lines are never scanned as delimiters

    } while(indexline < endline);

  if(part && (part->iniline != -1))
    {               
      part->endline = indexline-1; 
      
      parts.Add(part); 
      part = NULL;           
    }

  if(part && part->iniline == -1)
    {
      GEN_DELETE part;
      part = NULL;    
    }

  indexline = 0;

  for(XDWORD c=0; c<parts.GetSize(); c++)
    {
      part = parts.Get(c);
      if(part)
        {
          GRPVECTORFILEDXFENTITY* entity = GEN_NEW GRPVECTORFILEDXFENTITY();
          if(entity)
            {          
              entity->GetName()->Set(part->name);

              indexline = part->iniline;
                  
              do{ line = fileTXT->GetLine(indexline);      
                  if(line) 
                    {  
                      indexline++;                    
                      GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);                  

                      int type = line->ConvertToInt();
                  
                               
                      GRPVECTORFILEDXFVALUE* value = GEN_NEW GRPVECTORFILEDXFVALUE();
                      if(value)
                        {      
                          GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE* defType = IsKnownTypeValue(part->name, type);
                          if(!defType)
                            {      
                              #ifdef TEST_ONLY_DEFINE_IN_ENTITITES
                           
                              XSTRING message;

                              message.Format(NL("type data of entitity %s not register in definition [%d]"), part->name.Get(), type);
                               
                              GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PartUnknown);

                              vfevent.SetType(GRPVECTORFILETYPE_DXF);
                              vfevent.GetPath()->Set(NL(""));
                              vfevent.GetMessage()->Set(message);

                              PostEvent(&vfevent, GetGrpVectorFile());

                              indexline++;
                              continue;

                              #else
                           
                              GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE* genDefType = GetGenericDefType(type);
                              if(genDefType)
                                {
                                  value->SetType(genDefType->type);
                                  value->GetName()->Set(genDefType->name);
                                  value->GetRemark()->Set(genDefType->remark);                      
                                }
                               else 
                                {
                                  XSTRING message;

                                  message.Format(NL("type data of entitity %s not register in definition [%d]"), part->name.Get(), type);
                               
                                  GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

                                  vfevent.SetType(GRPVECTORFILETYPE_DXF);
                                  vfevent.GetPath()->Set(NL(""));
                                  vfevent.GetMsg()->Set(message);

                                  PostEvent(&vfevent, GetGrpVectorFile());
                            
                                  indexline++;
                                  continue;
                                }

                              #endif
                            }
                           else
                            {
                              value->SetType(defType->type);
                              value->GetName()->Set(defType->name);
                              value->GetRemark()->Set(defType->remark); 
                            }

                          line = fileTXT->GetLine(indexline);
                          if(line)
                            {
                              GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);      

                              GetVariableFromLine(value->GetName()->Get(), type, line, (*value->GetData()));                          

                              switch(IsXDataControl(type, (*line)))
                                {
                                  case GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_NOT : if(xdatactrl) 
                                                                                            {
                                                                                              if(value) 
                                                                                                {                                                                                                            
                                                                                                  xdatactrl->GetValues()->Add(value);   
                                                                                                }
                                                                                            }
                                                                                          break;

                                  case GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_INI : if(!xdatactrl)                
                                                                                            {
                                                                                              xdatactrl = GEN_NEW GRPVECTORFILEDXFXDATACTRL();
                                                                                              if(xdatactrl)                
                                                                                                {
                                                                                                  XSTRING name;
                                                                                                  line->Copy(1, name);
                                                                                       
                                                                                                  xdatactrl->GetName()->Set(name);
                                                                                                }
                                                                                            }
                                                                                           break;

                                  case GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_END : if(xdatactrl)                
                                                                                            { 
                                                                                              entity->GetXDataCtrlList()->Add(xdatactrl);
                                                                                              xdatactrl = NULL;
                                                                                            }                              
                                                                                          break;
                               }
 
                           
                              if(value) 
                                {  
                                  entity->AddValue(value);
                                }
                            }   
                        }               
                    }
         
                  indexline++;

                } while(indexline < part->endline); 
               
              AddEntity(entity);

              { GRPVECTORFILEDXFENTITYOBJ* entitybbj = GRPVECTORFILEDXFENTITYOBJ::CreateInstance(entity);
                if(entitybbj)
                  {
                    entitiesObj.Add(entitybbj); 
                  }
              }
            }
        }
    }
   
  parts.DeleteContents();
  parts.DeleteAll();

  #ifdef XTRACE_ACTIVE
  //ShowTraceAllEntities();
  #endif
   
  return GRPVECTORFILERESULT_OK;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntityEnumIndex(XCHAR* nameentity)
* @brief      Get entity enum index
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILEDXFTEXTSECTIONENTITIES::GetEntityEnumIndex(XCHAR* nameentity)
{
  for(XDWORD c=0; c<enumentities.GetSize(); c++)
    {
      XSTRING* name = enumentities.GetKey(c);
      if(name)
        {
          if(!name->Compare(nameentity)) return c;
        }   
    }

  return -1;
}
   
   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::AddEntityEnum(XCHAR* nameentity)
* @brief      Add entity enum
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::AddEntityEnum(XCHAR* nameentity)
{
  int index = GetEntityEnumIndex(nameentity);
  bool status = false;

  if(index == GRPVECTORFILEDXFEntities_NotEnumEntity) 
    {
      XSTRING* name = GEN_NEW XSTRING();
      if(name)
        {
          (*name) = nameentity;
          status = enumentities.Add(name, 1);          
        }      
    }
   else 
    {
      int nentities = enumentities.GetElement(index);
      nentities++;
      status = enumentities.Set(enumentities.GetKey(index), nentities);         
    }

  return status; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::SubtractEntityEnum(XCHAR* nameentity)
* @brief      Subtract entity enum
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::SubtractEntityEnum(XCHAR* nameentity)
{
  int   index   = GetEntityEnumIndex(nameentity);
  bool  status  = false;

  if(index != GRPVECTORFILEDXFEntities_NotEnumEntity)    
    {
      int nentities = enumentities.GetElement(index);
      nentities--;
      if(nentities)
        {         
          status = enumentities.Set(enumentities.GetKey(index), nentities);   
        }
       else
        {
          status = SetZeroEntityEnum(nameentity);
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::SetZeroEntityEnum(XCHAR* nameentity)
* @brief      Set zero entity enum
* @ingroup    GRAPHIC
* 
* @param[in]  nameentity : Nameentity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::SetZeroEntityEnum(XCHAR* nameentity)
{
  int   index  = GetEntityEnumIndex(nameentity);
  bool  status = false;

  if(index != GRPVECTORFILEDXFEntities_NotEnumEntity)
    {
      XSTRING* name = enumentities.GetKey(index);
      if(name)
        {
          enumentities.Delete(name);
          GEN_DELETE name;

          status = true;
        }   
    }

  return status;
}


#ifdef XTRACE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::ShowTraceAllEntities()
* @brief      Show trace all entities
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONENTITIES::ShowTraceAllEntities()
{
  for(XDWORD c=0; c<enumentities.GetSize(); c++)
    {
      XSTRING* name = enumentities.GetKey(c);
      if(name)
        {
          int nentities = enumentities.GetElement(c);
          if(nentities)
            {           
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, NL("[GRPVECTORFILEDXFTEXTSECTIONENTITIES] (%3d) Entity [%s] (%d) element(s). "), c, name->Get(), nentities);

              for(int d=0; d<nentities; d++)
                {
                  GRPVECTORFILEDXFENTITY* entity = GetEntity (name->Get(), d);
                  if(entity)
                    {
                      if(!entity->GetValues()->GetSize())
                        {
                          XTRACE_PRINTCOLOR((entity->GetValues()->GetSize()?XTRACE_COLOR_BLUE:XTRACE_COLOR_PURPLE), NL("  (%3d) Entity [%s] (%d) value(s). "), d, name->Get(), entity->GetValues()->GetSize());                 
                        }
                    }
                }
            }
        }    
    }

  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFTEXTSECTIONENTITIES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFTEXTSECTIONENTITIES::Clean()
{
  
}

