/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDPlayCFG.cpp
* 
* @class      SNDPLAYCFG
* @brief      Sound Play Configuration class
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

#include "SNDPlayCFG.h"

#include "SNDFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDPLAYCFG::SNDPLAYCFG()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDPLAYCFG::SNDPLAYCFG()
{
  Clean();

  volume    = SNDFACTORY_UNDEFINED;
  pitch     = SNDFACTORY_UNDEFINED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDPLAYCFG::~SNDPLAYCFG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDPLAYCFG::~SNDPLAYCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SNDPLAYCFG::GetVolume()
* @brief      Get volume
* @ingroup    SOUND
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SNDPLAYCFG::GetVolume()
{
  return volume;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDPLAYCFG::SetVolume(int volume)
* @brief      Set volume
* @ingroup    SOUND
* 
* @param[in]  volume : Volume value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDPLAYCFG::SetVolume(int volume)
{
  this->volume = volume;
}

        
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float SNDPLAYCFG::GetPitch()
* @brief      Get pitch
* @ingroup    SOUND
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float SNDPLAYCFG::GetPitch()
{
  return pitch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDPLAYCFG::SetPitch(float pitch)
* @brief      Set pitch
* @ingroup    SOUND
* 
* @param[in]  pitch : Pitch value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDPLAYCFG::SetPitch(float pitch)
{
  this->pitch = pitch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDPLAYCFG::CopyTo(SNDPLAYCFG& playCFG)
* @brief      Copy to
* @ingroup    SOUND
* 
* @param[in]  playCFG : Play configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDPLAYCFG::CopyTo(SNDPLAYCFG& playCFG)
{
  playCFG.volume  = volume;
  playCFG.pitch   = pitch;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDPLAYCFG::CopyFrom(SNDPLAYCFG& playCFG)
* @brief      Copy from
* @ingroup    SOUND
* 
* @param[in]  playCFG : Play configuration value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDPLAYCFG::CopyFrom(SNDPLAYCFG& playCFG)
{
  volume  = playCFG.volume;
  pitch   = playCFG.pitch;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDPLAYCFG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDPLAYCFG::Clean()
{
  volume    = 0;
  pitch     = 0;
}




