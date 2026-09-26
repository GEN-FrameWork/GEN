/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSectionHeader.cpp
* 
* @class      GRPVECTORFILEDXFTEXTSECTIONHEADER
* @brief      Graphic Vector File DXF Entity Text Section Header class
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

#include "GRPVectorFileDXFTextSectionHeader.h"

#include "XMap.h"
#include "XVariant.h"

#include "GRPVectorFile_XEvent.h"
#include "GRPVectorFileDXF.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


GRPVECTORFILEDXFTEXTSECTIONHEADERDEFVARIABLE GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNDEFVAR] = //
{ 
  { __L("$3DDWFPREC")               , 1, { {  40, __L("")  } } ,  __L("Controls the precision of 3D DWF or 3D DWFx publishing. Minimum AutoCAD version: R2007") },
  { __L("$ACADMAINTVER")            , 1, { {  70, __L("")  } } ,  __L("Maintenance version number (should be ignored)") },
  { __L("$ACADVER")                 , 1, { {   1, __L("")  } } ,  __L("The AutoCAD drawing database version number: AC1006 = R10, AC1009 = R11 and R12, AC1012 = R13, AC1014 = R14, AC1015 = AutoCAD 2000, AC1018 = AutoCAD 2004, AC1021 = AutoCAD 2007, AC1024 = AutoCAD 2010, AC1027 = AutoCAD 2013, AC1032 = AutoCAD 2018") },
  { __L("$ANGBASE")                 , 1, { {  50, __L("")  } } ,  __L("Angle 0 direction") },
  { __L("$ANGDIR")                  , 1, { {  70, __L("")  } } ,  __L("1 = clockwise angles, 0 = counterclockwise angles") },
  { __L("$ATTDIA")                  , 1, { {  70, __L("")  } } ,  __L("Attribute entry dialogs; 1 = on, 0 = off") },
  { __L("$ATTMODE")                 , 1, { {  70, __L("")  } } ,  __L("Attribute visibility; 0 = none, 1 = normal, 2 = all") },
  { __L("$ATTREQ")                  , 1, { {  70, __L("")  } } ,  __L("Attribute prompting during INSERT; 1 = on, 0 = off") },
  { __L("$AUNITS")                  , 1, { {  70, __L("")  } } ,  __L("Units format for angles") },
  { __L("$AUPREC")                  , 1, { {  70, __L("")  } } ,  __L("Units precision for angles") },
  { __L("$AXISMODE")                , 1, { {  70, __L("")  } } ,  __L("Axis on if nonzero (not functional in Release 12)") },
  { __L("$AXISUNIT")                , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Axis X and Y tick spacing (not functional in Release 12)") },
  { __L("$BLIPMODE")                , 1, { {  70, __L("")  } } ,  __L("Blip mode on if nonzero") },
  { __L("$CAMERADISPLAY")           , 1, { { 290, __L("")  } } ,  __L("Turns the display of camera objects on or off. Minimum AutoCAD version: R2007") },
  { __L("$CAMERAHEIGHT")            , 1, { {  40, __L("")  } } ,  __L("Specifies the default height for GEN_NEW camera objects. Minimum AutoCAD version: R2007") },     
  { __L("$CECOLOR")                 , 1, { {  62, __L("")  } } ,  __L("Current entity color number: 0 = BYBLOCK; 256 = BYLAYER") },
  { __L("$CELTSCALE")               , 1, { {  40, __L("")  } } ,  __L("Current entity linetype scale") },
  { __L("$CELTYPE")                 , 1, { {   6, __L("")  } } ,  __L("Entity linetype name, or BYBLOCK or BYLAYER") },
  { __L("$CELWEIGHT")               , 1, { { 370, __L("")  } } ,  __L("Lineweight of GEN_NEW objects") },
  { __L("$CEPSNID")                 , 1, { { 390, __L("")  } } ,  __L("Plotstyle handle of GEN_NEW objects; if CEPSNTYPE is 3, then this value indicates the handle") },
  { __L("$CEPSNTYPE")               , 1, { { 380, __L("")  } } ,  __L("Plot style type of GEN_NEW objects: 0 = Plot style by layer, 1 = Plot style by block, 2 = Plot style by dictionary default, 3 = Plot style by object ID/handle") },
  { __L("$CHAMFERA")                , 1, { {  40, __L("")  } } ,  __L("First chamfer distance") },
  { __L("$CHAMFERB")                , 1, { {  40, __L("")  } } ,  __L("Second chamfer distance") },
  { __L("$CHAMFERC")                , 1, { {  40, __L("")  } } ,  __L("Chamfer length") },
  { __L("$CHAMFERD")                , 1, { {  40, __L("")  } } ,  __L("Chamfer angle") },
  { __L("$CLAYER")                  , 1, { {   8, __L("")  } } ,  __L("Current layer name 11") },
  { __L("$COORDS")                  , 1, { {  70, __L("")  } } ,  __L("0 = static coordinate display, 1 = continuous update, 2 = 'd<a' format") },
  { __L("$CMATERIAL")               , 1, { { 280, __L("")  } } ,  __L("Sets the material of GEN_NEW objects. Minimum AutoCAD version: R2007") },
  { __L("$CMLJUST")                 , 1, { {  70, __L("")  } } ,  __L("Current multiline justification: 0 = Top; 1 = Middle; 2 = Bottom") },
  { __L("$CMLSCALE")                , 1, { {  40, __L("")  } } ,  __L("Current multiline scale") },
  { __L("$CMLSTYLE")                , 1, { {   2, __L("")  } } ,  __L("Current multiline style name") },
  { __L("$CSHADOW")                 , 1, { { 280, __L("")  } } ,  __L("Shadow mode for a 3D object: 0 = Casts and receives shadows, 1 = Casts shadows,, 2 = Receives shadows, 3 = Ignores shadows (Note: Starting with AutoCAD 2016-based products, this variable is obsolete but still supported for backwards compatibility)") },
  { __L("$DELOBJ")                  , 1, { {  70, __L("")  } } ,  __L("Controls object deletion. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { __L("$DIMADEC")                 , 1, { {  70, __L("")  } } ,  __L("Number of precision places displayed in angular dimensions") },
  { __L("$DIMALT")                  , 1, { {  70, __L("")  } } ,  __L("Alternate unt dimensioning performed if nonzero") },
  { __L("$DIMALTD")                 , 1, { {  70, __L("")  } } ,  __L("Alternate unit decimal places") },
  { __L("$DIMALTF")                 , 1, { {  40, __L("")  } } ,  __L("Alternate unit scale factor") },
  { __L("$DIMALTRND")               , 1, { {  40, __L("")  } } ,  __L("Determines rounding of alternate units") },
  { __L("$DIMALTTD")                , 1, { {  70, __L("")  } } ,  __L("Number of decimal places for tolerance values of an alternate units dimension") },
  { __L("$DIMALTTZ")                , 1, { {  70, __L("")  } } ,  __L("Controls suppression of zeros for alternate tolerance values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet-. To suppress leading or trailing zeros, add the following values to one of the preceding values: 4 = Suppresses leading zeros. 8 = Suppresses trailing zeros") },
  { __L("$DIMALTU")                 , 1, { {  70, __L("")  } } ,  __L("Units format for alternate units of all dimension style family members except angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural (stacked), 5 = Fractional (stacked), 6 = Architectural, 7 = Fractional, 8 = Operating system defines the decimal separator and number grouping symbols") },
  { __L("$DIMALTZ")                 , 1, { {  70, __L("")  } } ,  __L("Controls suppression of zeros for alternate unit dimension values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },  
  { __L("$DIMAPOST")                , 1, { {   1, __L("")  } } ,  __L("Alternate dimensioning suffix") },
  { __L("$DIMASO")                  , 1, { {  70, __L("")  } } ,  __L("1 = create associative dimensioning, 0 = draw individual entities") },
  { __L("$DIMASSOC")                , 1, { { 280, __L("")  } } ,  __L("Controls the associativity of dimension objects 0 = Creates exploded dimensions; there is no association between elements of the dimension, and the lines, arcs, arrowheads, and text of a dimension are drawn as separate objects, 1 = Creates non-associative dimension objects; the elements of the dimension are formed into a single object, and if the definition point on the object moves, then the dimension value is updated, 2 = Creates associative dimension objects; the elements of the dimension are formed into a single object and one or more definition points of the dimension are coupled with association points on geometric objects") },
  { __L("$DIMASZ")                  , 1, { {  40, __L("")  } } ,  __L("Dimensioning arrow size") },
  { __L("$DIMATFIT")                , 1, { {  70, __L("")  } } ,  __L("Controls dimension text and arrow placement when space is not sufficient to place both within the extension lines: 0 = Places both text and arrows outside extension lines, 1 = Moves arrows first, then text, 2 = Moves text first, then arrows, 3 = Moves either text or arrows, whichever fits best, AutoCAD adds a leader to moved dimension text when DIMTMOVE is set to 1") },
  { __L("$DIMAUNIT")                , 1, { {  70, __L("")  } } ,  __L("Angle format for angular dimensions: 0 = Decimal degrees, 1 = Degrees/minutes/seconds;, 2 = Gradians, 3 = Radians, 4 = Surveyor's units") },
  { __L("$DIMAZIN")                 , 1, { {  70, __L("")  } } ,  __L("Controls suppression of zeros for angular dimensions: 0 = Displays all leading and trailing zeros, 1 = Suppresses leading zeros in decimal dimensions, 2 = Suppresses trailing zeros in decimal dimensions, 3 = Suppresses leading and trailing zeros") },
  { __L("$DIMBLK")                  , 1, { {   2, __L("")  } } ,  __L("Arrow block name") },
  { __L("$DIMBLK1")                 , 1, { {   1, __L("")  } } ,  __L("First arrow block name") },
  { __L("$DIMBLK2")                 , 1, { {   1, __L("")  } } ,  __L("Second arrow block name") },
  { __L("$DIMCEN")                  , 1, { {  40, __L("")  } } ,  __L("Size of center mark/lines") },
  { __L("$DIMCLRD")                 , 1, { {  70, __L("")  } } ,  __L("Dimension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { __L("$DIMCLRE")                 , 1, { {  70, __L("")  } } ,  __L("Dimension extension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { __L("$DIMCLRT")                 , 1, { {  70, __L("")  } } ,  __L("Dimension text color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { __L("$DIMDEC")                  , 1, { {  70, __L("")  } } ,  __L("Number of decimal places for the tolerance values of a primary units dimension") },
  { __L("$DIMDLE")                  , 1, { {  40, __L("")  } } ,  __L("Dimension line extension") },
  { __L("$DIMDLI")                  , 1, { {  40, __L("")  } } ,  __L("Dimension line increment") },
  { __L("$DIMDSEP")                 , 1, { {  70, __L("")  } } ,  __L("Single-character decimal separator used when creating dimensions whose unit format is decimal") },
  { __L("$DIMEXE")                  , 1, { {  40, __L("")  } } ,  __L("Extension line extension") },
  { __L("$DIMEXO")                  , 1, { {  40, __L("")  } } ,  __L("Extension line offset") },
  { __L("$DIMFAC")                  , 1, { {  40, __L("")  } } ,  __L("Scale factor used to calculate the height of text for dimension fractions and tolerances. AutoCAD multiplies DIMTXT by DIMTFAC to set the fractional or tolerance text height") },
  { __L("$DIMFRAC")                 , 1, { {  70, __L("")  } } ,  __L("Sets the fraction format when DIMLUNIT is set to Architectural or Fractional. Minimum AutoCAD version: R2000") },
  { __L("$DIMFIT")                  , 1, { {  70, __L("")  } } ,  __L("Placement of text and arrowheads. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { __L("$DIMGAP")                  , 1, { {  40, __L("")  } } ,  __L("Dimension line gap") }, 
  { __L("$DIMJUST")                 , 1, { {  70, __L("")  } } ,  __L("Horizontal dimension text position: 0 = Above dimension line and center-justified between extension lines, 1 = Above dimension line and next to first extension line, 2 = Above dimension line and next to second extension line, 3 = Above and center-justified to first extension line, 4 = Above and center-justified to second extension line") },
  { __L("$DIMLDRBLK")               , 1, { {   1, __L("")  } } ,  __L("Arrow block name for leaders") },
  { __L("$DIMLFAC")                 , 1, { {  40, __L("")  } } ,  __L("Linear measurements scale factor") },
  { __L("$DIMLIM")                  , 1, { {  70, __L("")  } } ,  __L("Dimension limits generated if nonzero") },
  { __L("$DIMLUNIT")                , 1, { {  70, __L("")  } } ,  __L("Sets units for all dimension types except Angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural, 5 = Fractional, 6 = Operating system") },
  { __L("$DIMLWD")                  , 1, { {  70, __L("")  } } ,  __L("Dimension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { __L("$DIMLWE")                  , 1, { {  70, __L("")  } } ,  __L("Extension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { __L("$DIMPOST")                 , 1, { {   1, __L("")  } } ,  __L("General dimensioning suffix") },
  { __L("$DIMRND")                  , 1, { {  40, __L("")  } } ,  __L("Rounding value for dimension distances") },
  { __L("$DIMSAH")                  , 1, { {  70, __L("")  } } ,  __L("Use separate arrow blocks if nonzero") },
  { __L("$DIMSCALE")                , 1, { {  40, __L("")  } } ,  __L("Overall dimensioning scale factor") },
  { __L("$DIMSD1")                  , 1, { {  70, __L("")  } } ,  __L("Suppression of first extension line: 0 = Not suppressed, 1 = Suppressed") },
  { __L("$DIMSD2")                  , 1, { {  70, __L("")  } } ,  __L("Suppression of second extension line: 0 = Not suppressed, 1 = Suppressed") },
  { __L("$DIMSE1")                  , 1, { {  70, __L("")  } } ,  __L("First extension line suppressed if nonzero ") },
  { __L("$DIMSE2")                  , 1, { {  70, __L("")  } } ,  __L("Second extension line suppressed if nonzero") },
  { __L("$DIMSHO")                  , 1, { {  70, __L("")  } } ,  __L("1 = recompute dimensions while dragging, 0 = drag original image") },
  { __L("$DIMSOXD")                 , 1, { {  70, __L("")  } } ,  __L("Suppress outside-extensions dimension lines if nonzero") },
  { __L("$DIMSTYLE")                , 1, { {   2, __L("")  } } ,  __L("Dimension style name") },
  { __L("$DIMTAD")                  , 1, { {  70, __L("")  } } ,  __L("Text above dimension line if nonzero") },
  { __L("$DIMTDEC")                 , 1, { {  70, __L("")  } } ,  __L("Number of decimal places to display the tolerance values") },
  { __L("$DIMTFAC")                 , 1, { {  40, __L("")  } } ,  __L("Dimension tolerance display scale factor") },
  { __L("$DIMTIH")                  , 1, { {  70, __L("")  } } ,  __L("Text inside horizontal if nonzero") },
  { __L("$DIMTIX")                  , 1, { {  70, __L("")  } } ,  __L("Force text inside extensions if nonzero") },
  { __L("$DIMTM")                   , 1, { {  40, __L("")  } } ,  __L("Minus tolerance") },
  { __L("$DIMTMOVE")                , 1, { {  70, __L("")  } } ,  __L("Dimension text movement rules: 0 = Moves the dimension line with dimension text, 1 = Adds a leader when dimension text is moved, 2 = Allows text to be moved freely without a leader") },
  { __L("$DIMTOFL")                 , 1, { {  70, __L("")  } } ,  __L("If text outside extensions, force line extensions between extensions if nonzero") },
  { __L("$DIMTOH")                  , 1, { {  70, __L("")  } } ,  __L("Text outside horizontal if nonzero") },
  { __L("$DIMTOL")                  , 1, { {  70, __L("")  } } ,  __L("Dimension tolerances generated if nonzero") },
  { __L("$DIMTOLJ")                 , 1, { {  70, __L("")  } } ,  __L("Vertical justification for tolerance values: 0 = Top, 1 = Middle, 2 = Bottom") },
  { __L("$DIMTP")                   , 1, { {  40, __L("")  } } ,  __L("Plus tolerance") },
  { __L("$DIMTSZ")                  , 1, { {  40, __L("")  } } ,  __L("Dimensioning tick size; 0 = no ticks") },
  { __L("$DIMTVP")                  , 1, { {  40, __L("")  } } ,  __L("Text vertical position") },
  { __L("$DIMTXSTY")                , 1, { {   7, __L("")  } } ,  __L("Dimension text style") },
  { __L("$DIMTXT")                  , 1, { {  40, __L("")  } } ,  __L("Dimensioning text height") },
  { __L("$DIMTZIN")                 , 1, { {  70, __L("")  } } ,  __L("Controls suppression of zeros for tolerance values: 0 = Suppresses zero feet and precisely zero inches,1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { __L("$DIMUPT")                  , 1, { {  70, __L("")  } } ,  __L("Cursor functionality for user-positioned text: 0 = Controls only the dimension line location, 1 = Controls the text position as well as the dimension line location") },
  { __L("$DIMZIN")                  , 1, { {  70, __L("")  } } ,  __L("Controls suppression of zeros for primary unit values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes, zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { __L("$DIMFXL")                  , 1, { {  40, __L("")  } } ,  __L("Sets the total length of the extension lines starting from the dimension line toward the dimension origin. Minimum AutoCAD version: R2007") },
  { __L("$DIMFXLON")                , 1, { {  70, __L("")  } } ,  __L("Controls whether extension lines are set to a fixed length. Minimum AutoCAD version: R2007") },
  { __L("$DIMJOGANG")               , 1, { {  40, __L("")  } } ,  __L("Determines the angle of the transverse segment of the dimension line in a jogged radius dimension. Minimum AutoCAD version: R2007") },
  { __L("$DIMTFILL")                , 1, { {  70, __L("")  } } ,  __L("Controls the background of dimension text. Minimum AutoCAD version: R2007") },
  { __L("$DIMTFILLCLR")             , 1, { {  70, __L("")  } } ,  __L("Sets the color for the text background in dimensions. Minimum AutoCAD version: R2007") },
  { __L("$DIMARCSYM")               , 1, { {  70, __L("")  } } ,  __L("Controls the display of the arc symbol in an arc length dimension. Minimum AutoCAD version: R2007") },
  { __L("$DIMLTYPE")                , 1, { {   6, __L("")  } } ,  __L("Sets the line type of the dimension line. Minimum AutoCAD version: R2007") },
  { __L("$DIMLTEX1")                , 1, { {   6, __L("")  } } ,  __L("Sets the line type of the first extension line. Minimum AutoCAD version: R2007") },
  { __L("$DIMLTEX2")                , 1, { {   6, __L("")  } } ,  __L("Sets the line type of the second extension line. Minimum AutoCAD version: R2007") },
  { __L("$DIMTXTDIRECTION")         , 1, { {  70, __L("")  } } ,  __L("Specifies the reading direction of the dimension text. Minimum AutoCAD version: R2010") },  
  { __L("$DIMUNIT")                 , 1, { {  70, __L("")  } } ,  __L("Units format for all dimension style family members except angular. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { __L("$DISPSILH")                , 1, { {  70, __L("")  } } ,  __L("Controls the display of silhouette curves of body objects in Wireframe mode: 0 = Off, 1 = On") },
  { __L("$DGNFRAME")                , 1, { { 280, __L("")  } } ,  __L("Determines whether DGN underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { __L("$DRAGVS")                  , 1, { { 349, __L("")  } } ,  __L("Hard-pointer ID to visual style while creating 3D solid primitives. The default value is NULL") },
  { __L("$DWFFRAME")                , 1, { { 280, __L("")  } } ,  __L("Determines whether DWF or DWFx underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { __L("$DWGCODEPAGE")             , 1, { {   3, __L("")  } } ,  __L("Drawing code page. Set to the system code page when a GEN_NEW drawing is created, but not otherwise maintained by AutoCAD") },
  { __L("$DRAGMODE")                , 1, { {  70, __L("")  } } ,  __L("0 = off, 1 = on, 2 = auto") },
  { __L("$ELEVATION")               , 1, { {  40, __L("")  } } ,  __L("Current elevation set by ELEV command") },
  { __L("$ENDCAPS")                 , 1, { { 280, __L("")  } } ,  __L("Lineweight endcaps setting for GEN_NEW objects: 0 = None, 1 = Round, 2 = Angle, 3 = Square") },
  { __L("$EXTMAX")                  , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("X, Y, and Z drawing extents upper-right corner (in WCS)") },
  { __L("$EXTMIN")                  , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("X, Y, and Z drawing extents lower-left corner (in WCS)") },
  { __L("$EXTNAMES")                , 1, { { 290, __L("")  } } ,  __L("Controls symbol table naming: 0 = Release 14 compatibility. Limits names to 31 characters in length. Names can include the letters A to Z, the numerals 0 to 9, and the special characters dollar sign ($), underscore (_), and hyphen (-)., 1 = AutoCAD 2000. Names can be up to 255 characters in length, and can include the letters A to Z, the numerals 0 to 9, spaces, and any special characters not used for other purposes by Microsoft Windows and AutoCAD") },
  { __L("$FILLETRAD")               , 1, { {  40, __L("")  } } ,  __L("Fillet radius") },
  { __L("$FILLMODE")                , 1, { {  70, __L("")  } } ,  __L("Fill mode on if nonzero") },
  { __L("$FINGERPRINTGUID")         , 1, { {   2, __L("")  } } ,  __L("Set at creation time, uniquely identifies a particular drawing") },
  { __L("$HALOGAP")                 , 1, { { 280, __L("")  } } ,  __L("Specifies a gap to be displayed where an object is hidden by another object; the value is specified as a percent of one unit and is independent of the zoom level. A haloed line is shortened at the point where it is hidden when HIDE or the Hidden option of SHADEMODE is used") },
  { __L("$HANDLING")                , 1, { {  70, __L("")  } } ,  __L("Handles enabled if nonzero") },
  { __L("$HANDSEED")                , 1, { {   5, __L("")  } } ,  __L("Next available handle") },
  { __L("$HIDETEXT")                , 1, { { 290, __L("")  } } ,  __L("Specifies HIDETEXT system variable: 0 = HIDE ignores text objects when producing the hidden view, 1 = HIDE does not ignore text objects") },
  { __L("$HYPERLINKBASE")           , 1, { {   1, __L("")  } } ,  __L("Path for all relative hyperlinks in the drawing. If null, the drawing path is used") },
  { __L("$INDEXCTL")                , 1, { { 280, __L("")  } } ,  __L("Controls whether layer and spatial indexes are created and saved in drawing files: 0 = No indexes are created, 1 = Layer index is created, 2 = Spatial index is created, 3 = Layer and spatial indexes are created") },
  { __L("$INSBASE")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Insertion base set by BASE command (in WCS)") },
  { __L("$INSUNITS")                , 1, { {  70, __L("")  } } ,  __L("Default drawing units for AutoCAD DesignCenter blocks: 0 = Unitless, 1 = Inches, 2 = Feet, 3 = Miles, 4 = Millimeters, 5 = Centimeters, 6 = Meters, 7 = Kilometers, 8 = Microinches, 9 = Mils, 10 = Yards, 11 = Angstroms, 12 = Nanometers, 13 = Microns, 14 = Decimeters, 15 = Decameters, 16 = Hectometers, 17 = Gigameters, 18 = Astronomical units, 19 = Light years, 20 = Parsecs, 21 = US Survey Feet, 22 = US Survey Inch, 23 = US Survey Yard, 24 = US Survey Mile") },
  { __L("$INTERFERECOLOR")          , 1, { {  62, __L("")  } } ,  __L("Represents the ACI color index of the 'interference objects' created during the INTERFERE command. Default value is 1") },
  { __L("$INTERFEREOBJVS")          , 1, { { 345, __L("")  } } ,  __L("Hard-pointer ID to the visual style for interference objects. Default visual style is Conceptual") },
  { __L("$INTERFEREVPVS")           , 1, { { 346, __L("")  } } ,  __L("Hard-pointer ID to the visual style for the viewport during interference checking. Default visual style is 3d Wireframe") },
  { __L("$INTERSECTIONCOLOR")       , 1, { {  70, __L("")  } } ,  __L("Specifies the entity color of intersection polylines: Values 1-255 designate an AutoCAD color index (ACI) 0 = Color BYBLOCK, 256 = Color BYLAYER, 257 = Color BYENTITY") },
  { __L("$INTERSECTIONDISPLAY")     , 1, { { 290, __L("")  } } ,  __L("Specifies the display of intersection polylines: 0 = Turns off the display of intersection polylines 1 = Turns on the display of intersection polylines") },
  { __L("$JOINSTYLE")               , 1, { { 280, __L("")  } } ,  __L("Lineweight joint setting for GEN_NEW objects: 0=None 1= Round 2 = Angle 3 = Flat") },
  { __L("$LATITUDE")                , 1, { {  40, __L("")  } } ,  __L("The latitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { __L("$LASTSAVEDBY")             , 1, { {   1, __L("")  } } ,  __L("Name of the last user to modify the file. Minimum AutoCAD version: R2004") },  
  { __L("$LENSLENGTH")              , 1, { {  40, __L("")  } } ,  __L("Stores the length of the lens in millimeters used in perspective viewing. Minimum AutoCAD version: R2007") },
  { __L("$LIGHTGLYPHDISPLAY")       , 1, { { 280, __L("")  } } ,  __L("Turns on and off the display of light glyphs. Minimum AutoCAD version: R2007") },
  { __L("$LIMCHECK")                , 1, { {  70, __L("")  } } ,  __L("Nonzero if limits checking is on") },
  { __L("$LIMMAX")                  , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("XY drawing limits upper-right corner (in WCS)") },
  { __L("$LIMMIN")                  , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("XY drawing limits lower-left corner (in WCS)") },
  { __L("$LOFTANG1")                , 1, { {  40, __L("")  } } ,  __L("Sets the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { __L("$LOFTANG2")                , 1, { {  40, __L("")  } } ,  __L("Sets the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { __L("$LOFTMAG1")                , 1, { {  40, __L("")  } } ,  __L("Sets the magnitude of the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { __L("$LOFTMAG2")                , 1, { {  40, __L("")  } } ,  __L("Sets the magnitude of the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { __L("$LOFTPARAM")               , 1, { {  70, __L("")  } } ,  __L("Controls the shape of lofted solids and surfaces. Minimum AutoCAD version: R2007") },
  { __L("$LOFTNORMALS")             , 1, { { 280, __L("")  } } ,  __L("Controls the normals of a lofted object where it passes through cross sections. Minimum AutoCAD version: R2007") },
  { __L("$LONGITUDE")               , 1, { {  40, __L("")  } } ,  __L("The longitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { __L("$LTSCALE")                 , 1, { {  40, __L("")  } } ,  __L("Global linetype scale") },
  { __L("$LUNITS")                  , 1, { {  70, __L("")  } } ,  __L("Units format for coordinates and distances") },
  { __L("$LUPREC")                  , 1, { {  70, __L("")  } } ,  __L("Units precision for coordinates and distances") },
  { __L("$LWDISPLAY")               , 1, { { 290, __L("")  } } ,  __L("Controls the display of lineweights on the Model or Layout tab: 0 = Lineweight is not displayed, 1 = Lineweight is displayed") },
  { __L("$MAXACTVP")                , 1, { {  70, __L("")  } } ,  __L("Sets maximum number of viewports to be regenerated") },
  { __L("$MEASUREMENT")             , 1, { {  70, __L("")  } } ,  __L("Sets drawing units: 0 = English 1 = Metric") },
  { __L("$MENU")                    , 1, { {   1, __L("")  } } ,  __L("Name of menu file") },
  { __L("$MIRRTEXT")                , 1, { {  70, __L("")  } } ,  __L("Mirror text if nonzero") },
  { __L("$NORTHDIRECTION")          , 1, { {  40, __L("")  } } ,  __L("Specifies the angle between the Y axis of WCS and the grid north. Minimum AutoCAD version: R2007") },
  { __L("$OBSCOLOR")                , 1, { {  70, __L("")  } } ,  __L("Specifies the color of obscured lines. An obscured line is a hidden line made visible by changing its color and linetype and is visible only when the HIDE or SHADEMODE command is used. The OBSCUREDCOLOR setting is visible only if the OBSCUREDLTYPE is turned ON by setting it to a value other than 0. 0 and 256 = Entity color 1-255 = An AutoCAD color index (ACI)") },
  { __L("$OBSLTYPE")                , 1, { { 280, __L("")  } } ,  __L("Specifies the linetype of obscured lines. Obscured linetypes are independent of zoom level, unlike regular AutoCAD linetypes. Value 0 turns off display of obscured lines and is the default. Linetype values are defined as follows: 0 = Off, 1 = Solid, 2 = Dashed, 3 = Dotted, 4 = Short Dash, 5 = Medium Dash, 6 = Long Dash, 7 = Double Short Dash, 8 = Double Medium Dash, 9 = Double Long Dash, 10 = Medium Long Dash, 11 = Sparse Dot") },  
  { __L("$OLESTARTUP")              , 1, { { 290, __L("")  } } ,  __L("Controls whether the source application of an embedded OLE object loads when plotting. Minimum AutoCAD version: R2000") },
  { __L("$ORTHOMODE")               , 1, { {  70, __L("")  } } ,  __L("Ortho mode on if nonzero") },
  { __L("$OSMODE")                  , 1, { {  70, __L("")  } } ,  __L("Running object snap modes") },
  { __L("$PDMODE")                  , 1, { {  70, __L("")  } } ,  __L("Point display mode") },
  { __L("$PDSIZE")                  , 1, { {  40, __L("")  } } ,  __L("Point display size") },
  { __L("$PELEVATION")              , 1, { {  40, __L("")  } } ,  __L("Current paper space elevation") },
  { __L("$PEXTMAX")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Maximum X, Y, and Z extents for paper space") },
  { __L("$PEXTMIN")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Minimum X, Y, and Z extents for paper space") },
  { __L("$PICKSTYLE")               , 1, { {  70, __L("")  } } ,  __L("Controls the group selection and associative hatch selection. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { __L("$PINSBASE")                , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Paper space insertion base point") },
  { __L("$PLIMCHECK")               , 1, { {  70, __L("")  } } ,  __L("Limits checking in paper space when nonzero") },
  { __L("$PLIMMAX")                 , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Maximum X and Y limits in paper space") },
  { __L("$PLIMMIN")                 , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Minimum X and Y limits in paper space") },
  { __L("$PLINEGEN")                , 1, { {  70, __L("")  } } ,  __L("Governs the generation of linetype patterns around the vertices of a 2D Polyline 1 = linetype is generated in a continuous pattern around vertices of the Polyline 0 = each segment of the Polyline starts and ends with a dash") },
  { __L("$PLINEWID")                , 1, { {  40, __L("")  } } ,  __L("Default Polyline width") },
  { __L("$PROJECTNAME")             , 1, { {   1, __L("")  } } ,  __L("Assigns a project name to the current drawing. Used when an external reference or image is not found on its original path. The project name points to a section in the registry that can contain one or more search paths for each project name defined. Project names and their search directories are created from the Files tab of the Options dialog box") },
  { __L("$PROXYGRAPHICS")           , 1, { {  70, __L("")  } } ,  __L("Controls the saving of proxy object images") },
  { __L("$PSOLHEIGHT")              , 1, { {  40, __L("")  } } ,  __L("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { __L("$PSOLHEIGHT")              , 1, { {  40, __L("")  } } ,  __L("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { __L("$PSOLWIDTH")               , 1, { {  40, __L("")  } } ,  __L("Controls the default width for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { __L("$PSLTSCALE")               , 1, { {  70, __L("")  } } ,  __L("Controls paper space linetype scaling 1 = no special linetype scaling 0 = viewport scaling governs linetype scaling") },
  { __L("$PSTYLEMODE")              , 1, { { 290, __L("")  } } ,  __L("Indicates whether the current drawing is in a Color-Dependent or Named Plot Style mode: 0 = Uses named plot style tables in the current drawing, 1 = Uses color-dependent plot style tables in the current drawing") },
  { __L("$PSVPSCALE")               , 1, { {  40, __L("")  } } ,  __L("View scale factor for GEN_NEW viewports: 0 = Scaled to fit >0 = Scale factor (a positive real value)") },
  { __L("$PUCSBASE")                , 1, { {   2, __L("")  } } ,  __L("Name of the UCS that defines the origin and orientation of orthographic UCS settings (paper space only)") },
  { __L("$PUCSNAME")                , 1, { {   2, __L("")  } } ,  __L("Current paper space UCS name") },
  { __L("$PUCSORG")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Current paper space UCS origin") },
  { __L("$PUCSORGBACK")             , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BACK when PUCSBASE is set to WORLD") },
  { __L("$PUCSORGBOTTOM")           , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BOTTOM when PUCSBASE is set to WORLD") },
  { __L("$PUCSORGFRONT")            , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to FRONT when PUCSBASE is set to WORLD") },
  { __L("$PUCSORGLEFT")             , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to LEFT when PUCSBASE is set to WORLD") },
  { __L("$PUCSORGRIGHT")            , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to RIGHT when PUCSBASE is set to WORLD") },
  { __L("$PUCSORGTOP")              , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to TOP when PUCSBASE is set to WORLD") },
  { __L("$PUCSORTHOREF")            , 1, { {   2, __L("")  } } ,  __L("If paper space UCS is orthographic (PUCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { __L("$PUCSORTHOVIEW")           , 1, { {  70, __L("")  } } ,  __L("Orthographic view type of paper space UCS: 0 = UCS is not orthographic,1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { __L("$PUCSXDIR")                , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Current paper space UCS X axis") },
  { __L("$PUCSYDIR")                , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Current paper space UCS Y axis") },
  { __L("$QTEXTMODE")               , 1, { {  70, __L("")  } } ,  __L("Quick text mode on if nonzero") },
  { __L("$REALWORLDSCALE")          , 1, { { 290, __L("")  } } ,  __L("Drawing is scaled to the real world. Minimum AutoCAD version: R2007") },
  { __L("$REGENMODE")               , 1, { {  70, __L("")  } } ,  __L("REGENAUTO mode on if nonzero") },
  { __L("$REQUIREDVERSIONS")        , 1, { { 160, __L("")  } } ,  __L("Unknown. Minimum AutoCAD version: R2013.") },
  { __L("$SHADEDGE")                , 1, { {  70, __L("")  } } ,  __L("0 = faces shaded, edges not highlighted 1 = faces shaded, edges highlighted in black 2 = faces not filled, edges in entity color 3 = faces in entity color, edges in black") },
  { __L("$SHADEDIF")                , 1, { {  70, __L("")  } } ,  __L("Percent ambient-diffuse light, range 1-100, default 70") },
  { __L("$SHADOWPLANELOCATION")     , 1, { {  40, __L("")  } } ,  __L("Location of the ground shadow plane. This is a Z axis ordinate") },
  { __L("$SHOWHIST")                , 1, { { 280, __L("")  } } ,  __L("Controls the Show History property for solids in a drawing. Minimum AutoCAD version: R2007") },
  { __L("$SKETCHINC")               , 1, { {  40, __L("")  } } ,  __L("Sketch record increment") },
  { __L("$SKPOLY")                  , 1, { {  70, __L("")  } } ,  __L("0 = sketch lines, 1 = sketch polylines") },
  { __L("$STEPSPERSEC")             , 1, { {  40, __L("")  } } ,  __L("Specifies the number of steps taken per second when you are in walk or fly mode. Minimum AutoCAD version: R2007") },
  { __L("$STEPSIZE")                , 1, { {  40, __L("")  } } ,  __L("Specifies the size of each step when in walk or fly mode, in drawing units. Minimum AutoCAD version: R2007") },
  { __L("$STYLESHEET")              , 1, { {   1, __L("")  } } ,  __L("Path to the stylesheet for the drawing. Minimum AutoCAD version: R2000") },
  { __L("$SOLIDHIST")               , 1, { { 280, __L("")  } } ,  __L("Controls whether GEN_NEW composite solids retain a history of their original components. Minimum AutoCAD version: R2007") },
  { __L("$SORTENTS")                , 1, { { 280, __L("")  } } ,  __L("Controls the object sorting methods; accessible from the Options dialog box User Preferences tab. SORTENTS uses the following bitcodes: 0 = Disables SORTENTS 1 = Sorts for object selection, 2 = Sorts for object snap, 4 = Sorts for redraws; obsolete, 8 = Sorts for MSLIDE command slide creation; obsolete, 16 = Sorts for REGEN commands 32 = Sorts for plotting, 64 = Sorts for PostScript output; obsolete") },
  { __L("$SPLFRAME")                , 1, { {  70, __L("")  } } ,  __L("Spline control polygon display; 1 = on, 0 = off") },  
  { __L("$SPLINESEGS")              , 1, { {  70, __L("")  } } ,  __L("Number of line segments per spline patch") },
  { __L("$SPLINETYPE")              , 1, { {  70, __L("")  } } ,  __L("Spline curve type for PEDIT Spline") },
  { __L("$SURFTAB1")                , 1, { {  70, __L("")  } } ,  __L("Number of mesh tabulations in first direction") },
  { __L("$SURFTAB2")                , 1, { {  70, __L("")  } } ,  __L("Number of mesh tabulations in second direction") },
  { __L("$SURFTYPE")                , 1, { {  70, __L("")  } } ,  __L("Surface type for PEDIT Smooth") },
  { __L("$SURFU")                   , 1, { {  70, __L("")  } } ,  __L("Surface density (for PEDIT Smooth) in M direction") },
  { __L("$SURFV")                   , 1, { {  70, __L("")  } } ,  __L("Surface density (for PEDIT Smooth) in N direction") },
  { __L("$TDCREATE")                , 1, { {  40, __L("")  } } ,  __L("Date/time of drawing creation") },
  { __L("$TDINDWG")                 , 1, { {  40, __L("")  } } ,  __L("Cumulative editing time for this drawing") },
  { __L("$TDUCREATE")               , 1, { {  40, __L("")  } } ,  __L("Universal date/time the drawing was created (see Special Handling of Date/Time Variables)") },
  { __L("$TDUPDATE")                , 1, { {  40, __L("")  } } ,  __L("Date/time of last drawing update") },
  { __L("$TDUSRTIMER")              , 1, { {  40, __L("")  } } ,  __L("User elapsed timer") },
  { __L("$TDUUPDATE")               , 1, { {  40, __L("")  } } ,  __L("Universal date/time of the last update/save (see Special Handling of Date/Time Variables)") }, 
  { __L("$TEXTSIZE")                , 1, { {  40, __L("")  } } ,  __L("Default text height") },
  { __L("$TEXTSTYLE")               , 1, { {   7, __L("")  } } ,  __L("Current text style name") },
  { __L("$THICKNESS")               , 1, { {  40, __L("")  } } ,  __L("Current thickness set by ELEV command") },
  { __L("$TILEMODELIGHTSYNCH")      , 1, { { 280, __L("")  } } ,  __L("Unknown. Minimum AutoCAD version: R2007") },
  { __L("$TIMEZONE")                , 1, { {  70, __L("")  } } ,  __L("Sets the time zone for the sun in the drawing. Minimum AutoCAD version: R2007") },
  { __L("$TILEMODE")                , 1, { {  70, __L("")  } } ,  __L("1 for previous release compatibility mode, 0 otherwise") },
  { __L("$TILEMODELIGHTSYNCH")      , 1, { { 280, __L("")  } } ,  __L("Unknown. Minimum AutoCAD version: R2007") },
  { __L("$TRACEWID")                , 1, { {  40, __L("")  } } ,  __L("Default Trace width") },
  { __L("$TREEDEPTH")               , 1, { {  70, __L("")  } } ,  __L("Specifies the maximum depth of the spatial index") },
  { __L("$UCSBASE")                 , 1, { {   2, __L("")  } } ,  __L("Name of the UCS that defines the origin and orientation of orthographic UCS settings") },
  { __L("$UCSNAME")                 , 1, { {   2, __L("")  } } ,  __L("Name of current UCS") },
  { __L("$UCSORG")                  , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Origin of current UCS (in WCS)") },
  { __L("$UCSORGBACK")              , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BACK when UCSBASE is set to WORLD") },
  { __L("$UCSORGBOTTOM")            , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BOTTOM when UCSBASE is set to WORLD") },
  { __L("$UCSORGFRONT")             , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to FRONT when UCSBASE is set to WORLD") },
  { __L("$UCSORGLEFT")              , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to LEFT when UCSBASE is set to WORLD") },
  { __L("$UCSORGRIGHT")             , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to RIGHT when UCSBASE is set to WORLD") },
  { __L("$UCSORGTOP")               , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to TOP when UCSBASE is set to WORLD") },
  { __L("$UCSORTHOREF")             , 1, { {   2, __L("")  } } ,  __L("If model space UCS is orthographic (UCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { __L("$UCSORTHOVIEW")            , 1, { {  70, __L("")  } } ,  __L("Orthographic view type of model space UCS: 0 = UCS is not orthographic, 1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { __L("$UCSXDIR")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Direction of current UCS's X axis (in WCS)") },
  { __L("$UCSYDIR")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Direction of current UCS's Y axis (in WCS)") },
  { __L("$UNITMODE")                , 1, { {  70, __L("")  } } ,  __L("Low bit set = display fractions, feet-and-inches, and surveyor's angles in input format") },
  { __L("$USERI1")                  , 1, { {  70, __L("")  } } ,  __L("1 For integer variables intended for use by third-party developers") },
  { __L("$USERI2")                  , 1, { {  70, __L("")  } } ,  __L("2 For integer variables intended for use by third-party developers") },
  { __L("$USERI3")                  , 1, { {  70, __L("")  } } ,  __L("3 For integer variables intended for use by third-party developers") },
  { __L("$USERI4")                  , 1, { {  70, __L("")  } } ,  __L("4 For integer variables intended for use by third-party developers") },
  { __L("$USERI5")                  , 1, { {  70, __L("")  } } ,  __L("5 For integer variables intended for use by third-party developers") },
  { __L("$USERR1")                  , 1, { {  40, __L("")  } } ,  __L("1 Five real variables intended for use by third-party developers") },
  { __L("$USERR2")                  , 1, { {  40, __L("")  } } ,  __L("2 Five real variables intended for use by third-party developers") },
  { __L("$USERR3")                  , 1, { {  40, __L("")  } } ,  __L("3 Five real variables intended for use by third-party developers") },
  { __L("$USERR4")                  , 1, { {  40, __L("")  } } ,  __L("4 Five real variables intended for use by third-party developers") },
  { __L("$USERR5")                  , 1, { {  40, __L("")  } } ,  __L("1 Five real variables intended for use by third-party developers") },        
  { __L("$USRTIMER")                , 1, { {  70, __L("")  } } ,  __L("0 = timer off, 1 = timer on") },
  { __L("$VERSIONGUID")             , 1, { {   2, __L("")  } } ,  __L("Uniquely identifies a particular version of a drawing. Updated when the drawing is modified") },
  { __L("$VISRETAIN")               , 1, { {  70, __L("")  } } ,  __L("0 = don't retain Xref-dependent visibility settings 1 = retain Xref-dependent visibility settings") },
  { __L("$WORLDVIEW")               , 1, { {  70, __L("")  } } ,  __L("1 = set UCS to WCS during DVIEW/VPOINT 0 = don't change UCS") },   
  { __L("$XCLIPFRAME")              , 1, { { 290, __L("")  } } ,  __L("Controls the visibility of xref clipping boundaries: 0 = Clipping boundary is not visible 1 = Clipping boundary is visible") },
  { __L("$XEDIT")                   , 1, { { 290, __L("")  } } ,  __L("Controls whether the current drawing can be edited in-place when being referenced by another drawing: 0 = Can't use in-place reference editing, 1 = Can use in-place reference editing") },

  // The following header variables existed before AutoCAD Release 11 but now have independent settings for each active viewport. OPEN honors these variables when read from DXF files. If a VPORT symbol table with *ACTIVE entries is present (as is true for any DXF file produced by Release 11 or later),
  // the values in the VPORT table entries override the values of these header variables.
   
  { __L("$FASTZOOM")                , 1, { {  70, __L("")  } } ,  __L("Fast zoom enabled if nonzero") },
  { __L("$GRIDMODE")                , 1, { {  70, __L("")  } } ,  __L("Grid mode on if nonzero") },
  { __L("$GRIDUNIT")                , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Grid X and Y spacing") },
  { __L("$SNAPANG")                 , 1, { {  50, __L("")  } } ,  __L("Snap grid rotation angle") },
  { __L("$SNAPBASE")                , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Snap/grid base point (in UCS)") },
  { __L("$SNAPISOPAIR")             , 1, { {  70, __L("")  } } ,  __L("Isometric plane; 0 = left, 1 = top. 2 = right") },
  { __L("$SNAPMODE")                , 1, { {  70, __L("")  } } ,  __L("Snap mode on if nonzero") },
  { __L("$SNAPSTYLE")               , 1, { {  70, __L("")  } } ,  __L("Snap style; 0 = standard, 1 = isometric") },
  { __L("$SNAPUNIT")                , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("Snap grid X and Y spacing") },
  { __L("$VIEWCTR")                 , 2, { {  10, __L("X") }, { 20, __L("Y") } } ,  __L("XY center of current view on screen") },
  { __L("$VIEWDIR")                 , 3, { {  10, __L("X") }, { 20, __L("Y") }, { 30, __L("Z") } } ,  __L("Viewing direction (direction from target, in WCS)") },
  { __L("$VIEWSIZE")                , 1, { {  40, __L("")  } } ,  __L("Height of view") }                             
}; 




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONHEADER::GRPVECTORFILEDXFTEXTSECTIONHEADER()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONHEADER::GRPVECTORFILEDXFTEXTSECTIONHEADER()
{
  Clean();

  type = GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONHEADER::~GRPVECTORFILEDXFTEXTSECTIONHEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONHEADER::~GRPVECTORFILEDXFTEXTSECTIONHEADER()
{
  DeleteAllVariables();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::IsKnownVariable(XSTRING& namevar)
* @brief      Is known variable
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::IsKnownVariable(XSTRING& namevar)
{  
  for(XDWORD c=0; c<GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNDEFVAR; c++)
    {
      GRPVECTORFILEDXFTEXTSECTIONHEADERDEFVARIABLE* variable = &GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[c];
      if(variable)
        {
          if(!namevar.Compare(variable->name, true)) 
            {
              return true;
            }    
        }
    }
  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::AddVariable(XCHAR* namevar, XVARIANT* variant)
* @brief      Add variable
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar pointer to use.
* @param[in]  variant : Variant pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::AddVariable(XCHAR* namevar, XVARIANT* variant)
{ 
  XSTRING* namevarStr = GEN_NEW XSTRING();
  if(!namevarStr) 
    {
      return false;
    }

  XVARIANT* variantCpy = GEN_NEW XVARIANT();
  if(!variantCpy) 
    {
      return false;
    }

  namevarStr->Add(namevar);

  (*variantCpy) = (*variant);

  variables.Add(namevarStr, variantCpy);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMAP<XSTRING*, XVARIANT*>* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariables()
* @brief      Get variables
* @ingroup    GRAPHIC
* 
* @return     XMAP<XSTRING*, XVARIANT*>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMAP<XSTRING*, XVARIANT*>* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariables()
{
  return &variables;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariable(XCHAR* namevar)
* @brief      Get variable
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar pointer to use.
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariable(XCHAR* namevar)
{
  if(variables.IsEmpty()) 
    {
      return NULL;
    }

  for(XDWORD c=0; c<variables.GetSize(); c++)
    {
      XSTRING* key = variables.GetKey(c);
      if(key)
        {
          if(key->Find(namevar, false) != XSTRING_NOTFOUND)
            {
              return variables.GetElement(c);               
            }
        }    
    }
  
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::DeleteVariable(XCHAR* namevar)
* @brief      Delete variable
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::DeleteVariable(XCHAR* namevar)
{
  if(variables.IsEmpty()) 
    {
      return false;
    }

   for(XDWORD c=0; c<variables.GetSize(); c++)
   {
      XSTRING* key = variables.GetKey(c);
      if(key)
      {
         if(key->Find(namevar, false) != XSTRING_NOTFOUND)
         {
            return variables.Delete(key);
         }
      }    
   }

   return false;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::DeleteAllVariables(bool withcontens)
* @brief      Delete all variables
* @ingroup    GRAPHIC
* 
* @param[in]  withcontens : Withcontens value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::DeleteAllVariables(bool withcontens)
{
  if(variables.IsEmpty()) 
    {
      return false;
    }

  variables.DeleteKeyContents();

  if(withcontens) 
    {
      variables.DeleteElementContents();
    }

  variables.DeleteAll();

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariableRemark(XSTRING& namevar)
* @brief      Get variable remark
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar value.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRPVECTORFILEDXFTEXTSECTIONHEADER::GetVariableRemark(XSTRING& namevar)
{
  for(XDWORD c=0; c<GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNDEFVAR; c++)
    {
      GRPVECTORFILEDXFTEXTSECTIONHEADERDEFVARIABLE* variable = &GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[c];
      if(variable)
        {
          if(namevar.Find(variable->name, true) != XSTRING_NOTFOUND) 
            {
              return variable->remark;
            }    
        }
    }
  
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::ParserVariable(XFILETXT* file, GRPVECTORFILEDXFTEXTPART* part, XCHAR* namevar, ...)
* @brief      Parser variable
* @ingroup    GRAPHIC
* 
* @param[in]  file : File object to use.
* @param[in]  part : Part pointer to use.
* @param[in]  namevar : Namevar pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::ParserVariable(XFILETXT* file, GRPVECTORFILEDXFTEXTPART* part, XCHAR* namevar, ...)
{   
  if(!part) 
    {
      return false;
    }
   
  if(!namevar) 
    {
      return false;
    }

  if(part->name.Compare(namevar, false))
    {
      return false;
    }

  int         type;
  XSTRING     extname;
  XSTRING*    namevarext  = NULL;
  XVARIANT*   variant     = NULL;
  
  va_list arg;

  va_start(arg, namevar);

  type        = (int)va_arg(arg, int);
  extname     = (XCHAR*)va_arg(arg, XCHAR*);
  namevarext  = (XSTRING*)va_arg(arg, XSTRING*);
  variant     = (XVARIANT*)va_arg(arg, XVARIANT*);
   
  va_end(arg);

  int indexline = 0;

  for(int c=0; c<GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNVAR; c++)
    {
      XSTRING* line = file->GetLine(part->iniline + indexline);
      GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

      indexline++;

      if(type == line->ConvertToInt())
        {
          line = file->GetLine(part->iniline + indexline);
          GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);  

          indexline++;

          namevarext->Format(__L("%s_%d"), part->name.Get(), type);

          if(!extname.IsEmpty())
            {
              namevarext->AddFormat(__L("_%s"), extname.Get());          
            }

          GetVariableFromLine(namevar, type, line, (*variant));         

          break;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTIONHEADER::ParserTextSection(XFILETXT* fileTXT)
* @brief      Parser text section
* @ingroup    GRAPHIC
* 
* @param[in]  fileTXT : File TXT pointer to use.
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTIONHEADER::ParserTextSection(XFILETXT* fileTXT)
{
  XVECTOR<GRPVECTORFILEDXFTEXTPART*> parts;
  GRPVECTORFILEDXFTEXTPART*          part  = NULL;  
  int                                c     = iniline;

  do{ if(!part)
        {
          part = GEN_NEW GRPVECTORFILEDXFTEXTPART ();
        }

      XSTRING* line = fileTXT->GetLine(c);
      if(!line) 
        {  
          c++;
          continue;
        }

      GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

      if(line && !line->Compare(__L("9"), true))
        {
          c++;
          line = fileTXT->GetLine(c);
          GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

          if(line->Get()[0]== __C('$'))
            {
              part->iniline = c+1;
              part->name = line->Get();

              if(parts.GetSize())
                {
                  GRPVECTORFILEDXFTEXTPART* partbefore = parts.Get(parts.GetSize()-1);
                  partbefore->endline = c-2;
                }

              parts.Add(part); 
              part = NULL;

              c++;
            } 

        } else c++;

    } while(c < endline);

  if(parts.GetSize())
    {
      GRPVECTORFILEDXFTEXTPART* partbefore = parts.Get(parts.GetSize()-1);
      partbefore->endline = c-1;
    }

  if(part && part->iniline == -1)
    {
      GEN_DELETE part;
      part = NULL;    
    }

  for(c=0; c<parts.GetSize(); c++)
    {     
      part = parts.Get(c);
      if(part)
        {
          if(!IsKnownVariable(part->name))
            {            
              XSTRING message;

              message.Format(__L("Header Variable Unknown [%s]"), part->name.Get());
                               
              GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

              vfevent.SetType(GRPVECTORFILETYPE_DXF);
              vfevent.GetPath()->Set(__L(""));
              vfevent.GetMsg()->Set(message);

              PostEvent(&vfevent, GetGrpVectorFile());          
            }
           else
            {
              XSTRING  namevar[GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNVAR];
              XVARIANT variant[GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNVAR];
       
              for(int d=0; d<GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNDEFVAR; d++)
                { 
                  for(int e=0; e<GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[d].nvalues; e++)
                    {
                      if(GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[d].value[e].valuetype)
                        {                     
                          ParserVariable (fileTXT, part, GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[d].name, GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[d].value[e].valuetype, GRPVECTORFILEDXFTEXTSECTIONHEADER::defvariable[d].value[e].valuename, &namevar[e], &variant[e]);
                        }
                    }                                           
                }
         
              for(int d=0; d<GRPVECTORFILEDXFTEXTSECTIONHEADER_MAXNVAR; d++)
                {
                  if((!namevar[d].IsEmpty()) && (variant->GetType() != XVARIANT_TYPE_NULL))
                    {
                      AddVariable(namevar[d].Get(), &variant[d]);                                                  
                    }         
                }
            }
        }  
    }

  parts.DeleteContents();
  parts.DeleteAll();

  //config.SetHeader(&header);

  #ifdef XTRACE_ACTIVE
  //ShowTraceAllVariables();
  #endif

  return GRPVECTORFILERESULT_OK;
}


#ifdef XTRACE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFTEXTSECTIONHEADER::ShowTraceAllVariables()
* @brief      Show trace all variables
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFTEXTSECTIONHEADER::ShowTraceAllVariables()
{
  for(XDWORD c=0; c<variables.GetSize(); c++)
    {
      XSTRING* key = variables.GetKey(c);
      if(key)
        {
          XVARIANT* value = variables.GetElement(c);
          if(value)
            {
              XSTRING string;

              value->ToString(string);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[GRPVECTORFILEDXFTEXTSECTIONHEADER] (%3d) var [%s] %s"), c, key->Get(), string.Get());
            }
        }    
    }

  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFTEXTSECTIONHEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFTEXTSECTIONHEADER::Clean()
{
  
}

