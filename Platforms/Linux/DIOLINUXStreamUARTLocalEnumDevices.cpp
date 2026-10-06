/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamUARTLocalEnumDevices.cpp
* 
* @class      DIOLINUXSTREAMUARTLOCALENUMDEVICES
* @brief      LINUX Data Input/Output Stream UART Local Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamUARTLocalEnumDevices.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#include "XTrace.h"
#include "XPath.h"
#include "DIOStreamDevice.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMUARTLOCALENUMDEVICES::DIOLINUXSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMUARTLOCALENUMDEVICES::DIOLINUXSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMUARTLOCALENUMDEVICES::~DIOLINUXSTREAMUARTLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMUARTLOCALENUMDEVICES::~DIOLINUXSTREAMUARTLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMUARTLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUARTLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  struct dirent** namelist;
  XSTRING         sysdir;
  int             ndevices;

  sysdir = _L("/sys/class/tty/");

  XBUFFER charstr;
  
  sysdir.ConvertToASCII(charstr);
  ndevices = scandir(charstr.GetPtrChar(), &namelist, NULL, NULL);  
  if(ndevices >= 0)
    {
      while(ndevices--)
        {
          if(strcmp(namelist[ndevices]->d_name, "..") && strcmp(namelist[ndevices]->d_name, "."))
            {
              XSTRING     devicedir;
              struct stat st;

              devicedir  = sysdir;
              devicedir += namelist[ndevices]->d_name;

              devicedir += _L("/device");

              XBUFFER charstr;
              
              devicedir.ConvertToASCII(charstr);
                            
              bool result = (lstat(charstr.GetPtrChar(), &st)==0) && (S_ISLNK(st.st_mode));                          
              if(result)
                {
                  char buffer[1024];

                  memset(buffer, 0, sizeof(buffer));

                  devicedir += _L("/driver");

                  XBUFFER charstr;
                  
                  devicedir.ConvertToASCII(charstr);                                   
                  result = (readlink(charstr.GetPtrChar(), buffer, sizeof(buffer)) > 0);                  
                  if(result)
                    {
                      XSTRING tty;
                      XPATH   devicepath;
                      XPATH   devicefile;

                      tty = buffer;

                      devicepath  = sysdir;
                      devicepath += namelist[ndevices]->d_name;

                      devicepath.GetNamefile(devicefile);

                      devicepath  = _L("/dev/");
                      devicepath += devicefile;

                      //XTRACE_PRINTCOLOR(4,_L(" %s %s %s"),devicedir.Get(), devicepath.Get(), tty.Get());

                      bool isvalid = false;

                      if(tty.Find(_L("serial8250"), true) != XSTRING_NOTFOUND)
                        {
                           struct serial_struct serinfo;

                           XBUFFER charstr;
                           
                           devicepath.ConvertToASCII(charstr);                           
                           int fd = open(charstr.GetPtrChar(), O_RDWR | O_NONBLOCK | O_NOCTTY);                           
                           if(fd >= 0)
                            {
                              // Get serial_info
                              if(ioctl(fd, TIOCGSERIAL, &serinfo)==0)
                                {
                                  // If device type is no PORT_UNKNOWN we accept the port
                                  if(serinfo.type != PORT_UNKNOWN) isvalid = true;
                                }

                              close(fd);
                            }
                        } else isvalid = true;

                      if(isvalid)
                        {
                          DIOSTREAMDEVICE* device = GEN_NEW DIOSTREAMDEVICE();
                          if(device)
                            {
                              //device->SetIndex(index);
                              device->GetResource()->Set(devicepath);

                              devices.Add(device);
                            }
                        }
                    }
                }
            }

          free(namelist[ndevices]);
        }

      free(namelist);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMUARTLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
* @brief      Is device available
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUARTLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
{
  /*
  XSTRING strdevice;

  strdevice = resource;

  HANDLE hcomt = CreateFile(strdevice.Get(),
                            GENERIC_READ|GENERIC_WRITE,
                            0,0,
                            OPEN_EXISTING,
                            0,0);

  if(hcomt==INVALID_HANDLE_VALUE) return false;

  CloseHandle(hcomt);
  */
  return true;
}




#endif

