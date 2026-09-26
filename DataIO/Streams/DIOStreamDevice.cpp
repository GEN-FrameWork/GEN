/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamDevice.cpp
* 
* @class      DIOSTREAMDEVICE
* @brief      Data Input/Output Stream Device class
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

#include "DIOStreamDevice.h"

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICE::DIOSTREAMDEVICE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICE::DIOSTREAMDEVICE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICE::~DIOSTREAMDEVICE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICE::~DIOSTREAMDEVICE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMDEVICE::GetIndex()
* @brief      Get index
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMDEVICE::GetIndex()
{
  return index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICE::SetIndex(int index)
* @brief      Set index
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICE::SetIndex(int index)
{
  this->index = index;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMDEVICE_TYPE DIOSTREAMDEVICE::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     DIOSTREAMDEVICE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMDEVICE_TYPE DIOSTREAMDEVICE::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICE::SetType(DIOSTREAMDEVICE_TYPE type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICE::SetType(DIOSTREAMDEVICE_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMDEVICE::GetName()
* @brief      Get name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMDEVICE::GetDescription()
* @brief      Get description
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetDescription()
{
  return &description;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMDEVICE::GetResource()
* @brief      Get resource
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetResource()
{
  return &resource;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICE::CopyFrom(DIOSTREAMDEVICE& device)
* @brief      Copy from
* @ingroup    DATAIO
* 
* @param[in]  device : Device value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICE::CopyFrom(DIOSTREAMDEVICE& device)
{
  return device.CopyTo((*this));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICE::CopyTo(DIOSTREAMDEVICE& device)
* @brief      Copy to
* @ingroup    DATAIO
* 
* @param[in]  device : Device value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICE::CopyTo(DIOSTREAMDEVICE& device)
{
  device.SetIndex(index);
  device.GetName()->Set(name);
  device.GetDescription()->Set(description);
  device.GetResource()->Set(resource);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMDEVICE::DebugPrintInfo()
* @brief      Debug print info
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMDEVICE::DebugPrintInfo()
{
  #ifdef XTRACE_ACTIVE
  XSTRING line;

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLACK, __L(" ")                      , NULL);
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLACK, __L("Index            : %02d"), index);
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLACK, __L("Name : %s"), name.Get());
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLACK, __L("Description : %s"), description.Get());
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLACK, __L("Resource : %s"), resource.Get());

  return true;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMDEVICE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMDEVICE::Clean()
{
  index = 0;
  type  = DIOSTREAMDEVICE_TYPE_UNKNOWN;
}



