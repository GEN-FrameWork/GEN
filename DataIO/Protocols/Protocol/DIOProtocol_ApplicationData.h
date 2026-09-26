/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol_ApplicationData.h
* 
* @class      DIOPROTOCOL_APPLICATIONDATA
* @brief      Data Input/Output Application Data for Generic Binary Protocol class
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
#include "XString.h"

#ifdef DIO_ALERTS_ACTIVE
#include "DIOAlerts.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOPROTOCOL_APPLICATIONDATA
{
  public:
                                        DIOPROTOCOL_APPLICATIONDATA         ();
    virtual                            ~DIOPROTOCOL_APPLICATIONDATA         ();

    #ifdef DIO_ALERTS_ACTIVE
    bool                                AddAlert                            (DIOALERT& alert);
    bool                                ExtractAlert                        (int index, DIOALERT& alert);
    bool                                DeleteAllAlerts                     ();
    #endif

    XWORD                               protocolversion;
    XWORD                               protocolsubversion;
    XWORD                               protocolsubversionerr;

    XWORD                               applicationversion;
    XWORD                               applicationsubversion;
    XWORD                               applicationsubversionerr;

    XSTRING                             applicationname;


  private:

    void                                Clean                               ();

    XMUTEX*                             xmutexalert;

    #ifdef DIO_ALERTS_ACTIVE
    XVECTOR<DIOALERT*>                  alerts;
    #endif
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





