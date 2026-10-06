/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSSystem.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include <comdef.h>			// for using bstr_t class
#include <vector>


#include <iostream>
#include <psapi.h>

#include "XSystem.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XWINDOWSSYSTEM_MAXNETADDRIP															16

#define XWINDOWSSYSTEM_SYSTEM_OBJECT_INDEX											2					// 'System' object
#define XWINDOWSSYSTEM_PROCESS_OBJECT_INDEX											230				// 'Process' object
#define XWINDOWSSYSTEM_PROCESSOR_OBJECT_INDEX										238				// 'Processor' object
#define XWINDOWSSYSTEM_TOTAL_PROCESSOR_TIME_COUNTER_INDEX				240				// '% Total processor time' counter (valid in WinNT under 'System' object)
#define XWINDOWSSYSTEM_PROCESSOR_TIME_COUNTER_INDEX							6					// '% processor time' counter (for Win2K/XP)

#define XWINDOWSSYSTEM_TOTALBYTES																100*1024
#define XWINDOWSSYSTEM_BYTEINCREMENT														10*1024

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

#ifndef BUILDER

#pragma pack(push,8)
template <class T>
class XWINDOWSSYSTEM_PERFCOUNTERS
{
	public:
																								XWINDOWSSYSTEM_PERFCOUNTERS												()
																								{
																
																								}

																							 ~XWINDOWSSYSTEM_PERFCOUNTERS												()
																								{

																								}

		T																						GetCounterValue																		(PERF_DATA_BLOCK** perfdata, DWORD objectindex, DWORD counterindex, LPCTSTR instancename = NULL)
																								{
																									QueryPerformanceData(perfdata, objectindex, counterindex);

																									PPERF_OBJECT_TYPE perfobj = NULL;
																									T									value		= { 0 };

																									perfobj = FirstObject(*perfdata);

																									for(DWORD i=0; i<(*perfdata)->NumObjectTypes; i++ )
																										{

																											if(perfobj->ObjectNameTitleIndex == objectindex)
																												{
																													value = GetCounterValue(perfobj, counterindex, instancename);
																													break;
																												}

																											perfobj = NextObject( perfobj );
																										}

																									return value;
																								}

		T																						GetCounterValueForProcessID											(PERF_DATA_BLOCK** perfdata, DWORD objectindex, DWORD counterindex, DWORD processID)
																								{
																									QueryPerformanceData(perfdata, objectindex, counterindex);

																									PPERF_OBJECT_TYPE perfobj		= NULL;
																									T									value			= { 0 };

																									perfobj = FirstObject(*perfdata);

																									for(DWORD i=0; i<(*perfdata)->NumObjectTypes; i++ )
																										{

																											if(perfobj->ObjectNameTitleIndex == objectindex)
																												{
																													value = GetCounterValueForProcessID(perfobj, counterindex, processID);
																													break;
																												}

																											perfobj = NextObject( perfobj );
																										}

																									return value;
																								}

	protected:

		class CBUFFER
		{
				public:
																								CBUFFER																						(UINT size)
																								{
																									_size  = size;
																									buffer = (LPBYTE) malloc(size * sizeof(BYTE));
																								}

																							 ~CBUFFER																						()
																								{
																									free(buffer);
																								}

					void*																	Realloc																						(UINT size)
																								{
																									_size = size;
																									buffer = (LPBYTE) realloc(buffer, size);

																									return buffer;
																								}

					void																	Reset																							()
																								{
																									memset(buffer, NULL, _size);
																								}

					operator															LPBYTE																						()
																								{
																									return buffer;
																								}

					UINT																	GetSize																						()
																								{
																									return _size;
																								}
				
					LPBYTE																buffer;

			private:

					UINT																	_size;
		};

		void																				QueryPerformanceData															(PERF_DATA_BLOCK** perfdata, DWORD objectindex, DWORD counterindex)
																								{

																									static CBUFFER		buffer(XWINDOWSSYSTEM_TOTALBYTES);
																									DWORD							buffersize = buffer.GetSize();
																									LONG							result;

																									XSTRING keyname;
																									keyname.Format(_L("%d"), objectindex);

																									buffer.Reset();
																									while((result = RegQueryValueEx( HKEY_PERFORMANCE_DATA	,	keyname.Get()
																																																					, NULL
																																																					, NULL
																																																					, buffer
																																																					, &buffersize )) == ERROR_MORE_DATA )
																										{

																											buffersize += XWINDOWSSYSTEM_BYTEINCREMENT;
																											buffer.Realloc(buffersize);
																										}

																									*perfdata = (PPERF_DATA_BLOCK) buffer.buffer;
																								}

		T																						GetCounterValue																		(PPERF_OBJECT_TYPE perfobj, DWORD counterindex, LPCTSTR instancename)
																								{
																									PPERF_COUNTER_DEFINITION		perfcntr			= NULL;
																									PPERF_INSTANCE_DEFINITION		perfinst			= NULL;
																									PPERF_COUNTER_BLOCK					counterblock	= NULL;

																									perfcntr = FirstCounter( perfobj );

																									for(DWORD j=0; j<perfobj->NumCounters; j++ )
																										{
																											if (perfcntr->CounterNameTitleIndex == counterindex)  break;

																											perfcntr = NextCounter( perfcntr );
																										}

																									if(perfobj->NumInstances == PERF_NO_INSTANCES)		
																										{
																											counterblock = (PPERF_COUNTER_BLOCK) ((LPBYTE) perfobj + perfobj->DefinitionLength);
																										}
																									 else
																										{
																											perfinst = FirstInstance( perfobj );
		
																											XSTRING instancenamestr;
																											XSTRING instancenamestr2;

																											instancenamestr = instancename;

																											for(int k=0; k<perfobj->NumInstances; k++)
																												{
																													instancenamestr2 = (XCHAR*)((PBYTE)perfinst + perfinst->NameOffset);

																													if(!instancenamestr.Compare(instancenamestr2))
																														{
																															counterblock = (PPERF_COUNTER_BLOCK) ((LPBYTE) perfinst + perfinst->ByteLength);
																															break;
																														}
				
																													perfinst = NextInstance( perfinst );
																												}
																										}

																									if(counterblock)
																										{
																											T* value = NULL;

																											value = (T*)((LPBYTE) counterblock + perfcntr->CounterOffset);
																											return *value;
																										}

																									return -1;
																								}

		T																						GetCounterValueForProcessID												(PPERF_OBJECT_TYPE perfobj, DWORD counterindex, DWORD processID)
																								{
																									int													PROC_ID_COUNTER				= 784;
																									BOOL												processIDexist				= FALSE;
																									PPERF_COUNTER_DEFINITION		perfcntr							= NULL;
																									PPERF_COUNTER_DEFINITION		therequestedperfcntr  = NULL;
																									PPERF_COUNTER_DEFINITION		procIDperfcntr				= NULL;
																									PPERF_INSTANCE_DEFINITION		perfinst							= NULL;
																									PPERF_COUNTER_BLOCK					counterblock					= NULL;

																									perfcntr = FirstCounter(perfobj);

																									for(DWORD j=0; j<perfobj->NumCounters; j++)
																										{
																											if(perfcntr->CounterNameTitleIndex == PROC_ID_COUNTER)
																												{
																													procIDperfcntr = perfcntr;
																													if(therequestedperfcntr) break;
																												}

																											if(perfcntr->CounterNameTitleIndex == counterindex)
																												{
																													therequestedperfcntr = perfcntr;
																													if(procIDperfcntr) break;
																												}

																											perfcntr = NextCounter( perfcntr );
																										}

																									if(perfobj->NumInstances == PERF_NO_INSTANCES)		
																										{
																											counterblock = (PPERF_COUNTER_BLOCK) ((LPBYTE) perfobj + perfobj->DefinitionLength);
																										}
																									 else
																										{
																											perfinst = FirstInstance( perfobj );
		
																											for(int k=0; k<perfobj->NumInstances; k++)
																												{
																													counterblock = (PPERF_COUNTER_BLOCK) ((LPBYTE) perfinst + perfinst->ByteLength);
																													if(counterblock)
																														{
																															T _processID  = 0;

																															_processID = *(T*)((LPBYTE) counterblock + procIDperfcntr->CounterOffset);
																															if((XDWORD)(_processID) == processID)
																																{
																																	processIDexist = TRUE;
																																	break;
																																}
																														}
				
																													perfinst = NextInstance( perfinst );
																												}
																										}

																									if(processIDexist && counterblock)
																										{
																											T *value = NULL;
																											value = (T*)((LPBYTE) counterblock + therequestedperfcntr->CounterOffset);
																											return *value;
																										}

																									return -1;
																								}


		PPERF_OBJECT_TYPE														FirstObject																				(PPERF_DATA_BLOCK perfdata)						{	return( (PPERF_OBJECT_TYPE)((PBYTE)perfdata + perfdata->HeaderLength));												}
		PPERF_OBJECT_TYPE														NextObject																				(PPERF_OBJECT_TYPE perfobj)						{	return( (PPERF_OBJECT_TYPE)((PBYTE)perfobj + perfobj->TotalByteLength));											}
		PPERF_COUNTER_DEFINITION										FirstCounter																			(PPERF_OBJECT_TYPE perfobj)						{	return( (PPERF_COUNTER_DEFINITION) ((PBYTE)perfobj + perfobj->HeaderLength));									}
		PPERF_COUNTER_DEFINITION										NextCounter																				(PPERF_COUNTER_DEFINITION perfcntr)		{	return( (PPERF_COUNTER_DEFINITION)((PBYTE)perfcntr + perfcntr->ByteLength));									}
		PPERF_INSTANCE_DEFINITION										FirstInstance																			(PPERF_OBJECT_TYPE perfobj)						{ return( (PPERF_INSTANCE_DEFINITION)((PBYTE)perfobj + perfobj->DefinitionLength));							}

		PPERF_INSTANCE_DEFINITION										NextInstance																			(PPERF_INSTANCE_DEFINITION PerfInst)
																								{
																									PPERF_COUNTER_BLOCK perfcntrblk;

																									perfcntrblk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst + PerfInst->ByteLength);

																									return( (PPERF_INSTANCE_DEFINITION)((PBYTE)perfcntrblk + perfcntrblk->ByteLength));
																								}
};
#pragma pack(pop)

#endif

class XWINDOWSSYSTEM_CPUUSAGESTATUS
{
	public:
																								XWINDOWSSYSTEM_CPUUSAGESTATUS											();
		virtual																		 ~XWINDOWSSYSTEM_CPUUSAGESTATUS											();

		XSTRING																			processname;		
		XDWORD																			processID;
		bool																				firsttime;
		LONGLONG																		oldvalue;
		LARGE_INTEGER																oldperftime100nsec;

	private:

    void																				Clean																							();
};


class XWINDOWSSYSTEM_CPUUSAGE 
{
	public:
																								XWINDOWSSYSTEM_CPUUSAGE														();
																							 ~XWINDOWSSYSTEM_CPUUSAGE														();

    double																			GetTotalCpuUsage																	();
    double																			GetProcessCpuUsage																(DWORD processID);
      
	private:
   
    ULONGLONG																		SubtractTimes																			(const FILETIME& fta, const FILETIME& ftb);		
    int																					GetNumberOfProcessors															();

		void																				Clean																							();

		FILETIME																		lastsysidletime;
		FILETIME																		lastsyskerneltime;
		FILETIME																		lastsysusertime;
    FILETIME																		lastprockerneltime;
		FILETIME																		lastprocusertime;
    ULONGLONG																		lastsystemtime;
    ULONGLONG																		lastprocesstime;		
};

class XWINDOWSSYSTEM : public XSYSTEM
{
  public:
																								XWINDOWSSYSTEM																		();
    virtual																		 ~XWINDOWSSYSTEM																		();

    XSYSTEM_HARDWARETYPE												GetTypeHardware													          (int* revision = NULL);

		XSYSTEM_PLATFORM														GetPlatform																				(XSTRING* namestring = NULL);	

		bool																				GetOperativeSystemID  														(XSTRING& ID);

    XDWORD																			GetLanguageSO																			();

    XSTRING*																		GetBIOSSerialNumber																();

		XSTRING*																		GetCPUSerialNumber																();

		float																				GetCPUTemperature																	();

    bool																				GetMemoryInfo																			(XDWORD& total, XDWORD& free);

    bool																				GetVolumesInfo																		(XVECTOR<XSYSTEM_VOLUMEINFO*>& volumes);
		
		int																					GetCPUUsageTotal																	();
    int																					GetCPUUsageForProcessName													(XCHAR* processname);
    int																					GetCPUUsageForProcessID														(XDWORD processID);

    XCHAR*																			GetEnviromentVariable															(XCHAR* name);
    bool																				SetEnviromentVariable															(XCHAR* name, XCHAR* value);
    bool																				DelEnviromentVariable															(XCHAR* name);

		bool																				GetUserAndDomain																	(XSTRING& user, XSTRING& domain);

    bool																				ShutDown																					(XSYSTEM_CHANGESTATUSTYPE type);

    int																					Sound_GetLevel																		();  
    bool																				Sound_SetLevel																		(int level); 
    bool																				Sound_SetMutex																		(bool on);   

		bool																				GetBatteryLevel																		(bool& isincharge, XBYTE& levelpercent);  
		int																					GetWifiRSSILevel																	();

		bool																				GetPathExecApplication														(XCHAR* appname, XPATH& apppath);
		

  private:
		
		XWINDOWSSYSTEM_CPUUSAGESTATUS*							AddCPUUsageStatus                                 (XCHAR* processname);
		XWINDOWSSYSTEM_CPUUSAGESTATUS*							GetCPUUsageStatus                                 (XCHAR* processname);

		XWINDOWSSYSTEM_CPUUSAGESTATUS*						  AddCPUUsageStatus																	(XDWORD processID);
		XWINDOWSSYSTEM_CPUUSAGESTATUS*							GetCPUUsageStatus																	(XDWORD processID);

		bool																				DeleteAllCPUUsageStatus                           ();

    void																				Clean																							();

		XVECTOR<XWINDOWSSYSTEM_CPUUSAGESTATUS*>			cpuusagestatus;		
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





