// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_XMLDOM_LITE_XMLLexer_HPP
#define BOOST_XMLDOM_LITE_XMLLexer_HPP
#  include <fstream>        // std::ifstream
#  include <stack>          // std::stack
#  include "exception.hpp"  // xmldom::exception
#  include "filestream.hpp" // xmldom::detail::filestream

   // BOOST_XMLDOMLITE_DUMP_TOKENSTREAM: dumps the token stream to std::cout
   // [todo]: normalize text() nodes?
   // [bug]:  when the cdata section ending is of the form ] ... ]]> | trace? :D

//#  define BOOST_XMLDOMLITE_DUMP_TOKENSTREAM

   namespace xmldom
   {
      // XML tokens

      namespace token { enum type
      {
         none,
         eof,

         // character sequences:
         cdataBegin,           // <![CDATA[
         commentBegin,         // <!--
         piBegin,              // <?
         elementBeginOpen,     // <
         elementEndOpen,       // </

         cdataEnd,             // ]]>
         commentEnd,           // -->
         piEnd,                // ?>
         elementClose,         // >
         elementSingleClose,   // />

         equal,                // =

         // text based elements:
         string,               // "..." or '...'
         name,                 // attribute or element name
         characterData         // text inside a cdata, comment or text element
      };}

      // xmldom::xmllexer -- convert a file to a token stream

      class xmllexer: public detail::filestream
      {
         public: // parser interface:
            token::type                tok;
            token::type                ptok;
            std::string                data;
         public:
            token::type                          gettoken();
            void                                 error( const char * = "invalid token" ); // throws( xmldom::exception )
         public:
            inline bool           operator!() const
            {
               return( eof());
            }
         private: // implementation details:
            bool                       in_element;
         private:
            void                                 dolex();
            void                                 gettext();
         private: // noncopyable:
            inline xmllexer & operator=( const xmllexer & );
            inline xmllexer( const xmllexer & );
         public: // constructor:
            inline                     xmllexer(): detail::filestream(),
               tok(  token::none ),
               ptok( token::none ),
               in_element( false )
            {
            }
      };

      std::ostream & operator<<( std::ostream &, const xmllexer & );
   }
#endif
