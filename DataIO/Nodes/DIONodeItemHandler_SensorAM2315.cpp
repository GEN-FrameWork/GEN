/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItemHandler_SensorAM2315.cpp
* 
* @class      DIONODEITEMHANDLER_SENSORAM2315
* @brief      Data Input/Output Node Item Handler Sensor AM2315
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include <cmath>

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIONode_XEvent.h"

#include "DIONodeItemHandler_SensorAM2315.h"

#include "XMemory_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEMHANDLER_SENSORAM2315::DIONODEITEMHANDLER_SENSORAM2315(int port, int remoteitemaddress, int timeout)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* @param[in]  remoteitemaddress : Remoteitemaddress value.
* @param[in]  timeout : Timeout value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEMHANDLER_SENSORAM2315::DIONODEITEMHANDLER_SENSORAM2315(int port, int remoteitemaddress, int timeout)
{
  Clean();

  am2315 = GEN_NEW DIOI2CTEMHUMSENSORAM2315();

  this->port                = port; 
  this->remoteitemaddress   = remoteitemaddress; 
  this->timeout             = timeout;   

  type                      = DIONODEITEMHANDLER_TYPE_SENSOR_AM2315;  
  name                      = _L("AOSONG AM2315");
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEITEMHANDLER_SENSORAM2315::~DIONODEITEMHANDLER_SENSORAM2315()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEITEMHANDLER_SENSORAM2315::~DIONODEITEMHANDLER_SENSORAM2315()
{
  Close();

  if(am2315)
    {
      GEN_DELETE am2315;  
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMHANDLER_SENSORAM2315::Open()
* @brief      Open
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMHANDLER_SENSORAM2315::Open()
{
  if(!am2315)
    {
      return false;
    }
    
  #ifndef WINDOWS
  isopen = am2315->Ini(port, remoteitemaddress, timeout);  
  #else
  isopen = true;
  #endif
  if(isopen)
    {
      isworking = true;
    }

  return isopen;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMHANDLER_SENSORAM2315::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMHANDLER_SENSORAM2315::Update()
{
  float value[2] = { 0.0f, 0.0f };
  
  isworking = am2315->Read(value[0], value[1]);

  DIONODEITEM* nodeitem = GetNodeItem();
  if(!nodeitem)
    {
      return false;
    }

  if(nodeitem->GetValues()->GetSize() != 2) 
    {
      return false;
    }

  DIONODEITEMVALUE* nodeitemvalue;

  for(XDWORD c=0; c<2; c++)
    {
      nodeitemvalue = nodeitem->GetValues()->Get(c);
      if(nodeitemvalue)
        {
          float      differencevalue = 0.0f;
          XVARIANT*  nodeitemdifferencevalue = nodeitemvalue->GetDifferenceForChange();
 
          if(nodeitemdifferencevalue)
            {
              if(!nodeitemdifferencevalue->IsNull())
                {                  
                   differencevalue = (float)(*nodeitemdifferencevalue);    
                   differencevalue = (float)fabs(differencevalue);
                }
            }

          float difference = (value[c] - (float)(*nodeitemvalue->GetValue()));

          if(fabs(difference) > differencevalue)
            {
              XSTRING description;

              nodeitemvalue->GetDescription(description);
              (*nodeitemvalue->GetValue()) = value[c];

              nodeitemvalue->ReadUpdateDateTime();
              nodeitemvalue->SetValueHasChanged(true); 


              DIONODE_XEVENT xevent(this, DIONODE_XEVENT_TYPE_UPDATEVALUE); 
                  
              xevent.SetNodeItem(nodeitem);
              xevent.SetIndexValue(c);
             
              PostEvent(&xevent);
              
              
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[DIONODE item handler %s] %s %s [value %f]"), GetName()->Get(), nodeitem->GetDescription()->Get(), description.Get(), value[c]);  
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMHANDLER_SENSORAM2315::Close()
* @brief      Close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMHANDLER_SENSORAM2315::Close()
{
  am2315->End();
        
  isopen = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEITEMHANDLER_SENSORAM2315::SetNodeItem(DIONODEITEM* nodeitem)
* @brief      Set node item
* @ingroup    DATAIO
* 
* @param[in]  nodeitem : Nodeitem pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEITEMHANDLER_SENSORAM2315::SetNodeItem(DIONODEITEM* nodeitem)
{
  if(!nodeitem) 
    {
      return false;
    }

  if(!DIONODEITEMHANDLER::SetNodeItem(nodeitem)) 
    {
      return false;
    }

  nodeitem->SetCategory(DIONODEITEM_CATEGORY_SENSORHUMIDITYTEMPERATURE);   
 
  for(XDWORD c=0; c<2; c++)
    {
      DIONODEITEMVALUE* value = GEN_NEW DIONODEITEMVALUE();
      if(value) 
        {
          switch(c)
            {
              case  0 : value->SetType(DIONODEITEMVALUE_TYPE_TEMPERATURE); 
                        value->SetMode(DIONODEITEMVALUE_MODE_READ);   

                        (*value->GetValue())    =    0.00f;
                        (*value->GetMinValue()) =  -50.00f;
                        (*value->GetMaxValue()) =   50.00f;

                        value->GetUnitFormat()->SetType(DIONODEITEMVALUE_UNITSFORMAT_TYPE_CELSIUSDEGREE);
                        break;

              case  1 : value->SetType(DIONODEITEMVALUE_TYPE_HUMIDITY); 
                        value->SetMode(DIONODEITEMVALUE_MODE_READ);   

                        (*value->GetValue())    =   0.00f; 
                        (*value->GetMinValue()) =   0.00f; 
                        (*value->GetMaxValue()) = 100.00f;   

                        value->GetUnitFormat()->SetType(DIONODEITEMVALUE_UNITSFORMAT_TYPE_RELATIVEHUMIDITY);
                        break;
            }

          nodeitem->GetValues()->Add(value);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEITEMHANDLER_SENSORAM2315::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEITEMHANDLER_SENSORAM2315::Clean()
{
  am2315              = NULL;

  port                = 0;
  remoteitemaddress   = 0;
  timeout             = 0;
}
