/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFile.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XPath.h"
#include "XSubject.h"

#include "GRPVectorFileConfig.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPVECTORFILERESULT
{
  GRPVECTORFILERESULT_OK                        = 0 ,  
  GRPVECTORFILERESULT_WARNINGTYPEFILEUNKNOWN        ,
  GRPVECTORFILERESULT_ERRORNOTSUPPORTED             ,           
  GRPVECTORFILERESULT_ERRORUNKNOWN                  , 
  GRPVECTORFILERESULT_ERRORNOTMEMORY                , 
  GRPVECTORFILERESULT_ERRORNOTFILE                  , 
  GRPVECTORFILERESULT_ERRORINVALIDFORMAT            ,

  GRPVECTORFILERESULT_LASTCOMMON
};

enum GRPVECTORFILETYPE
{
  GRPVECTORFILETYPE_UNKNOWN                        = 0 ,
  GRPVECTORFILETYPE_DXF                                ,   
  GRPVECTORFILETYPE_SVG                                ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILETXT;

class GRPVECTORFILE : public XSUBJECT
{
  public:

                                      GRPVECTORFILE                 ();    
    virtual                          ~GRPVECTORFILE                 ();

    static GRPVECTORFILE*             CreateInstance                (XPATH& pathfile);   
    static GRPVECTORFILE*             CreateInstance                (GRPVECTORFILETYPE type);
    static GRPVECTORFILE*             CreateInstance                (GRPVECTORFILETYPE type, XSTRING& content);
     
    XPATH*                            GetPathFile                   ();

    GRPVECTORFILETYPE                 GetType                       ();
    void                              SetType                       (GRPVECTORFILETYPE type);
    static XCHAR*                     GetTypeText                   (GRPVECTORFILETYPE type);

    virtual GRPVECTORFILERESULT       DetectType                    ();

    virtual GRPVECTORFILERESULT       Load                          ();
    virtual GRPVECTORFILERESULT       Load                          (XSTRING& content);
          
    virtual GRPVECTORFILECONFIG*      GetConfig                     ();

  protected:
    
    bool                              DetectFileFormatText          (XPATH& pathfile);

    static bool                       PopulateTextFileFromString    (XFILETXT* file, XSTRING& content);
    
    XPATH                             pathfile;
    GRPVECTORFILETYPE                 type;
      
  private:

    static bool                       DetectFile                    (XPATH& pathfile); 
    bool                              IsConversionFile              ();
    void                              SetIsConversionFile           (bool isconversionfile);
    
    void                              Clean                         ();

    bool                              isconversionfile;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





