/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSNotificationsManager.cpp
* 
* @class      DIOWINDOWSNOTIFICATIONSMANAGER
* @brief      WINDOWS Data Input/Output Notifications manager class
* @ingroup    PLATFORM_WINDOWS
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

#include "DIOWINDOWSNotificationsManager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated() const
* @brief      toast activated
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated() const 
{
  // std::wcout << L"The user clicked in this toast" << std::endl;
  // exit(0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated(int actionindex) const
* @brief      toast activated
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  actionindex : Actionindex value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated(int actionindex) const 
{
  // std::wcout << L"The user clicked on action #" << actionIndex << std::endl;
  // exit(16 + actionIndex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated(const char* response) const
* @brief      toast activated
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  response : Response value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastActivated(const char* response) const 
{
  // std::wcout << L"The user clicked on action #" << response << std::endl;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastDismissed(WinToastDismissalReason state) const
* @brief      toast dismissed
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  state : State value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastDismissed(WinToastDismissalReason state) const 
{
  switch(state) 
    {
      case UserCanceled       : // "The user dismissed this toast"       
                                break;

      case TimedOut           : // "The toast has timed out"                                
                                break;

      case ApplicationHidden  : // "The application hid the toast using ToastNotifier.hide()" 
                                break;

                    default   : // "Toast not activated"                                 
                                break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastFailed() const
* @brief      toast failed
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER::toastFailed() const 
{
  // "Error showing current toast"   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSNOTIFICATIONSMANAGER::DIOWINDOWSNOTIFICATIONSMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSNOTIFICATIONSMANAGER::DIOWINDOWSNOTIFICATIONSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSNOTIFICATIONSMANAGER::~DIOWINDOWSNOTIFICATIONSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSNOTIFICATIONSMANAGER::~DIOWINDOWSNOTIFICATIONSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSNOTIFICATIONSMANAGER::Ini(XCHAR* ownertitle, XCHAR* appname)
* @brief      Initialize the object
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ownertitle : Ownertitle pointer to use.
* @param[in]  appname : Appname pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSNOTIFICATIONSMANAGER::Ini(XCHAR* ownertitle, XCHAR* appname)
{
  if(!WinToast::isCompatible()) 
    {
        return false;
    }

  this->ownertitle = ownertitle;
  this->appname    = appname;

  WinToast::instance()->setAppUserModelId(ownertitle);
  WinToast::instance()->setAppName(appname);

  if(!WinToast::instance()->initialize()) 
    {
      return false;
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSNOTIFICATIONSMANAGER::Do(DIONOTIFICATION* notification)
* @brief      Execute the operation
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  notification : Notification pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSNOTIFICATIONSMANAGER::Do(DIONOTIFICATION* notification)
{
  if(!notification)
    {
      return false;
    }

  WinToastTemplate::AudioOption audiooption = WinToastTemplate::AudioOption::Default;
  WinToastTemplate              toast_template(WinToastTemplate::Text01);
  std::wstring                  text        = notification->Get_Message()->Get();
  std::wstring                  imagepath   = L"";
  std::wstring                  attribute   = L"default";

  toast_template.setTextField(text, WinToastTemplate::FirstLine);
  toast_template.setAudioOption(audiooption);
  //toast_template.setAttributionText(attribute);
  //toast_template.setImagePath(imagepath);

  if(WinToast::instance()->showToast(toast_template, GEN_NEW DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER() /*handler*/) < 0) 
    {
      return false;
    }
       
  return true;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSNOTIFICATIONSMANAGER::End()
* @brief      End the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSNOTIFICATIONSMANAGER::End()
{
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSNOTIFICATIONSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSNOTIFICATIONSMANAGER::Clean()
{
  handler  = NULL;
}





