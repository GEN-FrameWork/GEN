/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_LayoutEngine.h
*
* @class      UI_LAYOUTENGINE
* @brief      User Interface : builds a UI_LAYOUTBOX tree from a UI_ELEMENT tree.
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
* SCOPE (Phase 3 -- "caja, unidades y posicionamiento", fourth increment, first sub-step: "pequeños primero")
*   The first sub-step of the real layout engine: BuildTree() walks a UI_ELEMENT tree (father/compose_elements,
*   see UI_Element.h) and produces the EQUIVALENT UI_LAYOUTBOX tree, one box per element, populated from that
*   element's ALREADY-RESOLVED legacy geometry -- exactly what UI_SKINCANVAS::CalculePosition() and the padding/
*   border/margin setters (UI_Manager.cpp's builders) already computed and stored on each UI_ELEMENT today.
*
*   Deliberately still NOT a layout algorithm:
*
*     - it reads, it does not compute. Content box geometry comes straight from UI_CSSBox_Get() (already
*       convention-independent -- see UI_CSSAdapter.h); padding/border/margin come straight from
*       UI_ELEMENT::GetPadding()/GetBorderWidth()/GetMargin(). Nothing here resolves a percentage, applies
*       min/max, or interprets box-sizing -- those are exactly the kind of thing the SUBSEQUENT sub-steps
*       (position:relative/absolute + insets, then the real UI_CSSBox_Set() wiring) will start doing, once
*       there is a reason to diverge from what CalculePosition() already produced.
*     - it does not modify "root" or any descendant -- a read-only snapshot, same promise UI_LAYOUTBOX itself
*       already makes about not touching UI_ELEMENT/UI_CSSAdapter.h (see UI_LayoutBox.h's own SCOPE comment).
*     - UI_ELEMENT::GetBorderWidth() can return -1.0 ("use the skin's default width" -- a PAINTING concern, not
*       a geometry one). BuildTree() treats that the same as "no border for layout purposes" (0.0) rather than
*       guessing at a skin default it has no way to know; UI_LAYOUTBOX::SetBorder() would clamp a literal -1.0
*       to 0.0 anyway (see UI_LayoutBox.h), so this is spelled out here rather than left as an accidental
*       side effect of that clamp.
*
*   This exists so the position/insets sub-step (next) has a tree to work FROM instead of reinventing tree
*   traversal at the same time as positioning math.
*
* SCOPE ADDENDUM (fourth increment, second sub-step: "position:relative/absolute + insets")
*   ApplyPositioning() walks an ALREADY-BUILT UI_LAYOUTBOX tree (e.g. BuildTree()'s output, or one a test built
*   by hand) and, for every box whose UI_LAYOUTBOX::GetPosition() is not STATIC, moves its content box per CSS:
*
*     - RELATIVE: offset from the box's OWN current (static-flow) position. Horizontal offset is "left" if
*       specified, else "-right" if specified, else 0 (never both -- CSS says a RELATIVE box with both left and
*       right specified and no explicit width uses left; here both is disambiguated the same way: left wins).
*       Vertical is the analogous top/-bottom/0. Size is never touched, only position.
*     - ABSOLUTE: positioned against the PADDING box (UI_LAYOUTBOX::GetPaddingBox()) of its nearest ancestor
*       whose position is not STATIC, or the tree root's padding box if there is no such ancestor (a documented
*       simplification standing in for CSS's "initial containing block" -- there is no separate viewport object
*       at this layer). If "left" is specified: newleft = cbleft + left. Else if "right" is specified:
*       newleft = cbleft + cbwidth - right - boxwidth. Else: the box's current left is left untouched (CSS's
*       "auto on both" falls back to the box's static position; this engine has no separate static-position pass
*       yet, so "leave it where BuildTree()/the caller already put it" stands in for that fallback). Vertical is
*       the analogous top/bottom/cbheight/boxheight.
*
*   Still deliberately incomplete:
*
*     - does not read UI_COMPUTEDSTYLE's "position"/"top"/"right"/"bottom"/"left" properties -- callers set
*       UI_LAYOUTBOX::SetPosition()/SetInsets() directly (by hand in tests, exactly like every UI_LAYOUTBOX field
*       before it).
*     - position:fixed and position:sticky are not modeled (no UI_POSITION value for either yet).
*     - "auto" margins used for centering an ABSOLUTE box are not special-cased -- out of scope here, exactly
*       like every other unresolved-percentage/min-max concern this file has deferred since sub-step one.
*
* SCOPE ADDENDUM (fourth increment, third and last sub-step: "cableado a UI_CSSBox_Set(), legacy/CSS seleccionables")
*   RunLayout() is the real entry point the roadmap asked for: "UI_CSSBox_Set() pasa de puente aditivo sin
*   consumidor a punto de entrada real del nuevo motor", with both strategies (legacy XML layout / CSS layout)
*   selectable by the caller through UI_LAYOUTSTRATEGY, exactly as asked -- and nothing else:
*
*     - UI_LAYOUTSTRATEGY_LEGACY is a pure no-op: whatever CalculePosition() (or any other existing caller) has
*       already written into "root" and its descendants is left completely untouched. This is what makes the two
*       strategies genuinely "selectable" rather than the CSS path being forced on -- calling RunLayout() with
*       LEGACY anywhere in the existing pipeline changes nothing.
*     - UI_LAYOUTSTRATEGY_CSS runs the pipeline built across this increment's three sub-steps: BuildTree() reads
*       "root"'s current resolved geometry into a UI_LAYOUTBOX tree, ApplyPositioning() repositions the non-STATIC
*       boxes in that tree, and the new WriteBackTree() walks the (element tree, box tree) pair together and
*       drives each element's geometry back through UI_CSSBox_Set() -- the first real consumer that header has
*       ever had. The temporary tree is discarded (GEN_DELETE) once written back; "root" and its descendants keep
*       their own storage (UI_ELEMENT), nothing about UI_ELEMENT's shape changes.
*     - WriteBackTree() pairs each UI_LAYOUTBOX with the UI_ELEMENT it came from by walking BOTH trees in lockstep
*       with the SAME "skip NULL compose_elements entries" rule BuildTree() already uses to build the box tree in
*       the first place -- so the two traversals visit the same elements in the same order without UI_LAYOUTBOX
*       needing to store a back-pointer to UI_ELEMENT (which would break UI_LAYOUTBOX's own "independent of
*       UI_ELEMENT" promise -- see UI_LayoutBox.h's SCOPE comment). WriteBackTree() is exposed as its own public
*       method (not folded into RunLayout()'s body) specifically so it -- and therefore the position-application-
*       and-writeback path -- can be tested directly against a hand-built, already-positioned UI_LAYOUTBOX tree,
*       the same "populate by hand" pattern every UI_LAYOUTBOX-level test in this file already uses.
*
*   Deliberately NOT in scope, and NOT promised by anything above:
*
*     - reading "position"/"top"/"right"/"bottom"/"left" out of a real stylesheet and using that to set
*       UI_LAYOUTBOX::SetPosition()/SetInsets() before BuildTree() hands the tree to ApplyPositioning(). There is
*       currently no per-UI_ELEMENT stored UI_COMPUTEDSTYLE to read at layout time -- GetLayoutElement_Base() in
*       UI_Manager.cpp builds one transiently per XML node, at parse time, and does not keep it attached to the
*       UI_ELEMENT afterwards. Making that happen is a separate structural change (persisting computed style on
*       UI_ELEMENT, or threading it through some other path) that touches code well outside this file, and is
*       left as explicit future work rather than folded into this sub-step under time pressure.
*     - nothing in the existing pipeline (UI_Manager.cpp/UI_SkinCanvas.cpp, all ~30 CalculePosition() call sites)
*       calls RunLayout() yet, with either strategy. This file adds the entry point; deciding WHEN a real
*       widget-building pass should call it with UI_LAYOUTSTRATEGY_CSS is a product decision for a later step,
*       not something to sneak in as a side effect of adding the method.
*
* SCOPE (Phase 4 -- "Flexbox/Grid y migración del ejemplo", first increment: "Flexbox básico", first sub-step:
*   data model + single-line main-axis placement via justify-content)
*   ApplyFlexLayout() walks an ALREADY-BUILT UI_LAYOUTBOX tree (BuildTree()'s output, or one built by hand, same
*   pattern as ApplyPositioning()) and, for every box that IS a flex container (UI_LAYOUTBOX::IsFlexContainer()),
*   arranges its DIRECT children along the main axis per UI_JUSTIFY_CONTENT -- see UI_LayoutBox.h's own SCOPE
*   ADDENDUM for the exact algorithm (packing direction, gap selection, the six justify-content distributions).
*   The recursion continues into every descendant regardless of whether it is itself a flex container, so nested
*   flex containers are each arranged independently (a flex item can itself be a flex container for ITS children).
*
*   Deliberately still NOT the finished Flexbox algorithm -- see UI_LayoutBox.h's own SCOPE ADDENDUM for exactly
*   what is deferred (flex-wrap, flex-grow/shrink/basis, align-items/align-self) and why: each is its own later
*   sub-step of this same increment, "pequeños primero" applied to Flexbox itself.
*
* SCOPE ADDENDUM (Phase 4, first increment, second sub-step: "flex-grow/flex-shrink/flex-basis")
*   ArrangeFlexChildren() now calls a new private step, ResolveFlexItemSizes(), BEFORE packing -- exactly CSS's
*   own order (resolve sizes first, THEN position along the line):
*
*     1. Each child's "hypothetical main size" (base size) is its UI_LAYOUTBOX::GetFlexBasis() when specified,
*        else its CURRENT content main size (the same "whatever's already set" convention the first sub-step
*        used for every child, now demoted to only the "auto" case).
*     2. Sum every child's HYPOTHETICAL outer main size (base size + that child's own padding+border+margin on
*        the main axis) plus the container's main-axis gaps -> total hypothetical main size.
*     3. freespace = container main size - total hypothetical main size.
*        - freespace > 0 and the sum of every child's flex-grow is > 0: each child's final size = base size +
*          freespace * (its own flex-grow / the sum of every flex-grow) -- proportional growth, CSS's algorithm
*          for the single-iteration case (no min/max clamping loop -- see "still not covered" below).
*        - freespace < 0 (overflow) and the sum of every child's (flex-shrink * base size) is > 0: each child's
*          final size = base size - |freespace| * (its own flex-shrink * base size / that sum) -- CSS's actual
*          "scaled shrink factor" weighting (a bigger item shrinks by more, not just by an equal share), clamped
*          to never go below 0 (a box's content size is never negative -- same invariant UI_LAYOUTBOX::SetPadding
*          /SetBorder/SetMargin already enforce for their own edges).
*        - otherwise (freespace == 0, or the relevant sum is 0 -- no item wants to grow/shrink): every child's
*          final size stays its base size; any leftover freespace is exactly what UI_JUSTIFY_CONTENT already
*          distributes (unchanged from the previous sub-step -- this is precisely why growing/shrinking runs
*          FIRST: by the time justify-content sees the free space, growth has already consumed it whenever any
*          child asked to grow).
*     4. Each child's CONTENT main size (width for a ROW container, height for a COLUMN one) is updated to its
*        final size; the cross size and position are untouched here -- packing (unchanged from the previous
*        sub-step) runs next and repositions every child using these new sizes.
*
*   Still deliberately NOT covered:
*
*     - flex-basis interpreted under box-sizing:border-box (it is always a CONTENT-box size here, consistent
*       with every other "size" this file has handled so far).
*     - the CSS spec's iterative "freeze items that hit their min/max, redistribute among the rest" loop -- there
*       is no per-item min/max on a UI_LAYOUTBOX yet (UI_COMPUTEDSTYLE::ClampToMinMax() exists but nothing here
*       calls it), so growth/shrink is a single pass across every item, every time.
*     - flex-wrap and align-items/align-self remain exactly as deferred by the previous sub-step.
*
* SCOPE ADDENDUM (Phase 4, first increment, third sub-step: "flex-wrap + align-content")
*   ArrangeFlexChildren() now branches on UI_LAYOUTBOX::GetFlexWrap(). NOWRAP keeps the EXACT single-line
*   behaviour every earlier sub-step already tests (now factored out into the new private PackFlexLine() helper,
*   unchanged math). WRAP/WRAP_REVERSE run a new four-step pipeline:
*
*     1. Split the container's children into LINES, walking them in compose order and starting a new line
*        whenever the next item's outer main size (post grow/shrink -- ResolveFlexItemSizes() still runs once,
*        for the WHOLE container, before any of this) would overflow the container's main size. An item that
*        overflows the container on its own still gets its own line (CSS never "wraps" a single item away from
*        itself). This is a deliberate simplification: real CSS decides wrapping from each item's HYPOTHETICAL
*        (pre-grow/shrink) size and then runs grow/shrink PER LINE separately -- here grow/shrink runs once for
*        the whole container first, and wrapping groups items by the sizes that step already produced. The
*        difference only matters when growth/shrink and wrapping interact on the SAME container in a way that
*        would have produced different line breaks under the spec-exact order; documented here rather than
*        silently accepted.
*     2. Each line's own cross size is the LARGEST outer cross size among its items (a line in a ROW container
*        is only as tall as its tallest item -- no per-item cross stretch yet, see UI_LayoutBox.h's SCOPE).
*     3. UI_LAYOUTBOX::GetAlignContent() distributes the lines themselves across the container's cross axis,
*        using the exact same six-way distribution math PackFlexLine() already uses for items within one line
*        (flex-start/end, center, space-between/around/evenly) -- just operating on line cross-sizes and the
*        CROSS-axis gap (row-gap for a ROW container, column-gap for a COLUMN one -- the opposite of the
*        main-axis gap) instead of item main-sizes and the main-axis gap.
*     4. Each line is packed along the main axis via the SAME PackFlexLine() NOWRAP uses, then placed at its
*        cross coordinate. WRAP_REVERSE stacks lines in REVERSE cross order (the LAST line, in compose/wrap
*        order, ends up FIRST along the cross axis) -- independent of ROW_REVERSE/COLUMN_REVERSE, which only
*        reverses item order WITHIN a line, not which line comes first.
*
*   Still deliberately NOT covered at THIS sub-step (every item in every line still sat at that line's cross-
*   start -- see the next SCOPE ADDENDUM for align-items/align-self): "align-content: stretch" (not a value
*   UI_ALIGN_CONTENT even offers -- see UI_LayoutBox.h); per-line flex-grow/flex-shrink re-resolution (noted
*   above).
*
* SCOPE ADDENDUM (Phase 4, first increment, fourth and last sub-step: "align-items + align-self")
*   PackFlexLine() (the one helper both the NOWRAP and WRAP/WRAP_REVERSE paths already shared) now takes each
*   line's cross size and the container's UI_ALIGN_ITEMS, and positions -- or, for STRETCH, resizes -- every
*   item within that span instead of always pinning it to the line's cross-start:
*
*     - For NOWRAP, "the line's cross size" is simply the container's full cross size (per CSS: a single-line
*       flex container's one line always spans the container's whole cross size, regardless of its items'
*       actual sizes) -- so align-items now has an effect even without any wrapping at all.
*     - For WRAP/WRAP_REVERSE, it is the per-line cross size Step 2 above already computed (the tallest item's
*       outer cross size) -- align-items positions items WITHIN that, it does not change what that line's size
*       IS (that would be align-content:stretch, still not implemented -- see above).
*     - Each item's own UI_ALIGN_SELF, when not AUTO, overrides the container's align-items for that one item;
*       AUTO (the default) defers to it. This mirrors flex-grow/shrink's "container proposes, item can override"
*       shape from the second sub-step, just for the cross axis instead of the main one.
*     - STRETCH resizes the item's cross-axis CONTENT size to fill the line, shrunk by that item's own leading
*       and trailing margin/border/padding on the cross axis (clamped to >= 0) -- it does NOT check whether the
*       item's cross size was "auto" in CSS terms (UI_LAYOUTBOX has no such flag on content width/height, only
*       on flex-basis); this is a deliberate simplification, documented rather than silently approximated: a
*       STRETCH item's content-set cross size is always overwritten while STRETCH is in effect.
*
* SCOPE ADDENDUM (Phase 4, second increment: "Grid básico", first sub-step: fixed-size tracks (px/%) + gap +
*   row-major auto-placement)
*   ApplyGridLayout() walks a tree exactly like ApplyFlexLayout() does, and wherever UI_LAYOUTBOX::IsGridContainer()
*   is true, ArrangeGridChildren() runs:
*
*     1. Column widths are resolved from UI_LAYOUTBOX::GetGridColumnTracks() (px absolute, or % of the
*        container's content width) via ResolveGridTrackSize(). If grid-template-columns has zero tracks, the
*        container is left untouched -- a caller MUST define at least one column track (no implicit columns are
*        generated the way implicit ROWS are, see below; that direction of "auto-flow: column" is future work).
*     2. Row heights: an EXPLICIT row track (from GetGridRowTracks()) resolves the same way as a column track.
*        Any row beyond however many explicit tracks were given is IMPLICIT: it is sized to the tallest item
*        placed into it (GridItemOuterHeight() -- that item's own margin+border+padding+content height), the
*        same "size a line/track from its content" idea Flexbox's wrapped-line cross size already uses. This is
*        the ONLY content-based sizing this sub-step offers -- an EXPLICIT track (px or %) is never adjusted by
*        its content, matching real CSS (explicit tracks are never auto-sized unless declared "auto", a track
*        keyword this sub-step does not parse/store at all).
*     3. Children are placed row-major, one per cell, in compose/document order: cell index i -> column
*        (i % numcolumns), row (i / numcolumns) -- CSS's "grid-auto-flow: row" (the initial value), the only
*        flow this sub-step implements (no explicit grid-column/grid-row placement, no "column" flow, no spans
*        -- see UI_LayoutBox.h's SCOPE for what's deferred).
*     4. Every item FILLS its cell exactly on both axes, shrunk by its own margin+border+padding (clamped to
*        >= 0) -- i.e. CSS's real default of "stretch" for both justify-items and align-items in Grid (unlike
*        Flexbox, where this sub-step's Flexbox sibling deliberately defaults AWAY from stretch -- see
*        UI_LayoutBox.h's Flexbox SCOPE ADDENDUM for why). Grid has no such earlier-sub-step default to stay
*        compatible with, and implementing justify-items/align-items/justify-self/align-self as a SEPARATE
*        sub-step, only to have their own default ALSO be stretch, would just reimplement this exact behaviour
*        under a different name first -- so this sub-step goes straight to the CSS-correct default, and a later
*        sub-step can add the same override machinery align-self already has for Flexbox, if/when non-stretch
*        alignment is actually needed.
*
* SCOPE ADDENDUM (Phase 4, second increment, second sub-step: the `fr` track unit)
*   Step 1's ResolveGridTrackSize() call (a single track resolved against the container's FULL content size) is
*   no longer enough once a track can be `fr`: an `fr` track's size depends on every OTHER track's size on that
*   axis (how much space is left over), not just the container's size -- so it is resolved through the new
*   ResolveTrackSizesWithFr(), which resolves an ENTIRE axis's tracks together in one call:
*
*     1. Every non-`fr` track (px or %) resolves exactly as before (against the axis's FULL content size, per
*        CSS: percentages are relative to the grid container, never to the leftover "free" space).
*     2. Free space = the axis's full content size, minus its total gap, minus every non-`fr` track's resolved
*        size, minus (for ROWS only) whatever the container's IMPLICIT rows already consume -- so implicit rows
*        are computed FIRST now, before the explicit row template's `fr` tracks, exactly reversing Step 2's
*        original order (columns have no implicit-track counterpart yet, so this reordering is row-only).
*        Clamped to >= 0 -- an overflowing axis simply gives every `fr` track a 0 share, never negative.
*     3. Each `fr` track receives freespace * (its own value / the sum of every `fr` track's value on that
*        axis) -- CSS's real `fr` distribution, and numerically the exact same shape Flexbox's flex-grow
*        distribution already uses (ArrangeFlexChildren()'s "distribute proportionally by weight" step), just
*        applied to tracks instead of items.
*
*   Everything from the first sub-step keeps working unchanged when no track is `fr`: with frtotal == 0 on an
*   axis, ResolveTrackSizesWithFr() reduces to exactly the previous px/%-only resolution.
*
* SCOPE ADDENDUM (Phase 4, second increment, third sub-step: "acotados" minmax() tracks)
*   ResolveTrackSizesWithFr() now treats each track as one of three shapes, uniformly, in the SAME two-pass
*   structure the previous sub-step already used (a "how much does this track reserve no matter what" pass,
*   then a "what's its final size" pass):
*
*     - A simple track (isminmax == false): unchanged from the previous sub-step -- px/% reserve+resolve to
*       their fixed size; `fr` reserves nothing and resolves to its share of the free space.
*     - minmax(min, max) where max is PX or PERCENT: resolves to max(resolvedmax, resolvedmin) -- a single fully
*       determined size (min only matters if it is somehow larger than max, an edge case this engine tolerates
*       rather than rejects). Reserves that same size; contributes NO `fr` weight -- there is nothing left for
*       it to grow into, unlike real CSS's content-aware track sizing, this engine has no "auto"/content-based
*       track size to grow FROM, so a non-`fr` minmax() track is, in effect, just a clamped fixed track.
*     - minmax(min, max) where max is `fr`: reserves only its resolved MIN (its floor -- this is the one place
*       a minmax() track differs from a plain `fr` track, which reserves nothing at all) and contributes its
*       max value as an `fr` weight; Pass 2 gives it minresolved + its `fr` share of the free space -- i.e. it
*       NEVER shrinks below its floor, but grows exactly like a same-weight plain `fr` track would beyond that.
*       This is the bounded approximation UI_LayoutBox.h's SCOPE ADDENDUM refers to: real CSS's track sizing
*       algorithm is iterative and content-aware (a minmax() track can also be limited by ITS OWN content's
*       size, and growth is re-distributed in multiple rounds when clamping changes the available free space);
*       reserving the floor once, up front, and never re-checking it against the grown result, is simpler and
*       matches CSS exactly whenever minresolved is actually reachable as a floor (the common, intended case --
*       "never smaller than X, otherwise grow"), but can diverge from spec-exact CSS in adversarial multi-track
*       configurations where several minmax() floors and several plain `fr` tracks compete in the same axis.
*       Documented here rather than silently approximated.
*
* SCOPE ADDENDUM (Phase 4, second increment, fourth and last sub-step: "Grid básico" -- grid-column/grid-row
*   spans)
*   ArrangeGridChildren()'s placement step changes from the previous "cell index i -> column (i % numcolumns),
*   row (i / numcolumns)" one-liner into a real auto-placement scan, because a spanning item can make later
*   items skip cells (or, once a row-spanning item is involved, skip cells in a row that hasn't been reached by
*   the simple index arithmetic yet):
*
*     1. Every item still visits cells in the exact same order as before (row-major, one cursor sweeping left
*        to right then down) -- grid-auto-flow: row remains the only flow, unchanged from the first sub-step.
*        What is NEW is that the cursor now SKIPS any cell already claimed by an earlier item's span, and an
*        item whose own span would overflow the remaining columns on the current row wraps to the next row
*        first, exactly like a word that doesn't fit wrapping to a new line.
*     2. "occupied" (EnsureGridRows()/GridCellsOccupied()/MarkGridCellsOccupied()) is a flat row-major bool
*        grid that grows one row at a time, on demand, as the cursor or a row-spanning item's reach goes past
*        what has been seen before -- there is no upfront "how many rows will this take" calculation the way
*        the previous sub-steps' simple ceil(childcount / numcolumns) was; the final row count is simply
*        whatever the placement loop actually touched.
*     3. This is CSS's "sparse" packing algorithm (the initial value of grid-auto-flow's "dense" keyword,
*        which this engine does not implement): the placement cursor only ever moves FORWARD. A later, smaller
*        item is never backfilled into an earlier gap a bigger item's span left behind -- exactly like this
*        engine's Flexbox line-wrapping already never reshuffles items across a wrap point.
*     4. A column span wider than the container's own column count is clamped down to the full column count
*        (documented, not rejected) -- there is nowhere else for it to go without the grid growing extra
*        columns, itself a form of the "auto-flow: column"/implicit-column-tracks behaviour this sub-step does
*        not add (see UI_LayoutBox.h's SCOPE for the growable IMPLICIT ROWS this engine already had, by
*        contrast). A row span is never clamped -- EnsureGridRows() simply grows to fit it, the same as an
*        ordinary item wrapping onto a brand new row already did.
*     5. Implicit row sizing (Step 2 from the first sub-step) now only counts a row's SINGLE-row-span items
*        toward that row's "tallest item" height -- a spanning item's own height is not distributed across the
*        multiple rows it covers (real CSS does distribute it, proportionally, among the rows the item spans,
*        as one more input to those rows' track-sizing; this engine does not). A spanning item's cell simply
*        gets however tall its spanned rows already turned out to be (from other, single-row items, or explicit
*        tracks) -- if that is shorter than the item's own content, the item is stretched into whatever space
*        exists exactly like any other item (see the first sub-step's SCOPE), it does not grow ITS rows to fit.
*
* SCOPE ADDENDUM (Phase 4 -- "migración del ejemplo", first sub-step: "conectar el motor Flexbox al pipeline
*   real")
*   Everything above (Flexbox básico, Grid básico) ran only from GoogleTest, on UI_LAYOUTBOX trees built either
*   by hand or via BuildTree() from a stand-alone UI_ELEMENT tree -- nothing in the real widget-building pipeline
*   (UI_MANAGER::CreateLayouts()/GetLayoutElement_Base(), the ~30 CalculePosition() call sites) ever called
*   RunLayout(UI_LAYOUTSTRATEGY_CSS), so none of it had any effect on an actual loaded layout. This sub-step
*   wires the FLEXBOX half of the engine into that real pipeline -- Grid is deliberately deferred to its own
*   later sub-step (grid-template-columns/rows need real CSS track-list grammar -- fr, minmax(), several tokens
*   per property -- substantial parsing work in its own right, not a small addition to this one):
*
*     1. UI_ELEMENT (UI_Element.h/.cpp) gains the Flexbox container/item properties as plain storage, reusing
*        UI_LAYOUTBOX's OWN enums/structs (UI_FLEX_DIRECTION, UI_JUSTIFY_CONTENT, UI_FLEX_WRAP, UI_ALIGN_CONTENT,
*        UI_ALIGN_ITEMS, UI_ALIGN_SELF, UI_LAYOUTBOX_INSET) instead of a parallel duplicate set, with the SAME
*        defaults -- an element that never sets any of them behaves exactly like a freshly-built UI_LAYOUTBOX.
*     2. BuildTree() copies those properties from "root" onto the UI_LAYOUTBOX it mirrors "root" into -- both the
*        container side (how "root" arranges ITS OWN children) and the item side (how "root" is itself sized/
*        aligned by ITS OWN father, resolved one level up when BuildTree() is later called on that father).
*     3. RunLayout(UI_LAYOUTSTRATEGY_CSS) now calls ApplyFlexLayout(tree) BEFORE ApplyPositioning(tree) -- CSS's
*        own order: normal-flow placement (flex) settles a box's position first, RELATIVE/ABSOLUTE positioning
*        applies its own offset on top of that. ApplyFlexLayout() already recurses through the whole tree by
*        itself, so nested flex containers keep working with this single extra call.
*     4. UI_MANAGER::GetLayoutElement_Base() (UI_Manager.cpp) parses the corresponding CSS properties --
*        "display: flex" (any other value, or absent, leaves IsFlexContainer() at its default "false"),
*        "flex-direction", "justify-content", "gap"/"row-gap"/"column-gap", "flex-wrap", "align-content",
*        "align-items" on a container; "flex-grow", "flex-shrink", "flex-basis" ("auto" or a number),
*        "align-self" on an item -- into the new UI_ELEMENT setters, the exact same style.Get()+string-compare
*        pattern the adjacent "direction"/"role" parsing already uses.
*     5. UI_MANAGER::CreateLayouts() calls RunLayout(element, UI_LAYOUTSTRATEGY_CSS) once for every TOP-LEVEL
*        element of a freshly-built <layout>, right after that element's own subtree (every nested <element>
*        child, built recursively by CreatePartialLayout() before this point) is fully resolved by the existing
*        legacy XML pipeline. Deliberately unconditional (not gated behind an opt-in flag on the <layout> or the
*        element): BuildTree() reads a box's CURRENT already-resolved geometry, and ApplyFlexLayout()/
*        ApplyPositioning() are no-ops wherever IsFlexContainer() is false / position is STATIC -- which is
*        EVERY element of EVERY layout authored before this wiring existed, since there is today no XML/CSS
*        syntax that could have set those properties before this sub-step added the parsing for it. So this is a
*        behaviour-preserving, always-on second pass: existing layouts round-trip through
*        BuildTree()->ApplyFlexLayout()->ApplyPositioning()->WriteBackTree() unchanged, and only a NEW layout
*        that actually opts a container into "display: flex" sees any different geometry.
*
*   Still deliberately NOT covered:
*
*     - Grid (see above -- its own later sub-step).
*     - dashboard.xml/dashboard.css do not use any of this yet: today's 109/111 root-sibling elements have no
*       real XML nesting for a flex container to arrange in the first place. Restructuring the example into real
*       nested containers is the NEXT sub-step of this same "migración del ejemplo" increment.
*     - a live re-layout when a Flexbox property (display/flex-direction/justify-content/gap/etc.) itself
*       changes after load (no ReapplyStyleVisual() hook for these properties, same limitation UI_Element.h's
*       own SCOPE comment notes for "direction"/margin/padding). NARROWER than it used to be, though: a flex
*       CHILD's own resolved CONTENT SIZE changing after load (a live text value ticking, e.g. dashboard.xml's
*       "#[FOOTER_SO]"/"#[FOOTER_UPTIME]") IS now covered -- see UI_MANAGER::ChangeTextElementValue()'s own
*       comment, added in this same increment's footer live-update follow-up: it re-runs RunLayout() on the
*       flex father whenever the changed element's own father IsFlexContainer(), which reflows every sibling,
*       not just the one that changed. What is still NOT covered is authored Flexbox PROPERTIES (not sizes)
*       changing at runtime, e.g. toggling "display: flex" itself, or align-items, off an XML/CSS edit made
*       after load -- there is simply no code path that edits those at runtime today to regress.
*
* @author     Abraham J. Velez / EndoraSoft
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_LayoutBox.h"
#include "UI_Element.h"


/**
* @enum   UI_LAYOUTSTRATEGY
* @brief  Which layout strategy RunLayout() should drive "root"'s geometry with.
*/
enum UI_LAYOUTSTRATEGY
{
  UI_LAYOUTSTRATEGY_LEGACY = 0,      // no-op: whatever the existing XML layout (CalculePosition et al.) produced stands
  UI_LAYOUTSTRATEGY_CSS               // BuildTree() + ApplyFlexLayout() + ApplyPositioning() + WriteBackTree(), via UI_CSSBox_Set()
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


/**
* @brief  UI_LAYOUTENGINE has no instance state today: every method here is a static, pure-logic helper, the same
*         shape UI_PROPERTYREGISTRY already uses (see UI_PropertyRegistry.h) for the same reason -- nothing here
*         needs to persist between calls yet.
*/
class UI_LAYOUTENGINE
{
  public:

    static UI_LAYOUTBOX*           BuildTree                    (UI_ELEMENT* root);

    static void                    ApplyPositioning              (UI_LAYOUTBOX* root);

    static void                    WriteBackTree                 (UI_ELEMENT* root, UI_LAYOUTBOX* tree);

    static void                    RunLayout                     (UI_ELEMENT* root, UI_LAYOUTSTRATEGY strategy);

    static void                    ApplyFlexLayout                (UI_LAYOUTBOX* root);

    static void                    ApplyGridLayout                 (UI_LAYOUTBOX* root);

  private:

    static void                    ApplyPositioningRecursive     (UI_LAYOUTBOX* box, UI_LAYOUTBOX* rootbox, UI_LAYOUTBOX* nearestpositionedancestor);
    static void                    ApplyRelativeOffset           (UI_LAYOUTBOX* box);
    static void                    ApplyAbsolutePosition         (UI_LAYOUTBOX* box, UI_LAYOUTBOX* containingblock);
    static void                    WriteBackRecursive             (UI_ELEMENT* element, UI_LAYOUTBOX* box);
    static bool                    SubtreeUsesCSSFlowLayout       (UI_ELEMENT* element);
    static void                    ApplyFlowLayoutRecursive       (UI_LAYOUTBOX* box);
    static void                    ApplyFlexLayoutRecursive       (UI_LAYOUTBOX* box);
    static void                    ArrangeFlexChildren             (UI_LAYOUTBOX* container);
    static void                    ResolveFlexItemSizes            (UI_LAYOUTBOX* container);
    static void                    PackFlexLine                    (XVECTOR<UI_LAYOUTBOX*>& children, XDWORD start, XDWORD count, bool mainishorizontal, bool reversed, UI_JUSTIFY_CONTENT justifycontent, double maingap, double linemainsize, double linemainstart, double linecrossstart, double linecrosssize, UI_ALIGN_ITEMS containeralignitems);
    static double                  FlexItemOuterMainSize            (UI_LAYOUTBOX* child, bool mainishorizontal);
    static double                  FlexItemOuterCrossSize           (UI_LAYOUTBOX* child, bool mainishorizontal);
    static void                    ApplyGridLayoutRecursive         (UI_LAYOUTBOX* box);
    static void                    ArrangeGridChildren              (UI_LAYOUTBOX* container);
    static double                  ResolveGridTrackSize             (UI_GRIDTRACK track, double containersize);
    static void                    ResolveTrackSizesWithFr           (XVECTOR<UI_GRIDTRACK>& tracks, XDWORD count, double fullcontainersize, double reservedspace, XVECTOR<double>& outsizes);
    static double                  GridItemOuterHeight              (UI_LAYOUTBOX* child);
    static void                    EnsureGridRows                  (XVECTOR<bool>& occupied, XDWORD& occupiedrows, XDWORD targetrows, XDWORD numcolumns);
    static bool                    GridCellsOccupied                (XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan);
    static void                    MarkGridCellsOccupied            (XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan);
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
