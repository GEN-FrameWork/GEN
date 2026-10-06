/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamSPIConfig.cpp
* 
* @class      DIOSTREAMSPICONFIG
* @brief      Data Input/Output Stream SPI Config class
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

#include "DIOStreamSPIConfig.h"

#include "XFactory.h"
#include "XSystem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPICONFIG::DIOSTREAMSPICONFIG() : DIOSTREAMCONFIG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPICONFIG::DIOSTREAMSPICONFIG() : DIOSTREAMCONFIG()
{
  Clean();
  type  = DIOSTREAMTYPE_SPI;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPICONFIG::~DIOSTREAMSPICONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPICONFIG::~DIOSTREAMSPICONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMSPICONFIG::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMSPICONFIG::GetPort()
{ 
  return port;                                
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetPort(int port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetPort(int port)
{ 
  this->port = port;                          
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMSPICONFIG::GetLocalDeviceName()
* @brief      Get local device name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMSPICONFIG::GetLocalDeviceName()
{ 
  return &localdevicename;                    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPICONFIG::SetLocalDeviceNameByPort()
* @brief      Set local device name by port
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPICONFIG::SetLocalDeviceNameByPort()
{
  int revision = 0;

  switch(GEN_XSYSTEM.GetTypeHardware(&revision))
    {
      case XSYSTEM_HARDWARETYPE_PC          :
                                              #ifdef LINUX
                                              localdevicename.Format(_L("/dev/spi%d"), port-1);
                                              #endif

                                              #ifdef WINDOWS
                                              localdevicename.Format(_L("spi%d"), port-1);
                                              #endif
                                              break;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI : localdevicename.Format(_L("/dev/spidev%d.%d"), port-1, chipselect);
                                              break;

                                   default  : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMSPICONFIG::GetChipSelect()
* @brief      Get chip select
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMSPICONFIG::GetChipSelect()
{ 
  return chipselect;                          
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetChipSelect(int chipselect)
* @brief      Set chip select
* @ingroup    DATAIO
* 
* @param[in]  chipselect : Chipselect value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetChipSelect(int chipselect)                  
{ 
  this->chipselect = chipselect;              
}    


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPICONFIG::IsDirectAccess()
* @brief      Is direct access
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPICONFIG::IsDirectAccess()
{ 
  return isdirectaccess;                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSTREAMSPICONFIG::GetSPIMode()
* @brief      Get SPI mode
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSTREAMSPICONFIG::GetSPIMode()
{ 
  return SPImode;                             
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOSTREAMSPICONFIG::GetNBitsWord()
* @brief      Get N bits word
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSTREAMSPICONFIG::GetNBitsWord()
{ 
  return nbitsword;                           
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMSPICONFIG::GetSpeed()
* @brief      Get speed
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMSPICONFIG::GetSpeed()
{ 
  return speed;                               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMSPICONFIG::GetDelay()
* @brief      Get delay
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMSPICONFIG::GetDelay()
{ 
  return delay;                               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPICONFIG::IsOnlyWrite()
* @brief      Is only write
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPICONFIG::IsOnlyWrite()
{
  return isonlywrite;                         
}

  
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD* DIOSTREAMSPICONFIG::GetPinsDirectAccess()
* @brief      Get pins direct access
* @ingroup    DATAIO
* 
* @return     XDWORD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD* DIOSTREAMSPICONFIG::GetPinsDirectAccess()
{ 
  return GPIO_ID;                             
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPICONFIG::SetPinsDirectAccess(XDWORD GPIOID_CLK, XDWORD GPIOID_MISO, XDWORD GPIOID_MOSI, XDWORD GPIOID_CS)
* @brief      Set pins direct access
* @ingroup    DATAIO
* 
* @param[in]  GPIOID_CLK : GPIOID CLK value.
* @param[in]  GPIOID_MISO : GPIOID MISO value.
* @param[in]  GPIOID_MOSI : GPIOID MOSI value.
* @param[in]  GPIOID_CS : GPIOID CS value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPICONFIG::SetPinsDirectAccess(XDWORD GPIOID_CLK, XDWORD GPIOID_MISO, XDWORD GPIOID_MOSI, XDWORD GPIOID_CS)
{                                 
  GPIO_ID[DIOSTREAMSPI_GPIO_CLK]    = GPIOID_CLK;
  GPIO_ID[DIOSTREAMSPI_GPIO_MISO]   = GPIOID_MISO;
  GPIO_ID[DIOSTREAMSPI_GPIO_MOSI]   = GPIOID_MOSI;
  GPIO_ID[DIOSTREAMSPI_GPIO_CS]     = GPIOID_CS;
                                   
  isdirectaccess  = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetSPIMode(XBYTE SPImode)
* @brief      Set SPI mode
* @ingroup    DATAIO
* 
* @param[in]  SPImode : SP Imode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetSPIMode(XBYTE SPImode)
{ 
  this->SPImode = SPImode;                     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetNBitsWord(XBYTE nbitsword)
* @brief      Set N bits word
* @ingroup    DATAIO
* 
* @param[in]  nbitsword : Nbitsword value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetNBitsWord(XBYTE nbitsword)
{ 
  this->nbitsword = nbitsword;                 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetSpeed(XDWORD speed)
* @brief      Set speed
* @ingroup    DATAIO
* 
* @param[in]  speed : Speed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetSpeed(XDWORD speed)
{ 
  this->speed = speed;                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetDelay(XWORD delay)
* @brief      Set delay
* @ingroup    DATAIO
* 
* @param[in]  delay : Delay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetDelay(XWORD delay)
{ 
  this->delay = delay;                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::SetIsOnlyWrite(bool isonlywrite)
* @brief      Set is only write
* @ingroup    DATAIO
* 
* @param[in]  isonlywrite : Isonlywrite value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::SetIsOnlyWrite(bool isonlywrite)
{ 
  this->isonlywrite = isonlywrite;             
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPICONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPICONFIG::Clean()
{
  type              = DIOSTREAMTYPE_SPI;

  port              = 0;
  localdevicename.Empty();
  chipselect        = 0;

  isdirectaccess    = false;

  for(int c=0; c<DIOSTREAMSPI_GPIO_MAX; c++)
    {                                       
      GPIO_ID[c]    = DIOGPIO_ID_NOTDEFINED;
    }

  SPImode           = 0;
  nbitsword         = 0;
  speed             = 0;
  delay             = 0;
  isonlywrite       = false;
}



