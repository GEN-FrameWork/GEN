/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSectionEntities.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XMap.h"
#include "XVariant.h"
#include "XString.h"

#include "GRPVectorFile.h"
#include "GRPVectorFileDXFEntity.h"
#include "GRPVectorFileDXFEntityObj.h"
#include "GRPVectorFileDXFTextSection.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#undef  TEST_ONLY_DEFINE_IN_ENTITITES

#define GRPVECTORFILEDXFENTITIES_MAXNDEFENTITIES      46
#define GRPVECTORFILEDXFENTITIES_MAXNDEFTYPES         90
#define GRPVECTORFILEDXFEntities_NotEnumEntity        -1

#define VFDXF_SUBCLASS                                __L("SUBCLASS") 
#define VFDXF_SUBCLASS2                               __L("SUBCLASS2") 
#define VFDXF_POINT_X                                 __L("POINT_X")
#define VFDXF_POINT_Y                                 __L("POINT_Y")
#define VFDXF_POINT_Z                                 __L("POINT_Z")
#define VFDXF_INI_POINT_X                             __L("INI_POINT_X")
#define VFDXF_INI_POINT_Y                             __L("INI_POINT_Y")
#define VFDXF_INI_POINT_Z                             __L("INI_POINT_Z")
#define VFDXF_END_POINT_X                             __L("END_POINT_X")
#define VFDXF_END_POINT_Y                             __L("END_POINT_Y")
#define VFDXF_END_POINT_Z                             __L("END_POINT_Z")
#define VFDXF_INSERTION_POINT_X                       __L("INSERTION_POINT_X") 
#define VFDXF_INSERTION_POINT_Y                       __L("INSERTION_POINT_Y") 
#define VFDXF_INSERTION_POINT_Z                       __L("INSERTION_POINT_Z") 
#define VFDXF_CENTER_POINT_X                          __L("CENTER_POINT_X")
#define VFDXF_CENTER_POINT_Y                          __L("CENTER_POINT_Y")
#define VFDXF_CENTER_POINT_Z                          __L("CENTER_POINT_Z")
#define VFDXF_LOCATION_POINT_X                        __L("CENTER_LOCATION_X")
#define VFDXF_LOCATION_POINT_Y                        __L("CENTER_LOCATION_Y")
#define VFDXF_LOCATION_POINT_Z                        __L("CENTER_LOCATION_Z")
#define VFDXF_EXTRUSION_DIRECTION_X                   __L("EXTRUSION_DIRECTION_X")
#define VFDXF_EXTRUSION_DIRECTION_Y                   __L("EXTRUSION_DIRECTION_Y")
#define VFDXF_EXTRUSION_DIRECTION_Z                   __L("EXTRUSION_DIRECTION_Z")
#define VFDXF_INI_ANGLE                               __L("INI_ANGLE")
#define VFDXF_END_ANGLE                               __L("INI_ANGLE")
#define VFDXF_RADIOUS                                 __L("RADIUS") 
#define VFDXF_INI_PARAMETER                           __L("INI_PARAMETER") 
#define VFDXF_END_PARAMETER                           __L("INI_PARAMETER") 
#define VFDXF_ANGLE_X                                 __L("ANGLE_X")
#define VFDXF_THICKNESS                               __L("THICKNESS") 


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPVECTORFILEDXFXDATACTRL;

typedef struct
{
   int                                              type;
   XCHAR*                                           name;   
   XCHAR*                                           remark;   

} GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE;

typedef struct
{
   XCHAR*                                           name;
   int                                              ntypes;
   GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE         type[GRPVECTORFILEDXFENTITIES_MAXNDEFTYPES];

} GRPVECTORFILEDXFTEXTSECTIONENTITYDEF;

class GRPVECTORFILEDXFTEXTSECTIONENTITIES : public GRPVECTORFILEDXFTEXTSECTION
{
  public:

                                                    GRPVECTORFILEDXFTEXTSECTIONENTITIES         ();
    virtual                                        ~GRPVECTORFILEDXFTEXTSECTIONENTITIES         ();

    bool                                            IsKnownEntity                               (XSTRING& nameentity);
    GRPVECTORFILEDXFTEXTSECTIONENTITYDEFTYPE*       IsKnownTypeValue                            (XSTRING& nameentity, int type);
    
    bool                                            AddEntity                                   (GRPVECTORFILEDXFENTITY* entity);
    XVECTOR<GRPVECTORFILEDXFENTITY*>*               GetEntities                                 ();
    GRPVECTORFILEDXFENTITY*                         GetEntity                                   (XCHAR* nameentity, XDWORD index);
    bool                                            DeleteEntity                                (XCHAR* nameentity, XDWORD index);
    bool                                            DeleteAllEntities                           (XCHAR* nameentity);
    bool                                            DeleteAllEntities                           ();

    XMAP<XSTRING*, int>*                            GetEnumEntitys                              ();
    int                                             GetNEntitys                                 (XCHAR* nameentity);

    XVECTOR<GRPVECTORFILEDXFENTITYOBJ*>*            GetEntitiesObj                              ();
    bool                                            DeleteAllEntitiesObj                        ();

    GRPVECTORFILERESULT                             ParserTextSection                           (XFILETXT* fileTXT);

    #ifdef XTRACE_ACTIVE    
    bool                                            ShowTraceAllEntities                        ();
    #endif

  private:
    
    int                                             GetEntityEnumIndex                          (XCHAR* nameentity);
    bool                                            AddEntityEnum                               (XCHAR* nameentity);
    bool                                            SubtractEntityEnum                          (XCHAR* nameentity);
    bool                                            SetZeroEntityEnum                           (XCHAR* nameentity);
    
    void                                            Clean                                       ();  
    
    static GRPVECTORFILEDXFTEXTSECTIONENTITYDEF     defentity[GRPVECTORFILEDXFENTITIES_MAXNDEFENTITIES];
    XMAP<XSTRING*, int>                             enumentities;
    XVECTOR<GRPVECTORFILEDXFENTITY*>                entities;
    XVECTOR<GRPVECTORFILEDXFENTITYOBJ*>             entitiesObj;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





