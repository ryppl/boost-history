// Copyright 2007 Stefan Seefeld.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/xml/dom.hpp>
#include "string.hpp"
#include <fstream>

namespace dom = boost::xml::dom;

int main(int argc, char **argv)
{
  try
  {
    document_ptr document(new dom::document<std::string>());
    element_ptr root = document->create_root("config");
    dtd_ptr doc_type = document->create_internal_subset("foo", "bar", "baz");
    std::ofstream ofs("config.xml");
    write(*document, ofs.rdbuf());
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error : " << e.what() << std::endl;
  }
}
