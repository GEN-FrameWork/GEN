/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_LayoutBox.h
*
* @class      UI_LAYOUTBOX
* @brief      User Interface : one node of the CSS top-left layout box tree -- pure geometry, no painting.
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
* SCOPE (Phase 3 -- "caja, unidades y posicionamiento", third increment: "pequeños primero")
*   The roadmap's first Phase 3 bullet is "árbol de layout top-left independiente de las coordenadas de pintura
*   GEN (borde inferior)". This is that tree, and ONLY that tree: a pure geometry data structure (content box +
*   padding/border/margin edges, in CSS's own top-left/+Y-down convention -- see UI_CSSAdapter.h for the existing
*   bridge to GEN's bottom-anchored storage) plus owned parent/child links, with NOTHING behind it yet --
*
*     - it is not built FROM a UI_ELEMENT tree anywhere in this increment (a caller populates it by hand, exactly
*       like the tests below do);
*     - it does not run any layout algorithm (no flow, no percentage resolution, no min/max clamping applied --
*       though UI_COMPUTEDSTYLE::ClampToMinMax()/ResolveContentSize() from the previous increment are exactly
*       the kind of helper a future layout pass would call while filling one of these in);
*     - it does not replace or touch UI_SKINCANVAS::CalculePosition() (UI_Manager.cpp/UI_SkinCanvas.cpp are
*       untouched by this increment);
*     - it is not read by UI_CSSAdapter.h's UI_CSSBox_Get/Set (those still talk directly to UI_ELEMENT).
*
*   That wiring -- actually walking father/compose_elements, actually computing sizes from UI_COMPUTEDSTYLE, and
*   actually driving GEN's rendering through it -- is the fourth (and by far largest) Phase 3 increment: the
*   real UI_LAYOUTENGINE. Until then, UI_LAYOUTBOX exists purely so that increment can be built and tested
*   against a known-correct, already-tested geometry container instead of inventing one under time pressure.
*
* SCOPE ADDENDUM (fourth increment, second sub-step: "position:relative/absolute + insets")
*   Added here: a UI_POSITION tag per box, and a per-edge "specified or not" inset set (UI_LAYOUTBOX_INSETS --
*   NOT reusing UI_LAYOUTBOX_EDGES, because a CSS inset can be legitimately absent/"auto" on any one edge, unlike
*   padding/border/margin which are always numeric). Still pure data: UI_LAYOUTBOX itself applies no positioning
*   math -- that is UI_LAYOUTENGINE::ApplyPositioning()'s job (UI_LayoutEngine.h), reading exactly what is set
*   here. A caller (test, or the engine once wired) populates SetPosition()/SetInsets() by hand; this sub-step
*   still does not read UI_COMPUTEDSTYLE's "position"/"top"/"right"/"bottom"/"left" properties anywhere. The
*   third (and last) sub-step of this increment wires UI_LAYOUTENGINE's output to UI_CSSBox_Set() with the two
*   strategies (legacy/CSS) selectable -- it does NOT add CSS-rule-driven position/insets either: there is
*   currently no per-UI_ELEMENT stored computed style to read them from (GetLayoutElement_Base() in UI_Manager.cpp
*   builds a UI_COMPUTEDSTYLE transiently per XML node at parse time and does not persist it on the element), so
*   making "position:relative" in an actual stylesheet drive an actual UI_ELEMENT remains future work, tracked
*   there rather than promised here.
*
* SCOPE ADDENDUM (Phase 4 -- "Flexbox/Grid y migración del ejemplo", first increment: "Flexbox básico", first
*   sub-step: data model + single-line main-axis placement via justify-content)
*   Added here: the per-container flex properties (UI_FLEX_DIRECTION, UI_JUSTIFY_CONTENT, row/column gap) and a
*   "is this box a flex container" flag. Pure storage again, same shape as UI_POSITION/UI_LAYOUTBOX_INSETS above
*   -- UI_LAYOUTBOX applies no flex math itself, UI_LAYOUTENGINE::ApplyFlexLayout() does (UI_LayoutEngine.h).
*
*   Deliberately NOT yet covered (later Flexbox sub-steps, same increment):
*
*     - flex-wrap (multi-line) and align-content across lines.
*     - flex-grow/flex-shrink/flex-basis -- every child's CONTENT size is used exactly as already set (its
*       "hypothetical main size" in CSS terms); nothing here grows or shrinks a child to fill/fit the container.
*     - align-items/align-self on the cross axis -- every child is placed at the container's cross-axis START
*       (the CSS flex-start behaviour), regardless of the child's own cross size. Real "stretch"/"center"/etc.
*       cross alignment is later work.
*     - as with UI_POSITION/insets, nothing reads these properties from a real stylesheet -- a caller (test, or
*       a future wiring step) sets SetFlexContainer()/SetFlexDirection()/etc. by hand.
*
* SCOPE ADDENDUM (Phase 4, first increment, second sub-step: "flex-grow/flex-shrink/flex-basis")
*   Added here: the per-ITEM sizing properties -- flex-grow (default 0, CSS initial value), flex-shrink (default
*   1, CSS initial value) and flex-basis (UI_LAYOUTBOX_INSET reused verbatim: "auto", the default, or a resolved
*   value -- exactly the same "specified or not" shape a CSS inset already needed, so no new struct). These are
*   properties of a box AS A FLEX ITEM (how ITS OWN size is resolved by its flex-container PARENT), the mirror
*   image of the previous sub-step's container-side properties. Still pure storage -- UI_LAYOUTENGINE's
*   ArrangeFlexChildren() (UI_LayoutEngine.h) is what reads these to grow/shrink each child's CONTENT main size
*   before packing it, exactly as CSS's flex sizing algorithm does (basis -> hypothetical size -> grow OR shrink
*   the free/negative space across every item, weighted by grow factor or by shrink-factor*base-size).
*
* SCOPE ADDENDUM (Phase 4, first increment, third sub-step: "flex-wrap + align-content")
*   Added here: two more per-container properties -- UI_FLEX_WRAP (nowrap, the CSS initial value; wrap; or
*   wrap-reverse) and UI_ALIGN_CONTENT (how multiple LINES, not individual items, are distributed across the
*   container's cross axis once wrapping produces more than one). Same shape as every other container property
*   so far: pure storage, defaults to NOWRAP / FLEX_START, interpreted entirely by UI_LAYOUTENGINE.
*
*   UI_ALIGN_CONTENT intentionally has NO "stretch" value (CSS's real initial value for align-content is
*   "normal", which behaves like stretch for a flex container): stretching a LINE only has a visible effect if
*   the ITEMS inside it also stretch to fill it (align-items:stretch), and that per-item cross-sizing does not
*   exist yet (still deferred, see the first sub-step's SCOPE ADDENDUM above) -- so a value this class cannot
*   yet make meaningful is left out rather than added as a silent no-op. UI_LAYOUTENGINE::ArrangeFlexChildren()
*   (UI_LayoutEngine.h) is where this actually applies, once wrapping and align-content land there together.
*
* SCOPE ADDENDUM (Phase 4, first increment, fourth and last sub-step: "align-items + align-self")
*   Added here: UI_ALIGN_ITEMS (per-container: how every child is aligned on the CROSS axis, within its own line)
*   and UI_ALIGN_SELF (per-item: overrides the container's align-items for that one child; AUTO, the default,
*   means "defer to the container"). Unlike UI_ALIGN_CONTENT above, "stretch" IS included here, because this is
*   exactly the per-item cross-sizing that the earlier ADDENDUMs said stretch depends on -- UI_LAYOUTENGINE now
*   resizes a STRETCH-aligned child's cross size to fill its line (see UI_LayoutEngine.h).
*
*   Deliberate deviation from CSS: UI_ALIGN_ITEMS defaults to FLEX_START, not CSS's real initial value ("normal",
*   which behaves as stretch). Every previous Flexbox sub-step's tests already lock in flex-start cross placement
*   as the default (an item keeps its own content size and sits at its line's cross-start unless told otherwise);
*   defaulting to CSS-correct stretch here would silently change every earlier test's and caller's behavior.
*   Callers who want real CSS "normal" behaviour set SetAlignItems(UI_ALIGN_ITEMS_STRETCH) explicitly.
*
*   UI_ALIGN_CONTENT's "stretch" value is still deliberately NOT added even now: it would stretch a LINE's cross
*   size itself (not an item within a fixed-size line, which is what this sub-step does), a distinct and still
*   unimplemented behavior -- tracked as future work, not silently approximated by this sub-step's per-item
*   stretch.
*
* SCOPE ADDENDUM (Phase 4, second increment: "Grid básico", first sub-step: fixed-size tracks (px/%) + gap +
*   row-major auto-placement)
*   Added here: UI_GRID_TRACK_UNIT/UI_GRIDTRACK (one column or row track: a size in pixels or a percentage of the
*   grid container's content size on that axis), a "is this box a grid container" flag, and two per-container
*   track lists (grid-template-columns / grid-template-rows, added one track at a time via
*   AddGridColumnTrack()/AddGridRowTrack()). Row/column GAP reuses the exact same SetGap()/GetRowGap()/
*   GetColumnGap() Flexbox already added above -- CSS's "gap"/"row-gap"/"column-gap" are shared properties, not
*   Flexbox- or Grid-specific, so there is nothing Grid-specific to store for them.
*
*   Pure storage again, same shape as every Flexbox property above -- UI_LAYOUTBOX applies no grid math itself;
*   UI_LAYOUTENGINE::ApplyGridLayout() does (UI_LayoutEngine.h). A caller (test, or a future wiring step) sets
*   SetGridContainer()/AddGridColumnTrack()/etc. by hand; nothing here reads a real stylesheet's
*   grid-template-columns/rows yet.
*
*   Deliberately NOT yet covered (later Grid sub-steps, same increment): `minmax()` track sizing; `grid-column`/
*   `grid-row` spans (every item occupies exactly one cell for now, placed in row-major document order -- CSS
*   calls this "auto-flow: row", the initial value, which is the only flow this sub-step implements); implicit
*   tracks sized by content (if grid-template-rows is empty, UI_LAYOUTENGINE still generates one implicit row
*   per line of content, sized to its tallest item -- see UI_LayoutEngine.h -- but there is no way yet to size
*   an EXPLICIT track by content, only by px/%/fr); justify-items/align-items/justify-self/align-self for grid
*   (every item fills its cell exactly, CSS's real default of "stretch" on both axes -- see UI_LayoutEngine.h
*   for why that default was chosen here).
*
* SCOPE ADDENDUM (Phase 4, second increment, second sub-step: the `fr` track unit)
*   UI_GRID_TRACK_UNIT gained UI_GRID_TRACK_UNIT_FR: a UI_GRIDTRACK sharing the free space left on its axis
*   after every px/% track (and the axis's gaps) are accounted for, weighted by its own `value` against the sum
*   of every `fr` track's `value` on that same axis -- CSS's real `fr` semantics, and the direct Grid analogue
*   of Flexbox's flex-grow (see UI_LayoutBox.h's Flexbox SCOPE ADDENDUM for that mirror-image property). Still
*   pure storage here: UI_LAYOUTENGINE::ResolveTrackSizesWithFr() (UI_LayoutEngine.h) is what actually computes
*   an `fr` track's share, for both grid-template-columns and grid-template-rows.
*
* SCOPE ADDENDUM (Phase 4, second increment, third sub-step: "acotados" minmax() tracks)
*   UI_GRIDTRACK gained a second shape: isminmax == true turns it into a minmax(min, max) bound instead of a
*   single unit/value size -- "min" (minunit/minvalue) is restricted to PX or PERCENT, matching real CSS (a
*   minmax() minimum can never be a \<flex\>/`fr` value there either); "max" (maxunit/maxvalue) accepts PX,
*   PERCENT, or FR, covering the common "minmax(200px, 1fr)" idiom -- a track that never shrinks below its
*   floor but still grows to absorb free space. There is no builder method for this (unlike the simple-track
*   Add*Track() calls above) -- a caller fills in the fields directly, exactly as the tests below do; the
*   struct's in-class field defaults (see UI_GRIDTRACK itself) mean every EARLIER sub-step's track-building code
*   (which only ever sets "unit"/"value") keeps producing a plain, non-minmax track unchanged.
*
*   How a minmax() track resolves is UI_LAYOUTENGINE::ResolveTrackSizesWithFr()'s job (UI_LayoutEngine.h) --
*   see its own SCOPE ADDENDUM for the exact algorithm and why "min" acting as a reserved floor, with "max"
*   (when `fr`) sharing whatever's left, is the bounded approximation this sub-step deliberately chose over
*   real CSS's iterative, content-aware track sizing algorithm.
*
* SCOPE ADDENDUM (Phase 4, second increment, fourth and last sub-step: "Grid básico" -- grid-column/grid-row
*   spans)
*   Added here: gridcolumnspan/gridrowspan, two per-ITEM properties (a box's own span across each axis, CSS's
*   grid-column/grid-row -- though only the SPAN part; explicit start/end line placement, e.g. "grid-column:
*   2 / 4", is still not covered -- see below), defaulting to 1/1 (CSS's initial value: occupy exactly one
*   cell). Pure storage again, same shape as every other Grid/Flexbox item property above --
*   UI_LAYOUTENGINE::ArrangeGridChildren() (UI_LayoutEngine.h) is what actually auto-places a spanning item
*   across the right number of cells.
*
*   Deliberately still NOT covered, even now that Grid básico's four planned sub-steps are all in: explicit
*   grid-column-start/end / grid-row-start/end line-based placement (this sub-step ONLY changes how big an
*   auto-placed item is, never WHERE it starts -- auto-placement, in row-major document order, remains the only
*   placement algorithm); "auto-flow: column" (row-major only, as since the first sub-step); "dense" packing
*   (CSS can backfill earlier gaps a later, smaller item would fit into -- this engine's placement cursor only
*   ever moves forward, CSS's "sparse" algorithm, the initial value); named grid lines/areas; content-based
*   ("auto") explicit track sizing; justify-items/align-items/justify-self/align-self (still CSS's real default
*   of stretch on both axes for every item, span or not -- see the first sub-step's SCOPE ADDENDUM above).
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/**
* @struct  UI_LAYOUTBOX_EDGES
* @brief   Four edge widths (CSS order: top/right/bottom/left), reused identically for padding, border and margin.
*/
struct UI_LAYOUTBOX_EDGES
{
  double top;
  double right;
  double bottom;
  double left;
};

/**
* @enum   UI_POSITION
* @brief  CSS "position" property, restricted to the values this engine currently understands.
*/
enum UI_POSITION
{
  UI_POSITION_STATIC = 0,           // default: participates in normal flow, never a containing block for others
  UI_POSITION_RELATIVE,             // offset from its own static position; still a containing block for descendants
  UI_POSITION_ABSOLUTE              // positioned against its nearest non-STATIC ancestor (or the tree root)
};

/**
* @struct  UI_LAYOUTBOX_INSET
* @brief   One CSS inset edge (top/right/bottom/left): either unspecified ("auto", the default) or a resolved value.
*/
struct UI_LAYOUTBOX_INSET
{
  bool   specified;
  double value;
};

/**
* @struct  UI_LAYOUTBOX_INSETS
* @brief   The four CSS inset edges, each independently specified-or-not.
*/
struct UI_LAYOUTBOX_INSETS
{
  UI_LAYOUTBOX_INSET top;
  UI_LAYOUTBOX_INSET right;
  UI_LAYOUTBOX_INSET bottom;
  UI_LAYOUTBOX_INSET left;
};

/**
* @enum   UI_FLEX_DIRECTION
* @brief  CSS "flex-direction": which axis is the main axis, and which way it runs.
*/
enum UI_FLEX_DIRECTION
{
  UI_FLEX_DIRECTION_ROW = 0,          // main axis horizontal, left to right (CSS initial value)
  UI_FLEX_DIRECTION_ROW_REVERSE,      // main axis horizontal, right to left
  UI_FLEX_DIRECTION_COLUMN,           // main axis vertical, top to bottom
  UI_FLEX_DIRECTION_COLUMN_REVERSE    // main axis vertical, bottom to top
};

/**
* @enum   UI_JUSTIFY_CONTENT
* @brief  CSS "justify-content": how free main-axis space is distributed among a flex line's children.
*/
enum UI_JUSTIFY_CONTENT
{
  UI_JUSTIFY_CONTENT_FLEX_START = 0,   // pack at the main-axis start (CSS initial value)
  UI_JUSTIFY_CONTENT_FLEX_END,         // pack at the main-axis end
  UI_JUSTIFY_CONTENT_CENTER,           // pack in the middle
  UI_JUSTIFY_CONTENT_SPACE_BETWEEN,    // no leading/trailing space; free space split between children
  UI_JUSTIFY_CONTENT_SPACE_AROUND,     // free space split around each child (half-size gaps at the two ends)
  UI_JUSTIFY_CONTENT_SPACE_EVENLY      // free space split into equal gaps, ends included
};


/**
* @enum   UI_FLEX_WRAP
* @brief  CSS "flex-wrap": whether a flex container's items are forced onto a single line or may wrap onto more.
*/
enum UI_FLEX_WRAP
{
  UI_FLEX_WRAP_NOWRAP = 0,      // single line; items never wrap (CSS initial value)
  UI_FLEX_WRAP_WRAP,            // wraps onto additional lines, packed in the cross-axis-start-to-end direction
  UI_FLEX_WRAP_WRAP_REVERSE     // wraps onto additional lines, packed in the cross-axis-end-to-start direction
};


/**
* @enum   UI_ALIGN_CONTENT
* @brief  CSS "align-content": how free cross-axis space is distributed among a flex container's LINES (plural
*         -- see UI_JUSTIFY_CONTENT for the equivalent distribution of free space among a single line's items).
*/
enum UI_ALIGN_CONTENT
{
  UI_ALIGN_CONTENT_FLEX_START = 0,     // pack lines at the cross-axis start
  UI_ALIGN_CONTENT_FLEX_END,           // pack lines at the cross-axis end
  UI_ALIGN_CONTENT_CENTER,             // pack lines in the middle
  UI_ALIGN_CONTENT_SPACE_BETWEEN,      // no leading/trailing space; free space split between lines
  UI_ALIGN_CONTENT_SPACE_AROUND,       // free space split around each line (half-size gaps at the two ends)
  UI_ALIGN_CONTENT_SPACE_EVENLY        // free space split into equal gaps, ends included
};


/**
* @enum   UI_ALIGN_ITEMS
* @brief  CSS "align-items": how a flex container aligns its children on the CROSS axis, within each one's own
*         line. See the SCOPE ADDENDUM above for why the default here is FLEX_START rather than CSS's "normal".
*/
enum UI_ALIGN_ITEMS
{
  UI_ALIGN_ITEMS_FLEX_START = 0,       // pack at the line's cross-axis start (this class's default -- see above)
  UI_ALIGN_ITEMS_FLEX_END,             // pack at the line's cross-axis end
  UI_ALIGN_ITEMS_CENTER,               // pack in the middle of the line's cross size
  UI_ALIGN_ITEMS_STRETCH                // grow the child's cross size to fill the line (CSS's real default)
};


/**
* @enum   UI_ALIGN_SELF
* @brief  CSS "align-self": one child's own override of its flex container's align-items. AUTO (the default)
*         means "use whatever the container's align-items says" -- see UI_ALIGN_ITEMS above for the other values.
*/
enum UI_ALIGN_SELF
{
  UI_ALIGN_SELF_AUTO = 0,              // defer to the container's align-items (CSS initial value, this default)
  UI_ALIGN_SELF_FLEX_START,
  UI_ALIGN_SELF_FLEX_END,
  UI_ALIGN_SELF_CENTER,
  UI_ALIGN_SELF_STRETCH
};


/**
* @enum   UI_GRID_TRACK_UNIT
* @brief  How one grid track's UI_GRIDTRACK::value is interpreted. See the SCOPE ADDENDUM above for the units
*         (`fr`, `minmax()`) deliberately not yet included.
*/
enum UI_GRID_TRACK_UNIT
{
  UI_GRID_TRACK_UNIT_PX = 0,           // absolute pixels
  UI_GRID_TRACK_UNIT_PERCENT,          // percentage (0..100) of the grid container's content size on that axis
  UI_GRID_TRACK_UNIT_FR                 // a share of the FREE space left after every other track is sized
};


/**
* @struct  UI_GRIDTRACK
* @brief   One column or row track of a grid container's grid-template-columns/rows: either a simple size (unit
*          + value) or a minmax(min, max) bound -- see the SCOPE ADDENDUM above for exactly what each field
*          means in the minmax() case, and the deliberate restriction that "min" is never `fr` (matching real
*          CSS: a minmax() minimum can never be a \<flex\> value either).
*/
struct UI_GRIDTRACK
{
  bool                isminmax = false;

  UI_GRID_TRACK_UNIT unit  = UI_GRID_TRACK_UNIT_PX;
  double              value = 0.0;

  UI_GRID_TRACK_UNIT minunit  = UI_GRID_TRACK_UNIT_PX;
  double              minvalue = 0.0;
  UI_GRID_TRACK_UNIT maxunit  = UI_GRID_TRACK_UNIT_PX;
  double              maxvalue = 0.0;

  bool operator==(const UI_GRIDTRACK& other) const
  {
    return (isminmax == other.isminmax) && (unit == other.unit) && (value == other.value) &&
           (minunit == other.minunit) && (minvalue == other.minvalue) &&
           (maxunit == other.maxunit) && (maxvalue == other.maxvalue);
  }
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class UI_LAYOUTBOX
{
  public:
                                    UI_LAYOUTBOX                 ();
    virtual                        ~UI_LAYOUTBOX                 ();

    void                            SetContentBox                 (double left, double top, double width, double height);
    double                          GetContentLeft                ();
    double                          GetContentTop                  ();
    double                          GetContentWidth                ();
    double                          GetContentHeight               ();

    void                            SetPadding                     (double top, double right, double bottom, double left);
    void                            SetBorder                      (double top, double right, double bottom, double left);
    void                            SetMargin                      (double top, double right, double bottom, double left);
    UI_LAYOUTBOX_EDGES&              GetPadding                     ();
    UI_LAYOUTBOX_EDGES&              GetBorder                      ();
    UI_LAYOUTBOX_EDGES&              GetMargin                      ();

    void                            GetPaddingBox                  (double& left, double& top, double& width, double& height);
    void                            GetBorderBox                   (double& left, double& top, double& width, double& height);
    void                            GetMarginBox                   (double& left, double& top, double& width, double& height);

    void                            AddChild                       (UI_LAYOUTBOX* child);
    XVECTOR<UI_LAYOUTBOX*>&          GetChildren                    ();
    UI_LAYOUTBOX*                    GetParent                      ();

    void                            SetPosition                    (UI_POSITION position);
    UI_POSITION                     GetPosition                    ();
    void                            SetInsets                      (UI_LAYOUTBOX_INSETS& insets);
    UI_LAYOUTBOX_INSETS&             GetInsets                      ();

    void                            SetFlexContainer                (bool isflexcontainer);
    bool                            IsFlexContainer                 ();
    void                            SetFlexDirection                (UI_FLEX_DIRECTION direction);
    UI_FLEX_DIRECTION               GetFlexDirection                ();
    void                            SetJustifyContent               (UI_JUSTIFY_CONTENT justifycontent);
    UI_JUSTIFY_CONTENT              GetJustifyContent                ();
    void                            SetGap                          (double rowgap, double columngap);
    double                          GetRowGap                       ();
    double                          GetColumnGap                    ();

    void                            SetFlexGrow                     (double grow);
    double                          GetFlexGrow                     ();
    void                            SetFlexShrink                   (double shrink);
    double                          GetFlexShrink                   ();
    void                            SetFlexBasisAuto                ();
    void                            SetFlexBasis                    (double value);
    UI_LAYOUTBOX_INSET&              GetFlexBasis                    ();

    void                            SetFlexWrap                     (UI_FLEX_WRAP wrap);
    UI_FLEX_WRAP                    GetFlexWrap                     ();
    void                            SetAlignContent                 (UI_ALIGN_CONTENT aligncontent);
    UI_ALIGN_CONTENT                GetAlignContent                  ();

    void                            SetAlignItems                   (UI_ALIGN_ITEMS alignitems);
    UI_ALIGN_ITEMS                  GetAlignItems                    ();
    void                            SetAlignSelf                    (UI_ALIGN_SELF alignself);
    UI_ALIGN_SELF                   GetAlignSelf                     ();

    void                            SetGridContainer                (bool isgridcontainer);
    bool                            IsGridContainer                  ();
    void                            AddGridColumnTrack               (UI_GRIDTRACK track);
    void                            ClearGridColumnTracks            ();
    XVECTOR<UI_GRIDTRACK>&           GetGridColumnTracks             ();
    void                            AddGridRowTrack                  (UI_GRIDTRACK track);
    void                            ClearGridRowTracks               ();
    XVECTOR<UI_GRIDTRACK>&           GetGridRowTracks                ();

    void                            SetGridColumnSpan                (XDWORD span);
    XDWORD                          GetGridColumnSpan                ();
    void                            SetGridRowSpan                   (XDWORD span);
    XDWORD                          GetGridRowSpan                    ();

  private:

    static void                     ClampEdgesToZero               (UI_LAYOUTBOX_EDGES& edges);

    double                           contentleft;
    double                           contenttop;
    double                           contentwidth;
    double                           contentheight;

    UI_LAYOUTBOX_EDGES                padding;
    UI_LAYOUTBOX_EDGES                border;
    UI_LAYOUTBOX_EDGES                margin;

    UI_POSITION                       position;
    UI_LAYOUTBOX_INSETS                insets;

    bool                              flexcontainer;
    UI_FLEX_DIRECTION                 flexdirection;
    UI_JUSTIFY_CONTENT                justifycontent;
    double                            rowgap;
    double                            columngap;

    double                            flexgrow;
    double                            flexshrink;
    UI_LAYOUTBOX_INSET                 flexbasis;

    UI_FLEX_WRAP                       flexwrap;
    UI_ALIGN_CONTENT                   aligncontent;

    UI_ALIGN_ITEMS                     alignitems;
    UI_ALIGN_SELF                      alignself;

    bool                              gridcontainer;
    XVECTOR<UI_GRIDTRACK>             gridcolumntracks;
    XVECTOR<UI_GRIDTRACK>             gridrowtracks;

    XDWORD                            gridcolumnspan;
    XDWORD                            gridrowspan;

    UI_LAYOUTBOX*                     parent;
    XVECTOR<UI_LAYOUTBOX*>            children;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
