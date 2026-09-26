/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXDeviceMouseX11.h
* 
* @class      INPLINUXDEVICEMOUSEX11
* @brief      LINUX Input X11 device mouse class
* @ingroup    PLATFORM_LINUX
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


#ifdef LINUX_X11_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "INPCursor.h"
#include "INPDevice.h"

#include "GRPLINUXScreenX11.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define INPLINUXDEVICEMOUSE11_EVRELEASED    0
#define INPLINUXDEVICEMOUSE11_EVPRESSED     1
#define INPLINUXDEVICEMOUSE11_EVREPEAT      2




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class INPLINUXDEVICEMOUSEX11 : public INPDEVICE
{
  public:
                            INPLINUXDEVICEMOUSEX11    ();
    virtual                ~INPLINUXDEVICEMOUSEX11    ();

    bool                    Update                    ();

    bool                    SetScreen                 (void* param);

  private:

    void                    Clean                     ();

    bool                    CreateAllButtons          ();
    bool                    CreateAllCursors          ();

    GRPLINUXSCREENX11*      grpscreenx11;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



