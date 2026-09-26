/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDOpenALFactory.h
* 
* @class      SNDOPENALFACTORY
* @brief      Sound OpenAL Play Item class
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

#include "XFSMachine.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum SNDOPENALPLAYITEM_XFSMEVENTS
{
  SNDOPENALPLAYITEM_XFSMEVENT_NONE              = 0 ,
  SNDOPENALPLAYITEM_XFSMEVENT_INI                   ,
  SNDOPENALPLAYITEM_XFSMEVENT_PLAY                  ,
  SNDOPENALPLAYITEM_XFSMEVENT_PAUSE                 ,
  SNDOPENALPLAYITEM_XFSMEVENT_STOP                  ,
  SNDOPENALPLAYITEM_XFSMEVENT_END                   ,

  SNDOPENALPLAYITEM_LASTEVENT
};

enum DEVTESTSCONSOLEXFSMSTATES
{
  SNDOPENALPLAYITEM_XFSMSTATE_NONE              = 0 ,
  SNDOPENALPLAYITEM_XFSMSTATE_INI                   ,
  SNDOPENALPLAYITEM_XFSMSTATE_PLAY                  ,
  SNDOPENALPLAYITEM_XFSMSTATE_PAUSE                 ,
  SNDOPENALPLAYITEM_XFSMSTATE_STOP                  ,
  SNDOPENALPLAYITEM_XFSMSTATE_END                   ,

  SNDOPENALPLAYITEM_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SNDITEM;
class SNDOPENALSOURCE;

class SNDOPENALPLAYITEM : public XFSMACHINE
{
  public:
                                              SNDOPENALPLAYITEM         ();
    virtual                                  ~SNDOPENALPLAYITEM         ();

    bool                                      IniFSMachine              ();

    SNDITEM*                                  GetItem                   ();
    void                                      SetItem                   (SNDITEM* item);

    SNDOPENALSOURCE*                          GetSource                 ();
    void                                      SetSource                 (SNDOPENALSOURCE* source);

    bool                                      Delete                    ();

  private:

    void                                      Clean                     ();   

    SNDITEM*                                  item;
    SNDOPENALSOURCE*                          source;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





