/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileJSON.h
* 
* @class      XFILEJSON
* @brief      eXtended Utils JSON (JavaScript Object Notation) file class
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
#include "XString.h"
#include "XFileTXT.h"
#include "XVariant.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XFILEJSONVALUETYPE
{  
  XFILEJSONVALUETYPE_NULL               = 0 ,
  XFILEJSONVALUETYPE_BOOLEAN                ,
  XFILEJSONVALUETYPE_SHORT                  ,
  XFILEJSONVALUETYPE_WORD                   ,
  XFILEJSONVALUETYPE_INTEGER                ,
  XFILEJSONVALUETYPE_DWORD                  ,
  XFILEJSONVALUETYPE_DOUBLEINTEGER          ,
  XFILEJSONVALUETYPE_QWORD                  ,
  XFILEJSONVALUETYPE_FLOAT                  ,
  XFILEJSONVALUETYPE_DOUBLEFLOAT            ,
  XFILEJSONVALUETYPE_STRING                 ,
  XFILEJSONVALUETYPE_OBJECT                 ,
  XFILEJSONVALUETYPE_ARRAY                  ,  
};


enum XFILEJSONCONTROLCHAR
{
  XFILEJSONCONTROLCHAR_NOTCONTROL           , // All other Not control or number
  XFILEJSONCONTROLCHAR_OPENBRACE            , // {
  XFILEJSONCONTROLCHAR_CLOSEBRACE           , // }
  XFILEJSONCONTROLCHAR_OPENBRACKET          , // [
  XFILEJSONCONTROLCHAR_CLOSEBRACKET         , // ]
  XFILEJSONCONTROLCHAR_QUOTE                , // "
  XFILEJSONCONTROLCHAR_COMMA                , // ,
  XFILEJSONCONTROLCHAR_COLON                , // :
  XFILEJSONCONTROLCHAR_BLACKSLASH           , // (the other slash /)
  XFILEJSONCONTROLCHAR_NUMBER               , // '012345789' '-'
  XFILEJSONCONTROLCHAR_NUMBERSPECIAL        , //  '+' 'E' 'e' '.'
  XFILEJSONCONTROLCHAR_TEXT                   //  (All Chars except control,Space,TAB,Return o Line Feed).
};


#define XFILEJSON_SPACETABS                             2

#define XFILEJSON_ADDVALUE(node, name, value)           { XFILEJSONVALUE* jsonvalue = GEN_NEW XFILEJSONVALUE();             \
                                                          if(jsonvalue)                                                 \
                                                            {                                                           \
                                                              jsonvalue->SetName(name);                                 \
                                                              jsonvalue->Set(value);                                    \
                                                              node->Add(jsonvalue);                                     \
                                                            }                                                           \
                                                        }

#define XFILEJSON_ADDVALUE_NULL(node, name)             { XFILEJSONVALUE* jsonvalue = GEN_NEW XFILEJSONVALUE();             \
                                                          if(jsonvalue)                                                 \
                                                            {                                                           \
                                                              jsonvalue->SetName(name);                                 \
                                                              jsonvalue->Set();                                         \
                                                              node->Add(jsonvalue);                                     \
                                                            }                                                           \
                                                        }

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT;
class XFILEJSONOBJECT;
class XFILEJSONARRAY;

class XFILEJSONVALUE
{
  public:
                                XFILEJSONVALUE                ();
    virtual                    ~XFILEJSONVALUE                ();

    XFILEJSONVALUETYPE          GetType                       ();
    void                        SetType                       (XFILEJSONVALUETYPE type);
    
    XSTRING*                    GetName                       ();
    bool                        SetName                       (XSTRING& name);
    bool                        SetName                       (XCHAR* name);

    XVARIANT*                   GetValue                      ();
    bool                        GetValueBoolean               ();
    short                       GetValueShort                 ();
    XWORD                       GetValueWord                  ();
    int                         GetValueInteger               ();
    XDWORD                      GetValueDWord                 ();
    long long                   GetValueDoubleInteger         ();
    XQWORD                      GetValueQWord                 ();
    float                       GetValueFloating              ();
    double                      GetValueDoubleFloat           ();    
    XSTRING                     GetValueString                ();
    XFILEJSONOBJECT*            GetValueObject                ();
    XFILEJSONARRAY*             GetValueArray                 ();
    void*                       GetValuePointer               ();
    
    bool                        Set                           (void);
    bool                        Set                           (bool boolean); 
    bool                        Set                           (short number);
    bool                        Set                           (XWORD number);
    bool                        Set                           (int number);
    bool                        Set                           (XDWORD number);
    bool                        Set                           (long long number);
    bool                        Set                           (XQWORD number);
    bool                        Set                           (float number);
    bool                        Set                           (double number);    
    bool                        Set                           (XCHAR* string);
    bool                        Set                           (XSTRING& string);
    bool                        Set                           (XFILEJSONOBJECT* object);
    bool                        Set                           (XFILEJSONARRAY* array);
    bool                        Set                           (XVARIANT* variant);

    bool                        Delete                        ();

    XFILEJSONVALUE*             Clone                         ();                       

  private:

    void                        Clean                         ();

    XFILEJSONVALUETYPE          type;
    XSTRING*                    name;
    XVARIANT                    value;
};

class XFILEJSONOBJECT
{
  public:
                                XFILEJSONOBJECT               ();
    virtual                    ~XFILEJSONOBJECT               ();

    bool                        IsArray                       ();
    void                        SetIsArray                    (bool isarray);

    bool                        Add                           (XFILEJSONVALUE* value);
    bool                        Add                           (XCHAR* name, XFILEJSONARRAY* array);
    bool                        Add                           (XSTRING& name, XFILEJSONARRAY* array);
    bool                        Add                           (XCHAR* name, XFILEJSONOBJECT* object);
    bool                        Add                           (XSTRING& name, XFILEJSONOBJECT* object);

    XFILEJSONOBJECT*            GetFather                     ();  
    void                        SetFather                     (XFILEJSONOBJECT* father);  

    XVECTOR<XFILEJSONVALUE*>*   GetValues                     ();
    bool                        DeleteAllValues               ();

    XFILEJSONOBJECT*            Clone                         ();                       

  protected:

    XFILEJSONOBJECT*            father;  
    bool                        isarray;
    XVECTOR<XFILEJSONVALUE*>    values;

  private:

    void                        Clean                         ();
};

class XFILEJSONARRAY  : public XFILEJSONOBJECT
{
  public:
                                XFILEJSONARRAY                ();
    virtual                    ~XFILEJSONARRAY                ();

    XSTRING*                    GetName                       ();
    bool                        SetName                       (XSTRING& name);
    bool                        SetName                       (XCHAR* name);

    XFILEJSONARRAY*             Clone                         (); 

  private:

    void                        Clean                         ();

    XSTRING                     name;
};

class XFILEJSON : public XFILETXT
{
  public:
                                XFILEJSON                     ();
    virtual                    ~XFILEJSON                     ();

    XFILEJSONOBJECT*            GetRoot                       ();
    bool                        SetRoot                       (XFILEJSONOBJECT* root);

    bool                        DecodeAllLines                ();
    bool                        EncodeAllLines                (bool istabulatedline = true);

    bool                        ReadAndDecodeAllLines         ();
    bool                        WriteAndEncodeAllLines        (bool istabulatedline, XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF = XFILETXTTYPELF_0D0A);
    bool                        WriteAndEncodeAllLines        (bool istabulatedline = true);

    bool                        DeleteAllObjects              ();

    XFILEJSONOBJECT*            GetObj                        (XCHAR* name, XFILEJSONOBJECT* startobject = NULL);
    XFILEJSONOBJECT*            GetObj                        (XSTRING& name, XFILEJSONOBJECT* startobject = NULL);

    XFILEJSONVALUE*             GetValue                      (XCHAR* name, XFILEJSONOBJECT* startobject = NULL);
    XFILEJSONVALUE*             GetValue                      (XSTRING& name, XFILEJSONOBJECT* startobject = NULL);
    XFILEJSONVALUE*             GetValue                      (int index, XFILEJSONOBJECT* startobject = NULL);

    bool                        ShowTraceJSON                 (XBYTE color, bool istabulatedline = true);

  private:

    XFILEJSONOBJECT*            GetObjSubValue                (XCHAR* name, XFILEJSONVALUE* value);

    bool                        DecodeObject                  (int& position, bool isobject, XFILEJSONOBJECT* object);

    XFILEJSONCONTROLCHAR        CheckControlCharacter         (XCHAR character);
    XCHAR                       GetControlCharacter           (XFILEJSONCONTROLCHAR ctrlchar);
    bool                        AddControlCharacter           (XFILEJSONCONTROLCHAR ctrlchar, XSTRING& line);

    XFILEJSONCONTROLCHAR        SearchNextControlCharacter    (int& position);
    bool                        SearchControlCharacter        (XFILEJSONCONTROLCHAR ctrlchar, int& position);

    long long                   ConvertStringToLongLong       (XSTRING& string);

    bool                        GetNumber                     (int& position, XSTRING& string, bool& special);
    bool                        GetText                       (int& position, XSTRING& string);
    bool                        GetString                     (int& position, XSTRING& string);

    bool                        AddTabs                       (int level, int spacetabs, XSTRING& string);

    bool                        EncodeObject                  (bool isobject, XFILEJSONOBJECT* object, bool istabulatedline = true);

    void                        Clean                         ();

    XFILEJSONOBJECT*            root;
    XSTRING                     all;
    int                         encodelevel;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





