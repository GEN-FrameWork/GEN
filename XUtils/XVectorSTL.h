/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XVectorSTL.h
* 
* @class      XVECTORSTL
* @brief      eXtended Utils Vector STL class
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
#include "XSTL.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <class T>
class XVECTORSTL : public std::vector<T>
{
  public:
                                    XVECTORSTL                            ()
                                    {
                                      Clean();
                                    }

                                    XVECTORSTL                            (XDWORD size)
                                    {
                                      Clean();
                                      Resize(size);
                                    }

    virtual                        ~XVECTORSTL                            ()
                                    {                                      
                                      Clean();
                                    }

    bool                            IsEmpty                               ()                                              
                                    { 
                                      return this->empty();                          
                                    }

    XDWORD                          GetSize                               ()                                              
                                    { 
                                      return (XDWORD)this->size();                                     
                                    }

    bool                            Resize                                (XDWORD newsize)
                                    {                                      
                                      return this->resize(newsize);
                                    }

    virtual bool                    Add                                   (T element)
                                    {
                                      this->push_back(element);                                      

                                      return true;
                                    }

    virtual bool                    Add                                   (T& element)
                                    {
                                      this->push_back(element);                                      

                                      return true;
                                    }

    int                             Find                                  (T element)
                                    {
                                       
                                      return NOTFOUND;
                                    }

    T                               Get                                   (XDWORD index)
                                    {                                       
                                      return this->at(index);
                                    }

    T                               GetLast                               ()                                              
                                    { 
                                      return this->back();                              
                                    }

    bool                            Set                                   (XDWORD index, T element)
                                    {                                    
                                      return true;
                                    }

    bool                            Insert                                (XDWORD index, T element)
                                    {                                      
                                      return true;
                                    }

    virtual bool                    Delete                                (T element)
                                    {
                                      for(XDWORD c=0; c<this->GetSize(); c++)
                                        {
                                          if(this->at(c) == element)
                                            {
                                              return DeleteIndex(c);
                                            }
                                        }

                                      return false;
                                    }

    bool                            DeleteLast                            ()
                                    {
                                      if(!this->GetSize()) return false;

                                      Delete(GetLast());

                                      return true;
                                    }

    virtual bool                    DeleteIndex                           (XDWORD index)
                                    {      
                                      if(index >= this->size()) return false;

                                      this->erase(this->begin() + index);

                                      return true;
                                    }
    
    bool                            DeleteContents                        ()
                                    {
                                      if(this->empty()) return false;

                                      for(XDWORD c=0; c<this->GetSize(); c++)
                                        {
                                          T element = this->at(c);
                                          if(element) delete(element);
                                        }

                                      this->clear();

                                      return true;
                                    }

    bool                            DeleteAll                             ()
                                    {
                                      this->clear();

                                      return true;
                                    }

    bool                            Swap                                  (XDWORD firstindex, XDWORD secondindex)
                                    {

                                      return true;
                                    }


    bool                            Copy                                  (XVECTORSTL<T>* origin)
                                    {

                                      return true;
                                    }

    

  private:
                                    XVECTORSTL                            (const XVECTORSTL<T> & rhs)
                                    {
                                      Clean();
                                    }


    void                            Clean                                 ()
                                    {

                                    }
    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






