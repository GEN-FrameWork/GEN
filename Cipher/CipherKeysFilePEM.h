/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherKeysFilePEM.h
* 
* @class      CIPHERKEYSFILEPEM
* @brief      Cipher File Keys PEM (GEN Key File) class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XDateTime.h"
#include "XASN1.h"
#include "XObserver.h"

#include "CipherKey.h"
#include "CipherTrustedRootCertificatesX509.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERKEYSFILEPEM_EXT                 __L(".PEM")
#define CIPHERKEYSFILEKEY_EXT                 __L(".KEY")
#define CIPHERKEYSFILECER_EXT                 __L(".CER")

#define CIPHERKEYSFILEPEM_BEGINKEY            __L("-----BEGIN ")
#define CIPHERKEYSFILEPEM_ENDKEY              __L("-----END ")
#define CIPHERKEYSFILEPEM_FINISH              __L("-----")

#define CIPHERKEYSFILEPEM_PUBLIC              __L("PUBLIC")
#define CIPHERKEYSFILEPEM_PRIVATE             __L("PRIVATE")
#define CIPHERKEYSFILEPEM_CERTIFICATE         __L("CERTIFICATE")

#define  CIPHERKEYSFILETYPE_CERTIFICATEX509   CIPHERKEYTYPE_LASTTYPE + 1




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XFILETXT;
class XBER_XEVENT;
class CIPHERCERTIFICATEX509;

class CIPHERKEYSFILEPEM_ENTRYBUFFER
{
  public:
                            CIPHERKEYSFILEPEM_ENTRYBUFFER       ();
                           ~CIPHERKEYSFILEPEM_ENTRYBUFFER       ();

   void                     Clean                               ();

   XSTRING                  type;
   XDWORD                   ini_line;
   XDWORD                   end_line;
   XBUFFER                  data;  

};

class CIPHERKEYSFILEPEM : public XOBSERVER
{
  public:
                            CIPHERKEYSFILEPEM                   ();
    virtual                ~CIPHERKEYSFILEPEM                   ();

   
    bool                    Key_Add                             (CIPHERKEY* key);
    bool                    Key_Del                             (CIPHERKEY* key);
    bool                    Key_DelAll                          ();

    bool                    DecodeCertificates                  (XVECTOR<XSTRING*>* lines);
  
  private:

    bool                    GetCertificatedPropertys            (CIPHERCERTIFICATEX509* certificate, XBER_XEVENT* event);

    void                    HandleEvent_XBER                    (XBER_XEVENT* event);
    void                    HandleEvent                         (XEVENT* xevent);

    void                    Clean                               ();
                              
    XVECTOR<CIPHERKEY*>     keys;

    void*                   decodeobj;
    XDWORD                  decodeobjtype;
    XDWORD                  ndecodeobj; 
    XSTRING                 lastOID;    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





