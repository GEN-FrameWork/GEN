/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGCSSStyleSheet.cpp
* 
* @class      GRPVECTORFILESVGCSSSTYLESHEET
* @brief      Graphic Vector File SVG CSS StyleSheet class
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

#include "GRPVectorFileSVGCSSStyleSheet.h"

#include "XFileXML.h"
#include "XVector.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGCSSSTYLESHEET::GRPVECTORFILESVGCSSSTYLESHEET()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGCSSSTYLESHEET::GRPVECTORFILESVGCSSSTYLESHEET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGCSSSTYLESHEET::~GRPVECTORFILESVGCSSSTYLESHEET()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGCSSSTYLESHEET::~GRPVECTORFILESVGCSSSTYLESHEET()
{
  classes.DeleteKeyContents();
  classes.DeleteElementContents();
  classes.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCSSSTYLESHEET::ParseDocument(XFILEXMLELEMENT* root)
* @brief      Parse document : find every &lt;style&gt; element anywhere in the document and parse its CSS text
* @ingroup    GRAPHIC
* 
* @param[in]  root : root xml element (normally the &lt;svg&gt; element)
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCSSSTYLESHEET::ParseDocument(XFILEXMLELEMENT* root)
{
  return CollectStyleElements(root);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGCSSSTYLESHEET::Get(XCHAR* classname)
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @param[in]  classname : Classname pointer to use.
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGCSSSTYLESHEET::Get(XCHAR* classname)
{
  if(!classname) return NULL;

  for(XDWORD c=0; c<classes.GetSize(); c++)
    {
      XSTRING* key = classes.GetKey(c);
      if(key)
        {
          if(!key->Compare(classname)) return classes.GetElement(c);
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGCSSSTYLESHEET::Get(XSTRING& classname)
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @param[in]  classname : Classname value.
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGCSSSTYLESHEET::Get(XSTRING& classname)
{
  return Get(classname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCSSSTYLESHEET::Add(XCHAR* classname, XCHAR* declarations)
* @brief      Add value
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  classname : Classname pointer to use.
* @param[in]  declarations : Declarations pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCSSSTYLESHEET::Add(XCHAR* classname, XCHAR* declarations)
{
  if(!classname)          return false;
  if(!classname[0])       return false;
  if(!declarations)       return false;

  XSTRING* _classname = GEN_NEW XSTRING();
  if(!_classname) return false;

  _classname->Set(classname);

  XSTRING* _declarations = GEN_NEW XSTRING();
  if(_declarations)
    {
      _declarations->Set(declarations);

      classes.Add(_classname, _declarations);
      return true;
    }

  GEN_DELETE _classname;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCSSSTYLESHEET::CollectStyleElements(XFILEXMLELEMENT* element)
* @brief      Collect style elements : recursively find every &lt;style&gt; element and parse it
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element to process
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCSSSTYLESHEET::CollectStyleElements(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  if(!element->GetName().Compare(_L("style"), true))
    {
      // NOTE: the CSS text of <style>...</style> is NOT held in this element's own GetValue(); the XML reader
      // parses it as a separate child node (typically of type XFILEXMLELEMENTTYPE_CDATA). Gather this element's
      // own value (defensive) plus every direct child's value, so the actual CSS text is never missed.
      XSTRING text;

      text += element->GetValue();

      for(int c=0; c<element->GetNElements(); c++)
        {
          XFILEXMLELEMENT* child = element->GetElement(c);
          if(child) text += child->GetValue();
        }

      ParseStyleSheetText(text);
    }

  for(int c=0; c<element->GetNElements(); c++)
    {
      XFILEXMLELEMENT* child = element->GetElement(c);
      if(child) CollectStyleElements(child);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGCSSSTYLESHEET::ParseStyleSheetText(XSTRING& text)
* @brief      Parse style sheet text : minimal CSS subset, only flat class selectors plus the bare universal
*             selector are recognized
* @note       INTERNAL: ".fil0{fill:white} .fil1,.fil2{fill:black;fill-rule:nonzero}" -> "fil0"->"fill:white" ,
*             "fil1"->"fill:black;fill-rule:nonzero" , "fil2"->"fill:black;fill-rule:nonzero". "* {fill:#8B949E}"
*             -> reserved key "*" -> "fill:#8B949E" (see GRPVECTORFILESVGSTYLE::ApplyData for how/when that
*             reserved key is applied -- always lowest priority, exactly like the real universal selector). Any
*             OTHER selector that does not start with '.' and is not exactly '*' (tag, id, @media...) is still
*             ignored; those two are what CorelDRAW/Illustrator/Figma exports and this codebase's own icon
*             generators actually use.
* @ingroup    GRAPHIC
* 
* @param[in]  text : raw &lt;style&gt; element text content
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGCSSSTYLESHEET::ParseStyleSheetText(XSTRING& text)
{
  XSTRING sheet(text);

  // Defensive: some XML readers leave the CDATA delimiters in the element text content instead of stripping
  // them; if present, remove them here so they never get mistaken for part of the first/last selector.
  sheet.Replace(_L("<![CDATA["), _L(""));
  sheet.Replace(_L("]]>"), _L(""));

  XVECTOR<XSTRING*> rules;
  sheet.Split(_C('}'), rules, false);

  for(XDWORD c=0; c<rules.GetSize(); c++)
    {
      XSTRING* rule = rules.Get(c);
      if(rule && !rule->IsEmpty())
        {
          int brace = rule->Find(_L("{"), false, 0);
          if(brace > 0)
            {
              XSTRING selectors;
              XSTRING declarations;

              rule->Copy(0, brace, selectors);
              rule->Copy(brace+1, (int)rule->GetSize(), declarations);

              XVECTOR<XSTRING*> selectorlist;
              selectors.Split(_C(','), selectorlist, false);

              for(XDWORD s=0; s<selectorlist.GetSize(); s++)
                {
                  XSTRING* selector = selectorlist.Get(s);
                  if(selector)
                    {
                      XSTRING classname(*selector);

                      classname.DeleteCharacter(_C(' '));
                      classname.DeleteCharacter(_C('\r'));
                      classname.DeleteCharacter(_C('\n'));
                      classname.DeleteCharacter(_C('\t'));

                      if((!classname.IsEmpty()) && (classname[0] == _C('.')))
                        {
                          classname.DeleteCharacters(0, 1);                        // strip the leading '.'
                          if(!classname.IsEmpty()) Add(classname.Get(), declarations.Get());
                        }
                       else if((classname.GetSize() == 1) && (classname[0] == _C('*')))
                        {
                          // Universal selector ("* {fill:#RRGGBB}"): some icon generators emit this instead of
                          // CorelDRAW's per-part ".fil0{...}" convention when they are flattening a whole icon
                          // to a single colour. Stored under the reserved key "*" (Get() does a plain string
                          // compare, so this reserved key can never collide with a real class name -- ".":
                          // stripped above -- would have to produce an actual empty or single-'*' class name for
                          // that to happen, and neither is a legal CSS class name any generator would emit).
                          // GRPVECTORFILESVGSTYLE::ApplyData applies it FIRST, before presentation attributes,
                          // class rules or inline style -- exactly the lowest-specificity position the universal
                          // selector has in a real CSS cascade, so anything more specific still overrides it.
                          Add(_L("*"), declarations.Get());
                        }
                    }
                }

              selectorlist.DeleteContents();
              selectorlist.DeleteAll();
            }
        }
    }

  rules.DeleteContents();
  rules.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGCSSSTYLESHEET::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGCSSSTYLESHEET::Clean()
{
}
