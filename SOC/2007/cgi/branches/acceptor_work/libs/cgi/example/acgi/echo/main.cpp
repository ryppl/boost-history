#include <boost/cgi/acgi.hpp>

using namespace std;
using namespace boost::acgi;

template<typename MapT, typename OStreamT>
void show_map_contents(MapT& m, OStreamT& os, const std::string& title)
{
  os<< "<h3>" << title << "</h3>";
  if (m.empty()) os<< "NONE<br />";
  for (MapT::iterator i = m.begin(); i != m.end(); ++i)
  {
    os<< "<b>" << i->first << "</b> = <i>" << i->second << "</i><br />";
  }
}

int main()
{
  service s;
  request req(s);

  req.load(true);

  response resp;

  show_map_contents(req.env(), resp, "Environment Variables");
  show_map_contents(req.GET(), resp, "GET Variables");
  show_map_contents(req.POST(), resp, "POST Variables");
  show_map_contents(req.cookie(), resp, "Cookie Variables");

  resp<< content_type("text/html");
  resp.send(req.client());

  return 0;
}
