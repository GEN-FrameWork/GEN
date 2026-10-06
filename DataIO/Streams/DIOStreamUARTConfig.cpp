/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamUARTConfig.cpp
* 
* @class      DIOSTREAMUARTCONFIG
* @brief      Data Input/Output Stream UART config class
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

#include "DIOStreamUARTConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTCONFIG::DIOSTREAMUARTCONFIG(): DIOSTREAMCONFIG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTCONFIG::DIOSTREAMUARTCONFIG(): DIOSTREAMCONFIG()
{
  Clean();

  type = DIOSTREAMTYPE_UART;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTCONFIG::~DIOSTREAMUARTCONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTCONFIG::~DIOSTREAMUARTCONFIG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMUARTCONFIG::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMUARTCONFIG::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetPort(int port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetPort(int port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMUARTCONFIG::GetLocalDeviceName()
* @brief      Get local device name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMUARTCONFIG::GetLocalDeviceName()
{
  return &devicename;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMUARTCONFIG::GetBaudRate()
* @brief      Get baud rate
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMUARTCONFIG::GetBaudRate()
{
  return baudrate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetBaudRate(int baudrate)
* @brief      Set baud rate
* @ingroup    DATAIO
* 
* @param[in]  baudrate : Baudrate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetBaudRate(int baudrate)
{
  this->baudrate = baudrate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTDATABITS DIOSTREAMUARTCONFIG::GetDataBits()
* @brief      Get data bits
* @ingroup    DATAIO
* 
* @return     DIOSTREAMUARTDATABITS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTDATABITS DIOSTREAMUARTCONFIG::GetDataBits()
{
  return databits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetDataBits(DIOSTREAMUARTDATABITS databits)
* @brief      Set data bits
* @ingroup    DATAIO
* 
* @param[in]  databits : Databits value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetDataBits(DIOSTREAMUARTDATABITS databits)
{
  this->databits = databits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTPARITY DIOSTREAMUARTCONFIG::GetParity()
* @brief      Get parity
* @ingroup    DATAIO
* 
* @return     DIOSTREAMUARTPARITY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTPARITY DIOSTREAMUARTCONFIG::GetParity()
{
  return parity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetParity(DIOSTREAMUARTPARITY parity)
* @brief      Set parity
* @ingroup    DATAIO
* 
* @param[in]  parity : Parity value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetParity(DIOSTREAMUARTPARITY parity)
{
  this->parity = parity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTSTOPBITS DIOSTREAMUARTCONFIG::GetStopBits()
* @brief      Get stop bits
* @ingroup    DATAIO
* 
* @return     DIOSTREAMUARTSTOPBITS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTSTOPBITS DIOSTREAMUARTCONFIG::GetStopBits()
{
  return stopbits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetStopBits(DIOSTREAMUARTSTOPBITS stopbits)
* @brief      Set stop bits
* @ingroup    DATAIO
* 
* @param[in]  stopbits : Stopbits value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetStopBits(DIOSTREAMUARTSTOPBITS stopbits)
{
  this->stopbits = stopbits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMUARTFLOWCONTROL DIOSTREAMUARTCONFIG::GetFlowControl()
* @brief      Get flow control
* @ingroup    DATAIO
* 
* @return     DIOSTREAMUARTFLOWCONTROL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMUARTFLOWCONTROL DIOSTREAMUARTCONFIG::GetFlowControl()
{
  return flowcontrol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::SetFlowControl(DIOSTREAMUARTFLOWCONTROL flowcontrol)
* @brief      Set flow control
* @ingroup    DATAIO
* 
* @param[in]  flowcontrol : Flowcontrol value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::SetFlowControl(DIOSTREAMUARTFLOWCONTROL flowcontrol)
{
  this->flowcontrol = flowcontrol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUARTCONFIG::GetToString(XSTRING* string)
* @brief      Get to string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::GetToString(XSTRING* string)
{
  if(!string) return false;

  XSTRING xstringtmp;

  string->Empty();

  if(!GetLocalDeviceName()->IsEmpty())
    {
      string->Add(GetLocalDeviceName()->Get());
      string->Add(_L(","));
    }
   else
    {
      if(!GetPort()) return false;
      string->Format(_L("%d,"), GetPort());
    }

  if(!GetBaudRate()) return false;
  xstringtmp.Format(_L("%d,"), GetBaudRate());
  string->Add(xstringtmp);

  switch(GetDataBits())
    {
      case DIOSTREAMUARTDATABIT_5       : string->Add(_L("5,"));     break;
      case DIOSTREAMUARTDATABIT_6       : string->Add(_L("6,"));     break;
      case DIOSTREAMUARTDATABIT_7       : string->Add(_L("7,"));     break;
      case DIOSTREAMUARTDATABIT_8       : string->Add(_L("8,"));     break;
                        default         : return false;
    }

  switch(GetParity())
    {
      case DIOSTREAMUARTPARITY_NONE     : string->Add(_L("N,"));   break;
      case DIOSTREAMUARTPARITY_ODD      : string->Add(_L("O,"));   break;
      case DIOSTREAMUARTPARITY_EVEN     : string->Add(_L("E,"));   break;
      case DIOSTREAMUARTPARITY_MARK     : string->Add(_L("M,"));   break;
      case DIOSTREAMUARTPARITY_SPACE    : string->Add(_L("S,"));   break;
                             default    : return false;
    }

  switch(GetFlowControl())
    {
      case DIOSTREAMUARTFLOWCONTROL_NONE : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_NONE);   break;
      case DIOSTREAMUARTFLOWCONTROL_HARD : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_SOFT);   break;
      case DIOSTREAMUARTFLOWCONTROL_SOFT : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_HARD);   break;
                                default  : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUARTCONFIG::GetToString(XSTRING& string)
* @brief      Get to string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::GetToString(XSTRING& string)
{
  return GetToString(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUARTCONFIG::SetFromString(XCHAR* string)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::SetFromString(XCHAR* string)
{
  if(!string) return false;

  XSTRING   _string  = string;
  XSTRING   devicename;
  int       baudrate;
  int       databits;
  XCHAR     parity;
  XSTRING   stopbits;
  XSTRING   flowcontrol;

  devicename.AdjustSize(_MAXSTR);
  stopbits.AdjustSize(_MAXSTR);
  flowcontrol.AdjustSize(_MAXSTR);

  _string.UnFormat(_L("%s,%d,%d,%c,%s,%s"), devicename.Get(), &baudrate, &databits, &parity, stopbits.Get(), flowcontrol.Get());

  devicename.AdjustSize();
  stopbits.AdjustSize();
  flowcontrol.AdjustSize();


  if(devicename.HaveOnlyNumbers())
    {
      SetPort(devicename.ConvertToInt());
      if(!GetPort()) return false;
    }
   else GetLocalDeviceName()->Set(devicename);

  if(!baudrate) return false;
  SetBaudRate(baudrate);

  switch(databits)
    {
      case 5    : SetDataBits(DIOSTREAMUARTDATABIT_5);  break;
      case 6    : SetDataBits(DIOSTREAMUARTDATABIT_6);  break;
      case 7    : SetDataBits(DIOSTREAMUARTDATABIT_7);  break;
      case 8    : SetDataBits(DIOSTREAMUARTDATABIT_8);  break;
        default : return false;
    }

  switch(parity)
    {
      case 'N'  : SetParity(DIOSTREAMUARTPARITY_NONE);  break;
      case 'O'  : SetParity(DIOSTREAMUARTPARITY_ODD);   break;
      case 'E'  : SetParity(DIOSTREAMUARTPARITY_EVEN);  break;
      case 'M'  : SetParity(DIOSTREAMUARTPARITY_MARK);  break;
      case 'S'  : SetParity(DIOSTREAMUARTPARITY_SPACE); break;
       default  : return false;
    }

  if(!stopbits.Compare(_L("1")))
    {
      SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
    }
   else
    {
      if(!stopbits.Compare(_L("1.5")))
        {
          SetStopBits(DIOSTREAMUARTSTOPBITS_ONEANDAHALF);
        }
       else
        {
          if(!stopbits.Compare(_L("2")))
            {
              SetStopBits(DIOSTREAMUARTSTOPBITS_TWO);

            } else return false;
        }
    }

  if(!flowcontrol.Compare(DIOSTREAMUART_FLOWCONTROL_STR_NONE, true))
    {
      SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);
    }
   else
    {
      if(!stopbits.Compare(DIOSTREAMUART_FLOWCONTROL_STR_SOFT) || !stopbits.Compare(_L("SOFT")))
        {
          SetFlowControl(DIOSTREAMUARTFLOWCONTROL_SOFT);
        }
       else
        {
          if(!stopbits.Compare(DIOSTREAMUART_FLOWCONTROL_STR_HARD) || !stopbits.Compare(_L("HARD")))
            {
              SetFlowControl(DIOSTREAMUARTFLOWCONTROL_HARD);

            } else return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUARTCONFIG::SetFromString(XSTRING* string)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::SetFromString(XSTRING* string)
{
  if(!string) return false;
  return SetFromString(string->Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMUARTCONFIG::SetFromString(XSTRING& string)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::SetFromString(XSTRING& string)
{
  return SetFromString(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMUARTCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMUARTCONFIG::Clean()
{
  type          = DIOSTREAMTYPE_UART;

  port          = 0;
  baudrate      = 0;
  databits      = DIOSTREAMUARTDATABIT_8;
  parity        = DIOSTREAMUARTPARITY_NONE;
  stopbits      = DIOSTREAMUARTSTOPBITS_ONE;
  flowcontrol   = DIOSTREAMUARTFLOWCONTROL_NONE;
}



