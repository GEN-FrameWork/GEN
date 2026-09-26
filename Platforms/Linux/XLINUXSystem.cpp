/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXSystem.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLINUXSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <locale.h>
#include <fcntl.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <termios.h>
#ifdef HW_INTEL
#include <cpuid.h>
#endif
#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/mount.h>
#include <syslog.h>
#include <sys/resource.h>
#include <linux/reboot.h>

#ifdef SND_ACTIVE
#ifndef ANDROID
#include <alsa/asoundlib.h>
#endif
#endif

#include "XLINUXFactory.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XFileTXT.h"
#include "XTrace.h"
#include "XString.h"
#include "XPath.h"
#include "XLanguage_ISO_639_3.h"
#include "XProcessManager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

char*   whom;	
char	  message[90];
int	    timeout    = 0;	



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

void    int_handler                     (int code);
void    swap_off                        ();
void    unmount_disks                   ();
void    unmount_disks_ourselves         ();


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXSYSTEM::XLINUXSYSTEM(): XSYSTEM()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXSYSTEM::XLINUXSYSTEM(): XSYSTEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXSYSTEM::~XLINUXSYSTEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXSYSTEM::~XLINUXSYSTEM()
{
   Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_HARDWARETYPE XLINUXSYSTEM::GetTypeHardware(int* revision)
* @brief      Get type hardware
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  revision : revision of hardware
* 
* @return     XSYSTEM_HARDWARETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XLINUXSYSTEM::GetTypeHardware(int* revision)
{
  #define CPUINFOREVISION __L("Revision")

  if(revision)
    {
      (*revision) = -1;

      XFILETXT* xfileTXT = GEN_NEW XFILETXT();
      if(xfileTXT)
        {
          XPATH xpath;

          xpath = __L("/proc/cpuinfo");

          if(xfileTXT->Open(xpath))
            {
              if(xfileTXT->ReadAllFile())
                {
                  for(int c=0; c<xfileTXT->GetNLines(); c++)
                    {
                      XSTRING* line = xfileTXT->GetLine(c);
                      if(line)
                        {
                          if(line->Find(CPUINFOREVISION, false) != XSTRING_NOTFOUND)
                            {
                              int position = line->Find(__L(":"), false);
                              if(position != XSTRING_NOTFOUND)
                                {
                                  XSTRING number;

                                  number = &line->Get()[position+1];

                                  number.UnFormat(__L(" %04x"), revision);
                                }
                            }
                        }
                    }
                }

              xfileTXT->Close();
            }

          GEN_DELETE xfileTXT;
        }
    }


  #ifdef HW_INTEL
  return XSYSTEM_HARDWARETYPE_PC;
  #endif

  #ifdef HW_LEX3V700A
  return XSYSTEM_HARDWARETYPE_LEX3V700A;
  #endif

  #ifdef HW_GUMSTIXCONNEX
  return XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX;
  #endif

  #ifdef HW_ARTILA500
  return XSYSTEM_HARDWARETYPE_ARTILA500;
  #endif

  #ifdef HW_RASPBERRYPI
  return XSYSTEM_HARDWARETYPE_RASPBERRYPI;
  #endif

  #ifdef HW_ODROIDC1
  return XSYSTEM_HARDWARETYPE_ODROIDC1;
  #endif

  #ifdef HW_BEAGLEBONE
  return XSYSTEM_HARDWARETYPE_BEAGLEBONE;
  #endif
  
  #ifdef HW_ARM
  return XSYSTEM_HARDWARETYPE_NXP_IMX6;
  #endif

  #ifdef HW_ARM64
  return XSYSTEM_HARDWARETYPE_NXP_IMX8;
  #endif

  return XSYSTEM_HARDWARETYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_PLATFORM XLINUXSYSTEM::GetPlatform(XSTRING* namestring)
* @brief      Get platform
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  namestring : Namestring pointer to use.
* 
* @return     XSYSTEM_PLATFORM : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_PLATFORM XLINUXSYSTEM::GetPlatform(XSTRING* namestring)
{
 
  #ifdef HW_INTEL
  if(namestring)  namestring->Set(__L("Linux"));
  return XSYSTEM_PLATFORM_LINUX;
  #endif

  #ifdef HW_LEX3V700A
  if(namestring)  namestring->Set(__L("Linux"));
  return XSYSTEM_PLATFORM_LINUX;
  #endif

  #ifdef HW_GUMSTIXCONNEX
  if(namestring)  namestring->Set(__L("Linux Embedded"));
  return XSYSTEM_PLATFORM_LINUX_EMBEDDED;
  #endif

  #ifdef HW_ARTILA500
  if(namestring)  namestring->Set(__L("Linux Embedded"));
  return XSYSTEM_PLATFORM_LINUX_EMBEDDED;
  #endif

  #ifdef HW_RASPBERRYPI
  if(namestring)  namestring->Set(__L("Linux Embedded"));
  return XSYSTEM_PLATFORM_LINUX_EMBEDDED;
  #endif

  #ifdef HW_BEAGLEBONE
  if(namestring)  namestring->Set(__L("Linux Embedded"));
  return XSYSTEM_PLATFORM_LINUX_EMBEDDED;
  #endif

  #if defined(HW_ARM) || defined(HW_ARM64)
  if(namestring)  namestring->Set(__L("Linux Embedded"));
  return XSYSTEM_PLATFORM_LINUX_EMBEDDED;  
  #endif

  if(namestring)  namestring->Set(__L("Unknown"));  
  return XSYSTEM_PLATFORM_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::GetOperativeSystemID(XSTRING& ID)
* @brief      Get operative system ID
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::GetOperativeSystemID(XSTRING& ID)
{
  ID.Empty();

  struct utsname details;
    
  int ret = uname(&details);    
  if(ret != 0) return false;

  ID += details.sysname;      ID += __L(" ");
  // ID += details.nodename;     ID += __L(" ");
  ID += details.release;      ID += __L(" ");
  ID += details.version;      ID += __L(" ");
  ID += details.machine;      ID += __L(" ");
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLINUXSYSTEM::GetLanguageSO()
* @brief      Get language SO
* @ingroup    PLATFORM_LINUX
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLINUXSYSTEM::GetLanguageSO()
{
  XLANGUAGE_ISO_639_3 ISO639;
  XSTRING             languageenglish;

  languageenglish.Set(setlocale(LC_MESSAGES, ""));

  int undercodeindex = languageenglish.Find(__L("_"), false);
  if(undercodeindex == XSTRING_NOTFOUND) return XLANGUAGE_ISO_639_3_CODE_INVALID;

  int todelete = languageenglish.GetSize();
  todelete -= undercodeindex;

  languageenglish.DeleteCharacters(undercodeindex, todelete);

  XDWORD code = ISO639.Code_GetByCodeAlpha2(languageenglish.Get());
  if(code != XLANGUAGE_ISO_639_3_CODE_INVALID) return code;

  return ISO639.Code_GetByCodeAlpha3(languageenglish.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLINUXSYSTEM::GetBIOSSerialNumber()
* @brief      Get BIOS serial number
* @ingroup    PLATFORM_LINUX
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLINUXSYSTEM::GetBIOSSerialNumber()
{
  return &BIOSserialnumber;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLINUXSYSTEM::GetCPUSerialNumber()
* @brief      Get CPU serial number
* @ingroup    PLATFORM_LINUX
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLINUXSYSTEM::GetCPUSerialNumber()
{
  #ifdef HW_INTEL
  unsigned int level = 1;
  unsigned eax = 3 /* processor serial number */, ebx = 0, ecx = 0, edx = 0;

  __get_cpuid(level, &eax, &ebx, &ecx, &edx);

  XDWORD first = eax;
  XDWORD last  = edx;

  CPUserialnumber.Format(__L("%08X%08X"), last, first);
  #endif

  return &CPUserialnumber;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM_ReadThermalZoneTemperature(float& outcelsius)
* @brief      First CPU temperature source: the kernel's generic thermal sysfs interface
*             (/sys/class/thermal/thermal_zoneN/{type,temp}), present on any kernel built with CONFIG_THERMAL --
*             desktop/server x86, Raspberry Pi, and most ARM SoCs.
* @note       INTERNAL. Zones are numbered contiguously from 0 by the kernel, so the scan stops at the first
*             missing index. Looks for a "type" that names the CPU/SoC package itself (x86_pkg_temp/acpitz on
*             PC-class hardware, cpu-thermal/cpu_thermal/soc_thermal/soc-thermal on the ARM SoCs used by
*             Raspberry Pi and most Android devices); the first zone found while scanning is kept as a fallback
*             in case no zone matches one of those names, since some boards only expose a single,
*             unlabelled-but-still-valid zone.
* @ingroup    PLATFORM_LINUX
*
* @param[out] outcelsius : Temperature in Celsius. Untouched when this source has nothing to offer.
*
* @return     bool : true if a zone was read and parsed; false if the sysfs tree is absent, unreadable, or a
*             zone's "temp" file does not parse as an integer.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool XLINUXSYSTEM_ReadThermalZoneTemperature(float& outcelsius)
{
  static const char* preferredzonetypes[] = { "x86_pkg_temp", "cpu-thermal", "cpu_thermal", "soc_thermal", "soc-thermal", "acpitz", NULL };

  int foundzone    = -1;
  int fallbackzone = -1;

  for(int zone=0; zone<32; zone++)
    {
      char typepath[64];
      snprintf(typepath, sizeof(typepath), "/sys/class/thermal/thermal_zone%d/type", zone);

      FILE* typefile = fopen(typepath, "r");
      if(!typefile) break;   // thermal_zoneN entries are numbered contiguously from 0, so the first miss ends the scan

      char typevalue[64] = { 0 };
      if(fgets(typevalue, sizeof(typevalue), typefile))
        {
          char* newline = strchr(typevalue, '\n');
          if(newline) *newline = 0;

          if(fallbackzone < 0) fallbackzone = zone;

          for(int t=0; preferredzonetypes[t] && (foundzone < 0); t++)
            {
              if(!strcmp(typevalue, preferredzonetypes[t])) foundzone = zone;
            }
        }

      fclose(typefile);

      if(foundzone >= 0) break;
    }

  int usezone = (foundzone >= 0) ? foundzone : fallbackzone;
  if(usezone < 0) return false;

  char temppath[64];
  snprintf(temppath, sizeof(temppath), "/sys/class/thermal/thermal_zone%d/temp", usezone);

  FILE* tempfile = fopen(temppath, "r");
  if(!tempfile) return false;

  int  millidegrees = 0;
  bool valid        = (fscanf(tempfile, "%d", &millidegrees) == 1);

  fclose(tempfile);

  if(!valid) return false;

  outcelsius = (float)millidegrees / 1000.0f;   // thermal_zoneN/temp reports millidegrees Celsius

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM_ReadHWMonTemperature(float& outcelsius)
* @brief      Second CPU temperature source: /sys/class/hwmon/hwmonN, the interface the "coretemp" (Intel) and
*             "k10temp" (AMD) kernel drivers publish through, and what user-space tools like "sensors"
*             (lm-sensors) read from. Several real desktop/laptop installs report the CPU package temperature
*             ONLY here, with no matching entry under /sys/class/thermal at all, so this is tried as a fallback
*             whenever the thermal-zone source above found nothing.
* @note       INTERNAL. hwmonN indices are not guaranteed contiguous nor stable across reboots, so every index
*             is checked (unlike the thermal-zone scan above, this one does not stop at the first miss) up to a
*             generous bound. "temp1_input" is the package/die reading on both coretemp and k10temp in practice.
* @ingroup    PLATFORM_LINUX
*
* @param[out] outcelsius : Temperature in Celsius. Untouched when this source has nothing to offer.
*
* @return     bool : true if a matching hwmon device was read and parsed; false otherwise.
*
* --------------------------------------------------------------------------------------------------------------------*/
static bool XLINUXSYSTEM_ReadHWMonTemperature(float& outcelsius)
{
  static const char* preferreddrivernames[] = { "coretemp", "k10temp", "cpu_thermal", NULL };

  for(int hw=0; hw<32; hw++)
    {
      char namepath[64];
      snprintf(namepath, sizeof(namepath), "/sys/class/hwmon/hwmon%d/name", hw);

      FILE* namefile = fopen(namepath, "r");
      if(!namefile) continue;   // unlike thermal_zoneN, hwmonN numbering has gaps, so a miss does not end the scan

      char namevalue[64] = { 0 };
      bool matched       = false;

      if(fgets(namevalue, sizeof(namevalue), namefile))
        {
          char* newline = strchr(namevalue, '\n');
          if(newline) *newline = 0;

          for(int d=0; preferreddrivernames[d] && !matched; d++)
            {
              if(!strcmp(namevalue, preferreddrivernames[d])) matched = true;
            }
        }

      fclose(namefile);

      if(!matched) continue;

      char temppath[64];
      snprintf(temppath, sizeof(temppath), "/sys/class/hwmon/hwmon%d/temp1_input", hw);

      FILE* tempfile = fopen(temppath, "r");
      if(!tempfile) continue;

      int  millidegrees = 0;
      bool valid        = (fscanf(tempfile, "%d", &millidegrees) == 1);

      fclose(tempfile);

      if(!valid) continue;

      outcelsius = (float)millidegrees / 1000.0f;   // hwmon tempN_input also reports millidegrees Celsius

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float XLINUXSYSTEM::GetCPUTemperature()
* @brief      Get CPU temperature
* @note       Tries two independent kernel sysfs sources in order, since which one a given board/distro actually
*             populates varies: (1) the generic thermal-zone interface (XLINUXSYSTEM_ReadThermalZoneTemperature,
*             covers desktop/server x86, Raspberry Pi, and Android -- its kernel is Linux, and XANDROIDSYSTEM
*             inherits this implementation as-is instead of overriding it, so this one function covers both
*             platforms), then (2) the hwmon interface used by coretemp/k10temp
*             (XLINUXSYSTEM_ReadHWMonTemperature), for the real desktop/laptop installs whose CPU temperature
*             only ever shows up there. Returns 0.0f -- exactly the old stub's value, so
*             UI_SYSTEM::HardwareInfo_UpdateCPU() keeps showing "--" instead of a fake number -- when NEITHER
*             source has anything to offer.
* @note       A virtual machine (VMware, WSL2, VirtualBox, Hyper-V, ...) is expected to fail both sources: no
*             mainstream hypervisor passes real ACPI thermal tables or CPU thermal MSRs through to the guest,
*             precisely because "the CPU's temperature" is a property of the physical host, not of a vCPU, and
*             exposing it would leak host information across guests. This was confirmed against a WSL2 guest
*             (2026-09) and a VMware Workstation Debian guest (2026-09): both exposed cooling_device* nodes but
*             zero thermal_zone* nodes under /sys/class/thermal, and no coretemp/k10temp hwmon device either --
*             there is nothing here for ANY software, GEN included, to read. This is a property of running
*             inside a VM, not a defect in this function; validating a real reading requires bare-metal Linux,
*             a Raspberry Pi, or a real Android device.
* @ingroup    PLATFORM_LINUX
*
* @return     float : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
float XLINUXSYSTEM::GetCPUTemperature()
{
  float celsius = 0.0f;

  if(XLINUXSYSTEM_ReadThermalZoneTemperature(celsius)) return celsius;
  if(XLINUXSYSTEM_ReadHWMonTemperature(celsius))        return celsius;

  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
* @brief      Get memory info
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  total : total memory
* @param[in]  free : free memory
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
{
  struct sysinfo meminfo;

  if(sysinfo(&meminfo) == -1) return false;

  long long totalvirtualmem = meminfo.totalram;
  //Add other values in next statement to avoid int overflow on right hand side...
  //totalvirtualmem += meminfo.totalswap;
  totalvirtualmem *= meminfo.mem_unit;

  long long virtualmemused = meminfo.totalram - meminfo.freeram;
  //Add other values in next statement to avoid int overflow on right hand side...
  //virtualmemused += meminfo.totalswap - meminfo.freeswap;
  virtualmemused *= meminfo.mem_unit;

  total = (XDWORD)(totalvirtualmem                     / 1024);
  free  = (XDWORD)((totalvirtualmem - virtualmemused)  / 1024);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::GetVolumesInfo(XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes)
* @brief      Enumerate mounted block volumes with total/free space and file system type.
* @ingroup    PLATFORM_LINUX
* 
* @param[out] volumes : Cleared and filled with GEN_NEW entries (caller DeleteContents).
* 
* @return     bool : true if at least one volume was added; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::GetVolumesInfo(XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes)
{
  volumes.DeleteContents();

  FILE* mounts = setmntent(_PATH_MOUNTED, "r");
  if(!mounts) mounts = setmntent("/proc/mounts", "r");
  if(!mounts) return false;

  struct mntent* entry = NULL;

  while((entry = getmntent(mounts)) != NULL)
    {
      if(!entry->mnt_fsname || !entry->mnt_dir || !entry->mnt_type) continue;

      // Only real block devices (skip proc, sysfs, cgroup, ...).
      if(strncmp(entry->mnt_fsname, "/dev/", 5) != 0) continue;

      struct statvfs vfs;
      memset(&vfs, 0, sizeof(vfs));

      if(statvfs(entry->mnt_dir, &vfs) != 0) continue;
      if(!vfs.f_blocks || !vfs.f_frsize) continue;

      XQWORD totalbytes = (XQWORD)vfs.f_blocks * (XQWORD)vfs.f_frsize;
      XQWORD freebytes  = (XQWORD)vfs.f_bavail * (XQWORD)vfs.f_frsize;

      XSYSTEM_VOLUMEINFO* volume = GEN_NEW XSYSTEM_VOLUMEINFO();
      if(!volume) continue;

      volume->GetName()->Set(entry->mnt_dir);
      volume->GetFileSystem()->Set(entry->mnt_type);
      volume->SetTotalBytes(totalbytes);
      volume->SetFreeBytes(freebytes);

      XSYSTEM_VOLUME_TYPE volumetype = XSYSTEM_VOLUME_TYPE_FIXED;
      if((strstr(entry->mnt_dir, "/media/") != NULL) ||
         (strstr(entry->mnt_dir, "/run/media/") != NULL) ||
         (strstr(entry->mnt_dir, "/mnt/") != NULL))
        {
          volumetype = XSYSTEM_VOLUME_TYPE_REMOVABLE;
        }

      volume->SetType(volumetype);

      // Best-effort volume label from /dev/disk/by-label (symlink basename matches device).
      DIR* labeldir = opendir("/dev/disk/by-label");
      if(labeldir)
        {
          const char* fsbase = strrchr(entry->mnt_fsname, '/');
          fsbase = fsbase ? (fsbase + 1) : entry->mnt_fsname;

          struct dirent* de = NULL;
          while((de = readdir(labeldir)) != NULL)
            {
              if(de->d_name[0] == '.') continue;

              char linkpath[512];
              snprintf(linkpath, sizeof(linkpath), "/dev/disk/by-label/%s", de->d_name);

              char target[512];
              ssize_t n = readlink(linkpath, target, sizeof(target) - 1);
              if(n <= 0) continue;
              target[n] = 0;

              const char* resbase = strrchr(target, '/');
              resbase = resbase ? (resbase + 1) : target;

              if(fsbase && resbase && !strcmp(fsbase, resbase))
                {
                  volume->GetLabel()->Set(de->d_name);
                  break;
                }
            }

          closedir(labeldir);
        }

      volumes.Add(volume);
    }

  endmntent(mounts);

  return (!volumes.IsEmpty());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::FreeCacheMemory()
* @brief      Free cache memory
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::FreeCacheMemory()
{
  #ifdef XPROCESSMANAGER_ACTIVE
  return GEN_XPROCESSMANAGER.MakeSystemCommand(__L("sync && sysctl -w vm.drop_caches=3 > /dev/null"));  
  #else 
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLINUXSYSTEM::GetCPUUsageTotal()
* @brief      Get CPU usage total
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLINUXSYSTEM::GetCPUUsageTotal()
{
  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->Lock();
    }

  static unsigned int lasttotaluser, lasttotaluserlow, lasttotalsys, lasttotalidle;
  float               percent;
  int                 intpercent;     
  FILE*               file;
  unsigned long long  totaluser, totaluserlow, totalsys, totalidle, total;
     
  file = fopen("/proc/stat", "r");
  if(!file)
    {
      if(xmutexcheckCPUusage)
        {
          xmutexcheckCPUusage->UnLock();
        }

      return XSYSTEM_CPUUSAGE_ERROR;
    }

  if(fscanf(file, "cpu %llu %llu %llu %llu", &totaluser, &totaluserlow, &totalsys, &totalidle) != 4)
    {
      fclose(file);

      if(xmutexcheckCPUusage)
        {
          xmutexcheckCPUusage->UnLock();
        }

      return XSYSTEM_CPUUSAGE_ERROR;
    }

  fclose(file);
     
  if(totaluser < lasttotaluser || totaluserlow < lasttotaluserlow || totalsys < lasttotalsys || totalidle < lasttotalidle)
    {
      // Overflow detection. Just skip this value.
      percent = -1.0;
    }
   else
    {
      total    = (totaluser - lasttotaluser) + (totaluserlow - lasttotaluserlow) + (totalsys - lasttotalsys);
      percent  = total * 100;
      total   += (totalidle - lasttotalidle);
      percent /= total;
    }
     
  lasttotaluser    = totaluser;
  lasttotaluserlow = totaluserlow;
  lasttotalsys     = totalsys;
  lasttotalidle    = totalidle;
     
  intpercent = percent;

  if(xmutexcheckCPUusage)
    {
      xmutexcheckCPUusage->UnLock();
    }
    		
  return intpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLINUXSYSTEM::GetCPUUsageForProcessName(XCHAR* processname)
* @brief      Get CPU usage for process name
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  processname : Processname pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLINUXSYSTEM::GetCPUUsageForProcessName(XCHAR* processname)
{  
  XSTRING command;
  XSTRING result;
  XBUFFER charcmd;
  
  command.Format(__L("ps -C %s -o %%cpu"), processname);

  command.ConvertToASCII(charcmd);  

  // Open a pipe to the command
  FILE* pipe = popen((char*)charcmd.Get(), "r");
  if(!pipe) 
    {
       return 0; // Error
    }
   
  char buffer[128];    
  while(!feof(pipe)) 
    {
      if(fgets(buffer, 128, pipe) != NULL)
        {
          result += buffer;
        }
    }
  
  pclose(pipe);

  // Extract the CPU usage percentage from the result
  double cpupercentage = 0.0f;
  result.UnFormat(__L("%%CPU\n %lf\n"), &cpupercentage);

  return static_cast<int>(cpupercentage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLINUXSYSTEM::GetCPUUsageForProcessID(XDWORD processID)
* @brief      Get CPU usage for process ID
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  processID : Process ID value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLINUXSYSTEM::GetCPUUsageForProcessID(XDWORD processID)
{
  XSTRING command;
  XSTRING result;
  XBUFFER charcmd;
  
  command.Format(__L("ps -C -p %d -o %%cpu"), processID);

  command.ConvertToASCII(charcmd);  

  // Open a pipe to the command
  FILE* pipe = popen((char*)charcmd.Get(), "r");
  if(!pipe) 
    {
       return 0; // Error
    }
   
  char buffer[128];    
  while(!feof(pipe)) 
    {
      if(fgets(buffer, 128, pipe) != NULL)
        {
          result += buffer;
        }
    }
  
  pclose(pipe);

  // Extract the CPU usage percentage from the result
  double cpupercentage = 0.0f;
  result.UnFormat(__L("%%CPU\n %lf\n"), &cpupercentage);

  return static_cast<int>(cpupercentage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XLINUXSYSTEM::GetEnviromentVariable(XCHAR* name)
* @brief      Get enviroment variable
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  name : Name to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XLINUXSYSTEM::GetEnviromentVariable(XCHAR* name)
{
  XSTRING        _variablename = name;
  static XSTRING result;

  XBUFFER charstr;
  
  _variablename.ConvertToASCII(charstr); 
  result = getenv(charstr.GetPtrChar());
  
  return result.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::SetEnviromentVariable(XCHAR* name, XCHAR* value)
* @brief      Set enviroment variable
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::SetEnviromentVariable(XCHAR* name, XCHAR* value)
{
  XSTRING _name;
  XSTRING _value;

  _name   = name;
  _value  = value;

  XBUFFER charstr1;  
  XBUFFER charstr2;
  
  _name.ConvertToASCII(charstr1);  
  _value.ConvertToASCII(charstr2);

  int status = setenv(charstr1.GetPtrChar(), charstr2.GetPtrChar(), 1);
  
  return status?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::DelEnviromentVariable(XCHAR* name)
* @brief      Del enviroment variable
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::DelEnviromentVariable(XCHAR* name)
{
  XSTRING _name;

  _name = name;

  XBUFFER charstr;
  
  _name.ConvertToASCII(charstr);

  int status = unsetenv(charstr.GetPtrChar());
  
  return status?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
* @brief      Shut down
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  type : type of shutdown of SO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
{
  int	    opt_reboot = 0;
  int     fd;

  switch(type)
    {
      case XSYSTEM_CHANGESTATUSTYPE_REBOOT          : opt_reboot = 1;   break;     
      case XSYSTEM_CHANGESTATUSTYPE_POWEROFF        : opt_reboot = 0;   break;
                                        default     : break;
    }

  setpriority(PRIO_PROCESS, 0, PRIO_MIN);
	                                                    
  whom = getlogin();

  setpriority(PRIO_PROCESS, 0, PRIO_MIN);
	signal(SIGINT, int_handler);
	signal(SIGHUP, int_handler);
	signal(SIGQUIT, int_handler);
	signal(SIGTERM, int_handler);

	chdir("/");

	signal(SIGPIPE, SIG_IGN);	
	signal(SIGINT, SIG_IGN);   // now there's no turning back... 

	kill(1, SIGTSTP);	          // tell init not to spawn more getty's 
		
	sync();

	signal(SIGTERM, SIG_IGN);

	if(fork() > 0) sleep(1000); // the parent will die soon... 

	setpgrp();		              // so the shell wont kill us in the fall 
	
  kill(-1, SIGTERM);          // a gentle kill of all other processes except init 
	sleep(2);
	
	kill(-1, SIGKILL);          // now use brute force... 

	acct(NULL);	                // turn off accounting 

	sync();
	sleep(2);

	swap_off();	                // remove swap files and partitions using swapoff 

	unmount_disks();	          // unmount disks... 

	sync();
	sleep(1);

  if(opt_reboot) 
    {
		  reboot(LINUX_REBOOT_CMD_RESTART);
	  } 
   else 
    {
		  reboot(LINUX_REBOOT_CMD_POWER_OFF);
    }		  

  exit(0); // to quiet gcc 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLINUXSYSTEM::Sound_GetLevel()
* @brief      Sound get level
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLINUXSYSTEM::Sound_GetLevel()
{
  long _level = 0;

  bool status =  Sound_SetLevel(true, &_level);
  if(!status) return 0;

  return (int)_level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::Sound_SetLevel(int level)
* @brief      Sound set level
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::Sound_SetLevel(int level)
{
  long _level = (long)level;

  return Sound_SetLevel(false, &_level);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::Sound_SetMutex(bool on)
* @brief      Sound set mutex
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::Sound_SetMutex(bool on)
{
  static long old_level = Sound_GetLevel();

  if(on)
    {
      old_level = Sound_GetLevel();
      Sound_SetLevel(0);      
    }
   else
    {
      Sound_SetLevel(old_level);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXSYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXSYSTEM::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void int_handler(int code)
* @brief      nt_handler
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  code : Code value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void int_handler(int code)
{
  #ifdef _PATH_NOLOGIN
	unlink(_PATH_NOLOGIN);
  #endif

	signal(SIGINT, SIG_DFL);

	exit(1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void swap_off()
* @brief      wap_off
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void swap_off()
{
	/// swapoff esp. swap FILES so the underlying partition can be
	//  unmounted. It you don't have swapoff(1) or use mount to
	//  add swapspace, this may not be necessary, but I guess it
	//  won't hurt 

	int   pid;
	int   result;
	int   status;

	sync();

	if((pid = fork()) < 0) 
    {
		  // Cannot fork for swapoff. Shrug!
		  return;
	  }

	if(!pid) 
    {
		  execl("/sbin/swapoff" , SWAPOFF_ARGS, NULL);
		  execl("/etc/swapoff", SWAPOFF_ARGS, NULL);
		  execl("/bin/swapoff", SWAPOFF_ARGS, NULL);

		  execlp("swapoff"      , SWAPOFF_ARGS, NULL);

		  // Cannot exec swapoff, hoping umount will do the trick.
		  exit(0);
	  }

	while((result = wait(&status)) != -1 && result != pid);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void unmount_disks()
* @brief      nmount_disks
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void unmount_disks()
{
	// better to use umount directly because it may be smarter than us 

	int   pid;
	int   result;
	int   status;

	sync();

	if((pid = fork()) < 0) 
    {
		  // Cannot fork for umount, trying manually.
		  unmount_disks_ourselves();
		  return;
	  }

	if(!pid) 
    {
		  execl(_PATH_UMOUNT, UMOUNT_ARGS, NULL);

  		// Cannot exec %s, trying umount. _PATH_UMOUNT

  		execlp("umount", UMOUNT_ARGS, NULL);

	  	// Cannot exec umount, giving up on umount.
		  exit(0);
	  }

	while ((result = wait(&status)) != -1 && result != pid);

	// Unmounting any remaining filesystems...

	unmount_disks_ourselves();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void unmount_disks_ourselves()
* @brief      nmount_disks_ourselves
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void unmount_disks_ourselves()
{
	// unmount all disks 

	FILE*           mtab;
	struct mntent*  mnt;
	char*           mntlist[128];
	int             i;
	int             n;
	char*           filesys;
	
	sync();

	if(!(mtab = setmntent(_PATH_MTAB, "r"))) 
    {
		  // shutdown: Cannot open %s _PATH_MTAB;
		  return;
	  }

	n = 0;
	while (n < 100 && (mnt = getmntent(mtab))) 
    {
		  mntlist[n++] = strdup(mnt->mnt_fsname[0] == '/' ? mnt->mnt_fsname : mnt->mnt_dir);
	  }

	endmntent(mtab);

	// we are careful to do this in reverse order of the mtab file 

	for(i = n - 1; i >= 0; i--) 
    {
		  filesys = mntlist[i];

		  if(umount(mntlist[i]) < 0)
        {
			    // shutdown: Couldn't umount %s filesys
        }
	  }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXSYSTEM::Sound_SetLevel(bool read, long* level)
* @brief      Sound set level
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  read : Read value.
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::Sound_SetLevel(bool read, long* level)
{ 
  #ifdef SND_ACTIVE 
  #ifndef ANDROID

  snd_mixer_t*          handle;
  snd_mixer_elem_t*     elem;
  snd_mixer_selem_id_t* sid;

  static int            mix_index = 0;
  static const char*    mix_name1 = "PCM";       
  static const char*    mix_name2 = "Master";    // first "PCM" if error after "Master"
  static const char*    card      = "default";  
  long                  pmin, pmax;
  long                  get_vol, set_vol;
  float                 f_multi;

  if((snd_mixer_open(&handle, 0)) < 0) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_open: error"));
      return false;        
    }

  if((snd_mixer_attach(handle, card)) < 0) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_attach: error"));
      snd_mixer_close(handle);
      return false;
    }

  if((snd_mixer_selem_register(handle, NULL, NULL)) < 0) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_register: error"));
      snd_mixer_close(handle);
      return false;
    }

  int ret = snd_mixer_load(handle);
  if(ret < 0) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_load: error"));
      snd_mixer_close(handle);
      return false;
    }

  snd_mixer_selem_id_alloca(&sid);

  //sets simple-mixer index and name
  snd_mixer_selem_id_set_index(sid, mix_index);
  snd_mixer_selem_id_set_name(sid   , mix_name1);

  elem = snd_mixer_find_selem(handle, sid);
  if(!elem) 
    {
      snd_mixer_selem_id_set_index(sid  , mix_index);
      snd_mixer_selem_id_set_name(sid   , mix_name2);
      
      elem = snd_mixer_find_selem(handle, sid);
      if(!elem) 
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_find_selem: error"));

          snd_mixer_close(handle);
          return false;   
        }
    }

  long minv, maxv;

  snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);
  
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Alsa volume] Volume range <%i,%i>"), minv, maxv);

  if(read) 
    {
      if(snd_mixer_selem_get_playback_volume(elem, (snd_mixer_selem_channel_id_t)0, level) < 0) 
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_get_playback_volume: error"));  
          snd_mixer_close(handle);
          return false;
        }

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Alsa volume] Get volume %i with status %i"), *level, ret);

      // make the value bound to 100 
      *level  -= minv;
      maxv    -= minv;
      minv     = 0;
     
      *level = 100 * (*level) / maxv; // make the value bound from 0 to 100
    }    
   else   
    {
      if(*level < 0 || *level > 100) // out of bounds
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] out of bounds: error"));
          return false;
        }

      //minv = 0;
      //maxv = 100;

     
      *level = (*level * (maxv - minv) / (100-1)) + minv;
      
      if(snd_mixer_selem_set_playback_volume_range(elem, minv, maxv) < 0)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_set_playback_volume_range: error"));

          snd_mixer_close(handle);
          return false;
        }
         
      if(snd_mixer_selem_set_playback_volume_all(elem, *level) < 0) 
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_set_playback_volume_all: error"));

          snd_mixer_close(handle);
          return false;
        }
   
      /*
      if(snd_mixer_selem_set_playback_volume(elem, (snd_mixer_selem_channel_id_t)0, *level) < 0) 
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_set_playback_volume: error"));

          snd_mixer_close(handle);
          return false;
        }

      if(snd_mixer_selem_set_playback_volume(elem, (snd_mixer_selem_channel_id_t)1, *level) < 0) 
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Alsa volume] snd_mixer_selem_set_playback_volume: error"));

          snd_mixer_close(handle);
          return false;
        }
       */

      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Alsa volume] Set volume %i with status %i"), *level, ret);        
    }

  snd_mixer_close(handle);

  #endif

  return true;



  #else

  return false;

  #endif 
}



