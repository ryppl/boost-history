/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_MATCH_HPP)
#define BOOST_SPIRIT_MATCH_HPP

///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/core/basics.hpp>
#include <boost/call_traits.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/core/assert.hpp>
#include <boost/spirit/core/safe_bool.hpp>
#include <boost/spirit/core/impl/match_attr_traits.ipp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  match class
    //
    //      The match holds the result of a parser. A match object evaluates
    //      to true when a succesful match is found, otherwise false. The
    //      length of the match is the number of characters (or tokens) that
    //      is successfully matched. This can be queried through its length()
    //      member function. A negative value means that the match is
    //      unsucessful.
    //
    //      Each parser may have an associated attribute. This attribute is
    //      also returned back to the client on a successful parse through
    //      the match object. The match's value() member function returns the
    //      match's value.
    //
    //      A match attribute is valid:
    //
    //          * on a successful match
    //			* when its value is set through the value(val) member function
    //          * if it is assigned or copied from a compatible match object
    //            (e.g. match<double> from match<int>) with a valid attribute.
    //
    //      The match attribute is undefined:
    //
    //          * on an unsuccessful match
    //          * when an attempt to copy or assign from another match object
    //            with an incompatible attribute type (e.g. match<std::string>
    //            from match<int>).
    //
    //      The member function has_valid_attribute() can be queried to know if
    //      it is safe to get the match's attribute. The attribute may be set
    //      through the member function value(v) where v is the new attribute
    //      value.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T = nil_t>
    class match
    {
        typedef typename boost::call_traits<T>::param_type      param_type;
        typedef typename boost::call_traits<T>::const_reference const_reference;

    public:

        typedef T attr_t;

                                match();
        explicit                match(std::size_t length);
                                match(std::size_t length, param_type val);
                                operator safe_bool() const;

        bool                    operator!() const;
        std::ptrdiff_t          length() const;
        bool                    has_valid_attribute() const;
        const_reference         value() const;
        void                    value(param_type val);
        void                    swap(match& other);

        template <typename T2>
        match(match<T2> const& other)
        : len(other.length()), val()
        { impl::match_attr_traits<T>::copy(val, other); }

        template <typename T2>
        match&
        operator=(match<T2> const& other)
        {
            len = other.length();
            impl::match_attr_traits<T>::assign(val, other);
            return *this;
        }

        template <typename MatchT>
        void
        concat(MatchT const& other)
        {
            BOOST_SPIRIT_ASSERT(bool(*this) && bool(other));
            len += other.length();
        }

    private:

        std::ptrdiff_t len;
        boost::optional<T> val;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  match class specialization for nil_t values
    //
    ///////////////////////////////////////////////////////////////////////////
    template <>
    class match<nil_t>
    {
    public:

        typedef nil_t attr_t;

                                match();
        explicit                match(std::size_t length);
                                match(std::size_t length, nil_t);
                                operator safe_bool() const;

        bool                    operator!() const;
        bool                    has_valid_attribute() const;
        std::ptrdiff_t          length() const;
        nil_t                   value() const;
        void                    value(nil_t);
        void                    swap(match& other);

        template <typename T>
        match(match<T> const& other)
        : len(other.length()) {}

        template <typename T>
        match<>&
        operator=(match<T> const& other)
        {
            len = other.length();
            return *this;
        }

        template <typename T>
        void
        concat(match<T> const& other)
        {
            BOOST_SPIRIT_ASSERT(bool(*this) && bool(other));
            len += other.length();
        }

    private:

        std::ptrdiff_t len;
    };

}} // namespace boost::spirit

#endif
#include <boost/spirit/core/impl/match.ipp>

