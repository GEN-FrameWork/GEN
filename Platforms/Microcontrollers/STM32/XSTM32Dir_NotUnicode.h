/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Dir_NotUnicode.h
*
* @class      XSTM32DIR_NOTUNICODE
* @brief      STM32 eXtended Not Unicode Directory class
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <ff.h>

#include "XBase.h"
#include "XDir.h"
#include "XPath.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

typedef struct
{
  DIR       dir;
  FILINFO   fileinfo;
  
} XSTM32DIR_SEARCHINFO; 

class XSTM32DIR_NOTUNICODE : public XDIR
{
  public:
                        XSTM32DIR_NOTUNICODE      ();
    virtual            ~XSTM32DIR_NOTUNICODE      ();

    bool                Exist                     (XCHAR* path);
    bool                Make                      (XCHAR* path, bool recursive = true);
    bool                ChangeTo                  (XCHAR* path);
    bool                Delete                    (XCHAR* path, bool all);

    bool                GetActual                 (XPATH& xpath);

    bool                FirstSearch               (XCHAR* xpath, XCHAR* patternsearch, XDIRELEMENT* searchelement);
    bool                NextSearch                (XDIRELEMENT* searchelement);

  private:

    bool                ReconvertSearchData       (XDIRELEMENT* searchelement, FILINFO* fileinfo);
    
    void                Clean                     ();

    FATFS               userFATFS;
    FRESULT             fresult;
    bool                ismount;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

