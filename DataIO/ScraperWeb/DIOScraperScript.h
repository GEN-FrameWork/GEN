/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperScript.h
* @class      DIOSCRAPERSCRIPT
* @brief      Runs a scraper script and exchanges typed arg/result maps with SCRIPT_LIB_SCRAPER
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

#include "XMap.h"
#include "XString.h"
#include "XPath.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT   10



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSCRAPERSCRIPT
{
  public:
                                    DIOSCRAPERSCRIPT              ();
    virtual                        ~DIOSCRAPERSCRIPT              ();

    bool                            ClearArgs                     ();
    bool                            ClearResults                  ();

    bool                            SetArg                        (XCHAR* name, XCHAR* value);
    bool                            SetArg                        (XCHAR* name, XSTRING& value);
    bool                            SetArgInt                     (XCHAR* name, int value);

    bool                            GetArg                        (XCHAR* name, XSTRING& value);
    bool                            GetArgInt                     (XCHAR* name, int& value);

    bool                            SetResult                     (XCHAR* name, XCHAR* value);
    bool                            SetResult                     (XCHAR* name, XSTRING& value);
    bool                            GetResult                     (XCHAR* name, XSTRING& value);
    bool                            HaveResult                    (XCHAR* name);

    /**
     * @brief Resolve script under scripts root (e.g. scrapers/publicip.g) and run it.
     */
    bool                            Run                           (XCHAR* relativescriptpath);

    /**
     * @brief Run an absolute/resolved script path.
     */
    bool                            RunPath                       (XPATH& scriptpath);

  private:

    void                            Clean                         ();
    bool                            DeleteMapContents             (XMAP<XSTRING*, XSTRING*>& map);

    XMAP<XSTRING*, XSTRING*>        args;
    XMAP<XSTRING*, XSTRING*>        results;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

