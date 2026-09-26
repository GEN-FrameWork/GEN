/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOURL.cpp
* 
* @class      DIOURL
* @brief      Data Input/Output URL class
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOURL.h"

#include "XBuffer.h"

#include "DIODNSResolver.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL_IsIPv4Range(const XCHAR* host, XDWORD length)
* @brief      Is i pv4 range
* @ingroup    DATAIO
* 
* @param[in]  host : Pointer to host.
* @param[in]  length : Length value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOURL_IsIPv4Range(const XCHAR* host, XDWORD length)
{
  if(!host || !length) return false;

  int parts = 0;
  int value = 0;
  int digits = 0;

  for(XDWORD c=0; c<length; c++)
    {
      if((host[c] >= __C('0')) && (host[c] <= __C('9')))
        {
          value = (value * 10) + (host[c] - __C('0'));
          if(++digits > 3 || value > 255) return false;
        }
       else if(host[c] == __C('.'))
        {
          if(!digits || ++parts > 3) return false;
          value = 0;
          digits = 0;
        }
       else return false;
    }

  return (parts == 3) && digits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL_IsIPv4(XCHAR* host)
* @brief      Is i pv4
* @ingroup    DATAIO
* 
* @param[in]  host : Pointer to host.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOURL_IsIPv4(XCHAR* host)
{
  if(!host) return false;
  XDWORD length = 0;
  while(host[length]) length++;
  return DIOURL_IsIPv4Range(host, length);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL_IsIPv6(XCHAR* host)
* @brief      Is i pv6
* @ingroup    DATAIO
* 
* @param[in]  host : Pointer to host.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOURL_IsIPv6(XCHAR* host)
{
  if(!host || !host[0]) return false;

  XDWORD length = 0;
  while(host[length]) length++;

  XDWORD start = 0;
  XDWORD end = length;
  if(host[0] == __C('['))
    {
      if(length < 3 || host[length-1] != __C(']')) return false;
      start = 1;
      end = length - 1;
    }
   else if(host[length-1] == __C(']')) return false;

  XDWORD groups = 0;
  XDWORD position = start;
  bool   compressed = false;

  if(position < end && host[position] == __C(':'))
    {
      if((position + 1) >= end || host[position+1] != __C(':')) return false;
      compressed = true;
      position += 2;
      if(position == end) return true;
    }

  while(position < end)
    {
      XDWORD segmentstart = position;
      bool ipv4tail = false;
      while(position < end && host[position] != __C(':'))
        {
          if(host[position] == __C('%')) return false;
          if(host[position] == __C('.')) ipv4tail = true;
          position++;
        }

      XDWORD segmentsize = position - segmentstart;
      if(!segmentsize) return false;

      if(ipv4tail)
        {
          if(position != end || !DIOURL_IsIPv4Range(&host[segmentstart], segmentsize)) return false;
          groups += 2;
        }
       else
        {
          if(segmentsize > 4) return false;
          for(XDWORD c=segmentstart; c<position; c++)
            if(!((host[c] >= __C('0') && host[c] <= __C('9')) ||
                 (host[c] >= __C('a') && host[c] <= __C('f')) ||
                 (host[c] >= __C('A') && host[c] <= __C('F')))) return false;
          groups++;
        }

      if(groups > 8) return false;
      if(position == end) break;

      if((position + 1) < end && host[position+1] == __C(':'))
        {
          if(compressed) return false;
          compressed = true;
          position += 2;
          if(position == end) break;
        }
       else
        {
          position++;
          if(position == end) return false;
        }
    }

  return compressed?(groups < 8):(groups == 8);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL_IDNACodePointAllowed(XDWORD point, bool first)
* @brief      Idna code point allowed
* @ingroup    DATAIO
* 
* @param[in]  point : Point value.
* @param[in]  first : First value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOURL_IDNACodePointAllowed(XDWORD point, bool first)
{
  if(point < 0xA0 || (point >= 0xD800 && point <= 0xDFFF) || point > 0x10FFFF) return false;
  if((point >= 0xE000 && point <= 0xF8FF) || (point >= 0xF0000 && point <= 0xFFFFD) ||
     (point >= 0x100000 && point <= 0x10FFFD)) return false;
  if((point & 0xFFFE) == 0xFFFE) return false;
  if((point >= 0x200B && point <= 0x200F) || (point >= 0x202A && point <= 0x202E) ||
     (point >= 0x2060 && point <= 0x206F)) return false;
  if((point >= 0x1F000 && point <= 0x1FAFF) || (point >= 0x2600 && point <= 0x27BF)) return false;
  if(first && ((point >= 0x0300 && point <= 0x036F) || (point >= 0x1AB0 && point <= 0x1AFF) ||
               (point >= 0x1DC0 && point <= 0x1DFF) || (point >= 0x20D0 && point <= 0x20FF) ||
               (point >= 0xFE20 && point <= 0xFE2F))) return false;
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOURL_PunycodeDigit(XDWORD digit)
* @brief      Punycode digit
* @ingroup    DATAIO
* 
* @param[in]  digit : Digit value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XBYTE DIOURL_PunycodeDigit(XDWORD digit)
{
  return (XBYTE)((digit < 26)?(__C('a') + digit):(__C('0') + digit - 26));
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOURL_PunycodeAdapt(XDWORD delta, XDWORD points, bool first)
* @brief      Punycode adapt
* @ingroup    DATAIO
* 
* @param[in]  delta : Delta value.
* @param[in]  points : Points value.
* @param[in]  first : First value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XDWORD DIOURL_PunycodeAdapt(XDWORD delta, XDWORD points, bool first)
{
  delta = first?(delta / 700):(delta / 2);
  delta += delta / points;
  XDWORD k = 0;
  while(delta > 455) { delta /= 35; k += 36; }
  return k + ((36 * delta) / (delta + 38));
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL_PunycodeLabel(XSTRING& label, XSTRING& output)
* @brief      Punycode label
* @ingroup    DATAIO
* 
* @param[in]  label : Label value.
* @param[out] output : Output value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOURL_PunycodeLabel(XSTRING& label, XSTRING& output)
{
  XVECTOR<XDWORD> points;

  for(XDWORD c=0; c<label.GetSize(); c++)
    {
      XDWORD point = (XDWORD)label[c];
#if defined(WINDOWS)
      if((point >= 0xD800) && (point <= 0xDBFF) && ((c + 1) < label.GetSize()))
        {
          XDWORD low = (XDWORD)label[++c];
          if((low < 0xDC00) || (low > 0xDFFF)) return false;
          point = 0x10000 + (((point - 0xD800) << 10) | (low - 0xDC00));
        }
#endif
      if(!point || point > 0x10FFFF || (point >= 0xD800 && point <= 0xDFFF)) return false;
      if(point >= 0x80 && !DIOURL_IDNACodePointAllowed(point, points.IsEmpty())) return false;
      if(!points.Add(point)) return false;
    }

  XDWORD basic = 0;
  for(XDWORD c=0; c<points.GetSize(); c++)
    {
      XDWORD point = points.Get(c);
      if(point < 0x80)
        {
          XCHAR character = (XCHAR)point;
          if(character >= __C('A') && character <= __C('Z')) character += (__C('a') - __C('A'));
          if(!((character >= __C('a') && character <= __C('z')) ||
               (character >= __C('0') && character <= __C('9')) || character == __C('-'))) return false;
          output.Add(character);
          basic++;
        }
    }

  if(basic == points.GetSize())
    {
      if(output.IsEmpty() || output.GetSize() > 63 || output[0] == __C('-') || output.Character_GetLast() == __C('-')) return false;
      return true;
    }

  XSTRING encoded;
  encoded = __L("xn--");
  encoded += output;
  if(basic) encoded.Add(__C('-'));

  XDWORD n = 128;
  XDWORD delta = 0;
  XDWORD bias = 72;
  XDWORD handled = basic;

  while(handled < points.GetSize())
    {
      XDWORD minimum = 0x10FFFF;
      for(XDWORD c=0; c<points.GetSize(); c++) if(points.Get(c) >= n && points.Get(c) < minimum) minimum = points.Get(c);
      if(minimum == 0x10FFFF || (minimum - n) > ((0xFFFFFFFF - delta) / (handled + 1))) return false;
      delta += (minimum - n) * (handled + 1);
      n = minimum;

      for(XDWORD c=0; c<points.GetSize(); c++)
        {
          XDWORD point = points.Get(c);
          if(point < n && ++delta == 0) return false;
          if(point == n)
            {
              XDWORD q = delta;
              for(XDWORD k=36;;k+=36)
                {
                  XDWORD threshold = (k <= bias)?1:((k >= bias + 26)?26:(k - bias));
                  if(q < threshold) break;
                  encoded.Add((XCHAR)DIOURL_PunycodeDigit(threshold + ((q - threshold) % (36 - threshold))));
                  q = (q - threshold) / (36 - threshold);
                }
              encoded.Add((XCHAR)DIOURL_PunycodeDigit(q));
              bias = DIOURL_PunycodeAdapt(delta, handled + 1, handled == basic);
              delta = 0;
              handled++;
            }
        }
      delta++;
      n++;
    }

  if(encoded.GetSize() > 63) return false;
  output = encoded;
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL_HOSTTYPE DIOURL::Host_GetType(XCHAR* host)
* @brief      Host get type
* @ingroup    DATAIO
* 
* @param[in]  host : Pointer to host.
* 
* @return     DIOURL_HOSTTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL_HOSTTYPE DIOURL::Host_GetType(XCHAR* host)
{
  if(DIOURL_IsIPv4(host)) return DIOURL_HOSTTYPE_IPV4;
  if(DIOURL_IsIPv6(host)) return DIOURL_HOSTTYPE_IPV6;
  return (host && host[0])?DIOURL_HOSTTYPE_DNS:DIOURL_HOSTTYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::Host_Canonicalize(XCHAR* host, XSTRING& canonicalhost, DIOURL_HOSTTYPE& type)
* @brief      Host canonicalize
* @ingroup    DATAIO
* 
* @param[in]  host : Pointer to host.
* @param[in]  canonicalhost : Canonicalhost value.
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::Host_Canonicalize(XCHAR* host, XSTRING& canonicalhost, DIOURL_HOSTTYPE& type)
{
  canonicalhost.Empty();
  type = Host_GetType(host);
  if(type == DIOURL_HOSTTYPE_UNKNOWN) return false;

  XSTRING source;
  source = host;
  if(type == DIOURL_HOSTTYPE_IPV6)
    {
      if(source[0] == __C('[')) source.DeleteCharacters(0, 1);
      if(source.Character_GetLast() == __C(']')) source.DeleteLastCharacter();
      source.ToLowerCase();
      canonicalhost = source;
      return true;
    }
  if(type == DIOURL_HOSTTYPE_IPV4)
    {
      canonicalhost = source;
      return true;
    }

  XSTRING mapped;
  for(XDWORD c=0; c<source.GetSize(); c++)
    {
      XDWORD point=(XDWORD)source[c];
      if(point == 0x3002 || point == 0xFF0E || point == 0xFF61) point = __C('.');
      else if(point >= 0xFF01 && point <= 0xFF5E) point -= 0xFEE0; // UTS #46 width mapping
      else if(point == 0x3000) point = __C(' ');

      // UTS #46 deviation mappings (non-transitional form) and canonical
      // sigma folding. Local tables keep IDNA portable (no IdnToAscii / normaliz).
      if(point == 0x03C2) point = 0x03C3;
      if(point == 0x1E9E) point = 0x00DF;
      if(point == 0x00DF) { mapped.Add(__C('s')); mapped.Add(__C('s')); continue; }
      mapped.Add((XCHAR)point);
    }
  source = mapped;

  // UTS #46 non-transitional processing applies Unicode case mapping before
  // the label validity and punycode steps.
  source.ToLowerCase();

  if(source.Character_GetLast() == __C('.')) source.DeleteLastCharacter();
  if(source.IsEmpty()) return false;

  XDWORD start = 0;
  while(start < source.GetSize())
    {
      XDWORD end = start;
      while(end < source.GetSize() && source[end] != __C('.')) end++;
      if(end == start) return false;

      XSTRING label;
      XSTRING encoded;
      source.Copy(start, end, label);
      if(!DIOURL_PunycodeLabel(label, encoded)) return false;
      if(!canonicalhost.IsEmpty()) canonicalhost.Add(__C('.'));
      canonicalhost += encoded;
      start = end + 1;
    }

  return canonicalhost.GetSize() <= 253;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::~DIOURL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::~DIOURL()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(XDWORD size) : XSTRING(size)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(XDWORD size) : XSTRING(size)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(const char* url) : XSTRING(url)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(const char* url) : XSTRING(url)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(const XCHAR* url) : XSTRING(url)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(const XCHAR* url) : XSTRING(url)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(const XCHAR* url, XDWORD size) : XSTRING(url, size)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(const XCHAR* url, XDWORD size) : XSTRING(url, size)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(DIOURL& url) : XSTRING(url)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(DIOURL& url) : XSTRING(url)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(XSTRING& url) : XSTRING(url)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(XSTRING& url) : XSTRING(url)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL::DIOURL(XWORD* url) : XSTRING(url)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL::DIOURL(XWORD* url) : XSTRING(url)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::GetHTTPServer(XSTRING& server, XSTRING& login, XSTRING& password)
* @brief      Get HTTP server
* @ingroup    DATAIO
* 
* @param[in]  server : Server value.
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::GetHTTPServer(XSTRING& server, XSTRING& login, XSTRING& password)
{
  server.Empty();
  //login.Empty();
  //password.Empty();

  if(IsEmpty()) return false;

  Copy(DIOURL_WEBURLID, __L("/"), true, 0, server);
  if(server.IsEmpty())
    {
      int index = Find(__L("/"), true);
      if(index != XSTRING_NOTFOUND)
        {
          Copy(0, index, server);
        }
       else server = Get();
    }

  if(server.IsEmpty()) return false;

  int indexlp = server.Find(__L("@"), true);
  if(indexlp!=XSTRING_NOTFOUND)
    {
      XSTRING  lpstring;

      server.Copy(0, indexlp, lpstring);
      server.DeleteCharacters(0, indexlp+1);

      int indexp = lpstring.Find(__L(":"), true);
      if(indexp!=XSTRING_NOTFOUND)
        {
          lpstring.Copy(0, indexp, login);
          lpstring.Copy(indexp+1 , password);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::GetHTTPResource(XSTRING& resource)
* @brief      Get HTTP resource
* @ingroup    DATAIO
* 
* @param[in]  resource : Resource value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::GetHTTPResource(XSTRING& resource)
{
  resource.Empty();

  if(IsEmpty()) return false;

  XSTRING weburlid;

  weburlid = DIOURL_WEBURLID;

  int index = Find(weburlid.Get(), true);
  if(index == XSTRING_NOTFOUND)
        index  = 0;
   else index += weburlid.GetSize();

  index = Find(__L("/"), true, index);
  if(index != XSTRING_NOTFOUND)
    {
      Copy(index, GetSize(), resource);
    }

  if(resource.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::IsAURL()
* @brief      Is AURL
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::IsAURL()
{
  //int count = 0;

  for(XDWORD c=0; c<size; c++)
    {
      if(!text[c]) break;
  
      if((text[c] >= __C('0')) && (text[c] <= '9') || (text[c] == __C('.'))) 
        {
          

        }
       else
        {
          return true;        
        }     
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::ResolveURL()
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::ResolveURL()
{
  XSTRING IPstring;

  if(!ResolveURL(IPstring)) return false;

  Set(IPstring.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::ResolveURL(XSTRING& IPstring)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  IPstring : I Pstring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::ResolveURL(XSTRING& IPstring)
{
  IPstring.Empty();

  DIOIP IP;
  bool  status = false;
 
  status = GEN_DIODNSRESOLVER.ResolveURL(Get(), IP, DIODNSPROTOCOL_TYPEQUERY_A);
  if(status)  IP.GetXString(IPstring);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::ResolveURL(DIOIP& IP)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  IP : IP address to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::ResolveURL(DIOIP& IP)
{
  XSTRING stringIP;

  stringIP.Empty();

  if(!ResolveURL(stringIP)) return false;

  IP.Set(stringIP);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::IsLocalAddress()
* @brief      Is local address
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::IsLocalAddress()
{
  DIOIP IP;

  if(IsAURL())
    {
      ResolveURL(IP);

    } else IP.Set(Get());

  return IP.IsLocal();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::Slash_Add()
* @brief      Slash add
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::Slash_Add()
{
  int    count[2] = { 0 , 0 };
  XCHAR* str      = Get();
  bool   status   = false;

  int c=0;
  while(str[c])
    {
      if(str[c]==__C('/'))  count[0]++;
      if(str[c]==__C('\\')) count[1]++;

      c++;
    }

  if((Character_GetLast()!=__C('\\')) && (Character_GetLast()!=__C('/')))
    {
      if(count[0]>count[1])
             Add("/");
        else Add("\\");

      Slash_Normalize();

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::Slash_Normalize()
* @brief      Slash normalize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::Slash_Normalize()
{
  if(IsEmpty()) return false;

  XCHAR* str = Get();
  int    c   = 0;

  while(str[c])
    {
      if(str[c]==__C('\\')) str[c] = __C('/');
      c++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::Slash_Delete()
* @brief      Slash delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::Slash_Delete()
{
  bool status = false;

  if((Character_GetLast()==__C('\\')) || (Character_GetLast()==__C('/')))
    {
      DeleteCharacter('\\' , XSTRINGCONTEXT_TO_END);
      DeleteCharacter('/'  , XSTRINGCONTEXT_TO_END);

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::IsUnsafeChar(XCHAR character)
* @brief      Is unsafe char
* @ingroup    DATAIO
* 
* @param[in]  character : Character value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::IsUnsafeChar(XCHAR character)
{
  //XSTRING safecharacters(__L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!*'();:@=+$,/?[]"));
  XSTRING safecharacters(__L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_/\\."));

  for(int c=0; c<(int)safecharacters.GetSize(); c++)
    {
      if(character == safecharacters.Get()[c])  return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeChar(XCHAR character, XSTRING& encodechar)
* @brief      Encode unsafe char
* @ingroup    DATAIO
* 
* @param[in]  character : Character value.
* @param[in]  encodechar : Encodechar value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeChar(XCHAR character, XSTRING& encodechar)
{
  XWORD normalizechar = (XWORD)character;

  encodechar.Empty();

  if(character&0xFF00)
        encodechar.Format(__L("%%%02X%%%02X"), (normalizechar>>8), (normalizechar&0x00FF));
   else encodechar.Format(__L("%%%02X"), normalizechar);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeCharsFromString(XSTRING& string)
* @brief      Encode unsafe chars from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(XSTRING& string)
{
  Empty();

  for(int c=0; c<(int)string.GetSize(); c++)
    {
      if(!IsUnsafeChar(string.Get()[c]))
        {
          Add(string.Get()[c]);
        }
       else
        {
          XSTRING encodechar;

          encodechar.Empty();

          switch(string.Get()[c])
            {
              case __C('&')  : if(string.Get()[c+1] == __C(' '))
                                     EncodeUnsafeChar(string.Get()[c], encodechar);
                                else Add(string.Get()[c]);
                               break;

              case __C('\\') :
              case __C('%')  :
                    default  : EncodeUnsafeChar(string.Get()[c], encodechar);
                               break;
            }

          if(!encodechar.IsEmpty()) Add(encodechar);

        }
    }

  if(IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeCharsFromString(const char* string)
* @brief      Encode unsafe chars from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(const char* string)
{
  XSTRING string2(string);

  return EncodeUnsafeCharsFromString(string2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeCharsFromString(const XCHAR* string)
* @brief      Encode unsafe chars from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(const XCHAR* string)
{
  XSTRING string2(string);

  return EncodeUnsafeCharsFromString(string2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeCharsFromBuffer(XBUFFER& xbuffer)
* @brief      Encode unsafe chars from buffer
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromBuffer(XBUFFER& xbuffer)
{
  Empty();

  for(int c=0; c<(int)xbuffer.GetSize(); c++)
    {
      if(!IsUnsafeChar(xbuffer.Get()[c]))
        {
          Add(xbuffer.Get()[c]);
        }
       else
        {
          XSTRING encodechar;

          encodechar.Empty();

          switch(xbuffer.Get()[c])
            {
              case __C('&')  : if(xbuffer.GetByte(c+1) == __C(' '))
                                     EncodeUnsafeChar(xbuffer.GetByte(c), encodechar);
                                else Add(xbuffer.GetByte(c));
                               break;

              case __C('\\') :
              case __C('%')  :
                    default  : EncodeUnsafeChar(xbuffer.GetByte(c), encodechar);
                               break;
            }

          if(!encodechar.IsEmpty()) Add(encodechar);

        }
    }

  if(IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::EncodeUnsafeChars()
* @brief      Encode unsafe chars
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeChars()
{
  XSTRING string;

  string = Get();

  if(!EncodeUnsafeCharsFromString(string)) return false;

  return true;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::DecodeUnsafeCharsToString(XSTRING& string)
* @brief      Decode unsafe chars to string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::DecodeUnsafeCharsToString(XSTRING& string)
{
  string.Empty();

  if(IsEmpty()) return false;

  for(int c=0;c<(int)GetSize();c++)
    {
      XCHAR character[2] = { 0 , 0 };

      character[0] = Get()[c];

      if(character[0]==__C('%'))
        {
          XSTRING   numberhex;
          XCHAR part[2] = { 0 , 0 };

          c++;

          for(int d=0;d<2;d++)
            {
              part[0] = Get()[c+d];
              numberhex.Add(part);
            }

          numberhex.UnFormat(__L("%02X"), &part[0]);

          part[0]&=0x00FF;
          string.Add(part);

          c++;
        }
       else
        {
          if(character[0]==__C('+'))
            {
              string.Add(__C(' '));
            }
           else string.Add(character);
        }
    }

  if(string.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::DecodeUnsafeChars()
* @brief      Decode unsafe chars
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::DecodeUnsafeChars()
{
  XSTRING string;

  string = Get();

  if(!DecodeUnsafeCharsToString(string)) return false;

  Set(string);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::Normalize()
* @brief      Normalize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::Normalize()
{
  XSTRING string;

  string = Get();

  EncodeUnsafeCharsFromString(string);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::HaveHTTPID()
* @brief      Have HTTPID
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::HaveHTTPID()
{
  if(Find(DIOURL_WEBURLID, true)        != XSTRING_NOTFOUND) return true;
  if(Find(DIOURL_WEBURLID_SECURE, true) != XSTRING_NOTFOUND) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::AddHTTPID()
* @brief      Add HTTPID
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::AddHTTPID()
{
  if(HaveHTTPID()) return false;
  if(IsEmpty())    return false;

  XSTRING url;

  url  = DIOURL_WEBURLID;
  url += Get();

  Set(url);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::DeleteHTTPID()
* @brief      Delete HTTPID
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::DeleteHTTPID()
{
  if(!HaveHTTPID()) return false;
  if(IsEmpty())    return false;

  XSTRING url;
  XSTRING urlID;

  urlID = DIOURL_WEBURLID;
  url   = Get();
  if(url.Find(urlID, true) != XSTRING_NOTFOUND) 
    {
      url.DeleteCharacters(0, urlID.GetSize());
    }
   else 
    {
      urlID = DIOURL_WEBURLID_SECURE;
      if(url.Find(urlID, true) != XSTRING_NOTFOUND) 
        {
          url.DeleteCharacters(0, urlID.GetSize());
        }
    }

  Set(url);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::GetExtension(XSTRING& extension)
* @brief      Get extension
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::GetExtension(XSTRING& extension)
{
  extension.Empty();

  int indexext;
  for(indexext = GetSize(); indexext>0; indexext--)
    {
      if(Get()[indexext] == __C('.'))
        {
          break;
        }

      if((Get()[indexext] == __C('\\')) ||
          Get()[indexext] == __C('/')   ||
          Get()[indexext] == __C(':'))
        {
          indexext = -1;
          break;
        }
    }

  if(indexext > 0) Copy(indexext, extension);

  return (extension.IsEmpty()?false:true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOURL::GetFileName(XSTRING& filename)
* @brief      Get file name
* @ingroup    DATAIO
* 
* @param[in]  filename : File name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOURL::GetFileName(XSTRING& filename)
{
  filename.Empty();

  int indexext;
  for(indexext = GetSize(); indexext>0; indexext--)
    {
      if((Get()[indexext] == __C('\\')) ||
          Get()[indexext] == __C('/')   ||
          Get()[indexext] == __C(':'))
        {
          Copy(indexext+1, filename);
          break;
        }
    }

  return (filename.IsEmpty()?false:true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOURL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOURL::Clean()
{

}
