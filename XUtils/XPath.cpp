/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XPath.cpp
* 
* @class      XPATH
* @brief      eXtended Utils Path class
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

#include "XPath.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XDir.h"
#include "XString.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(): XSTRING()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(): XSTRING()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(XDWORD size): XSTRING(size)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(XDWORD size): XSTRING(size)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(const char* path) : XSTRING(path)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(const char* path) : XSTRING(path)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(const XCHAR* path): XSTRING(path)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(const XCHAR* path): XSTRING(path)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(const XCHAR* path, XDWORD size): XSTRING(path,size)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(const XCHAR* path, XDWORD size): XSTRING(path, size)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(XPATH& path): XSTRING(path)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(XPATH& path): XSTRING(path)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(XSTRING& string) : XSTRING(string)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(XSTRING& string)  : XSTRING(string)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::XPATH(XWORD* path) : XSTRING(path)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::XPATH(XWORD* path) : XSTRING(path)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH::~XPATH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH::~XPATH()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator = (const char* path)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator = (const char* path)
{
  Set(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator = (const XCHAR* path)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator = (const XCHAR* path)
{
  Set(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator = (XPATH& path)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator = (XPATH& path)
{
  Set(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator = (XSTRING& string)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator = (XSTRING& string)
{
  Set(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator = (XWORD* path)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator = (XWORD* path)
{
  Set(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator += (const char* path)
* @brief      operator +=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator += (const char* path)
{
  Add(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator += (const XCHAR* path)
* @brief      operator +=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator += (const XCHAR* path)
{
  Add(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator += (XPATH& path)
* @brief      operator +=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator += (XPATH& path)
{
  Add(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator += (XSTRING& string)
* @brief      operator +=
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator += (XSTRING& string)
{
  Add(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::operator += (XCHAR path)
* @brief      operator +=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::operator += (XCHAR path)
{
  Add(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator < (XPATH& path)
* @brief      operator <
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator <  (XPATH& path)
{
  return (bool)(Compare(path) == -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator > (XPATH& path)
* @brief      operator >
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator >  (XPATH& path)
{
  return (bool)(Compare(path) ==  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator <= (XPATH& path)
* @brief      operator <=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator <= (XPATH& path)
{
  return (bool)(Compare(path) !=  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator >= (XPATH& path)
* @brief      operator >=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator >= (XPATH& path)
{
  return (bool)(Compare(path) != -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator == (XPATH& path)
* @brief      operator ==
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator == (XPATH& path)
{
  return (bool)(Compare(path) ==  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator != (XPATH& path)
* @brief      operator !=
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator != (XPATH& path)
{
  return (bool)(Compare(path) !=  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator < (XSTRING& string)
* @brief      operator <
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator <  (XSTRING& string)
{
  return (bool)(Compare(string) == -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator > (XSTRING& string)
* @brief      operator >
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator >  (XSTRING& string)
{
  return (bool)(Compare(string) ==  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator <= (XSTRING& string)
* @brief      operator <=
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator <= (XSTRING& string)
{
  return (bool)(Compare(string) !=  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator >= (XSTRING& string)
* @brief      operator >=
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator >= (XSTRING& string)
{
  return (bool)(Compare(string) != -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator == (XSTRING& string)
* @brief      operator ==
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator == (XSTRING& string)
{
  return (bool)(Compare(string) ==  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::operator != (XSTRING& string)
* @brief      operator !=
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::operator != (XSTRING& string)
{
  return (bool)(Compare(string) !=  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR XPATH::operator [](int position)
* @brief      operator []
* @ingroup    XUTILS
* 
* @param[in]  position : Position value.
* 
* @return     XCHAR XPATH:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR XPATH::operator [](int position)
{
  if(IsEmpty())           return 0;

  if(position<0)          return text[0];
  if(position>=(int)size) return text[size-1];

  return text[position];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetDrive(XSTRING& drive)
* @brief      Get drive
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetDrive(XSTRING& drive)
{
  Split(&drive, NULL, NULL, NULL);

  if(drive.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetPath(XSTRING& path)
* @brief      Get path
* @ingroup    XUTILS
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetPath(XSTRING& path)
{
  XPATH xpath;

  Split(NULL, &xpath, NULL, NULL);

  if(xpath.IsEmpty()) return false;

  xpath.Slash_Add();

  path = xpath;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetDriveAndPath(XSTRING& drivepath)
* @brief      Get drive and path
* @ingroup    XUTILS
* 
* @param[in]  drivepath : Drivepath value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetDriveAndPath(XSTRING& drivepath)
{
  XSTRING drive;
  XPATH   path;

  Split(&drive, &path, NULL, NULL);

  drivepath  = drive;
  drivepath += path;

  if(drivepath.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetPathAndNamefile(XSTRING& pathnamefile)
* @brief      Get path and namefile
* @ingroup    XUTILS
* 
* @param[in]  pathnamefile : Pathnamefile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetPathAndNamefile(XSTRING& pathnamefile)
{
  XPATH      path;
  XSTRING name;

  Split(NULL, &path, &name, NULL);

  path.Slash_Add();

  pathnamefile  = path;
  pathnamefile += name;

  if(pathnamefile.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetPathAndNamefileExt(XSTRING& pathnamefile)
* @brief      Get path and namefile ext
* @ingroup    XUTILS
* 
* @param[in]  pathnamefile : Pathnamefile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetPathAndNamefileExt(XSTRING& pathnamefile)
{
  XPATH   path;
  XSTRING name;
  XSTRING ext;

  Split(NULL, &path, &name, &ext);

  path.Slash_Add();

  pathnamefile  = path;
  pathnamefile += name;
  pathnamefile += ext;

  if(pathnamefile.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetNamefile(XSTRING& namefile)
* @brief      Get namefile
* @ingroup    XUTILS
* 
* @param[in]  namefile : Namefile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetNamefile(XSTRING& namefile)
{
  Split(NULL, NULL, &namefile, NULL);

  if(namefile.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetNamefileExt(XSTRING& namefileext)
* @brief      Get namefile ext
* @ingroup    XUTILS
* 
* @param[in]  namefileext : Namefileext value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetNamefileExt(XSTRING& namefileext)
{
  XSTRING name;
  XSTRING ext;

  Split(NULL, NULL, &name, &ext);

  namefileext  = name;
  namefileext += ext;

  if(namefileext.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetExt(XSTRING& ext)
* @brief      Get ext
* @ingroup    XUTILS
* 
* @param[in]  ext : Ext value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetExt(XSTRING& ext)
{
  Split(NULL, NULL, NULL, &ext);

  if(ext.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::GetPathInSequence(int index, XSTRING& pathpart)
* @brief      Get path in sequence
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  pathpart : Pathpart value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::GetPathInSequence(int index, XSTRING& pathpart)
{
  pathpart.Empty();

  int   slashcount = 0;
  XDWORD c           = 0;

  while(c<GetSize())
    {
      if((Get()[c]=='/') || (Get()[c]=='\\'))
        {
          if(slashcount==index)
            {
              return true;
            }
           else
            {
              pathpart.Empty();
              slashcount++;
            }

        } else pathpart.Add(Get()[c]);

      c++;
    }

  pathpart.Empty();

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyDrive()
* @brief      Set only drive
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyDrive()
{
  XSTRING string;

  string.Empty();

  GetDrive(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyPath()
* @brief      Set only path
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyPath()
{
  XSTRING string;

  string.Empty();

  GetPath(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyDriveAndPath()
* @brief      Set only drive and path
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyDriveAndPath()
{
  XSTRING string;

  string.Empty();

  GetDriveAndPath(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyPathAndNamefile()
* @brief      Set only path and namefile
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyPathAndNamefile()
{
  XSTRING string;

  string.Empty();

  GetPathAndNamefile(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyPathAndNamefileExt()
* @brief      Set only path and namefile ext
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyPathAndNamefileExt()
{
  XSTRING string;

  string.Empty();

  GetPathAndNamefileExt(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyNamefile()
* @brief      Set only namefile
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyNamefile()
{
  XSTRING string;

  string.Empty();

  GetNamefile(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyNamefileExt()
* @brief      Set only namefile ext
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyNamefileExt()
{
  XSTRING string;

  string.Empty();

  GetNamefileExt(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* XPATH::SetOnlyExt()
* @brief      Set only ext
* @ingroup    XUTILS
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyExt()
{
  XSTRING string;

  string.Empty();

  GetExt(string);

  Set(string);

  return Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::DeleteDrive()
* @brief      Delete drive
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::DeleteDrive()
{
  if(IsEmpty())    return false;
  if(GetSize()<=2) return false;

  if(Get()[1]!=_C(':')) return false;

  XPATH xpath2;

  xpath2 = Get();
  xpath2.DeleteCharacters(0, 2);

  Set(xpath2.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::DeleteExt()
* @brief      Delete ext
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::DeleteExt()
{
  SetOnlyPathAndNamefile();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Slash_HaveAtLast()
* @brief      Slash have at last
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Slash_HaveAtLast()
{
  XCHAR character = Character_GetLast();
  
  if((character == _C('\\')) || (character == _C('/'))) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Slash_Add()
* @brief      Slash add
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Slash_Add()
{
  int           count[2] = { 0 , 0 };
  XCHAR* str             = Get();
  bool          status   = false;

  XDWORD c = 0;

  while(c < GetSize())
    {
      if(str[c]==_C('/'))  count[0]++;
      if(str[c]==_C('\\')) count[1]++;

      c++;
    }

  if(!Slash_HaveAtLast())
    {
      if(count[0]>count[1])
             Add("/");
        else Add("\\");

      Slash_Normalize();

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Slash_Normalize(bool inverse)
* @brief      Slash normalize
* @ingroup    XUTILS
* 
* @param[in]  inverse : Inverse value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Slash_Normalize(bool inverse)
{
  if(IsEmpty()) return false;

  XCHAR* str = Get();
  int           c   = 0;

  while(str[c])
    {
      if(inverse)
       {
         if(str[c]==_C('/'))  str[c] = _C('\\');
       }
      else
       {
         if(str[c]==_C('\\')) str[c] = _C('/');
       }

      c++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Slash_Delete()
* @brief      Slash delete
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Slash_Delete()
{
  bool status = false;

  if(Slash_HaveAtLast())
    {
      DeleteCharacter('\\' , XSTRINGCONTEXT_TO_END);
      DeleteCharacter('/'  , XSTRINGCONTEXT_TO_END);

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::AddToNameFile(bool isprefix, XCHAR* tag)
* @brief      Add to name file
* @ingroup    XUTILS
* 
* @param[in]  isprefix : Isprefix value.
* @param[in]  tag : Tag pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::AddToNameFile(bool isprefix, XCHAR* tag)
{
  XSTRING  drive;
  XPATH    xpath;
  XSTRING  name;
  XSTRING  newname;
  XSTRING  ext;

  if(!Split(&drive, &xpath, &name, &ext)) return false;

  //if(!drive.IsEmpty()) drive.Add(_L("/"));

  xpath.Slash_Add();

  if(isprefix)
    {
      newname  = tag;
      newname += name;
    }
   else
    {
      newname  = name;
      newname += tag;
    }

  Format(_L("%s%s%s%s"), drive.Get(), xpath.Get(), newname.Get(), ext.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Create(int nelements, ...)
* @brief      Create resource
* @ingroup    XUTILS
* 
* @param[in]  nelements : Nelements value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Create(int nelements, ...)
{
  va_list arg;

  Empty();

  va_start(arg, nelements);

  int c=0;
  while(c<nelements)
    {
      XPATH xpath;

      xpath = (XCHAR*)va_arg(arg, XCHAR*);
      if(!xpath.IsEmpty())
        {
          if(c && xpath.Character_GetFirst()!=_C('.'))
            {
              Slash_Add();
            }

          Add(xpath);
        }

      c++;
    }

  va_end(arg);

  Slash_Normalize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Create(XPATHSMANAGERSECTIONTYPE sectiontype, int nelements, ...)
* @brief      Create resource
* @ingroup    XUTILS
* 
* @param[in]  sectiontype : Sectiontype value.
* @param[in]  nelements : Nelements value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Create(XPATHSMANAGERSECTIONTYPE sectiontype, int nelements, ...)
{
  XPATH   xpathsection;
  va_list arg;
  bool    status = true;

  Empty();

  GEN_XPATHSMANAGER.GetPathOfSection(sectiontype, xpathsection);

  Add(xpathsection);

  va_start(arg, nelements);

  int c=0;
  while(c<nelements)
    {
      XPATH xpath;

      xpath = (XCHAR*)va_arg(arg, XCHAR*);
      if(!xpath.IsEmpty())
        {
          if(c && xpath.Character_GetFirst()!=_C('.'))
            {
              Slash_Add();
            }

          Add(xpath);
        }

      c++;
    }

  va_end(arg);

  Slash_Normalize();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Split(XSTRING* drive, XPATH* xpath, XSTRING* name, XSTRING* ext)
* @brief      Split
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive pointer to use.
* @param[in]  xpath : Path to use.
* @param[in]  name : Name to use.
* @param[in]  ext : Ext pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Split(XSTRING* drive, XPATH* xpath, XSTRING* name, XSTRING* ext)
{
  XCHAR* pathstr = Get();
  int    idrive  = 0;
  int    iname   = 0;
  int    iext    = 0;
  int    c;

  if(IsEmpty()) return false;
  if(!pathstr)  return false;

  c = 0;
  while(pathstr[c])
    {
      if(pathstr[c]==_C(':'))
        {
          if(drive) Copy(0, c+1, (*drive));

          idrive = c+1;
          break;
        }

      c++;
    }

  c = (GetSize()-1);
  while(pathstr[c])
    {
      if((pathstr[c]==_C('\\')) || (pathstr[c]==_C('/')))
        {
          break;
        }

      if(pathstr[c]==_C('.'))
        {
          if(ext) Copy(c, GetSize(), (*ext));

          iext = c;
          break;
        }

      c--;
    }

  c = (iext)?iext:GetSize();
  while(c>=0)
    {
      if((!c) || pathstr[c]==_C('\\') || pathstr[c]==_C('/')  || pathstr[c]==_C(':'))
        {
          if(pathstr[c]==_C('\\') || pathstr[c]==_C('/') || pathstr[c]==_C(':')) c++;

          if(name) Copy(c, (iext)?iext:GetSize(), (*name));

          iname = c;
          break;
        }

      c--;
    }

  if(xpath && iname) Copy(idrive, iname, (*xpath));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XPATH::Split(XSTRING* drive, XVECTOR<XSTRING*>& subpaths, XSTRING* name, XSTRING* ext)
* @brief      split
* @ingroup    XUTILS
* 
* @param[in]  drive : Drive pointer to use.
* @param[in]  subpaths : Subpaths pointer to use.
* @param[in]  name : Name to use.
* @param[in]  ext : Ext pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XPATH::Split(XSTRING* drive, XVECTOR<XSTRING*>& subpaths, XSTRING* name, XSTRING* ext)
{
  XSTRING  _drive;
  XPATH    _xpath; 
  XSTRING  _name;
  XSTRING  _ext;

  if(!Split(&_drive, &_xpath, &_name, &_ext))
    {
      return false;
    }

  if(!name)
    {
      if(!_name.IsEmpty() && _ext.IsEmpty())    
        {
          _xpath.Slash_Add();
          _xpath.Add(_name);
        }
    }

  XSTRING* newpart = NULL;  

  for(XDWORD c=0; c<_xpath.GetSize()+1; c++)
    {
      if(!newpart)
        {
          newpart = GEN_NEW XSTRING();
          if(!newpart) 
            {
              return false;
            }
        }
      
      if((_xpath[c]==_C('\\')) || (_xpath[c]==_C('/')) || (c == _xpath.GetSize()))
        {
          if(!c)
            {
              continue;
            }        
           else
            {
              subpaths.Add(newpart);  
              newpart = NULL;
            }
        }
       else 
        {
          newpart->Add(_xpath[c]);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XPATH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XPATH::Clean()
{

}


