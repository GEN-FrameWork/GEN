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
   { __L("3DFACE")            , 14 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbFace)") },
                                       {  10 , __L("CORNER1_X")                     , __L("First corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , __L("CORNER1_Y")                     , __L("DXF: Y value of first corner (in WCS)") },
                                       {  30 , __L("CORNER1_Z")                     , __L("DXF: Z value of first corner (in WCS)") },
                                       {  11 , __L("CORNER2_X")                     , __L("Second corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , __L("CORNER2_Y")                     , __L("DXF: Y value of second corner (in WCS)") },
                                       {  31 , __L("CORNER2_Z")                     , __L("DXF: Z value of second corner (in WCS)") },
                                       {  12 , __L("CORNER3_X")                     , __L("Third corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  22 , __L("CORNER3_Y")                     , __L("DXF: Y value of third corner (in WCS)") },
                                       {  32 , __L("CORNER3_Z")                     , __L("DXF: Z value of third corner (in WCS)") },
                                       {  13 , __L("CORNER4_X")                     , __L("Fourth corner (in WCS) DXF: X value; APP: 3D point") },
                                       {  23 , __L("CORNER4_Y")                     , __L("DXF: Y value of fourth corner (in WCS)") },
                                       {  33 , __L("CORNER4_Z")                     , __L("DXF: Z value of fourth corner (in WCS)") },
                                       {  70 , __L("INVISIBLE_EDGE_FLAGS")          , __L("Invisible edge flags (optional; default = 0)") } } },
   { __L("3DSOLID")           ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("ACAD_PROXY_ENTITY") ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("ARC")               , 11 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbCircle)") },
                                       { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbArc)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_CENTER_POINT_X                 , __L("Center point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                 , __L("DXF: Y value of center point (in OCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                 , __L("DXF: Z value of center point (in OCS)") },
                                       {  40 , VFDXF_RADIOUS                        , __L("Radius") },
                                       {  50 , VFDXF_INI_ANGLE                      , __L("Ini angle") },
                                       {  51 , VFDXF_END_ANGLE                      , __L("End angle") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("ATTDEF")            ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("ATTRIB")            ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("BODY")              ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("CIRCLE")            ,  9 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbCircle)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_CENTER_POINT_X                 , __L("Center point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                 , __L("DXF: Y value of center point (in OCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                 , __L("DXF: Z value of center point (in OCS)") },
                                       {  40 , VFDXF_RADIOUS                        , __L("Radius") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("DIMENSION")         ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("ELLIPSE")           , 13 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbEllipse)") },
                                       {  10 , VFDXF_CENTER_POINT_X                 , __L("Center point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                 , __L("DXF: Y value of center point (in WCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                 , __L("DXF: Z value of center point (in WCS)") },
                                       {  11 , VFDXF_END_POINT_X                    , __L("Endpoint of major axis, relative to the center (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , VFDXF_END_POINT_Y                    , __L("DXF: Y value of endpoint of major axis, relative to the center (in WCS)") },
                                       {  31 , VFDXF_END_POINT_Z                    , __L("DXF: Z value of endpoint of major axis, relative to the center (in WCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") },
                                       {  40 , __L("RATIO_MINOR_MAJOR_AXIS")        , __L("Ratio of minor axis to major axis") },
                                       {  41 , VFDXF_INI_PARAMETER                  , __L("Start parameter (this value is 0.0 for a full ellipse)") },
                                       {  42 , VFDXF_END_PARAMETER                  , __L("End parameter (this value is 2pi for a full ellipse)") } } },

   { __L("HATCH")            , 34 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbHatch)") },
                                       {   2 , __L("HATCH_PATTERN_NAME")            , __L("Hatch pattern name") },
                                       {  70 , __L("HATCH_SOLID_FILL_FLAG")         , __L("Solid fill flag (0 = pattern fill; 1 = solid fill)") },
                                       {  71 , __L("HATCH_ASSOCIATIVITY_FLAG")      , __L("Associativity flag (0 = non-associative; 1 = associative)") },
                                       {  91 , __L("HATCH_NUMBER_PATHS")            , __L("Number of boundary paths (loops)") },
                                       {  92 , __L("HATCH_PATH_TYPE_FLAG")          , __L("Boundary path type flag (bit coded: 0 = default; 2 = polyline; 16 = outermost)") },
                                       {  93 , __L("HATCH_NUMBER_EDGES")            , __L("Number of edges in this boundary path (or vertices if polyline)") },
                                       {  72 , __L("HATCH_EDGE_TYPE")               , __L("Edge type (1 = line; 2 = circular arc; 3 = elliptic arc; 4 = spline) / polyline has-bulge flag") },
                                       {  73 , __L("HATCH_IS_CLOSED")               , __L("Is closed flag (polyline) / counterclockwise flag (arc) / rational (spline)") },
                                       {  74 , __L("HATCH_PERIODIC")                , __L("Periodic flag (spline edge)") },
                                       {  94 , __L("HATCH_DEGREE")                  , __L("Degree of the spline edge") },
                                       {  95 , __L("HATCH_NUMBER_KNOTS")            , __L("Number of knots (spline edge)") },
                                       {  96 , __L("HATCH_NUMBER_CONTROL_POINTS")   , __L("Number of control points (spline edge)") },
                                       {  97 , __L("HATCH_NUMBER_SOURCE")           , __L("Number of source boundary objects / fit points") },
                                       {  98 , __L("HATCH_NUMBER_SEED")             , __L("Number of seed points") },
                                       {  10 , VFDXF_POINT_X                        , __L("DXF: X value of a point (vertex / center / start / control point)") },
                                       {  20 , VFDXF_POINT_Y                        , __L("DXF: Y value of a point") },
                                       {  30 , VFDXF_POINT_Z                        , __L("DXF: Z value (elevation)") },
                                       {  11 , __L("HATCH_POINT2_X")                , __L("DXF: X value of a second point (line end / ellipse major axis endpoint)") },
                                       {  21 , __L("HATCH_POINT2_Y")                , __L("DXF: Y value of a second point") },
                                       {  40 , __L("HATCH_RADIUS")                  , __L("Radius (arc) / minor-major ratio (ellipse) / knot value (spline) / pattern scale") },
                                       {  41 , __L("HATCH_PATTERN_SCALE")           , __L("Pattern scale or spacing") },
                                       {  42 , __L("HATCH_BULGE")                   , __L("Bulge (polyline vertex) / weight (rational spline)") },
                                       {  50 , __L("HATCH_START_ANGLE")             , __L("Start angle (arc / ellipse edge)") },
                                       {  51 , __L("HATCH_END_ANGLE")               , __L("End angle (arc / ellipse edge)") },
                                       {  52 , __L("HATCH_PATTERN_ANGLE")           , __L("Hatch pattern angle") },
                                       {  75 , __L("HATCH_STYLE")                   , __L("Hatch style (0 = normal/odd parity; 1 = outer; 2 = ignore)") },
                                       {  76 , __L("HATCH_PATTERN_TYPE")            , __L("Hatch pattern type") },
                                       {  78 , __L("HATCH_NUMBER_PATTERN_LINES")    , __L("Number of pattern definition lines") },
                                       {  47 , __L("HATCH_PIXEL_SIZE")              , __L("Pixel size") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction") },
                                       { 330 , __L("HATCH_SOURCE_HANDLE")           , __L("Source boundary object handle (soft pointer)") } } },
   { __L("HELIX")             ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("IMAGE")             ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("INSERT")            , 17 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbBlockReference)") },
                                       {  66 , __L("VARIABLE_ATTRIBUTES_FLAG")      , __L("Variable attributes-follow flag (optional; default = 0); if the value of attributes-follow flag is 1, a series of attribute entities is expected to follow the insert, terminated by a seqend entity") },
                                       {   2 , __L("BLOCK_NAME")                    , __L("Block name") },
                                       {  10 , VFDXF_INSERTION_POINT_X              , __L("Insertion point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INSERTION_POINT_Y              , __L("DXF: Y value of insertion point (in OCS)") },
                                       {  30 , VFDXF_INSERTION_POINT_Z              , __L("DXF: Z value of insertion point (in OCS)") },
                                       {  41 , __L("SCALE_FACTOR_X")                , __L("X scale factor (optional; default = 1)") },
                                       {  42 , __L("SCALE_FACTOR_Y")                , __L("Y scale factor (optional; default = 1)") },
                                       {  43 , __L("SCALE_FACTOR_Z")                , __L("Z scale factor (optional; default = 1)") },
                                       {  50 , __L("ROTATION_ANGLE")                , __L("Rotation angle (optional; default = 0)") },
                                       {  70 , __L("COLUMN_COUNT")                  , __L("Column count (optional; default = 1)") },
                                       {  71 , __L("ROW_COUNT")                     , __L("Row count (optional; default = 1)") },
                                       {  44 , __L("COLUMN_SPACING")                , __L("Column spacing (optional; default = 0)") },
                                       {  45 , __L("ROW_SPACING")                   , __L("Row spacing (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },


   { __L("LEADER")            ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("LIGHT")             ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("LINE")              , 11 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbLine)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , VFDXF_INI_POINT_X                    , __L("Start point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INI_POINT_Y                    , __L("DXF: Y value of start point (in WCS)") },
                                       {  30 , VFDXF_INI_POINT_Z                    , __L("DXF: Z value of start point (in WCS)") },
                                       {  11 , VFDXF_END_POINT_X                    , __L("Endpoint (in WCS) DXF: X value; APP: 3D point") },
                                       {  21 , VFDXF_END_POINT_Y                    , __L("DXF: Y value of endpoint (in WCS)") },
                                       {  31 , VFDXF_END_POINT_Z                    , __L("DXF: Z value of endpoint (in WCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("LWPOLYLINE")        , 15 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbPolyline)") },
                                       {  90 , __L("NVERTICES")                     , __L("Number of vertices") },
                                       {  70 , __L("POLYLINE_FLAG")                 , __L("Polyline flag (bit-coded); default is 0: 1 = Closed; 128 = Plinegen") },
                                       {  43 , __L("CONSTANT_WIDTH")                , __L("Constant width (optional; default = 0). Not used if variable width (codes 40 and/or 41) is set") },
                                       {  38 , __L("ELEVATION")                     , __L("Elevation (optional; default = 0)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , __L("VERTEX_X")                      , __L("Vertex coordinates (in OCS), multiple entries; one entry for each vertex DXF: X value; APP: 2D point") },
                                       {  20 , __L("VERTEX_Y")                      , __L("DXF: Y value of vertex coordinates (in OCS), multiple entries; one entry for each vertex") },
                                       {  91 , __L("VERTEX_ID")                     , __L("Vertex identifier") },
                                       {  40 , __L("INI_WIDTH")                     , __L("Starting width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set") },
                                       {  41 , __L("END_WIDTH")                     , __L("End width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set") },
                                       {  42 , __L("BULGLE")                        , __L("Bulge (multiple entries; one entry for each vertex) (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("MESH")              ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("MLEADER")           ,  1 , { {   0 , __L(""), __L("") } } },   
   { __L("MLEADERSTYLE")      ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("MLINE")             ,  1 , { {   0 , __L(""), __L("") } } },
   
   { __L("MTEXT")             , 51 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbMText)") },
                                       {  10 , VFDXF_INSERTION_POINT_X              , __L("Insertion point DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_INSERTION_POINT_Y              , __L("DXF: Y and Z values of insertion point") },
                                       {  30 , VFDXF_INSERTION_POINT_Z              , __L("DXF: Y and Z values of insertion point") },
                                       {  40 , __L("NOMINAL_TEXT_HEIGHT")           , __L("Nominal (initial) text height") },
                                       {  41 , __L("RECTANGLE_WIDTH")               , __L("Reference rectangle width") },
                                       {  71 , __L("ATTACHMENT_POINT")              , __L("Attachment point: 1 = Top left, 2 = Top center, 3 = Top right, 4 = Middle left, 5 = Middle center, 6 = Middle right, 7 = Bottom left, 8 = Bottom center, 9 = Bottom right.") },
                                       {  72 , __L("DRAWING_DIRECTION")             , __L("Drawing direction: 1 = Left to right, 3 = Top to bottom, 5 = By style (the flow direction is inherited from the associated text style).") },
                                       {   1 , __L("TEXT_STRING")                   , __L("Text string. If the text string is less than 250 characters, all characters appear in group 1. If the text string is greater than 250 characters, the string is divided into 250-character chunks, which appear in one or more group 3 codes. If group 3 codes are used, the last group is a group 1 and has fewer than 250 characters") },
                                       {   3 , __L("ADDITIONAL_TEXT_STRING")        , __L("Additional text (always in 250-character chunks) (optional)") },
                                       {   7 , __L("TEXT_STYLE_NAME")               , __L("Text style name (STANDARD if not provided) (optional)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") },
                                       {  11 , __L("DIRECTION_VECTOR_X")            , __L("X-axis direction vector (in WCS) DXF: X value; APP: 3D vector A group code 50 (rotation angle in radians) passed as DXF input is converted to the equivalent direction vector (if both a code 50 and codes 11, 21, 31 are passed, the last one wins). This is provided as a convenience for conversions from text objects") },
                                       {  21 , __L("DIRECTION_VECTOR_Y")            , __L("DXF: Y value of X-axis direction vector (in WCS)") },  
                                       {  31 , __L("DIRECTION_VECTOR_Z")            , __L("DXF: Z value of X-axis direction vector (in WCS)") },
                                       {  42 , __L("HORIZONTAL_WIDTH")              , __L("Horizontal width of the characters that make up the mtext entity. This value will always be equal to or less than the value of group code 41 (read-only, ignored if supplied)") },
                                       {  43 , __L("VERTICAL_HEIGHT")               , __L("Vertical height of the mtext entity (read-only, ignored if supplied)") },
                                       {  50 , __L("ROTATION_ANGLE")                , __L("Rotation angle in radians") },
                                       {  73 , __L("LINE_SPACING_STYLE")            , __L("Mtext line spacing style (optional): 1 = At least (taller characters will override), 2 = Exact (taller characters will not override).") },
                                       {  44 , __L("LINE_SPACING_FACTOR")           , __L("Mtext line spacing factor (optional): Percentage of default (3-on-5) line spacing to be applied. Valid values range from 0.25 to 4.00") },
                                       {  90 , __L("BACKGROUND_FILL_SETTING")       , __L("Background fill setting: 0 = Background fill off, 1 = Use background fill color, 2 = Use drawing window color as background fill color.") },
                                       { 420 , __L("BACKGROUND_COLOR_00")           , __L("Background color (if RGB color)") },
                                       { 421 , __L("BACKGROUND_COLOR_01")           , __L("Background color (if RGB color)") },
                                       { 422 , __L("BACKGROUND_COLOR_02")           , __L("Background color (if RGB color)") },  
                                       { 423 , __L("BACKGROUND_COLOR_03")           , __L("Background color (if RGB color)") },
                                       { 424 , __L("BACKGROUND_COLOR_04")           , __L("Background color (if RGB color)") },
                                       { 425 , __L("BACKGROUND_COLOR_05")           , __L("Background color (if RGB color)") },
                                       { 426 , __L("BACKGROUND_COLOR_06")           , __L("Background color (if RGB color)") },
                                       { 427 , __L("BACKGROUND_COLOR_07")           , __L("Background color (if RGB color)") },
                                       { 428 , __L("BACKGROUND_COLOR_08")           , __L("Background color (if RGB color)") },
                                       { 429 , __L("BACKGROUND_COLOR_09")           , __L("Background color (if RGB color)") },
                                       { 430 , __L("BACKGROUND_COLOR_10")           , __L("Background color (if RGB color)") }, 
                                       { 431 , __L("BACKGROUND_COLOR_11")           , __L("Background color (if RGB color)") },
                                       { 432 , __L("BACKGROUND_COLOR_12")           , __L("Background color (if RGB color)") },  
                                       { 433 , __L("BACKGROUND_COLOR_13")           , __L("Background color (if RGB color)") },
                                       { 434 , __L("BACKGROUND_COLOR_14")           , __L("Background color (if RGB color)") },
                                       { 435 , __L("BACKGROUND_COLOR_15")           , __L("Background color (if RGB color)") },
                                       { 436 , __L("BACKGROUND_COLOR_16")           , __L("Background color (if RGB color)") },
                                       { 437 , __L("BACKGROUND_COLOR_17")           , __L("Background color (if RGB color)") },
                                       { 438 , __L("BACKGROUND_COLOR_18")           , __L("Background color (if RGB color)") },
                                       { 439 , __L("BACKGROUND_COLOR_19")           , __L("Background color (if RGB color)") },
                                       {  45 , __L("FILL_BOX_SCALE")                , __L("Fill box scale (optional): Determines how much border there is around the text.") },
                                       {  63 , __L("BACKGROUND_FILL_COLOR")         , __L("Background fill color (optional): Color to use for background fill when group code 90 is 1.") },
                                       { 441 , __L("TRANSPARENCY_BACKGROUND_COLOR") , __L("Transparency of background fill color (not implemented)") },
                                       {  75 , __L("COLUMN_TYPE")                   , __L("Column type") },
                                       {  76 , __L("COLUMN_COUNT")                  , __L("Column count") },
                                       {  78 , __L("COLUMN_FLOW REVERSED")          , __L("Column Flow Reversed") },
                                       {  79 , __L("COLUMN_AUTOHEIGHT")             , __L("Column Autoheight") },
                                       {  48 , __L("COLUMN_WIDTH")                  , __L("Column width") },
                                       {  49 , __L("COLUMN_GUTTER")                 , __L("Column gutter") },
                                       {  50 , __L("COLUMN_HEIGHTS")                , __L("Column heights; this code is followed by a column count (Int16), and then the number of column heights.") } } },

   { __L("MULTILEADER")       ,  1 , { {   0 , __L(""), __L("") } } },                                          
   { __L("OLEFRAME")          ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("OLE2FRAME")         ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("POINT")             ,  9 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbPoint)") },
                                       {  10 , VFDXF_POINT_X                        , __L("Point location (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_POINT_Y                        , __L("DXF: Y value of point location (in WCS)") },
                                       {  30 , VFDXF_POINT_Z                        , __L("DXF: Z value of point location (in WCS)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") },
                                       {  50 , VFDXF_ANGLE_X                        , __L("Angle of the X axis for the UCS in effect when the point was drawn (optional, default = 0); used when PDMODE is nonzero") } } },

   { __L("POLYLINE")          , 17 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker AcDb2dPolyline") },
                                       { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker AcDb3dPolyline") },
                                       {  66 , __L("OBSOLETE")                      , __L("Obsolete; formerly an 'entities follow flag' (optional; ignore if present)") },
                                       {  10 , __L("ALWAYS_0_APP")                  , __L("DXF: always 0 APP: a 'dummy' point; the X and Y values are always 0, and the Z value is the polyline's elevation (in OCS when 2D, WCS when 3D)") },
                                       {  20 , __L("ALWAYS_0")                      , __L("DXF: always 0") },
                                       {  30 , __L("POLYLINE_ELEVATION")            , __L("DXF: polyline's elevation (in OCS when 2D; WCS when 3D)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  70 , __L("POLYLINE_FLAG")                 , __L("Polyline flag (bit-coded; default = 0): 1 = This is a closed polyline (or a polygon mesh closed in the M direction), 2 = Curve-fit vertices have been added, 4 = Spline-fit vertices have been added, 8 = This is a 3D polyline, 16 = This is a 3D polygon mesh, 32 = The polygon mesh is closed in the N direction, 64 = The polyline is a polyface mesh. 128 = The linetype pattern is generated continuously around the vertices of this polyline.") },
                                       {  40 , __L("DEFAULT_START WIDTH")           , __L("Default start width (optional; default = 0)") },
                                       {  41 , __L("DEFAULT_END_WIDTH")             , __L("Default end width (optional; default = 0)") },
                                       {  71 , __L("POLYGON_MESH_M_VERTEX_COUNT")   , __L("Polygon mesh M vertex count (optional; default = 0)") },
                                       {  73 , __L("SMOOTH_SURFACE_M_DENSITY")      , __L("Smooth surface M density (optional; default = 0)") },
                                       {  74 , __L("SMOOTH_SURFACE_N_DENSITY")      , __L("Smooth surface N density (optional; default = 0)") },
                                       {  75 , __L("CURVES_SMOOTH_SURFACE_TYPE")    , __L("Curves and smooth surface type (optional; default = 0); integer codes, not bit-coded: 0 = No smooth surface fitted, 5 = Quadratic B-spline surface, 6 = Cubic B-spline surface, 8 = Bezier surface.") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("RAY")               ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("REGION")            ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("SECTION")           ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("SEQEND")            ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("SHAPE")             ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("SOLID")             , 17 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbTrace)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , __L("CORNER1_X")                     , __L("First corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , __L("CORNER1_Y")                     , __L("DXF: Y value of first corner (in OCS)") },
                                       {  30 , __L("CORNER1_Z")                     , __L("DXF: Z value of first corner (in OCS)") },
                                       {  11 , __L("CORNER2_X")                     , __L("Second corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  21 , __L("CORNER2_Y")                     , __L("DXF: Y value of second corner (in OCS)") },
                                       {  31 , __L("CORNER2_Z")                     , __L("DXF: Z value of second corner (in OCS)") },
                                       {  12 , __L("CORNER3_X")                     , __L("Third corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  22 , __L("CORNER3_Y")                     , __L("DXF: Y value of third corner (in OCS)") },
                                       {  32 , __L("CORNER3_Z")                     , __L("DXF: Z value of third corner (in OCS)") },
                                       {  13 , __L("CORNER4_X")                     , __L("Fourth corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  23 , __L("CORNER4_Y")                     , __L("DXF: Y value of fourth corner (in OCS)") },
                                       {  33 , __L("CORNER4_Z")                     , __L("DXF: Z value of fourth corner (in OCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },

   { __L("SPLINE")            , 26 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbSpline)") },
                                       { 210 , __L("VECTOR_X")                      , __L("Normal vector (omitted if the spline is nonplanar) DXF: X value; APP: 3D vector") },
                                       { 220 , __L("VECTOR_Y")                      , __L("DXF: Y value of normal vector (optional)") },
                                       { 230 , __L("VECTOR_Z")                      , __L("DXF: Z value of normal vector (optional)") },
                                       {  70 , __L("SPLINE_FLAG")                   , __L("Spline flag (bit coded): 1 = Closed spline, 2 = Periodic spline, 4 = Rational spline, 8 = Planar, 16 = Linear (planar bit is also set)") },
                                       {  71 , __L("DEGREE")                        , __L("Degree of the spline curve") },
                                       {  72 , __L("NKNOTS")                        , __L("Number of knots") },
                                       {  73 , __L("NCTRL_POINTS")                  , __L("Number of control points") },
                                       {  74 , __L("NFIT_POINTS")                   , __L("Number of fit points (if any)") },
                                       {  42 , __L("KNOT_TOLERANCE")                , __L("Knot tolerance (default = 0.0000001)") },
                                       {  43 , __L("CTRLPOINT_TOLERANCE")           , __L("Control-point tolerance (default = 0.0000001)") },
                                       {  44 , __L("FIT_TOLERANCE")                 , __L("Fit tolerance (default = 0.0000000001)") },
                                       {  12 , __L("INI_TANGENT_X")                 , __L("Start tangent-may be omitted (in WCS) DXF: X value; APP: 3D point") },
                                       {  22 , __L("INI_TANGENT_Y")                 , __L("DXF: Y value of start tangent-may be omitted (in WCS)") },
                                       {  32 , __L("INI_TANGENT_Z")                 , __L("DXF: Z value of start tangent-may be omitted (in WCS)") },
                                       {  13 , __L("END_TANGENT_X")                 , __L("End tangent-may be omitted (in WCS) DXF: X value; APP: 3D point") },
                                       {  23 , __L("END_TANGENT_Y")                 , __L("DXF: Y value of end tangent-may be omitted (in WCS)") },
                                       {  33 , __L("END_TANGENT_Z")                 , __L("DXF: Z value of end tangent-may be omitted (in WCS)") },
                                       {  40 , __L("KNOT_VALUE")                    , __L("Knot value (one entry per knot)") },
                                       {  41 , __L("WEIGHT")                        , __L("Weight (if not 1); with multiple group pairs, they are present if all are not 1") },
                                       {  10 , __L("CTRL_POINT_X")                  , __L("Control points (in WCS); one entry per control point DXF: X value; APP: 3D point") },
                                       {  20 , __L("CTRL_POINT_Y")                  , __L("DXF: Y value of control points (in WCS); one entry per control point") },
                                       {  30 , __L("CTRL_POINT_Z")                  , __L("DXF: Z value of control points (in WCS); one entry per control point") },
                                       {  11 , __L("FIT_POINT_X")                   , __L("Fit points (in WCS); one entry per fit point DXF: X value; APP: 3D point") },
                                       {  21 , __L("FIT_POINT_Y")                   , __L("DXF: Y value of fit points (in WCS); one entry per fit point") },
                                       {  31 , __L("FIT_POINT_Z")                   , __L("DXF: Z value of fit points (in WCS); one entry per fit point") } } },

   { __L("SUN")               ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("SURFACE")           ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("TABLE")             ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("TEXT")              , 21 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbText)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , __L("ALIGNMENT_POINT_X")             , __L("First alignment point (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , __L("ALIGNMENT_POINT_Y")             , __L("DXF: Y value of first alignment point (in OCS)") },
                                       {  30 , __L("ALIGNMENT_POINT_Z")             , __L("DXF: Z value of first alignment point (in OCS)") },
                                       {  40 , __L("TEXT_HEIGHT")                   , __L("Text height") },
                                       {   1 , __L("DEFAULT_VALUE")                 , __L("Default value (the string itself)") },
                                       {  50 , __L("TEXT_ROTATION")                 , __L("Text rotation (optional; default = 0)") },
                                       {  41 , __L("RELATIVE_X_SCALE_WIDTH")        , __L("Relative X scale factor-width (optional; default = 1) This value is also adjusted when fit-type text is used") },
                                       {  51 , __L("OBLIQUE_ANGLE")                 , __L("Oblique angle (optional; default = 0)") },
                                       {   7 , __L("TEXT_STYLE_NAME")               , __L("Text style name (optional, default = STANDARD)") },
                                       {  71 , __L("TEXT GENERATION FLAGS")         , __L("Text generation flags (optional, default = 0): 2 = Text is backward (mirrored in X), 4 = Text is upside down (mirrored in Y)") },
                                       {  72 , __L("HORIZONTAL_JUSTIFICATION_TYPE") , __L("Horizontal text justification type (optional, default = 0) integer codes (not bit-coded): 0 = Left, 1= Center, 2 = Right, 3 = Aligned (if vertical alignment = 0), 4 = Middle (if vertical alignment = 0), 5 = Fit (if vertical alignment = 0), See the Group 72 and 73 integer codes table for clarification.") },
                                       {  11 , __L("SECOND_ALIGNMENT_POINT_X")      , __L("Second alignment point (in OCS) (optional) DXF: X value; APP: 3D point This value is meaningful only if the value of a 72 or 73 group is nonzero (if the justification is anything other than baseline/left)") },
                                       {  21 , __L("SECOND_ALIGNMENT_POINT_Y")      , __L("DXF: Y value of second alignment point (in OCS) (optional)") },
                                       {  31 , __L("SECOND_ALIGNMENT_POINT_Z")      , __L("DXF: Z value of second alignment point (in OCS) (optional)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1) DXF: X value; APP: 3D vector") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") },
                                       {  73 , __L("VERTICAL_JUSTIFICATION_TYPE")   , __L("Vertical text justification type (optional, default = 0): integer codes (not bit-coded): 0 = Baseline, 1 = Bottom, 2 = Middle, 3 = Top. See the Group 72 and 73 integer codes table for clarification.") } } },

   { __L("TOLERANCE")         ,  1 , { {   0 , __L(""), __L("") } } },
   { __L("TRACE")             , 17 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbTrace)") },
                                       {  39 , VFDXF_THICKNESS                      , __L("Thickness (optional; default = 0)") },
                                       {  10 , __L("CORNER1_X")                     , __L("First corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  20 , __L("CORNER1_Y")                     , __L("DXF: Y value of first corner (in OCS)") },
                                       {  30 , __L("CORNER1_Z")                     , __L("DXF: Z value of first corner (in OCS)") },
                                       {  11 , __L("CORNER2_X")                     , __L("Second corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  21 , __L("CORNER2_Y")                     , __L("DXF: Y value of second corner (in OCS)") },
                                       {  31 , __L("CORNER2_Z")                     , __L("DXF: Z value of second corner (in OCS)") },
                                       {  12 , __L("CORNER3_X")                     , __L("Third corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  22 , __L("CORNER3_Y")                     , __L("DXF: Y value of third corner (in OCS)") },
                                       {  32 , __L("CORNER3_Z")                     , __L("DXF: Z value of third corner (in OCS)") },
                                       {  13 , __L("CORNER4_X")                     , __L("Fourth corner (in OCS) DXF: X value; APP: 3D point") },
                                       {  23 , __L("CORNER4_Y")                     , __L("DXF: Y value of fourth corner (in OCS)") },
                                       {  33 , __L("CORNER4_Z")                     , __L("DXF: Z value of fourth corner (in OCS)") },
                                       { 210 , VFDXF_EXTRUSION_DIRECTION_X          , __L("Extrusion direction (optional; default = 0, 0, 1)") },
                                       { 220 , VFDXF_EXTRUSION_DIRECTION_Y          , __L("DXF: Y value of extrusion direction (optional)") },
                                       { 230 , VFDXF_EXTRUSION_DIRECTION_Z          , __L("DXF: Z value of extrusion direction (optional)") } } },
   { __L("UNDERLAY")          ,  1 , { {   0 , __L(""), __L("") } } },

   { __L("VERTEX")            , 15 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbVertex)") },
                                       { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDb2dVertex or AcDb3dPolylineVertex)") },
                                       {  10 , VFDXF_LOCATION_POINT_X               , __L("Location point (in OCS when 2D, and WCS when 3D) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_LOCATION_POINT_Y               , __L("DXF: Y and Z values of location point (in OCS when 2D, and WCS when 3D)") },
                                       {  30 , VFDXF_LOCATION_POINT_Z               , __L("DXF: Y and Z values of location point (in OCS when 2D, and WCS when 3D)") },
                                       {  40 , __L("STARTING_WIDTH")                , __L("Starting width (optional; default is 0)") },
                                       {  41 , __L("ENDING_WIDTH")                  , __L("Ending width (optional; default is 0)") },
                                       {  42 , __L("BULGE")                         , __L("Bulge (optional; default is 0). The bulge is the tangent of one fourth the included angle for an arc segment, made negative if the arc goes clockwise from the start point to the endpoint. A bulge of 0 indicates a straight segment, and a bulge of 1 is a semicircle") },
                                       {  70 , __L("VERTEX_FLAGS")                  , __L("Vertex flags: 1 = Extra vertex created by curve-fitting, 2 = Curve-fit tangent defined for this vertex. A curve-fit tangent direction of 0 may be omitted from DXF output but is significant if this bit is set, 4 = Not used, 8 = Spline vertex created by spline-fitting, 16 = Spline frame control point, 32 = 3D polyline vertex, 64 = 3D polygon mesh, 128 = Polyface mesh vertex.") },
                                       {  50 , __L("CURVE_FIT_TANGENT_DIRECTION")   , __L("Curve fit tangent direction") },
                                       {  71 , __L("POLYFACE_MESH_VERTEX_INDEX_1")  , __L("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  72 , __L("POLYFACE_MESH_VERTEX_INDEX_2")  , __L("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  73 , __L("POLYFACE_MESH_VERTEX_INDEX_3")  , __L("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  74 , __L("POLYFACE_MESH_VERTEX_INDEX_4")  , __L("Polyface mesh vertex index (optional; present only if nonzero)") },
                                       {  91 , __L("VERTEX_IDENTIFIER")             , __L("Vertex identifier") } } },

   { __L("VIEWPORT")          , 85 , { { 100 , VFDXF_SUBCLASS                       , __L("Subclass marker (AcDbViewport)") },                                                
                                       {  10 , VFDXF_CENTER_POINT_X                 , __L("Center point (in WCS) DXF: X value; APP: 3D point") },
                                       {  20 , VFDXF_CENTER_POINT_Y                 , __L("DXF: Y value of center point (in WCS)") },
                                       {  30 , VFDXF_CENTER_POINT_Z                 , __L("DXF: Z value of center point (in WCS)") },
                                       {  40 , __L("WIDTH_PAPER")                   , __L("Width in paper space units") },
                                       {  41 , __L("HEIGHT_PAPER")                  , __L("Height in paper space units") },
                                       {  68 , __L("VIEWPORT_STATUS_FIELD")         , __L("Viewport status field: -1 = On, but is fully off screen, or is one of the viewports that is not active because the $MAXACTVP count is currently being exceeded. 0 = Off <positive value > = On and active. The value indicates the order of stacking for the viewports, where 1 is the active viewport, 2 is the next, and so forth") },
                                       {  69 , __L("VIEWPORT_ID")                   , __L("Viewport ID") },
                                       {  12 , __L("VIEW_CENTER_POINT_X")           , __L("View center point (in DCS) DXF: X value; APP: 2D point") },
                                       {  22 , __L("VIEW_CENTER_POINT_Y")           , __L("DXF: View center point Y value (in DCS)") },
                                       {  13 , __L("SNAP_BASE_POINT_X")             , __L("Snap base point DXF: X value; APP: 2D point") },
                                       {  23 , __L("SNAP_BASE_POINT_Y")             , __L("DXF: Snap base point Y value") },
                                       {  14 , __L("SNAP_SPACING_X")                , __L("Snap spacing DXF: X value; APP: 2D point") },
                                       {  24 , __L("SNAP_SPACING_Y")                , __L("DXF: Snap spacing Y value") },
                                       {  15 , __L("GRID_SPACING_X")                , __L("Grid spacing DXF: X value; APP: 2D point") },
                                       {  25 , __L("GRID_SPACING_Y")                , __L("DXF: Grid spacing Y value") },
                                       {  16 , __L("VIEW_DIRECTION_VECTOR_X")       , __L("View direction vector (in WCS) DXF: X value; APP: 3D vector") },
                                       {  26 , __L("VIEW_DIRECTION_VECTOR_Y")       , __L("DXF: Y value of view direction vector (in WCS)") },
                                       {  36 , __L("VIEW_DIRECTION_VECTOR_Z")       , __L("DXF: Z value of view direction vector (in WCS)") },
                                       {  17 , __L("VIEW_TARGET_POINT_X")           , __L("View target point (in WCS) DXF: X value; APP: 3D vector") },
                                       {  27 , __L("VIEW_TARGET_POINT_Y")           , __L("DXF: Y value of view target point (in WCS)") },
                                       {  37 , __L("VIEW_TARGET_POINT_Z")           , __L("DXF: Z value of view target point (in WCS)") },
                                       {  42 , __L("PERSPECTIVE_LENS_LENGTH")       , __L("Perspective lens length") },
                                       {  43 , __L("FRONT_CLIP_PLANE_Z")            , __L("Front clip plane Z value") },
                                       {  44 , __L("BACK_CLIP_PLANE_Z")             , __L("Back clip plane Z value") },
                                       {  45 , __L("VIEW_HEIGHT")                   , __L("View height (in model space units)") },
                                       {  50 , __L("SNAP_ANGLE")                    , __L("Snap angle") },
                                       {  51 , __L("VIEW_TWIST_ANGLE")              , __L("View twist angle") },
                                       {  72 , __L("CIRCLE_ZOOM_PERCENT")           , __L("Circle zoom percent") },
                                       { 331 , __L("FROZEN_LAYER_ID-HDL")           , __L("Frozen layer object ID-handle (multiple entries may exist) (optional)") },
                                       {  90 , __L("VIEWPORT_STATUS")               , __L("Viewport status bit-coded flags: 1 (0x1) = Enables perspective mode,"
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
                                       { 340 , __L("HARD-POINTER_ID-HDL")           , __L("Hard-pointer ID-handle to entity that serves as the viewport's clipping boundary (only present if viewport is non-rectangular)") },
                                       {   1 , __L("PLOT_STYLE_SHEET_NAME")         , __L("Plot style sheet name assigned to this viewport") },
                                       { 281 , __L("RENDER_MODE")                   , __L("Render mode: 0 = 2D Optimized (classic 2D), 1 = Wireframe, 2 = Hidden line, 3 = Flat shaded, 4 = Gouraud shaded, 5 = Flat shaded with wireframe, 6 = Gouraud shaded with wireframe, All rendering modes other than 2D Optimized engage the GEN_NEW 3D graphics pipeline. These values directly correspond to the SHADEMODE command and the AcDbAbstractViewTableRecord::RenderMode enum.") },
                                       {  71 , __L("UCS_VIEWPORT_FLAG")             , __L("UCS per viewport flag: 0 = The UCS will not change when this viewport becomes active. 1 = This viewport stores its own UCS which will become the current UCS whenever the viewport is activated") },
                                       {  74 , __L("DISPLAY_UCS_ICON")              , __L("Display UCS icon at UCS origin flag: Controls whether UCS icon represents viewport UCS or current UCS (these will be different if UCSVP is 1 and viewport is not active). However, this field is currently being ignored and the icon always represents the viewport UCS") },
                                       { 110 , __L("UCS_ORIGIN_X")                  , __L("UCS origin DXF: X value; APP: 3D point") },
                                       { 120 , __L("UCS_ORIGIN_Y")                  , __L("DXF: Y value of UCS origin") },
                                       { 130 , __L("UCS_ORIGIN_Z")                  , __L("DXF: Z value of UCS origin") },
                                       { 111 , __L("UCS_X-AXIS_X")                  , __L("UCS X-axis DXF: X value; APP: 3D vector") },
                                       { 121 , __L("UCS_X-AXIS_Y")                  , __L("DXF: Y value of UCS X-axis") },
                                       { 131 , __L("UCS_X-AXIS_Z")                  , __L("DXF: Z value of UCS X-axis") },
                                       { 112 , __L("UCS_Y-AXIS_X")                  , __L("UCS Y-axis DXF: X value; APP: 3D vector") },
                                       { 122 , __L("UCS_Y-AXIS_Y")                  , __L("DXF: Y value of UCS Y-axis") },
                                       { 132 , __L("UCS_Y-AXIS_Z")                  , __L("DXF: Z value of UCS Y-axis") },
                                       { 345 , __L("ID_HDL_ACDBUCSTABLE_UCS")       , __L("ID/handle of AcDbUCSTableRecord if UCS is a named UCS. If not present, then UCS is unnamed") },
                                       { 346 , __L("ID_HDL_ACDBUCSTABLE_BASE_UCS")  , __L("ID/handle of AcDbUCSTableRecord of base UCS if UCS is orthographic (79 code is non-zero). If not present and 79 code is non-zero, then base UCS is taken to be WORLD") },
                                       {  79 , __L("ORTHOGRAPHIC_TYPE")             , __L("Orthographic type of UCS: 0 = UCS is not orthographic, 1 = Top; 2 = Bottom, 3 = Front; 4 = Back, 5 = Left; 6 = Right.") },
                                       { 146 , __L("ELEVATION")                     , __L("Elevation") },
                                       { 170 , __L("SHADEPLOT_MODE")                , __L("ShadePlot mode: 0 = As Displayed, 1 = Wireframe, 2 = Hidden, 3 = Rendered.") },
                                       {  61 , __L("FREQUENCY_GRID_LINES")          , __L("Frequency of major grid lines compared to minor grid lines") },
                                       { 332 , __L("BACKGROUND_ID-HDL")             , __L("Background ID/Handle (optional)") },
                                       { 333 , __L("SHADE_PLOT_ID-HDL")             , __L("Shade plot ID/Handle (optional)") },
                                       { 348 , __L("VISUAL_STYLE_ID-HDL")           , __L("Visual style ID/Handle (optional)") },
                                       { 292 , __L("DEFAULT_LIGHTING_FLAG")         , __L("Default lighting flag. On when no user lights are specified.") },
                                       { 282 , __L("DEFAULT_LIGHTING_TYPE")         , __L("Default lighting type: 0 = One distant light, 1 = Two distant lights.") },
                                       { 141 , __L("VIEW_BRIGHTNESS")               , __L("View brightness") },
                                       { 142 , __L("VIEW_CONTRAST")                 , __L("View contrast") },
                                       {  63 , __L("AMBIENT_LIGHT_COLOR_63")        , __L("Ambient light color. Write only if not black color.") },
                                       { 421 , __L("AMBIENT_LIGHT_COLOR_421")       , __L("Ambient light color. Write only if not black color.") },
                                       { 431 , __L("AMBIENT_LIGHT_COLOR_431")       , __L("Ambient light color. Write only if not black color.") },
                                       { 361 , __L("SUN_ID-HDL")                    , __L("Sun ID/Handle (optional)") },
                                       { 335 , __L("SOFT_PTR_REF_335")              , __L("Soft pointer reference to viewport object (for layer VP property override)") },
                                       { 343 , __L("SOFT_PTR_REF_343")              , __L("Soft pointer reference to viewport object (for layer VP property override)") },
                                       { 344 , __L("SOFT_PTR_REF_344")              , __L("Soft pointer reference to viewport object (for layer VP property override)") },
                                       {  91 , __L("SOFT_PTR_REF_VIEWPORT")         , __L("Soft pointer reference to viewport object (for layer VP property override)") } } },

   { __L("WIPEOUT")           ,  1 , { { 0 , __L(""), __L("") } } },
   { __L("XLINE")             ,  1 , { { 0 , __L(""), __L("") } } },
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

          if(!line->Compare(__L("0"), true))                                     // an entity starts at group code 0 : tested only at a code position (we step by pairs), so a *value* of "0" can never be mistaken for a delimiter
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

                          message.Format(__L("entity %s Unknown"), line->Get());
                                   
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

                              message.Format(__L("type data of entitity %s not register in definition [%d]"), part->name.Get(), type);
                               
                              GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PartUnknown);

                              vfevent.SetType(GRPVECTORFILETYPE_DXF);
                              vfevent.GetPath()->Set(__L(""));
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

                                  message.Format(__L("type data of entitity %s not register in definition [%d]"), part->name.Get(), type);
                               
                                  GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

                                  vfevent.SetType(GRPVECTORFILETYPE_DXF);
                                  vfevent.GetPath()->Set(__L(""));
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
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[GRPVECTORFILEDXFTEXTSECTIONENTITIES] (%3d) Entity [%s] (%d) element(s). "), c, name->Get(), nentities);

              for(int d=0; d<nentities; d++)
                {
                  GRPVECTORFILEDXFENTITY* entity = GetEntity (name->Get(), d);
                  if(entity)
                    {
                      if(!entity->GetValues()->GetSize())
                        {
                          XTRACE_PRINTCOLOR((entity->GetValues()->GetSize()?XTRACE_COLOR_BLUE:XTRACE_COLOR_PURPLE), __L("  (%3d) Entity [%s] (%d) value(s). "), d, name->Get(), entity->GetValues()->GetSize());                 
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

