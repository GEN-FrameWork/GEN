/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSSystem.cpp
* 
* @class      XWINDOWSSYSTEM
* @brief      WINDOWS eXtended Utils System class
* @ingroup    PLATFORM_WINDOWS
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

#include "XWINDOWSSystem.h"

#define SECURITY_WIN32

#include <cstdio>
#include <windows.h>
#include <process.h>
#include <tlhelp32.h>
#include <Commctrl.h>
#include <Security.h>
#include <powrprof.h>


#ifndef BUILDER
#include <wlanapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#endif


#ifndef BUILDER
#if(_MSC_PLATFORM_TOOLSET == 'v140')
#include <versionhelpers.h>
#endif
#endif

#ifndef BUILDER
#include "XWINDOWSWMIInterface.h"
#endif

#include "XWINDOWSRegistryManager.h"

#include "XFactory.h"
#include "XString.h"
#include "XSleep.h"
#include "XFile.h"
#include "XFileTXT.h"
#include "XTrace.h"
#include "XLanguage_ISO_639_3.h"

#ifndef BUILDER
//#pragma comment(lib, "wlanapi.lib")
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGE::XWINDOWSSYSTEM_CPUUSAGE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGE::XWINDOWSSYSTEM_CPUUSAGE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGE::~XWINDOWSSYSTEM_CPUUSAGE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGE::~XWINDOWSSYSTEM_CPUUSAGE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double XWINDOWSSYSTEM_CPUUSAGE::GetTotalCpuUsage()
* @brief      Get total cpu usage
* @ingroup    PLATFORM_WINDOWS
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double XWINDOWSSYSTEM_CPUUSAGE::GetTotalCpuUsage() 
{
  #ifndef BUILDER
  FILETIME idletime;
  FILETIME kerneltime;
  FILETIME usertime;

  if(GetSystemTimes(&idletime, &kerneltime, &usertime) == 0)
    {
      return -1;
    }

  ULONGLONG sysidlediff   = SubtractTimes(idletime, lastsysidletime);
  ULONGLONG syskerneldiff = SubtractTimes(kerneltime, lastsyskerneltime);
  ULONGLONG sysuserdiff   = SubtractTimes(usertime, lastsysusertime);

  ULONGLONG systotal      = syskerneldiff + sysuserdiff;
  ULONGLONG totalsystime  = systotal + sysidlediff;

  lastsysidletime         = idletime;
  lastsyskerneltime       = kerneltime;
  lastsysusertime         = usertime;

  if(totalsystime == 0)
    {
      return -1;
    }

  return (systotal * 100.0) / totalsystime;
  
  #else
  return 0.0f;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double XWINDOWSSYSTEM_CPUUSAGE::GetProcessCpuUsage(DWORD processID)
* @brief      Get process cpu usage
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processID : Process ID value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double XWINDOWSSYSTEM_CPUUSAGE::GetProcessCpuUsage(DWORD processID) 
{
  #ifndef BUILDER
  HANDLE hprocess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
  if(hprocess == NULL) 
    {
      return -1;
    }

  FILETIME proccreationtime;
  FILETIME procexittime; 
  FILETIME prockerneltime;
  FILETIME procusertime;

  if(GetProcessTimes(hprocess, &proccreationtime, &procexittime, &prockerneltime, &procusertime) == 0) 
    {
      CloseHandle(hprocess);
      return -1;
    }

  ULONGLONG prockerneldiff  = SubtractTimes(prockerneltime, lastprockerneltime);
  ULONGLONG procuserdiff    = SubtractTimes(procusertime, lastprocusertime);

  lastprockerneltime  = prockerneltime;
  lastprocusertime    = procusertime;

  CloseHandle(hprocess);

  if(!lastsystemtime) 
    {
      lastsystemtime  = GetTickCount64();
      lastprocesstime = prockerneldiff + procuserdiff;

      return -1;
    }

  ULONGLONG currenttime     = GetTickCount64();
  ULONGLONG timediff        = currenttime - lastsystemtime;
  ULONGLONG processtime     = prockerneldiff + procuserdiff;
  ULONGLONG processtimediff = processtime - lastprocesstime;

  lastsystemtime  = currenttime;
  lastprocesstime = processtime;

  return (processtimediff * 100.0) / (timediff * GetNumberOfProcessors());
  #else
  return 0.0f;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ULONGLONG XWINDOWSSYSTEM_CPUUSAGE::SubtractTimes(const FILETIME& fta, const FILETIME& ftb)
* @brief      Subtract times
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  fta : Fta value.
* @param[in]  ftb : Ftb value.
* 
* @return     ULONGLONG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
ULONGLONG XWINDOWSSYSTEM_CPUUSAGE::SubtractTimes(const FILETIME& fta, const FILETIME& ftb) 
{
  ULARGE_INTEGER a, b;

  a.LowPart   = fta.dwLowDateTime;
  a.HighPart  = fta.dwHighDateTime;

  b.LowPart   = ftb.dwLowDateTime;
  b.HighPart  = ftb.dwHighDateTime;

  return a.QuadPart - b.QuadPart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM_CPUUSAGE::GetNumberOfProcessors()
* @brief      Get number of processors
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM_CPUUSAGE::GetNumberOfProcessors() 
{
  SYSTEM_INFO sysInfo;
  
  GetSystemInfo(&sysInfo);
  
  return sysInfo.dwNumberOfProcessors;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSSYSTEM_CPUUSAGE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSYSTEM_CPUUSAGE::Clean()
{
  lastsystemtime    = 0;
  lastprocesstime   = 0;
	      
  ZeroMemory(&lastsysidletime, sizeof(FILETIME));
  ZeroMemory(&lastsyskerneltime, sizeof(FILETIME));
  ZeroMemory(&lastsysusertime, sizeof(FILETIME));
  ZeroMemory(&lastprockerneltime, sizeof(FILETIME));
  ZeroMemory(&lastprocusertime, sizeof(FILETIME));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS::XWINDOWSSYSTEM_CPUUSAGESTATUS()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS::XWINDOWSSYSTEM_CPUUSAGESTATUS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS::~XWINDOWSSYSTEM_CPUUSAGESTATUS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS::~XWINDOWSSYSTEM_CPUUSAGESTATUS()
{
  Clean();
}

	
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSSYSTEM_CPUUSAGESTATUS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSYSTEM_CPUUSAGESTATUS::Clean()
{
  processname.Empty();		
	processID                  = 0xFFFFFFFF;
  firsttime                   = true;
	oldvalue                    = 0;
  oldperftime100nsec.QuadPart = 0;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM::XWINDOWSSYSTEM() : XSYSTEM()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM::XWINDOWSSYSTEM() : XSYSTEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM::~XWINDOWSSYSTEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM::~XWINDOWSSYSTEM()
{
  DeleteAllCPUUsageStatus();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_HARDWARETYPE XWINDOWSSYSTEM::GetTypeHardware(int* revision)
* @brief      Get type hardware
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  revision : Revision pointer to use.
* 
* @return     XSYSTEM_HARDWARETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XWINDOWSSYSTEM::GetTypeHardware(int* revision)
{
  if(revision) (*revision) = -1;

  return XSYSTEM_HARDWARETYPE_PC;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_PLATFORM XWINDOWSSYSTEM::GetPlatform(XSTRING* namestring)
* @brief      Get platform
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  namestring : Namestring pointer to use.
* 
* @return     XSYSTEM_PLATFORM : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_PLATFORM XWINDOWSSYSTEM::GetPlatform(XSTRING* namestring)
{
  if(namestring)  namestring->Set(__L("Windows"));

  return XSYSTEM_PLATFORM_WINDOWS;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetOperativeSystemID(XSTRING& ID)
* @brief      Get operative system ID
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetOperativeSystemID(XSTRING& ID)
{
  ID.Empty();

  #ifndef BUILDER
  XWINDOWSWMIINTERFACE wmiinterface;
  XSTRING              wmianswer[4];
  
  if(!wmiinterface.Ini())
    {
      return false;
    }

  wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("Caption"), wmianswer[0]);
  wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("BuildNumber"), wmianswer[1]);
  wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("CSDVersion"), wmianswer[2]);
  wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("OSArchitecture") , wmianswer[3]);

  ID.Format(__L("%s Build(%s) %s %s"), wmianswer[0].Get(), wmianswer[1].Get(), wmianswer[2].Get(), wmianswer[3].Get());

  wmiinterface.End();          
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XWINDOWSSYSTEM::GetLanguageSO()
* @brief      Get language SO
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XWINDOWSSYSTEM::GetLanguageSO()
{
  XLANGUAGE_ISO_639_3 ISO639;
  XSTRING             languageenglish;

  languageenglish.AdjustSize(_MAXSTR);

  LCID lcID = GetUserDefaultLCID();
  GetLocaleInfo( lcID, LOCALE_SENGLANGUAGE, languageenglish.Get(), _MAXSTR);

  languageenglish.AdjustSize();

  return ISO639.Code_GetByEnglishName(languageenglish.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XWINDOWSSYSTEM::GetBIOSSerialNumber()
* @brief      Get BIOS serial number
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XWINDOWSSYSTEM::GetBIOSSerialNumber()
{
  #ifndef BUILDER
  XWINDOWSWMIINTERFACE wmiinterface;

  if(wmiinterface.Ini())
    {
      wmiinterface.DoQuery(__L("Win32_BIOS"), __L("SerialNumber"), BIOSserialnumber);

      wmiinterface.End();
    }
  #endif

  return &BIOSserialnumber;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XWINDOWSSYSTEM::GetCPUSerialNumber()
* @brief      Get CPU serial number
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XWINDOWSSYSTEM::GetCPUSerialNumber()
{
  #ifndef BUILDER
  XWINDOWSWMIINTERFACE wmiinterface;
  if(wmiinterface.Ini())
    { 
      wmiinterface.DoQuery(__L("Win32_Processor"), __L("ProcessorId"), CPUserialnumber);
      wmiinterface.End();     
    }
  #endif

  return &CPUserialnumber;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XWINDOWSSYSTEM::GetCPUTemperature()
* @brief      Get CPU temperature
* @ingroup    PLATFORM_WINDOWS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XWINDOWSSYSTEM::GetCPUTemperature()
{
  XSTRING CPUtemperaturestr;

  #ifndef BUILDER
  XWINDOWSWMIINTERFACE wmiinterface;
  if(wmiinterface.Ini())
    { 
      wmiinterface.DoQuery(__L("Win32_PerfFormattedData_Counters_ThermalZoneInformation"), __L("Temperature"), CPUtemperaturestr);

      wmiinterface.End();       
    }
  #endif

  int   CPUtemperature = 0;
  float CPUtemperaturefloat = 0.0f;

  CPUtemperature = CPUtemperaturestr.ConvertToInt();

  CPUtemperaturefloat   = (float)CPUtemperature;
  CPUtemperaturefloat  -= 273.2f;

  return CPUtemperaturefloat;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
* @brief      Get memory info
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  total : total memory in KB.
* @param[in]  free : free memory in KB.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
{
  /*
  MEMORYSTATUS mem;

  GlobalMemoryStatus(&mem);

  total = (XDWORD)(mem.dwTotalPhys/1024);
  free  = (XDWORD)(mem.dwAvailPhys/1024);
  */

  total = 0;
  free  = 0;

  #ifndef BUILDER
  XWINDOWSWMIINTERFACE wmiinterface;
  XSTRING              wmianswer;

  if(wmiinterface.Ini())
    { 
      if(wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("TotalVisibleMemorySize"), wmianswer))
        {
          total = wmianswer.ConvertToInt();      
        }

      if(wmiinterface.DoQuery(__L("Win32_OperatingSystem"), __L("FreePhysicalMemory"), wmianswer))
        {
          free = wmianswer.ConvertToInt();      
        }
     
      wmiinterface.End();
    } 
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetVolumesInfo(XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes)
* @brief      Enumerate logical drives with total/free space, label and file system.
* @ingroup    PLATFORM_WINDOWS
* 
* @param[out] volumes : Cleared and filled with GEN_NEW entries (caller DeleteContents).
* 
* @return     bool : true if at least one volume was added; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetVolumesInfo(XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes)
{
  volumes.DeleteContents();

  DWORD buffersize = GetLogicalDriveStrings(0, NULL);
  if(!buffersize) return false;

  XSTRING drives;
  drives.AdjustSize((XDWORD)buffersize);

  if(!GetLogicalDriveStrings(buffersize, drives.Get()))
    {
      drives.AdjustSize();
      return false;
    }

  drives.AdjustSize();

  XCHAR* cursor = drives.Get();
  if(!cursor) return false;

  while(cursor[0])
    {
      XSTRING root;
      root = cursor;

      UINT drivetype = GetDriveType(root.Get());
      XSYSTEM_VOLUME_TYPE volumetype = XSYSTEM_VOLUME_TYPE_UNKNOWN;

      switch(drivetype)
        {
          case DRIVE_FIXED      : volumetype = XSYSTEM_VOLUME_TYPE_FIXED;     break;
          case DRIVE_REMOVABLE  : volumetype = XSYSTEM_VOLUME_TYPE_REMOVABLE; break;
          case DRIVE_REMOTE     : volumetype = XSYSTEM_VOLUME_TYPE_REMOTE;    break;
          case DRIVE_CDROM      : volumetype = XSYSTEM_VOLUME_TYPE_CDROM;     break;
          case DRIVE_RAMDISK    : volumetype = XSYSTEM_VOLUME_TYPE_RAMDISK;   break;
          default               : volumetype = XSYSTEM_VOLUME_TYPE_UNKNOWN;   break;
        }

      if(volumetype != XSYSTEM_VOLUME_TYPE_UNKNOWN)
        {
          ULARGE_INTEGER freebytestocaller;
          ULARGE_INTEGER totalbytes;
          ULARGE_INTEGER totalfreebytes;

          freebytestocaller.QuadPart = 0;
          totalbytes.QuadPart        = 0;
          totalfreebytes.QuadPart    = 0;

          if(GetDiskFreeSpaceEx(root.Get(), &freebytestocaller, &totalbytes, &totalfreebytes))
            {
              XSYSTEM_VOLUMEINFO* volume = GEN_NEW XSYSTEM_VOLUMEINFO();
              if(volume)
                {
                  volume->GetName()->Set(root.Get());
                  volume->SetType(volumetype);
                  volume->SetTotalBytes((XQWORD)totalbytes.QuadPart);
                  volume->SetFreeBytes((XQWORD)freebytestocaller.QuadPart);

                  XSTRING vollabel;
                  XSTRING volfs;

                  vollabel.AdjustSize(_MAXSTR);
                  volfs.AdjustSize(_MAXSTR);

                  if(GetVolumeInformation(root.Get(),
                                          vollabel.Get(), vollabel.GetSize(),
                                          NULL, NULL, NULL,
                                          volfs.Get(), volfs.GetSize()))
                    {
                      vollabel.AdjustSize();
                      volfs.AdjustSize();
                      volume->GetLabel()->Set(vollabel.Get());
                      volume->GetFileSystem()->Set(volfs.Get());
                    }
                   else
                    {
                      vollabel.AdjustSize();
                      volfs.AdjustSize();
                    }

                  volumes.Add(volume);
                }
            }
        }

      cursor += root.GetSize() + 1;
    }

  return (!volumes.IsEmpty());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM::GetCPUUsageTotal()
* @brief      Get CPU usage total
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM::GetCPUUsageTotal()
{
  int				                              cpuusage            = 0;
  
  #ifndef BUILDER
  XWINDOWSSYSTEM_PERFCOUNTERS<LONGLONG>   perfcounters;
	DWORD                                   objectindex;
	DWORD                                   cpuusageindex;	
	LONGLONG		                            newvalue            = 0;
	PPERF_DATA_BLOCK                        perfdata            = NULL;
	LARGE_INTEGER	                          newperftime100nsec;
  XSTRING                                 processstr;
  XWINDOWSSYSTEM_CPUUSAGESTATUS*          cus                 = NULL;

  newperftime100nsec.HighPart = 0;
  newperftime100nsec.LowPart  = 0;

  objectindex     = XWINDOWSSYSTEM_PROCESSOR_OBJECT_INDEX;
  cpuusageindex   = XWINDOWSSYSTEM_PROCESSOR_TIME_COUNTER_INDEX;

  processstr     = __L("_Total");

  cus = AddCPUUsageStatus(processstr.Get());
  if(!cus) return 0;

  newvalue = perfcounters.GetCounterValue(&perfdata, objectindex, cpuusageindex, (LPCTSTR)processstr.Get());

	newperftime100nsec = perfdata->PerfTime100nSec;

	if(cus->firsttime)
	  {
		  cus->firsttime           = false;
		  cus->oldvalue            = newvalue;
		  cus->oldperftime100nsec  = newperftime100nsec;

		  return 0;
	  }

	LONGLONG  lnValueDelta         = newvalue - cus->oldvalue;
	double    DeltaPerfTime100nSec = (double)newperftime100nsec.QuadPart - (double)cus->oldperftime100nsec.QuadPart;

	cus->oldvalue            = newvalue;
	cus->oldperftime100nsec  = newperftime100nsec;

	double a = (double)lnValueDelta / DeltaPerfTime100nSec;

  double f = (1.0 - a) * 100.0;
  cpuusage = (int)(f + 0.5);	// rounding the result

  if(cpuusage < 0)  return 0;
  #endif

	return cpuusage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM::GetCPUUsageForProcessName(XCHAR* processname)
* @brief      Get CPU usage for process name
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processname : Processname pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM::GetCPUUsageForProcessName(XCHAR* processname)
{
  int	             cpuusage = 0;

  #ifndef BUILDER
  HANDLE           hprocesssnap;
  PROCESSENTRY32   pe32;  

  // Take a snapshot of all processes in the system.
  hprocesssnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if(hprocesssnap == INVALID_HANDLE_VALUE )  return false;

  // Set the size of the structure before using it.
  memset(&pe32, 0, sizeof(PROCESSENTRY32));
  pe32.dwSize = sizeof(PROCESSENTRY32);

  // Retrieve information about the first process, and exit if unsuccessful
  if(!Process32First(hprocesssnap, &pe32))
    {
      CloseHandle(hprocesssnap);  // clean the snapshot object
      return false;
    }

  // Now walk the snapshot of processes
  do{ XSTRING  nametask(pe32.szExeFile);

      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Process %s  : [%s]"), processname, nametask.Get());

      if(!nametask.Compare(processname, true))
        {
           cpuusage = GetCPUUsageForProcessID((XDWORD)pe32.th32ProcessID);
        }

    } while(Process32Next(hprocesssnap, &pe32));

  CloseHandle(hprocesssnap);

  #endif

  return cpuusage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM::GetCPUUsageForProcessID(XDWORD processID)
* @brief      Get CPU usage for process ID
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processID : Process ID value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM::GetCPUUsageForProcessID(XDWORD processID)
{ 
  int				                              cpuusage            = 0;

  #ifndef BUILDER
  XWINDOWSSYSTEM_PERFCOUNTERS<LONGLONG>   perfcounters;
	DWORD                                   objectindex;
	DWORD                                   cpuusageindex;
	LONGLONG		                            newvalue            = 0;
	PPERF_DATA_BLOCK                        perfdata            = NULL;
	LARGE_INTEGER	                          newperftime100nsec;
  XWINDOWSSYSTEM_CPUUSAGESTATUS*          cus                 = NULL;

  newperftime100nsec.HighPart = 0;
  newperftime100nsec.LowPart  = 0;

  cus = AddCPUUsageStatus(processID);
  if(!cus) return 0;

	objectindex     = XWINDOWSSYSTEM_PROCESS_OBJECT_INDEX;
	cpuusageindex   = XWINDOWSSYSTEM_PROCESSOR_TIME_COUNTER_INDEX;

	newvalue = perfcounters.GetCounterValueForProcessID(&perfdata, objectindex, cpuusageindex, (DWORD)processID);
	newperftime100nsec = perfdata->PerfTime100nSec;

	if(cus->firsttime)
	  {
		  cus->firsttime           = false;
		  cus->oldvalue            = newvalue;
		  cus->oldperftime100nsec  = newperftime100nsec;

		  return 0;
	  }

	LONGLONG  lnValueDelta         = newvalue - cus->oldvalue;
	double    DeltaPerfTime100nSec = (double)newperftime100nsec.QuadPart - (double)cus->oldperftime100nsec.QuadPart;

	cus->oldvalue            = newvalue;
	cus->oldperftime100nsec  = newperftime100nsec;

	double a = (double)lnValueDelta / DeltaPerfTime100nSec;

  cpuusage = (int) (a*100);

  if(cpuusage < 0)  return 0;

  #endif

  return cpuusage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XWINDOWSSYSTEM::GetEnviromentVariable(XCHAR* name)
* @brief      Get enviroment variable
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XWINDOWSSYSTEM::GetEnviromentVariable(XCHAR* name)
{
  return (XCHAR*)_wgetenv((const wchar_t *)name);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::SetEnviromentVariable(XCHAR* name, XCHAR* value)
* @brief      Set enviroment variable
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::SetEnviromentVariable(XCHAR* name, XCHAR* value)
{
  XSTRING                 all;
  XWINDOWSREGISTRYMANAGER registrymanager;
  XWINDOWSREGISTRYKEY     registrykeyenv;

  for(int c=0; c<2; c++)
    {
      bool status = false;

      switch(c)
        {
          case 0: status = registrymanager.OpenKey(HKEY_CURRENT_USER, __L("Environment"), registrykeyenv);
                  break;

          case 1: status = registrymanager.OpenKey(HKEY_LOCAL_MACHINE, __L("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), registrykeyenv);
                  break;
        }

      if(status)
        {
          XVARIANT valuevariant;

          registrykeyenv.ReadValue(name, valuevariant);
          valuevariant = value;
          registrykeyenv.WriteValue(name, valuevariant);

          registrymanager.CloseKey(registrykeyenv);
        }
    }

  all.Format(__L("%s=%s"), name, value);

  return _wputenv(all.Get())?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::DelEnviromentVariable(XCHAR* name)
* @brief      Del enviroment variable
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::DelEnviromentVariable(XCHAR* name)
{
  XSTRING                 all;
  XWINDOWSREGISTRYMANAGER registrymanager;
  XWINDOWSREGISTRYKEY     registrykeyenv;

  for(int c=0; c<2; c++)
    {
      bool status = false;

      switch(c)
        {
          case 0: status = registrymanager.OpenKey(HKEY_CURRENT_USER, __L("Environment"), registrykeyenv);
                  break;

          case 1: status = registrymanager.OpenKey(HKEY_LOCAL_MACHINE, __L("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), registrykeyenv);
                  break;
        }

      if(status)
        {
          registrykeyenv.DeleteValue(name);
          registrymanager.CloseKey(registrykeyenv);
        }
    }

  all.Format(__L("%s="), name);

  return _wputenv(all.Get())?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetUserAndDomain(XSTRING& user, XSTRING& domain)
* @brief      Get user and domain
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  user : User name.
* @param[in]  domain : Domain value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetUserAndDomain(XSTRING& user, XSTRING& domain)
{
  HANDLE        htoken   = NULL;
  PTOKEN_USER   ptiuser  = NULL;
  DWORD         cbti     = 0;
  SID_NAME_USE  snu;
  bool          status   = false;

  XSTRING       userextend;
    
  #ifndef BUILDER
  ULONG         size     = 1024;
   
  userextend.AdjustSize(size);  
  GetUserNameEx(NameUserPrincipal, userextend.Get(), &size);
  userextend.AdjustSize();  
  #endif
    
  if(!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &htoken)) 
    {
      if(GetLastError() == ERROR_NO_TOKEN)
        {
          if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &htoken))
            {
              if(!GetTokenInformation(htoken, TokenUser, NULL, 0, &cbti)) 
                {
                  ptiuser = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbti);
                  if(ptiuser) 
                    {
                      // Retrieve the user information from the token.
                      if(GetTokenInformation(htoken, TokenUser, ptiuser, cbti, &cbti))
                        {
                          TCHAR   _user[1024];
                          TCHAR   _domain[1024];
                          DWORD   sizeuser      = sizeof(user);
                          DWORD   sizedomain    = sizeof(domain);
    
                          // Retrieve user name and domain name based on user's SID.
                          if(LookupAccountSid(NULL, ptiuser->User.Sid, _user, &sizeuser, _domain, &sizedomain, &snu))
                            {  
                              user.Set(_user);
                              domain.Set(_domain);


                              if(userextend.GetSize()) user = userextend;

                              status = true;
                            }
                        } 
                      
                      if(ptiuser)  HeapFree(GetProcessHeap(), 0, ptiuser);                                     
                    }             
                }
            }
        }

      if(htoken) CloseHandle(htoken);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
* @brief      Shut down
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
{
  if(type == XSYSTEM_CHANGESTATUSTYPE_NONE) return false;

  HANDLE            hToken;
  TOKEN_PRIVILEGES  tkp;

  // Get a token for this process.
  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  return false;

  // Get the LUID for the shutdown privilege.
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

  tkp.PrivilegeCount            = 1;  // one privilege to set
  tkp.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;

  // Get the shutdown privilege for this process.
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
  if(GetLastError() != ERROR_SUCCESS) return false;

  switch(type)
    {
      case XSYSTEM_CHANGESTATUSTYPE_REBOOT           :  if(!ExitWindowsEx(EWX_REBOOT   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                                        break;

      case XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGOFF   :  if(!ExitWindowsEx(EWX_LOGOFF   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                                        break;

      case XSYSTEM_CHANGESTATUSTYPE_POWEROFF         :  if(!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                                        break;

      case XSYSTEM_CHANGESTATUSTYPE_SUSPEND          :    
                                                        #ifndef BUILDER
                                                        if(!SetSuspendState(TRUE, FALSE, FALSE)) return false; 
                                                        #endif    
                                                        break;

                                          default    :  return false;


    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM::Sound_GetLevel()
* @brief      Sound get level
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM::Sound_GetLevel()
{
  float                   currentvolume     = 0.0f;

  #ifndef BUILDER
  IMMDeviceEnumerator*    deviceenumerator  = NULL;
  IAudioEndpointVolume*   endpointvolume    = NULL;
  IMMDevice*              defaultdevice     = NULL;
  HRESULT                 hresult;
 
  //-------------------------------------------------------------------------------------------------------------------

  CoInitialize(NULL);

  hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceenumerator);
  if(!deviceenumerator) return 0;
  if(hresult != S_OK) return 0;

  //-------------------------------------------------------------------------------------------------------------------

  hresult = deviceenumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultdevice);
  if(hresult != S_OK) return 0;
  if(defaultdevice)
    {
      deviceenumerator->Release();
      deviceenumerator = NULL;

      //-------------------------------------------------------------------------------------------------------------------

      defaultdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointvolume);
      defaultdevice->Release();
      defaultdevice = NULL;
      endpointvolume->GetMasterVolumeLevel(&currentvolume);

      //-------------------------------------------------------------------------------------------------------------------

      endpointvolume->GetMasterVolumeLevelScalar(&currentvolume);
      if(hresult != S_OK) return 0;

      //-------------------------------------------------------------------------------------------------------------------

      endpointvolume->Release();
    }

  CoUninitialize();
  #endif

  return (int)(currentvolume*100);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::Sound_SetLevel(int level)
* @brief      Sound set level
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::Sound_SetLevel(int level)
{
  bool                    status            = false;

  #ifndef BUILDER
  float                   currentvolume     = 0.0f;
  IMMDeviceEnumerator*    deviceenumerator  = NULL;
  IAudioEndpointVolume*   endpointvolume    = NULL;
  IMMDevice*              defaultdevice     = NULL;
  HRESULT                 hresult;
 
  //-------------------------------------------------------------------------------------------------------------------

  CoInitialize(NULL);

  hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceenumerator);
  if(!deviceenumerator) return false;
  if(hresult != S_OK) return false;

  //-------------------------------------------------------------------------------------------------------------------

  hresult = deviceenumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultdevice);
  if(hresult != S_OK) return false;

  deviceenumerator->Release();
  deviceenumerator = NULL;

  if(defaultdevice)
    {
      //-------------------------------------------------------------------------------------------------------------------

      hresult = defaultdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointvolume);
      if(hresult != S_OK) return false;

      defaultdevice->Release();
      defaultdevice = NULL;
      endpointvolume->GetMasterVolumeLevel(&currentvolume);

      //-------------------------------------------------------------------------------------------------------------------

      endpointvolume->GetMasterVolumeLevelScalar(&currentvolume);

      endpointvolume->SetMute(false, NULL);
      endpointvolume->SetMasterVolumeLevelScalar((float)(level/100.0f), NULL);

      //-------------------------------------------------------------------------------------------------------------------

      endpointvolume->Release();

      status = true;
    }

  CoUninitialize();
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::Sound_SetMutex(bool on)
* @brief      Sound set mutex
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::Sound_SetMutex(bool on)
{
  bool                    status            = false;
  #ifndef BUILDER
  float                   currentvolume     = 0.0f;
  IMMDeviceEnumerator*    deviceenumerator  = NULL;
  IAudioEndpointVolume*   endpointvolume    = NULL;
  IMMDevice*              defaultdevice     = NULL;
  HRESULT                 hresult;
 
  CoInitialize(NULL);

  hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceenumerator);
  if(!deviceenumerator) return false;
  if(hresult != S_OK) return false;

  hresult = deviceenumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultdevice);
  if(hresult != S_OK) return false;
 
  deviceenumerator->Release();
  deviceenumerator = NULL; 

  if(defaultdevice)
    {
      hresult = defaultdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointvolume);
      status = (hresult == S_OK)?true:false;

      if(status)
        {
          defaultdevice->Release();
          defaultdevice = NULL;
          endpointvolume->GetMasterVolumeLevel(&currentvolume);

          endpointvolume->GetMasterVolumeLevelScalar(&currentvolume);
          endpointvolume->SetMute(on, NULL);

          endpointvolume->Release();
        }
    }

  CoUninitialize();
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetBatteryLevel(bool& isincharge, XBYTE& levelpercent)
* @brief      Get battery level
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  isincharge : Isincharge value.
* @param[in]  levelpercent : Levelpercent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetBatteryLevel(bool& isincharge, XBYTE& levelpercent)
{
  SYSTEM_POWER_STATUS powerstatus;

  isincharge    = false;
  levelpercent  = 0;

  if(!GetSystemPowerStatus(&powerstatus)) return false;

  if(powerstatus.BatteryLifePercent != 255) 
    {
      isincharge    = powerstatus.ACLineStatus?true:false;
      levelpercent  = powerstatus.BatteryLifePercent;

      return true;
    }

  return false;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSSYSTEM::GetWifiRSSILevel()
* @brief      get wifi RSSIlevel
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSSYSTEM::GetWifiRSSILevel()
{
  int                         index_status        = 0;

  #ifndef BUILDER
  HANDLE                      handleclient        = NULL;
  DWORD                       max_client          = 2;
  DWORD                       current_version     = 0;
  DWORD                       result              = 0;
  PWLAN_INTERFACE_INFO_LIST   interfacelist       = NULL;
  PWLAN_INTERFACE_INFO        interfacelinfo      = NULL;
  PWLAN_INTERFACE_CAPABILITY  interfacecapability = NULL;

  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;


  for(DWORD c=0; c<interfacelist->dwNumberOfItems; c++)
    {
      bool needactivated = false;

      interfacelinfo = (WLAN_INTERFACE_INFO*)&interfacelist->InterfaceInfo[c];            
      if(interfacelinfo->isState == wlan_interface_state_connected)
        {
          index_status = 4;

          PVOID*                  ptr_rssi = 0;
          DWORD                   sizerssi = sizeof(ptr_rssi);
          WLAN_OPCODE_VALUE_TYPE  wlanopcodevaluetype = wlan_opcode_value_type_invalid;

          result = WlanQueryInterface(handleclient, &interfacelinfo->InterfaceGuid, wlan_intf_opcode_rssi, NULL, &sizerssi, ptr_rssi, &wlanopcodevaluetype);

          if(result == ERROR_SUCCESS)
            {
              if(ptr_rssi)
                {               
                  int rssi = (int)(*(int*)(ptr_rssi));
                         
                  if(rssi < -67) index_status = 3; 
                  if(rssi < -70) index_status = 2; 
                  if(rssi < -80) index_status = 1;                     
                }
            }
        }
    }
  
  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  WlanCloseHandle(handleclient, NULL);
  #endif

  return index_status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::GetPathExecApplication(XCHAR* appname, XPATH& apppath)
* @brief      get path exec application
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  appname : Appname pointer to use.
* @param[in]  apppath : Apppath value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetPathExecApplication(XCHAR* appname, XPATH& apppath)
{
  apppath.Empty();

  XWINDOWSREGISTRYMANAGER registrymanager;
  XWINDOWSREGISTRYKEY     registrykey;
  XSTRING                 keyname;
  HKEY                    handlekey[2] = { HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE };
  XVARIANT                pathvalue;
  bool                    status    = false;
  
  for(XDWORD c=0; c<2; c++)
    {
      keyname.Format(__L("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\%s.exe"), appname);

      if(registrymanager.OpenKey(handlekey[c], keyname, registrykey))
        {
          if(registrykey.ReadValue(__L("Path"), pathvalue))
            {
              XSTRING apppath_tempo;

              apppath_tempo = (XSTRING&)pathvalue;

              apppath = apppath_tempo;
              apppath.Slash_Add();
              apppath.AddFormat(__L("%s.exe"), appname);  

              status = true; 

              break;                                  
            }

          registrymanager.CloseKey(registrykey);    
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::AddCPUUsageStatus(XCHAR* processname)
* @brief      Add CPU usage status
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processname : Processname pointer to use.
* 
* @return     XWINDOWSSYSTEM_CPUUSAGESTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::AddCPUUsageStatus(XCHAR* processname)
{
  XWINDOWSSYSTEM_CPUUSAGESTATUS* cus = GetCPUUsageStatus(processname);
  if(cus) return cus;
  
  cus = GEN_NEW XWINDOWSSYSTEM_CPUUSAGESTATUS();
  if(!cus) return NULL;

  cus->processname = processname;
  
  cpuusagestatus.Add(cus);
  
  return cus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::GetCPUUsageStatus(XCHAR* processname)
* @brief      Get CPU usage status
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processname : Processname pointer to use.
* 
* @return     XWINDOWSSYSTEM_CPUUSAGESTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::GetCPUUsageStatus(XCHAR* processname)
{
  if(cpuusagestatus.IsEmpty()) 
    {
      return NULL;
    }

  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->Lock();
    }

  for(XDWORD c=0; c<cpuusagestatus.GetSize(); c++)
    {
      XWINDOWSSYSTEM_CPUUSAGESTATUS* cus = cpuusagestatus.Get(c);
      if(cus)
        {
          if(!cus->processname.Compare(processname, true)) 
            {
              if(xmutexcheckCPUusage)
                {
                  xmutexcheckCPUusage->UnLock();
                }

              return cus;
            }
        }
    }

  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->UnLock();
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::AddCPUUsageStatus(XDWORD processID)
* @brief      Add CPU usage status
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processID : Process ID value.
* 
* @return     XWINDOWSSYSTEM_CPUUSAGESTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::AddCPUUsageStatus(XDWORD processID)
{
  XWINDOWSSYSTEM_CPUUSAGESTATUS* cus = GetCPUUsageStatus(processID);
  if(cus) return cus;
  
  cus = GEN_NEW XWINDOWSSYSTEM_CPUUSAGESTATUS();
  if(!cus) return NULL;

  cus->processID = processID;

  cpuusagestatus.Add(cus);
  
  return cus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::GetCPUUsageStatus(XDWORD processID)
* @brief      Get CPU usage status
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processID : Process ID value.
* 
* @return     XWINDOWSSYSTEM_CPUUSAGESTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM_CPUUSAGESTATUS* XWINDOWSSYSTEM::GetCPUUsageStatus(XDWORD processID)
{
  if(cpuusagestatus.IsEmpty()) 
    {
      return NULL;
    }

  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->Lock();
    }

  for(XDWORD c=0; c<cpuusagestatus.GetSize(); c++)
    {
      XWINDOWSSYSTEM_CPUUSAGESTATUS* cus = cpuusagestatus.Get(c);
      if(cus)
        {
          if(cus->processID == processID) 
            {
              if(xmutexcheckCPUusage)
                {
                  xmutexcheckCPUusage->UnLock();
                }

              return cus;
            }
        }    
    }

  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->UnLock();
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSSYSTEM::DeleteAllCPUUsageStatus()
* @brief      Delete all CPU usage status
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::DeleteAllCPUUsageStatus()
{
  if(cpuusagestatus.IsEmpty()) return false;

  cpuusagestatus.DeleteContents();
  cpuusagestatus.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSSYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSYSTEM::Clean()
{

}






