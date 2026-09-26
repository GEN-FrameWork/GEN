/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFTextSection.h
* 
* @class      GRPVECTORFILEDXFTEXTSECTION
* @brief      Graphic Vector File DXF Text Section class
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

#include "XSubject.h"
#include "XVariant.h"

#include "GRPVectorFile.h"
#include "GRPVectorFileDXFTextPart.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum GRPVECTORFILEDXFTEXTSECTION_BASICTYPE
{
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_UNKNOWN           = 0 ,
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_BOOLEAN               ,
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_INTEGER               ,
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_DOUBLE                ,
  GRPVECTORFILEDXFTEXTSECTION_BASICTYPE_STRING                ,
};


enum GRPVECTORFILEDXFText_XDataCtrl_Status
{
  GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_NOT        = 0 ,
  GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_INI            ,
  GRPVECTORFILEDXFTEXTSECTION_XDATACTRL_STATUS_END            ,
};


enum GRPVECTORFILEDXFTEXTSECTION_TYPESECTION
{
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_UNKNOWN         = 0 ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_HEADER              ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_CLASSES             ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_TABLES              , 
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_BLOCKS              , 
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ENTITIES            ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_OBJECTS             ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_ACDSDATA            ,
  GRPVECTORFILEDXFTEXTSECTION_TYPESECTION_THUMBNAILIMAGE      ,
};

#define GRPVECTORFILEDXFTEXTSECTION_MAXNDEFSECTIONS           8
#define GRPVECTORFILEDXFTEXTSECTION_MAXNGENERICDEFTYPES       93




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILETXT;

typedef struct
{
   XCHAR*                                               name;
   GRPVECTORFILEDXFTEXTSECTION_TYPESECTION              type;
   
} GRPVECTORFILEDXFDEFTEXTSECTION;

typedef struct
{  
   int                                                  type;
   int                                                  range;
   XCHAR*                                               name;
   XCHAR*                                               remark;

} GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE;

class GRPVECTORFILEDXFTEXTSECTION : public XSUBJECT, public GRPVECTORFILEDXFTEXTPART
{
  public:

                                                        GRPVECTORFILEDXFTEXTSECTION         ();
    virtual                                            ~GRPVECTORFILEDXFTEXTSECTION         ();    
   
    GRPVECTORFILE*                                      GetGrpVectorFile                    ();  
    void                                                SetGrpVectorFile                    (GRPVECTORFILE* vectorFile);  

    static GRPVECTORFILEDXFTEXTSECTION*                 CreateInstance                      (GRPVECTORFILEDXFTEXTSECTION_TYPESECTION type);   
    
    static GRPVECTORFILEDXFTEXTSECTION_TYPESECTION      GetTypeSection                      (XSTRING& namesection);
    
    static GRPVECTORFILEDXFTEXTSECTION_BASICTYPE        GetTypeBasic                        (int type);    
    GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE*          GetGenericDefType                   (int type);
    static void                                         GetVariableFromLine                 (XCHAR* namevar, int type, XSTRING* line, XVARIANT& variant);  
    GRPVECTORFILEDXFText_XDataCtrl_Status               IsXDataControl                      (int type, XSTRING& line);


    virtual GRPVECTORFILERESULT                         ParserTextSection                   (XFILETXT* filetxt);
    
    GRPVECTORFILEDXFTEXTSECTION_TYPESECTION             type;
    static GRPVECTORFILEDXFDEFTEXTSECTION               defsection[GRPVECTORFILEDXFTEXTSECTION_MAXNDEFSECTIONS];
    static GRPVECTORFILEDXFTEXTSECTIONGENERICDEFTYPE    genericdef[GRPVECTORFILEDXFTEXTSECTION_MAXNGENERICDEFTYPES];

  private:
    
    void                                                Clean                               ();

    GRPVECTORFILE*                                      vectorfile;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





