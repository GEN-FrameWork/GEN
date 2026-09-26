/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_CFG.h
* 
* @class      DIOCOREPROTOCOL_CFG
* @brief      Data Input/Output Core Protocol CFG class
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


#include "XFileJSON.h"

#include "DIOStreamConfig.h"
#include "DIOStream.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOCOREPROTOCOL_CFG_DEFAULT_MINSIZECOMPRESS                         150 

#define DIOCOREPROTOCOL_CFG_DEFAULT_TIMEOUTNORESPONSE                       5 
#define DIOCOREPROTOCOL_CFG_DEFAULT_TIMETOELIMINATECONNECTIONDISCONNECT     5 
#define DIOCOREPROTOCOL_CFG_DEFAULT_TIMETOCHECKCONNECTION                   60 
#define DIOCOREPROTOCOL_CFG_DEFAULT_NTRYSTOCHECKCONNECTION                  3 

#define DIOCOREPROTOCOL_CFG_DEFAULT_MODEBUS_TIMEOUTBUSFREE                  30 
#define DIOCOREPROTOCOL_CFG_DEFAULT_MODEBUS_TIMEOUTSENDDATA                 180 

/*---- CLASS ---------------------------------------------------------c------------------------------------------------*/

class DIOCOREPROTOCOL_CFG
{
  public:

                                              DIOCOREPROTOCOL_CFG                       ();                                              
    virtual                                  ~DIOCOREPROTOCOL_CFG                       ();

    bool                                      GetIsServer                               ();
    void                                      SetIsServer                               (bool isserver);

    bool                                      GetIsCompressHeader                       ();
    void                                      SetIsCompressHeader                       (bool iscompressheader);
    bool                                      GetIsCompressContent                      ();
    void                                      SetIsCompressContent                      (bool iscompresscontent);

    bool                                      GetIsEncapsulatedBase64                   ();
    void                                      SetIsEncapsulatedBase64                   (bool isencapsulatedbase64);

    XDWORD                                    GetMinSizeCompressContent                 ();
    void                                      SetMinSizeCompressContent                 (XDWORD minsizecompresscontent);

    bool                                      GetIsCipher                               ();
    void                                      SetIsCipher                               (bool iscipher);

    XDWORD                                    GetTimeOutNoResponse                      ();
    void                                      SetTimeOutNoResponse                      (XDWORD timeoutnoresponse);      

    XDWORD                                    GetTimeToEliminateConnectionDisconnect    ();
    void                                      SetTimeToEliminateConnectionDisconnect    (XDWORD timetoeliminateconnectiondisconnect);      

    XDWORD                                    GetTimeToCheckConnection                  ();
    void                                      SetTimeToCheckConnection                  (XDWORD timetocheckconnection);

    XDWORD                                    GetNTrysToCheckConnection                 ();
    void                                      SetNTrysToCheckConnection                 (XDWORD ntrystocheckconnection);

    bool                                      BusMode_Activate                           (bool activate, XDWORD timeoutbusfree = DIOCOREPROTOCOL_CFG_DEFAULT_MODEBUS_TIMEOUTBUSFREE, XDWORD timeoutsenddata = DIOCOREPROTOCOL_CFG_DEFAULT_MODEBUS_TIMEOUTSENDDATA); 
    bool                                      BusMode_IsActive                           ();
    XDWORD                                    BusMode_GetTimeOutBusFree                  (); 
    XDWORD                                    BusMode_GetTimeOutSendData                 (); 
    
  private:

    void                                      Clean                                     ();

    bool                                      isserver;

    bool                                      iscompressheader;
    bool                                      iscompresscontent;
    bool                                      isencapsulatedbase64;  
    XDWORD                                    minsizecompresscontent;    
    
    bool                                      iscipher;

    XDWORD                                    timeoutnoresponse;
    XDWORD                                    timetoeliminateconnectiondisconnect;
    XDWORD                                    timetocheckconnection;  
    XDWORD                                    ntrystocheckconnection;

    bool                                      busmodeactive;
    XDWORD                                    busmodetimeoutbusfree;
    XDWORD                                    busmodetimeoutsenddata;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






