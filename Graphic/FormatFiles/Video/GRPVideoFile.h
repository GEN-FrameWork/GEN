/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVideoFile.h
* 
* @class      GRPVIDEOFILE
* @brief      Graphic Video File class
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
#include "XPath.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPVIDEOFILE_DEFAULTFRAMERATE 25



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class GRPPROPERTY;
class GRPBITMAP;
class GRPBITMAPSEQUENCE;

class GRPVIDEOFILE_PROPERTYS
{
  public:
                                GRPVIDEOFILE_PROPERTYS      ();
    virtual                    ~GRPVIDEOFILE_PROPERTYS      ();

    XDWORD                      width;
    XDWORD                      height;
    XDWORD                      nframes;
    XSTRING                     codecstr;  
    XDWORD                      framerate;  

  private:

    void                        Clean                       ();
};


class GRPVIDEOFILE
{
  public:
                                GRPVIDEOFILE                ();
    virtual                    ~GRPVIDEOFILE                ();

    virtual bool                Open                        (XCHAR* xpath);  
    virtual bool                Create                      (XCHAR* xpath, GRPVIDEOFILE_PROPERTYS& propertys);

    GRPVIDEOFILE_PROPERTYS*     GetPropertys                ();
    
    virtual XBYTE*              GetDataFrame                (XDWORD index_frame, XDWORD& sizeframe);
    virtual bool                AddFrame                    (XBYTE* dataframe, XDWORD dataframesize);
  
    virtual bool                Close                       ();  

  protected:

    GRPVIDEOFILE_PROPERTYS      propertys;

  private:

    void                        Clean                       ();    
};





/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





