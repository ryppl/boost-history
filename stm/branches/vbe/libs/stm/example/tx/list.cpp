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
        , embeddeds_()
{}
    explicit list_node(T const &rhs) 
        : value_(rhs), next_() {}
    list_node(T const &rhs, tx::pointer<list_node<T> > next) 
        : value_(rhs), next_(next) {}

    // zero initialization for native types
    void clear() { value_ = T(); next_ = 0; }

    std::list<base_transaction_object*>& binds() {return embeddeds_;}
    void bind(base_transaction_object* bto) {embeddeds_.push_back(bto);}

    tx::object<T> value_;
    tx::pointer<list_node<T> > next_;
    std::list<base_transaction_object*> embeddeds_;
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
public:

    typedef tx::pointer<list_node<T> > node_type;
    tx::pointer<list_node<T> > head_;
    tx::uint_t size_;
    list()
    : head_(BOOST_STM_NEW_PTR(list_node<T>()))
    , size_(0)
    {
            std::cout << "list().head=" << head_ << std::endl;
            std::cout << "list().size=" << size_ << std::endl;
    }

    ~list() { }

    std::size_t size() const {
        BOOST_STM_ATOMIC(_) {
            BOOST_STM_RETURN(size_);
        }  BOOST_STM_END_ATOMIC
        return 0;
    }

    //--------------------------------------------------------------------------
    // find the location to insert the node. if the value already exists, fail
    //--------------------------------------------------------------------------
    void insert(const T& val) {
        cerr << __LINE__ << " insert" << endl;
        BOOST_STM_ATOMIC(_) {
        //for (boost::stm::transaction _; !_.committed() && _.restart(); _.end()) {
            cerr << __LINE__ << " insert head=" << head_.value() << endl;
            //tx::pointer<list_node<T> > prev(head_);
            list_node<T>*  prev(head_);
            //tx::pointer<list_node<T> > curr(head_->next_);
            list_node<T> * curr(head_->next_);
            while (curr!=0) {
                cerr << __LINE__ << " curr" << curr << endl;
                if (curr->value_ == val) return;
                else if (curr->value_ > val) break;
                prev = curr;
                curr = curr->next_;
            }
            cerr << __LINE__ << " insert" << endl;
            if (curr==0 || (curr->value_ > val)) {
                tx::pointer<list_node<T> >  mod(prev);
                mod->next_=BOOST_STM_TX_NEW_PTR(_,list_node<T>(val, curr));
                ++size_;
                cerr << __LINE__ << " inserting" << endl;

            }
        } BOOST_STM_END_ATOMIC
        catch (...) {
        cerr << __LINE__ << " insert" << endl;
        }
        cerr << __LINE__ << " insert" << endl;
   }

    // search function
    bool lookup(const T& val) const {
        bool found = false;
        BOOST_STM_ATOMIC(_) {
            tx::pointer<list_node<T> > curr=head_;
            curr = curr->next_;
            while (curr) {
                if (curr->value_ >= val) break;
                curr = curr->next_;
            }

            BOOST_STM_RETURN((curr) && (curr->value_ == val));
        }  BOOST_STM_END_ATOMIC
        catch (...) {
            cerr << __LINE__ << " lookup" << endl;
        }
        return false;
    }

    // remove a node if its value == val
    void remove(const T& val)
    {
        BOOST_STM_ATOMIC(_) {
            // find the node whose val matches the request
            tx::pointer<list_node<T> > prev=head_;
            tx::pointer<list_node<T> > curr=prev->next_;
            while (curr) {
                // if we find the node, disconnect it and end the search
                if (curr->value_ == val) {
                    prev->next_=curr->next_;
                    // delete curr...
                    delete_ptr(_,curr);
                    --size_;
                    break;
                } else if (curr->value_ > val) {
                    // this means the search failed
                    break;
                }
                prev = curr;
                curr = prev->next_;
            }
        }  BOOST_STM_END_ATOMIC
    }

};
}
//--------------------------------------------------------------------------

test::list<int> l;

void create() {
    BOOST_STM_ATOMIC(_) {
        cerr << __LINE__ << " create" << endl;
        cerr << " create size " << l.size() << endl;
    } BOOST_STM_END_ATOMIC
    catch (...) {
        cerr << "aborted" << endl;
    }
}
void insert1() {
    //thread_initializer thi;
    //BOOST_STM_ATOMIC(_) {
        cerr << __LINE__ << " try" << endl;
        cerr << __LINE__ << " insert1 size " << l.size() << endl;
        int val = 1;
        l.insert(val);
        cerr << __LINE__ << " insert1 size " << l.size() << endl;
    //}  BOOST_STM_END_ATOMIC 
    //catch(...) {
    //    cerr << __LINE__ << " aborted" << endl;
    //}
}
void insert2() {
    thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        l.insert(2);
    } BOOST_STM_END_ATOMIC
}

void insert3() {
    thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        l.insert(3);
    } BOOST_STM_END_ATOMIC
}
bool check_size(std::size_t val) {
    BOOST_STM_ATOMIC(_) {
        BOOST_STM_RETURN(l.size()==val);
    } BOOST_STM_END_ATOMIC
    return false;
}
int test_all() {
    //create();
    insert1();
    bool fails=false;
    //fails= fails || !check_size(0);
    #if 0
    thread  th1(insert1);
    thread  th2(insert2);
    thread  th3(insert2);
    thread  th4(insert3);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    bool fails=false;
    //fails= !check_size(1);
    //boost::stm::delete_ptr(l);
    #endif
    SLEEP(2);
    return fails;
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    return test_all();

}
