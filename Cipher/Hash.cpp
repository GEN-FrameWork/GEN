/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Hash.cpp
* 
* @class      HASH
* @brief      Hash interface class
* @ingroup    CIPHER
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

#include "Hash.h"

#include "XFactory.h"
#include "XFile.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASH::HASH()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASH::HASH()
{
  Clean();

  result = GEN_NEW XBUFFER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASH::~HASH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASH::~HASH()
{
  GEN_DELETE result;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHTYPE HASH::GetType()
* @brief      Get type
* @ingroup    CIPHER
* 
* @return     HASHTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHTYPE HASH::GetType()
{ 
  return type;                  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* HASH::GetTypeStr()
* @brief      get type str
* @ingroup    CIPHER
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* HASH::GetTypeStr()
{ 
  XCHAR* name = NULL;

  switch(type)
    {
      case HASHTYPE_NONE      : 
                    default   : name = __L("None");      break;
      case HASHTYPE_CKS16     : name = __L("CKS16");     break;
      case HASHTYPE_CRC16     : name = __L("CRC16");     break;
      case HASHTYPE_CRC32     : name = __L("CRC32");     break;
      case HASHTYPE_MD2       : name = __L("MD2");       break;
      case HASHTYPE_MD4       : name = __L("MD4");       break;
      case HASHTYPE_MD5       : name = __L("MD5");       break;
      case HASHTYPE_SHA1      : name = __L("SHA1");      break;
      case HASHTYPE_SHA224    : name = __L("SHA224");    break;
      case HASHTYPE_SHA256    : name = __L("SHA256");    break;
      case HASHTYPE_SHA384    : name = __L("SHA384");    break;
      case HASHTYPE_SHA512    : name = __L("SHA512");    break;
      case HASHTYPE_RIPEMD160 : name = __L("RIPEMD160"); break;
    }                  

  return name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* HASH::GetOUINoSign()
* @brief      Get OUI no sign
* @ingroup    CIPHER
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* HASH::GetOUINoSign()
{ 
  return &OUInosignstring;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::Do(XBYTE* input, XQWORD size)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::Do(XBYTE* input, XQWORD size)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::Do(XBUFFER& input)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::Do(XBUFFER& input)
{
  return Do(input.Get(), input.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::Do(XPATH& xpath, XQWORD size, XQWORD pos)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  xpath : Path to use.
* @param[in]  size : Size value.
* @param[in]  pos : Pos value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::Do(XPATH& xpath, XQWORD size, XQWORD pos)
{
  XFILE* xfile;
  bool   status = false;

  xfile=GEN_XFACTORY.Create_File();
  if(!xfile) return false;

  ResetResult();

  if(xfile->Open(xpath, true))
    {
      XQWORD _size;

      if(size == HASHALLFILESIZE) _size = xfile->GetSize(); else _size = size;

      status = Do(xfile, _size, pos);

      xfile->Close();
    }

  if(xfile) GEN_XFACTORY.Delete_File(xfile);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::Do(XFILE* xfile, XQWORD size, XQWORD pos)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  xfile : Xfile pointer to use.
* @param[in]  size : Size value.
* @param[in]  pos : Pos value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::Do(XFILE* xfile, XQWORD size, XQWORD pos)
{
  if(!xfile)           return false;
  if(!xfile->IsOpen()) return false;

  XBUFFER   xbuffer;
  XQWORD    block   = (((size==HASHALLFILESIZE)?xfile->GetSize():size) / HASHMAXFILESIZEBUFFER);
  XDWORD    mod     = (((size==HASHALLFILESIZE)?xfile->GetSize():size) % HASHMAXFILESIZEBUFFER);
  bool      status  = true;

  if(!xfile->SetPosition(pos)) return false;

  xbuffer.Resize((XDWORD)HASHMAXFILESIZEBUFFER);

  for(XDWORD c=0;c<block;c++)
    {
      if(xfile->Read(xbuffer.Get(), HASHMAXFILESIZEBUFFER))
        {
          if(!Do(xbuffer))
            {
              status = false;
              break;
            }
        }
       else
        {
          status = false;
          break;
        }
    }

  if(status)
    {
      xbuffer.Resize(mod);
      if(xfile->Read(xbuffer.Get(), mod))
        {
          if(!Do(xbuffer))
            {
              status = false;
            }
        }
       else
        {
          status = false;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::ResetResult()
* @brief      Reset result
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::ResetResult()
{
  if(!result) return false;

  result->Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int HASH::GetDefaultSize()
* @brief      Get default size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int HASH::GetDefaultSize()
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* HASH::GetResult()
* @brief      Get result
* @ingroup    CIPHER
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* HASH::GetResult()
{
  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* HASH::GetResult(XDWORD& resultsize)
* @brief      Get result
* @ingroup    CIPHER
* 
* @param[in]  resultsize : Output resultsize.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* HASH::GetResult(XDWORD& resultsize)
{
  resultsize = 0;

  if(!result) return NULL;

  resultsize  = result->GetSize();

  return result->Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASH::GetResultString(XSTRING& stringhex)
* @brief      Get result string
* @ingroup    CIPHER
* 
* @param[in]  stringhex : Stringhex value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASH::GetResultString(XSTRING& stringhex)
{
  stringhex.Empty();

  XBUFFER* xbuffer = GetResult();
  if(!xbuffer) return false;

  if(!xbuffer->GetSize()) return false;

  for(int c=0;c<(int)xbuffer->GetSize();c++)
    {
      XSTRING strbyte;

      strbyte.Format(__L("%02x"), xbuffer->GetByte(c));

      stringhex += strbyte;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASH::Clean()
{
  type     = HASHTYPE_NONE;
  result   = NULL;
}


