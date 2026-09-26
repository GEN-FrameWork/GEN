/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XList.h
* 
* @class      XLIST
* @brief      eXtended Utils Double Linked List
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

#include <stdio.h>
#include <string.h>

#include "XBase.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <class T>
class XLISTDATA
{
  template <class U>
  friend class XLIST;

  public:
                                    XLISTDATA                                 ()
                                    {
                                      Clean();
                                    }

    virtual                        ~XLISTDATA                                 ()
                                    {
                                      Delete();
                                      Clean();
                                    }

    bool                            Copy                                      (XLISTDATA* listdata)
                                    {
                                      if(!listdata) return false;

                                      active = listdata->active;
                                      data   = listdata->data;

                                      return true;
                                    }

    bool                            Delete                                    ()
                                    {
                                      if(!active) return false;

                                      active = false;
                                      data   = T();

                                      return true;
                                    }

    bool                            active;
    T                               data;
    XLISTDATA<T>*                   next;
    XLISTDATA<T>*                   prev;

private:

    void                            Clean                                     ()
                                    {
                                      active = false;
                                      data   = T();

                                      next    = 0;
                                      prev    = 0;
                                    }
};



template <class T>
XLISTDATA<T>* Increment(XLISTDATA<T>* data)
{
  return data->next;
}



template <class T>
XLISTDATA<T>* Decrement(XLISTDATA<T>* data)
{
  return data->prev;
}

template <class T>
bool IsFirst(XLISTDATA<T>* data)
{
  return data->prev == NULL;
}

template <class T>
bool IsLast(XLISTDATA<T>* data)
{
  return data == NULL;
}

template <class T>
class XLIST
{
  public:

    class XITERATOR;
                                    XLIST                                     ()
                                    {
                                      Clean();
                                    }
    virtual                        ~XLIST                                     ()
                                    {
                                      DeleteAll();
                                      Clean();
                                    }

    XLIST<T>&                       operator=(XLIST<T>& rhs)
                                    {
                                      XLISTDATA<T>* it = rhs.GetFirstData();

                                      while(!IsLast(it))
                                        {
                                          Add(it->data);

                                          it = Increment(it);
                                        }

                                      return *this;
                                    }

    bool                            IsEmpty                                   ()                  { return nelements == 0;                  }
    XDWORD                          GetSize                                   ()                  { return nelements;                       }

    bool                            Resize                                    (XDWORD newsize)
                                    {
                                      if(newsize == 0)
                                        {
                                          return DeleteAll();
                                        }

                                      if(newsize >= nelements)
                                        {

                                          XDWORD toinsert = newsize - nelements;

                                          XLISTDATA<T>* data = lastelement;

                                          for(XDWORD i = 0; i < toinsert; i++)
                                            {
                                              XLISTDATA<T>* newdata = GEN_NEW XLISTDATA<T>();
                                              if(!newdata) return false;

                                              newdata->prev = data;
                                              data->next = newdata;
                                              lastelement = newdata;
                                              data = newdata;
                                            }

                                          nelements = newsize;
                                          lastelement = data;
                                          return true;
                                        }

                                      XDWORD todelete = nelements - newsize;

                                      XLISTDATA<T>* data = lastelement;
                                      XLISTDATA<T>* previous;
                                      for(XDWORD i = 0; i < todelete; i++)
                                        {
                                          previous = data->prev;

                                          data->Delete();
                                          GEN_DELETE data;

                                          data = previous;
                                        }

                                      lastelement = data;

                                      nelements = newsize;
                                      return true;
                                    }

    void                            SetIsMulti                                 (bool ismulti)     { this->ismulti = ismulti; }

    virtual bool                    Add                                        (T element)
                                    {
                                      if(!ismulti)
                                        {
                                          if(Find(element) != NOTFOUND)
                                            {
                                              return false;
                                            }
                                        }

                                      XLISTDATA<T>* data = GEN_NEW XLISTDATA<T>();
                                      data->active = true;
                                      data->data = element;

                                      if(nelements == 0)
                                        {
                                          firstelement = data;
                                          lastelement = data;
                                          nelements++;
                                          return true;
                                        }

                                      lastelement->next = data;
                                      data->prev = lastelement;
                                      nelements++;
                                      lastelement = data;
                                      return true;
                                    }

    int                             Find                                     (T element)
                                    {
                                      int index = 0;

                                      XLISTDATA<T>* data = firstelement;

                                      while(data)
                                        {
                                          if(data->data == element)
                                            {
                                              return index;
                                            }

                                          data = data->next;
                                          index++;
                                        }

                                      return NOTFOUND;
                                    }

    XLISTDATA<T>*                   FindData                                  (T element)
                                    {
                                      XDWORD index = 0;

                                      XLISTDATA<T>* data = firstelement;

                                      while(data)
                                        {
                                          if(data->data == element)
                                            {
                                              return data;
                                            }

                                          data = data->next;
                                          index++;
                                        }

                                      return NULL;
                                    }

    T                               Get                                       (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return T();
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      return data->data;
                                    }

    XLISTDATA<T>*                   GetData                                   (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return T();
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      return data;
                                    }

    bool                            Set                                       (XDWORD index, T element)
                                    {
                                      if(index >= nelements)
                                        {
                                          return false;
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      data->data = element;
                                      return true;
                                    }

    T                               GetFirst                                  ()                  { return firstelement->data; }
    XLISTDATA<T>*                   GetFirstData                              ()                  { return firstelement;       }
    T                               GetLast                                   ()                  { return lastelement->data;  }
    XLISTDATA<T>*                   GetLastData                               ()                  { return lastelement;        }

    bool                            Insert                                    (XDWORD index, T element)
                                    {
                                      if(index >= nelements) return false;

                                      if(!ismulti)
                                        {
                                          if(Find(element) != NOTFOUND)
                                            {
                                              return false;
                                            }
                                        }

                                      if(index == 0)
                                        {
                                          Insert(firstelement, element);
                                          return true;
                                        }

                                      if(index == (nelements-1))
                                        {
                                          Insert(lastelement, element);
                                          return true;
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          if(i == index)
                                            {
                                              return Insert(data, element);
                                            }

                                          data = data->next;
                                        }

                                      return false;
                                    }

    bool                            Insert                                    (XLISTDATA<T>* position, T element)
                                    {
                                      if(nelements == 0)
                                        {
                                          XLISTDATA<T>* newdata = GEN_NEW XLISTDATA<T>();
                                          if(!newdata) return false;

                                          newdata->data = element;
                                          firstelement = newdata;
                                          lastelement = newdata;
                                          nelements++;
                                          return true;
                                        }

                                      if(position == NULL)
                                        {
                                          Add(element);
                                        }

                                      if(position == firstelement)
                                        {
                                          XLISTDATA<T>* newdata = GEN_NEW XLISTDATA<T>();
                                          newdata->data = element;
                                          newdata->next = firstelement;
                                          firstelement->prev = newdata;
                                          firstelement = newdata;
                                          nelements++;
                                          return true;
                                        }

                                      XLISTDATA<T>* previous = position->prev;
                                      XLISTDATA<T>* newdata = GEN_NEW XLISTDATA<T>();
                                      if(!newdata) return false;

                                      newdata->data = element;
                                      newdata->next = position;
                                      newdata->prev = previous;
                                      previous->next = newdata;
                                      position->prev = newdata;

                                      nelements++;

                                      return true;
                                    }

    bool                            InsertPrevious                            (XLISTDATA<T>* position, T element)
                                    {
                                      return Insert(position, element);
                                    }

    bool                            InsertNext                                (XLISTDATA<T>* position, T element)
                                    {
                                      return Insert(position->next, element);
                                    }

    bool                            Delete                                    (T element)
                                    {
                                      XLISTDATA<T>* data = firstelement;

                                      if(nelements == 1)
                                        {
                                          if(firstelement->data != element) return false;

                                          DeleteAll();
                                          return true;
                                        }

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          if(data->data == element)
                                            {
                                              data->Delete();

                                              if(data == firstelement)
                                                {
                                                  firstelement = firstelement->next;
                                                  firstelement->prev = 0;
                                                }
                                              else if(data == lastelement)
                                                {
                                                  lastelement = lastelement->prev;
                                                  lastelement->next = 0;
                                                }
                                              else
                                                {
                                                  data->prev->next = data->next;
                                                  data->next->prev = data->prev;
                                                }

                                              GEN_DELETE data;
                                              nelements--;

                                              return true;
                                          }

                                          data = data->next;
                                        }

                                      return false;
                                    }

        bool                        Delete                                    (XLISTDATA<T>* element)
                                    {
                                      if(nelements == 0) return false;

                                      if(nelements == 1)
                                        {
                                          return DeleteAll();
                                        }

                                      if(element == firstelement)
                                        {
                                          firstelement = firstelement->next;
                                          firstelement->prev = 0;
                                          element->Delete();
                                          GEN_DELETE element;
                                          nelements--;
                                          return true;
                                        }

                                      if(element == lastelement)
                                        {
                                          lastelement = lastelement->prev;
                                          lastelement->next = 0;
                                          element->Delete();
                                          GEN_DELETE element;
                                          nelements--;
                                          return true;
                                        }

                                      element->prev->next = element->next;
                                      element->next->prev = element->prev;

                                      element->Delete();
                                      GEN_DELETE element;

                                      nelements--;

                                      return true;
                                    }

    bool                            DeleteLast                                ()
                                    {
                                      if(nelements == 0) return false;

                                      if(nelements == 1)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      XLISTDATA<T>* data = lastelement;
                                      lastelement = data->prev;
                                      lastelement->next = 0;

                                      data->Delete();
                                      nelements--;

                                      GEN_DELETE data;
                                      return true;
                                    }

    bool                            DeleteIndex                               (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return false;
                                        }

                                      if(nelements == 1)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      if(index == 0)
                                        {
                                          XLISTDATA<T>* data = firstelement;

                                          firstelement = data->next;
                                          firstelement->prev = 0;

                                          data->Delete();
                                          GEN_DELETE data;
                                          nelements--;

                                          return true;
                                        }
                                      else if(index == (nelements-1))
                                        {
                                          DeleteLast();

                                          return true;
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      XLISTDATA<T>* prev = data->prev;
                                      XLISTDATA<T>* next = data->next;

                                      prev->next = next;
                                      next->prev = prev;

                                      data->Delete();
                                      GEN_DELETE data;
                                      nelements--;

                                      return true;
                                    }

    bool                            DeleteAll                                 ()
                                    {
                                      XLISTDATA<T>* data = firstelement;
                                      XLISTDATA<T>* datanext;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          datanext = data->next;

                                          data->Delete();
                                          GEN_DELETE data;

                                          data = datanext;
                                        }

                                      firstelement = NULL;
                                      nelements = 0;
                                      return true;
                                    }

    bool                            ResizeContents                            (XDWORD newsize)
                                    {
                                      if(newsize == 0)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      if(newsize >= nelements)
                                        {
                                          return false;
                                        }

                                      XDWORD todelete = nelements - newsize;

                                      XLISTDATA<T>* data = lastelement;
                                      XLISTDATA<T>* previous;
                                      for(XDWORD i = 0; i < todelete; i++)
                                        {
                                          previous = data->prev;

                                          data->Delete();
                                          GEN_DELETE data;

                                          data = previous;
                                        }

                                      lastelement = data;
                                      lastelement->next = 0;

                                      nelements = newsize;
                                      return true;
                                    }

    bool                            DeleteContents                            ()
                                    {
                                      if(nelements == 0)
                                        {
                                          return false;
                                        }

                                      XLISTDATA<T>* current = firstelement;
                                      XLISTDATA<T>* next = firstelement->next;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          next = current->next;

                                          GEN_DELETE current->data;
                                          current->Delete();
                                          GEN_DELETE current;

                                          current = next;
                                        }

                                      firstelement = NULL;
                                      nelements = 0;
                                      return true;
                                    }

    bool                            Swap                                      (XDWORD firstindex, XDWORD secondindex)
                                    {
                                      if(firstindex >= nelements) return false;
                                      if(secondindex >= nelements) return false;

                                      XLISTDATA<T>* first = firstelement;
                                      XLISTDATA<T>* second = firstelement;

                                      if((firstindex + secondindex) > nelements)
                                        {
                                          XLISTDATA<T>* data = firstelement;
                                          for(XDWORD i = 0; i < nelements; i++)
                                            {
                                              if(i == firstindex)
                                                {
                                                  first = data;
                                                }

                                              if(i == secondindex)
                                                {
                                                  second = data;
                                                }

                                              data = data->next;
                                            }
                                        }
                                      else
                                        {
                                          for(XDWORD i = 0; i < firstindex; i++)
                                            {
                                              first = first->next;
                                            }

                                          for(XDWORD i = 0; i < secondindex; i++)
                                            {
                                              second = second->next;
                                            }
                                        }

                                      T s = second->data;
                                      second->data = first->data;
                                      first->data = s;

                                      return true;
                                    }

  typename XLIST::XITERATOR         Begin                                     ()
                                    {
                                      XITERATOR it;
                                      it.data = GetFirstData();
                                      return it;
                                    }

  typename XLIST::XITERATOR         End                                       ()
                                    {
                                      XITERATOR it;
                                      it.data = NULL;
                                      return it;
                                    }

class XITERATOR
{
  friend class XLIST;

  public:

                                    XITERATOR                                 ()
                                    {
                                    }

                                    XITERATOR                                 (typename XLIST::XITERATOR& rhs)
                                    {
                                      *this = rhs;
                                    }

    typename XLIST::XITERATOR &     operator=                                 (const typename XLIST::XITERATOR& rhs)
                                    {
                                      data = rhs.data;
                                      return *this;
                                    }

    bool                            operator!=                                (const typename XLIST::XITERATOR & rhs)
                                    {
                                      return data != rhs.data;
                                    }

    typename XLIST::XITERATOR       operator++                                ()
                                    {
                                      data = data->next;

                                      return *this;
                                    }

    typename XLIST::XITERATOR       operator++                                (int)
                                    {
                                      typename XLIST::XITERATOR it = *this;

                                      data = data->next;

                                      return it;
                                    }

T                                   operator*                                 ()
                                    {
                                      return data->data;
                                    }

  private:

    XLISTDATA<T>*                   data;
};






  protected:

  private:

    void                            Clean                                     ()
                                    {
                                      ismulti         = true;
                                      nelements       = 0;
                                      firstelement    = NULL;
                                      lastelement     = NULL;
                                    }

    bool                            ismulti;
    XDWORD                          nelements;
    XLISTDATA<T>*                   firstelement;
    XLISTDATA<T>*                   lastelement;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/










