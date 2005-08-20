// (C) Copyright 2003: Reece H. Dunn

// Produce the same formatting as the examples created by John Torjo for his
// list outputter library/tutorial

#include <iostream>  // std::cout
#include <list>      // std::list

#include <boost/outfmt/formatlist.hpp>

template< class FormatType >
class position_output // Create a Format Object that maintains position information
{
   public: // needed for deduction of the nested format type:
      typedef FormatType                                   format_type;
   private:
      long pos;
   public:
      template< typename T, class OutputStream >
      OutputStream &              operator()( OutputStream & os, const T & value ) const
      {
         // Uses a const_cast      -- makes code harder to read
         // Direct output of value -- cannot hook to a specialist formatter
         return( os << '[' << const_cast< position_output & >( *this ).pos++ << "] " << value );
      }
   public:
      position_output(): pos( 0 ){}
};

class position_state // Create a position state object
{
   // Note that there is no Format Object requirements, reducing user code complexity
   private:
      long pos;
   public:
      template< class OutputStream >
      OutputStream & operator()( OutputStream & os )
      {
         // No const conversion here; only outputting of the position information
         return( os << '[' << pos++ << "] " );
      }
   public:
      position_state():
         pos( 0 )
      {
      }
};

int main()
{
   // names - the data to be formatted

   std::list< std::string >            names;
   names.push_back( "John" );
   names.push_back( "James" );
   names.push_back( "Corina" );

   // [1]: John, James, Corina

   std::cout << "[1]: "
             << boost::io::formatlist( names ).format( "", "" )
             << '\n';

   // [2]: {John}, {James}, {Corina}

   std::cout << "[2]: "
             << boost::io::formatlistout
                (
                   names,
                   // use wrapped output to simplify construction:
                   boost::io::wrappedfmt().format( "{", "}" )
                ).format( "", "" )
             << '\n';

   std::cout << "[2]: "
             << boost::io::formatlist( names ).format( "{", "}", "}, {" )
             << '\n';

   // [3]: {[0] John}, {[1] James}, {[2] Corina}

   std::cout << "[3]: "
             << boost::io::formatlistout
                (
                   names,
                   boost::io::wrappedfmtout
                   (
                      // create an instance of a position Format Object:
                      position_output< char * >()
                   ).format( "{", "}" )
                )
                .format( "", "" )
             << '\n';

   std::cout << "[3]: "
             << boost::io::formatlistout
                (
                   names,
                   boost::io::wrappedfmtout
                   (
                      // state based version of the above
                      boost::io::statefmt< position_state, true >()
                   ).format( "{", "}" )
                )
                .format( "", "" )
             << '\n';

   // [4]: [0] John, [1] James, [2] Corina

   std::cout << "[4]: "
             << boost::io::formatlistout( names, position_output< char * >())
                .format( "", "" )
             << '\n' << '\n';

   /*
      [5]:
      [John]
      [James]
      [Corina]
   */

   std::cout << "[5]:"
             << boost::io::formatlistout( names, boost::io::wrappedfmt().format( "[", "]" ))
                .format( "\n", "", "\n" )
             << '\n';

   /*
      [6]:
      [0] John
      [1] James
      [2] Corina
   */

   std::cout << "[6]:"
             << boost::io::formatlistout( names, position_output< char * >())
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /*
      [7]:
      '[0] John'
      '[1] James'
      '[2] Corina'
   */

   std::cout << "[7]:"
             << boost::io::formatlistout( names, position_output< char * >())
                .format( "\n'", "'", "'\n'" )
             << '\n' << '\n';

   return( 0 );
}
