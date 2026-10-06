/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_CSSParser.cpp
*
* @class      UI_CSSPARSER
* @brief      User Interface : minimal CSS parser (text -> UI_STYLESHEET), scoped to GEN's selector subset.
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

#include "UI_CSSParser.h"

#include "XFileTXT.h"
#include "XFactory.h"
#include "XTrace.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSPARSER::UI_CSSPARSER()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSPARSER::UI_CSSPARSER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSPARSER::~UI_CSSPARSER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSPARSER::~UI_CSSPARSER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ParseFile(XPATH& pathfile, UI_STYLESHEET& out)
* @brief      Read a .css file from disk and parse it into `out`.
* @note       Step 5 ("una sola hoja por layout"): this is also the entry point @@import recurses through, so
*             it (a) guards against cycles -- a file already open somewhere up this call's @@import chain is
*             skipped instead of recursed into again -- and (b) tracks `currentfiledir` (this file's own
*             directory) around the ParseText() call, restoring the caller's value afterwards, so a NESTED
*             @@import inside an imported file resolves against ITS OWN directory rather than the top-level
*             layout's. Both only matter to ResolveAndParseImport(); a normal, import-free stylesheet behaves
*             exactly as before.
* @ingroup    USERINTERFACE
*
* @param[in]  pathfile : Stylesheet file to load.
* @param[out] out : Target stylesheet; parsed rules are appended.
*
* @return     bool : true if the file was opened and read; false if it could not be opened, or if it is
*             already open along the current @@import chain (cycle).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ParseFile(XPATH& pathfile, UI_STYLESHEET& out)
{
  for(XDWORD c=0; c<importstack.GetSize(); c++)
    {
      XPATH* openfile = importstack.Get(c);
      if(openfile && (*openfile) == pathfile)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] @import cycle detected: [%s] is already open, skipping"), pathfile.Get());
          return false;
        }
    }

  XFILETXT* file = GEN_NEW XFILETXT();
  if(!file) return false;

  bool status = false;

  if(file->Open(pathfile, true))
    {
      if(file->ReadAllFile())
        {
          // Phase 2 ("diagnóstico con línea real de fichero"): preserve a plain '\n' between source lines
          // instead of the previous no-separator flatten (GetAllInOneLine()'s default, XFILETXTTYPELF_UNKNOWN).
          // Every scanning loop in this parser already treats '\r'/'\n' as ordinary skippable whitespace
          // (SkipWhitespaceAndComments(), the trim passes in ParseCompoundSelector()/ReadDeclarationBlock()), so
          // this changes nothing about what parses -- it only means an offset into `alltext` can now be turned
          // back into a real (line, column) pair via ResolveLineColumn() instead of being a meaningless index
          // into a one-line soup.
          XSTRING alltext;
          file->GetAllInOneLine(alltext, XFILETXTTYPELF_0A);

          XPATH* marker = GEN_NEW XPATH(pathfile);
          if(marker) importstack.Add(marker);

          XPATH   savedcurrentfiledir = currentfiledir;
          XSTRING drive;
          XSTRING dir;

          pathfile.GetDrive(drive);
          pathfile.GetPath(dir);

          currentfiledir  = drive;
          currentfiledir += dir;
          currentfilepath = pathfile;

          status = ParseText(alltext, out);

          currentfiledir = savedcurrentfiledir;
          currentfilepath.Empty();

          if(marker)
            {
              importstack.DeleteLast();
              GEN_DELETE marker;
            }
        }

      file->Close();
    }

  GEN_DELETE file;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ParseText(XSTRING& text, UI_STYLESHEET& out)
* @brief      Parse an in-memory CSS text buffer into `out`. Rules with syntax errors are skipped via
*             SkipToNextRule() and do not abort the parse of the remaining input.
* @note       Step 11: every discard (malformed rule, empty rule, malformed declaration -- see
*             ReadDeclarationBlock()) is reported through XTRACE_PRINTCOLOR(). This costs nothing when GEN is
*             built without XTRACE_ACTIVE (the macro compiles to a no-op, see XTrace.h) and is otherwise
*             filtered exactly like every other GEN trace, through the app's own ini ([log] activesectionsID /
*             levelmask) or target selection -- no separate on/off switch was added for this.
* @ingroup    USERINTERFACE
*
* @param[in]  text : CSS source text.
* @param[out] out : Target stylesheet; parsed rules are appended.
*
* @return     bool : true always (a valid empty text is a valid stylesheet); reserved for future hard errors.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ParseText(XSTRING& text, UI_STYLESHEET& out)
{
  int pos        = 0;
  int len        = (int)text.GetSize();
  int ruleindex  = 0;   // 1-based, counts every rule ATTEMPT (kept or discarded) -- Step 11 debug trace only.
  int rules_kept = 0;

  discarded_rules        = 0;
  unterminated_comments  = 0;
  last_rules_kept        = 0;

  while(pos < len)
    {
      SkipWhitespaceAndComments(text, pos);
      if(pos >= len) break;

      if(!ParseOneRuleOrAtRule(text, pos, out, NULL, NULL, ruleindex, rules_kept))
        {
          return false;
        }
    }

  last_rules_kept = rules_kept;

  // Resolve variable references now -- but ONLY if this ParseText() call is the outermost one, i.e. not itself
  // running because ResolveAndParseImport() recursed into ParseFile() for an "@import"ed file (importdepth > 0
  // in that case). Phase 2 fix for the residual variable-merge-order case: expanding here unconditionally, as
  // before, meant an IMPORTED file's var() references got substituted (and, on a miss, permanently replaced by
  // their fallback or by nothing at all -- SubstituteVars() always consumes the token) using whatever partial
  // variable table existed at that point in the recursion, which is BEFORE the importING file has had a chance
  // to parse its own ":root" block if that block is written after the "@import" line -- exactly the common
  // "@import theme first, override one variable after it" pattern. Deferring expansion to the single outermost
  // call means it always runs after every import in the chain (and the importer's own full ":root") has already
  // contributed to `out.Variables()`, so overrides declared after the import line are honoured, matching real
  // CSS "as if written in place" semantics. A ParseText() call made directly (no ParseFile() involved at all,
  // e.g. by a test or any future direct caller) has importdepth == 0 by construction and keeps expanding
  // immediately, exactly as before.
  if(importdepth == 0) out.ExpandVariables();

  // Phase 0 hygiene: make the "comment */ killed the rest of the file" failure mode impossible to miss.
  if(importdepth == 0)
    {
      if(unterminated_comments > 0)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[CSS Parse] ERROR: %d unterminated comment(s)%s%s -- rest of stylesheet may be truncated"), unterminated_comments, currentfilepath.IsEmpty() ? _L("") : _L(" in "), currentfilepath.IsEmpty() ? _L("") : currentfilepath.Get());
        }

      if(discarded_rules > 0)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] summary: kept %d rule(s), discarded %d%s%s"), rules_kept, discarded_rules, currentfilepath.IsEmpty() ? _L("") : _L(" in "), currentfilepath.IsEmpty() ? _L("") : currentfilepath.Get());
        }

      if((len > 0) && (rules_kept == 0) && (discarded_rules > 0))
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[CSS Parse] ERROR: non-empty CSS produced ZERO kept rules (%d discarded)%s%s -- check for an early '*/' inside a block comment (prose with asterisk+slash). UI may paint fully transparent."), discarded_rules, currentfilepath.IsEmpty() ? _L("") : _L(" in "), currentfilepath.IsEmpty() ? _L("") : currentfilepath.Get());
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ParseInlineDeclarations(XSTRING& text, UI_STYLE& out)
* @brief      Step 6 ("sin overrides puntuales por elemento"): parse a bare "prop: value; prop: value" list --
*             no selector, no surrounding "{ }" of its own -- into `out`. This is what an XML "style=" attribute
*             looks like; wrapping it in a synthetic "{ ... }" lets ReadDeclarationBlock() parse it unchanged,
*             so it accepts exactly the same grammar (comments, whitespace, trimming) as a real rule body.
* @ingroup    USERINTERFACE
*
* @param[in]  text : Raw declaration-list text, e.g. "color: red; padding: 4".
* @param[out] out : Style bag the declarations are written into.
*
* @return     bool : true if the block parsed to completion; false on malformed input (missing/mismatched
*             braces are impossible here since they are synthetic, so this mirrors ReadDeclarationBlock()'s own
*             EOF-before-'}' failure, which cannot happen with a well-formed wrapper -- kept for symmetry and
*             to surface a NULL member misuse defensively).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ParseInlineDeclarations(XSTRING& text, UI_STYLE& out)
{
  XSTRING wrapped = _L("{");
  wrapped += text;
  wrapped += _L("}");

  int pos = 0;
  return ReadDeclarationBlock(wrapped, pos, out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSPARSER::ResolveLineColumn(XSTRING& text, int offset, int& outline, int& outcolumn)
* @brief      Phase 2 ("diagnóstico con línea real de fichero"): turn a raw character offset into `text` into a
*             1-based (line, column-within-line) pair, by counting '\n' occurrences up to `offset`. Relies on
*             `text` actually containing real line separators -- true for anything ParseFile() flattened (see
*             its GetAllInOneLine() call), and harmlessly degenerates to "line 1" for a caller-built buffer with
*             no embedded '\n' at all (e.g. ParseInlineDeclarations()'s synthetic wrapper), which is correct: a
*             single-line buffer genuinely has no line 2 to report.
* @ingroup    USERINTERFACE
*
* @param[in]  text : Source text the offset was taken from.
* @param[in]  offset : Raw character offset, clamped into [0, text length] before use.
* @param[out] outline : 1-based line number.
* @param[out] outcolumn : 1-based column within that line.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSPARSER::ResolveLineColumn(XSTRING& text, int offset, int& outline, int& outcolumn)
{
  int len = (int)text.GetSize();

  if(offset < 0)   offset = 0;
  if(offset > len) offset = len;

  int line      = 1;
  int linestart = 0;

  for(int c=0; c<offset; c++)
    {
      if(text[c] == _C('\n'))
        {
          line++;
          linestart = c + 1;
        }
    }

  outline   = line;
  outcolumn = (offset - linestart) + 1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSPARSER::SkipWhitespaceAndComments(XSTRING& text, int& pos)
* @brief      Advance `pos` over runs of whitespace and C-style comments ("/ * ... * /"), including nested
*             adjacent runs. Unterminated comments consume up to end-of-text.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSPARSER::SkipWhitespaceAndComments(XSTRING& text, int& pos)
{
  int len = (int)text.GetSize();

  while(pos < len)
    {
      XCHAR ch = text[pos];

      if(ch == _C(' ') || ch == _C('\t') || ch == _C('\r') || ch == _C('\n'))
        {
          pos++;
          continue;
        }

      if(ch == _C('/') && (pos + 1) < len && text[pos + 1] == _C('*'))
        {
          int commentstart = pos;
          pos += 2;
          bool closed = false;
          while(pos < len)
            {
              if(text[pos] == _C('*') && (pos + 1) < len && text[pos + 1] == _C('/'))
                {
                  pos += 2;
                  closed = true;
                  break;
                }
              pos++;
            }
          if(!closed)
            {
              // Unterminated comment: consume to EOF (legacy behaviour) but surface a hard ERROR so authors
              // notice -- this is the same class of silent truncation as an early */ inside a long header.
              unterminated_comments++;
              int line, col;
              ResolveLineColumn(text, commentstart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[CSS Parse] ERROR: unterminated comment starting at line %d, column %d%s%s"), line, col, currentfilepath.IsEmpty() ? _L("") : _L(" in "), currentfilepath.IsEmpty() ? _L("") : currentfilepath.Get());
            }
          continue;
        }

      break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ReadIdentifier(XSTRING& text, int& pos, XSTRING& outident)
* @brief      Phase 2 ("lexer con tokens/strings/escapes reales"): read one identifier-like token starting at
*             `pos` -- a selector's type/id/class/pseudo fragment. Consumes everything up to (but not including)
*             an unescaped '#', '.' or ':', or end-of-text. "\X" decodes to the literal character X (dropping
*             the backslash) and never counts as a delimiter, even when X is itself '#'/'.'/':' -- this is what
*             lets e.g. ".foo\.bar" name a class literally containing a dot. See the class banner for why only
*             single-character escapes are supported.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text (typically a single already-bounded compound-selector token).
* @param[in,out] pos : Cursor; advanced past the consumed identifier.
* @param[out]    outident : Decoded identifier text; emptied first.
*
* @return     bool : true if at least one character was consumed; false if the identifier is empty (cursor
*             unchanged in that case, `pos` still sitting on the delimiter or at end-of-text).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadIdentifier(XSTRING& text, int& pos, XSTRING& outident)
{
  int len = (int)text.GetSize();

  outident.Empty();

  bool any = false;

  while(pos < len)
    {
      XCHAR ch = text[pos];

      if(ch == _C('\\'))
        {
          pos++;
          if(pos < len)
            {
              outident += text[pos];
              pos++;
              any = true;
            }
          continue;
        }

      if(ch == _C('#') || ch == _C('.') || ch == _C(':')) break;

      outident += ch;
      pos++;
      any = true;
    }

  return any;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ReadStringLiteral(XSTRING& text, int& pos, XSTRING& outstring)
* @brief      Phase 2 ("lexer con tokens/strings/escapes reales"): read one quoted string token ('...' or "...")
*             starting at `pos`, which MUST already be positioned on the opening quote. "\X" decodes to the
*             literal character X (dropping the backslash), including an escaped quote ("\"") that would
*             otherwise terminate the string early.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor; positioned on the opening quote on entry, advanced past the closing quote on
*                success. Left at end-of-text on failure (unterminated string).
* @param[out]    outstring : Decoded string contents (quotes stripped, escapes resolved); emptied first.
*
* @return     bool : true if a matching closing quote was found; false if `pos` was not on a quote, or the
*             string runs to end-of-text with no closing quote (unterminated -- a hard failure, same as before).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadStringLiteral(XSTRING& text, int& pos, XSTRING& outstring)
{
  int len = (int)text.GetSize();

  outstring.Empty();

  if(pos >= len) return false;

  XCHAR quote = text[pos];
  if(quote != _C('"') && quote != _C('\'')) return false;

  pos++;

  while(pos < len)
    {
      XCHAR ch = text[pos];

      if(ch == quote)
        {
          pos++;
          return true;
        }

      if(ch == _C('\\'))
        {
          pos++;
          if(pos < len)
            {
              outstring += text[pos];
              pos++;
            }
          continue;
        }

      outstring += ch;
      pos++;
    }

  return false;   // unterminated: pos left at end-of-text, outstring holds whatever was read so far (discarded by every caller on false)
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSPARSER::SkipStringLiteral(XSTRING& text, int& pos)
* @brief      Phase 2 ("lexer con tokens/strings/escapes reales"): same scan as ReadStringLiteral() but without
*             building the decoded text, for callers that only need to jump an intact quoted span (a string
*             embedded in a declaration value, or inside malformed content being resynced past) without caring
*             what it says -- so a ';'/'}'/','/'{' INSIDE the quotes is never mistaken for a real delimiter.
* @note       INTERNAL. A no-op if `pos` is not positioned on a quote character.
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor; positioned on the opening quote on entry, advanced past the closing quote (or to
*                end-of-text, if unterminated) on return.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSPARSER::SkipStringLiteral(XSTRING& text, int& pos)
{
  int len = (int)text.GetSize();

  if(pos >= len) return;

  XCHAR quote = text[pos];
  if(quote != _C('"') && quote != _C('\'')) return;

  pos++;

  while(pos < len)
    {
      XCHAR ch = text[pos];

      if(ch == quote) { pos++; return; }

      if(ch == _C('\\'))
        {
          pos++;
          if(pos < len) pos++;
          continue;
        }

      pos++;
    }

  // Unterminated: pos left at end-of-text, matching ReadStringLiteral()'s failure path.
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ReadSelectorList(XSTRING& text, int& pos, UI_CSSRULE* rule)
* @brief      Read the comma-separated selector list up to (but not including) '{'. Each compound selector is
*             parsed via ParseCompoundSelector() and added to the rule.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor.
* @param[in,out] rule : Rule to populate with selectors.
*
* @return     bool : true if at least the '{' terminator was found in-range; false on EOF before '{'.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadSelectorList(XSTRING& text, int& pos, UI_CSSRULE* rule)
{
  if(!rule) return false;

  int len   = (int)text.GetSize();
  int start = pos;

  while(pos < len)
    {
      XCHAR ch = text[pos];

      // Phase 2: a quoted span (not part of GEN's supported selector grammar, but defensive against a stray
      // ',' or '{' inside one) is skipped atomically so it can never be mistaken for a selector-list delimiter.
      if(ch == _C('"') || ch == _C('\''))
        {
          SkipStringLiteral(text, pos);
          continue;
        }

      if(ch == _C(',') || ch == _C('{'))
        {
          UI_CSSSELECTOR* sel = ParseCompoundSelectorSequence(text, start, pos);
          if(sel) rule->GetSelectors().Add(sel);

          if(ch == _C('{')) return true;

          pos++;
          SkipWhitespaceAndComments(text, pos);
          start = pos;
          continue;
        }

      pos++;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSSELECTOR* UI_CSSPARSER::ParseCompoundSelector(XSTRING& text, int start, int end)
* @brief      Parse one compound selector between [start, end): TypeSelector? ("#" Id)? ("." Class)*, or "*".
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Source text.
* @param[in]  start : Start offset (inclusive).
* @param[in]  end : End offset (exclusive).
*
* @return     UI_CSSSELECTOR* : Owned pointer, or NULL on empty/invalid token.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSSELECTOR* UI_CSSPARSER::ParseCompoundSelector(XSTRING& text, int start, int end)
{
  // Extract the token substring and trim surrounding whitespace.
  XSTRING token;
  text.Copy(start, end, token);

  int tlen = (int)token.GetSize();
  int i    = 0;

  while(i < tlen)
    {
      XCHAR ch = token[i];
      if(ch != _C(' ') && ch != _C('\t') && ch != _C('\r') && ch != _C('\n')) break;
      i++;
    }

  int j = tlen;
  while(j > i)
    {
      XCHAR ch = token[j - 1];
      if(ch != _C(' ') && ch != _C('\t') && ch != _C('\r') && ch != _C('\n')) break;
      j--;
    }

  if(j <= i) return NULL;

  UI_CSSSELECTOR* sel = GEN_NEW UI_CSSSELECTOR();
  if(!sel) return NULL;

  // Universal selector "*": no restrictions at all.
  if((j - i) == 1 && token[i] == _C('*'))
    {
      sel->RecomputeSpecificity();
      return sel;
    }

  // Phase 2 ("lexer con tokens/strings/escapes reales"): work over the trimmed [i, j) span as its own bounded
  // string so ReadIdentifier()'s cursor never wanders past it, then tokenize via ReadIdentifier() instead of
  // the previous hand-rolled "scan to the next '#'/'.'/':' " loop -- same delimiters, same "type is optional,
  // fragments in any order" grammar, now with backslash-escape support (".foo\.bar" is a class literally named
  // "foo.bar") shared with every other identifier read in this parser.
  XSTRING trimmed;
  token.Copy(i, j, trimmed);

  int tpos  = 0;
  int tsize = (int)trimmed.GetSize();

  // Type prefix (optional): identifier up to '#', '.' or ':'.
  XSTRING typestr;
  if(ReadIdentifier(trimmed, tpos, typestr))
    {
      sel->GetType().Set(typestr);
    }

  // Then a sequence of ("#" ident), ("." ident) and (":" ident) fragments, in any order.
  while(tpos < tsize)
    {
      XCHAR marker = trimmed[tpos];
      if(marker != _C('#') && marker != _C('.') && marker != _C(':'))
        {
          // Unknown character in selector: bail out but keep whatever we already parsed.
          break;
        }

      tpos++;

      XSTRING frag;
      if(ReadIdentifier(trimmed, tpos, frag))
        {
          if(marker == _C('#'))
            {
              // If two ids are supplied on the same compound selector, the last one wins: no meaningful
              // semantics for two ids, and this matches how most CSS engines silently handle it.
              sel->GetID().Set(frag);
            }
           else if(marker == _C('.'))
            {
              sel->AddClass(frag.Get());
            }
           else /* marker == ':' */
            {
              // Pseudo-class. ":root" is special-cased here at parse time: the parser lifts its declarations
              // into the stylesheet's variable table when the rule is finalized instead of storing it as a
              // regular pseudo. Every other pseudo-class (including the live state ones -- ":preselect"/
              // ":hover", ":selected", ":active", ":disabled" -- and any structural one GEN does not support,
              // e.g. ":first-child") is stored on the selector as authored, so it parses cleanly and
              // contributes 10 to specificity; whether it ever MATCHES an element is entirely Match()'s call
              // at Resolve()/ReapplyStyleVisual() time, driven by UI_ELEMENT::GetActivePseudos().
              sel->AddPseudo(frag.Get());
            }
        }
    }

  sel->RecomputeSpecificity();
  return sel;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_CSSSELECTOR* UI_CSSPARSER::ParseCompoundSelectorSequence(XSTRING& text, int start, int end)
* @brief      Phase 2 ("combinadores descendiente/hijo"): split [start, end) -- one comma-separated selector-list
*             entry -- on top-level combinators (whitespace = descendant, '>' = child) and build the resulting
*             UI_CSSSELECTOR: the RIGHTMOST span is parsed as the subject (via the unchanged
*             ParseCompoundSelector()), every earlier span becomes an ancestor step attached closest-first. A
*             single-span entry (the overwhelming common case, and every selector that existed before this
*             phase) is a pure pass-through to ParseCompoundSelector() -- identical object, identical behaviour.
* @note       INTERNAL. Quoted spans are skipped atomically (SkipStringLiteral()) while splitting, consistent
*             with every other scan in this parser -- not part of GEN's selector grammar, but defensive.
* @ingroup    USERINTERFACE
*
* @param[in]  text : Source text.
* @param[in]  start : Start offset (inclusive).
* @param[in]  end : End offset (exclusive).
*
* @return     UI_CSSSELECTOR* : Owned pointer (subject, with any earlier spans attached as ancestor steps), or
*             NULL if [start, end) contains no compound at all.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_CSSSELECTOR* UI_CSSPARSER::ParseCompoundSelectorSequence(XSTRING& text, int start, int end)
{
  XVECTOR<int>                    spanstarts;
  XVECTOR<int>                    spanends;
  XVECTOR<UI_CSSCOMBINATORTYPE>   combinatorafter;   // combinatorafter[c] ties span c to span c+1; last entry unused

  int pos = start;

  while(pos < end)
    {
      // Skip whitespace between compounds.
      while(pos < end && (text[pos]==_C(' ')||text[pos]==_C('\t')||text[pos]==_C('\r')||text[pos]==_C('\n'))) pos++;
      if(pos >= end) break;

      if(text[pos] == _C('>'))
        {
          // A '>' with no compound before it in this range (leading '>', or two combinators in a row): bail
          // out of splitting and keep whatever spans were already found -- same "unknown syntax: keep what we
          // already parsed" spirit as ParseCompoundSelector()'s own recovery.
          break;
        }

      int tokenstart = pos;

      while(pos < end)
        {
          XCHAR ch = text[pos];

          if(ch == _C('"') || ch == _C('\''))
            {
              SkipStringLiteral(text, pos);
              if(pos > end) pos = end;
              continue;
            }

          if(ch == _C('>')) break;
          if(ch == _C(' ') || ch == _C('\t') || ch == _C('\r') || ch == _C('\n')) break;

          pos++;
        }

      int tokenend = pos;
      if(tokenend <= tokenstart) break;

      spanstarts.Add(tokenstart);
      spanends.Add(tokenend);

      // Decide the combinator between THIS span and whatever comes next (meaningful only if another span
      // follows; harmless/never read otherwise).
      while(pos < end && (text[pos]==_C(' ')||text[pos]==_C('\t')||text[pos]==_C('\r')||text[pos]==_C('\n'))) pos++;

      UI_CSSCOMBINATORTYPE combinator = UI_CSSCOMBINATOR_DESCENDANT;

      if(pos < end && text[pos] == _C('>'))
        {
          combinator = UI_CSSCOMBINATOR_CHILD;
          pos++;
          while(pos < end && (text[pos]==_C(' ')||text[pos]==_C('\t')||text[pos]==_C('\r')||text[pos]==_C('\n'))) pos++;
        }

      combinatorafter.Add(combinator);
    }

  int spancount = (int)spanstarts.GetSize();
  if(spancount == 0) return NULL;

  // Fast path: no combinator at all -- pass straight through to ParseCompoundSelector() so a combinator-free
  // selector produces EXACTLY the object it always did (same [start,end) trimming behaviour and all), not a
  // one-ancestor-step-then-degenerate-subject rebuild of the same thing.
  if(spancount == 1)
    {
      return ParseCompoundSelector(text, spanstarts.Get(0), spanends.Get(0));
    }

  UI_CSSSELECTOR* subject = ParseCompoundSelector(text, spanstarts.Get(spancount - 1), spanends.Get(spancount - 1));
  if(!subject) return NULL;

  // Attach every earlier span as an ancestor step, closest-ancestor-first: span[spancount-2] is immediately to
  // the subject's left (step 0), span[spancount-3] is next (step 1), and so on down to span[0].
  for(int c=spancount-2; c>=0; c--)
    {
      UI_CSSSELECTOR* compound = ParseCompoundSelector(text, spanstarts.Get(c), spanends.Get(c));
      if(compound) subject->AddAncestorStep(compound, combinatorafter.Get(c));
    }

  // Ancestor compounds contribute to specificity too (see RecomputeSpecificity()'s own note) -- recompute now
  // that they're all attached.
  subject->RecomputeSpecificity();

  return subject;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ReadDeclarationBlock(XSTRING& text, int& pos, UI_STYLE& decls)
* @brief      Parse the "{ key : value ; ... }" block into `decls`.
* @note       INTERNAL. Takes the destination bag directly (rather than a UI_CSSRULE*) so it can populate either
*             a rule's own declarations (see ParseText()) or a standalone bag with no rule/selector around it
*             at all (see ParseInlineDeclarations(), Step 6).
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor; expected at '{' on entry, positioned right after '}' on success.
* @param[in,out] decls : Style bag to populate.
*
* @return     bool : true on success; false on EOF before '}'.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadDeclarationBlock(XSTRING& text, int& pos, UI_STYLE& decls)
{
  int len = (int)text.GetSize();

  if(pos >= len || text[pos] != _C('{')) return false;
  pos++;

  while(pos < len)
    {
      SkipWhitespaceAndComments(text, pos);
      if(pos >= len) return false;

      if(text[pos] == _C('}'))
        {
          pos++;
          return true;
        }

      // --- Key : identifier up to ':' -----------------------------------------------------------------------------
      // Phase 2: quoted spans are skipped atomically (SkipStringLiteral()) before ever testing a byte against
      // ':'/';'/'}' , so a delimiter INSIDE a quoted string can never be mistaken for the real one. Not expected
      // in a well-formed key, but keeps this scan consistent with the value scan just below.
      int keystart = pos;
      while(pos < len)
        {
          XCHAR ch = text[pos];

          if(ch == _C('"') || ch == _C('\''))
            {
              SkipStringLiteral(text, pos);
              continue;
            }

          if(ch == _C(':') || ch == _C(';') || ch == _C('}')) break;
          pos++;
        }

      if(pos >= len || text[pos] != _C(':'))
        {
          // Malformed declaration: skip to next ';' or '}' and try to keep parsing this block. Step 11: trace
          // it -- this is the one discard SkipToNextRule() never sees, since the enclosing rule is otherwise
          // fine and keeps being parsed. Phase 2: real (line, column), not a flat offset -- see ResolveLineColumn().
          int line, col;
          ResolveLineColumn(text, keystart, line, col);
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] declaration discarded at line %d, column %d: no ':' found before ';'/'}'"), line, col);

          while(pos < len && text[pos] != _C(';') && text[pos] != _C('}')) pos++;
          if(pos < len && text[pos] == _C(';')) pos++;
          continue;
        }

      XSTRING key;
      text.Copy(keystart, pos, key);
      key.DeleteNoCharacters(_L(" \t\r\n"));  // trim in-place

      pos++;  // past ':'

      // --- Value : everything up to ';' or '}' --------------------------------------------------------------------
      // Phase 2 ("lexer con tokens/strings/escapes reales"): the real correctness fix in this whole increment --
      // a quoted span is skipped atomically (SkipStringLiteral()) BEFORE its bytes are ever tested against
      // ';'/'}' , so a value like 'font-family: "A; B";' keeps its embedded semicolon instead of being cut short
      // at it. The value is still stored verbatim (quotes included) -- see the class banner's "Value semantics"
      // note; only where the value's END is found has changed, not what ends up in it.
      SkipWhitespaceAndComments(text, pos);
      int valstart = pos;
      while(pos < len)
        {
          XCHAR ch = text[pos];

          if(ch == _C('"') || ch == _C('\''))
            {
              SkipStringLiteral(text, pos);
              continue;
            }

          if(ch == _C(';') || ch == _C('}')) break;
          pos++;
        }

      int valend = pos;

      // Trim trailing whitespace from value without touching interior spaces (colors like "160,195,255" and
      // multi-token values like "0,0,175,30" must survive intact).
      while(valend > valstart)
        {
          XCHAR ch = text[valend - 1];
          if(ch != _C(' ') && ch != _C('\t') && ch != _C('\r') && ch != _C('\n')) break;
          valend--;
        }

      if(valend > valstart && !key.IsEmpty())
        {
          XSTRING value;
          text.Copy(valstart, valend, value);
          decls.Set(key.Get(), value);
        }

      if(pos < len && text[pos] == _C(';')) pos++;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSPARSER::SkipToNextRule(XSTRING& text, int& pos)
* @brief      Error-recovery: consume everything up to and including the next '}'.
* @note       INTERNAL. Phase 2 ("lexer con tokens/strings/escapes reales"): a quoted span inside the malformed
*             content being resynced past is now skipped atomically (SkipStringLiteral()), so a '}' INSIDE a
*             string literal can no longer be mistaken for the real rule terminator and end the resync early.
*             The recovery strategy itself -- resync on the next (now: unquoted) '}' -- is unchanged.
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSPARSER::SkipToNextRule(XSTRING& text, int& pos)
{
  int len   = (int)text.GetSize();
  int depth = 0;
  bool seen_brace = false;

  while(pos < len)
    {
      XCHAR ch = text[pos];

      if(ch == _C('"') || ch == _C('\''))
        {
          SkipStringLiteral(text, pos);
          continue;
        }

      if(ch == _C('{'))
        {
          depth++;
          seen_brace = true;
          pos++;
          continue;
        }

      if(ch == _C('}'))
        {
          pos++;
          if(!seen_brace) return;
          depth--;
          if(depth <= 0) return;
          continue;
        }

      if(!seen_brace && ch == _C(';'))
        {
          pos++;
          return;
        }

      pos++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_CSSPARSER::ParseOneRuleOrAtRule(XSTRING& text, int& pos, UI_STYLESHEET& out, int* media_min_w, int* media_max_w, int& ruleindex, int& rules_kept)
* @brief      Parse one top-level construct: @@import, @@media, or a normal rule. Optional media_* attach Track B
* @ingroup    USERINTERFACE
* 
* @param[in]  text : 
* @param[in]  pos : 
* @param[in]  out : 
* @param[in]  media_min_w : 
* @param[in]  media_max_w : 
* @param[in]  ruleindex : 
* @param[in]  rules_kept : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ParseOneRuleOrAtRule(XSTRING& text, int& pos, UI_STYLESHEET& out, int* media_min_w, int* media_max_w, int& ruleindex, int& rules_kept)
{
  int len = (int)text.GetSize();
  if(pos >= len) return true;

  // Nested @media body ends at the closing '}' of the outer block -- caller stops when it sees that.
  if(media_min_w && text[pos] == _C('}')) return true;

  if(text[pos] == _C('@'))
    {
      int     atrulestart = pos;
      int     kwend       = pos + 7; if(kwend > len) kwend = len;
      XSTRING atkeyword;
      text.Copy(pos, kwend, atkeyword);

      if(!atkeyword.Compare(_L("@import"), true))
        {
          if(media_min_w)
            {
              // @import inside @media is outside GEN's subset -- skip the statement.
              int line, col;
              ResolveLineColumn(text, atrulestart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] @import inside @media ignored (line %d, column %d)"), line, col);
              pos += 7;
              SkipToNextRule(text, pos);
              return true;
            }

          pos += 7;
          XSTRING importurl;
          if(ReadImportStatement(text, pos, importurl))
            {
              ResolveAndParseImport(importurl, out);
            }
           else
            {
              int line, col;
              ResolveLineColumn(text, atrulestart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] malformed @import discarded (line %d, column %d)"), line, col);
              SkipToNextRule(text, pos);
            }
          return true;
        }

      // "@media" is 6 chars -- compare by copying enough chars
      kwend = pos + 6; if(kwend > len) kwend = len;
      text.Copy(pos, kwend, atkeyword);
      if(!atkeyword.Compare(_L("@media"), true))
        {
          if(media_min_w)
            {
              int line, col;
              ResolveLineColumn(text, atrulestart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] nested @media ignored (line %d, column %d)"), line, col);
              SkipToNextRule(text, pos);
              return true;
            }

          pos += 6;
          int  min_w = -1;
          int  max_w = -1;
          bool ok    = false;
          if(!ReadMediaCondition(text, pos, min_w, max_w, ok))
            {
              int line, col;
              ResolveLineColumn(text, atrulestart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] malformed @media discarded (line %d, column %d)"), line, col);
              SkipToNextRule(text, pos);
              return true;
            }

          if(!ok)
            {
              // Unsupported feature list (e.g. orientation) -- skip block, do not keep rules.
              int line, col;
              ResolveLineColumn(text, atrulestart, line, col);
              XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] unsupported @media query ignored (line %d, column %d)"), line, col);
              SkipToNextRule(text, pos);
              return true;
            }

          return ParseMediaBlock(text, pos, out, min_w, max_w);
        }

      int line, col;
      ResolveLineColumn(text, atrulestart, line, col);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] unsupported at-rule ignored (line %d, column %d)"), line, col);
      SkipToNextRule(text, pos);
      return true;
    }

  int rulestartcolumn = pos;
  ruleindex++;

  UI_CSSRULE* rule = GEN_NEW UI_CSSRULE();
  if(!rule) return false;

  if(!ReadSelectorList(text, pos, rule) || !ReadDeclarationBlock(text, pos, rule->GetDeclarations()))
    {
      int startline, startcol, endline, endcol;
      ResolveLineColumn(text, rulestartcolumn, startline, startcol);
      ResolveLineColumn(text, pos, endline, endcol);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] rule #%d discarded: malformed selector or declaration block (started at line %d, column %d; gave up at line %d, column %d)"), ruleindex, startline, startcol, endline, endcol);
      if(!currentfilepath.IsEmpty())
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] file [%s] rule #%d discarded (lines %d:%d .. %d:%d)"), currentfilepath.Get(), ruleindex, startline, startcol, endline, endcol);
        }

      discarded_rules++;
      GEN_DELETE rule;
      SkipToNextRule(text, pos);
      return true;
    }

  if(rule->GetSelectors().GetSize() == 0 || rule->GetDeclarations().GetProperties()->GetSize() == 0)
    {
      int line, col;
      ResolveLineColumn(text, rulestartcolumn, line, col);
      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] rule #%d dropped: parsed with %d selector(s) and %d declaration(s) (started at line %d, column %d)"), ruleindex, (int)rule->GetSelectors().GetSize(), (int)rule->GetDeclarations().GetProperties()->GetSize(), line, col);
      if(!currentfilepath.IsEmpty())
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] file [%s] rule #%d empty after parse (line %d, column %d)"), currentfilepath.Get(), ruleindex, line, col);
        }

      discarded_rules++;
      GEN_DELETE rule;
      return true;
    }

  bool isroot = false;
  {
    XVECTOR<UI_CSSSELECTOR*>& sels = rule->GetSelectors();
    for(XDWORD s=0; s<sels.GetSize(); s++)
      {
        UI_CSSSELECTOR* sel = sels.Get(s);
        if(sel && sel->IsRootOnly()) { isroot = true; break; }
      }
  }

  if(isroot)
    {
      XVECTOR<UI_STYLEPROPERTY*>* declprops = rule->GetDeclarations().GetProperties();
      if(declprops)
        {
          for(XDWORD d=0; d<declprops->GetSize(); d++)
            {
              UI_STYLEPROPERTY* prop = declprops->Get(d);
              if(prop) out.Variables().Set(prop->GetKey().Get(), prop->GetValue());
            }
        }

      GEN_DELETE rule;
      return true;
    }

  if(media_min_w && media_max_w)
    {
      rule->SetMediaWidthRange(*media_min_w, *media_max_w);
    }

  out.Rules_Add(rule);
  rules_kept++;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_CSSPARSER::ReadMediaCondition(XSTRING& text, int& pos, int& out_min_w, int& out_max_w, bool& out_ok)
* @brief      Parse `@@media` query features after the keyword. Supports (min-width:N[px]) / (max-width:N[px])
* @ingroup    USERINTERFACE
* 
* @param[in]  text : 
* @param[in]  pos : 
* @param[in]  out_min_w : 
* @param[in]  out_max_w : 
* @param[in]  out_ok : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadMediaCondition(XSTRING& text, int& pos, int& out_min_w, int& out_max_w, bool& out_ok)
{
  int len = (int)text.GetSize();
  out_min_w = -1;
  out_max_w = -1;
  out_ok    = true;

  bool saw_feature = false;

  for(;;)
    {
      SkipWhitespaceAndComments(text, pos);
      if(pos >= len) return false;

      if(text[pos] == _C('{')) break;

      // optional "and" between features
      if(saw_feature)
        {
          if(pos + 3 <= len)
            {
              XSTRING andkw;
              text.Copy(pos, pos + 3, andkw);
              if(!andkw.Compare(_L("and"), true))
                {
                  // ensure word boundary
                  if(pos + 3 >= len || text[pos + 3] == _C(' ') || text[pos + 3] == _C('\t') || text[pos + 3] == _C('\n') || text[pos + 3] == _C('\r') || text[pos + 3] == _C('('))
                    {
                      pos += 3;
                      SkipWhitespaceAndComments(text, pos);
                    }
                }
            }
        }

      if(pos >= len || text[pos] != _C('(')) return false;
      pos++;   // '('

      SkipWhitespaceAndComments(text, pos);
      XSTRING feature;
      if(!ReadIdentifier(text, pos, feature)) return false;

      SkipWhitespaceAndComments(text, pos);
      if(pos >= len || text[pos] != _C(':')) return false;
      pos++;

      SkipWhitespaceAndComments(text, pos);
      // read number [px]
      int numstart = pos;
      while(pos < len && ((text[pos] >= _C('0') && text[pos] <= _C('9')) || text[pos] == _C('.'))) pos++;
      if(pos == numstart) return false;

      XSTRING numstr;
      text.Copy(numstart, pos, numstr);
      int value = numstr.ConvertToInt();

      SkipWhitespaceAndComments(text, pos);
      if(pos + 2 <= len)
        {
          XSTRING unit;
          text.Copy(pos, pos + 2, unit);
          if(!unit.Compare(_L("px"), true)) pos += 2;
        }

      SkipWhitespaceAndComments(text, pos);
      if(pos >= len || text[pos] != _C(')')) return false;
      pos++;

      if(!feature.Compare(_L("min-width"), true))
        {
          out_min_w = value;
          saw_feature = true;
        }
       else if(!feature.Compare(_L("max-width"), true))
        {
          out_max_w = value;
          saw_feature = true;
        }
       else
        {
          out_ok = false;   // unsupported feature; still consume the rest of the query until '{'
          // continue scanning so we can skip the block cleanly
          saw_feature = true;
        }
    }

  if(!saw_feature) return false;
  if(out_ok && out_min_w < 0 && out_max_w < 0) out_ok = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_CSSPARSER::ParseMediaBlock(XSTRING& text, int& pos, UI_STYLESHEET& out, int media_min_w, int media_max_w)
* @brief      Parse `{ ... }` body of an @@media rule; each kept rule receives the media width range.
* @ingroup    USERINTERFACE
* 
* @param[in]  text : 
* @param[in]  pos : 
* @param[in]  out : 
* @param[in]  media_min_w : 
* @param[in]  media_max_w : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ParseMediaBlock(XSTRING& text, int& pos, UI_STYLESHEET& out, int media_min_w, int media_max_w)
{
  int len = (int)text.GetSize();
  SkipWhitespaceAndComments(text, pos);
  if(pos >= len || text[pos] != _C('{')) return false;
  pos++;   // '{'

  int ruleindex  = 0;
  int rules_kept = 0;

  while(pos < len)
    {
      SkipWhitespaceAndComments(text, pos);
      if(pos >= len) break;
      if(text[pos] == _C('}'))
        {
          pos++;
          return true;
        }

      if(!ParseOneRuleOrAtRule(text, pos, out, &media_min_w, &media_max_w, ruleindex, rules_kept))
        {
          return false;
        }
    }

  return true;   // EOF without '}' -- tolerate like SkipToNextRule
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ReadImportStatement(XSTRING& text, int& pos, XSTRING& outurl)
* @brief      Step 5: parse the body of an "@@import" at-rule -- everything AFTER the "@@import" keyword itself
*             (the caller has already consumed that). Accepts a quoted string ("theme.css" / 'theme.css'),
*             optionally wrapped in CSS's "url(...)" form (quoted or bare), terminated by ';'. Nothing else in
*             real CSS's @@import grammar (media queries and the like) is supported -- GEN's layouts don't need
*             them, and a rule this small doesn't need to sniff for them either.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]     text : Source text.
* @param[in,out] pos : Cursor; expected right after the "@@import" keyword on entry, positioned right after the
*                terminating ';' (or at EOF, if none was found) on return.
* @param[out]    outurl : The quoted/bare URL text, verbatim (not yet resolved to a filesystem path).
*
* @return     bool : true if a non-empty URL was found; false otherwise (outurl is left empty).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ReadImportStatement(XSTRING& text, int& pos, XSTRING& outurl)
{
  int len = (int)text.GetSize();

  outurl.Empty();

  SkipWhitespaceAndComments(text, pos);

  bool sawurlwrapper = false;

  if((pos + 3) <= len)
    {
      XSTRING maybeurl;
      text.Copy(pos, pos + 3, maybeurl);
      if(!maybeurl.Compare(_L("url"), true))
        {
          sawurlwrapper = true;
          pos += 3;
          SkipWhitespaceAndComments(text, pos);
          if(pos < len && text[pos] == _C('(')) pos++;
          SkipWhitespaceAndComments(text, pos);
        }
    }

  if(pos >= len) return false;

  XCHAR quote = text[pos];

  if(quote == _C('"') || quote == _C('\''))
    {
      // Phase 2: reuse the shared string-literal reader instead of a duplicate ad hoc quote scan -- this also
      // gives the @import URL escape support ("\"") for free, consistent with every other quoted/identifier
      // read in this parser. Same "unterminated string: bail out, let the caller resync" failure behaviour.
      if(!ReadStringLiteral(text, pos, outurl)) return false;
    }
   else
    {
      // Bare token, only meaningful inside "url(...)" -- e.g. url(theme.css). Read up to ')' or ';'.
      int start = pos;
      while(pos < len && text[pos] != _C(')') && text[pos] != _C(';')) pos++;
      text.Copy(start, pos, outurl);
      outurl.DeleteNoCharacters(_L(" \t\r\n"));
    }

  SkipWhitespaceAndComments(text, pos);
  if(sawurlwrapper && pos < len && text[pos] == _C(')'))
    {
      pos++;
      SkipWhitespaceAndComments(text, pos);
    }

  while(pos < len && text[pos] != _C(';')) pos++;
  if(pos < len && text[pos] == _C(';')) pos++;

  return !outurl.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_CSSPARSER::ResolveAndParseImport(XSTRING& importurl, UI_STYLESHEET& out)
* @brief      Step 5: resolve an @@import URL against the directory of the file currently being parsed
*             (`currentfiledir`, maintained by ParseFile()) and merge the target file's rules and ":root"
*             variables into `out` by recursing into ParseFile() -- i.e. imported content behaves as if it had
*             been written in place of the "@@import" line, matching real CSS semantics.
* @note       A no-op (traced, never fatal to the importING file) when: `currentfiledir` is empty (ParseText()
*             was called directly on an in-memory buffer rather than through ParseFile(), so there is no
*             directory to resolve a RELATIVE url against), the target file is missing or empty, or the target
*             is already open along this @@import chain (cycle -- see ParseFile()).
* @ingroup    USERINTERFACE
*
* @param[in]  importurl : Raw URL text from ReadImportStatement(), e.g. "theme.css".
* @param[out] out : Target stylesheet; the imported file's rules/variables are appended into it.
*
* @return     bool : true if the imported file was found, parsed, and merged; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_CSSPARSER::ResolveAndParseImport(XSTRING& importurl, UI_STYLESHEET& out)
{
  if(importurl.IsEmpty()) return false;

  if(currentfiledir.IsEmpty())
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] @import [%s] ignored: no file context (stylesheet text was parsed directly, not loaded from a file)"), importurl.Get());
      return false;
    }

  XPATH importpath;
  importpath  = currentfiledir;
  importpath += importurl;

  // Phase 2: mark "we are now inside an @import recursion" for the duration of this nested ParseFile()/
  // ParseText() call, so ITS OWN ParseText() knows not to expand variables yet -- see ParseText()'s note.
  importdepth++;
  bool status = ParseFile(importpath, out);
  importdepth--;

  if(status)
    XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[CSS Parse] @import [%s] merged"), importpath.Get());
   else
    XTRACE_PRINTCOLOR(XTRACE_COLOR_WARNING, _L("[CSS Parse] @import [%s] not applied (missing, empty, or cyclic)"), importpath.Get());

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_CSSPARSER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_CSSPARSER::Clean()
{
  // Defensive only: importstack is pushed/popped in matched pairs within ParseFile(), so it is already empty
  // by the time a top-level ParseFile()/ParseText() call returns. DeleteContents() covers the one path where
  // it would not be -- an early return out of a deeply recursed @import chain -- without which those XPATH
  // markers would leak.
  importstack.DeleteContents();
  importstack.DeleteAll();

  currentfiledir.Empty();
  currentfilepath.Empty();
  importdepth             = 0;
  discarded_rules         = 0;
  unterminated_comments   = 0;
  last_rules_kept         = 0;
}
