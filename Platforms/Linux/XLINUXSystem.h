/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXSystem.h
* 
* @class      XLINUXSYSTEM
* @brief      LINUX eXtended Utils System class
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSystem.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define   _PATH_UMOUNT		  "/bin/umount"
#define   _PATH_PASSWD      "/etc/passwd"
#define   _PATH_MTAB		    "/etc/mtab"
#define   UMOUNT_ARGS		    "umount", "-a"
#define   SWAPOFF_ARGS      "swapoff", "-a"

//#define   WR(s) write(fd, s, strlen(s))

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XLINUXSYSTEM : public XSYSTEM
{
  public:
                                XLINUXSYSTEM                  ();
    virtual                    ~XLINUXSYSTEM                  ();

    XSYSTEM_HARDWARETYPE        GetTypeHardware               (int* revision = NULL);

    XSYSTEM_PLATFORM            GetPlatform                   (XSTRING* namestring = NULL);  

    bool                        GetOperativeSystemID          (XSTRING& ID);

    XDWORD                      GetLanguageSO                 ();

    XSTRING*                    GetBIOSSerialNumber           ();

    XSTRING*                    GetCPUSerialNumber            ();

    float                       GetCPUTemperature             ();

    bool                        GetMemoryInfo                 (XDWORD& total, XDWORD& free);
    bool                        FreeCacheMemory               ();

    bool                        GetVolumesInfo                (XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes);

    int			                    GetCPUUsageTotal			        ();
    int							            GetCPUUsageForProcessName	    (XCHAR* processname);
    int							            GetCPUUsageForProcessID		    (XDWORD processID);

    XCHAR*                      GetEnviromentVariable         (XCHAR* name);
    bool                        SetEnviromentVariable         (XCHAR* name, XCHAR* value);
    bool                        DelEnviromentVariable         (XCHAR* name);

    bool                        ShutDown                      (XSYSTEM_CHANGESTATUSTYPE type);

    int                         Sound_GetLevel                ();  
    bool                        Sound_SetLevel                (int level); 
    bool                        Sound_SetMutex                (bool on);   

  private:

    bool                        Sound_SetLevel                (bool read, long* level);
    void                        Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





