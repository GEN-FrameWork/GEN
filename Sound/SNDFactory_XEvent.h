/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFactory_XEvent.h
* 
* @class      SNDFACTORY_XEVENT
* @brief      Sound Factory eXtended event class
* @ingroup    SOUND
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

#include "XEvent.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum SNDFACTORY_XEVENT_TYPE
{
  SNDFACTORY_XEVENT_TYPE_UNKNOWN                = XEVENT_TYPE_SOUND ,
  SNDFACTORY_XEVENT_TYPE_SOUND_INI                                  ,  
  SNDFACTORY_XEVENT_TYPE_SOUND_PLAY                                 ,
  SNDFACTORY_XEVENT_TYPE_SOUND_PAUSE                                ,
  SNDFACTORY_XEVENT_TYPE_SOUND_STOP                                 ,  
  SNDFACTORY_XEVENT_TYPE_SOUND_END                                  ,
};



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SNDITEM;

class SNDFACTORY_XEVENT : public XEVENT
{
  public:
                                SNDFACTORY_XEVENT       (XSUBJECT* subject, XDWORD type = SNDFACTORY_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_SOUND);
    virtual                    ~SNDFACTORY_XEVENT       ();

    SNDITEM*                    GetItem                 ();
    void                        SetItem                 (SNDITEM* item);
                  
  private:

    void                        Clean                   ();           

    SNDITEM*                    item;                   
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





