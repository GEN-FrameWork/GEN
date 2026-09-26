/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFileCobol.h
* 
* @class      XFILECOBOL
* @brief      eXtended Utils Cobol file class
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
#include "XMap.h"
#include "XTree.h"
#include "XString.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XVariant.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XFILECOBOL_LINE_TYPE
{
  XFILECOBOL_LINE_TYPE_UNKNOWN             = 0  ,
  XFILECOBOL_LINE_TYPE_FD                       ,
  XFILECOBOL_LINE_TYPE_SELECT                   ,
  XFILECOBOL_LINE_TYPE_FIELD
};

enum XFILECOBOL_FIELD_TYPE
{
  XFILECOBOL_FIELD_TYPE_UNKNOWN             = 0 ,
  XFILECOBOL_FIELD_TYPE_KEY                     ,
  XFILECOBOL_FIELD_TYPE_VAR                     ,
};


enum XFILECOBOL_PIC_TYPE
{
  XFILECOBOL_PIC_TYPE_UNKNOWN               = 0 ,
  XFILECOBOL_PIC_TYPE_NUMERIC                   ,
  XFILECOBOL_PIC_TYPE_DECIMAL                   ,
  XFILECOBOL_PIC_TYPE_ALPHA                     ,
  XFILECOBOL_PIC_TYPE_BINARY                    ,
};


enum XFILECOBOL_RECORD_TYPE
{
  XFILECOBOL_RECORD_TYPE_UNKNOWN            = 0 ,
  XFILECOBOL_RECORD_TYPE_INDEXED                ,
  XFILECOBOL_RECORD_TYPE_LINE_SEQUENTIAL        ,
  XFILECOBOL_RECORD_TYPE_SEQUENTIAL             ,
  XFILECOBOL_RECORD_TYPE_RELATIVE               ,
};


enum XFILECOBOL_FILE_TYPE
{
  XFILECOBOL_FILE_TYPE_UNKNOWN              = 0 ,
  XFILECOBOL_FILE_TYPE_MF4                      ,
  XFILECOBOL_FILE_TYPE_C_ISAM
};


#define XFILECOBOL_VALIDCHARS                    __L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 +-*/$,;.\"()><:'=")
#define XFILECOBOL_CAPABILITIESFILE              __L("cobcap.dat")

#define XFILECOBOL_MASK_SIGN                     0x40

#define XFILECOBOL_TABSFORSPACES                 4

class XFILECOBOL_FIELD;
typedef XTREE<XFILECOBOL_FIELD*>                 XFILECOBOL_FD_TREE;
typedef XTREE_NODE<XFILECOBOL_FIELD*>            XFILECOBOL_FD_TREENODE;
typedef XTREE_NODE_ITERATOR<XFILECOBOL_FIELD*>   XFILECOBOL_FD_TREENODEITERATOR;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATH;
class XFILE;
class XFILETXT;
class XFILECOBOL_HANDLE;
class XVARIANT;

class XFILECOBOL_PIC
{
  public:
                                    XFILECOBOL_PIC                ();
    virtual                        ~XFILECOBOL_PIC                ();

    XFILECOBOL_PIC_TYPE             GetType                       ();

    XSTRING*                        GetMask                       ();
    XSTRING*                        GetMaskDecondensed            ();
    bool                            ResolveMask                   ();

    XDWORD                          GetSizeData                   ();

    int                             GetComputationalType          ();
    void                            SetComputationalType          (int computationaltype);

    int                             GetNDecimals                  ();
    bool                            HasSign                       ();

  private:

    void                            Clean                         ();

    XFILECOBOL_PIC_TYPE             type;
    XSTRING                         mask;
    XSTRING                         maskdecondensed;
    XDWORD                          sizedata;
    int                             computationaltype;
    XDWORD                          ndecimals;
    bool                            hassign;
};

class XFILECOBOL_FIELD
{
  public:
                                    XFILECOBOL_FIELD              ();
    virtual                        ~XFILECOBOL_FIELD              ();

    XFILECOBOL_FIELD_TYPE           GetType                       ();
    void                            SetType                       (XFILECOBOL_FIELD_TYPE type);

    int                             GetLevel                      ();
    void                            SetLevel                      (int level);

    XSTRING*                        GetName                       ();
    XFILECOBOL_PIC*                 GetPIC                        ();

    int                             GetNOccurs                    ();
    void                            SetNOccurs                    (int noccurs);

    bool                            IsRedefine                    ();
    void                            SetIsRedefine                 (bool isredefine);

    XSTRING*                        GetRedefineName               ();

    int                             GetRedefineOffset             ();
    void                            SetRedefineOffset             (int redefineoffset);

  private:

    void                            Clean                         ();

    XFILECOBOL_FIELD_TYPE           type;
    int                             level;
    XSTRING                         name;
    XFILECOBOL_PIC                  PIC;
    int                             noccurs;

    bool                            isredefine;
    XSTRING                         redefinename;
    int                             redefineoffset;
};

class XFILECOBOL_RECORD
{
  public:
                                    XFILECOBOL_RECORD             (XFILECOBOL_HANDLE* handle);
    virtual                        ~XFILECOBOL_RECORD             ();

    XFILECOBOL_RECORD_TYPE          GetType                       ();
    bool                            SetType                       (XFILECOBOL_RECORD_TYPE type);

    XDWORD                          GetNFields                    ();

    XFILECOBOL_FIELD*               GetField                      (int index);
    XFILECOBOL_FIELD*               GetField                      (XCHAR* name);
    XFILECOBOL_FIELD*               GetField                      (XFILECOBOL_FD_TREENODEITERATOR& it);

    bool                            GetFieldData                  (int index, XVARIANT& variant);
    bool                            GetFieldData                  (XCHAR* name, XVARIANT& variant);
    bool                            GetFieldData                  (XFILECOBOL_FD_TREENODEITERATOR& it, XVARIANT& variant);

    bool                            GetFieldData                  (int index, XSTRING& datastring);
    bool                            GetFieldData                  (XCHAR* name, XSTRING& datastring);
    bool                            GetFieldData                  (XFILECOBOL_FD_TREENODEITERATOR& it, XSTRING& datastring);

    bool                            GetFieldData                  (int index, XBUFFER& data);
    bool                            GetFieldData                  (XCHAR* name, XBUFFER& data);
    bool                            GetFieldData                  (XFILECOBOL_FD_TREENODEITERATOR& it, XBUFFER& data);

    XBUFFER*                        GetData                       ();

  private:

    bool                            ConvertDataTo                 (XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XVARIANT& datavariant);
    bool                            ConvertDataTo                 (XFILECOBOL_PIC* picture, XBUFFER& bufferdata, XSTRING& datastring);

    void                            Clean                         ();

    XFILECOBOL_HANDLE*              handle;
    XBUFFER                         data;
};

class XFILECOBOL_HANDLE
{
    public:
                                    XFILECOBOL_HANDLE             ();
    virtual                        ~XFILECOBOL_HANDLE             ();

    XSTRING*                        GetName                       ();
    XSTRING*                        GetValueLabel                 ();
    XSTRING*                        GetDataRecord                 ();
    XFILECOBOL_FD_TREE*             GetFDTree                     ();

    int                             GetIndexNodeForName           (XFILECOBOL_FD_TREENODE* node, XCHAR* name);

    XFILECOBOL_RECORD_TYPE          GetTypeRecord                 ();
    void                            SetTypeRecord                 (XFILECOBOL_RECORD_TYPE type);

    bool                            GetInterator                  (XDWORD index, XFILECOBOL_FD_TREENODEITERATOR& it);
    bool                            GetInterator                  (XCHAR* name, XFILECOBOL_FD_TREENODEITERATOR& it);

    XDWORD                          CalculateDataSizeNode         (XFILECOBOL_FD_TREENODE* node);
    XDWORD                          CalculateDataSize             ();

    int                             CalculatePositionData         (XFILECOBOL_FD_TREENODEITERATOR& it);

    XFILECOBOL_FIELD*               IsFieldRedefinedByAnother     (XCHAR* name);

  private:

    void                            Clean                         ();

    XSTRING                         name;
    XSTRING                         valuelabel;
    XSTRING                         datarecord;
    XFILECOBOL_FD_TREE              fdtree;
    XFILECOBOL_RECORD_TYPE          typerecord;
};

class XFILECOBOL
{
  public:
                                    XFILECOBOL                    ();
    virtual                        ~XFILECOBOL                    ();

    bool                            Open                          (int nfiles, XCHAR* xpathcpyfiles, XCHAR* xpathdatfiles, ...);
    bool                            IsOpen                        ();

    XFILECOBOL_FILE_TYPE            GetType                       ();
    void                            SetType                       (XFILECOBOL_FILE_TYPE type);

    bool                            HaveFileHeaders               ();
    void                            SetHaveFileHeaders            (bool haveheaders);

    XDWORD                          GetNRecords                   ();

    bool                            ReadRecord                    (XDWORD index, XFILECOBOL_RECORD& record);
    bool                            ReadRecords                   (XVECTOR<XFILECOBOL_RECORD*>& records, XDWORD startindex);

    bool                            Close                         (void);

    XFILECOBOL_HANDLE*              GetHandle                     ();
    XPATH*                          GetPathDataFile               ();

    bool                            CapabilitiesFile_Load         (XPATH& xpathcapabilitiesfile);
    bool                            CapabilitiesFile_UnLoad       ();
    bool                            CapabilitiesFile_GetFile      (XSTRING* fileID, XPATH& xpathfile);

  private:

    bool                            Eliminate_TabsForSpaces       (XVECTOR<XSTRING*>* lines);
    bool                            IsValidCharacter              (XCHAR character);
    bool                            Eliminate_InvalidCharacters   (XSTRING& line);
    bool                            Eliminate_InvalidCharacters   (XVECTOR<XSTRING*>* lines);
    bool                            Eliminate_Comments            (XVECTOR<XSTRING*>* lines);
    bool                            Eliminate_NumberLines         (XVECTOR<XSTRING*>* lines);
    bool                            Eliminate_UnnecessarySpaces   (XVECTOR<XSTRING*>* lines);
    bool                            Eliminate_EmptyLines          (XVECTOR<XSTRING*>* lines);

    bool                            CreateUniqueLines             (XVECTOR<XSTRING*>* lines);

    bool                            ExtractReservedToken          (XSTRING& line, XCHAR* token, XSTRING& var);
    bool                            ExtractReservedToken          (XSTRING& line, XCHAR* token);

    XFILECOBOL_LINE_TYPE            InterpreterLineForHandle      (XSTRING& line, int& level, XFILECOBOL_FIELD_TYPE& fieldtype, XFILECOBOL_HANDLE& handle, XSTRING& name, XSTRING& param, XSTRING& param2, int& noccurs, int& PICcompresslevel);
    XFILECOBOL_FD_TREENODE*         GetParentNodeOfLevel          (int level);
    bool                            TraslateTextToHandle          (XVECTOR<XSTRING*>* lines, XFILECOBOL_HANDLE& handle);

    bool                            ChangeNameNodeWithOccurs      (int index, XSTRING* name);

    XFILECOBOL_FD_TREENODE*         DuplicateNodeWithSubnodes     (int index, XFILECOBOL_FD_TREENODE* node, XFILECOBOL_FD_TREENODE* cretednode = NULL);
    bool                            AjustNodeWithOccurs           (XFILECOBOL_FD_TREENODE* node);

    void                            Clean                         ();

    XFILECOBOL_FILE_TYPE            type;

    bool                            havefileheaders;
    XDWORD                          fileheadersize;
    XDWORD                          maxfilerecordsize;
    XDWORD                          minfilerecordsize;
    XDWORD                          fileheaderrecordize;
    XDWORD                          filerecordpadding;

    XSTRING                         validchars;
    XPATH                           xpathcpyfiles;
    XPATH                           xpathdatfiles;

    XFILECOBOL_HANDLE               handle;
    XPATH                           xpathdatafile;

    XMAP<XSTRING*, XPATH*>          capabilitiesfilemap;

    XFILE*                          xfiledata;
    XDWORD                          sizedatarecord;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





