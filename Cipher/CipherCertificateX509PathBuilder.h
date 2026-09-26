/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCertificateX509PathBuilder.h
*
* @class      CIPHERCERTIFICATEX509PATHBUILDER
* @brief      X.509 certification path discovery, independent from validation policy
* @ingroup    CIPHER
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

#include "XVector.h"
#include "CipherCertificateX509.h"

#define CIPHERCERTIFICATEX509PATHBUILDER_MAX_PATHS  32
#define CIPHERCERTIFICATEX509PATHBUILDER_MAX_SEARCH_NODES  4096

class CIPHERCERTIFICATEX509PATHBUILDER
{
  public:
                                            CIPHERCERTIFICATEX509PATHBUILDER ();
    virtual                                ~CIPHERCERTIFICATEX509PATHBUILDER ();

    bool                                    Build                              (XBUFFER& leaf, XVECTOR<XBUFFER*>* intermediates, XVECTOR<XBUFFER*>* trustedroots, XVECTOR<XBUFFER*>& path, XDWORD maximumdepth = 10);
    bool                                    BuildAll                           (XBUFFER& leaf, XVECTOR<XBUFFER*>* intermediates, XVECTOR<XBUFFER*>* trustedroots, XVECTOR<XVECTOR<XBUFFER*>*>& paths, XDWORD maximumdepth = 10, XDWORD maximumpaths = CIPHERCERTIFICATEX509PATHBUILDER_MAX_PATHS);
    static void                             Path_Delete                        (XVECTOR<XBUFFER*>& path);
    static void                             Paths_Delete                       (XVECTOR<XVECTOR<XBUFFER*>*>& paths);

  private:
    bool                                    SearchAll                          (CIPHERCERTIFICATEX509* current, XVECTOR<CIPHERCERTIFICATEX509*>& candidates, XVECTOR<CIPHERCERTIFICATEX509*>& roots, XVECTOR<XDWORD>& selected, XVECTOR<XVECTOR<XDWORD>*>& results, XDWORD maximumdepth, XDWORD maximumpaths, XDWORD& searchednodes);
    bool                                    IsTrusted                          (CIPHERCERTIFICATEX509* certificate, XVECTOR<CIPHERCERTIFICATEX509*>& roots);
};
