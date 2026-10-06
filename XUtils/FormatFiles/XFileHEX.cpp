/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileHEX.cpp
* 
* @class      XFILEHEX
* @brief      eXtended Utils HEX file class (hex ASCII format)
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

#include "XFileHEX.h"

#include <stdio.h>
#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX_ENTRY::XFILEHEX_ENTRY()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX_ENTRY::XFILEHEX_ENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX_ENTRY::~XFILEHEX_ENTRY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX_ENTRY::~XFILEHEX_ENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XFILEHEX_ENTRY::GetSizeData()
* @brief      Get size data
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XFILEHEX_ENTRY::GetSizeData()
{
  return sizedata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEHEX_ENTRY::SetSizeData(XBYTE sizedata)
* @brief      Set size data
* @ingroup    XUTILS
* 
* @param[in]  sizedata : Sizedata value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEHEX_ENTRY::SetSizeData(XBYTE sizedata)
{
  this->sizedata = sizedata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD XFILEHEX_ENTRY::GetAddress()
* @brief      Get address
* @ingroup    XUTILS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD XFILEHEX_ENTRY::GetAddress()
{
  return address;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEHEX_ENTRY::SetAddress(XWORD address)
* @brief      Set address
* @ingroup    XUTILS
* 
* @param[in]  address : Address value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEHEX_ENTRY::SetAddress(XWORD address)
{
  this->address = address;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX_ENTRYTYPE XFILEHEX_ENTRY::GetEntryType()
* @brief      Get entry type
* @ingroup    XUTILS
* 
* @return     XFILEHEX_ENTRYTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX_ENTRYTYPE XFILEHEX_ENTRY::GetEntryType()
{
  return (XFILEHEX_ENTRYTYPE)entrytype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEHEX_ENTRY::SetEntryType(XFILEHEX_ENTRYTYPE entrytype)
* @brief      Set entry type
* @ingroup    XUTILS
* 
* @param[in]  entrytype : Entrytype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEHEX_ENTRY::SetEntryType(XFILEHEX_ENTRYTYPE entrytype)
{
  this->entrytype = entrytype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XFILEHEX_ENTRY::GetData()
* @brief      Get data
* @ingroup    XUTILS
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XFILEHEX_ENTRY::GetData()
{
  return &data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XFILEHEX_ENTRY::CalculeCRC(XBUFFER& data)
* @brief      Calcule CRC
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XFILEHEX_ENTRY::CalculeCRC(XBUFFER& data)
{
  XDWORD CRC = 0;

  for(XDWORD c=0; c<data.GetSize(); c++)
    {
      CRC += data.Get()[c];
    }

  XBYTE _CRC = CRC;

  return (XBYTE)~_CRC + 1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEHEX_ENTRY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEHEX_ENTRY::Clean()
{
  sizedata  = 0;
  address   = 0;
  entrytype = XFILEHEX_ENTRYTYPE_NODEFINED;

  data.Empty();
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX::XFILEHEX(): XFILETXT()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX::XFILEHEX(): XFILETXT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX::~XFILEHEX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX::~XFILEHEX()
{
  Entry_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::DecodeAllLines()
* @brief      Decode all lines
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::DecodeAllLines()
{
  if(!IsOpen())      return false;
  if(!ReadAllFile()) return false;

  bool status = false;

  for(int c=0; c<GetNLines(); c++)
    {
      XSTRING* line = GetLine(c);
      if(line)
        {
          if(!line->IsEmpty())
            {
              status = Entry_Add(line);
              if(!status)
                {
                  break;
                }
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::EncodeAllLines()
* @brief      Encode all lines
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::EncodeAllLines()
{
  if(!IsOpen())      return false;

  DeleteAllLines();

  for(XDWORD c=0; c<entrys.GetSize(); c++)
    {
      XFILEHEX_ENTRY* entry = entrys.Get(c);
      if(entry)
        {
          XSTRING line;

           if(!EncodeLine(entry, line)) return false;

           AddLine(line);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::DecodeLine(XSTRING* line, XFILEHEX_ENTRY& entry)
* @brief      Decode line
* @ingroup    XUTILS
* 
* @param[in]  line : Line pointer to use.
* @param[in]  entry : Entry value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::DecodeLine(XSTRING* line, XFILEHEX_ENTRY& entry)
{
  if(!line)            return false;
  if(line->IsEmpty())  return false;

  if(line->Get()[0] !=_C(':')) return false;

  int     sizedata;
  int     address;
  int     entrytype;
  XSTRING string;
  int     CRC;
  XBUFFER dataCRC;

  line->ToUpperCase();

  line->Copy( 1, 3, string);
  string.UnFormat(_L("%02X"), &sizedata);
  entry.SetSizeData((XBYTE)sizedata);

  line->Copy( 3, 7, string);
  string.UnFormat(_L("%04X"), &address);
  entry.SetAddress((XWORD)address);

  line->Copy( 7, 9, string);
  string.UnFormat(_L("%02X"), &entrytype);
  entry.SetEntryType((XFILEHEX_ENTRYTYPE)entrytype);

  if(sizedata)
    {
      line->Copy(9, line->GetSize()-2, string);
      string.ConvertHexStringToBuffer((*entry.GetData()));
    }

  line->Copy(line->GetSize()-2, string);
  string.UnFormat(_L("%02X"), &CRC);

  line->Copy(1, line->GetSize()-2, string);
  string.ConvertHexStringToBuffer(dataCRC);

  XBYTE calculeCRC = entry.CalculeCRC(dataCRC);

  if((XBYTE)CRC != calculeCRC)  return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::EncodeLine(XFILEHEX_ENTRY* entry, XSTRING& line)
* @brief      Encode line
* @ingroup    XUTILS
* 
* @param[in]  entry : Entry pointer to use.
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::EncodeLine(XFILEHEX_ENTRY* entry, XSTRING& line)
{
  if(!entry)  return false;

  XSTRING string;

  string.Format(_L("%02X"), entry->GetSizeData());                    line.Add(string);
  string.Format(_L("%04X"), entry->GetAddress());                     line.Add(string);
  string.Format(_L("%02X"), (XWORD)entry->GetEntryType());            line.Add(string);

  if(entry->GetSizeData())
    {
      string.ConvertHexStringFromBuffer((*entry->GetData()), true);
      line.Add(string);
    }

  XBUFFER dataCRC;

  line.ConvertHexStringToBuffer(dataCRC);
  string.Format(_L("%02X"), entry->CalculeCRC(dataCRC));  line.Add(string);

  line.Insert(_L(":"), 0);
  line.ToUpperCase();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILEHEX::GetNEntrys()
* @brief      Get N entrys
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILEHEX::GetNEntrys()
{
  return entrys.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::Entry_Add(XSTRING* line)
* @brief      Entry add
* @ingroup    XUTILS
* 
* @param[in]  line : Line pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::Entry_Add(XSTRING* line)
{
  if(!line)             return false;
  if(line->IsEmpty())   return false;

  XFILEHEX_ENTRY* entry = GEN_NEW XFILEHEX_ENTRY();
  if(!entry) return false;

  if(!DecodeLine(line, (*entry)))
    {
      GEN_DELETE entry;
      return false;
    }

  entrys.Add(entry);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX_ENTRY* XFILEHEX::Entry_Get(XWORD address, XFILEHEX_ENTRYTYPE type)
* @brief      Entry get
* @ingroup    XUTILS
* 
* @param[in]  address : Address value.
* @param[in]  type : Type value.
* 
* @return     XFILEHEX_ENTRY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX_ENTRY* XFILEHEX::Entry_Get(XWORD address, XFILEHEX_ENTRYTYPE type)
{
  if(entrys.IsEmpty()) return NULL;

  for(XDWORD c=0; c<entrys.GetSize(); c++)
    {
      XFILEHEX_ENTRY* entry = entrys.Get(c);
      if(entry)
        {
          if((entry->GetAddress() == address) && (entry->GetEntryType() == type))
            {
              return entry;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEHEX_ENTRY* XFILEHEX::Entry_Get(int index)
* @brief      Entry get
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     XFILEHEX_ENTRY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEHEX_ENTRY* XFILEHEX::Entry_Get(int index)
{
  return entrys.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::Entry_Delete(XWORD address, XFILEHEX_ENTRYTYPE type)
* @brief      Entry delete
* @ingroup    XUTILS
* 
* @param[in]  address : Address value.
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::Entry_Delete(XWORD address, XFILEHEX_ENTRYTYPE type)
{
  XFILEHEX_ENTRY* entry = Entry_Get(address, type);
  if(!entry) return false;

  if(!entrys.Delete(entry)) return false;

  GEN_DELETE entry;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::Entry_Delete(int index)
* @brief      Entry delete
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::Entry_Delete(int index)
{
  XFILEHEX_ENTRY* entry = Entry_Get(index);
  if(!entry) return false;

  if(!entrys.Delete(entry)) return false;

  GEN_DELETE entry;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEHEX::Entry_DeleteAll()
* @brief      Entry GEN_DELETE all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEHEX::Entry_DeleteAll()
{
  if(entrys.IsEmpty())  return false;

  entrys.DeleteContents();
  entrys.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEHEX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEHEX::Clean()
{

}





