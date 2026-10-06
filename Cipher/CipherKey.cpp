/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherKey.cpp
* 
* @class      CIPHERKEY
* @brief      Cipher Key interface class
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

#include "CipherKey.h"

#include <string.h>

#include "XFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEY::CIPHERKEY()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY::CIPHERKEY()
{ 
  Clean();                                          
}

       
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEY::~CIPHERKEY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY::~CIPHERKEY()
{ 
  Clean();                                          
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEYTYPE CIPHERKEY::GetType()
* @brief      Get type
* @ingroup    CIPHER
* 
* @return     CIPHERKEYTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEYTYPE CIPHERKEY::GetType()
{ 
  return type;                                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERKEY::SetType(CIPHERKEYTYPE type)
* @brief      Set type
* @ingroup    CIPHER
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERKEY::SetType(CIPHERKEYTYPE type)
{ 
  this->type = type;                                
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* CIPHERKEY::GetTypeStr()
* @brief      get type str
* @ingroup    CIPHER
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* CIPHERKEY::GetTypeStr()
{ 
  XCHAR* nametype = NULL;

  switch(type)
    {
      case CIPHERKEYTYPE_UNKNOWN                        : 
                                  default               : nametype = _L("Unknown");                         break;                                                        
      case CIPHERKEYTYPE_SYMMETRICAL                    : nametype = _L("Symmetrical");                     break;                                                                                                                
      case CIPHERKEYTYPE_RSA_PUBLIC                     : nametype = _L("RSA public");                      break;                                                         
      case CIPHERKEYTYPE_RSA_PRIVATE                    : nametype = _L("RSA private");                     break;                                                                                                                
      case CIPHERKEYTYPE_ECDSA_SECP192R1_PUBLIC	        : nametype = _L("ECDSA secp192r1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP192R1_PRIVATE	      : nametype = _L("ECDSA secp192r1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP224R1_PUBLIC	        : nametype = _L("ECDSA secp224r1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP224R1_PRIVATE	      : nametype = _L("ECDSA secp224r1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC	        : nametype = _L("ECDSA secp256r1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE	      : nametype = _L("ECDSA secp256r1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC	        : nametype = _L("ECDSA secp384r1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE	      : nametype = _L("ECDSA secp384r1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC	        : nametype = _L("ECDSA secp521r1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE	      : nametype = _L("ECDSA secp521r1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP256K1_PUBLIC	        : nametype = _L("ECDSA secp256k1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECP256K1_PRIVATE	      : nametype = _L("ECDSA secp256k1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECT163K1_PUBLIC	        : nametype = _L("ECDSA sect163k1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECT163K1_PRIVATE	      : nametype = _L("ECDSA sect163k1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECT233K1_PUBLIC	        : nametype = _L("ECDSA sect233k1 public");          break;                                                        
      case CIPHERKEYTYPE_ECDSA_SECT233K1_PRIVATE	      : nametype = _L("ECDSA sect233k1 private");         break;                                                        
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP256R1_PUBLIC	  : nametype = _L("ECDSA brainpoolp256r1 public");    break;                                                        
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP256R1_PRIVATE  : nametype = _L("ECDSA brainpoolp256r1 private");   break;                                                        
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP384R1_PUBLIC	  : nametype = _L("ECDSA brainpoolp384r1 public");    break;                                                        
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP384R1_PRIVATE  : nametype = _L("ECDSA brainpoolp384r1 private");   break;                                                        
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP512R1_PUBLIC	  : nametype = _L("ECDSA brainpoolp512r1 public");    break;                                                                                                          
      case CIPHERKEYTYPE_ECDSA_BRAINPOOLP512R1_PRIVATE  : nametype = _L("ECDSA brainpoolp512r1 private");   break;                                                        
      case CIPHERKEYTYPE_ED25519_PUBLIC                  : nametype = _L("Ed25519 public");                   break;
      case CIPHERKEYTYPE_ED25519_PRIVATE                 : nametype = _L("Ed25519 private");                  break;
    }

  return nametype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERKEY::GetSizeInBytes()
* @brief      Get size in bytes
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERKEY::GetSizeInBytes()
{ 
  return 0;                                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERKEY::GetSizeInBits()
* @brief      Get size in bits
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERKEY::GetSizeInBits()
{ 
  return (GetSizeInBytes()*8);                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERKEY::CopyFrom(CIPHERKEY* key)
* @brief      Copy from
* @ingroup    CIPHER
* 
* @param[in]  key : Key pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERKEY::CopyFrom(CIPHERKEY* key)
{
  if(!key) return false;
  type = key->GetType();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERKEY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERKEY::Clean()
{
  type = CIPHERKEYTYPE_UNKNOWN;
}





