/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVideoFileAVI.cpp
* 
* @class      GRPVIDEOFILEAVI
* @brief      Graphic Video File AVI class
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

#include "GRPVideoFileAVI.h"

#include <string.h>

#include "XFactory.h"

#include "GRPProperties.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIDEOFILEAVI::GRPVIDEOFILEAVI()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIDEOFILEAVI::GRPVIDEOFILEAVI()
{
  Clean();

  fileRIFF = GEN_NEW XFILERIFF();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVIDEOFILEAVI::~GRPVIDEOFILEAVI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVIDEOFILEAVI::~GRPVIDEOFILEAVI()
{
  if(fileRIFF)
    {
      //fileRIFF->Close();

      GEN_DELETE fileRIFF;
      fileRIFF = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::Open(XCHAR* path)
* @brief      Open
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::Open(XCHAR* path)
{
  if(!fileRIFF->Open(path)) return false;

  if(!fileRIFF->ReadAllLists()) return false; 

  XDWORD sizedata = 0;

  memset((XBYTE*)&mainheader, 0, sizeof(GRPVIDEOFILEAVI_MAINHEADER));
  memset((XBYTE*)&streamheader, 0, sizeof(GRPVIDEOFILEAVI_STREAMHEADER));
  memset((XBYTE*)&bitmapheader, 0, sizeof(GRPVIDEOFILEAVI_BITMAPHEADER));

  XFILERIFF_LIST* chunk_mainheader = fileRIFF->GetChunk(_L("avih"), _L("hdrl"));
  if(!chunk_mainheader) return false;
  
  fileRIFF->GetChunkData(chunk_mainheader, (XBYTE*)&mainheader, sizedata);

  XFILERIFF_LIST* strl_list = fileRIFF->GetList(_L("strl"), _L("hdrl"));
  if(!strl_list) return false;

  XFILERIFF_LIST* strh_chunk = fileRIFF->GetChunk(_L("strh"), _L("strl"));
  if(!strh_chunk) return false;

  fileRIFF->GetChunkData(strh_chunk, (XBYTE*)&streamheader, sizedata);
  
  XFILERIFF_LIST* strf_chunk = fileRIFF->GetChunk(_L("strf"), _L("strl"));
  if(!strf_chunk) return false;
  
  fileRIFF->GetChunkData(strf_chunk, (XBYTE*)&bitmapheader, sizedata);

  propertys.width       = mainheader.width;
  propertys.height      = mainheader.height;  
  propertys.nframes     = mainheader.totalframes;
  propertys.framerate   = streamheader.rate;

  frames_node = fileRIFF->GetListNode(_L("movi"));
  if(!frames_node) return false;

  XFILERIFF_LIST* index_list = fileRIFF->GetChunk(_L("idx1"), _L("AVI "));
  if(!index_list) return false;
 
  fileRIFF->GetFileBase()->SetPosition(index_list->GetPositionFileData());

  for(XDWORD index=0; index < propertys.nframes; index++)
    { 
      GRPVIDEOFILEAVI_INDEXENTRY* indexentry = GEN_NEW GRPVIDEOFILEAVI_INDEXENTRY;
      if(!indexentry) return false;  

      if(!fileRIFF->GetFileBase()->Read((XBYTE*)indexentry, sizeof(GRPVIDEOFILEAVI_INDEXENTRY))) return false;

      indexentrys.Add(indexentry);
    }

  iscreate = false;
   
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::Create(XCHAR* path, GRPVIDEOFILE_PROPERTYS& propertys)
* @brief      Create resource
* @ingroup    GRAPHIC
* 
* @param[in]  path : Path to use.
* @param[in]  propertys : Propertys value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::Create(XCHAR* path, GRPVIDEOFILE_PROPERTYS& propertys)
{
  if(!fileRIFF->Create(path)) return false; 

  //--------------------------------------------------------------------
  // AVI (Root)

  avi_node = fileRIFF->CreateListNode(_L("RIFF"), _L("AVI "));
  if(!avi_node) return false;

  fileRIFF->UpdateFilePosition(avi_node);  
  if(!fileRIFF->WriteListToFile(avi_node->GetData())) return false;  

  fileRIFF->GetXTreeList()->SetRoot(avi_node);


  //--------------------------------------------------------------------
  // AVI->hdrl

  hdrl_node = fileRIFF->CreateListNode(_L("LIST"), _L("hdrl"));
  if(!hdrl_node) return false;

  fileRIFF->UpdateFilePosition(hdrl_node);  
  if(!fileRIFF->WriteListToFile(hdrl_node->GetData())) return false;  

  avi_node->AddChild(hdrl_node);


  //--------------------------------------------------------------------
  // AVI->hdrl->avih

  avih_node = fileRIFF->CreateChunkNode(_L("avih"), sizeof(GRPVIDEOFILEAVI_MAINHEADER));
  if(!avih_node) return false;

  memset((XBYTE*)&mainheader, 0, sizeof(GRPVIDEOFILEAVI_MAINHEADER));
  
  mainheader.microsecperframe     = 40000;               
  mainheader.maxbytespersec       = 40000;                 
  mainheader.paddinggranularity   = 0;             
  mainheader.flags                = 2320;                          
  mainheader.totalframes          = 0;                    
  mainheader.initialframes        = 0;
  mainheader.streams              = 1;
  mainheader.suggestedbuffersize  = 1048576;
  mainheader.width                = propertys.width;
  mainheader.height               = propertys.height;
  mainheader.reserved[0]          = 0;
  mainheader.reserved[1]          = 0;
  mainheader.reserved[2]          = 0;
  mainheader.reserved[3]          = 0;
  
  fileRIFF->UpdateFilePosition(avih_node);    
  if(!fileRIFF->WriteListToFile(avih_node->GetData(), (XBYTE*)&mainheader, sizeof(GRPVIDEOFILEAVI_MAINHEADER))) return false;  
  
  hdrl_node->AddChild(avih_node);


  //--------------------------------------------------------------------
  // AVI->hdrl->srtl
  
  strl_node = fileRIFF->CreateListNode(_L("LIST"), _L("strl"));
  if(!strl_node) return false;

  fileRIFF->UpdateFilePosition(strl_node);  
  if(!fileRIFF->WriteListToFile(strl_node->GetData())) return false;  

  hdrl_node->AddChild(strl_node);


  //--------------------------------------------------------------------
  // AVI->hdrl->srtl->strh
  
  strh_node = fileRIFF->CreateChunkNode(_L("strh"), sizeof(GRPVIDEOFILEAVI_STREAMHEADER));
  if(!strh_node) return false;

  memset((XBYTE*)&streamheader, 0, sizeof(GRPVIDEOFILEAVI_STREAMHEADER));

  if(!propertys.framerate) propertys.framerate = GRPVIDEOFILE_DEFAULTFRAMERATE;

  streamheader.fcctype              = fileRIFF->GetTypeFromString(_L("vids"));
  streamheader.fcchandler           = fileRIFF->GetTypeFromString(propertys.codecstr.Get());  
  streamheader.flags                = 0;
  streamheader.priority             = 0;
  streamheader.language             = 0;
  streamheader.initialframes        = 0;
  streamheader.scale                = 1;
  streamheader.rate                 = propertys.framerate;       // dwRate / dwScale == samples/second 
  streamheader.start                = 0;
  streamheader.length               = 497;                       // In units above... 
  streamheader.suggestedbuffersize  = 1048576295;
  streamheader.quality              = 0xFFFFFFFF;
  streamheader.samplesize           = 0;
  streamheader.rect[0]              = 0;
  streamheader.rect[1]              = 0;
  streamheader.rect[2]              = propertys.width;
  streamheader.rect[3]              = propertys.height;
  
  fileRIFF->UpdateFilePosition(strh_node);    
  if(!fileRIFF->WriteListToFile(strh_node->GetData(), (XBYTE*)&streamheader, sizeof(GRPVIDEOFILEAVI_STREAMHEADER))) return false;  
  
  strl_node->AddChild(strh_node);


  //--------------------------------------------------------------------
  // AVI->hdrl->srtl->strf
  
  strf_node = fileRIFF->CreateChunkNode(_L("strf"), sizeof(GRPVIDEOFILEAVI_BITMAPHEADER));
  if(!strf_node) return false;

  memset((XBYTE*)&bitmapheader, 0, sizeof(GRPVIDEOFILEAVI_BITMAPHEADER));

  bitmapheader.size                 = sizeof(GRPVIDEOFILEAVI_BITMAPHEADER);
  bitmapheader.width                = propertys.width;
  bitmapheader.height               = propertys.height;
  bitmapheader.planes               = 1;
  bitmapheader.bitcount             = 24;
  bitmapheader.compression          = fileRIFF->GetTypeFromString(propertys.codecstr.Get());
  bitmapheader.sizeimage            = (propertys.width * propertys.height) * (bitmapheader.bitcount/3);
  bitmapheader.xpelspermeter        = 0;
  bitmapheader.ypelspermeter        = 0;
  bitmapheader.clrused              = 0;
  bitmapheader.clrimportant         = 0;
    
  fileRIFF->UpdateFilePosition(strf_node);    
  if(!fileRIFF->WriteListToFile(strf_node->GetData(), (XBYTE*)&bitmapheader, sizeof(GRPVIDEOFILEAVI_BITMAPHEADER))) return false;  
  
  strl_node->AddChild(strf_node);


  //--------------------------------------------------------------------
  // AVI->MOVI
  
  movi_node = fileRIFF->CreateListNode(_L("LIST"), _L("movi"));
  if(!movi_node) return false;

  fileRIFF->UpdateFilePosition(movi_node);  
  if(!fileRIFF->WriteListToFile(movi_node->GetData())) return false;  

  avi_node->AddChild(movi_node);

  //--------------------------------------------------------------------
  

  offsetforentrys = 4;  // initial offset

  iscreate = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* GRPVIDEOFILEAVI::GetDataFrame(XDWORD index_frame, XDWORD& sizeframe)
* @brief      Get data frame
* @ingroup    GRAPHIC
* 
* @param[in]  index_frame : Index frame value.
* @param[in]  sizeframe : Sizeframe value.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* GRPVIDEOFILEAVI::GetDataFrame(XDWORD index_frame, XDWORD& sizeframe)
{
  if(!frames_node)                      return NULL;
  if(index_frame >= propertys.nframes)  return NULL;

  XFILERIFF_LIST_NODE* frame_node = frames_node->GetChild(index_frame);
  if(!frame_node) return NULL;

  XFILERIFF_LIST* frame_list = frame_node->GetData();
  if(!frame_list) return NULL;

  XDWORD datasize = frame_list->GetSize();

  GRPVIDEOFILEAVI_INDEXENTRY* indexentry = indexentrys.Get(index_frame);
  if(!indexentry) return NULL;
    
  if(datasize != indexentry->chunklength) return NULL;  
    
  XBYTE* data = GEN_NEW XBYTE[datasize];
  if(!data) return NULL;
  
  if(!fileRIFF->GetFileBase()->SetPosition(frame_list->GetPositionFileData()))
    {
      GEN_DELETE_ARRAY data;
      return NULL;
    }

  if(!fileRIFF->GetFileBase()->Read(data, datasize))
    {
      GEN_DELETE_ARRAY data;
      return NULL;
    }

  sizeframe = datasize;

  return data;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::AddFrame(XBYTE* dataframe, XDWORD dataframesize)
* @brief      Add frame
* @ingroup    GRAPHIC
* 
* @param[in]  dataframe : Dataframe pointer to use.
* @param[in]  dataframesize : Dataframesize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::AddFrame(XBYTE* dataframe, XDWORD dataframesize)
{
  if(!dataframe)      return false;
  if(!dataframesize)  return false;

  if(!movi_node)      return false;


  XDWORD framesize = dataframesize;

  //framesize += ((framesize % 2)?1:0);

  //--------------------------------------------------------------------
  // AVI->movi->00dc
  
  XFILERIFF_LIST_NODE* frame_node = fileRIFF->CreateChunkNode(GRPVIDEOFILEAVI_TYPECHUNKFRAME, framesize);
  if(!frame_node) return false;
    
  fileRIFF->UpdateFilePosition(frame_node);    
  if(!fileRIFF->WriteListToFile(frame_node->GetData(), dataframe, framesize)) return false;  

  mainheader.totalframes++;
  movi_node->AddChild(frame_node);
  

  GRPVIDEOFILEAVI_INDEXENTRY* indexentry = GEN_NEW GRPVIDEOFILEAVI_INDEXENTRY();
  if(indexentry) 
    {
      indexentry->chunkID     = fileRIFF->GetTypeFromString(GRPVIDEOFILEAVI_TYPECHUNKFRAME);
      indexentry->flags       = 16;
      indexentry->chunklength = framesize;
      indexentry->chunkoffset = offsetforentrys;

      indexentrys.Add(indexentry);

      //offsetforentrys += (framesize + sizeof(GRPVIDEOFILEAVI_INDEXENTRY));

      offsetforentrys += (framesize + 8);
    }
 
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::Close()
* @brief      Close
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::Close()
{
  if(iscreate)
    {
      CreateIndexofFrames();

      fileRIFF->AdjustSizeOfLists(fileRIFF->GetXTreeList()->GetRoot());

      if(!fileRIFF->WriteListToFile(avih_node->GetData(), (XBYTE*)&mainheader, sizeof(GRPVIDEOFILEAVI_MAINHEADER))) return false;  

      avi_node  = NULL;
      hdrl_node = NULL;
      avih_node = NULL;
      strl_node = NULL;
      strh_node = NULL;
      strf_node = NULL;
      movi_node = NULL;
    }

  fileRIFF->Close();

  indexentrys.DeleteContents();
  indexentrys.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::CreateAndWriteJUNK(XFILERIFF_LIST_NODE* father_node, XDWORD datasize)
* @brief      Create and write JUNK
* @ingroup    GRAPHIC
* 
* @param[in]  father_node : Father node pointer to use.
* @param[in]  datasize : Datasize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::CreateAndWriteJUNK(XFILERIFF_LIST_NODE* father_node, XDWORD datasize)
{
  if(!father_node) return false;

  XFILERIFF_LIST_NODE* junk_node = fileRIFF->CreateChunkNode(_L("JUNK"), datasize);
  if(!junk_node) return false;

  XBYTE* data = GEN_NEW XBYTE[datasize];
  if(!data) return false;

  memset(data, 0, datasize);

  fileRIFF->UpdateFilePosition(junk_node);    
  if(!fileRIFF->WriteListToFile(junk_node->GetData(), data, datasize)) return false; 

  father_node->AddChild(junk_node);

  GEN_DELETE_ARRAY data;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVIDEOFILEAVI::CreateIndexofFrames()
* @brief      Create indexof frames
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVIDEOFILEAVI::CreateIndexofFrames()
{
  if(!avi_node) return false;

  XDWORD sizeindex = sizeof(GRPVIDEOFILEAVI_INDEXENTRY) * mainheader.totalframes;
  
  XFILERIFF_LIST_NODE* index_node = fileRIFF->CreateChunkNode(_L("idx1"), sizeindex);
  if(!index_node) return false;

  fileRIFF->UpdateFilePosition(index_node);    
  if(!fileRIFF->WriteListToFile(index_node->GetData(), NULL, sizeindex)) return false;  

  avi_node->AddChild(index_node);

  for(XDWORD c=0; c<indexentrys.GetSize(); c++)
    {
      GRPVIDEOFILEAVI_INDEXENTRY* indexentry = indexentrys.Get(c);  
      if(indexentry)
        {
          if(!fileRIFF->GetFileBase()->Write((XBYTE*)indexentry, sizeof(GRPVIDEOFILEAVI_INDEXENTRY))) return false;
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVIDEOFILEAVI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVIDEOFILEAVI::Clean()
{
  iscreate        = false;

  memset((XBYTE*)&mainheader, 0, sizeof(GRPVIDEOFILEAVI_MAINHEADER));
  memset((XBYTE*)&streamheader, 0, sizeof(GRPVIDEOFILEAVI_STREAMHEADER));
  memset((XBYTE*)&bitmapheader, 0, sizeof(GRPVIDEOFILEAVI_BITMAPHEADER));

  avi_node        = NULL;
  hdrl_node       = NULL;
  avih_node       = NULL;
  strl_node       = NULL;
  strh_node       = NULL;
  strf_node       = NULL;
  movi_node       = NULL;

  frames_node     = NULL;
  offsetforentrys = 0;
}



