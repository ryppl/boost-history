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

#include <boost/stm.hpp>
#include <boost/thread.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost;
using namespace boost::stm;

#define BOOST_STM_NEW(T, P) \
    ((T).throw_if_forced_to_abort_on_new(), \
    (T).as_new(new P))

#define BOOST_STM_NEW_1(P) \
    ((boost::stm::transaction::current_transaction()!=0)?BOOST_STM_NEW(*boost::stm::transaction::current_transaction(), P):new P)

///////////////////////////////////////////////////////////////////////////////
namespace test {
template <typename T>
class list_node
{
public:

   list_node() : value_(), next_() {}
   explicit list_node(T const &rhs) : value_(rhs), next_() {}
   list_node(T const &rhs, tx_ptr<list_node<T> > next) : value_(rhs), next_(next) {}

   // zero initialization for native types
   void clear() { value_ = T(); next_ = 0; }


   T value_;
   tx_ptr<list_node<T> > next_;
};

////////////////////////////////////////////////////////////////////////////
template <typename T>
class list
{
public:

    typedef tx_ptr<list_node<T> > node_type;
    tx_ptr<list_node<T> > head_;
    tx_ptr<std::size_t> size_;
    list() 
    : head_(BOOST_STM_NEW_1(transactional_object<list_node<T> >())) 
    , size_(BOOST_STM_NEW_1(transactional_object<std::size_t>(0))) 
    { }

    ~list() { }

    std::size_t size() {
        std::size_t res=0;
        use_atomic(_) {
            rd_ptr<std::size_t> s(_, size_);
            res=*s;
        }
        return res;
    }

    //--------------------------------------------------------------------------
    // find the location to insert the node. if the value already exists, fail
    //--------------------------------------------------------------------------
    void insert(const T& val) {
        use_atomic(_) {
            upgrd_ptr<list_node<T> > prev(_, head_);
            upgrd_ptr<list_node<T> > curr(_, head_->next_);
            while (curr) {
                if (curr->value_ == val) return;
                else if (curr->value_ > val) break;
                prev = curr;
                curr = curr->next_;
            }        
            if (!curr || (curr->value_ > val)) {
                wr_ptr<list_node<T> > insert_point(_,prev);
                insert_point->next_=BOOST_STM_NEW(_,transactional_object<list_node<T> >(val, curr));
                ++(*size_);
                
            }
        }
   }

    // search function
    bool lookup(const T& val) const {
        bool found = false;
        use_atomic(_) {
            rd_ptr<list_node<T> > curr(_, head_);
            curr = curr->next_;
            while (curr) {
                if (curr->value_ >= val) break;
                curr = curr->next_;
            }

            found = ((curr) && (curr->value_ == val));
        }
        return found;
    }

    // remove a node if its value == val
    void remove(const T& val)
    {
        use_atomic(_) {
            // find the node whose val matches the request
            upgrd_ptr<list_node<T> > prev(_,head_);
            upgrd_ptr<list_node<T> > curr(_,prev->next_);
            while (curr) {
                // if we find the node, disconnect it and end the search
                if (curr->value_ == val) {
                    wr_ptr<list_node<T> > mod_point(_,prev);
                    mod_point->next_=curr->next_;
                    // delete curr...
                    delete_ptr(_,curr);                   
                    --(*size_);
                    break;
                } else if (curr->value_ > val) {
                    // this means the search failed
                    break;
                }
                prev = curr;
                curr = prev->next_;
            }
        }
    }
    
};
}
tx_ptr<test::list<int> > l;

void insert1() {
    thread_initializer thi;
    use_atomic(_) {
        //wr_ptr<test::list<int> > w(_,l);
        //w->insert(1);
        make_wr_ptr(_,l)->insert(1);
    }
}
void insert2() {
    thread_initializer thi;
    use_atomic(_) {
        //wr_ptr<test::list<int> > w(_,l);
        //w->insert(2);
        make_wr_ptr(_,l)->insert(2);
    }
}

void insert3() {
    thread_initializer thi;
    use_atomic(_) {
        //wr_ptr<test::list<int> > w(_,l);
        //w->insert(3);
        make_wr_ptr(_,l)->insert(3);
    }
}
int test1() {
    l=boost::stm::make_tx_ptr<test::list<int> >();
    thread  th1(insert1);
    //thread  th2(insert2);
    //thread  th3(insert2);
    //thread  th4(insert3);

    th1.join(); 
    //th2.join(); 
    //th3.join(); 
    //th4.join(); 
    int res = (l->size()==1?0:1);
    //boost::stm::delete_ptr(l);
    return res;
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;
    srand(time(0));
    
    int res=0;
    res+=test1();
    
    return res;
    
}
