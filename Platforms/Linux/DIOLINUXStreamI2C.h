/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamI2C.h
* 
* @class      DIOLINUXSTREAMI2C
* @brief      LINUX Data Input/Output Stream I2C class
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOLINUXSTREAMI2C_MSG_TEN                       0x10
#define DIOLINUXSTREAMI2C_MSG_RD                        0x01
#define DIOLINUXSTREAMI2C_MSG_NOSTART                   0x4000
#define DIOLINUXSTREAMI2C_MSG_REV_DIR_ADDR              0x2000
#define DIOLINUXSTREAMI2C_MSG_SGIGNORE_NAK              0x1000
#define DIOLINUXSTREAMI2C_MSG_NO_RD_ACK                 0x0800

#define DIOLINUXSTREAMI2C_FUNC_I2C                      0x00000001
#define DIOLINUXSTREAMI2C_FUNC_10BIT_ADDR               0x00000002
#define DIOLINUXSTREAMI2C_FUNC_PROTOCOL_MANGLING        0x00000004  // I2C_M_{REV_DIR_ADDR,NOSTART,..}
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_PEC                0x00000008
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_BLOCK_PROC_CALL    0x00008000  // SMBus 2.0
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_QUICK              0x00010000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BYTE          0x00020000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BYTE         0x00040000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BYTE_TICKET   0x00080000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BYTE_TICKET  0x00100000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_WORD_TICKET   0x00200000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_WORD_TICKET  0x00400000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_PROC_CALL          0x00800000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BLOCK_TICKET  0x01000000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BLOCK_TICKET 0x02000000
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_I2C_BLOCK     0x04000000  // I2C-like block xfer
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_I2C_BLOCK    0x08000000  // w/ 1-byte reg. addr.

#define DIOLINUXSTREAMI2C_FUNC_SMBUS_BYTE               (DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BYTE       | DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BYTE)
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_BYTE_TICKET        (DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BYTE_TICKET  | DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BYTE_TICKET)
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_WORD_TICKET        (DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_WORD_TICKET  | DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_WORD_TICKET)
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_BLOCK_TICKET       (DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_BLOCK_TICKET | DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_BLOCK_TICKET)
#define DIOLINUXSTREAMI2C_FUNC_SMBUS_I2C_BLOCK          (DIOLINUXSTREAMI2C_FUNC_SMBUS_READ_I2C_BLOCK  | DIOLINUXSTREAMI2C_FUNC_SMBUS_WRITE_I2C_BLOCK)

#define DIOLINUXSTREAMI2C_FUNC_SMBUS_HWPEC_CALC         DIOLINUXSTREAMI2C_FUNC_SMBUS_PEC

#define DIOLINUXSTREAMI2C_SMBUS_BLOCK_MAX               32          // As specified in SMBus standard
#define DIOLINUXSTREAMI2C_SMBUS_I2C_BLOCK_MAX           32          // Not specified but we use same structure

#define DIOLINUXSTREAMI2C_SMBUS_READ                    1
#define DIOLINUXSTREAMI2C_SMBUS_WRITE                   0

#define DIOLINUXSTREAMI2C_SMBUS_QUICK                   0
#define DIOLINUXSTREAMI2C_SMBUS_BYTE                    1
#define DIOLINUXSTREAMI2C_SMBUS_BYTE_TICKET             2
#define DIOLINUXSTREAMI2C_SMBUS_WORD_TICKET             3
#define DIOLINUXSTREAMI2C_SMBUS_PROC_CALL               4
#define DIOLINUXSTREAMI2C_SMBUS_BLOCK_TICKET            5
#define DIOLINUXSTREAMI2C_SMBUS_I2C_BLOCK_BROKEN        6
#define DIOLINUXSTREAMI2C_SMBUS_BLOCK_PROC_CALL         7          // SMBus 2.0
#define DIOLINUXSTREAMI2C_SMBUS_I2C_BLOCK_TICKET        8

#define DIOLINUXSTREAMI2C_RETRIES                       0x0701    // number of times a device address should be polled when not acknowledging
#define DIOLINUXSTREAMI2C_TIMEOUT                       0x0702    // set timeout in units of 10 ms

#define DIOLINUXSTREAMI2C_SLAVE                         0x0703    // Use this slave address
#define DIOLINUXSTREAMI2C_SLAVE_FORCE                   0x0706    // Use this slave address, even if it is already in use by a driver!
#define DIOLINUXSTREAMI2C_TENBIT                        0x0704    // 0 for 7 bit addrs, != 0 for 10 bit

#define DIOLINUXSTREAMI2C_FUNCS                         0x0705    // Get the adapter functionality mask

#define DIOLINUXSTREAMI2C_RDWR                          0x0707    // Combined R/W transfer (one STOP only)

#define DIOLINUXSTREAMI2C_PEC                           0x0708    // != 0 to use PEC with SMBus
#define DIOLINUXSTREAMI2C_SMBUS                         0x0720    // SMBus transfer


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

typedef struct
{
  XWORD                                 addr;
  XWORD                                 flags;
  XWORD                                 size;
  XBYTE*                                buffer;

} DIOLINUXSTREAMI2C_MSG;


union DIOLINUXSTREAMI2C_SMBUS_TICKET
{
  XBYTE                                 byte;
  XWORD                                 word;
  XBYTE                                 block[DIOLINUXSTREAMI2C_SMBUS_BLOCK_MAX+2] ;  // block [0] is used for length + one more for PEC
};

typedef struct
{
  XBYTE                                 read_write;
  XBYTE                                 command;
  XDWORD                                size;
  union DIOLINUXSTREAMI2C_SMBUS_TICKET* data;

} DIOLINUXSTREAMI2C_SMBUS_IOCTL_TICKET;

typedef struct
{
  DIOLINUXSTREAMI2C_MSG*                msgs;
  XDWORD                                nmsgs;

} DIOLINUXSTREAMI2C_RDWR_IOCTL_TICKET;


class DIOLINUXSTREAMI2C : public DIOSTREAMI2C
{
  public:
                                  DIOLINUXSTREAMI2C                       ();
    virtual                      ~DIOLINUXSTREAMI2C                       ();

    bool                          Open                                    ();

    bool                          WaitToFilledReadingBuffer               (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);

    XDWORD                        ReadDirect                              (XBYTE* buffer, XDWORD size);
    XDWORD                        WriteDirect                             (XBYTE* buffer, XDWORD size);

    bool                          Close                                   ();

  private:

    void                          Clean                                   ();

    static void                   ThreadConnection                         (void* data);

    int                           handle;
    XWORD                         sizeread;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





