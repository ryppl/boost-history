#ifndef CGI_ASYNC_CGI_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_ASYNC_CGI_SERVICE_IMPL_HPP_INCLUDED__

namespace cgi {

  class async_cgi_service_impl
    : public boost::asio::io_service::service
    , public cgi_service_impl<async_cgi_request_impl>
  {
  public:
    typedef tags::async_cgi                         protocol_type;
    typedef typename protocol_type::service_type    protocol_service_type;

    /// The unique service identifier
    static boost::asio::io_service::id id;

    explicit async_cgi_service_impl(protocol_service_type& protocol_service)
      : boost::asio::io_service::service(protocol_service.io_service())
      , cgi_service_impl<async_cgi_request_impl>()
    {
    }

    void shutdown_service()
    {
    }

    /// Construct a new cgi request implementation
    void construct(impl_type& impl)
    {
    }

    template<typename ImplType, typename Handler>
    class load_handler
    {
    public:
      load_handler(impl_type& impl, bool parse_stdin, Handler handler)
	: impl_(impl)
	, service_(impl.service_)
	, work_(service.io_service())
	, parse_stdin_(parse_stdin)
	, handler_(handler)
      {
      }

      void operator()(const boost::system::error_code& error)
      {
	if (error)
	  service_.post(boost::bind(&Handler, handler_, error));
	
      }
    private:
      protocol_service_type& service_;
      protocol_service_type::work work_;
      Handler handler_;
      ImplType impl_;
    };

    /// Asynchronously read/parse the request meta-data
    /**
     * @param parse_stdin if true then STDIN data is also read/parsed
     */
    template<typename Handler>
    void async_load(impl_type& impl, bool parse_stdin, Handler handler)
    {
      impl.service_.post(boost::bind(&load_handler<impl_type, Handler>
                                       (impl, parse_stdin, handler)));
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(impl_type& impl, const MutableBufferSequence& buf
                        , Handler handler)
    {
      impl.connection_->async_read_some(buf, handler);
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(impl_type& impl, const ConstBufferSequence& buf
                         , Handler handler)
    {
      impl.connection_->async_write_some(buf, handler);
    }
  };

#endif // CGI_ASYNC_CGI_SERVICE_IMPL_HPP_INCLUDED__
