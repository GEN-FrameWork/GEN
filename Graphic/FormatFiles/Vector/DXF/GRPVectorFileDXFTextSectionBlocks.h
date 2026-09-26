/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSectionBlocks.h
* 
* @class      GRPVECTORFILEDXFTEXTSECTIONBLOCKS
* @brief      Graphic Vector File DXF Text Section Blocks class
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

#include "XMap.h"
#include "XVariant.h"
#include "XString.h"

#include "GRPVectorFileDXFTextSection.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPVECTORFILEDXFBLOCKS_MAXNDEFBLOCKS    2
#define GRPVECTORFILEDXFBLOCKS_MAXNDEFTYPES     35

#undef TEST_ONLY_DEFINE_IN_BLOCKS



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPVECTORFILEDXFTEXTBLOCK;

typedef struct
{
  int                                             type;
  XCHAR*                                          name;   
  XCHAR*                                          remark;   

} GRPVECTORFILEDXFTEXTSECTIONBLOCKDEFTYPE;

typedef struct
{
   XCHAR*                                         name;
   int                                            ntypes;
   bool                                           isendblock;
   GRPVECTORFILEDXFTEXTSECTIONBLOCKDEFTYPE        type[GRPVECTORFILEDXFBLOCKS_MAXNDEFTYPES];

} GRPVECTORFILEDXFTEXTSECTIONBLOCKDEF;

class GRPVECTORFILEDXFTEXTSECTIONBLOCKS : public GRPVECTORFILEDXFTEXTSECTION
{
  public:

                                                  GRPVECTORFILEDXFTEXTSECTIONBLOCKS               ();
    virtual                                      ~GRPVECTORFILEDXFTEXTSECTIONBLOCKS               ();
    
    GRPVECTORFILEDXFTEXTSECTIONBLOCKDEF*          IsKnownBlock                                    (XSTRING& name);
    GRPVECTORFILEDXFTEXTSECTIONBLOCKDEFTYPE*      IsKnownTypeValue                                (XSTRING& namevar, int type);

    bool                                          AddBlock                                        (GRPVECTORFILEDXFTEXTBLOCK* entity);
    XVECTOR<GRPVECTORFILEDXFTEXTBLOCK*>*          GetBlocks                                       ();
    GRPVECTORFILEDXFTEXTBLOCK*                    GetBlock                                        (XCHAR* nameblock, XDWORD index);
    bool                                          DeleteBlock                                     (XCHAR* nameblock, XDWORD index);
    bool                                          DeleteAllBlocks                                 (XCHAR* nameblock);
    bool                                          DeleteAllBlocks                                 ();

    GRPVECTORFILERESULT                           ParserTextSection                               (XFILETXT* file);

  private:
        
    bool                                          ShowTraceAllBlocks                              ();
    void                                          Clean                                           ();    

    static GRPVECTORFILEDXFTEXTSECTIONBLOCKDEF    defBlock[GRPVECTORFILEDXFBLOCKS_MAXNDEFBLOCKS];
    XVECTOR<GRPVECTORFILEDXFTEXTBLOCK*>           blocks;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





