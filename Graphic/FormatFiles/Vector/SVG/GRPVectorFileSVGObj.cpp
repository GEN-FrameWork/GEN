/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObj.cpp
* 
* @class      GRPVECTORFILESVGOBJ
* @brief      Graphic Vector File SVG Object class (base node of the SVG object tree + factory)
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

#include "GRPVectorFileSVGObj.h"

#include "GRPVectorFileSVGCSSStyleSheet.h"

#include "GRPVectorFileSVGObjRect.h"
#include "GRPVectorFileSVGObjCircle.h"
#include "GRPVectorFileSVGObjEllipse.h"
#include "GRPVectorFileSVGObjLine.h"
#include "GRPVectorFileSVGObjPolyLine.h"
#include "GRPVectorFileSVGObjPolygon.h"
#include "GRPVectorFileSVGObjPath.h"
#include "GRPVectorFileSVGObjUse.h"
#include "GRPVectorFileSVGObjGradient.h"
#include "GRPVectorFileSVGObjText.h"

#include "XFileXML.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ::GRPVECTORFILESVGOBJ()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ::GRPVECTORFILESVGOBJ()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ::~GRPVECTORFILESVGOBJ()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ::~GRPVECTORFILESVGOBJ()
{
  childs.DeleteContents();
  childs.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::CreateInstance(XFILEXMLELEMENT* element, GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
* @brief      Create instance : build a SVG object (and its sub tree) from a XML element
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* @param[in]  stylesheet : document-wide CSS class rules (class="" resolution), NULL if the document has none
* 
* @return     GRPVECTORFILESVGOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::CreateInstance(XFILEXMLELEMENT* element, GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
{
  if(!element) return NULL;

  GRPVECTORFILESVGOBJTYPE type = GetObjType(element->GetName());

  //  The factory instantiates the concrete geometry sub class for the shapes,
  //  and the base object for containers / not yet supported elements.
  GRPVECTORFILESVGOBJ* obj = NULL;

  switch(type)
    {
      case GRPVECTORFILESVGOBJTYPE_RECT     :  obj = GEN_NEW GRPVECTORFILESVGOBJRECT();      break;
      case GRPVECTORFILESVGOBJTYPE_CIRCLE   :  obj = GEN_NEW GRPVECTORFILESVGOBJCIRCLE();    break;
      case GRPVECTORFILESVGOBJTYPE_ELLIPSE  :  obj = GEN_NEW GRPVECTORFILESVGOBJELLIPSE();   break;
      case GRPVECTORFILESVGOBJTYPE_LINE     :  obj = GEN_NEW GRPVECTORFILESVGOBJLINE();      break;
      case GRPVECTORFILESVGOBJTYPE_POLYLINE :  obj = GEN_NEW GRPVECTORFILESVGOBJPOLYLINE();  break;
      case GRPVECTORFILESVGOBJTYPE_POLYGON  :  obj = GEN_NEW GRPVECTORFILESVGOBJPOLYGON();   break;
      case GRPVECTORFILESVGOBJTYPE_PATH     :  obj = GEN_NEW GRPVECTORFILESVGOBJPATH();      break;
      case GRPVECTORFILESVGOBJTYPE_USE      :  obj = GEN_NEW GRPVECTORFILESVGOBJUSE();       break;
      case GRPVECTORFILESVGOBJTYPE_TEXT     :  obj = GEN_NEW GRPVECTORFILESVGOBJTEXT();      break;
      case GRPVECTORFILESVGOBJTYPE_LINEARGRADIENT :
      case GRPVECTORFILESVGOBJTYPE_RADIALGRADIENT :  obj = GEN_NEW GRPVECTORFILESVGOBJGRADIENT();  break;

      default                               :  obj = GEN_NEW GRPVECTORFILESVGOBJ();          break;
    }

  if(!obj) return NULL;

  obj->SetObjType(type);
  obj->SetXMLElement(element);
  obj->SetCSSStyleSheet(stylesheet);

  obj->ApplyData(element);

  return obj;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJTYPE GRPVECTORFILESVGOBJ::GetObjType(XSTRING& name)
* @brief      Get obj type : map a SVG tag name to its object type
* @ingroup    GRAPHIC
* 
* @param[in]  name : tag name
* 
* @return     GRPVECTORFILESVGOBJTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJTYPE GRPVECTORFILESVGOBJ::GetObjType(XSTRING& name)
{
  if(!name.Compare(__L("svg")     , true))  return GRPVECTORFILESVGOBJTYPE_SVG;
  if(!name.Compare(__L("g")       , true))  return GRPVECTORFILESVGOBJTYPE_GROUP;
  if(!name.Compare(__L("a")       , true))  return GRPVECTORFILESVGOBJTYPE_GROUP;
  if(!name.Compare(__L("defs")    , true))  return GRPVECTORFILESVGOBJTYPE_DEFS;
  if(!name.Compare(__L("rect")    , true))  return GRPVECTORFILESVGOBJTYPE_RECT;
  if(!name.Compare(__L("circle")  , true))  return GRPVECTORFILESVGOBJTYPE_CIRCLE;
  if(!name.Compare(__L("ellipse") , true))  return GRPVECTORFILESVGOBJTYPE_ELLIPSE;
  if(!name.Compare(__L("line")    , true))  return GRPVECTORFILESVGOBJTYPE_LINE;
  if(!name.Compare(__L("polyline"), true))  return GRPVECTORFILESVGOBJTYPE_POLYLINE;
  if(!name.Compare(__L("polygon") , true))  return GRPVECTORFILESVGOBJTYPE_POLYGON;
  if(!name.Compare(__L("path")    , true))  return GRPVECTORFILESVGOBJTYPE_PATH;
  if(!name.Compare(__L("text")    , true))  return GRPVECTORFILESVGOBJTYPE_TEXT;
  if(!name.Compare(__L("tspan")   , true))  return GRPVECTORFILESVGOBJTYPE_TEXT;
  if(!name.Compare(__L("image")   , true))  return GRPVECTORFILESVGOBJTYPE_IMAGE;
  if(!name.Compare(__L("use")     , true))  return GRPVECTORFILESVGOBJTYPE_USE;
  if(!name.Compare(__L("linearGradient"), true))  return GRPVECTORFILESVGOBJTYPE_LINEARGRADIENT;
  if(!name.Compare(__L("radialGradient"), true))  return GRPVECTORFILESVGOBJTYPE_RADIALGRADIENT;

  return GRPVECTORFILESVGOBJTYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRPVECTORFILESVGOBJ::GetObjTypeName(GRPVECTORFILESVGOBJTYPE type)
* @brief      Get obj type name
* @ingroup    GRAPHIC
* 
* @param[in]  type : object type
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRPVECTORFILESVGOBJ::GetObjTypeName(GRPVECTORFILESVGOBJTYPE type)
{
  switch(type)
    {
      case GRPVECTORFILESVGOBJTYPE_SVG       :  return __L("svg");
      case GRPVECTORFILESVGOBJTYPE_GROUP     :  return __L("group");
      case GRPVECTORFILESVGOBJTYPE_DEFS      :  return __L("defs");
      case GRPVECTORFILESVGOBJTYPE_RECT      :  return __L("rect");
      case GRPVECTORFILESVGOBJTYPE_CIRCLE    :  return __L("circle");
      case GRPVECTORFILESVGOBJTYPE_ELLIPSE   :  return __L("ellipse");
      case GRPVECTORFILESVGOBJTYPE_LINE      :  return __L("line");
      case GRPVECTORFILESVGOBJTYPE_POLYLINE  :  return __L("polyline");
      case GRPVECTORFILESVGOBJTYPE_POLYGON   :  return __L("polygon");
      case GRPVECTORFILESVGOBJTYPE_PATH      :  return __L("path");
      case GRPVECTORFILESVGOBJTYPE_TEXT      :  return __L("text");
      case GRPVECTORFILESVGOBJTYPE_IMAGE     :  return __L("image");
      case GRPVECTORFILESVGOBJTYPE_USE       :  return __L("use");

      case GRPVECTORFILESVGOBJTYPE_UNKNOWN   :
                                  default    :  return __L("unknown");
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJ::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : parse the common attributes (id, transform, style) and build the child sub tree
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJ::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  XCHAR* valueid = element->GetValueAttribute(__L("id"));
  if(valueid)  id = valueid;

  XCHAR* valuetransform = element->GetValueAttribute(__L("transform"));
  if(valuetransform)  transform.ParseFromString(valuetransform);

  //  clip-path="url(#id)" : keep the referenced id (without '#').
  XCHAR* valueclippath = element->GetValueAttribute(__L("clip-path"));
  if(valueclippath)
    {
      XSTRING clipstr(valueclippath);

      int start = clipstr.Find(__L("url(#"), true, 0);
      if(start >= 0)
        {
          start += 5;                                                           // skip "url(#"
          int end = clipstr.Find(__L(")"), false, start);
          if(end < 0)  end = (int)clipstr.GetSize();

          clipstr.Copy(start, end, clippathid);
          clippathid.DeleteCharacter(__C(' '));
        }
    }

  style.ApplyData(element, cssstylesheet);

  BuildChilds(element);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJ::BuildPath(GRP2DPATH& path)
* @brief      Build path : emit the geometry of the object into a path (base does nothing)
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* @param[in]  path : output path
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJ::BuildPath(GRP2DPATH& path)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJTYPE GRPVECTORFILESVGOBJ::GetObjType()
* @brief      Get obj type
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGOBJTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJTYPE GRPVECTORFILESVGOBJ::GetObjType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJ::SetObjType(GRPVECTORFILESVGOBJTYPE type)
* @brief      Set obj type
* @ingroup    GRAPHIC
* 
* @param[in]  type : object type
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJ::SetObjType(GRPVECTORFILESVGOBJTYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJ::IsContainer()
* @brief      Is container : true for elements that hold children (svg, g, a, defs)
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJ::IsContainer()
{
  return ((type == GRPVECTORFILESVGOBJTYPE_SVG)   ||
          (type == GRPVECTORFILESVGOBJTYPE_GROUP) ||
          (type == GRPVECTORFILESVGOBJTYPE_DEFS));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGOBJ::GetID()
* @brief      Get id
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGOBJ::GetID()
{
  return &id;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGOBJ::GetClipPathID()
* @brief      Get clip path id : referenced clipPath id (without '#'), empty if none
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGOBJ::GetClipPathID()
{
  return &clippathid;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGSTYLE* GRPVECTORFILESVGOBJ::GetStyle()
* @brief      Get style
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGSTYLE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGSTYLE* GRPVECTORFILESVGOBJ::GetStyle()
{
  return &style;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGTRANSFORM* GRPVECTORFILESVGOBJ::GetTransform()
* @brief      Get transform
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGTRANSFORM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGTRANSFORM* GRPVECTORFILESVGOBJ::GetTransform()
{
  return &transform;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEXMLELEMENT* GRPVECTORFILESVGOBJ::GetXMLElement()
* @brief      Get xml element
* @ingroup    GRAPHIC
* 
* @return     XFILEXMLELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEXMLELEMENT* GRPVECTORFILESVGOBJ::GetXMLElement()
{
  return xmlelement;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJ::SetXMLElement(XFILEXMLELEMENT* element)
* @brief      Set xml element
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJ::SetXMLElement(XFILEXMLELEMENT* element)
{
  this->xmlelement = element;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGCSSSTYLESHEET* GRPVECTORFILESVGOBJ::GetCSSStyleSheet()
* @brief      Get CSS style sheet
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGCSSSTYLESHEET* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGCSSSTYLESHEET* GRPVECTORFILESVGOBJ::GetCSSStyleSheet()
{
  return cssstylesheet;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJ::SetCSSStyleSheet(GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
* @brief      Set CSS style sheet
* @ingroup    GRAPHIC
* 
* @param[in]  stylesheet : document-wide CSS class rules
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJ::SetCSSStyleSheet(GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
{
  this->cssstylesheet = stylesheet;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPVECTORFILESVGOBJ*>* GRPVECTORFILESVGOBJ::GetChilds()
* @brief      Get childs
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<GRPVECTORFILESVGOBJ*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVECTORFILESVGOBJ*>* GRPVECTORFILESVGOBJ::GetChilds()
{
  return &childs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRPVECTORFILESVGOBJ::GetNChilds()
* @brief      Get n childs
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRPVECTORFILESVGOBJ::GetNChilds()
{
  return childs.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::GetChild(int index)
* @brief      Get child
* @ingroup    GRAPHIC
* 
* @param[in]  index : index of child
* 
* @return     GRPVECTORFILESVGOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::GetChild(int index)
{
  return childs.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::FindByID(XCHAR* id)
* @brief      Find by id : recursive search of a node by its id (this node and its descendants)
* @ingroup    GRAPHIC
* 
* @param[in]  id : id to find (without the leading '#')
* 
* @return     GRPVECTORFILESVGOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ* GRPVECTORFILESVGOBJ::FindByID(XCHAR* id)
{
  if(!id) return NULL;

  if(!this->id.IsEmpty() && !this->id.Compare(id))  return this;

  for(XDWORD c=0; c<GetNChilds(); c++)
    {
      GRPVECTORFILESVGOBJ* child = GetChild(c);
      if(child)
        {
          GRPVECTORFILESVGOBJ* found = child->FindByID(id);
          if(found)  return found;
        }
    }

  return NULL;
}


#ifdef XTRACE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJ::Trace(int level)
* @brief      Trace : dump this node and its sub tree
* @ingroup    GRAPHIC
* 
* @param[in]  level : indentation level
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJ::Trace(int level)
{
  XSTRING indent;
  for(int c=0; c<level; c++)  indent += __L(" ");

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("%s<%s> id=[%s] childs=%d fill=%d stroke=%d"), indent.Get(), GetObjTypeName(type), id.Get(), (int)GetNChilds(), style.HasFill() ?1:0, style.HasStroke()?1:0);

  for(XDWORD c=0; c<GetNChilds(); c++)
    {
      GRPVECTORFILESVGOBJ* child = GetChild(c);
      if(child)  child->Trace(level+1);
    }
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJ::BuildChilds(XFILEXMLELEMENT* element)
* @brief      Build childs : recursively create the SVG objects for every normal child element
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJ::BuildChilds(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  for(int c=0; c<element->GetNElements(); c++)
    {
      XFILEXMLELEMENT* childelement = element->GetElement(c);
      if(childelement)
        {
          if(childelement->GetType() == XFILEXMLELEMENTTYPE_NORMAL)
            {
              GRPVECTORFILESVGOBJ* childobj = GRPVECTORFILESVGOBJ::CreateInstance(childelement, cssstylesheet);
              if(childobj)  childs.Add(childobj);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJ::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJ::Clean()
{
  type        = GRPVECTORFILESVGOBJTYPE_UNKNOWN;
  xmlelement  = NULL;
  cssstylesheet = NULL;
}




