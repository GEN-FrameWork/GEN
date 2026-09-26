/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBER.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBER.h"

#include "XTrace.h"
#include "XASN1.h"
#include "XObserver.h"
#include "XBER_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBER::XBER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBER::XBER()
{
  Clean();

  data.Empty();

  RegisterEvent(XBERXEVENT_TYPE_DECODE_START);
  RegisterEvent(XBERXEVENT_TYPE_DECODE_DATA);
  RegisterEvent(XBERXEVENT_TYPE_DECODE_END);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBER::~XBER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBER::~XBER()
{
  DeRegisterEvent(XBERXEVENT_TYPE_DECODE_START);
  DeRegisterEvent(XBERXEVENT_TYPE_DECODE_DATA);
  DeRegisterEvent(XBERXEVENT_TYPE_DECODE_END);

  Sequence_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBER::GetTagType()
* @brief      Get tag type
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBER::GetTagType()
{
  return (XBYTE)tagtype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::GetTagTypeName(XSTRING& name)
* @brief      Get tag type name
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::GetTagTypeName(XSTRING& name)
{
  name.Empty();

  switch(GetTagType())
    {
      case XBER_TAGTYPE_CONTEXT_SPECIFIC     : name = __L("CONTEXT_SPECIFIC");	          break;
      case XBER_TAGTYPE_BOOLEAN	             : name = __L("BOOLEAN");	  	                break;
      case XBER_TAGTYPE_INTEGER	             : name = __L("INTEGER");	  	                break;
      case XBER_TAGTYPE_BIT_STRING	         : name = __L("BIT STRING");	  	            break;
      case XBER_TAGTYPE_OCTET_STRING         : name = __L("OCTET STRING");	  	          break;
      case XBER_TAGTYPE_NULL                 : name = __L("NULL");	  	                  break;
      case XBER_TAGTYPE_OBJECT_IDENTIFIER    : name = __L("OBJECT IDENTIFIER");	  	      break;
      case XBER_TAGTYPE_OBJECT_DESCRIPTOR    : name = __L("Object Descriptor");	  	      break;
      case XBER_TAGTYPE_EXTERNAL             : name = __L("EXTERNAL");	  	              break;
      case XBER_TAGTYPE_REAL                 : name = __L("REAL (float)");	  	          break;
      case XBER_TAGTYPE_ENUMERATED           : name = __L("ENUMERATED");	  	            break;
      case XBER_TAGTYPE_EMBEDDED_PDV         : name = __L("EMBEDDED PDV");	  	          break;
      case XBER_TAGTYPE_UTF8STRING           : name = __L("UTF8String");	  	            break;
      case XBER_TAGTYPE_RELATIVE_OID         : name = __L("RELATIVE-OID");	  	          break;
      case XBER_TAGTYPE_TIME                 : name = __L("TIME");	  	                  break;
      case XBER_TAGTYPE_RESERVED             : name = __L("Reserved");	  		            break;
      case XBER_TAGTYPE_SEQUENCE             : name = __L("SEQUENCE");	                  break;
      case XBER_TAGTYPE_SET                  : name = __L("SET");	  	                    break;
      case XBER_TAGTYPE_NUMERICSTRING        : name = __L("NumericString");	  	          break;
      case XBER_TAGTYPE_PRINTABLESTRING      : name = __L("PrintableString");	  	        break;
      case XBER_TAGTYPE_T61STRING            : name = __L("T61String");	  	              break;
      case XBER_TAGTYPE_VIDEOTEXSTRING       : name = __L("VideotexString");	  	        break;
      case XBER_TAGTYPE_IA5STRING            : name = __L("IA5String");	  	              break;
      case XBER_TAGTYPE_UTCTIME              : name = __L("UTCTime");	  	                break;
      case XBER_TAGTYPE_GENERALIZEDTIME      : name = __L("GeneralizedTime");	  	        break;
      case XBER_TAGTYPE_GRAPHICSTRING        : name = __L("GraphicString");	  	          break;
      case XBER_TAGTYPE_VISIBLESTRING        : name = __L("VisibleString");	  	          break;
      case XBER_TAGTYPE_GENERALSTRING        : name = __L("GeneralString");	  	          break;
      case XBER_TAGTYPE_UNIVERSALSTRING      : name = __L("UniversalString");	  	        break;
      case XBER_TAGTYPE_CHARACTER_STRING     : name = __L("CHARACTER STRING");	  	      break;
      case XBER_TAGTYPE_BMPSTRING            : name = __L("BMPString");	  		            break;
      case XBER_TAGTYPE_DATE                 : name = __L("DATE");	  	                  break;
      case XBER_TAGTYPE_TIME_OF_DAY          : name = __L("TIME-OF-DAY");	  	            break;
      case XBER_TAGTYPE_DATE_TIME            : name = __L("DATE-TIME");	  	              break;
      case XBER_TAGTYPE_DURATION             : name = __L("DURATION");	  	              break;
      case XBER_TAGTYPE_OID_IRI              : name = __L("OID-IRI");	  	                break;
      case XBER_TAGTYPE_RELATIVE_OID_IRI     : name = __L("RELATIVE-OID-IRI");	  	      break;
    }

  return (!name.IsEmpty());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBER_TAGCLASS XBER::GetTagClass()
* @brief      Get tag class
* @ingroup    XUTILS
* 
* @return     XBER_TAGCLASS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBER_TAGCLASS XBER::GetTagClass()
{
  return tagclass;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::IsConstructed()
* @brief      Is constructed
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::IsConstructed()
{
  return isconstructed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBER::GetSizeHead()
* @brief      Get size head
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::GetSizeHead()
{
  return sizehead;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBER::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::GetSize()
{
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XBER::GetData()
* @brief      Get data
* @ingroup    XUTILS
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XBER::GetData()
{
  return &data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* XBER::GetValue()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* XBER::GetValue()
{
  return &value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBER::GetLevel()
* @brief      Get level
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::GetLevel()
{
  int c = 0; 

  while(levels[c])
    {  
      c++;
      if(c >= XBER_MAXLEVELS)
        {
          break;
        }                                      
    }

  return c;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD* XBER::GetLevels()
* @brief      Get levels
* @ingroup    XUTILS
* 
* @return     XDWORD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD* XBER::GetLevels()
{
  return levels;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::GetLevels(XSTRING& string)
* @brief      Get levels
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::GetLevels(XSTRING& string)
{
  string.Empty();

  int c=0; 
  while(levels[c])
    {
      string.AddFormat(__L("%d"), levels[c]);

      c++;
      if(c >= XBER_MAXLEVELS)
        {
          break;
        }
                                  
      if(levels[c])
        {
          string.AddFormat(__L("."));
        }
    }

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::GetDump(XBUFFER& xbuffer, bool notheader)
* @brief      Get dump
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  notheader : Notheader value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::GetDump(XBUFFER& xbuffer, bool notheader)
{
  if(!notheader)
    {
      xbuffer.Add((XBYTE)tagtype);

      XBUFFER sizedata;
      if(CodeSize(size, sizedata) && tagtype != XBER_TAGTYPE_NULL) 
        {
          xbuffer.Add(&sizedata);
        }
    }

  if(data.GetSize()) 
    {
      xbuffer.Add(data.Get(), data.GetSize());
    }

  for(XDWORD c=0;c<sequences.GetSize();c++)
    {
      XBER* xber = (XBER*)sequences.Get(c);
      if(xber) 
        {
          xber->GetDump(xbuffer);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetFromDump(XBUFFER& buffer, XOBSERVER* observer)
* @brief      Set from dump
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  observer : Observer pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetFromDump(XBUFFER& buffer, XOBSERVER* observer)
{
  totalposition = 0;

  level = 0;
  memset(levels, 0, sizeof(XDWORD)*XBER_MAXLEVELS);
  
  if(observer)
    {
      observer->SubscribeEvent(XBERXEVENT_TYPE_DECODE_START , this, observer);
      observer->SubscribeEvent(XBERXEVENT_TYPE_DECODE_END, this, observer);

      XBER_XEVENT xevent(this, XBERXEVENT_TYPE_DECODE_START); 

      xevent.SetLevel(0);   
      xevent.GetData()->CopyFrom(buffer);       
      xevent.SetStatus(buffer.IsEmpty()?false:true);

      PostEvent(&xevent);  
    }


  bool status = SetFromDumpInternal(buffer, observer);

  if(observer)
    {
      XBER_XEVENT xevent(this, XBERXEVENT_TYPE_DECODE_END); 

      xevent.SetLevel(0);  
      xevent.GetData()->CopyFrom(buffer);             
      xevent.SetStatus(status);

      PostEvent(&xevent);  

      observer->UnSubscribeEvent(XBERXEVENT_TYPE_DECODE_START , this, observer);
      observer->UnSubscribeEvent(XBERXEVENT_TYPE_DECODE_END, this, observer);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetTagType(XBYTE tagtype)
* @brief      Set tag type
* @ingroup    XUTILS
* 
* @param[in]  tagtype : Tagtype value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetTagType(XBYTE tagtype)
{
  this->tagtype = tagtype;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetSize(XDWORD size)
* @brief      Set size
* @ingroup    XUTILS
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetSize(XDWORD size)
{
  this->size = size;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetNULL()
* @brief      Set NULL
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetNULL()
{
  data.Delete();
  data.Add((XBYTE)0);

  tagtype = XBER_TAGTYPE_NULL;
  size    = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetINTEGER(XDWORD integer)
* @brief      Set INTEGER
* @ingroup    XUTILS
* 
* @param[in]  integer : integer to assign
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetINTEGER(XDWORD integer)
{ 
  data.Delete();

  XDWORD size = 0;

  if(integer&0xFF800000)
    {
      size = 4;
    }
   else
    {
      if(integer&0x00FF8000)
        {
          size = 3;
        }
       else
        {
          if(integer&0x0000FF80)
            {
              size = 2;
            }
           else
            {
              size = 1;
            }
        }
    }

  if(!size) return false;

  for(int c=(size-1);c>=0;c--)
    {
      XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

      data.Add((XBYTE)byte);
    }

  tagtype = XBER_TAGTYPE_INTEGER;
  this->size = size;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetINTEGER(XBUFFER& xbufferdata)
* @brief      Set INTEGER
* @ingroup    XUTILS
* 
* @param[in]  xbufferdata : buffer to assign
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetINTEGER(XBUFFER& xbufferdata)
{
  data.Delete();

  data.Add(xbufferdata);

  tagtype = XBER_TAGTYPE_INTEGER;
  size    = xbufferdata.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetBITSTRING(XBUFFER& xbuffer)
* @brief      Set BITSTRING
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : buffer to assign
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBUFFER& xbuffer)
{
  if(!xbuffer.GetSize()) return false;
  
  data.Delete();

  data.Add(xbuffer);

  tagtype = XBER_TAGTYPE_BIT_STRING;
  size    = xbuffer.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetBITSTRING(XBYTE* buffer, XDWORD size)
* @brief      Set BITSTRING
* @ingroup    XUTILS
* 
* @param[in]  buffer : pointer to buffer bytes
* @param[in]  size : size buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBYTE* buffer, XDWORD size)
{
  if(!buffer) return false;
  if(!size)   return false;
  
  data.Delete();

  data.Add(buffer, size);

  tagtype = XBER_TAGTYPE_BIT_STRING;
  this->size    = size;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetOCTETSTRING(XCHAR* string)
* @brief      Set OCTETSTRING
* @ingroup    XUTILS
* 
* @param[in]  string : string with octets
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XCHAR* string)
{
  if(!string)    return false;
  if(!string[0]) return false;
  
  data.Delete();

  XSTRING _string;

  _string = string;

  XBUFFER charstr;
  
  _string.ConvertToASCII(charstr);
 
  data.Add((XBYTE*)charstr.Get(), _string.GetSize());
  
  tagtype = XBER_TAGTYPE_OCTET_STRING;
  size    = _string.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetOCTETSTRING(XBUFFER& xbuffer)
* @brief      Set OCTETSTRING
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : buffer with octets
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBUFFER& xbuffer)
{
  if(!xbuffer.GetSize()) return false;
  
  data.Delete();

  data.Add(xbuffer);

  tagtype = XBER_TAGTYPE_OCTET_STRING;
  size    = xbuffer.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetOCTETSTRING(XBYTE* buffer, XDWORD size)
* @brief      Set OCTETSTRING
* @ingroup    XUTILS
* 
* @param[in]  buffer : pointer buffer with octets
* @param[in]  size : size of buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBYTE* buffer, XDWORD size)
{
  if(!buffer) return false;
  if(!size)   return false;
  
  data.Delete();

  data.Add(buffer, size);

  tagtype = XBER_TAGTYPE_OCTET_STRING;
  this->size    = size;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetOID(XCHAR* OIDstring)
* @brief      Set OID
* @ingroup    XUTILS
* 
* @param[in]  OIDstring : OID to assign
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOID(XCHAR* OIDstring)
{
  if(!OIDstring)    return false;
  if(!OIDstring[0]) return false;
  
  data.Delete();

  XSTRING       string;
  XVECTOR<int>  numbers;
  int           start = 0;
  int           end   = 0;

  string = OIDstring;

  while(1)
    {
      XSTRING numberstring;
      int     indexstring = string.Find(__L("."), true, start);

      if(indexstring != XSTRING_NOTFOUND)
        {
          end = indexstring;
          string.Copy(start, end, numberstring);
          numbers.Add(numberstring.ConvertToInt());
          start = end+1;
        }
       else
        {
          string.Copy(end+1, numberstring);
          numbers.Add(numberstring.ConvertToInt());

          break;
        }
    }

  if(!numbers.GetSize()) return false;

  //---------------------------------------------------
  // The firts 2 bytes are special
  //

  size_t _first = (size_t)(numbers.Get(0));
  int first = (int)_first;

  first *= (40);
  _first = (size_t)(numbers.Get(1));
  first += (int)(_first);

  data.Add((XBYTE)first);

  for(XDWORD c=2;c<numbers.GetSize();c++)
    {
      XBUFFER _data;

      size_t _number = (size_t)(numbers.Get(c));
      XDWORD number = (XDWORD)(_number);

      if(CodeBigNumber((XDWORD)(number), _data))
        {
          data.Add(_data.Get(), _data.GetSize());
        }
    }

  this->tagtype = XBER_TAGTYPE_OBJECT_IDENTIFIER;
  this->size    = data.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::Copy(XBER* newxber)
* @brief      Copy
* @ingroup    XUTILS
* 
* @param[in]  newxber : pointer to GEN_NEW BER to assign
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::Copy(XBER* newxber)
{
  if(!newxber)       return false;
 
  newxber->tagtype  = tagtype;
  newxber->size     = size;
 
  newxber->data.Delete();
  newxber->data.Add(data.Get(), data.GetSize());

  for(XDWORD c=0;c<sequences.GetSize();c++)
    {
      XBER* xbertmp = (XBER*)sequences.Get(c);
      if(xbertmp)
        {
          XBER* newxberseq = GEN_NEW XBER();
          if(newxberseq)
            {
              if(xbertmp->Copy(newxberseq))  newxber->sequences.Add(newxberseq);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::Sequence_AddTo(XBER& xber)
* @brief      Sequence add to
* @ingroup    XUTILS
* 
* @param[in]  xber : XBER to add in secuence
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::Sequence_AddTo(XBER& xber)
{
  data.Delete();

  XBER* newxber = GEN_NEW XBER();
  if(!newxber) return false;

  if(!xber.Copy(newxber))
    {
      GEN_DELETE newxber;
      return false;
    }

  sequences.Add(newxber);

  tagtype       = XBER_TAGTYPE_SEQUENCE | XBER_TAG_MASKISCONSTRUCTED;
  isconstructed = true;
  size          = 0;

  for(XDWORD c=0;c<sequences.GetSize();c++)
    {
      XBER* xber = (XBER*)sequences.Get(c);
      if(xber) size += xber->Sequence_GetSize();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBER*>* XBER::Sequence_GetSequences()
* @brief      Sequence get sequences
* @ingroup    XUTILS
* 
* @return     XVECTOR<XBER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBER*>* XBER::Sequence_GetSequences()
{
  return &sequences;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::Sequence_AddCommandTo(XBYTE command, XBER& xber)
* @brief      Sequence add command to
* @ingroup    XUTILS
* 
* @param[in]  command : Command value.
* @param[in]  xber : Xber value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::Sequence_AddCommandTo(XBYTE command, XBER& xber)
{ 
  data.Delete();

  xber.GetDump(data, true);

  this->tagtype = command;
  this->size    = data.GetSize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBER::Sequence_GetSize()
* @brief      Sequence get size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::Sequence_GetSize()
{
  XDWORD  sizeall = 0;

  // Size type
  if(tagtype != XBER_TAGTYPE_NULL) sizeall++;

  // Size of size;
  XBUFFER sizedata;

  CodeSize(size, sizedata);
  sizeall += sizedata.GetSize();

  sizeall += data.GetSize();

  for(XDWORD c=0;c<sequences.GetSize();c++)
    {
      XBER* xber = (XBER*)sequences.Get(c);
      if(xber) sizeall += xber->Sequence_GetSize();
    }

  return sizeall;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::Sequence_DeleteAll()
* @brief      Sequence GEN_DELETE all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::Sequence_DeleteAll()
{
  if(sequences.IsEmpty()) 
    {
      return false;
    }

  sequences.DeleteContents();

  sequences.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::SetFromDumpInternal(XBUFFER& buffer, XOBSERVER* observer, XBER* root)
* @brief      Set from dump internal
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  observer : Observer pointer to use.
* @param[in]  root : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetFromDumpInternal(XBUFFER& buffer, XOBSERVER* observer, XBER* root)
{
  static XASN1_OID_PROPERTY*  property       = NULL;
  XASN1_OID_PROPERTY*         propertyevent  = NULL;
  XSTRING                     line;
  XBER*                       rootber        = root?root:this;

  if(buffer.IsEmpty())
    {
      return false;
    }

  if(observer)
    {
      observer->SubscribeEvent(XBERXEVENT_TYPE_DECODE_DATA, this, observer);
    }

  rootber->levels[rootber->level]++;

  tagtype = (XBYTE)(buffer.Get()[0] & XBER_TAG_MASKTYPE); 

  if(!CalculeSize(buffer, size, sizehead)) 
    {
      return false;  
    }
  
  tagclass = XBER_TAG_CLASS(buffer.Get()[0]);

  switch(tagclass)
    {
      case XBER_TAGCLASS_UNIVERSAL	      : break;

      case XBER_TAGCLASS_APPLICATION	    : break;

      case XBER_TAGCLASS_CONTEXT_SPECIFIC	: contextspecificvalue = tagtype;
                                            tagtype = XBER_TAGTYPE_CONTEXT_SPECIFIC;
                                            break;

      case XBER_TAGCLASS_PRIVATE          :	break;

                                  default : break;
    } 

  GetTagTypeName(nametagtype);
  
  isconstructed =  ((buffer.Get()[0] & XBER_TAG_MASKISCONSTRUCTED) == XBER_TAG_MASKISCONSTRUCTED)?true:false;

  data.Delete();
  data.Add(&buffer.Get()[sizehead], size);

  //line.Format(__L("(%d, %d) %s"), totalposition, size, nametagtype.Get());  
  line.Format(__L("%s"), nametagtype.Get());  

  switch(tagtype)
   {   
     case XBER_TAGTYPE_CONTEXT_SPECIFIC    : line.AddFormat(__L(" (%d) "), contextspecificvalue);                                               
                                             break;

     case XBER_TAGTYPE_OBJECT_IDENTIFIER  : { XSTRING valuestr;
      
                                              ConvertToObjetIdentifier(data, value);
                                              value.ToString(valuestr);

                                              property        = XASN1::GetOIDProperty(valuestr.Get());
                                              propertyevent   = property;
                                              if(property)
                                                {
                                                  line.AddFormat(__L(": %s :"), property->description);  
                                                  
                                                  if(!property->isconstructed)
                                                    {
                                                      property = NULL;
                                                    }
                                                }
                                            }

                                            break;

     case XBER_TAGTYPE_BIT_STRING         : unusedbits = data.Get()[0];   
                                            data.Extract(NULL, 0, 1);                                            
                                            line.AddFormat(__L(" UnusedBits: %d "), unusedbits);                                                                           
                                            if(property)
                                              {
                                                isconstructed = property->isconstructed;
                                                if(isconstructed) rootber->totalposition++;
                                              }

                                            property = NULL;                                               
                                            break;

     case XBER_TAGTYPE_OCTET_STRING       : if(property)
                                              {
                                                isconstructed = property->isconstructed;  
                                              }

                                            property = NULL;                                            
                                            break;

                          default         : break;
   }

  if(isconstructed)  
    { 
      XBUFFER subdata;
      XBER*   sub_ber  = NULL; 
      XDWORD  position = 0;  

      rootber->level++;

      rootber->totalposition += sizehead;

      subdata.Delete();
      subdata.Add(data.Get(), data.GetSize());


      XBER_XEVENT event(this, XBERXEVENT_TYPE_DECODE_DATA);

      event.SetLevel(rootber->GetLevel());
      memcpy(event.GetLevels(), rootber->GetLevels(), sizeof(XDWORD)*XBER_MAXLEVELS);
      rootber->GetLevels((*event.GetLevelsString()));
      event.GetLine()->Set(line);
      event.GetData()->CopyFrom(buffer);
      event.SetTagType(tagtype);
      event.GetTagTypeName()->Set(nametagtype);
      event.SetTagClass(tagclass);
      event.SetProperty(propertyevent);
      event.GetValue()->Set();
      event.SetStatus(true);

      PostEvent(&event);  
                         
      while(subdata.GetSize())
        {                  
          sub_ber = GEN_NEW XBER();
          if(sub_ber) 
            {                                        
              if(!sub_ber->SetFromDumpInternal(subdata, observer, rootber))
                {           
                  GEN_DELETE sub_ber;    
                  sub_ber = NULL;

                  return false;  
                }
               else
                {                
                  sequences.Add(sub_ber);
                    
                  int addsize = sub_ber->GetSizeHead() + sub_ber->GetSize();    
                  
                  position += addsize;                                                                                     
                  
                  subdata.Extract(NULL, 0, addsize);
                }            
                
            } 
           else 
            {
              return false;
            }
        }
            
      rootber->levels[rootber->level] = 0;
      rootber->level--;
      
    } 
   else
    {       
      switch(tagtype)
        {  
          case XBER_TAGTYPE_CONTEXT_SPECIFIC    : break;

          case XBER_TAGTYPE_BOOLEAN	            : ConvertToBoolean(data, value);
                                                  break;

          case XBER_TAGTYPE_INTEGER	            : ConvertToInteger(data, value);
                                                  break;

          case XBER_TAGTYPE_BIT_STRING	        : ConvertToBitString(data, value);                                                  
                                                  break;

          case XBER_TAGTYPE_OCTET_STRING        : ConvertToOctetString(data, value);
                                                  break;

          case XBER_TAGTYPE_NULL                : value.Set();
                                                  break;

          case XBER_TAGTYPE_OBJECT_IDENTIFIER   : ConvertToObjetIdentifier(data, value);                                                  
                                                  break;

          case XBER_TAGTYPE_OBJECT_DESCRIPTOR   : break;
          case XBER_TAGTYPE_EXTERNAL            : break;
          case XBER_TAGTYPE_REAL                : break;
          case XBER_TAGTYPE_ENUMERATED          : break;
          case XBER_TAGTYPE_EMBEDDED_PDV        : break;
          case XBER_TAGTYPE_UTF8STRING          : break;
          case XBER_TAGTYPE_RELATIVE_OID        : break;
          case XBER_TAGTYPE_TIME                : break;
          case XBER_TAGTYPE_RESERVED            : break;
          case XBER_TAGTYPE_SEQUENCE            : break;
          case XBER_TAGTYPE_SET                 : break;
          case XBER_TAGTYPE_NUMERICSTRING       : break;

          case XBER_TAGTYPE_PRINTABLESTRING     : ConvertToPrintableString(data, value);
                                                  break;

          case XBER_TAGTYPE_T61STRING           : break;
          case XBER_TAGTYPE_VIDEOTEXSTRING      : break;
          case XBER_TAGTYPE_IA5STRING           : break;

          case XBER_TAGTYPE_UTCTIME             : ConvertToUTCTime(data, value);
                                                  break;

          case XBER_TAGTYPE_GENERALIZEDTIME     : break;
          case XBER_TAGTYPE_GRAPHICSTRING       : break;
          case XBER_TAGTYPE_VISIBLESTRING       : break;
          case XBER_TAGTYPE_GENERALSTRING       : break;
          case XBER_TAGTYPE_UNIVERSALSTRING     : break;
          case XBER_TAGTYPE_CHARACTER_STRING    : break;
          case XBER_TAGTYPE_BMPSTRING           : break;
          case XBER_TAGTYPE_DATE                : break;
          case XBER_TAGTYPE_TIME_OF_DAY         : break;
          case XBER_TAGTYPE_DATE_TIME           : break;
          case XBER_TAGTYPE_DURATION            : break;
          case XBER_TAGTYPE_OID_IRI             : break;
          case XBER_TAGTYPE_RELATIVE_OID_IRI    : break;
        }      

      if(!value.IsNull())
        {
          XSTRING valuestr;

          value.ToString(valuestr);

          if(!valuestr.IsEmpty())
            {     
              line.AddFormat(__L(" '%s' "), valuestr.Get());  
            }         
        }
   
      rootber->totalposition += sizehead + size;


      XBER_XEVENT event(this, XBERXEVENT_TYPE_DECODE_DATA);

      event.SetLevel(rootber->GetLevel());
      memcpy(event.GetLevels(), rootber->GetLevels(), sizeof(XDWORD)*XBER_MAXLEVELS);
      rootber->GetLevels((*event.GetLevelsString()));
      event.GetLine()->Set(line);
      event.GetData()->CopyFrom(data);
      event.SetTagType(tagtype);
      event.GetTagTypeName()->Set(nametagtype);
      event.SetTagClass(tagclass);
      event.SetProperty(propertyevent);
      (*event.GetValue()) = value;
      event.SetStatus(true);

      PostEvent(&event);         
    }

  if(observer)
    {
      observer->UnSubscribeEvent(XBERXEVENT_TYPE_DECODE_DATA, this, observer);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToBoolean(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to boolean
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToBoolean(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 
    
  string.Format(__L("%s"), data.Get()[0]?__L("True"):__L("False"));
  
  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToInteger(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to integer
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToInteger(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 

  if(data.GetSize() <= 4)
    {
      XDWORD integerdec = 0;

      for(int c=0; c<data.GetSize(); c++)
        {
          integerdec |= data.Get()[c];
          if(c != data.GetSize() -1) integerdec <<= 8;
        }   

      string.Format(__L("%d"), integerdec);
    }
   else
    { 
      for(int c=0; c<data.GetSize(); c++)
        {
          string.AddFormat(__L("%02X"), data.Get()[c]);
        }
    }
  
  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToOctetString(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to octet string
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToOctetString(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 

  for(int c=0; c<data.GetSize(); c++)
    {
      string.AddFormat(__L("%02X"), data.Get()[c]);
    }

  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToBitString(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to bit string
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToBitString(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 

  for(int c=0; c<data.GetSize(); c++)
    {
      string.AddFormat(__L("%02X"), data.Get()[c]);
    }

  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToNULL(XVARIANT& variant)
* @brief      Convert to NULL
* @ingroup    XUTILS
* 
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToNULL(XVARIANT& variant)
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToObjetIdentifier(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to objet identifier
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToObjetIdentifier(XBUFFER& data, XVARIANT& variant)
{  
  XSTRING string;      
  int     length  = data.GetSize();
  XBYTE*  bits    = data.Get();
 
  if(length < 2 && !(length == 1 && bits[0] == 0))
    {
      return false; 
    }

  string.Format(__L("%d.%d"), (data.Get()[0]/40), (data.Get()[0]%40));
   
  size_t i = 0;
  while(i != length - 1)
    {
      XDWORD component = 0;

      while(i != length - 1)
        {
          ++i;
 
          if(component >> (32-7))
            {
              return false;      
            }
 
          component = (component << 7) + (bits[i] & 0x7F);
 
          if(!(bits[i] & 0x80))
            {
              break;
            }
        }

      string.AddFormat(__L(".%d"), component);

    }

  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToPrintableString(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to printable string
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToPrintableString(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 

  string.Add(data);

  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::ConvertToUTCTime(XBUFFER& data, XVARIANT& variant)
* @brief      Convert to UTC time
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::ConvertToUTCTime(XBUFFER& data, XVARIANT& variant)
{
  XSTRING string; 

  string.Add(data);
  //string.Add(__L("Z"));

  variant = string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::CalculeSize(XBUFFER& buffer, XDWORD& sizedataber, XBYTE& sizehead)
* @brief      Calcule size
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  sizedataber : Sizedataber value.
* @param[in]  sizehead : Sizehead value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::CalculeSize(XBUFFER& buffer, XDWORD& sizedataber, XBYTE& sizehead)
{  
  int mode = -1;
  
  if(buffer.IsEmpty()) return false;

  if(!(buffer.Get()[1] & 0x80))
    {
      if(!(buffer.Get()[1]))
        {
          if(tagtype == XBER_TAGTYPE_NULL)            
                 mode = 0;             
            else mode = 3;
            
        } else mode  = 0;

    } else mode = 2;  
    

  switch(mode)
    {
      case  0 : // Short mode              
                sizedataber = buffer.Get()[1];              
                sizehead    = 2;
                break;

      case  2 : // Long mode;
                { 
                  int sizeofsize = (buffer.Get()[1] & 0x7F);
                  sizehead       = 2 + sizeofsize;

                  if(sizeofsize > 4)  return false;
                  if(!sizeofsize)     return false;                    
                
                  memcpy((XBYTE*)&sizedataber, (XBYTE*)&buffer.Get()[2], sizeofsize);   
                  SWAPDWORD(sizedataber);

                  XBYTE bitsrotate = (32-(sizeofsize*8));
                  sizedataber >>= bitsrotate; 
                }
                break;

      case  3 : // Indefinite mode;                               
                sizehead    = 2;
                sizedataber = 0;
                /*
                for(XDWORD c=2; c<data.GetSize()-1; c++)
                  {
                    if(!data.Get()[c] && !data.Get()[c+1])  
                      {
                        sizedataber++;
                      }
                  }  
                */
                break;

      default : break;

    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::CodeBigNumber(XDWORD number, XBUFFER& data)
* @brief      Code big number
* @ingroup    XUTILS
* 
* @param[in]  number : number to code
* @param[in]  data : buffer big numer coded
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::CodeBigNumber(XDWORD number, XBUFFER& data)
{
  data.Delete();

  int value = number;
  int length = 0;

  if(value >= (268435456))
    { // 2 ^ 28
      length = 5;
    }
   else
    {
      if(value >= (2097152))
        { // 2 ^ 21
          length = 4;
        }
       else
        {
          if(value >= 16384)
            { // 2 ^ 14
              length = 3;
            }
           else
            {
              if(value >= 128)
                { // 2 ^ 7
                  length = 2;
                }
               else
                {
                  length = 1;
                }
            }
        }
    }


  int j = 0;
  for(j = length - 1; j >= 0; j--)
   {
     if(j)
      {
        int p = ((value >> (7 * j)) & 0x7F) | 0x80;
        data.Add((XBYTE)p);
      }
     else
      {
        int p = ((value >> (7 * j)) & 0x7F);
        data.Add((XBYTE)p);
      }
    }

  return true;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBER::CodeSize(XDWORD integer, XBUFFER& data)
* @brief      Code size
* @ingroup    XUTILS
* 
* @param[in]  integer : integer to code
* @param[in]  data : buffer size coded
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBER::CodeSize(XDWORD integer, XBUFFER& data)
{
  data.Delete();

  XBYTE size = 0;

  if(integer&0xFF800000)
    {
      size = 4;
    }
   else
    {
      if(integer&0x00FF8000)
        {
          size = 3;
        }
       else
        {
          if(integer&0x0000FF80)
            {
              size = 2;
            }
           else
            {
              size = 1;
            }
        }
    }

  if(!size) return false;

  if(size>1) data.Add((XBYTE)((size-1)|0x80));

  for(int c=(size-data.GetSize()-1);c>=0;c--)
    {
      XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

      data.Add((XBYTE)byte);
    }

  return true;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBER::Clean()
{
  tagtype               = 0;  
  nametagtype.Empty();
  tagclass              = XBER_TAGCLASS_UNIVERSAL;

  sizehead              = 0;
  size                  = 0;

  data.Empty();

  isconstructed         = false;

  contextspecificvalue  = 0;
  unusedbits            = 0;

  totalposition         = 0;
  level                 = 0;
  memset(levels, 0, sizeof(XDWORD)*XBER_MAXLEVELS);

  value.Destroy();

  sequences.DeleteContents();
  sequences.DeleteAll();
}



