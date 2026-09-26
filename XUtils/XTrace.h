/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTrace.h
* 
* @class      XTRACE
* @brief      eXtended Utils Trace class 
* @ingroup    XUTILS
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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef BUILDER
#include <cstdarg>
#endif

#include "Common_DefaultXTraceCFG.h"

#include "XBase.h"
#include "XTimer.h"
#include "XDateTime.h"
#include "XString.h"
#include "XBuffer.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class XTRACE;


enum XTRACE_TYPE
{
  XTRACE_TYPE_NONE                   = 0  ,
  XTRACE_TYPE_SPECIAL                     ,
  XTRACE_TYPE_FILE                        ,
  XTRACE_TYPE_NET                         ,
};


enum XTRACE_TYPE_STATUS_MSG
{
  XTRACE_TYPE_STATUS_MSG_UNKNOWN     = 0  ,  
  XTRACE_TYPE_STATUS_MSG_BOOLEAN          , 
  XTRACE_TYPE_STATUS_MSG_INTEGER          ,
  XTRACE_TYPE_STATUS_MSG_STRING           ,  
  XTRACE_TYPE_STATUS_MSG_XDWORD           ,  
  XTRACE_TYPE_STATUS_MSG_FLOAT            ,  
  XTRACE_TYPE_STATUS_MSG_RGBCOLOR         ,  
};


#define XTRACE_LEVEL_WITHCOLOR              0x80
#define XTRACE_LEVEL_WITHTAB                0x40
#define XTRACE_LEVEL_WITHCODE               0x20
#define XTRACE_LEVEL_STATUS                 0x10

#define XTRACE_LEVEL_COLOR(level)           (level|XTRACE_LEVEL_WITHCOLOR)
#define XTRACE_LEVEL_TAB(level)             (level|XTRACE_LEVEL_WITHTAB)
#define XTRACE_LEVEL_CODE(level)            (level|XTRACE_LEVEL_WITHCODE)

#ifdef MICROCONTROLLER
  #define XTRACE_SIZELIMITDEFAULT           512       // Bytes
  #define XTRACE_MAXNTARGETS                1
  #define XTRACE_MAXSIZEDATABLOCK           512
  #define XTRACE_MAXNETBUFFER               512
#else
  #define XTRACE_SIZELIMITDEFAULT           64*1024   // Bytes
  #define XTRACE_MINNTARGETS                4
  #define XTRACE_MAXNTARGETS                6
  #define XTRACE_MAXSIZEDATABLOCK           10240
  #define XTRACE_MAXNETBUFFER               512
#endif

#define XTRACE_DEFAULT_SPECIALAIM           __L("eXtended Trace Monitor")

#define XTRACE_LIMITCTRLNETRESURCES         25

#define XTRACE_MAXAPPLICATIONNAMESIZE       20
#define XTRACE_MAXAPPLICATIONVERSIONSIZE    10
#define XTRACE_MAXAPPLICATIONIDSIZE         36

#define XTRACE_IDPACKET                     0xAADEB055
#define XTRACE_IDMSGCLEARSCREEN             __L("#[CLEAR_SCREEN]# :")    
#define XTRACE_IDMSGSTATUS                  __L("#[CHANGE_STATUS]# :")    
#define XTRACE_IDMSGSTATUSCLEAR             __L("#[CLEAR_STATUS]# :")    
#define XTRACE_IDMSGSTATUS_BOOLEAN          __C('B')    
#define XTRACE_IDMSGSTATUS_INTEGER          __C('I')    
#define XTRACE_IDMSGSTATUS_STRING           __C('S')    
#define XTRACE_IDMSGSTATUS_DWORD            __C('D')    
#define XTRACE_IDMSGSTATUS_FLOAT            __C('F')    
#define XTRACE_IDMSGSTATUS_COLOR            __C('C')    


#define XTRACE_MAXCACHEREMOTEUDPIP          100

#define XTRACE_COLOR_BLACK                  0
#define XTRACE_COLOR_BLUE                   1
#define XTRACE_COLOR_GREEN                  2
#define XTRACE_COLOR_PURPLE                 3
#define XTRACE_COLOR_RED                    4
#define XTRACE_COLOR_GRAY                   5

#define XTRACE_COLOR_CHECKERROR(STATUS)     (STATUS?XTRACE_COLOR_BLUE:XTRACE_COLOR_RED)

#define XTRACE_COLOR_INFO                   XTRACE_COLOR_BLACK
#define XTRACE_COLOR_WARNING                XTRACE_COLOR_PURPLE
#define XTRACE_COLOR_ERROR                  XTRACE_COLOR_RED



#define XTRACE_CRC16(ptrbuffer, buffersize, crc) { XBYTE*   buffer = ptrbuffer;                                                  \
                                                   XDWORD   size   = buffersize;                                                 \
                                                   XBYTE    x;                                                                   \
                                                   crc = 0xFFFF;                                                                 \
                                                   while(size--)                                                                 \
                                                    {                                                                            \
                                                      x = crc >> 8 ^ * buffer++;                                                 \
                                                      x ^= x>>4;                                                                 \
                                                      crc = (crc << 8) ^ ((XWORD)(x << 12)) ^ ((XWORD)(x <<5)) ^ ((XWORD)x);     \
                                                    }                                                                            \
                                                 }


#ifdef XTRACE_ACTIVE

  #define XTRACE_SETTARGET(index, type , aim)                                                             XTRACE::instance->SetTarget(index, type , aim)
  #define XTRACE_ADDTARGET(type , aim)                                                                    XTRACE::instance->AddTarget(type, aim)
  #define XTRACE_DEACTIVATEALLTARGETS()                                                                   XTRACE::instance->DeactivateAllTargets()
  #define XTRACE_SETSIZELIMIT(sizelimit)                                                                  XTRACE::instance->SetSizeLimit(sizelimit)
  #define XTRACE_SETAPPLICATIONNAME(name)                                                                 XTRACE::instance->Application_SetName(name)
  #define XTRACE_SETAPPLICATIONVERSION(version, subversion, subversionerr)                                XTRACE::instance->Application_SetVersion(version, subversion, subversionerr)
  #define XTRACE_SETAPPLICATIONID(ID)                                                                     XTRACE::instance->Application_SetID(ID)
  #define XTRACE_SETSCRAPERWEBSCRIPURLDOWNLOAD(URL)                                                       XTRACE::instance->SetScraperWebScriptURLDownload(URL)
  #define XTRACE_SETPUBLICIP(IP)                                                                          XTRACE::instance->SetPublicIPString(IP)
  #define XTRACE_SETLOCALIPFILTER(LOCALIPFILTER)                                                          XTRACE::instance->SetLocalIPFilter(LOCALIPFILTER)
  #define XTRACE_SETLOCALIP(IP)                                                                           XTRACE::instance->SetLocalIPString(IP)
  #define XTRACE_RESOLVEALLRESOURCES                                                                      XTRACE::instance->ResolveAllResources()
  #define XTRACE_PRINTHEADER(level, header)                                                               XTRACE::instance->PrintHeader(level, header)

  #define XTRACE_PRINTMSGSTATUS(name, value)                                                              XTRACE::instance->PrintMsgStatus(0, name, value)
  
  #define XTRACE_CLEARSCREEN                                                                              XTRACE::instance->ClearScreen(0)
  #define XTRACE_CLEARALLSCREENS                                                                          XTRACE::instance->ClearScreen(1)
  #define XTRACE_CLEARMSGSSTATUS                                                                          XTRACE::instance->ClearMsgsStatus(0)
  #define XTRACE_CLEARALLMSGSSTATUS                                                                       XTRACE::instance->ClearMsgsStatus(1)

  #ifdef BUILDER

    // Builder C ++ does not allow variable parameters in macros, a parameter must always be present in these macros. (only in Builder C ++ because it is not C99)

    #define XTRACE_PRINT(mask, params)                                                                    XTRACE::instance->Print(0                                        , mask ,params)
    #define XTRACE_PRINTLEVEL(level, mask, params)                                                        XTRACE::instance->Print(level                                    , mask ,params)
    #define XTRACE_PRINTCOLOR(level, mask, params)                                                        XTRACE::instance->Print((level|XTRACE_LEVEL_WITHCOLOR)           , mask ,params)
    #define XTRACE_PRINTTAB(level, mask, params)                                                          XTRACE::instance->Print((level|XTRACE_LEVEL_WITHTAB)             , mask ,params)
    #define XTRACE_PRINTCODE(level, mask, params)                                                         XTRACE::instance->Print((level|XTRACE_LEVEL_WITHCODE)            , mask ,params)

    #define XTRACE_PRINTDATABLOCK(data, params)                                                           XTRACE::instance->PrintDataBlock(0                               , data ,params)
    #define XTRACE_PRINTDATABLOCKLEVEL(level, data, params)                                               XTRACE::instance->PrintDataBlock(level                           , data ,params)
    #define XTRACE_PRINTDATABLOCKCOLOR(level, data, params)                                               XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHCOLOR)  , data ,params)
    #define XTRACE_PRINTDATABLOCKTAB(level, data, params)                                                 XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHTAB)    , data ,params)
    #define XTRACE_PRINTDATABLOCKCODE(level, data, params)                                                XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHCODE)   , data ,params)

  #else
    
    #define XTRACE_PRINT(mask, ...)                                                                       XTRACE::instance->Print(XTRACE_LEVEL_STATUS                      , mask , ## __VA_ARGS__)
    #define XTRACE_PRINTLEVEL(level, mask, ...)                                                           XTRACE::instance->Print(level                                    , mask , ## __VA_ARGS__)
    #define XTRACE_PRINTCOLOR(level, mask, ...)                                                           XTRACE::instance->Print((level|XTRACE_LEVEL_WITHCOLOR)           , mask , ## __VA_ARGS__)
    #define XTRACE_PRINTTAB(level, mask, ...)                                                             XTRACE::instance->Print((level|XTRACE_LEVEL_WITHTAB)             , mask , ## __VA_ARGS__)
    #define XTRACE_PRINTCODE(level, mask, ...)                                                            XTRACE::instance->Print((level|XTRACE_LEVEL_WITHCODE)            , mask , ## __VA_ARGS__)

    #define XTRACE_PRINTDATABLOCK(data, ...)                                                              XTRACE::instance->PrintDataBlock(0                               , data , ## __VA_ARGS__)
    #define XTRACE_PRINTDATABLOCKLEVEL(level, data, ...)                                                  XTRACE::instance->PrintDataBlock(level                           , data , ## __VA_ARGS__)
    #define XTRACE_PRINTDATABLOCKCOLOR(level, data, ...)                                                  XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHCOLOR)  , data , ## __VA_ARGS__)
    #define XTRACE_PRINTDATABLOCKTAB(level, data, ...)                                                    XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHTAB)    , data , ## __VA_ARGS__)
    #define XTRACE_PRINTDATABLOCKCODE(level, data, ...)                                                   XTRACE::instance->PrintDataBlock((level|XTRACE_LEVEL_WITHCODE)   , data , ## __VA_ARGS__)

  #endif

#else

  #define XTRACE_SETTARGET(index, type , aim)                                                             true
  #define XTRACE_ADDTARGET(type, aim)                                                       
  #define XTRACE_DEACTIVATEALLTARGETS()                                                                   true
  #define XTRACE_SETSIZELIMIT(sizelimit)
  #define XTRACE_SETAPPLICATIONNAME(name)
  #define XTRACE_SETAPPLICATIONVERSION(version, subversion, subversionerr)
  #define XTRACE_SETAPPLICATIONID(ID)
  #define XTRACE_SETSCRAPERWEBSCRIPURLDOWNLOAD(URL)
  #define XTRACE_SETPUBLICIP(IP)
  #define XTRACE_SETLOCALIPFILTER(LOCALIPFILTER)
  #define XTRACE_SETLOCALIP(IP)
  #define XTRACE_RESOLVEALLRESOURCES
  #define XTRACE_PRINTHEADER(header)
  
  #define XTRACE_CLEARSCREEN  
  #define XTRACE_CLEARALLSCREENS                                                                              
  #define XTRACE_CLEARMSGSSTATUS
  #define XTRACE_CLEARALLMSGSSTATUS
  
  #ifdef BUILDER

    #define XTRACE_PRINT
    #define XTRACE_PRINTLEVEL
    #define XTRACE_PRINTCOLOR
    #define XTRACE_PRINTTAB
    #define XTRACE_PRINTCODE

    #define XTRACE_PRINTDATABLOCK
    #define XTRACE_PRINTDATABLOCKLEVEL
    #define XTRACE_PRINTDATABLOCKCOLOR
    #define XTRACE_PRINTDATABLOCKTAB
    #define XTRACE_PRINTDATABLOCKCODE

    #define XTRACE_PRINTMSGSTATUS
    
  #else

    #define XTRACE_PRINT(level, mask, ...)
    #define XTRACE_PRINTLEVEL(level, mask, ...)
    #define XTRACE_PRINTCOLOR(level, mask, ...)  {}
    #define XTRACE_PRINTTAB(level, mask, ...)
    #define XTRACE_PRINTCODE(level, mask, ...)

    #define XTRACE_PRINTDATABLOCK(data, ...)
    #define XTRACE_PRINTDATABLOCKLEVEL(level, data, ...)
    #define XTRACE_PRINTDATABLOCKCOLOR(level, data, ...)
    #define XTRACE_PRINTDATABLOCKTAB(level, data, ...)
    #define XTRACE_PRINTDATABLOCKCODE(level, data, ...)

    #define XTRACE_PRINTMSGSTATUS(name, value)
    #define XTRACE_PRINTMSGSTATUSCOLOR(level, name, value)
    #define XTRACE_PRINTMSGSTATUSTAB(level, name, value)
    #define XTRACE_PRINTMSGSTATUSCODE(level, name, value)

  #endif

#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;
class DIOSTREAM;

class XTRACE_TARGET
{
  public:
                                    XTRACE_TARGET                     ();
    virtual                        ~XTRACE_TARGET                     ();

    XTRACE_TYPE                     GetType                           ();
    void                            SetType                           (XTRACE_TYPE type);

    XCHAR*                          GetAim                            ();
    bool                            GetAim                            (XSTRING& aim);

    void                            SetAim                            (XSTRING& aim);
    void                            SetAim                            (XCHAR* aim);

    XDWORD                          GetNSendings                      ();
    void                            AddNSendings                      ();

    #if (defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))
    
    XWORD                           GetPort                           ();
    void                            SetPort                           (XWORD port);

    bool                            IPTarget                  ();

    char*                           GetIP                             ();

    XQWORD                          GetNETHandle                      ();
    void                            SetNETHandle                      (XQWORD NEThandle);

    #endif 

    void                            Clean                             ();

  private:

    XTRACE_TYPE                     type;
    XCHAR                           aim[_MAXSTR];
    XDWORD                          nsendings;

    #if (defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

    XWORD                           port;
    char                            IP[_MAXSTR];
    XQWORD                          NEThandle;

    #endif
};

class XTRACE_STATUS_MSG
{
  public:
                                    XTRACE_STATUS_MSG                  ();
    virtual                        ~XTRACE_STATUS_MSG                  ();

    XTRACE_TYPE_STATUS_MSG          GetType                            ();
    void                            SetType                            (XTRACE_TYPE_STATUS_MSG type);

    XSTRING*                        GetName                            ();

    XTIMER*                         GetXTimerLastUpdate                ();
    
    bool                            Value_GetBoolean                   ();
    void                            Value_SetBoolean                   (bool value);

    int                             Value_GetInteger                   ();
    void                            Value_SetInteger                   (int value);

    XSTRING*                        Value_GetString                    ();

    XDWORD                          Value_GetDword                     ();
    void                            Value_SetDword                     (XDWORD value);

    float                           Value_GetFloat                     ();
    void                            Value_SetFloat                     (float value);

    XBYTE*                          Value_GetColor                     ();        
    void                            Value_SetColor                     (XBYTE* value);        

  private:

    void                            Clean                              ();

    XTRACE_TYPE_STATUS_MSG          type;

    XSTRING                         name; 

    XTIMER*                         xtimerlastupdate;

    bool                            value_boolean;
    int                             value_integer;
    XSTRING                         value_string;
    XDWORD                          value_dword;
    float                           value_float;
    XBYTE                           value_color[3];        
};

class XTRACE_STATUS_MSGS
{
  public:
                                    XTRACE_STATUS_MSGS                ();
    virtual                        ~XTRACE_STATUS_MSGS                ();

    XTRACE_STATUS_MSG*              StatusMsg_Add                     (XSTRING* line);    
    XTRACE_STATUS_MSG*              StatusMsg_Get                     (XCHAR* name);
    XTRACE_STATUS_MSG*              StatusMsg_Get                     (int index);
    XVECTOR<XTRACE_STATUS_MSG*>*    StatusMsg_GetAll                  ();
    bool                            StatusMsg_DeleteAll               ();

  private:

    void                            Clean                             ();
    
    XVECTOR<XTRACE_STATUS_MSG*>     statusmsgs;
};

class XTRACE
{
  public:
                                    XTRACE                            ();
    virtual                        ~XTRACE                            ();

    bool                            SetTarget                         (int index, XTRACE_TYPE type , XCHAR* aim);
    bool                            AddTarget                         (XTRACE_TYPE type , XCHAR* aim);
    bool                            DeactivateAllTargets              ();

    XDWORD                          GetSizeLimit                      ();
    bool                            SetSizeLimit                      (XDWORD sizelimit = XTRACE_SIZELIMITDEFAULT);

    XCHAR*                          Application_GetName               ();
    void                            Application_SetName               (XCHAR* applicationname);
    void                            Application_SetName               (XCHAR* applicationname, XDWORD size);
    void                            Application_SetName               (XSTRING& applicationname);

    void                            Application_GetVersion            (int& applicationversion, int& applicationsubversion, int& applicationsubversionerr);
    void                            Application_SetVersion            (int applicationversion, int applicationsubversion, int applicationsubversionerr);

    XCHAR*                          Application_GetID                 ();
    void                            Application_SetID                 (XCHAR* applicationID, XDWORD size);
    void                            Application_SetID                 (XSTRING& applicationID);   

    bool                            ClearScreen                       (XBYTE level);
    void                            PrintHeader                       (XBYTE level, XCHAR* header);
    bool                            Print                             (XBYTE level, XCHAR* mask, ...);
    bool                            PrintDataBlock                    (XBYTE level, XBYTE* data, XDWORD _size, XDWORD marginsize = 0, XDWORD sizeline = 32, bool showoffset = true, bool showtext = true);
    bool                            PrintDataBlock                    (XBYTE level, XBUFFER& data, XDWORD marginsize = 0, XDWORD sizeline = 32, bool showoffset = true, bool showtext = true);
    
    bool                            ClearMsgsStatus                   (XBYTE level);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, bool value);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, int value);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, XCHAR* value);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, XDWORD value);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, float value);
    bool                            PrintMsgStatus                    (XBYTE level, XCHAR* name, XBYTE value[3]);

    virtual void                    PrintSpecial                      (XTRACE_TARGET* target, XBYTE level, XCHAR* string);
    virtual void                    PrintFile                         (XTRACE_TARGET* target, XBYTE level, XCHAR* string);
    virtual void                    PrintNet                          (XTRACE_TARGET* target, XBYTE level, XCHAR* string);

    virtual bool                    GetHandleNet                      (XTRACE_TARGET* target);
    virtual bool                    CloseHandleNet                    (XTRACE_TARGET* target);

    XDWORD                          GetTraceFromXBuffer               (XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data);
    XDWORD                          GetTraceFromDIOStream             (DIOSTREAM* DIOstream, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data);
    bool                            SetTraceDataToText                (XBUFFER& data, XSTRING& string);

    bool                            SetTraceToXBuffer                 (XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XDWORD& sizepacket, XBUFFER& xbufferpacket);
    bool                            SetTraceTextToXBuffer             (XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket);

    #ifndef XTRACE_NOINTERNET

    bool                            ObtainPublicIP                    ();
    XDWORD                          GetPublicIP                       ();
    bool                            SetPublicIPString                 (XCHAR* publicIPstring);

    #endif

    void                            SetLocalIPFilter                  (XBYTE localIPfilter);
    bool                            ObtainLocalIP                     ();
    XDWORD                          GetLocalIP                        ();
    bool                            SetLocalIPString                  (XCHAR* localIPstring);

    bool                            ObtainResourcesIP                 ();
    bool                            ResolveAllNetTargets              (bool& changed);
    bool                            ReConnectedAllNetTargets          ();
    bool                            ResolveAllResources               ();

    static XTRACE*                  instance;

  protected:

    bool                            GenerateTab                       (int ntab, XSTRING& tab);

    XTRACE_TARGET                   targets[XTRACE_MAXNTARGETS];

    XDWORD                          sizelimit;

    XCHAR                           applicationname[_MAXSTR];
    int                             applicationversion;
    int                             applicationsubversion;
    int                             applicationsubversionerr;
    XCHAR                           applicationID[_MAXSTR];

    #ifndef XTRACE_NOINTERNET
    XDWORD                          publicIP;
    #endif

    XBYTE                           localIPfilter;
    XDWORD                          localIP;
    
    bool                            openheader;
    XDWORD                          sequence;

  private:

    void                            Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





