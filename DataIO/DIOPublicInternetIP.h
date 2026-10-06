/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOPublicInternetIP.h
* 
* @class      DIOPUBLICINTERNETIP
* @brief      Data Input/Output Public InternetIP
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

#include "DIOIP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOPUBLICINTERNETIP_URL                                     _L("ipecho.net/plain");

#define DIOPUBLICINTERNETIP_DEFAULTNCHECKSFOREVERYCONNECTION          5
#define DIOPUBLICINTERNETIP_DEFAULTTIMERCONNECTIONCHECK             100   // milliseconds

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOPUBLICINTERNETIP
{
  public:

    static bool                     GetIsInstanced                    ();
    static DIOPUBLICINTERNETIP&     GetInstance                       ();
    static bool                     DelInstance                       ();   

    bool                            FastIsThereInternetConnection     ();

    bool                            Get                               (XSTRING& publicIP);
    bool                            Get                               (DIOIP& publicIP);

  private:
                                    DIOPUBLICINTERNETIP               ();
                                    DIOPUBLICINTERNETIP               (DIOPUBLICINTERNETIP const&);
    virtual                        ~DIOPUBLICINTERNETIP               ();

    void                            operator =                        (DIOPUBLICINTERNETIP const&);

    void                            Clean                             ();

    static DIOPUBLICINTERNETIP*     instance;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







