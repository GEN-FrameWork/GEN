/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXGPIOPCParallel.cpp
* 
* @class      DIOLINUXGPIOPCPARALLEL
* @brief      LINUX Data Input/Output GPIO (General Purpose Input/Output) PC Parallel
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



#ifdef HW_INTEL


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXGPIOPCParallel.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/io.h>

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXGPIOPCPARALLEL::DIOLINUXGPIOPCPARALLEL() : DIOGPIO()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXGPIOPCPARALLEL::DIOLINUXGPIOPCPARALLEL() : DIOGPIO()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXGPIOPCPARALLEL::~DIOLINUXGPIOPCPARALLEL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXGPIOPCPARALLEL::~DIOLINUXGPIOPCPARALLEL()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOLINUXGPIOPCPARALLEL::GetParallelPort()
* @brief      Get parallel port
* @ingroup    PLATFORM_LINUX
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOLINUXGPIOPCPARALLEL::GetParallelPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXGPIOPCPARALLEL::SetParallelPort(XWORD port)
* @brief      Set parallel port
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXGPIOPCPARALLEL::SetParallelPort(XWORD port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::Ini()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::SetMode(DIOGPIO_ENTRY* entry, XWORD mode)
* @brief      Set mode
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : Entry pointer to use.
* @param[in]  mode : Mode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::SetMode(DIOGPIO_ENTRY* entry, XWORD mode)
{
  if(!entry) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::GetValue(DIOGPIO_ENTRY* entry)
* @brief      Get value
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : Entry pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::GetValue(DIOGPIO_ENTRY* entry)
{
	if(!entry) return false;

  XBYTE data = 0;

  PC_Get(port, data);

  return (data & ~(0x01 << entry->GetGPIO()))?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::SetValue(DIOGPIO_ENTRY* entry, bool value)
* @brief      Set value
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : Entry pointer to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::SetValue(DIOGPIO_ENTRY* entry, bool value)
{
	if(!entry) return false;

  XBYTE data = 0;

  PC_Get(port, data);
  PC_Set(port, value ? (data | (0x01 << entry->GetGPIO())) : (data & ~(0x01 << entry->GetGPIO())));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::End()
* @brief      End the object
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::End()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::PC_Get(XDWORD nport, XBYTE& data)
* @brief      PC get
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  nport : Nport value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::PC_Get(XDWORD nport, XBYTE& data)
{
  XDWORD val = 0;

  if(ioperm(nport, 1, 1)) return false;

  val = inb(nport);

  if(ioperm(nport, 1, 0)) return false;

  data = (XBYTE)(val & 0x0FF);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIOPCPARALLEL::PC_Set(XDWORD nport, XBYTE data)
* @brief      PC set
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  nport : Nport value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIOPCPARALLEL::PC_Set(XDWORD nport, XBYTE data)
{
  if(ioperm(nport, 1, 1)) return false;

  outb(data, nport);

  if(ioperm(nport, 1, 0)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXGPIOPCPARALLEL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXGPIOPCPARALLEL::Clean()
{
  port            = DIOLINUXGPIOPCPARALLEL_PORT1;
}




#endif


