/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_StyleSheet.cpp
*
* @class      UI_STYLESHEET
* @brief      User Interface : parsed CSS stylesheet (rule set + selector-based resolver over UI_STYLE bag).
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_StyleSheet.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS (UI_CSSSELECTOR) --------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSSELECTOR::UI_CSSSELECTOR()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSSELECTOR::UI_CSSSELECTOR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSSELECTOR::~UI_CSSSELECTOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSSELECTOR::~UI_CSSSELECTOR()
{
  for(XDWORD c=0; c<classes.GetSize(); c++)
    {
      XSTRING* classname = classes.Get(c);
      if(classname) GEN_DELETE classname;
    }

  classes.DeleteAll();

  for(XDWORD c=0; c<pseudos.GetSize(); c++)
    {
      XSTRING* pseudoname = pseudos.Get(c);
      if(pseudoname) GEN_DELETE pseudoname;
    }

  pseudos.DeleteAll();

  for(XDWORD c=0; c<ancestorsteps.GetSize(); c++)
    {
      UI_CSSANCESTORSTEP* step = ancestorsteps.Get(c);
      if(step) GEN_DELETE step;
    }

  ancestorsteps.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSSELECTOR::AddClass(XCHAR* classname)
* @brief      Add class name (owned copy).
* @ingroup    USERINTERFACE
*
* @param[in]  classname : Class name to add.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSSELECTOR::AddClass(XCHAR* classname)
{
  if(!classname) return;

  XSTRING* copy = GEN_NEW XSTRING();
  if(!copy) return;

  copy->Set(classname);
  classes.Add(copy);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSSELECTOR::AddPseudo(XCHAR* pseudoname)
* @brief      Add pseudo-class name (owned copy). Names are stored as authored, without the leading colon.
* @ingroup    USERINTERFACE
*
* @param[in]  pseudoname : Pseudo-class name to add ("root", "hover", "selected", ...).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSSELECTOR::AddPseudo(XCHAR* pseudoname)
{
  if(!pseudoname) return;

  XSTRING* copy = GEN_NEW XSTRING();
  if(!copy) return;

  copy->Set(pseudoname);
  pseudos.Add(copy);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSSELECTOR::AddAncestorStep(UI_CSSSELECTOR* compound, UI_CSSCOMBINATORTYPE combinator)
* @brief      Phase 2 ("combinadores descendiente/hijo"): append one ancestor requirement (takes ownership of
*             `compound`). Callers must append CLOSEST-ancestor-first -- see UI_CSSSELECTOR's own doc comment.
* @ingroup    USERINTERFACE
*
* @param[in]  compound : Owned compound selector this ancestor step must match.
* @param[in]  combinator : Combinator tying `compound` to whatever sits immediately to its right.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSSELECTOR::AddAncestorStep(UI_CSSSELECTOR* compound, UI_CSSCOMBINATORTYPE combinator)
{
  if(!compound) return;

  UI_CSSANCESTORSTEP* step = GEN_NEW UI_CSSANCESTORSTEP();
  if(!step)
    {
      GEN_DELETE compound;
      return;
    }

  step->compound   = compound;
  step->combinator = combinator;

  ancestorsteps.Add(step);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSSELECTOR::RecomputeSpecificity()
* @brief      Recompute cached CSS-style specificity. Pseudo-classes count the same as regular classes (=10).
*             Phase 2 ("combinadores descendiente/hijo"): a combinator sequence's specificity is the SUM of
*             every compound's own specificity, matching real CSS ("form .a > .b" is more specific than ".b"
*             alone) -- each ancestor compound already has its own specificity computed (ParseCompoundSelector()
*             calls RecomputeSpecificity() on every compound it builds, subject and ancestors alike), so this
*             just adds them in.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSSELECTOR::RecomputeSpecificity()
{
  int spec = 0;

  if(!id.IsEmpty())    spec += 100;
  if(!type.IsEmpty())  spec += 1;

  spec += (int)classes.GetSize() * 10;
  spec += (int)pseudos.GetSize() * 10;

  for(XDWORD c=0; c<ancestorsteps.GetSize(); c++)
    {
      UI_CSSANCESTORSTEP* step = ancestorsteps.Get(c);
      if(step && step->compound) spec += step->compound->GetSpecificity();
    }

  specificity = spec;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSSELECTOR::IsRootOnly()
* @brief      True iff this selector is exactly ":root" (parser hint to route the rule into variables).
* @ingroup    USERINTERFACE
*
* @return     bool : true when the selector has no type/id/class parts and exactly one pseudo named "root".
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSSELECTOR::IsRootOnly()
{
  if(!type.IsEmpty())          return false;
  if(!id.IsEmpty())            return false;
  if(classes.GetSize() > 0)    return false;
  if(pseudos.GetSize() != 1)   return false;

  XSTRING* p = pseudos.Get(0);
  if(!p) return false;

  return p->Compare(_L("root"), true) == 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSSELECTOR::Match(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_CSSANCESTORPROVIDER* ancestors)
* @brief      Test if this selector matches an element. Empty selector components are treated as wildcards.
*             Phase 2 ("combinadores descendiente/hijo"): if this selector has ancestor steps (see
*             GetAncestorSteps()), they are checked AFTER the subject's own type/id/class/pseudo checks below,
*             walking `ancestors` outward -- see the class banner in UI_StyleSheet.h for the full model.
* @ingroup    USERINTERFACE
*
* @param[in]  elementtype : Element type string (as reported by UI_ELEMENT::GetTypeString()).
* @param[in]  elementid : Element id (name).
* @param[in]  elementclasses : Element class list (may be empty).
* @param[in]  activepseudos : Pointer to activepseudos.
* @param[in]  ancestors : Ancestor-identity accessor for combinator matching; NULL if unavailable (any selector
*             with ancestor steps then cannot match -- see UI_CSSANCESTORPROVIDER's doc comment).
*
* @return     bool : true if the selector matches; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSSELECTOR::Match(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_CSSANCESTORPROVIDER* ancestors)
{
  // Type component: if set, must match element type string (case-insensitive).
  if(!type.IsEmpty())
    {
      if(elementtype.IsEmpty())                 return false;
      if(elementtype.Compare(type, true) != 0)  return false;
    }

  // Id component: if set, must match element id (case-insensitive).
  if(!id.IsEmpty())
    {
      if(elementid.IsEmpty())                 return false;
      if(elementid.Compare(id, true) != 0)    return false;
    }

  // Class components: every class listed by the selector must be present in the element's class list.
  for(XDWORD c=0; c<classes.GetSize(); c++)
    {
      XSTRING* wanted = classes.Get(c);
      if(!wanted) continue;

      bool found = false;

      for(XDWORD d=0; d<elementclasses.GetSize(); d++)
        {
          XSTRING* have = elementclasses.Get(d);
          if(!have) continue;

          if(have->Compare(*wanted, true) == 0)
            {
              found = true;
              break;
            }
        }

      if(!found) return false;
    }

  // Pseudo components: every pseudo required by the selector must be present in the element's active-pseudos
  // list. An empty active-pseudos list therefore makes any pseudo-carrying selector unmatchable (load-time /
  // stateless resolution), while an active-pseudos list like ["preselect"] enables ":preselect" rules.
  for(XDWORD c=0; c<pseudos.GetSize(); c++)
    {
      XSTRING* wanted = pseudos.Get(c);
      if(!wanted) continue;

      bool found = false;

      for(XDWORD d=0; d<activepseudos.GetSize(); d++)
        {
          XSTRING* have = activepseudos.Get(d);
          if(!have) continue;

          if(have->Compare(*wanted, true) == 0)
            {
              found = true;
              break;
            }
        }

      if(!found) return false;
    }

  // Phase 2 ("combinadores descendiente/hijo"): the subject itself matched above; now walk `ancestors` to
  // satisfy every ancestor step, CLOSEST-ancestor-first. `referencedepth` tracks the depth (per
  // UI_CSSANCESTORPROVIDER's own numbering, relative to the ORIGINAL target element) of the entity the
  // PREVIOUS step matched against; -1 means "the target element itself" (so the first step's CHILD combinator
  // looks at depth 0 = the target's own immediate parent, and its DESCENDANT combinator starts searching from
  // that same depth 0 upward).
  if(ancestorsteps.GetSize() > 0)
    {
      if(!ancestors) return false;

      int referencedepth = -1;

      XSTRING            emptystr;
      XVECTOR<XSTRING*>  emptyclasses;

      for(XDWORD c=0; c<ancestorsteps.GetSize(); c++)
        {
          UI_CSSANCESTORSTEP* step = ancestorsteps.Get(c);
          if(!step || !step->compound) return false;

          if(step->combinator == UI_CSSCOMBINATOR_CHILD)
            {
              int trydepth = referencedepth + 1;

              XSTRING*            atype     = NULL;
              XSTRING*            aid       = NULL;
              XVECTOR<XSTRING*>*  aclasses  = NULL;

              if(!ancestors->GetAncestor(trydepth, &atype, &aid, &aclasses)) return false;

              XSTRING&            at = atype    ? *atype    : emptystr;
              XSTRING&            ai = aid      ? *aid      : emptystr;
              XVECTOR<XSTRING*>&  ac = aclasses ? *aclasses : emptyclasses;

              // Live ancestor pseudos (opt-in via FillAncestorPseudos). Without them, :selected/:hover on an
              // ancestor compound can never match -- that blocked the recommended nav pattern
              // `form.nav-row:selected .nav-label` while force-pushing C++ color hacks in examples.
              XVECTOR<XSTRING*> apseudos;
              ancestors->FillAncestorPseudos(trydepth, apseudos);
              bool matched = step->compound->Match(at, ai, ac, apseudos);
              apseudos.DeleteContents();
              apseudos.DeleteAll();
              if(!matched) return false;

              referencedepth = trydepth;
            }
           else   // UI_CSSCOMBINATOR_DESCENDANT: some ancestor at ANY depth from referencedepth+1 upward
            {
              bool found    = false;
              int  trydepth = referencedepth + 1;

              while(true)
                {
                  XSTRING*            atype     = NULL;
                  XSTRING*            aid       = NULL;
                  XVECTOR<XSTRING*>*  aclasses  = NULL;

                  if(!ancestors->GetAncestor(trydepth, &atype, &aid, &aclasses)) break;

                  XSTRING&            at = atype    ? *atype    : emptystr;
                  XSTRING&            ai = aid      ? *aid      : emptystr;
                  XVECTOR<XSTRING*>&  ac = aclasses ? *aclasses : emptyclasses;

                  XVECTOR<XSTRING*> apseudos;
                  ancestors->FillAncestorPseudos(trydepth, apseudos);
                  bool matched = step->compound->Match(at, ai, ac, apseudos);
                  apseudos.DeleteContents();
                  apseudos.DeleteAll();

                  if(matched)
                    {
                      found          = true;
                      referencedepth = trydepth;
                      break;
                    }

                  trydepth++;
                }

              if(!found) return false;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSSELECTOR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSSELECTOR::Clean()
{
  specificity = 0;
}




/*---- CLASS MEMBERS (UI_CSSANCESTORSTEP) ----------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSANCESTORSTEP::UI_CSSANCESTORSTEP()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSANCESTORSTEP::UI_CSSANCESTORSTEP()
{
  compound   = NULL;
  combinator = UI_CSSCOMBINATOR_DESCENDANT;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSANCESTORSTEP::~UI_CSSANCESTORSTEP()
* @brief      Destructor of class
* @note       VIRTUAL. Owns `compound`.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSANCESTORSTEP::~UI_CSSANCESTORSTEP()
{
  if(compound) GEN_DELETE compound;
  compound = NULL;
}


/*---- CLASS MEMBERS (UI_CSSRULE) ------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSRULE::UI_CSSRULE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSRULE::UI_CSSRULE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSRULE::~UI_CSSRULE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSRULE::~UI_CSSRULE()
{
  for(XDWORD c=0; c<selectors.GetSize(); c++)
    {
      UI_CSSSELECTOR* sel = selectors.Get(c);
      if(sel) GEN_DELETE sel;
    }

  selectors.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSRULE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSRULE::Clean()
{
  sourceindex      = 0;
  has_media        = false;
  media_min_width  = -1;
  media_max_width  = -1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSRULE::ClearMedia()
* @brief      Clear optional @@media gate (rule becomes unconditional).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSRULE::ClearMedia()
{
  has_media       = false;
  media_min_width = -1;
  media_max_width = -1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_CSSRULE::SetMediaWidthRange(int min_width_px, int max_width_px)
* @brief      Attach Track B @@media (min/max-width). Pass -1 for an unbound edge.
* @ingroup    USERINTERFACE
* 
* @param[in]  min_width_px : 
* @param[in]  max_width_px : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSRULE::SetMediaWidthRange(int min_width_px, int max_width_px)
{
  has_media       = true;
  media_min_width = min_width_px;
  media_max_width = max_width_px;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_CSSRULE::MatchesMediaViewport(int viewport_w) const
* @brief      True if this rule has no media gate, or viewport_w satisfies min/max-width.
* @ingroup    USERINTERFACE
* 
* @param[in]  viewport_w : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSRULE::MatchesMediaViewport(int viewport_w) const
{
  if(!has_media) return true;
  if(viewport_w <= 0) return false;
  if(media_min_width >= 0 && viewport_w < media_min_width) return false;
  if(media_max_width >= 0 && viewport_w > media_max_width) return false;
  return true;
}




/*---- CLASS MEMBERS (UI_CSSINDEXBUCKET) -----------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSINDEXBUCKET::UI_CSSINDEXBUCKET()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSINDEXBUCKET::UI_CSSINDEXBUCKET()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSINDEXBUCKET::~UI_CSSINDEXBUCKET()
* @brief      Destructor of class
* @note       VIRTUAL. `rules` is a borrowed list (UI_CSSRULE ownership stays with UI_STYLESHEET::rules): only
*             the vector storage is released here, never the pointed-to rules.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSINDEXBUCKET::~UI_CSSINDEXBUCKET()
{
  rules.DeleteAll();
}


/*---- CLASS MEMBERS (UI_STYLESHEET) ---------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_STYLESHEET::UI_STYLESHEET()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_STYLESHEET::UI_STYLESHEET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_STYLESHEET::~UI_STYLESHEET()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_STYLESHEET::~UI_STYLESHEET()
{
  Rules_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_STYLESHEET::Rules_Add(UI_CSSRULE* rule)
* @brief      Append a rule at the end of the stylesheet (takes ownership).
* @ingroup    USERINTERFACE
*
* @param[in]  rule : Rule pointer to append; ownership is transferred to the stylesheet.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::Rules_Add(UI_CSSRULE* rule)
{
  if(!rule) return false;

  rule->SetSourceIndex((int)rules.GetSize());
  rules.Add(rule);

  IndexRule(rule);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::Rules_DeleteAll()
* @brief      Delete all owned rules.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::Rules_DeleteAll()
{
  // Index buckets/`index_unrestricted` only borrow these UI_CSSRULE pointers: drop them BEFORE the rules
  // themselves are freed below, so nothing is left dangling even momentarily.
  Index_DeleteAll();

  for(XDWORD c=0; c<rules.GetSize(); c++)
    {
      UI_CSSRULE* rule = rules.Get(c);
      if(rule) GEN_DELETE rule;
    }

  rules.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSINDEXBUCKET* UI_STYLESHEET::IndexFindBucket(XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key)
* @brief      Find the bucket for `key` inside one of the three axis indices (case-insensitive).
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  index : Index to search (index_bytype / index_byid / index_byclass).
* @param[in]  key : Bucket key to look up.
*
* @return     UI_CSSINDEXBUCKET* : the matching bucket, or NULL if none exists yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSINDEXBUCKET* UI_STYLESHEET::IndexFindBucket(XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key)
{
  for(XDWORD c=0; c<index.GetSize(); c++)
    {
      UI_CSSINDEXBUCKET* bucket = index.Get(c);
      if(!bucket) continue;

      if(bucket->key.Compare(key, true) == 0) return bucket;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::IndexAddRule(XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key, UI_CSSRULE* rule)
* @brief      File `rule` (borrowed pointer) under `key` inside one of the three axis indices, creating the
*             bucket on first use. De-duplicates: a rule with two selectors sharing the same class name is only
*             stored once per bucket.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  index : Index to update (index_bytype / index_byid / index_byclass).
* @param[in]  key : Bucket key.
* @param[in]  rule : Rule pointer to file (not owned by the bucket).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::IndexAddRule(XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key, UI_CSSRULE* rule)
{
  if(!rule) return;

  UI_CSSINDEXBUCKET* bucket = IndexFindBucket(index, key);

  if(!bucket)
    {
      bucket = GEN_NEW UI_CSSINDEXBUCKET();
      if(!bucket) return;

      bucket->key.Set(key);
      index.Add(bucket);
    }

  if(bucket->rules.Find(rule) == NOTFOUND) bucket->rules.Add(rule);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::IndexRule(UI_CSSRULE* rule)
* @brief      File a just-added rule into the type/id/class indices (one filing per selector-axis-value present
*             across all of the rule's compound selectors). A selector with none of type/id/class set (the
*             universal selector, or a bare pseudo-only selector like ":hover") cannot be restricted to any
*             bucket, so the whole rule is also added to `index_unrestricted`, which every element considers
*             regardless of its own type/id/class. Pseudos are deliberately NOT indexed on their own axis: both
*             Resolve() and HasPseudoRulesFor() only ever need a candidate set keyed by type/id/class (pseudo
*             matching still happens inside UI_CSSSELECTOR::Match(), unchanged), so a dedicated pseudo index
*             would add bookkeeping without shrinking any candidate set that matters.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  rule : Just-appended rule (already present in `rules`) to index.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::IndexRule(UI_CSSRULE* rule)
{
  if(!rule) return;

  XVECTOR<UI_CSSSELECTOR*>& sels = rule->GetSelectors();

  for(XDWORD d=0; d<sels.GetSize(); d++)
    {
      UI_CSSSELECTOR* sel = sels.Get(d);
      if(!sel) continue;

      bool restricted = false;

      if(!sel->GetType().IsEmpty())
        {
          IndexAddRule(index_bytype, sel->GetType(), rule);
          restricted = true;
        }

      if(!sel->GetID().IsEmpty())
        {
          IndexAddRule(index_byid, sel->GetID(), rule);
          restricted = true;
        }

      XVECTOR<XSTRING*>& classes = sel->GetClasses();

      for(XDWORD e=0; e<classes.GetSize(); e++)
        {
          XSTRING* classname = classes.Get(e);
          if(!classname) continue;

          IndexAddRule(index_byclass, *classname, rule);
          restricted = true;
        }

      if(!restricted)
        {
          if(index_unrestricted.Find(rule) == NOTFOUND) index_unrestricted.Add(rule);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::CollectCandidateRules(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<UI_CSSRULE*>& outcandidates)
* @brief      Build the candidate rule set for one element: `index_unrestricted` plus every bucket hit on the
*             element's own type, id and each of its classes, de-duplicated. This is a SUPERSET of every rule
*             that UI_CSSSELECTOR::Match() could accept for the element -- never a subset -- so callers can
*             iterate `outcandidates` exactly as they used to iterate the full `rules` list, with identical
*             results and no behavior change; see the index fields' doc comment in UI_StyleSheet.h for the
*             correctness argument in full.
* @note       INTERNAL. Shared by Resolve() and HasPseudoRulesFor().
* @ingroup    USERINTERFACE
*
* @param[in]  elementtype : Element type string.
* @param[in]  elementid : Element id (name).
* @param[in]  elementclasses : Element class list.
* @param[out] outcandidates : Emptied and filled with borrowed rule pointers (still owned by `rules`).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::CollectCandidateRules(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<UI_CSSRULE*>& outcandidates)
{
  outcandidates.DeleteAll();

  for(XDWORD c=0; c<index_unrestricted.GetSize(); c++)
    {
      UI_CSSRULE* rule = index_unrestricted.Get(c);
      if(rule && outcandidates.Find(rule) == NOTFOUND) outcandidates.Add(rule);
    }

  if(!elementtype.IsEmpty())
    {
      UI_CSSINDEXBUCKET* bucket = IndexFindBucket(index_bytype, elementtype);

      if(bucket)
        {
          for(XDWORD c=0; c<bucket->rules.GetSize(); c++)
            {
              UI_CSSRULE* rule = bucket->rules.Get(c);
              if(rule && outcandidates.Find(rule) == NOTFOUND) outcandidates.Add(rule);
            }
        }
    }

  if(!elementid.IsEmpty())
    {
      UI_CSSINDEXBUCKET* bucket = IndexFindBucket(index_byid, elementid);

      if(bucket)
        {
          for(XDWORD c=0; c<bucket->rules.GetSize(); c++)
            {
              UI_CSSRULE* rule = bucket->rules.Get(c);
              if(rule && outcandidates.Find(rule) == NOTFOUND) outcandidates.Add(rule);
            }
        }
    }

  for(XDWORD c=0; c<elementclasses.GetSize(); c++)
    {
      XSTRING* classname = elementclasses.Get(c);
      if(!classname) continue;

      UI_CSSINDEXBUCKET* bucket = IndexFindBucket(index_byclass, *classname);
      if(!bucket) continue;

      for(XDWORD d=0; d<bucket->rules.GetSize(); d++)
        {
          UI_CSSRULE* rule = bucket->rules.Get(d);
          if(rule && outcandidates.Find(rule) == NOTFOUND) outcandidates.Add(rule);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::Index_DeleteAll()
* @brief      Delete all owned index buckets and clear index_unrestricted. Called from Rules_DeleteAll(), which
*             discards `rules` right after -- the index must not outlive the rules it points into.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::Index_DeleteAll()
{
  for(XDWORD c=0; c<index_bytype.GetSize(); c++)
    {
      UI_CSSINDEXBUCKET* bucket = index_bytype.Get(c);
      if(bucket) GEN_DELETE bucket;
    }

  index_bytype.DeleteAll();

  for(XDWORD c=0; c<index_byid.GetSize(); c++)
    {
      UI_CSSINDEXBUCKET* bucket = index_byid.Get(c);
      if(bucket) GEN_DELETE bucket;
    }

  index_byid.DeleteAll();

  for(XDWORD c=0; c<index_byclass.GetSize(); c++)
    {
      UI_CSSINDEXBUCKET* bucket = index_byclass.Get(c);
      if(bucket) GEN_DELETE bucket;
    }

  index_byclass.DeleteAll();

  index_unrestricted.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_STYLESHEET::Resolve(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_STYLE& out, UI_CSSANCESTORPROVIDER* ancestors)
* @brief      Apply the cascade for one element into `out`. Existing keys in `out` are overwritten by matches
*             (CSS-wins semantics). Rules are applied in ascending (specificity, source-index) order, so on
*             return each key holds the value from the winning rule.
* @ingroup    USERINTERFACE
*
* @param[in]  elementtype : Element type string.
* @param[in]  elementid : Element id (name).
* @param[in]  elementclasses : Element class list.
* @param[in,out] out : Style bag to accumulate declarations into.
* @param[in]  activepseudos : Pointer to activepseudos.
* @param[in]  ancestors : Phase 2 ("combinadores descendiente/hijo"): optional ancestor-identity accessor,
*             NULL by default -- see UI_CSSANCESTORPROVIDER's doc comment in UI_StyleSheet.h.
*
* @return     bool : true if at least one rule matched; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::Resolve(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_STYLE& out, UI_CSSANCESTORPROVIDER* ancestors)
{
  // Two-pass to avoid depending on std::sort or an XVECTOR sort: collect matched (specificity, sourceindex,
  // rule*) tuples, then apply in ascending order. The tuple count is bounded by the number of rules in the
  // sheet, which is small in the target usage (one .css per layout).
  XVECTOR<int>          match_spec;
  XVECTOR<int>          match_index;
  XVECTOR<UI_CSSRULE*>  match_rule;

  // Phase 2 ("índice de reglas por id/tipo/clase"): candidates is a superset of every rule that could possibly
  // match (see CollectCandidateRules()'s doc comment), so scanning it instead of the full `rules` list is a
  // pure performance change -- the Match() call below still makes every accept/reject decision exactly as
  // before.
  XVECTOR<UI_CSSRULE*>  candidates;
  CollectCandidateRules(elementtype, elementid, elementclasses, candidates);

  for(XDWORD c=0; c<candidates.GetSize(); c++)
    {
      UI_CSSRULE* rule = candidates.Get(c);
      if(!rule) continue;
      if(!rule->MatchesMediaViewport(media_viewport_w)) continue;

      XVECTOR<UI_CSSSELECTOR*>& sels = rule->GetSelectors();

      int best_spec = -1;

      for(XDWORD d=0; d<sels.GetSize(); d++)
        {
          UI_CSSSELECTOR* sel = sels.Get(d);
          if(!sel) continue;

          if(sel->Match(elementtype, elementid, elementclasses, activepseudos, ancestors))
            {
              int s = sel->GetSpecificity();
              if(s > best_spec) best_spec = s;
            }
        }

      if(best_spec >= 0)
        {
          match_spec.Add(best_spec);
          match_index.Add(rule->GetSourceIndex());
          match_rule.Add(rule);
        }
    }

  if(match_rule.GetSize() == 0) return false;

  // Insertion-sort ascending by (specificity, sourceindex). O(n^2) is fine here: the number of matched rules
  // per element is small in practice, and this avoids introducing an STL sort dependency.
  for(XDWORD i=1; i<match_rule.GetSize(); i++)
    {
      int          s_i = match_spec.Get(i);
      int          x_i = match_index.Get(i);
      UI_CSSRULE*  r_i = match_rule.Get(i);

      XDWORD j = i;
      while(j > 0)
        {
          int s_p = match_spec.Get(j - 1);
          int x_p = match_index.Get(j - 1);

          bool greater_or_equal = (s_p < s_i) || (s_p == s_i && x_p <= x_i);
          if(greater_or_equal) break;

          match_spec .Set(j, match_spec .Get(j - 1));
          match_index.Set(j, match_index.Get(j - 1));
          match_rule .Set(j, match_rule .Get(j - 1));

          j--;
        }

      match_spec .Set(j, s_i);
      match_index.Set(j, x_i);
      match_rule .Set(j, r_i);
    }

  // Apply in ascending order: the winner (highest specificity, latest on ties) writes last and prevails.
  for(XDWORD c=0; c<match_rule.GetSize(); c++)
    {
      UI_CSSRULE* rule = match_rule.Get(c);
      if(!rule) continue;

      XVECTOR<UI_STYLEPROPERTY*>* declprops = rule->GetDeclarations().GetProperties();
      if(!declprops) continue;

      for(XDWORD d=0; d<declprops->GetSize(); d++)
        {
          UI_STYLEPROPERTY* prop = declprops->Get(d);
          if(prop) out.Set(prop->GetKey().Get(), prop->GetValue());
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_STYLESHEET::Variables_Get(XCHAR* name, XSTRING& out)
* @brief      Look up a theme variable by name ("--name"). Returns the stored value verbatim, without further
*             expansion (ExpandVariables() is expected to have run already if inter-variable references are used).
* @ingroup    USERINTERFACE
*
* @param[in]  name : Variable name including the leading "--".
* @param[out] out : Value string; emptied and left empty on miss.
*
* @return     bool : true if the variable exists; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::Variables_Get(XCHAR* name, XSTRING& out)
{
  return variables.Get(name, out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::ExpandVariables()
* @brief      Fold every var(--name[, fallback]) reference embedded in the stylesheet. Runs two phases:
*               1. Self-expand variable values (up to 3 iterations) so a variable pointing at another variable
*                  ends up holding the transitive final value.
*               2. Walk every rule's declaration values and substitute in place.
*             Safe to call more than once. Cheap on stylesheets with no var() usage (linear scan of values).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::ExpandVariables()
{
  // Phase 1: self-expand variable values. Up to 3 passes is enough to resolve one level of indirection while
  // remaining bounded even on pathological input (circular references stabilize at the last resolved form).
  for(int pass=0; pass<3; pass++)
    {
      bool changed = false;

      XVECTOR<UI_STYLEPROPERTY*>* props = variables.GetProperties();
      if(!props) break;

      for(XDWORD c=0; c<props->GetSize(); c++)
        {
          UI_STYLEPROPERTY* prop = props->Get(c);
          if(!prop) continue;

          XSTRING expanded;
          if(SubstituteVars(prop->GetValue(), expanded))
            {
              if(prop->GetValue().Compare(expanded, false) != 0)
                {
                  prop->GetValue().Set(expanded);
                  changed = true;
                }
            }
        }

      if(!changed) break;
    }

  // Phase 2: expand var() references inside regular rule declarations. Single pass is enough because Phase 1
  // has already resolved variable-to-variable indirection.
  for(XDWORD c=0; c<rules.GetSize(); c++)
    {
      UI_CSSRULE* rule = rules.Get(c);
      if(!rule) continue;

      XVECTOR<UI_STYLEPROPERTY*>* props = rule->GetDeclarations().GetProperties();
      if(!props) continue;

      for(XDWORD d=0; d<props->GetSize(); d++)
        {
          UI_STYLEPROPERTY* prop = props->Get(d);
          if(!prop) continue;

          XSTRING expanded;
          if(SubstituteVars(prop->GetValue(), expanded))
            {
              prop->GetValue().Set(expanded);
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_STYLESHEET::ExpandValueVars(XSTRING& in, XSTRING& out)
* @brief      Expand Value Vars
* @ingroup    USERINTERFACE
* 
* @param[in]  in : 
* @param[in]  out : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::ExpandValueVars(XSTRING& in, XSTRING& out)
{
  return SubstituteVars(in, out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_STYLESHEET::SubstituteVars(XSTRING& in, XSTRING& out)
* @brief      Substitute every "var(--name)" and "var(--name, fallback)" occurrence in `in` into `out`.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  in : Source string.
* @param[out] out : Destination; emptied and rewritten. Left equal to `in` when no var() token is present.
*
* @return     bool : true if at least one substitution ran; false when `in` had no var() token.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::SubstituteVars(XSTRING& in, XSTRING& out)
{
  int   len       = (int)in.GetSize();
  int   pos       = 0;
  bool  didanyth  = false;

  out.Empty();

  while(pos < len)
    {
      int found = in.Find(_L("var("), false, pos);
      if(found < 0)
        {
          // No more var(): copy the rest verbatim and finish.
          XSTRING rest;
          in.Copy(pos, len, rest);
          out += rest;
          break;
        }

      // Copy prefix [pos, found) unchanged.
      if(found > pos)
        {
          XSTRING prefix;
          in.Copy(pos, found, prefix);
          out += prefix;
        }

      int argsstart = found + 4;                 // skip "var("
      int close     = in.FindCharacter(_C(')'), (XDWORD)argsstart);
      if(close < 0)
        {
          // Unterminated var(...): treat everything from "var(" onward as literal text.
          XSTRING rest;
          in.Copy(found, len, rest);
          out += rest;
          break;
        }

      // Parse inside the parens: "--name" or "--name, fallback".
      XSTRING args;
      in.Copy(argsstart, close, args);

      XSTRING varname;
      XSTRING fallback;

      int comma = args.FindCharacter(_C(','));
      if(comma >= 0)
        {
          args.Copy(0, comma, varname);
          args.Copy(comma + 1, (int)args.GetSize(), fallback);
        }
       else
        {
          varname = args;
        }

      // Trim whitespace around varname; trim only outer whitespace of fallback (interior commas / spaces are
      // legitimate parts of tuple values like "160,195,255").
      varname.DeleteNoCharacters(_L(" \t\r\n"));

      int  fb_len   = (int)fallback.GetSize();
      int  fb_start = 0;
      while(fb_start < fb_len)
        {
          XCHAR ch = fallback[fb_start];
          if(ch != _C(' ') && ch != _C('\t') && ch != _C('\r') && ch != _C('\n')) break;
          fb_start++;
        }

      int fb_end = fb_len;
      while(fb_end > fb_start)
        {
          XCHAR ch = fallback[fb_end - 1];
          if(ch != _C(' ') && ch != _C('\t') && ch != _C('\r') && ch != _C('\n')) break;
          fb_end--;
        }

      XSTRING fb_trimmed;
      if(fb_end > fb_start) fallback.Copy(fb_start, fb_end, fb_trimmed);

      // Substitute.
      XSTRING value;
      if(Variables_Get(varname.Get(), value))
        {
          out += value;
        }
       else if(!fb_trimmed.IsEmpty())
        {
          out += fb_trimmed;
        }
      // else: missing variable and no fallback -> empty substitution.

      didanyth = true;
      pos = close + 1;
    }

  return didanyth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_STYLESHEET::HasPseudoRulesFor(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, UI_CSSANCESTORPROVIDER* ancestors)
* @brief      Cheap probe: does the stylesheet contain any pseudo-carrying rule whose type/id/class part could
*             match this element? Used at load time to skip snapshotting and state hooks for elements that no
*             pseudo rule will ever restyle.
* @ingroup    USERINTERFACE
*
* @param[in]  elementtype : Element type string.
* @param[in]  elementid : Element id (name).
* @param[in]  elementclasses : Element class list.
* @param[in]  ancestors : Phase 2 ("combinadores descendiente/hijo"): optional ancestor-identity accessor,
*             NULL by default -- see UI_CSSANCESTORPROVIDER's doc comment in UI_StyleSheet.h.
*
* @return     bool : true if the element must subscribe to state re-resolution.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_STYLESHEET::HasPseudoRulesFor(XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, UI_CSSANCESTORPROVIDER* ancestors)
{
  // Simulate "every possible pseudo active" so any pseudo-carrying selector matches its structural constraints.
  // The heuristic accepts a false positive when the sheet has pseudo rules for a different element with the
  // same identity (rare and harmless: we just do a re-resolve that changes nothing), and avoids a false
  // negative: any rule that could ever match this element is caught.
  XVECTOR<XSTRING*> allpseudos;
  XSTRING           s_root       (_L("root"));
  XSTRING           s_preselect  (_L("preselect"));
  XSTRING           s_selected   (_L("selected"));
  XSTRING           s_active     (_L("active"));
  XSTRING           s_disabled   (_L("disabled"));
  XSTRING           s_hover      (_L("hover"));
  XSTRING           s_pressed    (_L("pressed"));

  allpseudos.Add(&s_root);
  allpseudos.Add(&s_preselect);
  allpseudos.Add(&s_selected);
  allpseudos.Add(&s_active);
  allpseudos.Add(&s_disabled);
  allpseudos.Add(&s_hover);
  allpseudos.Add(&s_pressed);

  // Probe wrapper: ancestor compounds with :selected/:hover must also see the full pseudo set, otherwise
  // `form.row:selected .label` would never mark the label as style_has_state_rules at load time.
  class UI_CSS_PROBEANCESTORPROVIDER : public UI_CSSANCESTORPROVIDER
  {
    public:
      UI_CSS_PROBEANCESTORPROVIDER(UI_CSSANCESTORPROVIDER* inner, XVECTOR<XSTRING*>* probe)
        { this->inner = inner; this->probe = probe; }

      virtual bool GetAncestor(int depth, XSTRING** outtype, XSTRING** outid, XVECTOR<XSTRING*>** outclasses)
        {
          if(!inner) return false;
          return inner->GetAncestor(depth, outtype, outid, outclasses);
        }

      virtual bool FillAncestorPseudos(int depth, XVECTOR<XSTRING*>& outpseudos)
        {
          (void)depth;
          if(!probe) return false;
          for(XDWORD i=0; i<probe->GetSize(); i++)
            {
              XSTRING* src = probe->Get(i);
              if(!src) continue;
              XSTRING* copy = GEN_NEW XSTRING();
              if(copy) { copy->Set(src->Get()); outpseudos.Add(copy); }
            }
          return true;
        }

    private:
      UI_CSSANCESTORPROVIDER* inner;
      XVECTOR<XSTRING*>*      probe;
  };

  UI_CSS_PROBEANCESTORPROVIDER probeancestors(ancestors, &allpseudos);
  UI_CSSANCESTORPROVIDER*      matchancestors = ancestors ? (UI_CSSANCESTORPROVIDER*)&probeancestors : NULL;

  // Phase 2 ("índice de reglas por id/tipo/clase"): same superset-candidate optimization as Resolve(), see
  // CollectCandidateRules()'s doc comment.
  XVECTOR<UI_CSSRULE*> candidates;
  CollectCandidateRules(elementtype, elementid, elementclasses, candidates);

  for(XDWORD c=0; c<candidates.GetSize(); c++)
    {
      UI_CSSRULE* rule = candidates.Get(c);
      if(!rule) continue;
      if(!rule->MatchesMediaViewport(media_viewport_w)) continue;

      XVECTOR<UI_CSSSELECTOR*>& sels = rule->GetSelectors();

      for(XDWORD d=0; d<sels.GetSize(); d++)
        {
          UI_CSSSELECTOR* sel = sels.Get(d);
          if(!sel) continue;
          if(!sel->HasPseudos() && !sel->HasAncestorSteps()) continue;

          // Ancestor-only pseudos (pseudo on a parent compound, not the subject) still count as state rules.
          bool has_any_pseudo = sel->HasPseudos();
          if(!has_any_pseudo && sel->HasAncestorSteps())
            {
              XVECTOR<UI_CSSANCESTORSTEP*>& steps = sel->GetAncestorSteps();
              for(XDWORD s=0; s<steps.GetSize(); s++)
                {
                  UI_CSSANCESTORSTEP* step = steps.Get(s);
                  if(step && step->compound && step->compound->HasPseudos()) { has_any_pseudo = true; break; }
                }
            }
          if(!has_any_pseudo) continue;

          if(sel->Match(elementtype, elementid, elementclasses, allpseudos, matchancestors))
            {
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_STYLESHEET::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::Clean()
{
  media_viewport_w = 0;
  media_viewport_h = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_STYLESHEET::SetMediaViewport(int width_px, int height_px)
* @brief      Track B: set design viewport used to gate @@media (min/max-width) rules.
* @ingroup    USERINTERFACE
* 
* @param[in]  width_px : 
* @param[in]  height_px : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_STYLESHEET::SetMediaViewport(int width_px, int height_px)
{
  media_viewport_w = (width_px  > 0) ? width_px  : 0;
  media_viewport_h = (height_px > 0) ? height_px : 0;
}
