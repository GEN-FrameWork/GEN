/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_StyleSheet.h
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
* SINGLE PURPOSE
*   In-memory representation of a parsed CSS stylesheet, restricted to the subset that the UI module can already
*   consume through UI_STYLE. A rule is a (selector, declaration-block) pair. Declarations are stored as a
*   plain UI_STYLE bag so that Resolve() just copies key/value pairs on top of a target UI_STYLE, mirroring what
*   the XML front-end already does via FillFromXMLElement().
*
*   The supported selector grammar is deliberately minimal, aligned with GEN's widget model rather than with a
*   full CSS 2.1 engine:
*
*     Selector      := Universal | CombinatorSeq ("," CombinatorSeq)*
*     Universal     := "*"
*     CombinatorSeq := Compound (Combinator Compound)*
*     Combinator    := " " (descendant: an ancestor at ANY depth) | ">" (child: the IMMEDIATE parent only)
*     Compound      := TypeSelector? ("#" Id)? ("." Class)* (":" Pseudo)*    // at least one of the parts
*     TypeSelector  := IdentifierMatchingUI_ELEMENT_TYPE_STRING
*
*   DESCENDANT/CHILD COMBINATORS ("form .a > .b")
*   Only the RIGHTMOST compound in a CombinatorSeq is the "subject" that must match the target element -- this
*   is exactly what a UI_CSSSELECTOR already represented before combinators existed (its own type/id/classes/
*   pseudos fields), so a combinator-free selector produces the IDENTICAL object it always did. Every compound
*   to the left becomes an "ancestor step" (UI_CSSANCESTORSTEP) attached to the subject, ordered closest-
*   ancestor-first, each carrying the combinator that ties it to the step (or subject) to ITS right: CHILD means
*   that entity's immediate UI_ELEMENT::GetFather() must match; DESCENDANT means SOME ancestor at any depth must
*   (the CSS descendant combinator does not require the immediate parent). Ancestor compounds are matched with
*   an ALWAYS-EMPTY active-pseudos list (deliberately -- see UI_CSSANCESTORPROVIDER below); a pseudo-class on an
*   ancestor compound is therefore unmatchable, the same accepted degenerate case documented above for any
*   pseudo GEN's live-state model does not recognise. Specificity of a combinator sequence is the SUM of every
*   compound's own specificity (see UI_CSSSELECTOR::RecomputeSpecificity()), matching real CSS.
*
*   Matching against real ancestors needs SOMETHING that can walk "up" from the target element -- but this file
*   deliberately has no dependency on UI_ELEMENT (see the class banner's "pure-logic, no live tree" contract,
*   still true today: these are pure-logic tests with plain XSTRING/XVECTOR<XSTRING*> element identity). The
*   answer is UI_CSSANCESTORPROVIDER, a tiny caller-implemented interface (see its own doc comment) passed as an
*   optional last argument to Resolve()/HasPseudoRulesFor(); the concrete UI_ELEMENT-backed implementation lives
*   next to its only two call sites (UI_Style.cpp's FillFromCSSDeclarations(), UI_Manager.cpp's
*   PrepareElementStyleState()) rather than here, so this header still never needs to see UI_Element.h. Passing
*   NULL (the default) is always safe: any selector that HAS a combinator requirement simply cannot match then,
*   the same conservative "cannot verify, so no match" a pure-logic caller already gets for free.
*
*   No attribute selectors. `@media` supports a GEN subset: `(min-width: Npx)` / `(max-width: Npx)`
*   (optional `and` between features). Evaluation is load-time against the layout design viewport
*   (`UI_STYLESHEET::SetMediaViewport`); no re-eval on window resize (same contract as rem/vw Track L.3).
*   `@import` IS supported (see
*   UI_CSSPARSER::ReadImportStatement()/ResolveAndParseImport()), with cycle detection against the in-progress
*   import stack. Pseudo-classes are syntactically accepted (parsed into UI_CSSSELECTOR::pseudos); ":root" is
*   intercepted at parse time as the anchor for CSS custom properties (theme variables, see below) and never
*   reaches the matcher as a regular rule. Live STATE pseudo-classes are resolved against
*   UI_ELEMENT::GetActivePseudos() every time the element's state changes (see UI_ELEMENT::ReapplyStyleVisual()):
*   ":preselect" (mouse-over a selectable element; ":hover" is accepted as its exact synonym), ":selected",
*   ":active" and ":disabled" (the complement of ":active"). ":active" here means GEN's own "enabled" state, NOT
*   CSS's pointer-press state -- see the @note on UI_ELEMENT::GetActivePseudos() for the full rationale. Any
*   OTHER pseudo-class parses cleanly, adds 10 to the selector's specificity
*   (parity with regular classes), and renders the selector unmatchable at Resolve() time -- e.g. structural
*   pseudo-classes like ":first-child"/":nth-child" are silently inert; there is no DOM-style child list to
*   evaluate them against. Specificity: id=100, class/pseudo=10, type=1.
*
* THEME VARIABLES (":root" + "var(--name[, fallback])")
*   Any declaration block whose selector is exactly ":root" is intercepted by the parser and its declarations
*   are moved into UI_STYLESHEET::variables instead of becoming a regular rule. The stylesheet then walks
*   every remaining declaration value looking for "var(--name)" / "var(--name, fallback)" tokens and
*   substitutes them in place. Substitution runs at parse time (once), so Resolve() at load time sees fully
*   expanded values and never re-parses anything. Variables can reference other variables (up to a small
*   fixed number of expansion passes; deep recursion is not supported and is not intended to be).
*
*   With "@import": substitution is deliberately deferred to the OUTERMOST UI_CSSPARSER::ParseText() call in
*   an @import chain (see UI_CSSPARSER::importdepth), never to an imported file's own nested parse. This is
*   what lets "@import "theme.css"; :root { --brand-color: ...; }" work as real CSS does -- a variable the
*   IMPORTER declares AFTER the @import line still overrides the imported file's own var() uses, because every
*   file's rules and ":root" variables are fully merged into this UI_STYLESHEET before var() is resolved even
*   once, instead of the imported file substituting against a still-partial variable table of its own.
*
*   Cascade ordering, when several rules match one element: apply in ascending specificity, ties broken by
*   source order (later rule wins). Higher-specificity rules therefore always override lower ones, and equal
*   specificity resolves in the order the author wrote them - exactly what CSS does for this subset.
*
* @author     Abraham J. Velez / EndoraSoft
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"
#include "XVector.h"

#include "UI_Style.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/**
* @brief  Phase 2 ("combinadores descendiente/hijo"): the combinator tying one compound in a selector sequence
*         to the compound (or subject) immediately to its right.
*/
enum UI_CSSCOMBINATORTYPE
{
  UI_CSSCOMBINATOR_DESCENDANT = 0,   // whitespace: an ancestor at ANY depth
  UI_CSSCOMBINATOR_CHILD      = 1,   // '>': the IMMEDIATE parent only
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/**
* @brief  Phase 2 ("combinadores descendiente/hijo"): ancestor-identity accessor a caller implements once
*         (typically by walking its own UI_ELEMENT::GetFather() chain) so UI_CSSSELECTOR::Match() can evaluate
*         descendant/child combinator requirements without this file ever depending on UI_ELEMENT -- keeping
*         the "pure-logic, no live tree required" contract this whole file already promises (see the class
*         banner). depth=0 is the immediate parent of whatever element Match() was called for, depth=1 that
*         parent's own parent, and so on.
*/
class UI_CSSANCESTORPROVIDER
{
  public:
    virtual                        ~UI_CSSANCESTORPROVIDER     () {}

    virtual bool                    GetAncestor                 (int depth, XSTRING** outtype, XSTRING** outid, XVECTOR<XSTRING*>** outclasses) = 0;

    virtual bool                    FillAncestorPseudos         (int depth, XVECTOR<XSTRING*>& outpseudos)
                                      {
                                        (void)depth;
                                        (void)outpseudos;
                                        return false;
                                      }
};


class UI_CSSANCESTORSTEP;

/**
* @brief  One parsed compound selector (single alternative inside a comma-separated list). Empty type/id/class
*         lists mean "unrestricted along that axis". A UI_CSSSELECTOR with all three empty is the universal
*         selector ("*") and matches every element.
*/
class UI_CSSSELECTOR
{
  public:
                                    UI_CSSSELECTOR              ();
    virtual                        ~UI_CSSSELECTOR              ();

    XSTRING&                        GetType                     ()    { return type;    }
    XSTRING&                        GetID                       ()    { return id;      }
    XVECTOR<XSTRING*>&              GetClasses                  ()    { return classes; }
    XVECTOR<XSTRING*>&              GetPseudos                  ()    { return pseudos; }

    void                            AddClass                    (XCHAR* classname);
    void                            AddPseudo                   (XCHAR* pseudoname);

    int                             GetSpecificity              () const { return specificity; }
    void                            RecomputeSpecificity        ();

    bool                            IsRootOnly                  ();

    bool                            HasPseudos                  ()    { return pseudos.GetSize() > 0; }

    XVECTOR<UI_CSSANCESTORSTEP*>&   GetAncestorSteps            ()    { return ancestorsteps; }
    void                            AddAncestorStep             (UI_CSSSELECTOR* compound, UI_CSSCOMBINATORTYPE combinator);
    bool                            HasAncestorSteps            ()    { return ancestorsteps.GetSize() > 0; }

    bool                            Match                       (XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_CSSANCESTORPROVIDER* ancestors = NULL);

  private:

    void                            Clean                       ();

    XSTRING                         type;
    XSTRING                         id;
    XVECTOR<XSTRING*>               classes;
    XVECTOR<XSTRING*>               pseudos;
    int                             specificity;

    XVECTOR<UI_CSSANCESTORSTEP*>    ancestorsteps;
};

/**
* @brief  Phase 2 ("combinadores descendiente/hijo"): one link in a UI_CSSSELECTOR's ancestor chain -- a
*         compound selector (owned) plus the combinator tying it to whatever sits immediately to ITS right
*         (the next, closer-to-subject step, or the subject itself for step 0). See UI_CSSSELECTOR's own doc
*         comment and the class banner above for the full matching model.
*/
class UI_CSSANCESTORSTEP
{
  public:
                                    UI_CSSANCESTORSTEP          ();
    virtual                        ~UI_CSSANCESTORSTEP          ();

    UI_CSSSELECTOR*                 compound;
    UI_CSSCOMBINATORTYPE            combinator;
};


/**
* @brief  A CSS rule: one or more compound selectors sharing the same declaration block. The declarations are
*         held as a plain UI_STYLE bag; applying the rule to an element is a straight-forward key-by-key
*         Set() into that element's own UI_STYLE.
*/
class UI_CSSRULE
{
  public:
                                    UI_CSSRULE                  ();
    virtual                        ~UI_CSSRULE                  ();

    XVECTOR<UI_CSSSELECTOR*>&       GetSelectors                ()    { return selectors;    }
    UI_STYLE&                       GetDeclarations             ()    { return declarations; }


    int                             GetSourceIndex              () const { return sourceindex; }
    void                            SetSourceIndex              (int idx)                     { sourceindex = idx; }

    bool                            HasMedia                    () const { return has_media; }
    void                            ClearMedia                  ();
    void                            SetMediaWidthRange          (int min_width_px, int max_width_px);
    bool                            MatchesMediaViewport        (int viewport_w) const;

  private:

    void                            Clean                       ();

    XVECTOR<UI_CSSSELECTOR*>        selectors;
    UI_STYLE                        declarations;
    int                             sourceindex;

    bool                            has_media;
    int                             media_min_width;
    int                             media_max_width;
};


/**
* @brief  One bucket of a UI_STYLESHEET rule index: every (owned-by-`rules`) UI_CSSRULE that has at least one
*         selector constrained by this bucket's key (a type name, an id, or a class name -- one index per axis,
*         see UI_STYLESHEET::index_bytype/index_byid/index_byclass). Rule pointers here are borrowed; UI_CSSRULE
*         ownership stays with UI_STYLESHEET::rules.
*/
class UI_CSSINDEXBUCKET
{
  public:
                                    UI_CSSINDEXBUCKET           ();
    virtual                        ~UI_CSSINDEXBUCKET           ();

    XSTRING                         key;
    XVECTOR<UI_CSSRULE*>            rules;
};


/**
* @brief  A parsed stylesheet: an ordered list of rules plus a cascade resolver. The parser (UI_CSSPARSER) is
*         the only writer; the UI_MANAGER is the only consumer (via UI_STYLE::FillFromCSSDeclarations()).
*/
class UI_STYLESHEET
{
  public:
                                    UI_STYLESHEET               ();
    virtual                        ~UI_STYLESHEET               ();

    bool                            Rules_Add                   (UI_CSSRULE* rule);
    XVECTOR<UI_CSSRULE*>*           Rules_GetAll                ()    { return &rules; }
    int                             Rules_Count                 ()       { return (int)rules.GetSize(); }
    void                            Rules_DeleteAll             ();





    UI_STYLE&                       Variables                   ()    { return variables; }
    int                             Variables_Count             ()       { return variables.GetProperties() ? (int)variables.GetProperties()->GetSize() : 0; }
    bool                            Variables_Get               (XCHAR* name, XSTRING& out);





    void                            ExpandVariables             ();
    bool                            ExpandValueVars             (XSTRING& in, XSTRING& out);

    void                            SetMediaViewport            (int width_px, int height_px);
    int                             GetMediaViewportWidth       () const { return media_viewport_w; }
    int                             GetMediaViewportHeight      () const { return media_viewport_h; }





    bool                            Resolve                     (XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<XSTRING*>& activepseudos, UI_STYLE& out, UI_CSSANCESTORPROVIDER* ancestors = NULL);




    bool                            HasPseudoRulesFor           (XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, UI_CSSANCESTORPROVIDER* ancestors = NULL);

  private:

    void                            Clean                       ();


    UI_CSSINDEXBUCKET*              IndexFindBucket             (XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key);
    void                            IndexAddRule                (XVECTOR<UI_CSSINDEXBUCKET*>& index, XSTRING& key, UI_CSSRULE* rule);
    void                            IndexRule                   (UI_CSSRULE* rule);
    void                            CollectCandidateRules       (XSTRING& elementtype, XSTRING& elementid, XVECTOR<XSTRING*>& elementclasses, XVECTOR<UI_CSSRULE*>& outcandidates);
    void                            Index_DeleteAll             ();



    bool                            SubstituteVars              (XSTRING& in, XSTRING& out);

    XVECTOR<UI_CSSRULE*>            rules;
    UI_STYLE                        variables;

    XVECTOR<UI_CSSINDEXBUCKET*>     index_bytype;
    XVECTOR<UI_CSSINDEXBUCKET*>     index_byid;
    XVECTOR<UI_CSSINDEXBUCKET*>     index_byclass;
    XVECTOR<UI_CSSRULE*>            index_unrestricted;

    int                             media_viewport_w;
    int                             media_viewport_h;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
