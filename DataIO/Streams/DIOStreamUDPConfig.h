/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUDPConfig.h
* 
* @class      DIOSTREAMUDPCONFIG
* @brief      Data Input/Output Stream UDP Config class
* @ingroup    DATAIO
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
#include "XThread.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamConfig.h"
#include "DIOStreamConfigString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class DIOSTREAMUDPCONFIG : public DIOSTREAMCONFIG, public DIOSTREAMCONFIGSTRING
{
  public:
                            DIOSTREAMUDPCONFIG          ();
    virtual                ~DIOSTREAMUDPCONFIG          ();

    DIOIP*                  GetLocalIP                  ();

    DIOURL*                 GetRemoteURL                ();
    XSTRING*                GetResolvedRemoteURL        ();

    int                     GetRemotePort               ();
    bool                    SetRemotePort               (int port);

    bool                    IsUsedDatagrams             ();
    void                    SetIsUsedDatagrams          (bool active);

    bool                    IsBroadcastModeActive       ();
    void                    SetBroadcastModeActive      (bool active);

  protected:

    DIOIP                   localIP;
    XSTRING                 resolvedremoteURL;

    int                     remoteoffsetport;

    bool                    isuseddatagrams;
    bool                    isbroadcastmodeactive;

  private:

    void                    Clean                       ();
};




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





