/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_SkinCanvas.cpp
* 
* @class      UI_SKINCANVAS
* @brief      User Interface Skin Canvas class
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

#include "UI_SkinCanvas.h"

#include "XTrace.h"
#include "XTimer.h"
#include "XSleep.h"
#include "XTranslation.h"
#include "XFile.h"

#include "GRP2DCanvas.h"
#include "GRP2DColor.h"
#include "GRPViewPort.h"
#include "GRPScreen.h"
#include "GRPBitmap.h"
#include "GRPBitmapFile.h"
#include "GRPFactory.h"

// Needed only by UI_SkinCanvas_DrawSoftShadow's agg::stack_blur_rgba32 call further down: this file otherwise
// stays behind the GRP2DCANVAS/GRPBITMAP abstraction and never touches AGG types directly. agg_rendering_buffer.h
// and agg_pixfmt_rgba.h are already pulled in transitively (GRPBitmap.h, GRP2DCanvasAGG.h) on every platform
// that ships the AGG canvas backend; agg_blur.h is the one header this file is the first to require, so if a
// given platform's vendored AGG drop is a trimmed subset without it, this is the file that will fail to build --
// add agg_blur.h to that platform's AGG sources (it is dependency-free beyond agg_array.h) to fix it.
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgba.h"
#include "agg_blur.h"

#include "UI_Animation.h"
#include "UI_Element.h"
#include "UI_Property_Scrolleable.h"
#include "UI_Element_Scroll.h"
#include "UI_Element_Text.h"
#include "UI_Element_TextBox.h"
#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Element_Tree.h"
#include "UI_Element_Image.h"
#include "UI_Element_Animation.h"
#include "UI_Element_Option.h"
#include "UI_Element_MultiOption.h"
#include "UI_Element_Button.h"
#include "UI_Element_CheckBox.h"
#include "UI_Element_EditText.h"
#include "UI_Element_Form.h"
#include "UI_Element_Menu.h"
#include "UI_Element_ListBox.h"
#include "UI_Element_ProgressBar.h"
#include "UI_Element_ProgressRadial.h"
#include "UI_Element_ProgressImage.h"
#include "UI_Element_StatisticsChart.h"
#include "UI_Layout.h"
#include "UI_Manager.h"

#include "APPFlowBase.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define UI_SKINCANVAS_SCROLLBAR_WIDTH       UI_PROPERTY_SCROLLEABLE_BARWIDTH    // overlay scrollbar thickness (px)
#define UI_SKINCANVAS_SCROLLBAR_MINTHUMB    16       // minimum thumb length (px)

#define UI_SKINCANVAS_SCROLLBAR_TRACK_R     0        // track color (faint)
#define UI_SKINCANVAS_SCROLLBAR_TRACK_G     0
#define UI_SKINCANVAS_SCROLLBAR_TRACK_B     0
#define UI_SKINCANVAS_SCROLLBAR_TRACK_A     30

#define UI_SKINCANVAS_SCROLLBAR_THUMB_R     80       // thumb color (semi-transparent)
#define UI_SKINCANVAS_SCROLLBAR_THUMB_G     80
#define UI_SKINCANVAS_SCROLLBAR_THUMB_B     80
#define UI_SKINCANVAS_SCROLLBAR_THUMB_A     160



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ProgressRadial_AppendArc(GRP2DPATH& path, double cx, double cy, double r, double startdeg, double spandeg, bool& firstpoint)
* @brief      Append a circular arc to a path as a short-segment polyline (~2 deg per step). Uses only MoveTo/LineTo, which are fully exercised by the canvas stroke pipeline; this deliberately avoids GRP2DPATH::ArcTo (the SVG elliptical-arc command), whose AGG arc_to conversion is not used anywhere else and produces no geometry here. Angles are degrees; a positive span advances clockwise in this y-down canvas.
* @note       INTERNAL / FILE LOCAL
* @ingroup    USERINTERFACE
* 
* @param[in]  path : destination path.
* @param[in]  cx : Cx value.
* @param[in]  cy : Cy value.
* @param[in]  r : ring radius (to the stroke centerline).
* @param[in]  startdeg : start angle in degrees.
* @param[in]  spandeg : signed span in degrees.
* @param[in]  firstpoint : true on the first append (emits a MoveTo); set to false afterwards.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_ProgressRadial_AppendArc(GRP2DPATH& path, double cx, double cy, double r, double startdeg, double spandeg, bool& firstpoint)
{
  if(r <= 0.0) return;

  int nseg = (int)ceil(fabs(spandeg) / 2.0);                          // ~2 deg per segment
  if(nseg < 1) nseg = 1;

  double step = spandeg / nseg;

  for(int c=0; c<=nseg; c++)
    {
      double deg = startdeg + (step * c);
      double rad = deg * (PI / 180.0);

      double px  = cx + (r * cos(rad));
      double py  = cy + (r * sin(rad));

      if(firstpoint)
        {
          path.MoveTo(px, py);
          firstpoint = false;
        }
       else
        {
          path.LineTo(px, py);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DrawScrollPill(GRP2DCANVAS* canvas, double a, double b, double c, double d, double formradius)
* @brief      Draw Scroll Pill
* @ingroup    USERINTERFACE
* 
* @param[in]  canvas : Canvas object to use.
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  formradius : Formradius value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void DrawScrollPill(GRP2DCANVAS* canvas, double a, double b, double c, double d, double formradius)
{
  // Normalize coords (the edge/clip convention can give either order). The bar follows the form: it is rounded only
  // when the form has rounded corners, with the corner radius clamped to half the shorter side so a wide-thin or
  // tall-thin bar stays a valid pill (agg::rounded_rect can misround a wide reversed-Y rect).
  double x1   = __MIN(a, c);
  double x2   = __MAX(a, c);
  double y1   = __MIN(b, d);
  double y2   = __MAX(b, d);
  double half = __MIN((x2 - x1), (y2 - y1)) / 2.0f;
  double r    = (formradius > 0.0f) ? __MIN(formradius, half) : 0.0f;

  canvas->RoundRect(x1, y1, x2, y2, r, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ProgressBar_DrawRect(GRP2DCANVAS* canvas, double x1, double y1, double x2, double y2, double radius)
* @brief      Draw a filled rect, rounded when radius > 0. The radius is clamped to half the smaller side so a capsule (roundcap) stays valid even when the progress fill is narrower than the bar thickness (agg::rounded_rect does not self-normalize the radius).
* @note       INTERNAL / FILE LOCAL
* @ingroup    USERINTERFACE
* 
* @param[in]  canvas : Canvas object to use.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  radius : Radius value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_ProgressBar_DrawRect(GRP2DCANVAS* canvas, double x1, double y1, double x2, double y2, double radius)
{
  if(!canvas) return;

  double w = (x2 > x1) ? (x2 - x1) : (x1 - x2);
  double h = (y1 > y2) ? (y1 - y2) : (y2 - y1);

  if((w <= 0.0) || (h <= 0.0)) return;                                                // nothing to draw (e.g. level 0)

  if(radius <= 0.0) { canvas->Rectangle(x1, y1, x2, y2, true); return; }              // square corners

  // If the rect is shorter than 2*radius on the progress axis, a smaller clamped radius would make the cap squarer
  // than the frame's cap and poke outside it. Instead keep the frame radius, extend the rect to a valid size on that
  // axis, and clip to the real band: the cap then follows the SAME circle as the frame and nests exactly inside it.
  double dx2     = x2;
  double dy2     = y2;
  bool   clipped = false;

  if(w < (2.0 * radius)) { dx2 = (x2 >= x1) ? (x1 + (2.0 * radius)) : (x1 - (2.0 * radius)); clipped = true; }
  if(h < (2.0 * radius)) { dy2 = (y2 <= y1) ? (y1 - (2.0 * radius)) : (y1 + (2.0 * radius)); clipped = true; }

  if(!clipped) { canvas->RoundRect(x1, y1, x2, y2, radius, true); return; }

  double ox1 = 0.0, oy1 = 0.0, ox2 = 0.0, oy2 = 0.0;
  canvas->GetClipBox(ox1, oy1, ox2, oy2);

  double cminx = __MIN(ox1, ox2), cmaxx = __MAX(ox1, ox2);
  double cminy = __MIN(oy1, oy2), cmaxy = __MAX(oy1, oy2);
  double bminx = __MIN(x1, x2),   bmaxx = __MAX(x1, x2);
  double bminy = __MIN(y1, y2),   bmaxy = __MAX(y1, y2);

  double iminx = __MAX(cminx, bminx), imaxx = __MIN(cmaxx, bmaxx);
  double iminy = __MAX(cminy, bminy), imaxy = __MIN(cmaxy, bmaxy);

  if((imaxx > iminx) && (imaxy > iminy))
    {
      canvas->SetClipBox(iminx, imaxy, imaxx, iminy);                                 // (left, bottom, right, top)
      canvas->RoundRect(x1, y1, dx2, dy2, radius, true);
      canvas->SetClipBox(ox1, oy1, ox2, oy2);                                         // restore
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_AppendRoundRectPath(GRP2DPATH& path, double minx, double miny, double maxx, double maxy, double r)
* @brief      Builds a (optionally rounded) rectangle outline into a path with MoveTo/LineTo so it can be gradient-filled.
* @note       INTERNAL / FILE LOCAL. Inputs are already normalized (minx<=maxx, miny<=maxy) and r clamped by the caller.
* @ingroup    USERINTERFACE
* 
* @param[in]  path : Path to use.
* @param[in]  minx : Minx value.
* @param[in]  miny : Miny value.
* @param[in]  maxx : Maxx value.
* @param[in]  maxy : Maxy value.
* @param[in]  r : R value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_AppendRoundRectPath(GRP2DPATH& path, double minx, double miny, double maxx, double maxy, double r)
{
  if(r <= 0.0)
    {
      path.MoveTo(minx, miny);  path.LineTo(maxx, miny);  path.LineTo(maxx, maxy);  path.LineTo(minx, maxy);  path.Close();
      return;
    }

  double cx[4] = { minx + r, maxx - r, maxx - r, minx + r };                          // corner centers: TL, TR, BR, BL
  double cy[4] = { miny + r, miny + r, maxy - r, maxy - r };
  double a0[4] = { 180.0,    270.0,      0.0,     90.0    };                          // start angle of each quarter arc (deg)

  const int STEPS = 6;
  bool      first = true;

  for(int c = 0; c < 4; c++)
    {
      for(int s = 0; s <= STEPS; s++)
        {
          double ang = (a0[c] + (90.0 * (double)s / (double)STEPS)) * (PI / 180.0);
          double px  = cx[c] + (r * cos(ang));
          double py  = cy[c] + (r * sin(ang));

          if(first) { path.MoveTo(px, py); first = false; }
          else        path.LineTo(px, py);
        }
    }
  path.Close();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::AppendRoundRectPathPerCorner(GRP2DPATH& path, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL)
* @brief      Build a rounded-rectangle outline with a possibly-different radius per corner. Corners with radius
*             <= 0 are drawn square. Each non-zero radius is clamped to half the shorter side of the rect so
*             two adjacent large radii never overlap into an invalid shape. Uses only MoveTo/LineTo, matching the
*             stroke-friendly polyline convention of UI_SkinCanvas_AppendRoundRectPath so both fill and stroke
*             paint through the same AGG code path.
* @note       Shared box-model helper (built against base UI_ELEMENT accessors only, no GRP2DPATH-specific state
*             on UI_SKINCANVAS itself): any subclass' Draw_X can call it. Inputs are already normalized
*             (minx <= maxx, miny <= maxy).
* @ingroup    USERINTERFACE
*
* @param[in]  path : Path to append into.
* @param[in]  minx : Left edge.
* @param[in]  miny : Top edge.
* @param[in]  maxx : Right edge.
* @param[in]  maxy : Bottom edge.
* @param[in]  rTL : Top-left corner radius.
* @param[in]  rTR : Top-right corner radius.
* @param[in]  rBR : Bottom-right corner radius.
* @param[in]  rBL : Bottom-left corner radius.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::AppendRoundRectPathPerCorner(GRP2DPATH& path, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL)
{
  double w    = maxx - minx;
  double h    = maxy - miny;
  double half = ((w < h) ? w : h) / 2.0;

  // Clamp each corner independently to keep the outline valid on narrow rectangles.
  if(rTL < 0.0) rTL = 0.0;   if(rTL > half) rTL = half;
  if(rTR < 0.0) rTR = 0.0;   if(rTR > half) rTR = half;
  if(rBR < 0.0) rBR = 0.0;   if(rBR > half) rBR = half;
  if(rBL < 0.0) rBL = 0.0;   if(rBL > half) rBL = half;

  // If nothing is rounded at all, emit the plain rectangle (matches the shortcut in AppendRoundRectPath).
  if(rTL <= 0.0 && rTR <= 0.0 && rBR <= 0.0 && rBL <= 0.0)
    {
      path.MoveTo(minx, miny);  path.LineTo(maxx, miny);  path.LineTo(maxx, maxy);  path.LineTo(minx, maxy);  path.Close();
      return;
    }

  const int STEPS = 16;
  bool      first = true;

  // Traversal order (clockwise, starting at the top edge between the two top corners):
  //   TL corner arc -> top edge -> TR arc -> right edge -> BR arc -> bottom edge -> BL arc -> left edge -> Close.
  double cx[4] = { minx + rTL, maxx - rTR, maxx - rBR, minx + rBL };                  // corner centers: TL, TR, BR, BL
  double cy[4] = { miny + rTL, miny + rTR, maxy - rBR, maxy - rBL };
  double r [4] = { rTL,        rTR,        rBR,        rBL        };
  double a0[4] = { 180.0,      270.0,        0.0,       90.0      };                  // start angle of each quarter arc

  for(int c = 0; c < 4; c++)
    {
      if(r[c] <= 0.0)
        {
          // Square corner: emit the sharp corner vertex itself.
          double vx = (c == 0) ? minx : (c == 1) ? maxx : (c == 2) ? maxx : minx;
          double vy = (c == 0) ? miny : (c == 1) ? miny : (c == 2) ? maxy : maxy;
          if(first) { path.MoveTo(vx, vy); first = false; }
          else        path.LineTo(vx, vy);
          continue;
        }

      for(int s = 0; s <= STEPS; s++)
        {
          double ang = (a0[c] + (90.0 * (double)s / (double)STEPS)) * (PI / 180.0);
          double px  = cx[c] + (r[c] * cos(ang));
          double py  = cy[c] + (r[c] * sin(ang));

          if(first) { path.MoveTo(px, py); first = false; }
          else        path.LineTo(px, py);
        }
    }

  path.Close();
}


// Padding, in pixels, added around a shadow silhouette so the blur fade has room to reach zero before the
// off-screen bitmap ends. SINGLE SOURCE OF TRUTH: both the renderer (UI_SkinCanvas_DrawSoftShadow, which sizes
// and positions the bitmap) and the invalidation logic (UI_SKINCANVAS::PreDrawFunction, which expands the
// element's rebuild-area) must agree on this number. If they drift apart, the outer ring of the blurred shadow
// falls outside the saved/restored background rectangle and leaves ghost pixels that accumulate -- getting
// progressively darker -- on every repaint.
// Still valid with agg::stack_blur_rgba32 (see UI_SkinCanvas_DrawSoftShadow): its single-pass triangular kernel
// has non-zero support out to exactly the blur radius on each side, i.e. NARROWER than the old hand-rolled
// 3-pass box blur this padding was originally sized for (whose support reached roughly 3x the radius). 2x
// stays comfortably generous for the new algorithm -- no need to shrink it, and shrinking would risk exactly
// the ghost-pixel bug described above for no benefit.
#define UI_SKINCANVAS_SHADOW_BLURPADDING(blur)      ((blur) * 2)


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SkinCanvas_RoundedRectInside(int px, int py, int w, int h, double rTL, double rTR, double rBR, double rBL)
* @brief      Point-in-rounded-rect test for the four per-corner radius variant.
* @note       INTERNAL / FILE LOCAL. Rectangle spans [0, w) x [0, h). Corners with radius 0 draw square.
* @ingroup    USERINTERFACE
*
* @param[in]  px : Pixel X (relative to rectangle top-left).
* @param[in]  py : Pixel Y (relative to rectangle top-left).
* @param[in]  w : Rectangle width.
* @param[in]  h : Rectangle height.
* @param[in]  rTL : Top-left radius.
* @param[in]  rTR : Top-right radius.
* @param[in]  rBR : Bottom-right radius.
* @param[in]  rBL : Bottom-left radius.
*
* @return     bool : true iff the pixel is inside the shape.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool UI_SkinCanvas_RoundedRectInside(int px, int py, int w, int h, double rTL, double rTR, double rBR, double rBL)
{
  if(px < 0 || px >= w || py < 0 || py >= h) return false;

  // Which corner region does (px, py) fall into? A pixel not inside any corner square is always inside the shape.
  bool in_TL = (px <  (int)rTL) && (py <  (int)rTL);
  bool in_TR = (px >= (w - (int)rTR)) && (py <  (int)rTR);
  bool in_BR = (px >= (w - (int)rBR)) && (py >= (h - (int)rBR));
  bool in_BL = (px <  (int)rBL) && (py >= (h - (int)rBL));

  if(!in_TL && !in_TR && !in_BR && !in_BL) return true;

  double cx = 0.0, cy = 0.0, r = 0.0;

  if     (in_TL) { cx = rTL;      cy = rTL;      r = rTL; }
  else if(in_TR) { cx = w - rTR;  cy = rTR;      r = rTR; }
  else if(in_BR) { cx = w - rBR;  cy = h - rBR;  r = rBR; }
  else           { cx = rBL;      cy = h - rBL;  r = rBL; }

  if(r <= 0.0) return true;    // corner declared square: inside

  double dx = ((double)px + 0.5) - cx;
  double dy = ((double)py + 0.5) - cy;

  return (dx * dx + dy * dy) <= (r * r);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SkinCanvas_PunchRoundRectCornerTips
* @brief      Restore true backdrop into the four AABB corner cutouts of a rounded rect.
* @note       INTERNAL. After RoundRect/Path fill, AGG AA (and occasionally a sharp prior fill) can leave dark
*             "picos" in the square tips outside the arc. Copy those pixels back from FormBackdrop (or from a
*             1px sample just outside the AABB when no backdrop exists). Same family as the soft-shadow cutout
*             clear in UI_SkinCanvas_BuildSoftShadowBitmap.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_PunchRoundRectCornerTips(GRP2DCANVAS* canvas, GRP2DREBUILDAREA* backdrop, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL, const GRP2DCOLOR_RGBA8* parent_override)
{
  if(!canvas) return;

  // Inclusive pixel span. Plain (int)(max-min) drops the bottom/right edge row when AGG's RoundRect
  // stroke sits on that last pixel — tip punch then treated py==h / px==w as "outside" and painted
  // parent ink over the BL/BR (and sometimes TR/BR) border arcs. Symptom: overflow:hidden foot forms
  // lose their bottom rounded corners while the top arcs still look fine.
  int x0 = (int)floor(minx);
  int y0 = (int)floor(miny);
  int w  = (int)ceil(maxx - minx);
  int h  = (int)ceil(maxy - miny);
  if(w < 1) w = 1;
  if(h < 1) h = 1;
  // One extra pixel so the geometric edge column/row (stroke-centred on maxx/maxy) stays inside the test AABB.
  int pw = w + 1;
  int ph = h + 1;

  if(w <= 2 || h <= 2) return;
  if(rTL <= 0.0 && rTR <= 0.0 && rBR <= 0.0 && rBL <= 0.0) return;

  GRPBITMAP* bmp = (backdrop) ? backdrop->GetBitmap() : NULL;

  // Parent ink: override (must be non-black) > FormBackdrop mid-edge > sample just OUTSIDE the AABB
  // (true screen backdrop for top-level panels) > father's CSS bg (passed as override).
  // Never punch with 0,0,0 — that paints black picos worse than AGG tips.
  GRP2DCOLOR_RGBA8 parent(0, 0, 0, 255);
  bool have_parent = false;
  if(parent_override && parent_override->a > 0 &&
     (parent_override->r + parent_override->g + parent_override->b) > 8)
    {
      parent = *parent_override;
      have_parent = true;
    }
  if(!have_parent && bmp && backdrop && bmp->GetWidth() > 2 && bmp->GetHeight() > 2)
    {
      int bdx = (int)backdrop->GetXPos();
      int bdy = (int)backdrop->GetYPos();
      int bx = (x0 - bdx) + (w / 2);
      int by = (y0 - bdy) + 1;
      if(bx < 0) bx = 1;
      if(by < 0) by = 1;
      if(bx >= (int)bmp->GetWidth())  bx = (int)bmp->GetWidth()  / 2;
      if(by >= (int)bmp->GetHeight()) by = (int)bmp->GetHeight() / 2;
      GRP2DCOLOR* c = bmp->GetPixel(bx, by);
      if(c)
        {
          GRP2DCOLOR_RGBA8 s = *(GRP2DCOLOR_RGBA8*)c;
          if(s.a > 0 && (s.r + s.g + s.b) > 8) { parent = s; have_parent = true; }
        }
    }
  // Top-level forms often have no father and a FormBackdrop mid-edge that already baked panel ink.
  // Sample the live canvas just outside each AABB corner and pick the first non-black neighbour.
  if(!have_parent)
    {
      const int ox[4] = { x0 - 2, x0 + w + 1, x0 + w + 1, x0 - 2 };
      const int oy[4] = { y0 - 2, y0 - 2,     y0 + h + 1, y0 + h + 1 };
      for(int i = 0; i < 4 && !have_parent; i++)
        {
          GRP2DCOLOR* c = canvas->GetPixel((double)ox[i], (double)oy[i]);
          if(!c) continue;
          GRP2DCOLOR_RGBA8 s = *(GRP2DCOLOR_RGBA8*)c;
          if(s.a > 0 && (s.r + s.g + s.b) > 8) { parent = s; have_parent = true; }
        }
    }
  if(!have_parent) return;

  // Inflate tip boxes by +2 so outline AA spikes past the geometric radius are still cleared.
  int rTLi = (int)(rTL + 2.0); if(rTLi > pw) rTLi = pw; if(rTLi > ph) rTLi = ph;
  int rTRi = (int)(rTR + 2.0); if(rTRi > pw) rTRi = pw; if(rTRi > ph) rTRi = ph;
  int rBRi = (int)(rBR + 2.0); if(rBRi > pw) rBRi = pw; if(rBRi > ph) rBRi = ph;
  int rBLi = (int)(rBL + 2.0); if(rBLi > pw) rBLi = pw; if(rBLi > ph) rBLi = ph;

  // Keep the border stroke ring: AGG centres the stroke on the path, so ~0.5..1.5px of border AA
  // sits outside the fill radius. Punching with the raw radius ate those pixels at BL/BR.
  const double keep = 1.25;

  double ox1 = 0.0, oy1 = 0.0, ox2 = 0.0, oy2 = 0.0;
  canvas->GetClipBox(ox1, oy1, ox2, oy2);
  {
    double cminx = __MIN(ox1, ox2) - 2.0, cmaxx = __MAX(ox1, ox2) + 2.0;
    double cminy = __MIN(oy1, oy2) - 2.0, cmaxy = __MAX(oy1, oy2) + 2.0;
    canvas->SetClipBox(cminx, cmaxy, cmaxx, cminy);
  }

  for(int py = 0; py <= ph + 1; py++)
    {
      for(int px = 0; px <= pw + 1; px++)
        {
          bool in_TL = (px <= rTLi) && (py <= rTLi);
          bool in_TR = (px >= (pw - rTRi)) && (py <= rTRi);
          bool in_BR = (px >= (pw - rBRi)) && (py >= (ph - rBRi));
          bool in_BL = (px <= rBLi) && (py >= (ph - rBLi));
          if(!in_TL && !in_TR && !in_BR && !in_BL) continue;

          // Clear only deep AABB tips outside the stroke-inflated arc (preserve rounded border).
          bool inside_shape = (px >= 0 && py >= 0 && px < pw && py < ph) &&
                              UI_SkinCanvas_RoundedRectInside(px, py, pw, ph, rTL + keep, rTR + keep, rBR + keep, rBL + keep);
          if(inside_shape) continue;

          canvas->PutPixel((double)(x0 + px), (double)(y0 + py), &parent);
        }
    }

  canvas->SetClipBox(ox1, oy1, ox2, oy2);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SkinCanvas_DrawFramedPanelBackground
* @brief      Fill + stroke a panel with the same rounded-corner contract as Draw_Form (page seed under the
*             AABB, RoundRect isfill=true, PunchRoundRectCornerTips). Console / Tree / PropertyList used a bare
*             RoundRect before — corners looked sharper or dirtier than FORM (e.g. XTraceMonitor opstatus foot).
* @note       INTERNAL / FILE LOCAL.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool UI_SkinCanvas_DrawFramedPanelBackground(GRP2DCANVAS* canvas, GRPSCREEN* screen, UI_ELEMENT* element,
                                                    double x_position, double y_position, double box_w, double box_h)
{
  if(!canvas || !element) return false;

  double y_top = UI_BOUNDARYLINE_EdgeTop(y_position, box_h);

  bool has_bg     = element->IsBackgroundColorSet() && (element->GetBackgroundColor()->GetAlpha() > 0);
  bool has_border = element->IsBorderColorSet() && (element->GetBorderWidth() > 0.0);
  if(!has_bg && !has_border) return false;

  GRP2DCOLOR_RGBA8 fill(0, 0, 0, 0);
  if(has_bg)
    {
      UI_COLOR* bg = element->GetBackgroundColor();
      fill = GRP2DCOLOR_RGBA8(bg->GetRed(), bg->GetGreen(), bg->GetBlue(), bg->GetAlpha());
    }

  double           bw = element->GetBorderWidth();
  GRP2DCOLOR_RGBA8 line(0, 0, 0, 0);
  if(has_border)
    {
      UI_COLOR* lc = element->GetBorderColor();
      line = GRP2DCOLOR_RGBA8(lc->GetRed(), lc->GetGreen(), lc->GetBlue(), lc->GetAlpha());
    }

  double minx = x_position;
  double maxx = x_position + box_w;
  double miny = y_top;
  double maxy = y_position;
  double rr   = (double)element->GetRoundRect();

  // Same page-seed as Draw_Form's classic GetRoundRect path: father bg → neighbour sample → opaque page.
  GRP2DCOLOR_RGBA8 seed_bg(0, 0, 0, 0);
  {
    UI_ELEMENT* father = element->GetFather();
    UI_COLOR*   fbg    = NULL;
    if(father)
      {
        if(father->IsBackgroundColorSet()) fbg = father->GetBackgroundColor();
        else                               fbg = father->GetColor();
      }
    if(fbg && (fbg->GetAlpha() > 0) && ((fbg->GetRed() + fbg->GetGreen() + fbg->GetBlue()) > 8))
      {
        seed_bg = GRP2DCOLOR_RGBA8((XBYTE)fbg->GetRed(), (XBYTE)fbg->GetGreen(), (XBYTE)fbg->GetBlue(), (XBYTE)fbg->GetAlpha());
      }
     else if(screen && !screen->Style_Is(GRPSCREENSTYLE_TRANSPARENT))
      {
        const double sx[4] = { minx - 2.0, maxx + 1.0, maxx + 1.0, minx - 2.0 };
        const double sy[4] = { miny - 2.0, miny - 2.0, maxy + 1.0, maxy + 1.0 };
        bool have = false;
        for(int i = 0; i < 4 && !have; i++)
          {
            GRP2DCOLOR* c = canvas->GetPixel(sx[i], sy[i]);
            if(!c) continue;
            GRP2DCOLOR_RGBA8 s = *(GRP2DCOLOR_RGBA8*)c;
            if(s.a > 0 && (s.r + s.g + s.b) > 8) { seed_bg = s; have = true; }
          }
        if(!have) seed_bg = GRP2DCOLOR_RGBA8(0x19, 0x1E, 0x24, 255);
      }
  }

  if(seed_bg.a > 0)
    {
      GRP2DCOLOR_RGBA8 line_none(0, 0, 0, 0);
      canvas->SetFillColor(&seed_bg);
      canvas->SetLineColor(&line_none);
      canvas->SetLineWidth(1.0f);
      canvas->Rectangle(minx - 1.0, maxy + 1.0, maxx + 1.0, miny - 1.0, true);
    }

  if(!has_border || (bw == 0.0))
    {
      GRP2DCOLOR_RGBA8 line_none(0, 0, 0, 0);
      canvas->SetLineColor(&line_none);
      canvas->SetLineWidth(1.0f);
    }
   else
    {
      canvas->SetLineColor(&line);
      canvas->SetLineWidth((bw < 0.0) ? 1.0f : (float)bw);
    }
  canvas->SetFillColor(&fill);

  if(rr > 0.0)
    {
      canvas->RoundRect(minx, maxy, maxx, miny, rr, true);
      // Seeded AABB already removes pico negro; punching the tip box can erase the border stroke
      // on the bottom corners of overflow:hidden rounded panels.
      if(seed_bg.a == 0)
        {
          UI_SkinCanvas_PunchRoundRectCornerTips(canvas, NULL, minx, miny, maxx, maxy, rr, rr, rr, rr, &seed_bg);
        }
    }
   else
    {
      canvas->Rectangle(minx, maxy, maxx, miny, true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SkinCanvas_DrawSoftShadow(GRP2DCANVAS* canvas, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL, UI_COLOR* shadow_color, int blur_radius)
* @brief      Draw a soft-edged rounded-rect drop shadow. Rasterises the silhouette into an off-screen RGBA
*             bitmap padded to hold the blur fade, runs agg::stack_blur_rgba32 on it, then composites via
*             the canvas's PutBitmapAlpha. Falls back cleanly on unsupported canvas modes (returns false so
*             the caller can draw a hard shadow instead).
* @note       INTERNAL / FILE LOCAL.
* @ingroup    USERINTERFACE
*
* @param[in]  canvas : Target canvas.
* @param[in]  minx : Shadow rect left edge (screen coords).
* @param[in]  miny : Shadow rect top edge.
* @param[in]  maxx : Shadow rect right edge.
* @param[in]  maxy : Shadow rect bottom edge.
* @param[in]  rTL : Top-left corner radius.
* @param[in]  rTR : Top-right corner radius.
* @param[in]  rBR : Bottom-right corner radius.
* @param[in]  rBL : Bottom-left corner radius.
* @param[in]  shadow_color : Shadow tint (RGB used everywhere; A used inside the silhouette).
* @param[in]  blur_radius : Blur radius in pixels; must be > 0.
*
* @return     bool : true on success; false when the canvas mode is not 32-bit RGBA/BGRA (caller draws hard).
*
* --------------------------------------------------------------------------------------------------------------------*/
// Rasterises the shadow silhouette into a NEW off-screen RGBA bitmap, padded to hold the blur fade, and runs
// agg::stack_blur_rgba32 on it. Does NOT composite it onto the canvas and does NOT delete it -- the caller owns
// the returned bitmap (either compositing-then-deleting it immediately, as UI_SkinCanvas_DrawSoftShadow_Impl
// does for the two non-Form call sites, or compositing-then-CACHING it on the owning UI_ELEMENT_FORM, as
// UI_SkinCanvas_DrawSoftShadow_FormCached does, so the SAME bitmap can be reused on a later frame instead of
// rebuilt from scratch). Returns NULL on failure (caller falls back to a hard shadow).
static GRPBITMAP* UI_SkinCanvas_BuildSoftShadowBitmap(int shape_w, int shape_h, double rTL, double rTR, double rBR, double rBL, UI_COLOR* shadow_color, int blur_radius)
{
  if(!shadow_color || blur_radius <= 0)   return NULL;
  if(shape_w <= 0 || shape_h <= 0)        return NULL;

  // Pad the bitmap on each side so the fade at the shape's border has room to fully dissipate before the
  // buffer ends. Padding tighter than the blur radius would leave a visible hard cut. The very same formula
  // drives the rebuild-area expansion in PreDrawFunction -- keep them in sync via the shared macro.
  int pad  = UI_SKINCANVAS_SHADOW_BLURPADDING(blur_radius);
  int bw   = shape_w + (2 * pad);
  int bh   = shape_h + (2 * pad);

  // Always create the off-screen bitmap in RGBA_8888. This decouples the shadow pipeline from the canvas
  // native pixel format: our per-pixel writes below use fixed R=0/G=1/B=2/A=3 offsets, and the composite
  // step (UI_SkinCanvas_CompositeSoftShadowBitmap) goes through canvas->PutBlendPixel with a GRP2DCOLOR_RGBA8
  // -- the canvas layer handles whatever mode conversion is needed to write into 24-bit RGB, 16-bit RGB565 or
  // another 32-bit backend.
  GRPBITMAP* bitmap = GRPFACTORY::GetInstance().CreateBitmap(bw, bh, GRPPROPERTYMODE_32_RGBA_8888);
  if(!bitmap)         return NULL;
  if(!bitmap->IsValid())
    {
      GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
      return NULL;
    }

  XBYTE* buf = bitmap->GetBuffer();
  if(!buf)
    {
      GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
      return NULL;
    }

  // Fixed RGBA_8888 layout: R, G, B, A per pixel.
  const int r_off = 0;
  const int g_off = 1;
  const int b_off = 2;
  const int a_off = 3;

  XBYTE sr = (XBYTE)shadow_color->GetRed();
  XBYTE sg = (XBYTE)shadow_color->GetGreen();
  XBYTE sb = (XBYTE)shadow_color->GetBlue();
  XBYTE sa = (XBYTE)shadow_color->GetAlpha();

  // First pass: fill every pixel with the shadow RGB but alpha 0 (transparent shadow colour). This way the
  // subsequent blur averages RGB against itself (no drift) and only alpha fades from full-shadow to 0 at the
  // silhouette boundary -- the standard trick for correct soft-shadow colour at the edges.
  for(int y = 0; y < bh; y++)
    {
      XBYTE* row = buf + (y * bw * 4);
      for(int x = 0; x < bw; x++)
        {
          int off = x * 4;
          row[off + r_off] = sr;
          row[off + g_off] = sg;
          row[off + b_off] = sb;
          row[off + a_off] = 0;
        }
    }

  // Second pass: set alpha to sa inside the rounded rectangle silhouette (positioned at (pad, pad) relative
  // to the bitmap origin). Everything outside stays transparent.
  for(int y = 0; y < shape_h; y++)
    {
      XBYTE* row = buf + ((y + pad) * bw * 4);
      for(int x = 0; x < shape_w; x++)
        {
          if(UI_SkinCanvas_RoundedRectInside(x, y, shape_w, shape_h, rTL, rTR, rBR, rBL))
            {
              row[((x + pad) * 4) + a_off] = sa;
            }
        }
    }

  // Literal AGG stack blur (agg_blur.h), applied in place on the same off-screen RGBA_8888 buffer built above.
  // agg::stack_blur_rgba32 is templated on an "Img" concept (width()/height()/stride()/pix_ptr()/order_type),
  // not on a raw agg::rendering_buffer -- agg::pixfmt_rgba32 (order_rgba: R=0,G=1,B=2,A=3, agg_pixfmt_rgba.h)
  // is exactly that wrapper, and its byte order matches r_off/g_off/b_off/a_off above pixel for pixel, so no
  // channel shuffling is needed. rx/ry both use blur_radius: a single isotropic blur, same as the box-blur
  // version this replaces. See the NOTE on UI_SKINCANVAS_SHADOW_BLURPADDING for why the existing 2x padding
  // is still (more than) enough for this algorithm's narrower kernel support.
  {
    agg::rendering_buffer rbuf;
    rbuf.attach(buf, (unsigned)bw, (unsigned)bh, bw * 4);

    agg::pixfmt_rgba32 pixf(rbuf);

    agg::stack_blur_rgba32(pixf, (unsigned)blur_radius, (unsigned)blur_radius);
  }

  // CORNER SPIKES FIX (2026-09, UI_System cards): stack blur spreads alpha into (1) the four AABB corner
  // cutouts inside the shape rect and (2) the PAD pixels that sit in the same corner quadrants just
  // outside the AABB -- both read as sharp black "puntas" once the rounded fill is painted. Zero any
  // pixel that is outside the rounded silhouette (shape + pad), not only the in-AABB cutouts.
  for(int y = 0; y < bh; y++)
    {
      XBYTE* row = buf + (y * bw * 4);
      for(int x = 0; x < bw; x++)
        {
          int sx = x - pad;
          int sy = y - pad;
          // Inside the shape rect: keep only rounded-interior pixels. Outside (pad): keep only pixels
          // that are still within an expanded rounded test -- i.e. drop corner-quadrant pad tips.
          // For pad, treat coordinates relative to shape; outside shape AABB use circle tests per corner.
          if(sx >= 0 && sy >= 0 && sx < shape_w && sy < shape_h)
            {
              if(!UI_SkinCanvas_RoundedRectInside(sx, sy, shape_w, shape_h, rTL, rTR, rBR, rBL))
                {
                  row[(x * 4) + a_off] = 0;
                }
            }
           else
            {
              // Pad pixel: clear if it lies in a corner quadrant outside the rounded outline's circle.
              bool clear = false;
              double fx = (double)sx + 0.5;
              double fy = (double)sy + 0.5;

              if(fx < rTL && fy < rTL)
                {
                  double dx = fx - rTL;
                  double dy = fy - rTL;
                  if((dx * dx + dy * dy) > (rTL * rTL)) clear = true;
                }
              if(fx > ((double)shape_w - rTR) && fy < rTR)
                {
                  double dx = fx - ((double)shape_w - rTR);
                  double dy = fy - rTR;
                  if((dx * dx + dy * dy) > (rTR * rTR)) clear = true;
                }
              if(fx > ((double)shape_w - rBR) && fy > ((double)shape_h - rBR))
                {
                  double dx = fx - ((double)shape_w - rBR);
                  double dy = fy - ((double)shape_h - rBR);
                  if((dx * dx + dy * dy) > (rBR * rBR)) clear = true;
                }
              if(fx < rBL && fy > ((double)shape_h - rBL))
                {
                  double dx = fx - rBL;
                  double dy = fy - ((double)shape_h - rBL);
                  if((dx * dx + dy * dy) > (rBL * rBL)) clear = true;
                }

              if(clear) row[(x * 4) + a_off] = 0;
            }
        }
    }

  // One-shot self-check: the first time a soft shadow renders anywhere in the process, log the parameters
  // so the console confirms the soft path was reached and shows the effective bitmap / blur / colour. Any
  // subsequent invocation is silent so the log stays clean.
  static bool once = true;
  if(once)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, NL("[UI Draw] soft shadow: bitmap=%dx%d blur=%d shadow_alpha=%d"), bw, bh, blur_radius, (int)shadow_color->GetAlpha());
      once = false;
    }

  return bitmap;
}


// Composites an already-built (fresh or cached) blurred shadow bitmap onto the canvas, anchored so its
// silhouette lands back at (minx, miny) exactly as when it was rasterised (the padding added by
// UI_SkinCanvas_BuildSoftShadowBitmap is recovered here from blur_radius via the same shared macro, so the
// caller does not need to remember it). Safe to call every frame even when reusing a cached bitmap: this is
// the part of the work that genuinely must happen on every redraw (the canvas pixels underneath were erased).
static void UI_SkinCanvas_CompositeSoftShadowBitmap(GRP2DCANVAS* canvas, GRPBITMAP* bitmap, double minx, double miny, int blur_radius)
{
  if(!canvas || !bitmap) return;

  int pad = UI_SKINCANVAS_SHADOW_BLURPADDING(blur_radius);
  int bw  = (int)bitmap->GetWidth();
  int bh  = (int)bitmap->GetHeight();

  XBYTE* buf = bitmap->GetBuffer();
  if(!buf) return;

  const int r_off = 0;
  const int g_off = 1;
  const int b_off = 2;
  const int a_off = 3;

  // Composite the blurred bitmap onto the canvas. We deliberately do NOT use canvas->PutBitmapAlpha because
  // that helper passes the source pixel's alpha as agg's cover_type, which then multiplies the source alpha
  // AGAIN internally -- effectively squaring the alpha and crushing every semi-transparent edge to zero.
  // The blurred soft edges of a shadow are almost entirely in that low-alpha range, so PutBitmapAlpha would
  // reduce the shadow to only its most opaque central band, producing the same hard-edged silhouette we are
  // trying to escape. Instead we iterate and call PutBlendPixel with cover=255 (full coverage), so the
  // compositing weight is exactly the pixel's own alpha and the soft gradient survives intact.
  double dest_x = minx - (double)pad;
  double dest_y = miny - (double)pad;

  for(int y = 0; y < bh; y++)
    {
      XBYTE* row = buf + (y * bw * 4);
      for(int x = 0; x < bw; x++)
        {
          int off = x * 4;
          XBYTE a = row[off + a_off];
          if(a == 0) continue;

          GRP2DCOLOR_RGBA8 c(row[off + r_off], row[off + g_off], row[off + b_off], a);
          canvas->PutBlendPixel(dest_x + (double)x, dest_y + (double)y, &c, 255.0);
        }
    }
}


// Behaviour-preserving recombination of UI_SkinCanvas_BuildSoftShadowBitmap + UI_SkinCanvas_CompositeSoftShadowBitmap
// for the two call sites that do not (yet) cache: build, composite, delete, in one shot -- exactly what the
// original single-piece function used to do.
static bool UI_SkinCanvas_DrawSoftShadow_Impl(GRP2DCANVAS* canvas, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL, UI_COLOR* shadow_color, int blur_radius)
{
  if(!canvas) return false;

  int shape_w = (int)(maxx - minx);
  int shape_h = (int)(maxy - miny);

  GRPBITMAP* bitmap = UI_SkinCanvas_BuildSoftShadowBitmap(shape_w, shape_h, rTL, rTR, rBR, rBL, shadow_color, blur_radius);
  if(!bitmap) return false;

  UI_SkinCanvas_CompositeSoftShadowBitmap(canvas, bitmap, minx, miny, blur_radius);

  GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
  return true;
}


// Thin wrapper around UI_SkinCanvas_DrawSoftShadow_Impl (renamed above), kept under the ORIGINAL name so the
// two non-Form call sites are untouched.
static bool UI_SkinCanvas_DrawSoftShadow(GRP2DCANVAS* canvas, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL, UI_COLOR* shadow_color, int blur_radius)
{
  return UI_SkinCanvas_DrawSoftShadow_Impl(canvas, minx, miny, maxx, maxy, rTL, rTR, rBR, rBL, shadow_color, blur_radius);
}


// RADIAL HALO (2026-09, UI_System ring-cpu / ring-ram): a progressradial's CSS "box-shadow" must read as a soft
// glow AROUND the ring, not as a filled AABB/disc soft-shadow. DrawElementBoxShadow() rasterises a filled
// rounded-rect silhouette; with blur>0 that still leaves a near-opaque wash across the WHOLE widget box --
// including the hollow centre where the "%" caption sits. TextBackdrop then captures that wash under a
// text-sized rectangle and PutBitmapNoAlpha-restores it as a hard blue/purple block behind "9%" (confirmed
// against the Monitor del Sistema prototype vs live capture). This helper fills ONLY an annular band
// (inner_r..outer_r), blurs it with the same AGG path, and composites it -- centre stays the card backdrop.
static bool UI_SkinCanvas_DrawSoftRingGlow(GRP2DCANVAS* canvas, double cx, double cy, double outer_r, double inner_r, UI_COLOR* shadow_color, int blur_radius)
{
  if(!canvas || !shadow_color)        return false;
  if(blur_radius <= 0)                return false;
  if(outer_r <= 1.0)                  return false;
  if(inner_r < 0.0)                   inner_r = 0.0;
  if(inner_r >= outer_r)              inner_r = outer_r * 0.5;

  int shape_w = (int)(outer_r * 2.0 + 0.5);
  int shape_h = shape_w;
  if(shape_w <= 0) return false;

  int pad = UI_SKINCANVAS_SHADOW_BLURPADDING(blur_radius);
  int bw  = shape_w + (2 * pad);
  int bh  = shape_h + (2 * pad);

  GRPBITMAP* bitmap = GRPFACTORY::GetInstance().CreateBitmap(bw, bh, GRPPROPERTYMODE_32_RGBA_8888);
  if(!bitmap)         return false;
  if(!bitmap->IsValid())
    {
      GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
      return false;
    }

  XBYTE* buf = bitmap->GetBuffer();
  if(!buf)
    {
      GRPFACTORY::GetInstance().DeleteBitmap(bitmap);
      return false;
    }

  const int r_off = 0;
  const int g_off = 1;
  const int b_off = 2;
  const int a_off = 3;

  XBYTE sr = (XBYTE)shadow_color->GetRed();
  XBYTE sg = (XBYTE)shadow_color->GetGreen();
  XBYTE sb = (XBYTE)shadow_color->GetBlue();
  XBYTE sa = (XBYTE)shadow_color->GetAlpha();

  double mid = outer_r;   // silhouette centre in shape-local coords (top-left origin)
  double out2 = outer_r * outer_r;
  double in2  = inner_r * inner_r;

  for(int y = 0; y < bh; y++)
    {
      XBYTE* row = buf + (y * bw * 4);
      for(int x = 0; x < bw; x++)
        {
          int off = x * 4;
          row[off + r_off] = sr;
          row[off + g_off] = sg;
          row[off + b_off] = sb;
          row[off + a_off] = 0;
        }
    }

  for(int y = 0; y < shape_h; y++)
    {
      XBYTE* row = buf + ((y + pad) * bw * 4);
      for(int x = 0; x < shape_w; x++)
        {
          double dx = ((double)x + 0.5) - mid;
          double dy = ((double)y + 0.5) - mid;
          double d2 = dx * dx + dy * dy;

          if(d2 <= out2 && d2 >= in2)
            {
              row[((x + pad) * 4) + a_off] = sa;
            }
        }
    }

  {
    agg::rendering_buffer rbuf;
    rbuf.attach(buf, (unsigned)bw, (unsigned)bh, bw * 4);
    agg::pixfmt_rgba32 pixf(rbuf);
    agg::stack_blur_rgba32(pixf, (unsigned)blur_radius, (unsigned)blur_radius);
  }

  double minx = cx - outer_r;
  double miny = cy - outer_r;   // EdgeTop convention: smaller y is the top of the box

  UI_SkinCanvas_CompositeSoftShadowBitmap(canvas, bitmap, minx, miny, blur_radius);
  GRPFACTORY::GetInstance().DeleteBitmap(bitmap);

  return true;
}


// Box-shadow entry point used ONLY by Draw_Form. Reuses the already-blurred bitmap cached on "element_form"
// when the shadow's appearance (size/corner-radii/blur/colour) has not changed since it was last rendered,
// instead of rebuilding it from scratch on every redraw. The other call sites (generic DrawElementBoxShadow
// for images/animations; ProgressBar track shadow) keep using the uncached UI_SkinCanvas_DrawSoftShadow above.
static bool UI_SkinCanvas_DrawSoftShadow_FormCached(GRP2DCANVAS* canvas, UI_ELEMENT_FORM* element_form, double minx, double miny, double maxx, double maxy, double rTL, double rTR, double rBR, double rBL, UI_COLOR* shadow_color, int blur_radius)
{
  if(!canvas || !element_form) return false;

  int shape_w = (int)(maxx - minx);
  int shape_h = (int)(maxy - miny);

  GRPBITMAP* bitmap = element_form->ShadowCache_GetIfMatches(shape_w, shape_h, rTL, rTR, rBR, rBL, blur_radius, shadow_color);

  if(!bitmap)
    {
      bitmap = UI_SkinCanvas_BuildSoftShadowBitmap(shape_w, shape_h, rTL, rTR, rBR, rBL, shadow_color, blur_radius);
      if(!bitmap)
        {
          return false;
        }

      // Ownership transfers to the element's cache -- NOT deleted here. It is freed either the next time this
      // element's shadow key changes (ShadowCache_Set releases the old one first) or when the element itself
      // is destroyed (UI_ELEMENT_FORM::~UI_ELEMENT_FORM -> ShadowCache_Release).
      element_form->ShadowCache_Set(bitmap, shape_w, shape_h, rTL, rTR, rBR, rBL, blur_radius, shadow_color);
    }

  UI_SkinCanvas_CompositeSoftShadowBitmap(canvas, bitmap, minx, miny, blur_radius);

  return true;
}


// CARD / RADIAL NEIGHBOUR WIPE FIX (2026-09, UI_System triage Oleada 1): FormBackdrop / RadialBackdrop restore
// can blank ink owned by a sibling that is NOT dirty this tick (no rebuild-area registered), so MarkOverlapping-
// AreasDirty never sees it. Typical case: a progressradial's glow pad (blur*2) overlaps the next card, or two
// cards share a 22px vertical strip. Walk the layout's fatherless roots and mark every intersecting sibling
// subtree dirty so this same frame's draw pass repaints them after the wipe.
static void UI_SkinCanvas_MarkSubtreeDirty(UI_ELEMENT* element)
{
  if(!element) return;

  element->SetMustReDraw(true);

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
      if(subelement) UI_SkinCanvas_MarkSubtreeDirty(subelement);
    }
}


static void UI_SkinCanvas_DirtyOverlappingLayoutSiblings(UI_ELEMENT* element, double x, double y, double w, double h)
{
  if(!element) return;
  if(w <= 0.0 || h <= 0.0) return;

  UI_LAYOUT* layout = element->GetLayout();
  if(!layout) return;

  XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
  if(!roots) return;

  UI_ELEMENT* selfroot = element;
  while(selfroot->GetFather()) selfroot = selfroot->GetFather();

  double wipe_r = x + w;
  double wipe_b = y + h;

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      UI_ELEMENT* sib = roots->Get(c);
      if(!sib || sib == selfroot) continue;
      if(!sib->IsVisible()) continue;

      UI_BOUNDARYLINE* bl = sib->GetBoundaryLine();
      if(!bl) continue;

      double el = sib->GetXPosition();
      double eb = sib->GetYPosition();
      double et = UI_BOUNDARYLINE_EdgeTop(eb, bl->height);
      double er = el + bl->width;

      bool overlaps = (x < er) && (el < wipe_r) && (y < eb) && (et < wipe_b);
      if(!overlaps) continue;

      UI_SkinCanvas_MarkSubtreeDirty(sib);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SkinCanvas_RestoreBackdropIntersection
* @brief      PutBitmapNoAlpha only the INTERSECTION of a cached backdrop with a caller rect.
* @note       Full-box restore of Option/Text/Anim backdrops from a small neighbour (toolbar legend form
*             under a 59×59 bubble) re-painted the entire bubble AABB — including a gray strip in the
*             legend band outside the form fill — and left Demarrer/Arreter on a permanent plate after
*             maximize/restore. Same pattern as FormBackdrop sibling-contamination crop in Draw_Form.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_RestoreBackdropIntersection(UI_SKINCANVAS* skin, GRP2DREBUILDAREA* entry,
                                                     double clip_l, double clip_t, double clip_r, double clip_b)
{
  if(!skin || !entry || !entry->GetBitmap()) return;

  double left2   = entry->GetXPos();
  double top2    = entry->GetYPos();
  double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
  double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

  double ix1 = (clip_l > left2)  ? clip_l : left2;
  double iy1 = (clip_t > top2)   ? clip_t : top2;
  double ix2 = (clip_r < right2) ? clip_r : right2;
  double iy2 = (clip_b < bottom2)? clip_b : bottom2;
  if((ix2 <= ix1) || (iy2 <= iy1)) return;

  // Exact full-box hit: avoid an extra GetSubBitmap alloc.
  if((ix1 == left2) && (iy1 == top2) && (ix2 == right2) && (iy2 == bottom2))
    {
      skin->PutBitmapNoAlpha(left2, top2, entry->GetBitmap());
      return;
    }

  GRPRECTINT croprect((int)(ix1 - left2), (int)(iy1 - top2), (int)(ix2 - left2), (int)(iy2 - top2));
  GRPBITMAP* cropped = entry->GetBitmap()->GetSubBitmap(croprect);
  if(cropped)
    {
      skin->PutBitmapNoAlpha(ix1, iy1, cropped);
      GEN_DELETE cropped;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::DrawElementBoxShadow(GRP2DCANVAS* canvas, UI_ELEMENT* element, double x_position, double y_position)
* @brief      Draws an element's box-shadow layer, generic across widget types.
* @note       Shared box-model helper. Same box-shadow behaviour Draw_Form has had since steps 7-8 (soft shadow via
*             UI_SkinCanvas_DrawSoftShadow/agg::stack_blur_rgba32 when blur > 0, hard-edged silhouette otherwise),
*             but written against base UI_ELEMENT accessors only (IsBoxShadowSet/GetShadowOffsetX/GetShadowOffsetY/
*             GetShadowBlur/GetShadowColor/HasAnyPerCornerRadius/GetEffectiveBorderRadius/GetRoundRect are all
*             declared on UI_ELEMENT itself, not UI_ELEMENT_FORM) so any Draw_X, in any UI_SKINCANVAS subclass,
*             can call it. Draw_Form's own inline copy of this logic is deliberately left untouched rather than
*             rewired to call this helper: it anchors the shadow on GetVisibleRect(), a UI_ELEMENT_FORM-specific
*             rect (a distinct stored member, not simply recomputed from the base boundary line) that this
*             function cannot reproduce without risking a behaviour change on the one widget that already shipped
*             and was verified. This helper instead anchors the shadow on (x_position, y_position) plus
*             element->GetBoundaryLine()->width/height -- the exact same geometry UI_SKINCANVAS::PreDrawFunction
*             already uses to expand the rebuild-area for a shadow on ANY element type (see its own "Step 7"
*             block), and the same (x_position, y_position) the calling Draw_X already uses to place its own
*             content -- so the shadow, the widget's own drawing, and the invalidation rectangle all agree on
*             where the element is, for every type this is wired into.
* @ingroup    USERINTERFACE
*
* @param[in]  canvas : Target canvas.
* @param[in]  element : Element whose box-shadow (if any) should be drawn. A no-op if IsBoxShadowSet() is false.
* @param[in]  x_position : Element's resolved left edge (screen coords), as already computed by PreDrawFunction.
* @param[in]  y_position : Element's resolved bottom edge (screen coords), as already computed by PreDrawFunction.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::DrawElementBoxShadow(GRP2DCANVAS* canvas, UI_ELEMENT* element, double x_position, double y_position)
{
  if(!canvas)                        return;
  if(!element)                       return;
  if(!element->IsBoxShadowSet())     return;

  double width  = element->GetBoundaryLine()->width;
  double height = element->GetBoundaryLine()->height;

  double minx = UI_BOUNDARYLINE_EdgeLeft  (x_position, width);
  double maxx = UI_BOUNDARYLINE_EdgeRight (x_position, width);
  double miny = UI_BOUNDARYLINE_EdgeTop   (y_position, height);
  double maxy = UI_BOUNDARYLINE_EdgeBottom(y_position, height);

  double sh_x    = element->GetShadowOffsetX();
  double sh_y    = element->GetShadowOffsetY();
  double sh_blur = element->GetShadowBlur();

  double sh_minx = minx + sh_x;
  double sh_miny = miny + sh_y;
  double sh_maxx = maxx + sh_x;
  double sh_maxy = maxy + sh_y;

  // Resolve corner radii once: per-corner if any is authored, else uniform roundrect on all four -- identical
  // precedence to Draw_Form's own copy of this logic.
  double rTL, rTR, rBR, rBL;
  if(element->HasAnyPerCornerRadius())
    {
      rTL = element->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TL);
      rTR = element->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TR);
      rBR = element->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BR);
      rBL = element->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BL);
    }
   else
    {
      double r = (double)element->GetRoundRect();
      rTL = rTR = rBR = rBL = r;
    }

  bool soft_ok = false;
  if(sh_blur > 0.0)
    {
      soft_ok = UI_SkinCanvas_DrawSoftShadow(canvas, sh_minx, sh_miny, sh_maxx, sh_maxy, rTL, rTR, rBR, rBL, element->GetShadowColor(), (int)sh_blur);
    }

  if(!soft_ok)
    {
      // Hard shadow fallback (no blur, or canvas mode not 32-bit) -- same construction as Draw_Form's.
      GRP2DCOLOR_RGBA8  shadow_col(element->GetShadowColor()->GetRed(), element->GetShadowColor()->GetGreen(), element->GetShadowColor()->GetBlue(), element->GetShadowColor()->GetAlpha());

      GRP2DCOLOR_RGBA8  shadow_line_none(0, 0, 0, 0);

      canvas->SetFillColor(&shadow_col);
      canvas->SetLineColor(&shadow_line_none);
      canvas->SetLineWidth(1.0f);

      if(element->HasAnyPerCornerRadius())
        {
          GRP2DPATH shpath;
          AppendRoundRectPathPerCorner(shpath, sh_minx, sh_miny, sh_maxx, sh_maxy, rTL, rTR, rBR, rBL);
          canvas->Path(shpath, true);
        }
       else if(element->GetRoundRect())
        {
          canvas->RoundRect(sh_minx, sh_maxy, sh_maxx, sh_miny, element->GetRoundRect(), true);
        }
       else
        {
          canvas->Rectangle(sh_minx, sh_maxy, sh_maxx, sh_miny, true);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SkinCanvas_ProgressBar_DrawGradientRect(GRP2DCANVAS* canvas, double x1, double y1, double x2, double y2, double radius, GRP2DGRADIENTSTOP* stops, double gx1, double gy1, double gx2, double gy2)
* @brief      Fills a (optionally rounded) rect with a linear gradient along (gx1,gy1)->(gx2,gy2).
* @note       INTERNAL / FILE LOCAL. Used only when gradientcolor is set; the solid DrawRect path is left untouched.
* @ingroup    USERINTERFACE
* 
* @param[in]  canvas : Canvas object to use.
* @param[in]  x1 : X1 value.
* @param[in]  y1 : Y1 value.
* @param[in]  x2 : X2 value.
* @param[in]  y2 : Y2 value.
* @param[in]  radius : Radius value.
* @param[in]  stops : Stops pointer to use.
* @param[in]  gx1 : Gx1 value.
* @param[in]  gy1 : Gy1 value.
* @param[in]  gx2 : Gx2 value.
* @param[in]  gy2 : Gy2 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_ProgressBar_DrawGradientRect(GRP2DCANVAS* canvas, double x1, double y1, double x2, double y2, double radius, GRP2DGRADIENTSTOP* stops, double gx1, double gy1, double gx2, double gy2)
{
  if(!canvas || !stops) return;

  double minx = __MIN(x1, x2), maxx = __MAX(x1, x2);
  double miny = __MIN(y1, y2), maxy = __MAX(y1, y2);
  double w    = maxx - minx,   h    = maxy - miny;

  if((w <= 0.0) || (h <= 0.0)) return;                                                // nothing to draw (e.g. level 0)

  double r    = radius;
  double maxr = ((w < h) ? w : h) / 2.0;
  if(r > maxr) r = maxr;

  GRP2DPATH path;
  UI_SkinCanvas_AppendRoundRectPath(path, minx, miny, maxx, maxy, r);

  canvas->PathGradientLinear(path, false, gx1, gy1, gx2, gy2, stops, 2);
  canvas->Path(path, false);                                                          // stroke the same outline with the caller's line color/width (the solid RoundRect did this; the gradient fill does not)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS_REBUILDAREAS::UI_SKINCANVAS_REBUILDAREAS(GRPSCREEN* screen, int viewportindex)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewportindex value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS_REBUILDAREAS::UI_SKINCANVAS_REBUILDAREAS(GRPSCREEN* screen, int viewportindex)
{
  Clean();

  SetExcessEdge(0);

  GRPVIEWPORT* viewport = NULL;  

  this->viewportindex = viewportindex;
 
  this->screen = screen;
  if(screen) 
    { 
      viewport = screen->GetViewport(viewportindex);
      if(viewport) 
        {
          canvas = viewport->GetCanvas();
        }
    }      
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS_REBUILDAREAS::~UI_SKINCANVAS_REBUILDAREAS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS_REBUILDAREAS::~UI_SKINCANVAS_REBUILDAREAS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas()
* @brief      Rebuild all areas
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas()
{
  XDWORD max_z_level = 0;
  XDWORD nareas      = areas.GetSize();

  if(!nareas) return true;

  // Invalidating one saved area can expose pixels owned by another area that overlaps it. Resolve that
  // dependency transitively BEFORE restoring a single bitmap: every affected element is then dirty when the
  // restore pass starts, so all layers are peeled in one transaction and no neighbour is deferred to the next
  // frame. Deferring even one overlap is observable as a one-frame empty card/menu flash.
  XDWORD ndirty_before = 0;
  XDWORD ndirty_after  = 0;

  do
    {
      ndirty_before = 0;

      for(XDWORD c=0; c<nareas; c++)
        {
          GRP2DREBUILDAREA* area = areas.Get(c);
          if(!area) continue;

          UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
          if(element && (element->MustReDraw() || (!element->IsVisible()))) ndirty_before++;
        }

      for(XDWORD c=0; c<nareas; c++)
        {
          GRP2DREBUILDAREA* area = areas.Get(c);
          if(!area) continue;

          UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
          if(!element || ((!element->MustReDraw()) && element->IsVisible())) continue;

          // Do not let a stuck-dirty modal propagate MustReDraw onto ListBoxMenu / other siblings: the
          // modal composite layer will blit over the intersection; peeling neighbours for the modal's
          // sake recreates the punch-through cycle.
          if(GEN_USERINTERFACE.ModalLayer_IsRebuildProtected(element)) continue;

          GRPBITMAP* bitmap = area->GetBitmap();
          if(bitmap) MarkOverlappingAreasDirty(area, bitmap, (int)c);
        }

      ndirty_after = 0;

      for(XDWORD c=0; c<nareas; c++)
        {
          GRP2DREBUILDAREA* area = areas.Get(c);
          if(!area) continue;

          UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
          if(element && (element->MustReDraw() || (!element->IsVisible()))) ndirty_after++;
        }
    }
  while(ndirty_after > ndirty_before);

  for(XDWORD c=0; c<nareas; c++)
    {
      GRP2DREBUILDAREA* area = areas.Get(c);
      if(area)
        {
          UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
          if(element)
            {
              max_z_level =  __MAX(max_z_level, element->GetZLevel()); 
            }
        }
    }

  // Level zero is a real paint layer, not a sentinel. Every fatherless element keeps the default z_level=0,
  // including the cards and menu hit targets in UI_System. The old `level>0` loop never restored or deleted
  // those areas, so each redraw composited transparency and shadows over stale pixels indefinitely.
  //
  // CONTRACT: z_level orders RESTORE only (this loop). Paint order across layouts is screen composition
  // layers in UI_MANAGER::Update() (content → modal → chrome), not z_level.
  for(int level=(int)max_z_level; level>=0; level--)
    {
      nareas = areas.GetSize();  

      for(int index = nareas-1; index>=0; index--)  
        {
          GRP2DREBUILDAREA* area = areas.Get(index);
          if(area)
            {
              UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
              if(element)
                {
                  if(element->MustReDraw() || (!element->IsVisible()))
                    {
                      if(element->GetZLevel() == (XDWORD)level)
                        {
                          // Modal layer owns these pixels on the shared canvas: drop the area WITHOUT restoring
                          // and WITHOUT clearing MustReDraw (composite offscreen rebuild consumes that flag).
                          if(GEN_USERINTERFACE.ModalLayer_IsRebuildProtected(element))
                            {
                              areas.Delete(area);
                              GEN_DELETE area;
                              continue;
                            }

                          // XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, NL("Del area level [%d] [%s] "), element->GetZLevel(), element->GetName()->Get());
                          GRPBITMAP* bitmap = area->GetBitmap();
                          if(bitmap) PutBitmapNoAlpha(area->GetXPos(), area->GetYPos(), bitmap);

                          areas.Delete(area);
                          GEN_DELETE area;
                        }
                    }
                }
            }
        }
    }

  // Discard orphaned areas -- one whose owning element is neither dirty nor invisible was skipped by EVERY
  // z-level pass above (the "if" they are gated on never matched at any level), meaning nothing is scheduled
  // to redraw over it right now. That area's saved bitmap is the background from BEFORE the element's most
  // recent (successful, already-completed) draw; the element's CURRENT on-screen content is already correct,
  // so there is nothing to "undo" -- restoring that old snapshot would erase perfectly good pixels for no
  // reason. Left registered, it would instead sit inertly for an unbounded number of frames and eventually get
  // consumed whenever the element happens to become dirty again for a LATER, unrelated reason (a different
  // section selected, a future global Elements_SetToRedraw()...); at that point its bitmap is no longer a
  // snapshot of "just before this redraw" but of some arbitrary earlier moment, possibly missing content that
  // other elements have since drawn over the same rectangle. Restoring it then paints a stale picture over the
  // current correct one, and nothing is left dirty to draw it back -- exactly the multi-frame "blank flash"
  // seen after clicking a sidebar navigation entry (UI_SYSTEM::UserInterface_SelectSection() marks the WHOLE
  // tree dirty at once, so every element redraws and clears its own flag the same frame it was set; any area
  // whose owning element does not need a SECOND redraw this same frame is, by construction, exactly one frame
  // old and already served its purpose). Dropping it now (without restoring) costs nothing: if the element
  // becomes dirty again later, PreDrawFunction() finds no existing area and creates a fresh one from whatever
  // is on screen at that moment, which is always at least as correct as an old, possibly-invalidated snapshot.
  nareas = areas.GetSize();

  for(int index = (int)nareas-1; index>=0; index--)
    {
      GRP2DREBUILDAREA* area = areas.Get(index);
      if(!area) continue;

      UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
      if(!element) continue;

      if(element->MustReDraw() || (!element->IsVisible())) continue;  // still due a restore -- leave it alone

      areas.Delete(area);
      GEN_DELETE area;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS_REBUILDAREAS::MarkElementSubtreeDirty(UI_ELEMENT* element)
* @brief      Marks "element" and every element in its compose-children subtree as needing a redraw.
* @note       Root-cause fix for the "empty card" blank-flash defect: a rebuild area's neighbour is very often
*             an opaque container (a "form.card"/"form.inner-box"), not a leaf. Draw_Form() always visits every
*             child during its own redraw pass regardless of dirty state, but each child's own painter
*             (Draw_Text/Draw_Image/...) only emits pixels when THAT child's own MustReDraw() is true. Marking
*             only the container therefore repaints its background/border on top of whatever was already on
*             screen while its still-clean children never redraw to replace what just got painted over them --
*             a fully-formed but content-less card shell is exactly what gets presented that frame. Mirrors
*             UI_LAYOUT::Elements_SetToRedraw(element, true) (UI_Layout.cpp), repeated locally here because this
*             class only ever sees a bare UI_ELEMENT* (via a rebuild area's ExtraData) and has no UI_LAYOUT* to
*             call through -- see MarkOverlappingAreasDirty()'s own note on why this must stay inside the element
*             tree instead of reaching for a manager/layout singleton from inside the skin.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Root of the subtree to mark; NULL is a no-op.
*
* @return     void.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_REBUILDAREAS::MarkElementSubtreeDirty(UI_ELEMENT* element)
{
  if(!element) return;

  element->SetMustReDraw(true);

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
      if(subelement) MarkElementSubtreeDirty(subelement);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS_REBUILDAREAS::MarkOverlappingAreasDirty(GRP2DREBUILDAREA* area, GRPBITMAP* bitmap, int excludeindex)
* @brief      Marks dirty every other still-registered rebuild area whose saved rectangle overlaps the rectangle
*             of "area", so it gets its OWN proper restore+redraw cycle instead of being skipped as "clean" while
*             its shadow silently keeps stacking on top of a neighbor's. RebuildAllAreas() calls this in a
*             fixed-point pass before restoring any bitmap, so transitive overlaps are part of the same frame.
* @note       This function deliberately changes flags only. Two independently-owned, partially-overlapping
*             snapshots cannot both be replayed immediately here without one re-introducing stale data the other
*             has already cleared. The subsequent Z-ordered restore pass writes each affected snapshot once.
* @ingroup    USERINTERFACE
*
* @param[in]  area         : Area about to be restored/deleted by the caller; its saved rectangle is the overlap
*                             reference (xpos/ypos plus "bitmap"'s width/height).
* @param[in]  bitmap       : Bitmap of "area", already fetched by the caller (avoids fetching it twice).
* @param[in]  excludeindex : Index of "area" inside "areas", skipped so it is never compared against itself.
*
* @return     void.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_REBUILDAREAS::MarkOverlappingAreasDirty(GRP2DREBUILDAREA* area, GRPBITMAP* bitmap, int excludeindex)
{
  if((!area) || (!bitmap)) return;

  double x1 = area->GetXPos();
  double y1 = area->GetYPos();
  double w1 = (double)bitmap->GetWidth();
  double h1 = (double)bitmap->GetHeight();

  XDWORD nareas = areas.GetSize();

  for(int index = (int)nareas-1; index>=0; index--)
    {
      if(index == excludeindex) continue;

      GRP2DREBUILDAREA* neighborarea = areas.Get((XDWORD)index);
      if(!neighborarea) continue;

      GRPBITMAP* neighborbitmap = neighborarea->GetBitmap();
      if(!neighborbitmap) continue;

      double x2 = neighborarea->GetXPos();
      double y2 = neighborarea->GetYPos();
      double w2 = (double)neighborbitmap->GetWidth();
      double h2 = (double)neighborbitmap->GetHeight();

      bool overlaps = ((x1 < (x2+w2)) && (x2 < (x1+w1)) && (y1 < (y2+h2)) && (y2 < (y1+h1)));
      if(!overlaps) continue;

      // Just flag it dirty -- do NOT touch its bitmap or delete its area here. The restore phase starts only
      // after the complete overlap closure has been found, then restores it exactly once from its own snapshot.
      //
      // The flag must cover the neighbour's WHOLE subtree, not just the neighbour itself: see
      // MarkElementSubtreeDirty()'s note just above for why marking a single opaque container without its
      // children is exactly the "empty card" blank-flash defect (the container's own redraw repaints over its
      // children, and those children never redraw back because nothing marked THEM dirty).
      UI_ELEMENT* neighborelement = (UI_ELEMENT*)neighborarea->GetExtraData();
      if(neighborelement && GEN_USERINTERFACE.ModalLayer_IsRebuildProtected(neighborelement)) continue;
      if(neighborelement) MarkElementSubtreeDirty(neighborelement);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas(UI_LAYOUT* layout)
* @brief      Rebuild all areas
* @ingroup    USERINTERFACE
* 
* @param[in]  layout : Layout pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas(UI_LAYOUT* layout)
{ 
  if(!layout) return false;

  // The stored rebuild areas belong to the canvas, not to the XML layout order.
  // Restoring them by walking the layout tree can restore overlapping translucent
  // elements in the wrong order.  Use the global Z-level aware restoration instead.
  return RebuildAllAreas();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas(UI_ELEMENT* element)
* @brief      Rebuild all areas
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas(UI_ELEMENT* element)
{ 
  XDWORD max_z_level = 0;
  XDWORD nareas      = areas.GetSize();
  bool   first       = false;

  if(!nareas) return true;

  for(XDWORD c=0; c<nareas; c++)
    {
      GRP2DREBUILDAREA* area = areas.Get(c);
      if(area)
        {
          UI_ELEMENT* element = (UI_ELEMENT*)area->GetExtraData();
          if(element)
            {
              max_z_level =  __MAX(max_z_level, element->GetZLevel()); 
            }
        }
    }

  if(!max_z_level) return false;
  
  for(XDWORD level = max_z_level; level>0; level--)  
    {
      nareas = areas.GetSize();  

      for(int index = (int)nareas - 1; index >= 0; index--)                
        {
          GRP2DREBUILDAREA* area = areas.Get((XDWORD)index);
          if(area)
            {
              UI_ELEMENT* _element = (UI_ELEMENT*)area->GetExtraData();
              if(_element)
                {
                  if(_element == element)
                    {
                      if(_element->GetZLevel() == level)
                        {  
                          if(_element->MustReDraw() || (!_element->IsVisible())) 
                            {                              
                              GRPBITMAP* bitmap = area->GetBitmap();
                              if(bitmap) PutBitmapNoAlpha(area->GetXPos(), area->GetYPos(), bitmap);

                              areas.Delete(area);                
                              GEN_DELETE area;  

                              area = NULL;
                            }              
                        }
                    }
                }
            } 
        }
    } 

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
      if(subelement) RebuildAllAreas(subelement);            
    }

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea(double x, double y, double width, double height, UI_ELEMENT* element)
* @brief      Create rebuild area
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea(double x, double y, double width, double height, UI_ELEMENT* element)
{  
  return GRP2DREBUILDAREAS::CreateRebuildArea(x, y, width+1, height+1, (void*)element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_SKINCANVAS_REBUILDAREAS::GetBitmap(double x, double y, double width, double height)
* @brief      Get bitmap
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_SKINCANVAS_REBUILDAREAS::GetBitmap(double x, double y, double width, double height)
{
  if(!canvas) return NULL;

  return canvas->GetBitmap(x, y, width, height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS_REBUILDAREAS::PutBitmapNoAlpha(double x, double y, GRPBITMAP* bitmap)
* @brief      Put bitmap no alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  bitmap : Bitmap pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_REBUILDAREAS::PutBitmapNoAlpha(double x, double y, GRPBITMAP* bitmap)
{
  if(!canvas) return;

  canvas->PutBitmapNoAlpha(x, y, bitmap); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS_REBUILDAREAS::SetTargetCanvas(GRP2DCANVAS* newcanvas)
* @brief      Retarget rebuild-area GetBitmap/PutBitmapNoAlpha (modal offscreen composition).
* @ingroup    USERINTERFACE
* 
* @param[in]  newcanvas : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_REBUILDAREAS::SetTargetCanvas(GRP2DCANVAS* newcanvas)
{
  if(newcanvas) canvas = newcanvas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS* UI_SKINCANVAS_REBUILDAREAS::GetTargetCanvas()
* @brief      Canvas currently used by rebuild-area capture/restore.
* @ingroup    USERINTERFACE
* 
* @return     GRP2DCANVAS* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS* UI_SKINCANVAS_REBUILDAREAS::GetTargetCanvas()
{
  return canvas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS_REBUILDAREAS::GetRebuildAreaByElement(UI_ELEMENT* element)
* @brief      Get rebuild area by element
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     GRP2DREBUILDAREA* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS_REBUILDAREAS::GetRebuildAreaByElement(UI_ELEMENT* element)
{
  for(XDWORD c=0; c<GetRebuildAreas()->GetSize(); c++)
    {
      GRP2DREBUILDAREA* rebuild_area = GetRebuildAreas()->Get(c);
      if(rebuild_area)
        {
          UI_ELEMENT* _element = (UI_ELEMENT*)rebuild_area->GetExtraData();
          if(_element)
            {
              if(_element == element) return rebuild_area;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS_REBUILDAREAS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS_REBUILDAREAS::Clean()
{
  screen          = NULL;
  viewportindex   = 0;
  canvas          = NULL;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS::UI_SKINCANVAS(GRPSCREEN* screen, int viewportindex) : UI_SKIN(), UI_SKINCANVAS_REBUILDAREAS(screen)
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* @param[in]  screen : Screen object to use.
* @param[in]  viewportindex : Viewport index (default 0).
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS::UI_SKINCANVAS(GRPSCREEN* screen, int viewportindex) : UI_SKIN(),  UI_SKINCANVAS_REBUILDAREAS(screen)
{ 
  Clean();     

  this->screen        = screen;
  this->viewportindex = viewportindex;
  this->name          = UI_SKINCANVAS_NAME_UNKNOWN;
  this->drawmode      = UI_SKIN_DRAWMODE_CANVAS;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_SKINCANVAS::~UI_SKINCANVAS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_SKINCANVAS::~UI_SKINCANVAS()
{
  if(!fontpathfile.IsEmpty())
    {
      GEN_USERINTERFACE.DeleteTemporalUnZipFile(fontpathfile);
    }

  DeleteAllRebuildAreas();

  // Mirrors DeleteAllRebuildAreas() above but for our own, separate progressbackdrops cache (see UI_SkinCanvas.h
  // and Draw_ProgressBar()): GRP2DREBUILDAREA's destructor already frees its own captured GRPBITMAP, so
  // DeleteContents() correctly releases every cached backdrop bitmap here too.
  progressbackdrops.DeleteContents();
  progressbackdrops.DeleteAll();

  // Companion cache to progressbackdrops above (see UI_SkinCanvas.h and Draw_ProgressBar()): plain records,
  // no bitmaps to free, but still owned by this object and must be deleted here.
  progressbarlastbounds.DeleteContents();
  progressbarlastbounds.DeleteAll();

  // Mirrors progressbackdrops above but for the ALPHA-DARKENING FIX's own cache (see UI_SkinCanvas.h and
  // Draw_Form()): same reasoning, same cleanup.
  formbackdrops.DeleteContents();
  formbackdrops.DeleteAll();

  // Mirrors progressbackdrops above but for the RADIAL CAPTION GHOSTING FIX's own cache (see UI_SkinCanvas.h
  // and Draw_ProgressRadial()): same reasoning, same cleanup.
  radialbackdrops.DeleteContents();
  radialbackdrops.DeleteAll();

  // Mirrors progressbackdrops above but for the TEXT CAPTION GHOSTING FIX's own cache (see UI_SkinCanvas.h
  // and Draw_Text()): same reasoning, same cleanup.
  textbackdrops.DeleteContents();
  textbackdrops.DeleteAll();

  animbackdrops.DeleteContents();
  animbackdrops.DeleteAll();

  // Mirrors progressbackdrops above but for the HOVER-WASH GHOSTING FIX's own cache (see UI_SkinCanvas.h
  // and Draw_Option()): same reasoning, same cleanup.
  optionbackdrops.DeleteContents();
  optionbackdrops.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSCREEN* UI_SKINCANVAS::GetScreen()
* @brief      Get screen
* @ingroup    USERINTERFACE
* 
* @return     GRPSCREEN* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSCREEN* UI_SKINCANVAS::GetScreen()
{
  return screen;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCANVAS* UI_SKINCANVAS::GetCanvas()
* @brief      Get canvas
* @ingroup    USERINTERFACE
* 
* @return     GRP2DCANVAS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCANVAS* UI_SKINCANVAS::GetCanvas()
{
  if(canvas_override) return canvas_override;

  if(!screen)                   
    {
      return NULL;
    }

  if(!screen->GetViewport(viewportindex))   
    {
      return NULL;
    }

  return screen->GetViewport(viewportindex)->GetCanvas();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS::SetCanvasOverride(GRP2DCANVAS* override_canvas)
* @brief      Redirect GetCanvas() during modal offscreen composition (NULL = viewport canvas).
* @ingroup    USERINTERFACE
* 
* @param[in]  override_canvas : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::SetCanvasOverride(GRP2DCANVAS* override_canvas)
{
  canvas_override = override_canvas;
}
	

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::LoadFonts()
* @brief      Load fonts
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::LoadFonts()
{
  XPATH  xpath;
  bool   status = false; 

  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;
  
  if(!rasterfontname.IsEmpty())
    {
      status = canvas->RasterFont_Select(rasterfontname.Get());

    } else status = true;

  if(status)
    {
      if(!vectorfontname.IsEmpty())
        {          
          GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_FONTS, xpath);
          xpath.Slash_Add();
          xpath.Add(vectorfontname.Get());

          if(GEN_USERINTERFACE.IsZippedFile())
            {             
              XFILEUNZIP* unzipfile = GEN_USERINTERFACE.GetUnzipFile();
              if(unzipfile)
                {                  
                  XPATH pathnamefilecmp;

                  pathnamefilecmp = APPFLOW_DEFAULT_DIRECTORY_FONTS;
                  pathnamefilecmp.Slash_Add();
                  pathnamefilecmp += vectorfontname;

                  status = unzipfile->DecompressFile(pathnamefilecmp, (*GEN_USERINTERFACE.GetUnzipPathFile()), vectorfontname.Get());   
                  if(status)
                    {  
                      XPATH unzippathfile_tmp;

                      unzippathfile_tmp  = GEN_USERINTERFACE.GetUnzipPathFile()->Get();
                      unzippathfile_tmp += vectorfontname;

                      status = canvas->VectorFont_Load(unzippathfile_tmp);  

                      //GEN_USERINTERFACE.DeleteTemporalUnZipFile(unzippathfile_tmp);  

                      fontpathfile = unzippathfile_tmp;
                    }
                    
                }
            }
           else
            {                            
              status = canvas->VectorFont_Load(xpath);    
            }

          // Phase 0/1: skin@vectorfont is always the logical "default" face (compat).
          if(status)
            {
              XPATH* loaded = canvas->VectorFont_GetPathFile();
              if(loaded && !loaded->IsEmpty())
                {
                  canvas->VectorFont_Register(GRP2DCANVAS_VECTORFONT_DEFAULT, (*loaded));
                  canvas->VectorFont_Select(GRP2DCANVAS_VECTORFONT_DEFAULT);
                }
            }

        } else status = true;
    }

  // Phase 1/4: register optional <fonts> catalog faces by path only (lazy).
  // skin@vectorfont is loaded+selected above; catalog faces load on first VectorFont_Select.
  // An unused CJK/Noto entry is never opened. Failures on extras do not fail layout load.
  if(status)
    {
      for(XDWORD c=0; c<FontCatalog_GetSize(); c++)
        {
          UI_SKIN_FONT_ENTRY* entry = FontCatalog_Get(c);
          if(!entry) continue;
          if(entry->name.IsEmpty() || entry->filename.IsEmpty()) continue;

          // "default" in the catalog is informational; skin@vectorfont already owns that slot.
          if(!entry->name.Compare(GRP2DCANVAS_VECTORFONT_DEFAULT, true)) continue;

          XPATH facepath;
          bool  faceready = false;

          if(GEN_USERINTERFACE.IsZippedFile())
            {
              XFILEUNZIP* unzipfile = GEN_USERINTERFACE.GetUnzipFile();
              if(unzipfile)
                {
                  XPATH pathnamefilecmp;

                  pathnamefilecmp = APPFLOW_DEFAULT_DIRECTORY_FONTS;
                  pathnamefilecmp.Slash_Add();
                  pathnamefilecmp += entry->filename;

                  if(unzipfile->DecompressFile(pathnamefilecmp, (*GEN_USERINTERFACE.GetUnzipPathFile()), entry->filename.Get()))
                    {
                      facepath  = GEN_USERINTERFACE.GetUnzipPathFile()->Get();
                      facepath += entry->filename;
                      faceready = true;
                    }
                   else
                    {
                      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, NL("[UI Skin] font catalog '%s': cannot extract [%s] from bundle"),
                                        entry->name.Get(), entry->filename.Get());
                    }
                }
            }
           else
            {
              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_FONTS, facepath);
              facepath.Slash_Add();
              facepath.Add(entry->filename.Get());

              XFILE* check = GEN_XFACTORY.Create_File();
              if(check)
                {
                  faceready = check->Exist(facepath.Get());
                  if(!faceready)
                    {
                      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, NL("[UI Skin] font catalog '%s': file missing [%s]"),
                                        entry->name.Get(), facepath.Get());
                    }
                  GEN_XFACTORY.Delete_File(check);
                }
            }

          if(faceready) canvas->VectorFont_Register(entry->name.Get(), facepath);
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_SKINCANVAS_ResolveMaxSize(UI_ELEMENT* element, double fathersize)
* @brief      INTERNAL: corrects the one-pixel-short father size GetFatherSize() reports for a ROOT (fatherless)
*             element, but ONLY at the point a width="max" / height="max" resolution consumes it -- never for
*             GetFatherSize()'s other consumer, CalculePosition()'s xpos="right"/"center" and
*             ypos="up"/"down"/"center" resolution, which is left untouched on purpose.
* @note       GetFatherSize() reports "canvas size - 1" as a root element's father size, a "last valid pixel
*             index" convention (the same one SetClipBox(0,0,width-1,height-1) uses for the canvas' own clip
*             box). That is a defensible convention for a COORDINATE, but every CalculateBoundaryLine_X()
*             function also feeds the very same number into a "max" SIZE resolution, where it means something
*             different: "fill the father edge-to-edge". A root form with width="max" on a 1440 px canvas was
*             resolving to 1439 -- one column short of the window's own right edge -- while the exact same
*             form with an explicit width="1440" filled it completely. That mismatch is what left a sliver of
*             unpainted canvas behind a custom window chrome's caption bar when its width was left to default.
*             This helper closes that ONE pixel gap exactly where it is created (the "max" case), and only for
*             root elements (a child's father size never carries GetFatherSize()'s -1: it comes straight from
*             the father's own already-resolved UI_BOUNDARYLINE). Every other consumer of GetFatherSize()'s
*             output -- most notably CalculePosition(), called right after with the SAME fatherwidth/fatherheight
*             local variables this function does not touch -- keeps its pre-existing arithmetic byte for byte,
*             so no existing layout's positioning shifts because of this fix.
* @ingroup    USERINTERFACE
*
* @param[in]  element : the element being resolved (only element->GetFather() is inspected).
* @param[in]  fathersize : the father width or height as returned by GetFatherSize().
*
* @return     double : fathersize unchanged for a child element; fathersize + 1 for a root element.
*
* --------------------------------------------------------------------------------------------------------------------*/
static double UI_SKINCANVAS_ResolveMaxSize(UI_ELEMENT* element, double fathersize)
{
  return element->GetFather() ? fathersize : (fathersize + 1.0);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::GetFatherSize(UI_ELEMENT* element, double& width, double& height)
* @brief      Get father size
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::GetFatherSize(UI_ELEMENT* element, double& width, double& height)
{
  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;

  width  = 0.0f;
  height = 0.0f;
  
  if(element->GetFather())  
    {
      width  = element->GetFather()->GetBoundaryLine()->width; 
      height = element->GetFather()->GetBoundaryLine()->height; 
    }
   else  
    { 
      width  = canvas->GetWidth()-1;   
      height = canvas->GetHeight()-1;   
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKINCANVAS::GetPositionWithoutDefine(double position)
* @brief      Get position without define
* @ingroup    USERINTERFACE
* 
* @param[in]  position : Position value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKINCANVAS::GetPositionWithoutDefine(double position)
{ 
  switch((int)position)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT     : break;
      case UI_ELEMENT_TYPE_ALIGN_RIGHT		: break;
      case UI_ELEMENT_TYPE_ALIGN_UP       : break;
      case UI_ELEMENT_TYPE_ALIGN_DOWN   	: break;
      case UI_ELEMENT_TYPE_ALIGN_CENTER		: break;
                        default       : return position;
    } 
    
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::SetAroundFromSubElements(UI_ELEMENT* element)
* @brief      Set around from sub elements
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::SetAroundFromSubElements(UI_ELEMENT* element)
{
  if(!element) return false;
  if(element->GetComposeElements()->IsEmpty()) return false;

  UI_BOUNDARYLINE bondaryline[2];

  bondaryline[1].x      = element->GetXPosition();
  bondaryline[1].y      = element->GetYPosition();
  bondaryline[1].width  = element->GetBoundaryLine()->width;
  bondaryline[1].height = element->GetBoundaryLine()->height;

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement)
        {
          bondaryline[0].x       = subelement->GetXPosition();
          bondaryline[0].y       = subelement->GetYPosition();
          bondaryline[0].width   = subelement->GetBoundaryLine()->width;
          bondaryline[0].height  = subelement->GetBoundaryLine()->height;

          bondaryline[1].SetAround(bondaryline[0]);
        }
    }


  for(int c=(int)element->GetComposeElements()->GetSize()-1; c>=0; c--)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(subelement)
        {
          bondaryline[0].x       = subelement->GetXPosition();
          bondaryline[0].y       = subelement->GetYPosition();
          bondaryline[0].width   = subelement->GetBoundaryLine()->width;
          bondaryline[0].height  = subelement->GetBoundaryLine()->height;

          bondaryline[1].SetAround(bondaryline[0]);
        }
    }
 
  element->SetXPosition(bondaryline[1].x);
  element->SetYPosition(bondaryline[1].y);
  element->GetBoundaryLine()->width  = bondaryline[1].width;
  element->GetBoundaryLine()->height = bondaryline[1].height;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::AddPositionSubElements(UI_ELEMENT* element)
* @brief      Add position sub elements
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::AddPositionSubElements(UI_ELEMENT* element)
{
  if(!element) return false;   

  for(XDWORD d=0; d<element->GetComposeElements()->GetSize(); d++)   
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(d);
      if(subelement) 
        {
          subelement->SetXPosition(subelement->GetXPosition() + element->GetXPosition()); 
          subelement->SetYPosition(subelement->GetYPosition() + element->GetYPosition()); 
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculePosition(UI_ELEMENT* element, double fatherwidth, double fatherheight, bool adjustsizemargin)
* @brief      Calcule position
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  fatherwidth : Fatherwidth value.
* @param[in]  fatherheight : Fatherheight value.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculePosition(UI_ELEMENT* element, double fatherwidth, double fatherheight, bool adjustsizemargin)
{ 
  if(!element) 
    {
      return false;
    }

  double x_position = 0.0f;
  double y_position = 0.0f;

  if(element->GetFather())
    {
      x_position = GetPositionWithoutDefine(element->GetFather()->GetXPosition());
      y_position = GetPositionWithoutDefine(element->GetFather()->GetYPosition());     
    }

  if(adjustsizemargin)
    {
      element->GetBoundaryLine()->width  -= (element->GetMargin(UI_ELEMENT_TYPE_ALIGN_LEFT) + element->GetMargin(UI_ELEMENT_TYPE_ALIGN_RIGHT));
      element->GetBoundaryLine()->height -= (element->GetMargin(UI_ELEMENT_TYPE_ALIGN_UP)   + element->GetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN));
    }


  switch((int)element->GetBoundaryLine()->x)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT     : break;

      case UI_ELEMENT_TYPE_ALIGN_RIGHT		: x_position += (fatherwidth - element->GetBoundaryLine()->width);                  
                                            break;

      case UI_ELEMENT_TYPE_ALIGN_CENTER   : x_position += (int)round((fatherwidth - element->GetBoundaryLine()->width)/2);                                               
                                            break;

                                default   : x_position += GetPositionWithoutDefine(element->GetBoundaryLine()->x);            
                                            break;
    }

  if(element->GetFather())
    {
      switch((int)element->GetBoundaryLine()->y)
        {
          case UI_ELEMENT_TYPE_ALIGN_UP       : y_position -= fatherheight - element->GetBoundaryLine()->height;                 
                                                break;

          case UI_ELEMENT_TYPE_ALIGN_DOWN   	: break;

          case UI_ELEMENT_TYPE_ALIGN_CENTER		: y_position -= (int)round((fatherheight - element->GetBoundaryLine()->height)/2);  
                                                break;

                                    default   : y_position -= GetPositionWithoutDefine(element->GetBoundaryLine()->y);            
                                                break;
        }
     
    }
   else 
    {
      switch((int)element->GetBoundaryLine()->y)
        {
          case UI_ELEMENT_TYPE_ALIGN_UP       : y_position += element->GetBoundaryLine()->height;                                     
                                                break;

          case UI_ELEMENT_TYPE_ALIGN_DOWN   	: y_position += fatherheight;                                                        
                                                break;

          case UI_ELEMENT_TYPE_ALIGN_CENTER		: y_position += (int)round((fatherheight + element->GetBoundaryLine()->height)/2);  
                                                break;

                                    default   : y_position += GetPositionWithoutDefine(element->GetBoundaryLine()->y);            
                                                break;
        }
    } 

  x_position += element->GetMargin(UI_ELEMENT_TYPE_ALIGN_LEFT);
  x_position -= element->GetMargin(UI_ELEMENT_TYPE_ALIGN_RIGHT);

  if(element->GetFather())
    {
      y_position += element->GetMargin(UI_ELEMENT_TYPE_ALIGN_UP);
      y_position -= element->GetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN);
    }
   else
    {
      y_position -= element->GetMargin(UI_ELEMENT_TYPE_ALIGN_UP);
      y_position += element->GetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN);
    }

  // --- Step 4: father padding delta -----------------------------------------------------------------------------
  // Apply the father's padding as a delta on top of the alignment already resolved above. The delta depends on
  // WHICH alignment / numeric mode the child uses, because padding shifts the reference edge differently for
  // each case (LEFT-aligned or numeric-x shifts right by pad_L; RIGHT-aligned shifts left by pad_R; CENTER by
  // half the padding differential). Zero padding leaves this a full no-op, preserving the pre-step-4 geometry
  // of every layout that does not set the property.
  if(element->GetFather())
    {
      UI_ELEMENT* father = element->GetFather();

      double pad_L = father->GetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT);
      double pad_R = father->GetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT);
      double pad_T = father->GetPadding(UI_ELEMENT_TYPE_ALIGN_UP);
      double pad_B = father->GetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN);

      if(pad_L != 0.0 || pad_R != 0.0 || pad_T != 0.0 || pad_B != 0.0)
        {
          switch((int)element->GetBoundaryLine()->x)
            {
              case UI_ELEMENT_TYPE_ALIGN_LEFT   : x_position += pad_L;                                     break;
              case UI_ELEMENT_TYPE_ALIGN_RIGHT  : x_position -= pad_R;                                     break;
              case UI_ELEMENT_TYPE_ALIGN_CENTER : x_position += (int)round((pad_L - pad_R) / 2.0);         break;
                                        default : x_position += pad_L;                                     break;
            }

          // Y is bottom-anchored in GEN's internal storage: increasing y moves DOWN on screen. Padding shifts
          // the interior origin the same way it does in CSS, but the sign flips for numeric / DOWN cases
          // because they measure "distance from the bottom edge".
          switch((int)element->GetBoundaryLine()->y)
            {
              case UI_ELEMENT_TYPE_ALIGN_UP     : y_position += pad_T;                                     break;
              case UI_ELEMENT_TYPE_ALIGN_DOWN   : y_position -= pad_B;                                     break;
              case UI_ELEMENT_TYPE_ALIGN_CENTER : y_position += (int)round((pad_T - pad_B) / 2.0);         break;
                                        default : y_position -= pad_B;                                     break;
            }
        }
    }

  element->SetXPosition(x_position);
  element->SetYPosition(y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKINCANVAS::GetWidthString(XCHAR* string, XDWORD sizefont)
* @brief      Get with string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKINCANVAS::GetWidthString(XCHAR* string, XDWORD sizefont)
{
  XDWORD width  = 0;
  XDWORD height = 0;

  SetFontSize(sizefont);
  GetFontSize(string, width, height);

  return (double)width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKINCANVAS::GetHeightString(XCHAR* string, XDWORD sizefont)
* @brief      Get height string
* @ingroup    USERINTERFACE
* 
* @param[in]  string : String to use.
* @param[in]  sizefont : Sizefont value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double  UI_SKINCANVAS::GetHeightString(XCHAR* string, XDWORD sizefont)
{
  XDWORD width  = 0;
  XDWORD height = 0;

  SetFontSize(sizefont);
  GetFontSize(string, width, height);

  return (double)height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Scroll(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Scroll(UI_ELEMENT* element, bool adjustsizemargin)
{
  return true;
}


// Phase 4: one-shot warning when a resolved logical face is unknown / unloadable (no crash; fallback default).
static void UI_SKINCANVAS_WarnUnknownFaceOnce(XCHAR* facename, XCHAR* elementname)
{
  if(!facename || !facename[0]) return;
  if(!XSTRING::Compare(facename, GRP2DCANVAS_VECTORFONT_DEFAULT, true)) return;

  static XVECTOR<XSTRING*> warned;
  for(XDWORD c=0; c<warned.GetSize(); c++)
    {
      XSTRING* entry = warned.Get(c);
      if(entry && !entry->Compare(facename, true)) return;
    }

  XSTRING* copy = GEN_NEW XSTRING();
  if(copy)
    {
      copy->Set(facename);
      warned.Add(copy);
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING,
                    NL("[UI Skin] unknown vector face '%s' (element '%s'); using default"),
                    facename,
                    (elementname && elementname[0]) ? elementname : NL("?"));
}


// Phase 2/3/4: single AGG FreeType face — Select for measure/draw, restore previous after.
// Priority: explicit font-family/fontface → skin default.
// Language/CJK faces are NOT selected here as the sole face: Noto SC also covers ASCII digits and
// symbols, which would otherwise steal Nunito metrics in Chinese. CJK is a PrintStack/GetWidthStack
// fallback only (see BuildTextFaceStack).
// Catalog faces are lazy: VectorFont_Register at LoadFonts; VectorFont_Load runs on first successful Select.
static void UI_SKINCANVAS_SelectResolvedFace(UI_SKIN* skin, GRP2DCANVAS* canvas, XCHAR* explicitface, XSTRING* masktext, XCHAR* elementname, XSTRING& previous)
{
  previous.Empty();
  if(!canvas) return;

  (void)skin;
  (void)masktext;

  XSTRING* selected = canvas->VectorFont_GetSelected();
  if(selected && !selected->IsEmpty()) previous = selected->Get();

  XSTRING resolved;

  if(explicitface && explicitface[0])
    {
      resolved = explicitface;
    }
   else
    {
      resolved = GRP2DCANVAS_VECTORFONT_DEFAULT;
    }

  if(!canvas->VectorFont_Select(resolved.Get()))
    {
      UI_SKINCANVAS_WarnUnknownFaceOnce(resolved.Get(), elementname);
      canvas->VectorFont_Select(GRP2DCANVAS_VECTORFONT_DEFAULT);
    }
}


static void UI_SKINCANVAS_SelectTextFace(UI_SKIN* skin, GRP2DCANVAS* canvas, UI_ELEMENT_TEXT* element_text, XSTRING& previous)
{
  XCHAR* explicitface = NULL;
  XSTRING* masktext   = NULL;
  XCHAR* elementname  = NULL;

  if(element_text)
    {
      if(element_text->GetFontFace() && !element_text->GetFontFace()->IsEmpty())
        {
          explicitface = element_text->GetFontFace()->Get();
        }
      masktext = element_text->GetMaskText();
      if(element_text->GetName()) elementname = element_text->GetName()->Get();
    }

  UI_SKINCANVAS_SelectResolvedFace(skin, canvas, explicitface, masktext, elementname, previous);
}


// Phase 5: build ordered face pointers for GetWidthStack/PrintStack.
// Order: authored stack (or single face) → default → language face (CJK fallback, if mapped).
// Default before language so ASCII digits, IPs, versions and symbols keep Nunito like other
// languages; Noto/CJK only paints glyphs the Latin face does not have.
static XDWORD UI_SKINCANVAS_BuildTextFaceStack(UI_SKIN* skin, UI_ELEMENT_TEXT* element_text, XCHAR** outfaces, XDWORD maxfaces, XSTRING& langstorage)
{
  XDWORD n = 0;
  if(!outfaces || !maxfaces) return 0;

  langstorage.Empty();

  if(element_text)
    {
      XDWORD stackn = element_text->GetFontFaceStackCount();
      if(stackn)
        {
          for(XDWORD i=0; i<stackn && n<maxfaces; i++)
            {
              XSTRING* entry = element_text->GetFontFaceStackEntry(i);
              if(entry && !entry->IsEmpty()) outfaces[n++] = entry->Get();
            }
        }
       else if(element_text->GetFontFace() && !element_text->GetFontFace()->IsEmpty())
        {
          outfaces[n++] = element_text->GetFontFace()->Get();
        }
    }

  bool hasdefault = false;
  for(XDWORD i=0; i<n; i++)
    {
      if(outfaces[i] && !XSTRING::Compare(outfaces[i], GRP2DCANVAS_VECTORFONT_DEFAULT, true))
        {
          hasdefault = true;
          break;
        }
    }
  if(!hasdefault && n<maxfaces) outfaces[n++] = GRP2DCANVAS_VECTORFONT_DEFAULT;

  if(skin && GEN_XTRANSLATION.IsInitialized())
    {
      if(skin->FontForLanguage(GEN_XTRANSLATION.GetActual(), langstorage) && !langstorage.IsEmpty())
        {
          bool listed = false;
          for(XDWORD i=0; i<n; i++)
            {
              if(outfaces[i] && !XSTRING::Compare(outfaces[i], langstorage.Get(), true))
                {
                  listed = true;
                  break;
                }
            }
          if(!listed && n<maxfaces) outfaces[n++] = langstorage.Get();
        }
    }

  return n;
}


static void UI_SKINCANVAS_SelectTextBoxFace(UI_SKIN* skin, GRP2DCANVAS* canvas, UI_ELEMENT_TEXTBOX* element_textbox, XSTRING& previous)
{
  XSTRING* masktext  = NULL;
  XCHAR* elementname = NULL;

  if(element_textbox)
    {
      masktext = element_textbox->GetMaskText();
      if(element_textbox->GetName()) elementname = element_textbox->GetName()->Get();
    }

  // TextBox has no Phase-2 fontface field yet; measure uses default (CJK via PrintStack fallback).
  UI_SKINCANVAS_SelectResolvedFace(skin, canvas, NULL, masktext, elementname, previous);
}

static void UI_SKINCANVAS_RestoreTextFace(GRP2DCANVAS* canvas, XSTRING& previous)
{
  if(!canvas) return;

  if(!previous.IsEmpty()) canvas->VectorFont_Select(previous.Get());
   else                    canvas->VectorFont_Select(GRP2DCANVAS_VECTORFONT_DEFAULT);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Text(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Text(UI_ELEMENT* element, bool adjustsizemargin)
{
  if(!element) return false;

  UI_ELEMENT_TEXT* element_text = (UI_ELEMENT_TEXT*)element;
  GRP2DCANVAS*       canvas       = GetCanvas();

  if(!canvas) return false;

  canvas->Vectorfont_GetConfig()->SetSize(element_text->GetSizeFont());

  XSTRING text;
  if(!element_text->GetText()->IsEmpty()) 
    {
      text = element_text->GetText()->Get();    
    }
   else 
    {
      if(!element_text->GetMaskText()->IsEmpty()) 
        {
          text = element_text->GetMaskText()->Get();
        }
    }

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  XSTRING string_max = text;
  if(element_text->GetMaxSizeText())
    {
      XDWORD size_max = element_text->GetMaxSizeText();     
      
      if(!size_max) size_max = text.GetSize();
      string_max.Empty();
      for(XDWORD c=0; c<size_max; c++)
        {
          string_max += NL("A");
        }
    }

  XCHAR*  faces[GRP2DCANVAS_VECTORFONT_STACK_MAX];
  XSTRING langface;
  XDWORD  nfaces = UI_SKINCANVAS_BuildTextFaceStack(this, element_text, faces, GRP2DCANVAS_VECTORFONT_STACK_MAX, langface);

  // Warn once for authored faces that cannot be selected (Phase 4 telemetry via stack walk).
  for(XDWORD f=0; f<nfaces; f++)
    {
      if(!faces[f]) continue;
      if(!XSTRING::Compare(faces[f], GRP2DCANVAS_VECTORFONT_DEFAULT, true)) continue;
      if(!canvas->VectorFont_Select(faces[f]))
        {
          UI_SKINCANVAS_WarnUnknownFaceOnce(faces[f], element_text->GetName() ? element_text->GetName()->Get() : NL("?"));
        }
    }
  canvas->VectorFont_Select(GRP2DCANVAS_VECTORFONT_DEFAULT);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	 :		 		
      case                           0 : element->GetBoundaryLine()->width  = canvas->VectorFont_GetWidthStack(faces, nfaces, string_max.Get());    break;
      case UI_ELEMENT_TYPE_ALIGN_MAX   : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                               break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	 :		 		
      case                           0 : { XSTRING previousface;
                                           UI_SKINCANVAS_SelectTextFace(this, canvas, element_text, previousface);
                                           element->GetBoundaryLine()->height  = (double)canvas->VectorFont_GetHeight(string_max.Get());
                                           UI_SKINCANVAS_RestoreTextFace(canvas, previousface);
                                         }
                                         break;
      case UI_ELEMENT_TYPE_ALIGN_MAX   : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                            break;	    
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_TextBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line text box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_TextBox(UI_ELEMENT* element, bool adjustsizemargin)
{  
  UI_ELEMENT_TEXTBOX* element_textbox = (UI_ELEMENT_TEXTBOX*)element;
  if(!element_textbox) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
      case                            0 : 
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false; 
                                          break;
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	  :		 		
      case                            0 : 
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false; 
                                          break;
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);  
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Console(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line console
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Console(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_CONSOLE* element_console = (UI_ELEMENT_CONSOLE*)element;
  if(!element_console) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_PropertyList(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line property list (keeps XML width / height, like console)
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_PropertyList(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_PROPERTYLIST* element_propertylist = (UI_ELEMENT_PROPERTYLIST*)element;
  if(!element_propertylist) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Tree(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line tree (keeps XML width / height, like console)
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Tree(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_TREE* element_tree = (UI_ELEMENT_TREE*)element;
  if(!element_tree) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                            0 :
      case UI_ELEMENT_TYPE_ALIGN_MAX    : return false;
                                          break;
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Image(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Image(UI_ELEMENT* element, bool adjustsizemargin)
{
  if(!element) return false;
  
  UI_ELEMENT_IMAGE* element_image = (UI_ELEMENT_IMAGE*)element;
  if(!element_image->GetImage()) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
      case                            0 : element->GetBoundaryLine()->width  = (double)element_image->GetImage()->GetWidth();       break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                          break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	  :		 		
      case                            0 : element->GetBoundaryLine()->height  = (double)element_image->GetImage()->GetHeight();     break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       break;	    
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Animation(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line animation
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Animation(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_ANIMATION* element_animation = (UI_ELEMENT_ANIMATION*)element;
  if(!element_animation) return false;  

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO     :		 		
      case                             0  : { double maximgwidth = 0.0f; 

                                              for(XDWORD c=0; c<element_animation->GetComposeElements()->GetSize(); c++)
                                                {
                                                 UI_ELEMENT_IMAGE* element_image = (UI_ELEMENT_IMAGE*)element_animation->GetComposeElements()->Get(c);
                                                 if(element_image) maximgwidth = __MAX((GetPositionWithoutDefine(element_image->GetBoundaryLine()->x) + element_image->GetBoundaryLine()->width), maximgwidth);                                                    
                                                }

                                              element->GetBoundaryLine()->width = maximgwidth;
                                            }
                                            break;

      case UI_ELEMENT_TYPE_ALIGN_MAX      : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                          
                                            break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO     :		 		
      case                             0  : { double maximgheight = 0.0f; 

                                              for(XDWORD c=0; c<element_animation->GetComposeElements()->GetSize(); c++)
                                                {
                                                 UI_ELEMENT_IMAGE* element_image = (UI_ELEMENT_IMAGE*)element_animation->GetComposeElements()->Get(c);
                                                 if(element_image) maximgheight = __MAX((GetPositionWithoutDefine(element_image->GetBoundaryLine()->y) + element_image->GetBoundaryLine()->height), maximgheight);                                                    
                                                }

                                              element->GetBoundaryLine()->height = maximgheight;
                                            }
                                            break;

      case UI_ELEMENT_TYPE_ALIGN_MAX      : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       
                                            break;	    
    }

  CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);

  for(XDWORD c=0; c<element_animation->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT_IMAGE* element_image = (UI_ELEMENT_IMAGE*)element_animation->GetComposeElements()->Get(c);
      if(element_image)
        {
          CalculePosition(element_image, element_animation->GetBoundaryLine()->width, element_image->GetBoundaryLine()->height, adjustsizemargin);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Option(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Option(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_OPTION* element_option = (UI_ELEMENT_OPTION*)element;
  if(!element_option) return false;  

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  UI_ELEMENT_ANIMATION* element_animation = (UI_ELEMENT_ANIMATION*)element_option->Get_UIAnimation();
  UI_ELEMENT_TEXT*      element_text      = (UI_ELEMENT_TEXT*)element_option->Get_UIText();

  // Explicit XML width/height (e.g. gal_* 208x28) must stay the wash/hit box. SetAroundFromSubElements
  // unions compose children in absolute coords; an extra TEXT (gallery date) that was CalculePosition'd
  // while the father button was still unpositioned sits near y≈0 and inflates height to hundreds of px.
  // DrawBackgroundColor then paints that huge opaque rect (SelectExclusiveRow white/gray alpha 255).
  const int raw_w = (int)element->GetBoundaryLine()->width;
  const int raw_h = (int)element->GetBoundaryLine()->height;
  const bool explicit_box = (raw_w != UI_ELEMENT_TYPE_ALIGN_AUTO) && (raw_w != 0) && (raw_w != UI_ELEMENT_TYPE_ALIGN_MAX) &&
                            (raw_h != UI_ELEMENT_TYPE_ALIGN_AUTO) && (raw_h != 0) && (raw_h != UI_ELEMENT_TYPE_ALIGN_MAX);

  double maximgwidth  = 0.0f; 
  double maximgheight = 0.0f; 

  if(element_animation || element_text)
    {
      switch((int)element->GetBoundaryLine()->width)
        {
          case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
          case                           0  : { switch(element_option->GetAllocationTextType())
                                                  {                                            
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT   : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT    : if(element_animation) maximgwidth += element_animation->GetBoundaryLine()->width;  
                                                                                                          if(element_text)      maximgwidth += element_text->GetBoundaryLine()->width;  
                                                                                                          break;

                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    :                                                                  
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : 
                                                                                              default   : if(element_animation) maximgwidth = __MAX(maximgwidth, element_animation->GetBoundaryLine()->width);                                                                                                    
                                                                                                          if(element_text)      maximgwidth = __MAX(maximgwidth, element_text->GetBoundaryLine()->width);                                                                                                  
                                                                                                          break; 
                                                  } 
                                     
                                                element->GetBoundaryLine()->width = maximgwidth;
                                              }
                                              break;

          case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                          
                                              break;	    
        }

      switch((int)element->GetBoundaryLine()->height)
        {
          case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
          case                           0  : { switch(element_option->GetAllocationTextType())
                                                  {                                           
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP      : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN    : if(element_animation) maximgheight += element_animation->GetBoundaryLine()->height;  
                                                                                                          if(element_text)      maximgheight += element_text->GetBoundaryLine()->height;
                                                                                                          // Gap between face and caption so soft-alpha icon rims do not sit
                                                                                                          // inside the text AABB (XTraceMonitorGEN legend gray bars).
                                                                                                          if(element_animation && element_text) maximgheight += 4.0;
                                                                                                          break;

                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : 
                                                                                              default   : if(element_animation) maximgheight = __MAX(maximgheight, element_animation->GetBoundaryLine()->height);                                                                                                    
                                                                                                          if(element_text)      maximgheight = __MAX(maximgheight, element_text->GetBoundaryLine()->height);                                                                                                  
                                                                                                          break; 
                                                  }                                      
                                                element->GetBoundaryLine()->height = maximgheight;
                                              }
                                              break;

          case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       
                                              break;	    
        }
    }
  
  CalculePosition(element_option, fatherwidth, fatherheight, adjustsizemargin);

  if(element_animation) CalculePosition(element_animation, element_option->GetBoundaryLine()->width, element_option->GetBoundaryLine()->height, adjustsizemargin);
  if(element_text)      CalculePosition(element_text, element_option->GetBoundaryLine()->width, element_option->GetBoundaryLine()->height, adjustsizemargin);  

  switch(element_option->GetAllocationTextType())
    {
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    : break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN    : if(element_animation && element_text) element_animation->SetYPosition(element_animation->GetYPosition() - element_text->GetBoundaryLine()->height - 4.0);     break;
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP      : if(element_animation && element_text) element_text->SetYPosition(element_text->GetYPosition() - element_animation->GetBoundaryLine()->height - 4.0);          break;                                                          
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT   : if(element_animation && element_text) element_text->SetXPosition(element_text->GetXPosition() + element_animation->GetBoundaryLine()->width);           break;
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT    : if(element_animation && element_text) element_animation->SetXPosition(element_animation->GetXPosition() + element_text->GetBoundaryLine()->width);      break;
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : break; 
    } 

  if(element_animation)
    {
      for(XDWORD c=0; c<element_animation->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* subelement = element_animation->GetComposeElements()->Get(c);
          if(subelement) CalculePosition(subelement, element_animation->GetBoundaryLine()->width, element_animation->GetBoundaryLine()->height, adjustsizemargin);         
        } 
    }

  // Extra compose TEXTs (e.g. nested gallery dates) are not Get_UIText() — reposition them now that
  // the button has its final absolute box so Draw_Option and (optional) SetAround see correct coords.
  for(XDWORD c=0; c<element_option->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* sub = element_option->GetComposeElements()->Get(c);
      if(!sub) continue;
      if(sub == (UI_ELEMENT*)element_animation) continue;
      if(sub == (UI_ELEMENT*)element_text)      continue;
      CalculePosition(sub, element_option->GetBoundaryLine()->width, element_option->GetBoundaryLine()->height, adjustsizemargin);
    }

  // AUTO-sized options still grow to content; fixed boxes keep authored wash geometry (208x28).
  if(!explicit_box) SetAroundFromSubElements(element_option);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_MultiOption(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line multi option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_MultiOption(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_MULTIOPTION* element_multioption = (UI_ELEMENT_MULTIOPTION*)element;
  if(!element_multioption) return false;   

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
      case                          0   : { double maximgwidth = 0.0f; 

                                            for(XDWORD c=0; c<element_multioption->GetComposeElements()->GetSize(); c++)
                                              {
                                               UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
                                               if(subelement) maximgwidth = __MAX((GetPositionWithoutDefine(subelement->GetBoundaryLine()->x) + subelement->GetBoundaryLine()->width), maximgwidth);                                                    
                                              }

                                            element->GetBoundaryLine()->width = maximgwidth;
                                          }
                                          break;

      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                          
                                          break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	  :		 		
      case                          0   : { double maximgheight = 0.0f; 

                                            for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
                                              {
                                               UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
                                               if(subelement) maximgheight = __MAX((GetPositionWithoutDefine(subelement->GetBoundaryLine()->y) + subelement->GetBoundaryLine()->height), maximgheight);                                                    
                                              }

                                            element->GetBoundaryLine()->height = maximgheight;
                                          }
                                          break;

      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       
                                          break;	    
    }

  CalculePosition(element_multioption, fatherwidth, fatherheight, adjustsizemargin);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Button(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line button
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Button(UI_ELEMENT* element, bool adjustsizemargin)
{
  return CalculateBoundaryLine_Option(element, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_CheckBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line check box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_CheckBox(UI_ELEMENT* element, bool adjustsizemargin)
{
  return CalculateBoundaryLine_Option(element, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_EditText(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line edit text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_EditText(UI_ELEMENT* element, bool adjustsizemargin)
{  
  if(!element) return false;

  UI_ELEMENT_EDITTEXT* element_edittext = (UI_ELEMENT_EDITTEXT*)element;
  GRP2DCANVAS*           canvas           = GetCanvas();

  if(!canvas) return false;
  
  canvas->Vectorfont_GetConfig()->SetSize(element_edittext->GetSizeFont());

  XSTRING text;
  if(!element_edittext->GetText()->IsEmpty()) text = element_edittext->GetText()->Get();
    else if(!element_edittext->GetMaskText()->IsEmpty()) text = element_edittext->GetMaskText()->Get();

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  XSTRING string_max;
  XDWORD  size_max   =  element_edittext->GetMaxSizeText();
  
  if(!size_max) size_max = text.GetSize();
  for(XDWORD c=0; c<size_max; c++)
    {
      string_max += NL("A");
    }

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO :		 		
      case                          0 : element->GetBoundaryLine()->width  = (double)canvas->VectorFont_GetWidth(string_max.Get());     break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                                break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO :		 		
      case                          0 : element->GetBoundaryLine()->height  = (double)canvas->VectorFont_GetHeight(string_max.Get());   break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                             break;	    
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Form(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line form
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Form(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_FORM* element_form = (UI_ELEMENT_FORM*)element;
  if(!element_form) return false;   

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  // --- Custom window chrome: the caption bar spans the whole window, always ---------------------------------------
  //
  // A form carrying role="caption" is the background bar of a GEN custom window chrome (see GRPSCREEN's
  // CFG Chromes). Its width is not a design decision of the layout: it IS the width of the window the chrome
  // is applied to. Resolving it here -- instead of making every chrome .xml hardcode a number -- is what lets
  // the very same chrome bundle (defaultwindowschromes.zip) be reused by applications of different sizes
  // (WindowChromes 1024, UI_System 1440, …).
  //
  // Without this, a caption authored with no "width" attribute falls into the generic AUTO/0 case below, which
  // shrink-wraps the form around its own children. The bar then collapses to roughly the width of the title
  // text, and every child positioned with xpos="right" (minimize / maximize / close) resolves against that
  // collapsed width and lands on top of the icon -- the "only the caption icon shows" symptom.
  //
  // ALWAYS override an authored numeric width too (e.g. legacy width="1024" in defaultwindowschromes.xml):
  // UpdateCFGChromesButtonsPosition() can stretch the caption AABB later, but the min/max/close buttons are
  // laid out once against the load-time father width. If that father stayed at 1024 while the app canvas is
  // 1440, xpos="right" pins them to the wrong edge until/unless the shift path runs with a matching client
  // size -- which fails as a universal default. Forcing canvas width here makes right-aligned chrome controls
  // resolve correctly for every app size on first layout.
  //
  // The value used is the FULL canvas width, not GetFatherSize()'s canvas width - 1: that -1 is the generic
  // "maximum valid coordinate" convention for width="max", and one pixel of bare canvas at the right end of
  // the caption bar is visible. A caption nested inside another element (unusual, but legal) simply takes its
  // father's width.
  if(element->GetChromeRole() == UI_ELEMENT_CHROMEROLE_CAPTION)
    {
      double captionwidth = fatherwidth;

      if(!element->GetFather())
        {
          GRP2DCANVAS* canvas = GetCanvas();
          if(canvas) captionwidth = (double)canvas->GetWidth();
        }

      element->GetBoundaryLine()->width = captionwidth;
    }

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	  :
      case                           0  : { double maximgwidth = 0.0f;

                                            for(XDWORD c=0; c<element_form->GetComposeElements()->GetSize(); c++)
                                              {
                                               UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
                                               if(subelement) maximgwidth = __MAX((GetPositionWithoutDefine(subelement->GetBoundaryLine()->x) + subelement->GetBoundaryLine()->width), maximgwidth);
                                              }

                                            element->GetBoundaryLine()->width = maximgwidth;
                                          }
                                          break;

      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);
                                          break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	  :		 		
      case                          0   : { double maximgheight = 0.0f; 

                                            for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
                                              {
                                               UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
                                               if(subelement) maximgheight = __MAX((GetPositionWithoutDefine(subelement->GetBoundaryLine()->y) + subelement->GetBoundaryLine()->height), maximgheight);                                                    
                                              }

                                            element->GetBoundaryLine()->height = maximgheight;
                                          }
                                          break;

      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       
                                          break;	    
    }

  CalculePosition(element_form, fatherwidth, fatherheight, adjustsizemargin);
  
  AddPositionSubElements(element_form);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_Menu(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line menu
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_Menu(UI_ELEMENT* element, bool adjustsizemargin)
{  
  return CalculateBoundaryLine_Form(element, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_ListBox(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line list box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_ListBox(UI_ELEMENT* element, bool adjustsizemargin)
{  
  if(!element) return false;

  UI_ELEMENT_LISTBOX* element_listbox  = (UI_ELEMENT_LISTBOX*)element;
  GRP2DCANVAS*          canvas           = GetCanvas();

  if(!canvas) return false;
  
  canvas->Vectorfont_GetConfig()->SetSize(element_listbox->GetSizeFont());

  XSTRING text;
  if(!element_listbox->GetText()->IsEmpty()) text = element_listbox->GetText()->Get();
    else if(!element_listbox->GetMaskText()->IsEmpty()) text = element_listbox->GetMaskText()->Get();

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  XSTRING string_max;
  XDWORD  size_max   =  element_listbox->GetMaxSizeText();
  
  if(!size_max) size_max = text.GetSize();
  for(XDWORD c=0; c<size_max; c++)
    {
      string_max += NL("A");
    }

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO :		 		
      case                          0 : element->GetBoundaryLine()->width  = (double)canvas->VectorFont_GetWidth(string_max.Get());     break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                                break;	    
    }
   
  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO	:		 		
      case                          0 : element->GetBoundaryLine()->height  = (double)canvas->VectorFont_GetHeight(string_max.Get());   break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                             break;	    
    }

  return CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::ReapplyProgressBarAllocationLayout(UI_ELEMENT_PROGRESSBAR* element_progressbar, bool adjustsizemargin)
* @brief      Positions element_progressrect/element_animation/element_text against element_progressbar's OWN
*             (already resolved) box and applies the allocationtext-driven shift (DOWN/UP/LEFT move the rect/
*             animation to free up room for the caption).
* @note       ROOT-CAUSE FIX (2026-09, fourth pass): extracted out of CalculateBoundaryLine_ProgressBar() (which
*             still calls this immediately after resolving element_progressbar's own position/size) so that
*             UI_MANAGER::RefreshFlexProgressBarTracks() (UI_Manager.cpp) can call the EXACT same logic.
*
*             That function is a post-load hook that runs, unconditionally, for every ProgressBar right after
*             UI_LAYOUTENGINE::RunLayout() -- so that a ProgressBar which is itself a flex item gets its track
*             re-anchored to its flex-resolved position (see that function's own banner: without this, a flex
*             ProgressBar's track stayed stuck near the origin and never rendered at all). Its OLD implementation
*             called only the bare CalculePosition(element_progressrect, ...) -- the same first step
*             CalculateBoundaryLine_ProgressBar() takes -- and stopped there, never reapplying the allocationtext
*             switch below. For allocationtext="none"/"center" that is harmless (neither shifts the rect), which
*             is exactly why this went unnoticed; but for "down"/"up"/"left" -- e.g. progressbar3 in UI_Options'
*             example.xml, allocationtext="down" -- CalculateBoundaryLine_ProgressBar() (called moments earlier,
*             from CreatePartialLayout()) had ALREADY shifted element_progressrect to free up room for the
*             caption underneath it, and this post-load hook's bare CalculePosition() call silently RESET it back
*             to its natural, unshifted position, undoing that shift for the entire lifetime of the very first
*             frame. Confirmed live (see the GHOST-FILL FIX comment in Draw_ProgressBar()): the rect renders
*             unshifted (overlapping the caption's own zone) from the very first frame, and only jumps to its
*             correct shifted position the first time something ELSE re-runs CalculateBoundaryLine_ProgressBar()
*             (a real value change) -- a full-fledged, one-time geometry jump between "load" and "first value
*             change" that every downstream restore/redraw mechanism (including the widget-level backdrop cache
*             added alongside this fix) has to treat as a real, on-screen move, when the truly correct behaviour
*             is for the rect to already be at its final, shifted position from the very first frame and never
*             move again for this reason.
* @ingroup    USERINTERFACE
*
* @param[in]  element_progressbar : The progress-bar element itself; NULL is a no-op. Its own position/size are
*                                    read here, never written -- the caller (either call site) is responsible for
*                                    those being correct before calling this.
* @param[in]  adjustsizemargin : Forwarded to every CalculePosition() call below, exactly as CalculateBoundaryLine_ProgressBar() does.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::ReapplyProgressBarAllocationLayout(UI_ELEMENT_PROGRESSBAR* element_progressbar, bool adjustsizemargin)
{
  if(!element_progressbar) return;

  UI_ELEMENT*           element_progressrect  = element_progressbar->GetProgressRect();
  UI_ELEMENT_ANIMATION* element_animation     = (UI_ELEMENT_ANIMATION*)element_progressbar->Get_UIAnimation();
  UI_ELEMENT_TEXT*      element_text          = (UI_ELEMENT_TEXT*)element_progressbar->Get_UIText();

  if(element_progressrect)  CalculePosition(element_progressrect, element_progressbar->GetBoundaryLine()->width, element_progressbar->GetBoundaryLine()->height, adjustsizemargin);
  if(element_animation)     CalculePosition(element_animation, element_progressbar->GetBoundaryLine()->width, element_progressbar->GetBoundaryLine()->height, adjustsizemargin);
  if(element_text)          CalculePosition(element_text, element_progressbar->GetBoundaryLine()->width, element_progressbar->GetBoundaryLine()->height, adjustsizemargin);

  // DEEP ROOT-CAUSE FIX (2026-09, third pass -- see the GHOST-FILL FIX comment in Draw_ProgressBar() for the
  // visual symptom this was ultimately traced back to): CalculePosition() just above gives element_progressrect/
  // element_animation/element_text their NATURAL, unshifted positions -- captured here, ONCE, before the
  // allocationtext switch below moves anything. Every branch of that switch MUST compute its shifted
  // position(s) from these frozen natural values, never from GetXPosition()/GetYPosition() read again after a
  // shift has already been applied -- otherwise a position that looks like a one-time "make room for the
  // caption" adjustment is actually INCREMENTAL: calling this function a second time (this function is re-run
  // on every real value change, and, it turns out, on every dirty tick while an element stays marked dirty --
  // see below) reads back the ALREADY-shifted coordinate and shifts it again by the same amount, silently
  // drifting the rect/animation further and further off its correct position on every call, with nothing to
  // ever bring it back. That drift was confirmed live: element_progressrect's own Y position for progressbar3
  // (allocationtext="down") changed on successive calls instead of converging, which in turn kept the widget's
  // on-screen boundary line "changing" every tick and thereby kept it (and its caption) marked dirty forever --
  // a second-order perpetual-redraw bug on top of the original one-shot mispositioning, and the true reason a
  // caption that DOES get redrawn every single tick can still end up invisible: whatever it draws one tick, a
  // still-drifting rect (or a backdrop-restore keyed to the widget's nominal box) can legitimately no longer
  // agree with the very next tick. A previous pass here already made element_text's OWN position absolute for
  // the DOWN and LEFT cases (see the two comments retained below); this pass finishes the job for
  // element_progressrect, element_animation, and the UP case's element_text, using the same principle
  // throughout: every SetXPosition()/SetYPosition() below is computed from a NATURAL baseline captured once,
  // so calling this function any number of times with the same inputs always produces the same output.
  double rect_natural_x = element_progressrect ? element_progressrect->GetXPosition() : 0.0;
  double rect_natural_y = element_progressrect ? element_progressrect->GetYPosition() : 0.0;
  double anim_natural_x = element_animation    ? element_animation->GetXPosition()    : 0.0;
  double anim_natural_y = element_animation    ? element_animation->GetYPosition()    : 0.0;
  double text_natural_y = element_text         ? element_text->GetYPosition()         : 0.0;

  switch(element_progressbar->GetAllocationTextType())
    {
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    : break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN    : if(element_animation && element_text)     element_animation->SetYPosition(anim_natural_y - element_text->GetBoundaryLine()->height);
                                                            if(element_progressrect && element_text)  element_progressrect->SetYPosition(rect_natural_y - (element_text->GetBoundaryLine()->height + 6));
                                                            if(element_progressrect && element_text)  element_text->SetXPosition(element_progressrect->GetXPosition() + ((element_progressrect->GetBoundaryLine()->width - element_text->GetBoundaryLine()->width) / 2.0));   // horizontal center on the bar
                                                            // ROOT-CAUSE FIX (2026-09, confirmed live against progressbar3 in UI_Options' example.xml,
                                                            // allocationtext="down"): the line above moves element_progressrect UP by
                                                            // (text height + 6) to free up room for the caption underneath it, but this function
                                                            // never used to reposition element_text itself along that same axis -- element_text
                                                            // kept whatever Y CalculePosition(element_text, ...) gave it moments earlier (the SAME
                                                            // "natural" baseline element_progressrect ALSO started from, before its own shift).
                                                            // The very first time this function runs for a given element (layout parse), that
                                                            // stale Y happens to still read correctly on screen because nothing has repainted the
                                                            // gap yet; the first REAL value change afterwards re-enters this function (see
                                                            // ChangeTextElementValue's progress-type-father special case) and reapplies the SAME
                                                            // rect shift, but Draw_ProgressBar's own track/fill painting only ever covers
                                                            // element_progressrect's (now shifted) rect -- never the stale, unshifted caption
                                                            // position -- so that row of the canvas is left with whatever raw, uninitialized
                                                            // (fully transparent) pixels were there, and the freshly-drawn glyph blends against
                                                            // that instead of an opaque background: exactly the "letters overlap / caption
                                                            // vanishes" defect reported on progress widgets after their first value change.
                                                            // Fix: reposition element_text into the space element_progressrect's shift just
                                                            // freed up, the same way every OTHER branch below positions text relative to the
                                                            // rect's post-shift position (absolute, not incremental -- safe to call this
                                                            // function any number of times with an identical result each time).
                                                            if(element_progressrect && element_text)  element_text->SetYPosition(element_progressrect->GetYPosition() + element_text->GetBoundaryLine()->height + 6);
                                                            break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP      : { double shifted_text_y = text_natural_y;
                                                            if(element_animation && element_text)     shifted_text_y -= element_animation->GetBoundaryLine()->height;
                                                            if(element_progressrect && element_text)  shifted_text_y -= (element_progressrect->GetBoundaryLine()->height + 6);
                                                            if(element_text)                          element_text->SetYPosition(shifted_text_y);
                                                            if(element_progressrect && element_text)  element_text->SetXPosition(element_progressrect->GetXPosition() + ((element_progressrect->GetBoundaryLine()->width - element_text->GetBoundaryLine()->width)/2));   // horizontal center on the bar
                                                            break; }

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT   : if(element_progressrect && element_text)
                                                              {
                                                                element_text->SetXPosition(element_progressrect->GetXPosition() + element_progressrect->GetBoundaryLine()->width + 6);                                              // right of the bar (+gap), absolute not incremental
                                                                element_text->SetYPosition(element_progressrect->GetYPosition() - ((element_progressrect->GetBoundaryLine()->height - element_text->GetBoundaryLine()->height)/2));   // vertical center on the bar
                                                              }
                                                            break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT    : if(element_animation && element_text)     element_animation->SetXPosition(anim_natural_x + element_text->GetBoundaryLine()->width);
                                                            if(element_progressrect && element_text)  element_progressrect->SetXPosition(rect_natural_x + element_text->GetBoundaryLine()->width);
                                                            if(element_progressrect && element_text)  element_text->SetYPosition(element_progressrect->GetYPosition() - ((element_progressrect->GetBoundaryLine()->height - element_text->GetBoundaryLine()->height)/2));   // vertical center on the bar
                                                            // ROOT-CAUSE FIX (2026-09): same class of bug as ALLOCATION_TEXT_TYPE_DOWN above --
                                                            // element_progressrect is shifted RIGHT by element_text's width to free up room for
                                                            // the caption on its left, but element_text's own X position was never set here at
                                                            // all, so it kept the stale X CalculePosition(element_text, ...) gave it (the same
                                                            // pre-shift baseline the rect started from). Absolute, not incremental -- safe to
                                                            // call this function any number of times with an identical result each time.
                                                            if(element_progressrect && element_text)  element_text->SetXPosition(element_progressrect->GetXPosition() - element_text->GetBoundaryLine()->width);
                                                            break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : if(element_progressrect && element_text)
                                                              {
                                                                element_text->SetXPosition(element_progressrect->GetXPosition() + ((element_progressrect->GetBoundaryLine()->width - element_text->GetBoundaryLine()->width)/2));
                                                                element_text->SetYPosition(element_progressrect->GetYPosition() - ((element_progressrect->GetBoundaryLine()->height - element_text->GetBoundaryLine()->height)/2));
                                                              }
                                                            break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressBar(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line progress bar
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressBar(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_PROGRESSBAR* element_progressbar = (UI_ELEMENT_PROGRESSBAR*)element;
  if(!element_progressbar) return false;  

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;
  
  GetFatherSize(element, fatherwidth, fatherheight);

  UI_ELEMENT*           element_progressrect  = element_progressbar->GetProgressRect();
  UI_ELEMENT_ANIMATION* element_animation     = (UI_ELEMENT_ANIMATION*)element_progressbar->Get_UIAnimation();
  UI_ELEMENT_TEXT*      element_text          = (UI_ELEMENT_TEXT*)element_progressbar->Get_UIText();

  double maximgwidth  = 0.0f; 
  double maximgheight = 0.0f; 

  if(element_animation || element_text || element_progressrect)
    {
      switch((int)element->GetBoundaryLine()->width)
        {
          case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
          case                           0  : { switch(element_progressbar->GetAllocationTextType())
                                                  {                                            
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT   : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT    : if(element_progressrect)  maximgwidth += element_progressrect->GetBoundaryLine()->width;  
                                                                                                          if(element_animation)     maximgwidth += element_animation->GetBoundaryLine()->width;  
                                                                                                          if(element_text)          maximgwidth += element_text->GetBoundaryLine()->width;  
                                                                                                          break;

                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    :                                                                  
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : 
                                                                                              default   : if(element_progressrect)  maximgwidth = __MAX(maximgwidth, element_progressrect->GetBoundaryLine()->width);  
                                                                                                          if(element_animation)     maximgwidth = __MAX(maximgwidth, element_animation->GetBoundaryLine()->width);                                                                                                    
                                                                                                          if(element_text)          maximgwidth = __MAX(maximgwidth, element_text->GetBoundaryLine()->width);                                                                                                  
                                                                                                          break; 
                                                  } 
                                     
                                                element->GetBoundaryLine()->width = maximgwidth;
                                              }
                                              break;

          case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                          
                                              break;	    
        }

      switch((int)element->GetBoundaryLine()->height)
        {
          case UI_ELEMENT_TYPE_ALIGN_AUTO   :		 		
          case                           0  : { switch(element_progressbar->GetAllocationTextType())
                                                  {                                           
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP      : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN    : if(element_progressrect)  maximgheight += element_progressrect->GetBoundaryLine()->height; 
                                                                                                          if(element_animation)     maximgheight += element_animation->GetBoundaryLine()->height;  
                                                                                                          if(element_text)          maximgheight += element_text->GetBoundaryLine()->height;  
                                                                                                          break;

                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE    : 
                                                    case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER  : 
                                                                                              default   : if(element_progressrect)  maximgheight = __MAX(maximgheight, element_progressrect->GetBoundaryLine()->height); 
                                                                                                          if(element_animation)     maximgheight = __MAX(maximgheight, element_animation->GetBoundaryLine()->height);                                                                                                    
                                                                                                          if(element_text)          maximgheight = __MAX(maximgheight, element_text->GetBoundaryLine()->height);                                                                                                  
                                                                                                          break; 
                                                  }                                      
                                                element->GetBoundaryLine()->height = maximgheight;
                                              }
                                              break;

          case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height  = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                       
                                              break;	    
        }
    }
  
  CalculePosition(element_progressbar, fatherwidth, fatherheight, adjustsizemargin);

  // Sub-element positioning + the allocationtext-driven shift (element_progressrect/element_animation/
  // element_text against element_progressbar's OWN box, just resolved above) is shared with
  // UI_MANAGER::RefreshFlexProgressBarTracks() (UI_Manager.cpp) -- see ReapplyProgressBarAllocationLayout()'s
  // own banner and .cpp comment for why that function needs the EXACT same logic, not a hand-rolled subset.
  ReapplyProgressBarAllocationLayout(element_progressbar, adjustsizemargin);

  if(element_animation)
    {
      for(XDWORD c=0; c<element_animation->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* subelement = element_animation->GetComposeElements()->Get(c);
          if(subelement) CalculePosition(subelement, element_animation->GetBoundaryLine()->width, element_animation->GetBoundaryLine()->height, adjustsizemargin);         
        } 
    }

  SetAroundFromSubElements(element_progressbar);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressRadial(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate boundary line radial progress
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressRadial(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_PROGRESS_RADIAL* element_progress = (UI_ELEMENT_PROGRESS_RADIAL*)element;
  if(!element_progress) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  UI_ELEMENT_TEXT* element_text = (UI_ELEMENT_TEXT*)element_progress->Get_UIText();

  // A radial progress element has no intrinsic content size. AUTO/0 falls back to a square using the resolved opposite side
  // (or the father size when both are unset). MAX resolves to the father size on that axis.
  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                           0  : element->GetBoundaryLine()->width  = (element->GetBoundaryLine()->height > 0) ? element->GetBoundaryLine()->height : fatherwidth;    break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width  = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);                                                                                    break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                           0  : element->GetBoundaryLine()->height = (element->GetBoundaryLine()->width > 0) ? element->GetBoundaryLine()->width : fatherheight;     break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight);                                                                                   break;
    }

  CalculePosition(element_progress, fatherwidth, fatherheight, adjustsizemargin);

  // Center the caption inside the progress box (the child <text> is expected to use xpos="center" ypos="center").
  if(element_text) CalculePosition(element_text, element_progress->GetBoundaryLine()->width, element_progress->GetBoundaryLine()->height, adjustsizemargin);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ProgressImage_Layout(int allocation, double imgw, double imgh, double textw, double texth, double gap, double& bw, double& bh, double& ox, double& oy, double& tx, double& ty)
* @brief      Calculate boundary line progress image
* @ingroup    USERINTERFACE
* 
* @param[in]  allocation : Allocation value.
* @param[in]  imgw : Imgw value.
* @param[in]  imgh : Imgh value.
* @param[in]  textw : Textw value.
* @param[in]  texth : Texth value.
* @param[in]  gap : Gap value.
* @param[in]  bw : Bw value.
* @param[in]  bh : Bh value.
* @param[in]  ox : Ox value.
* @param[in]  oy : Oy value.
* @param[in]  tx : Tx value.
* @param[in]  ty : Ty value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ProgressImage_Layout(int allocation, double imgw, double imgh, double textw, double texth, double gap, double& bw, double& bh, double& ox, double& oy, double& tx, double& ty)
* @brief      Computes the ProgressImage caption layout for the current allocationtext.
* @ingroup    USERINTERFACE
* 
* @param[in]  allocation : Allocation value.
* @param[in]  imgw : Imgw value.
* @param[in]  imgh : Imgh value.
* @param[in]  textw : Textw value.
* @param[in]  texth : Texth value.
* @param[in]  gap : Gap value.
* @param[in]  bw : Bw value.
* @param[in]  bh : Bh value.
* @param[in]  ox : Ox value.
* @param[in]  oy : Oy value.
* @param[in]  tx : Tx value.
* @param[in]  ty : Ty value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_ProgressImage_Layout(int allocation, double imgw, double imgh, double textw, double texth, double gap, double& bw, double& bh, double& ox, double& oy, double& tx, double& ty)
{
  bw = imgw;  bh = imgh;                                                  // defaults: image only
  ox = 0.0;   oy = 0.0;
  tx = 0.0;   ty = 0.0;

  if(textw <= 0.0 && texth <= 0.0) return;                               // no caption

  switch(allocation)
    {
      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT  : bw = imgw + gap + textw;  bh = __MAX(imgh, texth);
                                                           ox = 0.0;               oy = (bh - imgh) / 2.0;
                                                           tx = imgw + gap;        ty = (bh - texth) / 2.0;
                                                           break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT   : bw = imgw + gap + textw;  bh = __MAX(imgh, texth);
                                                           tx = 0.0;               ty = (bh - texth) / 2.0;
                                                           ox = textw + gap;       oy = (bh - imgh) / 2.0;
                                                           break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN   : bw = __MAX(imgw, textw);  bh = imgh + gap + texth;
                                                           ox = (bw - imgw) / 2.0; oy = 0.0;
                                                           tx = (bw - textw) / 2.0;ty = imgh + gap;
                                                           break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP     : bw = __MAX(imgw, textw);  bh = imgh + gap + texth;
                                                           tx = (bw - textw) / 2.0;ty = 0.0;
                                                           ox = (bw - imgw) / 2.0; oy = texth + gap;
                                                           break;

      case UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER : bw = imgw;  bh = imgh;
                                                           ox = 0.0;               oy = 0.0;
                                                           tx = (imgw - textw) / 2.0;
                                                           ty = (imgh - texth) / 2.0;
                                                           break;

      default                                            : break;        // NONE: image only, caption keeps its own xpos/ypos
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_StatisticsChart(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Position/size statistics chart from authored width/height (CalculePosition).
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_StatisticsChart(UI_ELEMENT* element, bool adjustsizemargin)
{
  if(!element) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO :
      case                          0 : element->GetBoundaryLine()->width  = 120.0; break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->width  = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth); break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO :
      case                          0 : element->GetBoundaryLine()->height = 64.0; break;
      case UI_ELEMENT_TYPE_ALIGN_MAX  : element->GetBoundaryLine()->height = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight); break;
    }

  CalculePosition(element, fatherwidth, fatherheight, adjustsizemargin);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressImage(UI_ELEMENT* element, bool adjustsizemargin)
* @brief      Calculate Boundary Line Progress Image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  adjustsizemargin : Adjustsizemargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::CalculateBoundaryLine_ProgressImage(UI_ELEMENT* element, bool adjustsizemargin)
{
  UI_ELEMENT_PROGRESS_IMAGE* element_progressimage = (UI_ELEMENT_PROGRESS_IMAGE*)element;
  if(!element_progressimage) return false;

  double fatherwidth  = 0.0f;
  double fatherheight = 0.0f;

  GetFatherSize(element, fatherwidth, fatherheight);

  UI_ELEMENT_TEXT* element_text = (UI_ELEMENT_TEXT*)element_progressimage->Get_UIText();

  GRPBITMAP*       ref          = element_progressimage->GetImageFull() ? element_progressimage->GetImageFull() : element_progressimage->GetImageEmpty();
  double           imgwidth     = ref ? (double)ref->GetWidth()  : 0.0;
  double           imgheight    = ref ? (double)ref->GetHeight() : 0.0;

  int    allocation = (int)element_progressimage->GetAllocationTextType();
  double textw      = element_text ? element_text->GetBoundaryLine()->width  : 0.0;
  double texth      = element_text ? element_text->GetBoundaryLine()->height : 0.0;

  // Reserve the width of the widest percent value ("100%", 4 chars). The caption is sized from the CURRENT value, so a
  // boundary built at e.g. "91%" (3 chars) would clip the extra digit at level 100; reserving 4 chars avoids that.
  if(element_text && (textw > 0.0))
    {
      XDWORD nchars = element_text->GetText()->GetSize();
      if(nchars > 0)
        {
          double maxw = (textw / (double)nchars) * 4.0;
          if(maxw > textw) textw = maxw;
        }
    }

  double bw, bh, ox, oy, tx, ty;
  UI_SkinCanvas_ProgressImage_Layout(allocation, imgwidth, imgheight, textw, texth, 6.0, bw, bh, ox, oy, tx, ty);

  switch((int)element->GetBoundaryLine()->width)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                           0  : element->GetBoundaryLine()->width  = bw;           break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->width  = UI_SKINCANVAS_ResolveMaxSize(element, fatherwidth);  break;
    }

  switch((int)element->GetBoundaryLine()->height)
    {
      case UI_ELEMENT_TYPE_ALIGN_AUTO   :
      case                           0  : element->GetBoundaryLine()->height = bh;           break;
      case UI_ELEMENT_TYPE_ALIGN_MAX    : element->GetBoundaryLine()->height = UI_SKINCANVAS_ResolveMaxSize(element, fatherheight); break;
    }

  CalculePosition(element_progressimage, fatherwidth, fatherheight, adjustsizemargin);

  if(element_text)
    {
      if(allocation == UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE)
        {
          CalculePosition(element_text, element_progressimage->GetBoundaryLine()->width, element_progressimage->GetBoundaryLine()->height, adjustsizemargin);   // no allocation: caption keeps its own xpos/ypos
        }
       else
        {
          double bheight = element_progressimage->GetBoundaryLine()->height;                                                   // place caption per allocationtext (y = BOTTOM edge of the text)
          element_text->SetXPosition(element_progressimage->GetXPosition() + tx);
          element_text->SetYPosition((element_progressimage->GetYPosition() - bheight) + ty + texth);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::SetElementPosition(UI_ELEMENT* element, double x_position, double y_position)
* @brief      Set element position
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::SetElementPosition(UI_ELEMENT* element, double x_position, double y_position)
{
  if(!element) return false;

  element->GetBoundaryLine()->x = x_position;
  element->GetBoundaryLine()->y = y_position;

  bool status = CalculeBoundaryLine_AllElements(element);

  if(dynamic_cast<UI_ELEMENT_FORM*>(element))                                                                                                                                                                                                                                          
    {                                                     
      UI_ELEMENT_FORM* element_form = (UI_ELEMENT_FORM*)element;

      element_form->GetVisibleRect()->x = element_form->GetXPosition();
      element_form->GetVisibleRect()->y = element_form->GetYPosition();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::RestoreOnHide(UI_ELEMENT* element)
* @brief      HIDE-RESTORE FIX (2026-09): restores an element's own persistent "true backdrop" cache, if it has
*             one, the instant it turns invisible -- see the call site in UI_Skin.cpp's Draw() for the full
*             root-cause writeup. Only "form"/"menu" elements have such a cache today (formbackdrops, populated
*             and normally consumed by Draw_Form()'s own ALPHA-DARKENING FIX on every ordinary visible redraw);
*             every other element type falls through to the base UI_SKIN::RestoreOnHide() no-op via the default
*             case below, unchanged from before this fix.
* @note       Deliberately narrow: this does NOT attempt to restore progressradial/text captions (radialbackdrops/
*             textbackdrops) the same way, because neither of those widgets is ever hidden with SetVisible() by
*             any code in this codebase today -- only "form" is (see UI_SYSTEM::UserInterface_SelectSection()).
*             Extending this switch is the correct move if/when a future widget needs the same treatment; doing
*             so speculatively now would be untested, unused code.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element that just turned invisible.
*
* @return     bool : true if a cached backdrop was found and restored; false otherwise (matches the base
*                     no-op's contract, so a caller never needs to know which skin/type it got).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::RestoreOnHide(UI_ELEMENT* element)
{
  if(!element) return false;

  switch(element->GetType())
    {
      case UI_ELEMENT_TYPE_FORM :
      case UI_ELEMENT_TYPE_MENU :         // Draw_Menu() is a thin wrapper around Draw_Form() (same formbackdrops
                                           // cache, keyed by element pointer), so it needs the same treatment.
        {
          // ACCENT-BAR NEVER APPEARS FIX (2026-09): remember that this element is now hidden (see
          // formhiddentracked's own comment in UI_SkinCanvas.h) so Draw_Form() can tell, the next time it is
          // shown again, that any overlapping option's cached backdrop is unconditionally stale.
          if(formhiddentracked.Find(element) == NOTFOUND) formhiddentracked.Add(element);

          GRP2DREBUILDAREA* formbackdrop = FormBackdrop_Find(element);

          if(formbackdrop)
            {
              PutBitmapNoAlpha(formbackdrop->GetXPos(), formbackdrop->GetYPos(), formbackdrop->GetBitmap());

              return true;
            }
        }
        break;

      // Shared-slot toolbar twins (Stop/Run): MULTIOPTION has no FormBackdrop. When hidden, restore every
      // face Option/Anim backdrop so the sibling that stays visible is not painted over leftover icon ink.
      //
      // SHARED-SLOT TWIN (2026-11): after maximize/restore, Elements_SetToRedraw marks the HIDDEN twin
      // dirty too. Draw() then hits RestoreOnHide HERE — AFTER the visible twin already painted in XML
      // order (stop before run). A full-box PutBitmapNoAlpha of a stale Option/Anim backdrop re-seeds
      // the gray peel plate (57,59,61) into the under-icon legend strip. When another visible multioption
      // occupies the same AABB, only drop caches (visible twin owns the pixels). Otherwise restore
      // clipped to this element's own box so legend captions below the slot stay untouched.
      case UI_ELEMENT_TYPE_MULTIOPTION :
        {
          bool restored = false;

          bool twin_visible = false;
          double slot_l = 0.0, slot_t = 0.0, slot_r = 0.0, slot_b = 0.0;
          if(element->GetBoundaryLine())
            {
              double sx = element->GetXPosition();
              double sy = element->GetYPosition();
              if(element->GetFather()) GetAddPositionScrollSteps(element->GetFather(), sx, sy);
              slot_l = UI_BOUNDARYLINE_EdgeLeft  (sx, element->GetBoundaryLine()->width);
              slot_r = UI_BOUNDARYLINE_EdgeRight (sx, element->GetBoundaryLine()->width);
              slot_t = UI_BOUNDARYLINE_EdgeTop   (sy, element->GetBoundaryLine()->height);
              slot_b = UI_BOUNDARYLINE_EdgeBottom(sy, element->GetBoundaryLine()->height);

              UI_LAYOUT* layout = element->GetLayout();
              XVECTOR<UI_ELEMENT*>* roots = layout ? layout->Elements_Get() : NULL;
              if(roots)
                {
                  for(XDWORD i = 0; i < roots->GetSize(); i++)
                    {
                      UI_ELEMENT* sib = roots->Get(i);
                      if(!sib || sib == element) continue;
                      if(sib->GetType() != UI_ELEMENT_TYPE_MULTIOPTION) continue;
                      if(!sib->IsVisible() || !sib->GetBoundaryLine()) continue;

                      double tx = sib->GetXPosition();
                      double ty = sib->GetYPosition();
                      if(sib->GetFather()) GetAddPositionScrollSteps(sib->GetFather(), tx, ty);
                      double tl = UI_BOUNDARYLINE_EdgeLeft  (tx, sib->GetBoundaryLine()->width);
                      double tr = UI_BOUNDARYLINE_EdgeRight (tx, sib->GetBoundaryLine()->width);
                      double tt = UI_BOUNDARYLINE_EdgeTop   (ty, sib->GetBoundaryLine()->height);
                      double tb = UI_BOUNDARYLINE_EdgeBottom(ty, sib->GetBoundaryLine()->height);
                      // Same shared slot: near-identical AABB (2px tolerance for scroll/rounding).
                      if((fabs(tl - slot_l) <= 2.0) && (fabs(tr - slot_r) <= 2.0) &&
                         (fabs(tt - slot_t) <= 2.0) && (fabs(tb - slot_b) <= 2.0))
                        {
                          twin_visible = true;
                          break;
                        }
                    }
                }
            }

          for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* face = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
              if(!face) continue;

              GRP2DREBUILDAREA* optionbackdrop = OptionBackdrop_Find(face);
              if(optionbackdrop && optionbackdrop->GetBitmap() && !twin_visible)
                {
                  UI_SkinCanvas_RestoreBackdropIntersection(this, optionbackdrop, slot_l, slot_t, slot_r, slot_b);
                  restored = true;
                }
              OptionBackdrop_Delete(face);

              for(XDWORD d=0; d<face->GetComposeElements()->GetSize(); d++)
                {
                  UI_ELEMENT* facechild = (UI_ELEMENT*)face->GetComposeElements()->Get(d);
                  if(!facechild) continue;

                  if(facechild->GetType() == UI_ELEMENT_TYPE_ANIMATION)
                    {
                      GRP2DREBUILDAREA* animbackdrop = AnimBackdrop_Find(facechild);
                      if(animbackdrop && animbackdrop->GetBitmap() && !twin_visible)
                        {
                          UI_SkinCanvas_RestoreBackdropIntersection(this, animbackdrop, slot_l, slot_t, slot_r, slot_b);
                          restored = true;
                        }
                      AnimBackdrop_Delete(facechild);
                    }
                   else if(facechild->GetType() == UI_ELEMENT_TYPE_TEXT)
                    {
                      GRP2DREBUILDAREA* textbackdrop = TextBackdrop_Find(facechild);
                      if(textbackdrop && textbackdrop->GetBitmap() && !twin_visible)
                        {
                          UI_SkinCanvas_RestoreBackdropIntersection(this, textbackdrop, slot_l, slot_t, slot_r, slot_b);
                          restored = true;
                        }
                      TextBackdrop_Delete(facechild);
                    }
                }
            }

          if(twin_visible) CompactLegendsMarkNeedRepair();
          return restored;
        }

      default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::RestoreCompositionCaches()
* @brief      PutBitmapNoAlpha-restore every persistent true-backdrop WITHOUT discarding the caches.
* @note       Use before InvalidateCompositionCaches() when the live canvas was NOT rewritten underneath
*             (e.g. language change via Layouts_ApplyTranslations). Dropping FormBackdrop alone leaves
*             translucent fills (chrome caption color="black,25") on the canvas; the next Draw_Form then
*             alpha-blends the same fill again and the bar compounds toward opaque black — worst with
*             CFG chromes auto-hide, where the caption is repeatedly shown/redrawn over content.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::RestoreCompositionCaches()
{
  // Leaf caches first (text/anim/option/radial/progress), then forms — same order as PrepareTextContentChange.
  for(XDWORD c=0; c<textbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = textbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }

  for(XDWORD c=0; c<animbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = animbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }

  for(XDWORD c=0; c<optionbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = optionbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }

  for(XDWORD c=0; c<radialbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = radialbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }

  for(XDWORD c=0; c<progressbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = progressbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }

  for(XDWORD c=0; c<formbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = formbackdrops.Get(c);
      if(entry && entry->GetBitmap()) PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::RestoreRebuildAreas()
* @brief      PutBitmapNoAlpha-restore every one-shot rebuild area WITHOUT deleting them.
* @note       Language change (Layouts_ApplyTranslations) must erase alpha-blended glyph ink before
*             InvalidateCompositionCaches(). DeleteAllRebuildAreas() alone drops the snapshots without
*             restoring, so the next PreDraw captures old glyphs. Do NOT use Layout_PutBackground for
*             that: a full canvas wipe under a visible auto-hide chrome caption leaves ListBoxMenu /
*             edittext blank when Overlay_ClearSuppressedContentDirt freezes dirt under the caption band.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::RestoreRebuildAreas()
{
  XVECTOR<GRP2DREBUILDAREA*>* areas = GetRebuildAreas();
  if(!areas) return;

  // Highest index first so overlapping snapshots peel in reverse capture order.
  for(XDWORD c = areas->GetSize(); c > 0; c--)
    {
      GRP2DREBUILDAREA* area = areas->Get(c - 1);
      if(area && area->GetBitmap()) PutBitmapNoAlpha(area->GetXPos(), area->GetYPos(), area->GetBitmap());
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::EraseElementRebuildArea(UI_ELEMENT* element)
* @brief      Clear glyph ink for a leaf TEXT/TEXTBOX before its string/language changes.
* @note       Used by language apply. Prefer this over RestoreRebuildAreas()+InvalidateCompositionCaches()
*             +full Elements_SetToRedraw(): that path restores ListBoxMenu/edit backdrops to parchment while
*             the auto-hide caption is visible, then Overlay_ClearSuppressedContentDirt freezes their dirt
*             (they intersect the caption AABB via EdgeTop=y-height) until the bar hides.
*
*             Idle translate-only TEXTBOX (UI_Options textbox01) often has no rebuild area (orphan-discard).
*             Neighbour rebuild areas that still hold a snapshot of the OLD glyphs (e.g. thermometer "%" that
*             overlaps the textbox) must be dropped WITHOUT restoring — otherwise RebuildAllAreas() would
*             PutBitmapNoAlpha those stale pixels back after this erase and the next Draw_TextBox stacks the
*             new language on top of the previous one.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::EraseElementRebuildArea(UI_ELEMENT* element)
{
  if(!element || !element->GetBoundaryLine()) return false;

  double x = element->GetXPosition();
  double y = element->GetYPosition();
  if(element->GetFather()) GetAddPositionScrollSteps(element->GetFather(), x, y);

  double w = element->GetBoundaryLine()->width;
  double h = element->GetBoundaryLine()->height;
  if((w <= 0.0) || (h <= 0.0)) return EraseElementWithLayoutBackground(element);

  const double pad = 8.0;
  double left   = UI_BOUNDARYLINE_EdgeLeft  (x, w) - pad;
  double top    = UI_BOUNDARYLINE_EdgeTop   (y, h) - pad;
  double right  = UI_BOUNDARYLINE_EdgeRight (x, w) + pad;
  double bottom = UI_BOUNDARYLINE_EdgeBottom(y, h) + pad;

  // Drop every rebuild area that intersects the erase AABB (including this element) WITHOUT restoring.
  // Restoring would re-introduce glyph ink captured while the previous language was on screen.
  XVECTOR<GRP2DREBUILDAREA*>* areas = GetRebuildAreas();
  if(areas)
    {
      for(int index = (int)areas->GetSize() - 1; index >= 0; index--)
        {
          GRP2DREBUILDAREA* area = areas->Get((XDWORD)index);
          if(!area || !area->GetBitmap()) continue;

          double ax1 = area->GetXPos();
          double ay1 = area->GetYPos();
          double ax2 = ax1 + (double)area->GetBitmap()->GetWidth();
          double ay2 = ay1 + (double)area->GetBitmap()->GetHeight();

          if(ax2 <= left || ax1 >= right || ay2 <= top || ay1 >= bottom) continue;

          UI_ELEMENT* owner = (UI_ELEMENT*)area->GetExtraData();
          if(owner) owner->SetMustReDraw(true);

          areas->Delete(area);
          GEN_DELETE area;
        }
    }

  // Always blit clean layout parchment into the AABB (do not rely on a prior rebuild-area snapshot).
  return EraseElementWithLayoutBackground(element);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::EraseElementWithLayoutBackground(UI_ELEMENT* element)
* @brief      Copy the owning layout's background image into the element's boundary (plus AA pad).
* @note       Fallback for language change on fatherless TEXTBOX when no rebuild area / FormBackdrop
*             exists. Uses GetCanvas()+GetPixel/PutPixel (same paint target as Draw_*) rather than
*             GetSubBitmap+PutBitmapNoAlpha: the latter can miss the visible buffer when rebuild-area
*             target canvas and GetCanvas() diverge, and GetSubBitmap's PutBlendPixel path can yield a
*             failed/empty crop. Avoids a full-canvas wipe (chrome auto-hide / suppressed menu dirt).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::EraseElementWithLayoutBackground(UI_ELEMENT* element)
{
  if(!element || !element->GetBoundaryLine()) return false;

  // Must match Draw_* paint target (honours canvas_override / design canvas).
  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(element);
  if(!layout || !layout->GetBackground()) return false;

  GRPBITMAP* bg = layout->GetBackground()->GetBitmap();
  if(!bg) return false;

  int cw = (int)canvas->GetWidth();
  int ch = (int)canvas->GetHeight();
  if((cw <= 0) || (ch <= 0)) return false;

  if((bg->GetWidth() != cw) || (bg->GetHeight() != ch))
    {
      bg->Scale(cw, ch);
    }

  double x = element->GetXPosition();
  double y = element->GetYPosition();
  if(element->GetFather()) GetAddPositionScrollSteps(element->GetFather(), x, y);

  double w = element->GetBoundaryLine()->width;
  double h = element->GetBoundaryLine()->height;
  if((w <= 0.0) || (h <= 0.0)) return false;

  // AA / inline-image (E.png drop-cap) overhang past the authored box.
  const double pad = 8.0;
  double left   = UI_BOUNDARYLINE_EdgeLeft  (x, w) - pad;
  double top    = UI_BOUNDARYLINE_EdgeTop   (y, h) - pad;
  double right  = UI_BOUNDARYLINE_EdgeRight (x, w) + pad;
  double bottom = UI_BOUNDARYLINE_EdgeBottom(y, h) + pad;

  if(left < 0.0) left = 0.0;
  if(top  < 0.0) top  = 0.0;
  if(right  > (double)cw) right  = (double)cw;
  if(bottom > (double)ch) bottom = (double)ch;

  int ix1 = (int)left;
  int iy1 = (int)top;
  int ix2 = (int)right;
  int iy2 = (int)bottom;
  if((ix2 <= ix1) || (iy2 <= iy1)) return false;

  int bgw = (int)bg->GetWidth();
  int bgh = (int)bg->GetHeight();

  for(int py = iy1; py < iy2; py++)
    {
      if(py < 0 || py >= bgh || py >= ch) continue;

      for(int px = ix1; px < ix2; px++)
        {
          if(px < 0 || px >= bgw || px >= cw) continue;

          GRP2DCOLOR* pixel = bg->GetPixel(px, py);
          if(pixel) canvas->PutPixel((double)px, (double)py, pixel);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::InvalidateCompositionCaches()
* @brief      COMPOSITION-RESET (2026-09): drop every persistent true-backdrop cache and the one-shot rebuild
*             areas after the canvas under the widgets has been fully rewritten (Layout_PutBackground, virtual
*             keyboard modal show/hide, etc.). Without this, Draw_* restores bitmaps captured against the
*             previous composition with PutBitmapNoAlpha and paints opaque white/stale rectangles over menus
*             and chrome. Next Draw_* re-captures from the fresh background. Framework-wide, not example-specific.
* @note       When the canvas was NOT rewritten first, call RestoreCompositionCaches() before this so
*             translucent fills (e.g. chrome caption) are erased instead of compounding on the next Draw_Form.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::InvalidateCompositionCaches()
{
  DeleteAllRebuildAreas();

  progressbackdrops.DeleteContents();
  progressbackdrops.DeleteAll();

  progressbarlastbounds.DeleteContents();
  progressbarlastbounds.DeleteAll();

  formbackdrops.DeleteContents();
  formbackdrops.DeleteAll();

  radialbackdrops.DeleteContents();
  radialbackdrops.DeleteAll();

  textbackdrops.DeleteContents();
  textbackdrops.DeleteAll();

  animbackdrops.DeleteContents();
  animbackdrops.DeleteAll();

  optionbackdrops.DeleteContents();
  optionbackdrops.DeleteAll();

  // Pointers only (elements owned by the layout); after a full composition reset the "was hidden" hint is
  // meaningless because every form/menu will re-capture from the new background on the next Draw_Form.
  formhiddentracked.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::PrepareTextContentChange(UI_ELEMENT* element)
* @brief      Synchronously erase a text element's alpha-blended glyph ink before its string (or language face)
*             changes. Restores the text's TextBackdrop (if any) then the nearest paintable father FormBackdrop
*             so CJK overhang outside the text box is cleared too. Call BEFORE replacing GetText() / remasuring.
* @note       Chrome title language switch (UI_System F12, start in Chinese then change): translucent caption
*             fill alone never erases Noto strokes; without this, Spanish paints over leftover 系统监控器.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::DiscardTextBackdrop(UI_ELEMENT* element)
{
  if(!element) return false;
  return TextBackdrop_Delete(element);
}


bool UI_SKINCANVAS::DiscardAnimBackdrop(UI_ELEMENT* element)
{
  if(!element) return false;
  return AnimBackdrop_Delete(element);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::RepaintCompactLegendForms(UI_LAYOUT* layout)
* @brief      Same-frame repair for compact under-icon legend forms after MultiOption peels.
* @note       Shared-slot Stop/Run peels and hidden-twin RestoreOnHide can leave a solid gray plate
*             (57,59,61) under Demarrer/Arreter. SetToRedraw alone is too late when RebuildAllAreas
*             already ran; page-fill + Draw here (end of layout / after chrome) reprints glyphs on a
*             clean page colour.
*             MUST stay narrow: height<=28 alone also matched XTraceMonitor's opstatus foot (h=22,
*             w=972). The pass-2 wipe then only redrew type=text children and erased the textbox
*             messages every frame. Limit to short AND narrow transparent under-icon caption strips.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::RepaintCompactLegendForms(UI_LAYOUT* layout)
{
  compact_legends_need_repair = false;
  if(!layout || !GetCanvas()) return false;

  XVECTOR<UI_ELEMENT*>* roots = layout->Elements_Get();
  if(!roots) return false;

  GRP2DCANVAS* canvas = GetCanvas();

  XBYTE pr = 24, pg = 28, pb = 36;
  if(layout->GetBackground() && layout->GetBackground()->GetColor() &&
     layout->GetBackground()->GetColor()->IsValid())
    {
      UI_COLOR* page = layout->GetBackground()->GetColor();
      pr = (XBYTE)page->GetRed();
      pg = (XBYTE)page->GetGreen();
      pb = (XBYTE)page->GetBlue();
    }

  bool any = false;
  for(XDWORD ei = 0; ei < roots->GetSize(); ei++)
    {
      UI_ELEMENT* sib = roots->Get(ei);
      if(!sib || !sib->IsVisible()) continue;
      if(sib->GetType() != UI_ELEMENT_TYPE_FORM) continue;
      if(!sib->GetBoundaryLine()) continue;
      // Under-icon caption strips only (e.g. 73×14). Full-width foots/status bars are excluded.
      if(sib->GetBoundaryLine()->height > 28.0) continue;
      if(sib->GetBoundaryLine()->width  > 120.0) continue;

      UI_ELEMENT_FORM* sf = (UI_ELEMENT_FORM*)sib;
      UI_COLOR* sfill = sf->IsBackgroundColorSet() ? sf->GetBackgroundColor() : sf->GetColor();
      // Paintable panels (border/fill/shadow) own their own compose (textbox, etc.) — never force-wipe.
      bool legend_paintable = (sfill && sfill->GetAlpha() > 0) ||
                              (sf->GetBorderWidth() > 0.0)     ||
                              sf->IsBoxShadowSet();
      if(legend_paintable) continue;

      double sx = sib->GetXPosition();
      double sy = sib->GetYPosition();
      if(sib->GetFather()) GetAddPositionScrollSteps(sib->GetFather(), sx, sy);
      double sl = UI_BOUNDARYLINE_EdgeLeft(sx, sib->GetBoundaryLine()->width);
      double st = UI_BOUNDARYLINE_EdgeTop(sy, sib->GetBoundaryLine()->height);
      double sr = UI_BOUNDARYLINE_EdgeRight(sx, sib->GetBoundaryLine()->width);
      double sb = UI_BOUNDARYLINE_EdgeBottom(sy, sib->GetBoundaryLine()->height);
      double lw = sr - sl;
      double lh = sb - st;
      // Guard band below the authored box: peel plates sat in y127-132 while height=14 ended at 133.
      if(lh < 14.0) lh = 14.0;
      lh += 8.0;
      if(lw < 1.0) lw = 1.0;
      if(lh < 1.0) lh = 1.0;

      XBYTE fr = pr, fg = pg, fb = pb;

      GRP2DCOLOR_RGBA8 col(fr, fg, fb, 255);

      // Pass 1: opaque page seed (PutPixel — copy_pixel, clip-safe).
      {
        int fh_i = (int)lh; if(fh_i < 1) fh_i = 1;
        int fw_i = (int)lw; if(fw_i < 1) fw_i = 1;
        for(int py = 0; py < fh_i; py++)
          {
            for(int px = 0; px < fw_i; px++)
              {
                canvas->PutPixel(sl + (double)px, st + (double)py, (const GRP2DCOLOR*)&col);
              }
          }
      }

      GEN_USERINTERFACE.Elements_SetToRedraw(sib, true);
      Draw(sib);

      // Pass 2: Draw_Form/Draw_Text can still leave a gray plate in the lower strip (confirmed:
      // y127-132 = 57,59,61 after form+text). Wipe again, then reprint ONLY text children so
      // nothing after the form's PostDraw can keep the plate under the glyphs.
      {
        int fh_i = (int)lh; if(fh_i < 1) fh_i = 1;
        int fw_i = (int)lw; if(fw_i < 1) fw_i = 1;
        for(int py = 0; py < fh_i; py++)
          {
            for(int px = 0; px < fw_i; px++)
              {
                canvas->PutPixel(sl + (double)px, st + (double)py, (const GRP2DCOLOR*)&col);
              }
          }
      }
      for(XDWORD c = 0; c < sib->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* child = sib->GetComposeElements()->Get(c);
          if(!child || child->GetType() != UI_ELEMENT_TYPE_TEXT) continue;
          child->SetMustReDraw(true);
          Draw(child);
        }

      any = true;
    }

  compact_legends_need_repair = false;
  return any;
}


bool UI_SKINCANVAS::PrepareTextContentChange(UI_ELEMENT* element)
{
  if(!element) return false;

  bool erased = false;

  GRP2DREBUILDAREA* textbackdrop = TextBackdrop_Find(element);
  if(textbackdrop && textbackdrop->GetBitmap())
    {
      PutBitmapNoAlpha(textbackdrop->GetXPos(), textbackdrop->GetYPos(), textbackdrop->GetBitmap());
      TextBackdrop_Delete(element);
      erased = true;
    }

  // Nearest form/menu father (chrome caption, card, …).
  UI_ELEMENT* father = element->GetFather();
  while(father)
    {
      if((father->GetType() == UI_ELEMENT_TYPE_FORM) || (father->GetType() == UI_ELEMENT_TYPE_MENU))
        break;
      father = father->GetFather();
    }

  GRP2DCANVAS* canvas = GetCanvas();

  if(father)
    {
      GRP2DREBUILDAREA* formbackdrop = FormBackdrop_Find(father);

      // Prefer restoring the true under-form backdrop (not the fill RGB). Painting opaque "black" from
      // color="black,25" left a visible pure-black patch under/after the title glyphs (text-box width
      // includes side bearings past the last letter's ink) while the rest of the bar stayed charcoal.
      //
      // When FormBackdrop restore succeeds, that ALREADY clears glyph ink (including CJK overhang) back to
      // the content under the caption. Do NOT follow with an opaque Rectangle wipe: Layouts_ApplyTranslations
      // then calls RestoreCompositionCaches, but FormBackdrop was deleted here so the wipe would stay on the
      // canvas, get re-captured as the new FormBackdrop/TextBackdrop, and show up as a black stub after the
      // chrome title. Drop the cache so the next Draw_Form re-captures from the restored under-content.
      if(formbackdrop && formbackdrop->GetBitmap())
        {
          PutBitmapNoAlpha(formbackdrop->GetXPos(), formbackdrop->GetYPos(), formbackdrop->GetBitmap());
          FormBackdrop_Delete(father);
          erased = true;
        }
       else if(canvas && father->GetBoundaryLine())
        {
          // No FormBackdrop: wipe only when the father paints ink (translucent chrome caption, etc.).
          // Transparent toolbar legend forms must NOT get a hard-coded charcoal plate — that became a
          // permanent gray bar under Stop/Run captions once re-captured as TextBackdrop.
          UI_ELEMENT_FORM* father_form = (UI_ELEMENT_FORM*)father;
          UI_COLOR* fillsrc = father_form->IsBackgroundColorSet() ? father_form->GetBackgroundColor()
                                                                  : father_form->GetColor();
          bool father_paintable = (fillsrc && fillsrc->GetAlpha() > 0) ||
                                  (father_form->GetBorderWidth() > 0.0) ||
                                  father_form->IsBoxShadowSet();

          if(father_paintable)
            {
              double fx = father->GetXPosition();
              double fy = father->GetYPosition();
              if(father->GetFather()) GetAddPositionScrollSteps(father->GetFather(), fx, fy);

              double left   = UI_BOUNDARYLINE_EdgeLeft  (fx, father->GetBoundaryLine()->width);
              double right  = UI_BOUNDARYLINE_EdgeRight (fx, father->GetBoundaryLine()->width);
              double top    = UI_BOUNDARYLINE_EdgeTop   (fy, father->GetBoundaryLine()->height);
              double bottom = UI_BOUNDARYLINE_EdgeBottom(fy, father->GetBoundaryLine()->height);

              XBYTE pr = 13, pg = 17, pb = 23;
              UI_LAYOUT* layout = father->GetLayout();
              if(layout && layout->GetBackground() && layout->GetBackground()->GetColor() &&
                 layout->GetBackground()->GetColor()->IsValid())
                {
                  UI_COLOR* page = layout->GetBackground()->GetColor();
                  pr = (XBYTE)page->GetRed();
                  pg = (XBYTE)page->GetGreen();
                  pb = (XBYTE)page->GetBlue();
                }

              GRP2DCOLOR_RGBA8 solid(pr, pg, pb, 255);
              GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);
              canvas->SetFillColor(&solid);
              canvas->SetLineColor(&noline);
              canvas->Rectangle(left, bottom, right, top, true);
              erased = true;
            }
           else if(!erased)
            {
              erased = EraseElementWithLayoutBackground(element) || erased;
            }
        }

      father->SetMustReDraw(true);
    }
  // No form father (classic XML TEXTBOX): opaque fill would leave a black pad. Always blit layout
  // background (even if a TextBackdrop was restored) so language change cannot stack glyph ink on
  // parchment when TEXTBACKDROP is off and the leaf rebuild area was orphan-discarded.
  if(!father)
    {
      erased = EraseElementWithLayoutBackground(element) || erased;
    }

  element->SetMustReDraw(true);
  return erased;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_SKINCANVAS::CompositionCacheCount()
* @brief      Count persistent true-backdrop cache entries (for tests / diagnostics).
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Sum of form/option/text/progress/radial backdrop entries.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_SKINCANVAS::CompositionCacheCount()
{
  return formbackdrops.GetSize()
       + optionbackdrops.GetSize()
       + textbackdrops.GetSize()
       + animbackdrops.GetSize()
       + progressbackdrops.GetSize()
       + radialbackdrops.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::Draw_Scroll(UI_ELEMENT* element)
* @brief      Draw scroll
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Scroll(UI_ELEMENT* element)
{
  return true;
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Text(UI_ELEMENT* element)
* @brief      Draw text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Text(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_TEXT*    element_text  = (UI_ELEMENT_TEXT*)element;
  GRP2DCANVAS*          canvas        = GetCanvas();
  double              x_position    = 0.0f;
  double              y_position    = 0.0f;
  XRECT               clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      // TEXT CAPTION GHOSTING FIX (2026-09): Draw_Text() paints glyph ink PURELY by alpha-blending
      // (VectorFont_Print below is anti-aliased glyph ink, never a solid fill) onto whatever is already on the
      // canvas. That is normally safe because the generic per-tick rebuild-area system (PreDrawFunction() /
      // UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas()) is expected to restore the true backdrop before each
      // real repaint -- but a text element whose value changes via a live "#[...]" placeholder (e.g.
      // dashboard.xml's "#[UPTIME_SECONDS]") is typically dirty for exactly ONE frame per value change and idle
      // in between, so its rebuild area is ORPHAN-DISCARDED (deleted WITHOUT restoring -- see
      // RebuildAllAreas()'s own orphan-discard comment: "the element's current on-screen content is already
      // correct", true only for a widget that clears its own background, NOT for glyph ink blended on top of
      // it) the very next frame. The NEXT real value change then creates a brand-new area that captures
      // whatever is CURRENTLY on screen -- already showing the PREVIOUS glyph's ink -- and blends the new glyph
      // straight on top. Confirmed live on UI_System's "uptime_seconds_value" (ticks every second): its
      // "Segundos" tile converges from a clean single digit into a solid, near-opaque green block within a few
      // seconds, each new digit's anti-aliased strokes blending onto every previous one. Exactly the same root
      // cause already fixed for progressradial captions -- see the RADIAL CAPTION GHOSTING FIX in
      // Draw_ProgressRadial() -- generalised here to Draw_Text() itself, since ANY text element painted this
      // way can suffer it, not only captions nested inside a progress widget.
      //
      // Fix: cache the TRUE backdrop and restore it (PutBitmapNoAlpha(), GEN's own real restore primitive,
      // never a synthetic fill colour) immediately before repainting on every later real redraw, mirroring the
      // already-verified ProgressBackdrop/FormBackdrop/RadialBackdrop fixes elsewhere in this file.
      //
      // One extra wrinkle specific to plain text: an element authored with no explicit "width" auto-measures
      // its own rebuild-area box from its CURRENT string on every tick (see UI_MANAGER::
      // ChangeTextElementValue()), so a caption whose text grows/shrinks (e.g. "9" -> "10") can change WIDTH
      // between two real redraws. Guarded below by discarding a cached entry whose stored geometry no longer
      // matches this tick's rebuild area and re-capturing fresh instead of blindly restoring it, so a resize
      // does not restore a wrong-sized bitmap over a differently-sized area.
      //
      // RESIZE-RECAPTURE FIX (2026-09, live-diagnosed with a temporary instrumented build): the first version of
      // this guard re-captured the NEW, wider area directly from whatever was currently on screen at that
      // instant -- but "currently on screen" at that exact moment is NOT the true backdrop for the old, still-
      // un-erased sub-region: the OLD glyph ("9") was painted there on a previous tick and, being pure alpha-
      // blended ink with nothing that ever clears it, is still sitting on screen when the box widens for "10".
      // Capturing it as the "backdrop" permanently baked that leftover "9" into the cache, which was then
      // faithfully restored every following tick -- a static ghost digit stuck in front of every later value,
      // confirmed live via a temporary trace (mismatch detected and a fresh capture taken at the exact "9"->"10"
      // transition, old cached area narrower than the new one, followed by an unbroken run of plain restores
      // while the visible tile kept showing a stuck extra "9"). Fix: when an old, now-undersized cached entry
      // exists, restore IT FIRST -- into its own old position/size, using the same real PutBitmapNoAlpha()
      // primitive as every other restore in this file -- before dropping it and capturing the new, wider area.
      // That erases the stale ink from the sub-region the old entry covered, so the fresh capture that follows
      // reads genuinely clean pixels there; the newly-exposed strip outside the old bounds was never painted by
      // this element in the first place, so it is already correct.
      // TEXT CAPTION GHOSTING (classic XML / UI_Options): rebuild areas are orphan-discarded while idle, so the
      // next MustReDraw (hover overlap chain, exclusive-select redraw, chrome/content overlap invalidation)
      // would alpha-blend glyphs onto the previous ink and titles go progressively blacker.
      //
      // Always: (1) restore any TextBackdrop (erase prior glyphs), (2) re-capture a clean backdrop if missing
      // (ownarea geometry when available, else the text AABB), (3) never Delete without restoring first.
      // Capturing without ownarea matters when a dirty father owns the rebuild area — FormBackdrop already
      // cleared our box, so the canvas is clean HERE and we must snapshot before printing or the next solo
      // redraw (orphan path) will bake the previous glyphs into a brand-new rebuild area.
      if(!GEN_USERINTERFACE.ModalLayer_IsCompositing())
        {
          GRP2DREBUILDAREA* ownarea_text = GetRebuildAreaByElement(element);
          GRP2DREBUILDAREA* textbackdrop = TextBackdrop_Find(element);

          // Compact form fathers (toolbar legends under shared-slot bubbles): never trust a
          // TextBackdrop snapshot — Option/Anim peel bakes a solid gray plate that PutBitmapNoAlpha
          // would restore AFTER Draw_Form's page wipe (confirmed: form wipe then TextBackdrop
          // restore re-seeded 57,59,61 under Demarrer/Arreter). Walk fathers so a text nested in
          // an option/button still finds the legend form. Height<=32 = under-icon caption strip.
          UI_ELEMENT* text_father = element->GetFather();
          bool legend_form_father = false;
          UI_COLOR* legend_fill = NULL;
          for(UI_ELEMENT* walk = text_father; walk; walk = walk->GetFather())
            {
              if(walk->GetType() != UI_ELEMENT_TYPE_FORM) continue;
              if(!walk->GetBoundaryLine()) continue;
              if(walk->GetBoundaryLine()->height > 32.0) continue;
              UI_ELEMENT_FORM* tf = (UI_ELEMENT_FORM*)walk;
              legend_fill = tf->IsBackgroundColorSet() ? tf->GetBackgroundColor() : tf->GetColor();
              text_father = walk;
              legend_form_father = true;
              break;
            }

          if(legend_form_father && canvas && element->GetBoundaryLine())
            {
              if(textbackdrop)
                {
                  // NEVER restore a legend TextBackdrop — it routinely bakes the shared-slot peel
                  // gray plate (57,59,61). Father/page fill below is the only trusted backdrop.
                  TextBackdrop_Delete(element);
                  textbackdrop = NULL;
                }

              // Wipe the legend strip from the live father position (not a possibly stale
              // VisibleRect at 0,0). Extend below the content box: the gray peel plate sat in
              // y127-132 while a content-shrunk wipe only covered ~y119-126.
              double fill_x = text_father->GetXPosition();
              double fill_w = text_father->GetBoundaryLine()->width;
              double fill_h = text_father->GetBoundaryLine()->height;
              if(fill_h < 14.0) fill_h = 14.0;
              fill_h += 8.0;
              double fill_y = text_father->GetYPosition() - (fill_h - 8.0);
              if(text_father->GetFather()) GetAddPositionScrollSteps(text_father->GetFather(), fill_x, fill_y);
              if(fill_w < 1.0) fill_w = 1.0;
              if(fill_h < 1.0) fill_h = 1.0;

              XBYTE pr = 24, pg = 28, pb = 36;
              if(legend_fill && legend_fill->GetAlpha() > 0)
                {
                  pr = (XBYTE)legend_fill->GetRed();
                  pg = (XBYTE)legend_fill->GetGreen();
                  pb = (XBYTE)legend_fill->GetBlue();
                }
               else
                {
                  UI_LAYOUT* layout = element->GetLayout();
                  if(layout && layout->GetBackground() && layout->GetBackground()->GetColor() &&
                     layout->GetBackground()->GetColor()->IsValid())
                    {
                      UI_COLOR* page = layout->GetBackground()->GetColor();
                      pr = (XBYTE)page->GetRed();
                      pg = (XBYTE)page->GetGreen();
                      pb = (XBYTE)page->GetBlue();
                    }
                }
              // PreDraw clipped this TEXT to a content-shrunk box; PutBitmapNoAlpha then only
              // updated ~8px and left the gray peel plate in the rest of the legend strip.
              // Lift clip to the wipe rect (y-down: top < bottom) and write pixels directly.
              XRECT legend_clip_saved;
              canvas->GetClipBox(legend_clip_saved);
              // GEN/AGG clip order: (left, bottom, right, top) — same as PreDrawFunction.
              canvas->SetClipBox(fill_x, fill_y + fill_h, fill_x + fill_w, fill_y);

              GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
              int fh_i = (int)fill_h;
              int fw_i = (int)fill_w;
              if(fh_i < 1) fh_i = 1;
              if(fw_i < 1) fw_i = 1;
              for(int py = 0; py < fh_i; py++)
                {
                  for(int px = 0; px < fw_i; px++)
                    {
                      canvas->PutPixel(fill_x + (double)px, fill_y + (double)py, (const GRP2DCOLOR*)&pagecol);
                    }
                }

              canvas->SetClipBox(legend_clip_saved);
            }
           else
            {
              if(textbackdrop && textbackdrop->GetBitmap())
                {
                  if(ownarea_text && ownarea_text->GetBitmap() && !TextBackdrop_MatchesArea(textbackdrop, ownarea_text))
                    {
                      PutBitmapNoAlpha(textbackdrop->GetXPos(), textbackdrop->GetYPos(), textbackdrop->GetBitmap());
                      TextBackdrop_Delete(element);
                      textbackdrop = NULL;
                    }
                   else
                    {
                      PutBitmapNoAlpha(textbackdrop->GetXPos(), textbackdrop->GetYPos(), textbackdrop->GetBitmap());
                    }
                }

              if(!TextBackdrop_Find(element))
                {
                  double cap_x = 0.0;
                  double cap_y = 0.0;
                  double cap_w = 0.0;
                  double cap_h = 0.0;

                  if(ownarea_text && ownarea_text->GetBitmap())
                    {
                      cap_x = ownarea_text->GetXPos();
                      cap_y = ownarea_text->GetYPos();
                      cap_w = (double)ownarea_text->GetBitmap()->GetWidth();
                      cap_h = (double)ownarea_text->GetBitmap()->GetHeight();
                    }
                   else
                    {
                      double tw = element->GetBoundaryLine()->width;
                      double th = element->GetBoundaryLine()->height;
                      if(tw < 1.0) tw = 1.0;
                      if(th < 1.0) th = 1.0;
                      cap_x = x_position;
                      cap_y = UI_BOUNDARYLINE_EdgeTop(y_position, th);
                      cap_w = tw;
                      cap_h = th;
                    }

                  if((cap_w > 0.0) && (cap_h > 0.0))
                    {
                      TextBackdrop_Capture(element, cap_x, cap_y, cap_w, cap_h);
                    }
                }
            }
        }

      GRP2DCOLOR_RGBA8  color(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());

      canvas->Vectorfont_GetConfig()->SetColor(&color);
      canvas->Vectorfont_GetConfig()->SetSize(element_text->GetSizeFont());

      XCHAR*  faces[GRP2DCANVAS_VECTORFONT_STACK_MAX];
      XSTRING langface;
      XDWORD  nfaces = UI_SKINCANVAS_BuildTextFaceStack(this, element_text, faces, GRP2DCANVAS_VECTORFONT_STACK_MAX, langface);

      // "text-align" (Step 10, see UI_ELEMENT::GetTextAlign() / UI_MANAGER::GetLayoutElement_Base). GEN's
      // VectorFont_Print(x, y, ...) always takes the LEFT edge of the string as its origin; CENTER/RIGHT are
      // implemented here, locally, as a one-off x offset computed from the string's own measured width against
      // the element's box width -- nothing in the canvas or layout layer needs to know alignment exists. LEFT
      // (the default for every element created before this) reduces to offset 0.0: pixel-identical to before.
      double alignoffset = 0.0f;
      double textwidth   = canvas->VectorFont_GetWidthStack(faces, nfaces, element_text->GetText()->Get());

      switch(element_text->GetTextAlign())
        {
          case UI_ELEMENT_TYPE_ALIGN_CENTER : alignoffset = (element_text->GetBoundaryLine()->width - textwidth) / 2.0;
                                               break;

          case UI_ELEMENT_TYPE_ALIGN_RIGHT  : alignoffset = element_text->GetBoundaryLine()->width - textwidth;
                                               break;

          default                           : break; // LEFT (and any non-horizontal enum value): no offset.
        }

      canvas->VectorFont_PrintStack(x_position + alignoffset, y_position, faces, nfaces, element_text->GetText()->Get());
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::Draw_TextBox(UI_ELEMENT* element)
* @brief      Draw text box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_TextBox(UI_ELEMENT* element)  
{ 
  if(!element) return false;  

  UI_ELEMENT_TEXTBOX* element_textbox = (UI_ELEMENT_TEXTBOX*)element;
  GRP2DCANVAS*          canvas          = GetCanvas();  
  double              x_position      = 0.0f;
  double              y_position      = 0.0f;  
  XRECT               clip_rect;                       
  
  if(!canvas) return false;
 
  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw()) 
    {
      GRP2DCOLOR_RGBA8                color(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());
      XVECTOR<UI_SKIN_TEXTBOX_PART*>  parts;
      XSTRING                         previousface;

      UI_SKINCANVAS_SelectTextBoxFace(this, canvas, element_textbox, previousface);
                                     
      canvas->Vectorfont_GetConfig()->SetColor(&color);
      canvas->Vectorfont_GetConfig()->SetSize(element_textbox->GetSizeFont());  

      TextBox_GenerateLines(element_textbox, canvas, x_position, y_position, parts);

      for(XDWORD c=0; c<parts.GetSize(); c++)
        {
          UI_SKIN_TEXTBOX_PART* textbox_part = parts.Get(c);
          if(textbox_part)
            {
              if(textbox_part->GetImage())
                {
                  // inline image: bottom on the text baseline (drawn at ypos - height)
                  canvas->PutBitmapAlpha((double)textbox_part->GetXPos(), (double)textbox_part->GetYPos() - (double)textbox_part->GetHeight(), textbox_part->GetImage(), 100.0f);
                }
               else
                {
                  GRP2DCOLOR_RGBA8 color_part(textbox_part->GetColor()->GetRed(), textbox_part->GetColor()->GetGreen(), textbox_part->GetColor()->GetBlue(), textbox_part->GetColor()->GetAlpha());

                  canvas->Vectorfont_GetConfig()->SetColor(&color_part);

                  canvas->VectorFont_Printf(textbox_part->GetXPos(), textbox_part->GetYPos(), textbox_part->GetText()->Get());   


                  #ifdef USERINTERFACE_DEBUG 
                  GRP2DCOLOR_RGBA8  color_debug(255, 0, 255);

                  int width   = (int)canvas->VectorFont_GetWidth(textbox_part->GetText()->Get());
                  int height  = (int)canvas->VectorFont_GetHeight(NL("A"));

                  canvas->SetLineWidth(1.0f);
                  canvas->SetLineColor(&color_debug);
                  canvas->Rectangle(textbox_part->GetXPos(), textbox_part->GetYPos(), textbox_part->GetXPos() + width, textbox_part->GetYPos() - height);             
                  #endif  
                }
            }
        }     
   
      parts.DeleteContents();
      parts.DeleteAll();

      UI_SKINCANVAS_RestoreTextFace(canvas, previousface);
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);
  
  return true;
}


static void UI_SkinCanvas_ConsolePalette(XBYTE colorindex, int& r, int& g, int& b, int& a)
{
  a = 255;
  switch(colorindex)
    {
      case UI_ELEMENT_CONSOLE_COLOR_BLUE   : r =  64; g = 128; b = 255; break;
      case UI_ELEMENT_CONSOLE_COLOR_GREEN  : r =  64; g = 200; b =  64; break;
      case UI_ELEMENT_CONSOLE_COLOR_PURPLE : r = 180; g =  80; b = 200; break;
      case UI_ELEMENT_CONSOLE_COLOR_RED    : r = 230; g =  64; b =  64; break;
      case UI_ELEMENT_CONSOLE_COLOR_GRAY   : r = 160; g = 160; b = 160; break;
      case UI_ELEMENT_CONSOLE_COLOR_BLACK  :
                              default      : r = 220; g = 220; b = 220; break;  // light text on dark console
    }
}


// Largest end > start such that text[start..end) fits in max_text_w (at least one char).
static XDWORD UI_SkinCanvas_ConsoleWrapEnd(GRP2DCANVAS* canvas, XCHAR* text, XDWORD start, XDWORD n, double max_text_w)
{
  if(!canvas || !text || start >= n) return start;

  XSTRING probe;
  probe.Set(&text[start], n - start);
  if(canvas->VectorFont_GetWidth(probe.Get()) <= max_text_w) return n;

  XDWORD lo = 1;
  XDWORD hi = n - start;
  while(lo < hi)
    {
      XDWORD mid = lo + ((hi - lo + 1) / 2);
      probe.Set(&text[start], mid);
      if(canvas->VectorFont_GetWidth(probe.Get()) <= max_text_w) lo = mid;
       else                                                      hi = mid - 1;
    }

  if(lo < 1) lo = 1;
  return start + lo;
}


static XDWORD UI_SkinCanvas_ConsoleMeasureRows(GRP2DCANVAS* canvas, UI_ELEMENT_CONSOLE_LINE* line, bool wordwrap, double max_text_w)
{
  if(!line) return 0;
  if(line->visualrows > 0) return line->visualrows;

  if(!wordwrap || line->text.IsEmpty())
    {
      line->visualrows = 1;
      return 1;
    }

  XCHAR* text = line->text.Get();
  XDWORD n    = line->text.GetSize();
  XDWORD rows = 0;
  XDWORD start = 0;

  while(start < n)
    {
      XDWORD end = UI_SkinCanvas_ConsoleWrapEnd(canvas, text, start, n, max_text_w);
      if(end <= start) end = start + 1;
      rows++;
      start = end;
    }

  if(rows < 1) rows = 1;
  line->visualrows = rows;
  return rows;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::Draw_Console(UI_ELEMENT* element)
* @brief      Draw graphical console (visible lines only)
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Console(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_CONSOLE* element_console = (UI_ELEMENT_CONSOLE*)element;
  GRP2DCANVAS*        canvas          = GetCanvas();
  double              x_position      = 0.0f;
  double              y_position      = 0.0f;
  XRECT               clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      const double pad_x = 8.0f;
      const double pad_y = 6.0f;

      double box_w = element_console->GetBoundaryLine()->width;
      double box_h = element_console->GetBoundaryLine()->height;
      double y_top = UI_BOUNDARYLINE_EdgeTop(y_position, box_h);
      double y_bot = y_top + box_h;

      // Same framed-panel contract as Draw_Form (page seed + RoundRect isfill + corner punch).
      UI_SkinCanvas_DrawFramedPanelBackground(canvas, GetScreen(), element_console, x_position, y_position, box_w, box_h);

      canvas->Vectorfont_GetConfig()->SetSize(element_console->GetSizeFont());

      double fontheight = (double)canvas->VectorFont_GetHeight(NL("A"));
      if(fontheight < 1.0f) fontheight = (double)element_console->GetSizeFont();
      double lineheight = fontheight + (double)element_console->GetLineSpacing();
      if(lineheight < 1.0f) lineheight = 1.0f;

      // Reserve right gutter for the overlay scrollbar so wrapped text stays clear of the thumb.
      double bar_gutter = UI_SKINCANVAS_SCROLLBAR_WIDTH + 4.0f;
      double max_text_w = box_w - (pad_x * 2.0f) - bar_gutter;
      if(max_text_w < 8.0f) max_text_w = 8.0f;

      // Invalidate per-line wrap cache when measure width or font changes.
      if((element_console->GetLayoutMaxTextWidth() != max_text_w) ||
         (element_console->GetLayoutSizeFont() != element_console->GetSizeFont()))
        {
          element_console->InvalidateLayoutCache();
          element_console->SetLayoutMaxTextWidth(max_text_w);
          element_console->SetLayoutSizeFont(element_console->GetSizeFont());
        }

      bool   wordwrap   = element_console->IsWordWrap();
      XDWORD visualrows = 0;
      XDWORD linecount  = element_console->GetLineCount();
      double max_line_w = 0.0f;

      if(!wordwrap)
        {
          visualrows = linecount;   // O(1): one visual row per buffer line
          for(XDWORD i=0; i<linecount; i++)
            {
              UI_ELEMENT_CONSOLE_LINE* line = element_console->GetLine(i);
              if(!line || line->text.IsEmpty()) continue;
              double w = (double)canvas->VectorFont_GetWidth(line->text.Get());
              if(w > max_line_w) max_line_w = w;
            }
        }
       else
        {
          // Only uncached (new) lines call the font measurer; older lines reuse visualrows.
          for(XDWORD i=0; i<linecount; i++)
            {
              visualrows += UI_SkinCanvas_ConsoleMeasureRows(canvas, element_console->GetLine(i), true, max_text_w);
            }
        }

      // Content height includes vertical padding so scroll can frame first/last lines.
      element_console->SetContentHeight((double)visualrows * lineheight + (pad_y * 2.0f));

      // Horizontal extent: only meaningful when word-wrap is off (long lines can exceed the box).
      double content_w = box_w;
      if(!wordwrap)
        {
          content_w = max_line_w + (pad_x * 2.0f) + bar_gutter;
          if(content_w < box_w) content_w = box_w;
        }
      element_console->SetContentWidth(content_w);
      element_console->SetLayoutMetrics(pad_x, pad_y, lineheight, fontheight);

      UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element_console);
      if(scrolleable)
        {
          // Resolve extent first so displacement/limit reads see an active axis.
          ResolveScrollPolicy(element_console, scrolleable);

          double lim = scrolleable->Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);

          // ON  = pin to live tail (SetFraction 1).
          // OFF = leave displacement where it is. As content grows, lim moves more negative while
          //       disp stays put → the viewport freezes. Front-trims under maxlines_off are
          //       compensated so the ring buffer does not slide under a fixed offset.
          bool follow = element_console->GetAutoscroll();
          double disp = scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);
          XDWORD trimmed = element_console->ConsumeTrimmedRows();

          if(!follow)
            {
              if(trimmed > 0)
                {
                  disp += (double)trimmed * lineheight;
                  if(disp > 0.0f) disp = 0.0f;
                  // Keep line-aligned after trim so a sliced glyph cannot stick under the top edge.
                  if(lineheight > 0.0f)
                    {
                      double rows = -disp / lineheight;
                      disp = -floor(rows + 1e-9) * lineheight;
                      if(disp > 0.0f) disp = 0.0f;
                    }
                  if(lim < 0.0f && disp < lim) disp = lim;
                  scrolleable->Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, disp);
                }
              element_console->ConsumeAutoscrollPending();
            }
           else if(lim < 0.0f)
            {
              element_console->ConsumeAutoscrollPending();
              // Live tail, but line-aligned within [lim, 0]. Using raw SetFraction(1)=lim often
              // leaves a fractional row at the top (half-glyphs that look like junk).
              double rows = -lim / lineheight;
              disp = -floor(rows + 1e-9) * lineheight;
              if(disp > 0.0f) disp = 0.0f;
              if(disp < lim) disp = lim;
              scrolleable->Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, disp);
            }
           else
            {
              element_console->ConsumeAutoscrollPending();
            }
        }

      double x_text = x_position + pad_x
                      + element_console->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);
      double y_text = y_top + pad_y + fontheight
                      + element_console->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);

      // Visible window: glyph ascenders often exceed GetHeight("A"), so use a taller
      // estimate + pad wipe below. Loose y_vis_hi used to leave a sliced bottom row too.
      double glyph_asc = (double)canvas->VectorFont_GetHeight(NL("Ay[]|M"));
      if(glyph_asc < fontheight) glyph_asc = fontheight;
      glyph_asc += 1.0; // AA bleed above the measured box
      double y_vis_lo = y_top + pad_y;
      double y_vis_hi = y_bot - pad_y;

      XDWORD row = 0;
      for(XDWORD i=0; i<linecount; i++)
        {
          UI_ELEMENT_CONSOLE_LINE* line = element_console->GetLine(i);
          if(!line) continue;

          XDWORD rows_here = wordwrap ? UI_SkinCanvas_ConsoleMeasureRows(canvas, line, true, max_text_w) : 1;
          double y_block_lo = y_text + (double)row * lineheight;
          double y_block_hi = y_text + (double)(row + rows_here - 1) * lineheight;

          if(y_block_hi < y_vis_lo || y_block_lo > y_vis_hi)
            {
              row += rows_here;
              continue;
            }

          if(element_console->Selection_IsLineSelected(i))
            {
              double sel_top = y_text + (double)row * lineheight - fontheight;
              double sel_bot = sel_top + (double)rows_here * lineheight;
              if(sel_top < y_top + pad_y) sel_top = y_top + pad_y;
              if(sel_bot > y_bot - pad_y) sel_bot = y_bot - pad_y;
              // Stay in the text band: full-width wash bled into the scrollbar gutter/border
              // and left blue "marks" on the side when the overlay bar did not cover every pixel.
              double sel_left  = x_position + pad_x;
              double sel_right = x_position + box_w - bar_gutter;
              if(sel_bot > sel_top && sel_right > sel_left)
                {
                  GRP2DCOLOR_RGBA8 selcolor(50, 90, 160, 110);
                  canvas->SetLineWidth(0.0f);
                  canvas->SetLineColor(&selcolor);
                  canvas->SetFillColor(&selcolor);
                  canvas->Rectangle(sel_left, sel_bot, sel_right, sel_top, true);
                }
            }

          int r = 220, g = 220, b = 220, a = 255;
          if(line->colorindex == UI_ELEMENT_CONSOLE_COLOR_RGB && line->rgb.IsValid())
            {
              r = line->rgb.GetRed();
              g = line->rgb.GetGreen();
              b = line->rgb.GetBlue();
              a = line->rgb.GetAlpha();
              if(a <= 0) a = 255;
            }
           else
            {
              UI_SkinCanvas_ConsolePalette(line->colorindex, r, g, b, a);
            }

          GRP2DCOLOR_RGBA8 color_line(r, g, b, a);
          canvas->Vectorfont_GetConfig()->SetColor(&color_line);

          if(!wordwrap || line->text.IsEmpty())
            {
              double y = y_text + (double)row * lineheight;
              // Baseline must sit in the pad-framed band. Requiring (y - glyph_asc) >= y_vis_lo
              // culled the first row whenever glyph_asc > fontheight (XTraceMonitorGEN lost the
              // green ready line). Pad wipe still clears ascender crumbs in the top pad.
              if(y >= y_vis_lo && y <= y_vis_hi)
                {
                  canvas->VectorFont_Printf(x_text, y, line->text.IsEmpty() ? NL("") : line->text.Get());
                }
              row++;
              continue;
            }

          XDWORD start = 0;
          XDWORD n     = line->text.GetSize();
          XCHAR* text  = line->text.Get();
          while(start < n)
            {
              XDWORD end = UI_SkinCanvas_ConsoleWrapEnd(canvas, text, start, n, max_text_w);
              if(end <= start) end = start + 1;

              double y = y_text + (double)row * lineheight;
              if(y >= y_vis_lo && y <= y_vis_hi)
                {
                  XSTRING chunk;
                  chunk.Set(&text[start], end - start);
                  canvas->VectorFont_Printf(x_text, y, chunk.Get());
                }

              row++;
              start = end;
            }
        }

      // Cover ascender/AA crumbs in the vertical padding bands. Stay strictly inside the RoundRect
      // *inner* fill (lw inset + corner radius) — a sharp AABB wipe from x+bw/y+bw erases the border
      // ring that AGG leaves in the corner AABB tips (same pixels Draw_Form keeps), and the bottom
      // wipe used to include y_bot-bw which is the bottom border row itself.
      if(element_console->IsBackgroundColorSet() && element_console->GetBackgroundColor()->GetAlpha() > 0)
        {
          UI_COLOR* bg = element_console->GetBackgroundColor();
          GRP2DCOLOR_RGBA8 bkgcolor(bg->GetRed(), bg->GetGreen(), bg->GetBlue(), bg->GetAlpha());
          canvas->SetLineWidth(0.0f);
          canvas->SetLineColor(&bkgcolor);
          canvas->SetFillColor(&bkgcolor);
          if(pad_y > 0.0)
            {
              double bw_i = (element_console->IsBorderColorSet() && element_console->GetBorderWidth() > 0.0f)
                            ? element_console->GetBorderWidth() : 0.0;
              double rr   = (double)element_console->GetRoundRect();
              double r_in = (rr > bw_i) ? (rr - bw_i) : 0.0;
              // Horizontal: clear of corner border tips. Vertical: leave the border row alone.
              double x0 = x_position + bw_i + r_in;
              double x1 = x_position + box_w - bw_i - r_in;
              if(x1 > x0)
                {
                  double y_top_wipe0 = y_top + bw_i;                 // first interior scanline
                  double y_top_wipe1 = y_top + pad_y;
                  double y_bot_wipe0 = y_bot - pad_y;
                  double y_bot_wipe1 = y_bot - bw_i - ((bw_i > 0.0) ? 1.0 : 0.0); // exclude border row
                  if(y_top_wipe1 > y_top_wipe0)
                    canvas->Rectangle(x0, y_top_wipe1, x1, y_top_wipe0, true);
                  if(y_bot_wipe1 > y_bot_wipe0)
                    canvas->Rectangle(x0, y_bot_wipe1, x1, y_bot_wipe0, true);
                }
            }
        }
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


// Sets canvas clip box = parent clip INTERSECT (left, top, right, bottom) given in top-down design px.
static void UI_SkinCanvas_PropertyListClip(GRP2DCANVAS* canvas, XRECT& parent, double left, double top, double right, double bottom)
{
  double p_minx = __MIN(parent.x1, parent.x2);
  double p_maxx = __MAX(parent.x1, parent.x2);
  double p_miny = __MIN(parent.y1, parent.y2);
  double p_maxy = __MAX(parent.y1, parent.y2);

  double l = __MAX(left  , p_minx);
  double r = __MIN(right , p_maxx);
  double t = __MAX(top   , p_miny);
  double b = __MIN(bottom, p_maxy);

  if(r < l) r = l;
  if(b < t) b = t;

  canvas->SetClipBox(l, b, r, t);                                              // (left, bottom, right, top)
}


// Filled 1px-granular rectangle (x1,y1 top-left; x2,y2 bottom-right, top-down design px) in the current fill colour.
static void UI_SkinCanvas_TreeRect(GRP2DCANVAS* canvas, double x1, double y1, double x2, double y2)
{
  canvas->Rectangle(x1, y2, x2, y1, true);                                     // canvas wants (x1, bottom, x2, top)
}


// Dotted vertical guide at column x (1px dots every 2px, phase locked to the absolute y so rows join seamlessly).
static void UI_SkinCanvas_TreeDotsV(GRP2DCANVAS* canvas, double x, double y1, double y2)
{
  int ya = (int)(y1 + 0.5f);
  int yb = (int)(y2 + 0.5f);
  if(ya & 1) ya++;

  for(int y=ya; y<yb; y+=2) UI_SkinCanvas_TreeRect(canvas, x, (double)y, x + 1.0f, (double)(y + 1));
}


// Dotted horizontal guide at row y.
static void UI_SkinCanvas_TreeDotsH(GRP2DCANVAS* canvas, double y, double x1, double x2)
{
  int xa = (int)(x1 + 0.5f);
  int xb = (int)(x2 + 0.5f);
  if(xa & 1) xa++;

  for(int x=xa; x<xb; x+=2) UI_SkinCanvas_TreeRect(canvas, (double)x, y, (double)(x + 1), y + 1.0f);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::Draw_Tree(UI_ELEMENT* element)
* @brief      Draw tree (backdrop, dotted guides, [+]/[-] boxes, labels, selection, scroll)
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Tree(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_TREE* element_tree = (UI_ELEMENT_TREE*)element;
  GRP2DCANVAS*     canvas       = GetCanvas();
  double           x_position   = 0.0f;
  double           y_position   = 0.0f;
  XRECT            clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      double box_w = element_tree->GetBoundaryLine()->width;
      double box_h = element_tree->GetBoundaryLine()->height;
      double y_top = UI_BOUNDARYLINE_EdgeTop(y_position, box_h);
      double y_bot = y_top + box_h;

      bool has_border = element_tree->IsBorderColorSet() && (element_tree->GetBorderWidth() > 0.0f);
      double bw       = has_border ? element_tree->GetBorderWidth() : 0.0;

      GRP2DCOLOR_RGBA8 color_back(12, 14, 18, 255);
      if(element_tree->IsBackgroundColorSet() && (element_tree->GetBackgroundColor()->GetAlpha() > 0))
        {
          UI_COLOR* bg = element_tree->GetBackgroundColor();
          color_back = GRP2DCOLOR_RGBA8(bg->GetRed(), bg->GetGreen(), bg->GetBlue(), 255);
        }

      // Same framed-panel contract as Draw_Form (page seed + RoundRect isfill + corner punch).
      UI_SkinCanvas_DrawFramedPanelBackground(canvas, GetScreen(), element_tree, x_position, y_position, box_w, box_h);

      canvas->Vectorfont_GetConfig()->SetSize(element_tree->GetSizeFont());

      double fontheight = (double)canvas->VectorFont_GetHeight(NL("A"));
      if(fontheight < 1.0f) fontheight = (double)element_tree->GetSizeFont();

      double rowheight   = element_tree->GetRowHeight();
      double indentwidth = element_tree->GetIndentWidth();
      XDWORD rowcount    = element_tree->GetVisibleRowCount();

      double bar_gutter  = UI_SKINCANVAS_SCROLLBAR_WIDTH + 4.0f;

      element_tree->SetContentHeight((UI_ELEMENT_TREE_PADDING_Y * 2.0f) + ((double)rowcount * rowheight));

      // Horizontal extent: deepest/longest visible label (pad + indent + optional icon + text).
      {
        double max_right = UI_ELEMENT_TREE_PADDING_X;

        for(XDWORD i=0; i<rowcount; i++)
          {
            UI_ELEMENT_TREE_NODE* node = element_tree->GetVisibleNode(i);
            if(!node) continue;

            double text_x = element_tree->GetTextX(node);
            double text_w = node->text.IsEmpty() ? 0.0f : (double)canvas->VectorFont_GetWidth(node->text.Get());
            double right  = text_x + text_w + UI_ELEMENT_TREE_PADDING_X;

            if(right > max_right) max_right = right;
          }

        element_tree->SetContentWidth(max_right + bar_gutter);
      }

      UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element_tree);
      if(scrolleable)
        {
          ResolveScrollPolicy(element_tree, scrolleable);

          for(int axis=0; axis<2; axis++)
            {
              UI_PROPERTY_SCROLLEABLE_TYPE type = (axis == 0) ? UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL
                                                             : UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL;
              double lim  = scrolleable->Scroll_GetLimit(type);
              double disp = scrolleable->Scroll_GetDisplacement(type);
              if(lim >= 0.0f && disp != 0.0f)
                {
                  scrolleable->Scroll_SetDisplacement(type, 0.0f);
                }
               else if(lim < 0.0f && disp < lim)
                {
                  scrolleable->Scroll_SetDisplacement(type, lim);
                }
            }
        }

      double vdisp = (double)((int)element_tree->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL));
      double hdisp = (double)((int)element_tree->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL));

      // Ink / lines.
      int ink_r = 200, ink_g = 208, ink_b = 220, ink_a = 255;
      if(element_tree->IsColorSet() && element_tree->GetColor()->IsValid())
        {
          ink_r = element_tree->GetColor()->GetRed();
          ink_g = element_tree->GetColor()->GetGreen();
          ink_b = element_tree->GetColor()->GetBlue();
          ink_a = element_tree->GetColor()->GetAlpha();
          if(ink_a <= 0) ink_a = 255;
        }

      GRP2DCOLOR_RGBA8 color_ink(ink_r, ink_g, ink_b, ink_a);
      GRP2DCOLOR_RGBA8 color_guide(104, 114, 130, 255);
      GRP2DCOLOR_RGBA8 color_boxline(150, 160, 176, 255);
      GRP2DCOLOR_RGBA8 color_boxsign(210, 218, 230, 255);
      GRP2DCOLOR_RGBA8 color_select(40, 50, 70, 255);

      double x_left  = x_position + bw;
      double x_right = x_position + box_w - bw;

      XRECT parentclip;
      canvas->GetClipBox(parentclip);

      double y_vis_top = y_top + bw;
      double y_vis_bot = y_bot - bw;
      double bar_bottom = UI_SKINCANVAS_SCROLLBAR_WIDTH + 4.0f;

      UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left, y_vis_top, x_right - bar_gutter, y_vis_bot - bar_bottom);

      canvas->SetLineWidth(0.0f);

      UI_ELEMENT_TREE_NODE* selected = element_tree->GetSelected();

      for(XDWORD i=0; i<rowcount; i++)
        {
          UI_ELEMENT_TREE_NODE* node = element_tree->GetVisibleNode(i);
          if(!node) continue;

          double row_top = (double)((int)(y_top + UI_ELEMENT_TREE_PADDING_Y + ((double)i * rowheight) + vdisp + 0.5f));
          double row_bot = row_top + rowheight;

          if(row_bot < y_vis_top || row_top > (y_vis_bot - bar_bottom)) continue;

          XDWORD depth    = element_tree->GetNodeDepth(node);
          double row_mid  = (double)((int)(row_top + (rowheight / 2.0f)));
          double label_x  = x_position + element_tree->GetLabelX(depth) + hdisp;
          double text_x   = x_position + element_tree->GetTextX(node) + hdisp;
          double icon_sz  = element_tree->GetIconSize();
          double cx       = (double)((int)(x_position + element_tree->GetBoxCenterX(depth) + hdisp + 0.5f));
          double half     = (double)((int)(UI_ELEMENT_TREE_BOXSIZE / 2.0f));

          // Selection wash behind icon + label.
          if(node == selected)
            {
              double text_w = node->text.IsEmpty() ? 0.0f : (double)canvas->VectorFont_GetWidth(node->text.Get());
              double sel_r  = text_x + text_w + 4.0f;

              canvas->SetLineColor(&color_select);
              canvas->SetFillColor(&color_select);
              UI_SkinCanvas_TreeRect(canvas, label_x - 3.0f, row_top + 1.0f, sel_r, row_bot - 1.0f);
            }

          // Dotted guides.
          canvas->SetLineColor(&color_guide);
          canvas->SetFillColor(&color_guide);

          if(depth > 0)
            {
              XVECTOR<UI_ELEMENT_TREE_NODE*>* siblings = node->father ? &node->father->children : element_tree->GetRoots();
              bool                            haslater = (siblings->Find(node) < ((int)siblings->GetSize() - 1));
              double                          px       = (double)((int)(x_position + element_tree->GetBoxCenterX(depth - 1) + hdisp + 0.5f));

              // Own connector: from the parent's column, down to the middle, continuing if a later sibling exists.
              UI_SkinCanvas_TreeDotsV(canvas, px, row_top, haslater ? row_bot : row_mid);
              UI_SkinCanvas_TreeDotsH(canvas, row_mid, px, node->HasChildren() ? (cx - half) : (label_x - 3.0f));

              // Ancestor columns whose branch continues below this row.
              // An ancestor at depth a (>= 1) owns a connector column a-1.
              UI_ELEMENT_TREE_NODE* ancestor = node->father;
              XDWORD                a        = depth - 1;
              while(ancestor && ancestor->father && (a >= 1))
                {
                  XVECTOR<UI_ELEMENT_TREE_NODE*>* asib = &ancestor->father->children;
                  bool                            alat = (asib->Find(ancestor) < ((int)asib->GetSize() - 1));

                  if(alat)
                    {
                      double ax = (double)((int)(x_position + element_tree->GetBoxCenterX(a - 1) + hdisp + 0.5f));
                      UI_SkinCanvas_TreeDotsV(canvas, ax, row_top, row_bot);
                    }

                  ancestor = ancestor->father;
                  a--;
                }
            }

          if(node->HasChildren())
            {
              // Line from the box down to the children's connector.
              if(node->expanded)
                {
                  canvas->SetLineColor(&color_guide);
                  canvas->SetFillColor(&color_guide);
                  UI_SkinCanvas_TreeDotsV(canvas, cx, row_mid + half + 1.0f, row_bot);
                }

              // [+] / [-] box (9x9).
              double bx1 = cx - half;
              double by1 = row_mid - half;
              double bx2 = cx + half + 1.0f;
              double by2 = row_mid + half + 1.0f;

              canvas->SetLineColor(&color_boxline);
              canvas->SetFillColor(&color_boxline);
              UI_SkinCanvas_TreeRect(canvas, bx1, by1, bx2, by2);

              canvas->SetLineColor(&color_back);
              canvas->SetFillColor(&color_back);
              UI_SkinCanvas_TreeRect(canvas, bx1 + 1.0f, by1 + 1.0f, bx2 - 1.0f, by2 - 1.0f);

              canvas->SetLineColor(&color_boxsign);
              canvas->SetFillColor(&color_boxsign);
              UI_SkinCanvas_TreeRect(canvas, cx - 2.0f, row_mid, cx + 3.0f, row_mid + 1.0f);                        // '-'

              if(!node->expanded)
                {
                  UI_SkinCanvas_TreeRect(canvas, cx, row_mid - 2.0f, cx + 1.0f, row_mid + 3.0f);                    // '|' of '+'
                }
            }

          // Optional node icon (height = sizefont, square). Drawn before the label text.
          if(node->HasIcon())
            {
              GRPPROPERTYMODE mode = canvas->GetMode();
              UI_ANIMATION*   anim = GEN_USERINTERFACE.GetOrAddAnimationCache(UI_SKIN_DRAWMODE_CANVAS, mode, NL(""), node->GetIcon()->Get(), canvas, icon_sz, icon_sz);
              GRPBITMAP*      bmp  = anim ? anim->GetBitmap() : NULL;

              if(bmp)
                {
                  double icon_top = row_mid - ((double)bmp->GetHeight() / 2.0f);
                  // Soft-edge safe blit (cover=255), same rule as Draw_Image aqua PNGs.
                  for(XDWORD yb = 0; yb < bmp->GetHeight(); yb++)
                    {
                      for(XDWORD xb = 0; xb < bmp->GetWidth(); xb++)
                        {
                          GRP2DCOLOR* raw = bmp->GetPixel((int)xb, (int)yb);
                          if(!raw) continue;

                          GRP2DCOLOR_RGBA8* px = (GRP2DCOLOR_RGBA8*)raw;
                          if(!px->a) continue;

                          GRP2DCOLOR_RGBA8 c(px->r, px->g, px->b, px->a);
                          canvas->PutBlendPixel(label_x + (double)xb, icon_top + (double)yb, &c, 255.0);
                        }
                    }
                }
            }

          // Label.
          if(!node->text.IsEmpty())
            {
              double baseline = row_top + ((rowheight + fontheight) / 2.0f) - 1.0f;

              canvas->Vectorfont_GetConfig()->SetColor(&color_ink);
              canvas->VectorFont_Print(text_x, baseline, node->text.Get());
            }
        }

      canvas->SetClipBox(parentclip);
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::Draw_PropertyList(UI_ELEMENT* element)
* @brief      Draw property list (header, scrollable Name/Value rows, column splitter)
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_PropertyList(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_PROPERTYLIST* element_list = (UI_ELEMENT_PROPERTYLIST*)element;
  GRP2DCANVAS*             canvas       = GetCanvas();
  double                   x_position   = 0.0f;
  double                   y_position   = 0.0f;
  XRECT                    clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      const double pad_x = 8.0f;

      double box_w = element_list->GetBoundaryLine()->width;
      double box_h = element_list->GetBoundaryLine()->height;
      double y_top = UI_BOUNDARYLINE_EdgeTop(y_position, box_h);
      double y_bot = y_top + box_h;

      bool has_border = element_list->IsBorderColorSet() && (element_list->GetBorderWidth() > 0.0f);
      double bw       = has_border ? element_list->GetBorderWidth() : 0.0;

      // Same framed-panel contract as Draw_Form (page seed + RoundRect isfill + corner punch).
      UI_SkinCanvas_DrawFramedPanelBackground(canvas, GetScreen(), element_list, x_position, y_position, box_w, box_h);

      canvas->Vectorfont_GetConfig()->SetSize(element_list->GetSizeFont());

      double fontheight = (double)canvas->VectorFont_GetHeight(NL("A"));
      if(fontheight < 1.0f) fontheight = (double)element_list->GetSizeFont();

      double rowheight    = element_list->GetRowHeight();
      double headerheight = element_list->GetHeaderHeight();
      double namewidth    = element_list->GetNameColumnWidth();
      XDWORD rowcount     = element_list->GetRowCount();

      // Keep the stored splitter inside [min, box_w - min] once the real box width is known.
      element_list->SetNameColumnWidth(namewidth);
      namewidth = element_list->GetNameColumnWidth();

      double bar_gutter = UI_SKINCANVAS_SCROLLBAR_WIDTH + 4.0f;

      // Scroll extent is the variable rows only. The header band is sticky (drawn without vdisp) and is
      // excluded from the vertical viewport / scrollbar track via GetScrollContentBox.
      element_list->SetContentHeight(((double)rowcount * rowheight) + 4.0f);

      UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element_list);
      if(scrolleable)
        {
          ResolveScrollPolicy(element_list, scrolleable);

          double lim  = scrolleable->Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);
          double disp = scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);
          if(lim >= 0.0f && disp != 0.0f)
            {
              scrolleable->Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 0.0f);
            }
           else if(lim < 0.0f && disp < lim)
            {
              scrolleable->Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, lim);
            }
        }

      double vdisp = element_list->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);

      // Ink / lines.
      int ink_r = 200, ink_g = 208, ink_b = 220, ink_a = 255;
      if(element_list->IsColorSet() && element_list->GetColor()->IsValid())
        {
          ink_r = element_list->GetColor()->GetRed();
          ink_g = element_list->GetColor()->GetGreen();
          ink_b = element_list->GetColor()->GetBlue();
          ink_a = element_list->GetColor()->GetAlpha();
          if(ink_a <= 0) ink_a = 255;
        }

      GRP2DCOLOR_RGBA8 color_ink(ink_r, ink_g, ink_b, ink_a);
      GRP2DCOLOR_RGBA8 color_line(50, 58, 70, 255);
      if(has_border)
        {
          UI_COLOR* lc = element_list->GetBorderColor();
          color_line = GRP2DCOLOR_RGBA8(lc->GetRed(), lc->GetGreen(), lc->GetBlue(), 255);
        }

      GRP2DCOLOR_RGBA8 color_header(26, 30, 40, 255);
      GRP2DCOLOR_RGBA8 color_rowline(34, 40, 50, 255);

      double x_left   = x_position + bw;
      double x_right  = x_position + box_w - bw;
      // Inclusive Rectangle fills cover x2: keep the right border column (x_right when bw>0) unpainted so the
      // frame line survives header / row / splitter plates (same visible right edge as TREE / FORM).
      double x_fill_r = (bw > 0.0) ? (x_right - 1.0) : x_right;
      if(x_fill_r < x_left) x_fill_r = x_left;
      double x_split  = x_position + namewidth;
      double y_body   = y_top + headerheight;
      double y_header_top = y_top + bw;

      XRECT parentclip;
      canvas->GetClipBox(parentclip);

      // ---- Body rows (clipped under the header, inside the border, per column) ------------------------------------
      {
        double y_vis_top = y_body;
        double y_vis_bot = y_bot - bw;

        for(int pass=0; pass<2; pass++)
          {
            if(pass == 0) UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left , y_vis_top, x_split - 4.0f           , y_vis_bot);
             else         UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_split + 4.0f, y_vis_top, x_right - bar_gutter, y_vis_bot);

            canvas->Vectorfont_GetConfig()->SetColor(&color_ink);

            for(XDWORD i=0; i<rowcount; i++)
              {
                UI_ELEMENT_PROPERTYLIST_ROW* row = element_list->GetRow(i);
                if(!row) continue;

                double row_top = y_body + ((double)i * rowheight) + vdisp;
                double row_bot = row_top + rowheight;

                if(row_bot < y_vis_top || row_top > y_vis_bot) continue;

                double baseline = row_top + ((rowheight + fontheight) / 2.0f) - 1.0f;
                XSTRING* text   = (pass == 0) ? &row->name : &row->value;
                double   tx     = (pass == 0) ? (x_left + pad_x) : (x_split + pad_x);

                if(!text->IsEmpty()) canvas->VectorFont_Printf(tx, baseline, text->Get());
              }
          }

        // Horizontal separators between rows (full width, under the splitter).
        UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left, y_vis_top, x_fill_r, y_vis_bot);

        canvas->SetLineWidth(0.0f);
        canvas->SetLineColor(&color_rowline);
        canvas->SetFillColor(&color_rowline);

        for(XDWORD i=0; i<rowcount; i++)
          {
            double line_y = y_body + ((double)(i + 1) * rowheight) + vdisp;
            if(line_y < y_vis_top || line_y > y_vis_bot) continue;

            canvas->Rectangle(x_left, line_y + 1.0f, x_fill_r, line_y, true);
          }
      }

      // ---- Header band (painted after rows; body clip already kept rows out of it) -----------------------------------
      // Leave the top-left/right r_in×r_in AABB tips untouched so the panel RoundRect border-ring pixels
      // survive (same stepped corner as FORM/opstatus). Never paint x_fill_r past the right border column.
      UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left, y_header_top, x_fill_r, y_body);

      canvas->SetLineWidth(0.0f);
      canvas->SetLineColor(&color_header);
      canvas->SetFillColor(&color_header);
      {
        double r_in = (double)element_list->GetRoundRect() - bw;
        if(r_in < 0.0) r_in = 0.0;
        double y_below = y_header_top + r_in;
        if(y_below > y_body) y_below = y_body;

        if(y_body > y_below)
          canvas->Rectangle(x_left, y_body, x_fill_r, y_below, true);

        if((r_in > 0.0) && (y_below > y_header_top) && ((x_fill_r - r_in) > (x_left + r_in)))
          canvas->Rectangle(x_left + r_in, y_below, x_fill_r - r_in, y_header_top, true);
         else if(r_in <= 0.0)
          canvas->Rectangle(x_left, y_body, x_fill_r, y_header_top, true);
      }

      canvas->SetLineColor(&color_line);
      canvas->SetFillColor(&color_line);
      canvas->Rectangle(x_left, y_body + 1.0f, x_fill_r, y_body, true);

      {
        UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left, y_header_top, x_split - 4.0f, y_body);
        canvas->Vectorfont_GetConfig()->SetColor(&color_ink);

        double baseline = y_top + ((headerheight + fontheight) / 2.0f) - 1.0f;

        if(!element_list->GetHeaderName()->IsEmpty())  canvas->VectorFont_Printf(x_left + pad_x, baseline, element_list->GetHeaderName()->Get());

        UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_split + 4.0f, y_header_top, x_right - bar_gutter, y_body);
        if(!element_list->GetHeaderValue()->IsEmpty()) canvas->VectorFont_Printf(x_split + pad_x, baseline, element_list->GetHeaderValue()->Get());
      }

      // ---- Column splitter (header + body, inside the border) ---------------------------------------------------------
      UI_SkinCanvas_PropertyListClip(canvas, parentclip, x_left, y_header_top, x_fill_r, y_bot - bw);

      canvas->SetLineWidth(0.0f);
      canvas->SetLineColor(&color_line);
      canvas->SetFillColor(&color_line);
      canvas->Rectangle(x_split - 1.0f, y_bot - bw, x_split + 1.0f, y_header_top, true);

      canvas->SetClipBox(parentclip);
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Image(UI_ELEMENT* element)
* @brief      Draw image
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Image(UI_ELEMENT* element)  
{
  if(!element) return false;
 
  UI_ELEMENT_IMAGE*   element_image = (UI_ELEMENT_IMAGE*)element;
  GRP2DCANVAS*          canvas        = GetCanvas(); 
  double              x_position    = 0.0f;
  double              y_position    = 0.0f;
  XRECT               clip_rect;                       

  if(!element_image->GetImage()) return false; 
  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      // Box-shadow (see UI_SKINCANVAS::DrawElementBoxShadow): drawn before the bitmap, same "shadow first, own
      // content on top" ordering Draw_Form uses. A no-op when the element does not declare box-shadow, so this
      // is zero behaviour change for every existing image layout.
      DrawElementBoxShadow(canvas, element, x_position, y_position);

      // Soft-edge PNGs (aqua bubble toolbars): do NOT use PutBitmapAlpha here. That helper feeds the pixel's
      // alpha as AGG cover, which multiplies alpha again and crushes translucent glass to invisible. Same
      // cover=255 rule as UI_SkinCanvas_CompositeSoftShadowBitmap.
      GRPBITMAP* bm = element_image->GetImage();
      double     dx = x_position;
      double     dy = y_position - (double)bm->GetHeight();
      double     fade = (double)element_image->GetAlpha();
      if(fade > 0.0)
        {
          for(XDWORD yb = 0; yb < bm->GetHeight(); yb++)
            {
              for(XDWORD xb = 0; xb < bm->GetWidth(); xb++)
                {
                  GRP2DCOLOR* raw = bm->GetPixel((int)xb, (int)yb);
                  if(!raw) continue;

                  GRP2DCOLOR_RGBA8* px = (GRP2DCOLOR_RGBA8*)raw;
                  if(!px->a) continue;

                  XBYTE a = px->a;
                  if(fade < 100.0) a = (XBYTE)((fade * (double)a) / 100.0);
                  if(!a) continue;

                  GRP2DCOLOR_RGBA8 c(px->r, px->g, px->b, a);
                  canvas->PutBlendPixel(dx + (double)xb, dy + (double)yb, &c, 255.0);
                }
            }
        }
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_StatisticsChart(UI_ELEMENT* element)
* @brief      Draw statistics chart
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_StatisticsChart(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_STATISTICSCHART* element_chart = (UI_ELEMENT_STATISTICSCHART*)element;
  GRP2DCANVAS*                canvas       = GetCanvas();
  double                      x_position   = 0.0f;
  double                      y_position   = 0.0f;
  XRECT                       clip_rect;

  if(!canvas) return false;

  if(element_chart->GetNeedsRebuild() || (!element_chart->GetBitmap()))
    {
      if(element_chart->HasData())
        {
          element_chart->RebuildBitmap(canvas);
        }
       else
        {
          // Phase 2 empty state: no samples yet -- keep the card blank (no stale bitmap).
          PreDrawFunction(element, canvas, clip_rect, x_position, y_position);
          PostDrawFunction(element, canvas, clip_rect, x_position, y_position);
          return true;
        }
    }

  if(!element_chart->GetBitmap()) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      DrawElementBoxShadow(canvas, element, x_position, y_position);

      canvas->PutBitmapAlpha(x_position, y_position - element_chart->GetBitmap()->GetHeight(), element_chart->GetBitmap(), element_chart->GetAlpha());
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Animation(UI_ELEMENT* element)
* @brief      Draw animation
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Animation(UI_ELEMENT* element)  
{
  if(!element) return false;

  UI_ELEMENT_ANIMATION* element_animation  = (UI_ELEMENT_ANIMATION*)element;  
  UI_ELEMENT_IMAGE*     element_image      = (UI_ELEMENT_IMAGE*)element_animation->GetComposeElements()->Get(element_animation->IndexImage_Get());
  double                x_position         = 0.0f;
  double                y_position         = 0.0f;  
  GRP2DCANVAS*            canvas             = GetCanvas();    
  XRECT                 clip_rect;          
  bool                  redraw             = false;

  if(!element_animation)  return false;
  if(!element_image)      return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw()) 
    {
      // ANIM ICON GHOSTING FIX (2026-10): same orphan-discard + alpha-blend stack as Draw_Text — idle
      // rebuild areas are deleted without restore, so the next MustReDraw (menu select overlap chain)
      // PutBitmapAlpha's the icon onto itself and section headers go progressively blacker
      // (UI_Options ic_list next to "GALLERYS EXISTENTES"). Mirror TextBackdrop: restore then recapture.
      //
      // OPAQUE-PARENT WASH (2026-10): when the father option/button just painted an opaque selection
      // wash (UI_Options gal_*/nav_* exclusive gray/white), a stale AnimBackdrop captured on the
      // previous wash colour must NOT be PutBitmapNoAlpha'd — that punches a white icon plate into
      // the gray selection bar (and vice versa on deselect). Drop without restore and re-capture
      // whatever the parent wash left under us.
      //
      // CHROME APP ICON (role=icon): never keep an AnimBackdrop. After UpdateSize wipe, a backdrop
      // captured on pure black (or a compounded caption fill) is PutBitmapNoAlpha'd under the PNG's
      // translucent corners as a permanent black plate. Soft-blend the icon onto the live caption.
      bool chrome_app_icon = (element->GetChromeRole() == UI_ELEMENT_CHROMEROLE_ICON);
      for(UI_ELEMENT* walk = element->GetFather(); !chrome_app_icon && walk; walk = walk->GetFather())
        {
          if(walk->GetChromeRole() == UI_ELEMENT_CHROMEROLE_ICON)
            {
              chrome_app_icon = true;
              break;
            }
        }

      if(!GEN_USERINTERFACE.ModalLayer_IsCompositing())
        {
          if(chrome_app_icon)
            {
              AnimBackdrop_Delete(element);

              // Soft-edge PNG must not blend onto wiped pure black after UpdateSize. Caption is already
              // painted under us; only replace near-black pixels (stale plate) with the live caption
              // sample so AA corners match the bar — do NOT opaque-fill the whole AABB (that erased
              // the translucent caption wash and left a lighter square under the icon).
              if(canvas && element_image->GetImage())
                {
                  double ih = (double)element_image->GetImage()->GetHeight();
                  double iw = (double)element_image->GetImage()->GetWidth();
                  double ix = x_position;
                  double iy = y_position - ih;
                  XBYTE pr = 24, pg = 28, pb = 36;
                  GRP2DCOLOR* sample = canvas->GetPixel(ix - 2.0, iy + ih * 0.5);
                  if(!sample) sample = canvas->GetPixel(ix + iw + 2.0, iy + ih * 0.5);
                  if(sample)
                    {
                      GRP2DCOLOR_RGBA8* s = (GRP2DCOLOR_RGBA8*)sample;
                      if(s->a > 0 && (s->r + s->g + s->b) > 8)
                        {
                          pr = s->r; pg = s->g; pb = s->b;
                        }
                    }
                  GRP2DCOLOR_RGBA8 under(pr, pg, pb, 255);
                  int ih_i = (int)ih; if(ih_i < 1) ih_i = 1;
                  int iw_i = (int)iw; if(iw_i < 1) iw_i = 1;
                  for(int py = 0; py < ih_i; py++)
                    {
                      for(int px = 0; px < iw_i; px++)
                        {
                          GRP2DCOLOR* cur = canvas->GetPixel(ix + (double)px, iy + (double)py);
                          if(!cur) continue;
                          GRP2DCOLOR_RGBA8* c = (GRP2DCOLOR_RGBA8*)cur;
                          if((c->r + c->g + c->b) < 24)
                            {
                              canvas->PutPixel(ix + (double)px, iy + (double)py, (const GRP2DCOLOR*)&under);
                            }
                        }
                    }
                }
            }
           else
            {
              GRP2DREBUILDAREA* ownarea_anim = GetRebuildAreaByElement(element);
              GRP2DREBUILDAREA* animbackdrop = AnimBackdrop_Find(element);

              bool father_opaque_wash = false;
              UI_ELEMENT* father = element->GetFather();
              if(father)
                {
                  XDWORD ft = father->GetType();
                  if((ft == UI_ELEMENT_TYPE_OPTION) || (ft == UI_ELEMENT_TYPE_BUTTON) || (ft == UI_ELEMENT_TYPE_CHECKBOX))
                    {
                      UI_ELEMENT_OPTION* father_opt = (UI_ELEMENT_OPTION*)father;
                      if(father_opt->GetBackgroundColor() && (father_opt->GetBackgroundColor()->GetAlpha() == 255))
                        {
                          father_opaque_wash = true;
                        }
                    }
                }

              // Opaque exclusive-row wash (nav/gal): never PutBitmapNoAlpha a stale AnimBackdrop (that
              // punched a white icon plate into the gray selection). Drop and re-capture under the wash
              // that is already on screen (parent just painted it, or it is still showing from an earlier
              // select). Solo-icon redraws under an opaque parent are rare; a one-frame recapture is safer
              // than restoring a white plate.
              if(father_opaque_wash)
                {
                  AnimBackdrop_Delete(element);
                  animbackdrop = NULL;
                }
               else if(animbackdrop && animbackdrop->GetBitmap())
                {
                  if(ownarea_anim && ownarea_anim->GetBitmap() && !AnimBackdrop_MatchesArea(animbackdrop, ownarea_anim))
                    {
                      PutBitmapNoAlpha(animbackdrop->GetXPos(), animbackdrop->GetYPos(), animbackdrop->GetBitmap());
                      AnimBackdrop_Delete(element);
                      animbackdrop = NULL;
                    }
                   else
                    {
                      PutBitmapNoAlpha(animbackdrop->GetXPos(), animbackdrop->GetYPos(), animbackdrop->GetBitmap());
                    }
                }

              if(!AnimBackdrop_Find(element))
                {
                  double cap_x = 0.0;
                  double cap_y = 0.0;
                  double cap_w = 0.0;
                  double cap_h = 0.0;

                  if(ownarea_anim && ownarea_anim->GetBitmap())
                    {
                      cap_x = ownarea_anim->GetXPos();
                      cap_y = ownarea_anim->GetYPos();
                      cap_w = (double)ownarea_anim->GetBitmap()->GetWidth();
                      cap_h = (double)ownarea_anim->GetBitmap()->GetHeight();
                    }
                   else if(element_image->GetImage())
                    {
                      double ih = (double)element_image->GetImage()->GetHeight();
                      double iw = (double)element_image->GetImage()->GetWidth();
                      if(iw < 1.0) iw = 1.0;
                      if(ih < 1.0) ih = 1.0;
                      cap_x = x_position;
                      cap_y = y_position - ih;
                      cap_w = iw;
                      cap_h = ih;
                    }

                  if((cap_w > 0.0) && (cap_h > 0.0))
                    {
                      AnimBackdrop_Capture(element, cap_x, cap_y, cap_w, cap_h);
                    }
                }
            }
        }

      if(element_animation)
        {
          XBYTE alpha = element_animation->GetAlpha();
          if(alpha < element_image->GetAlpha()) element_image->SetAlpha(alpha);
        }

      // Image child only paints when ITS MustReDraw is set — force it when the animation father redraws.
      element_image->SetMustReDraw(true);
      Draw(element_image);     
    }

  switch(element_animation->GetState())
    {
      case UI_ELEMENT_ANIMATION_STATE_UNKNOWN   : break;

      case UI_ELEMENT_ANIMATION_STATE_PLAY      : if(element_animation->GetXTimerPlay()->GetMeasureMilliSeconds() >= element_animation->GetMilliSecondsDelay())
                                                    {
                                                      element_animation->IndexImage_Add();
                                                      element_animation->GetXTimerPlay()->Reset();  
                                                      redraw = true;                                                          
                                                    }
                                                  
                                                  break;

      case UI_ELEMENT_ANIMATION_STATE_STOP      : element_animation->IndexImage_Reset();                                                  
                                                  redraw = true;
                                                  break;


      case UI_ELEMENT_ANIMATION_STATE_PAUSE     : break;
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);
  
  if(redraw) 
    {
      // Self only. Animations now keep their own rebuild area inside scrolleables (see PreDrawFunction), so
      // advancing a frame must NOT Elements_SetToRedraw(owner, true): that re-rasterized entire textboxes /
      // multioptions every tick (UI_Options ~11 fps with Quixote + walking sprites).
      Elements_SetToRedraw(element_animation);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Option(UI_ELEMENT* element)
* @brief      Draw option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Option(UI_ELEMENT* element)  
{
  if(!element) return false;
 
  UI_ELEMENT_OPTION*    element_option    = (UI_ELEMENT_OPTION*)element;
  UI_ELEMENT_ANIMATION* element_animation = element_option->Get_UIAnimation();
  UI_ELEMENT_TEXT*      element_text      = element_option->Get_UIText();
  double                x_position        = 0.0f;
  double                y_position        = 0.0f;
  GRP2DCANVAS*            canvas            = GetCanvas();
  XRECT                 clip_rect;              

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position, UI_SKINCANVAS_PRESELECT_MAXEDGE);

  if(element->MustReDraw()) 
    {
      bool isselected  = false;
      bool ispreselect = false;

      if(element_option->GetFather()) 
        {
          if(element_option->GetFather()->GetType() != UI_ELEMENT_TYPE_MULTIOPTION)
            {
              ispreselect = element_option->IsPreSelect();
              if(!ispreselect) ispreselect = element_option->GetFather()->IsPreSelect();    
            }

          if(element_option->GetFather()->GetType() != UI_ELEMENT_TYPE_MULTIOPTION)
            {
              isselected = element_option->IsSelected();
              if(!isselected) isselected = element_option->GetFather()->IsSelected();    
            }
  
        } 
       else 
        {
          isselected = element_option->IsSelected();
          ispreselect = element_option->IsPreSelect();
        }

      if(element->IsBlinking() && !element->GetStateBlink()) ispreselect = false;

      // HOVER-WASH GHOSTING FIX (2026-09): root cause and design are written up in full on the "optionbackdrops"
      // member comment in UI_SkinCanvas.h -- summary: DrawBackgroundColor() below (and the selected/preselect
      // ring painted further down) blend purely by alpha, with nothing restoring the true backdrop first, so a
      // continuously-dirty hover/selection transition (UI_System's "nav-*-btn", CSS ":hover"/"transition:150")
      // compounds its own wash on top of itself frame after frame, and the residual never clears because
      // RebuildAllAreas() orphan-discards the generic rebuild area (without restoring) the moment the tween
      // settles. Fix: cache the TRUE backdrop once, the first time this element is drawn after being idle, and
      // restore it (PutBitmapNoAlpha(), GEN's own real restore primitive, never a synthetic fill colour)
      // immediately before every later real redraw within the SAME hover/selection episode -- mirroring the
      // already-verified ProgressBackdrop/FormBackdrop/RadialBackdrop/TextBackdrop fixes elsewhere in this file.
      // Unlike those, the anchor is explicitly forgotten the instant this element settles back to fully idle
      // (see the OptionBackdrop_Delete() call after the selection/preselect ring block below): this element's own
      // box overlaps sibling "nav-<section>-hl"/"-bar" elements that can change independently while it is NOT the
      // one being interacted with (e.g. a different row gets selected while this one sits idle), so a backdrop
      // kept forever would go stale against that; re-capturing fresh at the start of every new episode always
      // picks up whatever those siblings currently show.
      // Opaque exclusive-row washes (UI_Options nav/gal): skip OptionBackdrop. An opaque fill already
      // replaces prior pixels; keeping a permanent OptionBackdrop forced restore→force-dirty-children every
      // paint, which MarkOverlapping'd frame_records and wiped sibling gallery rows into a blank column.
      //
      // CHROME APP ICON (role=icon): same rule as AnimBackdrop — never keep/restore an OptionBackdrop.
      // After UpdateSize wipe a backdrop captured on pure black is PutBitmapNoAlpha'd under the PNG's
      // translucent corners as a permanent black plate on the caption.
      bool chrome_app_icon = (element_option->GetChromeRole() == UI_ELEMENT_CHROMEROLE_ICON);
      for(UI_ELEMENT* walk = element_option->GetFather(); !chrome_app_icon && walk; walk = walk->GetFather())
        {
          if(walk->GetChromeRole() == UI_ELEMENT_CHROMEROLE_ICON)
            {
              chrome_app_icon = true;
              break;
            }
        }

      bool opaque_option_wash = (element_option->GetBackgroundColor()->GetAlpha() == 255);
      bool optionbackdrop_restored = false;
      if(opaque_option_wash || chrome_app_icon)
        {
          OptionBackdrop_Delete(element_option);
        }
       else
        {
          GRP2DREBUILDAREA* ownarea_bg = GetRebuildAreaByElement(element_option);
          if(ownarea_bg && !GEN_USERINTERFACE.ModalLayer_IsCompositing())
            {
              GRP2DREBUILDAREA* optionbackdrop = OptionBackdrop_Find(element_option);

              if(!optionbackdrop)
                {
                  OptionBackdrop_Capture(element_option, ownarea_bg->GetXPos(), ownarea_bg->GetYPos(), (double)ownarea_bg->GetBitmap()->GetWidth(), (double)ownarea_bg->GetBitmap()->GetHeight());
                }
               else
                {
                  PutBitmapNoAlpha(optionbackdrop->GetXPos(), optionbackdrop->GetYPos(), optionbackdrop->GetBitmap());
                  optionbackdrop_restored = true;
                }
            }
        }

      // Classic XML buttons often keep bg alpha 0: OptionBackdrop restore can still leave prior glyph/icon
      // ink (capture after orphan-discard). Opaque white erase under the content prevents stack-to-black on
      // hover. CSS/dashboard options with a real wash (alpha > 0) rely on DrawBackgroundColor instead.
      // Skip when the window is layered-transparent OR a translucent form ancestor would show through
      // (UI_Message formcolorback=black,N): the white plate punches an opaque hole in the frosted panel.
      // Also skip empty hit overlays (UI_System nav-*-btn / disk-nav-hit): no compose children to repaint
      // over the white plate, so erase bleaches sibling icons/labels under the transparent root button.
      // UI_Options keeps white-erase — its nav/gal buttons own ic+tx children that redraw on top.
      // Icon-only multioption faces (custom-chrome SVGs, aqua PNG toolbars): the white plate sits under the
      // alpha bitmap and kills the transparent bubble. Father is MULTIOPTION (not a translucent form).
      if(element_option->GetBackgroundColor()->GetAlpha() == 0)
        {
          bool skip_white_erase = false;
          GRPSCREEN* scr = GetScreen();
          if(scr && scr->Style_Is(GRPSCREENSTYLE_TRANSPARENT)) skip_white_erase = true;
          if(!skip_white_erase && element_option->GetComposeElements()->IsEmpty() &&
             !element_option->Get_UIText() && !element_option->Get_UIAnimation())
            {
              skip_white_erase = true;
            }
          if(!skip_white_erase && element_option->Get_UIAnimation() && element_option->GetFather() &&
             (element_option->GetFather()->GetType() == UI_ELEMENT_TYPE_MULTIOPTION))
            {
              skip_white_erase = true;
            }
          if(!skip_white_erase && chrome_app_icon)
            {
              skip_white_erase = true;
            }
          if(!skip_white_erase)
            {
              UI_ELEMENT* anc = element_option->GetFather();
              while(anc)
                {
                  if(anc->GetType() == UI_ELEMENT_TYPE_FORM)
                    {
                      UI_COLOR* fc = anc->IsBackgroundColorSet() ? anc->GetBackgroundColor() : anc->GetColor();
                      if(fc && (fc->GetAlpha() > 0) && (fc->GetAlpha() < 255)) skip_white_erase = true;
                      break;
                    }
                  anc = anc->GetFather();
                }
            }
          if(!skip_white_erase)
            {
              GRP2DCOLOR_RGBA8 erase(255, 255, 255, 255);
              canvas->SetLineWidth(0.0f);
              canvas->SetLineColor(&erase);
              canvas->SetFillColor(&erase);
              canvas->Rectangle(x_position, y_position, x_position + element->GetBoundaryLine()->width, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height), true);
            }
        }

      DrawBackgroundColor(element_option, canvas, x_position, y_position);

      if(isselected)
        { 
          if(element_option->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_SELECT) 
            {     
              // Light blue select ring (was hard-coded red); matches technical-manual / UI_Options wash.
              GRP2DCOLOR_RGBA8  colorselect(120, 180, 220);
              GRP2DCOLOR_RGBA8  colorhover(0x32, 0x33, 0x36, 150);
  
              canvas->SetLineColor(&colorselect);
              canvas->SetFillColor(&colorhover);

              canvas->SetLineWidth(1.0f);

              if(element_option->GetRoundRect())
                {
                  canvas->RoundRect(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, element_option->GetRoundRect(), true);      
                }
               else
                {
                  canvas->Rectangle(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, true);      
                } 
            }
        }
       else
        {
          if(ispreselect)
            {       
              if(element_option->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_PRESELECT) 
                {        
                  // Same wash as UI_Options left-menu exclusive selection (220,236,250) on opaque UIs.
                  // On layered-transparent dialogs (UI_Message ok-btn + white "Confirmar" caption) that
                  // plate is too light for readable text — start from 50% of (220,236,250) then +20%
                  // lighter → (132,142,150).
                  GRP2DCOLOR_RGBA8  colorline(120, 180, 220);
                  GRP2DCOLOR_RGBA8  colorhover(220, 236, 250, 255);
                  GRPSCREEN* scr_pre = GetScreen();
                  if(scr_pre && scr_pre->Style_Is(GRPSCREENSTYLE_TRANSPARENT))
                    {
                      colorhover = GRP2DCOLOR_RGBA8(132, 142, 150, 255);
                    }
  
                  canvas->SetLineColor(&colorline);
                  canvas->SetFillColor(&colorhover);

                  canvas->SetLineWidth(1.0f);
      
                  if(element_option->GetRoundRect())
                    {
                      canvas->RoundRect(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, element_option->GetRoundRect(), true);      
                    }
                   else
                    {
                      canvas->Rectangle(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, true);      
                    } 
                }
            }
           else          
            { 
              if(element_option->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_ACTIVE)
                {
                  // RoundRect(..., true) first paints an OPAQUE outer disc in the line colour, then punches
                  // the interior with the fill colour. On layered-transparent UIs (UI_Message) a white line
                  // colour therefore leaves a solid white button plate. Skip the plate there; keep the classic
                  // white outline look on opaque windows.
                  GRPSCREEN* scr_active = GetScreen();
                  bool skip_active_plate = (scr_active && scr_active->Style_Is(GRPSCREENSTYLE_TRANSPARENT));
                  if(!skip_active_plate)
                    {
                      GRP2DCOLOR_RGBA8  colorwhite(255, 255, 255);
                      GRP2DCOLOR_RGBA8  colorhover(0x32, 0x33, 0x36, 0);
  
                      canvas->SetLineColor(&colorwhite);
                      canvas->SetFillColor(&colorhover);

                      canvas->SetLineWidth(1.0f);
      
                      if(element_option->GetRoundRect())
                        {
                          canvas->RoundRect(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, element_option->GetRoundRect(), true);      
                        }
                       else
                        {
                          canvas->Rectangle(x_position - UI_SKINCANVAS_PRESELECT_MAXEDGE, y_position + UI_SKINCANVAS_PRESELECT_MAXEDGE, x_position + element->GetBoundaryLine()->width + UI_SKINCANVAS_PRESELECT_MAXEDGE, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height) - UI_SKINCANVAS_PRESELECT_MAXEDGE, true);      
                        }
                    } 
                }
            }
        }

      // HOVER-WASH GHOSTING FIX (2026-09), continued: forget the OptionBackdrop anchor the instant this element
      // has nothing of its own left on screen. Deliberately does NOT gate on "isselected" alone -- a plain click
      // on a "nav-hit" button (visiblelimit intentionally has neither SELECT nor PRESELECT set, see dashboard.css)
      // can leave UI_ELEMENT_OPTION::IsSelected() latched true with nothing ever painted for it, and gating on
      // that would keep this anchor alive forever after the first click, exactly the staleness this fix exists to
      // avoid. Instead: nothing of this element's own is left on screen when the background wash has faded to
      // alpha 0 AND neither the selected ring nor the preselect ring is actually enabled+active for it right now.
      // UI_ELEMENT::UpdateTransition() snaps a tween to its exact target value at the end (never leaves it short),
      // so alpha==0 is reliably reached exactly once at the tail of every hover/selection fade-out. Leaving the
      // anchor alive past that point would restore a stale backdrop the NEXT time this element is hovered,
      // potentially undoing a sibling "nav-<section>-hl"/"-bar" change that happened in between (see the member
      // comment in UI_SkinCanvas.h for the concrete scenario this avoids).
      bool optionoverlaypainted = (isselected  && (element_option->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_SELECT)) ||
                                   (!isselected && ispreselect && (element_option->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_PRESELECT));

      if(!optionoverlaypainted && (element_option->GetBackgroundColor()->GetAlpha() == 0))
        {
          OptionBackdrop_Delete(element_option);
        }

      // After OptionBackdrop restore, compose child ink must be repainted (Draw_Form pattern).
      // Opaque exclusive-row wash: drop stale Text/Anim backdrops WITHOUT restoring — the wash just
      // painted is the new true backdrop. Restoring a white AnimBackdrop punched a white icon plate
      // into the gray selection bar (UI_Options gallery). Do NOT force-dirty the whole compose tree
      // every opaque paint (MarkOverlapping↔form wipe loop). Same-pass MustReDraw on the primary
      // animation/text (+ extra TEXT siblings) is enough so Draw() below re-captures on the new wash;
      // SelectExclusiveRow also MarkSubtreeDirties when the wash colour actually flips.
      if(opaque_option_wash)
        {
          for(XDWORD c=0; c<element_option->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* optionchild = (UI_ELEMENT*)element_option->GetComposeElements()->Get(c);
              if(!optionchild) continue;
              if(optionchild->GetType() == UI_ELEMENT_TYPE_TEXT)      DiscardTextBackdrop(optionchild);
              if(optionchild->GetType() == UI_ELEMENT_TYPE_ANIMATION) DiscardAnimBackdrop(optionchild);
            }
          if(element_animation) element_animation->SetMustReDraw(true);
          if(element_text)      element_text->SetMustReDraw(true);
          for(XDWORD c=0; c<element_option->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* optionchild = (UI_ELEMENT*)element_option->GetComposeElements()->Get(c);
              if(optionchild && (optionchild->GetType() == UI_ELEMENT_TYPE_TEXT) && (optionchild != (UI_ELEMENT*)element_text))
                optionchild->SetMustReDraw(true);
            }
        }

      if(optionbackdrop_restored)
        {
          for(XDWORD c=0; c<element_option->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* optionchild = (UI_ELEMENT*)element_option->GetComposeElements()->Get(c);
              if(optionchild) optionchild->SetMustReDraw(true);
            }
        }

      if(element_animation)  Draw(element_animation);
      if(element_text)       Draw(element_text);

      // Extra compose TEXT siblings (e.g. UI_Options gallery row dates nested in the button) — not
      // returned by Get_UIText(), which only tracks the primary label.
      for(XDWORD c=0; c<element_option->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* sub = element_option->GetComposeElements()->Get(c);
          if(!sub || (sub->GetType() != UI_ELEMENT_TYPE_TEXT)) continue;
          if(sub == (UI_ELEMENT*)element_text) continue;
          Draw(sub);
        }

      // MULTIOPTION under-icon legend band: after the bubble face is on screen, opaque-fill the
      // strip immediately below the AABB with the layout page colour. Shared-slot peels
      // (Stop/Run) leave a solid gray plate (57,59,61) in that band; sibling caption forms then
      // cannot fully erase it (text clip box). Horizontally padded so a wider legend form is covered.
      // Skip inside the active modal tree: dialog buttons have no under-icon captions, and a
      // 24px band below a bottom-padded control bleeds past the modal AABB (XTM config modal).
      bool under_active_modal = false;
      if(UI_ELEMENT* modal = GEN_USERINTERFACE.Element_GetModal())
        {
          for(UI_ELEMENT* walk = element_option; walk; walk = walk->GetFather())
            {
              if(walk == modal) { under_active_modal = true; break; }
            }
        }
      if(element->MustReDraw() && canvas && element_option->GetFather() &&
         (element_option->GetFather()->GetType() == UI_ELEMENT_TYPE_MULTIOPTION) &&
         !GEN_USERINTERFACE.ModalLayer_IsCompositing() &&
         !under_active_modal)
        {
          XBYTE pr = 24, pg = 28, pb = 36;
          UI_LAYOUT* layout = element->GetLayout();
          if(layout && layout->GetBackground() && layout->GetBackground()->GetColor() &&
             layout->GetBackground()->GetColor()->IsValid())
            {
              UI_COLOR* page = layout->GetBackground()->GetColor();
              pr = (XBYTE)page->GetRed();
              pg = (XBYTE)page->GetGreen();
              pb = (XBYTE)page->GetBlue();
            }

          const double legend_band = 24.0;
          const double hpad = 10.0;
          double band_x = x_position - hpad;
          double band_w = element->GetBoundaryLine()->width + (hpad * 2.0);
          double band_y = y_position;                 // button EdgeBottom = top of under-icon band
          double band_h = legend_band;

          XRECT band_clip_saved;
          canvas->GetClipBox(band_clip_saved);
          canvas->SetClipBox(band_x, band_y + band_h, band_x + band_w, band_y);

          GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
          int bh = (int)band_h;
          int bw = (int)band_w;
          if(bh < 1) bh = 1;
          if(bw < 1) bw = 1;
          for(int py = 0; py < bh; py++)
            {
              for(int px = 0; px < bw; px++)
                {
                  canvas->PutPixel(band_x + (double)px, band_y + (double)py, (const GRP2DCOLOR*)&pagecol);
                }
            }

          canvas->SetClipBox(band_clip_saved);
        }
    }

  
  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element_animation )
    {
      if(element_animation->GetState() == UI_ELEMENT_ANIMATION_STATE_PLAY)
        { 
          Elements_SetToRedraw(element_option);  
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_MultiOption(UI_ELEMENT* element)
* @brief      Draw multi option
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_MultiOption(UI_ELEMENT* element)
{    
  if(!screen)  return false;
  if(!element) return false;

  UI_ELEMENT_MULTIOPTION* element_multioption  = (UI_ELEMENT_MULTIOPTION*)element;
  double                  x_position           = 0.0f;
  double                  y_position           = 0.0f;
  GRP2DCANVAS*              canvas               = GetCanvas();
  XRECT                   clip_rect;  
  
  if(!canvas) return false;

  // SAME-BUTTON FACE SWITCH + SHARED-SLOT TWINS (Stop/Run at the same AABB): before PreDraw,
  // (1) peel any OTHER element's Anim/Option backdrop that overlaps this slot (invisible twin ink),
  // (2) restore this multioption's own face backdrops so active/preselect/selected/deactive swaps
  // do not stack. RebuildAllAreas orphan-discard makes both ghosts common after maximize/restore.
  if(element->MustReDraw() && !GEN_USERINTERFACE.ModalLayer_IsCompositing())
    {
      double slot_x = element_multioption->GetXPosition();
      double slot_y = element_multioption->GetYPosition();
      double slot_w = element_multioption->GetBoundaryLine() ? element_multioption->GetBoundaryLine()->width  : 0.0;
      double slot_h = element_multioption->GetBoundaryLine() ? element_multioption->GetBoundaryLine()->height : 0.0;
      if(element_multioption->GetFather()) GetAddPositionScrollSteps(element_multioption->GetFather(), slot_x, slot_y);

      if((slot_w > 0.0) && (slot_h > 0.0))
        {
          double pad = (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
          double left   = UI_BOUNDARYLINE_EdgeLeft  (slot_x, slot_w) - pad;
          double top    = UI_BOUNDARYLINE_EdgeTop   (slot_y, slot_h) - pad;
          double width  = slot_w + (pad * 2.0);
          double height = slot_h + (pad * 2.0);

          // Anim/Option only before the face paint. Sibling legends under the slot are refreshed
          // AFTER the new face is drawn (see post-paint TextBackdrop_Invalidate below) so their
          // TextBackdrop re-capture cannot bake the bubble rim / gray plate under Demarrer/Arreter.
          OptionBackdrop_InvalidateOverlapping(left, top, width, height, true);
          AnimBackdrop_InvalidateOverlapping(left, top, width, height, element_multioption);
        }

      // Clip face-cache restores to the slot AABB (+preselect pad). A full-box blit of a padded
      // Option/Anim backdrop spilled into the under-icon legend and left a gray plate there.
      double peel_l = UI_BOUNDARYLINE_EdgeLeft  (slot_x, slot_w) - (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
      double peel_t = UI_BOUNDARYLINE_EdgeTop   (slot_y, slot_h) - (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
      double peel_r = UI_BOUNDARYLINE_EdgeRight (slot_x, slot_w) + (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
      double peel_b = UI_BOUNDARYLINE_EdgeBottom(slot_y, slot_h) + (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;

      for(XDWORD c=0; c<element_multioption->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* face = (UI_ELEMENT*)element_multioption->GetComposeElements()->Get(c);
          if(!face) continue;

          GRP2DREBUILDAREA* optionbackdrop = OptionBackdrop_Find(face);
          if(optionbackdrop && optionbackdrop->GetBitmap())
            {
              UI_SkinCanvas_RestoreBackdropIntersection(this, optionbackdrop, peel_l, peel_t, peel_r, peel_b);
            }
          OptionBackdrop_Delete(face);

          for(XDWORD d=0; d<face->GetComposeElements()->GetSize(); d++)
            {
              UI_ELEMENT* facechild = (UI_ELEMENT*)face->GetComposeElements()->Get(d);
              if(!facechild) continue;

              if(facechild->GetType() == UI_ELEMENT_TYPE_ANIMATION)
                {
                  GRP2DREBUILDAREA* animbackdrop = AnimBackdrop_Find(facechild);
                  if(animbackdrop && animbackdrop->GetBitmap())
                    {
                      UI_SkinCanvas_RestoreBackdropIntersection(this, animbackdrop, peel_l, peel_t, peel_r, peel_b);
                    }
                  AnimBackdrop_Delete(facechild);
                }
               else if(facechild->GetType() == UI_ELEMENT_TYPE_TEXT)
                {
                  GRP2DREBUILDAREA* textbackdrop = TextBackdrop_Find(facechild);
                  if(textbackdrop && textbackdrop->GetBitmap())
                    {
                      UI_SkinCanvas_RestoreBackdropIntersection(this, textbackdrop, peel_l, peel_t, peel_r, peel_b);
                    }
                  TextBackdrop_Delete(facechild);
                }
            }
        }

      CompactLegendsMarkNeedRepair();
    }

  // Remember whether this tick is a real face/slot redraw so we can refresh sibling legends after paint.
  bool multioption_face_redraw = element->MustReDraw() && !GEN_USERINTERFACE.ModalLayer_IsCompositing();
  
  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  UI_PROPERTY_SELECTABLE_STATE actual_state = UI_PROPERTY_SELECTABLE_STATE_DEACTIVE;
  
  if(element_multioption->IsSelected()) 
    { 
      actual_state = UI_PROPERTY_SELECTABLE_STATE_SELECTED;
    }
   else 
    {
      if(element_multioption->IsPreSelect()) 
        {
          actual_state = UI_PROPERTY_SELECTABLE_STATE_PRESELECT;        
        }
       else 
        {
          if(element_multioption->IsActive()) 
            { 
              actual_state = UI_PROPERTY_SELECTABLE_STATE_ACTIVE;              
            }
        }
    }

  // Ungated paint (historical): a MustReDraw gate here left chrome buttons blank when a sibling
  // caption/form wiped their pixels while the multioption flag stayed clean. Face-switch erase above
  // remains gated on MustReDraw so only real redraws peel prior faces.
  //if(element->MustReDraw()) 
    {
      DrawBackgroundColor(element_multioption, canvas, x_position, y_position);
 
      for(XDWORD c=0; c<element_multioption->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT_OPTION* element_option = (UI_ELEMENT_OPTION*)element_multioption->GetComposeElements()->Get(c);
          if(element_option) 
            {
              if(element_option->GetSelectableState() == actual_state) 
                {
                  // On a real multioption dirty (any state→state), force this face to emit pixels.
                  if(element->MustReDraw())
                    {
                      element_option->SetMustReDraw(true);
                      for(XDWORD d=0; d<element_option->GetComposeElements()->GetSize(); d++)
                        {
                          UI_ELEMENT* facechild = (UI_ELEMENT*)element_option->GetComposeElements()->Get(d);
                          if(facechild) facechild->SetMustReDraw(true);
                        }
                    }
                  Draw(element_option);              
                  break;              
                }       
            }            
        }
    }

  // After the bubble face is on screen: restore+dirty any sibling TEXT whose plate overlaps the slot
  // or the legend strip just below it (shared Stop/Run caption). Draw_Text then page-seeds a clean
  // TextBackdrop (transparent-father path) and reprints glyphs on top — no gray bar, no double caption.
  if(multioption_face_redraw && element_multioption->GetBoundaryLine())
    {
      double slot_x = element_multioption->GetXPosition();
      double slot_y = element_multioption->GetYPosition();
      double slot_w = element_multioption->GetBoundaryLine()->width;
      double slot_h = element_multioption->GetBoundaryLine()->height;
      if(element_multioption->GetFather()) GetAddPositionScrollSteps(element_multioption->GetFather(), slot_x, slot_y);

      const double legend_pad = 20.0;
      double left   = UI_BOUNDARYLINE_EdgeLeft  (slot_x, slot_w) - (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
      double top    = UI_BOUNDARYLINE_EdgeTop   (slot_y, slot_h) - (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
      double width  = slot_w + (double)(UI_SKINCANVAS_PRESELECT_MAXEDGE * 2);
      // Extend past the bubble bottom into the typical under-icon legend band.
      double height = slot_h + (double)(UI_SKINCANVAS_PRESELECT_MAXEDGE * 2) + legend_pad;

      TextBackdrop_InvalidateOverlapping(left, top, width, height, element_multioption);

      // Same-frame: page-fill + Draw overlapping compact legend forms. SetToRedraw alone is not
      // enough when RebuildAllAreas already ran — a peel can leave a gray plate under Demarrer/
      // Arreter until the next tick, and a later orphan-restore can bake it permanently.
      UI_LAYOUT* molayout = element_multioption->GetLayout();
      if(molayout)
        {
          double right  = left + width;
          double bottom = top + height;

          XBYTE pr = 24, pg = 28, pb = 36;
          if(molayout->GetBackground() && molayout->GetBackground()->GetColor() &&
             molayout->GetBackground()->GetColor()->IsValid())
            {
              UI_COLOR* page = molayout->GetBackground()->GetColor();
              pr = (XBYTE)page->GetRed();
              pg = (XBYTE)page->GetGreen();
              pb = (XBYTE)page->GetBlue();
            }

          for(XDWORD ei = 0; ei < molayout->Elements_Get()->GetSize(); ei++)
            {
              UI_ELEMENT* sib = molayout->Elements_Get()->Get(ei);
              if(!sib || sib == element_multioption) continue;
              if(sib->GetType() != UI_ELEMENT_TYPE_FORM) continue;
              if(!sib->GetBoundaryLine()) continue;

              // Only compact under-icon legend strips (not large panels that happen to overlap).
              if(sib->GetBoundaryLine()->height > 28.0) continue;

              double sx = sib->GetXPosition();
              double sy = sib->GetYPosition();
              if(sib->GetFather()) GetAddPositionScrollSteps(sib->GetFather(), sx, sy);
              double sl = UI_BOUNDARYLINE_EdgeLeft(sx, sib->GetBoundaryLine()->width);
              double sr = UI_BOUNDARYLINE_EdgeRight(sx, sib->GetBoundaryLine()->width);
              double st = UI_BOUNDARYLINE_EdgeTop(sy, sib->GetBoundaryLine()->height);
              double sb = UI_BOUNDARYLINE_EdgeBottom(sy, sib->GetBoundaryLine()->height);
              if(sl < right && sr > left && st < bottom && sb > top)
                {
                  double lw = sr - sl;
                  double lh = sb - st;
                  if(lw < 1.0) lw = 1.0;
                  if(lh < 1.0) lh = 1.0;

                  // Prefer authored opaque legend fill; otherwise layout page colour.
                  UI_ELEMENT_FORM* sf = (UI_ELEMENT_FORM*)sib;
                  UI_COLOR* sfill = sf->IsBackgroundColorSet() ? sf->GetBackgroundColor() : sf->GetColor();
                  XBYTE fr = pr, fg = pg, fb = pb;
                  if(sfill && sfill->GetAlpha() == 255)
                    {
                      fr = (XBYTE)sfill->GetRed();
                      fg = (XBYTE)sfill->GetGreen();
                      fb = (XBYTE)sfill->GetBlue();
                    }
                          GRPBITMAP* legbmp = GetBitmap(sl, st, lw, lh);
                  if(legbmp)
                    {
                      GRP2DCOLOR_RGBA8 col(fr, fg, fb, 255);
                      for(XDWORD py = 0; py < legbmp->GetHeight(); py++)
                        {
                          for(XDWORD px = 0; px < legbmp->GetWidth(); px++)
                            {
                              legbmp->PutPixel((int)px, (int)py, (const GRP2DCOLOR*)&col);
                            }
                        }
                      PutBitmapNoAlpha(sl, st, legbmp);
                      GEN_DELETE legbmp;
                    }

                  // Legend is authored after the bubble in XML — layout walk may still be before
                  // later siblings that re-contaminate. Mark for end-of-layout same-frame repair.
                  GEN_USERINTERFACE.Elements_SetToRedraw(sib, true);
                  CompactLegendsMarkNeedRepair();
                }
            }
        }
    }
      
  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Button(UI_ELEMENT* element)
* @brief      Draw button
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Button(UI_ELEMENT* element)  
{  
  return Draw_Option(element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_CheckBox(UI_ELEMENT* element)
* @brief      Draw check box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_CheckBox(UI_ELEMENT* element)  
{
  UI_ELEMENT_CHECKBOX* checkbox = (UI_ELEMENT_CHECKBOX*)element;

  if(checkbox)
    {
      UI_ELEMENT_ANIMATION* animation = checkbox->Get_UIAnimation();
      if(animation) animation->IndexImage_Set(checkbox->GetStatus()?1:0);
    }

  return Draw_Option(element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_EditText(UI_ELEMENT* element)
* @brief      Draw edit text
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_EditText(UI_ELEMENT* element)  
{    
  if(!element) return false;  

  UI_ELEMENT_EDITTEXT*  element_edittext  = (UI_ELEMENT_EDITTEXT*)element;
  double                x_position        = 0.0f;
  double                y_position        = 0.0f;
  GRP2DCANVAS*            canvas            = GetCanvas();    
  static bool           visible           = false;
  XRECT                 clip_rect;                       
  
  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position, UI_SKINCANVAS_EDIT_MAXEDGE);
  
  GRP2DCOLOR_RGBA8  color(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());

  GRP2DCOLOR_RGBA8  bkgcolor(element->GetBackgroundColor()->GetRed(), element->GetBackgroundColor()->GetGreen(), element->GetBackgroundColor()->GetBlue(), element_edittext->IsPreSelect()?element->GetBackgroundColor()->GetAlpha()/2:element->GetBackgroundColor()->GetAlpha());

  GRP2DCOLOR_RGBA8  linecolor;
 
  if(element->MustReDraw()) 
    {
      canvas->SetLineColor(&linecolor);
      canvas->SetFillColor(&bkgcolor);

      if(element->GetRoundRect())
        {
          canvas->RoundRect(element->GetXPosition() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetYPosition() + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetXPosition() + element->GetBoundaryLine()->width + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetTopY() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetRoundRect(), true);        
        }
       else
        {
          canvas->Rectangle(element->GetXPosition() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetYPosition() + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetXPosition() + element->GetBoundaryLine()->width + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetTopY() - UI_SKINCANVAS_EDIT_MAXEDGE, true);
        }
   
                                   
      canvas->Vectorfont_GetConfig()->SetColor(&color);
      canvas->Vectorfont_GetConfig()->SetSize(element_edittext->GetSizeFont());  
      canvas->VectorFont_Printf(element->GetXPosition(), element->GetYPosition(), element_edittext->GetText()->Get());

      if(element_edittext->Cursor_IsVisible())
        {
          XDWORD    cursor_xpos = 0;
          XSTRING   cursor_string;

          element_edittext->GetText()->Copy(0, element_edittext->Cursor_GetPosition(), cursor_string);

          cursor_xpos = (XDWORD)GetWidthString(cursor_string.Get(), element_edittext->GetSizeFont());
       
          if(element_edittext->Cursor_GetXTimerBlink()->GetMeasureMilliSeconds() >= element_edittext->Cursor_GetTimeBlink())
            {
              element_edittext->Cursor_ChangeStateBlink();   
              element_edittext->Cursor_GetXTimerBlink()->Reset();
            } 
 
          if(element_edittext->Cursor_GetStateBlink())
            {
              int cursor_size = 3;

              // Cursor follows the edit text colour so it stays visible on both light (e.g. virtual-keyboard
              // input on white) and dark edit backgrounds — never hard-code white.
              canvas->SetLineColor(&color);
              canvas->SetFillColor(&color);

              canvas->Rectangle(x_position + cursor_xpos, y_position + UI_SKINCANVAS_EDIT_MAXEDGE - 4, x_position + cursor_xpos + cursor_size, UI_BOUNDARYLINE_EdgeTop(y_position, element_edittext->GetBoundaryLine()->height) - UI_SKINCANVAS_EDIT_MAXEDGE + 4, true);                          
            }
        }
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Form(UI_ELEMENT* element)
* @brief      Draw form
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Form(UI_ELEMENT* element)
{    
  if(!screen)  return false;
  if(!element) return false;

  UI_ELEMENT_FORM*    element_form = (UI_ELEMENT_FORM*)element;
  double              x_position   = 0.0f;
  double              y_position   = 0.0f;
  GRP2DCANVAS*          canvas       = GetCanvas();
  XRECT               clip_rect;  
  
  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      // ALPHA-DARKENING FIX (2026-09): steps 4-8 below (box-shadow, then fill+stroke) always paint by
      // ALPHA-BLENDING onto whatever is already on the canvas (AGG "over" compositing) -- never a plain
      // overwrite -- because that is what lets a translucent color (e.g. color="black,50", used by both
      // "form" and "menu" elements for a frosted-glass panel look) show the real background through it. A
      // form/menu that gets marked MustReDraw() again later WITHOUT ever moving or becoming invisible (the
      // common case: some unrelated interaction elsewhere in the layout calls a global
      // Elements_SetToRedraw(), which recursively re-dirties this element and its children too) simply
      // repaints the SAME nominal translucent color on top of its own earlier repaint of itself -- and
      // standard "over" compositing of a color onto itself compounds toward full opacity every time (50%
      // over 50% = 75%, then 87.5%, ...), NOT a no-op. Confirmed live via canvas->GetBitmap() buffer reads on
      // "ListBoxMenuID" (color="black,50"): stored alpha measured 127 -> 191 -> 242 across three consecutive
      // real redraws, each one triggered by selecting a different row (an unrelated, recursive
      // Elements_SetToRedraw() call, not a change to this element's own geometry or color) -- visually, the
      // translucent panel darkens toward solid black a little more on every such redraw, exactly as reported.
      //
      // Fix: cache the TRUE backdrop (the real pixels behind this element, e.g. the seamless background
      // pattern) once, the very first time this element is ever drawn -- before any of its own shadow, fill,
      // border or children have painted anything -- and restore it (PutBitmapNoAlpha(), GEN's own real
      // restore primitive, never a synthetic fill colour) immediately before repainting on every later real
      // redraw, so the translucent blend below always starts fresh instead of compounding on top of itself.
      //
      // Only done when this element actually owns its own rebuild-area THIS tick (GetRebuildAreaByElement(),
      // set a few lines above by PreDrawFunction()): that area's geometry is already correctly expanded for
      // the box-shadow footprint (see PreDrawFunction()'s own step-7 padding), so reusing it here keeps the
      // captured/restored box perfectly in sync with the shadow without duplicating that padding logic. When
      // this element is instead a descendant of an already-dirty ancestor (no area of its own), it is left
      // alone: the ancestor's own redraw -- running first, higher up this same recursive Draw() walk -- is
      // trusted to already have cleared/reset the canvas beneath before this descendant ever paints, exactly
      // the same "ancestor covers descendant" assumption the rest of this rebuild-area system relies on.
      GRP2DREBUILDAREA* ownarea = GetRebuildAreaByElement(element);

      // STRUCTURAL-CONTAINER GHOSTING FIX (2026-09): root cause of the sidebar's "selection band never
      // clears / stacks on hover" report. A purely-structural flex container (e.g. dashboard.xml's
      // "sidebar_nav": background-color 0,0,0,0, border-width 0, no box-shadow -- CSS comment calls it
      // "otherwise-invisible... paints nothing of its own") paints NOTHING at steps 4-8 below: fill alpha
      // is 0, there is no stroke (border-width 0), there is no shadow. The ALPHA-DARKENING FIX above this
      // block exists to protect a form that DOES alpha-composite a translucent color onto its backdrop from
      // compounding across repeated redraws -- it has nothing to do when the element has no ink of its own.
      // Worse, running it anyway is actively harmful here: this element's rebuild area is its full bounding
      // box (matching "sidebar_nav"'s own box, which spans the ENTIRE nav list, xpos=0 ypos=41..862 --
      // BEHIND every "nav-<section>-hl"/"nav-<section>-bar" selection-band overlay, which are separate
      // root-level siblings declared/painted BEFORE it, not its children -- see dashboard.xml's own
      // "Z-ORDER" comment on "sidebar_nav"). Its backdrop is captured ONCE, the very first time it is ever
      // drawn -- which happens AFTER the default-selected section's band has already painted underneath, so
      // the "true backdrop" this element captures permanently bakes in that first band's colour. Every later
      // redraw of this container (triggered whenever ANY nav row's icon/text needs to change colour, e.g. on
      // every section click, since UI_SYSTEM::UserInterface_SelectSection() recolours the text label inside
      // it) then restores that stale full-panel snapshot with PutBitmapNoAlpha() -- painting the OLD section's
      // band back over the freshly-corrected canvas -- and force-marks only its OWN real children (the icon/
      // text row wrappers) dirty to repaint on top, which never touches "nav-<section>-hl"/"-bar" (not its
      // children), so the stale band shows through indefinitely. Confirmed live via a name-agnostic pixel
      // probe (since removed): "sidebar_nav"'s own restore is the LAST thing to
      // touch the selection-band pixel on every reproduction, landing the old colour there every time.
      // Fix: only run the capture/restore machinery when this element could actually paint something of its
      // own at steps 4-8 (matches those steps' own fill/stroke/shadow resolution exactly, so this reflects
      // reality rather than re-guessing it) -- a container with nothing to alpha-composite has nothing to
      // protect against compounding, so it is simply left out of the backdrop system entirely; its real
      // children keep repainting themselves via their own independent dirty flags exactly as before.
      UI_COLOR* fillsrc_check = element_form->IsBackgroundColorSet() ? element_form->GetBackgroundColor()
                                                                      : element_form->GetColor();
      // border_width -1 = "default" (not an authored stroke). Counting it as paintable made fully
      // transparent toolbar legend forms (Stop/Run captions) keep a FormBackdrop that baked neighbour
      // bubble-rim pixels into a permanent gray plate under the glyphs. Only authored border > 0,
      // real fill alpha, or box-shadow makes the form paint ink of its own.
      bool haspaintable = (fillsrc_check && fillsrc_check->GetAlpha() > 0) ||
                           (element_form->GetBorderWidth() > 0.0)          ||
                           element_form->IsBoxShadowSet();

      // ACCENT-BAR NEVER APPEARS FIX (2026-09): true when this exact element (a "form", e.g. one of UI_System's
      // "nav-<section>-bar"/"-hl") is either being drawn for the very first time in its life, OR is being drawn
      // visible again right after having been hidden (formhiddentracked, see its own comment in UI_SkinCanvas.h)
      // -- both are moments where it is about to paint genuinely NEW ink that no existing option cache could
      // possibly already reflect. See the fuller root-cause writeup at the OptionBackdrop_InvalidateOverlapping()
      // call a few lines down.
      int  formwashiddenindex   = formhiddentracked.Find(element);
      bool formneverdrawnbefore = (formwashiddenindex != NOTFOUND);

      if(formwashiddenindex != NOTFOUND) formhiddentracked.Delete(element);

      // Element geometry (screen coords, y-down). Shared by the shadow layer (step 7) and the fill+stroke
      // layer (steps 4-5-6) below. Computed here (moved up from its original position further down) so the
      // ACCENT-BAR TRAIL FIX block just below can use it as a fallback box too.
      double  vr_minx = element_form->GetVisibleRect()->x;
      double  vr_miny = element_form->GetVisibleRect()->GetTop();
      double  vr_maxx = element_form->GetVisibleRect()->x + element_form->GetVisibleRect()->width;
      double  vr_maxy = element_form->GetVisibleRect()->y;

      // Phase 4: padding as paint inset for the fill/stroke content box (layout still uses padding for children).
      double pad_L = element_form->GetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT);
      double pad_R = element_form->GetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT);
      double pad_T = element_form->GetPadding(UI_ELEMENT_TYPE_ALIGN_UP);
      double pad_B = element_form->GetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN);
      if(pad_L > 0.0 || pad_R > 0.0 || pad_T > 0.0 || pad_B > 0.0)
        {
          vr_minx += pad_L;
          vr_maxx -= pad_R;
          vr_miny += pad_T;
          vr_maxy -= pad_B;
          if(vr_maxx < vr_minx) vr_maxx = vr_minx;
          if(vr_maxy < vr_miny) vr_maxy = vr_miny;
        }

      // Kept across the haspaintable/FormBackdrop block so the RoundRect corner-tip punch below can
      // restore true parent pixels into the AABB cutouts (datetime_box / inner-box "picos").
      GRP2DREBUILDAREA* formbackdrop_for_punch = NULL;

      if(haspaintable && !GEN_USERINTERFACE.ModalLayer_IsCompositing())
        {
          // ACCENT-BAR TRAIL FIX (2026-09): root cause of "quedan rastros de la barra azul en la opcion
          // previamente seleccionada". Prefer "ownarea" (already expanded for the box-shadow/edge footprint)
          // when this element owns one THIS tick; otherwise fall back to the plain visible-rect (vr_*) box --
          // exactly the same "ownarea-or-vr_*" fallback already used a few lines down for
          // OptionBackdrop_InvalidateOverlapping()/TextBackdrop_InvalidateOverlapping(). This used to be gated
          // on "ownarea &&", which skipped ALL backdrop capture/restore on a tick where this element paints but
          // does not own a rebuild area (a descendant of an already-dirty ancestor, or -- confirmed live via a
          // temporary trace -- "nav-<section>-hl"/"-bar" themselves during the very first few application-
          // startup ticks, before the per-element rebuild-area system has settled). For a section selected by
          // DEFAULT at startup (e.g. "Resumen"), that meant its very FIRST real fill-paint happened on one of
          // those early "ownarea==nil" ticks with NO backdrop protection at all; by the time this element
          // finally owned a rebuild area and reached the "first-ever capture" below, its own selection-wash
          // colour was already on screen -- captured as if it were the pristine, empty backdrop, and restored
          // forever after on every later deselect. Confirmed live: the residual patch's colour measured exactly
          // "nav-band"'s CSS colour (33,58,105 @ 45%) alpha-blended ONCE over the true panel background -- not
          // a mismatched-geometry leftover, but this element's own wash, permanently baked into its "forever"
          // cache. Using the same vr_* fallback box used elsewhere in this function closes that gap: this
          // element's OWN capture/restore now runs on every tick it paints, ownarea or not, so a startup tick
          // with no rebuild area still gets its "first genuinely pristine moment" protection instead of silently
          // skipping it.
          // FormBackdrop capture/restore tracks the FILL box (vr_*), not an ownarea expanded for
          // child text overhang. Toolbar legends under shared-slot bubbles grow ownarea into the
          // bubble AABB; capturing that baked neighbour OptionBackdrop gray into FormBackdrop and
          // restored a permanent plate above the form fill after maximize/restore.
          // Box-shadow forms already used vr_* (blur pad blanks siblings). Accent-bar / nav-hl washes
          // match their VisibleRect, so vr_* keeps that fix; InvalidateOverlapping below still uses
          // ownarea when present so neighbours under a shadow footprint stay invalidated.
          double bd_x = vr_minx;
          double bd_y = vr_miny;
          double bd_w = vr_maxx - vr_minx;
          double bd_h = vr_maxy - vr_miny;
          if(!element_form->IsBoxShadowSet() && ownarea && ownarea->GetBitmap())
            {
              // Accent-bar startup path: when ownarea exists and is NOT larger than the fill (no
              // text-overhang inflation), prefer it so a padded wash box still matches PreDraw.
              double ow = (double)ownarea->GetBitmap()->GetWidth();
              double oh = (double)ownarea->GetBitmap()->GetHeight();
              if((ow <= bd_w + 1.0) && (oh <= bd_h + 1.0))
                {
                  bd_x = ownarea->GetXPos();
                  bd_y = ownarea->GetYPos();
                  bd_w = ow;
                  bd_h = oh;
                }
            }

      GRP2DREBUILDAREA* formbackdrop = FormBackdrop_Find(element);

      // ACCENT-BAR TRAIL FIX (2026-09), continued: see FormBackdrop_MatchesArea()'s own header comment.
          // The cached box can be smaller than THIS tick's actual backdrop box (confirmed live: the very first
          // capture of "nav-<section>-hl"/"-bar" measured smaller than a later ownarea-based redraw's own
          // padded box) -- restoring it as-is would leave the extra margin permanently un-erased. Erase the
          // OLD (mismatched) box first -- exactly like the RESIZE-RECAPTURE FIX in Draw_Text() -- then drop it
          // and fall through to the "never drawn before" branch below, which captures fresh at the current,
          // correct size.
          if(formbackdrop && !FormBackdrop_MatchesArea(formbackdrop, bd_x, bd_y, bd_w, bd_h))
            {
              PutBitmapNoAlpha(formbackdrop->GetXPos(), formbackdrop->GetYPos(), formbackdrop->GetBitmap());
              FormBackdrop_Delete(element);
              formbackdrop = NULL;
            }

          if(!formbackdrop) formneverdrawnbefore = true;

          if(!formbackdrop)
            {
              // FIRST-CAPTURE CONTAMINATION FIX (2026-09): "nothing has painted... ink here yet" (see below) is
              // true for THIS element's own shadow/fill/border/children, but not necessarily for an unrelated,
              // independently-animating sibling that happens to share this exact box -- concretely, UI_System's
              // "nav-<section>-btn" hit-target (transparent, CSS ":hover" wash, see the HOVER-WASH GHOSTING FIX
              // in Draw_Option()) sits ON TOP of "nav-<section>-hl" and can already be mid-hover (wash actively
              // blended in) the very first time this band is ever selected -- a click both starts the button's
              // hover episode AND makes this band visible for the first time, so by the time THIS capture runs,
              // the canvas can already show [true backdrop + button's hover wash], not the true backdrop alone.
              // A FormBackdrop is kept FOREVER (unlike optionbackdrops), so baking that transient wash in here
              // would permanently tint this band's "cleared" state every time it is later deselected -- confirmed
              // live: after Resumen/CPU/Memoria were each selected once then deselected, their rows settled to a
              // visibly lighter grey instead of the true panel colour, exactly matching the overlapping button's
              // captured (clean, pre-wash) OptionBackdrop pixel value. Fix: if an option-family element's own
              // (already-pristine, captured before ITS wash ever painted -- see OptionBackdrop_Capture()) cached
              // backdrop overlaps this box, restore it here first (PutBitmapNoAlpha(), the same real primitive
              // used everywhere else in this file) to strip out that transient wash before capturing -- harmless
              // even when nothing overlaps (OptionBackdrop_FindOverlapping() then returns NULL, no-op), and
              // invisible to the user either way: the overlapping button still repaints its own wash on top of
              // this band later in this very same frame (z-order), and nothing reaches the screen until the
              // whole frame's draw pass finishes and swaps -- only the FUTURE (fully accurate) cached value
              // changes.
              GRP2DREBUILDAREA* overlappingoption = OptionBackdrop_FindOverlapping(bd_x, bd_y, bd_w, bd_h);
              if(overlappingoption)
                {
                  // Intersection only — full OptionBackdrop blit would seed the legend strip outside
                  // this form's fill (gray plate under Demarrer/Arreter).
                  UI_SkinCanvas_RestoreBackdropIntersection(this, overlappingoption, bd_x, bd_y, bd_x + bd_w, bd_y + bd_h);
                }

              // SIBLING-CONTAMINATION FIX (2026-09): root cause of "quedan rastros de la barra azul en la opcion
              // previamente seleccionada" -- see FormBackdrop_FindOverlapping()'s own header comment for the full
              // root-cause writeup (confirmed live via a raw-pixel dump of the exact contaminated capture).
              // UNLIKE the option-family case just above, a form-family sibling (e.g. "nav-<section>-hl", drawn
              // immediately before its sibling "nav-<section>-bar" in the same frame) does NOT repaint itself
              // again later this same frame -- its own Draw_Form() call already finished for good. Restoring its
              // FULL cached box here (the option-family pattern) would therefore permanently blank out whatever
              // part of its fill lies OUTSIDE our own capture box, since nothing would ever repaint that part
              // again. Instead, restore only the INTERSECTION of our own box with the sibling's cached box --
              // enough to strip its fresh paint out of the region we are about to capture, without touching any
              // of its fill outside that region. Safe even for the part we DO touch: our own paint (steps 4-8,
              // immediately after this capture, in this very same call) unconditionally repaints our own whole
              // box right afterwards anyway, so nothing reaches the screen still erased.
              GRP2DREBUILDAREA* overlappingform = FormBackdrop_FindOverlapping(bd_x, bd_y, bd_w, bd_h);
              if(overlappingform && overlappingform->GetBitmap())
                {
                  double sib_x = overlappingform->GetXPos();
                  double sib_y = overlappingform->GetYPos();
                  double sib_w = (double)overlappingform->GetBitmap()->GetWidth();
                  double sib_h = (double)overlappingform->GetBitmap()->GetHeight();

                  double ix1 = (bd_x > sib_x) ? bd_x : sib_x;
                  double iy1 = (bd_y > sib_y) ? bd_y : sib_y;
                  double ix2 = ((bd_x + bd_w) < (sib_x + sib_w)) ? (bd_x + bd_w) : (sib_x + sib_w);
                  double iy2 = ((bd_y + bd_h) < (sib_y + sib_h)) ? (bd_y + bd_h) : (sib_y + sib_h);

                  if((ix2 > ix1) && (iy2 > iy1))
                    {
                      GRPRECTINT croprect((int)(ix1 - sib_x), (int)(iy1 - sib_y), (int)(ix2 - sib_x), (int)(iy2 - sib_y));

                      GRPBITMAP* cropped = overlappingform->GetBitmap()->GetSubBitmap(croprect);
                      if(cropped)
                        {
                          PutBitmapNoAlpha(ix1, iy1, cropped);
                          GEN_DELETE cropped;
                        }
                    }
                }

              // First time this element is ever drawn: nothing has painted shadow/fill/border/children ink
              // here yet, so this is the one guaranteed-pristine moment to capture the true backdrop.
              // (Do NOT TextBackdrop_InvalidateOverlapping here: a title TextBackdrop stores caption-fill
              // pixels, not the true under-form backdrop -- restoring it before this capture would bake
              // that fill into FormBackdrop and break ALPHA-DARKENING on later redraws.)
              FormBackdrop_Capture(element, bd_x, bd_y, bd_w, bd_h);
              formbackdrop_for_punch = FormBackdrop_Find(element);
            }
           else
            {
              // Not the first draw: restore the true backdrop now, before repainting below.
              PutBitmapNoAlpha(formbackdrop->GetXPos(), formbackdrop->GetYPos(), formbackdrop->GetBitmap());
              formbackdrop_for_punch = formbackdrop;

              // Restoring just wiped any ink our own children (an edit field, listbox rows, a scrollbar...)
              // already painted on earlier ticks. The children loop further down in this function already
              // calls Draw() on every child unconditionally, but each Draw_X() only actually repaints when
              // THAT child's own MustReDraw() is set -- so a child not ALSO marked dirty this exact tick
              // would be erased by the restore above and never repainted. Force every direct child dirty here
              // so the loop below always repaints it on top of the freshly restored backdrop. Harmless when a
              // child WAS already going to redraw anyway (repainting unchanged content is idempotent, the
              // same reasoning already relied on elsewhere in this file -- see the GHOST-FILL FIX comment in
              // Draw_ProgressBar()).
              for(XDWORD c=0; c<element_form->GetComposeElements()->GetSize(); c++)
                {
                  UI_ELEMENT* formchild = (UI_ELEMENT*)element_form->GetComposeElements()->Get(c);
                  if(formchild) formchild->SetMustReDraw(true);
                }

              // FormBackdrop restore just PutBitmapNoAlpha'd over any overlapping option/text ink (e.g.
              // sidebar_bg wiping nav icons under nav-*-btn). Selected nav-hit buttons are "busy" so the
              // later InvalidateOverlapping(force=false) would KEEP a stale OptionBackdrop and re-paint a
              // blank strip forever (video 2026-09-20: sidebar empty until click). Force-discard now so the
              // buttons recapture AFTER compose children / later siblings have redrawn this frame.
              OptionBackdrop_InvalidateOverlapping(bd_x, bd_y, bd_w, bd_h, true);
              TextBackdrop_InvalidateOverlapping(bd_x, bd_y, bd_w, bd_h, element);
              AnimBackdrop_InvalidateOverlapping(bd_x, bd_y, bd_w, bd_h, element);

              // Cards in UI_System overlap each other by ~22px vertically even with AABB-only FormBackdrop;
              // dirty any layout sibling whose box intersects the restore rect so neighbour ink is redrawn
              // this frame (see UI_SkinCanvas_DirtyOverlappingLayoutSiblings). Also covers brand_* roots
              // sitting on the sidebar column when sidebar_bg restores.
              UI_SkinCanvas_DirtyOverlappingLayoutSiblings(element, bd_x, bd_y, bd_w, bd_h);
            }
        }

      // MID-EPISODE STALENESS GAP fix (2026-09): about to paint real ink of our own (haspaintable) over this
      // box -- any option-family element (see optionbackdrops' own comment in UI_SkinCanvas.h) whose cached
      // "true backdrop" overlaps it is now stale, whether or not that element's own hover/selection episode
      // ever went idle in between. Prefer "ownarea" (this tick's actual rebuild-area box, already expanded for
      // the box-shadow footprint) when this element owns one; otherwise fall back to the plain visible-rect --
      // still correct, just without the shadow's extra margin.
      //
      // ACCENT-BAR NEVER APPEARS FIX (2026-09): root cause of "la franja azul... por que no aparece en otra
      // opcion cuando la seleccionas". UI_System's "nav-<section>-bar" starts invisible for every section but
      // the default one, so the FIRST time a user selects e.g. "CPU", THIS Draw_Form call is that element's
      // one-and-only first-ever draw (formneverdrawnbefore, set above). At that exact moment the overlapping
      // "nav-cpu-btn" hit-target is almost always ALSO mid-hover (the user's cursor is sitting on the row they
      // just clicked) -- so the REGRESSION FIX guard just below (in OptionBackdrop_InvalidateOverlapping()
      // itself) reports it "busy" and refuses to discard its cached backdrop, exactly as intended for an
      // UNRELATED neighbour's incidental overlap (the original STUCK-PRESELECT-AFTER-LONG-HOVER scenario).
      // But this overlap is not incidental: "nav-cpu-btn"'s cache was captured BEFORE the bar ever existed on
      // screen (it starts invisible, so nothing could have painted it into any earlier capture), so it is
      // unconditionally stale here regardless of how busy the button currently is -- confirmed live: the bar
      // paints its correct accent-blue fill (see the PAINTING FILL trace below) and is erased on the very next
      // hover-driven OptionBackdrop restore of "nav-cpu-btn", which keeps re-painting that pre-bar snapshot for
      // as long as the row stays hovered/selected, i.e. essentially forever from the user's point of view. Pass
      // "force" only for this one-time case: a "busy" option can never have legitimately painted-over content
      // that predates an element which, by definition, has never painted anything before this exact call.
      if(haspaintable)
        {
          if(ownarea)
            {
              OptionBackdrop_InvalidateOverlapping(ownarea->GetXPos(), ownarea->GetYPos(), (double)ownarea->GetBitmap()->GetWidth(), (double)ownarea->GetBitmap()->GetHeight(), formneverdrawnbefore);

              // STALE CAPTION BACKGROUND FIX (2026-09): see TextBackdrop_InvalidateOverlapping()'s own header
              // comment for the full root-cause writeup -- this form is about to paint real ink over this same
              // box, so any text element's cached backdrop overlapping it (e.g. a sidebar label sitting on top
              // of a selection-wash band) is now stale too, exactly like the option case just above.
              TextBackdrop_InvalidateOverlapping(ownarea->GetXPos(), ownarea->GetYPos(), (double)ownarea->GetBitmap()->GetWidth(), (double)ownarea->GetBitmap()->GetHeight(), element);
              AnimBackdrop_InvalidateOverlapping(ownarea->GetXPos(), ownarea->GetYPos(), (double)ownarea->GetBitmap()->GetWidth(), (double)ownarea->GetBitmap()->GetHeight(), element);
            }
           else
            {
              OptionBackdrop_InvalidateOverlapping(vr_minx, vr_miny, vr_maxx - vr_minx, vr_maxy - vr_miny, formneverdrawnbefore);

              // STALE CAPTION BACKGROUND FIX (2026-09): see the ownarea branch just above.
              TextBackdrop_InvalidateOverlapping(vr_minx, vr_miny, vr_maxx - vr_minx, vr_maxy - vr_miny, element);
              AnimBackdrop_InvalidateOverlapping(vr_minx, vr_miny, vr_maxx - vr_minx, vr_maxy - vr_miny, element);
            }
        }

      // Structural transparent forms (no fill / authored border / shadow): do not FormBackdrop or stroke.
      // Compact toolbar-legend forms (Demarrer/Arreter under glyph boxes) still get an opaque page wipe so
      // neighbour multioption peels cannot leave a gray plate under the glyphs. Must NOT wipe:
      //   - chrome-role elements (UI_Options caption color="black,0" is drag-hit-test only — painting
      //     page ink 24,28,36 there made the "transparent" chrome a solid black/dark band);
      //   - wide/tall transparent containers that intentionally show content underneath.
      // Same size gate as RepaintCompactLegendForms (height<=28, width<=120).
      if(!haspaintable && canvas &&
         (element->GetChromeRole() == UI_ELEMENT_CHROMEROLE_NONE) &&
         (element->GetBoundaryLine()->width  <= 120.0) &&
         (element->GetBoundaryLine()->height <= 28.0))
        {
          XBYTE pr = 24, pg = 28, pb = 36;
          UI_LAYOUT* layout = element->GetLayout();
          if(layout && layout->GetBackground() && layout->GetBackground()->GetColor() &&
             layout->GetBackground()->GetColor()->IsValid())
            {
              UI_COLOR* page = layout->GetBackground()->GetColor();
              pr = (XBYTE)page->GetRed();
              pg = (XBYTE)page->GetGreen();
              pb = (XBYTE)page->GetBlue();
            }
          double fx = UI_BOUNDARYLINE_EdgeLeft(x_position, element->GetBoundaryLine()->width);
          double fw = element->GetBoundaryLine()->width;
          double fh = element->GetBoundaryLine()->height;
          if(fh < 14.0) fh = 14.0;
          fh += 8.0;
          double fy = y_position - (fh - 8.0);
          if(fw < 1.0) fw = 1.0;
          if(fh < 1.0) fh = 1.0;

          XRECT form_clip_saved;
          canvas->GetClipBox(form_clip_saved);
          canvas->SetClipBox(fx, fy + fh, fx + fw, fy);

          GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
          int fh_i = (int)fh;
          int fw_i = (int)fw;
          if(fh_i < 1) fh_i = 1;
          if(fw_i < 1) fw_i = 1;
          for(int py = 0; py < fh_i; py++)
            {
              for(int px = 0; px < fw_i; px++)
                {
                  canvas->PutPixel(fx + (double)px, fy + (double)py, (const GRP2DCOLOR*)&pagecol);
                }
            }

          canvas->SetClipBox(form_clip_saved);

          // PreDraw may have snapshotted the neighbour-contaminated strip into the rebuild area
          // BEFORE this page wipe. Rewrite the whole ownarea with page colour (do NOT require
          // size match with the inflated fh wipe — CreateRebuildArea uses the authored height
          // ±1 while the wipe uses height+8, so the old tolerance never fired and RebuildAllAreas
          // kept resurrecting the gray plate under Demarrer/Arreter).
          if(ownarea && ownarea->GetBitmap())
            {
              GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
              for(XDWORD py = 0; py < ownarea->GetBitmap()->GetHeight(); py++)
                {
                  for(XDWORD px = 0; px < ownarea->GetBitmap()->GetWidth(); px++)
                    {
                      ownarea->GetBitmap()->PutPixel((int)px, (int)py, (const GRP2DCOLOR*)&pagecol);
                    }
                }
            }
        }

      if(haspaintable)
      {

      // Step 7-8: box-shadow. Drawn BEFORE the fill so the form paints on top of it. When blur > 0 the skin
      // rasterises the silhouette into an off-screen RGBA bitmap, applies a 3-pass box blur (Gaussian
      // approximation, visually equivalent to agg::stack_blur_rgba32 for typical UI radii) and composites
      // via PutBitmapAlpha. Falls back to the hard-edged variant when the canvas mode is not 32-bit
      // RGBA/BGRA, when blur is 0, or when the off-screen allocation fails. The rebuild-area was already
      // expanded in PreDrawFunction to include the blurred footprint so save/restore is safe.
      if(element_form->IsBoxShadowSet())
        {
          double sh_x    = element_form->GetShadowOffsetX();
          double sh_y    = element_form->GetShadowOffsetY();
          double sh_blur = element_form->GetShadowBlur();

          double sh_minx = vr_minx + sh_x;
          double sh_miny = vr_miny + sh_y;
          double sh_maxx = vr_maxx + sh_x;
          double sh_maxy = vr_maxy + sh_y;

          // Resolve corner radii once: per-corner if any is authored, else uniform roundrect on all four.
          double rTL, rTR, rBR, rBL;
          if(element_form->HasAnyPerCornerRadius())
            {
              rTL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TL);
              rTR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TR);
              rBR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BR);
              rBL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BL);
            }
           else
            {
              double r = (double)element_form->GetRoundRect();
              rTL = rTR = rBR = rBL = r;
            }

          bool soft_ok = false;
          if(sh_blur > 0.0)
            {
              // Cached path (see UI_SkinCanvas_DrawSoftShadow_FormCached): reuses the already-blurred bitmap
              // on "element_form" across frames instead of rebuilding it every redraw.
              soft_ok = UI_SkinCanvas_DrawSoftShadow_FormCached(canvas, element_form, sh_minx, sh_miny, sh_maxx, sh_maxy, rTL, rTR, rBR, rBL, element_form->GetShadowColor(), (int)sh_blur);
            }

          if(!soft_ok)
            {
              // Hard shadow fallback (no blur, or canvas mode not 32-bit).
              GRP2DCOLOR_RGBA8  shadow_col(element_form->GetShadowColor()->GetRed(), element_form->GetShadowColor()->GetGreen(), element_form->GetShadowColor()->GetBlue(), element_form->GetShadowColor()->GetAlpha());

              GRP2DCOLOR_RGBA8  shadow_line_none(0, 0, 0, 0);

              canvas->SetFillColor(&shadow_col);
              canvas->SetLineColor(&shadow_line_none);
              canvas->SetLineWidth(1.0f);

              if(element_form->HasAnyPerCornerRadius())
                {
                  GRP2DPATH shpath;
                  AppendRoundRectPathPerCorner(shpath, sh_minx, sh_miny, sh_maxx, sh_maxy, rTL, rTR, rBR, rBL);
                  canvas->Path(shpath, true);
                }
               else if(element_form->GetRoundRect())
                {
                  canvas->RoundRect(sh_minx, sh_maxy, sh_maxx, sh_miny, element_form->GetRoundRect(), true);
                }
               else
                {
                  canvas->Rectangle(sh_minx, sh_maxy, sh_maxx, sh_miny, true);
                }
            }
        }

      // Step 6: CSS-natural fill. Prefer background_color when authored (via "bckgrdcolor" or the CSS-standard
      // alias "background-color"); fall back to the historical "color" property for retro-compat with layouts
      // that predate this unification. A one-time XTRACE warning per fallback element helps authors migrate.
      UI_COLOR* fillsrc = element_form->IsBackgroundColorSet() ? element_form->GetBackgroundColor()
                                                               : element_form->GetColor();

      if(!element_form->IsBackgroundColorSet() && !element_form->GetLegacyFillWarningEmitted())
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, NL("[UI Draw] Form [%s]: fill via legacy \"color\" property; migrate to \"bckgrdcolor\" (or CSS \"background-color\") for CSS-natural semantics"), element_form->GetName() ? element_form->GetName()->Get() : NL("(unnamed)"));
          element_form->SetLegacyFillWarningEmitted(true);
        }

      GRP2DCOLOR_RGBA8  color(fillsrc->GetRed(), fillsrc->GetGreen(), fillsrc->GetBlue(), fillsrc->GetAlpha());

      // Step 5: prefer base-level border-color if authored, otherwise fall back to the historical per-type
      // linecolor member so pre-step-5 layouts render unchanged.
      UI_COLOR* bcsrc = element_form->IsBorderColorSet() ? element_form->GetBorderColor()
                                                         : element_form->GetLineColor();

      GRP2DCOLOR_RGBA8  linecolor(bcsrc->GetRed(), bcsrc->GetGreen(), bcsrc->GetBlue(), bcsrc->GetAlpha());

      canvas->SetLineColor(&linecolor);
      canvas->SetFillColor(&color);

      // Step 4: honour element-configured border-width. -1 = default (historical 1.0f for containers), 0 = no
      // stroke drawn at all (fill-only), >0 = author-specified thickness. AGG's RoundRect / Rectangle draw a
      // stroke along with the fill even when linewidth is 0, so the "no stroke" case is handled by pushing a
      // fully transparent line color instead of relying on width 0.
      double bw = element_form->GetBorderWidth();

      if(bw == 0.0)
        {
          GRP2DCOLOR_RGBA8  linecolor_none(0, 0, 0, 0);
          canvas->SetLineColor(&linecolor_none);
          canvas->SetLineWidth(1.0f);
        }
       else
        {
          canvas->SetLineWidth((bw < 0.0) ? 1.0f : bw);
        }

      // RoundRect stroke AA + AABB seed/punch need ~1px outside the authored box. PreDraw pads
      // overflow:hidden clips symmetrically now, but expand here too so the frame still completes when
      // the active clip is flush to the box (nested scrolleables / parent intersection). Restore before
      // compose children so overflow clipping stays tight for content.
      XRECT frame_clip_saved;
      bool  frame_clip_expanded = false;
      if(element_form->GetRoundRect() || element_form->HasAnyPerCornerRadius())
        {
          canvas->GetClipBox(frame_clip_saved);
          double fminx = __MIN(frame_clip_saved.x1, frame_clip_saved.x2) - 1.0;
          double fmaxx = __MAX(frame_clip_saved.x1, frame_clip_saved.x2) + 1.0;
          double fminy = __MIN(frame_clip_saved.y1, frame_clip_saved.y2) - 1.0;
          double fmaxy = __MAX(frame_clip_saved.y1, frame_clip_saved.y2) + 1.0;
          canvas->SetClipBox(fminx, fmaxy, fmaxx, fminy);
          frame_clip_expanded = true;
        }

      // Step 5: per-corner radius. When any corner carries an authored radius, build a custom path and draw
      // via canvas->Path so each corner arcs with its own radius. Otherwise keep the classic single-radius
      // canvas->RoundRect call, which is the fastest path and unchanged from step 4.
      //
      // CORNER SPIKES FIX (2026-09): CSS "border-radius: N" sets all four per-corner slots, so Draw_Form used
      // to take the Path branch even when every radius is identical. AppendRoundRectPathPerCorner emits a coarse
      // polyline (MoveTo/LineTo only); GRP2DCanvasAGG::Path then strokes it with agg::miter_join. Miters on those
      // chord vertices poke sharp triangular "picos" past the true rounded corner -- exactly the dark tips visible
      // in the gutters between UI_System cards. When all four radii match, use AGG's native rounded_rect instead
      // (same path as the classic GetRoundRect() branch). Unequal corners still use Path, but with more arc steps
      // so miters (if any remain) stay tiny.
      if(element_form->HasAnyPerCornerRadius())
        {
          double rTL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TL);
          double rTR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_TR);
          double rBR = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BR);
          double rBL = element_form->GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER_BL);

          bool uniform = (rTL == rTR) && (rTR == rBR) && (rBR == rBL) && (rTL > 0.0);

          // Father's CSS background is the true parent ink for nested panels (datetime_box on card).
          GRP2DCOLOR_RGBA8 corner_parent(0, 0, 0, 255);
          const GRP2DCOLOR_RGBA8* corner_parent_ptr = NULL;
          {
            UI_ELEMENT* father = element_form->GetFather();
            UI_COLOR* fbg = NULL;
            if(father)
              {
                if(father->IsBackgroundColorSet()) fbg = father->GetBackgroundColor();
                else fbg = father->GetColor();
              }
            if(fbg && fbg->GetAlpha() > 0 && (fbg->GetRed() + fbg->GetGreen() + fbg->GetBlue()) > 8)
              {
                corner_parent = GRP2DCOLOR_RGBA8((XBYTE)fbg->GetRed(), (XBYTE)fbg->GetGreen(), (XBYTE)fbg->GetBlue(), (XBYTE)fbg->GetAlpha());
                corner_parent_ptr = &corner_parent;
              }
          }

          // Opaque UI_System page fallback (0x191E24) must NOT run on GRPSCREENSTYLE_TRANSPARENT:
          // seeding the AABB opaque then painting color="black,N" blends to an opaque gray panel
          // (UI_Message frosted dialog) instead of keeping per-pixel alpha for UpdateLayeredWindow.
          GRP2DCOLOR_RGBA8 seed_bg(0, 0, 0, 0);
          if(corner_parent_ptr)
            {
              seed_bg = *corner_parent_ptr;
            }
           else if(screen && !screen->Style_Is(GRPSCREENSTYLE_TRANSPARENT))
            {
              seed_bg = GRP2DCOLOR_RGBA8(0x19, 0x1E, 0x24, 255);
            }
          if(seed_bg.a > 0)
          {
            GRP2DCOLOR_RGBA8 line_none(0, 0, 0, 0);
            canvas->SetFillColor(&seed_bg);
            canvas->SetLineColor(&line_none);
            canvas->SetLineWidth(1.0f);
            canvas->Rectangle(vr_minx - 1.0, vr_maxy + 1.0, vr_maxx + 1.0, vr_miny - 1.0, true);

            UI_COLOR* fillsrc2 = element_form->IsBackgroundColorSet() ? element_form->GetBackgroundColor()
                                                                       : element_form->GetColor();
            UI_COLOR* bcsrc2   = element_form->IsBorderColorSet() ? element_form->GetBorderColor()
                                                                   : element_form->GetLineColor();
            GRP2DCOLOR_RGBA8 fill2(fillsrc2->GetRed(), fillsrc2->GetGreen(), fillsrc2->GetBlue(), fillsrc2->GetAlpha());
            GRP2DCOLOR_RGBA8 line2(bcsrc2->GetRed(), bcsrc2->GetGreen(), bcsrc2->GetBlue(), bcsrc2->GetAlpha());
            double bw2 = element_form->GetBorderWidth();
            canvas->SetFillColor(&fill2);
            if(bw2 == 0.0) { GRP2DCOLOR_RGBA8 ln(0,0,0,0); canvas->SetLineColor(&ln); canvas->SetLineWidth(1.0f); }
            else { canvas->SetLineColor(&line2); canvas->SetLineWidth((bw2 < 0.0) ? 1.0f : bw2); }
          }

          if(uniform)
            {
              canvas->RoundRect(vr_minx, vr_maxy, vr_maxx, vr_miny, rTL, true);
            }
           else
            {
              GRP2DPATH path;
              AppendRoundRectPathPerCorner(path, vr_minx, vr_miny, vr_maxx, vr_maxy, rTL, rTR, rBR, rBL);

              canvas->Path(path, true);       // fill
              canvas->Path(path, false);      // stroke (transparent line colour when border-width == 0)
            }

          // CORNER SPIKES FIX: punch AABB cutouts with father/FormBackdrop parent ink.
          // Skip when the AABB was just seeded with parent ink — RoundRect AA then blends against that
          // seed (no pico negro), and punching the tip box was eating the bottom border stroke on
          // overflow:hidden rounded forms (opstatus foot BL/BR arcs).
          if(seed_bg.a == 0)
            {
              UI_SkinCanvas_PunchRoundRectCornerTips(canvas, formbackdrop_for_punch, vr_minx, vr_miny, vr_maxx, vr_maxy, rTL, rTR, rBR, rBL, &seed_bg);
            }
        }
       else if(element_form->GetRoundRect())
        {
          // Use the same padded content box (vr_*) as the per-corner branch so fill, stroke and tip-punch agree.
          // Sample TRUE backdrop BEFORE RoundRect paints -- after paint, outside GetPixel can still see AA tips.
          GRP2DCOLOR_RGBA8 corner_parent(0, 0, 0, 255);
          const GRP2DCOLOR_RGBA8* corner_parent_ptr = NULL;
          {
            UI_ELEMENT* father = element_form->GetFather();
            UI_COLOR* fbg = NULL;
            if(father)
              {
                if(father->IsBackgroundColorSet()) fbg = father->GetBackgroundColor();
                else fbg = father->GetColor();
              }
            if(fbg && fbg->GetAlpha() > 0 && (fbg->GetRed() + fbg->GetGreen() + fbg->GetBlue()) > 8)
              {
                corner_parent = GRP2DCOLOR_RGBA8((XBYTE)fbg->GetRed(), (XBYTE)fbg->GetGreen(), (XBYTE)fbg->GetBlue(), (XBYTE)fbg->GetAlpha());
                corner_parent_ptr = &corner_parent;
              }
            // Do not sample canvas neighbours on transparent screens: leftover/opaque junk would
            // become an opaque seed under black,N and destroy the frosted alpha (same as 0x191E24).
            if(!corner_parent_ptr && screen && !screen->Style_Is(GRPSCREENSTYLE_TRANSPARENT))
              {
                const double sx[4] = { vr_minx - 2.0, vr_maxx + 1.0, vr_maxx + 1.0, vr_minx - 2.0 };
                const double sy[4] = { vr_miny - 2.0, vr_miny - 2.0, vr_maxy + 1.0, vr_maxy + 1.0 };
                for(int i = 0; i < 4 && !corner_parent_ptr; i++)
                  {
                    GRP2DCOLOR* c = canvas->GetPixel(sx[i], sy[i]);
                    if(!c) continue;
                    GRP2DCOLOR_RGBA8 s = *(GRP2DCOLOR_RGBA8*)c;
                    if(s.a > 0 && (s.r + s.g + s.b) > 8)
                      {
                        corner_parent = s;
                        corner_parent_ptr = &corner_parent;
                      }
                  }
              }
          }

          double rr = (double)element_form->GetRoundRect();

          // Seed the AABB with true backdrop BEFORE RoundRect. AGG AA of the rounded fill/border otherwise
          // blends against leftover black in the corner tips and shows as "pico negro".
          // On transparent screens the fallback must stay a=0 (see per-corner branch above).
          GRP2DCOLOR_RGBA8 seed_bg(0, 0, 0, 0);
          if(corner_parent_ptr)
            {
              seed_bg = *corner_parent_ptr;
            }
           else if(screen && !screen->Style_Is(GRPSCREENSTYLE_TRANSPARENT))
            {
              seed_bg = GRP2DCOLOR_RGBA8(0x19, 0x1E, 0x24, 255);
            }
          if(seed_bg.a > 0)
          {
            GRP2DCOLOR_RGBA8 line_none(0, 0, 0, 0);
            canvas->SetFillColor(&seed_bg);
            canvas->SetLineColor(&line_none);
            canvas->SetLineWidth(1.0f);
            canvas->Rectangle(vr_minx - 1.0, vr_maxy + 1.0, vr_maxx + 1.0, vr_miny - 1.0, true);
          }

          // Re-apply the form's own fill/line colours for RoundRect (seed Rectangle may have overwritten them).
          {
            UI_COLOR* fillsrc2 = element_form->IsBackgroundColorSet() ? element_form->GetBackgroundColor()
                                                                       : element_form->GetColor();
            UI_COLOR* bcsrc2   = element_form->IsBorderColorSet() ? element_form->GetBorderColor()
                                                                   : element_form->GetLineColor();
            GRP2DCOLOR_RGBA8 fill2(fillsrc2->GetRed(), fillsrc2->GetGreen(), fillsrc2->GetBlue(), fillsrc2->GetAlpha());
            GRP2DCOLOR_RGBA8 line2(bcsrc2->GetRed(), bcsrc2->GetGreen(), bcsrc2->GetBlue(), bcsrc2->GetAlpha());
            double bw2 = element_form->GetBorderWidth();
            if(bw2 == 0.0)
              {
                GRP2DCOLOR_RGBA8 line_none2(0, 0, 0, 0);
                canvas->SetLineColor(&line_none2);
                canvas->SetLineWidth(1.0f);
              }
             else
              {
                canvas->SetLineColor(&line2);
                canvas->SetLineWidth((bw2 < 0.0) ? 1.0f : bw2);
              }
            canvas->SetFillColor(&fill2);
          }

          canvas->RoundRect(vr_minx, vr_maxy, vr_maxx, vr_miny, rr, true);
          // See per-corner branch: seeded AABB already kills pico negro; punch was clipping BL/BR stroke.
          if(seed_bg.a == 0)
            {
              UI_SkinCanvas_PunchRoundRectCornerTips(canvas, formbackdrop_for_punch, vr_minx, vr_miny, vr_maxx, vr_maxy, rr, rr, rr, rr, &seed_bg);
            }
        }
       else
        {
          canvas->Rectangle(element_form->GetVisibleRect()->x, element_form->GetVisibleRect()->y, element_form->GetVisibleRect()->x + element_form->GetVisibleRect()->width, element_form->GetVisibleRect()->GetTop(), true);
        }

      if(frame_clip_expanded)
        {
          canvas->SetClipBox(frame_clip_saved);
        }

      // Opaque fill just replaced neighbour peel residue on the canvas. PreDraw may still hold a
      // rebuild-area snapshot taken BEFORE that fill (gray plate under toolbar legends). Rewrite the
      // matching ownarea with the fill colour so RebuildAllAreas cannot resurrect the plate.
      if(ownarea && ownarea->GetBitmap() && fillsrc && (fillsrc->GetAlpha() == 255))
        {
          double fw = vr_maxx - vr_minx;
          double fh = vr_maxy - vr_miny;
          double ow = (double)ownarea->GetBitmap()->GetWidth();
          double oh = (double)ownarea->GetBitmap()->GetHeight();
          // CreateRebuildArea pads +1 — tolerate so opaque legend forms rewrite too.
          if((fabs(ow - fw) <= 2.0) && (fabs(oh - fh) <= 2.0))
            {
              GRP2DCOLOR_RGBA8 fillcol((XBYTE)fillsrc->GetRed(), (XBYTE)fillsrc->GetGreen(),
                                       (XBYTE)fillsrc->GetBlue(), 255);
              for(XDWORD py = 0; py < ownarea->GetBitmap()->GetHeight(); py++)
                {
                  for(XDWORD px = 0; px < ownarea->GetBitmap()->GetWidth(); px++)
                    {
                      ownarea->GetBitmap()->PutPixel((int)px, (int)py, (const GRP2DCOLOR*)&fillcol);
                    }
                }
            }
        }
      } // haspaintable: own ink (shadow/fill/stroke)
    }

  for(XDWORD c=0; c<element_form->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = (UI_ELEMENT*)element->GetComposeElements()->Get(c);
      if(subelement) 
        {
          Draw(subelement);
        }  
    }
  
  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_Menu(UI_ELEMENT* element)
* @brief      Draw menu
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_Menu(UI_ELEMENT* element)
{
  return Draw_Form(element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_ListBox(UI_ELEMENT* element)
* @brief      Draw list box
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_ListBox(UI_ELEMENT* element)
{
  if(!element) return false;  

  UI_ELEMENT_LISTBOX*   element_listbox = (UI_ELEMENT_LISTBOX*)element;
  double                x_position      = 0.0f;
  double                y_position      = 0.0f;
  GRP2DCANVAS*            canvas          = GetCanvas();    
  static bool           visible         = false;
  XRECT                 clip_rect;                       
  
  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position, UI_SKINCANVAS_EDIT_MAXEDGE);

  if(element->MustReDraw()) 
    {
      GRP2DCOLOR_RGBA8  color(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());

      GRP2DCOLOR_RGBA8  bkgcolor(element->GetBackgroundColor()->GetRed(), element->GetBackgroundColor()->GetGreen(), element->GetBackgroundColor()->GetBlue(), element_listbox->IsPreSelect()?element->GetBackgroundColor()->GetAlpha()/2:element->GetBackgroundColor()->GetAlpha());

      GRP2DCOLOR_RGBA8  linecolor;

      GRP2DCOLOR_RGBA8  colorwhite(255, 255, 255, 255);
 
  
      canvas->SetLineColor(&linecolor);
      canvas->SetFillColor(&bkgcolor);

      if(element->GetRoundRect())
        {
          canvas->RoundRect(element->GetXPosition() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetYPosition() + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetXPosition() + element->GetBoundaryLine()->width + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetTopY() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetRoundRect(), true);
        }
       else
        {
          canvas->Rectangle(element->GetXPosition() - UI_SKINCANVAS_EDIT_MAXEDGE, element->GetYPosition() + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetXPosition() + element->GetBoundaryLine()->width + UI_SKINCANVAS_EDIT_MAXEDGE, element->GetTopY() - UI_SKINCANVAS_EDIT_MAXEDGE, true);
        }
                                      
      canvas->Vectorfont_GetConfig()->SetColor(&color);
      canvas->Vectorfont_GetConfig()->SetSize(element_listbox->GetSizeFont());  
      canvas->VectorFont_Printf(element->GetXPosition(), element->GetYPosition(), element_listbox->GetText()->Get());
    }
  
  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_ProgressBar(UI_ELEMENT* element)
* @brief      Draw progress bar
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_ProgressBar(UI_ELEMENT* element)
{
  if(!element) return false;  

  UI_ELEMENT_PROGRESSBAR* element_progressbar   = (UI_ELEMENT_PROGRESSBAR*)element;
  UI_ELEMENT*             element_progressrect  = element_progressbar->GetProgressRect();
  UI_ELEMENT_ANIMATION*   element_animation     = element_progressbar->Get_UIAnimation();
  UI_ELEMENT_TEXT*        element_text          = element_progressbar->Get_UIText();
  double                  x_position            = 0.0f;
  double                  y_position            = 0.0f;
  GRP2DCANVAS*              canvas                = GetCanvas();    
  static bool             visible               = false;
  XRECT                   clip_rect;

  if(!canvas) return false;

  // Our OWN rebuild-area (created by PreDrawFunction() right below, sized from GetXPosition()/GetYPosition()/
  // GetBoundaryLine()) must cover only what OUR OWN painting further down in this function actually repaints --
  // element_progressrect's track/fill (and element_animation's frame, if present). For an AUTO-sized progress
  // bar whose caption sits OUTSIDE the rect (allocationtext DOWN/UP/LEFT/RIGHT), GetBoundaryLine() is
  // deliberately bigger than that: it spans rect+gap+caption too, so SIBLINGS lay out around the whole widget,
  // caption included (see the width/height AUTO block in CalculateBoundaryLine_ProgressBar()). Reusing that
  // same box to size OUR rebuild-area conflates "how big I am for layout" with "what my own redraw repaints" --
  // element_text (below) now owns its own independent rebuild-area, so if OUR area still also covers the
  // caption strip, restoring it later pastes OUR stale "before" caption snapshot back over whatever
  // element_text's own, more current area just correctly settled: the caption vanishes/overlaps after a real
  // value change (root cause of the long-standing progressbar caption defect). Fix: temporarily shrink our own
  // position/size to the exact union of element_progressrect + element_animation's CURRENT absolute bounds --
  // already correctly placed by CalculateBoundaryLine_ProgressBar() -- for this one PreDrawFunction() call,
  // then restore the real (caption-inclusive) box immediately after so every other use of GetBoundaryLine()/
  // GetXPosition()/GetYPosition() this tick (siblings' layout, hit-testing, etc.) is unaffected.
  double savedx      = element->GetXPosition();
  double savedy      = element->GetYPosition();
  double savedwidth  = element->GetBoundaryLine()->width;
  double savedheight = element->GetBoundaryLine()->height;
  bool   shrunkarea  = false;

  if(element_text && element_progressrect &&
     (element_progressbar->GetAllocationTextType() != UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE) &&
     (element_progressbar->GetAllocationTextType() != UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER))
    {
      double left   = element_progressrect->GetXPosition();
      double right  = element_progressrect->GetXPosition() + element_progressrect->GetBoundaryLine()->width;
      double bottom = element_progressrect->GetYPosition();
      double top    = element_progressrect->GetYPosition() - element_progressrect->GetBoundaryLine()->height;

      if(element_animation)
        {
          left   = __MIN(left, element_animation->GetXPosition());
          right  = __MAX(right, element_animation->GetXPosition() + element_animation->GetBoundaryLine()->width);
          bottom = __MAX(bottom, element_animation->GetYPosition());
          top    = __MIN(top, element_animation->GetYPosition() - element_animation->GetBoundaryLine()->height);
        }

      // Pull every edge in by a small, fixed safety margin (well under the >=6px gap every allocationtext case
      // in CalculateBoundaryLine_ProgressBar() leaves between the rect/animation and the caption -- see the
      // "+ 6" in each branch there). Without this, PreDrawFunction()'s own "edge" padding on this area and on
      // element_text's OWN independent area (added a few lines below) can make the two areas touch or overlap
      // by a pixel or two even though the real content does not -- and if they do, MarkOverlappingAreasDirty()
      // (called from RebuildAllAreas() before every restore) perpetually re-marks each dirty because of the
      // other, which defeats element_text's own one-shot capture/redraw/settle cycle exactly as badly as the
      // original bug this whole block exists to avoid. The margin only shrinks what OUR OWN rebuild-area
      // covers, never the actual drawing below, so it cannot clip anything on screen.
      const double SAFETY_MARGIN = 3.0;

      left   += SAFETY_MARGIN;
      right  -= SAFETY_MARGIN;
      top    += SAFETY_MARGIN;
      bottom -= SAFETY_MARGIN;

      element->SetXPosition(left);
      element->SetYPosition(bottom);
      element->GetBoundaryLine()->width  = __MAX(0.0, right - left);
      element->GetBoundaryLine()->height = __MAX(0.0, bottom - top);

      shrunkarea = true;
    }

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position, 1);

  if(shrunkarea)
    {
      element->SetXPosition(savedx);
      element->SetYPosition(savedy);
      element->GetBoundaryLine()->width  = savedwidth;
      element->GetBoundaryLine()->height = savedheight;
    }

  if(element->MustReDraw())
    {
      // GHOST-FILL FIX (2026-09, follow-up to the two fixes above): CalculateBoundaryLine_ProgressBar() can
      // MOVE element_progressrect (and element_animation) to make room for the caption -- for DOWN it shifts
      // the rect's bottom edge inward (up), for LEFT its right edge inward, freeing a strip that used to be
      // part of the rect's own painted area (border AND fill). The very first time this widget is laid out,
      // nothing has painted anything there yet, so the shift is invisible; but from the first REAL value
      // change onward, this function re-runs with the rect already sitting at its shifted position, paints
      // only that CURRENT (already-shifted) rect -- and the strip the rect used to occupy, now vacated, is
      // never explicitly repainted by anything: the shrink-fix above intentionally narrows OUR OWN
      // rebuild-area to the rect/animation's CURRENT bounds (so it cannot fight with element_text's own
      // area), so the vacated strip falls outside both this tick's capture and every later one, and the OLD
      // rect/border/fill ink painted there the previous frame is left behind for good -- confirmed live as a
      // persistent smear of the fill colour (plus a stray border line) sitting behind/under the caption from
      // the second real value change onward, distinct from the text-ghosting defect fixed below.
      //
      // Fix: the same real-backdrop-cache technique as the (now superseded) caption-only fix below, but sized
      // to the WIDGET'S FULL natural box (rect + gap + caption -- savedx/savedy/savedwidth/savedheight above,
      // captured BEFORE the temporary shrink further up in this function) instead of just the caption's own
      // box. Captured once, the very first time this widget is ever drawn -- before rect, animation or
      // caption have painted anything, so it is guaranteed the true backdrop -- and painted back
      // (PutBitmapNoAlpha(), GEN's own restore primitive, never a synthetic fill) whenever the rect/animation
      // actually MOVE, so everything below repaints onto a clean canvas exactly at the moment the vacated
      // strip would otherwise start collecting stale ink.
      //
      // Restoring this backdrop is only safe to do on a tick where it is actually NEEDED, never unconditionally
      // on every tick this widget happens to be marked dirty (see the perpetual-redraw note below). Two,
      // INDEPENDENT conditions each mean a vacated/stale strip can exist right now and must be wiped before the
      // rect/animation/caption painting below repaints:
      //
      //   (a) element_progressrect/element_animation's OWN bounds changed since the last tick we looked (the
      //       original GHOST-FILL FIX case: a shift/resize vacates the strip the rect used to occupy).
      //
      //   (b) element_text is about to actually repaint new glyph content (element_text->MustReDraw()). This
      //       second condition was MISSING from the original GHOST-FILL FIX and is the root cause of a
      //       regression reported live by a user: CAPTION GHOSTING RETURNED from the second real value change
      //       onward, even though the rect never moves for a same-digit-count change (e.g. "15%" -> "16%" for
      //       allocationtext="down"/"up"/"left"/"right" -- only the fill AMOUNT changes, drawn from
      //       element_progressrect's own, unchanged, stored bounds; see widthpercent/heightpercent below).
      //       Condition (a) alone therefore NEVER re-fires after the very first capture for an ordinary value
      //       change, which was the (incorrect) assumption behind the "GHOSTING FIX" comment a few dozen lines
      //       below this one ("it is now handled once, up front, by the GHOST-FILL FIX's whole-widget
      //       restore") -- it is not, because that whole-widget restore was gated ONLY on (a). Proven live via
      //       direct GetBitmap() buffer reads (not a screen capture): three consecutive real value changes on
      //       "progressbar3" (15% -> 16% -> 17% -> 18%) render cleanly on the FIRST change, then show a solid
      //       black blob merging the old and new second digit from the SECOND change onward -- exactly the
      //       "letters overlap" defect this whole investigation started from, because UI_SKINCANVAS::Draw_Text()
      //       never clears its own background, it only ever blends new glyph ink on top of whatever is already
      //       there (see the GHOSTING FIX comment below), and nothing else was clearing the caption's zone for
      //       an ordinary value-only change. Fix: also restore on condition (b), reusing the SAME already-
      //       captured widget backdrop (its box already spans rect + gap + caption, see WIDGETEDGE below) --
      //       no new capture, no synthetic fill color, just GEN's own real PutBitmapNoAlpha() restore primitive
      //       applied more often.
      //
      // element_progressrect and (independently) element_text can both stay marked dirty for many consecutive
      // ticks after a single value change (confirmed live: dozens to hundreds of extra redraw ticks, with the
      // rect's own resolved bounds and the text's own resolved string identical to the previous tick's the
      // whole time) -- an unrelated, pre-existing perpetual-redraw condition on this widget that remains out of
      // scope for this fix. Restoring the backdrop on every one of those extra ticks is still safe with
      // condition (b) added: the restore is immediately followed, in every case, by this same function's own
      // rect/animation repaint and then element_text's own repaint further down, so an extra restore+repaint
      // cycle on a tick where nothing actually changed just reproduces the same pixels a second time -- visibly
      // idempotent, unlike a bare/unconditional restore with nothing scheduled to redraw after it (which is
      // what caused the caption to vanish outright in an earlier, naive attempt at this same fix).
      if(shrunkarea)
        {
          double rx = element_progressrect ? element_progressrect->GetXPosition() : 0.0;
          double ry = element_progressrect ? element_progressrect->GetYPosition() : 0.0;
          double rw = element_progressrect ? element_progressrect->GetBoundaryLine()->width  : 0.0;
          double rh = element_progressrect ? element_progressrect->GetBoundaryLine()->height : 0.0;
          double ax = element_animation ? element_animation->GetXPosition() : 0.0;
          double ay = element_animation ? element_animation->GetYPosition() : 0.0;
          double aw = element_animation ? element_animation->GetBoundaryLine()->width  : 0.0;
          double ah = element_animation ? element_animation->GetBoundaryLine()->height : 0.0;

          GRP2DREBUILDAREA* widgetbackdrop = ProgressBackdrop_Find(element);

          if(!widgetbackdrop)
            {
              // First time this widget is ever drawn: nothing has painted rect/animation/caption ink yet, so
              // this is the one guaranteed-pristine moment to capture the true backdrop.
              const double WIDGETEDGE = 5.0;

              double wb_left = savedx - WIDGETEDGE;
              double wb_top  = (savedy - savedheight) - WIDGETEDGE;
              double wb_w    = savedwidth  + (WIDGETEDGE * 2.0);
              double wb_h    = savedheight + (WIDGETEDGE * 2.0);

              if(wb_left < 0) wb_left = 0;
              if(wb_top  < 0) wb_top  = 0;

              ProgressBackdrop_Capture(element, wb_left, wb_top, wb_w, wb_h);
              ProgressBounds_Remember(element, rx, ry, rw, rh, ax, ay, aw, ah);
            }
           else if(ProgressBounds_HasChanged(element, rx, ry, rw, rh, ax, ay, aw, ah) ||
                   (element_text && element_text->MustReDraw()))
            {
              // Either the rect/animation moved (condition (a): a vacated strip of stale ink can appear), or
              // the caption is about to repaint new text (condition (b): its old glyph ink would otherwise
              // never be cleared -- see the long comment above). Either way, restore the true backdrop now,
              // then let the rect/animation/caption painting below repaint the whole box fresh.
              PutBitmapNoAlpha(widgetbackdrop->GetXPos(), widgetbackdrop->GetYPos(), widgetbackdrop->GetBitmap());
              ProgressBounds_Remember(element, rx, ry, rw, rh, ax, ay, aw, ah);
            }
          // else: neither the geometry nor the caption text is about to change -- leave the canvas alone here;
          // the normal per-element redraw just below (and element_text's own redraw further down) is already
          // correct and idempotent for this case, exactly as it was before this whole GHOST-FILL FIX existed.
        }

      if(element_progressrect)
        {
          GRP2DCOLOR_RGBA8  linecolor(element_progressbar->GetLineColor()->GetRed(), element_progressbar->GetLineColor()->GetGreen(), element_progressbar->GetLineColor()->GetBlue(), element_progressbar->GetLineColor()->GetAlpha());
          GRP2DCOLOR_RGBA8  color    (element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());      
          GRP2DCOLOR_RGBA8  bkgcolor (element->GetBackgroundColor()->GetRed(), element->GetBackgroundColor()->GetGreen(), element->GetBackgroundColor()->GetBlue(), element->GetBackgroundColor()->GetAlpha());
          
          canvas->SetLineColor(&linecolor);
          canvas->SetFillColor(&bkgcolor);

          canvas->SetLineWidth(1.0);                                                  // thin, deterministic border (do not inherit another element's line width)

          double roundradius = element->GetRoundRect();                               // existing rounded-corner radius

          if(element_progressbar->GetRoundCap())                                      // capsule ends: radius = half the bar thickness
            {
              if(element_progressbar->GetDirection() == UI_ELEMENT_TYPE_DIRECTION_VERTICAL)
                   roundradius = element_progressrect->GetBoundaryLine()->width  / 2.0;
              else roundradius = element_progressrect->GetBoundaryLine()->height / 2.0;
            }

          // Box-shadow, read from the PROGRESSBAR element (element/element_progressbar) but drawn behind the
          // TRACK rect (element_progressrect) -- that sub-element is what UI_SkinCanvas_ProgressBar_DrawRect
          // below actually paints, so the shadow has to match its rect and its radius (including the roundcap
          // override just above) to line up. This is deliberately NOT a call to the generic
          // DrawElementBoxShadow helper: that helper anchors on the ELEMENT's own boundary and
          // supports per-corner radii, neither of which is what gets drawn here -- the visible track is a
          // different rect (a sub-element) with a single uniform radius (roundradius above), never per-corner.
          // Reuses UI_SkinCanvas_ProgressBar_DrawRect itself for the hard-shadow (no blur) fallback so the
          // shadow's capsule-clamping (short bars where the frame is narrower than 2*radius) is pixel-identical
          // to the track's own, instead of a second hand-written copy of that edge case.
          if(element->IsBoxShadowSet())
            {
              double sh_x    = element->GetShadowOffsetX();
              double sh_y    = element->GetShadowOffsetY();
              double sh_blur = element->GetShadowBlur();

              double rect_minx = element_progressrect->GetXPosition();
              double rect_maxx = element_progressrect->GetXPosition() + element_progressrect->GetBoundaryLine()->width;
              double rect_miny = element_progressrect->GetTopY();
              double rect_maxy = element_progressrect->GetYPosition();

              double sh_minx = rect_minx + sh_x;
              double sh_miny = rect_miny + sh_y;
              double sh_maxx = rect_maxx + sh_x;
              double sh_maxy = rect_maxy + sh_y;

              bool soft_ok = false;
              if(sh_blur > 0.0)
                {
                  soft_ok = UI_SkinCanvas_DrawSoftShadow(canvas, sh_minx, sh_miny, sh_maxx, sh_maxy, roundradius, roundradius, roundradius, roundradius, element->GetShadowColor(), (int)sh_blur);
                }

              if(!soft_ok)
                {
                  GRP2DCOLOR_RGBA8  shadow_col(element->GetShadowColor()->GetRed(), element->GetShadowColor()->GetGreen(), element->GetShadowColor()->GetBlue(), element->GetShadowColor()->GetAlpha());
                  GRP2DCOLOR_RGBA8  shadow_line_none(0, 0, 0, 0);

                  canvas->SetFillColor(&shadow_col);
                  canvas->SetLineColor(&shadow_line_none);
                  canvas->SetLineWidth(1.0f);

                  UI_SkinCanvas_ProgressBar_DrawRect(canvas, sh_minx, sh_maxy, sh_maxx, sh_miny, roundradius);
                }

              // Restore the track's own colours/width: both branches above may have changed canvas state
              // (the hard-shadow fallback always does; the soft path never touches it, but restoring
              // unconditionally is one cheap, always-correct line instead of two conditional ones).
              canvas->SetLineColor(&linecolor);
              canvas->SetFillColor(&bkgcolor);
              canvas->SetLineWidth(1.0);
            }

          UI_SkinCanvas_ProgressBar_DrawRect(canvas, element_progressrect->GetXPosition(), element_progressrect->GetYPosition(), element_progressrect->GetXPosition() + element_progressrect->GetBoundaryLine()->width, element_progressrect->GetTopY(), roundradius);

          double widthpercent   = element_progressrect->GetBoundaryLine()->width;
          double heightpercent  = element_progressrect->GetBoundaryLine()->height;  
                                    
          if(element_progressbar->ContinuousCycle_Is())
            {                
              XDWORD xpos_segment = 0;
              XDWORD ypos_segment = 0;
           
              switch(element_progressbar->GetDirection())
                {     
                  case UI_ELEMENT_TYPE_DIRECTION_UNKWOWN	  :
	                case UI_ELEMENT_TYPE_DIRECTION_HORIZONTAL : widthpercent   = (element_progressrect->GetBoundaryLine()->width  * element_progressbar->ContinuousCycle_GetSizeSegment()) / 100.0f;   
                                                              xpos_segment   = element_progressbar->ContinuousCycle_GetNSteps();
                                                              xpos_segment  += element_progressbar->ContinuousCycle_GetStepSegment();
                                                              element_progressbar->ContinuousCycle_SetNSteps(xpos_segment);        
                                                                                                                          
                                                              if(xpos_segment < widthpercent) 
                                                                {                                                                   
                                                                  widthpercent = xpos_segment;                                                                  
                                                                  xpos_segment = 0;                                                                                                                                                                                                                                                                                                                                           
                                                                }                                                                
                                                               else
                                                                {                                                                                                                                      
                                                                  if((xpos_segment + widthpercent) >= element_progressrect->GetBoundaryLine()->width) 
                                                                    {                                                                       
                                                                      widthpercent = (element_progressrect->GetBoundaryLine()->width - xpos_segment);                                                                       
                                                                      if(!widthpercent) 
                                                                        {
                                                                          xpos_segment = 0;
                                                                          element_progressbar->ContinuousCycle_SetNSteps(xpos_segment);                                                                          
                                                                        }

                                                                      xpos_segment -= (XDWORD)widthpercent;   
                                                                    }  
                                                                   else
                                                                    {
                                                                      xpos_segment -= (XDWORD)widthpercent;   
                                                                    }                                                                                                                                                                                                                       
                                                                }
                                                                 
                                                                                                                                                                                      
                                                              break;

	                case UI_ELEMENT_TYPE_DIRECTION_VERTICAL		: heightpercent  = (element_progressrect->GetBoundaryLine()->height * element_progressbar->ContinuousCycle_GetSizeSegment()) / 100.0f;   
                                                              ypos_segment   = element_progressbar->ContinuousCycle_GetNSteps();
                                                              ypos_segment  += element_progressbar->ContinuousCycle_GetStepSegment();
                                                              element_progressbar->ContinuousCycle_SetNSteps(ypos_segment);        
                                                              
                                                              if(ypos_segment < heightpercent) 
                                                                {                                                                   
                                                                  heightpercent = ypos_segment;                                                                  
                                                                  ypos_segment = 0;                                                                                                                                                                                                                                                                                                                                           
                                                                }                                                                
                                                               else
                                                                {                                                                                                                                      
                                                                  if((ypos_segment + heightpercent) >= element_progressrect->GetBoundaryLine()->height) 
                                                                    {                                                                       
                                                                      heightpercent = (element_progressrect->GetBoundaryLine()->height - ypos_segment);                                                                       
                                                                      if(!heightpercent) 
                                                                        {
                                                                          ypos_segment = 0;
                                                                          element_progressbar->ContinuousCycle_SetNSteps(ypos_segment);                                                                          
                                                                        }

                                                                      ypos_segment -= (XDWORD)heightpercent;   
                                                                    }  
                                                                   else
                                                                    {
                                                                      ypos_segment -= (XDWORD)heightpercent;   
                                                                    }                                                                                                                                                                                                                       
                                                                }
                                                                 
                                                              break;
                }

              canvas->SetFillColor(&color);

              UI_SkinCanvas_ProgressBar_DrawRect(canvas, element_progressrect->GetXPosition() + xpos_segment, element_progressrect->GetYPosition() + ypos_segment, element_progressrect->GetXPosition() + xpos_segment + widthpercent, element_progressrect->GetYPosition() - (ypos_segment + heightpercent), roundradius);
              
            }
           else
            {                
              switch(element_progressbar->GetDirection())
                {     
                  case UI_ELEMENT_TYPE_DIRECTION_UNKWOWN	  :
	                case UI_ELEMENT_TYPE_DIRECTION_HORIZONTAL : widthpercent   = (element_progressrect->GetBoundaryLine()->width  * element_progressbar->GetLevel()) / 100.0f;   break;
	                case UI_ELEMENT_TYPE_DIRECTION_VERTICAL		: heightpercent  = (element_progressrect->GetBoundaryLine()->height * element_progressbar->GetLevel()) / 100.0f;   break;
                }
               
              if(element_progressbar->GetGradientColor()->GetAlpha())                 // gradient fill: color -> gradientcolor (else solid color below)
                {
                  GRP2DGRADIENTSTOP stops[2];

                  stops[0].offset = 0.0;
                  stops[0].color  = color;
                  stops[1].offset = 1.0;
                  stops[1].color  = GRP2DCOLOR_RGBA8(element_progressbar->GetGradientColor()->GetRed(), element_progressbar->GetGradientColor()->GetGreen(), element_progressbar->GetGradientColor()->GetBlue(), element_progressbar->GetGradientColor()->GetAlpha());

                  double bx    = element_progressrect->GetXPosition();
                  double by    = element_progressrect->GetYPosition();
                  double bw    = element_progressrect->GetBoundaryLine()->width;
                  double bh    = element_progressrect->GetBoundaryLine()->height;
                  bool   track = (element_progressbar->GetGradientMode() == UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK);

                  double gx1, gy1, gx2, gy2;

                  if(element_progressbar->GetDirection() == UI_ELEMENT_TYPE_DIRECTION_VERTICAL)
                    {
                      gx1 = bx;  gx2 = bx;
                      gy1 = by;                                                        // bottom = start color
                      gy2 = track ? (by - bh) : (by - heightpercent);                  // track: full bar | fill: current fill top
                    }
                   else
                    {
                      gy1 = by;  gy2 = by;
                      gx1 = bx;                                                        // left = start color
                      gx2 = track ? (bx + bw) : (bx + widthpercent);                   // track: full bar | fill: current fill right
                    }

                  UI_SkinCanvas_ProgressBar_DrawGradientRect(canvas, bx, by, bx + widthpercent, by - heightpercent, roundradius, stops, gx1, gy1, gx2, gy2);
                }
               else
                {
                  canvas->SetFillColor(&color);

                  UI_SkinCanvas_ProgressBar_DrawRect(canvas, element_progressrect->GetXPosition(), element_progressrect->GetYPosition(), element_progressrect->GetXPosition() + widthpercent, element_progressrect->GetYPosition() - heightpercent, roundradius);
                }
            }

          // %-caption must repaint when the fill changes. TextBackdrop invalidate/recapture was removed from
          // Draw_Text (classic XML / UI_Options): it forced GetBitmap every tick and could paste a solid box
          // under the glyphs. Rebuild-area restore + this MustReDraw is enough.
          if(element_text)
            {
              element_text->SetMustReDraw(true);
            }

          // ROOT-CAUSE FIX (2026-09, confirmed live pixel-by-pixel against progressbar3/progressbar0/
          // progressbar4 in UI_Options' example.xml -- see PreDrawFunction() above in this same file): for
          // every allocationtext mode except NONE/CENTER, element_text's own box sits OUTSIDE
          // element_progressrect's box (CalculateBoundaryLine_ProgressBar offsets one or the other to make
          // room). PreDrawFunction()'s ancestor-walk assumes a dirty ancestor's own redraw geometrically
          // covers every descendant, so it never lets element_text own its own rebuild-area while its father
          // (this progressbar) is dirty -- which normally IS every frame here. But this progressbar's own
          // redraw (the track/fill painting above) only ever covers element_progressrect's bounds, never
          // element_text's -- so that assumption is false for exactly this child, and element_text is left
          // with no rebuild-area of its own to correctly capture/restore its own backdrop. In practice the
          // caption's zone still LOOKS fine while element_progressrect's bounds happen to overlap it (the
          // father's restore+repaint cycle papers over it by accident), but the moment a real value change
          // shifts the rect away from the caption (see CalculateBoundaryLine_ProgressBar), nothing is left to
          // restore/repaint that zone at all, and the freshly-drawn glyph blends against whatever was there
          // one frame too early in the cycle (proven via direct framebuffer reads: a transient, wrong
          // snapshot that the rect's own repaint happened to hide every prior frame) -- exactly the "letters
          // overlap / caption vanishes after the value changes" defect reported on progress widgets.
          // Fix: momentarily tell PreDrawFunction() this element is NOT dirty while element_text draws, so
          // its own ancestor-walk stops at OUR still-real dirty state and lets element_text own a normal,
          // self-restoring rebuild-area of its own -- precisely as if it were a standalone dirty element, the
          // same protection every other on-screen element already gets. Restored immediately after (Draw()
          // clears element_text's OWN flag via its own PostDrawFunction(), never touches ours), so
          // element_animation right below and our own PostDrawFunction() call at the end of this function see
          // this element's real dirty state, unchanged.
          if(element_text)
            {
              // GHOSTING FIX (2026-09, follow-up to the ROOT-CAUSE FIX above; the actual ghosting protection now
              // lives in the GHOST-FILL FIX at the top of this function -- see that comment's condition (b),
              // added as a correction to a regression this same toggle used to (incompletely) guard against):
              // the toggle below gives element_text its own one-shot rebuild-area, which correctly protects its
              // FIRST-ever draw -- but that area is captured fresh and thrown away every time (see
              // RebuildAllAreas()'s orphan-discard path: an area whose element has already settled clean is
              // dropped WITHOUT ever being restored, because normally "nothing to undo" is exactly right). For
              // an ordinary element that is correct: some ancestor's own redraw already re-covers its zone from
              // scratch every time it repaints. A progress-bar caption has no such ancestor -- our own painting
              // above only ever touches element_progressrect/element_animation, never the caption strip -- and
              // Draw_Text() itself (shared by every text element) never erases anything either, it only ever
              // blends new glyph ink on top of whatever is already there (canvas->VectorFont_Print(), no clear
              // step). This toggle ALONE cannot prevent that: it only changes which rebuild-area element_text
              // gets, never what is already on the canvas the moment Draw_Text() paints onto it. The actual
              // clearing has to happen BEFORE this point in the tick, and is now condition (b) of the GHOST-FILL
              // FIX's restore above (element_text->MustReDraw()) -- confirmed live via GetBitmap() buffer reads
              // to fully resolve the "letters overlap" defect across multiple consecutive value changes, not
              // just the first one.

              // Momentarily hide OUR OWN dirty state from PreDrawFunction()'s ancestor-walk (see the ROOT-CAUSE
              // FIX comment above) so element_text's own PreDrawFunction() call, made from inside Draw() below,
              // is free to give it an independent rebuild-area instead of being suppressed as "a descendant of
              // an already-dirty ancestor". This does NOT affect OUR OWN rebuild-area: that one was already
              // created by the PreDrawFunction(element, ...) call at the top of this function, before this
              // toggle ever runs. Restored immediately after so element_animation below and our own
              // PostDrawFunction() call at the end of this function see our real dirty state, unchanged.
              element->SetMustReDraw(false);
              Draw(element_text);
              element->SetMustReDraw(true);
            }

        }

      if(element_animation) Draw(element_animation);
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);
    
  if(element_progressbar->ContinuousCycle_Is()) 
    {
      if(element_progressbar->ContinuousCycle_GetXTimer()->GetMeasureMilliSeconds() > element_progressbar->ContinuousCycle_GetTimeStep())
        {
          element->SetMustReDraw(true);

          element_progressbar->ContinuousCycle_GetXTimer()->Reset();
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_ProgressRadial(UI_ELEMENT* element)
* @brief      Draw radial progress (track ring + gradient value arc + centered caption)
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_ProgressRadial(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_PROGRESS_RADIAL* element_progress = (UI_ELEMENT_PROGRESS_RADIAL*)element;
  UI_ELEMENT_TEXT*            element_text     = (UI_ELEMENT_TEXT*)element_progress->Get_UIText();
  GRP2DCANVAS*                canvas           = GetCanvas();
  double                      x_position       = 0.0f;
  double                      y_position       = 0.0f;
  XRECT                       clip_rect;

  if(!canvas) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position, 1);

  if(element->MustReDraw())
    {
      // RADIAL CAPTION GHOSTING FIX (2026-09): this widget's track ring, value arc, round caps and centered
      // caption are ALL painted by alpha-blending onto whatever is already on the canvas -- the ring/arc never
      // fill the whole box (only the ring geometry itself), and the caption is anti-aliased glyph ink, painted
      // via VectorFont blending, never a solid fill. Nothing here ever clears the box first.
      //
      // That is normally safe because the generic per-tick rebuild-area system (see GetRebuildAreaByElement()
      // and UI_SKINCANVAS_REBUILDAREAS::RebuildAllAreas()) is expected to restore the true backdrop before each
      // real repaint. But a progressradial is typically dirty for exactly ONE frame per value change and idle
      // in between, so its rebuild area is ORPHAN-DISCARDED (deleted WITHOUT restoring -- see RebuildAllAreas()'s
      // orphan-discard comment: "the element's current on-screen content is already correct", true only for a
      // widget that clears its own background) the very next frame. The NEXT real value change then creates a
      // brand-new area that captures whatever is CURRENTLY on screen -- already showing the PREVIOUS caption's
      // ink -- and blends the new caption straight on top of it. Confirmed live via canvas->GetBitmap() reads
      // (alpha/RGB of the caption area) and a temporary trace of PreDrawFunction/RebuildAllAreas: progressbar1's
      // area is created fresh on every real tick and is ALWAYS orphan-discarded (never restored) afterwards.
      //
      // Fix: cache the TRUE backdrop (the real pixels behind this whole widget) once, the very first time it is
      // ever drawn -- before any track/arc/cap/caption ink exists -- and restore it (PutBitmapNoAlpha(), never a
      // synthetic fill colour) immediately before repainting on every later real redraw, exactly mirroring the
      // already-verified ProgressBackdrop (linear progress bar) and FormBackdrop (form/menu) fixes above. Unlike
      // the linear bar's EXTERNAL caption, the radial caption is centered INSIDE the widget's own box, so one
      // capture covering the widget's own rebuild-area geometry is enough for ring+arc+caption together.
      GRP2DREBUILDAREA* ownarea = GetRebuildAreaByElement(element);

      if(ownarea)
        {
          GRP2DREBUILDAREA* radialbackdrop = RadialBackdrop_Find(element);

          // Capture/restore against the widget's visible AABB, not the glow-padded ownarea: annular box-shadow
          // pads (blur*2) bleed into neighbouring cards, and restoring that padded box blanks their ink when
          // those cards have no rebuild-area this tick. Glow is still covered by PreDraw's rebuild-area.
          double rdx = x_position;
          double rdy = UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height);
          double rdw = element->GetBoundaryLine()->width;
          double rdh = element->GetBoundaryLine()->height;

          // Drop a stale padded capture (pre-AABB change) so the next branch re-captures at the visible size.
          if(radialbackdrop && radialbackdrop->GetBitmap() &&
             (((double)radialbackdrop->GetBitmap()->GetWidth()  != rdw) ||
              ((double)radialbackdrop->GetBitmap()->GetHeight() != rdh) ||
              (radialbackdrop->GetXPos() != rdx) ||
              (radialbackdrop->GetYPos() != rdy)))
            {
              PutBitmapNoAlpha(radialbackdrop->GetXPos(), radialbackdrop->GetYPos(), radialbackdrop->GetBitmap());
              for(XDWORD c=0; c<radialbackdrops.GetSize(); c++)
                {
                  GRP2DREBUILDAREA* entry = radialbackdrops.Get(c);
                  if(entry && (entry->GetExtraData() == (void*)element))
                    {
                      radialbackdrops.Delete(entry);
                      GEN_DELETE entry;
                      break;
                    }
                }
              radialbackdrop = NULL;
            }

          if(!radialbackdrop)
            {
              // First time this widget is ever drawn: nothing has painted track/arc/caption ink here yet, so
              // this is the one guaranteed-pristine moment to capture the true backdrop.
              RadialBackdrop_Capture(element, rdx, rdy, rdw, rdh);
            }
           else
            {
              // Not the first draw: restore the true backdrop now, before repainting below.
              PutBitmapNoAlpha(radialbackdrop->GetXPos(), radialbackdrop->GetYPos(), radialbackdrop->GetBitmap());

              // Restoring just wiped the previous caption's glyph ink. Draw(element_text) below only actually
              // repaints when the caption's OWN MustReDraw() is set -- force it here so the caption always
              // repaints on top of the freshly restored backdrop instead of vanishing (same reasoning as the
              // force-children-dirty step in the Draw_Form() ALPHA-DARKENING FIX above).
              if(element_text) element_text->SetMustReDraw(true);

              // Glow still paints into neighbour AABBs after this restore; dirty overlapping layout siblings
              // using the padded ownarea so they redraw on top of any bleed this frame.
              UI_SkinCanvas_DirtyOverlappingLayoutSiblings(element, ownarea->GetXPos(), ownarea->GetYPos(), (double)ownarea->GetBitmap()->GetWidth(), (double)ownarea->GetBitmap()->GetHeight());
            }
        }

      double width  = element->GetBoundaryLine()->width;
      double height = element->GetBoundaryLine()->height;

      double cx     = x_position + (width  / 2.0);
      double cy     = y_position - (height / 2.0);                       // y_position is the BOTTOM edge

      double outer  = __MIN(width, height) / 2.0;

      double thick  = element_progress->GetThickness();
      if(thick <= 0.0)    thick = outer * UI_ELEMENT_PROGRESS_RADIAL_AUTOTHICKNESS_FACTOR;
      if(thick > outer)   thick = outer;

      double r      = outer - (thick / 2.0) - 1.0;                       // radius to the ring centerline
      if(r < 1.0) r = 1.0;

      // Soft glow / box-shadow (CSS Lite): annular halo around the ring -- NOT DrawElementBoxShadow()'s filled
      // AABB/disc (that washed the hollow centre and TextBackdrop froze it as a hard blue block behind "%").
      // Only runs when the element authored box-shadow; XML-only layouts without it are untouched.
      if(element->IsBoxShadowSet())
        {
          double sh_blur = element->GetShadowBlur();
          double glow_cx = cx + element->GetShadowOffsetX();
          double glow_cy = cy + element->GetShadowOffsetY();
          double band_outer = r + (thick / 2.0) + 2.0;
          double band_inner = r - (thick / 2.0) - 2.0;
          if(band_inner < 0.0) band_inner = 0.0;
          if(band_outer > outer) band_outer = outer;

          bool soft_ok = false;
          if(sh_blur > 0.0)
            {
              soft_ok = UI_SkinCanvas_DrawSoftRingGlow(canvas, glow_cx, glow_cy, band_outer, band_inner, element->GetShadowColor(), (int)sh_blur);
            }

          if(!soft_ok)
            {
              // Hard fallback: stroke the ring band only -- never fill the disc (would recreate the blue box).
              GRP2DCOLOR_RGBA8 glow_col(element->GetShadowColor()->GetRed(), element->GetShadowColor()->GetGreen(), element->GetShadowColor()->GetBlue(), element->GetShadowColor()->GetAlpha());
              canvas->SetLineWidth(thick);
              canvas->SetLineColor(&glow_col);
              canvas->Circle(glow_cx, glow_cy, r, false);
            }
        }

      double start  = element_progress->GetStartAngle();
      double sweep  = element_progress->GetSweepAngle();

      // ---- 1) track ring (full sweep, flat background color) ---------------------------------------------------------
      if(element->GetBackgroundColor()->GetAlpha())
        {
          GRP2DCOLOR_RGBA8 trackcolor(element->GetBackgroundColor()->GetRed(), element->GetBackgroundColor()->GetGreen(), element->GetBackgroundColor()->GetBlue(), element->GetBackgroundColor()->GetAlpha());

          canvas->SetLineWidth(thick);
          canvas->SetLineColor(&trackcolor);

          if(fabs(sweep) >= 359.999)
            {
              canvas->Circle(cx, cy, r, false);                                       // full ring: closed ellipse outline (no polyline seam or end caps)
            }
           else
            {
              GRP2DPATH trackpath;
              bool      firstpoint = true;

              UI_SkinCanvas_ProgressRadial_AppendArc(trackpath, cx, cy, r, start, sweep, firstpoint);
              canvas->Path(trackpath, false);                                         // partial sweep: open arc
            }
        }

      // ---- 2) value arc (gradient color -> linecolor, scaled by level) -----------------------------------------------
      double valuesweep = sweep * (element_progress->GetLevel() / 100.0);

      if(fabs(valuesweep) > 0.01)
        {
          GRP2DPATH valuepath;
          bool      firstpoint = true;

          UI_SkinCanvas_ProgressRadial_AppendArc(valuepath, cx, cy, r, start, valuesweep, firstpoint);

          if(element_progress->GetGradientColor()->GetAlpha())             // ANGULAR gradient along the arc: color -> gradientcolor
            {
              UI_COLOR* c0    = element->GetColor();
              UI_COLOR* c1    = element_progress->GetGradientColor();
              bool      track = (element_progress->GetGradientMode() == UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK);

              int    nseg = (int)(fabs(valuesweep) / 3.0);                  // ~3 degrees per segment
              if(nseg < 1) nseg = 1;

              double segsweep = valuesweep / (double)nseg;

              canvas->SetLineWidth(thick);

              for(int i = 0; i < nseg; i++)
                {
                  double a0 = start + (segsweep * (double)i);
                  double t  = track ? ((segsweep * ((double)i + 0.5)) / sweep)        // along the FULL sweep: strong only at 100%
                                    : (((double)i + 0.5) / (double)nseg);             // along the value arc: strong at the leading edge

                  if(t < 0.0) t = 0.0;
                  if(t > 1.0) t = 1.0;

                  int rr = (int)c0->GetRed()   + (int)(((int)c1->GetRed()   - (int)c0->GetRed())   * t);
                  int gg = (int)c0->GetGreen() + (int)(((int)c1->GetGreen() - (int)c0->GetGreen()) * t);
                  int bb = (int)c0->GetBlue()  + (int)(((int)c1->GetBlue()  - (int)c0->GetBlue())  * t);
                  int aa = (int)c0->GetAlpha() + (int)(((int)c1->GetAlpha() - (int)c0->GetAlpha()) * t);

                  GRP2DCOLOR_RGBA8 segcolor(rr, gg, bb, aa);
                  canvas->SetLineColor(&segcolor);

                  double s = segsweep;
                  if(i < (nseg - 1)) s += (segsweep * 0.6);                            // overlap forward so the round joins hide the seams (last segment not extended)

                  GRP2DPATH segpath;
                  bool      segfirst = true;

                  UI_SkinCanvas_ProgressRadial_AppendArc(segpath, cx, cy, r, a0, s, segfirst);
                  canvas->Path(segpath, false);
                }
            }
           else                                                          // flat stroke with the element color
            {
              GRP2DCOLOR_RGBA8 arccolor(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());

              canvas->SetLineWidth(thick);
              canvas->SetLineColor(&arccolor);
              canvas->Path(valuepath, false);
            }
        }

      // ---- 2b) rounded caps (optional): filled circles sampling the gradient at each endpoint ------------------------
      if(element_progress->GetRoundCap() && (fabs(valuesweep) > 0.01))
        {
          double cap_r   = thick / 2.0;
          double a_start = start                 * (PI / 180.0);
          double a_end   = (start + valuesweep)  * (PI / 180.0);

          double sx = cx + (r * cos(a_start));
          double sy = cy + (r * sin(a_start));
          double ex = cx + (r * cos(a_end));
          double ey = cy + (r * sin(a_end));

          GRP2DCOLOR_RGBA8 capstart;
          GRP2DCOLOR_RGBA8 capend;

          if(element_progress->GetGradientColor()->GetAlpha())              // caps match the ANGULAR arc color at each end
            {
              UI_COLOR* c0    = element->GetColor();
              UI_COLOR* c1    = element_progress->GetGradientColor();
              bool      track = (element_progress->GetGradientMode() == UI_ELEMENT_PROGRESS_GRADIENTMODE_TRACK);
              double    tend  = track ? (element_progress->GetLevel() / 100.0) : 1.0;

              if(tend < 0.0) tend = 0.0;
              if(tend > 1.0) tend = 1.0;

              capstart = GRP2DCOLOR_RGBA8(c0->GetRed(), c0->GetGreen(), c0->GetBlue(), c0->GetAlpha());   // start cap = gradient at t 0

              int rr = (int)c0->GetRed()   + (int)(((int)c1->GetRed()   - (int)c0->GetRed())   * tend);
              int gg = (int)c0->GetGreen() + (int)(((int)c1->GetGreen() - (int)c0->GetGreen()) * tend);
              int bb = (int)c0->GetBlue()  + (int)(((int)c1->GetBlue()  - (int)c0->GetBlue())  * tend);
              int aa = (int)c0->GetAlpha() + (int)(((int)c1->GetAlpha() - (int)c0->GetAlpha()) * tend);

              capend = GRP2DCOLOR_RGBA8(rr, gg, bb, aa);                                                  // end cap = gradient at the leading-edge t
            }
           else                                                          // flat color
            {
              capstart = GRP2DCOLOR_RGBA8(element->GetColor()->GetRed(), element->GetColor()->GetGreen(), element->GetColor()->GetBlue(), element->GetColor()->GetAlpha());
              capend   = capstart;
            }

          canvas->SetLineWidth(0.0);                                       // fill only, no outline

          canvas->SetFillColor(&capstart);
          canvas->Circle(sx, sy, cap_r, true);

          canvas->SetFillColor(&capend);
          canvas->Circle(ex, ey, cap_r, true);
        }

      // ---- 3) centered caption (e.g. "37%") --------------------------------------------------------------------------
      // TextBackdrop invalidate/recapture removed (see Draw_Text): rebuild area + MustReDraw is enough.
      if(element_text)
        {
          element_text->SetMustReDraw(true);
          Draw(element_text);
        }

      canvas->SetLineWidth(1.0);                                                       // reset shared line width so it doesn't leak to the next element
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Draw_ProgressImage(UI_ELEMENT* element)
* @brief      Draw progress image: empty (0%) graphic first, then full (100%) clipped to the level by direction.
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Draw_ProgressImage(UI_ELEMENT* element)
{
  if(!element) return false;

  UI_ELEMENT_PROGRESS_IMAGE* element_progressimage = (UI_ELEMENT_PROGRESS_IMAGE*)element;
  UI_ELEMENT_TEXT*           element_text          = (UI_ELEMENT_TEXT*)element_progressimage->Get_UIText();
  GRP2DCANVAS*               canvas                = GetCanvas();
  double                     x_position            = 0.0f;
  double                     y_position            = 0.0f;
  XRECT                      clip_rect;

  GRPBITMAP*                 imgempty              = element_progressimage->GetImageEmpty();
  GRPBITMAP*                 imgfull               = element_progressimage->GetImageFull();

  if(!canvas) return false;
  if(!imgempty && !imgfull) return false;

  PreDrawFunction(element, canvas, clip_rect, x_position, y_position);

  if(element->MustReDraw())
    {
      GRPBITMAP* ref    = imgfull ? imgfull : imgempty;                  // both graphics share size
      double     imgw   = (double)ref->GetWidth();
      double     imgh   = (double)ref->GetHeight();
      double     alpha  = (double)element_progressimage->GetAlpha();

      // caption-aware image offset (mirrors the boundary computed in CalculateBoundaryLine_ProgressImage; (0,0) for none/right/up/center)
      int        allocation = (int)element_progressimage->GetAllocationTextType();
      double     textw      = element_text ? element_text->GetBoundaryLine()->width  : 0.0;
      double     texth      = element_text ? element_text->GetBoundaryLine()->height : 0.0;
      double     bw, bh, ox, oy, tx, ty;
      UI_SkinCanvas_ProgressImage_Layout(allocation, imgw, imgh, textw, texth, 6.0, bw, bh, ox, oy, tx, ty);
      (void)bw; (void)bh; (void)tx; (void)ty;                                       // Draw only needs the image offset (ox,oy)

      double     left   = x_position + ox;
      double     right  = left + imgw;
      double     top    = (y_position - element->GetBoundaryLine()->height) + oy;   // boundary top (y_position is BOTTOM) + image offset
      double     bottom = top + imgh;

      // ---- 1) empty graphic (0%) drawn fully -----------------------------------------------------------------------
      if(imgempty) canvas->PutBitmapAlpha(left, top, imgempty, alpha);

      // ---- 2) full graphic (100%) revealed up to level, clipped by direction ---------------------------------------
      if(imgfull && (element_progressimage->GetLevel() > 0.0f))
        {
          double level = (double)element_progressimage->GetLevel();
          if(level > 100.0) level = 100.0;

          double r_left   = left;
          double r_right  = right;
          double r_top    = top;
          double r_bottom = bottom;

          double offsetstart = element_progressimage->GetOffsetStart();
          double offsetend   = element_progressimage->GetOffsetEnd();

          if(element_progressimage->GetDirection() == UI_ELEMENT_TYPE_DIRECTION_VERTICAL)
            {
              double activelen  = imgh - offsetstart - offsetend;       // progression band along the height
              if(activelen < 0.0) activelen = 0.0;

              double bandbottom = bottom - offsetstart;                 // progression starts offsetstart up from the bottom
              r_bottom = bandbottom;                                    // revealed region bottom edge
              r_top    = bandbottom - (activelen * (level / 100.0));    // grows upward with level
            }
           else
            {
              double activelen = imgw - offsetstart - offsetend;        // progression band along the width
              if(activelen < 0.0) activelen = 0.0;

              double bandleft  = left + offsetstart;                    // progression starts offsetstart from the left
              r_left  = bandleft;                                       // revealed region left edge
              r_right = bandleft + (activelen * (level / 100.0));       // grows rightward with level
            }

          // intersect the reveal rect with the current clip, then draw only that part of the full graphic
          double ox1 = 0.0, oy1 = 0.0, ox2 = 0.0, oy2 = 0.0;
          canvas->GetClipBox(ox1, oy1, ox2, oy2);

          double cminx = __MIN(ox1, ox2), cmaxx = __MAX(ox1, ox2);
          double cminy = __MIN(oy1, oy2), cmaxy = __MAX(oy1, oy2);

          double rminx = __MIN(r_left, r_right),  rmaxx = __MAX(r_left, r_right);
          double rminy = __MIN(r_top, r_bottom), rmaxy = __MAX(r_top, r_bottom);

          double iminx = __MAX(cminx, rminx), imaxx = __MIN(cmaxx, rmaxx);
          double iminy = __MAX(cminy, rminy), imaxy = __MIN(cmaxy, rmaxy);

          if((imaxx > iminx) && (imaxy > iminy))
            {
              canvas->SetClipBox(iminx, imaxy, imaxx, iminy);            // (left, bottom, right, top)
              canvas->PutBitmapAlpha(left, top, imgfull, alpha);
              canvas->SetClipBox(ox1, oy1, ox2, oy2);                    // restore previous clip
            }
        }

      // ---- 3) optional caption -------------------------------------------------------------------------------------
      if(element_text) Draw(element_text);
    }

  PostDrawFunction(element, canvas, clip_rect, x_position, y_position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::GetFontSize(XCHAR* text, XDWORD& width, XDWORD& height)
* @brief      Get font size
* @ingroup    USERINTERFACE
* 
* @param[in]  text : Text to use.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::GetFontSize(XCHAR* text, XDWORD& width, XDWORD& height)
{
  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;

  width   = (XDWORD)canvas->VectorFont_GetWidth(text);
  height  = (XDWORD)canvas->VectorFont_GetHeight(text);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::SetFontSize(XDWORD size)
* @brief      Set font size
* @ingroup    USERINTERFACE
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::SetFontSize(XDWORD size)
{
  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;

  canvas->Vectorfont_GetConfig()->SetSize(size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::DrawBackgroundColor(UI_ELEMENT* element, GRP2DCANVAS* canvas, double x_position, double y_position)
* @brief      Draw background color
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  canvas : Canvas object to use.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::DrawBackgroundColor(UI_ELEMENT* element, GRP2DCANVAS* canvas, double x_position, double y_position)
{
  if(!element->GetBackgroundColor()->GetAlpha()) return false;

  GRP2DCOLOR_RGBA8  bkgcolor(element->GetBackgroundColor()->GetRed(), element->GetBackgroundColor()->GetGreen(), element->GetBackgroundColor()->GetBlue(), element->GetBackgroundColor()->GetAlpha());


  canvas->SetLineWidth(0.0f);
  canvas->SetLineColor(&bkgcolor);
  canvas->SetFillColor(&bkgcolor);

  canvas->Rectangle(x_position, y_position, x_position + element->GetBoundaryLine()->width, UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height), true);      

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ContentExtent(UI_ELEMENT* element, bool vx, bool vy, double margin, double& minx, double& maxx, double& miny, double& maxy)
* @brief      Pre draw function
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  vx : Vx value.
* @param[in]  vy : Vy value.
* @param[in]  margin : Margin value.
* @param[in]  minx : Minx value.
* @param[in]  maxx : Maxx value.
* @param[in]  miny : Miny value.
* @param[in]  maxy : Maxy value.
* 
* --------------------------------------------------------------------------------------------------------------------*/

// Accumulate the screen bounding box of an element's descendants, but only on the axes flagged (vx/vy). Used to size a
// scrollable container's rebuild area so it covers content that overflows on a non-clipped (VISIBLE) axis. Descendants
// no longer own rebuild areas, so without this their overflowing pixels (e.g. an animated icon sticking out of the bar)
// would never be erased and would pile up. Positions are absolute (scroll is applied at draw time, not stored), and a
// VISIBLE axis is never scrolled, so the stored positions are the right ones to measure on that axis.
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_ContentExtent(UI_ELEMENT* element, bool vx, bool vy, double margin, double& minx, double& maxx, double& miny, double& maxy)
* @brief      Skin canvas content extent
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Pointer to element.
* @param[in]  vx : Vx value.
* @param[in]  vy : Vy value.
* @param[in]  margin : Margin value.
* @param[in]  minx : Minx value.
* @param[in]  maxx : Maxx value.
* @param[in]  miny : Miny value.
* @param[in]  maxy : Maxy value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_ContentExtent(UI_ELEMENT* element, bool vx, bool vy, double margin, double& minx, double& maxx, double& miny, double& maxy)
{
  if(!element) return;

  XVECTOR<UI_ELEMENT*>* childs = element->GetComposeElements();
  if(!childs) return;

  for(XDWORD c=0; c<childs->GetSize(); c++)
    {
      UI_ELEMENT* child = childs->Get(c);
      if(!child)                   continue;
      if(!child->IsVisible())      continue;
      if(!child->GetBoundaryLine()) continue;

      double cx = child->GetXPosition();
      double cy = child->GetYPosition();
      double cw = child->GetBoundaryLine()->width;
      double ch = child->GetBoundaryLine()->height;

      if(vx) { minx = __MIN(minx, UI_BOUNDARYLINE_EdgeLeft(cx, cw) - margin); maxx = __MAX(maxx, UI_BOUNDARYLINE_EdgeRight (cx, cw) + margin); }
      if(vy) { miny = __MIN(miny, UI_BOUNDARYLINE_EdgeTop (cy, ch) - margin); maxy = __MAX(maxy, UI_BOUNDARYLINE_EdgeBottom(cy, ch) + margin); }

      // Recurse to reach deeper content (e.g. an animated sprite inside a MultiOption). A nested clipping scrollable
      // bounds its content only on the axes IT clips; on a VISIBLE axis its content still overflows, so keep measuring
      // that axis and drop the ones it clips.
      bool child_vx = vx;
      bool child_vy = vy;
      UI_PROPERTY_SCROLLEABLE* csc = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(child);
      if(csc)
        {
          if(csc->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) != UI_OVERFLOW_VISIBLE) child_vx = false;
          if(csc->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL)   != UI_OVERFLOW_VISIBLE) child_vy = false;
        }

      if(child_vx || child_vy) UI_SkinCanvas_ContentExtent(child, child_vx, child_vy, margin, minx, maxx, miny, maxy);
    }
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::PreDrawFunction(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double& x_position, double& y_position, XDWORD edge)
* @brief      Pre Draw Function
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  canvas : Canvas object to use.
* @param[in]  clip_rect : Clip rect value.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* @param[in]  edge : Edge value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::PreDrawFunction(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double& x_position, double& y_position, XDWORD edge)
{
  if(!canvas) return false;

  x_position = element->GetXPosition();
  y_position = element->GetYPosition();

  if(element->GetFather()) GetAddPositionScrollSteps(element->GetFather(), x_position, y_position);

  element->SetXPositionWithScroll(x_position);
  element->SetYPositionWithScroll(y_position);
 
  bool createarea = false;
  if(element->MustReDraw())
    {
      // Modal offscreen rebuild must not register shared-canvas rebuild areas (wrong target / peel noise).
      createarea = !GEN_USERINTERFACE.ModalLayer_IsCompositing();
      if(GetRebuildAreaByElement(element)) createarea = false;

      // A descendant of an element that is already going to be redrawn must NOT own another rebuild area.
      // Otherwise nested transparent elements (button -> animation -> image) store overlapping copies of the same
      // background and those copies can be restored/repainted more than once, accumulating alpha.
      //
      // A descendant of a clipping scrollable container must NOT own a rebuild area either: it moves with the
      // container's scroll, and the area's save/restore (a fixed screen rectangle) would repaint ghosts at
      // stale/scrolled positions. The container's single rebuild area covers all its content and redraws it as a unit.
      //
      // EXCEPTION — UI_ELEMENT_TYPE_ANIMATION: keep a per-sprite rebuild area even inside a scrolleable. The
      // previous rule forced Draw_Animation to Elements_SetToRedraw(owner, true) on every frame advance so the
      // owner's area could erase the old sprite. That recursively re-dirtied every text/option child (UI_Options
      // textbox Quixote, multioption trees) and collapsed FPS to ~11. Clip still applies via SetClipBox; the
      // animation's own area erases the prior frame without re-rasterizing the whole container.
      UI_ELEMENT* ancestor = element->GetFather();
      while(ancestor)
        {
          if(ancestor->MustReDraw()) { createarea = false; break; }

          UI_PROPERTY_SCROLLEABLE* sc = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(ancestor);
          if(sc && sc->Scroll_NeedClip() && (element->GetType() != UI_ELEMENT_TYPE_ANIMATION))
            {
              createarea = false;
              break;
            }
          ancestor = ancestor->GetFather();
        }

    }

  if(createarea)
    {     
      double area_left   = UI_BOUNDARYLINE_EdgeLeft  (x_position, element->GetBoundaryLine()->width);
      double area_right  = UI_BOUNDARYLINE_EdgeRight  (x_position, element->GetBoundaryLine()->width);
      double area_top    = UI_BOUNDARYLINE_EdgeTop    (y_position, element->GetBoundaryLine()->height);
      double area_bottom = UI_BOUNDARYLINE_EdgeBottom  (y_position, element->GetBoundaryLine()->height);

      // A scrollable container with a non-clipped (VISIBLE) axis lets its content overflow the viewport on that axis.
      // Those descendants no longer own rebuild areas, so the container's area must stretch to cover their overflow,
      // otherwise the overflowing pixels are never erased (animated icons pile up). The clipped axis stays at the
      // viewport (content there is clipped, so it never overflows).
      UI_PROPERTY_SCROLLEABLE* sc_self = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
      if(sc_self)
        {
          bool vx = (sc_self->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) == UI_OVERFLOW_VISIBLE);
          bool vy = (sc_self->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL)   == UI_OVERFLOW_VISIBLE);
          if(vx || vy) UI_SkinCanvas_ContentExtent(element, vx, vy, (double)UI_SKINCANVAS_PRESELECT_MAXEDGE, area_left, area_right, area_top, area_bottom);
        }

      // Step 7: expand the rebuild-area to include the box-shadow footprint, so save/restore cycles at
      // repaint time do not leave ghost pixels where the shadow was drawn OUTSIDE the element's own rect.
      // Positive offset extends the corresponding far side; negative extends the near side. Blur, when
      // rendered in a follow-up rebanada, will add further symmetric expansion on all sides -- we already
      // account for it below so the rebuild-area is right on the day blur ships.
      double  shadow_L = 0.0, shadow_R = 0.0, shadow_T = 0.0, shadow_B = 0.0;
      if(element->IsBoxShadowSet())
        {
          double sx = element->GetShadowOffsetX();
          double sy = element->GetShadowOffsetY();
          double sb = element->GetShadowBlur();

          if(sx >= 0.0) shadow_R = sx; else shadow_L = -sx;
          if(sy >= 0.0) shadow_B = sy; else shadow_T = -sy;

          if(sb > 0.0)
            {
              // Must match the off-screen bitmap padding used by UI_SkinCanvas_DrawSoftShadow exactly: the
              // blurred bitmap is composited from (shape - pad) to (shape + pad), so an expansion of only
              // `blur` would leave the outer ring of the fade outside the rebuild-area. Those pixels would
              // never be restored, so each repaint would blend a new shadow on top of the previous one and
              // the halo would darken frame after frame.
              double pad = (double)UI_SKINCANVAS_SHADOW_BLURPADDING(sb);

              shadow_L += pad; shadow_R += pad;
              shadow_T += pad; shadow_B += pad;
            }
        }

      CreateRebuildArea(area_left - edge - shadow_L, area_top - edge - shadow_T, (area_right - area_left) + (edge * 2) + shadow_L + shadow_R, (area_bottom - area_top ) + (edge * 2) + shadow_T + shadow_B, element);
    }

  UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
  if(scrolleable && scrolleable->Scroll_NeedClip())
    {
      double vp_width    = 0.0f;
      double vp_height   = 0.0f;
      GetScrollViewportSize(element, vp_width, vp_height);
      vp_width  = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_width);
      vp_height = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_height);

      double clip_top    = UI_BOUNDARYLINE_EdgeTop    (y_position, vp_height);
      double clip_bottom = UI_BOUNDARYLINE_EdgeBottom (y_position, vp_height);
      double clip_right  = UI_BOUNDARYLINE_EdgeRight  (x_position, vp_width);
      double clip_left   = x_position;

      canvas->GetClipBox(clip_rect);                                       // save the parent clip

      // Per-axis clipping (CSS overflow-x / overflow-y are independent): an axis whose overflow is VISIBLE is NOT
      // clipped (it inherits the parent bounds, so content may overflow on that axis). A clipped axis is INTERSECTED
      // with the parent clip, so a nested scrolleable is bounded by its ancestors instead of replacing their clip.
      bool clip_h = (scrolleable->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL) != UI_OVERFLOW_VISIBLE);
      bool clip_v = (scrolleable->Scroll_GetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL)   != UI_OVERFLOW_VISIBLE);

      // AGG RoundRect / border stroke AA needs a few px outside the authored box on EVERY side. Historically
      // only left/top were padded; right/bottom stayed flush, so overflow:hidden rounded forms (e.g. the
      // opstatus foot when expanded) lost their bottom-left / bottom-right border arcs. Pad all sides equally;
      // keep it small so children still respect overflow.
      const double stroke_aa_pad = 2.0;

      // element box and parent box, normalized to [min,max] (the stored clip can use either Y order)
      double e_minx = clip_left   - stroke_aa_pad;
      double e_maxx = clip_right  + stroke_aa_pad;
      double e_miny = clip_top    - stroke_aa_pad;
      double e_maxy = clip_bottom + stroke_aa_pad;

      double p_minx = __MIN(clip_rect.x1, clip_rect.x2);
      double p_maxx = __MAX(clip_rect.x1, clip_rect.x2);
      double p_miny = __MIN(clip_rect.y1, clip_rect.y2);
      double p_maxy = __MAX(clip_rect.y1, clip_rect.y2);

      double r_minx = clip_h ? __MAX(e_minx, p_minx) : p_minx;
      double r_maxx = clip_h ? __MIN(e_maxx, p_maxx) : p_maxx;
      double r_miny = clip_v ? __MAX(e_miny, p_miny) : p_miny;
      double r_maxy = clip_v ? __MIN(e_maxy, p_maxy) : p_maxy;

      canvas->SetClipBox(r_minx, r_maxy, r_maxx, r_miny);                  // (left, bottom, right, top): same order as before
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::PostDrawFunction(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double x_position, double y_position)
* @brief      Post draw function
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  canvas : Canvas object to use.
* @param[in]  clip_rect : Clip rect value.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::PostDrawFunction(UI_ELEMENT* element, GRP2DCANVAS* canvas, XRECT& clip_rect, double x_position, double y_position)
{
  bool redrew = element->MustReDraw();           // capture before clearing: true only when the area was just repainted

  element->SetMustReDraw(false);

  UI_PROPERTY_SCROLLEABLE* scrolleable = dynamic_cast<UI_PROPERTY_SCROLLEABLE*>(element);
  if(scrolleable && scrolleable->Scroll_NeedClip())
    {
      canvas->SetClipBox(clip_rect);
    }

  if(scrolleable)
    {
      ResolveScrollPolicy(element, scrolleable);                                       // state only (no drawing): always
      if(redrew) DrawScrollBars(element, scrolleable, canvas, x_position, y_position); // draw only when the gutter was just
                                                                                       // cleared, else the translucent bar
                                                                                       // would be painted over itself every
                                                                                       // frame and the alpha would accumulate
    }
  
  #ifdef USERINTERFACE_DEBUG 
	Debug_Draw(element, x_position, y_position);			
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS::GetScrollViewportSize(UI_ELEMENT* element, double& width, double& height)
* @brief      Returns the scroll viewport SIZE for an element. For a Form (or Menu) with a defined visiblerect this is the visible window; otherwise (and for non-Form scrolleables) it is the element boundary line. The box POSITION is left to each call site (it already tracks scroll), so only the size needs to change.
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::GetScrollViewportSize(UI_ELEMENT* element, double& width, double& height)
{
  width  = 0.0f;
  height = 0.0f;
  if(!element) return;

  UI_ELEMENT_FORM* element_form = dynamic_cast<UI_ELEMENT_FORM*>(element);
  if(element_form && element_form->GetVisibleRect()->width > 0 && element_form->GetVisibleRect()->height > 0)
    {
      width  = element_form->GetVisibleRect()->width;
      height = element_form->GetVisibleRect()->height;
    }
   else
    {
      width  = element->GetBoundaryLine()->width;
      height = element->GetBoundaryLine()->height;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::GetScrollContentBox(UI_ELEMENT* element, double x_position, double y_position, double& box_left, double& box_top, double& box_right, double& box_bottom, double& viewport_w, double& viewport_h)
* @brief      Scrollable content box used by ResolveScrollPolicy, DrawScrollBars and scrollbar hit-test. Matches the
*             element viewport, except UI_ELEMENT_PROPERTYLIST keeps a sticky header outside the scroll region.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to process.
* @param[in]  x_position : Resolved left edge (screen coords).
* @param[in]  y_position : Resolved bottom edge (screen coords).
* @param[out] box_left : Content box left.
* @param[out] box_top : Content box top (below sticky header when applicable).
* @param[out] box_right : Content box right.
* @param[out] box_bottom : Content box bottom.
* @param[out] viewport_w : Horizontal content viewport size.
* @param[out] viewport_h : Vertical content viewport size.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::GetScrollContentBox(UI_ELEMENT* element, double x_position, double y_position, double& box_left, double& box_top, double& box_right, double& box_bottom, double& viewport_w, double& viewport_h)
{
  box_left    = x_position;
  box_top     = y_position;
  box_right   = x_position;
  box_bottom  = y_position;
  viewport_w  = 0.0f;
  viewport_h  = 0.0f;
  if(!element) return;

  double box_w = 0.0f;
  double box_h = 0.0f;
  GetScrollViewportSize(element, box_w, box_h);

  double header_inset = 0.0f;
  UI_ELEMENT_PROPERTYLIST* element_propertylist = dynamic_cast<UI_ELEMENT_PROPERTYLIST*>(element);
  if(element_propertylist)
    {
      header_inset = element_propertylist->GetHeaderHeight();
      if(header_inset < 0.0f)     header_inset = 0.0f;
      if(header_inset > box_h)    header_inset = box_h;
    }

  viewport_w = box_w;
  viewport_h = box_h - header_inset;
  box_left   = x_position;
  box_right  = x_position + box_w;
  box_bottom = y_position;
  box_top    = UI_BOUNDARYLINE_EdgeTop(y_position, box_h) + header_inset;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::ResolveScrollPolicy(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable)
* @brief      Measures the content extent of a scrollable container and resolves its scroll policy (active/visible/limit).
* @note       Called at draw time (not only at layout) because children can be added after the layout pass (e.g. menu
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  scrolleable : Scrolleable pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::ResolveScrollPolicy(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable)
{
  if(!element || !scrolleable) return false;

  // One thickness for every overlay bar (vertical width / horizontal height): same as UI_ELEMENT_TREE.
  // Dock the track inside the frame line (sepedge >= border-width) so the bar never erases the right/bottom edge.
  scrolleable->Scroll_SetBarWidth(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, (double)UI_PROPERTY_SCROLLEABLE_BARWIDTH);
  {
    double bw = 0.0;
    if(element->IsBorderColorSet() && (element->GetBorderWidth() > 0.0)) bw = element->GetBorderWidth();
    double sepedge  = 0.0;
    double sepalong = 0.0;
    scrolleable->Scroll_GetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, sepedge, sepalong);
    if(bw > sepedge) scrolleable->Scroll_SetBarSeparation(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, bw, sepalong);
  }

  UI_ELEMENT_TEXTBOX* element_textbox = dynamic_cast<UI_ELEMENT_TEXTBOX*>(element);
  if(element_textbox)
    {
      // TextBox content is text, not compose elements: the scroll extent is the total laid-out text height (set by
      // TextBox_GenerateLines during draw). Text wraps to the box width, so there is no horizontal scroll.
      double tb_w = 0.0f;
      double tb_h = 0.0f;
      GetScrollViewportSize(element, tb_w, tb_h);

      double vp_w = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, tb_w);
      double vp_h = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, tb_h);

      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_w, vp_w);
      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_h, element_textbox->GetContentHeight());
      return true;
    }

  UI_ELEMENT_CONSOLE* element_console = dynamic_cast<UI_ELEMENT_CONSOLE*>(element);
  if(element_console)
    {
      double tb_w = 0.0f;
      double tb_h = 0.0f;
      GetScrollViewportSize(element, tb_w, tb_h);

      double vp_w = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, tb_w);
      double vp_h = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, tb_h);
      double cw   = element_console->GetContentWidth();
      if(cw < vp_w) cw = vp_w;

      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_w, cw);
      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_h, element_console->GetContentHeight());
      return true;
    }

  UI_ELEMENT_TREE* element_tree = dynamic_cast<UI_ELEMENT_TREE*>(element);
  if(element_tree)
    {
      double tr_w = 0.0f;
      double tr_h = 0.0f;
      GetScrollViewportSize(element, tr_w, tr_h);

      double vp_w = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, tr_w);
      double vp_h = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, tr_h);
      double cw   = element_tree->GetContentWidth();
      if(cw < vp_w) cw = vp_w;

      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_w, cw);
      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_h, element_tree->GetContentHeight());
      return true;
    }

  UI_ELEMENT_PROPERTYLIST* element_propertylist = dynamic_cast<UI_ELEMENT_PROPERTYLIST*>(element);
  if(element_propertylist)
    {
      // Body-only vertical viewport: sticky Name/Value header is outside the scroll region.
      double box_left   = 0.0f;
      double box_top    = 0.0f;
      double box_right  = 0.0f;
      double box_bottom = 0.0f;
      double pl_w       = 0.0f;
      double pl_h       = 0.0f;
      GetScrollContentBox(element, element->GetXPositionWithScroll(), element->GetYPositionWithScroll(), box_left, box_top, box_right, box_bottom, pl_w, pl_h);
      (void)box_left; (void)box_top; (void)box_right; (void)box_bottom;

      double vp_w = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, pl_w);
      double vp_h = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, pl_h);

      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_w, vp_w);
      scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_h, element_propertylist->GetContentHeight());
      return true;
    }

  bool   has  = false;
  double minx = 0.0f;
  double maxx = 0.0f;
  double miny = 0.0f;
  double maxy = 0.0f;

  for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* subelement = element->GetComposeElements()->Get(c);
      if(!subelement) continue;

      double sx = GetPositionWithoutDefine(subelement->GetBoundaryLine()->x);
      double sy = GetPositionWithoutDefine(subelement->GetBoundaryLine()->y);
      double sw = subelement->GetBoundaryLine()->width;
      double sh = subelement->GetBoundaryLine()->height;

      if(!has)
        {
          minx = sx;   maxx = sx + sw;
          miny = sy;   maxy = sy + sh;
          has  = true;
        }
       else
        {
          if(sx      < minx) minx = sx;
          if(sx + sw > maxx) maxx = sx + sw;
          if(sy      < miny) miny = sy;
          if(sy + sh > maxy) maxy = sy + sh;
        }
    }

  double box_w = 0.0f;
  double box_h = 0.0f;
  GetScrollViewportSize(element, box_w, box_h);

  double vp_w = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, box_w);
  double vp_h = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, box_h);

  // Content extent vs viewport [0 .. viewport]. When the content overflows, the scroll range is the overflow PLUS the
  // content's natural leading margin mirrored at the trailing edge, so the first and last items are framed the same
  // way (symmetric margins). When the content fits, extent == viewport (no scroll). Convention-robust: a constant
  // offset cancels in min/max.
  double content_w = vp_w;
  double content_h = vp_h;

  if(has)
    {
      double over_left   = (minx < 0.0f) ? (-minx)        : 0.0f;   // content past the near edge (overflow)
      double over_right  = (maxx > vp_w) ? (maxx - vp_w)  : 0.0f;   // content past the far edge
      double lead_margin = (vp_w > maxx) ? (vp_w - maxx)  : 0.0f;   // natural gap before content, mirrored after it
      if(over_left > 0.0f || over_right > 0.0f) content_w = vp_w + over_left + over_right + lead_margin;

      double over_below  = (miny < 0.0f) ? (-miny)        : 0.0f;
      double over_above  = (maxy > vp_h) ? (maxy - vp_h)  : 0.0f;
      double top_margin  = (vp_h > maxy) ? (vp_h - maxy)  : 0.0f;
      if(over_below > 0.0f || over_above > 0.0f) content_h = vp_h + over_below + over_above + top_margin;
    }

  scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, vp_w, content_w);
  scrolleable->Scroll_ResolvePolicy(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, vp_h, content_h);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::DrawScrollBars(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable, GRP2DCANVAS* canvas, double x_position, double y_position)
* @brief      Draws the overlay scrollbars (track + proportional thumb) of a scrollable container.
* @note       Overlay model: drawn after the content clip is restored and pinned to the container viewport (it does
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  scrolleable : Scrolleable pointer to use.
* @param[in]  canvas : Canvas object to use.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::DrawScrollBars(UI_ELEMENT* element, UI_PROPERTY_SCROLLEABLE* scrolleable, GRP2DCANVAS* canvas, double x_position, double y_position)
{
  if(!element || !scrolleable || !canvas) return false;

  double box_left   = 0.0f;
  double box_top    = 0.0f;
  double box_right  = 0.0f;
  double box_bottom = 0.0f;
  double box_w      = 0.0f;
  double box_h      = 0.0f;
  GetScrollContentBox(element, x_position, y_position, box_left, box_top, box_right, box_bottom, box_w, box_h);

  // PROPERTYLIST sticky header: the content box top is mid-element (not a rounded corner), so only keep
  // the radius inset at the bottom end of the vertical track.
  double radius      = (double)element->GetRoundRect();
  double radius_vert = radius;
  if(dynamic_cast<UI_ELEMENT_PROPERTYLIST*>(element)) radius_vert = 0.0f;

  double track_left;
  double track_top;
  double track_right;
  double track_bottom;
  int    tr_r;
  int    tr_g;
  int    tr_b;
  int    tr_a;
  int    th_r;
  int    th_g;
  int    th_b;
  int    th_a;

  // ---- vertical bar (right gutter) ----
  if(scrolleable->Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, box_left, box_top, box_right, box_bottom, radius_vert, track_left, track_top, track_right, track_bottom))
    {
      // Restore bottom corner clearance when radius_vert was cleared for a sticky header.
      if((radius_vert == 0.0f) && (radius > 0.0f)) track_bottom -= radius;

      if((track_bottom - track_top) > 0.0f)
        {
          double tracklen  = track_bottom - track_top;
          double viewport  = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, box_h);
          double limit     = scrolleable->Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);            // <= 0
          double disp      = scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);
          double content   = viewport - limit;                                                              // >= viewport
          double fraction  = (limit != 0.0f) ? (disp / limit) : 0.0f;

          if(fraction < 0.0f) fraction = 0.0f;
          if(fraction > 1.0f) fraction = 1.0f;

          double thumb     = (content > 0.0f) ? (tracklen * (viewport / content)) : tracklen;
          if(thumb < UI_SKINCANVAS_SCROLLBAR_MINTHUMB) thumb = UI_SKINCANVAS_SCROLLBAR_MINTHUMB;
          if(thumb > tracklen)                         thumb = tracklen;

          double thumb_top = track_top + (fraction * (tracklen - thumb));

          scrolleable->Scroll_GetBarTrackColor(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, tr_r, tr_g, tr_b, tr_a);
          scrolleable->Scroll_GetBarThumbColor(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, th_r, th_g, th_b, th_a);

          GRP2DCOLOR_RGBA8 trackcolor(tr_r, tr_g, tr_b, tr_a);
          GRP2DCOLOR_RGBA8 thumbcolor(th_r, th_g, th_b, th_a);

          canvas->SetLineWidth(0.0f);
          canvas->SetLineColor(&trackcolor);
          canvas->SetFillColor(&trackcolor);
          DrawScrollPill(canvas, track_left, track_bottom, track_right, track_top, radius);

          canvas->SetLineColor(&thumbcolor);
          canvas->SetFillColor(&thumbcolor);
          DrawScrollPill(canvas, track_left, thumb_top + thumb, track_right, thumb_top, radius);
        }
    }

  // ---- horizontal bar (bottom gutter) ----
  if(scrolleable->Scroll_GetTrackRect(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, box_left, box_top, box_right, box_bottom, radius, track_left, track_top, track_right, track_bottom))
    {
      double tracklen  = track_right - track_left;
      double viewport  = scrolleable->Scroll_GetContentViewport(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, box_w);
      double limit     = scrolleable->Scroll_GetLimit(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);
      double disp      = scrolleable->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);
      double content   = viewport - limit;
      double fraction  = (limit != 0.0f) ? (disp / limit) : 0.0f;

      if(fraction < 0.0f) fraction = 0.0f;
      if(fraction > 1.0f) fraction = 1.0f;

      double thumb     = (content > 0.0f) ? (tracklen * (viewport / content)) : tracklen;
      if(thumb < UI_SKINCANVAS_SCROLLBAR_MINTHUMB) thumb = UI_SKINCANVAS_SCROLLBAR_MINTHUMB;
      if(thumb > tracklen)                         thumb = tracklen;

      double thumb_left = track_left + (fraction * (tracklen - thumb));

      scrolleable->Scroll_GetBarTrackColor(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, tr_r, tr_g, tr_b, tr_a);
      scrolleable->Scroll_GetBarThumbColor(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, th_r, th_g, th_b, th_a);

      GRP2DCOLOR_RGBA8 trackcolor(tr_r, tr_g, tr_b, tr_a);
      GRP2DCOLOR_RGBA8 thumbcolor(th_r, th_g, th_b, th_a);

      canvas->SetLineWidth(0.0f);
      canvas->SetLineColor(&trackcolor);
      canvas->SetFillColor(&trackcolor);
      DrawScrollPill(canvas, track_left, track_bottom, track_right, track_top, radius);

      canvas->SetLineColor(&thumbcolor);
      canvas->SetFillColor(&thumbcolor);
      DrawScrollPill(canvas, thumb_left, track_bottom, thumb_left + thumb, track_top, radius);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKINCANVAS::TextBox_SizeLine(UI_ELEMENT_TEXTBOX* element_textbox, GRP2DCANVAS* canvas, double x_position, double y_position, int nline, XSTRING& characterstr, XDWORD index_char, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts)
* @brief      Text box size line
* @ingroup    USERINTERFACE
* 
* @param[in]  element_textbox : Element textbox pointer to use.
* @param[in]  canvas : Canvas object to use.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* @param[in]  nline : Nline value.
* @param[in]  characterstr : Characterstr value.
* @param[in]  index_char : Index char value.
* @param[in]  parts : Parts pointer to use.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKINCANVAS::TextBox_SizeLine(UI_ELEMENT_TEXTBOX* element_textbox, GRP2DCANVAS* canvas, double x_position, double y_position, int nline, XSTRING& characterstr, XDWORD index_char, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts)
{
  XSTRING _characterstr;
  XDWORD  y_pos    = 0;    
  double  sizeline = 0;

  _characterstr = characterstr;
                               
  for(XDWORD c=0; c<parts.GetSize(); c++)
    {
      UI_SKIN_TEXTBOX_PART* textbox_part = parts.Get(c);
      if(textbox_part->GetLineNumber() == nline)
        {
          sizeline += canvas->VectorFont_GetWidth(textbox_part->GetText()->Get());    
          y_pos     = textbox_part->GetYPos();    
        }                                      
    }

  _characterstr.Add(element_textbox->GetText()->Get()[index_char]); 
  sizeline += canvas->VectorFont_GetWidth(_characterstr.Get());  

  return sizeline;
}


#define UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES   32

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_TextObstacle(double x, double ltop, double lbot, const double* ol, const double* orr, const double* ot, const double* ob, int n, double& jumpx, double& aheadleft, double& aheadright)
* @brief      UI Skin Canvas Text Obstacle
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X coordinate.
* @param[in]  ltop : Ltop value.
* @param[in]  lbot : Lbot value.
* @param[in]  ol : Ol value.
* @param[in]  orr : Orr value.
* @param[in]  ot : Ot value.
* @param[in]  ob : Ob value.
* @param[in]  n : N value.
* @param[in]  jumpx : Jumpx value.
* @param[in]  aheadleft : Aheadleft value.
* @param[in]  aheadright : Aheadright value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
// Inline-image obstacle resolver for the text box. Active images are rectangles the running text must flow around.
// For a line whose vertical band [ltop,lbot] overlaps an image, this advances the cursor past any image it currently
// sits inside (jumpx), and reports the nearest image edge still ahead on that line (aheadleft/aheadright) so a word is
// never laid down on top of the picture. With no active images the outputs leave the cursor untouched (aheadleft < 0),
// so the layout is byte-for-byte the original behaviour.
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SkinCanvas_TextObstacle(double x, double ltop, double lbot, const double* ol, const double* orr, const double* ot, const double* ob, int n, double& jumpx, double& aheadleft, double& aheadright)
* @brief      Skin canvas text obstacle
* @ingroup    USERINTERFACE
* 
* @param[in]  x : X value.
* @param[in]  ltop : Ltop value.
* @param[in]  lbot : Lbot value.
* @param[in]  ol : Pointer to ol.
* @param[in]  orr : Pointer to orr.
* @param[in]  ot : Pointer to ot.
* @param[in]  ob : Pointer to ob.
* @param[in]  n : N value.
* @param[in]  jumpx : Jumpx value.
* @param[in]  aheadleft : Aheadleft value.
* @param[in]  aheadright : Aheadright value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void UI_SkinCanvas_TextObstacle(double x, double ltop, double lbot, const double* ol, const double* orr, const double* ot, const double* ob, int n, double& jumpx, double& aheadleft, double& aheadright)
{
  jumpx = x;

  bool changed = true;
  while(changed)
    {
      changed = false;
      for(int i=0; i<n; i++)
        {
          if((lbot > ot[i]) && (ltop < ob[i]) && (jumpx >= ol[i]) && (jumpx < orr[i])) { jumpx = orr[i]; changed = true; }
        }
    }

  aheadleft  = -1.0f;
  aheadright = -1.0f;

  bool   has  = false;
  double best = 0.0f;
  for(int i=0; i<n; i++)
    {
      if((lbot > ot[i]) && (ltop < ob[i]) && (ol[i] > jumpx))
        {
          if(!has || (ol[i] < best)) { best = ol[i]; aheadleft = ol[i]; aheadright = orr[i]; has = true; }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::TextBox_GenerateLines(UI_ELEMENT_TEXTBOX* element_textbox, GRP2DCANVAS* canvas, double x_position, double y_position, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts)
* @brief      Text box generate lines
* @ingroup    USERINTERFACE
* 
* @param[in]  element_textbox : Element textbox pointer to use.
* @param[in]  canvas : Canvas object to use.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* @param[in]  parts : Parts pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::TextBox_GenerateLines(UI_ELEMENT_TEXTBOX* element_textbox, GRP2DCANVAS* canvas, double x_position, double y_position, XVECTOR<UI_SKIN_TEXTBOX_PART*>& parts)
{
  double                  x_text_position = x_position;
  double                  y_text_position = UI_BOUNDARYLINE_EdgeTop(y_position, element_textbox->GetBoundaryLine()->height) + (double)canvas->VectorFont_GetHeight(NL("A")) + element_textbox->Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);  
  XDWORD                  index_char      = 0;  
  UI_COLOR                actual_color;
  XDWORD                  nline           = 1;
  double                  lineheight      = (canvas->VectorFont_GetHeight(NL("A")) + element_textbox->GetLineSpacing());
  XSTRING                 text;
  UI_SKIN_TEXTBOX_PART*   textbox_part    = NULL;
  bool                    outlimit        = false;
  bool                    newpart         = false;  

  double                  fontheight      = (double)canvas->VectorFont_GetHeight(NL("A"));
  double                  box_right       = (double)element_textbox->GetXPosition() + (double)element_textbox->GetBoundaryLine()->width;

  double                  obs_l[UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES];   // active inline-image rectangles (text flows around them)
  double                  obs_r[UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES];
  double                  obs_t[UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES];
  double                  obs_b[UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES];
  int                     n_obs           = 0;
  double                  max_obs_bottom  = 0.0f;
  double                  firstline_top   = y_text_position - fontheight;   // top of line 1 (scroll-independent reference for image content height)

  text = element_textbox->GetText()->Get();
  if(text.IsEmpty()) { element_textbox->SetContentHeight(0.0f); return false; }

  actual_color.CopyFrom(element_textbox->GetColor());

  textbox_part = GEN_NEW UI_SKIN_TEXTBOX_PART();
  if(!textbox_part) return false;
   
  textbox_part->SetLineNumber(nline);
  textbox_part->SetXPos((XDWORD)x_text_position);  
  textbox_part->SetYPos((XDWORD)y_text_position);  
  textbox_part->GetColor()->CopyFrom(&actual_color);
  
  while(index_char < text.GetSize() && (!outlimit))  
    {
      // flow the text around active inline images: if the cursor lands inside an image band on this line, jump past it
      if(n_obs)
        {
          double jumpx       = x_text_position;
          double aheadleft   = -1.0f;
          double aheadright  = -1.0f;
          UI_SkinCanvas_TextObstacle(x_text_position, y_text_position - fontheight, y_text_position, obs_l, obs_r, obs_t, obs_b, n_obs, jumpx, aheadleft, aheadright);
          if(jumpx > x_text_position)
            {
              if(!textbox_part->GetText()->IsEmpty())
                {
                  textbox_part->SetWidth((XDWORD)canvas->VectorFont_GetWidth(textbox_part->GetText()->Get()));
                  textbox_part->SetHeight((XDWORD)canvas->VectorFont_GetHeight(NL("A")));
                  parts.Add(textbox_part);
                  textbox_part = GEN_NEW UI_SKIN_TEXTBOX_PART();
                  if(!textbox_part) return false;
                }
              x_text_position = jumpx;
              textbox_part->SetLineNumber(nline);
              textbox_part->SetXPos((XDWORD)x_text_position);
              textbox_part->SetYPos((XDWORD)y_text_position);
              textbox_part->GetColor()->CopyFrom(&actual_color);
            }
        }

      XCHAR character = text.Get()[index_char];

      switch(character)
        {
          case _C('\n')  : x_text_position  = x_position;      
                            y_text_position += lineheight;                
                            nline++;
                            newpart = true;     

                            index_char++;                            
                            break;

          case _C('!')   : { int indexstart = index_char+1;
                              if(text.Get()[indexstart] == _C('['))
                                {                
                                  int type;
                
                                  int indexend = text.FindCharacter(_C(']'), indexstart);
                                  if(indexend != XSTRING_NOTFOUND)
                                    {                                                                      
                                      XCHAR*    keywords[] = {  NL("COLOR") ,                                                              
                                                                NL("END") ,
                                                                NL("IMAGE") ,
                                                             };
                                      XSTRING   keyword;
                                      bool      found_keyword = false;  
                                                                                    
                                      text.Copy(indexstart+1, indexend, keyword);    

                                      for(type=0; type<(sizeof(keywords)/sizeof(XCHAR*)); type++)
                                        {
                                          int keywordindex = keyword.Find(keywords[type], true);
                                          if(!keywordindex)
                                            {
                                              XSTRING keyword_params;
                                              XSTRING string;        
                                              
                                              found_keyword = true;  

                                              string = keywords[type];
                                              keywordindex += string.GetSize()+1;

                                              keyword.Copy(keywordindex, keyword_params);
                                              keyword_params.DeleteCharacter(_C(' '));

                                              switch(type)
                                                {
                                                  case  0 : actual_color.SetFromString(keyword_params);
                                                            newpart = true;
                                                            break;      

                                                  case  1 : for(int d=0; d<(sizeof(keywords)/sizeof(XCHAR*)); d++)
                                                              {
                                                                if(!keyword_params.Compare(keywords[d], true))
                                                                  {
                                                                    switch(d)
                                                                      {
                                                                        case  0 : actual_color.CopyFrom(element_textbox->GetColor());
                                                                                  newpart = true;
                                                                                  break;

                                                                        case  1 : break;  

                                                                        case  2 : break;   // "END IMAGE" -> no-op (images are single inline insertions)
                                                                      }                                                                    
                                                                  }
                                                              }
                                                            break;                                                          

                                                  case  2 : { // IMAGE <file> : insert an inline image; following text flows after it (wrapping to the next line)
                                                              GRPBITMAP*    bitmap = NULL;
                                                              UI_ANIMATION* anim   = GEN_USERINTERFACE.GetOrAddAnimationCache(UI_SKIN_DRAWMODE_CANVAS, canvas->GetMode(), NL(""), keyword_params.Get());
                                                              if(anim) bitmap = anim->GetBitmap();

                                                              if(bitmap)
                                                                {
                                                                  double imgwidth  = (double)bitmap->GetWidth();
                                                                  double imgheight = (double)bitmap->GetHeight();
                                                                  double sep       = (double)element_textbox->GetImageSeparation();   // gap kept around the graphic (all edges)

                                                                  // close the current text run (the text that precedes the image)
                                                                  if(!textbox_part->GetText()->IsEmpty())
                                                                    {
                                                                      textbox_part->SetWidth((XDWORD)canvas->VectorFont_GetWidth(textbox_part->GetText()->Get()));
                                                                      textbox_part->SetHeight((XDWORD)canvas->VectorFont_GetHeight(NL("A")));
                                                                      parts.Add(textbox_part);
                                                                      textbox_part = GEN_NEW UI_SKIN_TEXTBOX_PART();
                                                                    }

                                                                  // wrap to a new line if the image (plus its side margins) does not fit on the current one
                                                                  if((x_text_position > x_position) && ((x_text_position + sep + imgwidth + sep) > box_right))
                                                                    {
                                                                      x_text_position  = x_position;
                                                                      y_text_position += lineheight;
                                                                      nline++;
                                                                    }

                                                                  // The image grows DOWNWARD from the top of the current line (so it never paints over the line above).
                                                                  // It is drawn at its real rectangle, but the obstacle the text flows around is inflated by 'sep' on every
                                                                  // edge, so a configurable gap is kept between the graphic and the text on all four sides.
                                                                  double img_left = x_text_position + sep;
                                                                  double img_top  = y_text_position - fontheight;
                                                                  double img_bot  = img_top + imgheight;

                                                                  if(textbox_part)
                                                                    {
                                                                      UI_SKIN_TEXTBOX_PART* imagepart = GEN_NEW UI_SKIN_TEXTBOX_PART();
                                                                      if(imagepart)
                                                                        {
                                                                          imagepart->SetImage(bitmap);
                                                                          imagepart->SetLineNumber(nline);
                                                                          imagepart->SetXPos((XDWORD)img_left);
                                                                          imagepart->SetYPos((XDWORD)img_bot);     // drawn at ypos - height = img_top (downward from the line)
                                                                          imagepart->SetWidth((XDWORD)imgwidth);
                                                                          imagepart->SetHeight((XDWORD)imgheight);
                                                                          parts.Add(imagepart);
                                                                        }
                                                                    }

                                                                  if(n_obs < UI_SKINCANVAS_TEXTBOX_MAXINLINEIMAGES)
                                                                    {
                                                                      obs_l[n_obs] = img_left - sep;             // left margin   (= x_text_position)
                                                                      obs_r[n_obs] = img_left + imgwidth + sep;  // right margin
                                                                      obs_t[n_obs] = img_top;                   // top aligns with the line (image grows downward; the line above stays free)
                                                                      obs_b[n_obs] = img_bot + sep;             // bottom margin (keeps the line below clear)
                                                                      n_obs++;
                                                                    }
                                                                  if((img_bot + sep) > max_obs_bottom) max_obs_bottom = img_bot + sep;

                                                                  x_text_position = img_left + imgwidth;     // following text re-flows past the image (the right margin is applied by the obstacle jump)

                                                                  // re-anchor the (empty) current part for the text that follows the image on this line
                                                                  if(textbox_part)
                                                                    {
                                                                      textbox_part->SetLineNumber(nline);
                                                                      textbox_part->SetXPos((XDWORD)x_text_position);
                                                                      textbox_part->SetYPos((XDWORD)y_text_position);
                                                                      textbox_part->GetColor()->CopyFrom(&actual_color);
                                                                    }

                                                                  newpart = false;
                                                                }
                                                            }
                                                            break;
                                                }
                                              
                                              break;
                                            }
                                        }
                                                                  
                                      if(found_keyword) 
                                        {    
                                          text.DeleteCharacters(index_char, (keyword.GetSize() + 3));                                         
                                          
                                                        
                                          if(type == 1 && (indexstart > 1))
                                            {
                                              indexstart = -2;

                                              if(text.Get()[indexstart] != _C(' '))  
                                                {
                                                  text.Insert(NL(" "), indexstart);                                                                                                  
                                                }
                                            }
                                            
                                          break;
                                        }
                                    }
                                } 
                            }

                  default : { XSTRING characterstr;                              
                              characterstr.Add(character);
                              
                              double            charwidth = (double)canvas->VectorFont_GetWidth(characterstr.Get());
                              double            xbefore   = x_text_position;
                              x_text_position += charwidth; 

                              UI_BOUNDARYLINE   boundaryline;
                              bool              isinbox    = false;
                                
                              boundaryline.x      = element_textbox->GetXPosition();
                              boundaryline.y      = element_textbox->GetYPosition();
                              boundaryline.width  = element_textbox->GetBoundaryLine()->width;
                              boundaryline.height = element_textbox->GetBoundaryLine()->height;

                              isinbox = boundaryline.IsWithin((XDWORD)(x_text_position), (XDWORD)(y_text_position));                                                                                        

                              // would this character paint on top of an inline image ahead? jump past it (or wrap if the image reaches the margin)
                              bool jumpedimage = false;
                              if(isinbox && n_obs)
                                {
                                  double jx         = xbefore;
                                  double aheadleft  = -1.0f;
                                  double aheadright = -1.0f;
                                  UI_SkinCanvas_TextObstacle(xbefore, y_text_position - fontheight, y_text_position, obs_l, obs_r, obs_t, obs_b, n_obs, jx, aheadleft, aheadright);
                                  if((aheadleft >= 0.0f) && (x_text_position > aheadleft))
                                    {
                                      if(aheadright < box_right)
                                        {
                                          if(!textbox_part->GetText()->IsEmpty())
                                            {
                                              textbox_part->SetWidth((XDWORD)canvas->VectorFont_GetWidth(textbox_part->GetText()->Get()));
                                              textbox_part->SetHeight((XDWORD)canvas->VectorFont_GetHeight(NL("A")));
                                              parts.Add(textbox_part);
                                              textbox_part = GEN_NEW UI_SKIN_TEXTBOX_PART();
                                              if(!textbox_part) return false;
                                            }
                                          x_text_position = aheadright;
                                          textbox_part->SetLineNumber(nline);
                                          textbox_part->SetXPos((XDWORD)x_text_position);
                                          textbox_part->SetYPos((XDWORD)y_text_position);
                                          textbox_part->GetColor()->CopyFrom(&actual_color);
                                          jumpedimage = true;       // reprocess this character after the image (do not consume it)
                                        }
                                       else
                                        {
                                          isinbox = false;          // image runs to the right margin -> wrap to the next line
                                        }
                                    }
                                }

                              if(jumpedimage)
                                {
                                  // character intentionally not consumed; it will be laid down after the image on the next pass
                                }
                               else if(isinbox)
                                {                                        
                                  textbox_part->GetText()->Add(characterstr);
                                  index_char++;
                                }
                               else
                                {  
                                  if(element_textbox->IsWordClipping())
                                    {      
                                      while(!textbox_part->GetText()->IsEmpty())
                                        {
                                          if(textbox_part->GetText()->Get()[textbox_part->GetText()->GetSize()-1] == _C(' ')) break;
                                          textbox_part->GetText()->DeleteLastCharacter();
                                          index_char--;
                                        }          
                                    }
                                    
                                  x_text_position  = x_position;      
                                  y_text_position += lineheight; 
                                  nline++;

                                  if(!textbox_part->GetText()->IsEmpty()) 
                                    {
                                      newpart = true;                                      
                                    }
                                   else
                                    {
                                      textbox_part->SetLineNumber(nline);
                                      textbox_part->SetXPos((XDWORD)x_text_position);  
                                      textbox_part->SetYPos((XDWORD)y_text_position);  
                                    }
                                }                               
                            }
                            break;
        }


      if(newpart)
        {
          if(!textbox_part->GetText()->IsEmpty())    
            {
              textbox_part->SetWidth((XDWORD)canvas->VectorFont_GetWidth(textbox_part->GetText()->Get()));
              textbox_part->SetHeight((XDWORD)canvas->VectorFont_GetHeight(NL("A")));
              parts.Add(textbox_part);  

              textbox_part = GEN_NEW UI_SKIN_TEXTBOX_PART();
              if(!textbox_part) return false;
            }
   
          textbox_part->SetLineNumber(nline);
          textbox_part->SetXPos((XDWORD)x_text_position);  
          textbox_part->SetYPos((XDWORD)y_text_position);  
          textbox_part->GetColor()->CopyFrom(&actual_color);

          newpart = false;
        }
    }

  if(outlimit)
    {
      GEN_DELETE textbox_part;
      textbox_part = NULL;
    }
   else
    {
      parts.Add(textbox_part);  
    }

  XVECTOR<UI_SKIN_TEXTBOX_PART*>    checkparts;
  XDWORD                            maxnlines   = 0;

  for(XDWORD c=0; c<parts.GetSize(); c++)
    {
      UI_SKIN_TEXTBOX_PART* textbox_part = parts.Get(c);
      if(textbox_part) maxnlines = __MAX(maxnlines, textbox_part->GetLineNumber());  
    }

  for(XDWORD nl=0; nl<maxnlines; nl++)
    {     
      for(XDWORD p=0; p<parts.GetSize(); p++)
        {          
          UI_SKIN_TEXTBOX_PART* textbox_part = parts.Get(p);        
          if(textbox_part) 
            {
              if(textbox_part->GetLineNumber() == nl+1) checkparts.Add(textbox_part);
            }
        }

      if(checkparts.GetSize())
        {  
          switch(element_textbox->GetTextAlignment())
            {             
                                      default   : break;

              case UI_ELEMENT_TYPE_ALIGN_LEFT	  : break;										

              case UI_ELEMENT_TYPE_ALIGN_RIGHT	:	break;																				                  

              case UI_ELEMENT_TYPE_ALIGN_CENTER	: { // a line that flows around an inline image is kept left-aligned, so the wrap stays aligned to the picture
                                                    bool constrained = false;
                                                    if(n_obs)
                                                      {
                                                        for(XDWORD p=0; p<checkparts.GetSize() && (!constrained); p++)
                                                          {
                                                            UI_SKIN_TEXTBOX_PART* cp = checkparts.Get(p);
                                                            if(!cp) continue;
                                                            double ly = (double)cp->GetYPos();
                                                            for(int i=0; i<n_obs; i++)
                                                              {
                                                                if((ly > obs_t[i]) && ((ly - fontheight) < obs_b[i])) { constrained = true; break; }
                                                              }
                                                          }
                                                      }

                                                    if(!constrained)
                                                      {
                                                        XDWORD sizealltext = 0;
                                                        for(XDWORD p=0; p<checkparts.GetSize(); p++)
                                                          {          
                                                            UI_SKIN_TEXTBOX_PART* textbox_part = checkparts.Get(p);        
                                                            if(textbox_part) sizealltext += textbox_part->GetWidth();
                                                          }
                                          
                                                        XDWORD shift = (XDWORD)(element_textbox->GetBoundaryLine()->width - sizealltext)/2;    

                                                        for(XDWORD p=0; p<checkparts.GetSize(); p++)
                                                          {          
                                                            UI_SKIN_TEXTBOX_PART* textbox_part = checkparts.Get(p);        
                                                            if(textbox_part) textbox_part->SetXPos(textbox_part->GetXPos()+shift); 
                                                          }
                                                      }
                                                  }       
                                                  break;										           
            }              
        }

      checkparts.DeleteAll();                 
    }

  {
    double textheight  = (double)maxnlines * lineheight;
    double imageextent = max_obs_bottom - firstline_top;                  // image bottom relative to the top of line 1 (scroll cancels out)
    element_textbox->SetContentHeight(__MAX(textheight, imageextent));    // make sure scrolling can reach the bottom of an image too
  }

  return true;
}


#ifdef USERINTERFACE_DEBUG 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::Debug_Draw(UI_ELEMENT* element, double x_position, double y_position)
* @brief      Debug draw
* @ingroup    USERINTERFACE
* 
* @param[in]  element : Element to process.
* @param[in]  x_position : X position value.
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::Debug_Draw(UI_ELEMENT* element, double x_position, double y_position)
{  
  GRP2DCANVAS* canvas = GetCanvas();
  if(!canvas) return false;
  
  GRP2DCOLOR_RGBA8  colorred(255, 0, 0);
  GRP2DCOLOR_RGBA8  colorblue(0, 0, 255);
 
  canvas->SetLineWidth(1.0f);
  canvas->SetLineColor(&colorblue);  
  canvas->Circle(x_position, y_position, 5);               

  canvas->SetLineWidth(1.0f);
  canvas->SetLineColor(&colorred);
  canvas->Rectangle(x_position, y_position, x_position + (element->GetBoundaryLine()->width), UI_BOUNDARYLINE_EdgeTop(y_position, element->GetBoundaryLine()->height));
  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::Clean()
{
  fontsize                     = 0;
  screen                       = NULL;
  viewportindex                = 0;
  canvas_override              = NULL;
  paint_density                = 1.0;
  compact_legends_need_repair  = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_SKINCANVAS::GetPaintDensity() const
* @brief      Fase 7: canvas px per design px while painting (1.0 = identity).
* @ingroup    USERINTERFACE
* 
* @return     double : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_SKINCANVAS::GetPaintDensity() const
{
  return paint_density;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_SKINCANVAS::SetPaintDensity(double density)
* @brief      Fase 7: set paint density for denser design canvas (clamped to > 0).
* @ingroup    USERINTERFACE
* 
* @param[in]  density : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::SetPaintDensity(double density)
{
  if(density < UI_LAYOUT_UISCALE_MIN) density = UI_LAYOUT_UISCALE_MIN;
  paint_density = density;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::ProgressBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent "true backdrop" snapshot previously captured for a progress-bar widget's
*             FULL natural box (rect + gap + caption together).
* @note       See the GHOST-FILL FIX comment in Draw_ProgressBar() and the progressbackdrops member comment in
*             UI_SkinCanvas.h. Linear scan is deliberate: this cache only ever holds one entry per progress-bar
*             widget in a layout (a handful at most), so there is nothing to gain from a hash lookup here.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Progress-bar element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     GRP2DREBUILDAREA* : The cached entry (xpos/ypos/bitmap already positioned for PutBitmapNoAlpha);
*                                  NULL if this widget has never been captured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::ProgressBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<progressbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = progressbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::ProgressBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Captures the CURRENT on-screen pixels under a progress-bar widget's full natural box and keeps them
*             indefinitely as that widget's "true backdrop" reference.
* @note       Only ever correct to call the FIRST time a given widget is about to be drawn (see the caller in
*             Draw_ProgressBar(), gated on ProgressBackdrop_Find() returning NULL): at that point nothing has
*             painted a rect, animation or caption glyph in this zone yet, so whatever is on screen right now
*             genuinely IS the backdrop -- the same real pixels (seamless background pattern, gradient,
*             translucent panel, whatever this widget actually sits on) that must show through on every later
*             redraw. Reuses the inherited GetBitmap() -- the exact same capture primitive
*             UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea() itself uses -- so this is GEN's own real capture
*             machinery, not a new one.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Progress-bar element this capture belongs to (stored only as an opaque identity key).
* @param[in]  x       : Left edge of the region to capture, in canvas coordinates.
* @param[in]  y       : Top edge of the region to capture, in canvas coordinates.
* @param[in]  width   : Width of the region to capture.
* @param[in]  height  : Height of the region to capture.
*
* @return     bool : true if the capture was stored; false if the bitmap grab or allocation failed (caller simply
*                     has no cached backdrop this tick and behaves as before this fix -- never worse).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::ProgressBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);
  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return progressbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_PROGRESSBAR_LASTBOUNDS* UI_SKINCANVAS::ProgressBounds_Find(UI_ELEMENT* element)
* @brief      Look up the last-known element_progressrect/element_animation bounds recorded for a progress bar.
* @note       See the ProgressBounds_Remember/HasChanged declarations and the GHOST-FILL FIX comment in
*             Draw_ProgressBar(). Linear scan is deliberate: same small-cardinality reasoning as
*             ProgressBackdrop_Find() above (one entry per progress-bar widget in a layout).
* @ingroup    USERINTERFACE
*
* @param[in]  element : Progress-bar element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     UI_PROGRESSBAR_LASTBOUNDS* : The cached entry; NULL if this widget's bounds have never been
*                                          recorded yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROGRESSBAR_LASTBOUNDS* UI_SKINCANVAS::ProgressBounds_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<progressbarlastbounds.GetSize(); c++)
    {
      UI_PROGRESSBAR_LASTBOUNDS* entry = progressbarlastbounds.Get(c);
      if(entry && (entry->element == element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_SKINCANVAS::ProgressBounds_Remember(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight)
* @brief      Records (creating the entry the first time) the CURRENT element_progressrect/element_animation
*             bounds for a progress bar, so the next tick can tell whether they moved.
* @ingroup    USERINTERFACE
*
* @param[in]  element     : Progress-bar element these bounds belong to (stored only as an opaque identity key).
* @param[in]  rectx       : element_progressrect's current X position (0 if this widget has no rect sub-element).
* @param[in]  recty       : element_progressrect's current Y position.
* @param[in]  rectwidth   : element_progressrect's current width.
* @param[in]  rectheight  : element_progressrect's current height.
* @param[in]  animx       : element_animation's current X position (0 if this widget has no animation sub-element).
* @param[in]  animy       : element_animation's current Y position.
* @param[in]  animwidth   : element_animation's current width.
* @param[in]  animheight  : element_animation's current height.
*
* @return     void.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_SKINCANVAS::ProgressBounds_Remember(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight)
{
  if(!element) return;

  UI_PROGRESSBAR_LASTBOUNDS* entry = ProgressBounds_Find(element);

  if(!entry)
    {
      entry = GEN_NEW UI_PROGRESSBAR_LASTBOUNDS();
      if(!entry) return;

      entry->element = element;

      progressbarlastbounds.Add(entry);
    }

  entry->rectx      = rectx;
  entry->recty      = recty;
  entry->rectwidth  = rectwidth;
  entry->rectheight = rectheight;
  entry->animx      = animx;
  entry->animy      = animy;
  entry->animwidth  = animwidth;
  entry->animheight = animheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::ProgressBounds_HasChanged(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight)
* @brief      Compares the CURRENT element_progressrect/element_animation bounds against the last-recorded ones.
* @note       An element with no recorded entry yet counts as "changed" (the caller in Draw_ProgressBar() only
*             calls this after already handling the "never captured" case via ProgressBackdrop_Find() returning
*             NULL, so in practice this is always called with an existing entry -- treating a missing one as
*             changed is simply the safe default were that ever not true). Exact double comparison is
*             deliberate and safe here: both the current values and the remembered ones come from the same
*             deterministic, absolute (never incremental -- see CalculateBoundaryLine_ProgressBar()) layout
*             math, so two ticks with nothing genuinely different between them always compare bit-for-bit equal.
* @ingroup    USERINTERFACE
*
* @param[in]  element     : Progress-bar element to check.
* @param[in]  rectx       : element_progressrect's CURRENT X position to compare.
* @param[in]  recty       : element_progressrect's CURRENT Y position to compare.
* @param[in]  rectwidth   : element_progressrect's CURRENT width to compare.
* @param[in]  rectheight  : element_progressrect's CURRENT height to compare.
* @param[in]  animx       : element_animation's CURRENT X position to compare.
* @param[in]  animy       : element_animation's CURRENT Y position to compare.
* @param[in]  animwidth   : element_animation's CURRENT width to compare.
* @param[in]  animheight  : element_animation's CURRENT height to compare.
*
* @return     bool : true if these bounds differ from the last-recorded ones (or none were recorded yet);
*                    false if they are identical, meaning nothing moved since the last tick we looked.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::ProgressBounds_HasChanged(UI_ELEMENT* element, double rectx, double recty, double rectwidth, double rectheight, double animx, double animy, double animwidth, double animheight)
{
  UI_PROGRESSBAR_LASTBOUNDS* entry = ProgressBounds_Find(element);

  if(!entry) return true;

  return (entry->rectx      != rectx)      ||
         (entry->recty      != recty)      ||
         (entry->rectwidth  != rectwidth)  ||
         (entry->rectheight != rectheight) ||
         (entry->animx      != animx)      ||
         (entry->animy      != animy)      ||
         (entry->animwidth  != animwidth)  ||
         (entry->animheight != animheight);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::FormBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent "true backdrop" snapshot previously captured for a "form"/"menu" element's
*             own box (see the ALPHA-DARKENING FIX comment in Draw_Form() and the formbackdrops member comment
*             in UI_SkinCanvas.h).
* @note       Linear scan is deliberate: same small-cardinality reasoning as ProgressBackdrop_Find() above -- a
*             typical layout has only a handful of translucent forms/menus.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Form/menu element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     GRP2DREBUILDAREA* : The cached entry (xpos/ypos/bitmap already positioned for PutBitmapNoAlpha);
*                                  NULL if this element has never been captured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::FormBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<formbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = formbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::FormBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Captures the CURRENT on-screen pixels under a form/menu element's own box and keeps them indefinitely
*             as that element's "true backdrop" reference.
* @note       Only ever correct to call the FIRST time a given element is about to be drawn (see the caller in
*             Draw_Form(), gated on FormBackdrop_Find() returning NULL): at that point nothing has painted this
*             element's own shadow, fill, border or children in this zone yet, so whatever is on screen right now
*             genuinely IS the backdrop (the real background image/pattern, or whatever sits behind this element).
*             Reuses the inherited GetBitmap() -- the exact same capture primitive
*             UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea() and ProgressBackdrop_Capture() themselves use -- so
*             this is GEN's own real capture machinery, not a new one, and never a synthetic/flat fill colour.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Form/menu element this capture belongs to (stored only as an opaque identity key).
* @param[in]  x       : Left edge of the region to capture, in canvas coordinates.
* @param[in]  y       : Top edge of the region to capture, in canvas coordinates.
* @param[in]  width   : Width of the region to capture.
* @param[in]  height  : Height of the region to capture.
*
* @return     bool : true if the capture was stored; false if the bitmap grab or allocation failed (caller simply
*                     has no cached backdrop this tick and behaves as before this fix -- never worse).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::FormBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);

  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return formbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::FormBackdrop_MatchesArea(GRP2DREBUILDAREA* formbackdrop, double x, double y, double width, double height)
* @brief      ACCENT-BAR TRAIL FIX (2026-09): root cause of "quedan rastros de la barra azul en la opcion
*             previamente seleccionada" -- confirmed live (temporary BARTRAIL trace, since removed) on
*             UI_System's "nav-<section>-hl"/"nav-<section>-bar", two DIFFERENT contributing issues:
*             (1) The FormBackdrop captured the very first time either element is ever drawn can be
*             geometrically SMALLER than the box a later, ownarea-based redraw actually uses -- confirmed live:
*             "nav-resumen-hl"'s very first capture measured 211x54px, while a later redraw's own rebuild area
*             measured 221x59px (ownarea's box already includes PreDrawFunction()'s standard "edge" padding
*             margin, 5px on every side by default, which the very first capture had not yet reflected).
*             Draw_Form()'s restore path used to blindly trust the cached box forever, with no size check at all
*             -- unlike Draw_Text(), which has had exactly this protection (TextBackdrop_MatchesArea(), the
*             RESIZE-RECAPTURE FIX) since an earlier revision. This function closes that gap the same way.
*             (2) A DEEPER, separate cause for the specific case of a section selected by DEFAULT at application
*             startup (e.g. "Resumen"): its first-ever real fill-paint can happen on an early tick where this
*             element does not yet own a rebuild area at all (a normal, intentional case elsewhere -- see the
*             "ownarea-or-vr_*" fallback at this function's call site in Draw_Form()) -- before this fix, THAT
*             tick's paint ran with NO backdrop protection whatsoever, so by the time this element finally owned
*             an area and reached "first-ever capture", its own selection-wash colour was already on screen and
*             got captured as if pristine. Confirmed live: the residual patch's colour measured exactly
*             "nav-band"'s CSS colour blended ONCE over the true panel background, not a mismatched-geometry
*             leftover but this element's own wash baked permanently into its "forever" cache. Taking plain
*             doubles here (rather than a second GRP2DREBUILDAREA*) is what lets the same size/position check
*             cover both the ownarea-based box AND the vr_*-based fallback box uniformly.
* @note       Purely geometric, exactly like TextBackdrop_MatchesArea(): compares position and cached-bitmap
*             size against the CURRENT tick's target box, never by element name or type, so it protects any
*             current or future translucent form/menu whose first-ever capture happens to be taken at a
*             different size (or on a different ownarea-vs-fallback footing) than its later redraws, not only
*             today's nav-<section>-hl/-bar pair.
* @ingroup    USERINTERFACE
*
* @param[in]  formbackdrop : The previously cached entry (never NULL when called from Draw_Form()).
* @param[in]  x            : Left edge of this tick's target backdrop box, in canvas coordinates.
* @param[in]  y            : Top edge of this tick's target backdrop box.
* @param[in]  width        : Width of this tick's target backdrop box.
* @param[in]  height       : Height of this tick's target backdrop box.
*
* @return     bool : true if position AND bitmap size still match (safe to restore-and-reuse); false if they
*                     differ (caller must restore the OLD box first, then discard and re-capture at the new size).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::FormBackdrop_MatchesArea(GRP2DREBUILDAREA* formbackdrop, double x, double y, double width, double height)
{
  if(!formbackdrop) return false;

  GRPBITMAP* cachedbitmap = formbackdrop->GetBitmap();
  if(!cachedbitmap) return false;

  if(formbackdrop->GetXPos() != x) return false;
  if(formbackdrop->GetYPos() != y) return false;

  if((double)cachedbitmap->GetWidth()  != width)  return false;
  if((double)cachedbitmap->GetHeight() != height) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::FormBackdrop_Delete(UI_ELEMENT* element)
* @brief      Discards the cached FormBackdrop entry for a given element, if any -- used by the ACCENT-BAR TRAIL
*             FIX (see FormBackdrop_MatchesArea()) right before re-capturing at the correct, current size.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Form/menu element whose cached entry should be discarded.
*
* @return     bool : true if an entry was found and discarded; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::FormBackdrop_Delete(UI_ELEMENT* element)
{
  if(!element) return false;

  for(XDWORD c=0; c<formbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = formbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element))
        {
          formbackdrops.Delete(entry);
          GEN_DELETE entry;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::FormBackdrop_FindOverlapping(double x, double y, double width, double height)
* @brief      Returns the first cached FormBackdrop entry whose box overlaps the given rectangle (NULL if none).
*             SIBLING-CONTAMINATION FIX (2026-09): root cause of "quedan rastros de la barra azul en la opcion
*             previamente seleccionada" (persisted after the earlier ACCENT-BAR TRAIL FIX attempts in Draw_Form()).
*             Confirmed live via a raw-pixel dump (temporary trace, since removed) taken at the EXACT moment
*             "nav-resumen-bar" captures its own "true backdrop": the top and bottom rows of that capture read
*             the plain panel background, but the MIDDLE rows already read "nav-band"'s selection-wash colour --
*             a rounded-corner shape (clean at its extreme top/bottom rows, filled in the middle) that could only
*             belong to an element ALREADY painted by that instant. "nav-resumen-hl" and "nav-resumen-bar" are
*             independent root-level siblings (dashboard.xml's own "Z-ORDER" comment), drawn back-to-back in the
*             same frame: "-hl" draws first and, being itself haspaintable, paints its OWN selection-wash fill
*             (steps 4-8 further down in Draw_Form()) as part of that SAME Draw_Form() call -- before control ever
*             returns to draw its sibling "-bar". By the time "-bar" reaches ITS OWN first-ever capture a fraction
*             of a millisecond later, "-hl"'s fresh wash is already sitting in their shared region, and "-bar"
*             bakes it in as if it were the pristine backdrop -- permanently, since a FormBackdrop, unlike an
*             OptionBackdrop, is never re-captured. The existing "FIRST-CAPTURE CONTAMINATION FIX" in Draw_Form()
*             already guards against exactly this shape of bug for an overlapping OPTION-family sibling
*             (OptionBackdrop_FindOverlapping()) but had no equivalent for an overlapping FORM-family sibling --
*             this function closes that gap; see its call site for how it is used.
* @note       Same linear-scan, small-cardinality reasoning as every other *_Find() in this file.
* @ingroup    USERINTERFACE
*
* @param[in]  x      : Left edge of the region to check, in canvas coordinates.
* @param[in]  y      : Top edge of the region to check.
* @param[in]  width  : Width of the region to check.
* @param[in]  height : Height of the region to check.
*
* @return     GRP2DREBUILDAREA* : The first overlapping cached entry found, or NULL if none overlap.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::FormBackdrop_FindOverlapping(double x, double y, double width, double height)
{
  double left1  = x;
  double top1   = y;
  double right1 = x + width;
  double bottom1 = y + height;

  for(XDWORD c=0; c<formbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = formbackdrops.Get(c);
      if(!entry || !entry->GetBitmap()) continue;

      double left2   = entry->GetXPos();
      double top2    = entry->GetYPos();
      double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
      double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

      if((left1 < right2) && (right1 > left2) && (top1 < bottom2) && (bottom1 > top2)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::RadialBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent "true backdrop" snapshot previously captured for a progressradial widget's
*             own box (see the RADIAL CAPTION GHOSTING FIX comment in Draw_ProgressRadial() and the
*             radialbackdrops member comment in UI_SkinCanvas.h).
* @note       Linear scan is deliberate: same small-cardinality reasoning as ProgressBackdrop_Find()/
*             FormBackdrop_Find() above -- a typical layout has only a handful of radial progress widgets.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Progressradial element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     GRP2DREBUILDAREA* : The cached entry (xpos/ypos/bitmap already positioned for PutBitmapNoAlpha);
*                                  NULL if this element has never been captured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::RadialBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<radialbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = radialbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::RadialBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Captures the CURRENT on-screen pixels under a progressradial element's own box and keeps them
*             indefinitely as that element's "true backdrop" reference.
* @note       Only ever correct to call the FIRST time a given element is about to be drawn (see the caller in
*             Draw_ProgressRadial(), gated on RadialBackdrop_Find() returning NULL): at that point nothing has
*             painted this widget's own track/arc/caps/caption in this zone yet, so whatever is on screen right
*             now genuinely IS the backdrop. Reuses the inherited GetBitmap() -- the exact same capture primitive
*             UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea(), ProgressBackdrop_Capture() and FormBackdrop_Capture()
*             themselves use -- so this is GEN's own real capture machinery, not a new one, and never a
*             synthetic/flat fill colour.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Progressradial element this capture belongs to (stored only as an opaque identity key).
* @param[in]  x       : Left edge of the region to capture, in canvas coordinates.
* @param[in]  y       : Top edge of the region to capture, in canvas coordinates.
* @param[in]  width   : Width of the region to capture.
* @param[in]  height  : Height of the region to capture.
*
* @return     bool : true if the capture was stored; false if the bitmap grab or allocation failed (caller simply
*                     has no cached backdrop this tick and behaves as before this fix -- never worse).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::RadialBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);
  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return radialbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::TextBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent "true backdrop" snapshot previously captured for a text element's own box
*             (see the TEXT CAPTION GHOSTING FIX comment in Draw_Text() and the textbackdrops member comment in
*             UI_SkinCanvas.h).
* @note       Linear scan is deliberate: same small-cardinality reasoning as ProgressBackdrop_Find()/
*             FormBackdrop_Find()/RadialBackdrop_Find() above -- kept simple and consistent with them.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Text element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     GRP2DREBUILDAREA* : The cached entry (xpos/ypos/bitmap already positioned for PutBitmapNoAlpha);
*                                  NULL if this element has never been captured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::TextBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<textbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = textbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::TextBackdrop_MatchesArea(GRP2DREBUILDAREA* textbackdrop, GRP2DREBUILDAREA* ownarea)
* @brief      Checks whether a cached TextBackdrop entry still covers exactly the same screen rectangle as this
*             tick's rebuild area, i.e. whether it is still safe to restore-and-reuse (see the "extra wrinkle"
*             part of the TEXT CAPTION GHOSTING FIX comment in Draw_Text(): an auto-width text element's own box
*             can change size between two real redraws when its string content changes length).
* @ingroup    USERINTERFACE
*
* @param[in]  textbackdrop : The previously cached entry (never NULL when called from Draw_Text()).
* @param[in]  ownarea      : This tick's rebuild area for the same element (never NULL when called from Draw_Text()).
*
* @return     bool : true if position AND bitmap size still match (safe to restore); false if the element's box
*                     has moved or been resized since the capture was taken (caller must re-capture instead).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::TextBackdrop_MatchesArea(GRP2DREBUILDAREA* textbackdrop, GRP2DREBUILDAREA* ownarea)
{
  if(!textbackdrop || !ownarea) return false;

  GRPBITMAP* cachedbitmap = textbackdrop->GetBitmap();
  GRPBITMAP* areabitmap   = ownarea->GetBitmap();

  if(!cachedbitmap || !areabitmap) return false;

  // Epsilon: sub-pixel jitter from layout/scale must not force a recapture that can bake leftover ink.
  const double eps = 0.75;
  if(fabs(textbackdrop->GetXPos() - ownarea->GetXPos()) > eps) return false;
  if(fabs(textbackdrop->GetYPos() - ownarea->GetYPos()) > eps) return false;

  if(fabs((double)cachedbitmap->GetWidth()  - (double)areabitmap->GetWidth())  > eps) return false;
  if(fabs((double)cachedbitmap->GetHeight() - (double)areabitmap->GetHeight()) > eps) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::TextBackdrop_Delete(UI_ELEMENT* element)
* @brief      Discards (and frees) a stale TextBackdrop entry for "element", e.g. right before re-capturing it at
*             a new size (see TextBackdrop_MatchesArea() and the caller in Draw_Text()).
* @ingroup    USERINTERFACE
*
* @param[in]  element : Text element whose cached entry should be dropped (used only as an opaque identity key).
*
* @return     bool : true if a matching entry was found and removed; false if there was none to remove.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::TextBackdrop_Delete(UI_ELEMENT* element)
{
  if(!element) return false;

  for(XDWORD c=0; c<textbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = textbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element))
        {
          textbackdrops.Delete(entry);
          GEN_DELETE entry;

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::TextBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Captures the CURRENT on-screen pixels under a text element's own box and keeps them as that
*             element's "true backdrop" reference until it is next discarded (element destroyed, or a size
*             change invalidates it -- see TextBackdrop_MatchesArea()).
* @note       Only ever correct to call when TextBackdrop_Find() (after the TextBackdrop_MatchesArea() check)
*             found no usable entry (see the caller in Draw_Text()): at that point nothing has painted THIS
*             tick's glyph ink in this zone yet, so whatever is on screen right now genuinely IS the backdrop.
*             Reuses the inherited GetBitmap() -- the exact same capture primitive
*             UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea(), ProgressBackdrop_Capture(), FormBackdrop_Capture()
*             and RadialBackdrop_Capture() themselves use -- so this is GEN's own real capture machinery, not a
*             new one, and never a synthetic/flat fill colour.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Text element this capture belongs to (stored only as an opaque identity key).
* @param[in]  x       : Left edge of the region to capture, in canvas coordinates.
* @param[in]  y       : Top edge of the region to capture, in canvas coordinates.
* @param[in]  width   : Width of the region to capture.
* @param[in]  height  : Height of the region to capture.
*
* @return     bool : true if the capture was stored; false if the bitmap grab or allocation failed (caller simply
*                     has no cached backdrop this tick and behaves as before this fix -- never worse).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::TextBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);
  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return textbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::OptionBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent "true backdrop" snapshot previously captured for an option-family element's
*             own box (see the HOVER-WASH GHOSTING FIX comment in Draw_Option() and the optionbackdrops member
*             comment in UI_SkinCanvas.h).
* @note       Linear scan is deliberate: same small-cardinality reasoning as FormBackdrop_Find() above -- a
*             typical layout has only a handful of buttons/checkboxes/options.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Option-family element to look up (used only as an opaque identity key, never dereferenced).
*
* @return     GRP2DREBUILDAREA* : The cached entry (xpos/ypos/bitmap already positioned for PutBitmapNoAlpha);
*                                  NULL if this element has never been captured yet.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::OptionBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<optionbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = optionbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::OptionBackdrop_Delete(UI_ELEMENT* element)
* @brief      Discards (and frees) this element's cached OptionBackdrop anchor, e.g. once it has finished a
*             redraw with nothing of its own left on screen (see the caller in Draw_Option() and the
*             optionbackdrops member comment in UI_SkinCanvas.h for why this cache -- unlike
*             formbackdrops/radialbackdrops/textbackdrops -- must NOT be kept forever).
* @ingroup    USERINTERFACE
*
* @param[in]  element : Option-family element whose cached entry should be dropped (used only as an opaque
*                        identity key).
*
* @return     bool : true if a matching entry was found and removed; false if there was none to remove.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::OptionBackdrop_Delete(UI_ELEMENT* element)
{
  if(!element) return false;

  for(XDWORD c=0; c<optionbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = optionbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element))
        {
          optionbackdrops.Delete(entry);
          GEN_DELETE entry;

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::OptionBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Captures the CURRENT on-screen pixels under an option-family element's own box and keeps them
*             indefinitely as that element's "true backdrop" reference.
* @note       Only ever correct to call the FIRST time a given element is about to be drawn (see the caller in
*             Draw_Option(), gated on OptionBackdrop_Find() returning NULL): at that point nothing has painted
*             this element's own background wash/selection ring/children ink here yet, so whatever is on screen
*             right now genuinely IS the backdrop. Reuses the inherited GetBitmap() -- the exact same capture
*             primitive UI_SKINCANVAS_REBUILDAREAS::CreateRebuildArea(), FormBackdrop_Capture() and
*             TextBackdrop_Capture() themselves use -- so this is GEN's own real capture machinery, not a new
*             one, and never a synthetic/flat fill colour.
* @ingroup    USERINTERFACE
*
* @param[in]  element : Option-family element this capture belongs to (stored only as an opaque identity key).
* @param[in]  x       : Left edge of the region to capture, in canvas coordinates.
* @param[in]  y       : Top edge of the region to capture, in canvas coordinates.
* @param[in]  width   : Width of the region to capture.
* @param[in]  height  : Height of the region to capture.
*
* @return     bool : true if the capture was stored; false if the bitmap grab or allocation failed (caller simply
*                     has no cached backdrop this tick and behaves as before this fix -- never worse).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::OptionBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);
  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return optionbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_SKINCANVAS::OptionBackdrop_InvalidateOverlapping(double x, double y, double width, double height, bool force)
* @brief      Discards every cached OptionBackdrop entry whose box overlaps the given rectangle. Called from
* @note       STUCK-PRESELECT-AFTER-LONG-HOVER FIX (2026-09): discarding an entry here is only safe when its
* @ingroup    USERINTERFACE
* 
* @param[in]  x : Left edge of the region about to be repainted, in canvas coordinates.
* @param[in]  y : Top edge of the region about to be repainted.
* @param[in]  width : Width of the region about to be repainted.
* @param[in]  height : Height of the region about to be repainted.
* @param[in]  force : 
* 
* @return     bool : true if at least one overlapping entry was found and discarded; false otherwise.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::OptionBackdrop_InvalidateOverlapping(double x, double y, double width, double height, bool force)
{
  bool  discardedany = false;
  double left1        = x;
  double top1          = y;
  double right1        = x + width;
  double bottom1        = y + height;

  // Walk backwards since Delete() below removes entries from the same vector we are iterating.
  XDWORD remaining = optionbackdrops.GetSize();
  while(remaining > 0)
    {
      remaining--;

      GRP2DREBUILDAREA* entry = optionbackdrops.Get(remaining);
      if(!entry || !entry->GetBitmap()) continue;

      // STUCK-PRESELECT-AFTER-LONG-HOVER FIX (2026-09), continued: the owner is "busy" (unsafe to discard) only
      // when it currently has actual ink of its own blended onto the canvas -- its CSS background wash has
      // non-zero alpha (DrawBackgroundColor() runs unconditionally in Draw_Option(), so this alone already
      // covers "nav-hit"'s own ":hover" wash), OR its selected/preselected RING is both enabled (visiblelimit)
      // AND currently the active one being shown. This intentionally mirrors Draw_Option()'s own
      // "optionoverlaypainted" resolution exactly (father-fallback included), NOT the raw IsSelected()/
      // IsPreSelect() flags alone.
      //
      // REGRESSION FIX (2026-09), same day: an earlier version of this guard treated IsSelected()/IsPreSelect()
      // as "busy" by themselves. IsSelected() stays true for as long as that row IS the selected section -- far
      // longer than any transient wash -- so that version permanently blocked Fix 2 ("MID-EPISODE STALENESS GAP
      // fix" above) from ever invalidating the JUST-SELECTED button's cache again. The result, confirmed live
      // from Abraham's own screen recording: clicking a row no longer showed ANY selection band/accent bar (Fix
      // 2 exists specifically to let that first band paint through), and the still-selected "Resumen" row's own
      // label read back with its stale, pre-selection (muted) colour restored on top of the freshly recoloured
      // text -- "el texto... tiene capturado el color del fondo" -- because nothing ever forced a clean
      // recapture again after that first click. Selected/preselected is a STATE, not "ink on screen right now";
      // only the ring's actual paint condition (state AND the visiblelimit bit that would draw it) means ink is
      // really there.
      UI_ELEMENT_OPTION* owner = dynamic_cast<UI_ELEMENT_OPTION*>((UI_ELEMENT*)entry->GetExtraData());
      if(owner && !force)
        {
          bool ownerbgpainted = owner->GetBackgroundColor() && (owner->GetBackgroundColor()->GetAlpha() != 0);

          bool ownerselected  = owner->IsSelected();
          bool ownerpreselect = owner->IsPreSelect();
          if(owner->GetFather() && (owner->GetFather()->GetType() != UI_ELEMENT_TYPE_MULTIOPTION))
            {
              if(!ownerselected)  ownerselected  = owner->GetFather()->IsSelected();
              if(!ownerpreselect) ownerpreselect = owner->GetFather()->IsPreSelect();
            }

          bool ownerringpainted = (ownerselected  && (owner->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_SELECT)) ||
                                   (!ownerselected && ownerpreselect && (owner->GetVisibleLimitType() & UI_ELEMENT_OPTION_VISIBLE_LIMIT_PRESELECT));

          if(ownerbgpainted || ownerringpainted) continue;
        }

      double left2   = entry->GetXPos();
      double top2    = entry->GetYPos();
      double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
      double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

      bool overlaps = (left1 < right2) && (right1 > left2) && (top1 < bottom2) && (bottom1 > top2);
      if(overlaps)
        {
          // Restore ONLY the overlap with the caller rect. A full-box PutBitmapNoAlpha of a 59×59
          // bubble OptionBackdrop from a 73×14 legend form re-seeded a gray plate in the strip
          // outside the form fill (Stop/Run caption after maximize/restore).
          UI_SkinCanvas_RestoreBackdropIntersection(this, entry, left1, top1, right1, bottom1);
          if(owner) owner->SetMustReDraw(true);

          optionbackdrops.Delete(entry);
          GEN_DELETE entry;
          discardedany = true;
        }
    }

  return discardedany;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::TextBackdrop_InvalidateOverlapping(double x, double y, double width, double height)
* @brief      STALE CAPTION BACKGROUND FIX (2026-09): root cause of "al seleccionar un objeto en el menu o en un
*             boton, el texto se queda con el fondo anterior" -- reproduced live on both UI_System's CSS-styled
*             "nav-<section>-text" sidebar labels and a second, non-CSS listbox example's selected-row caption, so
*             this is a generic Draw_Text()/textbackdrops bug, not anything UI_System-specific.
*
*             Draw_Text()'s "OWNERLESS-ANCESTOR GHOSTING FIX" branch (used by any text element with no rebuild
*             area of its own, e.g. "nav-<section>-text", whose father is a purely structural hit-target form)
*             restores its OWN last-captured backdrop first (to erase only its own glyph ink), THEN re-captures
*             "whatever is on screen now" as the fresh backdrop. That is only correct when nothing OTHER than
*             this element's own ink changed under its box since the previous capture. Confirmed live (temporary
*             TXTDIAG trace, since removed) that a mouse click breaks exactly that assumption: selecting
*             "Memoria" makes the sidebar's "nav-memoria-hl" selection-wash form -- a root-level sibling, not an
*             ancestor of the label at all -- paint brand-new ink UNDER the label earlier in the very same tick
*             (confirmed by trace ordering: hl/bar, then the row, then the label). When the label's own Draw_Text()
*             runs right after, it restores its OWN previous snapshot (captured on an earlier tick, before this
*             selection existed) directly on top of "nav-memoria-hl"'s freshly-painted wash -- overwriting the
*             correct new pixels with the stale ones -- and then "recaptures" that very same stale content it
*             just pasted back, baking the wrong background in as the new "true backdrop" before painting the
*             (correctly recoloured) glyph on top of it. The result is pixel-identical to what Abraham's marked
*             screenshots show: a dark, mismatched rectangle exactly the size of the caption, sitting on an
*             otherwise-correctly-lit selected row. The same staleness can equally affect Draw_Text()'s other,
*             "ownarea"-gated forever-cache branch (used when a text element DOES own a rebuild area, e.g. the
*             second example's listbox row captions), which never had any mechanism to notice a sibling painting
*             over it either -- both branches share this exact textbackdrops vector, so one fix here covers both.
*
*             Exactly the same class of bug already root-caused and fixed for optionbackdrops (see
*             OptionBackdrop_InvalidateOverlapping() above) and formbackdrops (see the ALPHA-DARKENING FIX): a
*             forever/reused "true backdrop" cache goes stale the instant something else paints new, genuinely
*             different ink under it, and nothing was telling this particular cache to notice. Fix: called from
*             Draw_Form(), right next to the pre-existing OptionBackdrop_InvalidateOverlapping() call, every time
*             a form is about to paint real ink of its own -- purely geometric (box overlap against whatever is
*             cached), never by element name or type, so it protects any current or future text/form pairing that
*             happens to share screen space, not only today's nav-<section>-hl/-bar/-text triple.
* @note       RESTORE-THEN-DISCARD (2026-10, chrome title CJK ghost): unlike the earlier "forget only" approach,
*             this MUST PutBitmapNoAlpha() the overlapping TextBackdrop before deleting it. Draw_Text paints
*             glyph ink by pure alpha-blend with no opaque clear of its own. A caller form that then paints a
*             translucent fill (e.g. defaultwindowschromes caption color="black,25") does NOT erase those
*             glyphs -- it only darkens them -- so simply forgetting the cache left CJK strokes on screen.
*             Confirmed live on UI_System: F12 Chinese → Spanish left "系统监控器" blended under the new
*             "Monitor del Sistema" in the custom chrome title. Restoring the cached (glyph-free) backdrop
*             first strips the old ink; the form's own fill then composites onto a clean region; Draw_Text's
*             next redraw takes the "no TextBackdrop" path and re-captures fresh pixels. When the caller
*             already restored a FormBackdrop covering the same box, this restore is idempotent (puts the
*             caption fill that sat under the glyphs, which the upcoming form fill will repaint anyway).
* @note       Unlike OptionBackdrop_InvalidateOverlapping(), there is no "busy owner" guard here: a text
*             element's cached backdrop is a passive background snapshot, never live ink of its own that could
*             legitimately still be mid-blend when a neighbour's box happens to overlap it, so there is no
*             equivalent "protect this instead" case to guard against.
* @ingroup    USERINTERFACE
*
* @param[in]  x      : Left edge of the region about to be repainted, in canvas coordinates.
* @param[in]  y      : Top edge of the region about to be repainted.
* @param[in]  width  : Width of the region about to be repainted.
* @param[in]  height : Height of the region about to be repainted.
* @param[in]  source : Form/menu about to paint (optional). When set, only invalidate TextBackdrops whose
*                      owner lives in the SAME UI_LAYOUT — chrome caption AABBs must not discard content
*                      title TextBackdrops (UI_Options: hover near the top stacked "GEN XML Gallery").
*
* @return     bool : true if at least one overlapping entry was found and discarded; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::TextBackdrop_InvalidateOverlapping(double x, double y, double width, double height, UI_ELEMENT* source)
{
  bool  discardedany = false;
  double left1        = x;
  double top1          = y;
  double right1        = x + width;
  double bottom1        = y + height;
  UI_LAYOUT* source_layout = source ? source->GetLayout() : NULL;

  // Walk backwards since Delete() below removes entries from the same vector we are iterating.
  XDWORD remaining = textbackdrops.GetSize();
  while(remaining > 0)
    {
      remaining--;

      GRP2DREBUILDAREA* entry = textbackdrops.Get(remaining);
      if(!entry || !entry->GetBitmap()) continue;

      UI_ELEMENT* text_owner = (UI_ELEMENT*)entry->GetExtraData();
      if(source_layout && text_owner && (text_owner->GetLayout() != source_layout)) continue;

      double left2   = entry->GetXPos();
      double top2    = entry->GetYPos();
      double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
      double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

      bool overlaps = (left1 < right2) && (right1 > left2) && (top1 < bottom2) && (bottom1 > top2);
      if(overlaps)
        {
          // Compact legend captions (form father height<=32): do NOT restore — the cache is often
          // the gray peel plate. Drop and dirty; Draw_Text page-fills the father before reprinting.
          bool legend_caption = false;
          for(UI_ELEMENT* walk = text_owner; walk; walk = walk->GetFather())
            {
              if(walk->GetType() != UI_ELEMENT_TYPE_FORM) continue;
              if(walk->GetBoundaryLine() && walk->GetBoundaryLine()->height <= 32.0)
                {
                  legend_caption = true;
                  break;
                }
              break; // stop at first form even if taller
            }

          if(!legend_caption)
            {
              // FULL restore (not intersection-only). Section titles that sit just above a frame
              // (UI_Options lbl_nav / ic_nav above frame_nav) only graze the form's padded ownarea.
              // Intersection restore left most glyphs on screen; discarding the cache then forced
              // Draw_Text to re-capture those leftover strokes and alpha-blend again → progressive
              // blackening on every hover of the menu below. Full PutBitmapNoAlpha clears all glyph
              // ink before the owner redraws. (Intersection remains elsewhere for OptionBackdrop
              // peels that must not repaint a whole bubble into a legend strip.)
              PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());
            }

          if(text_owner) text_owner->SetMustReDraw(true);

          textbackdrops.Delete(entry);
          GEN_DELETE entry;
          discardedany = true;
        }
    }

  return discardedany;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::AnimBackdrop_Find(UI_ELEMENT* element)
* @brief      Look up the persistent backdrop snapshot for an animation element (see Draw_Animation ANIM ICON GHOSTING FIX).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::AnimBackdrop_Find(UI_ELEMENT* element)
{
  if(!element) return NULL;

  for(XDWORD c=0; c<animbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = animbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element)) return entry;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::AnimBackdrop_MatchesArea(GRP2DREBUILDAREA* animbackdrop, GRP2DREBUILDAREA* ownarea)
* @brief      Geometry match check (same epsilon policy as TextBackdrop_MatchesArea).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::AnimBackdrop_MatchesArea(GRP2DREBUILDAREA* animbackdrop, GRP2DREBUILDAREA* ownarea)
{
  if(!animbackdrop || !ownarea) return false;

  GRPBITMAP* cachedbitmap = animbackdrop->GetBitmap();
  GRPBITMAP* areabitmap   = ownarea->GetBitmap();

  if(!cachedbitmap || !areabitmap) return false;

  const double eps = 0.75;
  if(fabs(animbackdrop->GetXPos() - ownarea->GetXPos()) > eps) return false;
  if(fabs(animbackdrop->GetYPos() - ownarea->GetYPos()) > eps) return false;

  if(fabs((double)cachedbitmap->GetWidth()  - (double)areabitmap->GetWidth())  > eps) return false;
  if(fabs((double)cachedbitmap->GetHeight() - (double)areabitmap->GetHeight()) > eps) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::AnimBackdrop_Delete(UI_ELEMENT* element)
* @brief      Discard AnimBackdrop without restoring (caller already restored or is re-capturing).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::AnimBackdrop_Delete(UI_ELEMENT* element)
{
  if(!element) return false;

  for(XDWORD c=0; c<animbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = animbackdrops.Get(c);
      if(entry && (entry->GetExtraData() == (void*)element))
        {
          animbackdrops.Delete(entry);
          GEN_DELETE entry;

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::AnimBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
* @brief      Capture true backdrop under an animation before PutBitmapAlpha.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::AnimBackdrop_Capture(UI_ELEMENT* element, double x, double y, double width, double height)
{
  if(!element) return false;

  GRPBITMAP* bitmap = GetBitmap(x, y, width, height);
  if(!bitmap) return false;

  GRP2DREBUILDAREA* entry = GEN_NEW GRP2DREBUILDAREA();
  if(!entry)
    {
      GEN_DELETE bitmap;
      return false;
    }

  entry->SetXPos(x);
  entry->SetYPos(y);
  entry->SetBitmap(bitmap);
  entry->SetExtraData((void*)element);

  return animbackdrops.Add(entry);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_SKINCANVAS::AnimBackdrop_InvalidateOverlapping(double x, double y, double width, double height, UI_ELEMENT* source)
* @brief      Restore-then-discard AnimBackdrops overlapping a region about to be repainted (mirrors TextBackdrop).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_SKINCANVAS::AnimBackdrop_InvalidateOverlapping(double x, double y, double width, double height, UI_ELEMENT* source)
{
  bool  discardedany = false;
  double left1        = x;
  double top1          = y;
  double right1        = x + width;
  double bottom1        = y + height;
  UI_LAYOUT* source_layout = source ? source->GetLayout() : NULL;

  XDWORD remaining = animbackdrops.GetSize();
  while(remaining > 0)
    {
      remaining--;

      GRP2DREBUILDAREA* entry = animbackdrops.Get(remaining);
      if(!entry || !entry->GetBitmap()) continue;

      UI_ELEMENT* anim_owner = (UI_ELEMENT*)entry->GetExtraData();
      if(source_layout && anim_owner && (anim_owner->GetLayout() != source_layout)) continue;

      double left2   = entry->GetXPos();
      double top2    = entry->GetYPos();
      double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
      double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

      bool overlaps = (left1 < right2) && (right1 > left2) && (top1 < bottom2) && (bottom1 > top2);
      if(overlaps)
        {
          // FULL restore — same rationale as TextBackdrop_InvalidateOverlapping: edge-overlap from a
          // neighbour form's padded rebuild area (UI_Options ic_nav just above frame_nav) must erase
          // the whole soft-alpha icon before discard. Intersection-only left the bulk of the PNG on
          // screen; the next Draw_Animation re-captured it and PutBitmapAlpha stacked → progressive
          // blackening while hovering the nav/gal menu. Chrome role=icon no longer keeps an
          // AnimBackdrop (Draw_Animation deletes it), so the old intersection carve-out is obsolete.
          PutBitmapNoAlpha(entry->GetXPos(), entry->GetYPos(), entry->GetBitmap());

          if(anim_owner) anim_owner->SetMustReDraw(true);

          animbackdrops.Delete(entry);
          GEN_DELETE entry;
          discardedany = true;
        }
    }

  return discardedany;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRP2DREBUILDAREA* UI_SKINCANVAS::OptionBackdrop_FindOverlapping(double x, double y, double width, double height)
* @brief      Returns the first cached OptionBackdrop entry whose box overlaps the given rectangle (NULL if none).
*             Unlike OptionBackdrop_InvalidateOverlapping() (which discards matches), this only looks: used by
*             Draw_Form()'s first-ever FormBackdrop capture to find a sibling option-family element's own
*             already-clean cached backdrop, so a transient wash painted by that sibling can be stripped back out
*             before this element bakes in a "true backdrop" snapshot forever -- see the "FIRST-CAPTURE
*             CONTAMINATION FIX" comment at that call site.
* @note       Same linear-scan, small-cardinality reasoning as every other *_Find() in this file.
* @ingroup    USERINTERFACE
*
* @param[in]  x      : Left edge of the region to check, in canvas coordinates.
* @param[in]  y      : Top edge of the region to check.
* @param[in]  width  : Width of the region to check.
* @param[in]  height : Height of the region to check.
*
* @return     GRP2DREBUILDAREA* : The first overlapping cached entry found, or NULL if none overlap.
*
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DREBUILDAREA* UI_SKINCANVAS::OptionBackdrop_FindOverlapping(double x, double y, double width, double height)
{
  double left1  = x;
  double top1   = y;
  double right1 = x + width;
  double bottom1 = y + height;

  for(XDWORD c=0; c<optionbackdrops.GetSize(); c++)
    {
      GRP2DREBUILDAREA* entry = optionbackdrops.Get(c);
      if(!entry || !entry->GetBitmap()) continue;

      double left2   = entry->GetXPos();
      double top2    = entry->GetYPos();
      double right2  = left2 + (double)entry->GetBitmap()->GetWidth();
      double bottom2 = top2  + (double)entry->GetBitmap()->GetHeight();

      if((left1 < right2) && (right1 > left2) && (top1 < bottom2) && (bottom1 > top2)) return entry;
    }

  return NULL;
}


