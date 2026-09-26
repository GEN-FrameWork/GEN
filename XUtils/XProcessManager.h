/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XProcessManager.h
* 
* @class      XPROCESSMANAGER
* @brief      eXtended Utils process manager class
* @note       Cant be construct Factory + singelton without depends of system. ITS NOT A SINGLETON.
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

#include "XPath.h"
#include "XString.h"
#include "XBuffer.h"
#include "XConsole.h"
#include "XSerializable.h"

#include "GRPRect.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPROCESS 
{
  public:
                                    XPROCESS                          ();
    virtual                        ~XPROCESS                          ();

    XDWORD                          GetID                             ();
    void                            SetID                             (XDWORD handle);

    XVECTOR<XDWORD>*                GetProcessIDs                     ();
  
    XPATH*                          GetPath                           ();
    XSTRING*                        GetName                           ();

    void*                           GetWindowHandle                   ();
    void                            SetWindowHandle                   (void* windowhandle);
    
    XSTRING*                        GetWindowTitle                    ();  
    GRPRECTINT*                     GetWindowRect                     ();      
    int                             GetWindowTitleHeight              ();    
    void                            SetWindowTitleHeight              (int windowtitleheight);    
    int                             GetWindowBorderWidth              (); 
    void                            SetWindowBorderWidth              (int windowborderwidth);
  
    bool                            CopyTo                            (XPROCESS& xprocess);    
    bool                            CopyFrom                          (XPROCESS& xprocess);    

    bool                            Compare                           (XPROCESS& xprocess, bool onlyfixed = true);  

  private:

    void                            Clean                             ();

    XDWORD                          ID;

    XVECTOR<XDWORD>                 processIDs;    
    XPATH                           path;  
    XSTRING                         name;  

    void*                           windowhandle;          
    XSTRING                         windowtitle; 
    GRPRECTINT                      windowrect; 
    int                             windowtitleheight;    
    int                             windowborderwidth;
};

class XPROCESSMANAGER
{
  public:
                                    XPROCESSMANAGER                   ();
    virtual                        ~XPROCESSMANAGER                   ();

    static bool                     GetIsInstanced                    ();
    static XPROCESSMANAGER&         GetInstance                       ();
    static bool                     SetInstance                       (XPROCESSMANAGER* instance);
    static bool                     DelInstance                       ();

    virtual bool                    MakeSystemCommand                 (XCHAR* command);    
    virtual bool                    MakeCommand                       (XCHAR* command, XBUFFER* out = NULL, int* returncode = NULL);

    virtual bool                    OpenURL                           (XCHAR* url);

    virtual bool                    Application_Execute               (XCHAR* applicationpath, XCHAR* params = NULL, XBUFFER* in = NULL, XBUFFER* out = NULL, int* returncode = NULL);        
    bool                            Application_Execute               (XCHAR* applicationpath, XCHAR* params, int* returncode);  
    bool                            Application_Execute               (XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode = NULL);

    bool                            AdjustStringToConsolaSymbolsUsed  (XSTRING& string, XBUFFER& target_buffer);
    bool                            AdjustConsolaSymbolsUsedToString  (XBUFFER& origin_buffer, XSTRING& string);


    virtual bool                    Application_ExecuteElevated       (XCHAR* applicationpath, XCHAR* params = NULL, XBUFFER* in = NULL, XBUFFER* out = NULL, int* returncode = NULL);
    bool                            Application_ExecuteElevated       (XCHAR* applicationpath, XCHAR* params, int* returncode);  
    bool                            Application_ExecuteElevated       (XCHAR* applicationpath, XCHAR* params, XSTRING* in, XSTRING* out, int* returncode = NULL);

    virtual bool                    Application_IsRunning             (XCHAR* command, XDWORD* ID = NULL);

    virtual bool                    Application_GetRunningList        (XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow = false);

    virtual bool                    Application_Terminate             (XDWORD processID, XDWORD exitcode = 0);
    bool                            Application_Terminate             (XPROCESS& process, XDWORD exitcode = 0);
    bool                            Application_Terminate             (XCHAR* processname, XDWORD exitcode = 0);
    bool                            Application_Terminate             (XSTRING& processname, XDWORD exitcode = 0);

    static XPROCESS*                Application_GetProcessByID        (XDWORD processID, XVECTOR<XPROCESS*>& applist);
  
  private:

    void                            Clean                             ();

    static XPROCESSMANAGER*         instance;        
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





