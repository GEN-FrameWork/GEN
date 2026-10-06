/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebPublicIP.cpp
* 
* @class      DIOSCRAPERWEBPUBLICIP
* @brief      Typed Public IP scraper filled by script
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

#ifdef DIO_SCRAPERWEB_PUBLICIP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebPublicIP.h"

#include "XFactory.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPUBLICIP_RESULT::DIOPUBLICIP_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPUBLICIP_RESULT::DIOPUBLICIP_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPUBLICIP_RESULT::~DIOPUBLICIP_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPUBLICIP_RESULT::~DIOPUBLICIP_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIP* DIOPUBLICIP_RESULT::Get()
* @brief      Get value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIP* DIOPUBLICIP_RESULT::Get()
{
  return &IP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPUBLICIP_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPUBLICIP_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBPUBLICIP::DIOSCRAPERWEBPUBLICIP()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBPUBLICIP::DIOSCRAPERWEBPUBLICIP()
{
  Clean();

  cache    = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBPUBLICIP_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBPUBLICIP::~DIOSCRAPERWEBPUBLICIP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBPUBLICIP::~DIOSCRAPERWEBPUBLICIP()
{
  if(cache)
    {
      cache->DeleteAll();
      GEN_DELETE cache;
    }

  if(xmutexdo)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexdo);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBPUBLICIP::Get(DIOIP& IP, int timeoutforurl, XSTRING* localIP, bool usecache)
* @brief      Get public IP via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBPUBLICIP::Get(DIOIP& IP, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  XSTRING publicIPID;
  bool    status = false;

  if(xmutexdo) xmutexdo->Lock();

  publicIPID = DIOSCRAPERWEBPUBLICIP_CACHEASK;

  if(usecache && cache)
    {
      DIOPUBLICIP_RESULT* publicIPresult = (DIOPUBLICIP_RESULT*)cache->Get(publicIPID);
      if(publicIPresult)
        {
          XSTRING IPstring;

          publicIPresult->Get()->GetXString(IPstring);
          IP.Set(IPstring.Get());

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;

  runner.SetArgInt(_L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(_L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING stringIP;

      runner.GetResult(_L("ok"), ok);
      runner.GetResult(_L("ip"), stringIP);

      if((ok.Compare(_L("1")) == 0) && (!stringIP.IsEmpty()))
        {
          stringIP.DeleteCharacter(0x20);
          if(!stringIP.IsEmpty())
            {
              IP.Set(stringIP);

              if(usecache && cache)
                {
                  DIOPUBLICIP_RESULT* publicIPresult = GEN_NEW DIOPUBLICIP_RESULT();
                  if(publicIPresult)
                    {
                      publicIPresult->Get()->Set(stringIP.Get());
                      cache->Add(publicIPID, publicIPresult);
                    }
                }

              status = true;
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBPUBLICIP::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBPUBLICIP::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBPUBLICIP::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBPUBLICIP::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBPUBLICIP::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBPUBLICIP::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
