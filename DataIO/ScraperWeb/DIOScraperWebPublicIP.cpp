/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebPublicIP.cpp
* 
* @class      DIOSCRAPERWEBPUBLICIP
* @brief      Data Input/Output Scraper Web Public IP class
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

#include "DIOScraperWebPublicIP.h"

#include "XThread.h"
#include "XTrace.h"
#include "DIOURL.h"



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
* @return     DIOIP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIP* DIOPUBLICIP_RESULT::Get()
{
  return &IP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPUBLICIP_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPUBLICIP_RESULT::Clean()
{

}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBPUBLICIP::DIOSCRAPERWEBPUBLICIP() : DIOSCRAPERWEB()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBPUBLICIP::DIOSCRAPERWEBPUBLICIP() : DIOSCRAPERWEB()
{
  Clean();
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
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBPUBLICIP::Get(DIOIP& IP, int timeoutforurl, XSTRING* localIP, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  IP : IP address to use.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  usecache : Usecache value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBPUBLICIP::Get(DIOIP& IP, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  XSTRING publicIPID;
  bool    status = false;

  if(xmutexdo) xmutexdo->Lock();

  publicIPID = __L("public IP ID");
  
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

  if(Load(DIOSCRAPERWEBPUBLICIP_NAMEFILE))
    {      
      if(Do(DIOSCRAPERWEBPUBLICIP_NAMESERVICE, timeoutforurl, localIP))
        {
          XSTRING stringIP;

          stringIP = GetValue(__L("IP"));

          if(!stringIP.IsEmpty())
            {
              stringIP.DeleteCharacter(0x20);
              if(!stringIP.IsEmpty())
                {       
                  IP.Set(stringIP);

                  if(usecache)
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
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBPUBLICIP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBPUBLICIP::Clean()
{

}





