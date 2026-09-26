/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOATCMDGSM.h
* 
* @class      DIOATCMDGSM
* @brief      Data Input/Output AT command for GSM protocol class
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

#include "XBase.h"
#include "DIOATCMDS.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum
{
  DIOATCMD_TYPE_GSMGETMANUFACTURED          = DIOATCMD_TYPE_GSM ,
  DIOATCMD_TYPE_GSMGETMODEL                                     ,
  DIOATCMD_TYPE_GSMGETSERIAL                                    ,
  DIOATCMD_TYPE_GSMGETVERSION                                   ,
  DIOATCMD_TYPE_GSMGETSIGNALQUALITY                             ,
  DIOATCMD_TYPE_GSMGETSERVICES                                  ,
  DIOATCMD_TYPE_GSMGETIMSI                                      ,
  DIOATCMD_TYPE_GSMPIN                                          ,
  DIOATCMD_TYPE_GSMCOMMAND                                      ,
  DIOATCMD_TYPE_GSMGETSMSSUPPORT                                ,
  DIOATCMD_TYPE_GSMGETSMSFORMAT                                 ,
  DIOATCMD_TYPE_GSMSETSMSFORMAT                                 ,
  DIOATCMD_TYPE_GSMGETSMSCENTER                                 ,
  DIOATCMD_TYPE_GSMSETSMSCENTER                                 ,
  DIOATCMD_TYPE_GSMSENDSMS                                      ,
};

#define DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED     0x01
#define DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED     0x02
#define DIOATCMDGSM_SMSSUPPORT_BROADCAST            0x04

#define DIOATCMDGSM_SMSFORMAT_PDU                   0x01
#define DIOATCMDGSM_SMSFORMAT_TXT                   0x02

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOATCMDGSM_ALPHABET
{
  public:

    virtual XWORD           Traslate                      (bool from, XWORD data) = 0;
};

class DIOATCMDGSM_ALPHABET_8859_1 : public DIOATCMDGSM_ALPHABET
{
  public:

                            DIOATCMDGSM_ALPHABET_8859_1   ();
    virtual                ~DIOATCMDGSM_ALPHABET_8859_1   ();

    virtual XWORD           Traslate                      (bool from, XWORD data);

  private:

    bool                    CreateTable                   ();
    void                    Clean                         ();

    XBYTE                   table[138][2];
};

class DIOATCMDGSM  : public DIOATCMDS
{
  public:
                            DIOATCMDGSM                   (DIOSTREAM* diostream);
    virtual                ~DIOATCMDGSM                   ();

    DIOATCMD_ERROR          GetManufactured               (XSTRING& manufactured, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetModel                      (XSTRING& model, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetIMEI                       (XSTRING& IMEI, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetVersion                    (XSTRING& version, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetSignalQuality              (int& RSSI, int& BER, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          IsAvailableServiceGSM         (bool& available, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetIMSI                       (XSTRING& IMSI, int timeout = DIOATCMDS_TIMEOUT);

    DIOATCMD_ERROR          PIN_Is                        (bool& isresolved, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          PIN_Enter                     (XCHAR* PIN, bool& isenter, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          PIN_Activate                  (XCHAR* PIN, bool activate, int timeout = DIOATCMDS_TIMEOUT);
 
    DIOATCMD_ERROR          GetSMSSupport                 (XBYTE& support, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetSMSFormat                  (XBYTE& format, bool support = false, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          SetSMSFormat                  (XBYTE format, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          GetSMSCenter                  (XSTRING& SMScenter, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          SetSMSCenter                  (XCHAR* smscenter, int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR          SendSMS                       (XCHAR* number, XCHAR* message, int* mrID = NULL, int timeout = DIOATCMDS_TIMEOUT);

  private:

    void                    Clean                         ();
    bool                    CodecPDUFormat                (XCHAR* number, XCHAR* message, bool inoctets, int validperiod, XSTRING& length, XSTRING& result);

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





