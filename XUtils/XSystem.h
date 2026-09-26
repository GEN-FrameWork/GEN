/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSystem.h
* 
* @class      XSYSTEM
* @brief      System class
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

#include "XString.h"
#include "XPath.h"
#include "XFileTXT.h"
#include "XVector.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XSYSTEM_HARDWARETYPE
{
  XSYSTEM_HARDWARETYPE_UNKNOWN                    = 0     ,

  XSYSTEM_HARDWARETYPE_PC                                 ,
  XSYSTEM_HARDWARETYPE_LEX3V700A                          ,

  XSYSTEM_HARDWARETYPE_ARMGENERIC                         ,  
  XSYSTEM_HARDWARETYPE_ARTILA500                          ,  
  XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX                      ,
  XSYSTEM_HARDWARETYPE_RASPBERRYPI                        ,
  XSYSTEM_HARDWARETYPE_BEAGLEBONE                         , 
  XSYSTEM_HARDWARETYPE_NXP_IMX6                           ,
  XSYSTEM_HARDWARETYPE_NXP_IMX8                           ,

  XSYSTEM_HARDWARETYPE_MICRO_STM32                        ,
  XSYSTEM_HARDWARETYPE_MICRO_ESP32	    				  , 
  XSYSTEM_HARDWARETYPE_MICRO_SAMD5XE5X                    , 
  
  XSYSTEM_HARDWARETYPE_END
};


enum XSYSTEM_SO
{
  XSYSTEM_SO_UNKNOWN                            = 0x0000  ,

  XSYSTEM_SO_WINDOWS                            = 0x0100  ,
  XSYSTEM_SO_WINDOWS95                                    ,
  XSYSTEM_SO_WINDOWS98                                    ,
  XSYSTEM_SO_WINDOWSNT4                                   ,
  XSYSTEM_SO_WINDOWS2000                                  ,
  XSYSTEM_SO_WINDOWSXP                                    ,
  XSYSTEM_SO_WINDOWSSERVER2003                            ,
  XSYSTEM_SO_WINDOWSVISTA                                 ,
  XSYSTEM_SO_WINDOWS7                                     ,
  XSYSTEM_SO_WINDOWS8                                     ,
  XSYSTEM_SO_WINDOWS81                                    ,
  XSYSTEM_SO_WINDOWS10                                    ,
  XSYSTEM_SO_WINDOWS11                                    ,

  XSYSTEM_SO_LINUX                              = 0x0200  ,

  XSYSTEM_SO_LINUX_EMBEDDED                     = 0x0400  ,

  XSYSTEM_SO_ANDROID                            = 0x0800  ,
};


enum XSYSTEM_PLATFORM
{
  XSYSTEM_PLATFORM_UNKNOWN                      =       0 ,
  XSYSTEM_PLATFORM_WINDOWS                                ,  
  XSYSTEM_PLATFORM_LINUX                                  ,
  XSYSTEM_PLATFORM_LINUX_EMBEDDED                         ,
  XSYSTEM_PLATFORM_ANDROID                                ,
  XSYSTEM_PLATFORM_STM32								  ,
  XSYSTEM_PLATFORM_ESP32								  ,
  XSYSTEM_PLATFORM_SAMD5XE5X 									
};


enum XSYSTEM_CHANGESTATUSTYPE
{
  XSYSTEM_CHANGESTATUSTYPE_NONE                 =       0 ,
  XSYSTEM_CHANGESTATUSTYPE_REBOOT                         ,
  XSYSTEM_CHANGESTATUSTYPE_CONSOLE_CONNECT                ,
  XSYSTEM_CHANGESTATUSTYPE_CONSOLE_DISCONNECT             ,
  XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGON                  ,
  XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGOFF                 ,
  XSYSTEM_CHANGESTATUSTYPE_SESSION_LOCK                   ,
  XSYSTEM_CHANGESTATUSTYPE_SESSION_UNLOCK                 ,
  XSYSTEM_CHANGESTATUSTYPE_POWEROFF                       ,
  XSYSTEM_CHANGESTATUSTYPE_SUSPEND
};

enum XSYSTEM_VOLUME_TYPE
{
  XSYSTEM_VOLUME_TYPE_UNKNOWN                   =       0 ,
  XSYSTEM_VOLUME_TYPE_FIXED                               ,
  XSYSTEM_VOLUME_TYPE_REMOVABLE                           ,
  XSYSTEM_VOLUME_TYPE_REMOTE                              ,
  XSYSTEM_VOLUME_TYPE_CDROM                               ,
  XSYSTEM_VOLUME_TYPE_RAMDISK
};

#define XSYSTEM_CPUUSAGE_ERROR              -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSYSTEM_VOLUMEINFO
{
  public:
                                    XSYSTEM_VOLUMEINFO              ();
    virtual                        ~XSYSTEM_VOLUMEINFO              ();

    XSTRING*                        GetName                         ();
    XSTRING*                        GetLabel                        ();
    XSTRING*                        GetFileSystem                   ();

    XSYSTEM_VOLUME_TYPE             GetType                         ();
    void                            SetType                         (XSYSTEM_VOLUME_TYPE type);

    XQWORD                          GetTotalBytes                   ();
    void                            SetTotalBytes                   (XQWORD totalbytes);

    XQWORD                          GetFreeBytes                    ();
    void                            SetFreeBytes                    (XQWORD freebytes);

    XQWORD                          GetUsedBytes                    ();
    float                           GetUsedPercent                  ();

  private:

    void                            Clean                           ();

    XSTRING                         name;
    XSTRING                         label;
    XSTRING                         filesystem;
    XSYSTEM_VOLUME_TYPE             type;
    XQWORD                          totalbytes;
    XQWORD                          freebytes;
};

class XSYSTEM
{
  public:
                                    XSYSTEM                         ();
    virtual                        ~XSYSTEM                         ();

    static bool                     GetIsInstanced                  ();
    static XSYSTEM&                 GetInstance                     ();
    static bool                     SetInstance                     (XSYSTEM* instance);
    static bool                     DelInstance                     ();

    virtual XSYSTEM_HARDWARETYPE    GetTypeHardware                 (int* revision = NULL);

    bool                            HardwareUseLittleEndian         ();
        
    virtual XSYSTEM_PLATFORM        GetPlatform                     (XSTRING* namestring = NULL);

    virtual bool					          GetOperativeSystemID	          (XSTRING& ID);

    virtual XDWORD                  GetLanguageSO                   ();

    virtual XSTRING*                GetBIOSSerialNumber             ();

    virtual XSTRING*                GetCPUSerialNumber              ();

    virtual float                   GetCPUTemperature               ();

    virtual bool                    GetMemoryInfo                   (XDWORD& total, XDWORD& free);
    int                             GetFreeMemoryPercent            ();
    virtual bool                    FreeCacheMemory                 ();

    virtual bool                    GetVolumesInfo                  (XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes);

    virtual int                     GetCPUUsageTotal                ();  
    virtual int                     GetCPUUsageForProcessName       (XCHAR* processname);  
    virtual int                     GetCPUUsageForProcessID         (XDWORD processID);  

    virtual XCHAR*                  GetEnviromentVariable           (XCHAR* name);
    XCHAR*                          GetEnviromentVariable           (XSTRING& name);
    virtual bool                    SetEnviromentVariable           (XCHAR* name, XCHAR* value);
    bool                            SetEnviromentVariable           (XSTRING& name, XSTRING& value);
    virtual bool                    DelEnviromentVariable           (XCHAR* name);
    bool                            DelEnviromentVariable           (XSTRING& name);

    virtual bool                    GetUserAndDomain                (XSTRING& user, XSTRING& domain);

    virtual bool                    ShutDown                        (XSYSTEM_CHANGESTATUSTYPE type);

    virtual int                     Sound_GetLevel                  ();  
    virtual bool                    Sound_SetLevel                  (int level); 
    virtual bool                    Sound_SetMutex                  (bool on); 

    virtual bool                    GetBatteryLevel                 (bool& isincharge, XBYTE& levelpercent); 
    virtual int                     GetWifiRSSILevel                ();
    
    virtual bool                    GetPathExecApplication          (XCHAR* appname, XPATH& apppath);      
    bool                            GetPathExecApplication          (XSTRING* appname, XPATH& apppath);      
		
		#ifndef MICROCONTROLLER

    bool                            HostFile                        (bool add, XCHAR* domain, XCHAR* IP);
    bool                            HostFile                        (bool add, XSTRING& domain, XSTRING& IP);
		
		#endif

  protected:

    XSTRING                         BIOSserialnumber; 
    XSTRING                         CPUserialnumber; 
    XMUTEX*                         xmutexcheckCPUusage;  

  private:

    int                             IsLineInHostFile                (XFILETXT& xfileTXT, XCHAR* domain, XCHAR* IP);

    void                            Clean                           ();

    static XSYSTEM*                 instance;
   
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






