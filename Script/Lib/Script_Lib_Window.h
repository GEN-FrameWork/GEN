/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Window.h
* 
* @class      SCRIPT_LIB_WINDOW
* @brief      Script Lib Window class
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

#include "Script_Lib.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LIB_NAME_WINDOW    __L("Window")

#define SCRIPT_SET_LIB_APPFLOWGRAPHICS(script, appgraphics)     { SCRIPT_LIB_WINDOW* lib = (SCRIPT_LIB_WINDOW*)script->GetLibrary(SCRIPT_LIB_NAME_WINDOW); \
                                                              if(lib) \
                                                                { \
                                                                  lib->SetAppGraphics(appgraphics); \
                                                                } \
                                                            }

//#define SCRIPT_LIB_WINDOWS_DEBUG

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class SCRIPT;
class GRPBITMAP; 
class APPFLOWGRAPHICS;

class SCRIPT_LIB_WINDOW : public SCRIPT_LIB
{
  public:
                          SCRIPT_LIB_WINDOW                     ();
    virtual              ~SCRIPT_LIB_WINDOW                     ();

    bool                  AddLibraryFunctions                   (SCRIPT* script);

    XBYTE                 BmpFindCFG_GetDiffLimitPercent        ();
    void                  BmpFindCFG_SetDiffLimitPercent        (XBYTE difflimitpercent);
    XBYTE                 BmpFindCFG_GetPixelMargin             ();
    void                  BmpFindCFG_SetPixelMargin             (XBYTE pixelmargin);


    #ifdef SCRIPT_LIB_WINDOWS_DEBUG
    static APPFLOWGRAPHICS*   GetAppGraphics                        ();
    static void           SetAppGraphics                        (APPFLOWGRAPHICS* appgraphics);
    #endif

  private:

    void                  Clean                                 ();

    XBYTE                 bmpfindCFG_difflimitpercent;
    XBYTE                 bmpfindCFG_pixelmargin;

    #ifdef SCRIPT_LIB_WINDOWS_DEBUG
    static APPFLOWGRAPHICS*   appgraphics;
    #endif

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

void        Call_Window_GetPosX           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_GetPosY           (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_SetBmpFindCFG     (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_SetFocus          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_SetPosition       (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_Resize            (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_Minimize          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);
void        Call_Window_Maximize          (SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);

#ifdef SCRIPT_LIB_WINDOWS_DEBUG

bool        DifferencesPerCent            (XDWORD ndiff, XDWORD max, int limit);
bool        IsSimilarPixel                (XDWORD origin, XDWORD target, XBYTE margin);
bool        FindSubBitmap                 (GRPBITMAP* bitmapscreen, GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent = 2, XBYTE pixelmargin = 25);
bool        PutBitmap                     (int x, int y, GRPBITMAP* bitmap);
GRPBITMAP*  GetBitmap                     (int x, int y, int sizex, int sizey);
void        FillLineDebug                 (GRPBITMAP* bitmapscreen, XDWORD* bufferscreen, XDWORD scrpos, XDWORD linesize, XDWORD color);

#endif





