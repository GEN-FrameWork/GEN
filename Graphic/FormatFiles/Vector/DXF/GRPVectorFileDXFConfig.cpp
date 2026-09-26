/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFConfig.cpp
* 
* @class      GRPVECTORFILEDXFCONFIG
* @brief      Graphic Vector File DXF Config class
* @ingroup    GRAPHIC
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

#include "GRPVectorFileDXFConfig.h"

#include "XPath.h"
#include "XFileINI.h"
#include "XFileCFG.h"

#include "GRPVectorFileDXF.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFCONFIG::GRPVECTORFILEDXFCONFIG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFCONFIG::GRPVECTORFILEDXFCONFIG()
{
  Clean();

  fileCFG = GEN_NEW XFILECFG();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFCONFIG::~GRPVECTORFILEDXFCONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFCONFIG::~GRPVECTORFILEDXFCONFIG()
{
  if(fileCFG)
    {
      GEN_DELETE fileCFG;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTIONHEADER* GRPVECTORFILEDXFCONFIG::GetHeader()
* @brief      Get header
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILEDXFTEXTSECTIONHEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTIONHEADER* GRPVECTORFILEDXFCONFIG::GetHeader()
{
  return header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFCONFIG::SetHeader(GRPVECTORFILEDXFTEXTSECTIONHEADER* header)
* @brief      Set header
* @ingroup    GRAPHIC
* 
* @param[in]  header : Header pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFCONFIG::SetHeader(GRPVECTORFILEDXFTEXTSECTIONHEADER* header)
{
  this->header = header;
  
  fileCFG->DeleteAllValues();
  
  for(XDWORD c=0; c<header->GetVariables()->GetSize(); c++)
    {   
      XSTRING*  namekey = header->GetVariables()->GetKey(c);
      XVARIANT* variant = header->GetVariables()->GetElement(c);
     
      if(namekey && variant)
        {
          switch(variant->GetType())
            {         
              case XVARIANT_TYPE_BOOLEAN        : fileCFG->AddValue(XFILECFG_VALUETYPE_BOOLEAN, VECTORFILEDXFCONFIG_SECTION_DFX_CFG, namekey->Get(), (bool*)(variant->GetData()));      break;  
              case XVARIANT_TYPE_INTEGER        : fileCFG->AddValue(XFILECFG_VALUETYPE_INT, VECTORFILEDXFCONFIG_SECTION_DFX_CFG, namekey->Get(), (int*)(variant->GetData()));       break;  
              case XVARIANT_TYPE_FLOAT          : fileCFG->AddValue(XFILECFG_VALUETYPE_FLOAT, VECTORFILEDXFCONFIG_SECTION_DFX_CFG, namekey->Get(), (float*)(variant->GetData()));     break;  
              case XVARIANT_TYPE_STRING         : fileCFG->AddValue(XFILECFG_VALUETYPE_STRING, VECTORFILEDXFCONFIG_SECTION_DFX_CFG, namekey->Get(), (XSTRING*)(variant->GetData()));   break;
  
              case XVARIANT_TYPE_NULL           :
              case XVARIANT_TYPE_DOUBLEINTEGER  :            
              case XVARIANT_TYPE_DOUBLE         :
              case XVARIANT_TYPE_CHAR           :
              case XVARIANT_TYPE_XCHAR          :            
              case XVARIANT_TYPE_DATE           :
              case XVARIANT_TYPE_TIME           :
              case XVARIANT_TYPE_DATETIME       : 
                                   default      : return false;
                                                  break;
            }         
  
          XCHAR* remark = header->GetVariableRemark((*namekey)); 
          if(remark) 
            {
              fileCFG->AddRemark(VECTORFILEDXFCONFIG_SECTION_DFX_CFG, namekey->Get(), remark, 64, 0);
            }
          
        }   
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFCONFIG::Create_File(XCHAR* pathfile)
* @brief      Create file
* @ingroup    GRAPHIC
* 
* @param[in]  pathfile : Pathfile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFCONFIG::Create_File(XCHAR* pathfile)
{
  if(!fileCFG) 
    {
      return false;
    }

  XPATH path;
   
  path = pathfile;

  return fileCFG->Save(path);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXFCONFIG::Load_File(XCHAR* pathfile)
* @brief      Load file
* @ingroup    GRAPHIC
* 
* @param[in]  pathfile : Pathfile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXFCONFIG::Load_File(XCHAR* pathfile)
{
  if(!fileCFG) 
    {
      return false;
    }

  if(!header)  
    {
      return false;
    }
  
  XPATH path;
  bool  status = false;
  
  path = pathfile;
  
  header->DeleteAllVariables();
  
  fileCFG->DeleteAllValues();
  
  if(fileCFG->IniFile(path)) 
    {
      fileCFG->AjustRemarks();
  
      XFILEINISECTION* sectionINI = fileCFG->GetFileINI()->GetSection(VECTORFILEDXFCONFIG_SECTION_DFX_CFG);
      if(sectionINI)
        {
          XSTRING key;         
          XSTRING value;
  
          while(sectionINI->GetNextKey(key, value))
            {
              XVARIANT variant;
              XSTRING  trash;                              
              int      type = 0;
  
              trash.AdjustSize(_MAXSTR);
  
              key.UnFormat(__L("$%s_%d"), trash.Get(), &type, NULL);
  
              GRPVECTORFILEDXFTEXTSECTION::GetVariableFromLine(trash.Get(), type, &value, variant);
                          
              if(variant.GetType() != XVARIANT_TYPE_NULL)   
                {
                  header->AddVariable(key.Get(), &variant);
                }
            }
        }
      
      //status = fileCFG->EndFile();
  
      fileCFG->GetFileINI()->XFILETXT::Close();
      fileCFG->DeleteAllRemarks();
    }
  
  #ifdef XTRACE_ACTIVE
  header->ShowTraceAllVariables();
  #endif
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXFCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXFCONFIG::Clean()
{
  header    = NULL;

  pathfile.Empty();

  fileCFG   = NULL;
}


