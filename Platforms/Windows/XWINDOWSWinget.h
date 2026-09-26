/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XWINDOWSWinget.h
*
* @class      XWINDOWSWINGET
* @brief      WINDOWS eXtended Utils WinGet API class
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
#include "XSerializable.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XWINDOWSWINGET_APPLICATIONOPERATION
{
  XWINDOWSWINGET_APPLICATIONOPERATION_UNKNOWN         =   0 ,
  XWINDOWSWINGET_APPLICATIONOPERATION_INSTALL               ,
  XWINDOWSWINGET_APPLICATIONOPERATION_UPDATEVERSION         ,
  XWINDOWSWINGET_APPLICATIONOPERATION_UNINSTALL             ,  
};





/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSWINGET_ELEMENTRESULT : public XSERIALIZABLE
{
  public:
                                                XWINDOWSWINGET_ELEMENTRESULT  ();
    virtual                                    ~XWINDOWSWINGET_ELEMENTRESULT  ();

    bool                                        Serialize                     ();    
    bool                                        Deserialize                   ();  

    XSTRING                                     name;
    XSTRING                                     ID;
    XSTRING                                     actualversion;
    XSTRING                                     availableversion;

  private:

    void                                        Clean                         ();
};

class XWINDOWSWINGET_LISTRESULT : public XSERIALIZABLE
{
  public:
                                                XWINDOWSWINGET_LISTRESULT     ();
    virtual                                    ~XWINDOWSWINGET_LISTRESULT     ();

    bool                                        Serialize                     ();    
    bool                                        Deserialize                   ();  

    XVECTOR<XWINDOWSWINGET_ELEMENTRESULT*>      list;

  private:

    void                                        Clean                         ();
};

class XWINDOWSWINGET
{
   public:

    static bool                                 GetIsInstanced                ();
    static XWINDOWSWINGET&                      GetInstance                   ();
    static bool                                 DelInstance                   ();  

    bool                                        InstallModule                 ();

    bool                                        List                          (XWINDOWSWINGET_LISTRESULT* listresult);
    bool                                        ListUpdateAvailable           (XWINDOWSWINGET_LISTRESULT* listresult);
    bool                                        Find                          (XCHAR* search, XWINDOWSWINGET_LISTRESULT* listresult);  

    bool                                        List                          (bool updateavaible, XSTRING& jsonresult);  
    bool                                        Find                          (XCHAR* search, XSTRING& jsonresult);

    bool                                        ApplicationOperation          (XWINDOWSWINGET_APPLICATIONOPERATION appoper, XCHAR* ID, bool force = true);
           
  private:                                             
                                                XWINDOWSWINGET                ();
                                                XWINDOWSWINGET                (XWINDOWSWINGET const&);
    virtual                                    ~XWINDOWSWINGET                ();

    void                                        operator =                    (XWINDOWSWINGET const&);
    
    bool                                        Exec                          (XCHAR* params, XBUFFER& output);

    int                                         LooksLikeUTF16                (XBYTE* buffer, int size);
    bool                                        LooksLikeUTF8                 (XBYTE* buffer, int size);
    void                                        ConvertUTF16ToUTF8            (XBYTE* data);
    void                                        CP437ToASCII                  (XBYTE* dst, const XBYTE* src, int max);
    void                                        NormalizeUnicode              (XBYTE* data);
    
    bool                                        GenerateColumnList            (XCHAR* ask, XVECTOR<XSTRING*>* list);
    bool                                        GenerateList                  (XSTRING& string, XVECTOR<XSTRING*>* list);
    
    void                                        Clean                         ();

    static XWINDOWSWINGET*                      instance;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



