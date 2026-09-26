/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFile_XEvent.h
* 
* @class      GRPVECTORFILE_XEVENT
* @brief      Graphic Vector File eXtended Event class
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

#include "XEvent.h"
#include "XSubject.h"

#include "GRPVectorFile.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPVECTORFILE_XEVENTTYPE : XDWORD
{
   GRPVECTORFILE_XEVENTTYPE_UNKNOWN         = XEVENT_TYPE_VECTORFILE ,
   GRPVECTORFILE_XEVENTTYPE_PARTUNKNOWN                             
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPUBLISHER;

class GRPVECTORFILE_XEVENT : public XEVENT
{
  public:
                            GRPVECTORFILE_XEVENT        (XSUBJECT* subject, XDWORD type = GRPVECTORFILE_XEVENTTYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_VECTORFILE);
    virtual                ~GRPVECTORFILE_XEVENT        ();

    GRPVECTORFILETYPE       GetType                     ();
    void                    SetType                     (GRPVECTORFILETYPE type);

    XPATH*                  GetPath                     ();

    XSTRING*                GetMsg                      ();

  private:

    void                    Clean                       ();

    XPATH                   pathfile;
    GRPVECTORFILETYPE       type;
    XPATH                   path;
    XSTRING                 message;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/








