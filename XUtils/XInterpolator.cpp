/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XInterpolator.cpp
* 
* @class      XINTERPOLATOR
* @brief      eXtended Utils Interpolator class
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

#include "XInterpolator.h"

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR::XINTERPOLATOR(XINTERPOLATOR_TYPE type)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR::XINTERPOLATOR(XINTERPOLATOR_TYPE type)
{
  Clean();

  this->type = type;
  carry      = 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR::~XINTERPOLATOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR::~XINTERPOLATOR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR_TYPE XINTERPOLATOR::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XINTERPOLATOR_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR_TYPE XINTERPOLATOR::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetType(XINTERPOLATOR_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetType(XINTERPOLATOR_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR_LIMITMODE XINTERPOLATOR::GetMode()
* @brief      Get mode
* @ingroup    XUTILS
* 
* @return     XINTERPOLATOR_LIMITMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR_LIMITMODE XINTERPOLATOR::GetMode()
{
  return mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetMode(XINTERPOLATOR_LIMITMODE mode)
* @brief      Set mode
* @ingroup    XUTILS
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetMode(XINTERPOLATOR_LIMITMODE mode)
{
  this->mode = mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::Get(XDWORD& current, XDWORD& target, float& speed)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  current : Current value.
* @param[in]  target : Target value.
* @param[in]  speed : Speed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::Get(XDWORD& current, XDWORD& target, float& speed)
{
  current = this->current;
  target  = this->target;
  speed   = this->speed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::Set(XDWORD current, XDWORD target, float speed)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  current : Current value.
* @param[in]  target : Target value.
* @param[in]  speed : Speed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::Set(XDWORD current, XDWORD target, float speed)
{
  this->current = current;
  this->target  = target;
  this->speed   = speed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetCurrent()
* @brief      Get current
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetCurrent()
{
  return current;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetCurrent(XDWORD current)
* @brief      Set current
* @ingroup    XUTILS
* 
* @param[in]  current : Current value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetCurrent(XDWORD current)
{
  this->current = current;

  if(limits & XINTERPOLATOR_LIMITS_MAXIMUN)
    {
      if(current>(XDWORD)(GetMaximun() * GetMultiplicator()))
        {
          this->current =(XDWORD)(GetMaximun() * GetMultiplicator());
        }
    }

  if(limits & XINTERPOLATOR_LIMITS_MINIMUN)
    {
      if(this->current<(XDWORD)(GetMinimun() * GetMultiplicator()))
        {
          this->current =(XDWORD)(GetMinimun() * GetMultiplicator());
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetTarget()
* @brief      Get target
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetTarget()
{
  return target;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XINTERPOLATOR::GetSpeed()
* @brief      Get speed
* @ingroup    XUTILS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XINTERPOLATOR::GetSpeed()
{
  return speed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetSpeed(float speed)
* @brief      Set speed
* @ingroup    XUTILS
* 
* @param[in]  speed : Speed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetSpeed(float speed)
{
  this->speed = speed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetMaximun()
* @brief      Get maximun
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetMaximun()
{
  return max;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetMaximun(XDWORD max)
* @brief      Set maximun
* @ingroup    XUTILS
* 
* @param[in]  max : Max value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetMaximun(XDWORD max)
{
  this->max = max;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetMinimun()
* @brief      Get minimun
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetMinimun()
{
  return min;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetMinimun(XDWORD min)
* @brief      Set minimun
* @ingroup    XUTILS
* 
* @param[in]  min : Min value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetMinimun(XDWORD min)
{
  this->min = min;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetMultiplicator()
* @brief      Get multiplicator
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetMultiplicator()
{
  return multiplier;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetMultiplicator(XDWORD multiplier)
* @brief      Set multiplicator
* @ingroup    XUTILS
* 
* @param[in]  multiplier : Multiplier value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetMultiplicator(XDWORD multiplier)
{
  this->multiplier = multiplier;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetFactorType(XINTERPOLATOR_FACTOR factortype)
* @brief      Set factor type
* @ingroup    XUTILS
* 
* @param[in]  factortype : Factortype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetFactorType(XINTERPOLATOR_FACTOR factortype)
{
  this->factortype = factortype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR_FACTOR XINTERPOLATOR::GetFactorType()
* @brief      Get factor type
* @ingroup    XUTILS
* 
* @return     XINTERPOLATOR_FACTOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR_FACTOR XINTERPOLATOR::GetFactorType()
{
  return factortype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetLimitFlags(XINTERPOLATOR_LIMITS limits)
* @brief      Set limit flags
* @ingroup    XUTILS
* 
* @param[in]  limits : Limits value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetLimitFlags(XINTERPOLATOR_LIMITS limits)
{
  this->limits = limits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XINTERPOLATOR_LIMITS XINTERPOLATOR::GetLimitFlags()
* @brief      Get limit flags
* @ingroup    XUTILS
* 
* @return     XINTERPOLATOR_LIMITS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XINTERPOLATOR_LIMITS  XINTERPOLATOR::GetLimitFlags()
{
  return limits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::CopyFrom(XINTERPOLATOR& interpolator)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  interpolator : Interpolator value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::CopyFrom(XINTERPOLATOR& interpolator)
{
  type          = interpolator.type;
  mode          = interpolator.mode;

  current       = interpolator.current;
  target        = interpolator.target;
  speed         = interpolator.speed;
  max           = interpolator.max;
  min           = interpolator.min;
  limits        = interpolator.limits;

  factortype    = interpolator.factortype;

  Update(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::CopyFrom(XINTERPOLATOR* interpolator)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  interpolator : Interpolator pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::CopyFrom(XINTERPOLATOR* interpolator)
{
  if(!interpolator) return false;

  return CopyFrom((*interpolator));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::CopyTo(XINTERPOLATOR& interpolator)
* @brief      Copy to
* @ingroup    XUTILS
* 
* @param[in]  interpolator : Interpolator value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::CopyTo(XINTERPOLATOR& interpolator)
{
  interpolator.type     = type;
  interpolator.mode     = mode;

  interpolator.current  = current;
  interpolator.target   = target;
  interpolator.speed    = speed;
  interpolator.max      = max;
  interpolator.min      = min;
  interpolator.limits   = limits;

  factortype            = interpolator.factortype;

  Update(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::CopyTo(XINTERPOLATOR* interpolator)
* @brief      Copy to
* @ingroup    XUTILS
* 
* @param[in]  interpolator : Interpolator pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::CopyTo(XINTERPOLATOR* interpolator)
{
  if(!interpolator) return false;

  return CopyTo((*interpolator));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XINTERPOLATOR XINTERPOLATOR::operator = (XINTERPOLATOR& interpolator)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  interpolator : Interpolator value.
* 
* @return     const XINTERPOLATOR XINTERPOLATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XINTERPOLATOR XINTERPOLATOR::operator = (XINTERPOLATOR& interpolator)
{
  CopyFrom(interpolator);
  return (*this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetTarget(XDWORD target)
* @brief      Set target
* @ingroup    XUTILS
* 
* @param[in]  target : Target value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetTarget(XDWORD target)
{
  //XTRACE_PRINTCOLOR(3,__L("XINTERPOLATOR::SetTarget(%d)"),target);

  if(limits & XINTERPOLATOR_LIMITS_MAXIMUN)
    {
      if(target>max)
        {
          //XTRACE_PRINTCOLOR(4,__L("Assigned over maximun: %d/%d"),target,max);

          this->target  = max;

          return;
        }
    }

  if(limits & XINTERPOLATOR_LIMITS_MINIMUN)
    {
      if(target < min)
        {
          //XTRACE_PRINTCOLOR(4,__L("Assigned under minimun: %d/%d"),target,max);

          this->target = min;

          return;
        }
    }

  this->target = target;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::Update(XQWORD delta)
* @brief      Update
* @ingroup    XUTILS
* 
* @param[in]  delta : Delta value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::Update(XQWORD delta)
{
  switch(type)
    {
      case XINTERPOLATOR_TYPE_LIMIT     : Interpolate(delta);

                                          if(mode == XINTERPOLATOR_LIMITMODE_INCREMENTAL)
                                            {
                                              if(target<=current) current=target;
                                            }
                                           else
                                            {
                                              if(mode == XINTERPOLATOR_LIMITMODE_DECREMENTAL)
                                              if(target>=current) current = target;
                                            }

                                          break;

      case XINTERPOLATOR_TYPE_RELATIVE  :
      case XINTERPOLATOR_TYPE_ABSOLUTE  : if(current > target) current = target;
                                          if(current < 0)      current = 0;
                                          break;

      case XINTERPOLATOR_TYPE_NONE      : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::Interpolate(XQWORD delta)
* @brief      Interpolate
* @ingroup    XUTILS
* 
* @param[in]  delta : Delta value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::Interpolate(XQWORD delta)
{
  switch(factortype)
    {
      case XINTERPOLATOR_FACTOR_RELATIVE  : return InterpolateRelative(delta);
      case XINTERPOLATOR_FACTOR_LINEAR    : return InterpolateLinear  (delta);
      case XINTERPOLATOR_FACTOR_NONE      :
                                default   : break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::InterpolateLinear(XQWORD delta)
* @brief      Interpolate linear
* @ingroup    XUTILS
* 
* @param[in]  delta : Delta value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::InterpolateLinear(XQWORD delta)
{
  float   fdelta    = (float)(delta/100.0f);
  int     itarget   = (int)(target);
  float   result    = (float)current;
  int     margin    = 0;

  if(result ==  target) return false;

  if(result < itarget)
    {
      if (delta!=0)
        {
          if (carry<0.0000001f)  carry=0.0f;

          float inc = (speed*fdelta) + carry;
          carry = inc - float((int)inc);

          result += (int)inc;
        }

      if(result >= itarget)
        {
          result  = (float)itarget;
          carry   = 0.0f;
        }

    }
   else
    {
      if(delta!=0)
        {
          if(carry<0.0000001f) carry=0.0f;

          float inc=(speed*fdelta)+carry;
          carry=inc-float((int)inc);

          result -= (int)inc;
        }

      if(result <= float(itarget))
        {
          result = (float)itarget;
          carry  = 0.0f;
        }
    }

  current = (XDWORD)result;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::InterpolateRelative(XQWORD delta)
* @brief      Interpolate relative
* @ingroup    XUTILS
* 
* @param[in]  delta : Delta value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::InterpolateRelative(XQWORD delta)
{
  float   fdelta    = (float) (delta/100.0f);
  int     itarget   = (int)   (target);
  float   result    = (float) (current);
  int     margin    = 0;

  if(result ==  target) return false;

  if(result < itarget)
    {
      if(delta!=0)
        {
          if(carry<0.0000001f) carry=0.0f;

          float dif = (float)(itarget - result);
          float inc = ((speed/100.0f)*fdelta*dif)+carry;

          carry   = inc - float((int)inc);
          result += (int)inc;
        }

      if(result > (itarget-margin))
        {
          result  = (float)itarget;
          carry   = 0.0f;
        }
    }
   else
    {
      if(delta!=0)
        {
          if (carry<0.0000001f) carry=0.0f;

          float dif = (float)(result-itarget);
          float inc = ((speed/100.0f)*fdelta*dif)+carry;

          carry   = inc - float((int)inc);
          result -= (int)inc;
        }

      if(result < float(itarget+margin))
        {
          result = (float)itarget;
          carry  = 0.0f;
        }
    }

  current = (XDWORD)result;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::DeSerialize(XBUFFER* xbuffer) const
* @brief      De serialize
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::DeSerialize(XBUFFER* xbuffer) const
{
  xbuffer->Resize(9*sizeof(XDWORD) + 1*sizeof(float));

  xbuffer->SetPosition(0);
  xbuffer->Set((XDWORD)mask);
  xbuffer->Set((XDWORD)type);
  xbuffer->Set((XDWORD)mode);
  xbuffer->Set((XDWORD)current);
  xbuffer->Set((XDWORD)target);
  xbuffer->Set((float)speed);
  xbuffer->Set((XDWORD)max);
  xbuffer->Set((XDWORD)min);
  xbuffer->Set((XDWORD)multiplier);
  xbuffer->Set((XDWORD)factortype);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::Serialize(XBUFFER* buffer)
* @brief      Serialize
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::Serialize(XBUFFER* buffer)
{
  XDWORD buffer_type,    buffer_target, buffer_mode,   buffer_factortype,buffer_max,      buffer_min,buffer_current,buffer_multiplier;
  float  buffer_speed;

  buffer->SetPosition(0);

  buffer->Get((XDWORD&)mask);
  buffer->Get((XDWORD&)buffer_type);
  buffer->Get((XDWORD&)buffer_mode);
  buffer->Get((XDWORD&)buffer_current);
  buffer->Get((XDWORD&)buffer_target);
  buffer->Get((float&)buffer_speed);
  buffer->Get((XDWORD&)buffer_max);
  buffer->Get((XDWORD&)buffer_min);
  buffer->Get((XDWORD&)buffer_multiplier);
  buffer->Get((XDWORD&)buffer_factortype);

  if(mask & XINTERPOLATOR_MASK_SPEED)      speed       = buffer_speed;
  if(mask & XINTERPOLATOR_MASK_MAXIMUN)    max         = buffer_max;
  if(mask & XINTERPOLATOR_MASK_MINIMUN)    min         = buffer_min;
  if(mask & XINTERPOLATOR_MASK_MULTIPLIER) multiplier  = buffer_multiplier;
  if(mask & XINTERPOLATOR_MASK_TYPE)       type        = (XINTERPOLATOR_TYPE)buffer_type;
  if(mask & XINTERPOLATOR_MASK_FACTOR)     factortype  = (XINTERPOLATOR_FACTOR)buffer_factortype;
  if(mask & XINTERPOLATOR_MASK_TARGET)     SetTarget(buffer_target);
  if(mask & XINTERPOLATOR_MASK_CURRENT)    SetCurrent(buffer_current);

  Update(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XINTERPOLATOR::SetMask(XDWORD mask)
* @brief      Set mask
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XINTERPOLATOR::SetMask(XDWORD mask)
{
  this->mask = mask;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XINTERPOLATOR::GetMask()
* @brief      Get mask
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetMask()
{
  return mask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XINTERPOLATOR::GetCarry()
* @brief      Get carry
* @ingroup    XUTILS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XINTERPOLATOR::GetCarry()
{
  return carry;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::SetCarry(float carry)
* @brief      Set carry
* @ingroup    XUTILS
* 
* @param[in]  carry : Carry value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::SetCarry(float carry)
{
  this->carry = carry;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XINTERPOLATOR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XINTERPOLATOR::Clean()
{
  mask        = XINTERPOLATOR_MASK_ALL;
  type        = XINTERPOLATOR_TYPE_ABSOLUTE;
  mode        = XINTERPOLATOR_LIMITMODE_NONE;

  max         = 0;
  min         = 0;
  current     = 0;
  target      = 0;
  speed       = 0.0f;
  factortype  = XINTERPOLATOR_FACTOR_LINEAR;
  limits      = XINTERPOLATOR_LIMITS_NONE;
  carry       = 0.0f;
  multiplier  = 1;
}



