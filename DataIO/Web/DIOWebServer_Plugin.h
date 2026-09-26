/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_Plugin.h
* 
* @class      DIOWEBSERVER_PLUGIN
* @brief      Data Input/Output Web Server Plugin class
* @ingroup    DATAIO
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
#include "XPath.h"
#include "XVector.h"

#include "DIOURL.h"
#include "DIOWebHeader.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWEBSERVER_QUERYSTRINGS;
class DIOWEBSERVER_REQUEST;

class DIOWEBSERVER_PLUGIN
{
  public:
                                          DIOWEBSERVER_PLUGIN           ();
    virtual                              ~DIOWEBSERVER_PLUGIN           ();

    XSTRING*                              GetName                       ();

    bool                                  PageExtension_Add             (XCHAR* ext);
    bool                                  PageExtension_Add             (XSTRING& ext);
    XSTRING*                              PageExtension_Get             (int index);
    bool                                  PageExtension_IsContent       (XCHAR* ext);
    bool                                  PageExtension_IsContent       (XSTRING& ext);
    bool                                  PageExtension_DeleteAll       ();

    virtual bool                          PageExtension                 (XPATH& pathfile, DIOWEBSERVER_REQUEST* request, DIOWEBSERVER_QUERYSTRINGS* querystrings, DIOWEBHEADER_RESULT& headerresult, XSTRING& result, bool istls = false);

    bool                                  SeparedResult                 (XSTRING& result, XSTRING& head, XSTRING& data);

  private:

    void                                  Clean                         ();

    XSTRING                               name;
    XVECTOR<XSTRING*>                     pageextensions;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





