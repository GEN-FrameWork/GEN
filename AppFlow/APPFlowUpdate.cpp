/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowUpdate.cpp
* 
* @class      APPFLOWUPDATE
* @brief      Application Flow Update class
* @ingroup    APPFLOW
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

#include "APPFlowUpdate.h"

#include "XFactory.h"
#include "XLog.h"
#include "XSystem.h"
#include "XScheduler_XEvent.h"
#include "XScheduler.h"

#include "DIOApplicationUpdate.h"

#include "APPFlowBase.h"
#include "APPFlowCFG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPFLOWUPDATE* APPFLOWUPDATE::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/*---- CLASS APPFLOWUPDATE_CFG -------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWUPDATE_CFG::APPFLOWUPDATE_CFG()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWUPDATE_CFG::APPFLOWUPDATE_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWUPDATE_CFG::~APPFLOWUPDATE_CFG()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWUPDATE_CFG::~APPFLOWUPDATE_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* APPFLOWUPDATE_CFG::GetURL()
* @brief      Get URL
* @ingroup    APPFLOW
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* APPFLOWUPDATE_CFG::GetURL()
{
  return &URL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWUPDATE_CFG::Get_Port()
* @brief      Get port
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWUPDATE_CFG::Get_Port()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE_CFG::Set_Port(XDWORD port)
* @brief      Set port
* @ingroup    APPFLOW
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE_CFG::Set_Port(XDWORD port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_VERSIONDATA* APPFLOWUPDATE_CFG::GetAppVersion()
* @brief      Get app version
* @ingroup    APPFLOW
* 
* @return     DIOAPPLICATIONUPDATE_VERSIONDATA* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_VERSIONDATA* APPFLOWUPDATE_CFG::GetAppVersion()
{
  return &appversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWUPDATE_CFG::GetAppName()
* @brief      Get app name
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWUPDATE_CFG::GetAppName()
{
  return &appname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* APPFLOWUPDATE_CFG::GetPathRootApp()
* @brief      Get path root app
* @ingroup    APPFLOW
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* APPFLOWUPDATE_CFG::GetPathRootApp()
{
  return &xpathrootapp;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE_CFG::GetDolog()
* @brief      Get dolog
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE_CFG::GetDolog()
{
  return dolog;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE_CFG::SetDolog(bool dolog)
* @brief      Set dolog
* @ingroup    APPFLOW
* 
* @param[in]  dolog : Dolog value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE_CFG::SetDolog(bool dolog)
{
  this->dolog = dolog;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE_CFG::CopyFrom(APPFLOWUPDATE_CFG* updatecfg)
* @brief      Copy from
* @ingroup    APPFLOW
* 
* @param[in]  updatecfg : Updateconfiguration pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE_CFG::CopyFrom(APPFLOWUPDATE_CFG* updatecfg)
{
  if(!updatecfg) return false;

  URL.Set(updatecfg->GetURL()->Get());

  port = updatecfg->Get_Port();

  appversion.SetVersion(updatecfg->GetAppVersion()->GetVersion());
  appversion.SetSubVersion(updatecfg->GetAppVersion()->GetSubVersion());
  appversion.SetSubVersionError(updatecfg->GetAppVersion()->GetSubVersionError());

  appname.Set(updatecfg->GetAppName()->Get());
  xpathrootapp.Set(updatecfg->GetPathRootApp()->Get());

  dolog = updatecfg->GetDolog();


  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE_CFG::CopyTo(APPFLOWUPDATE_CFG* updatecfg)
* @brief      Copy to
* @ingroup    APPFLOW
* 
* @param[in]  updatecfg : Updateconfiguration pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE_CFG::CopyTo(APPFLOWUPDATE_CFG* updatecfg)
{
  if(!updatecfg) return false;

  updatecfg->GetURL()->Set(URL.Get());

  updatecfg->Set_Port(port);

  updatecfg->GetAppVersion()->SetVersion(appversion.GetVersion());
  updatecfg->GetAppVersion()->SetSubVersion(appversion.GetSubVersion());
  updatecfg->GetAppVersion()->SetSubVersionError(appversion.GetSubVersionError());

  updatecfg->GetAppName()->Set(appname.Get());
  updatecfg->GetPathRootApp()->Set(xpathrootapp.Get());

  updatecfg->SetDolog(dolog);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE_CFG::Clean()
{
  port = 0;
  dolog = false;
}



/*---- CLASS APPFLOWUPDATE -----------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::GetIsInstanced()
{
  return instance != NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWUPDATE& APPFLOWUPDATE::GetInstance()
* @brief      Get instance
* @ingroup    APPFLOW
* 
* @return     APPFLOWUPDATE& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWUPDATE& APPFLOWUPDATE::GetInstance()
{
  if(!instance) instance = GEN_NEW APPFLOWUPDATE();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::DelInstance()
* @brief      Del instance
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::DelInstance()
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
* @fn         bool APPFLOWUPDATE::Ini(APPFLOWCFG* cfg, APPFLOWUPDATE_CFG* updatecfg)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* @param[in]  updatecfg : Updateconfiguration pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::Ini(APPFLOWCFG* cfg, APPFLOWUPDATE_CFG* updatecfg)
{
  if(!cfg) return false;

  this->cfg = cfg;

  this->update_cfg.CopyFrom(updatecfg);

  //--------------------------------------------------------------------------------------

  if(!cfg->ApplicationUpdate_IsActive()) return true;

  //--------------------------------------------------------------------------------------

  xscheduler = GEN_NEW XSCHEDULER();
  if(!xscheduler) return false;

  CreateTaskUpdate();

  SubscribeEvent(XEVENT_TYPE_SCHEDULER, xscheduler);
  xscheduler->Ini();

  //--------------------------------------------------------------------------------------

  EventManager(APPFLOWUPDATE_EVENTOPERATION_REGISTER);

  dioappupdate = GEN_NEW DIOAPPLICATIONUPDATE(update_cfg.GetAppVersion()->GetVersion(), update_cfg.GetAppVersion()->GetSubVersion(), update_cfg.GetAppVersion()->GetSubVersionError(), update_cfg.GetAppName()->Get(), (*update_cfg.GetPathRootApp()));

  if(!dioappupdate) return false;

  dioappupdate->EliminateOldRestorations(cfg->ApplicationUpdate_GetMaxRestorations());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::EventManager(APPFLOWUPDATE_EVENTOPERATION eventoperation, XSUBJECT* xsubject, XOBSERVER* observer)
* @brief      Event manager
* @ingroup    APPFLOW
* 
* @param[in]  eventoperation : Eventoperation value.
* @param[in]  xsubject : Xsubject pointer to use.
* @param[in]  observer : Observer pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::EventManager(APPFLOWUPDATE_EVENTOPERATION eventoperation, XSUBJECT* xsubject, XOBSERVER* observer)
{
  XDWORD events[] = {  DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE                                    ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE                                    ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE                                           ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE                                              ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILES                                          ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE                                     ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES                                    ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES                                   ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE                                             ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEEXECFILE                                         ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILES                                            ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END                                                ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE                                      ,
                       DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILES                                     ,
                    };


  for(XDWORD c=0; c< sizeof(events)/sizeof(XDWORD); c++)
    {
      switch(eventoperation)
        {
          case APPFLOWUPDATE_EVENTOPERATION_REGISTER     : RegisterEvent(events[c]);                           break;
          case APPFLOWUPDATE_EVENTOPERATION_DEREGISTER   : DeRegisterEvent(events[c]);                         break;
          case APPFLOWUPDATE_EVENTOPERATION_SUBSCRIBE    : SubscribeEvent(events[c], xsubject, observer);   break;
          case APPFLOWUPDATE_EVENTOPERATION_UNSUBSCRIBE  : UnSubscribeEvent(events[c] , xsubject, observer);   break;
                                           default   : return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::Do()
* @brief      Execute the operation
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::Do()
{
  DIOAPPLICATIONUPDATE_VERSIONDATA  applicationversiondata;
  XPATH                             xpathupdate;
  XPATH                             xpathbackup;
  XSTRING                           string;
  XSTRING                           stringresult;
  bool                              status        = false;
  XSTRING                           dir_mask;
  DIOAPPLICATIONUPDATE_ERROR        error;

  dir_mask  = DIOAPPLICATIONUPDATE_RESTOREDIR_ID;
  dir_mask += DIOAPPLICATIONUPDATE_RESTOREDIR_MASK;

  if(!dioappupdate) return false;

  if(update_cfg.GetDolog()) EventManager(APPFLOWUPDATE_EVENTOPERATION_SUBSCRIBE, dioappupdate, this);

  // --------------------------------------------------------------------------------------------
  // Get last update available
  if(dioappupdate->GetVersionAvailable((*update_cfg.GetURL()), update_cfg.Get_Port(), applicationversiondata, 20, error))
    {
      // Check last update available
      if(dioappupdate->IsNewVersion(applicationversiondata))
        {
          // Have GEN_NEW version available

          // Download GEN_NEW version to update available to subdir root/update[x.x.x] Update Version
          xpathupdate = update_cfg.GetPathRootApp()->Get();
          xpathupdate.Slash_Add();
          xpathupdate.AddFormat(dir_mask.Get(), applicationversiondata.GetVersion(), applicationversiondata.GetSubVersion(), applicationversiondata.GetSubVersionError());

          status = dioappupdate->Download((*update_cfg.GetURL()), update_cfg.Get_Port(), xpathupdate, applicationversiondata, 60, error);

          //-----------------------------------------------------------------------
          DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILES);

          xevent.SetVersionData(&applicationversiondata);
          xevent.SetError(error);

          PostEvent(&xevent);
          //-----------------------------------------------------------------------

          if(status)
            {
              // New version update downloaded


              //-----------------------------------------------------------------------
              // Eliminate resources APPLICATION
              { DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES);
                xevent.SetVersionData(&applicationversiondata);
                xevent.SetError(error);

                PostEvent(&xevent);
              }
              //-----------------------------------------------------------------------


              // Actual files to be updated available to subdir root/update[x.x.x] Actual version
              xpathbackup = update_cfg.GetPathRootApp()->Get();;
              xpathbackup.Slash_Add();

              xpathbackup.AddFormat(dir_mask.Get(), update_cfg.GetAppVersion()->GetVersion(), update_cfg.GetAppVersion()->GetSubVersion(), update_cfg.GetAppVersion()->GetSubVersionError());

              // Change actual files for the GEN_NEW version
              status = dioappupdate->ChangeFilesFromDownload(applicationversiondata, xpathupdate, xpathbackup, error);

              //-----------------------------------------------------------------------
              { DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILES);

                xevent.SetVersionData(&applicationversiondata);
                xevent.SetError(error);

                PostEvent(&xevent);
              }
              //-----------------------------------------------------------------------

              if(!status)
                {
                  status = dioappupdate->Restore(xpathbackup, error);

                  //-----------------------------------------------------------------------
                  DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILES);

                  xevent.SetVersionData(&applicationversiondata);
                  xevent.SetError(error);

                  PostEvent(&xevent);
                 //-----------------------------------------------------------------------
                }

              if(status)
                {
                  if(applicationversiondata.SystemMustBeInit())
                    {
                      GEN_XSYSTEM.ShutDown(XSYSTEM_CHANGESTATUSTYPE_REBOOT);
                    }
                   else
                    {
                      // Exit & Restart of Application

                      DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END);

                      xevent.SetVersionData(&applicationversiondata);
                      xevent.SetError(error);

                      PostEvent(&xevent);

                      dioappupdate->RestartApplication(error);
                    }
                }
            }
        }
    }

  if(update_cfg.GetDolog()) EventManager(APPFLOWUPDATE_EVENTOPERATION_UNSUBSCRIBE, dioappupdate, this);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::RestorePrevious()
* @brief      Restore previous
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::RestorePrevious()
{
  DIOAPPLICATIONUPDATE_VERSIONDATA            versiontorestore;
  XVECTOR<DIOAPPLICATIONUPDATE_VERSIONDATA*>  listversionstorestore;
  DIOAPPLICATIONUPDATE_ERROR                  error   = DIOAPPLICATIONUPDATE_NOT_ERROR;
  bool                                        status  = false;

  if(!dioappupdate) return false;

  dioappupdate->GetListOfVersionsToRestore(listversionstorestore);
  if(listversionstorestore.GetSize())
    {
      if(!dioappupdate->GetPreviousVersionToRestore(listversionstorestore, versiontorestore)) return false;
    }

   //-----------------------------------------------------------------------
   // Eliminate resources APPLICATION
   { DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES);

     xevent.SetVersionData(&versiontorestore);
     xevent.SetError(error);

     PostEvent(&xevent);
   }

  //-----------------------------------------------------------------------

  status = dioappupdate->Restore(versiontorestore, error);
  if(status)
    {
      DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END);

      xevent.SetVersionData(&versiontorestore);
      xevent.SetError(error);
      PostEvent(&xevent);

      dioappupdate->RestartApplication(error);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::Restore(DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata)
* @brief      Restore
* @ingroup    APPFLOW
* 
* @param[in]  applicationversiondata : Applicationversiondata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::Restore(DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata)
{
  XPATH                       xpathbackup;
  DIOAPPLICATIONUPDATE_ERROR  error   = DIOAPPLICATIONUPDATE_NOT_ERROR;
  bool                        status  = false;

  if(!dioappupdate) return false;

   //-----------------------------------------------------------------------
   // Eliminate resources APPLICATION
   { DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES);

     xevent.SetVersionData(&applicationversiondata);
     xevent.SetError(error);

     PostEvent(&xevent);
   }

  //-----------------------------------------------------------------------

  status = dioappupdate->Restore(applicationversiondata, error);
  if(status)
    {
      DIOAPPLICATIONUPDATE_XEVENT xevent(this, DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END);

      xevent.SetVersionData(&applicationversiondata);
      xevent.SetError(error);
      PostEvent(&xevent);

      dioappupdate->RestartApplication(error);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::End()
* @brief      End the object
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::End()
{
  //--------------------------------------------------------------------------------------

  EventManager(APPFLOWUPDATE_EVENTOPERATION_DEREGISTER);

  //--------------------------------------------------------------------------------------

  if(xscheduler)
    {
      UnSubscribeEvent(XEVENT_TYPE_SCHEDULER, xscheduler);

      xscheduler->End();
      GEN_DELETE xscheduler;
      xscheduler = NULL;
    }

  //--------------------------------------------------------------------------------------

  if(dioappupdate)
    {
      GEN_DELETE dioappupdate;
      dioappupdate = NULL;
    }

  //--------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE* APPFLOWUPDATE::GetDIOAPPFlowUpdate()
* @brief      Get DIOAPP flow update
* @ingroup    APPFLOW
* 
* @return     DIOAPPLICATIONUPDATE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE* APPFLOWUPDATE::GetDIOAPPFlowUpdate()
{
  return dioappupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::LogOfEvent(DIOAPPLICATIONUPDATE_XEVENT* event)
* @brief      Log of event
* @ingroup    APPFLOW
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::LogOfEvent(DIOAPPLICATIONUPDATE_XEVENT* event)
{
  DIOAPPLICATIONUPDATE_FILE*  updatefile = event->GetActualFileToUpdate();
  XSTRING                     string;

  switch(event->GetEventType())
    {
      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_GETVERSIONAVAILABLE       : if(event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR)
                                                                          {
                                                                            string.Format(__L("Update Version available %d.%d.%d"), event->GetVersionData()->GetVersion(), event->GetVersionData()->GetSubVersion(), event->GetVersionData()->GetSubVersionError());
                                                                          }
                                                                         else
                                                                          {
                                                                            string.Format( __L("No version available to update"));
                                                                          }
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_CHECKVERSIONAVAILABLE     : string.Format(__L("Version %d.%d.%d is "), event->GetVersionData()->GetVersion(), event->GetVersionData()->GetSubVersion(), event->GetVersionData()->GetSubVersionError());

                                                                        switch(event->GetVersionComparation())
                                                                          {
                                                                            case -1 : string.Add(__L("old"));   break;
                                                                            case  0 : string.Add(__L("equal"));  break;
                                                                            case  1 : string.Add(__L("GEN_NEW"));    break;
                                                                          }

                                                                        if(event->GetVersionComparation() <=0) string.AddFormat(__L(". It will not be updated"));

                                                                        break;


      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILE              : if(updatefile) string.Format(__L("Download file %s (%dk)."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024);

                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_UNZIPFILE                 : if(updatefile) string.Format(__L("Unzip file %s (%dk) -> %s."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024, event->GetActualUnzipFileName()->Get());
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_DOWNLOADFILES             : string.Format(__L("Downloads file %s."), (event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR?__L("Ok."):__L("ERROR!")));
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILE        : if(updatefile) string.Format(__L("Backup original file %s (%dk)."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024);
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_BACKUPORIGINALFILES       : string.Format(__L("Backup Original file %s."), (event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR?__L("Ok."):__L("ERROR!")));
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_RELEASERESOURCES      : string.Format(__L("Application release blocked resources"));
                                                                        break;


      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILE                : if(updatefile) string.Format(__L("Copy update file %s (%dk)."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024);
                                                                        break;

       case DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEEXECFILE           : if(updatefile) string.Format(__L("Notify update EXEC file %s (%dk)."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024);
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_UPDATEFILES               : string.Format(__L("Update files %s."), (event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR?__L("Ok."):__L("ERROR!")));
                                                                        break;


      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_APP_END                   : string.Format(__L("Terminate application"));
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILE         : if(updatefile) string.Format(__L("Restore file %s from backup (%dk)."), updatefile->GetXPathFile()->Get(), updatefile->GetSize()/1024);
                                                                        break;

      case DIOAPPLICATIONUPDATE_XEVENT_TYPE_RESTOREUPDATEFILES        : string.Format(__L("Restore update files %s."), (event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR?__L("Ok."):__L("ERROR!")));
                                                                        break;
    }

  if(string.GetSize())
    {
      XLOG::GetInstance().AddEntry((event->GetError() == DIOAPPLICATIONUPDATE_NOT_ERROR)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR, DIOAPPLICATIONUPDATE_LOG_SECTIONID_APPFLOWUPDATE, false, string.Get());
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWUPDATE::APPFLOWUPDATE()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWUPDATE::APPFLOWUPDATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWUPDATE::~APPFLOWUPDATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWUPDATE::~APPFLOWUPDATE()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWUPDATE::CreateTaskUpdate(bool doajust)
* @brief      Create task update
* @ingroup    APPFLOW
* 
* @param[in]  doajust : Doajust value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWUPDATE::CreateTaskUpdate(bool doajust)
{
  if(!cfg) return false;

  XSCHEDULERTASK* xtask = GEN_NEW XSCHEDULERTASK(xscheduler);
  if(!xtask) return false;

  bool toactive = false;

  XDATETIME* start=GEN_XFACTORY.CreateDateTime();
  XDATETIME* end=GEN_XFACTORY.CreateDateTime();
  if(start && end)
    {
      if(!cfg->ApplicationUpdate_GetCheckTime()->IsEmpty())
        {
          start->Read();

          start->GetDateTimeFromString((*cfg->ApplicationUpdate_GetCheckTime()), XDATETIME_FORMAT_ADDTIME);
          start->SetSeconds(0);
          start->SetMilliSeconds(0);

          start->AddDays(1);

          end->Set(start);
          end->AddSeconds(5);

          xtask->SetTimeLimits(start, end);

          toactive = true;
        }
       else
        {
          int minutescadence = cfg->ApplicationUpdate_GetCheckCadence();
          if(minutescadence)
            {
              start->Read();

              if(doajust)
                {
                  int minutes = ((start->GetMinutes() / minutescadence) + ((start->GetMinutes() % minutescadence)?1:0)) * minutescadence;
                  if(minutes == start->GetMinutes()) minutes += minutescadence;
                  if(minutes >= 60)
                    {
                      start->AddHours(1);
                      minutes = 0;
                    }

                  start->SetMinutes(minutes);

                } else start->AddMinutes(minutescadence);

              start->SetSeconds(0);
              start->SetMilliSeconds(0);

              xtask->SetTimeLimits(start);

              toactive = true;
            }
        }
    }

  if(toactive)
    {
      xtask->SetID(APPFLOWUPDATE_TASKID_CHECKUPDATEAPPLICATION);
      xtask->SetNCycles(1);
      xtask->SetIsActive(true);

      xscheduler->Task_Add(xtask);
    }
   else GEN_DELETE xtask;

  GEN_XFACTORY.DeleteDateTime(start);
  GEN_XFACTORY.DeleteDateTime(end);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE::HandleEvent_Scheduler(XSCHEDULER_XEVENT* event)
* @brief      Handle event scheduler
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE::HandleEvent_Scheduler(XSCHEDULER_XEVENT* event)
{
  switch(event->GetTask()->GetID())
    {
      case APPFLOWUPDATE_TASKID_CHECKUPDATEAPPLICATION: Do();
                                                    break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE::HandleEvent_ApplicationUpdate(DIOAPPLICATIONUPDATE_XEVENT* event)
* @brief      Handle event application update
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE::HandleEvent_ApplicationUpdate(DIOAPPLICATIONUPDATE_XEVENT* event)
{
  if(update_cfg.GetDolog()) LogOfEvent(event);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_SCHEDULER           :  { XSCHEDULER_XEVENT* event = (XSCHEDULER_XEVENT*)xevent;
                                                if(!event) return;

                                                HandleEvent_Scheduler(event);
                                              }
                                              break;

      case XEVENT_TYPE_APPFLOWUPDATE       :  { DIOAPPLICATIONUPDATE_XEVENT* event = (DIOAPPLICATIONUPDATE_XEVENT*)xevent;
                                                if(!event) return;

                                                HandleEvent_ApplicationUpdate(event);
                                              }
                                              break;
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWUPDATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWUPDATE::Clean()
{
  xscheduler    = NULL;
  dioappupdate  = NULL;

  cfg           = NULL;
}




