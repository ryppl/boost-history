#ifndef BOOST_ORDERING_OPERATORS_20020920_HPP
#define BOOST_ORDERING_OPERATORS_20020920_HPP

namespace boost
{

////////////////////////////////////////////////////////////////////////////////
// free ordering operators for smart_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator<(
        smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs
    )
    {
        return get_impl(lhs) < rhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator<(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return lhs < get_impl(rhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator>(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return rhs < lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator>(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return rhs < lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator<=(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return !(rhs < lhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator<=(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return !(rhs < lhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator>=(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return !(lhs < rhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator>=(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return !(lhs < rhs);
    }

}   // namespace boost

#endif // BOOST_ORDERING_OPERATORS_20020920_HPP
