/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XConsole.h
* 
* @class      XCONSOLE
* @brief      eXtended Utils Console base class
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


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XCONSOLE_SYMBOLSUSED
{  
  XCONSOLE_SYMBOLSUSED_NOTCONSOLE        = 0 ,  
  XCONSOLE_SYMBOLSUSED_UNKNWON               , 
  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED          , 
  
  XCONSOLE_SYMBOLSUSED_CODEPAGE_437          ,
  XCONSOLE_SYMBOLSUSED_CODEPAGE_850          ,     
  XCONSOLE_SYMBOLSUSED_CODEPAGE_852          ,     
  XCONSOLE_SYMBOLSUSED_CODEPAGE_866          ,     

  XCONSOLE_SYMBOLSUSED_ISO_8859_1            ,
  XCONSOLE_SYMBOLSUSED_ISO_8859_2            ,
  XCONSOLE_SYMBOLSUSED_ISO_8859_3            ,
  XCONSOLE_SYMBOLSUSED_ISO_8859_4            ,

  XCONSOLE_SYMBOLSUSED_WINDOWS_1250          ,
  XCONSOLE_SYMBOLSUSED_WINDOWS_1251          ,
  XCONSOLE_SYMBOLSUSED_WINDOWS_1252          ,  
  XCONSOLE_SYMBOLSUSED_WINDOWS_1253          ,
  XCONSOLE_SYMBOLSUSED_WINDOWS_1254          ,  

  XCONSOLE_SYMBOLSUSED_UNICODE_UTF8          ,  
  XCONSOLE_SYMBOLSUSED_UNICODE_UTF16         ,  

};

#define XCONSOLE_MAXSIZEDATABLOCK             10240

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;

class XCONSOLE 
{
  public:

                                    XCONSOLE                      ();
    virtual                        ~XCONSOLE                      ();

    virtual bool                    GetSize                       (int& width, int& height);
    virtual bool                    SetSize                       (int width, int height);

    virtual bool                    GetSizeText                   (int& columms, int& rows);

    virtual XCONSOLE_SYMBOLSUSED    GetSymbolsUsed                ();

    virtual bool                    Maximize                      ();
    virtual bool                    Minimize                      ();

    virtual bool                    Hide                          ();
    virtual bool                    IsHide                        ();
    virtual bool                    UnHide                        ();


    virtual bool                    Print                         (XCHAR* string);
    bool                            Printf                        (XCHAR* mask, ...);

    bool                            PrintDataBlock                (XBYTE* data, XDWORD _size, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);
    bool                            PrintDataBlock                (XBUFFER& data, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);

    virtual bool                    Clear                         (bool fill = true);

    virtual bool                    KBHit                         ();
    virtual int                     GetChar                       ();

    bool                            Format_Message                (XCHAR* message, XDWORD margin, bool prelude, bool returnline, XSTRING& string);
    bool                            PrintMessage                  (XCHAR* message, XDWORD margin, bool prelude, bool returnline);  
    bool                            WaitKey                       (XCHAR* text, XDWORD margin, bool prelude, XDWORD timeout);
    void                            EraseToEndLine                (int actualpos = 0);    

  private:

    void                            Clean                         ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

XCONSOLE_SYMBOLSUSED Console_GetSymbolsUsed();



