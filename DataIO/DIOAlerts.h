/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOAlerts.h
* 
* @class      DIOALERTS
* @brief      Data Input/Output Alerts class
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


#ifdef DIO_ALERTS_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XMap.h"
#include "XString.h"
#include "XDateTime.h"
#include "XFactory.h"

#include "DIOFactory.h"

#include "DIOAlerts_Conditions.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOALERTLEVEL
{
  DIOALERTLEVEL_UNKNOWN               = 0     ,
  DIOALERTLEVEL_INFO                          ,
  DIOALERTLEVEL_WARNING                       ,
  DIOALERTLEVEL_SERIOUS                       ,
  DIOALERTLEVEL_DANGER                        ,
};


enum DIOALERTSENDER
{
  DIOALERTSSENDER_SMPT                =  0x01 ,
  DIOALERTSSENDER_SMS                 =  0x02 ,
  DIOALERTSSENDER_WEB                 =  0x04 ,
  DIOALERTSSENDER_UDP                 =  0x08 ,

  DIOALERTSSENDER_ALL                 =  (DIOALERTSSENDER_SMPT  | DIOALERTSSENDER_SMS | DIOALERTSSENDER_WEB | DIOALERTSSENDER_UDP),
};


#define DIOALERTS_QSPARAM_APPLICATIONID   _L("applicationID")
#define DIOALERTS_QSPARAM_TYPE            _L("type")
#define DIOALERTS_QSPARAM_LEVEL           _L("level")
#define DIOALERTS_QSPARAM_TITLE           _L("title")
#define DIOALERTS_QSPARAM_ORIGIN          _L("origin")
#define DIOALERTS_QSPARAM_MESSAGE         _L("message")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XDATETIME;
class DIOSTREAMTCPIPCONFIG;
class DIOSMTP;
class DIOWEBCLIENT;
class DIOURL;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDPACKNOWLEDGE;
class DIOIP;

class DIOALERT_CONDITION
{
  public:
                                                    DIOALERT_CONDITION          ();
    virtual                                        ~DIOALERT_CONDITION          ();
    
    int                                             GetTimeLimitForRepeat       ();
    void                                            SetTimeLinitRepeat          (int timelimitforrepeat);

    int                                             GetEveryNumberOfTimes       ();
    void                                            SetEveryNumberOfTimes       (int everynumberoftimes);

    XTIMER*                                         GetLastTimerSend            (); 
    
    int                                             GetNSended                  ();
    void                                            SetNSended                  (int nsended);

    int                                             GetNTimesSended             ();
    void                                            SetNTimesSended             (int ntimessended);
           
  private:

    void                                            Clean                       ();
    
    int                                             timelimitforrepeat;
    int                                             everynumberoftimes;  

    XTIMER*                                         lasttimersend;
    int                                             nsended;
    int                                             ntimessended;
};

class DIOALERT
{
  public:
                                                    DIOALERT                    ();
    virtual                                        ~DIOALERT                    ();

    XDATETIME*                                      GetDateTime                 ();

    XSTRING*                                        Application_GetID            ();

    bool                                            Application_GetVersion       (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr);
    bool                                            Application_SetVersion       (XDWORD version, XDWORD subversion, XDWORD subversionerr);

    XDWORD                                          GetID                       ();
    void                                            SetID                       (XDWORD ID);
    XDWORD                                          CalculateID                 (bool withdatetime = true);

    DIOALERTLEVEL                                   GetLevel                    ();
    void                                            SetLevel                    (DIOALERTLEVEL level);

    XSTRING*                                        GetOrigin                   ();
    XSTRING*                                        GetTitle                    ();
    XSTRING*                                        Get_Message                 ();

    bool                                            CopyFrom                    (DIOALERT* alert);

  private:

    void                                            Clean                       ();

    XDATETIME*                                      xdatetime;
    XSTRING                                         applicationID;
    XDWORD                                          applicationversion;
    XDWORD                                          applicationsubversion;
    XDWORD                                          applicationsubversionerr;
    XDWORD                                          ID;
    DIOALERTLEVEL                                   level;
    XSTRING                                         origin;
    XSTRING                                         title;
    XSTRING                                         message;
};

class DIOALERTS
{
  public:

    static bool                                     GetIsInstanced              ();
    static DIOALERTS&                               GetInstance                 ();
    static bool                                     DelInstance                 ();

    bool                                            Ini                         ();

    XSTRING*                                        Application_GetID            ();
    bool                                            Application_GetVersion       (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr);
    bool                                            Application_SetVersion       (XDWORD version, XDWORD subversion, XDWORD subversionerr);

    XSTRING*                                        GetOrigin                   ();

    bool                                            IsSending                   ();

    DIOALERT*                                       CreateAlert                 (XCHAR* applicationID, DIOALERTLEVEL level, XCHAR* origin, XCHAR* title, XCHAR* message);
    DIOALERT*                                       CreateAlert                 (XSTRING& applicationID, DIOALERTLEVEL level, XSTRING& origin, XSTRING& title, XSTRING& message);
    DIOALERT*                                       CreateAlert                 (DIOALERTLEVEL level, XCHAR* title, XCHAR* message);
    DIOALERT*                                       CreateAlert                 (DIOALERTLEVEL level, XSTRING& title, XSTRING& message);

    bool                                            Sender_SMTPConfig           (XCHAR* URL, int port, XCHAR* login, XCHAR* password, XCHAR* senderemail, int nrecipients, ...);    
    bool                                            Sender_SMSConfig            (DIOSTREAM* diostream, int nrecipients, ...);  
    bool                                            Sender_WEBConfig            (XCHAR* command, bool withget, int nrecipients, ...);
    bool                                            Sender_UDPConfig            (int port, int nrecipients, ...);

    bool                                            IsValidConditionToSend      (XDWORD conditionID);
   
    int                                             Send                        (DIOALERTSENDER sender, XDWORD conditionID, DIOALERT* alert);

    bool                                            AddCondition                (XDWORD conditionID, int timelimitforrepeat, int everynumberoftimes); 
    DIOALERT_CONDITION*                             GetCondition                (XDWORD conditionID);
    bool                                            DeleteAllConditions         (); 

    bool                                            End                         ();

  private:
                                                    DIOALERTS                   (); 
                                                    DIOALERTS                   (DIOALERTS const&);
    virtual                                        ~DIOALERTS                   ();

    void                                            operator =                  (DIOALERTS const&);

    bool                                            Sender_SMTPSend             (DIOALERT* alert); 
    bool                                            Sender_SMSSend              (DIOALERT* alert);
    bool                                            Sender_WEBSend              (DIOALERT* alert); 
    bool                                            Sender_UDPSend              (DIOALERT* alert);

    void                                            Clean                       ();

    static DIOALERTS*                               instance;

    bool                                            isinitialized;

    XSTRING                                         applicationID;
    XDWORD                                          applicationversion;
    XDWORD                                          applicationsubversion;
    XDWORD                                          applicationsubversionerr;
    XSTRING                                         origin;

    XMUTEX*                                         xmutexsending;
    bool                                            issending;

    bool                                            SMTPsenderisactive;
    DIOSTREAMTCPIPCONFIG*                           SMTPdiostreamcfg;
    DIOSTREAM*                                      SMTPdiostream;
    DIOSMTP*                                        SMTP;

    bool                                            SMSsenderisactive;
    DIOSTREAM*                                      SMSdiostream;
    XVECTOR<XSTRING*>                               SMSrecipients;

    XSTRING                                         WEBcommand;
    bool                                            WEBsenderisactive;
    bool                                            WEBisuseget;
    DIOWEBCLIENT*                                   WEBdiowebclient;
    XVECTOR<DIOURL*>                                WEBrecipients;

    bool                                            UDPsenderisactive;
    DIOSTREAMUDPCONFIG*                             UDPdiostreamcfg;
    DIOSTREAMUDPACKNOWLEDGE*                        UDPdiostream;
    XVECTOR<DIOIP*>                                 UDPrecipients;
  
    XMAP<XDWORD, DIOALERT_CONDITION*>               conditions;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



