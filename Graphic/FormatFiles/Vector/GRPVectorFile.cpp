/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFile.cpp
* 
* @class      GRPVECTORFILE
* @brief      Graphic Vector File class
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

#include "GRPVectorFile.h"

#include "GRPVectorFile_XEvent.h"

#include "XFactory.h"
#include "XFileTXT.h"
#include "XBuffer.h"

#ifdef GRP_VECTOR_FILE_DXF_ACTIVE
#include "GRPVectorFileDXF.h"
#endif

#ifdef GRP_VECTOR_FILE_SVG_ACTIVE
#include "GRPVectorFileSVG.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE::GRPVECTORFILE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE::GRPVECTORFILE()
{
  Clean();

  RegisterEvent(GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE::~GRPVECTORFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE::~GRPVECTORFILE()
{
  if(IsConversionFile())
    {
      XFILE* xfile = GEN_XFACTORY.Create_File();
      if(xfile)
        {
          xfile->Erase((*GetPathFile()));
          GEN_XFACTORY.Delete_File(xfile);
        }
  
    }
  
  DeRegisterEvent(GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);
  
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* GRPVECTORFILE::GetPathFile()
* @brief      Get path file
* @ingroup    GRAPHIC
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* GRPVECTORFILE::GetPathFile()
{
  return &pathfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILETYPE GRPVECTORFILE::GetType()
* @brief      Get type
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILETYPE GRPVECTORFILE::GetType()
{
  return type;
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILE::SetType(GRPVECTORFILETYPE type)
* @brief      Set type
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILE::SetType(GRPVECTORFILETYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE* GRPVECTORFILE::CreateInstance(XPATH& pathfile)
* @brief      Create instance
* @ingroup    GRAPHIC
* 
* @param[in]  pathfile : Pathfile value.
* 
* @return     GRPVECTORFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE* GRPVECTORFILE::CreateInstance(XPATH& pathfile)
{
  if(!GRPVECTORFILE::DetectFile(pathfile)) 
    {
      return NULL;
    }

  GRPVECTORFILE*    vectorFile = NULL;
  GRPVECTORFILETYPE list[]     = { GRPVECTORFILETYPE_DXF ,
                                   GRPVECTORFILETYPE_SVG                                 
                                 };

  for(int c=0; c < (sizeof(list)/sizeof(GRPVECTORFILETYPE)); c++)
    {    
      vectorFile = GRPVECTORFILE::CreateInstance(list[c]);
      if(vectorFile) 
        {
          vectorFile->GetPathFile()->Set(pathfile);

          if(vectorFile->DetectType() == GRPVECTORFILERESULT_OK) 
            {              
              return vectorFile;

            } 
           else 
            {
              GEN_DELETE vectorFile;
            }
        }   
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE* GRPVECTORFILE::CreateInstance(GRPVECTORFILETYPE type)
* @brief      Create instance
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE* GRPVECTORFILE::CreateInstance(GRPVECTORFILETYPE type)
{
  GRPVECTORFILE* vectorFile = NULL;

  switch(type)
    {
      case GRPVECTORFILETYPE_UNKNOWN  : 
                            default   : break;

      #ifdef GRP_VECTOR_FILE_DXF_ACTIVE
      case GRPVECTORFILETYPE_DXF      : vectorFile = GEN_NEW GRPVECTORFILEDXF();
                                        break; 
      #endif      

      #ifdef GRP_VECTOR_FILE_SVG_ACTIVE
      case GRPVECTORFILETYPE_SVG      : vectorFile = GEN_NEW GRPVECTORFILESVG();
                                        break; 
      #endif      
   }

  if(vectorFile) 
    {
      vectorFile->SetType(type);
    }

  return vectorFile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILE* GRPVECTORFILE::CreateInstance(GRPVECTORFILETYPE type, XSTRING& content)
* @brief      Create instance : build the vector file of the given type and load it from an in memory text buffer. The content is handled exactly as if it had been read from disk in text mode.
* @ingroup    GRAPHIC
* 
* @param[in]  type : vector file type (SVG, DXF, ...).
* @param[in]  content : text content of the vector file (SVG / DXF source as a string).
* 
* @return     GRPVECTORFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILE* GRPVECTORFILE::CreateInstance(GRPVECTORFILETYPE type, XSTRING& content)
{
  GRPVECTORFILE* vectorFile = GRPVECTORFILE::CreateInstance(type);
  if(!vectorFile) 
    {
      return NULL;
    }

  if(vectorFile->Load(content) != GRPVECTORFILERESULT_OK) 
    {
      GEN_DELETE vectorFile;
      return NULL;
    }

  return vectorFile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRPVECTORFILE::GetTypeText(GRPVECTORFILETYPE type)
* @brief      Get type text
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRPVECTORFILE::GetTypeText(GRPVECTORFILETYPE type)
{
  XCHAR* typeTXT = NULL;

  switch(type)
    {
      case GRPVECTORFILETYPE_UNKNOWN : 
                           default   :  typeTXT = _L("Unknown");
                                        break;

      #ifdef GRP_VECTOR_FILE_DXF_ACTIVE
      case GRPVECTORFILETYPE_DXF     :  typeTXT = _L("DXF");
                                        break; 
      #endif     

      #ifdef GRP_VECTOR_FILE_SVG_ACTIVE
      case GRPVECTORFILETYPE_SVG     :  typeTXT = _L("SVG");
                                        break; 
      #endif     
    }

  return typeTXT;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILE::DetectType()
* @brief      Detect type
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILE::DetectType()
{  
  if(pathfile.IsEmpty()) 
    {
      type = GRPVECTORFILETYPE_UNKNOWN;
      return GRPVECTORFILERESULT_ERRORNOTFILE;
    }

  XSTRING extension;
  pathfile.GetExt(extension);

  if(extension.IsEmpty())
    {
      type = GRPVECTORFILETYPE_UNKNOWN;
      return  GRPVECTORFILERESULT_ERRORNOTFILE;  
    }

  return (type != GRPVECTORFILETYPE_UNKNOWN)?GRPVECTORFILERESULT_OK:GRPVECTORFILERESULT_ERRORNOTFILE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILE::Load()
* @brief      Load
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILE::Load()
{
  return GRPVECTORFILERESULT_ERRORNOTSUPPORTED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILE::Load(XSTRING& content)
* @brief      Load the vector file from an in memory text buffer (base : not implemented, each format overrides it).
* @ingroup    GRAPHIC
* 
* @param[in]  content : text content of the vector file.
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILE::Load(XSTRING& content)
{
  return GRPVECTORFILERESULT_ERRORNOTSUPPORTED;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILE::PopulateTextFileFromString(XFILETXT* file, XSTRING& content)
* @brief      Fill a text file object with the lines of a string, as if the file had been read from disk in text mode. Shared by every format backend so the in memory load reuses the same parsers as the disk load.
* @ingroup    GRAPHIC
* 
* @param[in]  file : text file object to fill (will be cleared first).
* @param[in]  content : text content to load.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILE::PopulateTextFileFromString(XFILETXT* file, XSTRING& content)
{
  if(!file) 
    {
      return false;
    }

  XBUFFER buffer;

  if(!content.ConvertToUTF8(buffer, false)) 
    {
      return false;
    }

  file->DeleteAllLines();

  return file->AddBufferLines(XFILETXTFORMATCHAR_UTF8, buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILECONFIG* GRPVECTORFILE::GetConfig()
* @brief      Get config
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILECONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILECONFIG* GRPVECTORFILE::GetConfig()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILE::DetectFileFormatText(XPATH& pathfile)
* @brief      Detect file format text
* @ingroup    GRAPHIC
* 
* @param[in]  pathfile : Pathfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILE::DetectFileFormatText(XPATH& pathfile)
{  
  XFILETXTFORMATCHAR  formatchar  =  XFILETXTFORMATCHAR_UNKNOWN;
  bool                isText      = false;
  
  XFILETXT* fileTXT = GEN_NEW XFILETXT();
  if(fileTXT)  
    {
      if(fileTXT->Open(pathfile))
        {
          formatchar = fileTXT->GetFormatCharFromFile();
  
          fileTXT->Close();
        }
  
      GEN_DELETE fileTXT;
    }
  
  if(formatchar != XFILETXTFORMATCHAR_ASCII) return true;

  XFILE* file = GEN_XFACTORY.Create_File();
  if(file)  
    {
      if(file->Open(pathfile))
        {
          #define MAX_BUFFER_TEST 1024*5 
  
          XBUFFER buffer;
          XDWORD  size = MAX_BUFFER_TEST;
          XDWORD  controlchars = 0;
  
          buffer.Resize(MAX_BUFFER_TEST);
          file->Read(buffer.Get(), &size);
  
          if(size)
            {
              for(XDWORD c=0; c<size; c++)
                {
                  if(((buffer.Get()[c] >= 0x00) && (buffer.Get()[c] < 0x0A)) ||         
                     ((buffer.Get()[c] >  0x0D) && (buffer.Get()[c] < 0x20)))
                    {
                      controlchars++;
                    }
                }
  
              int percent = (controlchars*100/size);
              if(percent < 10) isText = true;
            }
     
          file->Close();
        }
  
      GEN_DELETE file;
    }

   return isText;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILE::IsConversionFile()
* @brief      Is conversion file
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILE::IsConversionFile()
{
  return isconversionfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILE::SetIsConversionFile(bool isconversionfile)
* @brief      Set is conversion file
* @ingroup    GRAPHIC
* 
* @param[in]  isconversionfile : Isconversionfile value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILE::SetIsConversionFile(bool isconversionfile)
{
  this->isconversionfile = isconversionfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILE::DetectFile(XPATH& pathfile)
* @brief      Detect file
* @ingroup    GRAPHIC
* 
* @param[in]  pathfile : Pathfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILE::DetectFile(XPATH& pathfile)
{   
  XFILE* file = GEN_XFACTORY.Create_File();
  if(!file) 
    {
      return false;
    }
  
  bool result = file->Exist(pathfile);  
  
  GEN_DELETE file;
  
  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILE::Clean()
{
  pathfile.Empty();

  type             = GRPVECTORFILETYPE_UNKNOWN;
  isconversionfile = false;
}




