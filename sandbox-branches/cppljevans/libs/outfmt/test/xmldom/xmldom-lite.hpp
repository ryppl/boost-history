// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XMLDOM_LITE_XMLDOMLite_HPP
#define BOOST_XMLDOM_LITE_XMLDOMLite_HPP
#  include <boost/shared_ptr.hpp>
#  include <fstream>
#  include <list>

#  include "indent.hpp"
#  include "xmllexer.hpp"

   // [todo]: output escaping for text data (i.e. comment() and text(), but not cdata())

   namespace xmldom
   {
      namespace nodetype { enum type
      {
         element,
         document,
         ProcessingInstruction,
         text,
         comment,
         CDATASection
      };}

      // xmldom::attribute_list

      typedef std::pair< std::string, std::string >                  attribute;

      class attribute_list: public std::list< attribute >
      {
         public:
            std::string &         operator[]( const std::string );
         public:
            bool                       find( const std::string );
      };

      // xmldom::element

      class   element;
      typedef boost::shared_ptr< element >                           element_ptr;

      class element: public std::list< element_ptr >
      {
         public:
            typedef std::list< element_ptr >                         collection;
         public:
            std::string                name;
            nodetype::type             type;
            attribute_list             attr;
         public:
            bool                                 parse( xmllexer & );
            void                                 save(  boost::detail::indentor< std::ofstream > &, bool = true, bool = true );
         public:
            inline element_ptr                   append( std::string n, nodetype::type nt = nodetype::element )
            {
               push_back( value_type( new element( n, nt )));
               return( back());
            }
         public:
            inline           element( std::string n, nodetype::type nt = nodetype::element ):
               name( n ),
               type( nt )
            {
            }
      };

      // xmldom::document

      class document
      {
         public:
            element_ptr                root;
         public:
            void                                 save( std::string );
            bool                                 load( std::string );
         public:
            inline           document(): root( new element( "#document", nodetype::document ))
            {
            }
      };
   }
#endif
