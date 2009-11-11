//  error_demo.cpp  --------------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------//
//                                                                                      //
//  The purpose of this program is to demonstrate how error reporting works.            //
//                                                                                      //
//--------------------------------------------------------------------------------------//

#include <boost/filesystem.hpp>
#include <boost/system/system_error.hpp>
#include <iostream>

using std::cout;
using boost::filesystem::path;
using boost::filesystem::filesystem_error;
using boost::system::error_code;
using boost::system::system_error;
namespace fs = boost::filesystem;

namespace
{
  void report_system_error(const system_error& ex)
  {
    cout << " threw system_error:\n"
         << "    ex.code().value() is " << ex.code().value() << '\n'
         << "    ex.code().category().name() is " << ex.code().category().name() << '\n'
         << "    ex.what() is " << ex.what() << '\n'
         ;
  }

  void report_filesystem_error(const system_error& ex)
  {
    cout << "  Threw filesystem_error exception:\n"
         << "    ex.code().value() is " << ex.code().value() << '\n'
         << "    ex.code().category().name() is " << ex.code().category().name() << '\n'
         << "    ex.what() is " << ex.what() << '\n'
         ;
  }

  void report_status(fs::file_status s)
  {
    cout << "file_status::type() is ";
    switch (s.type())
    {
    case fs::status_error:
      cout << "status_error\n"; break;
    case fs::file_not_found:
      cout << "file_not_found\n"; break;
    case fs::regular_file:
      cout << "regular_file\n"; break;
    case fs::directory_file:
      cout << "directory_file\n"; break;
    case fs::symlink_file:
      cout << "symlink_file\n"; break;
    case fs::block_file:
      cout << "block_file\n"; break;
    case fs::character_file:
      cout << "character_file\n"; break;
    case fs::fifo_file:
      cout << "fifo_file\n"; break;
    case fs::socket_file:
      cout << "socket_file\n"; break;
    case fs::type_unknown:
      cout << "type_unknown\n"; break;
    default:
      cout << "not a valid enumeration constant\n";
    }
  }

  void report_error_code(const error_code& ec)
  {
    cout << "  ec:\n"
         << "    value() is " << ec.value() << '\n'
         << "    category().name() is " << ec.category().name() << '\n'
         << "    message() is " <<  ec.message() << '\n'
         ;
  }

  bool threw_exception;

}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: error_demo path\n";
    return 1;
  }

  error_code ec;

  //  construct path - no error_code

  try { path p1(argv[1]); }
  catch (const system_error& ex)
  {
    cout << "construct path without error_code";
    report_system_error(ex);
  }

  //  construct path - with error_code


  path p(argv[1]);

  //  get status - no error_code

  fs::file_status s;

  cout << "status(argv[1]);\n";
  threw_exception = false;

  try { s = fs::status(p); }
  catch (const system_error& ex)
  {
    report_filesystem_error(ex);
    threw_exception = true;
  }
  if (!threw_exception)
    cout << "  Did not throw exception\n";
  report_status(s);

  //  get status - with error_code

  cout << "status(argv[1], ec);\n";
  s = fs::status(p, ec);
  report_status(s);
  report_error_code(ec);

  //  query existence - no error_code

  //  query existence - with error_code

  return 0;
}
