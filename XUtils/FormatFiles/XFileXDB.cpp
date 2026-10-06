/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileXDB.cpp
* 
* @class      XFILEXDB
* @brief      eXtended Utils Dialog Sentence file class
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

#include "XFileXDB.h"

#include <string.h>

#include "XBuffer.h"
#include "XFactory.h"
#include "XTrace.h"

#include "Hash.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEXDB::XFILEXDB(HASH* hash) : XFILEHASH(hash)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  hash : Hash pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEXDB::XFILEXDB(HASH* hash) : XFILEHASH(hash)
{
  Clean();

  this->hash     = hash;

  Set(XFILE_XDB_ID, XFILEXDB_TYPE, XFILEXDB_VERSION, XFILEXDB_IDSTRING);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEXDB::XFILEXDB(HASH* hash, XPATH& xpath) : XFILEHASH(hash)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  hash : Hash pointer to use.
* @param[in]  xpath : Path to the XDB file.
*
* --------------------------------------------------------------------------------------------------------------------*/
XFILEXDB::XFILEXDB(HASH* hash, XPATH& xpath)
  : XFILEHASH(hash)
{
  Clean();

  this->hash     = hash;

  Set(XFILE_XDB_ID, XFILEXDB_TYPE, XFILEXDB_VERSION, XFILEXDB_IDSTRING);

  SetPath(xpath);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEXDB::~XFILEXDB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEXDB::~XFILEXDB()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEXDB::SetPath(XPATH& xpath)
* @brief      Set path
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEXDB::SetPath(XPATH& xpath)
{
  if(xpath.IsEmpty()) return false;

  this->xpath = xpath;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEXDB::Ini()
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEXDB::Ini()
{
  return OpenFile();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEXDB::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEXDB::End()
{
  return CloseFile();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMAP<XDWORD, XDWORD>* XFILEXDB::GetIndexMap()
* @brief      Get index map
* @ingroup    XUTILS
* 
* @return     XMAP<XDWORD, XDWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMAP<XDWORD, XDWORD>* XFILEXDB::GetIndexMap()
{
  return &indexmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEXDB::OpenFile()
* @brief      Open file
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEXDB::OpenFile()
{
  if(!Open(this->xpath)) return false;

  GetPrimaryFile()->Read((XBYTE*)&nrecords, sizeof(XDWORD));

  if(!nrecords)
    {
      Close();
      return false;
    }

  indexmap.SetIsMulti(true);
  for(int c=0;c<nrecords;c++)
    {
      XDWORD ID     = 0;
      XDWORD filepos  = 0;

      GetPrimaryFile()->Read((XBYTE*)&ID, sizeof(XDWORD));
      GetPrimaryFile()->Read((XBYTE*)&filepos , sizeof(XDWORD));

      //XTRACE_PRINTCOLOR(1,_L("ID: %08X"), ID);

      indexmap.Add(ID, filepos);
    }

  indexmap.SetIsMulti(false);

  isopen=true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILEXDB::GetNumberRecords()
* @brief      Get number records
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILEXDB::GetNumberRecords()
{
  return nrecords;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XFILEXDB::GetRecord(XDWORD ID)
* @brief      Get record
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XFILEXDB::GetRecord(XDWORD ID)
{
  if(!isopen)
    {
      if(!OpenFile()) return NULL;
    }

  if(!nrecords) return NULL;

  XDWORD filepos = GetPosition(ID);
  if(!filepos) return NULL;

  XDWORD size;

  if(!GetPrimaryFile()->SetPosition(filepos)) return NULL;

  if(!GetPrimaryFile()->Read((XBYTE*)&size, sizeof(XDWORD))) return NULL;

  XBUFFER* xbuffer = GEN_NEW XBUFFER();
  if(xbuffer)
    {
      if(size)
        {
          xbuffer->Resize(size);
          xbuffer->FillBuffer();
        }

      if(!GetPrimaryFile()->Read((XBYTE*)xbuffer->Get() , size))
        {
          GEN_DELETE xbuffer;
          return NULL;
        }

      return xbuffer;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XFILEXDB::GetRecordIndex(XDWORD index)
* @brief      Get record index
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XFILEXDB::GetRecordIndex(XDWORD index)
{
  if(!isopen)
    {
      if(!OpenFile()) return NULL;
    }

  if(!nrecords) return NULL;

  XDWORD filepos = indexmap.GetElement(index);
  if(!filepos) return NULL;

  XDWORD size;

  if(!GetPrimaryFile()->SetPosition(filepos)) return NULL;

  if(!GetPrimaryFile()->Read((XBYTE*)&size, sizeof(XDWORD))) return NULL;

  XBUFFER* xbuffer = GEN_NEW XBUFFER();
  if(xbuffer)
    {
      if(size)
        {
          xbuffer->Resize(size);
          xbuffer->FillBuffer();
        }

      if(!GetPrimaryFile()->Read((XBYTE*)xbuffer->Get() , size))
        {
          GEN_DELETE xbuffer;
          return NULL;
        }

      return xbuffer;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILEXDB::GetPosition(XDWORD ID)
* @brief      Get position
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILEXDB::GetPosition(XDWORD ID)
{
  if(!isopen)
    {
      if(!OpenFile()) return 0;
    }

  if(!nrecords) return 0;

  int indexfilepos = indexmap.Find(ID);
  if(indexfilepos==-1) return 0;

  XDWORD filepos = indexmap.GetElement(indexfilepos);

  return filepos;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILEXDB::CloseFile()
* @brief      Close file
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILEXDB::CloseFile()
{
  Close();

  nrecords = 0;

  indexmap.DeleteAll();

  isopen = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILEXDB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILEXDB::Clean()
{
  xpath.Empty();

  isopen        = false;
  nrecords      = 0;

  indexmap.DeleteAll();
}



