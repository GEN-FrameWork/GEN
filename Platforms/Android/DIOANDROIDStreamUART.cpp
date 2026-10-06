/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamUART.cpp
* 
* @class      DIOANDROIDSTREAMUART
* @brief      ANDROID Data Input/Output Stream UART class
* @ingroup    PLATFORM_ANDROID
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDStreamUART.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <iostream>
#include <errno.h>
#include <termios.h>

#include <sys/wait.h>
#include <string.h>


#include "XBase.h"
#include "XFactory.h"
#include "XString.h"
#include "XTrace.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamUARTConfig.h"
#include "DIOANDROIDStreamUART.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART() : DIOSTREAMUART() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART() : DIOSTREAMUART() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOANDROIDUARTFSMSTATE_NONE             ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_CONNECTED        ,
            DIOANDROIDUARTFSMEVENT_WAITINGTOREAD    , DIOANDROIDUARTFSMSTATE_WAITINGTOREAD     ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_DISCONNECTING    ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_WAITINGTOREAD    , DIOANDROIDUARTFSMSTATE_WAITINGTOREAD     ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, _L("DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART"), ThreadConnection, (void*)this);

  //threadconnection->SetPriority(THREADPRIORITY_REALTIME);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUART::~DIOANDROIDSTREAMUART()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUART::~DIOANDROIDSTREAMUART()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOANDROIDSTREAMUART::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_ANDROID
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOANDROIDSTREAMUART::GetStatus()
{
  if(fd<0)    return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::Open()
* @brief      Open
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::Open()
{
  if(!config) return false;

  XSTRING ttyS;

  if(!config->GetLocalDeviceName()->IsEmpty())
    {
      ttyS = config->GetLocalDeviceName()->Get();
    }
   else
    {
      ttyS.Format(_L("/dev/ttyS%d"), config->GetPort());
    }

  XBUFFER charstr;
  
  ttyS.ConvertToASCII(charstr); 
  fd= open(charstr.GetPtrChar(), O_RDWR | O_NOCTTY | O_NONBLOCK);  
  if(fd<0) return false;

  SetRTS(false);
  SetDTR(false);

  CleanBuffers();

  //fcntl(fd, F_SETFL, FNDELAY);
 //fcntl(fd, F_SETFL, 0);

  if(!Config()) return false;

  SetEvent(DIOANDROIDUARTFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::Config(XWORD mask)
* @brief      Config
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::Config(XWORD mask)
{
  if(fd<0) return false;

  struct termios cfg;
  speed_t        speed;

  tcgetattr(fd, &cfg);      // save current port settings

  if((mask&DIOSTREAMUARTMASK_BAUDRATE) == DIOSTREAMUARTMASK_BAUDRATE)
    {
      switch(this->config->GetBaudRate())
        {
          case 50     : speed = B50;        break;
          case 300    : speed = B300;       break;
          case 600    : speed = B600;       break;
          case 1200   : speed = B1200;      break;
          case 4800   : speed = B4800;      break;
          case 9600   : speed = B9600;      break;
          case 19200  : speed = B19200;     break;
          case 38400  : speed = B38400;     break;
          case 57600  : speed = B57600;     break;
          case 115200 : speed = B115200;    break;
        }

      cfsetispeed(&cfg, speed);
      cfsetospeed(&cfg, speed);
    }

  if((mask&DIOSTREAMUARTMASK_DATABITS ) == DIOSTREAMUARTMASK_DATABITS)
    {
      cfg.c_cflag&= ~CSIZE;

      switch(this->config->GetDataBits())
        {
          case DIOSTREAMUARTDATABIT_5           : cfg.c_cflag|= CS5; break;
          case DIOSTREAMUARTDATABIT_6           : cfg.c_cflag|= CS6; break;
          case DIOSTREAMUARTDATABIT_7           : cfg.c_cflag|= CS7; break;
          case DIOSTREAMUARTDATABIT_8           : cfg.c_cflag|= CS8; break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_PARITY) == DIOSTREAMUARTMASK_PARITY)
    {
      cfg.c_iflag  = (IGNBRK| IGNPAR);

      cfg.c_cflag &= ~CMSPAR;
      cfg.c_cflag &= ~PARENB;
      cfg.c_cflag &= ~PARODD;

      switch(this->config->GetParity())
        {
          case DIOSTREAMUARTPARITY_NONE         : cfg.c_cflag &= ~CMSPAR;
                                                  cfg.c_cflag &= ~PARENB;
                                                  cfg.c_cflag &= ~PARODD;
                                                  break;

          case DIOSTREAMUARTPARITY_MARK         : cfg.c_cflag |= CMSPAR;       // Not available on many systems.

          case DIOSTREAMUARTPARITY_ODD          : cfg.c_cflag |= PARENB;
                                                  cfg.c_cflag |= PARODD;
                                                  break;

          case DIOSTREAMUARTPARITY_SPACE        : cfg.c_cflag |= CMSPAR;      // Not available on many systems.

          case DIOSTREAMUARTPARITY_EVEN         : cfg.c_cflag |= PARENB;
                                                  cfg.c_cflag &= ~PARODD;
                                                  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_STOPBITS ) == DIOSTREAMUARTMASK_STOPBITS)
    {
      cfg.c_cflag &= ~CSTOPB;

      switch(this->config->GetStopBits())
        {
          case DIOSTREAMUARTSTOPBITS_ONE          : cfg.c_cflag &= ~CSTOPB;   break;
          case DIOSTREAMUARTSTOPBITS_ONEANDAHALF  :
          case DIOSTREAMUARTSTOPBITS_TWO          : cfg.c_cflag |=  CSTOPB;   break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_FLOWCONTROL) == DIOSTREAMUARTMASK_FLOWCONTROL)
    {
      switch(config->GetFlowControl())
        {
          case DIOSTREAMUARTFLOWCONTROL_NONE    : cfg.c_cflag &= ~CRTSCTS;
                                                  cfg.c_iflag &= ~(IXON | IXOFF | IXANY);
                                                  break;

          case DIOSTREAMUARTFLOWCONTROL_HARD    : cfg.c_cflag |=  CRTSCTS;
                                                  cfg.c_iflag &= ~(IXON | IXOFF | IXANY);
                                                  break;

          case DIOSTREAMUARTFLOWCONTROL_SOFT    : cfg.c_cflag &= ~CRTSCTS;
                                                  cfg.c_iflag |= (IXON | IXOFF | IXANY);
                                                  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_ALL) == DIOSTREAMUARTMASK_ALL)
    {

      cfg.c_cflag |= (CLOCAL | CREAD);

      cfg.c_oflag &= ~OPOST;
      cfg.c_lflag &= ~(ICANON | ECHO | ISIG);


      cfg.c_cc[VMIN]  = 0;      // 0 means use-vtime
      cfg.c_cc[VTIME] = 1;      // time to wait until
    }

  tcsetattr(fd, TCSANOW, &cfg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOANDROIDSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
* @brief      Read direct
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOANDROIDSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(fd<0)                                             return 0;
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = readtimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int br = 0;

  if(select(fd+1, &fds, NULL, NULL, &tv) > 0)
    {
      br = read(fd, buffer, size);

      if(br< 0)
        {
          /*
          if(errno == EAGAIN)
            {

            }
          else
            {
              return 0;
            }
          */

          return 0;
        }
    }

  return (XDWORD)br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOANDROIDSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOANDROIDSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(fd<0)                                             return 0;
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = writetimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int bw = 0;

  if(select(fd+1, NULL , &fds, NULL, &tv) > 0)
    {
      bw = write(fd, buffer, size);
      if(bw<0) return 0;
    }

  return (XDWORD)bw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::Disconnect()                                
{ 
  return false; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::Close()
* @brief      Close
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::Close()
{
  //if(threadconnection) threadconnection->End();

  //if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(fd != -1)
    {
      close(fd);
      fd = -1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::GetCTS()
* @brief      Get CTS
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::GetCTS()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_CTS) != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::GetDSR()
* @brief      Get DSR
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::GetDSR()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_DSR) != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::GetRing()
* @brief      Get ring
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::GetRing()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_RNG) != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::GetRLSD()
* @brief      Get RLSD
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::GetRLSD()
{
  if(fd<0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::SetRTS(bool on)
* @brief      Set RTS
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::SetRTS(bool on)
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status); // get the serial port status

  if(on) _status &= ~TIOCM_RTS; else _status |= TIOCM_RTS;

  ioctl(fd, TIOCMSET, &_status); // set the serial port status

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::SetDTR(bool on)
* @brief      Set DTR
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::SetDTR(bool on)
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);  // get the serial port status

  if(on) _status &= ~TIOCM_DTR; else  _status |= TIOCM_DTR;

  ioctl(fd, TIOCMSET, &_status);  // set the serial port status

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUART::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::CleanBuffers()
{
  if(fd<0) return false;

  tcflush(fd, TCIOFLUSH);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMUART::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMUART::ThreadConnection(void* param)
{
  DIOANDROIDSTREAMUART* diostream = (DIOANDROIDSTREAMUART*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOANDROIDUARTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOANDROIDUARTFSMSTATE_NONE                    : break;

          case DIOANDROIDUARTFSMSTATE_CONNECTED               : break;

          case DIOANDROIDUARTFSMSTATE_WAITINGTOREAD           : {
                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                      XDWORD br = diostream->ReadDirect(buffer, DIOSTREAM_MAXBUFFER);
                                                                      if(br)
                                                                        {
                                                                          //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, (XBYTE*)diostream->buffer, (int)br);
                                                                          diostream->inbuffer->Add(buffer, br);
                                                                        }
                                                                    }

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      int esize = diostream->outbuffer->GetSize();
                                                                      if(esize)
                                                                        {
                                                                          if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->WriteDirect(diostream->outbuffer->Get(), esize);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_RED, (XBYTE*)diostream->buffer, (int)bw);
                                                                              diostream->outbuffer->Extract(NULL, 0, bw);
                                                                            }
                                                                        }
                                                                    }
                                                              }
                                                              break;

          case DIOANDROIDUARTFSMSTATE_DISCONNECTING           : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOANDROIDUART_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOANDROIDUARTFSMSTATE_NONE                : break;

              case DIOANDROIDUARTFSMSTATE_CONNECTED           : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOANDROIDUARTFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOANDROIDUARTFSMSTATE_WAITINGTOREAD       : break;

              case DIOANDROIDUARTFSMSTATE_DISCONNECTING       : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->threadconnection->Run(false);
                                                                diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMUART::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMUART::Clean()
{
  fd            = -1;
  readtimeout   = 3000;
  writetimeout  = 3000;
}




#endif

