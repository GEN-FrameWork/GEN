/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_FileCSV.cpp
* 
* @class      SCRIPT_LIB_FILECSV
* @brief      Script Library File CSV (read / write)
* @ingroup    SCRIPT
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

#include "Script_Lib_FileCSV.h"

#include "XFactory.h"
#include "XDateTime.h"
#include "XFileCSV.h"
#include "XPath.h"
#include "XTrace.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


static bool ScriptLibFileCSV_VariantToElement(XVARIANT* variant, XSTRING& element)
{
  element.Empty();
  if(!variant) return true;

  // JS (duktape) always passes numbers as double. Never use ToString() for numerics:
  // ConvertFromDouble defaults to "%lf" → "1001.000000".
  switch(variant->GetType())
    {
      case XVARIANT_TYPE_BOOLEAN        :
      case XVARIANT_TYPE_SHORT          :
      case XVARIANT_TYPE_WORD           :
      case XVARIANT_TYPE_INTEGER        :
      case XVARIANT_TYPE_DWORD          :
      case XVARIANT_TYPE_DOUBLEINTEGER  :
      case XVARIANT_TYPE_QWORD          :
      case XVARIANT_TYPE_FLOAT          :
      case XVARIANT_TYPE_DOUBLE         :
      case XVARIANT_TYPE_CHAR           :
      case XVARIANT_TYPE_XCHAR          : element.ConvertFromInt((int)(double)(*variant));
                                          return true;

      case XVARIANT_TYPE_STRING         : {
                                            XSTRING string = (*variant);
                                            element = string.Get();

                                            // If a numeric ID arrived already stringified with decimals, strip them.
                                            int dot = element.Find(__L("."), false, 0);
                                            if(dot != XSTRING_NOTFOUND)
                                              {
                                                bool alldigits = (dot > 0);
                                                for(int i = 0; alldigits && (i < element.GetSize()); i++)
                                                  {
                                                    if(i == dot) continue;
                                                    XCHAR ch = element.Get()[i];
                                                    if((ch < __C('0')) || (ch > __C('9'))) alldigits = false;
                                                  }

                                                if(alldigits)
                                                  {
                                                    XSTRING integerpart;
                                                    element.Copy(0, dot, integerpart);
                                                    element = integerpart;
                                                  }
                                              }
                                          }
                                          return true;

                              default   : variant->ToString(element);
                                          return true;
    }
}


static bool ScriptLibFileCSV_CollectElements(SCRIPT_LIB* library, XVECTOR<XVARIANT*>* params, int startindex, XVECTOR<XSTRING*>* elements)
{
  if(!library)  return false;
  if(!params)   return false;
  if(!elements) return false;

  elements->DeleteContents();
  elements->DeleteAll();

  for(XDWORD c = (XDWORD)startindex; c < params->GetSize(); c++)
    {
      XSTRING* element = GEN_NEW XSTRING();
      if(!element) return false;

      ScriptLibFileCSV_VariantToElement(params->Get(c), (*element));
      elements->Add(element);
    }

  return true;
}


static void ScriptLibFileCSV_DeleteElements(XVECTOR<XSTRING*>* elements)
{
  if(!elements) return;

  elements->DeleteContents();
  elements->DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_FILECSV::SCRIPT_LIB_FILECSV()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_FILECSV::SCRIPT_LIB_FILECSV() : SCRIPT_LIB(SCRIPT_LIB_NAME_FILECSV)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_FILECSV::~SCRIPT_LIB_FILECSV()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_FILECSV::~SCRIPT_LIB_FILECSV()
{
  Close();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("FileCSV_Create")            , Call_FileCSV_Create);
  script->AddLibraryFunction(this, __L("FileCSV_Open")              , Call_FileCSV_Open);
  script->AddLibraryFunction(this, __L("FileCSV_Close")             , Call_FileCSV_Close);
  script->AddLibraryFunction(this, __L("FileCSV_Save")              , Call_FileCSV_Save);
  script->AddLibraryFunction(this, __L("FileCSV_SetSeparator")      , Call_FileCSV_SetSeparator);
  script->AddLibraryFunction(this, __L("FileCSV_SetHeader")         , Call_FileCSV_SetHeader);
  script->AddLibraryFunction(this, __L("FileCSV_AddRecord")         , Call_FileCSV_AddRecord);
  script->AddLibraryFunction(this, __L("FileCSV_GetNRecords")       , Call_FileCSV_GetNRecords);
  script->AddLibraryFunction(this, __L("FileCSV_GetNHeaderElements"), Call_FileCSV_GetNHeaderElements);
  script->AddLibraryFunction(this, __L("FileCSV_GetHeaderElement")  , Call_FileCSV_GetHeaderElement);
  script->AddLibraryFunction(this, __L("FileCSV_GetElement")        , Call_FileCSV_GetElement);
  script->AddLibraryFunction(this, __L("FileCSV_GetPath")           , Call_FileCSV_GetPath);
  script->AddLibraryFunction(this, __L("FileCSV_IsOpen")            , Call_FileCSV_IsOpen);
  script->AddLibraryFunction(this, __L("FileCSV_GetShortDateTime")  , Call_FileCSV_GetShortDateTime);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::Create(XSTRING& path)
* @brief      Create a new CSV file
* @ingroup    SCRIPT
* 
* @param[in]  path : Full path of the CSV file.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::Create(XSTRING& path)
{
  if(path.IsEmpty()) return false;

  Close();

  if(!EnsureInstance()) return false;

  filepath = path.Get();
  haveheaderdeclared = false;

  if(!filecsv->Create(filepath))
    {
      Close();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::Open(XSTRING& path, bool readonly)
* @brief      Open an existing CSV file
* @ingroup    SCRIPT
* 
* @param[in]  path : Full path of the CSV file.
* @param[in]  readonly : Open as read-only when true.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::Open(XSTRING& path, bool readonly)
{
  if(path.IsEmpty()) return false;

  // Preserve a pre-declared header across reopen (XFILECSV::Open needs HaveHeader() true
  // before parsing so the first line is treated as header, not as a data record).
  XVECTOR<XSTRING*> headercolumns;
  bool              restoreheader = false;

  if(filecsv && filecsv->HaveHeader() && filecsv->GetHeader())
    {
      for(int c = 0; c < filecsv->GetHeader()->GetNElements(); c++)
        {
          XSTRING* column = GEN_NEW XSTRING();
          if(!column)
            {
              ScriptLibFileCSV_DeleteElements(&headercolumns);
              return false;
            }

          filecsv->GetHeader()->GetElement(c, (*column));
          headercolumns.Add(column);
        }

      restoreheader = !headercolumns.IsEmpty();
    }

  Close();

  if(!EnsureInstance())
    {
      ScriptLibFileCSV_DeleteElements(&headercolumns);
      return false;
    }

  if(restoreheader)
    {
      if(!ApplyHeader(&headercolumns))
        {
          ScriptLibFileCSV_DeleteElements(&headercolumns);
          Close();
          return false;
        }

      haveheaderdeclared = true;
    }

  ScriptLibFileCSV_DeleteElements(&headercolumns);

  filepath = path.Get();

  if(!filecsv->Open(filepath, readonly))
    {
      Close();
      return false;
    }

  haveheaderdeclared = filecsv->HaveHeader();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::Close()
* @brief      Close current CSV session
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::Close()
{
  bool status = true;

  if(filecsv)
    {
      if(filecsv->IsOpen())
        {
          status = filecsv->Close();
        }

      GEN_DELETE filecsv;
      filecsv = NULL;
    }

  filepath.Empty();
  haveheaderdeclared = false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::Save()
* @brief      Persist in-memory CSV to disk
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::Save()
{
  return FlushToDisk();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::SetSeparator(XSTRING& separator)
* @brief      Set CSV field separator ("," or ";")
* @ingroup    SCRIPT
* 
* @param[in]  separator : Separator string (first character used).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::SetSeparator(XSTRING& separator)
{
  if(!EnsureInstance())      return false;
  if(separator.IsEmpty())    return false;

  return filecsv->SetSeparator(separator.Get()[0]);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::SetHeader(XVECTOR<XSTRING*>* elements)
* @brief      Set CSV header columns
* @ingroup    SCRIPT
* 
* @param[in]  elements : Column names.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::SetHeader(XVECTOR<XSTRING*>* elements)
{
  if(!EnsureInstance()) return false;
  if(!ApplyHeader(elements)) return false;

  haveheaderdeclared = true;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::AddRecord(XVECTOR<XSTRING*>* elements)
* @brief      Append one CSV record and save
* @ingroup    SCRIPT
* 
* @param[in]  elements : Field values.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::AddRecord(XVECTOR<XSTRING*>* elements)
{
  if(!filecsv)            return false;
  if(filepath.IsEmpty())  return false;
  if(!elements)           return false;
  if(elements->IsEmpty()) return false;

  XFILECSV_RECORD* record = GEN_NEW XFILECSV_RECORD();
  if(!record) return false;

  for(XDWORD c = 0; c < elements->GetSize(); c++)
    {
      XSTRING* element = elements->Get(c);
      if(!element)
        {
          GEN_DELETE record;
          return false;
        }

      record->AddElement(element->Get());
    }

  if(!filecsv->AddRecord(record))
    {
      GEN_DELETE record;
      return false;
    }

  return FlushToDisk();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SCRIPT_LIB_FILECSV::GetNRecords()
* @brief      Get number of data records
* @ingroup    SCRIPT
* 
* @return     int : Number of records.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SCRIPT_LIB_FILECSV::GetNRecords()
{
  if(!filecsv) return 0;

  return filecsv->GetNRecords();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int SCRIPT_LIB_FILECSV::GetNHeaderElements()
* @brief      Get number of header columns
* @ingroup    SCRIPT
* 
* @return     int : Number of header elements.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int SCRIPT_LIB_FILECSV::GetNHeaderElements()
{
  if(!filecsv) return 0;
  if(!filecsv->GetHeader()) return 0;

  return filecsv->GetHeader()->GetNElements();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::GetHeaderElement(int index, XSTRING& element)
* @brief      Get one header column
* @ingroup    SCRIPT
* 
* @param[in]  index : Column index.
* @param[out] element : Column name.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::GetHeaderElement(int index, XSTRING& element)
{
  element.Empty();

  if(!filecsv) return false;
  if(!filecsv->GetHeader()) return false;

  return filecsv->GetHeader()->GetElement(index, element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::GetElement(int recordindex, int elementindex, XSTRING& element)
* @brief      Get one field from a record
* @ingroup    SCRIPT
* 
* @param[in]  recordindex : Record index.
* @param[in]  elementindex : Field index.
* @param[out] element : Field value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::GetElement(int recordindex, int elementindex, XSTRING& element)
{
  element.Empty();

  if(!filecsv) return false;

  XFILECSV_RECORD* record = filecsv->ReadRecord((XDWORD)recordindex);
  if(!record) return false;

  return record->GetElement(elementindex, element);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* SCRIPT_LIB_FILECSV::GetFilePath()
* @brief      Get current CSV path
* @ingroup    SCRIPT
* 
* @return     XCHAR* : Path string.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* SCRIPT_LIB_FILECSV::GetFilePath()
{
  return filepath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::IsOpen()
* @brief      Is CSV file currently open
* @ingroup    SCRIPT
* 
* @return     bool : true if open.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::IsOpen()
{
  if(!filecsv) return false;

  return filecsv->IsOpen();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::GetShortDateTime(XSTRING& stamp)
* @brief      Get local short datetime stamp YYYYMMDD_HHMMSS
* @ingroup    SCRIPT
* 
* @param[out] stamp : Output stamp.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::GetShortDateTime(XSTRING& stamp)
{
  stamp.Empty();

  XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
  if(!datetime) return false;

  datetime->Read(true);
  stamp.Format(__L("%04d%02d%02d_%02d%02d%02d")
             , datetime->GetYear()
             , datetime->GetMonth()
             , datetime->GetDay()
             , datetime->GetHours()
             , datetime->GetMinutes()
             , datetime->GetSeconds());

  GEN_XFACTORY.DeleteDateTime(datetime);

  return !stamp.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::EnsureInstance()
* @brief      Create XFILECSV instance if needed
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::EnsureInstance()
{
  if(filecsv) return true;

  filecsv = GEN_NEW XFILECSV();
  return (filecsv != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::ApplyHeader(XVECTOR<XSTRING*>* elements)
* @brief      Apply header columns to current CSV
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* @param[in]  elements : Column names.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::ApplyHeader(XVECTOR<XSTRING*>* elements)
{
  if(!filecsv)            return false;
  if(!elements)           return false;
  if(elements->IsEmpty()) return false;

  filecsv->GetHeader()->DeleteAllElements();

  XFILECSV_RECORD header;

  for(XDWORD c = 0; c < elements->GetSize(); c++)
    {
      XSTRING* element = elements->Get(c);
      if(!element) return false;

      header.AddElement(element->Get());
    }

  return filecsv->SetHeader(&header);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_FILECSV::FlushToDisk()
* @brief      Rewrite CSV from in-memory state
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_FILECSV::FlushToDisk()
{
  if(!filecsv)            return false;
  if(filepath.IsEmpty())  return false;

  if(filecsv->IsOpen())
    {
      if(!filecsv->Close()) return false;
    }

  // Recreate + write full in-memory header/records (avoids Open header-append quirk).
  if(!filecsv->Create(filepath)) return false;

  return filecsv->Close();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_FILECSV::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_FILECSV::Clean()
{
  filecsv              = NULL;
  haveheaderdeclared   = false;
  filepath.Empty();
}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_Create(...)
* @brief      FileCSV_Create(path) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_Create(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  XSTRING             path;
  bool                status = false;

  library->GetParamConverted(params->Get(0), path);
  status = lib->Create(path);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_Open(...)
* @brief      FileCSV_Open(path [, readonly]) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_Open(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib      = (SCRIPT_LIB_FILECSV*)library;
  XSTRING             path;
  bool                readonly = true;
  bool                status   = false;

  library->GetParamConverted(params->Get(0), path);

  if(params->GetSize() >= 2)
    {
      library->GetParamConverted(params->Get(1), readonly);
    }

  status = lib->Open(path, readonly);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_Close(...)
* @brief      FileCSV_Close() → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_Close(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;

  (*returnvalue) = lib->Close();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_Save(...)
* @brief      FileCSV_Save() → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_Save(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;

  (*returnvalue) = lib->Save();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_SetSeparator(...)
* @brief      FileCSV_SetSeparator(sep) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_SetSeparator(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  XSTRING             separator;

  library->GetParamConverted(params->Get(0), separator);

  (*returnvalue) = lib->SetSeparator(separator);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_SetHeader(...)
* @brief      FileCSV_SetHeader(col1, col2, ...) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_SetHeader(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  XVECTOR<XSTRING*>   elements;
  bool                status = false;

  if(ScriptLibFileCSV_CollectElements(library, params, 0, &elements))
    {
      status = lib->SetHeader(&elements);
    }

  ScriptLibFileCSV_DeleteElements(&elements);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_AddRecord(...)
* @brief      FileCSV_AddRecord(field1, field2, ...) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_AddRecord(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  XVECTOR<XSTRING*>   elements;
  bool                status = false;

  if(ScriptLibFileCSV_CollectElements(library, params, 0, &elements))
    {
      status = lib->AddRecord(&elements);
    }

  ScriptLibFileCSV_DeleteElements(&elements);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetNRecords(...)
* @brief      FileCSV_GetNRecords() → int
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetNRecords(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;

  (*returnvalue) = lib->GetNRecords();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetNHeaderElements(...)
* @brief      FileCSV_GetNHeaderElements() → int
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetNHeaderElements(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;

  (*returnvalue) = lib->GetNHeaderElements();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetHeaderElement(...)
* @brief      FileCSV_GetHeaderElement(index) → string
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetHeaderElement(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  int                 index = 0;
  XSTRING             element;

  library->GetParamConverted(params->Get(0), index);

  if(lib->GetHeaderElement(index, element))
    {
      (*returnvalue) = element.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetElement(...)
* @brief      FileCSV_GetElement(record, index) → string
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetElement(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;
  int                 recordindex  = 0;
  int                 elementindex = 0;
  XSTRING             element;

  library->GetParamConverted(params->Get(0), recordindex);
  library->GetParamConverted(params->Get(1), elementindex);

  if(lib->GetElement(recordindex, elementindex, element))
    {
      (*returnvalue) = element.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetPath(...)
* @brief      FileCSV_GetPath() → string
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetPath(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib  = (SCRIPT_LIB_FILECSV*)library;
  XCHAR*              path = lib->GetFilePath();

  if(path) (*returnvalue) = path;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_IsOpen(...)
* @brief      FileCSV_IsOpen() → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_IsOpen(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  SCRIPT_LIB_FILECSV* lib = (SCRIPT_LIB_FILECSV*)library;

  (*returnvalue) = lib->IsOpen();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_FileCSV_GetShortDateTime(...)
* @brief      FileCSV_GetShortDateTime() → string (YYYYMMDD_HHMMSS)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_FileCSV_GetShortDateTime(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING stamp;

  if(SCRIPT_LIB_FILECSV::GetShortDateTime(stamp))
    {
      (*returnvalue) = stamp.Get();
    }
}

