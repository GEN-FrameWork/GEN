/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XESP32Factory.h
* 
* @class      XESP32FACTORY
* @brief      eXtended utils ESP32 platform factory class
* @ingroup    PLATFORM_ESP32
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

#include "XFactory.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XESP32FACTORY : public XFACTORY
{
  public:

    virtual XTIMER*     				CreateTimer             ();
    virtual bool                DeleteTimer             (XTIMER* timer);

    virtual XDATETIME*          CreateDateTime          ();
    virtual bool                DeleteDateTime          (XDATETIME* time);

    virtual XRAND*              CreateRand              ();
    virtual bool                DeleteRand              (XRAND* rand);
    
    XFILE*                      Create_File             ();
    bool                        Delete_File             (XFILE* file);
       
    XDIR*                       Create_Dir              ();
    bool                        Delete_Dir              (XDIR* dir);

    virtual XSYSTEM*            CreateSystem            ();
    virtual bool                DeleteSystem            (XSYSTEM* system);

    virtual XMUTEX*             Create_Mutex            ();
    virtual bool                Delete_Mutex            (XMUTEX* phone);

    virtual XTHREAD*            CreateThread            (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* data = NULL);
    virtual bool                DeleteThread            (XTHREADGROUPID groupID, XTHREAD* xthread);    
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




