/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSRegistryManager.cpp
* 
* @class      XWINDOWSREGISTRYMANAGER
* @brief      WINDOWS eXtended Utils Registry Manager class
* @ingroup    PLATFORM_WINDOWS
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



#ifdef WINDOWS


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XWINDOWSRegistryManager.h"

#include "XString.h"
#include "XPath.h"
#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSREGISTRYKEY::XWINDOWSREGISTRYKEY()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSREGISTRYKEY::XWINDOWSREGISTRYKEY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSREGISTRYKEY::~XWINDOWSREGISTRYKEY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSREGISTRYKEY::~XWINDOWSREGISTRYKEY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HKEY XWINDOWSREGISTRYKEY::GetHandle()
* @brief      Get handle
* @ingroup    PLATFORM_WINDOWS
* 
* @return     HKEY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
HKEY XWINDOWSREGISTRYKEY::GetHandle()
{
  return handlekey;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSREGISTRYKEY::SetHandle(HKEY handlekey)
* @brief      Set handle
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSREGISTRYKEY::SetHandle(HKEY handlekey)
{
  this->handlekey = handlekey;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XWINDOWSREGISTRYKEY::GetType()
* @brief      Get type
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XWINDOWSREGISTRYKEY::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSREGISTRYKEY::SetType(XDWORD type)
* @brief      Set type
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSREGISTRYKEY::SetType(XDWORD type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::ReadValue(XCHAR* name, XVARIANT& data)
* @brief      Read value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::ReadValue(XCHAR* name, XVARIANT& data)
{
  if(handlekey == NULL) return false;

  XBUFFER databuffer;
  XDWORD  size = _MAXBUFFER;
  LSTATUS status;

  databuffer.Resize(size);

  status = RegQueryValueEx(handlekey, name, 0, (DWORD*)&type, (LPBYTE)databuffer.Get(), (DWORD*)&size);
  if(status != ERROR_SUCCESS) return false;

  SetType(type);

  switch(type)
    {
      case REG_NONE                         : // No value type
                                              return false;

      case REG_SZ                           : // Unicode nul terminated string
                                              data = (XCHAR*)databuffer.Get();
                                              break;

      case REG_EXPAND_SZ                    : // Unicode nul terminated string (with environment variable references)
                                              break;

      case REG_BINARY                       : // Free form binary
                                              data = databuffer;
                                              break;

      case REG_DWORD                        :  
    //case REG_DWORD_LITTLE_ENDIAN          : // 32-bit number (same as REG_DWORD) 
                                              { XDWORD _data = 0;

                                                databuffer.Get(_data);
                                                SWAPDWORD(_data);
                                                data = _data;
                                              }
                                              break;

      case REG_DWORD_BIG_ENDIAN             : { XDWORD _data = 0;
                                                databuffer.Get(_data);
                                               
                                                data = _data;
                                              }
                                              break;

      case REG_LINK                         : // Symbolic Link (unicode)
                                              break;

      case REG_MULTI_SZ                     : // Multiple Unicode strings
                                              data = databuffer;
                                              break;

      case REG_RESOURCE_LIST                : // Resource list in the resource map
                                              break;

      case REG_FULL_RESOURCE_DESCRIPTOR     : // Resource list in the hardware description
                                              break;

      case REG_RESOURCE_REQUIREMENTS_LIST   : break;

      case REG_QWORD                        :  
    //case REG_QWORD_LITTLE_ENDIAN          : // 64-bit number (same as REG_QWORD)
                                              { XQWORD _data = 0;

                                                databuffer.Get(_data);
                                                SWAPQWORD(_data);
                                                data = _data;
                                              }
                                              break;

                                    default : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::ReadValue(XSTRING& name, XVARIANT& data)
* @brief      Read value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::ReadValue(XSTRING& name, XVARIANT& data)
{
  return ReadValue(name.Get(), data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::WriteValue(XCHAR* name, XDWORD type, XVARIANT& data)
* @brief      Write value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::WriteValue(XCHAR* name, XDWORD type, XVARIANT& data)
{
  if(handlekey == NULL) return false;

  LSTATUS status;

  status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)data.GetData(), (DWORD)data.GetSize());
  if(status != ERROR_SUCCESS) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::WriteValue(XCHAR* name, XVARIANT& data)
* @brief      Write value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::WriteValue(XCHAR* name, XVARIANT& data)
{
  if(handlekey == NULL) return false;

  LSTATUS status = ERROR_SUCCESS;

  switch(data.GetType())
    {    
      case XVARIANT_TYPE_UNKNOWN          : break;

      case XVARIANT_TYPE_MULTIPLE         : return false;

      case XVARIANT_TYPE_SHORT            :
      case XVARIANT_TYPE_WORD             :
      case XVARIANT_TYPE_INTEGER          :
      case XVARIANT_TYPE_DWORD            : type = REG_DWORD;
                                            status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)data.GetData(), (DWORD)data.GetSize());
                                            break;  

      case XVARIANT_TYPE_DOUBLEINTEGER    : 
      case XVARIANT_TYPE_QWORD            : type = REG_QWORD;
                                            status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)data.GetData(), (DWORD)data.GetSize());
                                            break;
      case XVARIANT_TYPE_NULL             :  
      case XVARIANT_TYPE_BOOLEAN          : 
      case XVARIANT_TYPE_FLOAT            :
      case XVARIANT_TYPE_DOUBLE           :   
      case XVARIANT_TYPE_DATE             :
      case XVARIANT_TYPE_TIME             :  
      case XVARIANT_TYPE_DATETIME         : 
      case XVARIANT_TYPE_POINTER          : type = REG_NONE; 
                                            status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)data.GetData(), (DWORD)data.GetSize());
                                            break;

      case XVARIANT_TYPE_CHAR             :
      case XVARIANT_TYPE_XCHAR            :
      case XVARIANT_TYPE_STRING           : { XSTRING* string =  (XSTRING*)data.GetData();
                                              type = REG_SZ;
                                              status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)(string->Get()), (DWORD)string->GetSize()*sizeof(XCHAR));
                                            }
                                            break;
        
      case XVARIANT_TYPE_BUFFER           : { XBUFFER* buffer =  (XBUFFER*)data.GetData();
                                              type = REG_BINARY;
                                              status = RegSetValueEx(handlekey, name, 0, (DWORD)type, (LPBYTE)buffer->Get(), (DWORD)buffer->GetSize());
                                              }
                                            break;           
    }

  
  if(status != ERROR_SUCCESS) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::WriteValue(XSTRING& name, XVARIANT& data)
* @brief      Write value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::WriteValue(XSTRING& name, XVARIANT& data)
{
  type = REG_NONE;

  return WriteValue(name.Get(), data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::WriteValue(XSTRING& name, XDWORD type, XVARIANT& data)
* @brief      Write value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* @param[in]  type : Type value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::WriteValue(XSTRING& name, XDWORD type, XVARIANT& data)
{
  return WriteValue(name.Get(), type, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::EnumValues(XVECTOR<XSTRING*>* valuelist)
* @brief      Enum values
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  valuelist : Valuelist pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::EnumValues(XVECTOR<XSTRING*>* valuelist)
{
  if(GetHandle() == NULL) return false;

  LSTATUS status;
  XSTRING namevalue;
  DWORD   size;
  int     index = 0;

  do{ size  = _MAXPATH;

      namevalue.AdjustSize(size);
      status = RegEnumValue(GetHandle(), index, namevalue.Get(), &size, 0, NULL, NULL, NULL);
      namevalue.AdjustSize();

      if(status == ERROR_SUCCESS)
        {
          XSTRING* _namevalue = GEN_NEW XSTRING();
          if(_namevalue)
            {
              (*_namevalue) = namevalue;
              valuelist->Add(_namevalue);
            }
        }

      index++;

    } while(status != ERROR_NO_MORE_ITEMS);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::DeleteValue(XCHAR* name)
* @brief      Delete value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::DeleteValue(XCHAR* name)
{
  if(handlekey == NULL) return false;

  LSTATUS status = RegDeleteValueW(handlekey, name);
  if(status != ERROR_SUCCESS) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYKEY::DeleteValue(XSTRING& name)
* @brief      Delete value
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYKEY::DeleteValue(XSTRING& name)
{
  return DeleteValue(name.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSREGISTRYKEY::Clear()
* @brief      Clear
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSREGISTRYKEY::Clear()
{
  handlekey = NULL;
  type      = REG_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSREGISTRYKEY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSREGISTRYKEY::Clean()
{
  Clear();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSREGISTRYMANAGER::XWINDOWSREGISTRYMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSREGISTRYMANAGER::XWINDOWSREGISTRYMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSREGISTRYMANAGER::~XWINDOWSREGISTRYMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSREGISTRYMANAGER::~XWINDOWSREGISTRYMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::CreateKey(HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey)
* @brief      Create key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring pointer to use.
* @param[in]  registrykey : Registrykey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::CreateKey(HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey)
{
  if(registrykey.GetHandle() != NULL) return false;

  HKEY              oper_handlekey;
  DWORD             disposition;
  XPATH             subkeystr1;
  XSTRING           subkeystr2;
  XVECTOR<XSTRING*> partkey;
  XSTRING           subkeystr3;
  XDWORD            error;

  subkeystr1  = subkeystring;
  subkeystr1.Slash_Normalize(true);
  subkeystr2 = subkeystr1;

  subkeystr2.Split(_C('\\'), partkey);

  for(XDWORD c=0; c<partkey.GetSize(); c++)
     {
        subkeystr3 += partkey.Get(c)->Get();

        error = RegCreateKeyEx(handlekey, subkeystr3.Get(), 0, NULL, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &oper_handlekey, &disposition);
        if(error != ERROR_SUCCESS)
          {
            RegCloseKey(oper_handlekey);
            break;
          }
         else
          {
            if((partkey.GetSize()-1) != c)
              {
                RegCloseKey(oper_handlekey);
              }
          }

        subkeystr3 +=_C('\\');
     }

  if(error == ERROR_SUCCESS)
    {
      registrykey.SetHandle(oper_handlekey);
    }

  partkey.DeleteContents();
  partkey.DeleteAll();

  return (error == ERROR_SUCCESS)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::CreateKey(HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey)
* @brief      Create key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring value.
* @param[in]  registrykey : Registrykey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::CreateKey(HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey)
{
  return CreateKey(handlekey, subkeystring.Get(), registrykey);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::OpenKey(HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey)
* @brief      Open key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring pointer to use.
* @param[in]  registrykey : Registrykey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::OpenKey(HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey)
{
  if(registrykey.GetHandle() != NULL) return false;

  HKEY    oper_handlekey;
  LSTATUS status;

  status = RegOpenKeyEx(handlekey, subkeystring, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &oper_handlekey);
  if(status != ERROR_SUCCESS)  return false;

  registrykey.SetHandle(oper_handlekey);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::OpenKey(HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey)
* @brief      Open key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring value.
* @param[in]  registrykey : Registrykey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::OpenKey(HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey)
{
  return OpenKey(handlekey, subkeystring.Get(), registrykey);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::EnumKeys(XWINDOWSREGISTRYKEY& registrykey, XVECTOR<XSTRING*>* keylist)
* @brief      Enum keys
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  registrykey : Registrykey value.
* @param[in]  keylist : Keylist pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::EnumKeys(XWINDOWSREGISTRYKEY& registrykey, XVECTOR<XSTRING*>* keylist)
{
  if(registrykey.GetHandle() == NULL) return false;

  LSTATUS status;
  XSTRING namekey;
  DWORD   size;
  int     index = 0;

  do{ size  = _MAXPATH;

      namekey.AdjustSize(size);
      status = RegEnumKeyEx(registrykey.GetHandle(), index, namekey.Get(), &size, 0, NULL, NULL, NULL);
      namekey.AdjustSize();

      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("--- %d"), status);

      if(status == ERROR_SUCCESS)
        {
          XSTRING* _namekey = GEN_NEW XSTRING();
          if(_namekey)
            {
              (*_namekey) = namekey;
              keylist->Add(_namekey);
            }

        } else break;

      index++;

    } while(status != ERROR_NO_MORE_ITEMS);

  return ((status == ERROR_SUCCESS) || (status == ERROR_NO_MORE_ITEMS))?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::CloseKey(XWINDOWSREGISTRYKEY& registrykey)
* @brief      Close key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  registrykey : Registrykey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::CloseKey(XWINDOWSREGISTRYKEY& registrykey)
{
  if(registrykey.GetHandle() == NULL) return false;

  if(RegCloseKey(registrykey.GetHandle()) != ERROR_SUCCESS) return false;

  registrykey.Clear();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::DeleteKey(HKEY handlekey, XCHAR* subkeystring)
* @brief      Delete key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::DeleteKey(HKEY handlekey, XCHAR* subkeystring)
{
  HKEY      operhandlekey = NULL;
  XSTRING   namekey;
  DWORD     size;
  LONG      result;
  FILETIME  ftwrite;

  result = RegDeleteKey(handlekey, subkeystring);
  if(result == ERROR_SUCCESS)  return false;

  result = RegOpenKeyEx(handlekey, subkeystring, 0, KEY_READ, &operhandlekey);
  if(result != ERROR_SUCCESS)
    {
      if(result == ERROR_FILE_NOT_FOUND)
             return true;
        else return false;
    }

  XPATH subkey;

  subkey = subkeystring;
  subkey.Slash_Add();

  size = _MAXPATH;

  namekey.AdjustSize(size);
  result = RegEnumKeyEx(operhandlekey, 0, namekey.Get(), &size, NULL, NULL, NULL, &ftwrite);
  namekey.AdjustSize();

  if(result == ERROR_SUCCESS)
    {
      do{ subkey.Add(namekey);
          if(!DeleteKey(handlekey, namekey))  break;

          size = _MAXPATH;

          namekey.AdjustSize(size);
          result = RegEnumKeyEx(operhandlekey, 0, namekey.Get(), &size, NULL, NULL, NULL, &ftwrite);
          namekey.AdjustSize();

        } while(result == ERROR_SUCCESS);
    }

  RegCloseKey(operhandlekey);

  // Try again to GEN_DELETE the key.
  result = RegDeleteKey(handlekey, subkeystring);

 if(result == ERROR_SUCCESS)  return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSREGISTRYMANAGER::DeleteKey(HKEY handlekey, XSTRING& subkeystring)
* @brief      Delete key
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlekey : Handlekey value.
* @param[in]  subkeystring : Subkeystring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSREGISTRYMANAGER::DeleteKey(HKEY handlekey, XSTRING& subkeystring)
{
  return XWINDOWSREGISTRYMANAGER::DeleteKey(handlekey, subkeystring.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSREGISTRYMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSREGISTRYMANAGER::Clean()
{

}






#endif

