/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Image.h
* 
* @class      UI_ELEMENT_IMAGE
* @brief      User Interface Element Image class
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

#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;


class UI_ELEMENT_IMAGE : public UI_ELEMENT
{
  public:
																		  UI_ELEMENT_IMAGE					  ();
    virtual													 ~UI_ELEMENT_IMAGE					  ();

    GRPBITMAP*                        GetImage                    ();  
    bool                              SetImage                    (GRPBITMAP* image);

    XSTRING*                          GetResource                 ();
    void                              SetResource                 (XCHAR* resource);

    XBYTE                             GetAlpha                    (); 
    void                              SetAlpha                    (XBYTE alpha); 

  private:

		void															Clean												();

    GRPBITMAP*                        image;
    XSTRING                           resource;
    XBYTE                             alpha;
 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





