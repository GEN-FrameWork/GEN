/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODisplayDevice.cpp
* 
* @class      DIODISPLAYDEVICE
* @brief      Data Input/Output Display Device (generic display device class)
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

#include "DIODisplayDevice.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODISPLAYDEVICE_CONFIG::DIODISPLAYDEVICE_CONFIG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODISPLAYDEVICE_CONFIG::DIODISPLAYDEVICE_CONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODISPLAYDEVICE_CONFIG::~DIODISPLAYDEVICE_CONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODISPLAYDEVICE_CONFIG::~DIODISPLAYDEVICE_CONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIODISPLAYDEVICE_CONFIG::GetWidth()
* @brief      Get width
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIODISPLAYDEVICE_CONFIG::GetWidth()
{
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIODISPLAYDEVICE_CONFIG::GetHeight()
* @brief      Get height
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIODISPLAYDEVICE_CONFIG::GetHeight()
{
  return height;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIODISPLAYDEVICE_CONFIG::GetNPixels()
* @brief      Get N pixels
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIODISPLAYDEVICE_CONFIG::GetNPixels()
{
  return (width*height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIODISPLAYDEVICE_CONFIG::GetSizeBuffer()
* @brief      Get size buffer
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIODISPLAYDEVICE_CONFIG::GetSizeBuffer()
{
  return sizebuffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODISPLAYDEVICE_CONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODISPLAYDEVICE_CONFIG::Clean()
{
  width                   = 0;
  height                  = 0;
  sizebuffer              = 0;

  isdirectconnection      = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODISPLAYDEVICE::DIODISPLAYDEVICE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODISPLAYDEVICE::DIODISPLAYDEVICE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODISPLAYDEVICE::~DIODISPLAYDEVICE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODISPLAYDEVICE::~DIODISPLAYDEVICE()
{  
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIODISPLAYDEVICE::GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID index)
* @brief      Get GPIO entry ID
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIODISPLAYDEVICE::GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID index)
{
  return GPIOentrysID[index];        
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODISPLAYDEVICE::SetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID index, XDWORD GPIOentryID)
* @brief      Set GPIO entry ID
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* @param[in]  GPIOentryID : GPI Oentry ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODISPLAYDEVICE::SetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID index, XDWORD GPIOentryID)
{
  GPIOentrysID[index] = GPIOentryID;   

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODISPLAYDEVICE::SetActiveBlackLight(bool active)
* @brief      Set active black light
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODISPLAYDEVICE::SetActiveBlackLight(bool active)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODISPLAYDEVICE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODISPLAYDEVICE::Clean()
{
  for(int c=0; c<DIODISPLAYDEVICE_INDEX_GPIOENTRYID_MAX; c++)
    {
      GPIOentrysID[c] =  DIOGPIO_ID_NOTDEFINED;     
    }  
}






