/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CompressLZRW1KH.cpp
* 
* @class      COMPRESSLZRW1KH
* @brief      Compress LZRW1/KH class
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

#include "CompressLZRW1KH.h"

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         COMPRESS_LZRW1KH::COMPRESS_LZRW1KH()
* @brief      Constructor of class
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
COMPRESS_LZRW1KH::COMPRESS_LZRW1KH()
{
  SetType(COMPRESSBASE_TYPE_LZRW1KH);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         COMPRESS_LZRW1KH::~COMPRESS_LZRW1KH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    COMPRESS
* 
* --------------------------------------------------------------------------------------------------------------------*/
COMPRESS_LZRW1KH::~COMPRESS_LZRW1KH()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZRW1KH::Compress(XBYTE* origin, XDWORD size, XBUFFER* buffer)
* @brief      Compress
* @ingroup    COMPRESS
* 
* @param[in]  origin : Origin pointer to use.
* @param[in]  size : Size value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZRW1KH::Compress(XBYTE* origin, XDWORD size, XBUFFER* buffer)
{
  param1  =  NULL;

  buffer->SetSize(CompressionBuffer(origin, buffer->Get(), (XDWORD)size));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin, XDWORD size, XBUFFER* buffer)
* @brief      Decompress
* @ingroup    COMPRESS
* 
* @param[in]  origin : Origin pointer to use.
* @param[in]  size : Size value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin, XDWORD size, XBUFFER* buffer)
{
  param1  =  NULL;

  buffer->SetSize(DecompressionBuffer(origin, buffer->Get(), (XDWORD)size));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZRW1KH::Compress(XBYTE* origin, XDWORD size, void* param1)
* @brief      Compress
* @ingroup    COMPRESS
* 
* @param[in]  origin : Origin pointer to use.
* @param[in]  size : Size value.
* @param[in]  param1 : Param1 pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZRW1KH::Compress(XBYTE* origin, XDWORD size, void* param1)
{
  this->param1  =  param1;

  CompressionBuffer(origin, NULL, (XDWORD)size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin, XDWORD size, void* param1)
* @brief      Decompress
* @ingroup    COMPRESS
* 
* @param[in]  origin : Origin pointer to use.
* @param[in]  size : Size value.
* @param[in]  param1 : Param1 pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin, XDWORD size, void* param1)
{
  this->param1  =  param1;

  DecompressionBuffer(origin, NULL, (XDWORD)size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE COMPRESS_LZRW1KH::GetMatch(XBYTE* source, XDWORD x, XDWORD sourcesize, XDWORD* hash, XDWORD* size, XDWORD* pos)
* @brief      Get match
* @ingroup    COMPRESS
* 
* @param[in]  source : Source pointer to use.
* @param[in]  x : X coordinate.
* @param[in]  sourcesize : Sourcesize value.
* @param[in]  hash : Hash pointer to use.
* @param[in]  size : Size value.
* @param[in]  pos : Pos pointer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE COMPRESS_LZRW1KH::GetMatch(XBYTE* source, XDWORD x, XDWORD sourcesize, XDWORD* hash, XDWORD* size, XDWORD* pos)
{
  XWORD hashvalue = (XWORD)((40543L*((((source[x] << 4) ^ source[x+1]) << 4) ^ source[x+2]) >> 4) & 0xfff);

  (*pos) = hash[hashvalue];

  hash[hashvalue] = x;

  if(((*pos)!=-1)&&((x-*pos) < 4096))
    {
      for ((*size) = 0; (((*size) < 18)&& (source[x+(*size)] == source[(*pos)+(*size)]) && ((x+*size) < sourcesize)); (*size)++)
        {

        }

      return ((*size) >= 3);
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD COMPRESS_LZRW1KH::CompressionBuffer(XBYTE* source, XBYTE* target, XDWORD sourcesize)
* @brief      Compression buffer
* @ingroup    COMPRESS
* 
* @param[in]  source : Source pointer to use.
* @param[in]  target : Target pointer to use.
* @param[in]  sourcesize : Sourcesize value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD COMPRESS_LZRW1KH::CompressionBuffer(XBYTE* source, XBYTE* target, XDWORD sourcesize)
{
  XDWORD  key;
  XDWORD  size;
  XDWORD  pos;
  XBYTE   bit      = 0;
  XWORD   command  = 0;
  XDWORD  x        = 0;
  XDWORD  y        = 3;
  XDWORD  z        = 1;

  for(key = 0; key < 4096; key++)
    {
      hash[key] = 0x80000000;
    }

  CMPTARGET(true, 0, CMPFLAG_COMPRESS);

  for(;(x<sourcesize) && (y<=sourcesize);)
    {
      if(bit>15)
        {
          CMPTARGET(true, z++, (XBYTE)((command >> 8) & 0x00ff));
          CMPTARGET(true, z, (XBYTE)(command & 0x00ff));

          z = y;
          bit = 0;
          y += 2;
        }

      for(size=1; (source[x] == source[x+size]) && (size < 0x0fff) && (x+size < sourcesize); size++)
        {

        }

      if(size >= 16)
        {
          CMPTARGET(true, y++, 0);
          CMPTARGET(true, y++, (XBYTE)(((size - 16) >> 8) & 0x00ff));
          CMPTARGET(true, y++, (XBYTE)((size - 16) & 0x00ff));
          CMPTARGET(true, y++, source[x]);

          x=(x+size);
          command = (XWORD)((command << 1) + 1);
        }
       else
        {
          if(GetMatch(source, x, sourcesize, hash, &size, (XDWORD*)&pos))
            {
              key = (((x-pos) << 4) + (size - 3));

              CMPTARGET(true, y++, (XBYTE)((key >> 8) & 0x00ff));
              CMPTARGET(true, y++, (XBYTE)(key & 0x00ff));

              x = (x+size);
              command = (XWORD)((command << 1) + 1);
            }
           else
            {
              CMPTARGET(true, y++, source[x++]);

              command = (XWORD)((command << 1));
            }
        }

      bit++;
   }

  command <<= (16-bit);

  CMPTARGET(true, z++, (XBYTE)((command >> 8) & 0x00ff));
  CMPTARGET(true, z, (XBYTE)(command & 0x00ff));

  if(y > sourcesize)
    {
      // FIXED: the original loop invoked CMPTARGET(true, y+1, source[y++]) as the for-loop's own
      // increment expression, i.e. target[y+1] = source[y++]. Both the index (y+1) and the data
      // (source[y++], which side-effects y) referenced the same variable with no sequence point
      // between them (the original "#Imanol order of execution not guaranteed, y could have weird
      // values" comment already flagged this), which is unsequenced modification/access UB and, on
      // this toolchain, a confirmed live data-corruption bug (see UnitTests_Compress' findings for
      // COMPRESS_LZRW1KH). Moving the increment of 'y' into the for-loop's own increment clause
      // keeps the exact same target[y+1] = source[y] assignment for y=0..sourcesize-1, now fully
      // sequenced.
      for(y=0; y<sourcesize; y++)
        {
          CMPTARGET(true, y+1, source[y]);
        }

      CMPTARGET(true, 0, CMPFLAG_COPIED);

      return (sourcesize+1);
    }

  return y;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD COMPRESS_LZRW1KH::DecompressionBuffer(XBYTE *source, XBYTE *target, XDWORD sourcesize)
* @brief      Decompression buffer
* @ingroup    COMPRESS
* 
* @param[in]  source : Source pointer to use.
* @param[in]  target : Target pointer to use.
* @param[in]  sourcesize : Sourcesize value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD COMPRESS_LZRW1KH::DecompressionBuffer(XBYTE *source, XBYTE *target, XDWORD sourcesize)
{
  XDWORD    x       = 3;
  XDWORD    y       = 0;
  XDWORD    pos;
  XDWORD    size;
  XDWORD     k;
  XDWORD    command = ((source[1] << 8) + source[2]);
  XBYTE     bit     = 16;

  if(source[0]==CMPFLAG_COPIED)
    {
      // FIXED: same unsequenced-modification hazard as CompressionBuffer()'s mirroring copy-fallback
      // loop above (target[y-1] = source[y++] as the for-loop's increment expression). Moving the
      // increment of 'y' into the for-loop's own increment clause keeps the exact same
      // target[y-1] = source[y] assignment for y=1..sourcesize-1, now fully sequenced.
      for(y=1; y<sourcesize; y++)
        {
          CMPTARGET(false, y-1, source[y]);
        }

      return (sourcesize-1);
    }

  for(;x<sourcesize;)
    {
      if(bit == 0)
        {
          command  = ((source[x++] << 8));
          command  = (command + source[x++]);
          bit = 16;
        }

      if(command&0x8000)
        {
          pos = (source[x++] << 4);
          pos = (pos + (source[x] >> 4));

          if(pos)
            {
              size = ((source[x++] & 0x0f)+3);

              for(k=0;k<size;k++)
                {
                  CMPTARGET(false, y+k, target[y-pos+k]);
                }

              y = (y+size);
            }
           else
            {
              size = (source[x++] << 8);
              size = (size + source[x++] + 16);

              for (k=0;k<size;CMPTARGET(false, y+k++, source[x]))
                {

                }

              x++;
              y= (y+size);
            }

        }
       else
        {
          CMPTARGET(false, y++, source[x++]);
        }

       command <<= 1;
       bit--;
    }

 return y;
}



