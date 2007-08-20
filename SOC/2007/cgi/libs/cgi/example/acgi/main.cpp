#include <boost/cgi/acgi.hpp>
#include <iostream>

class handlerer
{
public:
  handlerer(cgi::request_ostream& ros, cgi::acgi_request& req)
    : ros_(ros)
    , req_(req)
  {
  }

  void operator()(boost::system::error_code& ec)
  {
    if (!ec)
    {
      ros_<< "All ok";
      ros_.flush(req_);
    }
  }
private:
  cgi::request_ostream& ros_;
  cgi::acgi_request& req_;
};


int main()
{
  cgi::acgi_service service;
  cgi::acgi_request req(service);
  req.load();

  std::cout<< "checking in";

  std::string buf("Content-type: text/html\r\n\r\nHello there, Universe.");
  cgi::write(req, cgi::buffer(buf.c_str(), buf.size()));

  std::cout<< "checking reply";

  cgi::reply rep;

  rep<< "Content-type: <br />Yuppers.";

  rep<< "<form method='POST'>"
        "<input name='name' type='text' value='"
     << req.meta_get("name")
     << "'></input>"
        "<input type='submit'></input>"
        "</form>"
    ;

  rep.send(req);



  /*
  for(cgi::streambuf::const_buffers_type::const_iterator 
        i = rep.rdbuf()->data().begin()
     ; i != rep.rdbuf()->data().end(); ++i)
  {
    std::size_t buf_len = boost::asio::buffer_size(*i);
    std::string s(boost::asio::buffer_cast<const char*>(*i)
                 , buf_len);

    rep<< "s = " << s;
    std::cerr<< "s = " << s;
  }
  */

  rep.flush(req);

  return 0;
}
