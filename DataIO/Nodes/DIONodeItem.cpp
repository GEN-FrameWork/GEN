/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItem.cpp
* 
* @class      DIONODEITEM
* @brief      Data Input/Output Node item (Base for DIONODESENSOR, DIONODEACTUATOR, ...) class
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

#include "DIONodeItem.h"

#include "XFactory.h"

#include "DIONodeItemHandler.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEM::DIONODEITEM()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEM::DIONODEITEM()
{
  Clean();

  UUID.GenerateRandom();

  updatetimer = GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEM::DIONODEITEM(DIONODEITEMHANDLER* itemhandler)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  itemhandler : Itemhandler pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEM::DIONODEITEM(DIONODEITEMHANDLER* itemhandler)
{
  Clean();

  UUID.GenerateRandom();

  updatetimer = GEN_XFACTORY.CreateTimer();

  ItemHandler_Set(itemhandler);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEM::~DIONODEITEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEM::~DIONODEITEM()

{
  ItemHandler_Close();  

  values.DeleteContents();
  values.DeleteAll();    

  if(updatetimer)
    {
      GEN_XFACTORY.DeleteTimer(updatetimer);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIONODEITEM::GetCategory()
* @brief      Get category
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIONODEITEM::GetCategory()
{
  return category;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEM::SetCategory(XDWORD category)
* @brief      Set category
* @ingroup    DATAIO
* 
* @param[in]  category : Category value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEM::SetCategory(XDWORD category)
{
  this->category = category;

  switch(category)
    {
      case DIONODEITEM_CATEGORY_UNKNOWN                    : description = _L("unknown");                          break;
      case DIONODEITEM_CATEGORY_SENSORHUMIDITYTEMPERATURE  : description = _L("Sensor Humidity/Temperature");      break;
      case DIONODEITEM_CATEGORY_GPIO_DIGITAL               : description = _L("GPIO Digital");                     break; 
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIONODEITEM::GetDescription()
* @brief      Get description
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIONODEITEM::GetDescription()
{
  return &description;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID& DIONODEITEM::GetID()
* @brief      Get ID
* @ingroup    DATAIO
* 
* @return     XUUID& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID& DIONODEITEM::GetID()
{
  return UUID;
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEM::SetID(XUUID& UUID)
* @brief      Set ID
* @ingroup    DATAIO
* 
* @param[in]  UUID : UUID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEM::SetID(XUUID& UUID)
{
  UUID.CopyTo(this->UUID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEMHANDLER* DIONODEITEM::ItemHandler_Get()
* @brief      Item handler get
* @ingroup    DATAIO
* 
* @return     DIONODEITEMHANDLER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEMHANDLER* DIONODEITEM::ItemHandler_Get()
{
  return itemhandler;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::ItemHandler_Set(DIONODEITEMHANDLER* itemhandler)
* @brief      Item handler set
* @ingroup    DATAIO
* 
* @param[in]  itemhandler : Itemhandler pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::ItemHandler_Set(DIONODEITEMHANDLER* itemhandler)
{
  this->itemhandler = itemhandler;

  if(!itemhandler->SetNodeItem(this))
    { 
      return false;
    }

  if(!ItemHandler_Open())       
    {
      return false;      
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::ItemHandler_Open()
* @brief      Item handler open
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::ItemHandler_Open()
{
  if(!itemhandler)               
    {
      return false;  
    }
  
  if(itemhandler->IsOpen())      
    {
      itemhandler->Close();      
    }

  if(!itemhandler->Open())       
    {
      return false;
    }

  if(!itemhandler->IsWorking())  
    {
      return false;      
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::ItemHandler_Close()
* @brief      Item handler close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::ItemHandler_Close()
{
  if(!itemhandler)              
    {
      return false;  
    }

  if(!itemhandler->Close())     
    {
      return false;
    }

  if(itemhandler->IsWorking())  
    {
      return false;      
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::IsSimulate()
* @brief      Is simulate
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::IsSimulate()
{
  return issimulated;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEM::SetIsSimulate(bool issimulated)
* @brief      Set is simulate
* @ingroup    DATAIO
* 
* @param[in]  issimulated : Issimulated value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEM::SetIsSimulate(bool issimulated)
{
  this->issimulated = issimulated;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIONODEITEM::GetTimeToUpdate()
* @brief      Get time to update
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIONODEITEM::GetTimeToUpdate()
{
  return timetoupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEM::SetTimeToUpdate(XQWORD timetoupdate)
* @brief      Set time to update
* @ingroup    DATAIO
* 
* @param[in]  timetoupdate : Timetoupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEM::SetTimeToUpdate(XQWORD timetoupdate)
{
  this->timetoupdate = timetoupdate;

  if(updatetimer)
    {
      updatetimer->Reset();
      updatetimer->AddMilliSeconds(timetoupdate);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIONODEITEM::GetUpdateTimer()
* @brief      Get update timer
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIONODEITEM::GetUpdateTimer()
{
  return updatetimer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIONODEITEMVALUE*>* DIONODEITEM::GetValues()
* @brief      Get values
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIONODEITEMVALUE*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIONODEITEMVALUE*>* DIONODEITEM::GetValues()
{
  return &values;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::Update()
{
  if(!itemhandler)              
    {
      return false;  
    }

  bool status = false;  
  
  if(updatetimer)
    {
      if(updatetimer->GetMeasureMilliSeconds() > timetoupdate)  
        {
          status = itemhandler->Update();
          updatetimer->Reset();
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::Serialize()
* @brief      Serialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::Serialize()
{
  XSTRING ID;
  
  UUID.GetToString(ID); 
  
  Primitive_Add<XSTRING*>(&ID, _L("ID"));
  Primitive_Add<int>(category, _L("category"));
  Primitive_Add<XSTRING*>(&description, _L("description"));
  Primitive_Add<bool>(issimulated, _L("simulated"));
  Primitive_Add<XQWORD>(timetoupdate, _L("timetoupdate"));

  DIONODEITEMHANDLER* itemhandler =  ItemHandler_Get();
  if(itemhandler)
    {
      Primitive_Add<int>(itemhandler->GetType(), _L("handler_type"));
      Primitive_Add<XSTRING*>(itemhandler->GetName(), _L("handler_name"));
    }

  XVectorClass_Add<DIONODEITEMVALUE>(&values, _L("values"), _L("value"));  
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEM::Deserialize()
* @brief      Deserialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEM::Deserialize()
{   
  XSTRING ID;

  Primitive_Extract<XSTRING>(ID            , _L("ID"));
  Primitive_Extract<XDWORD>(category       , _L("category"));
  Primitive_Extract<XSTRING>(description   , _L("description"));
  Primitive_Extract<bool>(issimulated      , _L("simulated"));
  Primitive_Extract<XQWORD>(timetoupdate   , _L("timetoupdate"));

  DIONODEITEMHANDLER* itemhandler =  ItemHandler_Get();
  if(itemhandler)
    {
      XDWORD type;

      Primitive_Extract<XDWORD>(type, _L("handler_type"));
      itemhandler->SetType(type);


      Primitive_Extract<XSTRING>((*itemhandler->GetName()), _L("handler_name"));
    }

  XVectorClass_Extract<DIONODEITEMVALUE>(&values, _L("values"), _L("value"));  
  
  UUID.SetFromString(ID);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEM::Clean()
{
  category      = DIONODEITEM_CATEGORY_UNKNOWN;

  itemhandler   = NULL;

  issimulated   = false;

  timetoupdate  = 0;
  updatetimer   = NULL;
}



