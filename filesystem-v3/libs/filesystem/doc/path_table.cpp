//  Generate an HTML table showing path decomposition  ---------------------------------//

//  Copyright Beman Dawes 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/filesystem for documentation.

//  For purposes of generating the table, support both POSIX and Windows paths

#include "boost/filesystem.hpp"
#include <iostream>
#include <fstream>

using boost::filesystem::path;
using std::string;
using std::cout;

namespace
{
  std::ifstream infile;
  std::ofstream outfile;

  const string empty_string;

  struct column_base
  {
    virtual string heading() const = 0;
    virtual string cell_value( const path & p ) const = 0;
  };

  struct c0 : public column_base
  {
    string heading() const { return string("<code>string()</code>"); }
    string cell_value( const path & p ) const { return p.string(); }
  } o0;

    struct c1 : public column_base
  {
    string heading() const { return string("<code>generic_<br>string()</code>"); }
    string cell_value( const path & p ) const { return p.generic_string(); }
  } o1;

  struct c2 : public column_base
  {
    string heading() const { return string("Iteration<br>over<br>Elements"); }
    string cell_value( const path & p ) const
    {
      string s;
      for( path::iterator i(p.begin()); i != p.end(); ++i )
      {
        if ( i != p.begin() ) s += ',';
        s += (*i).string();
      }
      return s;
    }
  } o2;

  struct c3 : public column_base
  {
    string heading() const { return string("<code>root_<br>path()</code>"); }
    string cell_value( const path & p ) const { return p.root_path().string(); }
  } o3;

  struct c4 : public column_base
  {
    string heading() const { return string("<code>root_<br>name()</code>"); }
    string cell_value( const path & p ) const { return p.root_name().string(); }
  } o4;

  struct c5 : public column_base
  {
    string heading() const { return string("<code>root_<br>directory()</code>"); }
    string cell_value( const path & p ) const { return p.root_directory().string(); }
  } o5;

  struct c6 : public column_base
  {
    string heading() const { return string("<code>relative_<br>path()</code>"); }
    string cell_value( const path & p ) const { return p.relative_path().string(); }
  } o6;

  struct c7 : public column_base
  {
    string heading() const { return string("<code>parent_<br>path(</code>"); }
    string cell_value( const path & p ) const { return p.parent_path().string(); }
  } o7;

  struct c8 : public column_base
  {
    string heading() const { return string("<code>filename()</code>"); }
    string cell_value( const path & p ) const { return p.filename().string(); }
  } o8;

  const column_base * column[] = { &o2, &o0, &o1, &o3, &o4, &o5, &o6, &o7, &o8 };

  //  do_cell  ---------------------------------------------------------------//

  void do_cell( const string & test_case, int i )
  {
    string value = column[i]->cell_value(path(test_case));
    if (value.empty())
      outfile << "<td><font size=\"-1\"><i>empty</i></font></td>\n";
    else
      outfile << "<td><code>" << value << "</code></td>\n";
  }

//  do_row  ------------------------------------------------------------------//

  void do_row( const string & test_case )
  {
    outfile << "<tr>\n";

    if (test_case.empty())
      outfile << "<td><font size=\"-1\"><i>empty</i></font></td>\n";
    else
      outfile << "<td><code>" << test_case << "</code></td>\n";
    
    for ( int i = 0; i < sizeof(column)/sizeof(column_base&); ++i )
    {
      do_cell( test_case, i );
    }

    outfile << "</tr>\n";
  }

//  do_table  ----------------------------------------------------------------//

  void do_table()
  {
    outfile <<
      "<h1>Path Decomposition Table</h1>\n"
      "<p>Shaded entries indicate cases where <i>POSIX</i> and <i>Windows</i>\n"
      "implementations yield different results. The top value is the\n"
      "<i>POSIX</i> result and the bottom value is the <i>Windows</i> result.\n" 
      "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">\n"
      "<p>\n"
      ;

    // generate the column headings

    outfile << "<tr><td><b>Constructor<br>argument</b></td>\n";

    for ( int i = 0; i < sizeof(column)/sizeof(column_base&); ++i )
    {
      outfile << "<td><b>" << column[i]->heading() << "</b></td>\n";
    }

    outfile << "</tr>\n";

    // generate a row for each input line

    string test_case;
    while ( std::getline( infile, test_case ) )
    {
      if (test_case.empty() || test_case[0] != '#')
        do_row( test_case );
    }

    outfile << "</table>\n";
  }

} // unnamed namespace

//  main  ------------------------------------------------------------------------------//

#define BOOST_NO_CPP_MAIN_SUCCESS_MESSAGE
#include <boost/test/included/prg_exec_monitor.hpp>

int cpp_main( int argc, char * argv[] ) // note name!
{
  if ( argc != 3 )
  {
    std::cerr <<
      "Usage: path_table input-file output-file\n"
      "  input-file contains the paths to appear in the table.\n"
      "  output-file will contain the generated HTML.\n"
      ;
    return 1;
  }

  infile.open( argv[1] );
  if ( !infile )
  {
    std::cerr << "Could not open input file: " << argv[1] << std::endl;
    return 1;
  }

  outfile.open( argv[2] );
  if ( !outfile )
  {
    std::cerr << "Could not open output file: " << argv[2] << std::endl;
    return 1;
  }

  outfile << "<html>\n"
          "<head>\n"
          "<title>Path Decomposition Table</title>\n"
          "</head>\n"
          "<body bgcolor=\"#ffffff\" text=\"#000000\">\n"
          ;

  do_table();

  outfile << "</body>\n"
          "</html>\n"
          ;

  return 0;
}
