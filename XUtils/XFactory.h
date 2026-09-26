/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFactory.h
* 
* @class      XFACTORY
* @brief      eXtended Utils factory
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

#include "XThread.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XDATETIME;
class XRAND;
class XFILE;
class XDIR;
#ifdef XCONSOLE_ACTIVE
class XCONSOLE;
#endif

class XFACTORY
{
  public:
                                XFACTORY                    ();
    virtual                    ~XFACTORY                    ();

    static bool                 GetIsInstanced              ();
    static XFACTORY&            GetInstance                 ();
    static bool                 SetInstance                 (XFACTORY* instance);
    static bool                 DelInstance                 ();

    virtual XTIMER*             CreateTimer                 ();
    virtual bool                DeleteTimer                 (XTIMER* timer);

    virtual XDATETIME*          CreateDateTime              ();
    virtual bool                DeleteDateTime              (XDATETIME* datetime);

    virtual XRAND*              CreateRand                  ();
    virtual bool                DeleteRand                  (XRAND* rand);

    virtual XFILE*              Create_File                 ();
    virtual bool                Delete_File                 (XFILE* file);

    virtual XDIR*               Create_Dir                  ();
    virtual bool                Delete_Dir                  (XDIR* dir);

    #ifdef XCONSOLE_ACTIVE
    virtual XCONSOLE*           CreateConsole               ();
    virtual bool                DeleteConsole               (XCONSOLE* xconsole);
    #endif

    virtual XMUTEX*             Create_Mutex                ();
    virtual bool                Delete_Mutex                (XMUTEX* mutex);

    virtual XTHREAD*            CreateThread                (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* param = NULL);
    virtual bool                DeleteThread                (XTHREADGROUPID groupID, XTHREAD* xthread);

  private:

    void                        Clean                       ();

    static XFACTORY*            instance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





