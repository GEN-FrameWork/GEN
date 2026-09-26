/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOPCap_Filters.h
* 
* @class      DIOPCAP_FILTERS
* @brief      Data Input/Output PCap Library Filters class
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

#ifdef DIO_PCAP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

#include "DIOPCap.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOPCAP_FILTERS_PORT_TARGET      0x80000000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOPCAP_FILTER_ENTRY
{
  public:
                                      DIOPCAP_FILTER_ENTRY          ();
    virtual                          ~DIOPCAP_FILTER_ENTRY          ();

    XSTRING*                          GetName                       (); 

    XDWORD                            GetProtocolType               ();
    bool                              SetProtocolType               (XDWORD protocoltype);

    XVECTOR<XDWORD>*                  Ports_GetAll                  (); 

    bool                              Ports_Exists                  (XDWORD port);
    XDWORD                            Ports_Get                     (int index);
    bool                              Ports_Add                     (XDWORD port); 
    bool                              Ports_Del                     (XDWORD port); 
    bool                              Ports_DelAll                  (); 
    
  private:

    void                              Clean                         ();

    XSTRING                           name;
    XDWORD                            protocoltype;
    XVECTOR<XDWORD>                   ports;                       
};

class DIOPCAP_FILTERS
{
  public:
                                      DIOPCAP_FILTERS               ();
    virtual                          ~DIOPCAP_FILTERS               ();

    XVECTOR<DIOPCAP_FILTER_ENTRY*>*   Entrys_GetAll                 ();

    DIOPCAP_FILTER_ENTRY*             Entrys_Get                    (XCHAR* name);
    DIOPCAP_FILTER_ENTRY*             Entrys_Get                    (XSTRING& name);
    DIOPCAP_FILTER_ENTRY*             Entrys_Get                    (int index);

    bool                              Entrys_Set                    (XCHAR* name, XDWORD typeprotocol = DIOPCAPPROTOCOL_TYPE_UNKNOWN, int nports = 0, ...);
    bool                              Entrys_Add                    (XCHAR* name, XDWORD typeprotocol = DIOPCAPPROTOCOL_TYPE_UNKNOWN, int nports = 0, ...);
    bool                              Entrys_Del                    (XCHAR* name);

    bool                              Entrys_Check                  (DIOPCAPFRAME* frame);

    bool                              Entrys_DelAll                 ();
  
  private:

    void                              Clean                         ();

    XVECTOR<DIOPCAP_FILTER_ENTRY*>    entrys;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



#endif




