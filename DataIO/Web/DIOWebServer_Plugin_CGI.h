/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_Plugin_CGI.h
* 
* @class      DIOWEBSERVER_PLUGIN_CGI
* @brief      Data Input/Output Web Server CGI (Common Gateway Interface) Plugin class
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

#include "XPath.h"

#include "DIOWebServer_Plugin.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOWEBSERVER_PLUGIN_CGI : public DIOWEBSERVER_PLUGIN
{
  public:
                          DIOWEBSERVER_PLUGIN_CGI     ();
    virtual              ~DIOWEBSERVER_PLUGIN_CGI     ();

    bool                  Config                      (XPATH* pathCGIinterpreter, XCHAR* nameexec = NULL);
    bool                  PageExtension               (XPATH& pathfile, DIOWEBSERVER_REQUEST* request, DIOWEBSERVER_QUERYSTRINGS* querystrings, DIOWEBHEADER_RESULT& headerresult, XSTRING& result, bool istls = false);

  protected:

    XPATH                 pathCGIinterpreter;
    XSTRING               nameexec;

  private:

    void                  Clean                       ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





