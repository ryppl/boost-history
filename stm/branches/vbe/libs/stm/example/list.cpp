

class BankAccount 
{
protected:
    int nb_;
    int balance_;
public:
    BankAccount(int bnb) : nb_(nb), balance_(0) {}
    BankAccount(const BankAccount &rhs) {
        atomic(_) balance_=rhs.balance_;
    }

    BankAccount& operator=(BankAccount &rhs)
    {
        if(&rhs == this) return *this;

        atomic(_) {
            balance_=rhs.balance_;
            return *this;
        }
    }

    void Deposit(int amount) {
        atomic(_) balance_ += amount;
    }
    int Withdraw(int amount) {
        atomic(_) {
            balance_ -= amount;
            return amount;
        }
    }
    int GetBalance() {
        atomic(_) return balance_;
    }
};

tx_ptr<BankAccount> JoesAccount(new transactional_object<BankAccount>(0));
tx_ptr<BankAccount> A(new transactional_object<BankAccount>(1));
tx_ptr<BankAccount> B(new transactional_object<BankAccount>(2));

void bankAgent()
{
    for (int i =10; i>0; --i) {
        //...
        atomic(_) JoesAccount->Deposit(500);
        //...
    }
}

void Joe() {
    for (int i =10; i>0; --i) {
        //...
        int myPocket;
        atomic(_) {
            myPocket= JoesAccount->Withdraw(100);
        }
        std::cout << myPocket << std::endl;
        //...
    }
}

    atomic(_) {
        *B = *A;
    }
    
    
    
   //////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifndef TEST_LINKED_LIST_H
#define TEST_LINKED_LIST_H

//#define BOOST_STM_USES_AS_NEW 1
//#define BOOST_STM_USES_PARAM 1
#define BOOST_STM_LL_USES_NODE 1

#include "main.h"
#include <boost/stm/transaction.hpp>

#include <fstream>

#define BIG_SIZE 1000
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class list_node
{
public:

   list_node() : value_(0), next_(0) {
       }
   explicit list_node(T const &rhs) : value_(rhs), next_(NULL) {
       }

   // zero initialization for native types
   void clear() { value_ = T(); next_ = NULL; }

   T &value() { return value_; }
   T const &value() const { return value_; }

   list_node const *next() const { return next_->get(); }

   void next(list_node const *rhs, boost::stm::transaction &t)
   {
      if (NULL == rhs) next_ = NULL;
      else { 
          tx_ptr<list_node> tmp(rhs);
          next_ = tmp;
      }
   }

private:

   T value_;
   tx_ptr<list_node> next_;

};

////////////////////////////////////////////////////////////////////////////
template <typename T>
class LinkedList
{
public:

    typedef tx_ptr<list_node<T> > node_type;
    LinkedList() { head_->value() = T(); }

    ~LinkedList() { quick_clear(); }

   void move(node_type const &node1, node_type const &node2)
   {
      using namespace boost::stm;
      bool succeeded1 = true, succeeded2 = true;
      transaction_state state = e_no_state;

      do
      {
         try
         {
            transaction t;
            succeeded1 = internal_remove(node1);
            succeeded2 = internal_insert(node2);
            t.end();
         }
         catch (aborted_transaction_exception&) {}

         if (!succeeded1 || !succeeded2)
         {
            return false; // auto abort of t
         }

      } while (e_committed != state);

      return true;
   }

   ////////////////////////////////////////////////////////////////////////////
   bool insert(list_node<T> const &node)
   {
      atomic(_) { return internal_insert(node, t); }
   }
#ifdef BOOST_STM_USES_PARAM
   bool insert(T val)
#else
   bool insert(const T& val)
#endif
   {
        atomic(_) { return internal_insert(val, t); }
   }
   ////////////////////////////////////////////////////////////////////////////
#ifdef BOOST_STM_USES_PARAM
   bool lookup(T val)
#else
   bool lookup(T const &val)
#endif
   {
      atomic(_) { return internal_lookup(val, t); }
   }

   ////////////////////////////////////////////////////////////////////////////
   bool remove(list_node<T> const &node)
   {
      atomic(_) { return internal_remove(node, t); }
   }

#ifdef BOOST_STM_USES_PARAM
   bool remove(T val)
#else
   bool remove(T const &val)
#endif
   {
      atomic(_) { return internal_remove(val, t); }
   }

   ////////////////////////////////////////////////////////////////////////////
   void outputList(std::ofstream &o)
   {
      int i = 0;
      for (list_node<T> const *cur = head_->next(); cur != NULL; cur = cur->next())
      {
         o << "element [" << i++ << "]: " << cur->value() << std::endl;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   int walk_size()
   {
      int i = 0;
      for (list_node<T> const *cur = head_.next(); cur != NULL; cur = cur->next())
      {
         ++i;
      }

      return i;
   }

   ////////////////////////////////////////////////////////////////////////////
   void quick_clear()
   {
      for (list_node<T> const *cur = head_.next(); cur != NULL;)
      {
         list_node<T> const *prev = cur;
         cur = cur->next();
         delete prev;
      }

      head_.clear();
   }

   ////////////////////////////////////////////////////////////////////////////
   boost::stm::transaction_state clear()
   {
      atomic(_) {

      for (list_node<T> const *cur = head_->next(); cur != NULL;)
      {
         list_node<T> const *prev = cur;
         cur = cur->next();
         prev.delete_memory();
      }

      head_->clear();

      return t.end();
   }

private:

   //--------------------------------------------------------------------------
   // find the location to insert the node. if the value already exists, fail
   //--------------------------------------------------------------------------
   bool internal_insert(node_type n)
   {
      if (0 != head_->next_)
      {
         node_type prev = head_;
         node_type cur = head_->next_;
         T val = n->value_;

         while (true)
         {
            if (cur->value_ == val) return false;
            else if (cur->value_ > val || !cur->next_) break;

            prev = cur;

            node_type curNext = cur->next_;

            if (!curNext) break;

            cur = curNext;
         }

         //--------------------------------------------------------------------
         // if cur->next() is null it means our newNode value is greater than
         // cur, so insert ourselves after cur.
         //--------------------------------------------------------------------
         if (val > cur->value()) cur->next=n;
         //--------------------------------------------------------------------
         // otherwise, we are smaller than cur, so insert between prev and cur
         //--------------------------------------------------------------------
         else
         {
            n->next=cur;
            prev->next_=n;
         }
      }
      else
      {
         head_.next_=n;
      }

      return true;
   }

#ifdef BOOST_STM_USES_PARAM
   bool internal_insert(T val, boost::stm::transaction &t)
#else
   bool internal_insert(const T& val, boost::stm::transaction &t)
#endif
   {
      //T val = valr;
      if (0 != head_->next())
      {
         node_type_ptr const prev = &head_;
         node_type_ptr const cur = head_->next_;

         while (true)
         {
            if (cur->value() == val) return false;
            else if (cur->value() > val || !cur->next()) break;

            prev = cur;

            node_type_ptr curNext = cur->next();

            if (0 == curNext) break;

            cur = curNext;
         }
         t.throw_if_forced_to_abort_on_new();
         node_type_ptr newNode(new transactional_object<list_node<T> >(val)));
         //--------------------------------------------------------------------
         // if cur->next() is null it means our newNode value is greater than
         // cur, so insert ourselves after cur.
         //--------------------------------------------------------------------
         if (val > cur->value()) cur->next_ = newNode;
         //--------------------------------------------------------------------
         // otherwise, we are smaller than cur, so insert between prev and cur
         //--------------------------------------------------------------------
         else
         {
            newNode->next= cur;
            prev->next_=newNode;
         }
      }
      else
      {
         t.throw_if_forced_to_abort_on_new();
         node_type_ptr newNode(new transactional_object<list_node<T> >(val)));
         head_->next_ = newNode;
      }

      t.end();
      return true;
   }
   //--------------------------------------------------------------------------
   // find the location to insert the node. if the value already exists, bail
   //--------------------------------------------------------------------------
#ifdef BOOST_STM_USES_PARAM
   bool internal_lookup(T val, boost::stm::transaction &t)
#else
   bool internal_lookup(T const &val, boost::stm::transaction &t)
#endif
   {
      node_type_ptr cur = head_;

      for (; true ; cur = cur->next() )
      {
         if (cur->value() == val)
         {
            return true;
         }

         if (0 == cur->next()) break;
      }

      return false;
   }

   ////////////////////////////////////////////////////////////////////////////
   bool internal_remove(list_node<T> const &rhs, boost::stm::transaction &t)
   {
      for (rd_ptr<list_node<T> > prev(head_), rd_ptr<list_node<T> > cur(t, prev->next_); 
            0!=cur; 
            prev = cur, cur=cur->next_) {
         if (cur->value_ == rhs.value_) {
            wr_ptr<list_node<T> > wr_prev(t, prev);
            wr_prev->next_=cur->next();
            cur.delete_memory();
            return true;
         }
      }

      return false;
   }

#ifdef BOOST_STM_USES_PARAM
   bool internal_remove(T value, boost::stm::transaction &t)
#else
   bool internal_remove(T const &value, boost::stm::transaction &t)
#endif
   {
      list_node<T> const *prev = &t.read(head_);

      for (list_node<T> const *cur = prev; cur != NULL; prev = cur)
      {
         cur = t.read(*cur).next();

         if (NULL == cur) break;

         if (cur->value() == value)
         {
            list_node<T> const *curNext = t.read(*cur).next();

            t.delete_memory(*cur);
            t.write(*(list_node<T>*)prev).next(curNext, t);
            t.end();
            return true;
         }
      }

      return false;
   }
   //--------------------------------------------------------------------------
   // find the location to insert the node. if the value already exists, bail
   //--------------------------------------------------------------------------
   bool internal_insert(list_node<T> const &rhs)
   {
      using namespace boost::stm;
      transaction t;

      list_node<T> const *headP = &t.read(head_);

      if (NULL != headP->next())
      {
         list_node<T> const *prev = headP;
         list_node<T> const *cur = t.read_ptr(headP->next());
         T val = rhs.value();

         while (true)
         {
            if (cur->value() == val) return false;
            else if (cur->value() > val || !cur->next()) break;

            prev = cur;

            list_node<T> const *curNext = t.read_ptr(cur->next());

            if (NULL == curNext) break;

            cur = curNext;
         }

         list_node<T> *newNode = t.new_memory_copy(rhs);

         //--------------------------------------------------------------------
         // if cur->next() is null it means our newNode value is greater than
         // cur, so insert ourselves after cur.
         //--------------------------------------------------------------------
         if (NULL == cur->next()) t.write_ptr((list_node<T>*)cur)->next_for_new_mem(newNode, t);
         //--------------------------------------------------------------------
         // otherwise, we are smaller than cur, so insert between prev and cur
         //--------------------------------------------------------------------
         else
         {
            newNode->next(cur, t);
            t.write_ptr((list_node<T>*)prev)->next_for_new_mem(newNode, t);
         }
      }
      else
      {
         list_node<T> *newNode = t.new_memory_copy(rhs);
         t.write(head_).next_for_new_mem(newNode, t);
      }

      t.end();
      return true;
   }

   ////////////////////////////////////////////////////////////////////////////
   bool internal_remove(list_node<T> const &rhs)
   {
      using namespace boost::stm;
      transaction t;

      list_node<T> const *prev = &t.read(head_);

      for (list_node<T> const *cur = prev; cur != NULL;
           prev = cur, cur = t.read(*cur).next())
      {
         if (cur->value() == rhs.value())
         {
            t.write(*(list_node<T>*)prev).next(t.read_ptr(cur)->next(), t);
            t.delete_memory(*cur);
            t.end();
            return true;
         }
      }

      return false;
   }

   tx_obj<list_node<T> > head_;
};


#endif // TEST_LINKED_LIST_H
 