/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDriveImageManager.cpp
* 
* @class      XDRIVEIMAGEMANAGER
* @brief      eXtended Utils Drive Image Manager class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDriveImageManager.h"

#include "XBase.h"
#include "XFactory.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XFile.h"

#include "XDriveImageManager_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XDRIVEIMAGEMANAGER* XDRIVEIMAGEMANAGER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER_DRIVE::XDRIVEIMAGEMANAGER_DRIVE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER_DRIVE::XDRIVEIMAGEMANAGER_DRIVE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER_DRIVE::~XDRIVEIMAGEMANAGER_DRIVE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER_DRIVE::~XDRIVEIMAGEMANAGER_DRIVE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::GetIsInstanced()
* @brief      Get is instanced
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER& XDRIVEIMAGEMANAGER::GetInstance()
* @brief      Get instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     XDRIVEIMAGEMANAGER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER& XDRIVEIMAGEMANAGER::GetInstance()
{
  if(!instance) 
    {
      instance = GEN_NEW XDRIVEIMAGEMANAGER();
    }

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::SetInstance(XDRIVEIMAGEMANAGER* _instance)
* @brief      Set instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @param[in]  _instance : GEN_NEW instance
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::SetInstance(XDRIVEIMAGEMANAGER* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::DelInstance()
* @brief      Del instance
* @note       STATIC
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XDRIVEIMAGEMANAGER_DRIVE::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XDRIVEIMAGEMANAGER_DRIVE::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XDRIVEIMAGEMANAGER_DRIVE::GetLabel()
* @brief      Get label
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XDRIVEIMAGEMANAGER_DRIVE::GetLabel()
{
  return &label;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XDRIVEIMAGEMANAGER_DRIVE::GetPath()
* @brief      Get path
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XDRIVEIMAGEMANAGER_DRIVE::GetPath()
{
  return &path;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XDRIVEIMAGEMANAGER_DRIVE::GetVolume()
* @brief      Get volume
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XDRIVEIMAGEMANAGER_DRIVE::GetVolume()
{
  return volume;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetVolume(int volume)
* @brief      Set volume
* @ingroup    XUTILS
* 
* @param[in]  volume : Volume value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetVolume(int volume)
{
  this->volume = volume;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetDeviceID()
* @brief      Get device ID
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetDeviceID()
{
  return deviceID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetDeviceID(XDWORD deviceID)
* @brief      Set device ID
* @ingroup    XUTILS
* 
* @param[in]  deviceID : Device ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void  XDRIVEIMAGEMANAGER_DRIVE::SetDeviceID(XDWORD deviceID)
{
  this->deviceID  = deviceID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetID(XQWORD ID)
* @brief      Set ID
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetID(XQWORD ID)
{
  this->ID = ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetSectorSize()
* @brief      Get sector size
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetSectorSize()
{
  return sectorsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetSectorSize(XQWORD sectorsize)
* @brief      Set sector size
* @ingroup    XUTILS
* 
* @param[in]  sectorsize : Sectorsize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetSectorSize(XQWORD sectorsize)
{
  this->sectorsize = sectorsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetNumberSectors()
* @brief      Get number sectors
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetNumberSectors()
{
  return numbersectors;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetNumberSectors(XQWORD numbersectors)
* @brief      Set number sectors
* @ingroup    XUTILS
* 
* @param[in]  numbersectors : Numbersectors value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetNumberSectors(XQWORD numbersectors)
{
  this->numbersectors = numbersectors;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XDRIVEIMAGEMANAGER_DRIVE::GetSize()
{
  return (numbersectors * sectorsize);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetSizeInGB()
* @brief      Get size in GB
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetSizeInGB()
{
  XQWORD size = GetSize();

  size  = (size / 1000000000);
  size += ((GetSize() % 1000000000) > 500)?1:0;

  return (XDWORD)size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetHandle()
* @brief      Get handle
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetHandle()
{
  return handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetHandle(XDWORD handle)
* @brief      Set handle
* @ingroup    XUTILS
* 
* @param[in]  handle : Handle value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetHandle(XDWORD handle)
{
  this->handle = handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetRawHandle()
* @brief      Get raw handle
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XDRIVEIMAGEMANAGER_DRIVE::GetRawHandle()
{
  return rawhandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::SetRawHandle(XDWORD rawhandle)
* @brief      Set raw handle
* @ingroup    XUTILS
* 
* @param[in]  rawhandle : Rawhandle value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::SetRawHandle(XDWORD rawhandle)
{
  this->rawhandle = rawhandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER_DRIVE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER_DRIVE::Clean()
{
  volume        = 0;
  deviceID      = 0;
  ID            = 0L;
  sectorsize    = 0;
  numbersectors = 0;
  handle        = 0;
  rawhandle     = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER::XDRIVEIMAGEMANAGER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER::XDRIVEIMAGEMANAGER()
{
  Clean();

  RegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_READSECTORS);
  RegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITESECTORS);
  RegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_READTOIMGFILE);
  RegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITEFROMIMGFILE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER::~XDRIVEIMAGEMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER::~XDRIVEIMAGEMANAGER()
{
  DeRegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_READSECTORS);
  DeRegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITESECTORS);
  DeRegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_READTOIMGFILE);
  DeRegisterEvent(XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITEFROMIMGFILE);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::GetDrives(XVECTOR<XDRIVEIMAGEMANAGER_DRIVE*>& drives)
* @brief      Get drives
* @ingroup    XUTILS
* 
* @param[in]  drives : Drives pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::GetDrives(XVECTOR<XDRIVEIMAGEMANAGER_DRIVE*>& drives)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::Open(XDRIVEIMAGEMANAGER_DRIVE& drive)
* @brief      Open
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::Open(XDRIVEIMAGEMANAGER_DRIVE& drive)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::Read(XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer)
* @brief      Read
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* @param[in]  startsector : Startsector value.
* @param[in]  numbersectors : Numbersectors value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::Read(XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer)
{

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::Write(XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer)
* @brief      Write
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* @param[in]  startsector : Startsector value.
* @param[in]  numbersectors : Numbersectors value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::Write(XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::ReadToIMGFile(XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock)
* @brief      Read to IMG file
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* @param[in]  pathfileIMG : Pathfile IMG value.
* @param[in]  nsectorsperblock : Nsectorsperblock value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::ReadToIMGFile(XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock)
{
  XFILE*   xfileIMG;
  XBUFFER  readbuffer;
  XQWORD   nsectorstoread = nsectorsperblock;
  XQWORD   nsectorsread   = 0;
  bool     status         = 0;

  if(!drive.GetSize()) return status;

  xfileIMG=GEN_XFACTORY.Create_File();
  if(xfileIMG)
    {
      if(xfileIMG->Create(pathfileIMG))
        {
          XQWORD indexsector=0;

          while(indexsector<drive.GetNumberSectors())
            {
              if((indexsector + nsectorstoread) > drive.GetNumberSectors()) nsectorstoread = (drive.GetNumberSectors() - indexsector);

              XDRIVEIMAGEMANAGER_XEVENT xevent(this, XDRIVEIMAGEMANAGER_XEVENT_TYPE_READTOIMGFILE);

              xevent.SetDrive(&drive);
              xevent.SetIndexSector(indexsector);
              xevent.SetNumberSectorsInOperation(nsectorstoread);
              xevent.SetError(XDRIVEIMAGEMANAGER_ERROR_NONE);

              status = Read(drive, indexsector, nsectorstoread, readbuffer);
              if(!status) xevent.SetError(XDRIVEIMAGEMANAGER_ERROR_READSECTORS);

              if(status)
                {
                  status = xfileIMG->Write(readbuffer);
                  if(!status) xevent.SetError(XDRIVEIMAGEMANAGER_ERROR_WRITEIMGFILE);
                }

              if(status) nsectorsread += nsectorstoread;

              lastoperationpercent = (((float)nsectorsread*100)/(float)drive.GetNumberSectors());

              xevent.SetError(lasterror);
              xevent.SetOperationPercent(lastoperationpercent);

              PostEvent(&xevent);

              if(!status) break;

              indexsector += nsectorstoread;
            }

          xfileIMG->Close();
        }

      GEN_XFACTORY.Delete_File(xfileIMG);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::WriteFromIMGFile(XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock)
* @brief      Write from IMG file
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* @param[in]  pathfileIMG : Pathfile IMG value.
* @param[in]  nsectorsperblock : Nsectorsperblock value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::WriteFromIMGFile(XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock)
{
  XFILE*   xfileIMG;
  XBUFFER  writebuffer;
  XQWORD   nsectorstowrite    = nsectorsperblock;
  XQWORD   nsectorswritten    = 0;
  bool     status             = 0;

  if(!drive.GetSize()) return status;

  writebuffer.Resize((XDWORD)(nsectorstowrite * drive.GetSectorSize()));

  xfileIMG=GEN_XFACTORY.Create_File();
  if(xfileIMG)
    {
      if(xfileIMG->Open(pathfileIMG, true))
        {
          XQWORD indexsector=0;

          while(indexsector<drive.GetNumberSectors())
            {
              if((indexsector + nsectorstowrite) > drive.GetNumberSectors()) nsectorstowrite = (drive.GetNumberSectors() - indexsector);

              XDRIVEIMAGEMANAGER_XEVENT xevent(this, XDRIVEIMAGEMANAGER_XEVENT_TYPE_WRITEFROMIMGFILE);

              xevent.SetDrive(&drive);
              xevent.SetIndexSector(indexsector);
              xevent.SetNumberSectorsInOperation(nsectorstowrite);
              lasterror = XDRIVEIMAGEMANAGER_ERROR_NONE;

              writebuffer.FillBuffer();

              status = xfileIMG->Read(writebuffer.Get(), (XDWORD)(nsectorstowrite * drive.GetSectorSize()));
              if(!status) lasterror = XDRIVEIMAGEMANAGER_ERROR_READIMGFILE;

              status = Write(drive, indexsector, nsectorstowrite, writebuffer);
              if(!status) lasterror = XDRIVEIMAGEMANAGER_ERROR_WRITESECTORS;

              if(status) nsectorswritten += nsectorstowrite;

              lastoperationpercent = (((float)nsectorswritten*100)/(float)drive.GetNumberSectors());

              xevent.SetError(lasterror);
              xevent.SetOperationPercent(lastoperationpercent);

              PostEvent(&xevent);

              if(!status) break;

              indexsector += nsectorstowrite;
            }

          xfileIMG->Close();
        }

      GEN_XFACTORY.Delete_File(xfileIMG);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XDRIVEIMAGEMANAGER::GetLastOperationPercent()
* @brief      Get last operation percent
* @ingroup    XUTILS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XDRIVEIMAGEMANAGER::GetLastOperationPercent()
{
  return lastoperationpercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDRIVEIMAGEMANAGER_ERROR XDRIVEIMAGEMANAGER::GetLastError()
* @brief      Get last error
* @ingroup    XUTILS
* 
* @return     XDRIVEIMAGEMANAGER_ERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDRIVEIMAGEMANAGER_ERROR XDRIVEIMAGEMANAGER::GetLastError()
{
  return lasterror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XDRIVEIMAGEMANAGER::Close(XDRIVEIMAGEMANAGER_DRIVE& drive)
* @brief      Close
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XDRIVEIMAGEMANAGER::Close(XDRIVEIMAGEMANAGER_DRIVE& drive)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XDRIVEIMAGEMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XDRIVEIMAGEMANAGER::Clean()
{
  lastoperationpercent = 0.0f;
  lasterror            = XDRIVEIMAGEMANAGER_ERROR_NONE;
}





