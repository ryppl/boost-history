#ifndef CGI_BASIC_SERVICE_HPP_INCLUDE__
#define CGI_BASIC_SERVICE_HPP_INCLUDE__

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace cgi {

  /*
   * This should inherit another object, such as service_base, which takes the
   * protocol as a template arguement. Things like the mutexes don't need to be
   * here for standard CGI.
   *
   * The question is whether the standard CGI service is so different from the
   * more general network-interface case that the whole class should
   * specialised for it (it probably should be).
   */
  template< typename Protocol >
  class basic_service
    : public boost::asio::io_service::service
    , private boost::noncopyable
  {
  public:
    typedef Protocol                       protocol;
    typedef basic_request<Protocol>        request_type;
    typedef basic_service<Protocol>        type;
    typedef service_options<Protocol>      service_options; // just an idea

    template< typename Handler >
    explicit basic_service(Handler handler, service_options& opts)
      : work_io_service()
      , boost::asio::io_service::service(work_io_service_)
      , work_(new boost::asio::io_service::work(work_io_service_))
      , gateway_(this, opts.max_concurrent_connections)
    {
    }

    /**
     * If the user passes in an asio::io_service, set that as the 'owner'
     * instead of the (internal) work_io_service_ instance.
     */
    template< typename Handler >
    explicit basic_service( boost::asio::io_service& ios, Handler handler
                          , service_options& opts )
      : work_io_service()
      , boost::asio::io_service::service(ios)
      , work_(new boost::asio::io_service::work(work_io_service_))
      , gateway_(this, opts.max_concurrent_connections)
    {
    }

    /// Block until a request can be returned
    /**
     * This function is called in the construction of a @code basic_request
     * @endcode, or a general @code request @endcode. It either returns a
     * valid request from the request queue immediately, or blocks on a
     * condition variable.
     *
     * Before blocking, if there are available connection slots then another
     * is @code accept() @endcode ed.
     */
    request_type& get_request()
    {
      boost::mutex::scoped_lock lk(mutex_);
      if( !request_queue_.empty() )
      {
        request_type& req = request_queue_.front();
        request_queue_.pop();
        return req;
      }
      // see gateway::accept() reference about this
      if( gateway_.accept() )
      {
        // parse the request now and return it
      }
      cond_.wait(lk);
      if( !request_queue_.empty() )
      {
        request_type& req = request_queue_.front();
        request_queue_.pop();
        return req;
      }
    }

  private:
    static boost::asio::io_service::id id;
    boost::asio::io_service work_io_service_;
    boost::scoped_ptr<boost::io_service::work> work_;

    gateway gateway_;


    boost::mutex mutex_;
    boost::condition cond_;
  };

} // namespace cgi

#endif // CGI_BASIC_SERVICE_HPP_INCLUDE__

