/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapFile.h
* 
* @class      GRPBITMAPFILE
* @brief      Graphics Bitmap File class
* @ingroup    GRAPHIC
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
#include "XFile.h"

#include "GRPProperties.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPBITMAPFILE_TYPE
{
  GRPBITMAPFILE_TYPE_UNKNOWN      = 0 ,
  GRPBITMAPFILE_TYPE_JPG              ,
  GRPBITMAPFILE_TYPE_PNG              ,
  GRPBITMAPFILE_TYPE_BMP              ,
  GRPBITMAPFILE_TYPE_TGA              
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;
class GRPBITMAPSEQUENCE;


class GRPBITMAPFILEBASE : public XFILECONTAINER
{
  public:
                                GRPBITMAPFILEBASE           ();
    virtual                    ~GRPBITMAPFILEBASE           ();

    virtual GRPBITMAP*          CreateBitmapFromFile        (XPATH& xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    virtual GRPBITMAP*          CreateBitmapFromBuffer      (XBYTE* buffer, XDWORD size, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);

    virtual bool                CreateFileFromBitmap        (XPATH& xpath, GRPBITMAP* bitmap, int quality = 100);

    virtual XBYTE*              ReadImageBufferFromFile     (XPATH& xpath);
    GRPBITMAP*                  ForceModeOfBitmap           (GRPPROPERTYMODE forcemode, GRPBITMAP* bitmap);

  protected:

    bool                        Open                        (XPATH& xpath, bool readonly=true);
    bool                        Create                      (XPATH& xpath);
    bool                        Close                       (void);

  private:

    void                        Clean                       ();
};

class GRPBITMAPFILE
{
  public:
                                GRPBITMAPFILE           ();
                                GRPBITMAPFILE           (XCHAR* xpath);
                                GRPBITMAPFILE           (XPATH& xpath);
    virtual                    ~GRPBITMAPFILE           ();

    XCHAR*                      GetPath                 ();
    void                        SetPath                 (XPATH& xpath);
    void                        SetPath                 (XCHAR* xpath);


    GRPBITMAPFILE_TYPE          GetTypeFromExtenxion    (XCHAR* xpath);

    GRPBITMAP*                  Load                    (XCHAR* xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    GRPBITMAP*                  Load                    (XPATH& xpath, GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);
    GRPBITMAP*                  Load                    (GRPPROPERTYMODE forcemode = GRPPROPERTYMODE_XX_UNKNOWN);

    bool                        Save                    (XCHAR* xpath, GRPBITMAP* bitmap, int quality = 100);
    bool                        Save                    (XPATH& xpath, GRPBITMAP* bitmap, int quality = 100);
    bool                        Save                    (GRPBITMAP* bitmap, int quality = 100);

    GRPBITMAPSEQUENCE*          LoadSequence            (XCHAR* namemask, int inisequence = 0, int endsequence=-1);

  private:

    void                        Clean                   ();

    XPATH                       xpath;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





