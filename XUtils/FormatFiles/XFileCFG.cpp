/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileCFG.cpp
* 
* @class      XFILECFG
* @brief      eXtended Utils CFG (Config) file class
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

#include "XFileCFG.h"

#include <stdio.h>
#include <string.h>

#include "XPath.h"
#include "XFileINI.h"
#include "XVector.h"
#include "XVariant.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFGVALUE::XFILECFGVALUE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFGVALUE::XFILECFGVALUE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFGVALUE::~XFILECFGVALUE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFGVALUE::~XFILECFGVALUE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFG_VALUETYPE XFILECFGVALUE::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XFILECFG_VALUETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFG_VALUETYPE XFILECFGVALUE::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFGVALUE::SetType(XFILECFG_VALUETYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : GEN_NEW type of value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFGVALUE::SetType(XFILECFG_VALUETYPE type)
{
  this->type = type;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECFGVALUE::GetGroup()
* @brief      Get group
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECFGVALUE::GetGroup()
{
  return &group;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECFGVALUE::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECFGVALUE::GetID()
{
  return &ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XFILECFGVALUE::GetValue()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XFILECFGVALUE::GetValue()
{
  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFGVALUE::SetValue(void* value)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  value : GEN_NEW data of value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFGVALUE::SetValue(void* value)
{
  this->value = value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECFGVALUE::GetIDBasic()
* @brief      Get ID basic
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECFGVALUE::GetIDBasic()
{
  return &IDbasic;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECFGVALUE::GetMask()
* @brief      Get mask
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECFGVALUE::GetMask()
{
  return &mask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECFGVALUE::GetMinSecuences()
* @brief      Get min secuences
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECFGVALUE::GetMinSecuences()
{
  return minsecuences;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetMinSecuences(XDWORD minsecuences)
* @brief      Set min secuences
* @ingroup    XUTILS
* 
* @param[in]  minsecuences : Minsecuences value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetMinSecuences(XDWORD minsecuences)
{
  this->minsecuences = minsecuences;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECFGVALUE::GetMaxSecuences()
* @brief      Get max secuences
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECFGVALUE::GetMaxSecuences()
{
  return maxsecuences;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetMaxSecuences(XDWORD maxsecuences)
* @brief      Set max secuences
* @ingroup    XUTILS
* 
* @param[in]  maxsecuences : Maxsecuences value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetMaxSecuences(XDWORD maxsecuences)
{
  this->maxsecuences = maxsecuences;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECFGVALUE::GetIndexSecuence()
* @brief      Get index secuence
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECFGVALUE::GetIndexSecuence()
{
  return indexsecuence;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetIndexSecuence(int index)
* @brief      Set index secuence
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetIndexSecuence(int index)
{
  this->indexsecuence = index;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECFGVALUE::GetNSecuences()
* @brief      Get N secuences
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECFGVALUE::GetNSecuences()
{
  return nsecuences;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetNSecuences(XDWORD nsecuences)
* @brief      Set N secuences
* @ingroup    XUTILS
* 
* @param[in]  nsecuences : Nsecuences value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetNSecuences(XDWORD nsecuences)
{
  this->nsecuences = nsecuences;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XFILECFGVALUE::GetRemarkText()
* @brief      Get remark text
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILECFGVALUE::GetRemarkText()
{
  return &remark_text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XFILECFGVALUE::GetRemarkXPos()
* @brief      Get remark X pos
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XFILECFGVALUE::GetRemarkXPos()
{
  return remark_xpos;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetRemarkXPos(XDWORD remark_xpos)
* @brief      Set remark X pos
* @ingroup    XUTILS
* 
* @param[in]  remark_xpos : Remark xpos value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetRemarkXPos(XDWORD remark_xpos)
{
  this->remark_xpos = remark_xpos;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XFILECFGVALUE::GetValuesVector()
* @brief      Get values vector
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XFILECFGVALUE::GetValuesVector()
{
  return valuesvector;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetValuesVector(void* valuesvector)
* @brief      Set values vector
* @ingroup    XUTILS
* 
* @param[in]  valuesvector : Valuesvector pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetValuesVector(void* valuesvector)
{
  this->valuesvector = valuesvector;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFG_MODEREMOTEMIX XFILECFGVALUE::GetModeRemoteMix()
* @brief      Get mode remote mix
* @ingroup    XUTILS
* 
* @return     XFILECFG_MODEREMOTEMIX : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFG_MODEREMOTEMIX XFILECFGVALUE::GetModeRemoteMix()
{
  return moderemotemix;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::SetModeRemoteMix(XFILECFG_MODEREMOTEMIX moderemotemix)
* @brief      Set mode remote mix
* @ingroup    XUTILS
* 
* @param[in]  moderemotemix : Moderemotemix value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::SetModeRemoteMix(XFILECFG_MODEREMOTEMIX moderemotemix)
{
  this->moderemotemix = moderemotemix;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFGVALUE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFGVALUE::Clean()
{
  type            = XFILECFG_VALUETYPE_UNKNOWN;;
  group.Empty();
  ID.Empty();
  value           = NULL;
    
  IDbasic.Empty();
  mask.Empty();
  minsecuences    = 0;    
  maxsecuences    = 0;    
  indexsecuence   = XFILECFG_INVALIDINDEXSECUENCE;
  nsecuences      = 0;
  remark_text.Empty();
  remark_xpos     = 0;
  valuesvector    = NULL;  

  moderemotemix   = XFILECFG_MODEREMOTEMIX_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFG::XFILECFG(XCHAR* namefile)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  namefile : name file of config
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFG::XFILECFG(XCHAR* namefile)
{
  Clean();

  fileini = GEN_NEW XFILEINI();

  if(namefile) 
    {
      this->namefile = namefile;
      if(this->namefile.IsEmpty()) return;

      XPATH xpathroot;
      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

      xpathfile.Add(xpathroot.Get());
      if(!xpathfile.IsEmpty()) xpathfile.Slash_Add();

      xpathfile.Add(this->namefile.Get());
      xpathfile.Add(XFILECFG_EXTENSIONFILE);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFG::~XFILECFG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFG::~XFILECFG()
{
  if(fileini) GEN_DELETE fileini;

  DeleteAllRemarks();
  DeleteAllValues();
  
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::DoVariableMapping()
* @brief      Do variable mapping
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::DoVariableMapping()
{
  return true;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::DoDefault()
* @brief      Do default
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::DoDefault()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::LoadReadjustment()
* @brief      Load readjustment
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::LoadReadjustment()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::Load()
* @brief      Load
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::Load()
{
  if(xpathfile.IsEmpty()) return false;

  return Load(xpathfile);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::Load(XPATH& xpath)
* @brief      Load
* @ingroup    XUTILS
* 
* @param[in]  xpath : path to file config
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::Load(XPATH& xpath)
{
  bool status = false;

  if(values.IsEmpty()) return status;

  if(fileini->IsOpen())
    {
      EndFile();
    }
    
  if(IniFile(xpath)) 
    {
      AjustRemarks();

      for(int c=0;c<(int)values.GetSize();c++)
        {
          XFILECFGVALUE* cfgvalue = (XFILECFGVALUE*)values.Get(c);
          if(cfgvalue)
            {
              XSTRING strvalue;

              if(fileini->ReadValue(cfgvalue->GetGroup()->Get(), cfgvalue->GetID()->Get() , strvalue))
                {
                  switch(cfgvalue->GetType())
                    {
                      case XFILECFG_VALUETYPE_UNKNOWN : break;

                      case XFILECFG_VALUETYPE_INT     : { int* value = (int*)cfgvalue->GetValue();
                                                          if(!value) return false;

                                                          (*value) = strvalue.ConvertToInt();
                                                        }
                                                        break;

                      case XFILECFG_VALUETYPE_MASK    : { XWORD* value = (XWORD*)cfgvalue->GetValue();
                                                          if(!value) return false;

                                                          strvalue.UnFormat(__L("%04X"), value);
                                                        }
                                                        break;

                      case XFILECFG_VALUETYPE_FLOAT   : { float* value = (float*)cfgvalue->GetValue();
                                                          if(!value) return false;

                                                          (*value) = strvalue.ConvertToFloat();
                                                        }
                                                        break;

                      case XFILECFG_VALUETYPE_STRING  : { XSTRING* value = (XSTRING*)cfgvalue->GetValue();
                                                          if(!value) return false;

                                                          (*value) = strvalue.Get();

                                                          value->DeleteCharacter(__C('\t'));
                                                          value->DeleteCharacter(__C('\n'));
                                                          value->DeleteCharacter(__C('\r'));

                                                          value->DeleteCharacter(__C(' '), XSTRINGCONTEXT_TO_END);
                                                        }
                                                        break;

                      case XFILECFG_VALUETYPE_BOOLEAN : { bool* value = (bool*)cfgvalue->GetValue();
                                                          if(!value) return false;

                                                          (*value) = strvalue.ConvertToBoolean();
                                                        }
                                                        break;
                    }
                }
            }
        }

      status = EndFile();
    } 

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::Save(XFILETXTFORMATCHAR formatchar)
* @brief      save
* @ingroup    XUTILS
* 
* @param[in]  formatchar : Formatchar value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::Save(XFILETXTFORMATCHAR formatchar)
{
  if(xpathfile.IsEmpty()) 
    {
      return false;
    }

  return Save(xpathfile, formatchar);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::Save(XPATH& xpath, XFILETXTFORMATCHAR formatchar)
* @brief      save
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* @param[in]  formatchar : Formatchar value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::Save(XPATH& xpath, XFILETXTFORMATCHAR formatchar)
{
  bool status = false;

  if(values.IsEmpty()) return status;
  
  if(fileini->IsOpen())
    {
      EndFile();
    }

  if(IniFile(xpath))
    {
      if(formatchar != XFILETXTFORMATCHAR_UNKNOWN)
        {
          if(GetFileINI())
            {
              GetFileINI()->SetFormatChar(formatchar);
            }
        }

      AjustRemarks();    

      fileini->DeleteAllSections();

      for(int c=0;c<(int)values.GetSize();c++)
        {
          XSTRING strvalue;

          XFILECFGVALUE* cfgvalue = (XFILECFGVALUE*)values.Get(c);
          if(cfgvalue)
            {
              switch(cfgvalue->GetType())
                {
                  case XFILECFG_VALUETYPE_UNKNOWN : break;

                  case XFILECFG_VALUETYPE_INT     : { int* value = (int*)cfgvalue->GetValue();
                                                      if(!value) return false;

                                                      strvalue.Format(__L("%d"), (*value));
                                                    }
                                                    break;

                  case XFILECFG_VALUETYPE_MASK    : { XWORD* value = (XWORD*)cfgvalue->GetValue();
                                                      if(!value) return false;

                                                      strvalue.Format(__L("%04X"), (*value));
                                                    }
                                                    break;

                  case XFILECFG_VALUETYPE_FLOAT   : { float* value = (float*)cfgvalue->GetValue();
                                                      if(!value) return false;

                                                      strvalue.Format(__L("%f"), (*value));
                                                    }
                                                    break;

                  case XFILECFG_VALUETYPE_STRING  : { XSTRING* value = (XSTRING*)cfgvalue->GetValue();
                                                      if(!value) return false;

                                                      strvalue = value->Get();
                                                    }
                                                    break;

                  case XFILECFG_VALUETYPE_BOOLEAN : { bool* value = (bool*)cfgvalue->GetValue();
                                                      if(!value) return false;

                                                      strvalue.ConvertFromBoolean((*value), (XSTRINGBOOLEANMODE_HUMAN | XSTRINGBOOLEANMODE_LOWERCASE)); 
                                                    }
                                                    break;
                }

              status = fileini->WriteValue(cfgvalue->GetGroup()->Get(), cfgvalue->GetID()->Get() , strvalue);
              if(!status) break;

            }
        }

      status = EndFile();
    }    

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::End()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* XFILECFG::GetPathFile()
* @brief      Get path file
* @ingroup    XUTILS
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* XFILECFG::GetPathFile()
{
   return &xpathfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFGVALUE* XFILECFG::AddValue(XFILECFG_VALUETYPE type, XCHAR* group, XCHAR* ID, void* value, XCHAR* remark_text, XDWORD remark_xpos)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* @param[in]  group : Group pointer to use.
* @param[in]  ID : Identifier to use.
* @param[in]  value : Value pointer to use.
* @param[in]  remark_text : Remark text pointer to use.
* @param[in]  remark_xpos : Remark xpos value.
* 
* @return     XFILECFGVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFGVALUE* XFILECFG::AddValue(XFILECFG_VALUETYPE type, XCHAR* group, XCHAR* ID, void* value, XCHAR* remark_text, XDWORD remark_xpos)
{
  XFILECFGVALUE* CFGvalue = GEN_NEW XFILECFGVALUE();
  if(!CFGvalue) return NULL;

  CFGvalue->SetType(type);
  CFGvalue->GetGroup()->Set(group);
  CFGvalue->GetID()->Set(ID);
  CFGvalue->SetValue(value);

  if(values.Add(CFGvalue))
    {
      if(remark_text)
        {
          AddRemark(group, ID, remark_text, remark_xpos, 0);
        }
    }

  return CFGvalue;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XFILECFGVALUE*>* XFILECFG::GetValues()
* @brief      Get values
* @ingroup    XUTILS
* 
* @return     XVECTOR<XFILECFGVALUE*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XFILECFGVALUE*>* XFILECFG::GetValues()
{
  return &values;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILEINI* XFILECFG::GetFileINI()
* @brief      Get file INI
* @ingroup    XUTILS
* 
* @return     XFILEINI* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILEINI* XFILECFG::GetFileINI()
{
  return fileini;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* XFILECFG::GetValue(XCHAR* group, XCHAR* ID)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  ID : Identifier to use.
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* XFILECFG::GetValue(XCHAR* group, XCHAR* ID)
{
  XVARIANT* value = NULL;

  if((!group) || (!ID))
    {
      return NULL;
    }

  for(XDWORD c=0; c<values.GetSize(); c++)
    {
      XFILECFGVALUE* cfgvalue = values.Get(c);
      if(cfgvalue)
        {
          if((!cfgvalue->GetGroup()->Compare(group, true)) && (!cfgvalue->GetID()->Compare(ID, true)))
            {
              value = GEN_NEW XVARIANT();
              if(value)
                {
                  switch(cfgvalue->GetType())
                    {
                      case XFILECFG_VALUETYPE_UNKNOWN :
                                             default  : GEN_DELETE value; 
                                                        value = NULL;
                                                        break;

                      case XFILECFG_VALUETYPE_INT     : (*value) = (*(int*)(cfgvalue->GetValue()));     break; 
                      case XFILECFG_VALUETYPE_MASK    : (*value) = (*(XWORD*)(cfgvalue->GetValue()));   break;
                      case XFILECFG_VALUETYPE_FLOAT   : (*value) = (*(float*)(cfgvalue->GetValue()));   break;                                                        

                      case XFILECFG_VALUETYPE_STRING  : { XSTRING* _value = (XSTRING*)(cfgvalue->GetValue());                                                        
                                                          (*value) = _value->Get(); 
                                                        }
                                                        break;

                      case XFILECFG_VALUETYPE_BOOLEAN : (*value) = (*(bool*)(cfgvalue->GetValue()));    break; 
                    }
                }    
            }
        }
    }
  
  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVARIANT* XFILECFG::GetValue(XFILECFGVALUE* CFGvalue)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  CFGvalue : CF Gvalue pointer to use.
* 
* @return     XVARIANT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVARIANT* XFILECFG::GetValue(XFILECFGVALUE* CFGvalue)
{
  XVARIANT* value = NULL;

  if(!CFGvalue)
    {
      return NULL;
    }

  value = GEN_NEW XVARIANT();
  if(!value) return NULL;
      
  switch(CFGvalue->GetType())
    {
      case XFILECFG_VALUETYPE_UNKNOWN :
                              default : GEN_DELETE value; 
                                        value = NULL;
                                        break;

      case XFILECFG_VALUETYPE_INT     : (*value) = (*(int*)  (CFGvalue->GetValue()));     break; 
      case XFILECFG_VALUETYPE_MASK    : (*value) = (*(XWORD*)(CFGvalue->GetValue()));   break;
      case XFILECFG_VALUETYPE_FLOAT   : (*value) = (*(float*)(CFGvalue->GetValue()));   break;                                                        

      case XFILECFG_VALUETYPE_STRING  : { XSTRING* _value = (XSTRING*)(CFGvalue->GetValue());                                                        
                                          (*value) = _value->Get(); 
                                        }
                                        break;

      case XFILECFG_VALUETYPE_BOOLEAN : (*value) = (*(bool*)(CFGvalue->GetValue()));    break; 
    }
 
  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFGVALUE* XFILECFG::GetCFGValue(XCHAR* group, XCHAR* ID)
* @brief      Get CFG value
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  ID : Identifier to use.
* 
* @return     XFILECFGVALUE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFGVALUE* XFILECFG::GetCFGValue(XCHAR* group, XCHAR* ID)
{
  if((!group) || (!ID))
    {
      return NULL;
    }

  for(XDWORD c=0; c<values.GetSize(); c++)
    {
      XFILECFGVALUE* CFGvalue = values.Get(c);
      if(CFGvalue)
        {
          if( (!CFGvalue->GetGroup()->Compare(group, true)) &&
              (!CFGvalue->GetID()->Compare(ID, true)))
           {
              return CFGvalue;
           }          
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::SetValue(XFILECFGVALUE* CFGvalue, XVARIANT* value)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  CFGvalue : CF Gvalue pointer to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::SetValue(XFILECFGVALUE* CFGvalue, XVARIANT* value)
{
  if((!CFGvalue) || (!value))
    {
      return false;
    }

   switch(CFGvalue->GetType())
    {
      case XFILECFG_VALUETYPE_UNKNOWN :
                              default : { GEN_DELETE (XBYTE*)(CFGvalue->GetValue());
                                          CFGvalue->SetValue(NULL);                                          
                                        }
                                        break;

      case XFILECFG_VALUETYPE_INT     : (*(int*)  CFGvalue->GetValue()) = (*(int*)  (value->GetData()));   break; 
      case XFILECFG_VALUETYPE_MASK    : (*(XWORD*)CFGvalue->GetValue()) = (*(XWORD*)(value->GetData()));   break;
      case XFILECFG_VALUETYPE_FLOAT   : (*(float*)CFGvalue->GetValue()) = (*(float*)(value->GetData()));   break;                                                        

      case XFILECFG_VALUETYPE_STRING  : { XSTRING* _value = (XSTRING*)(CFGvalue->GetValue());                                                        
                                          (*_value) = ((XSTRING*)value->GetData())->Get();
                                        }
                                        break;

      case XFILECFG_VALUETYPE_BOOLEAN : (*(bool*)CFGvalue->GetValue()) = (*(bool*)(value->GetData()));    break; 
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::DeleteAllValues()
* @brief      Delete all values
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::DeleteAllValues()
{
  if(values.IsEmpty()) return false;

  values.DeleteContents();
  values.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::AddRemark(XCHAR* group, XCHAR* text, XDWORD xpos, XDWORD relativeypos)
* @brief      Add remark
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  text : Text to use.
* @param[in]  xpos : Xpos value.
* @param[in]  relativeypos : Relativeypos value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::AddRemark(XCHAR* group, XCHAR* text, XDWORD xpos, XDWORD relativeypos)
{
  if(!fileini) return false;

  XFILEINIREMARK* remark =  GEN_NEW XFILEINIREMARK();
  if(!remark) return false;
  
  remark->SetType(XFILEINI_TYPEREMARK_ALL_LINE);
  remark->GetRelativeSection()->Set(group);
  remark->GetRelativeKey()->Empty();
  remark->SetXPos(xpos);
  remark->SetRelativeYPos(relativeypos);
  remark->GetTextRemark()->Empty();
  remark->GetTextRemark()->Add(text);

  remarks.Add(remark);

  return true;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::AddRemark(XCHAR* group, XCHAR* ID, XCHAR* text, XDWORD xpos, XDWORD relativeypos)
* @brief      Add remark
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  ID : Identifier to use.
* @param[in]  text : Text to use.
* @param[in]  xpos : Xpos value.
* @param[in]  relativeypos : Relativeypos value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::AddRemark(XCHAR* group, XCHAR* ID, XCHAR* text, XDWORD xpos, XDWORD relativeypos)
{
  if(!fileini) return false;

  XFILEINIREMARK* remark =  GEN_NEW XFILEINIREMARK();
  if(!remark) return false;
  
  remark->SetType(XFILEINI_TYPEREMARK_IN_KEY);
  remark->GetRelativeSection()->Set(group);
  remark->GetRelativeKey()->Set(ID);
  remark->SetXPos(xpos);
  remark->SetRelativeYPos(relativeypos);
  remark->GetTextRemark()->Empty();
  remark->GetTextRemark()->Add(text);

  remarks.Add(remark);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XFILEINIREMARK*>* XFILECFG::GetRemarks()
* @brief      Get remarks
* @ingroup    XUTILS
* 
* @return     XVECTOR<XFILEINIREMARK*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XFILEINIREMARK*>* XFILECFG::GetRemarks()
{
  return &remarks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::DelCFGValue(XCHAR* group, XCHAR* ID)
* @brief      Del CFG value
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::DelCFGValue(XCHAR* group, XCHAR* ID)
{
  XFILECFGVALUE* CFGvalue = XFILECFG::GetCFGValue(group, ID);
  if(!CFGvalue) return false;

  values.Delete(CFGvalue);

  GEN_DELETE CFGvalue;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::DeleteAllRemarks()
* @brief      Delete all remarks
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::DeleteAllRemarks()
{
  if(remarks.IsEmpty()) return false;

  remarks.DeleteContents();
  remarks.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::IniFile(XPATH& xpath)
* @brief      Ini file
* @note       INTERNAL
* @ingroup    XUTILS
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::IniFile(XPATH& xpath)
{
  if(!fileini)           return false;
  if(xpath.IsEmpty())    return false;

  bool status = false;

  if(!fileini->Open(xpath, false)) 
    {
      fileini->Create(xpath);
    }

  if(fileini->IsOpen())
    {
      if(fileini->GetFormatCharFromFile() == XFILETXTFORMATCHAR_UNKNOWN) 
        {
          fileini->SetFormatChar(XFILETXTFORMATCHAR_ASCII);
        }

      if(fileini->GetTypeLF() == XFILETXTTYPELF_UNKNOWN)                 
        {
          fileini->SetTypeLF(XFILETXTTYPELF_DEFAULT);
        }

      status = true;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::EndFile()
* @brief      End file
* @note       INTERNAL
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::EndFile()
{
  if(!fileini) return false;

  return fileini->Close();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::AjustRemarks()
* @brief      Ajust remarks
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::AjustRemarks()
{
  if(!fileini) return false;

  XVECTOR<XFILEINIREMARK*>* ini_remarks = fileini->GetRemarks();
  if(!ini_remarks) return false;
  
  for(XDWORD d=0; d<remarks.GetSize(); d++)
    {
      XFILEINIREMARK* remark = remarks.Get(d);
      if(remark)
        {
          bool found  = false;

          for(XDWORD c=0; c<ini_remarks->GetSize(); c++)
            {
              XFILEINIREMARK* ini_remark = ini_remarks->Get(c);
              if(ini_remark)
                {
                  if(ini_remark->Compare(remark))  
                    {
                      found = true;
                      break;
                    }                    
                }  
            }

          if(!found)
            {
              XFILEINIREMARK* add_remark = GEN_NEW XFILEINIREMARK();
              if(add_remark)
                {
                  remark->CopyTo(add_remark);
                  ini_remarks->Add(add_remark);  
                }
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFILECFG::GetCountKeys(XCHAR* group, XCHAR* IDbasic, XCHAR* mask, int maxcount)
* @brief      Get count keys
* @ingroup    XUTILS
* 
* @param[in]  group : Group pointer to use.
* @param[in]  IDbasic : I Dbasic pointer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  maxcount : Maxcount value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFILECFG::GetCountKeys(XCHAR* group, XCHAR* IDbasic, XCHAR* mask, int maxcount)
{
  XSTRING   section       = group;      
  int       enumeratekeys = -1;
  
  if(!fileini->IsOpen())
    {
      if(!fileini->Open(xpathfile, true))
        {
          return 0;
        }
    }

  enumeratekeys = 0;

  for(int c=1; c<maxcount+1; c++)
    {    
      XSTRING key;       
      GenerateKeySecuence(IDbasic, mask, c, key);
      
      XFILEINIKEY* inikey = GetFileINI()->GetKey(section, key);
      if(inikey) enumeratekeys++;
    }      

  return enumeratekeys;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFILECFG::GenerateKeySecuence(XCHAR* IDbase, XCHAR* mask, int index, XSTRING& key)
* @brief      Generate key secuence
* @ingroup    XUTILS
* 
* @param[in]  IDbase : I Dbase pointer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  index : Index value.
* @param[in]  key : Key value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFILECFG::GenerateKeySecuence(XCHAR* IDbase, XCHAR* mask, int index, XSTRING& key)
{
  XSTRING keymask;      
  
  key.Empty();     
  
  if(!IDbase) return false;
    
  if(mask)  
    {
      keymask.Format(__L("%s%s"), IDbase, mask);
      key.Format(keymask.Get(), index);

    } else key.Format(__L("%s%d"), IDbase, index);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFILECFG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFILECFG::Clean()
{
  fileini     = NULL;

  namefile.Empty();
}



