/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFactory.cpp
* 
* @class      SNDFACTORY
* @brief      Sound Factory class
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

#include "SNDFactory.h"

#include "XSystem.h"

#include "SNDFactory_XEvent.h"
#include "SNDItem.h"
#include "SNDFile.h"
#include "SNDNote.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

SNDFACTORY* SNDFACTORY::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFACTORY::SNDFACTORY()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFACTORY::SNDFACTORY()
{
  Clean();

  RegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_INI);
  RegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_PLAY);
  RegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_PAUSE);
  RegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_STOP);
  RegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_END);

  sounditems.SetIsMulti(false);

  mastervolume = 100;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFACTORY::~SNDFACTORY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFACTORY::~SNDFACTORY()
{
  End(); 

  DeRegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_INI);
  DeRegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_PLAY);
  DeRegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_PAUSE);
  DeRegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_STOP);
  DeRegisterEvent(SNDFACTORY_XEVENT_TYPE_SOUND_END);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::GetIsInstanced()
{
  return (instance!=NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFACTORY& SNDFACTORY::GetInstance()
* @brief      Get instance
* @ingroup    SOUND
* 
* @return     SNDFACTORY& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFACTORY& SNDFACTORY::GetInstance()
{
  if(!instance) instance = GEN_NEW SNDFACTORY();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::SetInstance(SNDFACTORY* _instance)
* @brief      Set instance
* @ingroup    SOUND
* 
* @param[in]  _instance : Instance pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::SetInstance(SNDFACTORY* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::DelInstance()
* @brief      Del instance
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::DelInstance()
{
  if(!instance) return false;

  GEN_DELETE instance;
  instance = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Ini()
* @brief      Initialize the object
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Ini()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Update()
* @brief      Update
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Update()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::End()
* @brief      End the object
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::End()
{
  DeleteAllItems();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::IsSoundActive()
* @brief      Is sound active
* @ingroup    SOUND
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::IsSoundActive()
{
  return soundactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_Play(SNDITEM* item, SNDPLAYCFG* playCFG, int ntimestoplay)
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
bool SNDFACTORY::Sound_Play(SNDITEM* item, SNDPLAYCFG* playCFG, int ntimestoplay)                                   
{ 
  if(!soundactive)
    {
      return false;
    }

  if(!item)
    {
      return false;
    }

  if(playCFG)
    {
      item->SetPlayCFG((*playCFG));
    }

  item->SetNTimesToPlay(ntimestoplay);

  if(item->GetStatus() != SNDITEM_STATUS_PAUSE)   
    {
      item->SetPlayingTime(0);
      item->SetCurrentPlayingTime(0);
    }

  item->SetStatus(SNDITEM_STATUS_INI);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_Pause(SNDITEM* item)
* @brief      Sound pause
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_Pause(SNDITEM* item)                                   
{ 
  return false;                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_Stop(SNDITEM* item)
* @brief      Sound stop
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_Stop(SNDITEM* item)                                   
{                                     
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_StopAll()
* @brief      Sound stop all
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_StopAll()                                                      
{ 
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_WaitToEnd(SNDITEM* item, int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
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
bool SNDFACTORY::Sound_WaitToEnd(SNDITEM* item, int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_WaitAllToEnd(int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
* @brief      Sound wait all to end
* @ingroup    SOUND
* 
* @param[in]  maxtimeout : Maxtimeout value.
* @param[in]  waitfunction : Waitfunction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_WaitAllToEnd(int maxtimeout, SNDFACTORY_WAITFUNCTION waitfunction)
{
  return false;
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDFACTORY::Sound_GetVolume(SNDITEM* item)
* @brief      Sound get volume
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDFACTORY::Sound_GetVolume(SNDITEM* item)
{
  return SNDFACTORY_UNDEFINED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_SetVolume(SNDITEM* item, int volume)
* @brief      Sound set volume
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  volume : Volume value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_SetVolume(SNDITEM* item, int volume)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float SNDFACTORY::Sound_GetPitch(SNDITEM* item)
* @brief      Sound get pitch
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float SNDFACTORY::Sound_GetPitch(SNDITEM* item)
{
  return SNDFACTORY_UNDEFINED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_SetPitch(SNDITEM* item, float pitch)
* @brief      Sound set pitch
* @ingroup    SOUND
* 
* @param[in]  item : Item pointer to use.
* @param[in]  pitch : Pitch value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_SetPitch(SNDITEM* item, float pitch)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::DeleteAllItems()
* @brief      Delete all items
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::DeleteAllItems()
{
  if(sounditems.IsEmpty())
    {
      return false; 
    }

  sounditems.DeleteContents();
  sounditems.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDFACTORY::MasterVolume_Get()
* @brief      Master volume get
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDFACTORY::MasterVolume_Get()
{ 
  return mastervolume;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::MasterVolume_Set(int volume)
* @brief      Master volume set
* @ingroup    SOUND
* 
* @param[in]  volume : Volume value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::MasterVolume_Set(int volume)
{   
  this->mastervolume = volume;
                                
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM* SNDFACTORY::CreateItem(XCHAR* path)
* @brief      Create item
* @ingroup    SOUND
* 
* @param[in]  path : Path to use.
* 
* @return     SNDITEM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM* SNDFACTORY::CreateItem(XCHAR* path)  
{
  if(!soundactive)
    {
      return NULL;
    }

  if(!path)
    {
      return NULL;
    }

  SNDITEM* item = GEN_NEW SNDITEM();
  if(!item)
    {
      return NULL;
    }

  item->SetType(SNDITEM_TYPE_FILE);

  XPATH     xpath;
  XSTRING   ID;

  xpath = path;

  xpath.GetNamefileExt(ID);

  item->GetID()->Set(ID);
  if(item->GetID()->IsEmpty())
    {
      return NULL;
    }

  SNDFILE* soundfile = SNDFILE::Create(path);
  if(!soundfile)
    {
      GEN_DELETE item;
      return NULL;
    }

  item->SetStatus(SNDITEM_STATUS_NONE);
  item->SetSoundFile(soundfile);

  sounditems.Add(item);
  
  return item;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM* SNDFACTORY::CreateItem(XPATH& xpath)
* @brief      Create item
* @ingroup    SOUND
* 
* @param[in]  xpath : Path to use.
* 
* @return     SNDITEM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM* SNDFACTORY::CreateItem(XPATH& xpath)
{
  return CreateItem(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDITEM* SNDFACTORY::CreateItem(XDWORD frecuency, XDWORD duration)
* @brief      Create item
* @ingroup    SOUND
* 
* @param[in]  frecuency : Frecuency value.
* @param[in]  duration : Duration value.
* 
* @return     SNDITEM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDITEM* SNDFACTORY::CreateItem(XDWORD frecuency, XDWORD duration)
{
  if(!soundactive)
    {
      return NULL;
    }

  if((!frecuency) || (!duration))
    {
      return NULL;
    }

  SNDITEM* item = GEN_NEW SNDITEM();
  if(!item)
    {
      return NULL;
    }

  item->SetType(SNDITEM_TYPE_NOTE);

  item->GetID()->Format(_L("frec: %d (Hz) - time: %d (msec)"), frecuency, duration);

  SNDNOTE* note = GEN_NEW SNDNOTE();
  if(!note)
    {
      GEN_DELETE item;
      return NULL;
    }

  note->SetFrequency(frecuency);
  note->SetDuration(duration);

  item->SetStatus(SNDITEM_STATUS_NONE);
  item->SetSoundNote(note);

  sounditems.Add(item);
  
  return item;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<SNDITEM*>* SNDFACTORY::GetItems()
* @brief      Get items
* @ingroup    SOUND
* 
* @return     XVECTOR<SNDITEM*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<SNDITEM*>* SNDFACTORY::GetItems()
{
  return &sounditems;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_IsAnyActive()
* @brief      Sound is any active
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_IsAnyActive()
{ 
  if(!soundactive)
    {
      return false;
    }

  XDWORD counter = 0;

  for(XDWORD c=0; c<sounditems.GetSize(); c++)
    {
      SNDITEM* item = sounditems.Get(c);
      if(item)
        {
          if((item->GetStatus() != SNDITEM_STATUS_END) && 
             (item->GetStatus() != SNDITEM_STATUS_NONE))
            {
              counter++;
            }
        }
    }

  return counter?true:false;                     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFACTORY::Sound_IsAnyPlaying()
* @brief      Sound is any playing
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFACTORY::Sound_IsAnyPlaying()
{ 
  if(!soundactive)
    {
      return false;
    }

  for(XDWORD c=0; c<sounditems.GetSize(); c++)
    {
      SNDITEM* item = sounditems.Get(c);
      if(item)
        {
          if(item->GetStatus() == SNDITEM_STATUS_PLAY)       
            {
              return true;
            }
        }
    }

  return false;                     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDFACTORY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDFACTORY::Clean()
{
  instance     = NULL;

  soundactive  = false;
  mastervolume = 0;
}



