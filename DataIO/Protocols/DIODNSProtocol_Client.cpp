/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIODNSProtocol_Client.cpp
*
* @class      DIODNSPROTOCOL_CLIENT
* @brief      Data Input/Output Domain Network System (DNS) protocol Client class
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

/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODNSProtocol_Client.cpp
* 
* @class      DIODNSPROTOCOL_CLIENT
* @brief      Data Input/Output Domain Network System (DNS) protocol Client class
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

#include "DIODNSProtocol_Client.h"

#include <stdio.h>
#include <stdlib.h>

#include "XBase.h"
#include "XFactory.h"
#include "XDateTime.h"
#include "XTrace.h"
#include "XTimer.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSPROTOCOLCLIENT::DIODNSPROTOCOLCLIENT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSPROTOCOLCLIENT::DIODNSPROTOCOLCLIENT()
{
  Clean();

  diostreamudpcfg = GEN_NEW DIOSTREAMUDPCONFIG();
  diostreamudp    = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO(diostreamudpcfg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSPROTOCOLCLIENT::~DIODNSPROTOCOLCLIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSPROTOCOLCLIENT::~DIODNSPROTOCOLCLIENT()
{
  if(diostreamudpcfg)
    {
      GEN_DELETE diostreamudpcfg;
      diostreamudpcfg = NULL;
    }

  if(diostreamudp)
    {
      GEN_DIOFACTORY.DeleteStreamIO(diostreamudp);
      diostreamudp    = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::SetServer(DIOIP& serverIP, XWORD serverport)
* @brief      Set server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP value.
* @param[in]  serverport : Serverport value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::SetServer(DIOIP& serverIP, XWORD serverport)
{
  if(!serverIP.Get()) return false;

  this->serverIP.Set(serverIP.Get());

  this->serverport = serverport;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::SetServer(XCHAR* serverIP, XWORD serverport)
* @brief      Set server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP pointer to use.
* @param[in]  serverport : Serverport value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::SetServer(XCHAR* serverIP, XWORD serverport)
{
  if(!serverIP) return false;

  if(!this->serverIP.Set(serverIP)) return false;

  this->serverport = serverport;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::SetServer(XSTRING& serverIP, XWORD serverport)
* @brief      Set server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP value.
* @param[in]  serverport : Serverport value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::SetServer(XSTRING& serverIP, XWORD serverport)
{
  return SetServer(serverIP.Get(), serverport);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::ResolveURL(XCHAR* URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::ResolveURL(XCHAR* URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  #define ASK_ID 0x55AA

  if(!diostreamudpcfg)  return false;

  DIOURL  url;  
  XSTRING serverIPDNSstring;
  XBUFFER askbuffer;
  XBUFFER answerbuffer;
  bool    status = false;

  url = URL;

  if(!url.IsAURL())
    {
      IPresolved.Set(url);
      return true;
    }

  serverIP.GetXString(serverIPDNSstring);

  diostreamudpcfg->SetMode(DIOSTREAMMODE_CLIENT);
  diostreamudpcfg->SetIsUsedDatagrams(false);
  diostreamudpcfg->GetRemoteURL()->Set(serverIPDNSstring);
  diostreamudpcfg->SetRemotePort(serverport?serverport:DIODNSPROTOCOL_DEFAULTPORT);
  
  if(diostreamudp->Open())
    {
      DIODNSPROTOCOL_HEADER     askheaderDNS; 
      XSTRING                   askURL;
      XBUFFER                   EncodedURL;
      DIODNSPROTOCOL_QUESTION   question;

      askheaderDNS.id           = ASK_ID;
      askheaderDNS.qr           = 0;  //  This is a query
      askheaderDNS.opcode       = 0;  //  This is a standard query
      askheaderDNS.aa           = 0;  //  Not Authoritative
      askheaderDNS.tc           = 0;  //  This message is not truncated
      askheaderDNS.rd           = 1;  //  Recursion Desired
      askheaderDNS.ra           = 0;  //  Recursion not available! hey we dont have it (lol)
      askheaderDNS.z            = 0;
      askheaderDNS.ad           = 0;
      askheaderDNS.cd           = 0;
      askheaderDNS.rcode        = 0;
      askheaderDNS.q_count      = SwapWORD(1);  // htons(1); //we have only 1 question
      askheaderDNS.ans_count    = 0;
      askheaderDNS.auth_count   = 0;
      askheaderDNS.add_count    = 0;

      askbuffer.Add((XBYTE*)&askheaderDNS, sizeof(DIODNSPROTOCOL_HEADER));
      
      askURL.Set(URL);
      EncodeDNSFormat(askURL, EncodedURL);
      askbuffer.Add(EncodedURL);
   
      question.qtype  = SwapWORD(querytype);   // htons(querytype);
      question.qclass = SwapWORD(1);           // htons(1);

      askbuffer.Add((XBYTE*)&question, sizeof(DIODNSPROTOCOL_QUESTION));

      status = diostreamudp->Write(askbuffer);
      if(status) 
        {
          status = diostreamudp->WaitToFlushOutXBuffer(3);
        }

      if(status)
        {
          answerbuffer.Empty();
          answerbuffer.Resize(65535);

          status = diostreamudp->WaitToFilledReadingBuffer(sizeof(DIODNSPROTOCOL_HEADER) + sizeof(RES_RECORD), timeout);
          if(status) 
            {
              status = diostreamudp->Read(answerbuffer);
            }
        }

      if(status)
        {
          status = false;

          DIODNSPROTOCOL_HEADER* answerheaderDNS = (DIODNSPROTOCOL_HEADER*)answerbuffer.Get();
          if(answerheaderDNS)
            {
              if(answerheaderDNS->id == ASK_ID)
                {
                  int answerscount = SwapWORD(answerheaderDNS->ans_count);   
                 
                  for(int c=0; c<answerscount; c++)
                    {
                      RES_RECORD* result = GEN_NEW RES_RECORD();
                      if(result)
                        {
                          XBYTE* reader = &answerbuffer.Get()[askbuffer.GetSize()];
                          int    type;
                          int    stop   = 0;

                          result->name      = NULL;
                          result->resource  = NULL;
                          result->rdata     = NULL;

                          result->name  = GetBufferName(reader, answerbuffer.Get(), &stop);
                          reader += stop;

                          result->resource = (DIODNSPROTOCOL_R_DATA*)reader;
                          reader += sizeof(DIODNSPROTOCOL_R_DATA);

                          type = SwapWORD(result->resource->type);

                          if(type == 1)
                            {
                              IPresolved.Set((XBYTE*)reader);
                              reader = reader +  SwapWORD(result->resource->data_len);  // ntohs(result->resource->data_len);
                              status = true;
                            }
                            else
                            {
                              result->rdata = GetBufferName(reader, answerbuffer.Get(), &stop);
                              reader += stop;

                              XSTRING URLmore;

                              URLmore = (char*)result->rdata;

                              status = ResolveURL((XCHAR*)URLmore.Get(), IPresolved, querytype, timeout);
                            }

                          if(result->name)
                            {
                              GEN_DELETE_ARRAY result->name;
                              result->name = NULL;
                            }

                          if(result->rdata)
                            {
                              GEN_DELETE_ARRAY result->rdata;
                              result->rdata = NULL;
                            }

                          GEN_DELETE result;

                          if(status) break;                                               
                        }
                    }  
                }            
            }
        }

      diostreamudp->Close();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::ResolveURL(DIOURL& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::ResolveURL(DIOURL& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  return ResolveURL(URL.Get(), IPresolved, querytype, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::ResolveURL(XSTRING& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::ResolveURL(XSTRING& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  return ResolveURL(URL.Get(), IPresolved, querytype, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::EncodeDNSFormat(XSTRING& URL, XBUFFER& EncodedURL)
* @brief      Encode DNS format
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  EncodedURL : Encoded URL value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::EncodeDNSFormat(XSTRING& URL, XBUFFER& EncodedURL) 
{
  int subsize = 0;

  if(URL.IsEmpty())
    {
      return false;
    }
  
  for(XDWORD c=0; c<URL.GetSize(); c++)
    {
      for(XDWORD d=c; d<URL.GetSize(); d++)
        {
          if(URL.Get()[d] != _C('.'))
            {
              subsize++;  
            }
           else 
            {
              break;
            } 
        }

      EncodedURL.Add((XBYTE)subsize);

      for(int d=0; d<subsize; d++)
        {
          EncodedURL.Add((XBYTE)URL.Get()[c]);
          c++;
        }  

      subsize = 0;
    }

  EncodedURL.Add((XBYTE)0x00);
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::DecodeDNSFormat(XBUFFER& EncodedURL, XSTRING& URL)
* @brief      Decode DNS format
* @ingroup    DATAIO
* 
* @param[in]  EncodedURL : Encoded URL value.
* @param[in]  URL : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::DecodeDNSFormat(XBUFFER& EncodedURL, XSTRING& URL) 
{ 
  URL.Empty();

  int size  = EncodedURL.GetSize();
  if(!size)
    {
      return false;
    }
  
  int c=0;
  while(c < size)
    {
      int count = EncodedURL.Get()[c];

      if(!count)
        {
          break;
        }

      c++;

      for(int d=0; d<count; d++) 
        {
          URL.Add(EncodedURL.Get()[c]);
          c++;
        }

      if(EncodedURL.Get()[c])
        {
          URL.Add(_C('.'));              
        }
    }  
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOLCLIENT::DecodeDNSFormat(XBYTE* EncodedURL, XSTRING& URL)
* @brief      Decode DNS format
* @ingroup    DATAIO
* 
* @param[in]  EncodedURL : Encoded URL pointer to use.
* @param[in]  URL : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOLCLIENT::DecodeDNSFormat(XBYTE* EncodedURL, XSTRING& URL)
{
  XBUFFER _EncodedURL;
  int     size = (int)strlen((char*)EncodedURL);

  _EncodedURL.Add(EncodedURL, size);
  
  return DecodeDNSFormat(_EncodedURL, URL); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* DIODNSPROTOCOLCLIENT::GetBufferName(XBYTE* reader, XBYTE* buffer, int* count)
* @brief      Get buffer name
* @ingroup    DATAIO
* 
* @param[in]  reader : Reader pointer to use.
* @param[in]  buffer : Buffer to use.
* @param[in]  count : Count pointer to use.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* DIODNSPROTOCOLCLIENT::GetBufferName(XBYTE* reader, XBYTE* buffer, int* count)
{
  XBYTE*  name;
  XDWORD  p       = 0;
  XDWORD  jumped  = 0;
  XDWORD  offset;
  int     i;
  int     j;

  *count = 1;
  name = GEN_NEW XBYTE[_MAXSTR];
  if(!name) return NULL;

  name[0] = '\0';

  //read the names in 3www6google3com format
  while(*reader != 0)
    {
      if(*reader >= 192)
        {
          offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
          reader = buffer + offset - 1;
          jumped = 1; //we have jumped to another location so counting wont go up!
        }
       else
        {
          name[p++] = *reader;
        }

     reader = reader + 1;

     if(jumped == 0)
       {
         *count = *count + 1; //if we havent jumped to another location then we can count up
       }
    }

  name[p] = '\0'; //string complete
  if(jumped==1)
    {
      *count = *count + 1; //number of steps we actually moved forward in the packet
    }

  //now convert 3www6google3com0 to www.google.com
  for(i=0; i<(int)strlen((const char*)name); i++)
    {
      p = name[i];

      for(j=0; j<(int)p; j++)
        {
          name[i]=name[i+1];
          i = i + 1;
        }

      name[i]='.';
    }

  name[i-1]='\0'; //remove the last dot

  return name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSPROTOCOLCLIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSPROTOCOLCLIENT::Clean()
{
  diostreamudpcfg = NULL;
  diostreamudp    = NULL;

  serverport      = 0;
}


