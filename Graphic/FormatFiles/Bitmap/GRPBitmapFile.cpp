/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFile.cpp
* 
* @class      GRPBITMAPFILE
* @brief      Graphics Bitmap File class
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

#include "GRPBitmapFile.h"

#include <string.h>

#include "XFactory.h"

#include "GRPProperties.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"

#ifdef GRP_BITMAP_FILE_BMP_ACTIVE
  #include "GRPBitmapFileBMP.h"
#endif
#ifdef GRP_BITMAP_FILE_JPG_ACTIVE
  #include "GRPBitmapFileJPG.h"
#endif
#ifdef GRP_BITMAP_FILE_PNG_ACTIVE
  #include "GRPBitmapFilePNG.h"
#endif
#ifdef GRP_BITMAP_FILE_TGA_ACTIVE
  #include "GRPBitmapFileTGA.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBASE::GRPBITMAPFILEBASE(): XFILECONTAINER()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBASE::GRPBITMAPFILEBASE(): XFILECONTAINER()
{
  Clean();

  file=GEN_XFACTORY.Create_File();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILEBASE::~GRPBITMAPFILEBASE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILEBASE::~GRPBITMAPFILEBASE()
{
  if(file) GEN_XFACTORY.Delete_File(file);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEBASE::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
* @brief      Create bitmap from file
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEBASE::CreateBitmapFromFile(XPATH& xpath, GRPPROPERTYMODE forcemode)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEBASE::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
* @brief      Create bitmap from buffer
* @ingroup    GRAPHIC
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEBASE::CreateBitmapFromBuffer(XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBASE::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
* @brief      Create file from bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  quality : Quality value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBASE::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap, int quality)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRPBITMAPFILEBASE::ReadImageBufferFromFile(XPATH& xpath)
* @brief      Read image buffer from file
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : path to the Graphic file
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPBITMAPFILEBASE::ReadImageBufferFromFile(XPATH& xpath)
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILEBASE::ForceModeOfBitmap(GRPPROPERTYMODE forcemode, GRPBITMAP* bitmap)
* @brief      Force mode of bitmap
* @ingroup    GRAPHIC
* 
* @param[in]  forcemode : Forcemode value.
* @param[in]  bitmap : Bitmap pointer to use.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILEBASE::ForceModeOfBitmap(GRPPROPERTYMODE forcemode, GRPBITMAP* bitmap)
{
  GRPBITMAP* newbitmap = bitmap;
  if(!newbitmap) return NULL;

  if(forcemode == GRPPROPERTYMODE_XX_UNKNOWN) return bitmap;
  if(forcemode == bitmap->GetMode())          return bitmap;

  newbitmap = bitmap->ConvertToMode(forcemode);
  if(!newbitmap) return bitmap;

  GEN_DELETE bitmap;

  return newbitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBASE::Open(XPATH& xpath, bool readonly)
* @brief      Open
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : path to the Graphic file
* @param[in]  readonly : is read only flag
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBASE::Open(XPATH& xpath, bool readonly)
{
  if(!file)                          return false;
  if(!file->Open(xpath, readonly))    return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBASE::Create(XPATH& xpath)
* @brief      Create resource
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : path to create the Graphic file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBASE::Create(XPATH& xpath)
{
  if(!file)                return false;
  if(!file->Create(xpath)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILEBASE::Close()
* @brief      Close
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILEBASE::Close()
{
  if(!file->Close()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILEBASE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILEBASE::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILE::GRPBITMAPFILE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILE::GRPBITMAPFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILE::GRPBITMAPFILE(XCHAR* xpath)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILE::GRPBITMAPFILE(XCHAR* xpath)
{
  Clean();

  this->xpath = xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILE::GRPBITMAPFILE(XPATH& xpath)
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : initial path to file
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILE::GRPBITMAPFILE(XPATH& xpath)
{
  Clean();

  this->xpath = xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILE::~GRPBITMAPFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILE::~GRPBITMAPFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* GRPBITMAPFILE::GetPath()
* @brief      Get path
* @ingroup    GRAPHIC
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* GRPBITMAPFILE::GetPath()
{
  return xpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILE::SetPath(XPATH& xpath)
* @brief      Set path
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path of file
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILE::SetPath(XPATH& xpath)
{
  this->xpath = xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILE::SetPath(XCHAR* xpath)
* @brief      Set path
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path of file
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILE::SetPath(XCHAR* xpath)
{
  this->xpath = xpath;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPFILE_TYPE GRPBITMAPFILE::GetTypeFromExtenxion(XCHAR* xpath)
* @brief      Get type from extenxion
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* 
* @return     GRPBITMAPFILE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPFILE_TYPE GRPBITMAPFILE::GetTypeFromExtenxion(XCHAR* xpath)
{
  XPATH               _xpath;
  XSTRING             ext;
  GRPBITMAPFILE_TYPE  type = GRPBITMAPFILE_TYPE_UNKNOWN;

  _xpath = xpath;

  _xpath.GetExt(ext);
  
  if(!ext.Compare(__L(".jpg"), true))  type = GRPBITMAPFILE_TYPE_JPG; 
  if(!ext.Compare(__L(".jpeg"), true))  type = GRPBITMAPFILE_TYPE_JPG;
  if(!ext.Compare(__L(".png"), true))  type = GRPBITMAPFILE_TYPE_PNG;
  if(!ext.Compare(__L(".bmp"), true))  type = GRPBITMAPFILE_TYPE_BMP;
  if(!ext.Compare(__L(".tga"), true))  type = GRPBITMAPFILE_TYPE_TGA;

  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILE::Load(XCHAR* xpath, GRPPROPERTYMODE forcemode)
* @brief      Load
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILE::Load(XCHAR* xpath, GRPPROPERTYMODE forcemode)
{
  if(!xpath) return NULL;

  XPATH               _xpath;
  GRPBITMAPFILE_TYPE  type          = GetTypeFromExtenxion(xpath);
  GRPBITMAPFILEBASE*  grpfilebase   = NULL;
  GRPBITMAP*          bitmap        = NULL;

  _xpath = xpath;

  switch(type)
    {
      case GRPBITMAPFILE_TYPE_UNKNOWN : 
                              default : break;

      #ifdef GRP_BITMAP_FILE_BMP_ACTIVE
      case GRPBITMAPFILE_TYPE_BMP     : grpfilebase = GEN_NEW GRPBITMAPFILEBMP(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_JPG_ACTIVE
      case GRPBITMAPFILE_TYPE_JPG     : grpfilebase = GEN_NEW GRPBITMAPFILEJPG(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_PNG_ACTIVE
      case GRPBITMAPFILE_TYPE_PNG     : grpfilebase = GEN_NEW GRPBITMAPFILEPNG(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_JPG_ACTIVE
      case GRPBITMAPFILE_TYPE_TGA     : grpfilebase = GEN_NEW GRPBITMAPFILETGA(); 
                                        break;  
      #endif
    }
  
  if(grpfilebase)
    {
      bitmap = grpfilebase->CreateBitmapFromFile(_xpath, forcemode);

      GEN_DELETE grpfilebase;
    }

  return bitmap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILE::Load(XPATH& xpath, GRPPROPERTYMODE forcemode)
* @brief      Load
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILE::Load(XPATH& xpath, GRPPROPERTYMODE forcemode)
{
  return Load(xpath.Get(), forcemode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* GRPBITMAPFILE::Load(GRPPROPERTYMODE forcemode)
* @brief      Load
* @ingroup    GRAPHIC
* 
* @param[in]  forcemode : Forcemode value.
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* GRPBITMAPFILE::Load(GRPPROPERTYMODE forcemode)
{
  if(xpath.IsEmpty()) return NULL;

  return Load(xpath, forcemode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILE::Save(XCHAR* xpath, GRPBITMAP* bitmap, int quality)
* @brief      Save
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  quality : Quality value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILE::Save(XCHAR* xpath, GRPBITMAP* bitmap, int quality)
{  
  if(!xpath) 
    {
      return false;
    }

  XPATH               _xpath;
  GRPBITMAPFILE_TYPE  type        = GetTypeFromExtenxion(xpath);
  GRPBITMAPFILEBASE*  grpfilebase = NULL;  
  bool                status      = false;

  _xpath = xpath;

  switch(type)
    {
      case GRPBITMAPFILE_TYPE_UNKNOWN : 
                            default   : break;

      #ifdef GRP_BITMAP_FILE_BMP_ACTIVE
      case GRPBITMAPFILE_TYPE_BMP     : grpfilebase = GEN_NEW GRPBITMAPFILEBMP(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_JPG_ACTIVE
      case GRPBITMAPFILE_TYPE_JPG     : grpfilebase = GEN_NEW GRPBITMAPFILEJPG(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_PNG_ACTIVE
      case GRPBITMAPFILE_TYPE_PNG     : grpfilebase = GEN_NEW GRPBITMAPFILEPNG(); 
                                        break;
      #endif

      #ifdef GRP_BITMAP_FILE_JPG_ACTIVE
      case GRPBITMAPFILE_TYPE_TGA     : grpfilebase = GEN_NEW GRPBITMAPFILETGA(); 
                                        break;  
      #endif
    }
   
  if(grpfilebase)
    {
      status = grpfilebase->CreateFileFromBitmap(_xpath, bitmap, quality);
      GEN_DELETE grpfilebase;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILE::Save(XPATH& xpath, GRPBITMAP* bitmap, int quality)
* @brief      Save
* @ingroup    GRAPHIC
* 
* @param[in]  xpath : Path to use.
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  quality : Quality value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILE::Save(XPATH& xpath, GRPBITMAP* bitmap, int quality)
{
  return Save(xpath.Get(), bitmap, quality);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPBITMAPFILE::Save(GRPBITMAP* bitmap, int quality)
* @brief      Save
* @ingroup    GRAPHIC
* 
* @param[in]  bitmap : Bitmap pointer to use.
* @param[in]  quality : Quality value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPBITMAPFILE::Save(GRPBITMAP* bitmap, int quality)
{
  if(xpath.IsEmpty()) return false;

  return Save(xpath, bitmap, quality);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAPSEQUENCE* GRPBITMAPFILE::LoadSequence(XCHAR* namemask, int inisequence, int endsequence)
* @brief      Load sequence
* @ingroup    GRAPHIC
* 
* @param[in]  namemask : string to mask
* @param[in]  inisequence : first ini of the sequence
* @param[in]  endsequence : end ini of the sequence
* 
* @return     GRPBITMAPSEQUENCE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAPSEQUENCE* GRPBITMAPFILE::LoadSequence(XCHAR* namemask, int inisequence, int endsequence)
{
  if(xpath.IsEmpty()) return NULL;

  GRPBITMAPSEQUENCE* bitmapsequence = GEN_NEW GRPBITMAPSEQUENCE();
  if(!bitmapsequence) return NULL;

  XSTRING namefile;

  //xpath.GetNamefileExt(namefile);
  //if(!namefile.IsEmpty()) return NULL;

  // The sequence is in individual files in a directory. Sequence numbers must be continuos
  int nfiles = inisequence;

  while((nfiles < endsequence)|| (endsequence < 0))
    {
      XPATH xpathtemp;
      bool  existfile = false;

      namefile.Format(namemask, nfiles);
      xpathtemp.Create(2, xpath.Get(), namefile.Get());

      XFILE* xfile=GEN_XFACTORY.Create_File();
      if(xfile)
        {
          existfile = xfile->Exist(xpathtemp);
          GEN_XFACTORY.Delete_File(xfile);
        }

      if(!existfile) break;

      GRPBITMAP* bitmap = Load(xpathtemp);
      if(!bitmap) break;

      bitmapsequence->AddFrame(bitmap);

      nfiles++;
    }

  return bitmapsequence;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPBITMAPFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPBITMAPFILE::Clean()
{

}





