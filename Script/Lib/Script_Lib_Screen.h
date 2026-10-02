/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Screen.h
* 
* @class      SCRIPT_LIB_SCREEN
* @brief      Script Library Screen
* @ingroup    SCRIPT
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

#include "XPath.h"
#include "XVector.h"

#include "Script_Lib.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_SCREEN    __L("Screen")

enum SCRIPT_LIB_SCREEN_POSSTATUS
{
  SCRIPT_LIB_SCREEN_POSSTATUS_OK            = 0 ,
  SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND          ,
  SCRIPT_LIB_SCREEN_POSSTATUS_BMPNOTFOUND
};

#define SCRIPT_SET_LIB_APPFLOWGRAPHICS(script, appgraphics)     { SCRIPT_LIB_SCREEN* lib = (SCRIPT_LIB_SCREEN*)script->GetLibrary(SCRIPT_LIB_NAME_SCREEN); \
                                                              if(lib) \
                                                                { \
                                                                  lib->SetAppGraphics(appgraphics); \
                                                                } \
                                                            }

//#define SCRIPT_LIB_SCREEN_DEBUG

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class SCRIPT;
class GRPBITMAP; 
class APPFLOWGRAPHICS;

struct SCRIPT_LIB_SCREEN_POS
{
  SCRIPT_LIB_SCREEN_POSSTATUS status;
  int                         x;
  int                         y;

  SCRIPT_LIB_SCREEN_POS()
  {
    status = SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND;
    x      = 0;
    y      = 0;
  }

  bool IsOk() const
  {
    return (status == SCRIPT_LIB_SCREEN_POSSTATUS_OK);
  }
};

class SCRIPT_LIB_SCREEN : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_SCREEN                     ();
    virtual              ~SCRIPT_LIB_SCREEN                     ();

    bool                  AddLibraryFunctions                   (SCRIPT* script);

    XBYTE                 BmpFindCFG_GetDiffLimitPercent        ();
    void                  BmpFindCFG_SetDiffLimitPercent        (XBYTE difflimitpercent);
    XBYTE                 BmpFindCFG_GetPixelMargin             ();
    void                  BmpFindCFG_SetPixelMargin             (XBYTE pixelmargin);


    #ifdef SCRIPT_LIB_SCREEN_DEBUG
    static APPFLOWGRAPHICS*   GetAppGraphics                        ();
    static void           SetAppGraphics                        (APPFLOWGRAPHICS* appgraphics);
    #endif

  private:

    void                  Clean                                 ();

    XBYTE                 bmpfindCFG_difflimitpercent;
    XBYTE                 bmpfindCFG_pixelmargin;

    #ifdef SCRIPT_LIB_SCREEN_DEBUG
    static APPFLOWGRAPHICS*   appgraphics;
    #endif

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

bool        Script_Lib_Screen_ResolvePos  (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, SCRIPT_LIB_SCREEN_POS& pos, XDWORD ntrailingouts = 0);

void        Call_Screen_GetPosX           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_GetPosY           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_GetPosXY          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_SetBmpFindCFG     (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_SetFocus          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_SetPosition       (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_Resize            (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_Minimize          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Screen_Maximize          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);

#ifdef SCRIPT_LIB_SCREEN_DEBUG

bool        DifferencesPerCent            (XDWORD ndiff, XDWORD max, int limit);
bool        IsSimilarPixel                (XDWORD origin, XDWORD target, XBYTE margin);
bool        FindSubBitmap                 (GRPBITMAP* bitmapscreen, GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent = 2, XBYTE pixelmargin = 25);
bool        PutBitmap                     (int x, int y, GRPBITMAP* bitmap);
GRPBITMAP*  GetBitmap                     (int x, int y, int sizex, int sizey);
void        FillLineDebug                 (GRPBITMAP* bitmapscreen, XDWORD* bufferscreen, XDWORD scrpos, XDWORD linesize, XDWORD color);

#endif





