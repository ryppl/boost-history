#include <boost/xml/dom.hpp>
#include "string.hpp"
#include <iostream>

namespace dom = boost::xml::dom;

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: example <xml-file>" << std::endl;
    return -1;
  }
  try
  {
    // create a document from a file
    document_ptr document = dom::parse_file<std::string>(argv[1], false);
    element_ptr root = document->root();
    {
      xpath path("//xi:include");
      path.declare_namespace("xi", "http://www.w3.org/2003/XInclude");
      std::cout << "searching for xi:include nodes: ";
      node_set set = path.find(root);
      std::cout << set.size() << " nodes have been found." << std::endl;
    }
    // process xinclude nodes in-place
    std::cout << "process xinclude: " << std::endl;
    dom::process_xinclude(*document);
    {
      xpath path("//section");
      std::cout << "searching for sections: ";
      node_set set = path.find(root);
      std::cout << set.size() << " nodes have been found." << std::endl;
      for (node_set::iterator i = set.begin(); i != set.end(); ++i)
      {
	//...and print their structural path
 	std::cout << " " << (*i)->path() << std::endl;
      }
    }
    std::cout << "writing modified document to 'book2.xml'..." << std::endl;
    document->write_to_file("book2.xml");
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error : " << e.what() << std::endl;
  }
}
