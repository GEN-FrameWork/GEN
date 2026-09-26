/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCLIProtocol.h
* 
* @class      DIOCLIPROTOCOL
* @brief      Data Input/Output protocol in CLI class
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

#include "XString.h"
#include "XTimer.h"
#include "XFSMachine.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOCLIPROTOCOL_ERROR
{
  DIOCLIPROTOCOL_ERROR_NONE                    = 0 ,
  DIOCLIPROTOCOL_ERROR_INTERNAL                    ,
  DIOCLIPROTOCOL_ERROR_INVALIDCOMMAND              ,
  DIOCLIPROTOCOL_ERROR_INVALIDPARAM                ,
  DIOCLIPROTOCOL_ERROR_INSUFFICIENTPARAMS          ,
  DIOCLIPROTOCOL_ERROR_ERROREXECCOMMAND            ,
  DIOCLIPROTOCOL_ERROR_TIMEOUT                     ,
  DIOCLIPROTOCOL_ERROR_WRITECMD                    ,
  DIOCLIPROTOCOL_ERROR_NOTANSWER                   ,
};

#define DIOCLIPROTOCOL_MARK_DONOTANSWER     __C('#')
#define DIOCLIPROTOCOL_MARK_ISANSWER        __C('>')
#define DIOCLIPROTOCOL_MARK_ORIGIN          __L("[@]")
#define DIOCLIPROTOCOL_MARK_TARGET          __L("[$]") 
#define DIOCLIPROTOCOL_MARK_BROADCAST       __L("[*]") 
#define DIOCLIPROTOCOL_MARK_ANSWER          __L("[-]") 
#define DIOCLIPROTOCOL_MARK_CRC32           __L("[|]") 

#define DIOCLIPROTOCOL_OK                   __L("ok")
#define DIOCLIPROTOCOL_ERROR                __L("error")

#define DIOCLIPROTOCOL_TIMEOUT              5  //Seconds




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAM;

class DIOCLIPROTOCOLCOMMAND
{
  public:
                                          DIOCLIPROTOCOLCOMMAND         ();
    virtual                              ~DIOCLIPROTOCOLCOMMAND         ();
    
    XCHAR*                                GetCommand                    ();
    int                                   GetNParams                    ();
    bool                                  Set                           (XCHAR* command, int nparams);
                                          
  private:

    void                                  Clean                         ();
    
    XSTRING                               command;
    int                                   nparams;
};

class DIOCLIPROTOCOLANSWER
{
  public:
                                          DIOCLIPROTOCOLANSWER          ();                                       
    virtual                              ~DIOCLIPROTOCOLANSWER          ();                                        

    XSTRING*                              GetOriginID                   ();
    XSTRING*                              GetCommand                    ();
    XSTRING*                              GetAnswer                     ();

  private:

    void                                  Clean                         ();
    
    XSTRING                               originID;                                      
    XSTRING                               command;
    XSTRING                               answer;
};

class DIOCLIPROTOCOL
{
  public:
                                          DIOCLIPROTOCOL                ();
    virtual                              ~DIOCLIPROTOCOL                ();

    virtual bool                          Ini                           (DIOSTREAM* diostream, XCHAR* ID, int timeout = DIOCLIPROTOCOL_TIMEOUT);

    bool                                  IsIni                         ();  

    void                                  ActiveCRC                     (bool activated = true);
    
    virtual bool                          SendCommand                   (XCHAR* command, XSTRING* target, XSTRING* answer, int timeoutanswer, ...);
    bool                                  SendCommandArg                (XCHAR* command, XSTRING* target, XSTRING* answer, int timeoutanswer, va_list* arg);
    
    virtual bool                          ReceivedCommand               (XSTRING& originID, XSTRING& command, XVECTOR<XSTRING*>& params, XSTRING& answer);    
    virtual bool                          ReceivedAnswer                (XSTRING& origin, XSTRING& command, XSTRING& answer);

    void                                  ReceivedCommandManager        ();

    bool                                  AddAnswer                     (XSTRING& originID, XSTRING& command, XSTRING& answer);
    DIOCLIPROTOCOLANSWER*                 GetFirstAnswer                (XCHAR* command);
    bool                                  DeleteFirstAnswer             (XCHAR* command);
    bool                                  DeleteAllAnswers              ();

    virtual void                          End                           ();

  protected:

    bool                                  AddCommand                    (XCHAR* command, int nparams);
    DIOCLIPROTOCOLCOMMAND*                GetCommand                    (XCHAR* command);
    bool                                  DeleteAllCommand              ();

    bool                                  ExtractParamsFromCommand      (XSTRING& stringreceived, XSTRING& command, XVECTOR<XSTRING*>& params);

    XSTRING                               ID; 
    bool                                  exitproccess;    

  private:

    void                                  Clean                         ();
                                        
    DIOSTREAM*                            diostream;

    bool                                  isini;
    bool                                  activeCRC;
    
    XTIMER*                               xtimerout;
    int                                   timeout;

    XVECTOR<DIOCLIPROTOCOLCOMMAND*>       commands;

    XMUTEX*                               xmutexanswers;
    XVECTOR<DIOCLIPROTOCOLANSWER*>        answers;

    XSTRING                               laststringreceived;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






