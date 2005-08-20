// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__DETAIL__LIST_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__DETAIL__LIST_OUTPUT__HPP
#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io { namespace detail
   {
      template< typename FormatType, class RetType, typename Outputter = boost::io::basic_output >
      class list_output: public formatter_t< FormatType, RetType >
      {
         public:
            typedef list_output< FormatType, RetType, Outputter >    this_type;
            typedef typename formatter_t< FormatType, this_type >::traits_type
                                                                     traits_type;
         private:
            Outputter                  outputter;
         public:
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream &  os,
                                     ForwardIterator first,
                                     ForwardIterator last
                                  ) const 
            {
               os << open();

               while( first != last )
               {
                  outputter( os, *first );

                  if( ++first != last )
                     os << separator();
               }

               return( os << close());
            }
         public:
            inline           list_output()
            {
            }
            inline           list_output( const list_output & lo ):
               formatter_t< FormatType, RetType >( lo ),
               outputter( lo.outputter )
            {
            }
            inline           list_output( const Outputter & o ):
               outputter( o )
            {
            }
      };
   }}}
#endif
