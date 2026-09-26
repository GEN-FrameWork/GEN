/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSWFP.h
* 
* @class      DIOWINDOWSWFP
* @brief      WINDOWS Data Input/Output Windows Filtering Platform class
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

#ifdef DIO_ACTIVE
#ifdef DIO_WINDOWSWFP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <winsock2.h>
#include <windows.h>

#include "XBase.h"
#include "XString.h"
#include "XVector.h"

#include "DIOIP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOWINDOWSWFP_DNS_PORT                             53

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWINDOWSWFP
{
  public:

                                  DIOWINDOWSWFP                 ();
    virtual                      ~DIOWINDOWSWFP                 ();

    bool                          Ini                           (bool dynamicsession = true);
    bool                          End                           ();

    bool                          IsInitialized                 ();
    XDWORD                        GetLastError                  ();

    bool                          AddBlockRemoteIPv4            (XCHAR* remoteIP, XWORD remoteport = 0, XBYTE protocol = 0, XCHAR* filtername = NULL);
    bool                          AddBlockRemoteIPv4            (XSTRING& remoteIP, XWORD remoteport = 0, XBYTE protocol = 0, XCHAR* filtername = NULL);
    bool                          AddBlockRemoteIPv4            (DIOIP& remoteIP, XWORD remoteport = 0, XBYTE protocol = 0, XCHAR* filtername = NULL);

    bool                          AddBlockRemotePortIPv4        (XWORD remoteport, XBYTE protocol, XCHAR* filtername = NULL);
    bool                          AddBlockRemotePortIPv6        (XWORD remoteport, XBYTE protocol, XCHAR* filtername = NULL);

    bool                          AddBlockDNSExternal           (bool excludeownprocess = true);
    bool                          AddBlockDNSExternal           (XCHAR* applicationpathexception);
    bool                          AddBlockDNSExternal           (XSTRING& applicationpathexception);

    bool                          DeleteFilters                 ();

  private:

    bool                          Provider_Add                  ();
    bool                          SubLayer_Add                  ();
    bool                          AddBlockRemotePort            (const GUID& layerkey, XWORD remoteport, XBYTE protocol, XCHAR* applicationpathexception, XCHAR* filtername);
    bool                          AddFilter                     (const GUID& layerkey, void* filterconditions, XDWORD nfilterconditions, UINT64& filterID, XCHAR* filtername);

    bool                          GetCurrentProcessPath         (XSTRING& path);
    bool                          GetAppIDFromPath              (XCHAR* applicationpath, void** appID);
    bool                          FreeAppID                     (void** appID);

    XDWORD                        IPv4ToUINT32                  (DIOIP& IP);

    void                          Clean                         ();

    HANDLE                        enginehandle;
    bool                          initialized;
    XDWORD                        lasterror;
    bool                          dynamicsession;

    XVECTOR<UINT64>               filtersID;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif
#endif

