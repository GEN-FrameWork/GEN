/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSAccessControlLists.h
* 
* @class      XWINDOWSACCESSCONTROLLISTS
* @brief      WINDOWS eXtended Utils Access Control Lists class
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include "XString.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

#define XWINDOWSACL_DELETE                           (0x00010000L)
#define XWINDOWSACL_READ_CONTROL                     (0x00020000L)
#define XWINDOWSACL_WRITE_DAC                        (0x00040000L)
#define XWINDOWSACL_WRITE_OWNER                      (0x00080000L)
#define XWINDOWSACL_SYNCHRONIZE                      (0x00100000L)

#define XWINDOWSACL_STANDARD_RIGHTS_REQUIRED         (0x000F0000L)

#define XWINDOWSACL_STANDARD_RIGHTS_READ             (READ_CONTROL)
#define XWINDOWSACL_STANDARD_RIGHTS_WRITE            (READ_CONTROL)
#define XWINDOWSACL_STANDARD_RIGHTS_EXECUTE          (READ_CONTROL)

#define XWINDOWSACL_STANDARD_RIGHTS_ALL              (0x001F0000L)

#define XWINDOWSACL_SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


class XWINDOWSACCESSCONTROLLISTS
{
  public:
                            XWINDOWSACCESSCONTROLLISTS            ();
    virtual                ~XWINDOWSACCESSCONTROLLISTS            ();

    bool                    SetFileOnlyPermissionForAdmin         (XCHAR* filename);
    bool                    SetFileOnlyPermissionForSystemUser    (XCHAR* filename);
    bool                    SetFileOnlyPermissionForAdminUser     (XCHAR* filename);
    bool                    SetFilePermissionForEveryone          (XCHAR* filename);

  private:

    void                    Clean                                 ();
};




