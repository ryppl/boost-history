// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__FORMAT_OBJECTS__STATIC_NARY_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__STATIC_NARY_OUTPUT__HPP
#  include <boost/outfmt/formatter.hpp>

#  if !defined(BOOST_IOFM__NO_LIB_QUATERNION)
#     include <boost/math/quaternion.hpp> // boost::math::quaternion
#  endif
#  if !defined(BOOST_IOFM__NO_LIB_OCTONION)
#     include <boost/math/octonion.hpp>   // boost::math::octonion
#  endif

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class static_nary_output: public formatter_t
                                <
                                   FormatType,
                                   static_nary_output< FormatType, Outputter >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef static_nary_output< FormatType, Outputter >      this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
         public:
            Outputter                  out;
         public: // nary-valued types
#        if !defined(BOOST_IOFM__NO_LIB_QUATERNION)
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const boost::math::quaternion< T > & h
                                  ) const
            {
               os << open();
               out( os, h.R_component_1()) << separator();
               out( os, h.R_component_2()) << separator();
               out( os, h.R_component_3()) << separator();
               return( out( os, h.R_component_4()) << close());
            }
#        endif
#        if !defined(BOOST_IOFM__NO_LIB_OCTONION)
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const boost::math::octonion< T > & o
                                  ) const 
            {
               os << open();
               out( os, o.R_component_1()) << separator();
               out( os, o.R_component_2()) << separator();
               out( os, o.R_component_3()) << separator();
               out( os, o.R_component_4()) << separator();
               out( os, o.R_component_5()) << separator();
               out( os, o.R_component_6()) << separator();
               out( os, o.R_component_7()) << separator();
               return( out( os, o.R_component_8()) << close());
            }
#        endif
         public:
            inline           static_nary_output()
            {
            }
            inline           static_nary_output( const static_nary_output & o ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( o ),
               out( o.out )
            {
            }
            inline           static_nary_output( const Outputter & o ):
               out( o )
            {
            }
      };

      template< class FormatType >
      inline static_nary_output< FormatType >    naryfmtex()
      {
         static_nary_output< FormatType > out;
         return( out );
      }

      inline static_nary_output< char * >        naryfmt()
      {
         return( naryfmtex< char * >());
      }

      template< class Outputter >
      inline static_nary_output< typename Outputter::format_type, Outputter >
                                                 naryfmtout
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( static_nary_output< typename Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
