/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Window.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "Script_Lib_Window.h"

#include "XVariant.h"
#include "XProcessManager.h"

#include "APPFlowBase.h"
#include "APPFlowMain.h"

#ifdef SCRIPT_LIB_WINDOWS_DEBUG
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

int            windowsposx = 0;
int            windowsposy = 0;

#ifdef SCRIPT_LIB_WINDOWS_DEBUG
APPFLOWGRAPHICS*   SCRIPT_LIB_WINDOW::appgraphics = NULL;
#endif
			


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_WINDOW::SCRIPT_LIB_WINDOW()
* @brief      Constructor
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_WINDOW::SCRIPT_LIB_WINDOW() : SCRIPT_LIB(SCRIPT_LIB_NAME_WINDOW)
{
  Clean();
  
  bmpfindCFG_difflimitpercent = 2;
  bmpfindCFG_pixelmargin      = 10;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_WINDOW::~SCRIPT_LIB_WINDOW()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_WINDOW::~SCRIPT_LIB_WINDOW()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_WINDOW::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_WINDOW::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("Window_GetPosX"), Call_Window_GetPosX);
  script->AddLibraryFunction(this, __L("Window_GetPosY"), Call_Window_GetPosY);
  script->AddLibraryFunction(this, __L("Window_SetBmpFindCFG"), Call_Window_SetBmpFindCFG);
  script->AddLibraryFunction(this, __L("Window_SetFocus"), Call_Window_SetFocus);
  script->AddLibraryFunction(this, __L("Window_SetPosition"), Call_Window_SetPosition);
  script->AddLibraryFunction(this, __L("Window_Resize"), Call_Window_Resize);
  script->AddLibraryFunction(this, __L("Window_Minimize"), Call_Window_Minimize);
  script->AddLibraryFunction(this, __L("Window_Maximize"), Call_Window_Maximize);
      
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE SCRIPT_LIB_WINDOW::BmpFindCFG_GetDiffLimitPercent()
* @brief      Bmp find CFG get diff limit percent
* @ingroup    SCRIPT
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE SCRIPT_LIB_WINDOW::BmpFindCFG_GetDiffLimitPercent()
{
  return bmpfindCFG_difflimitpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_WINDOW::BmpFindCFG_SetDiffLimitPercent(XBYTE difflimitpercent)
* @brief      Bmp find CFG set diff limit percent
* @ingroup    SCRIPT
* 
* @param[in]  difflimitpercent : Difflimitpercent value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_WINDOW::BmpFindCFG_SetDiffLimitPercent(XBYTE difflimitpercent)
{
  bmpfindCFG_difflimitpercent = difflimitpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE SCRIPT_LIB_WINDOW::BmpFindCFG_GetPixelMargin()
* @brief      Bmp find CFG get pixel margin
* @ingroup    SCRIPT
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE SCRIPT_LIB_WINDOW::BmpFindCFG_GetPixelMargin()
{
  return bmpfindCFG_pixelmargin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_WINDOW::BmpFindCFG_SetPixelMargin(XBYTE pixelmargin)
* @brief      Bmp find CFG set pixel margin
* @ingroup    SCRIPT
* 
* @param[in]  pixelmargin : Pixelmargin value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_WINDOW::BmpFindCFG_SetPixelMargin(XBYTE pixelmargin)
{
  bmpfindCFG_pixelmargin = pixelmargin;
}


#ifdef SCRIPT_LIB_WINDOWS_DEBUG

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWGRAPHICS* SCRIPT_LIB_WINDOW::GetAppGraphics()
* @brief      Get app graphics
* @ingroup    SCRIPT
* 
* @return     APPFLOWGRAPHICS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWGRAPHICS* SCRIPT_LIB_WINDOW::GetAppGraphics()
{
  return appgraphics;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_WINDOW::SetAppGraphics(APPFLOWGRAPHICS* _appgraphics)
* @brief      Set app graphics
* @ingroup    SCRIPT
* 
* @param[in]  _appgraphics : Appgraphics pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_WINDOW::SetAppGraphics(APPFLOWGRAPHICS* _appgraphics)
{
  appgraphics = _appgraphics;
}

#endif

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_WINDOW::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_WINDOW::Clean()
{ 
  windowsposx                 = 0;
  windowsposy                 = 0;

  bmpfindCFG_difflimitpercent = 2;
  bmpfindCFG_pixelmargin      = 10;

  #ifdef SCRIPT_LIB_WINDOWS_DEBUG
  appgraphics = NULL;
  #endif
}


/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Window_GetPosX(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_GetPosX
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_GetPosX(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  SCRIPT_LIB_WINDOW* windows_library = (SCRIPT_LIB_WINDOW*)library;

  returnvalue->Set();

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  windowsposx = 0xFFFFFF;
  windowsposy = 0xFFFFFF;
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
    
  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
                {
                  windowsposx = applist.Get(c)->GetWindowRect()->x1; 
                  windowsposy = applist.Get(c)->GetWindowRect()->y1; 
                  
                  if(params->GetSize() >= 3)
                    {                     
                      void* handle_windows = applist.Get(c)->GetWindowHandle();                  
                      if(handle_windows)
                        {  
                          GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                          if(screen)
                            {                          
                              screen->SetHandle(handle_windows);
                              screen->SetWidth(applist.Get(c)->GetWindowRect()->x2 - applist.Get(c)->GetWindowRect()->x1);
                              screen->SetHeight(applist.Get(c)->GetWindowRect()->y2 - applist.Get(c)->GetWindowRect()->y1);
                              
                              GRPBITMAP* bitmapscreen = screen->CaptureContent();
                              if(bitmapscreen)
                                {                                     
                                  // ----------------------------------------------------------------------------------

                                  #ifdef SCRIPT_LIB_WINDOWS_DEBUG
                                  PutBitmap(0, 0, bitmapscreen);
                                  #endif                             

                                  // ----------------------------------------------------------------------------------

                                  bool found = false;

                                  for(XDWORD d=2; d<params->GetSize(); d++)
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
                                              #ifdef SCRIPT_LIB_WINDOWS_DEBUG                              
                                              XPATH  xpathbitmaptest;

                                              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbitmaptest);
                                              xpathbitmaptest.Slash_Add();
                                              xpathbitmaptest.Add(__L("back.png"));

                                              bitmapfileref->Save(xpathbitmaptest, bitmapscreen);

                                              /*
                                              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbitmaptest);
                                              xpathbitmaptest.Slash_Add();
                                              xpathbitmaptest.Add(__L("image001.png"));

                                              GRPBITMAP* bitmaprefcap = GetBitmap(10, 418, 74, 11);
                                              if(bitmaprefcap)
                                                {
                                                  bitmapfileref->Save(xpathbitmaptest, bitmaprefcap);
                                                  GEN_DELETE bitmaprefcap;
                                                }
                                              */  
                                              #endif
                            
                                              GRPBITMAP* bitmapref = bitmapfileref->Load();         
                                              if(bitmapref)
                                                {
                                                  int x = 0;
                                                  int y = 0;   

                                                  #ifdef SCRIPT_LIB_WINDOWS_DEBUG
                                                  if(FindSubBitmap(bitmapscreen, bitmapref, x, y, windows_library->BmpFindCFG_GetDiffLimitPercent(), windows_library->BmpFindCFG_GetPixelMargin()))    
                                                  #else                                                
                                                  if(bitmapscreen->FindSubBitmap(bitmapref, x, y, windows_library->BmpFindCFG_GetDiffLimitPercent(), windows_library->BmpFindCFG_GetPixelMargin()))
                                                  #endif    
                                                    {
                                                      windowsposx += (x + (bitmapref->GetWidth() /2) + applist.Get(c)->GetWindowBorderWidth()); 
                                                      windowsposy += (y + (bitmapref->GetHeight()/2) + applist.Get(c)->GetWindowTitleHeight()); 
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
                                      windowsposx = 0xFFFFFE;
                                      windowsposy = 0xFFFFFE;
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
   
  (*returnvalue) = windowsposx;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Window_GetPosY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Window_GetPosY
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_GetPosY(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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

  (*returnvalue) = windowsposy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Window_SetBmpFindCFG(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_SetBmpFindCFG
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_SetBmpFindCFG(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  SCRIPT_LIB_WINDOW* windows_library = (SCRIPT_LIB_WINDOW*)library;
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

  windows_library->BmpFindCFG_SetDiffLimitPercent((XBYTE)bmpfindCFG_difflimitpercent);
  windows_library->BmpFindCFG_SetPixelMargin((XBYTE)bmpfindCFG_pixelmargin);

  status = true;

  (*returnvalue) =  status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Window_SetFocus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_SetFocus
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_SetFocus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
                {
                  void* handle_windows = applist.Get(c)->GetWindowHandle();
                  
                  if(handle_windows)
                    {  
                      GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
                      if(screen)
                        {                          
                          screen->SetHandle(handle_windows);
                          screen->Set_Focus();
                        
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
* @fn         void Call_Window_SetPosition(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_SetPosition
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_SetPosition(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
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
* @fn         void Call_Window_Resize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_Resize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_Resize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
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
* @fn         void Call_Window_Minimize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_Window_Minimize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_Minimize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
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
* @fn         void Call_Window_Maximize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Window_Maximize
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Window_Maximize(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
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


#ifdef SCRIPT_LIB_WINDOWS_DEBUG

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
      GRPSCREEN*   screen   = SCRIPT_LIB_WINDOW::GetAppGraphics()->GetMainScreen();

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
  if(!SCRIPT_LIB_WINDOW::GetAppGraphics())
    {
      return false;
    }
                                        
  GRPVIEWPORT* viewport = NULL;
  GRP2DCANVAS*   canvas   = NULL;
  GRPSCREEN*   screen   = SCRIPT_LIB_WINDOW::GetAppGraphics()->GetMainScreen();

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
  if(!SCRIPT_LIB_WINDOW::GetAppGraphics())
    {
      return NULL;
    }
                                        
  GRPVIEWPORT* viewport = NULL;
  GRP2DCANVAS*   canvas   = NULL;
  GRPSCREEN*   screen   = SCRIPT_LIB_WINDOW::GetAppGraphics()->GetMainScreen();
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





