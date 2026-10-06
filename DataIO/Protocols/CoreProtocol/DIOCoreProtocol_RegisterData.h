/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_RegisterData.h
* 
* @class      DIOCOREPROTOCOL_REGISTERDATA
* @brief      Data Input/Output Core Protocol Register Data class
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

#include "XUUID.h"
#include "XSerializable.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_MACHINE          _L("ID_machine")    
#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_CONNECTION       _L("ID_connection")    
#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_PLATAFORM           _L("plataform")
#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_SOIDENTIFIER        _L("soidentifier")
#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_USER                _L("user")    
#define DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_DOMAIN              _L("domain")    

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOCOREPROTOCOL_REGISTERDATA : public XSERIALIZABLE
{
  public:
                            DIOCOREPROTOCOL_REGISTERDATA    ();
    virtual                ~DIOCOREPROTOCOL_REGISTERDATA    ();

    virtual bool            InitializeData                  (bool isserver);

    XUUID*                  GetIDMmachine                   ();
    XUUID*                  GetIDConnection                 ();
    XSTRING*                GetPlataformName                ();
    XSTRING*                GetSOIdentifier                 ();                     
    XSTRING*                GetUser                         ();                      
    XSTRING*                GetDomain                       ();                      

    virtual bool            Serialize                       ();    
    virtual bool            Deserialize                     ();  

    bool                    ShowDebug                       ();

  private:

    bool                    CreateIDMachine                 (XUUID& ID);

    void                    Clean                           ();

    XUUID                   ID_machine;
    XUUID                   ID_connection;
    XSTRING                 plataformname;
    XSTRING                 SOidentifier;
    XSTRING                 user;
    XSTRING                 domain;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




