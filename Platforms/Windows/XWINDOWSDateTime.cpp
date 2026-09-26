/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSDateTime.cpp
* 
* @class      XWINDOWSDATETIME
* @brief      WINDOWS eXtended Utils Data Time class
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

#include "XWINDOWSDateTime.h"

#include <cmath>

#include "XString.h"
#include "XTrace.h"
#include "XPath.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSDATETIME::XWINDOWSDATETIME()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDATETIME::XWINDOWSDATETIME()
{
  GetActualDateTime(this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSDATETIME::~XWINDOWSDATETIME()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDATETIME::~XWINDOWSDATETIME()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDATETIME::Read(bool islocal)
* @brief      Read
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  islocal : Islocal value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDATETIME::Read(bool islocal)
{
  GetActualDateTime(this, islocal);

  bool status = IsValidDate();  
  if(status)
    {
       SetIsLocal(islocal);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDATETIME::Write(bool islocal)
* @brief      Write
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  islocal : Islocal value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDATETIME::Write(bool islocal)
{
  if(IsValidDate()) 
    {
      return false;
    }

  if(IsLocal() != islocal)
    {
      return false;
    }

  SetActualDateTime(this, islocal);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath, void* tmzip, XDWORD* dt)
* @brief      Get file date time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  xpath : Path to use.
* @param[in]  tmzip : Tmzip pointer to use.
* @param[in]  dt : Dt pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath, void* tmzip, XDWORD* dt)
{
  HANDLE           hfind;
  FILETIME         ftlocal;
  WIN32_FIND_DATA  ff32;

  hfind = FindFirstFile(xpath.Get(), &ff32);
  if(hfind != INVALID_HANDLE_VALUE)
    {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftlocal);
      FileTimeToDosDateTime(&ftlocal, ((LPWORD)dt)+1, ((LPWORD)dt)+0);
      FindClose(hfind);

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath)
* @brief      Get file date time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath)
{
  HANDLE           hfind;
  FILETIME         ftlocal;
  WIN32_FIND_DATA  ff32;
  SYSTEMTIME       systemtime;

  hfind = FindFirstFile(xpath.Get(), &ff32);
  if(hfind != INVALID_HANDLE_VALUE)
    {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftlocal);
      FileTimeToSystemTime(&ftlocal, &systemtime);
      FindClose(hfind);

      year          = systemtime.wYear;
      month         = systemtime.wMonth;
      day           = systemtime.wDay;

      hours         = systemtime.wHour;
      minutes       = systemtime.wMinute;
      seconds       = systemtime.wSecond;
      milliseconds  = systemtime.wMilliseconds;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSDATETIME::GetMeridianDifference()
* @brief      Get meridian difference
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSDATETIME::GetMeridianDifference()
{
  TIME_ZONE_INFORMATION timezoneinfo = { 0 };

  ::GetTimeZoneInformation(&timezoneinfo);

  //float difference = -(float(timezoneinfo.Bias)/60);
  //return (int)difference;

  return (int)-timezoneinfo.Bias;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSDATETIME::IsDayLigthSavingTime(int* bias)
* @brief      Is day ligth saving time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  bias : Bias pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDATETIME::IsDayLigthSavingTime(int* bias)
{
  TIME_ZONE_INFORMATION timezoneinfo = { 0 };

  XDWORD ret = (XDWORD)::GetTimeZoneInformation(&timezoneinfo);

  switch(ret)
    {
      case TIME_ZONE_ID_UNKNOWN   : break;

      case TIME_ZONE_ID_INVALID   :
      case TIME_ZONE_ID_STANDARD  : if(bias)
                                      {
                                        (*bias)  = timezoneinfo.StandardBias;                                       
                                      }
                                    break;

      case TIME_ZONE_ID_DAYLIGHT  : if(bias)
                                      {
                                        (*bias)  = timezoneinfo.DaylightBias;                                        
                                      }
                                    return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSDATETIME::GetActualDateTime(XWINDOWSDATETIME* time, bool islocal)
* @brief      Get actual date time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  time : Time pointer to use.
* @param[in]  islocal : Islocal value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDATETIME::GetActualDateTime(XWINDOWSDATETIME* time, bool islocal)
{
  SYSTEMTIME st;

  if(islocal)
    {
      GetLocalTime(&st);
    }
   else
    {
      GetSystemTime(&st);
    }
  
  time->year          = st.wYear;
  time->month         = st.wMonth;
  time->day           = st.wDay;

  time->hours         = st.wHour;
  time->minutes       = st.wMinute;
  time->seconds       = st.wSecond;
  time->milliseconds  = st.wMilliseconds;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSDATETIME::SetActualDateTime(XWINDOWSDATETIME* time, bool islocal)
* @brief      Set actual date time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  time : Time pointer to use.
* @param[in]  islocal : Islocal value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDATETIME::SetActualDateTime(XWINDOWSDATETIME* time, bool islocal)
{
  SYSTEMTIME st;

  st.wYear          = time->year;
  st.wMonth         = time->month;
  st.wDay           = time->day;

  st.wHour          = time->hours;
  st.wMinute        = time->minutes;
  st.wSecond        = time->seconds;
  st.wMilliseconds  = time->milliseconds;

  if(islocal)
    {
      SetLocalTime(&st);
    }
   else
    {
      SetSystemTime(&st);
    }
}



