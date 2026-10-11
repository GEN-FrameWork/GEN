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
  { NL("$3DDWFPREC")               , 1, { {  40, NL("")  } } ,  NL("Controls the precision of 3D DWF or 3D DWFx publishing. Minimum AutoCAD version: R2007") },
  { NL("$ACADMAINTVER")            , 1, { {  70, NL("")  } } ,  NL("Maintenance version number (should be ignored)") },
  { NL("$ACADVER")                 , 1, { {   1, NL("")  } } ,  NL("The AutoCAD drawing database version number: AC1006 = R10, AC1009 = R11 and R12, AC1012 = R13, AC1014 = R14, AC1015 = AutoCAD 2000, AC1018 = AutoCAD 2004, AC1021 = AutoCAD 2007, AC1024 = AutoCAD 2010, AC1027 = AutoCAD 2013, AC1032 = AutoCAD 2018") },
  { NL("$ANGBASE")                 , 1, { {  50, NL("")  } } ,  NL("Angle 0 direction") },
  { NL("$ANGDIR")                  , 1, { {  70, NL("")  } } ,  NL("1 = clockwise angles, 0 = counterclockwise angles") },
  { NL("$ATTDIA")                  , 1, { {  70, NL("")  } } ,  NL("Attribute entry dialogs; 1 = on, 0 = off") },
  { NL("$ATTMODE")                 , 1, { {  70, NL("")  } } ,  NL("Attribute visibility; 0 = none, 1 = normal, 2 = all") },
  { NL("$ATTREQ")                  , 1, { {  70, NL("")  } } ,  NL("Attribute prompting during INSERT; 1 = on, 0 = off") },
  { NL("$AUNITS")                  , 1, { {  70, NL("")  } } ,  NL("Units format for angles") },
  { NL("$AUPREC")                  , 1, { {  70, NL("")  } } ,  NL("Units precision for angles") },
  { NL("$AXISMODE")                , 1, { {  70, NL("")  } } ,  NL("Axis on if nonzero (not functional in Release 12)") },
  { NL("$AXISUNIT")                , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Axis X and Y tick spacing (not functional in Release 12)") },
  { NL("$BLIPMODE")                , 1, { {  70, NL("")  } } ,  NL("Blip mode on if nonzero") },
  { NL("$CAMERADISPLAY")           , 1, { { 290, NL("")  } } ,  NL("Turns the display of camera objects on or off. Minimum AutoCAD version: R2007") },
  { NL("$CAMERAHEIGHT")            , 1, { {  40, NL("")  } } ,  NL("Specifies the default height for GEN_NEW camera objects. Minimum AutoCAD version: R2007") },     
  { NL("$CECOLOR")                 , 1, { {  62, NL("")  } } ,  NL("Current entity color number: 0 = BYBLOCK; 256 = BYLAYER") },
  { NL("$CELTSCALE")               , 1, { {  40, NL("")  } } ,  NL("Current entity linetype scale") },
  { NL("$CELTYPE")                 , 1, { {   6, NL("")  } } ,  NL("Entity linetype name, or BYBLOCK or BYLAYER") },
  { NL("$CELWEIGHT")               , 1, { { 370, NL("")  } } ,  NL("Lineweight of GEN_NEW objects") },
  { NL("$CEPSNID")                 , 1, { { 390, NL("")  } } ,  NL("Plotstyle handle of GEN_NEW objects; if CEPSNTYPE is 3, then this value indicates the handle") },
  { NL("$CEPSNTYPE")               , 1, { { 380, NL("")  } } ,  NL("Plot style type of GEN_NEW objects: 0 = Plot style by layer, 1 = Plot style by block, 2 = Plot style by dictionary default, 3 = Plot style by object ID/handle") },
  { NL("$CHAMFERA")                , 1, { {  40, NL("")  } } ,  NL("First chamfer distance") },
  { NL("$CHAMFERB")                , 1, { {  40, NL("")  } } ,  NL("Second chamfer distance") },
  { NL("$CHAMFERC")                , 1, { {  40, NL("")  } } ,  NL("Chamfer length") },
  { NL("$CHAMFERD")                , 1, { {  40, NL("")  } } ,  NL("Chamfer angle") },
  { NL("$CLAYER")                  , 1, { {   8, NL("")  } } ,  NL("Current layer name 11") },
  { NL("$COORDS")                  , 1, { {  70, NL("")  } } ,  NL("0 = static coordinate display, 1 = continuous update, 2 = 'd<a' format") },
  { NL("$CMATERIAL")               , 1, { { 280, NL("")  } } ,  NL("Sets the material of GEN_NEW objects. Minimum AutoCAD version: R2007") },
  { NL("$CMLJUST")                 , 1, { {  70, NL("")  } } ,  NL("Current multiline justification: 0 = Top; 1 = Middle; 2 = Bottom") },
  { NL("$CMLSCALE")                , 1, { {  40, NL("")  } } ,  NL("Current multiline scale") },
  { NL("$CMLSTYLE")                , 1, { {   2, NL("")  } } ,  NL("Current multiline style name") },
  { NL("$CSHADOW")                 , 1, { { 280, NL("")  } } ,  NL("Shadow mode for a 3D object: 0 = Casts and receives shadows, 1 = Casts shadows,, 2 = Receives shadows, 3 = Ignores shadows (Note: Starting with AutoCAD 2016-based products, this variable is obsolete but still supported for backwards compatibility)") },
  { NL("$DELOBJ")                  , 1, { {  70, NL("")  } } ,  NL("Controls object deletion. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { NL("$DIMADEC")                 , 1, { {  70, NL("")  } } ,  NL("Number of precision places displayed in angular dimensions") },
  { NL("$DIMALT")                  , 1, { {  70, NL("")  } } ,  NL("Alternate unt dimensioning performed if nonzero") },
  { NL("$DIMALTD")                 , 1, { {  70, NL("")  } } ,  NL("Alternate unit decimal places") },
  { NL("$DIMALTF")                 , 1, { {  40, NL("")  } } ,  NL("Alternate unit scale factor") },
  { NL("$DIMALTRND")               , 1, { {  40, NL("")  } } ,  NL("Determines rounding of alternate units") },
  { NL("$DIMALTTD")                , 1, { {  70, NL("")  } } ,  NL("Number of decimal places for tolerance values of an alternate units dimension") },
  { NL("$DIMALTTZ")                , 1, { {  70, NL("")  } } ,  NL("Controls suppression of zeros for alternate tolerance values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet-. To suppress leading or trailing zeros, add the following values to one of the preceding values: 4 = Suppresses leading zeros. 8 = Suppresses trailing zeros") },
  { NL("$DIMALTU")                 , 1, { {  70, NL("")  } } ,  NL("Units format for alternate units of all dimension style family members except angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural (stacked), 5 = Fractional (stacked), 6 = Architectural, 7 = Fractional, 8 = Operating system defines the decimal separator and number grouping symbols") },
  { NL("$DIMALTZ")                 , 1, { {  70, NL("")  } } ,  NL("Controls suppression of zeros for alternate unit dimension values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },  
  { NL("$DIMAPOST")                , 1, { {   1, NL("")  } } ,  NL("Alternate dimensioning suffix") },
  { NL("$DIMASO")                  , 1, { {  70, NL("")  } } ,  NL("1 = create associative dimensioning, 0 = draw individual entities") },
  { NL("$DIMASSOC")                , 1, { { 280, NL("")  } } ,  NL("Controls the associativity of dimension objects 0 = Creates exploded dimensions; there is no association between elements of the dimension, and the lines, arcs, arrowheads, and text of a dimension are drawn as separate objects, 1 = Creates non-associative dimension objects; the elements of the dimension are formed into a single object, and if the definition point on the object moves, then the dimension value is updated, 2 = Creates associative dimension objects; the elements of the dimension are formed into a single object and one or more definition points of the dimension are coupled with association points on geometric objects") },
  { NL("$DIMASZ")                  , 1, { {  40, NL("")  } } ,  NL("Dimensioning arrow size") },
  { NL("$DIMATFIT")                , 1, { {  70, NL("")  } } ,  NL("Controls dimension text and arrow placement when space is not sufficient to place both within the extension lines: 0 = Places both text and arrows outside extension lines, 1 = Moves arrows first, then text, 2 = Moves text first, then arrows, 3 = Moves either text or arrows, whichever fits best, AutoCAD adds a leader to moved dimension text when DIMTMOVE is set to 1") },
  { NL("$DIMAUNIT")                , 1, { {  70, NL("")  } } ,  NL("Angle format for angular dimensions: 0 = Decimal degrees, 1 = Degrees/minutes/seconds;, 2 = Gradians, 3 = Radians, 4 = Surveyor's units") },
  { NL("$DIMAZIN")                 , 1, { {  70, NL("")  } } ,  NL("Controls suppression of zeros for angular dimensions: 0 = Displays all leading and trailing zeros, 1 = Suppresses leading zeros in decimal dimensions, 2 = Suppresses trailing zeros in decimal dimensions, 3 = Suppresses leading and trailing zeros") },
  { NL("$DIMBLK")                  , 1, { {   2, NL("")  } } ,  NL("Arrow block name") },
  { NL("$DIMBLK1")                 , 1, { {   1, NL("")  } } ,  NL("First arrow block name") },
  { NL("$DIMBLK2")                 , 1, { {   1, NL("")  } } ,  NL("Second arrow block name") },
  { NL("$DIMCEN")                  , 1, { {  40, NL("")  } } ,  NL("Size of center mark/lines") },
  { NL("$DIMCLRD")                 , 1, { {  70, NL("")  } } ,  NL("Dimension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { NL("$DIMCLRE")                 , 1, { {  70, NL("")  } } ,  NL("Dimension extension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { NL("$DIMCLRT")                 , 1, { {  70, NL("")  } } ,  NL("Dimension text color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { NL("$DIMDEC")                  , 1, { {  70, NL("")  } } ,  NL("Number of decimal places for the tolerance values of a primary units dimension") },
  { NL("$DIMDLE")                  , 1, { {  40, NL("")  } } ,  NL("Dimension line extension") },
  { NL("$DIMDLI")                  , 1, { {  40, NL("")  } } ,  NL("Dimension line increment") },
  { NL("$DIMDSEP")                 , 1, { {  70, NL("")  } } ,  NL("Single-character decimal separator used when creating dimensions whose unit format is decimal") },
  { NL("$DIMEXE")                  , 1, { {  40, NL("")  } } ,  NL("Extension line extension") },
  { NL("$DIMEXO")                  , 1, { {  40, NL("")  } } ,  NL("Extension line offset") },
  { NL("$DIMFAC")                  , 1, { {  40, NL("")  } } ,  NL("Scale factor used to calculate the height of text for dimension fractions and tolerances. AutoCAD multiplies DIMTXT by DIMTFAC to set the fractional or tolerance text height") },
  { NL("$DIMFRAC")                 , 1, { {  70, NL("")  } } ,  NL("Sets the fraction format when DIMLUNIT is set to Architectural or Fractional. Minimum AutoCAD version: R2000") },
  { NL("$DIMFIT")                  , 1, { {  70, NL("")  } } ,  NL("Placement of text and arrowheads. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { NL("$DIMGAP")                  , 1, { {  40, NL("")  } } ,  NL("Dimension line gap") }, 
  { NL("$DIMJUST")                 , 1, { {  70, NL("")  } } ,  NL("Horizontal dimension text position: 0 = Above dimension line and center-justified between extension lines, 1 = Above dimension line and next to first extension line, 2 = Above dimension line and next to second extension line, 3 = Above and center-justified to first extension line, 4 = Above and center-justified to second extension line") },
  { NL("$DIMLDRBLK")               , 1, { {   1, NL("")  } } ,  NL("Arrow block name for leaders") },
  { NL("$DIMLFAC")                 , 1, { {  40, NL("")  } } ,  NL("Linear measurements scale factor") },
  { NL("$DIMLIM")                  , 1, { {  70, NL("")  } } ,  NL("Dimension limits generated if nonzero") },
  { NL("$DIMLUNIT")                , 1, { {  70, NL("")  } } ,  NL("Sets units for all dimension types except Angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural, 5 = Fractional, 6 = Operating system") },
  { NL("$DIMLWD")                  , 1, { {  70, NL("")  } } ,  NL("Dimension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { NL("$DIMLWE")                  , 1, { {  70, NL("")  } } ,  NL("Extension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { NL("$DIMPOST")                 , 1, { {   1, NL("")  } } ,  NL("General dimensioning suffix") },
  { NL("$DIMRND")                  , 1, { {  40, NL("")  } } ,  NL("Rounding value for dimension distances") },
  { NL("$DIMSAH")                  , 1, { {  70, NL("")  } } ,  NL("Use separate arrow blocks if nonzero") },
  { NL("$DIMSCALE")                , 1, { {  40, NL("")  } } ,  NL("Overall dimensioning scale factor") },
  { NL("$DIMSD1")                  , 1, { {  70, NL("")  } } ,  NL("Suppression of first extension line: 0 = Not suppressed, 1 = Suppressed") },
  { NL("$DIMSD2")                  , 1, { {  70, NL("")  } } ,  NL("Suppression of second extension line: 0 = Not suppressed, 1 = Suppressed") },
  { NL("$DIMSE1")                  , 1, { {  70, NL("")  } } ,  NL("First extension line suppressed if nonzero ") },
  { NL("$DIMSE2")                  , 1, { {  70, NL("")  } } ,  NL("Second extension line suppressed if nonzero") },
  { NL("$DIMSHO")                  , 1, { {  70, NL("")  } } ,  NL("1 = recompute dimensions while dragging, 0 = drag original image") },
  { NL("$DIMSOXD")                 , 1, { {  70, NL("")  } } ,  NL("Suppress outside-extensions dimension lines if nonzero") },
  { NL("$DIMSTYLE")                , 1, { {   2, NL("")  } } ,  NL("Dimension style name") },
  { NL("$DIMTAD")                  , 1, { {  70, NL("")  } } ,  NL("Text above dimension line if nonzero") },
  { NL("$DIMTDEC")                 , 1, { {  70, NL("")  } } ,  NL("Number of decimal places to display the tolerance values") },
  { NL("$DIMTFAC")                 , 1, { {  40, NL("")  } } ,  NL("Dimension tolerance display scale factor") },
  { NL("$DIMTIH")                  , 1, { {  70, NL("")  } } ,  NL("Text inside horizontal if nonzero") },
  { NL("$DIMTIX")                  , 1, { {  70, NL("")  } } ,  NL("Force text inside extensions if nonzero") },
  { NL("$DIMTM")                   , 1, { {  40, NL("")  } } ,  NL("Minus tolerance") },
  { NL("$DIMTMOVE")                , 1, { {  70, NL("")  } } ,  NL("Dimension text movement rules: 0 = Moves the dimension line with dimension text, 1 = Adds a leader when dimension text is moved, 2 = Allows text to be moved freely without a leader") },
  { NL("$DIMTOFL")                 , 1, { {  70, NL("")  } } ,  NL("If text outside extensions, force line extensions between extensions if nonzero") },
  { NL("$DIMTOH")                  , 1, { {  70, NL("")  } } ,  NL("Text outside horizontal if nonzero") },
  { NL("$DIMTOL")                  , 1, { {  70, NL("")  } } ,  NL("Dimension tolerances generated if nonzero") },
  { NL("$DIMTOLJ")                 , 1, { {  70, NL("")  } } ,  NL("Vertical justification for tolerance values: 0 = Top, 1 = Middle, 2 = Bottom") },
  { NL("$DIMTP")                   , 1, { {  40, NL("")  } } ,  NL("Plus tolerance") },
  { NL("$DIMTSZ")                  , 1, { {  40, NL("")  } } ,  NL("Dimensioning tick size; 0 = no ticks") },
  { NL("$DIMTVP")                  , 1, { {  40, NL("")  } } ,  NL("Text vertical position") },
  { NL("$DIMTXSTY")                , 1, { {   7, NL("")  } } ,  NL("Dimension text style") },
  { NL("$DIMTXT")                  , 1, { {  40, NL("")  } } ,  NL("Dimensioning text height") },
  { NL("$DIMTZIN")                 , 1, { {  70, NL("")  } } ,  NL("Controls suppression of zeros for tolerance values: 0 = Suppresses zero feet and precisely zero inches,1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { NL("$DIMUPT")                  , 1, { {  70, NL("")  } } ,  NL("Cursor functionality for user-positioned text: 0 = Controls only the dimension line location, 1 = Controls the text position as well as the dimension line location") },
  { NL("$DIMZIN")                  , 1, { {  70, NL("")  } } ,  NL("Controls suppression of zeros for primary unit values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes, zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { NL("$DIMFXL")                  , 1, { {  40, NL("")  } } ,  NL("Sets the total length of the extension lines starting from the dimension line toward the dimension origin. Minimum AutoCAD version: R2007") },
  { NL("$DIMFXLON")                , 1, { {  70, NL("")  } } ,  NL("Controls whether extension lines are set to a fixed length. Minimum AutoCAD version: R2007") },
  { NL("$DIMJOGANG")               , 1, { {  40, NL("")  } } ,  NL("Determines the angle of the transverse segment of the dimension line in a jogged radius dimension. Minimum AutoCAD version: R2007") },
  { NL("$DIMTFILL")                , 1, { {  70, NL("")  } } ,  NL("Controls the background of dimension text. Minimum AutoCAD version: R2007") },
  { NL("$DIMTFILLCLR")             , 1, { {  70, NL("")  } } ,  NL("Sets the color for the text background in dimensions. Minimum AutoCAD version: R2007") },
  { NL("$DIMARCSYM")               , 1, { {  70, NL("")  } } ,  NL("Controls the display of the arc symbol in an arc length dimension. Minimum AutoCAD version: R2007") },
  { NL("$DIMLTYPE")                , 1, { {   6, NL("")  } } ,  NL("Sets the line type of the dimension line. Minimum AutoCAD version: R2007") },
  { NL("$DIMLTEX1")                , 1, { {   6, NL("")  } } ,  NL("Sets the line type of the first extension line. Minimum AutoCAD version: R2007") },
  { NL("$DIMLTEX2")                , 1, { {   6, NL("")  } } ,  NL("Sets the line type of the second extension line. Minimum AutoCAD version: R2007") },
  { NL("$DIMTXTDIRECTION")         , 1, { {  70, NL("")  } } ,  NL("Specifies the reading direction of the dimension text. Minimum AutoCAD version: R2010") },  
  { NL("$DIMUNIT")                 , 1, { {  70, NL("")  } } ,  NL("Units format for all dimension style family members except angular. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { NL("$DISPSILH")                , 1, { {  70, NL("")  } } ,  NL("Controls the display of silhouette curves of body objects in Wireframe mode: 0 = Off, 1 = On") },
  { NL("$DGNFRAME")                , 1, { { 280, NL("")  } } ,  NL("Determines whether DGN underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { NL("$DRAGVS")                  , 1, { { 349, NL("")  } } ,  NL("Hard-pointer ID to visual style while creating 3D solid primitives. The default value is NULL") },
  { NL("$DWFFRAME")                , 1, { { 280, NL("")  } } ,  NL("Determines whether DWF or DWFx underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { NL("$DWGCODEPAGE")             , 1, { {   3, NL("")  } } ,  NL("Drawing code page. Set to the system code page when a GEN_NEW drawing is created, but not otherwise maintained by AutoCAD") },
  { NL("$DRAGMODE")                , 1, { {  70, NL("")  } } ,  NL("0 = off, 1 = on, 2 = auto") },
  { NL("$ELEVATION")               , 1, { {  40, NL("")  } } ,  NL("Current elevation set by ELEV command") },
  { NL("$ENDCAPS")                 , 1, { { 280, NL("")  } } ,  NL("Lineweight endcaps setting for GEN_NEW objects: 0 = None, 1 = Round, 2 = Angle, 3 = Square") },
  { NL("$EXTMAX")                  , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("X, Y, and Z drawing extents upper-right corner (in WCS)") },
  { NL("$EXTMIN")                  , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("X, Y, and Z drawing extents lower-left corner (in WCS)") },
  { NL("$EXTNAMES")                , 1, { { 290, NL("")  } } ,  NL("Controls symbol table naming: 0 = Release 14 compatibility. Limits names to 31 characters in length. Names can include the letters A to Z, the numerals 0 to 9, and the special characters dollar sign ($), underscore (_), and hyphen (-)., 1 = AutoCAD 2000. Names can be up to 255 characters in length, and can include the letters A to Z, the numerals 0 to 9, spaces, and any special characters not used for other purposes by Microsoft Windows and AutoCAD") },
  { NL("$FILLETRAD")               , 1, { {  40, NL("")  } } ,  NL("Fillet radius") },
  { NL("$FILLMODE")                , 1, { {  70, NL("")  } } ,  NL("Fill mode on if nonzero") },
  { NL("$FINGERPRINTGUID")         , 1, { {   2, NL("")  } } ,  NL("Set at creation time, uniquely identifies a particular drawing") },
  { NL("$HALOGAP")                 , 1, { { 280, NL("")  } } ,  NL("Specifies a gap to be displayed where an object is hidden by another object; the value is specified as a percent of one unit and is independent of the zoom level. A haloed line is shortened at the point where it is hidden when HIDE or the Hidden option of SHADEMODE is used") },
  { NL("$HANDLING")                , 1, { {  70, NL("")  } } ,  NL("Handles enabled if nonzero") },
  { NL("$HANDSEED")                , 1, { {   5, NL("")  } } ,  NL("Next available handle") },
  { NL("$HIDETEXT")                , 1, { { 290, NL("")  } } ,  NL("Specifies HIDETEXT system variable: 0 = HIDE ignores text objects when producing the hidden view, 1 = HIDE does not ignore text objects") },
  { NL("$HYPERLINKBASE")           , 1, { {   1, NL("")  } } ,  NL("Path for all relative hyperlinks in the drawing. If null, the drawing path is used") },
  { NL("$INDEXCTL")                , 1, { { 280, NL("")  } } ,  NL("Controls whether layer and spatial indexes are created and saved in drawing files: 0 = No indexes are created, 1 = Layer index is created, 2 = Spatial index is created, 3 = Layer and spatial indexes are created") },
  { NL("$INSBASE")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Insertion base set by BASE command (in WCS)") },
  { NL("$INSUNITS")                , 1, { {  70, NL("")  } } ,  NL("Default drawing units for AutoCAD DesignCenter blocks: 0 = Unitless, 1 = Inches, 2 = Feet, 3 = Miles, 4 = Millimeters, 5 = Centimeters, 6 = Meters, 7 = Kilometers, 8 = Microinches, 9 = Mils, 10 = Yards, 11 = Angstroms, 12 = Nanometers, 13 = Microns, 14 = Decimeters, 15 = Decameters, 16 = Hectometers, 17 = Gigameters, 18 = Astronomical units, 19 = Light years, 20 = Parsecs, 21 = US Survey Feet, 22 = US Survey Inch, 23 = US Survey Yard, 24 = US Survey Mile") },
  { NL("$INTERFERECOLOR")          , 1, { {  62, NL("")  } } ,  NL("Represents the ACI color index of the 'interference objects' created during the INTERFERE command. Default value is 1") },
  { NL("$INTERFEREOBJVS")          , 1, { { 345, NL("")  } } ,  NL("Hard-pointer ID to the visual style for interference objects. Default visual style is Conceptual") },
  { NL("$INTERFEREVPVS")           , 1, { { 346, NL("")  } } ,  NL("Hard-pointer ID to the visual style for the viewport during interference checking. Default visual style is 3d Wireframe") },
  { NL("$INTERSECTIONCOLOR")       , 1, { {  70, NL("")  } } ,  NL("Specifies the entity color of intersection polylines: Values 1-255 designate an AutoCAD color index (ACI) 0 = Color BYBLOCK, 256 = Color BYLAYER, 257 = Color BYENTITY") },
  { NL("$INTERSECTIONDISPLAY")     , 1, { { 290, NL("")  } } ,  NL("Specifies the display of intersection polylines: 0 = Turns off the display of intersection polylines 1 = Turns on the display of intersection polylines") },
  { NL("$JOINSTYLE")               , 1, { { 280, NL("")  } } ,  NL("Lineweight joint setting for GEN_NEW objects: 0=None 1= Round 2 = Angle 3 = Flat") },
  { NL("$LATITUDE")                , 1, { {  40, NL("")  } } ,  NL("The latitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { NL("$LASTSAVEDBY")             , 1, { {   1, NL("")  } } ,  NL("Name of the last user to modify the file. Minimum AutoCAD version: R2004") },  
  { NL("$LENSLENGTH")              , 1, { {  40, NL("")  } } ,  NL("Stores the length of the lens in millimeters used in perspective viewing. Minimum AutoCAD version: R2007") },
  { NL("$LIGHTGLYPHDISPLAY")       , 1, { { 280, NL("")  } } ,  NL("Turns on and off the display of light glyphs. Minimum AutoCAD version: R2007") },
  { NL("$LIMCHECK")                , 1, { {  70, NL("")  } } ,  NL("Nonzero if limits checking is on") },
  { NL("$LIMMAX")                  , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("XY drawing limits upper-right corner (in WCS)") },
  { NL("$LIMMIN")                  , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("XY drawing limits lower-left corner (in WCS)") },
  { NL("$LOFTANG1")                , 1, { {  40, NL("")  } } ,  NL("Sets the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { NL("$LOFTANG2")                , 1, { {  40, NL("")  } } ,  NL("Sets the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { NL("$LOFTMAG1")                , 1, { {  40, NL("")  } } ,  NL("Sets the magnitude of the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { NL("$LOFTMAG2")                , 1, { {  40, NL("")  } } ,  NL("Sets the magnitude of the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { NL("$LOFTPARAM")               , 1, { {  70, NL("")  } } ,  NL("Controls the shape of lofted solids and surfaces. Minimum AutoCAD version: R2007") },
  { NL("$LOFTNORMALS")             , 1, { { 280, NL("")  } } ,  NL("Controls the normals of a lofted object where it passes through cross sections. Minimum AutoCAD version: R2007") },
  { NL("$LONGITUDE")               , 1, { {  40, NL("")  } } ,  NL("The longitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { NL("$LTSCALE")                 , 1, { {  40, NL("")  } } ,  NL("Global linetype scale") },
  { NL("$LUNITS")                  , 1, { {  70, NL("")  } } ,  NL("Units format for coordinates and distances") },
  { NL("$LUPREC")                  , 1, { {  70, NL("")  } } ,  NL("Units precision for coordinates and distances") },
  { NL("$LWDISPLAY")               , 1, { { 290, NL("")  } } ,  NL("Controls the display of lineweights on the Model or Layout tab: 0 = Lineweight is not displayed, 1 = Lineweight is displayed") },
  { NL("$MAXACTVP")                , 1, { {  70, NL("")  } } ,  NL("Sets maximum number of viewports to be regenerated") },
  { NL("$MEASUREMENT")             , 1, { {  70, NL("")  } } ,  NL("Sets drawing units: 0 = English 1 = Metric") },
  { NL("$MENU")                    , 1, { {   1, NL("")  } } ,  NL("Name of menu file") },
  { NL("$MIRRTEXT")                , 1, { {  70, NL("")  } } ,  NL("Mirror text if nonzero") },
  { NL("$NORTHDIRECTION")          , 1, { {  40, NL("")  } } ,  NL("Specifies the angle between the Y axis of WCS and the grid north. Minimum AutoCAD version: R2007") },
  { NL("$OBSCOLOR")                , 1, { {  70, NL("")  } } ,  NL("Specifies the color of obscured lines. An obscured line is a hidden line made visible by changing its color and linetype and is visible only when the HIDE or SHADEMODE command is used. The OBSCUREDCOLOR setting is visible only if the OBSCUREDLTYPE is turned ON by setting it to a value other than 0. 0 and 256 = Entity color 1-255 = An AutoCAD color index (ACI)") },
  { NL("$OBSLTYPE")                , 1, { { 280, NL("")  } } ,  NL("Specifies the linetype of obscured lines. Obscured linetypes are independent of zoom level, unlike regular AutoCAD linetypes. Value 0 turns off display of obscured lines and is the default. Linetype values are defined as follows: 0 = Off, 1 = Solid, 2 = Dashed, 3 = Dotted, 4 = Short Dash, 5 = Medium Dash, 6 = Long Dash, 7 = Double Short Dash, 8 = Double Medium Dash, 9 = Double Long Dash, 10 = Medium Long Dash, 11 = Sparse Dot") },  
  { NL("$OLESTARTUP")              , 1, { { 290, NL("")  } } ,  NL("Controls whether the source application of an embedded OLE object loads when plotting. Minimum AutoCAD version: R2000") },
  { NL("$ORTHOMODE")               , 1, { {  70, NL("")  } } ,  NL("Ortho mode on if nonzero") },
  { NL("$OSMODE")                  , 1, { {  70, NL("")  } } ,  NL("Running object snap modes") },
  { NL("$PDMODE")                  , 1, { {  70, NL("")  } } ,  NL("Point display mode") },
  { NL("$PDSIZE")                  , 1, { {  40, NL("")  } } ,  NL("Point display size") },
  { NL("$PELEVATION")              , 1, { {  40, NL("")  } } ,  NL("Current paper space elevation") },
  { NL("$PEXTMAX")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Maximum X, Y, and Z extents for paper space") },
  { NL("$PEXTMIN")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Minimum X, Y, and Z extents for paper space") },
  { NL("$PICKSTYLE")               , 1, { {  70, NL("")  } } ,  NL("Controls the group selection and associative hatch selection. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { NL("$PINSBASE")                , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Paper space insertion base point") },
  { NL("$PLIMCHECK")               , 1, { {  70, NL("")  } } ,  NL("Limits checking in paper space when nonzero") },
  { NL("$PLIMMAX")                 , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Maximum X and Y limits in paper space") },
  { NL("$PLIMMIN")                 , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Minimum X and Y limits in paper space") },
  { NL("$PLINEGEN")                , 1, { {  70, NL("")  } } ,  NL("Governs the generation of linetype patterns around the vertices of a 2D Polyline 1 = linetype is generated in a continuous pattern around vertices of the Polyline 0 = each segment of the Polyline starts and ends with a dash") },
  { NL("$PLINEWID")                , 1, { {  40, NL("")  } } ,  NL("Default Polyline width") },
  { NL("$PROJECTNAME")             , 1, { {   1, NL("")  } } ,  NL("Assigns a project name to the current drawing. Used when an external reference or image is not found on its original path. The project name points to a section in the registry that can contain one or more search paths for each project name defined. Project names and their search directories are created from the Files tab of the Options dialog box") },
  { NL("$PROXYGRAPHICS")           , 1, { {  70, NL("")  } } ,  NL("Controls the saving of proxy object images") },
  { NL("$PSOLHEIGHT")              , 1, { {  40, NL("")  } } ,  NL("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { NL("$PSOLHEIGHT")              , 1, { {  40, NL("")  } } ,  NL("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { NL("$PSOLWIDTH")               , 1, { {  40, NL("")  } } ,  NL("Controls the default width for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { NL("$PSLTSCALE")               , 1, { {  70, NL("")  } } ,  NL("Controls paper space linetype scaling 1 = no special linetype scaling 0 = viewport scaling governs linetype scaling") },
  { NL("$PSTYLEMODE")              , 1, { { 290, NL("")  } } ,  NL("Indicates whether the current drawing is in a Color-Dependent or Named Plot Style mode: 0 = Uses named plot style tables in the current drawing, 1 = Uses color-dependent plot style tables in the current drawing") },
  { NL("$PSVPSCALE")               , 1, { {  40, NL("")  } } ,  NL("View scale factor for GEN_NEW viewports: 0 = Scaled to fit >0 = Scale factor (a positive real value)") },
  { NL("$PUCSBASE")                , 1, { {   2, NL("")  } } ,  NL("Name of the UCS that defines the origin and orientation of orthographic UCS settings (paper space only)") },
  { NL("$PUCSNAME")                , 1, { {   2, NL("")  } } ,  NL("Current paper space UCS name") },
  { NL("$PUCSORG")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Current paper space UCS origin") },
  { NL("$PUCSORGBACK")             , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BACK when PUCSBASE is set to WORLD") },
  { NL("$PUCSORGBOTTOM")           , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BOTTOM when PUCSBASE is set to WORLD") },
  { NL("$PUCSORGFRONT")            , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to FRONT when PUCSBASE is set to WORLD") },
  { NL("$PUCSORGLEFT")             , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to LEFT when PUCSBASE is set to WORLD") },
  { NL("$PUCSORGRIGHT")            , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to RIGHT when PUCSBASE is set to WORLD") },
  { NL("$PUCSORGTOP")              , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to TOP when PUCSBASE is set to WORLD") },
  { NL("$PUCSORTHOREF")            , 1, { {   2, NL("")  } } ,  NL("If paper space UCS is orthographic (PUCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { NL("$PUCSORTHOVIEW")           , 1, { {  70, NL("")  } } ,  NL("Orthographic view type of paper space UCS: 0 = UCS is not orthographic,1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { NL("$PUCSXDIR")                , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Current paper space UCS X axis") },
  { NL("$PUCSYDIR")                , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Current paper space UCS Y axis") },
  { NL("$QTEXTMODE")               , 1, { {  70, NL("")  } } ,  NL("Quick text mode on if nonzero") },
  { NL("$REALWORLDSCALE")          , 1, { { 290, NL("")  } } ,  NL("Drawing is scaled to the real world. Minimum AutoCAD version: R2007") },
  { NL("$REGENMODE")               , 1, { {  70, NL("")  } } ,  NL("REGENAUTO mode on if nonzero") },
  { NL("$REQUIREDVERSIONS")        , 1, { { 160, NL("")  } } ,  NL("Unknown. Minimum AutoCAD version: R2013.") },
  { NL("$SHADEDGE")                , 1, { {  70, NL("")  } } ,  NL("0 = faces shaded, edges not highlighted 1 = faces shaded, edges highlighted in black 2 = faces not filled, edges in entity color 3 = faces in entity color, edges in black") },
  { NL("$SHADEDIF")                , 1, { {  70, NL("")  } } ,  NL("Percent ambient-diffuse light, range 1-100, default 70") },
  { NL("$SHADOWPLANELOCATION")     , 1, { {  40, NL("")  } } ,  NL("Location of the ground shadow plane. This is a Z axis ordinate") },
  { NL("$SHOWHIST")                , 1, { { 280, NL("")  } } ,  NL("Controls the Show History property for solids in a drawing. Minimum AutoCAD version: R2007") },
  { NL("$SKETCHINC")               , 1, { {  40, NL("")  } } ,  NL("Sketch record increment") },
  { NL("$SKPOLY")                  , 1, { {  70, NL("")  } } ,  NL("0 = sketch lines, 1 = sketch polylines") },
  { NL("$STEPSPERSEC")             , 1, { {  40, NL("")  } } ,  NL("Specifies the number of steps taken per second when you are in walk or fly mode. Minimum AutoCAD version: R2007") },
  { NL("$STEPSIZE")                , 1, { {  40, NL("")  } } ,  NL("Specifies the size of each step when in walk or fly mode, in drawing units. Minimum AutoCAD version: R2007") },
  { NL("$STYLESHEET")              , 1, { {   1, NL("")  } } ,  NL("Path to the stylesheet for the drawing. Minimum AutoCAD version: R2000") },
  { NL("$SOLIDHIST")               , 1, { { 280, NL("")  } } ,  NL("Controls whether GEN_NEW composite solids retain a history of their original components. Minimum AutoCAD version: R2007") },
  { NL("$SORTENTS")                , 1, { { 280, NL("")  } } ,  NL("Controls the object sorting methods; accessible from the Options dialog box User Preferences tab. SORTENTS uses the following bitcodes: 0 = Disables SORTENTS 1 = Sorts for object selection, 2 = Sorts for object snap, 4 = Sorts for redraws; obsolete, 8 = Sorts for MSLIDE command slide creation; obsolete, 16 = Sorts for REGEN commands 32 = Sorts for plotting, 64 = Sorts for PostScript output; obsolete") },
  { NL("$SPLFRAME")                , 1, { {  70, NL("")  } } ,  NL("Spline control polygon display; 1 = on, 0 = off") },  
  { NL("$SPLINESEGS")              , 1, { {  70, NL("")  } } ,  NL("Number of line segments per spline patch") },
  { NL("$SPLINETYPE")              , 1, { {  70, NL("")  } } ,  NL("Spline curve type for PEDIT Spline") },
  { NL("$SURFTAB1")                , 1, { {  70, NL("")  } } ,  NL("Number of mesh tabulations in first direction") },
  { NL("$SURFTAB2")                , 1, { {  70, NL("")  } } ,  NL("Number of mesh tabulations in second direction") },
  { NL("$SURFTYPE")                , 1, { {  70, NL("")  } } ,  NL("Surface type for PEDIT Smooth") },
  { NL("$SURFU")                   , 1, { {  70, NL("")  } } ,  NL("Surface density (for PEDIT Smooth) in M direction") },
  { NL("$SURFV")                   , 1, { {  70, NL("")  } } ,  NL("Surface density (for PEDIT Smooth) in N direction") },
  { NL("$TDCREATE")                , 1, { {  40, NL("")  } } ,  NL("Date/time of drawing creation") },
  { NL("$TDINDWG")                 , 1, { {  40, NL("")  } } ,  NL("Cumulative editing time for this drawing") },
  { NL("$TDUCREATE")               , 1, { {  40, NL("")  } } ,  NL("Universal date/time the drawing was created (see Special Handling of Date/Time Variables)") },
  { NL("$TDUPDATE")                , 1, { {  40, NL("")  } } ,  NL("Date/time of last drawing update") },
  { NL("$TDUSRTIMER")              , 1, { {  40, NL("")  } } ,  NL("User elapsed timer") },
  { NL("$TDUUPDATE")               , 1, { {  40, NL("")  } } ,  NL("Universal date/time of the last update/save (see Special Handling of Date/Time Variables)") }, 
  { NL("$TEXTSIZE")                , 1, { {  40, NL("")  } } ,  NL("Default text height") },
  { NL("$TEXTSTYLE")               , 1, { {   7, NL("")  } } ,  NL("Current text style name") },
  { NL("$THICKNESS")               , 1, { {  40, NL("")  } } ,  NL("Current thickness set by ELEV command") },
  { NL("$TILEMODELIGHTSYNCH")      , 1, { { 280, NL("")  } } ,  NL("Unknown. Minimum AutoCAD version: R2007") },
  { NL("$TIMEZONE")                , 1, { {  70, NL("")  } } ,  NL("Sets the time zone for the sun in the drawing. Minimum AutoCAD version: R2007") },
  { NL("$TILEMODE")                , 1, { {  70, NL("")  } } ,  NL("1 for previous release compatibility mode, 0 otherwise") },
  { NL("$TILEMODELIGHTSYNCH")      , 1, { { 280, NL("")  } } ,  NL("Unknown. Minimum AutoCAD version: R2007") },
  { NL("$TRACEWID")                , 1, { {  40, NL("")  } } ,  NL("Default Trace width") },
  { NL("$TREEDEPTH")               , 1, { {  70, NL("")  } } ,  NL("Specifies the maximum depth of the spatial index") },
  { NL("$UCSBASE")                 , 1, { {   2, NL("")  } } ,  NL("Name of the UCS that defines the origin and orientation of orthographic UCS settings") },
  { NL("$UCSNAME")                 , 1, { {   2, NL("")  } } ,  NL("Name of current UCS") },
  { NL("$UCSORG")                  , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Origin of current UCS (in WCS)") },
  { NL("$UCSORGBACK")              , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BACK when UCSBASE is set to WORLD") },
  { NL("$UCSORGBOTTOM")            , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BOTTOM when UCSBASE is set to WORLD") },
  { NL("$UCSORGFRONT")             , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to FRONT when UCSBASE is set to WORLD") },
  { NL("$UCSORGLEFT")              , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to LEFT when UCSBASE is set to WORLD") },
  { NL("$UCSORGRIGHT")             , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to RIGHT when UCSBASE is set to WORLD") },
  { NL("$UCSORGTOP")               , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Point which becomes the GEN_NEW UCS origin after changing model space UCS to TOP when UCSBASE is set to WORLD") },
  { NL("$UCSORTHOREF")             , 1, { {   2, NL("")  } } ,  NL("If model space UCS is orthographic (UCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { NL("$UCSORTHOVIEW")            , 1, { {  70, NL("")  } } ,  NL("Orthographic view type of model space UCS: 0 = UCS is not orthographic, 1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { NL("$UCSXDIR")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Direction of current UCS's X axis (in WCS)") },
  { NL("$UCSYDIR")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Direction of current UCS's Y axis (in WCS)") },
  { NL("$UNITMODE")                , 1, { {  70, NL("")  } } ,  NL("Low bit set = display fractions, feet-and-inches, and surveyor's angles in input format") },
  { NL("$USERI1")                  , 1, { {  70, NL("")  } } ,  NL("1 For integer variables intended for use by third-party developers") },
  { NL("$USERI2")                  , 1, { {  70, NL("")  } } ,  NL("2 For integer variables intended for use by third-party developers") },
  { NL("$USERI3")                  , 1, { {  70, NL("")  } } ,  NL("3 For integer variables intended for use by third-party developers") },
  { NL("$USERI4")                  , 1, { {  70, NL("")  } } ,  NL("4 For integer variables intended for use by third-party developers") },
  { NL("$USERI5")                  , 1, { {  70, NL("")  } } ,  NL("5 For integer variables intended for use by third-party developers") },
  { NL("$USERR1")                  , 1, { {  40, NL("")  } } ,  NL("1 Five real variables intended for use by third-party developers") },
  { NL("$USERR2")                  , 1, { {  40, NL("")  } } ,  NL("2 Five real variables intended for use by third-party developers") },
  { NL("$USERR3")                  , 1, { {  40, NL("")  } } ,  NL("3 Five real variables intended for use by third-party developers") },
  { NL("$USERR4")                  , 1, { {  40, NL("")  } } ,  NL("4 Five real variables intended for use by third-party developers") },
  { NL("$USERR5")                  , 1, { {  40, NL("")  } } ,  NL("1 Five real variables intended for use by third-party developers") },        
  { NL("$USRTIMER")                , 1, { {  70, NL("")  } } ,  NL("0 = timer off, 1 = timer on") },
  { NL("$VERSIONGUID")             , 1, { {   2, NL("")  } } ,  NL("Uniquely identifies a particular version of a drawing. Updated when the drawing is modified") },
  { NL("$VISRETAIN")               , 1, { {  70, NL("")  } } ,  NL("0 = don't retain Xref-dependent visibility settings 1 = retain Xref-dependent visibility settings") },
  { NL("$WORLDVIEW")               , 1, { {  70, NL("")  } } ,  NL("1 = set UCS to WCS during DVIEW/VPOINT 0 = don't change UCS") },   
  { NL("$XCLIPFRAME")              , 1, { { 290, NL("")  } } ,  NL("Controls the visibility of xref clipping boundaries: 0 = Clipping boundary is not visible 1 = Clipping boundary is visible") },
  { NL("$XEDIT")                   , 1, { { 290, NL("")  } } ,  NL("Controls whether the current drawing can be edited in-place when being referenced by another drawing: 0 = Can't use in-place reference editing, 1 = Can use in-place reference editing") },

  // The following header variables existed before AutoCAD Release 11 but now have independent settings for each active viewport. OPEN honors these variables when read from DXF files. If a VPORT symbol table with *ACTIVE entries is present (as is true for any DXF file produced by Release 11 or later),
  // the values in the VPORT table entries override the values of these header variables.
   
  { NL("$FASTZOOM")                , 1, { {  70, NL("")  } } ,  NL("Fast zoom enabled if nonzero") },
  { NL("$GRIDMODE")                , 1, { {  70, NL("")  } } ,  NL("Grid mode on if nonzero") },
  { NL("$GRIDUNIT")                , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Grid X and Y spacing") },
  { NL("$SNAPANG")                 , 1, { {  50, NL("")  } } ,  NL("Snap grid rotation angle") },
  { NL("$SNAPBASE")                , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Snap/grid base point (in UCS)") },
  { NL("$SNAPISOPAIR")             , 1, { {  70, NL("")  } } ,  NL("Isometric plane; 0 = left, 1 = top. 2 = right") },
  { NL("$SNAPMODE")                , 1, { {  70, NL("")  } } ,  NL("Snap mode on if nonzero") },
  { NL("$SNAPSTYLE")               , 1, { {  70, NL("")  } } ,  NL("Snap style; 0 = standard, 1 = isometric") },
  { NL("$SNAPUNIT")                , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("Snap grid X and Y spacing") },
  { NL("$VIEWCTR")                 , 2, { {  10, NL("X") }, { 20, NL("Y") } } ,  NL("XY center of current view on screen") },
  { NL("$VIEWDIR")                 , 3, { {  10, NL("X") }, { 20, NL("Y") }, { 30, NL("Z") } } ,  NL("Viewing direction (direction from target, in WCS)") },
  { NL("$VIEWSIZE")                , 1, { {  40, NL("")  } } ,  NL("Height of view") }                             
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

          namevarext->Format(NL("%s_%d"), part->name.Get(), type);

          if(!extname.IsEmpty())
            {
              namevarext->AddFormat(NL("_%s"), extname.Get());          
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

      if(line && !line->Compare(NL("9"), true))
        {
          c++;
          line = fileTXT->GetLine(c);
          GRPVECTORFILEDXF::ParserTextFilePrepareLine(line);

          if(line->Get()[0]== _C('$'))
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

              message.Format(NL("Header Variable Unknown [%s]"), part->name.Get());
                               
              GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

              vfevent.SetType(GRPVECTORFILETYPE_DXF);
              vfevent.GetPath()->Set(NL(""));
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
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, NL("[GRPVECTORFILEDXFTEXTSECTIONHEADER] (%3d) var [%s] %s"), c, key->Get(), string.Get());
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

