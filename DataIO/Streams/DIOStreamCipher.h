/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamCipher.h
* 
* @class      DIOSTREAMCIPHER
* @brief      Data Input/Output Stream with Cipher class
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

#include "DIOStream.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMCIPHER_HEAD_MAGICNUMBER   0xAAE00055
#define DIOSTREAMCIPHER_HEAD_SIZE          0x0e

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHER;
class HASHCRC32;
class DIOSTREAM;
class DIOSTREAMCONFIG;

class DIOSTREAMCIPHER : public DIOSTREAM
{
  public:

                                DIOSTREAMCIPHER                 (DIOSTREAMCONFIG* config, CIPHER* cipher);
    virtual                    ~DIOSTREAMCIPHER                 ();

    DIOSTREAMCONFIG*            GetConfig                       ();
    bool                        SetConfig                       (DIOSTREAMCONFIG* config);

    DIOSTREAMSTATUS             GetStatus                       ();

    bool                        Open                            ();
    bool                        Disconnect                      ();
    bool                        Close                           ();



    XDWORD                      Read                            (XBYTE* buffer, XDWORD size);
    XDWORD                      Write                           (XBYTE* buffer, XDWORD size);

    DIOSTREAM*                  GetDIOStreamBase                ();
    CIPHER*                     GetCipher                       ();

  private:

    static void                 ThreadRunFunction               (void* param);

    void                        Clean                           ();

    DIOSTREAMCONFIG*            config;
    CIPHER*                     cipher;

    HASHCRC32*                  hashcrc32;
    DIOSTREAM*                  diostream;

    XMUTEX*                     mutexread;
    XMUTEX*                     mutexwrite;

    XTHREADCOLLECTED*           xthreadconnection;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





