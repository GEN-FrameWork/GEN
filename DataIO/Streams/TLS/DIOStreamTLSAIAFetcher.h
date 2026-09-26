/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSAIAFetcher.h
*
* @class      DIOSTREAMTLSAIAFETCHER
* @brief      Data Input/Output Stream TLS Authority Information Access (AIA) Fetcher class
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

#include "XBuffer.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLSAIAFETCHER_DEFAULTPORT      80                  // CA Issuers access locations are published over plain HTTP -- RFC 5280
                                                                     // does not require HTTPS for them, and requiring it here would make
                                                                     // completing this TLS chain validation depend on a second, unrelated
                                                                     // TLS validation succeeding first (only https:// is ever what's actually
                                                                     // missing; the fetch itself is never allowed to run over it, see Fetch()).
#define DIOSTREAMTLSAIAFETCHER_TIMEOUT           5                  // seconds -- a best-effort fetch that must never stall a handshake
#define DIOSTREAMTLSAIAFETCHER_MAXBODYSIZE      (16*1024)           // an intermediate DER certificate is a few KB; refuse anything unusual
#define DIOSTREAMTLSAIAFETCHER_MAXHEADERSIZE    (8*1024)
#define DIOSTREAMTLSAIAFETCHER_READBLOCKSIZE     4096

#define DIOSTREAMTLSAIAFETCHER_MAXCHAINFETCHES   3                  // hard cap on how many missing-intermediate hops a single chain
                                                                     // completion will chase, independent of CIPHERCERTIFICATEX509VALIDATOR's
                                                                     // own CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE cap on the final size

enum DIOSTREAMTLSAIAFETCHER_CHUNKRESULT
{
  DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_ERROR           = -1 ,
  DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_INCOMPLETE       =  0 ,
  DIOSTREAMTLSAIAFETCHER_CHUNKRESULT_COMPLETE             ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTCPIP;
class DIOIP;


class DIOSTREAMTLSAIAFETCHER
{
  public:
                                            DIOSTREAMTLSAIAFETCHER          ();
    virtual                                ~DIOSTREAMTLSAIAFETCHER          ();

    bool                                    Fetch                           (XSTRING& url, XBUFFER& tobuffer, int timeout = DIOSTREAMTLSAIAFETCHER_TIMEOUT, XDWORD maximumheadersize = DIOSTREAMTLSAIAFETCHER_MAXHEADERSIZE, XDWORD maximumbodysize = DIOSTREAMTLSAIAFETCHER_MAXBODYSIZE);

  private:

    bool                                    Destination_IsPublic            (DIOIP& IP);
    bool                                    Exchange                        (DIOSTREAMTCPIP* diostream, XSTRING& server, XSTRING& resource, int timeout, XBUFFER& raw);
    bool                                    ResponseBody_Extract            (XBUFFER& raw, XBUFFER& tobuffer);
    DIOSTREAMTLSAIAFETCHER_CHUNKRESULT      ChunkSize_Get                   (XBUFFER& input, XQWORD& chunksize);

    void                                    Clean                           ();

    XDWORD                                  maximumheadersize;
    XDWORD                                  maximumbodysize;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
