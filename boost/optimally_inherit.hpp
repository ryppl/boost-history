//----------------------------------------------------------------------------
//
// optimally_inherit.hpp
//
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
// optimally_inherit<> helps avoid EBO pessimization with multiple inheritance
// by only inheriting from non-empty base classes.  See 
// libs/optimally_inherit/doc/index.html
//----------------------------------------------------------------------------
#ifndef BOOST_OPTIMALLY_INHERIT_20020703_HPP
#define BOOST_OPTIMALLY_INHERIT_20020703_HPP
//----------------------------------------------------------------------------
#include <boost/type_traits/object_traits.hpp>
//----------------------------------------------------------------------------
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
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
        struct init_first_tag { };
        struct init_first_first_tag { };
        //--------------------------------------------------------------------
        // Child implementation
        template <class T1, class T2, bool FirstIsEmpty, bool SecondIsEmpty>
        class optimal_parents;
        //--------------------------------------------------------------------
        // Inherit both
        template <class T1, class T2>
        class optimal_parents<T1, T2, false, false>
            : public T1, public T2
        {
        public:
            typedef T1 base1_type;
            typedef T2 base2_type;
        public:
                    optimal_parents(void)           { }

                    template <class U1, class U2>
                    optimal_parents(optimal_parents<U1, U2, false, false> const& rhs)
                        : T1(static_cast<U1 const&>(rhs)), T2(static_cast<U2 const&>(rhs))
                                                    { }

                    template <typename U>
                    optimal_parents(U const& x)
                        : T1(x), T2(x)              { }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_tag const&)
                        : T1(x)                     { }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_first_tag const&)
                        : T1(x, detail::init_first_tag())
                                                    { }

            void    swap(optimal_parents& rhs)
            {
                T1::swap(static_cast<T1&>(rhs));
                T2::swap(static_cast<T2&>(rhs));
            }
        };
        //--------------------------------------------------------------------
        // Inherit first
        template <class T1, class T2>
        class optimal_parents<T1, T2, false, true>
            : public T1
        {
        public:
            typedef T1 base1_type;
            typedef T2 base2_type;
        public:
                    optimal_parents(void)           { }

                    template <class U1, class U2>
                    optimal_parents(optimal_parents<U1, U2, false, true> const& rhs)
                        : T1(static_cast<U1 const&>(rhs))
                                                    { U2 u2; T2 t2(u2); (void) t2; }

                    template <typename U>
                    optimal_parents(U const& x)
                        : T1(x)                     { T2 t2(x); (void) t2; }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_tag const&)
                        : T1(x)                     { }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_first_tag const&)
                        : T1(x, detail::init_first_tag())
                                                    { }

            void    swap(optimal_parents& rhs)
            {
                T1::swap(static_cast<T1&>(rhs));
            }
        };
        //--------------------------------------------------------------------
        // Inherit second
        template <class T1, class T2>
        class optimal_parents<T1, T2, true, false>
            : public T2
        {
        public:
            typedef T1 base1_type;
            typedef T2 base2_type;
        public:
                    optimal_parents(void)           { }

                    template <class U1, class U2>
                    optimal_parents(optimal_parents<U1, U2, true, false> const& rhs)
                        : T2(static_cast<U2 const&>(rhs))
                                                    { U1 u1; T1 t1(u1); (void) t1; }

                    template <typename U>
                    optimal_parents(U const& x)
                        : T2(x)                     { T1 t1(x); (void) t1; }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_tag const&)
                                                    { T1 t1(x); (void) t1; }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_first_tag const&)
                                                    { T1 t1(x, detail::init_first_tag()); (void) t1; }

            void    swap(optimal_parents& rhs)
            {
                T2::swap(static_cast<T2&>(rhs));
            }
        };
        //--------------------------------------------------------------------
        // Inherit neither
        template <class T1, class T2>
        class optimal_parents<T1, T2, true, true>
        {
        public:
            typedef T1 base1_type;
            typedef T2 base2_type;
        public:
                    optimal_parents(void)           { }

                    template <class U1, class U2>
                    optimal_parents(optimal_parents<U1, U2, true, true> const& rhs)
                                                    { U1 u1; T1 t1(u1); U2 u2; T2 t2(u2); (void) t1; (void) t2; }

                    template <typename U>
                    optimal_parents(U const& x)
                                                    { T1 t1(x); T2 t2(x); (void) t1; (void) t2; }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_tag const&)
                                                    { T1 t1(x); (void) t1; }

                    template <typename U>
                    optimal_parents(U const& x, detail::init_first_first_tag const&)
                                                    { T1 t1(x, detail::init_first_tag()); (void) t1; }

            void    swap(optimal_parents& rhs)      { }
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
        typedef detail::optimal_parents<T1, T2,
            ::boost::is_empty<T1>::value,
            ::boost::is_empty<T2>::value
        > type;
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#else  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include "detail/ob_optimally_inherit.hpp"
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//----------------------------------------------------------------------------
#endif // BOOST_OPTIMALLY_INHERIT_20020703_HPP
