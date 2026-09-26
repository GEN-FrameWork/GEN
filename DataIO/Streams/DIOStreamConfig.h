/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamConfig.h
* 
* @class      DIOSTREAMCONFIG
* @brief      Data Input/Output Stream Config class
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

#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMTYPE
{
  DIOSTREAMTYPE_UNKNOWN               = 0 ,
  DIOSTREAMTYPE_UART                      ,
  DIOSTREAMTYPE_USB                       ,
  DIOSTREAMTYPE_ICMP                      ,
  DIOSTREAMTYPE_UDP                       ,
  DIOSTREAMTYPE_TCPIP                     ,
  DIOSTREAMTYPE_BLUETOOTH                 ,
  DIOSTREAMTYPE_SPI                       ,
  DIOSTREAMTYPE_I2C                       ,
};


enum DIOSTREAMMODE
{
  DIOSTREAMMODE_NONE                  = 0 ,
  
  DIOSTREAMMODE_SERVER                = 1 ,
  DIOSTREAMMODE_MASTER                = 1 ,

  DIOSTREAMMODE_SERVERMULTISOCKET     = 2 ,
 
  DIOSTREAMMODE_SEMISERVER            = 3 ,
  DIOSTREAMMODE_SEMIMASTER            = 3 ,
  
  DIOSTREAMMODE_CLIENT                = 4 ,
  DIOSTREAMMODE_SLAVE                 = 4
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class DIOSTREAMCONFIG : public XSUBJECT
{
  public:

                              DIOSTREAMCONFIG                 ();
    virtual                  ~DIOSTREAMCONFIG                 ();

    DIOSTREAMTYPE             GetType                         ();
    void                      SetType                         (DIOSTREAMTYPE type);

    DIOSTREAMMODE             GetMode                         ();
    void                      SetMode                         (DIOSTREAMMODE mode);

    bool                      IsTLS                           ();
    void                      SetIsTLS                        (bool isTLS);

    bool                      IsServer                        ();

    XDWORD                    GetSizeBufferSO                 ();
    void                      SetSizeBufferSO                 (XDWORD sizebufferSO);

    XTHREADPRIORITY           GetThreadPriority               ();
    void                      SetThreadPriority               (XTHREADPRIORITY threadpriority);

    XDWORD                    GetThreadWaitYield              ();
    void                      SetThreadWaitYield              (XDWORD threadwaityield);

    XDWORD                    GetPollInterval                 ();
    void                      SetPollInterval                 (XDWORD pollinterval);

    
  protected:

    DIOSTREAMTYPE             type;
    DIOSTREAMMODE             mode;
    bool                      isTLS;

    XDWORD                    sizebufferSO;
    XTHREADPRIORITY           threadpriority;
    XDWORD                    threadwaityield;
    XDWORD                    pollinterval;

  private:

    void                      Clean                           ();
};





