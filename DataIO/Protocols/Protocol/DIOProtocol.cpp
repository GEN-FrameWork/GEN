/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOProtocol.h"

#include <stdarg.h>
#include <string.h>

#include "XSleep.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XRand.h"
#include "XFile.h"
#include "XSystem.h"
#include "XFactory.h"
#include "XString.h"
#include "XThreadCollected.h"
#include "XTrace.h"
#include "XVariant.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOStream.h"
#include "DIOProtocol_ConnectionsManager.h"
#include "DIOProtocolConnectionsInUseDefinitions.h"

#include "DIOProtocol_ApplicationData.h"
#include "DIOProtocol_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER::DIOPROTOCOL_ANSWER(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER::DIOPROTOCOL_ANSWER(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
{
  Clean();

  this->type    = type;
  this->ID      = ID;

  if(!xbuffer.IsEmpty()) this->xbuffer.Add(xbuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER::~DIOPROTOCOL_ANSWER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER::~DIOPROTOCOL_ANSWER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL_ANSWER::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL_ANSWER::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL_ANSWER::GetID()
* @brief      Get ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL_ANSWER::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER& DIOPROTOCOL_ANSWER::GetBuffer()
* @brief      Get buffer
* @ingroup    DATAIO
* 
* @return     XBUFFER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER& DIOPROTOCOL_ANSWER::GetBuffer()
{
  return xbuffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_ANSWER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_ANSWER::Clean()
{
  type      = DIOPROTOCOL_CMDTYPE_NONE;
  ID        = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_COMMAND::DIOPROTOCOL_COMMAND(XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  inmask : Inmask pointer to use.
* @param[in]  outmask : Output outmask.
* @param[in]  receivedfunc : Receivedfunc value.
* @param[in]  description : Description pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_COMMAND::DIOPROTOCOL_COMMAND(XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description)
{
  Clean();

  this->type    = type;
  this->inmask  = inmask;
  this->outmask = outmask;

  if(receivedfunc)  this->receivedfunc  = receivedfunc;
  if(description)   this->description   = description;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_COMMAND::~DIOPROTOCOL_COMMAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_COMMAND::~DIOPROTOCOL_COMMAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL_COMMAND::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL_COMMAND::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOPROTOCOL_COMMAND::GetINMask()
* @brief      Get IN mask
* @ingroup    DATAIO
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOPROTOCOL_COMMAND::GetINMask()
{
  return inmask.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOPROTOCOL_COMMAND::GetOUTMask()
* @brief      Get OUT mask
* @ingroup    DATAIO
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOPROTOCOL_COMMAND::GetOUTMask()
{
  return outmask.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_RECEIVEDFUNC DIOPROTOCOL_COMMAND::GetReceivedFunc()
* @brief      Get received func
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_RECEIVEDFUNC : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_RECEIVEDFUNC DIOPROTOCOL_COMMAND::GetReceivedFunc()
{
  return receivedfunc;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOPROTOCOL_COMMAND::GetDescription()
* @brief      Get description
* @ingroup    DATAIO
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOPROTOCOL_COMMAND::GetDescription()
{
  return description.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL_COMMAND::GetSINMask()
* @brief      Get SIN mask
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL_COMMAND::GetSINMask()
{
  return &inmask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL_COMMAND::GetSOUTMask()
* @brief      Get SOUT mask
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL_COMMAND::GetSOUTMask()
{
  return &outmask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL_COMMAND::GetSDescription()
* @brief      Get S description
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL_COMMAND::GetSDescription()
{
  return &description;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_COMMAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_COMMAND::Clean()
{
  type         = DIOPROTOCOL_CMDTYPE_NONE;

  inmask.Empty();
  outmask.Empty();

  receivedfunc = NULL;

  description.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_FILE::DIOPROTOCOL_FILE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_FILE::DIOPROTOCOL_FILE()
{
  Clean();

  xfile=GEN_XFACTORY.Create_File();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_FILE::~DIOPROTOCOL_FILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_FILE::~DIOPROTOCOL_FILE()
{
  GEN_XFACTORY.Delete_File(xfile);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::IsActive()
* @brief      Is active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetActive(bool isactive)
* @brief      Set active
* @ingroup    DATAIO
* 
* @param[in]  isactive : Isactive value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetActive(bool isactive)
{
  this->isactive = isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_DIRECTION DIOPROTOCOL_FILE::GetDirection()
* @brief      Get direction
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_DIRECTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_DIRECTION DIOPROTOCOL_FILE::GetDirection()
{
  return direction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetDirection(DIOPROTOCOL_DIRECTION direction)
* @brief      Set direction
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetDirection(DIOPROTOCOL_DIRECTION direction)
{
  this->direction = direction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOPROTOCOL_FILE::GetSourceName()
* @brief      Get source name
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOPROTOCOL_FILE::GetSourceName()
{
  return &sourcename;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::SetSourceName(XCHAR* sourcename)
* @brief      Set source name
* @ingroup    DATAIO
* 
* @param[in]  sourcename : Sourcename pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::SetSourceName(XCHAR* sourcename)
{
  if(!sourcename) return false;
  this->sourcename.Set(sourcename);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::SetSourceName(XSTRING& sourcename)
* @brief      Set source name
* @ingroup    DATAIO
* 
* @param[in]  sourcename : Sourcename value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::SetSourceName(XSTRING& sourcename)
{
  return SetSourceName(sourcename.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOPROTOCOL_FILE::GetTargetName()
* @brief      Get target name
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOPROTOCOL_FILE::GetTargetName()
{
  return &targetname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::SetTargetName(XCHAR* targetname)
* @brief      Set target name
* @ingroup    DATAIO
* 
* @param[in]  targetname : Targetname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::SetTargetName(XCHAR* targetname)
{
  if(!targetname) return false;
  this->targetname.Set(targetname);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::SetTargetName(XSTRING& targetname)
* @brief      Set target name
* @ingroup    DATAIO
* 
* @param[in]  targetname : Targetname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::SetTargetName(XSTRING& targetname)
{
  return SetTargetName(targetname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOPROTOCOL_FILE::GetSize()
* @brief      Get size
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOPROTOCOL_FILE::GetSize()
{
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetSize(XQWORD size)
* @brief      Set size
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetSize(XQWORD size)
{
  this->size = size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL_FILE::GetCRC32()
* @brief      Get CRC32
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL_FILE::GetCRC32()
{
  return crc32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetCRC32(XDWORD crc32)
* @brief      Set CRC32
* @ingroup    DATAIO
* 
* @param[in]  crc32 : Crc32 value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetCRC32(XDWORD crc32)
{
  this->crc32 = crc32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_FILE::GetSizeBlock()
* @brief      Get size block
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_FILE::GetSizeBlock()
{
  return sizeblock;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetSizeBlock(XDWORD sizeblock)
* @brief      Set size block
* @ingroup    DATAIO
* 
* @param[in]  sizeblock : Sizeblock value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetSizeBlock(XDWORD sizeblock)
{
  this->sizeblock = sizeblock;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOPROTOCOL_FILE::GetPosition()
* @brief      Get position
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOPROTOCOL_FILE::GetPosition()
{
  return position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetPosition(XQWORD position)
* @brief      Set position
* @ingroup    DATAIO
* 
* @param[in]  position : Position value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetPosition(XQWORD position)
{
  this->position = position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOPROTOCOL_FILE::GetPercent()
* @brief      Get percent
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOPROTOCOL_FILE::GetPercent()
{
  return percent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::SetPercent(XBYTE percent)
* @brief      Set percent
* @ingroup    DATAIO
* 
* @param[in]  percent : Percent value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::SetPercent(XBYTE percent)
{
  this->percent = percent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::ReadBlock(XBYTE* buffer)
* @brief      Read block
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::ReadBlock(XBYTE* buffer)
{
  if(!sourcename[0]) return false;

  if(!xfile->Open(sourcename)) return false;

  bool status = false;

  size = xfile->GetSize();
  if(size)
    {
      xfile->SetPosition(position);

      xfile->Read(buffer, &sizeblock);
      if(sizeblock)
        {
          xfile->GetPosition(position);
          if(size) percent = (XBYTE)(position*100/size);

          status = true;
        }
    }

  xfile->Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::WriteBlock(XBYTE* buffer)
* @brief      Write block
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::WriteBlock(XBYTE* buffer)
{
  if(!xfile)         return false;
  if(!targetname[0]) return false;

  if(position)
    {
      if(!xfile->Open(targetname, false))
        {
          if(!xfile->Create(targetname)) return false;
        }
    }
   else
    {
      if(!xfile->Create(targetname)) return false;
    }

  bool status = false;

  if(sizeblock)
    {
      xfile->SetPosition((XDWORD)XFILE_SEEKEND);

      status = xfile->Write(buffer, sizeblock);
      if(status)
        {
          xfile->GetPosition(position);
          if(size) percent = (XBYTE)(position*100/size);
        }
    }

  xfile->Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_FILE::Erase()
* @brief      Erase
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::Erase()
{
  if(!xfile)         return false;
  if(!targetname[0]) return false;

  xfile->Erase(targetname);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::Reset()
{
  isactive        = false;
  direction       = DIOPROTOCOL_DIRECTION_UNKNOWN;
  size            = 0;
  sizeblock       = 0;
  position        = 0;
  percent         = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_FILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_FILE::Clean()
{
  xfile           = NULL;

  isactive        = false;
  direction       = DIOPROTOCOL_DIRECTION_UNKNOWN;

  size            = 0;
  crc32           = 0;
  sizeblock       = 0;
  position        = 0;
  percent         = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL::DIOPROTOCOL(DIOSTREAM* diostream)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL::DIOPROTOCOL(DIOSTREAM* diostream)
{
  Clean();

  this->diostream   = diostream;

  answers.SetIsMulti(true);
  asks.SetIsMulti(true);

  RegisterEvent(DIOPROTOCOL_XEVENT_TYPE_COMMAND);
  RegisterEvent(DIOPROTOCOL_XEVENT_TYPE_FILEBLOCK);
  RegisterEvent(DIOPROTOCOL_XEVENT_TYPE_ADDALERT);

  xdatetime=GEN_XFACTORY.CreateDateTime();
  xtimer=GEN_XFACTORY.CreateTimer();
  watchdog=GEN_XFACTORY.CreateTimer();
  xdir=GEN_XFACTORY.Create_Dir();
  timerfileprogress=GEN_XFACTORY.CreateTimer();

  AddCommand(DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL, _L("O"), _L("O"), RCV_SendIsInitServerProtocol, _L("SendIsInitServerProtocol"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION, _L(""), _L("WWW"), RCV_GetProtocolVersion, _L("GetProtocolVersion"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION, _L(""), _L("WWW"), RCV_Application_GetVersion, _L("Application_GetVersion"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONNAME, _L(""), _L("S"), RCV_Application_GetName, _L("Application_GetName"));
  AddCommand(DIOPROTOCOL_CMDTYPE_PING, _L(""), _L("O"), RCV_Ping, _L("Ping"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETDATETIME, _L(""), _L("S"), RCV_GetDateTime, _L("GetDateTime"));
  AddCommand(DIOPROTOCOL_CMDTYPE_SETDATETIME, _L("S"), _L("O"), RCV_SetDateTime, _L("SetDateTime"));
  AddCommand(DIOPROTOCOL_CMDTYPE_INITFILEBLOCK, _L("SSBD"), _L("B"), RCV_InitFileBlock, _L("FileInit"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETFILEBLOCK, _L(""), _L("BDDDBAX"), RCV_GetFileBlock, _L("GetFileBlock"));
  AddCommand(DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK, _L("BDDDBAX"), _L("B"), RCV_SendFileBlock, _L("SendFileBlock"));
  AddCommand(DIOPROTOCOL_CMDTYPE_DELETEFILE, _L("SB"), _L("O"), RCV_DeleteFile, _L("DeleteFile"));
  AddCommand(DIOPROTOCOL_CMDTYPE_MAKEDIR, _L("S"), _L("O"), RCV_MakeDir, _L("MakeDir"));
  AddCommand(DIOPROTOCOL_CMDTYPE_DELETEDIR, _L("SB"), _L("O"), RCV_DeleteDir, _L("DeleteDir"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT, _L("SS"), _L("SBDD"), RCV_GetDirElement, _L("GetFirstDirElement"));
  AddCommand(DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT, _L(""), _L("SBDD"), RCV_GetDirElement, _L("GetNextDirElement"));
  #ifdef DIO_ALERTS_ACTIVE
  AddCommand(DIOPROTOCOL_CMDTYPE_SENDALERT, _L("SSDDDDBSSS"), _L("B"), RCV_SendAlert, _L("SendAlert"));
  #endif

  fileprogress = GEN_NEW DIOPROTOCOL_FILE();
  if(!fileprogress) return;

  xmutexanswers=GEN_XFACTORY.Create_Mutex();
  if(!xmutexanswers) return;

  xmutexasks=GEN_XFACTORY.Create_Mutex();
  if(!xmutexasks) return;

  xmutexcommands=GEN_XFACTORY.Create_Mutex();
  if(!xmutexcommands) return;

  xmutexreads=GEN_XFACTORY.Create_Mutex();
  if(!xmutexreads) return;

  xmutexwrites=GEN_XFACTORY.Create_Mutex();
  if(!xmutexwrites) return;

  xmutexfilesprogress=GEN_XFACTORY.Create_Mutex();
  if(!xmutexfilesprogress) return;

  xthreadreceived = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, _L("DIOPROTOCOL::DIOPROTOCOL received"), ThreadReceived, (void*)this);
  if(xthreadreceived)
    {
      if(!xthreadreceived->Ini()) return;
    }

  xthreadasks = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, _L("DIOPROTOCOL::DIOPROTOCOL ask"), ThreadAsks, (void*)this);
  if(xthreadasks)
    {
      if(!xthreadasks->Ini()) return;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL::~DIOPROTOCOL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL::~DIOPROTOCOL()
{
  DeRegisterEvent(DIOPROTOCOL_XEVENT_TYPE_COMMAND);
  DeRegisterEvent(DIOPROTOCOL_XEVENT_TYPE_FILEBLOCK);
  DeRegisterEvent(DIOPROTOCOL_XEVENT_TYPE_ADDALERT);

  doexit = true;

  if(xthreadreceived)
    {
      xthreadreceived->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, xthreadreceived);
      xthreadreceived = NULL;
    }

  if(xthreadasks)
    {
      xthreadasks->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, xthreadasks);
      xthreadasks = NULL;
    }

  DeleteAllAnswers();
  DeleteAllAsks();

  DeleteAllCommands();

  GEN_XFACTORY.Delete_Mutex(xmutexanswers);
  GEN_XFACTORY.Delete_Mutex(xmutexasks);
  GEN_XFACTORY.Delete_Mutex(xmutexcommands);
  GEN_XFACTORY.Delete_Mutex(xmutexreads);
  GEN_XFACTORY.Delete_Mutex(xmutexwrites);
  GEN_XFACTORY.Delete_Mutex(xmutexfilesprogress);

  GEN_DELETE fileprogress;

  if(xdir)              GEN_XFACTORY.Delete_Dir(xdir);
  if(watchdog)          GEN_XFACTORY.DeleteTimer(watchdog);
  if(xtimer)            GEN_XFACTORY.DeleteTimer(xtimer);
  if(xdatetime)         GEN_XFACTORY.DeleteDateTime(xdatetime);
  if(timerfileprogress) GEN_XFACTORY.DeleteTimer(timerfileprogress);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL::GetDescription()
* @brief      Get description
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL::GetDescription()
{
  return &description;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::Connect()
* @brief      Connect
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::Connect()
{
  if(!diostream)              return false;
  if(!diostream->GetConfig()) return false;

  if(!diostream->Open())      return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::Disconnect()
* @brief      Disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::Disconnect()
{
  if(!diostream) return false;

  diostream->Disconnect();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::Reset()
{
  if(!Disconnect()) return false;

  return Connect();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsConnected()
* @brief      Is connected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsConnected()
{
  if(!diostream)                                                    return false;
  if(diostream->GetStatus()==DIOSTREAMSTATUS_CONNECTED)      return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsConnecting()
* @brief      Is connecting
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsConnecting()
{
  if(!diostream)                                                       return false;
  if(diostream->GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION)  return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsDisconnected()
* @brief      Is disconnected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsDisconnected()
{
  if(!diostream)                                                      return false;
  if(diostream->GetStatus()==DIOSTREAMSTATUS_DISCONNECTED)     return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsInitialized()
* @brief      Is initialized
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsInitialized()
{
  return isinitialized;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetIsInitialized(bool isinitialized)
* @brief      Set is initialized
* @ingroup    DATAIO
* 
* @param[in]  isinitialized : Isinitialized value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetIsInitialized(bool isinitialized)
{
  this->isinitialized = isinitialized;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsServerInitialized()
* @brief      Is server initialized
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsServerInitialized()
{
  return isserverinitialized;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsOperational()
* @brief      Is operational
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsOperational()
{
  if(!IsConnected())    return false;
  if(!IsInitialized())  return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL::GetMaskID()
* @brief      Get mask ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL::GetMaskID()
{
  return maskID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SetMaskID(XDWORD maskID)
* @brief      Set mask ID
* @ingroup    DATAIO
* 
* @param[in]  maskID : Mask ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SetMaskID(XDWORD maskID)
{
  this->maskID = (maskID&~DIOPROTOCOL_CMDTYPE_ANSWER);
  return (this->maskID > 65535)?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* DIOPROTOCOL::GetConnectionsManager()
* @brief      Get connections manager
* @ingroup    DATAIO
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* DIOPROTOCOL::GetConnectionsManager()
{
  return connectionsmanager;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetConnectionsManager(void* connectionsmanager)
* @brief      Set connections manager
* @ingroup    DATAIO
* 
* @param[in]  connectionsmanager : Connectionsmanager pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetConnectionsManager(void* connectionsmanager)
{
  this->connectionsmanager = connectionsmanager;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* DIOPROTOCOL::GetConnection()
* @brief      Get connection
* @ingroup    DATAIO
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* DIOPROTOCOL::GetConnection()
{
  return connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetConnection(void* connection)
* @brief      Set connection
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetConnection(void* connection)
{
  this->connection = connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* DIOPROTOCOL::GetApplicationData()
* @brief      Get application data
* @ingroup    DATAIO
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* DIOPROTOCOL::GetApplicationData()
{
  return applicationdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetApplicationData(void* applicationdata)
* @brief      Set application data
* @ingroup    DATAIO
* 
* @param[in]  applicationdata : Applicationdata pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetApplicationData(void* applicationdata)
{
  this->applicationdata = applicationdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::IsOwnApplicationData()
* @brief      Is own application data
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::IsOwnApplicationData()
{
  return isownapplicationdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetIsOwnApplicationData(bool isownapplicationdata)
* @brief      Set is own application data
* @ingroup    DATAIO
* 
* @param[in]  isownapplicationdata : Isownapplicationdata value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetIsOwnApplicationData(bool isownapplicationdata)
{
  this->isownapplicationdata = isownapplicationdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM* DIOPROTOCOL::GetDIOStream()
* @brief      Get DIO stream
* @ingroup    DATAIO
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOPROTOCOL::GetDIOStream()
{
  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* DIOPROTOCOL::GetDateTime()
* @brief      Get date time
* @ingroup    DATAIO
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* DIOPROTOCOL::GetDateTime()
{
  return xdatetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::AddCommand(XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description)
* @brief      Add command
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  inmask : Inmask pointer to use.
* @param[in]  outmask : Output outmask.
* @param[in]  receivedfunc : Receivedfunc value.
* @param[in]  description : Description pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::AddCommand(XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description)
{
  DIOPROTOCOL_COMMAND* cmd = GEN_NEW DIOPROTOCOL_COMMAND(type, inmask, outmask, receivedfunc, description);
  if(!cmd) return false;

  commands.Add(cmd);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_COMMAND* DIOPROTOCOL::GetCommand(XDWORD type)
* @brief      Get command
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* @return     DIOPROTOCOL_COMMAND* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_COMMAND* DIOPROTOCOL::GetCommand(XDWORD type)
{
  for(int c=0;c<(int)commands.GetSize();c++)
    {
      DIOPROTOCOL_COMMAND* cmd = (DIOPROTOCOL_COMMAND*)commands.Get(c);
      if(cmd)
        {
          if(cmd->GetType() == type) return cmd;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAnswer(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
* @brief      Add answer
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     DIOPROTOCOL_ANSWER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAnswer(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
{
  if(xmutexanswers) xmutexanswers->Lock();

  DIOPROTOCOL_ANSWER* answer = GEN_NEW DIOPROTOCOL_ANSWER(type, ID, xbuffer);
  if(answer) answers.Add(answer);

  if(xmutexanswers) xmutexanswers->UnLock();

  return answer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAnswer(XDWORD type, XDWORD ID)
* @brief      Get answer
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* 
* @return     DIOPROTOCOL_ANSWER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAnswer(XDWORD type, XDWORD ID)
{
  if(xmutexanswers) xmutexanswers->Lock();

  DIOPROTOCOL_ANSWER* answer = NULL;

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      answer = (DIOPROTOCOL_ANSWER*)answers.Get(c);
      if(answer)
        {
          if((answer->GetType() == type) && (answer->GetID()  == ID))  break; else answer = NULL;
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return answer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::DeleteAnswer(XDWORD type, XDWORD ID)
* @brief      Delete answer
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAnswer(XDWORD type, XDWORD ID)
{
  if(xmutexanswers) xmutexanswers->Lock();

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      DIOPROTOCOL_ANSWER* answer = (DIOPROTOCOL_ANSWER*)answers.Get(c);
      if(answer)
        {
          if((answer->GetType() == type) && (answer->GetID() == ID))
            {
              answers.Delete(answer);
              GEN_DELETE answer;

              break;
            }
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::DeleteAllAnswers()
* @brief      Delete all answers
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllAnswers()
{
  if(xmutexanswers) xmutexanswers->Lock();

  bool status = false;

  if(!answers.IsEmpty())
    {
      answers.DeleteContents();
      answers.DeleteAll();

      status = true;
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAsk(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
* @brief      Add ask
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     DIOPROTOCOL_ANSWER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAsk(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* ask = GEN_NEW DIOPROTOCOL_ANSWER(type, ID, xbuffer);
  if(ask) asks.Add(ask);

  if(xmutexasks) xmutexasks->UnLock();

  return ask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAsk(XDWORD index)
* @brief      Get ask
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOPROTOCOL_ANSWER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAsk(XDWORD index)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* ask = (DIOPROTOCOL_ANSWER*)asks.Get(index);

  if(xmutexasks) xmutexasks->UnLock();

  return ask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::DeleteAsk(XDWORD index)
* @brief      Delete ask
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAsk(XDWORD index)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* answer = (DIOPROTOCOL_ANSWER*)asks.Get(index);
  if(answer)
    {
      asks.Delete(answer);
      GEN_DELETE answer;
    }

  if(xmutexasks) xmutexasks->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::DeleteAllAsks()
* @brief      Delete all asks
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllAsks()
{
  if(xmutexasks) xmutexasks->Lock();

  bool status =  false;

  if(!asks.IsEmpty())
    {
      asks.DeleteContents();
      asks.DeleteAll();

      status = true;
    }

  if(xmutexasks) xmutexasks->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::ReadCommand(XDWORD& type, XDWORD& ID, XBUFFER& xbuffer)
* @brief      Read command
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::ReadCommand(XDWORD& type, XDWORD& ID, XBUFFER& xbuffer)
{
  if(!IsConnected()) return 0;

  XDWORD sizecmd = diostream->GetInXBuffer()->GetSize();
  if(sizecmd < DIOPROTOCOL_SIZEOFHEAD) return 0;

  XBYTE head[DIOPROTOCOL_SIZEOFHEAD];

  memset(head, 0, DIOPROTOCOL_SIZEOFHEAD);
  diostream->GetInXBuffer()->Get(head, DIOPROTOCOL_SIZEOFHEAD);

  HASHCRC32 crc32;
  XDWORD*   headcmd         = (XDWORD*)head;
  XDWORD    IDhead          = 0;
  XDWORD    sizebuffer      = 0;
  XDWORD    bodycrc32value  = 0;
  XDWORD    headcrc32value  = 0;
  XDWORD    crc32value      = 0;

  IDhead          = headcmd[0];
  type            = headcmd[1];
  sizebuffer      = headcmd[2];
  ID              = headcmd[3];
  bodycrc32value  = headcmd[4];
  headcrc32value  = headcmd[5];

  crc32.ResetResult();
  crc32.Do((XBYTE*)headcmd, (DIOPROTOCOL_SIZEOFHEAD - sizeof(XDWORD)));
  crc32value  = crc32.GetResultCRC32();

  if((IDhead != DIOPROTOCOL_MSGIDHEAD) || (headcrc32value != crc32value))
    {
      return 0;
    }

  if(!diostream->WaitToFilledReadingBuffer((DIOPROTOCOL_SIZEOFHEAD + sizebuffer), 5))
    {
      return 0;
    }

  //diostream->GetInXBuffer()->Extract(NULL, 0, DIOPROTOCOL_SIZEOFHEAD);
  diostream->Read(head, DIOPROTOCOL_SIZEOFHEAD);

  if(sizebuffer)
    {
      xbuffer.Delete();
      xbuffer.Resize(sizebuffer);

    //if(diostream->GetInXBuffer()->Extract(xbuffer.Get(), 0, sizebuffer))
      if(diostream->Read(xbuffer.Get(), sizebuffer))
        {
          crc32.ResetResult();
          crc32.Do(xbuffer);
          crc32value  = crc32.GetResultCRC32();

          if(bodycrc32value == crc32value)
            {
              return (DIOPROTOCOL_SIZEOFHEAD + sizebuffer);
            }
           else
            {
              return 0;
            }

        }
       else
        {
          return 0;
        }
    }

  return DIOPROTOCOL_SIZEOFHEAD;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_RESULT DIOPROTOCOL::Received()
* @brief      Received
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_RESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_RESULT DIOPROTOCOL::Received()
{
  if(!IsConnected()) return DIOPROTOCOL_RESULT_NOTCONNEX;

  if(xmutexreads) xmutexreads->Lock();

  int                 size = 0;
  DIOPROTOCOL_RESULT  status = DIOPROTOCOL_RESULT_UNKNOWNCMD;

  XDWORD                type   = DIOPROTOCOL_CMDTYPE_NONE;
  XDWORD                ID     = 0;
  XBUFFER               xbuffer;

  size = ReadCommand(type, ID, xbuffer);
  if(size)
    {
      bool   isanswer = ((type&DIOPROTOCOL_CMDTYPE_ANSWER)==DIOPROTOCOL_CMDTYPE_ANSWER)?true:false;
      XDWORD rtype    = (type&~DIOPROTOCOL_CMDTYPE_ANSWER);

      /*
      DIOPROTOCOL_COMMAND* cmd  = GetCommand(rtype);
      if(cmd)
        {
         XTRACE_PRINTCOLOR(((isanswer)?1:2), _L("DIOPROTOCOL %s %s : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?_L("Server  "):_L("Client  ")
                                                                                                    , (isanswer)?_L("Answer  "):_L("Ask     ")
                                                                                                    , this
                                                                                                    , ID
                                                                                                    , rtype
                                                                                                    , cmd->GetDescription()
                                                                                                    , xbuffer.GetSize());
        }
      */
      if(isanswer)
        {
          if(AddAnswer(rtype, ID, xbuffer)) status = DIOPROTOCOL_RESULT_OK;
        }
       else
        {
          //status = ReceivedHandle(rtype, ID, xbuffer);
          if(AddAsk(rtype, ID, xbuffer))    status = DIOPROTOCOL_RESULT_OK;
        }

    }

  if(xmutexreads) xmutexreads->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_RESULT DIOPROTOCOL::ReceivedHandle(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
* @brief      Received handle
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     DIOPROTOCOL_RESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_RESULT DIOPROTOCOL::ReceivedHandle(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
{
  DIOPROTOCOL_RESULT status     = DIOPROTOCOL_RESULT_UNKNOWNCMD;
  XDWORD             param      = 0;

  if((type & DIOPROTOCOL_CMDTYPE_ANSWER) == DIOPROTOCOL_CMDTYPE_ANSWER) return DIOPROTOCOL_RESULT_UNKNOWNCMD;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(type);
  if(!cmd)  return DIOPROTOCOL_RESULT_UNKNOWNCMD;

  DIOPROTOCOL_CONNECTION* connection =  (DIOPROTOCOL_CONNECTION*)GetConnection();

  if(connection) connection->SetInUse(true, DIOPROTOCOL_CONNECTIONSINUSEID_ANSWER);

  if(cmd->GetReceivedFunc())
    {
      int ssize = cmd->GetReceivedFunc()(this, cmd, xbuffer, ID, param);

      if(ssize != DIOPROTOCOL_ERRORRECEIVED)
        {
          if(cmd->GetSOUTMask()->GetSize())
            {
              if(ssize)
                {
                  if(SendCommand(type|DIOPROTOCOL_CMDTYPE_ANSWER, ID, xbuffer)) status = DIOPROTOCOL_RESULT_OK;
                }

            } else status = DIOPROTOCOL_RESULT_OK;
        }
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, type, (status==DIOPROTOCOL_RESULT_OK)?true:false, NULL, param);

  if(connection) connection->SetInUse(false, DIOPROTOCOL_CONNECTIONSINUSEID_ANSWER);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SendCommand(XDWORD type, XDWORD& ID, XBUFFER& xbuffer)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  ID : Identifier to use.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SendCommand(XDWORD type, XDWORD& ID, XBUFFER& xbuffer)
{
  if(!IsConnected()) return false;

  HASHCRC32 crc32;
  XDWORD    crc32head = 0;
  XDWORD    crc32body = 0;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(type&~DIOPROTOCOL_CMDTYPE_ANSWER);
  if(!cmd) return false;

  if(xmutexwrites) xmutexwrites->Lock();

  XBYTE   head[DIOPROTOCOL_SIZEOFHEAD];
  XDWORD* ptr_head  = (XDWORD*)head;

  memset(head, 0, DIOPROTOCOL_SIZEOFHEAD);

  if(xbuffer.GetSize())
    {
      crc32.ResetResult();
      crc32.Do(xbuffer);
      crc32body = crc32.GetResultCRC32();
    }

  if(!ID)
    {
      ID = IDmsgcounter;
      ID |= maskID;

      IDmsgcounter++;
      if(IDmsgcounter > DIOPROTOCOL_CMDTYPE_SECUENCIALLIMIT)  IDmsgcounter = 0;
    }

  ptr_head[0] = DIOPROTOCOL_MSGIDHEAD;
  ptr_head[1] = type;
  ptr_head[2] = xbuffer.GetSize();
  ptr_head[3] = ID;
  ptr_head[4] = crc32body;

  crc32.ResetResult();
  crc32.Do(head, DIOPROTOCOL_SIZEOFHEAD-sizeof(XDWORD));
  crc32head = crc32.GetResultCRC32();

  ptr_head[5] = crc32head;

  bool status = false;

  if(xbuffer.GetSize())
    {
      XBUFFER xbuffersend;

      xbuffersend.Add(head, DIOPROTOCOL_SIZEOFHEAD);
      xbuffersend.Add(xbuffer);

      status = (diostream->Write(xbuffersend) == xbuffersend.GetSize())?true:false;
      if(status) diostream->WaitToFlushOutXBuffer(5);
    }
   else
    {
      status = (diostream->Write(head, DIOPROTOCOL_SIZEOFHEAD) == DIOPROTOCOL_SIZEOFHEAD)?true:false;
      if(status) status = diostream->WaitToFlushOutXBuffer(5);
    }

  if(xmutexwrites) xmutexwrites->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::Command(XDWORD type, XBUFFER& xbuffer, int timeout)
* @brief      Command
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::Command(XDWORD type, XBUFFER& xbuffer, int timeout)
{
  if(!IsConnected())  return false;
  if(!watchdog)       return false;

  if(xmutexcommands) xmutexcommands->Lock();

  XDWORD ID     = 0;
  bool   status = false;


  DIOPROTOCOL_COMMAND* cmd = GetCommand(type);
  if(cmd)
    {
      /*
     XTRACE_PRINTCOLOR(1, _L("DIOPROTOCOL %s Command  : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?_L("Server  "):_L("Client  ")
                                                                                       , this
                                                                                       , ID
                                                                                       , type
                                                                                       , cmd->GetDescription()
                                                                                       , xbuffer.GetSize());
      */
      if(SendCommand(type, ID, xbuffer))
        {
          /*
         XTRACE_PRINTCOLOR(1, _L("DIOPROTOCOL %s Send     : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?_L("Server  "):_L("Client  ")
                                                                                           , this
                                                                                           , ID
                                                                                           , type
                                                                                           , cmd->GetDescription()
                                                                                           , xbuffer.GetSize());
          */
          xbuffer.Delete();

          //--------------------------------------------
          // No Answer
          if(!cmd->GetSOUTMask()->IsEmpty())
            {
              watchdog->Reset();

              while(1)
                {
                  if(!IsConnected()) break;

                  DIOPROTOCOL_ANSWER* answer = GetAnswer(type, ID);
                  if(answer)
                    {
                      xbuffer.Delete();
                      xbuffer.Add(answer->GetBuffer());

                      DeleteAnswer(type, ID);

                      status = true;
                      break;
                    }

                  if((int)watchdog->GetMeasureSeconds() >= timeout)
                    {
                      break;
                    }

                  GEN_XSLEEP.MilliSeconds(10);
                }

            } else status = true;

          if(!status)
            {
             XTRACE_PRINTCOLOR((status?1:4), _L("DIOPROTOCOL %s : %08X %08X %02d %-35s (%8d) %s"), GetDIOStream()->GetConfig()->IsServer()?_L("Server "):_L("Client "), this, ID, type, cmd->GetDescription(), xbuffer.GetSize(), status?_L("Ok"):_L("ERROR!"));
            }
        }

    }

  if(xmutexcommands) xmutexcommands->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SendEvent(DIOPROTOCOL_XEVENT_TYPE type, DIOPROTOCOL* protocol)
* @brief      Send event
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  protocol : Protocol pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SendEvent(DIOPROTOCOL_XEVENT_TYPE type, DIOPROTOCOL* protocol)
{
  DIOPROTOCOL_XEVENT xevent(this, type);

  xevent.SetProtocol(protocol);
  PostEvent(&xevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SendEventCommand(DIOPROTOCOL* protocol, DIOPROTOCOL_DIRECTION direction, XDWORD type, bool result, void* param1, XDWORD param2)
* @brief      Send event command
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  direction : Direction value.
* @param[in]  type : Type value.
* @param[in]  result : Output result.
* @param[in]  param1 : Param1 pointer to use.
* @param[in]  param2 : Param2 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SendEventCommand(DIOPROTOCOL* protocol, DIOPROTOCOL_DIRECTION direction, XDWORD type, bool result, void* param1, XDWORD param2)
{
  DIOPROTOCOL_XEVENT xevent(this, DIOPROTOCOL_XEVENT_TYPE_COMMAND);

  xevent.SetProtocol(protocol);
  xevent.SetDirection(direction);
  xevent.SetType(type);
  xevent.SetResult(result);

  xevent.SetParam1(param1);
  xevent.SetParam2(param2);

  PostEvent(&xevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SendEventFileBlock(DIOPROTOCOL_FILESTATUS filestatus, bool fileissend, XPATH* filesource, XPATH* filetarget, int filepercent, void* param)
* @brief      Send event file block
* @ingroup    DATAIO
* 
* @param[in]  filestatus : Filestatus value.
* @param[in]  fileissend : Fileissend value.
* @param[in]  filesource : Filesource pointer to use.
* @param[in]  filetarget : Filetarget pointer to use.
* @param[in]  filepercent : Filepercent value.
* @param[in]  param : Param pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SendEventFileBlock(DIOPROTOCOL_FILESTATUS filestatus, bool fileissend, XPATH* filesource, XPATH* filetarget, int filepercent, void* param)
{
  DIOPROTOCOL_XEVENT xevent(this, DIOPROTOCOL_XEVENT_TYPE_FILEBLOCK);

  xevent.SetFileStatus(filestatus);
  xevent.SetFileIsSend(fileissend);
  xevent.SetFileSource(filesource->Get());
  xevent.SetFileTarget(filetarget->Get());
  xevent.SetFilePercent(filepercent);

  xevent.SetParam1(param);

  PostEvent(&xevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::GetProtocolVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
* @brief      Get protocol version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::GetProtocolVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
{
  version         = this->protocolversion;
  subversion      = this->protocolsubversion;
  subversionerr   = this->protocolsubversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::SetProtocolVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
* @brief      Set protocol version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::SetProtocolVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
{
  this->protocolversion           = version;
  this->protocolsubversion        = subversion;
  this->protocolsubversionerr     = subversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::Application_GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
* @brief      Application get version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::Application_GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
{
  version         = this->applicationversion;
  subversion      = this->applicationsubversion;
  subversionerr   = this->applicationsubversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::Application_SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
* @brief      Application set version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::Application_SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
{
  this->applicationversion        = version;
  this->applicationsubversion     = subversion;
  this->applicationsubversionerr  = subversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL::Application_GetName()
* @brief      Application get name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL::Application_GetName()
{
  return &applicationname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::SetFileProgress(DIOPROTOCOL_DIRECTION direction, XPATH& sourcename, XPATH& targetname, XDWORD CRC32)
* @brief      Set file progress
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
* @param[in]  sourcename : Sourcename value.
* @param[in]  targetname : Targetname value.
* @param[in]  CRC32 : CRC32 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::SetFileProgress(DIOPROTOCOL_DIRECTION direction, XPATH& sourcename, XPATH& targetname, XDWORD CRC32)
{
  if(sourcename.IsEmpty())  return false;

  fileprogress->Reset();

  fileprogress->SetSourceName(sourcename);

  if(targetname.IsEmpty())
         fileprogress->SetTargetName(sourcename);
    else fileprogress->SetTargetName(targetname);

  fileprogress->SetActive(false);
  fileprogress->SetDirection(direction);
  fileprogress->SetPosition(0);

  if(CRC32)
        fileprogress->SetCRC32(CRC32);
   else fileprogress->SetCRC32(CRC32OfFile(sourcename));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_FILE* DIOPROTOCOL::GetFileProgress()
* @brief      Get file progress
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_FILE* DIOPROTOCOL::GetFileProgress()
{
  return fileprogress;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOPROTOCOL::GetPathSearch()
* @brief      Get path search
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOPROTOCOL::GetPathSearch()
{
  return &pathsearch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIRELEMENT* DIOPROTOCOL::GetSearchElement()
* @brief      Get search element
* @ingroup    DATAIO
* 
* @return     XDIRELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIRELEMENT* DIOPROTOCOL::GetSearchElement()
{
  return &searchelement;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDIR* DIOPROTOCOL::GetXDir()
* @brief      Get X dir
* @ingroup    DATAIO
* 
* @return     XDIR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDIR* DIOPROTOCOL::GetXDir()
{
  return xdir;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_GetProtocolVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
* @brief      CMD get protocol version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetProtocolVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
{
  bool result = false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION);
  if(!cmd) return false;

  XBUFFER xbuffer;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &version, &subversion, &subversionerr);
      result = true;
    }

  XDWORD param = (version<<8)|subversion;

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, param);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_GetProtocolVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV get protocol version
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetProtocolVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), protocol->protocolversion, protocol->protocolsubversion, protocol->protocolsubversionerr);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_Application_GetVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
* @brief      CMD application get version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_Application_GetVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
{
  bool result = false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION);
  if(!cmd) return false;

  XBUFFER xbuffer;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &version, &subversion, &subversionerr);
      result = true;
    }

  XDWORD param = (version<<8)|subversion;

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, param);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_Application_GetVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV application get version
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_Application_GetVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), protocol->applicationversion, protocol->applicationsubversion, protocol->applicationsubversionerr);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_Application_GetName(XSTRING& applicationname)
* @brief      CMD application get name
* @ingroup    DATAIO
* 
* @param[in]  applicationname : Application name.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_Application_GetName(XSTRING& applicationname)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONNAME);
  if(!cmd) return false;

  XBUFFER xbuffer;
  bool    result = false;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &applicationname);
      result = true;
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_Application_GetName(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV application get name
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_Application_GetName(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), &protocol->applicationname);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_Ping(XDWORD& milliseconds, int timeout)
* @brief      CMD ping
* @ingroup    DATAIO
* 
* @param[in]  milliseconds : Milliseconds value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_Ping(XDWORD& milliseconds, int timeout)
{
  if(!xtimer) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_PING);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XBYTE result = 0;

  xtimer->Reset();

  if(Command(cmd->GetType(), xbuffer, timeout))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  milliseconds = (XDWORD)xtimer->GetMeasureMilliSeconds();

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result?true:false, NULL, milliseconds);

  return result?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_Ping(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV ping
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_Ping(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  bool result = true;

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_SendIsInitServerProtocol(bool isinit)
* @brief      CMD send is init server protocol
* @ingroup    DATAIO
* 
* @param[in]  isinit : Isinit value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendIsInitServerProtocol(bool isinit)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XBYTE    _isinit    = isinit?1:0;
  bool     result     = false;

  xbuffer.AddWithMask(cmd->GetSINMask(), _isinit);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_SendIsInitServerProtocol(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV send is init server protocol
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendIsInitServerProtocol(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XBYTE  isinit;
  bool   result = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &isinit);

  if(isinit)
    {
      protocol->isserverinitialized = true;
      result                        = true;
    }

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_GetDateTime(XSTRING& datestring)
* @brief      CMD get date time
* @ingroup    DATAIO
* 
* @param[in]  datestring : Datestring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetDateTime(XSTRING& datestring)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETDATETIME);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XBYTE   result = 0;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &datestring);

      if(!datestring.IsEmpty()) result = 1;
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result?true:false, NULL, 0);

  return result?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_GetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV get date time
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING datestring;

  protocol->GetDateTime()->Read();
  protocol->GetDateTime()->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datestring);

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), &datestring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_SetDateTime(XCHAR* datestring)
* @brief      CMD set date time
* @ingroup    DATAIO
* 
* @param[in]  datestring : Datestring pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SetDateTime(XCHAR* datestring)
{
  if(!datestring) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SETDATETIME);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XSTRING  _datestring;
  bool     result     = 0;

  _datestring = datestring;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_datestring);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_SetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV set date time
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING datestring;
  bool    result = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &datestring);

  if(protocol->GetDateTime()->GetDateTimeFromString(datestring, XDATETIME_FORMAT_STANDARD))
    {
      protocol->GetDateTime()->Write();
      result = true;
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_File(DIOPROTOCOL_DIRECTION direction, XCHAR* sourcename, XCHAR* targetname, int timeout, void* param)
* @brief      CMD file
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
* @param[in]  sourcename : Sourcename pointer to use.
* @param[in]  targetname : Targetname pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  param : Param pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_File(DIOPROTOCOL_DIRECTION direction, XCHAR* sourcename, XCHAR* targetname, int timeout, void* param)
{
  if(!xtimer)    return false;

  if(!sourcename) return 0;
  if(!targetname) return 0;

  XPATH _sourcename;
  XPATH _targetname;

  _sourcename = sourcename;
  _targetname = targetname;

  if(_sourcename.IsEmpty()) return false;
  if(_targetname.IsEmpty()) return false;

  if(!SetFileProgress(direction, _sourcename, _targetname, 0)) return false;

  if(!CMD_InitFileBlock(fileprogress)) return false;

  bool status  = false;

  SendEventFileBlock(DIOPROTOCOL_FILESTATUS_REQUEST, fileprogress->IsActive(), fileprogress->GetSourceName(), fileprogress->GetTargetName(), fileprogress->GetPercent(), param);
  xtimer->Reset();

  while(1)
    {
      if(fileprogress->IsActive())
        {
          switch(fileprogress->GetDirection())
            {
              case DIOPROTOCOL_DIRECTION_UNKNOWN  : fileprogress->SetActive(false);     break;
              case DIOPROTOCOL_DIRECTION_GET      : CMD_GetFileBlock(fileprogress);     break;
              case DIOPROTOCOL_DIRECTION_SEND     : CMD_SendFileBlock(fileprogress);    break;
            }
        }

      SendEventFileBlock(DIOPROTOCOL_FILESTATUS_WAIT, fileprogress->IsActive(), fileprogress->GetSourceName(), fileprogress->GetTargetName(), fileprogress->GetPercent(), param);

      GEN_XSLEEP.MilliSeconds(10);

      if(!IsConnected())                              break;
      if((int)xtimer->GetMeasureSeconds()>=timeout)   break;
      if(!fileprogress->IsActive())                   break;
    }

  if(fileprogress->GetPercent()>=100)  status = true;

  DIOPROTOCOL_FILESTATUS fstatus = DIOPROTOCOL_FILESTATUS_END;
  if(!status) fstatus = DIOPROTOCOL_FILESTATUS_ERROR;

  SendEventFileBlock(fstatus, fileprogress->IsActive(), fileprogress->GetSourceName(), fileprogress->GetTargetName(), fileprogress->GetPercent(), param);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_DeleteFile(XCHAR* namefile, bool overwrite)
* @brief      CMD GEN_DELETE file
* @ingroup    DATAIO
* 
* @param[in]  namefile : Namefile pointer to use.
* @param[in]  overwrite : Overwrite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_DeleteFile(XCHAR* namefile, bool overwrite)
{
  if(!namefile) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_DELETEFILE);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namefile;
  bool    result    = false;

  _namefile = namefile;
  if(_namefile.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namefile, overwrite?1:0);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_DeleteFile(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV GEN_DELETE file
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_DeleteFile(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  XBYTE overwrite = 0;
  bool  result    = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path, &overwrite);

  XFILE* xfile=GEN_XFACTORY.Create_File();
  if(xfile)
    {
      result = xfile->Erase(path, overwrite?true:false)?true:false;
      GEN_XFACTORY.Delete_File(xfile);
    }

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_MakeDir(XCHAR* namedir)
* @brief      CMD make dir
* @ingroup    DATAIO
* 
* @param[in]  namedir : Namedir pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_MakeDir(XCHAR* namedir)
{
  if(!namedir) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_MAKEDIR);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namedir;
  bool    result    = false;

  _namedir = namedir;
  if(_namedir.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namedir);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_MakeDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV make dir
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_MakeDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  bool  result = 0;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path);

  result = protocol->GetXDir()->Make(path.Get())?true:false;

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_DeleteDir(XCHAR* namedir, bool all)
* @brief      CMD GEN_DELETE dir
* @ingroup    DATAIO
* 
* @param[in]  namedir : Namedir pointer to use.
* @param[in]  all : All value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_DeleteDir(XCHAR* namedir, bool all)
{
  if(!namedir) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_DELETEDIR);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namedir;
  bool    result    = false;

  _namedir = namedir;
  if(_namedir.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namedir, all?1:0);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_DeleteDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV GEN_DELETE dir
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_DeleteDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  XBYTE all   = 0;
  bool  result = 0;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path, &all);

  result = protocol->GetXDir()->Delete(path.Get(), all?true:false);

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), 0, result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_GetDirElement(XCHAR* pathsearch, XCHAR* mask, XPATH& namefile, XBYTE& type, int& sizefile, XDWORD& CRC32)
* @brief      CMD get dir element
* @ingroup    DATAIO
* 
* @param[in]  pathsearch : Pathsearch pointer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  namefile : Namefile value.
* @param[in]  type : Type value.
* @param[in]  sizefile : Sizefile value.
* @param[in]  CRC32 : CRC32 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetDirElement(XCHAR* pathsearch, XCHAR* mask, XPATH& namefile, XBYTE& type, int& sizefile, XDWORD& CRC32)
{
  if(!pathsearch) return false;
  if(!mask)       return false;

  XBUFFER   xbuffer;
  XPATH     _pathsearch;
  XSTRING   _mask;
  bool      result      = false;

  _pathsearch = pathsearch;
  _mask       = mask;

  DIOPROTOCOL_COMMAND* cmd = GetCommand((!_pathsearch.IsEmpty())?DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT:DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT);
  if(!cmd) return false;

  if(_mask.IsEmpty()) _mask = _L("*");

  xbuffer.AddWithMask(cmd->GetSINMask(), &_pathsearch, &_mask);

  if(Command(cmd->GetType(), xbuffer))
    {
      namefile.Empty();

      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &namefile, &type, &sizefile, &CRC32);

      if(!namefile.IsEmpty()) result = true;
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result?true:false, NULL, 0);

  return result?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_GetDirElement(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV get dir element
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetDirElement(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH     pathsearch;
  XSTRING   masksearch;
  XPATH     pathfile;
  XBYTE     typefile     = 0;
  int       sizefile     = 0;
  XDWORD    crc32value  = 0;
  bool      found       = false;

  pathfile.Empty();

  if(cmd->GetType()==DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT)
    {
      pathsearch.Empty();

      xbuffer.GetWithMask(cmd->GetSINMask(), 0, &pathsearch, &masksearch);

      protocol->GetPathSearch()->Set(pathsearch);
      protocol->GetSearchElement()->DeleteFindFileData();

      found = protocol->GetXDir()->FirstSearch(pathsearch, masksearch, protocol->GetSearchElement());
    }
   else found =  protocol->GetXDir()->NextSearch(protocol->GetSearchElement());

  if(found)
    {
      typefile    = (XBYTE)protocol->GetSearchElement()->GetType();

      if(protocol->GetSearchElement()->GetType() == XDIRELEMENTTYPE_FILE)
        {
          pathfile  = protocol->GetPathSearch()->Get();
          pathfile += protocol->GetSearchElement()->GetNameFile()->Get();

          HASHCRC32 crc32;

          if(crc32.Do(pathfile, sizefile)) crc32value = crc32.GetResultCRC32();
        }

      pathfile  = protocol->GetSearchElement()->GetNameFile()->Get();
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), &pathfile, typefile, sizefile, crc32value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::DeleteAllCommands()
* @brief      Delete all commands
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllCommands()
{
  if(commands.IsEmpty())  return false;

  commands.DeleteContents();
  commands.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPROTOCOL::CRC32OfFile(XPATH& xpath)
* @brief      CRC32 of file
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPROTOCOL::CRC32OfFile(XPATH& xpath)
{
  XDWORD value = 0;

  HASHCRC32* crc32 = GEN_NEW HASHCRC32();
  if(!crc32) return false;

  XFILE* file=GEN_XFACTORY.Create_File();
  if(file)
    {
      if(file->Open(xpath))
        {
          crc32->ResetResult();
          crc32->Do(file, file->GetSize(), 0);
          value  = crc32->GetResultCRC32();

          file->Close();
        }

      GEN_XFACTORY.Delete_File(file);
    }

  GEN_DELETE crc32;

  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_InitFileBlock(DIOPROTOCOL_FILE* fp)
* @brief      CMD init file block
* @ingroup    DATAIO
* 
* @param[in]  fp : Fp pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_InitFileBlock(DIOPROTOCOL_FILE* fp)
{
  if(!fp) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_INITFILEBLOCK);
  if(!cmd) return false;

  XBUFFER xbuffer;
  bool    result = false;

  xbuffer.AddWithMask(cmd->GetSINMask(), fp->GetSourceName(), fp->GetTargetName(), fp->GetDirection(), fp->GetCRC32());


  if(Command(cmd->GetType(), xbuffer))
    {
      if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result))
        {
          if(result) GetFileProgress()->SetActive(true);
        }
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_InitFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV init file block
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_InitFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH                  sourcename;
  XPATH                  targetname;
  XBYTE                  _direction   = 0;
  DIOPROTOCOL_DIRECTION  direction    = (DIOPROTOCOL_DIRECTION)_direction;
  XDWORD                 CRC32        = 0;
  bool                   result       = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &sourcename, &targetname, &_direction, &CRC32);

  direction = (DIOPROTOCOL_DIRECTION)_direction;

  if(protocol->SetFileProgress(direction, sourcename, targetname, CRC32))
    {
      protocol->GetFileProgress()->SetActive(true);
      result = true;
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_GetFileBlock(DIOPROTOCOL_FILE* fp)
* @brief      CMD get file block
* @ingroup    DATAIO
* 
* @param[in]  fp : Fp pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetFileBlock(DIOPROTOCOL_FILE* fp)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETFILEBLOCK);
  if(!cmd) return false;

  if(!fp) return false;

  if(!fp->IsActive()) return false;

  XBUFFER xbuffer;
  bool    status = false;

  if(Command(cmd->GetType(), xbuffer))
    {
      XBYTE* filebuffer = GEN_NEW XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
      if(filebuffer)
        {
          XBYTE   islastblock   = 0;
          XDWORD  size          = 0;
          XDWORD  crc32         = 0;
          XDWORD  sizeblock     = DIOPROTOCOL_MAXFILEBLOCK;
          XBYTE   percent       = 0;

          memset(filebuffer, 0, DIOPROTOCOL_MAXFILEBLOCK);

          if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0  , &islastblock
                                                        , &size
                                                        , &crc32
                                                        , &sizeblock
                                                        , &percent
                                                        , filebuffer
                                                        , &sizeblock))

            {
              fp->SetSize(size);
              fp->SetCRC32(crc32);
              fp->SetSizeBlock(sizeblock);

              if(fp->GetSizeBlock()) status = fp->WriteBlock(filebuffer);

              if(islastblock)
                {
                  XDWORD valuecrc = CRC32OfFile((*fp->GetTargetName()));
                  if(crc32!=valuecrc) fp->Erase();

                  fp->SetActive(false);
                }
            }

          GEN_DELETE_ARRAY filebuffer;
        }
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), status, NULL, 0);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_GetFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV get file block
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  DIOPROTOCOL_FILE*   fp;
  int                 ssize  = 0;
  bool                status = false;

  XBYTE* filebuffer = GEN_NEW XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      memset(filebuffer, 0, DIOPROTOCOL_MAXFILEBLOCK);

      fp = protocol->GetFileProgress();
      if(fp)
        {
          XBYTE islastblock = 0;

          fp->SetSizeBlock(DIOPROTOCOL_MAXFILEBLOCK);

          fp->ReadBlock(filebuffer);

          //if(fp->GetSizeBlock()!=DIOPROTOCOL_MAXFILEBLOCK)  islastblock = 1;
          if(fp->GetSize() == fp->GetPosition()) islastblock = 1;

          if(fp->GetSizeBlock())
            {
              xbuffer.Delete();
              ssize = xbuffer.AddWithMask(cmd->GetSOUTMask(), islastblock, fp->GetSize(), fp->GetCRC32(), fp->GetSizeBlock(), fp->GetPercent(), filebuffer, fp->GetSizeBlock());



              param  = 1;
              status = true;
            }

          if(islastblock) fp->SetActive(false);
        }

      GEN_DELETE_ARRAY filebuffer;
    }

  if(!status) ssize = 0;

  return ssize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_SendFileBlock(DIOPROTOCOL_FILE* fp)
* @brief      CMD send file block
* @ingroup    DATAIO
* 
* @param[in]  fp : Fp pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendFileBlock(DIOPROTOCOL_FILE* fp)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK);
  if(!cmd) return false;

  if(!fp) return false;

  if(!fp->IsActive()) return false;

  XBUFFER xbuffer;
  bool    status = false;


  XBYTE* filebuffer = GEN_NEW XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      XBYTE islastblock = 0;

      fp->SetSizeBlock(DIOPROTOCOL_MAXFILEBLOCK);

      fp->ReadBlock(filebuffer);

      //if(fp->GetSizeBlock()!=DIOPROTOCOL_MAXFILEBLOCK)  islastblock = 1;
      if(fp->GetSize() == fp->GetPosition()) islastblock = 1;

      if(fp->GetSizeBlock())
        {
          xbuffer.Delete();
          XDWORD size = xbuffer.AddWithMask(cmd->GetSINMask(), islastblock, fp->GetSize(), fp->GetCRC32(), fp->GetSizeBlock(), fp->GetPercent(), filebuffer, fp->GetSizeBlock());


          if(Command(cmd->GetType(), xbuffer))
            {
              XBYTE result = 0;

              if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result))
                {
                  if(result) status = true;
                }

              if(islastblock) fp->SetActive(false);
            }
        }

      GEN_DELETE_ARRAY filebuffer;
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), status, NULL, 0);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_SendFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV send file block
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XDWORD  filesize    = 0;
  XDWORD  crc32       = 0;
  XDWORD  sizeblock   = DIOPROTOCOL_MAXFILEBLOCK;
  XBYTE percent     = 0;
  int   ssize       = 0;
  bool  status      = false;

  XBYTE* filebuffer = GEN_NEW XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      XBYTE islastblock = 0;

      memset(filebuffer, 0, DIOPROTOCOL_MAXFILEBLOCK);

      xbuffer.GetWithMask(cmd->GetSINMask(), 0, &islastblock, &filesize, &crc32, &sizeblock, &percent, filebuffer, &sizeblock);

      DIOPROTOCOL_FILE* fp = protocol->GetFileProgress();
      if(fp)
        {
          fp->SetSize(filesize);
          fp->SetCRC32(crc32);
          fp->SetSizeBlock(sizeblock);

          fp->WriteBlock(filebuffer);

          if(fp->GetSizeBlock())
            {
              xbuffer.Delete();
              ssize = xbuffer.AddWithMask(cmd->GetSOUTMask(), fp->GetSizeBlock()?1:0);

              param  = 1;
              status = true;
            }

          if(islastblock)
            {
              XDWORD valuecrc = protocol->CRC32OfFile((*fp->GetTargetName()));
              if(crc32!=valuecrc) fp->Erase();

              fp->SetActive(false);
            }
        }

      GEN_DELETE_ARRAY filebuffer;
    }

  if(!status) ssize = 0;

  return ssize;
}


#ifdef DIO_ALERTS_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL::CMD_SendAlert(DIOALERT* alert)
* @brief      CMD send alert
* @ingroup    DATAIO
* 
* @param[in]  alert : Alert pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendAlert(DIOALERT* alert)
{
  if(!alert) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDALERT);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XSTRING  datetimestr;
  XDWORD   applicationversion;
  XDWORD   applicationsubversion;
  XDWORD   applicationsubversionerr;
  bool     result     = false;

  alert->GetDateTime()->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datetimestr);
  alert->Application_GetVersion(applicationversion, applicationsubversion, applicationsubversionerr);

  xbuffer.AddWithMask(cmd->GetSINMask(), &datetimestr, alert->Application_GetID(), applicationversion, applicationsubversion, applicationsubversionerr, alert->GetID(), (XBYTE)alert->GetLevel(), alert->GetOrigin(), alert->GetTitle(), alert->Get_Message());

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL::RCV_SendAlert(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
* @brief      RCV send alert
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  cmd : Cmd pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  rID : R ID value.
* @param[in]  param : Param value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendAlert(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING  datetimestr;
  XSTRING  applicationID;
  XDWORD   applicationversion       = 0;
  XDWORD   applicationsubversion    = 0;
  XDWORD   applicationsubversionerr = 0;
  XDWORD   ID                       = 0;
  XDWORD   type                     = 0;
  XBYTE    level                    = 0;
  XSTRING  origin;
  XSTRING  title;
  XSTRING  message;
  bool     result                   = false;

  datetimestr.AdjustSize(_MAXSTR);
  applicationID.AdjustSize(_MAXSTR);
  origin.AdjustSize(_MAXSTR);
  title.AdjustSize(_MAXSTR);
  message.AdjustSize(2048);

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &datetimestr, &applicationID, &applicationversion, &applicationsubversion, &applicationsubversionerr, &ID, &type, &level, &origin, &title, &message);

  datetimestr.AdjustSize();
  applicationID.AdjustSize();
  origin.AdjustSize();
  title.AdjustSize();
  message.AdjustSize();

  if((!title.IsEmpty())  && (!origin.IsEmpty()) && (!message.IsEmpty()))
    {
      DIOALERT alert;

      alert.GetDateTime()->GetDateTimeFromString(datetimestr, XDATETIME_FORMAT_STANDARD);
      alert.Application_GetID()->Set(applicationID);
      alert.Application_SetVersion(applicationversion, applicationsubversion, applicationsubversionerr);
      alert.SetID(ID);      
      alert.SetLevel((DIOALERTLEVEL)level);
      alert.GetOrigin()->Set(origin);
      alert.GetTitle()->Set(title);
      alert.Get_Message()->Set(message);

      if(protocol->GetApplicationData())
        {
          DIOPROTOCOL_APPLICATIONDATA* applicationdata = (DIOPROTOCOL_APPLICATIONDATA*)protocol->GetApplicationData();
          if(applicationdata)  result = applicationdata->AddAlert(alert);

          if(result)
            {
              DIOPROTOCOL_XEVENT xevent(protocol, DIOPROTOCOL_XEVENT_TYPE_ADDALERT);

              xevent.SetProtocol(protocol);
              xevent.SetAlert(&alert);

              protocol->PostEvent(&xevent);
            }
        }
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::ThreadReceived(void* param)
* @brief      Thread received
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::ThreadReceived(void* param)
{
  DIOPROTOCOL* protocol = (DIOPROTOCOL*)param;
  if(!protocol) return;

  if(protocol->doexit) return;

  if(!protocol->xthreadreceived)              return;
  if(!protocol->xthreadreceived->IsRunning()) return;

  if(protocol->GetDIOStream())
    {
      if(protocol->IsConnected())
        {
          protocol->Received();
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::ThreadAsks(void* param)
* @brief      Thread asks
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::ThreadAsks(void* param)
{
  DIOPROTOCOL* protocol = (DIOPROTOCOL*)param;
  if(!protocol) return;

  if(protocol->doexit) return;

  if(!protocol->xthreadreceived)              return;
  if(!protocol->xthreadreceived->IsRunning()) return;

  if(protocol->GetDIOStream())
    {
      if(protocol->IsConnected())
        {
          if(!protocol->asks.IsEmpty())
            {
              DIOPROTOCOL_ANSWER* ask = protocol->GetAsk(0);
              if(ask)
                {
                  XDWORD t = ask->GetType();
                  protocol->ReceivedHandle(ask->GetType(), ask->GetID(), ask->GetBuffer());
                  protocol->DeleteAsk(0);
                }
            }

          //if(protocol->xtimer) protocol->GEN_XSLEEP.MilliSeconds(50);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL::Clean()
{
  diostream                     = NULL;

  protocolversion               = DIOPROTOCOL_VERSION;
  protocolsubversion            = DIOPROTOCOL_SUBVERSION;
  protocolsubversionerr         = DIOPROTOCOL_SUBVERSIONERR;

  applicationversion            = 0;
  applicationsubversion         = 0;
  applicationsubversionerr      = 0;

  isinitialized                 = false;
  isserverinitialized           = false;

  xdatetime                     = NULL;
  xtimer                        = NULL;
  watchdog                      = NULL;

  xdir                          = NULL;

  connectionsmanager             = NULL;
  connection                     = NULL;
  applicationdata               = NULL;
  isownapplicationdata          = false;

  maskID                        = 0;

  IDmsgcounter                  = 1;
  timerfileprogress             = NULL;

  xdir                          = NULL;

  doexit                        = false;

  xmutexanswers                 = NULL;
  xmutexasks                    = NULL;
  xmutexcommands                = NULL;
  xmutexreads                   = NULL;
  xmutexwrites                  = NULL;
  xmutexfilesprogress           = NULL;

  xthreadreceived               = NULL;
  xthreadasks                   = NULL;
}





