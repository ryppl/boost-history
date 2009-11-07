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

   list_node() : value_(), next_() {}
   explicit list_node(T const &rhs) : value_(rhs), next_() {}
   list_node(T const &rhs, tx_ptr<list_node<T> > next) : value_(rhs), next_(next) {}

   // zero initialization for native types
   void clear() { value_ = T(); next_ = 0; }


   T value_;
   tx_ptr<list_node<T> > next_;
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

    typedef tx_ptr<list_node<T> > node_type;
    tx_ptr<list_node<T> > head_;
    tx_ptr<std::size_t> size_;
    list()
    : head_(BOOST_STM_NEW_PTR(transactional_object<list_node<T> >()))
    , size_(BOOST_STM_NEW_PTR(transactional_object<std::size_t>(0)))
    {
            std::cout << "list().head_" << *head_.get() << std::endl;
            std::cout << "list().size_" << *size_.get() << std::endl;
    }

    ~list() { }

    std::size_t size() const {
        std::size_t res=0;
        atomic(_) {
            upgrd_ptr<std::size_t> s(_, size_);
            std::cout << "size_.get()" << s.get() << std::endl;
            res=*s;
        }  end_atom
        return res;
    }

    //--------------------------------------------------------------------------
    // find the location to insert the node. if the value already exists, fail
    //--------------------------------------------------------------------------
    void insert(const T& val) {
        cerr << __LINE__ << " insert" << endl;
        atomic(_) {
        //for (boost::stm::transaction _; !_.committed() && _.restart(); _.end()) {
        cerr << __LINE__ << " insert" << endl;
            upgrd_ptr<list_node<T> > prev(_, head_);
            upgrd_ptr<list_node<T> > curr(_, head_->next_);
            while (curr.get()!=0) {
                cerr << __LINE__ << " curr" << curr.get() << endl;
                if (curr->value_ == val) return;
                else if (curr->value_ > val) break;
                prev = curr;
                curr = curr->next_;
            }
            cerr << __LINE__ << " insert" << endl;
            if (curr.get()==0 || (curr->value_ > val)) {
                cerr << __LINE__ << " inserting" << endl;
                wr_ptr<list_node<T> > insert_point(_,prev);
                cerr << __LINE__ << " inserting" << endl;
                insert_point->next_=BOOST_STM_TX_NEW_PTR(_,transactional_object<list_node<T> >(val, curr));
                cerr << __LINE__ << " inserting" << endl;
                //wr_ptr<std::size_t > size_tx(_,size_);
                //cerr << __LINE__ << " inserting" << endl;
                //++(*size_tx);

            }
        } end_atom
        catch (...) {
        cerr << __LINE__ << " insert" << endl;
        }
        cerr << __LINE__ << " insert" << endl;
   }

    // search function
    bool lookup(const T& val) const {
        bool found = false;
        atomic(_) {
            rd_ptr<list_node<T> > curr(_, head_);
            curr = curr->next_;
            while (curr) {
                if (curr->value_ >= val) break;
                curr = curr->next_;
            }

            found = ((curr) && (curr->value_ == val));
        }  end_atom
        catch (...) {
        cerr << __LINE__ << " lookup" << endl;
        }
        return found;
    }

    // remove a node if its value == val
    void remove(const T& val)
    {
        atomic(_) {
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
                    wr_ptr<std::size_t > size_tx(_,size_);
                    //--(*size_tx);
                    break;
                } else if (curr->value_ > val) {
                    // this means the search failed
                    break;
                }
                prev = curr;
                curr = prev->next_;
            }
        }  end_atom
    }

};
}
//--------------------------------------------------------------------------

tx_ptr<test::list<int> > l;

void create() {
    //l=boost::stm::make_tx_ptr<test::list<int> >();
    atomic(_) {
        cerr << __LINE__ << " create" << endl;
        l=BOOST_STM_TX_NEW_PTR(_,transactional_object<test::list<int> >());
        cerr << __LINE__ << " create" << endl;
        cerr << " create size " << l->size() << endl;
        //cerr << " insert " << l.get() << endl;
        //l->insert(1);
    } end_atom
    catch (...) {
        cerr << "aborted" << endl;
    }
    cerr << " create pointer " << l.get() << endl;
}
void insert1() {
    //thread_initializer thi;
    atomic(_) {
        cerr << __LINE__ << " try" << endl;
        cerr << __LINE__ << " insert1 size " << l->size() << endl;
        wr_ptr<test::list<int> > l_tx(_,l);
        cerr << __LINE__ << " insert1 size " << l_tx->size() << endl;
        int val = 1;
        l_tx->insert(val);
        cerr << __LINE__ << " insert1 size " << l->size() << endl;
    }  end_atom 
    catch(...) {
        cerr << __LINE__ << " aborted" << endl;
    }
}
void insert2() {
    thread_initializer thi;
    atomic(_) {
        make_wr_ptr(_,l)->insert(2);
    } end_atom
}

void insert3() {
    thread_initializer thi;
    atomic(_) {
        make_wr_ptr(_,l)->insert(3);
    } end_atom
}
bool check_size(std::size_t val) {
    int res=true;
    atomic(_) {
        //cerr << "size" <<make_rd_ptr(_,l)->size()<< endl;
        res = make_rd_ptr(_,l)->size()==val;
    } end_atom
    return res;
}
int test1() {
    create();
    insert1();
    bool fails=false;
    //fails= fails || !check_size(0);
    #if 0
    thread  th1(insert1);
    //thread  th2(insert2);
    //thread  th3(insert2);
    //thread  th4(insert3);

    th1.join();
    //th2.join();
    //th3.join();
    //th4.join();
    bool fails=true;
    fails= !check_size(1);
    //boost::stm::delete_ptr(l);
    return 0;
    #else
    return 0;
    #endif
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    int res=0;
    res+=test1();

    return res;

}
