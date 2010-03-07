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
#include <boost/stm/tx/proxy_cache.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
// transactional object numeric class that wraps a numeric builtin type providing
// a transparent transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(numeric<T>)>>>>=sizeof(T)
//-----------------------------------------------------------------------------

template <typename T>
class numeric : public proxy_cache< numeric<T>, T >
{
    typedef proxy_cache< numeric<T> ,T > base_type;
public:
    //-----------------------------------------------------------------------------
    numeric() : base_type(0) {}
    template<class U>
    numeric(numeric<U> const& r) : base_type(r) {}
    template <typename U>
    numeric(U v) : base_type(v) {}

    // shallow copy
    numeric(numeric const& rhs, stm::shallow_t)
    : base_type(rhs, stm::shallow)
    {}
    // shallow assignment
    numeric& shallow_assign(numeric const& rhs)
    {
        this->base_type::shallow_assign(rhs);
        return *this;
    }

};

}
// shallow trait
template <typename T>
struct has_shallow_copy_semantics<tx::numeric<T> > : boost::mpl::true_
{};

namespace tx {
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


