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

using namespace std;
using namespace boost;
using namespace boost::stm;


class Base 
    : public transaction_object<Base> 
{
public:
    Base() : derived_called(false) {}
    void mod_fct() {}
    bool derived_called;
};

class Derived 
    : public Base 
{
public:
    static Derived* make_cache(Derived const& rhs, transaction& t) {
        return new Derived(rhs);
    };
    virtual base_transaction_object* make_cache(transaction& t) const {
        return make_cache(*this, t);
    }
    virtual void delete_cache() {
        delete this;
    }
    virtual void copy_cache(base_transaction_object const &rhs)
    {
       *this = *static_cast<Derived const *>(&rhs);
    }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      *this = draco_move(*(static_cast<Derived*>(rhs)));
   }
#endif
    Derived() : Base(), derived_mofified(false) {}
    bool derived_mofified;
    void mod_fct() {
        derived_called=true;
    }
};

class Derived2 : public transaction_object<Derived2, Base> {
public:
    Derived2() : transaction_object<Derived2, Base>(), derived_mofified(false) {}
    bool derived_mofified;
    void mod_fct() {
        derived_called=true;
    }
};    

  
bool test1_KO() {
    Base* ptrB = new Derived();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr(ptrB)->mod_fct();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, dynamic_cast<Derived const*>(_.read_ptr(ptrB))->derived_mofified==false) ;
    } BOOST_STM_RETRY
    return false;    
}  


bool test2_KO() {
    Base* ptrB = new Derived2();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr(ptrB)->mod_fct();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, dynamic_cast<Derived2 const*>(_.read_ptr(ptrB))->derived_mofified==false) ;
        //BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB)->derived_called==false) ;
    } BOOST_STM_RETRY
    return false;    
}  

bool test1_OK() {
    Base* ptrB = new Derived();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr_dyn(ptrB)->mod_fct();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, dynamic_cast<Derived const*>(_.read_ptr(ptrB))->derived_mofified==true) ;
        //BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB)->derived_called==true) ;
    } BOOST_STM_RETRY
    return false;    
}  


bool test2_OK() {
    Base* ptrB = new Derived2();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr_dyn(ptrB)->mod_fct();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, dynamic_cast<Derived2 const*>(_.read_ptr(ptrB))->derived_mofified==true) ;
        //BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB)->derived_called==true) ;
    } BOOST_STM_RETRY
    return false;    
}  

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    int res=0;
    res+=test1_OK();
    //res+=test1_KO();
    res+=test2_OK();
    //res+=test2_KO();
    return res;

}
