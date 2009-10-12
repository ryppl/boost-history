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

#ifndef BOOST_STM_TRANSACTION_OBJECT_PTR__HPP
#define BOOST_STM_TRANSACTION_OBJECT_PTR__HPP


#include <boost/stm/base_transaction.hpp>

namespace boost { namespace stm {

template <typename TO>
class transaction_object_ptr : public transaction_object<transaction_object_ptr<TO> > {
public:
    TO* ptr_;
    typedef transaction_object_ptr<TO> this_type;
    typedef transaction_object<transaction_object_ptr<TO> > base_type;
    transaction_object_ptr() :  base_type(), ptr_(0) {}
    transaction_object_ptr(const transaction_object_ptr & rhs) :  base_type(rhs),  ptr_(rhs.ptr_) {}
    transaction_object_ptr(transaction_object_ptr & rhs) :  base_type(rhs), ptr_(rhs.rhs) {}
    transaction_object_ptr(TO* ptr) :  base_type(), ptr_(ptr) {}
    ~transaction_object_ptr() {}

    this_type& operator=(TO* rhs) {
        ptr_=rhs;
        return *this;
    }

    TO* get() const {
        return ptr_;
    }
    inline TO& operator*() const { return *get(); }
    inline TO* operator->() const { return get(); }

    };


} // namespace core
}
#endif // BOOST_STM_TRANSACTION_OBJECT_PTR__HPP


