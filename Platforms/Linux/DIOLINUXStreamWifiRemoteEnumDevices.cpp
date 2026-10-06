/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamWifiRemoteEnumDevices.cpp
* 
* @class      DIOLINUXSTREAMWIFIREMOTEENUMDEVICES
* @brief      LINUX Data Input/Output Stream Wifi Remote Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMTWIFI_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamWifiRemoteEnumDevices.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
#include <iwlib.h>

#include "XFactory.h"
#include "XVector.h"
#include "XString.h"
#include "XThreadCollected.h"
#include "XTrace.h"

#include "DIOStream.h"
#include "DIOStreamDeviceWifi.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXWIFIENUMFSMSTATE_NONE               ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCH             , DIOLINUXWIFIENUMFSMSTATE_SEARCH           ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCHEND          , DIOLINUXWIFIENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXWIFIENUMFSMSTATE_SEARCH             ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCHEND          , DIOLINUXWIFIENUMFSMSTATE_SEARCHEND        ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXWIFIENUMFSMSTATE_SEARCHEND          ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCH             , DIOLINUXWIFIENUMFSMSTATE_SEARCH           ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMWIFI, _L("DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::~DIOLINUXSTREAMWIFIREMOTEENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::~DIOLINUXSTREAMWIFIREMOTEENUMDEVICES()
{
  StopSearch(true);

  if(threadenumdevices)
    {
      threadenumdevices->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMWIFI, threadenumdevices);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCH);

  threadenumdevices->Run(true);

  sleep(1);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
* @brief      Stop search
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  waitend : Waitend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(IsSearching())
    {
      SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCHEND);

      if(waitend)
        {
          while(GetCurrentState()!=DIOLINUXWIFIENUMFSMEVENT_SEARCHEND)
            {
              usleep(1000);
            }
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  usleep(1000);

  return true;
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
* @brief      Is searching
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices)              return false;
  if(!threadenumdevices->IsRunning()) return false;

  if(GetCurrentState()!=DIOLINUXWIFIENUMFSMSTATE_SEARCH) return false;

  return true;
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
* @brief      Thread enum devices
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{
  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES* enumeration = (DIOLINUXSTREAMWIFIREMOTEENUMDEVICES*)param;
  if(!enumeration) return;

  if(enumeration->GetEvent()==DIOLINUXWIFIENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(enumeration->GetCurrentState())
        {
          case DIOLINUXWIFIENUMFSMSTATE_NONE            : break;

          case DIOLINUXWIFIENUMFSMSTATE_SEARCH          : enumeration->SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCHEND);
                                                          break;

          case DIOLINUXWIFIENUMFSMSTATE_SEARCHEND       : break;
        }
    }
   else
    {
      if(enumeration->GetEvent()<DIOLINUXWIFIENUM_LASTEVENT)
        {
          enumeration->CheckTransition();

          switch(enumeration->GetCurrentState())
            {
              case DIOLINUXWIFIENUMFSMSTATE_NONE            : break;

              case DIOLINUXWIFIENUMFSMSTATE_SEARCH          : { wireless_scan_head  head;
                                                                wireless_scan*      result = NULL;
                                                                iwrange             range;
                                                                int                 sock;
                                                                int                 index = 0;

                                                                memset(&head, 0, sizeof(wireless_scan_head));
                                                                memset(&range, 0, sizeof(iwrange));

                                                                /* Open socket to kernel */
                                                                sock = iw_sockets_open();
                                                                if(sock==-1) break;

                                                                /* Get some metadata to use for scanning */
                                                                if (iw_get_range_info(sock, "wlan0", &range)<0) break;

                                                                /* Perform the scan */
                                                                if(iw_scan(sock, (char*)"wlan0", range.we_version_compiled, &head)<0) break;

                                                                /* Traverse the results */
                                                                result = head.result;
                                                                while (NULL != result)
                                                                  {
                                                                    DIOSTREAMDEVICEWIFI* device = GEN_NEW DIOSTREAMDEVICEWIFI();
                                                                    if(device)
                                                                      {
                                                                        device->SetIndex(index);
                                                                        (*device->GetName()) = result->b.essid;

                                                                        enumeration->devices.Add(device);

                                                                        index++;
                                                                      }

                                                                    result = result->next;
                                                                  }

                                                                iw_sockets_close(sock);
                                                              }
                                                              break;

              case DIOLINUXWIFIENUMFSMSTATE_SEARCHEND       : break;
           }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;
}




#endif



