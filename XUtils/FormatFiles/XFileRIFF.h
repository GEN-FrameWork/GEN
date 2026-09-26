/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileRIFF.h
* 
* @class      XFILERIFF
* @brief      eXtended Utils RIFF file class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFile.h"
#include "XTree.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class XFILERIFF_LIST;

typedef XTREE_NODE<XFILERIFF_LIST*>                      XFILERIFF_LIST_NODE;
typedef XTREE_NODE_ITERATOR<XFILERIFF_LIST*>             XFILERIFF_LIST_NODE_ITERATOR;
typedef XTREE<XFILERIFF_LIST*>                           XFILERIFF_XTREE;

#define XFILERIFF_TYPE_RIFF  0x46464952
#define XFILERIFF_TYPE_LIST  0x5453494C
#define XFILERIFF_TYPE_INFO  0x4F464E49

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILERIFF_LIST
{
  public:
                                    XFILERIFF_LIST            ();
    virtual                        ~XFILERIFF_LIST            ();

    XDWORD                          GetType                   ();
    void                            SetType                   (XDWORD type);

    bool                            IsTypeList                ();

    XDWORD                          GetSize                   ();
    void                            SetSize                   (XDWORD size);

    XDWORD                          GetTypeList               ();
    void                            SetTypeList               (XDWORD type);

    XQWORD                          GetPositionFileData       ();
    void                            SetPositionFileData       (XQWORD positionfiledata);

  private:    
    
    void                            Clean                     ();

    XDWORD                          type;
    XDWORD                          size;
    XDWORD                          typelist;
    XQWORD                          positionfiledata;

    #ifdef DEBUG_ACTIVE 
    
    char                            typestr[5];
    char                            typeliststr[5];
    
    #endif
};

class XFILERIFF 
{
  public:
                                    XFILERIFF                 ();
    virtual                        ~XFILERIFF                 ();

    bool                            Open                      (XCHAR* xpath, bool isreadonly = true);
    bool                            Create                    (XCHAR* xpath);
    
    bool                            ReadAllLists              ();
    XQWORD                          ReadNodeLists             (XFILERIFF_LIST_NODE* node);
  
    XDWORD                          GetTypeFromString         (XCHAR* string);

    XFILERIFF_LIST*                 GetList                   (XDWORD type, XDWORD fathertypelist = 0);
    XFILERIFF_LIST*                 GetList                   (XCHAR* typestr, XCHAR* fathertypeliststr = NULL);

    XFILERIFF_LIST*                 GetChunk                  (XDWORD type, XDWORD fathertypelist = 0);
    XFILERIFF_LIST*                 GetChunk                  (XCHAR* typestr, XCHAR* fathertypeliststr = NULL);

    bool                            GetChunkData              (XFILERIFF_LIST* chunkinfo, XBYTE* data, XDWORD& datasize);
    bool                            GetChunkData              (XFILERIFF_LIST* chunkinfo, XBUFFER& data);
    
    XFILERIFF_LIST_NODE*            GetListNode               (XDWORD type, XDWORD fathertypelist = 0);
    XFILERIFF_LIST_NODE*            GetListNode               (XCHAR* typestr, XCHAR* fathertypeliststr = NULL);

    XFILERIFF_LIST_NODE*            GetChunkNode              (XDWORD type, XDWORD fathertypelist = 0);
    XFILERIFF_LIST_NODE*            GetChunkNode              (XCHAR* typestr, XCHAR* fathertypeliststr = NULL);

    XFILERIFF_LIST_NODE*            CreateListNode            (XDWORD type, XDWORD typelist);   
    XFILERIFF_LIST_NODE*            CreateListNode            (XCHAR* typestr, XCHAR* typeliststr);
    
    XFILERIFF_LIST_NODE*            CreateChunkNode           (XDWORD type, XDWORD datasize);   
    XFILERIFF_LIST_NODE*            CreateChunkNode           (XCHAR* typestr, XDWORD datasize);

    bool                            UpdateFilePosition        (XFILERIFF_LIST_NODE* node);
    bool                            WriteListToFile           (XFILERIFF_LIST* list, XBYTE* data = NULL, XDWORD datasize = 0);  
    
    XFILERIFF_XTREE*                GetXTreeList              ();  
    XFILE*                          GetFileBase               ();  

    bool                            Close                     (); 

    XDWORD                          AdjustSizeOfLists         (XFILERIFF_LIST_NODE* node);


  private:

    bool                            ConvertDWORDtoString      (XDWORD data, XSTRING& string);

    XFILERIFF_LIST*                 GetList                   (bool search_chunk, XFILERIFF_LIST_NODE* listnode, XDWORD type, XDWORD fathertypelist);
    XFILERIFF_LIST_NODE*            GetListNode               (bool search_chunk, XFILERIFF_LIST_NODE* listnode, XDWORD type, XDWORD fathertypelist);

   
    void                            Clean                     ();

    XFILE*                          xfilebase; 
    bool                            iscreatefile;        
    XFILERIFF_XTREE                 xtreelist; 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





