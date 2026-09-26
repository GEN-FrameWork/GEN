/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDateTime.h
* 
* @class      XDATETIME
* @brief      eXtended Utils date time class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XPath.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XDATETIME_DAYWEEK
{
  XDATETIME_DAYWEEK_SUNDAY          = 0 ,
  XDATETIME_DAYWEEK_MONDAY              ,
  XDATETIME_DAYWEEK_TUESDAY             ,
  XDATETIME_DAYWEEK_WEDNESDAY           ,
  XDATETIME_DAYWEEK_THURSDAY            ,
  XDATETIME_DAYWEEK_FRIDAY              ,
  XDATETIME_DAYWEEK_SATURDAY            ,
};

enum XDATETIME_SENTENCES
{
  XDATETIME_SENTENCE_UNKNOWN        = 0 ,
  XDATETIME_SENTENCE_YEARS              ,
  XDATETIME_SENTENCE_YEAR               ,
  XDATETIME_SENTENCE_MONTHS             ,
  XDATETIME_SENTENCE_MONTH              ,
  XDATETIME_SENTENCE_DAYS               ,
  XDATETIME_SENTENCE_DAY                ,
  XDATETIME_SENTENCE_HOURS              ,
  XDATETIME_SENTENCE_HOUR               ,
  XDATETIME_SENTENCE_MINUTES            ,
  XDATETIME_SENTENCE_MINUTE             ,
  XDATETIME_SENTENCE_SECONDS            ,
  XDATETIME_SENTENCE_SECOND             ,
  XDATETIME_SENTENCE_JANUARY            ,
  XDATETIME_SENTENCE_FEBRUARY           ,
  XDATETIME_SENTENCE_MARCH              ,
  XDATETIME_SENTENCE_APRIL              ,
  XDATETIME_SENTENCE_MAY                ,
  XDATETIME_SENTENCE_JUNE               ,
  XDATETIME_SENTENCE_JULY               ,
  XDATETIME_SENTENCE_AUGUST             ,
  XDATETIME_SENTENCE_SEPTEMBER          ,
  XDATETIME_SENTENCE_OCTOBER            ,
  XDATETIME_SENTENCE_NOVEMBER           ,
  XDATETIME_SENTENCE_DECEMBER           ,

  XDATETIME_SENTENCE_LAST
};

#define XDATETIME_FORMAT_ADDDATE                            0x00000001                        // Add date in format of string
#define XDATETIME_FORMAT_ADDTIME                            0x00000002                        // Add time in format of string
#define XDATETIME_FORMAT_ADDDAYOFWEEK                       0x00000004                        // Add day of the week + ( "monday", "tuesday", etc...)       [Not applicable in ISO 8901]
#define XDATETIME_FORMAT_FIRSTDATEDAY                       0x00000010                        // First goes the day in the date DD MM YYYY                  [Not applicable in ISO 8901] 
#define XDATETIME_FORMAT_FIRSTDATEMONTH                     0x00000020                        // First goes the month in the date MM DD YYYY                [No applicable in ISO 8901] 
#define XDATETIME_FORMAT_FIRSTDATEYEAR                      0x00000040                        // First goes the year in the date YYYY MM DD                 [Always applicable in ISO 8901] 
#define XDATETIME_FORMAT_FIRSTTIME                          0x00000080                        // First goes the time and then the date                      [Not applicable in ISO 8901] 
#define XDATETIME_FORMAT_FIRSTDAYOFWEEK                     0x00000100                        // First goes the day of week                                 [Not applicable in ISO 8901] 
#define XDATETIME_FORMAT_TIMEWITHSECONDS                    0x00000200                        // Add Seconds to time 
#define XDATETIME_FORMAT_TIMEWITHMILLISECONDS               0x00000400                        // Add Milliseconds to time   
#define XDATETIME_FORMAT_DATEWITHDASH                       0x00000800                        // Date separated by Dash                                     [Always applicable in ISO 8901]                            
#define XDATETIME_FORMAT_TEXTMONTH                          0x00001000                        // Month in text (not numeric) : January, February, etc...    [Not applicable in ISO 8901]
#define XDATETIME_FORMAT_ISO8601_BASIC                      0x00002000                        // Basic Representation
#define XDATETIME_FORMAT_ISO8601_UTC                        0x00004000                        // UTC Representation
#define XDATETIME_FORMAT_ISO8601_ADDHOUROFFSET              0x00008000                        // Add Hour Offset 

#define XDATETIME_FORMAT_STANDARD                           (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_FIRSTDATEDAY  | XDATETIME_FORMAT_ADDTIME         | XDATETIME_FORMAT_TIMEWITHSECONDS)
#define XDATETIME_FORMAT_POSTGRESQL                         (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_FIRSTDATEYEAR | XDATETIME_FORMAT_ADDTIME         | XDATETIME_FORMAT_TIMEWITHSECONDS)
#define XDATETIME_FORMAT_ISO8601_STANDARD                   (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_ADDTIME       | XDATETIME_FORMAT_TIMEWITHSECONDS)
#define XDATETIME_FORMAT_DMY                                (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_FIRSTDATEDAY)
#define XDATETIME_FORMAT_YMD                                (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_FIRSTDATEYEAR)
#define XDATETIME_FORMAT_MDY                                (XDATETIME_FORMAT_ADDDATE | XDATETIME_FORMAT_FIRSTDATEMONTH)

#define XDATETIME_FORMAT_TIMEONLY                           (XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS)

#define XDATETIME_SECONDSINYEAR                             31556926
#define XDATETIME_SECONDSINMONTH                            2629743   //2629743.83
#define XDATETIME_SECONDSINDAY                              86400
#define XDATETIME_SECONDSINHOUR                             3600

#define XDATETIME_SECONDSYEARS(allseconds)                  (XDWORD)(allseconds / XDATETIME_SECONDSINYEAR)
#define XDATETIME_SECONDSMONTHS(allseconds)                 (XDWORD)(allseconds % XDATETIME_SECONDSINYEAR) / XDATETIME_SECONDSINMONTH
#define XDATETIME_SECONDSDAYS(allseconds)                   (XDWORD)((allseconds % XDATETIME_SECONDSINYEAR) % XDATETIME_SECONDSINMONTH) / XDATETIME_SECONDSINDAY
#define XDATETIME_SECONDSHOURS(allseconds)                  (XDWORD)(((allseconds % XDATETIME_SECONDSINYEAR) % XDATETIME_SECONDSINMONTH) % XDATETIME_SECONDSINDAY) / XDATETIME_SECONDSINHOUR
#define XDATETIME_SECONDSMINUTES(allseconds)                (XDWORD)((((allseconds % XDATETIME_SECONDSINYEAR) % XDATETIME_SECONDSINMONTH) % XDATETIME_SECONDSINDAY) % XDATETIME_SECONDSINHOUR) / 60
#define XDATETIME_SECONDSSECONDS(allseconds)                (XDWORD)((((allseconds % XDATETIME_SECONDSINYEAR) % XDATETIME_SECONDSINMONTH) % XDATETIME_SECONDSINDAY) % XDATETIME_SECONDSINHOUR) % 60

#define XDATETIME_GETACTUALTOSTRING(modificator, string)    { XDATETIME* xdatetime=GEN_XFACTORY.CreateDateTime();                 \
                                                              if(xdatetime)                                                               \
                                                                {                                                                         \
                                                                  xdatetime->Read();                                                      \
                                                                  xdatetime->GetDateTimeToString(modificator, string);                    \
                                                                  GEN_XFACTORY.DeleteDateTime(xdatetime);                                 \
                                                                }                                                                         \
                                                            }

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XDATETIME 
{
  public:

                            XDATETIME                       ();
    virtual                ~XDATETIME                       ();

    int                     GetYear                         ();
    int                     GetMonth                        ();
    int                     GetDay                          ();
    int                     GetHours                        ();
    int                     GetMinutes                      ();
    int                     GetSeconds                      ();
    int                     GetMilliSeconds                 ();

    bool                    IsZero                          ();
    bool                    IsLocal                         ();      
    bool                    IsLeapYear                      ();
    XDATETIME_DAYWEEK       GetDayOfWeek                    ();
    int                     GetDaysOfYear                   ();

    long                    GetNDaysInJulian                ();
    long                    GetNDaysInGregorian             ();
    void                    GetDateFromNDays                (long ndays, bool injulian);

    XQWORD                  GetSeconsFromDate               (bool injulian = true);
    bool                    SetDateFromSeconds              (XQWORD allseconds, bool injulian = true);

    XDWORD                  GetSecondsFromADay              ();

    void                    SetIsLocal                      (bool islocal);
    void                    SetYear                         (int year);
    void                    SetMonth                        (int month);
    void                    SetDay                          (int day);
    void                    SetHours                        (int hours);
    void                    SetMinutes                      (int minutes);
    void                    SetSeconds                      (int seconds);
    void                    SetMilliSeconds                 (int milliseconds);

    bool                    IsValidDate                     ();

    bool                    CopyFrom                        (XDATETIME* xtimesource);
    bool                    CopyFrom                        (XDATETIME& xtimesource);

    bool                    CopyTo                          (XDATETIME* xtimetarget);
    bool                    CopyTo                          (XDATETIME& xtimetarget);

    bool                    IsTheSameDay                    (XDATETIME* xtime);
    bool                    IsTheSameDay                    (XDATETIME& xtime);

    bool                    AddYears                        (int years);
    bool                    AddMonths                       (int months);
    bool                    AddDays                         (int days);
    bool                    AddHours                        (XQWORD hours);
    bool                    AddMinutes                      (XQWORD minutes);
    bool                    AddSeconds                      (XQWORD seconds);

    bool                    SubtractYears                   (int years);
    bool                    SubtractMonths                  (int months);
    bool                    SubtractDays                    (int days);
    bool                    SubtractHours                   (XQWORD hours);
    bool                    SubtractMinutes                 (XQWORD minutes);
    bool                    SubtractSeconds                 (XQWORD seconds);

    bool                    operator  ==                    (XDATETIME xtime);
    bool                    operator  !=                    (XDATETIME xtime);
    bool                    operator  >                     (XDATETIME xtime);
    bool                    operator  >=                    (XDATETIME xtime);
    bool                    operator  <                     (XDATETIME xtime);
    bool                    operator  <=                    (XDATETIME xtime);

    bool                    IsBetween                       (XDATETIME& start, XDATETIME& end);

    virtual bool            Read                            (bool islocal = true);
    virtual bool            Write                           (bool islocal = true);

    virtual bool            GetFileDateTime                 (XPATH& xpath, void* tmzip, XDWORD* dt);
    virtual bool            GetFileDateTime                 (XPATH& xpath);

    virtual int             GetMeridianDifference           ();

    virtual bool            IsDayLigthSavingTime            (int* bias = NULL);

    bool                    GetDateTimeToString             (XDWORD modificator, XSTRING& string);
    bool                    GetDateTimeToStringISO8601      (XDWORD modificator, XSTRING& string);

    bool                    GetDateTimeFromString           (XSTRING& string, XWORD modificator);    
    bool                    GetDateTimeFromStringISO8601    (XSTRING& string, XWORD modificator);

    bool                    GetMonthString                  (XSTRING& string);
    bool                    GetDayOfWeekString              (XSTRING& string);   

    int                     GetWeekOfYear                   ();
    int                     GetDayOfYear                    (); 

    XQWORD                  GetEPOCHFormat                  ();

    bool                    SetToZero                       ();
    bool                    SetDateToZero                   ();
    bool                    SetHourToZero                   ();

    bool                    Set                             (XDATETIME* datetime);
    void                    Set                             (XDATETIME& datetime);

    int                     Compare                         (XDATETIME& datetime);

  protected:

    bool                    islocal;
    int                     year;
    int                     month;
    int                     day;
    int                     hours;
    int                     minutes;
    int                     seconds;
    int                     milliseconds;

  private:

    bool                    IsModificatorActive             (XDWORD modificator, XDWORD tocheck);

    long                    GetNDaysInJulian                (int day, int month, int year);
    long                    GetNDaysInGregorian             (int day, int month, int year);

    void                    Clean                           ();

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





