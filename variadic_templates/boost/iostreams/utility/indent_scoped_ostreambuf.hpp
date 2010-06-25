#ifndef BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED
#define BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED

#include <boost/iostreams/filter/indent.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace iostreams {

template<typename Ch>
struct select_scoped_ostreambuf
;
template<>
struct select_scoped_ostreambuf<char>
{
    typedef filtering_ostreambuf type;
};
template<>
struct select_scoped_ostreambuf<wchar_t>
{
    typedef filtering_wostreambuf type;
};

//
// Template name: indent_scoped_ostreambuf.
// Description: Temporarily replaces the ostreambuf of ostream 
//   passed to CTOR. DTOR restores original ostreambuf.
// Template paramters:
//      Ch - The charactertype.
//
template<typename Ch=char>
class indent_scoped_ostreambuf 
{
 public:
    typedef std::basic_streambuf<Ch> std_buf_type;
    typedef std::basic_ostream<Ch> std_ostrm_type;
    typedef typename select_scoped_ostreambuf<Ch>::type filt_buf_type;
    
    indent_scoped_ostreambuf
    ( std_ostrm_type& a_ostrm
    , int width=2
    )
      : my_old_buf(a_ostrm.rdbuf())
      , my_strm(a_ostrm)
      , my_filt_buf(*my_old_buf,width)
    {
        my_strm.rdbuf(&my_filt_buf);
    }
    
    ~indent_scoped_ostreambuf(void)
    {
        my_strm.rdbuf(my_old_buf);
    }

    // Class name: push_filt_strmbuf
    // Description: streambuf allowing user to indent
    //   the stream (by calling filt_get and then
    //   calling filt_type::indent_in or filt_type::indent_out).
    struct push_filt_strmbuf
    : public filt_buf_type
    {
      typedef indent_filter<Ch> filt_type;
      
      push_filt_strmbuf
      ( std_buf_type& a_buf
      , int width=2
      )
      {
          filt_type my_filter(width);
          this->push(my_filter,0,0);
          this->push(a_buf);
      }
      
      filt_type*filt_get(void)
      {
          filt_type*const my_filter = this->template component<filt_type>(0);
          return my_filter;
      }
       
    };
    
 private:
    std_buf_type* my_old_buf;//For DTOR.
    
    std_ostrm_type& my_strm;//For DTOR.
    
    push_filt_strmbuf my_filt_buf;//The temporary replacement buffer.
};

} } // End namespaces iostreams, boost.

// Function name: indent_buf
// Descrption:: Indents the buffer of ostream argument, if possible.
//
template<class charT, class traits>
inline std::basic_ostream<charT, traits>&
indent_buf_in(std::basic_ostream<charT, traits>& os)
{
    typedef boost::iostreams::indent_scoped_ostreambuf<charT> filt_scoped_type;
    typedef typename filt_scoped_type::push_filt_strmbuf filt_strmbuf_type;
    filt_strmbuf_type*buf_ptr = dynamic_cast<filt_strmbuf_type*>(os.rdbuf());
    if (buf_ptr) {
        typedef typename filt_strmbuf_type::filt_type filt_type;
        filt_type* filt_ptr=buf_ptr->filt_get();
        filt_ptr->indent_in(); 
    }
    return os;
}

// Function name: undent_buf
// Descrption:: Indents outwardly the buffer of ostream argument, if possible.
//
template<class charT, class traits>
inline std::basic_ostream<charT, traits>&
indent_buf_out(std::basic_ostream<charT, traits>& os)
{
    typedef boost::iostreams::indent_scoped_ostreambuf<charT> filt_scoped_type;
    typedef typename filt_scoped_type::push_filt_strmbuf filt_strmbuf_type;
    filt_strmbuf_type*buf_ptr = dynamic_cast<filt_strmbuf_type*>(os.rdbuf());
    if (buf_ptr) {
        typedef typename filt_strmbuf_type::filt_type filt_type;
        filt_type* filt_ptr=buf_ptr->filt_get();
        filt_ptr->indent_out(); 
    }
    return os;
}

#endif // #ifndef BOOST_IOSTREAMS_INDENT_SCOPED_OSTREAMBUF_HPP_INCLUDED
