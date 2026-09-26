/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Style.h
*
* @class      UI_STYLE
* @brief      User Interface neutral property/style bag (source-format independent IR)
* @ingroup    USERINTERFACE
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XFILEXMLELEMENT;
class UI_ELEMENT;
class UI_STYLESHEET;

class UI_STYLEPROPERTY
{
  public:
                                    UI_STYLEPROPERTY            ()                                 {                       }
                                    UI_STYLEPROPERTY            (XCHAR* key, XCHAR* value)         { this->key = key; this->value = value; }
    virtual                        ~UI_STYLEPROPERTY           ()                                 {                       }

    XSTRING&                        GetKey                      ()                                 { return key;           }
    XSTRING&                        GetValue                    ()                                 { return value;         }

  private:

    XSTRING                         key;
    XSTRING                         value;
};

/**
* @brief  UI_STYLE is a neutral, source-format-independent property bag.
*
*         It mirrors exactly the (key -> string) attribute model that the loader used to read straight from the XML
*         node via UI_MANAGER::GetLayoutElementValue(). The element-construction code (GetLayoutElement_Base, and the
*         per-type builders) now read from a UI_STYLE instead of from an XFILEXMLELEMENT*, so the front-end format is
*         pluggable: today an XML front-end fills the bag (FillFromXMLElement); the GEN CSS Lite front-end (its own
*         parser/cascade, no third-party HTML/CSS engine) fills the very same bag through FillFromCSSDeclarations,
*         without touching any builder.
*
*         The typed getters reproduce the historical conversion semantics 1:1, so the refactor is behaviour-preserving:
*           - Get(key, XSTRING&)  == GetLayoutElementValue(node, key, XSTRING&)
*           - Get(key, double&)   == GetLayoutElementValue(node, key, double&)   (XSTRING::ConvertToDouble())
*/
class UI_STYLE
{
  public:
                                    UI_STYLE                    ();
    virtual                        ~UI_STYLE                    ();

    void                            Set                         (XCHAR* key, XCHAR* value);
    void                            Set                         (XCHAR* key, XSTRING& value);

    bool                            Exists                      (XCHAR* key);

    bool                            Get                         (XCHAR* key, XSTRING& value);
    bool                            Get                         (XCHAR* key, double& value);

    XVECTOR<UI_STYLEPROPERTY*>*     GetProperties               ();

    void                            DeleteAll                   ();


    bool                            FillFromXMLElement          (XFILEXMLELEMENT* node);





    bool                            FillFromCSSDeclarations     (UI_STYLESHEET* sheet, UI_ELEMENT* element);


    bool                            FillFromInlineStyle         (XSTRING& styletext);
    bool                            FillFromInlineStyle         (XSTRING& styletext, UI_STYLESHEET* expandvarsfrom);

  private:

    UI_STYLEPROPERTY*               Find                        (XCHAR* key);

    void                            Clean                       ();

    XVECTOR<UI_STYLEPROPERTY*>      properties;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
