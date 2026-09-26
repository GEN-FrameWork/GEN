/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOOBEXPush.cpp
* 
* @class      DIOOBEXPUSH
* @brief      Data Input/Output Obex Push protocol class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOOBEXPush.h"

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XFile.h"
#include "XPath.h"
#include "XBuffer.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XTrace.h"

#include "HashCRC32.h"

#include "DIOStreamBluetooth.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSHFILE::DIOOBEXPUSHFILE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSHFILE::DIOOBEXPUSHFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSHFILE::~DIOOBEXPUSHFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSHFILE::~DIOOBEXPUSHFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOOBEXPUSHFILE::GetID()
* @brief      Get ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOOBEXPUSHFILE::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOOBEXPUSHFILE::GetXPath()
* @brief      Get X path
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOOBEXPUSHFILE::GetXPath()
{
  return &xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOOBEXPUSHFILE::GetNameSend()
* @brief      Get name send
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOOBEXPUSHFILE::GetNameSend()
{
  return &namesend;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::IsSend()
* @brief      Is send
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::IsSend()
{
  return sendis;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOOBEXPUSHFILE::GetSendPercent()
* @brief      Get send percent
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOOBEXPUSHFILE::GetSendPercent()
{
  return sendpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOOBEXPUSHFILE::GetSendTimeElapsed()
* @brief      Get send time elapsed
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOOBEXPUSHFILE::GetSendTimeElapsed()
{
  return sendtimeelapsed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSHERROR DIOOBEXPUSHFILE::GetError()
* @brief      Get error
* @ingroup    DATAIO
* 
* @return     DIOOBEXPUSHERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSHERROR DIOOBEXPUSHFILE::GetError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::SetID(XDWORD ID)
* @brief      Set ID
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetID(XDWORD ID)
{
  this->ID = ID;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::SetIsSend(bool issend)
* @brief      Set is send
* @ingroup    DATAIO
* 
* @param[in]  issend : Issend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetIsSend(bool issend)
{
  this->sendis = issend;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::SetSendPercent(XBYTE percent)
* @brief      Set send percent
* @ingroup    DATAIO
* 
* @param[in]  percent : Percent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetSendPercent(XBYTE percent)
{
  sendpercent = percent;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::SetSendTimeElapsed(XQWORD sendtimeelapsed)
* @brief      Set send time elapsed
* @ingroup    DATAIO
* 
* @param[in]  sendtimeelapsed : Sendtimeelapsed value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetSendTimeElapsed(XQWORD sendtimeelapsed)
{
  this->sendtimeelapsed = sendtimeelapsed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSHFILE::SetError(DIOOBEXPUSHERROR error)
* @brief      Set error
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetError(DIOOBEXPUSHERROR error)
{
  this->error = error;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOOBEXPUSHFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOOBEXPUSHFILE::Clean()
{
  ID               = 0;
  sendis           = false;
  sendpercent      = 0;
  sendtimeelapsed  = 0;
  error            = DIOOBEXPUSHERROR_NONE;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSH::DIOOBEXPUSH(DIOSTREAM* diostream, bool inlittleendian) : XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @param[in]  inlittleendian : Little-endian mode flag.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSH::DIOOBEXPUSH(DIOSTREAM* diostream, bool inlittleendian) : XFSMACHINE(0)
{
  Clean();

  this->diostream       = diostream;
  this->inlittleendian  = inlittleendian;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_NONE            ,
                DIOOBEXPUSH_XFSMEVENT_CONNECT         , DIOOBEXPUSH_XFSMSTATE_CONNECT       ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_CONNECT         ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_INIPUSH         ,
                DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH    , DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH  ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH    ,
                DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH    , DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH  ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH    ,
                DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH   ,
                DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH   ,
                DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_ENDPUSH         ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                XFSMACHINESTATE_EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_DISCONNECT      ,
                DIOOBEXPUSH_XFSMEVENT_NONE            , DIOOBEXPUSH_XFSMSTATE_NONE          ,
                XFSMACHINESTATE_EVENTDEFEND)) return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSH::~DIOOBEXPUSH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSH::~DIOOBEXPUSH()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::Ini(int timeout)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::Ini(int timeout)
{
  if(!diostream)               return false;
  if(!diostream->GetConfig()) return false;

  sendbuffer = GEN_NEW XBUFFER();
  xfile=GEN_XFACTORY.Create_File();
  xtimer=GEN_XFACTORY.CreateTimer();
 
  issending  = false;

  if(xfile &&  sendbuffer && xtimer)
    {
      if(diostream->Open())
        {
          if(diostream->WaitToConnected(timeout))
            {
              xthreadobex = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("DIOOBEXPUSH::Ini"), ThreadSend, (void*)this);
              if(xthreadobex)
                {
                  xthreadobex->Ini(false);
                  return true;
                }
            }

          diostream->Close();
        }
    }

  GEN_DELETE sendbuffer;

  GEN_XFACTORY.Delete_File(xfile);
  GEN_XFACTORY.DeleteTimer(xtimer);

  sendbuffer = NULL;
  xfile      = NULL;
  xtimer     = NULL;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::IsSending()
* @brief      Is sending
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::IsSending()
{
  if(sendfilelist.IsEmpty()) return false;

  if(!issending) xthreadobex->Run(false);

  return issending;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::SetIsSending(bool issending)
* @brief      Set is sending
* @ingroup    DATAIO
* 
* @param[in]  issending : Issending value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::SetIsSending(bool issending)
{
  this->issending = issending;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::SendFile(XPATH& xpath, XSTRING& namesend, XDWORD ID, int timeoutask)
* @brief      Send file
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* @param[in]  namesend : Namesend value.
* @param[in]  ID : Identifier to use.
* @param[in]  timeoutask : Timeoutask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFile(XPATH& xpath, XSTRING& namesend, XDWORD ID, int timeoutask)
{
  SendFileList_DeleteAll();
  if(!SendFileList_AddFile(xpath, namesend, ID)) return false;

  return SendFiles(timeoutask);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::SendFiles(int asktimeout)
* @brief      Send files
* @ingroup    DATAIO
* 
* @param[in]  asktimeout : Asktimeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFiles(int asktimeout)
{
  if(!sendfilelist.GetSize()) return false;

  xtimer->Reset();

  this->asktimeout =  asktimeout;

  asktimeelapsed   = 0;
  issending        = true;
  indexfilelist    = 0;

  SetEvent(sendisstart?DIOOBEXPUSH_XFSMSTATE_INIPUSH:DIOOBEXPUSH_XFSMSTATE_CONNECT);
  xthreadobex->Run(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOOBEXPUSH::GetXTimer()
* @brief      Get X timer
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOOBEXPUSH::GetXTimer()
{
  return xtimer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOOBEXPUSH::GetAskTimeElapsed()
* @brief      Get ask time elapsed
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOOBEXPUSH::GetAskTimeElapsed()
{
  return asktimeelapsed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::SendFileList_AddFile(XPATH& xpath, XSTRING& namesend, XDWORD ID)
* @brief      Send file list add file
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* @param[in]  namesend : Namesend value.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFileList_AddFile(XPATH& xpath, XSTRING& namesend, XDWORD ID)
{
  DIOOBEXPUSHFILE* obexpushfile = GEN_NEW DIOOBEXPUSHFILE();
  if(!obexpushfile) return false;

  obexpushfile->GetXPath()->Set(xpath);
  obexpushfile->GetNameSend()->Set(namesend);

  XDWORD _ID = GenerateFileID(xpath);
  if(!_ID)
    {
      GEN_DELETE obexpushfile;
      return false;
    }

  if(!ID)
     {
       obexpushfile->SetID(_ID);
     }
    else
     {
        if(ID == _ID)
          {
            obexpushfile->SetID(ID);
          }
         else
          {
            GEN_DELETE obexpushfile;
            return false;
          }
     }

  obexpushfile->SetError(DIOOBEXPUSHERROR_NONE);

  sendfilelist.Add(obexpushfile);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetFile(int index)
* @brief      Send file list get file
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOOBEXPUSHFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetFile(int index)
{
  if(sendfilelist.IsEmpty())                    return NULL;
  if((indexfilelist>=(int)sendfilelist.GetSize())) return NULL;

  return (DIOOBEXPUSHFILE*)sendfilelist.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetActualFile()
* @brief      Send file list get actual file
* @ingroup    DATAIO
* 
* @return     DIOOBEXPUSHFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetActualFile()
{
  int index = SendFileList_GetIndex();
  if(index==-1) return NULL;

  return (DIOOBEXPUSHFILE*)SendFileList_GetFile(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::SendFileList_DeleteAll()
* @brief      Send file list GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFileList_DeleteAll()
{
  if(sendfilelist.IsEmpty()) return false;

  sendfilelist.DeleteContents();

  sendfilelist.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOOBEXPUSH::SendFileList_GetIndex()
* @brief      Send file list get index
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOOBEXPUSH::SendFileList_GetIndex()
{
  if(sendfilelist.IsEmpty())                    return -1;
  if((indexfilelist>=(int)sendfilelist.GetSize())) return -1;

  return indexfilelist;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOOBEXPUSH::SendFileList_GetSize()
* @brief      Send file list get size
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOOBEXPUSH::SendFileList_GetSize()
{
  return sendfilelist.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::End()
{
  if(xthreadobex)
    {
      xthreadobex->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, xthreadobex);
      xthreadobex = NULL;
    }

  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();
    }

  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
      xtimer = NULL;
    }

  if(xfile)
    {
      xfile->Close();
      GEN_XFACTORY.Delete_File(xfile);
      xfile = NULL;
    }

  if(sendbuffer)
    {
      GEN_DELETE sendbuffer;
      sendbuffer = NULL;
    }

  SendFileList_DeleteAll();

  SetCurrentState(DIOOBEXPUSH_XFSMSTATE_NONE);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM* DIOOBEXPUSH::GetDIOStream()
* @brief      Get DIO stream
* @ingroup    DATAIO
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOOBEXPUSH::GetDIOStream()
{
  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOOBEXPUSH::GetFileNameSend()
* @brief      Get file name send
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOOBEXPUSH::GetFileNameSend()
{
  return &xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOOBEXPUSH::GenerateFileID(XPATH& xpath)
* @brief      Generate file ID
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOOBEXPUSH::GenerateFileID(XPATH& xpath)
{
  if(xpath.IsEmpty()) return 0;

  HASHCRC32 hash;

  hash.Do(xpath);

  return hash.GetResultCRC32();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::CheckSending()
* @brief      Check sending
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::CheckSending()
{
  DIOOBEXPUSHFILE*    obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(indexfilelist);
  DIOSTREAMBLUETOOTH* sio          = (DIOSTREAMBLUETOOTH*)diostream;
  XWORD               wsize        = 0;
  XDWORD              dsize        = 0;
  bool                islastblock  = false;

  if((!sio)||(!obexpushfile))
    {
      if(obexpushfile) obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNECTION);
      return false;
    }

  if(sio->GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION)
    {
      if(xtimer->GetMeasureSeconds()>=(XDWORD)asktimeout)
        {
          asktimeelapsed = xtimer->GetMeasureMilliSeconds();
          for(int c=0;c<(int)sendfilelist.GetSize();c++)
            {
              obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(c);
              if(obexpushfile) obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNECTION);
            }

          return false;
        }

      return true;
    }

  if(sio->GetStatus()==DIOSTREAMSTATUS_DISCONNECTED)
    {
      haveerror=true;
      SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
    }

  if(GetEvent()==DIOOBEXPUSH_XFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(GetCurrentState())
        {
          case DIOOBEXPUSH_XFSMSTATE_NONE               : break;

          case DIOOBEXPUSH_XFSMSTATE_CONNECT            : { XBUFFER answer;

                                                            if(!ReadAnswer(answer)) break;

                                                            //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_RED, answer);

                                                            if(answer.Get()[0]==0xA0)
                                                              {
                                                                sendblocksize  = (XWORD)answer.Get()[5];
                                                                sendblocksize <<=8;
                                                                sendblocksize |= (XWORD)answer.Get()[6];

                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_INIPUSH);
                                                              }
                                                             else
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                              }
                                                          }
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_INIPUSH            : SetEvent(DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH       : SetEvent(DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH      : sendbuffer->Delete();

                                                          islastblock = ((filesize - readfilesize) >= sendblocksize)? false:true;

                                                          sendbuffer->Add((!islastblock)?(XBYTE)0x02:(XBYTE)0x82);

                                                          wsize = 0;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          if(islastblock)
                                                                   readfilesizeblock = (filesize - readfilesize);
                                                              else readfilesizeblock = sendblocksize -16;

                                                          sendbuffer->Add((!islastblock)?(XBYTE)0x48:(XBYTE)0x49);

                                                          wsize+=3;

                                                          wsize += (int)readfilesizeblock;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          { XBUFFER datafile((XDWORD)readfilesizeblock, true);

                                                            xfile->Read(datafile.Get(), (XDWORD*)&readfilesizeblock);
                                                            if(!readfilesizeblock)
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                break;
                                                              }

                                                            sendbuffer->Add(datafile);
                                                          }

                                                          if(inlittleendian)
                                                                  wsize = SwapWORD((XWORD)sendbuffer->GetSize());
                                                            else  wsize = (XWORD)sendbuffer->GetSize();
                                                          sendbuffer->Set((XBYTE*)&wsize, 2, 1);

                                                          //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_GREEN, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(), sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);

                                                          if(filesize) obexpushfile->SetSendPercent((XBYTE)((readfilesize*100)/filesize));

                                                          SetEvent(DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH       :
          case DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH      : { XBUFFER answer;

                                                            if(!ReadAnswer(answer)) break;

                                                            //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_RED, answer);

                                                            switch(answer.Get()[0])
                                                              {
                                                                case 0x90 : readfilesize+=readfilesizeblock;
                                                                            SetEvent(DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH);
                                                                            break;

                                                                case 0xA0 : haveerror = false;
                                                                            SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                            break;

                                                                  default : if(GetEvent()==DIOOBEXPUSH_XFSMEVENT_NONE)
                                                                              {
                                                                                haveerror = true;
                                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                              }
                                                                            break;
                                                              }

                                                          }
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_ENDPUSH            : return false;
        }
    }
   else //  Nuevo evento
    {
      if(GetEvent()<DIOOBEXPUSH_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case DIOOBEXPUSH_XFSMSTATE_NONE           : break;

              case DIOOBEXPUSH_XFSMSTATE_CONNECT        : sendisstart = false;

                                                          asktimeelapsed = xtimer->GetMeasureMilliSeconds();

                                                          sendbuffer->Delete();

                                                          sendbuffer->Add((XBYTE)0x80);

                                                          wsize = 0x0007;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          sendbuffer->Add((XBYTE)0x10);
                                                          sendbuffer->Add((XBYTE)0x00);

                                                          wsize = DIOOBEXPUSH_MAXBUFFER;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_GREEN, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(), sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_INIPUSH        : { xtimer->Reset();

                                                            obexpushfile->SetIsSend(false);
                                                            obexpushfile->SetSendPercent(0);

                                                            xpath = (*obexpushfile->GetXPath());

                                                            if((!xfile)||(!xfile->Open(xpath, true))||(!sendbuffer))
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                break;
                                                              }

                                                            readfilesizeblock = 0;

                                                            sendisstart     = true;
                                                            filesize        = xfile->GetSize();
                                                            readfilesize    = 0;
                                                          }
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH   : sendbuffer->Delete();

                                                          sendbuffer->Add((XBYTE)0x02);

                                                          wsize = 0;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          sendbuffer->Add((XBYTE)0x01);

                                                          // Unicode 16 + 1 byte of head;
                                                          wsize++;
                                                          wsize += (XWORD)((obexpushfile->GetNameSend()->GetSize()+1)*2);
                                                          sendbuffer->Add((XWORD)wsize);

                                                          XBUFFER xbufferexchange;
                                                          (*obexpushfile->GetNameSend())->ConvertToBufferExchange(xbufferexchange, true, true);
                                                          sendbuffer->Add(xbufferexchange->Get(), wsize);  

                                                          sendbuffer->Add((XBYTE)0xC3);
                                                          sendbuffer->Add((XDWORD)filesize);

                                                          if(inlittleendian)
                                                                  wsize = SwapWORD((XWORD)sendbuffer->GetSize());
                                                            else  wsize = (XWORD)sendbuffer->GetSize();
                                                          sendbuffer->Set((XBYTE*)&wsize, 2, 1);

                                                          //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_GREEN, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(), sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH   : break;

              case DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH  : break;

              case DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH  : break;

              case DIOOBEXPUSH_XFSMSTATE_ENDPUSH        : if(xfile) xfile->Close();

                                                          if(!haveerror)
                                                            {
                                                              obexpushfile->SetIsSend(true);
                                                              obexpushfile->SetSendPercent(100);
                                                              obexpushfile->SetSendTimeElapsed(xtimer->GetMeasureMilliSeconds());
                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_NONE);

                                                              if((indexfilelist+1<(int)sendfilelist.GetSize()))
                                                                {
                                                                  indexfilelist++;
                                                                  SetEvent(DIOOBEXPUSH_XFSMEVENT_INIPUSH);
                                                                }
                                                            }
                                                           else
                                                            {
                                                              for(int c=0;c<(int)sendfilelist.GetSize();c++)
                                                                {
                                                                  obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(c);
                                                                  if(obexpushfile)
                                                                    {
                                                                      obexpushfile->SetIsSend(false);

                                                                      if(sendisstart)
                                                                        {
                                                                          obexpushfile->SetError(DIOOBEXPUSHERROR_SENDING);
                                                                        }
                                                                       else
                                                                        {
                                                                          if(sio->IsRefusedConnection())
                                                                            {
                                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_REFUSEDCONNECTION);
                                                                            }
                                                                           else
                                                                            {
                                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNECTION);
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                              return false;
                                                            }
                                                          break;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOOBEXPUSH::ReadAnswer(XBUFFER& xbuffer)
* @brief      Read answer
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOOBEXPUSH::ReadAnswer(XBUFFER& xbuffer)
{
  if(!diostream) return false;

  int  sizeread;

  xbuffer.Resize(DIOOBEXPUSH_MAXANSWER);

  sizeread = diostream->Read((XBYTE*)xbuffer.Get(), xbuffer.GetSize());
  xbuffer.Resize(sizeread);

  return sizeread?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOOBEXPUSH::ThreadSend(void* data)
* @brief      Thread send
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOOBEXPUSH::ThreadSend(void* data)
{
  DIOOBEXPUSH* obexpush = (DIOOBEXPUSH*)data;
  if(!obexpush) return;

  obexpush->SetIsSending(obexpush->CheckSending());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOOBEXPUSH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOOBEXPUSH::Clean()
{
  diostream           = NULL;
  inlittleendian      = false;

  xtimer              = NULL;
  asktimeout          = DIOOBEXPUSH_MAXTIMEOUTASK;
  asktimeelapsed      = 0;

  indexfilelist       = -1;

  xfile               = NULL;

  sendbuffer          = NULL;

  issending           = false;

  sendblocksize       = 0;
  sendisstart         = false;

  filesize            = 0;
  readfilesize        = 0;
  readfilesizeblock   = 0;

  haveerror           = false;

  xthreadobex         = NULL;
}




