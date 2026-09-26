/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XFSMachine.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFSMachine.h"

#include <stdarg.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/





/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFSMACHINESTATE::XFSMACHINESTATE(int stateID, int ntransitions)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  stateID : State ID value.
* @param[in]  ntransitions : Ntransitions value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFSMACHINESTATE::XFSMACHINESTATE(int stateID, int ntransitions)
{
  Clean();

  this->maxtransitions = ntransitions?ntransitions:1;
  this->stateID        = stateID;

  inputs               = NULL;
  outputstates         = NULL;

  inputs = GEN_NEW int[this->maxtransitions];
  if(!inputs) return;

  outputstates = GEN_NEW int[this->maxtransitions];
  if(!outputstates)
    {
      GEN_DELETE_ARRAY inputs;
      return;
    }

  for(int c=0; c<this->maxtransitions;c++)
    {
      inputs[c]       = 0;
      outputstates[c] = 0;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFSMACHINESTATE::~XFSMACHINESTATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFSMACHINESTATE::~XFSMACHINESTATE()
{
  GEN_DELETE_ARRAY inputs;
  GEN_DELETE_ARRAY outputstates;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFSMACHINESTATE::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFSMACHINESTATE::GetID()
{
  return stateID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINESTATE::AddTransition(int input, int outputID)
* @brief      Add transition
* @ingroup    XUTILS
* 
* @param[in]  input : Input value.
* @param[in]  outputID : Output output id.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINESTATE::AddTransition(int input, int outputID)
{
  int c;

  for(c=0;c<maxtransitions;c++)
    {
      if(!outputstates[c]) break;
    }

  if(c<maxtransitions)
    {
      outputstates[c] = outputID;
      inputs[c]       = input;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFSMACHINESTATE::GetOutput(int input)
* @brief      Get output
* @ingroup    XUTILS
* 
* @param[in]  input : Input value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFSMACHINESTATE::GetOutput(int input)
{
  int outputID = stateID;

  for(int c=0;c<maxtransitions;c++)
    {
      if(!outputstates[c]) break;

      if(input == inputs[c])
        {
          outputID = outputstates[c];
          break;
        }
    }

  return(outputID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINESTATE::DeleteTransition(int outputID)
* @brief      Delete transition
* @ingroup    XUTILS
* 
* @param[in]  outputID : Output output id.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINESTATE::DeleteTransition(int outputID)
{
  int c;

  for(c=0;c<maxtransitions;c++)
    {
      if(outputstates[c]==outputID) break;
    }

  if(c>=maxtransitions) return false;

  for(;c<(maxtransitions-1);c++)
    {
      inputs[c]       = inputs[c+1];
      outputstates[c] = outputstates[c+1];
    }

  inputs[c]       = 0;
  outputstates[c] = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFSMACHINESTATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFSMACHINESTATE::Clean()
{
  stateID        = 0;
  maxtransitions = 0;

  inputs         = NULL;
  outputstates   = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFSMACHINE::XFSMACHINE(int stateID)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  stateID : State ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFSMACHINE::XFSMACHINE(int stateID)
{
  Clean();

  SetCurrentState(stateID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFSMACHINE::~XFSMACHINE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFSMACHINE::~XFSMACHINE()
{
  DeleteAllStates();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFSMACHINE::GetEvent()
* @brief      Get event
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFSMACHINE::GetEvent()
{
  return fsmevent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::SetEvent(int event)
* @brief      Set event
* @ingroup    XUTILS
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::SetEvent(int event)
{
  fsmevent = event;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFSMACHINE::GetCurrentState()
* @brief      Get current state
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFSMACHINE::GetCurrentState()
{
  return currentstate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFSMACHINE::SetCurrentState(int stateID)
* @brief      Set current state
* @ingroup    XUTILS
* 
* @param[in]  stateID : State ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFSMACHINE::SetCurrentState(int stateID)
{
  currentstate = stateID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFSMACHINESTATE* XFSMACHINE::GetState(int stateID)
* @brief      Get state
* @ingroup    XUTILS
* 
* @param[in]  stateID : State ID value.
* 
* @return     XFSMACHINESTATE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFSMACHINESTATE* XFSMACHINE::GetState(int stateID)
{
  XFSMACHINESTATE* state = NULL;
  if(map.IsEmpty()) return state;

  int index = map.Find(stateID);
  if(index==-1) return state;

  state = map.GetElement(index);

  return state;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::AddState(XFSMACHINESTATE* state)
* @brief      Add state
* @ingroup    XUTILS
* 
* @param[in]  state : State pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::AddState(XFSMACHINESTATE* state)
{
  if(GetState(state->GetID())) return false;

  map.Add(state->GetID(), state);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::AddState(int state, int event, int tostate, ...)
* @brief      Add state
* @ingroup    XUTILS
* 
* @param[in]  state : State value.
* @param[in]  event : Event to process.
* @param[in]  tostate : Tostate value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::AddState(int state, int event, int tostate, ...)
{
  XFSMACHINESTATE* _state;
  int              narg = 0;
  va_list          arg;

  va_start(arg, tostate);
  while(va_arg(arg, int) != XFSMACHINESTATE_EVENTDEFEND)
   {
     narg++;
   }
  va_end(arg);

  if((narg)&&(narg%2)) return false;

  narg+=2;

  _state = GEN_NEW XFSMACHINESTATE(state, (narg/2));
  if(!_state) return false;

  _state->AddTransition(event, tostate);

  va_start(arg, tostate);

  int _event;
  int _tostate;

  do { _event   = va_arg(arg, int);
       if(_event != XFSMACHINESTATE_EVENTDEFEND)
        {
          _tostate = va_arg(arg, int);
          _state->AddTransition(_event, _tostate);
        }

     } while(_event != XFSMACHINESTATE_EVENTDEFEND);

  va_end(arg);

  if(!AddState(_state))
    {
      GEN_DELETE _state;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::AddSecuencialStates(int state, int maxsecuencialtransitions)
* @brief      Add secuencial states
* @ingroup    XUTILS
* 
* @param[in]  state : State value.
* @param[in]  maxsecuencialtransitions : Maxsecuencialtransitions value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::AddSecuencialStates(int state, int maxsecuencialtransitions)
{
  XFSMACHINESTATE* _state;

  _state = GEN_NEW XFSMACHINESTATE(state, maxsecuencialtransitions);
  if(!_state) return false;

  for(int c=0;c<maxsecuencialtransitions;c++)
    {
      _state->AddTransition(c, c);
    }

  if(!AddState(_state))
    {
      GEN_DELETE _state;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::DeleteState(int stateID)
* @brief      Delete state
* @ingroup    XUTILS
* 
* @param[in]  stateID : State ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::DeleteState(int stateID)
{
  XFSMACHINESTATE* state = GetState(stateID);

  if(!state) return false;

  map.Delete(stateID);

  GEN_DELETE state;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::DeleteAllStates()
* @brief      Delete all states
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::DeleteAllStates()
{
  if(map.IsEmpty()) return false;

  map.DeleteElementContents();
  map.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XFSMACHINE::StateTransition(int input)
* @brief      State transition
* @ingroup    XUTILS
* 
* @param[in]  input : Input value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XFSMACHINE::StateTransition(int input)
{
  XFSMACHINESTATE*  state = GetState(currentstate);

  if(!state)
    {
      currentstate = 0;
      return currentstate;
    }

  currentstate = state->GetOutput(input);

  return currentstate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::CheckTransition(int& event)
* @brief      Check transition
* @ingroup    XUTILS
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::CheckTransition(int& event)
{
  int state = GetCurrentState();

  int tevent = (int)GetEvent();

  StateTransition(GetEvent());

  SetEvent(0);

  if(GetCurrentState()==state) return false;

  event = tevent;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XFSMACHINE::CheckTransition()
* @brief      Check transition
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XFSMACHINE::CheckTransition()
{
  int state = GetCurrentState();

  StateTransition(GetEvent());

  SetEvent(0);

  if(GetCurrentState()==state) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XFSMACHINE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XFSMACHINE::Clean()
{
  fsmevent     = 0;
  currentstate = 0;
}




