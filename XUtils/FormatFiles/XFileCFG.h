/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileCFG.h
* 
* @class      XFILECFG
* @brief      eXtended Utils CFG (Config) file class
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
#include "XPath.h"
#include "XSubject.h"
#include "XVector.h"
#include "XVariant.h"
#include "XFileTXT.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILECFG_VALUETYPE
{
  XFILECFG_VALUETYPE_UNKNOWN          = 0 ,
  XFILECFG_VALUETYPE_INT                  ,
  XFILECFG_VALUETYPE_MASK                 ,
  XFILECFG_VALUETYPE_FLOAT                ,
  XFILECFG_VALUETYPE_STRING               ,
  XFILECFG_VALUETYPE_BOOLEAN
};


enum XFILECFG_MODEREMOTEMIX
{
  XFILECFG_MODEREMOTEMIX_NONE            = 0x00 ,
  XFILECFG_MODEREMOTEMIX_NOTDELADDKEYS   = 0x01 ,
};

#define XFILECFG_EXTENSIONFILE              _L(".ini")

#define XFILECFG_DEFAULTMAXSECUENCEENTRYS    99
#define XFILECFG_INVALIDINDEXSECUENCE        -1  

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATHS;
class XFILEINI;
class XFILEINIREMARK;

class XFILECFGVALUE
{
  public:
                                        XFILECFGVALUE             ();
    virtual                            ~XFILECFGVALUE             ();

    XFILECFG_VALUETYPE                  GetType                   ();
    bool                                SetType                   (XFILECFG_VALUETYPE type);

    XSTRING*                            GetGroup                  ();
    XSTRING*                            GetID                     ();

    void*                               GetValue                  ();
    bool                                SetValue                  (void* value);

    XSTRING*                            GetIDBasic                ();
    XSTRING*                            GetMask                   (); 

    XDWORD                              GetMinSecuences           ();
    void                                SetMinSecuences           (XDWORD minsecuences = 1);

    XDWORD                              GetMaxSecuences           ();
    void                                SetMaxSecuences           (XDWORD maxsecuences = 1);

    int                                 GetIndexSecuence          ();
    void                                SetIndexSecuence          (int index = XFILECFG_INVALIDINDEXSECUENCE);
   
    XDWORD                              GetNSecuences             ();
    void                                SetNSecuences             (XDWORD nsecuences = 0);
    
    XSTRING*                            GetRemarkText             ();
    XDWORD                              GetRemarkXPos             ();
    void                                SetRemarkXPos             (XDWORD remark_xpos);
    
    void*                               GetValuesVector           ();
    void                                SetValuesVector           (void* valuesvector);

    XFILECFG_MODEREMOTEMIX              GetModeRemoteMix          ();
    void                                SetModeRemoteMix          (XFILECFG_MODEREMOTEMIX moderemotemix);

  private:

    void                                Clean                     ();

    XFILECFG_VALUETYPE                  type;
    XSTRING                             group;
    XSTRING                             ID;
    void*                               value;

    XSTRING                             IDbasic;
    XSTRING                             mask;
    int                                 minsecuences;    
    int                                 maxsecuences;    
    int                                 indexsecuence;
    XDWORD                              nsecuences; 
    XSTRING                             remark_text;
    XDWORD                              remark_xpos;
    void*                               valuesvector;            
    XFILECFG_MODEREMOTEMIX              moderemotemix;
};

class XFILECFG : public XSUBJECT
{
  public:                                        
                                        XFILECFG                  (XCHAR* namefile = NULL);
    virtual                            ~XFILECFG                  ();

    virtual bool                        DoVariableMapping         (); 
    virtual bool                        DoDefault                 ();

    template<class T>
    bool                                Ini                       ()
                                        { 
                                          bool status[2];

                                          DoVariableMapping();

                                          DoDefault();

                                          status[0] = Load();  

                                          LoadReadjustment();

                                          status[1] = Save();

                                          return (status[0] && status[1]);  
                                        }    

    virtual bool                        LoadReadjustment          ();        

    bool                                Load                      ();
    virtual bool                        Load                      (XPATH& xpath);
    
    bool                                Save                      (XFILETXTFORMATCHAR formatchar = XFILETXTFORMATCHAR_UNKNOWN);
    virtual bool                        Save                      (XPATH& xpath, XFILETXTFORMATCHAR formatchar = XFILETXTFORMATCHAR_UNKNOWN);

    virtual bool                        End                       ();    

    XPATH*                              GetPathFile               ();
    XFILEINI*                           GetFileINI                ();
    
    XFILECFGVALUE*                      AddValue                  (XFILECFG_VALUETYPE type, XCHAR* group, XCHAR* ID, void* value, XCHAR* remark_text = NULL, XDWORD remark_xpos = 0);
    XVECTOR<XFILECFGVALUE*>*            GetValues                 ();   
    XVARIANT*                           GetValue                  (XCHAR* group, XCHAR* ID);
    XVARIANT*                           GetValue                  (XFILECFGVALUE* cfgvalue);
    XFILECFGVALUE*                      GetCFGValue               (XCHAR* group, XCHAR* ID);
    bool                                SetValue                  (XFILECFGVALUE* CFGvalue, XVARIANT* value);
    bool                                DeleteAllValues           ();
    
    bool                                AddRemark                 (XCHAR* group, XCHAR* text, XDWORD xpos, XDWORD relativeypos);
    bool                                AddRemark                 (XCHAR* group, XCHAR* ID, XCHAR* text, XDWORD xpos, XDWORD relativeypos);
    XVECTOR<XFILEINIREMARK*>*           GetRemarks                ();

    bool                                DelCFGValue               (XCHAR* group, XCHAR* ID);
    bool                                DeleteAllRemarks          ();  
    
    bool                                IniFile                   (XPATH& xpath);
    bool                                EndFile                   ();
    bool                                AjustRemarks              (); 

    int                                 GetCountKeys              (XCHAR* group, XCHAR* IDbasic, XCHAR* mask = NULL, int maxcount = XFILECFG_DEFAULTMAXSECUENCEENTRYS); 

    template<typename T>
    XFILECFGVALUE*                      AddValueSecuence          (XFILECFG_VALUETYPE type, XCHAR* group, XCHAR* IDbasic, XCHAR* mask, int mincount, int maxcount, XVECTOR<T*>& values, int& nkeys, XCHAR* remark_text = NULL, XDWORD remark_xpos = 0)
                                        {
                                          XSTRING         key;
                                          XFILECFGVALUE*  CFGvalue = NULL;
                                                                                 
                                          nkeys = GetCountKeys(group, IDbasic, mask, maxcount);    
                                        
                                          if(nkeys < mincount)  nkeys = mincount;

                                          values.DeleteContents();
                                          values.DeleteAll();

                                          for(int c=1; c<nkeys+1; c++)
                                            {
                                              T* value = GEN_NEW T();
                                              if(!value) 
                                                {
                                                  values.DeleteContents();
                                                  values.DeleteAll();

                                                  return NULL;

                                                } else values.Add(value);
       
                                              GenerateKeySecuence(IDbasic, mask, c, key);
                                              
                                              AddValue(type , group, key.Get(), value, remark_text, remark_xpos);

                                              XFILECFGVALUE* _CFGvalue = GetCFGValue(group, key.Get()); 
                                              if(_CFGvalue) 
                                                {
                                                  _CFGvalue->GetIDBasic()->Set(IDbasic);
                                                  _CFGvalue->GetMask()->Set(mask);
                                                  _CFGvalue->SetMinSecuences(mincount);
                                                  _CFGvalue->SetMaxSecuences(maxcount);
                                                  _CFGvalue->SetIndexSecuence(c);
                                                  _CFGvalue->GetRemarkText()->Set(remark_text);
                                                  _CFGvalue->SetRemarkXPos(remark_xpos);
                                                }  

                                              if(c==1) CFGvalue = _CFGvalue;
                                            }

                                          for(int c=1; c<nkeys+1; c++)
                                            {
                                              XSTRING key;
                                               
                                              GenerateKeySecuence(IDbasic, mask, c, key);

                                              XFILECFGVALUE* _CFGvalue = GetCFGValue(group, key.Get()); 
                                              if(_CFGvalue) 
                                                {
                                                  _CFGvalue->SetValuesVector((void*)&values);  
                                                  _CFGvalue->SetNSecuences(nkeys);                                  
                                                }
                                            }

                                          return CFGvalue;
                                        }     
    
  protected:

    bool                                GenerateKeySecuence       (XCHAR* IDbase, XCHAR* mask, int index, XSTRING& key);

    XPATH                               xpathfile;
    XSTRING                             namefile;
    XFILEINI*                           fileini;

  private:
       
    void                                Clean                     ();
 
    XVECTOR<XFILECFGVALUE*>             values;
    XVECTOR<XFILEINIREMARK*>            remarks;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





