//  boost cast.hpp header file  ----------------------------------------------//

//  (C) Copyright Kevlin Henney and Dave Abrahams 1999.
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/conversion for Documentation.


#ifndef BOOST_SAFE_DOWNCAST__HPP
#define BOOST_SAFE_DOWNCAST__HPP

# include <boost/config.hpp>
# include <boost/assert.hpp>
# include <typeinfo>
# include <boost/type.hpp>
# include <boost/limits.hpp>
# include <boost/detail/select_type.hpp>
# include <boost/type_traits/is_virtual_base_of.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/mpl/or.hpp>

# include <boost/cast.hpp>
# include <boost/serialization/smart_cast.hpp>



//  It has been demonstrated numerous times that MSVC 6.0 fails silently at link
//  time if you use a template function which has template parameters that don't
//  appear in the function's argument list.
//
//  TODO: Add this to config.hpp?
# if defined(BOOST_MSVC) && BOOST_MSVC < 1300
#  define BOOST_EXPLICIT_DEFAULT_TARGET , ::boost::type<Target>* = 0
# else
#  define BOOST_EXPLICIT_DEFAULT_TARGET
# endif

namespace boost
{

//  safe_polymorphic_downcast  --------------------------------------------------------//

// When the Base class is virtual inherited we need to use dynamic_cast to downcast.
// Of course as most of the time the Derived class don't inherit virtualy from BASE a static_cast is enough

    namespace detail {
        template <typename IsVirtualBaseOf, class Target, class Source>
        struct safe_polymorphic_downcast;

        template <class Target, class Source>
        struct safe_polymorphic_downcast<true_type, Target,Source> {
            inline static Target apply(Source* x ) {
                return dynamic_cast<Target>(x);
            }
        };
        template <class Target, class Source>
        struct safe_polymorphic_downcast<false_type, Target,Source> {
            inline static Target apply(Source* x ) {
                return static_cast<Target>(x);
                //return polymorphic_downcast<Target>(x);
            }
        };

    }
    template <class Target, class Source>
    inline Target safe_polymorphic_downcast(Source* x BOOST_EXPLICIT_DEFAULT_TARGET)
    {
        #if 1
        return serialization::smart_cast<Target, Source*>(x);
        #else
        typedef typename is_virtual_base_of<Source, typename remove_pointer<Target>::type >::type IsVirtualBaseOf;
        return detail::safe_polymorphic_downcast<true_type,Target,Source>::apply(x);
        #endif
    }

    template <class Target, class Inter, class Source>
    inline Target safe_polymorphic_downcast_2(Source* x BOOST_EXPLICIT_DEFAULT_TARGET)
    {
        typedef typename is_virtual_base_of<Source, Inter >::type IsVirtualBaseOf;
        return detail::safe_polymorphic_downcast<IsVirtualBaseOf,Target,Source>::apply(x);
    }

    template <class Target, class Inter1, class Inter2, class Source>
    inline Target safe_polymorphic_downcast_3(Source* x BOOST_EXPLICIT_DEFAULT_TARGET)
    {
        typedef integral_constant<bool,
                                is_virtual_base_of<Source, Inter1>::type::value || is_virtual_base_of<Source, Inter2>::type::value
                            > IsVirtualBaseOf;
        return detail::safe_polymorphic_downcast<IsVirtualBaseOf,Target,Source>::apply(x);
    }

#  undef BOOST_EXPLICIT_DEFAULT_TARGET

} // namespace boost

#endif  // BOOST_CAST_HPP
