/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUDP.h
* 
* @class      DIOSTREAMUDP
* @brief      Data Input/Output Stream UDP class
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

#include "XVector.h"
#include "XBuffer.h"

#include "DIOStream.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMUDP_NOTFOUND             -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMUDPCONFIG;
class DIOURL;

class DIOSTREAMUDPDATAGRAM
{
  public:
                                      DIOSTREAMUDPDATAGRAM                      ();
    virtual                          ~DIOSTREAMUDPDATAGRAM                      ();


    bool                              IsToSend                                  ();
    void                              SetIsToSend                               (bool istosend);

    XSTRING*                          GetAddress                                ();

    bool                              SetAddress                                (XCHAR* address);
    bool                              SetAddress                                (XSTRING& address);

    XWORD                             GetPort                                   ();
    void                              SetPorts                                  (XWORD port);

    XBUFFER*                          GetData                                   ();

    bool                              SetData                                   (XBYTE* data, XDWORD size);
    void                              SetData                                   (XBUFFER& data);

  private:

    void                              Clean                                     ();

    bool                              istosend;
    XSTRING                           address;
    XWORD                             port;
    XBUFFER*                          data;
};

class DIOSTREAMUDP : public DIOSTREAM
{
  public:
                                      DIOSTREAMUDP                              ();
    virtual                          ~DIOSTREAMUDP                              ();

    DIOSTREAMCONFIG*                  GetConfig                                 ();
    bool                              SetConfig                                 (DIOSTREAMCONFIG* config);

    virtual bool                      Open                                      ();

    bool                              ReadDatagram                              (XSTRING& address, XWORD& port, XBUFFER& xbuffer);

    bool                              WriteDatagram                             (XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size);
    bool                              WriteDatagram                             (XSTRING& address, XWORD port, XBUFFER& xbuffer);

    virtual bool                      Disconnect                                ();
    virtual bool                      Close                                     ();

    bool                              ResetXBuffers                             ();
    bool                              ResetInXBuffer                            ();
    bool                              ResetOutXBuffer                           ();

    DIOURL*                           GetHost                                   ();

    bool                              WaitToWriteDatagramsEmpty                 (int timeout);
    int                               WaitToGetFirstDatagram                    (bool tosend, int timeout);

    XVECTOR<DIOSTREAMUDPDATAGRAM*>*   GetDatagramsVector                        ();

    DIOSTREAMUDPDATAGRAM*             GetDatagram                               (int index);

    bool                              DeleteDatagram                            (int index);
    bool                              DeleteAllDatagrams                        ();

  protected:

    bool                              AddDatagram                               (bool istosend, XCHAR* address, XWORD port, XBYTE* data, XDWORD size);
    bool                              AddDatagram                               (bool istosend, XSTRING& address, XWORD port, XBYTE* data, XDWORD size);
    
    int                               GetFirstDatagram(bool issend);

    DIOSTREAMUDPCONFIG*               config;
    DIOURL*                           host;

  private:

    void                              Clean                                     ();

    XMUTEX*                           datagramsmutex;
    XVECTOR<DIOSTREAMUDPDATAGRAM*>    datagrams;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





