/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Common_DefaultXTraceCFG.h
* 
* @class      COMMON_DEFAULTXTRACECFG
* @brief      GEN Default XTrace Config 
* @ingroup    COMMON
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

#define  GEN_XTRACE_NET_DEFAULT_01            _L("xtracegen.dyndns.org:10001")
#define  GEN_XTRACE_NET_DEFAULT_02            _L("xtracegen.dyndns.org:10002")
#define  GEN_XTRACE_NET_DEFAULT_03            _L("xtracegen.dyndns.org:10003")
#define  GEN_XTRACE_NET_DEFAULT_04            _L("xtracegen.dyndns.org:10004")
#define  GEN_XTRACE_NET_DEFAULT_05            _L("xtracegen.dyndns.org:10005")

#ifdef XTRACE_ACTIVE

  #define  GEN_XTRACE_NET_CFG_DEFAULT_01      if(tracetargets.Get(0)) tracetargets.Get(0)->Format(_L("%d,%s"), XTRACE_TYPE_NET, GEN_XTRACE_NET_DEFAULT_01);
  #define  GEN_XTRACE_NET_CFG_DEFAULT_02      if(tracetargets.Get(1)) tracetargets.Get(1)->Format(_L("%d,%s"), XTRACE_TYPE_NET, GEN_XTRACE_NET_DEFAULT_02);
  #define  GEN_XTRACE_NET_CFG_DEFAULT_03      if(tracetargets.Get(2)) tracetargets.Get(2)->Format(_L("%d,%s"), XTRACE_TYPE_NET, GEN_XTRACE_NET_DEFAULT_03);
  #define  GEN_XTRACE_NET_CFG_DEFAULT_04      if(tracetargets.Get(3)) tracetargets.Get(3)->Format(_L("%d,%s"), XTRACE_TYPE_NET, GEN_XTRACE_NET_DEFAULT_04);
//#define  GEN_XTRACE_NET_CFG_DEFAULT_05      if(tracetargets.Get(4)) tracetargets.Get(4)->Format(_L("%d,%s"), XTRACE_TYPE_NET, GEN_XTRACE_NET_DEFAULT_05);
  #define  GEN_XTRACE_NET_CFG_DEFAULT_LOCAL   if(tracetargets.Get(4)) tracetargets.Get(4)->Format(_L("%d,*:10001"), XTRACE_TYPE_NET);

#else

  #define  GEN_XTRACE_NET_CFG_DEFAULT_01      
  #define  GEN_XTRACE_NET_CFG_DEFAULT_02      
  #define  GEN_XTRACE_NET_CFG_DEFAULT_03      
  #define  GEN_XTRACE_NET_CFG_DEFAULT_04      
  #define  GEN_XTRACE_NET_CFG_DEFAULT_05      
  #define  GEN_XTRACE_NET_CFG_DEFAULT_LOCAL   

#endif



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





