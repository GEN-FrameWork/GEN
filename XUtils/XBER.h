/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBER.h
* 
* @class      XBER
* @brief      eXtended Utils Basic Encoding Rules (BER) encoding class (https://en.wikipedia.org/wiki/X.690)
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

#include "XBase.h"
#include "XVector.h"
#include "XBuffer.h"
#include "XString.h"
#include "XVariant.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XBER_TAGTYPE_RESERVEDBER                   0
#define XBER_TAGTYPE_BOOLEAN	                     1
#define XBER_TAGTYPE_INTEGER	                     2
#define XBER_TAGTYPE_BIT_STRING	                   3
#define XBER_TAGTYPE_OCTET_STRING                  4	
#define XBER_TAGTYPE_NULL                          5	
#define XBER_TAGTYPE_OBJECT_IDENTIFIER             6	
#define XBER_TAGTYPE_OBJECT_DESCRIPTOR             7	
#define XBER_TAGTYPE_EXTERNAL                      8	
#define XBER_TAGTYPE_REAL                          9	
#define XBER_TAGTYPE_ENUMERATED                   10	
#define XBER_TAGTYPE_EMBEDDED_PDV                 11	
#define XBER_TAGTYPE_UTF8STRING                   12	
#define XBER_TAGTYPE_RELATIVE_OID                 13	
#define XBER_TAGTYPE_TIME                         14	
#define XBER_TAGTYPE_RESERVED                     15	
#define XBER_TAGTYPE_SEQUENCE                     16
#define XBER_TAGTYPE_SET                          17
#define XBER_TAGTYPE_NUMERICSTRING                18	
#define XBER_TAGTYPE_PRINTABLESTRING              19
#define XBER_TAGTYPE_T61STRING                    20
#define XBER_TAGTYPE_VIDEOTEXSTRING               21
#define XBER_TAGTYPE_IA5STRING                    22
#define XBER_TAGTYPE_UTCTIME                      23
#define XBER_TAGTYPE_GENERALIZEDTIME              24
#define XBER_TAGTYPE_GRAPHICSTRING                25
#define XBER_TAGTYPE_VISIBLESTRING                26
#define XBER_TAGTYPE_GENERALSTRING                27
#define XBER_TAGTYPE_UNIVERSALSTRING              28
#define XBER_TAGTYPE_CHARACTER_STRING        	    29
#define XBER_TAGTYPE_BMPSTRING                    30
#define XBER_TAGTYPE_DATE                         31
#define XBER_TAGTYPE_TIME_OF_DAY             	    32
#define XBER_TAGTYPE_DATE_TIME                    33
#define XBER_TAGTYPE_DURATION                     34
#define XBER_TAGTYPE_OID_IRI                      35
#define XBER_TAGTYPE_RELATIVE_OID_IRI             36
#define XBER_TAGTYPE_CONTEXT_SPECIFIC            127

enum XBER_TAGCLASS
{ 
  XBER_TAGCLASS_UNIVERSAL	                      = 0 ,	  // The type is native to ASN.1
  XBER_TAGCLASS_APPLICATION	                        ,	  // The type is only valid for one specific application
  XBER_TAGCLASS_CONTEXT_SPECIFIC	                  ,	  // Meaning of this type depends on the context (such as within a sequence, set or choice)
  XBER_TAGCLASS_PRIVATE	                            ,	  // Defined in private specifications

  XBER_TAGCLASS_UNKNOWN                               
};

#define XBER_TAG_MASKTYPE                       0x1F  
#define XBER_TAG_MASKISCONSTRUCTED              0x20  

#define XBER_TAG_ISCONSTRUCTED(tag)             (tag&XBER_TAG_MASKISCONSTRUCTED)?true:false
#define XBER_TAG_CLASS(tag)                     (XBER_TAGCLASS)(tag>>6)

// OBSOLETE
#define XBERTYPE_ISCONSTRUCTED                  XBER_TAG_MASKISCONSTRUCTED

#define XBER_MAXLEVELS                          256

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBER : public XSUBJECT
{
  public:
                              XBER                      ();
    virtual                  ~XBER                      ();

    XBYTE                     GetTagType                ();
    bool                      GetTagTypeName            (XSTRING& name);
    XBER_TAGCLASS             GetTagClass               ();

    bool                      IsConstructed             ();    
        
    XDWORD                    GetSizeHead               ();
    XDWORD                    GetSize                   ();

    XBUFFER*                  GetData                   ();

    XVARIANT*                 GetValue                  ();

    XDWORD                    GetLevel                  ();
    XDWORD*                   GetLevels                 ();
    bool                      GetLevels                 (XSTRING& string);

    bool                      GetDump                   (XBUFFER& xbuffer, bool notheader = false);
    bool                      SetFromDump               (XBUFFER& xbuffer, XOBSERVER* observer = NULL);

    bool                      SetTagType                (XBYTE type);
    bool                      SetSize                   (XDWORD size);

    bool                      SetNULL                   ();

    bool                      SetINTEGER                (XDWORD integer);
    bool                      SetINTEGER                (XBUFFER& xbufferdata);

    bool                      SetBITSTRING              (XBUFFER& databuffer);
    bool                      SetBITSTRING              (XBYTE* buffer, XDWORD size);

    bool                      SetOCTETSTRING            (XCHAR* string);
    bool                      SetOCTETSTRING            (XBUFFER& databuffer);
    bool                      SetOCTETSTRING            (XBYTE* buffer, XDWORD size);

    bool                      SetOID                    (XCHAR* OIDstring);

    bool                      Copy                      (XBER* newber);

    XVECTOR<XBER*>*           Sequence_GetSequences     ();
    bool                      Sequence_AddCommandTo     (XBYTE command, XBER& xber);
    virtual bool              Sequence_AddTo            (XBER& xber);
    XDWORD                    Sequence_GetSize          ();
    bool                      Sequence_DeleteAll        ();

  protected:

    XBYTE                     tagtype;
    XSTRING                   nametagtype;
    XBER_TAGCLASS             tagclass; 

    bool                      isconstructed;  

    XBYTE                     sizehead;
    XDWORD                    size;
    XBUFFER                   data;
    XBYTE                     contextspecificvalue;
    XBYTE                     unusedbits;
    XVARIANT                  value;
    
    XVECTOR<XBER*>            sequences;

  private:

    bool                      SetFromDumpInternal             (XBUFFER& xbuffer, XOBSERVER* observer = NULL, XBER* root = NULL);

    bool                      ConvertToBoolean                (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToInteger                (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToOctetString            (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToBitString              (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToNULL                   (XVARIANT& variant);
    bool                      ConvertToObjetIdentifier        (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToPrintableString        (XBUFFER& data, XVARIANT& variant);
    bool                      ConvertToUTCTime                (XBUFFER& data, XVARIANT& variant); 

    bool                      CalculeSize                     (XBUFFER& buffer, XDWORD& sizedataber, XBYTE& sizehead);

    bool                      CodeBigNumber                   (XDWORD number, XBUFFER& data);
    bool                      CodeSize                        (XDWORD number, XBUFFER& data);

    void                      Clean                           ();

    XDWORD                    totalposition;
    XDWORD                    level;
    XDWORD                    levels[XBER_MAXLEVELS];

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





