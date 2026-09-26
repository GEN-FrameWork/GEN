/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPDevice.h
* 
* @class      INPDEVICE
* @brief      Input device class
* @ingroup    INPUT
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

#include "INPButton.h"
#include "INPCursor.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum INPDEVICE_TYPE
{
  INPDEVICE_TYPE_NONE           = 0 ,

  INPDEVICE_TYPE_KEYBOARD           ,
  INPDEVICE_TYPE_MOUSE              ,
  INPDEVICE_TYPE_JOSTICK            ,
  INPDEVICE_TYPE_TOUCHSCREEN        ,
  INPDEVICE_TYPE_WIIMOTE            ,
};

#define INPDEVICE_SIZEID  32

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class INPBUTTON;
class INPCURSOR;
class GRPSCREEN;

class INPDEVICE
{
  public:
                                    INPDEVICE           ();
    virtual                        ~INPDEVICE           ();

    bool                            IsCreated           ();
    bool                            IsEnabled           ();

    void                            SetEnabled          (bool ON);

    INPDEVICE_TYPE                  GetType             ();
    void                            SetType             (INPDEVICE_TYPE type);

    int                             GetNButtons         ();

    XVECTOR<INPBUTTON*>*            GetButtons          ();
    XVECTOR<INPCURSOR*>*            GetCursors          ();

    bool                            DeleteAllButtons    ();
    bool                            DeleteAllCursors    ();

    INPBUTTON*                      GetButton           (INPBUTTON_ID ID);
    INPBUTTON*                      GetButton           (int index);
    INPBUTTON*                      GetButtonByCode     (XWORD code);

    INPBUTTON_STATE                 GetButtonState      (INPBUTTON_ID ID);

    INPBUTTON*                      IsPressButton       ();

    bool                            ReleaseAllButtons   ();

    int                             GetNCursors         ();
    INPCURSOR*                      GetCursor           (INPCURSOR_ID ID);
    INPCURSOR*                      GetCursor           (int index);
    INPCURSOR*                      IsChangeCursor      ();

    virtual bool                    SetScreen           (void* screenhandle);

    virtual bool                    Release             ();
    virtual bool                    Update              ();
  
  protected:

    bool                            created;
    bool                            enabled;

    INPDEVICE_TYPE                  type;

    GRPSCREEN*                      grpscreen;

    XVECTOR<INPBUTTON*>             buttons;
    XVECTOR<INPCURSOR*>             cursors;

  private:

    void                            Clean               ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




