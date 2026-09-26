/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXGPIORPi.cpp
* 
* @class      DIOLINUXGPIORPI
* @brief      LINUX Data Input/Output GPIO (General Purpose Input/Output) Raspberry Pi class
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



#ifdef HW_RASPBERRYPI


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXGPIORPi.h"

#include "XTrace.h"
#include "XSleep.h"
#include "XFileTXT.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

//static uint32_t* RPI_GPIO_map  = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXGPIORPI::DIOLINUXGPIORPI()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXGPIORPI::DIOLINUXGPIORPI(): DIOLINUXGPIO()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXGPIORPI::~DIOLINUXGPIORPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXGPIORPI::~DIOLINUXGPIORPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::Ini()
{
  XLINUXIDRPI   IDpi;
  RPI_MODEL     model = IDpi.GetModel();


  /*
  switch(model)
    {
      case RPI_MODEL_UNKNOWN        :
      case RPI_MODEL_A              :
      case RPI_MODEL_B              :
      case RPI_MODEL_A_PLUS         :
      case RPI_MODEL_B_PLUS         :
      case RPI_MODEL_COMPUTERMODULE :
      case RPI_MODEL_ZERO           : RPI_map_base   = (RPI_BCM2708_PERI_BASE + 0x200000);
                                      RPI_map_base64 = 0;
                                      break;
      case RPI_MODEL_B_2            :
      case RPI_MODEL_B_3            : 
      case RPI_MODEL_CM3P           :
      case RPI_MODEL_B_3P           : RPI_map_base   = (RPI_BCM2709_PERI_BASE + 0x200000);
                                      RPI_map_base64 = 0;
                                      break;

      case RPI_MODEL_B_4            : RPI_map_base   = (RPI_BCM2711_PERI_BASE + 0x200000);
                                      RPI_map_base64 = 0;
                                      break;
                                                                                      
      case RPI_MODEL_B_5            : RPI_map_base   = 0;
                                      RPI_map_base64 = RPI_BCM2712_PERI_BASE;
                                      break;
    }
  */
  

  // ------------- All Common ---------------------------------------------------------

  if(model == RPI_MODEL_B_5)  
    {
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 1, DIOGPIO_INVALID);     /* 01 - Not available (3v3 Power)   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 2, DIOGPIO_INVALID);     /* 02 - Not available (5v  Power) */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 3, 401);     /* 03 - I2C (SDA)                   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 4, DIOGPIO_INVALID);     /* 04 - Not available (5v  Power) */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 5, 402);     /* 05 - I2C (SDL)                   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 6, DIOGPIO_INVALID);     /* 06 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 7, 403);     /* 07 - GPCLK0                      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 8, 413);     /* 08 - UART TXD                  */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 9, DIOGPIO_INVALID);     /* 09 - Not available (Ground)      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 10, 414);     /* 10 - UART RXD                  */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 11, 416);     /* 11 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 12, 417);     /* 12 - PCM_CLK                   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 13, 426);     /* 13 - PCM_DOUT                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 14, DIOGPIO_INVALID);     /* 14 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 15, 421);     /* 15 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 16, 422);     /* 16 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 17, DIOGPIO_INVALID);     /* 17 - Not available (3v3 Power)   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 18, 423);     /* 18 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 19, 409);     /* 19 - SPI MOSI                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 20, DIOGPIO_INVALID);     /* 20 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 21, 408);     /* 21 - SPI MISO                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 22, 423);     /* 22 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 23, 410);     /* 23 - SPI SCLK                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 24, 407);     /* 24 - SPI CE0                   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 25, DIOGPIO_INVALID);     /* 25 - Not available               */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 26, 406);     /* 26 - SPI CE1                   */
    }
   else
    {      
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 1, DIOGPIO_INVALID);     /* 01 - Not available (3v3 Power)   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 2, DIOGPIO_INVALID);     /* 02 - Not available (5v  Power) */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 3, 2);     /* 03 - I2C (SDA)                   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 4, DIOGPIO_INVALID);     /* 04 - Not available (5v  Power) */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 5, 3);     /* 05 - I2C (SDL)                   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 6, DIOGPIO_INVALID);     /* 06 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 7, 4);     /* 07 - GPCLK0                      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 8, 14);     /* 08 - UART TXD                  */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 9, DIOGPIO_INVALID);     /* 09 - Not available (Ground)      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 10, 15);     /* 10 - UART RXD                  */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 11, 17);     /* 11 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 12, 18);     /* 12 - PCM_CLK                   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 13, 27);     /* 13 - PCM_DOUT                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 14, DIOGPIO_INVALID);     /* 14 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 15, 22);     /* 15 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 16, 23);     /* 16 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 17, DIOGPIO_INVALID);     /* 17 - Not available (3v3 Power)   */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 18, 24);     /* 18 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 19, 10);     /* 19 - SPI MOSI                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 20, DIOGPIO_INVALID);     /* 20 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 21, 9);     /* 21 - SPI MISO                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 22, 25);     /* 22 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 23, 11);     /* 23 - SPI SCLK                    */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 24, 8);     /* 24 - SPI CE0                   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 25, DIOGPIO_INVALID);     /* 25 - Not available               */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 26, 7);     /* 26 - SPI CE1                   */      
    }

  // ------------- Extended PI2 -------------------------------------------------------

  if(model == RPI_MODEL_B_5)  
    {
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 27, DIOGPIO_INVALID);     /* 27 - I2C ID EEProm SC            */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 28, DIOGPIO_INVALID);     /* 28 - I2C ID EEProm SD          */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 29, 404);     /* 29 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 30, DIOGPIO_INVALID);     /* 30 - Not available  (Ground)   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 31, 405);     /* 31 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 32, 411);     /* 32 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 33, 412);     /* 33 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 34, DIOGPIO_INVALID);     /* 34 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 35, 418);     /* 35 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 36, 415);     /* 36 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 37, 425);     /* 37 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 38, 419);     /* 38 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 39, DIOGPIO_INVALID);     /* 39 - Not available (Ground)      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 40, 420);     /* 40 - Generic I/O               */

    }


  if((model == RPI_MODEL_A_PLUS)          ||  (model == RPI_MODEL_B_PLUS) ||
     (model == RPI_MODEL_COMPUTERMODULE)  ||  (model == RPI_MODEL_B_2)    ||
     (model == RPI_MODEL_ZERO)            ||  (model == RPI_MODEL_CM3P)   ||
     (model == RPI_MODEL_B_3)             ||  (model == RPI_MODEL_B_3P)   ||
     (model == RPI_MODEL_B_4)                   
    )
    {
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 27, DIOGPIO_INVALID);     /* 27 - I2C ID EEProm SC            */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 28, DIOGPIO_INVALID);     /* 28 - I2C ID EEProm SD          */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 29, 5);     /* 29 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 30, DIOGPIO_INVALID);     /* 30 - Not available  (Ground)   */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 31, 6);     /* 31 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 32, 12);     /* 32 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 33, 13);     /* 33 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 34, DIOGPIO_INVALID);     /* 34 - Not available (Ground)    */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 35, 19);     /* 35 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 36, 16);     /* 36 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 37, 26);     /* 37 - Generic I/O                 */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 38, 20);     /* 38 - Generic I/O               */
      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 39, DIOGPIO_INVALID);     /* 39 - Not available (Ground)      */      GPIOEntry_Create(DIOGPIO_ID_NOTDEFINED, 40, 21);     /* 40 - Generic I/O               */
    }
    

  //return RPI_Ini();

  return DIOLINUXGPIO::Ini();
}


#if 0
// Legacy memory-mapped RPI GPIO path (kept for reference). Active path uses DIOLINUXGPIO.

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::SetMode(DIOGPIO_ENTRY* entry, XWORD mode)
* @brief      Set Mode
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : 
* @param[in]  mode : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::SetMode(DIOGPIO_ENTRY* entry, XWORD mode)
{
  if(!entry) return false;
  if(entry->GetGPIO() == DIOGPIO_INVALID) return false;

  bool isinput = true;

  if((mode & DIOGPIO_MODE_INPUT)  == DIOGPIO_MODE_INPUT)   isinput = true;
  if((mode & DIOGPIO_MODE_OUTPUT) == DIOGPIO_MODE_OUTPUT)  isinput = false;

  return RPI_GPIOMode(entry->GetGPIO(), isinput);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::GetValue(DIOGPIO_ENTRY* entry)
* @brief      Get Value
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::GetValue(DIOGPIO_ENTRY* entry)
{  
  if(!entry) return false;
  if(entry->GetGPIO() == DIOGPIO_INVALID) return false;
  
  return RPI_GPIORead(entry->GetGPIO());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::SetValue(DIOGPIO_ENTRY* entry, bool value)
* @brief      Set Value
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  entry : 
* @param[in]  value : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::SetValue(DIOGPIO_ENTRY* entry, bool value)
{
  if(!entry) return false;
  if(entry->GetGPIO() == DIOGPIO_INVALID) return false;

  return RPI_GPIOWrite(entry->GetGPIO(), value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::End()
* @brief      End
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::End()
{
  return RPI_End();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_Ini()
* @brief      RPI Ini
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_Ini()
{ 
  uint32_t* map = NULL;
  int       fd;

  initialization = false;
  
  if((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) 
    {
      return false;
    }

  if(model != RPI_MODEL_B_5)
    {    
      map = (uint32_t*)mmap(0, RPI_BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, RPI_map_base) ;      
    }
   else 
    {    
      map = (uint32_t*)mmap(NULL, 64 * 1024* 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, RPI_map_base64);      
    }

  if(map == MAP_FAILED)
    {
      return false;    
    } 

  RPI_GPIO_map  = map;

  initialization = true;
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_End()
* @brief      RPI End
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_End()
{
  if(!initialization) 
    {
      return false;
    }

  //fixme - set all gpios back to input
  //munmap((caddr_t)RPI_GPIO_map, RPI_BLOCK_SIZE);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_IsGPIOValid(XQWORD GPIO)
* @brief      RPI Is GPIOValid
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  GPIO : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_IsGPIOValid(XQWORD GPIO)
{
  if(GPIO<2 || GPIO>27) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_GPIOMode(XQWORD GPIO, bool isinput)
* @brief      RPI GPIOMode
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  GPIO : 
* @param[in]  isinput : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_GPIOMode(XQWORD GPIO, bool isinput)
{
  if(!initialization) 
    {
      return false;
    }

  if(!RPI_IsGPIOValid(GPIO))      
    {
      return false;
    }
  
  if(model != RPI_MODEL_B_5)
    {
      uint8_t gpiotoGPFsel[] = {  0,0,0,0,0,0,0,0,0,0,
                                  1,1,1,1,1,1,1,1,1,1,
                                  2,2,2,2,2,2,2,2,2,2,
                                  3,3,3,3,3,3,3,3,3,3,
                                  4,4,4,4,4,4,4,4,4,4,
                                  5,5,5,5,5,5,5,5,5,5,
                                };

      uint8_t gpiotoshift[]   = { 0,3,6,9,12,15,18,21,24,27,
                                  0,3,6,9,12,15,18,21,24,27,
                                  0,3,6,9,12,15,18,21,24,27,
                                  0,3,6,9,12,15,18,21,24,27,
                                  0,3,6,9,12,15,18,21,24,27,
                                };
      int     gpiopin;
      int     fsel;
      int     shift;
  
      fsel  = gpiotoGPFsel[GPIO];
      shift = gpiotoshift[GPIO];

      if(isinput) 
        {
          *(RPI_GPIO_map + fsel) = (*(RPI_GPIO_map + fsel) & ~(7 << shift)) ;                   // Sets bits to zero = input
        }
       else  
        {
          *(RPI_GPIO_map + fsel) = (*(RPI_GPIO_map + fsel) & ~(7 << shift)) | (1 << shift);
        }  
    }
   else
    {
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("RPI 5 Data Port Mode: GPIO %lld -> %s "), GPIO, isinput?__L("input"):__L("output"));
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_GPIORead(XQWORD GPIO)
* @brief      RPI GPIORead
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  GPIO : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_GPIORead(XQWORD GPIO)
{
  if(!initialization) 
    {
      return false;
    }

  if(!RPI_IsGPIOValid(GPIO))      
    {
      return false;
    }

  if(model != RPI_MODEL_B_5)
    {
      uint8_t gpiotoGPLEV [] = {  13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
                                  14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
                               };

      if((*(RPI_GPIO_map + gpiotoGPLEV[GPIO]) & (1 << (GPIO & 31))) != 0)
        {     
          return true;
        }
    }
   else
    {
      // XTRACE_PRINTCOLOR(1, __L("RPI 5 Data Port Read: GPIO %lld"), GPIO);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXGPIORPI::RPI_GPIOWrite(XQWORD GPIO, bool isactive)
* @brief      RPI GPIOWrite
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  GPIO : 
* @param[in]  isactive : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXGPIORPI::RPI_GPIOWrite(XQWORD GPIO, bool isactive)
{
  if(!initialization) 
    {
      return false;
    }

  if(!RPI_IsGPIOValid(GPIO))      
    {
      return false;
    }

  if(model != RPI_MODEL_B_5)
    {
      uint8_t gpiotoGPSET [] = {  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                               };

      uint8_t gpiotoGPCLR [] = { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
                                 11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
                               };
      if(isactive)   
        {
          *(RPI_GPIO_map + gpiotoGPSET[GPIO]) = 1 << (GPIO & 31);
        }          
       else   
        {
          *(RPI_GPIO_map + gpiotoGPCLR[GPIO]) = 1 << (GPIO & 31);
        }
    }
   else
    {
      uint32_t* PERIbase   = RPI_GPIO_map;
      uint32_t* GPIObase   = PERIbase + 0xD0000 / 4;
      uint32_t* RIObase    = PERIbase + 0XE0000 / 4;
      uint32_t* PADbase    = PERIbase + 0XF0000 / 4;
      uint32_t* pad        = PADbase  + 1;   
    
      uint32_t pin         = GPIO;
      uint32_t fn          = 5;
     
      RPI5_GPIO[pin].ctrl  = fn;
      pad[pin]             = 0x10;
      RPI5_RIOSET->oe      = 0x01<<pin;
      RPI5_RIOSET->out     = 0x01<<pin;

      RPI5_RIOXOR->out     = 0x04;

      XTRACE_PRINTCOLOR(1, __L("RPI 5 Data Port Write: GPIO %lld ->%s"), GPIO, isactive?__L("on"):__L("off"));
    }

  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXGPIORPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXGPIORPI::Clean()
{
  model               = RPI_MODEL_UNKNOWN;
  
  /*
  RPI_map_base        = 0;
  RPI_map_base64      = 0;

  initialization      = false;
  */
}




#endif

