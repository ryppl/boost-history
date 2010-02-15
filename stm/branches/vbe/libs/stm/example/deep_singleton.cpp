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
//#include <boost/foreach.hpp>
//#include <boost/thread.hpp>
//#include <boost/shared_ptr.hpp>
//#include <vector>
//#include <list>
//#include <stdlib.h>

//#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace boost::stm;

  class Singleton 
      : public deep_transaction_object<Singleton> 
  {
    Singleton():f(0) {}
    Singleton(Singleton const&); // =deleted
    Singleton& operator=(
	Singleton const&); // =deleted
  public:
    static  Singleton* instance() {
        static  Singleton instance_;
    
        return &instance_;
    }
    int f;
  };

  
bool test() {
    BOOST_STM_TRANSACTION(_) {
        _.write_ptr(Singleton::instance());
        Singleton::instance()->f=1;
    } BOOST_STM_RETRY
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, Singleton::instance()->f==1) ;
    } BOOST_STM_RETRY
    return false;    
}  

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    int res=0;
    res+=test();
    return res;

}
