/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBER_XEVENT.h
* 
* @class      XBER_XEVENT
* @brief      eXtended Utils XBER XEvent class
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

#include "XString.h"
#include "XBuffer.h"
#include "XVariant.h"
#include "XEvent.h"
#include "XBER.h"
#include "XASN1.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XBERXEVENT_TYPE
{
  XBERXEVENT_TYPE_UNKNOWN               =  XEVENT_TYPE_XBER     ,
  XBERXEVENT_TYPE_DECODE_START                                  ,                                          
  XBERXEVENT_TYPE_DECODE_DATA                                   ,          
  XBERXEVENT_TYPE_DECODE_END                                                                          
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBER;

class XBER_XEVENT : public XEVENT
{
  public:

                          XBER_XEVENT                   ();
                          XBER_XEVENT                   (XSUBJECT* subject, XDWORD type = XBERXEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_XBER);
    virtual              ~XBER_XEVENT                   ();
  
    int                   GetLevel                      ();  
    void                  SetLevel                      (int level);  

    XDWORD*               GetLevels                     ();   
    XSTRING*              GetLevelsString               ();   

    XSTRING*              GetLine                       ();   

    XBUFFER*              GetData                       ();

    XBYTE                 GetTagType                    ();
    void                  SetTagType                    (XBYTE tagtype);

    XSTRING*              GetTagTypeName                ();

    XBER_TAGCLASS         GetTagClass                   ();
    void                  SetTagClass                   (XBER_TAGCLASS tagclass);

    XASN1_OID_PROPERTY*   GetProperty                   ();
    void                  SetProperty                   (XASN1_OID_PROPERTY* property);

    XVARIANT*             GetValue                      ();

    bool                  GetStatus                     ();   
    void                  SetStatus                     (bool status);  

  
  private:

    void                  Clean                         ();

    XDWORD                level;
    XDWORD                levels[XBER_MAXLEVELS];
    XSTRING               levelsstr;

    XSTRING               line;

    XBUFFER               data;

    XBYTE                 tagtype;
    XSTRING               tagtypename;
    XBER_TAGCLASS         tagclass;
    XASN1_OID_PROPERTY*   property;

    XVARIANT              value;  

    bool                  status;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





