/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSection.cpp
* 
* @class      GRPVECTORFILEDXFTEXTSECTION
* @brief      Graphic Vector File DXF Text Section class
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

#include "GRPVectorFileDXFTextSection.h"

#include "GRPVectorFileDXFXDataCtrl.h"
#include "GRPVectorFileDXFTextSectionHeader.h"
#include "GRPVectorFileDXFTextSectionBlocks.h"
#include "GRPVectorFileDXFTextSectionEntities.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


GRPVECTORFILEDXFDEFTEXTSECTION GRPVECTORFILEDXFTEXTSECTION::defsection[GRPVECTORFILEDXFTEXTSECTION_MAXNDEFSECTIONS] = 
{ 
   { _L("HEADER")          , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER                        },
   { _L("CLASSES")         , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_CLASSES                       },
   { _L("TABLES")          , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_TABLES                        },
   { _L("BLOCKS")          , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_BLOCKS                        },
   { _L("ENTITIES")        , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES                      },
   { _L("OBJECTS")         , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_OBJECTS                       },
   { _L("ACDSDATA")        , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ACDSDATA                      },
   { _L("THUMBNAILIMAGE")  , GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_THUMBNAILIMAGE                },
};


GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE GRPVECTORFILEDXFTEXTSECTION::genericdef[GRPVECTORFILEDXFTEXTSECTION_MAXNGENERICDEFTYPES] = 
{
   {    -5,    0, _L("G_APP_REACTOR_CHAIN")                          , _L("APP: persistent reactor chain")                                                                                            },
   {    -4,    0, _L("G_APP_CONDITIONAL_OPERATOR")                   , _L("APP: conditional operator (used only with ssget)")                                                                         },
   {    -3,    0, _L("G_APP_EXTENDED DATA")                          , _L("APP: extended data (XDATA) sentinel (fixed)")                                                                              },
   {    -2,    0, _L("G_APP_ENTITY_NAME_REFERENCE")                  , _L("APP: entity name reference (fixed)")                                                                                       },
   {    -1,    0, _L("G_APP_ENTITY_NAME")                            , _L("APP: entity name. The name changes each time a drawing is opened. It is never saved (fixed)")                              },
   {     0,    0, _L("G_TEXT_STRING")                                , _L("Text string indicating the entity type (fixed)")                                                                           },
   {     1,    0, _L("G_PRIMARY_TEXT")                               , _L("Primary text value for an entity")                                                                                         },
   {     2,    0, _L("G_NAME")                                       , _L("Name (attribute tag, block name, and so on)")                                                                              },
   {     3,    4, _L("G_OTHER_TEXT")                                 , _L("Other text or name values")                                                                                                },
   {     5,    0, _L("G_ENTITY_HANDLE")                              , _L("Entity handle; text string of up to 16 hexadecimal digits (fixed)")                                                        },
   {     6,    0, _L("G_LINETYPE_NAME")                              , _L("Linetype name (fixed)")                                                                                                    },
   {     7,    0, _L("G_TEXT_STYLE_NAME")                            , _L("Text style name (fixed)")                                                                                                  },
   {     8,    0, _L("G_LAYER_NAME")                                 , _L("Layer name (fixed)")                                                                                                       },
   {     9,    0, _L("G_VARIABLE_NAME_ID")                           , _L("DXF: variable name identifier (used only in HEADER section of the DXF file)")                                              },
   {    10,    0, _L("G_PRIMARY_POINT")                              , _L("Primary point; this is the start point of a line or text entity, center of a circle, and so on DXF: X value of the primary point (followed by Y and Z value codes 20 and 30) APP: 3D point (list of three reals)") },
   {    11,   18, _L("G_OTHER_POINTS")                               , _L("Other points DXF: X value of other points (followed by Y value codes 21-28 and Z value codes 31-38) APP: 3D point (list of three reals)") },
   {    20,    0, _L("G_Y_PRIMARY")                                  , _L("DXF: Y and Z values of the primary point")                                                                                 },
   {    30,    0, _L("G_Z_PRIMARY")                                  , _L("DXF: Y and Z values of the primary point")                                                                                 },
   {    21,   28, _L("G_Y_OTHER")                                    , _L("DXF: Y and Z values of other points")                                                                                      },
   {    31,   37, _L("G_Z_OTHER")                                    , _L("DXF: Y and Z values of other points")                                                                                      },
   {    38,    0, _L("G_ENTITY_ELEVATION")                           , _L("DXF: entity's elevation if nonzero")                                                                                       },
   {    39,    0, _L("G_THICKNESS")                                  , _L("Entity's thickness if nonzero (fixed)")                                                                                    },
   {    40,   48, _L("G_FLOATING-POINT_VALUES")                      , _L("Double-precision floating-point values (text height, scale factors, and so on)")                                           },
   {    48,    0, _L("G_LINETYPE_SCALE")                             , _L("Linetype scale; double precision floating point scalar value; default value is defined for all entity types")              },
   {    49,    0, _L("G_REPEATED_FLOATING-POINT_VALUE")              , _L("Repeated double-precision floating-point value. Multiple 49 groups may appear in one entity for variable-length tables (such as the dash lengths in the LTYPE table). A 7x group always appears before the first 49 group to specify the table length") },
   {    50,   58, _L("G_ANGLES")                                     , _L("Angles (output in degrees to DXF files and radians through AutoLISP and ObjectARX applications)")                          },
   {    60,    0, _L("G_ENTITY_VISIBILITY")                          , _L("Entity visibility; integer value; absence or 0 indicates visibility; 1 indicates invisibility")                            },
   {    62,    0, _L("G_COLOR_NUMBER")                               , _L("Color number (fixed)")                                                                                                     },
   {    66,    0, _L("G_ENTITIES_FLAG")                              , _L("'Entities follow' flag (fixed)")                                                                                           },
   {    67,    0, _L("G_TYPE_SPACE")                                 , _L("Space-that is, model or paper space (fixed)")                                                                              },
   {    68,    0, _L("G_APP_IDENTIFIES")                             , _L("APP: identifies whether viewport is on but fully off screen; is not active or is off")                                     },
   {    69,    0, _L("G_APP_VIEWPORT")                               , _L("APP: viewport identification number")                                                                                      },
   {    70,   78, _L("G_INTEGER_VALUES")                             , _L("Integer values, such as repeat counts, flag bits, or modes")                                                               },
   {    90,   99, _L("G_32-BIT_INTEGER_VALUES")                      , _L("32-bit integer values")                                                                                                    },
   {   100,    0, _L("G_SUBCLASS_MARKER")                            , _L("Subclass data marker (with derived class name as a string). Required for all objects and entity classes that are derived from another concrete class. The subclass data marker segregates data defined by different classes in the inheritance chain for the same object. This is in addition to the requirement for DXF names for each distinct concrete class derived from ObjectARX (see Subclass Markers)") },
   {   102,    0, _L("G_CONTROL_STRING")                             , _L("Control string, followed by '{<arbitrary name>' or '}'. Similar to the xdata 1002 group code, except that when the string begins with '{', it can be followed by an arbitrary string whose interpretation is up to the application. The only other control string allowed is '}' as a group terminator. AutoCAD does not interpret these strings except during drawing audit operations.") },
   {   105,    0, _L("G_OBJECT_HANDLE_DIMVAR")                       , _L("Object handle for DIMVAR symbol table entry")                                                                              },
   {   110,    0, _L("G_APP_3D_POINT")                               , _L("UCS origin (appears only if code 72 is set to 1) DXF: X value; APP: 3D point")                                             },
   {   111,    0, _L("G_APP_UCS_X_3D_VECTOR")                        , _L("UCS X-axis (appears only if code 72 is set to 1) DXF: X value; APP: 3D vector")                                            },
   {   112,    0, _L("G_APP_UCS_Y_3D_VECTOR ")                       , _L("UCS Y-axis (appears only if code 72 is set to 1) DXF: X value; APP: 3D vector")                                            },
   {   120,  122, _L("G_UCS_ORIGIN_Y")                               , _L("DXF: Y value of UCS origin, UCS X-axis, and UCS Y-axis")                                                                   },
   {   130,  132, _L("G_UCS_ORIGIN_Z")                               , _L("DXF: Z value of UCS origin, UCS X-axis, and UCS Y-axis")                                                                   },
   {   140,  149, _L("G_FLOATING-POINT_VALUES")                      , _L("Double-precision floating-point values (points, elevation, and DIMSTYLE settings, for example)")                           },
   {   170,  179, _L("G_16-BIT_INTEGER_VALUES")                      , _L("16-bit integer values, such as flag bits representing DIMSTYLE settings")                                                  },
   {   210,    0, _L("G_X_EXTRUSION_DIRECTION")                      , _L("Extrusion direction (fixed) DXF: X value of extrusion direction APP: 3D extrusion direction vector")                       },
   {   220,    0, _L("G_Y_EXTRUSION_DIRECTION")                      , _L("DXF: Y and Z values of the extrusion direction")                                                                           },  
   {   230,    0, _L("G_Z_EXTRUSION_DIRECTION")                      , _L("DXF: Y and Z values of the extrusion direction")                                                                           },
   {   270,  279, _L("G_16-BIT_INTEGER_VALUES")                      , _L("16-bit integer values")                                                                                                    },
   {   280,  289, _L("G_16-BIT_INTEGER_VALUE")                       , _L("16-bit integer value")                                                                                                     },
   {   290,  299, _L("G_BOOLEAN_FLAG_VALUE")                         , _L("Boolean flag value")                                                                                                       },
   {   300,  309, _L("G_ARBITRARY_TEXT_STRING")                      , _L("Arbitrary text strings")                                                                                                   },
   {   310,  319, _L("G_ARBITRARY_BINARY_CHUNK")                     , _L("Arbitrary binary chunks with same representation and limits as 1004 group codes: hexadecimal strings of up to 254 characters represent data chunks of up to 127 bytes")  },
   {   320,  329, _L("G_ARBITRARY_OBJECT_HANDLE")                    , _L("Arbitrary object handles; handle values that are taken 'as is'. They are not translated during INSERT and XREF operations")                                              },
   {   330,  339, _L("G_SOFT-POINTER_HANDLE")                        , _L("Soft-pointer handle; arbitrary soft pointers to other objects within same DXF file or drawing. Translated during INSERT and XREF operations")                            },
   {   340,  349, _L("G_HARD-POINTER_HANDLE")                        , _L("Hard-pointer handle; arbitrary hard pointers to other objects within same DXF file or drawing. Translated during INSERT and XREF operations")                            },
   {   350,  359, _L("G_SOFT-OWNER_HANDLE")                          , _L("Soft-owner handle; arbitrary soft ownership links to other objects within same DXF file or drawing. Translated during INSERT and XREF operations")                       },
   {   360,  369, _L("G_HARD-OWNER_HANDLE")                          , _L("Hard-owner handle; arbitrary hard ownership links to other objects within same DXF file or drawing. Translated during INSERT and XREF operations")                       },
   {   370,  379, _L("G_LINEWEIGHT_ENUM_VALUE")                      , _L("Lineweight enum value (AcDb::LineWeight). Stored and moved around as a 16-bit integer. Custom non-entity objects may use the full range, but entity classes only use 371-379 DXF group codes in their representation, because AutoCAD and AutoLISP both always assume a 370 group code is the entity's lineweight. This allows 370 to behave like other 'common' entity fields") },
   {   380,  389, _L("G_PLOTSTYLENAME_TYPE")                         , _L("PlotStyleName type enum (AcDb::PlotStyleNameType). Stored and moved around as a 16-bit integer. Custom non-entity objects may use the full range, but entity classes only use 381-389 DXF group codes in their representation, for the same reason as the Lineweight range above") },
   {   390,  399, _L("G_STRING_REPRESENTING_HANDLE")                 , _L("String representing handle value of the PlotStyleName object, basically a hard pointer, but has a different range to make backward compatibility easier to deal with. Stored and moved around as an object ID (a handle in DXF files) and a special type in AutoLISP. Custom non-entity objects may use the full range, but entity classes only use 391-399 DXF group codes in their representation, for the same reason as the lineweight range above") },
   {   400,  409, _L("G_16-BIT_INTEGERS")                            , _L("16-bit integers")                                                                                                          },
   {   410,  419, _L("G_STRING")                                     , _L("String")                                                                                                                   },
   {   420,  427, _L("G_24-BIT_COLOR_VALUE")                         , _L("32-bit integer value. When used with True Color; a 32-bit integer representing a 24-bit color value. The high-order byte (8 bits) is 0, the low-order byte an unsigned char holding the Blue value (0-255), then the Green value, and the next-to-high order byte is the Red Value. Converting this integer value to hexadecimal yields the following bit mask: 0x00RRGGBB. For example, a true color with Red==200, Green==100 and Blue==50 is 0x00C86432, and in DXF, in decimal, 13132850") },
   {   430,  437, _L("G_STRING")                                     , _L("String; when used for True Color, a string representing the name of the color")                                            },
   {   440,  447, _L("G_32-BIT_INTEGER_VALUE")                       , _L("32-bit integer value. When used for True Color, the transparency value")                                                   },
   {   450,  459, _L("G_LONG")                                       , _L("Long")                                                                                                                     },
   {   460,  469, _L("G_FLOATING-POINT_VALUE")                       , _L("Double-precision floating-point value")                                                                                    },
   {   470,  479, _L("G_STRING")                                     , _L("String")                                                                                                                   },
   {   480,  481, _L("G_HARD-POINTER_HANDLE")                        , _L("Hard-pointer handle; arbitrary hard pointers to other objects within same DXF file or drawing. Translated during INSERT and XREF operations") },
   {   999,    0, _L("G_REMARKS")                                    , _L("DXF: The 999 group code indicates that the line following it is a comment string. SAVEAS does not include such groups in a DXF output file, but OPEN honors them and ignores the comments. You can use the 999 group to include comments in a DXF file that you have edited") },
   {  1000,    0, _L("G_ASCII STRING")                               , _L("ASCII string (up to 255 bytes long) in extended data")                                                                     },
   {  1001,    0, _L("G_REGISTERED_APPLICATION_NAME")                , _L("Registered application name (ASCII string up to 31 bytes long) for extended data")                                         },
   {  1002,    0, _L("G_EXTENDED_DATA_CTRL_STRING")                  , _L("Extended data control string ('{' or '}')")                                                                                },
   {  1003,    0, _L("G_EXTENDED_DATA_LAYER_NAME")                   , _L("Extended data layer name")                                                                                                 },
   {  1004,    0, _L("G_CHUNK_BYTES")                                , _L("Chunk of bytes (up to 127 bytes long) in extended data")                                                                   },
   {  1005,    0, _L("G_ENTITY_HANDLE_EXTENDED_STRING")              , _L("Entity handle in extended data; text string of up to 16 hexadecimal digits")                                               },
   {  1010,    0, _L("G_X_POINT_EXTENDED_DATA_VALUE")                , _L("A point in extended data  DXF: X value (followed by 1020 and 1030 groups) APP: 3D point")                                  },
   {  1020,    0, _L("G_Y POINT_EXTENDED_DATA_VALUE")                , _L("DXF: Y and Z values of a point")                                                                                           },
   {  1030,    0, _L("G_Z POINT_EXTENDED_DATA_VALUE")                , _L("DXF: Y and Z values of a point")                                                                                           },
   {  1011,    0, _L("G_X_WORLD_SPACE_POSITION")                     , _L("A 3D world space position in extended data DXF: X value (followed by 1021 and 1031 groups) APP: 3D point")                 },
   {  1021,    0, _L("G_Y_WORLD_SPACE_POSITION")                     , _L("DXF: Y and Z values of a world space position")                                                                            },  
   {  1031,    0, _L("G_Z_WORLD_SPACE_POSITION")                     , _L("DXF: Y and Z values of a world space position")                                                                            },
   {  1012,    0, _L("G_X_WORLD_SPACE_DISPLACEMENT")                 , _L("A 3D world space displacement in extended data DXF: X value (followed by 1022 and 1032 groups) APP: 3D vector")            },
   {  1022,    0, _L("G_Y_WORLD_SPACE_DISPLACEMENT")                 , _L("DXF: Y and Z values of a world space displacement")                                                                        },
   {  1032,    0, _L("G_Z_WORLD_SPACE_DISPLACEMENT")                 , _L("DXF: Y and Z values of a world space displacement")                                                                        },
   {  1013,    0, _L("G_X_WORLD_SPACE_DIRECTION")                    , _L("A 3D world space direction in extended data DXF: X value (followed by 1022 and 1032 groups) APP: 3D vector")               },
   {  1023,    0, _L("G_Y_WORLD_SPACE_DIRECTION")                    , _L("DXF: Y and Z values of a world space direction")                                                                           },
   {  1033,    0, _L("G_Z_WORLD_SPACE_DIRECTION")                    , _L("DXF: Y and Z values of a world space direction")                                                                           },
   {  1040,    0, _L("G_EXTENDED DATA FLOATING-POINT VALUE")         , _L("Extended data double-precision floating-point value")                                                                      },
   {  1041,    0, _L("G_EXTENDED DATA DISTANCE VALUE")               , _L("Extended data distance value")                                                                                             },
   {  1042,    0, _L("G_EXTENDED DATA SCALE FACTOR")                 , _L("Extended data scale factor")                                                                                               },
   {  1070,    0, _L("G_EXTENDED DATA 16-BIT SIGNED INTEGER")        , _L("Extended data 16-bit signed integer")                                                                                      },
   {  1071,    0, _L("G_EXTENDED DATA 32-BIT SIGNED LONG")           , _L("Extended data 32-bit signed long")                                                                                         }
};




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION::GRPVECTORFILEDXFTEXTSECTION()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION::GRPVECTORFILEDXFTEXTSECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION::~GRPVECTORFILEDXFTEXTSECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION::~GRPVECTORFILEDXFTEXTSECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE* GRPVECTORFILEDXFTEXTSECTION::GetGrpVectorFile()
* @brief      Get grp vector file
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE* GRPVECTORFILEDXFTEXTSECTION::GetGrpVectorFile()
{
  return vectorfile;
}
   
 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFTEXTSECTION::SetGrpVectorFile(GRPVECTORFILE* vectorfile)
* @brief      Set grp vector file
* @ingroup    GRAPHIC
* 
* @param[in]  vectorfile : Vectorfile pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFTEXTSECTION::SetGrpVectorFile(GRPVECTORFILE* vectorfile)
{
  this->vectorfile = vectorfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION* GRPVECTORFILEDXFTEXTSECTION::CreateInstance(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION type)
* @brief      Create instance
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION* GRPVECTORFILEDXFTEXTSECTION::CreateInstance(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION type)
{
  GRPVECTORFILEDXFTEXTSECTION* section = NULL;

  switch(type)
    {      
      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN        :  
                                                  default         : break;

      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER         : section = GEN_NEW GRPVECTORFILEDXFTEXTSECTIONHEADER();                                                               
                                                                    break;  

      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_CLASSES        : break;   
      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_TABLES         : break;    

      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_BLOCKS         : section = GEN_NEW GRPVECTORFILEDXFTEXTSECTIONBLOCKS();
                                                                    break; 

      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES       : section = GEN_NEW GRPVECTORFILEDXFTEXTSECTIONENTITIES();
                                                                    break;  
                                                              
      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_OBJECTS        : break; 
      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ACDSDATA       : break;
      case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_THUMBNAILIMAGE : break;   
    }

  return section;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION_TYPESECTION GRPVECTORFILEDXFTEXTSECTION::GetTypeSection(XSTRING& namesection)
* @brief      Get type section
* @ingroup    GRAPHIC
* 
* @param[in]  namesection : Namesection value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTION_TYPESECTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION_TYPESECTION GRPVECTORFILEDXFTEXTSECTION::GetTypeSection(XSTRING& namesection)
{
  for(int c=0; c<GRPVECTORFILEDXFTEXTSECTION_MAXNDEFSECTIONS ; c++)
    {
      if(!namesection.Compare(GRPVECTORFILEDXFTEXTSECTION::defsection[c].name))
        {
          return GRPVECTORFILEDXFTEXTSECTION::defsection[c].type;
         }
    }

  return GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION_BASICTYPE GRPVECTORFILEDXFTEXTSECTION::GetTypeBasic(int type)
* @brief      Get type basic
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTION_BASICTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION_BASICTYPE GRPVECTORFILEDXFTEXTSECTION::GetTypeBasic(int type)
{
   switch(type)
   {     
      case    0   :  
      case    1   :
      case    2   :
      case    3   :
      case    4   :
      case    5   :
      case    6   :
      case    7   :
      case    8   :
      case    9   :  // String (with the introduction of extended symbol names in AutoCAD 2000, the 255-character limit has been increased to 2049 single-byte characters not including the newline at the end of the line); see the "Storage of String Values" section for more information      
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case   10   :  
      case   11   :
      case   12   :
      case   13   :
      case   14   :
      case   15   :
      case   16   :
      case   17   :
      case   18   :
      case   19   :  
      case   20   :  
      case   21   :
      case   22   :
      case   23   :
      case   24   :
      case   25   :
      case   26   :
      case   27   :
      case   28   :
      case   29   :  
      case   30   :  
      case   31   :
      case   32   :
      case   33   :
      case   34   :
      case   35   :
      case   36   :
      case   37   :
      case   38   :
      case   39   :  // Double precision 3D point value
                                                
      case   40   :  
      case   41   :
      case   42   :
      case   43   :
      case   44   :
      case   45   :
      case   46   :
      case   47   :
      case   48   :
      case   49   :  
      case   50   :  
      case   51   :
      case   52   :
      case   53   :
      case   54   :
      case   55   :
      case   56   :
      case   57   :
      case   58   :
      case   59   :  // Double-precision floating-point value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE;

      case   60   :  
      case   61   :
      case   62   :
      case   63   :
      case   64   :
      case   65   :
      case   66   :
      case   67   :
      case   68   :
      case   69   :  
      case   70   :  
      case   71   :
      case   72   :
      case   73   :
      case   74   :
      case   75   :
      case   76   :
      case   77   :
      case   78   :
      case   79   :  // 16-bit integer value

      case   90   :  
      case   91   :
      case   92   :
      case   93   :
      case   94   :
      case   95   :
      case   96   :
      case   97   :
      case   98   :
      case   99   :  // 32-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  100   :  // String (255-character maximum, less for Unicode strings); see the "Storage of String Values" section for more information
      case  102   :  // String (255-character maximum, less for Unicode strings); see the "Storage of String Values" section for more information
      case  105   :  // String representing hexadecimal (hex) handle value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;
                     
      case  110   :  
      case  111   :
      case  112   :
      case  113   :
      case  114   :
      case  115   :
      case  116   :
      case  117   :
      case  118   :
      case  119   :  // Double precision floating-point value

      case  120   :  
      case  121   :
      case  122   :
      case  123   :
      case  124   :
      case  125   :
      case  126   :
      case  127   :
      case  128   :
      case  129   :  // Double precision floating-point value


      case  130   :  
      case  131   :
      case  132   :
      case  133   :
      case  134   :
      case  135   :
      case  136   :
      case  137   :
      case  138   :
      case  139   :  // Double precision floating-point value

      case  140   : 
      case  141   :
      case  142   :
      case  143   :
      case  144   :
      case  145   :
      case  146   :
      case  147   :
      case  148   :
      case  149   :  // Double precision scalar floating-point value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE;

      case  160   :  
      case  161   :
      case  162   :
      case  163   :
      case  164   :
      case  165   :
      case  166   :
      case  167   :
      case  168   :
      case  169   :  // 64-bit integer value

      case  170   :  
      case  171   :
      case  172   :
      case  173   :
      case  174   :
      case  175   :
      case  176   :
      case  177   :
      case  178   :
      case  179   :  // 16-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  210   :  
      case  211   :
      case  212   :
      case  213   :
      case  214   :
      case  215   :
      case  216   :
      case  217   :
      case  218   :
      case  219   : 
      case  220   :
      case  221   :
      case  222   :
      case  223   :
      case  224   :
      case  225   :
      case  226   :
      case  227   :
      case  228   :
      case  229   :
      case  230   :
      case  231   :
      case  232   :
      case  233   :
      case  234   :
      case  235   :
      case  236   :
      case  237   :
      case  238   :
      case  239   :  // Double-precision floating-point value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE;

      case  270   :  
      case  271   :
      case  272   :
      case  273   :
      case  274   :
      case  275   :
      case  276   :
      case  277   :
      case  278   :
      case  279   :  // 16-bit integer value

      case  280   :  
      case  281   :
      case  282   :
      case  283   :
      case  284   :
      case  285   :
      case  286   :
      case  287   :
      case  288   :
      case  289   :  // 16-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  290   :  
      case  291   :
      case  292   :
      case  293   :
      case  294   :
      case  295   :
      case  296   :
      case  297   :
      case  298   :
      case  299   :  // Boolean flag value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_BOOLEAN;

      case  300   :  
      case  301   :
      case  302   :
      case  303   :
      case  304   :
      case  305   :
      case  306   :
      case  307   :
      case  308   :
      case  309   :  // Arbitrary text string; see the "Storage of String Values" section for more information

      case  310   :  
      case  311   :
      case  312   :
      case  313   :
      case  314   :
      case  315   :
      case  316   :
      case  317   :
      case  318   :
      case  319   :  // String representing hex value of binary chunk

      case  320   :  
      case  321   :
      case  322   :
      case  323   :
      case  324   :
      case  325   :
      case  326   :
      case  327   :
      case  328   :
      case  329   :  // String representing hex handle value

      case  330   :  
      case  331   :
      case  332   :
      case  333   :
      case  334   :
      case  335   :
      case  336   :
      case  337   :
      case  338   :
      case  339   : 
      case  341   :
      case  342   :
      case  343   :
      case  344   :
      case  345   :
      case  346   :
      case  347   :
      case  348   :
      case  349   : 
      case  351   :
      case  352   :
      case  353   :
      case  354   :
      case  355   :
      case  356   :
      case  357   :
      case  358   :
      case  359   :
      case  361   :
      case  362   :
      case  363   :
      case  364   :
      case  365   :
      case  366   :
      case  367   :
      case  368   :
      case  369   :  // String representing hex object IDs
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case  370   :  
      case  371   :
      case  372   :
      case  373   :
      case  374   :
      case  375   :
      case  376   :
      case  377   :
      case  378   :
      case  379   :  // 16-bit integer value

      case  380   :  
      case  381   :
      case  382   :
      case  383   :
      case  384   :
      case  385   :
      case  386   :
      case  387   :
      case  388   :
      case  389   :  // 16-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  390   :  
      case  391   :
      case  392   :
      case  393   :
      case  394   :
      case  395   :
      case  396   :
      case  397   :
      case  398   :
      case  399   :  // String representing hex handle value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case  400   :  
      case  401   :
      case  402   :
      case  403   :
      case  404   :
      case  405   :
      case  406   :
      case  407   :
      case  408   :
      case  409   :  // 16-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  410   :  
      case  411   :
      case  412   :
      case  413   :
      case  414   :
      case  415   :
      case  416   :
      case  417   :
      case  418   :
      case  419   :  // String; see the "Storage of String Values" section for more information
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case  420   :  
      case  421   :
      case  422   :
      case  423   :
      case  424   :
      case  425   :
      case  426   :
      case  427   :
      case  428   :
      case  429   :  // 32-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  430   :  
      case  431   :
      case  432   :
      case  433   :
      case  434   :
      case  435   :
      case  436   :
      case  437   :
      case  438   :
      case  439   :  // String; see the "Storage of String Values" section for more information
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case  440   :  
      case  441   :
      case  442   :
      case  443   :
      case  444   :
      case  445   :
      case  446   :
      case  447   :
      case  448   :
      case  449   :  // 32-bit integer value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  450   : 
      case  451   :
      case  452   :
      case  453   :
      case  454   :
      case  455   :
      case  456   :
      case  457   :
      case  458   :
      case  459   :  // Long
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;

      case  460   :  
      case  461   :
      case  462   :
      case  463   :
      case  464   :
      case  465   :
      case  466   :
      case  467   :
      case  468   :
      case  469   :  // Double-precision floating-point value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE;

      case  470   :  
      case  471   :
      case  472   :
      case  473   :
      case  474   :
      case  475   :
      case  476   :
      case  477   :
      case  478   :
      case  479   :  // String; see the "Storage of String Values" section for more information

      case  480   :  
      case  481   :  // String representing hex handle value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case  999   :  // Comment (string); see the "Storage of String Values" section for more information
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case 1000   :  
      case 1001   :
      case 1002   :
      case 1003   :
      case 1004   :
      case 1005   :
      case 1006   :
      case 1007   :
      case 1008   :
      case 1009   :  // String (same limits as indicated with 0-9 code range); see the "Storage of String Values" section for more information
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING;

      case 1010   :  
      case 1011   :
      case 1012   :
      case 1013   :
      case 1014   :
      case 1015   :
      case 1016   :
      case 1017   :
      case 1018   :
      case 1019   :
      case 1020   :
      case 1021   :
      case 1022   :
      case 1023   :
      case 1024   :
      case 1025   :
      case 1026   :
      case 1027   :
      case 1028   :
      case 1029   :
      case 1030   :
      case 1031   :
      case 1032   :
      case 1033   :
      case 1034   :
      case 1035   :
      case 1036   :
      case 1037   :
      case 1038   :
      case 1039   :
      case 1040   :
      case 1041   :
      case 1042   :
      case 1043   :
      case 1044   :
      case 1045   :
      case 1046   :
      case 1047   :
      case 1048   :
      case 1049   :
      case 1050   :
      case 1051   :
      case 1052   :
      case 1053   :
      case 1054   :
      case 1055   :
      case 1056   :
      case 1057   :
      case 1058   :
      case 1059   :  // Double-precision floating-point value
                     return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE;
                
      case 1060   :  
      case 1061   :
      case 1062   :
      case 1063   :
      case 1064   :
      case 1065   :
      case 1066   :
      case 1067   :
      case 1068   :
      case 1069   :
      case 1070   :  // 16-bit integer value

      case 1071   : // 32-bit integer value            
                    return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER;
   }
  
  return GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE* GRPVECTORFILEDXFTEXTSECTION::GetGenericDefType(int type)
* @brief      Get generic def type
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE* GRPVECTORFILEDXFTEXTSECTION::GetGenericDefType(int type)
{
  for(int c=0; c<GRPVECTORFILEDXFTEXTSECTION_MAXNGENERICDEFTYPES; c++)
    {
      GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE* defType = &genericdef[c];
      if(defType)
        {
          if(defType->type)
            {
              if((defType->type >= type) && (defType->range <= type))
                {
                  return defType;
                }
            }
           else
            {
              if(defType->type == type)  return defType;
            }            
        }   
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFTEXTSECTION::GetVariableFromLine(XCHAR* namevar, int type, XSTRING* line, XVARIANT& variant)
* @brief      Get variable from line
* @ingroup    GRAPHIC
* 
* @param[in]  namevar : Namevar pointer to use.
* @param[in]  type : Type value.
* @param[in]  line : Line pointer to use.
* @param[in]  variant : Variant value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFTEXTSECTION::GetVariableFromLine(XCHAR* namevar, int type, XSTRING* line, XVARIANT& variant)
{
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE basicType = GRPVECTORFILEDXFTEXTSECTION::GetTypeBasic(type);

  switch(basicType)
    {  
      case GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_BOOLEAN    : variant = line->ConvertToBoolean();
                                                              break;

      case GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER    : variant = line->ConvertToInt();
                                                              break;  
                                                   
      case GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE     : variant = line->ConvertToDouble();
                                                              break;          

      case GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING     : variant = line->Get();
                                                              break;       
      
                                        default             : XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("[GRPVECTORFILEDXF] Type Variable %s Unknown [%d]"), namevar, type);
                                                              break; 
                  
    }   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFText_XDataCtrl_Status GRPVECTORFILEDXFTEXTSECTION::IsXDataControl(int type, XSTRING& line)
* @brief      Is X data control
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* @param[in]  line : Line value.
* 
* @return     GRPVECTORFILEDXFText_XDataCtrl_Status : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFText_XDataCtrl_Status GRPVECTORFILEDXFTEXTSECTION::IsXDataControl(int type, XSTRING& line)
{
  switch(type)
    {
      case 102    :
      case 1002   : if(line.Find(_L("{"), true) != XSTRING_NOTFOUND)
                      {
                        return GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_INI;
                      }

                    if(!line.Compare(_L("}"), true))
                      {
                        return GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_END;
                      }

                    break;
    }

  return GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_NOT;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTION::ParserTextSection (XFILETXT* fileTXT)
* @brief      Parser text section
* @ingroup    GRAPHIC
* 
* @param[in]  fileTXT : File TXT pointer to use.
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILEDXFTEXTSECTION::ParserTextSection (XFILETXT* fileTXT)
{
  return GRPVECTORFILERESULT_OK;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFTEXTSECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFTEXTSECTION::Clean()
{
  type       = GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN;
  vectorfile = NULL;
}

