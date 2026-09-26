/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XVector.h
* 
* @class      XVECTOR
* @brief      eXtended Utils Vector template
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

#include "XMemory_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifdef MICROCONTROLLER
  #define XVECTOR_DEFAULTADDINLIMIT   2
#else
  #define XVECTOR_DEFAULTADDINLIMIT   100
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template<class T>
class XVECTOR
{
  public:
                                    XVECTOR                               ()
                                    {
                                      Clean();
                                    }

                                    XVECTOR                               (XDWORD size)
                                    {
                                      Clean();
                                      Resize(size);
                                    }

    virtual                        ~XVECTOR                               ()
                                    {
                                      DeleteAll();
                                      Clean();
                                    }

    bool                            IsEmpty                               ()                                              { return nelements?false:true;                          }
    XDWORD                          GetSize                               ()                                              { return nelements;                                     }

    int                             GetAddInLimit                         ()                                              { return addinlimit;                                    }
    void                            SetAddInLimit                         (int addinlimit = XVECTOR_DEFAULTADDINLIMIT)    { this->addinlimit = addinlimit?addinlimit:1;           }

    void                            SetIsMulti                            (bool ismulti)                                  { this->ismulti = ismulti;                              }

    bool                            Resize                                (XDWORD newsize)
                                    {
                                      XDWORD _nelements = nelements;

                                      if(newsize == _nelements) 
                                        {
                                          return false;
                                        }
                                            
                                      if(newsize > _nelements)
                                        {
                                          for(XDWORD c=0;c<(newsize - _nelements);c++)
                                            {
                                              Add(0);
                                            }
                                        }
                                       else
                                        {
                                          for(XDWORD c=0;c<(_nelements - newsize);c++)
                                            {
                                              DeleteLast();
                                            }
                                        } 
                                     
                                      return true;
                                    }

    virtual bool                    Add                                   (T element)
                                    {
                                      if(!ismulti)
                                        {
                                          for(XDWORD c=0; c<nelements; c++)
                                            {
                                              if(array[c] == element) 
                                                {
                                                  return false;
                                                }
                                            }
                                        }

                                      if(!ResizeAdd()) 
                                        {
                                          return false;
                                        }

                                      array[nelements] = element;

                                      nelements++;

                                      return true;
                                    }

    int                             Find                                  (T element)
                                    {
                                      if(!array) 
                                        {
                                          return NOTFOUND;
                                        }

                                      for(int c=nelements-1; c>=0; c--)
                                        {
                                          if(array[c] == element) 
                                            {
                                              return c;
                                            }
                                        }

                                      return NOTFOUND;
                                    }

    T                               Get                                   (XDWORD index)
                                    {
                                      if(!array) 
                                        {
                                          return T();
                                        }

                                      if(index >= nelements)  
                                        {
                                          return T();
                                        }

                                      return array[index];
                                    }

    T*                              GetPointer                            (XDWORD index)
                                    {
                                      if(!array) 
                                        {
                                          return 0;
                                        }

                                      if(index >= nelements)  
                                        {
                                          return 0;
                                        }

                                      return &array[index];
                                    }

    inline T                        FastGet                               (XDWORD index)
                                    {                                     
                                      return array[index];
                                    }

    T                               GetLast                               ()                                              { return Get(nelements-1);                              }

    bool                            Set                                   (XDWORD index, T element)
                                    {
                                      if(index >= nelements)  
                                        {
                                          return false;
                                        }

                                      array[index] = element;

                                      return true;
                                    }

    bool                            Insert                                (XDWORD index, T element)
                                    {

                                      if(index>nelements) index = nelements;

                                      if(!Add(element))
                                        {
                                          return false;
                                        }

                                      for(XDWORD c=GetSize()-1; c>(XDWORD)index; c--)
                                        {
                                          array[c] = array[c-1];
                                        }

                                      return Set(index, element);
                                    }

    virtual bool                    Delete                                (T element)
                                    {
                                      if(!array) 
                                        {
                                          return false;
                                        }

                                      for(XDWORD c=0;c<nelements;c++)
                                        {
                                          if(array[c] == element)
                                            {
                                              ResizeRemove(c);
                                              nelements--;

                                              return true;
                                            }
                                        }

                                      return false;
                                    }

    bool                            DeleteLast                            ()
                                    {
                                      if(!array) 
                                        {                                          
                                          return false;
                                        }

                                      DeleteIndex(nelements-1);

                                      return true;
                                    }

    virtual bool                    DeleteIndex                           (XDWORD index)
                                    {
                                      if(!array) 
                                        {
                                          return false;
                                        }

                                      if(index >= nelements)  
                                        {
                                          return false;
                                        }

                                      ResizeRemove(index);
                                      nelements--;

                                      return true;
                                    }

    void                            FastDeleteIndex                       (XDWORD index)
                                    {
                                      ResizeRemove(index);
                                      nelements--;
                                    }

    bool                            DeleteAll                             ()
                                    {
                                      if(!array) 
                                        {
                                          return false;
                                        }

                                      GEN_DELETE_ARRAY array;

                                      array      = NULL;
                                      narray     = 0;
                                      nelements  = 0;

                                      return true;
                                    }

    bool                            ResizeContents                        (int newsize)
                                    {
                                      int nsize = (int)GetSize();

                                      if(newsize < nsize)
                                        {
                                          for(int counter=newsize; counter<nsize; counter++)
                                            {
                                              T element = GetLast();
                                              if(element)
                                                {
                                                  DeleteLast();
                                                  GEN_DELETE element;
                                                }
                                            }
                                        }
                                       else
                                        {
                                          for(int counter=nelements; counter<newsize; counter++)
                                            {
                                              ResizeAdd();
                                            }
                                        }
                                               
                                      return true;
                                    }

    bool                            DeleteContentsInstanced()
                                    {
                                      if(!nelements) return false;

                                      for(XDWORD s=0; s<nelements; s++)
                                        {
                                          T element = array[s];
                                          if (element)
                                            {
                                              for(XDWORD c=s+1; c<nelements; c++)
                                                {
                                                  if(array[c] == element) 
                                                    {
                                                      array[c] = NULL;
                                                    }
                                                }

                                              delete(element);
                                              array[s] = NULL;
                                            }

                                        }

                                      nelements = 0;

                                      return true;
                                    }

    bool                            DeleteContents                        ()
                                    {
                                      if(!nelements) 
                                        {
                                          return false;
                                        }

                                      for(XDWORD c=0; c<nelements; c++)
                                        {
                                          T element = array[c];
                                          if(element)
                                            {                                              
                                              delete(element);
                                              array[c] = NULL;
                                            }
                                        }

                                      nelements=0;

                                      return true;
                                    }

    bool                            Swap                                  (XDWORD firstindex, XDWORD secondindex)
                                    {
                                      if(firstindex   >=  nelements)  
                                        {
                                          return false;
                                        }

                                      if(secondindex  >=  nelements)  
                                        {
                                          return false;
                                        }

                                      T element = array[secondindex];

                                      array[secondindex] = array[firstindex];
                                      array[firstindex]  = element;

                                      return true;
                                    }

    bool                            Copy                                  (XVECTOR<T>* origin)
                                    {
                                      if(this == origin)
                                        {
                                          return false;
                                        }

                                      DeleteAll();

                                      nelements         = origin->GetSize();

                                      XDWORD  _narray   = nelements;
                                      int     sizearray = (_narray*sizeof(T));

                                      array = GEN_NEW T[_narray];
                                      if(!array) 
                                        {
                                          return false;
                                        }

                                      memcpy(array, origin->array, sizearray);

                                      return true;
                                    }

    bool                            ResizeAdd                             ()
                                    {

                                      if((nelements >= narray) || (!narray))
                                        {
                                          XDWORD newsize  = nelements + addinlimit;                                       
                                          T*     newarray = GEN_NEW T[newsize];

                                          if(!newarray) 
                                            {
                                              return false;
                                            }

                                          if(array)
                                            {
                                              memcpy(newarray, array, nelements*sizeof(T));
                                              GEN_DELETE_ARRAY array;
                                            }

                                          narray = newsize;
                                          array  = newarray;

                                          if((newsize * sizeof (T)) == 400)
                                            {                                                                                                             
                                              int a=0;
                                              a++;                                                      
                                            }

                                        }

                                      return true;
                                    }

  private:
                                    XVECTOR(const XVECTOR<T> & rhs)
                                    {
                                      Clean();
                                    }

    void                            Clean                                 ()
                                    {
                                      ismulti     = true;
                                      addinlimit  = XVECTOR_DEFAULTADDINLIMIT;
                                      nelements   = 0;
                                      narray      = 0;
                                      array       = NULL;
                                    }

    bool                            ResizeRemove                          (XDWORD index)
                                    {

                                      if(index <= (nelements-1))
                                        {
                                          array[index].~T();

                                          if(index != (nelements-1))
                                            {

                                              XDWORD movedelta = (nelements-1) - index;
                                              memmove(&array[index], &array[index+1], movedelta*sizeof(T));
                                            }

                                          return true;
                                        }

                                      return false;
                                    }

    bool                            ismulti;
    int                             addinlimit;
    XDWORD                          nelements;
    XDWORD                          narray;
    T*                              array;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






