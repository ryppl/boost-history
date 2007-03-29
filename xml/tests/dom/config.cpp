#include <boost/xml/dom.hpp>
#include "string.hpp"
#include <iostream>

namespace dom = boost::xml::dom;

int main(int argc, char **argv)
{
  try
  {
    document_ptr document(new dom::document<std::string>());
    element_ptr root = document->create_root("config");
    dtd_ptr doc_type = document->create_internal_subset("foo", "bar", "baz");
    document->write_to_file("config.xml");
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error : " << e.what() << std::endl;
  }
}
