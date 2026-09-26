/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       mallocstats.h
* 
* @class      MALLOCSTATS
* @brief      MALLOCSTATS
* @ingroup    PLATFORM_STM32
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

#ifdef __cplusplus
extern "C"
{
#endif
    
/* ------ Declarations copied from arm\src\lib\<dlib\heap>\dlmalloc.c ----*/
/* ------ MAKE SURE to copy correct/current content from dlmalloc.c ------*/

#define MALLINFO_FIELD_TYPE size_t

#define STRUCT_MALLINFO_DECLARED 1
struct mallinfo {
  MALLINFO_FIELD_TYPE arena;
  MALLINFO_FIELD_TYPE ordblks;
  MALLINFO_FIELD_TYPE smblks;
  MALLINFO_FIELD_TYPE hblks;
  MALLINFO_FIELD_TYPE hblkhd;
  MALLINFO_FIELD_TYPE usmblks;
  MALLINFO_FIELD_TYPE fsmblks;
  MALLINFO_FIELD_TYPE uordblks;
  MALLINFO_FIELD_TYPE fordblks;
  MALLINFO_FIELD_TYPE keepcost;
};
/* ------------------------------------------------------------------------*/

struct mallinfo __iar_dlmallinfo(void);
void __iar_dlmalloc_stats(void);

#ifdef __cplusplus
}
#endif

