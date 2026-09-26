/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTree.h
* 
* @class      XTREE
* @brief      eXtended Utils Tree template
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

#include "XVector.h"
#include "XStack.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <typename T>  class XTREE;

template <typename T>
class XTREE_NODE
{

  public:
                                      XTREE_NODE              ()
                                      {
                                        Clean();
                                      }

    explicit                          XTREE_NODE              (T data)
                                      {
                                        Clean();
                                        this->data = data;
                                      }

                                      XTREE_NODE              (T data, XTREE_NODE<T>* parent)
                                      {
                                        Clean();

                                        this->data     = data;
                                        this->parent   = parent;
                                      }

    virtual                           ~XTREE_NODE              ()
                                      {
                                        DeleteContents();

                                        Clean();
                                      }

    XTREE_NODE<T>*                    GetParent               ()                                  { return parent;                                            }
    void                              SetParent               (XTREE_NODE<T>* parent)             { this->parent = parent;                                    }

    int                               GetNChildren            ()
                                      {
                                        int size = (int)children.GetSize();
                                        return size;
                                      }

    XVECTOR<XTREE_NODE<T>*>*          GetChildren             ()                                  { return &children;                                         }

    XTREE_NODE<T>*                    GetChild                (int index)                         { return children.Get(index);                               }
    bool                              HasChildren             ()                                  { return (GetNChildren() > 0)?true:false;                   }
    bool                              AddChild                (XTREE_NODE<T>* child)
                                      {
                                        child->SetParent(this);
                                        return children.Add(child);
                                      }

    bool                              InsertChild             (int index, XTREE_NODE<T>* child)
                                      {
                                        child->SetParent(this);
                                        return children.Insert(index, child);
                                      }

    bool                              DelChild                (XTREE_NODE<T>* child)
                                      {
                                        if(!child) return false;

                                        child->DeleteContents();
                                        child->DeleteAll();

                                        GEN_DELETE child;

                                        if(!children.Delete(child)) return false;

                                        return true;
                                      }

    int                               GetLevel                ()
                                      {
                                        XTREE_NODE<T>* _parent = parent;
                                        int   level   = 0;

                                        while(_parent)
                                          {
                                            level++;
                                            _parent = _parent->GetParent();
                                          }

                                        return level;
                                      }

    T&                                GetData                 ()                                  { return data;                                              }

    bool                              DeleteContent           ()
                                      {
                                        if(!data) return false;

                                        GEN_DELETE data;
                                        data = NULL;

                                        return true;
                                      }

    bool                              DeleteContents          ()
                                      {
                                        DeleteContent();

                                        for(XDWORD c=0;c<children.GetSize(); c++)
                                          {
                                            XTREE_NODE<T>* child = children.Get(c);
                                            if(child)
                                              {
                                                child->DeleteContents();
                                              }
                                          }

                                        children.DeleteContents();

                                        return true;
                                      }

    bool                              DeleteAll               ()
                                      {
                                        for(XDWORD c=0;c<children.GetSize(); c++)
                                          {
                                            XTREE_NODE<T>* child = children.Get(c);
                                            if(child)
                                              {
                                                child->DeleteAll();
                                              }
                                          }

                                        return children.DeleteAll();
                                      }

  private:

    void                              Clean                   ()
                                      {
                                        parent = NULL;
                                      }

    XVECTOR<XTREE_NODE<T>*>           children;
    XTREE_NODE<T>*                    parent;
    T                                 data;
};

template <typename T>
class XTREE_NODE_ITERATOR
{
  public:
                                      XTREE_NODE_ITERATOR     (XTREE<T>& tree, XTREE_NODE<T>* start_node)
                                      {
                                        Clean();

                                        this->tree     = &tree;
                                        this->current  = start_node;
                                      }

                                      XTREE_NODE_ITERATOR     (const XTREE_NODE_ITERATOR<T>& iterator)
                                      {
                                        Clean();
                                        (*this) = iterator;
                                      }

    virtual                          ~XTREE_NODE_ITERATOR     ()
                                      {
                                        Clean();
                                      }

    XTREE_NODE_ITERATOR<T>&           operator ++             ()
                                      {

                                        if(current)
                                          {
                                            for( int i = current->GetNChildren()-1; i>=0 ; i--)
                                              {
                                                stack.Push(current->GetChild(i));
                                              }
                                          }

                                        if(!stack.IsEmpty())
                                          {
                                            current = stack.Top();
                                            stack.Pop();
                                          }
                                          else
                                          {
                                            (*this) = tree->End();
                                          }

                                        return (*this);
                                      }

                                      operator bool           ()                              { return (current != NULL);                                 }

    const XTREE_NODE_ITERATOR<T>&     operator =              (const XTREE_NODE_ITERATOR<T>& iterator)
                                      {
                                        this->tree    = iterator.tree;
                                        this->current = iterator.current;

                                        while(!stack.IsEmpty())
                                          {
                                            stack.Pop();
                                          }

                                        XTREE_NODE_ITERATOR<T>&   it        = const_cast <XTREE_NODE_ITERATOR<T>&>(iterator);
                                        int                       stacksize = it.GetStack()->GetSize();
                                        XVECTOR<XTREE_NODE<T>*>   vtree(stacksize);

                                        while(!it.stack.IsEmpty())
                                          {
                                            vtree.Add(it.stack.Top());
                                            it.stack.Pop();
                                          }

                                        for(int i=vtree.GetSize()-1; i>=0; i--)
                                          {
                                            it.stack.Push(vtree.Get(i));
                                            stack.Push(vtree.Get(i));
                                          }

                                        return (*this);
                                      }

    bool                              operator ==             (const XTREE_NODE_ITERATOR<T>& iterator) const
                                      {

                                        if ((current == iterator.current) && (tree == iterator.tree)  )
                                                return true;
                                          else return false;
                                      }

    bool                              operator !=             (const XTREE_NODE_ITERATOR<T>& iterator) const
                                      {
                                        return (! operator == (iterator) );
                                      }

    T*                                operator ->             ()                              { return current?&current->GetData():NULL;                  }
    T&                                operator *              ()                              { return current->GetData();                                }

    XSTACK<XTREE_NODE<T>*>*           GetStack                ()                              { return &stack;                                            }
    XTREE_NODE<T>*                    GetCurrent              ()                              { return (current);                                         }

  private:

    void                              Clean                   ()
                                      {
                                        current = NULL;
                                        tree    = NULL;
                                      }

    XSTACK<XTREE_NODE<T>*>            stack;
    XTREE_NODE<T>*                    current;
    XTREE<T>*                         tree;

};

template <typename T>
class XTREE
{
  public:

    explicit                          XTREE                   (XTREE_NODE<T>* root)
                                      {
                                        Clean();

                                        this->root = root;
                                      }

                                      XTREE                   ()
                                      {
                                        Clean();

                                        root = NULL;
                                      }

    virtual                          ~XTREE                   ()
                                      {
                                        if(root) GEN_DELETE root;

                                        Clean();
                                      }

    XTREE_NODE<T>*                    GetRoot                 ()
                                      {
                                        return root;
                                      }

    void                              SetRoot                 (XTREE_NODE<T>* root)
                                      {
                                        if(this->root)  GEN_DELETE this->root;
                                        this->root = root;
                                      }

    XDWORD                            GetNNodes               ()
                                      {
                                        if(!root)  return 0;
                                        return GetNNodes(root);
                                      }

    XDWORD                            GetNNodes               (XTREE_NODE<T>* node)
                                      {
                                        XDWORD nchildren = 0;
                                        if(!node) return nchildren;

                                        nchildren++;

                                        for(int c=0; c<node->GetNChildren(); c++)
                                          {
                                            XTREE_NODE<T>* subnode = node->GetChild(c);
                                            if(subnode) nchildren += GetNNodes(subnode);
                                          }

                                        return nchildren;
                                      }

    XTREE_NODE_ITERATOR<T>            Begin                   ()                                  { return XTREE_NODE_ITERATOR<T>(*this, root);                  }
    XTREE_NODE_ITERATOR<T>            End                     ()                                  { return XTREE_NODE_ITERATOR<T>(*this, NULL);                  }

    bool                              DeleteContents          ()
                                      {
                                        if(!root) return false;
                                        return root->DeleteContents();
                                      }

    bool                              DeleteAll               ()
                                      {
                                        if(!root) return false;
                                        return root->DeleteAll();
                                      }

  private:


    void                              Clean                   ()
                                      {
                                        root = NULL;
                                      }

    XTREE_NODE<T>*                    root;

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





