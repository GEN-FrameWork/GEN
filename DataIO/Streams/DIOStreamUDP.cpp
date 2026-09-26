/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUDP.cpp
* 
* @class      DIOSTREAMUDP
* @brief      Data Input/Output Stream UDP class
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

#include "DIOStreamUDP.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XThread.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDPDATAGRAM::DIOSTREAMUDPDATAGRAM()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDPDATAGRAM::DIOSTREAMUDPDATAGRAM()
{
  Clean();
  data = GEN_NEW XBUFFER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDPDATAGRAM::~DIOSTREAMUDPDATAGRAM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDPDATAGRAM::~DIOSTREAMUDPDATAGRAM()
{
  if(data) GEN_DELETE data;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPDATAGRAM::IsToSend()
* @brief      Is to send
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPDATAGRAM::IsToSend()
{
  return istosend;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPDATAGRAM::SetIsToSend(bool istosend)
* @brief      Set is to send
* @ingroup    DATAIO
* 
* @param[in]  istosend : Istosend value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPDATAGRAM::SetIsToSend(bool istosend)
{
  this->istosend = istosend;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMUDPDATAGRAM::GetAddress()
* @brief      Get address
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMUDPDATAGRAM::GetAddress()
{
  return &address;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPDATAGRAM::SetAddress(XCHAR* address)
* @brief      Set address
* @ingroup    DATAIO
* 
* @param[in]  address : Address pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPDATAGRAM::SetAddress(XCHAR* address)
{
  if(!address) return false;

  this->address = address;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPDATAGRAM::SetAddress(XSTRING& address)
* @brief      Set address
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPDATAGRAM::SetAddress(XSTRING& address)
{
  return SetAddress(address.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMUDPDATAGRAM::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMUDPDATAGRAM::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPDATAGRAM::SetPorts(XWORD port)
* @brief      Set ports
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPDATAGRAM::SetPorts(XWORD port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMUDPDATAGRAM::GetData()
* @brief      Get data
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMUDPDATAGRAM::GetData()
{
  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDPDATAGRAM::SetData(XBYTE* data, XDWORD size)
* @brief      Set data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDPDATAGRAM::SetData(XBYTE* data, XDWORD size)
{
  if(!this->data) return false;

  this->data->Delete();
  this->data->Add(data, size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPDATAGRAM::SetData(XBUFFER& data)
* @brief      Set data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPDATAGRAM::SetData(XBUFFER& data)
{
  SetData(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDPDATAGRAM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDPDATAGRAM::Clean()
{
  istosend = false;
  address.Empty();
  port     = 0;
  data     = NULL;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDP::DIOSTREAMUDP() : DIOSTREAM()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDP::DIOSTREAMUDP() : DIOSTREAM()
{
  Clean();

  datagramsmutex=GEN_XFACTORY.Create_Mutex();
  host = GEN_NEW DIOURL();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDP::~DIOSTREAMUDP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDP::~DIOSTREAMUDP()
{
  DeleteAllDatagrams();

  GEN_DELETE host;

  GEN_XFACTORY.Delete_Mutex(datagramsmutex);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIG* DIOSTREAMUDP::GetConfig()
* @brief      Get config
* @ingroup    DATAIO
* 
* @return     DIOSTREAMCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIG* DIOSTREAMUDP::GetConfig()
{
  return config;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::SetConfig(DIOSTREAMCONFIG* config)
* @brief      Set config
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::SetConfig(DIOSTREAMCONFIG* config)
{
  if(!config) return false;
  this->config = (DIOSTREAMUDPCONFIG *)config;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::Open()
* @brief      Open
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::Open()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::ReadDatagram(XSTRING& address, XWORD& port, XBUFFER& xbuffer)
* @brief      Read datagram
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* @param[in]  port : Port number to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::ReadDatagram(XSTRING& address, XWORD& port, XBUFFER& xbuffer)
{
  int index = GetFirstDatagram(false);
  if(index != DIOSTREAMUDP_NOTFOUND)
    {
      DIOSTREAMUDPDATAGRAM* datagram = GetDatagram(index);
      if(datagram)
        {
          address = datagram->GetAddress()->Get();
          port    = datagram->GetPort();

          xbuffer.Delete();
          xbuffer.Add(datagram->GetData());

          nbytesread+= datagram->GetData()->GetSize();
          if(xtimernotactivity) xtimernotactivity->Reset();

          DeleteDatagram(index);

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size)
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
bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size)
{
  if(address.IsEmpty())                                 return false;
  if(!AddDatagram(true, address , port, buffer, size))  return false;
  if(!outbuffer->Add(buffer, size))                      return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
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
bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
{
  return WriteDatagram(address, port, xbuffer.Get(), xbuffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::Disconnect()
* @brief      Disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::Close()
* @brief      Close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::Close()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::ResetXBuffers()
* @brief      Reset X buffers
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetXBuffers()
{
  if(!DIOSTREAM::ResetXBuffers()) return false;

  return DeleteAllDatagrams();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::ResetInXBuffer()
* @brief      Reset in X buffer
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetInXBuffer()
{
  if(!DIOSTREAM::ResetInXBuffer()) return false;

  int index = 0;

  while(1)
    {
      DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(index);
      if(!datagram) break;

      if(datagram->GetAddress()->GetSize())
        {
          DeleteDatagram(index);

        } else index++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::ResetOutXBuffer()
* @brief      Reset out X buffer
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetOutXBuffer()
{
  if(!DIOSTREAM::ResetOutXBuffer()) return false;

  int index = 0;

  while(1)
    {
      DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(index);
      if(!datagram) break;

      if(!datagram->GetAddress()->GetSize())
        {
          DeleteDatagram(index);

        } else index++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOSTREAMUDP::GetHost()
* @brief      Get host
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOSTREAMUDP::GetHost()
{
  return host;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::WaitToWriteDatagramsEmpty(int timeout)
* @brief      Wait to write datagrams empty
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::WaitToWriteDatagramsEmpty(int timeout)
{
  if (!xtimerconnection) return false;

  int index;

  xtimerconnection->Reset();

  while(1)
    {
      index = GetFirstDatagram(true);
      if(index == DIOSTREAMUDP_NOTFOUND)
        {
          return true;
        }

      if((int)xtimerconnection->GetMeasureSeconds() >= timeout) break;

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMUDP::WaitToGetFirstDatagram(bool tosend, int timeout)
* @brief      Wait to get first datagram
* @ingroup    DATAIO
* 
* @param[in]  tosend : Tosend value.
* @param[in]  timeout : Timeout value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMUDP::WaitToGetFirstDatagram(bool tosend, int timeout)
{
  if (!xtimerconnection) return false;

  int index;

  xtimerconnection->Reset();

  while(1)
    {
      index = GetFirstDatagram(tosend);
      if(index != DIOSTREAMUDP_NOTFOUND)
        {
          break;
        }

      if((int)xtimerconnection->GetMeasureSeconds() >= timeout) break;

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  return index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOSTREAMUDPDATAGRAM*>* DIOSTREAMUDP::GetDatagramsVector()
* @brief      Get datagrams vector
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOSTREAMUDPDATAGRAM*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMUDPDATAGRAM*>* DIOSTREAMUDP::GetDatagramsVector()
{
  return &datagrams;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUDPDATAGRAM* DIOSTREAMUDP::GetDatagram(int index)
* @brief      Get datagram
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOSTREAMUDPDATAGRAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUDPDATAGRAM* DIOSTREAMUDP::GetDatagram(int index)
{
  if(datagramsmutex)  datagramsmutex->Lock();

  DIOSTREAMUDPDATAGRAM* datagram = datagrams.Get(index);

  if(datagramsmutex)  datagramsmutex->UnLock();

  return datagram;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::DeleteDatagram(int index)
* @brief      Delete datagram
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::DeleteDatagram(int index)
{
  if(datagramsmutex)  datagramsmutex->Lock();

  DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)GetDatagram(index);
  if(datagram)
    {
      datagrams.DeleteIndex(index);
      GEN_DELETE datagram;
      datagram = NULL;
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return datagram?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::DeleteAllDatagrams()
* @brief      Delete all datagrams
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::DeleteAllDatagrams()
{
  if(datagrams.IsEmpty()) return false;

  if(datagramsmutex)  datagramsmutex->Lock();

  datagrams.DeleteContents();

  datagrams.DeleteAll();

  if(datagramsmutex)  datagramsmutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::AddDatagram(bool istosend, XCHAR* address, XWORD port, XBYTE* data, XDWORD size)
* @brief      Add datagram
* @ingroup    DATAIO
* 
* @param[in]  istosend : Istosend value.
* @param[in]  address : Address pointer to use.
* @param[in]  port : Port number to use.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::AddDatagram(bool istosend, XCHAR* address, XWORD port, XBYTE* data, XDWORD size)
{
  if(!address) return false;
  if(!data)    return false;

  if(datagramsmutex)  datagramsmutex->Lock();

  DIOSTREAMUDPDATAGRAM* datagram = GEN_NEW DIOSTREAMUDPDATAGRAM();
  if(datagram)
    {
      datagram->SetIsToSend(istosend);
      datagram->SetAddress(address);
      datagram->SetPorts(port);
      datagram->SetData(data, size);

      datagrams.Add(datagram);
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return datagram?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUDP::AddDatagram(bool istosend, XSTRING& address, XWORD port, XBYTE* data, XDWORD size)
* @brief      Add datagram
* @ingroup    DATAIO
* 
* @param[in]  istosend : Istosend value.
* @param[in]  address : Address value.
* @param[in]  port : Port number to use.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUDP::AddDatagram(bool istosend, XSTRING& address, XWORD port, XBYTE* data, XDWORD size)
{
  return AddDatagram(istosend, address.Get(), port, data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMUDP::GetFirstDatagram(bool tosend)
* @brief      Get first datagram
* @ingroup    DATAIO
* 
* @param[in]  tosend : Tosend value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMUDP::GetFirstDatagram(bool tosend)
{
  int index = DIOSTREAMUDP_NOTFOUND;

  if(datagramsmutex)  datagramsmutex->Lock();

  for(int c=0;c<(int)datagrams.GetSize();c++)
    {
      DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(c);
      if(datagram)
        {
          if(datagram->IsToSend() == tosend)
            {
              index = c;
              break;
            }
        }
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUDP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUDP::Clean()
{
  config          = NULL;
  host            = NULL;
  datagramsmutex  = NULL;
}





