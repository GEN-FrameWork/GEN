/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamICMP.h
* 
* @class      DIOSTREAMICMP
* @brief      Data Input/Output Stream ICMP class
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

#define DIOSTREAMICMP_NOTFOUND            -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XMUTEX;
class DIOURL;
class DIOSTREAMICMPCONFIG;

class DIOSTREAMICMPDATAGRAM
{
  public:
                                      DIOSTREAMICMPDATAGRAM                     ();
    virtual                          ~DIOSTREAMICMPDATAGRAM                     ();

    bool                              IsToSend                                  ();
    void                              SetIsToSend                               (bool istosend);

    XSTRING*                          GetAddress                                ();
    bool                              SetAddress                                (XCHAR* address);
    bool                              SetAddress                                (XSTRING& address);

    XBUFFER*                          GetData                                   ();
    bool                              SetData                                   (XBYTE* data, XDWORD size);
    void                              SetData                                   (XBUFFER& data);

  private:

    void                              Clean                                     ();

    bool                              istosend;
    XSTRING                           address;
    XBUFFER*                          data;
};

class DIOSTREAMICMP : public DIOSTREAM
{
  public:
                                      DIOSTREAMICMP                             ();
    virtual                          ~DIOSTREAMICMP                             ();

    DIOSTREAMCONFIG*                  GetConfig                                 ();
    bool                              SetConfig                                 (DIOSTREAMCONFIG* config);

    virtual bool                      Open                                      ()                                = 0;

    XDWORD                            Read                                      (XBYTE* buffer, XDWORD size);
    XDWORD                            Write                                     (XBYTE* buffer, XDWORD size);

    bool                              ReadDatagram                              (XSTRING& address, XBUFFER& xbuffer);

    bool                              WriteDatagram                             (XSTRING& address, XBYTE* buffer, XDWORD size);
    bool                              WriteDatagram                             (XSTRING& address, XBUFFER& xbuffer);

    virtual bool                      Disconnect                                ()                                = 0;
    virtual bool                      Close                                     ()                                = 0;

    bool                              ResetXBuffers                             ();
    bool                              ResetInXBuffer                            ();
    bool                              ResetOutXBuffer                           ();

    DIOURL*                           GetHost                                   ();

    XVECTOR<DIOSTREAMICMPDATAGRAM*>*  GetDatagramsVector                        ();
    bool                              DeleteAllDatagrams                        ();

  protected:

    bool                              AddDatagram                               (bool istosend, XCHAR* address, XBYTE* data, XDWORD size);
    bool                              AddDatagram                               (bool istosend, XSTRING& address, XBYTE* data, XDWORD size);

    DIOSTREAMICMPDATAGRAM*            GetDatagram                               (int index);
    int                               GetFirstDatagram                          (bool issend);

    bool                              DeleteDatagram                            (int index);


    DIOSTREAMICMPCONFIG*              config;
    DIOURL*                           host;

  private:

    void                              Clean                                     ();

    XMUTEX*                           datagramsmutex;
    XVECTOR<DIOSTREAMICMPDATAGRAM*>   datagrams;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




