/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XVariant.h
* 
* @class      XVARIANT
* @brief      eXtended Utils Variant class
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
#include "XString.h"
#include "XDateTime.h"
#include "XTrace.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XVARIANT_TYPE
{
  XVARIANT_TYPE_NULL                        = 0 ,
  XVARIANT_TYPE_BOOLEAN                         ,  
  XVARIANT_TYPE_SHORT                           ,
  XVARIANT_TYPE_WORD                            ,
  XVARIANT_TYPE_INTEGER                         , 
  XVARIANT_TYPE_DWORD                           ,
  XVARIANT_TYPE_DOUBLEINTEGER                   , 
  XVARIANT_TYPE_QWORD                           ,
  XVARIANT_TYPE_FLOAT                           ,
  XVARIANT_TYPE_DOUBLE                          ,
  XVARIANT_TYPE_CHAR                            ,
  XVARIANT_TYPE_XCHAR                           ,
  XVARIANT_TYPE_STRING                          ,
  XVARIANT_TYPE_DATE                            ,
  XVARIANT_TYPE_TIME                            ,
  XVARIANT_TYPE_DATETIME                        ,  
  XVARIANT_TYPE_BUFFER                          ,  
  XVARIANT_TYPE_POINTER                         ,

  XVARIANT_TYPE_UNKNOWN                               
};

#define XVARIANT_CREATE(vtype, ctype, value)      if(data) Destroy(); \
                                                  type  = vtype; \
                                                  size  = sizeof(ctype); \
                                                  data = (void*)GEN_NEW ctype; \
                                                  if(data) *((ctype*)data) = value;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XVARIANT
{
  public:
                                    XVARIANT                ();
                                    XVARIANT                (const bool value);
                                    XVARIANT                (const short value); 
                                    XVARIANT                (const XWORD value);
                                    XVARIANT                (const int value);
                                    XVARIANT                (const XDWORD value);
                                    XVARIANT                (const long long value);
                                    XVARIANT                (const XQWORD value);
                                    XVARIANT                (const float value);
                                    XVARIANT                (const double value);
                                    XVARIANT                (const char value);                                      
                                    XVARIANT                (const char* value);                                    
                                    XVARIANT                (const XCHAR value);
                                    XVARIANT                (const XCHAR* value);
                                    XVARIANT                (const XSTRING& value);  
                                    XVARIANT                (const XDATETIME& value);
                                    XVARIANT                (const XBUFFER& value);                                                                 
                                    XVARIANT                (const void* value);

                                    XVARIANT                (const XVARIANT& value);           

    virtual                        ~XVARIANT                ();
        
    const XVARIANT&                 operator =              (const bool value);
    const XVARIANT&                 operator =              (const short value);
    const XVARIANT&                 operator =              (const XWORD value);
    const XVARIANT&                 operator =              (const int value);
    const XVARIANT&                 operator =              (const XDWORD value);
    const XVARIANT&                 operator =              (const long long value);
    const XVARIANT&                 operator =              (const XQWORD value);
    const XVARIANT&                 operator =              (const float value);
    const XVARIANT&                 operator =              (const double value);    
    const XVARIANT&                 operator =              (const char value);   
    const XVARIANT&                 operator =              (const char* value);
    const XVARIANT&                 operator =              (const XCHAR value);
    const XVARIANT&                 operator =              (const XCHAR* value);
    const XVARIANT&                 operator =              (const XSTRING& value);     
    const XVARIANT&                 operator =              (const XDATETIME& value);
    const XVARIANT&                 operator =              (const XBUFFER& value);    
    const XVARIANT&                 operator =              (const void* value);

    const XVARIANT&                 operator =              (const XVARIANT& value);  
   
    operator                        bool                    () const;
    operator                        short                   () const;
    operator                        XWORD                   () const;
    operator                        int                     () const;
    operator                        XDWORD                  () const;
    operator                        long long               () const;
    operator                        XQWORD                  () const;
    operator                        float                   () const;
    operator                        double                  () const;    
    operator                        char                    () const;
    operator                        char*                   () const;
    operator                        XCHAR                   () const;
    operator                        XCHAR*                  () const;
    operator                        XSTRING                 () const;
    operator                        XDATETIME               () const;
    operator                        XBUFFER                 () const;
    operator                        void*                   () const;
     
    XVARIANT_TYPE                   GetType                 ();
    bool                            GetType                 (XSTRING& typestr);

    void                            SetType                 (XVARIANT_TYPE type);
    virtual XDWORD                  GetSize                 ();
    virtual void*                   GetData                 ();
 
    bool                            GetDataFromString       (char* string);
    bool                            GetDataFromString       (XCHAR* string);
    bool                            GetDataVariant          (XVARIANT& value);

    virtual bool                    Set                     (XVARIANT_TYPE type = XVARIANT_TYPE_NULL, void* data = NULL, XDWORD size = 0);

    virtual bool                    ToString                (XSTRING& to);
    bool                            FromString              (XSTRING& from, XVARIANT_TYPE from_type = XVARIANT_TYPE_UNKNOWN);

    bool                            IsNull                  ();  

    virtual bool                    Destroy                 ();

    #ifdef XTRACE_ACTIVE
    void                            PrintDebug              ();    
    #endif

  protected:
  
    XVARIANT_TYPE                   type;
    XDWORD                          size;
    void*                           data;

  private:

    void                            Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





