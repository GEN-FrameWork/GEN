/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProcLINUX.cpp
* 
* @class      MAINPROCLINUX
* @brief      LINUX Main procedure class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <utmp.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fenv.h>
#include <dlfcn.h>
#include <stdint.h>

#ifdef GOOGLETEST_ACTIVE      
#include "gtest/gtest.h"
#endif

#include "MainProcLINUX.h"

#include "XLINUXFactory.h"
#include "XLINUXTrace.h"
#include "XLINUXRand.h"
#include "XLINUXSleep.h"

#ifdef XSYSTEM_ACTIVE
#include "XLINUXSystem.h"
#endif

#ifdef XPROCESSMANAGER_ACTIVE
#include "XLINUXProcessManager.h"
#endif

#ifdef XSHAREDMEMORYMANAGER_ACTIVE
#include "XLINUXSharedMemoryManager.h"
#endif

#ifdef XDRIVEIMAGE_ACTIVE
#include "XLINUXDriveImage.h"
#endif

#ifdef XEEPROMMEMORYMANAGER_ACTIVE
#include "XLINUXEEPROMMemoryManager.h"
#endif


#ifdef GRP_ACTIVE
#include "GRPScreen.h"
#include "GRPLINUXFactory.h"
#endif

#ifdef INP_ACTIVE
#include "INPManager.h"
#include "INPFactory.h"
#include "INPLINUXFactory.h"
#endif

#ifdef SND_ACTIVE
#include "SNDLINUXFactory.h"
#endif

#ifdef DIO_ACTIVE
#include "DIODNSResolver.h"
#include "DIOLINUXFactory.h"

#ifdef DIO_GPIO_ACTIVE
#include "DIOLINUXGPIO.h"
#ifdef HW_INTEL
#ifdef DIO_GPIO_PCPARALLEL_ACTIVE
#include "DIOLINUXGPIOPCParallel.h"
#endif
#endif
#ifdef HW_RASPBERRYPI
#include "DIOLINUXGPIORPi.h"
#endif
#if defined(HW_ARM) || defined(HW_ARM64)
#include "DIOLINUXGPIOARM.h"
#endif
#endif

#ifdef DIO_PING_ACTIVE
#include "DIOLINUXPing.h"
#include "DIOPing.h"
#endif

#ifdef DIO_PUBLICINTERNETIP_ACTIVE
#include "DIOPublicInternetIP.h"
#endif

#ifdef DIO_ALERTS_ACTIVE
#include "DIOAlerts.h"
#endif
#endif

#include "VersionFrameWork.h"

#ifdef SCRIPT_CACHE_ACTIVE
#include "Script_Cache.h"
#endif

#include "XFeedback_Control.h"
#include "XRand.h"
#include "XSleep.h"
#include "XPath.h"
#include "XString.h"
#include "XLog.h"
#include "XFile.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XDateTime.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XPublisher.h"

#include "APPFlowBase.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
XLINUXTRACE           linuxdebugtrace;
#endif

MAINPROCLINUX         mainproclinux;
XSTRING               allexceptiontext;

bool                  libmainproclinux      = false;


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

void                  Signal_Ini                (void);
static void           Signal_Handler            (int sig);
bool                  Signal_Printf             (bool iserror, XCHAR* title, XCHAR* mask, ...);
static inline void    Signal_PrintfStackTrace   (FILE *out = stderr, unsigned int max_frames = 63);
bool                  Signal_RunLevel           (XCHAR& previous, XCHAR& actual);


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCLINUX::MAINPROCLINUX()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCLINUX::MAINPROCLINUX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCLINUX::~MAINPROCLINUX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCLINUX::~MAINPROCLINUX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCLINUX::Ini(APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
* @brief      Initialize the object
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  appmain : Appmain pointer to use.
* @param[in]  applicationmode : Applicationmode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCLINUX::Ini(APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
{
  this->appmain = appmain;

  //---------------------------------------------------------------------------
  #ifdef FPE_ACTIVE
  feenableexcept(FE_DIVBYZERO | FE_OVERFLOW /*| FE_UNDERFLOW | FE_INEXACT*/);
  #endif
  //---------------------------------------------------------------------------
  
  if(!Factorys_Ini()) 
    {
      return false;
    }
  
  #ifdef APPFLOW_ACTIVE

  if(!appmain)                              
    {
      return false;
    }

  if(!appmain->Create())                    
    {
      return false;
    }

  if(!appmain->Ini(this, applicationmode))  
    {
      return false;
    }

  #else

   if(Main_Proc_PlatformIni)
    {
      if(!Main_Proc_PlatformIni(this)) return false;
    }

  GEN_XPATHSMANAGER.GetAppExecPath()->Set(GetXPathExec()->Get());
  GetXPathExec()->SetOnlyDriveAndPath();
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT , (*GetXPathExec()));

  if(Main_Proc_Ini)
    {
      if(!Main_Proc_Ini(this)) return false;
    }

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCLINUX::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCLINUX::Update()
{
  #ifdef INP_ACTIVE
  GEN_INPMANAGER.Update();
  #endif

  #if defined(GRP_ACTIVE) && defined(GRP_SCREEN_CUSTOMCHROMES_ACTIVE)
  // Custom-chrome caption drag, ticked HERE -- right after the input devices have been read -- and not only
  // from GRPSCREEN::UpdateViewports(), which the application calls after its own DrawFrame(). That ordering
  // meant the window was always placed where the cursor had been one full rendered frame earlier; at a few
  // thousand pixels per second that is a visible gap, enough for the pointer to slide out of the caption
  // (and, before the pointer grab was added, out of the window altogether). The call left in
  // UpdateViewports() is kept for main loops that do not go through MAINPROC, and is a no-op here because
  // the tick recomputes an absolute target instead of accumulating deltas.
  GRPSCREEN::UpdateAllCFGChromesDrag();
  #endif

  #ifdef APPFLOW_ACTIVE

  if(appmain)
    {
      if(!appmain->Update()) return false;
    }

  #else

   if(Main_Proc_Update)
    {
      if(!Main_Proc_Update(this)) return false;
    }

  #endif

  #ifndef GRP_ACTIVE
  usleep(5000);
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCLINUX::End()
* @brief      End the object
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCLINUX::End()
{
  #ifdef APPFLOW_ACTIVE

  if(appmain) 
    {
      appmain->End();
    }

  #else

  if(Main_Proc_End)
    {
      if(!Main_Proc_End(this)) return false;
    }

  #endif

  #ifdef APPFLOW_ACTIVE

  if(appmain) 
    {
      appmain->Delete();
    }

  #else

  if(Main_Proc_PlatformEnd)
    {
      if(!Main_Proc_PlatformEnd(this)) return false;
    }

  #endif  

  DeleteAllExecParams();

  #ifdef SCRIPT_CACHE_ACTIVE 
  GEN_SCRIPT_CACHE.DelInstance();
  #endif

  GEN_VERSION.DelInstance();
  
  XFILE_DISPLAYNOTCLOSEFILES
  XFEEDBACK_CONTROL_DISPLAYFEEDBACK  
  XFEEDBACK_CONTROL_DELETE 

  Factorys_End();

  XMEMORY_CONTROL_DEACTIVATED
  XMEMORY_CONTROL_DISPLAYMEMORYLEAKS

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCLINUX::Factorys_Ini()
* @brief      Factorys ini
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCLINUX::Factorys_Ini()
{
  if(!XFACTORY::SetInstance(GEN_NEW XLINUXFACTORY())) 
    {
      return false;
    }

  XFEEDBACK_CONTROL_CREATE
  
  #ifdef XSYSTEM_ACTIVE  
  if(!XSYSTEM::SetInstance(GEN_NEW XLINUXSYSTEM())) 
    {
      return false;
    }
  XBUFFER::SetGlobalHardwareUseLittleEndian(GEN_XSYSTEM.HardwareUseLittleEndian());
  #endif  
  
  if(!XRAND::SetInstance(GEN_NEW XLINUXRAND())) 
    {
      return false;
    }

  #ifdef XSLEEP_ACTIVE
  if(!XSLEEP::SetInstance(GEN_NEW XLINUXSLEEP())) 
    {
      return false;
    }
  #endif
    
  #ifdef XPROCESSMANAGER_ACTIVE
  if(!XPROCESSMANAGER::SetInstance(GEN_NEW XLINUXPROCESSMANAGER())) 
    {
      return false;
    }
  #endif
  
  #ifdef XSHAREDMEMORYMANAGER_ACTIVE
  if(!XSHAREDMEMORYMANAGER::SetInstance(GEN_NEW XLINUXSHAREDMEMORYMANAGER())) 
    {
      return false;  
    }
  #endif
    
  #ifdef XDRIVEIMAGE_ACTIVE
  if(!XDRIVEIMAGE::SetInstance(GEN_NEW XLINUXDRIVEIMAGE())) 
    {
      return false;
    }
  #endif

  #ifdef XEEPROMMEMORYMANAGER_ACTIVE
  if(!XEEPROMMEMORYMANAGER::SetInstance(GEN_NEW XLINUXEEPROMMEMORYMANAGER())) 
    {
      return false;
    }
  #endif

  #ifdef XTRACE_VIRTUALCLOCKTICK
  xtimerclock = GEN_NEW XTIMERCLOCK();
  if(!xtimerclock) 
    {
      return false;
    }
  #endif

  #ifdef INP_ACTIVE
  if(!INPFACTORY::SetInstance(GEN_NEW INPLINUXFACTORY())) 
    {
      return false;  
    }
  #endif

  #ifdef DIO_ACTIVE
  if(!DIOFACTORY::SetInstance(GEN_NEW DIOLINUXFACTORY())) 
    {
      return false;
    }
  
    #ifdef DIO_GPIO_ACTIVE

      #ifdef HW_INTEL
        #ifdef DIO_GPIO_PCPARALLEL_ACTIVE
        if(!DIOGPIO::SetInstance(GEN_NEW DIOLINUXGPIOPCPARALLEL())) 
          {
            return false;
          }
        #endif
      #endif

      #ifdef HW_RASPBERRYPI
      if(!DIOGPIO::SetInstance(GEN_NEW DIOLINUXGPIORPI())) 
        {
          return false;
        }
      #endif

      #if defined(HW_ARM) || defined(HW_ARM64)
      if(!DIOGPIO::SetInstance(GEN_NEW DIOLINUXGPIOARM())) 
        {
          return false;
        }
      #endif
    
    if(!DIOGPIO::GetInstance().Ini()) 
      {
        return false;
      }

    #endif

    #ifdef DIO_PING_ACTIVE    
    if(!DIOPING::SetInstance(GEN_NEW DIOLINUXPING())) 
      {
        return false;
      }    
    #endif
	
  #endif

  #ifdef SND_ACTIVE
  if(!SNDFACTORY::SetInstance(GEN_NEW SNDLINUXFACTORY())) 
    {    
      return false;
    }

  if(!SNDFACTORY::GetInstance().Ini()) 
    {
      // return false;
    }
  #endif

  #ifdef GRP_ACTIVE
  if(!GRPFACTORY::SetInstance(GEN_NEW GRPLINUXFACTORY())) 
    {
      return false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCLINUX::Factorys_End()
* @brief      Factorys end
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCLINUX::Factorys_End()
{
  #ifdef GRP_ACTIVE
  GRPFACTORY::DelInstance();
  #endif

  #ifdef SND_ACTIVE
  if(SNDFACTORY::GetIsInstanced())
    {
      SNDFACTORY::GetInstance().End();
      SNDFACTORY::DelInstance();
    }
  #endif

  #ifdef DIO_ACTIVE
    #ifdef DIO_GPIO_ACTIVE
    if(DIOGPIO::GetIsInstanced())
      {
        DIOGPIO::GetInstance().End();
        DIOGPIO::DelInstance();
      }
    #endif

    #ifdef DIO_STREAMUDP_ACTIVE
    DIODNSRESOLVER::DelInstance();
    #endif

    #ifdef DIO_PING_ACTIVE    
    DIOPING::DelInstance();      
    #endif

  DIOFACTORY::DelInstance();
  #endif

  #ifdef INP_ACTIVE
  INPMANAGER::DelInstance();
  INPFACTORY::DelInstance();
  #endif

  #ifdef XTRACE_VIRTUALCLOCKTICK
  if(xtimerclock)
    {
      GEN_DELETE xtimerclock;
      xtimerclock = NULL;
    }
  #endif

  #ifdef DIO_PUBLICINTERNETIP_ACTIVE
  DIOPUBLICINTERNETIP::DelInstance();
  #endif

  #ifdef DIO_ALERTS_ACTIVE
  DIOALERTS::DelInstance();
  #endif 

  #ifdef XLOG_ACTIVE
  XLOG::DelInstance();
  #endif


  #ifdef XPROCESSMANAGER_ACTIVE
  XPROCESSMANAGER::DelInstance();
  #endif

  #ifdef XEEPROMMEMORYMANAGER_ACTIVE
  XEEPROMMEMORYMANAGER::DelInstance();
  #endif

  #ifdef XDRIVEIMAGE_ACTIVE
  XDRIVEIMAGE::DelInstance();
  #endif

  #ifdef XSHAREDMEMORYMANAGER_ACTIVE   
  XSHAREDMEMORYMANAGER::DelInstance();
  #endif  

  XPATHSMANAGER::DelInstance();

  XTRANSLATION::DelInstance();
  XTRANSLATION_GEN::DelInstance();

  #ifdef XSLEEP_ACTIVE
  XSLEEP::DelInstance();
  #endif

  XRAND::DelInstance();

  #ifdef XSYSTEM_ACTIVE
  XSYSTEM::DelInstance();
  #endif

  #ifdef XPUBLISHER_ACTIVE
  XPUBLISHER::DelInstance();
  #endif

  #ifdef XTHREADCOLLECTED_ACTIVE
  XTHREADSCOLLECTEDMANAGER::DelInstance();
  #endif

  XFACTORY::DelInstance();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCLINUX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCLINUX::Clean()
{

}





/*--------------------------------------------------------------------------------------------------------------------*/
/*  main        : Linux normal entry                                                                                  */
/*  LIBRARY_Ini :                                                                                                     */
/*  LIBRARY_End : Linux Library entry (Construct / Destruct)                                                          */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/


#if !defined(APPMODE_LIBRARY_STATIC_ACTIVE) && !defined(APPMODE_LIBRARY_DINAMIC_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int main(int argc, char* argv[])
* @brief      Main
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  argc : Argc value.
* @param[in]  argv : Argv pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  XMEMORY_CONTROL_ACTIVATED

  #ifdef GOOGLETEST_ACTIVE
  testing::InitGoogleTest(&argc, argv);
  #endif

  Signal_Ini();

  char  sztmp[64];
  int   pathsize = 512;
  char  path[pathsize];

  memset(path, 0, pathsize);

  sprintf(sztmp, "/proc/%d/exe", getpid());
  int rb = readlink(sztmp, path, pathsize);
  if(rb >= 0)
    {
      path[rb] = '\0';
      mainproclinux.GetXPathExec()->Set(path);
    }
   else mainproclinux.GetXPathExec()->Set(argv[0]);

  mainproclinux.CreateParams(argc, argv);

  int status = 0;

  #ifdef GOOGLETEST_ACTIVE
  int returngoogletest = 0;
  #endif
  
  #ifdef APPFLOW_ACTIVE
  if(!mainproclinux.Ini(&GEN_appmain, APPFLOWBASE_MODE_TYPE_APPLICATION))
  #else
  if(!mainproclinux.Ini())
  #endif
    {
      status=1;
    }

  if(!status)
    {
      #ifdef GOOGLETEST_ACTIVE      
      returngoogletest = RUN_ALL_TESTS();
      if(returngoogletest)
        {
          status = 1; 
        }
      #else
      while(mainproclinux.Update());
      #endif      
    }

  int returncode = 0;

  #ifdef APPFLOW_ACTIVE
  if(mainproclinux.GetAppMain())
    {
      if(mainproclinux.GetAppMain()->GetApplication())
        {
          returncode = mainproclinux.GetAppMain()->GetApplication()->GetExitType();
        }
    }
  #endif

  mainproclinux.GetXPathExec()->Empty();

  mainproclinux.End();

  #ifdef GOOGLETEST_ACTIVE
  // Match Windows MainProc: unit-test process exit code is gtest's, not AppFlow exit type
  // (tests may temporarily SetExitType and leave a non-zero value that would fail the stage).
  returncode = returngoogletest;
  #endif
      
  return returncode;
}


#else


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void LIBRARY_Ini(void)
* @brief      LIB Ini
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void LIBRARY_Ini(void)
{
  XMEMORY_CONTROL_ACTIVATED 

  char  xpathexecutable[_MAXPATH];
  int   status = 0;
  char  sztmp[64];

  memset(xpathexecutable, 0, _MAXPATH);

  sprintf(sztmp, "/proc/%d/exe", getpid());
  int rb = readlink(sztmp, xpathexecutable, _MAXPATH);
  if(rb >= 0) xpathexecutable[rb] = '\0';

  mainproclinux.GetXPathExec()->Set(xpathexecutable);
  #ifdef APPFLOW_ACTIVE
  mainproclinux.Ini(&GEN_appmain, APPFLOWBASE_MODE_TYPE_DINAMICLIBRARY);
  #else
  mainproclinux.Ini();
  #endif

  libmainproclinux = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void LIBRARY_End(void)
* @brief      LIB End
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void LIBRARY_End(void)
{
  mainproclinux.GetXPathExec()->Empty();
  mainproclinux.End();
}


#endif






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Signal_Ini(void)
* @brief      signal ini
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Signal_Ini(void)
{  
  struct sigaction  act;
  int               i = 0;

  memset(&act, 0, sizeof act);

  sigemptyset(&act.sa_mask);
  act.sa_handler = Signal_Handler;
  act.sa_flags = 0;

  int signalcap[] = {  SIGSEGV    , 
                       SIGBUS     ,
                       SIGFPE     , 
                       SIGABRT    , 
                       SIGSTKFLT  , 
                       SIGILL     , 
                       SIGHUP     , 
                       SIGTERM    , 
                       SIGINT     , 
                       SIGQUIT    , 
                       SIGTSTP    , 
                       SIGUSR1    ,
                       SIGUSR2    
                    }; 

  for(int c=0; c<(sizeof(signalcap)/sizeof(int)); c++)
    {
      if(sigaction(signalcap[c], &act, NULL)) 
        {          
          fprintf(stderr, "Cannot install realtime signal %d handler: %s.\n", i, strerror(errno));
          exit(EXIT_FAILURE);
        }
    }  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Signal_Handler(int sign)
* @brief      signal  handler
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  sign : Sign value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void Signal_Handler(int sign)
{
  XSTRING   signalstr;
  XSTRING   description;
  XSTRING   string;
  bool      iserror = false;

  allexceptiontext.Empty();

  #ifdef APPFLOW_ACTIVE
  APPFLOWBASE* app = NULL;
  if(mainproclinux.GetAppMain())     
    {
      app = mainproclinux.GetAppMain()->GetApplication();
    }

  if(app)
    {
      if(app->GetExitType()!=APPFLOWBASE_EXITTYPE_UNKNOWN) 
        {
          return;
        }
    }
  #endif

  switch(sign)
    {
      case SIGSEGV    : iserror     = true;
                        signalstr   = _L("SIGSEGV");
                        description = _L("Segmentation fault.");
                        break;

      case SIGBUS     : iserror     = true;
                        signalstr   = _L("SIGBUS");
                        description = _L("Bus fault.");
                        break;

      case SIGFPE     : iserror     = true;
                        signalstr   = _L("SIGFPE");
                        description = _L("Erroneous arithmetic operation (such as divide by zero).");
                        break;

      case SIGABRT    : iserror     = true;
                        signalstr   = _L("SIGABRT");
                        description = _L("Error: Abnormal termination condition.");
                        break;

      case SIGSTKFLT  : iserror     = true;
                        signalstr   = _L("SIGSTKFLT");
                        description = _L("Error: Stack fault.");
                        break;

      case SIGILL     : iserror     = true;
                        signalstr   = _L("SIGILL");
                        description = _L("Error: Illegal instruction.");
                        break;

      case SIGHUP     : signalstr   = _L("SIGHUP");
                        description = _L("Program hanged up.");
                        break;

      case SIGTERM    : signalstr   = _L("SIGTERM");
                        description = _L("Termination request, sent to the program.");
                        break;

      case SIGINT     : signalstr   = _L("SIGINT");
                        description = _L("External interrupt, usually initiated by the user.");
                        break;

      case SIGQUIT    : signalstr   = _L("SIGQUIT");
                        description = _L("Terminal quit.");
                        break;

      case SIGTSTP    : signalstr   = _L("SIGTSTP");
                        description = _L("Terminal stop signalstr.");
                        break;

      case SIGUSR1    :
      case SIGUSR2    : signalstr = _L("SIGUSR");
                        description = _L("User signal received.");
                        break;
    }

  Signal_Printf(iserror, LOG_SIGNAL, _L("[%s %d] Error: %s"), signalstr.Get(), sign, description.Get());

  #ifdef APPFLOW_ACTIVE
  if(app)
    {
      if(app->GetTimerGlobal())
        {
          XSTRING string2;
        
          app->GetTimerGlobal()->GetMeasureString(string2, true);
          Signal_Printf(iserror, LOG_SIGNAL, _L("Time working: %s."), string2.Get());
        }
    }
  #endif

  switch(sign)
    {
      case SIGSEGV    :
      case SIGBUS     :
      case SIGFPE     :
      case SIGABRT    :
      case SIGSTKFLT  :
      case SIGILL     : { Signal_PrintfStackTrace();
  
                          string.Format(_L("SIGNAL %s: %s"), signalstr.Get(), description.Get());

                          #ifdef DIO_ALERTS_ACTIVE
                          DIOALERT* alert = GEN_DIOALERTS.CreateAlert(DIOALERTLEVEL_DANGER, string.Get(), allexceptiontext.Get());
                          if(alert)
                            {
                              GEN_DIOALERTS.Send(DIOALERTSSENDER_ALL, DIOALERTS_CONDITIONS_ID_GENINTERN_EXCEPTION, alert);
                              GEN_DELETE alert;
                            }
                          #endif

                          allexceptiontext.Empty();

                          fprintf(stderr, " \n");

                          #ifdef APPFLOW_ACTIVE
                          if(app)
                            {
                              app->SetExitType(APPFLOWBASE_EXITTYPE_BY_SERIOUSERROR);
                              app->AppProc_End();
                            }
                          #endif

                          signal(sign, SIG_DFL);
                          kill(getpid(), sign);
                        }
                        break;

      case SIGINT     :
                        #ifdef APPFLOW_ACTIVE
                        if(app) 
                          {
                            app->SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);                                                  
                          }
                        #endif

                        allexceptiontext.Empty();
                        break;

      case SIGQUIT    :
                        #ifdef APPFLOW_ACTIVE
                        if(app) 
                          {
                            app->SetExitType(APPFLOWBASE_EXITTYPE_BY_SHUTDOWN);
                          } 
                        #endif

                        allexceptiontext.Empty();
                        break;

      case SIGHUP     :
      case SIGTERM    : { XCHAR previous;
                          XCHAR actual;

                          #ifdef APPFLOW_ACTIVE
                          if(app)
                            {
                              if(app->GetExitType() != APPFLOWBASE_EXITTYPE_UNKNOWN) 
                                {
                                  return;
                                }
                            }
                          #endif

                          if(Signal_RunLevel(previous, actual))
                            {
                              if(actual == _C('0'))
                                {
                                  #ifdef APPFLOW_ACTIVE
                                  if(app) 
                                    {
                                      app->SetExitType(APPFLOWBASE_EXITTYPE_BY_SHUTDOWN);
                                    }
                                  #endif

                                  allexceptiontext.Empty();
                                  break;
                                }
                            }

                          #ifdef APPFLOW_ACTIVE
                          if(app) 
                            {
                              app->SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                             }
                          #endif

                          allexceptiontext.Empty();
                        }
                        break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool Signal_Printf(bool iserror, XCHAR* title, XCHAR* mask, ...)
* @brief      ignal_Printf
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  iserror : Iserror value.
* @param[in]  title : Title text.
* @param[in]  mask : Mask pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool Signal_Printf(bool iserror, XCHAR* title, XCHAR* mask, ...)
{
  XSTRING outstring;

  va_list arg;

  va_start(arg, mask);

  outstring.FormatArg(mask, &arg);

  va_end(arg);

  bool do_log = false;

  #ifdef XLOG_ACTIVE
  if(GEN_XLOG.IsActive())
    {
      GEN_XLOG.SetFilters(LOG_SIGNAL);

      if(GEN_XLOG.AddEntry(iserror?XLOGLEVEL_ERROR:XLOGLEVEL_WARNING, title, false, outstring.Get()))
        {
          do_log = true;
        }
    }
  #endif

  allexceptiontext += outstring;
  allexceptiontext += _L("\r\n");

  #ifdef XTRACE_ACTIVE
  if(!do_log)
    {
      int level = iserror?4:0;

     XTRACE_PRINTCOLOR(level, outstring.Get());
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline bool Signal_ResolveFunctionName(void* addr, XSTRING& namefunc)
* @brief      nline bool  signal  resolve function name
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  addr : Addr pointer to use.
* @param[in]  namefunc : Namefunc value.
* 
* @return     static inline bool : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static inline bool Signal_ResolveFunctionName(void* addr, XSTRING& namefunc)
{
  Dl_info info;

  namefunc.Empty();

  namefunc = _L("?");

  if(dladdr(addr, &info) == 0)
    {    
      return false;
    }

  if(!info.dli_sname)
    {
      return false;
    }

  int status = -1;
  char* dem = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
  
  if(status == 0 && dem)
    {
      namefunc = dem;
    }
  else
    {
      namefunc = info.dli_sname;
    }

  free(dem);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline void Signal_PrintfStackTrace(FILE *out, unsigned int max_frames)
* @brief      nline void Signal_PrintfStackTrace
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  out : Output out.
* @param[in]  max_frames : Max frames value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
static inline void Signal_PrintfStackTrace(FILE *out, unsigned int max_frames)
{
  void*  addrlist[max_frames+1];        // storage array for stack trace address data
  XDWORD addrlen = 0;

  memset(addrlist, 0, sizeof(addrlist));

  addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*)); 

  if(addrlen == 0)
    {
      Signal_Printf(true, LOG_SIGNAL, _L("Stack trace: Not available."));
      return;

    } else Signal_Printf(true, LOG_SIGNAL, _L("Stack trace: (%d) calls"), addrlen);

  char** symbollist = backtrace_symbols(addrlist, addrlen);

  for(int c=0; c<addrlen; c++)
    {
      XSTRING symbolstr = symbollist[c];
      XSTRING namefunc;

      Signal_ResolveFunctionName(addrlist[c], namefunc);
      
      Signal_Printf(true, LOG_SIGNAL, _L("[%-64s] %s"), namefunc.Get(), symbolstr.Get());      
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool Signal_RunLevel(XCHAR& previous, XCHAR& actual)
* @brief      ignal_RunLevel
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  previous : Previous value.
* @param[in]  actual : Actual value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool Signal_RunLevel(XCHAR& previous, XCHAR& actual)
{
  struct utmp* ut;
  bool         status = false;

  previous = 0;
  actual   = 0;

  setutent();

  while((ut = getutent()) != NULL)
    {
      if(ut->ut_type == RUN_LVL)
        {
          previous = (XCHAR)(ut->ut_pid / 256);
          if(!previous) previous = _C('N');

          actual   = (XCHAR)(ut->ut_pid % 256);

          status = true;
        }
    }

  endutent();

  return status;
}






