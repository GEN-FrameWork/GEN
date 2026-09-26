/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSDriveImageManager.h
* 
* @class      XWINDOWSDRIVEIMAGEMANAGER
* @brief      WINDOWS eXtended Utils Drive Image Manager class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include <windows.h>
#include <winioctl.h>

#include "XDriveImageManager.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef struct
{
  DEVICE_TYPE  devicetype;
  XQWORD       devicenumber;
  XQWORD       partitionnumber;

} XWINDOWSDRIVEIMAGE_DEVICE_NUMBER;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSDRIVEIMAGEMANAGER : public XDRIVEIMAGEMANAGER
{
  public:
                                XWINDOWSDRIVEIMAGEMANAGER       ();
    virtual                    ~XWINDOWSDRIVEIMAGEMANAGER       ();

    bool                        GetDrives                       (XVECTOR<XDRIVEIMAGEMANAGER_DRIVE*>& drives);

    bool                        Open                            (XDRIVEIMAGEMANAGER_DRIVE& drive);

    bool                        Read                            (XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer);
    bool                        Write                           (XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer);

    bool                        Close                           (XDRIVEIMAGEMANAGER_DRIVE& drive);

  private:

    HANDLE                      GetHandleFromDevice             (int device, XDWORD access);
    HANDLE                      GetHandleFromVolume             (int volume, XDWORD access);

    bool                        GetDeviceID                     (HANDLE handlevolume, XDWORD& deviceID);

    bool                        LockVolume                      (HANDLE handle, bool islock);

    bool                        UnMountVolume                   (HANDLE handle);
    bool                        IsVolumeUnmounted               (HANDLE handle);

    bool                        GetNumberOfSectors              (HANDLE handle, XQWORD& sectorsize, XQWORD& numbersectors);
    bool                        GetSpaceAvailable               (XCHAR* location, XQWORD& spaveavailable);

    bool                        GetDriveLabel                   (XCHAR* location, XSTRING& label);

    bool                        CheckDriveType                  (XPATH& path, XQWORD& ID);

    bool                        GetMediaType                    (HANDLE hdevice);

    bool                        ReadSectorDataFromHandle        (HANDLE handle, XQWORD startsector, XQWORD sectorsize, XBYTE* buffer, XQWORD& size);
    bool                        WriteSectorDataToHandle         (HANDLE handle, XQWORD startsector, XQWORD sectorsize, XBYTE* buffer, XQWORD& size);

    void                        Clean                           ();

    HANDLE                      handle;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





