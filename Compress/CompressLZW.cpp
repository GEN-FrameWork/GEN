/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CompressLZW.cpp
* 
* @class      COMPRESSLZW
* @brief      Compress LZW (GIF compression) class
* @ingroup    COMPRESS
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

#include "CompressLZW.h"

#include <stdio.h>
#include <string.h>

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         COMPRESS_LZW::COMPRESS_LZW()
* @brief      Constructor of class
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
COMPRESS_LZW::COMPRESS_LZW()
{
  SetType(COMPRESSBASE_TYPE_LZW);

  interval[0] = interval[1] = 8;
  interval[2] = 4;
  interval[3] = 2;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         COMPRESS_LZW::~COMPRESS_LZW()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
COMPRESS_LZW::~COMPRESS_LZW()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZW::Compress(XBYTE* source, XDWORD size, XBUFFER* xbuffer)
* @brief      Compress
* @ingroup    COMPRESS
* 
* @param[in]  source : Source pointer to use.
* @param[in]  size : Size value.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZW::Compress(XBYTE* source, XDWORD size, XBUFFER* xbuffer)
{
  if(!source)  return false;
  if(!size)    return false;
  if(!xbuffer) return false;

  xbuffer->Delete();
  xbuffer->Resize(size+LZW_MAX_OVERCMP);

  XDWORD csize = LZW_Encode(source, size, xbuffer->Get());

  xbuffer->Resize(csize);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZW::Decompress(XBYTE* source, XDWORD size, XBUFFER* xbuffer)
* @brief      Decompress
* @ingroup    COMPRESS
* 
* @param[in]  source : Source pointer to use.
* @param[in]  size : Size value.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZW::Decompress(XBYTE* source, XDWORD size, XBUFFER* xbuffer)
{
  if(!source)  return false;
  if(!xbuffer) return false;
  if(!size)    return false;

  LZW_Decode(source, xbuffer->Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD COMPRESS_LZW::LZW_Encode(XBYTE* inbuffer, XDWORD length, XBYTE* outbuffer)
* @brief      LZW encode
* @ingroup    COMPRESS
* 
* @param[in]  inbuffer : Inbuffer pointer to use.
* @param[in]  length : Length value.
* @param[in]  outbuffer : Output outbuffer.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD COMPRESS_LZW::LZW_Encode(XBYTE* inbuffer, XDWORD length, XBYTE* outbuffer)
{
  *outbuffer++ = 8 ;

  return 1 + this->LZW_GIF_Encode(inbuffer, outbuffer, length, 1, 8);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::LZW_Decode(XBYTE* inbuffer, XBYTE* outbuffer)
* @brief      LZW decode
* @ingroup    COMPRESS
* 
* @param[in]  inbuffer : Inbuffer pointer to use.
* @param[in]  outbuffer : Output outbuffer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::LZW_Decode(XBYTE* inbuffer, XBYTE* outbuffer)
{
  this->LZW_GIF_Decode (inbuffer, outbuffer, 0, 0, false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD COMPRESS_LZW::LZW_GIF_Encode(XBYTE* dibbuffer, XBYTE* outbuffer, XDWORD dibwidth, XDWORD dibheight, XWORD colorbit)
* @brief      LZWGIF encode
* @ingroup    COMPRESS
* 
* @param[in]  dibbuffer : Dibbuffer pointer to use.
* @param[in]  outbuffer : Output outbuffer.
* @param[in]  dibwidth : Dibwidth value.
* @param[in]  dibheight : Dibheight value.
* @param[in]  colorbit : Colorbit value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD COMPRESS_LZW::LZW_GIF_Encode(XBYTE* dibbuffer, XBYTE* outbuffer, XDWORD dibwidth, XDWORD dibheight, XWORD colorbit)
{
  hash = GEN_NEW XWORD [LZW_MAX_HASHSIZE] ;

  if((hash == NULL) || (dibbuffer == NULL) || (outbuffer == NULL)) return 0;

  bymincode     = (colorbit == 1) ? 2 : colorbit;
  LZWclear      = 1 << bymincode;
  LZWend        = LZWclear + 1;
  origin        = curin = dibbuffer;
  curout        = outbuffer;
  byinbit       = 8;
  byoutbit      = 0;

  m_dibwidth    = dibwidth;
  m_dibheight   = dibheight;
  currpixel     = 0;
  currheight    = 0;
  pitch         = 4 * ((dibwidth * bymincode + 31) / 32);

  XWORD old;
  XBYTE pixel;

  this->Encode_InitStringTable();
  this->Encode_WriteIndex(LZWclear);

  old = this->Encode_GetNextPixel();

  while(currheight<m_dibheight)
    {
      pixel = this->Encode_GetNextPixel();

      if(this->Encode_IsInTable (old, pixel))
        {
          old = hash[(old << 8) | pixel];
        }
       else
        {
          this->Encode_WriteIndex(old);
          this->Encode_AddStringToTable(old, pixel);

          old = pixel ;
          if(currtableindex == LZW_MAX_TABLE_SIZE)
            {
              this->Encode_WriteIndex(pixel);
              this->Encode_WriteIndex(LZWclear);
              this->Encode_InitStringTable();

              old = this->Encode_GetNextPixel();
            }
        }
    }

  this->Encode_WriteIndex(old);
  this->Encode_WriteIndex(LZWend);

  GEN_DELETE_ARRAY hash;

  return (XDWORD)(curout - outbuffer + 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::LZW_GIF_Decode(XBYTE* inbuffer, XBYTE* dibbuffer, XDWORD dibwidth, XDWORD dibheight, bool binterlace)
* @brief      LZWGIF decode
* @ingroup    COMPRESS
* 
* @param[in]  inbuffer : Inbuffer pointer to use.
* @param[in]  dibbuffer : Dibbuffer pointer to use.
* @param[in]  dibwidth : Dibwidth value.
* @param[in]  dibheight : Dibheight value.
* @param[in]  binterlace : Binterlace value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::LZW_GIF_Decode(XBYTE* inbuffer, XBYTE* dibbuffer, XDWORD dibwidth, XDWORD dibheight, bool binterlace)
{
  strbegin = GEN_NEW LZW_STRING [LZW_MAX_TABLE_SIZE + 32];

  if((strbegin == NULL) || (inbuffer == NULL) || (dibbuffer == NULL)) return;

  curin       = inbuffer + 1;
  curout      = dibbuffer;
  byinbit     = 0;
  bymincode   = *inbuffer;
  LZWclear    = 1 << bymincode;
  LZWend      = LZWclear + 1;

  m_dibwidth  = dibwidth;
  currpixel   = 0;
  pitch       = 4 * ((dibwidth * 8 + 31) / 32);

  byinterval  = binterlace ? 0 : 0xFF;

  if(byinterval == 0)
    {
      origin      = curout;
      m_dibheight  = dibheight;
      currheight = 0;
    }

  XWORD code;
  XWORD old = 0;

  this->Decode_InitStringTable();

  while((code = this->Decode_GetNextcode ()) != LZWend)
    {
      if(code == LZWclear)
        {
          this->Decode_InitStringTable () ;
          while((code = this->Decode_GetNextcode ()) == LZWclear) ;
        }
       else
        {
          if(this->Decode_IsInTable (code))
                 this->Decode_AddStringToTable (old, this->Decode_GetFirstChar (code));
            else this->Decode_AddStringToTable (old, this->Decode_GetFirstChar (old));
        }

      this->Decode_WriteString_to8(code);
      old = code;
    }

  GEN_DELETE_ARRAY strbegin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Decode_InitStringTable()
* @brief      Decode init string table
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Decode_InitStringTable()
{
  memset(strbegin, 0xFF, LZW_MAX_TABLE_SIZE * sizeof(LZW_STRING));

  for (XWORD i = 0 ; i < LZWclear ; i++)
    strbegin[i].suffix = i ;

  currtableindex = LZWend + 1;
  bycurrbits     = bymincode + 1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD COMPRESS_LZW::Decode_GetNextcode()
* @brief      Decode get nextcode
* @ingroup    COMPRESS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD COMPRESS_LZW::Decode_GetNextcode()
{
  XDWORD       dwRet = 0;
  unsigned int uiAdd = byinbit + bycurrbits;

  if(uiAdd <= 8)
    {
      dwRet |= * curin;
    }
   else
    {
      if(uiAdd <= 16)
        {
          dwRet |= * (XWORD *) curin;
        }
       else
        {
          dwRet |= * (curin + 2);
          dwRet <<= 16;
          dwRet |= * (XWORD *) curin;
        }
    }

  curin += uiAdd/8;
  byinbit = uiAdd%8;

  dwRet <<= 32 - uiAdd ;
  dwRet >>= 32 - bycurrbits ;

  return (XWORD)dwRet ;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZW::Decode_IsInTable(XWORD code)
* @brief      Decode is in table
* @ingroup    COMPRESS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZW::Decode_IsInTable(XWORD code)
{
  return (code < currtableindex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Decode_AddStringToTable(XWORD prefix, XWORD suffix)
* @brief      Decode add string to table
* @ingroup    COMPRESS
* 
* @param[in]  prefix : Prefix value.
* @param[in]  suffix : Suffix value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Decode_AddStringToTable(XWORD prefix, XWORD suffix)
{
  strbegin[currtableindex  ].prefix = prefix ;
  strbegin[currtableindex++].suffix = suffix ;

  if((currtableindex == 0x008) || (currtableindex == 0x010) ||
     (currtableindex == 0x020) || (currtableindex == 0x040) ||
     (currtableindex == 0x080) || (currtableindex == 0x100) ||
     (currtableindex == 0x200) || (currtableindex == 0x400) ||
     (currtableindex == 0x800)) bycurrbits++ ;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE COMPRESS_LZW::Decode_GetFirstChar(XWORD code)
* @brief      Decode get first char
* @ingroup    COMPRESS
* 
* @param[in]  code : Code value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE COMPRESS_LZW::Decode_GetFirstChar(XWORD code)
{
  while (strbegin[code].prefix != 0xFFFF)
    {
      code = strbegin[code].prefix;
    }

  return (XBYTE) strbegin[code].suffix;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Decode_WriteString_to8(XWORD code)
* @brief      Decode write stringto8
* @ingroup    COMPRESS
* 
* @param[in]  code : Code value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Decode_WriteString_to8(XWORD code)
{
  if(code < LZWclear)
    {
      curout[currpixel++] = (XBYTE) strbegin[code].suffix;

      if(currpixel == m_dibwidth) this->Decode_SwitchToFollowLine();
    }
   else
    {
      this->Decode_WriteString_to8(strbegin[code].prefix);
      this->Decode_WriteString_to8(strbegin[code].suffix);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Decode_SwitchToFollowLine()
* @brief      Decode switch to follow line
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Decode_SwitchToFollowLine()
{
 currpixel = 0;

  if (byinterval == 0xFF)
    {
      curout -= pitch;
    }
   else
    {
      currheight += interval[byinterval];

      if(currheight >= m_dibheight)
        {
          if(++byinterval < 4)  currheight = interval[byinterval]/2;  else return;
        }

      curout = origin - currheight * pitch;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Encode_InitStringTable()
* @brief      Encode init string table
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Encode_InitStringTable()
{
  currtableindex = LZWend   + 1;
  bycurrbits     = bymincode + 1;

  memset(hash, 0x00, LZW_MAX_HASHSIZE * sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE COMPRESS_LZW::Encode_GetNextPixel()
* @brief      Encode get next pixel
* @ingroup    COMPRESS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE COMPRESS_LZW::Encode_GetNextPixel()
{
  XBYTE byRet = 0;

  switch(bymincode)
    {
      case 8 : byRet = curin[currpixel];
               break;

      case 4 : byRet = (currpixel % 2 == 0) ? curin[currpixel / 2] >> 4 : curin[currpixel / 2] & 0x0F;
               break;

      case 1 : byRet = 0x01 & (curin[currpixel / 8] >> (7 - (currpixel & 7)));
               break;
    }

  if(++currpixel == m_dibwidth)
    {
     currpixel = 0;

      currheight++;

      curin = origin - currheight * pitch;
    }

  return byRet;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZW::Encode_IsInTable(XWORD old, XWORD pixel)
* @brief      Encode is in table
* @ingroup    COMPRESS
* 
* @param[in]  old : Old value.
* @param[in]  pixel : Pixel value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZW::Encode_IsInTable(XWORD old, XWORD pixel)
{
  return (hash[(old << 8) | pixel] != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Encode_AddStringToTable(XWORD old, XWORD pixel)
* @brief      Encode add string to table
* @ingroup    COMPRESS
* 
* @param[in]  old : Old value.
* @param[in]  pixel : Pixel value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Encode_AddStringToTable(XWORD old, XWORD pixel)
{
  hash[(old << 8) | pixel] = currtableindex++;

  if ((currtableindex == 0x009) || (currtableindex == 0x011) ||
      (currtableindex == 0x021) || (currtableindex == 0x041) ||
      (currtableindex == 0x081) || (currtableindex == 0x101) ||
      (currtableindex == 0x201) || (currtableindex == 0x401) ||
      (currtableindex == 0x801))  bycurrbits++;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void COMPRESS_LZW::Encode_WriteIndex(XDWORD Index)
* @brief      Encode write index
* @ingroup    COMPRESS
* 
* @param[in]  Index : Index value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void COMPRESS_LZW::Encode_WriteIndex(XDWORD Index)
{
  unsigned int uiAdd = byoutbit + bycurrbits;

  *(XDWORD*)curout |= (Index << byoutbit);

  byoutbit = uiAdd % 8;
  curout += uiAdd / 8;
}



