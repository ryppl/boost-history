
#define BOOST_LOG_COMPILE_FAST_OFF
#include <boost/logging/format/named_write_fwd.hpp>


BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )
using namespace boost::logging;

typedef logger_format_write< default_, default_, writer::threading::no_ts > logger_type;


BOOST_DECLARE_LOG_FILTER(g_log_filter, filter::no_ts )
BOOST_DECLARE_LOG(g_l, logger_type) 

#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 

BOOST_DEFINE_LOG(g_l, logger_type)
BOOST_DEFINE_LOG_FILTER(g_log_filter, filter::no_ts )



// Example of custom formatter:
// dump the no. of seconds since start of program
struct secs_since_start : formatter::class_<secs_since_start, formatter::implement_op_equal::no_context > {
    ::time_t m_start;
    secs_since_start() : m_start( ::time(0) ) {}
    void operator()(param str) const {
        ::time_t now = ::time(0);
        std::stringstream out;
        out << "+" << (int)(now-m_start) << "s ";
        str.prepend_string( out.str() );
    }
};

// Example of custom destination:
// Dump each message as XML
struct as_xml : 
        destination::class_<as_xml, destination::implement_op_equal::has_context>, 
        destination::non_const_context<std::ofstream> {

    std::string m_name;
    as_xml(const char* name) : non_const_context_base(name), m_name(name) {}
    void operator()(param str) const {
        context() << "<msg>" << str << "</msg>" << std::endl; 
    }

    bool operator==(const as_xml& other) const { return m_name == other.m_name; }
};

#if 0
int main() {
    secs_since_start s;
    secs_since_start::raw_param a;
    std::cout << a;
}
#endif

#if 1
void custom_fmt_dest_example() {
    //         add formatters and destinations
    //         That is, how the message is to be formatted and where should it be written to
    g_l()->writer().add_formatter( formatter::idx(), "[%] " );
    g_l()->writer().add_formatter( formatter::append_newline() );
    g_l()->writer().add_formatter( secs_since_start() );

    g_l()->writer().add_destination( destination::cout() );
    g_l()->writer().add_destination( destination::dbg_window() );
    g_l()->writer().add_destination( as_xml("out.txt") );
    g_l()->mark_as_initialized();

    int i = 1;
    L_ << "this is so cool " << i++;
    L_ << "this is so cool again " << i++;

    std::string hello = "hello", world = "world";
    L_ << hello << ", " << world;

    g_log_filter()->set_enabled(false);
    L_ << "this will not be written to the log";
    L_ << "this won't be written to the log";

    g_log_filter()->set_enabled(true);
    L_ << "good to be back ;) " << i++;
}



int main() {
    custom_fmt_dest_example();
}

#endif
// End of file
