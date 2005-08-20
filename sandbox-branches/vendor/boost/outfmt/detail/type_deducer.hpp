// (C) Copyright Jonathan Turkanis 2003-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_TYPE_DEDUCER_HPP
#define BOOST_IOFM_DETAIL_TYPE_DEDUCER_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/config.hpp> // Make sure size_t is in 'std'
#  include <cstddef>
#  include <boost/outfmt/detail/type_traits.hpp>
#  include <boost/outfmt/detail/select.hpp>    

   namespace boost { namespace io
   {
      // type information - for mapping an outputter to the type

      namespace detail {

          struct basic_tag { };
          struct pair_tag { };
          struct container_tag { };
          struct array_tag { };
          struct nary_tag { };
          struct range_tag { };

        //--------------array_deducer-----------------------------------------//

        template<typename T>
        struct basic_deducer {
            typedef basic_tag category;
        };

        template<typename T>
        struct array_deducer {
            typedef array_tag category;
            static const T* t;
            typedef typename remove_bounds<T>::type base_type;
            BOOST_STATIC_CONSTANT(std::size_t, length = sizeof(T) / sizeof(*t[0]));
        };

        //--------------container_deducer-------------------------------------//

        template<typename T>
        struct container_deducer {
            typedef container_tag category;
            typedef typename T::value_type base_type;
        };

        //--------------pair_deducer------------------------------------------//

        template<typename T>
        struct pair_deducer {
            typedef pair_tag category;
            template<typename U>
            struct get_value_type { typedef typename U::value_type type; };

            template<typename U>
            struct get_int_type { typedef typename U::int_type type; };

            template<typename U>
            struct get_base_type { typedef typename U::base_type type; };

            template<typename U>
            struct get_first_type { typedef typename U::first_type type; };

            template<typename U>
            struct get_second_type { typedef typename U::second_type type; };

            typedef typename 
                    select< is_type<T, pair_type>,         eval< get_first_type<T> >,
                            is_type<T, nary2value_type>,   eval< get_value_type<T> >,
                            is_type<T, nary2int_type>,     eval< get_int_type<T> >,
                            is_type<T, nary2base_type>,    eval< get_base_type<T> >
                    >::type                                            base1_type;

            typedef typename 
                    select< is_type<T, pair_type>, eval< get_second_type<T> >,
                            mpl::true_,            base1_type 
                    >::type                                            base2_type;    
        };

        //--------------nary_deducer---------------------------------------------//

        template<typename T>
        struct nary_deducer {
            typedef nary_tag category;
            typedef typename T::value_type base_type;
        };

        //--------------range_deducer-------------------------------------//

        template<typename T>
        struct range_deducer {
            typedef range_tag category;
            typedef typename T::elem_type base_type;
        };

      }             // End namespace detail.

      template< typename T,
                typename Base = // VC6 Workaround (error C2516)
                    BOOST_DEDUCED_TYPENAME 
                    select<
                        is_array<T>, detail::array_deducer<T>,
                        is_container<T>, detail::container_deducer<T>,
                        is_pair<T>, detail::pair_deducer<T>,
                        is_nary<T>, detail::nary_deducer<T>,
                        is_range<T>, detail::range_deducer<T>,
                        mpl::true_, detail::basic_deducer<T>
                    >::type >
      struct type_deducer : public Base { typedef T object_type; };
   }}
#endif
