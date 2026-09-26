/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_Messages.h
* 
* @class      DIOCOREPROTOCOL_MESSAGES
* @brief      Data Input/Output Core Protocol Messages class
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
#include "XMap.h"
#include "XFileJSON.h"

#include "DIOStream.h"

#include "DIOCoreProtocol_Header.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION
{
  DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_UNKNOWN      =  0  ,
  DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_READ               ,  
  DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_WRITE              ,  
};

/*---- CLASS ---------------------------------------------------------c------------------------------------------------*/

class XMUTEX;

class DIOCOREPROTOCOL_MESSAGE
{
  public:
                                                                DIOCOREPROTOCOL_MESSAGE       ();                                              
    virtual                                                    ~DIOCOREPROTOCOL_MESSAGE       ();

    DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION                    GetAcquisitionType            ();
    void                                                        SetAcquisitionType            (DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION acquisitiontype);  
    
    DIOCOREPROTOCOL_HEADER*                                     GetHeader                     (); 
    XBUFFER*                                                    GetContent                    ();

    bool                                                        IsConsumed                    ();
    void                                                        SetIsConsumed                 (bool isconsumed);

    XDWORD                                                      GetSizeAllMessage             ();
    void                                                        SetSizeAllMessage             (XDWORD sizeallmessage);
        
  private:

    void                                                        Clean                         ();

    DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION                    acquisitiontype;
    DIOCOREPROTOCOL_HEADER                                      header; 
    XBUFFER                                                     content;
    bool                                                        isconsumed;     
    XDWORD                                                      sizeallmessage;
};

class DIOCOREPROTOCOL_MESSAGES
{
  public:
                                                                DIOCOREPROTOCOL_MESSAGES      ();                                              
    virtual                                                    ~DIOCOREPROTOCOL_MESSAGES      ();

    XMAP<DIOCOREPROTOCOL_MESSAGE*, DIOCOREPROTOCOL_MESSAGE*>*   GetAll                        (); 
    bool                                                        Delete                        (XUUID* IDmessage);
    bool                                                        DeleteAll                     ();

    int                                                         FindRequest                   (XUUID* IDmessage);
    int                                                         FindResponse                  (XUUID* IDmessage);

    int                                                         FindRequest                   (DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param);
    int                                                         FindResponse                  (DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param);

    bool                                                        AddRequest                    (DIOCOREPROTOCOL_MESSAGE* message); 
    bool                                                        AddResponse                   (DIOCOREPROTOCOL_MESSAGE* message); 

    bool                                                        ShowDebug                     (bool isserver);  

  private:

    void                                                        Clean                         ();

    XMUTEX*                                                     xmutexmessages;    
    XMAP<DIOCOREPROTOCOL_MESSAGE*, DIOCOREPROTOCOL_MESSAGE*>    allmessages;     
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






