/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOBusPirate.cpp
* 
* @class      DIOBUSPIRATE
* @brief      Data Input/Output interface Bus Pirate 4.0
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

#include "DIOBusPirate.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamConfig.h"
#include "DIOStream.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOBUSPIRATE::DIOBUSPIRATE(DIOSTREAM* diostream)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOBUSPIRATE::DIOBUSPIRATE(DIOSTREAM* diostream)
{
  Clean();

  this->diostream     = diostream;

  xtimerout=GEN_XFACTORY.CreateTimer();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOBUSPIRATE::~DIOBUSPIRATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOBUSPIRATE::~DIOBUSPIRATE()
{
  End();

  if(xtimerout) GEN_XFACTORY.DeleteTimer(xtimerout);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::Ini(int timeout)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::Ini(int timeout)
{
  if(!diostream)                           return false;

  if(!diostream->Open())                   return false;
  if(!diostream->WaitToConnected(timeout)) return false;

  if(!WriteCommand(_L(""), timeout))      return false;
  if(!WriteCommand(_L("m"), timeout))     return false;
  if(!WriteCommand(_L("1") , timeout))    return false;

  GEN_XSLEEP.Seconds(1);
  diostream->GetInXBuffer()->Empty();

  return (GetActualProtocol() == DIOBUSPIRATETYPEPROCOTOL_HIZ);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOBUSPIRATETYPEPROCOTOL DIOBUSPIRATE::GetActualProtocol(int timeout)
* @brief      Get actual protocol
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     DIOBUSPIRATETYPEPROCOTOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOBUSPIRATETYPEPROCOTOL DIOBUSPIRATE::GetActualProtocol(int timeout)
{
  if(!diostream)                                                 return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;
  if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED) return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;

  XSTRING promptstatus;

  WriteCommand(_L(""), timeout);
  ReadPromptStatus(promptstatus, timeout);

  if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_HIZ, false) != XSTRING_NOTFOUND)        return DIOBUSPIRATETYPEPROCOTOL_HIZ;
    else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_1WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_1WIRE;
      else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_UART, false) != XSTRING_NOTFOUND) return DIOBUSPIRATETYPEPROCOTOL_UART;
        else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_I2C, false) != XSTRING_NOTFOUND)    return DIOBUSPIRATETYPEPROCOTOL_I2C;
          else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_SPI, false) != XSTRING_NOTFOUND)    return DIOBUSPIRATETYPEPROCOTOL_SPI;
            else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_2WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_2WIRE;
              else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_3WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_3WIRE;

  return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::SetMode(DIOBUSPIRATETYPEPROCOTOL protocol, bool issoftware, int speed, bool issniffer, int timeout)
* @brief      Set mode
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol value.
* @param[in]  issoftware : Issoftware value.
* @param[in]  speed : Speed value.
* @param[in]  issniffer : Issniffer value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::SetMode(DIOBUSPIRATETYPEPROCOTOL protocol, bool issoftware, int speed, bool issniffer, int timeout)
{
  if(!xtimerout)                                                 return false;
  if(!diostream)                                                 return false;
  if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  XSTRING option;
  bool    status = false;

  switch(protocol)
    {
      case DIOBUSPIRATETYPEPROCOTOL_I2C : if(WriteCommand(_L("m"), timeout))
                                            {
                                              option.Format(_L("%d"), protocol);
                                              if(WriteCommand(option, timeout))
                                                {
                                                  option.Format(_L("%d"), issoftware?1:2);
                                                  if(WriteCommand(option, timeout))
                                                    {
                                                      option.Format(_L("%d"), speed);
                                                      if(WriteCommand(option, timeout))
                                                        {
                                                          if(issniffer)
                                                            {
                                                              option.Format(_L("(2)"), speed);
                                                              status = WriteCommand(option, timeout);

                                                            } else status = true;
                                                        }
                                                     }
                                                  }
                                              }

                                            GEN_XSLEEP.Seconds(1);
                                            diostream->GetInXBuffer()->Empty();
                                            break;

                              default   :   break;

    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::ReadI2CSnifferCommand(XBUFFER& in, XBUFFER& out)
* @brief      Read I2C sniffer command
* @ingroup    DATAIO
* 
* @param[in]  in : In value.
* @param[in]  out : Output out.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::ReadI2CSnifferCommand(XBUFFER& in, XBUFFER& out)
{
  XBUFFER xbuffer;
  bool    foundend    = false;
  int     stringsize  = 0;

  diostream->GetInXBuffer()->SetBlocked(true);
  for(int c=0;c<(int)diostream->GetInXBuffer()->GetSize();c++)
    {
      if(diostream->GetInXBuffer()->GetByte(c) == ']')
        {
          stringsize++;
          foundend = true;
          break;

        } else stringsize++;
    }
  diostream->GetInXBuffer()->SetBlocked(false);

  if(!foundend) return false;

  XSTRING string;

  XBYTE* buffer = GEN_NEW XBYTE[stringsize+1];
  if(!buffer) return false;

  memset(buffer, 0, stringsize+1);

  int br = diostream->Read((XBYTE*)buffer, stringsize);
  if(br != stringsize) return false;

  string.Empty();
  string = (char*)buffer;

  GEN_DELETE_ARRAY buffer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::End()
{
  if(diostream) diostream->Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::ReadPromptStatus(XSTRING& promptstatus, int timeout)
* @brief      Read prompt status
* @ingroup    DATAIO
* 
* @param[in]  promptstatus : Promptstatus value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::ReadPromptStatus(XSTRING& promptstatus, int timeout)
{
  if(!diostream)                                                 return false;
  if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  promptstatus.Empty();

  if(!xtimerout) return false;

  xtimerout->Reset();

  while(1)
    {
      if(diostream->WaitToFilledReadingBuffer(1, 1))
        {
          XBYTE data;
          if(diostream->Read(&data, 1))
            {
              if(data != '>')
                {
                  promptstatus.Add(data);
                }
               else return true;

              xtimerout->Reset();
            }
        }

      if((int)xtimerout->GetMeasureSeconds() >= timeout) break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOBUSPIRATE::WriteCommand(XCHAR* command, int timeout)
* @brief      Write command
* @ingroup    DATAIO
* 
* @param[in]  command : Command pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOBUSPIRATE::WriteCommand(XCHAR* command, int timeout)
{
  if(!diostream)                                                 return false;
  if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  XSTRING _command;
  bool    status;

  _command  = command;
  _command += _L("\r");
  
  XBUFFER charstr;
  
  _command.ConvertToASCII(charstr);
  status = diostream->Write((XBYTE*)charstr.Get(), _command.GetSize())?true:false;
  if(status)  status = diostream->WaitToFlushOutXBuffer(timeout);
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOBUSPIRATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOBUSPIRATE::Clean()
{
  diostream   = NULL;
  xtimerout   = NULL;
}



