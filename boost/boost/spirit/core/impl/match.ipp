/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_MATCH_IPP)
#define BOOST_SPIRIT_MATCH_IPP
#include <algorithm>

namespace boost { namespace spirit {

///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline match<T>::match()
    : len(-1), val() {}

    template <typename T>
    inline match<T>::match(std::size_t length)
    : len(length), val() {}

    template <typename T>
    inline match<T>::match(std::size_t length, param_type val_)
    : len(length), val(val_) {}

    template <typename T>
    inline match<T>::operator safe_bool() const
    { return make_safe_bool(len >= 0); }

    template <typename T>
    inline bool
    match<T>::operator!() const
    { return len < 0; }

    template <typename T>
    inline std::ptrdiff_t
    match<T>::length() const
    { return len; }

    template <typename T>
    inline bool
    match<T>::has_valid_attribute() const
    { return !!val; }

    template <typename T>
    inline typename boost::call_traits<T>::const_reference
    match<T>::value() const
    { BOOST_SPIRIT_ASSERT(val.get()); return *val.get(); }

    template <typename T>
    inline void
    match<T>::value(param_type val_)
    { val.reset(val_); }

    template <typename T>
    inline void
    match<T>::swap(match& other)
    {
        std::swap(len, other.len);
        std::swap(val, other.val);
    }

///////////////////////////////////////////////////////////////////////////////
    inline match<nil_t>::match()
    : len(-1) {}

    inline match<nil_t>::match(std::size_t length)
    : len(length) {}

    inline match<nil_t>::match(std::size_t length, nil_t)
    : len(length) {}

    inline match<nil_t>::operator safe_bool() const
    { return make_safe_bool(len >= 0); }

    inline bool
    match<nil_t>::operator!() const
    { return len < 0; }

    inline bool
    match<nil_t>::has_valid_attribute() const
    { return false; }

    inline std::ptrdiff_t
    match<nil_t>::length() const
    { return len; }

    inline nil_t
    match<nil_t>::value() const
    { return nil_t(); }

    inline void
    match<nil_t>::value(nil_t) {}

    inline void
    match<nil_t>::swap(match& other)
    { std::swap(len, other.len); }

}} // namespace boost::spirit

#endif

