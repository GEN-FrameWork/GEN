/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDItem.cpp
* 
* @class      SNDITEM
* @brief      Sound Item class
* @ingroup    SOUND
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

#include "SNDItem.h"

#include "XFactory.h"
#include "XTimer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM::SNDITEM()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM::SNDITEM()
{
  Clean();

  timerplay = GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM::~SNDITEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM::~SNDITEM()
{
  if(soundfile)
    {
      GEN_DELETE soundfile;
      soundfile = NULL;
    }

  if(soundnote)
    {
      GEN_DELETE soundnote;
      soundnote = NULL;
    }

  if(timerplay)
    {
      GEN_XFACTORY.DeleteTimer(timerplay);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM_TYPE SNDITEM::GetType()
* @brief      Get type
* @ingroup    SOUND
* 
* @return     SNDITEM_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM_TYPE SNDITEM::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetType(SNDITEM_TYPE type)
* @brief      Set type
* @ingroup    SOUND
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetType(SNDITEM_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDITEM::GetType(XSTRING& typestr)
* @brief      Get type
* @ingroup    SOUND
* 
* @param[in]  typestr : Typestr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDITEM::GetType(XSTRING& typestr)
{
  bool status = true;

  typestr.Empty();

  switch(type)
    {
      case SNDITEM_TYPE_UNKNOWN : 
                        default : status = false; break;

      case SNDITEM_TYPE_FILE    : typestr = _L("File");    break;
      case SNDITEM_TYPE_NOTE    : typestr = _L("Note");    break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* SNDITEM::GetID()
* @brief      Get ID
* @ingroup    SOUND
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* SNDITEM::GetID()
{
  return &ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM_STATUS SNDITEM::GetStatus()
* @brief      Get status
* @ingroup    SOUND
* 
* @return     SNDITEM_STATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM_STATUS SNDITEM::GetStatus()
{
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetStatus(SNDITEM_STATUS status)
* @brief      Set status
* @ingroup    SOUND
* 
* @param[in]  status : Status value to read or update.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetStatus(SNDITEM_STATUS status)
{
  this->status = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDITEM::GetStatus(XSTRING& statusstr)
* @brief      Get status
* @ingroup    SOUND
* 
* @param[in]  statusstr : Statusstr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDITEM::GetStatus(XSTRING& statusstr)
{
  bool result = true;

  statusstr.Empty();

  switch(status)
    {
      case SNDITEM_STATUS_NONE    : statusstr = _L("None");    break;                        
      case SNDITEM_STATUS_INI     : statusstr = _L("Ini");     break;             
      case SNDITEM_STATUS_STOP    : statusstr = _L("Stop");    break;          
      case SNDITEM_STATUS_PLAY    : statusstr = _L("Play");    break;         
      case SNDITEM_STATUS_PAUSE   : statusstr = _L("Pause");   break;            
      case SNDITEM_STATUS_END     : statusstr = _L("End");     break;  
                         default  : result = false;             break;             
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDITEM::GetNTimesPlayed()
* @brief      Get N times played
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDITEM::GetNTimesPlayed()
{
  return ntimesplayed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::AddOneNTimesPlayed()
* @brief      Add one N times played
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::AddOneNTimesPlayed()
{
  ntimesplayed++;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDITEM::GetNTimesToPlay()
* @brief      Get N times to play
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDITEM::GetNTimesToPlay()
{
  return ntimestoplay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetNTimesToPlay(int ntimestoplay)
* @brief      Set N times to play
* @ingroup    SOUND
* 
* @param[in]  ntimestoplay : Ntimestoplay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetNTimesToPlay(int ntimestoplay)
{
  this->ntimestoplay = ntimestoplay;  
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDITEM::GetCounterPlay()
* @brief      Get counter play
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDITEM::GetCounterPlay()
{
  return counterplay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetCounterPlay(int counterplay)
* @brief      Set counter play
* @ingroup    SOUND
* 
* @param[in]  counterplay : Counterplay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetCounterPlay(int counterplay)
{
  this->counterplay = counterplay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDITEM::GetPlayingTime()
* @brief      Get playing time
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDITEM::GetPlayingTime()
{
  return playingtime;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetPlayingTime(XDWORD playingtime)
* @brief      Set playing time
* @ingroup    SOUND
* 
* @param[in]  playingtime : Playingtime value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetPlayingTime(XDWORD playingtime)
{
  this->playingtime = playingtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDITEM::GetCurrentPlayingTime()
* @brief      Get current playing time
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDITEM::GetCurrentPlayingTime()
{
  return currentplayingtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetCurrentPlayingTime(XDWORD currentplayingtime)
* @brief      Set current playing time
* @ingroup    SOUND
* 
* @param[in]  currentplayingtime : Currentplayingtime value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetCurrentPlayingTime(XDWORD currentplayingtime)
{
  this->currentplayingtime = currentplayingtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* SNDITEM::GetTimerPlay()
* @brief      Get timer play
* @ingroup    SOUND
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* SNDITEM::GetTimerPlay()
{
  return timerplay;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDITEM::GetDuration()
* @brief      Get duration
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDITEM::GetDuration()
{
  XDWORD duration = 0;

  switch(type)
    {
      case SNDITEM_TYPE_UNKNOWN   : break;

      case SNDITEM_TYPE_NOTE      : if(soundnote)
                                      {
                                        duration = soundnote->GetDuration();
                                      }
                                    break;

      case SNDITEM_TYPE_FILE      : if(soundfile)
                                      {
                                        duration = soundfile->GetDuration();
                                      }
                                    break;
    }

  return duration;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDPLAYCFG* SNDITEM::GetPlayCFG()
* @brief      Get play CFG
* @ingroup    SOUND
* 
* @return     SNDPLAYCFG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDPLAYCFG* SNDITEM::GetPlayCFG()
{
  return &playCFG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDITEM::SetPlayCFG(SNDPLAYCFG& playCFG)
* @brief      Set play CFG
* @ingroup    SOUND
* 
* @param[in]  playCFG : Play configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDITEM::SetPlayCFG(SNDPLAYCFG& playCFG)
{
  return this->playCFG.CopyFrom(playCFG);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE* SNDITEM::GetSoundFile()
* @brief      Get sound file
* @ingroup    SOUND
* 
* @return     SNDFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE* SNDITEM::GetSoundFile()
{
  return soundfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetSoundFile(SNDFILE* soundfile)
* @brief      Set sound file
* @ingroup    SOUND
* 
* @param[in]  soundfile : Soundfile pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetSoundFile(SNDFILE* soundfile)
{
  this->soundfile = soundfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDNOTE* SNDITEM::GetSoundNote()
* @brief      Get sound note
* @ingroup    SOUND
* 
* @return     SNDNOTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDNOTE* SNDITEM::GetSoundNote()
{
  return soundnote;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::SetSoundNote(SNDNOTE* soundnote)
* @brief      Set sound note
* @ingroup    SOUND
* 
* @param[in]  soundnote : Soundnote pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::SetSoundNote(SNDNOTE* soundnote)
{
  this->soundnote = soundnote;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDITEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDITEM::Clean()
{
  type            = SNDITEM_TYPE_UNKNOWN;    
  status          = SNDITEM_STATUS_NONE;

  ntimesplayed    = 0;
  ntimestoplay    = 0;
  counterplay     = 0;

  timerplay       = NULL;

  playingtime     = 0;

  soundfile       = NULL;
  soundnote       = NULL;
}




