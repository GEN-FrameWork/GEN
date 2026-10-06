/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUDPAcknowledge.cpp
* 
* @class      DIOSTREAMUDPACKNOWLEDGE
* @brief      Data Input/Output Stream UDP with Acknowledge class
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

#include "DIOStreamUDPAcknowledge.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"

#include "HashCRC32.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDPACKNOWLEDGE::DIOSTREAMUDPACKNOWLEDGE(DIOSTREAMUDPCONFIG* config) : DIOSTREAMUDP()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* 
* @param[in]  config : Stream UDP configuration.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDPACKNOWLEDGE::DIOSTREAMUDPACKNOWLEDGE(DIOSTREAMUDPCONFIG* config) : DIOSTREAMUDP()
{
  Clean();

  this->config      = config;
  timeout           = DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT;

  diostream = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO((DIOSTREAMCONFIG*)config);
  if(diostream)  URLremote = GEN_NEW DIOURL();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDPACKNOWLEDGE::~DIOSTREAMUDPACKNOWLEDGE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDPACKNOWLEDGE::~DIOSTREAMUDPACKNOWLEDGE()
{
  Close();

  if(diostream) GEN_DIOFACTORY.DeleteStreamIO(diostream);
  if(URLremote) GEN_DELETE URLremote;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIG* DIOSTREAMUDPACKNOWLEDGE::GetConfig()
* @brief      Get config
* @ingroup    DATAIO
* 
* @return     DIOSTREAMCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIG* DIOSTREAMUDPACKNOWLEDGE::GetConfig()
{ 
  return (DIOSTREAMCONFIG*)config;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::SetConfig(DIOSTREAMCONFIG* config)
* @brief      Set config
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::SetConfig(DIOSTREAMCONFIG* config)
{
  this->config = (DIOSTREAMUDPCONFIG*)config;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOSTREAMUDPACKNOWLEDGE::GetStatus()
* @brief      Get status
* @ingroup    DATAIO
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOSTREAMUDPACKNOWLEDGE::GetStatus()
{
  if(!diostream) return DIOSTREAMSTATUS_DISCONNECTED;
  return diostream->GetStatus();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMUDPACKNOWLEDGE::GetTimeout()
* @brief      Get timeout
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMUDPACKNOWLEDGE::GetTimeout()                                                              
{ 
  return timeout;                    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPACKNOWLEDGE::SetTimeout(int timeout)
* @brief      Set timeout
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPACKNOWLEDGE::SetTimeout(int timeout)
{ 
  this->timeout = timeout;           
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::Open()
* @brief      Open
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Open()
{
  if(!diostream) return false;
  if(!URLremote) return false;
  if(!config) return false;

  if(config->GetRemoteURL()->GetSize())
    {
      URLremote->Set(config->GetRemoteURL()->Get());
      URLremote->ResolveURL(addressremote);
    }

  portremote = config->GetRemotePort();

  if(!diostream->Open()) return false;

  if(config->IsServer())
    {
      xthreadconnection  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, _L("DIOSTREAMUDPACKNOWLEDGE::Open"), ThreadRunFunction, (void*)this);
      if(xthreadconnection) xthreadconnection->Ini();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMUDPACKNOWLEDGE::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMUDPACKNOWLEDGE::Write(XBYTE* buffer, XDWORD size)
{
  if(!diostream)      return 0;
  if(!size)           return 0;

  if(addressremote.IsEmpty()) return 0;

  DIOSTREAMUDP* diostreamudp = (DIOSTREAMUDP*)diostream;
  XDWORD        bw           = 0;

  if(diostreamudp->WriteDatagram(addressremote, portremote, buffer, size)) bw = size;

  if(diostreamudp->WaitToWriteDatagramsEmpty(timeout))
    {
      //XTRACE_PRINTCOLOR(1, _L("Write UDP %s:%d : %s"), addressremote.Get(), portremote, bw==size??_L("[Ok.]"):_L("[Error!]"));

      if(diostreamudp->WaitToGetFirstDatagram(false, timeout) != DIOSTREAMUDP_NOTFOUND)
        {
          XSTRING address;
          XWORD   port       = 0;
          XBUFFER xbuffer;
          XDWORD  hashresult[2] = { 0, 0 };

          diostreamudp->ReadDatagram(address, port, xbuffer);

          xbuffer.Extract(hashresult[0]);

          HASHCRC32  hashcrc32;

          hashcrc32.ResetResult();
          hashcrc32.Do(buffer, size);

          hashresult[1] = hashcrc32.GetResultCRC32();

          if(hashresult[0] != hashresult[1])
            {
              //XTRACE_PRINTCOLOR(1, _L("Write received bad CRC!! %08X <> %08X"), hashresult, hashcrc32.GetResultCRC32());
              bw = 0;
            }
        }
       else
        {
          //XTRACE_PRINTCOLOR(1, _L("Write received NOT read"));
          bw = 0;
        }
    }
   else
    {
      //XTRACE_PRINTCOLOR(1, _L("Do not write"));
      bw = 0;
    }

  if(!bw)
    {
      diostream->Close();
      diostream->Open();
      diostream->WaitToConnected(5);
    }

  return bw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size)
* @brief      Write datagram
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* @param[in]  port : Port number to use.
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size)
{
  XSTRING old_address;
  int     old_port;

  old_address   = addressremote.Get();
  old_port      = portremote;

  addressremote = address.Get();
  portremote    = port;

  int _size = Write(buffer, size);

  addressremote = old_address.Get();
  portremote    = old_port;

  return (size == _size)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
* @brief      Write datagram
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* @param[in]  port : Port number to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
{
  return WriteDatagram(address, port, xbuffer.Get(), xbuffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::Disconnect()
* @brief      Disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Disconnect()
{
  if(!diostream) return false;

  if(!config) return false;

  if(config->IsServer())
    {
      if(xthreadconnection)
        {
          xthreadconnection->End();
          DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadconnection);
          xthreadconnection = NULL;
        }
    }

  if(!diostream->Disconnect()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPACKNOWLEDGE::Close()
* @brief      Close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Close()
{
  if(!diostream) return false;

  if(!config) return false;

  if(config->IsServer())
    {
      if(xthreadconnection)
        {
          xthreadconnection->End();
          DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadconnection);
          xthreadconnection = NULL;
        }
    }

  if(!diostream->Close()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM* DIOSTREAMUDPACKNOWLEDGE::GetDIOStreamBase()
* @brief      Get DIO stream base
* @ingroup    DATAIO
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOSTREAMUDPACKNOWLEDGE::GetDIOStreamBase()
{ 
  return diostream;                 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPACKNOWLEDGE::ThreadRunFunction(void* param)
* @brief      Thread run function
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPACKNOWLEDGE::ThreadRunFunction(void* param)
{
  DIOSTREAMUDPACKNOWLEDGE* diostreamudpacknowledge = (DIOSTREAMUDPACKNOWLEDGE*)param;
  if(!diostreamudpacknowledge) return;

  DIOSTREAMUDP* diostreamudp = diostreamudpacknowledge->diostream;
  if(!diostreamudp) return;

  XBUFFER  data;
  XSTRING  address;
  XWORD    port;
  
  if(diostreamudp->ReadDatagram(address, port, data))
    {
      HASHCRC32 hashcrc32;

      hashcrc32.ResetResult();
      if(hashcrc32.Do(data.Get(), data.GetSize()))
        {
          XBUFFER result;
          XDWORD  hashresult = hashcrc32.GetResultCRC32();

          result.Add(hashresult);

          diostreamudp->WriteDatagram(address, port, result);
          if(diostreamudp->WaitToWriteDatagramsEmpty(diostreamudpacknowledge->timeout))
            {
              diostreamudpacknowledge->AddDatagram(false, address, port, data.Get(), data.GetSize());
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPACKNOWLEDGE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPACKNOWLEDGE::Clean()
{
  config              = NULL;

  timeout             = 0;

  URLremote           = NULL;
  portremote          = 0;
  diostream           = NULL;
  xthreadconnection   = NULL;
}



