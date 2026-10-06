/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebPageHTMLCreator.h
* 
* @class      DIOWEBPAGEHTMLCREATOR
* @brief      Data Input/Output Web Page HTML Create class
* @ingroup    DATAIO
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
#include "XPath.h"
#include "XEvent.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWEBPAGEHTMLCREATORALIGN
{
  DIOWEBPAGEHTMLCREATORALIGN_NONE                       =  0 ,
  DIOWEBPAGEHTMLCREATORALIGN_LEFT                            ,
  DIOWEBPAGEHTMLCREATORALIGN_RIGHT                           ,
  DIOWEBPAGEHTMLCREATORALIGN_CENTER                          ,
};

enum DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS
{
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND        =  0 ,
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND          ,
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND
};

#define DIOWEBPAGEHTMLCREATOR_TAGXENGINE   _L("GEN")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWEBPAGEHTMLCREATOR : public XSTRING, public XSUBJECT
{
  public:
                                            DIOWEBPAGEHTMLCREATOR         ();
    virtual                                ~DIOWEBPAGEHTMLCREATOR         ();

    void                                    operator =                    (const char* htlmpage);
    void                                    operator =                    (const XCHAR* htlmpage);
    void                                    operator =                    (DIOWEBPAGEHTMLCREATOR& htlmpage);
    void                                    operator =                    (XSTRING& string);
    void                                    operator =                    (XWORD* htlmpage);

    void                                    operator +=                   (const char* htlmpage);
    void                                    operator +=                   (const XCHAR* htlmpage);
    void                                    operator +=                   (XPATH& htlmpage);
    void                                    operator +=                   (XSTRING& string);
    void                                    operator +=                   (XCHAR htlmpage);

    bool                                    operator <                    (DIOWEBPAGEHTMLCREATOR& htlmpage);
    bool                                    operator >                    (DIOWEBPAGEHTMLCREATOR& htlmpage);
    bool                                    operator <=                   (DIOWEBPAGEHTMLCREATOR& htlmpage);
    bool                                    operator >=                   (DIOWEBPAGEHTMLCREATOR& htlmpage);
    bool                                    operator ==                   (DIOWEBPAGEHTMLCREATOR& htlmpage);
    bool                                    operator !=                   (DIOWEBPAGEHTMLCREATOR& htlmpage);

    bool                                    operator <                    (XSTRING& string);
    bool                                    operator >                    (XSTRING& string);
    bool                                    operator <=                   (XSTRING& string);
    bool                                    operator >=                   (XSTRING& string);
    bool                                    operator ==                   (XSTRING& string);
    bool                                    operator !=                   (XSTRING& string);

    XCHAR                                   operator []                   (int position);

    bool                                    Printf                        (XCHAR* mask, ...);
    bool                                    Print                         (XCHAR* text);

    bool                                    Table_Ini                     (int border = false, XCHAR* addstype = NULL);
    bool                                    Table_Line                    (int ncolumns, ...);
    bool                                    Table_Line                    (XCHAR* colorbackground, int ncolumns, ...);
    bool                                    Table_LineColor               (int ncolumns, ...);
    bool                                    Table_End                     ();

    bool                                    AddAutoRefresh                (int refresh, XSTRING* url = NULL);

    bool                                    AddFile                       (XPATH& xpath);

  private:

    void                                    Clean                         ()
                                            {
                                              tableborder = false;
                                            }

    DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS   CheckXTAG                     (XSTRING* originalline, bool& intag, XSTRING& inioutputline, XSTRING& endoutputline);

    XSTRING                                 lastXTAG;
    int                                     tableborder;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





