/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFSMachine.h
* 
* @class      XFSMACHINE
* @brief      eXtended Utils Finite state machine class
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
#include "XMap.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XFSMACHINESTATE_EVENTDEFEND  -1



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFSMACHINESTATE
{
  public:
                                    XFSMACHINESTATE               (int stateID, int ntransitions);
    virtual                        ~XFSMACHINESTATE               ();

    int                             GetID                         ();
    bool                            AddTransition                 (int input, int outputID);
    int                             GetOutput                     (int input);
    bool                            DeleteTransition              (int outputID);

  private:

    void                            Clean                         ();

    int                             stateID;
    int                             maxtransitions;
    int*                            inputs;
    int*                            outputstates;
};

class XFSMACHINE
{
  public:
                                    XFSMACHINE                    (int stateID);
    virtual                        ~XFSMACHINE                    ();

    int                             GetEvent                      ();
    bool                            SetEvent                      (int event);

    int                             GetCurrentState               ();
    void                            SetCurrentState               (int stateID);

    XFSMACHINESTATE*                GetState                      (int stateID);
    bool                            AddState                      (XFSMACHINESTATE* state);
    bool                            AddSecuencialStates           (int state, int maxsecuencialtransitions);
    bool                            AddState                      (int state, int event, int tostate, ...);
    bool                            DeleteState                   (int stateID);
    bool                            DeleteAllStates               ();

    int                             StateTransition               (int input);
    bool                            CheckTransition               (int& event);
    bool                            CheckTransition               ();

  private:

    void                            Clean                         ();

    int                             fsmevent;
    int                             currentstate;
    XMAP<int, XFSMACHINESTATE*>     map;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





