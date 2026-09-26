/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Cache.h
* 
* @class      SCRIPT_CACHE
* @brief      Script Cache class
* @ingroup    SCRIPT
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
#include "XMap.h"
#include "XPath.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_CACHE_NOTFOUND   NOTFOUND

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SCRIPT_CACHE
{
  public:

    static bool                       GetIsInstanced              ();
    static SCRIPT_CACHE&              GetInstance                 ();
    static bool                       DelInstance                 ();

    XDWORD                            GenerateID                  (XSTRING& stringID);    

    bool                              Cache_Add                   (XDWORD ID, XSTRING* script);    
    XSTRING*                          Cache_Get                   (XDWORD ID, int* index = NULL);
    bool                              Cache_Set                   (XDWORD ID, XSTRING* script);
    bool                              Cache_Del                   (XDWORD ID);
    
    XMAP<XDWORD, XSTRING*>*           Cache_GetAll                ();
    bool                              Cache_DelAll                ();

    bool                              Cache_AllDirectory          (XPATH& xpath);
    bool                              Cache_AllList               (XVECTOR<XSTRING*>* listscripts);

  private:
                                      SCRIPT_CACHE                ();
                                      SCRIPT_CACHE                (SCRIPT_CACHE const&);
    virtual                          ~SCRIPT_CACHE                ();
    void                              operator =                  (SCRIPT_CACHE const&);

    void                              Clean                       ();

    static SCRIPT_CACHE*              instance;
    XMAP<XDWORD, XSTRING*>            cache;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






