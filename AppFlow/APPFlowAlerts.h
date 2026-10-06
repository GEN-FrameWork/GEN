/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowAlerts.h
* 
* @class      APPFLOWALERTS
* @brief      Application Flow Alerts class
* @ingroup    APPFLOW
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

#include "XPath.h"
#include "XString.h"

#include "DIOAlerts.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum APPFLOW_ALERT_TYPE
{
  APPFLOW_ALERT_TYPE_SMTP              = 0 ,
  APPFLOW_ALERT_TYPE_SMS                   ,
  APPFLOW_ALERT_TYPE_WEB                   ,
  APPFLOW_ALERT_TYPE_UDP                   ,

  APPFLOW_ALERT_TYPE_MAX
};

enum APPFLOW_ALERT_STATUS
{
  APPFLOW_ALERT_STATUS_NOTACTIVATED    = 0 ,  
  APPFLOW_ALERT_STATUS_NOTRECIPIENTS       ,
  APPFLOW_ALERT_STATUS_NOTCONFIG           ,  
  APPFLOW_ALERT_STATUS_ACTIVE              ,  
};

#define APPFLOW_ALERTS_WEBALERTCMD   _L("alert")

#define APPFLOW_ALERTS               APPFLOWALERTS ::GetInstance()

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAM;
class APPFLOWCFG;

class APPFLOWALERTS
{
  public:

    static bool                         GetIsInstanced                  ();
    static APPFLOWALERTS&               GetInstance                     ();
    static bool                         DelInstance                     ();

    bool                                Ini                             (APPFLOWCFG* cfg, XCHAR* applicationname, int appversion, int appsubversion, int appsubversionerror, int status[APPFLOW_ALERT_TYPE_MAX], DIOSTREAM* streamSMS = NULL);
    int                                 Send                            (XDWORD senders, XDWORD condition, DIOALERTLEVEL level, XCHAR* title, XCHAR* message);
    bool                                End                             ();

  private:
                                        APPFLOWALERTS                   ();
                                        APPFLOWALERTS                   (APPFLOWALERTS const&);
    virtual                            ~APPFLOWALERTS                   ();

    void                                operator =                      (APPFLOWALERTS const&);

    void                                Clean                           ();

    static APPFLOWALERTS*               instance;

    APPFLOWCFG*                         cfg;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif


