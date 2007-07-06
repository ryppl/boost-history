// Copyright 2007 Stefan Seefeld.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/xml/reader.hpp>

using namespace boost::xml::reader;

int main(int, char **)
{
  try
  {
    parser<std::string> parser("article.xml");
    while (parser.next())
    {
      token_base<std::string> const &current = parser.get_token();
      std::cout << current.depth() << ' ' << current.name() << ' ' << current.value() << ' '
                << '(' << current.type() << ')' << std::endl;
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
