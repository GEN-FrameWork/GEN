/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVG.cpp
* 
* @class      GRPVECTORFILESVG
* @brief      Graphic Vector File SVG class
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

#include "GRPVectorFileSVG.h"

#include "XTrace.h"
#include "XFileTXT.h"
#include "XFileXML.h"

#include "GRPVectorFileSVGObj.h"
#include "GRPVectorFileSVGCSSStyleSheet.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVG::GRPVECTORFILESVG()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVG::GRPVECTORFILESVG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVG::~GRPVECTORFILESVG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVG::~GRPVECTORFILESVG()
{
  if(root)
    {
      GEN_DELETE root;
      root = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILESVG::DetectType()
* @brief      Detect type : check the extension (.svg) and that the file is a text file with a &lt;svg&gt; root tag
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILESVG::DetectType()
{
  GRPVECTORFILERESULT result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT;

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

  if(!extension.Compare(GRPVECTORFILESVG_EXTENSION, true))
    {
      type = GRPVECTORFILETYPE_SVG;
    }

  if(DetectFileFormatText(pathfile))
    {
      XFILETXT* file = GEN_NEW XFILETXT();
      if(file)
        {
          if(file->Open(pathfile))
            {
              int nlines = 200;

              file->ReadNLines(nlines);

              nlines = file->GetNLines();

              if(DetectIsSVG(file, nlines))
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
* @fn         GRPVECTORFILERESULT GRPVECTORFILESVG::Load()
* @brief      Load : parse the XML, read the &lt;svg&gt; config and build the SVG object tree
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILESVG::Load()
{
  GRPVECTORFILERESULT result = GRPVECTORFILERESULT_ERRORNOTMEMORY;

  XFILEXML* file = GEN_NEW XFILEXML();
  if(!file)
    {
      return GRPVECTORFILERESULT_ERRORNOTMEMORY;
    }

  if(file->Open(pathfile, true))
    {
      if(file->ReadAndDecodeAllLines())
        {
          result = BuildFromXML(file);
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

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILESVG::Load(XSTRING& content)
* @brief      Load : parse the XML from an in memory text buffer and build the SVG object tree (same path as the disk load, only the source of the text lines changes)
* @ingroup    GRAPHIC
* 
* @param[in]  content : SVG source as a string
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILESVG::Load(XSTRING& content)
{
  GRPVECTORFILERESULT result = GRPVECTORFILERESULT_ERRORNOTMEMORY;

  XFILEXML* file = GEN_NEW XFILEXML();
  if(!file)
    {
      return GRPVECTORFILERESULT_ERRORNOTMEMORY;
    }

  if(GRPVECTORFILE::PopulateTextFileFromString(file, content))
    {
      if(file->DecodeAllLines())
        {
          result = BuildFromXML(file);
        }
       else
        {
          result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT;
        }
    }
   else
    {
      result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT;
    }

  GEN_DELETE file;

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILERESULT GRPVECTORFILESVG::BuildFromXML(XFILEXML* file)
* @brief      Build the SVG object tree from an already decoded XML file (shared by the disk and the in memory loads)
* @ingroup    GRAPHIC
* 
* @param[in]  file : decoded XML file (root must be the &lt;svg&gt; element)
* 
* @return     GRPVECTORFILERESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILERESULT GRPVECTORFILESVG::BuildFromXML(XFILEXML* file)
{
  GRPVECTORFILERESULT result = GRPVECTORFILERESULT_ERRORINVALIDFORMAT;

  if(!file)
    {
      return GRPVECTORFILERESULT_ERRORNOTMEMORY;
    }

  XFILEXMLELEMENT* xmlroot = file->GetRoot();
  if(xmlroot && !xmlroot->GetName().Compare(_L("svg"), true))
    {
      config.ApplyData(xmlroot);

      if(root)
        {
          GEN_DELETE root;
          root = NULL;
        }

      GRPVECTORFILESVGCSSSTYLESHEET cssstylesheet;
      cssstylesheet.ParseDocument(xmlroot);                            // class="" + <style> resolution (CorelDRAW/Illustrator/Figma exports)

      root = GRPVECTORFILESVGOBJ::CreateInstance(xmlroot, &cssstylesheet);

      result = root ? GRPVECTORFILERESULT_OK : GRPVECTORFILERESULT_ERRORNOTMEMORY;
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILECONFIG* GRPVECTORFILESVG::GetConfig()
* @brief      Get config
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILECONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILECONFIG* GRPVECTORFILESVG::GetConfig()
{
  return &config;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ* GRPVECTORFILESVG::GetRoot()
* @brief      Get root : the root SVG object (the &lt;svg&gt; node)
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ* GRPVECTORFILESVG::GetRoot()
{
  return root;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJ* GRPVECTORFILESVG::FindObjByID(XCHAR* id)
* @brief      Find obj by id : search the object tree (including the &lt;defs&gt; content) for a node with the given id
* @ingroup    GRAPHIC
* 
* @param[in]  id : id to find (without the leading '#')
* 
* @return     GRPVECTORFILESVGOBJ* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJ* GRPVECTORFILESVG::FindObjByID(XCHAR* id)
{
  if(!root) return NULL;

  return root->FindByID(id);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVG::DetectIsSVG(XFILETXT* file, int nlinesmax)
* @brief      Detect is SVG : look for a <svg tag in the first lines of the file
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  file : text file
* @param[in]  nlinesmax : max lines to scan
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVG::DetectIsSVG(XFILETXT* file, int nlinesmax)
{
  if(!file) return false;

  for(int c=0; c<nlinesmax; c++)
    {
      XSTRING* line = file->GetLine(c);
      if(line)
        {
          if(line->Find(_L("<svg"), true, 0) >= 0)
            {
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVG::Clean()
{
  root = NULL;
}




