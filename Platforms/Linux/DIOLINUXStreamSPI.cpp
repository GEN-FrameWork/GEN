/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamSPI.cpp
* 
* @class      DIOLINUXSTREAMSPI
* @brief      LINUX Data Input/Output Stream SPI class
* @ingroup    PLATFORM_LINUX
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMSPI_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamSPI.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "XBuffer.h"
#include "XTrace.h"
#include "DIOStreamSPIConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMSPI::DIOLINUXSTREAMSPI() : DIOSTREAMSPI()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMSPI::DIOLINUXSTREAMSPI() : DIOSTREAMSPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMSPI::~DIOLINUXSTREAMSPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMSPI::~DIOLINUXSTREAMSPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMSPI::Open()
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::Open()
{
  if(!config) return false;

  XBUFFER charstr;
  
  (*config->GetLocalDeviceName()).ConvertToASCII(charstr);
  handle = open(charstr.GetPtrChar(), O_RDWR);  
  //XTRACE_PRINTCOLOR(((handle<0)?4:1), _L("SPI Open %s : %s"), config->GetLocalDeviceName()->Get(), ((handle<0)?_L("Error"):_L("Ok")));
  if(handle<0) return false;

  //  Set SPI parameters.
  //  Why are we reading it afterwriting it? I've no idea, but for now I'm blindly
  //  copying example code I've seen online...

  XBYTE   _mode  = config->GetSPIMode();
  XBYTE   _bits  = config->GetNBitsWord();
  XDWORD  _speed = config->GetSpeed();

  if (ioctl(handle, SPI_IOC_WR_MODE           , &_mode)   < 0) return false;
  if (ioctl(handle, SPI_IOC_WR_BITS_PER_WORD  , &_bits)   < 0) return false;
  if (ioctl(handle, SPI_IOC_WR_MAX_SPEED_HZ   , &_speed)  < 0) return false;

  if (ioctl(handle, SPI_IOC_RD_MODE           , &_mode)   < 0) return false;
  if (ioctl(handle, SPI_IOC_RD_BITS_PER_WORD  , &_bits)   < 0) return false;
  if (ioctl(handle, SPI_IOC_RD_MAX_SPEED_HZ   , &_speed)  < 0) return false;

  return DIOSTREAMSPI::Open();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMSPI::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::Close()
{
  bool status = DIOSTREAMSPI::Close();

  if(handle>=0)
    {
      close(handle);
      handle  = -1;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
* @brief      Transfer buffer
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  bufferread : Bufferread pointer to use.
* @param[in]  bufferwrite : Bufferwrite pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
{
  struct spi_ioc_transfer transf;

  XDWORD sizeint = size;

  memset((XBYTE*)&transf, 0, sizeof(struct spi_ioc_transfer));

  //XTRACE_PRINTCOLOR(1, _L("real SPI %d"), size);

  //wprintf((wchar_t *)_L("\n Write %4d bytes ->"),size);
  //fflush(stdout);

  transf.tx_buf         = (unsigned long)bufferwrite;
  transf.rx_buf         = (unsigned long)bufferread;
  transf.len            = sizeint;
  transf.speed_hz       = config->GetSpeed();
  transf.delay_usecs    = config->GetDelay();
  transf.bits_per_word  = config->GetNBitsWord();
  //transf.cs_change      = true;

  int err = ioctl(handle, SPI_IOC_MESSAGE(1), &transf);

  //wprintf((wchar_t *)_L("%4d %d\n"), transf.len, err);
  //fflush(stdout);

  if(err<0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMSPI::Sleep(int count)
* @brief      Sleep
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  count : Count value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMSPI::Sleep(int count)
{
  for(int i=0; i<count; i++)
    {
      asm("nop");
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMSPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMSPI::Clean()
{
  handle = -1;
}




#endif

