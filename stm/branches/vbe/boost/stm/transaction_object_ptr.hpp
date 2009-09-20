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

#ifndef BOOST_STM_TRANSACTION_OBJECT_PTR__H
#define BOOST_STM_TRANSACTION_OBJECT_PTR__H


#include <boost/stm/base_transaction.hpp>

namespace boost { namespace stm {

template <typename TO>
class transaction_object_ptr : public transaction_object<transaction_object_ptr<TO> > {
public:
    TO* ptr_;
    typedef transaction_object_ptr<TO> this_type;
    typedef transaction_object<transaction_object_ptr<TO> > base_type;
    transaction_object_ptr() :  base_type(), ptr_(0) {
        std::cout << "transaction_object_ptr "<< __LINE__ <<" " << __FILE__ << " "<< int(this) << std::endl;
    }
    transaction_object_ptr(const transaction_object_ptr & rhs) :  base_type(rhs),  ptr_(rhs.ptr_) {
        std::cout << "transaction_object_ptr "<< __LINE__ <<" " << __FILE__ << " "<< int(this) <<" "<< int(&rhs) << std::endl;
    }
    transaction_object_ptr(transaction_object_ptr & rhs) :  base_type(rhs), ptr_(rhs.rhs) {
        std::cout << "transaction_object_ptr "<< __LINE__ <<" " << __FILE__ << " "<< int(this) <<" "<< int(&rhs) << std::endl;
    }
    transaction_object_ptr(TO* ptr) :  base_type(), ptr_(ptr) {
        std::cout << "transaction_object_ptr "<<__LINE__ <<" " << __FILE__ << " "<< int(this) << std::endl;
    }
    ~transaction_object_ptr() {
        std::cout << "transaction_object_ptr "<<__LINE__ <<" " << __FILE__ << " "<< int(this) << std::endl;
    }    
    this_type& operator=(TO* rhs) {
        ptr_=rhs;
        return *this;
    }

    TO* get() const {
        std::cout << "get" << std::endl;
        return ptr_;
    }

    inline TO& operator*() const { return *get(); }
    //inline TO* operator->() const { return get(); }
    
    };


} // namespace core
}
#endif // BASE_TRANSACTION_H


