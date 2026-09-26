/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XMap.h
* 
* @class      XMAP
* @brief      eXtended Utils MAP class
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
#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifdef MICROCONTROLLER
  #define XMAP_ADDINLIMIT           2
#else
  #define XMAP_ADDINLIMIT           10
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template<class KEY, class ELEMENT>
class XMAP
{
  public:
                                    XMAP                                  ()
                                    {
                                      keys.SetIsMulti(false);
                                      elements.SetIsMulti(true);
                                    }

    virtual                        ~XMAP                                  ()                                              {                                       }

    bool                            IsEmpty                               ()                                              { return keys.GetSize()?false:true;     }
    XDWORD                          GetSize                               ()                                              { return keys.GetSize();                }

    int                             GetAddInLimit                         ()                                              { return keys.GetAddInLimit();          }
    bool                            SetAddInLimit                         (int addinlimit = XMAP_ADDINLIMIT)
                                    {
                                      keys.SetAddInLimit(addinlimit);
                                      elements.SetAddInLimit(addinlimit);

                                      return true;
                                    }

    void                            SetIsMulti                            (bool ismulti)
                                    {
                                      keys.SetIsMulti(ismulti);
                                      elements.SetIsMulti(ismulti);
                                    }

    int                             CountKey                              (KEY key)
                                    {
                                      int count = 0;

                                      for(XDWORD c=0; c<keys.GetSize(); c++)
                                        {
                                          if(key==keys.Get(c))                                             
                                            { 
                                              count++;
                                            }
                                        }

                                      return count;
                                    }

    bool                            Add                                   (KEY key, ELEMENT element)
                                    {
                                      if(!keys.Add(key)) 
                                        {
                                          return false;
                                        }

                                      bool status = elements.Add(element);
                                      if(!status) 
                                        {
                                          keys.Delete(key);
                                        }

                                      return status;
                                    }

    int                             Find                                  (KEY key)                                       { return keys.Find(key);                }

    KEY                             GetKey                                (int index)                                     { return keys.Get(index);               }
    KEY                             FastGetKey                            (int index)                                     { return keys.FastGet(index);           }
    ELEMENT                         GetElement                            (int index)                                     { return elements.Get(index);           }
    ELEMENT                         FastGetElement                        (int index)                                     { return elements.FastGet(index);       }

    ELEMENT                         Get                                   (KEY key)
                                    {
                                      int position = NOTFOUND;

                                      if((position = keys.Find(key)) != NOTFOUND)
                                        {
                                          return elements.FastGet(position);
                                        }

                                      return (ELEMENT)0;
                                    }

    bool                            Set                                   (KEY key, ELEMENT element)
                                    {
                                      int index=keys.Find(key);

                                      if(index != NOTFOUND)
                                        {
                                          return elements.Set(index, element);
                                        }

                                      return false;
                                    }

    bool                            Insert                                (int index, KEY key, ELEMENT element)
                                    {
                                      if(!keys.Insert(index, key))
                                        { 
                                          return false;
                                        }

                                      return elements.Insert(index, element);
                                    }

    bool                            Delete                                (KEY key)
                                    {
                                      int index = keys.Find(key);
                                      if(index == NOTFOUND) 
                                        {
                                          return false;
                                        }

                                      ELEMENT element = elements.Get(index);

                                      if(!keys.Delete(key)) 
                                        {
                                          return false;
                                        }

                                      return elements.Delete(element);
                                    }

    bool                            Delete                                (KEY key, ELEMENT element)
                                    {
                                      bool   status = false;
                                      XDWORD c      = 0;

                                      while(c<keys.GetSize())
                                        {
                                          KEY     _key     = keys.Get(c);
                                          ELEMENT _element = elements.Get(c);

                                          if((_key == key) && (_element == element))
                                            {
                                              keys.DeleteIndex(c);
                                              elements.DeleteIndex(c);

                                              status = true;

                                            } else c++;
                                        }

                                      return status;
                                    }

    bool                            DeleteAll                             ()
                                    {
                                      if(!keys.DeleteAll()) 
                                        {
                                          return false;
                                        }

                                      return elements.DeleteAll();
                                    }


    bool                            DeleteKeyContents                     ()                                              { return keys.DeleteContents();     }
    bool                            DeleteElementContents                 ()                                              { return elements.DeleteContents(); }


    XVECTOR<KEY>                    keys;
    XVECTOR<ELEMENT>                elements;

    ELEMENT                         operator[](KEY k)
                                    {
                                      return this->Get(k);
                                    }

  private:

    bool                            CreateArraySize                       (int nelements)
                                    {
                                      if(!keys.CreateArraySize(nelements)) 
                                        {
                                          return false;
                                        }

                                      return elements.CreateArraySize(nelements);
                                    }
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





