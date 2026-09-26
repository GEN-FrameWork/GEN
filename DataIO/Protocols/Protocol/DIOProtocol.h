/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol.h
* 
* @class      DIOPROTOCOL
* @brief      Data Input/Output Generic Binary Protocol class
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

#include "XBase.h"
#include "XDir.h"
#include "XPath.h"
#include "XEvent.h"
#include "XSubject.h"
#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#ifdef DIO_ALERTS_ACTIVE
#include "DIOAlerts.h"
#endif



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOPROTOCOL_CMDTYPE
{
  DIOPROTOCOL_CMDTYPE_NONE                        =   00 ,
  DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION          =   10 ,
  DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION       =   20 ,
  DIOPROTOCOL_CMDTYPE_GETAPPLICATIONNAME          =   25 ,
  DIOPROTOCOL_CMDTYPE_PING                        =   30 ,
  DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL    =   40 ,
  DIOPROTOCOL_CMDTYPE_GETDATETIME                 =   50 ,
  DIOPROTOCOL_CMDTYPE_SETDATETIME                 =   60 ,
  DIOPROTOCOL_CMDTYPE_INITFILEBLOCK               =   70 ,
  DIOPROTOCOL_CMDTYPE_GETFILEBLOCK                =   80 ,
  DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK               =   90 ,
  DIOPROTOCOL_CMDTYPE_DELETEINACTIVEFILEPROGRESS  =  100 ,
  DIOPROTOCOL_CMDTYPE_DELETEFILE                  =  110 ,
  DIOPROTOCOL_CMDTYPE_MAKEDIR                     =  120 ,
  DIOPROTOCOL_CMDTYPE_DELETEDIR                   =  130 ,
  DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT          =  140 ,
  DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT           =  150 ,
  DIOPROTOCOL_CMDTYPE_SENDALERT                   =  160 ,

  DIOPROTOCOL_CMDTYPE_END                         = 1000 ,
};


enum DIOPROTOCOL_DIRECTION
{
  DIOPROTOCOL_DIRECTION_UNKNOWN                 = 0 ,
  DIOPROTOCOL_DIRECTION_GET                         ,
  DIOPROTOCOL_DIRECTION_SEND                        ,
};


enum DIOPROTOCOL_RESULT
{
  DIOPROTOCOL_RESULT_OK                         = 0 ,
  DIOPROTOCOL_RESULT_UNKNOWNCMD                     ,
  DIOPROTOCOL_RESULT_NOTMEM                         ,
  DIOPROTOCOL_RESULT_NOTFILE                        ,
  DIOPROTOCOL_RESULT_NOTCONNEX                      ,
  DIOPROTOCOL_RESULT_ERRORCMD                       ,
};


enum DIOPROTOCOL_FILESTATUS
{
  DIOPROTOCOL_FILESTATUS_UNKNOWN                = 0 ,
  DIOPROTOCOL_FILESTATUS_REQUEST                    ,
  DIOPROTOCOL_FILESTATUS_WAIT                       ,
  DIOPROTOCOL_FILESTATUS_END                        ,
  DIOPROTOCOL_FILESTATUS_ERROR                      ,
};


enum DIOPROTOCOL_XEVENT_TYPE
{
  DIOPROTOCOL_XEVENT_TYPE_UNKNOWN         = XEVENT_TYPE_PROTOCOL ,
  DIOPROTOCOL_XEVENT_TYPE_COMMAND                               ,
  DIOPROTOCOL_XEVENT_TYPE_FILEBLOCK                             ,
  DIOPROTOCOL_XEVENT_TYPE_ADDALERT                              ,
};


#define DIOPROTOCOL_MSGIDHEAD                   0xAAA00555
#define DIOPROTOCOL_CMDTYPE_ANSWER              0x80000000
#define DIOPROTOCOL_CMDTYPE_ISSERVER            0x04000000
#define DIOPROTOCOL_CMDTYPE_SECUENCIALLIMIT     0x0000FFFF

#define DIOPROTOCOL_MAXWATCHDOG                 3
#define DIOPROTOCOL_TIMEOUT                     5
#define DIOPROTOCOL_SIZEOFHEAD                  (sizeof(XDWORD)*6)

#define DIOPROTOCOL_MAXBUFFER                   (DIOSTREAM_MAXBUFFER/2)

#define DIOPROTOCOL_TIMERPING                   60

#define DIOPROTOCOL_MAXFILEBLOCK                1024  //(DIOPROTOCOL_MAXBUFFER/2)

#define DIOPROTOCOL_TIMEFILEDELETE              0x00000100
#define DIOPROTOCOL_TIMEOUTFILEBLOCK            60

#define DIOPROTOCOL_MAXDESCRIPTIONSTR           64

#define DIOPROTOCOL_VERSION                     0
#define DIOPROTOCOL_SUBVERSION                  1
#define DIOPROTOCOL_SUBVERSIONERR               2

#define DIOPROTOCOL_ERRORRECEIVED              -1

class DIOPROTOCOL_COMMAND;
class DIOPROTOCOL;

typedef int (*DIOPROTOCOL_RECEIVEDFUNC)   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& buffer, XDWORD rID, XDWORD& param);

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XRAND;
class XDATETIME;
class XTIMER;
class XSYSTEM;
class XDIR;
class XDIRELEMENT;
class XFILE;
class XFACTORY;
class HASHCRC32;
class CIPHER;
class DIOPROTOCOL_XEVENT;
class DIOALERT;

class DIOPROTOCOL_ANSWER
{
  public:
                                              DIOPROTOCOL_ANSWER              (XDWORD type, XDWORD ID, XBUFFER& xbuffer);
                                             ~DIOPROTOCOL_ANSWER              ();

    XDWORD                                    GetType                         ();
    XDWORD                                    GetID                           ();
    XBUFFER&                                  GetBuffer                       ();

  private:

    void                                      Clean                           ();

    XDWORD                                    type;
    XDWORD                                    ID;
    XBUFFER                                   xbuffer;
};

class DIOPROTOCOL_COMMAND
{
  public:
                                              DIOPROTOCOL_COMMAND             (XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description);
                                             ~DIOPROTOCOL_COMMAND             ();

    XDWORD                                    GetType                         ();
    XCHAR*                                    GetINMask                       ();
    XCHAR*                                    GetOUTMask                      ();

    DIOPROTOCOL_RECEIVEDFUNC                  GetReceivedFunc                 ();
    XCHAR*                                    GetDescription                  ();

    XSTRING*                                  GetSINMask                      ();
    XSTRING*                                  GetSOUTMask                     ();

    XSTRING*                                  GetSDescription                 ();

  private:

    void                                      Clean                           ();

    XDWORD                                    type;

    XSTRING                                   inmask;
    XSTRING                                   outmask;
    DIOPROTOCOL_RECEIVEDFUNC                  receivedfunc;
    XSTRING                                   description;
};

class DIOPROTOCOL_FILE
{
  public:
                                              DIOPROTOCOL_FILE                ();
                                             ~DIOPROTOCOL_FILE                ();

    bool                                      IsActive                        ();
    void                                      SetActive                       (bool isactive);

    DIOPROTOCOL_DIRECTION                     GetDirection                    ();
    void                                      SetDirection                    (DIOPROTOCOL_DIRECTION direction);

    XPATH*                                    GetSourceName                   ();
    bool                                      SetSourceName                   (XCHAR* sourcename);
    bool                                      SetSourceName                   (XSTRING& sourcename);

    XPATH*                                    GetTargetName                   ();
    bool                                      SetTargetName                   (XCHAR* targetname);
    bool                                      SetTargetName                   (XSTRING& targetname);

    XQWORD                                    GetSize                         ();
    void                                      SetSize                         (XQWORD size);

    XDWORD                                    GetCRC32                        ();
    void                                      SetCRC32                        (XDWORD crc32);

    int                                       GetSizeBlock                    ();
    void                                      SetSizeBlock                    (XDWORD sizeblock);

    XQWORD                                    GetPosition                     ();
    void                                      SetPosition                     (XQWORD position);

    XBYTE                                     GetPercent                      ();
    void                                      SetPercent                      (XBYTE percent);

    bool                                      ReadBlock                       (XBYTE* buffer);
    bool                                      WriteBlock                      (XBYTE* buffer);

    bool                                      Erase                           ();
    void                                      Reset                           ();

  private:

    XFILE*                                    xfile;

    bool                                      isactive;
    DIOPROTOCOL_DIRECTION                     direction;

    XPATH                                     sourcename;
    XPATH                                     targetname;

    XQWORD                                    size;
    XDWORD                                    crc32;
    XDWORD                                    sizeblock;
    XQWORD                                    position;
    XBYTE                                     percent;

  private:

    void                                      Clean                           ();
};

class DIOPROTOCOL : public XSUBJECT
{
  public:
                                              DIOPROTOCOL                     (DIOSTREAM* diostream);
     virtual                                 ~DIOPROTOCOL                     ();

     XSTRING*                                 GetDescription                  ();

     bool                                     Connect                         ();
     bool                                     Disconnect                      ();

     bool                                     Reset                           ();

     bool                                     IsConnected                     ();
     bool                                     IsConnecting                    ();
     bool                                     IsDisconnected                  ();

     bool                                     IsInitialized                   ();
     void                                     SetIsInitialized                (bool isinitialized);

     bool                                     IsServerInitialized             ();

     bool                                     IsOperational                   ();

     XDWORD                                   GetMaskID                       ();
     bool                                     SetMaskID                       (XDWORD maskID);

     void*                                    GetConnectionsManager           ();
     void                                     SetConnectionsManager           (void* connectionsmanager);

     void*                                    GetConnection                   ();
     void                                     SetConnection                   (void* connection);

     void*                                    GetApplicationData              ();
     void                                     SetApplicationData              (void* applicationdata);

     bool                                     IsOwnApplicationData            ();
     void                                     SetIsOwnApplicationData         (bool isownapplicationdata);

     DIOSTREAM*                               GetDIOStream                    ();

     XDATETIME*                               GetDateTime                     ();

     bool                                     AddCommand                      (XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description);
     DIOPROTOCOL_COMMAND*                     GetCommand                      (XDWORD type);

     DIOPROTOCOL_ANSWER*                      AddAnswer                       (XDWORD type, XDWORD ID, XBUFFER& xbuffer);
     DIOPROTOCOL_ANSWER*                      GetAnswer                       (XDWORD type, XDWORD ID);
     bool                                     DeleteAnswer                    (XDWORD type, XDWORD ID);
     bool                                     DeleteAllAnswers                ();

     DIOPROTOCOL_ANSWER*                      AddAsk                          (XDWORD type, XDWORD ID, XBUFFER& xbuffer);
     DIOPROTOCOL_ANSWER*                      GetAsk                          (XDWORD index);
     bool                                     DeleteAsk                       (XDWORD index);
     bool                                     DeleteAllAsks                   ();

     int                                      ReadCommand                     (XDWORD& type, XDWORD& ID, XBUFFER& xbuffer);
     DIOPROTOCOL_RESULT                       Received                        ();
     DIOPROTOCOL_RESULT                       ReceivedHandle                  (XDWORD type, XDWORD ID, XBUFFER& xbuffer);

     bool                                     SendCommand                     (XDWORD type, XDWORD& ID, XBUFFER& xbuffer);
     bool                                     Command                         (XDWORD type, XBUFFER& xbuffer, int timeout = DIOPROTOCOL_TIMEOUT);

     bool                                     SendEvent                       (DIOPROTOCOL_XEVENT_TYPE type, DIOPROTOCOL* protocol);
     bool                                     SendEventCommand                (DIOPROTOCOL* protocol, DIOPROTOCOL_DIRECTION direction, XDWORD type, bool result, void* param1, XDWORD param2);
     bool                                     SendEventFileBlock              (DIOPROTOCOL_FILESTATUS filestatus, bool fileissend, XPATH* filesource, XPATH* filetarget, int filepercent, void* param);

     void                                     GetProtocolVersion              (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr);
     void                                     SetProtocolVersion              (XDWORD version, XDWORD subversion, XDWORD subversionerr);

     void                                     Application_GetVersion           (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr);
     void                                     Application_SetVersion           (XDWORD version, XDWORD subversion, XDWORD subversionerr);

     XSTRING*                                 Application_GetName              ();

     bool                                     SetFileProgress                 (DIOPROTOCOL_DIRECTION direction, XPATH& sourcename, XPATH& targetname, XDWORD CRC32);
     DIOPROTOCOL_FILE*                        GetFileProgress                 ();

     XPATH*                                   GetPathSearch                   ();
     XDIRELEMENT*                             GetSearchElement                ();
     XDIR*                                    GetXDir                         ();

     bool                                     CMD_GetProtocolVersion          (XWORD& version, XWORD& subversion, XWORD& subversionerr);
     static int                               RCV_GetProtocolVersion          (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_Application_GetVersion       (XWORD& version, XWORD& subversion, XWORD& subversionerr);
     static int                               RCV_Application_GetVersion       (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_Application_GetName          (XSTRING& applicationname);
     static int                               RCV_Application_GetName          (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_Ping                        (XDWORD& milliseconds, int timeout = DIOPROTOCOL_TIMEOUT);
     static int                               RCV_Ping                        (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SendIsInitServerProtocol    (bool isinit);
     static int                               RCV_SendIsInitServerProtocol    (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetDateTime                 (XSTRING& datestring);
     static int                               RCV_GetDateTime                 (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SetDateTime                 (XCHAR* datestring);
     static int                               RCV_SetDateTime                 (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_File                        (DIOPROTOCOL_DIRECTION direction, XCHAR* sourcename, XCHAR* targetname, int timeout = DIOPROTOCOL_TIMEOUTFILEBLOCK, void* param = NULL);

     bool                                     CMD_DeleteFile                  (XCHAR* namefile, bool overwrite);
     static int                               RCV_DeleteFile                  (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_MakeDir                     (XCHAR* namedir);
     static int                               RCV_MakeDir                     (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_DeleteDir                   (XCHAR* namedir, bool all);
     static int                               RCV_DeleteDir                   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetDirElement               (XCHAR* pathsearch, XCHAR* mask, XPATH& namefile, XBYTE& type, int& size, XDWORD& CRC32);
     static int                               RCV_GetDirElement               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     #ifdef DIO_ALERTS_ACTIVE
     bool                                     CMD_SendAlert                   (DIOALERT* alert);
     static int                               RCV_SendAlert                   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);
     #endif

  protected:

     DIOSTREAM*                               diostream;
     XSTRING                                  description;

     XWORD                                    protocolversion;
     XWORD                                    protocolsubversion;
     XWORD                                    protocolsubversionerr;

     XWORD                                    applicationversion;
     XWORD                                    applicationsubversion;
     XWORD                                    applicationsubversionerr;

     XSTRING                                  applicationname;

     bool                                     isinitialized;
     bool                                     isserverinitialized;

     XDATETIME*                               xdatetime;
     XTIMER*                                  xtimer;
     XTIMER*                                  watchdog;

     void*                                    connectionsmanager;
     void*                                    connection;
     void*                                    applicationdata;
     bool                                     isownapplicationdata;

     XVECTOR<DIOPROTOCOL_COMMAND*>            commands;
     XVECTOR<DIOPROTOCOL_ANSWER*>             answers;
     XVECTOR<DIOPROTOCOL_ANSWER*>             asks;

     XDWORD                                   maskID;

     DIOPROTOCOL_FILE*                        fileprogress;

  private:

     void                                     Clean                           ();

     bool                                     DeleteAllCommands               ();

     XDWORD                                   CRC32OfFile                     (XPATH& xpath);

     bool                                     CMD_InitFileBlock               (DIOPROTOCOL_FILE* fp);
     static int                               RCV_InitFileBlock               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetFileBlock                (DIOPROTOCOL_FILE* fp);
     static int                               RCV_GetFileBlock                (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SendFileBlock               (DIOPROTOCOL_FILE* fp);
     static int                               RCV_SendFileBlock               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     static void                              ThreadReceived                  (void* param);
     static void                              ThreadAsks                      (void* param);

     XDWORD                                   IDmsgcounter;
     XTIMER*                                  timerfileprogress;

     XPATH                                    pathsearch;
     XDIR*                                    xdir;
     XDIRELEMENT                              searchelement;

     bool                                     doexit;

     XMUTEX*                                  xmutexanswers;
     XMUTEX*                                  xmutexasks;

     XMUTEX*                                  xmutexcommands;
     XMUTEX*                                  xmutexreads;
     XMUTEX*                                  xmutexwrites;
     XMUTEX*                                  xmutexfilesprogress;

     XTHREADCOLLECTED*                        xthreadreceived;
     XTHREADCOLLECTED*                        xthreadasks;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





