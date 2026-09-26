/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXGPIORPi.h
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

#pragma once

#ifdef HW_RASPBERRYPI

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
/*
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

#include "DIOGPIO.h"
*/

#include "XLINUXIDRPi.h"

#include "DIOLINUXGPIO.h"
#include "DIOLINUXGPIORPi.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*

#define RPI_BCM2708_PERI_BASE           0x20000000      // Rapsberry Pi A+, B+
#define RPI_BCM2709_PERI_BASE           0x3F000000      // Rapsberry Pi 2 y 3
#define	RPI_BCM2711_PERI_BASE           0xFE000000      // Rapsberry Pi 4 
#define	RPI_BCM2712_PERI_BASE           0x1F00000000    // Rapsberry Pi 5                                         

#define RPI_PAGE_SIZE                   (4*1024)
#define RPI_BLOCK_SIZE                  (4*1024)


typedef struct
{
  uint32_t  status;
  uint32_t  ctrl; 

} RPI5_GPIOREGS;


typedef struct
{
  uint32_t  out;
  uint32_t  oe;
  uint32_t  in;
  uint32_t  insync;

} RPI5_RIOREGS;

#define RPI5_GPIO     ((RPI5_GPIOREGS*)GPIObase)

#define RPI5_RIO      ((RPI5_RIOREGS*)(RIObase))
#define RPI5_RIOXOR   ((RPI5_RIOREGS*)(RIObase + 0x1000 / 4))
#define RPI5_RIOSET   ((RPI5_RIOREGS*)(RIObase + 0x2000 / 4))
#define RPI5_RIOCLR   ((RPI5_RIOREGS*)(RIObase + 0x3000 / 4))
*/



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

//class DIOLINUXGPIORPI : public DIOGPIO
class DIOLINUXGPIORPI : public DIOLINUXGPIO
{
  public:
                                        DIOLINUXGPIORPI             ();
    virtual                            ~DIOLINUXGPIORPI             ();

      
    bool                                Ini                         ();
    
         

  private:

    

    void                                Clean                       ();
    
    RPI_MODEL                           model;

    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



