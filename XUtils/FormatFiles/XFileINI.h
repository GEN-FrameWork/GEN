/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileINI.h
* 
* @class      XFILEINI
* @brief      eXtended Utils INI (Windows INItialization file) file class
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

#include "XBase.h"
#include "XVector.h"
#include "XFileTXT.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEINI_TYPEREMARK
{
  XFILEINI_TYPEREMARK_UNKNOWN       ,
  XFILEINI_TYPEREMARK_ALL_LINE      ,
  XFILEINI_TYPEREMARK_IN_SECTION    ,
  XFILEINI_TYPEREMARK_IN_KEY   
};

typedef struct
{
  XSTRING key;
  XSTRING value;

} XFILEINIKEYMAP;

#define XFILEININKEYMAP(map)    (sizeof(map)/sizeof(XFILEINIKEYMAP))



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEINIKEY
{
  public:
                                    XFILEINIKEY                 ();
                                   ~XFILEINIKEY                 ();

    XSTRING*                        GetKey                      ();
    bool                            SetKey                      (XSTRING& key);

    XSTRING*                        GetValue                    ();
    bool                            SetValue                    (XSTRING& key);

  private:

    void                            Clean                       ();

    XSTRING                         key;
    XSTRING                         value;
};

class XFILEINISECTION
{
  public:
                                    XFILEINISECTION             ();
                                   ~XFILEINISECTION             ();

    XSTRING*                        GetName                     ();
    bool                            SetName                     (XSTRING& section);

    XFILEINIKEY*                    FindKey                     (XSTRING& key);

    bool                            GetKey                      (XSTRING& key, XSTRING& value);
    bool                            SetKey                      (XSTRING& key, XSTRING& value);

    bool                            DeleteKey                   (XSTRING& key);

    bool                            GetNextKey                  (XSTRING& key, XSTRING& value);
    bool                            ResetSelectionKey           ();    

    void                            Clean                       ();

  private:    

    XSTRING                         section;
    XVECTOR<XFILEINIKEY*>           keys;
    XDWORD                          selectkey;
};

class XFILEINIREMARK
{
  public:
                                    XFILEINIREMARK              ();
                                   ~XFILEINIREMARK              ();

   XFILEINI_TYPEREMARK              GetType                     ();
   void                             SetType                     (XFILEINI_TYPEREMARK type);

   XDWORD                           GetXPos                     ();
   void                             SetXPos                     (XDWORD xpos);

   int                              GetRelativeYPos             ();                               
   void                             SetRelativeYPos             (int relativeypos);                                       

   XSTRING*                         GetRelativeSection          ();   
   XSTRING*                         GetRelativeKey              (); 
   
   XSTRING*                         GetTextRemark               (); 

   bool                             Compare                     (XFILEINIREMARK* remake); 

   bool                             CopyTo                      (XFILEINIREMARK* remake); 
   bool                             CopyFrom                    (XFILEINIREMARK* remake); 

  private:    

    void                            Clean                       ();

    XFILEINI_TYPEREMARK             type;

    XDWORD                          xpos;
    int                             relativeypos;

    XSTRING                         relativesection;
    XSTRING                         relativekey;

    XSTRING                         textremark;    
};

class XFILEINI : public XFILETXT
{
  public:
                                    XFILEINI                    ();
                                    XFILEINI                    (XPATH& xpath);
                                    XFILEINI                    (XFILE* file);
    virtual                        ~XFILEINI                    ();

    bool                            Open                        (XPATH& xpath, bool readonly=true);
    bool                            Close                       (void);

    XFILEINISECTION*                GetSection                  (XSTRING& section);
    XFILEINISECTION*                GetSection                  (XCHAR* section);

    bool                            SelectSection               (XSTRING& section);
    bool                            SelectSection               (XCHAR* section);

    bool                            CreateSection               (XSTRING& section);
    bool                            CreateSection               (XCHAR* section);

    bool                            DeleteSection               (XSTRING& section);
    bool                            DeleteSection               (XCHAR* section);

    XFILEINIKEY*                    GetKey                      (XSTRING& section, XSTRING& key);

    bool                            DeleteKey                   (XSTRING& section, XSTRING& key);
    bool                            DeleteKey                   (XCHAR* section, XSTRING& key);
    bool                            DeleteKey                   (XSTRING& section, XCHAR* key);
    bool                            DeleteKey                   (XCHAR* section, XCHAR* key);

    bool                            ReadValue                   (XSTRING& section, XSTRING& key, XSTRING& value);
    bool                            ReadValue                   (XCHAR* section, XSTRING& key, XSTRING& value);
    bool                            ReadValue                   (XSTRING& section, XCHAR* key, XSTRING& value);
    bool                            ReadValue                   (XCHAR* section, XCHAR* key, XSTRING& value);
    bool                            ReadValue                   (XSTRING& key, XSTRING& value);
    bool                            ReadValue                   (XCHAR* key, XSTRING& value);

    bool                            WriteValue                  (XSTRING& section, XSTRING& key, XSTRING& value);
    bool                            WriteValue                  (XCHAR* section, XSTRING& key, XSTRING& value);
    bool                            WriteValue                  (XSTRING& section, XCHAR* key, XSTRING& value);
    bool                            WriteValue                  (XCHAR* section, XCHAR* key, XSTRING& value);
    bool                            WriteValue                  (XSTRING& key, XSTRING& value);
    bool                            WriteValue                  (XCHAR* key, XSTRING& value);

    bool                            GetNextKey                  (XSTRING& key, XSTRING& value);

    bool                            ReadKeyMap                  (XSTRING& section, XFILEINIKEYMAP* keymap, int nkeymap);
    bool                            ReadKeyMap                  (XCHAR* section, XFILEINIKEYMAP* keymap, int nkeymap);

    bool                            ConvertFromLines            ();
    bool                            ConvertToLines              ();

    bool                            Serialization               (bool read);

    bool                            DeleteAllSections           ();

    XVECTOR<XFILEINIREMARK*>*       GetRemarks                  ();
    bool                            DeleteAllRemarks            ();


  private:

    bool                            Line_IsSection              (XSTRING& line, XSTRING& section);
    bool                            Line_IsKey                  (XSTRING& line, XSTRING& key, XSTRING& value);
    bool                            Line_GetRemark              (XSTRING& line, XFILEINIREMARK& remark);

    void                            Clean                       ();

    XVECTOR<XFILEINISECTION*>       sections;
    XDWORD                          selectsection;
    XVECTOR<XFILEINIREMARK*>        remarks;
    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





