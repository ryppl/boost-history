// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_XMLDOM_LITE_XMLLexer_CPP
#define BOOST_XMLDOM_LITE_XMLLexer_CPP
#  if defined(BOOST_MSVC)
#     pragma warning( push )
#     pragma warning( disable : 4706 ) // assignment within conditional expression
#     pragma warning( disable : 4800 ) // 'void *' : forcing value to bool 'true' or 'false' (performance warning)
#  elif defined(__BORLANDC__)
      // W8060: Possibly incorrect assignment in function <function>
      // W8004: '<variable>' is assigned a value that is never used in function <function>
#     pragma option push -w-8060 -w-8004
#  endif

#  include "../xmllexer.hpp"

   xmldom::token::type xmldom::xmllexer::gettoken()
   {
      ptok = tok;
      tok  = token::none;

           if( eof())                  tok = token::eof;
      else if(( ptok == token::cdataBegin ) || ( ptok == token::commentBegin ))
      {
         gettext();
         getch();
      }
      else while( tok == token::none )
      {
         dolex();
         getch();
      }

#     if defined(BOOST_XMLDOMLITE_DUMP_TOKENSTREAM)
         std::cout << *this << '\n'; // token dump
#     endif
      return( tok );
   }

   void xmldom::xmllexer::dolex()
   {
      switch( cch )
      {
         case '<':
         {
            in_element = true;
            char                       ch2 = getch();
            switch( ch2 )
            {
               case '!':
               {
                  in_element = false;
                  getch();
                  if( match( "[CDATA[", 7 ))
                     tok = token::cdataBegin;
                  else if( match( "--", 2 ))
                     tok = token::commentBegin;
                  else
                     error();
               } break;
               case '?':     tok = token::piBegin;         break;
               case '/':     tok = token::elementEndOpen;  break;
               default:      tok = token::elementBeginOpen;
                             putback( ch2 );
           }
         } break;
         case '>':
            tok = token::elementClose;
            in_element = false;
            break;
         case '=': tok = token::equal;                     break;
         case '?': case '/':
         {
            char                       ch = cch;
            if( getch() == '>' )
            {
               tok = ( ch == '?' ) ? token::piEnd : token::elementSingleClose;
               in_element = false;
            }
            else                       error();
         } break;
         case '-': case ']':
         {
            char                       ch = cch;
            if( getch() == ch )
            {
               if( getch() == '>' )
                  tok = ( ch == '-' ) ? token::commentEnd : token::cdataEnd;
               else                    error();
            }
            else                       error();
         } break;
         case ' ': case '\r': case '\n': case '\t': // skipws
            break;
         case '"': case '\'':
         {
            char                       ch = cch;
            data.erase();

            char                       ch2 = 0;
            while(( ch2 = getch()) && ( ch2 != ch ))
               data.push_back( ch2 );

            if( ch2 != ch )            error();
            else                       tok = token::string;
         } break;
         default:
            gettext();
      }
   }

   void xmldom::xmllexer::error( const char * err )
   {
      tok = token::none;
      throw( xmldom::exception( err, filename, line ));
   }

   void xmldom::xmllexer::gettext()
   {
      putback( cch );
      data.erase();

      if( in_element )
      {
         while( getch() != ' ' )
         {
            if( cch == '>' )      break;
            if( cch == '/' )      break;
            data.push_back( cch );
         }

         switch( cch )
         {
            case '>': case '/':  putback( cch );
            case ' ':            tok = token::name;        break;
            default:             error();
         }
      }
      else
      {
         bool                          quit = false;
         char                          ch   = 0;
         while( ch = getch())
         {
            switch( ptok )
            {
               case token::cdataBegin:
                  if( match( "]]>", 3, true ))   quit = true;
                  break;
               case token::commentBegin:
                  if( match( "-->", 3, true ))   quit = true;
                  break;
               default:
                  if( cch == '<' )
                  {
                     quit = true;
                     putback( cch );
                  }
                  break;
            }
            if( quit )                 break;
            data.push_back( ch );
         }

         tok = token::characterData;
      }
   }

#  define _case                                  break; case

   std::ostream & xmldom::operator<<( std::ostream & os, const xmldom::xmllexer & xl )
   {
      os << "token == ";
      switch( xl.tok )
      {
         case xmldom::token::none:               os << "none";
        _case xmldom::token::eof:                os << "eof";
        _case xmldom::token::cdataBegin:         os << "cdata::begin";
        _case xmldom::token::cdataEnd:           os << "cdata::end";
        _case xmldom::token::commentBegin:       os << "comment::begin";
        _case xmldom::token::commentEnd:         os << "comment::end";
        _case xmldom::token::piBegin:            os << "pi::begin";
        _case xmldom::token::piEnd:              os << "pi::end";
        _case xmldom::token::elementBeginOpen:   os << "element-begin::open";
        _case xmldom::token::elementEndOpen:     os << "element-end::open";
        _case xmldom::token::elementClose:       os << "element::close";
        _case xmldom::token::elementSingleClose: os << "element::close-single";
        _case xmldom::token::equal:              os << "equal";
        _case xmldom::token::string:             os << "string; value = "         << xl.data;
        _case xmldom::token::name:               os << "name; value = "           << xl.data;
        _case xmldom::token::characterData:      os << "character-data; value = " << xl.data;
      }
      return( os );
   }

#  undef _case

#  if defined(BOOST_MSVC)
#     pragma warning( pop )
#  elif defined(__BORLANDC__)
#     pragma option pop
#  endif
#endif
