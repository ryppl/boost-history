//  (C) Copyright Giovanni P. Deretta 2005. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/coroutine/future.hpp>
 
typedef boost::asio::ip::tcp::acceptor acceptor_type;
typedef boost::asio::ip::tcp::endpoint endpoint_type;

boost::asio::io_service demuxer;

// Do something, calculate xor of the buffer.
void frob(char * begin, size_t len) {
  char first = begin[0];
  for(size_t i=0; i<len -1; i++)
    begin[i] ^=begin[i+1];
  begin[len-1] ^= first;
}

typedef boost::coroutines::coroutine<void(void)> thread_type;
namespace coro = boost::coroutines;

typedef boost::asio::error error_type;
void thread(thread_type::self& self,
	    acceptor_type* acceptor, 
	    endpoint_type* endpoint,
	    int index, int counter, int token_size){
  // std::cout<<"running\n";
  boost::shared_ptr<char> token_(new char[token_size]);
  char * token = token_.get();
  for(int i=0; i<token_size; i++) token[i] = 0;
  boost::asio::ip::tcp::socket sink(acceptor->io_service());
  boost::asio::ip::tcp::socket source(acceptor->io_service());
  coro::future<error_type> accept_error(self);
  coro::future<error_type> connect_error(self);

  acceptor->async_accept(sink, 
			 coro::make_callback(accept_error));

  source.async_connect(*endpoint, 
		       coro::make_callback(connect_error));

  coro::wait_all(connect_error, accept_error);

  BOOST_ASSERT(connect_error);
  BOOST_ASSERT(accept_error);
 

  coro::future<error_type, std::size_t> read_error(self);
  coro::future<error_type, std::size_t> write_error(self);
  
  boost::asio::async_read(source,
			  boost::asio::buffer(token, token_size),
			  coro::make_callback(read_error));

  boost::asio::async_write(sink,
			   boost::asio::buffer(token, token_size),
			   coro::make_callback(write_error));

  while(counter) {
    //   std::cout<<counter<<std::endl;
    
    if(write_error) {
      if(write_error->get<0>()) {
	// std::cout<<"[Thread "<<index<<"] :error while writing, exiting..."<<std::endl;
	break;
      }
      // std::cout<<"[Thread "<<index<<"] :token sent"<<std::endl;
      write_error = boost::none; 

      frob(token, token_size);
      // std::cout<<"[Thread "<<index<<"] :sending token"<<std::endl;
      boost::asio::async_write(sink,
			       boost::asio::buffer(token, token_size),
			       coro::make_callback(write_error));
      counter--;

    }
 
    if(read_error) {
      if(read_error->get<0>()) {
	// std::cout<<"[Thread "<<index<<"] :error while readin, exiting..."<<std::endl;
	break;
      }
      // std::cout<<"[Thread "<<index<<"] :token read"<<std::endl;
      read_error = boost::none;   
      // std::cout<<"[Thread "<<index<<"] :reading token"<<std::endl;
      boost::asio::async_read(source,
			      boost::asio::buffer(token, token_size),
			      coro::make_callback(read_error));
    } 
 
    BOOST_ASSERT(!(read_error || write_error));
    coro::wait(read_error, write_error);
  }
   
  sink.close();  
  //wait for all pending activities
  coro::wait_all(read_error, write_error);
  return;
}
 
typedef std::vector<boost::shared_ptr<acceptor_type> > acceptor_vector_type;
typedef std::vector< endpoint_type > endpoint_vector_type;

int main(int argc, char** argv) {
  int count = ((argc >= 2) ? boost::lexical_cast<int>(argv[1]): 100);
  int count_2 = ((argc >= 3) ? boost::lexical_cast<int>(argv[2]): 1);
  int base_port = ((argc >= 4) ? boost::lexical_cast<int>(argv[3]): 30000);
  int token_size = ((argc == 5) ? boost::lexical_cast<int>(argv[4]): 4096);

  acceptor_vector_type acceptor_vector;
  endpoint_vector_type endpoint_vector;

  acceptor_vector.reserve(count_2);
  endpoint_vector.reserve(count_2);

  for(int i= 0; i< count_2; i++) {
    endpoint_vector.push_back
      (endpoint_type
       (boost::asio::ip::address_v4::from_string("127.0.0.1"),
	base_port + i));
    acceptor_vector.push_back
      (boost::shared_ptr<acceptor_type> 
       (new acceptor_type(demuxer)));
    acceptor_vector.back()->open(endpoint_vector.back().protocol());
    acceptor_vector.back()->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_vector.back()->bind(endpoint_vector.back());
    acceptor_vector.back()->listen();
  }

  post(thread_type(boost::bind(thread, 
			       _1, 
			       &*acceptor_vector.back(), 
			       &endpoint_vector.back(),
			       0,
			       count,
			       token_size)), demuxer);
  for(int i=1; i< count_2; i++) {
    post(thread_type(boost::bind(thread, 
				 _1, 
				 &*acceptor_vector.at(i-1), 
				 &endpoint_vector.at(i-1),
				 i,
				 count, 
				 token_size)), demuxer);
  }
  demuxer.run(); 
 } 
 
