/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONTP.h
* 
* @class      DIONTP
* @brief      Data Input/Output NTP (Network Time Protocol) UTC class
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

#include "XDateTime.h"

#include "DIOURL.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIONTP_DEFAULTPORT        123
#define DIONTP_JAN1ST1900         2415021
#define DIONTP_DEFAULTTIMEOUT     10              



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;

#pragma pack(push, r1, 1)
class DIOTIMEPACKET
{
  public:

    XDWORD                        integer;
    XDWORD                        fractional;
};
#pragma pack(pop, r1)


#pragma pack(push, r2, 1)
class DIONTPBASICINFO
{
  public:

    XBYTE                         livnmode;
    XBYTE                         stratum;






    XBYTE                         poll;
    XBYTE                         precision;
    XDWORD                        rootdelay;
    XDWORD                        rootdispersion;
    XBYTE                         referenceID[4];
    DIOTIMEPACKET                 referencetimestamp;
    DIOTIMEPACKET                 originatetimestamp;
    DIOTIMEPACKET                 receivetimestamp;
    DIOTIMEPACKET                 transmittimestamp;
};
#pragma pack(pop, r2)


#pragma pack(push, r3, 1)
class DIONTPAUTHENTICATIONINFO
{
  public:

    XDWORD                        keyID;
    XBYTE                         messagedigest[16];
};
#pragma pack(pop, r3)


#pragma pack(push, r4, 1)
class DIONTPFULLPACKET
{
  public:

    DIONTPBASICINFO               basic;
    DIONTPAUTHENTICATIONINFO      authentication;
};
#pragma pack(pop, r4)

#pragma pack(push, r5, 1)
class DIONTPRESPONSE
{
  public:

    int                           leapindicator;
    int                           stratum;
    XQWORD                        originatetime;
    XQWORD                        receivetime;
    XQWORD                        transmittime;
    XQWORD                        destinationtime;
    double                        roundtripdelay;
    double                        localclockoffset;
};
#pragma pack(pop, r5)

class DIONTP
{
  public:

                                  DIONTP                              ();
    virtual                      ~DIONTP                              ();

    bool                          GetTimeResponse                     (XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response);
    bool                          GetTimeResponse                     (DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response);

    bool                          GetTimeSeconds                      (XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds);
    bool                          GetTimeSeconds                      (DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds);

    bool                          GetTime                             (XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xtime);
    bool                          GetTime                             (DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xtime);

  private:

    XQWORD                        ConvertTimePacket                   (DIOTIMEPACKET& timepacket, bool hardwareuselittleendian);

    void                          Clean                               ();

    XTIMER*                       xtimer;

    DIOSTREAMUDPCONFIG*           diostreamudpcfg;
    DIOSTREAMUDP*                 diostreamudp;
};




