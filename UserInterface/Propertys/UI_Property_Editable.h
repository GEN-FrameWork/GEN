/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Editable.h
* 
* @class      UI_PROPERTY_EDITABLE
* @brief      User Interface Property Editable class
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

#include "XBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class UI_PROPERTY_EDITABLE
{
  public:
														   UI_PROPERTY_EDITABLE           ();
    virtual                   ~UI_PROPERTY_EDITABLE	          ();

    XDWORD                     Cursor_GetPosition             ();      	
    void                       Cursor_SetPosition             (XDWORD cursorposition);      	

    XDWORD                     GetMaxSize                     ();      	
    void                       SetMaxSize                     (XDWORD maxsize);

    // Text selection: sel_anchor is the fixed end; cursor_position is the active end.
    XDWORD                     Selection_GetAnchor            ();
    void                       Selection_SetAnchor            (XDWORD anchor);
    bool                       Selection_Has                  ();
    void                       Selection_Clear                ();
    void                       Selection_SelectAll            (XDWORD textsize);
    void                       Selection_GetNormalized        (XDWORD& start, XDWORD& end);
    bool                       Selection_GetText              (class XSTRING* source, class XSTRING& out);
    bool                       Selection_Delete               (class XSTRING* source);

  private:

		void										   Clean										      ();

    XDWORD                     cursor_position;
    XDWORD                     maxsize;
    XDWORD                     sel_anchor;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




