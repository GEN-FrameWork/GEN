/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUARTLocalEnumDevices.cpp
* 
* @class      DIOWINDOWSSTREAMUARTLOCALENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream UART Local Enum Devices class
* @ingroup    PLATFORM_WINDOWS
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

#include "DIOWINDOWSStreamUARTLocalEnumDevices.h"

#include <windows.h>

#include "DIOStreamDevice.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::DIOWINDOWSSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::DIOWINDOWSSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::~DIOWINDOWSSTREAMUARTLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::~DIOWINDOWSSTREAMUARTLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  HKEY  hkey;
  LONG  status;

  status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _L("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_QUERY_VALUE, &hkey);
  if(status==ERROR_SUCCESS)
    {
      DWORD   index  = 0;
      XCHAR*  name   = GEN_NEW XCHAR[_MAXSTR];
      DWORD   sizename;
      DWORD   type;
      XCHAR*  value  = GEN_NEW XCHAR[_MAXSTR];
      DWORD   sizevalue;

      if((value)&&(name))
        {
          memset(name, 0, sizeof(XCHAR)*_MAXSTR);
          memset(value, 0, sizeof(XCHAR)*_MAXSTR);

          do{ name[0]   = 0;
              sizename  = _MAXSTR;
              value[0]  = 0;
              sizevalue = _MAXSTR;
              type      = 0;

              status=RegEnumValue(hkey, index, name, &sizename, NULL, &type, (XBYTE*)value, &sizevalue);
              if(status==ERROR_SUCCESS)
                {
                  DIOSTREAMDEVICE* device = GEN_NEW DIOSTREAMDEVICE();
                  if(device)
                    {
                      device->SetIndex(index);
                      device->SetType(DIOSTREAMDEVICE_TYPE_UART);

                      device->GetResource()->Set(value);

                      devices.Add(device);
                    }

                  index++;
                }

            } while(status==ERROR_SUCCESS);
        }

      GEN_DELETE_ARRAY value;
      GEN_DELETE_ARRAY name;

      RegCloseKey(hkey);
    }

  return true;
}




#endif

