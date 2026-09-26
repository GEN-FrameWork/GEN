/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebWeather.cpp
* 
* @class      DIOSCRAPERWEBWEATHER
* @brief      Data Input/Output Scraper Web Weather class
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

#include "DIOScraperWebWeather.h"

#include "XThread.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBWEATHER::DIOSCRAPERWEBWEATHER() : DIOSCRAPERWEB()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBWEATHER::DIOSCRAPERWEBWEATHER() : DIOSCRAPERWEB()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBWEATHER::~DIOSCRAPERWEBWEATHER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBWEATHER::~DIOSCRAPERWEBWEATHER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::ChangeURL(XCHAR* maskurl, DIOURL& url)
* @brief      Change URL
* @ingroup    DATAIO
* 
* @param[in]  maskurl : Maskurl pointer to use.
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  url.Format(maskurl, location.Get(), iscelsius?__L("units=metric"):__L("units=imperial"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl, XSTRING* localIP, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  location : Location pointer to use.
* @param[in]  iscelsius : Iscelsius value.
* @param[in]  condition : Condition mask to evaluate.
* @param[in]  temperature : Temperature value.
* @param[in]  humidity : Humidity value.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  usecache : Usecache value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool status = false;
  if(xmutexdo) xmutexdo->Lock();

  this->location  = location;
  this->iscelsius = iscelsius;

  if(Load(DIOSCRAPERWEBWEATHER_NAMEFILE))
    {
      if(Do(DIOSCRAPERWEBWEATHER_NAMESERVICE, timeoutforurl, localIP))
        {
          XSTRING temperaturestring;
          XSTRING humiditystring;

          condition         = GetValue(__L("CONDITION"));
          temperaturestring = GetValue(__L("TEMPERATURE"));
          humiditystring    = GetValue(__L("HUMIDITY"));

          if((!temperaturestring.IsEmpty()) && (!humiditystring.IsEmpty()))
            {
              temperature = temperaturestring.ConvertToFloat();
              humidity    = humiditystring.ConvertToFloat();

              status = true;
            }
        }

    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeout, XSTRING* localIP, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  location : Location value.
* @param[in]  iscelsius : Iscelsius value.
* @param[in]  condition : Condition mask to evaluate.
* @param[in]  temperature : Temperature value.
* @param[in]  humidity : Humidity value.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  usecache : Usecache value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeout, XSTRING* localIP, bool usecache)
{
  return Get(location.Get(), iscelsius, condition, temperature, humidity, timeout, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBWEATHER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBWEATHER::Clean()
{
  iscelsius = false;
}




