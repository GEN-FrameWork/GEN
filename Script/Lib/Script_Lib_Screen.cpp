/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Screen.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "Script_Lib_Screen.h"

#include "XVariant.h"
#include "XBuffer.h"
#include "XProcessManager.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

#include "APPFlowBase.h"
#include "APPFlowMain.h"

#ifdef SCRIPT_LIB_SCREEN_DEBUG
#include "APPFlowGraphics.h"
#endif

#include "INPFactory.h"
#include "INPSimulate.h"

#include "GRPFactory.h"
#include "GRPScreen.h"
#include "GRPBitmap.h"
#include "GRPBitmapFile.h"

#include "GRPViewPort.h"
#include "GRP2DCanvas.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#ifdef SCRIPT_LIB_SCREEN_DEBUG
APPFLOWGRAPHICS*   SCRIPT_LIB_SCREEN::appgraphics = NULL;
#endif
			


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SCREEN::SCRIPT_LIB_SCREEN()
* @brief      Constructor
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SCREEN::SCRIPT_LIB_SCREEN() : SCRIPT_LIB(SCRIPT_LIB_NAME_SCREEN)
{
  Clean();
  
  bmpfindCFG_difflimitpercent = 2;
  bmpfindCFG_pixelmargin      = 10;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SCREEN::~SCRIPT_LIB_SCREEN()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SCREEN::~SCRIPT_LIB_SCREEN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_SCREEN::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_SCREEN::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("Screen_GetPosX"), Call_Screen_GetPosX);
  script->AddLibraryFunction(this, __L("Screen_GetPosY"), Call_Screen_GetPosY);
  script->AddLibraryFunction(this, __L("Screen_GetPosXY"), Call_Screen_GetPosXY);
  script->AddLibraryFunction(this, __L("Screen_SetBmpFindCFG"), Call_Screen_SetBmpFindCFG);
  script->AddLibraryFunction(this, __L("Screen_SetFocus"), Call_Screen_SetFocus);
  script->AddLibraryFunction(this, __L("Screen_SetPosition"), Call_Screen_SetPosition);
  script->AddLibraryFunction(this, __L("Screen_Resize"), Call_Screen_Resize);
  script->AddLibraryFunction(this, __L("Screen_Minimize"), Call_Screen_Minimize);
  script->AddLibraryFunction(this, __L("Screen_Maximize"), Call_Screen_Maximize);
      
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE SCRIPT_LIB_SCREEN::BmpFindCFG_GetDiffLimitPercent()
* @brief      Bmp find CFG get diff limit percent
* @ingroup    SCRIPT
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE SCRIPT_LIB_SCREEN::BmpFindCFG_GetDiffLimitPercent()
{
  return bmpfindCFG_difflimitpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCREEN::BmpFindCFG_SetDiffLimitPercent(XBYTE difflimitpercent)
* @brief      Bmp find CFG set diff limit percent
* @ingroup    SCRIPT
* 
* @param[in]  difflimitpercent : Difflimitpercent value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCREEN::BmpFindCFG_SetDiffLimitPercent(XBYTE difflimitpercent)
{
  bmpfindCFG_difflimitpercent = difflimitpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE SCRIPT_LIB_SCREEN::BmpFindCFG_GetPixelMargin()
* @brief      Bmp find CFG get pixel margin
* @ingroup    SCRIPT
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE SCRIPT_LIB_SCREEN::BmpFindCFG_GetPixelMargin()
{
  return bmpfindCFG_pixelmargin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCREEN::BmpFindCFG_SetPixelMargin(XBYTE pixelmargin)
* @brief      Bmp find CFG set pixel margin
* @ingroup    SCRIPT
* 
* @param[in]  pixelmargin : Pixelmargin value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCREEN::BmpFindCFG_SetPixelMargin(XBYTE pixelmargin)
{
  bmpfindCFG_pixelmargin = pixelmargin;
}


#ifdef SCRIPT_LIB_SCREEN_DEBUG

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWGRAPHICS* SCRIPT_LIB_SCREEN::GetAppGraphics()
* @brief      Get app graphics
* @ingroup    SCRIPT
* 
* @return     APPFLOWGRAPHICS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWGRAPHICS* SCRIPT_LIB_SCREEN::GetAppGraphics()
{
  return appgraphics;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCREEN::SetAppGraphics(APPFLOWGRAPHICS* _appgraphics)
* @brief      Set app graphics
* @ingroup    SCRIPT
* 
* @param[in]  _appgraphics : Appgraphics pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCREEN::SetAppGraphics(APPFLOWGRAPHICS* _appgraphics)
{
  appgraphics = _appgraphics;
}

#endif

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCREEN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCREEN::Clean()
{ 
  bmpfindCFG_difflimitpercent = 2;
  bmpfindCFG_pixelmargin      = 10;

  #ifdef SCRIPT_LIB_SCREEN_DEBUG
  appgraphics = NULL;
  #endif
}


/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool Script_Lib_Screen_ResolvePos(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, SCRIPT_LIB_SCREEN_POS& pos)
* @brief      Resolve window / bitmap position without shared mutable state
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[out] pos : Resolved position (x,y).
* 
* @return     bool : true if params were accepted; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool Script_Lib_Screen_ResolvePos(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, SCRIPT_LIB_SCREEN_POS& pos, XDWORD ntrailingouts)
{
  pos.status = SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND;
  pos.x      = 0;
  pos.y      = 0;

  if(!library)  return false;
  if(!script)   return false;
  if(!params)   return false;

  SCRIPT_LIB_SCREEN* screen_library = (SCRIPT_LIB_SCREEN*)library;

  if(params->GetSize() < (2 + ntrailingouts))
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return false;
    }

  XDWORD              nresolve      = params->GetSize() - ntrailingouts;
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
    
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  pos.status = SCRIPT_LIB_SCREEN_POSSTATUS_OK;
                  pos.x      = applist.Get(c)->GetWindowRect()->x1; 
                  pos.y      = applist.Get(c)->GetWindowRect()->y1; 
                  
                  if(nresolve >= 3)
                    {                     
                      void* handle_windows = applist.Get(c)->GetWindowHandle();                  
                      if(handle_windows)
                        {  
                          GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                          if(screen)
                            {                          
                              screen->SetHandle(handle_windows);
                              // CaptureContent uses GetDC (client area). Prefer client size when available.
                              int captw = applist.Get(c)->GetWindowRect()->x2 - applist.Get(c)->GetWindowRect()->x1;
                              int capth = applist.Get(c)->GetWindowRect()->y2 - applist.Get(c)->GetWindowRect()->y1;
                              #ifdef WINDOWS
                              RECT crect;
                              if(GetClientRect((HWND)handle_windows, &crect))
                                {
                                  int cw = crect.right  - crect.left;
                                  int ch = crect.bottom - crect.top;
                                  if(cw > 0) captw = cw;
                                  if(ch > 0) capth = ch;
                                }
                              #endif
                              screen->SetWidth(captw);
                              screen->SetHeight(capth);
                              
                              GRPBITMAP* bitmapscreen = screen->CaptureContent();
                              if(bitmapscreen)
                                {                                     
                                  // ----------------------------------------------------------------------------------

                                  #ifdef SCRIPT_LIB_SCREEN_DEBUG
                                  PutBitmap(0, 0, bitmapscreen);
                                  #endif                             

                                  // ----------------------------------------------------------------------------------

                                  bool found = false;

                                  for(XDWORD d=2; d<nresolve; d++)
                                    { 
                                      XSTRING bitmaprefname  = (*params->Get(d));
                                      if(!bitmaprefname.IsEmpty())
                                        {                                         
                                          XPATH  xpathbitmapref;  
                                      
                                          GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbitmapref);
                                          xpathbitmapref.Slash_Add();
                                          xpathbitmapref.Add(bitmaprefname);

                                          GRPBITMAPFILE* bitmapfileref = GEN_NEW GRPBITMAPFILE(xpathbitmapref);
                                          if(bitmapfileref)
                                            {                   
                                              #ifdef SCRIPT_LIB_SCREEN_DEBUG                              
                                              XPATH  xpathbitmaptest;

                                              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbitmaptest);
                                              xpathbitmaptest.Slash_Add();
                                              xpathbitmaptest.Add(__L("back.png"));

                                              bitmapfileref->Save(xpathbitmaptest, bitmapscreen);
                                              #endif
                            
                                              GRPBITMAP* bitmapref = bitmapfileref->Load();         
                                              if(bitmapref)
                                                {
                                                  int x = 0;
                                                  int y = 0;   

                                                  #ifdef SCRIPT_LIB_SCREEN_DEBUG
                                                  if(FindSubBitmap(bitmapscreen, bitmapref, x, y, screen_library->BmpFindCFG_GetDiffLimitPercent(), screen_library->BmpFindCFG_GetPixelMargin()))    
                                                  #else                                                
                                                  if(bitmapscreen->FindSubBitmap(bitmapref, x, y, screen_library->BmpFindCFG_GetDiffLimitPercent(), screen_library->BmpFindCFG_GetPixelMargin()))
                                                  #endif    
                                                    {
                                                      pos.x += (x + (bitmapref->GetWidth() /2) + applist.Get(c)->GetWindowBorderWidth()); 
                                                      pos.y += (y + (bitmapref->GetHeight()/2) + applist.Get(c)->GetWindowTitleHeight()); 
                                                      found = true;
                                                    }      
                                                }                                                 

                                              GEN_DELETE bitmapref;
                                              GEN_DELETE bitmapfileref;    
                                            }                                            
                                        }
                                       else  
                                        {
                                          break;
                                        } 
                                        
                                      if(found)
                                        {
                                          break;    
                                        }     
                                     } 

                                  if(!found)
                                    {
                                      pos.status = SCRIPT_LIB_SCREEN_POSSTATUS_BMPNOTFOUND;
                                      pos.x      = 0;
                                      pos.y      = 0;
                                    }

                                  GEN_DELETE bitmapscreen;

                                  GEN_GRPFACTORY.DeleteScreen(screen);                                
                                }
                            } 
                        }      
                    }
                
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_GetPosX(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Screen_GetPosX
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use (app, title [, bmp...], out_x).
* @param[in]  returnvalue : Returnvalue pointer to use (status int).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_GetPosX(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();
  (*returnvalue) = (int)SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND;

  SCRIPT_LIB_SCREEN_POS pos;

  if(!Script_Lib_Screen_ResolvePos(library, script, params, pos, 1))
    {
      return;
    }

  XVARIANT* outx = params->Get(params->GetSize() - 1);
  if(outx) (*outx) = pos.IsOk() ? pos.x : 0;

  (*returnvalue) = (int)pos.status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_GetPosY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Screen_GetPosY
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use (app, title [, bmp...], out_y).
* @param[in]  returnvalue : Returnvalue pointer to use (status int).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_GetPosY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();
  (*returnvalue) = (int)SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND;

  SCRIPT_LIB_SCREEN_POS pos;

  if(!Script_Lib_Screen_ResolvePos(library, script, params, pos, 1))
    {
      return;
    }

  XVARIANT* outy = params->Get(params->GetSize() - 1);
  if(outy) (*outy) = pos.IsOk() ? pos.y : 0;

  (*returnvalue) = (int)pos.status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_GetPosXY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Screen_GetPosXY
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use (app, title [, bmp...], out_x, out_y).
* @param[in]  returnvalue : Returnvalue pointer to use (status int).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_GetPosXY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();
  (*returnvalue) = (int)SCRIPT_LIB_SCREEN_POSSTATUS_NOTFOUND;

  SCRIPT_LIB_SCREEN_POS pos;

  if(!Script_Lib_Screen_ResolvePos(library, script, params, pos, 2))
    {
      return;
    }

  XVARIANT* outx = params->Get(params->GetSize() - 2);
  XVARIANT* outy = params->Get(params->GetSize() - 1);
  if(outx) (*outx) = pos.IsOk() ? pos.x : 0;
  if(outy) (*outy) = pos.IsOk() ? pos.y : 0;

  (*returnvalue) = (int)pos.status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_SetBmpFindCFG(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Screen_SetBmpFindCFG
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_SetBmpFindCFG(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  SCRIPT_LIB_SCREEN* screen_library = (SCRIPT_LIB_SCREEN*)library;
  bool               status = false; 

  returnvalue->Set();

  (*returnvalue) = status;

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int bmpfindCFG_difflimitpercent = 0;
  int bmpfindCFG_pixelmargin      = 0;

  library->GetParamConverted(params->Get(0), bmpfindCFG_difflimitpercent);
  library->GetParamConverted(params->Get(1), bmpfindCFG_pixelmargin);

  screen_library->BmpFindCFG_SetDiffLimitPercent((XBYTE)bmpfindCFG_difflimitpercent);
  screen_library->BmpFindCFG_SetPixelMargin((XBYTE)bmpfindCFG_pixelmargin);

  status = true;

  (*returnvalue) =  status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_SetFocus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Screen_SetFocus
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_SetFocus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  bool                status        = false;
  
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {                          
                          screen->SetHandle(handle_windows);
                          status = screen->Set_Focus();
                        
                          GEN_GRPFACTORY.DeleteScreen(screen);  
                        }
                    } 
                    
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_SetPosition(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Screen_SetPosition
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_SetPosition(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<4)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  int                 positionx     = 0;
  int                 positiony     = 0;
  bool                status        = false;

  library->GetParamConverted(params->Get(2), positionx);
  library->GetParamConverted(params->Get(3), positiony);
  
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {
                          screen->SetHandle(handle_windows);
                          screen->Set_Position(positionx, positiony);
                        
                          GEN_GRPFACTORY.DeleteScreen(screen);  

                          status = true;   
                        }
                    } 
                    
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_Resize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Screen_Resize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_Resize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<4)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  int                 sizex         = 0;
  int                 sizey         = 0;
  bool                status        = false;

  library->GetParamConverted(params->Get(2), sizex);
  library->GetParamConverted(params->Get(3), sizey);
  
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {
                          screen->SetHandle(handle_windows);
                          screen->Resize(sizex, sizey);
                        
                          GEN_GRPFACTORY.DeleteScreen(screen);  

                          status = true;   
                        }
                    } 
                    
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_Minimize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Screen_Minimize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_Minimize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  bool                showstatus    = false;
  bool                status        = false;

  library->GetParamConverted(params->Get(2), showstatus);
  
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {
                          screen->SetHandle(handle_windows);
                          status = screen->Minimize(showstatus);
                          
                          GEN_GRPFACTORY.DeleteScreen(screen);  
                        }
                    } 
                    
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Screen_Maximize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Screen_Maximize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Screen_Maximize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<3)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  bool                showstatus    = false;
  bool                status        = false;

  library->GetParamConverted(params->Get(2), showstatus);
  
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, true) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {
                          screen->SetHandle(handle_windows);
                          status = screen->Maximize(showstatus);
                          
                          GEN_GRPFACTORY.DeleteScreen(screen);  
                        }
                    } 
                    
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}


#ifdef SCRIPT_LIB_SCREEN_DEBUG

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DifferencesPerCent(XDWORD ndiff, XDWORD max, int limit)
* @brief      ifferencesPerCent
* @ingroup    SCRIPT
* 
* @param[in]  ndiff : Ndiff value.
* @param[in]  max : Max value.
* @param[in]  limit : Limit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DifferencesPerCent(XDWORD ndiff, XDWORD max, int limit)
{
  int actualdiff = ((ndiff*100)/max);

  if(actualdiff > limit) return false;

  return true;
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool IsSimilarPixel(XDWORD origin, XDWORD target, XBYTE margin)
* @brief      sSimilarPixel
* @ingroup    SCRIPT
* 
* @param[in]  origin : Origin value.
* @param[in]  target : Target value.
* @param[in]  margin : Margin value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool IsSimilarPixel(XDWORD origin, XDWORD target, XBYTE margin)
{
  XBYTE* originRGBA = (XBYTE*)&origin;
  XBYTE* targetRGBA = (XBYTE*)&target;  
  bool   status     = false;

  int ncomponent = 0;

  for(int c=0; c<4; c++)
    {
      if(abs(originRGBA[c] - targetRGBA[c]) > margin)
        {
          ncomponent++;
        }
    }

  if(!ncomponent) 
    {
      status = true;
    }  

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool FindSubBitmap(GRPBITMAP* bitmapscreen, GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent, XBYTE pixelmargin)
* @brief      indSubBitmap
* @ingroup    SCRIPT
* 
* @param[in]  bitmapscreen : Bitmapscreen pointer to use.
* @param[in]  bitmapref : Bitmapref pointer to use.
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  difflimitpercent : Difflimitpercent value.
* @param[in]  pixelmargin : Pixelmargin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool FindSubBitmap(GRPBITMAP* bitmapscreen, GRPBITMAP* bitmapref, int& x, int& y, XBYTE difflimitpercent, XBYTE pixelmargin)
{
  x = 0; 
  y = 0;

  if(!bitmapref)
    {
      return false;
    }
  
  GRPBITMAP* _bitmap = bitmapref->ConvertToMode(bitmapscreen->GetMode());
  if(!_bitmap)
    {
      return false;
    }

  if(_bitmap->GetMode() != bitmapscreen->GetMode()) 
    {
      return false;
    }

  if(!_bitmap->GetBuffer())
    {
      return false;
    }
  
  XDWORD*   bufferscreen        = (XDWORD*)bitmapscreen->GetBuffer();
  XDWORD*   bufferbitmap        = (XDWORD*)_bitmap->GetBuffer();
  XDWORD    sizepixel           = sizeof(XDWORD);
  XDWORD    bufferscreensize    = (bitmapscreen->GetBufferSize() / sizepixel) - _bitmap->GetWidth();
  XDWORD    bufferbmplinesize   = _bitmap->GetWidth();
  XDWORD    ndiff               = 0;
  bool      found               = false;
  
  for(XDWORD scrpos = 0; scrpos < bufferscreensize; scrpos++)
    {
      ndiff = 0;
      for(XDWORD bmppos = 0; bmppos < bufferbmplinesize; bmppos++)  
        {    
          if(bufferscreen[scrpos + bmppos] != bufferbitmap[bmppos])
            {
              if(!IsSimilarPixel(bufferscreen[scrpos + bmppos], bufferbitmap[bmppos], pixelmargin))
                {
                  ndiff++;
                }
               else
                {
                  //FillLineDebug(bitmapscreen, bufferscreen, scrpos + bmppos, 1, 0xFF0000FF);
                } 
            }
        }
               
      found = DifferencesPerCent(ndiff, bufferbmplinesize, difflimitpercent);
      if(found)
        {
          // FillLineDebug(bitmapscreen, bufferscreen, scrpos, bufferbmplinesize, 0xFF0000FF);
          
          found = false;

          XDWORD srcpixelsline  = bitmapscreen->GetWidth();
          XDWORD scrpos_tmp     = scrpos;
          XDWORD bmppos_tmp     = bufferbmplinesize;
         
          x =  (scrpos % bitmapscreen->GetWidth());
          y =  bitmapscreen->GetHeight() - (scrpos / srcpixelsline) - _bitmap->GetHeight();           

          scrpos_tmp += srcpixelsline;              
    
          for(XDWORD line = 1; line < _bitmap->GetHeight(); line++)
            {                                                   
              ndiff = 0;
              for(XDWORD bmppos = 0; bmppos < bufferbmplinesize; bmppos++)  
                {    
                  if(scrpos_tmp + bmppos >= bufferscreensize)
                    {
                      ndiff += difflimitpercent;       
                      break;
                    }

                  if(bufferscreen[scrpos_tmp + bmppos] != bufferbitmap[bmppos_tmp])
                    {
                      if(!IsSimilarPixel(bufferscreen[scrpos_tmp + bmppos], bufferbitmap[bmppos_tmp], pixelmargin))
                        {
                          ndiff++;
                        }
                       else
                        {
                          // FillLineDebug(bitmapscreen, bufferscreen, scrpos_tmp + bmppos, 1, 0xFF0000FF);
                        } 
                    }  
                  
                  bmppos_tmp++;
                }
              
              found = DifferencesPerCent(ndiff, bufferbmplinesize, difflimitpercent);              

              if(!found)
                {
                  break;              
                }

              //FillLineDebug(bitmapscreen, bufferscreen, scrpos_tmp, bufferbmplinesize, 0xFF0000FF);

              scrpos_tmp += srcpixelsline;

            }

          if(found)                  
            {
              break;
            }              
        }  
    } 
  
  if(!found)
    {
      x = 0;
      y = 0;
    }
   else
    {
      GRPVIEWPORT* viewport = NULL;
      GRP2DCANVAS*   canvas   = NULL;
      GRPSCREEN*   screen   = SCRIPT_LIB_SCREEN::GetAppGraphics()->GetMainScreen();

      if(screen) viewport = screen->GetViewport(0);
      if(viewport) canvas = viewport->GetCanvas();

      if(canvas)
        {
          GRP2DCOLOR_RGBA8  colorred(255, 0, 0);

          canvas->SetLineWidth(1.0f);
          canvas->SetLineColor(&colorred);

          canvas->Rectangle(x, y, x + _bitmap->GetWidth(), y + _bitmap->GetHeight());  
          canvas->PutPixel((x + _bitmap->GetWidth()/2), y + (_bitmap->GetHeight()/2), &colorred);

          screen->UpdateViewports();
        }      
    }
 
  GEN_DELETE _bitmap;
  
  return found;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool PutBitmap(int x, int y, GRPBITMAP* bitmap)
* @brief      utBitmap
* @ingroup    SCRIPT
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  bitmap : Bitmap pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool PutBitmap(int x, int y, GRPBITMAP* bitmap)
{
  if(!SCRIPT_LIB_SCREEN::GetAppGraphics())
    {
      return false;
    }
                                        
  GRPVIEWPORT* viewport = NULL;
  GRP2DCANVAS*   canvas   = NULL;
  GRPSCREEN*   screen   = SCRIPT_LIB_SCREEN::GetAppGraphics()->GetMainScreen();

  if(screen) viewport = screen->GetViewport(0);
  if(viewport) canvas = viewport->GetCanvas();

  if(canvas)
    {
      canvas->PutBitmap(x, y, bitmap);  
      screen->UpdateViewports();                                              
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GetBitmap(int x, int y, int sizex, int sizey)
* @brief      etBitmap
* @ingroup    SCRIPT
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  sizex : Sizex value.
* @param[in]  sizey : Sizey value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GetBitmap(int x, int y, int sizex, int sizey)
{
  if(!SCRIPT_LIB_SCREEN::GetAppGraphics())
    {
      return NULL;
    }
                                        
  GRPVIEWPORT* viewport = NULL;
  GRP2DCANVAS*   canvas   = NULL;
  GRPSCREEN*   screen   = SCRIPT_LIB_SCREEN::GetAppGraphics()->GetMainScreen();
  GRPBITMAP*   bitmap   = NULL;

  if(screen) viewport = screen->GetViewport(0);
  if(viewport) canvas = viewport->GetCanvas();

  if(canvas)
    {
      bitmap = canvas->GetBitmap(x, y, sizex, sizey);  
      screen->UpdateViewports();                                              
    }
  
  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void FillLineDebug(GRPBITMAP* bitmapscreen, XDWORD* bufferscreen, XDWORD scrpos, XDWORD linesize, XDWORD color)
* @brief      illLineDebug
* @ingroup    SCRIPT
* 
* @param[in]  bitmapscreen : Bitmapscreen pointer to use.
* @param[in]  bufferscreen : Bufferscreen pointer to use.
* @param[in]  scrpos : Scrpos value.
* @param[in]  linesize : Linesize value.
* @param[in]  color : Color value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void FillLineDebug(GRPBITMAP* bitmapscreen, XDWORD* bufferscreen, XDWORD scrpos, XDWORD linesize, XDWORD color)
{
  for(XDWORD c=scrpos; c<(scrpos + linesize); c++)
    {
      bufferscreen[c] = color; // 0xFF0000FF;
    }

  PutBitmap(0, 0, bitmapscreen);
}



#endif





