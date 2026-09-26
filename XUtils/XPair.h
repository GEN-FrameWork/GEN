/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XPair.h
* 
* @class      XPAIR
* @brief      eXtended Utils Pair
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

template <class L, class R>
class XPAIR
{
  public:
                          XPAIR                                     ()
                          {
                            Clean();
                          }

                          XPAIR                                     (L left, R right)
                          {
                            Clean();
                            this->left = left;
                            this->right = right;
                          }

                          XPAIR                                     (const XPAIR<L, R>& rhs)
                          {
                            *this = rhs;
                          }

    XPAIR &               operator=                                 (const XPAIR<L, R>& rhs)
                          {
                            left = rhs.left;
                            right = rhs.right;
                            return *this;
                          }

  bool                    operator ==                               (const XPAIR<L, R>& rhs) const
                          {
                            return (left == rhs.left) && (right == rhs.right);
                          }

    L                     left;
    R                     right;

  private:

    void                  Clean                                     ()
                          {
                            left  = L();
                            right = R();
                          }
};


template <class T1, class T2>
bool operator<(const XPAIR<T1, T2>& x, const XPAIR<T1, T2>& y)
{
  return x.left < y.left; // SGI stl did the same, not much sense comparing the right element
}




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





