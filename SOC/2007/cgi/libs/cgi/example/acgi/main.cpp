#include <boost/cgi/acgi.hpp>
#include <boost/cgi/response.hpp>
#include <iostream>

// class handler
// {
// public:
//   handler(cgi::request_ostream& ros, cgi::acgi::request& req)
//     : ros_(ros)
//     , req_(req)
//   {
//   }
//
//   void operator()(boost::system::error_code& ec)
//   {
//     if (!ec)
//     {
//       ros_<< "All ok";
//       ros_.flush(req_);
//     }
//   }
// private:
//   cgi::request_ostream& ros_;
//   cgi::acgi::request& req_;
// };


int main()
{
  using namespace cgi::acgi;

  service srv;
  request req(srv);

  // Load up the request data
  req.load(true);

  response resp;

  if (req.get_("reset") == "true")
  {
    resp<< cookie("name")
        << location(req.script_name())
        << header();
    resp.send(req);
    return 0;
  }

  // First, see if they have a cookie set
  std::string& name = req.cookie_()["name"];
  if (!name.empty())
  {
    resp<< header("Content-type", "text/html")
        << header() // terminate the headers
        << "Hello again " << name << "<p />"
        << "<a href='?reset=true'><input type='submit' value='Reset' /></a>";
    resp.send(req);
    return 0;
  }

  // Now we'll check if they sent us a name
  name = req.form_("name");
  if (!name.empty())
  {
    resp<< header("Content-type", "text/html")
        << cookie("name", name)
        << header() // terminate the headers
        << "Hello there, " << name;
    resp.send(req);
    return 0;
  }

  //std::string buf("Content-type: text/html\r\n\r\nHello there, Universe.<br />");
  //cgi::write(req, cgi::buffer(buf.c_str(), buf.size()));

  resp<< header("Content-type", "text/html")
      << header()
      << "Hello there, Universe.<p />"
      << "What's your name?<br />";

  //std::cerr<< std::endl << "name = " << req.form_("name") << std::endl;
  //std::cerr.flush();

  resp<< "<form method='POST'>"
         "<input name='name' type='text' value='" << req.form_("name") << "'>"
         "</input>"
         "<input type='submit'></input>"
         "</form>";

  resp.send(req);

  return 0;
}
