/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileDFU.h
* 
* @class      XFILEDFU
* @brief      eXtended Utils (DFU Device Firmware Update) file class
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

#include "XFile.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XFILEDFU_PREFIX_SIGNATUREPREXIF_MAX   5
#define XFILEDFU_PREFIX_SIGNATUREIMAGE_MAX    6
#define XFILEDFU_IMAGEPREFIX_NAME_MAX         255
#define XFILEDFU_PREFIX_SIGNATURESUFFIX_MAX   3

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEDFU_ELEMENT;

class XFILEDFU_PREFIX
{
  public:
                                    XFILEDFU_PREFIX         ();
    virtual                        ~XFILEDFU_PREFIX         ();

    XBYTE                           signature[XFILEDFU_PREFIX_SIGNATUREPREXIF_MAX];
    XBYTE                           version;
    XDWORD                          imagesize;
    XBYTE                           ntargets;

  private:

    void                            Clean                   ();
};


class XFILEDFU_IMAGE        
{
  public:
                                    XFILEDFU_IMAGE          ();
    virtual                        ~XFILEDFU_IMAGE          ();

    XBYTE                           signature[XFILEDFU_PREFIX_SIGNATUREIMAGE_MAX];
    XBYTE                           alternatesetting;

    XDWORD                          targetnameexist;
    XBYTE                           targetname[XFILEDFU_IMAGEPREFIX_NAME_MAX];
      
    XDWORD                          targetsize;
    XDWORD                          nelements;

    XVECTOR<XFILEDFU_ELEMENT*>      elements;
  
  private:

    void                            Clean                   ();
};

class XFILEDFU_ELEMENT
{
  public:
                                    XFILEDFU_ELEMENT        ();
    virtual                        ~XFILEDFU_ELEMENT        ();

    XDWORD                          address;
    XDWORD                          size;
    
    XBUFFER                         data;
    
  private:

    void                            Clean                   ();
};


class XFILEDFU_SUFFIX
{
  public:
                                    XFILEDFU_SUFFIX         ();
    virtual                        ~XFILEDFU_SUFFIX         ();

    XWORD                           BCDdevice;   

    XWORD                           IDproduct; 
    XWORD                           IDvendor;
  
    XWORD                           BCDDFU;
 
    XBYTE                           signature[XFILEDFU_PREFIX_SIGNATURESUFFIX_MAX];

    XBYTE                           length;
    XDWORD                          CRC;

  private:

    void                            Clean                   ();
};

class XFILEDFU 
{
  public:
                                    XFILEDFU                ();
    virtual                        ~XFILEDFU                ();

    bool                            Open                    (XCHAR* xpath, bool isreadonly = true);

    bool                            ReadAll                 ();

    XFILEDFU_PREFIX*                GetPrefix               ();

    XVECTOR<XFILEDFU_IMAGE*>*       GetImages               ();

    XFILEDFU_SUFFIX*                GetSuffix               ();

    bool                            Close                   ();

  private:

    void                            Clean                   ();

    XFILEDFU_IMAGE*                 Read_Image              ();             
    XFILEDFU_ELEMENT*               Read_Element            ();             
    bool                            CheckCRC                ();
    
    XFILE*                          xfilebase; 

    XFILEDFU_PREFIX                 prefix;
    XFILEDFU_SUFFIX                 suffix;

    XVECTOR<XFILEDFU_IMAGE*>        images;

    static XDWORD                   CRCtable[256];
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




