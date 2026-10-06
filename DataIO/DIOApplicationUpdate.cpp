/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOApplicationUpdate.cpp
* 
* @class      DIOAPPLICATIONUPDATE
* @brief      Data Input/Output Application Update class
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

#include "DIOApplicationUpdate.h"

#include "XFactory.h"
#include "XPath.h"
#include "XPathsManager.h"
#include "XDir.h"
#include "XFileINI.h"
#include "XFileZIP.h"
#include "XString.h"
#include "XSystem.h"
#include "XProcessManager.h"
#include "XTrace.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOWebClient.h"
#include "DIOApplicationUpdate_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_VERSIONDATA::DIOAPPLICATIONUPDATE_VERSIONDATA()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_VERSIONDATA::DIOAPPLICATIONUPDATE_VERSIONDATA()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_VERSIONDATA::~DIOAPPLICATIONUPDATE_VERSIONDATA()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_VERSIONDATA::~DIOAPPLICATIONUPDATE_VERSIONDATA()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetVersion()
* @brief      Get version
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetVersion()
{
  return version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_VERSIONDATA::SetVersion(XDWORD version)
* @brief      Set version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_VERSIONDATA::SetVersion(XDWORD version)
{
  this->version = version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetSubVersion()
* @brief      Get sub version
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetSubVersion()
{
  return subversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSubVersion(XDWORD subversion)
* @brief      Set sub version
* @ingroup    DATAIO
* 
* @param[in]  subversion : Subversion value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSubVersion(XDWORD subversion)
{
  this->subversion = subversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetSubVersionError()
* @brief      Get sub version error
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE_VERSIONDATA::GetSubVersionError()
{
  return subversionerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSubVersionError(XDWORD subversionerror)
* @brief      Set sub version error
* @ingroup    DATAIO
* 
* @param[in]  subversionerror : Subversionerror value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSubVersionError(XDWORD subversionerror)
{
  this->subversionerror = subversionerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE_VERSIONDATA::SystemMustBeInit()
* @brief      System must be init
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE_VERSIONDATA::SystemMustBeInit()
{
  return systemmustbeinit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE_VERSIONDATA::CopyTo(DIOAPPLICATIONUPDATE_VERSIONDATA& versiondata)
* @brief      Copy to
* @ingroup    DATAIO
* 
* @param[in]  versiondata : Versiondata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE_VERSIONDATA::CopyTo(DIOAPPLICATIONUPDATE_VERSIONDATA& versiondata)
{
  versiondata.SetVersion(version);
  versiondata.SetSubVersion(subversion);
  versiondata.SetSubVersionError(subversionerror);
  versiondata.SetSystemMustBeInit(systemmustbeinit);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE_VERSIONDATA::CopyFrom(DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata)
* @brief      Copy from
* @ingroup    DATAIO
* 
* @param[in]  versiondata : Versiondata pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE_VERSIONDATA::CopyFrom(DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata)
{
  if(!versiondata) return false;

  version           = versiondata->GetVersion();
  subversion        = versiondata->GetSubVersion();
  subversionerror   = versiondata->GetSubVersionError();
  systemmustbeinit  = versiondata->SystemMustBeInit();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOAPPLICATIONUPDATE_VERSIONDATA::Compare(XDWORD version, XDWORD subversion, XDWORD subversionerror)
* @brief      Compare
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerror : Subversionerror value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOAPPLICATIONUPDATE_VERSIONDATA::Compare(XDWORD version, XDWORD subversion, XDWORD subversionerror)
{
  // Equal;
  if((GetVersion() == version) && (GetSubVersion() == subversion) && (GetSubVersionError() == subversionerror)) return 0;

  bool ismayorversion = false;

  if(GetVersion() > version)
    {
      ismayorversion = true;
    }
   else
    {
      if(GetVersion() == version)
        {
          if(GetSubVersion() > subversion)
            {
              ismayorversion = true;
            }
           else
            {
              if(GetSubVersion() == subversion)
                {
                  if(GetSubVersionError() > subversionerror)  ismayorversion = true;
                }
            }
        }
    }

  return ismayorversion?1:-1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSystemMustBeInit(bool applicationmustbeinit)
* @brief      Set system must be init
* @ingroup    DATAIO
* 
* @param[in]  applicationmustbeinit : Applicationmustbeinit value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_VERSIONDATA::SetSystemMustBeInit(bool applicationmustbeinit)
{
  this->systemmustbeinit = applicationmustbeinit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_VERSIONDATA::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_VERSIONDATA::Clean()
{
  version                 = 0;
  subversion              = 0;
  subversionerror         = 0;
  systemmustbeinit        = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_FILE::DIOAPPLICATIONUPDATE_FILE(XPATH& xpathfile, XDWORD CRC32, XQWORD size)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  xpathfile : Xpathfile value.
* @param[in]  CRC32 : CRC32 value.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_FILE::DIOAPPLICATIONUPDATE_FILE(XPATH& xpathfile, XDWORD CRC32, XQWORD size)
{
  Clean();

  this->xpathfile = xpathfile;
  this->CRC32     = CRC32;
  this->size      = size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_FILE::~DIOAPPLICATIONUPDATE_FILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_FILE::~DIOAPPLICATIONUPDATE_FILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* DIOAPPLICATIONUPDATE_FILE::GetXPathFile()
* @brief      Get X path file
* @ingroup    DATAIO
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* DIOAPPLICATIONUPDATE_FILE::GetXPathFile()
{
  return &xpathfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE_FILE::GetCRC32()
* @brief      Get CRC32
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE_FILE::GetCRC32()
{
  return CRC32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOAPPLICATIONUPDATE_FILE::GetSize()
* @brief      Get size
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOAPPLICATIONUPDATE_FILE::GetSize()
{
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE_FILE::HasSpecialControlUpdate()
* @brief      Has special control update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE_FILE::HasSpecialControlUpdate()
{
  return specialcontrolupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_FILE::SetHasSpecialControlUpdate(bool specialcontrolupdate)
* @brief      Set has special control update
* @ingroup    DATAIO
* 
* @param[in]  specialcontrolupdate : Specialcontrolupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_FILE::SetHasSpecialControlUpdate(bool specialcontrolupdate)
{
  this->specialcontrolupdate = specialcontrolupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_FILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_FILE::Clean()
{
  CRC32                 = 0;
  size                  = 0;
  specialcontrolupdate  = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE::DIOAPPLICATIONUPDATE(XDWORD applicationversion, XDWORD applicationsubversion, XDWORD applicationsubversionerror, XCHAR* applicationname, XPATH& xpathrootapp)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  applicationversion : Applicationversion value.
* @param[in]  applicationsubversion : Applicationsubversion value.
* @param[in]  applicationsubversionerror : Applicationsubversionerror value.
* @param[in]  applicationname : Application name.
* @param[in]  xpathrootapp : Xpathrootapp value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE::DIOAPPLICATIONUPDATE(XDWORD applicationversion, XDWORD applicationsubversion, XDWORD applicationsubversionerror, XCHAR* applicationname, XPATH& xpathrootapp)
{
  Clean();

  this->applicationversion          = applicationversion;
  this->applicationsubversion       = applicationsubversion;
  this->applicationsubversionerror  = applicationsubversionerror;

  this->applicationname             = applicationname;
  this->xpathrootapp                = xpathrootapp;

  #ifndef DEBUG_ACTIVE
  DeleteFilesRenamed(xpathrootapp);
  #endif

  xdir=GEN_XFACTORY.Create_Dir();
  webclient = GEN_NEW DIOWEBCLIENT;

  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE);
  RegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE::~DIOAPPLICATIONUPDATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE::~DIOAPPLICATIONUPDATE()
{
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE);

  FilesToUpdate_DeleteList();

  if(xdir)      GEN_XFACTORY.Delete_Dir(xdir);
  if(webclient) GEN_DELETE webclient;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::GetVersionAvailable(DIOURL& url, int port, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Get version available
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  port : Port number to use.
* @param[in]  updateversiondata : Updateversiondata value.
* @param[in]  timeout : Timeout value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::GetVersionAvailable(DIOURL& url, int port, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

  if(!webclient) return false;

  bool  status = false;

  DIOURL urlfile;
  XPATH  xpath;

  urlfile  = url;
  urlfile.Slash_Add();
  urlfile += DIOAPPLICATIONUPDATE_INIFILE;

  urlfile.Slash_Normalize();

  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
  xpath.Slash_Add();
  xpath += DIOAPPLICATIONUPDATE_INIFILE;

  webclient->Set_Port(port);

  status = webclient->Get(urlfile, xpath, NULL, timeout);
  if(!status)
    {
      if(stopprocess)
        {
          error = DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER;
        }
       else
        {
          error = DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE;
        }
    }
   else
    {
      error = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

      XFILEINI* ini = GEN_NEW XFILEINI();
      if(ini)
        {
          if(ini->Open(xpath))
            {
              XSTRING key;
              XSTRING value;
              int     index;

              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION           , value))  updateversiondata.SetVersion(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION        , value))  updateversiondata.SetSubVersion(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL      , value))  updateversiondata.SetSubVersionError(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT  , value))  updateversiondata.SetSystemMustBeInit(value.ConvertToBoolean());

              //----------------------------------------------------------------------------------------

              FilesToUpdate_DeleteList();

              index = 0;

              do{ key.Format(_L("%s%d"), DIOAPPLICATIONUPDATE_FILESSECTION_FILES, index);

                  ini->ReadValue(DIOAPPLICATIONUPDATE_FILESSECTION, key, value);
                  if(!value.IsEmpty())
                    {
                      XPATH     xpathfile(_MAXSTR);
                      int       sizefile = 0;
                      XDWORD    CRC32    = 0;

                      int comma=value.FindCharacter(_C(','));
                      XSTRING name;
                      value.Copy(0, comma, name);
                      value.DeleteCharacters(0, comma+1);
                      value.UnFormat(_L("%d,%08X"), &sizefile, &CRC32);
                      xpathfile.Set(name.Get());

                      if(xpathfile.GetSize() && sizefile && CRC32)
                        {
                          DIOAPPLICATIONUPDATE_FILE* updatefile = GEN_NEW DIOAPPLICATIONUPDATE_FILE(xpathfile, CRC32, sizefile);
                          if(updatefile) filestoupdate.Add(updatefile);
                        }
                    }

                  index++;

                } while(index<DIOAPPLICATIONUPDATE_MAXINDEXFILES);

              //----------------------------------------------------------------------------------------

              ini->Close();
              error = DIOAPPLICATIONUPDATE_NOT_ERROR;
            }

          GEN_DELETE ini;
       }
    }

  //----------------------------------------------------------------------------------------
  DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE);

  xevent.SetVersionData(&updateversiondata);
  xevent.SetFilesToUpdate(&filestoupdate);
  xevent.SetError(error);

  PostEvent(&xevent);
  //----------------------------------------------------------------------------------------

  XFILE* file=GEN_XFACTORY.Create_File();
  if(file)
    {
      file->Erase(xpath);
      GEN_XFACTORY.Delete_File(file);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XCHAR* addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Download
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  port : Port number to use.
* @param[in]  addtargetpath : Addtargetpath pointer to use.
* @param[in]  updateversiondata : Updateversiondata value.
* @param[in]  timeout : Timeout value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XCHAR* addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_NOT_ERROR;

  if(!xdir) return false;

  XPATH   namefile;
  XPATH   extenxion;
  DIOURL  urlfile;
  int     c;
  bool    status = false;

  if(!filestoupdate.GetSize())
    {
      error = DIOAPPLICATIONUPDATE_ERROR_NOTFILES;
      return false;
    }

  for(c=0;c<(int)filestoupdate.GetSize();c++)
    {
      XPATH xpathfile;

      progressfiletoupdate = c;

      DIOAPPLICATIONUPDATE_FILE* updatefile = (DIOAPPLICATIONUPDATE_FILE*)filestoupdate.Get(c);
      if(!updatefile)  continue;

      urlfile  = url;
      urlfile.Slash_Add();
      urlfile += updatefile->GetXPathFile()->Get();
      urlfile.Slash_Normalize();

      xpathfile.Empty();

      if(addtargetpath)
        {
          if(addtargetpath[0])
            {
              xpathfile = addtargetpath;
              xpathfile.Slash_Add();
            }
        }

      xpathfile    += updatefile->GetXPathFile()->Get();
      xpathfile.Slash_Normalize();

      if(!xpathfile.IsEmpty())
        {
          XPATH xpath;

          xpathfile.GetDriveAndPath(xpath);

          if(!xpath.IsEmpty())
            {
              if(xdir) xdir->Make(xpath);
            }
        }

      xpathfile.GetNamefileExt(namefile);
      xpathfile.GetExt(extenxion);

      HASHCRC32 hashCRC32;
      
      hashCRC32.ResetResult();
      hashCRC32.Do(xpathfile);

      if(updatefile->GetCRC32() != hashCRC32.GetResultCRC32())
        {
          webclient->Set_Port(port);

          status = webclient->Get(urlfile, xpathfile, NULL, timeout);
          if(!status)
            {
              if(stopprocess)
                        error = DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER;
                  else  error = DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE;

              break;
            }

          hashCRC32.ResetResult();
          hashCRC32.Do(xpathfile);

          if(updatefile->GetCRC32() != hashCRC32.GetResultCRC32())
            {
              error = DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE;
              status = false;
              break;

            } else status = true;

        } else status = true;


      DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE);

      xevent.SetVersionData(&updateversiondata);
      xevent.SetFilesToUpdate(&filestoupdate);
      xevent.SetIndexOfOperation(c);
      xevent.SetError(error);

      PostEvent(&xevent);


      //-------------------------------------------------------------
      // Descompress File
      if(!extenxion.Compare(_L(".zip"), true))
        {
          XFILEUNZIP* unzip = GEN_NEW XFILEUNZIP();
          if(unzip)
            {
              if(unzip->Open(xpathfile))
                {
                  XPATH xpath;
                  int   sourceindex = 0;

                  xpath = xpathfile;
                  xpath.SetOnlyDriveAndPath();

                  status = true;

                  do{ XFILECMPZIP* xfilecmp =  unzip->GotoFile(sourceindex);
                      if(!xfilecmp) break;

                      XPATH xpathcomplete;

                      xpathcomplete  = xpath;
                      xpathcomplete += xfilecmp->GetName();

                      if(xfilecmp->IsDirectory())
                        {
                          xdir->Make(xpathcomplete);
                          status = true;
                        }
                       else
                        {
                          hashCRC32.ResetResult();
                          hashCRC32.Do(xpathcomplete);

                          if(xfilecmp->GetCRC32() != hashCRC32.GetResultCRC32())
                            {
                              status = unzip->DecompressFile(sourceindex, xpath);

                            } else status = true;

                          DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE);

                          xevent.SetVersionData(&updateversiondata);
                          xevent.SetFilesToUpdate(&filestoupdate);
                          xevent.SetIndexOfOperation(c);
                          xevent.GetActualUnzipFileName()->Set(xfilecmp->GetName());
                          xevent.SetError(status?DIOAPPLICATIONUPDATE_NOT_ERROR:DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION);

                          PostEvent(&xevent);
                        }

                      sourceindex++;

                      GEN_DELETE xfilecmp;

                    } while(status);

                  unzip->Close();

                  XFILE* file=GEN_XFACTORY.Create_File();
                  if(file)
                    {
                      file->Erase(xpathfile);
                      GEN_XFACTORY.Delete_File(file);
                    }
                }

              GEN_DELETE unzip;
            }
        }

      //-------------------------------------------------------------
    }

  if(!status)  error = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XPATH& addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Download
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  port : Port number to use.
* @param[in]  addtargetpath : Addtargetpath value.
* @param[in]  updateversiondata : Updateversiondata value.
* @param[in]  timeout : Timeout value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XPATH& addtargetpath, DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  return Download(url, port, addtargetpath.Get(), updateversiondata, timeout, error);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::CopyOriginalFilesToBackup(XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Copy original files to backup
* @ingroup    DATAIO
* 
* @param[in]  xpathbackup : Xpathbackup value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::CopyOriginalFilesToBackup(XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error = DIOAPPLICATIONUPDATE_NOT_ERROR;

  if(!filestoupdate.GetSize())
    {
      error = DIOAPPLICATIONUPDATE_ERROR_NOTFILES;
      return false;
    }

  bool status        = false;

  for(XDWORD c=0; c<filestoupdate.GetSize(); c++)
    {
      XPATH   xpathbackupadd;
      XPATH   xpathrootappadd;
      XSTRING drivepath;
      bool    origenfilefound = false;

      xpathbackupadd   = xpathbackup;
      xpathbackupadd.Slash_Add();
      xpathbackupadd  += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd  = xpathrootapp;
      xpathrootappadd.Slash_Add();
      xpathrootappadd += filestoupdate.Get(c)->GetXPathFile()->Get();

      XFILE* xfile=GEN_XFACTORY.Create_File();
      if(xfile)
        {
          if(xfile->Open(xpathrootappadd))
            {
              origenfilefound = true;
              xfile->Close();
            }

          GEN_XFACTORY.Delete_File(xfile);
        }

      if(origenfilefound)
        {
          xpathbackupadd.GetDriveAndPath(drivepath);

          XDIR* xdir=GEN_XFACTORY.Create_Dir();
          if(xdir)
            {
              xdir->Make(drivepath.Get());

              GEN_XFACTORY.Delete_Dir(xdir);
            }

          status = CopyFile(xpathrootappadd, xpathbackupadd);

          //----------------------------------------------------------------
          DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE);

          xevent.SetFilesToUpdate(&filestoupdate);
          xevent.SetIndexOfOperation(c);
          xevent.SetError(status?DIOAPPLICATIONUPDATE_NOT_ERROR:DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION);

          PostEvent(&xevent);
          //----------------------------------------------------------------

          if(!status) break;

        }
    }

  if(!status) error = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::CopyUpdateFilesToOriginal(XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error, bool restore)
* @brief      Copy update files to original
* @ingroup    DATAIO
* 
* @param[in]  xpathupdate : Xpathupdate value.
* @param[in]  error : Error value.
* @param[in]  restore : Restore value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::CopyUpdateFilesToOriginal(XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error, bool restore)
{
  error = DIOAPPLICATIONUPDATE_NOT_ERROR;

  if(!filestoupdate.GetSize())
    {
      error = DIOAPPLICATIONUPDATE_ERROR_NOTFILES;
      return false;
    }

  bool status = false;

  for(XDWORD c=0; c<filestoupdate.GetSize(); c++)
    {
      XPATH   xpathupdateadd;
      XPATH   xpathrootappadd;
      XPATH   xpathrootappaddrename;
      XSTRING drivepath;

      xpathupdateadd   = xpathupdate;
      xpathupdateadd.Slash_Add();
      xpathupdateadd  += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd  = xpathrootapp;
      xpathrootappadd.Slash_Add();
      xpathrootappadd += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd.GetDriveAndPath(drivepath);

      XDIR* xdir=GEN_XFACTORY.Create_Dir();
      if(xdir)
        {
          xdir->Make(drivepath.Get());
          GEN_XFACTORY.Delete_Dir(xdir);
        }

      status = true;

      if(filestoupdate.Get(c)->HasSpecialControlUpdate())
        {
          DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEEXECFILE);

          xevent.SetFilesToUpdate(&filestoupdate);
          xevent.SetIndexOfOperation(c);
          xevent.SetError(DIOAPPLICATIONUPDATE_NOT_ERROR);

          PostEvent(&xevent);

          if(xevent.GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR)
            {
               XSTRING originfilename;

               xpathrootappadd.GetNamefileExt(originfilename);

               xpathrootappaddrename  = xpathrootappadd;
               xpathrootappaddrename.SetOnlyDriveAndPath();

               xpathrootappaddrename += DIOAPPLICATIONUPDATE_BACKUPPREFIX;
               xpathrootappaddrename += originfilename.Get();

               XFILE* xfile=GEN_XFACTORY.Create_File();
               if(xfile)
                 {
                   status = xfile->Rename(xpathrootappadd, xpathrootappaddrename);
                   GEN_XFACTORY.Delete_File(xfile);
                 }
            }
           else
            {
              error = xevent.GetError();
              break;
            }
        }

      if(status)
        {
          status = CopyFile(xpathupdateadd, xpathrootappadd);
          if(status)
            {
               if(filestoupdate.Get(c)->HasSpecialControlUpdate() && !xpathrootappaddrename.IsEmpty())
                 {
                   // Try to erase. In windows probably fails.

                   XFILE* xfile=GEN_XFACTORY.Create_File();
                   if(xfile)
                     {
                       xfile->Erase(xpathrootappaddrename);
                       GEN_XFACTORY.Delete_File(xfile);
                     }
                 }
            }
        }

      DIOAPPLICATIONUPDATE_XEVENT xevent(this, (restore?DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE:DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE));

      xevent.SetFilesToUpdate(&filestoupdate);
      xevent.SetIndexOfOperation(c);
      xevent.SetError(status?DIOAPPLICATIONUPDATE_NOT_ERROR:DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION);

      PostEvent(&xevent);

      if(!status)
        {
          error  = DIOAPPLICATIONUPDATE_ERROR_COPYORIGINALFILE;
          break;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::DeleteFilesRenamed(XPATH& xpathtocheck)
* @brief      Delete files renamed
* @ingroup    DATAIO
* 
* @param[in]  xpathtocheck : Xpathtocheck value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::DeleteFilesRenamed(XPATH& xpathtocheck)
{
  XDIRELEMENT xdirelement;
  XDIR*       xdir;
  bool        status = true;

  xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(xdir->FirstSearch(xpathtocheck, _L("*") , &xdirelement))
    {
      XPATH xpathadd;

      do{ xpathadd   = xpathtocheck;
          xpathadd.Slash_Add();
          xpathadd  += xdirelement.GetNameFile()->Get();

          if(xdirelement.GetType() == XDIRELEMENTTYPE_DIR)
            {
              if(!FilesToUpdate_CreateList(xpathadd))
                {
                  status = false;
                  break;
                }
            }
           else
            {
              int ifound = xdirelement.GetNameFile()->Find(DIOAPPLICATIONUPDATE_BACKUPPREFIX, false);
              if(ifound!= XSTRING_NOTFOUND)
                {
                  XFILE* xfile=GEN_XFACTORY.Create_File();
                  if(xfile)
                    {
                      xfile->Erase(xpathadd);
                      GEN_XFACTORY.Delete_File(xfile);
                    }
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::ChangeFilesFromDownload(DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, XPATH& xpathupdate, XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Change files from download
* @ingroup    DATAIO
* 
* @param[in]  updateversiondata : Updateversiondata value.
* @param[in]  xpathupdate : Xpathupdate value.
* @param[in]  xpathbackup : Xpathbackup value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::ChangeFilesFromDownload(DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata, XPATH& xpathupdate, XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
{
  bool status = false;

  FilesToUpdate_DeleteList();

  if(FilesToUpdate_CreateList(xpathupdate))
    {
      FilesToUpdate_EliminateFixedDir(xpathupdate);
      FilesToUpdate_CheckListChangedFiles(xpathupdate);

      if(filestoupdate.GetSize())
        {
          status = CopyOriginalFilesToBackup(xpathbackup, error);

          //----------------------------------------------------------------------------------------
          DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES);

          xevent.SetError(error);

          PostEvent(&xevent);
          //----------------------------------------------------------------------------------------

          if(status) status = CopyUpdateFilesToOriginal(xpathupdate, error, false);

        } else status = true;

    } else status = true;

  FilesToUpdate_DeleteList();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::RestartApplication(DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Restart application
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::RestartApplication(DIOAPPLICATIONUPDATE_ERROR& error)
{
  XSTRING command;
  XSTRING appname;

  appname = applicationname.Get();  
  //appname.ToLowerCase();

  command.Format(_L("%s/%s"), xpathrootapp.Get(), appname.Get());

  #ifdef WINDOWS
  command += _L(".exe");
  #endif

  GEN_XPROCESSMANAGER.Application_Execute(command.Get(), NULL);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::GetListOfVersionsToRestore(XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversionstorestore)
* @brief      Get list of versions to restore
* @ingroup    DATAIO
* 
* @param[in]  listversionstorestore : Listversionstorestore pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::GetListOfVersionsToRestore(XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversionstorestore)
{
  XDIRELEMENT xdirelement;
  XDIR*       xdir;
  XSTRING     searchstring;
  XSTRING     dir_mask;
  bool        status = true;

  xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) 
    {
      return false;
    }

  dir_mask  = DIOAPPLICATIONUPDATE_RESTOREDIR_ID;
  dir_mask += DIOAPPLICATIONUPDATE_RESTOREDIR_MASK;

  searchstring  = DIOAPPLICATIONUPDATE_RESTOREDIR_ID;
  searchstring += _L("*");

  if(xdir->FirstSearch(xpathrootapp, searchstring.Get() , &xdirelement))
    {
      do{  xdirelement.GetNameFile()->Get();

          if(xdirelement.GetType() == XDIRELEMENTTYPE_DIR)
            {
              if(!xdirelement.GetNameFile()->Find(DIOAPPLICATIONUPDATE_RESTOREDIR_ID, false))
                {
                  int version         = 0;
                  int subversion      = 0;
                  int subversionerror = 0;

                  xdirelement.GetNameFile()->UnFormat(dir_mask.Get(), &version, &subversion, &subversionerror);

                  if(version || subversion || subversionerror)
                    {
                      DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = GEN_NEW DIOAPPLICATIONUPDATE_VERSIONDATA();
                      if(versiondata)
                        {
                          versiondata->SetVersion(version);
                          versiondata->SetSubVersion(subversion);
                          versiondata->SetSubVersionError(subversionerror);

                          listversionstorestore.Add(versiondata);
                        }
                    }
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::GetPreviousVersionToRestore(XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversionstorestore, DIOAPPLICATIONUPDATE_VERSIONDATA& versiontorestore)
* @brief      Get previous version to restore
* @ingroup    DATAIO
* 
* @param[in]  listversionstorestore : Listversionstorestore pointer to use.
* @param[in]  versiontorestore : Versiontorestore value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::GetPreviousVersionToRestore(XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>& listversionstorestore, DIOAPPLICATIONUPDATE_VERSIONDATA& versiontorestore)
{
  XVECTOR<int> indexversiontorestore;

  for(XDWORD c=0; c<listversionstorestore.GetSize(); c++)
    {
      DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(c);
      if(versiondata)
        {
          int comparateversion = versiondata->Compare(applicationversion, applicationsubversion, applicationsubversionerror);
          if(comparateversion < 0)
            {
              indexversiontorestore.Add(c);
            }
        }
    }

  if(!indexversiontorestore.GetSize()) 
    {
      return false;
    }

  if(indexversiontorestore.GetSize() == 1)
    {
      DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(0);
      if(!versiondata) return false;

      versiontorestore.CopyFrom(versiondata);
    }
   else
    {
      int indexprevius = indexversiontorestore.GetLast();

      for(XDWORD c=0; c<indexversiontorestore.GetSize(); c++)
        {
          DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(indexprevius);
          DIOAPPLICATIONUPDATE_VERSIONDATA* versiondatacheck = listversionstorestore.Get(indexversiontorestore.Get(c));
          if(versiondatacheck && versiondata)
            {
              if(versiondatacheck->Compare(versiondata->GetVersion(), versiondata->GetSubVersion(), versiondata->GetSubVersionError()) == 1)
                {
                  indexprevius = indexversiontorestore.Get(c);
                }
            }
        }

      DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(indexprevius);
      if(!versiondata) return false;

      versiontorestore.CopyFrom(versiondata);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::RestorePrevious(DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Restore previous
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::RestorePrevious(DIOAPPLICATIONUPDATE_ERROR& error)
{
  DIOAPPLICATIONUPDATE_VERSIONDATA            versiontorestore;
  XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>  listversionstorestore;
  bool                                        status = false;

  GetListOfVersionsToRestore(listversionstorestore);
  if(listversionstorestore.GetSize())
    {
      if(GetPreviousVersionToRestore(listversionstorestore, versiontorestore))
        {
          status = Restore(versiontorestore, error);
        }
    }

  listversionstorestore.DeleteContents();
  listversionstorestore.DeleteAll();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::Restore(DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Restore
* @ingroup    DATAIO
* 
* @param[in]  applicationversiondata : Applicationversiondata value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Restore(DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, DIOAPPLICATIONUPDATE_ERROR& error)
{
  XPATH     xpathbackup;
  XSTRING   dir_mask;
  bool      status = true;

  dir_mask  = DIOAPPLICATIONUPDATE_RESTOREDIR_ID;
  dir_mask += DIOAPPLICATIONUPDATE_RESTOREDIR_MASK;

  xpathbackup =  xpathrootapp;
  xpathbackup.Slash_Add();
  xpathbackup.AddFormat(dir_mask.Get(), applicationversiondata.GetVersion(), applicationversiondata.GetSubVersion(), applicationversiondata.GetSubVersionError());

  status = Restore(xpathbackup, error);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::Restore(XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
* @brief      Restore
* @ingroup    DATAIO
* 
* @param[in]  xpathbackup : Xpathbackup value.
* @param[in]  error : Error value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Restore(XPATH& xpathbackup, DIOAPPLICATIONUPDATE_ERROR& error)
{
   bool status = false;

  FilesToUpdate_DeleteList();

  if(FilesToUpdate_CreateList(xpathbackup))
    {
      FilesToUpdate_EliminateFixedDir(xpathbackup);
      FilesToUpdate_CheckListChangedFiles(xpathbackup);

      if(filestoupdate.GetSize())
        {
          status = CopyUpdateFilesToOriginal(xpathbackup, error, true);
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::EliminateOldRestorations(int maxrestorations)
* @brief      Eliminate old restorations
* @ingroup    DATAIO
* 
* @param[in]  maxrestorations : Maxrestorations value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::EliminateOldRestorations(int maxrestorations)
{
  XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>  listversionstorestore;
  XVECTOR<int>                                indexversiontorestore;
  bool                                        status = false;

  GetListOfVersionsToRestore(listversionstorestore);

  for(XDWORD c=0; c<listversionstorestore.GetSize(); c++)
    {
      DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(c);
      if(versiondata)
        {
          int comparateversion = versiondata->Compare(applicationversion, applicationsubversion, applicationsubversionerror);
          if(comparateversion < 0)
            {
              indexversiontorestore.Add(c);
            }
        }
    }

  if(indexversiontorestore.GetSize())
    {
      for(XDWORD c=0; c<indexversiontorestore.GetSize()-1; c++)
        {
          for(XDWORD d=1; d<indexversiontorestore.GetSize(); d++)
            {
              DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(d);
              DIOAPPLICATIONUPDATE_VERSIONDATA* versiondatacheck = listversionstorestore.Get(indexversiontorestore.Get(d-1));
              if(versiondatacheck && versiondata)
                {
                  if(versiondatacheck->Compare(versiondata->GetVersion(), versiondata->GetSubVersion(), versiondata->GetSubVersionError()) == 1)
                    {
                      int index = indexversiontorestore.Get(c);

                      indexversiontorestore.Set(c, indexversiontorestore.Get(d));
                      indexversiontorestore.Set(d, index);
                    }
                }
            }
        }
    }

  status = true;

  int toeliminate =  indexversiontorestore.GetSize()-maxrestorations;
  if(toeliminate>0)
    {
      for(int c=0; c<toeliminate; c++)
        {
          DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata = listversionstorestore.Get(indexversiontorestore.Get(c));
          if(versiondata)
            {
              XPATH     xpath;
              XSTRING   dir_mask;

              xpath = xpathrootapp;
              xpath.Slash_Add();

              dir_mask  = DIOAPPLICATIONUPDATE_RESTOREDIR_ID;
              dir_mask += DIOAPPLICATIONUPDATE_RESTOREDIR_MASK;

              xpath.AddFormat(dir_mask.Get(), versiondata->GetVersion(), versiondata->GetSubVersion(), versiondata->GetSubVersionError());

              XDIR* xdir=GEN_XFACTORY.Create_Dir();
              if(xdir)
                {
                  status = xdir->Delete(xpath.Get(), true);
                  GEN_XFACTORY.Delete_Dir(xdir);

                  if(!status) break;
                }              
            }
        }
    }

  listversionstorestore.DeleteContents();
  listversionstorestore.DeleteAll();

  indexversiontorestore.DeleteAll();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::IsNewVersion(DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata)
* @brief      Is GEN_NEW version
* @ingroup    DATAIO
* 
* @param[in]  updateversiondata : Updateversiondata value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::IsNewVersion(DIOAPPLICATIONUPDATE_VERSIONDATA& updateversiondata)
{
  int versioncomparation = updateversiondata.Compare(applicationversion, applicationsubversion, applicationsubversionerror);

  //----------------------------------------------------------------------------------------

  DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE);

  xevent.SetVersionComparation(versioncomparation);
  xevent.SetVersionData(&updateversiondata);
  xevent.SetFilesToUpdate(&filestoupdate);
  xevent.SetError(DIOAPPLICATIONUPDATE_NOT_ERROR);

  PostEvent(&xevent);

  //----------------------------------------------------------------------------------------

  return (versioncomparation == 1)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE::Application_GetVersion()
* @brief      Application get version
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE::Application_GetVersion()
{
  return this->applicationversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE::GetApplicationSubversion()
* @brief      Get application subversion
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE::GetApplicationSubversion()
{
  return this->applicationsubversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE::GetApplicationSubVersionError()
* @brief      Get application sub version error
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE::GetApplicationSubVersionError()
{
  return this->applicationsubversionerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::FilesToUpdate_CreateList(XPATH& xpathupdate)
* @brief      Files to update create list
* @ingroup    DATAIO
* 
* @param[in]  xpathupdate : Xpathupdate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::FilesToUpdate_CreateList(XPATH& xpathupdate)
{
  XDIRELEMENT xdirelement;
  XDIR*       xdir;
  bool        status = true;

  xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(xdir->FirstSearch(xpathupdate, _L("*") , &xdirelement))
    {
      XPATH xpathadd;

      do{ xpathadd   = xpathupdate;
          xpathadd.Slash_Add();
          xpathadd  += xdirelement.GetNameFile()->Get();
        
          if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE)
            {
              HASHCRC32 CRC32file;
              XQWORD    sizefile = 0;

              CRC32file.Do(xpathadd);

              XFILE* xfile=GEN_XFACTORY.Create_File();
              if(xfile)
                {
                  if(xfile->Open(xpathadd))
                    {
                      sizefile = xfile->GetSize();
                      xfile->Close();
                    }

                  GEN_XFACTORY.Delete_File(xfile);
                }

              DIOAPPLICATIONUPDATE_FILE* updatefile = GEN_NEW DIOAPPLICATIONUPDATE_FILE(xpathadd, CRC32file.GetResultCRC32(), sizefile);
              if(updatefile)
                {
                  XSTRING filename;
                  XSTRING fileext;

                  updatefile->GetXPathFile()->GetNamefile(filename);
                  updatefile->GetXPathFile()->GetExt(fileext);

                  if(!filename.Compare(applicationname, true))
                    {
                      updatefile->SetHasSpecialControlUpdate(true);
                    }

                  filestoupdate.Add(updatefile);
                }
            }

          if(xdirelement.GetType() == XDIRELEMENTTYPE_DIR)
            {
              if(!FilesToUpdate_CreateList(xpathadd))
                {
                  status = false;
                  break;
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::FilesToUpdate_EliminateFixedDir(XPATH& xpath)
* @brief      Files to update eliminate fixed dir
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::FilesToUpdate_EliminateFixedDir(XPATH& xpath)
{
  XDWORD c = 0;

  if(!filestoupdate.GetSize()) return false;

  while(c < filestoupdate.GetSize())
    {
      DIOAPPLICATIONUPDATE_FILE* updatefile = filestoupdate.Get(c);
      if(updatefile)
        {
           updatefile->GetXPathFile()->DeleteCharacters(0, xpath.GetSize()+1);
           c++;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::FilesToUpdate_CheckListChangedFiles(XPATH& xpathupdate)
* @brief      Files to update check list changed files
* @ingroup    DATAIO
* 
* @param[in]  xpathupdate : Xpathupdate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::FilesToUpdate_CheckListChangedFiles(XPATH& xpathupdate)
{
  XDWORD c = 0;

  while(c < filestoupdate.GetSize())
    {
      DIOAPPLICATIONUPDATE_FILE* updatefile = filestoupdate.Get(c);
      if(updatefile)
        {
          HASHCRC32 CRC32file;
          XPATH     xpath;

          xpath  = xpathrootapp;
          xpath.Slash_Add();
          xpath += updatefile->GetXPathFile()->Get();

          CRC32file.Do(xpath);

          if(CRC32file.GetResultCRC32() == updatefile->GetCRC32())
            {
              filestoupdate.Delete(updatefile);
              GEN_DELETE updatefile;
            }
           else
            {
              c++;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::FilesToUpdate_DeleteList()
* @brief      Files to update GEN_DELETE list
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::FilesToUpdate_DeleteList()
{
  if(filestoupdate.IsEmpty()) return false;

  filestoupdate.DeleteContents();
  filestoupdate.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAPPLICATIONUPDATE::CopyFile(XPATH& origin, XPATH& target)
* @brief      Copy file
* @ingroup    DATAIO
* 
* @param[in]  origin : Origin value.
* @param[in]  target : Target value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::CopyFile(XPATH& origin, XPATH& target)
{
  XFILE* xfile[2] = { NULL, NULL };
  bool   status   = false;

  xfile[0]=GEN_XFACTORY.Create_File();
  xfile[1]=GEN_XFACTORY.Create_File();

  if(xfile[0] && xfile[1])
    {
      if(xfile[0]->Open(origin))
        {
          if(xfile[1]->Create(target))
            {
              #define COPYFILEBLOCKSIZE  500*1024

              XBUFFER xbuffer;
              XDWORD  br = COPYFILEBLOCKSIZE;

              xbuffer.Resize(COPYFILEBLOCKSIZE);

              do{ xfile[0]->Read(xbuffer.Get(), &br);
                  if(!br) break;

                  if(!xfile[1]->Write(xbuffer.Get(), br)) break;

                } while(br == COPYFILEBLOCKSIZE);


              xfile[1]->Close();

              status = true;
            }

          xfile[0]->Close();
        }
    }

  GEN_XFACTORY.Delete_File(xfile[0]);
  GEN_XFACTORY.Delete_File(xfile[1]);

  if(status)
    {
      status = false;

      HASHCRC32* CRC32file[2];

      CRC32file[0] = GEN_NEW HASHCRC32();
      CRC32file[1] = GEN_NEW HASHCRC32();

      if(CRC32file[0] && CRC32file[1])
        {
          CRC32file[0]->Do(origin);
          CRC32file[1]->Do(target);

          if(CRC32file[0]->GetResultCRC32() == CRC32file[1]->GetResultCRC32()) status = true;
        }

      GEN_DELETE CRC32file[0];
      GEN_DELETE CRC32file[1];
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE::Clean()
{
  applicationversion          = 0;
  applicationsubversion       = 0;
  applicationsubversionerror  = 0;

  xdir                        = NULL;
  webclient                   = NULL;

  progressfiletoupdate        = -1;
  stopprocess                = false;
}





