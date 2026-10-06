/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOMPSSE.cpp
* 
* @class      DIOMPSSE
* @brief      Data Input/Output Multi-Protocol Synchronous Serial Engine (MPSSE) class
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

#include "DIOMPSSE.h"

#include "DIOFactory.h"
#include "DIOStreamConfig.h"
#include "DIOStreamDeviceUSB.h"
#include "DIOStreamUSBConfig.h"
#include "DIOStreamUSBLocalEnumDevices.h"
#include "DIOStream.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMPSSE::DIOMPSSE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMPSSE::DIOMPSSE()
{
  Clean();

  dioenumUSBdevices  = (DIOSTREAMUSBLOCALENUMDEVICES*)GEN_DIOFACTORY.CreateStreamEnumDevices(DIOSTREAMENUMTYPE_USB_LOCAL);
  if(dioenumUSBdevices)  diostreamUSBcfg = GEN_NEW DIOSTREAMUSBCONFIG();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMPSSE::~DIOMPSSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMPSSE::~DIOMPSSE()
{
  if(dioenumUSBdevices) GEN_DIOFACTORY.DeleteStreamEnumDevices(dioenumUSBdevices);

  if(diostreamUSBcfg)   GEN_DELETE diostreamUSBcfg;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMPSSE_CHIP_TYPE DIOMPSSE::GetChipType()
* @brief      Get chip type
* @ingroup    DATAIO
* 
* @return     DIOMPSSE_CHIP_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMPSSE_CHIP_TYPE DIOMPSSE::GetChipType()
{
  return chiptype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOMPSSE::GetChipTypeName()
* @brief      Get chip type name
* @ingroup    DATAIO
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOMPSSE::GetChipTypeName()
{
  switch(chiptype)
    {
      case DIOMPSSE_CHIP_TYPE_AM       :  return _L("AM");
      case DIOMPSSE_CHIP_TYPE_BM       :  return _L("BM");
      case DIOMPSSE_CHIP_TYPE_2232C    :  return _L("2232C");
      case DIOMPSSE_CHIP_TYPE_R        :  return _L("R");
      case DIOMPSSE_CHIP_TYPE_2232H    :  return _L("2232H");
      case DIOMPSSE_CHIP_TYPE_4232H    :  return _L("4232H");
      case DIOMPSSE_CHIP_TYPE_232H     :  return _L("232H");
                            default    :  return _L("Unknown");
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::Open(int index)
* @brief      Open
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::Open(int index)
{
  if(!dioenumUSBdevices)  return false;

  EnumDevices();

  XVECTOR<DIOSTREAMDEVICE*>* devices = dioenumUSBdevices->GetDevices();
  if(!devices)            return false;
  if(devices->IsEmpty())  return false;

  USBdevice = (DIOSTREAMDEVICEUSB*)devices->Get(index);
  if(!USBdevice) return false;

  switch(USBdevice->GetDescriptor()->GetBCDDevice())
    {
      case 0x200  : if(USBdevice->GetDescriptor()->GetSerialNumber() == 0) chiptype = DIOMPSSE_CHIP_TYPE_BM;
                    break;

      case 0x400  : chiptype = DIOMPSSE_CHIP_TYPE_BM;                                         break;
      case 0x500  : chiptype = DIOMPSSE_CHIP_TYPE_2232C;                                      break;
      case 0x600  : chiptype = DIOMPSSE_CHIP_TYPE_R;                                          break;
      case 0x700  : chiptype = DIOMPSSE_CHIP_TYPE_2232H;                                      break;
      case 0x800  : chiptype = DIOMPSSE_CHIP_TYPE_4232H;                                      break;
      case 0x900  : chiptype = DIOMPSSE_CHIP_TYPE_232H;                                       break;

          default : chiptype = DIOMPSSE_CHIP_TYPE_UNKNOWN;                                    break;
    }

  diostreamUSBcfg->GetResource()->Set((*USBdevice->GetResource()));

  if(diostreambase)
    {
      if(diostreambase->IsConnected()) Close();
      GEN_DIOFACTORY.DeleteStreamIO(diostreambase);

      diostreambase = NULL;
    }

  diostreambase = GEN_DIOFACTORY.CreateStreamIO(diostreamUSBcfg);
  if(!diostreambase) return false;

  if(!diostreambase->Open())  return false;

  if(!Reset()) return false;

  isopen = true;

  return isopen;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::IsOpen()
* @brief      Is open
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::IsOpen()
{
  return isopen;
}


/*
int API_EXPORTED libusb_control_transfer(libusb_device_handle *dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
{
  struct libusb_transfer *transfer = libusb_alloc_transfer(0);
  unsigned char *buffer;
  int completed = 0;
  int r;

  if (!transfer) return LIBUSB_ERROR_NO_MEM;

  buffer = (unsigned char*) malloc(LIBUSB_CONTROL_SETUP_SIZE + wLength);
  if(!buffer)
    {
      libusb_free_transfer(transfer);
      return LIBUSB_ERROR_NO_MEM;
    }

  libusb_fill_control_setup(buffer, bmRequestType, bRequest, wValue, wIndex, wLength);
  if ((bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT) memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, wLength);

  libusb_fill_control_transfer(transfer, dev_handle, buffer, ctrl_transfer_cb, &completed, timeout);
  transfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;
  r = libusb_submit_transfer(transfer);
  if (r < 0)
    {
      libusb_free_transfer(transfer);
      return r;
    }

  while (!completed)
    {
      r = libusb_handle_events_completed(HANDLE_CTX(dev_handle), &completed);
      if (r < 0)
        {
          if (r == LIBUSB_ERROR_INTERRUPTED) continue;

          libusb_cancel_transfer(transfer);

          while (!completed)
            if (libusb_handle_events_completed(HANDLE_CTX(dev_handle), &completed) < 0) break;

          libusb_free_transfer(transfer);

          return r;
        }
  }


  if ((bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) memcpy(data, libusb_control_transfer_get_data(transfer), transfer->actual_length);

  switch (transfer->status)
    {
      case LIBUSB_TRANSFER_COMPLETED  : r = transfer->actual_length;
                                        break;

      case LIBUSB_TRANSFER_TIMED_OUT  : r = LIBUSB_ERROR_TIMEOUT;
                                        break;

      case LIBUSB_TRANSFER_STALL      : r = LIBUSB_ERROR_PIPE;
                                        break;

      case LIBUSB_TRANSFER_NO_DEVICE  : r = LIBUSB_ERROR_NO_DEVICE;
                                        break;

      case LIBUSB_TRANSFER_OVERFLOW   : r = LIBUSB_ERROR_OVERFLOW;
                                        break;
      case LIBUSB_TRANSFER_ERROR      :
      case LIBUSB_TRANSFER_CANCELLED  : r = LIBUSB_ERROR_IO;
                                        break;

                              default : usbi_warn(HANDLE_CTX(dev_handle), "unrecognised status code %d", transfer->status);
                                        r = LIBUSB_ERROR_OTHER;
    }

  libusb_free_transfer(transfer);
  return r;
}
*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::Reset()
* @brief      Reset
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::Reset()
{
  /*
  int API_EXPORTED libusb_control_transfer(libusb_device_handle *dev_handle,
                                           uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
                                           unsigned char *data, uint16_t wLength, unsigned int timeout)

   if (ftdi == NULL || ftdi->usb_dev == NULL) ftdi_error_return(-2, "USB device unavailable");

   if (libusb_control_transfer(ftdi->usb_dev, FTDI_DEVICE_OUT_REQTYPE,
                                SIO_RESET_REQUEST, SIO_RESET_SIO,
                                ftdi->index, NULL, 0, ftdi->usb_write_timeout) < 0)
   ftdi_error_return(-1,"FTDI reset failed");

   // Invalidate data in the readbuffer
   ftdi->readbuffer_offset    = 0;
   ftdi->readbuffer_remaining = 0;
  */

  XBUFFER buffer;

  if(SendCommand(DIOMPSSE_SIO_RESET_REQUEST, buffer))
    {
      buffer.Empty();
      ReadResponse(buffer, 5);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::Close()
* @brief      Close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::Close()
{
  if(!diostreambase)  return false;

  diostreambase->Close();

  if(diostreambase)
    {
      GEN_DIOFACTORY.DeleteStreamIO(diostreambase);
      diostreambase = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::EnumDevices()
* @brief      Enum devices
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::EnumDevices()
{
  if(!dioenumUSBdevices) return false;

  if(!dioenumUSBdevices->Search()) return false;

  XVECTOR<DIOSTREAMDEVICE*>* devices = dioenumUSBdevices->GetDevices();
  if(!devices)            return false;
  if(devices->IsEmpty())  return false;

  DIOSTREAMDEVICEUSB* device;
  int                 index = 0;

  do{ device = (DIOSTREAMDEVICEUSB*)devices->Get(index);
      if(device)
        {
          if((device->GetVendorID() == 0x403) &&  ((device->GetProductID() == 0x6001) ||
                                                   (device->GetProductID() == 0x6010) ||
                                                   (device->GetProductID() == 0x6011) ||
                                                   (device->GetProductID() == 0x6014)))
            {
              index++;
            }
           else
            {
              devices->Delete(device);
              GEN_DELETE device;
            }
        }

    } while(device);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::SendCommand(XBYTE command, XBUFFER& buffer)
* @brief      Send command
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::SendCommand(XBYTE command, XBUFFER& buffer)
{
  XBUFFER datasend;
  XBYTE   lowsize;
  XBYTE   highsize;
  bool    status;

  if(buffer.GetSize() > 65535)  return false;

  lowsize   = (buffer.GetSize()  & 0x00FF);
  highsize  = ((buffer.GetSize() & 0xFF00) >> 0x08);

  datasend.Add((XBYTE)command);
  datasend.Add((XBYTE)lowsize);
  datasend.Add((XBYTE)highsize);

  datasend.Add(buffer);

  status = diostreambase->Write(datasend);
  if(status) status = diostreambase->WaitToFlushOutXBuffer(5);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMPSSE::ReadResponse(XBUFFER& xbuffer, int timeout)
* @brief      Read response
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMPSSE::ReadResponse(XBUFFER& xbuffer, int timeout)
{
  if(!diostreambase->WaitToFilledReadingBuffer(1, timeout)) return false;

  xbuffer.Resize(diostreambase->GetInXBuffer()->GetSize());
  if(!diostreambase->Read(xbuffer)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOMPSSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOMPSSE::Clean()
{
  chiptype          = DIOMPSSE_CHIP_TYPE_UNKNOWN;
  isopen            = false;

  dioenumUSBdevices = NULL;
  diostreamUSBcfg   = NULL;
  USBdevice         = NULL;
  diostreambase     = NULL;
}



