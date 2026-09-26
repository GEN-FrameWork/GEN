/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBinaryMap.h
* 
* @class      XBINARYMAP
* @brief      eXtended Utils Binary map class
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

#include "XAVLTree.h"
#include "XPair.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <class K, class E, class comparator = XLESS<XPAIR<K, E> > , class Container = XAVLTREE<XPAIR<K, E>, comparator> >
class XBINARYMAP
{
  public:

    class XITERATOR;

                                                XBINARYMAP                                      (bool multi = true) : container(multi)
                                                {
                                                  Clean();
                                                }

    virtual                                    ~XBINARYMAP                                      ()
                                                {
                                                  Clean();
                                                }

    void                                        Add                                             (K key, E element)
                                                {
                                                  container.Add(XPAIR<K, E>(key, element));
                                                }

    E                                           Get                                             (K key)
                                                {
                                                  XPAIR<K, E> p = container.Get(XPAIR<K, E>(key, E()));
                                                  return p.right;
                                                }

    bool                                        Delete                                          (const K& key)
                                                {
                                                  return container.Delete(XPAIR<K, E>(key, E()));
                                                }

    bool                                        Delete                                          (XITERATOR it)
                                                {
                                                  return container.Delete(it.ite);
                                                }

    XDWORD                                      GetSize                                         ()
                                                {
                                                  return container.GetSize();
                                                }

    typename XBINARYMAP::XITERATOR              Begin                                           ()
                                                {
                                                  XITERATOR it;
                                                  it.ite = container.Begin();
                                                  return it;
                                                }

    typename XBINARYMAP::XITERATOR              End                                             ()
                                                {
                                                  XITERATOR it;
                                                  it.ite = container.End();
                                                  return it;
                                                }

      XLIST<E>                                  GetMultiple                                   (K& key)
                                                {
                                                  XLIST<E> ret;

                                                  GetMultiple(key, ret);

                                                  return ret;
                                                }

    void                                        GetMultiple                                   (K& key, XLIST<E>& list)
                                                {
                                                  XLIST<XPAIR<K, E> > lst;
                                                  container.GetMultiple(XPAIR<K, E>(key, E()), lst);

                                                  typename XLIST<XPAIR<K, E> >::XITERATOR it;
                                                  for(it = lst.Begin(); it != lst.End(); it++)
                                                    {
                                                      list.Add((*it).right);
                                                    }
                                                }

    int                                         Find                                          (K& key)
                                                {
                                                  return container.Find(XPAIR<K, E>(key, E()));
                                                }

  class XITERATOR
  {
    friend class XBINARYMAP;

    public:

                                                XITERATOR                                       ()
                                                {
                                                }

                                                XITERATOR                                       (typename XBINARYMAP::XITERATOR& rhs)
                                                {
                                                  *this = rhs;
                                                }

        typename XBINARYMAP::XITERATOR &        operator=                                       (const typename XBINARYMAP::XITERATOR& rhs)
                                                {
                                                  ite = rhs.ite;
                                                  return *this;
                                                }

        bool                                    operator!=                                      (const typename XBINARYMAP::XITERATOR & rhs)
                                                {
                                                  return ite != rhs.ite;
                                                }

        typename XBINARYMAP::XITERATOR          operator++                                      ()
                                                {
                                                  ++ite;

                                                  return *this;
                                                }

        typename XBINARYMAP::XITERATOR          operator++                                      (int)
                                                {
                                                  typename XBINARYMAP::XITERATOR it = *this;
                                                  (*this).ite++;

                                                  return it;
                                                }

        E                                       operator*                                       ()
                                                {
                                                  return (*ite).right;
                                                }

        K                                       GetKey                                          ()
                                                {
                                                  return (*ite).left;
                                                }

        E                                       GetElement                                      ()
                                                {
                                                  return (*ite).right;
                                                }

    private:

      typename Container::XITERATOR             ite;
};


  private:

    void                                        Clean                                           ()
                                                {

                                                }

    Container                                   container;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




