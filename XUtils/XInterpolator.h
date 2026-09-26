/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XInterpolator.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSerializable.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XINTERPOLATOR_TYPE
{
  XINTERPOLATOR_TYPE_NONE               = 0    ,
  XINTERPOLATOR_TYPE_ABSOLUTE                  ,
  XINTERPOLATOR_TYPE_RELATIVE                  ,
  XINTERPOLATOR_TYPE_LIMIT
};

enum XINTERPOLATOR_LIMITMODE
{
  XINTERPOLATOR_LIMITMODE_NONE          = 0    ,
  XINTERPOLATOR_LIMITMODE_INCREMENTAL          ,
  XINTERPOLATOR_LIMITMODE_DECREMENTAL          ,
  XINTERPOLATOR_LIMITMODE_BOTHWAYS
};

enum XINTERPOLATOR_LIMITS
{
  XINTERPOLATOR_LIMITS_NONE             = 0     ,
  XINTERPOLATOR_LIMITS_MAXIMUN          = 1<<1  ,
  XINTERPOLATOR_LIMITS_MINIMUN          = 1<<2  ,
  XINTERPOLATOR_LIMITS_RANGE            = XINTERPOLATOR_LIMITS_MAXIMUN | XINTERPOLATOR_LIMITS_MINIMUN
};

enum XINTERPOLATOR_FACTOR
{
  XINTERPOLATOR_FACTOR_NONE             = 0     ,
  XINTERPOLATOR_FACTOR_LINEAR                   ,
  XINTERPOLATOR_FACTOR_RELATIVE
};

enum XINTERPOLATOR_MASK
{
  XINTERPOLATOR_MASK_NONE               = 0     ,
  XINTERPOLATOR_MASK_CURRENT            = 1     ,
  XINTERPOLATOR_MASK_TARGET             = 1<<1  ,
  XINTERPOLATOR_MASK_SPEED              = 1<<2  ,
  XINTERPOLATOR_MASK_TYPE               = 1<<3  ,
  XINTERPOLATOR_MASK_FACTOR             = 1<<4  ,
  XINTERPOLATOR_MASK_MAXIMUN            = 1<<5  ,
  XINTERPOLATOR_MASK_MINIMUN            = 1<<6  ,
  XINTERPOLATOR_MASK_MULTIPLIER         = 1<<7  ,
  XINTERPOLATOR_MASK_ALL                = XINTERPOLATOR_MASK_CURRENT      |
                                          XINTERPOLATOR_MASK_TARGET       |
                                          XINTERPOLATOR_MASK_SPEED        |
                                          XINTERPOLATOR_MASK_MAXIMUN      |
                                          XINTERPOLATOR_MASK_MINIMUN      |
                                          XINTERPOLATOR_MASK_TYPE         |
                                          XINTERPOLATOR_MASK_MULTIPLIER   |
                                          XINTERPOLATOR_MASK_FACTOR
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XINTERPOLATOR : public XSERIALIZABLE
{
  public:

                                    XINTERPOLATOR                 (XINTERPOLATOR_TYPE type  = XINTERPOLATOR_TYPE_ABSOLUTE);
    virtual                        ~XINTERPOLATOR                 ();

    XINTERPOLATOR_TYPE              GetType                       ();
    void                            SetType                       (XINTERPOLATOR_TYPE type);

    XINTERPOLATOR_LIMITMODE         GetMode                       ();
    void                            SetMode                       (XINTERPOLATOR_LIMITMODE mode);

    void                            Get                           (XDWORD& current, XDWORD& target, float& speed);
    void                            Set                           (XDWORD current, XDWORD target, float speed);

    XDWORD                          GetCurrent                    ();
    void                            SetCurrent                    (XDWORD current);

    XDWORD                          GetTarget                     ();

    float                           GetSpeed                      ();
    void                            SetSpeed                      (float speed);

    XDWORD                          GetMaximun                    ();
    void                            SetMaximun                    (XDWORD max);

    XDWORD                          GetMinimun                    ();
    void                            SetMinimun                    (XDWORD min);

    XDWORD                          GetMultiplicator              ();
    void                            SetMultiplicator              (XDWORD mult);

    void                            SetFactorType                 (XINTERPOLATOR_FACTOR factortype);
    XINTERPOLATOR_FACTOR            GetFactorType                 ();

    void                            SetLimitFlags                 (XINTERPOLATOR_LIMITS l);
    XINTERPOLATOR_LIMITS            GetLimitFlags                 ();

    bool                            CopyFrom                      (XINTERPOLATOR& interpolator);
    bool                            CopyFrom                      (XINTERPOLATOR* interpolator);

    bool                            CopyTo                        (XINTERPOLATOR& interpolator);
    bool                            CopyTo                        (XINTERPOLATOR* interpolator);

    const XINTERPOLATOR             operator  =                   (XINTERPOLATOR& interpolator);

    virtual bool                    Update                        (XQWORD delta);
    void                            SetTarget                     (XDWORD target);

    bool                            DeSerialize                   (XBUFFER* buffer) const;
    bool                            Serialize                     (XBUFFER* buffer);

    bool                            SetMask                       (XDWORD mask);
    XDWORD                          GetMask                       ();

    float                           GetCarry                      ();
    void                            SetCarry                      (float carry);

protected:

    bool                            Interpolate                   (XQWORD delta);
    bool                            InterpolateLinear             (XQWORD delta);
    bool                            InterpolateRelative           (XQWORD delta);

    XDWORD                          mask;

    XINTERPOLATOR_TYPE              type;
    XINTERPOLATOR_LIMITMODE         mode;

    XDWORD                          current;
    XDWORD                          target;
    float                           speed;
    XDWORD                          max;
    XDWORD                          min;
    XDWORD                          multiplier;

    float                           carry;

    XINTERPOLATOR_FACTOR            factortype;
    XINTERPOLATOR_LIMITS            limits;

  private:

    void                            Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





