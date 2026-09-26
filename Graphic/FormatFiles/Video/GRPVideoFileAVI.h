/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVideoFileAVI.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XPath.h"
#include "XFile.h"

#include "XFileRIFF.h"

#include "GRPVideoFile.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef struct
{
  XDWORD                                          microsecperframe;
  XDWORD                                          maxbytespersec;
  XDWORD                                          paddinggranularity;
  XDWORD                                          flags;
  XDWORD                                          totalframes;
  XDWORD                                          initialframes;
  XDWORD                                          streams;
  XDWORD                                          suggestedbuffersize;
  XDWORD                                          width;
  XDWORD                                          height;
  XDWORD                                          reserved[4];


} GRPVIDEOFILEAVI_MAINHEADER;

typedef struct 
{
  XDWORD                                          fcctype;
  XDWORD                                          fcchandler;
  XDWORD                                          flags;
  XWORD                                           priority;
  XWORD                                           language;
  XDWORD                                          initialframes;
  XDWORD                                          scale;
  XDWORD                                          rate;
  XDWORD                                          start;
  XDWORD                                          length;
  XDWORD                                          suggestedbuffersize;
  XDWORD                                          quality;
  XDWORD                                          samplesize;
  XDWORDSIG                                       rect[4];

} GRPVIDEOFILEAVI_STREAMHEADER;


typedef struct 
{
  XDWORD                                          size;
  XDWORDSIG                                       width;
  XDWORDSIG                                       height;
  XWORD                                           planes;
  XWORD                                           bitcount;
  XDWORD                                          compression;
  XDWORD                                          sizeimage;
  XDWORDSIG                                       xpelspermeter;
  XDWORDSIG                                       ypelspermeter;
  XDWORD                                          clrused;
  XDWORD                                          clrimportant;

} GRPVIDEOFILEAVI_BITMAPHEADER;


typedef struct 
{
  XDWORD                                          chunkID;
  XDWORD                                          flags;
  XDWORD                                          chunkoffset;
  XDWORD                                          chunklength;

} GRPVIDEOFILEAVI_INDEXENTRY;

#define GRPVIDEOFILEAVI_TYPECHUNKFRAME        __L("00dc")  
#define GRPVIDEOFILEAVI_INDEXENTRY_MAXSIZE    16

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPPROPERTY;
class GRPBITMAP;
class GRPBITMAPSEQUENCE;

class GRPVIDEOFILEAVI : public GRPVIDEOFILE
{
  public:
                                                  GRPVIDEOFILEAVI           ();
    virtual                                      ~GRPVIDEOFILEAVI           ();

    bool                                          Open                      (XCHAR* path);  
    bool                                          Create                    (XCHAR* path, GRPVIDEOFILE_PROPERTYS& propertys);

    XBYTE*                                        GetDataFrame              (XDWORD index_frame, XDWORD& sizeframe);
    bool                                          AddFrame                  (XBYTE* dataframe, XDWORD dataframesize);

    bool                                          Close                     ();        

  private:

    bool                                          CreateAndWriteJUNK        (XFILERIFF_LIST_NODE* father, XDWORD datasize);
    bool                                          CreateIndexofFrames       ();    

    void                                          Clean                     ();

    XFILERIFF*                                    fileRIFF;

    bool                                          iscreate;

    GRPVIDEOFILEAVI_MAINHEADER                    mainheader;
    GRPVIDEOFILEAVI_STREAMHEADER                  streamheader;
    GRPVIDEOFILEAVI_BITMAPHEADER                  bitmapheader;
     
    XFILERIFF_LIST_NODE*                          avi_node;
    XFILERIFF_LIST_NODE*                          hdrl_node;
    XFILERIFF_LIST_NODE*                          avih_node;
    XFILERIFF_LIST_NODE*                          strl_node;
    XFILERIFF_LIST_NODE*                          strh_node;
    XFILERIFF_LIST_NODE*                          strf_node;
    XFILERIFF_LIST_NODE*                          movi_node;
    
    XFILERIFF_LIST_NODE*                          frames_node;
    XVECTOR<GRPVIDEOFILEAVI_INDEXENTRY*>          indexentrys;  
    XDWORD                                        offsetforentrys;      
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






