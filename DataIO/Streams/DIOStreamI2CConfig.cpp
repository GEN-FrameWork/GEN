/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamI2CConfig.cpp
* 
* @class      DIOSTREAMI2CCONFIG
* @brief      Data Input/Output Stream I2C Config class
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

#include "DIOStreamI2CConfig.h"

#include "XFactory.h"
#include "XSystem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMI2CCONFIG::DIOSTREAMI2CCONFIG() : DIOSTREAMCONFIG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMI2CCONFIG::DIOSTREAMI2CCONFIG() : DIOSTREAMCONFIG()
{
  Clean();

  type  = DIOSTREAMTYPE_I2C;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMI2CCONFIG::~DIOSTREAMI2CCONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMI2CCONFIG::~DIOSTREAMI2CCONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMI2CCONFIG::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMI2CCONFIG::GetPort()                               
{ 
  return port;                               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::SetPort(int port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::SetPort(int port)                       
{ 
  this->port = port;                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMI2CCONFIG::GetLocalDeviceName()
* @brief      Get local device name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMI2CCONFIG::GetLocalDeviceName()                               
{ 
  return &localdevicename;                   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMI2CCONFIG::SetLocalDeviceNameByPort()
* @brief      Set local device name by port
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMI2CCONFIG::SetLocalDeviceNameByPort()
{
  int revision = 0;

  switch(GEN_XSYSTEM.GetTypeHardware(&revision))
    {
      case XSYSTEM_HARDWARETYPE_PC          : localdevicename.Format(_L("/dev/i2c-%d"), port);
                                              break;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI : if(revision == 1)
                                                    localdevicename = _L("/dev/i2c-0");
                                               else localdevicename = _L("/dev/i2c-1");
                                              break;

                                   default  : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMI2CCONFIG::GetLocalAddress()
* @brief      Get local address
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMI2CCONFIG::GetLocalAddress()
{ 
  return localaddress;                       
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::SetLocalAddress(int localaddress)
* @brief      Set local address
* @ingroup    DATAIO
* 
* @param[in]  localaddress : Localaddress value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::SetLocalAddress(int localaddress)
{ 
  this->localaddress = localaddress;         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMI2CCONFIG::GetRemoteAddress()
* @brief      Get remote address
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMI2CCONFIG::GetRemoteAddress()
{ 
  return remoteaddress;                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::SetRemoteAddress(int remoteaddress)
* @brief      Set remote address
* @ingroup    DATAIO
* 
* @param[in]  remoteaddress : Remoteaddress value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::SetRemoteAddress(int remoteaddress)
{ 
  this->remoteaddress = remoteaddress;       
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMI2CCONFIG_ADDRESSSIZE DIOSTREAMI2CCONFIG::GetAddressSize()
* @brief      Get address size
* @ingroup    DATAIO
* 
* @return     DIOSTREAMI2CCONFIG_ADDRESSSIZE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMI2CCONFIG_ADDRESSSIZE DIOSTREAMI2CCONFIG::GetAddressSize()
{ 
  return addresssize;                        
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::SetAddressSize(DIOSTREAMI2CCONFIG_ADDRESSSIZE addresssize)
* @brief      Set address size
* @ingroup    DATAIO
* 
* @param[in]  addresssize : Addresssize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::SetAddressSize(DIOSTREAMI2CCONFIG_ADDRESSSIZE addresssize)
{
  this->addresssize = addresssize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMI2CCONFIG::IsStretchActive()
* @brief      Is stretch active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMI2CCONFIG::IsStretchActive()
{ 
  return isstretchactive;                    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::SetIsStretchActive(bool isstretchactive)
* @brief      Set is stretch active
* @ingroup    DATAIO
* 
* @param[in]  isstretchactive : Isstretchactive value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::SetIsStretchActive(bool isstretchactive)
{ 
  this->isstretchactive = isstretchactive;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMI2CCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMI2CCONFIG::Clean()
{
  type            = DIOSTREAMTYPE_I2C;

  port            = 0;
  localaddress    = DIOSTREAMI2CCONFIG_ADDRESSINVALID;
  remoteaddress   = DIOSTREAMI2CCONFIG_ADDRESSINVALID;
  addresssize     = DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT;

  isstretchactive = false;
}



