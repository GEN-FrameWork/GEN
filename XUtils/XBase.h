/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBase.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#ifndef XBYTE
typedef unsigned char         XBYTE;
#endif

#ifndef XWORD
typedef unsigned short        XWORD;
#endif

#ifndef XDWORD
typedef unsigned int          XDWORD;
#endif

#ifndef XQWORD
typedef unsigned long long    XQWORD;
#endif


#ifndef XBYTESIG
typedef signed char           XBYTESIG;
#endif

#ifndef XWORDSIG
typedef signed short          XWORDSIG;
#endif

#ifndef XDWORDSIG
typedef signed int            XDWORDSIG;
#endif

#ifndef XQWORDSIG
typedef signed long long      XQWORDSIG;
#endif

#ifndef XREAL
typedef double                XREAL;
#endif

#ifndef POINTER
typedef void*                 POINTER;
#endif


#ifndef NULL
#define NULL                  0
#endif

#ifndef TRUE
#define TRUE                  1
#endif

#ifndef FALSE
#define FALSE                 0
#endif


#define NOTFOUND             -1

#define ISEQUAL               0
#define ISGREATER             1
#define ISLESS               -1

#define COMMENT SLASH(/)
#define SLASH(s) /##s

#ifndef __MAX
#define __MAX(a,b)            (((a)>(b))?(a):(b))
#endif

#ifndef __MIN
#define __MIN(a,b)            (((a)<(b))?(a):(b))
#endif

#ifdef MICROCONTROLLER
#define _MAXSTR               32
#else
#define _MAXSTR               256
#endif
#define _MAXPATH              _MAXSTR
#define _MAXBUFFER            _MAXSTR

#define PI                    3.141592654f

#ifndef MPI
#define MPI                   1.570796327f
#endif

#define EPSILON               0.00000000001f

#define DEGREE2RADIAN(c)      ((c*PI)/180.0f)//((c / 2.0f / 180.0f) * PI)
#define RADIAN2DEGREE(c)      ((c * 180.0f) / PI)

#define TRUNCATE(number)      (number - fmod(number, 0.01))

#define SWAPWORD(data)        data = SwapWORD(data);
#define SWAPDWORD(data)       data = SwapDWORD(data);
#define SWAPQWORD(data)       data = SwapQWORD(data);

#define SWAPVAR(a, b)          a = a ^ b; \
                               b = a ^ b; \
                               a = a ^ b;  

            
#if defined(APPMODE_LIBRARY_DINAMIC_ACTIVE)

  #if defined(WINDOWS)
    #define GEN_API_LIB_EXP   __declspec(dllexport)  
    #define GEN_API_LIB_IMP   __declspec(dllimport)
  #endif  

  #if defined(LINUX)  
    #include <dlfcn.h> 
    #define GEN_API_LIB_EXP   __attribute__((visibility("default")))
    #define GEN_API_LIB_IMP
  #endif

 #if !defined(WINDOWS) && !defined(LINUX)      
    #define GEN_API_LIB_EXP
    #define GEN_API_LIB_IMP  
  #endif
  
#else
  #define GEN_API_LIB_EXP
  #define GEN_API_LIB_IMP  
#endif


typedef bool (*PROGRESS_FUNCTION)(void);

#ifdef _MSC_VER    
  #define  TYPEDEF_STRUCT_ALIGNMENT_BYTE_INI                                            \
                                                      _Pragma("pack(push)")             \
                                                      _Pragma("pack(1)")                \
                                                      typedef struct
#endif  


#ifdef __GNUC__                                                          
  #define  TYPEDEF_STRUCT_ALIGNMENT_BYTE_INI          typedef struct __attribute__ ((__packed__))
#endif
 

#ifdef _MSC_VER    
 #define  TYPEDEF_STRUCT_ALIGNMENT_BYTE_END           _Pragma("pack(pop)")                                                          
#else
 #define  TYPEDEF_STRUCT_ALIGNMENT_BYTE_END
#endif


#define GEN_MODULE_EXEC   __FILE__
#define GEN_LINE_EXEC     __LINE__

#ifdef COMPILER_MSVC
  #define GEN_FUNCTION_EXEC  __FUNCSIG__
#else
  #ifdef COMPILER_GCC
    #define GEN_FUNCTION_EXEC  __PRETTY_FUNCTION__
  #else
    #ifdef COMPILER_CLANG
      #define GEN_FUNCTION_EXEC  __PRETTY_FUNCTION__
    #else 
      #define GEN_FUNCTION_EXEC  ""  
    #endif
  #endif 
#endif 




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XBASE
{
  public:
                     XBASE   ();
    virtual         ~XBASE   ();

  protected:

  private:

    void             Clean   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


double    Truncate            (double value);
double    Fraction            (double value);

float     RoundOff            (float value , XBYTE prec);
double    RoundOff            (double value, XBYTE prec);

float     RoundFloat          (float x);

float     AdjustFloat         (float valor, int decimals);
double    AdjustDouble        (double valor, int decimals);

XBYTE     HighWORD            (XWORD data);
XWORD     HighDWORD           (XDWORD data);
XDWORD    HighQWORD           (XQWORD data);

XBYTE     LowWORD             (XWORD data);
XWORD     LowDWORD            (XDWORD data);
XDWORD    LowQWORD            (XQWORD data);

XWORD     SwapWORD            (XWORD data);
XDWORD    SwapDWORD           (XDWORD data);
XQWORD    SwapQWORD           (XQWORD data);

int       InvertSign          (int number);

XQWORD    DWORDToBCD          (XDWORD dword);
XDWORD    BCDToDWORD          (XQWORD bcd);

XBYTE     RotateBitLeft       (XBYTE byte);
XBYTE     RotateBitRight      (XBYTE byte);
XBYTE     RotateBitReverse    (XBYTE byte);

int       SizeBufferASCII     (XBYTE* buffer, int sizemax = 65535);




