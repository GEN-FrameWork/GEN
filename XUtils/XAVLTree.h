/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XAVLTree.h
* 
* @class      XAVLTREE
* @brief      eXtended Utils AVL Tree class (https://en.wikipedia.org/wiki/AVL_tree)
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
#include "XList.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <class T>
struct XLESS
{
  public:

    bool                                  operator()                                    (const T& a, const T& b)
                                          {
                                            return a < b;
                                          }

    bool                                  operator()                                    (T& a, T& b)
                                          {
                                            return a < b;
                                          }

};

template <class K>
class XAVLNODE
{
  public:
                                          XAVLNODE                                      (const K& k)
                                          {
                                            Clean();

                                            key     = k;
                                          }

                                          XAVLNODE                                      (XAVLNODE<K> & rhs)
                                          {
                                            Clean();
                                            *this = rhs;
                                          }

    virtual                              ~XAVLNODE                                      ()
                                          {
                                            Clean();
                                          }

    XAVLNODE<K> &                         operator=(XAVLNODE<K>& rhs)
                                          {
                                            key = rhs.key;
                                            height = rhs.height;
                                            left = rhs.left;
                                            right = rhs.right;

                                            return *this;
                                          }


    K                                     key;
    int                                   height;
    int                                   balance;

    XAVLNODE*                             left;
    XAVLNODE*                             right;
    XAVLNODE*                             parent;

  private:

    void                                  Clean                                         ()
                                          {
                                            parent  = NULL;
                                            left    = NULL;
                                            right   = NULL;
                                            balance = 0;
                                            height  = 0;
                                          }
};

template <class K, class Comparator = XLESS<K> >
class XAVLTREE
{
  public:
    class XITERATOR;

                                          XAVLTREE                                      (bool multi = true)
                                          {
                                            Clean();
                                            ismulti = multi;
                                          }

                                          XAVLTREE                                      (XAVLTREE<K, Comparator>& rhs)
                                          {
                                            Clean();
                                            ismulti = rhs.ismulti;
                                            Copy(rhs.GetRoot());
                                          }

    XAVLTREE<K, Comparator>&              operator=                                     (XAVLTREE<K, Comparator>& rhs)
                                          {
                                            if(this == &rhs) return *this;
                                            DeleteNodes(root);
                                            Clean();
                                            ismulti = rhs.ismulti;
                                            Copy(rhs.GetRoot());
                                            return *this;
                                          }

    virtual                              ~XAVLTREE                                      ()
                                          {

                                            DeleteNodes(root);

                                            Clean();
                                          }

    XAVLNODE<K>*                          GetRoot                                       ()                                            { return root; }

    bool                                  Add                                           (const K& key)
                                          {
                                            return Insert(root, key) != NULL;
                                          }

    XAVLNODE<K>*                          Insert                                        (XAVLNODE<K>* i, const K& key)
                                          {
                                            XAVLNODE<K>* node = i;
                                            XAVLNODE<K>* parent = NULL;
                                            XAVLNODE<K>* q = NULL;

                                            bool issmaller = false;
                                            while(node)
                                              {
                                                if((issmaller = islessimp(key, node->key)))
                                                  {
                                                    parent = node;
                                                    node = node->left;
                                                  }
                                                else
                                                  {
                                                    if(!ismulti)
                                                      {

                                                        if(!islessimp(node->key, key))
                                                          {
                                                            return NULL;
                                                          }
                                                      }

                                                    parent = node;
                                                    node = node->right;
                                                  }

                                                if(parent->balance)
                                                  {
                                                    q = parent;
                                                  }
                                              }

                                            XAVLNODE<K>* add = node = GEN_NEW XAVLNODE<K>(key);
                                            if(!node)
                                              {
                                                return NULL;
                                              }
                                            nelements++;

                                            if(!(node->parent = parent))
                                              {
                                                root = node;
                                              }
                                            else
                                              {
                                                if(issmaller)
                                                  {
                                                    parent->left = node;
                                                  }
                                                else
                                                  {
                                                    parent->right = node;
                                                  }

                                                while(parent != q)
                                                  {
                                                    parent->balance = (parent->right == node) * 2 - 1;
                                                    node = parent;
                                                    parent = node->parent;
                                                  }

                                                if(q)
                                                  {
                                                    if(q->left == node)
                                                      {
                                                        if(--q->balance == -2)
                                                          {
                                                            if(q->left->balance > 0)
                                                              {
                                                                RotateLeft(q->left);
                                                              }
                                                            RotateRight(q);
                                                          }
                                                      }
                                                    else
                                                      {
                                                        if(++q->balance == 2)
                                                          {
                                                            if(q->right->balance < 0)
                                                              {
                                                                RotateRight(q->right);
                                                              }
                                                            RotateLeft(q);
                                                          }
                                                      }
                                                  }
                                              }

                                            return add;
                                          }

    K                                     Get                                           (const K& key)
                                          {
                                            if(root != NULL)
                                              {
                                                if((!islessimp(root->key, key)) && (!islessimp(key, root->key)))
                                                  {

                                                    return root->key;
                                                  }

                                                XAVLNODE<K>* n = (root->key < key) ? root->right : root->left;

                                                while(n != NULL)
                                                  {

                                                    if((!islessimp(n->key, key)) && (!islessimp(key, n->key)))
                                                      {

                                                        return n->key;
                                                      }

                                                    n = (islessimp(n->key, key)) ? n->right : n->left;
                                                  }
                                              }

                                            return K();
                                          }

    XLIST<K>                              GetMultiple                                   (const K& key)
                                          {
                                            XLIST<K> list;

                                            GetMultiple(key, list);

                                            return list;
                                          }

    void                                  GetMultiple                                   (const K& key, XLIST<K>& list)
                                          {
                                            FillList(root, key, &list);
                                          }

    int                                   Find                                          (const K& key)
                                          {
                                            XDWORD index = 0;
                                            if(root != NULL)
                                              {
                                                if((!islessimp(root->key, key)) && (!islessimp(key, root->key)))
                                                  {
                                                    return index;
                                                  }

                                                XAVLNODE<K>* n = (islessimp(root->key, key)) ? root->right : root->left;
                                                index++;

                                                while(n != NULL)
                                                  {

                                                    if((!islessimp(n->key, key)) && (!islessimp(key, n->key)))
                                                      {
                                                        return index;
                                                      }

                                                    n = (islessimp(n->key, key)) ? n->right : n->left;
                                                    index++;
                                                  }
                                              }

                                            return NOTFOUND;
                                          }

    bool                                  Delete                                        (const K& key)
                                          {
                                            XAVLNODE<K>* target = FindNode(key);
                                            if(!target) return false;
                                            return DeleteNode(target);
                                          }

    bool                                  Delete(XITERATOR it)
                                          {
                                            return DeleteNode(it.current);
                                          }

    typename XAVLTREE::XITERATOR          Begin                                         ()
                                          {
                                            typename XAVLTREE::XITERATOR it;

                                            it.current = root;

                                            if(root)
                                              {
                                                while(it.current->left != NULL)
                                                {
                                                  it.current->left->parent = it.current;
                                                  it.current = it.current->left;
                                                }
                                              }

                                            return it;
                                          }

    typename XAVLTREE::XITERATOR          End()
                                          {
                                            typename XAVLTREE::XITERATOR it;

                                            it.current = NULL;

                                            return it;
                                          }

    XDWORD                                GetSize                                       ()
                                          {
                                            return nelements;
                                          }

class XITERATOR
{
  public:

    friend class XAVLTREE;

                                          XITERATOR                                     ()
                                          {
                                          }

                                          XITERATOR                                     (typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            *this = rhs;
                                          }

    typename XAVLTREE::XITERATOR &        operator=                                     (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            current = rhs.current;
                                            return *this;
                                          }

    typename XAVLTREE::XITERATOR          operator++                                    ()
                                          {
                                            if(current->right != NULL)
                                              {
                                                current = current->right;

                                                while(current->left != NULL)
                                                  {
                                                    current = current->left;
                                                  }
                                              }
                                            else
                                              {
                                                XAVLNODE<K>* y = current->parent;

                                                if(y == NULL)
                                                  {
                                                    current = NULL;
                                                    return *this;
                                                  }
                                                else
                                                  {
                                                    while(current == y->right)
                                                      {
                                                        current = y;
                                                        y = y->parent;

                                                        if(y == NULL)
                                                          {
                                                            current = NULL;
                                                            return *this;
                                                          }
                                                      }
                                                  }

                                                if(current->right != y)
                                                  {
                                                    current = y;
                                                  }
                                              }

                                            return *this;
                                          }

    typename XAVLTREE::XITERATOR          operator++                                    (int)
                                          {
                                            typename XAVLTREE::XITERATOR it = *this;
                                            ++(*this);

                                            return it;
                                          }

    bool                                  operator==                                    (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            return (current == rhs.current);
                                          }

    bool                                  operator!=                                    (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            return !(*this == rhs);
                                          }

    K                                     operator*                                     ()
                                          {
                                            if(!current) return K();
                                            return current->key;

                                          }

    K                                     GetKey()
                                          {
                                            if(!current) return K();
                                            return current->key;
                                          }

  private:

    XAVLNODE<K>*                          current;
    XAVLNODE<K>*                          root;
};

  protected:

    void                                  FillList                                      (XAVLNODE<K>* n, const K& key, XLIST<K>* list)
                                          {

                                            if(n == NULL)
                                              {
                                                return;
                                              }

                                            FillList(n->left, key, list);

                                            if(islessimp(key, n->key))
                                              {
                                                return;
                                              }
                                            else if(!islessimp(n->key, key))
                                              {

                                                list->Add(n->key);
                                              }

                                            FillList(n->right, key, list);
                                          }

    XAVLNODE<K>*                          FindNode                                      (const K& key)
                                          {
                                            XAVLNODE<K>* node = root;
                                            while(node)
                                              {
                                                if(islessimp(key, node->key)) node = node->left;
                                                else if(islessimp(node->key, key)) node = node->right;
                                                else return node;
                                              }
                                            return NULL;
                                          }

    int                                   RebuildMetadata                               (XAVLNODE<K>* node, XAVLNODE<K>* parent)
                                          {
                                            if(!node) return -1;
                                            node->parent = parent;
                                            int leftheight  = RebuildMetadata(node->left, node);
                                            int rightheight = RebuildMetadata(node->right, node);
                                            node->height  = __MAX(leftheight, rightheight)+1;
                                            node->balance = rightheight-leftheight;
                                            return node->height;
                                          }

    void                                  UpdateMetadata                                (XAVLNODE<K>* node)
                                          {
                                            if(!node) return;
                                            int leftheight  = node->left?node->left->height:-1;
                                            int rightheight = node->right?node->right->height:-1;
                                            node->height  = __MAX(leftheight, rightheight)+1;
                                            node->balance = rightheight-leftheight;
                                          }

    XAVLNODE<K>*                          RotateSubtreeLeft                              (XAVLNODE<K>* node)
                                          {
                                            XAVLNODE<K>* pivot = node->right;
                                            XAVLNODE<K>* parent = node->parent;
                                            node->right = pivot->left;
                                            if(node->right) node->right->parent = node;
                                            pivot->left = node;
                                            node->parent = pivot;
                                            pivot->parent = parent;
                                            UpdateMetadata(node);
                                            UpdateMetadata(pivot);
                                            return pivot;
                                          }

    XAVLNODE<K>*                          RotateSubtreeRight                             (XAVLNODE<K>* node)
                                          {
                                            XAVLNODE<K>* pivot = node->left;
                                            XAVLNODE<K>* parent = node->parent;
                                            node->left = pivot->right;
                                            if(node->left) node->left->parent = node;
                                            pivot->right = node;
                                            node->parent = pivot;
                                            pivot->parent = parent;
                                            UpdateMetadata(node);
                                            UpdateMetadata(pivot);
                                            return pivot;
                                          }

    XAVLNODE<K>*                          RebalanceSubtree                               (XAVLNODE<K>* node)
                                          {
                                            if(!node) return NULL;
                                            UpdateMetadata(node);
                                            if(node->balance < -1)
                                              {
                                                UpdateMetadata(node->left);
                                                if(node->left && node->left->balance > 0)
                                                  {
                                                    node->left = RotateSubtreeLeft(node->left);
                                                    node->left->parent = node;
                                                  }
                                                return RotateSubtreeRight(node);
                                              }
                                            if(node->balance > 1)
                                              {
                                                UpdateMetadata(node->right);
                                                if(node->right && node->right->balance < 0)
                                                  {
                                                    node->right = RotateSubtreeRight(node->right);
                                                    node->right->parent = node;
                                                  }
                                                return RotateSubtreeLeft(node);
                                              }
                                            return node;
                                          }

    XAVLNODE<K>*                          RemoveTarget                                  (XAVLNODE<K>* node, XAVLNODE<K>* target, bool& removed)
                                          {
                                            if(!node) return NULL;
                                            if(node == target)
                                              {
                                                if(!node->left || !node->right)
                                                  {
                                                    XAVLNODE<K>* child = node->left?node->left:node->right;
                                                    if(child) child->parent = node->parent;
                                                    GEN_DELETE node;
                                                    removed = true;
                                                    return child;
                                                  }

                                                XAVLNODE<K>* successor = node->right;
                                                while(successor->left) successor = successor->left;
                                                node->key = successor->key;
                                                node->right = RemoveTarget(node->right, successor, removed);
                                                if(node->right) node->right->parent = node;
                                              }
                                            else
                                              {
                                                node->left = RemoveTarget(node->left, target, removed);
                                                if(node->left) node->left->parent = node;
                                                if(!removed)
                                                  {
                                                    node->right = RemoveTarget(node->right, target, removed);
                                                    if(node->right) node->right->parent = node;
                                                  }
                                              }
                                            return removed?RebalanceSubtree(node):node;
                                          }

    bool                                  DeleteNode                                    (XAVLNODE<K>* target)
                                          {
                                            if(!root || !target) return false;
                                            RebuildMetadata(root, NULL);
                                            bool removed = false;
                                            root = RemoveTarget(root, target, removed);
                                            if(root) root->parent = NULL;
                                            if(removed && nelements) nelements--;
                                            return removed;
                                          }

    bool                                  RotateLeft                                    (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* rlink = n->right;
                                            ImpRotateLeft(n);
                                            bool hc = rlink->balance != 0;
                                            n->balance      -= 1 + __MAX(rlink->balance, 0);
                                            rlink->balance  -= 1 - __MIN(n->balance, 0);
                                            return hc;
                                          }

    bool                                  RotateRight                                   (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* llink = n->left;
                                            ImpRotateRight(n);
                                            bool hc = llink->balance != 0;
                                            n->balance      += 1 - __MIN(llink->balance, 0);
                                            llink->balance  += 1 + __MAX(n->balance, 0);
                                            return hc;
                                          }

    void                                  ImpRotateLeft                                 (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* nr = n->right;
                                            if((n->right = nr->left) != NULL)
                                              {
                                                n->right->parent = n;
                                              }

                                            nr->left = n;

                                            XAVLNODE<K>* p = n->parent;
                                            n->parent = nr;
                                            if((nr->parent = p) != NULL)
                                              {
                                                if(p->left == n)
                                                  {
                                                    p->left = nr;
                                                  }
                                                else
                                                  {
                                                    p->right = nr;
                                                  }
                                              }
                                            else
                                              {
                                                root = nr;
                                              }
                                          }

    void                                  ImpRotateRight                                (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* nl = n->left;

                                            if((n->left = nl->right) != NULL)
                                              {
                                                n->left->parent = n;
                                              }
                                            nl->right = n;

                                            XAVLNODE<K>* p = n->parent;
                                            n->parent = nl;
                                            if((nl->parent = p) != NULL)
                                              {
                                                if(p->left == n)
                                                  {
                                                    p->left = nl;
                                                  }
                                                else
                                                  {
                                                    p->right = nl;
                                                  }
                                              }
                                            else
                                              {
                                                root = nl;
                                              }
                                          }

    int                                   Height                                        (XAVLNODE<K>* n)
                                          {
                                            if(n == NULL)
                                              {
                                                return -1;

                                              }

                                            return n->height;
                                          }

    void                                  FixHeight                                     (XAVLNODE<K>* n)
                                          {
                                            int hl = Height(n->left);
                                            int hr = Height(n->right);

                                            n->height = __MAX(hl, hr) + 1;
                                          }

    void                                  Copy(XAVLNODE<K>*node)
                                          {
                                            if(!node)
                                              {
                                                return;
                                              }

                                            this->Copy(node->left);
                                            this->Add(node->key);
                                            this->Copy(node->right);
                                          }

    int                                   SetBalance                                    (XAVLNODE<K>* n)
                                          {
                                            return Height(n->right) - Height(n->left);
                                          }

    void                                  DeleteNodes                                   (XAVLNODE<K>* n)
                                          {
                                            if(!n)
                                            {
                                              return;
                                            }

                                            DeleteNodes(n->left);
                                            DeleteNodes(n->right);

                                            GEN_DELETE n;
                                          }

    XAVLNODE<K>*                          root;
    Comparator                            islessimp;

    bool                                  ismulti;

    XDWORD                                nelements;

  private:

    void                                  Clean                                         ()
                                          {
                                            root        = NULL;
                                            ismulti     = false;
                                            nelements   = 0;
                                          }
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





