/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamConfigString.cpp
* 
* @class      DIOSTREAMCONFIGSTRING
* @brief      Data Input/Output Stream Config String class
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

#include "DIOStreamConfigString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIGSTRING::DIOSTREAMCONFIGSTRING()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIGSTRING::DIOSTREAMCONFIGSTRING()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIGSTRING::~DIOSTREAMCONFIGSTRING()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIGSTRING::~DIOSTREAMCONFIGSTRING()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMCONFIGSTRING::GetToString(XSTRING* string)
* @brief      Get to string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMCONFIGSTRING::GetToString(XSTRING* string)
{
  if(!string)     return false;

  string->Format(_L("%s:%d"), remoteURL.Get(), remoteport);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMCONFIGSTRING::GetToString(XSTRING& string)
* @brief      Get to string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMCONFIGSTRING::GetToString(XSTRING& string)
{
  return GetToString(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMCONFIGSTRING::SetFromString(XCHAR* string)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMCONFIGSTRING::SetFromString(XCHAR* string)
{
  if(!string) return false;

  DIOURL    _string;
  DIOURL    urlall;
  XSTRING   url;
  XSTRING   urladd;
  XSTRING   login;
  XSTRING   password;
  int       endurlaccess;
  int       port = 0;

  remoteURL.Empty();

  _string = string;

  if(_string.HaveHTTPID()) _string.DeleteHTTPID();

  endurlaccess = _string.Find(_L("/"), true);
  if(endurlaccess == XSTRING_NOTFOUND) endurlaccess = _string.Find(_L("\\"), true);
  if(endurlaccess == XSTRING_NOTFOUND) endurlaccess = _string.GetSize();

  _string.Copy(endurlaccess+1, urladd);
  _string.Copy(0, endurlaccess, urlall);
  urlall.Slash_Delete();

  int havepassword = urlall.Find(_L("@"), true);
  int haveport     = urlall.Find(_L(":"), true, (havepassword == XSTRING_NOTFOUND)?0:havepassword);

  login.AdjustSize(_MAXSTR*10);
  password.AdjustSize(_MAXSTR*10);
  url.AdjustSize(_MAXSTR*10);

  if(haveport== XSTRING_NOTFOUND)
    {
      if(havepassword == XSTRING_NOTFOUND)
              urlall.UnFormat(_L("%s"), url.Get());
         else urlall.UnFormat(_L("%s:%s@%s"), login.Get(), password.Get(), url.Get());
    }
   else
    {
      if(havepassword == XSTRING_NOTFOUND)
              urlall.UnFormat(_L("%s:%d"), url.Get(), &port);
         else urlall.UnFormat(_L("%s:%s@%s:%d"), login.Get(), password.Get(), url.Get(), &port);
    }

  login.AdjustSize();
  password.AdjustSize();
  url.AdjustSize();


  if(!login.IsEmpty() || !password.IsEmpty())  remoteURL.AddFormat(_L("%s:%s@"), login.Get(), password.Get());

  remoteURL.Add(url);
  if(urladd.Get()[0])
    {
      remoteURL.Slash_Add();
      remoteURL.Add(urladd);
    }


  if(port) this->remoteport = port;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMCONFIGSTRING::SetFromString(XSTRING* string)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMCONFIGSTRING::SetFromString(XSTRING* string)
{
  if(!string) return false;
  return SetFromString(string->Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMCONFIGSTRING::SetFromString(XSTRING& string, bool addslash)
* @brief      Set from string
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  addslash : Addslash value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMCONFIGSTRING::SetFromString(XSTRING& string, bool addslash)
{
  return SetFromString(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMCONFIGSTRING::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMCONFIGSTRING::Clean()
{
  remoteport = 0;
}



