/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSNMP.h
* 
* @class      DIOSNMP
* @brief      Data Input/Output SNMP (Simple Network Management Protocol)
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

#include "XBER.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSNMP_VERSION
{
  DIOSNMP_VERSION_V1                  = 0  ,
  DIOSNMP_VERSION_V2c                 = 1  ,  // Version 2 classic
  DIOSNMP_VERSION_V2u                 = 2  ,  // decapred no supported by this code 
  DIOSNMP_VERSION_V3                  = 3  
};


enum DIOSNMP_OPERATION
{
  DIOSNMP_OPERATION_GETREQUEST        = 0xA0 ,
  DIOSNMP_OPERATION_GETNEXTREQUEST    = 0xA1 ,
  DIOSNMP_OPERATION_GETRESPONSE       = 0xA2 ,
  DIOSNMP_OPERATION_SETREQUEST        = 0xA3 ,
  DIOSNMP_OPERATION_TRAP              = 0xA4 
};

#define DIOSNMPBERTYPE_IPADDRESS                0x00
#define DIOSNMPBERTYPE_COUNTER32                0x01
#define DIOSNMPBERTYPE_GAUGE32                  0x02
#define DIOSNMPBERTYPE_TIMETICKS                0x03
#define DIOSNMPBERTYPE_OPAQUE                   0x04
#define DIOSNMPBERTYPE_NSAPADDRESS              0x05
#define DIOSNMPBERTYPE_COUNTER64                0x06  // Available only in SNMP v2
#define DIOSNMPBERTYPE_UINTEGER32               0x07  // Available only in SNMP v2

#define DIOSNMPBERTYPE_SEQUENCE                 0x10

#define DIOSNMPBERTYPE_GETRESQUESTPDU           0x00
#define DIOSNMPBERTYPE_GETNEXTRESQUESTPDU       0x01
#define DIOSNMPBERTYPE_GETRESPONSEPDU           0x02  // Response-PDU in SNMP v2
#define DIOSNMPBERTYPE_SETREQUESTPDU            0x03
#define DIOSNMPBERTYPE_TRAPPDU                  0x04  // Obsolete in SNMP v2
#define DIOSNMPBERTYPE_GETBULKREQUESTPDU        0x05  // Added in SNMP v2
#define DIOSNMPBERTYPE_INFORMRESQUESTPDU        0x06  // Added in SNMP v2
#define DIOSNMPBERTYPE_SNMPV2TRAPPDU            0x07  // Added in SNMP v2

#define DIOSNMP_DEFAULT_PORT                    161
#define DIOSNMP_DEFAULT_TRAPPORT                162



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;

class DIOSNMP_XBER :  public XBER
{
  public:

                              DIOSNMP_XBER                  ();
    virtual                  ~DIOSNMP_XBER                  ();

    bool                      SetIPADDRESS                  (XCHAR* IPstring);
    bool                      SetTIMETICKS                  (XDWORD ticks);

    bool                      Sequence_AddTo                (XBER& newxber);
    bool                      TrapData_AddTo                (XBER& newxber);
};

class DIOSNMP_BASE
{
  public:
                              DIOSNMP_BASE                  ();
    virtual                  ~DIOSNMP_BASE                  ();

    DIOSTREAMUDPCONFIG*       GetDIOStreamUDPCFG            ();
    DIOSTREAMUDP*             GetDIOStreamUDP               ();  

    DIOSNMP_VERSION           GetVersion                    ();
    void                      SetVersion                    (DIOSNMP_VERSION version);     
    
    bool                      Open                          (XCHAR* IPtarget, int port = DIOSNMP_DEFAULT_TRAPPORT, bool checkavailableNMS = true, XCHAR* localIP = NULL);    
    bool                      Close                         ();  


  protected: 

    DIOSNMP_VERSION           version;
    DIOSTREAMUDPCONFIG*       diostreamudpcfg;
    DIOSTREAMUDP*             diostreamudp;

  private:

     void                     Clean                         ();
};

class DIOSNMP_TRAP : public DIOSNMP_BASE
{
   public:
                              DIOSNMP_TRAP                  ();
    virtual                  ~DIOSNMP_TRAP                  ();
    
    bool                      Send                          (XCHAR* community, XCHAR* enterprise, XCHAR* agentip, int generictrap, int specifictrap, XDWORD timestamp, int nvar, ...);
   
   private:

    void                      Clean                         ();        
};


class DIOSNMP : public DIOSNMP_BASE
{
   public:
                              DIOSNMP                       ();
    virtual                  ~DIOSNMP                       ();

    bool                      Get                           (DIOSNMP_OPERATION operation, XCHAR* community, XCHAR* OIDstr, DIOSNMP_XBER& xberout);
    bool                      Send                          (XCHAR* community, XCHAR* enterprise, XCHAR* agentip, int generictrap, int specifictrap, XDWORD timestamp, int nvar, ...);
    
   private:

    void                      Clean                         ();        
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





