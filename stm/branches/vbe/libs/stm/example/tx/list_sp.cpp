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
class list_node : public transaction_object<list_node<T> >
{
public:

    list_node() : value_(), next_()
    {
    }
    explicit list_node(T const &rhs)
        : value_(rhs), next_(0) {}
    list_node(T const &rhs, list_node<T>* next)
        : value_(rhs), next_(next) {}

    // zero initialization for native types
    void clear() { value_ = T(); next_ = 0; }

    T value_;
    list_node<T> * next_;

};

template <typename OSTREAM, typename T>
OSTREAM& operator<<(OSTREAM& os, list_node<T>& v) {
    os << v.value_;
    return os;
}

//--------------------------------------------------------------------------
template <typename T>
class list : public transaction_object<list<T> >
{
    list_node<T> head_storage;
    list_node<T>* head_;
    std::size_t size_;

public:
    list()
    : head_(BOOST_STM_NEW_PTR(list_node<T>()))
    , size_(0)
    {}

    ~list() {}

    std::size_t size() const {
        BOOST_STM_E_TRANSACTION {
            BOOST_STM_E_RETURN(size_);
        }  BOOST_STM_E_END_TRANSACTION;
        return 0;
    }

    //--------------------------------------------------------------------------
    // find the location to insert the node. if the value already exists, fail
    //--------------------------------------------------------------------------
    void insert(const T& val) {
        BOOST_STM_E_TRANSACTION {
            read_ptr<list_node<T> > prev(BOOST_STM_CURRENT, head_);
            read_ptr<list_node<T> > curr(BOOST_STM_CURRENT, prev->next_);
            while (curr!=0) {
                if (curr->value_ == val)  {
        std::cout << __FILE__ << "["<<__LINE__<<"] return" << std::endl;\
                    BOOST_STM_E_RETURN_NOTHING;
                }
                else if (curr->value_ > val) break;
                prev = curr;
                curr = curr->next_;
            }
            if (curr==0 || (curr->value_ > val)) {
                make_write_ptr<static_poly>(prev)->next_=BOOST_STM_E_NEW_PTR(list_node<T>(val, curr.get()));
                ++(make_write_ptr<static_poly>(BOOST_STM_CURRENT, this)->size_);
            }
        } BOOST_STM_E_END_TRANSACTION;
    }

    // search function
    bool lookup(const T& val) const {
        BOOST_STM_E_TRANSACTION {
            read_ptr<list_node<T> > curr(BOOST_STM_CURRENT, head_->next_);
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
            // find the node whose val matches the request
            read_ptr<list_node<T> > prev(BOOST_STM_CURRENT, head_);
            read_ptr<list_node<T> > curr(BOOST_STM_CURRENT, prev->next_);
            while (curr) {
                // if we find the node, disconnect it and end the search
                if (curr->value_ == val) {
                    make_write_ptr<static_poly>(prev)->next_=curr->next_;
                    // delete curr...
                    BOOST_STM_E_DELETE_PTR(curr);
                    //--size_;
                    --(make_write_ptr<static_poly>(BOOST_STM_CURRENT, this)->size_);
                    //write_ptr<list<T> > that(_, this);
                    //++(that->size_);
                    break;
                } else if (curr->value_ > val) {
                    cout << __LINE__ << " * remove not found" << endl;
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
    BOOST_STM_E_TRANSACTION {
        cout << __LINE__ << " create" << endl;
        cout << " create size " << l.size() << endl;
    } BOOST_STM_E_END_TRANSACTION;
}
bool check_size(std::size_t val) {
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(l.size()==val);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}
bool check_lookup(int val) {
    //BOOST_STM_TRANSACTION(_) {
        return (l.lookup(val));
    //} BOOST_STM_RETRY
    //~ return false;
}

bool insert1() {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        int val = 10;
        l.insert(val);
    }  BOOST_STM_RETRY
    return check_size(1);
}
void insert1_th() {
    thread_initializer thi;
    try {
        BOOST_STM_E_TRANSACTION {
            l.insert(1);
        }  BOOST_STM_E_END_TRANSACTION;
    }
    CATCH_AND_PRINT_ALL
}
void insert2_th() {
    thread_initializer thi;
    try {
        BOOST_STM_E_TRANSACTION {
            l.insert(2);
        } BOOST_STM_E_END_TRANSACTION;
    }
    CATCH_AND_PRINT_ALL
}

void remove2() {
    //thread_initializer thi;
    BOOST_STM_E_TRANSACTION {
        l.remove(2);
    } BOOST_STM_E_END_TRANSACTION;
}

void insert3_th() {
    thread_initializer thi;
    try {
        BOOST_STM_E_TRANSACTION {
            l.insert(3);
        } BOOST_STM_E_END_TRANSACTION;
    }
    CATCH_AND_PRINT_ALL
}
bool n1() {
    BOOST_STM_E_TRANSACTION {
        int val = 10;
        n.next_=BOOST_STM_E_NEW_PTR(test::list_node<int>(val, 0));
    } BOOST_STM_E_END_TRANSACTION;
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN (n.next_->value_==10);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool n2() {
    BOOST_STM_TRANSACTION(_) {
        n.next_->value_=12;
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN (n.next_->value_==12);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool n3() {
        //cout << __LINE__ << " * n3" << endl;
    BOOST_STM_TRANSACTION(_) {
        test::list_node<int>* prev =&n;
        test::list_node<int>* curr =prev->next_;
        int val = 10;
        prev->next_=BOOST_STM_TX_NEW_PTR(_,test::list_node<int>(val, curr));
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN (n.next_->value_==10);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

int test_all() {
    //create();
    bool fails=false;
    fails= fails || !n1();
    fails= fails || !n2();
    fails= fails || !n3();
    fails= fails || !check_size(0);
    //fails= fails || !insert1();
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
    remove2();
    fails= fails || !check_lookup(1);
    fails= fails || check_lookup(2);
    fails= fails || !check_lookup(3);
    fails= fails || !check_size(2);
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
