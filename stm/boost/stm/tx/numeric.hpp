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

#ifndef BOOST_STM_TX_NUMERIC__HPP
#define BOOST_STM_TX_NUMERIC__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

template <typename T>
class numeric : public transaction_object< numeric<T> >
{
protected:
    T val_;
public:
    //-----------------------------------------------------------------------------
    numeric() : val_(0) {}

    //
    template<class U>
    numeric(numeric<U> const& r) : val_(r.value()) {}

    // contructor from a implicitly convertible to T
    template <typename U>
    numeric(U v) : val_(v) {}
    //numeric(T v) : val_(v) {}
    ~numeric() {}

    #if 0
    template<class U>
    numeric& operator=(numeric<U> const& r) {
        val_=r.value();
    }
    #endif

    operator T() const { return value(); }
    operator T&() { return ref(); }

    T& ref() {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }

            return tx->write(*this).val_;
        }
        return val_;
    }

    T value() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(*this).val_;
        }
        return val_;
    }

};


typedef numeric<bool> boolean;

typedef numeric<char> char_t;
typedef numeric<unsigned char> uchar_t;
typedef numeric<short> short_t;
typedef numeric<unsigned short> ushort_t;
typedef numeric<int> int_t;
typedef numeric<unsigned int> uint_t;
typedef numeric<long> long_t;
typedef numeric<unsigned long> ulong_t;

typedef numeric<float> float_t;
typedef numeric<double> double_t;

}}}
#endif


