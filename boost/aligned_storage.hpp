//-----------------------------------------------------------------------------
// boost aligned_storage.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_ALIGNED_STORAGE_HPP
#define BOOST_ALIGNED_STORAGE_HPP

#include <cstddef> // for std::size_t
#include <cstring> // for std::memcpy
#include <new> // for placement new

#include "boost/config.hpp"
#include "boost/static_assert.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/enum.hpp"
#include "boost/preprocessor/enum_params.hpp"
#include "boost/preprocessor/repeat_from_to_2nd.hpp"
#include "boost/type_traits/alignment_of.hpp"
#include "boost/type_traits/type_with_alignment.hpp"

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/identity.hpp"

// / Delay inclusion of {safe_swap,safe_assign}.hpp     \
// \ because of circular dependencies with this header: /
// #include "boost/utility/safe_assign.hpp"
// #include "boost/utility/safe_swap.hpp"

// BOOST_ALIGNED_STORAGE_LIMIT_CONSTRUCT_AS_ARITY
// / Implementation-defined value describing the greatest number of arguments \
// \ that may be provided to aligned_storage::construct_as.                   /
#define BOOST_ALIGNED_STORAGE_LIMIT_CONSTRUCT_AS_ARITY 5

namespace boost {
namespace detail {

BOOST_STATIC_CONSTANT(
      std::size_t
    , alignment_of_max_align = alignment_of<max_align>::value
    );

} // namespace detail

template <
      std::size_t size_
    , std::size_t alignment_ = -1
>
class aligned_storage
{
private:
    typedef typename mpl::apply_if_c<
          alignment_ == -1
        , mpl::identity<detail::max_align>
        , type_with_alignment<alignment_>
        >::type align_t;

public:
    BOOST_STATIC_CONSTANT(
          std::size_t
        , size = size_
        );
    BOOST_STATIC_CONSTANT(
          std::size_t
        , alignment = (
              alignment_ == -1
            ? detail::alignment_of_max_align
            : alignment_
            )
        );

private:
    union
    {
        char data[size];
        align_t dummy_;
    };

    template <typename T>
    struct fits
    {
        BOOST_STATIC_CONSTANT(
              bool
            , value = (
                   size >= sizeof(T)
                && alignment >= alignment_of<T>::value
                )
            );
    };

public:
	aligned_storage()
	{
	}

    void* raw_pointer()
    {
        return &data[0];
    }

    const void* raw_pointer() const
    {
        return &data[0];
    }

    void memcpy_in(const void* src)
    {
        std::memcpy(raw_pointer(), src, size);
    }

    void memcpy_out(void* dest) const
    {
        std::memcpy(dest, raw_pointer(), size);
    }

public:
    template <typename T>
    void construct_as()
    {
        BOOST_STATIC_ASSERT(fits<T>::value);
        new(&data[0]) T;
    }

    #define BOOST_AUX_ALIGNED_STORAGE_CONSTRUCT_AS_PARAM(N,TYPE)                 \
        const BOOST_PP_CAT(TYPE,N)& BOOST_PP_CAT(arg,N)

    #define BOOST_ALIGNED_STORAGE_CONSTRUCT_AS_FUNCTION(N,_)                     \
        template <                                                               \
          typename T                                                             \
        , BOOST_PP_ENUM_PARAMS(N, typename A)                                    \
        >                                                                        \
        void construct_as(                                                       \
              BOOST_PP_ENUM(N, BOOST_AUX_ALIGNED_STORAGE_CONSTRUCT_AS_PARAM, A)  \
            )                                                                    \
        {                                                                        \
            BOOST_STATIC_ASSERT(fits<T>::value);                                 \
            new(&data[0]) T(BOOST_PP_ENUM_PARAMS(N, arg));                       \
        }

    BOOST_PP_REPEAT_FROM_TO_2ND(
          1
        , BOOST_PP_INC(BOOST_ALIGNED_STORAGE_LIMIT_CONSTRUCT_AS_ARITY)
        , BOOST_ALIGNED_STORAGE_CONSTRUCT_AS_FUNCTION
        , _
        )

    #undef BOOST_AUX_ALIGNED_STORAGE_CONSTRUCT_AS_PARAM
    #undef BOOST_ALIGNED_STORAGE_CONSTRUCT_AS_FUNCTION

    template <typename T>
    T& get_as()
    {
        return *reinterpret_cast<T*>(&data[0]);
    }

    template <typename T>
    const T& get_as() const
    {
        return *reinterpret_cast<const T*>(&data[0]);
    }

    template <typename T>
    T* get_pointer_as()
    {
        return reinterpret_cast<T*>(&data[0]);
    }

    template <typename T>
    const T* get_pointer_as() const
    {
        return reinterpret_cast<const T*>(&data[0]);
    }

    template <typename T>
    void destroy_as() const
    {
        get_pointer_as<const T>()->~T();
    }

    template <typename T>
    void assign_as(const aligned_storage& operand)
    {
        safe_assign(
              get_as<T>()
            , operand.template get_as<const T>()
            );
    }

    template <typename T>
    void swap_as(aligned_storage& operand)
    {
        safe_swap(
              get_as<T>()
            , operand.template get_as<T>()
            );
    }

private:
	aligned_storage(const aligned_storage&);
	aligned_storage& operator=(const aligned_storage&);
};

} // namespace boost

// / Delay inclusion of {safe_swap,safe_assign}.hpp because \
// \ of circular dependencies with this header:             /
#include "boost/utility/safe_assign.hpp"
#include "boost/utility/safe_swap.hpp"

#endif // BOOST_ALIGNED_STORAGE_HPP