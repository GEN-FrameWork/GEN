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
  { _L("$3DDWFPREC")               , 1, { {  40, _L("")  } } ,  _L("Controls the precision of 3D DWF or 3D DWFx publishing. Minimum AutoCAD version: R2007") },
  { _L("$ACADMAINTVER")            , 1, { {  70, _L("")  } } ,  _L("Maintenance version number (should be ignored)") },
  { _L("$ACADVER")                 , 1, { {   1, _L("")  } } ,  _L("The AutoCAD drawing database version number: AC1006 = R10, AC1009 = R11 and R12, AC1012 = R13, AC1014 = R14, AC1015 = AutoCAD 2000, AC1018 = AutoCAD 2004, AC1021 = AutoCAD 2007, AC1024 = AutoCAD 2010, AC1027 = AutoCAD 2013, AC1032 = AutoCAD 2018") },
  { _L("$ANGBASE")                 , 1, { {  50, _L("")  } } ,  _L("Angle 0 direction") },
  { _L("$ANGDIR")                  , 1, { {  70, _L("")  } } ,  _L("1 = clockwise angles, 0 = counterclockwise angles") },
  { _L("$ATTDIA")                  , 1, { {  70, _L("")  } } ,  _L("Attribute entry dialogs; 1 = on, 0 = off") },
  { _L("$ATTMODE")                 , 1, { {  70, _L("")  } } ,  _L("Attribute visibility; 0 = none, 1 = normal, 2 = all") },
  { _L("$ATTREQ")                  , 1, { {  70, _L("")  } } ,  _L("Attribute prompting during INSERT; 1 = on, 0 = off") },
  { _L("$AUNITS")                  , 1, { {  70, _L("")  } } ,  _L("Units format for angles") },
  { _L("$AUPREC")                  , 1, { {  70, _L("")  } } ,  _L("Units precision for angles") },
  { _L("$AXISMODE")                , 1, { {  70, _L("")  } } ,  _L("Axis on if nonzero (not functional in Release 12)") },
  { _L("$AXISUNIT")                , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Axis X and Y tick spacing (not functional in Release 12)") },
  { _L("$BLIPMODE")                , 1, { {  70, _L("")  } } ,  _L("Blip mode on if nonzero") },
  { _L("$CAMERADISPLAY")           , 1, { { 290, _L("")  } } ,  _L("Turns the display of camera objects on or off. Minimum AutoCAD version: R2007") },
  { _L("$CAMERAHEIGHT")            , 1, { {  40, _L("")  } } ,  _L("Specifies the default height for GEN_NEW camera objects. Minimum AutoCAD version: R2007") },     
  { _L("$CECOLOR")                 , 1, { {  62, _L("")  } } ,  _L("Current entity color number: 0 = BYBLOCK; 256 = BYLAYER") },
  { _L("$CELTSCALE")               , 1, { {  40, _L("")  } } ,  _L("Current entity linetype scale") },
  { _L("$CELTYPE")                 , 1, { {   6, _L("")  } } ,  _L("Entity linetype name, or BYBLOCK or BYLAYER") },
  { _L("$CELWEIGHT")               , 1, { { 370, _L("")  } } ,  _L("Lineweight of GEN_NEW objects") },
  { _L("$CEPSNID")                 , 1, { { 390, _L("")  } } ,  _L("Plotstyle handle of GEN_NEW objects; if CEPSNTYPE is 3, then this value indicates the handle") },
  { _L("$CEPSNTYPE")               , 1, { { 380, _L("")  } } ,  _L("Plot style type of GEN_NEW objects: 0 = Plot style by layer, 1 = Plot style by block, 2 = Plot style by dictionary default, 3 = Plot style by object ID/handle") },
  { _L("$CHAMFERA")                , 1, { {  40, _L("")  } } ,  _L("First chamfer distance") },
  { _L("$CHAMFERB")                , 1, { {  40, _L("")  } } ,  _L("Second chamfer distance") },
  { _L("$CHAMFERC")                , 1, { {  40, _L("")  } } ,  _L("Chamfer length") },
  { _L("$CHAMFERD")                , 1, { {  40, _L("")  } } ,  _L("Chamfer angle") },
  { _L("$CLAYER")                  , 1, { {   8, _L("")  } } ,  _L("Current layer name 11") },
  { _L("$COORDS")                  , 1, { {  70, _L("")  } } ,  _L("0 = static coordinate display, 1 = continuous update, 2 = 'd<a' format") },
  { _L("$CMATERIAL")               , 1, { { 280, _L("")  } } ,  _L("Sets the material of GEN_NEW objects. Minimum AutoCAD version: R2007") },
  { _L("$CMLJUST")                 , 1, { {  70, _L("")  } } ,  _L("Current multiline justification: 0 = Top; 1 = Middle; 2 = Bottom") },
  { _L("$CMLSCALE")                , 1, { {  40, _L("")  } } ,  _L("Current multiline scale") },
  { _L("$CMLSTYLE")                , 1, { {   2, _L("")  } } ,  _L("Current multiline style name") },
  { _L("$CSHADOW")                 , 1, { { 280, _L("")  } } ,  _L("Shadow mode for a 3D object: 0 = Casts and receives shadows, 1 = Casts shadows,, 2 = Receives shadows, 3 = Ignores shadows (Note: Starting with AutoCAD 2016-based products, this variable is obsolete but still supported for backwards compatibility)") },
  { _L("$DELOBJ")                  , 1, { {  70, _L("")  } } ,  _L("Controls object deletion. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { _L("$DIMADEC")                 , 1, { {  70, _L("")  } } ,  _L("Number of precision places displayed in angular dimensions") },
  { _L("$DIMALT")                  , 1, { {  70, _L("")  } } ,  _L("Alternate unt dimensioning performed if nonzero") },
  { _L("$DIMALTD")                 , 1, { {  70, _L("")  } } ,  _L("Alternate unit decimal places") },
  { _L("$DIMALTF")                 , 1, { {  40, _L("")  } } ,  _L("Alternate unit scale factor") },
  { _L("$DIMALTRND")               , 1, { {  40, _L("")  } } ,  _L("Determines rounding of alternate units") },
  { _L("$DIMALTTD")                , 1, { {  70, _L("")  } } ,  _L("Number of decimal places for tolerance values of an alternate units dimension") },
  { _L("$DIMALTTZ")                , 1, { {  70, _L("")  } } ,  _L("Controls suppression of zeros for alternate tolerance values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet-. To suppress leading or trailing zeros, add the following values to one of the preceding values: 4 = Suppresses leading zeros. 8 = Suppresses trailing zeros") },
  { _L("$DIMALTU")                 , 1, { {  70, _L("")  } } ,  _L("Units format for alternate units of all dimension style family members except angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural (stacked), 5 = Fractional (stacked), 6 = Architectural, 7 = Fractional, 8 = Operating system defines the decimal separator and number grouping symbols") },
  { _L("$DIMALTZ")                 , 1, { {  70, _L("")  } } ,  _L("Controls suppression of zeros for alternate unit dimension values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },  
  { _L("$DIMAPOST")                , 1, { {   1, _L("")  } } ,  _L("Alternate dimensioning suffix") },
  { _L("$DIMASO")                  , 1, { {  70, _L("")  } } ,  _L("1 = create associative dimensioning, 0 = draw individual entities") },
  { _L("$DIMASSOC")                , 1, { { 280, _L("")  } } ,  _L("Controls the associativity of dimension objects 0 = Creates exploded dimensions; there is no association between elements of the dimension, and the lines, arcs, arrowheads, and text of a dimension are drawn as separate objects, 1 = Creates non-associative dimension objects; the elements of the dimension are formed into a single object, and if the definition point on the object moves, then the dimension value is updated, 2 = Creates associative dimension objects; the elements of the dimension are formed into a single object and one or more definition points of the dimension are coupled with association points on geometric objects") },
  { _L("$DIMASZ")                  , 1, { {  40, _L("")  } } ,  _L("Dimensioning arrow size") },
  { _L("$DIMATFIT")                , 1, { {  70, _L("")  } } ,  _L("Controls dimension text and arrow placement when space is not sufficient to place both within the extension lines: 0 = Places both text and arrows outside extension lines, 1 = Moves arrows first, then text, 2 = Moves text first, then arrows, 3 = Moves either text or arrows, whichever fits best, AutoCAD adds a leader to moved dimension text when DIMTMOVE is set to 1") },
  { _L("$DIMAUNIT")                , 1, { {  70, _L("")  } } ,  _L("Angle format for angular dimensions: 0 = Decimal degrees, 1 = Degrees/minutes/seconds;, 2 = Gradians, 3 = Radians, 4 = Surveyor's units") },
  { _L("$DIMAZIN")                 , 1, { {  70, _L("")  } } ,  _L("Controls suppression of zeros for angular dimensions: 0 = Displays all leading and trailing zeros, 1 = Suppresses leading zeros in decimal dimensions, 2 = Suppresses trailing zeros in decimal dimensions, 3 = Suppresses leading and trailing zeros") },
  { _L("$DIMBLK")                  , 1, { {   2, _L("")  } } ,  _L("Arrow block name") },
  { _L("$DIMBLK1")                 , 1, { {   1, _L("")  } } ,  _L("First arrow block name") },
  { _L("$DIMBLK2")                 , 1, { {   1, _L("")  } } ,  _L("Second arrow block name") },
  { _L("$DIMCEN")                  , 1, { {  40, _L("")  } } ,  _L("Size of center mark/lines") },
  { _L("$DIMCLRD")                 , 1, { {  70, _L("")  } } ,  _L("Dimension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { _L("$DIMCLRE")                 , 1, { {  70, _L("")  } } ,  _L("Dimension extension line color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { _L("$DIMCLRT")                 , 1, { {  70, _L("")  } } ,  _L("Dimension text color, range is 0 = BYBLOCK, 256 = BYLAYER") },
  { _L("$DIMDEC")                  , 1, { {  70, _L("")  } } ,  _L("Number of decimal places for the tolerance values of a primary units dimension") },
  { _L("$DIMDLE")                  , 1, { {  40, _L("")  } } ,  _L("Dimension line extension") },
  { _L("$DIMDLI")                  , 1, { {  40, _L("")  } } ,  _L("Dimension line increment") },
  { _L("$DIMDSEP")                 , 1, { {  70, _L("")  } } ,  _L("Single-character decimal separator used when creating dimensions whose unit format is decimal") },
  { _L("$DIMEXE")                  , 1, { {  40, _L("")  } } ,  _L("Extension line extension") },
  { _L("$DIMEXO")                  , 1, { {  40, _L("")  } } ,  _L("Extension line offset") },
  { _L("$DIMFAC")                  , 1, { {  40, _L("")  } } ,  _L("Scale factor used to calculate the height of text for dimension fractions and tolerances. AutoCAD multiplies DIMTXT by DIMTFAC to set the fractional or tolerance text height") },
  { _L("$DIMFRAC")                 , 1, { {  70, _L("")  } } ,  _L("Sets the fraction format when DIMLUNIT is set to Architectural or Fractional. Minimum AutoCAD version: R2000") },
  { _L("$DIMFIT")                  , 1, { {  70, _L("")  } } ,  _L("Placement of text and arrowheads. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { _L("$DIMGAP")                  , 1, { {  40, _L("")  } } ,  _L("Dimension line gap") }, 
  { _L("$DIMJUST")                 , 1, { {  70, _L("")  } } ,  _L("Horizontal dimension text position: 0 = Above dimension line and center-justified between extension lines, 1 = Above dimension line and next to first extension line, 2 = Above dimension line and next to second extension line, 3 = Above and center-justified to first extension line, 4 = Above and center-justified to second extension line") },
  { _L("$DIMLDRBLK")               , 1, { {   1, _L("")  } } ,  _L("Arrow block name for leaders") },
  { _L("$DIMLFAC")                 , 1, { {  40, _L("")  } } ,  _L("Linear measurements scale factor") },
  { _L("$DIMLIM")                  , 1, { {  70, _L("")  } } ,  _L("Dimension limits generated if nonzero") },
  { _L("$DIMLUNIT")                , 1, { {  70, _L("")  } } ,  _L("Sets units for all dimension types except Angular: 1 = Scientific, 2 = Decimal, 3 = Engineering, 4 = Architectural, 5 = Fractional, 6 = Operating system") },
  { _L("$DIMLWD")                  , 1, { {  70, _L("")  } } ,  _L("Dimension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { _L("$DIMLWE")                  , 1, { {  70, _L("")  } } ,  _L("Extension line lineweight: -3 = Standard, -2 = ByLayer, -1 = ByBlock, 0-211 = an integer representing 100th of mm") },
  { _L("$DIMPOST")                 , 1, { {   1, _L("")  } } ,  _L("General dimensioning suffix") },
  { _L("$DIMRND")                  , 1, { {  40, _L("")  } } ,  _L("Rounding value for dimension distances") },
  { _L("$DIMSAH")                  , 1, { {  70, _L("")  } } ,  _L("Use separate arrow blocks if nonzero") },
  { _L("$DIMSCALE")                , 1, { {  40, _L("")  } } ,  _L("Overall dimensioning scale factor") },
  { _L("$DIMSD1")                  , 1, { {  70, _L("")  } } ,  _L("Suppression of first extension line: 0 = Not suppressed, 1 = Suppressed") },
  { _L("$DIMSD2")                  , 1, { {  70, _L("")  } } ,  _L("Suppression of second extension line: 0 = Not suppressed, 1 = Suppressed") },
  { _L("$DIMSE1")                  , 1, { {  70, _L("")  } } ,  _L("First extension line suppressed if nonzero ") },
  { _L("$DIMSE2")                  , 1, { {  70, _L("")  } } ,  _L("Second extension line suppressed if nonzero") },
  { _L("$DIMSHO")                  , 1, { {  70, _L("")  } } ,  _L("1 = recompute dimensions while dragging, 0 = drag original image") },
  { _L("$DIMSOXD")                 , 1, { {  70, _L("")  } } ,  _L("Suppress outside-extensions dimension lines if nonzero") },
  { _L("$DIMSTYLE")                , 1, { {   2, _L("")  } } ,  _L("Dimension style name") },
  { _L("$DIMTAD")                  , 1, { {  70, _L("")  } } ,  _L("Text above dimension line if nonzero") },
  { _L("$DIMTDEC")                 , 1, { {  70, _L("")  } } ,  _L("Number of decimal places to display the tolerance values") },
  { _L("$DIMTFAC")                 , 1, { {  40, _L("")  } } ,  _L("Dimension tolerance display scale factor") },
  { _L("$DIMTIH")                  , 1, { {  70, _L("")  } } ,  _L("Text inside horizontal if nonzero") },
  { _L("$DIMTIX")                  , 1, { {  70, _L("")  } } ,  _L("Force text inside extensions if nonzero") },
  { _L("$DIMTM")                   , 1, { {  40, _L("")  } } ,  _L("Minus tolerance") },
  { _L("$DIMTMOVE")                , 1, { {  70, _L("")  } } ,  _L("Dimension text movement rules: 0 = Moves the dimension line with dimension text, 1 = Adds a leader when dimension text is moved, 2 = Allows text to be moved freely without a leader") },
  { _L("$DIMTOFL")                 , 1, { {  70, _L("")  } } ,  _L("If text outside extensions, force line extensions between extensions if nonzero") },
  { _L("$DIMTOH")                  , 1, { {  70, _L("")  } } ,  _L("Text outside horizontal if nonzero") },
  { _L("$DIMTOL")                  , 1, { {  70, _L("")  } } ,  _L("Dimension tolerances generated if nonzero") },
  { _L("$DIMTOLJ")                 , 1, { {  70, _L("")  } } ,  _L("Vertical justification for tolerance values: 0 = Top, 1 = Middle, 2 = Bottom") },
  { _L("$DIMTP")                   , 1, { {  40, _L("")  } } ,  _L("Plus tolerance") },
  { _L("$DIMTSZ")                  , 1, { {  40, _L("")  } } ,  _L("Dimensioning tick size; 0 = no ticks") },
  { _L("$DIMTVP")                  , 1, { {  40, _L("")  } } ,  _L("Text vertical position") },
  { _L("$DIMTXSTY")                , 1, { {   7, _L("")  } } ,  _L("Dimension text style") },
  { _L("$DIMTXT")                  , 1, { {  40, _L("")  } } ,  _L("Dimensioning text height") },
  { _L("$DIMTZIN")                 , 1, { {  70, _L("")  } } ,  _L("Controls suppression of zeros for tolerance values: 0 = Suppresses zero feet and precisely zero inches,1 = Includes zero feet and precisely zero inches, 2 = Includes zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { _L("$DIMUPT")                  , 1, { {  70, _L("")  } } ,  _L("Cursor functionality for user-positioned text: 0 = Controls only the dimension line location, 1 = Controls the text position as well as the dimension line location") },
  { _L("$DIMZIN")                  , 1, { {  70, _L("")  } } ,  _L("Controls suppression of zeros for primary unit values: 0 = Suppresses zero feet and precisely zero inches, 1 = Includes zero feet and precisely zero inches, 2 = Includes, zero feet and suppresses zero inches, 3 = Includes zero inches and suppresses zero feet, 4 = Suppresses leading zeros in decimal dimensions, 8 = Suppresses trailing zeros in decimal dimensions, 12 = Suppresses both leading and trailing zeros") },
  { _L("$DIMFXL")                  , 1, { {  40, _L("")  } } ,  _L("Sets the total length of the extension lines starting from the dimension line toward the dimension origin. Minimum AutoCAD version: R2007") },
  { _L("$DIMFXLON")                , 1, { {  70, _L("")  } } ,  _L("Controls whether extension lines are set to a fixed length. Minimum AutoCAD version: R2007") },
  { _L("$DIMJOGANG")               , 1, { {  40, _L("")  } } ,  _L("Determines the angle of the transverse segment of the dimension line in a jogged radius dimension. Minimum AutoCAD version: R2007") },
  { _L("$DIMTFILL")                , 1, { {  70, _L("")  } } ,  _L("Controls the background of dimension text. Minimum AutoCAD version: R2007") },
  { _L("$DIMTFILLCLR")             , 1, { {  70, _L("")  } } ,  _L("Sets the color for the text background in dimensions. Minimum AutoCAD version: R2007") },
  { _L("$DIMARCSYM")               , 1, { {  70, _L("")  } } ,  _L("Controls the display of the arc symbol in an arc length dimension. Minimum AutoCAD version: R2007") },
  { _L("$DIMLTYPE")                , 1, { {   6, _L("")  } } ,  _L("Sets the line type of the dimension line. Minimum AutoCAD version: R2007") },
  { _L("$DIMLTEX1")                , 1, { {   6, _L("")  } } ,  _L("Sets the line type of the first extension line. Minimum AutoCAD version: R2007") },
  { _L("$DIMLTEX2")                , 1, { {   6, _L("")  } } ,  _L("Sets the line type of the second extension line. Minimum AutoCAD version: R2007") },
  { _L("$DIMTXTDIRECTION")         , 1, { {  70, _L("")  } } ,  _L("Specifies the reading direction of the dimension text. Minimum AutoCAD version: R2010") },  
  { _L("$DIMUNIT")                 , 1, { {  70, _L("")  } } ,  _L("Units format for all dimension style family members except angular. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { _L("$DISPSILH")                , 1, { {  70, _L("")  } } ,  _L("Controls the display of silhouette curves of body objects in Wireframe mode: 0 = Off, 1 = On") },
  { _L("$DGNFRAME")                , 1, { { 280, _L("")  } } ,  _L("Determines whether DGN underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { _L("$DRAGVS")                  , 1, { { 349, _L("")  } } ,  _L("Hard-pointer ID to visual style while creating 3D solid primitives. The default value is NULL") },
  { _L("$DWFFRAME")                , 1, { { 280, _L("")  } } ,  _L("Determines whether DWF or DWFx underlay frames are visible or plotted in the current drawing. Minimum AutoCAD version: R2007") },
  { _L("$DWGCODEPAGE")             , 1, { {   3, _L("")  } } ,  _L("Drawing code page. Set to the system code page when a GEN_NEW drawing is created, but not otherwise maintained by AutoCAD") },
  { _L("$DRAGMODE")                , 1, { {  70, _L("")  } } ,  _L("0 = off, 1 = on, 2 = auto") },
  { _L("$ELEVATION")               , 1, { {  40, _L("")  } } ,  _L("Current elevation set by ELEV command") },
  { _L("$ENDCAPS")                 , 1, { { 280, _L("")  } } ,  _L("Lineweight endcaps setting for GEN_NEW objects: 0 = None, 1 = Round, 2 = Angle, 3 = Square") },
  { _L("$EXTMAX")                  , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("X, Y, and Z drawing extents upper-right corner (in WCS)") },
  { _L("$EXTMIN")                  , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("X, Y, and Z drawing extents lower-left corner (in WCS)") },
  { _L("$EXTNAMES")                , 1, { { 290, _L("")  } } ,  _L("Controls symbol table naming: 0 = Release 14 compatibility. Limits names to 31 characters in length. Names can include the letters A to Z, the numerals 0 to 9, and the special characters dollar sign ($), underscore (_), and hyphen (-)., 1 = AutoCAD 2000. Names can be up to 255 characters in length, and can include the letters A to Z, the numerals 0 to 9, spaces, and any special characters not used for other purposes by Microsoft Windows and AutoCAD") },
  { _L("$FILLETRAD")               , 1, { {  40, _L("")  } } ,  _L("Fillet radius") },
  { _L("$FILLMODE")                , 1, { {  70, _L("")  } } ,  _L("Fill mode on if nonzero") },
  { _L("$FINGERPRINTGUID")         , 1, { {   2, _L("")  } } ,  _L("Set at creation time, uniquely identifies a particular drawing") },
  { _L("$HALOGAP")                 , 1, { { 280, _L("")  } } ,  _L("Specifies a gap to be displayed where an object is hidden by another object; the value is specified as a percent of one unit and is independent of the zoom level. A haloed line is shortened at the point where it is hidden when HIDE or the Hidden option of SHADEMODE is used") },
  { _L("$HANDLING")                , 1, { {  70, _L("")  } } ,  _L("Handles enabled if nonzero") },
  { _L("$HANDSEED")                , 1, { {   5, _L("")  } } ,  _L("Next available handle") },
  { _L("$HIDETEXT")                , 1, { { 290, _L("")  } } ,  _L("Specifies HIDETEXT system variable: 0 = HIDE ignores text objects when producing the hidden view, 1 = HIDE does not ignore text objects") },
  { _L("$HYPERLINKBASE")           , 1, { {   1, _L("")  } } ,  _L("Path for all relative hyperlinks in the drawing. If null, the drawing path is used") },
  { _L("$INDEXCTL")                , 1, { { 280, _L("")  } } ,  _L("Controls whether layer and spatial indexes are created and saved in drawing files: 0 = No indexes are created, 1 = Layer index is created, 2 = Spatial index is created, 3 = Layer and spatial indexes are created") },
  { _L("$INSBASE")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Insertion base set by BASE command (in WCS)") },
  { _L("$INSUNITS")                , 1, { {  70, _L("")  } } ,  _L("Default drawing units for AutoCAD DesignCenter blocks: 0 = Unitless, 1 = Inches, 2 = Feet, 3 = Miles, 4 = Millimeters, 5 = Centimeters, 6 = Meters, 7 = Kilometers, 8 = Microinches, 9 = Mils, 10 = Yards, 11 = Angstroms, 12 = Nanometers, 13 = Microns, 14 = Decimeters, 15 = Decameters, 16 = Hectometers, 17 = Gigameters, 18 = Astronomical units, 19 = Light years, 20 = Parsecs, 21 = US Survey Feet, 22 = US Survey Inch, 23 = US Survey Yard, 24 = US Survey Mile") },
  { _L("$INTERFERECOLOR")          , 1, { {  62, _L("")  } } ,  _L("Represents the ACI color index of the 'interference objects' created during the INTERFERE command. Default value is 1") },
  { _L("$INTERFEREOBJVS")          , 1, { { 345, _L("")  } } ,  _L("Hard-pointer ID to the visual style for interference objects. Default visual style is Conceptual") },
  { _L("$INTERFEREVPVS")           , 1, { { 346, _L("")  } } ,  _L("Hard-pointer ID to the visual style for the viewport during interference checking. Default visual style is 3d Wireframe") },
  { _L("$INTERSECTIONCOLOR")       , 1, { {  70, _L("")  } } ,  _L("Specifies the entity color of intersection polylines: Values 1-255 designate an AutoCAD color index (ACI) 0 = Color BYBLOCK, 256 = Color BYLAYER, 257 = Color BYENTITY") },
  { _L("$INTERSECTIONDISPLAY")     , 1, { { 290, _L("")  } } ,  _L("Specifies the display of intersection polylines: 0 = Turns off the display of intersection polylines 1 = Turns on the display of intersection polylines") },
  { _L("$JOINSTYLE")               , 1, { { 280, _L("")  } } ,  _L("Lineweight joint setting for GEN_NEW objects: 0=None 1= Round 2 = Angle 3 = Flat") },
  { _L("$LATITUDE")                , 1, { {  40, _L("")  } } ,  _L("The latitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { _L("$LASTSAVEDBY")             , 1, { {   1, _L("")  } } ,  _L("Name of the last user to modify the file. Minimum AutoCAD version: R2004") },  
  { _L("$LENSLENGTH")              , 1, { {  40, _L("")  } } ,  _L("Stores the length of the lens in millimeters used in perspective viewing. Minimum AutoCAD version: R2007") },
  { _L("$LIGHTGLYPHDISPLAY")       , 1, { { 280, _L("")  } } ,  _L("Turns on and off the display of light glyphs. Minimum AutoCAD version: R2007") },
  { _L("$LIMCHECK")                , 1, { {  70, _L("")  } } ,  _L("Nonzero if limits checking is on") },
  { _L("$LIMMAX")                  , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("XY drawing limits upper-right corner (in WCS)") },
  { _L("$LIMMIN")                  , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("XY drawing limits lower-left corner (in WCS)") },
  { _L("$LOFTANG1")                , 1, { {  40, _L("")  } } ,  _L("Sets the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { _L("$LOFTANG2")                , 1, { {  40, _L("")  } } ,  _L("Sets the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { _L("$LOFTMAG1")                , 1, { {  40, _L("")  } } ,  _L("Sets the magnitude of the draft angle through the first cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { _L("$LOFTMAG2")                , 1, { {  40, _L("")  } } ,  _L("Sets the magnitude of the draft angle through the second cross section in a loft operation. Minimum AutoCAD version: R2007") },
  { _L("$LOFTPARAM")               , 1, { {  70, _L("")  } } ,  _L("Controls the shape of lofted solids and surfaces. Minimum AutoCAD version: R2007") },
  { _L("$LOFTNORMALS")             , 1, { { 280, _L("")  } } ,  _L("Controls the normals of a lofted object where it passes through cross sections. Minimum AutoCAD version: R2007") },
  { _L("$LONGITUDE")               , 1, { {  40, _L("")  } } ,  _L("The longitude of the geographic location assigned to the drawing. Minimum AutoCAD version: R2007") },
  { _L("$LTSCALE")                 , 1, { {  40, _L("")  } } ,  _L("Global linetype scale") },
  { _L("$LUNITS")                  , 1, { {  70, _L("")  } } ,  _L("Units format for coordinates and distances") },
  { _L("$LUPREC")                  , 1, { {  70, _L("")  } } ,  _L("Units precision for coordinates and distances") },
  { _L("$LWDISPLAY")               , 1, { { 290, _L("")  } } ,  _L("Controls the display of lineweights on the Model or Layout tab: 0 = Lineweight is not displayed, 1 = Lineweight is displayed") },
  { _L("$MAXACTVP")                , 1, { {  70, _L("")  } } ,  _L("Sets maximum number of viewports to be regenerated") },
  { _L("$MEASUREMENT")             , 1, { {  70, _L("")  } } ,  _L("Sets drawing units: 0 = English 1 = Metric") },
  { _L("$MENU")                    , 1, { {   1, _L("")  } } ,  _L("Name of menu file") },
  { _L("$MIRRTEXT")                , 1, { {  70, _L("")  } } ,  _L("Mirror text if nonzero") },
  { _L("$NORTHDIRECTION")          , 1, { {  40, _L("")  } } ,  _L("Specifies the angle between the Y axis of WCS and the grid north. Minimum AutoCAD version: R2007") },
  { _L("$OBSCOLOR")                , 1, { {  70, _L("")  } } ,  _L("Specifies the color of obscured lines. An obscured line is a hidden line made visible by changing its color and linetype and is visible only when the HIDE or SHADEMODE command is used. The OBSCUREDCOLOR setting is visible only if the OBSCUREDLTYPE is turned ON by setting it to a value other than 0. 0 and 256 = Entity color 1-255 = An AutoCAD color index (ACI)") },
  { _L("$OBSLTYPE")                , 1, { { 280, _L("")  } } ,  _L("Specifies the linetype of obscured lines. Obscured linetypes are independent of zoom level, unlike regular AutoCAD linetypes. Value 0 turns off display of obscured lines and is the default. Linetype values are defined as follows: 0 = Off, 1 = Solid, 2 = Dashed, 3 = Dotted, 4 = Short Dash, 5 = Medium Dash, 6 = Long Dash, 7 = Double Short Dash, 8 = Double Medium Dash, 9 = Double Long Dash, 10 = Medium Long Dash, 11 = Sparse Dot") },  
  { _L("$OLESTARTUP")              , 1, { { 290, _L("")  } } ,  _L("Controls whether the source application of an embedded OLE object loads when plotting. Minimum AutoCAD version: R2000") },
  { _L("$ORTHOMODE")               , 1, { {  70, _L("")  } } ,  _L("Ortho mode on if nonzero") },
  { _L("$OSMODE")                  , 1, { {  70, _L("")  } } ,  _L("Running object snap modes") },
  { _L("$PDMODE")                  , 1, { {  70, _L("")  } } ,  _L("Point display mode") },
  { _L("$PDSIZE")                  , 1, { {  40, _L("")  } } ,  _L("Point display size") },
  { _L("$PELEVATION")              , 1, { {  40, _L("")  } } ,  _L("Current paper space elevation") },
  { _L("$PEXTMAX")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Maximum X, Y, and Z extents for paper space") },
  { _L("$PEXTMIN")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Minimum X, Y, and Z extents for paper space") },
  { _L("$PICKSTYLE")               , 1, { {  70, _L("")  } } ,  _L("Controls the group selection and associative hatch selection. Minimum AutoCAD version: R13. Maximum AutoCAD version: R14") },
  { _L("$PINSBASE")                , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Paper space insertion base point") },
  { _L("$PLIMCHECK")               , 1, { {  70, _L("")  } } ,  _L("Limits checking in paper space when nonzero") },
  { _L("$PLIMMAX")                 , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Maximum X and Y limits in paper space") },
  { _L("$PLIMMIN")                 , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Minimum X and Y limits in paper space") },
  { _L("$PLINEGEN")                , 1, { {  70, _L("")  } } ,  _L("Governs the generation of linetype patterns around the vertices of a 2D Polyline 1 = linetype is generated in a continuous pattern around vertices of the Polyline 0 = each segment of the Polyline starts and ends with a dash") },
  { _L("$PLINEWID")                , 1, { {  40, _L("")  } } ,  _L("Default Polyline width") },
  { _L("$PROJECTNAME")             , 1, { {   1, _L("")  } } ,  _L("Assigns a project name to the current drawing. Used when an external reference or image is not found on its original path. The project name points to a section in the registry that can contain one or more search paths for each project name defined. Project names and their search directories are created from the Files tab of the Options dialog box") },
  { _L("$PROXYGRAPHICS")           , 1, { {  70, _L("")  } } ,  _L("Controls the saving of proxy object images") },
  { _L("$PSOLHEIGHT")              , 1, { {  40, _L("")  } } ,  _L("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { _L("$PSOLHEIGHT")              , 1, { {  40, _L("")  } } ,  _L("Controls the default height for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { _L("$PSOLWIDTH")               , 1, { {  40, _L("")  } } ,  _L("Controls the default width for a swept solid object created with the POLYSOLID command. Minimum AutoCAD version: R2007") },
  { _L("$PSLTSCALE")               , 1, { {  70, _L("")  } } ,  _L("Controls paper space linetype scaling 1 = no special linetype scaling 0 = viewport scaling governs linetype scaling") },
  { _L("$PSTYLEMODE")              , 1, { { 290, _L("")  } } ,  _L("Indicates whether the current drawing is in a Color-Dependent or Named Plot Style mode: 0 = Uses named plot style tables in the current drawing, 1 = Uses color-dependent plot style tables in the current drawing") },
  { _L("$PSVPSCALE")               , 1, { {  40, _L("")  } } ,  _L("View scale factor for GEN_NEW viewports: 0 = Scaled to fit >0 = Scale factor (a positive real value)") },
  { _L("$PUCSBASE")                , 1, { {   2, _L("")  } } ,  _L("Name of the UCS that defines the origin and orientation of orthographic UCS settings (paper space only)") },
  { _L("$PUCSNAME")                , 1, { {   2, _L("")  } } ,  _L("Current paper space UCS name") },
  { _L("$PUCSORG")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Current paper space UCS origin") },
  { _L("$PUCSORGBACK")             , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BACK when PUCSBASE is set to WORLD") },
  { _L("$PUCSORGBOTTOM")           , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to BOTTOM when PUCSBASE is set to WORLD") },
  { _L("$PUCSORGFRONT")            , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to FRONT when PUCSBASE is set to WORLD") },
  { _L("$PUCSORGLEFT")             , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to LEFT when PUCSBASE is set to WORLD") },
  { _L("$PUCSORGRIGHT")            , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to RIGHT when PUCSBASE is set to WORLD") },
  { _L("$PUCSORGTOP")              , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing paper space UCS to TOP when PUCSBASE is set to WORLD") },
  { _L("$PUCSORTHOREF")            , 1, { {   2, _L("")  } } ,  _L("If paper space UCS is orthographic (PUCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { _L("$PUCSORTHOVIEW")           , 1, { {  70, _L("")  } } ,  _L("Orthographic view type of paper space UCS: 0 = UCS is not orthographic,1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { _L("$PUCSXDIR")                , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Current paper space UCS X axis") },
  { _L("$PUCSYDIR")                , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Current paper space UCS Y axis") },
  { _L("$QTEXTMODE")               , 1, { {  70, _L("")  } } ,  _L("Quick text mode on if nonzero") },
  { _L("$REALWORLDSCALE")          , 1, { { 290, _L("")  } } ,  _L("Drawing is scaled to the real world. Minimum AutoCAD version: R2007") },
  { _L("$REGENMODE")               , 1, { {  70, _L("")  } } ,  _L("REGENAUTO mode on if nonzero") },
  { _L("$REQUIREDVERSIONS")        , 1, { { 160, _L("")  } } ,  _L("Unknown. Minimum AutoCAD version: R2013.") },
  { _L("$SHADEDGE")                , 1, { {  70, _L("")  } } ,  _L("0 = faces shaded, edges not highlighted 1 = faces shaded, edges highlighted in black 2 = faces not filled, edges in entity color 3 = faces in entity color, edges in black") },
  { _L("$SHADEDIF")                , 1, { {  70, _L("")  } } ,  _L("Percent ambient-diffuse light, range 1-100, default 70") },
  { _L("$SHADOWPLANELOCATION")     , 1, { {  40, _L("")  } } ,  _L("Location of the ground shadow plane. This is a Z axis ordinate") },
  { _L("$SHOWHIST")                , 1, { { 280, _L("")  } } ,  _L("Controls the Show History property for solids in a drawing. Minimum AutoCAD version: R2007") },
  { _L("$SKETCHINC")               , 1, { {  40, _L("")  } } ,  _L("Sketch record increment") },
  { _L("$SKPOLY")                  , 1, { {  70, _L("")  } } ,  _L("0 = sketch lines, 1 = sketch polylines") },
  { _L("$STEPSPERSEC")             , 1, { {  40, _L("")  } } ,  _L("Specifies the number of steps taken per second when you are in walk or fly mode. Minimum AutoCAD version: R2007") },
  { _L("$STEPSIZE")                , 1, { {  40, _L("")  } } ,  _L("Specifies the size of each step when in walk or fly mode, in drawing units. Minimum AutoCAD version: R2007") },
  { _L("$STYLESHEET")              , 1, { {   1, _L("")  } } ,  _L("Path to the stylesheet for the drawing. Minimum AutoCAD version: R2000") },
  { _L("$SOLIDHIST")               , 1, { { 280, _L("")  } } ,  _L("Controls whether GEN_NEW composite solids retain a history of their original components. Minimum AutoCAD version: R2007") },
  { _L("$SORTENTS")                , 1, { { 280, _L("")  } } ,  _L("Controls the object sorting methods; accessible from the Options dialog box User Preferences tab. SORTENTS uses the following bitcodes: 0 = Disables SORTENTS 1 = Sorts for object selection, 2 = Sorts for object snap, 4 = Sorts for redraws; obsolete, 8 = Sorts for MSLIDE command slide creation; obsolete, 16 = Sorts for REGEN commands 32 = Sorts for plotting, 64 = Sorts for PostScript output; obsolete") },
  { _L("$SPLFRAME")                , 1, { {  70, _L("")  } } ,  _L("Spline control polygon display; 1 = on, 0 = off") },  
  { _L("$SPLINESEGS")              , 1, { {  70, _L("")  } } ,  _L("Number of line segments per spline patch") },
  { _L("$SPLINETYPE")              , 1, { {  70, _L("")  } } ,  _L("Spline curve type for PEDIT Spline") },
  { _L("$SURFTAB1")                , 1, { {  70, _L("")  } } ,  _L("Number of mesh tabulations in first direction") },
  { _L("$SURFTAB2")                , 1, { {  70, _L("")  } } ,  _L("Number of mesh tabulations in second direction") },
  { _L("$SURFTYPE")                , 1, { {  70, _L("")  } } ,  _L("Surface type for PEDIT Smooth") },
  { _L("$SURFU")                   , 1, { {  70, _L("")  } } ,  _L("Surface density (for PEDIT Smooth) in M direction") },
  { _L("$SURFV")                   , 1, { {  70, _L("")  } } ,  _L("Surface density (for PEDIT Smooth) in N direction") },
  { _L("$TDCREATE")                , 1, { {  40, _L("")  } } ,  _L("Date/time of drawing creation") },
  { _L("$TDINDWG")                 , 1, { {  40, _L("")  } } ,  _L("Cumulative editing time for this drawing") },
  { _L("$TDUCREATE")               , 1, { {  40, _L("")  } } ,  _L("Universal date/time the drawing was created (see Special Handling of Date/Time Variables)") },
  { _L("$TDUPDATE")                , 1, { {  40, _L("")  } } ,  _L("Date/time of last drawing update") },
  { _L("$TDUSRTIMER")              , 1, { {  40, _L("")  } } ,  _L("User elapsed timer") },
  { _L("$TDUUPDATE")               , 1, { {  40, _L("")  } } ,  _L("Universal date/time of the last update/save (see Special Handling of Date/Time Variables)") }, 
  { _L("$TEXTSIZE")                , 1, { {  40, _L("")  } } ,  _L("Default text height") },
  { _L("$TEXTSTYLE")               , 1, { {   7, _L("")  } } ,  _L("Current text style name") },
  { _L("$THICKNESS")               , 1, { {  40, _L("")  } } ,  _L("Current thickness set by ELEV command") },
  { _L("$TILEMODELIGHTSYNCH")      , 1, { { 280, _L("")  } } ,  _L("Unknown. Minimum AutoCAD version: R2007") },
  { _L("$TIMEZONE")                , 1, { {  70, _L("")  } } ,  _L("Sets the time zone for the sun in the drawing. Minimum AutoCAD version: R2007") },
  { _L("$TILEMODE")                , 1, { {  70, _L("")  } } ,  _L("1 for previous release compatibility mode, 0 otherwise") },
  { _L("$TILEMODELIGHTSYNCH")      , 1, { { 280, _L("")  } } ,  _L("Unknown. Minimum AutoCAD version: R2007") },
  { _L("$TRACEWID")                , 1, { {  40, _L("")  } } ,  _L("Default Trace width") },
  { _L("$TREEDEPTH")               , 1, { {  70, _L("")  } } ,  _L("Specifies the maximum depth of the spatial index") },
  { _L("$UCSBASE")                 , 1, { {   2, _L("")  } } ,  _L("Name of the UCS that defines the origin and orientation of orthographic UCS settings") },
  { _L("$UCSNAME")                 , 1, { {   2, _L("")  } } ,  _L("Name of current UCS") },
  { _L("$UCSORG")                  , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Origin of current UCS (in WCS)") },
  { _L("$UCSORGBACK")              , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BACK when UCSBASE is set to WORLD") },
  { _L("$UCSORGBOTTOM")            , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to BOTTOM when UCSBASE is set to WORLD") },
  { _L("$UCSORGFRONT")             , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to FRONT when UCSBASE is set to WORLD") },
  { _L("$UCSORGLEFT")              , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to LEFT when UCSBASE is set to WORLD") },
  { _L("$UCSORGRIGHT")             , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to RIGHT when UCSBASE is set to WORLD") },
  { _L("$UCSORGTOP")               , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Point which becomes the GEN_NEW UCS origin after changing model space UCS to TOP when UCSBASE is set to WORLD") },
  { _L("$UCSORTHOREF")             , 1, { {   2, _L("")  } } ,  _L("If model space UCS is orthographic (UCSORTHOVIEW not equal to 0), this is the name of the UCS that the orthographic UCS is relative to. If blank, UCS is relative to WORLD") },
  { _L("$UCSORTHOVIEW")            , 1, { {  70, _L("")  } } ,  _L("Orthographic view type of model space UCS: 0 = UCS is not orthographic, 1 = Top, 2 = Bottom, 3 = Front, 4 = Back, 5 = Left, 6 = Right") },
  { _L("$UCSXDIR")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Direction of current UCS's X axis (in WCS)") },
  { _L("$UCSYDIR")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Direction of current UCS's Y axis (in WCS)") },
  { _L("$UNITMODE")                , 1, { {  70, _L("")  } } ,  _L("Low bit set = display fractions, feet-and-inches, and surveyor's angles in input format") },
  { _L("$USERI1")                  , 1, { {  70, _L("")  } } ,  _L("1 For integer variables intended for use by third-party developers") },
  { _L("$USERI2")                  , 1, { {  70, _L("")  } } ,  _L("2 For integer variables intended for use by third-party developers") },
  { _L("$USERI3")                  , 1, { {  70, _L("")  } } ,  _L("3 For integer variables intended for use by third-party developers") },
  { _L("$USERI4")                  , 1, { {  70, _L("")  } } ,  _L("4 For integer variables intended for use by third-party developers") },
  { _L("$USERI5")                  , 1, { {  70, _L("")  } } ,  _L("5 For integer variables intended for use by third-party developers") },
  { _L("$USERR1")                  , 1, { {  40, _L("")  } } ,  _L("1 Five real variables intended for use by third-party developers") },
  { _L("$USERR2")                  , 1, { {  40, _L("")  } } ,  _L("2 Five real variables intended for use by third-party developers") },
  { _L("$USERR3")                  , 1, { {  40, _L("")  } } ,  _L("3 Five real variables intended for use by third-party developers") },
  { _L("$USERR4")                  , 1, { {  40, _L("")  } } ,  _L("4 Five real variables intended for use by third-party developers") },
  { _L("$USERR5")                  , 1, { {  40, _L("")  } } ,  _L("1 Five real variables intended for use by third-party developers") },        
  { _L("$USRTIMER")                , 1, { {  70, _L("")  } } ,  _L("0 = timer off, 1 = timer on") },
  { _L("$VERSIONGUID")             , 1, { {   2, _L("")  } } ,  _L("Uniquely identifies a particular version of a drawing. Updated when the drawing is modified") },
  { _L("$VISRETAIN")               , 1, { {  70, _L("")  } } ,  _L("0 = don't retain Xref-dependent visibility settings 1 = retain Xref-dependent visibility settings") },
  { _L("$WORLDVIEW")               , 1, { {  70, _L("")  } } ,  _L("1 = set UCS to WCS during DVIEW/VPOINT 0 = don't change UCS") },   
  { _L("$XCLIPFRAME")              , 1, { { 290, _L("")  } } ,  _L("Controls the visibility of xref clipping boundaries: 0 = Clipping boundary is not visible 1 = Clipping boundary is visible") },
  { _L("$XEDIT")                   , 1, { { 290, _L("")  } } ,  _L("Controls whether the current drawing can be edited in-place when being referenced by another drawing: 0 = Can't use in-place reference editing, 1 = Can use in-place reference editing") },

  // The following header variables existed before AutoCAD Release 11 but now have independent settings for each active viewport. OPEN honors these variables when read from DXF files. If a VPORT symbol table with *ACTIVE entries is present (as is true for any DXF file produced by Release 11 or later),
  // the values in the VPORT table entries override the values of these header variables.
   
  { _L("$FASTZOOM")                , 1, { {  70, _L("")  } } ,  _L("Fast zoom enabled if nonzero") },
  { _L("$GRIDMODE")                , 1, { {  70, _L("")  } } ,  _L("Grid mode on if nonzero") },
  { _L("$GRIDUNIT")                , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Grid X and Y spacing") },
  { _L("$SNAPANG")                 , 1, { {  50, _L("")  } } ,  _L("Snap grid rotation angle") },
  { _L("$SNAPBASE")                , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Snap/grid base point (in UCS)") },
  { _L("$SNAPISOPAIR")             , 1, { {  70, _L("")  } } ,  _L("Isometric plane; 0 = left, 1 = top. 2 = right") },
  { _L("$SNAPMODE")                , 1, { {  70, _L("")  } } ,  _L("Snap mode on if nonzero") },
  { _L("$SNAPSTYLE")               , 1, { {  70, _L("")  } } ,  _L("Snap style; 0 = standard, 1 = isometric") },
  { _L("$SNAPUNIT")                , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("Snap grid X and Y spacing") },
  { _L("$VIEWCTR")                 , 2, { {  10, _L("X") }, { 20, _L("Y") } } ,  _L("XY center of current view on screen") },
  { _L("$VIEWDIR")                 , 3, { {  10, _L("X") }, { 20, _L("Y") }, { 30, _L("Z") } } ,  _L("Viewing direction (direction from target, in WCS)") },
  { _L("$VIEWSIZE")                , 1, { {  40, _L("")  } } ,  _L("Height of view") }                             
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

          namevarext->Format(_L("%s_%d"), part->name.Get(), type);

          if(!extname.IsEmpty())
            {
              namevarext->AddFormat(_L("_%s"), extname.Get());          
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

      if(line && !line->Compare(_L("9"), true))
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

              message.Format(_L("Header Variable Unknown [%s]"), part->name.Get());
                               
              GRPVECTORFILE_XEVENT vfevent(GetGrpVectorFile(), GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

              vfevent.SetType(GRPVECTORFILETYPE_DXF);
              vfevent.GetPath()->Set(_L(""));
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
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[GRPVECTORFILEDXFTEXTSECTIONHEADER] (%3d) var [%s] %s"), c, key->Get(), string.Get());
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

