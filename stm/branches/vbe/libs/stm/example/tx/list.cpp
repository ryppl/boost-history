//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm.hpp>
#include <boost/thread.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost;
using namespace boost::stm;

//--------------------------------------------------------------------------

namespace test {
template <typename T>
class list_node
{
public:

    list_node() : value_(), next_()
        , binds_()
    {
        bind(&value_);
        bind(&next_);
    }
    explicit list_node(T const &rhs)
        : value_(rhs), next_() {}
    list_node(T const &rhs, list_node<T>* next)
        : value_(rhs), next_(next) {}

    // zero initialization for native types
    void clear() { value_ = T(); next_ = 0; }

    tx::object<T> value_;
    tx::pointer<list_node<T> > next_;

    std::list<base_transaction_object*>& binds() {return binds_;}
    std::list<base_transaction_object*> binds_;

    void bind(base_transaction_object* bto) {binds_.push_back(bto);}
};

template <typename OSTREAM, typename T>
OSTREAM& operator<<(OSTREAM& os, list_node<T>& v) {
    os << v.value_;
    return os;
}

//--------------------------------------------------------------------------
template <typename T>
class list
{
    typedef tx::pointer<list_node<T> > node_type;
    tx::pointer<list_node<T> > head_;
    tx::uint_t size_;

    std::list<base_transaction_object*>& binds() {return binds_;}
    std::list<base_transaction_object*> binds_;

    void bind(base_transaction_object* bto) {binds_.push_back(bto);}

public:
    list()
    : head_(BOOST_STM_NEW_PTR(list_node<T>()))
    , size_(0)
    , binds_()
    {
        bind(&head_);
        bind(&size_);
    }

    ~list() { }

    std::size_t size() const {
        BOOST_STM_E_TRANSACTION {
            BOOST_STM_E_RETURN(size_);
        }  BOOST_STM_E_END_TRANSACTION;
        //~ BOOST_STM_TRANSACTION(_) {
            //~ BOOST_STM_RETURN(_, size_);
        //~ }  BOOST_STM_RETRY
        BOOST_STM_E_TRANSACTION {
            BOOST_STM_E_RETURN(size_);
        }  BOOST_STM_E_END_TRANSACTION;
        //~ BOOST_STM_TRANSACTION(_) {
            //~ BOOST_STM_RETURN(_, size_);
        //~ }  BOOST_STM_RETRY
        return 0;
    }

    //--------------------------------------------------------------------------
    // find the location to insert the node. if the value already exists, fail
    //--------------------------------------------------------------------------
    void insert_e(const T& val) {
        BOOST_STM_TRANSACTION(_) {
            list_node<T> * prev = head_;
            list_node<T> * curr = prev->next_;
            while (curr!=0) {
                if (curr->value_ == val) BOOST_STM_TX_RETURN_NOTHING(_);
                //~ if (curr->value_ == val) return;
                else if (curr->value_ > val) BOOST_STM_BREAK(_);
                prev = curr;
                curr = curr->next_;
            }
            if (curr==0 || (curr->value_ > val)) {
                prev->next_=BOOST_STM_TX_NEW_PTR(_,list_node<T>(val, curr));
                ++size_;
            }
        } BOOST_STM_RETRY
   }

    void insert(const T& val) {
        BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO insert " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }
            list_node<T> * prev = head_;
            list_node<T> * curr = prev->next_;
            while (curr!=0) {
                //~ if (curr->value_ == val) {
                    //~ BOOST_STM_E_RETURN_NOTHING;
                //~ }
                if (curr->value_ == val) return;
                else if (curr->value_ > val) break;
                prev = curr;
                curr = curr->next_;
            }
            if (curr==0 || (curr->value_ > val)) {
                prev->next_=BOOST_STM_E_NEW_PTR(list_node<T>(val, curr));
                ++size_;
            }
        } BOOST_STM_E_END_TRANSACTION;
   }

    // search function
    bool lookup(const T& val) const {
        BOOST_STM_E_TRANSACTION {
            list_node<T> const * curr=head_->next_;
            while (curr) {
                if (curr->value_ >= val) break;
                curr = curr->next_;
            }
            BOOST_STM_E_RETURN((curr) && (curr->value_ == val));
        }  BOOST_STM_E_END_TRANSACTION;
        return false;
    }

    // remove a node if its value == val
    void remove(const T& val)
    {
        BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO remove " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }
            // find the node whose val matches the request
            list_node<T> * prev=head_;
            list_node<T> * curr=prev->next_;
            while (curr) {
                // if we find the node, disconnect it and end the search
                if (curr->value_ == val) {
                    prev->next_=curr->next_;
                    // delete curr...
                    BOOST_STM_E_DELETE_PTR(curr);
                    --size_;
                    break;
                } else if (curr->value_ > val) {
                    cerr << __LINE__ << " * remove not found" << endl;
                    break;
                }
                prev = curr;
                curr = prev->next_;
            }
        }  BOOST_STM_E_END_TRANSACTION;
    }

};
}
//--------------------------------------------------------------------------

test::list<int> l;
test::list_node<int> n;

void create() {
    BOOST_STM_OUTER_TRANSACTION(_) {
        cerr << __LINE__ << " create" << endl;
        cerr << " create size " << l.size() << endl;
    } BOOST_STM_RETRY
    catch (...) {
                std::cout << "*** ERROR: "<< __FILE__ << "["<<__LINE__<<"] catch" << std::endl;
    }
}
bool check_size(std::size_t val) {
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(l.size()==val);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}
bool check_lookup(int val) {
    BOOST_STM_E_TRANSACTION {
        //cerr << " check_lookup " << l.lookup(val) << endl;
        BOOST_STM_E_RETURN(l.lookup(val));
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool insert(int val) {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        l.insert(val);
    }  BOOST_STM_RETRY
    return check_size(1);
}
void insert1_th() {
    thread_initializer thi;
    try {
        BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO insert1_th " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }

            l.insert(1);
        }  BOOST_STM_E_END_TRANSACTION;
    } 
    CATCH_AND_PRINT_ALL
}
void insert2_th() {
    thread_initializer thi;
    try {
    //~ BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO insert2_th " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }
            l.insert(2);
        } BOOST_STM_E_END_TRANSACTION;
    //~ } BOOST_STM_RETRY
    } 
    CATCH_AND_PRINT_ALL
}
void insert3_th() {
    thread_initializer thi;
    try {
    //~ BOOST_STM_OUTER_TRANSACTION(_) {
    BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO insert3_th " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }
        l.insert(3);
    } BOOST_STM_E_END_TRANSACTION;
    //~ } BOOST_STM_RETRY
    } 
    CATCH_AND_PRINT_ALL
}

bool remove(int val) {
    //thread_initializer thi;
    BOOST_STM_E_TRANSACTION {
            {boost::lock_guard<boost::mutex> lk(log_mutex);
            std::cout << &BOOST_STM_CURRENT << " " << BOOST_STM_CURRENT.parent() << " " << boost::this_thread::get_id() << " $$$$$INFO remove " << __FILE__ << "[" << __LINE__ << "]" << std::endl;
            }
        l.remove(val);
    } BOOST_STM_E_END_TRANSACTION;
    return true;
}

bool n1() {
    BOOST_STM_TRANSACTION(_) {
        int val = 10;
        n.next_=BOOST_STM_TX_NEW_PTR(_,test::list_node<int>(val, 0));
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(n.next_->value_==10);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool n2() {
    BOOST_STM_TRANSACTION(_) {
        n.next_->value_=12;
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(n.next_->value_==12);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool n3() {
    BOOST_STM_TRANSACTION(_) {
        test::list_node<int>* prev =&n;
        test::list_node<int>* curr =prev->next_;
        int val = 10;
        prev->next_=BOOST_STM_TX_NEW_PTR(_,test::list_node<int>(val, curr));
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(n.next_->value_==10);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool test_par() {
    bool fails=false;
    thread  th1(insert1_th);
    thread  th2(insert2_th);
    thread  th3(insert2_th);
    thread  th4(insert3_th);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    fails= fails || !check_lookup(1);
    fails= fails || !check_lookup(2);
    fails= fails || !check_lookup(3);
    fails= fails || !check_size(3);
    fails= fails || !remove(2);
    fails= fails || !check_lookup(1);
    fails= fails || check_lookup(2);
    fails= fails || !check_lookup(3);
    fails= fails || !check_size(2);

    return !fails;
}

int test_all() {

    //create();
    bool fails=false;
    fails= fails || !n1();
    fails= fails || !n2();
    fails= fails || !n3();
    fails= fails || !check_size(0);
    fails= fails || !insert(1);
    fails= fails || !remove(1);
    fails= fails || !check_size(0);
    fails= fails || !remove(2);
    fails= fails || !check_size(0);

    fails= fails || !test_par();

    return fails;
}

void term_hd() {
    std::cout << "****************** ERROR: "<< __FILE__ << "["<<__LINE__<<"] term_hd"<< std::endl;
    std::abort();
}
int main() {
    try {
    std::set_terminate(term_hd);
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;


    return test_all();
    } 
    CATCH_AND_PRINT_ALL

}
