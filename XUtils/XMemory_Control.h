/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XMemory_Control.h
* 
* @class      XMEMORY_CONTROL
* @brief      eXtended Utils Memory Control class
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


#ifdef XMEMORY_CONTROL_ACTIVE


#ifdef _MSC_VER
  #pragma warning (disable:4291 4786)
#endif


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <new>

#ifdef WINDOWS
#include <windows.h>
#include <process.h>
#endif


#ifdef LINUX
#include <pthread.h>
#endif

#include "XBase.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifdef MICROCONTROLLER
  #define XMEMORY_CONTROL_MAXBLOCKASSIGN        128
#else
  #define XMEMORY_CONTROL_MAXBLOCKASSIGN        16384
#endif

#define XMEMORY_CONTROL_MAXNAMEMODULESIZE       32
#define XMEMORY_CONTROL_SHOWDATABLOCKSIZE       512
#define XMEMORY_CONTROL_MAXIMUNLEAKSTODISPLAY   100

#define XMEMORY_CONTROL_DISPLAYMEMORYLEAKS      XMemory_Control.DisplayAll(true);
#define XMEMORY_CONTROL_GETMEMORYUSED           XMemory_Control.GetMemoryUsed()
#define XMEMORY_CONTROL_GETMEMORYNBLOCKS        XMemory_Control.GetMemoryNBlocks()

#define XMEMORY_CONTROL_ACTIVATED               XMemory_Control.Activate(true);
#define XMEMORY_CONTROL_DEACTIVATED             XMemory_Control.Activate(false);


#define XMEMORY_CONTROL_PRE_ACTIVATED           

#define XMEMORY_CONTROL_PRE_DEACTIVATED

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class XMEMORY_CONTROL_ASSIGN
{
  public:

    void*                       ptr;
    XDWORD                      size;
    char                        namemodule[XMEMORY_CONTROL_MAXNAMEMODULESIZE];
    int                         linemodule;
};

class XMEMORY_CONTROL
{
  public:
                                XMEMORY_CONTROL             ();
    virtual                    ~XMEMORY_CONTROL             ();

    bool                        IsActive                    ();
    bool                        Activate                    (bool isactive);

    void*                       Assign                      (XDWORD size, const char* namefile, int line);
    void                        Free                        (void* ptr);

    bool                        FreeAll                     ();

    XDWORD                      GetUsed                     ();
    XDWORD                      GetNAssigns                 ();

    bool                        DisplayAll                  (bool displaydata = false);

  private:

    bool                        Lock                        ();
    bool                        UnLock                      ();

    XDWORD                      CRC32ForByte                (XDWORD ini);
    XDWORD                      CRC32                       (XBYTE* data, XWORD size);

    bool                        RegisterAssign              (void* ptr, XDWORD size, const char* pathfile, int line);
    bool                        DeRegisterAssign            (void* ptr, XDWORD& size);

    bool                        ResizeAssignList            ();

    bool                        SearchAssignIndex           (bool free, void* ptr, XDWORD& index);

    void                        Clean                       ();


    bool                        isactive;

    XMEMORY_CONTROL_ASSIGN*     assignlist;

    XDWORD                      nregisterelements;
    XDWORD                      nblockregisterelements;

    XDWORD                      nassigns;
    XDWORD                      maxnassigns;

    XDWORD                      used;
    XDWORD                      maxused;

    XQWORD                      mutexhandle;

    #ifdef LINUX
    pthread_mutex_t             mutex;
    pthread_mutexattr_t         attr;
    #endif
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

  extern XMEMORY_CONTROL XMemory_Control;

  void* operator new      (size_t size, char const* namefile, int line);
  void* operator new[]    (size_t size, char const* namefile, int line);
  void  operator delete   (void* ptr) throw ();
  void  operator delete[] (void* ptr) throw ();
  
  void* ReAlloc           (void* assign, size_t size);

  #define GEN_NEW                                 new(GEN_MODULE_EXEC, GEN_LINE_EXEC)
  #define GEN_DELETE                              delete
  #define GEN_DELETE_ARRAY                        delete [] 

  #define GEN_MALLOC(size)                        GEN_NEW XBYTE[size]
  #define GEN_CALLOC(nelements, elementsize)      GEN_NEW XBYTE[nelements * elementsize]
  #define GEN_REALLOC(assign, size)               ReAlloc(assign, size)                      
  #define GEN_FREE(assign)                        GEN_DELETE_ARRAY assign;

#else

  #define GEN_NEW                                 new
  #define GEN_DELETE                              delete
  #define GEN_DELETE_ARRAY                        delete []  

  #define GEN_MALLOC(size)                        malloc(size)
  #define GEN_CALLOC(nelements, elementsize)      calloc(nelements, elementsize)
  #define GEN_REALLOC(assign, size)               realloc(assign, size)
  #define GEN_FREE(assign)                        free(assign);

  #define XMEMORY_CONTROL_DISPLAYMEMORYLEAKS
  #define XMEMORY_CONTROL_GETMEMORYUSED           0
  #define XMEMORY_CONTROL_GETMEMORYNBLOCKS        0
  #define XMEMORY_CONTROL_ACTIVATED               
  #define XMEMORY_CONTROL_DEACTIVATED

#endif





