/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XPath.h
* 
* @class      XPATH
* @brief      eXtended Utils Path class
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
#include "XVector.h"

#include "XPathsManager.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GEN_API_LIB_EXP XPATH : public XSTRING
{
  public:
                              XPATH                         ();
                              XPATH                         (XDWORD size);
                              XPATH                         (const char* path);
                              XPATH                         (const XCHAR* path);
                              XPATH                         (const XCHAR* path, XDWORD size);
                              XPATH                         (XPATH& path);
                              XPATH                         (XSTRING& path);
                              XPATH                         (XWORD* path);

    virtual                  ~XPATH                         ();

    void                      operator =                    (const char* path);
    void                      operator =                    (const XCHAR* path);
    void                      operator =                    (XPATH& path);
    void                      operator =                    (XSTRING& string);    
    void                      operator =                    (XWORD* path);

    void                      operator +=                   (const char* path);
    void                      operator +=                   (const XCHAR* path);
    void                      operator +=                   (XPATH& path);
    void                      operator +=                   (XSTRING& string);  
    void                      operator +=                   (XCHAR character);

    bool                      operator <                    (XPATH& path);
    bool                      operator >                    (XPATH& path);
    bool                      operator <=                   (XPATH& path);
    bool                      operator >=                   (XPATH& path);
    bool                      operator ==                   (XPATH& path);
    bool                      operator !=                   (XPATH& path);

    bool                      operator <                    (XSTRING& string);
    bool                      operator >                    (XSTRING& string);
    bool                      operator <=                   (XSTRING& string);
    bool                      operator >=                   (XSTRING& string);
    bool                      operator ==                   (XSTRING& string);
    bool                      operator !=                   (XSTRING& string);

    XCHAR                     operator []                   (int position);

    bool                      GetDrive                      (XSTRING& drive);
    bool                      GetPath                       (XSTRING& path);
    bool                      GetDriveAndPath               (XSTRING& drivepath);
    bool                      GetPathAndNamefile            (XSTRING& pathnamefile);
    bool                      GetPathAndNamefileExt         (XSTRING& pathnamefile);
    bool                      GetNamefile                   (XSTRING& namefile);
    bool                      GetNamefileExt                (XSTRING& namefileext);
    bool                      GetExt                        (XSTRING& ext);

    bool                      GetPathInSequence             (int index, XSTRING& pathpart);

    const XCHAR*              SetOnlyDrive                  ();
    const XCHAR*              SetOnlyPath                   ();
    const XCHAR*              SetOnlyDriveAndPath           ();
    const XCHAR*              SetOnlyPathAndNamefile        ();
    const XCHAR*              SetOnlyPathAndNamefileExt     ();
    const XCHAR*              SetOnlyNamefile               ();
    const XCHAR*              SetOnlyNamefileExt            ();
    const XCHAR*              SetOnlyExt                    ();

    bool                      DeleteDrive                   ();
    bool                      DeleteExt                     ();

    bool                      Slash_HaveAtLast              ();
    bool                      Slash_Add                     ();
    bool                      Slash_Normalize               (bool inverse = false);
    bool                      Slash_Delete                  ();

    bool                      AddToNameFile                 (bool isprefix, XCHAR* tag);

    bool                      Create                        (int nelements, ...);
    bool                      Create                        (XPATHSMANAGERSECTIONTYPE sectiontype, int nelements, ...);

    bool                      Split                         (XSTRING* drive, XPATH* xpath, XSTRING* name, XSTRING* ext);
    bool                      Split                         (XSTRING* drive, XVECTOR<XSTRING*>& subpaths, XSTRING* name, XSTRING* ext);

  private:

    void                      Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





