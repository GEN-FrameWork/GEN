/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFString.h
* 
* @class      XFSTRING
* @brief      Unicode Fast String class
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

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifdef MICROCONTROLLER
#error "this class should not be used on microcontrollers"
#endif

#pragma warning( push )
#pragma warning( disable : 4996)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;

class XFSTRING : public XSTRING
{
  public:
                        XFSTRING                  ();
                        XFSTRING                  (int n);
                        XFSTRING                  (const XCHAR* string);
                        XFSTRING                  (XCHAR* string);
                        XFSTRING                  (const char* string);
                        XFSTRING                  (XSTRING& string);

    int                 Fast_ConvertToInt         (int index = 0);
    XDWORD              Fast_ConvertToDWord       (int index = 0);
    XQWORD              Fast_ConvertToQWord       (int index = 0);
    XQWORDSIG           Fast_ConvertToSQWord      (int index = 0);
    float               Fast_ConvertToFloat       (int index = 0);
    double              Fast_ConvertToDouble      (int index = 0);
    bool                Fast_AddCharacter         (XCHAR character);

    bool                Fast_Empty                ();

    void                operator =                (const char* string);
    void                operator =                (const XCHAR* string);
    void                operator =                (const XSTRING& string);
    void                operator =                (XWORD* string);
    void                operator =                (XBUFFER& string);

    void                operator +=               (const char* string);
    void                operator +=               (const XCHAR* string);
    void                operator +=               (XSTRING& string);
    void                operator +=               (XCHAR character);

    bool                operator <                (const XSTRING& string) const;
    bool                operator >                (XSTRING& string);
    bool                operator <=               (XSTRING& string);
    bool                operator >=               (XSTRING& string);
    bool                operator ==               (const XSTRING& string) const;
    bool                operator !=               (XSTRING& string);

    XCHAR               operator []               (int position);

    int                 Scan                      (const XCHAR* mask, ...);
    bool                Explode                   (XCHAR token, XVECTOR<XFSTRING*>* receive);
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



#pragma warning(pop)





