/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOMACManufactured.h
* 
* @class      DIOMACMANUFACTURED
* @brief      Data Input/Output MAC Manufactured Device class
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

#include "DIOMAC.h"
#include "DIOWebClient.h"
#include "DIOScraperWeb.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOMACMANUFACTURED_URL             __L("http://standards.ieee.org/develop/regauth/oui/")
#define DIOMACMANUFACTURED_URLNAMEFILE     __L("oui.txt")

#define DIOMACMANUFACTURED_FILE_VERSION    0x0100
#define DIOMACMANUFACTURED_FILE_IDSTRING   __L("[MAC Manufactured ID]")
#define DIOMACMANUFACTURED_FILE_NAME       __L("macmanufacturedid.dat")




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOMACMANUFACTURED
{
  public:
                                  DIOMACMANUFACTURED              ();
    virtual                      ~DIOMACMANUFACTURED              ();

    bool                          Web_GetManufactured             (DIOMAC& MAC, XSTRING& manufactured);

    bool                          File_GetManufactured            (XPATH& xpath, DIOMAC& MAC, XSTRING& manufactured);
    bool                          File_GetManufacturedMACs        (XPATH& xpath, XSTRING& manufactured, XVECTOR<XDWORD>& MACs);

    bool                          File_Download                   (XBUFFER& xbuffer, int timeout = 10);
    bool                          File_Convert                    (XBUFFER& xbuffer, XPATH& xpath);

  private:

    void                          Clean                           ();    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





