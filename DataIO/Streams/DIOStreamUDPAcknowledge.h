/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUDPAcknowledge.h
* 
* @class      DIOSTREAMUDPACKNOWLEDGE
* @brief      Data Input/Output Stream UDP with Acknowledge class
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

#include "XThreadCollected.h"

#include "DIOStreamUDP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT 5

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASHCRC32;
class DIOSTREAMUDPCONFIG;

class DIOSTREAMUDPACKNOWLEDGE : public DIOSTREAMUDP
{
  public:
                                    DIOSTREAMUDPACKNOWLEDGE               (DIOSTREAMUDPCONFIG* config);
    virtual                        ~DIOSTREAMUDPACKNOWLEDGE               ();

    DIOSTREAMCONFIG*                GetConfig                             ();
    bool                            SetConfig                             (DIOSTREAMCONFIG* config);
    
    DIOSTREAMSTATUS                 GetStatus                      ();
    
    int                             GetTimeout                            ();
    void                            SetTimeout                            (int timeout = DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT);

    bool                            Open                                  ();

    XDWORD                          Write                                 (XBYTE* buffer, XDWORD size);

    bool                            WriteDatagram                         (XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size);
    bool                            WriteDatagram                         (XSTRING& address, XWORD port, XBUFFER& xbuffer);

    bool                            Disconnect                            ();
    bool                            Close                                 ();

    DIOSTREAM*                      GetDIOStreamBase                      ();

  private:
   
    static void                     ThreadRunFunction                     (void* param); 
    void                            Clean                                 ();
    
    DIOSTREAMUDPCONFIG*             config;
    int                             timeout;
    DIOURL*                         URLremote;
    XSTRING                         addressremote;
    XWORD                           portremote;
    DIOSTREAMUDP*                   diostream;
    XTHREADCOLLECTED*               xthreadconnection;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





