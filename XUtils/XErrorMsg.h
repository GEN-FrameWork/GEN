/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XErrorMsg.h
* 
* @class      XERRORMSG
* @brief      eXtended Utils Error Message class
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

#include <stdio.h>
#include <string.h>

#include "XBase.h"
#include "XVector.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XERRORMSG_RETURN
{
  XERRORMSG_RETURN_NONE   = 0x00 ,
  XERRORMSG_RETURN_OK     = 0x01 ,
  XERRORMSG_RETURN_CANCEL = 0x02 ,
  XERRORMSG_RETURN_RETRY  = 0x04 ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XERRORMSG
{
  public:
                                XERRORMSG             ();
                                XERRORMSG             (int code, XCHAR* title, XCHAR* msg);
    virtual                    ~XERRORMSG             ();

    XERRORMSG*                  Get                   ();
    bool                        Set                   (int code, XCHAR* title, XCHAR* msg);
    bool                        Set                   (int code, int level, int image, int sound, XCHAR* title, XCHAR* msg);

    int                         GetCode               ();
    bool                        SetCode               (int code);

    int                         GetLevel              ();
    bool                        SetLevel              (int level);

    int                         GetImage              ();
    bool                        SetImage              (int image);

    int                         GetSound              ();
    bool                        SetSound              (int sound);

    XSTRING*                    GetTitle              ();
    bool                        SetTitle              (XCHAR* title);
    bool                        SetTitle              (XSTRING& title);

    XSTRING*                    GetMsg                ();
    bool                        SetMsg                (XCHAR* msg);
    bool                        SetMsg                (XSTRING& msg);

  private:

    bool                        Clean                 ();

    int                         code;
    int                         level;
    int                         image;
    int                         sound;
    XSTRING                     title;
    XSTRING                     msg;

};

class XERRORSMSG
{
  public:
                                XERRORSMSG            ();
                                XERRORSMSG            (XCHAR* titleApp, XCHAR* version);
    virtual                    ~XERRORSMSG            ();

    XSTRING*                    GetTitleApplication   ();
    XSTRING*                    GetVersion            ();

    bool                        SetTitleApplication   (XCHAR* titleapp);
    bool                        SetTitleApplication   (XSTRING& titleapp);

    bool                        SetVersion            (XCHAR* version);
    bool                        SetVersion            (XSTRING& version);

    XERRORMSG*                  FindMsg               (int code);
    bool                        AddMsg                (int code, int level, int image, int sound, XCHAR* title, XCHAR* msg);
    bool                        EraseMsg              (int code);
    bool                        EraseAllMsg           ();

    virtual XERRORMSG_RETURN    ShowMsg               (int code, bool statusline=false, XBYTE returns=0x01, XCHAR* addstr=NULL, bool sound=false);

  private:

    bool                        Clean                 ();

  protected:

    XSTRING                     titleapp;
    XSTRING                     version;
    XVECTOR<XERRORMSG*>         errors;

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





