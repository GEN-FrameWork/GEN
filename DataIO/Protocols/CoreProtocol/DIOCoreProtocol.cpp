/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol.cpp
* 
* @class      DIOCOREPROTOCOL
* @brief      Data Input/Output Core Protocol class
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

#include "DIOCoreProtocol.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XString.h"
#include "XFileJSON.h"
#include "XDateTime.h"

#include "HashCRC32.h"

#include "CompressManager.h"

#include "DIOStream.h"

#include "DIOCoreProtocol_Header.h"
#include "DIOCoreProtocol_CFG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_COMMAND::DIOCOREPROTOCOL_COMMAND()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_COMMAND::DIOCOREPROTOCOL_COMMAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_COMMAND::~DIOCOREPROTOCOL_COMMAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_COMMAND::~DIOCOREPROTOCOL_COMMAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOCOREPROTOCOL_COMMAND::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOCOREPROTOCOL_COMMAND::GetType()
{
  return type;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_COMMAND::SetType(int type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_COMMAND::SetType(int type)
{
  this->type = type;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_COMMAND::GetTypeString()
* @brief      Get type string
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_COMMAND::GetTypeString()
{
  return &typestr;
}
    
    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_BIDIRECTIONALITYMODE DIOCOREPROTOCOL_COMMAND::GetBidirectionalityMode()
* @brief      Get bidirectionality mode
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_BIDIRECTIONALITYMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_BIDIRECTIONALITYMODE DIOCOREPROTOCOL_COMMAND::GetBidirectionalityMode()
{
  return bidirectionalitymode;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_COMMAND::SetBidirectionalityMode(DIOCOREPROTOCOL_BIDIRECTIONALITYMODE bidirectionalitymode)
* @brief      Set bidirectionality mode
* @ingroup    DATAIO
* 
* @param[in]  bidirectionalitymode : Bidirectionalitymode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_COMMAND::SetBidirectionalityMode(DIOCOREPROTOCOL_BIDIRECTIONALITYMODE bidirectionalitymode)
{
  this->bidirectionalitymode = bidirectionalitymode;
}

   
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_COMMAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_COMMAND::Clean()
{
  type                  = DIOCOREPROTOCOL_COMMAND_TYPE_UNKNOWN;
  typestr.Empty();   
  bidirectionalitymode  = DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_UPDATECLASS::DIOCOREPROTOCOL_UPDATECLASS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_UPDATECLASS::DIOCOREPROTOCOL_UPDATECLASS()
{
  Clean();
  
  timerlastupdate = GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_UPDATECLASS::~DIOCOREPROTOCOL_UPDATECLASS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_UPDATECLASS::~DIOCOREPROTOCOL_UPDATECLASS()
{
  if(timerlastupdate)
    {
      GEN_XFACTORY.DeleteTimer(timerlastupdate);
    }

  Clean();
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_UPDATECLASS::GetClassName()
* @brief      Get class name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_UPDATECLASS::GetClassName()
{
  return &classname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_UPDATECLASS::InitCache()
* @brief      Init cache
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_UPDATECLASS::InitCache()
{
  if(classptr)
    {
      return false;
    }

  classptr->HasBeenChanged();
  classptr->SetHasBeenChanged(false);  

  return false;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSERIALIZABLE* DIOCOREPROTOCOL_UPDATECLASS::GetClassPtr()
* @brief      Get class ptr
* @ingroup    DATAIO
* 
* @return     XSERIALIZABLE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSERIALIZABLE* DIOCOREPROTOCOL_UPDATECLASS::GetClassPtr()
{
  return classptr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::SetClassPtr(XSERIALIZABLE* classptr)
* @brief      Set class ptr
* @ingroup    DATAIO
* 
* @param[in]  classptr : Classptr pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::SetClassPtr(XSERIALIZABLE* classptr)
{
  this->classptr = classptr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_UPDATECLASS::RequieredInitialUpdate()
* @brief      requiered initial update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_UPDATECLASS::RequieredInitialUpdate()
{
  return requieredinitialupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::SetRequieredInitialUpdate(bool requieredinitialupdate)
* @brief      set requiered initial update
* @ingroup    DATAIO
* 
* @param[in]  requieredinitialupdate : Requieredinitialupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::SetRequieredInitialUpdate(bool requieredinitialupdate)
{
  this->requieredinitialupdate = requieredinitialupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOCOREPROTOCOL_UPDATECLASS::GetNUpdates()
* @brief      get Nupdates
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOCOREPROTOCOL_UPDATECLASS::GetNUpdates()
{
  return nupdates;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::SetNUpdates(XQWORD nupdates)
* @brief      set Nupdates
* @ingroup    DATAIO
* 
* @param[in]  nupdates : Nupdates value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::SetNUpdates(XQWORD nupdates)
{
  this->nupdates = nupdates;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::AddOneToNUpdates()
* @brief      add one to Nupdates
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::AddOneToNUpdates()
{
  nupdates++;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_UPDATECLASS::GetTimeToUpdate()
* @brief      Get time to update
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_UPDATECLASS::GetTimeToUpdate()
{
  return timetoupdate;
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::SetTimeToUpdate(XDWORD timetoupdate)
* @brief      Set time to update
* @ingroup    DATAIO
* 
* @param[in]  timetoupdate : Timetoupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::SetTimeToUpdate(XDWORD timetoupdate)
{
  this->timetoupdate = timetoupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOCOREPROTOCOL_UPDATECLASS::GetTimerLastUpdate()
* @brief      Get timer last update
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOCOREPROTOCOL_UPDATECLASS::GetTimerLastUpdate()
{
  return timerlastupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_UPDATECLASS::GetFlags()
* @brief      get flags
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_UPDATECLASS::GetFlags()
{
  return flags;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::SetFlags(XDWORD flags)
* @brief      set flags
* @ingroup    DATAIO
* 
* @param[in]  flags : Flags value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::SetFlags(XDWORD flags)
{
  this->flags = flags;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_UPDATECLASS::IsFlag(XDWORD flag)
* @brief      is flag
* @ingroup    DATAIO
* 
* @param[in]  flag : Flag value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_UPDATECLASS::IsFlag(XDWORD flag)
{
  if((flags & flag) == flag) 
    {
      return true;
    }

  return false;
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_UPDATECLASS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_UPDATECLASS::Clean()
{  
  classname.Empty();   
  classptr                = NULL;
  
  requieredinitialupdate  = false;

  nupdates                = 0;
  timetoupdate            = 0;
  timerlastupdate         = NULL;

  flags                   = 0;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL::DIOCOREPROTOCOL(DIOCOREPROTOCOL_CFG* protocolCFG, DIOSTREAM* diostream)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  protocolCFG : Protocol configuration pointer to use.
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL::DIOCOREPROTOCOL(DIOCOREPROTOCOL_CFG* protocolCFG, DIOSTREAM* diostream)
{
  Clean();

  compressmanager = GEN_NEW COMPRESSMANAGER();
  if(compressmanager)
    {
	    compressor = compressmanager->Create(COMPRESSBASE_TYPE_GZ);
      if(!compressor)
        {
          GEN_DELETE compressmanager;          
        }
    }  

  base64headermagic.Format(_L("%c%c%c%c"), ((DIOCOREPROTOCOL_HEADER_MAGIC_ID >> 24) & 0x000000FF), ((DIOCOREPROTOCOL_HEADER_MAGIC_ID >> 16) & 0x000000FF), ((DIOCOREPROTOCOL_HEADER_MAGIC_ID >> 8) & 0x000000FF), (DIOCOREPROTOCOL_HEADER_MAGIC_ID & 0x000000FF));
 

  this->protocolCFG = protocolCFG;
  this->diostream   = diostream;  

  Commands_Add(DIOCOREPROTOCOL_COMMAND_TYPE_HEARTBEAT             , DIOCOREPROTOCOL_COMMAND_TYPE_STRING_HEARTBEAT             , DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_BOTH);
  Commands_Add(DIOCOREPROTOCOL_COMMAND_TYPE_DISCONNECT            , DIOCOREPROTOCOL_COMMAND_TYPE_STRING_DISCONNECT            , DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_BOTH);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL::~DIOCOREPROTOCOL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL::~DIOCOREPROTOCOL()
{
  Commands_DeleteAll();

  UpdateClass_DeleteAll();

  if(compressmanager)
    {
      GEN_DELETE compressmanager;
    }

  if(compressor)
    {
      GEN_DELETE compressor;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::Ini()
{  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::End()
{  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CFG* DIOCOREPROTOCOL::GetProtocolCFG()
* @brief      Get protocol CFG
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_CFG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CFG* DIOCOREPROTOCOL::GetProtocolCFG()
{
  return protocolCFG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM* DIOCOREPROTOCOL::GetDIOStream()
* @brief      Get DIO stream
* @ingroup    DATAIO
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOCOREPROTOCOL::GetDIOStream()
{
  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL::SetDIOStream(DIOSTREAM* diostream)
* @brief      Set DIO stream
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL::SetDIOStream(DIOSTREAM* diostream)
{
  this->diostream = diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL::SendMsg(DIOCOREPROTOCOL_HEADER* header, XBUFFER& contentresult)
* @brief      send msg
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* @param[in]  contentresult : Output contentresult.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL::SendMsg(DIOCOREPROTOCOL_HEADER* header, XBUFFER& contentresult)
{
  XBUFFER senddata; 
  XWORD   headersize;
  bool    status   = false;

  if(!header)
    {
      return 0;
    }
    
  if(!GenerateHeaderToSend(header, senddata, &headersize))
    {
      return 0;
    }
           
  senddata.Add(contentresult);

  XDWORD size = senddata.GetSize();
  if(size)
    {
      if(!SendData(senddata))
        {
          return 0;
        }
    }

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL::ReceivedMsg(DIOCOREPROTOCOL_HEADER& header, XBUFFER& content)
* @brief      Received msg
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* @param[in]  content : Content value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL::ReceivedMsg(DIOCOREPROTOCOL_HEADER& header, XBUFFER& content)
{
  if(!protocolCFG)
    {
      return 0;
    }

  if(!diostream)
    {
      return 0;
    }

  XBUFFER* readbuffer = diostream->GetInXBuffer();
  if(!readbuffer)
    {
      return 0;
    }
  
  readbuffer->ResetPosition();

  if(readbuffer->GetSize() < 100)
    {
      return 0;
    }

  XDWORD    start      = 0;
  XDWORD    sizeread   = 0;
  XDWORD    offsetini  = 0;
  XDWORD    index      = 0;
  XBUFFER   dataread; 
  XDWORD    base64size = 0;
  bool      status     = false;
  
  dataread.Empty();  
  dataread.CopyFrom((*readbuffer));

  for(offsetini=0; offsetini<dataread.GetSize()-sizeof(XDWORD); offsetini++)
    {
      if(dataread.Get((XDWORD&)start, offsetini))
        {
           if(start == DIOCOREPROTOCOL_HEADER_MAGIC_ID)
             {
                if(offsetini)  
                  {
                    readbuffer->Extract(NULL, 0, dataread.GetSize()-offsetini);
                    dataread.CopyFrom((*readbuffer));                    
                  }
             
               break;
             }
        }
    }
 
  if(dataread.IsEmpty())
    {
      readbuffer->Extract(NULL, 0, dataread.GetSize());
      return 0;  
    }

  if(protocolCFG->GetIsEncapsulatedBase64())
    {
      int       sizeprelude = 22;
      XSTRING   base64senddata;
      XSTRING   base64header;
      XSTRING   base64headermask;
      XSTRING   extract;
      HASHCRC32 hashCRC32;
      XDWORD    CRC32value  = 0;

      base64senddata.ConvertFromUTF8(dataread);

      base64headermask.Format(_L("%s"), base64headermagic.Get()); 
      base64senddata.Copy(0, sizeprelude, base64header);
    
      base64header.Copy(0, 4, extract);
      if(extract.Compare(base64headermask, true))
        {
          return 0;
        }

      base64header.Copy(4, 14, extract);
      if(!extract.UnFormat(_L("%10d"), &base64size))
        {
          return 0;
        }

      base64header.Copy(14, 22, extract);
      if(!extract.UnFormat(_L("%08x"), &CRC32value))
        {
          return 0;
        }

      // ------------------------------------------------------------------------------------------------------------------------
                  
      /*
      XBUFFER debugdata;
      base64senddata.ConvertToASCII(debugdata);
      debugdata.Resize(debugdata.GetSize()-1);
      */
            
      // ------------------------------------------------------------------------------------------------------------------------          
      
      base64senddata.DeleteCharacters(0, sizeprelude);
      if(base64senddata.GetSize() < base64size)
        {
          return 0;
        } 

      // ------------------------------------------------------------------------------------------------------------------------
            
      /*
      XBYTE color = XTRACE_COLOR_GREEN;

      XTRACE_PRINTCOLOR(color, _L("READ < < < < < < < < < < < <"));   
      XTRACE_PRINTDATABLOCKCOLOR(color, debugdata); 
      */           
      // ------------------------------------------------------------------------------------------------------------------------

      base64senddata.DeleteCharacters(base64size, base64senddata.GetSize() - base64size); 
      if(base64senddata.GetSize() != base64size)
        {
          return 0;
        } 

      dataread.Empty();
      if(!base64senddata.ConvertBase64ToBinary(dataread))
        {
          return 0;
        } 

      hashCRC32.Do(dataread);  
      if(CRC32value != hashCRC32.GetResultCRC32())
        {
          readbuffer->Extract(NULL, 0, sizeprelude);  
          return 0;
        }
      
      base64size += sizeprelude;
    }
   
  if(dataread.Get((XDWORD&)start, 0))
    {
      sizeread += sizeof(XDWORD);
      index    += sizeof(XDWORD); 

      // Compress Data
      if(start == DIOCOREPROTOCOL_HEADER_MAGIC_ID)
        {
          XBUFFER headerbuffer;
          XWORD   sizeheader    = 0;
          XWORD   sizeheadercmp = 0;
          XDWORD  crc32header   = 0;
          int     totalsizemsg  = 0;

          if(dataread.Get((XWORD&)sizeheader, index))
            {
              sizeread += sizeof(XWORD);
              index    += sizeof(XWORD); 

              if(dataread.Get((XWORD&)sizeheadercmp, index))
                {
                  sizeread += sizeof(XWORD);
                  index    += sizeof(XWORD);
                            
                  if(sizeheadercmp)
                    {     
                      totalsizemsg += sizeheadercmp;
                    }
                    else
                    {
                      totalsizemsg += sizeheader;
                    } 

                  if(totalsizemsg > dataread.GetSize())
                    {
                      return 0;  
                    }  

                  if(dataread.Get((XDWORD&)crc32header, index))
                    {
                      sizeread += sizeof(XDWORD);
                      index    += sizeof(XDWORD);
  
                      if(sizeheader)
                        {       
                          XBYTE*    ptrheader       = &dataread.Get()[DIOCOREPROTOCOL_HEADER_SIZE_ID];
                          XBYTE*    ptrcontent      = ptrheader;                           
                          HASHCRC32 hashCRC32;
                          XDWORD    crc32headercalc = 0;
  
                          if(sizeheadercmp)
                            {     
                              hashCRC32.Do(ptrheader, sizeheadercmp); 
                            }
                            else
                            {
                              hashCRC32.Do(ptrheader, sizeheader); 
                            } 

                          crc32headercalc = hashCRC32.GetResultCRC32();
   
                          if(crc32headercalc == crc32header)
                            {
                              if(sizeheadercmp)
                                {
                                  headerbuffer.Resize(sizeheader);                                        
                                  status      = compressor->Decompress(ptrheader, sizeheadercmp, &headerbuffer);
                                  ptrcontent += sizeheadercmp;
                                  sizeread   += sizeheadercmp;

                                  if(!status)
                                    {
                                      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: to decompress header"));
                                    }
                                }
                               else
                                {
                                  status      = headerbuffer.Add(ptrheader, sizeheader);
                                  ptrcontent += sizeheader;
                                  sizeread   += sizeheader;

                                  if(!status)
                                    {
                                      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: to get header"));
                                    }
                                }

                              XFILEJSON* headerxfileJSON = header.GetSerializationXFileJSON();
                              if(headerxfileJSON)
                                {                                                                                                
  
                                  header.GetSerializationXFileJSON()->AddBufferLines(XFILETXTFORMATCHAR_UTF8, headerbuffer);
                                  header.GetSerializationXFileJSON()->DecodeAllLines();
                                  header.DoDeserialize();
                                }                               
                                                                      
                              if(header.GetContentCompressSize())
                                {
                                  totalsizemsg += header.GetContentCompressSize();
                                }
                               else
                                {
                                  totalsizemsg += header.GetContentSize();
                                }   

                              if(totalsizemsg > dataread.GetSize())
                                {
                                  return 0;  
                                }  

                              if(header.GetContentCompressSize() || header.GetContentSize())
                                {
                                  HASHCRC32 hashCRC32;
                                  XDWORD    crc32contentcalc = 0;

                                  hashCRC32.Do(ptrcontent, header.GetContentCompressSize()?header.GetContentCompressSize():header.GetContentSize());  

                                  crc32contentcalc = hashCRC32.GetResultCRC32();
                              
                                  if(header.GetContentCRC32() == crc32contentcalc)
                                    {                                              
                                      if(header.GetContentCompressSize())
                                        {
                                          content.Resize(header.GetContentSize());
                                          status    = compressor->Decompress(ptrcontent, header.GetContentCompressSize(), &content);
                                          sizeread += header.GetContentCompressSize();

                                          if(!status)
                                            {
                                              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: to decompress content"));
                                            }
                                        }
                                       else
                                        {
                                          if(header.GetContentSize())
                                            {
                                              status = content.Add(ptrcontent, header.GetContentSize());
                                              sizeread += header.GetContentSize();

                                              if(!status)
                                                {
                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: to get content"));
                                                }
                                            }
                                        }
                                    }
                                }
                               else
                                {
                                  if(!header.GetContentCRC32())
                                    {
                                      status =  true;
                                    }
                                } 
                            }

                          // ------------------------------------------------------------------------------------------------------------------------
                          /*
                          XBYTE color = XTRACE_COLOR_GREEN;
                          if(GetProtocolCFG()->GetIsServer())
                            {
                              color = XTRACE_COLOR_BLUE;      
                            }

                          XTRACE_PRINTCOLOR(color, _L("READ -----------------------------------------------------------------------------------------------"));   
                          XTRACE_PRINTDATABLOCKCOLOR(color, dataread.Get(), sizeread +  index); 
                          */
                          // ------------------------------------------------------------------------------------------------------------------------
                          
                          if(protocolCFG->GetIsEncapsulatedBase64())
                            {
                              status = readbuffer->Extract(NULL, 0, base64size);  
                              if(!status)
                                {
                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: eliminate buffer used: %d"), base64size);
                                }
                            }
                           else 
                            {
                              status = readbuffer->Extract(NULL, 0, sizeread + offsetini);  
                              if(!status)
                                {
                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: eliminate buffer used: %d + %d"), sizeread, index);
                                }
                            }
                     
                          return sizeread;  
                        }
                       else
                        {
                          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: not read size header")); 
                        }           
                    }
                   else
                    {
                      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: not read header CRC")); 
                    }           
                }
               else
                {
                  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: not read size header cmp")); 
                }           
            }
           else
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: not read size header")); 
            }           
        }
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: not found magic")); 
        }           
    }
   else
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[DIO Core Protocol Header] Error to read msg: read magic")); 
    }
   
  return sizeread;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param)
* @brief      Create header
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* 
* @return     DIOCOREPROTOCOL_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param)
{
  DIOCOREPROTOCOL_HEADER* header = GEN_NEW DIOCOREPROTOCOL_HEADER();
  if(!header)
    {
      return NULL;
    }

  bool IDmsgempty = false;

  if(ID_message)
    {    
      if(ID_message->IsEmpty())
        {
          IDmsgempty = true;
        }
    }
   else
    {
      return NULL;
    }

  if(IDmsgempty)
    {
      header->SetMessageType(DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST);
      header->GetIDMessage()->GenerateRandom();

      ID_message->CopyFrom((*header->GetIDMessage()));      
    }
   else
    {      
      header->SetMessageType(DIOCOREPROTOCOL_HEADER_MESSAGETYPE_RESPONSE);
      header->GetIDMessage()->CopyFrom((*ID_message));
    }

  header->SetOperation(operation);
  header->GetOperationParam()->Set(operation_param);
  header->GetDateTimeSend()->Read();

  header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE);
  header->SetContentSize(0); 
  header->SetContentCRC32(0);

  return header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XBUFFER* content, XBUFFER* contentresult)
* @brief      Create header
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* @param[in]  contentresult : Output contentresult.
* 
* @return     DIOCOREPROTOCOL_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XBUFFER* content, XBUFFER* contentresult)
{
  DIOCOREPROTOCOL_HEADER* header = CreateHeader(ID_message, operation, operation_param);
  if(!header)
    {
      return NULL;
    }

  bool havecontent = false;

  if(content)
    {
      if(!content->IsEmpty())
        {  
          header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY);
          header->SetContentSize(content->GetSize()); 

          if(!CompressContent(header, (*content), (*contentresult)))
            {                  
              contentresult->CopyFrom((*content));
            }   
  
          HASHCRC32 hashCRC32;

          hashCRC32.Do((*contentresult));  
          header->SetContentCRC32(hashCRC32.GetResultCRC32());

          havecontent = true;
        }
    }

  if(!havecontent)
    {
      header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY);
      header->SetContentSize(0); 
      header->SetContentCRC32(0);
    }

  return header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XSTRING* content, XBUFFER* contentresult)
* @brief      Create header
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* @param[in]  contentresult : Output contentresult.
* 
* @return     DIOCOREPROTOCOL_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XSTRING* content, XBUFFER* contentresult)
{
  DIOCOREPROTOCOL_HEADER* header = CreateHeader(ID_message, operation, operation_param);
  if(!header)
    {
      return NULL;
    }  

  bool havecontent = false;

  if(content)
    {
      if(!content->IsEmpty())
        {     
          XBUFFER contentbinary;
          content->ConvertToUTF8(contentbinary, false);

          header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT);
          header->SetContentSize(contentbinary.GetSize());   

          if(!CompressContent(header, contentbinary, (*contentresult)))
            {                  
              contentresult->CopyFrom(contentbinary);
            }   

          HASHCRC32 hashCRC32;

          hashCRC32.Do((*contentresult));  
          header->SetContentCRC32(hashCRC32.GetResultCRC32());

          havecontent = true;
        }
    }
   
  if(!havecontent)
    {
      header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT);
      header->SetContentSize(0);   
      header->SetContentCRC32(0);
    }

  return header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XFILEJSON* content, XBUFFER* contentresult)
* @brief      Create header
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* @param[in]  contentresult : Output contentresult.
* 
* @return     DIOCOREPROTOCOL_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_HEADER* DIOCOREPROTOCOL::CreateHeader(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XFILEJSON* content, XBUFFER* contentresult)
{
  DIOCOREPROTOCOL_HEADER* header = CreateHeader(ID_message, operation, operation_param);
  if(!header)
    {
      return NULL;
    }  

  bool havecontent = false;

  if(content)
    {
      content->EncodeAllLines(false);  

      if(!content->GetLines()->IsEmpty())
        {  
          XSTRING contentstring;
          XBUFFER contentbinary;   

          content->GetAllInOneLine(contentstring);
          contentstring.ConvertToUTF8(contentbinary, false);

          header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON);
          header->SetContentSize(contentbinary.GetSize());   

          if(!CompressContent(header, contentbinary, (*contentresult)))
            {                  
              contentresult->CopyFrom(contentbinary);
            }   

          HASHCRC32 hashCRC32;

          hashCRC32.Do((*contentresult));  
          header->SetContentCRC32(hashCRC32.GetResultCRC32());

          havecontent = true;
        }
    }

  if(!havecontent)
    {
      header->SetContentType(DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON);
      header->SetContentSize(0);   
      header->SetContentCRC32(0);
    }

  return header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::GenerateAuthenticationChallenge(XBUFFER& autentication_challange)
* @brief      Generate authentication challenge
* @ingroup    DATAIO
* 
* @param[in]  autentication_challange : Autentication challange value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::GenerateAuthenticationChallenge(XBUFFER& autentication_challange)
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::GenerateAuthenticationResponse(XBUFFER& autentication_challange, XBUFFER& autentication_response)
* @brief      Generate authentication response
* @ingroup    DATAIO
* 
* @param[in]  autentication_challange : Autentication challange value.
* @param[in]  autentication_response : Autentication response value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::GenerateAuthenticationResponse(XBUFFER& autentication_challange, XBUFFER& autentication_response)
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::MaskKey(XBYTE* key, int size, XBYTE mask)
* @brief      Mask key
* @ingroup    DATAIO
* 
* @param[in]  key : Key pointer to use.
* @param[in]  size : Size value.
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::MaskKey(XBYTE* key, int size, XBYTE mask)
{
  if(!key)
    {
      return false;
    }

  for(XDWORD c=0; c<size; c+=2)
    {    
      key[c]   ^= (0xAA + mask);
      key[c+1] ^= (0x55 + mask);
    }
      
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOCOREPROTOCOL_COMMAND*>* DIOCOREPROTOCOL::Commands_GetAll()
* @brief      Commands get all
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOCOREPROTOCOL_COMMAND*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOCOREPROTOCOL_COMMAND*>* DIOCOREPROTOCOL::Commands_GetAll()
{
  return &commands;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::Commands_Add(XDWORD type, XCHAR* command, DIOCOREPROTOCOL_BIDIRECTIONALITYMODE bidirectionalitymode)
* @brief      Commands add
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  command : Command pointer to use.
* @param[in]  bidirectionalitymode : Bidirectionalitymode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::Commands_Add(XDWORD type, XCHAR* command, DIOCOREPROTOCOL_BIDIRECTIONALITYMODE bidirectionalitymode)
{
  if(!command)
    {
      return false;
    }

  if(Commands_Get(command))
    {
      return false;
    }
  
  if(Commands_Get(type))
    {
      return false;
    }


  DIOCOREPROTOCOL_COMMAND* coreprotocolcommand = GEN_NEW DIOCOREPROTOCOL_COMMAND();
  if(!coreprotocolcommand)
    {
      return false;
    }
  
  coreprotocolcommand->SetType(type);
  coreprotocolcommand->GetTypeString()->Set(command);
  coreprotocolcommand->SetBidirectionalityMode(bidirectionalitymode);

  return commands.Add(coreprotocolcommand);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL::Commands_Get(XCHAR* command)
* @brief      Commands get
* @ingroup    DATAIO
* 
* @param[in]  command : Command pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL::Commands_Get(XCHAR* command)
{
  if(!command)
    {
      return false;
    }

  for(XDWORD c=0; c<commands.GetSize(); c++)
    {
      DIOCOREPROTOCOL_COMMAND* coreprotocolcommand = commands.Get(c);
      if(coreprotocolcommand)
        {
          if(!coreprotocolcommand->GetTypeString()->Compare(command, true))
            {
               return c; 
            }
        }
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOCOREPROTOCOL::Commands_Get(XDWORD type)
* @brief      Commands get
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOCOREPROTOCOL::Commands_Get(XDWORD type)
{
  if(!type)
    {
      return NULL;
    }

  for(XDWORD c=0; c<commands.GetSize(); c++)
    {
      DIOCOREPROTOCOL_COMMAND* coreprotocolcommand = commands.Get(c);
      if(coreprotocolcommand)
        {
          if(coreprotocolcommand->GetType() == type)
            {
               return coreprotocolcommand->GetTypeString()->Get(); 
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_COMMAND* DIOCOREPROTOCOL::Commands_GetCoreProtocol(XDWORD type)
* @brief      Commands get core protocol
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* @return     DIOCOREPROTOCOL_COMMAND* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_COMMAND* DIOCOREPROTOCOL::Commands_GetCoreProtocol(XDWORD type)
{
  if(!type)
    {
      return NULL;
    }

  for(XDWORD c=0; c<commands.GetSize(); c++)
    {
      DIOCOREPROTOCOL_COMMAND* coreprotocolcommand = commands.Get(c);
      if(coreprotocolcommand)
        {
          if(coreprotocolcommand->GetType() == type)
            {
               return coreprotocolcommand; 
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::Commands_DeleteAll()
* @brief      Commands GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::Commands_DeleteAll()
{
  if(commands.IsEmpty())
    {
      return false;
    }
    
  commands.DeleteContents();
  commands.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOCOREPROTOCOL_UPDATECLASS*>* DIOCOREPROTOCOL::UpdateClass_GetAll()
* @brief      Update class get all
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOCOREPROTOCOL_UPDATECLASS*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOCOREPROTOCOL_UPDATECLASS*>*  DIOCOREPROTOCOL::UpdateClass_GetAll()
{
  return &updateclasses;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::UpdateClass_Add(XCHAR* classname, XSERIALIZABLE* classptr, bool requieredinitialupdate, XDWORD timetoupdate, XDWORD flags)
* @brief      update class  add
* @ingroup    DATAIO
* 
* @param[in]  classname : Classname pointer to use.
* @param[in]  classptr : Classptr pointer to use.
* @param[in]  requieredinitialupdate : Requieredinitialupdate value.
* @param[in]  timetoupdate : Timetoupdate value.
* @param[in]  flags : Flags value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::UpdateClass_Add(XCHAR* classname, XSERIALIZABLE* classptr, bool requieredinitialupdate, XDWORD timetoupdate, XDWORD flags)
{
  if(!classname)
    {
      return false;
    }

  if(!classptr)
    {
      return false;
    }

  if(UpdateClass_Get(classname))
    {
      return false;
    }

  DIOCOREPROTOCOL_UPDATECLASS* updateclass =  GEN_NEW DIOCOREPROTOCOL_UPDATECLASS();
  if(!updateclass)
    {
      return false;
    }

  updateclass->GetClassName()->Set(classname);
  if(updateclass->GetClassName()->IsEmpty())
    {
      GEN_DELETE updateclass;
      return false;
    }

  updateclass->SetClassPtr(classptr);  
  updateclass->SetRequieredInitialUpdate(requieredinitialupdate);
  updateclass->SetTimeToUpdate(timetoupdate);
  updateclass->SetFlags(flags);
 
  updateclass->InitCache();

  return updateclasses.Add(updateclass);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_UPDATECLASS* DIOCOREPROTOCOL::UpdateClass_Get(XCHAR* classname)
* @brief      Update class get
* @ingroup    DATAIO
* 
* @param[in]  classname : Classname pointer to use.
* 
* @return     DIOCOREPROTOCOL_UPDATECLASS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_UPDATECLASS* DIOCOREPROTOCOL::UpdateClass_Get(XCHAR* classname)
{
  if(!classname)
    {
      return NULL;
    }

  for(XDWORD c=0; c<updateclasses.GetSize(); c++)
    {
      DIOCOREPROTOCOL_UPDATECLASS* coreprotocolupdateclass = updateclasses.Get(c);
      if(coreprotocolupdateclass)
        {
          if(!coreprotocolupdateclass->GetClassName()->Compare(classname, true))
            {
              return coreprotocolupdateclass; 
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::UpdateClass_DeleteAll()
* @brief      Update class GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::UpdateClass_DeleteAll()
{
  if(updateclasses.IsEmpty())
    {
      return false;
    }

  updateclasses.DeleteContents();
  updateclasses.DeleteAll();  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::ShowDebug(bool send, DIOCOREPROTOCOL_HEADER* header, XBUFFER& content, XDWORD sizeallmessage, bool showlongformat)
* @brief      Show debug
* @ingroup    DATAIO
* 
* @param[in]  send : Send value.
* @param[in]  header : Header pointer to use.
* @param[in]  content : Content value.
* @param[in]  sizeallmessage : Sizeallmessage value.
* @param[in]  showlongformat : Showlongformat value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::ShowDebug(bool send, DIOCOREPROTOCOL_HEADER* header, XBUFFER& content, XDWORD sizeallmessage, bool showlongformat)
{
  XBUFFER   contentresult;  
  int       colormsg = XTRACE_COLOR_PURPLE;
  XSTRING   title; 
 
  if(!header)
    {
      return false;
    }
 
  if(!send)
    {
      colormsg = XTRACE_COLOR_GREEN;
  
      title.Format(_L("[CoreProtocol] Read message [%5d bytes] : "), sizeallmessage); 
    }  
   else 
    {
      title.Format(_L("[CoreProtocol] Write message [%5d bytes] : "), sizeallmessage);  
    }

  if(!showlongformat)
    {
      XSTRING ID_message;
      XSTRING operationstring;

      header->GetIDMessage()->GetToString(ID_message);

      #ifdef DIOCOREPROTOCOL_HUMANFORMAT_ACTIVE
      header->GetOperationToString(operationstring);
      title.AddFormat(_L("%-20s %-15s [%s] "), ID_message.Get(), operationstring.Get(), header->GetOperationParam()->Get()); 
      #else
      title.AddFormat(_L("%-20s %03d [%s] "), ID_message.Get(), header->GetOperation(), header->GetOperationParam()->Get()); 
      #endif

      XTRACE_PRINTCOLOR(colormsg, title.Get()); 
    }
   else
    {
      XTRACE_PRINTCOLOR(colormsg, title.Get()); 

      if(header->GetSerializationXFileJSON()->GetRoot())
        {  
          if(!header->GetSerializationXFileJSON()->GetRoot()->GetValues()->GetSize())
            {
              header->Serialize();
            }
        }

      header->GetSerializationXFileJSON()->ShowTraceJSON(colormsg);

      if(header->GetContentCompressSize())
        {
          contentresult.Resize(header->GetContentSize());
          compressor->Decompress(content.Get(), header->GetContentCompressSize(), &contentresult);
        }
       else  
        {
          contentresult.CopyFrom(content);
        }  
  
      switch(header->GetContentType())
        {
          case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_NONE      : break;
  
          case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY    : XTRACE_PRINTDATABLOCKCOLOR(colormsg, contentresult);
                                                              break; 
  
          case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_TEXT      : { XSTRING string;
  
                                                                string.ConvertFromUTF8(contentresult);
                                                                XTRACE_PRINTCOLOR(colormsg, string.Get());
                                                              }
                                                              break;
  
          case DIOCOREPROTOCOL_HEADER_CONTENTTYPE_JSON      : { XFILEJSON xfileJSON;   
                                                            
                                                                xfileJSON.AddBufferLines(XFILETXTFORMATCHAR_UTF8, contentresult);
                                                                xfileJSON.DecodeAllLines();
                                                                xfileJSON.ShowTraceJSON(colormsg);                                                                  
                                                              }
                                                              break;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::GenerateHeaderToSend(DIOCOREPROTOCOL_HEADER* header, XBUFFER& headersenddata, XWORD* headersize)
* @brief      Generate header to send
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* @param[in]  headersenddata : Headersenddata value.
* @param[in]  headersize : Headersize pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::GenerateHeaderToSend(DIOCOREPROTOCOL_HEADER* header, XBUFFER& headersenddata, XWORD* headersize)
{
  XFILEJSON*  headerxfileJSON = NULL;
  XSTRING     headerstring;
  XBUFFER     headerbuffer;  
  XBUFFER     headercmp;
  bool        isheadercmp     = false;  
  bool        status          = false;
  
  if(!header)
    {
      return status;
    }

  headersenddata.Empty();

  headerxfileJSON = header->GetSerializationXFileJSON();
  if(!headerxfileJSON)
    {
      return false;
    }

  status = header->DoSerialize();
  if(!status)
    {    
      return status;
    }
 
  headerxfileJSON->EncodeAllLines(false);  
  headerxfileJSON->GetAllInOneLine(headerstring);

  headerstring.ConvertToUTF8(headerbuffer, false);

  if(protocolCFG)
    {
      if(protocolCFG->GetIsCompressHeader())
        {
          if(!compressor->Compress(headerbuffer.Get(), headerbuffer.GetSize(), &headercmp))
            {
              return status;        
            }

          if(headercmp.GetSize() < headerbuffer.GetSize())
            {
              isheadercmp = true;
            }
           else 
            {
              headercmp.Empty();    
            } 
        }
    }
      
  headersenddata.Add((XDWORD)DIOCOREPROTOCOL_HEADER_MAGIC_ID);          
  headersenddata.Add((XWORD)headerbuffer.GetSize()); 
  headersenddata.Add((XWORD)headercmp.GetSize()); 

  HASHCRC32 hashCRC32;
  
  if(isheadercmp)
    {     
      hashCRC32.Do(headercmp); 
    }
   else
    {
      hashCRC32.Do(headerbuffer); 
    } 
   
  headersenddata.Add((XDWORD)hashCRC32.GetResultCRC32());
              
  if(isheadercmp)
    {
      if(headersize)
        {
          (*headersize) = headercmp.GetSize();  
        }

      headersenddata.Add(headercmp); 
    }
   else
    {
      if(headersize)
        {
          (*headersize) = headerbuffer.GetSize();  
        }

      headersenddata.Add(headerbuffer); 
    } 

  return status;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::SendData(XBUFFER& senddata)
* @brief      Send data
* @ingroup    DATAIO
* 
* @param[in]  senddata : Senddata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::SendData(XBUFFER& senddata)
{
  bool occupated = false;
  bool status    = false;

  if(!protocolCFG)
    {
      return false;
    }

  if(!diostream)
    {
      return false;
    } 

  if(protocolCFG->BusMode_IsActive())
    {
      XTIMER* xtimer = GEN_XFACTORY.CreateTimer();
      if(xtimer)
        {
          xtimer->Reset();

          while(diostream->GetInXBuffer()->GetSize())
            {    
              if(xtimer->GetMeasureSeconds() > protocolCFG->BusMode_GetTimeOutBusFree())        
                {
                  occupated =  true;
                  break;
                }
            }

          GEN_XFACTORY.DeleteTimer(xtimer);
        }

      if(occupated)
        {
          return false;
        }
    }

  if(protocolCFG->GetIsEncapsulatedBase64())
    { 
      HASHCRC32 hashCRC32;
      XDWORD    CRC32value = 0;
      XSTRING   base64senddata;
      XSTRING   base64header;

      hashCRC32.Do(senddata);  
      CRC32value = hashCRC32.GetResultCRC32();

      base64senddata.ConvertBinaryToBase64(senddata);   

      base64header.Format(_L("%s%010d%08X"), base64headermagic.Get(), base64senddata.GetSize(), CRC32value); 
      base64senddata.Insert(base64header, 0);          
      base64senddata.ConvertToUTF8(senddata, false);
    }

  // ------------------------------------------------------------------------------------------------------------------------  
  
  /*
  XBYTE color = XTRACE_COLOR_PURPLE;

  XTRACE_PRINTCOLOR(color, _L("WRITE > > > > > > > > > > >"));     
  XTRACE_PRINTDATABLOCKCOLOR(color, senddata);   
  */
  
  // ------------------------------------------------------------------------------------------------------------------------
     
  status = diostream->Write(senddata);

  if(protocolCFG->BusMode_IsActive())
    {
      if(status)
        {
          status = diostream->WaitToFlushOutXBuffer(protocolCFG->BusMode_GetTimeOutSendData());
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL::CompressContent(DIOCOREPROTOCOL_HEADER* header, XBUFFER& content, XBUFFER& contentresult)
* @brief      Compress content
* @ingroup    DATAIO
* 
* @param[in]  header : Header pointer to use.
* @param[in]  content : Content value.
* @param[in]  contentresult : Output contentresult.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL::CompressContent(DIOCOREPROTOCOL_HEADER* header, XBUFFER& content, XBUFFER& contentresult)
{
  bool status = false;

  if(protocolCFG)
    {    
      if(protocolCFG->GetIsCompressContent())
        {
          if(content.GetSize() >= protocolCFG->GetMinSizeCompressContent())
            {
              if(compressor->Compress(content.Get(), content.GetSize(), &contentresult))
                {
                  if(content.GetSize() > contentresult.GetSize()) 
                    {
                      header->SetContentCompressSize(contentresult.GetSize());   
                      status = true; 
                    }
                }    
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL::Clean()
{
  protocolCFG                           = NULL;
  diostream                             = NULL;
  
  compressmanager                       = NULL;
  compressor                            = NULL;    	
  
  initialization                        = false;
}




