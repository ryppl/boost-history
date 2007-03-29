#include <boost/xml/reader.hpp>
#include <boost/variant.hpp>

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
