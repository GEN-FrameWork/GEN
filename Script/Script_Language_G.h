/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Language_G.h
* 
* @class      SCRIPT_LANGUAGE_G
* @brief      Script Language G interpreter (mini .c interpreter)
* @ingroup    SCRIPT
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
#include "XSubject.h"
#include "XString.h"
#include "XPath.h"
#include "XVariant.h"
#include "XEvent.h"

#include "Script.h"
#include "Script_ErrorCode.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SCRIPT_LNG_G_VERSION                    1
#define SCRIPT_LNG_G_SUBVERSION                 1
#define SCRIPT_LNG_G_SUBVERSIONERR              0

enum SCRIPT_LNG_G_TOKENTYPES
{
  SCRIPT_LNG_G_TOKENTYPES_UNDEFTT      ,
  SCRIPT_LNG_G_TOKENTYPES_DELIMITER    ,
  SCRIPT_LNG_G_TOKENTYPES_IDENTIFIER   ,
  SCRIPT_LNG_G_TOKENTYPES_NUMBER       ,
  SCRIPT_LNG_G_TOKENTYPES_KEYWORD      ,
  SCRIPT_LNG_G_TOKENTYPES_TEMP         ,
  SCRIPT_LNG_G_TOKENTYPES_STRING       ,
  SCRIPT_LNG_G_TOKENTYPES_BLOCK
};


enum SCRIPT_LNG_G_DOUBLEOPERATOR
{
  SCRIPT_LNG_G_DOUBLEOPERATOR_LT  = 1  ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_LE       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_GT       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_GE       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_EQ       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_NE       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_LS       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_RS       ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_INC      ,
  SCRIPT_LNG_G_DOUBLEOPERATOR_DEC
};


enum SCRIPT_LNG_G_TOKENIREPS
{
  SCRIPT_LNG_G_TOKENIREPS_UNDEFTOK     ,
  SCRIPT_LNG_G_TOKENIREPS_CHAR         ,
  SCRIPT_LNG_G_TOKENIREPS_INT          ,
  SCRIPT_LNG_G_TOKENIREPS_STRING       ,
  SCRIPT_LNG_G_TOKENIREPS_SWITCH       ,
  SCRIPT_LNG_G_TOKENIREPS_CASE         ,
  SCRIPT_LNG_G_TOKENIREPS_IF           ,
  SCRIPT_LNG_G_TOKENIREPS_ELSE         ,
  SCRIPT_LNG_G_TOKENIREPS_FOR          ,
  SCRIPT_LNG_G_TOKENIREPS_DO           ,
  SCRIPT_LNG_G_TOKENIREPS_WHILE        ,
  SCRIPT_LNG_G_TOKENIREPS_BREAK        ,
  SCRIPT_LNG_G_TOKENIREPS_RETURN       ,
  SCRIPT_LNG_G_TOKENIREPS_END          ,
  SCRIPT_LNG_G_TOKENIREPS_FLOAT
};

enum SCRIPT_LNG_G_ERRORCODE
{
  SCRIPT_LNG_G_ERRORCODE_SYNTAX         = SCRIPT_ERRORCODE_OWN  ,   // "Syntax error"
  SCRIPT_LNG_G_ERRORCODE_NO_EXP                                 ,   // "No expression present"
  SCRIPT_LNG_G_ERRORCODE_NOT_VAR                                ,   // "Not a variable"
  SCRIPT_LNG_G_ERRORCODE_DUP_VAR                                ,   // "Duplicate variable name"
  SCRIPT_LNG_G_ERRORCODE_DUP_FUNC                               ,   // "Duplicate function name"
  SCRIPT_LNG_G_ERRORCODE_SEMI_EXPECTED                          ,   // "Semicolon expected"
  SCRIPT_LNG_G_ERRORCODE_UNBAL_BRACES                           ,   // "Unbalanced braces"
  SCRIPT_LNG_G_ERRORCODE_FUNC_UNDEF                             ,   // "Function undefined"
  SCRIPT_LNG_G_ERRORCODE_TYPE_EXPECTED                          ,   // "Type specifier expected"
  SCRIPT_LNG_G_ERRORCODE_RET_NOCALL                             ,   // "Return without call"
  SCRIPT_LNG_G_ERRORCODE_PAREN_EXPECTED                         ,   // "Parentheses expected"
  SCRIPT_LNG_G_ERRORCODE_WHILE_EXPECTED                         ,   // "While expected"
  SCRIPT_LNG_G_ERRORCODE_QUOTE_EXPECTED                         ,   // "Closing quote expected"
  SCRIPT_LNG_G_ERRORCODE_DIV_BY_ZERO                            ,   // "Division by zero"
  SCRIPT_LNG_G_ERRORCODE_BRACE_EXPECTED                         ,   // "{ expected (control statements must use blocks)"
  SCRIPT_LNG_G_ERRORCODE_COLON_EXPECTED                         ,   // "Colon expected"
  SCRIPT_LNG_G_ERRORCODE_USERBREAK                              ,   // "Break by user"
  SCRIPT_LNG_G_ERRORCODE_TOO_MANY_PARAMS                        ,   // "Too many parameters"
  SCRIPT_LNG_G_ERRORCODE_TOKEN_TOO_LONG                         ,   // "Token too long"
  SCRIPT_LNG_G_ERRORCODE_TYPE_MISMATCH                               // "Type mismatch"
};

union SCRIPT_LNG_G_VARVALUE
{
  int          integer;
  double       real;
  XCHAR        character;
  XDWORD       uinteger;
  XSTRING*     string;
};

#define SCRIPT_LNG_G_MAXIDLEN                   31      // max ID     length
#define SCRIPT_LNG_G_MAXTOKENLEN                128     // max token  length
#define SCRIPT_LNG_G_NUMPARAMS                  41      // max params lenght

#define SCRIPT_LNG_G_EXTENSION                  __L(".g")



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREADCOLLECTED;
class SCRIPT_LIB;

class SCRIPT_LNG_G_COMMAND
{
  public:
                                        SCRIPT_LNG_G_COMMAND        ();
                                       ~SCRIPT_LNG_G_COMMAND        ();

    XSTRING*                            GetCommand                  ();
    SCRIPT_LNG_G_TOKENIREPS             GetToken                    ();

    bool                                Set                         (XCHAR* command, SCRIPT_LNG_G_TOKENIREPS token);
    bool                                Set                         (XSTRING& command, SCRIPT_LNG_G_TOKENIREPS token);

  private:

    void                                Clean                       ();

    XSTRING                             command;
    SCRIPT_LNG_G_TOKENIREPS             token;
};


class SCRIPT_LNG_G_FUNCTIONTYPE
{
  public:
                                        SCRIPT_LNG_G_FUNCTIONTYPE   ();
    virtual                            ~SCRIPT_LNG_G_FUNCTIONTYPE   ();

    XSTRING*                            GetName                     ();
    SCRIPT_LNG_G_TOKENIREPS             GetReturnType               ();
    XCHAR*                              GetLocation                 ();

    bool                                SetName                     (XCHAR* name);
    bool                                SetName                     (XSTRING& name);
    bool                                SetReturnType               (SCRIPT_LNG_G_TOKENIREPS returntype);
    bool                                SetLocation                 (XCHAR* location);

  private:

    void                                Clean                       ();

    XSTRING                             name;
    SCRIPT_LNG_G_TOKENIREPS             returntype;
    XCHAR*                              location;
};

class SCRIPT_LNG_G_VAR
{
  public:

                                        SCRIPT_LNG_G_VAR            ();
    virtual                            ~SCRIPT_LNG_G_VAR            ();

    XSTRING*                            GetName                     ();
    SCRIPT_LNG_G_TOKENIREPS             GetType                     ();

    int                                 GetValueInteger             ();
    float                               GetValueFloat               ();
    double                              GetValueDouble              ();
    XCHAR                               GetValueCharacter           ();
    XDWORD                              GetValueUInteger            ();
    XSTRING*                            GetValueString              ();

    bool                                HaveReservedSize            ();
    bool                                IsArg                       ();
    bool                                IsReturnValue               ();
    bool                                IsNumeric                   ();
    bool                                IsTrue                      ();

    bool                                Set                         (SCRIPT_LNG_G_VAR* var);

    bool                                SetName                     (XCHAR* name);
    bool                                SetType                     (SCRIPT_LNG_G_TOKENIREPS type);

    bool                                SetValueInteger             (int value);
    bool                                SetValueFloat               (double value);
    bool                                SetValueCharacter           (XCHAR value);
    bool                                SetValueUInteger            (XDWORD value);
    bool                                SetValueString              (XSTRING* value);
    bool                                SetValueString              ();

    bool                                SetHaveReservedSize         (bool havereservedsize);
    bool                                SetIsArg                    (bool isarg);
    bool                                SetIsReturnValue            (bool isreturnvalue);

    bool                                ConvertToXVariant           (XVARIANT& variant);
    bool                                ConvertFromXVariant         (XVARIANT& variant);

    bool                                Clear                       ();

  private:

    void                                Clean                       ();

    XSTRING                             name;
    SCRIPT_LNG_G_TOKENIREPS             type;
    SCRIPT_LNG_G_VARVALUE               value;    
    bool                                havereservedsize;
    bool                                isarg;
    bool                                isreturnvalue;
};

class SCRIPT_LNG_G : public SCRIPT
{
  public:
                                        SCRIPT_LNG_G                ();
    virtual                            ~SCRIPT_LNG_G                ();

   int                                  Run                         (int* returnval = NULL);

    bool                                AddCommand                  (XCHAR* command, SCRIPT_LNG_G_TOKENIREPS token);
    bool                                DeleteCommands              ();

    int                                 GetFuncParams               (SCRIPT_LNG_G_VAR* params);
    void                                NotFuncParams               ();

    int                                 GetReturnValueScript        ();
    int                                 SetReturnValueScript        (int returnvaluescript);

    bool                                HaveError                   (int errorcode);

  private:

    bool                                DeleteVarsExec              ();

    bool                                IsSpace                     (XCHAR character);
    bool                                IsDigit                     (XCHAR character);
    bool                                IsAlpha                     (XCHAR character);

    bool                                IsDelimiter                 (XCHAR c);
    SCRIPT_LNG_G_TOKENIREPS             LookUpToken                 (XCHAR* s);
    void                                PutBackToken                ();

    void                                EvalExp                     (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp0                    (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp1                    (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp2                    (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp3                    (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp4                    (SCRIPT_LNG_G_VAR& value);
    void                                EvalExp5                    (SCRIPT_LNG_G_VAR& value);
    void                                Atom                        (SCRIPT_LNG_G_VAR& value);

    SCRIPT_LNG_G_VAR*                   IsVariable                  (XCHAR* variblename);
    void                                AssignVariable              (XCHAR* variablename, SCRIPT_LNG_G_VAR& value);
    SCRIPT_LNG_G_VAR*                   FindVariable                (XCHAR* variablename);
    SCRIPT_LNG_G_TOKENIREPS             FindVariableType            (XCHAR* variablename);

    XCHAR*                              FindFunction                (XCHAR* name);
    SCRIPT_LNG_G_FUNCTIONTYPE*          GetFunction                 (XCHAR* name);
    void                                FindEndofBlock              ();
    void                                SkipStatement               ();

    void                                DeclareGlobalVariable       ();
    void                                DeclareLocalVariable        ();

    SCRIPT_LNG_G_TOKENTYPES             GetToken                    ();

    void                                PreScan                     ();

    void                                GetArgs                     ();
    void                                GetParams                   ();

    void                                FunctionReturn              ();

    void                                Exec_IF                     ();
    void                                Exec_SWITCH                 ();
    void                                Exec_WHILE                  ();
    void                                Exec_DO                     ();
    void                                Exec_FOR                    ();

    bool                                Interpret                   ();

    void                                Call                        ();

    void                                Clean                       ();

    int                                 returnvaluescript;

    XCHAR*                              ipprg;

    XVECTOR<SCRIPT_LNG_G_COMMAND*>      commands;

    XVECTOR<SCRIPT_LIB*>                librarys;

    XVECTOR<SCRIPT_LIB_FUNCTION*>       libraryfunctions;

    XVECTOR<SCRIPT_LNG_G_VAR*>          globalvars;
    XVECTOR<SCRIPT_LNG_G_FUNCTIONTYPE*> functiontable;
    XVECTOR<SCRIPT_LNG_G_VAR*>          localvarsstack;
    XVECTOR<int>                        functioncallstack;
    XVECTOR<int>                        nestscopestack;

    SCRIPT_LNG_G_TOKENTYPES             tokentype;
    SCRIPT_LNG_G_TOKENIREPS             tokenireps;

    bool                                breakfound;
    bool                                returnfound;

    XCHAR                               currenttoken[SCRIPT_LNG_G_MAXTOKENLEN+1];
    XCHAR                               currentfunction[_MAXSTR];
    SCRIPT_LNG_G_VAR                    returnvalue;
};





/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



