/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSharedMemoryManager.h
* 
* @class      XSHAREDMEMORYMANAGER
* @brief      eXtended Utils Shared memory Manager class
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

#include "XBase.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSHAREDMEMORYMANAGER
{
  public:
                                  XSHAREDMEMORYMANAGER        ();
    virtual                      ~XSHAREDMEMORYMANAGER        ();

    static bool                   GetIsInstanced              ();
    static XSHAREDMEMORYMANAGER&  GetInstance                 ();
    static bool                   SetInstance                 (XSHAREDMEMORYMANAGER* instance);
    static bool                   DelInstance                 ();

    virtual XBYTE*                Create                      (XCHAR* ID, XDWORD size, bool ispublic = true);
    XBYTE*                        Create                      (XSTRING& ID, XDWORD size, bool ispublic = true);

    virtual XBYTE*                Open                        (XCHAR* ID, XDWORD& size);
    XBYTE*                        Open                        (XSTRING& ID, XDWORD& size);

    XBYTE*                        GetPointer                  ();

    virtual bool                  Close                       ();

    bool                          IsServer                    ();

  protected:
    
    XSTRING                       ID;
    XDWORD                        size;
    XBYTE*                        base;
    XBYTE*                        pointer;
    bool                          isserver;
 
  private:

    void                          Clean                       ();

    static XSHAREDMEMORYMANAGER*  instance;  
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





