/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBase.cpp
* 
* @class      XBASE
* @brief      Basic functions and defines
* @ingroup    XUTILS
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

#include "XBase.h"

#include <cmath>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBASE::XBASE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBASE::XBASE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBASE::~XBASE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBASE::~XBASE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBASE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBASE::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double Truncate(double value)
* @brief      Trucate double
* @ingroup    XUTILS
* 
* @param[in]  value : double to truncate
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double Truncate(double value)
{
  double truncate = 0;

  modf(value, &truncate);

  return truncate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double Fraction(double value)
* @brief      Return the fractional part of double
* @ingroup    XUTILS
* 
* @param[in]  value : double value to obtain fractional part
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double Fraction(double value)
{
  double truncate = 0;
  return modf(value, &truncate);
  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float RoundOff(float value, XBYTE prec)
* @brief      oundOff
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* @param[in]  prec : Prec value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float RoundOff(float value, XBYTE prec)
{
  double pow_10 = pow(10.0f, (double)prec);
  return (float)(round(value * pow_10) / pow_10);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double RoundOff(double value, XBYTE prec)
* @brief      RoundOff
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* @param[in]  prec : Prec value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double RoundOff(double value, XBYTE prec)
{
  double pow_10 = pow(10.0f, (float)prec);
  return round(value * pow_10) / pow_10;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float RoundFloat(float x)
* @brief      Round float
* @ingroup    XUTILS
* 
* @param[in]  x : float to round
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float RoundFloat(float x)
{
  return float(x >= 0.0f ? floor(x + 0.5f) : ceil(x - 0.5f));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float AdjustFloat(float valor, int decimals)
* @brief      AdjustFloat
* @ingroup    XUTILS
* 
* @param[in]  valor : Valor value.
* @param[in]  decimals : Decimals value.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float AdjustFloat(float valor, int decimals) 
{
  float factor = 1.0f;
  
  for(int i = 0; i < decimals; ++i) 
    {
      factor *= 10.0f;
    }

  float valorAjustado = (float)floor(valor * factor + 0.5f) / factor;
  
  return valorAjustado;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double AdjustDouble(double valor, int decimals)
* @brief      AdjustDouble
* @ingroup    XUTILS
* 
* @param[in]  valor : Valor value.
* @param[in]  decimals : Decimals value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double AdjustDouble(double valor, int decimals) 
{
  double factor = 1.0;

  for(int i = 0; i < decimals; ++i) 
    {
      factor *= 10.0;
    }

  double valorAjustado = floor(valor * factor + 0.5) / factor;

  return valorAjustado;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE HighWORD(XWORD data)
* @brief      HighWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE HighWORD(XWORD data)
{
  return ((data & 0xFF00) >> 8);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD HighDWORD(XDWORD data)
* @brief      HighDWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD HighDWORD(XDWORD data)
{
  return ((data & 0xFFFF0000) >> 16);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD HighQWORD(XQWORD data)
* @brief      HighQWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD HighQWORD(XQWORD data)
{
  return ((data & 0xFFFFFFFF00000000) >> 32);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE LowWORD(XWORD data)
* @brief      LowWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE LowWORD(XWORD data)
{
  return (XBYTE)(data & 0x00FF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD LowDWORD(XDWORD data)
* @brief      LowDWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD LowDWORD(XDWORD data)
{
  return (XWORD)(data & 0x0000FFFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD LowQWORD(XQWORD data)
* @brief      Low_QWORD
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD LowQWORD(XQWORD data)
{
  return (XDWORD)(data & 0x00000000FFFFFFFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD SwapWORD(XWORD data)
* @brief      Return the swap of word (2 bytes) hi-lo  > lo-hi
* @ingroup    XUTILS
* 
* @param[in]  data : word to swap
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD SwapWORD(XWORD data)
{
  const XWORD hi = (XWORD)((data&0xFF00)>>8);
  const XWORD lo = (XWORD)((data&0x00FF)<<8);

  return (XWORD)(hi|lo);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SwapDWORD(XDWORD data)
* @brief      Swap double word (4 bytes) hi1 lo1 h2 lo2 >  lo2 hi2 lo1 hi1
* @ingroup    XUTILS
* 
* @param[in]  data : dword to swap
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SwapDWORD(XDWORD data)
{
  const XDWORD hi1 = (data&0xFF000000)>>24;
  const XDWORD lo1 = (data&0x00FF0000)>>8;
  const XDWORD hi2 = (data&0x0000FF00)<<8;
  const XDWORD lo2 = (data&0x000000FF)<<24;

  return (hi1|lo1|hi2|lo2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD SwapQWORD(XQWORD data)
* @brief      Swap quad word (8 bytes) hi1 lo1 h2 lo2 hi3 lo3 h4 lo4  > lo3 hi3 lo4 hi4 lo2 hi2 lo1 hi1
* @ingroup    XUTILS
* 
* @param[in]  data : quad word to swap
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD SwapQWORD(XQWORD data)
{
  const XQWORD hi1 = (data&0xFF00000000000000ULL) >> (24 + 32);
  const XQWORD lo1 = (data&0x00FF000000000000ULL) >> ( 8 + 32);
  const XQWORD hi2 = (data&0x0000FF0000000000ULL) >> (     24);
  const XQWORD lo2 = (data&0x000000FF00000000ULL) >> (      8);

  const XQWORD hi3 = (data&0x00000000FF000000ULL) << (      8);
  const XQWORD lo3 = (data&0x0000000000FF0000ULL) << (     24);
  const XQWORD hi4 = (data&0x000000000000FF00ULL) << ( 8 + 32);
  const XQWORD lo4 = (data&0x00000000000000FFULL) << (24 + 32);

  return (hi1|lo1|hi2|lo2|hi3|lo3|hi4|lo4);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int InvertSign(int number)
* @brief      nvertSign
* @ingroup    XUTILS
* 
* @param[in]  number : Number value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int InvertSign(int number) 
{
  return -number;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DWORDToBCD(XDWORD dword)
* @brief      Convert double word to BCD
* @ingroup    XUTILS
* 
* @param[in]  dword : double word to convert
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DWORDToBCD(XDWORD dword)
{
  XQWORD shift  = 16;
  XQWORD result = (dword % 10);

  while((dword = (dword/10)))
    {
      result += (dword % 10) * shift;
      shift *= 16; // weirdly, it's not possible to left shift more than 32 bits
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD BCDToDWORD(XQWORD bcd)
* @brief      Convert BCD to double word
* @ingroup    XUTILS
* 
* @param[in]  bcd : BCD to convert
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD BCDToDWORD(XQWORD bcd)
{
  XQWORD mask = 0x000f;
  XQWORD pwr  = 1;
  XQWORD i    = (bcd & mask);

  while((bcd = (bcd >> 4)))
    {
      pwr *= 10;
      i += (bcd & mask) * pwr;
    }

  return (XDWORD)i;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE RotateBitLeft(XBYTE byte)
* @brief      RotateBitLeft
* @ingroup    XUTILS
* 
* @param[in]  byte : Byte value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE RotateBitLeft(XBYTE byte)
{
  return (byte << 1) | (byte >> 7);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE RotateBitRight(XBYTE byte)
* @brief      RotateBitRight
* @ingroup    XUTILS
* 
* @param[in]  byte : Byte value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE RotateBitRight(XBYTE byte)
{
  return (byte >> 1) | (byte << 7);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE RotateBitReverse(XBYTE byte)
* @brief      RotateBitReverse
* @ingroup    XUTILS
* 
* @param[in]  byte : Byte value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE RotateBitReverse(XBYTE byte)
{
  byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
  byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
  byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;

  return byte;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SizeBufferASCII(XBYTE* buffer, int sizemax)
* @brief      izeBufferASCII
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  sizemax : Sizemax value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SizeBufferASCII(XBYTE* buffer, int sizemax)
{
  int _max = sizemax;

  if(_max > 65535) 
    {
      _max = 65535;
    }

  int size = 0;   
  for(int c=0; c<_max; c++)
    {
      if(buffer[c]) 
        {
          size++;
        }
       else 
        {
          break;
        } 
    }

  return size;
}




