// (C) Copyright Jonathan Turkanis 2003.

#ifndef BOOST__IOFM__DETAIL__OUTPUT_DEDUCER__HPP
#define BOOST__IOFM__DETAIL__OUTPUT_DEDUCER__HPP
#  include <boost/mpl/apply_if.hpp>
#  include <boost/mpl/identity.hpp>
#  include <boost/outfmt/format_objects.hpp>

   namespace boost { namespace io
   {
      namespace detail
      {
         template< typename Category,
                   typename T, 
                   typename FormatType = char >
         struct output_deducer {

            template<class TT, typename FF>
            struct composite_deducer {
                typedef output_deducer<
                            typename type_deducer<TT>::category,
                            typename type_deducer<TT>::object_type,
                            FF
                        > type;        
            };

            typedef type_deducer<T>                                 info;
            typedef typename info::category                         category;

                // Helper metafunctions.

            template<typename U>
            struct get_comp {
                typedef composite_deducer<
                            typename type_deducer<U>::base_type,
                            FormatType
                        > type; // RHD: broken in GCC 2.95.3-5
            };

            template<typename U>
            struct get_comp1 {
                typedef composite_deducer<
                            typename type_deducer<U>::base1_type,
                            FormatType
                        > type; // RHD: broken in GCC 2.95.3-5
            };

            template<typename U>
            struct get_comp2 {
                typedef composite_deducer<
                            typename type_deducer<U>::base2_type,
                            FormatType
                        > type; // RHD: broken in GCC 2.95.3-5
            };

            template<typename Base>
            struct get_array_out { 
                typedef array_output<  
                            FormatType, 
                            typename Base::type::outputter
                        > type;
            };

            template<typename Base>
            struct get_container_out { 
                typedef container_output<  
                            FormatType, 
                            typename Base::type::outputter
                        > type;
            };

            template<typename Base1, typename Base2>
            struct get_pair_out { 
                typedef pair_output<  
                            FormatType, 
                            typename Base1::type::outputter, 
                            typename Base2::type::outputter 
                        > type;
            };

            template<typename Base>
            struct get_nary_out { 
                typedef static_nary_output<  
                            FormatType, 
                            typename Base::type::outputter
                        > type;
            };

                // Base types.

            typedef typename
                    select<
                        is_same<category, basic_tag>, 
                            void,
                        is_same<category, pair_tag>, 
                            eval< get_comp1<T> >,
                        mpl::true_, 
                            eval< get_comp<T> >
                    >::type                                         base1;

            typedef typename
                    mpl::apply_if<
                        is_same<category, pair_tag>, 
                        get_comp2<T>,
                        mpl::identity< void >
                    >::type                                         base2;

                // Outputter type.

            typedef typename
                    select<
                        is_same<category, basic_tag>, 
                            basic_output,
                        is_same<category, array_tag>, 
                            eval< get_array_out<base1> >,
                        is_same<category, container_tag>, 
                            eval< get_container_out<base1> >,
                        is_same<category, pair_tag>, 
                            eval< get_pair_out<base1, base2> >,
                        is_same<category, nary_tag>, 
                            eval< get_nary_out<base1> > 
                    >::type                                         outputter;        
            
                // Generate function.

            static outputter generate() { return generate(category()); }

                // Implementation of generate.

            static outputter generate(basic_tag) { return outputter(); }
            static outputter generate(array_tag) 
                { 
                    typedef typename base1::type composite;
                    return outputter(composite::generate(), info::length);
                }
            static outputter generate(container_tag) 
                {
                    typedef typename base1::type composite;
                    return outputter(composite::generate());
                }
            static outputter generate(pair_tag) 
                {
                    typedef typename base1::type composite1;
                    typedef typename base2::type composite2;
                    return outputter( composite1::generate(), 
                                      composite2::generate() );
                } 
            static outputter generate(nary_tag)
                {
                    typedef typename base1::type composite;
                    return outputter(composite::generate());
                }
            };

      }     // End namespace detail.

      template< class T, typename FormatType = char >
      struct deducer {
         typedef type_deducer<T>                  info;
         typedef detail::output_deducer< 
                     typename info::category,
                     typename info::object_type, 
                     FormatType
                 >                                type;
      };
   }}
#endif
