/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSRegistryManager.h
* 
* @class      XWINDOWSREGISTRYMANAGER
* @brief      WINDOWS eXtended Utils Registry Manager class
* @ingroup    PLATFORM_WINDOWS
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

#ifdef WINDOWS

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <Windows.h>

#include "XVariant.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifdef BUILDER
typedef LONG LSTATUS;
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSREGISTRYKEY
{
  public:
                                XWINDOWSREGISTRYKEY       ();
    virtual                    ~XWINDOWSREGISTRYKEY       ();

    HKEY                        GetHandle                 ();
    void                        SetHandle                 (HKEY hkey);

    XDWORD                      GetType                   ();
    void                        SetType                   (XDWORD hkey);

    bool                        ReadValue                 (XCHAR* name, XVARIANT& data);
    bool                        ReadValue                 (XSTRING& name, XVARIANT& data);

    bool                        WriteValue                (XCHAR* name, XDWORD type, XVARIANT& data);
    bool                        WriteValue                (XCHAR* name, XVARIANT& data);

    bool                        WriteValue                (XSTRING& name, XVARIANT& data);
    bool                        WriteValue                (XSTRING& name, XDWORD type, XVARIANT& data);

    bool                        EnumValues                (XVECTOR<XSTRING*>* listvalues);

    bool                        DeleteValue               (XCHAR* name);
    bool                        DeleteValue               (XSTRING& name);

    void                        Clear                     ();

  private:

    void                        Clean                     ();

    HKEY                        handlekey;
    XDWORD                      type;

};

class XWINDOWSREGISTRYMANAGER
{
  public:
                                XWINDOWSREGISTRYMANAGER   ();
    virtual                    ~XWINDOWSREGISTRYMANAGER   ();

    bool                        CreateKey                 (HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey);
    bool                        CreateKey                 (HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey);

    bool                        OpenKey                   (HKEY handlekey, XCHAR* subkeystring, XWINDOWSREGISTRYKEY& registrykey);
    bool                        OpenKey                   (HKEY handlekey, XSTRING& subkeystring, XWINDOWSREGISTRYKEY& registrykey);

    bool                        EnumKeys                  (XWINDOWSREGISTRYKEY& registrykey, XVECTOR<XSTRING*>* keylist);

    bool                        CloseKey                  (XWINDOWSREGISTRYKEY& registrykey);

    bool                        DeleteKey                 (HKEY handlekey, XCHAR* subkeystring);
    bool                        DeleteKey                 (HKEY handlekey, XSTRING& subkeystring); 

  private:

    void                        Clean                     ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



