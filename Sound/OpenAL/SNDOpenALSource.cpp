/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDOpenALSource.cpp
* 
* @class      SNDOPENALSOURCE
* @brief      Sound OpenAL Source class
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

#include "SNDOpenALSource.h"

#include "XTrace.h"

#include "SNDFactory_XEvent.h"
#include "SNDOpenALFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALSOURCE::SNDOPENALSOURCE()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALSOURCE::SNDOPENALSOURCE()
{
  Clean();

  alGenSources(1, &source);
  alSourcef(source  , AL_PITCH, 1);
  alSourcef(source  , AL_GAIN, 1);
  alSource3f(source , AL_POSITION, 0, 0, 0);
  alSource3f(source , AL_VELOCITY, 0, 0, 0);
  alSourcei(source  , AL_LOOPING, AL_FALSE);
//alSourcef(source  , AL_MAX_GAIN, 1000.0f); 

  albuffer.Create();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALSOURCE::~SNDOPENALSOURCE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALSOURCE::~SNDOPENALSOURCE()
{
  alDeleteSources(1, &source); 

  albuffer.Delete();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDOPENALBUFFER* SNDOPENALSOURCE::GetBuffer()
* @brief      Get buffer
* @ingroup    SOUND
* 
* @return     SNDOPENALBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDOPENALBUFFER* SNDOPENALSOURCE::GetBuffer()
{
  return &albuffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Stop()
* @brief      Stop
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Stop()
{
  alSourceStopv(1, &source);
  isplaying = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Pause()
* @brief      Pause
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Pause()
{
  alSourcePause(source);
  isplaying = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::UnPause()
* @brief      Un pause
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::UnPause()
{
  alSourcePlay(source);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALSOURCE::IsPLaying()
* @brief      Is P laying
* @ingroup    SOUND
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsPLaying()
{
  if(isplaying)
    {
      ALint source_state;

      alGetSourcei(source, AL_SOURCE_STATE, &source_state);

      bool _isplaying = isplaying;

      isplaying = ((source_state == AL_PLAYING) || (source_state == AL_PAUSED));

      return isplaying;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALSOURCE::IsStopped()
* @brief      Is stopped
* @ingroup    SOUND
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsStopped()
{
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);

  return (source_state == AL_STOPPED) || (source_state == AL_INITIAL);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALSOURCE::IsPaused()
* @brief      Is paused
* @ingroup    SOUND
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsPaused()
{
  ALint source_state;

  alGetSourcei(source, AL_SOURCE_STATE, &source_state);

  return source_state == AL_PAUSED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::SetInLoop(bool inloop)
* @brief      Set in loop
* @ingroup    SOUND
* 
* @param[in]  inloop : Inloop value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::SetInLoop(bool inloop)
{
  alSourcei(source, AL_LOOPING, inloop);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDOPENALSOURCE::GetVolume()
* @brief      Get volume
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDOPENALSOURCE::GetVolume()
{  
  float fvolume = 0.0f;
  int   rvolume = 0;
  int   volume  = 0;

  alGetSourcef(source, AL_GAIN, &fvolume);

  volume = (int)(fvolume * 100);

  rvolume = (volume * GEN_SNDFACTORY.MasterVolume_Get())/100;

  return rvolume;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALSOURCE::SetVolume(int volume)
* @brief      Set volume
* @ingroup    SOUND
* 
* @param[in]  volume : Volume value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALSOURCE::SetVolume(int volume)
{  
  int   rvolume = (volume * GEN_SNDFACTORY.MasterVolume_Get())/100;
  float fvolume = (rvolume /100.0f);

  alSourcef(source, AL_GAIN, fvolume); 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float SNDOPENALSOURCE::GetPitch()
* @brief      Get pitch
* @ingroup    SOUND
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float SNDOPENALSOURCE::GetPitch()
{
  float pitch;

  alGetSourcef(source, AL_PITCH, &pitch);

  return pitch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::SetPitch(float pitch)
* @brief      Set pitch
* @ingroup    SOUND
* 
* @param[in]  pitch : Pitch value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::SetPitch(float pitch)
{
  alSourcef(source, AL_PITCH, pitch);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Play()
* @brief      Play
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Play()
{
  if(!IsPLaying())
    {     
      alSourcei(source, AL_BUFFER, albuffer.GetHandle());
      alSourcePlay(source);

      isplaying = true;      
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::ResetPlay()
* @brief      Reset play
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::ResetPlay()
{
  alSourcef(source, AL_SEC_OFFSET, 0.0f);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::SetSecondsOffset(float seconds)
* @brief      Set seconds offset
* @ingroup    SOUND
* 
* @param[in]  seconds : Seconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::SetSecondsOffset(float seconds)
{
  /*
  if(element)
    {
      if(seconds <= element->GetDuration())
        {
          alSourcef(source, AL_SEC_OFFSET, seconds);
        }
    }
  */
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::SetSamplesOffset(int samples)
* @brief      Set samples offset
* @ingroup    SOUND
* 
* @param[in]  samples : Samples value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::SetSamplesOffset(int samples)
{
  /*
  if(element)
    {
      if(samples > element->GetSamples())
        {
          samples = element->GetSamples();
        }

      alSourcei(source, AL_SAMPLE_OFFSET, samples);
    }
  */
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::SetAquired(bool aquired)
* @brief      Set aquired
* @ingroup    SOUND
* 
* @param[in]  aquired : Aquired value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::SetAquired(bool aquired)
{ 
  this->aquired = aquired;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDOPENALSOURCE::IsAquired()
* @brief      Is aquired
* @ingroup    SOUND
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsAquired()
{ 
  return this->aquired;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Aquire()
* @brief      Aquire
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Aquire()
{ 
  this->aquired = true;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Release()
* @brief      Release
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Release()
{ 
  this->aquired = false;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Queue()
* @brief      Queue
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Queue()
{
  ALuint handle = albuffer.GetHandle();
  alSourceQueueBuffers(source, 1, &handle);             

  albuffer.SetHandle(handle);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::UnQueue()
* @brief      Un queue
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::UnQueue()
{
  alSourceStop(source);

  ALuint ID = 0;
  alSourceUnqueueBuffers(source, 1, &ID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDOPENALSOURCE::GetQueueLength()
* @brief      Get queue length
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDOPENALSOURCE::GetQueueLength()
{
  int queue = 0;
  alGetSourcei(source, AL_BUFFERS_QUEUED, &queue);
  return queue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDOPENALSOURCE::GetProcessedBuffers()
* @brief      Get processed buffers
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDOPENALSOURCE::GetProcessedBuffers()
{
  int processed = 0;
  alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
  return processed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDOPENALSOURCE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDOPENALSOURCE::Clean()
{
  source        = 0;
  
  isplaying     = false;
  aquired       = false;
}



