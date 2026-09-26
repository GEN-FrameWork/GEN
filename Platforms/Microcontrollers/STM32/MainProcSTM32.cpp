/**-------------------------------------------------------------------------------------------------------------------
*
* @file       MainProcSTM32.cpp
*
* @class      MAINPROCSTM32
* @brief      Main Proc STM32 class
* @ingroup    PLATFORM_STM32
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "XSTM32_HAL.h"
#include "XSTM32Factory.h"
#include "XSTM32System.h"
#include "XSTM32Rand.h"
#include "XSTM32Sleep.h"
#include "XSTM32Trace.h"
#include "XSTM32EEPROMMemoryManager.h"

#ifdef DIO_ACTIVE
#include "DIOSTM32Factory.h"

  #ifdef DIO_GPIO_ACTIVE
  #include "DIOGPIO.h"
  #include "DIOSTM32GPIO.h"
  #endif

#endif

#include "VersionFrameWork.h"

#include "XThreadListNonPreemptive.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XPublisher.h"

#include "APPFlowBase.h"
#include "APPFlowMain.h"

#include "MainProcSTM32.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
XSTM32TRACE      STM32trace;
#endif
MAINPROCSTM32    mainprocstm32;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCSTM32::MAINPROCSTM32()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCSTM32::MAINPROCSTM32()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCSTM32::~MAINPROCSTM32()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCSTM32::~MAINPROCSTM32()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCSTM32::Ini(APPFLOWMAIN* appmain)
* @brief      Initialize the object
* @ingroup    PLATFORM_STM32
* 
* @param[in]  appmain : Appmain pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCSTM32::Ini(APPFLOWMAIN* appmain)
{
  this->appmain = appmain;

  if(!Factorys_Ini()) return false;

  #ifdef APPFLOW_ACTIVE

  if(!appmain)            return false;
  if(!appmain->Create())  return false;
  if(!appmain->Ini(this)) return false;

  #else

  if(Main_Proc_PlatformIni)
    {
      if(!Main_Proc_PlatformIni(this)) return false;
    }

  if(Main_Proc_Ini)
    {
      if(!Main_Proc_Ini(this)) return false;
    }

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCSTM32::Update()
* @brief      Update
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCSTM32::Update()
{
  #ifdef APPFLOW_ACTIVE
  if(appmain)
    {
      if(!appmain->Update()) return false;
    }
  #else
  if(Main_Proc_Update)
    {
      if(!Main_Proc_Update(this)) return false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCSTM32::End()
* @brief      End the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCSTM32::End()
{
  #ifdef APPFLOW_ACTIVE

  if(appmain) appmain->End();

  #else

  if(Main_Proc_End)
    {
      if(!Main_Proc_End(this)) return false;
    }

  #endif

	#ifndef MICROCONTROLLER
  DeleteAllExecParams();
	#endif

  Factorys_End();

  #ifdef APPFLOW_ACTIVE

  if(appmain) appmain->Delete();

  #else

  if(Main_Proc_PlatformEnd)
    {
      if(!Main_Proc_PlatformEnd(this)) return false;
    }

  #endif
  
  VERSION::DelInstance();
  
  XFILE_DISPLAYNOTCLOSEFILES
  XFEEDBACK_CONTROL_DISPLAYFEEDBACK
  XFEEDBACK_CONTROL_DELETERESOURCES
  XMEMORY_CONTROL_DISPLAYMEMORYLEAKS

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCSTM32::Factorys_Ini()
* @brief      Factorys ini
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCSTM32::Factorys_Ini()
{
  XFACTORY::SetInstance(GEN_NEW XSTM32FACTORY());
  if(!XFACTORY::GetIsInstanced()) return false;

  XRAND::SetInstance(GEN_NEW XSTM32RAND());
  if(!XRAND::GetIsInstanced()) return false;

  XSLEEP::SetInstance(GEN_NEW XSTM32SLEEP());
  if(!XSLEEP::GetIsInstanced()) return false;

  XSYSTEM::SetInstance(GEN_NEW XSTM32SYSTEM());
  if(!XSYSTEM::GetIsInstanced()) return false;
  
  #ifdef XEEPROMMEMORYMANAGER_ACTIVE  
  XEEPROMMEMORYMANAGER::SetInstance(GEN_NEW XSTM32EEPROMMEMORYMANAGER());
  if(!XEEPROMMEMORYMANAGER::GetIsInstanced()) return false;  
  #endif

  #ifdef DIO_ACTIVE
  DIOFACTORY::SetInstance(GEN_NEW DIOSTM32FACTORY());
  if(!DIOFACTORY::GetIsInstanced()) return false;

    #ifdef DIO_GPIO_ACTIVE
    DIOGPIO::SetInstance(GEN_NEW DIOSTM32GPIO());
    if(!DIOGPIO::GetIsInstanced()) return false;

    if(!DIOGPIO::GetInstance().Ini()) return false;
    #endif

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCSTM32::Factorys_End()
* @brief      Factorys end
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCSTM32::Factorys_End()
{
  #ifdef DIO_ACTIVE

    #ifdef DIO_GPIO_ACTIVE
    if(DIOGPIO::GetIsInstanced())
      {
        DIOGPIO::GetInstance().End();
        DIOGPIO::DelInstance();
      }
    #endif

  if(DIOFACTORY::GetIsInstanced()) DIOFACTORY::DelInstance();
  #endif
    
  #ifdef XEEPROMMEMORYMANAGER_ACTIVE    
  if(XEEPROMMEMORYMANAGER::GetIsInstanced()) XEEPROMMEMORYMANAGER::DelInstance();
  #endif
  
  #ifdef XEEPROMMEMORYMANAGER_ACTIVE
  XEEPROMMEMORYMANAGER::DelInstance();
  #endif
  
  XPATHSMANAGER::DelInstance();

  XTRANSLATION::DelInstance();
  XTRANSLATION_GEN::DelInstance();
  
  XRAND::DelInstance();

  XSLEEP::DelInstance();

  XSYSTEM::DelInstance();
  
  XPUBLISHER::DelInstance();
  
  XTHREADLISTNONPREEMPTIVE::DelInstance();
  
  #ifdef XTHREADCOLLECTED_ACTIVE
  XTHREADSCOLLECTEDMANAGER::DelInstance();
  #endif

  XFACTORY::DelInstance();
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCSTM32::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCSTM32::Clean()
{

}





/**-------------------------------------------------------------------------------------------------------------------
*
* Main entrys: main
*
* --------------------------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int main()
* @brief      main
* @ingroup    PLATFORM_STM32
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
 int main()
{
  int status = 0;

  if(mainprocstm32.Ini(&GEN_appmain))
    {
      while(mainprocstm32.Update())
        {

        }

    } else status = 1;

  if(!mainprocstm32.End()) status = 1;

  return status;
}


