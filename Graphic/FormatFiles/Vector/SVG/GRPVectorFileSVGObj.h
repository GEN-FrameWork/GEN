/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObj.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"
#include "XVector.h"

#include "GRPVectorFileSVGStyle.h"
#include "GRPVectorFileSVGTransform.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum GRPVECTORFILESVGOBJTYPE
{
  GRPVECTORFILESVGOBJTYPE_UNKNOWN   = 0 ,
  GRPVECTORFILESVGOBJTYPE_SVG           ,                                       // <svg>      (container / root)
  GRPVECTORFILESVGOBJTYPE_GROUP         ,                                       // <g> , <a>  (container)
  GRPVECTORFILESVGOBJTYPE_DEFS          ,                                       // <defs>     (container, not rendered directly)
  GRPVECTORFILESVGOBJTYPE_RECT          ,                                       // <rect>
  GRPVECTORFILESVGOBJTYPE_CIRCLE        ,                                       // <circle>
  GRPVECTORFILESVGOBJTYPE_ELLIPSE       ,                                       // <ellipse>
  GRPVECTORFILESVGOBJTYPE_LINE          ,                                       // <line>
  GRPVECTORFILESVGOBJTYPE_POLYLINE      ,                                       // <polyline>
  GRPVECTORFILESVGOBJTYPE_POLYGON       ,                                       // <polygon>
  GRPVECTORFILESVGOBJTYPE_PATH          ,                                       // <path>
  GRPVECTORFILESVGOBJTYPE_TEXT          ,                                       // <text> , <tspan>
  GRPVECTORFILESVGOBJTYPE_IMAGE         ,                                       // <image>
  GRPVECTORFILESVGOBJTYPE_USE           ,                                       // <use>
  GRPVECTORFILESVGOBJTYPE_LINEARGRADIENT,                                       // <linearGradient>  (paint server, not rendered directly)
  GRPVECTORFILESVGOBJTYPE_RADIALGRADIENT,                                       // <radialGradient>  (paint server, not rendered directly)
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLELEMENT;
class GRPVECTORFILESVGCSSSTYLESHEET;

class GRPVECTORFILESVGOBJ
{
  public:
                                    GRPVECTORFILESVGOBJ        ();
    virtual                        ~GRPVECTORFILESVGOBJ        ();

    static GRPVECTORFILESVGOBJ*     CreateInstance             (XFILEXMLELEMENT* element, GRPVECTORFILESVGCSSSTYLESHEET* stylesheet = NULL);

    static GRPVECTORFILESVGOBJTYPE  GetObjType                 (XSTRING& name);
    static XCHAR*                   GetObjTypeName             (GRPVECTORFILESVGOBJTYPE type);

    virtual bool                    ApplyData                  (XFILEXMLELEMENT* element);

    virtual bool                    BuildPath                  (GRP2DPATH& path);

    GRPVECTORFILESVGOBJTYPE         GetObjType                 ();
    void                            SetObjType                 (GRPVECTORFILESVGOBJTYPE type);

    bool                            IsContainer                ();

    XSTRING*                        GetID                      ();

    GRPVECTORFILESVGSTYLE*          GetStyle                   ();
    GRPVECTORFILESVGTRANSFORM*      GetTransform               ();

    XSTRING*                        GetClipPathID              ();

    XFILEXMLELEMENT*                GetXMLElement              ();
    void                            SetXMLElement              (XFILEXMLELEMENT* element);

    GRPVECTORFILESVGCSSSTYLESHEET*  GetCSSStyleSheet           ();
    void                            SetCSSStyleSheet           (GRPVECTORFILESVGCSSSTYLESHEET* stylesheet);

    XVECTOR<GRPVECTORFILESVGOBJ*>*  GetChilds                  ();
    XDWORD                          GetNChilds                 ();
    GRPVECTORFILESVGOBJ*            GetChild                   (int index);

    GRPVECTORFILESVGOBJ*            FindByID                   (XCHAR* id);

    #ifdef XTRACE_ACTIVE
    void                            Trace                      (int level = 0);
    #endif

  protected:

    bool                            BuildChilds                (XFILEXMLELEMENT* element);

    GRPVECTORFILESVGOBJTYPE         type;
    XSTRING                         id;
    XSTRING                         clippathid;
    GRPVECTORFILESVGSTYLE           style;
    GRPVECTORFILESVGTRANSFORM       transform;

    XFILEXMLELEMENT*                xmlelement;

    GRPVECTORFILESVGCSSSTYLESHEET*  cssstylesheet;

    XVECTOR<GRPVECTORFILESVGOBJ*>   childs;

  private:

    void                            Clean                      ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




