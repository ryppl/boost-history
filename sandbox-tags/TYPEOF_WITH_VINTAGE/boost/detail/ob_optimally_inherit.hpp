//----------------------------------------------------------------------------
// Copyright (C) 2002, David B. Held.
// 
// Code inspired by Andrei Alexandrescu's OptionallyInherit template as well
// as boost::compressed_pair.
// 
// Permission to copy, use, modify, sell, and distribute this software is
// granted provided this copyright notice appears in all copies. This software
// is provided "as is" without express or implied warranty, and with no claim
// as to its suitability for any purpose.
// 
// See http://www.boost.org/ for most recent version, including documentation.
//----------------------------------------------------------------------------
//
// ob_optimally_inherit.hpp
// 
// Obsolete implementation of optimally_inherit<> for compilers lacking 
// partial template specialization support.
//----------------------------------------------------------------------------
#ifndef BOOST_OPTIMALLY_INHERIT_20020930_HPP
#define BOOST_OPTIMALLY_INHERIT_20020930_HPP
//----------------------------------------------------------------------------
#include <boost/mpl/if.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        // Initialization tags - bad hack
        //   These tags allow c'tors which pass an argument to just selected
        // derived classes.  Clearly, this is not an optimal solution, but
        // is a necessary evil until a better one is found.
        //--------------------------------------------------------------------
//        struct copy_tag { };
        struct init_first_tag { };
        struct init_first_first_tag { };
        //--------------------------------------------------------------------
        // Child implementation
        template <bool FirstIsEmpty, bool SecondIsEmpty>
        struct optimal_parents;
        //--------------------------------------------------------------------
        // Inherit both
            template <class T1, class T2>
            class impl0 : public T1, public T2
            {
            public:
                typedef T1 base1_type;
                typedef T2 base2_type;
            public:
                        impl0(void)                      { }

                        template <class U1, class U2>
                        impl0(impl0<U1, U2>& rhs)
                            : T1(static_cast<U1&>(rhs)), T2(static_cast<U2&>(rhs))
                                                        { }

                        template <typename U>
                        impl0(U const& x)
                            : T1(x), T2(x)              { }

                        template <typename U>
                        impl0(U const& x, detail::init_first_tag const&)
                            : T1(x)                     { }

                        template <typename U>
                        impl0(U const& x, detail::init_first_first_tag const&)
                            : T1(x, detail::init_first_tag())
                                                        { }
            };
        //--------------------------------------------------------------------
        // Inherit first
            template <class T1, class T2>
            class impl1 : public T1
            {
            public:
                typedef T1 base1_type;
                typedef T2 base2_type;
            public:
                        impl1(void)                      { }

                        template <class U1, class U2>
                        impl1(impl1<U1, U2>& rhs)
                            : T1(static_cast<U1&>(rhs))
                                                        { U2 u2; T2 t2(u2); }

                        template <typename U>
                        impl1(U const& x)
                            : T1(x)                     { T2 t2(x); }

                        template <typename U>
                        impl1(U const& x, detail::init_first_tag const&)
                            : T1(x)                     { }

                        template <typename U>
                        impl1(U const& x, detail::init_first_first_tag const&)
                            : T1(x, detail::init_first_tag())
                                                        { }
            };
        //--------------------------------------------------------------------
        // Inherit second
            template <class T1, class T2>
            class impl2 : public T2
            {
            public:
                typedef T1 base1_type;
                typedef T2 base2_type;
            public:
                        impl2(void)                      { }

                        template <class U1, class U2>
                        impl2(impl2<U1, U2>& rhs)
                            : T2(static_cast<U2&>(rhs))
                                                        { U1 u1; T1 t1(u1); }

                        template <typename U>
                        impl2(U const& x)
                            : T2(x)                     { T1 t1(x); }

                        template <typename U>
                        impl2(U const& x, detail::init_first_tag const&)
                                                        { T1 t1(x); }

                        template <typename U>
                        impl2(U const& x, detail::init_first_first_tag const&)
                                                        { T1 t1(x, detail::init_first_tag()); }
            };
        //--------------------------------------------------------------------
        // Inherit neither
            template <class T1, class T2>
            class impl3
            {
            public:
                typedef T1 base1_type;
                typedef T2 base2_type;
            public:
                        impl3(void)                      { }

                        template <class U1, class U2>
                        impl3(impl3<U1, U2>& rhs)         { U1 u1; T1 t1(u1); U2 u2; T2 t2(u2); }

                        template <typename U>
                        impl3(U const& x)
                                                        { T1 t1(x); T2 t2(x); }

                        template <typename U>
                        impl3(U const& x, detail::init_first_tag const&)
                                                        { T1 t1(x); }

                        template <typename U>
                        impl3(U const& x, detail::init_first_first_tag const&)
                                                        { T1 t1(x, detail::init_first_tag()); }
            };
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
    // type generator optimally_inherit
    //------------------------------------------------------------------------
    template <class T1, class T2>
    class optimally_inherit
    {
    public:
        typedef mpl::if_<
            ::boost::is_empty<T1>,
            mpl::if_<
                ::boost::is_empty<T2>,
                detail::impl3<T1, T2>,
                detail::impl2<T1, T2>
            >::type,
            mpl::if_<
                ::boost::is_empty<T2>,
                detail::impl1<T1, T2>,
                detail::impl0<T1, T2>
            >::type
        >::type type;
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_OPTIMALLY_INHERIT_20020930_HPP
