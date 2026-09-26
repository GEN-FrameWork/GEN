/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileXML.h
* 
* @class      XFILEXML
* @brief      eXtended Utils XML (eXtensible Markup Language) file class
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
#include "XBuffer.h"
#include "XFile.h"
#include "XString.h"
#include "XFileTXT.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILEXMLELEMENTTYPE
{
  XFILEXMLELEMENTTYPE_UNKNOWN       = 0 ,
  XFILEXMLELEMENTTYPE_CONFIG            ,
  XFILEXMLELEMENTTYPE_COMMENT           ,
  XFILEXMLELEMENTTYPE_CDATA             ,
  XFILEXMLELEMENTTYPE_NORMAL            ,
};

enum XFILEXMLELEMENTTYPELINE
{
  XFILEXMLELEMENTTYPELINE_UNKNOWN   = 0 ,
  XFILEXMLELEMENTTYPELINE_START         ,
  XFILEXMLELEMENTTYPELINE_END           ,
  XFILEXMLELEMENTTYPELINE_STARTEND      ,
};

#define XFILEXML_SPACETABS      2

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFILEXMLATTRIBUTE
{
  public:
                                    XFILEXMLATTRIBUTE           ();
                                    XFILEXMLATTRIBUTE           (XSTRING& name);
                                    XFILEXMLATTRIBUTE           (XCHAR* name);
                                    XFILEXMLATTRIBUTE           (XSTRING& name, XSTRING& value);
                                    XFILEXMLATTRIBUTE           (XCHAR* name, XCHAR* value);
    virtual                        ~XFILEXMLATTRIBUTE           ();

    XSTRING&                        GetName                     ();
    bool                            SetName                     (XSTRING& name);
    bool                            SetName                     (XCHAR* name);

    XSTRING&                        GetValue                    ();
    bool                            SetValue                    (XSTRING& value);
    bool                            SetValue                    (XCHAR* value);

  private:

    void                            Clean                       ();

    XSTRING                         name;
    XSTRING                         value;
};

class XFILEXMLELEMENT
{
  public:
                                    XFILEXMLELEMENT             ();
                                    XFILEXMLELEMENT             (XSTRING& name, XFILEXMLELEMENTTYPE type = XFILEXMLELEMENTTYPE_NORMAL);
                                    XFILEXMLELEMENT             (XCHAR* name, XFILEXMLELEMENTTYPE type = XFILEXMLELEMENTTYPE_NORMAL);
                                    XFILEXMLELEMENT             (XSTRING& name, XSTRING& value, XFILEXMLELEMENTTYPE type = XFILEXMLELEMENTTYPE_NORMAL);
                                    XFILEXMLELEMENT             (XCHAR* name, XCHAR* value, XFILEXMLELEMENTTYPE type = XFILEXMLELEMENTTYPE_NORMAL);
    virtual                        ~XFILEXMLELEMENT             ();

    XSTRING&                        GetName                     ();
    bool                            SetName                     (XSTRING& name);
    bool                            SetName                     (XCHAR* name);

    XFILEXMLATTRIBUTE*              AddAtribute                 (XSTRING& name, XSTRING& value);
    XFILEXMLATTRIBUTE*              AddAtribute                 (XCHAR* name, XCHAR* value);

    int                             GetNAttributes              ();

    XFILEXMLATTRIBUTE*              GetAttribute                (int index);
    XCHAR*                          GetValueAttribute           (XSTRING& name);
    XCHAR*                          GetValueAttribute           (XCHAR* name);
    bool                            GetValueAttribute           (XSTRING& name, XSTRING& value);
    bool                            GetValueAttribute           (XCHAR* name, XSTRING& value);
    int                             GetIntValueAttribute        (XSTRING& name);
    int                             GetIntValueAttribute        (XCHAR* name);
    bool                            GetBoolValueAttribute       (XCHAR* name);
    double                          GetFloatValueAttribute      (XCHAR* name);

    bool                            DeleteAtribute              (int index);
    bool                            DeleteAllAtributes          ();

    XSTRING&                        GetValue                    ();
    bool                            SetValue                    (XSTRING& value);
    bool                            SetValue                    (XCHAR* value);

    XFILEXMLELEMENT*                AddElement                  ();
    XFILEXMLELEMENT*                AddElement                  (XSTRING& name);
    XFILEXMLELEMENT*                AddElement                  (XCHAR* name);
    XFILEXMLELEMENT*                AddElement                  (XSTRING& name, XSTRING& value);
    XFILEXMLELEMENT*                AddElement                  (XCHAR* name, XCHAR* value);
    int                             GetNElements                ();
    XFILEXMLELEMENT*                GetElement                  (int index);
    bool                            DeleteElement               (int index);
    bool                            DeleteAllElements           ();

    XFILEXMLELEMENT*                GetFather                   ();
    bool                            SetFather                   (XFILEXMLELEMENT* father);

    XFILEXMLELEMENTTYPE             GetType                     ();
    bool                            SetType                     (XFILEXMLELEMENTTYPE type);

  private:

    void                            Clean                       ();

    XFILEXMLELEMENTTYPE             type;
    XFILEXMLELEMENT*                father;
    XSTRING                         name;
    XVECTOR<XFILEXMLATTRIBUTE*>     attributes;
    XSTRING                         value;
    XVECTOR<XFILEXMLELEMENT*>       elements;
};

class XFILEXML : public XFILETXT
{
  public:
                                    XFILEXML                    ();
    virtual                        ~XFILEXML                    ();

    XFILEXMLELEMENT*                GetRoot                     ();
    bool                            SetRoot                     (XFILEXMLELEMENT* element);

    virtual bool                    DecodeAllLines              ();
    virtual bool                    EncodeAllLines              (bool istabulatedline = true);

    virtual bool                    ReadAndDecodeAllLines       ();
    bool                            WriteAndEncodeAllLines      (bool istabulatedline, XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF = XFILETXTTYPELF_0D0A);
    bool                            WriteAndEncodeAllLines      (bool istabulatedline = true);

    XFILEXMLELEMENT*                SearchChild                 (XCHAR* name, XFILEXMLELEMENT* father = NULL);

    XFILEXMLELEMENT*                SearchElement               (XSTRING& name, int& index, XFILEXMLELEMENT* father = NULL);
    XFILEXMLELEMENT*                SearchElement               (XCHAR* name, int& index, XFILEXMLELEMENT* father = NULL);
    XFILEXMLELEMENT*                SearchElement               (XCHAR* name, int& index, XSTRING& namefather);
    XFILEXMLELEMENT*                SearchElement               (XCHAR* name, int& index, XCHAR* namefather);

    bool                            SearchAttributeValue        (XFILEXMLELEMENT* element, XSTRING& nameattribute, XSTRING& value);
    bool                            SearchAttributeValue        (XFILEXMLELEMENT* element, XCHAR* nameattribute, XSTRING& value);

    XVECTOR<XFILEXMLATTRIBUTE*>*    GetCFGAtributes             ();

    bool                            DeleteAllCFGAtributes       ();
    bool                            DeleteAllElements           ();

    void                            SetIgnoreComments           (bool b) {  this->ignorecomments=b; }

protected:

    int                             DecodeLine                  (XSTRING& string, bool iselement);

    bool                            DecodeConfig                (XSTRING& string);
    bool                            DecodeComent                (XSTRING& string);
    bool                            DecodeCDATA                 (XSTRING& string);
    bool                            DecodeElement               (XSTRING& string, XFILEXMLELEMENTTYPELINE typeline);

    bool                            AddTabs                     (int level, int spacetabs, XSTRING& string);

    bool                            EncodeConfig                (XSTRING& string);
    bool                            EncodeElements              (bool istabulatedline, XFILEXMLELEMENT* element);

    XVECTOR<XFILEXMLATTRIBUTE*>     cfgattributes;
    XFILEXMLELEMENT*                root;
    XFILEXMLELEMENT*                actualelement;
    int                             encodelevel;

    bool                            ignorecomments;

    XDWORD                          currentLine;
    bool                            is_multicomment;

  private:

    void                            Clean                       ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





