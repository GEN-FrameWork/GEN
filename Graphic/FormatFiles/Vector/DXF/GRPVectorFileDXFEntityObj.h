/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFEntityObj.h
* 
* @class      GRPVECTORFILEDXFENTITYOBJ
* @brief      Graphic Vector File DXF Entity Obj class
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

#include "XString.h"

#include "GRPVectorFileDXFEntity.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum GRPVECTORFILEDXFENTITYOBJTYPE
{
  GRPVECTORFILEDXFENTITYOBJTYPE_UNKNOWN         = 0 ,
  GRPVECTORFILEDXFENTITYOBJTYPE_3DFACE              ,  
  GRPVECTORFILEDXFENTITYOBJTYPE_ARC                 ,  
  GRPVECTORFILEDXFENTITYOBJTYPE_CIRCLE              , 
  GRPVECTORFILEDXFENTITYOBJTYPE_ELLIPSE             ,   
  GRPVECTORFILEDXFENTITYOBJTYPE_LINE                ,
  GRPVECTORFILEDXFENTITYOBJTYPE_LWPOLYLINE          ,
  GRPVECTORFILEDXFENTITYOBJTYPE_MTEXT               , 
  GRPVECTORFILEDXFENTITYOBJTYPE_POINT               , 
  GRPVECTORFILEDXFENTITYOBJTYPE_POLYLINE            , 
  GRPVECTORFILEDXFENTITYOBJTYPE_TEXT                , 
   
  GRPVECTORFILEDXFENTITYOBJTYPE_MAXELEMENTS
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPVECTORFILEDXFENTITYOBJ
{  
  public: 
      
                                        GRPVECTORFILEDXFENTITYOBJ       (XCHAR* name);
    virtual                            ~GRPVECTORFILEDXFENTITYOBJ       ();
      
    static GRPVECTORFILEDXFENTITYOBJ*   CreateInstance                  (GRPVECTORFILEDXFENTITY* entity);
      
    GRPVECTORFILEDXFENTITYOBJTYPE       GetType                         ();
    XSTRING*                            GetNameType                     ();

    XSTRING*                            GetLayerName                    ();
    
    int                                 GetLineColor                    ();
    void                                SetLineColor                    (int linecolor); 
    
    XSTRING*                            GetLineTypeName                 ();

    int                                 GetTypeSpace                    ();
    void                                SetTypeSpace                    (int typespace); 

    bool                                GetVisibility                   ();
    void                                SetVisibility                   (bool visibility);

    virtual bool                        ApplyData                       (GRPVECTORFILEDXFENTITY* entity);

    GRPVECTORFILEDXFVALUE*              GetDataValue                    (int type, GRPVECTORFILEDXFENTITY* entity);
    GRPVECTORFILEDXFVALUE*              GetDataValue                    (XCHAR* name, GRPVECTORFILEDXFENTITY* entity);

  protected:

    GRPVECTORFILEDXFENTITYOBJTYPE       type;      
    XSTRING                             nametype;      
      
    XSTRING                             layername;
    XSTRING                             linetypename;
    int                                 linecolor; 
      
    int                                 typespace;

    bool                                visibility;
      
  private: 
   
    void                                Clean                           ();        
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







