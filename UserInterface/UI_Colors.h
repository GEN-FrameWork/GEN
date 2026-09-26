/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Colors.h
* 
* @class      UI_COLORS
* @brief      User Interface Colors class
* @ingroup    USERINTERFACE
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

#include "XString.h"
#include "XMap.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GEN_UI_COLORS  UI_COLORS::GetInstance()

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_COLORS
{
  public:

    static bool                     GetIsInstanced         ();
    static UI_COLORS&               GetInstance            ();
    static bool                     DelInstance            ();

    bool                            Add                    (XCHAR* name, XCHAR* colorstr);
    bool                            Add                    (XSTRING& name, XSTRING& colorstr);

    XSTRING*                        Get                    (XCHAR* name);
    XSTRING*                        Get                    (XSTRING& name);

    bool                            DeleteAll              ();
  

  private:
                                    UI_COLORS              ();   
                                    UI_COLORS              (UI_COLORS const&);
    virtual                        ~UI_COLORS              ();

    void                            operator =             (UI_COLORS const&);

    void                            Clean                  ();

    static UI_COLORS*               instance;

    XMAP<XSTRING*,XSTRING*>         colors;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





