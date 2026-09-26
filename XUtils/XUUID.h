/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XUUID.h
* 
* @class      XUUID
* @brief      eXtended Utils UUID (Universally Unique IDentifier)
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
#include "XBuffer.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XUUIDMAXDATA4   6

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XRAND;

class XUUID
{
  public:
                        XUUID             ();
    virtual            ~XUUID             ();

    XDWORD              GetData1          ();
    XWORD               GetData2          ();
    XWORD               GetData3          ();
    XBYTE               GetData4          ();
    XBYTE               GetData5          ();
    XBYTE*              GetData6          ();

    bool                GetToBuffer       (XBUFFER& data);  
    bool                GetToString       (XSTRING& string);

    bool                SetData1          (XDWORD data);
    bool                SetData2          (XWORD data);
    bool                SetData3          (XWORD data);
    bool                SetData4          (XBYTE data);
    bool                SetData5          (XBYTE data);
    bool                SetData6          (XBYTE* data);

    bool                SetFromBuffer     (XBUFFER& data);
    bool                SetFromString     (XSTRING& string);

    bool                IsEmpty           ();
    bool                Empty             ();
    
    bool                Set               (XUUID& ID);
    bool                Set               (XDWORD data1, XWORD data2, XWORD data3, XBYTE data4, XBYTE data5, XBYTE* data6);

    bool                CopyFrom          (XUUID& uuid);
    bool                CopyTo            (XUUID& uuid);
    
    bool                GenerateRandom    ();

    bool                Compare           (XUUID& uuid);  
    
  protected:

    XDWORD              data1;
    XWORD               data2;
    XWORD               data3;
    XBYTE               data4;
    XBYTE               data5;
    XBYTE               data6[XUUIDMAXDATA4];

  private:

    void                Clean       ();

    XRAND*              xrand;    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





