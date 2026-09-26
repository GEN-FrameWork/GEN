/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOMAC.cpp
* 
* @class      DIOMAC
* @brief      Data Input/Output MAC class
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

#include "DIOMAC.h"

#include <stdio.h>
#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMAC::DIOMAC()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMAC::DIOMAC()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMAC::~DIOMAC()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMAC::~DIOMAC()

{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::IsZero()
* @brief      Is zero
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::IsZero()
{  
  for(int c=0; c<DIOMAC_MAXSIZE; c++)
    {
      if(MAC[c]) return false;
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* DIOMAC::Get()
* @brief      Get value
* @ingroup    DATAIO
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* DIOMAC::Get()
{
  return MAC;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOMAC::GetManufactured()
* @brief      Get manufactured
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOMAC::GetManufactured()
{
  XDWORD manufactured = 0;

  manufactured |= MAC[0];   manufactured <<=8;
  manufactured |= MAC[1];   manufactured <<=8;
  manufactured |= MAC[2];

  return manufactured;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::GetXString(XSTRING& MACstring, XCHAR separator)
* @brief      Get X string
* @ingroup    DATAIO
* 
* @param[in]  MACstring : MA Cstring value.
* @param[in]  separator : Separator value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::GetXString(XSTRING& MACstring, XCHAR separator)
{
  MACstring.Empty();

  MACstring.Format(__L("%02X%c%02X%c%02X%c%02X%c%02X%c%02X"), this->MAC[0], separator, this->MAC[1], separator, this->MAC[2], separator, this->MAC[3], separator, this->MAC[4], separator, this->MAC[5]);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOMAC::GetLongNumber()
* @brief      Get long number
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOMAC::GetLongNumber()
{
  XQWORD number = 0;

  for(int c=0;c<DIOMAC_MAXSIZE;c++)
    {
      number  |= MAC[c];
      number <<= 8;
    }

  number >>= 8;

  return number;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::Set(XBYTE* MAC)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  MAC : MAC pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::Set(XBYTE* MAC)
{
  for(XDWORD c=0; c<DIOMAC_MAXSIZE; c++)
    {
      this->MAC[c] = MAC[c];
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::Set(XSTRING& MAC)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  MAC : MAC value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::Set(XSTRING& MAC)
{
  int number[DIOMAC_MAXSIZE];

  for(int c=0; c<DIOMAC_MAXSIZE; c++)
    {
      number[c] = 0;
    }

  MAC.UnFormat(__L("%02X:%02X:%02X:%02X:%02X:%02X"), &number[0], &number[1], &number[2], &number[3], &number[4], &number[5]);
  for(int c=0; c<DIOMAC_MAXSIZE; c++)
    {
      this->MAC[c] = number[c];
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::Set(char* MAC)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  MAC : MAC pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::Set(char* MAC)
{
  XSTRING _MAC;

  _MAC = MAC;

  return Set(_MAC);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::IsEqual(DIOMAC* MAC)
* @brief      Is equal
* @ingroup    DATAIO
* 
* @param[in]  MAC : MAC pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::IsEqual(DIOMAC* MAC)
{
  if(!MAC) return false;

  for(int c=0; c<DIOMAC_MAXSIZE; c++)
    {
      if(MAC->Get()[c] != Get()[c]) return false;
    }

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMAC::IsEqual(DIOMAC& MAC)
* @brief      Is equal
* @ingroup    DATAIO
* 
* @param[in]  MAC : MAC value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMAC::IsEqual(DIOMAC& MAC)
{
  return IsEqual(&MAC);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOMAC::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOMAC::Clean()
{
  memset(MAC, 0, DIOMAC_MAXSIZE);
}


