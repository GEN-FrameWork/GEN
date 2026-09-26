/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDOpenALFactory.cpp
* 
* @class      SNDOPENALFACTORY
* @brief      Sound OpenAL Factory class
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

#include "SNDOpenALFactory.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"

#include "SNDFactory_XEvent.h"
#include "SNDOpenALSource.h"
#include "SNDItem.h"
#include "SNDPlayCFG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALFACTORY::SNDOPENALFACTORY()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALFACTORY::SNDOPENALFACTORY() 
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALFACTORY::~SNDOPENALFACTORY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALFACTORY::~SNDOPENALFACTORY() 
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Ini()
* @brief      Initialize the object
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Ini()
{
  device = alcOpenDevice(NULL);
  if(!device) 
    {
      return false;
    }
      
  ALCenum error;
  
  context = alcCreateContext(device, NULL);
  error   = alcGetError(device);

  if(!context || (error != ALC_NO_ERROR))
    {
      alcCloseDevice(device);
      return false;
    }

  alcMakeContextCurrent(context);
  error = alcGetError(device);
  if(!context || (error != ALC_NO_ERROR))
    {
      alcCloseDevice(device);
      return false;
    }

  maxchannels   = 16;
  mastervolume  = 1.0f;

  playmutex = GEN_XFACTORY.Create_Mutex();
  if(!playmutex)
    {
      return false;
    }
  
  playthread = GEN_XFACTORY.CreateThread(XTHREADGROUPID_SOUND, __L("SNDOPENALFACTORY::SNDOPENALFACTORY"), SNDOPENALFACTORY::ThreadPlay, this);  
  if(!playthread)
    {
      return false;
    }

  if(!playthread->Ini())
    {
      return false;
    }

  soundactive = true;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Update()
* @brief      Update
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Update()
{  
  if(!soundactive)
    {
      return false;
    }

  if(playmutex)
    {
      playmutex->Lock();
    }

  for(XDWORD c=0; c<soundplayitems.GetSize(); c++)
    {
      SNDOPENALPLAYITEM* playitem = soundplayitems.Get(c);
      if(playitem)
        {
          SNDITEM*         item   = playitem->GetItem();  
          SNDOPENALSOURCE* source = playitem->GetSource();        
          if(item && source)            
            {  
              SNDNOTE* soundnote = NULL;
              SNDFILE* soundfile = NULL;

              switch(item->GetType())
                {
                  case SNDITEM_TYPE_UNKNOWN : break;

                  case SNDITEM_TYPE_NOTE    : soundnote = playitem->GetItem()->GetSoundNote();
                                              break;

                  case SNDITEM_TYPE_FILE    : soundfile = playitem->GetItem()->GetSoundFile();
                                              break;              
                }
              
              if(!soundnote && !soundfile)
                {
                  continue;
                }

              if(playitem->GetEvent() == SNDOPENALPLAYITEM_XFSMEVENT_NONE) // Not GEN_NEW event
                {
                  switch(playitem->GetCurrentState())
                    {
                      case SNDOPENALPLAYITEM_XFSMSTATE_NONE         : break;
                      case SNDOPENALPLAYITEM_XFSMSTATE_INI          : break;

                      case SNDOPENALPLAYITEM_XFSMSTATE_PLAY         : { XQWORD  millisecond = item->GetTimerPlay()->GetMeasureMilliSeconds();
                                                                        bool    outoftime   = false;                                                                       

                                                                        item->SetPlayingTime((XDWORD)item->GetPlayingTime() + (XDWORD)millisecond);
                                                                        item->SetCurrentPlayingTime((XDWORD)item->GetCurrentPlayingTime() + (XDWORD)millisecond);

                                                                        item->GetTimerPlay()->Reset();

                                                                        if(item->GetNTimesToPlay() == SNDFACTORY_INLOOP)
                                                                          {
                                                                            if(!source->IsPLaying())
                                                                              {
                                                                                outoftime = true;  
                                                                              }
                                                                          }
                                                                          else     
                                                                          { 
                                                                            if(item->GetCurrentPlayingTime() >= (item->GetDuration()))
                                                                              {
                                                                                outoftime = true;    
                                                                              }
                                                                          }                                                             

                                                                        if(outoftime)
                                                                          {
                                                                            item->SetCurrentPlayingTime(0);  
                                                                          
                                                                            if(!item->GetCounterPlay())
                                                                              {
                                                                                playitem->SetEvent(SNDOPENALPLAYITEM_XFSMEVENT_STOP); 
                                                                                item->AddOneNTimesPlayed();  
                                                                              }
                                                                              else
                                                                              {
                                                                                item->SetCounterPlay(item->GetCounterPlay()-1);

                                                                                item->GetTimerPlay()->Reset();

                                                                                source->ResetPlay();
                                                                                source->Play();
                                                                              }                                                              
                                                                          }
                                                                      }
                                                                      break;

                      case SNDOPENALPLAYITEM_XFSMSTATE_PAUSE        : break;

                      case SNDOPENALPLAYITEM_XFSMSTATE_STOP         : break;

                      case SNDOPENALPLAYITEM_XFSMSTATE_END          : break;
                    }
                }
               else //  New event
                {
                  if(playitem->GetEvent() < SNDOPENALPLAYITEM_LASTEVENT)
                    {
                      playitem->CheckTransition();

                      switch(playitem->GetCurrentState())
                        {
                          case SNDOPENALPLAYITEM_XFSMSTATE_NONE     : break;

                          case SNDOPENALPLAYITEM_XFSMSTATE_INI      : {  switch(item->GetType())
                                                                          {
                                                                            case SNDITEM_TYPE_UNKNOWN : break;

                                                                            case SNDITEM_TYPE_NOTE    : GenerateBuffer(playitem, soundnote);
                                                                                                        break;

                                                                            case SNDITEM_TYPE_FILE    : GenerateBuffer(playitem, soundfile);
                                                                                                        break;              
                                                                          }   

                                                                        if(item->GetNTimesToPlay())
                                                                          {     
                                                                            item->SetCounterPlay(item->GetNTimesToPlay()-1);                                                                
                                                                            playitem->SetEvent(SNDOPENALPLAYITEM_XFSMEVENT_PLAY);                                                              
                                                                          }

                                                                        SNDFACTORY_XEVENT xevent(this, SNDFACTORY_XEVENT_TYPE_SOUND_INI);      
                                                                        xevent.SetItem(item);
                                                                        PostEvent(&xevent); 
                                                                      }
                                                                      break;

                          case SNDOPENALPLAYITEM_XFSMSTATE_PLAY     : { item->GetTimerPlay()->Reset();
                                                                        source->Play();
                                                                        item->SetStatus(SNDITEM_STATUS_PLAY);

                                                                        SNDFACTORY_XEVENT xevent(this, SNDFACTORY_XEVENT_TYPE_SOUND_PLAY);      
                                                                        xevent.SetItem(item);
                                                                        PostEvent(&xevent); 
                                                                      }
                                                                      break;

                          case SNDOPENALPLAYITEM_XFSMSTATE_PAUSE    : { source->Pause();
                                                                        item->SetStatus(SNDITEM_STATUS_PAUSE);
                                                                        
                                                                        SNDFACTORY_XEVENT xevent(this, SNDFACTORY_XEVENT_TYPE_SOUND_PAUSE);      
                                                                        xevent.SetItem(item);
                                                                        PostEvent(&xevent); 
                                                                      }
                                                                      break;

                          case SNDOPENALPLAYITEM_XFSMSTATE_STOP     : { source->Stop(); 

                                                                        item->SetStatus(SNDITEM_STATUS_STOP);

                                                                        if(item->GetNTimesToPlay() == SNDFACTORY_INLOOP)
                                                                          {
                                                                            playitem->SetEvent(SNDOPENALPLAYITEM_XFSMEVENT_END);                                                        
                                                                          }
                                                                          else
                                                                          {     
                                                                            if(!item->GetCounterPlay())
                                                                              {
                                                                                playitem->SetEvent(SNDOPENALPLAYITEM_XFSMEVENT_END);  
                                                                              }
                                                                          }

                                                                        SNDFACTORY_XEVENT xevent(this, SNDFACTORY_XEVENT_TYPE_SOUND_STOP);      
                                                                        xevent.SetItem(item);
                                                                        PostEvent(&xevent); 

                                                                      }
                                                                      break;

                            case SNDOPENALPLAYITEM_XFSMSTATE_END    : { if(playmutex)
                                                                          {
                                                                            playmutex->Lock();
                                                                          } 
                                                          
                                                                        soundplayitems.Delete(playitem);
                                                                        GEN_DELETE playitem;

                                                                        if(playmutex)
                                                                          {
                                                                            playmutex->UnLock();
                                                                          }

                                                                        item->SetStatus(SNDITEM_STATUS_END);

                                                                        SNDFACTORY_XEVENT xevent(this, SNDFACTORY_XEVENT_TYPE_SOUND_END);      
                                                                        xevent.SetItem(item);
                                                                        PostEvent(&xevent); 
                                                                      }
                                                                      break;
                        }
                    }
                }              
            }
        }
    }

  if(playmutex)
    {
      playmutex->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::End()
* @brief      End the object
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::End()
{  
  if(!soundactive)
    {
      return false;
    }

  Sound_StopAll();

  if(playthread)
    {
      playthread->End();

      GEN_XFACTORY.DeleteThread(XTHREADGROUPID_SOUND, playthread);
      playthread = NULL;
    }
    
  if(playmutex)
    {
      GEN_XFACTORY.Delete_Mutex(playmutex);
      playmutex = NULL;
    }

  alcMakeContextCurrent(NULL);

  if(context)
    {
      alcDestroyContext(context);
      context = NULL;
    }

  if(device)
    {
      alcCloseDevice(device);
      device = NULL;
    }

  soundplayitems.DeleteContents();
  soundplayitems.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_Play(SNDITEM* item, SNDPLAYCFG* playCFG, int ntimestoplay)
* @brief      Sound play
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  playCFG : Play configuration pointer to use.
* @param[in]  ntimestoplay : Ntimestoplay value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_Play(SNDITEM* item, SNDPLAYCFG* playCFG, int ntimestoplay)
{   
  if(!soundactive)
    {
      return false;
    }

  if(!SNDFACTORY::Sound_Play(item, playCFG, ntimestoplay))
    {
      return false;
    }

  if(soundplayitems.GetSize() >= maxchannels)
    {
      return false;
    }

  SNDOPENALSOURCE* source = GEN_NEW SNDOPENALSOURCE();
  if(!source)
    {
      return false;
    }

  SNDOPENALPLAYITEM* soundplayitem = GEN_NEW SNDOPENALPLAYITEM();
  if(!soundplayitem) 
    {
      GEN_DELETE source;
      return false;
    }

  soundplayitem->SetItem(item);
  soundplayitem->SetSource(source);  

  if(!soundplayitems.Add(soundplayitem))
    {
      return false;
    }  

  if(item->GetPlayCFG()->GetVolume() != SNDFACTORY_UNDEFINED)
    {
      source->SetVolume(item->GetPlayCFG()->GetVolume());
    }

  if(item->GetPlayCFG()->GetPitch() != SNDFACTORY_UNDEFINED)
    {
      source->SetPitch(item->GetPlayCFG()->GetPitch());
    }

  if(ntimestoplay == SNDFACTORY_INLOOP)
    {
      source->SetInLoop(true);      
    }
   else
    { 
      item->SetPlayingTime(0);
      source->SetInLoop(false);
    }

  soundplayitem->SetEvent(SNDOPENALPLAYITEM_XFSMEVENT_INI);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_Pause(SNDITEM* item)
* @brief      Sound pause
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_Pause(SNDITEM* item)
{
  if(!soundactive)
    {
      return false;
    }

  if(!item)
    {
      return false;
    }

  if(item->GetStatus() != SNDITEM_STATUS_PLAY)
    {   
      return false;
    }

  if(playmutex)
    {
      playmutex->Lock();
    }

  for(XDWORD c=0; c<soundplayitems.GetSize(); c++)
    {
      SNDOPENALPLAYITEM* playitem = soundplayitems.Get(c);
      if(playitem)
        {
          SNDITEM* pitem = playitem->GetItem();          
          if(item == pitem)
            {  
              playitem->SetEvent(SNDOPENALPLAYITEM_XFSMSTATE_PAUSE);              
            }
        }
    }

  if(playmutex)
    {
      playmutex->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_Stop(SNDITEM* item)
* @brief      Sound stop
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_Stop(SNDITEM* item)
{  
  if(!soundactive)
    {
      return false;
    }

  if(!item)
    {
      return false;
    }

  if((item->GetStatus() != SNDITEM_STATUS_PLAY) && (item->GetStatus() != SNDITEM_STATUS_PAUSE))
    {   
      return false;
    }

  for(XDWORD c=0; c<soundplayitems.GetSize(); c++)
    {
      SNDOPENALPLAYITEM* playitem = soundplayitems.Get(c);
      if(playitem)
        {
          SNDITEM* pitem = playitem->GetItem();          
          if(item == pitem)
            {  
              playitem->SetEvent(SNDOPENALPLAYITEM_XFSMSTATE_STOP);              
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_StopAll()
* @brief      Sound stop all
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_StopAll()
{   
  if(!soundactive)
    {
      return false;
    }

  for(XDWORD c=0; c<soundplayitems.GetSize(); c++)
    {
      SNDOPENALPLAYITEM* playitem = soundplayitems.Get(c);
      if(playitem)
        {
          SNDITEM* item = playitem->GetItem();          
          if((item->GetStatus() == SNDITEM_STATUS_PLAY) || (item->GetStatus() == SNDITEM_STATUS_PAUSE))
            {  
              playitem->SetEvent(SNDOPENALPLAYITEM_XFSMSTATE_STOP);              
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_WaitToEnd(SNDITEM* item, int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
* @brief      Sound wait to end
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  maxtimeout : Maxtimeout value.
* @param[in]  waitfunction : Waitfunction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_WaitToEnd(SNDITEM* item, int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
{
  if(!soundactive)
    {
      return false;
    }

  if(!item)
    {
      return false;
    }

  XTIMER* timeout = NULL;
  bool    status  =  true;
  
  timeout = GEN_XFACTORY.CreateTimer();
  if(!timeout)
    {
      return false;
    }

  timeout->Reset();

  while(item->GetStatus() != SNDITEM_STATUS_END)
    {
      if(maxtimeout)
        {
          if(timeout->GetMeasureMilliSeconds() >= maxtimeout)
            {
              status = false;  
              break;
            }
        }

      if(waitfunction)
        {
          if(!waitfunction(item))
            {
              status = false;
              break;
            }
        }
       else
        { 
          GEN_XSLEEP.MilliSeconds(100);
        }
    }

  GEN_XFACTORY.DeleteTimer(timeout);

  if(waitfunction)
    {
      if(!waitfunction(item))
        {
          status = false;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_WaitAllToEnd(int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
* @brief      Sound wait all to end
* @ingroup    SOUND
* 
* @param[in]  maxtimeout : Maxtimeout value.
* @param[in]  waitfunction : Waitfunction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_WaitAllToEnd(int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
{
  if(!soundactive)
    {
      return false;
    }

  XTIMER* timeout = NULL;
  bool    status  =  true;
  
  timeout = GEN_XFACTORY.CreateTimer();
  if(!timeout)
    {
      return false;
    }

  timeout->Reset();

  while(GEN_SNDFACTORY.Sound_IsAnyActive())
    {
      if(maxtimeout)
        {
          if(timeout->GetMeasureMilliSeconds() >= maxtimeout)
            {
              status = false;  
              break;
            }
        }

      if(waitfunction)
        {
          if(!waitfunction(NULL))
            {
              status = false;
              break;
            }
        }
       else
        { 
          GEN_XSLEEP.MilliSeconds(100);
        }
    }

  GEN_XFACTORY.DeleteTimer(timeout);

  if(waitfunction)
    {
      if(!waitfunction(NULL))
        {
          status = false;          
        }
    }

  return status;
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDOPENALFACTORY::Sound_GetVolume(SNDITEM* item)
* @brief      Sound get volume
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDOPENALFACTORY::Sound_GetVolume(SNDITEM* item)
{
  if(!soundactive)
    {
      return false;
    }

  SNDOPENALPLAYITEM* playitem =  GetPlayItemFromItem(item);
  if(playitem)
    {
      SNDOPENALSOURCE* source = playitem->GetSource();
      if(source)
        {
          return source->GetVolume();
        }    
    }

  return SNDFACTORY_UNDEFINED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_SetVolume(SNDITEM* item, int volume)
* @brief      Sound set volume
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  volume : Volume value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_SetVolume(SNDITEM* item, int volume)
{
  if(!soundactive)
    {
      return false;
    }

  SNDOPENALPLAYITEM* playitem =  GetPlayItemFromItem(item);
  if(playitem)
    {
      SNDOPENALSOURCE* source = playitem->GetSource();
      if(source)
        {
          return source->SetVolume(volume);
        }    
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float SNDOPENALFACTORY::Sound_GetPitch(SNDITEM* item)
* @brief      Sound get pitch
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float SNDOPENALFACTORY::Sound_GetPitch(SNDITEM* item)
{
  if(!soundactive)
    {
      return false;
    }

  SNDOPENALPLAYITEM* playitem =  GetPlayItemFromItem(item);
  if(playitem)
    {
      SNDOPENALSOURCE* source = playitem->GetSource();
      if(source)
        {
          return source->GetPitch();
        }    
    }

  return SNDFACTORY_UNDEFINED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::Sound_SetPitch(SNDITEM* item, float pitch)
* @brief      Sound set pitch
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  pitch : Pitch value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::Sound_SetPitch(SNDITEM* item, float pitch)
{
  if(!soundactive)
    {
      return false;
    }

  SNDOPENALPLAYITEM* playitem =  GetPlayItemFromItem(item);
  if(playitem)
    {
      SNDOPENALSOURCE* source = playitem->GetSource();
      if(source)
        {
          source->SetPitch(pitch);
          return true;
        }    
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::DeleteAllItems()
* @brief      Delete all items
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::DeleteAllItems()
{
  if(!soundactive)
    {
      return false;
    }

  Sound_StopAll();

  Sound_WaitAllToEnd(3000);  
  
  SNDFACTORY::DeleteAllItems();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* SNDOPENALFACTORY::GetPlayMutex()
* @brief      Get play mutex
* @ingroup    SOUND
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* SNDOPENALFACTORY::GetPlayMutex()
{
  return playmutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<SNDOPENALPLAYITEM*>* SNDOPENALFACTORY::GetSoundPlayItems()
* @brief      Get sound play items
* @ingroup    SOUND
* 
* @return     XVECTOR<SNDOPENALPLAYITEM*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<SNDOPENALPLAYITEM*>* SNDOPENALFACTORY::GetSoundPlayItems()
{
  return &soundplayitems;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALPLAYITEM* SNDOPENALFACTORY::GetPlayItemFromItem(SNDITEM* item)
* @brief      Get play item from item
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     SNDOPENALPLAYITEM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALPLAYITEM* SNDOPENALFACTORY::GetPlayItemFromItem(SNDITEM* item)
{ 
  if(!soundactive)
    {
      return NULL;
    }

  for(XDWORD c=0; c<soundplayitems.GetSize(); c++)
    {
      SNDOPENALPLAYITEM* playitem = soundplayitems.Get(c);
      if(playitem)
        {
          SNDITEM* _item = playitem->GetItem();          
          if(item == _item)
            {  
              return playitem;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::GenerateBuffer(SNDOPENALPLAYITEM* playitem, SNDNOTE* soundnote)
* @brief      Generate buffer
* @ingroup    SOUND
* 
* @param[in]  playitem : Playitem pointer to use.
* @param[in]  soundnote : Soundnote pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::GenerateBuffer(SNDOPENALPLAYITEM* playitem, SNDNOTE* soundnote)
{
  if(!soundactive)
    {
      return false;
    }

  if(!playitem || !soundnote)
    {
      return false;
    }

  XDWORD           samplerate = 10000;
  XDWORD           size       = 0;
  SNDOPENALSOURCE* source     = playitem->GetSource();

  if(!source)
    {
      return false;
    }  

  source->GetBuffer()->GetXBuffer()->Empty();

  size += source->GetBuffer()->GenerateNote(soundnote->GetFrequency(), soundnote->GetDuration()*2, samplerate);
  size += source->GetBuffer()->GenerateNote(soundnote->GetFrequency(), 2000, samplerate);
                                                            
  source->GetBuffer()->Assign(1, size, samplerate);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALFACTORY::GenerateBuffer(SNDOPENALPLAYITEM* playitem, SNDFILE* soundfile)
* @brief      Generate buffer
* @ingroup    SOUND
* 
* @param[in]  playitem : Playitem pointer to use.
* @param[in]  soundfile : Soundfile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALFACTORY::GenerateBuffer(SNDOPENALPLAYITEM* playitem, SNDFILE* soundfile)
{
  if(!soundactive)
    {
      return false;
    }

  if(!playitem || !soundfile)
    {
      return false;
    }

  XDWORD           size       = 0;
  SNDOPENALSOURCE* source     = playitem->GetSource();

  source->GetBuffer()->GetXBuffer()->Add(soundfile->GetData());

  source->GetBuffer()->Assign(soundfile->GetChannels(), soundfile->GetData()->GetSize(), soundfile->GetSampleRate());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALFACTORY::ThreadPlay(void* param)
* @brief      Thread play
* @ingroup    SOUND
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALFACTORY::ThreadPlay(void* param)
{  
  SNDOPENALFACTORY* sndopenal = (SNDOPENALFACTORY*)param;
  if(!sndopenal) return;  

  if(sndopenal->IsSoundActive())
    {
      sndopenal->Update();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALFACTORY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALFACTORY::Clean()
{        
  device        = NULL;
  context       = NULL;

  mastervolume  = 0.0f;
  maxchannels   = 0;
                                        
  playthread    = NULL;
  playmutex     = NULL;                                                    
}



