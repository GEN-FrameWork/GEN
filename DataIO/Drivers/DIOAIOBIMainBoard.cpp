/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOAIOBIMainBoard.cpp
* 
* @class      DIOAIOBIMAINBOARD
* @brief      Data Input/Output AIOBI Main board class
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

#include "DIOAIOBIMainBoard.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAIOBIMAINBOARD::DIOAIOBIMAINBOARD()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAIOBIMAINBOARD::DIOAIOBIMAINBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAIOBIMAINBOARD::~DIOAIOBIMAINBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAIOBIMAINBOARD::~DIOAIOBIMAINBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::Ini()
{
  for(int c=0; c<3; c++)
    {
      pcf8574[c] = GEN_NEW DIOI2CGPIOPCF8574();
      if(!pcf8574[c]) return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME     , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_IN      , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO IN not init!"));
      return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME    , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_OUT     , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO OUT not init!"));
      return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_RELAYS  , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO Relays not init!"));
      return false;
    }

  for(int c=1; c<9; c++)
    {
      SetDigitalOutput(c, false);
    }

  eeprom  = GEN_NEW DIOI2CEEPROM24XXX();
  if(!eeprom) return false;

  eeprom->SetType(DIOI2CEEPROM24XXXTYPE_16);

  if(!eeprom->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME, DIOAIOBIMAINBOARD_I2CEEPROM, DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C Serial EEPROM not init!"));
      return false;
    }


  pca9685 = GEN_NEW DIOI2CPWMCONTROLERPCA9685();
  if(!pca9685) return false;

  if(!pca9685->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME, DIOAIOBIMAINBOARD_I2CPWM, DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C PCA9685 (PWM) not init!"));
      return false;
    }


  gpio = GEN_DIOFACTORY.CreateGPIO();
  if(!gpio)
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! GPIO not created!"));
      return false;
    }

  if(!gpio->Ini())
    {
      GEN_XLOG.AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! GPIO not init!"));
      return false;
    }

  isini = true;

  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::IsIni()
* @brief      Is ini
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin, bool isinput)
* @brief      Set mode header pin
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* @param[in]  pin : Pin value.
* @param[in]  isinput : Isinput value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin, bool isinput)
{
  if(!gpio) return false;

  switch(header)
    {
      case DIOAIOBIMAINBOARD_HEADER_P1        : switch(pin)
                                                  {
                                                    case 1  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN1     , isinput)) return false; break;
                                                    case 2  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN2     , isinput)) return false; break;
                                                    case 3  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN3     , isinput)) return false; break;
                                                    case 4  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN4     , isinput)) return false; break;
                                                    case 5  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN5     , isinput)) return false; break;
                                                    case 6  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P1_PIN6     , isinput)) return false; break;
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_P24       : switch(pin)
                                                  {
                                                    case 2  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P24_PIN2    , isinput)) return false; break;
                                                    case 3  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P24_PIN3    , isinput)) return false; break;
                                                    case 4  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P24_PIN4    , isinput)) return false; break;
                                                    case 5  : if(!gpio->SetMode(DIOAIOBIMAINBOARD_P24_PIN5    , isinput)) return false; break;
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_SLOT      : switch(pin)
                                                  {
                                                    case  9 : if(!gpio->SetMode(DIOAIOBIMAINBOARD_SLOT_PIN9   , isinput)) return false; break;
                                                    case 10 : if(!gpio->SetMode(DIOAIOBIMAINBOARD_SLOT_PIN10  , isinput)) return false; break;
                                                    case 11 : if(!gpio->SetMode(DIOAIOBIMAINBOARD_SLOT_PIN11  , isinput)) return false; break;
                                                    case 21 : if(!gpio->SetMode(DIOAIOBIMAINBOARD_SLOT_PIN21  , isinput)) return false; break;
                                                    default : return false;
                                                  }
                                                break;

                                  default     : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::GetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin)
* @brief      Get status header pin
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* @param[in]  pin : Pin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::GetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin)
{
  if(!gpio) return false;

  switch(header)
    {
      case DIOAIOBIMAINBOARD_HEADER_P1        : switch(pin)
                                                  {
                                                    case 1  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN1);
                                                    case 2  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN2);
                                                    case 3  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN3);
                                                    case 4  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN4);
                                                    case 5  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN5);
                                                    case 6  : return gpio->Get(DIOAIOBIMAINBOARD_P1_PIN6);
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_P24       : switch(pin)
                                                  {
                                                    case 2  : return gpio->Get(DIOAIOBIMAINBOARD_P24_PIN2);
                                                    case 3  : return gpio->Get(DIOAIOBIMAINBOARD_P24_PIN3);
                                                    case 4  : return gpio->Get(DIOAIOBIMAINBOARD_P24_PIN4);
                                                    case 5  : return gpio->Get(DIOAIOBIMAINBOARD_P24_PIN5);
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_SLOT      : switch(pin)
                                                  {
                                                    case  9 : return gpio->Get(DIOAIOBIMAINBOARD_SLOT_PIN9);
                                                    case 10 : return gpio->Get(DIOAIOBIMAINBOARD_SLOT_PIN10);
                                                    case 11 : return gpio->Get(DIOAIOBIMAINBOARD_SLOT_PIN11);
                                                    case 21 : return gpio->Get(DIOAIOBIMAINBOARD_SLOT_PIN21);
                                                    default : return false;
                                                  }
                                                break;

                                  default     : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin, bool active)
* @brief      Set status header pin
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* @param[in]  pin : Pin value.
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER header, int pin, bool active)
{
  if(!gpio) return false;

  switch(header)
    {
      case DIOAIOBIMAINBOARD_HEADER_P1        : switch(pin)
                                                  {
                                                    case 1  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN1, active);
                                                    case 2  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN2, active);
                                                    case 3  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN3, active);
                                                    case 4  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN4, active);
                                                    case 5  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN5, active);
                                                    case 6  : return gpio->Set(DIOAIOBIMAINBOARD_P1_PIN6, active);
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_P24       : switch(pin)
                                                  {
                                                    case 2  : return gpio->Set(DIOAIOBIMAINBOARD_P24_PIN2, active);
                                                    case 3  : return gpio->Set(DIOAIOBIMAINBOARD_P24_PIN3, active);
                                                    case 4  : return gpio->Set(DIOAIOBIMAINBOARD_P24_PIN4, active);
                                                    case 5  : return gpio->Set(DIOAIOBIMAINBOARD_P24_PIN5, active);
                                                    default : return false;
                                                  }
                                                break;

      case DIOAIOBIMAINBOARD_HEADER_SLOT      : switch(pin)
                                                  {
                                                    case  9 : return gpio->Set(DIOAIOBIMAINBOARD_SLOT_PIN9 , active);
                                                    case 10 : return gpio->Set(DIOAIOBIMAINBOARD_SLOT_PIN10, active);
                                                    case 11 : return gpio->Set(DIOAIOBIMAINBOARD_SLOT_PIN11, active);
                                                    case 21 : return gpio->Set(DIOAIOBIMAINBOARD_SLOT_PIN21, active);
                                                    default : return false;
                                                  }
                                                break;

                                  default     : return false;
  }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetP4HeaderForExternalButton(bool on)
* @brief      Set P4 header for external button
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetP4HeaderForExternalButton(bool on)
{
  if(!SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 2, on))    return false;
  if(!SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 3, on))    return false;
  if(!SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 4, false)) return false;
  if(!SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 5, false)) return false;

  SetLedA(false);
  SetLedB(false);

  issetp24forexternalbutton = on;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::IsButtonAPressed()
* @brief      Is button A pressed
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::IsButtonAPressed()
{
  if(!issetp24forexternalbutton) return false;
  return GetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::IsButtonBPressed()
* @brief      Is button B pressed
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::IsButtonBPressed()
{
  if(!issetp24forexternalbutton) return false;
  return GetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 3);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetLedA(bool on)
* @brief      Set led a
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetLedA(bool on)
{
  if(!issetp24forexternalbutton) return false;
  return SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 4, on);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetLedB(bool on)
* @brief      Set led b
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetLedB(bool on)
{
  if(!issetp24forexternalbutton) return false;
  return SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_P24, 5, on);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::GetDigitalInput(XBYTE& data)
* @brief      Get digital input
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::GetDigitalInput(XBYTE& data)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN])                          return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->IsInitialized())         return false;

  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->Read(data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetDigitalOutput(XBYTE bit, bool on)
* @brief      Set digital output
* @ingroup    DATAIO
* 
* @param[in]  bit : Bit value.
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetDigitalOutput(XBYTE bit, bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT])                           return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->IsInitialized())          return false;

  XBYTE mask = 0x01;

  if(bit-1)
    {
      mask <<= (bit-1);
    }

  if(on)
        outputdigitaldata |=  mask;
   else outputdigitaldata &= ~mask;

  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->Write(outputdigitaldata);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SwitchReleK1(bool on)
* @brief      Switch rele K1
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK1(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x01):(value&~0x01);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SwitchReleK2(bool on)
* @brief      Switch rele K2
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK2(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x02):(value&~0x02);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SwitchReleK3(bool on)
* @brief      Switch rele K3
* @ingroup    DATAIO
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK3(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x04):(value&~0x04);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::EEProm(bool write, XBUFFER& xbuffer)
* @brief      EE prom
* @ingroup    DATAIO
* 
* @param[in]  write : Write value.
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::EEProm(bool write, XBUFFER& xbuffer)
{
  if(!eeprom)                   return false;
  if(!eeprom->IsInitialized())  return false;

  bool status;

  if(!write)
        status =  eeprom->Read(0, 16, xbuffer);
    else status =  eeprom->Write(0, xbuffer);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOGPIO* DIOAIOBIMAINBOARD::GetGPIO()
* @brief      Get GPIO
* @ingroup    DATAIO
* 
* @return     DIOGPIO* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOGPIO* DIOAIOBIMAINBOARD::GetGPIO()
{
  return gpio;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::SetPWM(XBYTE channel, XWORD on, XWORD off, bool wait)
* @brief      Set PWM
* @ingroup    DATAIO
* 
* @param[in]  channel : Channel value.
* @param[in]  on : On value.
* @param[in]  off : Off value.
* @param[in]  wait : Wait value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetPWM(XBYTE channel, XWORD on, XWORD off, bool wait)
{
  if(!pca9685) return false;
  return pca9685->SetPWM(channel, on, off, wait);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::ResetMicroControler()
* @brief      Reset micro controler
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::ResetMicroControler()
{
  if(!SetModeHeaderPin(DIOAIOBIMAINBOARD_HEADER_SLOT, 9, false)) return false;   // Reset Micro
  if(!SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_SLOT, 9, true)) return false;

  bool pinstatus = false;

  if(SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_SLOT, 9, pinstatus))
    {
      GEN_XSLEEP.MilliSeconds(10);

      pinstatus = !pinstatus;

      return SetStatusHeaderPin(DIOAIOBIMAINBOARD_HEADER_SLOT, 9, pinstatus);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOAIOBIMAINBOARD::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::End()
{
  for(int c=0; c<3; c++)
    {
      if(pcf8574[c])
        {
          pcf8574[c]->End();

          GEN_DELETE pcf8574[c];
          pcf8574[c] = NULL;
        }
    }

  if(eeprom)
    {
      eeprom->End();
      GEN_DELETE eeprom;
      eeprom = NULL;
    }


  if(pca9685)
    {
      pca9685->GetDIOStream()->WaitToFlushXBuffers(3);
      pca9685->End();
      GEN_DELETE pca9685;
      pca9685 = NULL;
    }

  if(gpio)
    {
      SetLedA(false);
      SetLedB(false);

      gpio->End();
      GEN_DIOFACTORY.DeleteGPIO(gpio);
      gpio = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAIOBIMAINBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAIOBIMAINBOARD::Clean()
{
  isini                      = false;

  pcf8574[0]                 = NULL;
  pcf8574[1]                 = NULL;
  pcf8574[2]                 = NULL;

  eeprom                     = NULL;

  gpio                       = NULL;

  pca9685                    = NULL;

  issetp24forexternalbutton  = false;

  outputdigitaldata          = 0;
}



