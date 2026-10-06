/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebTranslation.cpp
* 
* @class      DIOSCRAPERWEBTRANSLATION
* @brief      Typed Translation scraper filled by script (not XML)
* @ingroup    DATAIO
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

#ifdef DIO_SCRAPERWEB_TRANSLATION_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebTranslation.h"

#include "XFactory.h"
#include "XThread.h"
#include "XVector.h"
#include "XLanguage_ISO_639_3.h"

#include "DIOURL.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetSourceLanguage()
* @brief      Get source language
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetSourceLanguage()
{
  return sourcelanguage.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetTargetLanguage()
* @brief      Get target language
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetTargetLanguage()
{
  return targetlanguage.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetTranslation()
* @brief      Get translation
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetTranslation()
{
  return translation.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::IsEmpty()
{
  if(translation.IsEmpty()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOTRANSLATION_RESULT* translationresult = (DIOTRANSLATION_RESULT*)result;

  if(translationresult->IsEmpty()) return false;

  sourcelanguage = translationresult->sourcelanguage;
  targetlanguage = translationresult->targetlanguage;
  translation    = translationresult->translation;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOTRANSLATION_RESULT* translationresult = (DIOTRANSLATION_RESULT*)result;

  translationresult->sourcelanguage = sourcelanguage;
  translationresult->targetlanguage = targetlanguage;
  translationresult->translation    = translation;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::Set(XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::Set(XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation)
{
  return Set(sourcelanguage.Get(), targetlanguage.Get(), translation.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::Set(XCHAR* sourcelanguage, XCHAR* targetlanguage, XCHAR* translation)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::Set(XCHAR* sourcelanguage, XCHAR* targetlanguage, XCHAR* translation)
{
  if(sourcelanguage) this->sourcelanguage = sourcelanguage;
  if(targetlanguage) this->targetlanguage = targetlanguage;
  if(translation)    this->translation    = translation;

  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      this->sourcelanguage.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
      this->targetlanguage.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
      this->translation.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOTRANSLATION_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOTRANSLATION_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool DIOSCRAPERWEBTRANSLATION_IsPrintfConversion(XCHAR character)
* @brief      True if character is a C/C++ printf conversion specifier
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSCRAPERWEBTRANSLATION_IsPrintfConversion(XCHAR character)
{
  switch(character)
    {
      case _C('d') :
      case _C('i') :
      case _C('o') :
      case _C('u') :
      case _C('x') :
      case _C('X') :
      case _C('e') :
      case _C('E') :
      case _C('f') :
      case _C('F') :
      case _C('g') :
      case _C('G') :
      case _C('a') :
      case _C('A') :
      case _C('c') :
      case _C('s') :
      case _C('p') :
      case _C('n') : return true;
      // '%' is NOT a conversion here: literal percent is only "%%" (handled above).
      // Treating '%' as conversion after a width wrongly matches "%20%d" as "%20%".

           default  : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int DIOSCRAPERWEBTRANSLATION_GetPrintfFormatLength(XSTRING& text, int start)
* @brief      Length of a complete printf mask at start, or 0 if not a mask
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
static int DIOSCRAPERWEBTRANSLATION_GetPrintfFormatLength(XSTRING& text, int start)
{
  int size = (int)text.GetSize();
  int i;

  if((start < 0) || (start >= size))         return 0;
  if(text.Get()[start] != _C('%'))          return 0;

  i = start + 1;
  if(i >= size) return 0;

  // Escaped percent "%%"
  if(text.Get()[i] == _C('%')) return 2;

  // Flags
  while(i < size)
    {
      XCHAR c = text.Get()[i];
      if((c == _C('+')) || (c == _C('-')) || (c == _C(' ')) || (c == _C('#')) || (c == _C('0')))
            i++;
       else break;
    }

  // Width
  if((i < size) && (text.Get()[i] == _C('*')))
    {
      i++;
    }
   else
    {
      while((i < size) && (text.Get()[i] >= _C('0')) && (text.Get()[i] <= _C('9')))
        {
          i++;
        }
    }

  // Precision
  if((i < size) && (text.Get()[i] == _C('.')))
    {
      i++;
      if((i < size) && (text.Get()[i] == _C('*')))
        {
          i++;
        }
       else
        {
          while((i < size) && (text.Get()[i] >= _C('0')) && (text.Get()[i] <= _C('9')))
            {
              i++;
            }
        }
    }

  // Length modifier
  if(i < size)
    {
      XCHAR c = text.Get()[i];

      if(c == _C('h'))
        {
          i++;
          if((i < size) && (text.Get()[i] == _C('h'))) i++;
        }
       else if(c == _C('l'))
        {
          i++;
          if((i < size) && (text.Get()[i] == _C('l'))) i++;
        }
       else if((c == _C('L')) || (c == _C('j')) || (c == _C('z')) || (c == _C('t')))
        {
          i++;
        }
       else if(c == _C('I'))
        {
          if(((i + 2) < size) && (text.Get()[i + 1] == _C('3')) && (text.Get()[i + 2] == _C('2')))
            {
              i += 3;
            }
           else if(((i + 2) < size) && (text.Get()[i + 1] == _C('6')) && (text.Get()[i + 2] == _C('4')))
            {
              i += 3;
            }
           else
            {
              i++;
            }
        }
    }

  if((i < size) && DIOSCRAPERWEBTRANSLATION_IsPrintfConversion(text.Get()[i]))
    {
      return (i + 1) - start;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool DIOSCRAPERWEBTRANSLATION_IsPercentHex(XSTRING& text, int start)
* @brief      True if text at start is a valid URL "%HH" escape
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSCRAPERWEBTRANSLATION_IsPercentHex(XSTRING& text, int start)
{
  if((start + 2) >= (int)text.GetSize()) return false;
  if(text.Get()[start] != _C('%'))       return false;

  XCHAR h1 = text.Get()[start + 1];
  XCHAR h2 = text.Get()[start + 2];
  bool  h1hex = (((h1 >= _C('0')) && (h1 <= _C('9'))) ||
                 ((h1 >= _C('A')) && (h1 <= _C('F'))) ||
                 ((h1 >= _C('a')) && (h1 <= _C('f'))));
  bool  h2hex = (((h2 >= _C('0')) && (h2 <= _C('9'))) ||
                 ((h2 >= _C('A')) && (h2 <= _C('F'))) ||
                 ((h2 >= _C('a')) && (h2 <= _C('f'))));

  return (h1hex && h2hex);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool DIOSCRAPERWEBTRANSLATION_IsClassicPrintfConv(XCHAR character)
* @brief      Classic integer/string printf conversions worth preserving over URL %HH
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSCRAPERWEBTRANSLATION_IsClassicPrintfConv(XCHAR character)
{
  switch(character)
    {
      case _C('d') :
      case _C('i') :
      case _C('o') :
      case _C('u') :
      case _C('x') :
      case _C('X') :
      case _C('c') :
      case _C('s') :
      case _C('p') :
      case _C('n') : return true;
           default  : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool DIOSCRAPERWEBTRANSLATION_ShouldProtectPrintf(XSTRING& text, int start, int len)
* @brief      Whether a printf mask at start should be shielded from URL decoding
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSCRAPERWEBTRANSLATION_ShouldProtectPrintf(XSTRING& text, int start, int len)
{
  if(len <= 0) return false;

  // Pure printf (no "%HH" ambiguity): %s, %d, %.2f, %%...
  if(!DIOSCRAPERWEBTRANSLATION_IsPercentHex(text, start)) return true;

  // "%2d" / "%2x" / "%2s"... — prefer printf over URL for classic conversions.
  if(len == 3)
    {
      return DIOSCRAPERWEBTRANSLATION_IsClassicPrintfConv(text.Get()[start + 2]);
    }

  if(len >= 4)
    {
      XCHAR after = text.Get()[start + 1];

      // Flagged / zero-padded / precision forms: %08X, %+d, %.2f...
      if((after == _C('0')) || (after == _C('+')) || (after == _C('-')) ||
         (after == _C('#')) || (after == _C(' ')) || (after == _C('.')) ||
         (after == _C('*')))
        {
          return true;
        }

      // "%20of" / "%20at" look like printf "%20o" / "%20a" but are URL "%20" + text.
      return false;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBTRANSLATION_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
{
  if(cache)
    {
      cache->DeleteAll();
      GEN_DELETE cache;
    }

  if(xmutexdo)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexdo);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSCRAPERWEBTRANSLATION::DecodeTranslationText(XSTRING& text)
* @brief      Decode URL percent-escapes in scraper text; keep C/C++ printf masks intact
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::DecodeTranslationText(XSTRING& text)
{
  XVECTOR<XSTRING*> masks;
  XSTRING           work;
  DIOURL            url;
  XSTRING           decoded;
  XSTRING           plusguard(_L("\x03"));
  bool              status = true;

  if(text.IsEmpty()) return false;

  // Protect complete printf masks (%s, %d, %2d, %08X, %%...) before URL decode.
  // Ambiguous cases ("%20of" vs "%20o", "%2d" vs URL 0x2D) resolved by
  // DIOSCRAPERWEBTRANSLATION_ShouldProtectPrintf.
  for(int c = 0; c < (int)text.GetSize(); )
    {
      if(text.Get()[c] == _C('%'))
        {
          int len = DIOSCRAPERWEBTRANSLATION_GetPrintfFormatLength(text, c);
          if((len > 0) && DIOSCRAPERWEBTRANSLATION_ShouldProtectPrintf(text, c, len))
            {
              XSTRING* mask = GEN_NEW XSTRING();
              XSTRING  token;

              if(!mask)
                {
                  status = false;
                  break;
                }

              text.Copy(c, c + len, (*mask));
              masks.Add(mask);

              token.Format(_L("\x01FMT%04d\x02"), (int)(masks.GetSize() - 1));
              work.Add(token);
              c += len;
              continue;
            }
        }

      work.Add(text.Get()[c]);
      c++;
    }

  if(status)
    {
      // Keep literal '+' (e.g. "C++"); DIOURL form-decoding maps '+' → space.
      {
        XSTRING plus(_L("+"));
        work.Replace(plus.Get(), plusguard.Get());
      }

      url = work;
      if(url.DecodeUnsafeCharsToString(decoded))
        {
          work = decoded;
        }

      work.Replace(plusguard.Get(), _L("+"));

      for(XDWORD i = 0; i < masks.GetSize(); i++)
        {
          XSTRING  token;
          XSTRING* mask = masks.Get(i);

          token.Format(_L("\x01FMT%04d\x02"), (int)i);
          if(mask) work.Replace(token.Get(), mask->Get());
        }

      text = work;
    }

  masks.DeleteContents();
  masks.DeleteAll();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, DIOTRANSLATION_RESULT& result, ...)
* @brief      Get translation via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool    status = false;
  XSTRING cacheask;
  XSTRING sl;
  XSTRING tl;

  if((!text) || (!text[0]))           return false;
  if((!targetlanguage) || (!targetlanguage[0])) return false;

  {
    XLANGUAGE_ISO_639_3 iso;
    XSTRING             resolved;

    // Source: "auto" stays; otherwise resolve GEN alpha3/alpha2 (or legacy iw/jw) → ISO 639-1.
    if(sourcelanguage && sourcelanguage[0] && (XSTRING::Compare(sourcelanguage, _L("auto"), true) != 0))
      {
        if(iso.CodeAlpha2_Resolve(sourcelanguage, resolved))
              sl = resolved;
         else sl = sourcelanguage; // pass-through BCP-47 (zh-CN, zh-TW, ...)
      }
     else
      {
        sl = _L("auto");
      }

    // Target must be a concrete language for translate APIs.
    if(iso.CodeAlpha2_Resolve(targetlanguage, resolved))
          tl = resolved;
     else tl = targetlanguage;
  }

  if(xmutexdo) xmutexdo->Lock();

  cacheask  = text;
  cacheask += _L("|");
  cacheask += sl;
  cacheask += _L("|");
  cacheask += tl;

  if(usecache && cache)
    {
      DIOTRANSLATION_RESULT* cached = (DIOTRANSLATION_RESULT*)cache->Get(cacheask);
      if(cached)
        {
          result.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;

  runner.SetArg(_L("text"), text);
  runner.SetArg(_L("sl"), sl);
  runner.SetArg(_L("tl"), tl);
  runner.SetArgInt(_L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(_L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING translation;
      XSTRING detected;

      runner.GetResult(_L("ok"), ok);
      runner.GetResult(_L("translation"), translation);
      runner.GetResult(_L("detected"), detected);

      if((ok.Compare(_L("1")) == 0) && (!translation.IsEmpty()))
        {
          if(detected.IsEmpty()) detected = sl;

          DecodeTranslationText(translation);
          result.Set(detected, tl, translation);

          if(!result.IsEmpty())
            {
              if(usecache && cache)
                {
                  DIOTRANSLATION_RESULT* entry = GEN_NEW DIOTRANSLATION_RESULT();
                  if(entry)
                    {
                      entry->CopyFrom(&result);
                      cache->Add(cacheask, entry);
                    }
                }

              status = true;
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, DIOTRANSLATION_RESULT& result, ...)
* @brief      Get translation via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(text.Get(), sourcelanguage.Get(), targetlanguage.Get(), result, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, XSTRING& translation, ...)
* @brief      Legacy-compatible out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOTRANSLATION_RESULT result;

  if(!Get(text, sourcelanguage, targetlanguage, result, timeoutforurl, localIP, usecache)) return false;

  translation = result.GetTranslation();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation, ...)
* @brief      Legacy-compatible out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(text.Get(), sourcelanguage.Get(), targetlanguage.Get(), translation, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBTRANSLATION::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBTRANSLATION::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBTRANSLATION::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBTRANSLATION::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
