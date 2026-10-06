/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XUUID.cpp
* 
* @class      XUUID
* @brief      eXtended Utils UUID (Universally Unique IDentifier)
* @ingroup    XUTILS
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

#include "XUUID.h"

#include "XFactory.h"
#include "XDateTime.h"
#include "XRand.h"
#include "HashCRC32.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID::XUUID()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID::XUUID()
{
  Clean();

  xrand = GEN_XFACTORY.CreateRand();
  if(xrand)
    {
      xrand->Ini();
    }    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID::~XUUID()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID::~XUUID()
{
  if(xrand)
    {
      GEN_XFACTORY.DeleteRand(xrand);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XUUID::GetData1()
* @brief      Get data1
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XUUID::GetData1()
{
  return data1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD XUUID::GetData2()
* @brief      Get data2
* @ingroup    XUTILS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD XUUID::GetData2()
{
  return data2;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD XUUID::GetData3()
* @brief      Get data3
* @ingroup    XUTILS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD XUUID::GetData3()
{
  return data3;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XUUID::GetData4()
* @brief      Get data4
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XUUID::GetData4()
{
  return data4;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XUUID::GetData5()
* @brief      Get data4
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XUUID::GetData5()
{
  return data5;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* XUUID::GetData6()
* @brief      Get data6
* @ingroup    XUTILS
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* XUUID::GetData6()
{
  return data6;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::GetToBuffer(XBUFFER& data)
* @brief      get to buffer
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::GetToBuffer(XBUFFER& data)
{
  data.Empty();

  data.Add((XDWORD)GetData1());
  data.Add((XWORD)GetData2());
  data.Add((XWORD)GetData3());
  data.Add((XBYTE)GetData4());
  data.Add((XBYTE)GetData5());
  data.Add((XBYTE*)GetData6(), XUUIDMAXDATA4);

  return data.IsEmpty()?false:true;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::GetToString(XSTRING& string)
* @brief      Get to string
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::GetToString(XSTRING& string)
{
  string.Format(_L("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"), data1, data2, data3, data4, data5, data6[0], data6[1], data6[2], data6[3], data6[4], data6[5]);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData1(XDWORD data)
* @brief      Set data1
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData1(XDWORD data)
{
  this->data1 = data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData2(XWORD data)
* @brief      Set data2
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData2(XWORD data)
{
  this->data2 = data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData3(XWORD data)
* @brief      Set data3
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData3(XWORD data)
{
  this->data3 = data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData4(XBYTE data)
* @brief      Set data4
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData4(XBYTE data)
{
  this->data4 = data;
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData5(XBYTE data)
* @brief      Set data5
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData5(XBYTE data)
{
  this->data5 = data;
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetData6(XBYTE* data)
* @brief      Set data6
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetData6(XBYTE* data)
{
  for(int c=0; c<XUUIDMAXDATA4; c++)
    {
      this->data6[c] = data[c];
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetFromBuffer(XBUFFER& data)
* @brief      set from buffer
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetFromBuffer(XBUFFER& data)
{ 
  XDWORD data1  = 0;
  XWORD  data2  = 0;
  XWORD  data3  = 0;
  XBYTE  data4  = 0;
  XBYTE  data5  = 0;
  XBYTE  data6[XUUIDMAXDATA4];

  memset(data6, 0, XUUIDMAXDATA4);

  data.Get((XDWORD&)data1);
  data.Get((XWORD&)data2);
  data.Get((XWORD&)data3);
  data.Get((XBYTE&)data4);
  data.Get((XBYTE&)data5);
  data.Get(data6, XUUIDMAXDATA4);

  return Set(data1, data2, data3, data4, data5, data6);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::SetFromString(XSTRING& string)
* @brief      Set from string
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::SetFromString(XSTRING& string)
{
  XSTRING string2;  
  XDWORD  _data[6] = { 0, 0, 0, 0, 0, 0 };

  string2 = string;

  if(string2.IsEmpty())
    {
      return false;
    }

  string2.Insert(_L("-"), 28);

  string2.UnFormat(_L("%08X-%04X-%04X-%04X-%04X-%08X"), &_data[0], &_data[1], &_data[2], &_data[3], &_data[4], &_data[5]);
  data1     = (XDWORD)_data[0];
  data2     = (XWORD)_data[1];
  data3     = (XWORD)_data[2];  

  data4     = (XBYTE)(_data[3] >> 8);
  data5     = (XBYTE)(_data[3] & 0x00FF);

  data6[0]  = (XBYTE)(_data[4] >> 8);
  data6[1]  = (XBYTE)(_data[4] & 0x00FF);
  
  data6[2]  = (XBYTE)((_data[5] >> 24) & 0x000000FF);
  data6[3]  = (XBYTE)((_data[5] >> 16) & 0x000000FF);
  data6[4]  = (XBYTE)((_data[5] >>  8) & 0x000000FF);
  data6[5]  = (XBYTE)(_data[5] & 0x000000FF);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::IsEmpty()
* @brief      Is empty
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::IsEmpty()
{
  if(data1)
    {
      return false;
    }

  if(data2)
    {
      return false;
    }

  if(data3)
    {
      return false;
    }

  if(data4)
    {
      return false;
    }

  if(data5)
    {
      return false;
    }

  for(int c=0; c<XUUIDMAXDATA4; c++)
    {
      if(data6[c])
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::Empty()
* @brief      Empty
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::Empty()
{
  data1 = 0;
  data2 = 0;
  data3 = 0;
  data4 = 0;
  data5 = 0;

  for(int c=0; c<XUUIDMAXDATA4; c++)
    {
      this->data6[c] = 0;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::Set(XUUID& ID)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::Set(XUUID& ID)
{
  SetData1(ID.GetData1());
  SetData2(ID.GetData2());
  SetData3(ID.GetData3());
  SetData4(ID.GetData4());
  SetData5(ID.GetData5());
  SetData6(ID.GetData6());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::Set(XDWORD data1, XWORD data2, XWORD data3, XBYTE data4, XBYTE data5, XBYTE* data6)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data1 : Data1 value.
* @param[in]  data2 : Data2 value.
* @param[in]  data3 : Data3 value.
* @param[in]  data4 : Data4 value.
* @param[in]  data5 : Data5 value.
* @param[in]  data6 : Data6 pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::Set(XDWORD data1, XWORD data2, XWORD data3, XBYTE data4, XBYTE data5, XBYTE* data6)
{
  SetData1(data1);
  SetData2(data2);
  SetData3(data3);
  SetData4(data4);
  SetData5(data5);
  SetData6(data6);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::CopyFrom(XUUID& uuid)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  uuid : Uuid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::CopyFrom(XUUID& uuid)
{
  return Set(uuid.GetData1(), uuid.GetData2(), uuid.GetData3(), uuid.GetData4(), uuid.GetData5(), uuid.GetData6());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::CopyTo(XUUID& uuid)
* @brief      Copy to
* @ingroup    XUTILS
* 
* @param[in]  uuid : Uuid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::CopyTo(XUUID& uuid)
{
  return uuid.Set(data1, data2, data3, data4, data5, data6);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::GenerateRandom()
* @brief      Generate random
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::GenerateRandom()
{
  
  if(!xrand) 
    { 
      return false;
    }
      
  XDATETIME* datetime=GEN_XFACTORY.CreateDateTime();
  if(!datetime)
    {
      return false;
    }

  XSTRING     datetimestr;
  HASHCRC32   hashcrc32;

  datetime->Read();
  datetime->GetDateTimeToString((XDATETIME_FORMAT_YMD | XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS | XDATETIME_FORMAT_TIMEWITHMILLISECONDS), datetimestr);

  hashcrc32.Do((XBYTE*)datetimestr.Get(), (datetimestr.GetSize() * sizeof(XCHAR)));

  data1 = hashcrc32.GetResultCRC32();
             
  if(xrand->Between(0, 1)) 
    {
      data1 |= 0x80000000;
    }
   else
    {
      data1 &= 0x7FFFFFFF;
    }
          
  data2 = (XWORD)xrand->Between(0, 65535);
  data3 = (XWORD)xrand->Between(0, 65535);

  data4 = (XBYTE)xrand->Between(0, 255);
  data5 = (XBYTE)xrand->Between(0, 255);

  for(int c=0; c<XUUIDMAXDATA4; c++)
    {
      data6[c] = (XBYTE)xrand->Max(255);
    }

  GEN_XFACTORY.DeleteDateTime(datetime);
       
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XUUID::Compare(XUUID& uuid)
* @brief      Compare
* @ingroup    XUTILS
* 
* @param[in]  uuid : Uuid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XUUID::Compare(XUUID& uuid)
{  
  if(data1 != uuid.GetData1()) 
    {
      return false;
    }

  if(data2 != uuid.GetData2()) 
    {
      return false;
    }

  if(data3 != uuid.GetData3()) 
    {
      return false;
    }

  if(data4 != uuid.GetData4()) 
    {
      return false;
    }

  if(data5 != uuid.GetData5()) 
    {
      return false;
    }

  for(int c=0; c<XUUIDMAXDATA4; c++)
    {
      if(data6[c] != uuid.GetData6()[c]) 
        {
          return false;
        }      
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XUUID::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XUUID::Clean()
{
  xrand     = NULL;

  Empty();
}



