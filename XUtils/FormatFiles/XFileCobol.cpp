/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileCobol.cpp
* 
* @class      XFILECOBOL
* @brief      eXtended Utils Cobol file class 
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

#include "XFileCobol.h"

#include <string.h>
#include <stdarg.h>

#include "XFactory.h"
#include "XPath.h"
#include "XFile.h"
#include "XFileTXT.h"
#include "XVariant.h"
#include "XDateTime.h"
#include "XSystem.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_PIC::XFILECOBOL_PIC()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_PIC::XFILECOBOL_PIC()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_PIC::~XFILECOBOL_PIC()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_PIC::~XFILECOBOL_PIC()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_PIC_TYPE XFILECOBOL_PIC::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_PIC_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_PIC_TYPE XFILECOBOL_PIC::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_PIC::GetMask()
* @brief      Get mask
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_PIC::GetMask()
{
  return &mask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_PIC::GetMaskDecondensed()
* @brief      Get mask decondensed
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_PIC::GetMaskDecondensed()
{
  return &maskdecondensed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_PIC::ResolveMask()
* @brief      Resolve mask
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_PIC::ResolveMask()
{
  if(mask.IsEmpty())
    {
      XTRACE_PRINTCOLOR(4, _L("Cobol Reader: Error PIC Missing"));
      return false;
    }

  hassign    = false;
  ndecimals  = 0;
  sizedata   = 0;

  XDWORD index      = 0;

  while(index < mask.GetSize())
    {
      switch(mask.Get()[index])
        {
          case _C('X')   : type = XFILECOBOL_PIC_TYPE_BINARY;
                            maskdecondensed += _C('X');
                            sizedata++;
                            index++;
                            break;

          case _C('A')   : type = XFILECOBOL_PIC_TYPE_ALPHA;
                            maskdecondensed += _C('A');
                            sizedata++;
                            index++;
                            break;

          case _C('9')   : if(type != XFILECOBOL_PIC_TYPE_DECIMAL) type = XFILECOBOL_PIC_TYPE_NUMERIC;
                            if(type == XFILECOBOL_PIC_TYPE_DECIMAL) ndecimals++;
                            maskdecondensed += _C('9');
                            sizedata++;
                            index++;
                            break;

          case _C(',')   : type = XFILECOBOL_PIC_TYPE_DECIMAL;
                            maskdecondensed += _C(',');
                            sizedata++;
                            index++;
                            break;

          case _C('S')   : hassign = true;
                            maskdecondensed += _C('S');
                            index++;
                            break;

          case _C('(')   : { XSTRING modificator;
                              int     sizemodificator;

                              int end = mask.Find(_L(")"), true, index+1);
                              if(end == XSTRING_NOTFOUND)
                                {
                                  XTRACE_PRINTCOLOR(4, _L("Cobol Reader: Error PIC invalid %s. Missing ')'"), mask.Get());
                                  return false;
                                }

                             mask.Copy(index, end+1, modificator);
                             modificator.UnFormat(_L("(%d)"), &sizemodificator);

                             if(!sizemodificator)
                               {
                                 XTRACE_PRINTCOLOR(4, _L("Cobol Reader: Error PIC invalid %s. Modificador is zero '(0)'"), mask.Get());
                                 return false;
                               }

                             for(int c=0; c<sizemodificator-1; c++)
                                {
                                  maskdecondensed += mask.Get()[index-1];
                                  sizedata++;
                                  if(type == XFILECOBOL_PIC_TYPE_DECIMAL) ndecimals++;
                                }

                             index = end;
                           }
                           break;

          case _C(')')  : index++;
                           break;

          case _C('V')  : type = XFILECOBOL_PIC_TYPE_DECIMAL;
                           maskdecondensed += _C('V');
                           index++;
                           break;

                 default : XTRACE_PRINTCOLOR(4, _L("Cobol Reader: Error PIC invalid %s. Unknown Specifier '%c'"), mask.Get(), mask.Get()[index]);
                           return false;
                           break;
        }
    }

   switch(computationaltype)
     {
        case 1  : // Float point 4 Bytes
                  break;

        case 2  : // Float Point 8 Bytes
                  break;

        case 3  : // BCD + Sing
                  sizedata = ((sizedata + 1) / 2) + (((sizedata + 1) % 2)?1:0);
                  break;

        case 4  : break;

        case 5  : // Conversion Packet number standard binary data
                  if((sizedata >= 1) && (sizedata <= 2))
                    {
                      sizedata = 1;
                    }
                   else
                    {
                      if((sizedata >=3) && (sizedata <= 4))
                        {
                          sizedata = 2;
                        }
                       else
                        {
                          if((sizedata >= 5) && (sizedata <= 9))
                            {
                              sizedata = 4;
                            }
                           else
                            {
                              if((sizedata >= 10) && (sizedata <= 18))
                                {
                                  sizedata = 8;
                                }
                               else
                                {
                                  XTRACE_PRINTCOLOR(4, _L("Cobol Reader: Error PIC invalid %s. Size of modificator to long %s > 18 "), mask.Get(), sizedata);
                                  return false;
                                }
                            }
                        }
                    }
                  break;
     }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECOBOL_PIC::GetSizeData()
* @brief      Get size data
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECOBOL_PIC::GetSizeData()
{
  return sizedata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_PIC::GetNDecimals()
* @brief      Get N decimals
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_PIC::GetNDecimals()
{
  return ndecimals;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_PIC::HasSign()
* @brief      Has sign
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_PIC::HasSign()
{
  return hassign;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_PIC::GetComputationalType()
* @brief      Get computational type
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_PIC::GetComputationalType()
{
  return computationaltype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_PIC::SetComputationalType(int computationaltype)
* @brief      Set computational type
* @ingroup    XUTILS
* 
* @param[in]  computationaltype : Computationaltype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_PIC::SetComputationalType(int computationaltype)
{
  this->computationaltype = computationaltype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_PIC::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_PIC::Clean()
{
  type                =  XFILECOBOL_PIC_TYPE_UNKNOWN;
  mask.Empty();
  sizedata            = 0;
  computationaltype   = 0;
  ndecimals           = 0;
  hassign             = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD::XFILECOBOL_FIELD()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD::XFILECOBOL_FIELD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD::~XFILECOBOL_FIELD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD::~XFILECOBOL_FIELD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD_TYPE XFILECOBOL_FIELD::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_FIELD_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD_TYPE XFILECOBOL_FIELD::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::SetType(XFILECOBOL_FIELD_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::SetType(XFILECOBOL_FIELD_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_FIELD::GetLevel()
* @brief      Get level
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_FIELD::GetLevel()
{
  return level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::SetLevel(int level)
* @brief      Set level
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::SetLevel(int level)
{
  this->level = level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_FIELD::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_FIELD::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_PIC* XFILECOBOL_FIELD::GetPIC()
* @brief      Get PIC
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_PIC* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_PIC* XFILECOBOL_FIELD::GetPIC()
{
  return &PIC;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_FIELD::GetNOccurs()
* @brief      Get N occurs
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_FIELD::GetNOccurs()
{
  return noccurs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::SetNOccurs(int noccurs)
* @brief      Set N occurs
* @ingroup    XUTILS
* 
* @param[in]  noccurs : Noccurs value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::SetNOccurs(int noccurs)
{
  this->noccurs = noccurs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_FIELD::IsRedefine()
* @brief      Is redefine
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_FIELD::IsRedefine()
{
  return isredefine;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::SetIsRedefine(bool isredefine)
* @brief      Set is redefine
* @ingroup    XUTILS
* 
* @param[in]  isredefine : Isredefine value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::SetIsRedefine(bool isredefine)
{
  this->isredefine = isredefine;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_FIELD::GetRedefineName()
* @brief      Get redefine name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_FIELD::GetRedefineName()
{
  return &redefinename;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_FIELD::GetRedefineOffset()
* @brief      Get redefine offset
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_FIELD::GetRedefineOffset()
{
  return redefineoffset;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::SetRedefineOffset(int redefineoffset)
* @brief      Set redefine offset
* @ingroup    XUTILS
* 
* @param[in]  redefineoffset : Redefineoffset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::SetRedefineOffset(int redefineoffset)
{
  this->redefineoffset = redefineoffset;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_FIELD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_FIELD::Clean()
{
  type            = XFILECOBOL_FIELD_TYPE_UNKNOWN;
  level           = 0;
  name.Empty();
  noccurs         = 0;

  isredefine      = false;
  redefinename.Empty();
  redefineoffset  = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_RECORD::XFILECOBOL_RECORD(XFILECOBOL_HANDLE* handle)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  handle : Handle pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_RECORD::XFILECOBOL_RECORD(XFILECOBOL_HANDLE* handle)
{
  Clean();

  this->handle = handle;

  XDWORD sizedata = handle->CalculateDataSize();
  data.Resize(sizedata);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_RECORD::~XFILECOBOL_RECORD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_RECORD::~XFILECOBOL_RECORD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_RECORD_TYPE XFILECOBOL_RECORD::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_RECORD_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_RECORD_TYPE XFILECOBOL_RECORD::GetType()
{
  if(!handle) return XFILECOBOL_RECORD_TYPE_UNKNOWN;

  return handle->GetTypeRecord();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::SetType(XFILECOBOL_RECORD_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::SetType(XFILECOBOL_RECORD_TYPE type)
{
  if(!handle) return false;

  handle->SetTypeRecord(type);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECOBOL_RECORD::GetNFields()
* @brief      Get N fields
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECOBOL_RECORD::GetNFields()
{
  if(!handle)               return 0;
  if(!handle->GetFDTree())  return 0;

  XDWORD nnodes = handle->GetFDTree()->GetNNodes();
  if(nnodes) nnodes--;

  return nnodes;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(int index)
* @brief      Get field
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     XFILECOBOL_FIELD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(int index)
{
  if(!handle)               return NULL;
  if(!handle->GetFDTree())  return NULL;

  XFILECOBOL_FD_TREENODEITERATOR  it(handle->GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(handle->GetFDTree()->End());

  int counterindex = 0;

  while(it != it_end)
    {
      XFILECOBOL_FIELD* field  = (*it);
      if(!field) break;

      if(counterindex >= index) return field;

      counterindex++;
      ++it;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(XCHAR* name)
* @brief      Get field
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     XFILECOBOL_FIELD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(XCHAR* name)
{
  if(!handle) return NULL;

  XFILECOBOL_FD_TREENODEITERATOR it(handle->GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR it_end(handle->GetFDTree()->End());

  while(it != it_end)
    {
      XFILECOBOL_FIELD* field = (*it);
      if(!field) break;

      if(!field->GetName()->Compare(name, true)) return field;

      ++it;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(XFILECOBOL_FD_TREENODEITERATOR& it)
* @brief      Get field
* @ingroup    XUTILS
* 
* @param[in]  it : It value.
* 
* @return     XFILECOBOL_FIELD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD* XFILECOBOL_RECORD::GetField(XFILECOBOL_FD_TREENODEITERATOR& it)
{
  XFILECOBOL_FIELD* field = (*it);

  return field;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(int index, XVARIANT& variant)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(int index, XVARIANT& variant)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(index, it)) return false;

  return GetFieldData(it, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XVARIANT& variant)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XVARIANT& variant)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(name, it)) return false;

  return GetFieldData(it, variant);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XVARIANT& variant)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  it : It value.
* @param[in]  variant : Variant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XVARIANT& variant)
{
  XBUFFER           data;
  XFILECOBOL_FIELD* field = GetField(it);

  if(!field) return false;

  if(!GetFieldData(it, data)) return false;

  bool status = false;
  if(data.GetSize()) status = ConvertDataTo(field->GetPIC(), data, variant);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(int index, XSTRING& datastring)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  datastring : Datastring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(int index, XSTRING& datastring)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(index, it)) return false;

  return GetFieldData(it, datastring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XSTRING& datastring)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  datastring : Datastring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XSTRING& datastring)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(name, it)) return false;

  return GetFieldData(it, datastring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XSTRING& datastring)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  it : It value.
* @param[in]  datastring : Datastring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XSTRING& datastring)
{
  XBUFFER           data;
  XFILECOBOL_FIELD* field = GetField(it);

  if(!field)                  return false;
  if(!GetFieldData(it, data)) return false;

  bool status = false;
  if(data.GetSize()) status = ConvertDataTo(field->GetPIC(), data, datastring);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(int index, XBUFFER& data)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(int index, XBUFFER& data)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(index, it)) return false;

  return GetFieldData(it, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XBUFFER& data)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XCHAR* name, XBUFFER& data)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FD_TREENODEITERATOR  it((*handle->GetFDTree()), NULL);
  if(!handle->GetInterator(name, it)) return false;

  return GetFieldData(it, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XBUFFER& data)
* @brief      Get field data
* @ingroup    XUTILS
* 
* @param[in]  it : It value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::GetFieldData(XFILECOBOL_FD_TREENODEITERATOR& it, XBUFFER& data)
{
  if(!handle)               return false;
  if(!handle->GetFDTree())  return false;

  XFILECOBOL_FIELD* field = (*it);
  if(!field)                           return false;
  if(!field->GetPIC())                 return false;
  if(!field->GetPIC()->GetSizeData())  return false;

  int position = handle->CalculatePositionData(it);
  if(position == -1) return false;

  data.Resize(field->GetPIC()->GetSizeData());
  this->data.Get(data.Get(), data.GetSize(), position);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* XFILECOBOL_RECORD::GetData()
* @brief      Get data
* @ingroup    XUTILS
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XFILECOBOL_RECORD::GetData()
{
  return &data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::ConvertDataTo(XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XVARIANT& datavariant)
* @brief      Convert data to
* @ingroup    XUTILS
* 
* @param[in]  picture : Picture pointer to use.
* @param[in]  bufferdata : Bufferdata value.
* @param[in]  datavariant : Datavariant value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::ConvertDataTo(XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XVARIANT& datavariant)
{
  if(!bufferdata.GetSize()) return false;


  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_RECORD::ConvertDataTo(XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XSTRING& datastring)
* @brief      Convert data to
* @ingroup    XUTILS
* 
* @param[in]  picture : Picture pointer to use.
* @param[in]  bufferdata : Bufferdata value.
* @param[in]  datastring : Datastring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_RECORD::ConvertDataTo(XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XSTRING& datastring)
{
  datastring.Empty();

  if(!picture)              return false;
  if(!picture->GetMask())   return false;
  if(!bufferdata.GetSize()) return false;

  XSTRING mask;

  mask = picture->GetMaskDecondensed()->Get();
  if(!mask.GetSize()) return false;

  int indexdata = 0;

  switch(picture->GetComputationalType())
    {
      case  0 : for(XDWORD c=0; c<mask.GetSize(); c++)
                  {
                    switch(mask.Get()[c])
                      {
                        case _C('S') : { XBYTE lastdata = bufferdata.GetLastByte();
                                          if(lastdata & XFILECOBOL_MASK_SIGN)
                                            {
                                              datastring += _L("-");
                                              lastdata &= ~XFILECOBOL_MASK_SIGN;

                                              bufferdata.Get()[bufferdata.GetSize()-1] = lastdata;
                                            }
                                        }
                                        break;

                        case _C('9') :
                        case _C('X') :
                        case _C('A') : datastring.AddFormat(_L("%c"), bufferdata.Get()[indexdata]);
                                        indexdata++;
                                        break;

                        case _C('V') :
                        case _C('.') : datastring += _L(".");
                                        break;
                      }
                  }
                break;

      case  1 : break;

      case  3 : { XBYTE lastdata = bufferdata.GetLastByte();
                  lastdata &= 0x0F;

                  switch(lastdata)
                    {
                      case 0x0C : break;
                      case 0x0D : datastring = _L("-");
                                  break;
                      case 0x0F : break;
                    }

                  lastdata>>=0x4;

                  for(XDWORD c=0; c< bufferdata.GetSize()-1; c++)
                    {
                      datastring.AddFormat(_L("%02X"), bufferdata.Get()[c]);
                    }

                  datastring.AddFormat(_L("%1X"), lastdata);
                }
                break;

      case  5 : switch(bufferdata.GetSize())
                  {
                    case 1 : { char byte;

                               bufferdata.Get((XBYTE&)byte);
                               datastring.Format(_L("%02d"), byte);
                             }
                             break;

                    case 2 : { short word;

                               bufferdata.Get((XWORD&)word);
                               datastring.Format(_L("%04d"), word);
                             }
                             break;

                    case 4 : { int integer;

                               bufferdata.Get((XDWORD&)integer);
                               datastring.Format(_L("%08d"), integer);
                             }
                             break;

                    case 8 : { long long large;

                               bufferdata.Get((XQWORD&)large);
                               datastring.Format(_L("%016d"), large);
                             }
                             break;
                  }
                break;

    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_RECORD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_RECORD::Clean()
{
  handle = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_HANDLE::XFILECOBOL_HANDLE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_HANDLE::XFILECOBOL_HANDLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_HANDLE::~XFILECOBOL_HANDLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_HANDLE::~XFILECOBOL_HANDLE()
{
  fdtree.DeleteContents();
  fdtree.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_HANDLE::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_HANDLE::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_HANDLE::GetValueLabel()
* @brief      Get value label
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_HANDLE::GetValueLabel()
{
  return &valuelabel;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECOBOL_HANDLE::GetDataRecord()
* @brief      Get data record
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECOBOL_HANDLE::GetDataRecord()
{
  return &datarecord;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FD_TREE* XFILECOBOL_HANDLE::GetFDTree()
* @brief      Get FD tree
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_FD_TREE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FD_TREE* XFILECOBOL_HANDLE::GetFDTree()
{
  return &fdtree;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_HANDLE::GetIndexNodeForName(XFILECOBOL_FD_TREENODE* node, XCHAR* name)
* @brief      Get index node for name
* @ingroup    XUTILS
* 
* @param[in]  node : Node pointer to use.
* @param[in]  name : Name to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_HANDLE::GetIndexNodeForName(XFILECOBOL_FD_TREENODE* node, XCHAR* name)
{
  if(!node) return NOTFOUND;

  int index = NOTFOUND;

  for(XDWORD c=0; c<node->GetChildren()->GetSize(); c++)
    {
      XFILECOBOL_FD_TREENODE* subnode = node->GetChildren()->Get(c);
      if(subnode)
        {
          XFILECOBOL_FIELD* field = subnode->GetData();
          if(field)
            {
              if(!field->GetName()->Compare(name)) return (int)c;
            }
        }
    }

  return index;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_RECORD_TYPE XFILECOBOL_HANDLE::GetTypeRecord()
* @brief      Get type record
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_RECORD_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_RECORD_TYPE  XFILECOBOL_HANDLE::GetTypeRecord()
{
  return typerecord;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_HANDLE::SetTypeRecord(XFILECOBOL_RECORD_TYPE type)
* @brief      Set type record
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_HANDLE::SetTypeRecord(XFILECOBOL_RECORD_TYPE type)
{
  typerecord = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_HANDLE::GetInterator(XDWORD index, XFILECOBOL_FD_TREENODEITERATOR& it)
* @brief      Get interator
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  it : It value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_HANDLE::GetInterator(XDWORD index, XFILECOBOL_FD_TREENODEITERATOR& it)
{
  if(!GetFDTree()) return false;

  XFILECOBOL_FD_TREENODEITERATOR  it_start(GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(GetFDTree()->End());

  XDWORD counterindex = 0;

  it = it_start;

  while(it_start != it_end)
    {
      if(counterindex >= index)
        {
          it = it_start;
          return true;
        }

      counterindex++;

      ++it_start;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL_HANDLE::GetInterator(XCHAR* name, XFILECOBOL_FD_TREENODEITERATOR& it)
* @brief      Get interator
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* @param[in]  it : It value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL_HANDLE::GetInterator(XCHAR* name, XFILECOBOL_FD_TREENODEITERATOR& it)
{
  if(!GetFDTree()) return false;

  XFILECOBOL_FD_TREENODEITERATOR  it_start(GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(GetFDTree()->End());

  it = it_start;

  while(it_start != it_end)
    {
      XFILECOBOL_FIELD* field = (*it_start);
      if(!field) return false;

      if(!field->GetName()->Compare(name, true))
        {
          it = it_start;
          return true;
        }

      ++it_start;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECOBOL_HANDLE::CalculateDataSizeNode(XFILECOBOL_FD_TREENODE* node)
* @brief      Calculate data size node
* @ingroup    XUTILS
* 
* @param[in]  node : Node pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECOBOL_HANDLE::CalculateDataSizeNode(XFILECOBOL_FD_TREENODE* node)
{
  static XDWORD redefineoffset = 0;
  XDWORD size     = 0;
  if(!node) return size;

  if(node->GetData()->IsRedefine())
    {
      node->GetData()->SetRedefineOffset(redefineoffset);

      redefineoffset  = node->GetData()->GetPIC()->GetSizeData();

      for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
        {
          XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
          if(subnode)
            {
              subnode->GetData()->SetIsRedefine(true);
              CalculateDataSizeNode(subnode);
            }
        }
    }
   else
    {
      redefineoffset = 0;

      if(node->GetData()->GetPIC())
        {
          size = node->GetData()->GetPIC()->GetSizeData();
        }

      for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
        {
          XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
          if(subnode)
            {
              size +=  CalculateDataSizeNode(subnode);
            }
        }
    }

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECOBOL_HANDLE::CalculateDataSize()
* @brief      Calculate data size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECOBOL_HANDLE::CalculateDataSize()
{
  XDWORD size = 0;

  XFILECOBOL_FD_TREENODE* node = GetFDTree()->GetRoot();
  if(!node)  return size;

  for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
    {
      XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
      if(subnode) size =  CalculateDataSizeNode(subnode);
    }

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECOBOL_HANDLE::CalculatePositionData(XFILECOBOL_FD_TREENODEITERATOR& it)
* @brief      Calculate position data
* @ingroup    XUTILS
* 
* @param[in]  it : It value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECOBOL_HANDLE::CalculatePositionData(XFILECOBOL_FD_TREENODEITERATOR& it)
{
  if(!GetFDTree()) return -1;

  XFILECOBOL_FD_TREENODEITERATOR  it_start(GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(GetFDTree()->End());

  int        position            = 0;
  static int positionposredefine = 0;

  while(it_start != it_end)
    {
      XFILECOBOL_FIELD* fieldtmp  = (*it_start);
      if(!fieldtmp) break;

      if(fieldtmp)
        {
          if(fieldtmp->GetPIC())
            {
              if(!fieldtmp->IsRedefine())
                {
                  if(positionposredefine)
                    {
                      position            = positionposredefine;
                      positionposredefine = 0;
                    }

                  if(it_start == it) break;

                  position += fieldtmp->GetPIC()->GetSizeData();
                }
               else
                {
                  if(fieldtmp->GetRedefineName()->IsEmpty())
                    {
                      position += fieldtmp->GetRedefineOffset();

                      if(it_start == it) break;
                    }
                   else
                    {
                      XFILECOBOL_FD_TREENODEITERATOR itredefine(GetFDTree()->Begin());

                      if(GetInterator(fieldtmp->GetRedefineName()->Get(), itredefine))
                        {
                          position            = CalculatePositionData(itredefine);
                          positionposredefine = position + (*itredefine)->GetPIC()->GetSizeData();
                        }

                      if(it_start == it) break;
                    }
                }
            }
        }

      ++it_start;
    }

  return position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FIELD* XFILECOBOL_HANDLE::IsFieldRedefinedByAnother(XCHAR* name)
* @brief      Is field redefined by another
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     XFILECOBOL_FIELD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FIELD* XFILECOBOL_HANDLE::IsFieldRedefinedByAnother(XCHAR* name)
{
  if(!GetFDTree()) return NULL;

  XFILECOBOL_FD_TREENODEITERATOR  it_start(GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(GetFDTree()->End());

  while(it_start != it_end)
    {
      XFILECOBOL_FIELD* fieldtmp  = (*it_start);
      if(!fieldtmp) break;

      if(!fieldtmp->GetRedefineName()->IsEmpty())
        {
          if(!fieldtmp->GetRedefineName()->Compare(name))
            {
              return fieldtmp;
            }
        }

      ++it_start;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL_HANDLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL_HANDLE::Clean()
{
  typerecord =  XFILECOBOL_RECORD_TYPE_UNKNOWN;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL::XFILECOBOL()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL::XFILECOBOL()
{
  Clean();

  validchars = XFILECOBOL_VALIDCHARS;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL::~XFILECOBOL()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL::~XFILECOBOL()
{
  Close();

  CapabilitiesFile_UnLoad();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Open(int nfiles, XCHAR* xpathcpyfiles, XCHAR* xpathdatfiles, ...)
* @brief      Open
* @ingroup    XUTILS
* 
* @param[in]  nfiles : Nfiles value.
* @param[in]  xpathcpyfiles : Xpathcpyfiles pointer to use.
* @param[in]  xpathdatfiles : Xpathdatfiles pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Open(int nfiles, XCHAR* xpathcpyfiles, XCHAR* xpathdatfiles, ...)
{
  XPATH     xpath_cpy;
  va_list   arg;

  this->xpathcpyfiles      = xpathcpyfiles;
  this->xpathcpyfiles.Slash_Add();

  this->xpathdatfiles      = xpathdatfiles;
  this->xpathdatfiles.Slash_Add();

  if(capabilitiesfilemap.IsEmpty())
    {
      XPATH  xpathcapabilitiesfile;

      xpathcapabilitiesfile  = xpathcpyfiles;
      xpathcapabilitiesfile.Slash_Add();
      xpathcapabilitiesfile += XFILECOBOL_CAPABILITIESFILE;

      CapabilitiesFile_Load(xpathcapabilitiesfile);
    }

  va_start(arg, xpathdatfiles);

  handle.GetFDTree()->DeleteContents();
  handle.GetFDTree()->DeleteAll();

  for(int c=0; c<nfiles; c++)
    {
      XCHAR* namefile = va_arg(arg, XCHAR*);

      xpath_cpy = xpathcpyfiles;
      xpath_cpy.Slash_Add();
      xpath_cpy += namefile;

      XFILETXT*  file_fd = GEN_NEW XFILETXT();
      if(file_fd)
        {
          if(file_fd->Open(xpath_cpy, true))
            {
              file_fd->ReadAllFile();

              TraslateTextToHandle(file_fd->GetLines(), handle);

              file_fd->Close();
            }

          GEN_DELETE file_fd;
        }
    }

  va_end(arg);

  sizedatarecord = handle.CalculateDataSize();
  if(!sizedatarecord) return false;

  if(this->xpathdatafile.IsEmpty()) return false;


  XPATH xpathnamefile;

  this->xpathdatafile.GetNamefileExt(xpathnamefile);
  xpathnamefile.Insert(this->xpathdatfiles.Get(), 0);


  xfiledata=GEN_XFACTORY.Create_File();
  if(!xfiledata) return false;

  bool status = xfiledata->Open(xpathnamefile.Get());

  if(status)
    {
      xfiledata->SetPosition(0);
      if(xfiledata->Read((XBYTE*)&fileheadersize, sizeof(XDWORD)))
        {
          if(GEN_XSYSTEM.HardwareUseLittleEndian()) fileheadersize = SWAPDWORD(fileheadersize);

          switch(fileheadersize)
            {
              case 0x307E0000 : { fileheadersize = 0x80;
                                  SetHaveFileHeaders(true);
                                  type = XFILECOBOL_FILE_TYPE_MF4;
                                }
                                break;

              case 0x3000007C : { fileheadersize = 0x80;
                                  SetHaveFileHeaders(true);
                                  type = XFILECOBOL_FILE_TYPE_MF4;
                                }
                                break;

              case 0x300003FC : { fileheadersize = 0x80;
                                  SetHaveFileHeaders(true);
                                  type = XFILECOBOL_FILE_TYPE_C_ISAM;
                                }
                                break;

                    default   : { fileheadersize = 0x00;
                                  SetHaveFileHeaders(false);
                                  type = XFILECOBOL_FILE_TYPE_C_ISAM;
                                }
                                break;
            }

          if(fileheadersize)
            {
              XWORD ID = 0;

              status = false;

              xfiledata->SetPosition(36);
              if(xfiledata->Read((XBYTE*)&ID, sizeof(XWORD)))
                {
                  if(GEN_XSYSTEM.HardwareUseLittleEndian()) ID = SWAPWORD(ID);
                  if(ID == 0x003E)
                    {
                      xfiledata->SetPosition(54);
                      if(xfiledata->Read((XBYTE*)&maxfilerecordsize, sizeof(XDWORD)))
                        {
                          if(GEN_XSYSTEM.HardwareUseLittleEndian()) maxfilerecordsize = SWAPDWORD(maxfilerecordsize);

                          xfiledata->SetPosition(58);
                          if(xfiledata->Read((XBYTE*)&minfilerecordsize, sizeof(XDWORD)))
                            {
                              if(GEN_XSYSTEM.HardwareUseLittleEndian()) minfilerecordsize = SWAPDWORD(minfilerecordsize);

                              if(maxfilerecordsize < 4095) fileheaderrecordize = 2; else fileheaderrecordize = 4;

                              // Check if the size in the file header is diferent
                              if(maxfilerecordsize == handle.CalculateDataSize())
                                {
                                  status = true;
                                }

                              if(status) filerecordpadding = 16 - ((fileheaderrecordize + maxfilerecordsize) % 16);
                            }
                        }
                    }
                }
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::IsOpen()
* @brief      Is open
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::IsOpen()
{
  if(!xfiledata) return false;
  return xfiledata->IsOpen();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FILE_TYPE XFILECOBOL::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_FILE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FILE_TYPE XFILECOBOL::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL::SetType(XFILECOBOL_FILE_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL::SetType(XFILECOBOL_FILE_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::HaveFileHeaders()
* @brief      Have file headers
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::HaveFileHeaders()
{
  return havefileheaders;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL::SetHaveFileHeaders(bool havefileheaders)
* @brief      Set have file headers
* @ingroup    XUTILS
* 
* @param[in]  havefileheaders : Havefileheaders value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL::SetHaveFileHeaders(bool havefileheaders)
{
  this->havefileheaders = havefileheaders;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECOBOL::GetNRecords()
* @brief      Get N records
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECOBOL::GetNRecords()
{
  XQWORD size       = xfiledata->GetSize();
  XDWORD sizerecord = handle.CalculateDataSize();

  if(havefileheaders)
    {
      sizerecord += (filerecordpadding + fileheaderrecordize);
      size       -= fileheadersize;
    }

  if(!size || !sizerecord) return 0;

  return (XDWORD)(size / sizerecord);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::ReadRecord(XDWORD index, XFILECOBOL_RECORD& record)
* @brief      Read record
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  record : Record value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::ReadRecord(XDWORD index, XFILECOBOL_RECORD& record)
{
  if(!IsOpen()) return false;

  if(index<1) return false;

  int position = 0;

  if(havefileheaders)  position = fileheadersize;

  switch(record.GetType())
    {
      case XFILECOBOL_RECORD_TYPE_UNKNOWN           :
                                        default     :

      case XFILECOBOL_RECORD_TYPE_INDEXED           :
      case XFILECOBOL_RECORD_TYPE_LINE_SEQUENTIAL   :  if(havefileheaders)
                                                          {
                                                              position += ((index-1) * (sizedatarecord + fileheaderrecordize + filerecordpadding));
                                                              position += fileheaderrecordize;
                                                          }
                                                         else position += ((index-1) * (sizedatarecord + 1));

                                                      break;
      case XFILECOBOL_RECORD_TYPE_SEQUENTIAL        :
      case XFILECOBOL_RECORD_TYPE_RELATIVE          : break;
    }



  xfiledata->SetPosition((XDWORD)position);

  return xfiledata->Read(record.GetData()->Get(), record.GetData()->GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::ReadRecords(XVECTOR<XFILECOBOL_RECORD*>& records, XDWORD startindex)
* @brief      Read records
* @ingroup    XUTILS
* 
* @param[in]  records : Records pointer to use.
* @param[in]  startindex : Startindex value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::ReadRecords(XVECTOR<XFILECOBOL_RECORD*>& records, XDWORD startindex)
{

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Close()
* @brief      Close
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Close()
{
  xpathdatafile.Empty();

  handle.GetFDTree()->DeleteContents();
  handle.GetFDTree()->DeleteAll();

  if(xfiledata)
    {
      xfiledata->Close();

      GEN_XFACTORY.Delete_File(xfiledata);
      xfiledata = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_HANDLE* XFILECOBOL::GetHandle()
* @brief      Get handle
* @ingroup    XUTILS
* 
* @return     XFILECOBOL_HANDLE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_HANDLE* XFILECOBOL::GetHandle()
{
  return &handle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XFILECOBOL::GetPathDataFile()
* @brief      Get path data file
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XFILECOBOL::GetPathDataFile()
{
  return &xpathdatafile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::CapabilitiesFile_Load(XPATH& xpathcapabilitiesfile)
* @brief      Capabilities file load
* @ingroup    XUTILS
* 
* @param[in]  xpathcapabilitiesfile : Xpathcapabilitiesfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::CapabilitiesFile_Load(XPATH& xpathcapabilitiesfile)
{
  XFILETXT* filetxt;
  bool      status = false;

  CapabilitiesFile_UnLoad();

  filetxt = GEN_NEW XFILETXT();
  if(!filetxt) return false;

  if(filetxt->Open(xpathcapabilitiesfile))
    {
      filetxt->ReadAllFile();

      XVECTOR<XSTRING*>* lines = filetxt->GetLines();
      if(lines)
        {
          for(XDWORD c=0; c<lines->GetSize(); c++)
            {
              XSTRING* IDfile    = GEN_NEW XSTRING();
              XPATH*   xpathfile = GEN_NEW XPATH();

              if(IDfile && xpathfile)
                {
                  XSTRING* line  = lines->Get(c);
                  if(line)
                    {
                      if(!line->IsEmpty())
                        {
                          if(line->Get()[0] != _C('#'))
                            {
                              IDfile->AdjustSize(_MAXSTR);
                              xpathfile->AdjustSize(_MAXSTR);

                              line->UnFormat(_L("%s:%s"), IDfile->Get(), xpathfile->Get());

                              IDfile->AdjustSize();
                              xpathfile->AdjustSize();

                              if(!IDfile->IsEmpty() && !xpathfile->IsEmpty())
                                {
                                  capabilitiesfilemap.Add(IDfile, xpathfile);
                                  continue;
                                }
                            }
                        }
                    }
                }

              GEN_DELETE IDfile;
              GEN_DELETE xpathfile;
            }
        }

      filetxt->Close();
      status = true;
    }

  GEN_DELETE filetxt;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::CapabilitiesFile_UnLoad()
* @brief      Capabilities file un load
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::CapabilitiesFile_UnLoad()
{
  if(capabilitiesfilemap.IsEmpty()) return false;

  capabilitiesfilemap.DeleteKeyContents();
  capabilitiesfilemap.DeleteElementContents();
  capabilitiesfilemap.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::CapabilitiesFile_GetFile(XSTRING* fileID, XPATH& xpathfile)
* @brief      Capabilities file get file
* @ingroup    XUTILS
* 
* @param[in]  fileID : File ID pointer to use.
* @param[in]  xpathfile : Xpathfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::CapabilitiesFile_GetFile(XSTRING* fileID, XPATH& xpathfile)
{
  if(!fileID) return false;

  xpathfile.Empty();

  for(XDWORD c=0; c<capabilitiesfilemap.GetSize(); c++)
    {
      if(capabilitiesfilemap.GetKey(c))
        {
          if(!capabilitiesfilemap.GetKey(c)->Compare(fileID->Get(), true))
            {
              if(capabilitiesfilemap.GetElement(c))
                {
                  xpathfile = capabilitiesfilemap.GetElement(c)->Get();
                  break;
                }
            }
        }
    }

  return xpathfile.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::IsValidCharacter(XCHAR character)
* @brief      Is valid character
* @ingroup    XUTILS
* 
* @param[in]  character : Character value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::IsValidCharacter(XCHAR character)
{
  bool found = false;

  for(XDWORD c=0; c<validchars.GetSize(); c++)
    {
      if(validchars.Get()[c] == character) found = true;
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_InvalidCharacters(XSTRING& line)
* @brief      Eliminate invalid characters
* @ingroup    XUTILS
* 
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_InvalidCharacters(XSTRING& line)
{
  if(line.IsEmpty()) return false;

  XDWORD c = 0;
  while(c<line.GetSize())
    {
      if(!IsValidCharacter(line.Get()[c]))
        {
          line.DeleteCharacters(c, 1);

        } else c++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_TabsForSpaces(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate tabs for spaces
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_TabsForSpaces(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  XSTRING tabspaces;

  for(int c=0; c<XFILECOBOL_TABSFORSPACES; c++)
    {
      tabspaces += _L(" ");
    }

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(line)
        {
          int index = line->Find(_L("\x09"), true);
          if(index != XSTRING_NOTFOUND)
            {
              line->DeleteCharacters(index, 1);
              line->Insert(tabspaces, index);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_InvalidCharacters(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate invalid characters
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_InvalidCharacters(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(line) Eliminate_InvalidCharacters((*line));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_Comments(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate comments
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_Comments(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(line)
        {
          int index = line->Find(_L("*"), true);
          if(index != XSTRING_NOTFOUND) line->DeleteCharacters(index, line->GetSize()-index);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_NumberLines(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate number lines
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_NumberLines(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(line) line->DeleteCharacters(0, 7);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_EmptyLines(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate empty lines
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_EmptyLines(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  XDWORD c=0;
  while(c<lines->GetSize())
    {
      XSTRING* line = lines->Get(c);
      if(line)
        {
          line->DeleteCharacter(_C(' '), XSTRINGCONTEXT_FROM_FIRST);
          line->DeleteCharacter(_C(' '), XSTRINGCONTEXT_TO_END);
          if(line->IsEmpty())
            {
              GEN_DELETE line;
              lines->Delete(line);

            } else c++;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::Eliminate_UnnecessarySpaces(XVECTOR<XSTRING*>* lines)
* @brief      Eliminate unnecessary spaces
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::Eliminate_UnnecessarySpaces(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(line)
        {
          XDWORD d = 0;

          while(d<line->GetSize()-1)
            {
              if((line->Get()[d] == _C(' ')) && (line->Get()[d+1] == _C(' ')))
                {
                  line->DeleteCharacters(d, 1);

                } else d++;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::CreateUniqueLines(XVECTOR<XSTRING*>* lines)
* @brief      Create unique lines
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::CreateUniqueLines(XVECTOR<XSTRING*>* lines)
{
  if(!lines) return false;

  XDWORD c=0;
  while(c<lines->GetSize())
    {
      XSTRING* line = lines->Get(c);
      if(line)
        {
          XSTRING* linenext = lines->Get(c+1);
          if((linenext) && (line->Get()[line->GetSize()-1] != _C('.')))
            {
              line->Add(_C(' '));
              line->Add(linenext->Get());

              GEN_DELETE linenext;

              lines->Delete(linenext);

            } else c++;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::ExtractReservedToken(XSTRING& line, XCHAR* token, XSTRING& var)
* @brief      Extract reserved token
* @ingroup    XUTILS
* 
* @param[in]  line : Line value.
* @param[in]  token : Token pointer to use.
* @param[in]  var : Var value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::ExtractReservedToken(XSTRING& line, XCHAR* token, XSTRING& var)
{
  XSTRING mask;
  XSTRING strvar;

  mask.Format(_L("%s "), token);

  int start         = line.Find(mask.Get(), true);
  int startendtoken = 0;
  int end           = 0;
  if(start == XSTRING_NOTFOUND)  return false;

  startendtoken = start + mask.GetSize();

  end = line.Find(_L(" "), true, startendtoken);
  if(end == XSTRING_NOTFOUND) end = line.Find(_L("."), true, startendtoken);
  if(end == XSTRING_NOTFOUND) return false;

  line.Copy(startendtoken, end, var);
  line.DeleteCharacters(start, (end-start));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::ExtractReservedToken(XSTRING& line, XCHAR* token)
* @brief      Extract reserved token
* @ingroup    XUTILS
* 
* @param[in]  line : Line value.
* @param[in]  token : Token pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::ExtractReservedToken(XSTRING& line, XCHAR* token)
{
  XSTRING mask;
  XSTRING strvar;

  mask = token;

  int start         = line.Find(mask.Get(), true);
  int end           = 0;
  if(start == XSTRING_NOTFOUND)  return false;

  end = start + mask.GetSize();
  line.DeleteCharacters(start, (end-start));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_LINE_TYPE XFILECOBOL::InterpreterLineForHandle(XSTRING& line, int& level, XFILECOBOL_FIELD_TYPE& fieldtype, XFILECOBOL_HANDLE& handle, XSTRING& name, XSTRING& param, XSTRING& param2, int& noccurs, int& PICcomputationaltype)
* @brief      Interpreter line for handle
* @ingroup    XUTILS
* 
* @param[in]  line : Line value.
* @param[in]  level : Level value.
* @param[in]  fieldtype : Fieldtype value.
* @param[in]  handle : Handle value.
* @param[in]  name : Name to use.
* @param[in]  param : Param value.
* @param[in]  param2 : Param2 value.
* @param[in]  noccurs : Noccurs value.
* @param[in]  PICcomputationaltype : PI Ccomputationaltype value.
* 
* @return     XFILECOBOL_LINE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_LINE_TYPE XFILECOBOL::InterpreterLineForHandle(XSTRING& line, int& level, XFILECOBOL_FIELD_TYPE& fieldtype, XFILECOBOL_HANDLE& handle, XSTRING& name, XSTRING& param, XSTRING& param2, int& noccurs, int& PICcomputationaltype)
{
  XFILECOBOL_LINE_TYPE  returntype = XFILECOBOL_LINE_TYPE_UNKNOWN;
  int                   index      = -1;

  level                 = 0;
  fieldtype             =  XFILECOBOL_FIELD_TYPE_UNKNOWN;
  PICcomputationaltype  = 0;

  //----------------------------------------------------------
  // FD
  index = line.Find(_L("FD "), true);
  if(!index)
    {
      ExtractReservedToken(line, _L("VALUE OF LABEL IS"), (*handle.GetValueLabel()));
      param.DeleteCharacter(_C('\''), XSTRINGCONTEXT_ALLSTRING);

      ExtractReservedToken(line, _L("DATA RECORD IS"), (*handle.GetDataRecord()));
      ExtractReservedToken(line, _L("FD"), (*handle.GetName()));

      returntype = XFILECOBOL_LINE_TYPE_FD;
    }
   else
    {
      index = line.Find(_L("SELECT "), true);
      if(!index)
        {
          ExtractReservedToken(line, _L("SELECT"), name);
          ExtractReservedToken(line, _L("ORGANIZATION IS") , param);
          ExtractReservedToken(line, _L("ASSIGN TO"), param2);

          returntype = XFILECOBOL_LINE_TYPE_SELECT;
        }
       else
        {
          //----------------------------------------------------------
          // FIELD LEVEL

          XSTRING levelstring;

          line.Copy(0, 2, levelstring);

          level = levelstring.ConvertToInt();
          if(level)
            {
              noccurs = 0;

              ExtractReservedToken(line, _L(" TIMES"));
              ExtractReservedToken(line, _L("OCCURS"), param2);
              if(!param2.IsEmpty())
                {
                  param2.UnFormat(_L("%d"), &noccurs);
                  param2.Empty();
                }

              ExtractReservedToken(line, _L("REDEFINES"), param2);

              if(line.Find(_L("PIC "), true) != XSTRING_NOTFOUND)
                {
                  if(ExtractReservedToken(line, _L("COMP-1")))  PICcomputationaltype = 1;
                  if(ExtractReservedToken(line, _L("COMP-2")))  PICcomputationaltype = 2;
                  if(ExtractReservedToken(line, _L("COMP-3")))  PICcomputationaltype = 3;
                  if(ExtractReservedToken(line, _L("COMP-4")))  PICcomputationaltype = 4;
                  if(ExtractReservedToken(line, _L("COMP-5")))  PICcomputationaltype = 5;

                  ExtractReservedToken(line, _L("PIC"), param);

                  name.AdjustSize(_MAXSTR);
                  line.UnFormat(_L("%02d %s"), &level, name.Get());
                  name.AdjustSize();

                  fieldtype =  XFILECOBOL_FIELD_TYPE_VAR;
                }
               else
                {
                  param.Empty();

                  name.AdjustSize(_MAXSTR);
                  line.UnFormat(_L("%02d %s."), &level, name.Get());
                  name.AdjustSize();

                  fieldtype =  XFILECOBOL_FIELD_TYPE_KEY;
                }

              returntype = XFILECOBOL_LINE_TYPE_FIELD;
            }

           name.DeleteCharacter(_C(' '));
           param.DeleteCharacter(_C(' '));
           param2.DeleteCharacter(_C(' '));

           name.DeleteCharacter(_C('.'));
           param.DeleteCharacter(_C('.'));
           param2.DeleteCharacter(_C('.'));
        }
    }

  return returntype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FD_TREENODE* XFILECOBOL::GetParentNodeOfLevel(int level)
* @brief      Get parent node of level
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* 
* @return     XFILECOBOL_FD_TREENODE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FD_TREENODE* XFILECOBOL::GetParentNodeOfLevel(int level)
{
  if(!handle.GetFDTree()) return NULL;

  XFILECOBOL_FD_TREENODEITERATOR  it_start(handle.GetFDTree()->Begin());
  XFILECOBOL_FD_TREENODEITERATOR  it_end(handle.GetFDTree()->End());

  while(it_start != it_end)
    {
      XFILECOBOL_FIELD* fieldtmp  = (*it_start);
      if(!fieldtmp) break;

      if(fieldtmp->GetLevel() == level)
        {
          return it_start.GetCurrent()->GetParent();
        }

      ++it_start;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::TraslateTextToHandle(XVECTOR<XSTRING*>* lines, XFILECOBOL_HANDLE& handle)
* @brief      Traslate text to handle
* @ingroup    XUTILS
* 
* @param[in]  lines : Lines pointer to use.
* @param[in]  handle : Handle value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::TraslateTextToHandle(XVECTOR<XSTRING*>* lines, XFILECOBOL_HANDLE& handle)
{
  if(!lines) return false;

  Eliminate_TabsForSpaces(lines);
  Eliminate_InvalidCharacters(lines);
  Eliminate_Comments(lines);
  Eliminate_NumberLines(lines);
  Eliminate_EmptyLines(lines);
  Eliminate_UnnecessarySpaces(lines);

  CreateUniqueLines(lines);

  XFILECOBOL_FD_TREENODE* actualfieldnode = NULL;
  int                     actuallevel     = 0;

  for(XDWORD c=0; c<lines->GetSize(); c++)
    {
      XFILECOBOL_LINE_TYPE    linetype          = XFILECOBOL_LINE_TYPE_UNKNOWN;
      int                     level             = 0;
      XFILECOBOL_FIELD_TYPE   fieldtype         = XFILECOBOL_FIELD_TYPE_UNKNOWN;
      XSTRING                 name;
      XSTRING                 param;
      XSTRING                 param2;
      int                     noccurs                = 0;
      int                     PICcomputationaltype   = 0;

      linetype = InterpreterLineForHandle((*lines->Get(c)), level, fieldtype, handle, name, param, param2, noccurs, PICcomputationaltype);

      switch(linetype)
        {
          case XFILECOBOL_LINE_TYPE_FD          : { XFILECOBOL_FIELD* rootfielddata = GEN_NEW XFILECOBOL_FIELD();
                                                    if(!rootfielddata) return false;

                                                    XFILECOBOL_FD_TREENODE* rootfieldnode = GEN_NEW XFILECOBOL_FD_TREENODE(rootfielddata);
                                                    if(!rootfieldnode)
                                                      {
                                                        GEN_DELETE rootfielddata;
                                                        return false;
                                                      }

                                                    rootfielddata->GetName()->Set(_L("ROOT"));
                                                    rootfielddata->SetLevel(0);

                                                    handle.GetFDTree()->SetRoot(rootfieldnode);
                                                    actualfieldnode = rootfieldnode;

                                                    actuallevel     = 1;
                                                  }
                                                  break;

          case XFILECOBOL_LINE_TYPE_SELECT      : { XPATH xpathdata;

                                                    if(handle.GetName()->Compare(name, true))
                                                      {
                                                        return false;
                                                      }

                                                    if(!param.IsEmpty())
                                                      {
                                                        if(!param.Compare(_L("INDEXED")         , true)) handle.SetTypeRecord(XFILECOBOL_RECORD_TYPE_INDEXED);
                                                        if(!param.Compare(_L("LINE SEQUENTIAL") , true)) handle.SetTypeRecord(XFILECOBOL_RECORD_TYPE_LINE_SEQUENTIAL);
                                                        if(!param.Compare(_L("SEQUENTIAL")      , true)) handle.SetTypeRecord(XFILECOBOL_RECORD_TYPE_SEQUENTIAL);
                                                        if(!param.Compare(_L("RELATIVE")        , true)) handle.SetTypeRecord(XFILECOBOL_RECORD_TYPE_RELATIVE);
                                                      }

                                                    if(!param2.IsEmpty())
                                                      {
                                                        if(!param2.Compare(_L("DISK"), true))
                                                          {
                                                            CapabilitiesFile_GetFile(handle.GetName(), xpathdata);
                                                            xpathdata.SetOnlyPathAndNamefileExt();
                                                          }
                                                         else
                                                          {
                                                            xpathdata.AdjustSize(_MAXSTR);

                                                            if(param2.Find(_L("'") , true) != XSTRING_NOTFOUND)  param2.UnFormat(_L("'%s'"), xpathdata.Get());
                                                            if(param2.Find(_L("\""), true) != XSTRING_NOTFOUND)  param2.UnFormat(_L("\"%s\""), xpathdata.Get());

                                                            xpathdata.AdjustSize();
                                                          }
                                                      }

                                                    if(xpathdatafile.IsEmpty())
                                                      {
                                                        xpathdatafile = xpathdata.Get();
                                                      }

                                                  }

                                                  break;


          case XFILECOBOL_LINE_TYPE_FIELD       : { XFILECOBOL_FIELD*       fielddata = NULL;
                                                    XFILECOBOL_FD_TREENODE* fieldnode = NULL;

                                                    fielddata = GEN_NEW XFILECOBOL_FIELD();
                                                    if(fielddata)
                                                      {
                                                        fieldnode = GEN_NEW XFILECOBOL_FD_TREENODE(fielddata);
                                                        if(!fieldnode) break;

                                                      } else break;

                                                    if(fieldnode)
                                                      {
                                                        fieldnode->GetData()->SetType(fieldtype);
                                                        fieldnode->GetData()->SetLevel(level);
                                                        fieldnode->GetData()->GetName()->Set(name);
                                                        fieldnode->GetData()->GetPIC()->GetMask()->Set(param);
                                                        fieldnode->GetData()->GetPIC()->SetComputationalType(PICcomputationaltype);
                                                        fieldnode->GetData()->SetNOccurs(noccurs);

                                                        if(param2.GetSize())
                                                          {
                                                            fieldnode->GetData()->SetIsRedefine(true);
                                                            fieldnode->GetData()->GetRedefineName()->Set(param2);
                                                          }
                                                         else
                                                          {
                                                            fieldnode->GetData()->SetIsRedefine(false);
                                                          }

                                                        if(fieldnode->GetData()->GetType() == XFILECOBOL_FIELD_TYPE_VAR)
                                                          {
                                                            fieldnode->GetData()->GetPIC()->ResolveMask();
                                                          }

                                                        if(level == actuallevel)
                                                          {
                                                            actualfieldnode->AddChild(fieldnode);
                                                          }
                                                          else
                                                          {
                                                            if((level > actuallevel))
                                                              {
                                                                actualfieldnode->AddChild(fieldnode);
                                                                actuallevel     = level;
                                                              }
                                                             else
                                                              {
                                                                actualfieldnode = GetParentNodeOfLevel(level);
                                                                if(!actualfieldnode)
                                                                  {
                                                                    actualfieldnode = handle.GetFDTree()->GetRoot();
                                                                    break;
                                                                  }

                                                                /*
                                                                for(int d=0; d<(actuallevel-level); d++)
                                                                  {
                                                                    actualfieldnode = actualfieldnode->GetParent();
                                                                    if(!actualfieldnode)
                                                                      {
                                                                        actualfieldnode = handle.GetFDTree()->GetRoot();
                                                                        break;
                                                                      }
                                                                  }
                                                                */

                                                                actualfieldnode->AddChild(fieldnode);
                                                                actuallevel = level;
                                                              }
                                                          }

                                                        if(fieldtype == XFILECOBOL_FIELD_TYPE_KEY)
                                                          {
                                                            actualfieldnode = fieldnode;
                                                          }
                                                      }
                                                  }

                                                  break;

        }
    }

  // -----------------------------------------------------------------------------
  // Recreate the nodes with "Occurs" in Cobol
  // Root is special usually only has one child, but it may not ...

  XFILECOBOL_FD_TREENODE* node = handle.GetFDTree()->GetRoot();
  if(node)
    {
      for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
        {
          XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
          if(subnode)
            {
              if(subnode->GetData())
                {
                  AjustNodeWithOccurs(subnode);
                }
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::ChangeNameNodeWithOccurs(int index, XSTRING* name)
* @brief      Change name node with occurs
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::ChangeNameNodeWithOccurs(int index, XSTRING* name)
{
  if(!name)             return false;
  if(name->IsEmpty())   return false;

  int indexstart = name->Find(_L("("), true);
  if(indexstart == XSTRING_NOTFOUND)
    {
      name->AddFormat(_L("(%d)"), index);
    }
   else
    {
      int indexend = name->Find(_L(")"), true);
      if(indexend == XSTRING_NOTFOUND) return false;

      XSTRING addindexstr;
      addindexstr.Format(_L(",%d"), index);

      name->Insert(addindexstr, indexend);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECOBOL_FD_TREENODE* XFILECOBOL::DuplicateNodeWithSubnodes(int index, XFILECOBOL_FD_TREENODE* node, XFILECOBOL_FD_TREENODE* cretednode)
* @brief      Duplicate node with subnodes
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* @param[in]  node : Node pointer to use.
* @param[in]  cretednode : Cretednode pointer to use.
* 
* @return     XFILECOBOL_FD_TREENODE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECOBOL_FD_TREENODE* XFILECOBOL::DuplicateNodeWithSubnodes(int index, XFILECOBOL_FD_TREENODE* node, XFILECOBOL_FD_TREENODE* cretednode)
{
  XFILECOBOL_FIELD*       newfielddata = NULL;
  XFILECOBOL_FD_TREENODE* newfieldnode = NULL;
  XSTRING                 namesubnode;

  namesubnode =  node->GetData()->GetName()->Get();
  if(!ChangeNameNodeWithOccurs(index, &namesubnode)) return NULL;

  newfielddata = GEN_NEW XFILECOBOL_FIELD();
  if(newfielddata)
    {
      newfieldnode = GEN_NEW XFILECOBOL_FD_TREENODE(newfielddata);
      if(!newfieldnode) return NULL;

    } else return NULL;

  newfieldnode->GetData()->SetType(node->GetData()->GetType());
  newfieldnode->GetData()->SetLevel(node->GetData()->GetLevel());
  newfieldnode->GetData()->GetName()->Set(namesubnode);
  newfieldnode->GetData()->GetPIC()->GetMask()->Set(node->GetData()->GetPIC()->GetMask()->Get());
  newfieldnode->GetData()->GetPIC()->SetComputationalType(node->GetData()->GetPIC()->GetComputationalType());
  newfieldnode->GetData()->SetNOccurs(node->GetData()->GetNOccurs());

  if(newfieldnode->GetData()->GetType() == XFILECOBOL_FIELD_TYPE_VAR)
    {
      newfieldnode->GetData()->GetPIC()->ResolveMask();
    }

 if(!cretednode)
    {
      int indexnode = handle.GetIndexNodeForName(node->GetParent(), node->GetData()->GetName()->Get());
      if(indexnode != NOTFOUND)
             node->GetParent()->InsertChild(indexnode, newfieldnode);
       else  node->GetParent()->AddChild(newfieldnode);
    }
   else
    {
      cretednode->AddChild(newfieldnode);
    }

  for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
    {
      XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
      if(subnode) DuplicateNodeWithSubnodes(index, subnode, newfieldnode);
    }

  return newfieldnode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECOBOL::AjustNodeWithOccurs(XFILECOBOL_FD_TREENODE* node)
* @brief      Ajust node with occurs
* @ingroup    XUTILS
* 
* @param[in]  node : Node pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECOBOL::AjustNodeWithOccurs(XFILECOBOL_FD_TREENODE* node)
{
  if(!node)            return false;
  if(!node->GetData()) return false;

  XDWORD noccurs = node->GetData()->GetNOccurs();
  if(noccurs)
    {
      XFILECOBOL_FD_TREENODE*  newnode  = NULL;
      XFILECOBOL_FD_TREENODE*  parent   = node->GetParent();

      node->GetData()->SetNOccurs(0);

      for(XDWORD c=0; c<(XDWORD)noccurs; c++)
        {
          newnode = DuplicateNodeWithSubnodes(c+1, node);
          if(newnode)
            {
              for(XDWORD c=0; c<(XDWORD)newnode->GetNChildren(); c++)
                {
                  XFILECOBOL_FD_TREENODE* subnode = newnode->GetChild(c);
                  if(subnode) AjustNodeWithOccurs(subnode);
                }
            }
        }

      if(parent) parent->DelChild(node);
    }
   else
    {
      for(XDWORD c=0; c<(XDWORD)node->GetNChildren(); c++)
        {
          XFILECOBOL_FD_TREENODE* subnode = node->GetChild(c);
          if(subnode) AjustNodeWithOccurs(subnode);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECOBOL::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECOBOL::Clean()
{
  havefileheaders     = false;
  type                = XFILECOBOL_FILE_TYPE_UNKNOWN;
  fileheadersize      = 0;

  xfiledata           = NULL;

  sizedatarecord      = 0;
  maxfilerecordsize   = 0;
  minfilerecordsize   = 0;
}





