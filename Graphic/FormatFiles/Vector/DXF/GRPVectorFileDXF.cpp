/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXF.cpp
* 
* @class      GRPVECTORFILEDXF
* @brief      Graphic Vector File DXF class
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

#include "GRPVectorFileDXF.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XTrace.h"
#include "XFileTXT.h"
#include "XVariant.h"

#include "GRPVectorFile_XEvent.h"
#include "GRPVectorFileDXFTextBlock.h"
#include "GRPVectorFileDXFTextSectionBlocks.h"
#include "GRPVectorFileDXFTextSectionEntities.h"
#include "GRPVectorFileDXFTextSectionHeader.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXF::GRPVECTORFILEDXF()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXF::GRPVECTORFILEDXF()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXF::~GRPVECTORFILEDXF()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXF::~GRPVECTORFILEDXF()
{
  sections.DeleteContents();
  sections.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILEDXF::DetectType()
* @brief      Detect type
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILEDXF::DetectType()
{  
  GRPVECTORFILERESULT result;

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

      return GRPVECTORFILERESULT_ERRORNOTFILE;  
    }

  if(!extension.Compare(GRPVECTORFILEDXF_EXTENSION, true))
    {
      type = GRPVECTORFILETYPE_DXF;  
    }
  
  if(DetectFileFormatText(pathfile))
    {
      XFILETXT* file = GEN_NEW XFILETXT();
      if(file)  
        {
          if(file->Open(pathfile))
            {
              int nlines = 100;

              file->ReadNLines(nlines);

              nlines = file->GetNLines();

              if(ParserHaveAnySection(file, nlines))
                {
                  result = GRPVECTORFILERESULT_OK; 
                }
               else
                {
                  result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT;   
                }        
 
              file->Close();             
            } 
           else 
            {
              result = GRPVECTORFILERESULT_ERRORNOTFILE;
            }

          GEN_DELETE file;
        } 
       else 
        {
          result = GRPVECTORFILERESULT_ERRORNOTMEMORY;
        }
    } 
   else 
    {
      result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT; 
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILEDXF::Load()
* @brief      Load
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILEDXF::Load()
{
  GRPVECTORFILEDXFRESULT result = GRPVECTORFILERESULT_ERRORNOTMEMORY;

  XFILETXT* file = GEN_NEW XFILETXT();
  if(!file)  
    {
      return GRPVECTORFILERESULT_ERRORNOTMEMORY;
    }

  if(file->Open(pathfile))
    {
      XTIMER* xtimer = GEN_XFACTORY.CreateTimer();
      if(xtimer)
        {
          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Reading TXT file ..."));

          xtimer->Reset();
      
          file->ReadAllFile();      

          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Read TXT file: %ld Milliseconds."), xtimer->GetMeasureMilliSeconds());   

          GEN_XFACTORY.DeleteTimer(xtimer);
        }
   
      result = ParserTextFile (file);
    
      file->Close();    
    } 
   else 
    {
      result = GRPVECTORFILERESULT_ERRORNOTFILE;
    }

   GEN_DELETE file;

   return result.TakeAsBase();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILECONFIG* GRPVECTORFILEDXF::GetConfig()
* @brief      Get config
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILECONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILECONFIG* GRPVECTORFILEDXF::GetConfig()
{
  return (GRPVECTORFILECONFIG*)&config;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPVECTORFILEDXFTEXTSECTION*>* GRPVECTORFILEDXF::GetSections()
* @brief      Get sections
* @ingroup    GRAPHIC
* 
* @return     XVECTOR<GRPVECTORFILEDXFTEXTSECTION*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVECTORFILEDXFTEXTSECTION*>* GRPVECTORFILEDXF::GetSections()
{
  return &sections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFTEXTSECTION* GRPVECTORFILEDXF::GetSection(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION type)
* @brief      Get section
* @ingroup    GRAPHIC
* 
* @param[in]  type : Type value.
* 
* @return     GRPVECTORFILEDXFTEXTSECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFTEXTSECTION* GRPVECTORFILEDXF::GetSection(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION type)
{
  for(XDWORD c=0; c<sections.GetSize(); c++)
    {
      GRPVECTORFILEDXFTEXTSECTION* section = sections.Get(c);
      if(section)
        {
          if(section->type == type) return section;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXF::ParserTextFilePrepareLine(XSTRING* line)
* @brief      Parser text file prepare line
* @ingroup    GRAPHIC
* 
* @param[in]  line : Line pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXF::ParserTextFilePrepareLine(XSTRING* line)
{
  if(!line) 
    {
      return false;
    }
   
  if(line->IsEmpty()) 
    {
      return false;
    }

  line->DeleteCharacter(_C(' '), XSTRINGCONTEXT_FROM_FIRST);
  line->DeleteCharacter(_C(' '), XSTRINGCONTEXT_TO_END);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFRESULT GRPVECTORFILEDXF::ParserTextSections(XFILETXT* fileTXT, XVECTOR<GRPVECTORFILEDXFTEXTSECTION*>& sections)
* @brief      Parser text sections
* @ingroup    GRAPHIC
* 
* @param[in]  fileTXT : File TXT pointer to use.
* @param[in]  sections : Sections pointer to use.
* 
* @return     GRPVECTORFILEDXFRESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFRESULT GRPVECTORFILEDXF::ParserTextSections(XFILETXT* fileTXT, XVECTOR<GRPVECTORFILEDXFTEXTSECTION*>& sections)
{
  GRPVECTORFILEDXFTEXTSECTION*  section = NULL;
  GRPVECTORFILEDXFRESULT        result  = GRPVECTORFILERESULT_ERRORUNKNOWN;
  int                           c       = 0;
 
  do{ if(!section)
        {
          section = GEN_NEW GRPVECTORFILEDXFTEXTSECTION ();
        }
    
      XSTRING* line = fileTXT->GetLine(c);
      ParserTextFilePrepareLine(line);

      if(line && !line->Compare(_L("0"), true))
        {           
          line = fileTXT->GetLine(c+1);
          ParserTextFilePrepareLine(line);

          if(line && !line->Compare(_L("SECTION"), false))
            {             
              line = fileTXT->GetLine(c+2);
              ParserTextFilePrepareLine(line);
              if(line && !line->Compare(_L("2"), true))
                {
                  c += 3;

                  line = fileTXT->GetLine(c);
                  ParserTextFilePrepareLine(line);

                  section->iniline  = c+1;
                  section->name     = line->Get(); 

                  c++;
                }
             }

          if(line && !line->Compare(_L("ENDSEC"), false))
            {
              section->endline  = c-1;                                       
              section->type     = GRPVECTORFILEDXFTEXTSECTION::GetTypeSection(section->name);  
              
              #ifdef XTRACE_ACTIVE
              if(section->type == GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN)
                {                 
                  XSTRING message;

                  message.Format(_L("section %s Unknown"), section->name.Get());   
                                                
                  GRPVECTORFILE_XEVENT vfEvent(this, GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN);

                  vfEvent.SetType(GRPVECTORFILETYPE_DXF);
                  vfEvent.GetPath()->Set(fileTXT->GetPrimaryFile()->GetPathNameFile());
                  vfEvent.GetMsg()->Set(message);

                  PostEvent(&vfEvent);       
                }
              #endif

              GRPVECTORFILEDXFTEXTSECTION* sectionType = GRPVECTORFILEDXFTEXTSECTION::CreateInstance(section->type);
              if(sectionType) 
                { 
                  section->CopyTo(sectionType);
                  sectionType->SetGrpVectorFile(this);
                  sections.Add(sectionType);
                  
                  GEN_DELETE section;                                         
                }
               else 
                {
                  sections.Add(section);
                }

              section = NULL;
            }

          if(line && !line->Compare(_L("EOF"), false))
            {
              c++;
              break;
            }
        }
        
      c++;

   } while(c < fileTXT->GetNLines());
     
  if(section &&  (section->type == GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN))
    {
      GEN_DELETE section;
    }

  if(!sections.IsEmpty()) 
    {    
      result = GRPVECTORFILERESULT_OK;
    } 
   else
    {
      result = GRPVECTORFILEDXFRESULT_NOTSECTIONS;
    }
  
  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXF::ParserHaveAnySection(XFILETXT* file, int nlinesmax)
* @brief      Parser have any section
* @ingroup    GRAPHIC
* 
* @param[in]  file : File object to use.
* @param[in]  nlinesmax : Nlinesmax value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXF::ParserHaveAnySection(XFILETXT* file, int nlinesmax)
{
  bool  foundSection  = false;
  int   c             = 0;
 
  do{ XSTRING* line = file->GetLine(c);
      ParserTextFilePrepareLine(line);

      if(line && !line->Compare(_L("0"), true))
        {
          c++;
          line = file->GetLine(c);
          ParserTextFilePrepareLine(line);

          if(line && !line->Compare(_L("SECTION"), false))
            {
              c++;
              line = file->GetLine(c);
              ParserTextFilePrepareLine(line);

              if(line && !line->Compare(_L("2"), true))
                {
                  c++;
                  line = file->GetLine(c);
                  ParserTextFilePrepareLine(line);

                  foundSection = true;
                
                  break;
                }
            }
        }

      c++;

    } while(c < nlinesmax);

  return foundSection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILEDXFRESULT GRPVECTORFILEDXF::ParserTextFile(XFILETXT* fileTXT)
* @brief      Parser text file
* @ingroup    GRAPHIC
* 
* @param[in]  fileTXT : File TXT pointer to use.
* 
* @return     GRPVECTORFILEDXFRESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILEDXFRESULT GRPVECTORFILEDXF::ParserTextFile(XFILETXT* fileTXT)
{
  GRPVECTORFILEDXFRESULT result = GRPVECTORFILERESULT_ERRORUNKNOWN;
  
  sections.DeleteContents();
  sections.DeleteAll();
  
  result = ParserTextSections (fileTXT, sections);
  if(result == (GRPVECTORFILEDXFRESULT)GRPVECTORFILERESULT_OK)
    {         
      for(XDWORD c=0; c<sections.GetSize(); c++)
        {
          GRPVECTORFILEDXFTEXTSECTION* section = sections.Get(c);
          if(section)
            {
              result = section->ParserTextSection(fileTXT);
              if(result != (GRPVECTORFILEDXFRESULT)GRPVECTORFILERESULT_OK) 
                {
                  break;
                }
            }
        }
  
      if(!sections.GetSize()) 
        {
          result = GRPVECTORFILEDXFRESULT_NOTSECTIONS;
        }      
    }
  
  GRPVECTORFILEDXFTEXTSECTIONHEADER* sectionheader = (GRPVECTORFILEDXFTEXTSECTIONHEADER*)GetSection(GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER);
  if(sectionheader)
    {
      config.SetHeader(sectionheader);
    }
  
  #ifdef XTRACE_ACTIVE    
  ShowTraceAllSections();
  #endif
  
  return result;
}


#ifdef XTRACE_ACTIVE         

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXF::GenerateSectionsEntities(GRPVECTORFILEDXFTEXTSECTIONENTITIES* sectionentities, XSTRING& line)
* @brief      Generate sections entities
* @ingroup    GRAPHIC
* 
* @param[in]  sectionentities : Sectionentities pointer to use.
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXF::GenerateSectionsEntities(GRPVECTORFILEDXFTEXTSECTIONENTITIES* sectionentities, XSTRING& line)
{  
  XMAP<XSTRING*, int>* entities = sectionentities->GetEnumEntitys();
  if(entities)
    {                                          
      for(XDWORD d=0; d<entities->GetSize(); d++)
        {
          XSTRING*  name      = entities->GetKey(d);
          int       nentities = entities->GetElement(d);

          if(name) 
            { 
              line.AddFormat(_L(" %s (%d)"), name->Get(), nentities);
              if(d < (entities->GetSize()-1)) 
                {
                  line.Add(_L(","));
                }
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILEDXF::ShowTraceAllSections()
* @brief      Show trace all sections
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILEDXF::ShowTraceAllSections()
{
  for(XDWORD c=0; c<sections.GetSize(); c++)
    {
      GRPVECTORFILEDXFTEXTSECTION* section = sections.Get(c);
      if(section)
        {
          XSTRING line;

          line.Format(_L("[Vector file DXF load] Section [%d, %d]: %s"), section->iniline, section->endline, section->name.Get());

          switch(section->type)
            {
              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN        :
                                                        default           :  break;

              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER         : { GRPVECTORFILEDXFTEXTSECTIONHEADER* sectionheader = (GRPVECTORFILEDXFTEXTSECTIONHEADER*)GetSection (section->type);
                                                                              if(sectionheader) 
                                                                                {
                                                                                  line.AddFormat(_L(" -> NVariables (%d)"), sectionheader->GetVariables()->GetSize());                                                                                                                                                       
                                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, line.Get());
                                                                                }
                                                                            }
                                                                            break;         

              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_CLASSES        : 
              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_TABLES         : break;

              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_BLOCKS         : { GRPVECTORFILEDXFTEXTSECTIONBLOCKS* sectionblocks = (GRPVECTORFILEDXFTEXTSECTIONBLOCKS*)GetSection(section->type);
                                                                              if(sectionblocks)
                                                                                {
                                                                                  line.AddFormat(_L(" -> (%d)"), sectionblocks->GetBlocks()->GetSize());
                                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, line.Get());

                                                                                  for(XDWORD d=0; d<sectionblocks->GetBlocks()->GetSize(); d++)
                                                                                    {
                                                                                      GRPVECTORFILEDXFTEXTBLOCK* block = sectionblocks->GetBlocks()->Get(d);
                                                                                      if(block)
                                                                                        {
                                                                                          if(!block->IsEndBlock())
                                                                                            {
                                                                                              GRPVECTORFILEDXFTEXTSECTIONENTITIES* sectionentities = block->GetSectionEntities();  
                                                                                              if(sectionentities)
                                                                                                {                   
                                                                                                  line.Format(_L("[Vector file DXF load] Block (%2d) [%s] ENTITIES "), d+1, block->GetName()->Get());                                                                                          
                                                                                                  GenerateSectionsEntities(sectionentities, line);
                                                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, line.Get());
                                                                                                }                                                                               
                                                                                            }
                                                                                        }                                                                             
                                                                                    }                                                                            
                                                                                }                                                                              
                                                                            }
                                                                            break; 

              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES       : { GRPVECTORFILEDXFTEXTSECTIONENTITIES* sectionentities = (GRPVECTORFILEDXFTEXTSECTIONENTITIES*)GetSection (section->type);
                                                                              if(sectionentities) 
                                                                                {                                                                              
                                                                                  GenerateSectionsEntities(sectionentities, line);
                                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, line.Get());
                                                                                }
                                                                            }
                                                                            break; 
              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_OBJECTS        : 
              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ACDSDATA       : 
              case GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_THUMBNAILIMAGE : break;
          }           
      }
   }            

   return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILEDXF::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILEDXF::Clean()
{
  
}



