/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XInheritEnum.h
* 
* @class      XINHERITENUM
* @brief      eXtended Utils Inherit Enums
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <typename enumT, typename baseenumT>
class XINHERITENUM
{
  public:

                        XINHERITENUM                    () 
                        {
                        }

                        XINHERITENUM                    (enumT e) : enum_(e)
                        {        
                        }

                        XINHERITENUM                    (baseenumT e) : baseEnum_(e)
                        {        
                        }

    explicit            XINHERITENUM                    ( int val ) : enum_(static_cast<enumT>(val))
                        {
    
                        }

    operator            enumT                           () const 
                        { 
                          return enum_; 
                        }

     baseenumT          TakeAsBase()
                        {
                          return  baseenumT(static_cast<enumT>((int)(*this)));
                        }
  private:





     union
     { 
        enumT enum_;
        baseenumT baseEnum_;
     };
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





