/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProcESP32.h
* 
* @class      MAINPROCESP32
* @brief      Main Proc ESP32 class
* @ingroup    PLATFORM_ESP32
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

#include "XESP32Trace.h"

#include "MainProc.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSTRING;

class MAINPROCESP32 : public MAINPROC
{
  public:
                                  MAINPROCESP32         ();
    virtual                      ~MAINPROCESP32         ();

    virtual bool                  Ini                   (APPFLOWMAIN* appmain = NULL);
    virtual bool                  Update                ();
    virtual bool                  End                   ();

  private:

    virtual bool                  Factorys_Ini          ();
    virtual bool                  Factorys_End          ();

    void                          Clean                 ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
extern  XESP32TRACE     ESP32trace;
#endif




