/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPFactory.h
* 
* @class      GRPFACTORY
* @brief      Graphics Factory class
* @ingroup    GRAPHIC
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

#include "GRPProperties.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPFACTORY_GENERATECANVAS(newcanvas, canvas)    if(newcanvas)                               \
                                                          {                                         \
                                                            newcanvas->CopyPropertysFrom(screen);   \
                                                            newcanvas->CreateBuffers();             \
                                                            canvas = (GRP2DCANVAS*)newcanvas;       \
                                                          }

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSCREEN;
class GRP2DCANVAS;
class GRPBITMAP;
class GRPDESKTOPMANAGER;

class GRPFACTORY
{
  public:
                                      GRPFACTORY                ();
    virtual                          ~GRPFACTORY                ();

    static bool                       GetIsInstanced            ();
    static GRPFACTORY&                GetInstance               ();
    static bool                       SetInstance               (GRPFACTORY* instance);
    static bool                       DelInstance               ();

    virtual GRPSCREEN*                CreateScreen              ();
    virtual bool                      DeleteScreen              (GRPSCREEN* screen);

    GRP2DCANVAS*                      CreateCanvas              (GRPPROPERTIES* properties);
    bool                              DeleteCanvas              (GRP2DCANVAS* canvas);

    GRPBITMAP*                        CreateBitmap              (int width, int height, GRPPROPERTYMODE mode, bool isbufferinverse = false);
    bool                              DeleteBitmap              (GRPBITMAP* bitmap);

    #ifdef GRP_DESKTOPMANAGER_ACTIVE
    virtual GRPDESKTOPMANAGER*        CreateDesktopManager      ();
    virtual bool                      DeleteDesktopManager      (GRPDESKTOPMANAGER* desktopmanager);
    #endif

  private:

    void                              Clean                     ();

    static GRPFACTORY*                instance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





