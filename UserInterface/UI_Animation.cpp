/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Animation.cpp
* 
* @class      UI_ANIMATION
* @brief      User Interface Animation class
* @ingroup    USERINTERFACE
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

#include "UI_Animation.h"

#include "XPathsManager.h"

#include "UI_Manager.h"

#include "APPFlowBase.h"

#include "GRPFactory.h"
#include "GRP2DCanvas.h"
#include "GRP2DColor.h"
#include "GRPVectorFile.h"
#include "GRP2DVectorFileRenderAGG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ANIMATION::UI_ANIMATION()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ANIMATION::UI_ANIMATION()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ANIMATION::~UI_ANIMATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ANIMATION::~UI_ANIMATION()    
{ 
  DeleteAll();

  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ANIMATION::LoadFromFile(XSTRING& resourcename, GRPPROPERTYMODE mode)
* @brief      Load from file
* @ingroup    USERINTERFACE
* 
* @param[in]  resourcename : Resourcename value.
* @param[in]  mode : Mode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ANIMATION::LoadFromFile(XSTRING& resourcename, GRPPROPERTYMODE mode)
{
  XPATH               xpathimg;   
  GRPBITMAPFILE       bitmapfile;
  GRPBITMAP*          bitmap    = NULL; 
  XSTRING             mask;
  int                 indexmask = 0;
  int                 indeximg  = 0;
  GRPPROPERTYMODE     modealpha = mode;
  GRPBITMAPFILE_TYPE  typefile  = bitmapfile.GetTypeFromExtenxion(resourcename.Get());

  modealpha = ChangeModeWithAlpha(typefile, mode);
                                                                                                                                                                       
  while(1)
    {                                                                                                                               
      XSTRING namefile; 

      indexmask = resourcename.Copy(UI_ANIMATION_FRAME_INI, UI_ANIMATION_FRAME_END, true, 0, mask);  
                                                               
      namefile = resourcename;    

      if(!mask.IsEmpty())
        {
          XSTRING result;
          int     masksize = mask.GetSize()+3;

          namefile.DeleteCharacters(indexmask-masksize, masksize);  
                                                                                                                                        
          mask.Insert(_L("%"), 0);
          mask.Add(_L("d"));
                                                                
          result.Format(mask.Get(), indeximg);
          indeximg++;
                                                                  
          namefile.Insert(result.Get(), indexmask-masksize);
        }                                               

      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathimg);
      xpathimg.Slash_Add();
      xpathimg.Add(namefile);

      if(GEN_USERINTERFACE.IsZippedFile())
        {
          XFILEUNZIP* unzipfile = GEN_USERINTERFACE.GetUnzipFile();
          if(unzipfile)
            {       
              XPATH pathnamefilecmp(APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
              XPATH namefileonly;

              pathnamefilecmp.Slash_Add();
              pathnamefilecmp += namefile;

              pathnamefilecmp.GetNamefileExt(namefileonly);
           
              bool status = unzipfile->DecompressFile(pathnamefilecmp, (*GEN_USERINTERFACE.GetUnzipPathFile()), namefileonly.Get());   
              if(status)
                {  
                  XPATH unzippathfile_tmp;

                  unzippathfile_tmp  = GEN_USERINTERFACE.GetUnzipPathFile()->Get();
                  unzippathfile_tmp += namefileonly;                    

                  bitmap = bitmapfile.Load(unzippathfile_tmp, modealpha);

                  GEN_USERINTERFACE.DeleteTemporalUnZipFile(unzippathfile_tmp);  

                  if(bitmap) 
                    { 
                      bitmaps.Add(bitmap);                                                      
                      if(mask.IsEmpty()) 
                        {
                          break;                                                                                                            
                        }
                    } 
                   else 
                    {
                      break;          
                    }
                }
               else
                {
                  break;
                }
            }
        }
       else
        {                            
          bitmap = bitmapfile.Load(xpathimg, modealpha);
          if(bitmap) 
            { 
              bitmaps.Add(bitmap);                                                      
              if(mask.IsEmpty()) 
                {
                  break;                                                                                                            
                }
            } 
            else 
            {
              break;          
            }
        }
    }                                                          

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ANIMATION::LoadFromFileVector(XSTRING& resourcename, GRP2DCANVAS* referencecanvas, double width, double height)
* @brief      Load from file vector
* @note       Rasterizes a vector resource (SVG, DXF...) into a single bitmap frame, at the given target size, and
*             stores it the same way LoadFromFile() stores a bitmap-file frame (so GetBitmap()/GetBitmaps() work
*             unchanged regardless of the resource's origin). Unlike a bitmap file, a vector file has no pixel size
*             of its own, so width/height are mandatory here; the caller (see UI_MANAGER::GetOrAddAnimationCache)
*             is also responsible for folding width/height into the cache key, since the very same vector file
*             requested at two different sizes must never share the same cached bitmap.
* @ingroup    USERINTERFACE
* 
* @param[in]  resourcename : Resourcename value.
* @param[in]  referencecanvas : Canvas to copy the pixel format and the loaded vector font from.
* @param[in]  width : Target width, in pixels, to rasterize at.
* @param[in]  height : Target height, in pixels, to rasterize at.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ANIMATION::LoadFromFileVector(XSTRING& resourcename, GRP2DCANVAS* referencecanvas, double width, double height)
{
  if(!referencecanvas)                  return false;
  if((width <= 0.0) || (height <= 0.0)) return false;

  XPATH pathvector;

  if(GEN_USERINTERFACE.IsZippedFile())
    {
      XFILEUNZIP* unzipfile = GEN_USERINTERFACE.GetUnzipFile();
      if(!unzipfile) return false;

      XPATH pathnamefilecmp(APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
      XPATH namefileonly;

      pathnamefilecmp.Slash_Add();
      pathnamefilecmp += resourcename;

      pathnamefilecmp.GetNamefileExt(namefileonly);

      if(!unzipfile->DecompressFile(pathnamefilecmp, (*GEN_USERINTERFACE.GetUnzipPathFile()), namefileonly.Get())) return false;

      pathvector  = GEN_USERINTERFACE.GetUnzipPathFile()->Get();
      pathvector += namefileonly;
    }
   else
    {
      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, pathvector);
      pathvector.Slash_Add();
      pathvector.Add(resourcename);
    }

  bool           status     = false;
  GRPVECTORFILE* vectorfile = GRPVECTORFILE::CreateInstance(pathvector);

  if(vectorfile)
    {
      if(vectorfile->Load() == GRPVECTORFILERESULT_OK)
        {
          GRPPROPERTIES properties;

          properties.CopyPropertysFrom(referencecanvas);
          properties.SetPosition(0, 0);
          properties.SetSize((XDWORD)width, (XDWORD)height);

          GRP2DCANVAS* offscreen = GEN_GRPFACTORY.CreateCanvas(&properties);
          if(offscreen)
            {
              offscreen->SetWidth(width);
              offscreen->SetHeight(height);

              if(offscreen->Buffer_Create())
                {
                  offscreen->VectorFont_CopyFrom(referencecanvas);          // SVG text needs the same loaded vector font as the real canvas

                  GRP2DCOLOR_RGBA8 transparent(0, 0, 0, 0);
                  offscreen->Clear(&transparent);                          // transparent background

                  GRP2DVECTORFILERENDERAGG vectorrender;

                  status = vectorrender.Render(vectorfile, offscreen, 0.0, 0.0, width, height);
                  if(status)
                    {
                      GRPBITMAP* bitmap = offscreen->GetBitmap(0.0, 0.0, width, height);   // capture with its own alpha

                      if(bitmap) bitmaps.Add(bitmap);
                        else     status = false;
                    }
                }

              GEN_GRPFACTORY.DeleteCanvas(offscreen);
            }
        }

      GEN_DELETE vectorfile;
    }

  if(GEN_USERINTERFACE.IsZippedFile())
    {
      GEN_USERINTERFACE.DeleteTemporalUnZipFile(pathvector);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ANIMATION::GetName()
* @brief      Get name
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ANIMATION::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ANIMATION::GetResource()
* @brief      Get resource
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ANIMATION::GetResource()
{
  return &resource;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<GRPBITMAP*>* UI_ANIMATION::GetBitmaps()
* @brief      Get bitmaps
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<GRPBITMAP*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPBITMAP*>* UI_ANIMATION::GetBitmaps()
{
  return &bitmaps;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ANIMATION::GetBitmap(int index)
* @brief      Get bitmap
* @ingroup    USERINTERFACE
* 
* @param[in]  index : Index value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ANIMATION::GetBitmap(int index)
{
  return bitmaps.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ANIMATION::DeleteAll()
* @brief      Delete all
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ANIMATION::DeleteAll()
{
  if(bitmaps.IsEmpty()) return false;

  bitmaps.DeleteContents();
  bitmaps.DeleteAll();
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPPROPERTYMODE UI_ANIMATION::ChangeModeWithAlpha(GRPBITMAPFILE_TYPE typefile, GRPPROPERTYMODE modescreen)
* @brief      Change mode with alpha
* @ingroup    USERINTERFACE
* 
* @param[in]  typefile : Typefile value.
* @param[in]  modescreen : Modescreen value.
* 
* @return     GRPPROPERTYMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPPROPERTYMODE UI_ANIMATION::ChangeModeWithAlpha(GRPBITMAPFILE_TYPE typefile, GRPPROPERTYMODE modescreen)
{
  GRPPROPERTYMODE mode = GRPPROPERTYMODE_XX_UNKNOWN;

  if((typefile == GRPBITMAPFILE_TYPE_PNG) || (typefile == GRPBITMAPFILE_TYPE_TGA))
    {
      switch(modescreen)
        {
          case GRPPROPERTYMODE_XX_UNKNOWN      : break;
          case GRPPROPERTYMODE_08_INDEX        : break;

          case GRPPROPERTYMODE_16_RGBA_4444    : mode = modescreen;
                                                 break;

          case GRPPROPERTYMODE_16_RGBA_5551    : mode = modescreen;
                                                 break;

          case GRPPROPERTYMODE_16_RGB_565      : break;
          case GRPPROPERTYMODE_16_SRGB_565     : break;
          case GRPPROPERTYMODE_24_RGB_888      : break;
          case GRPPROPERTYMODE_24_BGR_888      : break;

          case GRPPROPERTYMODE_32_RGBA_8888    : mode = modescreen;
                                                 break;

          case GRPPROPERTYMODE_32_BGRA_8888    : mode = modescreen;
                                                 break;
        }
    }
   else
    {
      mode = modescreen;
    }

  return mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ANIMATION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ANIMATION::Clean()
{
 
}



