/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFEntityObj.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPVectorFileDXFEntityObj.h"

#include "GRPVectorFileDXFEntityObj3DFace.h"
#include "GRPVectorFileDXFEntityObjArc.h"
#include "GRPVectorFileDXFEntityObjCircle.h"
#include "GRPVectorFileDXFEntityObjEllipse.h"
#include "GRPVectorFileDXFEntityObjLine.h"
#include "GRPVectorFileDXFEntityObjLWPolyLine.h"
#include "GRPVectorFileDXFEntityObjMText.h"
#include "GRPVectorFileDXFEntityObjPolyLine.h"
#include "GRPVectorFileDXFEntityObjPoint.h"
#include "GRPVectorFileDXFEntityObjText.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYOBJ::GRPVECTORFILEDXFENTITYOBJ(XCHAR* nametype)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  nametype : Nametype pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYOBJ::GRPVECTORFILEDXFENTITYOBJ(XCHAR* nametype)
{
  Clean();   

  this->nametype = nametype;

  visibility = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYOBJ::~GRPVECTORFILEDXFENTITYOBJ()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYOBJ::~GRPVECTORFILEDXFENTITYOBJ()
{ 
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYOBJ* GRPVECTORFILEDXFENTITYOBJ::CreateInstance(GRPVECTORFILEDXFENTITY* entity)
* @brief      Create instance
* @ingroup    GRAPHIC
* 
* @param[in]  entity : Entity pointer to use.
* 
* @return     GRPVECTORFILEDXFENTITYOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYOBJ* GRPVECTORFILEDXFENTITYOBJ::CreateInstance(GRPVECTORFILEDXFENTITY* entity)
{   
  typedef struct
  {
    XCHAR*                          entityname;
    GRPVECTORFILEDXFENTITYOBJTYPE   objtype;
   
  } ENTITY2CLASS;

  ENTITY2CLASS                entity2class[] = {  { _L("3DFACE")           , GRPVECTORFILEDXFENTITYOBJTYPE_3DFACE         },
                                                  { _L("ARC")              , GRPVECTORFILEDXFENTITYOBJTYPE_ARC            },
                                                  { _L("CIRCLE")           , GRPVECTORFILEDXFENTITYOBJTYPE_CIRCLE         },           
                                                  { _L("ACAD_CIRCLE")      , GRPVECTORFILEDXFENTITYOBJTYPE_CIRCLE         },
                                                  { _L("ELLIPSE")          , GRPVECTORFILEDXFENTITYOBJTYPE_ELLIPSE        },
                                                  { _L("LINE")             , GRPVECTORFILEDXFENTITYOBJTYPE_LINE           },
                                                  { _L("LWPOLYLINE")       , GRPVECTORFILEDXFENTITYOBJTYPE_LWPOLYLINE     },
                                                  { _L("MTEXT")            , GRPVECTORFILEDXFENTITYOBJTYPE_MTEXT          },
                                                  { _L("POINT")            , GRPVECTORFILEDXFENTITYOBJTYPE_POINT          },
                                                  { _L("POLYLINE")         , GRPVECTORFILEDXFENTITYOBJTYPE_POLYLINE       },
                                                  { _L("TEXT")             , GRPVECTORFILEDXFENTITYOBJTYPE_TEXT           }
                                               };

  GRPVECTORFILEDXFENTITYOBJ*  entityobj     = NULL; 
  
  if(!entity) 
    {
      return entityobj;
    }

  for(int c=0; c<sizeof(entity2class)/sizeof(ENTITY2CLASS); c++)
    { 
      if(!entity->GetName()->Compare(entity2class[c].entityname, true))
        {
          switch(entity2class[c].objtype)
            {
              case GRPVECTORFILEDXFENTITYOBJTYPE_UNKNOWN      :
                                                 default      : break;
              case GRPVECTORFILEDXFENTITYOBJTYPE_3DFACE       : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJ3DFACE(entity2class[c].entityname);       break; 
              case GRPVECTORFILEDXFENTITYOBJTYPE_ARC          : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJARC(entity2class[c].entityname);          break;            
              case GRPVECTORFILEDXFENTITYOBJTYPE_CIRCLE       : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJCIRCLE(entity2class[c].entityname);       break;          
              case GRPVECTORFILEDXFENTITYOBJTYPE_ELLIPSE      : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJELLIPSE(entity2class[c].entityname);      break;         
              case GRPVECTORFILEDXFENTITYOBJTYPE_LINE         : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJLINE(entity2class[c].entityname);         break;            
              case GRPVECTORFILEDXFENTITYOBJTYPE_LWPOLYLINE   : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJLWPOLYLINE(entity2class[c].entityname);   break;      
              case GRPVECTORFILEDXFENTITYOBJTYPE_MTEXT        : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJMTEXT(entity2class[c].entityname);        break;           
              case GRPVECTORFILEDXFENTITYOBJTYPE_POINT        : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJPOINT(entity2class[c].entityname);        break;           
              case GRPVECTORFILEDXFENTITYOBJTYPE_POLYLINE     : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJPOLYLINE(entity2class[c].entityname);     break;        
              case GRPVECTORFILEDXFENTITYOBJTYPE_TEXT         : entityobj = GEN_NEW GRPVECTORFILEDXFENTITYOBJTEXT(entity2class[c].entityname);         break;                              
            }
        }
    }

  if(entityobj)
    {
      entityobj->ApplyData(entity);
    }
  
  return entityobj;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFENTITYOBJTYPE GRPVECTORFILEDXFENTITYOBJ::GetType()
* @brief      Get type
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILEDXFENTITYOBJTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFENTITYOBJTYPE GRPVECTORFILEDXFENTITYOBJ::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetNameType()
* @brief      Get name type
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetNameType()
{
  return &nametype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetLayerName()
* @brief      Get layer name
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetLayerName()
{
  return &layername;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILEDXFENTITYOBJ::GetLineColor()
* @brief      Get line color
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILEDXFENTITYOBJ::GetLineColor()
{
  return linecolor;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYOBJ::SetLineColor(int linecolor)
* @brief      Set line color
* @ingroup    GRAPHIC
* 
* @param[in]  linecolor : Linecolor value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYOBJ::SetLineColor(int linecolor)
{
  this->linecolor = linecolor;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetLineTypeName()
* @brief      Get line type name
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILEDXFENTITYOBJ::GetLineTypeName()
{
  return &linetypename;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILEDXFENTITYOBJ::GetTypeSpace()
* @brief      Get type space
* @ingroup    GRAPHIC
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILEDXFENTITYOBJ::GetTypeSpace()
{
  return typespace;
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYOBJ::SetTypeSpace(int typespace)
* @brief      Set type space
* @ingroup    GRAPHIC
* 
* @param[in]  typespace : Typespace value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYOBJ::SetTypeSpace(int typespace)
{
  this->typespace = typespace;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFENTITYOBJ::GetVisibility()
* @brief      Get visibility
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFENTITYOBJ::GetVisibility()
{
  return visibility;
}
      
     
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYOBJ::SetVisibility(bool visibility)
* @brief      Set visibility
* @ingroup    GRAPHIC
* 
* @param[in]  visibility : Visibility value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYOBJ::SetVisibility(bool visibility)
{
  this->visibility = visibility;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFENTITYOBJ::ApplyData(GRPVECTORFILEDXFENTITY* entity)
* @brief      Apply data
* @ingroup    GRAPHIC
* 
* @param[in]  entity : Entity pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFENTITYOBJ::ApplyData(GRPVECTORFILEDXFENTITY* entity)
{
  GRPVECTORFILEDXFVALUE* value;
   
  value = GetDataValue(_L("G_LAYER_NAME"), entity);   
  if(value) 
    {
      layername.Set((XCHAR*)(*value->GetData()));
    }

  value = GetDataValue(_L("G_COLOR_NUMBER"), entity);   
  if(value) 
    {
      linecolor = (int)(*value->GetData());
    }

  value = GetDataValue(_L("G_LINETYPE_NAME"), entity);      
  if(value) 
    {
      linetypename.Set((XCHAR*)(*value->GetData()));
    }

  value = GetDataValue(_L("G_TYPE_SPACE"), entity);      
  if(value) 
    {
      typespace = (int)(*value->GetData());
    }

  value = GetDataValue(_L("G_ENTITY_VISIBILITY"), entity);      
  if(value) 
    {
      visibility = ((int)(*value->GetData()))?false:true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFVALUE* GRPVECTORFILEDXFENTITYOBJ::GetDataValue(int type, GRPVECTORFILEDXFENTITY* entity)
* @brief      Get data value
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* @param[in]  entity : Entity pointer to use.
* 
* @return     GRPVECTORFILEDXFVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFVALUE* GRPVECTORFILEDXFENTITYOBJ::GetDataValue(int type, GRPVECTORFILEDXFENTITY* entity)
{
  if(!entity) 
    {
      return NULL;
    }

  for(XDWORD c=0; c<entity->GetValues()->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = entity->GetValues()->Get(c);
      if(value)
        {
          if(value->GetType() == type)
            {
              return value;
            }           
        }           
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFVALUE* GRPVECTORFILEDXFENTITYOBJ::GetDataValue(XCHAR* name, GRPVECTORFILEDXFENTITY* entity)
* @brief      Get data value
* @ingroup    GRAPHIC
* 
* @param[in]  name : Name to use.
* @param[in]  entity : Entity pointer to use.
* 
* @return     GRPVECTORFILEDXFVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFVALUE* GRPVECTORFILEDXFENTITYOBJ::GetDataValue(XCHAR* name, GRPVECTORFILEDXFENTITY* entity)
{
  if(!entity) 
    {
      return NULL;
    }

  for(XDWORD c=0; c<entity->GetValues()->GetSize(); c++)
    {
      GRPVECTORFILEDXFVALUE* value = entity->GetValues()->Get(c);
      if(value)
        {
          if(!value->GetName()->Compare(name, true))
            {
              return value;
            }           
        }           
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFENTITYOBJ::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFENTITYOBJ::Clean()
{
  type        = GRPVECTORFILEDXFENTITYOBJTYPE_UNKNOWN;
  
  nametype.Empty();
  layername.Empty();
  linetypename.Empty();

  linecolor   = 0;  

  typespace   = 0;

  visibility  = false;
}

