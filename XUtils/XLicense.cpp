/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLicense.cpp
* 
* @class      XLICENSE
* @brief      eXtended Utils License generator and control class
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

#include "XLicense.h"

#include "XFactory.h"
#include "XSystem.h"
#include "XPublisher.h"
#include "XFileHash.h"
#include "XFile.h"
#include "XFileINI.h"

#include "HashSHA1.h"
#include "HashSHA2.h"
#include "CipherKeySymmetrical.h"
#include "CipherAES.h"

#include "DIOFactory.h"
#include "DIOMAC.h"
#include "DIOWebClient.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLICENSEID::XLICENSEID()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLICENSEID::XLICENSEID()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLICENSEID::~XLICENSEID()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLICENSEID::~XLICENSEID()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID* XLICENSEID::GetID()
* @brief      get Id
* @ingroup    XUTILS
* 
* @return     XUUID* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID* XLICENSEID::GetID()
{
  return &ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLICENSEID::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLICENSEID::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLICENSE::XLICENSE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLICENSE::XLICENSE()
{
  Clean();

  license = GEN_NEW XBUFFER();

  RegisterEvent(XLICENSEXEVENT_TYPE_INVALID);
  RegisterEvent(XLICENSEXEVENT_TYPE_EXPIRED);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLICENSE::~XLICENSE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLICENSE::~XLICENSE()
{
  DeRegisterEvent(XLICENSEXEVENT_TYPE_INVALID);
  DeRegisterEvent(XLICENSEXEVENT_TYPE_EXPIRED);

  GEN_DELETE license;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::GenerateMachineID(XLICENSEID& licenseID)
* @brief      Generate machine ID
* @ingroup    XUTILS
* 
* @param[in]  licenseID : License ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::GenerateMachineID(XLICENSEID& licenseID)
{
  XSTRING   origin; 
  XBUFFER   originbuffer; 
  HASHSHA2* sha2        = NULL;
  XUUID     ID;
  XBUFFER*  result      = NULL;

  ID.Empty();

  //GEN_XSYSTEM.GetOperativeSystemID(origin);
  
  origin.AddFormat(__L(" %s"), GEN_XSYSTEM.GetBIOSSerialNumber()->Get());
  origin.AddFormat(__L(" %s"), GEN_XSYSTEM.GetCPUSerialNumber()->Get());

  origin.ConvertToUTF8(originbuffer);
    
  sha2 = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
  if(!sha2)
    {
      return false;
    }

  sha2->Do(originbuffer);
  result = sha2->GetResult();

  if(!result)
    {
      return false;
    }

  if(!result->GetSize())
    {
      return false;
    }

  XDWORD data1 = 0; 
  XWORD  data2 = 0;
  XWORD  data3 = 0;
  XBYTE  data4 = 0;
  XBYTE  data5 = 0; 
  XBYTE* data6 = &result->Get()[12];

  result->Get((XDWORD&)data1);  
  result->Get((XWORD&)data2);  
  result->Get((XWORD&)data3);  
  result->Get((XBYTE&)data4);  
  result->Get((XBYTE&)data5);  

  ID.Set(data1, data2, data3, data4, data5, data6);

  ID.GetToString(origin);

  GEN_DELETE sha2;

  licenseID.GetID()->Set(ID);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::Generate(XLICENSEID& licenseID, XSTRING* applicationID, XBUFFER* license)
* @brief      Generate
* @ingroup    XUTILS
* 
* @param[in]  licenseID : License ID value.
* @param[in]  applicationID : Application ID pointer to use.
* @param[in]  license : License pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::Generate(XLICENSEID& licenseID, XSTRING* applicationID, XBUFFER* license)
{
  CIPHERAES             cipherAES;
  CIPHERKEYSYMMETRICAL  key;
  XBUFFER               xbufferkey;
  XBUFFER               xbuffercipher;
  XBUFFER*              _license       = license;
  XSTRING*              _applicationID = applicationID;

  if(!_license) _license = this->license;
  if(!_license) 
    {
      return false;
    }

  if(!_applicationID) _applicationID = &this->applicationID;
  if(!_applicationID) 
    {
      return false;
    }

  _license->Delete();
  licenseID.GetID()->GetToBuffer(xbuffercipher);

  GetBufferKeyFromMachineID((*_applicationID), xbufferkey);

  key.Set(xbufferkey);

  cipherAES.SetKey(&key);
  cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);
  cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_NONE);

  if(cipherAES.Cipher(xbuffercipher))
    {
      _license->Delete();
      _license->Add(cipherAES.GetResult());

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::GetBufferKeyFromMachineID(XSTRING& applicationID, XBUFFER& xbufferkey)
* @brief      Get buffer key from machine ID
* @ingroup    XUTILS
* 
* @param[in]  applicationID : Application ID value.
* @param[in]  xbufferkey : Xbufferkey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::GetBufferKeyFromMachineID(XSTRING& applicationID, XBUFFER& xbufferkey)
{
  HASHSHA1 sha1;

  xbufferkey.Delete();
  
  XBUFFER charstr;
    
  applicationID.ConvertToASCII(charstr);
  xbufferkey.Add((XBYTE*)charstr.Get(), applicationID.GetSize());

  sha1.Do(xbufferkey);

  xbufferkey.Delete();
  xbufferkey.Add(sha1.GetResult());

  xbufferkey.Resize(16);

  return xbufferkey.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::CipherExpirationDate(bool cipher, XSTRING& applicationID, XSTRING& expirationdate)
* @brief      Cipher expiration date
* @ingroup    XUTILS
* 
* @param[in]  cipher : Cipher value.
* @param[in]  applicationID : Application ID value.
* @param[in]  expirationdate : Expirationdate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::CipherExpirationDate(bool cipher, XSTRING& applicationID, XSTRING& expirationdate)
{
  XBUFFER               xbufferkey;
  CIPHERAES             cipherAES;
  CIPHERKEYSYMMETRICAL  key;
  bool                  status = false;

  if(!GetBufferKeyFromMachineID(applicationID, xbufferkey)) 
    {
      return false;
    }

  key.Set(xbufferkey);

  cipherAES.SetKey(&key);
  cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);
  cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_SPACES);

  if(cipher)
    {
      XBUFFER charstr;
      
      expirationdate.ConvertToASCII(charstr);
   
      status = cipherAES.Cipher((XBYTE*)charstr.Get(), expirationdate.GetSize());
      if(status)
        {
          expirationdate.Empty();
          cipherAES.GetResult()->ConvertToBase64(expirationdate);
          return true;
        }
    }
   else
    {
      XBUFFER xbuffer;
      
      xbuffer.ConvertFromBase64(expirationdate);

      status = cipherAES.Uncipher(xbuffer);
      if(status)
        {
          expirationdate.Empty();

          for(XDWORD c=0;  c<cipherAES.GetResult()->GetSize(); c++)
            {
              expirationdate.Add((XCHAR)cipherAES.GetResult()->GetByte(c));
            }

          expirationdate.DeleteCharacter(0x20, XSTRINGCONTEXT_TO_END);

          return true;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLICENSE::Application_GetID()
* @brief      Application get ID
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLICENSE::Application_GetID()
{
  return &applicationID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XLICENSE::GetExpirationDateTime()
* @brief      Get expiration date time
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XLICENSE::GetExpirationDateTime()
{
  return &expirationdatetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XLICENSE::Get()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XLICENSE::Get()
{
  return license;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::Get(XSTRING& licensestring)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  licensestring : Licensestring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::Get(XSTRING& licensestring)
{
  licensestring.Empty();

  if(!license)            
    {
      return false;
    }

  if(!license->GetSize()) 
    {
      return false;
    }

  for(XDWORD c=0; c<license->GetSize(); c++)
    {
      XSTRING hexchar;

      hexchar.Format(__L("%02X"), license->GetByte(c));
      licensestring += hexchar;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::LoadFromFile(XPATH& xpath, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
* @brief      Load from file
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  applicationID : Application ID value.
* @param[in]  license : License pointer to use.
* @param[in]  expirationdate : Expirationdate pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::LoadFromFile(XPATH& xpath, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{
  if(!license) return false;

  XFILE*    xfile   = NULL;
  XBUFFER   xbuffer;
  bool      status  = false;

  xfile=GEN_XFACTORY.Create_File();
  if(!xfile) 
    {
      return false;
    }

  if(xfile->Open(xpath))
    {
      xbuffer.Resize((XDWORD)xfile->GetSize());

      status = xfile->Read(xbuffer.Get(), xbuffer.GetSize());

      xfile->Close();
      GEN_XFACTORY.Delete_File(xfile);

      if(status) 
        {
          status = LoadFromBuffer(xbuffer, applicationID, license, expirationdate);
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::LoadFromURL(DIOURL& url, int timeout, XSTRING* IPlocal, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
* @brief      Load from URL
* @ingroup    XUTILS
* 
* @param[in]  url : URL to use.
* @param[in]  timeout : Timeout value.
* @param[in]  IPlocal : I Plocal pointer to use.
* @param[in]  applicationID : Application ID value.
* @param[in]  license : License pointer to use.
* @param[in]  expirationdate : Expirationdate pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::LoadFromURL(DIOURL& url, int timeout, XSTRING* IPlocal, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{
  DIOWEBCLIENT* webclient = GEN_NEW DIOWEBCLIENT;
  if(!webclient) 
    {
      return false;
    }

  XBUFFER xbuffer;
  bool    status;

  status = webclient->Get(url.Get(), xbuffer, NULL, timeout, IPlocal);
  if(status) 
    {
      status = LoadFromBuffer(xbuffer, applicationID, license, expirationdate);
    }

  GEN_DELETE webclient;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::LoadFromBuffer(XBUFFER& xbuffer, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
* @brief      Load from buffer
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  applicationID : Application ID value.
* @param[in]  license : License pointer to use.
* @param[in]  expirationdate : Expirationdate pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::LoadFromBuffer(XBUFFER& xbuffer, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{
  if(!license) 
    {
      return false;
    }

  XFILEINI    fileini;
  XLICENSEID  licenseID;
  XSTRING     licenseIDstring;
  XSTRING     licenseIDstring2;
  XSTRING     licensestring;
  XSTRING     licenseexprirationstring;
  XBUFFER*    _license = license;

  if(!_license) _license = this->license;
  if(!_license) 
    {
      return false;
    }

  fileini.AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);

  fileini.ConvertFromLines();

  fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_ID, licenseIDstring);
  fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE, licensestring);
  fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_EXPIRATION , licenseexprirationstring);

  GenerateMachineID(licenseID);
  licenseID.GetID()->GetToString(licenseIDstring2);

  if(licenseIDstring.Compare(licenseIDstring2)) 
    {
      return false;
    }

  CipherExpirationDate(false, applicationID, licenseexprirationstring);

  if(expirationdate) (*expirationdate) = licenseexprirationstring;

  licensestring.ConvertHexStringToBuffer((*_license));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::CheckMasterCreation(XPATH& xpath, XLICENSEID& licenseID, XSTRING& applicationID, int expirationdays)
* @brief      check master creation
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  licenseID : License ID value.
* @param[in]  applicationID : Application ID value.
* @param[in]  expirationdays : Expirationdays value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::CheckMasterCreation(XPATH& xpath, XLICENSEID& licenseID, XSTRING& applicationID, int expirationdays)
{
  XFILEINI    fileini;
  XSTRING     licensestring;

  if(!fileini.Open(xpath)) 
    {
      return false;
    }

  fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE, licensestring);
  fileini.Close();

  if(!applicationID.Compare(licensestring))
    {
      XFILEINI    fileini;
      XSTRING     licenseIDstring;
      XSTRING     licensestring;
      XSTRING     licenseexpiration;
      XBUFFER     licencia;
      XDATETIME*  expirationdatetime;

      licenseID.GetID()->GetToString(licenseIDstring);
      if(!Generate(licenseID, &applicationID, &licencia)) 
        {
          return false;
        }

      licensestring.ConvertHexStringFromBuffer(licencia);

      expirationdatetime=GEN_XFACTORY.CreateDateTime();
      if(!expirationdatetime) 
        {
          return false;
        }

      expirationdatetime->SetToZero();
      if(expirationdays)
        {
          expirationdatetime->Read();
          expirationdatetime->AddDays((int)expirationdays);
        }

      expirationdatetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, licenseexpiration);
      GEN_XFACTORY.DeleteDateTime(expirationdatetime);

      if(!CipherExpirationDate(true, applicationID, licenseexpiration)) 
        {
          return false;
        }

      if(!fileini.Create(xpath)) 
        {
          return false;
        }

      fileini.CreateSection(XLICENSE_FILESECTION);
      fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_ID, licenseIDstring);
      fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE, licensestring);
      fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_EXPIRATION, licenseexpiration);

      fileini.Close();

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLICENSE::SetEvent(XLICENSEXEVENT_TYPE type)
* @brief      Set event
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLICENSE::SetEvent(XLICENSEXEVENT_TYPE type)
{
  XLICENSE_XEVENT xlicenseevent(this, type);

  xlicenseevent.Set(this);
  PostEvent(&xlicenseevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLICENSE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLICENSE::Clean()
{
  license = NULL;
}





