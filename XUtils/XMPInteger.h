/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XMPInteger.h
* 
* @class      XMPINTEGER
* @brief      eXtended Utils Multi-Precision Integer class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XBuffer.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef XDWORD XLIMB;
typedef XQWORD XUDBL;

typedef bool (*XMPINTEGER_FUNCRANDOM)   (XBYTE* buffer, XDWORD size, void* param);


#define XMPINTEGER_MAXLIMBS           10000
#define XMPINTEGER_WINDOWSIZE         6

#define XMPINTEGER_MAXSIZE            512                                 // Maximum number of bytes for usable MPIs.
#define XMPINTEGER_MAXBITS            (8*XMPINTEGER_MAXSIZE)

#define XMPINTEGER_CHARSINLIMB        sizeof(XLIMB)                       // chars in limb
#define XMPINTEGER_BITSINLIMB         (XMPINTEGER_CHARSINLIMB << 3)       // bits  in limb
#define XMPINTEGER_HALFLIMBSIZE       (XMPINTEGER_CHARSINLIMB << 2)       // half limb size

#define XMPINTEGER_BITSTOLIMBS(i)     (((i) + XMPINTEGER_BITSINLIMB  - 1) / XMPINTEGER_BITSINLIMB)
#define XMPINTEGER_CHARSTOLIMBS(i)    (((i) + XMPINTEGER_CHARSINLIMB - 1) / XMPINTEGER_CHARSINLIMB)


#define XMPINTEGER_MULADDC_INIT       \
                                      XLIMB s0, s1, b0, b1;                                               \
                                      XLIMB r0, r1, rx, ry;                                               \
                                      b0 = ( b << XMPINTEGER_HALFLIMBSIZE ) >> XMPINTEGER_HALFLIMBSIZE;   \
                                      b1 = ( b >> XMPINTEGER_HALFLIMBSIZE );

#define XMPINTEGER_MULADDC_CORE       \
                                      s0 = ( *s << XMPINTEGER_HALFLIMBSIZE ) >> XMPINTEGER_HALFLIMBSIZE;  \
                                      s1 = ( *s >> XMPINTEGER_HALFLIMBSIZE ); s++;                        \
                                      rx = s0 * b1; r0 = s0 * b0;                                         \
                                      ry = s1 * b0; r1 = s1 * b1;                                         \
                                      r1 += ( rx >> XMPINTEGER_HALFLIMBSIZE );                            \
                                      r1 += ( ry >> XMPINTEGER_HALFLIMBSIZE );                            \
                                      rx <<= XMPINTEGER_HALFLIMBSIZE; ry <<= XMPINTEGER_HALFLIMBSIZE;     \
                                      r0 += rx; r1 += (r0 < rx);                                          \
                                      r0 += ry; r1 += (r0 < ry);                                          \
                                      r0 +=  c; r1 += (r0 <  c);                                          \
                                      r0 += *d; r1 += (r0 < *d);                                          \
                                      c = r1; *(d++) = r0;

#define XMPINTEGER_MULADDC_STOP       \

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMPINTEGER
{
  public:
                                XMPINTEGER                    ();
    virtual                    ~XMPINTEGER                    ();

    int                         GetSign                       ();
    XDWORD                      GetNLimbs                     ();
    XLIMB*                      GetLimbs                      ();

    void                        SetSign                       (int sign);
    void                        SetNLimbs                     (XDWORD nlimbs);
    void                        SetLimbs                      (XLIMB* limbs);

    bool                        Ini                           ();

    bool                        Grow                          (XDWORD nblimbs);
    bool                        Shrink                        (XDWORD nblimbs);
    bool                        CopyFrom                      (XMPINTEGER* xmpinteger);
    void                        Swap                          (XMPINTEGER* xmpinteger);

    bool                        LeftSet                       (int z);

    int                         GetBit                        (XDWORD position);
    bool                        SetBit                        (XDWORD position, XBYTE value);

    int                         GetLSB                        ();
    int                         GetMSB                        ();

    XDWORD                      GetSize                       ();

    bool                        ImportFromBinary              (XBYTE* buffer, XDWORD size);
    bool                        ExportToBinary                (XBYTE* buffer, XDWORD size);

    bool                        LeftShift                     (XDWORD count);
    bool                        RightShift                    (XDWORD count);

    int                         CompareABSValues              (XMPINTEGER& xmpinteger);
    int                         CompareSignedValues           (XMPINTEGER& xmpinteger);
    int                         CompareSignedValues           (int integer);

    bool                        AdditionABS                   (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        SubtractionABS                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);

    bool                        AdditionSigned                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        SubtractionSigned             (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);

    bool                        AdditionSigned                (XMPINTEGER* xmpinteger, int integer);
    bool                        SubtractionSigned             (XMPINTEGER* xmpinteger, int integer);

    bool                        Multiplication                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        Multiplication                (XMPINTEGER* xmpinteger, XDWORD integer);

    bool                        Division                      (XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB);
    bool                        Division                      (XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, int integerB);

    bool                        Module                        (XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB);
    bool                        Module                        (XLIMB* r, XMPINTEGER* xmpintegerA, int integerb);

    void                        MontgomeryIni                 (XLIMB *mm, XMPINTEGER* N);
    void                        MontgomeryMultiplication      (XMPINTEGER* A, XMPINTEGER* B, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T);
    void                        MontgomeryReduction           (XMPINTEGER* A, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T);
    bool                        SlidingWindowExponentiation   (XMPINTEGER* A, XMPINTEGER* E, XMPINTEGER* N, XMPINTEGER* _RR);

    bool                        GreatestCommonDivisor         (XMPINTEGER* G, XMPINTEGER* A, XMPINTEGER* B);

    int                         FillRandom                    (XDWORD size, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandon);

    int                         ModularInverse                (XMPINTEGER* A, XMPINTEGER* N);

    int                         CheckSmallFactors             ();
    bool                        MillerRabin                   (XMPINTEGER* X, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    int                         IsPrime                       (XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);
    bool                        GeneratePrime                 (int nbits, int dhflag, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    bool                        SetFromString                 (int radix, XCHAR* string);
    bool                        SetFromString                 (int radix, XSTRING& string);
    bool                        GetToString                   (int radix, XSTRING& string);

    bool                        SetFromXBuffer                (XBUFFER& xbuffer);
    bool                        GetToXBuffer                  (XBUFFER& xbuffer, XDWORD size);

    bool                        End                           ();

  private:

    void                        Clean                         ();

    void                        SubtractionHelper             (XDWORD n, XLIMB *s, XLIMB *d);
    void                        MultiplicationHelper          (XDWORD i, XLIMB* s, XLIMB* d, XLIMB b);

    bool                        GetDigit                      (XDWORD* data, int radix, XCHAR character);
    bool                        SetToStringHelper             (int radix, XSTRING& string);

    int                         sign;
    XDWORD                      nlimbs;
    XLIMB*                      limbs;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





