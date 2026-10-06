/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_RegisterData.cpp
* 
* @class      DIOCOREPROTOCOL_REGISTERDATA
* @brief      Data Input/Output Core Protocol Register Data class
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

#include "DIOCoreProtocol_RegisterData.h"

#include "XSystem.h"
#include "HashSHA2.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_REGISTERDATA::DIOCOREPROTOCOL_REGISTERDATA()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_REGISTERDATA::DIOCOREPROTOCOL_REGISTERDATA()
{
  Clean();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_REGISTERDATA::~DIOCOREPROTOCOL_REGISTERDATA()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_REGISTERDATA::~DIOCOREPROTOCOL_REGISTERDATA()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_REGISTERDATA::InitializeData(bool isserver)
* @brief      Initialize data
* @ingroup    DATAIO
* 
* @param[in]  isserver : Isserver value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_REGISTERDATA::InitializeData(bool isserver)
{
  if(isserver)
    {
      return false;
    }

  CreateIDMachine(ID_machine);

  GEN_XSYSTEM.GetPlatform(&plataformname);
  GEN_XSYSTEM.GetOperativeSystemID(SOidentifier);

  GEN_XSYSTEM.GetUserAndDomain(user, domain);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID* DIOCOREPROTOCOL_REGISTERDATA::GetIDMmachine()
* @brief      Get ID mmachine
* @ingroup    DATAIO
* 
* @return     XUUID* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID* DIOCOREPROTOCOL_REGISTERDATA::GetIDMmachine()
{
  return &ID_machine;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID* DIOCOREPROTOCOL_REGISTERDATA::GetIDConnection()
* @brief      Get ID connection
* @ingroup    DATAIO
* 
* @return     XUUID* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID* DIOCOREPROTOCOL_REGISTERDATA::GetIDConnection()
{
  return &ID_connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetPlataformName()
* @brief      Get plataform name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetPlataformName()
{
  return &plataformname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetSOIdentifier()
* @brief      Get SO identifier
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetSOIdentifier()
{
  return &SOidentifier;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetUser()
* @brief      Get user
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetUser()
{
  return &user;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetDomain()
* @brief      Get domain
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOCOREPROTOCOL_REGISTERDATA::GetDomain()
{
  return &domain;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_REGISTERDATA::Serialize()
* @brief      Serialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_REGISTERDATA::Serialize()
{
  XSTRING   string; 
  XVARIANT  data;

  string.Empty();
  ID_machine.GetToString(string);
  Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_MACHINE);

  string.Empty();
  ID_connection.GetToString(string);
  Primitive_Add<XSTRING*>(&string, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_CONNECTION);
  
  Primitive_Add<XSTRING*>(&plataformname, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_PLATAFORM);

  Primitive_Add<XSTRING*>(&SOidentifier, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_SOIDENTIFIER);

  Primitive_Add<XSTRING*>(&user, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_USER);

  Primitive_Add<XSTRING*>(&domain, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_DOMAIN);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_REGISTERDATA::Deserialize()
* @brief      Deserialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_REGISTERDATA::Deserialize()
{
  XSTRING string;
  
  string.Empty();
  Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_MACHINE);
  ID_machine.SetFromString(string);

  string.Empty();
  Primitive_Extract<XSTRING>(string, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_ID_CONNECTION);
  ID_connection.SetFromString(string);
  
  Primitive_Extract<XSTRING>(plataformname, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_PLATAFORM); 

  Primitive_Extract<XSTRING>(SOidentifier, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_SOIDENTIFIER);

  Primitive_Extract<XSTRING>(user, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_USER);

  Primitive_Extract<XSTRING>(domain, DIOCOREPROTOCOL_REGISTERDATA_HEADER_VAR_DOMAIN);
       
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_REGISTERDATA::CreateIDMachine(XUUID& ID)
* @brief      Create ID machine
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_REGISTERDATA::CreateIDMachine(XUUID& ID)
{ 
  XSTRING   origin; 
  XBUFFER   originbuffer; 
  HASHSHA2* sha2        = NULL;
  XBUFFER*  result      = NULL;

  ID.Empty();
  
  origin.AddFormat(_L(" %s"), GEN_XSYSTEM.GetBIOSSerialNumber()->Get());
  origin.AddFormat(_L(" %s"), GEN_XSYSTEM.GetCPUSerialNumber()->Get());

  origin.ConvertToUTF8(originbuffer);
    
  sha2 = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
  if(!sha2)
    {
      return false;
    }

  sha2->Do(originbuffer);
  result = sha2->GetResult();

  if(!result)
    {
      return false;
    }

  if(!result->GetSize())
    {
      return false;
    }

  XDWORD data1 = 0; 
  XWORD  data2 = 0;
  XWORD  data3 = 0;
  XBYTE  data4 = 0;
  XBYTE  data5 = 0; 
  XBYTE* data6 = &result->Get()[12];

  result->Get((XDWORD&)data1);  
  result->Get((XWORD&)data2);  
  result->Get((XWORD&)data3);  
  result->Get((XBYTE&)data4);  
  result->Get((XBYTE&)data5);  

  ID.Set(data1, data2, data3, data4, data5, data6);

  ID.GetToString(origin);

  GEN_DELETE sha2;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_REGISTERDATA::ShowDebug()
* @brief      Show debug
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_REGISTERDATA::ShowDebug()
{
  XFILEJSON             classcontent;                                                                                                                                                     
  XSERIALIZATIONMETHOD* serializationmethod = XSERIALIZABLE::CreateInstance(classcontent);
  if(!serializationmethod)
    {
      return false;
    }                                                                
    
  SetSerializationMethod(serializationmethod);
  Serialize(); 

  if(serializationmethod)
    {
      GEN_DELETE serializationmethod;
    }     

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Register data:"));
  classcontent.ShowTraceJSON(XTRACE_COLOR_BLUE);      

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_REGISTERDATA::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_REGISTERDATA::Clean()
{
}




