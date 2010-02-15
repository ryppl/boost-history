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
namespace boost { namespace stm {
    
template <class B>
struct virtually2 : virtual B {
    // forward constructors
};    
}}

using namespace std;
using namespace boost;
using namespace boost::stm;

struct B1 : transaction_object<B1,
       virtually<base_transaction_object> > 
{
    int i_;
    B1() : i_(0), derived_called1(false) {}
    B1(int i) : i_(i), derived_called1(false) {}
    void mod_fct1() {}
    bool derived_called1;
};

struct B2 : transaction_object<B2, 
       virtually<base_transaction_object> > 
{
    int j_;
    B2() : j_(0), derived_called2(false) {}
    B2(int j) : j_(j), derived_called2(false) {}
    void mod_fct2() {}
    bool derived_called2;
};

#if 0
struct D_aux : B1, B2 
       //virtually<base_transaction_object> 
{
	// forward constructors 
    D_aux() : B1(1), B2(2) {}
};

#endif
struct D : transaction_object2<D, B1, B2 > 
{
    //D() : B1(1), B2(2) {}
    void mod_fct1() {
        derived_called1=true;
    }
    void mod_fct2() {
        derived_called2=true;
    }
};


#if 0  
#endif

bool test1_KO() {
    B1* ptrB1 = new D();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr(ptrB1)->mod_fct1();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB1)->derived_called1==false) ;
    } BOOST_STM_RETRY
    return false;    
}  


bool test2_KO() {
    B2* ptrB2 = new D();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr(ptrB2)->mod_fct2();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB2)->derived_called2==false) ;
    } BOOST_STM_RETRY
    return false;    
}  


bool test1_OK() {
    B1* ptrB1 = new D();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr_dyn(ptrB1)->mod_fct1();
        //~ B1* aptrB1=_.write_ptr_dyn(ptrB1);
        //~ B1* optrB1 =_.find_original_ptr(aptrB1);
        //~ B1* gptrB1 =_.get_written_ptr(aptrB1);
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB1)->derived_called1==true) ;
    } BOOST_STM_RETRY
    return false;    
}  


bool test2_OK() {
    B2* ptrB2 = new D();
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write_ptr_dyn(ptrB2)->mod_fct2();
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, _.read_ptr(ptrB2)->derived_called2==true) ;
    } BOOST_STM_RETRY
    return false;    
}  

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

//B1 b1;
//typedef boost::is_virtual_base_of<boost::stm::base_transaction_object, B1> TT;
//typedef boost::detail::safe_polymorphic_downcast<TT, B1, boost::stm::base_transaction_object> TT2;

    int res=0;
    res+=test1_OK();
    //res+=test1_KO();
    res+=test2_OK();
    //res+=test2_KO();
    return res;

}
