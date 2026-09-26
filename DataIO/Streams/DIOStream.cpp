/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStream.cpp
* 
* @class      DIOSTREAM
* @brief      Data Input/Output Stream class
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

#include "DIOStream.h"

#include <string.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XTimer.h"

#include "DIOStream_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM::DIOSTREAM()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM::DIOSTREAM()
{
  Clean();

  inbuffer  = GEN_NEW XBUFFER();
  outbuffer = GEN_NEW XBUFFER();

  xmutextimerout=GEN_XFACTORY.Create_Mutex();
  xtimerconnection=GEN_XFACTORY.CreateTimer();
  xtimernotactivity=GEN_XFACTORY.CreateTimer();
  xtimerout=GEN_XFACTORY.CreateTimer();
  
  RegisterEvent(DIOSTREAM_XEVENT_TYPE_GETTINGCONNECTION);
  RegisterEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED);
  RegisterEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM::~DIOSTREAM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM::~DIOSTREAM()
{
  if(inbuffer)
    {
      GEN_DELETE inbuffer;
      inbuffer = NULL;
    }

  if(outbuffer)
    {
      GEN_DELETE outbuffer;
      outbuffer = NULL;
    }

  if(xmutextimerout) xmutextimerout->Lock();

  if(xtimerout)
    {
      GEN_XFACTORY.DeleteTimer(xtimerout);
      xtimerout = NULL;
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  if(xtimerconnection)
    {
      GEN_XFACTORY.DeleteTimer(xtimerconnection);
      xtimerconnection = NULL;
    }

  if(xtimernotactivity)
    {
      GEN_XFACTORY.DeleteTimer(xtimernotactivity);
      xtimernotactivity = NULL;
    }

  if(xmutextimerout)
    {
      GEN_XFACTORY.Delete_Mutex(xmutextimerout);
      xmutextimerout = NULL;
    }

  DeRegisterEvent(DIOSTREAM_XEVENT_TYPE_GETTINGCONNECTION);
  DeRegisterEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED);
  DeRegisterEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOSTREAM::GetStatus()
* @brief      Get status
* @ingroup    DATAIO
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOSTREAM::GetStatus()
{
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::SetStatus(DIOSTREAMSTATUS status)
* @brief      Set status
* @ingroup    DATAIO
* 
* @param[in]  status : Status value to read or update.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::SetStatus(DIOSTREAMSTATUS status)
{
  this->status = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsConnected()
* @brief      Is connected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsConnected()
{
  if(GetStatus()==DIOSTREAMSTATUS_CONNECTED) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsDisconnected()
* @brief      Is disconnected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsDisconnected()
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WaitToConnected(int timeout)
* @brief      Wait to connected
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WaitToConnected(int timeout)
{
  if(!xtimerconnection) 
    {
      return false;
    }

  bool status = false;

  xtimerconnection->Reset();  

  while(1)
    {
      if(IsConnected())
        {
          status = true;
          break;
        }
      
      if(GetStatus() == DIOSTREAMSTATUS_DISCONNECTED)
        {
          break;
        }
      

      if((int)xtimerconnection->GetMeasureSeconds() >= timeout)
        {
          break;
        }

      // Poll cadence is config-driven (defaults to DIOSTREAM_TIMEINWAITFUNCTIONS) so a
      // specific stream (e.g. DIOPING, for accurate RTT) can request a tighter cadence
      // via DIOSTREAMCONFIG::SetPollInterval() without affecting other DIOSTREAM users.
      { DIOSTREAMCONFIG* _pollconfig = GetConfig();
        Wait(_pollconfig ? _pollconfig->GetPollInterval() : DIOSTREAM_TIMEINWAITFUNCTIONS);
      }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WaitToDisconnected(int timeout)
* @brief      Wait to disconnected
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WaitToDisconnected(int timeout)
{
  if(!xtimerconnection) return false;

  bool status = false;

  xtimerconnection->Reset();

  while(1)
    {
      if(IsDisconnected())
        {
          status = true;
          break;
        }

      if((int)xtimerconnection->GetMeasureSeconds() >= timeout) break;

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::Read(XBYTE* buffer, XDWORD size)
* @brief      Read
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::Read(XBYTE* buffer, XDWORD size)
{
  if(!inbuffer) return false;

  XDWORD esize = inbuffer->Extract(buffer, 0, size);

  if(esize)
    {
      nbytesread+=size;
      if(xtimernotactivity) xtimernotactivity->Reset();
    }

  return esize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::Write(XBYTE* buffer, XDWORD size)
{
  if(!outbuffer)                    return 0;
  if(!outbuffer->Add(buffer, size))  return 0;

  nbyteswrite+=size;
  if(xtimernotactivity) xtimernotactivity->Reset();

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::Read(XBUFFER& xbuffer)
* @brief      Read
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::Read(XBUFFER& xbuffer)
{
  if(inbuffer->IsEmpty()) return 0;

  XDWORD br = Read(xbuffer.Get(), xbuffer.GetSize());
  if(br)
    {
      if(br!=xbuffer.GetSize()) xbuffer.Resize(br);
    }
   else
    {
      xbuffer.Delete();
    }

  return xbuffer.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::Write(XBUFFER& xbuffer)
* @brief      Write
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::Write(XBUFFER& xbuffer)
{
  return Write(xbuffer.Get(), xbuffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WaitToFilledReadingBuffer(int filledto, int timeout)
* @brief      Wait to filled reading buffer
* @ingroup    DATAIO
* 
* @param[in]  filledto : Filledto value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(!xtimerout)  return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(1)
    {
      if(filledto == DIOSTREAM_SOMETHINGTOREAD)
        {
          if(GetInXBuffer()->GetSize()) break;
        }
       else
        {
          if((int)GetInXBuffer()->GetSize() >= filledto) break;
        }

      if(timeout!=XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      // Poll cadence is config-driven (defaults to DIOSTREAM_TIMEINWAITFUNCTIONS) so a
      // specific stream (e.g. DIOPING, for accurate RTT) can request a tighter cadence
      // via DIOSTREAMCONFIG::SetPollInterval() without affecting other DIOSTREAM users.
      { DIOSTREAMCONFIG* _pollconfig = GetConfig();
        Wait(_pollconfig ? _pollconfig->GetPollInterval() : DIOSTREAM_TIMEINWAITFUNCTIONS);
      }
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ReadStr(char* str)
* @brief      Read str
* @ingroup    DATAIO
* 
* @param[in]  str : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(char* str)
{
  if(!str) return false;

  XSTRING string;
  bool    status = ReadStr(string);

  XBUFFER charstr;
  
  string.ConvertToASCII(charstr);  
  memcpy(str, charstr.Get(), string.GetSize());
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ReadStr(XSTRING& string)
* @brief      Read str
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(XSTRING& string)
{
  XBUFFER* readbuffer;
  bool foundreturn = false;
  int  stringsize  = 0;

  readbuffer = GetInXBuffer();
  if(!readbuffer) return false;

  readbuffer->SetBlocked(true);

  for(int c=0;c<(int)readbuffer->GetSize();c++)
    {
      if((readbuffer->GetByte(c) =='\n') || (readbuffer->GetByte(c) == '\r'))
        {
          stringsize++;

          if(c+1<(int)readbuffer->GetSize())
            {
              if((readbuffer->GetByte(c+1) == '\n') || (readbuffer->GetByte(c+1) =='\r')) stringsize++;
            }

          foundreturn = true;

          break;

        } else stringsize++;
    }

  readbuffer->SetBlocked(false);

  if(!foundreturn) return false;

  XBYTE* buffer = GEN_NEW XBYTE[stringsize+1];
  if(!buffer) return false;

  memset(buffer, 0, stringsize+1);

  int br = Read((XBYTE*)buffer, stringsize);

  if(br != stringsize) return false;

  int c=(int)strlen((const char*)buffer);
  while(c>=0)
    {
      if((buffer[c]=='\n')||(buffer[c]=='\r')) buffer[c]=0;
      c--;
    }

  string.Empty();
  string = (char*)buffer;

  GEN_DELETE_ARRAY buffer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ReadStr(XSTRING& string, int timeout)
* @brief      Read str
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(XSTRING& string, int timeout)
{
  bool status = true;

  if(!xtimerout) return false;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(!ReadStr(string))
    {
      if((int)xtimerout->GetMeasureSeconds() >= timeout)
        {
          status = false;
          break;
        }
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WriteStr(const char* str)
* @brief      Write str
* @ingroup    DATAIO
* 
* @param[in]  str : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WriteStr(const char* str)
{
  if(!str) return false;

  if(!Write((XBYTE*)str, (XDWORD)strlen(str))) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WriteStr(XCHAR* string)
* @brief      Write str
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WriteStr(XCHAR* string)
{
  XSTRING str(string);

  XBUFFER charstr;
  
  str.ConvertToASCII(charstr);
  
  return WriteStr(charstr.GetPtrChar());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WriteStr(XSTRING& string)
* @brief      Write str
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WriteStr(XSTRING& string)
{
  XBUFFER charstr;
  
  string.ConvertToASCII(charstr);
  
  return WriteStr(charstr.GetPtrChar());  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsBlockRead()
* @brief      Is block read
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsBlockRead()
{
  return isblockread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::SetIsBlockRead(bool isblockread)
* @brief      Set is block read
* @ingroup    DATAIO
* 
* @param[in]  isblockread : Isblockread value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::SetIsBlockRead(bool isblockread)
{
  this->isblockread = isblockread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsBlockWrite()
* @brief      Is block write
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsBlockWrite()
{
  return isblockwrite;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::SetIsBlockWrite(bool isblockwrite)
* @brief      Set is block write
* @ingroup    DATAIO
* 
* @param[in]  isblockwrite : Isblockwrite value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::SetIsBlockWrite(bool isblockwrite)
{
  this->isblockwrite = isblockwrite;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::Disconnect()
* @brief      Disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTYPE DIOSTREAM::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTYPE DIOSTREAM::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::SetType(DIOSTREAMTYPE type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::SetType(DIOSTREAMTYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAM::GetInXBuffer()
* @brief      Get in X buffer
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAM::GetInXBuffer()
{
  return inbuffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAM::GetOutXBuffer()
* @brief      Get out X buffer
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAM::GetOutXBuffer()
{
  return outbuffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ResetXBuffers()
* @brief      Reset X buffers
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ResetXBuffers()
{
  if((!inbuffer) && (!outbuffer)) return false;

  inbuffer->Delete();
  outbuffer->Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ResetInXBuffer()
* @brief      Reset in X buffer
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ResetInXBuffer()
{
  if(!inbuffer) return false;

  inbuffer->Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ResetOutXBuffer()
* @brief      Reset out X buffer
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ResetOutXBuffer()
{
  if(!outbuffer) return false;

  outbuffer->Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsFlushOutXBuffer()
* @brief      Is flush out X buffer
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsFlushOutXBuffer()
{
  if(!outbuffer) return false;

  if(outbuffer->GetSize()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::IsFlushXBuffers()
* @brief      Is flush X buffers
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::IsFlushXBuffers()
{
  if((!inbuffer) && (!outbuffer)) return false;

  if(inbuffer->GetSize())  return false;
  if(outbuffer->GetSize()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WaitToFlushOutXBuffer(int timeout, bool mutexactive)
* @brief      wait to flush out Xbuffer
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* @param[in]  mutexactive : Mutexactive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WaitToFlushOutXBuffer(int timeout, bool mutexactive)
{
  if(!xtimerout) return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout && mutexactive) 
    {
      xmutextimerout->Lock();
    }

  while(!IsFlushOutXBuffer())
    {
      if(timeout != XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              ResetOutXBuffer();

              status = false;
              break;
            }
        }

      // Poll cadence is config-driven (defaults to DIOSTREAM_TIMEINWAITFUNCTIONS) so a
      // specific stream (e.g. DIOPING, for accurate RTT) can request a tighter cadence
      // via DIOSTREAMCONFIG::SetPollInterval() without affecting other DIOSTREAM users.
      { DIOSTREAMCONFIG* _pollconfig = GetConfig();
        Wait(_pollconfig ? _pollconfig->GetPollInterval() : DIOSTREAM_TIMEINWAITFUNCTIONS);
      }
    }

  if(xmutextimerout && mutexactive) 
    {
      xmutextimerout->UnLock();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::WaitToFlushXBuffers(int timeout, bool mutexactive)
* @brief      wait to flush Xbuffers
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* @param[in]  mutexactive : Mutexactive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::WaitToFlushXBuffers(int timeout, bool mutexactive)
{
  if(!xtimerout) return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout && mutexactive) 
    {
      xmutextimerout->Lock();
    }

  while(!IsFlushXBuffers())
    {
      if(timeout!=XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              ResetXBuffers();

              status = false;
              break;
            }
        }

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  if(xmutextimerout && mutexactive) 
    {
      xmutextimerout->UnLock();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOSTREAM::GetXTimerConnection()
* @brief      Get X timer connection
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOSTREAM::GetXTimerConnection()
{
  return xtimerconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOSTREAM::GetXTimerNotActivity()
* @brief      Get X timer not activity
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOSTREAM::GetXTimerNotActivity()
{
  return xtimernotactivity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::GetNBytesRead()
* @brief      Get N bytes read
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::GetNBytesRead()
{
  return nbytesread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::AddNBytesRead(XDWORD size)
* @brief      Add N bytes read
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::AddNBytesRead(XDWORD size)
{
  nbytesread += size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAM::GetNBytesWrite()
* @brief      Get N bytes write
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAM::GetNBytesWrite()
{
  return nbyteswrite;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::AddNBytesWrite(XDWORD size)
* @brief      Add N bytes write
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::AddNBytesWrite(XDWORD size)
{
  nbyteswrite += size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMERROR DIOSTREAM::GetLastDIOError()
* @brief      Get last DIO error
* @ingroup    DATAIO
* 
* @return     DIOSTREAMERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMERROR DIOSTREAM::GetLastDIOError()
{
  DIOSTREAMERROR ret = error;

  error = DIOSTREAMERROR_NONE;
  return ret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMERROR DIOSTREAM::PeekLastDIOError()
* @brief      Read the last DIO error without consuming it
* @ingroup    DATAIO
*
* @return     DIOSTREAMERROR : Last error retained by the stream.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMERROR DIOSTREAM::PeekLastDIOError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::SetLastDIOError(DIOSTREAMERROR error)
* @brief      Set last DIO error
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::SetLastDIOError(DIOSTREAMERROR error)
{
  this->error = error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::Wait(int milliseconds)
* @brief      Wait
* @ingroup    DATAIO
* 
* @param[in]  milliseconds : Milliseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::Wait(int milliseconds)
{
  GEN_XSLEEP.MilliSeconds(milliseconds);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAM::ResetConnectionStatistics()
* @brief      Reset connection statistics
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAM::ResetConnectionStatistics()
{
  nbytesread  = 0;
  nbyteswrite = 0;

  if(xtimerconnection)   xtimerconnection->Reset();
  if(xtimernotactivity) xtimernotactivity->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAM::Clean()
{
  type                = DIOSTREAMTYPE_UNKNOWN;

  status              = DIOSTREAMSTATUS_GETTINGCONNECTION;
  error               = DIOSTREAMERROR_NONE;

  xtimerconnection    = NULL;
  xtimernotactivity   = NULL;
  xtimerout           = NULL;

  xmutextimerout      = NULL;

  inbuffer            = NULL;
  outbuffer           = NULL;

  isblockread         = false;
  isblockwrite        = false;

  nbytesread          = 0;
  nbyteswrite         = 0;
}

