// (C) Copyright Jonathan Turkanis 2003.

#ifndef BOOST__IOFM__DETAIL__TYPE_DEDUCER__HPP
#define BOOST__IOFM__DETAIL__TYPE_DEDUCER__HPP

#include <boost/outfmt/detail/type_traits.hpp>
#include <boost/outfmt/detail/select.hpp>    

   namespace boost { namespace io
   {
      // type information - for mapping an outputter to the type

      namespace detail {

          struct basic_tag { };
          struct pair_tag { };
          struct container_tag { };
          struct array_tag { };
          struct nary_tag { };

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
            BOOST_STATIC_CONSTANT(size_t, length = sizeof(T) / sizeof(*t[0]));
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
            struct get_first_type { typedef typename U::first_type type; };

            template<typename U>
            struct get_second_type { typedef typename U::second_type type; };

            template<typename U>
            struct get_base_type { typedef typename U::base_type type; };

            typedef typename 
                    select< is_std_pair<T>, eval< get_first_type<T> >,
                            is_compressed_pair<T>, eval< get_first_type<T> >,
                            is_std_complex<T>, eval< get_value_type<T> >,
                            is_rational<T>, eval< get_int_type<T> >,
                            is_interval<T>, eval< get_base_type<T> > 
                    >::type                                            base1_type;

            typedef typename 
                    select< is_std_pair<T>, eval< get_second_type<T> >,
                            is_compressed_pair<T>, eval< get_second_type<T> >,
                            mpl::true_, base1_type 
                    >::type                                            base2_type;    
        };

        //--------------nary_deducer---------------------------------------------//

        template<typename T>
        struct nary_deducer {
            typedef nary_tag category;
            typedef typename T::value_type base_type;
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
                        mpl::true_, detail::basic_deducer<T>
                    >::type >
      struct type_deducer : public Base { typedef T object_type; };
   }}
#endif
