// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XMLDOM_LITE_XMLDOMLite_CPP
#define BOOST_XMLDOM_LITE_XMLDOMLite_CPP
#  if defined(__BORLANDC__)
      // W8004: '<variable>' is assigned a value that is never used in function <function>
#     pragma option push -w-8004
#  endif

#  include "../xmldom-lite.hpp"
#  include <iostream>

   // xmldom::attribute_list

   std::string & xmldom::attribute_list::operator[]( const std::string a ) // retain insertion order
   {
      for( iterator i = begin(); i != end(); ++i )
      {
         if(( *i ).first == a )  return(( *i ).second );
      }

      push_back( std::pair< std::string, std::string >( a, "" ));
      return( back().second );
   }

   bool xmldom::attribute_list::find( const std::string a )
   {
      for( iterator i = begin(); i != end(); ++i )
      {
         if(( *i ).first == a )  return( true );
      }
      return( false );
   }

   // xmldom::element

   bool xmldom::element::parse( xmllexer & xl )
   {
      switch( xl.tok )
      {
         case token::cdataBegin: case token::commentBegin:
         {
            bool                       cdata = xl.tok == token::cdataBegin;
            if( xl.gettoken() == token::characterData )
            {
               append( xl.data, cdata ? nodetype::CDATASection : nodetype::comment );

               xl.gettoken();
               if(( cdata && ( xl.tok != token::cdataEnd )) || ( !cdata && ( xl.tok != token::commentEnd )))
                  xl.error( "no closing markup found" );
            }
         } break;
         
         case token::piBegin:
         {
            if( xl.gettoken() != token::name )
               xl.error( "name expected" );

            element_ptr                elem = append( xl.data, nodetype::ProcessingInstruction );
            for( ;; ) switch( xl.gettoken())
            {
               case token::name:
               {
                  std::string &        attr = elem -> attr[ xl.data ];

                  if(( xl.gettoken() == token::equal ) && ( xl.gettoken() == token::string ))
                     attr = xl.data;
                  else
                     xl.error( "unexpected token" );
               } break;
               case token::piEnd:      return( true );
               default:                xl.error( "unexpected token" );
            }
         } // break;

         case token::elementBeginOpen:
         {
            if( xl.gettoken() != token::name )
               xl.error( "name expected" );

            element_ptr                elem = append( xl.data, nodetype::element );
            bool                       quit = false;
            for( quit = false; !quit; ) switch( xl.gettoken())
            {
               case token::name:
               {
                  std::string &        attr = elem -> attr[ xl.data ];

                  if(( xl.gettoken() == token::equal ) && ( xl.gettoken() == token::string ))
                     attr = xl.data;
                  else
                     xl.error( "unexpected token" );
               } break;
               case token::elementSingleClose:
                  return( true );
               case token::elementClose:
                  quit = true;
                  break;
               default:                xl.error( "unexptected token" );
            }

            for( quit = false; !quit; ) switch( xl.gettoken())
            {
               case token::elementEndOpen:
                  quit = true;
                  break;
               default:
               {
                  if( !elem -> parse( xl ))
                     xl.error( "unexpected token" );
               }
            }

            if(( xl.gettoken() != token::name ) || ( xl.data != elem -> name ))
               xl.error( "end tag does not match opening tag" );

            if( xl.gettoken() != token::elementClose )
               xl.error( "unexpected token" );
         } break;

         case token::characterData:
            append( xl.data, nodetype::text );
            break;
         default:
            xl.error( "unexpected token" );
      }
      return( true );
   }

   void xmldom::element::save( boost::detail::indentor< std::ofstream > & os, bool pre, bool post )
   {
      switch( type )
      {
         case nodetype::CDATASection: case nodetype::comment:
         {
            if( pre )                  os.indent( true );

            if( type == nodetype::CDATASection )
               os << "<![CDATA[" << name << "]]>";
            else
               os << "<!--" << name << "-->";
         } break;
         case nodetype::text:          os << name;         break;
         default:
         {
            bool                       single = ( size() == 1 ) &&
                                                (
                                                   front() -> type == nodetype::text ||
                                                   front() -> type == nodetype::CDATASection
                                                );

            if( pre )                  os.indent( true );

            os << ( type == nodetype::ProcessingInstruction ? "<?" : "<" )
               << name;

            for( attribute_list::iterator i = attr.begin(); i != attr.end(); ++i )
               os << ' ' << ( *i ).first << " = \"" << ( *i ).second << "\"";

            if( type == nodetype::ProcessingInstruction )
               os << "?>";
            else if( empty())
            {
               os << "/>";
               if( post )              os.indent( true );
            }
            else
            {
               os << ">";
               {
                  boost::detail::indentor< std::ofstream >::block
                                       indent( os );

                  bool                 iof = true; // indent-on-first
                  size_type            n   = 1;
                  for( iterator i = begin(); i != end(); ++i, ++n )
                  {
                     bool              pre  = single ? false : iof;
                     ( *i ) -> save( os, pre, n != size());
                     iof = false;
                  }
               }

               if( !single )           os.indent( true );
               os << "</" << name << '>';
               if( post && single )    os.indent( true );
            }
         }
      }
   }

   // xmldom::document

   bool xmldom::document::load( std::string fn )
   {
      root -> clear();
      xmllexer                         xl;
      if( !xl.open( fn ))              return( false );
      
      try
      {
         for( bool quit = false; !quit; ) switch( xl.gettoken())
         {
            case token::eof: quit = true;                  break;
            default:         if( !root -> parse( xl ))
                                quit = true;
         }
      }
      catch( xmldom::exception & e )
      {
         std::cout << "[error]: " << e << '\n';
         root -> clear();
         return( false );
      }

      return( true );
   }

   void xmldom::document::save( std::string fn )
   {
      boost::detail::indentor< std::ofstream >   os( fn.c_str());
      bool                             iof = false;
      for( element::collection::iterator i = root -> begin(); i != root -> end(); ++i )
      {
         ( *i ) -> save( os, iof );
         iof = true;
      }
   }

#  if defined(__BORLANDC__)
#     pragma option pop
#  endif
#endif
