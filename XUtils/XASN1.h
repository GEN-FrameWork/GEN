/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XASN1.h
* 
* @class      XASN1
* @brief      eXtended Utils ANS.1 format buffer  (Abstract Syntax Notation One, defined in X.208)
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBER.h"
#include "XBuffer.h"
#include "XSubject.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef struct
{
  XCHAR*    OID;
  XCHAR*    description; 
  bool      isconstructed;
   
} XASN1_OID_PROPERTY;



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XASN1 : public XSUBJECT
{
  public:
                                      XASN1										        ();
    virtual                          ~XASN1										        ();

    static XASN1_OID_PROPERTY*        GetOIDProperty                  (XCHAR* OID); 
    static XCHAR*                     GetOIDPropertyDescription       (XCHAR* OID);

		bool    				                  Decode								          (XBUFFER& databin, XOBSERVER* observer = NULL);

  private:
	
    void                              Clean										        ();
    
    static XASN1_OID_PROPERTY         OID_properties[];
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





